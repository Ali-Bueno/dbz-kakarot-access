# UE4SS-settings.ini reference

Every notable option, by section, with defaults. Lives next to `UE4SS.dll` (flat layout) or in `ue4ss/`
(subfolder layout). The load-bearing ones for **getting UE4SS to run at all** are in
[ue4ss-compatibility-and-crashes](ue4ss-compatibility-and-crashes.md); this is the full menu.

## [Overrides]
- `ModsFolderPath` (`<dll_dir>/Mods`) — where mods live.
- `ModsFolderPaths` — extra mod dirs, `+`/`-` prefixes to add/remove.
- `ControllingModsTxt` — use a specific `mods.txt` as the enable list.

## [General]
- `EnableHotReloadSystem` (1) — allow reload-on-hotkey.
- `HotReloadKey` (R) — reload key (CTRL always required → **Ctrl+R**).
- `EnableAutoReloadingLuaMods` (0) — auto-reload when a Script file changes on disk.
- `UseCache` (1) / `InvalidateCacheIfDLLDiffers` (1) — AOB scan cache.
- `SecondsToScanBeforeGivingUp` (30) — scanner timeout.
- **`bUseUObjectArrayCache` (true)** — build UObject listeners for a fast GUObjectArray cache instead of
  iterating it. **Set `false` if the game crashes on startup.** (Note: some non-zDev releases default this
  to `false`.)
- `DoEarlyScan` (0) — AOB scan immediately at start.
- `bEnableSeachByMemoryAddress` (false) — allow finding UObjects by hex address in Live View.
- `DefaultExecuteInGameThreadMethod` (EngineTick) — how `ExecuteInGameThread` is dispatched. If tick hooks
  are unavailable/janky, alternatives exist — relevant to how your poll runs on the game thread.
- `DefaultFNameToStringMethod` (Scan) — FName→string strategy.

## [EngineVersionOverride]  ← the #1 compatibility fix
- `MajorVersion` / `MinorVersion` (empty) — **force the engine version** the scanner assumes. For a
  stubborn game, set to **one minor below** the real version (e.g. real 4.21 → `4` / `20`). This is the
  single most common fix for `Failed to find GUObjectArray`.
- `DebugBuild` (false) — target is a Debug/Development build.

## [ObjectDumper]
- `LoadAllAssetsBeforeDumpingObjects` (0) — force-load assets before `Ctrl+J` (see
  [discovery-tools](ue4ss-discovery-tools.md)).
- `UseModuleOffsets` (0) — show module offsets instead of raw pointers.

## [CXXHeaderGenerator]
- `DumpOffsetsAndSizes` (1) — include property offsets/sizes.
- `KeepMemoryLayout` (0) — accurate layouts (needed for real C++ projects; off for browsing).
- **`LoadAllAssetsBeforeGeneratingCXXHeaders` (0)** — set **1** so `Ctrl+H` captures every UI class.

## [UHTHeaderGenerator]
- `IgnoreAllCoreEngineModules` (0), `IgnoreEngineAndCoreUObject` (0) — skip engine packages.
- `MakeAllFunctionsBlueprintCallable` (1), `MakeAllPropertyBlueprintsReadWrite` (1),
  `MakeEnumClassesBlueprintType` (1), `MakeAllConfigsEngineConfig` (1) — force modifiers for a mirror uproject.

## [Debug]
- `ConsoleEnabled` (1) — external text console.
- `GuiConsoleEnabled` (1) — GUI console + **Live View** (core, no mod needed).
- `GuiConsoleVisible` (1) — show it on start.
- `GuiConsoleFontScaling` (1) — font multiplier.
- `GraphicsAPI` (opengl) — `dx11`/`d3d11`/`opengl`; switch if the GUI is blank.
- `RenderMode` (ExternalThread) — GUI render method.

## [Threads]
- `SigScannerNumThreads` (8), `SigScannerMultithreadingModuleSizeThreshold` (16777216) — scanner threading.

## [Memory]
- `MaxMemoryUsageDuringAssetLoading` (85) — % ceiling before asset force-loading stops.

## [Hooks]  ← function-dispatch detours UE4SS installs
Defaults are all `1`. Relevant facts:
- `HookProcessInternal` (1), `HookProcessLocalScriptFunction` (1), `HookUObjectProcessEvent` (1) — the
  UFunction-dispatch detours that back Lua `RegisterHook`. **Setting these to 0 does NOT reliably stop the
  detour**: any mod that calls `RegisterHook` makes UE4SS install it on demand (see compatibility doc).
- `HookInitGameState`, `HookLoadMap`, `HookBeginPlay`, `HookEndPlay`, `HookAActorTick`, `HookEngineTick`,
  `HookGameViewportClientTick`, `HookCallFunctionByNameWithArguments`, `HookLocalPlayerExec`,
  `HookProcessConsoleExec`, `HookUStructLink` (all 1) — enable the corresponding Register*Hook Lua APIs and
  `ExecuteInGameThread` via tick. **`HookEngineTick` must stay on** for tick-based game-thread execution.
- `EngineTickResolveMethod` (Scan), `FExecVTableOffsetInLocalPlayer` (0x28) — resolution details.

## [CrashDump]
- `EnableDumping` (1) — write crash dumps.
- `FullMemoryDump` (0) — full memory dump on crash.

## [ExperimentalFeatures]
- (varies by build; usually empty.)

---

### Minimal "make it run + let me explore" profile
```ini
[General]
EnableHotReloadSystem = 1
bUseUObjectArrayCache = true      ; try false if it crashes on startup
[EngineVersionOverride]
MajorVersion = 4
MinorVersion = 20                 ; N-1 of the real version, then try exact N
[Debug]
ConsoleEnabled = 1
GuiConsoleEnabled = 1
GuiConsoleVisible = 1
GraphicsAPI = opengl
[ObjectDumper]
LoadAllAssetsBeforeDumpingObjects = 1
[CXXHeaderGenerator]
LoadAllAssetsBeforeGeneratingCXXHeaders = 1
```
