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
