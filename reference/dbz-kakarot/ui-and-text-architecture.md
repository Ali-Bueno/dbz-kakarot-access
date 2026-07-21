# DBZ Kakarot — UI & text architecture (static recon)

Engine study for the accessibility mod. Findings from **static** analysis of the shipped `.pak`
files (no game running yet). Confidence noted per section; anything needing runtime confirmation is
flagged for the first in-game session (UE4SS Ctrl+H header dump / Ctrl+J object dump).

## How this was obtained

- Game: `D:\games\steam\steamapps\common\DRAGON BALL Z KAKAROT`, UE project `AT`, **Unreal Engine 4.21**.
- Paks: `AT/Content/Paks/pakchunk0|1-WindowsNoEditor.pak` (~14 GB, pak version 7).
  **Index NOT encrypted** (`bEncryptedIndex=0`, zero key GUID) → unpackable without an AES key.
- Tool: [`repak`](https://github.com/trumank/repak) v0.2.3 (`repak list` / `repak get`). 348,382 entries total.

### The pak path index (regenerated 2026-07-21) — grep this before asking the game anything

`D:\code\tools\repak\repak.exe` (v0.2.3, SHA256-verified release binary) and the prebuilt index at
**`D:\code\tools\repak\pak_index.txt`** — all 348,382 asset paths, sorted, 25 MB, deliberately OUTSIDE
git. Regenerate in ~6 s:

```powershell
$r = "D:\code\tools\repak\repak.exe"; $p = "…\AT\Content\Paks"
@("pakchunk0-WindowsNoEditor.pak","pakchunk1-WindowsNoEditor.pak") |
  ForEach-Object { & $r list "$p\$_" } | Sort-Object |
  Set-Content "D:\code\tools\repak\pak_index.txt" -Encoding UTF8
```

It answers "what exists, and what is it called" offline and instantly — no game, no dump, no crash.
It does NOT answer "what members does this class have" (that is `AT.hpp`, Ctrl+H) nor "what is
loaded right now" (Ctrl+J).

**CORRECTION to the section below (2026-07-21):** "UI is not UMG / `Blueprints/UI/` is almost empty"
is right about `Blueprints/UI/`, but the widget blueprints DO exist — they live under
**`AT/Content/Art/UI/<Screen>/`** (e.g. `Art/UI/Info_Log/Info_Log.uasset`, `Info_Log02`,
`Info_Log_Bar`, `_Bar01`, `_Bar02`, `Info_Log_Level`), with localized variants under
`Art/UI_L10N/<lang>/<Screen>/`. So the index CAN enumerate a screen's blueprint family. The
architectural conclusion still holds: the logic and readable state live on the NATIVE parent classes.
Worked example, same day: `Info_Log_Level.uasset` exists in the paks yet `Info_Log_Level_C` appears
nowhere in the ObjectDump → the class ships but is never instantiated, which is exactly why level-up
banners were silent until they were read off `Info_Log02_C` instead.

## UI is NOT standard UMG (high confidence)

The menus are **not** built from conventional UMG Widget Blueprints:

- `AT/Content/Blueprints/UI/` is almost empty (2 files, `Capture/`).
- No `WBP_*` assets exist; only ~58 paths contain "Widget" at all.
- The bulk of `/UI/` (16,413 paths) is **art/textures** (`Charicon_*`, `C_Img_*`, `*_Tex`), under
  `Art/UI/`, `Art/UI_L10N/`, not logic.
- There is a game-specific framework dir `AT/Content/CFramework/`.

**Implication:** menu/HUD logic lives in **native C++ classes** (CyberConnect2 custom UI framework),
not in UMG `TextBlock`/`UserWidget`. So the standard "hook `UMG.TextBlock:SetText`" approach won't be
the primary path. We must identify the native UI classes/functions at runtime:
- First session: **Ctrl+H** (C++ header dump) and **Ctrl+J** (object dump), then search the headers for
  the menu/list/cursor classes (look under names related to `CFramework`, `UI`, `Menu`, `Window`,
  `Select`/`Cursor`, `Title`, `Option`).
- Use UE4SS Live View to watch which objects/properties change as the cursor moves in a menu.

## Game text lives in a custom "Message" system (high confidence on location)

Localized strings are **not** in UE `.locres` (those exist only for `Engine`/`OnlineSubsystem`, not the
game). The game's text is here:

```
AT/Content/Message/PLAT_X/<lang>/messageData.uasset (+ .uexp)   ← all game strings, per language
AT/Content/Message/PLAT_X/<lang>/nounParam.uasset  (+ .uexp)    ← nouns/params (substitution)
AT/Content/Message/PLAT_W/<lang>/...                            ← platform variant
AT/Content/Message/PLAT_P/<lang>/...                            ← platform variant
```

- Languages present include `en_US`, `es_ES`, `es_MX`, `ja_JP`, `fr_FR`, `de_DE`, `pt_BR`, … (20 langs).
- `PLAT_X` / `PLAT_W` / `PLAT_P` are almost certainly **platform variants** (button glyphs etc.);
  which one PC uses is TBD at runtime.
- `messageData.uexp` is a **custom binary blob** (~21 MB for es_ES), NOT plain UTF-8/UTF-16 text — a
  naive `strings` finds nothing. Parsing it statically would need full uasset parsing (FModel/CUE4Parse)
  **and** reverse-engineering CC2's message container. **Not worth it statically.**

**Implication:** the native UI shows text by **message ID**. The accessibility mod should read the
**same message system at runtime** (the loaded message table UObject, or by hooking the function that
resolves an ID → display string). That gives us correct, already-localized labels for free. Identifying
that class/function is a first-session task (look for a "Message"/"Text" manager class in the Ctrl+H dump;
hook its ID→FString/FText getter).

## Other notable content roots (for later mod sections)

`AT/Content/`: `Blueprints/` (Character 12.8k, Quest 5.8k, Unique, Demo, Mob, AI, Field, Battle, …),
`MasterData/` (data tables incl. `MasterData/UI/CharacterIconParam`), `Parameter/`, `Maps/`
(`Boot/` has the boot menu), `Sound/`, `DataAssets/`.

## Detecting the game's current language at runtime (CONFIRMED 2026-07-01)

**Do NOT use UE culture** — `UKismetInternationalizationLibrary.GetCurrentCulture/Language/Locale` all
return **`"en"`** even when the game runs in Spanish (CC2 decouples its language from UE culture). Instead
read it from the **message manager's loaded data table path**:

```
GameInstance (BP_ATGameInstance_C → AT_GameInstance → CFGameInstance → GameInstance) →
  .MessageManager (AT_MessageManager_BP_C : CFMessageManager) →
    .DataTable  → full name ".../Message/PLAT_W/es_ES/messageData.messageData"
```

Parse the lang out of that path: `path:match("/Message/[%w_]+/(%a%a)_%u%u")` → `es` (es_ES and es_MX both →
`es`). `CFGameInstance` also exposes `LocalizeManager` (a `CFLocalizeManager`) but it has **no reflectable
fields** — the MessageManager path is the reliable source. Note the **two independent PLAT axes**: message
text uses `PLAT_W` here, while button glyphs use `PLAT_X` (Xbox) — don't conflate them. Implemented in
`mod/KakarotAccess/Scripts/i18n.lua` (`detect()`), which drives the mod's own localized strings.

## TODO at first in-game session (runtime confirmation)

1. Confirm UE4SS loads (UE4SS.log) and the KakarotAccess C++ bridge logs "PRISM ready".
2. **Ctrl+H** + **Ctrl+J** dumps → commit the menu/UI + message-manager class names here.
3. Identify: menu container class, current-selection/cursor property, item label getter, and the
   message ID→string resolver. These are the hook points for "accessibilize menus".
4. Confirm which `PLAT_*` set and language the PC build uses.
