# STATUS — Dragon Ball Z Kakarot

> Per-mod status ledger / dashboard. Open this first when resuming the mod so progress isn't re-derived from the code each session. Keep it short — a dashboard, not docs. Update the **Next step** line and the section table whenever you finish a chunk. Derive every value from the game's real data — no guessed offsets.

**Last updated:** 2026-07-14

## Identity
- **Engine / framework:** UE4 (AT project) + UE4SS v3.0.1 — Lua scripts plus C bridge modules (`prism_bridge`, `audio_bridge`, `input_bridge`, `mem_bridge`).
- **Screen-reader transport:** PRISM (`prism.dll` + `prism_bridge.dll` in `Scripts/`, `tolk.dll` fallback backend).
- **Build command:** per-bridge `src/<bridge>/build.ps1` (rebuild only the bridge you touched). Lua is not compiled; validate syntax with `libs/lua54/luac.exe -p <file>`.
- **Game install path:** `D:\games\steam\steamapps\common\DRAGON BALL Z KAKAROT`. Exe + UE4SS at `…\AT\Binaries\Win64\`. RE dumps live there: **`CXXHeaderDump\`** (per-class `.hpp`, the authority on layouts/members — `AT.hpp` is the big one) and **`UE4SS_ObjectDump.txt`** (what Lua can actually reflect). Grep these instead of re-deriving.
- **Mod install path:** junction `…\DRAGON BALL Z KAKAROT\AT\Binaries\Win64\Mods\KakarotAccess\Scripts` → repo `mod/KakarotAccess/Scripts`; enabled in `mods.txt` (`KakarotAccess : 1`).
- **Run / test:** launch the game with the mod enabled. `Ctrl+Shift+R` in-game reloads the Lua feature adapters + i18n; `main.lua` changes (keybinds, `Mem.init`, `Speech.init`) or a new/rebuilt DLL need a full game restart.

## Section status
`done` = works with the screen reader on; `wip` = started; `todo` = not begun.

| Section / feature | Status | Notes |
|---|---|---|
| Speech pipeline (PRISM) | done | Logs chosen backend on boot |
| Overworld main menu (native selection) | done | `screen_field.lua`, reads via `UAT_UIStartTop` offsets |
| Battle-pause menu (native selection) | done | `screen_pause.lua`, `UAT_UIXCmnPause +0x43C` |
| Dialog / message / confirm popups | done | `screen_dialog.lua` (incl. reward/emblem content pools, gift picker) |
| NPC subtitles / dialogue | done | `screen_dialogue.lua` |
| Difficulty / choice lists | done | `screen_choicelist.lua`, `screen_choice.lua` |
| Options / System / Title / Tutorials / Tips | done | `screen_options/title/tutorials/tutorial/tips.lua` |
| Shops (food/material/info) + item palette | done | `screen_shop*.lua`, `screen_palette.lua` (verified in-game) |
| Items inventory + Party + Characters | done | Party/Characters done. Items list reads populated categories (via `Txt_Title00` detail-pane live name; reflected index tracks). EMPTY categories: the whole item UI goes STALE (row/detail/visible-count keep the last item), so emptiness is read from the native flag `itemMenu.hasItems = 0x620` (F4-confirmed; 0 = empty) and announced ("vacío") via `screen_list.lua` factory `empty_off` param. Verified in-game 2026-07-11 |
| Item submenu (use-item char select) | done | `screen_itemuse.lua` — A on a usable item → pick who uses it. Reads the on-screen `AT_UIItemMenu.WL_Start_Party_Bars` bar (the selected char; only it animates in): `Txt_Name01` + `Txt_Lv01→Txt_Lv02` level-up preview, with the "choose character" prompt. Registered before the item list reader. Verified in-game 2026-07-11 |
| Save / Load data slots | done | `screen_saveload.lua` — `AT_UIStartSaveLoad`. VIRTUALIZED 3-bar window (`UISaveLoadBar_List`), so pool-position ≠ ordinal. Ordinal from native index `saveLoad.selectedIndex = 0x410` (+1), cursor bar = `windowPos = 0x418` (F4-confirmed over ~11 saves); reads FILLED and EMPTY slots (Canvas_None checked first); SETTLE_TICKS debounce drops mid-scroll frames. Slow re-entry (widget destroyed+recreated → stale class-list cache) FIXED by `ui_core.first_on_screen` churn-force (re-scan a recently-on-screen class immediately, budget-gated). Verified in-game 2026-07-11 (reads all slots, correct index, fast entry, no lag). DEBUG off |
| Character status page (stats sheet) | wip | `screen_status.lua` — `UAT_UIStartStatus`, the sheet you get by confirming a character in Personajes. ALL reflected: `WL_Txt_Name/_Lv/_Title/_Num/_Power/_Power_Num`, the two `UAT_UIStartStatusHud` gauges (`HpGauge`/`SpGauge` → `TextBox_Number`) with their `TxtStatus`/`TxtStatusNum` row TArrays, and the five `UAT_UIStartStatusList01` attribute blocks (`WL_Txt_Power(_Num)` = total, `TxtStatusList`/`TxtStatusNumList` = rows). `WL_Start_Char_List01` is a fixed C array → UE4SS COLLAPSES it (never index it — the `WL_StartCharBarList` lesson), so the blocks come from the class's cached instance list, on-screen ones only, ordered by `Core.slot_pos(w,"Y")`. Entry reads the header (name, level, next, HP, Ki, BP); the 7 blocks (total + base/estado/comida) are walked with the **d-pad down/up** on the gamepad (own 20 ms `Input.read` loop — the 100 ms registry poll misses quick taps — pad only READ, never blocked; gated on `Registry.active_adapter()` so the palette/tree opening OVER the page don't inherit it) and with **F11 / Shift+F11** on the keyboard. Registered below the palette+tree, above the Characters list. Pending in-game verify (needs a full RESTART) |
| Skill Palette / Super Attack equip | done | `screen_skillcustom.lua` — selected slot plate = `SelectActiveBorder` visible AND `BaseBlinkImage` hidden (structural plates 4/7 have both always ON); slot button from plate `ButtonIconImage` → `A.platbtn_name`; empty slot = literal "---" → "ranura vacía"; level/Ki/desc from the detail pane only while it names the same skill (pane lags and goes stale on empty). `SkillListMenu:GetSelectValue()` is DEAD here (frozen 0) — never use it. Verified in-game 2026-07-13 |
| Skill Tree / learn super attacks | done | **2026-07-14: the lock is read NATIVELY** from the game's own per-node state byte (offsets in *Derived facts*), so "bloqueada"/"adquirida" is correct on EVERY node whatever the browsing path — the old KNOWN LIMIT (noted below) is GONE. The name-propagation heuristic survives only as the fallback if the native read fails its bounds/FName self-check. Entry was slow (~30 s) until the feed's storm guard was fixed (see *Known issues*). Verified in-game 2026-07-14. — `screen_skilltree.lua` — `Start_Skilltree_C` < `UAT_UISkillTreeMenu`, ALL reflected (`Txt_Skillname/Txt_Lv_Num/Txt_Energy_Num/Txt_Detail/Txt_Name`); orbs = `WL_Skilltree_Zorb00` TArray of 12 `UAT_UISkilltreeZorb`: entries 1–6 = REQUIRED cost, 7–12 = OWNED. Orb color = POSITION in its 6-orb grid (red/blue/green/purple/silver/gold — verified twice 2026-07-14; textures don't encode color, all named "Ins_Item"). Reads name+lvl+Ki+non-zero cost, desc, owned orbs at the end. "How to acquire" text is NOT on screen while browsing (7-node full-text scan) — it only exists in the post-A message window, which already reads (decision: leave as-is). LOCKED nodes: the ONLY marker the game exposes is the tree cursor's padlock (`UISkillTree.Skilltree_Cursor.WL_ImgIconMicon` visible), and only on a skill's ENTRY (lv-1) node when the skill is unowned — 6 capture rounds proved there is nothing else: no readable panel index/pointer/position (cursor grid coords do live at tree+0x15F8/+0x15FC as grid×95), key-help bar identical on every node, no cover/lock widget on screen; the char-level gate ("need level 10") exists ONLY in the post-confirm message window (dialog reader speaks it). So the lock is PROPAGATED by skill name within a screen visit (`locked_skills`, cleared in reset): once the entry node reads locked, every level of that skill announces "bloqueada". KNOWN LIMIT (accepted): reaching a level-2/3 node WITHOUT passing its level-1 node first announces no lock — unfixable by reflection. Round-7 panel sweep closed the last door: all 108 `WL_Ins_Panel_Cover` are visible always (a frame, not a padlock) and every skill icon is an anonymous `MaterialInstanceDynamic`, so panels can't be matched to skills. The real data (`USkillManager`/`USkillTree`) exposes ZERO reflected functions and owned levels live in private save memory (`USkillSave`) — a full fix needs native RE (Ghidra), deferred. Verified in-game 2026-07-14 |
| Contextual actions (keyhelp) | done | `keyhelp_watch.lua` — the screen's ACTION prompts ("X: asignar", "Y: árbol de habilidades", "A: usar") read once on entering any menu and again only when the set CHANGES (diff-gated, queued behind the screen's own readout). Hangs off the `ui_registry` dispatcher, so every menu (incl. future ones) is covered; passive/time-critical readers opt out with `keyhelp_auto = false` (13 adapters). Face buttons are finally NAMED: `keyhelp.lua` now falls through to `A.platbtn_token` (the palettes' resolver) when the bar's device-INDEXED textures (Btn00..03) can't name themselves. Nav entries ("mover", "cambiar pestaña") are dropped. Ctrl+F2 toggles it (needs a game RESTART — main.lua); F2 still reads the whole bar. Read LEFT-TO-RIGHT as on screen: the bar is a CanvasPanel the game lays out itself, so the widget number is a slot id, NOT a position — the place comes from the slot's `GetPosition()` (its `LayoutData` offsets reflect back as 0.0), falling back to the render transform / ancestors. COST RULE (learned the hard way: the first cut lagged the item + skill-palette menus to a crawl): inside the poll step it may ONLY use `Core.cached_all` (tick passed — a raw `FindAllOf` per poll stalls the game thread) and it polls the bar's LABELS, resolving the glyphs just once, on the poll where they changed. Verified in-game 2026-07-14 |
| Quest objective HUD (text) | wip | `quest_objective.lua` — `Quest_Navi_C` rows `Txt_List_00`; announces on change + F10 on demand. F10 needs a game RESTART (main.lua); reactive works on Ctrl+Shift+R. Pending verify |
| Cooking menu | wip | `screen_cooking.lua` revised; pending re-verify (detail-pane read, markup strip) |
| Fishing minigame | done | `screen_fishing.lua`; verified end-to-end (user landed a fish) |
| Soul Emblems grid / Community | done | `screen_community.lua`; verified in-game |
| Community Board cursor (story tutorial) | done | Verified in-game 2026-07-04, unblocked story; offsets in `native_offsets.commuBoard` |
| Story / battle results | done | `screen_results.lua`, `screen_battleresult.lua` (rank from brush textures) |
| Quest navigation radar | done | `nav_tracker.lua` + `audio_bridge`; auto-tracks quest markers, arrival cue confirmed |
| Radar categories 2.0 (sites/enemies/collectibles) | wip | Built 2026-07-06; pending in-game verify of the batch |
| R2 radar target picker (modal) | wip | Built 2026-07-04; needs a game RESTART to verify (new `input_bridge.dll`) |
| Battle monitor | wip | `battle_monitor.lua` present |

## Derived facts (so we never re-RE them)
| Fact | Value | Source |
|---|---|---|
| Battle-pause selected row | `UAT_UIXCmnPause +0x43C` int32 | Ghidra; `native_offsets.lua` |
| Overworld ring index / nav-depth | `UAT_UIStartTop +0x4E4` (idx), `+0x4EC` sub, `+0x4DC` depth flag | Ghidra `In_Curs`; `native_offsets.lua` |
| Overworld item entry id | `UAT_UIStartTopList +0x404` u8 = `START_TOP_LIST_ID` | Ghidra `FUN_1416bca00`; `native_offsets.lua` |
| Reliable player field char | `AT_UIMiniMapRadar.PlayerIns` (camera `.CameraMng`) — NOT `PlayerController.Pawn` | live verify 2026-07-03 |
| Community board hovered socket | `Start_Commu_Brd_C +0x5D8` (game cache) / mode gate `+0x500` = 7 | Ghidra `FUN_1414c7de0`; `native_offsets.commuBoard` |
| Skill-tree hovered node + its state | cursor `UAT_UISkillTree +0x15F8/+0x15FC` (raw grid col/row) → `cell = i32[+0x4CC + (row*30+col)*4]` (1-based) → `zorb = ptr[+0x438][cell-1]` → **state `u8[zorb+0x460]`**: `{0,1}` locked, `{2,5,8}` acquired, `{3,4,6,7,9,10}` open. Node `FName` at `zorb+0x418` (= `ptr[+0x1538][cell-1]`, self-check) | Ghidra `FUN_141672080` (cursor refresh), `FUN_141664ab0` (OnInputDecide gates on `0x124>>state&1`); `native_offsets.skillTree` |
| Skill ownership (authoritative) | `USkillManager+0x138` = `USkillSave`; `FSkillSaveInfo` via a **TMap** at `SkillSave+0x28` keyed by `CHARACTER_TYPE` (elem stride 0x18, value = `FSkillSaveInfo*` at elem+0x08 — NOT a flat array); then `HaveSkillTreeId` +0x100 / `OpenSkillTreeId` +0xF0 | Ghidra `HasSkill` → `FUN_14145efc0`, `FUN_14145e140`, tri-state `FUN_14145c4b0` |
| Skill-tree "requires level N" | `SkillTreeDataTable = USkillManager+0x108`; row via `RowMap` TSet at `DataTable+0x30` (hash = `FName.ComparisonIndex + Number`); `CharacterLevel` = `i32[row+0x58]`. Lock reason codes in `FUN_14144efc0` (2 = level too low) | Ghidra `FUN_14145e360` (row lookup), `skill_rowmap.java`. **`+0x30` is DERIVED, not read from an instruction — sanity-check `RowMap.Num` at runtime before trusting** |
| Face-button glyph index (`EATPlatBtnId`) | **0=B, 1=A, 2=X, 3=Y** — the enum is a PS one and orders the faces right/bottom/left/top (○✕□△), so A/B are the MIRROR of the naive Xbox order (X/Y already match). Shared by the keyhelp bar, the QTE/fishing prompts and the item quick-slot palette | `AT_enums.hpp` + live `dump_keyhelp` 2026-07-14 ("Usar"=idx 1, "Atrás"=idx 0); `ui_archetypes.FACE_TOKEN`, [reference](reference/dbz-kakarot/input-icons-and-keyconfig.md) |
| Fishing phase/cursor/zone | `Mgame_Fishing_C +0x50C` phase, `+0x518` cursor, `[0x520,0x524]` zone | live verify; `native_offsets.fishing` |
| All other native offsets / class names | — | See `native_offsets.lua`, `dumps/`, and `code/` (Ghidra) |

## Next step
**The Skill Tree is CLOSED — do not reopen it.** The "requiere nivel N" reason was deliberately DROPPED
(user, 2026-07-14): pressing A on a locked node makes the game state the requirement itself, in the message
window the dialog reader already speaks, so the tri-state ("bloqueada" / "adquirida") is all the reader
needs. The RE for it is recorded in *Derived facts* if that ever changes — don't re-derive it.

**FIRST, verify the crash + slowness fixes** (2026-07-14, needs a RESTART — `mem_bridge.dll` is new):
play a normal session — menus, a map load, and several minutes of free-roam WITH a quest tracked (route
radar on) crossing streaming boundaries (that is what reproduced the crash). Expect: (a) NO crash, in
menus OR gameplay; (b) no `UObject instance is nullptr` in the log; (c) the reader does NOT get sluggish
the longer you play; (d) re-entry into the item menu / skill palette / skill tree starts reading within a
few seconds. Do NOT bring the notify feed back whatever happens (see Known issues).

Then: **verify the character STATUS page + the contextual-action reader in game** (one RESTART covers both
— F11/Shift+F11 and Ctrl+F2 are new keybinds in main.lua). On the status page: entering a character should
speak name, level, "Siguiente N", the HP and Ki gauges and BP; the **d-pad down/up** (or F11 / Shift+F11)
should then walk the 7 blocks (PS, Ki, ATQ/DEF cuerpo a cuerpo, ATQ/DEF de Ki, crítico), each as its total
+ base/aumento/comida, in the on-screen order top→bottom. Two things to watch: if the order comes out
scrambled, the block sort had no positions to work with (`Core.slot_pos`) — dump it before changing
anything; and if the GAME turns out to react to the d-pad on that page (it shows only R-stick rotate + X/Y/B,
so it shouldn't), move the binding to LB/RB rather than blocking the pad. For the keyhelp: open the inventory and the character
menu and check you hear the choices ("botón X: asignar", "botón Y: árbol de habilidades", "botón A: usar")
once on entry, and again only when they change. Then the **backlog** below: the quest-objective HUD and the
radar 2.0 batch are both built but never verified in-game.

## Backlog
All work through 2026-07-14 is COMMITTED and PUSHED (latest: 47df2d2). The narrative of how each
feature was derived lives in PROGRESS.md and in the git log; this list is only what is still OPEN.

- **Quest objective HUD** (`quest_objective.lua`) — pending verify. The F10 on-demand read needs a FULL
  game restart (Ctrl+Shift+R does not re-run main.lua, where the keybind is registered); no Lua errors in
  the log and the bind is present, so most likely it was just never restarted. The reactive reader only
  speaks when the objective CHANGES — an objective already on screen staying put is expected silence.
- **Radar 2.0 batch** — sites/enemies/collectibles categories + the "Caza"/Hunt category: built, never
  verified in-game. R2 target picker: check the boot log for `hooked=true`.
- **Cooking menu** (`screen_cooking.lua`) — revised, pending re-verify (detail-pane read, markup strip).
- **"View Controls"** (from the battle pause) reads jumbled, and the pause does not re-announce on return.
- Niceties: skill-palette plates 4/7 (structural, cursor never lands there so far); in assign mode the
  A/B press itself is silent until the first cursor move (no signal exists for the press).

## Known issues / open questions
- **SOLVED 2026-07-14 (round 2) — the mid-GAMEPLAY crash was `nav_tracker` reading a TArray length on a
  dying world object.** Separate bug from the feed (this one survived the feed removal). Signature: the same
  uncatchable `0xe06d7363` throw, no Lua error logged, mid free-roam after a while. Root cause: `Transition.active()`
  fires only on a full map load (new GameMode), NOT on streamed sublevels — so streaming frees world actors
  during free-roam with no gate, and only per-object re-validation protects us. `nav_tracker` read raw
  `#arr` / `.PathPoints` on objects that were only nil-checked: `compute_route` (`path.PathPoints` — a
  reflected NavigationSystem result, ran every ~3 s with route tracking, the prime site), `aim_watch`
  (`comp.m_xActors`, every 100 ms), and the `MapIconList` scans in `best_candidate`/`list_targets`. All now
  go through `Core.array_of` (validates owner+array before `GetArrayNum`; `pcall` can't catch the throw, so
  the validity check BEFORE the call is the only defence). Dev-only dumps (`nav_dump`, F7) fixed too.
  Verify: free-roam with a quest tracked (route radar on) for several minutes across streaming boundaries.
- **FIXED 2026-07-14 — the reader went sluggish the longer you played** (my own regression from the feed
  removal). The first no-feed detection gave freshly-destroyed classes a 1 s "fast lane" and forced a
  re-scan the next tick; as a session accumulated closed screens they all demanded scans at once and
  saturated the 3-per-tick budget, starving live detection. Now: ONE fixed cadence — alive pool refreshes
  every ~30 s, a list with no live instance (absent OR destroyed) re-scans every ~4 s (`DEAD_BACKOFF`), no
  per-tick forcing anywhere. Re-entry to a churned screen ≤ ~4 s; no saturation regardless of play length.
- **SOLVED 2026-07-14 — the widget event feed. It is REMOVED; do not rebuild it in Lua.**
  Root cause, measured (not assumed): UE4SS delivers a `NotifyOnNewObject` callback on the engine's async
  LOADING thread as well as the game thread (`widget notify thread(s): 5744 (FOREIGN!), 38620 (game
  thread)` — via the new `mem_bridge.thread_id()`). Any Lua there — even the feed's two table writes —
  runs the shared `lua_State` concurrently with the poll step, so the allocator/GC race frees userdata the
  game thread still holds. Symptom: a cached widget that passed `IsValid()` and then reported a NULL
  UObject on the next member call (`GetArrayNum` → `screen_skillcustom.lua:92`), once per tick — the
  poisoned entry was inserted at the HEAD of the cached list, so it shadowed the real screen permanently —
  until UE4SS's uncatchable C++ throw killed the process (0xe06d7363). The earlier AV at a garbage address
  was the same corruption. **Dead ends (do not retry):** `ExecuteInGameThread` inside the callback (the
  wrapper is itself Lua on the foreign thread) and moving the stash into a mutex-protected C bridge
  (reaching C still executes Lua bytecode). A safe event feed would have to be armed from a native UE4SS
  C++ mod, outside this `lua_State`.
  - Replacement (no notify, no per-tick scans): `Core.first_on_screen` already walks the cached list, so it
    spots for free when a class's instances have all gone invalid — the screen was DESTROYED and its reopen
    will build a new one — and puts that class in a fast lane (`CHURN_FAST` ~1 s scans for a `CHURN_WINDOW`
    of ~15 s). A screen that is merely CLOSED keeps its instances valid, so the steady state costs nothing.
    **The distinction is load-bearing** (first cut got it wrong and half the menus, main menu included, went
    SILENT): an EMPTY cached list means "never opened", which is the normal state of nearly every menu — it
    must NOT force a scan, or all ~25 idle classes ask every tick, the 3-scans-per-tick budget is eaten by
    whoever probes first, and the adapters at the BOTTOM of the registry never get scanned at all. Empty →
    `DEAD_BACKOFF` (~4 s, cheap). Dead-but-not-empty → fast lane. **Verify in game: re-entry into the item
    menu / skill palette / skill tree should start reading in a second or two, not 30.**
  - `Core.array_of(owner, name)` is now the ONLY way to read a TArray: `owner[prop]` returns an INVALID
    RemoteObject rather than nil, and a raw `GetArrayNum` on that is the uncatchable throw. Migrated:
    `screen_skillcustom`, `screen_skilltree`, `screen_tutorials`, `screen_status`, `ui_archetypes`.
  - The transition gate is now the mod's ONLY `NotifyOnNewObject`. A GameMode is an actor (spawned on the
    game thread), so it should be safe — and it now PROVES it: its log line reports its own thread
    (`New game mode — transition gate ON (notify thread: game thread)`). If that ever says FOREIGN, that
    notify has to go too and the gate must be driven from a poll.
  - (Superseded by the above: the whole 2026-07-13/14 series of feed fixes — the boot-crash hardening,
    the storm guard, the throughput/memoized-chain rework — is gone with the feed. The "a slow screen is
    a FEED bug, never fix it with scans" lesson is void: the feed itself was the bug.)
- F7 discover dump can fatal (0xe06d7363) if the swept UI is dying mid-animation: two caught `brush_of`
  "nullptr instance" errors then a raw C++ throw (2026-07-14). Mitigated with a 3-failure fuse in
  `discover.lua brush_of`; still, avoid F7 during screen transitions/animations.
- R2 picker: if the boot log shows `hooked=false`, pad blocking is off (read-only) — needs a
  GetProcAddress/inline-hook fallback.
- After any game patch, re-verify offsets in `native_offsets.lua` via the F4 probe.

**Detailed history:** see PROGRESS.md.
