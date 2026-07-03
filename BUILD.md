# KakarotAccess — build, install & test

Accessibility mod for **DRAGON BALL Z KAKAROT** (Unreal Engine 4.21), loaded via **UE4SS**.

## Architecture

A thin **in-process Lua C module** (`prism_bridge.dll`) exposes the PRISM screen reader to UE4SS's
Lua VM; **all accessibility logic lives in Lua** (hot-reloadable, no game restart).

`prism_bridge.dll` statically links **Lua 5.4.4** — the exact version UE4SS v3.0.1 bundles, so the
`lua_State` ABI matches — and dynamically loads `prism.dll` at runtime (`LoadLibrary`/`GetProcAddress`).
UE4SS does `require("prism_bridge")`, which calls `luaopen_prism_bridge` with UE4SS's own `lua_State`;
our statically-linked Lua manipulates that state.

> Pattern validated by **AccessForge/SparkingZeroAccess**'s `speech_bridge` and ported here.
> **No Rust, no RE-UE4SS source build, no UEPseudo, no external process** — just MSVC + the vendored Lua.

## Layout

```
dragon ball kakarot access/            ← this repo
├── src/prism_bridge/                  ← the Lua C module (built once, rarely changes)
│   ├── prism_bridge.c                 ← static Lua 5.4.4 + dynamic prism.dll; exposes the prism table
│   └── build.ps1                      ← compiles lua54.lib + prism_bridge.dll, deploys to the game
├── src/mem_bridge/                    ← guarded native memory reads (non-reflected members)
├── src/audio_bridge/                  ← XAudio2 cue player for the navigation radar
│   ├── audio_bridge.cpp               ← ping (pan/volume/pitch) + arrival cues; sounds from Scripts/sounds/
│   └── build.ps1                      ← same pattern; deploys audio_bridge.dll
├── mod/KakarotAccess/Scripts/         ← Lua mod + runtime DLLs (junctioned into the game)
│   ├── main.lua                       ← entry: F8 speech test, F9 read position, Ctrl+F8 silence
│   ├── speech.lua                     ← speech sink over the prism_bridge module
│   ├── prism_bridge.dll               ← built by build.ps1
│   ├── prism.dll, tolk.dll            ← PRISM runtime (copied by build.ps1)
├── libs/lua54/                        ← vendored Lua 5.4.4 source (UE4SS's exact version) + lua54.lib
├── libs/prism/                        ← PRISM v0.16.7 prebuilt (bin/include/lib)
└── reference/dbz-kakarot/             ← static recon (UI/text architecture)
```

## Prerequisites (already set up on this machine)

- **MSVC 2022** with the "Desktop development with C++" workload (the build locates it via `vswhere`).
- **UE4SS v3.0.1 (zDEV)** installed in `…\DRAGON BALL Z KAKAROT\AT\Binaries\Win64\`.
- Mod enabled: `…\Win64\Mods\mods.txt` contains `KakarotAccess : 1`.
- `…\Win64\Mods\KakarotAccess\Scripts` is a **junction** to `mod/KakarotAccess/Scripts` in this repo.

> No Rust toolchain is required (the old hybrid plan that built RE-UE4SS via Corrosion was dropped in
> favor of this self-contained Lua C module).

## Build

```sh
pwsh -File src\prism_bridge\build.ps1     # screen reader bridge
pwsh -File src\mem_bridge\build.ps1       # native memory reader
pwsh -File src\audio_bridge\build.ps1     # navigation radar cue player
```

`build.ps1` is self-contained:
1. Compiles the vendored Lua 5.4.4 (`libs/lua54/src`) into `libs/lua54/lua54.lib` (only if missing).
2. Compiles + links `prism_bridge.dll` against it.
3. Deploys `prism_bridge.dll`, `prism.dll`, `tolk.dll` into `mod/KakarotAccess/Scripts/`
   (which is junctioned into the game, so they land in the game automatically).

## Test (with a screen reader running — NVDA/JAWS, or SAPI)

1. Launch the game. The UE4SS console (and `UE4SS.log`) should show the mod load and
   `prism_bridge loaded, screen reader: <name>`.
2. **F8** → speaks "Kakarot accessibility online. Screen reader: <name>" — validates the whole
   PRISM→Lua pipeline.
3. **F9** → speaks the player's X/Y/Z world position — validates reading live game state.
4. **Ctrl+F8** → silence.

## Conventions

- **Dumps live with the source.** Discovery dumps (F7 → `discover.lua`) are always written to
  `mod/KakarotAccess/Scripts/dumps/` as `dump_<time>_<NNN>.txt`. Always call them **dumps** (one
  consistent name — not "probes"). Only the mod itself runs from the game folder, via the
  `Scripts` junction; nothing gets copied there by hand.
- **Screenshots** the user takes of unread screens are always in
  `C:\Users\ali-b\Pictures\Screenshots` (`Captura de pantalla (N).png`). Pick the most recent by
  mtime to match the current session's dumps.

## Iterate (no game restart)

Edit `mod/KakarotAccess/Scripts/*.lua`, then press **Ctrl+R** in the UE4SS console to hot-reload.
Only changes to the **C module** (`prism_bridge.c`) require re-running `build.ps1` + a game restart (rare).
