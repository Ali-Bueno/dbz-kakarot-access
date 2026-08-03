# dbz-kakarot-perf-architecture

> DBZ Kakarot perf architecture — VERIFIED CLOSED 2026-07-17 (cinematics + loading smooth, user-confirmed): screen directory, sticky-active registry, quiet mode, watch lane, scan budgets, pad_poll scheduler. Rules for future code.


## 2026-08-03 (d) — MEASURED AT LAST: 29% OF THE GAME THREAD IS `FindAllOf`, AND 71% OF THAT FINDS NOTHING

First dump taken with the complete meter (Ctrl+F5, 410 s of ordinary play):

```
ui step ms: max=925.0 avg=35.48 over 4102 ticks
findall scans: n=1982 total_ms=119463 max_ms=455.0 avg_ms=60.3
ghost classes: 42 never found (1397 scans, 84783 ms burned)
```

The arithmetic that matters: **a scan averages 60 ms** (worst 455) and `SCANS_PER_TICK = 2` on a
100 ms grid, so the scan machinery is permitted to spend **120% of a tick**. That is not a budget,
it is a licence, and `ui step ms: max=925.0` is what it buys. Everything else queues behind it —
which is why "menus take about a second to start reading" and "the radar is slow to resume after a
battle" turned out to be ONE bug, not two.

**Reading the ghost list correctly.** `found = 0` means "no scan of this name ever returned an
instance", and that covers two different populations:
1. Names the game never instantiates (`AT_UIQteMashAlert` 10.0 s, the five `CompZ_Page_*`/`Memo`
   ~25 s, the three `Start_Commu_*` ~15.5 s, `Map_World_Icon_C` 6.5 s).
2. **Directory-mapped classes whose SCAN IS ONLY THE FALLBACK** — they scan precisely when the
   screen is absent, so every one of those scans is waste, but the fix is different. The tell is
   `AT_UIMiniMapRadar` sitting in the list while the radar plainly works.
   The structural point behind population 2, worth keeping: **a mapped class whose own ROOT is
   unreachable correctly falls back to scanning** (an unreachable root cannot assert absence), so a
   screen rooted somewhere that does not exist right now scans forever *exactly while it is
   impossible*. `Gametitle_C` (9.6 s) + `AT_UIXcmnAgreement` (8.5 s) = 21% of the whole ghost bill,
   all of it during gameplay, where the title cannot exist. The fix is a POSITIVE precondition —
   `Core.gameplay_world()`, the `mm` root — not a negative one.

**Shipped against it:** ledger-driven escalation of `ABSENT_BACKOFF` (4→8→16 s, disarmed forever by
one sighting, never applied to `QUIET_EXEMPT`, always bypassed by the boost window) and the
gameplay-world gate on the two boot adapters. Deliberately not touched: `SCANS_PER_TICK`. Raising it
worsens the spike the 2026-07-14 dump measured; lowering it starves first detection. The honest fix
is fewer classes asking, not a different number of slots.

**The ledger's blind spot, learned the hard way the same day.** A class the directory resolves by
pointer only ever SCANS when the screen is genuinely absent, so it sits in the ghost ledger at
`found = 0` looking identical to a name that does not exist. For a screen that is merely wasted
work; for a **PREDICATE** — `AT_UIMiniMapRadar` (free roam + the nav world gate), `Start_Top_C`
(ring open), `Battle_Hud_P_Main_C` (in battle) — it is a trap, because other machinery is gated on
the answer and the fallback matters most exactly when the pointer path cannot answer (right after a
load, roots flushed). Hence `NEVER_ESCALATE` in `ui_core`. GENERAL RULE: **a measurement that cannot
distinguish "never exists" from "not reachable right now" must not be allowed to drive a decision
whose failure mode is silence.** The ledger is honest about cost and blind about meaning.

**Latency bugs found in the same passes, unrelated to scanning:**
- `nav_tracker.minimap()` wrote its 5 s retry lockout BEFORE running the probe, so one miss cost the
  full 5 s — and `world_alive()` is false throughout, i.e. the entire nav loop is dead, not just the
  minimap. Arm a backoff on the OUTCOME, never on the attempt. (The 5 s itself was also stale: it
  was sized when acquisition was a raw `FindAllOf`, and `cached_live` has owned that cost since the
  directory landed.)
- Two separate "fire on `tick % N == 0`" gates carried an arbitrary phase across a battle or a load,
  because the tick counter free-runs while the gates are shut: up to 1.4 s of silence after control
  is already back. A modulo on a free-running counter is a RANDOM delay, not a cadence, whenever
  the thing it gates can be blocked for a while — measure from the last attempt instead, or reset
  the phase on the rising edge.
- `Nav.SW.lists` (the sweep's build) is cleared only by `Nav.sweep_partial()`, and three of the five
  `list_targets` call sites never called it. `class_list` serves a cached list without re-scanning,
  so after one such call **the sweep never scanned again** and kept serving actor handles gathered
  before the battle. Now self-healing at build entry: only a build that reported PARTIAL may be
  resumed. LESSON, the same one as the fishresult gate: a contract that says "every caller must
  remember to call X" will be broken by most callers — make it structural.
- `confirm_ticks = 1` was inert. `ui_registry`'s first-sighting branch returned unconditionally, so
  no candidate could commit before its second tick and `= 1` was identical to the default 2. Five
  adapters had opted out of a 100 ms debounce and were paying it anyway.

## 2026-08-03 (c) — THE SCAN SET, AUDITED: 35 CLASSES NOTHING RESOLVES, AND A METER THAT COULD NOT SEE THE BIGGEST SCANNER

A full sweep of every call into the scanning helpers (`Core.cached_all` / `cached_live` /
`first_on_screen` / `peek_all` / `first_text_offviewport`, plus every raw `FindAllOf`) across the 75
shipped Lua files. What it found:

**1. 35 distinct classes across 19 production adapters have no `ui_directory` chain**, so each is
resolved by a full-object scan. Grouped by owner: `keyhelp` (Xcmn_Keyhelp_C); `nav_tracker`
(AT_UIMiniMapNaviIcon); `screen_choice` (Choice_Win_C, AT_UIChoiceWin, Choice_Cmd_C,
AT_UIChoiceCmd); `screen_choicelist` + `screen_questreward` (Xcmn_Win01_List_C);
`screen_community` (AT_UICommunityStart, Start_Commu_Emb_C, Start_Commu_Brd_C); `screen_compz` (six
`CompZ_Page_*` / `CompZ_Memo_C`); `screen_dialog` (Xcmn_Win00_Choice_C); `screen_dialogue`
(Xcmn_Subtitles_C, ATUISubtitles, Field_Talk_Win_C, Xcmn_Header_C); `screen_fishing` /
`screen_fishresult` (AT_UIBattleRushSpeedCore, Mgame_Result_C); `screen_list` / `screen_palette` /
`screen_tutorial` (CFUIMultiLineTextBox); `screen_loading` (Xcmn_MultiLineText_C); `screen_map`
(Map_World_Curs_C, Map_World_Icon_C); `screen_options` (Xlist_Bar03_C); `screen_palette`
(Start_Item_Bar_C); `screen_questreward` (Quest_Sub_Reward_C, AT_UIQuestSubReward); `screen_shopcmn`
(Xlist_Bar04_C); `screen_status` (AT_UIStartStatusList01); `screen_telop` (Quest_Main_Telop_C);
`screen_toasts` (Info_Log_C, Info_Log02_C). Many are legitimately unmappable — they are ROWS and
text pools, not top-level screens, and the directory maps screens by owner pointer.

**2. Four native/Blueprint TWIN pairs, and the two sources contradict each other on what they
cost.** `Choice_Win_C`/`AT_UIChoiceWin`, `Choice_Cmd_C`/`AT_UIChoiceCmd`,
`Xcmn_Subtitles_C`/`ATUISubtitles`, `Quest_Sub_Reward_C`/`AT_UIQuestSubReward`. RE-UE4SS's shipped
docs (`docs/lua-api/global-functions/findallof.md:3`, `assets/API.txt:110-112`) say `FindAllOf`
returns "all **non-default** instances of the supplied class name" **including subclasses** — under
which reading the native name alone would cover both and the `_C` twin is redundant. But this
repo's own comments, written from observation on THIS game, say the opposite (`nav_tracker.lua:2925`
"a native-base FindAllOf can return nothing when a subclass exists", `:2951`, and the community-board
lesson at `:3198`). The likely reconciliation is that the *class lookup by short name* fails when the
native `UClass` is not loaded, so the answer depends on load state — but that is a hypothesis, and
either way **one half of each pair is a permanent ghost scan**. Note the caveat on the docs claim: the
actual comparison lives in the un-vendored `UEPseudo` submodule, so it is a documented claim, not
source-verified.

**3. The meter had a blind spot big enough to invert the ranking.** `timed_findall` only saw scans
routed through `Core.cached_all` / `Core.first_live`. Everything raw — the radar sweep's ~17 scans
(~68 ms each, ~1.2 s cold), the navi-icon pool on its own 10 s cadence, `AT_Character`,
`RecastNavMesh`, the Options rows — was invisible, which `nav_tracker.lua:2560-2562` stated in a
comment and nobody acted on. Any offender list built before 2026-08-03 was therefore ranking the
budgeted scans while ignoring the unbudgeted ones.

**What shipped (phase 1, source-only):** `Core.findall` (timed entry point for the raw sites —
accounting only, no budget/backoff), `Core.scan_ledger` (session-persistent, per class the most
instances ever returned; `found = 0` = ghost), and a `ghost classes:` block in the Ctrl+F5 dump.
Deliberately NOT shipped: any change to `ABSENT_BACKOFF` or any class-name deletion — the rule at
the bottom of this file ("do NOT blind-tune ABSENT_BACKOFF without an offender list, it starves
event-less popups") is the whole reason phase 1 is a meter and not a fix. Escalating the backoff for
proven ghosts is the phase-2 candidate; note the boost window (`Core.boost_missing`, ~1.5 s, armed by
registry commits and pad presses) already covers any screen a player OPENS, so the exposure of a
longer backoff is limited to surfaces that appear with no press — which is precisely what
`QUIET_EXEMPT` already enumerates.

## 2026-08-03 — THE GAMEMODE IS A BAG OF ~50 MANAGERS, AND WE USE EXACTLY ONE

Source: the first UE4SS dump ever taken **in gameplay** on this game (`Area11_P`, 2026-08-03;
every earlier one was captured at the title screen, which is why the gameplay Blueprint classes
had always come back as empty stubs). Nothing below is implemented — it is a work list.

**The pattern already exists in the mod and was only ever used once.** `ui_directory.getters.mm`
resolves `MenuManager` by `FindAllOf` once + cache forever. The object dump shows the SAME
GameMode instance carrying dozens more components under
`Area11_P:PersistentLevel.BP_ATGameModeMain_C_0.*` — `QuestManager`, `NpcManager`,
`NpcVisibleMeshManager`, `BattleManager`, `ATWindRoadManager`, `PlacementObjectInfoManager`,
`CrossTalkManager`, `HudManager`, … (`UE4SS_ObjectDump.txt:285492-285700`). None are reflected as
named fields on native `AATGameMode` (`AT.hpp:13208-13256` has no such members), so each costs
exactly what `MenuManager` costs today: one `getters.X` entry, one scan, cached forever. **No new
UE4SS surface, same risk profile as what already ships.**

**Why this matters: it attacks `Nav.list_targets()`**, which the mod's own comment calls its
"single most expensive and most dangerous operation" — 17 unbudgeted `FindAllOf`, ~68 ms each,
≈1.2 s of game thread in one tick (`nav_tracker.lua:2016-2018`), flagged as needing "a cheaper
source" and never followed up. Concrete replacements found:

| Scanned today | Reachable instead via |
|---|---|
| `FindAllOf("ATWindRoad")` (`nav_tracker:2958`) | `GameMode.ATWindRoadManager.WindRoadList` — a real `TArray<AATWindRoad*>` (`AT.hpp:29801`) |
| `FindAllOf("PlacementObjectInfo")` (`:2934`) | `GameMode.PlacementObjectInfoManager.PlacementObjectInfoList` (`AT.hpp:42642`) |
| `FindAllOf("QuestCharacter")` (`:2805`) | `GameMode.QuestManager.QuestCharacterFindList` (`AT.hpp:42856`) or `NpcVisibleMeshManager.NpcCharacterList` (`AT.hpp:42525`) |
| `FindAllOf("ATMapIconComponent")` (`:2763`) | **Needs no new root at all** — `fm.FieldIconManager.IconActorArray[i].UserComponent` hangs off the already-mapped `"fm"` root (`AAT_UIFieldManager.FieldIconManager` @0x698, `AT.hpp:32911`; back-ref at `AT.hpp:14812`). Bonus: that array is the RENDERED icon set, likely smaller than the class scan. |
| `FindAllOf("AT_UIMiniMapNaviIcon")` (`:603`, a ~10 s hot loop, and `:2721`) | The already-resolved minimap root's `MapNaviIconList` (`AT.hpp:34970`) — **but it dumps as a single `ObjectProperty` of `size 0x50`, i.e. a FIXED C ARRAY of 10 that collapses to element 0.** That is the exact shape `ui_core.lua:716-728` documents, and the only recovery is `RegisterCustomProperty` — which the playbook separately calls a trap. Not a cheap win; treat as its own decision. |
| `FindAllOf("AT_Character")` (`:1252`), `FindAllOf("AT_MobBase")` (`:3052`) | **No manager array found.** `UNpcManager` only holds `Dinosaurs` (`AT.hpp:42350-42385`). This pair is the one genuine case for the Sparking ZERO acquisition hook below. |

Not searched exhaustively (44k lines): owners for `Portal`, `LevelNavigator`,
`ATEnemiesBaseBehaviour`, `FieldPointComponent`. "Not found" here means not found, not absent.

**The Sparking ZERO approach (`RegisterBeginPlayPostHook`), assessed at last.** It fires once per
actor as it finishes `BeginPlay`; a mod matches `GetClass()` and caches. Two hard limits this game
imposes, neither of which the architecture doc spells out:
- **It is anchored on `AActor::BeginPlay`, so it fires for ACTORS ONLY.** Every manager above is a
  `UActorComponent` and every screen in `ui_directory.MAP` is a widget/`UObject`. **The entire
  widget half of the mod's scanning gets zero benefit.** Its whole value is `nav_tracker`'s world
  side — i.e. essentially the `AT_Character` / `AT_MobBase` pair the table above could not solve.
- **It inherits `tick_hook.lua`'s boot hazard, and worse.** That hook crashed the game at boot
  because the mod's own guards are not armed that early; a global `BeginPlay` hook fires for every
  actor in the boot and title sequence, i.e. far earlier and far more often. It needs the same
  "arm only once in confirmed gameplay" gating, proven in isolation first.

**Other findings from the same pass (accessibility, not perf):**
- **`UCrossTalkManager.LipSyncOwner`** (`AT.hpp:40754`, `AAT_Character*`, live at
  `UE4SS_ObjectDump.txt:285537`) — a direct pointer to **whoever is currently being lip-synced**.
  This is a candidate answer to BOTH open dialogue problems: the `nav_mute` cutscene-vs-ambient
  ambiguity (currently a surface list plus a timer) and the "don't read subtitles that are already
  voiced" request. UNKNOWN and only testable in game: whether it is set for ambient field chatter
  too, or only for formal scenes.
- **`UAT_UIBattleManager` is half unread.** The mod already holds this root every battle tick and
  reads four fields; sitting unused: `LockonCursor` (0x1F8), `BattleCond` (0x258, buff/debuff
  display), `SkillChain` (0x278), `CommandPalette` (0x290), `GaugeDurable` (0x2B8, guard
  durability), `BattleTime` (0x2D0), `BattleEscape` (0x318), `TPSSkill*` (0x208-0x218).
- **`AAT_GameHUD.m_pPlayer`** (`AT.hpp:14690`) — a more direct player-character pointer than
  `nav_tracker.lua:344-356`'s minimap→`PlayerIns`→`FindFirstOf` chain.
- **`UAT_UIGameover.CurrentSelectIndex`** (`AT.hpp:33473`) is a REAL reflected int32. The notes'
  "this game does not reflect selection indices" conclusion was about the battle PAUSE ring
  specifically, not universally — do not over-generalise it.

**Hooking, from the same pass.** `Field_Talk_Win_C:Construct()` (`Field_Talk_Win.hpp:16`) and
`Quest_Sub_C:Construct()` (`Quest_Sub.hpp:84`) are genuine Blueprint overrides and are the same
*category* of engine boundary as the `ReceiveTick` that works — the best remaining candidates, but
extrapolated from two data points, untested. **The whole `OnBattleStart` / `OnBattleEnd` /
`OnQuest` family is NATIVE** (`AT.hpp:13252`, `:40030`, `:42370`, `:41954`, `:42531`) — never on a
`_C` class — and this game is 3-for-3 on native hooks silently never firing (`header_hook.lua`).
Do not build on them without a cheap isolated test. Battle/pause/results/game-over/loading/quest
widgets have NO Blueprint override at all and must stay on the resolve-then-poll path.

Caveat on all of it: one live capture, one field map. The core managers are systemic rather than
level content, so they should be present on every field GameMode — but that is an inference.

---

**The measured problem (Ctrl+F5 dump, 2026-07-14):** `ui step ms: max=153.0 avg=39.84` — ONE registry
poll step ate ~40% of the game thread steady-state (with a menu open!). Root causes: all ~33 adapters'
`is_active()` polled every 100 ms; up to 3 `FindAllOf` per tick (each O(all UObjects), ~30-50 ms — the
153 ms max was a triple-scan tick); `Core.on_screen` 24-ancestor walks duplicated dozens of times per
step; ~25 absent classes rescanned every 4 s forever; three separate 20 ms pad loops = 150
`ExecuteInGameThread` dispatches/s. This one number explained sluggish free-roam, cinematic audio
stutter AND ~2 s menu-entry latency — "menus fluent, gameplay slow" was an illusion (nav gates out in
menus, masking that the registry was heavy everywhere).

**The architecture (all in ui_core.lua / ui_registry.lua / pad_poll.lua, 2026-07-14 evening):**
- **Sticky-active registry** (`ui_registry.SWEEP_EVERY=3`): with an adapter active only IT is polled;
  full priority sweep every 3rd tick, immediately when active goes inactive, every tick while nothing
  is active or a pending switch is confirming. Overlay detection worst case ~500 ms (accepted).
- **Per-tick `on_screen` memo** keyed by `GetAddress()`, cleared in `begin_scan_tick`; `Core.valid` is
  NEVER memoized.
- **Scans:** `SCANS_PER_TICK=1`, plus a time gate (step already >10 ms → no scan), per-class jittered
  backoffs (anti-cluster), `timed_findall` telemetry → `_G.__KakarotScanStats`, printed by the Ctrl+F5
  dump (`findall scans: n/total_ms/max_ms`).
- **Resurrect probe: DISABLED (`PROBE_ENABLED=false`), 2026-07-15 — do NOT re-enable by flipping the
  flag.** The idea (StaticFindObject by recorded path to re-find destroyed menus in ~300 ms) FROZE the
  game twice, adversarially confirmed: boot hang at the Bandai logo (probes during the initial async
  load), and after gating boot/transitions, a MID-SESSION freeze — **free-roam streams sublevels
  asynchronously too, so on this game there is NO provably-safe window for `StaticFindObject` from the
  game thread** (deadlock/livelock vs the loader over the engine object tables; the log just stops, no
  crash, no Lua error). A safe version needs a native-side "loader idle" check or the lookup moved into
  mem_bridge. Menu re-detection is back on the budgeted FindAllOf at DEAD_BACKOFF (~4 s) — the proven
  path. LESSON: no speculative object lookups at ANY time the engine might be async-loading — which on
  this game is any time.
- **Scan gating is a COUNT budget, never a TIME budget (2026-07-15 lesson):** a 10 ms elapsed-time gate
  in `scan_allowed` permanently silenced the tail-of-sweep menus (saveload/items/characters/tree/palette,
  sweep indices ~24-31): the walk cost alone exceeded 10 ms, so their scans were denied EVERY tick — a
  time gate starves by sweep position; a count budget drains fairly (served classes back off and leave
  the queue). `SCANS_PER_TICK=2`.
- **Backoffs run on wall-clock ticks** (`now()` = os.clock×10; MSVC clock() is wall time) — the old
  per-adapter tick counters fed a now-ignored parameter.
- **`pad_poll.lua`:** ONE shared 20 ms LoopAsync for radar_menu R3 + map-travel d-pad + status d-pad
  (150→50 dispatches/s). **Any future fast pad stepper registers there — never add a new 20 ms
  LoopAsync.** Steppers get per-stepper pcall + optional on_error cleanup.

**Rules for future code:**
- A loop that scans (cached_all/cached_live/first_on_screen) OUTSIDE the registry step MUST call
  `Core.begin_scan_tick()` at its step top (battle_monitor, quest_objective do) or the time gate
  starves it via a stale step_t0.
- Empty cached list = "never opened" — must NOT force scans (the 2026-07-14 saturation lesson).
  Dead-but-nonempty (marked by `first_on_screen` for free) is the only fast-lane state.
- `aim_watch` runs every 3rd nav tick; keep speculative per-tick reflection out of free-roam.

**THE SCREEN DIRECTORY (2026-07-15, `ui_directory.lua`) — the definitive fix.** Measured: one FindAllOf
≈ 65ms on this game (max 272ms); the absent-class rescan cycle alone was ~30% of the game thread, and
menu-entry latency (>4s on submenus) was the scan backoffs. The game already registers every top-level
screen in named pointer fields, so detection is now 2-3 guarded property reads:
- `PlayerController.MyHUD` (Engine.hpp:7011) → `AAT_GameHUD` (AT.hpp:14676): `UIFieldManager` 0x578
  (AT.hpp:32792 — one field per field/shop/map/quest/info screen), `UIBattleManager` 0x570,
  `UICommManager` 0x588, `UIPause` 0x5A8, `UICommBoardIns` 0x5D0.
- **`UMenuManager`** (GameMode component, AT.hpp:41837) — the pause SUBMENUS (the slow ones):
  `m_xItemInventoryMenu.ItemMenu/ItemPalette`, `m_xStartSkillTreeMenu.SkillTreeMenu`,
  `m_xStartSkillCustomize.SkillCustomize`, `m_xCharacterMenu.MenuUI`, `m_xStatusMenu.MenuUI`,
  `m_xSaveMenu/m_xLoadMenu.m_UIStartSaveLoad`, `m_xOptionMenu.m_UIStartOption`, `m_xPartyMenu`,
  `m_xTipsMenu`, `m_xRootMenu.m_UIStartTop`.
- GameInstance (`BP_ATGameInstance_C`): `WindowManager` 0x1FB0 (System/Game/Number window cores),
  `LoadingScreen` 0x1F88, `KeyHelp` 0x1FB8.
`Core.cached_all`/`cached_live` consult `Directory.resolve(cls)` first: nil = unmapped → scan path;
{} = manager field null → screen doesn't exist, NO scan; list = valid candidates (callers pick the
on-screen one — multi-copy classes like Tips_C map several chains). RULES: (1) a mapped class NEVER
falls back to scans — a wrong field name silently kills that reader, so every name comes verified from
AT.hpp; (2) roots are found with FindAllOf skipping `Default__` CDOs (**FindFirstOf can return the CDO**,
whose fields are all null — and `Core.first_live` can't be used because it filters `/Engine/Transient`
while MenuManager lives on the LEVEL's GameMode); (3) root lookups draw from the shared scan budget
(`Core.take_scan_slot`) and back off ~2s while absent (title screen has no GameMode/HUD); (4) everything
cleared on transition; every hop pcall'd + `Core.valid`-checked (null field = INVALID RemoteObject, not
nil). nav_tracker's minimap acquisition also routes through it.
LESSONS from the first in-game round (2026-07-15, three silent screens, all fixed): (5) **"owner
unreachable" must fall back to scans, not assert absence** — the TITLE menu opens load-game/options with
no field GameMode → no UMenuManager; resolve() only returns {} after reading a REACHABLE owner's null
field, else nil (scan path). (6) A field that resolves isn't proof it's the LIVE instance: the soul-emblem
list (`UICommManager.UIEmbListIns`) resolved yet stayed silent in game → AT_UICommunityStart UNMAPPED;
same for multi-instance pools (Info_Log_C overflow toasts). Unmap is always the safe rollback for one
screen — never add scans back for mapped ones.

**VERIFIED IN GAME 2026-07-15** (five dump-driven rounds; commit 969d30a pushed): all menus read on
entry — pause submenus (items/palette/tree/characters/status/save-load), shops, maps, community/emblems,
title load/options. Extra fixes that made it stick: title screens live on the `tt` root
(ATTitleLevelScriptActor.TitleLoad/OptionMenuComponent chains; the title load widget is the BP
`Start_Save_Load_C`, invisible to native-name FindAllOf); wrapper text nodes hold mainTxt + empty SubTxt
(prefer mainTxt when collecting detail panes — screen_list); pad-press boost (A/B/X/Y/Start edge while an
adapter is active) covers ring→submenu for scan-path screens, cooldown 300ms, one bypass per class per
window. Still open: battle HUD + R3/map/status pads on the shared scheduler unverified in battle.

**2026-07-16 — THE WATCH LANE (`Core.watch_for/watch_clear`, commit d70f7df): the primitive for
"a specific screen is being opened RIGHT NOW but its widget doesn't exist yet"** (unmappable
classes only — mapped ones need nothing). Bounded per-class re-scan: ~800ms cadence per class,
3s window, renewable; multi-class arms stagger via a fresh-arm-only `delay` (a renewal must NEVER
touch watch_next or the delay pushes forward forever and the class never scans). Two hard rules
from the emblems saga: (1) **pump watches from `begin_scan_tick`, BEFORE the adapter sweep** —
a watched class queried at the sweep tail is budget-starved on every contended tick (the time-gate
starvation lesson, budget edition), and the registry's first-active short-circuit can stop its
adapter being polled at all; (2) **arm only on REAL user-driven entry signals and cancel on
positive gameplay evidence** (minimap `free_roam` OR directory-mapped battle HUD `on_screen`) —
a wrong anchor (the soul-emblem controller, born at SAVE LOAD) scanned through free roam and
combat as felt stutters. Cost of one FindAllOf is ~65ms on this game: every watch design must be
sized against that number.
**2026-07-16 (later) — CINEMATICS-LAG PASS (pending in-game verify).** User: cutscenes + menu↔cinematic
transitions stutter. Sonnet map confirmed: with NO active adapter the full ~33-adapter sweep ran EVERY
100ms tick (camera cutscenes = the worst state), dialogue-mash pad-boosts opened scan windows all
through cutscenes, and steady-state backoff scans (~65ms) landed mid-sequencer. Seven changes:
(1) **ui_core QUIET MODE** — `Core.set_quiet(q)`/`Core.scan_quiet()`; ui_registry publishes it each tick
from the committed adapter's **`scan_quiet` flag** (set on `screen_dialogue` only — questcard/telop are
notice-pattern adapters, never stay committed). While quiet: plain backoff-expiry scans in
cached_live/cached_all DEFER (no backoff written, retry when quiet lifts); boost-window and watch-lane
scans still run. Everything a dialogue can open (skip confirm, choices) is pooled+cached → no scan needed.
(2) **ui_registry IDLE THROTTLE** — with active==nil, sweep every SWEEP_EVERY (300ms) instead of every
tick, EXCEPT inside a ~1s HOT window (`hot_until`, any fresh pad press or screen commit) → menu-open
latency unchanged; event-less popups detect ≤300ms later. (3) pad_boost skips `Core.boost_missing()`
when `active.scan_quiet`. (4) **pad_poll RELAX** — `_G.__KakarotPadRelax` (set by registry: quiet OR
Transition.active; cleared in Registry.stop) → dispatch every 5th 20ms tick (worker-side global read,
same class as the generation guard). (5) nav_tracker's two RAW FindAllOf refreshes (navi icons,
AT_Character enemies) skip while `Core.scan_quiet()` (stale list keeps serving, entries re-validated).
(6) screen_loading content() pool walk throttled to 0.3s wall clock. (7) **game INI**:
`HookAActorTick=0`, `HookBeginPlay=0` (mod registers NO hooks — header_hook is a documented dead end;
these intercepted every actor tick/spawn; needs full game restart). NOT touched: HookProcessInternal/
ProcessLocalScriptFunction (UE4SS internals/ExecuteInGameThread pump may depend on them).
**2026-07-16 (night) — MEASURED BASELINE after the cinematics pass (user Ctrl+F5, 24-min session):
`ui step ms: max=518 avg=20.00 over 14226 ticks`; `findall scans: n=3310 total_ms=225612 avg=68.2`**
— i.e. the reader still eats ~20% of the game thread steady with half-second spikes, and ~2.3
scans/s sustained. Only ~90 of those were the (logged) community watch bursts; the other ~3200 are
SILENT absent-class backoff scans (ABSENT/DEAD_BACKOFF 4s × however many unmapped-absent classes).
Fixes shipped: community `maintain_wait` now cancels when the subtitles adapter (`scan_quiet`) is
active — it used to RENEW for 30s straight through cutscenes (no minimap + no battle HUD = neither
cancel fired; the 23:44 log storm) — and `WAIT_RENEW_S` 30→15. Plus **per-class scan attribution**:
`timed_findall` fills `__KakarotScanStats.by[cls] = {n, ms}` and the Ctrl+F5 dump prints the top-12
(`scan <class> n= ms=`). RESULT (same night, second Ctrl+F5 after the fixes, ~6 min incl. a cutscene): **avg 20.0→3.72 ms,
max 518→175, scans 2.3/s→0.17/s** — single-digit avg reached; the per-class list showed only the
post-reload one-shot rebuild (n=1 each) + AT_UIQteMashAlert n=3, no steady offender. Residue = one
~70-100ms hitch per scan tick (max 175 = a 2-scan tick; SCANS_PER_TICK=2 kept — the "NOT 1" note).
Round 4 (same night, still stuttery): the per-class list named the residue — menu classes
(QTE alert, map icons, community trio, dragon-ball menu, Gametitle, choice windows) cycling
their absent backoffs through the cutscene at ~0.4 scans/s. **WHOLE-CUTSCENE QUIET shipped**:
quiet also engages when active==nil AND minimap hidden AND battle HUD (directory, pointer reads)
not live AND free roam SEEN since the last transition (`seen_free_roam`, reset in the registry's
transition branch — protects boot/title and travel→cutscene). Any fresh press lifts it for the
~1s hot window; `QUIET_EXEMPT` in ui_core keeps auto-appearing narrative classes scanning
(Xcmn_Subtitles_C, Field_Talk_Win_C, Quest_Main_Telop_C, Xcmn_Win01_C). Pending verify: cutscene
dump ≈ 0 scans; battle results + boot notices unaffected. Round 5 (same night): **the mods.txt A/B
came back — mod OFF = "súper fluidas", so the stutter IS the mod while the registry step reads only
~5%: the cost is OUTSIDE the measured step attribution. Prime suspect = SPEECH: every prism.say is
a synchronous game-thread call (speech.lua → prism_bridge → NVDA IPC/SAPI) and cinematics are the
densest speech state — one backend stall per subtitle line. Instrumented (`timed_say` →
__KakarotSpeechStats; nav loop → __KakarotNavStats; both in the Ctrl+F5 dump). Round 6 — the
in-cinematic dump ANSWERED: **speech is CHEAP (43 calls, 10ms total — NVDA hypothesis dead; no
async bridge needed) and the stutter was the COMMIT-BOOST STORM**: the subtitles adapter commits
in/out on every line↔gap flip, every commit called Core.boost_missing() (fresh generation), and
boost bypasses quiet BY DESIGN → one scan per absent class (~15) every ~2s = 572 scans/90s,
avg step 34ms. FIX: commits whose two sides are nil-or-scan_quiet (dialogue cadence) no longer
boost nor set the hot window; menu flips keep the boost. Plus `Core.peek_all` (scan-free
directory-or-cache peek) for per-tick predicates — the registry's battle_hud_live probe was
itself scanning the absent HUD class every backoff (n=24). LESSON: every quiet/gating predicate
must be scan-free, and every "event" that opens a boost window must be a USER action — a
poll-driven flip is not an event. Rounds 7-9 (2026-07-17): (a) loops all measured innocent
(battle 0.21ms / quest 0.20ms / nav 0.04ms / speech 0.5ms avg); (b) cutscene-quiet never armed
on the user's sessions — their save loads DIRECTLY into a paused cinematic, free roam never
seen → line-GAP backoff scans ran all scene (14/12s); the user's own observation "smooth for a
second after Ctrl+F5" was the diagnostic gift: the dump's census FindAllOfs serve every backoff
→ ~4s scan-free window. FIX: DIALOGUE_GRACE_S=10 — a no-adapter gap within 10s of a scan_quiet
adapter inherits quiet (cleared on transition). (c) SUBTITLES-OFF bug: NEVER pick game
singletons by "first instance found" — this game keeps TEMPLATE copies alive (ATSaveSystem _0/_1
pristine + real _4; ATSaveManager _0→template, _2→real). Resolve through the OWNERSHIP CHAIN:
gi root → UAT_GameInstance.SaveManager @0x1EA0 (AT.hpp:30078) → .SaveSystem @0x108 →
Option.EnableSubtitle, via Dir.peek("gi","SaveManager"), re-resolved per query, fail-open.
Xcmn_Subtitles_C renders vis=3 (HitTestInvisible) op=1 with pane_live=false as its NORMAL state
— pane_live is NOT a discriminator for subtitle widgets. Round 10 FINAL: session-history
heuristics for cutscene-quiet (free-roam-seen, dialogue-grace) both broke — the user's saves
load DIRECTLY into paused cinematics, and the grace keyed on the dialogue adapter that the
subtitles-off gate had just silenced. The honest predicate is the WORLD: `Dir.root_ok("mm")`
(gameplay GameMode's MenuManager root — only exists in playable worlds, never boot/title) AND
no battle HUD AND no minimap AND not hot. Battle-end falling edge opens a 3s hot window so
event-less results screens read. LESSON: gate on world/engine STATE, never on session history.
Round 11 (clean two-dump window, 27 scans/62s all attributed): (a) watch_grid now refuses FRESH
arms while Core.scan_quiet() — two spurious mid-cinematic arms were 20/27 scans; legit entry
signals always ride a press (hot window lifts quiet). (b) QUIET_EXEMPT refined: bypasses quiet
only while the pool holds NOTHING live — alive pools detect from cache, their 30s refresh waits
out the scene. Subs-ON re-read verified (log 00:44). Round 12: the quiet-only arm guard FAILED —
the PARKED community board claims the line gaps (adapter active ⇒ quiet off) and its stale
mode-10 read armed the lane anyway (19/19 scans in the next clean window). Fresh watch arms now
ALSO require **`Registry.hot()`** (~1s press/commit window; every legit entry signal is
press-driven, a cutscene has no presses). LATENT: a parked board claiming mid-cutscene could
speak into a scene — if ever reported, gate the commu claim on world state. Round 13: hot+quiet
STILL passed arms (fresh boot straight into a cinematic; load-confirm/line-advance presses open
hot windows inside it; the story level's own board widgets read as handoff) — third guard:
**gameplay recency** (`last_roam_t` via Core.free_roam in Commu.is_active; fresh arms need
<30s). And the LOADING-SCREEN regression: post-transition (mm ok, no minimap, no adapter, no
press) IS the quiet state + flushed pools ⇒ the loading adapter could never claim ⇒ recap/tips
went unread — `Loading_C` + `Xcmn_MultiLineText_C` added to QUIET_EXEMPT (nothing-live
refinement keeps them free mid-scene). LESSON: any reader that must claim DURING the quiet
state needs its detection classes exempted-when-empty. If mini-cuts persist at scans=0 → NVDA
audio ducking (per-line game-audio dips are reader-side). **SAGA CLOSED 2026-07-17: user
verified in game — loading screens (recap/tips) read again and cinematics run smoothly, no
mini-cuts. SUB_TRACE turned off; whole batch committed (checkpoint 9503cec + closure 7d84a3a)
and pushed.** Do NOT blind-tune ABSENT_BACKOFF without an
offender list (it starves event-less popups). Note: the Ctrl+F5 census prints a
pcall'd nullptr traceback from nav_tracker (~line 2732) to the log — dump-time noise, harmless.
Related: [dbz-kakarot-crash-bug](dbz-kakarot-crash-bug.md), [dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md),
[dbz-kakarot-community-board](dbz-kakarot-community-board.md).
