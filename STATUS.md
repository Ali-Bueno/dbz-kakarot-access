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
| Items inventory + Party + Characters | wip | Party/Characters done. **Items list mute when it HAS items** (only reads empty) — diagnostic re-armed in `screen_list.lua` (`DEBUG=true` → `dumps/dump_items.txt`); needs a live capture to pin the failing path |
| Item submenu (use-item char select) | todo | `UAT_UIItemMenu.WL_Start_Party_Bars` + `RequestPlayInCharaSelectAnim`; each bar's `Param.Name/Level/hp*/sp*` is readable — pending confirm of what the submenu shows + build |
| Save / Load data slots | wip | `screen_saveload.lua` — `AT_UIStartSaveLoad`. List is a VIRTUALIZED 3-bar window (`UISaveLoadBar_List`), so pool-position ≠ ordinal (index stuck at 3 for slots 3/4/5). FIXED via native index: `native_offsets.saveLoad.selectedIndex = 0x410` (ordinal = +1), cursor bar = window slot at `windowPos = 0x418`; both CONFIRMED by F4 runtime diff over ~11 saves (0x410 stepped 0..10; 0x414 = scroll offset). `Mem.i32` path + Border_Cursor fallback; SETTLE_TICKS debounce drops mid-scroll title-only/flashing-empty frames. `DEBUG=true` → `dumps/dump_saveload.txt`. Pending in-game verify of the fix. OPEN: first entry from title "Continue" doesn't read until a dialog cycles (separate on_screen/entry issue) |
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
Verify the 2026-07-11 menu batch: **Save/Load** (`screen_saveload.lua`) and **Quest objective HUD**
(`quest_objective.lua`) — both load via Ctrl+Shift+R except the **F10** objective key (needs a game
RESTART, it's a new keybind in `main.lua`). Then, for the **Items** list: open it with items present,
move the cursor, and send `Scripts/dumps/dump_items.txt` (diagnostic re-armed) so the mute-when-populated
path can be pinned. And confirm what the **item submenu** (press confirm on a usable item) shows, so the
`WL_Start_Party_Bars` char-select reader can be built. Still pending from before: the "Caza"/Hunt radar
category verify, the R2 picker hook log, and the radar 2.0 batch.

## Known issues / open questions
- Cooking menu cursor read pending re-verify — confirm `dumps/dump_cooking.txt` shows the frozen-index diagnosis, then set `DEBUG=false`.
- R2 picker: if boot log shows `hooked=false`, pad blocking is off (read-only) — needs a GetProcAddress/inline-hook fallback.
- "View Controls" (from battle pause) reads jumbled and pause doesn't re-announce on return.
- After any game patch, re-verify offsets in `native_offsets.lua` via the F4 probe.

**Detailed history:** see PROGRESS.md.
