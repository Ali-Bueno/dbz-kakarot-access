# DBZ Kakarot — UI map & accessibility roadmap

Authoritative structure of the game's UI, from the **UE4SS CXX header dump** (`Ctrl+H` →
`AT\Binaries\Win64\CXXHeaderDump\`, 1839 `.hpp`; the natives are in `AT.hpp`, the framework in
`CFramework.hpp`). Every menu is a `UUserWidget` whose readable state lives on its **native parent**
(`UAT_UI*` / `UCFUIUserWidget` / `UATUIUserWidget`); the `_C` blueprint only adds visual images/anims.

Use this to decide, per screen, the robust read strategy — instead of guessing at runtime. See also
[input-icons-and-keyconfig.md](input-icons-and-keyconfig.md) and
[ui-and-text-architecture.md](ui-and-text-architecture.md).

> **Relationship to [the generic UI strategy](../ui-accessibility/generic-strategy.md):** this doc is
> Kakarot's **Step 0 output** — the shared layers found by reading the game's code (dumps): the
> selection signal (`GetSelectValue()` on `MenuListBase00/01/03` — one read inherited by every screen
> embedding those lists, encapsulated in `ui_archetypes.lua`), the navigation hub (`UMenuManager`),
> and the text chokepoint (`UCFUIXcmnMultiLineText`). The per-screen `screen_*.lua` adapters exist
> only where §1 proves there is **no readable shared state** (family B: material/highlight-only
> selection) — they are the strategy's legitimate quirk overrides, not screen-by-screen duplication.

---

## 1. Universal patterns

### Text nodes
All text is `Xcmn_MultiLineText_C` → native `UCFUIXcmnMultiLineText` (`CFramework.hpp:1038`), with two
renderers: **`mainTxt`** (plain, `UCFUIMultiLineTextBox`) and **`ExMainTxt`** (rich,
`UCFExtendRichTextBlock` — holds inline `<inputicon KeyConfigId="…"/>` glyph markup). Read plain via
`node.mainTxt.Text:ToString()`; read a button glyph via `node.ExMainTxt.Text` → `A.markup_to_speech`.
Top-level `SetText(FText)`/`GetText()` and a cached `CFMText` (FText) exist on the parent.

### Button glyphs
Corroborates the existing input-icon doc: `<inputicon KeyConfigId=…>` resolved through
`UCFExtendRichTextIconData.KeyConfigList : FCFKeyConfigAssign{ConfigName, IconName, DynamicAssignInputId,
DynamicAssignInputControllerId}` — exactly the 4 fields our resolver uses. Message IDs resolve through
`UCFMessageManager.GetMessageDataFromName` → `FCFMessageTableRow{VoiceGroupID, VoiceID, speakerID,
Message}` where `Message` carries the markup. Simplest path: read the already-rendered widget's
`ExMainTxt.Text` (what we do), don't re-resolve message IDs.

### Selection — there is NO engine-wide "selected index"
`UCFUIUserWidget`/`UATUIUserWidget` have **no shared cursor API**; each menu rolls its own. Two families:

**A) READABLE selection (has a real index/enum/getter — read it directly):**
| Class | How |
|---|---|
| `UAT_UIGameover` | `int32 CurrentSelectIndex` (property) |
| `UAT_UIMenuListBase00/01/03` (+ `…Bar03`) and all subclasses | `int32 GetSelectValue()` (fn); rows in `ListPlateCtn`, labels `ListPlateCtn[i].TxtName/Txt_List` |
| `UAT_UIStartChar` | `GetCursorIndex()` / `GetViewIndex()` |
| `UAT_UIMiniGameTop` | `GetSelectItem()` (enum) |
| `UAT_UIStartOption` | via its child list `UICmn03MenuBar10` (→ `MenuListBase03`) `GetSelectValue()` |
| `UAT_UIXListBar01` (per-bar) | `GetState()==STATE_SELECT` / `!GetNotSelect()`; `CursorWidget`, `Focus/LoseFocus(idx)` |

**B) MATERIAL/HOVER-ONLY (no readable index — must infer behaviourally: read whole menu, or track nav
input, or hook `MouseClickDecide`/`SetSelectState`):**
`UAT_UIXCmnPause` (battle pause — rows `UAT_UIGameoverBar` have no state either; **worst case**),
`UAT_UIGameTitle`/`GameTitleList` (title), `UAT_UIGameWindow`/`SystemWindow`/`MenuWindow`/`Xcmn_Win00_Choice`
(message + Yes/No — highlight image only), `UAT_UIChoiceWin`/`ChoiceCmd` (hover image), `UAT_UICompZList`,
and all leaf `*Choice` rows (`List00/01/03/06Choice` — `SetSelect(bool)` setter but **no getter**;
highlight is a `MaterialInstanceDynamic`). `UAT_UIBar03Choice` exposes `int32 Volume` (slider value) but
its selection is `CursorFade` (not readable).

### Selection — VERIFIED against the object dump (2026-07-01)
The reflection dump (`UE4SS_ObjectDump.txt`, 80 MB) is the authority on what Lua can actually
call/read — a member in the CXX header that is NOT in the dump is C++-only and unusable. Confirmed:

- **`GetSelectValue() -> int32` is REFLECTED and CALLABLE from Lua** on `UAT_UIMenuListBase00`
  (dump `Function …AT_UIMenuListBase00:GetSelectValue`), `…Base01`, `…Base03`, and
  `…MenuBarBase03`. This is the game's ONE generic, robust selection read — the highlighted
  row index — and every screen that embeds one of these lists inherits it: **Item/Inventory**
  (`UIItemMenu.Xmenu_List00` = Cmn01MenuList09), **Shop** (`ShopTop.WidgetSkillList` =
  Cmn00MenuList11), **Shop Develop/Custom**, **Skill Customize** (`SkillListMenu`), **Dragon
  Ball menu**, **Options** (`Cmn03MenuBar10` = MenuBarBase03), quest-retry, status/party
  sublists, generic `Cmn*MenuList*`. Rows live in `ListPlateCtn` (TArray, **1-based in UE4SS
  Lua** → engine index + 1); each row (`UAT_UIList0NChoice`) has `TxtName` + `TxtNum`.
  → Encapsulated as `A.list_select_index` / `A.list_selected_row` in `ui_archetypes.lua`.
- **`MenuListBase06` is the exception — NO `GetSelectValue`** (only `SelectUp/Down/Page*`). The
  **quest LIST** is a Base06 → stays highlight-only. (Its detail panel `Quest_Sub` text is readable.)
- **Stored reflected index fields** (read directly, no call): `UAT_UIGameover.CurrentSelectIndex`
  (`IntProperty [o:3E0]`), `USimpleTalkWindow.CurrentPage`, `UAT_UIInfoLog.CurrentIdx`,
  `UAT_UIXmenuCommu.CurRank`.
- **Navigation hub:** `UMenuManager` exposes reflected pointers to every submenu
  (`m_xItemInventoryMenu`, `m_xQuestMenu`, `m_xOptionMenu`, `m_xShop*`, …) — traverse from it to
  find the active menu, then call `GetSelectValue()` on its list. Field ring selection is only
  `URootMenu.OnSelectItem(int32)` (a push EVENT, hookable, not a pull getter).
- **Genuinely material/highlight-only (NO reflected index anywhere — our heuristics are the
  game's own mechanism, keep them):** title, all system/message/choice windows (dialog,
  difficulty, Yes/No), **battle pause** (`Xcmn_Pause_C`/`UAT_UIGameoverBar` — zero selection
  state), CompZ encyclopedia, world/fast-travel map (analog cursor), skill tree (2-D graph),
  field/lock-on cursors, and the Base06 quest list. These need image/geometry inference.

---

## 2. Current adapters — status vs the headers

| Adapter | Target → native parent | Verdict |
|---|---|---|
| `screen_title` | `Gametitle_C` → `UAT_UIGameTitle`/`…List` | **Keep.** Material-only; our "`Txt_Select` visible" is the intended read. (Container itself not in the dump.) |
| `screen_options` | `Start_Option_C` → `UAT_UIStartOption`; rows `Xlist_Bar03_C` → `UAT_UIBar03Choice` | **Selection: keep** (`Ins_Cursor_Fad` = native `CursorFade`, no index). **Slider value: could use native `int32 Volume`** instead of counting 20 segments — but VERIFY its range (0–20 vs 0–100) at runtime first; current segment-count works, so low priority. Alternatively the parent list is a `MenuListBase03` → `GetSelectValue()` (readable index) if we ever want a true index. |
| `screen_choicelist` (difficulty) | `Xcmn_Win01_List_C` → `UAT_UIGameWindowChoice` | **Keep.** `Img_Xwin01_List` highlight is the intended selection signal (`SetSelectState(bool)` has no getter). |
| `screen_dialog` (Yes/No + messages) | `Xcmn_Win00_C`/`Win01_C` + `Xcmn_Win00_Choice_C` → `UAT_UISystemWindow`/`Choice` | **Keep.** Highlight-image only; `Active/DeactiveColor` are static, `defaultSelectNo` is a setup arg, not live state. |
| `screen_pause` (battle pause) | `Xcmn_Pause_C` → `UAT_UIXCmnPause`; rows `List_Bar_C` → `UAT_UIGameoverBar` | **Per-item selection added (2026-07-01).** Confirmed NO reflected index on container OR rows (both are bare shells; the sibling classes with a selection index are not on this inheritance path — `PnlCursEff` is just a Border, material-only). Selection now inferred from the readable signal the highlight animates: the cursor-glow image `Ins_Curs_Hexa_Eff00.ColorAndOpacity.A` — the selected row's glow alpha leads. Debounced; falls back to the old whole-menu read if the alphas are uniform. **Pending in-game verify.** |
| `screen_dialogue` (story) | `Xcmn_Subtitles_C` → `UATUISubtitles`; `Field_Talk_Win_C` → `UAT_UIFieldTalkWin` | **NEW (2026-07-01).** Reactive line reader (no selection): polls both surfaces, reads speaker + line (`Txt_Name`/`Txt_Selif`; `Txt_Speaker`/`Txt_Msg`) via plain `mainTxt`, then reflected `GetText()`, then rich markup; announces each new "Speaker: line" (diff-gated). **Pending in-game verify.** |
| `screen_shop` (shop) | `Shop_Top_C` → `UAT_UIShopTop`; list `WidgetSkillList` = Cmn00MenuList11 → MenuListBase00 | **NEW (2026-07-01).** First user of the robust `A.list_selected_row` (`GetSelectValue()` index → `ListPlateCtn[idx].TxtName/TxtNum`). Additive + guarded — inactive (harmless) if the header-read widget names differ live. **Pending in-game verify of `WidgetSkillList`/`ListPlateCtn`/`TxtName`.** |
| `screen_tutorial` | `Battle_Tips_Tutorial_C` → `UAT_UIBattleTipsTutorial`; rows `Battle_Tips_List_C` → `UAT_UIBattleTipsList` | **REFACTORED (2026-07-01):** read the 9 direct rows `Battle_Tips_List_00..08`, each `Txt_Btn` (glyph via `markup_to_speech`) + `Txt_Operat` (action) → "button: action"; header from the container's non-row text. Fallback to the old subtree scan if the native row props aren't reachable. |
| `screen_loading` | `Loading_C`/`Tips_C` → `UAT_UILoadingScreen`/`UAT_UITips` | **Keep.** Free-form recap/tips text; note native nodes `Txt_Title/Detail/Tips_Title/Tips_Detail` (Loading) and `Txt_Title/Subtitle/Detail00..04/Page` (Tips) if we want structured reads. The "Saving…" indicator is `Icon_Save_C → UAT_UIIconSave.Txt_Saving`. |

**Net:** only the tutorial had a clearly-better structural read; the other selection heuristics are the
game's intended mechanism (highlight image/material), confirmed. Don't "improve" them into regressions.

---

## 3. Roadmap by domain (future adapters)

### Dialogue / subtitles / story — the big opportunity (reactive)
**Implemented (2026-07-01):** `screen_dialogue.lua` POLLS `Xcmn_Subtitles_C` (cutscene/skill VO:
`Txt_Name`+`Txt_Selif`) and `Field_Talk_Win_C` (overworld NPC: `Txt_Speaker`+`Txt_Msg`), reading each via
plain `mainTxt` → reflected `GetText()` → rich markup, and announcing "Speaker: line" on change. Choices
(`Choice_Win01`/`Choice_Cmd00/01` under `Quest_Sub_C`, highlight-image selection via `Dmy_Choice_Hover`)
and telops (`Quest_Main_Telop_C`, 12 slots) are NOT yet read — next pass. `AAT_GameHUD::OnMobMessage(FString
Speaker, FString Message, bool)` IS a reflected UFunction (hookable) for lower-latency field talk, but
subtitles do NOT route through it, so polling stays the primary path.

**Universal chokepoint:** every dialogue line flows through `UCFUIXcmnMultiLineText::SetText(FText)` with
already-localized text. **Hook it once** (fires per line, no diff-gating) and route by the owning widget:
speaker nodes (`TextName`/`TextBox_Name`/`Txt_Speaker`) vs body (`TextSelif`/`TextBox_Message`/`Txt_Msg`).
For field NPC talk, hook the higher-level **`AAT_GameHUD::OnMobMessage(FString Speaker, FString Message,
bool)`** — resolved speaker+line in one call. Dialogue widgets: `Xcmn_Subtitles_C → UATUISubtitles`
(`Txt_Name`+`Txt_Selif`), `Field_Talk_Win_C → UAT_UIFieldTalkWin`, `Telop_Txt`/`Quest_Main_Telop`,
choices `Choice_Cmd`/`Choice_Win`. Gate audio cues while any of these is active. **This is how a blind
player follows the story — highest future value.**

### Combat HUD
Grab **`UAT_UIBattleManager`** once (via the HUD's reflected `UIBattleManager` field) — it points to every
battle widget (`BattleHudPlayer`/`BattleHudEnemy`/`CommandPalette`/`TotalDamage`/`BattleCond`). **VERIFIED
reflected (2026-07-01):** every `WL_Txt*` node is a `CFUIXcmnMultiLineText` with a reflected `GetText()` →
TextProperty — read via `node:GetText():ToString()` (don't descend to leaves). Total damage is the reflected
`UAT_UITotalDamage.CurrentValue` IntProperty (skip the sprite digits). Readable as text: player/enemy HP & Ki
numbers, level, name (`Battle_Hud_P/E_Main.WL_TxtNumHp/NumSp/Break` — read the non-`Shadow` node), each
skill-palette slot's name+Ki cost (`Battle_Command_List00.WL_TxtList/TxtNumCost`),
fired skill (`Battle_Skill_Name`), battle conditions (`Battle_Cond.TxtConds` array), timer, remaining
enemies, total damage (`UAT_UITotalDamage.CurrentValue` int), QTE prompt (`Qte_Battle_Blow.WL_TxtAction`).
**Not readable** (bar/sprite/localized image only): the **Heat/Tension gauge has no number**, damage-pop
& combo counts are digit sprites, BREAK/dodge/counter banners are textures, and the raw gauge components
(`UATCharacterGaugeLife/Magic/Sparking`) are opaque. Best hooks: `OnBattleStart/End`, `OnChangeTarget`,
`OnSkillBegin`, `OnComboAddEvent`, `AnimTextSetting(int32 damage)`, `RequestStartAnim(rank, exp)`.

### Map / navigation (audio-nav)
Direction has **no numeric angle field anywhere** — compute bearing/range yourself:
- **Player transform:** `UAT_UIMiniMapRadar.PlayerIns` → `AAT_Character` → `GetActorLocation()` /
  `GetActorRotation()` (yaw); camera via `.CameraMng` (`APlayerCameraManager`).
- **POI/objective world coords:** each map icon holds an actor pointer — `UAT_UIMiniMapIcon.TargetActor`
  (minimap) / `UAT_MapIconSwitchBase.Target` (full map) / `FMapManagerIconInfo.Owner`
  (`UAT_UIMapManager.MapIconInfoCacheList`). Subtract player pos → distance + relative bearing → sound.
  Classify via `UATMapIconComponent.MapIconType` (`EMapIcon`) or `EEncounterDirectionType`
  {NORMAL, SUB_QUEST, MAIN_QUEST, ENEMY, DB, CHECKPOINT}.
- **Objective text (no coords):** `Quest_Navi_*`/`CompZ_Navi` labels, `FUIQuestInfo`.
- **Already-formatted distance:** `Xcmn_Distance.DistanceText` ("120m") as a fallback.
- **Area name:** `UAT_UIMiniMapRadar.CurrentAreaName` / `Map_M.Txt_Area`.
- **Fast travel:** `FFastTravelDestinationTableRow` has real `DestinationLocationX/Y/Z`.
This matches the queryable-world → bearing/distance → sound model in
`D:\code\modding projects\reference\audio-navigation`.

### Other menus (mostly readable)
Prefer any host that is a **`UAT_UIMenuListBase00/01/03/06`** → `GetSelectValue()` + `ListPlateCtn[i].Txt*`.
Master logic controller `UMenuManager` holds `m_xItemInventoryMenu`/`m_xQuestMenu`/`m_xOptionMenu`/…
- **Field/overworld pause (Start menu):** `Start_Top_C → UAT_UIStartTop` (top ring `UIStartTopList` uses
  **image-font labels, no index** → needs an index→name map, index via `URootMenu.OnSelectItem(int32)`).
  Options/Return-to-Title/Save confirmations are `UAT_UISystemWindow` (`SetupSelectOnlyWindow(items,
  default)`; choices readable `ChoiceTxt`, selection highlight-only).
- **Shop:** `Shop_Top_C → UAT_UIShopTop` (rows `UAT_UIXListBar01`, skill list `UAT_UICmn00MenuList11` →
  `GetSelectValue()`); cooking `Shop_Cook_C → UAT_UICookingMenu` (`SetSelectTab`).
- **Inventory:** `Start_Item_Win00_C → UAT_UIStartItemWin00` (`WL_Txt_List` = readable item labels); item
  struct `FUIXlistCustom00Param{ItemId, ItemType, ItemName, rarity}`.
- **Quest:** `Quest_Sub_C → UAT_UIQuestSub` (readable title/detail/reward; accept via `UAT_UIChoiceWin`);
  quest list rows `UAT_UIList06Choice`.

---

## 4. Discovery method (reproduce)
`Ctrl+H` (CXX headers) works with the GUI console OFF; set
`[CXXHeaderGenerator] LoadAllAssetsBeforeGeneratingCXXHeaders = 1` first so lazily-loaded UI is included.
Then `grep "class U<Name> " AT.hpp` and read a ~40-line window; the `_C` blueprint is in its own
`<Name>.hpp`. See `D:\code\modding projects\reference\engines\ue4ss\ue4ss-discovery-tools.md`.
