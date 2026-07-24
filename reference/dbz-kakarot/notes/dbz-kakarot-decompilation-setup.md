# dbz-kakarot-decompilation-setup

> DBZ Kakarot native decompilation — exe is SteamStub DRM, unpack with Steamless before Ghidra; hidden selection indices in non-UPROPERTY tail gaps


Native RE workspace for the accessibility mod lives in the repo at `code/` (see
[dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md) for the Lua mod, [dbz-kakarot-ui-map](dbz-kakarot-ui-map.md) for the UI classes).

**CRITICAL: `AT-Win64-Shipping.exe` is DRM-protected (SteamStub v3.1 x64).** On disk `.text` is
100% encrypted (entropy 8.0) and the entry point sits in a `.bind` section → Ghidra on the raw
file = noise. **Must unpack first with Steamless** (installed at
`D:\code\decompilers\Steamless_v3.1.0.5`, `Steamless.CLI.exe --recalcchecksum <exe>`) →
`AT-Win64-Shipping.exe.unpacked.exe` (`.text` decrypted, `.bind` gone). That unpacked file is
what Ghidra analyzes. The **Rich header is stripped by SteamStub** (DOS stub zeroed) so it can't
validate the toolchain; use the optional-header **linker version 14.13 = VS2017 v141** + RTTI
present (901 `.?AV`) instead. Image base 0x140000000.

**Ghidra:** project at `code/ghidra/KakarotAT`, launched with a 16 GB-heap wrapper
`ghidra_12.0_PUBLIC\support\analyzeHeadless_kakarot.bat` (stock bat caps MAXMEM at 2G — too small).
Spec `x86:LE:64:default:windows` → MSVC RTTI analyzer + demangler run by default (real vftable/
class names). Post-script `code/ghidra/export_ui.py` decompiles the target UI classes' vftable
virtuals + xrefs to `code/decompiled/<Class>.c`.

**WHY native at all:** the two Lua dead ends (battle-pause per-item selection, field Start menu
section) fail because the selected index is a **non-UPROPERTY C++ member** — invisible to UE
reflection/Lua. The CXX dump proves it: each class has an unlisted **tail gap** after its last
reflected member where the private index lives — `UAT_UIXCmnPause` 0x438–0x500,
`UAT_UIXCmnHeader` 0x400–0x510 (FontType, set by SetFontType), `UAT_UIStartTop` 0x4D8–0x600.
Contrast `UAT_UIGameover.CurrentSelectIndex@0x3E0` which IS reflected. Ghidra pins the exact
`this+0xNNN` write; map in `code/structs/ui_target_layouts.h`.

**GHIDRA DONE (2026-07-02): full analysis OK (51 min, 1.4GB .rep, saved).** Two gotchas solved:
(1) **awt.dll JVM crash** — the "Embedded Media" analyzer decodes a malformed embedded PNG via
native ImageIO and hard-crashes the JVM (~19 min in). FIX: pre-script `disable_media.java` sets
`setAnalysisOption(program,"Embedded Media","false")` (+ `-Djava.awt.headless=true` in the 16GB
wrapper bat). (2) **Ghidra 12 dropped Jython** → GhidraScripts MUST be **Java**, not Python
(`Ghidra was not started with PyGhidra`). **KEY RE FINDING: the game has NO C++ RTTI for its own
classes** — the 901 `.?AV` are all third-party (ICU/OpenEXR/libwebm); UE class names exist only as
UTF-16 reflection strings (`AT_UIXCmnPause`: utf16=4/ascii=0). UE4 `/GR-`. So Ghidra's RTTI analyzer
recovers 0 game vtables (forcing it gave 536, all third-party). **Way in = UE native-function
registration table** `{const char* Name, FNativeFuncPtr exec}`: find ASCII UFunction name → exec
thunk → native impl (`code/ghidra/find_ufunc.java` + `decompile_addrs.java`). **RESULT: header
section (field menu) SOLVED statically** — `UAT_UIXCmnHeader::SetFontType`=`FUN_1416f7bd0`: FontType
arg = section id (indexes ~59 `Lang_*` labels: Lang_MainMenu/World/Char/Shop/Quest…); stores mode
byte at `this+0x400` + resolved label FString at `this+0x408` (mode0)/`this+0x460` (mode1) — hidden
tail. **StartTop ring selection SOLVED statically: `STARTTOP_SEL_OFFSET = 0x4e4`** (int32) — found via the
`In_Curs` UFunction impl `FUN_1417bb1c0`: `idx=*(int*)(this+0x4e4); item=UIStartTopList[idx]` bounds-
checked vs count, where UIStartTopList = reflected TArray at 0x428(data)/0x430(count). **Pause selection
SOLVED: `PAUSE_SEL_OFFSET = 0x43C`** (int32, steps 0→1→2→wrap) — pinned at RUNTIME with the F4 dev tool
(static scan failed: 0x3a8 too common, 540 false hits). `screen_pause.lua` now reads it via
`Mem.i32(pause,0x43C)`, maps index→row through the reflected `ListBarArray`, announces per move (fallback =
old whole-menu read). **BOTH original Lua dead ends now closed.** Ghidra scripts + decompiled output kept in
`code/ghidra/` + `code/decompiled/`.

**FIELD/OVERWORLD MAIN MENU (Start_Top_C) FULLY ACCESSIBILIZED (2026-07-02, WORKING).** It's a vertical
list (Items/Party/Characters/Story/Community/Dragon Balls/System/DLC), image-only items + a FIXED "Main
Menu" header (so no readable text). `screen_field.lua`: idx = `Mem.i32(top,0x4e4)`; item =
`top.UIStartTopList[idx+1]` (reflected); **each item's entry id = `Mem.u8(item, 0x404)`** — a
**`START_TOP_LIST_ID`** (ITEM=2,PARTY=3,QUEST/Story=4,CHARACTER=5,SYSTEM=6,COMMUNITY=0,DRAGONBALL=1,
COMMUNITY_BOARD=7,COMMUNITY_EMBLEM=8,SYSTEM_SAVE=9,LOAD=10,OPTION=11,TUTORIAL=12,TITLE=13,DLC=14), NOT
EXCmnHeaderFontType. Ghidra: `FUN_1416bca00` returns `*(byte*)(item+0x404)`. Mapped by new `I18n.startlist`.
LESSON: 0x404's enum was verified against a user SCREENSHOT (I can read screenshots at
C:\Users\ali-b\Pictures\Screenshots newest) — faster than guessing. **Reusable `Mem.fstring(obj,off)`** added
(reads non-reflected UE FString: ptr@off + count@off+8, UTF-16LE→UTF-8). NEXT: the 8 submenus (Items/Party/
Characters/Story/Community/DragonBalls/System sub-screens) still need adapters — many likely reuse the
reflected MenuListBase00/01/03 GetSelectValue reader (already in ui_archetypes as A.list_selected_row, used by
screen_shop).

**mem_bridge BUILT (2026-07-02):** `src/mem_bridge/mem_bridge.c` + `build.ps1` → `mem_bridge.dll` (90KB)
deployed to `mod/KakarotAccess/Scripts/` **separate from prism_bridge**. Guarded (SEH) in-process reads
`read_i32/u8/i64/ptr/bytes/readable/module_base`; static Lua 5.4.4 same as prism_bridge; export
`luaopen_mem_bridge`. Lua side: `mem.lua` (Mem.i32(obj,off) via obj:GetAddress(), guarded),
`native_offsets.lua` (StartTop 0x4e4 + header 0x400/0x408 confirmed; pause TODO), `dev_memdiff.lua` (F4
runtime tail-diff to find pause offset). `main.lua` Mem.init()s it (survives Ctrl+Shift+R). Reusable
`luac.exe` built at `libs/lua54/luac.exe` (Lua syntax validator). Compile bridges: `pwsh -File
src/mem_bridge/build.ps1`.

**HYBRID MOD DECISION (feasibility in `code/hybrid-mod-feasibility.md`):** keep ALL logic in Lua;
add ONE tiny native helper `mem_bridge.dll` (separate Lua C module, static Lua 5.4.4 like
prism_bridge) exposing only generic reads `read_i32(addr,off)` etc. Lua gets the object address
via `obj:GetAddress()`. **Do NOT put mod/memory code in `prism_bridge.dll` — that stays purely
the screen reader** (explicit user requirement). Offsets live in a Lua data file
(`native_offsets.lua`) so a game patch = re-run Ghidra + edit one file, no recompile. Full
C++-from-scratch mod stays REJECTED. Reads must be SEH-guarded (bad addr must not crash game).


**PAK ASSET EXTRACTION (2026-07-06, the fastest RE path for BLUEPRINT widgets — use
BEFORE any live-probe round-trips):** when a blueprint class isn't in the CXX header
dump, extract its .uasset straight from the pak and read the NAME TABLE offline —
gives every widget node name + the native base class in one shot, no game running.
DBZ Kakarot paks are UNENCRYPTED, zlib-compressed, pak **version 7** (index offset via
footer magic 0x5A6F12E1). ~30-line Python: parse footer -> index (FString mount +
count + entries: name, offset q, size q, usize q, cmeth i, sha20, [nblocks + q,q per
block] flag b, cbs i); data sits after a duplicate entry-header at the file offset;
zlib.decompress per block (block offsets are ABSOLUTE in this pak). Then parse the
uasset summary (tag 0x9E2A83C1, legacy=-4 -> no UE3 field, skip custom versions ->
TotalHeaderSize, FolderName FString, PackageFlags, NameCount/NameOffset) and read
NameCount FStrings (+4 bytes hashes each). Used to crack Start_Item_Customize_C
(= AT_UIItemPaletteCustomize; rows Start_Item_Bar_C = AT_UIItemPaletteBar with
Img_Win01_Curs00 cursor) after live reflection/pool sweeps kept aborting.
ALSO LEARNED: on this game even ForEachProperty value reads and FindAllOf("Image")
pool sweeps abort uncatchably — class-reflection of NAMES is safe, VALUE reads only
for members confirmed declared (header dump or pak asset).
