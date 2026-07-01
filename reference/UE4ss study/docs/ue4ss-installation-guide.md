# UE4SS Installation Guide

## What is UE4SS?

UE4SS (Unreal Engine 4/5 Scripting System) is a modding framework for Unreal Engine games (4.12+ and UE5). It provides:
- Lua scripting API for runtime game manipulation
- C++ mod API for compiled mods
- Blueprint mod loading
- Live object viewer/editor (GUI debugger)
- Header/object dumpers for reverse engineering

## Installation Types

### Basic Install (End Users)
- File: `UE4SS_v{version}.zip`
- No debug windows, minimal footprint

### Developer Install (Modders)
- File: `zDEV-UE4SS_v{version}.zip`
- Includes GUI console, live viewer, debug features
- **Always use this for development**

## Installation Steps

1. Find the game's executable directory:
   ```
   GameName/Binaries/Win64/
   ```
   Look for the **large** `.exe` (not a small launcher wrapper).

2. Extract UE4SS contents into that directory.

3. The DLL proxy (`dwmapi.dll`) loads automatically with the game.

4. Verify: Press `@` or `F10` for console, check `UE4SS.log`, or enable GUI console.

## Key Directories

| Directory | Contains |
|-----------|----------|
| Root directory | UE4SS DLL files |
| Working directory | `UE4SS-settings.ini`, `Mods/` folder, logs |
| Game executable dir | The actual game .exe |

## Configuration (UE4SS-settings.ini)

### Essential Settings for Development
```ini
[Debug]
ConsoleEnabled = 1
GuiConsoleEnabled = 1
GuiConsoleVisible = 1

[General]
EnableHotReloadSystem = 1
```

### Graphics API (if GUI crashes)
```ini
[Debug]
GraphicsAPI = opengl   ; options: dx11, d3d11, opengl
```

## Dumper Hotkeys

| Hotkey | Output | Purpose |
|--------|--------|---------|
| Ctrl+H | `.hpp` files | C++ headers per class (with offsets) |
| Ctrl+Numpad 9 | UHT headers | UHT-compatible for .uproject reconstruction |
| Ctrl+J | `UE4SS_ObjectDump.txt` | All loaded objects |
| Ctrl+Numpad 6 | `.usmap` | Mapping files |
| Ctrl+Numpad 7 | CSV | Static mesh / actor data |

## Alternative Installation Methods

### Manual Injection
Delete `dwmapi.dll`, inject `UE4SS.dll` with your own injector.

### Central Install
One UE4SS install for multiple games with per-game working directories.

### Command Line Options
- `--disable-ue4ss` — Temporarily disable
- `--ue4ss-path <path>` — Custom UE4SS.dll location

### Environment Variables
- `UE4SS_MODS_PATHS` — Semicolon-separated additional mod directories

## Custom Game Configs

For games where default AOB scans fail, create custom signatures:
```
CustomGameConfigs/[GameName]/UE4SS_Signatures/GUObjectArray.lua
```

Each Lua file provides:
```lua
function Register()
    return "48 8B C4 57 48 83 EC 70 ..."  -- AOB pattern with ?? wildcards
end

function OnMatchFound(MatchAddress)
    return MatchAddress  -- or calculate final address
end
```
