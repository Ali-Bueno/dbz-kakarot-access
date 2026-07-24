# dbz-kakarot-accessibility-plan

> DBZ Kakarot accessibility mod — chosen architecture (UE4SS Lua C module bridge), screen reader, build setup


Accessibility mod for DBZ Kakarot (engine facts in [dbz-kakarot-engine-facts](dbz-kakarot-engine-facts.md)).

**Screen reader: PRISM** (not Tolk — the UE4SS study's Tolk template was an old draft). Prebuilt release v0.16.7 (windows-x64), kept in the project at `libs/prism/` (`bin/prism.dll` + `tolk.dll`, `include/prism.h`, `lib/prism.lib`). Loaded via dynamic `LoadLibrary`/`GetProcAddress` per the playbook §4.

**Architecture: in-process Lua C module** (decided 2026-06-30, ported from AccessForge/SparkingZeroAccess's `speech_bridge` — confirmed portable because Kakarot and Sparking Zero both run UE4SS with the same bundled Lua 5.4.4):
- `prism_bridge.dll` STATICALLY links its own **Lua 5.4.4** (UE4SS v3.0.1's exact version, so the `lua_State` ABI matches) and DYNAMICALLY loads `prism.dll` at runtime.
- UE4SS does `require("prism_bridge")` → calls `luaopen_prism_bridge` with UE4SS's own `lua_State`; our static Lua manipulates that state.
- ALL accessibility logic lives in **Lua** → hot-reload with Ctrl+R, no game restart.
- **No Rust, no RE-UE4SS source build, no UEPseudo, no external process.** This replaced the earlier "hybrid C++ UE4SS mod + Rust/Corrosion build of RE-UE4SS" plan, which was dropped as fragile/heavy.

**Toolchain present:** MSVC 2022 Community (the build locates it via `vswhere`). Rust was installed earlier for the abandoned plan but is NO LONGER required by this architecture.

**Build (self-contained PowerShell, no CMake):** `pwsh -File src\prism_bridge\build.ps1`:
1. Compiles vendored Lua 5.4.4 (`libs/lua54/src`) → `libs/lua54/lua54.lib` (only if missing).
2. Compiles + links `src/prism_bridge/prism_bridge.c` → `prism_bridge.dll` against lua54.lib + user32.lib.
3. Deploys `prism_bridge.dll` + `prism.dll` + `tolk.dll` into `mod/KakarotAccess/Scripts/`.

**Mod folder** `ue4ss/Mods/KakarotAccess/`: just `Scripts/` (Lua + the 3 runtime DLLs). Enabled in `ue4ss/Mods/mods.txt` AND `mods.json` (`KakarotAccess : 1` / `"mod_enabled": true`). `Scripts/` is a **junction** to the repo (`mod/KakarotAccess/Scripts`) so Lua edits + deployed DLLs are live → Ctrl+R, no restart. build.ps1 deploys DLLs to the repo Scripts; it does NOT create the junction (created manually with `mklink /J`).

**Bridge API exposed to Lua** (module returned by `require("prism_bridge")`): `say(text,interrupt=true)`, `output(text,interrupt)`, `stop()`, `is_speaking()`, `detect()` (backend name or nil), `set_rate/volume/pitch(float)`, `is_ready()`. `speech.lua` wraps it as the single speech sink; `main.lua` tests it (F8 speak, F9 read player position, Ctrl+F8 silence).

UE4SS installed in game `AT\Binaries\Win64\`. **Updated 2026-07-01 from v3.0.1 → experimental-latest build `v3.0.1-1000-gc62716b3`** (2024-12-29; still bundles Lua 5.4.4, so prism_bridge ABI unchanged, no rebuild needed). Reason: v3.0.1 crashed the game deterministically at `SecondsSinceStart=0` inside UE4SS's ProcessLocalScriptFunction hook (memcpy of UFunction params) — no Lua/prism frames, so not our mod; triggered by a demo mod's early UFunction hook. Experimental has ~1000 commits of hook/GUI-crash fixes. **FINAL working UE4SS build (2026-07-01): the one bundled by `DBZK_Fix` — `UE4SS.dll` 19.4 MB, compiled 2024-04-10, patternsleuth, Lua 5.4.4, FLAT layout** (dwmapi.dll + UE4SS.dll + UE4SS-settings.ini + Mods/ directly in `Win64/`, NO `ue4ss/` subfolder). We first tried the official **experimental-latest (Dec 2024)** build: it fixed the crash (with 4.20 override) but then **HUNG at `Locating KismetSystemLibrary CDO...`** and never started the Lua mods (a regression in the newer build for UE4.20/4.21). The April-2024 DBZK build does NOT hang. So: copied DBZK's dwmapi.dll+UE4SS.dll+UE4SS-settings.ini+Mods into `Win64/` (flat), set version override 4.20 + bUseUObjectArrayCache=true (already in their settings), enabled Console/GuiConsole/GuiConsoleVisible=1 for Live View. Their build is Lua 5.4.4 → prism_bridge ABI unchanged. Source: Naitrate/DBZK_Fix release DBZK_Fix_Alpha01.zip. DBZK_Fix gameplay mod itself kept DISABLED (faithful gameplay); we only use its UE4SS core. (Superseded the ue4ss/ subfolder layout from the Dec build.) Old v3.0.1 backed up at `<game>\_ue4ss_backup_v3.0.1\`. Re-applied settings: ConsoleEnabled/GuiConsoleEnabled/GuiConsoleVisible=1, EnableHotReloadSystem=1, GraphicsAPI=opengl. First-boot mods.txt: only KakarotAccess + Console mods enabled; hook-registering demos (CheatManagerEnablerMod, BPModLoader, LineTrace) disabled. No DBZ Kakarot config in zCustomGameConfigs.

**ACTUAL ROOT CAUSE + FIX (confirmed 2026-07-01 via DBZK_Fix community bundle):** UE4SS's AOB scanner could not locate the engine globals on this UE4.21 binary — log: `[PS] Failed to find GUObjectArray` / `Failed to find StaticConstructObject_Internal` — so it operated on misidentified addresses and faulted in a memcpy. **THE FIX is a single load-bearing setting: `[EngineVersionOverride] MajorVersion=4, MinorVersion=20` (override 4.21 → 4.20).** With the version forced to 4.20, patternsleuth resolves the object layout/globals correctly and the crash stops. Also set `bUseUObjectArrayCache=true`. No `UE4SS_Signatures/*.lua` files needed (the known-working DBZK_Fix bundle ships none). Our experimental-latest build is already patternsleuth-based (same family as DBZK_Fix's MinGW build), so only the settings matter — UE4SS version/Lua 5.4.4/prism_bridge all unchanged. Reference: KingKrouch/Naitrate `DBZK_Fix` (Naitrate/DBZK_Fix releases → DBZK_Fix_Alpha01.zip) is the authoritative working config. Once addresses resolve correctly the function-dispatch hooks are safe, so `HookProcessInternal/HookProcessLocalScriptFunction/HookUObjectProcessEvent` were re-enabled (=1) → **Lua RegisterHook IS available again** (supersedes the earlier note about avoiding it). The earlier hypotheses below (ProcessInternal hook / demo mods) were WRONG — kept for history:

~~**ROOT CAUSE of the startup crash (found 2026-07-01): UE4SS's global function-dispatch hook, NOT our mod and NOT the UE4SS version**~~ — experimental crashed identically (same game frame `AT-Win64-Shipping +2d333f1` = ProcessInternal, → UE4SS posthook → param marshaling → `VCRUNTIME memcpy` → crash). UE4SS installs `[UE4SS.ProcessInternal.LuaModImplScriptHook]` globally (to service Lua RegisterHook); on Kakarot's UE4.21 UFunction param layout the marshaling memcpy faults on an early UFunction. This game is generally hostile to UE4SS function hooking — log also shows `Was unable to resolve JMP instruction` / `CallFunctionByNameWithArguments`/`ProcessConsoleExec` failing to install. **KEY GOTCHA: the `[Hooks]` settings flags do NOT prevent the crash — a mod calling `RegisterHook` makes UE4SS install the ProcessInternal detour ON DEMAND regardless of `HookProcessInternal=0`.** The real trigger was the bundled **`ConsoleEnablerMod`**, which calls `RegisterHook("/Script/Engine.PlayerController:ClientRestart")`; that forces the global ProcessInternal detour, which then faults on the first UFunction at world init. **FIX (2026-07-01): disable every demo mod that calls RegisterHook — in mods.txt/mods.json enable ONLY `KakarotAccess` + built-in `Keybinds`; ConsoleEnablerMod AND ConsoleCommandsMod OFF.** (The UE4SS GUI console + Live View are core, `GuiConsoleEnabled=1`, so we keep them without those mods.) Also set the `[Hooks]` flags `HookProcessInternal=0`/`HookProcessLocalScriptFunction=0`/`HookUObjectProcessEvent=0` (belt-and-suspenders; alone they were insufficient). Our mod needs none of them now (only RegisterKeyBind + state reads via FindFirstOf/properties; tick/input hooks like HookEngineTick stay ON for ExecuteInGameThread). **Consequence for later: Lua `RegisterHook` on UFunctions is unavailable on this game** → to announce menu/HUD changes, use tick-polling (read widget state each EngineTick and diff) instead of UFunction hooks; revisit whether ProcessLocalScriptFunction alone can be re-enabled or a game-specific signature override helps.

**Dev flow (playbook §1):** bridge built ✔ → **F8 speech + F9 live-position read CONFIRMED WORKING in-game (2026-07-01).** Config locked/stable (DBZK April build + 4.20 override).

**MAIN MENU (title screen) READER — WORKING 2026-07-01.** Widget model (verified via reflection):
`Gametitle_C` (live instance under `/Engine/Transient`, NOT the `/Game/Art/UI` archetype) →
`.Gametitle_List00 .. .Gametitle_List05` (class `Gametitle_List_C`, one per option) →
`.Txt_Select` / `.Txt_List` (class `Xcmn_MultiLineText_C`) → `.mainTxt` (class `CFUIMultiLineTextBox`)
→ `.Text` (FText; also `GetText()`). **Selected option = the item whose `.Txt_Select` is `IsVisible()==true`**
(`Txt_List` visible on the unselected ones; both hold the same label). Options seen: Nueva partida /
Continuar / DRAGON BALL CARD WARRIORS / Opciones / Tienda / Salir del juego — **read live from the game's
FText, so language-agnostic** (switch game language → mod reads that language, nothing hardcoded).

**Reading approach = tick-poll, NOT RegisterHook** (simpler, and avoids SparkingZero's heavy patterns —
see [sparkingzero-reference-scope](sparkingzero-reference-scope.md)): `LoopAsync(100ms)` → `ExecuteInGameThread(step)`; step navigates
directly by property on a CACHED `Gametitle_C` (re-acquired via `FindAllOf("Gametitle_C")` filtered to
`/Engine/Transient`, only when the cache is invalid, rate-limited), finds the selected index, and speaks
ONLY on change (diff-gated, interrupt=true). **Never iterate huge `FindAllOf` lists** (e.g.
`FindAllOf("Xcmn_MultiLineText_C")` walks hundreds of nodes and HARD-CRASHES the game on a stale one) —
and calling a member fn on a null UObject is an **uncatchable C++ abort pcall can't catch**, so guard every
deref with `IsValid` and navigate from the live widget only.

**Runtime architecture (Scripts/):** `main.lua` = bootstrap loaded once (init PRISM, register keybinds,
snapshot `package.loaded` as "protected" right after speech/bridge). `app.lua` = controller owning a
`features` list (currently `{menu}`; add options/gameplay here). `menu.lua` = title reader (has a
`_G.__KakarotMenuGen` **generation guard** so a leftover poll loop from a prior load stops itself — this
fixed a reload crash caused by two loops racing on a stale Lua state). `speech.lua` = the PRISM sink.
`discover.lua` = **dev-only** reflection tool (ForEachProperty/ForEachFunction + targeted dumps to
`Scripts/probe.txt`/`dump.txt`), NOT wired into runtime; use it to map future screens.

**Keybinds:** F8 speech test, Ctrl+F8 silence, F9 position, F1 menu diagnostic (speaks current selection or
why nothing read), Ctrl+M toggle menu reader, **Ctrl+Shift+R = in-game reload of the WHOLE mod** (stops
features, clears every non-protected module from `package.loaded`, re-requires `app.lua`, restarts; PRISM
bridge untouched). Reload takes effect for all logic; changes to `main.lua`/keybinds still need a restart.

**UE4SS debug windows now HIDDEN** (user pref, 2026-07-01): `ConsoleEnabled=0`, `GuiConsoleEnabled=0`,
`GuiConsoleVisible=0` in `AT\Binaries\Win64\UE4SS-settings.ini`. → **the console Ctrl+R hot-reload is no
longer available**; iterate with the mod's own **Ctrl+Shift+R** instead. `EnableHotReloadSystem=1` still set.

**OPTIONS MENU (Start_Option_C) READER — WORKING 2026-07-01** (feature `options.lua`, in `app.lua`).
Model: rows `Xlist_Bar03_C` (cached pool of 9, recycled on scroll) under `Start_Option_C_0`; selected =
lowest-index row whose `.Ins_Cursor_Fad` is visible. Value source (recycled rows keep STALE text on hidden
nodes, so gate on node visibility): `.Xlist_Bar_03_Gauge` visible → volume slider, level = # visible
`Ins_System_OnNN` of 20 → percentage; else `.Txt_Mode` if its node is visible; else `.Txt_Mode_Scroll` if
visible. Name = `.Txt_List`; tab title = `Start_Option_C.Txt_Title_Steam`; tooltip = fresh
`Xcmn_Keyhelp_C.Txt_Helpmsg_Main` (NOT cached — recreated on context change; read on announce + a short
window for late arrivals). **"Guardar cambios" is a phantom overflow row** at the last slot that DUPLICATES
the last real option's `Txt_List`; detect it by that duplication (last row's Txt_List == the row above's),
NOT by the fad cursor (which flickers during scroll and lingers) and NOT by any index (the menu exposes NO
scalar/index property anywhere — confirmed by reflecting Start_Option_C, the container, rows, scrollbar).
Behaviour per menus.md: vertical → name+value+tooltip; value change → only value; tab change → only tab.

**HYBRID UI ARCHITECTURE — DONE 2026-07-01, committed+pushed** (per
`D:\code\modding projects\reference\ui-accessibility\generic-strategy.md`, now also ported into this repo's
`reference/ui-accessibility/`). **This SUPERSEDES the "Runtime architecture" paragraph above** — `menu.lua`
and `options.lua` were DELETED; new layout in `Scripts/`:
- `ui_core.lua` — helpers (`valid/is_visible/text_of/first_live/phrase`), the shared **Announcer**
  (diff-gated menus.md speech: enter→screen name+item; tab change→tab; name→name+value+tooltip; value→value;
  late tooltip serviced within a window), and the single **poll engine** `Core.loop(step, is_enabled)`
  (generation guard `_G.__KakarotUiGen`).
- `ui_archetypes.lua` — reusable readers keyed on the game's widgets: `Xlist_Bar03` list row
  (`row_name`/`row_value`/`scan_list`/`is_overflow_button`). **row_value reads a value ONLY on ADJUSTABLE
  rows** (gauge visible, or `Xmenu_Arrow01_L/R` visible) — otherwise recycled rows bleed a previous tab's
  stale `Txt_Mode` (e.g. a graphics value onto a button-remap row). Fixed 2026-07-01.
- `ui_registry.lua` — register screen→adapter; one poll picks the first ACTIVE (visible) adapter and drives
  it; on screen switch resets announcers. **Register submenus BEFORE the title** (title stays "visible"
  underneath, so the overlay must win). `toggle()`/`repeat_current()`.
- `screen_title.lua` (Gametitle_C) + `screen_options.lua` (Start_Option_C) — thin adapters: is_active /
  update (classify + delegate) / reset, each with its own Announcer. Title announces mod-added screen name
  "Menú principal"; Options announces its tab title as the screen name on entry.
- `app.lua` registers the two screens + drives the registry. `main.lua` unchanged role (bootstrap+keybinds).

**Keybinds now:** F1 = repeat current item; Ctrl+M = toggle reader; F7 = dev discovery (re-requires
`discover.lua` fresh each press, writes timestamped `Scripts/dumps/dump_<os.time>_NNN.txt` — read newest by
mtime); Ctrl+Shift+R = reload whole mod. F8/Ctrl+F8/F9 = speech/silence/position tests.

**PER-ACTION BUTTON BINDING IN THE MANDO TAB — READABLE via the RICH text KeyConfigId (SOLVED 2026-07-01;
earlier "dead end" note was WRONG).** The binding is NOT in the plain text, NOT a UMG `Image`, NOT an
`Xcmn_Btn_Plat` (only 2 of those exist = the L1/R1 tab-switch prompts). This game's text nodes
(`Xcmn_MultiLineText_C`) have TWO renderers: `mainTxt` (`CFUIMultiLineTextBox`, plain) and **`ExMainTxt`
(`CFExtendRichTextBlock`, rich)**. The mando row shows its button glyph as an INLINE IMAGE in the rich
renderer of `Txt_Mode`, whose markup source carries the authoritative id:
`<inputicon KeyConfigId="KeyConfig_Controller_Btn_B"/>`. Read it via `row.Txt_Mode.ExMainTxt.Text:ToString()`
and extract `KeyConfigId="([^"]+)"`; the token after `Btn_` is the canonical **Xbox-style** button id, the
SAME regardless of the connected device (only the glyph changes: 8BitDo/Xbox shows A/B/X/Y, a DualSense
would show ✕○△□, but the id stays `Btn_B`). Confirmed 9/9 against a screenshot: Examinar=Btn_B, Cancelar=Btn_X,
Saltar=Btn_A, Girar=Btn_Y, Buscar Ki=Btn_LB, Cursor objetivo=Btn_LT, Superacometida=Btn_L3, Ascender=Btn_RB,
Descender=Btn_RT. **The plain `mainTxt` here is STALE recycled-row bleed** (showed "1920x1080"/"FXAA"/"Alto"
— graphics-tab values) → never use it; the rich `ExMainTxt` markup is correct/live. Gate on
`ExMainTxt:IsVisible()` (mando tab shows the rich glyph; other tabs show plain `mainTxt` and leave stale
markup here). NOTE: I can now READ SCREENSHOTS — the user pastes a Windows screenshot and I read the glyphs
directly; far faster than reflection for UI discovery. (Engine `HighResShot` via `pc:ConsoleCommand` /
`ExecuteConsoleCommand` ABORTS uncatchably on this game — console exec is unavailable, matching the
ProcessConsoleExec note — so Lua-side screenshots don't work; use manual Windows captures.)

**COMBAT / EXPLORATION tabs + the KeyConfig resolver (2026-07-01).** Those tabs' markup KeyConfigId is an
ACTION alias (`KeyConfig_Battle_HighBoost`), not a physical button. Resolution goes through the shared icon
asset **`/Game/CFramework/DataAssets/CFTextIconData.CFTextIconData`** → `KeyConfigList` (159 entries, struct
`/Script/CFramework.CFKeyConfigAssign`, array is **1-based**, index 0 aborts). Each entry:
`ConfigName` (StrProperty), `IconName` (current-device glyph, INDEXED e.g. `Btn_2`/`Btn_L3`/`Btn_Key_6` —
NOT descriptive), `DynamicAssignInputId` (action, e.g. `Battle_HighBoost`), `DynamicAssignInputControllerId`
(semantic controller button, e.g. `Controller_Btn_L3`). The `KeyConfig_*` alias entries have EMPTY icon/ctrl
but a dyn id; another entry sharing that dyn id carries the ctrl button. So: id→configToDyn→dynToCtrl→button.
**Controller button = fully recoverable for every action on every tab, device-independent.** **KEYBOARD key =
NOT recoverable**: `IconName` for keyboard is an indexed glyph (`Btn_Key_6`), there's no key field, and UE's
`UPlayerInput.ActionMappings`/`AxisMappings` are EMPTY (count 0) — the game uses a proprietary CFramework
input system; getting the actual key would need native RE of the .exe. So keyboard combat/exploration tabs can
announce the controller-equivalent button but not the literal key.

**Implementation (DONE 2026-07-01, pending in-game verify):** `ui_archetypes.lua`: `A.button_name`
(Btn_ token → "botón X"/friendly, `BUTTON_NAMES` incl. Start/Back/L3/R3/dpad), `A.row_keyconfig` (reads the
rich markup, visibility-gated), a cached KeyConfig resolver (`build_bindings` reads the 159-entry asset once;
`A.clear_binding_cache` called from `screen_options.reset` so a rebind refreshes it), and `A.row_binding`
(direct button id → button_name; action alias → resolve_ctrl → button). `A.row_value` returns the binding
first → `screen_options` speaks "<action>, botón X" on ALL config tabs (mando + combat + exploration), no new
keybind. Also `keyhelp.lua` (F2) reads the bottom help bar (glyph-texture based, separate from this).
DECISION (user, 2026-07-01): on the combat/exploration tabs announce the controller EQUIVALENT LABELED as
"mando: <button>" (so it isn't mistaken for the literal keyboard key); the mando tab stays plain "botón X".
Glyphs render Xbox
(8BitDo in Xbox mode drives PLAT_X) even with a DualSense connected; optional future: PS-name translation
(A→✕,B→○,X→□,Y→△,LB→L1,...). I CAN READ SCREENSHOTS the user pastes — use that for UI discovery.

**KEYHELP BAR READER — DONE 2026-07-01** (`keyhelp.lua`, wired in `app.lua`, keybind **F2**). Reads the
shared bottom help bar `Xcmn_Keyhelp_C` (present in almost every menu + in-game) on demand: index-aligned
`Txt_Keyhelp_01..09` (action label, `Xcmn_MultiLineText_C`) + `Xcmn_Btn_Plat_01..09` (glyph). The glyph
widget `Xcmn_Btn_Plat_C` carries NO semantic button id — only up to 4 `Dmy_Btn_00..03` images (combo
glyphs), so the button is identified by the brush texture token (`Btn_L1`, `Btn_Options`, `Btn_Pad`,
`Btn_Key_Ud`, indexed face buttons `Btn00`/`Btn01`, …) in `/Art/UI/Xcmn/<platform>/`. `BUTTON_NAMES`
centralizes token→word for the CONFIDENT semantic tokens only; the device-dependent indexed face-button
glyphs (`Btn00`/`Btn01`) are intentionally left unmapped → the reader speaks the action label alone rather
than fabricate a physical button (pending: fill face-button names per real device). Announces
"button: action, …", interrupt=true. **NEXT: verify F2 in-game, then extend `BUTTON_NAMES` with the user's
real face-button tokens; consider auto-announce on keyhelp change; continue remaining submenus
(Continuar/load, Tienda, DB Card Warriors, exit confirm) as adapters reusing archetypes.**

**POLL BUSY-GUARD — CRITICAL, applies to EVERY screen (DONE 2026-07-01).** `Core.loop` (ui_core) fires
`LoopAsync(POLL_MS=100ms)` on a worker thread and `ExecuteInGameThread(step)` WITHOUT waiting. When the game
thread is saturated (loading, a `FullScreenMovie_C`, save/DLC checks) the queued steps pile into a BACKLOG
that runs late and in bursts → huge input-to-speech delays (the user hit this navigating a Yes/No popup during
a movie). FIX: a `busy` flag — only queue the next step once the previous finished (wrapped in pcall; logs
errors). This keeps the reader on the CURRENT state, no backlog. **When adding menus, keep per-tick work cheap
and don't reintroduce backlog**: compute state ONCE per tick (cache between is_active→update, as screen_dialog
does) and avoid redundant FindAllOf. This is why new menus won't have those weird delays.

**DIALOG + CONFIRMATION READER (DONE 2026-07-01, `screen_dialog.lua`, registered 2nd after choicelist).**
General popup reader covering two game-wide shared patterns at once: (a) MESSAGE popup = `Xcmn_Win01_C` with a
prompt in `Txt_Detail` (+ `Txt_Title`), e.g. "Select the battle difficulty…", "Skip the movie?"; (b)
CONFIRMATION = same window + Yes/No buttons `Xcmn_Win00_Choice_C` (`Txt_Choice` label; SELECTED = its
`Img_Xwin01_List` highlight visible — same marker as the difficulty list). **Active choices = TOP-LEVEL
`Xcmn_Win00_Choice_C` (full name `BP_ATGameInstance_C_%d.Xcmn_Win00_Choice_C_%d$`)** — ones nested under another
screen are that screen's pooled/stale widgets (the loading pool makes many widgets falsely report visible). All
text run through `A.markup_to_speech` (strips `<span>`, resolves `<inputicon>`). **CHOICE-LIST READER**
(`screen_choicelist.lua`, registered 1st): the New Game "Difficulty Settings" — `Xcmn_Win01_List_C` items
(`Txt_Item` label), selected = `Img_Xwin01_List` visible, title `Xcmn_Win01_C.Txt_Title`, description
`Txt_Help` (live). Register order (app.lua): choicelist > dialog > options > title.

**GENERAL DISCOVERY TOOL (`discover.lua`, F7) — one-shot snapshot** (class summary + all visible text +
safe targeted reflection). CAVEATS learned: `FindAllOf("UserWidget")` is VERY noisy mid-load (hundreds of
pooled widgets report visible=true from every screen ever loaded); and a fully-generic "reflect each widget's
object children" pass ABORTS uncatchably on some children (e.g. Xcmn_Win01_C's) — so only reflect CLASS
properties (safe) and read leaf text/images via FindAllOf, never blindly walk instance children. Prefer
screenshots (user pastes to C:\Users\ali-b\Pictures\Screenshots, newest) for layout; they're faster than
reflection. NEXT MENUS requested: pause menu, tutorial dialogs, startup dialogs ("don't power off while
loading"/"searching for DLC" — likely message popups the dialog reader may already catch; verify).

**HANDOFF / CURRENT STATE (2026-07-01, session restarted here).** Adapters registered in app.lua IN ORDER:
`screen_choicelist` (Xcmn_Win01_List selection lists — difficulty) > `screen_dialog` (Xcmn_Win01_C AND
Xcmn_Win00_C messages + Yes/No confirmations via top-level Xcmn_Win00_Choice_C) > `screen_tutorial`
(Battle_Tips_Tutorial_C) > `screen_options` > `screen_title`. Keybinds: F1 repeat, F2 keyhelp, Ctrl+M toggle,
F7 discover, F8/F9/Ctrl+F8 tests, Ctrl+Shift+R reload. **WORKING (confirmed by user): title, options,
difficulty read fine — poll is healthy, NO crash.** **PENDING USER TEST after a reload:** (a) `screen_tutorial`
was BROKEN (gated on `win.Txt_Title` which is NOT a direct prop of Battle_Tips_Tutorial_C — only `Txt_Show` +
rows `Battle_Tips_List_00..08` are) → REWRITTEN to read every visible CFUIMultiLineTextBox under the container
subtree (robust); should now read "Battle Controls, Basic Controls, 1/2, Guard, Burst, …". (b) `screen_dialog`
extended to Xcmn_Win00_C (its `Txt_Detail`/`Txt_Title` ARE direct props, confirmed by reflection) → the boot
messages (autosave notice, "Checking system data…", "Saving game…") should read, but only appear at game
BOOT so need a restart to verify. **UNCOMMITTED:** screen_tutorial.lua, screen_choicelist.lua(committed),
screen_dialog.lua Win00 extension, i18n `page` key, discover.lua (general item breakdown + scalar/opacity/
color/class-reflection probes). Commit after tutorial is verified.

**PAUSE MENU — PASSIVE-READ DEAD END, needs a new approach (this is the user's ask for next session).**
`Xcmn_Pause_C` (x1) → rows `List_Bar_C` = `List_Bar00/01/02` (labels in `Txt_List`: "Return to Battle" /
"View Battle Conditions" / "View Controls"; sort by index — dump order is reversed; List_Bar03/04 are empty
pooled slots). NO pause adapter exists yet. The SELECTED row is IMPOSSIBLE to detect from widget properties:
across all 3 rows, EVERYTHING is identical — `IsVisible`=true, `RenderOpacity`=1.00, `ColorAndOpacity.A`
identical (0.30/0.50/1.00 same pattern), same shared MaterialInstanceConstant on the 5 cursor images
(`Ins_Base_Hexa_Curs*`, `Ins_Curs_Hexa_Eff00/01`). Class reflection of `List_Bar_C` and `Xcmn_Pause_C` shows
NO selection bool/enum/index property and NO getter fn (only In/Out/Loop/Enter WidgetAnimations + Lang_Pause).
The highlight is purely material/animation-driven. `text_color` (node.mainTxt.ColorAndOpacity) returned "-"
(CFUIMultiLineTextBox is a custom primitive — its color is some OTHER property). **UNTRIED IDEAS for next
session, in rough priority:** (1) reflect the `CFUIMultiLineTextBox` class to find its real text-color prop —
the selected label looks yellow (B≈0) in the screenshot, so its color likely differs (most promising). (2) The
`Enter` WidgetAnimation per row — check `List_Bar:IsAnimationPlaying(row.Enter)` / anim end-state (selected row
holds its Enter anim). (3) `RenderTransform`/Slot geometry (selected row may scale/shift) — not yet measured.
(4) UE4SS `RegisterHook` on the pause cursor-move UFunction (RegisterHook IS available on this build now) to
track index actively. (5) Input-tracking: maintain our own cursor index from nav keys while pause is open
(fragile; keyboard arrows/WASD + controller d-pad/stick, must not consume input). Fallback if all fail:
announce all 3 options on entry (functional but no live cursor). Get a CLEAN pause-only dump (not mid-load) for
any new probe — the loading storm makes hundreds of pooled widgets falsely report visible.

**UPDATE 2026-07-01 (shadowing fix + pause/loading + tutorial confirmed).** Root cause found for why
pause/tutorial/loading were mute: **`IsVisible()` LIES on this game** — a pooled/closed window is Collapsed
at some ANCESTOR, but each child still reports `IsVisible()==true`, so `screen_dialog` latched onto stale
"Yes/No"/"Saving…" choice widgets and shadowed everything below it. FIX: new **`Core.on_screen(o)`** =
`IsVisible()` AND walk `GetParent()` up rejecting any ancestor whose `GetVisibility()` is Collapsed(1)/
Hidden(2). Confirmed by dump: the real `Xcmn_Pause_C` is enum 0 with no collapsed ancestor; ghost
`Xcmn_Win00_Choice_C_0..4` are IsVisible=true but under `VerticalBox(1:Collapsed)`. Swapped `is_visible`→
`on_screen` for container/selection gating in ALL adapters; `screen_dialog` now activates ONLY when a
message window is truly on_screen (kills the ghost-choice shadowing). **`screen_tutorial` now reads**
(Battle Controls tutorial). **NEW adapters:** `screen_pause.lua` (Xcmn_Pause_C; v1 reads the WHOLE 3-item
menu on entry — selected row still not distinguishable, see below) and `screen_loading.lua` (reads on_screen
text under Loading_C + Tips_C = Story Recap / Adventure Tips; only scans while a load container is up).
i18n `paused` key added. app.lua order now: choicelist > dialog > loading > tutorial > pause > options >
title. `discover.lua` rewritten as a FOCUSED dump (cached FindAllOf → no more hang; per-container on_screen
signals; per-List_Bar brush material type + all scalars/images; fixed "all widgets under pause" enumeration
using the object PATH needle). **PAUSE SELECTION still pending** a capture with the enhanced dump: looking
for the selected row's cursor image using a **MaterialInstanceDynamic** brush (vs shared Constant) or a
separate moving cursor widget. Screenshot shows the selected row has a GOLD BORDER (a cursor material), text
stays white — so the earlier "yellow label / text color" idea is likely WRONG.

**PERFORMANCE FIX (2026-07-01) — the mod was causing input lag** (user: menus/pause/tutorial took ms to
respond, had to press Enter several times). Root cause: `Core.loop` runs `step` on the GAME THREAD every
100ms, and every adapter re-ran `FindAllOf(...)` (scans ALL UObjects) in its acquire when its screen wasn't
visible — with 7 adapters, aligned ticks fired ~8 full-array scans in one frame → periodic game-thread
stalls → dropped input. FIX: **`Core.cached_live(cls, tick)`** — a persistent per-class ref cache. These
container widgets are POOLED (persist for the session, only Collapsed when closed), so we find each ONCE and
keep the ref; per-tick detection is now a cheap `on_screen(cached)` check. Re-scan only when the cached ref
is invalid; back off `ABSENT_BACKOFF=20` ticks for a class not present yet. All adapters refactored to
`cached_live` (dropped their acquire/REACQUIRE loops). `first_live` now PREFERS the top-level instance
(direct GameInstance child `…BP_ATGameInstance_C_0.<Name>$`) so multi-instance classes (e.g. Xcmn_Win01_C
has the real window + a nested collapsed copy) cache the RIGHT one. Remaining FindAllOf (choices/options
rows/tutorial+loading text) are all gated behind an on_screen check → run only while that screen is up.
Tutorial text scan (FindAllOf ~870 CFUIMultiLineTextBox) throttled to every 4th tick. **Also fixed a latent
bug: `screen_tutorial` (and discover's per-container "text:") matched the subtree with a needle that
included the CLASS PREFIX (`win:GetFullName()` = "Battle_Tips_Tutorial_C /Engine/…") which NEVER matches a
child's name (starts with its own class) → the tutorial reader found NO text and never worked. Fixed to use
the object PATH (`:match("%s(.+)$")`).** Net: per-tick cost ≈ cached refs + one on_screen per adapter; no
FindAllOf storm.

**PERF FIX ROUND 2 (2026-07-01) — FindAllOf scans ALL UObjects on EVERY call**, so any adapter iterating a
class every tick while active still stalled (user: slowdowns navigating skip-movie/tutorial/pause). Worst:
`screen_tutorial` — `Battle_Tips_Tutorial_C` stays on_screen (enum 3 HitTestInvisible) the WHOLE battle even
with no tutorial, so it FindAllOf'd ~870 CFUIMultiLineTextBox every 3 ticks throughout combat/pause. FIX:
**`Core.cached_all(cls, tick)`** — caches the FindAllOf list, re-scans only every REFRESH_EVERY=30 ticks
(pools are ~static; callers still validity-check). `screen_tutorial` now collects its container's text boxes
ONCE (`collect_boxes`, prefix-filtered off cached_all) and just checks their visibility each tick. Also
`screen_dialog.choices`, `screen_choicelist.options`, `screen_loading.content` use cached_all. Remaining
FindAllOf are on-demand (keyhelp F2, options tooltip on focus-change) or once-per-entry (options rows). Title
false-read at boot: gave `screen_title.confirm_ticks = 8` (registry now honors a per-adapter `confirm_ticks`)
so the title won't announce in the <800ms gaps between boot dialogs (persists if the gap is longer).
**PAUSE SELECTION still unsolved + the enhanced pause dump CRASHED at List_Bar00** (uncatchable abort in a
node deref — guarded node_text/label_color with valid() per level + added per-bar breadcrumb+flush in
discover). The selection is material/animation-driven (no readable widget prop differs across rows), so pause
stays v1 (reads whole 3-item menu on entry). Tutorial "which button per control" still TODO (glyphs are
images; needs a Battle_Tips_List_C row dump to map like keyhelp).

**TITLE-LEAK PROGRESS (2026-07-01, dumps).** During the intro CINEMATIC, `Gametitle_C` is on_screen but
`GetVisibility()==3` (HitTestInvisible = rendered, non-interactive), whereas the real menu is `==0` (Visible)
→ `screen_title.is_active` now requires enum 0, killing the cinematic "Main menu" blurt. BUT after
skipping / after choosing difficulty the title is genuinely enum 0 (full render) with "Saving…"/autosave
over it → enum can't distinguish those; the `confirm_ticks=15` (1.5s) debounce is the only mitigation there
(persists if the save/transition lasts longer). Added `Xcmn_System_Icon_C` + `Xcmn_Fade_Black_C/Fade_C/
Fade_Blur_C` to discover CONTAINERS to hunt a save/fade "we're mid-transition" signal next. Tutorial
page-turn re-read FULLY fixed by making cached content STICKY (never drop to nil on a transient scan while
on_screen). **discover PAUSE dump: `RenderTransform` (xform) is the uncatchable-abort crasher — removed it;
per-bar breadcrumb+flush confirmed the crash point was List_Bar00's detail line.** Pause reads
"View Controls | View Battle Conditions | Return to Battle"; selection still material-driven (v1 whole menu).

**BEHAVIORAL FIXES (2026-07-01, pending in-game verify).** Three bugs the user reported (pre-existing, not
perf): (1) skip-intro confirmation read "Difficulty Settings, Skip the movie?, No" — stale title bleed →
`screen_dialog.message()` now gates each node (Txt_Title/Txt_Detail) on **on_screen** (a node IsVisible
under a collapsed ancestor keeps stale text). NEEDS a "Skip the movie?" dump to confirm the stale title is
actually collapsed vs visible-on-the-active-window. (2) tutorial re-read page 1 when turning to 2/2 —
page-turn crossfade briefly shows BOTH pages' text → `screen_tutorial` now DEBOUNCES: commits new content
only when identical across 2 scans (SCAN_EVERY=3). (3) at boot the reader blurted "Main menu, Continue"
before the "Checking system data" dialog — `ui_registry` now DEBOUNCES screen switches (`CONFIRM_TICKS=2`):
a screen must be the top active adapter 2 polls running before it commits/announces, so a 1-tick flash is
suppressed (only helps if the gap is <~200ms). STILL OPEN: tutorial doesn't say WHICH BUTTON each control
is (glyphs are images, we read only labels) — needs a dump of Battle_Tips_List_C row internals to map the
glyph texture per control like keyhelp.lua does.

**i18n / LOCALIZATION LAYER (DONE 2026-07-01).** The mod's OWN spoken strings (button names, "mando:"/
"controller:" prefix, "Menú principal", reader on/off, keyhelp glyph words, combo join, "Guardar cambios"
fallback) are centralized in `Scripts/i18n.lua` with ES + EN tables; game-authored text stays read-live.
`A.button_name`/keyhelp/screen_title/screen_options/app all call I18n. **Game language detection** (the hard
part): UE culture is USELESS (`GetCurrentCulture` returns "en" even in Spanish — CC2 decouples it). Reliable
source = the message manager's loaded table path: `BP_ATGameInstance_C`(chain →`CFGameInstance`)`.MessageManager`
(`CFMessageManager`)`.DataTable` full name = `.../Message/PLAT_W/es_ES/messageData` → `match("/Message/[%w_]+/
(%a%a)_%u%u")` → "es" (cached; `I18n.refresh()` re-detects). `CFGameInstance.LocalizeManager` exists but has
no reflectable fields. Two PLAT axes: message text `PLAT_W`, glyphs `PLAT_X`. Detection spec also in
reference/dbz-kakarot/ui-and-text-architecture.md. main.lua's F8/F9/F7/reload dev strings left English on
purpose. To add a language: add a table to i18n.lua's `S`.

**PENDING USER TEST (pushed to GitHub 2026-07-01, not yet verified in-game — user will test later).**
Committed to `main` and pushed. To verify after Ctrl+Shift+R:
- Perf: navigation smooth in menus/pause/tutorial (the cached_live/cached_all FindAllOf fix) — user already
  confirmed "muchísimo mejor".
- Cinematic no longer blurts "Main menu" (title enum-0 gate) — NEW, unverified.
- After-skip / after-difficulty "Main menu" leak: may persist (enum-0 transition); if so, F7 at that instant
  (discover now dumps Xcmn_System_Icon_C + fades) to find the save/fade suppression signal.
- Tutorial reads and no longer re-reads page 1 on turning to 2/2 (sticky content) — unverified.
- Pause reads the 3 battle-pause items; re-dump pause (F7) now that the RenderTransform crash is removed.
STILL TODO: per-item pause selection, tutorial "which button per control", and the FIELD/overworld pause
menu (different widget, not yet mapped).

**CXX HEADER DUMP FINDINGS (2026-07-01, Ctrl+H → AT\Binaries\Win64\CXXHeaderDump\, 1839 .hpp).** Ctrl+H
works with the GUI console OFF (keybind feature). Authoritative class structures:
- **Pause selection is NOT a reflected property** (confirmed): `UXcmn_Pause_C : UAT_UIXCmnPause` has only
  ListBarArray + anims + funcs (OnEnterAnimEnd, MouseClickDecide, CheckHovered); `UList_Bar_C :
  UAT_UIGameoverBar` has only cursor Images + Txt_List + AnimEnter + a Border **`PnlCursEff`** (native-parent
  prop, accessible from Lua as `bar.PnlCursEff`) — the UNTESTED highlight candidate for the selected row.
  Next: Live-View-watch or read `PnlCursEff` vis/opacity while moving the pause cursor.
- **Tutorial buttons SOLVED path:** each row `UBattle_Tips_List_C : UAT_UIBattleTipsList` has `Txt_Btn`
  (button glyph — rich `<inputicon KeyConfigId=…>` markup, resolvable by our A.markup_to_speech) AND
  `Txt_Operat` (action name), set via `SetText(InBtnMsgId, InOpeMsgId)`. So read Txt_Btn.ExMainTxt +
  Txt_Operat per row → "LT: Guard". Container parent `UAT_UIBattleTipsTutorial` has DT_KeyGuideList,
  WL_TxtShow, WL_TipsList.
- Contrast: other list widgets DO expose selection (e.g. `UAT_UIXListBar01` has CursorWidget, SetNotSelect,
  LoseFocus) — so when a menu's selection isn't reflected, check the parent native class in AT.hpp first.
LESSON: use Ctrl+H CXX headers (AT.hpp = the CC2/AT native UI classes) to read real structure offline
instead of hand-dumping. See [ue4ss-screenreader-template](ue4ss-screenreader-template.md) discovery-tools doc.

**SESSION 2026-07-01 (late) — object-dump reflection study + new adapters + robust selection.** Mined the
CXX header dump (1839 hpp) AND the 80 MB `UE4SS_ObjectDump.txt` (6 parallel Explore agents) to answer "which
menu selection is reflected-readable vs fragile image-only". Full findings in
`reference/dbz-kakarot/ui-map-and-roadmap.md` (§1 "Selection — VERIFIED"). **KEY VERDICT:** the object dump
is the authority on Lua-callable API (a header member NOT in the dump is C++-only). **`GetSelectValue()->int32`
IS reflected/callable** on `UAT_UIMenuListBase00/01/03` + `MenuBarBase03` → the game's ONE generic robust
selection index, covering shop/item/cooking/skill-customize/DragonBall/options-bar/status sublists. Rows in
`ListPlateCtn` (TArray, **1-based in UE4SS Lua**), each `UAT_UIList0NChoice` with `TxtName`+`TxtNum`.
Exceptions with NO reflected index (our highlight heuristics ARE the game's own mechanism — confirmed keep):
title, all system/message/choice windows, **battle pause** (`Xcmn_Pause_C`/`UAT_UIGameoverBar` = bare shells),
CompZ, map, skill tree, field/lock-on cursors, and **`MenuListBase06`** (quest LIST). Stored reflected index
fields exist for a few screens (`UAT_UIGameover.CurrentSelectIndex`, `USimpleTalkWindow.CurrentPage`,
`UAT_UIInfoLog.CurrentIdx`). Nav hub `UMenuManager` has reflected pointers to every submenu. Combat HUD
fully mapped + VERIFIED reflected: read `node:GetText()` on the `WL_Txt*` `CFUIXcmnMultiLineText` nodes under
`AT_UIBattleManager` (BattleHudPlayer/Enemy/CommandPalette/BattleCond); total damage = `UAT_UITotalDamage.
CurrentValue` int. Dialogue surfaces mapped: `Xcmn_Subtitles_C`(`Txt_Name`/`Txt_Selif`), `Field_Talk_Win_C`
(`Txt_Speaker`/`Txt_Msg`), choices `Choice_Win01`/`Choice_Cmd00/01` (Dmy_Choice_Hover highlight), telops
`Quest_Main_Telop_C`. `AAT_GameHUD::OnMobMessage` is a reflected hookable UFunction; don't hook
`CFUIXcmnMultiLineText::SetText` (fires project-wide, FText-deref crash risk).

**BUILT THIS SESSION (all luac-validated against vendored Lua 5.4.4 = UE4SS's; PENDING in-game verify — user
away):**
1. **`screen_dialogue.lua`** (NEW, registered after screen_dialog) — story subtitles + NPC talk reader; polls
   both surfaces, reads via plain mainTxt → reflected `GetText()` → rich markup, announces "Speaker: line"
   diff-gated. Highest story value.
2. **`screen_pause.lua`** — battle pause now infers the SELECTED row from the cursor-glow alpha
   `Ins_Curs_Hexa_Eff00.ColorAndOpacity.A` (relative max + `SELECT_MARGIN=0.2` + 2-tick debounce), announcing
   per-item like a normal menu; **falls back to the old whole-menu read if alphas are uniform** → can only
   improve, never regress.
3. **`A.list_select_index` / `A.list_selected_row`** in `ui_archetypes.lua` — the reusable robust
   `GetSelectValue()` reader (guarded: valid() then pcall). 
4. **`screen_shop.lua`** (NEW) — first user of #3 (`Shop_Top_C.WidgetSkillList`). Additive + fully guarded →
   inactive/harmless if the header-read widget names (`WidgetSkillList`/`ListPlateCtn`/`TxtName`) differ live.
5. **Cleanup:** deduped the `Xcmn_Keyhelp_C` bar-finding — `keyhelp.lua` now exposes `Keyhelp.bar()` +
   `Keyhelp.helpmsg()`, and `screen_options.lua`'s tooltip reuses `Keyhelp.helpmsg` instead of its own scan.
   Built `luac.exe` (from vendored `libs/lua54/src/luac.c`) — reusable Lua syntax validator.

**IN-GAME TEST CHECKLIST for next session (Ctrl+Shift+R first):** (a) story dialogue/subtitles read
"Speaker: line" in a cutscene and an NPC talk; (b) battle pause announces the individual selected option as
you move (not just the whole list) — if it still reads the whole list, the glow-alpha signal is uniform and
`SELECT_MARGIN` may need tuning or the fallback is active; (c) shop reads the selected item+number (if silent,
verify `WidgetSkillList`/`ListPlateCtn`/`TxtName` names via F7/reflection and fix `screen_shop`/archetype);
(d) options tooltip still reads after the keyhelp dedup. NEXT ADAPTERS (reader ready, need runtime path
verify): inventory (`UIItemMenu.Xmenu_List00`), quest detail (`Quest_Sub` text; list is Base06 highlight-only),
skill customize, Dragon Ball menu, combat HUD — all via `A.list_selected_row` or `GetText()`.

**SESSION 2026-07-02 — new readers, glyph fixes, and two definitive DEAD ENDS.** Built + fixed
(all luac-validated; some verified in-game by the user):
- **`screen_dialogue.lua`** (Xcmn_Subtitles_C `Txt_Name`/`Txt_Selif` + Field_Talk_Win_C `Txt_Speaker`/
  `Txt_Msg`) — **CONFIRMED WORKING in-game** ("subtítulos funcionan a la perfección"). Reactive line
  reader, dedup via the announcer.
- **GLYPH RESOLVER — two bugs fixed** (`ui_archetypes.markup_to_speech` + `resolve_ctrl`): (1) the
  `<inputicon>` pattern only matched `KeyConfigId="X"/>` with NO extra attributes, so tutorial/HUD glyphs
  (`<inputicon KeyConfigId="Battle_Guard" ForceIconScale="2.7"/>`) were stripped whole → now match the
  whole tag and extract KeyConfigId from any attribute order; (2) the tutorial/HUD ids are the raw DYN id
  (`Battle_Guard`), not the ConfigName (`KeyConfig_Battle_Guard`) → resolver now also tries
  `dynToCtrl[id]` directly. Also handle `Plus`/`id="Icon_Plus"` → combo-join word. This fixed the tutorial
  buttons.
- **`screen_tips.lua`** (NEW, Tips_C → UAT_UITips) — REPLACES screen_loading's Tips handling.
  Tips is a **2-window book** (Tips_Win00/01 shown together); consecutive pages are BYTE-IDENTICAL in every
  readable property (enum/opacity/slot/ZOrder=0/canvas), and AT_UITips exposes NO page index (only
  MouseClickTabLeft/Right), so the current page is NOT pollable. Solution: read BOTH on-screen windows,
  speak each UNIQUE page ONCE (dedup by content, cleared when Tips_C closes) → no page mute, no stale
  re-read. Per window: header Txt_Title/Subtitle/Txt_Page, then EITHER the command list (Tips_List00_*
  `Txt_Detail`+`Txt_Btn`, only when `Canvas_KeyTips` is on_screen) OR the explanation prose
  (`Txt_Detail00/01/03/04`) — never both, so text pages read the explanation not the Guard/Burst/Charge
  footer. **`screen_loading.lua`** now reads ONLY Loading_C (Story Recap), and reads the Xcmn_MultiLineText_C
  WRAPPERS via `GetText()`+markup_to_speech (glyphs + `<span>` stripped) with on_screen gating + consecutive
  dedup (fixed the jumbled multi-page `<span>` mess).
- **`screen_shop.lua`** (NEW) + generic **`A.list_select_index`/`A.list_selected_row`** (GetSelectValue()
  → ListPlateCtn[idx].TxtName/TxtNum) — the reusable robust reader for MenuListBase00/01/03 lists (shop/
  item/cooking/skill/DB). Pending in-game verify of the widget names.
- **`screen_pause.lua`**: added an **enum-0 gate** (only active when the battle pause is genuinely OPEN,
  ESlateVisibility.Visible; it stays resident HitTestInvisible all battle) and **moved it above the
  tutorial** in app.lua registration — this FIXED "pause doesn't read" (it was shadowed by the resident
  Battle_Tips_Tutorial). App.lua order now: choicelist > dialog > pause > dialogue > tips > loading >
  tutorial > options > shop > title.
- Dedup: `keyhelp.lua` exposes `bar()`/`helpmsg()`, reused by screen_options' tooltip. Built `luac.exe`
  (vendored libs/lua54/src/luac.c) as a reusable Lua 5.4.4 syntax validator (in the session scratchpad).

**DEAD END 1 — battle pause per-item selection (DEFINITIVE, don't retry from Lua).** Confirmed with the
pause OPEN (enum 0) and DIFFERENT rows highlighted: ALL rows read identical on EVERY signal — `Ins_Curs_Hexa_
Eff00.ColorAndOpacity.A`=0.30, `Eff01`=1.00, `PnlCursEff.ContentColorAndOpacity.A`=1.00, BrushColor.A=0.00,
RenderOpacity=1.00, all the same. Object dump confirms `AT_UIXCmnPause` (only CheckHovered/MouseClickDecide/
MouseClickTabLeft/Right/OnEnterAnimEnd + anims/ListBarArray) and `AT_UIGameoverBar` (only AnimEnter/TxtList/
PnlCursEff/RootPanel) have ZERO selection members; `UCFPauseManager` is the gameplay-freeze manager, not the
menu. So the selection is a NON-reflected C++ member (invisible to Lua) — reads all 3 options on entry
(functional). Only a native path could read it.

**DEAD END 2 — field/overworld Start menu (Start_Top_C → UAT_UIStartTop), DEFINITIVE from Lua.** Fully
image-font: ring items `Start_Top_List00..06` (UAT_UIStartTopList) have only Images (Image_Font/FontOn), the
shared header `UAT_UIXCmnHeader` shows the section as `Image_HeaderFont` (an image), the section id is the
`EXCmnHeaderFontType` enum passed to `SetFontType(FontType,…)` with no stored getter, and URootMenu/UBaseMenu
store no current index. THREE angles all failed: (a) hooking — `RegisterHook` INSTALLS fine but menu nav is
C++-direct (not ProcessEvent): live test fired SetFontType=0, RootMenu:OnSelectItem=0, RootMenu/StartTop:
CheckHovered=0, StartTop:In_Curs=1 (menu open only); (b) reflection — no text/index anywhere; (c) reading the
header/ring IMAGES — `hdr.Image_HeaderFont` (and ForEachProperty / FindAllOf("Image")) throw a **C++ exception
that Lua pcall CANNOT catch** (kills the operation but does NOT crash the game — the game survived every
probe). So the overworld menu is unreachable from Lua entirely. `header_hook.lua` install() is NEUTERED to a
no-op (kept, with the tried-list recorded); `header_reader.lua` + the `EXCmnHeaderFontType`→name map in
`i18n.lua` (`I18n.header`) + `App.header_font` are kept DORMANT, ready for a future native reader.

**FUTURE C++ / native option (discussed, not started).** Both dead ends need native code: the selection is a
non-UPROPERTY C++ member (Lua/reflection can't see it) and nav is C++-direct. LIGHTWEIGHT route = extend the
existing `prism_bridge.dll` with `read_int(address, offset)`, get the object address in Lua via
`obj:GetAddress()`, and find the offset via **Ghidra** (already at `D:\code\decompilers\ghidra_12.0_PUBLIC`)
on `AT-Win64-Shipping.exe`. Native functions/offsets are FRAGILE (break on game patches) — the reason the
project chose Lua-only. The full C++ UE4SS mod route stays REJECTED (heavy/fragile). Decision deferred.
**USER REQUIREMENT for the Ghidra pass (2026-07-02): decompile so the code is as READABLE as possible —
recover NAMES, not raw numbers/addresses.** No `FUN_xxxx`/`DAT_xxxx`/offset soup. Concretely: enable the
MSVC RTTI analyzer + demangler (per the global memory) so vtables/classes get real names, AND leverage the
**CXX header dump** — it already gives the real class names, member names AND offsets (e.g.
`AT_UIGameover:CurrentSelectIndex [o: 3E0]`), so import/recreate those structs in Ghidra and apply them to
the objects so the pseudocode reads with real field names. The goal is legible decompiled code that maps
1:1 to the known UE classes, then find where the pause selection / menu-section index actually lives.

**DISCOVER TOOL LESSON.** The dump "cutting off" was NEVER a game crash — it's an uncaught error killing the
`ExecuteInGameThread` lambda (UE4SS catches it at the boundary; game fine). Two stoppers on this game:
`ForEachProperty` on some widget classes (List_Bar, Xcmn header) and any read that raises a C++ exception
(the Start menu images). Wrap EVERY property access in its own pcall AND avoid ForEachProperty on unknown
widgets; use breadcrumb `flush()` before risky reads so the last line names the culprit.

**SESSION 2026-07-02 — HYBRID NATIVE BRIDGE DONE + committed/pushed (commit e523eba).** Both Lua dead
ends SOLVED via a small native memory reader. See `PROGRESS.md` (repo root) for the full handoff.
- **`mem_bridge.dll`** (`src/mem_bridge/mem_bridge.c`, `build.ps1`) — a SEPARATE in-process Lua C module,
  guarded (SEH) raw memory reads only (`read_i32/u8/i64/ptr/bytes`, `readable`, `module_base`), NO mod
  logic. Kept apart from `prism_bridge.dll` (screen reader stays screen-reader-only). `Scripts/mem.lua`
  wraps it (`Mem.i32(obj,off)` reads at `obj:GetAddress()+off`; `Mem.fstring` reads non-reflected FString
  ptr@off+count@off+8 UTF-16LE→UTF-8). **`Scripts/native_offsets.lua`** = ALL offsets in one data file
  (game patch → re-run Ghidra + edit this, no recompile).
- **CONFIRMED OFFSETS:** battle-pause selected row `UAT_UIXCmnPause 0x43C` i32; overworld ring index
  `UAT_UIStartTop 0x4E4` i32; overworld item entry-id `UAT_UIStartTopList 0x404` u8 (=`START_TOP_LIST_ID`
  enum: COMMUNITY0 DRAGONBALL1 ITEM2 PARTY3 QUEST/Story4 CHARACTER5 SYSTEM6 COMMUNITY_BOARD7
  COMMUNITY_EMBLEM8 SYSTEM_SAVE9 SYSTEM_LOAD10 SYSTEM_OPTION11 SYSTEM_TUTORIAL12 SYSTEM_TITLE13 DLC14,
  mapped by `I18n.startlist`); field header `UAT_UIXCmnHeader 0x400/0x428`.
- **`screen_pause.lua`** now announces the selected row natively; **`screen_field.lua`** (NEW) reads the
  overworld main menu per-entry, localized. This SUPERSEDES both "DEAD END" paragraphs above (battle-pause
  per-item + overworld Start menu) — they're now readable natively.
- **Ghidra workflow (`code/`):** exe is SteamStub v3.1 DRM → unpack with Steamless
  (`D:\code\decompilers\Steamless_v3.1.0.5`) → `code/AT-Win64-Shipping.exe.unpacked.exe` is what Ghidra 12
  analyzes (project `code/ghidra/KakarotAT`, 16GB-heap `analyzeHeadless_kakarot.bat`). Gotchas: disable the
  "Embedded Media" analyzer (crashes JVM on a bad PNG) via `disable_media.java`; Ghidra 12 dropped Jython →
  scripts are JAVA. No RTTI for game classes → reach native methods via UE's function-registration table
  (`{const char* Name, FNativeFuncPtr exec}` pairs): `find_ufunc.java` finds a UFunction impl by name,
  `decompile_addrs.java` dumps any address. Docs: `code/README.md`, `code/hybrid-mod-feasibility.md`.
- **NEW DEV TOOL F4 (`dev_memdiff.lua`):** universal runtime offset probe — open a menu, F4 (baseline),
  move cursor, F4 again → reports `Class +0xNNN = value` for whatever i32 changed; also appended to
  `Scripts/dev_probe.txt` (git-ignored). This is how both offsets were pinned. F7 reflection discovery +
  `dev_log.lua` also added.
- **app.lua order now:** choicelist > dialog > pause(native) > dialogue > tips > loading > tutorial >
  options > shop > field(native) > title. Plus keyhelp (F2).
- **NEXT (per PROGRESS.md):** the 8 overworld submenus, START WITH SYSTEM (Save/Load/Options/Tutorial/
  Title — lets you load saves + return to menu while testing; likely `UIStartTopList06_Sub`, names 9-13
  already in I18n.startlist, just need the sub-selection offset via an F4 probe inside System). Then
  Items/Party/Characters/Story/Community/DragonBalls — many likely reuse reflected `MenuListBase00/01/03`
  (`GetSelectValue` → `A.list_selected_row`, already used by screen_shop) so a generic list adapter may
  cover several. Loose ends: Tips multi-page state, "View Controls" from pause (reads jumbled, no re-announce
  on close). Housekeeping: `header_hook.lua`/`header_reader.lua` now superseded by native field reader →
  retire once submenus done.

**SESSION 2026-07-03 — SYSTEM overworld submenu SOLVED + the other submenus mapped.**
- **`screen_field.lua` now reads the SYSTEM submenu** (Save/Load/Options/Tutorial/Title). The hard part
  was distinguishing "hovering System on the ring" from "inside System" — the `_Sub` arrays AND
  `on_screen()` both LIE on the ring (the System sub-list `UIStartTopList06_Sub` MIRRORS/RETAINS ring or
  stale System items, and its pooled item widgets report on_screen=true even at ring level → an early
  attempt to gate on them read the wrong item and SILENCED the ring). **THE RELIABLE DISCRIMINATOR is a
  native depth flag: `UAT_UIStartTop + 0x4DC` (int32) == 1 → browsing the RING; == 0 → INSIDE a submenu.**
  Found via a labeled F4 snapshot (added `dump_starttop` to dev_memdiff dumping i32 at 0x4b0..0x4fc). So
  screen_field: read `0x4DC`; if 1 → ring (read `0x4E4`); if 0 → submenu (read sub-cursor `0x4EC` into the
  sub-list picked by the ring section `0x4E4`). Offsets now in native_offsets.lua: `startTop.depthFlag=0x4dc`,
  `startTop.subIndex=0x4ec`. `SUBMENU_BY_SID = {[6]="UIStartTopList06_Sub"}` (System). Ring reading itself
  is UNCHANGED/safe. CONFIRMED working in-game.
- **STORY is NOT a Start_Top sub-list — DEAD END for the sub-list path.** Inside Story the depth flag
  STAYS 1 (Start_Top freezes at ring level) because Story opens a SEPARATE screen `UAT_UIStartQuest` (a
  Base06 highlight-only list, no reflected index). `UIStartTopList04_Sub` is unused for us. Story needs its
  own adapter and is one of the HARD ones.
- **OTHER RING SUBMENUS MAPPED (from the game's CXX header dump at
  `<game>\AT\Binaries\Win64\CXXHeaderDump\`, 1839 hpp; game install = `D:\games\steam\steamapps\common\
  DRAGON BALL Z KAKAROT`). KEY: these 7 screens have NO blueprint `_C` — instantiated directly from native
  classes, so FindAllOf the NATIVE short-name (e.g. `FindAllOf("AT_UIItemMenu")`), not a `_C`.**
  - EASY (reflected index — reuse `A.list_selected_row`/getter, build thin adapters like screen_shop):
    **Items** `AT_UIItemMenu` → `Xmenu_List00` (MenuListBase01) `GetSelectValue()` (+ tabs `SetSelectTab`);
    **Dragon Balls** `AT_UIStartDragonBallMenu` → `UICmn00MenuList` (MenuListBase00) `GetSelectValue()`;
    **Characters** `AT_UIStartChar` → `GetCursorIndex()` (rows `WL_StartCharBarList[].Txt_Name`);
    **Party** `AT_UIStartParty` → stored `partySelectData.cursorIndex` int32 @0x420 (rows
    `WL_Start_Char_Bar[].Txt_Name`).
  - HARD (highlight-only, no reflected index): **Story** `AT_UIStartQuest` (Base06), **Community**
    `AT_UICommunityStart` (emblem grid), **DLC** `AT_UIStartDlc` (only ClickSelectUp/Down). NOTE:
    `UAT_UIXmenuCommu.CurRank` is a shop star-rank widget, NOT the community submenu index — don't use it.
- **Dev tool:** F4 (`dev_memdiff`) now also emits a `[fields] 4B0=.. .. 4FC=.. ptr4d8=..` labeled Start_Top
  snapshot each press (for finding more native flags cleanly, vs the noisy byte-diff).

**SESSION 2026-07-03 (cont.) — 4 overworld menus DONE + a REUSABLE fixed-array technique.** Built thin
adapters for the ring entries that open their own screen. KEY: these have NO blueprint `_C` in the CXX dump
but DO exist as `_C` at runtime (e.g. `Start_Char_C`, `Start_Party_C`); FindAllOf the NATIVE short-name
(`AT_UIItemMenu`, `AT_UIStartChar`, `AT_UIStartParty`, `AT_UIStartDragonBallMenu`) finds them.
- **Items** (`screen_list.lua` factory) + **Dragon Balls**: MenuListBase → `GetSelectValue()` via
  `A.list_selected_row` (reflected index). `screen_list.new(class, list_member, name_fn)` is a reusable
  factory for any MenuListBase host. Items=`Xmenu_List00`, DragonBalls=`UICmn00MenuList`.
- **Characters** (`screen_characters.lua`): `GetCursorIndex()` is Lua-callable; the bar is a single-display
  carousel (`WL_StartCharBarList` reads the selected char directly). Reads name+Lv+BP.
- **Party** (`screen_party.lua`): the LEFT slots (Player/Support#1/#2) = `WL_Start_Party_List`
  (Start_Party_List_C) → `WL_Start_Party_Bar` (0x398, 3-ptr fixed array). Reads role (Txt_Player/Txt_Suport)
  + char (Txt_Name / Txt_None "Not Set"/"Cannot Change") + Txt_Lv. cursorIndex = `partySelectData.cursorIndex`
  (0x420, also readable natively as `startParty.cursorIndex`). CONFIRMED working in-game.
- **THREE REUSABLE UI-CORE HELPERS added (ui_core.lua):**
  1. **`Core.first_on_screen(cls, tick)`** — for screens with SEVERAL pooled instances where the active
     (visible) one ALTERNATES (e.g. `Start_Char_C_3`/`_4`). `cached_live` locks onto one and goes silent
     when the other is live; this picks the on_screen one each tick from `cached_all`. USE THIS for any
     multi-pooled screen (was the root cause of "menu found but silent").
  2. **`Core.read_text(node)`** — plain `mainTxt` (text_of) else reflected `GetText()`; some
     CFUIXcmnMultiLineText nodes render only via the parent FText (Txt_Lv/Txt_Power_Num).
  3. **FIXED-ARRAY UPROPERTY COLLAPSE (the big one):** UE4SS reflection collapses a fixed C-array member
     (`Type* Name; // (size: 0xNN)` where NN>8) to ONLY element 0 — `host.Member` yields element 0,
     `host.Member[i]` yields garbage/nil, and array methods (`:GetArrayNum()`/`#`) UNCATCHABLY ABORT. To read
     the other elements, use UE4SS **`RegisterCustomProperty`**: register each hidden pointer at
     `base + i*8` as an ObjectProperty. VERIFIED WORKING on this build:
     `RegisterCustomProperty({Name="X", Type=PropertyTypes.ObjectProperty, BelongsToClass="<blueprint obj
     path e.g. /Game/Art/UI/Start_Party/Start_Party_List.Start_Party_List_C>", OffsetInternal=0x3A0})` then
     `host.X` yields the element UObject. Register lazily once (guarded) when the screen first appears; the
     BelongsToClass is the runtime blueprint class path (`obj:GetClass():GetFullName()` minus the
     "WidgetBlueprintGeneratedClass " prefix). **This unlocks EVERY fixed-array row menu** (party slots,
     multi-char lists, status stat lists, etc.).
- **How offsets/paths were found:** the CXX header dump in the GAME folder
  (`D:\games\steam\steamapps\common\DRAGON BALL Z KAKAROT\AT\Binaries\Win64\CXXHeaderDump\`, `AT.hpp`) gives
  member names+offsets; a one-shot in-adapter probe (log `obj:GetClass():GetFullName()`, `Mem.addr`,
  `Mem.ptr(host, off)`) confirms the live class path + that `ptr@base == reflected element-0 address`.
- **STILL TODO on Party:** the RIGHT "Standby Characters" panel (`WL_Start_Char_Bar` @0x3C0, 4-ptr array:
  entries = available chars + "Remove") that focus jumps to when confirming a Support slot — needs detecting
  the left↔right focus switch + its own cursor. Also: Character **Status** detail screen (`UAT_UIStartStatus`,
  reflection-friendly WL_Txt_* + stat list `WL_Start_Char_List01`) requested earlier, deferred.

**SESSION 2026-07-03 (cont. 2) — Tutorials list + Tips book DONE (commit 4cb9244, pushed).**
- **`screen_tutorials.lua`** (System > Tutorials, `AT_UIStartTips`): reads category tab
  (`TextBox_Category`, LB/RB) + selected topic. The topic list `UIXlistTutorial` (Cmn03MenuBar10 →
  MenuBarBase03) is SCROLLING: `GetSelectValue()` returns an ABSOLUTE index that exceeds the ~9 pooled
  `ListPlateCtn` rows, so DON'T index by it — instead scan the pooled `UAT_UIBar03Choice` rows and read the
  one whose cursor highlight `Ins_Cursor_Fad` is visible. `A.list_selected_row` got a bounds check
  (`ListPlateCtn:GetArrayNum()` before `[idx+1]`) — an out-of-range TArray index ABORTS UNCATCHABLY, which
  crashed on a tab switch (the list rebuilds); it also falls back to `Txt_List` (List03 rows) + `Core.read_text`.
  KNOWN MINOR: some navigation delay in this list (9-row scan per tick) — optimizable, user accepted for now.
- **`screen_tips.lua` REWRITTEN — the pending hard Tips problem is SOLVED.** Tips_C (`UAT_UITips`) is a
  2-page book: BOTH windows (Tips_Win00/01) stay loaded; which is the visible FRONT page is a non-reflected
  C++ member. The widgets' own page numbers/canvases LAG a navigation step (max-page heuristics failed on
  back-nav). **FOUND VIA F4: `UAT_UITips + 0x420` = front window index (0=Win00, 1=Win01); `+ 0x424` =
  current page index (0-based). Both update immediately, no lag.** So read the front window directly
  (`WINDOWS[mem.i32(tips,0x420)+1]`). Within a window, each page uses ONE layout (`Canvas_TextOnly`/
  `TextAndImage`/`BNID`/`KeyTips`) → read ONLY the active layout's native TextBox (`TextBox_TextOnly` etc.);
  inactive layouts + the back page hold STALE text. Single-page tutorials leave a stale "X/Y" count → drop it
  unless X-1 == native page index. Announce front content on change, debounced 2 ticks. CONFIRMED PERFECT
  in-game (forward+back nav, page switching, 1-page tutorials). Offsets in native_offsets `tips.frontWindow`
  /`tips.pageIndex`. **LESSON reinforced: F4 (dev_memdiff) is the go-to for any "current selection/page/window"
  that reflection can't see — it found this in one probe.**

**SESSION 2026-07-03 (cont. 3) — QUEST NAVIGATION RADAR built (PENDING in-game verify).** Audio radar
ported from the XV2 mod (`D:\code\dbxv2`: same sounds `beacon.wav`/`arrived.wav` now in `Scripts/sounds/`,
same by-ear formulas: cadence 120–850 ms, vol `1−0.6t`, behind-pitch `1+fb·0.3` min 0.6, equal-power pan
vs CAMERA forward, arrival 3D). New pieces:
- **3rd bridge `audio_bridge.dll`** (`src/audio_bridge/audio_bridge.cpp`, C++/XAudio2 dynamic-loaded):
  `ping(pan,vol,pitch)`/`arrival()`/`stop()`. GOTCHA: XAudio2 2.9 needs `CoInitializeEx` on the calling
  thread + link `ole32.lib`, else `CreateMasteringVoice` fails. Verified end-to-end with a standalone Lua
  host (audible pan/pitch/arrival OK).
- **`Scripts/nav_tracker.lua`** — own 50 ms LoopAsync (gen+busy guarded). Target = the game's own guidance
  markers: `FindAllOf("AT_UIMiniMapNaviIcon")` → `.TargetActor`, classified via the actor's
  `ATMapIconComponent.MapIconType` (EMapIcon: 24/71 main quest, 26/40/47/53/59/72 sub — from AT_enums.hpp;
  ALL these members verified reflected in UE4SS_ObjectDump). Auto-tracks new markers (mission accept),
  cadence tracks the FINAL target while pan follows the next **NavMesh route corner**
  (`NavigationSystemV1.FindPathToLocationSynchronously` — verified reflected; auto-fallback to straight
  line; do NOT call the path's reflected `IsValid()` — UE4SS shadows it). Gated silent when a screen
  adapter is active or the minimap is hidden (battle/cutscene/menu). Keys: **F3** on/off, **Shift+F3**
  route mode, **F5** speak target (type/meters/clock/up-down), **Ctrl+F5** dev dump →
  `Scripts/dumps/dump_nav_targets.txt`.
- **Verify checklist:** pan sign by ear (if mirrored, negate `pan`); navi-icon scan finds markers
  (Ctrl+F5 dump shows `in_use`/`pri` per icon + navmesh probe); arrival radius ~8 m feels right;
  FAR_DIST=200 m cadence scaling suits Kakarot field sizes.

**SESSION 2026-07-03 (cont. 4) — FISHING MINIGAME fully decoded + accessibilized, VERIFIED
WORKING end-to-end (user landed a fish). NEXT TASK: accessibilize the COOKING menu
(`Shop_Cook_C -> UAT_UICookingMenu`, SetSelectTab; likely a MenuListBase list -> reuse
`A.list_selected_row`; crafting readout rules in reference/ui-accessibility/inventories.md;
start with an F7 dump inside the cooking screen). (commit 49a3c67 + follow-up).**
Full model in PROGRESS.md + `native_offsets.fishing`. Key reusable lessons:
- **Iterative self-instrumentation WORKS**: a DEBUG flag in the adapter appending per-tick samples of
  the widget's non-reflected tail to dumps/dump_fishing.txt let us decode a live minigame (cursor,
  zones, phase index, ring size/target, rating tiers) across a few user play sessions, no Ghidra.
- **QTE ring** = `AT_UIBattleRushSpeedCore` (find live — parent pointers to it are unset); rating =
  |ringSize−ringTarget|/target vs tiers 0.75/0.5/0.25 in its tail; its `Xcmn_Btn_Plat` holds the
  button IN ADVANCE while hidden. Same widget likely used by battle QTEs → reusable.
- **Face buttons are indexed everywhere** (glyphs Btn00..03, EATPlatBtnId 0..3, ActionBtn pad ids);
  the KeyConfig asset IconName pairing was WRONG live; standard Xbox order (0=A,1=B,2=X,3=Y) is the
  truth (FACE_TOKEN in ui_archetypes; `A.platbtn_token` resolves any UAT_UIXcmnPlatBtn to a token).
- **Difficulty assist via reflected config writes**: `CursorSpeed`/`CursorLapLimit` are writable from
  Lua — halved speed / doubled laps per attempt, verified live. Pattern for other minigames.
- **Time-critical prompts speak BARE TOKENS** ("X, luego A") — full sentences cost the reaction
  window; continuous tone (audio_bridge `tone(vol,pitch)`) carries the WHEN with a 2-tick lead.
- Attempt boundary = the re-randomized zone value changing (the pooled HUD never deactivates).

**SESSION 2026-07-03 (cont. 5) — COOKING MENU first live test + revision (commit 20099b1).**
First test: entry read the full recipe (name+effects+ingredients+description) but cursor moves
spoke NOTHING, and raw `<span color=…>` markup was read aloud. Fixes in `screen_cooking.lua`:
- **`GetSelectValue()` on the cooking list FROZE after entry** (suspected: live cursor in
  `UAT_UIMenuListBase00`'s non-reflected tail 0x450..0x4A0 — same pattern as pause/StartTop).
  CAUTION for shop/item/skill adapters that trust `A.list_selected_row`: the reflected index may
  be entry-only there too (shop still unverified live).
- Fix: read the selection from the **detail pane**, which the game repopulates per cursor move —
  dish name = `CookMenuList.WL_CookWin_Cap_Title` (UAT_UICookingMenuShopList; static captions are
  separate `Cap_*` members), fallback = list row. A selection SIGNATURE (raw title+effect+desc)
  drives the new `Announcer:invalidate()` (ui_core) so a stale name can never silence the reader.
- **Always pipe spoken text through `A.markup_to_speech`** — reflected `GetText()` returns RAW
  markup; the game's renderer resolves it, we must strip/resolve ourselves.
- `DEBUG=true` in screen_cooking appends per-selection-change lines to `dumps/dump_cooking.txt`
  (GetSelectValue vs pane title) — read it after the user's next test; then set DEBUG=false.

**Same session — SOUL EMBLEMS (commit 4eeff8b), PENDING verify.** The "Soul Emblems Received"
notice = `Xcmn_Win01_C`; its native base `UAT_UIGameWindow` has typed CONTENT POOLS (huge find,
reusable for ALL reward/notice popups): `WL_ItemPlateCtn`, `WL_TextCmuCtn` (soul-emblem rows:
WL_CharName+WL_LevelTitle+WL_LvTextList), `WL_LinkListPlateCtn`, `WL_CheckPlateCtn`,
`WL_DetailReward`, and `WL_TextPlateCtn` (window-owned selectable choices, same
Xcmn_Win00_Choice_C rows/highlight as top-level Yes/No). screen_dialog now reads pools as the
message tooltip + nested choices. NEW `screen_community.lua`: grid = `AT_UICommunityStart`
(native find) slots `EmbList.EmbAry`; cursor signal UNVERIFIED → probes 4 candidates
(Ins_Frame_Set/CanvasEffectA/B/AnimLoop) and only accepts one that singles out EXACTLY ONE slot
(sticky); unacquired = `UIXCmnEmb.ImageUnacquired` (the "?"). Detail = `Start_Commu_Detail_C`
(Txt_Name/Txt_Commu_Lv/Txt_Popular00/Txt_Link/Txt_Link_Detail/Txt_Char_Detail/Reward_Bar00..02).
DEBUG dump → `dumps/dump_community.txt`. I CAN READ the user's screenshots (Pictures/Screenshots,
newest) — they confirmed the grid layout, pages 6/7, yellow cursor frame.

**Grid pinned live (commits b47f181 + 7cf1fd5):** cursor = slot whose `AnimLoop` IsAnimationPlaying
(transient → hold last index while idle); `Txt_Commu` = the emblem's community LEVEL; acquisition =
face brush is a **MaterialInstanceDynamic** vs the constant "?" mask — TRAP: the MID is ALSO named
"Ins_Emb_Mask", never match on the mask name. **REUSABLE: character identity from image-only widgets**
— the face MID's `TextureParameterValues[i].ParameterValue:GetFullName()` is a Charicon texture
(`/Game/Art/UI/Charicon_Ev/Ev_<Tok>NN_…`); token → name via `CHAR_TOKENS` in screen_community.lua
(Gok=Goku, Ghn=Gohan, Vgt=Vegeta, …; FULL token list extracted from the PAK INDEX with
`grep -ao "Ev_[A-Za-z0-9_]*" pakchunk*.pak | sort -u` — pak index strings are plaintext). Same
Charicon token families exist as Charicon_D (D_…) and Charicon_F (F_…) for other screens (party,
characters). Unmapped tokens are spoken raw so the user can report them.

**Community board SOLVED (commits bc9388d..4504a73):** board host = `Start_Commu_Brd_C`
(from the PAK INDEX — FindAllOf on the native base AT_UICommunityBoard finds NOTHING, unlike
the grid; when a native-name find fails, grep the pak for the blueprint name). Grid stays
rendered UNDER the board → board checked first. ALL canvas slots on the board read 0,0
(render-transform positioning) → hovered socket pinned via IN-ADAPTER TAIL DIFF (auto memdiff
on cursor move, no F4 needed — REUSABLE technique): `Start_Commu_Brd_C +0x500` = hovered
WL_PanelTbl index (0-based; +0x5D8 flips 1/-1 on/off socket) → native_offsets.commuBoard.
Notice-RELEASE pattern in screen_dialog (announce once, release the tick, `spoken` NOT
cleared in reset) verified: persistent tutorial popups no longer mute the screen beneath.

**Evening batch (commit e568980), PENDING verify:** (1) emblem-grid INPUT LAG fixed — cause was
per-tick heavy reads (2 anim probes ×21 slots + label rebuilt with GetFullName/texture walks every
100 ms); rule: ONE reflected scan per tick + cache the spoken label (`cached_label`). (2) COMMUNITY
BOARD mode (AT_UICommunityBoard): sockets WL_BrdFrame.WL_PanelTbl (emblem face → CHAR_TOKENS, WL_Lv,
leader pedestal, empty); hovered = ActiveAnim unique else nearest-by-Slot.LayoutData.Offsets to
WL_PanelCursor (NEVER RenderTransform — abort); summary from WL_CommuBrdDetail queues on board
change; cursor/socket mapping UNVERIFIED (DEBUG flag ready). (3) screen_choice.lua — dialogue/quest
choices: Choice_Cmd_C rows, selected = Dmy_Choice_Hover/HoverImage visible; prompt = Choice_Win_C
message or Quest_Sub_C title+detail; registered ABOVE dialogue. (4) screen_telop.lua — quest banners
(Quest_Main_Telop_C 12 Telop_Txt slots, dedup-once, queued, nav_mute=false), BELOW dialogue.

Reusable bridge + crash-fix docs live at `D:\code\unreal\UE4ss study` (see [ue4ss-screenreader-template](ue4ss-screenreader-template.md)).

**SESSION 2026-07-06 batch (lupa-compile-validated, PENDING in-game verify):** (1) **Super Attacks
menu (Shop_Training_C) empty-list fix** — is_active gated on the detail panel's skill name, so a
character with NO acquirable skills read NOTHING; now gates on the live list title
(Xlist_List05_Lay7.Txt_Title) and speaks "Lista vacia" (i18n `list_empty`) after a 4-tick settle.
(2) **Controls guide announce-once** — screen_tutorial keeps module-level `last_spoken` NOT cleared
by reset(), so the resident guide no longer re-reads itself every time it re-becomes the active
adapter (mid-dialogue repeats); F1 re-reads via new adapter hook `reannounce()` (ui_registry.
repeat_current prefers reannounce over reset). (3) Radar: collected-item filter via
AAccessPointBase.InteractState==State_Taken(11), item names (ActionName / drop-table id), sites +
enemies categories, pickup chaining, enemy proximity alert — details in [dbz-kakarot-radar-picker](dbz-kakarot-radar-picker.md).

**SESSION 2026-07-06 batch 2 — three shop screens + ring-pause fix (PENDING verify):**
(1) `screen_shopinfo.lua` — Shop_Info_C (information store): rows = blueprint props
Xlist_Bar05_00..07 (Shop_Info.hpp), selected = `Ins_Cursor_Fad` visible (same marker as
options rows), tooltip = live Txt_Detail. (2) `screen_shopcmn.lua` — Shop_Cmn_C
(food/material buy list): rows = prefix-matched `Xlist_Bar04_C` pool entries (the
Shop_Cmn_C BLUEPRINT class is NOT in the CXX dump -> never touch blueprint-named host
props; host text via the native WL_Txt_* BindWidgets of UAT_UIShopCommon), value =
price + "comprar: N" + "tienes: N" (+ sold out), tooltip = WL_Txt_Detail + WL_Txt_Stock.
Both registered ABOVE screen_shoplist. (3) screen_shoplist rows_of falls back to the
blueprint's direct Xlist_Bar01_00..03 when ListBarCtn is empty (the NPC-talk mode list
"Preparar un platillo / Salir" on Shop_Top_C_1 wasn't in ListBarCtn). (4) **Ring pause
mute after leaving a shop**: the game opens the ring on ANOTHER pooled Start_Top_C
instance; `cached_live` never re-scans while holding a valid-but-collapsed ref ->
screen_field now uses `Core.first_on_screen("Start_Top_C")`. LESSON: for any pooled
GameInstance widget that can have multiple instances, gate with first_on_screen, not
cached_live.

**2026-07-06 mute-ring ROOT CAUSE (from Ctrl+F5 dump: adapter_index=17, ring enum=0
on_screen=true):** the shadowing adapter was `screen_cooking` — a pooled/closed
Shop_Cook_C kept `on_screen(host)=true` + a FROZEN `GetSelectValue()` index, staying
active with nothing to speak (name nil -> silent shadow). Fixes: (a) `Core.on_screen`
now also requires a ROOT UserWidget top-of-chain to be `IsInViewport()` (this game
closes some screens by removing the root from the viewport with slate enum still
Visible; IsInViewport proven safe — discover.lua's inVP column) + ancestor walk depth
8->24; IsInViewport called ONLY on UserWidgets (IsA check — nonexistent member = the
abort). (b) screen_cooking: live-detail gate (title()/genre() required) + the
complete/result overlay uses the notice-RELEASE pattern (overlay_spoken, not cleared in
reset). LESSON: a silent active adapter = check Ctrl+F5 adapter_index vs app.lua order.
Enemy alert STILL silent near pre-Raditz robots even with the SpawnType scan — raw
AT_Character probe (class + SpawnType within 300m) added to Ctrl+F5; awaiting a field
dump to learn what class the roaming symbols really are.

**2026-07-06 batch 3 (PENDING verify):** (1) `screen_results.lua` — story results
(Quest_Main_Clear_C -> UAT_UIQuestMainClear): bars UIClearBar_List (TextBox_Item +
Image_Rank), details UIClearDetail_List (TextBox_Detail + Image_Rank), total
UIQuestMainClearRank.Image_Rank. RANK = brush TEXTURE name suffix (keyhelp technique),
validated against {S,A,B,C,D,Z,SS}. INCREMENTAL reader: each line queued once as it
becomes readable (follows the reveal animation); `spoken` set cleared on close, not
reset(). Registered right after screen_memory (adapter indexes after #2 shifted +1!).
(2) **Items inventory FIXED**: it was registered as FindAllOf("AT_UIItemMenu") — the
native base finds NOTHING on this game (community-board lesson); the real widget is the
blueprint `Start_Item_C`. Its Xmenu_List00 is Cmn01MenuList09 -> MenuListBase01
(GetSelectValue + ListPlateCtn of UAT_UIList01Choice TxtName/TxtNum work). screen_list
factory gained `tab_member` (list.TxtTitle = category) and `detail_nodes` (subtree-
scanned blueprint detail pane: Txt_Cap00/Detail00/Cap01/Detail01/Cap02/Detail02/
Detail03) spoken as tooltip. The DragonBalls ListScreen ("AT_UIStartDragonBallMenu")
likely has the same native-name problem — no blueprint in the CXX dump; needs an F7
dump of that screen to fix.

**2026-07-06 batch 4 — item palette (PENDING verify):** `screen_palette.lua` —
Start_Item_Customize_C ("Registrar paleta", X from the inventory; blocks the tutorial).
Expected native base UAT_UIItemPaletteCustomize (titleTxt/MsgTxt/PaletteCtn of
UAT_UIItemPaletteBar rows: Txt_List item, Txt_Num, Xcmn_Btn_Plat d-pad glyph,
cursor candidates Img_Win01_Curs00/Pnl_Curs_All) — base NOT in the CXX dump, so the
adapter verifies `host:IsA(StaticFindObject("/Script/AT.AT_UIItemPaletteCustomize"))`
at runtime before ANY property read (new REUSABLE pattern for unverified blueprint
bases). Selection = whichever cursor signal marks exactly ONE row; fallback reads all
slots on entry. Keyhelp.glyph(plat) exported (guarded on class name Xcmn_Btn_Plat_C —
Dmy_Btn_NN are blueprint-only). Registered ABOVE the Items ListScreen (palette opens
over it) — adapter indexes shifted again vs Ctrl+F5 adapter_index.

**Palette v2 (the IsA guard proved the base was NOT UAT_UIItemPaletteCustomize; F7
dump_1783326153 gave the real tree):** Start_Item_Customize_C tree = Txt_Title +
Txt_Msg + rows Start_Item_Bar00..03 (Txt_List "---" when empty; face-button rows
Y/X/B/A). Rewritten with ZERO property access: subtree scans of CFUIMultiLineTextBox /
Xcmn_Btn_Plat_C / Image pools by host path prefix. Button name from plat's native
CurrentDynamicAssignInputControllerId ("...Btn_Y" -> I18n.button), glyph fallback.
Selection = the row whose "Curs"-named child Image is uniquely visible; fallback =
read all rows on entry. Active gate = host on_screen + Txt_Msg on_screen.

**Palette v3 (PENDING verify):** rows = FindAllOf("Start_Item_Bar_C") instances under
the host (blueprint-name lookup), native members (AT_UIItemPaletteBar) all safe:
Txt_List/Txt_Num/Xcmn_Btn_Plat/Img_Win01_Curs00 (the cursor). Button via the existing
A.platbtn_token resolver (face-index map). Structure came from PAK EXTRACTION (see
[dbz-kakarot-decompilation-setup](dbz-kakarot-decompilation-setup.md)) after live sweeps kept dying.

**Palette SOLVED (live-verified signals, 2026-07-06):** open gate = host
Start_Item_Customize_C `GetVisibility()==0` (parks at 3 HitTestInvisible when closed —
Txt_Msg on_screen was UNRELIABLE as a gate); selected row = `Pnl_Curs_All` is_visible
(exactly one bar, moves with cursor); `Img_Win01_Curs00` is visible on EVERY row (red
herring). Bars sorted by name = screen order top-to-bottom. Debug telemetry
(dump_palette.txt + version stamp on load) kept in the file, DEBUG=false.
REUSABLE LESSONS: (a) popup-over-menu widgets can park HitTestInvisible instead of
Collapsed -> gate on enum==0; (b) always version-stamp debug files — one 'reload
didn't apply' wasted a whole round-trip; (c) cached collect() must RETRY until
complete (scan-budget race can return empty pools on the first tick).

**Committed + pushed 2026-07-06 (commit 4c24da1, user-confirmed palette WORKING
in-game):** palette/results/shopcmn/shopinfo readers, Items inventory fix, radar
categories+sweep+enemy alert, ghost-screen fixes, PROGRESS.md session entry. STILL
PENDING in-game verify: the radar batch (taken-filter, names, sites/enemies cats,
chained sweep, enemy alert — user owes a field Ctrl+F5 near enemies + a
before/after-collect dump), screen_results, shopcmn/shopinfo, training empty-list,
controls-guide-once, ring-pause fix after shops.

**2026-07-06 SHADOW LESSON (the big one):** an adapter that is ACTIVE but SILENT is the
worst failure mode — the announce-once change to screen_tutorial made the RESIDENT
controls guide hold the dispatcher tick mutely, shadowing EVERY adapter below it
(Items/map/shops all "inaccessible"; Ctrl+F5 adapter_index=12 exposed it). RULE: any
resident/overlay adapter must use the notice-RELEASE pattern (speak once, then
is_active returns false until content changes / F1 reannounce). F1 now falls back to
Tutorial.reannounce() when no adapter owns the tick. Items menu ALSO had a frozen
GetSelectValue (stuck 0) — ListScreen gained `name_node` (detail-pane title as the
LIVE selection name, subtree-scanned; Items uses Txt_Title00). Diagnostic flow that
cracked all of these: dumps + adapter_index + per-adapter change-logged telemetry
files (dump_items.txt / dump_palette.txt).
ENEMY LEAD: pc.Pawn once showed **TwinFootBP_C** (a two-legged dino pawn at origin) —
field creatures are their own BP pawn classes (not AT_Character SpawnType); the
passive dump_enemies.txt Pawn probe will name them as the user roams.

**Items menu VERIFIED working (2026-07-06)** after the two fixes: tutorial
notice-release (the silent shadow) + name_node live detail-pane title (frozen
GetSelectValue). screen_list DEBUG off.

**Pushed db495a5 (battle HP narrator + results details + toasts) and 165afab
(subtitles fix).** Battle monitor: HP thresholds from Battle_Hud_P/E_Main_C Txt_Num_Hp
vs per-battle max baseline; bands cross-only (99% is NOT "75"). Subtitles: the game
drives any of THREE pooled Xcmn_Subtitles_C / Field_Talk_Win_C instances across scenes
-> scan ALL (missed narrator lines); speaker+body read atomically (body re-checked) to
kill mixed-line pairing. MULTI-INSTANCE RULE now bit FIVE times (Start_Top, Start_Char,
Map_M/Map_World, Battle_Hud_E, dialogue): NEVER cached_live for pooled GameInstance
widgets — scan instances. PENDING verify: subtitles fix, results details v2, battle
narrator band fix, toasts. Enemy probe: creatures so far TwinFootBP_C, A001_BP_C
(AnimalComponentBase family has HpMax); actual aggro ROBOTS still unseen in
dump_enemies.txt — alert wiring waits for that.
