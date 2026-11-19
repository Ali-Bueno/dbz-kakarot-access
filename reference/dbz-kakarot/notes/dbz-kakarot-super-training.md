# Super training (Whis and the other DLC trainers)

Player report 2026-11-11: "I'm playing a new power awakens dlc and the training menu from whis
doesn't have text to speech feedback". Coded from the header and Ghidra, **unplayed**: the game
was not run that day.

Native evidence: `code/decompiled/_villain_supertraining_findings.txt`, section B.

## The flow

`UAT_BlueprintFunctionLibrary::LaunchSuperTrainingMenu` (AT.hpp:29933) → `FUN_1411bb840`, which
switches on a menu type 0–4:

| Type | Opens |
|---|---|
| 0 hub | `fm.ShopTop` (shop type 11 at `ShopTop+0x468`): Super Training / Breakthrough / Cancel |
| 1 | `fm.ShopTraining03` |
| 2, 3, 4 | `fm.ShopTraining04` in mode 1 Breakthrough / 2 God of Destruction / 3 Frieza (`+0x520`) |

- The hub is an ordinary `Shop_Top_C` on the field manager, so `screen_shoplist` already reads it.
- The panes were the silent part: `Shop_Training03_C` / `04_C` were mapped nowhere.
- `fm.ShopTraining02` and `05` are written by the same loader but this flow never shows them.
- The trainer NPC carries a `USuperTrainingComponent` (AT.hpp:43616) with its OWN party,
  skill-palette and skill-tree widgets, which no manager points at.

## What the mod reads

- `ui_directory`: `Shop_Training02..05_C` → `fm.ShopTraining02..05` (pointer reads, no scan).
- `screen_trainroom`: one layout table per pane. Members are native and read by name
  (AT.hpp:35996-36132): list `Xlist_List05_Lay7`; selected name `Txt_Cap00` (03, 05) or
  `Txt_Cap03` (02, 04); `Txt_Recommend` + `Txt_Num_Lv`; enemies `Txt_Name_E0n` + `Txt_Lv_E0n`;
  goals `Txt_Task_Detail0n`; 03 adds `Txt_Title`, `Txt_Best` + `Txt_Score` and the two challenge
  lists (`Xlist_Bar06_Lay10` / `_Lay3`, rows `Txt_Challenge`, `Txt_Progress`, `Txt_Item`,
  `Txt_Num`); 04 adds `Txt_Skill00`, rewards `Xlist_Bar02_00..04` and `Txt_Detail`.
- The `st` directory root is the component. It is the one root found by a class scan, and only
  while a press lands in a gameplay world with no minimap, no battle HUD and no committed
  adapter (a menu nobody is reading), every 5 s at most. It serves the component's skill tree
  and skill palette (extra chains) and its party pane (`screen_party.shop_party`, `pane_live`).

## Unknown, to settle on the first live run

1. Whether the detail texts follow the cursor. `DetailUpdate` is a Blueprint event, so this is
   unproven; two captures a second apart answer it.
2. Which text is the title on 03 (`Txt_Cap00` vs `Txt_Title`).
3. "Cleared" is only drawn (row `PnlClear` @0x3D0 / `WL_Icon_ClearS` @0x438); not spoken.
4. Whether the minimap hides while the hub is up. `screen_shoplist` refuses in free roam, so a
   visible minimap would keep the hub silent.
5. Open flags, if a cheaper gate is ever needed: `ShopTraining04+0x5A0` (u8, 1 while open),
   `ShopTop+0x45C` non-zero with `ShopTop+0x468 == 11`.

If it is still silent, ask the tester for `UE4SS.log` and grep `screen -> screen_trainroom`,
`screen -> screen_shoplist` and `super training component found`.

## Tough training grounds (player report 2026-11-19: silent)

Ghidra (`code/decompiled/_tough_training_findings.txt`, probe `code/ghidra/tough_probe.java`):
`ATrainingPointActor::OpenMenu` (thunk `0x141910dd0` → `FUN_1411e8b70`) switches on `OpenMode`
@0x7B0. Mode 0 opens `fm.ShopTraining` (`Shop_Training_C`); **CrowdTraining and every
ClearTraining / MainStoryEx mode open `fm.ShopTraining03`** (`Shop_Training03_C`), writing a
sub-mode byte at widget+0x590. That pane is already mapped and read by `screen_trainroom`
(`super03`), never seen live. Selection: unreflected int32 at `Xlist_List05_Lay7+0x490`; the
game rewrites `Txt_Cap00` / `Txt_Num_Lv` natively on every cursor move (`FUN_14166f500`).

Why it is silent is not settled offline. `screen_trainroom` now logs
`trainroom: super03 on screen but refused (pane not live | no selected name)`. If that line
never appears, grep `screen -> ` to see which adapter holds the tick (`screen_training`, registered
first, is the suspect if a parked `Shop_Training_C` stays on screen); if nothing, take a census.
