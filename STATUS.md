# STATUS — Dragon Ball Z Kakarot

> Per-mod status ledger / dashboard. Open this first when resuming the mod so progress isn't re-derived from the code each session. Keep it short — a dashboard, not docs. Update the **Next step** line and the section table whenever you finish a chunk. Derive every value from the game's real data — no guessed offsets.

**Last updated:** 2026-07-16

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
| Dialog / message / confirm popups | done | `screen_dialog.lua` (incl. reward/emblem content pools, gift picker). 2026-07-16: notice latch (`spoken`) now survives off-screen BLINKS (cleared only after 3s continuously off) — the parked reward window (`Xcmn_Win01_C_0`, ALL visibility signals true 20 min after closing) flickered with every R1/L1 page flip in the tutorial emblems grid and re-announced "Recibiste al líder… Goku" on each blink. Pending in-game verify |
| Tutorial guidance line | wip | `guide_watch.lua` (NEW 2026-07-16) — the pinned instruction box ("Después de seleccionar a Gohan, oprime A…"): Xcmn_Win01 reused with the text in the RICH side (`ExMainTxt`) of `WL_TxtDetail`/`WL_TxtHelp`/`WL_WorkText` (plain `mainTxt` keeps STALE text — dump-proven), so only rich is read, gated on the rich block visible+non-empty. Keyhelp_watch pattern: registry dispatcher, queued, text-keyed latch that deliberately survives screen changes. Pending in-game verify |
| NPC subtitles / dialogue | done | `screen_dialogue.lua`. 2026-07-15: `Xcmn_Subtitles_C` now gated on the game's OWN subtitles option — `ATSaveSystem.Option.EnableSubtitle` (reflected property read, FAIL-OPEN if unreadable); `Field_Talk_Win_C` (dialogue box) never gated. Pending re-verify with the option off |
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
| Quest objective HUD (text) | done | `quest_objective.lua`, VERIFIED in-game 2026-07-15 night. Two-step fix: (1) directory-mapped `{"fm","QuestNavigation"}` (detection was scan-starved); (2) single-objective quests put the text in the TITLE node (`Txt_Main00`/`WL_MainQuestListTitle`) with every M/S row hidden (F10 dump proved it) — groups now speak on title OR rows. F10 diagnostic kept but `DUMP=false` |
| Episode title cards | wip | `screen_questcard.lua` (NEW 2026-07-15) — `AT_UIQuestMainStart.TitleText` (0x3E0) via `{"fm","QuestMainStart"}` (0x558); telop pattern (once per appearance, queued), registered below telop. `fm.QuestMainLogo` is image-only (ChapterTitleImage) — unread, by design. Pending in-game verify |
| Cooking menu | done | `screen_cooking.lua`, VERIFIED in-game 2026-07-15 night end-to-end (entry menu via the second `Shop_Top_C` chain; dish list; cook — latch spoke "Bollo jugoso al vapor" legitimately). HONEST CAVEAT from the latch log: the ghost pane read `vis=0 opacity=1.0` — `pane_live` did NOT discriminate it; the shadowing was actually killed by the yields (ring/entry-rows) + spoken-key suppression + the game parking the pane a while later. `LATCH_DEBUG` stays ON (one line per activation) to catch any residual window (e.g. emblems right after cooking) |
| Fishing minigame | done | `screen_fishing.lua`. RE-VERIFIED in-game 2026-07-15 (user landing fish consistently) after FOUR fixes that day: (1) directory regression — `AT_UIBattleRushSpeedCore` mapped via a pointer the game never sets → phase 2 dead; unmapped. (2) adapter's own 2 s absence backoff on a ~3 s hook bar → phase-1 cue late/absent since forever; removed (throttling is ui_core's job). (3) the game ALTERNATES between several pooled ring cores — the single cached_live pin was stale half the reels (vis=false, ringSize frozen; caught in the dump); now `ring_core()` picks the on-screen pool instance. (4) reel is <1 s (~420 u/s) and both buttons are random per catch → speech redesigned: phase 2 = bare letter only, on the phase byte (`fishing.phase == 2`), first tick; the "X, luego Y" pre-pair removed (the second letter was the stale core's). DEBUG off |
| Soul Emblems grid / Community | done | `screen_community.lua`. 2026-07-15 saga, ALL VERIFIED in-game by night (entry on the normal path ~5 s first visit of a session, then instant; MOVEMENT verified — the native commuGrid cursor IS driven in the menu flow on `Start_Commu_Emb_C`, gridcurs dump; `GRID_DEBUG` back OFF): (1) unmapped from the directory (no trustworthy owner — two flows, `MenuSoulEmListIns` = `USoulEmblemMenu` reflects nothing); (2) menu-flow instance is the BP class **`Start_Commu_Emb_C`** (census) — `grid_host()` scans both names; (3) the GHOST BOARD was claiming the screen (`BOARD_LIVE_MODES` gate + `Core.pane_live` fixed it — screenshot 98); (4) "reads only after reload" = stale pool + parked-first pick — `grid_host()` enumerates the pool picking the live instance, and a ghost board with no live grid forces ONE budgeted rescan per visit. **ENTRY READS on the normal path now (user-verified)**. 2026-07-16: the ~5 s FIRST-visit lag (never-seen class waiting out ABSENT_BACKOFF; no ghost-board signature exists yet then) fixed with an ENTRY SIGNAL — the game's lazy menu controller (`mm.m_xSoulEmblemMenu` @0x158 / `cm.MenuSoulEmListIns` @0x80, both reflected; the controller's WIDGET pointer is not, AT.hpp:43512, which is why the class can't be directory-mapped) flips null→valid on first open and arms `Core.watch_for("Start_Commu_Emb_C")` (~400 ms budgeted re-scans, ~5 s cap, cleared when the grid reads); the ghost-board path arms the same lane instead of the old single-shot refresh. PENDING in-game verify (fresh session → open emblems: should read in ~1 s, log line "soul-emblem menu controller appeared"). STILL PENDING: cursor MOVEMENT verify (`GRID_DEBUG=true` writes `gridcurs` lines to `dumps/dump_community.txt` — if movement is silent, that dump says whether the native commuGrid offsets are driven in the menu flow). Bonus fact: each slot (`UAT_UIXCmnEmb_Cursor`) reflects `UnlockState` u8 @0x408 |
| Community Board cursor (story tutorial) | done | Verified in-game 2026-07-04, unblocked story; offsets in `native_offsets.commuBoard` |
| Story / battle results | wip | `screen_results.lua`, `screen_battleresult.lua` (rank from brush textures). 2026-07-15: constant-"222" value bug — detail rows have NO reflected numeric members (value likely in the 0x3C0..0x418 unreflected tail; digit images may share one atlas texture) — `DEBUG=true` in screen_results dumps per-digit brush textures to `dumps/dump_results.txt` on the next real results screen; fix follows the dump |
| Quest navigation radar | done | `nav_tracker.lua` + `audio_bridge`; auto-tracks quest markers, arrival cue confirmed. 2026-07-15: battle-interruption resume — a world-gate/transition drop of a MANUAL pick stashes `resume_pick` (plain data) and re-acquires it by category+key when the world returns (10 tries, ~3 s apart); the quest auto-scan stays quiet while pending; cleared by B / F3 off / a new pick. Pending in-game verify |
| Radar categories 2.0 (sites/enemies/collectibles) | done | Verified in-game 2026-07-15 (user: "funciona perfecto") |
| R3 radar target picker (modal) | done | Verified in-game 2026-07-15 together with the categories batch (bind is R3 — early docs said "hold R2", stale) |
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
| Subtitles ON/OFF option | `ATSaveSystem.Option.EnableSubtitle` (int32, 0=off) — both reflected UPROPERTYs (`UATSaveSystem.Option` = `FATSaveSystemOption` @0xB0, `EnableSubtitle` @0x1C inside); find the instance with FindAllOf("ATSaveSystem") skipping `Default__` | ATExt.hpp:815/4670 |
| Quest HUD / episode-card fields | `UIFieldManager.QuestNavigation` 0x568 (`Quest_Navi_C`), `.QuestMainStart` 0x558 (`TitleText` 0x3E0), `.QuestMainLogo` 0x700 (image-only) | AT.hpp (2026-07-15 sweep) |
| Soul-emblem grid slot lock | `UAT_UIXCmnEmb_Cursor.UnlockState` u8 @0x408 (reflected), name text `Txt_Commu` @0x3B0; grid = `AT_UICommunityStart.EmbList.EmbAry`; the MENU-flow owner (`cm.MenuSoulEmListIns` = `USoulEmblemMenu`) reflects NO widget field → class must stay UNMAPPED | AT.hpp:37780/31730-31790 |
| Battle-result detail values | `UAT_UIQuestMainClearDetail` reflects NO numeric members — digits only as `Image_PercentageList` textures; real values presumably in unreflected tail 0x3C0..0x418 | AT.hpp:35209 |
| Message window text nodes | `UAT_UIGameWindow` (Xcmn_Win01 base): `WL_TxtTitle` 0x468, `WL_TxtDetail` 0x470, `WL_TxtHelp` 0x4E8, `WL_WorkText` 0x4F0 — each an `Xcmn_MultiLineText` wrapper with PLAIN `mainTxt` + RICH `ExMainTxt` inners; the game reuses the window across notices/tutorial boxes and the plain side keeps stale text with every visibility signal still true (no reflected layout/live state — unreflected tail 0x640..0x6B8, Ghidra if ever needed) | AT.hpp:33299; dumps 2026-07-16 |
| All other native offsets / class names | — | See `native_offsets.lua`, `dumps/`, and `code/` (Ghidra) |

## Next step
**2026-07-16 afternoon batch: the COMMUNITY STORY TUTORIAL (pick Gohan → place next to Goku),
CODED, pending in-game verify.** The user's report: every R1/L1 press in the emblems grid
re-announced "Recibiste al líder de la comunidad… Goku" and the grid selection never read.
Three defects, three fixes (dumps 11:33-11:34 + screenshots 99-102 + `dump_nav_targets.txt`
directory-trace snapshots were the evidence):
1. `screen_dialog` — the parked reward window (`Xcmn_Win01_C_0`) keeps ALL visibility
   signals true (vis/Visible/op=1/inVP=true, census line 791) 20 min after closing, and
   blinks off/on-screen per page flip; each blink cleared the `spoken` latch → re-announce
   loop. Latch now survives blinks (`SPOKEN_GRACE_S=3.0` continuous-off before clearing;
   the empty-read branch no longer clears either).
2. `guide_watch.lua` (NEW) — reads the tutorial instruction box (rich `ExMainTxt` of
   `WL_TxtDetail`/`WL_TxtHelp`/`WL_WorkText`; plain is stale by design), queued behind the
   active screen's readout, once per text change.
3. `screen_community` — board→grid handoff (mode 10) with no grid slots now ARMS the watch
   lane fresh + renews while the handoff persists (capped `WAIT_RENEW_S`); the story
   tutorial had NO other entry signal (no ring close, no ghost board), so the session's
   first grid sat out the never-seen backoff exactly while the tutorial said "pick Gohan".
Plus `discover.lua`: the rich-text sweep now prints FULL owner paths (the truncated form
hid the instruction box's owner all session — next F7 pins it definitively).
VERIFY (fresh save start or any community tutorial): (a) reward popup reads ONCE, never
again while paging the grid with R1/L1; (b) on entering the grid the instruction
("Después de seleccionar a Gohan, oprime botón A…") reads after the grid's own readout, and
re-reads only when the tutorial step changes; (c) grid slots read within ~1s of the grid
opening from the board (watch lines in the log if ENTRY_DEBUG re-enabled); (d) regression:
normal dialogs/confirmations still read, autosave notices still read on each REAL
appearance (>3s apart), cooking/emblems/pause unaffected. If the instruction does NOT
read, take one F7 on that screen — the rich sweep now names the real owner; adjust
`guide_watch.MEMBERS` (or its host class) from that dump instead of guessing.

**The Skill Tree is CLOSED — do not reopen it.** The "requiere nivel N" reason was deliberately DROPPED
(user, 2026-07-14): pressing A on a locked node makes the game state the requirement itself, in the message
window the dialog reader already speaks, so the tri-state ("bloqueada" / "adquirida") is all the reader
needs. The RE for it is recorded in *Derived facts* if that ever changes — don't re-derive it.

**The 2026-07-15 batch is CLOSED except two items** (user, end of night: "todo funciona bien" —
quest HUD, level-up toast path, radar resume, subtitles gate, episode cards, cook-NPC menu +
latch saga, Soul Emblems grid entry AND movement all verified in game). Remaining:

1. **Battle results "222"** — waiting on ONE battle played to the results screen;
   `screen_results.lua` has `DEBUG=true` and will write `Scripts/dumps/dump_results.txt`
   (per-digit brush textures). Diagnosis so far: detail rows reflect NO numeric members
   (AT.hpp:35209) — if the dump shows one shared atlas texture, the value needs the native
   tail (F4/Ghidra); if distinct textures, fix the token parse. Turn DEBUG off after.
2. **FEATURE: d-pad snap navigation for the Community BOARD** (move the cursor socket-to-socket
   with the d-pad instead of the free left stick) — needs a WRITE primitive (mem_bridge poke API
   or input steering), a DLL rebuild and a full restart: a design session of its own. All the
   socket/cursor data is already mapped (see the Backlog bullet).
3. **Emblems-grid FIRST-visit latency — RUN 2 (2026-07-16) found three more defects, fixed,
   pending verify.** (a) ~~screen_loading ghost~~ — RETRACTED by the user: those 10s WERE the
   real loading screen (ring opened after, read fine at t=65.8). The pane_live gate + LOAD_DEBUG
   stay as rule-compliance/prophylaxis, but the REAL run-2 news is: the soul-emblem controller
   is created DURING the save load (edge t=62.69 mid-loading-screen), so the controller edge is
   NOT an entry signal on save-load sessions — the re-arm window (c) happens to cover a
   straight-to-emblems flow (<30s), but a late visit needs the ring arm. AND the ring-close arm
   didn't fire despite the user browsing to the emblems row — consistent with the game resetting
   the depth flag during the close animation (last poll reads ring level, sid 0, clobbering the
   cache) → the arm now uses a RECENCY grace (`EMBLEM_ARM_GRACE_S=2.0`: emblems row focused
   within 2s of the close) + `RING_DEBUG` one-liner per ring close (sid + focus age) to confirm.
   **RUN 3 (13:45 session): FIRST entry read in ~1.5s after confirm** (widget born t=77.3, gates
   live 77.84, commit 77.86 — detection ~0.4s: solved). Remaining, fixed same day: (d) RE-ENTRY
   silent — no arm exists on a re-visit (controller never re-edges, edge_clock cleared, ring arm
   was still sid-clobbered, ghost-board path needs a parked board this save doesn't have; stale
   pool → 30s refresh) — the ring grace arm (c-fix) is the cover, now in place; (e) the edge
   re-arm loop scanned 21s straight (controller is born during the SAVE LOAD, so "flow in
   progress" held through tutorial+ring) → re-arm now ALSO gated on `Registry.active_adapter()
   == nil` (display idle = the real waiting-for-widget gap); (f) the pane_live gate on
   screen_loading REVERTED — the run-2 "ghost" was a real 10s load (user), and with the gate on
   the loading adapter never activated in run 3 (non-interactive overlay ≠ Visible visibility —
   pane_live rejects it by design; recap reader would be dead). VERIFY: enter (fast), exit,
   RE-ENTER (the new case — expect "ring closed sid=... armed=true" then watch lines then
   commit ≤1s), plus one save-load to confirm the recap still reads.
   (g) user felt NAVIGATION LAG SPIKES with the reads now fast — the watch cost: two classes
   at 400ms each = one 65ms FindAllOf per 200ms (~30% game thread) during windows (incl. false
   arms on backing out). Tuned: `WATCH_EVERY` 4→8 per class + `watch_for(cls, ticks, delay)`
   stagger (fresh-arm only — a renewal must never push watch_next or the class never scans)
   interleaving the two classes at ~400ms combined, `WATCH_TICKS` 5s→3s (long constructions
   are covered by RENEWAL, not window length). Expected: reads ~0.8-1.2s, spikes halved and
   capped at 3s per arm. If still felt, next lever is WATCH_EVERY 8→10 + accepting ~1.5s reads.
   (h) stutters persisted in GAMEPLAY AND COMBAT → the renewal loop was anchored to the
   CONTROLLER edge, which is born during the SAVE LOAD — so it scanned through the first ~30s
   of post-load free roam; and a back-out false arm kept renewing through COMBAT (minimap
   hidden there + no registry adapter active, so neither old gate stopped it). Final shape:
   the controller edge arms NOTHING (diagnostic print only); renewals anchor to `wait_clock`,
   set ONLY by a real arm (ring-close grace / ghost board) and CANCELLED by positive gameplay
   evidence — `Core.free_roam` (minimap) OR `battle_hud_up()` (Battle_Hud_P_Main_C via the
   directory, pointer reads) — cap `WAIT_RENEW_S=30`. A false arm now costs 1-2 scans. If any
   stutter remains after this, STOP GUESSING and profile: play a few minutes, Ctrl+F5, read
   `findall scans` / `ui step ms`.
   (b) the first-of-session menu flow materialized the NATIVE-named instance
   (`AT_UICommunityStart`), not the BP one — the watch covered only `Start_Commu_Emb_C` (13
   scans of the wrong pool) → `GRID_CLASSES` + watch_grid()/unwatch_grid() cover both, in
   community (edge/ghost/clear) and field (ring-close arm). (c) the widget genuinely does not
   EXIST until 5-13s after the controller edge (tutorial popup in between) — one 5s window
   expires mid-flow → menu_entry_signal now RENEWS the watch while the controller exists and
   no community mode has read, capped at `EDGE_REARM_S=30` per edge. Detection itself is
   PROVEN instant now: gates flipped live → commit in the same tick (75.63→75.65). VERIFY:
   fresh session → emblems; expect ring readable right after load (loading ghost gone),
   watch lines on BOTH classes, commit within ~1s of the widget existing; the residual wait
   = the game's own construction (tutorial + assets), unfixable — if it still confuses,
   consider announcing "cargando" on the edge. Then turn OFF: ENTRY_DEBUG, TRACE_COMMITS,
   LOAD_DEBUG. The instrumented timeline (edge 13:13:17, commit +24.46s,
   ZERO watched scans, ZERO gate lines) decomposed into THREE defects: (a) the
   controller-existence edge fires when the ring's Community SUBMENU opens — ~20s before the
   user confirms — so the 5s watch expired mid-browse (the edge is now just a fallback; the
   PRECISE arm is screen_field: ring closes while the focused row's sid == COMMUNITY_EMBLEM(8)
   → `watch_for`); (b) watched scans were BUDGET-STARVED by sweep position (Commu is adapter
   #128; earlier adapters drained the 2/tick budget on contended ticks — the 10ms-time-gate
   lesson, budget edition) → watches are now pumped from `begin_scan_tick` BEFORE the sweep
   (`service_watches`, transition-gated, first claim on the budget); (c) the ENTRY_DEBUG gate
   lines never printed — declared BELOW grid_host, so inside it they resolved as nil GLOBALS
   (Lua upvalue rule) — moved above. Plus `TRACE_COMMITS=true` in ui_registry (one line per
   screen commit, adapter index) to name any shadowing adapter. VERIFY (restart not strictly
   needed for the ring path — reload ok — but a fresh session tests the real first-visit):
   ring → Comunidad → Emblemas de alma; expect log "ring closed on Soul Emblems row" at
   confirm, "watch Start_Commu_Emb_C: N found" lines within ~1s, "emb gates" lines, and
   "emb grid commit +~1s". Then turn OFF: ENTRY_DEBUG (screen_community), TRACE_COMMITS
   (ui_registry) — the watch-scan print in ui_core can stay (bounded, event-driven).

**FISHING is CLOSED** (re-verified end-to-end 2026-07-15; the four fixes and the pooled-ring-core
lesson are recorded in the section table row — the reusable rule: a QTE-style overlay class may have
SEVERAL pooled instances the game alternates between, so never pin one `cached_live` ref; enumerate
the pool and take the on-screen one).

**Verify the 2026-07-15 SCREEN DIRECTORY in game** (FULL RESTART — `ui_directory.lua` is a new
module). This is the fix for "submenus take >4s": items / palette / skill tree / characters / save-load /
status must start reading **immediately** on entry (the pause ring and battle pause were already fast).
Also: shops, maps, community, dialogs, battle HUD, loading tips, minimap radar (nav) — all now resolve via
the directory; if any of those went SILENT, a field name in `ui_directory.MAP` is wrong for that screen
(mapped classes never fall back to scans — check the map against AT.hpp, don't add scans back). Ctrl+F5
after some play: `findall scans` n should be near-zero in steady state and `ui step ms` single digits.

Then the remains of the 2026-07-14 EVENING batch (same restart covers it). It has two halves:

1. **Crash migration COMPLETED.** The `Core.array_of` migration was only half-done (audit found raw TArray
   reads in 10 more files, two of them hot on every session: `ui_archetypes.platbtn_token` — used by the
   keyhelp bar on EVERY screen — and `screen_dialog`). All migrated + luac-validated. Verify: play normally
   (dialogues, results, community board, cooking, shops, fishing prompts); expect NO 0xe06d7363, no
   `UObject instance is nullptr` in the log.
2. **Performance restructure** (against the MEASURED 40 ms avg / 153 ms max registry step — Ctrl+F5
   2026-07-14, `adapter_index=33`): sticky-active registry (full 33-adapter sweep only every 3rd tick while
   a menu is active), per-tick `on_screen` memo, `SCANS_PER_TICK` 3→1 + a 10 ms step time gate + jittered
   backoffs, the **resurrect probe** (`StaticFindObject` by recorded path — O(1) — re-finds destroyed
   screens in ~300 ms instead of the ~4 s `DEAD_BACKOFF`; log line `probe resurrected <cls>`), ONE shared
   20 ms pad scheduler (`pad_poll.lua`) replacing the radar/map/status loops (150→50 game-thread
   dispatches/s), `aim_watch` every 3rd tick, and `battle_monitor` now owns a scan-budget window.
   Verify, in this order: (a) free-roam feels smooth and cinematics no longer stutter audio; (b) pause /
   items / palette start reading in well under a second (watch for `probe resurrected` in the log — if that
   line never appears the probe missed and re-detection is on the 4 s fallback); (c) menus still all read
   (the sticky registry must not silence anything — overlays like dialogs may take ~0.5 s max); (d) battle
   HUD announcements still work (battle_monitor); (e) R3 radar picker, map-travel d-pad and status-page
   d-pad all still respond (they share `pad_poll` now); (f) press **Ctrl+F5** after some play and read the
   dump: `ui step ms` should be single-digit avg (was 39.84) and the new `findall scans` line attributes
   scan cost. If avg is still high, the dump now has the data to say why.
   The raycast arity error in the log was ONLY the Ctrl+F5 dump's probe stage (not gameplay); fixed by
   passing the `OutHit` out-param as a table (signature from `Engine.hpp:11636`).

Then: **verify the character STATUS page + the contextual-action reader in game** (one RESTART covers both
— F11/Shift+F11 and Ctrl+F2 are new keybinds in main.lua). On the status page: entering a character should
speak name, level, "Siguiente N", the HP and Ki gauges and BP; the **d-pad down/up** (or F11 / Shift+F11)
should then walk the 7 blocks (PS, Ki, ATQ/DEF cuerpo a cuerpo, ATQ/DEF de Ki, crítico), each as its total
+ base/aumento/comida, in the on-screen order top→bottom. Two things to watch: if the order comes out
scrambled, the block sort had no positions to work with (`Core.slot_pos`) — dump it before changing
anything; and if the GAME turns out to react to the d-pad on that page (it shows only R-stick rotate + X/Y/B,
so it shouldn't), move the binding to LB/RB rather than blocking the pad. For the keyhelp: open the inventory and the character
menu and check you hear the choices ("botón X: asignar", "botón Y: árbol de habilidades", "botón A: usar")
once on entry, and again only when they change. Then the **backlog** below (the radar 2.0 batch + R3 picker were verified 2026-07-15; the
quest-objective HUD verify FAILED — see the batch above, item 0).

## Backlog
All work through 2026-07-14 is COMMITTED and PUSHED (latest: 47df2d2). The narrative of how each
feature was derived lives in PROGRESS.md and in the git log; this list is only what is still OPEN.

- **2026-07-15 batch: 6 of 8 bugs CODED, pending in-game verify** (the per-bug verify script is in
  *Next step*): quest HUD directory-mapped, level-up toast unmapped, radar resume_pick, subtitles
  option gate (`ATSaveSystem.Option.EnableSubtitle`), episode-card reader (`screen_questcard.lua`),
  Soul Emblems grid unmapped (two-flow owner problem — see the section table). Still OPEN below:
  battle-results "222" (instrumented, needs one dump) and the cooking entry menu (needs census).
- **BUG OPEN (user, 2026-07-15): battle results read a CONSTANT "222" for every stat.** Labels and
  rank letters resolve; the VALUE is wrong across all rows/fights. RE (2026-07-15): the detail row
  class reflects NO numeric members — the number exists only as `Image_PercentageList` digit images,
  and a shared atlas texture whose name ends in a digit would produce exactly a constant digit
  (`(%d)$` parse). Real values presumably in the row's unreflected tail (0x3C0..0x418).
  `screen_results.lua` has `DEBUG=true`: the next real results screen appends per-digit brush
  texture names to `Scripts/dumps/dump_results.txt` — read it, then either fix the token parse (if
  digits are distinct textures after all) or pin the native value via F4/Ghidra and turn DEBUG off.
- **CODED (2026-07-15 evening): the cook-NPC ENTRY menu ("Preparar un platillo" / "Salir").**
  It is NOT a new widget: it's the SECOND pooled `Shop_Top_C` (the cooking menu's embedded
  `WL_CookingTop`, AT.hpp:32237) that `screen_shoplist` already read since 2026-07-06 — the
  DIRECTORY mapping (`fm.ShopTop` only) hid it from `cached_all`. Second chain added:
  `{"fm","CookingMenu","WL_CookingTop"}`. VERIFIED reading (user, same evening) — but it exposed
  a LATCH: at the cook NPC the pooled cooking pane stays VISIBLE with its last dish after
  leaving, so `screen_cooking` stayed active ("Cocina, asado con hueso") and shadowed the ring
  pause. THREE-layer fix (round 2 — the free_roam gate alone was NOT enough, because "Salir"
  returns to the NPC DIALOGUE where the minimap is still hidden, and every registry flip
  between the dialogue and cooking re-announced the stale dish): (1) `Core.free_roam(tick)`
  gate (minimap = the game's "menu closed" signal) on screen_cooking AND screen_shoplist;
  (2) flip-flop suppression — `spoken_key` survives reset() (same selection never re-announces
  within a visit; cleared on genuine close / entry-menu / free-roam; F1 → `reannounce()`
  overrides); (3) `Cooking.confirm_ticks = 5` (~0.5 s) so the exit-animation blip never
  commits, plus cooking yields while `WL_CookingTop` shows rows (`A.shoptop_rows`, the shared
  helper screen_shoplist now uses too); (4) round 3 (user: PAUSING after a cook visit spoke the
  stale dish — the ring family is registered BELOW cooking and the minimap gate is moot inside
  menus): cooking/shoplist yield when the ring is GENUINELY open via `Core.ring_open` (the
  live_ring test — on_screen AND GetVisibility()==0; bare on_screen over-triggers on the pooled
  closed ring; screen_field now shares the helper); (5) round 4 (user: it sneaked in again on
  the EMBLEMS menu, which closes the ring → gate moot): ROOT gate `pane_live(host)` — the pane
  must be genuinely live (ESlateVisibility Visible(0) AND RenderOpacity > ~0, both
  pcall-guarded, unreadable = live) — plus `LATCH_DEBUG` (one `[latch]` line per activation
  with vis/opacity/free_roam/ring to `dump_cooking.txt`) so any remaining leak names its state.
  **VERIFIED (user, same night) on the pause ring and the emblems menu** — the pane_live approach
  holds; the actual COOKING flow (dish list + cook + exit) still pending one re-test. The rule is
  now permanent: CLAUDE.md §8 "pooled pane must gate on genuinely live" + memory
  `feedback-pane-live-gate` — apply to every new menu adapter with a pooled host.
  (General lessons: mapping a POOLED multi-instance class to ONE field silently drops the other
  instances — map every holder or don't map; a pooled pane that never collapses needs the
  free-roam cross-check + spoken-key suppression, not a content gate.)
- **WIP (2026-07-15 night): Soul Emblems grid — the GHOST BOARD was shadowing it.** Screenshot 98
  + Ctrl+F5 proved the user was in the menu grid ("EMBLEMAS DE ALMA", cursor on Chiaotzu) while
  the adapter read the BOARD summary ("24 huecos…"): `Start_Commu_Brd_C` reports on-screen with
  its frame in that flow, and any rendered-frame mode ≠10 used to claim "board", so grid_update
  NEVER ran (that's also why no `gridcurs` lines appeared). Fix: `Core.pane_live` (promoted from
  screen_cooking — VERIFIED there in-game same night) now guards the board AND the grid hosts,
  plus the board only owns the screen in its LIVE mode-machine values (`BOARD_LIVE_MODES` =
  7/9/12/13/14/16/17; 5 = closed, other = parked ghost → fall through to the grid). Cursor
  while MOVING is still unverified: `GRID_DEBUG=true` logs raw/col/row/anim-fallback per change
  to `dumps/dump_community.txt` — now that grid mode can actually run, one grid session + that
  dump answers whether the native cursor (commuGrid, mapped on the board flow) is driven in the
  menu flow or the AnimLoop fallback carries it. Turn DEBUG off once verified.
- **FEATURE (user, 2026-07-15): d-pad grid navigation for the Community BOARD** (the free-cursor
  socket board). Blind-friendly navigation: a d-pad press should SNAP the cursor to the nearest
  socket in that direction (treat the board as a grid), instead of free analog wandering. All the
  data exists (native_offsets.commuBoard + `code/decompiled/manual_1414f2ab0.c`): socket positions =
  `panel.PointerCenterOffset` (reflected) + hidden board pos (`panel+0x550/0x554`), cursor pos =
  `WL_PanelCursor.RenderTransform.Translation` (raw read at +0x90/+0x94 — reflection aborts on it).
  Moving the cursor needs a WRITE: check whether mem_bridge has a poke API (F4 memdiff is read-only
  today) or whether steering the game's own cursor input is safer; once snapped, the game's hovered
  cache (`host+0x5D8`) should announce the socket by itself. Register the d-pad stepper on
  `pad_poll.lua` (never a new 20 ms loop).
- **Cooking menu** (`screen_cooking.lua`) — revised, pending re-verify (detail-pane read, markup strip).
- **"View Controls"** (from the battle pause) reads jumbled, and the pause does not re-announce on return.
- Niceties: skill-palette plates 4/7 (structural, cursor never lands there so far); in assign mode the
  A/B press itself is silent until the first cursor move (no signal exists for the press).

## Known issues / open questions
- **2026-07-14 evening — the `Core.array_of` migration is now COMPLETE** (it was half-done: an adversarial
  sweep found raw reflected-TArray reads in 10 more files — `ui_archetypes` `platbtn_token`/`ids_token`,
  `screen_dialog`, `screen_community`, `screen_battleresult`, `screen_results`, `screen_cooking`,
  `screen_shoplist`, `screen_fishing` (one LIVE read in the phase-1 prompt path), `nav_tracker` dump lines,
  `discover.lua`). Every remaining `#x` in Scripts operates on plain Lua tables. Rule stands: **every**
  reflected TArray read goes through `Core.array_of` — new code included.
- **2026-07-15 — the resurrect probe is DISABLED (`PROBE_ENABLED=false` in ui_core.lua). Do NOT re-enable
  by flipping the flag.** It caused TWO freezes, adversarially confirmed: (1) boot hang at the Bandai logo
  (probes during the initial async load); (2) after gating boot/transitions, a MID-SESSION freeze — free-roam
  streams sublevels asynchronously too, so there is NO provably-safe window for `StaticFindObject` from the
  game thread on this game (deadlock/livelock against the loader over the engine object tables; log just
  stops, no crash). A safe version needs a native-side "loader idle" check (mem_bridge) or the lookup moved
  into a native bridge. Menu re-detection is back on the budgeted FindAllOf (DEAD_BACKOFF ~4s) — proven.
  RULE: **no speculative object lookups (StaticFindObject / FindAllOf bursts) at ANY time the engine might
  be async-loading — which on this game is any time.**
- **2026-07-15 — silent submenus (saveload/items/characters/tree/palette) — CONFIRMED cause: the 10ms
  elapsed-time scan gate (removed).** Those adapters sit at indices ~24-31 of the 33-adapter sweep; the
  walk cost alone exceeded 10ms, so the time gate denied their FindAllOf EVERY tick — a time gate starves
  by sweep position, permanently; a count budget drains fairly (served classes back off ~4s and leave the
  queue). `scan_allowed` is pure budget again, `SCANS_PER_TICK=2`. Steady demand ≈0.7 scans/tick < 2.
  Known minor: while a menu is HELD open, sweep ticks are 1-in-3 so effective scan throughput ≈0.67/tick —
  an overlay opened over a long-held menu can detect a few seconds late; self-heals in free-roam.
- **2026-07-15 (round 2) — MEASURED: one FindAllOf = ~65ms on this game (max 272ms), and the absent-class
  re-scan cycle burned 102s of game thread in 5.5min (30%) — `findall scans: n=1576 total_ms=102214`.**
  The per-class FindAllOf polling design is bankrupt here: it is BOTH the free-roam slowness AND the menu
  latency (scans this costly keep the budget contended, so a just-opened menu waits multi-second backoffs).
  Interim shipped: **event-driven scan boost** (`Core.boost_missing`, fired by the registry on every screen
  commit/close — missing pools skip their backoff for ~1.5s) + backoff split (destroyed pools 4s, never-seen
  8s). The boost CANNOT cover ring→submenu (the ring stays active underneath — no commit event), which is
  exactly the flow the user reported still >4s. REAL fix shipped 2026-07-15: **the screen directory** (next
  bullet).
- **2026-07-15 — THE SCREEN DIRECTORY (`ui_directory.lua`): screen detection is now pointer reads, not
  scans.** The game tracks every top-level screen in named pointer fields on persistent managers:
  `PlayerController.MyHUD` → `AAT_GameHUD` (`UIFieldManager` AT.hpp:32792 = field/shop/map/quest screens;
  `UIBattleManager` = battle HUD/results/QTE; `UICommManager`; `UIPause`), the GameMode component
  **`UMenuManager`** (AT.hpp:41837) whose `UBaseMenu` controllers hold every pause SUBMENU widget
  (`m_xItemInventoryMenu.ItemMenu/ItemPalette`, `m_xStartSkillTreeMenu.SkillTreeMenu`,
  `m_xStartSkillCustomize.SkillCustomize`, `m_xCharacterMenu/StatusMenu.MenuUI`,
  `m_xSaveMenu/m_xLoadMenu.m_UIStartSaveLoad`, options/tips/party/root), and the GameInstance
  (`WindowManager` dialogs, `LoadingScreen`). ~40 adapter classes are mapped in `ui_directory.MAP`
  (every field name verified letter-for-letter against AT.hpp — while an OWNER is reachable, a mapped class
  never falls back to scans, so a wrong name = a silently dead reader). `Core.cached_all`/`cached_live`
  consult it first; unmapped classes (pooled rows, keyhelp, headers, title) keep the budgeted-FindAllOf
  path. Roots are found once (FindAllOf skipping `Default__` CDOs — FindFirstOf can return the CDO with
  null fields), cached, cleared on transition, budget-gated and backed off while absent. First in-game round
  (2026-07-15): everything reads instantly EXCEPT three screens, fixed same day — (1)+(2) TITLE-menu
  load-game/options: no field GameMode at the title → no `UMenuManager`, and "owner unreachable" wrongly
  asserted "absent"; the resolver now returns nil there (fall back to the scan path) and only asserts
  absence after actually reading a reachable owner's null field. (3) Soul-emblem list
  (`AT_UICommunityStart`): its only reflected owner (`UICommManager.UIEmbListIns`) resolved but the screen
  stayed silent — UNMAPPED (scan path) until a trustworthy owner exists. Toasts `Info_Log_C` also unmapped
  (multi-instance overflow pool — opus review). SECOND round (options fixed; title-load/emblems still
  silent, items regressed): all consistent with LAZY UMenuManager controllers (null until first open →
  directory falls back) + the scan path being too slow for ring→submenu (the ring stays active underneath →
  no commit → no boost → full ABSENT_BACKOFF wait). Fixes: (a) **pad-press boost** in ui_registry — an
  A/B/X/Y/Start press EDGE while an adapter is active fires `Core.boost_missing()` (the missing event for
  ring→submenu; gated off in combat/free-roam where no adapter is active); (b) `ABSENT_BACKOFF` 80→40 (the
  absent scan set is small now); (c) **Ctrl+F5 dumps a screen-directory trace** (every mapped class, hop by
  hop — open the silent screen first, then dump: the broken link names itself). If a trace shows an owner
  VALID but its field null while the screen is open, unmap that class (the field isn't trustworthy); never
  re-add scans for working mapped classes.
  THIRD round (dump-driven, 2026-07-15): the traces found the real bugs. (1) **The whole hud branch never
  worked**: `FindAllOf("PlayerController")`'s first instance is the MOUNT controller (TwinFootController,
  MyHUD null) in the field and ATTitleController (plain HUD) at the title — `find_hud` now probes every PC
  for the one whose MyHUD has `UIFieldManager`. This was also the REAL cause of round-1's silent soul
  emblems (re-mapped: `cm.UIEmbListIns`). (2) **The pad boost caused a scan storm** (57k scans, 1s spikes:
  every A press during list navigation opened a fresh window, and cached_live's bypass had no
  once-per-generation cap) — fixed: cooldown 1s between pad windows + one bypass scan per gen in
  cached_live. (3) Items CONFIRMED working via directory in the dump (`ItemMenu(ok) ON-SCREEN`, adapter 23
  active; the game NULLs `ItemMenu` on close — ideal open/closed semantics). (4) Title "cargar partida"
  still unidentified: the census v1 anchor saw NOTHING at the title (wrong text class) — census v2 uses 4
  anchors with per-anchor totals; needs one more Ctrl+F5 with that screen open.
  FOURTH round (2026-07-15): **Items was active-but-MUTE** — `screen_list` speaks through the detail pane,
  whose text boxes it finds by subtree-filtering the cached `CFUIMultiLineTextBox` pool; the game REBUILDS
  the items screen on every open (manager field NULLs on close), so the new boxes weren't in the ~30s-old
  pool cache, and a miss set `boxes = {}` which NEVER retried (mute for the whole visit — the "worked once,
  never replicable" report). Fix: `Core.refresh_all(cls)` (pool re-scan due now, budget-gated) fired on the
  host's entry event + `collect_boxes` leaves `boxes = nil` on a miss so it retries next tick. RULE for any
  adapter that subtree-scans an always-alive pool for a REBUILT screen's children: request a pool refresh
  on entry, and never cache an empty collection as final. Census v3 keys owners by the full-name path up to
  `.WidgetTree.` (GetParent walks only slate panels and never crosses UserWidget boundaries — v2 printed
  "(no owner)" for everything). Title-load: pending one Ctrl+F5 with the screen open (census v3 names it).
  FIFTH round (2026-07-15, census v3 + dump_items capture — both conclusive): (1) title load screen =
  **`Start_Save_Load_C`** (BP subclass, GameInstance child; the native-name scan can't see it) →
  screen_saveload scans both names AND the directory now has a **`tt` root** (`ATTitleLevelScriptActor`,
  AT.hpp:14119: `TitleLoadMenuComponent.m_xLoadMenu.m_UIStartSaveLoad` 0x390/0x100/0x180,
  `TitleOptionMenuComponent.m_xOptionMenu.m_UIStartOption`) — title load/options by pointer. (2) items
  intermittent mute = wrapper nodes hold TWO boxes (`Txt_Title00.mainTxt` + empty `SubTxt`) and pool
  iteration order decided which one `collect_boxes` latched → `is_maintxt` preference (mainTxt always
  wins). (3) the 3-4s residue on title-load = pad-boost cooldown (1s) swallowed the second of two quick A
  presses while the first press's boosted scan had already consumed the class credit before the screen
  existed → cooldown 300ms. DEBUG in screen_list back OFF.
- **2026-07-14 evening — performance architecture** (see *Next step* for the verify list): sticky-active
  registry (`ui_registry.SWEEP_EVERY=3`), per-tick `on_screen` memo, 1-scan/tick + 10 ms time gate +
  per-class jitter, resurrect probe (paths derived at runtime from `GetFullName`, never hardcoded; probe
  misses are harmless — `FindAllOf` backoff is the safety net), shared `pad_poll.lua` (any future fast pad
  stepper registers there — do NOT add new 20 ms `LoopAsync` loops). Backoffs now run on wall-clock ticks
  (`os.clock`), not per-adapter counters. Loops that run OUTSIDE the registry step and scan
  (`battle_monitor`, `quest_objective`) must call `Core.begin_scan_tick()` at their step top or the time
  gate starves them. Accepted trade-offs from the review: higher-priority overlay detection ≤ ~500 ms;
  never-seen non-GameInstance classes burn a few futile O(1) probes while absent (sub-ms, bounded).
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
- R3 picker: if the boot log shows `hooked=false`, pad blocking is off (read-only) — needs a
  GetProcAddress/inline-hook fallback.
- After any game patch, re-verify offsets in `native_offsets.lua` via the F4 probe.

**Detailed history:** see PROGRESS.md.
