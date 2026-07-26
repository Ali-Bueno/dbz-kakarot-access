<#
.SYNOPSIS
  Validates the mod's Lua before it can ship. Exits non-zero on any violation.

.DESCRIPTION
  Three checks, because each one catches a class the others cannot:

    1. SYNTAX      - luac -p. Table stakes.

    2. GLOBALS     - `luac -l -p` and inspect every _ENV access. This is the check that
                     matters most and the one a syntax pass cannot do: `luac -p` happily
                     accepts a local used ABOVE its own declaration. That compiles to a
                     global read, is nil at runtime, and in a polling mod it raises from
                     ABOVE the loop's pcall on every tick - i.e. a mod that is silent from
                     boot with nothing in the log. It nearly shipped once (2026-07-25).

    3. GUARDS      - the crash class this codebase keeps rediscovering. On this engine a
                     member fetch on a freed object, a fetch of a member the class does not
                     declare, and GetArrayNum on a fixed C array are all UNCATCHABLE aborts:
                     they unwind below the Lua boundary, so no pcall contains them and the
                     process dies with no traceback. UE4SS's own `IsValid()` cannot be the
                     guard either - it dereferences the object BEFORE the lookup that would
                     have rejected it, so it faults on exactly the handles it exists to
                     catch. The substrate provides the safe forms; everything else must use
                     them. Every fix in this area has been undone later by a new call site
                     written the old way, which is why this is enforced mechanically and not
                     by a rule in a document.

  Run it after ANY Lua edit, and from package.ps1 before staging a release.

.EXAMPLE
  pwsh tools/lint-lua.ps1
  pwsh tools/lint-lua.ps1 -Verbose
#>
[CmdletBinding()]
param(
    [string]$ScriptsDir = (Join-Path $PSScriptRoot '..\mod\KakarotAccess\Scripts'),
    [string]$Luac       = (Join-Path $PSScriptRoot '..\libs\lua54\luac.exe')
)

$ErrorActionPreference = 'Stop'
$ScriptsDir = (Resolve-Path $ScriptsDir).Path
if (-not (Test-Path $Luac)) { throw "luac not found at $Luac" }
$Luac = (Resolve-Path $Luac).Path

# --- what a global is allowed to be -------------------------------------------------
# Lua 5.4 builtins plus the globals the UE4SS host injects. Anything else is either a
# typo or the local-above-its-declaration bug described above.
$AllowedGlobals = @(
    # Lua builtins
    '_G','assert','collectgarbage','debug','dofile','error','getmetatable','io','ipairs',
    'load','loadfile','math','next','os','package','pairs','pcall','print','rawequal',
    'rawget','rawlen','rawset','require','select','setmetatable','string','table','tonumber',
    'tostring','type','utf8','xpcall','coroutine','arg','warn',
    # UE4SS host globals
    'ExecuteInGameThread','FindAllOf','FindFirstOf','FindObject','StaticFindObject',
    'LoopAsync','RegisterHook','RegisterKeyBind','RegisterCustomProperty','LoadAsset',
    'FName','FText','FString','Key','ModifierKey','PropertyTypes','CreateInvalidObject',
    'UnregisterHook','NotifyOnNewObject','ExecuteWithDelay','print_ue','IsKeyBindRegistered'
) | ForEach-Object { $_.ToLowerInvariant() } | Group-Object | ForEach-Object { $_.Name }
$AllowedSet = [System.Collections.Generic.HashSet[string]]::new(
    [string[]]$AllowedGlobals, [System.StringComparer]::OrdinalIgnoreCase)

# --- guard rules --------------------------------------------------------------------
# `Substrate` = the files that legitimately make the raw call because they ARE the guard.
# `DevOnly`   = tools that run on a manual keypress, never on the poll loop. They are
#               exempt from the guard rules but NOT from syntax/globals. Listing them
#               here rather than silently skipping them keeps the exemption honest: the
#               playbook's position is that a diagnostic which can die destroys the
#               evidence it exists to collect, so these are a known debt, not a blessing.
$DevOnly = @('discover.lua','dev_memdiff.lua')

$GuardRules = @(
    @{ Name = 'raw-IsValid'
       Pattern = ':IsValid\s*\('
       Allow = @('ui_core.lua','mem.lua')
       Fix   = 'use Core.valid(o) for UObjects, Core.valid_ref(o) for TArray/struct handles - a bare IsValid() faults on the freed handle it is meant to reject' }

    @{ Name = 'raw-GetArrayNum'
       Pattern = ':GetArrayNum\s*\('
       Allow = @('ui_core.lua')
       Fix   = 'use Core.array_of(owner, "Name") - owner[prop] yields an INVALID RemoteObject, not nil, and GetArrayNum on it (or on a fixed C array) is the uncatchable abort' }

    # THE RULE THAT WOULD HAVE CAUGHT THE 2026-07-26 CRASH. `host["Info_Log_Bar" .. i]` in
    # screen_toasts was the last thing the mod did before the process died; the same shape was
    # live in keyhelp, quest_objective and guide_watch on the same day. A COMPOSED member name is
    # the highest-risk fetch there is, because composing one means iterating candidates — names
    # the class may well not declare — and an undeclared-member fetch aborts below the Lua
    # boundary where no pcall can reach it. (screen_toasts had its fetch inside a pcall and the
    # author believed that fixed it. It does not.)
    #
    # Scoped to subscripts containing a quote or a `..`, so ordinary array indexing (`t[i]`,
    # `list[#list]`) is untouched. That makes it decidable, which is why it can be an error.
    # Scoped to subscripts that are BUILT (a `..` concatenation or a string.format), because that
    # is the shape that means "iterating candidate names". Plain `t[i]` / `t["key"]` is untouched.
    # A line whose receiver is genuinely a plain Lua table opts out with a `-- lint:plain-table`
    # comment: explicit, greppable, and it documents the claim rather than hiding it.
    @{ Name = 'dynamic-member-fetch'
       Pattern = '[A-Za-z_][A-Za-z0-9_]*\s*\[[^\]]*(?:\.\.|string\.format)'
       Allow = @('ui_core.lua')
       Skip  = 'lint:plain-table'
       Fix   = 'use Core.member(obj, name) - a composed member name is one the class may not declare, and that fetch is an uncatchable abort (pcall does NOT help). If the receiver is a plain Lua table, add -- lint:plain-table' }

    # WARNING, not an error, and deliberately so. GetAddress is the one UObject method that
    # does NOT dereference, so on a UObject it is the safe call - the danger is calling it on
    # a NON-UObject RemoteObject (a TArray or struct handle), where UE4SS raises an error that
    # PIERCES pcall and kills the calling function mid-flight while every enclosing pcall
    # reports success. A regex cannot tell the two apart. Flagging every site as an error would
    # bury the two rules that ARE decidable under noise from provably-safe identity-key calls,
    # and a lint people learn to ignore protects nothing. So: surfaced for review, never fatal.
    # Routing through Mem.raw_addr is still preferred where convenient - it sits behind the
    # transactional guard that NOTICES a pierce instead of dying silently.
    @{ Name = 'raw-GetAddress'
       Pattern = ':GetAddress\s*\('
       Allow = @('mem.lua','ui_core.lua','ui_directory.lua','native_offsets.lua')
       Warn  = $true
       Fix   = 'prefer Mem.raw_addr(o); confirm the receiver is a UObject and not a TArray/struct handle, where GetAddress PIERCES pcall' }
)

# Strips a trailing `--` comment so a rule name merely MENTIONED in prose is not a finding
# (several files carry notes like `-- never ro:IsValid(), see Core.nonnull`, which are the
# rule being obeyed, not broken). Only strips when the quotes before the `--` are balanced,
# so a `--` inside a string literal is left alone; the worst case is a false positive we see
# rather than a real violation we miss.
function Remove-TrailingComment([string]$line) {
    $idx = $line.IndexOf('--')
    while ($idx -ge 0) {
        $before = $line.Substring(0, $idx)
        $dq = ([regex]::Matches($before, '"')).Count
        $sq = ([regex]::Matches($before, "'")).Count
        if (($dq % 2) -eq 0 -and ($sq % 2) -eq 0) { return $before }
        $idx = $line.IndexOf('--', $idx + 2)
    }
    return $line
}

# --- run ------------------------------------------------------------------------------
$files = Get-ChildItem -Path $ScriptsDir -Filter *.lua -File | Sort-Object Name
if (-not $files) { throw "No .lua files found in $ScriptsDir" }

$errors = @()
$warnings = @()

foreach ($f in $files) {
    $rel = $f.Name

    # 1. syntax
    $syntax = & $Luac -p $f.FullName 2>&1
    if ($LASTEXITCODE -ne 0) {
        $errors += "[syntax] ${rel}: $syntax"
        continue          # a file that does not compile cannot be listed or scanned
    }

    # 2. globals
    $listing = & $Luac -l -p $f.FullName 2>&1
    $seen = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::Ordinal)
    foreach ($m in [regex]::Matches(($listing -join "`n"), '_ENV\s+"([A-Za-z_][A-Za-z0-9_]*)"')) {
        $null = $seen.Add($m.Groups[1].Value)
    }
    foreach ($g in $seen) {
        if (-not $AllowedSet.Contains($g)) {
            $errors += "[globals] ${rel}: reads global '$g' - either a typo, or a local used above its own declaration (compiles fine, nil at runtime, raises above the loop's pcall on every tick)"
        }
    }

    # 3. guards
    if ($DevOnly -notcontains $rel) {
        $lines = Get-Content -LiteralPath $f.FullName
        foreach ($rule in $GuardRules) {
            if ($rule.Allow -contains $rel) { continue }
            for ($i = 0; $i -lt $lines.Count; $i++) {
                if ($lines[$i] -match '^\s*--') { continue }        # whole-line comment
                if ($rule.Skip -and $lines[$i] -match [regex]::Escape($rule.Skip)) { continue }
                $code = Remove-TrailingComment $lines[$i]
                if ($code -match $rule.Pattern) {
                    $msg = "[$($rule.Name)] ${rel}:$($i + 1): $($code.Trim())`n        -> $($rule.Fix)"
                    if ($rule.Warn) { $warnings += $msg } else { $errors += $msg }
                }
            }
        }
    } else {
        $warnings += "[dev-only] ${rel}: exempt from guard rules (manual-keypress tool, never on the poll loop)"
    }
}

# --- report ----------------------------------------------------------------------------
Write-Host "Checked $($files.Count) Lua files in $ScriptsDir"
foreach ($w in $warnings) { Write-Host "  warn  $w" -ForegroundColor DarkYellow }

if ($errors.Count -gt 0) {
    Write-Host ""
    Write-Host "LINT FAILED - $($errors.Count) problem(s):" -ForegroundColor Red
    foreach ($e in $errors) { Write-Host "  $e" -ForegroundColor Red }
    exit 1
}

Write-Host "OK - syntax, globals and guards all clean." -ForegroundColor Green
exit 0
