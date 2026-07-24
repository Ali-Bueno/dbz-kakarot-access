# dbz-kakarot-community-board

> DBZ Kakarot community board SOLVED + VERIFIED in-game (Ghidra mode machine, hovered cache host+0x5D8); story unblocked


The Community Board (emblem-placement editor) in the DBZ Kakarot accessibility mod
([dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md)) — the screen that blocked story progress — was
**SOLVED with Ghidra and VERIFIED IN-GAME on 2026-07-04**: the user placed Gohan next
to Goku via the spoken cursor and completed the blocking tutorial. Details + write-up
in the repo's PROGRESS.md; decompiled sources in code/decompiled/. Two late additions
PENDING verify: link-bonus subtitles read by the board adapter itself (the registry
runs ONE adapter, so screen_dialogue can't cover a board-owned overlay), and the
Community Skills threshold list (detail WL_SkillParts = FIXED array of 10 →
RegisterCustomProperty on the runtime class, elements 1..9).

**THE WHOLE MYSTERY, RESOLVED:**
- Board tick `FUN_1414c7de0` is a mode machine on `Start_Commu_Brd_C+0x500`:
  7 = free-cursor browse (the ONLY mode whose tick runs the hover tracker),
  9 detail, 10 = Soul-Emblems grid open, 12/13/14/17 = TUTORIAL POPUP WAITS,
  16 = link-bonus demo. In tutorial modes the stick is DEAD by design → every frozen
  diagnostic (d-pad AND stick) was the user sitting inside the community tutorial's
  popup chain. Not an input bug, not a read bug.
- **Hovered socket = `host+0x5D8` int32** (1-based into WL_PanelTbl; -1 none;
  0x80000001 sentinel), written each mode-7 tick by hover tracker `FUN_1414e3170`.
- Hit-test `FUN_1414f2ab0(frame,b)`: cursor = WL_PanelCursor RenderTransform
  Translation (+0x90/+0x94 — the field we always read; it was genuinely still);
  socket pos = PointerCenterOffset(0x3F0) + hidden panel+0x550 + frame adjusts
  (0x428/0x42C, leader 0x430/0x434), range 0x420 (leader 0x424), count frame+0x628.
- `frame+0x7B8` = held-emblem ptr (in hand), `frame+0x7C0` = source socket; leader
  socket (index 1) refuses pick/place; `host+0x4F3` = pick(6)/place(7) sub-state.

**Implemented** in screen_community.lua + native_offsets.commuBoard: mode-7 gate,
hovered = game cache else exact hit-test replica (names empty sockets too), per-move
announcements + "free" + "emblem in hand" + numbered placed-emblems summary.
DEBUG=true for one pass (state snapshot + hunt-v2 memory diff as safety net).

**GHIDRA LESSONS (reusable):** (1) When a UFunction name is shared by 40+ classes,
anchor the class's native-registration table via a name UNIQUE to that class
(PlayInCommRank) and walk the contiguous {char* name, exec} pairs — find_commu.java.
(2) There are TWO name tables: the UECodeGen singleton/constructor table (noise) and
the real native one; the real exec thunks advance `Stack.Code` then call the impl.
(3) cmd.exe splits -postScript args on COMMAS — decompile_addrs.java now rejoins args.
(4) Decompiling the UI tick + its callees beats memory-diffing when the state is
input-gated (frozen memory tells you nothing about WHERE, only that it didn't move).

NOT committed/pushed yet — user must confirm in-game first (his explicit request).

**2026-07-06 — Soul Emblems GRID + board↔grid handoff + Community submenu:**
- **Grid opened FROM the board was shadowed** (board frame stays rendered beneath):
  fixed — when board mode (host+0x500) == 10 the tick goes to the GRID reader.
  Emblems VERIFIED reading by the user.
- **Grid cursor FULLY MAPPED** (three gridhunt passes; beware the first wrong read):
  `EmbList+0x3D0` = column 0..6, `+0x3D4` = row 0..2, **`+0x3EC` = row*7+col = the
  COMPLETE 0-based EmbAry slot — this is the one to use**
  (`native_offsets.commuGrid.cursorIndex`, wired as primary; AnimLoop heuristic only
  as fallback — it trails the input, and mixing it with a partial native read made
  adjacent slots alternate). First pass misread +0x3D4 as the full index → only
  slots 1..3 ever spoke.
- **Main-menu Community submenu — cursor found but STILL NOT READING (open bug,
  user parked it 2026-07-06)**: fixed rows follow START_TOP_LIST_ID enum (BOARD=7,
  EMBLEM=8) via FIXED_SUB_BY_SID in screen_field, cursor = **`startTop+0x4E8`**
  (`fixedSubIndex` — subhunt toggled 0↔1 with the rows; System's subIndex +0x4EC
  stays 0 here). WIRED but the user reports the submenu still silent in play.
  Next lead: the dev_probe log showed the depth flag FLICKERING 0↔1 while inside
  this submenu (dbg alternated FIXEDSUB ↔ ring-level "Comunidad") — the announcer
  may be reset by that screen-name flip every tick, or depth==0 only holds on some
  frames; consider treating ring_sid==0 + ANY depth as the submenu while +0x4E8
  moves, or edge-gating on 0x4E8 changes directly. Hunters OFF; hunt_submenu/
  gridhunt code kept for the next unmapped cursor.

**2026-07-15 — Soul Emblems GRID silence, root cause found (third strike):**
`AT_UICommunityStart` must stay **UNMAPPED** in `ui_directory` — the manager exposes TWO
flows: `cm.UIEmbListIns` (set only in the BOARD flow) and `cm.MenuSoulEmListIns` (the
MENU flow, "EMBLEMAS DE ALMA") — but the latter is a `USoulEmblemMenu` CONTROLLER whose
only reflected member is `GameHUD`, and `UMenuObjectBase` (the `UCommunityMenu`'s
`m_xStart_Community`) reflects NOTHING (AT.hpp:41863). No trustworthy owner field exists
for the menu flow, so a mapping asserts "absent" (null UIEmbListIns) and silences the
menu-opened grid. It's back on scan+pad-boost. Useful reflected fact for the adapter:
each grid slot `UAT_UIXCmnEmb_Cursor` has `UnlockState` u8 @0x408 and name text
`Txt_Commu` @0x3B0 (AT.hpp:37780). Pending in-game verify.

**2026-07-15 night — Soul Emblems grid FULLY SOLVED + VERIFIED** (entry and movement, user
confirmed): the saga's four fixes in `screen_community.lua` — BP-name scan (`Start_Commu_Emb_C`),
ghost-BOARD gate (`BOARD_LIVE_MODES` {7,9,12,13,14,16,17}; mode 5/other = parked → fall through
to grid), pool ENUMERATION picking the live instance (never first_on_screen), and one forced
`Core.refresh_all("Start_Commu_Emb_C")` per visit when a parked board shows with no live grid
(fixes "reads only after mod reload"; first visit per session still ~5 s — known minor).
The native `commuGrid` cursor offsets (EmbList+0x3EC/0x3D0/0x3D4) ARE driven in the MENU flow
too (gridcurs dump) — same offsets both flows. CAVEAT: `Core.pane_live` did NOT discriminate
the ghost cooking pane (vis=0, opacity=1.0 while parked) — visibility/opacity is not sufficient
on this game; the mode-machine gate + spoken-key suppression are what actually killed the
shadowing. Remaining feature: d-pad socket-snap on the free-cursor board (needs mem_bridge
write API; socket/cursor data already mapped).

**2026-07-16 afternoon — STORY-TUTORIAL grid (pick Gohan) coded, pending verify.** User report:
every R1/L1 in the tutorial's emblems grid re-announced the stale "Recibiste al líder… Goku"
reward notice and the grid never read. Root causes + fixes: (1) the parked `Xcmn_Win01_C_0`
keeps ALL visibility signals true (vis/enum 0/op 1/inVP true) 20+ min after closing and BLINKS
off/on-screen per page flip — each blink cleared screen_dialog's `spoken` latch → re-announce
loop; latch now survives blinks (SPOKEN_GRACE_S=3.0 continuous-off). (2) The tutorial
instruction box is the SAME Win01 reused: instruction lives in the RICH `ExMainTxt` side of
`WL_TxtDetail/WL_TxtHelp/WL_WorkText` (AT.hpp:33299) while plain `mainTxt` keeps stale text
("Sí" in Txt_Work, dump-proven) — new `guide_watch.lua` (keyhelp_watch pattern) reads rich-only,
queued, text-keyed latch surviving screen changes. (3) Board→grid handoff (mode 10) had NO
watch arm (ring/ghost arms don't exist in the story flow) → session's first grid waited out
the never-seen backoff; mode-10-without-slots now arms watch_grid + renews (WAIT_RENEW_S cap).
Also discover.lua rich sweep now prints FULL owner paths (short() hid the box's owner).
Round 2 (12:00:45 F7 with full-path rich sweep) PINNED it: instruction =
**`Xcmn_Win01_C.Txt_Detail.ExMainTxt`**, window genuinely LIVE (inVP=true) as the pinned guide
box while its plain mainTxt keeps the stale reward body. New rule shipped: **rich-non-empty +
no choices ⇒ guide layer ⇒ screen_dialog YIELDS** (plain never speaks; guide_watch reads it;
notices keep markup in PLAIN mainTxt so real dialogs are unaffected). guide_watch runs every
registry tick (works over free roam too); F1 repeats the pinned instruction
(Registry.repeat_current → GuideWatch.reannounce). i18n board wording = the game's word
"panel"/"panel vacío" (was "hueco" — mismatched the instruction the user hears).
NOT COMMITTED — user gate 2026-07-16: no commit until they confirm the tutorial reads right.

**2026-07-16 — first-visit/re-entry latency SOLVED + VERIFIED (commit d70f7df), 3 instrumented
runs.** Facts worth never re-deriving: (1) the game creates the `USoulEmblemMenu` CONTROLLER
during the SAVE LOAD, not at menu entry — controller existence is NOT an entry signal (arming
scans off it stuttered post-load free roam AND combat); (2) the first-of-session menu flow can
materialize the grid as the NATIVE-named `AT_UICommunityStart` instance (later visits use
`Start_Commu_Emb_C`) — always handle BOTH names; (3) the grid widget genuinely does NOT EXIST
until ~1s after confirm (5-13s on the very first visit, tutorial popup in between) — no scan
design can beat that, so once detection is instant the residual wait is the game's own load;
(4) the ring resets its depth flag during the CLOSE animation, so "row X was selected when the
ring closed" must use a ~2s recency grace, not last-tick state. The working entry signal:
`screen_field` arms `Core.watch_for` on ring-close-with-emblems-row-recent (sid 8); renewals
anchored to real arms only, cancelled by minimap OR battle HUD (see
[dbz-kakarot-perf-architecture](dbz-kakarot-perf-architecture.md) for the watch-lane primitive).
