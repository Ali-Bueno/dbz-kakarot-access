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

## State: half done

- **DONE (2026-08-19):** a collectible the active quest still asks for is promoted into the
  radar's **quests** group, which has no distance cap — so it is findable from wherever the marker
  dropped the player, instead of being filtered out by the tight collectible cap.
- **NEXT:** make the AUTO-TRACK prefer that item over the NAVI marker, advance to the next one as
  each is collected, and hand back to the marker when the count is met.

  Deliberately left for its own pass: the acquire/preempt/resume path in `nav_tracker.lua` has a
  documented history of subtle bugs (fresh-objective preempt vs manual pick vs post-battle
  re-acquire), and it is not something to graft onto the end of a long session. Entry points to
  read first: the fresh-objective auto-track block (`nav_tracker.lua` ~line 256) and the
  re-acquire logic (~1705). An optional refinement once it works: hook
  `UAT_UIQuestNavigation::SetSubQuestCollectionNum` / `SetMainQuestCollectionNum` for an instant
  pickup edge instead of polling the text.
