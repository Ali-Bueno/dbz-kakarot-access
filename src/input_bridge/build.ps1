# Builds input_bridge.dll (the in-process Lua C module that reads the gamepad and
# hides it from the game via an XInput IAT hook) and deploys it into Scripts/.
#
# SEPARATE from the other bridges by design (prism = screen reader, mem = memory
# reads, audio = cues, input = pad). Reuses the SAME vendored Lua 5.4.4
# (libs/lua54/lua54.lib). No external deps — the real XInputGetState pointer is
# taken from the exe's own import table, so we never link xinput.
#
# Usage:  pwsh -File src\input_bridge\build.ps1

$ErrorActionPreference = "Stop"
$proj = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$lua = "$proj\libs\lua54"
$scripts = "$proj\mod\KakarotAccess\Scripts"   # junctioned into the game
$tmp = "$env:TEMP\kakarot_build"
New-Item -ItemType Directory -Force -Path $tmp | Out-Null

# Locate the MSVC dev environment.
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vsRoot = & $vswhere -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -latest -property installationPath
if (-not $vsRoot) { throw "MSVC C++ tools not found (install VS Build Tools 'Desktop development with C++')." }
$vcvars = "$vsRoot\VC\Auxiliary\Build\vcvars64.bat"

# 1) Build lua54.lib (only if missing) — same vendored Lua 5.4.4 as the other bridges.
if (-not (Test-Path "$lua\lua54.lib")) {
    Write-Host "Building lua54.lib from vendored Lua 5.4.4..."
    $luaObjs = "$tmp\lua_obj"; New-Item -ItemType Directory -Force -Path $luaObjs | Out-Null
    $luaSrcs = Get-ChildItem "$lua\src\*.c" | Where-Object { $_.Name -notin @('lua.c','luac.c') } |
        ForEach-Object { "`"$($_.FullName)`"" }
    $c1 = "call `"$vcvars`" >nul 2>&1 && cl /nologo /c /O2 /MD /W0 /D_CRT_SECURE_NO_WARNINGS /TC /I`"$lua\include`" /Fo:`"$luaObjs\\`" $($luaSrcs -join ' ') && lib /nologo /out:`"$lua\lua54.lib`" `"$luaObjs\*.obj`""
    cmd /c $c1 | Out-Null
    if (-not (Test-Path "$lua\lua54.lib")) { throw "lua54.lib build failed" }
}

# 2) Build input_bridge.dll.
Write-Host "Building input_bridge.dll..."
$c2 = "call `"$vcvars`" >nul 2>&1 && cl /nologo /O2 /MD /TC /I`"$lua\include`" `"$PSScriptRoot\input_bridge.c`" /Fo:`"$tmp\input_bridge.obj`" /LD /Fe:`"$tmp\input_bridge.dll`" /link `"$lua\lua54.lib`""
cmd /c $c2 | Select-Object -Last 3
if (-not (Test-Path "$tmp\input_bridge.dll")) { throw "input_bridge.dll build failed" }

# 3) Deploy into the mod's Scripts/ folder (next to the other bridges — kept separate).
Copy-Item "$tmp\input_bridge.dll" "$scripts\input_bridge.dll" -Force
Write-Host "Deployed input_bridge.dll to $scripts"
Get-ChildItem "$scripts\*.dll" | Select-Object Name, Length | Format-Table -AutoSize
