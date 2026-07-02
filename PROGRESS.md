# DBZ Kakarot Accessibility — Progress & Next Steps

_Session handoff, last updated 2026-07-02._

This is the "where did we leave off" doc. It captures what works, how the native side was
built, the key lessons, and exactly what to do next.

---

## TL;DR of this session

The project had **two long-standing dead ends** that pure-Lua/UE4SS reflection could not read
(the selected index was a private C++ member, invisible to reflection). Both are now **SOLVED**,
plus the whole overworld main menu:

- ✅ **Battle-pause menu** — announces the selected row per move.
- ✅ **Overworld main menu** (Items / Party / Characters / Story / Community / Dragon Balls /
  System / DLC) — announces each entry, in the game's language.
- ✅ Built a small **native memory-reader** (`mem_bridge.dll`) that reads the non-reflected
  members, kept **completely separate** from the screen-reader bridge (`prism_bridge.dll`).
- ✅ Decompiled the game with Ghidra to find the exact offsets (the `code/` folder).

**Next up: the 8 submenus** behind those main-menu entries. Starting with **System** (Save /
Load / Options / Tutorial / Title) so we can load saves and return to the menu while testing.

---

## How the native side works (the important new capability)

UE4 shipping builds have **no C++ RTTI for the game's own classes** and some menu state lives in
**non-`UPROPERTY` C++ members** that UE reflection (and therefore UE4SS Lua) cannot see. To read
those, we added a tiny native helper:

- **`mem_bridge.dll`** (`src/mem_bridge/mem_bridge.c`, built by `src/mem_bridge/build.ps1`) — an
  in-process Lua C module exposing **guarded** memory reads (`read_i32/u8/i64/ptr/bytes`,
  `readable`, `module_base`). Reads are wrapped in SEH, so a bad address returns `nil` instead of
  crashing the game. **It holds NO mod logic** — only raw reads. **Deliberately separate from
  `prism_bridge.dll`**, which stays the screen reader only.
- **`Scripts/mem.lua`** — Lua wrapper: `Mem.i32(obj, off)` etc. take a live UObject and read at
  `obj:GetAddress() + off`, all guarded. Also `Mem.fstring(obj, off)` reads a non-reflected UE
  `FString` (ptr@off + count@off+8, UTF-16LE → UTF-8).
- **`Scripts/native_offsets.lua`** — every native offset in ONE data file, so a game patch =
  re-run the Ghidra workflow and edit this file (no recompile).

### Confirmed native offsets (all in `native_offsets.lua`)

| What | Class | Offset | Notes |
|---|---|---|---|
| Battle-pause selected row | `UAT_UIXCmnPause` | **`0x43C`** int32 | steps 0→1→2, wraps |
| Overworld ring/list index | `UAT_UIStartTop` | **`0x4E4`** int32 | index into `UIStartTopList` |
| Overworld item's entry id | `UAT_UIStartTopList` | **`0x404`** u8 | a `START_TOP_LIST_ID` enum |
| Field header mode / label | `UAT_UIXCmnHeader` | `0x400` / `0x428` | header is a FIXED "Main Menu" title |

**`START_TOP_LIST_ID`** (the item's `0x404` byte, mapped by `I18n.startlist`):
`COMMUNITY=0, DRAGONBALL=1, ITEM=2, PARTY=3, QUEST(Story)=4, CHARACTER=5, SYSTEM=6,
COMMUNITY_BOARD=7, COMMUNITY_EMBLEM=8, SYSTEM_SAVE=9, SYSTEM_LOAD=10, SYSTEM_OPTION=11,
SYSTEM_TUTORIAL=12, SYSTEM_TITLE=13, DLC=14`.

---

## The Ghidra decompilation workflow (in `code/`)

Full detail in `code/README.md` + `code/hybrid-mod-feasibility.md`. Key facts learned:

1. **The exe is DRM-encrypted (SteamStub v3.1).** `.text` is 100% encrypted on disk; the entry
   point is in a `.bind` section. **Unpack first with Steamless** (installed at
   `D:\code\decompilers\Steamless_v3.1.0.5`) → `code/AT-Win64-Shipping.exe.unpacked.exe`. That is
   the file Ghidra analyzes.
2. **Ghidra 12** project at `code/ghidra/KakarotAT` (16 GB-heap wrapper
   `analyzeHeadless_kakarot.bat`). Two gotchas: the **"Embedded Media" analyzer crashes the JVM**
   on a malformed embedded PNG → disabled via the `disable_media.java` pre-script; and **Ghidra 12
   dropped Jython**, so all scripts are **Java**, not Python.
3. **No RTTI for game classes** → no automatic class→vtable map. The way in is **UE's native
   function-registration table** (`{const char* Name, FNativeFuncPtr exec}` pairs):
   `code/ghidra/find_ufunc.java` finds a UFunction's native impl by its ASCII name;
   `decompile_addrs.java` dumps any address. That's how the offsets above were found (e.g.
   `In_Curs` impl reads `this+0x4E4`; `FUN_1416bca00` returns `item+0x404`).

**To re-open / query the analyzed program** (no re-analysis):
```
D:\code\decompilers\ghidra_12.0_PUBLIC\support\analyzeHeadless_kakarot.bat ^
  "…\code\ghidra" KakarotAT -process "AT-Win64-Shipping.exe.unpacked.exe" -noanalysis ^
  -scriptPath "…\code\ghidra" -postScript <script>.java -log <log> -scriptlog <log>
```

---

## Dev tools (for mapping new screens)

- **F4 — universal memory probe** (`dev_memdiff.lua`): open any menu, tap F4 (baseline), move the
  cursor, tap F4 again. It reports `Class +0xNNN = value` for whatever int32 changed — this is how
  we pinned the pause (0x43C) and the overworld selection. Results are also appended to
  **`Scripts/dev_probe.txt`** (git-ignored) so they can be read directly, no relaying by voice.
- **F7 — reflection discovery** (`discover.lua`): dumps an on-screen widget's structure to
  `Scripts/dumps/` for mapping.
- **Screenshots**: pasting a screenshot to `C:\Users\ali-b\Pictures\Screenshots` (newest) is often
  the fastest way to establish ground truth — that's how the overworld menu's `START_TOP_LIST_ID`
  mapping was verified.
- **`libs/lua54/luac.exe`** — Lua 5.4.4 syntax validator; run `luac -p <file>` before reloading.
- **Ctrl+Shift+R** in-game reloads the whole mod's logic (feature adapters + i18n). `main.lua`
  (keybinds, `Mem.init`, `Speech.init`) needs a full game restart.

---

## Screens accessibilized so far (adapters in `Scripts/`)

Registered in `app.lua` (order matters — overlays first):
`screen_choicelist` (difficulty), `screen_dialog` (message/confirm popups), `screen_pause`
(battle pause — **native selection**), `screen_dialogue` (subtitles / NPC talk), `screen_tips`,
`screen_loading`, `screen_tutorial`, `screen_options`, `screen_shop`, **`screen_field`**
(overworld main menu — **native selection**), `screen_title`. Plus `keyhelp` (F2 button bar).

---

## What to do next

### 1. The 8 overworld submenus (priority — start with System)
Each main-menu entry opens its own screen. Plan per submenu: open it → check if the reader says
anything → grab a screenshot and/or an F4 probe → build a `screen_<name>.lua` adapter → test.

- **System first** (Save / Load / Options / Tutorial / Title). Likely uses the StartTop sub-list
  **`UIStartTopList06_Sub`** (System is main index 6) with items carrying the same `0x404`
  `START_TOP_LIST_ID` byte — and **those names (9–13) are already in `I18n.startlist`**, so it
  should be quick once we know the sub-selection offset (get it with an F4 probe inside System).
- Then **Items, Party, Characters, Story, Community, Dragon Balls**. Many likely reuse the
  **reflected `MenuListBase00/01/03` list** (`GetSelectValue` → `ListPlateCtn[idx].TxtName`), for
  which the mod already has a reader (`A.list_selected_row` in `ui_archetypes.lua`, used by
  `screen_shop`) — so a generic list adapter may cover several at once.

### 2. Loose ends from today
- **Tips multi-page**: the page-turn state is noisy to probe (two windows, heavy per-page churn).
  Best pinned with a narrowed F4 probe on the on-screen Tips window only.
- **"View Controls"** (from battle pause): reads jumbled and the pause doesn't re-announce after
  closing it. Need to identify that widget (F7 while it's open) — it's likely a Tips/controls
  window, and it opens *over* the pause (pause index unchanged, so no re-announce on return).

### 3. Housekeeping
- The header hook (`header_hook.lua`, F6) is now superseded by the native field reader — can be
  retired once the submenus are done.
- After a game patch, re-verify the offsets in `native_offsets.lua` via the F4 probe.

---

## Key lessons (so we don't relearn them)

- **UE4 game classes have no RTTI**; class names exist only as UTF-16 reflection strings. Reach
  native methods via the UE function-registration table, not vtables.
- **Non-`UPROPERTY` members** (selection indices, page ids) sit in the object's tail past the last
  reflected member — find the range from the CXX header dump, pin the exact offset with the F4
  runtime probe (fast) or Ghidra (authoritative).
- **A user screenshot beats guessing** for UI ground truth (I can read them).
- **Enums matter**: the overworld item byte was `START_TOP_LIST_ID`, not `EXCmnHeaderFontType` —
  always confirm the enum in the CXX dump (`AT_enums.hpp`) rather than assuming.
- Keep the native surface tiny and data-driven (`native_offsets.lua`) so patches are cheap.
