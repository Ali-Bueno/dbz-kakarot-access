# D Medals in the radar

Player request 2026-11-19: "how many D medals are left in a location and how many we have
collected", and list them from far away like quest items. Coded **unplayed**. Native evidence:
`code/decompiled/dmedal_findings.c` (raw `dmedal_raw.c`, probe `code/ghidra/dmedal_probe.java`).

## Facts

| Fact | Source | Confidence |
|---|---|---|
| `DMedalTreasure_BP_C` < `ATreasureAccessPoint` (AT.hpp:18242): `SaveDataIndex` @0x4A8, `TreasureSaveName` @0x4B0, `TreasureCategory` @0x4D4 (0 = DMedal), `TreasureState` @0x4D6 | header | high |
| A collected medal STILL SPAWNS: BeginPlay (`FUN_1410cb580`) registers it in `UTreasureManager.TreasureAccessPointList` (+0x128), reads the save flag (`FUN_1410d6320`) and sets `TreasureState = 11`. It is hidden later (`OnChangePhase` → `OnEnableItem(false)`), not destroyed | Ghidra | high |
| One placement umap per area (`Maps/AreaNN/PLA/AREANN_PLA_010.umap`, DLC `PLA_01/02`), so an area's medals load together | pak scan | medium |
| Per-area totals come from `Parameter/AccessPoint/TreasureSaveTable` and `DLC3..7TreasureSaveTable` (`FTreasureAccessPointSaveTable`, AT.hpp:11932: row = save name, `SaveIndex`, `AreaIndex`). Base game: Area01 30, 02 30, 03 40, 04 35, 05 30, 06 25, 07 40, 08 20, 09 30, 10 30, 11 30, 16 20. The `LNN_` prefix is the level, not always the area (`L56_*` belong to Area58). DLC4's table is empty and DLC8 has none | pak tables | high |
| Save flags: `UATSaveManager.SaveGame` @0x100 → `UATSaveGame.Treasure` @0x55350 = `FATSaveGameTreasure` (ATExt.hpp:613), seven `bool[500]` arrays. Which array a medal uses depends on DLC-ownership predicates in `FUN_1410d6320`, not settled | Ghidra | high / open |
| The area map shows the game's own counters: `UAT_UIMapM.WL_Pnl_Medal` @0xD70 and `WL_Pnl_Chest` @0xD68 (`UAT_UIMapMCount.WL_Text_Count`), rebuilt from the world table (`FUN_1410ceb30`) | header, Ghidra | high; text format unknown |

## What the mod does

- **R3 group *Medallas D*** (`dmedals`, after *Coleccionables*): the medals left to collect, no
  distance cap. Its name carries the count of the loaded area: "Medallas D, 12 de 30 recogidas"
  (every loaded `DMedal` actor counts, taken = `TreasureState` 11).
- **Area map**: a few polls after opening, the medal and chest counters are read, queued after the
  objective ("Medallas D: 12 de 30, Cofres: 3 de 10"); any other format is spoken as shown.

## To check live

1. Area map: the counters are spoken; note whether the game's number is collected or remaining.
2. R3 → *Medallas D* lists far medals, and its total matches the area map's.
3. If the radar total is smaller than the area map's, the medals stream after all: build the
   static table from the save tables above (a generator was written and dropped on 2026-11-19
   because nothing needed it yet).
