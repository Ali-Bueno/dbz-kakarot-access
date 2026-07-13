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
**Skill Palette VERIFIED & committed; group-crossing announcement pending verify.** The reader works
(user-confirmed 2026-07-13, DEBUG off) — see the section table for the derived facts. The 6 visible rows
span TWO groups (4 main slots + 2 in the second group, plates 5–6), which read seamlessly and confuse:
NEXT = announce the group title (`Active1stGroupTitleText` / `Active2ndGroupTitleText`, picked by
whether the selected plate has a button glyph) when the cursor crosses groups — implemented, needs an
in-game listen. Minor open question: plates 4/7 (structural, always-on border+blink) — if the cursor can
ever land on them they'd read via the stale-pane fallback (re-arm DEBUG if the user reports a bad row).
"Acquired skills" assign list (after pressing A on a slot) is a later nicety.
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
