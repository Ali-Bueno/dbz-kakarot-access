# Builds mem_bridge.dll (the in-process Lua C module for guarded native memory
# reads) and deploys it into the mod's Scripts/ folder.
#
# SEPARATE from prism_bridge by design: prism_bridge = screen reader only; this =
# memory reads only (no accessibility logic). Reuses the SAME vendored Lua 5.4.4
# (libs/lua54/lua54.lib, built on demand by either bridge's build). No external deps.
#
# Usage:  pwsh -File src\mem_bridge\build.ps1

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

# 1) Build lua54.lib (only if missing) — same vendored Lua 5.4.4 as prism_bridge.
if (-not (Test-Path "$lua\lua54.lib")) {
    Write-Host "Building lua54.lib from vendored Lua 5.4.4..."
    $luaObjs = "$tmp\lua_obj"; New-Item -ItemType Directory -Force -Path $luaObjs | Out-Null
    $luaSrcs = Get-ChildItem "$lua\src\*.c" | Where-Object { $_.Name -notin @('lua.c','luac.c') } |
        ForEach-Object { "`"$($_.FullName)`"" }
    $c1 = "call `"$vcvars`" >nul 2>&1 && cl /nologo /c /O2 /MD /W0 /D_CRT_SECURE_NO_WARNINGS /TC /I`"$lua\include`" /Fo:`"$luaObjs\\`" $($luaSrcs -join ' ') && lib /nologo /out:`"$lua\lua54.lib`" `"$luaObjs\*.obj`""
    cmd /c $c1 | Out-Null
    if (-not (Test-Path "$lua\lua54.lib")) { throw "lua54.lib build failed" }
}

# 2) Build mem_bridge.dll.  /EHa enables SEH (__try/__except) alongside the CRT.
Write-Host "Building mem_bridge.dll..."
$c2 = "call `"$vcvars`" >nul 2>&1 && cl /nologo /O2 /MD /TC /EHa /I`"$lua\include`" `"$PSScriptRoot\mem_bridge.c`" /Fo:`"$tmp\mem_bridge.obj`" /LD /Fe:`"$tmp\mem_bridge.dll`" /link `"$lua\lua54.lib`""
cmd /c $c2 | Select-Object -Last 3
if (-not (Test-Path "$tmp\mem_bridge.dll")) { throw "mem_bridge.dll build failed" }

# 3) Deploy into the mod's Scripts/ folder (next to prism_bridge.dll — kept separate).
Copy-Item "$tmp\mem_bridge.dll" "$scripts\mem_bridge.dll" -Force
Write-Host "Deployed mem_bridge.dll to $scripts"
Get-ChildItem "$scripts\*.dll" | Select-Object Name, Length | Format-Table -AutoSize
