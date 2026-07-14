# Accessibility Modding — Playbook (CLAUDE.md)

> **What this file is:** the manual for building accessibility mods (screen reader via **PRISM** +
> audio cues). It's a **template**: apply it game by game, section by section. The reusable
> reverse-engineering knowledge lives in [the reference library](reference/), and the general
> engineering & accessibility ground rules that apply under this playbook live in
> [`PRINCIPLES.md`](PRINCIPLES.md) — read those first.

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
4. [PRISM — integrating the prebuilt release](#4-prism--integrating-the-prebuilt-release)
5. [Reference library and section-by-section build](#5-reference-library-and-section-by-section-build)
6. [Code architecture and best practices](#6-code-architecture-and-best-practices)
7. [Patches / Hooks](#7-patches--hooks)
8. [Accessibilizing UI (menus, inventories)](#8-accessibilizing-ui-menus-inventories)
9. [Speech, audio cues and state tracking](#9-speech-audio-cues-and-state-tracking)
10. [Conventions, logging and publishing](#10-conventions-logging-and-publishing)
11. [Per-mod status ledger](#11-per-mod-status-ledger)

---

## 1. Development flow

Build **section by section**, validating with the screen reader active at every step:

0. **Open the mod's `STATUS.md` first** (the per-mod ledger — see §11). On a fresh mod, create it from
   [the status template](reference/_mod-status-template.md). It tells you what's done, what's next, and
   the offsets/symbols already derived — so you don't rebuild that context from the code each session.
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

### Cold-start dispatch table

**Start here.** Match the clue in the game's install folder → this is your engine, framework, screen-reader
transport, and which reference docs to open. One glance routes the whole mod; skip the docs that don't apply.

| Clue in install folder | Engine | Framework | SR transport | Read before coding |
|---|---|---|---|---|
| `UnityPlayer.dll` + `*_Data/`, **no** `GameAssembly.dll` | Unity (Mono) | **BepInEx 5** (Harmony) | C# P/Invoke → PRISM, else `TolkDotNet` | [SR integration](reference/screen-reader-integration/README.md) |
| Same **plus** `GameAssembly.dll` | Unity (IL2CPP) | **BepInEx 6** (Harmony, IL2CPP reflection) | C# P/Invoke → PRISM (correct IL2CPP interop) | [SR integration](reference/screen-reader-integration/README.md) |
| `*.pak` + `Engine/` + `*-Win64-Shipping.exe` | Unreal | **UE4SS** / native | Lua C module `prism_bridge.dll` → `prism.dll` | [SR integration](reference/screen-reader-integration/README.md), [UE4SS docs](<reference/UE4ss study/docs/>) |
| Capcom **RE Engine** DLLs | RE Engine | **REFramework** + native plugin / Lua (NOT BepInEx) | Native C++ plugin loads PRISM, injects `prism.*`; or C#/P-Invoke | [SR integration](reference/screen-reader-integration/README.md) |
| **Dragon Ball Xenoverse 2** | proprietary | **XV Patcher** / game mods (NOT BepInEx) | native hook → `prism.dll` | [SR integration](reference/screen-reader-integration/README.md) |
| Java + Kahlua Lua (Project Zomboid) | JVM | game Lua + Java agent | Lua → Java (**JNA**) → PRISM | [SR integration](reference/screen-reader-integration/README.md) |
| Native C/C++/VB6 exe, no managed runtime | native | native plugin / custom hook (no Harmony) | native hook `LoadLibrary("prism.dll")` | [SR integration](reference/screen-reader-integration/README.md) |
| **Decomp / native PC port** — `libultraship/` (Ship of Harkinian), `.o2r` archives, CMake, decomp-style C + C++ port layer | native (source-level) | accessibility **compiled into the exe**; a per-frame `Accessibility_Tick()` hook in the game loop; CMake build | PRISM **static-linked** (`prism.lib` compiled in) — not dynamic-load | [SR integration](reference/screen-reader-integration/README.md), [prism-prebuilt.md](reference/screen-reader-integration/prism-prebuilt.md) |

- Every engine ships PRISM via [prism-prebuilt.md](reference/screen-reader-integration/prism-prebuilt.md) (§4); the adapter contract is identical everywhere. Injected/plugin hosts dynamic-load `prism.dll`; source-level ports (decomp / libultraship) can **static-link** `prism.lib` instead.
- Choose **Harmony** only if the framework supports it (.NET/BepInEx). On native engines, use the framework's own hooking system.
- **Navigation mods:** pick the style(s) via the [audio-navigation decision guide](reference/audio-navigation/README.md) (§5).
- If after investigating the framework is still unclear, **ask the user** before assuming.

### Reflection vs. decompiler: three tools, three questions (rule from the Kakarot skill-tree episode, 2026-07-14)

They are not ranked, they answer **different questions** — pick by what you're missing, and don't grind
one tool against a wall the other walks around:

| Tool | Answers | Use it for |
|---|---|---|
| **Dumps / reflection** (CXX header dump, UE4SS ObjectDump, pak assets) | *What exists* | On-screen text, widget names, declared properties, class layouts, the **UFunction name list** |
| **Decompiler** (Ghidra) | *What the game does* | Hidden selection indices, private state, and any **predicate the game computes** (locked / owned / available / enabled) |
| **Runtime memory diff** (the F4 dev tool) | *Which address holds this value* | Pinning an offset when the static scan is ambiguous (it beat Ghidra for the battle-pause index: 540 false hits statically) |

- **Go straight to the decompiler** — do not open with in-game capture rounds — when the datum is a
  selection index, private state, or a computed predicate. Tells you it will never be reflected: the
  class **dumps as empty**, it exposes **zero reflected functions**, or the value must live in the
  **tail gap** after the last reflected member. In this game *every* hidden selection index (pause,
  field ring, community board, save/load, skill tree) turned out to be native — assume the next one is too.
- **Stop rule:** if **two** in-game capture rounds produce no readable signal, there is no third round —
  switch to native RE. (The skill tree cost seven rounds to conclude what the empty `USkillSave` dump
  said on day one.)
- They are **complementary, in this order**: the dumps make the decompiler *possible*. This game ships
  no C++ RTTI for its own classes, so the only way in is the **UFunction native-registration table**
  (ASCII name → exec thunk → impl) — and those names, plus every struct offset you check the result
  against, come from the dumps. Never skip the dump pass; just don't mistake it for the whole answer.
- The Ghidra setup cost (unpack, 51-min analysis, 16 GB heap) is **already paid** and permanent: a new
  question now costs ~3 headless minutes. Price your decision accordingly — the old "reflection first
  because native is expensive" reflex is stale.

---

## 3. Dependencies

Per the engine/framework detected in the §2 dispatch table:

- **Modding framework + Harmony**: per the dispatch table (Harmony only on .NET/BepInEx; native engines
  use their own hooking mechanism).
- **PRISM** (https://github.com/ethindp/prism) — the **default** screen-reader library on every target;
  consume the prebuilt release per §4. **Tolk** is the fallback only on legacy .NET/BepInEx projects
  (`TolkDotNet`, namespace `DavyKager`). The library choice, placement and transport per engine are in
  the dispatch table and [the SR integration index](reference/screen-reader-integration/README.md) — not
  repeated here.

---

## 4. PRISM — integrating the prebuilt RELEASE

> The full **verified** consumption reference (release layout, undecorated x64 exports + dynamic-load
> path, runtime deps / no VC++ redist, the minimal `prism_init → … → prism_shutdown` sequence, what to
> ship) lives in **[reference/screen-reader-integration/prism-prebuilt.md](reference/screen-reader-integration/prism-prebuilt.md)**.
> Read it once per mod when wiring PRISM; no need to re-investigate or compile the PRISM repo.
> For *how the call physically reaches PRISM on each engine* (the transport), see
> [the screen-reader integration index](reference/screen-reader-integration/README.md).

- **In one line:** download `prism-windows-x64.zip`, `LoadLibrary("prism.dll")` + `GetProcAddress` by
  name (x64 exports are undecorated), ship `prism.dll` (+ `tolk.dll`) next to the mod. No x86 build
  exists. Everything else is in the doc above.

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
  - **Style 1 — Reactive radar** (cues as you move: an impassable obstacle — wall, rock, tree,
    invisible barrier — closing in / space opening-closing). Primary
    study (.NET/MonoGame, near-source level):
    [`a-heros-call/`](reference/audio-navigation/a-heros-call/) (docs 00–10), radar in doc 02.
  - **Style 2 — Polar scan radar** (on-demand sweep of N rays). Corroborating study (native VB6, Ghidra):
    [`swamp/navigation-spec.md`](reference/audio-navigation/swamp/navigation-spec.md) §3.
  - **Style 3 — Wall sonification** (one continuous tone per direction; volume = proximity of the
    nearest **impassable obstacle** — walls, rocks, trees, invisible barriers, anything the player
    can't pass through).
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
> delegate to. Read the relevant doc before implementing that part of a mod. Read
> [the generic UI strategy](reference/ui-accessibility/generic-strategy.md) **first**: coverage by
> default — classify **widgets, never screens**, so reusable archetype readers cover every screen
> (including unseen ones) automatically; per-screen overrides only for genuine quirks; generic
> tree-walk fallback and fault isolation so a changed screen degrades locally instead of breaking the
> mod. **Never implement UI screen by screen.** Menus are a **fixed-cost phase**: when a shared
> pattern exists, implement it once for all menus at the same time — build the pipeline, validate on
> 2–3 representative screens, sweep the critical path. Menus must end up **100% readable** in the
> least time possible; how much of the mod that phase is depends on the genre (in point-and-click /
> visual novels the UI *is* the gameplay — same architecture, bigger share). See the workflow in the
> generic strategy doc. The menu/inventory rules below are the per-archetype read behavior that plugs
> into it.

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

**Screen detection must be EVENT-DRIVEN, never per-tick scanning** (rule from the Kakarot latency
episode, 2026-07-14). Discovering a just-opened screen by object scans (`FindAllOf`-style sweeps,
budgets, back-offs, churn re-scans, short refresh periods) creates an unsolvable trade-off: entry
latency vs navigation lag. Instead, arm ONE construction notify for the engine's widget base (UE4SS:
`NotifyOnNewObject("/Script/UMG.Widget")`) that feeds the shared widget cache, and let adapters probe
only cached refs. Details that made it correct in Kakarot (`ui_core.lua`, reuse them): feed every
tracked cache key along the constructed widget's **class chain** (adapters may key caches by a native
BASE name while instances report the blueprint class); use the **widest widget base** (detail panes
read non-UserWidget text pools); arm the notify ONCE per session with the handler reached via a `_G`
indirection (hot-reload safe); wipe the pending feed on map transitions (probing a freed object
aborts). New menu adapters must NOT add per-tick scans, churn-forcing, or refresh shortening to fix
slow detection — if a screen is slow to appear, fix the feed. Deliberately-opened sub-screens set
`confirm_ticks = 1` (the global confirmation debounce exists only for screens that flash at boot).

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

---

## 11. Per-mod status ledger

Each mod keeps a short **`STATUS.md`** at its repo root — a dashboard of where the mod is, so resuming it
is instant instead of re-derived from the code/git every session (the biggest context/time saver
day-to-day). Copy it from [the status template](reference/_mod-status-template.md).

- **Open it first** when starting a session on a mod (§1, step 0); **update it** (the *Next step* line and
  the section table) whenever you finish a chunk.
- It records: identity (engine/framework/transport/build/install, from the §2 dispatch table), a
  **section-status table** (features `done`/`wip`/`todo`), **derived facts** (offsets, symbols, type names
  already recovered — *with their source*, so they're never re-RE'd), the single **next step**, and known
  issues.
- Keep it **short** — a dashboard, not documentation. The *how* lives in `reference/`; `STATUS.md` only
  tracks *where this mod is*. Derive every value from real game data (no guessed offsets — PRINCIPLES §4).
