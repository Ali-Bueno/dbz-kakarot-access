# DBZ Kakarot — UI & text architecture (static recon)

Engine study for the accessibility mod. Findings from **static** analysis of the shipped `.pak`
files (no game running yet). Confidence noted per section; anything needing runtime confirmation is
flagged for the first in-game session (UE4SS Ctrl+H header dump / Ctrl+J object dump).

## How this was obtained

- Game: `D:\games\steam\steamapps\common\DRAGON BALL Z KAKAROT`, UE project `AT`, **Unreal Engine 4.21**.
- Paks: `AT/Content/Paks/pakchunk0|1-WindowsNoEditor.pak` (~14 GB, pak version 7).
  **Index NOT encrypted** (`bEncryptedIndex=0`, zero key GUID) → unpackable without an AES key.
- Tool: [`repak`](https://github.com/trumank/repak) v0.2.3 (`repak list` / `repak get`). 348,382 entries total.

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

## TODO at first in-game session (runtime confirmation)

1. Confirm UE4SS loads (UE4SS.log) and the KakarotAccess C++ bridge logs "PRISM ready".
2. **Ctrl+H** + **Ctrl+J** dumps → commit the menu/UI + message-manager class names here.
3. Identify: menu container class, current-selection/cursor property, item label getter, and the
   message ID→string resolver. These are the hook points for "accessibilize menus".
4. Confirm which `PLAT_*` set and language the PC build uses.
