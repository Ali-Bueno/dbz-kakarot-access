# STATUS — Dragon Ball Z Kakarot

> Per-mod status ledger / dashboard. Open this first when resuming the mod so progress isn't
> re-derived from the code each session. Keep it short — a dashboard, not docs. Update the **Next
> step** line and the section table whenever you finish a chunk. Derive every value from the game's
> real data — no guessed offsets.
>
> **The diary lives elsewhere now.** Everything this file used to carry — the derivations, the
> refuted hypotheses, the per-batch test plans, the in-game reports — is archived verbatim in
> [STATUS history](reference/dbz-kakarot/notes/dbz-kakarot-status-history.md) (3,456 lines, nothing
> deleted). Grep that for *why*; keep this file for *where we are*. **If an entry here needs more
> than about two lines, it belongs in the archive or in a note**, not in the dashboard — that is how
> this file reached 355 KB and stopped being readable in one pass.

**Architecture — read before changing how UI state is read:** [`reference/UE4ss study/docs/ue4ss-mod-architecture.md`](<reference/UE4ss study/docs/ue4ss-mod-architecture.md>) — *resolve, don't scan*, synthesised across this mod and the Sparking ZERO one: scan cost measured on both (~65 ms here vs ~115 ms there), the decision ladder, and the `RegisterBeginPlayPostHook` acquisition this mod has **not** tried yet (the ini ships with BeginPlay hooking off). Game-specific counterpart: `reference/dbz-kakarot/notes/dbz-kakarot-perf-architecture.md`.

**Last updated:** 2026-08-14 — STATUS pruned back to a dashboard. No code changed.

## Where the mod stands

44 screen adapters, PRISM speech, 4 native bridges, an audio radar with 14 target categories, and
external localization in 13 languages. Most of it is verified in game; the table below says which.

Two things are outstanding, and both are **unrun rather than unfinished**:

- The last code batch (2026-08-03, post-battle radar re-acquisition) is **source-only, never
  played**. Lint clean over 75 files. Needs a full restart.
- The **MCP inspection server** (2026-08-06) is installed and wired — `Inspector : 1` in `mods.txt`,
  `build_flags.debug = true`, both junctions in place — but has **never answered against a running
  game**. `kak_alive` on 2026-08-14: game not running, `UE4SS.log` ~10 days stale.

## Next step

**One session with the game running**, in this order, because each item unblocks the next.

1. **Smoke-test the MCP** (owed since 2026-08-06): `kak_alive` → `kak_screen` → `kak_census` with
   one menu open. Until this passes, every diagnosis below still costs a full capture round through
   the player.
2. **Play the 2026-08-03 batch** (full restart). Finish a battle with the radar tracking a quest
   objective: it should resume guiding within about a tick of regaining control, with no
   re-announcement. Try it with a HAND-PICKED target too (R3 → pick → get into a fight) — that is
   the separate resume lane, so say which of the two is slow if either is. Then advance a quest so
   the objective CHANGES: guidance should start almost at once. Two things that are NOT bugs — the
   game sometimes spawns the marker seconds after the HUD text changes, and a marker that never
   appears still burns the preempt's tries over ~15 s.
3. **Ctrl+F5 after ~10 minutes** — owed since 2026-08-03, and the only way to know whether the ghost
   work paid. Compare against `ui step ms: avg 35.48 / max 925`, `findall scans: n=1982 /
   119463 ms`, `ghost classes: 42 / 84783 ms`.
4. **Soul Emblems**: the grid must read, and the board alone must still read.
5. **Dev loop**: create `UE4SS-settings.dev.ini` (consoles on) and have `package.ps1` ship the
   release ini instead, so development stops running against the release profile.

## Identity
- **Engine / framework:** UE4 (AT project) + UE4SS v3.0.1 — Lua scripts plus C bridge modules (`prism_bridge`, `audio_bridge`, `input_bridge`, `mem_bridge`).
- **Screen-reader transport:** PRISM (`prism.dll` + `prism_bridge.dll` in `Scripts/`). No `tolk.dll` — NVDA/JAWS/SAPI are built into `prism.dll`, which does not import it (dumpbin-verified 2026-07-29).
- **Build command:** per-bridge `src/<bridge>/build.ps1` (rebuild only the bridge you touched). Lua is not compiled; validate with **two** checks, not one: `libs/lua54/luac.exe -p <file>` for syntax, **and** a globals lint — `luac.exe -l -p <file> | grep -oE '_ENV "[A-Za-z_][A-Za-z0-9_]*"' | sort -u` — where anything that is not a Lua builtin or a UE4SS global is a bug. `luac -p` does NOT catch a local used above its own declaration: that compiles to a global read, is nil at runtime, and on 2026-07-25 one such line would have shipped a mod that was silent from boot (it also exposed two pre-existing ones, see the crash ledger).
- **Game install path:** `D:\games\steam\steamapps\common\DRAGON BALL Z KAKAROT`. Exe + UE4SS at `…\AT\Binaries\Win64\`. RE dumps live there: **`CXXHeaderDump\`** (per-class `.hpp`, the authority on layouts/members — `AT.hpp` is the big one) and **`UE4SS_ObjectDump.txt`** (what Lua can actually reflect). Grep these instead of re-deriving. **Regenerating them (2026-07-21):** Ctrl+H = headers, Ctrl+J = object dump, and **both `LoadAllAssetsBefore*` MUST stay 0** — force-loading reaches the stripped debug blueprint `AutoDebugMainUI_C` and kills the game with `LowLevelFatalError … Could not find SuperStruct` before writing anything. Native classes (`AT.hpp`) are complete from the title screen regardless; only lazily-loaded BP `_C` classes need you to have visited their screen, and they accumulate over a session (one Ctrl+H at the end). **Third source, offline:** `D:\code\tools\repak\pak_index.txt` — all 348,382 pak asset paths, grep-able without running the game (see [ui-and-text-architecture.md](reference/dbz-kakarot/ui-and-text-architecture.md)).
- **Mod install path:** junction `…\DRAGON BALL Z KAKAROT\AT\Binaries\Win64\Mods\KakarotAccess\Scripts` → repo `mod/KakarotAccess/Scripts`; enabled in `mods.txt` (`KakarotAccess : 1`).
- **Run / test:** launch the game with the mod enabled. `Ctrl+Shift+R` in-game reloads the Lua feature adapters + i18n; `main.lua` changes (keybinds, `Mem.init`, `Speech.init`) or a new/rebuilt DLL need a full game restart.

## Dev loop (UE4SS) — verified 2026-07-24

Facts verified directly against the real install (`D:\games\steam\steamapps\common\DRAGON BALL Z KAKAROT\AT\Binaries\Win64\`):

- UE4SS **v3.0.1 Beta #0** (SHA #7d6f790). Engine UE4 with `[EngineVersionOverride]` set to 4.20.
- `[Debug] ConsoleEnabled/GuiConsoleEnabled/GuiConsoleVisible = 0` → **no console**, so today there is no live in-game inspection. The ini carries its own comment: `package.ps1` ships THAT file to end users. **Pending action**: keep a `UE4SS-settings.dev.ini` with the consoles set to 1 and have packaging copy the release ini instead, rather than developing against the release profile.
- `[General] EnableHotReloadSystem = 1` (good: Ctrl+R hot-reloads without a restart). The keys `HotReloadKey`, `EnableAutoReloadingLuaMods`, `DoEarlyScan`, `bEnableSeachByMemoryAddress` do NOT exist in the 3.0.1 ini — adding them does nothing.
- All 6 dumpers are global Lua functions callable live, confirmed in `Mods\Keybinds\Scripts\main.lua`: `DumpAllObjects()` (Ctrl+J), `GenerateSDK()` (Ctrl+H), `GenerateUHTCompatibleHeaders()` (Ctrl+Num9), `DumpStaticMeshes()` (Ctrl+Num8), `DumpAllActors()` (Ctrl+Num7), `DumpUSMAP()` (Ctrl+Num6). No need to close the game to dump.
- `Mods\shared\types\` already exists with **1,361 .lua type files** for the game → autocomplete available in VSCode with the sumneko.lua extension (NEVER `require()` those files: they overwrite UE4SS's own globals!).
- `Mods\mods.txt`: every universal mod is at 0 (`ConsoleEnablerMod`, `ConsoleCommandsMod`, `CheatManagerEnablerMod`, …). Setting `ConsoleEnablerMod : 1` gives the game's own console (`@`/`F10`) and `ConsoleCommandsMod : 1` gives the `set` / `summon` / `dump_object` commands.
- Tool at `tools\ue4ss-inspector\`: a non-visual inspector (`dumpclass`, `props`, `probe`, `watch`, `dump` commands) driven by console **or by a command file** (`inspector_cmd.txt` → `inspector_out.txt`), built to not depend on the ImGui GUI, which is not accessible. **Installed 2026-08-06** as a junction `…\Mods\Inspector\Scripts` → `tools\ue4ss-inspector\Scripts`, `Inspector : 1` in `mods.txt` — so edits to it are live like the mod's own.
- Reference: `UE4ss study\docs\ue4ss-live-workflow.md` (the no-restart loop).

### MCP server (2026-08-06) — the assistant drives the game directly

`tools\kakarot-mcp\` (Node, zero deps, registered in `.mcp.json`). Wraps **both** file channels so an
inspection costs one tool call instead of a round-trip through the player. See its README for the
tool list; the parts that matter to this ledger:

- **Two channels, because the mods cannot call each other.** UE4SS `lua_State` isolation between mods
  is undocumented, so mod internals are driven from inside the mod: new `dev_channel.lua`
  (`kakarot_cmd.txt` → `kakarot_out.txt`, gated on `Build.debug`, dropped from releases by
  `package.ps1`) exposes `screen`, `census`, `reload`, `navdump`, `navlevels`, `memdiff`, `say`,
  `ping`. The Inspector keeps generic reflection.
- **Sequence tags.** Commands are written `#<seq> <cmd>` and answered between `<<<BEGIN seq>>>` /
  `<<<END seq>>>` — the answer is findable in an append-only file, and a repeated command (`probe X`
  twice, the whole point of probe) is no longer swallowed by the whole-line dedup.
- **`kak_screen` answers the question this codebase asks most:** which adapter owns the tick, plus
  the last lines actually spoken (new `Registry.active_name()` and `Speech.recent()`). An adapter
  that never appears has a gate problem in `is_active`; one that appears saying nothing is holding
  the tick with nothing to say. Both are documented recurring failures.
- **What it does NOT do:** navigate. A human still has to open the screen; from there the assistant
  can ask many questions without asking for anything.

## Section status

`done` = works with the screen reader on; `wip` = started; `todo` = not begun. *Unverified* means
written and lint-clean but never seen working in game. The full derivation of every row is in the
[STATUS history](reference/dbz-kakarot/notes/dbz-kakarot-status-history.md).

| Section / feature | Status | Notes |
|---|---|---|
| Speech pipeline (PRISM) | done | Logs the chosen backend on boot. |
| Overworld main menu (native selection) | done | `screen_field.lua`, reads via `UAT_UIStartTop` offsets. |
| Battle-pause menu (native selection) | done | `screen_pause.lua`, `UAT_UIXCmnPause +0x43C`. |
| Defeat / game-over menu | wip | Directory-mapped (`fm.Gameover`), so detection is a pointer read. `inViewport=true`, so `IsInViewport` is not what rejects the host. Probe now logs on signal change (incl. `pane_live`). Unverified. |
| Defeat / game-over menu — earlier rounds | wip | `screen_gameover.lua`, `Gameover_C` → `UAT_UIGameover`. Index = `CurrentSelectIndex` (fallback @0x3E0); rows via `List_Bar0N`. Unverified, needs restart. |
| Dialog / message / confirm popups | done | `screen_dialog.lua`. Per-node composition; emblem reward via `WL_TextCmuCtn`. Verified 2026-07-16. Verbatim-repeat rewards rescued by fresh content rows within ~3 s: unverified. |
| Tutorial guidance line | done | `guide_watch.lua` — reads the RICH side (`Txt_Detail`/`Txt_Help`/`Txt_Work` ExMainTxt; the plain side keeps stale text). F1 repeats. Verified 2026-07-16. |
| NPC subtitles / dialogue — four surfaces | done | The fourth is `UAT_UIFieldNaviWin` (AT.hpp:33020, `fm.FieldNaviWin` @0x550), the portrait pop-up. Nodes `Txt_Speaker`/`Txt_Msg`; natively `TextBox_Name` @0x400 / `TextBox_Message` @0x408. |
| NPC subtitles / dialogue — earlier rounds | done | `screen_dialogue.lua`. `Xcmn_Subtitles_C` gated on the game's own subtitle option (fail-open); `Field_Talk_Win_C` never gated. Reads `UATSaveManager.SaveSystem` @0x108 — the old path read a dead template. Unverified. |
| Boot agreement viewer (EULA / privacy / data) | done | `screen_agreement.lua`, `AT_UIXcmnAgreement` (AT.hpp:37902) — every page is a TEXTURE. docId @0x5B4, page @0x5A8/@0x5B0, `AgreementDialog` @0x340. NEVER brush-read this widget (piercing nullptr). Verified 2026-07-17. |
| Difficulty / choice lists | done | `screen_choicelist.lua`, `screen_choice.lua`. |
| Notices no longer cut off by the screen underneath | done | `Announcer:focus` + `keyhelp_watch` defer while `Speech.protected()`. A substrate fix, not a per-adapter one. Verified 2026-07-25. |
| Dialog re-trigger reads again | done (unverified) | `recent_set`/`pinned_set` scoped to window PRESENCE (cleared after `CLOSED_CLEAR_S` = 0.7 s of absence), not to the map epoch. |
| Keyboard: radar picker + key names + input block | done (unverified) | `radar_menu.Menu.key(cmd)` plus Ctrl+F3 / PageUp-PageDown / arrows / Enter. `input_bridge.kb_block(ms)` hides the keyboard from the game, so the ↑/↓ mount conflict is gone. Needs restart. |
| Controller remap — LIVE, before "Save changes" | done | The pending layout is a process-global `TMap` at exe base + `0x569C3B0`. Reader compares FName comparison indices and refuses unless the 12 slots @0x180..0x1D8 form a permutation. `native_offsets.inputAssignMap`. Verified 2026-07-25. |
| Controller remap honoured everywhere | done (unverified) | Every controller id names a SLOT, not a button, and the asset is never rewritten on rebind. Slot→physical layer in `ui_archetypes` reads `UATSaveSystem.InputAssign`; applied in `A.button_name`. |
| Options / System / Title / Tutorials / Tips | done | `screen_options/title/tutorials/tutorial/tips.lua`. Title SETTLE GATE: silent until it holds the screen 2.5 s with no dispatcher reset (no boot-phase field exists on `AAT_Title`); F1 bypasses via `reannounce()`. Verified 2026-07-17. |
| Shops (food/material/info) + item palette | done | `screen_shop*.lua`, `screen_palette.lua`. Verified in game. |
| Party menu — character list (right panel) | done | `UAT_UIStartParty.WL_Start_Char_Bar` @0x3C0 = 4-slot fixed array of `UAT_UIStartCharBar`; shared code in `ui_charbar.lua`. No focus field exists — inferred from the cursor marker, with `partySelectData.cursorIndex` @0x420 as tiebreak. Verified 2026-07-28. |
| Items inventory + Party + Characters | done | Party/Characters done. An empty category leaves the whole item UI STALE. `itemMenu.hasItems = 0x620` is NOT a has-items flag, it is the tab INDEX. `screen_list.lua`. Verified 2026-07-11. |
| Item submenu (use-item char select) | done | `screen_itemuse.lua` — A on a usable item → pick who uses it. Reads `WL_Start_Party_Bars` (`Txt_Name01`, `Txt_Lv01`→`Txt_Lv02` preview). Verified 2026-07-11. |
| Save / Load data slots | done | `screen_saveload.lua`, `AT_UIStartSaveLoad`. Virtualized `UISaveLoadBar_List`, so pool position ≠ ordinal. `saveLoad.selectedIndex` @0x410 (+1), cursor `windowPos` @0x418. Verified 2026-07-11. |
| Character status page (stats sheet) | wip | `screen_status.lua`, `UAT_UIStartStatus`. `WL_Start_Char_List01` is a fixed C array — UE4SS COLLAPSES it, NEVER index it. D-pad / F11 navigation. Unverified, needs restart. |
| Skill palette / Super Attack equip | done | `screen_skillcustom.lua`. Active slot = `SelectActiveBorder` visible AND `BaseBlinkImage` hidden. `SkillListMenu:GetSelectValue()` is DEAD here (frozen 0) — never use it. Verified 2026-07-13. |
| Skill tree / learn super attacks | done | `screen_skilltree.lua`, `Start_Skilltree_C` < `UAT_UISkillTreeMenu`. Lock read from the native per-node state byte. Orbs `WL_Skilltree_Zorb00` (1–6 = cost, 7–12 = owned). KNOWN LIMIT: a lv2/3 node reached without passing its lv1 node announces no lock. Verified 2026-07-14; the 2026-07-29 substrate patch is unverified and needs a restart. |
| Contextual actions (keyhelp) | done | `keyhelp_watch.lua` / `keyhelp.lua` — prompts read on entering any menu and again only when the set changes; 30 s same-phrase cooldown. The poll may use ONLY `Core.cached_all`, never a raw `FindAllOf` (stalls the game thread). Ctrl+F2 needs a restart. Verified 2026-07-17. |
| Quest objective HUD (text) | done | `quest_objective.lua`. A single-objective quest puts the text in the TITLE node (`Txt_Main00`/`WL_MainQuestListTitle`). 2026-07-31: signature diff, fallback to last known text, L3+Y repeats. Verified 2026-07-15; the 07-31 changes are unverified. |
| Episode title cards | wip | `screen_questcard.lua`, `AT_UIQuestMainStart.TitleText` @0x3E0 via `{"fm","QuestMainStart"}` @0x558. `fm.QuestMainLogo` is image-only, unread by design. A cinematic chapter card went unread 2026-07-17; trace armed. Unverified. |
| Cooking menu | done | `screen_cooking.lua`; entry menu via the second `Shop_Top_C`. CAVEAT: the ghost pane reads `vis=0 opacity=1.0`, so `pane_live` does NOT discriminate it — the shadowing is killed by the yields plus spoken-key suppression. `LATCH_DEBUG` on. Verified 2026-07-15. |
| Fishing minigame | done | `screen_fishing.lua`. `AT_UIBattleRushSpeedCore` deliberately unmapped (the game never sets that pointer). `ring_core()` picks the on-screen pool instance. Phase 2 = bare letter on `fishing.phase == 2`. Verified 2026-07-15. |
| Fishing catch result ("¡BRAVO!" sheet) | done | `screen_fishresult.lua`, `Mgame_Result_C` < `AT_UIMiniGameResult`. `fm.MiniGameResult` @0x630 deliberately unmapped. OPACITY-ONLY gate. Registered ABOVE `screen_fishing`. Verified 2026-07-17, rest pending. |
| Gameplay toasts (item log + level-up) | done | `screen_toasts.lua`. The level-up banner is `Info_Log02_C` (`TextBox` @0x3C0); `Info_Log_Level_C` DOES NOT EXIST. A raw `bar.Txt00` killed the game 2026-07-26 — the 07-17 pcall was NOT a fix. Every fetch now goes through `Core.member`. Unverified. |
| Cinematic character-intro cards | wip | `screen_infoname.lua`. The pooled `Info_Name_C` class is a DEAD END — never use it; the real display is `fm.InfoName` → `InfoCoreCtn` @0x3A0. GONE_S=30 dedup, `Speech.say` `no_requeue`. Verified 2026-07-17. |
| Soul Emblems grid / Community | done | `screen_community.lua`, menu-flow class `Start_Commu_Emb_C`. `Mrs` = Muten Roshi, `Mst` = Mr. Satan (they were swapped). Entry signal via `mm.m_xSoulEmblemMenu` @0x158 / `cm.MenuSoulEmListIns` @0x80. Grid verified 2026-07-15; the 07-16 entry signal is unverified. |
| Community board cursor (story tutorial) | done | Offsets in `native_offsets.commuBoard`. Verified 2026-07-04 — unblocked the story. |
| Story menu ("Historia") | done | `screen_story.lua`, `Start_Quest_C` < `UAT_UIStartQuest`. Reads the DETAIL PANE, never the list. Mode from `All_Win_Story` @0x3A0 / `All_Win_Task` @0x3A8. Locked entries read the game's own "¿?". Verified 2026-07-28; the Task-mode tweak needs a re-verify. |
| Z Encyclopedia | done | `screen_compz.lua`, `CompZ_Page_*_C` < `UAT_UICompZPage*`. The book renders to a TEXTURE and is NEVER in the viewport (`IsInViewport` / `on_screen` both fail) — use `Core.is_visible`. Cursor `Canvas_Cursor` @0x3A0. `CompZ_Memo_C`'s body node is `Txt_Detail`, not `Txt_Memo`. Verified 2026-07-28. |
| Substory clear rewards | wip | `screen_questreward.lua`, `Quest_Sub_Reward_C` < `UAT_UIQuestSubReward`, `Txt_Title` @0x540. Never ran once: `Core.pane_live` rejects HitTestInvisible overlays; fixed with `Core.pane_rendered`. Unverified. |
| Crash hardening — full-codebase audit | done (unverified) | 71 Lua files + 4 bridges swept (48 candidates → 11 real). Fixed `Core.array_of`'s missing existence gate (`ui_core.lua:419`), a 32-bit overflow in `audio_bridge`'s RIFF bounds check, an unbounded write in `mem_bridge` (now `expect_class`). See the crash ledger. Needs restart. |
| Crash diagnostics (black box + breadcrumb) | done (unverified) | `mem_bridge.mark()` — a 64-slot ring in `crash_trail.bin`; `main.lua` prints the previous session's trail at boot. Named the crash site on BOTH crashes it has seen. Tested with TerminateProcess. |
| Story / battle results | wip | `screen_results.lua`, `screen_battleresult.lua` (rank from brush textures). The constant-"222" bug: all digits share one atlas `Ins_Num_Result02`, so the digit must be a MATERIAL PARAMETER on the MID. Round-2 dump goes to `dumps/dump_results.txt`. Unverified. |
| Quest navigation radar | done | `nav_tracker.lua` + `audio_bridge`. 2026-07-26 hardening: 24 bare `:IsValid()` and ~95 naked fetches on streamed/destroyed actors migrated; an expired-but-unrefreshable list is now dropped, never served. Battle-interruption resume. Unverified. |
| Radar categories 2.0 (sites/enemies/collectibles) | done | Verified 2026-07-15. |
| R3 radar target picker (modal) | done (regression fixed, unverified) | 2026-07-28 it was dead on both binds: `Core.member`'s result gate called `IsValid` on `UniqueId`, an FName, which raises THROUGH pcall. Fixed in the substrate (gate narrowed to Array/Struct, new `Core.name_str`). The bind is R3, not R2. |
| Telepathic messages (King Kai) | todo | Probably nothing to answer: no telepathy widget, no accept/reply function. The one hit is `ATDebugSendTelepathyNotice()` (AT.hpp:26783) — telepathy belongs to the ambient CrossTalk / Notice family. GAP: zero mod coverage by name, so if the line does not route through `Xcmn_Subtitles_C` it is silent. Needs the user's go-ahead. |
| Radar: Exits category | wip | `nav_tracker.lua` ~1531. The ghost filter dropped doors because an `ATriggerBox` is `bHidden` by definition, so an exit could never be tracked at all. `chainable()` now excludes `"exit"`. Unverified. |
| Radar: Exits category — round 1 | done | `AATDoorVolume` < `ATriggerBox` (AT.hpp:13004): `AreaName` @0x378, `DoorName` @0x370, `AreaMessageId` @0x358, `PlayerStartTransform` @0x390, `DestinationDoor*` @0x560. Group `"exit"`, 300 m cap. Needs restart; unverified. |
| Radar: Companions category | done | `companions` group reusing the Shift+F5 collector (player / spawn-type enemy / parked pool excluded, 300 m cap); names via `enemy_display_name`. Unverified. |
| Radar: ghost-enemy fix (hidden actors) | done | `enemies_list()` + `companions()` filter `bHidden` via `char_visible`. NEVER read `CurrentHiddenType` on `AT_Character` — it is `AQuestCharacter`-only (AT.hpp:17553), so it is an uncatchable abort. Verified 2026-07-17. |
| Radar: enemy levels | done | Enemy level = `ATEnemyStatus+0x390` int32; on the player that offset is a pointer, so the reader is enemy-only. Pinned at runtime as `StatusInstance+0x1C`; HP = `SI+0x394` f32. Unverified. |
| Radar: quest FOCUS (stay on the side story) | done (unverified) | `classify()` now asks the navi widget's switcher (1 = main / 2 = sub / 3 = DLC6-sub) before `ATMapIconComponent.MapIconType`. New standing `preempt.focus`, released only by `Nav.notify_objective_gone()` after 3 polls against a READABLE HUD. |
| Radar: objective auto-track (smart radar) | done | `quest_objective.lua` diffs the HUD signature → `Nav.notify_objective_change()` → preempt in `nav_tracker` (~10-scan TTL); B restores the stashed pick. Idle re-arm makes a freshly activated objective auto-track persistently. Verified 2026-07-17. |
| Radar: gathering chain fixes | done | Stateful points (`AccessPoint*` / `MiningPoint*` / `PlacementObjectInfo` < `AAccessPointBase`) advance on `InteractState = Taken`; `ASpawner*Volume` (fruit / small fish) have no taken state, so they use the wide `ARRIVE_DIST` (8 m). Verified 2026-07-17. |
| Localization (external TXT + all game languages) | done | `i18n.lua` overlays `Scripts/lang/<code>.txt` on the built-in es/en tables (external wins). 13 languages from the game's own `ELanguageType`. Resolution: ext[lang] → S[lang] → S.en → key. Verified 2026-07-17. |
| Mod config menu (L3+R3) | done | `config_menu.lua`, opens with L3+R3 in the overworld (`Nav.field_ready()`): audio cues, cue volume, radar auto-activate, language. Persists to `Scripts/config.txt`. Mutex `_G.__KakarotPadModal` with the radar picker. Verified 2026-07-17. |
| Explore-radar toggle (double-R3) fix | done | `radar_menu.lua` — the window went to ~400 ms plus a slow-double rescue (~680 ms), so a slow double no longer just opens and cancels the picker. Verified 2026-07-17. |
| Battle monitor | wip | `battle_monitor.lua` present. |
| Cinematics / transition fluidity | wip | CAVEAT: quiet mode must never mean "keep serving stale handles" — that reading caused a crash. `Core.set_quiet`, 300 ms registry idle throttle, pad_boost / pad_poll relax, nav defer, INI `HookAActorTick=0`. Needs restart; unverified. |


## Player requests (2026-08-14)

Asked for by players; scoped against the code the same day, so the entry says what is already built
and what is genuinely missing. None of these is started.

- **Dragon Balls in the R3 radar.** The category is already wired — `dragonball` is in `GROUP_ORDER`
  (`nav_tracker.lua:112`), `EMapIcon 28` maps to it (:136) and it has its own spoken noun
  `cat_dragonball` (:174) — but it is the one radar group with **no VERIFIED tag**, and no world
  pickup actor has ever been identified. `AT_UIStartDragonBallMenu` is the *menu* of balls you own,
  not a world actor; the only other hits are debug execs (`ATDebugWishDragonBall`,
  `ATDebugChangeDragonBallUsedState`). So this is most likely a **bug, not a feature**: find out
  whether the balls carry an `ATMapIconComponent` at all, and whether `bShowMapIcon` is what hides
  them before the Dragon Radar is obtained (a variant reader that ignores that flag already exists,
  `nav_tracker.lua:460-473`). One MCP session standing next to a ball should settle it.
- **Announce a boss / unusually strong enemy and its direction.** Most of this exists. `SpawnType`
  is reflected and `ENEMY_NOUN_BY_SPAWN` already maps `3` → `cat_enemy_boss`
  (`nav_tracker.lua:1182`); `ICON_GROUP` separately knows `EVIL_ENEMY_BOSS` (31/35) and
  `GRD_SuperEnemyGroup` (50); `enemy_level()` reads the level; and `enemy_alert()`
  (`nav_tracker.lua:1476-1516`) already announces direction, elevation and distance of the nearest
  enemy every `SCAN_EVERY`. Missing: a distinct alert for a boss / over-levelled enemy with its own
  threshold and cue. There is **no battle-power field** — level is the only number. Still unused and
  worth trying: reflected `GetPowerCompareRank` on `UCharacterAuraComponent` (0–6 relative strength
  vs the player), noted in [status level](reference/dbz-kakarot/notes/dbz-kakarot-status-level.md).
- **Improve the character detector.** Needs the player to say what actually fails (names, range,
  misses, noise) before it can be scoped.
- **Accessibilize the DLC menu.** No adapter exists. The overworld ring entry is already named
  (`START_TOP_LIST_ID` DLC = 14; widget selector `DLC = 35` in `i18n.lua`), but the screen itself is
  unread. Requires DLC installed and a census with the menu open — and it must be **directory-mapped
  by pointer, not scanned**, because every class name that is never instantiated costs a ~65 ms
  `FindAllOf` every `ABSENT_BACKOFF` (~4 s) forever.

## Backlog

Open work only. How each item was derived is in the archive and the git log.

- **2026-07-15 batch, coded and never verified in game**: quest HUD, level-up toast, radar
  `resume_pick`, subtitles option gate, episode-card reader (`screen_questcard.lua`), Soul Emblems
  grid.
- **Battle results read a constant "222" for every stat.** Read `dumps/dump_results.txt` (round 2
  dumps each brush material's parameters), fix the decode or pin the value natively, then turn
  `DEBUG` off. `screen_results.lua`.
- **Cooking**: full flow re-test (dish list → cook → exit) and the detail-pane markup strip.
  `screen_cooking.lua`, `screen_shoplist.lua`.
- **Soul Emblems grid**: cursor MOVEMENT still unverified. One session with `GRID_DEBUG=true`, read
  `dumps/dump_community.txt`, then turn it off. `screen_community.lua`.
- **D-pad SNAP for the Community board free cursor** (both browsing and placing an emblem). All the
  geometry is mapped; what is missing is a WRITE primitive (a `mem_bridge` poke of the cursor
  translation, or steering the game's own input) plus registration on `pad_poll.lua` — never a new
  20 ms loop. Unify with the socket-board request.
- **"View Controls"** (from the battle pause) reads jumbled, and the pause does not re-announce on
  return.
- **Niceties**: skill-palette plates 4/7 are structural and the cursor never lands there; in assign
  mode the A/B press is silent until the first cursor move (no signal exists for the press).

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
| Door / area transition (building exits) | `AATDoorVolume` < `ATriggerBox`: `AreaMessageId` FName @0x358, `ConfirmationMessageId` FString @0x360, `DoorName` FName @0x370, `AreaName` FName @0x378, `bOnlyUsedInRoom` @0x350, `bUseDialog` @0x380, `PlayerStartTransform` @0x390, `DestinationDoor*` @0x560; fires on `OnActorBeginOverlap`, doors are PAIRED so one class serves both directions. Blueprint is `BP_Door`. **No EMapIcon enumerator for doors exists** (whole enum checked, AT_enums.hpp:6746-6823) — so the minimap cannot supply them and a direct `FindAllOf("ATDoorVolume")` is the only route. Related but NOT the exit: `AATCharacterRestrictGate` (area boundary), `AATWarpPointWithMontage`, `APortal` | AT.hpp:13004 (read directly); `nav_tracker` "exit" group |
| Gathering-point classes | fruit/small-fish = `ASpawnerFruitVolume`/`ASmallFishSpawnVolume` (< `ASpawnerItemVolumeBase`, volumes, NO taken state); ore/bugs/chests = `AMineralMiningPointNormal/Rare`, `ATreasureAccessPoint`, `AInsectAccessPoint` (< `AAccessPointBase`, `InteractState` @0x348, Taken = 11); EMapIcon: 5 FISHING, 6 COLLECTING, 7 HUNTING, 8 ORE, 18 FOODSTUFF_COMM, 63 BugNest, 68 VolcanoRockTrader, 69 ShopBug | CXX dump sweep 2026-07-17 |
| Char-icon tokens (soul emblems etc.) | romaji-derived 3-letter tokens on `/Game/Art/UI/Charicon_Ev/Ev_<Tok>NN` textures; `Mrs`=Muten Roshi, `Mst`=Mr. Satan (Misutā Satan), `Msn`=UNKNOWN third char (speaks raw) | pak string scan 2026-07-17 + user report (Roshi emblem); `screen_community.CHAR_TOKENS` |
| Level-up banner | `Info_Log02_C` < `UAT_UIInfoLog02` — bars `Info_Log_Bar_00..04` (`AT_UIInfoLog02Bar.TextBox` @0x3C0); `Info_Log_Level_C` DOES NOT EXIST in the ObjectDump | Info_Log02.hpp + ObjectDump + F7 census 2026-07-17 |
| Minigame result sheet | `Mgame_Result_C` < `AT_UIMiniGameResult`: TxtCap00 @0x478 / TxtName @0x480 / TxtCap01 @0x488 (BP-tree twins `Txt_*`), rows `Xlist_Bar02_NN`; owner field fm.MiniGameResult @0x630 (unmapped, lazy-field risk) | ObjectDump + F7 census 2026-07-17 |
| Cinematic character-intro card | `Info_Name_C` (BP) < `AT_UIInfoNameCore`: NameTxt @0x3A0, NameTxt_Large @0x3A8, PopularNameTxt @0x3B8, PopularNameTxt_Large @0x3C0, In/OutAnim; container `AT_UIInfoName.InfoCoreCtn` @0x3A0, owner fm.InfoName @0x668 (unmapped); data track `ATMovieSceneCharaIntroductionSection` (CharacterID/PopularNameID/IsLargeSize) | ObjectDump + AT.hpp:33663 + ATSpecialTrack.hpp:238 |
| All other native offsets / class names | — | See `native_offsets.lua`, `dumps/`, and `code/` (Ghidra) |

## Living rules and known limits

Things that will bite again. The engineering rules themselves live in
[`CLAUDE.md`](CLAUDE.md) / [`PRINCIPLES.md`](PRINCIPLES.md); these are the ones specific to this
game's runtime.

- **`PROBE_ENABLED = false` in `ui_core.lua` — do NOT re-enable it.** The resurrect probe froze the
  game twice (boot hang at the Bandai logo, then a mid-session freeze). General form: **no
  speculative object lookups — `StaticFindObject`, `FindAllOf` bursts — at any time the engine might
  be async-loading, which on this game is any time.**
- **One `FindAllOf` costs ~65 ms here (max 272 ms).** Per-class polling is bankrupt; detection goes
  through the screen directory (pointer reads) with `Core.boost_missing` on screen commit/close.
  Backoff: destroyed pools ~4 s, never-seen classes ~8 s.
- **`scan_allowed` must stay a pure BUDGET (`SCANS_PER_TICK = 2`), never a time gate.** A time gate
  starves adapters by sweep position, permanently. Known limit: while a menu is held open, effective
  throughput drops to ~0.67 scans/tick.
- **`Core.array_of` is the only valid way to read a reflected TArray.** `owner[prop]` yields an
  INVALID RemoteObject rather than nil, and a raw `GetArrayNum` on it is the uncatchable throw
  (0xe06d7363).
- **No construction notifies.** UE4SS delivers `NotifyOnNewObject` on the async loading thread too,
  which corrupts the shared `lua_State`. Screens are detected by polling cached refs.
- **A directory-mapped class NEVER falls back to a scan** (`ui_directory.lua`, ~40 classes), so a
  misspelled field is a silently dead reader. Roots are found with `FindAllOf` skipping `Default__`.
- **Never add a new 20 ms `LoopAsync`.** Every pad stepper registers on `pad_poll.lua`. Loops outside
  the registry (`battle_monitor`, `quest_objective`) must call `Core.begin_scan_tick()` at the top of
  their step.
- **Pad-press boost**: 300 ms cooldown between windows plus one bypass scan per generation in
  `cached_live`. Removing either reproduces the scan storm (~57k scans, 1 s spikes).
- **F7 (`discover.lua`) can go fatal** if the UI dies mid-animation (mitigated by a 3-failure fuse in
  `brush_of`) — avoid it during transitions.
- **R3 picker**: `hooked=false` at boot means the pad block is read-only; a `GetProcAddress` /
  inline-hook fallback is still missing.
- **After any game patch, re-verify `native_offsets.lua` with the F4 probe.**

## History

- [STATUS history to 2026-08-14](reference/dbz-kakarot/notes/dbz-kakarot-status-history.md) — this
  file's full diary before the prune: every derivation, refuted hypothesis, per-batch test plan and
  in-game report.
- [`reference/dbz-kakarot/notes/`](reference/dbz-kakarot/notes/) — one note per subsystem, indexed in
  its own README. The [crash ledger](reference/dbz-kakarot/notes/dbz-kakarot-crash-bug.md) is the one
  to read before touching any UObject access.
- [`PROGRESS.md`](PROGRESS.md) — the original session handoff (last updated 2026-07-04, historical).
- [`CHANGELOG.md`](CHANGELOG.md) — what shipped to players.
