# ue4ss-screenreader-template

> Reusable UE4SS+PRISM screen-reader bridge + UE4SS reference docs (canonical at D:\code\modding projects\reference\engines\ue4ss, mirrored in each project)


Reusable starting point so ANY UE4SS-modded Unreal game gets a screen reader (PRISM) with minimal setup.

**LOCATIONS (updated 2026-07-01):** the old `D:\code\unreal\UE4ss study` is EMPTY/moved. The canonical
UE4SS reference library now lives at **`D:\code\modding projects\reference\engines\ue4ss\`** and is
MIRRORED per-project (for this game: `reference/UE4ss study/docs/`). Both must be kept in sync when
updated. Docs set: `ue4ss-api-reference.md` (finding/hooks/threading/classes + **performance**),
**`ue4ss-discovery-tools.md`** (built-in dumpers Ctrl+J/H + **Live View watches** — the recommended way to
find UI state), `ue4ss-settings-reference.md` (full UE4SS-settings.ini), `ue4ss-compatibility-and-crashes.md`,
`workflow-per-game.md`, `accessibility-patterns.md` (now includes **hard-won lessons**: on_screen/IsVisible-
lies, pooled-widget caching, FindAllOf O(N), uncatchable struct-read aborts, debounce). The runnable
PRISM-bridge template is under `D:\code\modding projects\templates\ue4ss-prism-bridge\`.

**KEY DISCOVERY LESSON (2026-07-01):** stop hand-rolling reflection dumps first. Use `Ctrl+H` (CXX headers,
with LoadAllAssets=1) for a class's real property/function list OFFLINE, and the **Live View "Add watch"**
to see which property flips as you navigate a menu. Dumpers (Ctrl+J/H) are keybind features that work even
with the GUI console OFF; only the Live View needs `GuiConsoleEnabled=1` (temporarily, for discovery).

**NON-REFLECTED STATE — two proven techniques documented in `ue4ss-discovery-tools.md` §4 (added 2026-07-03,
both locations):** (a) **native memory diff / "F4 probe"** — a separate SEH-guarded `mem_bridge.dll` + a
keybind that diffs a raw tail window and reports the int32 that steps 0→1→2 as you move the cursor → finds
a hidden selection/page/window index reflection can't see (DBZ Kakarot: pause 0x43C, StartTop 0x4E4/0x4DC/
0x4EC, Tips 0x420/0x424). (b) **fixed-array UPROPERTY collapse** — UE4SS flattens a fixed C-array widget
member (`Type* Name; //(size:0xNN)`, NN>8) to element 0 (indexing yields garbage, array methods abort); use
`RegisterCustomProperty` to expose each `base+i*8` pointer as an ObjectProperty. Plus `first_on_screen`
(cached_all + on_screen) for screens with several pooled instances where the active one alternates. See
[dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md) for the worked examples.

- **`template/`** — drop-in bridge:
  - `libs/lua54/` (vendored Lua 5.4.4 src + prebuilt lua54.lib), `libs/prism/` (PRISM prebuilt: include/bin/lib, prism.dll + tolk.dll).
  - `src/prism_bridge/prism_bridge.c` + `build.ps1` (self-contained MSVC build, no CMake; deploys the 3 DLLs to `mod/Scripts`).
  - `mod/Scripts/` — the ONE mod folder (copy/junction into `Mods/<ModName>/Scripts`). Contains prebuilt `prism_bridge.dll` + `prism.dll` + `tolk.dll`, plus the merged service architecture: `main.lua` (wires services + LoadMap/BeginPlay/new-widget hooks), `screen_reader.lua` (single PRISM-backed speech sink: Init/Output/Silence/BackendName), `announcer.lua` (dedup/anti-spam), `game_state_tracker.lua`, `ui_reader.lua` (UMG text), `keybind_manager.lua` (F1 state/F2 player/F3 UI/F4 verbose/F8 speech-test/Ctrl+Esc silence), `utils.lua` (safe reads + discovery dumps). Verified building from the template produces a working bridge (2026-07-01).
- **`docs/ue4ss-compatibility-and-crashes.md`** — field guide for UE4SS not loading on a stubborn game, ordered cheapest-first. Key lessons (from the DBZ Kakarot debug, see [dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md)): read `%LOCALAPPDATA%\<Project>\Saved\Crashes\*\CrashContext.runtime-xml`; `Failed to find GUObjectArray/StaticConstructObject` → set `[EngineVersionOverride]` one minor below real (4.20 for a 4.21 game); hang at `Locating KismetSystemLibrary CDO` → UE4SS build regression, use a known-working per-game build (e.g. DBZK_Fix's April-2024 UE4SS); `HookProcessInternal=0` doesn't stop the detour if a mod calls RegisterHook.

Bridge design: `prism_bridge.dll` STATICALLY links Lua 5.4.4 (UE4SS's exact version → matching `lua_State` ABI) and DYNAMICALLY loads `prism.dll`. No UE4SS source build / Rust / external process. A UE4SS update needs no rebuild while its bundled Lua stays 5.4.x.

**MERGED (2026-07-01):** the user's older `template/scripts/` scaffold (service architecture) and the PRISM bridge are now unified under `template/mod/Scripts/`. The Tolk-based `screen_reader.lua` was rewired to PRISM (`require("prism_bridge")`, same Init/Output/Silence API so announcer/keybind_manager/main are unchanged); the redundant `speech.lua` and the duplicate lowercase `scripts/` folder were removed. One Scripts folder only — no more duplication.
