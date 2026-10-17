# Accessibility Modding — Playbook (CLAUDE.md)

> The manual for building accessibility mods (screen reader via **PRISM** + audio cues), applied game
> by game, section by section. It rides on every request, so it holds only routing and hard rules; the
> *how* lives in [the reference library](reference/README.md) and the engineering ground rules in
> [`PRINCIPLES.md`](PRINCIPLES.md). Other docs cite these sections by number (§1–§12): keep them stable.

## Goal

Make **[GAME NAME]** completable start to finish by a blind player, with the same mechanical and
narrative experience as a sighted player: screen-reader output through **PRISM**
(https://github.com/ethindp/prism), audio cues **only** where text can't convey spatial orientation or
immediate feedback, gameplay faithful to the original, and clean, modular code.

## 1. Development flow

Build section by section, testing with the screen reader at every step:

0. **Open the mod's `STATUS.md` first** (§11); on a fresh mod, create it from
   [the status template](reference/_mod-status-template.md).
1. Identify the engine and the modding framework (§2).
2. Identify the critical screens and states, then the methods/data to hook or read.
3. **Adapter + speech first:** `Speak("test", interrupt)` through PRISM plus one simple state
   (position/heading) on a key — this validates the whole pipeline.
4. Minimal hooks (no logic) that delegate to accessibility services.
5. Test with the screen reader and tune verbosity and timing; add audio cues only where they add value.

Navigation mods: section order and per-section reading list in
[the audio-navigation guide](reference/audio-navigation/README.md).

## 2. Investigate the engine BEFORE assuming anything

**Do NOT assume BepInEx or Unity.** Identify the engine and framework of THIS game before writing a
line; if it is still unclear after investigating, **ask the user**. Match the clue in the install
folder, then read [the SR integration index](reference/screen-reader-integration/README.md) plus the
docs in the last column:

| Clue in install folder | Engine | Framework | SR transport | Read before coding |
|---|---|---|---|---|
| `UnityPlayer.dll` + `*_Data/`, **no** `GameAssembly.dll` | Unity (Mono) | **BepInEx 5** (Harmony) | C# P/Invoke → PRISM | [engines/bepinex](file:///D:/code/modding%20projects/reference/engines/bepinex/README.md) |
| Same **plus** `GameAssembly.dll` | Unity (IL2CPP) | **BepInEx 6 bleeding edge** (Harmony, IL2CPP interop) | C# P/Invoke → PRISM (correct IL2CPP interop) | [engines/bepinex](file:///D:/code/modding%20projects/reference/engines/bepinex/README.md), [pick the build](file:///D:/code/modding%20projects/reference/engines/bepinex/bepinex-versions-and-install.md) — bundled Cpp2IL must match the metadata version; Unity 6 interop needs [repairing](file:///D:/code/modding%20projects/reference/engines/bepinex/bepinex-il2cpp-interop.md) |
| `*.pak` + `Engine/` + `*-Win64-Shipping.exe` | Unreal | **UE4SS** / native | Lua C module `prism_bridge.dll` → `prism.dll` | [SR integration](reference/screen-reader-integration/README.md), [UE4SS docs](<reference/UE4ss study/docs/>) |
| Capcom **RE Engine** DLLs | RE Engine | **REFramework** + native plugin / Lua (NOT BepInEx) | Native C++ plugin loads PRISM, injects `prism.*`; or C#/P-Invoke | [engines/reframework](file:///D:/code/modding%20projects/reference/engines/reframework/README.md) |
| **Batman: Arkham City / Arkham Knight** — `BmGame\CookedPCConsole\*.upk`, `Binaries\Win32\BatmanAC.exe` | Unreal **Engine 3** | **BmSDK** (C#/.NET 10 script hook via ASI loader — NOT UE4SS, NOT BepInEx); mods are loose `.cs` compiled at runtime | AK (x64): C# P/Invoke → `prism.dll`. **AC is x86 → no PRISM prebuilt**, decide transport first | [engines/bmsdk](file:///D:/code/modding%20projects/reference/engines/bmsdk/README.md) |
| **Dragon Ball Xenoverse 2** | proprietary | **XV Patcher** / game mods (NOT BepInEx) | native hook → `prism.dll` | — |
| Java + Kahlua Lua (Project Zomboid) | JVM | game Lua + Java agent | Lua → Java (**JNA**) → PRISM | — |
| Native C/C++/VB6 exe, no managed runtime | native | native plugin / custom hook (no Harmony) | native hook `LoadLibrary("prism.dll")` | — |
| **Decomp / native PC port** — `libultraship/` (Ship of Harkinian), `.o2r` archives, CMake, decomp-style C + C++ port layer | native (source-level) | accessibility **compiled into the exe**; a per-frame `Accessibility_Tick()` hook in the game loop; CMake build | PRISM **static-linked** (`prism.lib` compiled in) — not dynamic-load | [prism-prebuilt.md](reference/screen-reader-integration/prism-prebuilt.md) |

- Plugin/injected hosts dynamic-load `prism.dll`; source-level ports may static-link `prism.lib`. The
  adapter contract is identical everywhere (§4).
- **Harmony** only on .NET/BepInEx; native engines use their framework's own hooks.
- **Unreal/UE4SS:** never dump-and-relaunch — answer questions in place by reflection with the
  zero-restart loop ([`reference/UE4ss study/docs/ue4ss-live-workflow.md`](<reference/UE4ss study/docs/ue4ss-live-workflow.md>))
  and [`tools/ue4ss-inspector/`](tools/ue4ss-inspector/) (console or command file; the Live View GUI
  is ImGui and not screen-reader accessible).
- **Unity:** Mono → net472 + `BaseUnityPlugin`; IL2CPP → BepInEx 6 bleeding edge + net6.0 + `BasePlugin`
  + generated interop. On IL2CPP, get BepInEx loading and the interop repaired **before** any mod code
  (run [`templates/bepinex-interop-fixer/`](file:///D:/code/modding%20projects/templates/bepinex-interop-fixer/) on Unity 6 targets).

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

## 3. Dependencies

The framework (and Harmony, where it applies) per the §2 table, plus **PRISM** — the only screen-reader
library on every target (no Tolk fallback). Its official bindings are **Prismatoid**: .NET (NuGet,
targets **.NET 10**, so not loadable in BepInEx net472/net6 hosts — P/Invoke `prism.dll` there), Python
(PyPI, for tooling) and Godot; the core is a stable C API.

## 4. PRISM — integrating the prebuilt RELEASE

Download `prism-windows-x64.zip`, `LoadLibrary("prism.dll")` + `GetProcAddress` by name (x64 exports are
undecorated) and ship **only** `prism.dll` next to the mod: the release's `tolk.dll` is not needed
(NVDA/JAWS/SAPI are built in). No x86 build exists. The verified details — exports, runtime deps, the
`prism_init → … → prism_shutdown` sequence, what to ship — are in
[prism-prebuilt.md](reference/screen-reader-integration/prism-prebuilt.md); read it once per mod and
don't re-investigate or compile the PRISM repo.

## 5. Reference library and section-by-section build

Read the relevant section of [the reference library](reference/README.md) before implementing it.

- **Audio navigation** — [the guide](reference/audio-navigation/README.md) has the decision guide, the
  per-section reading list and the convention differences behind mirrored-direction bugs. Pick one or
  more styles: reactive radar ([`a-heros-call/`](reference/audio-navigation/a-heros-call/), doc 02),
  polar scan radar ([`swamp/navigation-spec.md`](reference/audio-navigation/swamp/navigation-spec.md)
  §3), wall sonification ([`wall-sonification/`](reference/audio-navigation/wall-sonification/)). An
  obstacle is anything impassable: walls, rocks, trees, invisible barriers.
- **UI** — §8.
- **Studying a new engine:** copy [the engine-study template](reference/_engine-study-template.md) into
  `reference/<topic>/<game>/`, identify the engine first (§2), and note confidence per section.

## 6. Code architecture and best practices

- Decoupled services: `AccessibilityManager`, `ScreenReaderService` (PRISM), `AudioCueService`,
  `GameStateTracker`. **No accessibility logic inside patches** — they delegate to a service.
- **Engine adapter:** accessibility logic is written against a minimal interface (`PlayerPosition`,
  `HeadingDegrees`, `Raycast(...)`, `Speak(...)`, `PlaySound(...)`), never against the engine.
- Every feature toggles on/off. Accessible constants and strings live in one layer; offsets and names
  are derived from real game data, never hardcoded (PRINCIPLES §5).

## 7. Patches / Hooks

Hook **only** what you need and never alter the original logic unless strictly necessary. Harmony:
prefer **`Postfix`** to read state, `Prefix` only when essential. Native frameworks: their own hooks,
same minimalism. Every hook delegates to an accessibility service.

## 8. Accessibilizing UI (menus, inventories)

Read [the generic UI strategy](reference/ui-accessibility/generic-strategy.md) **first**. Classify
**widgets, never screens**: reusable archetype readers cover every screen (unseen ones included),
per-screen overrides only for genuine quirks, a generic tree-walk fallback and fault isolation. **Never
implement UI screen by screen.** Menus are a fixed-cost phase: build the shared pipeline once, validate
on 2–3 representative screens, sweep the critical path — 100% readable in the least time.

- **"accessibilize menus" / "accesibilizar menús"** → [the menus spec](reference/ui-accessibility/menus.md)
  (entering screens, tabs, vertical lists, sliders/checkboxes/dropdowns, popups and confirmations).
- **"accessibilize the inventory" / "accesibilizar inventario"** →
  [the inventories spec](reference/ui-accessibility/inventories.md) (item readout, subsections, crafting
  have/needed); builds on the menu rules.
- Context change → `interrupt=true`; a value change reads only what changed; follow the on-screen order.

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
**Beware the SELF-SUSTAINING quiet/scan deadlock, and never let a fast loop depend on a slow one**
(rule from the Kakarot map-d-pad episode, 2026-07-28 — three fix attempts before it was found).
Quiet mode defers a pool's scan; an adapter commits only once it SEES that pool; and with no
committed adapter the quiet heuristic stays satisfied. The loop closes on itself and is broken only
by the player happening to press a face button (which lifts the hot window) — which is exactly why
the symptom reads as "sometimes it works, sometimes it doesn't". Any pool an adapter needs *in
order to activate at all* belongs in `QUIET_EXEMPT`. Two companion rules from the same bug:
- **A 20 ms pad loop must not call scanning helpers.** It never runs `Core.begin_scan_tick`, so it
  never REFILLS the shared budget while draining it five times faster than the 100 ms registry
  does — starving the very scan it is waiting on. Use `Core.peek_all` (directory list or cached
  pool, never scans) on fast loops; leave `Core.cached_all` to the registry side.
- **Commit input edges BEFORE any early return.** `ft_prevbtn` was updated at the end of the
  handler, after a "no data yet" bail-out, so every press made during the warm-up was swallowed
  without even being remembered. Read the pad once at the top, compute the edges, store the new
  button state immediately, and LATCH a press you cannot serve yet so it fires when you can.
**Every new adapter TAXES THE WHOLE MOD: each class it names joins the ABSENT scan set** (rule from
the Kakarot 2026-07-28 stutter report). A class that is never present still costs a full
`FindAllOf` every `ABSENT_BACKOFF` (~4 s) forever, and `ui_core`'s own comment records that a
cluster of those expiring on the same tick is precisely what a periodic stutter feels like. Two new
menu adapters added ELEVEN class names and the player felt it within a session. So:
- **No speculative class names.** A native fallback alongside a census-proven Blueprint name looks
  like cheap insurance; it is a permanent scan for a class that has never once been observed. Name
  only what a dump has actually shown.
- **Gate the probe on a cheap precondition.** A menu can only be open when a menu owns the screen,
  and `Core.free_roam(tick)` is the game's own signal for that (the minimap is up while walking
  around and hidden the moment any menu, battle or cutscene takes over). It reads an
  always-present class, so it is a cached pointer walk, not a scan. Put it FIRST in `is_active`.
- Prefer a screen-directory pointer mapping over any scan when an owner field exists.
The measurement, when a stutter is reported rather than reasoned about: `_G.__KakarotScanStats`
carries per-class attribution and Ctrl+F5 dumps the top offenders — use it before optimising blind.
**An adapter must claim the tick on READABLE TEXT, never on a live handle** (rule from the Kakarot
recovery-tab episode, 2026-07-28; the same shape was caught in review on the Z-Encyclopedia days
earlier). `screen_itemuse` returned active whenever a character bar was rendered, without checking
that the bar yielded a name. On the item menu's RECOVERY tab the game permanently renders the whole
party strip, so the adapter claimed that tab forever and then had nothing to say — the tab was
silent, and it was the ONLY silent tab, because it is the only one that shows the strip. An adapter
that holds the tick and says nothing is indistinguishable from a broken screen AND it shadows every
adapter registered below it. Two corollaries: gate `is_active` on the text you are about to speak,
and when a "the selected one is the only one on screen" assumption comes from a screenshot of the
selecting mode, re-check it against the screen AT REST — require UNIQUENESS, because several on
screen means you are looking at a list, not a selection.
**A THROTTLED lazy-collect plus a `reset()` that nils the cache is a fault waiting to happen.**
`screen_palette` rebuilt its node cache only when a throttle window allowed it, while `reset()`
(which runs on every screen change) set the cache to nil — a reset landing inside a live window
left the cache nil and the next line indexed it. The registry's pcall isolation converts that into
`adapter '<name>' faulted in is_active`, the adapter counts as inactive, the screen below claims,
and the next tick faults again: a flip-flop that re-announces the whole screen several times a
second. Clear the throttle in `reset()`, and bail out explicitly if the cache is still absent after
the collect attempt. The log line naming the faulting adapter is the fastest way in — grep for
`faulted in is_active` before anything else.
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

**Never queue `ExecuteInGameThread` from code that is ALREADY running inside one** (rule from
the Kakarot `questdump` freeze, 2026-09-08, which also explained the `navdump` freeze of
2026-08-18). RE-UE4SS 3.0.1 drains its action vector inside a `std::remove_if` and runs each
callback from within it (`LuaMod.cpp:2919-2948`); a nested call does `emplace_back` on the same
vector under the live iterators, and the reallocation that eventually follows is undefined
behaviour that presents as a silent HANG — the callback never runs, so a dump file is never even
created, nothing is logged, the process stays up. It works while the vector has spare capacity,
which makes it look like a size or a safety problem for weeks. `main.lua`'s F10 comment had
already refused to do this on the grounds that UE4SS documents nothing; the source now says why.
Modules wrap their body for the keybind path (worker thread → game thread), and anything that
calls them from a game-thread context — the dev channel, a hook, another action — must call the
body directly. In the Kakarot mod that is centralised: `dev_channel.dispatch` shadows the global
with "call it now" for the duration of a command.

**A reflection walk that TRUNCATES is indistinguishable from a complete one unless you check where
it STOPPED — and a cached "the class has no X" derived from a truncated walk is a guard that fails
CLOSED forever on shared substrate** (rule from the Kakarot skill-tree crash episode, 2026-07-29). A
per-class property-name set built by walking `GetSuperStruct()` can stop early — the walk raises, hits
a depth cap, or the enumeration itself raises partway — and a set that stopped at the Blueprint class
without its native base looks exactly like a real one: same shape, same cache slot, same lifetime. A
gate that trusts it refuses every inherited member forever, with one deduped log line and no error, on
a member that reads fine before the truncated set gets cached. Mark the walk PARTIAL and let an absent
name fail OPEN there, never closed. The natural-looking end condition, `GetSuperStruct() == nil`, is
itself a trap: UE4SS answers an INVALID handle at the true root, not nil, so that test never fires and
everything looks partial forever — use the ROOT CLASS NAME (`Object`) as the discriminator instead,
matching every other super-walk in this codebase, which already terminates on `not IsValid`.

## 9. Speech, audio cues and state tracking

- **Speech:** init PRISM on load, release on shutdown, log the backend (`prism_registry_name`). One sink
  for all output, so a backend swap or a test mock happens in one place. Concise announcements;
  `interrupt=true` only on context change (new menu, region, state), `false` for incremental info.
- **Audio cues:** only where text has no clear equivalent or spatial/immediate feedback is needed;
  simple, distinguishable, reusable. **Silence all cues during NPC dialogue and menus that pause the
  game** (keep them running if that menu doesn't pause it) and resume on close, driven from the
  centralized state — the same way the wall-sonification bed goes quiet in menus.
- **State:** one centralized accessible state (menu, submenu, combat, interaction/dialogue).
  **Diff-gate:** announce only real changes. All spoken text in one translatable layer.

## 10. Conventions, logging and publishing

- Code, comments, logs and commits in **English**; short commits (`Add screen reader support for main menu`).
- Framework logger for init, important states and patch errors; **never log every frame**.
- **On Lua mods, a syntax check is only half the validation — lint the compiled GLOBALS too.**
  `luac -p` happily accepts a local used ABOVE its own declaration: it compiles to a global read,
  is nil at runtime, and blows up from wherever it is called — which in a polling mod means every
  tick, often above the loop's `pcall`, i.e. a mod that is silent from boot with nothing in the
  log. The check is
  `luac -l -p FILE.lua | grep -oE '_ENV "[A-Za-z_][A-Za-z0-9_]*"' | sort -u`; everything it prints
  is a global access, and anything that is not a Lua builtin or one of the host's globals is a bug.
  Run it over every file after any edit (Kakarot, 2026-07-25: it caught one change that would have
  shipped a dead mod, plus two pre-existing latch bugs nobody had noticed).
- **Project knowledge goes in the repo, never in assistant memory** (auto-memory included; see §12):
  [PRINCIPLES §3](file:///D:/code/modding%20projects/PRINCIPLES.md#project-knowledge-lives-in-the-repository). Mods built from this template
  and how to sync them: [`MODS.md`](file:///D:/code/modding%20projects/MODS.md).
- Plan for game updates: no fragile offsets or names, constants centralized.
- Publishing: English `README.md` (what is accessible, requirements: framework, PRISM, screen reader); no
  unnecessary binaries; **no releases unless the user asks**.
- **Every link you hand to a user points at `/releases/latest`, never at a tag** (user directive,
  2026-07-28). An announcement lives on a forum or a Discord for years; a tag link starts serving
  the wrong version the day the next release ships. Use the direct, always-current asset URLs —
  `…/releases/latest/download/<AssetName>` — which is why `package.ps1` deliberately keeps the asset
  names version-free. A tag URL is fine in one place only: inside that release's own notes.
- **Announcements are short and informal.** Lead with the one thing that matters to a player, in a
  sentence. Keep the caveats for the changelog, which is where someone goes when they want detail.

## 11. Per-mod status ledger

Each mod keeps a short `STATUS.md` at its root ([template](reference/_mod-status-template.md)): identity
(from the §2 table), a section table (`done`/`wip`/`todo`), derived facts with their source so nothing is
re-RE'd, a *Code map*, the single next step and known issues. Open it first; update the next step and the
table whenever a chunk is done. A dashboard, not documentation — the *how* lives in `reference/`.

## 12. Session knowledge lives IN THE REPO, never in a local memory store

Everything learned about a mod — findings, dead ends, per-subsystem offsets, working agreements —
belongs in **versioned files inside the repo**: [`STATUS.md`](STATUS.md) for *where the mod is*,
[`reference/dbz-kakarot/notes/`](reference/dbz-kakarot/notes/) for *what we know and how we learned it*
(that folder has its own index). Read them at the start of a session and update them at the end.

**Do NOT write project knowledge into an assistant-side memory folder** (user directive, 2026-07-24).
A local memory store dies with the machine, is invisible to anyone contributing through GitHub, and
silently forks from the code it describes. The repo is the single source of truth — if a fact is worth
remembering, it is worth committing.

## 13. Working model: expensive model as director

The orchestration rules live in the user-level `~/.claude/CLAUDE.md`, which loads in every repo and wins
on conflict. For modding they mean: the Opus/Fable main session forms the RE hypotheses, decides the
architecture (adapter, services, hooks), writes the critical code and the `STATUS.md` update. Bulk output
— decompiler dumps, IL2CPP symbol lists, engine-folder sweeps, build logs, web research — goes to
subagents that return `path:line` / RVA / symbol + conclusion, so the director's context stays small.
Pick the cheapest tier that gets it right: Haiku 5.5+ for simple tasks (build/test errors only, log
filtering, file and symbol lists, engine-identification sweeps); Sonnet for reasoning (call-flow
tracing, subsystem summaries, `reference/` drafts); Opus for the hard separable parts (gnarly decompiled
functions, struct/vtable layouts, crashes and hangs in the injected mod). Independent investigations go
out in parallel in one message.

### Token efficiency

Every message re-sends the whole context, so its size is what costs. Evidence and the available settings:
[token efficiency](reference/token-efficiency/README.md).

- **Short sessions:** one task per session; resume from `STATUS.md` and its *Code map*; `/clear` between
  unrelated tasks.
- **Narrow search:** `Grep`/`Glob` and `Read` with line ranges; broad sweeps go to the Sonnet `Explore`
  agent ([setup](templates/claude-code/README.md)).
- **Delegate only bulk:** a subagent starts with a cold cache and reloads every `CLAUDE.md`; a known
  lookup is cheaper done directly. Always pass `model` — without it the subagent runs on the main model.
- **No code-index tooling:** graphify, Serena and vector-search MCPs were rejected, LSP plugins deferred.
- **Measure** with `/usage` (`w` = last 7 days) and `/context`.
