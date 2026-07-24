# dbz-kakarot-input-icons-ref

> Where the reusable DBZ Kakarot inline button-glyph / KeyConfig RE knowledge is documented


DBZ Kakarot renders button/key prompts as inline `<inputicon KeyConfigId="..."/>` markup inside the
rich-text renderer (`ExMainTxt`, a `CFExtendRichTextBlock`) of every `Xcmn_MultiLineText_C` node — the
plain `mainTxt` (`CFUIMultiLineTextBox`) holds no glyph. This same system shows button prompts in **move
lists, tutorials, quest text, HUD prompts**, not just Options, so reuse it when accessibilizing those.

Full runtime-confirmed spec (markup, the `CFTextIconData`/`KeyConfigList` resolver, controller-button
recoverable but keyboard-key NOT, `PLAT_X`=Xbox, gotchas): repo doc
`reference/dbz-kakarot/input-icons-and-keyconfig.md`. Implementation to reuse:
`mod/KakarotAccess/Scripts/ui_archetypes.lua` (`button_name`, `row_keyconfig`, `resolve_ctrl`, `row_binding`).

**FACE-BUTTON INDEX ORDER — the A/B mirror trap (2026-07-14, live-confirmed, commit d9b4c79).** Prompt
glyphs (`UAT_UIXcmnPlatBtn`, incl. every keyhelp-bar entry) usually expose NO semantic id (ctrl id and
action id come back empty); only the pad INDEX answers. `EATPlatBtnId` is a PlayStation enum (BtnR1/BtnL1,
BtnOptions, BtnTouchPad) ordering the faces right/bottom/left/top (○ ✕ □ △), so in Xbox names:
**0=B, 1=A, 2=X, 3=Y** — A/B are the MIRROR of the naive order while X/Y already match (so never "just
swap all four"). `ui_archetypes.FACE_TOKEN`; shared by the keyhelp bar, QTE/fishing prompts and the item
quick-slot palette (whose 4 slots ARE these buttons).

**Keyhelp bar order:** `Txt_Keyhelp_NN`'s number is a SLOT ID, not a place in the row (the game lays the
CanvasPanel out itself). On-screen X comes from `Slot:GetPosition()`; `Slot.LayoutData.Offsets` reflects
back as 0.0 and is useless. Direction/stick glyphs = navigation, not choices.

Part of the broader plan in [dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md).
