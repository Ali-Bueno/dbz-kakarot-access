# STATUS — Dragon Ball Z Kakarot

> Per-mod status ledger / dashboard. Open this first when resuming the mod so progress isn't re-derived from the code each session. Keep it short — a dashboard, not docs. Update the **Next step** line and the section table whenever you finish a chunk. Derive every value from the game's real data — no guessed offsets.

**Last updated:** 2026-07-24 (end-user crash + stale-read batch, source-only, UNVERIFIED in game). CRASH: cooking-browse AV = the 07-21 dangling-UObject `__index` class in steady-state menu browsing (pooled ListView/detail recycled on scroll); migrated screen_cooking + shared `A.list_selected_row` to `Core.member`, then swept EVERY adapter and migrated 18 more files with the same naked-fetch-as-call-argument pattern (all luac). UI fixes (round 2 after user retest): (1) quest objective — SETTLE debounce (2 stable polls) so progressive HUD fill / pooled-instance flicker no longer speaks it "several times" or re-narrates on map close; map re-read DEFERRED + queued in `screen_map` so it no longer overlaps the map's area/help readout. (2) TOAST — presence dedup with a ~1.5 s GRACE (Info_Log02 is a flashing banner whose blink re-armed the dedup). (3) the persisting "desbloqueaste superataque" was actually `screen_dialog`'s post-confirm window re-firing once the 24-entry recent-set FIFO evicted its marks — added a `pinned_set` immune to FIFO eviction, cleared only on map transition. NEW: **defeat menu** (`screen_gameover.lua`, Gameover_C → UAT_UIGameover) — was silent (no adapter); reads "Fin de la partida" + the selected row (Reintentar/Cargar/Volver al título) via CurrentSelectIndex (reflected, mem_bridge @0x3E0 fallback).

## Identity
- **Engine / framework:** UE4 (AT project) + UE4SS v3.0.1 — Lua scripts plus C bridge modules (`prism_bridge`, `audio_bridge`, `input_bridge`, `mem_bridge`).
- **Screen-reader transport:** PRISM (`prism.dll` + `prism_bridge.dll` in `Scripts/`, `tolk.dll` fallback backend).
- **Build command:** per-bridge `src/<bridge>/build.ps1` (rebuild only the bridge you touched). Lua is not compiled; validate syntax with `libs/lua54/luac.exe -p <file>`.
- **Game install path:** `D:\games\steam\steamapps\common\DRAGON BALL Z KAKAROT`. Exe + UE4SS at `…\AT\Binaries\Win64\`. RE dumps live there: **`CXXHeaderDump\`** (per-class `.hpp`, the authority on layouts/members — `AT.hpp` is the big one) and **`UE4SS_ObjectDump.txt`** (what Lua can actually reflect). Grep these instead of re-deriving. **Regenerating them (2026-07-21):** Ctrl+H = headers, Ctrl+J = object dump, and **both `LoadAllAssetsBefore*` MUST stay 0** — force-loading reaches the stripped debug blueprint `AutoDebugMainUI_C` and kills the game with `LowLevelFatalError … Could not find SuperStruct` before writing anything. Native classes (`AT.hpp`) are complete from the title screen regardless; only lazily-loaded BP `_C` classes need you to have visited their screen, and they accumulate over a session (one Ctrl+H at the end). **Third source, offline:** `D:\code\tools\repak\pak_index.txt` — all 348,382 pak asset paths, grep-able without running the game (see [ui-and-text-architecture.md](reference/dbz-kakarot/ui-and-text-architecture.md)).
- **Mod install path:** junction `…\DRAGON BALL Z KAKAROT\AT\Binaries\Win64\Mods\KakarotAccess\Scripts` → repo `mod/KakarotAccess/Scripts`; enabled in `mods.txt` (`KakarotAccess : 1`).
- **Run / test:** launch the game with the mod enabled. `Ctrl+Shift+R` in-game reloads the Lua feature adapters + i18n; `main.lua` changes (keybinds, `Mem.init`, `Speech.init`) or a new/rebuilt DLL need a full game restart.

## Section status
`done` = works with the screen reader on; `wip` = started; `todo` = not begun.

| Section / feature | Status | Notes |
|---|---|---|
| Speech pipeline (PRISM) | done | Logs chosen backend on boot |
| Overworld main menu (native selection) | done | `screen_field.lua`, reads via `UAT_UIStartTop` offsets |
| Battle-pause menu (native selection) | done | `screen_pause.lua`, `UAT_UIXCmnPause +0x43C` |
| Defeat / game-over menu | wip | `screen_gameover.lua` (NEW 2026-07-24) — Gameover_C → UAT_UIGameover. Reads "Fin de la partida" + selected row (Reintentar/Cargar/Volver al título). Index = CurrentSelectIndex (reflected first, mem_bridge @0x3E0 fallback — ObjectDump wasn't regenerated to confirm reflection); rows via SelectionWidgetArray / positional List_Bar0N. Pending in-game verify (needs game RESTART: new adapter + native_offsets) |
| Dialog / message / confirm popups | done | `screen_dialog.lua`. VERIFIED 2026-07-16 night after the 21-round saga: per-NODE composition (rendered state via node_rt first, recent-set novelty second, help requires rendered title), row-filtered content folds on titled notices, latched per-appearance choice prompts, transition-epoch clear, emblem reward via the window's own WL_TextCmuCtn. See the Next step closure + git log for the derivation. 2026-07-16 night: verbatim-repeat rewards (Milk emblem) rescued by fresh CONTENT ROWS within ~3s of the appearance edge — pending verify |
| Tutorial guidance line | done | `guide_watch.lua` — the pinned instruction box, read from the RICH side (`Txt_Detail/Txt_Help/Txt_Work` ExMainTxt; plain keeps stale text), every registry tick, F1 repeats. VERIFIED 2026-07-16 night ("oprime Confirmar para…" — abstract Decide glyph spoken as the action) |
| NPC subtitles / dialogue | done | `screen_dialogue.lua`. 2026-07-15: `Xcmn_Subtitles_C` gated on the game's OWN subtitles option (FAIL-OPEN if unreadable); `Field_Talk_Win_C` (dialogue box) never gated. 2026-07-16 night: the gate read a DEAD TEMPLATE — the object array holds several ATSaveSystem instances (_0/_1 pristine EnableSubtitle=1, _4 = the user's real settings) and first-non-CDO picked _0, so it always read 1. Now resolved through the game's own pointer `UATSaveManager.SaveSystem` @0x108 (AT.hpp:29391), manager cached, pointer re-read per query. Pending verify with the option off |
| Boot agreement viewer (EULA / privacy / data analysis) | done | `screen_agreement.lua` — `AT_UIXcmnAgreement` (AT.hpp:37902): ONE viewer cycles the consent documents; every page is a TEXTURE, no readable text exists (two F7 censuses), so it announces a localized screen label + "shown as an image" note; the actions bar comes from the automatic keyhelp read. DOC + PAGE named NATIVELY (2026-07-17 Ghidra, `native_offsets.agreement`): docId @0x5B4 → "Política de privacidad"/"Análisis de datos"/"Acuerdo de licencia", "página N de M" from @0x5A8/@0x5B0, sanity-checked, silent-degrade if a patch moves them. The earlier brush-texture heuristic is REMOVED (piercing nullptr — see Derived facts). Detection = pointer reads: cached `Gametitle_C` → `ActorTitle` → `AgreementDialog` @0x340, fallback budgeted scan; `pane_live`-gated. Registered below screen_dialog — the Accept/Reject consent popup is a pooled `Xcmn_Win01` and belongs to the dialog reader. VERIFIED in-game 2026-07-17 (user): entry, native document naming ("Política de privacidad" / "Análisis de datos") and page N-of-M readout |
| Difficulty / choice lists | done | `screen_choicelist.lua`, `screen_choice.lua` |
| Options / System / Title / Tutorials / Tips | done | `screen_options/title/tutorials/tutorial/tips.lua`. Title 2026-07-17: SETTLE GATE — the boot-check dialogs re-commit the title on every gap and each re-commit re-announced "Menú principal, continuar"; the title now stays silent until it holds the screen 2.5 s with no dispatcher reset (no reflected boot-phase field exists on AAT_Title/AATTitleLevelScriptActor — swept), a cursor move lifts the gate at once, F1 bypasses it via `reannounce()`. Verified in-game 2026-07-17 (announced once, after the boot dialogs; 4 s felt sluggish → tuned to 2.5 s) |
| Shops (food/material/info) + item palette | done | `screen_shop*.lua`, `screen_palette.lua` (verified in-game) |
| Items inventory + Party + Characters | done | Party/Characters done. Items list reads populated categories (via `Txt_Title00` detail-pane live name; reflected index tracks). EMPTY categories: the whole item UI goes STALE (row/detail/visible-count keep the last item), so emptiness is read from the native flag `itemMenu.hasItems = 0x620` (F4-confirmed; 0 = empty) and announced ("vacío") via `screen_list.lua` factory `empty_off` param. Verified in-game 2026-07-11 |
| Item submenu (use-item char select) | done | `screen_itemuse.lua` — A on a usable item → pick who uses it. Reads the on-screen `AT_UIItemMenu.WL_Start_Party_Bars` bar (the selected char; only it animates in): `Txt_Name01` + `Txt_Lv01→Txt_Lv02` level-up preview, with the "choose character" prompt. Registered before the item list reader. Verified in-game 2026-07-11 |
| Save / Load data slots | done | `screen_saveload.lua` — `AT_UIStartSaveLoad`. VIRTUALIZED 3-bar window (`UISaveLoadBar_List`), so pool-position ≠ ordinal. Ordinal from native index `saveLoad.selectedIndex = 0x410` (+1), cursor bar = `windowPos = 0x418` (F4-confirmed over ~11 saves); reads FILLED and EMPTY slots (Canvas_None checked first); SETTLE_TICKS debounce drops mid-scroll frames. Slow re-entry (widget destroyed+recreated → stale class-list cache) FIXED by `ui_core.first_on_screen` churn-force (re-scan a recently-on-screen class immediately, budget-gated). Verified in-game 2026-07-11 (reads all slots, correct index, fast entry, no lag). DEBUG off |
| Character status page (stats sheet) | wip | `screen_status.lua` — `UAT_UIStartStatus`, the sheet you get by confirming a character in Personajes. ALL reflected: `WL_Txt_Name/_Lv/_Title/_Num/_Power/_Power_Num`, the two `UAT_UIStartStatusHud` gauges (`HpGauge`/`SpGauge` → `TextBox_Number`) with their `TxtStatus`/`TxtStatusNum` row TArrays, and the five `UAT_UIStartStatusList01` attribute blocks (`WL_Txt_Power(_Num)` = total, `TxtStatusList`/`TxtStatusNumList` = rows). `WL_Start_Char_List01` is a fixed C array → UE4SS COLLAPSES it (never index it — the `WL_StartCharBarList` lesson), so the blocks come from the class's cached instance list, on-screen ones only, ordered by `Core.slot_pos(w,"Y")`. Entry reads the header (name, level, next, HP, Ki, BP); the 7 blocks (total + base/estado/comida) are walked with the **d-pad down/up** on the gamepad (own 20 ms `Input.read` loop — the 100 ms registry poll misses quick taps — pad only READ, never blocked; gated on `Registry.active_adapter()` so the palette/tree opening OVER the page don't inherit it) and with **F11 / Shift+F11** on the keyboard. Registered below the palette+tree, above the Characters list. Pending in-game verify (needs a full RESTART) |
| Skill Palette / Super Attack equip | done | `screen_skillcustom.lua` — selected slot plate = `SelectActiveBorder` visible AND `BaseBlinkImage` hidden (structural plates 4/7 have both always ON); slot button from plate `ButtonIconImage` → `A.platbtn_name`; empty slot = literal "---" → "ranura vacía"; level/Ki/desc from the detail pane only while it names the same skill (pane lags and goes stale on empty). `SkillListMenu:GetSelectValue()` is DEAD here (frozen 0) — never use it. Verified in-game 2026-07-13 |
| Skill Tree / learn super attacks | done | **2026-07-14: the lock is read NATIVELY** from the game's own per-node state byte (offsets in *Derived facts*), so "bloqueada"/"adquirida" is correct on EVERY node whatever the browsing path — the old KNOWN LIMIT (noted below) is GONE. The name-propagation heuristic survives only as the fallback if the native read fails its bounds/FName self-check. Entry was slow (~30 s) until the feed's storm guard was fixed (see *Known issues*). Verified in-game 2026-07-14. — `screen_skilltree.lua` — `Start_Skilltree_C` < `UAT_UISkillTreeMenu`, ALL reflected (`Txt_Skillname/Txt_Lv_Num/Txt_Energy_Num/Txt_Detail/Txt_Name`); orbs = `WL_Skilltree_Zorb00` TArray of 12 `UAT_UISkilltreeZorb`: entries 1–6 = REQUIRED cost, 7–12 = OWNED. Orb color = POSITION in its 6-orb grid (red/blue/green/purple/silver/gold — verified twice 2026-07-14; textures don't encode color, all named "Ins_Item"). Reads name+lvl+Ki+non-zero cost, desc, owned orbs at the end. "How to acquire" text is NOT on screen while browsing (7-node full-text scan) — it only exists in the post-A message window, which already reads (decision: leave as-is). LOCKED nodes: the ONLY marker the game exposes is the tree cursor's padlock (`UISkillTree.Skilltree_Cursor.WL_ImgIconMicon` visible), and only on a skill's ENTRY (lv-1) node when the skill is unowned — 6 capture rounds proved there is nothing else: no readable panel index/pointer/position (cursor grid coords do live at tree+0x15F8/+0x15FC as grid×95), key-help bar identical on every node, no cover/lock widget on screen; the char-level gate ("need level 10") exists ONLY in the post-confirm message window (dialog reader speaks it). So the lock is PROPAGATED by skill name within a screen visit (`locked_skills`, cleared in reset): once the entry node reads locked, every level of that skill announces "bloqueada". KNOWN LIMIT (accepted): reaching a level-2/3 node WITHOUT passing its level-1 node first announces no lock — unfixable by reflection. Round-7 panel sweep closed the last door: all 108 `WL_Ins_Panel_Cover` are visible always (a frame, not a padlock) and every skill icon is an anonymous `MaterialInstanceDynamic`, so panels can't be matched to skills. The real data (`USkillManager`/`USkillTree`) exposes ZERO reflected functions and owned levels live in private save memory (`USkillSave`) — a full fix needs native RE (Ghidra), deferred. Verified in-game 2026-07-14 |
| Contextual actions (keyhelp) | done | `keyhelp_watch.lua` — the screen's ACTION prompts ("X: asignar", "Y: árbol de habilidades", "A: usar") read once on entering any menu and again only when the set CHANGES (diff-gated, queued behind the screen's own readout). Hangs off the `ui_registry` dispatcher, so every menu (incl. future ones) is covered; passive/time-critical readers opt out with `keyhelp_auto = false` (13 adapters). Face buttons are finally NAMED: `keyhelp.lua` now falls through to `A.platbtn_token` (the palettes' resolver) when the bar's device-INDEXED textures (Btn00..03) can't name themselves. Nav entries ("mover", "cambiar pestaña") are dropped. Ctrl+F2 toggles it (needs a game RESTART — main.lua); F2 still reads the whole bar. 2026-07-17: SAME-PHRASE COOLDOWN (30 s, `os.clock`, survives screen changes) — the dispatcher's screen flips cleared the diff gate and re-announced an identical bar within seconds, and an A↔B phrase alternation on cursor moves re-spoke each move; identical phrases now wait out the cooldown, new phrases speak at once, F2 unaffected. Verified in-game 2026-07-17. Read LEFT-TO-RIGHT as on screen: the bar is a CanvasPanel the game lays out itself, so the widget number is a slot id, NOT a position — the place comes from the slot's `GetPosition()` (its `LayoutData` offsets reflect back as 0.0), falling back to the render transform / ancestors. COST RULE (learned the hard way: the first cut lagged the item + skill-palette menus to a crawl): inside the poll step it may ONLY use `Core.cached_all` (tick passed — a raw `FindAllOf` per poll stalls the game thread) and it polls the bar's LABELS, resolving the glyphs just once, on the poll where they changed. Verified in-game 2026-07-14 |
| Quest objective HUD (text) | done | `quest_objective.lua`, VERIFIED in-game 2026-07-15 night. Two-step fix: (1) directory-mapped `{"fm","QuestNavigation"}` (detection was scan-starved); (2) single-objective quests put the text in the TITLE node (`Txt_Main00`/`WL_MainQuestListTitle`) with every M/S row hidden (F10 dump proved it) — groups now speak on title OR rows. F10 diagnostic kept but `DUMP=false` |
| Episode title cards | wip | `screen_questcard.lua` (NEW 2026-07-15) — `AT_UIQuestMainStart.TitleText` (0x3E0) via `{"fm","QuestMainStart"}` (0x558); telop pattern (once per appearance, queued), registered below telop. `fm.QuestMainLogo` is image-only (ChapterTitleImage) — unread, by design. 2026-07-17 night (user): a story-chapter card in a cinematic ("Detén la invasión saiyan") went UNREAD — this reader was never verified and its `on_screen` gate may be blind like the intro cards' was; TRACE armed (`qcard` lines: both fm cards' on/vis/op/text per state change) — next episode/chapter card in the log decides the fix |
| Cooking menu | done | `screen_cooking.lua`, VERIFIED in-game 2026-07-15 night end-to-end (entry menu via the second `Shop_Top_C` chain; dish list; cook — latch spoke "Bollo jugoso al vapor" legitimately). HONEST CAVEAT from the latch log: the ghost pane read `vis=0 opacity=1.0` — `pane_live` did NOT discriminate it; the shadowing was actually killed by the yields (ring/entry-rows) + spoken-key suppression + the game parking the pane a while later. `LATCH_DEBUG` stays ON (one line per activation) to catch any residual window (e.g. emblems right after cooking) |
| Fishing minigame | done | `screen_fishing.lua`. RE-VERIFIED in-game 2026-07-15 (user landing fish consistently) after FOUR fixes that day: (1) directory regression — `AT_UIBattleRushSpeedCore` mapped via a pointer the game never sets → phase 2 dead; unmapped. (2) adapter's own 2 s absence backoff on a ~3 s hook bar → phase-1 cue late/absent since forever; removed (throttling is ui_core's job). (3) the game ALTERNATES between several pooled ring cores — the single cached_live pin was stale half the reels (vis=false, ringSize frozen; caught in the dump); now `ring_core()` picks the on-screen pool instance. (4) reel is <1 s (~420 u/s) and both buttons are random per catch → speech redesigned: phase 2 = bare letter only, on the phase byte (`fishing.phase == 2`), first tick; the "X, luego Y" pre-pair removed (the second letter was the stale core's). DEBUG off |
| Fishing catch result ("¡BRAVO!" sheet) | done | NEW 2026-07-17 `screen_fishresult.lua` — `Mgame_Result_C` < `AT_UIMiniGameResult`; header+species via TxtCap00/TxtName (BP-tree twins `Txt_Cap00`/`Txt_Name` — both spellings tried), obtained rows via the native `InfoLogBarList` TArray @0x408 (Txt_List/Txt_Num on each bar, census tree names as guarded fallback). fm.MiniGameResult @0x630 deliberately UNMAPPED (fm minigame fields twice proven unset); scan path + QUIET_EXEMPT; notice pattern. ROUND 2 (user: read only after pressing "Siguiente"): the pane_live gate demands GetVisibility()==Visible(0) but passive overlays render HitTestInvisible (Xcmn_Subtitles precedent) — gate now OPACITY-ONLY (fade-ghost still dropped). ROUND 3 (user: still silent; F7 dump_1784307940 shows the sheet + texts on screen): the reader was SHADOWED — the pooled fishing HUD lingers on_screen under the sheet and screen_fishing (registered far above) claimed every tick; "Siguiente" was merely the release. Fix: fishresult registered ABOVE screen_fishing (a notice, it releases immediately) AND screen_fishing yields while Mgame_Result_C is on screen (same feature/flow — also stops flip-flop prompt chatter over the sheet). Roshi token fix VERIFIED in-game same day. Pending re-verify |
| Gameplay toasts (item log + level-up) | done | `screen_toasts.lua`. 2026-07-17: LEVEL-UP BANNER FIXED — the real class is `Info_Log02_C` (bars `Info_Log_Bar_00..04`, text on bar `TextBox` @0x3C0, its ONLY text member); the old `Info_Log_Level_C` exists NOWHERE in the ObjectDump, which is why level-ups were silent forever (F7 census dump_1784302864_002 pinned it). SAME DAY the first cut CRASHED the game while fishing: a naked `bar.Txt00` ARGUMENT fetch (member that doesn't exist on `Info_Log_Bar02_C`, evaluated outside pcall, retried per tick on blank pooled bars) — fixed, fetch inside pcall, TextBox only. Pending in-game verify |
| Cinematic character-intro cards | wip | NEW 2026-07-17 `screen_infoname.lua` — the "Gohan, hijo de Goku" cards: `Info_Name_C` < `AT_UIInfoNameCore` (`NameTxt`/`PopularNameTxt` + `_Large` twins, all reflected), driven by sequencer track `ATMovieSceneCharaIntroductionSection`. fm.InfoName @0x668 deliberately UNMAPPED (same lazy-field risk); scan path + QUIET_EXEMPT. ROUND 2 (user: only a bare "Goku" heard, Gohan/Piccolo silent): rewritten — reads ALL on-screen instances, 2-tick text-stability gate (name/popular animate in separately), opacity-only liveness (pane_live's visibility==0 blocks passive overlays). ROUND 3 (user: cards READ now, but re-announced on every subtitle flip — long-lived cards outlived the 10 s dedup window): APPEARANCE-EDGE dedup — per-text {first,last,spoken} state, speaks once per continuous presence (stability 2 ticks, `spoken` set in update so a pending line survives subtitle ticks). ROUNDS 4-6 (repeats, then silence): SOLVED by the round-6 trace — the pooled `Info_Name_C` class is a DEAD END (4 parked hosts, never any text, never rendered); the REAL display is the fm-owned container `fm.InfoName` (AT_UIInfoName) flipping Collapsed↔rendered per card, `InfoCoreCtn` @0x3A0 → the active core with the texts. Reader rewritten as PURE POINTER READS (no scans; QUIET_EXEMPT entry removed); wall-clock GONE_S=30 dedup + 2-tick stability kept. FINAL round: the 2-3x repeats near subtitles were the SPEECH sink re-queuing the cut card after each subtitle interrupt (adapter provably spoke once — trace) — `Speech.say` gained a `no_requeue` flag for queued ephemeral overlays (speech.lua does NOT hot-reload: full restart). **VERIFIED in-game 2026-07-17 night (user): three cards, one read each, no repeats.** TRACE off |
| Soul Emblems grid / Community | done | 2026-07-17: CHAR_TOKENS `Mrs`/`Mst` were SWAPPED (user: Roshi spoke as "Mr. Satan" on board AND menu) — pak scan + romaji naming (Tpp=Tao Pai Pai): `Mrs`=Muten **R**o**s**hi, `Mst`=**M**i**s**u**t**ā Satan; `Ev_Msn` is a third unidentified token, left unmapped so it speaks raw. — `screen_community.lua`. 2026-07-15 saga, ALL VERIFIED in-game by night (entry on the normal path ~5 s first visit of a session, then instant; MOVEMENT verified — the native commuGrid cursor IS driven in the menu flow on `Start_Commu_Emb_C`, gridcurs dump; `GRID_DEBUG` back OFF): (1) unmapped from the directory (no trustworthy owner — two flows, `MenuSoulEmListIns` = `USoulEmblemMenu` reflects nothing); (2) menu-flow instance is the BP class **`Start_Commu_Emb_C`** (census) — `grid_host()` scans both names; (3) the GHOST BOARD was claiming the screen (`BOARD_LIVE_MODES` gate + `Core.pane_live` fixed it — screenshot 98); (4) "reads only after reload" = stale pool + parked-first pick — `grid_host()` enumerates the pool picking the live instance, and a ghost board with no live grid forces ONE budgeted rescan per visit. **ENTRY READS on the normal path now (user-verified)**. 2026-07-16: the ~5 s FIRST-visit lag (never-seen class waiting out ABSENT_BACKOFF; no ghost-board signature exists yet then) fixed with an ENTRY SIGNAL — the game's lazy menu controller (`mm.m_xSoulEmblemMenu` @0x158 / `cm.MenuSoulEmListIns` @0x80, both reflected; the controller's WIDGET pointer is not, AT.hpp:43512, which is why the class can't be directory-mapped) flips null→valid on first open and arms `Core.watch_for("Start_Commu_Emb_C")` (~400 ms budgeted re-scans, ~5 s cap, cleared when the grid reads); the ghost-board path arms the same lane instead of the old single-shot refresh. PENDING in-game verify (fresh session → open emblems: should read in ~1 s, log line "soul-emblem menu controller appeared"). STILL PENDING: cursor MOVEMENT verify (`GRID_DEBUG=true` writes `gridcurs` lines to `dumps/dump_community.txt` — if movement is silent, that dump says whether the native commuGrid offsets are driven in the menu flow). Bonus fact: each slot (`UAT_UIXCmnEmb_Cursor`) reflects `UnlockState` u8 @0x408 |
| Community Board cursor (story tutorial) | done | Verified in-game 2026-07-04, unblocked story; offsets in `native_offsets.commuBoard` |
| Story / battle results | wip | `screen_results.lua`, `screen_battleresult.lua` (rank from brush textures). Constant-"222" value bug: round-1 dump (2026-07-17, user) CONFIRMED all digit images share ONE atlas material `Ins_Num_Result02` (mix of the shared MaterialInstanceConstant and per-widget MaterialInstanceDynamics) → the digit must be a MATERIAL PARAMETER on the MID. `DEBUG=true` round 2 now also dumps each brush material's Scalar/Vector parameter values + parent to `dumps/dump_results.txt`; decoder fix follows the next results-screen dump |
| Quest navigation radar | done | `nav_tracker.lua` + `audio_bridge`; auto-tracks quest markers, arrival cue confirmed. 2026-07-15: battle-interruption resume — a world-gate/transition drop of a MANUAL pick stashes `resume_pick` (plain data) and re-acquires it by category+key when the world returns (10 tries, ~3 s apart); the quest auto-scan stays quiet while pending; cleared by B / F3 off / a new pick. Pending in-game verify |
| Radar categories 2.0 (sites/enemies/collectibles) | done | Verified in-game 2026-07-15 (user: "funciona perfecto") |
| R3 radar target picker (modal) | done | Verified in-game 2026-07-15 together with the categories batch (bind is R3 — early docs said "hold R2", stale) |
| Radar: Companions category | done | 2026-07-17: `companions` group in the R3 picker (between Characters and Enemies) reusing the Shift+F5 `companions()` collector verbatim (player/SpawnType-enemy/parked-pool exclusion, 300 m cap) + `enemy_display_name` for real names. radar_menu untouched (generic). Pending in-game verify |
| Radar: ghost-enemy fix (hidden actors) | done | 2026-07-17 VERIFIED in-game (user: gone at Goku's house). `enemies_list()` + `companions()` now filter `bHidden` via `char_visible` — the game parks future-story characters hidden nearby. `CurrentHiddenType` is AQuestCharacter-ONLY (AT.hpp:17553): reading it on AT_Character = uncatchable abort, so bHidden is the whole safe check |
| Radar: enemy levels | done | 2026-07-17 SOLVED by LIVE getter-chain decoding (the dump v2 follows the game's own virtual getters in machine code — no Ghidra round needed for the final hop): enemy level = **`ATEnemyStatus+0x390` (int32)**; player differs (`[[si+0x390]+0x328]`, 0x390 is a pointer there) — reader is enemy-only. Cross-checked: resolved 3 on an enemy a level-6 player beat easily (the earlier +0x1C=94 was a per-character id, REFUTED). Announcements re-enabled (proximity alert + R3 picker: "Soldado, nivel 3"). Pending final in-game listen. Chain evidence, HP find (`SI+0x394` f32), GetPowerCompareRank bonus: No reflected level exists (haiku dump sweep); Ghidra (opus, 17 queries) proved the chain — `AttributeComponent`@0x8E8 vtable[0x3E8]() → int32, floor 1 — but the member is statically unreachable (RTTI-stripped, ~200 candidate vtables); PINNED at runtime with the new Ctrl+Shift+F5 dump (2 saves): **`StatusInstance+0x1C`** (player 4/64 per save, enemies 2/5/6/93; corroborated live: SI+0x394 float = current HP fell 500→287 under attack). Reader `enemy_level()` (reflected hops + `Mem.i32`), level baked into enemy `disp` label → proximity alert + R3 picker say "Soldado, nivel 12". Bonus found: reflected `GetPowerCompareRank` (UCharacterAuraComponent, 0–6 stronger/weaker rank) unused for now. Pending in-game verify |
| Radar: objective auto-track (smart radar) | done | 2026-07-17: quest HUD objective-text SIGNATURE diff (quest_objective.lua — title + objective lines, counters excluded; stored sigs survive the HUD hiding in battle, so re-appearing unchanged text never fires) → `Nav.notify_objective_change(kind)` (wired in app.lua) → one-shot auto-scan preemption (`preempt` table in nav_tracker: ~10-scan TTL, `best_candidate` prefers the changed main/sub marker kind). The interrupted manual pick/sweep/resume is stashed (`preempt.stash`, plain data) and B in the R3 menu RESTORES it (nearest remaining item for chainable categories, the exact pick otherwise; B again = real stop). Explicit pick / F3 disarm everything. VERIFIED in-game 2026-07-17 (user: "parece que funciona bien"). 2026-07-17 IDLE RE-ARM (VERIFIED in-game, user): an objective signal while the radar is IDLE (no manual pick, no resume) now also lifts `auto_suppressed` in `notify_objective_change`, so a freshly activated objective is auto-tracked PERSISTENTLY, not just for the ~15 s preempt window — fixes the "wait for Gohan to find the apples, marker spawns seconds after the HUD text changes, radar stayed silent because it was suppressed post-B/post-arrival" case. Manual picks untouched (still preempt-and-stash, B restores). One log line on the idle re-arm |
| Radar: gathering chain fixes | done | 2026-07-17 (user report: collected a spot, radar never advanced): (1) mining/bug-nest/chest icons marked STATEFUL by class name (AccessPoint*/MiningPoint*/PlacementObjectInfo < AAccessPointBase, CXX dump) → the chained sweep advances on InteractState=Taken and taken points drop from the picker; (2) fruit/small-fish spots are `ASpawner*Volume` (NO taken state exists) → gathering picks use the wide ARRIVE_DIST (8 m) instead of the 1.5 m interact radius, so the sweep arms anywhere inside the patch and the walk-away trigger advances it. VERIFIED in-game 2026-07-17 (user, same session as the auto-track) |
| Localization (external TXT + all game languages) | done | `i18n.lua` now overlays editable `Scripts/lang/<code>.txt` on top of the built-in es/en tables (external wins → editable source of truth). 13 base languages (the game's own `ELanguageType`: ja en fr de es it ko pl pt ru zh ar th; es_ES/es_MX→es, zh_CN/zh_TW→zh). TXT format: `key = value`, `#` comments, dotted prefixes `buttons.` `keyhelp.` `header.N` `startlist.N`, `\n` newline, `\s` edge-space (combo_join/controller_prefix). Resolution: ext[lang] → S[lang] → S.en → key. Language override in the config menu (`I18n.force_language`, re-applied by app.start on reload). Verified in-game 2026-07-17 |
| Mod config menu (L3+R3) | done | `config_menu.lua` — screen-reader modal like the radar picker; opened with **L3+R3** in the overworld (gated on `Nav.field_ready()`). Options: audio cues on/off, cue volume 0-100% (±10, samples a ping), radar auto-activate on/off, language (auto + 13 codes). Persists via `settings.lua` → `Scripts/config.txt` (user-editable, gitignored). D-pad up/down move, left/right (or A) change, B closes. Mutex `_G.__KakarotPadModal` with radar_menu; radar ignores R3 while L3 held so the chord never opens the picker. Verified in-game 2026-07-17 |
| Explore-radar toggle (double-R3) fix | done | `radar_menu.lua` — the double-R3 "explore other objects" toggle didn't turn OFF: a double slower than the 280 ms window let the single-tap fallback open the picker on tap 1, and tap 2 only cancelled it (`Nav.toggle_explore` never fired). Fix: window widened to ~400 ms + a slow-double RESCUE (tap 2 within ~680 ms of tap 1, no picker navigation in between, closes+toggles). Verified in-game 2026-07-17 |
| Battle monitor | wip | `battle_monitor.lua` present |
| Cinematics / transition fluidity | wip | 2026-07-16 pass, PENDING in-game verify. (1) ui_core QUIET MODE (`Core.set_quiet`, published by ui_registry from the committed adapter's `scan_quiet` flag — set on `screen_dialogue`): steady-state backoff-expiry scans (~65ms each) defer while subtitles/talk own the screen; boost- and watch-driven scans still run. (2) ui_registry IDLE THROTTLE: with no active adapter the ~33-adapter sweep now runs every SWEEP_EVERY (300ms) instead of every tick, except inside a ~1s HOT window after any pad press or screen commit (menu-open latency unchanged). (3) pad_boost no longer opens boost windows while quiet (mashing A through dialogue was a steady scan drip). (4) pad_poll RELAX: 20ms dispatcher drops to every 5th tick (100ms) while `_G.__KakarotPadRelax` (quiet or map transition). (5) nav_tracker's two raw FindAllOf refreshes (navi icons / AT_Character) defer while quiet. (6) screen_loading content() pool walk throttled to ~300ms wall clock. (7) game INI: `HookAActorTick=0`, `HookBeginPlay=0` (mod registers no hooks; needs game RESTART). Verify: cutscenes + menu↔cinematic flips feel smoother; menus still read instantly; subtitles unaffected |

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
| Agreement viewer doc/page | `UAT_UIXcmnAgreement +0x5B4` docId (0=EULA, 1=privacy, 2\|3=KPI/data-analysis), `+0x5A8` page (0-based, post-flip commit), `+0x5B0` page count, `+0x5AC` requested page. Texture key = `FCrc::Strihash(UPPER("Eula_%02d"/"Privacypolicy_%02d"/"Kpi_%02d"))` into `ImageTextureMap` @0x508. Consent answered-flags: `UATSaveSystem.User` @0x28 → `IsEulaAgree +0x14 / IsPrivacyPolicy +0x15 / IsKpiNotice +0x16 / +0x17,+0x18 W180` (reflected, plan-B). NEVER brush-read this widget's images (piercing nullptr through pcall, killed two F7 dumps) | opus Ghidra RE 2026-07-17 (`code/ghidra/agree_*.java`, evidence `code/decompiled/agree_*.c`; TabRight `FUN_1416eee20`, SetPage `FUN_141700200`, texture loader `FUN_1416daef0`); `native_offsets.agreement`. Beware look-alike cluster `0x1416c2xxx` (tutorial/commu-rank viewer, arrows off-by-8 — NOT the agreement) |
| Subtitles ON/OFF option | `ATSaveSystem.Option.EnableSubtitle` (int32, 0=off) — both reflected UPROPERTYs (`UATSaveSystem.Option` = `FATSaveSystemOption` @0xB0, `EnableSubtitle` @0x1C inside); find the instance with FindAllOf("ATSaveSystem") skipping `Default__` | ATExt.hpp:815/4670 |
| Quest HUD / episode-card fields | `UIFieldManager.QuestNavigation` 0x568 (`Quest_Navi_C`), `.QuestMainStart` 0x558 (`TitleText` 0x3E0), `.QuestMainLogo` 0x700 (image-only) | AT.hpp (2026-07-15 sweep) |
| Soul-emblem grid slot lock | `UAT_UIXCmnEmb_Cursor.UnlockState` u8 @0x408 (reflected), name text `Txt_Commu` @0x3B0; grid = `AT_UICommunityStart.EmbList.EmbAry`; the MENU-flow owner (`cm.MenuSoulEmListIns` = `USoulEmblemMenu`) reflects NO widget field → class must stay UNMAPPED | AT.hpp:37780/31730-31790 |
| Battle-result detail values | `UAT_UIQuestMainClearDetail` reflects NO numeric members — digits only as `Image_PercentageList` textures; real values presumably in unreflected tail 0x3C0..0x418 | AT.hpp:35209 |
| Icon glyph direction (`Btn_Key_N`) | DATA, not code: `CFTextIconData.IconList` (`FCFIconArt {Key, PLAT_P/X/W_Icon brushes}`, CFramework.hpp:694-698) maps IconName → brush whose TEXTURE name carries the direction (`Btn_Key_Dwn` etc., enum `EGCGPlatTexType` GCG_enums.hpp:240-262); exe has NO `Btn_Key_1..6` strings and no switch. Render path: `<inputicon>` → KeyConfigList → IconName → IconList Key → platform brush. Same indexed IconName on keyboard (key literal unrecoverable — speaking the d-pad direction is correct there too) | opus Ghidra RE 2026-07-16 (project `code/ghidra/KakarotAT`); read at runtime in `ui_archetypes.build_bindings` (`iconTex`) |
| Message window text nodes | `UAT_UIGameWindow` (Xcmn_Win01 base): `WL_TxtTitle` 0x468, `WL_TxtDetail` 0x470, `WL_TxtHelp` 0x4E8, `WL_WorkText` 0x4F0 — each an `Xcmn_MultiLineText` wrapper with PLAIN `mainTxt` + RICH `ExMainTxt` inners; the game reuses the window across notices/tutorial boxes and the plain side keeps stale text with every visibility signal still true (no reflected layout/live state — unreflected tail 0x640..0x6B8, Ghidra if ever needed) | AT.hpp:33299; dumps 2026-07-16 |
| Gathering-point classes | fruit/small-fish = `ASpawnerFruitVolume`/`ASmallFishSpawnVolume` (< `ASpawnerItemVolumeBase`, volumes, NO taken state); ore/bugs/chests = `AMineralMiningPointNormal/Rare`, `ATreasureAccessPoint`, `AInsectAccessPoint` (< `AAccessPointBase`, `InteractState` @0x348, Taken = 11); EMapIcon: 5 FISHING, 6 COLLECTING, 7 HUNTING, 8 ORE, 18 FOODSTUFF_COMM, 63 BugNest, 68 VolcanoRockTrader, 69 ShopBug | CXX dump sweep 2026-07-17 |
| Char-icon tokens (soul emblems etc.) | romaji-derived 3-letter tokens on `/Game/Art/UI/Charicon_Ev/Ev_<Tok>NN` textures; `Mrs`=Muten Roshi, `Mst`=Mr. Satan (Misutā Satan), `Msn`=UNKNOWN third char (speaks raw) | pak string scan 2026-07-17 + user report (Roshi emblem); `screen_community.CHAR_TOKENS` |
| Level-up banner | `Info_Log02_C` < `UAT_UIInfoLog02` — bars `Info_Log_Bar_00..04` (`AT_UIInfoLog02Bar.TextBox` @0x3C0); `Info_Log_Level_C` DOES NOT EXIST in the ObjectDump | Info_Log02.hpp + ObjectDump + F7 census 2026-07-17 |
| Minigame result sheet | `Mgame_Result_C` < `AT_UIMiniGameResult`: TxtCap00 @0x478 / TxtName @0x480 / TxtCap01 @0x488 (BP-tree twins `Txt_*`), rows `Xlist_Bar02_NN`; owner field fm.MiniGameResult @0x630 (unmapped, lazy-field risk) | ObjectDump + F7 census 2026-07-17 |
| Cinematic character-intro card | `Info_Name_C` (BP) < `AT_UIInfoNameCore`: NameTxt @0x3A0, NameTxt_Large @0x3A8, PopularNameTxt @0x3B8, PopularNameTxt_Large @0x3C0, In/OutAnim; container `AT_UIInfoName.InfoCoreCtn` @0x3A0, owner fm.InfoName @0x668 (unmapped); data track `ATMovieSceneCharaIntroductionSection` (CharacterID/PopularNameID/IsLargeSize) | ObjectDump + AT.hpp:33663 + ATSpecialTrack.hpp:238 |
| All other native offsets / class names | — | See `native_offsets.lua`, `dumps/`, and `code/` (Ghidra) |

## Next step

**2026-07-24 (b): THIRD crash class — AV mid-COMBAT. Fixes applied and luac-validated; the user left
before testing — WAITING ON THE IN-GAME RETEST (they will report back; if it crashes again they send
the new stack).** Report: "a huge lag spike, then it crashes"
— `EXCEPTION_ACCESS_VIOLATION reading address 0x00000010`. That fault address IS `UObjectBase::
ClassPrivate` (+0x10), i.e. UE4SS resolved a property off a **NULL** object base; the lag spike is
the engine's GC / streaming pass, which is precisely when pooled widgets die under us. Applied:
0. **THE LIKELY ONE — `Core.nonnull(o)`, and CLASS A of the crash ledger is no longer unguardable.**
   The ledger's own note said "WATCH `keyhelp.lua:88` first if a 0x10 AV recurs on an arbitrary
   screen" — this is that recurrence. The blocker was believed to be that no guard exists for
   `FSlateBrush.ResourceObject`, because `ro:IsValid()` is ITSELF the deref that pierces pcall on a
   null resource. The way out: **`GetAddress()` returns the STORED pointer without dereferencing it**,
   so it can be asked safely and answers 0 for a null handle. New `Core.nonnull` does exactly that
   (fails closed) and now gates EVERY brush read: `keyhelp.texture_token` (widest radius in the mod —
   every keyhelp bar on every screen, and keyhelp is re-read on every adapter flip, including the
   subtitle flips that happen constantly during a battle), `screen_results` (x2),
   `screen_battleresult`, `screen_community` face_resource + face_char (the latter also fed a null
   `ro` straight into `array_of`, which asks IsValid), `screen_fishing`, `discover.brush_of`.
1. **`Core.valid` now also rejects NULL-handle RemoteObjects** (`GetAddress() == 0`) — a wrapper can
   answer `IsValid() == true` while holding NULL (CLAUDE.md §8), and the very next member fetch is
   then the uncatchable AV. `GetAddress` only returns the STORED pointer (no deref), so it is the one
   safe pre-check. This is the global fix for the whole class; everything routed through
   `Core.member`/`Core.valid` inherits it.
2. **`battle_monitor.lua` migrated to `Core.member`** — it was MISSED by the 07-21/07-24 sweeps and is
   the only loop that polls (250ms) *through combat*, naked-fetching `hud.Txt_Num_Hp` /
   `hud.Txt_Name_Char` on pooled battle HUDs the game recycles the instant a fight ends. Prime suspect.
3. Same migration for the other naked fetches still live on churn-exposed paths: `nav_tracker`
   `icon_in_use`, `screen_battleresult`, `screen_results`, `screen_map` (incl. a chained
   `host.A.B`), `screen_telop`, `screen_questcard`, `screen_dialog` (choices/item rows/dump),
   `screen_choice`, `quest_objective`.
**`UE4SS.log` OF THE CRASHED SESSION: READ (don't ask for it again — it was preserved, the user had not
relaunched).** Verdict: it stops 50s after boot at `New game mode — transition gate ON`, and contains
**no `Error:`, no traceback, no "UObject instance is nullptr"** through the whole ~40min session. That
RULES OUT the caught-error-retried-per-tick class (2026-07-13 raycast) and the `0xe06d7363` C++ throw
(which always leaves a UE4SS traceback), and CONFIRMS a hard AV that kills the process before anything
reaches disk — consistent with both suspects below, so the log cannot pick between them. (The boot-time
`[PS] Failed to find GMalloc / FName::ToString / Scan failed` lines are UE4SS's normal scan fallback,
present every session — not a clue.) **Retest checklist when the user returns:** full game RESTART (new
adapter + offsets, Ctrl+Shift+R is not enough) → normal combat, ideally a fight WITH dialogue (that is
what makes keyhelp re-read glyphs constantly) → if it crashes, compare the new stack's RELATIVE UE4SS
offsets against today's; if they match, the guards were not enough and the next layer is the world-epoch
invalidation. → then die once and press F7 on the defeat menu for the GAME OVER probe.
**Still naked (menu-only paths, lower churn exposure, NOT yet migrated):** `screen_community` (its
local `read(node)` helper takes the fetch as an argument — ~20 sites), `screen_fishing`,
`screen_shopcmn`/`shopinfo`/`shoplist`, `screen_training` (chained), `screen_tips`,
`screen_tutorial(s)`, `screen_saveload`, `screen_palette`, `discover.lua` (dev tool).
**Durable fix still pending** (unchanged): the world-epoch invalidation described below.

**2026-07-24 (a): the defeat menu still reads NOTHING in game.** Diagnosis: `Gameover_C` is unmapped,
so it needs a SCAN — and the defeat state is exactly the state cinematic quiet mode defers scans in
(gameplay world up, battle HUD gone, no minimap, no user press). Applied: the adapter now detects
through `Core.first_on_screen` (cached_all → the watch lane applies) and **arms `Core.watch_for
("Gameover_C")` on the battle-HUD falling edge** (~8s), plus a one-shot log line when the host IS live
but exposes no readable rows. Open question the fix can't answer blind — whether `List_Bar0N` /
`SelectionWidgetArray` / `CurrentSelectIndex` are reflected at all on this host (the BP dumps empty and
the ObjectDump is gone): a new **F7 "GAME OVER" probe section** in `discover.lua` answers it, and also
hunts the OWNER FIELD (walks gi/HUD/field-manager properties for a pointer to the live host) so the
class can be mapped in `ui_directory` and stop needing scans entirely.

**2026-07-21: TWO crashes reported by an END USER of the release — fixes applied, PENDING in-game
verify.** Both stacks identical in the whole UE4SS portion (relative offsets) = ONE crash: property
`__index` on a **dangling UObject**, raised from our tick running inside the `ExecuteInGameThread`
flush in ProcessEvent (`AT+0x152134`, same frame in both, different callers below). Applied:
(1) new **`Core.member(o, name)`** guarded fetch in `ui_core.lua` + migrated the always-on naked call
sites — `Core.text_of`'s own `node.mainTxt` (the hottest path in the mod), `quest_objective.row_line`,
`keyhelp.helpmsg`, `screen_dialog.choices`; (2) `screen_toasts.lua:58` naked `bar.Txt00` (the
2026-07-17 crash pattern, fixed back then only in the `Info_Log02_C` twin); (3) `screen_questcard.lua`
`TRACE = false` — it SHIPPED ON, resolving two per-level `fm` pointer chains every tick in every state.
**NOT done (deliberate, next durable step):** the world-teardown hole itself. `transition.lua:27-32`
claims no Lua tick runs between teardown and the gate arming — that is FALSE (LoadMap fires dozens of
UFunctions through ProcessEvent, each draining our queue). And "harden `Dir.root_ok('mm')`" is
CIRCULAR (root_ok = `IsValid()` on the dead level's MenuManager). The real fix is a **world epoch** off
a persistent root (gi → LocalPlayer → PlayerController, nulled by the engine BEFORE CollectGarbage)
stamped on every cache; measured as net-negative cost (it also removes `find_hud`'s FindAllOf).
**Also found:** the RE dumps (`CXXHeaderDump\`, `UE4SS_ObjectDump.txt`) are GONE from the game folder
that line 11 points at — regenerate with Ctrl+H / Ctrl+J after setting both `LoadAllAssetsBefore*` to 1
(and back to 0 afterwards). **VERIFY:** normal play + return to title + map changes with no crash; ask
the user for `UE4SS.log` BEFORE relaunching if a third one arrives.

**2026-07-17 (pre-release batch): LOCALIZATION + CONFIG MENU + double-R3 fix — VERIFIED in-game by the
user ("funcionó!") and COMMITTED (release NOT cut yet, per the user).** Four things wanted before the
first release:
1. **Localization to editable TXT, all game languages.** `i18n.lua` now overlays `Scripts/lang/<code>.txt`
   on top of the built-in es/en tables (external file WINS → it is the editable source of truth;
   delete it and the embedded es/en still work). Detection unchanged (game language → base code);
   a config override forces a specific language. 13 languages shipped as `lang/*.txt` (the game's own
   `ELanguageType`: es/en authored from the tables, the other 11 machine-translated for the community
   to refine). Format: `key = value`, `#` comments, dotted prefixes (`buttons.`/`keyhelp.`/`header.N`/
   `startlist.N`), `\n` newline, `\s` edge-space. Loader in i18n.lua (`ext()`/`load_ext`), accessors
   consult ext first. **VERIFY:** switch the game to fr/de/… → mod speaks that language; edit a value in
   `lang/es.txt`, Ctrl+Shift+R → hear the edit.
2. **Config menu (L3+R3).** New `config_menu.lua` + `settings.lua` (persists `Scripts/config.txt`).
   Options: audio cues on/off, cue volume 0-100 %, radar auto-activate on/off, language. Modal like the
   radar picker, screen-reader driven (d-pad move, left/right change, B close). **VERIFY:** L3+R3 in the
   overworld opens it; each option speaks + persists across a restart; cues actually mute / change volume;
   "radar automático off" stops auto-tracking (manual R3 picks still work).
3. **Double-R3 explore toggle now turns OFF.** Root cause: a double slower than the 280 ms window let the
   single-tap fallback open the picker on tap 1, tap 2 only cancelled it, `toggle_explore` never fired.
   Fixed with a wider window (~400 ms) + a slow-double rescue (~680 ms, radar_menu.lua). **VERIFY:**
   double-R3 says "explorar activado", double-R3 again says "explorar desactivado", repeatably.
4. **L3+R3 must not trigger the radar picker** — radar_menu ignores R3 while L3 is held; the two overlays
   share the `_G.__KakarotPadModal` mutex. **VERIFY:** the chord never blurts a radar target.
All syntax-validated (`luac -p`). speech.lua/i18n/settings not hot-reload-sensitive except main.lua bits
(Settings.init, config_menu registration) — a NEW file + main.lua change → **full game restart** the first
time, then Ctrl+Shift+R for tweaks. Not committed yet — verify first.

**2026-07-17: SMART-RADAR IDLE RE-ARM — VERIFIED in-game by the user, committed.** User asked: when an
NPC activates a quest objective (mission 1: waiting for Gohan to find the apples) and the radar is
tracking NOTHING, auto-track the new objective. Root cause of the gap: the fresh-objective preempt
already bypasses `auto_suppressed`, but only for its ~15 s / 10-scan window; when the marker spawns
seconds AFTER the HUD text changes AND the radar was suppressed (post-B / post-arrival), the preempt
expires with nothing found and the idle radar stays silent forever (a manual pick has no such
asymmetry — it keeps its `target`). FIX (`nav_tracker.notify_objective_change`): when idle (no manual
target, no pending resume) the signal also clears `auto_suppressed`, so the base auto-scan owns the
objective persistently regardless of marker lag. Manual picks untouched. Logs one line
("objective change while idle: re-arming auto-track"). VERIFY: play mission 1 (or B a quest marker,
then trigger a new objective) with the radar idle → the new objective should be picked up and
announced even if its marker appears several seconds late. If it STILL stays silent, the cause is
upstream (the objective-text SIGNAL not firing — e.g. a counter-only change, counters are excluded —
or `best_candidate` not recognizing the apple's navi/minimap icon): capture the UE4SS.log around the
moment and check whether the "re-arming auto-track" line printed.

**2026-07-17 (evening): SECOND same-signature AV, on return-to-title — mitigated (opus verdict,
unproven pending next crash).** Differentiator vs weeks of stable title returns: aa1606b made the
`Mgame_Result_C` pool (a PER-LEVEL pooled widget) probed via `Core.first_on_screen` EVERY tick in
EVERY state (fishresult first-in-sweep + fishing's unconditional yield probe); `Core.valid`'s
`IsValid()` on a GC-freed pooled widget is the uncatchable teardown AV (2026-07-04 class — the
GameMode-notify gate leaves a pre-notify gap and a post-grace re-find window). FIX: both probes now
gated on a live/recent minigame (`_G.__KakarotMinigameLive` stamp, 30/60 s windows) — the pool is
never walked across an unrelated title transition. Same pass fixed the two re-read bugs (user):
fishing yields ~3 s PAST the sheet (lingering HUD re-spoke the phase-1 prompt on "Siguiente"), and
fishresult forgets `spoken` only after GONE_TICKS=5 of absence (the close flicker re-read the sheet).
IF A THIRD AV ARRIVES: capture UE4SS.log BEFORE relaunching (it overwrites) — check whether
"transition gate ON" printed before the AV, and get the AT-Win64-Shipping+0x offset; the durable fix
is keeping the flush engaged while `Dir.root_ok("mm")` is false >1 tick (opus's option 2).

**2026-07-17 (later): the notices batch CRASHED the game on its first test (fishing) — FIXED.**
Fatal AV, no Lua traceback: `screen_toasts` evaluated `bar.Txt00` as a function ARGUMENT
(outside every pcall) and `Info_Log_Bar02_C` has NO `Txt00` — the nonexistent-member fetch
is the uncatchable class (IsHidden lesson), retried each tick a blank pooled bar was
on-screen (raycast-storm signature). Opus adversarial review pinned it in one pass and
cleared the other new files; also hardened `screen_fishresult` rows to the native
`InfoLogBarList` TArray. NEW RULE in the crash ledger: member fetches must happen INSIDE a
pcall (never as call arguments), and never assume a member from a sibling class's layout.

**2026-07-17: NOTICES BATCH — coded, PENDING in-game verify.** Five fixes from the user's
morning reports + F7 censuses: (1) Roshi/Satan token swap (`screen_community.CHAR_TOKENS`,
evidence in Derived facts). (2) Level-up banner now read from the REAL class `Info_Log02_C`
(`screen_toasts.lua`; `Info_Log_Level_C` never existed). (3) NEW `screen_fishresult.lua` —
the fishing "¡BRAVO!" sheet (fish species + obtained items). (4) NEW `screen_infoname.lua` —
cinematic character-intro cards ("Gohan, hijo de Goku"). (5) `screen_results.lua` DEBUG
round 2: digit images share one atlas material, so the dump now captures the MID's
Scalar/Vector parameters — the "222" decoder fix follows the next results dump. All Lua
luac-validated and hot-reloadable (Ctrl+Shift+R). VERIFY: hover Roshi's emblem ("Maestro
Roshi"), level up ("¡X alcanzó el nvl. N!"), land a fish (sheet reads species + items),
watch an intro cinematic (cards spoken), open a story-results details pane (feeds the dump).

**2026-07-17 night: SMART-RADAR batch — VERIFIED in-game by the user ("parece que funciona
bien") and COMMITTED.** What was verified: (1) advance a side quest (talk/turn-in) while manually
tracking an item → the radar announces the NEW objective by itself; B in the R3 menu goes back
to the item sweep, B again stops. (2) Finish a story battle that moves the objective → the new
marker is tracked; finish a random battle → the manual pick resumes as before (resume_pick is
part of this flow now: a fresh objective converts it into the B-restorable stash). (3) Gather
at a fruit patch → on walking away the radar chains to the next gathering point; mine an ore
point → chains the moment it's collected, and mined-out points no longer appear in the picker.
Design notes in the section rows; no new keybinds, no i18n changes, luac-validated.

**CINEMATICS-PERF SAGA: CLOSED (2026-07-17, user verified in game).** Loading screens and
cinematics run smoothly; the whole batch is committed (`9503cec` checkpoint) and pushed;
`SUB_TRACE` turned off after the user's OK. The saga took the mod from 20% avg game-thread /
2.3 scans/s down to ~3ms avg / ~0 scans during scenes. Rounds 1-13 below are the archived
story of how.

**2026-07-17: boot agreement-viewer reader added** (`screen_agreement.lua`, see the section
table). Privacy doc verified in-game same day. Then the DATA-ANALYSIS document naming went
NATIVE (opus Ghidra RE, offsets + rationale in Derived facts) after the brush-texture route
proved cursed. VERIFIED in-game by the user same day: documents announce their real names +
"página N de M". The Accept/Reject consent popup after "Confirmar acuerdo" is presumed
covered by the dialog reader (user reported no issue) — if it ever turns up silent, capture
F7 WITH the popup open (the 07-17 dump only caught its stale pooled texts).

**Still-open items (unchanged from before the saga):** results "222", d-pad board nav,
combat false-positive verify, wind tunnel class hunt, radar `resume_pick` in-game verify.
**2026-07-17 (round 13): TWO fixes. (a) The watch STILL armed (16 scans ≈ the user's felt
3-4s audio mini-cuts; log 01:03 shows a FRESH BOOT straight into the cinematic): the
load-confirm / line-advance presses open hot windows INSIDE the scene and the story level's
own board widgets read as a handoff — quiet+hot guards passed those arms. New third guard:
**gameplay recency** — `last_roam_t` (set in Commu.is_active via Core.free_roam) must be
<30s old to fresh-arm; the emblems menu is only reachable from gameplay, and a
boot-into-cinematic session has NEVER roamed. (b) LOADING-SCREEN REGRESSION (user: recap/tips
unread): post-transition state (mm reachable, no minimap, no adapter, no presses) IS the
quiet state and the transition flush empties the pools — nothing could scan, the loading
adapter never claimed, quiet never lifted. `Loading_C` + `Xcmn_MultiLineText_C` added to
QUIET_EXEMPT (nothing-live refinement keeps them free mid-scene). If mini-cuts persist at
scans=0: next suspect is NVDA audio ducking (per-line game-audio dips are NVDA-side).**

**2026-07-17 (round 12): the exempt-refresh fix held (narrative classes gone from the clean
window) but the emblem watch STILL armed (19/19 scans): in the line gaps the PARKED community
board claims the screen (adapter active ⇒ quiet off), so the quiet-only arm guard passed and
its stale mode-10 read armed the lane. FIX: fresh arms now also require **`Registry.hot()`**
(new accessor: inside the ~1s press/commit window) — every legit entry signal is press-driven;
a cutscene has no presses. WATCH ITEM: the parked board CLAIMING mid-cutscene is itself latent
(could speak board text into a scene — no user report yet; if it happens, gate the commu claim
on quiet/world state). Expected next clean window: scans = 0.**

**2026-07-17 (round 11): the CLEAN two-dump window (post-world-predicate) measured 27
scans/62s, all attributed: 20 = TWO SPURIOUS community watch arms mid-cinematic; 7 = the
QUIET_EXEMPT narrative classes' 30s ALIVE-pool refreshes. Fixes: `watch_grid()` refuses a
FRESH arm while `Core.scan_quiet()` (every legit entry signal rides a press whose hot window
lifts quiet); exempt classes bypass quiet ONLY while their pool holds nothing live (an alive
pool serves detection from cache; its refresh waits out the scene). Subs-ON re-read
VERIFIED in the log (00:44 lines). Expected next clean window: scans ≈ 0. Then SUB_TRACE
off + commit. NOTE the residual floor after this: the registry sweep's walk cost (~3.5ms
avg step) + one exempt scan only when a narrative pool is genuinely dead/absent.**

**2026-07-17 (round 10): SUBTITLES-OFF GATE VERIFIED by the user (reader silent, big hitches
gone; subs-ON re-read still unverified) — and the 21:37 dump exposed the LAST hole: 666
scans/109s again, because silencing the dialogue adapter KILLED the dialogue-grace signal the
cutscene-quiet relied on, and free-roam-seen never arms on the user's load-into-cinematic
saves. FINAL DESIGN — no session-history heuristics: quiet (no-adapter branch) =
`Dir.root_ok("mm")` (new ui_directory accessor: the gameplay GameMode's MenuManager root
exists ONLY in playable worlds, never at boot/title) AND no battle HUD AND no minimap AND not
hot. Plus a BATTLE-END hot window (battle_hud_live falling edge → 3s of scans) so the
event-less results screens still read on their own. seen_free_roam / DIALOGUE_GRACE_S
removed. VERIFY: cutscene → F5 → scans ≈0 (only QUIET_EXEMPT strays), mini-hitches gone;
menus snappy; battle → results still announce; title screen still reads after
return-to-title; subs ON → lines read again. Then SUB_TRACE off + commit the whole batch.**

**2026-07-17 (round 9): THE 00:28 DUMP ANSWERED EVERYTHING — two fixes, pending verify.**
(1) SUBTITLES-OFF: lines confirmed from `Xcmn_Subtitles_C` (vis=3 HitTestInvisible, pane_live
false — NOT a usable discriminator since that's likely its normal rendered state) and the probe
showed TWO SaveManagers (_0 → template ATSaveSystem_1 with defaults, _2 → the real _4): every
"first instance" pick is a trap. Gate now walks the game's OWN chain, pure pointer reads:
**gi root → `UAT_GameInstance.SaveManager` @0x1EA0 (AT.hpp:30078) → `.SaveSystem` @0x108 →
`Option.EnableSubtitle`** via `Dir.peek("gi","SaveManager")`, re-resolved per query, fail-open.
(2) STUTTER RESIDUE: 14 scans/12s DURING the scene — cutscene-quiet never armed because the
user's save loads DIRECTLY into a paused cinematic (free roam never seen); the gaps between
lines ran backoff scans. The user's "smooth second after Ctrl+F5" nailed it: the dump's own
census FindAllOfs serve every backoff → ~4s scan-free. FIX: `DIALOGUE_GRACE_S=10` — a gap
within 10s of a scan_quiet adapter inherits quiet (cleared on transition). Loops all measured
innocent now (battle 0.21ms, quest 0.20ms, nav 0.04ms, speech 0.5ms avg). VERIFY (Ctrl+Shift+R):
subs-off cutscene → NO subtitle speech (no `line src=` lines in the log), smooth scene, F5
during it shows scans ≈ 0. Then: SUB_TRACE off, commit. If subs ON: lines must still read.**

**2026-07-17 (round 8): the manager-resolved gate STILL reads EnableSubtitle=1 (log 00:22)
while instance _4 holds the user's real 0 — and the user still feels the cinematic stutter.
INSTRUMENTED THE FINAL BLIND SPOTS (all hot-reloadable): (1) `SUB_TRACE` in screen_dialogue —
one log line per NEW spoken line: source surface (Xcmn_Subtitles_C = gate broken vs
Field_Talk_Win_C = never gated by design), instance name, vis/opacity/pane_live — decides
both the right gate and whether pane_live discriminates option-off; TURN OFF after. (2) F5
probe now prints every ATSaveManager → which ATSaveSystem it points to, and every
Xcmn_Subtitles_C instance's render state. (3) battle_monitor + quest_objective step timing
(__KakarotBattleStats/__KakarotQuestStats, printed by the dump) — the last unmeasured
game-thread loops. NEXT DUMP during a subs-off cutscene answers: who speaks the lines, the
correct save-system chain (else Ghidra/opus: who READS Option+0x1C in the subtitle path), and
whether any loop still costs. If the trace shows Field_Talk_Win_C carrying CUTSCENE lines,
the fix is gating that surface too during cutscene states (respecting its never-gate role in
normal NPC talk).**

**2026-07-16 (night, round 7): BOOST-STORM FIX VERIFIED BY NUMBERS (scans 6.4/s → 0.13/s,
avg step 3.71ms) — and the SUBTITLES-OFF bug SOLVED by the probe.** The 21:17 dump showed
THREE ATSaveSystem instances: _0/_1 pristine templates (EnableSubtitle=1, volumes 10) and _4
with the user's REAL settings (EnableSubtitle=0, VolumeBgm=6) — the gate's first-non-CDO pick
read the dead template forever. FIX: resolve through `UATSaveManager.SaveSystem` @0x108
(AT.hpp:29391, the game's own pointer; manager cached, pointer re-read per query, fail-open
kept). With the option off the reader goes INERT in cutscenes (no lines → no commits → pure
quiet → NVDA silent), which is also the remaining felt-stutter fix for this user. Remaining
scan residue in the window: Xcmn_Win00_Choice_C/Xcmn_Win01_List_C n=14 each (alive-pool 30s
refreshes, outside quiet moments) + QTE n=4 — small. IF stutter persists with subs off and
scans ≈0: instrument battle_monitor/quest_objective loops next (still unmeasured), then
re-run the mods.txt A/B against the CURRENT code. gamestate Subtitles/InMenuSubtitles probe
read null — GameState route discarded.**

**2026-07-16 (night, round 6): THE CINEMATIC STUTTER, FOUND AND FIXED (pending verify) — it
was the COMMIT-BOOST STORM, and speech is CLEARED.** The in-cinematic dump (21:11): speech
calls n=43 total 10ms (hypothesis dead); `findall scans n=572/90s, avg step 34ms` — quiet was
not acting because the subtitles adapter commits IN on every line and OUT in every gap, and
EVERY commit called `Core.boost_missing()` → fresh boost generation → one scan per absent
class (~15) every ~2s, and boost bypasses quiet BY DESIGN. FIX: a commit whose two sides are
nil-or-`scan_quiet` (dialogue cadence, not menu navigation) no longer boosts nor sets the hot
window. Also: the registry's own `battle_hud_live` probe was scanning the absent HUD class
every backoff (n=24) — new scan-free `Core.peek_all` (directory-or-cached-pool, never scans).
SUBTITLES-OFF BUG (user: option off, lines still read; gate logs EnableSubtitle=1): probe
added to the Ctrl+F5 dump — every ATSaveSystem's Option slice (volumes cross-check a shifted
layout) + AATGameState.Subtitles/InMenuSubtitles (0x590/0x598, AT.hpp:14685) render state.
VERIFY: cutscene with subs off → Ctrl+F5 → expect scans ≈0, and the savesys/gamestate lines
pick the correct gate. Menu flips (dialogue→pause, nil→menu) still boost — check menus stay
snappy.**

**2026-07-16 (night, round 5): THE A/B VERDICT — `KakarotAccess : 0` = "súper fluidas" (user).
So the residual cinematic stutter IS this mod — yet the registry step measures only ~5% of the
game thread, meaning the cost lives OUTSIDE what Ctrl+F5 measured. Prime suspect (unique to
cinematics being speech-dense): every `prism.say` runs SYNCHRONOUSLY ON THE GAME THREAD
(speech.lua → prism_bridge → NVDA IPC/SAPI) — a slow backend call = one hitch per subtitle
line, invisible to scan/step stats (say happens inside the step, but its cost was never
attributed). INSTRUMENTED, not yet fixed: `timed_say` in speech.lua (`__KakarotSpeechStats`) and
the nav loop's own step timing (`__KakarotNavStats`), both printed by the Ctrl+F5 dump
("speech calls: …" / "nav step ms: …"). speech.lua does NOT hot-reload → needs the full restart
the user must do anyway (mods.txt back to 1). NEXT: user plays a cutscene → Ctrl+F5 → if
`speech calls` shows max/avg in the tens of ms, the fix is making prism_bridge ASYNC internally
(C-side worker thread queue: speak() copies the string and returns; the worker calls PRISM) —
a DLL rebuild + restart. If speech is cheap, instrument battle/quest loops + audio_bridge next.**

**2026-07-16 (night, round 4): WHOLE-CUTSCENE QUIET — CODED, pending verify. Milk fix
VERIFIED by the user ("ya se lee"). The round-3 residue was named by the per-class dump
(~0.4 scans/s during the cinematic: AT_UIQteMashAlert n=12, Map_World_Icon_C, the community
trio, AT_UIStartDragonBallMenu, Gametitle_C, choice windows — all MENU classes that cannot
appear in a cutscene without a press). Change: quiet now also engages with NO active adapter
when free roam has been seen since the last transition AND the minimap is hidden AND the
battle HUD (directory-mapped, pointer reads) is not live. Guards: any fresh press lifts it
for the ~1s hot window (skip confirm, results advance); `QUIET_EXEMPT` in ui_core keeps the
auto-appearing narrative classes on the normal net (Xcmn_Subtitles_C, Field_Talk_Win_C,
Quest_Main_Telop_C, Xcmn_Win01_C); `seen_free_roam` resets on transition so boot/title and
travel→cutscene sequences keep their scans. Expected next dump during a cutscene: scans ≈ 0.
Verify also: battle results still read at battle end (press A advances them anyway), boot
notices still read.**

**2026-07-16 (night, round 3): MEASURED AFTER THE FIXES — `ui step ms: max=175 avg=3.72 over
3529 ticks`, `findall scans: n=59 total_ms=4569` (~6 min incl. a cutscene, after Ctrl+Shift+R).
avg 20.0→3.72 ms (5.4×), scans 2.3/s→0.17/s (14×). The per-class list is just the post-reload
cache rebuild (n=1 per class) + AT_UIQteMashAlert n=3 — no steady offender left. Remaining
residue: one ~70-100ms hitch per scan tick, worst tick 175ms (a 2-scan tick, SCANS_PER_TICK=2 —
left alone, the "NOT 1" note predates the directory but stays respected until a longer session
argues otherwise). User verdict: "algo mejor". NEXT: (a) A/B — `KakarotAccess : 0` in mods.txt
for one cutscene: if it still stutters, the residue is UE4SS/game baseline, not us; (b) a LONGER
session Ctrl+F5 for a real top-offenders list before touching anything else.**

**2026-07-16 (night, round 2): THE Ctrl+F5 NUMBERS — the mod is still heavy, and now it's
measured.** User's dump (24-min session): `ui step ms: max=518 avg=20.00 over 14226 ticks`
(≈20% of the game thread steady, half-second spikes) and `findall scans: n=3310
total_ms=225612 avg=68.2` (≈2.3 scans/s sustained — ~90 were the community watch bursts,
the other ~3200 are SILENT absent-class backoff scans). Three changes shipped tonight:
(1) **community watch storm fixed** — `maintain_wait` renewed for up to 30s in states with
no minimap and no battle HUD, i.e. straight through cutscenes (the 23:44 log storm);
now also cancels when the active adapter is the subtitles/cutscene one (`scan_quiet`) and
`WAIT_RENEW_S` 30→15. (2) **Per-class scan attribution** in `timed_findall`
(`__KakarotScanStats.by`) + the Ctrl+F5 dump prints the top-12 classes by total ms.
(3) The Ctrl+F5 census nullptr in the log (nav_tracker:2732) is the dump's own pcall'd
probe — harmless noise, ignore. **NEXT SESSION: play ~10 min incl. a cutscene, Ctrl+F5,
read the `scan <class> n= ms=` lines — then directory-map (or slow the backoff of) the top
offenders. That list is the path to getting avg step ms into single digits.** All hot-reloadable.

**2026-07-16 (night): MILK-EMBLEM SILENT REWARD — FIXED, pending in-game verify.** User: the
Milk soul-emblem reward notice never read (everything else fine). Cause (structural, no trace
needed): a SECOND emblem reward in the same notice epoch repeats Txt_Title AND Txt_Help
VERBATIM ("Emblemas de alma recibidos" / "Recibiste los siguientes…") — both recently marked,
so `fresh_notice` returned nil and the notice never activated; the only NEW text (the emblem
row: "Milk, nivel de comunidad N") lives in the CONTENT ROWS, which never counted as freshness.
FIX in screen_dialog: for ~3s after the window's APPEARANCE EDGE (off→on, tracked via
`was_on`/`appear_t`; never on the parked steady state — the rows walk isn't free), a rendered
title + any fresh plates()/emblems_received() row makes the notice fresh (composed = the stale
title; update() folds the fresh rows, recently-spoken rows still drop). The fold latch
(notice_msg/notice_full) clears on the appearance edge so two consecutive rows-only rewards
can't reuse a stale fold. Verify: receive two emblems without a map travel between them — both
read title + emblem name. Session log also showed the community WAIT_RENEW watch bursts
(pairs of 65ms scans every ~350ms for 3-5s per burst, bounded by design) — known cost, left alone.

**2026-07-16 (later): CINEMATICS-LAG PASS — CODED, pending in-game verify.** See the new
"Cinematics / transition fluidity" row for the seven changes (quiet mode, idle sweep throttle,
no boost while quiet, pad relax, nav rescan deferral, loading throttle, UE4SS ini hooks off).
All Lua is luac-validated and hot-reloadable EXCEPT the INI change (full game restart).
Verify: play a story cutscene (subtitles keep reading, no stutter), skip-confirm still reads,
menus still open/read instantly after a press, radar resumes after dialogue ends.


**2026-07-16 NIGHT: the whole dialog saga (rounds 1-21 below) is VERIFIED by the user and
COMMITTED — "el resto funcionó perfectamente".** What shipped, in one breath: notices/prompts on
the pooled Xcmn windows are composed by PER-NODE liveness — rendered state (node_rt/on_screen)
first, novelty vs the recent-set second (flicker only), help needs a rendered title; content rows
fold row-filtered into titled notices; choice prompts use the same composition (latched per
appearance); map transitions clear the notice epoch (Transition.on_begin); Speech re-queues
unfinished queued lines after an interrupt (subtitle vs pickup toast); keyconfig_button resolves
abstract glyphs (Decide/Cancel → action words, Btn_Key_*/Stk_* → device words); the emblem reward
reads through the window's WL_TextCmuCtn TArray. Diagnosis traces are OFF (cooking LATCH_DEBUG and
results DEBUG stay on — separate open items). **ROUND 22 (the Btn_Key_N direction, SOLVED — opus Ghidra RE):
the number→direction pairing is pure DATA (see the new Derived-facts row) — `build_bindings` now
also reads the asset's `IconList` into `iconTex` (IconName → brush texture name) and
`keyconfig_button` decodes the texture name through the shared glyph vocabulary (the four single
directions reuse the localized d-pad button words), so the vehicle guide rows should now speak
"cruceta abajo: Conducir" etc.; generic "cruceta" survives only if the brush is unreadable.
FINAL RESOLUTION (same night, after two failed runtime reads + the user's screenshots): the
IconList brush read is a DEAD END — every brush's reflected ResourceName is None, and ANY member
call on its ResourceObject (even behind a passing Core.valid) raises the pcall-PIERCING nullptr
error from inside the registry sweep (first cut silenced the tips MOD-WIDE and re-threw per tick
by aborting build_bindings mid-way; the gated retry still threw once per reload). AND the
question was MOOT: the user's screenshots (120/121) show the game renders the NEUTRAL whole-d-pad
glyph for the numbered Btn_Key_N ids — generic "cruceta" IS the faithful reading. Shipped state:
IconList read REMOVED (dead-end note in ui_archetypes), named glyph ids decode via glyph_word
(Btn_Key_Ud/Up/Dwn/L/R → localized d-pad words, Stk_* → sticks, Decide/Cancel → action words),
numbered Btn_Key_N → generic "cruceta". Verified: page 1 fully resolved (LT+X combos, sticks,
Start/Select), page 2 matches the sighted rendering.** The round-by-round narrative below is
history.

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
ROUND 2 (same day, ~12:30 — user retest at 11:58-12:01 ran round 1 and was still confusing;
NOT COMMITTED, user gate: "no commit until I confirm it's solved"). The 12:00:45 F7 (with
the new full-path rich sweep) PINNED the instruction: it lives in
**`Xcmn_Win01_C_0.Txt_Detail.ExMainTxt`** (rich side) while the window is GENUINELY live
(inVP=true — it renders the pinned guide box), and `Txt_Detail.mainTxt` (plain) still holds
the stale reward body — i.e. the window is REUSED as the guide box, and screen_dialog was
announcing the stale plain layer once per step. Round-2 changes: (1) screen_dialog YIELDS
when the window shows a rich guide layer and offers no choices (`Guide.showing(win)`) — the
stale plain junk never speaks, the screen underneath keeps reading, guide_watch reads the
instruction (evidence for the discriminator: in both captured moments rich-non-empty
happened ONLY on guide layers; notices carry their markup in the PLAIN mainTxt — dump line
414). (2) guide_watch runs every registry tick (also with NO active adapter — guide boxes
can pin over free roam) and exposes `showing()`/`reannounce()`. (3) **F1 now also repeats
the pinned instruction** (Registry.repeat_current → GuideWatch.reannounce, live read). (4)
i18n board wording aligned with the game's: "panel"/"panel vacío" (was "hueco") — the
instruction says "Con un panel vacío seleccionado…" and the reader must use the same word.
ROUND 21 (same day, ~21:45 — user: everything reads; ONE loose end: the title-screen notices
("Revisando contenido descargable…", "Guardando partida…") no longer read after a MID-GAME
return-to-title, though they read on first boot). Cause: those notices repeat VERBATIM, and after
return-to-title their texts were still inside the recent-set FIFO from the session boot →
suppressed (proof was already in the 20:42:57 log line: msg=Revisando contenido… new=nil). FIX: a
map TRANSITION is a new notice EPOCH — screen_dialog registers a Transition.on_begin flush
(pure Lua) clearing recent/latches/choice state; the blink/flicker suppression never spans a map
load, so it costs nothing. Verify: mid-game → Sistema → Volver a la pantalla de título: the
checking/saving notices read again; the reward/tutorial flow still behaves. Then traces off +
commit (user gate).

ROUND 20 (same day, ~21:30 — user: food confirm read "Ajustes de dificultad, ¿Confirmas que
quieres comer?, Nivel de dificultad estándar., Sí"). The recent-set is the WRONG long-horizon
discriminator: the difficulty box's stale Txt_Title/Txt_Help marks were FIFO-evicted minutes
later and re-composed as "fresh" around the live question. PRIMARY discriminator is now the
node's OWN RENDERED state: title/help reads in message(), fresh_notice() and mark_window_nodes()
go through node_rt (on_screen-gated, like the untitled members always were) — the untitled
layouts collapse the title block, so stale hidden text can never compose again regardless of
FIFO horizon; novelty stays as the secondary (flicker) layer. node_rt moved ABOVE message()
(Lua upvalue rule — the ENTRY_DEBUG lesson). The win01 enum now prints per-node rendered flags
(`ti=...(v0)`) — if a stale title still glues somewhere, the log now PROVES whether visibility
lies on that node. Verify: campfire eat confirm → just the question + Sí/No; reward/bonus titled
notices still read their titles. Then traces off + commit (user gate).

ROUND 19 (same day, ~21:15 — user: subtitle + item pickup at the same time → one of the two is
lost; wants pickup/orb counts always heard). Mechanism: toasts (screen_toasts) already speak
QUEUED (interrupt=false, correct), but every new subtitle line speaks interrupt=true, which
clears the reader's whole queue — the just-queued toast dies silently (and a toast mid-speech is
cut). There is no Lua-side queue, so a flushed line was lost forever. FIX in the SINK
(speech.lua, covers every reader pair): Speech.say now tracks queued lines with an estimated
finish time (same chars/s model as the protection window); on any interrupt=true say, unfinished
queued lines are RE-APPENDED right after the interrupting line (skipping duplicates of it),
capped at REQUEUE_MAX=2 per line so a busy menu can't make a toast immortal. DEBUG_LOG marks
re-queued lines "(requeue)". speech.lua may not be covered by Ctrl+Shift+R (round-9 note: it
survives reloads) — verify with a FULL RESTART: trigger a pickup during NPC dialogue; expect the
subtitle line, then the toast right after it, nothing lost. Then traces off + commit (user gate).

ROUND 18 (same day, ~21:00). The FIELD-CONTROLS tutorial page ("Controles de campo 2/2", vehicle
rows) spoke only ONE row with its button ("Restablecer cámara"/R3) — the rest action-only: those
rows' KeyConfigIds have NO ctrl pairing in CFTextIconData (AirCar_GetOff icon=Btn_Key_4,
Rides_Decide=Key_2, Rides_Select=Key_6, CameraRoll icon=Stk_Nut_R), so keyconfig_button → nil and
markup_to_speech dropped the glyphs. FIX: the icon fallback now decodes the NAVIGATION-glyph
families by their atlas NAME — Stk_(Nut_)L/R → "stick izquierdo/derecho", Btn_Key_Ud/Lr/All →
"cruceta arriba o abajo"/"cruceta izquierda o derecha"/"cruceta" — and the NUMBERED Btn_Key_N
(direction numbering nowhere in readable data) speak the generic "cruceta" rather than risk a
wrong direction. i18n keys stick_l/r, dpad, dpad_ud/lr. Applies everywhere markup_to_speech runs
(tutorials, guides, dialogs). BACKLOG: decode Btn_Key_N direction numbering (Ghidra: how the rich
text picks the PLAT_X texture per icon id) to upgrade "cruceta" → "cruceta abajo" etc. Verify:
reopen the field-controls guide — every row should carry a device ("cruceta: Paleta de vehículo…",
"stick derecho: Mover cámara"). Then traces off + commit (user gate).

ROUND 17 (same day, ~20:50 — new-game session log). The movie-SKIP confirm read "Ajustes de
dificultad, Nivel de dificultad" instead of the skip question: the pooled window kept the
difficulty box's Txt_Title+Txt_Help stale (20:44:47, lbl=4) and the CHOICE branch still built its
prompt with message()'s title-presence composite. FIX: choice prompts now compose with the same
per-node novelty (fresh_notice), LATCHED once per window appearance (choice_key/choice_prompt —
after mark_window_nodes marks the prompt, a re-compute would return nil and flip the announcer
back to the stale composite); message() only as fallback for verbatim repeats. If a skip confirm
STILL reads difficulty text, its live question is not in the window's nodes — take an F7 with the
prompt open. Verify: new game → skip intro cinematic: expect the skip question + Sí/No; difficulty
window still reads title+options; food/return-to-title confirms unaffected. Then traces off +
commit (user gate).

ROUND 16 (same day, ~20:40 — round 15 CONFIRMED: full flow reads; three polish defects from the
20:20 session's trace). (1) STALE-HELP GLUE: "Bonificaciones" (20:23:57) and "Subida de rango"
(20:26:41) carried a fresh title + live body in Txt_Detail while Txt_Help was stale (reward's
"Recibiste los siguientes…" / gift picker's "Selecciona un regalo.") — the round-15 "titled ⇒
Title+Help" branch glued the stale help and pushed the live detail into a separate utterance.
FIX: fresh_notice composes ALL nodes independently by per-node novelty (title if fresh; each
untitled member if fresh; help only if fresh AND a title is PRESENT — the title-less flicker
ticks must not speak help as a headless fragment). Choice windows now mark their node texts
recent too (mark_window_nodes, once per prompt) — the gift prompt's help can't resurface under
the next title. (2) MISSING BONUS NAME: content rows fold only on titled ticks and now
row-by-row filtered/marked through the recent-set (plates/emblems_received return LISTS) — the
link-bonus row reads with "Bonificaciones", the reward's stale Goku/Gohan rows don't re-fold.
(3) MISSING BUTTON in tutorial instructions ("oprime  para…"): <inputicon
KeyConfigId="Comm_EmbSet"/> → asset entry has NO ctrl, only IconName=Decide, and "Decide"/
"Cancel" are abstract platform glyphs with no physical pairing anywhere in CFTextIconData
(dump_1782929293_017; Field_Decide=Btn_B contradicts the menu keyhelp confirm=A, so a guessed
button could misdirect). FIX: keyconfig_button falls back to the entry's IconName — Btn_N
resolves via idxToCtrl (data), Decide/Cancel speak the localized ACTION ("Confirmar"/
"Cancelar", i18n btn_decide/btn_cancel) — "oprime Confirmar para mostrar…". luac-validated;
all hot-reloadable. VERIFY: replay tutorial — bonus notice reads title+body+bonus name in one
utterance, no "Recibiste los siguientes" glue on later titles, guide says "oprime Confirmar",
reward still reads with names. Then traces off + commit (user gate).

ROUND 15 (same day, ~20:10 — round 14 CONFIRMED by the user: reward reads with names, no mixing;
ONE regression it introduced). The untitled "¡Recibiste al líder… y desbloqueaste un tablero…!"
notice went silent: at 20:04:03 the window reopened with the PREVIOUS reward's Txt_Title AND
Txt_Help still set (win01 enum: ti=Emblemas… de=¡Recibiste al lí… he=Recibiste los sigu…) — the
round-11 title-PRESENCE rule composed the exact just-spoken reward string and the recent-set
swallowed it. So the game does NOT reliably nil Txt_Title between layouts (round 11's premise);
presence and visibility both lie — the live layout is discriminated by per-NODE novelty. FIX:
`fresh_notice(w)` — Txt_Title fresh (not recently spoken), or its Txt_Help fresh, ⇒ titled read
(Title+Help, Txt_Detail still never read with a title: rounds 12/13 anti-mixing preserved);
otherwise fall through to the untitled members that are fresh (líder case: stale title ignored,
fresh Txt_Detail speaks). Each spoken NODE text is marked recent (RECENT_MAX 8→24, ~3 marks per
titled notice), so a stale slot can never re-compose as "new"; plates+emblems fold only into
TITLED notices (an untitled notice would drag the still-visible stale reward rows in). is_active
releases on "nothing fresh" (replaces was_recent(msg)); dlg trace gains `new=`. Verify: replay →
reward reads with names AND "¡Recibiste al líder…" reads after it; startup notices + confirms
unaffected. Then traces off + commit (user gate).

ROUND 14 (same day, ~evening — the UE4SS.log TRACEBACK closed it; one root cause explained both
remaining symptoms). At 19:43:37 and again 19:45:19 (post-reload), the first update() of the
"Emblemas de alma recibidos" notice ABORTED mid-tick: plates()' WL_TextCmuCtn pool entry read the
row member `WL_LvTextList` — a native FIXED C array (`UCFUIXcmnMultiLineText*[7]`, AT.hpp:33412) —
and Core.array_of's guard has a hole: the collapsed RemoteObject PASSES IsValid yet GetArrayNum
raises the "UObject instance is nullptr" C++ error THROUGH every pcall (process survives, tick
dies). The abort landed after `notice_msg = cur_msg` but before `notice_full` was rebuilt, so the
next tick skipped the rebuild and spoke the STALE notice_full: cold path = the previous "¡El Ki y
los PS…!" again (the "mixing"); post-reload = nil → bare title, no Goku/Gohan. THREE fixes in
screen_dialog: (1) the fixed-array read REMOVED (WL_TextCmuCtn dropped from POOLS, `fixed` branch
deleted); (2) emblems_received(win) now reads the window's OWN WL_TextCmuCtn TArray (AT.hpp:33362
— populated while the notice is live, the traceback proves it; round-12's "pool is EMPTY" was a
parked-moment artifact; FindAllOf dropped) → Txt_Name + Txt_Num00, WL_CharName fallback; (3)
update() is TRANSACTIONAL — notice_full built first, notice_msg latched last, so any future abort
retries instead of latching half-updated state. ui_core.array_of doc: second failure mode recorded
(fixed C-array member passes IsValid, GetArrayNum pierces pcall — never pass one; a pointer with
size > 0x8 in the CXX dump is a fixed array). luac-validated; hot-reloadable (Ctrl+Shift+R).
VERIFY (replay the reward): expect "Emblemas de alma recibidos, Recibiste los siguientes emblemas
de alma: Goku, nivel de comunidad N, Gohan, nivel de comunidad N", no Ki/PS mix, NO nullptr error
in the log. Then turn off DLG_TRACE + ENUM_WINDOWS + guide/community traces and COMMIT (user gate).

ROUND 13 (same day, ~17:00 — cold-boot log pinned the last bug). At 19:35:27 is_active's
message() correctly read "Emblemas de alma recibidos, Recibiste lo…" (title present) but one tick
later update()'s message() read "¡El Ki y los PS…" — the reward window's **Txt_Title FLICKERS
present/empty tick-to-tick**, so the title-aware message() alternates between the titled reward
and the UNTITLED stale Txt_Detail (an already-spoken notice). A single last-spoken latch can't
stop it (the flip flips `spoken` back and forth). FIX: a RECENT-SET (`recent`/`recent_set`,
FIFO 8) — a notice said in the last 8 is never repeated, so the stale Ki/PS (spoken moments
earlier) is ignored while the genuinely new "Emblemas…" still speaks once. Replaces the single
`spoken` gate (kept only for the trace) and the round-12 debounce (removed — the flicker never
stabilizes, so a debounce blocked the reward). Emblems names (round-12 emblems_received) retained.
Traces + win01 enum still ON for the cold-boot confirm.

ROUND 12 (same day, ~16:40 — opus RE of the reward-window dump was decisive for the NAMES). The
Goku/Gohan emblem rows are NOT inside the reward Win01 (its WL_TextCmuCtn pool is EMPTY) — they
are SEPARATE top-level widgets `Xcmn_Emb_List_C_0` (Goku) / `_1` (Gohan), siblings of the window,
so plates(win) could NEVER reach them (that's why names never read). FIX: `emblems_received()` —
FindAllOf("Xcmn_Emb_List_C"), read each visible one's Txt_Name + **Txt_Num00** (the community
level number; Txt_Commu_Lv is only the caption) → "Goku, nivel de comunidad 3" / "Gohan, nivel de
comunidad 1", folded into the reward notice. SECOND: cold-boot reads stale "Ki/PS" instead of
emblems (hot-reload reads it right) — the mod witnesses the pooled window's LIVE transition and a
transient Txt_Title-not-set-yet frame reads the stale Txt_Detail. FIX: NOTICE_STABLE=2 debounce —
the notice text must hold before speaking, so transient frames don't win. win01 enum RE-ENABLED to
capture the cold-boot transition if the debounce isn't enough. Title-aware message() (round 11) +
fresh re-read + never-cleared spoken all retained. Traces ON. No commit.

ROUND 11 (same day, ~16:10 — the `win01` instance enumeration SOLVED the reward). It is NOT
multiple instances: ONE window `Xcmn_Win01_C_0` on=true with ti="Emblemas de alma recibidos"
(current title) + de="¡El Ki y los PS…" (STALE from the prior notice) + he="Recibiste los
siguientes…" (current body). The game reuses the pooled window with TWO notice LAYOUTS and does
NOT clear Txt_Detail between them, so a TITLED reward keeps the previous UNTITLED notice's text
stale in Txt_Detail — message() read both and the stale one often won. FIX: message() is now
layout-aware — Txt_Title present ⇒ read ONLY Txt_Title + Txt_Help (skip the stale Txt_Detail);
Txt_Title absent ⇒ read Txt_Detail + captions. Txt_Title IS nil'd between titled notices (enum
confirmed) so it's a reliable signal. User confirms "Emblemas de alma recibidos" now reads (then
the quest objective, which is correct sequencing — "Coloca a Gohan junto a Goku"). win01 enum
turned OFF (answered). REMAINING cleanup once user confirms the whole flow: turn off DLG_TRACE,
guide GUIDE_TRACE, community DEBUG + claim trace, speech DEBUG_LOG, then COMMIT.

ROUND 10 (same day, ~15:40 — the dlg-notice print + dlg trace together were decisive). TWO root
causes found: (A) REWARD spoke the PREVIOUS notice: at 18:12:11 the dlg trace (is_active) saw
msg="Emblemas de alma recibidos" but the SAME-second dlg-notice print spoke "¡El Ki y los PS…" —
update() used a STALE `state.msg` (the pooled Win01 flickers between the old and new notice, and
the registry's confirm debounce lands update() a few ticks after is_active, on a stale frame).
FIX: the notice branch RE-READS `message(win)` fresh + the never-cleared `spoken` latch → each
notice speaks once, never a stale one. (B) INSTRUCTIONS still never detected (guide trace EMPTY):
the discriminator "plain text is empty" failed because the pooled Txt_Detail keeps STALE plain
text. FIX: discriminate by VISIBILITY — guide = rich ExMainTxt IsVisible AND the plain mainTxt NOT
IsVisible (a notice renders through plain, a guide through rich). Members already corrected to the
blueprint names (round 9). Caveat: guide reads the directory-mapped Win01 (GameWindowCore) — the
same instance screen_dialog reads the reward on, so the instruction should be there. All
reloadable. Traces ON. No commit.

ROUND 9 (same day, ~15:20 — only the emblem reward + the two instructions remain). The `guide`
trace was EMPTY → guide_watch NEVER detected an instruction: it read the NATIVE UAT_UIGameWindow
members `WL_TxtDetail/WL_TxtHelp/WL_WorkText`, but the instruction lives on the BLUEPRINT wrapper
`Txt_Detail` (F7 path Xcmn_Win01_C_0.WidgetTree_0.**Txt_Detail**.WidgetTree_0.ExMainTxt — the same
names screen_dialog's message() uses). FIX: MEMBERS = {Txt_Detail, Txt_Help, Txt_Work}. That + the
round-8 rich-IsVisible gate should make instructions read. REWARD "Emblemas de alma recibidos"
still detected (dlg, spoken set → update() ran → notice branch's Speech.say_protected must have
run) yet absent from dump_speech — unexplained by static reasoning ("¡Recibiste al líder" goes
through the SAME branch and IS spoken; the only difference is plates(win) content). Added a
`dlg-notice speak=…` print right before say_protected to settle it at runtime. All reloadable
(Ctrl+Shift+R reloads ui_registry + everything it requires incl. guide_watch — confirmed: the
round-8 guide_watch DID load, it just had the wrong member names; speech.lua alone survives a
reload). Traces still ON. No commit.

ROUND 8 (same day, ~15:00 — the Speech DEBUG_LOG (dump_speech.txt) was decisive). FOUR fixes:
(1) REWARD NOTICE "Emblemas de alma recibidos" was DETECTED every time (dlg trace, spoken set) but
NEVER reached PRISM (absent from the utterance log) — the shared MENU announcer's stateful
name-dedup swallowed it. FIX: notices now speak via a DIRECT `Speech.say_protected`, bypassing the
announcer (is_active gates update() to one tick per new text → exactly one utterance). (2) GAME
INSTRUCTIONS ("Con un panel vacío…", "Después de seleccionar a Gohan…") NEVER reached speech —
guide_watch gated on `on_screen(win)`, but the instruction renders through a Win01 whose CONTAINER
is enum=1:Collapsed while its ExMainTxt rich block is IsVisible==true (F7 census). FIX: gate on the
rich block's OWN IsVisible (like discover.lua), drop on_screen; scan ALL Win01 instances
(pooled). (3) SPEECH PRIORITY (round 7) holds: guide + notices are `say_protected`, board/grid
readout defers via Speech.protected() then re-announces. (4) BOARD SUMMARY was reading all ~10
community skills in one breath — trimmed to level/rank + placed + hint (skills list → future
on-demand key). Traces still ON: dump_speech.txt (every utterance+interrupt), `dlg`, new `guide`
(detected instruction per change), community DEBUG. STILL OPEN: BUG C (adjacent panel distinction).
All hot-reloadable (Ctrl+Shift+R). No commit.

ROUND 7 (same day, ~14:40 — user: the speech "eats" dialogues; instruction/reward drowned by the
board/grid readout; "some dialogues just aren't detected"). ROOT: there is NO Lua speech queue —
`Speech.say` passes interrupt straight to PRISM, so any reader's interrupt=true SHREDS a
just-spoken instruction/notice. There was no priority arbitration between the board/grid panel
readout and the guide/dialog lines. FIX (speech priority): `Speech.say_protected(text)` (interrupt
+ protect for ~spoken duration, len-based, cap 6 s) + `Speech.protected()`. guide_watch speaks the
instruction PROTECTED; `Commu.update` DEFERS the board/grid readout while protected, then
`ann:invalidate()`s so the current panel re-announces after (the cut readout comes back, not
swallowed). "Not detected" dialogues — UNCONFIRMED cause yet; enabled Speech `DEBUG_LOG`
(dumps/dump_speech.txt: every utterance + interrupt flag) + kept the `dlg` trace to see exactly
what reaches PRISM and what suppresses what next test. Suspects: guide-yield false-positive on a
notice whose Txt_Detail holds STALE rich (should be rejected by on_screen, needs confirming), or
the never-cleared `spoken` latch on a repeated text. All in speech/guide_watch/screen_community/
screen_dialog (hot-reloadable — Ctrl+Shift+R). No commit (user gate).

ROUND 6 (same day, ~14:25 — BOARD FIXED & CONFIRMED by user ("funcionó"); pane_live drop was
right. Two dialogue bugs remained, both PINNED by the `dlg` trace). BUG A "notice eats the other
dialogues": the persistent tutorial notice "¡Recibiste al líder… Goku!" stays up through the
board step and BLINKS off-screen for 40+ s; SPOKEN_GRACE_S=3 s cleared the latch on each long
blink → re-announced (interrupt=true) at 17:22:25 / :34 / 17:23:16, cutting off "Coloca a Gohan"
and "elige un panel vacío". FIX: `spoken` is NEVER cleared by absence — only overwritten when a
DIFFERENT msg is announced; a same-text blink is suppressed, any new text still announces.
BUG B "reward Goku+Gohan not read": title "Emblemas de alma recibidos" announced but the
Goku/Gohan content rows were a LAZY tooltip that the next notice (3 s later) pre-empted. FIX:
fold `plates(win)` content into the title utterance, once per msg (cached). Both in screen_dialog
(hot-reloadable — Ctrl+Shift+R suffices). DLG_TRACE + community DEBUG still ON for confirm.
STILL OPEN — BUG C (user): on the board, an emblem panel (Goku) and the empty target panel next
to it aren't distinguished while moving (the "place Gohan next to Goku" fine step). Board hover
precision / debounce — next, after the dialogue fixes are confirmed. No commit (user gate).

ROUND 5 (same day, ~14:10 — user VERY frustrated; I had misread the flow). CORRECT FLOW: (1)
select an EMPTY PANEL on the board (browse, nothing held), (2) press A → grid, pick Gohan, (3)
place on Goku's area. The blocker is STEP 1 — empty panels read nothing. Fresh-boot claim trace:
board NEVER claimed (only grid), and the board DEBUG state-dump never wrote → board_update never
ran. F7 dump proof: `Start_Commu_Brd_C_0` EXISTS and is fully populated (Brd_Skill00-09, levels,
rank, Brd_Emb) and the keyhelp says board ("Confirmar bonificaciones / Mueve el cursor / Edita un
tablero"), yet the mod rejects it. ROOT CAUSE (high confidence): the `pane_live(board)` gate added
2026-07-15 (ghost-board fix) wants strict ESlateVisibility Visible(0), but the STORY-TUTORIAL
board renders under a non-Visible flag while the tutorial owns input → rejected → never claimed.
Memory is explicit that pane_live NEVER discriminated the ghost pane — the MODE MACHINE did — so
FIX: drop pane_live from the board gate, rely on frame on_screen + the mode machine (a parked
board reads a non-live mode → ghost path). Added `rej=` to the claim trace (not-found /
frame-offscreen / ghost-mode / no-mode) to CONFIRM the cause next test. Also: this is a REGRESSION
— the board tutorial was verified working 2026-07-04 (pre-directory, pre-pane_live). DIALOGUES:
reward popup "Emblemas de alma recibidos" (screenshot 115) still not read + tutorial dialogues
after "líder Goku" skipped/"mixing" — added a `DLG_TRACE` in screen_dialog (win/msg/labels/
guide/spoken per change) to pin which window is skipped and why; NOT yet fixed. Verify next run:
read the `commu ... rej=` and `dlg ...` log lines. STILL no commit (user gate).

ROUND 4 (same day, ~14:00 — user still "no lee" the panel; dialogue "líder Goku" repeating +
reward popup still mute). KEY META-FINDING: the user was only pressing **Ctrl+Shift+R**, which
does NOT load a NEW module (`guide_watch.lua`) or `ui_registry`/`main.lua` changes — those need
a FULL game restart. So the round-2/3 dialogue+guide fixes were never running; that half is
"pending a real restart", not broken. What Ctrl+Shift+R DID reload (screen_community) gave clean
movement data: (1) opus was WRONG that the reticle is fixed — in mode 7 + held, `WL_PanelCursor`
Translation MOVES with the stick (curs 210,317→498,446→583,53) and the replica tracks it
(hov 1→nil→2). So the mechanic works. (2) The "no lee" in PLACING is an interrupt storm: held =
free analog cursor, native hover cache frozen (opus), replica returns jittery socket/gap/socket
per tick, each announced interrupt=true → speech shredded. FIX: `debounce_hover` (HOVER_STABLE=2
ticks) in board_update, held path only (browse uses the stable native cache). (3) Board
first-detection latency persists on a fresh session (claim=nil for the seconds the user was on
it, then claim=board) — the scan-path cost of unmapping; a targeted watch arm is the follow-up
(backlog). Plus unrelated: quest objectives spoke raw markup ("...una <span color=…>fogata</>")
→ `quest_objective` now reads through `A.markup_to_speech` (F10 raw-dump kept). DEBUG +
claim-trace STILL ON for the next (full-restart) test.
VERIFY after a FULL RESTART: dialogue reward reads once + no "líder Goku" repeat (guide_watch
live now); board browse reads panels on move; PLACING reads the panel when you settle on it (no
garble); objective reads clean ("Cocina un pez en una fogata"). Then turn OFF: screen_community
DEBUG + tut trace, screen_dialog nothing (no debug), and commit only on the user's OK.

ROUND 3 (same day, ~13:45 — user: "moving the cursor reads nothing" ON THE BOARD, screenshot
103 + claim trace `board=false` on the open board, fresh session): THREE stacked defects.
(1) **`Start_Commu_Brd_C` UNMAPPED from ui_directory** — field names were real
(hud.UICommBoardIns @0x5D0, cm.UICommBrdIns @0x58) but the STORY-flow board leaves both
null (it hangs off the unreflected cm.MenuCommunityBrdIns controller — the emblems-menu
hole again): mapping asserted "absent", no scan fallback, board read NOTHING all session.
Earlier sessions worked because other flows do populate the fields. (2) `board_panels`
cached an EMPTY WL_PanelTbl read as final (populates ticks after the frame; the
items-rebuild lesson) → hit-test replica saw 0 sockets forever (hov=nil with reticle
readable) — now empty reads retry and the cache revalidates. (3) opus RE (decompiled):
the board cursor is a FIXED RETICLE (700,760) — the BOARD pans under it via frame adjusts
(+0x428/0x42C, leader +0x430/0x434) which the replica re-reads per call, so it works; but
while an emblem is HELD the game's hover tracker EARLY-OUTS (gate frame+0x7B8 != 0,
FUN_1414f91e0) and host+0x5D8 FREEZES — placing has NO native per-tick cache (game
hit-tests only on Decide press) → held now skips the cache and uses the replica only.
Board DEBUG=true + claim-trace print active for the verify; turn both OFF after.
VERIFY (restart; replay the community tutorial): (a) after the reward popup reads once, NO
"Recibiste al líder…" repeats ever; (b) on the board you hear the instruction ("Con un
panel vacío seleccionado, oprime A para mostrar la pantalla de emblemas de alma") after the
board readout; moving the cursor announces "Panel vacío, panel N de M" / emblem names; F1
repeats panel + instruction; (c) A on an empty panel → grid reads slots within ~1s, then
the "Después de seleccionar a Gohan, oprime A…" instruction; A on Gohan places him; (d)
regression: normal dialogs/confirmations/autosave notices still read; the gift picker
(choices) still reads its options.

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
- **FUTURE IMPROVEMENT (user, 2026-07-16): d-pad SNAP for the placing/browse board cursor.**
  The community board uses a FREE analog cursor (both browse and, especially, placing an emblem
  in hand). Blind-unfriendly: the player can't aim at a panel. Desired: a d-pad press snaps the
  cursor to the nearest panel in that direction (treat the board as a graph), so each press lands
  cleanly on one panel and announces it. Needs a WRITE primitive (mem_bridge poke of the cursor
  Translation, or steering the game's own input) + pad_poll registration + full restart — a
  design session of its own. All the geometry is already mapped (native_offsets.commuBoard +
  the FUN_1414f2ab0 hit-test replica in screen_community; opus RE 2026-07-16 confirmed the
  reticle/pan model and that the game only hit-tests on the Decide press while placing). Same
  request already exists for the SOCKET board free-cursor — unify them. Interim: the
  `debounce_hover` read makes settling on a panel announce it, which is usable but not snap.
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
