<#
.SYNOPSIS
  Runs the offline Lua regression tests in tools/tests/. Exits non-zero on any failure.

.DESCRIPTION
  Each test_*.lua is a standalone script for the VENDORED Lua 5.4 (the same sources the
  game-side luac.exe and the bridges link against), stubbing the UE4SS host so mod logic
  can be exercised without the game. lua54.lib and lua.exe are built on demand from
  libs/lua54 with the same MSVC recipe as the bridge builds.

  These tests complement, not replace, tools/lint-lua.ps1 and in-game verification: they
  pin pure-Lua state machines (gates, caches, release paths) that a live session can only
  probe by playing the exact interrupting scenario.

.EXAMPLE
  pwsh tools/run-lua-tests.ps1
#>
[CmdletBinding()]
param(
    [string]$TestsDir = (Join-Path $PSScriptRoot 'tests'),
    [string]$LuaDir   = (Join-Path $PSScriptRoot '..\libs\lua54')
)

$ErrorActionPreference = 'Stop'
$LuaDir = (Resolve-Path $LuaDir).Path
$lua = Join-Path $LuaDir 'lua.exe'
$luaLib = Join-Path $LuaDir 'lua54.lib'

if (-not (Test-Path $lua)) {
    Write-Host "Building lua.exe from vendored Lua 5.4 (one-time)..."
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $vsRoot = & $vswhere -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -latest -property installationPath
    if (-not $vsRoot) { throw "MSVC C++ tools not found (install VS Build Tools 'Desktop development with C++')." }
    $vcvars = "$vsRoot\VC\Auxiliary\Build\vcvars64.bat"
    $tmp = Join-Path $env:TEMP ('kakarot_lua_tests_' + [guid]::NewGuid().ToString('N'))
    New-Item -ItemType Directory -Force -Path $tmp | Out-Null
    if (-not (Test-Path $luaLib)) {
        Write-Host "Building lua54.lib from vendored Lua 5.4 (one-time)..."
        $luaObjs = Join-Path $tmp 'lua_obj'
        New-Item -ItemType Directory -Force -Path $luaObjs | Out-Null
        $luaSrcs = Get-ChildItem "$LuaDir\src\*.c" |
            Where-Object { $_.Name -notin @('lua.c', 'luac.c') } |
            ForEach-Object { "`"$($_.FullName)`"" }
        $cLib = "call `"$vcvars`" >nul 2>&1 && cl /nologo /c /O2 /MD /W0 /D_CRT_SECURE_NO_WARNINGS /TC /I`"$LuaDir\include`" /Fo:`"$luaObjs\\`" $($luaSrcs -join ' ') && lib /nologo /out:`"$luaLib`" `"$luaObjs\*.obj`""
        cmd /c $cLib | Out-Null
        if ($LASTEXITCODE -ne 0 -or -not (Test-Path $luaLib)) {
            throw "lua54.lib build failed"
        }
    }
    $c = "call `"$vcvars`" >nul 2>&1 && cl /nologo /O2 /MD /W0 /D_CRT_SECURE_NO_WARNINGS /TC /I`"$LuaDir\include`" `"$LuaDir\src\lua.c`" /Fo:`"$tmp\lua.obj`" /Fe:`"$lua`" /link `"$luaLib`""
    cmd /c $c | Out-Null
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path $lua)) { throw "lua.exe build failed" }
}

$files = Get-ChildItem -Path $TestsDir -Filter 'test_*.lua' -File | Sort-Object Name
if (-not $files) { throw "No test_*.lua files found in $TestsDir" }

$failed = @()
foreach ($f in $files) {
    Write-Host "== $($f.Name)"
    & $lua $f.FullName
    if ($LASTEXITCODE -ne 0) { $failed += $f.Name }
}

Write-Host ""
if ($failed.Count -gt 0) {
    Write-Host "TESTS FAILED - $($failed -join ', ')" -ForegroundColor Red
    exit 1
}
Write-Host "OK - $($files.Count) test file(s) passed." -ForegroundColor Green
exit 0
