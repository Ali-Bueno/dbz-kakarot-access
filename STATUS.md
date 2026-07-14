# STATUS — Dragon Ball Z Kakarot

> Per-mod status ledger / dashboard. Open this first when resuming the mod so progress isn't re-derived from the code each session. Keep it short — a dashboard, not docs. Update the **Next step** line and the section table whenever you finish a chunk. Derive every value from the game's real data — no guessed offsets.

**Last updated:** 2026-07-11

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
| Fishing phase/cursor/zone | `Mgame_Fishing_C +0x50C` phase, `+0x518` cursor, `[0x520,0x524]` zone | live verify; `native_offsets.fishing` |
| All other native offsets / class names | — | See `native_offsets.lua`, `dumps/`, and `code/` (Ghidra) |

## Next step
**Skill Palette committed (0b6e623, slots + groups verified); ASSIGN submenu v2 + CRASH FIX pending
verify (uncommitted).** 2026-07-13 in-game CRASH root-caused from UE4SS.log: NOT the skill reader —
`nav_tracker` steer_around → `raycast.lua` LineTrace calls raise a UFunction param-count Lua error
(expected 13, received 11) that pcall CATCHES, so the abort fuse never trips and the call retried every
tick until UE4SS died with a fatal ACCESS_VIOLATION. Fix: `RAYCAST_AVOIDANCE = false` in nav_tracker
(ray_api() returns nil; guidance goes direct) — raycast was already sentenced dead on this game
(uncatchable aborts Area02/04). LESSON: a pcall-CAUGHT reflected-call error retried per-tick can still
crash the game; fuse on Lua errors too if raycast is ever revisited.
ASSIGN mode v4 (movement state machine). Dump-proven (3rd capture, 2026-07-13): there is NO static focus
signal — the list is a permanent panel, `AllCurs` renders in BOTH modes (v3 dead: everything read as
assign again), the slot plate keeps its border while assigning. What discriminates is which cursor
MOVES: slot browsing sweeps the plate border with `GetSelectValue` frozen; in assign mode
**GetSelectValue comes ALIVE** (idx 0→1→2 with the plate frozen — earlier "dead list" captures were all
slot-mode) and the list ROW is cursor-synchronous while the PANE lags (row=Martillo/pane=Masenko line).
v4: mode = whichever cursor moved last (slot_moved → slots, idx_moved → assign); assign name = list row
(pane only for gated level/Ki/desc); `ann:reset()` on mode flip. Known cosmetic limit: pressing A/B
itself is silent until the first cursor move (no signal exists for the press itself).
"No entry announcement" ROOT-CAUSED via a speech-sink tap (`speech.lua` DEBUG_LOG, now off): entry
announcements WERE sent but got CUT 0.6–0.8 s later — partly the user's fast first move, partly OUR
re-announce when the pooled plate border settles onto the real slot after the opening animation (one
entry spoke a stale leftover slot). Fixed: entry SETTLE_TICKS=3 debounce (~300 ms; drop to 2 if entry
feels slow) + group title suppressed when equal to the category text ("Súper Ataque, Súper Ataque").
Entry VERIFIED reading 2026-07-13. Both debug taps OFF; everything committed.

**Menu latency — REDESIGNED to event-driven detection (2026-07-13, uncommitted, PENDING verify).**
History: sub-screen entries paid detection (scan budget) + `CONFIRM_TICKS=2` + settle (400 ms–1 s;
items had a latent ~10 s stale-cache case). First fix round (confirm_ticks=1 on deliberate sub-screens,
saveload SETTLE 2→1, churn-mark on Start_Item_C) cut entry cost but the churn-mark REGRESSED navigation
(per-tick re-scans saturating the budget — the known churn hazard). v2 (the better architecture, per
our own UE4SS reference §2): **`NotifyOnNewObject("/Script/UMG.UserWidget")` feeds the ui_core caches**
— constructed widgets are stashed by an ONCE-armed notify (survives Ctrl+Shift+R; handler via `_G`,
transition.lua pattern) and drained into `all_cache`/backoff-clear each poll tick (newest first), so a
(re)created screen is detectable within ~1 tick with ZERO extra scans. Both `mark_churning` call sites
REMOVED (mechanism kept as documented fallback); `REFRESH_EVERY` 100→300 (periodic re-scan now only a
safety net); pending feed wiped on transition (freed-object probes abort). confirm_ticks=1 + SETTLE=1
kept. **v2.1 (2026-07-14)**: first round verified skill palette fast but Save/Load + items still slow —
two feed holes, both fixed: (1) exact-name matching missed caches keyed by NATIVE BASE names
(`AT_UIStartSaveLoad` — a constructed instance reports its blueprint class), now the drain feeds every
tracked key along the CLASS CHAIN (`GetSuperStruct` walk, depth-capped 12); (2) the notify base was
`/Script/UMG.UserWidget`, which never delivers the non-UserWidget `CFUIMultiLineTextBox` pool behind
detail panes (items' text boxes invisible until the 30 s refresh) — now `/Script/UMG.Widget`
(guard flag `__KakarotWidgetNotifyArmedV2`; a stale narrow notify after hot reload double-delivers,
harmless). **VERIFIED in-game 2026-07-14 (nav fluid, all entries fast) — committed & pushed (14ac601);
the rule is now in CLAUDE.md §8: new adapters must use the event-driven cache, never per-tick scans.**

Backlog (older pending): verify Quest objective HUD F10 + reactive (`quest_objective.lua`); radar 2.0
batch + "Caza" category + R2 picker hook log; cooking menu re-verify; "View Controls" jumbled read.
Niceties: skill palette plates 4/7 (structural, unreachable so far); assign-mode A/B press silence.
F10 quest read: no Lua errors, bind present in main.lua — most likely the game wasn't fully restarted
(Ctrl+Shift+R doesn't re-run main.lua); reactive reader only speaks on objective CHANGE, so a pre-existing
objective staying on screen is expected silence. Re-test F10 after a full game restart.

Also still pending: verify **Quest objective HUD** (`quest_objective.lua`; F10 needs a game RESTART).
And older backlog: "Caza"/Hunt radar category verify, R2 picker hook log, radar 2.0 batch.

## Known issues / open questions
- Cooking menu cursor read pending re-verify — confirm `dumps/dump_cooking.txt` shows the frozen-index diagnosis, then set `DEBUG=false`.
- R2 picker: if boot log shows `hooked=false`, pad blocking is off (read-only) — needs a GetProcAddress/inline-hook fallback.
- "View Controls" (from battle pause) reads jumbled and pause doesn't re-announce on return.
- After any game patch, re-verify offsets in `native_offsets.lua` via the F4 probe.

**Detailed history:** see PROGRESS.md.
