# dbz-kakarot-pak-index

> Offline pak path index for Kakarot (repak at D:\\code\\tools\\repak, pak_index.txt, 348k paths) + the rule that LoadAllAssetsBefore* must stay 0


**Third RE source for Kakarot, offline and instant:** `D:\code\tools\repak\repak.exe` (v0.2.3,
SHA256-verified) + prebuilt **`D:\code\tools\repak\pak_index.txt`** — all **348,382** asset paths from
`pakchunk0/1-WindowsNoEditor.pak`, sorted, 25 MB, kept OUT of git. Regenerates in ~6 s (`repak list`
per pak → sort → file; recipe in `reference/dbz-kakarot/ui-and-text-architecture.md`). Paks are
**pak version 7, NOT encrypted** (`bEncryptedIndex=0`, zero key GUID) — no AES key needed.

Answers *what exists and what it is called*, with no game running. Does NOT answer class members
(that is `AT.hpp` / Ctrl+H) nor what is live (Ctrl+J). Widget blueprints live under
**`AT/Content/Art/UI/<Screen>/`** (localized twins in `Art/UI_L10N/<lang>/`), which corrects the older
"UI blueprints are almost absent" note in that same doc.

**HARD RULE — never set `LoadAllAssetsBeforeDumpingObjects` / `…BeforeGeneratingCXXHeaders` to 1 on
this game** (learned 2026-07-21): force-loading every asset reaches `AutoDebugMainUI_C`, a debug
blueprint left in the paks whose parent `AutoDebugUIBase` was stripped from the shipping build →
`LowLevelFatalError … Could not find SuperStruct` in `FAsyncLoadingThread`, **during** the load, so no
dump is written at all (UE4SS.log just ends at "Loading all assets..."). Dump with the flags at 0:
native classes are complete from the title screen anyway; BP `_C` classes accumulate as you visit
screens, so take one Ctrl+H at the END of a session. Both flags are now commented in the game's
`UE4SS-settings.ini` with the reason.

Game text is NOT statically extractable: it lives in CC2's custom `Message/PLAT_X/<lang>/
messageData.uexp` binary container, so the mod reads it at runtime. See
[dbz-kakarot-dumps-convention](dbz-kakarot-dumps-convention.md) and [dbz-kakarot-engine-facts](dbz-kakarot-engine-facts.md).
