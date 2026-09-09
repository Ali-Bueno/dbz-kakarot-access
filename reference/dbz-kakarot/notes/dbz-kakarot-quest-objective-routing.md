# Quest objective routing — every objective kind, not just "collect N items"

*Investigation, 2026-09-08. Live-verified the same day (section below); the resolver is BUILT (see "What was built" at the end) but never played. Sources: `CXXHeaderDump\AT.hpp` /
`AT_enums.hpp` and `UE4SS_ObjectDump.txt` (header sweep delegated; every offset below names its
line), plus the mod's own `quest_objective.lua` / `nav_tracker.lua`.*

## The question

Quest item tracking ([quest-item-tracking](dbz-kakarot-quest-item-tracking.md)) guides to the
required ITEM instead of the quest site, but it only fires when the HUD row parses as `name + x/y`.
Every other objective kind — fish here, talk to X, defeat these, reach that area, deliver — falls
back to the game's own navi marker. The user's ask: when the quest says *fish*, point at the fishing
spot; in general, route the radar to whatever the current objective actually needs.

## What the game has (all reflected unless flagged)

### The objective taxonomy is an enum, and each kind is its own phase class

- `EQUEST_PHASE_TYPE` — `AT_enums.hpp:7521`, 53 values. The ones a player acts on:
  `ARRIVAL=1, BATTLE=3, BREAK_ENEMY=5, SEARCH_ITEM=6, GET_ITEM=7, TALK_NPC=8,
  GET_ITEM_TALK_NPC=9 (deliver), COOKING=10, HUNTING=11, COMMUNITY=23, DOOR=26, MINIGAME=30,
  GET_FISH=31, WISH_DRAGONBALL=32, CAMPING=33, DESTROY_ENEMIES_BASE=38, ASSAULT_BATTLE=42`.
  The rest are flow control (demo, fade, loading, reward...).
- The live objective is a `UQuestPhaseBase` subclass instance, one class per kind
  (`AT.hpp:42942` base, `CurrentQuestId @0x30`). **The class name IS the kind** — no enum read
  needed. Per-kind target members:

| Phase class | Target members (reflected) | Source |
|---|---|---|
| `UQuestPhase_GetFish` | `TargetFishes @0x80 : TMap<FName,int32>` (species → count), `TargetAreaName @0xD0 : FName`, `HitFishes @0xD8` | `AT.hpp:43057` |
| `UQuestPhase_TalkNpc` | `TargetActorsId @0x78 : TMap<uint32,FName>` (find-list ids), `FinishedActorsId @0xC8`, `TargetPlayerType @0x12C` | `AT.hpp:43191` |
| `UQuestPhase_GetItemTalkNpc` | same shape: `TargetActorsId @0x80`, `FinishedActorsId @0xD0` | `AT.hpp:43069` |
| `UQuestPhase_Door` | `TargetDoorName @0x70 : FName` | `AT.hpp:43039` |
| `UQuestPhase_Minigame` | `MinigameName @0x68`, `TargetActorName @0x70` | `AT.hpp:43105` |
| `UQuestPhase_Camping` | `BonfireActor @0x70 : AFieldActionBonfireActor*` (the only direct actor pointer) | `AT.hpp:42984` |
| `UQuestPhase_Hunting` | only `bCompleted`; targets come from `FQuestHuntingCondition` | `AT.hpp:43084` |
| `UQuestPhase_Arrival`, `_GetItem`, `_SearchItem` | **empty dumps** — native-only | `AT.hpp:42951/43065/43149` |

- Condition rows (per-quest data tables, `FQuestDataTableStore` `AT.hpp:8990`):
  `FFishingPhaseCondition` (`AT.hpp:6210`: `TargetFishes`, `TargetAreaName @0x58`);
  `FTalkPhaseCondition` (`AT.hpp:11519`: `FindActorsName @0x18 : TArray<FName>`);
  `FQuestDefeatEnemiesSet` (`AT.hpp:9078`: `TargetType @0x10 : CHARACTER_TYPE`, `Count @0x14`);
  `FQuestHuntingCondition` (`AT.hpp:9180`: `ConditionType : EHuntingPhaseConditionType
  {Animal, Dinosaurs, Foods}`, `ContainsName : FString`, `Count`);
  `FArrivalPhaseCondition` (`AT.hpp:2118`: `UniqueId` only — the volume is found by id).
  `FQuestPhaseParam` (`AT.hpp:9381`) is the objective row: `phase_type @0x10`,
  `object_name @0x60`, `target_id @0x68`. Rows live in per-quest assets
  (`.../Parameter/Quest/PhaseParam/QuestPhaseParam_*.uasset`), so only the streamed quest's rows
  exist at any time.

### Name → actor: the game resolves it for us

- `UQuestActorFindListComponent.m_id @0x100 : FName` (`AT.hpp:42728`) tags a world actor with a
  find-id. `AFishingArea.QuestActorFindListComponent @0x3C8` (`AT.hpp:15577`) — this is how a
  fishing objective names its spot.
- `QuestManager:GetActorByFindListId(FName) → AActor*` (`ObjectDump:78510`) and
  `GetActorByType(CHARACTER_TYPE) → AActor*` (`:78513`) are **reflected UFunctions** returning the
  objective's actor directly. `QuestManager.FoundActorList @0xD98 : TMap<FName, AActor*>`
  (`:78632`) holds what has already been resolved.
- `QuestManager` is live at `BP_ATGameModeMain_C_0.QuestManager` (no `U` prefix);
  `AT_BlueprintFunctionLibrary:GetQuestManager` (`ObjectDump:54598`) is the static entry.
  `NavigatedSubQuestId @0x198`, `CurrentQuestMap @0x138 : TMap<FName, AQuestBase*>`.
  `AQuest_General.QuestPhaseMap @0x388 : TMap<FName, UQuestPhaseBase*>`,
  `GetPhaseId()` / `GetPhaseIdImmediate()` (`ObjectDump:78272/78274`),
  `GetQuestPhaseParamObjectName(FName phase_id) → FName` (`:78276`, returns `object_name`).
- **Change event:** `UQuestManager::OnSubQuestNavigationUpdate(FName PhaseID, EQUEST_PHASE_TYPE
  Type)` (`AT.hpp:42890`) fires exactly when the objective changes and carries the kind. A
  reflected UFunction — hookable with `RegisterHook`, on the game thread (not a construction
  notify). Polling `GetPhaseId()` on the navigated quest is the no-hook alternative.

### Where the player must stand (finer than "the actor")

- Fishing: `AFishingArea.FishingStartPoint @0x3E8 : AFishingStartPoint*` (`AT.hpp:15577`) — the
  spot where fishing begins, not the centre of the water volume. Neither class carries an
  `ATMapIconComponent`; the `EMapIcon::FISHING (5)` icon comes from BP (`FishingArea_BP.hpp`), so
  the spot only appears in `MapIconList` at runtime.
- Talk / deliver: `UNpcTalkComponent.TalkTargetPointPlayer @0x148 : AActor*` (`AT.hpp:~42233`) —
  where the player stands for the conversation. `AQuestCharacter.NpcTalkComponent @0x9D0`.
- Enemies: `FQuestDefeatEnemiesSet.TargetType` is a `CHARACTER_TYPE`; the radar already names
  enemies from that enum ([npc-names](dbz-kakarot-npc-names.md)), so "nearest enemy whose type is
  in the set" is a filter over the existing `enemies` list. Bases carry
  `EMapIcon::ENEMIES_BASE (32)`.
- Hunting: `EHuntingPhaseConditionType` picks the family, `ContainsName` filters by name; the
  world targets are the existing `hunt` group (`EMapIcon::HUNTING (7)`).

### Why the game's own marker is not always enough

The mod already follows `UAT_UIMiniMapNaviIcon.TargetActor` for every quest kind
(`nav_tracker.lua:456-528`), so a non-collection quest is never *untracked*. The gap is precision:
`MAINQUEST_RANGE_ONLY (71)` / `SUBQUEST_RANGE_ONLY (72)` markers guide to a **range circle** —
its target's location is the circle's centre, and for a fishing or hunting objective that centre
is not the spot. That is the shape the user reports as "the radar does not take me to the fishing
spot". The RANGE_ONLY icon plus a phase class that names a precise actor is the trigger to override.

## No "current objective location" function exists

Swept `AT.hpp` / `ATExt.hpp` for `Get*TargetLocation|TargetPos|NaviPos|GuideTarget|
CurrentObjective`: only look-at/IK code. Marker placement is native, inside the minimap tick. The
resolvers above (find-id → actor, type → actor) are the reflected surface; there is no shortcut.

## Proposed design (not built)

One resolver, keyed on the live phase class, returning `{actor, stand_actor?, label, count?}`;
`Nav._quest_item_target` becomes one branch of it.

1. **Find the live phase.** Poll `QuestManager.NavigatedSubQuestId` (sub) / the MAIN entry of
   `CurrentQuestMap` → `AQuest_General:GetPhaseId()` → `QuestPhaseMap[phase]`. Cheap reflected
   reads; re-resolve only when the phase id changes (or hook `OnSubQuestNavigationUpdate`).
   **Open question:** whether UE4SS Lua reads these `TMap`s (the mod has never read one through
   reflection — `native_offsets.lua:325` reads one natively). Verify with `probe` first. Fallback
   that avoids TMaps entirely: `FindAllOf("QuestPhase_<Kind>")` once per phase change and match
   `CurrentQuestId` — a scan, but gated on an objective change, not per tick.
2. **Per class:**
   - `GetFish` → `TargetAreaName` → `GetActorByFindListId` → `AFishingArea` → guide to
     `FishingStartPoint`; label the species from `TargetFishes` with `HitFishes` as the counter
     ("Fish X, 1/3"). Species name from `FFishDataTable.FishName` (`AT.hpp:6086`).
   - `TalkNpc` / `GetItemTalkNpc` → first id in `TargetActorsId` not in `FinishedActorsId` →
     `GetActorByFindListId` → guide to `NpcTalkComponent.TalkTargetPointPlayer` if present, else
     the actor; label from the existing NPC-name path.
   - `Camping` → `BonfireActor`. `Door` / `Minigame` → name → find-list → actor.
   - `Hunting` / `Battle*` / `DestroyEnemiesBase` → filter the existing `hunt` / `enemies` /
     bases lists by `CHARACTER_TYPE` or `ContainsName`; nearest wins. The condition rows are
     tables, so the first cut can use the HUD text (already parsed for counters) and the
     `EMapIcon` group, and only reach the tables if that proves ambiguous.
   - `SearchItem` / `GetItem` → today's HUD-counter path, unchanged.
   - `Arrival`, everything else → the game's navi marker, as today.
3. **Override rule:** use the resolved target when the navi icon is RANGE_ONLY or points at
   nothing precise; otherwise keep the game's marker (it is right for arrival and story beats).
4. Every new class name joins the ABSENT scan set (CLAUDE.md §8 tax) — gate all of it on
   `Core.free_roam` and on a phase-id change, never per tick.

## Evidence already on disk (swept 2026-09-08, no game running)

- Navi markers recorded in past dumps (`Scripts/dumps/dump_radar.txt`, `dump_nav_targets.txt`)
  pointed at `AutoMoveTrigger_C` (x2), `QuestCharacterBase_C` (x2) and `TriggerStaticActor_BP_C`
  (x1) — i.e. trigger volumes and NPCs, never a fishing or hunting actor. Icon numbers were not
  logged for navi entries, so RANGE_ONLY (71/72) is unconfirmed either way.
- `FishingArea_BP_C` shows up in the radar sweep (three instances, 1.3–2.9 km away) and
  `FishingStartPoint_BP_C` is a live actor class in `dump_nav_targets.txt` — the fishing branch can
  be written against real class names without waiting for a fishing quest.
- No `quest item promoted/released` line exists in any saved log yet: the collection branch is
  still unobserved in play.

## Verified LIVE (2026-09-08, Namek, main quest "go to Vegeta's hideout", via `questdump`)

- **TMaps are OFF THE TABLE in Lua, settled from the source, not by trying.** RE-UE4SS 3.0.1
  registers `FMapProperty` only as a type table (`LuaMod.cpp:797`); there is no value pusher, so
  a member read hits the "no registered handler" throw that pierces pcall. `CurrentQuestMap`,
  `QuestPhaseMap`, `FoundActorList`, `TargetActorsId`, `TargetFishes` are never to be fetched.
- **`Quest_General:GetPhaseId()` / `GetPhaseIdImmediate()` WORK** (no-arg reflected call, FName
  return): `C02_EVT_MAIN_04002_130` for the live main quest; `None` on `BP_PdmCheck_C` (a dummy
  `Quest_General` in `CreatedQuestList` with no `quest_id` of its own — `quest_id` is declared on
  each `C02_*_C` class, so read it STRICT).
- **The live phase is a transient object named after its class**: `/Engine/Transient.
  QuestPhase_Arrival_0` and `_1`, both `CurrentQuestId = C02_EVT_MAIN_04002` (two instances for
  one quest — pooled or the previous phase; match on `CurrentQuestId`, take the one the game
  navigates). `FindAllOf("QuestPhase_<Kind>")` on a class with no instance returns empty, no error.
- **`QuestActorFindListComponent.m_id` is the objective-target registry, in the clear** (60
  entries on Namek). Ids are speaking names: `FishingArea_BP_01_0 … _05` → the five
  `FishingArea_BP_C` actors with world positions; `C02_04002_040_Arrival` / `_050_` / `_060_` →
  `TriggerStaticActor_BP_C`; `C02_04002_AutoMoveTrigger_010`; `C02_04002_Cpl006` etc. →
  `QuestCharacterBase_C` (quest NPCs, with positions); `Itm071_0N` → item `SkeletalMeshActor`s;
  `Area11_Door` → `BP_Door_C`; `TrainingPoint_BP2`. `GetOwner()` on the component gives the actor,
  `K2_GetActorLocation()` the position. **No FName marshalling and no `GetActorByFindListId` call
  needed**: name → actor is a scan of this component class, rebuilt on world change.
- Phase ids and find-ids share the `<quest>_<phaseNo>_<what>` convention, so a routing rule for
  the empty native phases (Arrival/GetItem/SearchItem) is available without their members: pick
  the find-id whose prefix is the quest id and whose phase number matches `GetPhaseId()`'s suffix.
  (Observed, not yet used: the game's own navi marker already does this for arrival.)
- **Dev-channel hazard fixed on the way**: a probe that wraps itself in `ExecuteInGameThread`
  freezes the game when driven from the channel (nested queue push during the drain — see
  [crash-bug](dbz-kakarot-crash-bug.md), 2026-09-08 entry). `dev_channel.dispatch` now shadows
  the global; `Core.prop_warm` exists so a probe's strict reads get their property sets.

## Verification plan (do this before building step 2)

- With a **fishing sub-quest** active: `kak_dev navdump` + read the navi icon's `TargetActor`
  class and `EMapIcon`. Expected: a RANGE_ONLY icon on a volume/target point, not the
  `AFishingStartPoint`. That single capture confirms the gap.
- `probe` the navigated quest: `QuestManager.NavigatedSubQuestId`, `GetPhaseId`, and whether
  `QuestPhaseMap` / `TargetActorsId` read as maps in Lua (settles the TMap question).
- `GetActorByFindListId(TargetAreaName)` on that quest: confirm it returns the `AFishingArea`.

## Dead ends already known

- `QuestCharacterFindList` ≠ the radar's scanned set (`nav_tracker.lua:3427-3435`, one extra
  entry, cause unknown) — do not swap it in as a sweep source.
- `FFastTravelMapMainQuestIconTableRow.MapPosX/Y` (`AT.hpp:5869`) is 2D map space, not world
  coordinates — region only.
- `FQuestProgress` (`ATExt.hpp:1442`) is the save-game view, not the live objective.

## What was built (2026-09-08, source only — never played)

Two modules — `mod/KakarotAccess/Scripts/quest_phase.lua` (the engine reads: which quest is
navigated, which phase object is live, what actor it names, and whether a strict nil was
trustworthy) and `mod/KakarotAccess/Scripts/quest_route.lua` (the find-id index, the caches,
the deferral/memo/hold state machine and the nav-facing API) — plus ~30 lines in
`nav_tracker.lua` and 2 new i18n keys. `nav_tracker` requires only `quest_route`.

- **Pipeline.** `QuestManager` (one `FindAllOf` per world) → `NavigatedSubQuestId` picks sub vs
  main → `CreatedQuestList` walked with `GetPhaseId()` → `QuestPhase_<Kind>` scanned for an
  instance whose `CurrentQuestId` matches → its named target → a `QuestActorFindListComponent`
  `m_id -> GetOwner()` index built once per world. `GetFish` then hops to
  `AFishingArea.FishingStartPoint`, falling back to the area actor.
- **Kinds wired:** GetFish, Door, Minigame, Camping. Arrival / GetItem / SearchItem / TalkNpc are
  deliberately untouched (marker or the existing item path).
- **When it resolves:** only on `Nav.notify_objective_change` (the quest HUD's own settled
  signal) or when a ≥2 s poll of `GetPhaseId()` sees the phase move with no text change. At most
  ONE `FindAllOf` per call, so a cold resolve spreads over a few elections (~1.5 s each). Every
  scan is gated on `Core.free_roam`; everything is dropped by `Nav.release_world_refs`.
- **Election:** `Nav._quest_item_target() or Nav._quest_route_target() or best_candidate()`. The
  routed record is shaped like any other: `grp = "quests"`, `pri` from the quest side, `label`
  from i18n (`cat_fishing` / `cat_door` / `cat_minigame` / `cat_bonfire`), plus a `qroute` mark.
- **A STRICT nil is two answers, and this resolver must not confuse them.** `Core.member(o, n,
  true)` returns nil both for "the class does not declare it" and for "the property set was not
  enumerable this tick" (one set per tick, shared by every adapter, flushed on every map
  transition — and the first live `questdump` runs read nil for *every* name used here). Every
  verdict is permanent for the objective, so believing the second would skip a phase class for
  good, cache an EMPTY find-list index, or — the reported symptom itself — commit the water
  volume because `FishingStartPoint` was merely unaskable. So every negative conclusion is
  preceded by `Core.prop_ready`, and an unready set DEFERS like a spent scan budget, bounded by
  `SET_TRIES = 6` calls (~9 s) so a permanently un-gateable class cannot pin the resolver.
  (`Core.prop_warm` is dev-only and is never called from the loop.)
- **Negative (and positive) memo per signature.** The verdict is keyed on
  `"<quest id>|<phase id>"`. `Nav.notify_objective_change` fires on every objective-TEXT change,
  and a collection counter ticking ("2/5") is a text change with the SAME phase id — without the
  memo a fruit quest re-ran the whole four-class sweep, four full-object scans, on every pickup.
  Both paths that can falsify a verdict clear `sig`: `release()` and the routed-actor-died branch.
- **The stale-route hold is bounded.** A deferred call answers with the previous objective's route
  — that is what stops the beacon flip-flopping between the marker and the routed spot while a
  resolve spreads over its calls — but only for `HOLD_S = 3 s` (~2 nav elections) from the moment
  the re-resolve was armed. Past that the caller gets nil and the game's own marker owns the tick,
  because a hold that long is the WRONG objective's spot being announced as the current one.
- **The free-roam gate is above the phase poll too**, not just above the scans: the poll walks
  `CreatedQuestList` and calls `GetPhaseId()` on each entry, i.e. reflected work on quest actors,
  which is exactly what a streaming teardown is freeing.
- **Duplicate find-ids are real** (`C02_04002_Cpl006` tags two actors live). The first valid owner
  wins — the alternative is a target that changes with `FindAllOf` ordering between resolves — and
  one deduped log line names the id, so a route to the wrong twin is diagnosable.
- **Two heuristics, flagged as such.** (1) With no sub quest navigated, the main quest is the
  created quest whose id contains `_MAIN_` (observed: `C02_EVT_MAIN_04002`), else the first with
  a live phase — a wrong pick yields NO route rather than a wrong one, because the phase match
  is by `CurrentQuestId`. The reported SIDE is derived from that same token rather than from
  "NavigatedSubQuestId is None": a side story can be the only quest with a live phase, and
  calling it main would hand the election a `PRI_MAIN` record a sub-quest focus then declines. (2) Two phase instances can exist for one quest; the one that actually
  NAMES a target wins, and a bare match is only the fallback.
- **Sweep order:** the kind that answered LAST is tried first. That integer is the one thing
  that survives `release()`, because after a battle or a cutscene the objective is normally
  unchanged and restarting the round-robin costs three extra elections — during which the
  marker wins and the radar announces the range circle before correcting itself.
- **Bounded miss handling:** a find-id the index cannot answer rebuilds the index at most 3 times
  (5 s apart) and is then written off until the objective changes — a permanent miss must never
  become a permanent world scan.
- **Offline coverage:** `tools/tests/test_quest_route.lua` (kind dispatch, scan budget, index
  build/miss/backoff, FishingStartPoint + fallback, main/sub, phase poll, free-roam gate, world
  change, dead routed actor, the memo, the bounded hold, duplicate ids, and deferral on unready
  property sets — its `ui_core` double honours `strict` and can make `prop_ready` false).
- **Not covered, on purpose:** the routed target uses the WIDE auto arrival radius (`ARRIVE_DIST`,
  8 m) like any quests-group target, so "objective reached" fires 8 m out. If the fishing spot
  needs the tight radius, add `target.qroute` to the `arrive_r` condition in `nav_tracker.lua`.
