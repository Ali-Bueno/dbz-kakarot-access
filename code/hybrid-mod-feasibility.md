# Hybrid Lua mod vs. C++-from-scratch — feasibility

**Question posed:** can the accessibility mod stay a Lua (UE4SS) mod with a small native
assist, or does it need a full C++ mod written from scratch? And where does that native
code go? (Constraint from the user: **it must NOT live in `prism_bridge.dll`** — that DLL
stays purely the screen-reader speech sink; no mod logic in it.)

## TL;DR

**A hybrid is the right answer: keep 100% of the accessibility logic in Lua, and add a
*separate* tiny native helper DLL that only does generic memory reads.** A full C++ mod from
scratch is unnecessary and was already rejected as heavy/fragile — ~95% of the UI is already
read from Lua today (title, options, dialogue, subtitles, tips, loading, keyhelp, choice/
confirm dialogs, combat HUD text, difficulty, controller-binding resolution…). Only **two**
surfaces are unreachable from Lua, and both fail for the *same* reason, which native code
solves in one stroke.

## Why Lua alone can't finish the job (the two dead ends)

Both confirmed definitively this session (see the project memory + `reference/dbz-kakarot/
ui-map-and-roadmap.md`):

1. **Battle-pause per-item selection** (`UAT_UIXCmnPause` / rows `UAT_UIGameoverBar`).
2. **Field / overworld Start menu section** (`UAT_UIStartTop`, header `UAT_UIXCmnHeader`).

Root cause in both cases:

- The **currently-selected index is a plain C++ member, NOT a `UPROPERTY`**, so UE reflection
  (and therefore UE4SS Lua, which can only see reflected members) cannot read it. The CXX
  header dump proves this: every one of these classes has a **gap of unlisted bytes after its
  last reflected member** — exactly where a private `int32 m_currentIndex`-style field sits:

  | Class | Reflected members end | Class size | Hidden tail (where the index lives) |
  |---|---|---|---|
  | `UAT_UIXCmnPause` | 0x438 | 0x500 | **0x438 – 0x500** (200 bytes) |
  | `UAT_UIXCmnHeader` | 0x400 | 0x510 | **0x400 – 0x510** (272 bytes) |
  | `UAT_UIStartTop` | 0x4D8 | 0x600 | **0x4D8 – 0x600** (296 bytes) |
  | `UAT_UIGameover` | 0x3E4 | 0x460 | 0x3E4 – 0x460 (has reflected `CurrentSelectIndex@0x3E0`) |

  (`UAT_UIGameover` is the useful contrast: its selection *is* reflected at `0x3E0` — that's
  why the game-over screen would be trivially readable — whereas the pause/field variants keep
  it private.)

- **Menu navigation is driven C++-directly, not through `ProcessEvent`/UFunctions**, so a UE4SS
  Lua `RegisterHook` can't intercept the cursor move either (live test fired `SetFontType`,
  `RootMenu:OnSelectItem`, `CheckHovered` = all reached the hook only on *open*, never per nav
  step). And reading the header/ring **images** raises a C++ exception Lua `pcall` cannot catch.

So the missing capability is narrow and identical for both: **read an `int32` at
`objectBaseAddress + offset`** where `offset` lands in that hidden tail.

## The minimal native capability that closes both

UE4SS Lua already exposes an object's address: `obj:GetAddress()`. So the *only* new primitive
needed is a native memory read. A ~50-line C module gives us:

```
read_i32(addr, offset)      -> integer
read_u8 / read_i64 / read_ptr(addr, offset)
read_bytes(addr, offset, n) -> string   (for the rare struct peek)
```

That's it. With `read_i32(pause:GetAddress(), PAUSE_SEL_OFFSET)` the pause reader announces the
selected row like any normal menu; with `read_i32(header:GetAddress(), HEADER_FONT_OFFSET)` (or
the StartTop ring index) the field menu becomes readable. **No mod logic in the DLL** — it only
moves bytes; the diff-gating, i18n, menus.md behavior all stay in Lua.

### Where the offsets come from — the Ghidra pass (this task)

`code/structs/ui_target_layouts.h` already brackets *where* to look (the tail ranges above).
The decompiled cursor-move / `CheckHovered` / `SetFontType` methods (exported to
`code/decompiled/`) show the exact `*(int*)(this + 0xNNN) = newIndex` write. Those `0xNNN`
become the Lua-side offset constants. (Filled in `## Confirmed offsets` below once the Ghidra
export completes.)

## Architecture — keep it OFF the PRISM bridge

Per the constraint, the native assist is a **second, independent Lua C module**, mirroring the
proven `prism_bridge` ABI trick but with a different job:

```
Scripts/
  prism_bridge.dll   <- UNCHANGED. Screen reader only (PRISM speak/stop/rate…). No mod code.
  mem_bridge.dll     <- NEW. Generic process-memory reads only. No accessibility logic.
```

- `mem_bridge.dll` **statically links its own Lua 5.4.4** (UE4SS's exact version → matching
  `lua_State` ABI, same reason prism_bridge does) and exports `luaopen_mem_bridge`.
- It links against nothing game-specific: reads are just `*(int32*)(addr+off)` on our own
  process (the game), so no `ReadProcessMemory` even — a direct deref is enough (in-process).
  Guard with a structured-exception wrapper so a bad address can't hard-crash the game.
- Lua side: a new `mem.lua` wraps it (`mem.i32(obj, off)` with `IsValid` + address guard),
  and `screen_pause.lua` / a new `screen_field.lua` (revive the dormant `header_reader.lua`)
  consume it. Offsets live in **one Lua table** (`Scripts/native_offsets.lua`) so a game patch
  = re-run Ghidra, edit one file, hot-reload — no recompile.

This preserves everything the project already values: PRISM bridge untouched and single-purpose,
all logic hot-reloadable in Lua, and the fragile native surface reduced to a handful of offsets
in a data file rather than a compiled mod.

## Why NOT a full C++ mod from scratch

- It would duplicate the ~10 working Lua readers in C++ for no benefit.
- It loses Lua hot-reload (Ctrl+Shift+R), the project's core dev-loop advantage.
- It's *more* fragile, not less: a full native mod hard-binds to many offsets/vtables; the
  hybrid binds to ~2–3 offsets, all isolated in a data file and all fail-safe (a wrong offset
  yields a guarded bad read → fall back to the current "read whole menu on entry" behavior,
  never a crash).
- The heavy route (RE-UE4SS source build / UEPseudo / external process) was already dropped
  earlier in the project for exactly these reasons.

## Residual limits native reads do NOT fix

- **Keyboard combat/exploration key labels** stay unrecoverable (the game's proprietary
  CFramework input system keeps no key field and UE's ActionMappings are empty) — unrelated to
  selection indices; a native read of an index won't produce a keyboard glyph. Unchanged.
- If a future game patch reorders the class, the offset shifts; mitigated by the data-file
  design + this Ghidra workflow being repeatable.

## Ghidra findings (what the decompilation actually showed)

### The binary has NO C++ RTTI for the game's own classes
The 901 `.?AV` RTTI type descriptors in the exe are **all third-party** (ICU, OpenEXR `Imf`,
`Iex`, libwebm `mkvparser`, …). Grep proves `AT_UIXCmnPause` / `XCmnPause` appear **0 times as
ASCII** and only as **UTF-16** reflection strings (`utf16=4, ascii=0`). This is standard UE4
(`/GR-`: C++ RTTI off, UE's own reflection used instead). Consequences:
- Ghidra's MSVC RTTI analyzer does not even register for this binary; forcing it recovered 536
  vftables but **none** for the UObject UI classes (all third-party). So there is **no automatic
  class→vtable→method map** for the game's UI.
- The reusable way in is **UE's native-function registration table**: each UFunction is a
  `{ const char* Name, FNativeFuncPtr exec }` pair. Find the ASCII name (`SetFontType`,
  `GetSelectValue`, …) → the `exec` thunk → the native impl. Script: `ghidra/find_ufunc.java`
  (+ `decompile_addrs.java`). This **works** and is how the results below were obtained.

### `UAT_UIXCmnHeader::SetFontType(FontType, IconType, bAutoStart)` — `FUN_1416f7bd0`
The field-menu section setter. Confirmed:
- **`FontType` (1st arg) IS the section id** — it indexes a ~59-entry table of `Lang_*` labels
  (`Lang_MainMenu`, `Lang_World`, `Lang_Char`, `Lang_Shop`, `Lang_Quest`, …) that name each
  header section. So the overworld menu section is fully identifiable.
- It does **not** keep the raw index as a scalar; instead it writes into the **hidden tail**:
  - `*(u8)(this + 0x400)`   = a **mode flag** (0 = normal, 1 = "Commu" board mode).
  - resolved header **label FString** at `this + 0x408` (mode 0) **or** `this + 0x460` (mode 1).
  - extra flags at `this + 0x1ac` (bit3 = bAutoStart) and `this + 0x401`.
- ⇒ For the field-menu header, the hybrid reader can read the **FString at `this+0x408`**
  (gated by the mode byte at `this+0x400`) to get the active section label — a hidden-tail
  member, exactly matching the CXX-dump gap (`UAT_UIXCmnHeader` reflected ends 0x400, size 0x510).

### `GetSelectValue` (MenuListBase00/01/03) — native impls located
`GetSelectValue` resolves to native impls (e.g. `FUN_1415bcbc0`/`FUN_1415bcbd0`). This is the
already-Lua-reflected generic list index — the native side just confirms the offset it returns.

### StartTop ring selection — SOLVED statically ✅
Found via the StartTop cursor UFunction **`In_Curs`** (native impl `FUN_1417bb1c0`), which reads:
```c
idx = *(int*)(this + 0x4e4);                      // the selected ring index
if (0 <= idx && idx < *(int*)(this + 0x430)       // count of UIStartTopList
    && (item = ((void**)*(void**)(this+0x428))[idx]))   // UIStartTopList[idx]
    ...animate the selected item...
```
`0x428`/`0x430` are exactly the reflected `UAT_UIStartTop.UIStartTopList` TArray (data/count from
the CXX dump), so **`STARTTOP_SEL_OFFSET = 0x4e4`** (int32, in the hidden tail 0x4D8–0x600) is the
overworld-menu selection. → `read_i32(startTop:GetAddress(), 0x4e4)`.

### Battle-pause selection — SOLVED at runtime ✅
Static isolation failed (nav is C++-direct — no UFunction; and `0x3a8` is too common a scalar to
filter on, 540 false hits). So it was pinned with the **F4 runtime-diff tool**: reading the
`UAT_UIXCmnPause` tail while moving the pause cursor showed the int32 at **`0x43C`** step
0→1→2 and wrap (3 rows). ⇒ **`PAUSE_SEL_OFFSET = 0x43C`**. `screen_pause.lua` now reads it
(`Mem.i32(pause, 0x43C)`) and maps index→row through the reflected `ListBarArray`, announcing the
selected row per move (falls back to the old whole-menu read if the native read is unavailable).

## Recommended way to finish the offsets: runtime diffing via `mem_bridge`
Given no RTTI and C++-direct nav, the **robust, low-effort** way to get the remaining selection
offsets is the mem_bridge itself: with the game running, `read_bytes(obj:GetAddress()+0x438, N)`
over the pause tail (and `+0x4D8` for StartTop) **while moving the cursor**, and watch which
`int32` steps 0→1→2. That directly yields the offset, is resilient to how the value is written,
and needs no further static RE. Ghidra already gave us the exact **ranges** to scan and one solid
static hit (the header). So:
- `STARTTOP_SEL_OFFSET` = **0x4e4** (int32) — `UAT_UIStartTop` ring selection. **Static-confirmed.**
- `HEADER` (field menu section): mode byte `this+0x400`; label FString `this+0x408`/`0x460`
  (from `SetFontType` / `FUN_1416f7bd0`). **Static-confirmed.**
- `PAUSE_SEL_OFFSET` = **0x43C** (int32) — `UAT_UIXCmnPause` selected row. **Runtime-confirmed (F4).**

## Implementation status (native side is BUILT)
- **`src/mem_bridge/mem_bridge.c` + `build.ps1`** → `mod/KakarotAccess/Scripts/mem_bridge.dll`
  (built & deployed, **separate** from prism_bridge). Guarded reads (SEH): `read_i32/u8/ptr/bytes/…`,
  `readable`, `module_base`. Holds no mod logic.
- **`Scripts/mem.lua`** — wrapper (`Mem.i32(obj,off)` via `obj:GetAddress()`, all guarded).
- **`Scripts/native_offsets.lua`** — all offsets in one data file (StartTop 0x4e4, pause 0x43C,
  header 0x400/0x408 — all confirmed).
- **`Scripts/dev_memdiff.lua`** (F4) — runtime diff tool (used to pin pause 0x43C).
- **`Scripts/screen_pause.lua`** — now announces the selected pause row via `Mem.i32(pause,0x43C)`.
- `main.lua` now `Mem.init()`s the bridge (survives Ctrl+Shift+R, like the speech bridge).

Both original Lua dead ends are now closed. Remaining polish: consume `startTop.selectedIndex`
in the field-menu reader (revive `header_reader.lua`) so the overworld ring menu speaks too.
