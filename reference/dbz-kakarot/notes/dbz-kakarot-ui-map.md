# dbz-kakarot-ui-map

> DBZ Kakarot full UI map + accessibility roadmap from the CXX header dump (menus, dialogue, combat, nav)


Authoritative UI structure for the DBZ Kakarot mod, mined from the UE4SS **CXX header dump** (Ctrl+H →
`AT\Binaries\Win64\CXXHeaderDump\`; natives in `AT.hpp`, framework in `CFramework.hpp`). Full doc:
`reference/dbz-kakarot/ui-map-and-roadmap.md`. See [dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md),
[dbz-kakarot-input-icons-ref](dbz-kakarot-input-icons-ref.md), [ue4ss-screenreader-template](ue4ss-screenreader-template.md).

**Selection: no engine-wide index.** VERIFIED against the 80 MB `UE4SS_ObjectDump.txt` (2026-07-01, late
session): the dump is the authority on Lua-callable API. **`GetSelectValue()->int32` is REFLECTED/callable**
on `UAT_UIMenuListBase00/01/03` + `MenuBarBase03` (NOT Base06 — quest list is highlight-only). Rows in
`ListPlateCtn` (TArray, **1-based in UE4SS Lua**), each `UAT_UIList0NChoice` = `TxtName`+`TxtNum`. Wrapped as
`A.list_select_index`/`A.list_selected_row` in ui_archetypes. Other readable-index screens:
`UAT_UIGameover.CurrentSelectIndex` (stored int), `UAT_UIStartChar.GetCursorIndex()`, `UAT_UIXListBar01`
per-bar `GetState()`, `USimpleTalkWindow.CurrentPage`, `UAT_UIInfoLog.CurrentIdx`. Nav hub `UMenuManager` has
reflected pointers to every submenu. MATERIAL/HIGHLIGHT-ONLY (no reflected index anywhere — our highlight
heuristics ARE the intended method, KEEP): battle pause `UAT_UIXCmnPause` (+ rows `UAT_UIGameoverBar`, bare
shells — `PnlCursEff` is just a Border; selection now inferred from cursor-glow alpha), title
`UAT_UIGameTitle`, message/Yes-No `UAT_UISystemWindow`/`GameWindowChoice`/`ChoiceWin`, CompZ, map, skill tree,
field cursors, all leaf `*Choice` rows.

**Adapters verdict:** only the TUTORIAL had a better read → refactored (2026-07-01) to read rows
`Battle_Tips_List_00..08` structurally: `Txt_Btn` (glyph via markup_to_speech) + `Txt_Operat` (action) =
"button: action", header from non-row text, fallback to old scan. Title/difficulty/dialog/pause selection
= confirmed keep (material/highlight is the game's own mechanism). Options slider *could* use native
`int32 Volume` (UAT_UIBar03Choice) instead of counting 20 segments, but verify range at runtime first.

**Big future opportunities:**
- **Dialogue (reactive):** every line flows through `UCFUIXcmnMultiLineText::SetText(FText)` (localized) →
  hook it once, route by owner widget (speaker vs body). Field NPC talk: hook `AAT_GameHUD::OnMobMessage
  (Speaker, Message, bool)`. Widgets: `Xcmn_Subtitles_C`(Txt_Name/Txt_Selif), `Field_Talk_Win_C`, telops,
  `Choice_Cmd`/`Choice_Win`. Highest value for following the story.
- **Combat:** grab `UAT_UIBattleManager` once (hub to all battle widgets). Readable text: HP/Ki/level/name,
  skill slot name+cost (`Battle_Command_List00`), conditions, timer, total damage (`UAT_UITotalDamage.
  CurrentValue`), QTE prompt. NOT readable: Heat/Tension gauge (no number), digit-sprite combos/damage,
  banner textures. Hooks: OnBattleStart/End, OnChangeTarget, OnSkillBegin, OnComboAddEvent.
- **Map/nav (audio-nav):** NO numeric angle anywhere — compute yourself. Player: `UAT_UIMiniMapRadar.
  PlayerIns` → `AAT_Character:GetActorLocation()/GetActorRotation()`. POI coords: map-icon actor pointers
  `UAT_UIMiniMapIcon.TargetActor` / `UAT_MapIconSwitchBase.Target`; classify via `EMapIcon`/
  `EEncounterDirectionType`. Area: `MiniMapRadar.CurrentAreaName`. Fast-travel coords in
  `FFastTravelDestinationTableRow`.
- **Menus:** field pause `Start_Top_C → UAT_UIStartTop` (image-font ring, index via `URootMenu.
  OnSelectItem`); shop `Shop_Top_C`/`Shop_Cook_C`; inventory `Start_Item_Win00_C` (`WL_Txt_List`); quest
  `Quest_Sub_C → UAT_UIQuestSub`. `UMenuManager` is the logic hub. Prefer any `MenuListBase00/01/03/06`
  host → `GetSelectValue()`.
