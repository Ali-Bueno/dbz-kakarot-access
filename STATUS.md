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

Next: **verify the contextual-action reader in game** (needs a full RESTART — the Ctrl+F2 toggle is a new
keybind in main.lua). Open the inventory and the character menu and check you hear the choices ("botón X:
asignar", "botón Y: árbol de habilidades", "botón A: usar") once on entry, and again only when they change
(e.g. hovering an item that can't be used drops "usar"). Then the **backlog** below: the quest-objective
HUD and the radar 2.0 batch are both built but never verified in-game.

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
- INTERMITTENT boot crash (2026-07-14, AV 0x10 right after "Event loop start", during the first map
  load): the event feed's drain probed widgets whose classes the async loader was still linking.
  HARDENED (committed): the drain skips + wipes while `Transition.active()`, every stashed widget waits
  one tick (`aged` queue) before any probe, PROBE_CAP=256/tick, STORM backlog bound. **If a boot crash
  recurs → revert the notify feed to polling (plan B), stability over latency.**
- FIXED 2026-07-14 — the storm guard silently defeated the feed on the biggest screen. It used to DROP
  THE WHOLE BACKLOG past STORM=2048; the Skill Tree constructs 2064–2862 widgets in one frame (logged),
  so its ROOT went in the bin and the screen was only found by the 300-tick refresh (~30 s to start
  reading — every other menu builds far too few widgets to trip it). Now the drain TRUNCATES instead:
  UMG builds a screen's root before its children, so the oldest entries are the ones adapters key on;
  the dropped tail is leaf widgets the refresh net picks up. Per-tick cost is unchanged (PROBE_CAP still
  bounds it). **Lesson: a slow screen is a FEED bug — never "fix" it with scans or shorter refreshes.**
- FIXED 2026-07-14 — the widget feed was THROUGHPUT-starved, and that (not any adapter) was
  what made the item/skill-palette menus take ~30 s to read on a RE-entry. The drain walked each
  widget's class chain with up to 12 reflected calls and only took 256/tick, so the game produced
  faster than we consumed: the log showed 30+ storms with backlogs up to 8167, and the storm guard
  (keep the OLDEST, drop the tail) then binned the NEWEST arrivals — precisely the screen just
  opened, which fell back to the 300-tick refresh net. Now the class chain is memoized per class
  (2 calls per widget), PROBE_CAP=1024, STORM=8192 (a last resort again), and the cached lists are
  pruned (they grew unbounded as pooled screens were recreated). **If "widget storm" lines come
  back in the log, the drain is too slow — do NOT just raise the cap.**
- F7 discover dump can fatal (0xe06d7363) if the swept UI is dying mid-animation: two caught `brush_of`
  "nullptr instance" errors then a raw C++ throw (2026-07-14). Mitigated with a 3-failure fuse in
  `discover.lua brush_of`; still, avoid F7 during screen transitions/animations.
- R2 picker: if the boot log shows `hooked=false`, pad blocking is off (read-only) — needs a
  GetProcAddress/inline-hook fallback.
- After any game patch, re-verify offsets in `native_offsets.lua` via the F4 probe.

**Detailed history:** see PROGRESS.md.
