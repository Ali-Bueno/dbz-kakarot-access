# UE4SS built-in discovery tools (dumpers + Live View)

**Use these BEFORE writing custom reflection code.** UE4SS ships tools that map a game's classes,
properties and functions for you — and, crucially, let you **watch which property changes as you
navigate a menu**. Reaching for a hand-written `discover.lua` (FindAllOf + ForEachProperty dumps) should
be the *last* resort, not the first: it's slow to iterate, easy to crash on a stale pointer, and it can't
see what the Live View shows live.

---

## 1. The dumpers (offline class/property maps)

All are triggered by a keybind while the game runs (defaults; rebindable in `Mods/Keybinds/Scripts/main.lua`)
and write next to `UE4SS.dll`. Toggle "load all assets first" per dumper in `UE4SS-settings.ini` so
lazily-loaded UI classes are included.

| Dumper | Keybind | Output | Contains | Config section |
|---|---|---|---|---|
| **Object dump** | `Ctrl+J` | `UE4SS_ObjectDump.txt` | every live UObject: name, class, outer, flags, property offsets | `[ObjectDumper]` |
| **C++ headers (CXX)** | `Ctrl+H` | `CXXHeaderDump/*.hpp` (+ `_enums`) | full class/struct definitions: **every UPROPERTY + UFUNCTION signature**, per Blueprint/base class | `[CXXHeaderGenerator]` |
| **UHT headers** | `Ctrl+Numpad 9` | UHT-compatible `.hpp` | headers for a mirror `.uproject` (C++ modding) | `[UHTHeaderGenerator]` |
| **.usmap** | `Ctrl+Numpad 6` | `.usmap` | property mappings for unversioned-property tools | — |
| **.umap actor CSV** | `Ctrl+Numpad 7` | `ue4ss_static_mesh_data.csv` | actor transforms for in-editor level rebuild | — |

### Which one to use for UI work
- **`Ctrl+H` (CXX headers) is the authoritative source for a widget's structure.** Want to know if
  `List_Bar_C` has a `bSelected`/index property, or what nodes a tutorial row exposes? Open its `.hpp` and
  read the real UPROPERTY/UFUNCTION list — no guessing, no runtime reflection. Set
  `[CXXHeaderGenerator] LoadAllAssetsBeforeGeneratingCXXHeaders = 1` first so every menu's classes are
  present.
- **`Ctrl+J` (object dump)** shows what's actually instantiated at that moment — handy to learn the live
  container names (e.g. `…BP_ATGameInstance_C_0.Xcmn_Pause_C_0`).

> If a class truly has **no** selection/index/bool property in its CXX header, that state is not in
> reflection (it's material/animation/native-driven) — stop hunting via dumps and use the Live View to
> confirm, then fall back to a behavioural read (read the whole menu, or track input).

---

## 2. Live Property Viewer (the GUI) — the fast path

The GUI console's **Live View** lets you search, view, edit and **watch** the properties of every object
in real time. This is the single biggest time-saver for UI discovery.

### Enable it
```ini
[Debug]
ConsoleEnabled     = 1
GuiConsoleEnabled  = 1
GuiConsoleVisible  = 1
GraphicsAPI        = opengl   ; if the window is blank/white, try dx11 (or d3d11)
GuiConsoleFontScaling = 1     ; bump if the text is tiny
```
No mod is required — the console + Live View are core UE4SS. (You can keep them **off** for shipping/play
and only turn them on for a discovery session.)

### Find an object
Right-click the search bar for filters:
- **Instances only** — live actors/widgets/components with real-time values (what you usually want).
- **Non-instances / CDOs only** — loaded defaults, read-only.
- **Regex**, **class-name filter**, **property-name/type filter** — narrow to e.g. `Pause`, `Xcmn_Win`,
  `Battle_Tips`.

Expand an instance to walk its children (WidgetTree → panels → nodes).

### Watch a property change live — THE technique for "which field is the selection?"
1. Open the menu in-game, find its container instance in Live View (e.g. `Xcmn_Pause_C_0`).
2. Right-click a candidate property (or a child row) → **Add watch**.
3. Open the **Watches** tab; expand values with the `+` buttons.
4. **Move the cursor in-game and watch which value flips.** That's your selection signal — no colour, no
   guessing. If nothing in reflection flips, the highlight is material/animation-driven (read the menu
   behaviourally instead).
5. Watches can **write to a file on exit** and persist to `watches/watches.meta.json`.

### Call any function
The **"Find functions"** button opens a search over the object's UFUNCTIONs and lets you **call them**
in-game with no code — great for confirming what a setter/among does before you hook it.

### Limitation
The viewer only shows **reflected** data. Custom-engine primitives (e.g. CyberConnect2's
`CFUIMultiLineTextBox`) may hide their real fields (a text colour that reads as `-`), and material/native
state won't appear.

---

## 3. Recommended discovery flow (replaces most custom dumping)

1. Enable the GUI console (settings above), launch.
2. **`Ctrl+H`** with `LoadAllAssets…=1` → read the target class's `.hpp` for its real property/function list.
3. **Live View** → find the live instance, **add watches**, navigate the menu, see what changes.
4. Only if reflection can't express the state, fall back to a Lua read (behavioural) — and if you must
   write a throwaway dumper, guard **every** deref with `IsValid` and avoid `RenderTransform`/nested-struct
   reads (they can abort uncatchably; see [accessibility-patterns](accessibility-patterns.md)).
5. Iterate with **hot reload** (`Ctrl+R`).

---

## 4. Reading state that reflection CANNOT see (two proven techniques)

Some menu state is a **non-`UPROPERTY` C++ member** — a private selection/page/window index the game never
exposed to reflection. It's absent from the object dump and the Live View, and the CXX header shows it only
as an unnamed tail gap. Two techniques recover it. Both were used to accessibilize DBZ Kakarot menus that
were otherwise dead ends (battle pause, overworld menu, party slots, the Tips book).

### 4a. Native memory diff — find a hidden index by watching raw memory (the "F4 probe")
When Live View shows nothing flipping as you navigate, the index still lives in the object's memory tail.
Find its offset by **diffing raw bytes as you move the cursor** — the memory-level equivalent of a Live
View watch:

1. A tiny **separate** in-process Lua C module (`mem_bridge.dll`) does **SEH-guarded** reads
   (`read_i32/u8/ptr/bytes` at `obj:GetAddress() + off`; a bad address returns `nil`, never crashes). Keep
   it apart from the screen-reader bridge.
2. A dev keybind (we use **F4**) snapshots a broad tail window (e.g. `0x398..0x518`) of every candidate
   on-screen object, and on the next press reports each `int32` that changed to a small value
   (`0 ≤ v < 64`) — filtering out pointers/floats. Open a menu → press (baseline) → move the cursor →
   press again: the offset whose value **steps `0→1→2…` and wraps** is the selection/page index.
3. Read it at runtime via the same bridge and use it directly. A game patch → re-run the probe, edit one
   offsets data file (no recompile).

**Confirmed finds (DBZ Kakarot):** battle-pause row (`UAT_UIXCmnPause+0x43C`), overworld ring index
(`UAT_UIStartTop+0x4E4`) + submenu depth flag (`+0x4DC`) + sub-cursor (`+0x4EC`), Tips book front-window
index (`UAT_UITips+0x420`) + current-page index (`+0x424`). Each took ONE probe session. **This is the
go-to whenever a "current selection / page / tab / window" isn't reflected** — faster and more reliable
than any highlight-image heuristic. (For a purely static offset — e.g. a total-page count that doesn't
change as you navigate — the diff won't catch it; read it from the CXX header gap instead.)

### 4b. Fixed-array UPROPERTY collapse → `RegisterCustomProperty`
A widget member declared as a **fixed C array** — `SomeType* Name; // (size: 0xNN)` where `NN > 8` (so it's
`Name[NN/8]` inline pointers, not a `TArray`, whose size is `0x10`) — is **collapsed by UE4SS reflection to
element 0 only**. `host.Name` yields element 0; `host.Name[i]` yields garbage/nil; and array methods
(`:GetArrayNum()` / `#`) **abort uncatchably**. This hides every row of a multi-slot list (party slots,
standby lists, character bars…).

Recover the other elements with UE4SS's own **`RegisterCustomProperty`** — register each hidden pointer at
`base + i*8` as an `ObjectProperty`, and UE4SS then exposes it as a normal reflected property:
```lua
RegisterCustomProperty({
    ["Name"] = "Slot1",
    ["Type"] = PropertyTypes.ObjectProperty,
    ["BelongsToClass"] = "/Game/…/Start_Party_List.Start_Party_List_C",  -- runtime blueprint class path
    ["OffsetInternal"] = 0x3A0,                                          -- base (0x398) + 1*8
})
-- then: host.Slot1  ->  the element-1 UObject, read normally
```
Register lazily **once** (guarded) when the screen first appears. Get `BelongsToClass` from a live instance
(`obj:GetClass():GetFullName()` minus the `WidgetBlueprintGeneratedClass ` prefix), and confirm the offset
with a one-shot probe: `Mem.ptr(host, base)` must equal `Mem.addr(host.Name)` (element 0), and
`base + i*8` are the sibling pointers. **This unlocks any fixed-array row menu that reflection flattens.**

### Related helper — pooled instances whose active copy alternates
Unrelated to the above but a common "found but silent" cause: some screens have **several pooled instances**
(`Start_Char_C_3`/`_4`) and only one is on-screen at a time, alternating. Caching a single instance
(`cached_live`) locks onto one and goes mute when the other is live. Pick the on-screen one each tick
instead (`Core.first_on_screen` = `cached_all` + `on_screen` filter).
