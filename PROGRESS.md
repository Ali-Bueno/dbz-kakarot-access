# DBZ Kakarot Accessibility — Progress & Next Steps

_Session handoff, last updated 2026-07-04._

---

## R2 RADAR TARGET PICKER — BUILT 2026-07-04, PENDING in-game verify (needs a game RESTART)

A controller overlay to pick what the audio radar tracks. Press **R2** in the RPG
overworld → a modal target list opens and the game freezes; **L1/R1** switch category,
**d-pad** moves through items, **A/Cross** selects (radar starts guiding), **R2 again**
cancels, **B/Circle** closes and stops the current tracking. RPG-only: it can NEVER open
or block input in combat/cutscenes/menus.

**New native bridge `input_bridge.dll`** (`src/input_bridge/`, build.ps1 like the
others). The game reads the pad via `XINPUT1_3.dll!XInputGetState` (confirmed in the
exe imports). The bridge IAT-hooks that import (exe first, then any loaded module):
the wrapper calls the real function, caches the TRUE pad state for us, and — while
`block(true)` — hands the GAME a neutral pad so nothing triggers. Exposes
`install()/poll()/block()/is_hooked()`. No xinput link (the real pointer comes from the
IAT slot). SEH-free but pointer-safe; static Lua 5.4.4 like the other bridges. If the
import can't be patched it falls back to read-only (poll works, block is a no-op) — check
`is_hooked` in the boot log.

**Lua pieces:**
- `input.lua` — wrapper: `Input.init/read/block`, XINPUT button bitmask constants, R2
  trigger threshold. `Input.init()` in main.lua (before the protected snapshot, so the
  hook installs ONCE and survives Ctrl+Shift+R).
- `radar_menu.lua` — the modal picker (own 60 ms loop, gen+busy guard). Toggle model:
  R2 edge opens (blocks pad, builds list, announces), A selects, R2/B close. On close the
  pad stays blocked (a "drain") until every button/trigger is released, so the closing
  press never reaches the game. Force-closes if `Nav.field_ready()` goes false.
- `nav_tracker.lua` additions (all world reads live here, behind the radar's gates):
  `Nav.field_ready()` (RPG-only gate = not ui_muted AND minimap visible),
  `Nav.list_targets()` (minimap `MapIconList` + navi icons grouped by EMapIcon into the
  L1/R1 categories; non-quest items distance-limited by each icon's own
  `SearchRangeRadius`, else a 300 m cap; quests unlimited), `Nav.item_phrase()`,
  `Nav.set_manual_target()` (manual = auto-scan won't steal it), `Nav.stop_tracking()`.
  **Manual targets STOP on arrival** (beacon quiet + dropped; re-pick to track again);
  auto quest targets keep the re-arm behavior.
- EMapIcon→group map + per-type spoken nouns in nav_tracker; category/noun strings in
  i18n (ES+EN): `radar_cat_*`, `cat_*`, `radar_nothing`, `nav_stopped`.

**TEST (RESTART the game — main.lua changed + a new DLL loads; Ctrl+Shift+R is NOT
enough):**
1. Boot log should show `input_bridge loaded (hooked=true)`. hooked=false → blocking off
   (read-only); tell me and I'll add a GetProcAddress/inline-hook fallback.
2. In the field, tap R2 → hear a category + "noun, N meters, i of n". Move d-pad; switch
   category with L1/R1. The character/camera must NOT move while it's open.
3. A/Cross on an item → menu closes, beacon guides you there; arriving stops it.
4. R2-again and B both close; B also stops the current beacon.
5. In BATTLE, R2 must do its normal combat thing (menu must NOT open, input NOT blocked).

---

## UPDATE (2026-07-03, later): radar verified in-game + fishing + stability

The radar WORKS in-game (auto-tracks quest markers, arrival cue confirmed, F5 answers).
Iteration findings, all live-verified with the user:

- **Player position**: `PlayerController.Pawn` is UNRELIABLE in the overworld (a stale
  `TwinFootBP_C` at the origin) — use `AT_UIMiniMapRadar.PlayerIns` (the game's own field
  character pointer); camera via `.CameraMng`.
- **Target sources**: navi icons first (`AT_UIMiniMapNaviIcon.TargetActor`, filter by
  `icon_in_use` = one of its widgets on_screen); fallback = quest-typed `MapIconList`
  entries. Auto-retarget per quest step confirmed working.
- **Companions (Shift+F5)**: cycle nearby `AT_Character` actors (exclude the player by
  address AND the game's parked preload pool — ~100 characters sit at one distant spot).
- **CRASH lesson (return to title, crashed twice)**: level teardown happens BEHIND the
  confirm dialog + loading screen. The minimap lives under the PER-LEVEL HUD (unlike the
  pooled GameInstance menus), so even probing it during teardown is an uncatchable abort.
  FIX: when any muting adapter owns the screen (`ui_muted`, pure Lua), the nav loop
  touches ZERO engine objects; world gate (minimap hidden) second; actors last.
- **PERF lessons**: never FindAllOf while loading (stretched a 15 s save load past 1 min
  — nav's minimap acquisition now backs off + never scans while a screen owns the
  display); radar tick 100 ms (50 ms cost visible lag); per-cursor-move dev_log writes
  caused menu lag spikes (now behind DEBUG flags); ui_core cache refreshes raised
  (ABSENT_BACKOFF 40, REFRESH_EVERY 100). `Ctrl+F5` dump now includes step-time telemetry.
- **FISHING minigame accessibilized** (`screen_fishing.lua` + `native_offsets.fishing`):
  reads instructions/popups/mash-QTE, announces the action button (resolved semantically;
  face buttons via the KeyConfig asset's IconName↔ControllerId pairing, asset LoadAsset'd
  on demand), and sonifies the timing: cursor state lives in the NON-reflected tail
  (cursor 0x518 sweeps ~52..124; zone [0x520,0x524] width 37 random per attempt; both
  user failures froze OUTSIDE the zone = model confirmed). Continuous soft sine tone
  (new `audio_bridge.tone(vol,pitch)`, seamless 220 Hz loop): pitch rises approaching
  the zone, jumps high inside = press now. New attempt detected by zone change.
- **Registry**: `active_adapter()`/`active_index()` accessors; adapters can opt OUT of
  muting the radar with `nav_mute = false` (chatter, resident controls guide); the
  dialogue reader yields while the shared menu header `Xcmn_Header_C` is up (menus were
  shadowed by lingering talk windows).

**PENDING VERIFY after next restart:** fishing tone + button announce (Ctrl+F5's
`keyconfig bindings` line shows the resolver state), return-to-title no longer crashes,
menu lag gone (check `ui step ms` in the dump).

### COMMUNITY / SOUL EMBLEMS — BUILT (2026-07-03), PENDING in-game verify

User request: the "Soul Emblems Received" notice read only title+body (no emblem rows),
and the Soul Emblems GRID (screenshots: LB/RB pages 6/7, yellow cursor frame, "?" =
unacquired) read nothing. Three changes:

- **`screen_dialog`**: the notice/reward windows are `Xcmn_Win01_C` whose native base
  `UAT_UIGameWindow` exposes typed CONTENT POOLS — now read as the message's tooltip:
  `WL_ItemPlateCtn` (ItemName+ItemNum), `WL_TextCmuCtn` (**soul-emblem rows**:
  WL_CharName + WL_LevelTitle + WL_LvTextList levels), `WL_LinkListPlateCtn`,
  `WL_CheckPlateCtn`, `WL_DetailReward`. Also `choices()` now includes the
  window-owned `WL_TextPlateCtn` rows (nested Xcmn_Win00_Choice_C — e.g. the Sort
  popup), same Img_Xwin01_List highlight rule.
- **`screen_community.lua`** (NEW, registered before screen_field): GRID mode — host
  `AT_UICommunityStart` (native find), slots `EmbList.EmbAry`
  (UAT_UIXCmnEmb_Cursor); the selection signal is unverified, so 4 candidates
  (Ins_Frame_Set / CanvasEffectA/B / AnimLoop playing) are probed and the first that
  singles out EXACTLY ONE slot wins (sticky once found; wrong candidate = silence,
  never a wrong read). Speaks slot text (Txt_Commu), "not acquired"
  (UIXCmnEmb.ImageUnacquired = the "?"), "new", and "i of N". DETAIL mode — 
  `Start_Commu_Detail_C` (blueprint in CXX dump): Txt_Name as name; tooltip =
  Txt_Commu_Lv, Txt_Popular00, Txt_Link, Txt_Link_Detail, Txt_Char_Detail,
  Reward_Bar00..02 (Txt_Reward + Txt_Num + Txt_Max00).
- `DEBUG = true` in screen_community appends per-slot signal samples to
  `dumps/dump_community.txt` (which visual marks the cursor, per-slot texts, face
  textures) — if the grid stays silent, that file is the ground truth. i18n keys
  added: pos / not_acquired / new_label; screen name = I18n.header(5).

**LIVE ITERATION (same day, dump_community.txt):** cursor = the slot whose AnimLoop
is PLAYING (transient → last index held while idle); Txt_Commu = the emblem's
COMMUNITY LEVEL (now labeled); acquisition = face brush is a MaterialInstanceDynamic
(whose texture param is the character icon) vs the constant "?" mask — CAUTION: the
MID is ALSO named "Ins_Emb_Mask", matching on the mask name made everything read
"not acquired". Character NAME = the icon texture's token
(…/Charicon_Ev/Ev_Gok00… → "Gok" → "Goku") via the CHAR_TOKENS table in
screen_community.lua (full token list extracted from the pak index; unmapped tokens
are spoken raw — if the user hears a bare token like "Bnw", add that entry).
DEBUG off. **Verify**: grid reads "Goku, community level 3, 1 of 21" / "Not
acquired, 8 of 21"; detail on A; Sort options on X; the received-emblems notice
rows.

### COMMUNITY BOARD — SOLVED + VERIFIED IN-GAME (2026-07-04). Story unblocked.

**Live-verified with the user (2026-07-04, ~2 AM):** the cursor reader works end to
end — sockets announce as the stick moves ("Goku, leader…", "Empty socket, N of 11"),
the user picked Gohan from the grid, heard "Emblem in hand…", placed him in socket 2
next to Goku (dump: held→false, cache=2, mode→16 link-bonus demo) and **completed the
tutorial that blocked story progress**. The frozen-cursor mystery was exactly the
tutorial-mode gate described below. DEBUG is now OFF in screen_community.
Added right after the verify (all additive+guarded, PENDING verify):
- **Link-bonus subtitles**: the registry gives the screen to ONE adapter, so the
  dialogue reader never runs while the board owns it → `board_subtitles()` reads
  `host.LinkBonusSubtitles` (UATUISubtitles TextName/TextSelif) diff-gated.
- **Community Skills list** (user OCR'd it as unread): detail pane `WL_SkillParts`
  is a FIXED array of 10 (detail+0x470, UE4SS collapses to elem 0) → elements 1..9
  via RegisterCustomProperty on the runtime class (screen_party technique); each row
  speaks name+threshold, active variant preferred; appended to the entry summary.
- **GIFT picker** (user request): the emblem detail's PresentBtn (blueprint
  Start_Commu_Pr_Btn) opens a shared game window whose `WL_ItemPlateCtn` rows
  (UAT_UIGameWindowChoice: ItemName+ItemNum) are SELECTABLE — same Xwin01List
  highlight as Yes/No (native member `ImgXwin01List`, no underscores).
  `screen_dialog.choices()` now engages item rows as a navigable list WHEN one is
  highlighted (no highlight → unchanged notice behavior, so reward windows don't
  regress), speaking "item, count" per move + the live `WL_TxtDetailItem`
  description as tooltip. screen_dialog DEBUG left ON one more pass: if the gift
  window reads wrong/silent, dumps/dump_dialog.txt shows its pools; if NOTHING
  reads (not a message window at all), take an F7 discovery dump inside it.

Original static solution write-up:

The whole mystery is closed. Ghidra pass on the unpacked exe (scripts
`code/ghidra/find_commu.java` — anchors the class's native-registration table on its
UNIQUE names PlayIn/OutCommRank, then walks the contiguous {name, exec} pairs — plus
`decompile_addrs.java` / `dig_commu_callers.java`; output in `code/decompiled/`):

- **The board tick is `FUN_1414c7de0(this, ?, dt)`** (commu_tick_callers.c) — a mode
  state machine on `host+0x500`: 1 intro, 7 FREE-CURSOR BROWSE, 9 detail, 10 Soul-Emblems
  grid open, 12/13/14/17 TUTORIAL POPUP WAITS ("Comm_TutorialEvent_EmptyPanetChoose",
  "TUTO_MNU_24_01"), 16 link-bonus demo. **The hover tracker runs ONLY in mode 7** —
  in the tutorial modes the stick is DEAD by design. That is why every live diagnostic
  (d-pad AND left stick) saw 100% frozen memory: the user was inside the community
  tutorial's popup chain. Not an input-device issue, not a read bug.
- **The game caches the hovered socket at `host+0x5D8`** (int32, 1-based WL_PanelTbl
  index; -1 = none/empty-data socket; 0x80000001 = just-switched sentinel), written
  every mode-7 tick by its hover tracker `FUN_1414e3170`.
- **The hit-test is `FUN_1414f2ab0(frame, bCheckData)`** (manual_1414f2ab0.c): cursor =
  `frame.WL_PanelCursor->RenderTransform.Translation` (+0x90/+0x94, the exact field we
  always read — it was frozen because the cursor genuinely never moved); socket pos =
  `panel.PointerCenterOffset` (0x3F0) + hidden board pos (panel+0x550) + hidden frame
  adjusts (frame+0x428/0x42C, leader 0x430/0x434), square range frame+0x420 (leader
  0x424), bounded by `frame+0x628` (active count), iterated LAST→FIRST, first hit wins.
- Pick/place: `frame+0x7B8` = held-emblem ptr (non-null while carrying), `frame+0x7C0`
  = source socket; the LEADER socket (index 1) always refuses pick/place (error buzz).
  `host+0x4F3` = pick/place sub-state (6 picking / 7 placing).

**Implemented in `screen_community.lua` (offsets in `native_offsets.commuBoard`):**
board mode gate (track only in mode 7); hovered = game cache at 0x5D8 when >=1, else
our 1:1 hit-test replica (also names EMPTY sockets); announces "<char>, level N,
leader, socket i of n" / "Empty socket, i of n" per move, "free" when leaving all
sockets; "emblem in hand…" hint on pickup/grid return; the entry summary now numbers
each placed emblem's socket so it correlates with the live cursor. DEBUG=true for one
pass: `state` snapshot every ~2 s (host addr, instance count, mode, sub, held, cursor
xy, n, hovered, cache) + the hunt-v2 memory diff (host tail, UAT_UICmnInput 0x460,
UATCommunityBoard model 0x138, menu, WL_PnlCurs, frame gap) as the safety net.

**TEST CHECKLIST (first board visit):**
1. Dismiss any tutorial popup on the board (the reader/dialog reads it; press its
   confirm/close). Cursor only works after that (mode 7).
2. Move the LEFT STICK slowly: expect per-socket announcements; between sockets "free".
3. Confirm on an empty socket → grid opens (already accessible) → pick Gohan → back on
   the board expect "Emblem in hand…" → move to the socket NEXT TO GOKU (socket numbers
   were read on entry; Goku's socket says "leader") → confirm places it.
4. If moves stay silent: read dumps/dump_community.txt — the `state` lines show mode
   (7 vs tutorial), cursor xy moving, hovered/cache; `hunt` lines catch anything else.
5. Afterwards set DEBUG=false in screen_community.lua.

### COMMUNITY BOARD — superseded attack plan (2026-07-04, kept for history)

Status: the board's SUMMARY (title, overall level, to-next-rank, rank, active skills)
and the PLACED emblems + a "press confirm to open Soul Emblems" hint are read on entry
(commit 53606ef, `screen_community.board_update`). The live FREE CURSOR over the sockets
is NOT tracked — the user can still complete the tutorial by pressing confirm on an
empty socket → the accessible Soul Emblems grid opens → pick the emblem.

WHY the cursor is unread (all verified live 2026-07-04, dump_community.txt):
- Host is `Start_Commu_Brd_C` (ONE instance, we read the right one). Frame =
  `.WL_BrdFrame` (UAT_UICommunityBoard_PanelFrame). Sockets `WL_PanelTbl` (11), each
  `WL_Emblem` face → character name via CHAR_TOKENS; labels are CORRECT.
- `WL_PanelCursor.RenderTransform` (0x90) stayed **542,319 frozen** across a full
  "3 left, 3 down" move pass. Socket panels' RenderTransforms are in a DIFFERENT space
  (-400..300) — so nearest-to-cursor was always wrong anyway.
- A wide memory diff of **0x8C0 bytes** (host 0x3F0..0x760 + frame 0x430..0x950) showed
  **ZERO change** while the pad moved. So the selected-socket index is NOT in the
  widget memory — like battle pause, it's in the native input/logic layer.

**NEXT-SESSION ATTACK PLAN (in priority order):**
0. **INPUT DEVICE (test THIS first — likely the whole cause):** the user was moving the
   cursor with the **D-PAD**, but this board's cursor is a FREE pointer driven by the
   **LEFT STICK** (the help bar shows "Move Cursor" with the stick glyph). If so, the
   cursor never moved and EVERY frozen diagnostic below is meaningless. Re-test with the
   LEFT STICK before concluding anything — the `hunt` scan (commit 753eabd, cursor widget
   + slot memory) should then light up and hand us the cursor position immediately.
1. **CONFIRM THE INPUT (cheap, do before any RE):** also the user was in the COMMUNITY
   TUTORIAL, whose popups persist and may FREEZE the cursor. Re-test on a NORMAL board
   (tutorial done) and re-run the wide diff. If something changes now, the whole "native"
   conclusion was a tutorial artifact. If still frozen, continue below.
2. **Cursor by SLOT, not transform:** read `WL_PanelCursor`'s CanvasPanelSlot position
   (Slot.LayoutData.Offsets or Slot Position), not RenderTransform — a UMG cursor often
   moves via slot. If THAT tracks the stick, replicate the hit-test: nearest socket by
   the panels' RenderTransforms (already dumped: 1:-60,138 2:306,-182 3:33,-171 …) +
   the board's reflected `HitRange`/`HitBaseOffset`/`CursorAdjust`.
3. **Scan the objects we did NOT cover:** `UATCommunityManager` (ActorComponent),
   `UAT_UICommunityManager`, and the FULL memory of each `UAT_UICommunityBoard_Panel`
   (we only read their transform/scale). Diff while moving.
4. **Ghidra (the pause/world-menu method):** `code/ghidra/find_ufunc.java` on the board's
   UFunctions — `CheckHovered`, `MouseClickDecide`, `SetSelectTab` — to see which field
   holds the hovered-socket index (or confirm it's computed on the fly from the cursor +
   HitRange, in which case #2 is the real fix). Board host is UAT_UICommunityBoard;
   HitRange/HitBaseOffset/CursorAdjust/GridInterval* are its reflected hit-test params.

DECISION LOG so a repeat isn't tried: geometry-nearest (cursor in wrong space, dead),
panel scale (=leader pedestal, not selection), native +0x500 (=froze at 10, was the
release-level guess), RenderTransform of cursor (frozen). Don't re-try these.

### COMMUNITY BOARD — CURSOR PINNED NATIVELY (2026-07-03, late) — SUPERSEDED (see above)

Iteration findings (dump_community.txt + dump_dialog.txt, user passes):
- Board host = **`Start_Commu_Brd_C`** (pak index; FindAllOf on native AT_UICommunityBoard
  finds NOTHING). Grid stays rendered UNDER the board → board checked FIRST (frame
  on_screen gated). Title + 11 socket labels verified perfect in the dump.
- **Every canvas slot on the board reads 0,0** (render-transform positioning) → geometry
  dead. The in-adapter TAIL DIFF (memdiff technique, auto on cursor move) pinned the
  hovered socket: **`Start_Commu_Brd_C +0x500`** (0-based into WL_PanelTbl; 0x5D8 flips
  1/-1 on/off socket; 0x4F8 = coordinate). In native_offsets.commuBoard; board_selected
  reads it bounds-checked (silence, never a wrong socket).
- **Notice-release verified working**: dump_dialog captured the full window flow (boot
  messages, meal Yes/No with highlight, Soul Emblems Received with Goku/Gohan rows) —
  window-owned choices and Cmu rows all read. The "2 side options" on the community
  notice are NOT window rows (dump shows none) — likely the board's LB/RB tabs beneath.
- DEBUG still ON in screen_community + screen_dialog for one more verify pass; then off.

### COMMUNITY BOARD + perf fix + dialogue choices + telops (2026-07-03, evening) — PENDING verify

- **GRID INPUT LAG FIXED** (user: "omega delay" on the emblem grid). Causes: the old
  build probed 2 anim signals × 21 slots + rebuilt the spoken label (GetFullName +
  texture-parameter walks) EVERY 100 ms tick. Now: ONE AnimLoop scan per tick and the
  label is cached (recomputed on index change or ~1 s, `cached_label` in
  screen_community). Same rule was the old dev_log lesson: no per-tick heavy reads.
  Verify with Ctrl+F5 `ui step ms` if it still feels slow.
- **COMMUNITY BOARD mode** in screen_community (`AT_UICommunityBoard`): sockets =
  `WL_BrdFrame.WL_PanelTbl` (each `WL_Emblem` face → same CHAR_TOKENS name read,
  `WL_Lv` level, leader pedestal, else "empty socket"); hovered socket = a panel's
  ActiveAnim if unique, else NEAREST socket to the cursor widget by canvas-slot
  position (`WL_PanelCursor`/`WL_Img_Curs_Fing00`/`WL_EmbCursorFrame` — positions via
  Slot.LayoutData.Offsets, NEVER RenderTransform which abort-crashes); board title
  (`WL_CommuBrdDetail.WL_Txt_Titl00`) = the announcer tab (LB/RB), and the summary
  (overall level / to-next-rank / rank / active skills) queues on board change.
  Socket positions cached per board. UNVERIFIED live: the cursor/socket mapping —
  if wrong, set DEBUG=true in screen_community and visit the board once.
- **`screen_choice.lua`** (NEW, above dialogue): dialogue/quest choices —
  `Choice_Cmd_C` rows on screen, label `Txt_Choice`, selected = `Dmy_Choice_Hover` /
  native `HoverImage` visible; prompt = `Choice_Win_C` message or the sub-story
  accept window's `Quest_Sub_C.TextBox_Title+Detail`, spoken once as the screen.
- **`screen_telop.lua`** (NEW, below dialogue, nav_mute=false): quest banners —
  `Quest_Main_Telop_C`'s 12 `Telop_Txt_*` slots (Txt_Caption + Txt_Name), each
  unique text spoken once (queued) and re-armed when the banner leaves the screen.

### COOKING menu — REVISED (2026-07-03, after first live test), PENDING re-verify

First live test: **entry spoke the full readout (recipe + effects + ingredients +
description), cursor moves spoke NOTHING, and the raw `<span color=…>` markup was
read out loud.** Diagnosis and fixes in `screen_cooking.lua`:

- **`GetSelectValue()` froze after entry** — like the pause/overworld menus, the
  live cursor of `UAT_UIMenuListBase00` apparently sits in its NON-reflected tail
  (0x450..0x4A0, class size 0x4A0, last reflected member ends 0x450). The selected
  recipe is now read from the **detail pane**, which the game repopulates per move:
  name = `CookMenuList.WL_CookWin_Cap_Title` (the right-hand window's dish title;
  the static captions — "Meal Effect", "Status Boost", "Materials Needed", "Have /
  Need" — live in separate `Cap_*` members, confirmed in dump_1782938804_003),
  falling back to the `ListPlateCtn` row. A **selection signature** (raw title +
  first effect + description) calls the new `Announcer:invalidate()` (ui_core) when
  the pane is rewritten without the spoken name changing, so a stale name source
  can never silence the reader again.
- **Markup stripped**: every spoken text now goes through `A.markup_to_speech`
  (the reflected `GetText()` returns raw `<span color="#…">Ki ATK</>` tags).
- `DEBUG = true` appends one line per selection change to `dumps/dump_cooking.txt`
  (`idx=` GetSelectValue vs `title=` pane title) — if moves still don't speak, that
  file is the ground truth (does idx really freeze? is Cap_Title really the dish?).
  Turn it OFF once verified.
- Unchanged: genre tab (`WL_Txt_GenreTitle`, LB/RB), detail order (effects
  `Txt_Detail00_01..03`, perpetual `Txt_Detail01_01..03`, duration `Txt_Detail02`,
  ingredients `Shop_Cmn_Bar_01_00..` have/needed, description `Txt_Detail03` last),
  overlays (`WL_CookingComp.WL_Text` / `WL_CookingResult.TextBoxCtn[]`), registered
  BEFORE screen_shop (the menu embeds a `UAT_UIShopTop` via `WL_CookingTop`).

**To re-verify in game** (Ctrl+Shift+R reload is enough): arrows → each recipe +
detail, no `<span>` junk; LB/RB → genre; cook → complete/result read. Then read
`dumps/dump_cooking.txt` to confirm the frozen-index diagnosis and set DEBUG=false.

### Fishing minigame — VERIFIED WORKING END-TO-END (2026-07-03, user landed a fish)

Flow (see the fishing review link in the session): attract (wiggle stick, free) ->
HOOK (phase 1: bar + sweeping arrow, press inside the red zone) -> REEL (phase 2:
ring closes on a center button, press on alignment; the button CAN differ).

- **Phase state**: `Mgame_Fishing_C` tail — phase at 0x50C (1=bar, 2=ring, 0=idle),
  cursor 0x518 (triangle wave ~52..124), zone [0x520,0x524] (width 37, random),
  bounce count 0x51C vs the REFLECTED CursorLapLimit; 0x514 = slow timer.
- **Phase 2** = `AT_UIBattleRushSpeedCore` (own instance; the fishing widget's
  pointer to it is UNSET — find it live). Tail: ringSize 0x3E0 shrinks ~304/s to
  ringTarget 0x3E4 (=190); 0x3E8/0x3EC/0x3F0 = rating tiers 0.75/0.5/0.25 of
  |size-target|/target — a live press at ratio 0.747 FAILED (the 0.75 boundary),
  confirming the acceptance model. Its Xcmn_Btn_Plat carries the phase-2 button
  IN ADVANCE (readable while hidden).
- **Buttons are INDEXED face buttons** (glyph Btn00..03; ActionBtn structs carry pad
  ids 0..3). The KeyConfig asset's IconName pairing gave a WRONG mapping live;
  standard Xbox order (0=A,1=B,2=X,3=Y) is the source (A.platbtn_token / FACE_TOKEN,
  ui_archetypes). Announcements are BARE TOKENS ("X, luego A") — TTS length was
  costing the reaction window.
- **Assist**: CursorSpeed × 0.5 + CursorLapLimit × 2 (reflected, rewritten per
  attempt; verified live — cursor velocity halved in the dump).
- **Sonification**: continuous soft sine (audio_bridge tone voice) both phases —
  pitch rises on approach, high+louder through the accept band with a 2-tick
  reaction lead; "¡Enganchado! <letra>" on hook success (= the phase-2 announce).
- **DEBUG flag** in screen_fishing appends tail samples to dumps/dump_fishing.txt
  (still ON until the user lands a fish; then turn off).

---

## NEW (2026-07-03): Quest navigation radar — built, PENDING in-game verify

Audio radar that guides to the active quest objective, ported from the XV2 mod (same sounds,
same by-ear-validated formulas) and improved with NavMesh route guidance. Everything compiled
and the audio path is verified end-to-end outside the game (test harness: pan/pitch/arrival OK).

- **`src/audio_bridge/`** → `audio_bridge.dll` (3rd Lua-C bridge, XAudio2): `ping(pan,vol,pitch)`,
  `arrival()`, `stop()`. Sounds copied from XV2 into `Scripts/sounds/` (beacon.wav, arrived.wav).
  Needed `CoInitializeEx` + `ole32.lib` (XAudio2 2.9 enumerates devices via COM).
- **`Scripts/nav_tracker.lua`** — its own 50 ms loop (generation+busy guarded). Target = the game's
  own guidance markers: `FindAllOf("AT_UIMiniMapNaviIcon")` → `.TargetActor` (verified reflected),
  classified by the actor's `ATMapIconComponent.MapIconType` (EMapIcon 24/71 main, 26/40/47/53/59/72
  sub). Auto-tracks a new marker (quest accept), retargets on step change, drops after 3 missed
  scans. Cadence 120–850 ms over 5–200 m (3D dist), vol `1−0.6t`, behind-pitch `1+fb·0.3` (min 0.6),
  equal-power pan vs CAMERA forward. Arrival at 8 m (3D) → arrived.wav + "Objetivo alcanzado".
  Route mode (default ON): `NavigationSystemV1.FindPathToLocationSynchronously` (verified reflected)
  → beacon points at the next path corner, cadence still tracks the final objective; auto-fallback
  to straight line. Gated silent when a screen adapter is active OR the minimap is hidden.
- **Keys:** **F3** radar on/off (off = instant silence) · **Shift+F3** route mode · **F5** speak
  target (type, meters, clock, above/below) · **Ctrl+F5** dev dump → `Scripts/dumps/dump_nav_targets.txt`.
- **To verify in game:** (1) F8 pipeline OK; (2) accept a quest → should auto-announce "Rastreando…"
  and start beeping; (3) pan left/right correct (XV2 needed a by-ear sign flip — if mirrored, negate
  `pan` in nav_tracker.lua step()); (4) walk to the marker → arrival at ~8 m; (5) Ctrl+F5 dump if the
  scan finds nothing (check `in_use`/`pri` per icon and the navmesh probe line).

This is the "where did we leave off" doc. It captures what works, how the native side was
built, the key lessons, and exactly what to do next.

---

## TL;DR of this session

The project had **two long-standing dead ends** that pure-Lua/UE4SS reflection could not read
(the selected index was a private C++ member, invisible to reflection). Both are now **SOLVED**,
plus the whole overworld main menu:

- ✅ **Battle-pause menu** — announces the selected row per move.
- ✅ **Overworld main menu** (Items / Party / Characters / Story / Community / Dragon Balls /
  System / DLC) — announces each entry, in the game's language.
- ✅ Built a small **native memory-reader** (`mem_bridge.dll`) that reads the non-reflected
  members, kept **completely separate** from the screen-reader bridge (`prism_bridge.dll`).
- ✅ Decompiled the game with Ghidra to find the exact offsets (the `code/` folder).

**Next up: the 8 submenus** behind those main-menu entries. Starting with **System** (Save /
Load / Options / Tutorial / Title) so we can load saves and return to the menu while testing.

---

## How the native side works (the important new capability)

UE4 shipping builds have **no C++ RTTI for the game's own classes** and some menu state lives in
**non-`UPROPERTY` C++ members** that UE reflection (and therefore UE4SS Lua) cannot see. To read
those, we added a tiny native helper:

- **`mem_bridge.dll`** (`src/mem_bridge/mem_bridge.c`, built by `src/mem_bridge/build.ps1`) — an
  in-process Lua C module exposing **guarded** memory reads (`read_i32/u8/i64/ptr/bytes`,
  `readable`, `module_base`). Reads are wrapped in SEH, so a bad address returns `nil` instead of
  crashing the game. **It holds NO mod logic** — only raw reads. **Deliberately separate from
  `prism_bridge.dll`**, which stays the screen reader only.
- **`Scripts/mem.lua`** — Lua wrapper: `Mem.i32(obj, off)` etc. take a live UObject and read at
  `obj:GetAddress() + off`, all guarded. Also `Mem.fstring(obj, off)` reads a non-reflected UE
  `FString` (ptr@off + count@off+8, UTF-16LE → UTF-8).
- **`Scripts/native_offsets.lua`** — every native offset in ONE data file, so a game patch =
  re-run the Ghidra workflow and edit this file (no recompile).

### Confirmed native offsets (all in `native_offsets.lua`)

| What | Class | Offset | Notes |
|---|---|---|---|
| Battle-pause selected row | `UAT_UIXCmnPause` | **`0x43C`** int32 | steps 0→1→2, wraps |
| Overworld ring/list index | `UAT_UIStartTop` | **`0x4E4`** int32 | index into `UIStartTopList` |
| Overworld item's entry id | `UAT_UIStartTopList` | **`0x404`** u8 | a `START_TOP_LIST_ID` enum |
| Field header mode / label | `UAT_UIXCmnHeader` | `0x400` / `0x428` | header is a FIXED "Main Menu" title |

**`START_TOP_LIST_ID`** (the item's `0x404` byte, mapped by `I18n.startlist`):
`COMMUNITY=0, DRAGONBALL=1, ITEM=2, PARTY=3, QUEST(Story)=4, CHARACTER=5, SYSTEM=6,
COMMUNITY_BOARD=7, COMMUNITY_EMBLEM=8, SYSTEM_SAVE=9, SYSTEM_LOAD=10, SYSTEM_OPTION=11,
SYSTEM_TUTORIAL=12, SYSTEM_TITLE=13, DLC=14`.

---

## The Ghidra decompilation workflow (in `code/`)

Full detail in `code/README.md` + `code/hybrid-mod-feasibility.md`. Key facts learned:

1. **The exe is DRM-encrypted (SteamStub v3.1).** `.text` is 100% encrypted on disk; the entry
   point is in a `.bind` section. **Unpack first with Steamless** (installed at
   `D:\code\decompilers\Steamless_v3.1.0.5`) → `code/AT-Win64-Shipping.exe.unpacked.exe`. That is
   the file Ghidra analyzes.
2. **Ghidra 12** project at `code/ghidra/KakarotAT` (16 GB-heap wrapper
   `analyzeHeadless_kakarot.bat`). Two gotchas: the **"Embedded Media" analyzer crashes the JVM**
   on a malformed embedded PNG → disabled via the `disable_media.java` pre-script; and **Ghidra 12
   dropped Jython**, so all scripts are **Java**, not Python.
3. **No RTTI for game classes** → no automatic class→vtable map. The way in is **UE's native
   function-registration table** (`{const char* Name, FNativeFuncPtr exec}` pairs):
   `code/ghidra/find_ufunc.java` finds a UFunction's native impl by its ASCII name;
   `decompile_addrs.java` dumps any address. That's how the offsets above were found (e.g.
   `In_Curs` impl reads `this+0x4E4`; `FUN_1416bca00` returns `item+0x404`).

**To re-open / query the analyzed program** (no re-analysis):
```
D:\code\decompilers\ghidra_12.0_PUBLIC\support\analyzeHeadless_kakarot.bat ^
  "…\code\ghidra" KakarotAT -process "AT-Win64-Shipping.exe.unpacked.exe" -noanalysis ^
  -scriptPath "…\code\ghidra" -postScript <script>.java -log <log> -scriptlog <log>
```

---

## Dev tools (for mapping new screens)

- **F4 — universal memory probe** (`dev_memdiff.lua`): open any menu, tap F4 (baseline), move the
  cursor, tap F4 again. It reports `Class +0xNNN = value` for whatever int32 changed — this is how
  we pinned the pause (0x43C) and the overworld selection. Results are also appended to
  **`Scripts/dev_probe.txt`** (git-ignored) so they can be read directly, no relaying by voice.
- **F7 — reflection discovery** (`discover.lua`): dumps an on-screen widget's structure to
  `Scripts/dumps/` for mapping.
- **Screenshots**: pasting a screenshot to `C:\Users\ali-b\Pictures\Screenshots` (newest) is often
  the fastest way to establish ground truth — that's how the overworld menu's `START_TOP_LIST_ID`
  mapping was verified.
- **`libs/lua54/luac.exe`** — Lua 5.4.4 syntax validator; run `luac -p <file>` before reloading.
- **Ctrl+Shift+R** in-game reloads the whole mod's logic (feature adapters + i18n). `main.lua`
  (keybinds, `Mem.init`, `Speech.init`) needs a full game restart.

---

## Screens accessibilized so far (adapters in `Scripts/`)

Registered in `app.lua` (order matters — overlays first):
`screen_choicelist` (difficulty), `screen_dialog` (message/confirm popups), `screen_pause`
(battle pause — **native selection**), `screen_dialogue` (subtitles / NPC talk), `screen_tips`,
`screen_loading`, `screen_tutorial`, `screen_options`, `screen_shop`, **`screen_field`**
(overworld main menu — **native selection**), `screen_title`. Plus `keyhelp` (F2 button bar).

---

## What to do next

### 1. The 8 overworld submenus (priority — start with System)
Each main-menu entry opens its own screen. Plan per submenu: open it → check if the reader says
anything → grab a screenshot and/or an F4 probe → build a `screen_<name>.lua` adapter → test.

- **System first** (Save / Load / Options / Tutorial / Title). Likely uses the StartTop sub-list
  **`UIStartTopList06_Sub`** (System is main index 6) with items carrying the same `0x404`
  `START_TOP_LIST_ID` byte — and **those names (9–13) are already in `I18n.startlist`**, so it
  should be quick once we know the sub-selection offset (get it with an F4 probe inside System).
- Then **Items, Party, Characters, Story, Community, Dragon Balls**. Many likely reuse the
  **reflected `MenuListBase00/01/03` list** (`GetSelectValue` → `ListPlateCtn[idx].TxtName`), for
  which the mod already has a reader (`A.list_selected_row` in `ui_archetypes.lua`, used by
  `screen_shop`) — so a generic list adapter may cover several at once.

### 2. Loose ends from today
- **Tips multi-page**: the page-turn state is noisy to probe (two windows, heavy per-page churn).
  Best pinned with a narrowed F4 probe on the on-screen Tips window only.
- **"View Controls"** (from battle pause): reads jumbled and the pause doesn't re-announce after
  closing it. Need to identify that widget (F7 while it's open) — it's likely a Tips/controls
  window, and it opens *over* the pause (pause index unchanged, so no re-announce on return).

### 3. Housekeeping
- The header hook (`header_hook.lua`, F6) is now superseded by the native field reader — can be
  retired once the submenus are done.
- After a game patch, re-verify the offsets in `native_offsets.lua` via the F4 probe.

---

## Key lessons (so we don't relearn them)

- **UE4 game classes have no RTTI**; class names exist only as UTF-16 reflection strings. Reach
  native methods via the UE function-registration table, not vtables.
- **Non-`UPROPERTY` members** (selection indices, page ids) sit in the object's tail past the last
  reflected member — find the range from the CXX header dump, pin the exact offset with the F4
  runtime probe (fast) or Ghidra (authoritative).
- **A user screenshot beats guessing** for UI ground truth (I can read them).
- **Enums matter**: the overworld item byte was `START_TOP_LIST_ID`, not `EXCmnHeaderFontType` —
  always confirm the enum in the CXX dump (`AT_enums.hpp`) rather than assuming.
- Keep the native surface tiny and data-driven (`native_offsets.lua`) so patches are cheap.
