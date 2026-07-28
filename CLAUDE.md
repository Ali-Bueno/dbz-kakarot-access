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
- The 6 CXX/ObjectDump/SDK dumpers (`DumpAllObjects`, `GenerateSDK`, `GenerateUHTCompatibleHeaders`,
  `DumpStaticMeshes`, `DumpAllActors`, `DumpUSMAP`) are **global Lua functions callable live** (bound to
  Ctrl+J/H/Num9/Num8/Num7/Num6 in `Mods\Keybinds\Scripts\main.lua`) — no need to dump-and-close-the-game.
  Default day-to-day inspection is `dumpclass`/`probe` from `tools\ue4ss-inspector\` (console or command
  file), not a full dump pass. See `UE4ss study\docs\ue4ss-live-workflow.md` for the no-restart loop.

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

**NEVER run mod logic from an engine object-construction notify in a scripting VM** (rule from the
Kakarot crash episode, 2026-07-14 — it REPLACES the earlier "screen detection must be event-driven"
rule, which was wrong and cost two crashed sessions). Discovering a just-opened screen with a
construction notify (UE4SS: `NotifyOnNewObject("/Script/UMG.Widget")`) looks like the elegant answer to
menu-entry latency. It is a trap: **the engine constructs UMG widgets on its async LOADING thread**, and
the notify callback runs there — so the callback's Lua executes on the SAME `lua_State` as the poll loop,
concurrently. Even a two-line stash corrupts it (allocator + incremental GC race): a cached widget then
passes `IsValid()` and reports a NULL UObject on the very next member call, and UE4SS's *uncatchable* C++
throw (0xe06d7363) kills the process. MEASURE the thread before trusting any callback — expose
`GetCurrentThreadId()` from a native bridge and compare (`mem_bridge.thread_id()`); UE4SS does not
document it. Dead ends, do not retry: wrapping the callback body in `ExecuteInGameThread` (the wrapper is
itself Lua, already on the foreign thread) and moving the stash into a mutex-protected C module (reaching
C still executes Lua bytecode). A genuine event feed must be armed from a NATIVE plugin outside the VM.
**FINAL RULE (2026-07-25): use NO construction notify at all, not even for actors.** The earlier
carve-out ("actor-spawn notifies are on the game thread, so they're safe, and should log which thread
they ran on") was reasoning from *probably*: actors are normally spawned on the game thread, but UE4SS
gives no guarantee, and the cost of being wrong is a corrupted `lua_State` that crashes minutes later
somewhere unrelated. The Kakarot mod's last one (the transition gate,
`NotifyOnNewObject("/Script/Engine.GameModeBase")`) was replaced by a **game-thread poll of a world
epoch** — a persistent root's `GetWorld()` address, compared each tick — which has no thread question,
costs one guarded read, and arms EARLIER than the notify did (a new GameMode appears only after the old
world's objects are freed, and Lua ticks DO run in between because LoadMap drains the
`ExecuteInGameThread` queue through ProcessEvent). Poll a persistent root; never subscribe.
So: detect screens by **polling cached refs** — and make it fast the honest way, which costs nothing in
the steady state: pooled widgets stay valid while a screen is merely CLOSED, so re-scan a class only when
its cached list has NOTHING valid left in it (the screen was destroyed and will be recreated), gated by a
per-tick scan budget (`ui_core.lua`: `Core.first_on_screen` + `DEAD_BACKOFF`). Do not shorten the global
refresh and do not scan per tick — that is what lags navigation. Deliberately-opened sub-screens set
`confirm_ticks = 1` (the global confirmation debounce exists only for screens that flash at boot).
**Every TArray read goes through a guarded helper** (`Core.array_of`): `owner[prop]` yields an INVALID
RemoteObject, not nil, and a raw `GetArrayNum` on it is the uncatchable throw — `arr ~= nil` is not a
validity check.
**`GetAddress()` is safe ONLY on UObject-family handles.** On any other RemoteObject (a TArray
wrapper, a struct handle) UE4SS raises *"Call to RemoteObject:GetAddress on polymorphic type is not
allowed"*, and that error **pierces `pcall`** — it unwinds to UE4SS's own callback boundary, so the
calling adapter function dies mid-flight while every enclosing pcall reports success. On Kakarot one
such call inside the shared TArray helper silenced seven menus for two sessions. Keep two distinct
validity helpers: the full one (memory pre-check + `IsValid`) for UObjects, and an `IsValid`-only one
for array/struct handles (`IsValid` on those is a bare null-check, so it is safe and cheap).
**A scan SLOT is not a RATE LIMIT** (rule from the Kakarot Options regression, 2026-07-25). A
per-tick budget helper like `take_scan_slot` apportions N scans per tick *between competing callers*;
it does nothing to stop one caller from asking every single tick. An adapter that re-scans "whenever
its cache looks stale" therefore needs its own backoff as well — and before shipping one, ask what
happens if that staleness condition becomes **permanently** true. Here it did (a pooled host lingers
`on_screen` forever after its menu closes), which turned a once-per-session full-object scan into one
every 100 ms: it starved every scan-detected screen into silence AND multiplied dangling-object
exposure until the game crashed. The tell that the cause was GAME state rather than mod state was
that it survived a mod reload.
**Any signal you derive from engine state FLAPS — debounce the edge that costs something, and LOG
when it fires** (rule from the Kakarot 2026-07-25 session, where the same shape appeared twice in one
batch). Pooled widgets collapse and uncollapse; validity reads flicker during streaming and camera
work. So a raw "the HUD went away" or "the world went away" edge fires several times where the real
event happened once. Two consequences: (1) debounce the FALLING edge — require the absence to persist
— measured in WALL TIME, never in calls, because a poll invoked from several loops confirms a call
count in milliseconds and confirms nothing; the rising edge usually needs no debounce. (2) A false
fire must be visible: log the transition with its trigger. In this codebase a spurious "world gone"
armed the transition gate, and an armed gate makes the screen directory answer "absent" for EVERY
mapped class — one flicker silenced every menu for the grace period, with nothing in the log to say
why. Cheap signals that gate expensive machinery deserve more scepticism than the machinery.
**When a screen is silent and static data has not named the widget in ONE pass, stop and capture.**
An F7-style census taken WHILE the missing text is on screen names the widget path outright, and two
or three captures seconds apart show which widget's text ADVANCES — the strongest possible signal.
On Kakarot this settled in one pass what three rounds of header-grepping and hypothesising got wrong
(a story conversation had a fourth dialogue surface nobody had looked at). The rule of thumb: dumps
answer *what exists on screen right now*, headers answer *what could exist* — and "which of these
several widgets is the live one" is only ever a dump question. Budget one capture request early
instead of three guesses.
**A diagnostic that can die is worse than useless — it destroys the evidence it exists to collect.**
Dev dump tools tend to grow their own private helpers (`local valid = o ~= nil and o:IsValid()`, a
bare `pcall` around a member call) that bypass the mod's guards, and on an engine where those calls
abort uncatchably the dump then ends mid-section with no indication of where. Two rules: route the
tool through the SAME validity guards as the mod, and **write the file incrementally with a step
marker before each risky call**, so the output is a breadcrumb trail that survives the abort and
names the line that killed it.
**When several screens go quiet at once, READ THE HOST LOG BEFORE THEORISING.** A pcall-piercing
error always leaves a traceback naming the exact line; on Kakarot it named it 510 times while three
rounds of pure code reasoning produced the wrong mechanism twice.
**`IsValid()` does not lie, it FAULTS — so no guard built on it can work** (source-verified 2026-07-25,
RE-UE4SS v3.0.1 `LuaType/LuaUObject.hpp:610`): `IsValid` is
`m_cpp_object && !m_cpp_object->IsUnreachable() && is_object_in_global_unreal_object_map(...)`, i.e. it
DEREFERENCES the object before the object-set lookup that could have caught a freed handle. And UE4SS
never clears the raw pointer inside a Lua handle (its delete listener only erases a hash), so a freed
object — or one whose address a new object recycled — passes and dies at `GetClassPrivate()`,
`UObjectBase+0x10`. `GetAddress()` is the ONLY UObject method that does not dereference. Therefore the
first guard must run OUTSIDE the scripting VM: a native SEH-guarded bridge read (`Mem.alive` in the
Kakarot mod — read the object's class pointer and that class's class pointer; freed memory fails one of
them) called BEFORE `IsValid`. Derive the `ClassPrivate` offset at runtime (a UClass found by path, its
class pointer by reflection, matched against the object's first bytes) — never hardcode it. Related dead
end, do NOT spend a session on it: `bUseUObjectArrayCache = false` does not touch `IsValid`, is scoped to
startup crashes, and breaks Lua `IsA` (issue #772).
**Ask the class before fetching a member, and before treating one as an array.** A fetch of a member the
class does not declare is uncatchable, and so is `GetArrayNum` on a fixed C array. Build a per-class
property map once with `UStruct:ForEachProperty` + a `GetSuperStruct()` walk (ForEachProperty lists a
class's OWN properties only), recording each member's NAME and property TYPE: refuse an undeclared name,
and refuse `array_of` on anything that is not an `ArrayProperty`. Cache by class address, clear it on map
transition (BP classes unload; addresses get reused), budget the enumeration per tick, and log what it
blocks. Caveat that will bite you: **`RegisterCustomProperty` members are invisible to
ForEachProperty** — they live in UE4SS's own map, so every custom property must be declared to the gate
explicitly at registration.
**Guards must fail OPEN on "don't know", CLOSED only on evidence.** A crash costs one session; a guard
that silently fails closed on shared substrate costs every screen that depends on it, with no error
anywhere. This is not hypothetical: a `Core.valid` that rejected an unanswerable `GetAddress()` took out
every directory-mapped screen at once, because the screen directory gates every pointer hop on it.
Corollary (Kakarot radar regression, 2026-07-28): **a guard that widens its own scope by ASSUMPTION
takes out a working feature.** A result-validation gate dispatched "object types → check A,
*everything else with a known type* → check B", silently asserting that every non-object property
yields a handle answering `IsValid`. It does not: an `FName` is a VALUE with no `IsValid`, so the call
resolves against the **FName constructor** and raises `No overload found for function 'FName'` — a
third error that PIERCES pcall. It killed the target list on the first NPC it read, so the radar
picker never opened on any bind. Guards go on an **enumerated whitelist of cases you have evidence
for**, never on "everything else". Ask value types (FName/FText/string/number/enum) for **no validity
call at all** — convert and let the conversion be the test.
**A widget can be ON SCREEN and NOT IN THE VIEWPORT — check for a RenderTarget before trusting any
liveness test** (rule from the Kakarot Z-Encyclopedia, 2026-07-28). That book's pages
(`UAT_UICompZPageBase.RenderTarget`, drawn by `UCompZMenu.UMGRender` onto an `AZCW_BookActor`) are
rendered into TEXTURES and mapped onto a 3D mesh. They are never parented into the viewport widget
tree, so `on_screen` — an ancestor walk that ends at the viewport — returns false for a page the
player is reading, `IsInViewport` is false, and `Core.first_on_screen` finds nothing. The adapter
never activated once and **logged nothing at all, because nothing errored**: no screen commit, no
gate message, no traceback. That silence is the fingerprint — a correct-looking adapter whose name
never appears in a `screen ->` line and that produces no diagnostics whatsoever.
The tell in the header is a `UTextureRenderTarget2D` member on the page/base class, or a rendering
manager / book-actor sibling on the menu class. When you see one, the widget's OWN slate visibility
(`Core.is_visible`, `Core.first_text_offviewport`) is the only signal available; it is genuinely
weaker than `on_screen`, so the adapter must earn the screen another way — readable text plus a
marked cursor row, say. **And do not let an F7 census talk you out of this**: `discover.lua` lists
"visible on-screen text" using bare `IsVisible()`, not the ancestor walk, so a census showing the
text proves only that the widget exists — never that any viewport test would pass.
**Fail-OPEN inverts into a hazard the moment a probe expects its candidates to be ABSENT**
(rule from the Kakarot Story / Z-Encyclopedia round, 2026-07-28 — caught in adversarial review
before it ever ran). The standing rule is that guards fail open on "don't know", and for a single
member the caller believes exists that is right: the cost of being wrong is one ungated fetch on an
unknown class. A **multi-candidate** probe — "try the native spelling, then the Blueprint twin" —
has the opposite contract: most candidates are *expected not to exist*. Fail-open there is a
licence to fetch names you have positive reason to believe are absent, which is the uncatchable
abort, and it fires precisely when the property gate is unavailable: the per-tick enumeration
budget is **1 set per tick shared by every adapter**, so a screen presenting several new classes
spends several ticks completely ungated, and a class that introspects to nothing is marked
un-gateable *permanently*. So multi-candidate helpers take a STRICT gate (`Core.member(o, n, true)`
— skip the candidate when the set is unavailable, never fetch). Failing closed is acceptable here
only because it is BOUNDED: one tick of silence per newly-seen class, self-healing, and the
permanent case logs itself. That is the distinction from the Options regression, which failed
closed forever on shared substrate.
**Prefer a native TArray over Blueprint node names when the header offers one.** The census names
what is on screen; the header names what the class guarantees. Story's rewards read as
`Xlist_Reward_Bar00..05` in the dump, but `UAT_UIStartQuest.UIRewardBar_List` @0x6D0 is a real
`TArray<UAT_UIRewardBar*>` — `Core.array_of` then needs no name guessing, no fixed-array hazard,
and stays right however many rows the game shows. Check the header before hard-coding an index
range. Corollary: **`size: 0x10` on a `TArray<...>` line means a real TArray; a fixed C array is
written as a plain pointer type with `size = N*8`** (`WL_StartCharBarList` 0x30 = 6 slots). Confirm
which one you have before `array_of` — and note a fixed array collapses to element 0, so a native
name can only ever reach the first of them (`UAT_UICompZPageDetail.m_Detail` is 3).
**The liveness gate for a passive NOTICE is NOT the one for an interactive PANE** (rule from the
Kakarot reward-sheet episode, 2026-07-28 — the second time the same wall was hit). `pane_live`
(`GetVisibility() == Visible(0)` AND opacity) exists so a parked pooled pane cannot SHADOW the
adapters below it. A passive overlay — a reward sheet, a result sheet, a subtitle — cannot shadow
anything, because it speaks once and releases the dispatcher on the same tick; and in this game
passive overlays render as **HitTestInvisible / SelfHitTestInvisible**, so applying the interactive
gate to one holds it silent FOREVER. Use the opacity-only test (`Core.pane_rendered`); `on_screen`
has already dropped Collapsed/Hidden, and the opacity check still drops the close-animation ghost.
The meta-lesson matters more than the gate: `screen_fishresult` diagnosed this exact failure on
2026-07-17 and fixed it with a **private four-line helper**, so nine days later a brand-new adapter
walked into it again and never ran at all. **When a fix turns out to be about the shared substrate,
put it in the substrate** — a local workaround does not teach the next screen anything.
**Diagnosis shortcut for "an adapter says nothing":** grep the log for `screen -> <adapter>`. If the
name never appears, the bug is in `is_active`'s gates, not in the reading code — do not read the
reading code. That one grep replaced a whole round of hypothesising here.
**`RegisterCustomProperty` + `allow_member` is a TRAP — prefer the Blueprint WidgetTree name**
(rule from the Kakarot character-list episode, 2026-07-28). A fixed C array of widget pointers
collapses to element 0 under reflection, and the obvious recovery is to register each slot's raw
pointer at `base + i*8`. Two defects make it worse than the problem. (1) `Core.allow_member`
whitelists a **name**, so if the registration silently failed the gate flips from *protection* to
*permission* and the next fetch is the uncatchable abort. (2) `RegisterCustomProperty` resolves
`BelongsToClass` **once** and thereafter matches by the raw `UClass*`; BP generated classes unload
on a map transition and their addresses get reused, and `custom_props` is never flushed (unlike
`prop_sets`) — so after the first map load every slot but 0 stops resolving, the list reads one row
forever, and moving the cursor announces nothing. **Look for the WidgetTree name first:** an F7
census showed those same rows are Blueprint children (`…Start_Party_C_0.WidgetTree_0.Start_Char_Bar00`),
i.e. the engine already exposes each one as its own reflected property by name — no offsets, no
whitelist, nothing to invalidate, and an undeclared name is refused by the existence gate instead of
aborting. Reach for custom properties only when the census proves there is no name.
**Prefer a reflected INDEX over a visual cursor MARKER when the host exposes one.** A marker
(`Pnl_Curs_All`, `Img_Curs00`, a glow border) is a guess about which node the game drives, and a
marker that never clears is unfalsifiable; `GetCursorIndex()`/`GetViewIndex()` is a deterministic
read. Use the index as primary and the marker as fallback, never the other way round. And when you
must test a marker, test it with `Core.on_screen`, never `Core.is_visible` — see the next rule.
**A hand-rolled widget cache must be invalidated by `on_screen`, never by validity alone** (rule
from the Kakarot Options re-entry bug, 2026-07-25). Pooled submenu widgets are only COLLAPSED on
close, never destroyed, so a cached row from the previous visit stays `IsValid() == true` forever
while being nowhere on screen. An adapter whose staleness test is `valid(cache[1])` therefore never
re-scans, reads the orphaned widgets, finds no selection on them, and goes **permanently silent from
the SECOND entry onward** — while working perfectly the first time, because the cache started empty.
That intermittency is the tell. Adapters that fetch their host through the shared
`first_on_screen`/`cached_live` helpers are immune (those check on-screen per entry); the bug lives
in caches an adapter rolls itself. Two rules: the staleness test asks **`on_screen`**, and the
adapter's `reset()` clears the cache — `reset()` runs on every screen change, so it is the free
backstop. And budget the re-scan: a raw `FindAllOf` is ~65 ms and the test can be false for several
consecutive ticks.
**Every adapter for a pooled pane must gate on the pane being GENUINELY LIVE, not just rendered**
(rule from the Kakarot cooking-latch episode, 2026-07-15 — user directive). A pooled full-screen pane
can stay `on_screen` with readable stale content long after closing; an adapter gated only on
`on_screen` + content then SHADOWS every adapter registered below it (pause ring, emblems, …) and
re-announces the stale content on every dispatcher flip — and fixing it screen-by-screen ("yield to
the ring", "yield to X") is whack-a-mole. The live test is engine state, not content:
`GetVisibility() == Visible(0)` (a parked pooled widget keeps rendering under another
ESlateVisibility) AND `RenderOpacity > ~0` (close animations fade opacity to 0 while visibility flags
lag) — both pcall-guarded, unreadable counts as live. Reference implementation: `pane_live` +
spoken-key flip-flop suppression (survives `reset()`, cleared only on genuine close, `reannounce()`
for F1) in the Kakarot mod's `screen_cooking.lua`. Apply this check to EVERY new menu adapter whose
host is pooled.

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
- **On Lua mods, a syntax check is only half the validation — lint the compiled GLOBALS too.**
  `luac -p` happily accepts a local used ABOVE its own declaration: it compiles to a global read,
  is nil at runtime, and blows up from wherever it is called — which in a polling mod means every
  tick, often above the loop's `pcall`, i.e. a mod that is silent from boot with nothing in the
  log. The check is
  `luac -l -p FILE.lua | grep -oE '_ENV "[A-Za-z_][A-Za-z0-9_]*"' | sort -u`; everything it prints
  is a global access, and anything that is not a Lua builtin or one of the host's globals is a bug.
  Run it over every file after any edit (Kakarot, 2026-07-25: it caught one change that would have
  shipped a dead mod, plus two pre-existing latch bugs nobody had noticed).

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

## 12. Session knowledge lives IN THE REPO, never in a local memory store

Everything learned about a mod — findings, dead ends, per-subsystem offsets, working agreements —
belongs in **versioned files inside the repo**: [`STATUS.md`](STATUS.md) for *where the mod is*,
[`reference/dbz-kakarot/notes/`](reference/dbz-kakarot/notes/) for *what we know and how we learned it*
(that folder has its own index). Read them at the start of a session and update them at the end.

**Do NOT write project knowledge into an assistant-side memory folder** (user directive, 2026-07-24).
A local memory store dies with the machine, is invisible to anyone contributing through GitHub, and
silently forks from the code it describes. The repo is the single source of truth — if a fact is worth
remembering, it is worth committing.
