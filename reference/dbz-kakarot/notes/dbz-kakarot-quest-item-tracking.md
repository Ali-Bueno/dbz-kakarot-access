# Quest item tracking — guiding to the ITEM, not the quest site

**Why:** the radar follows the game's NAVI marker, which points at the quest DESTINATION. On a
collection quest that lands the player in an empty clearing; several users reported getting lost
because they did not realise the item itself has to be tracked as a separate radar target
(user report 2026-08-19).

## The clean data path does not exist — do not retry it

Measured against the CXX header dump and the live game (2026-08-19):

| What you'd want | Reality |
|---|---|
| `UQuestPhase_SearchItem`, `UQuestPhase_GetItem` | **Dump EMPTY** (AT.hpp:43065, 43149; size 0x80, zero reflected members) — native only |
| `FQuestItemParam.item_id` / `.item_num` (AT.hpp:9237) | The STRUCT is reflected, but nothing exposes a ROW |
| `UQuestManager.DataTableStore.QuestItemDataTable` | Live and reachable, but `UDataTable` reflects only `RowStruct` — `RowMap` is native |
| `FRequestQuestInfo.PhaseProgressList` (progress) | `FRequestQuestInfo` occurs ONCE in the whole dump: its own declaration. No reflected member holds one |

Instructive contrast: **`UQuestPhase_GetFish` DOES reflect `TargetFishes` / `HitFishes`
(`TMap<FName,int32>`)**. So this is a per-phase-type gap, not a blanket rule — if a future feature
needs the fishing objective, that one is reachable.

Reachable and useful anyway: `UQuestManager` is live at
`…BP_ATGameModeMain_C_0.QuestManager` (`findall QuestManager` — **no `U` prefix**), exposing
`CurrentQuestMap`, `NavigatedSubQuestId`, `FoundActorList` and `fn GetPhaseId(FName) -> FName`.

## The HUD carries both halves, measured

Host `Quest_Navi_C`, rows `Quest_Navi_M00..M02` (main) and `Quest_Navi_S00..S02` (sub). Live
capture during the Namek fruit sub-quest:

```
Quest_Navi_S00.Txt_List_00 = "3/4"
Quest_Navi_S00.Txt_List_01 = "Fruta namekuseijin"
```

Both strings come from the game's own localized message table — the SAME table the radar resolves a
collectible's name from (`Item_<n>_Name`). Matching one against the other therefore compares the
game against itself and is **language-independent**: no wording of ours takes part.

Three rules the implementation must keep:

1. **Decide which field is which BY CONTENT, never by position.** The capture had the counter in
   `Txt_List_00`, while the native member names behind those nodes (`WL_QuestDetail` /
   `WL_CollectionNum`) imply the opposite order. Whichever parses as `x/y` is the counter.
2. **Rows are COLLAPSED for ordinary single-step objectives** and only populate for collection
   phases. nil is the normal answer; every caller degrades silently to the game's own marker.
3. **A finished row (`got == need`) is not a target.** The phase flips to turn-in — observed live:
   the row vanished and the title changed to "¡Llévale las frutas al niño namekuseijin…!".

`quest_objective.Quest.item_requirement()` implements this and returns
`{ name = <localized item name>, got = n, need = m }` for the first unfinished row, main before sub.

## State: both halves built (2026-08-19), never played

- **Promotion (first half).** A collectible the active quest still asks for is moved into the
  radar's **quests** group, which has no distance cap, so it is findable from wherever the marker
  dropped the player instead of being filtered out by the tight collectible cap.
- **Auto-track election (second half).** `Nav._quest_item_target` is asked BEFORE `best_candidate`
  in the auto-scan, so the item outranks the NAVI marker. It answers nil for every non-collection
  objective, which is why the marker stays the normal answer.

### How each requirement is met, and why there is so little machinery

| Requirement | How |
|---|---|
| Prefer the item over the marker | One `or` at the election site: `Nav._quest_item_target(...) or best_candidate(...)`. The record is shaped like `best_candidate`'s, so stash / route / announce / re-adopt need no special case |
| Advance to the next as each is picked up | Nothing tracks "which one is next". The sweep sorts nearest-first and drops a taken point (`point_taken`), so the next election simply wins on the next item |
| Hand back when the count is met | `item_requirement()` already answers nil once `got == need`, so the election answers nil and the marker wins. No code of its own |
| Do not fight a hand-picked target | The election lives INSIDE the auto-scan block, which already refuses to run while `target.manual`, while `resume_pick` is pending, or while `auto_suppressed` |
| Do not drift to the other quest | The requirement now carries `kind`; the election declines when `preempt.focus` names the other group, the same rule `notify_objective_change` follows |

### The six things that are not obvious

All five came out of an adversarial review of the first draft; each one was a real defect, not a
hypothetical.

1. **The pickup edge is the HUD COUNTER, not the actor.** A collected point stays a valid UObject
   (it only flips `InteractState` to Taken), so validity cannot say it was taken. `Nav._quest_req`
   watches `got` and stamps `Nav.qreq_bump`, and the election refuses to elect from any snapshot
   older than that stamp. It ARMS a boxed rebuild rather than invalidating the snapshot outright —
   nilling `targets_snap_at` would push the next `explore_rescan` into the SYNCHRONOUS
   `Nav.targets_cached()` path, a 0.4–1.2 s sweep on the game thread.
2. **A held item is given up on EVIDENCE, never on a missing handle.** `release_world_refs` nils
   `target.actor` on every gate close — a battle, a cutscene — so a hold that demanded a live
   handle would fail exactly when it is needed: the marker would be announced in the item's place
   and the item again a tick later. Evidence means a live handle that reads Taken
   (`Nav._point_taken`, shared with the sweep's own filter).
3. **The absence of the requirement is debounced, its presence is not** — and the debounce is
   cleared by `release_world_refs`. The checklist is a widget read and widget reads flicker. The
   wait is one election interval (`SCAN_EVERY * TICK_MS`), but it is a WALL clock and the nav step
   is exactly what stops running during a cutscene, so without the clear a 20 s gate closure would
   "prove" the requirement had been absent long enough and release it on the first tick back. The
   release is logged (`nav quest item released`).
4. **A quest item ARRIVES like a hand-picked one.** It is a pickup, not a destination: the wide
   auto radius announces "you have arrived" and then goes silent 8 m short of the collectible,
   which is the original complaint reproduced one radius smaller. `ARRIVE_DIST_MANUAL` plus the
   interact prompt — except for a promoted **spawner volume**, whose actor origin can sit meters
   from the orbs, which is why the pre-promotion group is carried on the record as `homegrp`.
5. **The election asks for the shared snapshot only when it has something to ELECT.** While
   already guiding to a required item, no sweep is requested at all. After a fruitless look it
   waits for the player to travel `EXPLORE_RESCAN_DIST` *and* for `EXPLORE_RESCAN_MS` to pass —
   the same pair of conditions `explore_rescan` uses. The time floor is not redundant: inside a
   post-battle re-acquire window the election runs every tick, not every `SCAN_EVERY`.

6. **The promotion belongs in `add_target`, not at a call site — and a nameless scan poisons it.**
   `list_targets` reaches the same actor from several scans and `add_target` dedupes by ADDRESS, so
   whichever scan gets there first wins and the others silently no-op. The map-icon scan (2b) runs
   before the direct class scan (4a) and reaches every POI the game marks — which is most gathering
   points — so a promotion written into 4a could never fire for the very items it was built for.
   Worse, 2b passed `name = nil`, so those points were permanently ANONYMOUS: the promotion matches
   on the localized name, so it had nothing to match against. Both halves are fixed — the promotion
   moved into the funnel, and `add_icon` now resolves the item name (STRICT: the icon's owner can
   be any class) — and the side effect is that map-iconed gathering points finally speak their real
   item name in the radar and the R3 picker instead of a generic noun.

   The general lesson: **before writing a rule that reclassifies an item, find out which scan
   actually produces it.** A dedupe by address makes a mis-placed rule fail silently, with no error
   and no log line.

### Known limits

- A gate closure longer than `WORLD_DROP_TICKS` (5 s — any real battle or cutscene) drops the
  target outright, so the radar re-acquires afterwards: the marker is announced first and the item
  a beat later. That is the existing behaviour for every auto target, not something this feature
  added, and rescuing it would mean stashing quest-item targets the way `resume_pick` stashes
  manual ones.
- `Nav.SW.lists` (the raw `FindAllOf` results of an in-flight boxed build) is NOT dropped by
  `release_world_refs`, so a build interrupted by a battle resumes over pre-gate actor handles.
  Pre-existing — the R3 picker already arms boxed builds — but this feature makes an in-flight
  build much more common, so it is now in the backlog.

### THE OPEN QUESTION — settle this FIRST, in game

**Which actor class do real quest collection items use, and how does that class express "taken"?**
Everything below depends on it and none of it can be answered from the headers:

- The promotion matches on the item's localized NAME, which comes from
  `ItemTableComponent -> FieldItemDropData`. Only access-point classes carry that component. The
  fruit/small-fish **spawner volumes** (`ASpawnerFruitVolume`, `ASmallFishSpawnVolume`) do not —
  they are not `AAccessPointBase` at all — so if the Namek quest fruit is one of those, it can
  never be named and therefore never promoted, and the feature does nothing for it. A different
  name source would be needed.
- "Advance to the next one" leans on the point disappearing from the rebuilt sweep. Access points
  flip `InteractState` to Taken (11) and are filtered; a class with no taken state that also stays
  visible would be re-elected forever and the count would never advance. `FieldActionPointActor`
  is exactly that shape.

**How to settle it:** stand next to the quest item with the game running and ask the MCP server —
`kak_dev navdump` names the class, group and resolved name of every target in range. One capture
answers both halves. Do that before interpreting anything else in this feature as broken; the log
line `nav quest item promoted: <name> (was <grp>, src=<src>)` says whether the promotion fired at
all, and its absence is the first thing to check.

### What to listen for in game

- Accept a collection sub-quest, then walk toward the area. The radar should switch from the quest
  marker to the item and announce it with its count, e.g. *"Fruta namekuseijin (0/4)"*.
- Pick one up: within a couple of seconds it should retarget the next one, with a fresh count.
- Meet the count: it should go back to the marker (the turn-in) without any extra prompting.
- Nothing should announce twice for one event. If it does, the log line to look for is
  `nav quest item released`.
- **Optional refinement, still not built:** hook `UAT_UIQuestNavigation::SetSubQuestCollectionNum` /
  `SetMainQuestCollectionNum` for an instant pickup edge instead of noticing the counter on the
  next poll.

## 2026-11-09: every drop, every row, and hunted animals (verified live in part)

Player report: Goku's sub-story *Accidente Animorfalino* (`Sub_Cpl001_01`, Cell saga, the
antidote quest) had "no markers and the items are not in the tracker". The HUD row was
`17/3 Hierba de alta calidad, 0/4 Caracol de río carnoso, 0/1 Asta de gacela dorada`. Tester save,
measured with the new dev commands `items <text>`, `drops <slug>`, `spawners`.

**Why it found nothing, three separate causes:**

1. **The game sets no marker.** `QuestManager.NavigatedSubQuestId = None`, and the R3 picker had no
   *Misiones* group at all. A collection phase (`QuestPhase_GetItem`) shows nothing on the map, so
   the item promotion is the only guidance there is. Pressing the quest menu's "A: Navegación"
   did not change `NavigatedSubQuestId`.
2. **The name table held only the earliest drop.** `Lost_Snail_01` (the river pickups,
   `LostPropertyItem_R_C`) mapped to item 36015 "Camarón de río", its episode-1 drop, and never
   matched the snail (36019) it gives in the Cell saga. Fix: the generator now also writes
   `item_drop_alts.lua` (660 slugs, every other id any slot/episode/table can drop), and
   `Nav._drop_item_name(a, strict, req)` returns the WANTED alternate when there is one.
3. **Only the first open row was ever wanted.** `item_requirement()` now carries `all` (every
   unfinished row of the same group). Promotion, hold and election match any row through
   `Nav._req_row`, and the label uses that row's own counts. The pickup edge compares the counts of
   all rows (`sig` in `Nav._quest_req`).

**Verified live 2026-11-09:** after the fix the radar auto-tracked *"Rastreando Caracol de río
carnoso (0/4)"* at 9 m, 511 m and 2974 m (log: `nav quest item promoted: Caracol de río carnoso`).
Pickups read `bHidden = true` while the pause menu is open and `false` once it closes, at any
distance tested; that is not a distance cull.

**The antler (coded, not played):**

- Only gazelles whose drop slug can yield 53017 give it: `DLC03_Area02_Animal_GoldGazelle_02`
  (primary), `..._02_Fixed` and `DLC03_Area06_Animal_GoldDeer_01` (alternates). The base
  `Animal_GoldGazelle_Fixed` drops meat (31009) only.
- Guides: a golden gazelle appears at random inside an ordinary gazelle herd (East Ravine,
  Central Plains, Continent Center, Blake Bog...).
- Wild animals carry their drop on `AnimalMob_Pawn_C.FieldItemTable` (a `FieldItemTableComponent`,
  live reflection), not `ItemTableComponent`. Section 5b of the sweep now names an animal by a
  WANTED drop only, and the promotion gate accepts `hunt`. A promoted animal arrives with the wide
  radius and the plain "arrived" line, never "you can pick it up".
- Animals exist only while the player is inside their spawn volume (`AAnimalSpawnerVolume*`,
  `AnimalSpawnData[].ClassToSpawn`). `spawners` lists the loaded ones; only four bird volumes were
  loaded in Area09, so the volume cannot be targeted from afar yet.
- Rare animals now read "<species> (ejemplar raro)" in *Caza*, from
  `UAnimalComponentBase.RareFlag`, so the golden one can be picked out of a herd.

**Still to hear:** walk into a gazelle herd with the quest open. A golden gazelle must appear in
*Caza* as rare. If its slug can drop the antler, it must also move to *Misiones* as *"Asta de gacela
dorada (0/1)"*. If it never moves, its slug is one of the meat-only ones and the antler comes from
quest logic instead (look for `QuestPhase_Hunting`).
