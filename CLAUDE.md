# Accessibility Modding — Playbook (CLAUDE.md)

> **What this file is:** the manual for building accessibility mods (screen reader via **PRISM** +
> audio cues). It's a **template**: apply it game by game, section by section. The reusable
> reverse-engineering knowledge lives in [the reference library](reference/).

## Goal

Make the game **[GAME NAME]** accessible so a blind player can complete it start to finish with the
same mechanical and narrative experience as a sighted player, via:
- Accessible screen-reader output using **PRISM** (https://github.com/ethindp/prism).
- Audio cues **only** when they add spatial orientation or feedback that text can't convey.
- Keeping gameplay faithful to the original.
- Clean, modular, maintainable code.

## Table of contents

1. [Development flow](#1-development-flow)
2. [Investigate the engine BEFORE assuming anything](#2-investigate-the-engine-before-assuming-anything)
3. [Dependencies](#3-dependencies)
4. [PRISM — integrating the prebuilt release (verified)](#4-prism--integrating-the-prebuilt-release-verified-do-not-re-investigate)
5. [Reference library and section-by-section build](#5-reference-library-and-section-by-section-build)
6. [Code architecture and best practices](#6-code-architecture-and-best-practices)
7. [Patches / Hooks](#7-patches--hooks)
8. [Accessibilizing UI (menus, inventories)](#8-accessibilizing-ui-menus-inventories)
9. [Speech, audio cues and state tracking](#9-speech-audio-cues-and-state-tracking)
10. [Conventions, logging and publishing](#10-conventions-logging-and-publishing)

---

## 1. Development flow

Build **section by section**, validating with the screen reader active at every step:

1. Identify the engine and the modding framework (§2).
2. Identify the game's critical screens and states.
3. Locate the key methods/data to patch or read.
4. **Adapter + Speech first**: get `Speak("test", interrupt)` working through PRISM and read a simple
   state (position/heading) on a key. This validates the whole pipeline.
5. Create **minimal** patches/hooks (no logic) that delegate to accessibility services.
6. Test with the screen reader; tune verbosity and timing.
7. Add audio cues only when they add real value.

For navigation mods, the recommended order and which document to read for each section live in
[the audio-navigation guide](reference/audio-navigation/README.md) (§5).

---

## 2. Investigate the engine BEFORE assuming anything

> **CRITICAL RULE: do NOT assume the game uses BepInEx or Unity.** Investigate the engine and the
> modding framework of THAT specific game before writing a single line. BepInEx is just **one** of the
> possible cases.

### How to identify the engine (clues in the install folder)

- `UnityPlayer.dll`, a `*_Data/` folder, `GameAssembly.dll` → **Unity** (Mono if there's no
  `GameAssembly.dll`; **IL2CPP** if there is one).
- `*.pak`, an `Engine/` folder, `*-Win64-Shipping.exe` → **Unreal Engine**.
- Engine-specific DLLs/executables (e.g. Capcom's **RE Engine**) → proprietary engine.
- Native C/C++/VB6 executable with no managed runtime → native game.

### Modding framework per case

| Engine / Game | Typical framework | Notes |
|---|---|---|
| Unity (Mono) | **BepInEx 5** | Plugin Class Library + Harmony |
| Unity (IL2CPP) | **BepInEx 6** | Use correct IL2CPP reflection (see memories) |
| **RE Engine** (Capcom) | **REFramework** + native plugins / Lua | NOT BepInEx |
| **Dragon Ball Xenoverse 2** | **XV Patcher** / game-specific mods | NOT BepInEx |
| Unreal Engine | UE4SS / native mods | Depends on the game |
| Native game (C/C++/VB6) | Native plugin / custom hook | BepInEx and Harmony don't apply |

- Choose **Harmony** only if the framework supports it (.NET/BepInEx). On native engines, use the
  framework's own hooking system.
- If after investigating the framework is still unclear, **ask the user** before assuming.

---

## 3. Dependencies

These depend on the engine/framework detected in §2. Indicative:

- **Modding framework** per engine: BepInEx 5/6, REFramework, XV Patcher, UE4SS, or a native hook.
- **Harmony** — only on .NET/BepInEx projects. On other frameworks, their native hooking mechanism.
- **PRISM** (https://github.com/ethindp/prism) — a **cross-platform C++23** library unifying NVDA/JAWS
  (Windows), VoiceOver (macOS/iOS), Orca/Speech-dispatcher (Linux), native TTS (Android) and WebSpeech
  (Web). The **default** choice whenever the host is native or allows C++.
  - **Tolk fallback**: only on legacy .NET/BepInEx projects where integrating C++ isn't practical, via
    `TolkDotNet` (namespace `DavyKager`). `Tolk.dll` + `nvdaControllerClient64.dll` go in the game's
    root folder; only `TolkDotNet.dll` ships with the mod in plugins.

---

## 4. PRISM — integrating the prebuilt RELEASE (verified, DO NOT re-investigate)

> Checked against **PRISM v0.16.7** by downloading the asset and reading the binary/headers.
> **No need to compile the PRISM repo or use CMake/vcpkg** to consume it: the release is enough.

- **Per-platform releases** on GitHub (`ethindp/prism`, assets of the latest release). For Windows:
  **`prism-windows-x64.zip`** and `prism-windows-arm64.zip`. **There is no x86 (32-bit) build** → if the
  game/host is 32-bit, the PRISM prebuilt is NOT usable (you'd have to compile). Most modern x64 games →
  use `x64`.
- **Stable C API**: `include/prism.h` is wrapped in `extern "C"`, pure C types (`char*`, `float`,
  `size_t`, opaque pointers `PrismContext*`/`PrismBackend*`), `__cdecl` convention, `PRISM_API` macro
  (`__declspec(dllimport/dllexport)`). **No C++ type crosses the boundary** → a DLL built with **MinGW or
  MSVC** can consume `prism.dll` (which is built with MSVC) **with no ABI problems**.
- On **x64 the exports are NOT decorated** (`prism_init`, `prism_backend_speak`, …) → the most robust
  path is **dynamic loading**: `LoadLibrary("prism.dll")` + `GetProcAddress` by name. Zero link config,
  zero import lib, and you control when it loads (key if it's an injected DLL: load outside `DllMain`).
- **Windows zip contents**: `include/prism.h`; `dynamic/release/bin/prism.dll` (~1.1 MB, **the one you
  distribute**) + `tolk.dll` (~525 KB, NVDA/JAWS fallback backend); `dynamic/release/lib/prism.lib`
  (MSVC import lib, optional); a CMake package (target **`prism::prism`**). It also includes `static/`
  with a ~79 MB `prism.lib` → **it's MSVC C++: NOT linkable from MinGW** (MSVC only, yields a single
  self-contained DLL).
- **`prism.dll` runtime dependencies**: only system DLLs + UCRT (`KERNEL32`, `USER32`, `ole32`,
  `OLEAUT32`, `RPCRT4`, `UIAutomationCore`, `api-ms-win-core-*`). **Does NOT depend on
  `vcruntime140`/`msvcp140`** → **no VC++ Redistributable required**. Asian-reader backends
  (`PCTKUSR.dll`, `ZDSRAPI_x64.dll`, Orca/SpeechDispatcher bridges) load **on demand**; for NVDA/JAWS/SAPI
  **you don't have to distribute them**.
- **Minimal sequence**: `prism_config_init` → `prism_init(&cfg)` → `prism_registry_acquire_best(ctx)` →
  `prism_backend_initialize(be)` → `prism_backend_speak(be, txt, interrupt)` → … → `prism_backend_free` +
  `prism_shutdown`. Use `interrupt=true` when switching context; log the chosen backend with
  `prism_registry_name`. Others: `prism_backend_braille`, `prism_backend_is_speaking`, `set_rate/volume/pitch`.
- **Deploy**: copy `prism.dll` (+ `tolk.dll`) next to the mod DLL. That's all.

---

## 5. Reference library and section-by-section build

The reusable reverse-engineering knowledge lives in [the reference library](reference/), organized by topic →
game, with a cross-game synthesis per topic. To build a mod, read the relevant section before
implementing it.

- **General index:** [the reference library index](reference/README.md)
- **Audio navigation**:
  [the audio-navigation guide](reference/audio-navigation/README.md) — presents the **three
  navigation styles** (pick one, two, or all three depending on the game) with a **decision guide**, the
  **"which document to read for each mod section"** table, the shared model (queryable world + raycast →
  sound), and the convention differences that cause mirrored-direction bugs.
  - **Style 1 — Reactive radar** (cues as you move: wall closing in / space opening-closing). Primary
    study (.NET/MonoGame, near-source level):
    [`a-heros-call/`](reference/audio-navigation/a-heros-call/) (docs 00–10), radar in doc 02.
  - **Style 2 — Polar scan radar** (on-demand sweep of N rays). Corroborating study (native VB6, Ghidra):
    [`swamp/navigation-spec.md`](reference/audio-navigation/swamp/navigation-spec.md) §3.
  - **Style 3 — Wall sonification** (one continuous tone per direction; volume = wall proximity).
    Synthesis of our own Unity mods (Core Keeper / DRG / Megabonk):
    [`wall-sonification/`](reference/audio-navigation/wall-sonification/).
- **UI accessibility** (behavior specs, not RE knowledge):
  [the UI accessibility index](reference/ui-accessibility/README.md) — prescriptive rules
  for [menus](reference/ui-accessibility/menus.md) and
  [inventories](reference/ui-accessibility/inventories.md) (see §8).

**If you ask me to study a new engine's behavior:** copy
[the engine-study template](reference/_engine-study-template.md) into
`reference/<topic>/<game>/`, identify the engine first (§2), and fill it in with per-section confidence
notes.

---

## 6. Code architecture and best practices

- **Modular, decoupled** code with clear separation of responsibilities:
  `AccessibilityManager`, `ScreenReaderService` (PRISM), `AudioCueService`, `GameStateTracker`.
- **No accessibility logic inside the patches**: every patch/hook delegates to an external service.
- **Engine adapter**: isolate the game behind a minimal interface (e.g. `PlayerPosition`,
  `HeadingDegrees`, `Raycast(...)`, `Speak(...)`, `PlaySound(...)`). Accessibility logic is written
  against the adapter, not the engine. (See porting in `reference/audio-navigation`.)
- Every accessibility feature must be easy to **toggle on/off**.
- Prioritize readability over premature optimization; prefer composition over inheritance; **don't
  duplicate logic** across patches; **don't hardcode** offsets, fragile names, or magic numbers (derive
  them from the game's real data). Centralize accessible constants and strings in a single layer.

---

## 7. Patches / Hooks

> The mechanism depends on the framework (Harmony in BepInEx, native hooks in REFramework/XV Patcher/UE4SS).
> The principles are the same.

- Patch/hook **only** the methods you need; never alter the original logic unless strictly necessary.
- In Harmony (BepInEx/.NET): prefer **`Postfix`** for reading state; `Prefix` only when essential.
- On native frameworks: use their hooking system with the same minimalist philosophy.
- Every patch/hook delegates to an accessibility service (no logic inside the patch).

---

## 8. Accessibilizing UI (menus, inventories)

> The **behavior specs** for in-game UI live in
> [the UI accessibility specs](reference/ui-accessibility/) — prescriptive rules the patches/hooks
> delegate to. Read the relevant doc before implementing that part of a mod.

- **Menus** — [the menus spec](reference/ui-accessibility/menus.md). Trigger:
  **"accessibilize menus"** / **"accesibilizar menús"**. Covers entering a menu/screen, tabs/sections,
  vertical navigation, horizontal controls (sliders/checkboxes/dropdowns), and popup & confirmation
  dialogs.
- **Inventories** — [the inventories spec](reference/ui-accessibility/inventories.md).
  Trigger: **"accessibilize the inventory"** / **"accesibilizar inventario"**. Covers item readout,
  subsections (belt/equipment/storage), and crafting recipes (have/needed). Builds on the menu rules.

Shared principle for both: **context change → `interrupt=true`**; incremental value change → read only
what changed; always follow the game's on-screen order. Future UI specs (HUD, combat) get their own doc
in that folder.

---

## 9. Speech, audio cues and state tracking

### Speech with PRISM

- Initialize PRISM when the plugin loads / the game starts; release it on shutdown. Log the chosen
  backend (`prism_registry_name`).
- Clear, concise announcements; avoid spam. **Interrupt (`interrupt=true`) only when context changes**
  (new menu, new region/state); `interrupt=false` for incremental info.
- A **single sink** that routes to the backend, so you can swap PRISM↔Tolk↔SAPI in one place.
- The same code must work with NVDA, JAWS, VoiceOver, Orca, etc.

### Audio cues

- Use them **only** when there's no clear textual equivalent or when spatial orientation / immediate
  feedback is needed. Simple, distinguishable, reusable sounds.
- **Pause cues during dialogues and pausing menus.** Whenever an NPC dialogue is active, or a menu that
  pauses the game opens, pause/silence all audio cues — **except when the game does NOT pause on opening
  that menu** (then keep the cues running). Resume when the dialogue/menu closes. Drive this from the
  centralized accessible state (see *State tracking* below), the same way the wall-sonification bed is
  silenced in menus.

### State tracking

- Keep a **centralized accessible state**: current menu, active submenu, combat state,
  interaction/dialogue.
- **Detect real changes before announcing** (diff-gating): never repeat the same state.
- Centralize all spoken text in a single layer (translatable, outside the patch logic).

---

## 10. Conventions, logging and publishing

### Language

- **All code in English** (classes, methods, variables, namespaces) and **comments in English**.
- **Commits in English**, short and descriptive: `Add screen reader support for main menu`,
  `Refactor accessibility services`, `Patch combat state announcements`.

### Logging and debug

- Use the framework's logger (BepInEx/REFramework/XV Patcher/custom) for important states, patching
  errors, and service initialization. Logs in English. **Never log every frame.**

### Compatibility and maintenance

- Plan for future game updates. Avoid fragile offsets/names; centralize constants.

### Publishing on GitHub

- A clear, documented repo with a `README.md` in English explaining: what the mod does, which parts are
  accessibilized, and the requirements (framework if applicable, PRISM, system screen reader).
- Avoid committing unnecessary binaries. **Don't create releases automatically**: only when the user
  explicitly asks.
