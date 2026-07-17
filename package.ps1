<#
.SYNOPSIS
  Assemble a KakarotAccess release .zip for the installer to download.

.DESCRIPTION
  The .zip is an OVERLAY rooted at the game directory: every path inside is
  relative to <game_dir>, so the installer just extracts it there. It bundles:
    - UE4SS (the loader) copied from a working install,
    - the mod itself (mod/KakarotAccess/Scripts) from this repo,
    - a clean mods.txt and a version.txt.

  This does NOT publish anything. It prints the `gh release` command at the end;
  run that yourself when you want to publish.

.EXAMPLE
  pwsh -File package.ps1 -Version v1.0.0
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$Version,                      # e.g. v1.0.0  (goes into version.txt + zip name)
    [string]$GameDir,                      # source of UE4SS; auto-detected if omitted
    [string]$OutDir = (Join-Path $PSScriptRoot 'dist')
)

$ErrorActionPreference = 'Stop'
$AppId = 851850
$ModName = 'KakarotAccess'
$Rel = 'AT\Binaries\Win64'                 # game-relative root of everything we ship

function Find-GameDir {
    $steam = (Get-ItemProperty 'HKCU:\Software\Valve\Steam' -Name SteamPath -ErrorAction SilentlyContinue).SteamPath
    if (-not $steam) { return $null }
    $steam = $steam -replace '/', '\'
    $libs = @($steam)
    $vdf = Join-Path $steam 'steamapps\libraryfolders.vdf'
    if (Test-Path $vdf) {
        foreach ($m in Select-String -Path $vdf -Pattern '"path"\s+"([^"]+)"' -AllMatches) {
            foreach ($mm in $m.Matches) { $libs += ($mm.Groups[1].Value -replace '\\\\', '\') }
        }
    }
    foreach ($lib in $libs) {
        $acf = Join-Path $lib "steamapps\appmanifest_$AppId.acf"
        if (Test-Path $acf) {
            $m = Select-String -Path $acf -Pattern '"installdir"\s+"([^"]+)"'
            if ($m) {
                $dir = Join-Path $lib ('steamapps\common\' + $m.Matches[0].Groups[1].Value)
                if (Test-Path (Join-Path $dir "$Rel\AT-Win64-Shipping.exe")) { return $dir }
            }
        }
    }
    return $null
}

# --- locate the source of UE4SS ------------------------------------------------
if (-not $GameDir) { $GameDir = Find-GameDir }
if (-not $GameDir -or -not (Test-Path (Join-Path $GameDir "$Rel\AT-Win64-Shipping.exe"))) {
    throw "Game folder not found. Pass -GameDir '...\DRAGON BALL Z KAKAROT'."
}
$Win64 = Join-Path $GameDir $Rel
Write-Host "Game:   $GameDir"

$ScriptsSrc = Join-Path $PSScriptRoot 'mod\KakarotAccess\Scripts'
if (-not (Test-Path $ScriptsSrc)) { throw "Mod scripts not found at $ScriptsSrc" }

# The native runtime DLLs are built by src\*\build.ps1 and gitignored; they must
# exist locally before packaging.
$RequiredDlls = 'prism.dll', 'prism_bridge.dll', 'tolk.dll', 'mem_bridge.dll', 'audio_bridge.dll', 'input_bridge.dll'
$missing = $RequiredDlls | Where-Object { -not (Test-Path (Join-Path $ScriptsSrc $_)) }
if ($missing) {
    throw "Missing built DLLs in Scripts: $($missing -join ', '). Run the src\*\build.ps1 scripts first."
}

# --- stage the overlay ---------------------------------------------------------
$Stage = Join-Path $OutDir 'stage'
if (Test-Path $Stage) { Remove-Item $Stage -Recurse -Force }
$StageWin64 = Join-Path $Stage $Rel
New-Item -ItemType Directory -Force -Path $StageWin64 | Out-Null

# 1) UE4SS loader files.
foreach ($f in 'dwmapi.dll', 'UE4SS.dll', 'UE4SS-settings.ini') {
    $src = Join-Path $Win64 $f
    if (-not (Test-Path $src)) { throw "UE4SS file missing in the install: $f" }
    Copy-Item $src (Join-Path $StageWin64 $f)
}

# 2) UE4SS built-in mods (everything under Mods\ except our mod, the dev-only
#    DBZK_Fix, and runtime dumps).
$ModsSrc = Join-Path $Win64 'Mods'
$StageMods = Join-Path $StageWin64 'Mods'
New-Item -ItemType Directory -Force -Path $StageMods | Out-Null
$SkipMods = @($ModName, 'DBZK_Fix')
Get-ChildItem $ModsSrc -Directory | Where-Object { $SkipMods -notcontains $_.Name } | ForEach-Object {
    Copy-Item $_.FullName (Join-Path $StageMods $_.Name) -Recurse
}

# 3) A clean mods.txt (UE4SS built-ins off, our mod + Keybinds on). The installer
#    only uses this on a FRESH install; otherwise it preserves the user's own.
@'
CheatManagerEnablerMod : 0
ActorDumperMod : 0
ConsoleCommandsMod : 0
ConsoleEnablerMod : 0
LineTraceMod : 0
BPML_GenericFunctions : 0
jsbLuaProfilerMod : 0
KakarotAccess : 1

; Built-in keybinds, do not move up!
Keybinds : 1
'@ | Set-Content -Path (Join-Path $StageMods 'mods.txt') -Encoding utf8 -NoNewline

# 4) The mod itself. Copy Scripts, then strip dev-only / runtime-generated files.
$StageModDir = Join-Path $StageMods $ModName
$StageScripts = Join-Path $StageModDir 'Scripts'
New-Item -ItemType Directory -Force -Path $StageModDir | Out-Null
Copy-Item $ScriptsSrc $StageScripts -Recurse
$DevJunk = 'dumps', 'dump.txt', 'probe.txt', 'dev_probe.txt', 'config.txt'
foreach ($j in $DevJunk) {
    $p = Join-Path $StageScripts $j
    if (Test-Path $p) { Remove-Item $p -Recurse -Force }
}

# 5) version.txt (the installer reads this to know what's installed).
Set-Content -Path (Join-Path $StageModDir 'version.txt') -Value $Version -Encoding utf8 -NoNewline

# --- zip it --------------------------------------------------------------------
$Zip = Join-Path $OutDir "$ModName-$Version.zip"
if (Test-Path $Zip) { Remove-Item $Zip -Force }
Compress-Archive -Path (Join-Path $Stage '*') -DestinationPath $Zip -CompressionLevel Optimal
Remove-Item $Stage -Recurse -Force

$sizeMB = [math]::Round((Get-Item $Zip).Length / 1MB, 2)
Write-Host ""
Write-Host "Built $Zip ($sizeMB MB)" -ForegroundColor Green
Write-Host ""
Write-Host "To publish (run yourself when ready):" -ForegroundColor Cyan
Write-Host "  gh release create $Version `"$Zip`" --repo Ali-Bueno/dbz-kakarot-access --title `"$ModName $Version`" --notes `"...`""
