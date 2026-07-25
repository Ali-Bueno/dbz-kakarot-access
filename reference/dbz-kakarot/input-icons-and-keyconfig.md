# DBZ Kakarot — inline input icons (`<inputicon>`) & KeyConfig resolution (runtime-confirmed)

How the game shows **button/key glyphs inside text**, and how to turn them into spoken button names.
Confirmed live (UE4SS Lua reflection + screenshots) on 2026-07-01 while building the Options readers.

> **Why this matters beyond Options:** the same `<inputicon>` markup + KeyConfig system is how the game
> renders button prompts EVERYWHERE inline text appears — **move lists, battle tutorials, quest/objective
> text, HUD prompts, help/agreement popups**. Any of those become readable by reusing the exact
> mechanism below. Don't re-discover it; reuse `ui_archetypes.lua`.

## Two text renderers per text node (key gotcha)

Every game text node is an `Xcmn_MultiLineText_C` (there is **no** UMG `TextBlock`). It holds **two**
renderers, and which one is visible depends on the content:

| child property | class | holds |
|---|---|---|
| `mainTxt` | `CFUIMultiLineTextBox` | **plain** text (labels, values) |
| `ExMainTxt` | `CFExtendRichTextBlock` | **rich** text with inline-icon **markup** |

- Read plain text: `node.mainTxt.Text:ToString()`.
- Read the rich **markup source**: `node.ExMainTxt.Text:ToString()` → returns the raw string incl. tags,
  e.g. `" <inputicon KeyConfigId=\"KeyConfig_Controller_Btn_B\"/> "`.
- Exactly one renderer is shown; gate on `ExMainTxt:IsVisible()` to know a node is showing a glyph.
- **Trap:** the hidden renderer keeps **stale** content. On the controller tab `mainTxt` held recycled
  graphics-tab values ("1920x1080", "FXAA") while the real binding was only in `ExMainTxt`. Never trust
  the hidden renderer.

## The markup: `<inputicon KeyConfigId="..."/>`

Inline button/key glyphs are a decorator tag in the rich text source:

```
<inputicon KeyConfigId="KeyConfig_Controller_Btn_B"/>     ← a physical controller button (direct)
<inputicon KeyConfigId="KeyConfig_Battle_HighBoost"/>     ← an ACTION alias (needs resolution)
```

To make any inline-icon text accessible: read `ExMainTxt.Text`, and replace each
`<inputicon KeyConfigId="ID"/>` with the resolved button name (below). Other tags seen: `<span color=...>`
(plain color, strip it). Regex used: `KeyConfigId="([^"]+)"`.

## Resolving a KeyConfigId → button

Two id shapes:

1. **Direct button id** — `KeyConfig_Controller_Btn_<TOKEN>` (and the raw `Controller_Btn_<TOKEN>`).
   The token after the last `Btn_` **is** the button: `B`, `X`, `A`, `Y`, `LB`, `RB`, `LT`, `RT`, `L3`,
   `R3`, `Start`, `Back`. Canonical **Xbox-style**, device-independent (only the glyph changes per device).

2. **Action alias** — `KeyConfig_Battle_HighBoost`, `KeyConfig_Battle_Guard`, `KeyConfig_Boost`, … The
   physical button is resolved through the shared icon-data asset:

   Asset: **`/Game/CFramework/DataAssets/CFTextIconData.CFTextIconData`** (`StaticFindObject`).
   Array: **`KeyConfigList`** — 159 entries, element struct **`/Script/CFramework.CFKeyConfigAssign`**.
   **The array is 1-based; indexing `[0]` aborts.** Struct fields (StrProperty each):

   | field | meaning | example |
   |---|---|---|
   | `ConfigName` | the config slot / alias id | `Battle_HighBoost`, `KeyConfig_Battle_HighBoost`, `Jump` |
   | `IconName` | glyph of the **current device**, **INDEXED** (not descriptive) | `Btn_L3`, `Btn_2`, `Btn_Key_6` |
   | `DynamicAssignInputId` | the abstract input action | `Battle_HighBoost`, `Boost`, `Jump` |
   | `DynamicAssignInputControllerId` | **semantic controller button** | `Controller_Btn_L3` |

   The `KeyConfig_*` alias entries have EMPTY `IconName`/`ctrl` but a `DynamicAssignInputId`; a *different*
   entry with that same dyn id carries the controller button. So build two maps once and cache:
   - `configToCtrl[ConfigName] = ctrl` (for entries with a ctrl)
   - `configToDyn[ConfigName]  = dyn`  and  `dynToCtrl[dyn] = ctrl`

   Resolve: `ctrl = configToCtrl[id] or dynToCtrl[ configToDyn[id] ]` → strip `Controller_Btn_` → token.

## The player's REMAP lives in the save, not in this asset (2026-07-25)

`CFTextIconData.KeyConfigList` is the resolution table the *renderer* uses. The layout the **player**
configured in Options is elsewhere, and it persists across sessions:

- **`FATSaveSystemInputAssign`** (`ATExt.hpp:750`), held at **`UATSaveSystem+0x720`** — reached by the
  same chain as the subtitles option: `GameInstance.SaveManager` → `.SaveSystem` → `.InputAssign`
  (never "first instance found": the object array holds several save systems and the pristine template
  answers first — see `screen_dialogue.subtitles_on`).
- It carries **two directions**: one `FName` per **action** (`Battle_MeleeAtk`, `Battle_KiAtk`,
  `Boost`, … 48 of them — the keyboard side) and one per **physical controller button**
  (`Controller_Btn_B/X/A/Y/LB/LT/L3/RB/RT/R3/Start/Back`) holding the action currently bound to it.
  Plus `Keyboard_Type` (`ECFKeyboardLayoutType` — QWERTY/QWERTZ/AZERTY, whose defaults live in
  `UATDataAssetGlobal.DefaultInputAssignParam`, `AT.hpp:1033`).

### Every `Controller_Btn_*` id is a SLOT, not a button (proven 2026-07-25)

This is the whole trap, and it is invisible until someone remaps their pad. `Controller_Btn_B` is the
*name of a slot* — "the slot melee shipped on" — and it **keeps that name forever**. The save says which
physical key the slot currently drives. Evidence, one F7 dump taken with melee moved to X and ki to B
(`dumps/dump_keyconfig_1784989557_001.txt`):

| what | value after the remap |
|---|---|
| melee row's markup | `<inputicon KeyConfigId="KeyConfig_Controller_Btn_B"/>` — **unchanged** |
| asset entry `Battle_Attack` (dyn `Battle_MeleeAtk`) | `ctrl = Controller_Btn_B` — **unchanged** |
| save `InputAssign.Controller_Btn_B` | `Gamepad_FaceButton_Left` → physically **X** |
| save `InputAssign.Controller_Btn_X` | `Gamepad_FaceButton_Right` → physically **B** |

So resolving the markup or the asset alone reports the **pre-remap** button, and no cache invalidation
can help — the asset is *not* rewritten on a rebind. (All 68 live-vs-`DefaultKeyConfigList` differences
in that dump are one single thing: the game fills `DynamicAssignInputControllerId` in at runtime and the
default copy has it blank. `DefaultKeyConfigList` is at 0x2C8, live `KeyConfigList` at 0x48.)

**Rule: slot id → `InputAssign["Controller_Btn_" .. token]` → `FKey` name → spoken button.** The FKey
names are Unreal's own `EKeys` (`Gamepad_FaceButton_Bottom/Right/Left/Top` = A/B/X/Y,
`Gamepad_Left|RightShoulder` = LB/RB, `…Trigger` = LT/RT, `…Thumbstick` = L3/R3,
`Gamepad_Special_Right|Left` = Start/Back). Under a default config every slot maps to its own button, so
the whole hop is the identity — which is also the self-check that the table above is right.
Implemented once, in `ui_archetypes.A.physical_token`, behind `A.button_name` (the single funnel all
controller ids reach) — do not re-derive it per screen.

Related but NOT the mechanism here: `UCFExtendRichTextBlock::SetIconToKeyConfigByName(SearchConfigName,
SetIconName)` (`CFramework.hpp:678`) can repoint an icon without touching the markup. The dump shows the
icons unchanged, so the game is not using it for the remap.

## What is and isn't recoverable

- **Controller button: YES**, for every action, on every tab/screen, device-independent (via `ctrl`).
- **Keyboard key: YES since 2026-07-25** (this reverses the old verdict below, which was true only of the
  *icon* data). The literal key is in the same save struct, one `FName` per action:
  `InputAssign.Battle_MeleeAtk = LeftMouseButton`, `Jump = SpaceBar`, `Move_Up = W`, … plus
  `Keyboard_Type` (`ECFKeyboardLayoutType`, QWERTY/QWERTZ/AZERTY — factory values in
  `UATDataAssetGlobal.DefaultInputAssignParam`, `AT.hpp:1033`). Not wired into any reader yet; the mod
  currently speaks the controller side only.
  - *Why it looked unrecoverable:* from the ICON side the keyboard glyph is an **indexed** `IconName`
    (`Btn_Key_6`) with no key field, and UE's `UPlayerInput.ActionMappings`/`AxisMappings` are **empty
    (count 0)** — the game uses a proprietary CFramework input system. The data was never in the icon
    asset; it was in the save all along.

## Face-button GLYPH INDEX order (`EATPlatBtnId`) — the A/B mirror trap

The prompt glyphs (`UAT_UIXcmnPlatBtn`, incl. every entry of the keyhelp bar) carry no semantic id at
all in most screens — `DynamicAssignInputControllerId` and `CurrentActionID` come back EMPTY, and the
only thing that answers is the **pad index** (`KeyIdsForPad` / the indexed texture `Btn00..03`). So the
index→button table is the whole ballgame, and it is **not** the naive Xbox order:

`EATPlatBtnId` is a PlayStation enum (`BtnR1`/`BtnL1`, `BtnOptions`, `BtnTouchPad` — AT_enums.hpp) and it
enumerates the faces in the Japanese order **right, bottom, left, top** = ○ ✕ □ △:

| index | physical | Xbox name (what the game DRAWS) |
|---|---|---|
| `Btn00` = 0 | right | **B** |
| `Btn01` = 1 | bottom | **A** |
| `Btn02` = 2 | left | X |
| `Btn03` = 3 | top | Y |

Confirmed live (dump_keyhelp 2026-07-14): the item menu's "Usar" (the pad's A) carries index **1** and
"Atrás" (B) carries index **0**. Only the A/B pair mirrors — X/Y already line up, which is exactly why a
half-checked fix ("just swap them all") is wrong. The KeyConfig asset's `IconName` pairing does NOT
substitute for this table (it mis-resolved index 3 → "B", dump_fishing 2026-07-03). Table lives in
`ui_archetypes.lua` (`FACE_TOKEN`) and is shared by the keyhelp bar, the QTE/fishing prompts and the item
quick-slot palette (whose four slots ARE these four buttons).

## The keyhelp bar (`Xcmn_Keyhelp_C`) — reading the row in the order it's shown

The bar exposes 9 index-aligned pairs (`Txt_Keyhelp_01..09` + `Xcmn_Btn_Plat_01..09`). **The number is
a slot id, not a place in the row**: the bar is a CanvasPanel whose row the native class lays out itself
(`UAT_UIKeyHelp`: `KeyHelpRightMargin`, `KeyHelpItemGapX`), so reading in index order announces the row
in an order matching nothing on screen (in the character menu it put "Atrás" first and "Paleta de Súper
Ataque" last — the reverse of what the player sees).

Getting the real position, in the order that actually works (`keyhelp.lua widget_x`):
1. `widget.Slot:GetPosition()` — a `CanvasPanelSlot`; **this is the one that answers**;
2. `Slot.LayoutData.Offsets.Left` — reads back **0.0 on every entry** through reflection, so it is
   useless here (a nested-struct property read that returns a zeroed struct — don't trust it alone);
3. `widget.RenderTransform.Translation.X`, then the same probes up the ancestors (a leaf can sit at 0
   inside a container that carries the placement).

Sort by that X and you get the on-screen left-to-right order. If ANY entry can't be placed, don't sort at
all — a partial sort interleaves placed and unplaced entries into an order matching nothing.

Also: the direction/stick glyphs (`Btn_Key_Ud/Lr/All`, `Stk_*`) are *navigation*, not choices — an
accessibility reader should drop them and speak only the real button presses.

## Platform glyph sets

`PLAT_X` = **Xbox** glyph set (`/Game/Art/UI/Xcmn/PLAT_X/…`), `PLAT_P` = PlayStation, `PLAT_W` = other.
An 8BitDo pad in Xbox mode drives `PLAT_X` even with a DualSense connected. The KeyConfig **token stays
Xbox-canonical** regardless (`Btn_B`); only the rendered glyph swaps. Optional future: translate to PS
names (A→✕, B→○, X→□, Y→△, LB→L1, RB→R1, LT→L2, RT→R2, L3/R3 same) when a PS set is active.

## Reuse (don't re-discover)

Implemented in **`mod/KakarotAccess/Scripts/ui_archetypes.lua`**:
- `A.button_name(id)` — token → "botón X"/friendly (`BUTTON_NAMES`).
- `A.row_keyconfig(row)` — pulls the `KeyConfigId` out of a row's `ExMainTxt` markup (visibility-gated).
- `build_bindings()` / `resolve_ctrl(id)` — the cached KeyConfigList resolver (`A.clear_binding_cache()`
  drops the cache; call it on screen entry since a rebind changes the mapping).
- `A.row_binding(row)` — direct id → `button_name`; action alias → `resolve_ctrl` → `button_name`.

For a **move list / tutorial / quest** reader: find the `Xcmn_MultiLineText_C` (or its `CFExtendRichTextBlock`)
showing the prompt, read `ExMainTxt.Text`, and for each `<inputicon KeyConfigId="…"/>` substitute
`A.button_name(id) or A.button_name(resolve_ctrl(id))`. Same pipeline, different host widget.

## Gotchas recap

- 1-based struct arrays; index 0 aborts uncatchably (pcall can't catch a C++ abort).
- Accessing a struct field with a **wrong name** also aborts uncatchably → reflect the `UScriptStruct`
  (`StaticFindObject("/Script/CFramework.CFKeyConfigAssign"):ForEachProperty(...)`) to get real names first.
- Reading 159 structs per focus tick is too slow → build the map **once** and cache.
