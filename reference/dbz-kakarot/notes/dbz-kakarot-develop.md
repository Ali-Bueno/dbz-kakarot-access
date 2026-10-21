# Bulma's I+D (development) and the training room

Player report (2026-10-18, the tester who sent the driving save): "the building screen at Bulma's
place as well as the training room screen both don't have text to speech feedback". I+D was
captured, fixed and verified live the same day. A second save from a tester (2026-10-21, Android
saga, a machine built) reached the training room and Personalización; both were fixed and
verified live that day.

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

## Personalización and Subes de nivel (verified live 2026-10-21)

Reached on a second tester save (Android saga, an Aerodeslizador already built). Both options
open the SAME host, `Shop_Develop_Custom_C` (`UAT_UIShopDevelopCustom`, AT.hpp, owner
`MenuManager.UIShopDevelopCustom`, mapped in `ui_directory`). Read by `screen_develop`.

| What | Where |
|---|---|
| Header (entry) | `Txt_Titl00` machine, `Txt_Lv`, `Txt_Titl01` "Siguiente nivel:" + `Txt_Num01`, `Txt_Memory` + `Txt_Num_Memory` "0/10" |
| Stats | `UIDevParam_List`, same rows as I+D (`Txt_List` + `Txt_Status_00_01`) |
| Subes de nivel | materials list `Xlist_List01_Lay9_00` (`UAT_UICmn01MenuList09`, a MenuListBase01: `GetSelectValue` + `ListPlateCtn`), title "Materiales", rows name + owned count |
| Personalización | chip slots `Xlist_Custom00_Lay10.Item_List` (`UAT_UIXlistCustom00Item`), label `Txt_List00`; colour `Shop_Custom_Color` (`Txt_Cap00` "Color:" + `Txt_Color`), cycled with Y then left/right, B confirms |

- **Mode = whichever sub-list is interactive.** Leaving Subes de nivel parks its list
  `HitTestInvisible` (vis 3) but still `on_screen`, and it then shadowed Personalización. The
  live list is `Visible` (0), so the switch is `pane_live` on the sub-list.
- Feeding a material opens the shared quantity window (`Xcmn_Win02_C`, `wm.NumberWindowCore`),
  now read by `screen_numberwin`: "Subes de nivel, Mineral de hierro, 2/86, 1 → 1, EXP. ganada,
  80, Siguiente nivel: 100", and each up/down speaks only the new numbers. `Relation_List` holds
  only the EXP row; the "Siguiente nivel" row is found by walking `VBox_Container`'s children
  (strict member gate, rows that carry a number). `screen_dialog` used to read only the caption
  once and let go.
- **Unverified:** the chip slot under the cursor. The save has one slot ("No se puede equipar",
  no chips owned), so the `Border_Cursor` marker could not be checked against a second row, and
  the chip list (`UIXlistParts`, "Info. de objeto") never opened.
- The money balance is still not read inside I+D.

## Training room (verified live 2026-10-21)

Not a door: the room is run by a researcher NPC, `Cpl073_Training` (`QuestCharacterBase_C` in
sublevel `AREA19_C03_EVT_MAIN_02005`), whose `ShopComponent` opens everything. The dev `tp` onto
`ATDoorVolume` left the player stuck inside the door; `tp QuestCharacterBase_C Cpl073_Training`
and B is the way in.

Every pane hangs off the FIELD manager (AT.hpp:32893-32918), so no scan:

| Screen | Class / owner | Read by |
|---|---|---|
| Top menu: Modificar / Entrenar | `Shop_Top01_C` (`UAT_UIShopTop01` < `UAT_UIShopTop`), `fm.ShopTop01`; header `Txt_List00` "Sala de entrenamiento", `Txt_Cap01` "Nivel de instalación", `Txt_Num_Lv` | `screen_shoplist` (rows like any shop top; header on entry) |
| Remodel levels | `Shop_Training00_C` (`UAT_UIShopTraining00`), `fm.ShopTraining00` (null until opened); list `Xlist_List04_Lay9`, selected `Txt_Cap00`, `Txt_Detail`, `Txt_Cond` + `Start_Quest_Bar00List` (`Txt_Progress`, `Txt_Num` "have/need") | `screen_trainroom` |
| Training battles | `Shop_Training01_C` (`UAT_UIShopTraining01`), `fm.ShopTraining01`; list `Xlist_List05_Lay7`, character tab `Txt_Cap01` (LB/RB), `Txt_Cap03`, `Txt_Recommend` + `Txt_Num_Lv`, `Txt_Task_Detail00..02`, `Txt_Name_E00..02` + `Txt_Lv_E00..02`, `Txt_Cap00` + `Xlist_Bar02_00/01`, `Txt_Cap04` + `Txt_Name_Skill`, `Txt_Detail` | `screen_trainroom` |
| Party before the battle | `Start_Party_C`, owned by the NPC's `ShopComponent.StartPartyMenu` (AT.hpp:43371), which no root reaches | `screen_party` fallback |

What is spoken:

- Top: "Sala de entrenamiento, Nivel de instalación, Nvl. 1, Modificar", then each option.
- Remodel: "Sala de entrenamiento de nvl. 2: Suelo reforzado, <description>, Necesario para
  modificar, Zeni, 403640/4500, Medalla D, 89/5, …". X ("Entrenamiento para desbloquear") and
  "not enough materials" are read by the existing popup readers.
- Battles: "Sala de entrenamiento, Goku, Adquirir Poder desenfrenado, Nvl. recomendado: 40,
  ¡Derrota a todos los enemigos!, Raditz, Nvl. 40, …, Recompensas, Megamasa de nivel 1, 3, …,
  Maestría, Poder desenfrenado, <skill description>".
- Then the party screen, "¿Entrenar con este grupo?", the battle, the result sheet
  ("EXP. ganada"), the researcher's line, and back to the battle list. Run end to end live.

Traps found on the way:

- **The party screen is the ShopComponent's**, so the mapped `AT_UIStartParty` chains answer
  "absent". `screen_party` now scans `Start_Party_C` only while `Shop_Top01_C` is on screen, and
  the hit must be `pane_live`: her top menu stays `Visible` through the whole battle, and the
  party pane is parked there `HitTestInvisible`. Without that gate the party reader held the tick
  for the entire fight (seen live).
- **`screen_shoplist`'s first-row fallback spoke the wrong option** during the open/close
  animation ("Modificar" after pressing A on "Entrenar"). The confirmed row goes to
  `STATE_ENTER` (2), which now counts as selected, and the fallback waits 8 polls.
- **`gm.NpcTalkManager` is not reflected** (the component exists, the GameMode declares no
  property for it), so "the NPC being talked to" is not a usable root.

Not covered:

- `Icon_Clear` (the "cleared" stamp on a training) is on screen for the save's one training,
  which is cleared; with no uncleared training to compare, it is not spoken.
- `fm.ShopTraining02..05` (other training layouts: two-enemy, score, skill). No census yet, so
  not mapped (no speculative class names).
- The game-over menu after losing a training battle ("Reintentar" / "Abandonar el
  entrenamiento") is read, but in one run it ignored every confirm (pad A, Enter, Start) and the
  game had to be killed. Not reproduced; watch for it.

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
- **New dev commands (2026-10-21).** `vis <Class> [member]` prints, per cached instance (or its
  member), `on_screen`, the visibility enum, opacity, `pane_live` and `pane_rendered`: the
  one-call answer to "why does this pane still claim the tick?". `tpxyz <x> <y> <z>` moves the
  player to raw coordinates, e.g. out of a door volume a `tp` left them stuck in.
- **Scripts load at process start.** Edits made while the game was booting were not in the
  loaded mod; reload once after boot before testing.
- The test save in our Steam slot is now this second tester's (Android saga). The previous
  tester's slot was backed up to the session scratchpad only; ours is restored with
  `Downloads\Kakarot save backup 2026-10-17\restaurar mi partida.bat`.
