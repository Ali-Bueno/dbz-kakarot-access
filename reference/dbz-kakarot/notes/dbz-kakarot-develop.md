# Bulma's I+D (development) and the training room

Player report (2026-10-18, the tester who sent the driving save): "the building screen at Bulma's
place as well as the training room screen both don't have text to speech feedback". I+D was
captured, fixed and verified live the same day. The training room could not be reached on the
test save; see the end.

## I+D: the flow

I+D unlocks in the story right after the driving licence (Goku asks Bulma for a car). Bulma is
`Npc004_Develop` in Capsule Corp (Area19, sublevel `AREA19_MechanicChara`).

1. Talk to Bulma, pick "Construir algo" (dialogue choice, `screen_choice`/`screen_dialog`).
2. `Shop_Top_C`: I+D / Personalización / Subes de nivel (`screen_shoplist`).
3. I+D opens `Shop_Develop_Dev_C` (`UAT_UIShopDevelop`, AT.hpp:35727): three tabs on LB/RB
   (Máquinas, Piezas de máquina, Lista de chip de mejora), a list, a stats panel and the
   required materials (`screen_develop`).
4. A asks "¿Confirmas que quieres construir …?" in the shared `Xcmn_Win01_C` (`screen_dialog`).
5. Building shows `Shop_Develop_Result_C` (`UAT_UIShopDevelopResult`) (`screen_develop`).

## Why it was silent

Every I+D widget belongs to the game's mecha-dev manager, not to the field manager:
`AATGameMode.MechaDevManager` @0x4A0 (AT.hpp:13224) → `AATMechaDevManager.MenuManager` @0x358
(AT.hpp:13362) → `UATDevelopManager` (AT.hpp:28171) with `UIShopTop` @0x78, `UIShopDevelop`
@0x80, `UIShopDevelopCustom` @0x88 and `UIShopDevelopResult` @0x90.

`ui_directory` mapped `Shop_Top_C` only to `fm.ShopTop` and `fm.CookingMenu.WL_CookingTop`.
Both owners were reachable, so the directory answered "these are all the Shop_Top_C there are"
and the I+D copy never reached `screen_shoplist`. The new `gm` root is the GameMode, taken as the
`mm` root's outer (`MenuManager` is a GameMode component), so it costs no scan. Outside Capsule
Corp `MechaDevManager` is null, which the directory reads as "absent", again without scanning.

Area19 has two `ATMechaDevManager` actors (a native one in `AREA19_ALWAYS`, a Blueprint one in
`AREA19_MechanicChara`). The GameMode's pointer is the one the menu uses: it resolved to the
on-screen `Shop_Top_C` live.

## Widget names (F7 census, 2026-10-18)

| What | Where |
|---|---|
| Tab name | `Xlist_List_00_Lay9.Txt_Title` ("Máquinas", "Piezas de máquina", "Lista de chip de mejora") |
| List rows | `Xlist_List_00_Lay9` (`UAT_UICmn00MenuList9` < `MenuListBase00`), rows `Shop_Cmn_BarNN.Txt_List` |
| Selected item | `Txt_Title00`, the detail title; it follows the cursor |
| Stats | native `UIDevParam_List`; each row `Txt_List` + `Txt_Num_Param_L` (what a part or chip adds) or `Txt_Status_00_01` (a machine's base stat). Only one of the two values is on screen per tab |
| Materials | native `UIDevMaterial_List`; each row `Txt_List` + `Txt_Num` ("have/need"; Zeni shows the cost only). Header `Txt_Titl01` + `Txt_Titl01_02` |
| Result | `Txt_Titl00` ("Resultados de I+D"), `Txt_Detail00` (markup), stats `UIDevParam_List` (`Shop_Dev_Gauge_SNN`) |

Two traps: a recipe with fewer materials leaves the last row from the previous recipe valid
but collapsed, so every row is `on_screen`-gated; and the stat rows are numbered in a different
order from the screen (`Shop_Dev_Param00` is Memoria, drawn last), so they are sorted by
`Core.slot_pos`.

## What is spoken (verified live 2026-10-18)

- Entry and tab change: tab, item, its non-zero stats, then "Materiales necesarios, Tienes /
  Necesitas" and each material with have/need.
- Cursor move: item, stats, materials.
- After a build: "Resultados de I+D, ¡Piezas posteriores completado!, Encendido, 20". On closing,
  the item is read again with the new counts.
- "Inventory full" (a part you already own) comes from `screen_dialog`.

## Not covered yet

- **Personalización and Subes de nivel** (`UAT_UIShopDevelopCustom`, Blueprint
  `Shop_Develop_Custom_C`). They open only when a machine exists, and the test save has none:
  the Aerodeslizador needs four parts, and Piezas de control lacks Rodamiento G. The owner is
  known (`MenuManager.UIShopDevelopCustom`). Map it and write the reader only after a census of
  the open screen.
- The money balance is not read inside I+D. The Zeni row shows only the cost.

## Training room: what is known

- The game data names it: `ESHOPTOP_SELECT_ID` `TRAININGROOM_IN` / `_REMODEL` / `_SIMULATOR`
  (AT_enums.hpp:7867), map icon `EMapIcon::TRAININGROOM = 25`, remodel levels in
  `FTrainingRemodelingParam` (title, detail, Zeni, D medals, items; AT.hpp:11909), simulator
  battles in `SimulationBattleParam`. `UShopComponent` carries `backToTrainingRoomDoor`,
  `GotoTrainingRoomBattleField` and `OpenSimulatorTIPs` (AT.hpp:43364).
- Capsule Corp has a sublevel `AREA19_TRAINING`. The battle field is Area33 (`AREA33_TRAINING`).
- **Not reachable on the test save (2026-10-18).** After the gravity-room story, the licence and
  I+D, `AREA19_TRAINING` was not loaded: `find AREA19_TRAINING` returned nothing and the only
  `ShopComponent` in the area was Bulma's. The tester is further on than the save they sent.
- It probably fails the same way I+D did (a `Shop_Top` owned by something the directory does not
  map), but that is a guess. Do not map it until a census of the open screen exists.

## Dev-loop lessons from this session

- **`ui_core` survives a hot reload, and its cached `ui_directory` did too.** `main.lua` loads
  `ui_core` before the reload snapshot. Its `dir_mod()` kept the module it first required, so
  directory edits only took effect after a game restart. The symptom was `dir` showing the new
  chain while `cached_all` returned the old list. `dir_mod` now follows
  `package.loaded["ui_directory"]`. Edits to `ui_core` itself still need a restart.
- **The F7 census killed the dev channel.** Its brush probe called `GetName` on a brush resource
  that had passed `Mem.alive`. The error pierced `pcall` and killed the pad loop that hosts the
  dev channel. Brush probing is now off. To recover without a restart, send Ctrl+Shift+R to
  the game window with `keybd_event` and hold it about 200 ms. `SendKeys` is too short to
  register.
- **New dev command `dir [substring]`.** It prints the directory roots, each chain hop by hop,
  and what `cached_all` really returns. It works with a menu open; the nav dump skips this part
  whenever the minimap is down.
- The world-map d-pad (STATUS item -5) worked live: the first point was read immediately.
