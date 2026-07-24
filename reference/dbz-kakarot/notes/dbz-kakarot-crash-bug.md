# dbz-kakarot-crash-bug

> DBZ Kakarot mod crash ledger. LATEST (2026-07-24 c): AV mid-COMBAT reading 0x10 after a huge lag spike (= engine GC/streaming). KEY BREAKTHROUGH: crash CLASS A is guardable after all — obj:GetAddress() returns the stored pointer WITHOUT dereferencing, so new Core.nonnull() gates every Brush.ResourceObject read (keyhelp first, as this ledger predicted); Core.valid also rejects NULL-handle wrappers; battle_monitor (the only 250ms combat loop) finally migrated to Core.member. Earlier: end-user AV browsing COOKING recipes = SAME 07-21 dangling-UObject __index class but in steady-state menu browsing (pooled ListView/detail widget recycled on scroll), NOT a transition — menu adapters ARE exposed; migrated screen_cooking + shared A.list_selected_row to Core.member. Earlier: two end-user AVs from the ExecuteInGameThread flush during teardown; naked member fetch as a call argument; fixed C-array through array_of pierces pcall; nav_tracker raw #arr on streaming-freed objects.


**2026-07-24 (c) — AV mid-COMBAT ("huge lag spike, then crash"), reading 0x10 again. CLASS A IS
GUARDABLE AFTER ALL — `Core.nonnull`.** This is the recurrence this ledger predicted ("WATCH keyhelp
first if a 0x10 AV recurs on an arbitrary screen"). The CLASS A verdict below ("no safe guard exists")
was WRONG in one detail: it assumed the only way to ask about a handle is `IsValid()`, which IS the
piercing deref. **`obj:GetAddress()` returns the STORED pointer WITHOUT dereferencing it** — safe to
call, returns 0 for a null handle. New `Core.nonnull(o)` (ui_core, fails closed) now gates every
`Brush.ResourceObject` read: keyhelp.texture_token (widest radius: every bar, every screen, re-read on
every adapter flip — including the subtitle flips that fire constantly in a battle with dialogue),
screen_results x2, screen_battleresult, screen_community face_resource/face_char (the latter also fed a
null ro into array_of → IsValid), screen_fishing, discover.brush_of. Also applied the same idea to
`Core.valid` (reject GetAddress()==0, fail-open on error) so a NULL-wrapping widget handle can no longer
reach a member fetch. SECOND fix same pass: `battle_monitor.lua` was MISSED by both earlier naked-fetch
sweeps and is the only loop polling (250ms) THROUGH combat on pooled battle HUDs — migrated to
Core.member, plus nav_tracker icon_in_use, screen_battleresult/results/map(chained)/telop/questcard/
dialog/choice/quest_objective. Still naked (menu-only, lower exposure): screen_community's `read(node)`
helper ~20 sites, fishing, shop trio, training, tips, tutorial(s), saveload, palette.
UE4SS.log of the crashed session: ALREADY READ (do not ask again) — no Error:, no traceback, no "nullptr
instance" in the whole session; log stops 50s after boot at the last transition. That rules OUT the
caught-error-storm class and the 0xe06d7363 throw (both leave log evidence) and confirms a hard AV, but
cannot distinguish keyhelp-brush from battle_monitor. WAITING ON THE USER'S RETEST (left before playing):
full restart → combat WITH dialogue (that's what makes keyhelp re-read glyphs) → if it crashes, compare
the new stack's RELATIVE UE4SS offsets with today's; a match = guards insufficient, next layer is the
world-epoch invalidation. Then F7 on the defeat menu for the new GAME OVER probe.

**2026-07-24 (later) — END-USER AV in the SKILL PALETTE, reading address 0x10 (FIXED, source-only).**
Different signature from the cooking one: NO `VCRUNTIME140` at the top, faulting address `0x00000010` =
`UObject.ClassPrivate` (offset 0x10) on a NULL UObject — i.e. a reflection member/method call
(`obj.X` / `obj:Y()`) where obj wraps NULL (a real null, not recycled garbage). Same game frame
`AT-Win64-Shipping+0x152134` (ProcessEvent, our tick on the game thread); died through TWO pcall levels =
uncatchable. Context: ~5 min in the skill tree, ~30 s in the skill palette, then crash. ROOT CAUSE:
`ui_archetypes.A.platbtn_token`'s "last resort" read `img.Brush.ResourceObject` then `ro:IsValid()` /
`ro:GetFullName()` — the EXACT 2026-07-16 dead-end (FSlateBrush.ResourceObject unreadable; IsValid lies on
the wrapper; a NULL ResourceObject → GetFullName derefs null+0x10 THROUGH the pcall). That dead-end was
only removed from keyhelp's IconList back then; this TWIN in platbtn_token survived. platbtn_token is
called by the palette (plate_button) AND keyhelp (device-indexed glyphs), and the brush path only fires
when the earlier reflected resolvers all miss AND a visible Image_List glyph has a null ResourceObject —
so it's intermittent (the palette's face-button glyphs hit it). FIX: deleted the brush-read block; indexed
face buttons already resolve via KeyIdsForPad / CurrentKeyIds (reflected id arrays). RULE REAFFIRMED: NEVER
read FSlateBrush.ResourceObject (or call any method on it) from UE4SS — no pcall or Core.valid saves it.
FULL AUDIT (sonnet sweep of every .lua, 2026-07-24) split the pattern in two:
- CLASS A — `X.Brush.ResourceObject` then `ro:IsValid()`/`ro:GetFullName()`: NO safe guard exists.
  `Mem.addr` can't help — it calls `obj:IsValid()` itself (mem.lua:46), the very piercing call. `Core.valid`
  same. The read is only safe when ResourceObject is a REAL asset (non-null); it crashes on a NULL one. So:
  removable ONLY where redundant + reading null-RO glyphs (platbtn_token — DONE). LEFT (real assets, weeks
  stable, no safe fix without losing the glyph/rank name): `keyhelp.lua:88` texture_token (HIGHEST radius —
  every keyhelp bar, every screen; reads named glyph textures), `screen_results`/`screen_battleresult`
  texture_token (rank/digit real textures), `screen_community` face_resource/face_char ResourceObject read
  (emblem-face materials), `discover.lua:151` brush_of + `screen_fishing.lua:146` (both DEBUG/F2-gated).
  WATCH keyhelp first if a 0x10 AV recurs on an arbitrary screen; the only fix would be to return nil for
  device-indexed glyphs before the brush read (losing their texture-name path, which already falls through
  to platbtn_name).
- CLASS B — naked child CHAIN `obj.A.B` where A is a null UObject: FIXED with Core.member (safe, no
  functionality loss). Applied to `screen_community` face_resource/face_char (`emb.ImageFace` guard) and
  `screen_choice` row_hover (`c.HoverImage`/`Dmy_Choice_Hover.ColorAndOpacity.A`). Still-open class-B behind
  DEBUG flags (not live): `screen_map.lua:123` (`ic.Slot`, CURS_DEBUG=false), `discover.lua:773` (`w.Slot`,
  F2) — guard if ever enabled.
NOTE: the pak_index (D:\code\tools\repak\pak_index.txt) does NOT help this crash class — these are runtime
null-pointer derefs in UE4SS reflection, not asset-identification problems; the right tools are the crash
offset + this ledger + the code + the CXX header (class layouts). pak_index only helps asset-name lookups
(Charicon → character name, class discovery).

**2026-07-24 — END-USER AV while browsing COOKING recipes ("crashed after looking a while"), FIXED
(source only, UNVERIFIED in game).** Stack-only report, no UE4SS.log. Relative UE4SS offsets are
BYTE-IDENTICAL to the 2026-07-21 signature (base = top three frames minus 0xe89e → +0x1482b/+0x1449b/
+0xe89e; game frame `AT-Win64-Shipping+0x152134` = ProcessEvent) → SAME crash class: `obj.Property`
(`__index`) on a DANGLING UObject, on the game thread. The NEW datum: it reproduces in STEADY-STATE
menu browsing, NOT during a world transition — so the dangling object is a POOLED ListView entry / detail
node the game RECYCLES as you scroll the recipe list, not a torn-down world. This DISPROVES the 07-21
note's scoping premise ("menu adapters only run with their screen committed, so the exposure is only the
always-on readers"): a committed screen whose CONTENT churns is exposed too. `screen_cooking.lua` had
NEVER been migrated (last touched 014599e, 07-16) and still held naked member fetches AS CALL ARGUMENTS
(the 07-17 fishing class): `selection_sig` (every tick) `host.Txt_Detail00_01`/`host[DESC]`;
`ingredient_parts` `bar.Txt_List`/`bar.Txt_Num`; `detail` `host[m]`/`host[DESC]`; `overlay_text`
`comp.WL_Text`; `is_active` `shoplist.WL_Shop_Cmn_List`. PLUS the SHARED archetype `A.list_selected_row`
(ui_archetypes.lua) returned `row.TxtName`/`row.Txt_List`/`row.TxtNum` naked — a pooled recycled row that
passes `Core.valid` (IsValid lies on recycled memory) then AVs on the property fetch; this covers EVERY
list menu, not just cooking. FIX: routed all those sites through the existing guarded `Core.member(o,name)`
(the 07-21 helper); both files luac-validated. RULE: when a NEW reader is added for a pooled/virtualized
list or a detail pane that repopulates on cursor move, every `x.Member` used as a helper argument goes
through `Core.member` from day one — the "menu adapters are safe" exemption is retired.
FOLLOW-UP SAME DAY (user asked "any other screens like this?"): a sonnet sweep of every adapter found the
SAME naked-fetch-as-call-argument pattern in 18 MORE files and migrated them all to `Core.member`
(luac-validated): screen_dialogue (w[body/name_prop], the hottest — pooled subtitle instances),
ui_archetypes row_keyconfig/row_value/row_name/scan_list (Xlist_Bar03 rows recycled across option tabs),
screen_list (Items + Dragon Balls factory, host[list_member]/list[tab_member]), screen_map
(Map_M/World pooled), screen_saveload (virtualized 3-bar window, 11 sites), screen_shopcmn, screen_choicelist,
screen_tutorials, screen_skilltree, screen_characters/itemuse/party, screen_palette, screen_pause,
screen_options, screen_shopinfo, screen_title, screen_battleresult (host[m]). Sites already inside a
`pcall(function() … end)` were left (verified per-block, not by line grep). The always-on readers were
already migrated on 07-21; menu adapters are now covered too — the naked-arg class should be effectively
gone across the mod. (Same session also fixed NON-crash UI bugs unrelated to this ledger — see STATUS.md 2026-07-24 for the
full round 2. Key lessons after the user's retest: (a) the quest objective spoke "several times" and
re-narrated on map close because the HUD repopulates progressively and pools several Quest_Navi_C — the
diff gate alone wasn't enough; needed a SETTLE debounce (2 stable polls). (b) The map-open re-read must be
DEFERRED + queued (interrupt=false) or it talks over the map's own area/help readout. (c) The persisting
"desbloqueaste superataque" was NOT screen_toasts — it was screen_dialog's post-confirm window re-firing
once the 24-entry recent-set FIFO evicted its marks (the file's own node_rt comment already named this
hole); fix = a `pinned_set` immune to FIFO eviction, cleared only on map transition. screen_toasts still
got a ~1.5 s grace on its presence dedup (Info_Log02 is a FLASHING banner whose blink re-armed it). NEW
FEATURE: the DEFEAT menu (Gameover_C → UAT_UIGameover) was silent — added screen_gameover.lua; the BP
class dumps empty so selection is the native CurrentSelectIndex, NAMED in AT.hpp:33464 @0x3E0 (read
reflected first, mem_bridge fallback) — the §2 native-index case, but this time the header gave the offset
for free, no Ghidra needed.)

**2026-07-21 — TWO AVs reported by an END USER of the released build (no UE4SS.log available).
Stack-only diagnosis; fixes applied, UNVERIFIED in game.** Both stacks are byte-identical across the
whole UE4SS portion (compare RELATIVE offsets, ASLR moves the bases): `VCRUNTIME140` → `UE4SS+0x1482b
/+0x1449b/+0xe89e` (C++ property-access path, AV in a memcmp on a garbage class) → ~20 Lua-VM frames
`+0x71xxxx–0x73xxxx` (TWO nested pcall levels — and it still died, so it is a true AV, not the
0xe06d7363 throw that leaves a traceback) → `+0x452891/+0x45a5cd/+0x478c5a/+0x6a2051` (C++→Lua
dispatcher) → **`AT-Win64-Shipping+0x152134`, THE SAME in both, with DIFFERENT callers below it** =
`ProcessEvent`/`ProcessInternal`. READING RULE learned here: identical relative UE4SS offsets = same
crash; a constant game frame with varying callers = the `ExecuteInGameThread` queue flush, i.e. our
tick on the game thread. Verdict: `obj.Property` (`__index`) on a DANGLING UObject; faulting
addresses (0x6859d478/0x464cded8) are recycled garbage, not null.
**KEY STRUCTURAL FINDING (opus adversarial review):** `transition.lua`'s premise that "no Lua tick can
run between teardown and the gate arming" is **FALSE** — during `LoadMap` the engine fires dozens of
UFunctions through ProcessEvent and EVERY one drains our queue. That is the hole. ALSO: the
previously-noted durable fix "harden `Dir.root_ok('mm')`" is **CIRCULAR** — root_ok does
`Core.valid(roots.mm)`, an `IsValid()` on the dead level's MenuManager, the very crash it would
prevent. The real fix is a WORLD EPOCH off a persistent root (gi → LocalPlayer → PlayerController,
which the engine nulls BEFORE CollectGarbage), stamped on every cache; NOT DONE YET (user chose the
scoped hardening first).
FIXES APPLIED: (1) new `Core.member(o, name)` guarded fetch in ui_core + migrated the always-on naked
sites (`Core.text_of`'s own `node.mainTxt` — hottest in the mod, every reader goes through it —
`quest_objective` row_line, `keyhelp.helpmsg`, `screen_dialog.choices`); (2) `screen_toasts.lua:58`
`node_text(bar.Txt00)` — the 2026-07-17 bug fixed then ONLY in the Info_Log02_C twin, left live in the
Info_Log_C loop; (3) `screen_questcard.lua` shipped with `TRACE = true`, running two per-level `fm`
pointer chains + widget probes EVERY tick in EVERY state including teardown, for logging only.
LESSON: a diagnostic flag left ON is not just noise, it is crash surface — `screen_cooking.LATCH_DEBUG`
and `screen_results.DEBUG` also shipped ON (left alone deliberately, both are per-event not per-tick).
An audit of the whole codebase for naked fetches found ~60 sites but MOST ARE ALREADY INSIDE a
`pcall(function() … end)` block (a line-based grep flags them falsely — always check the enclosing
block before "fixing"); menu adapters only run with their screen committed, so the real exposure is
the always-on readers listed above.

**2026-07-17 evening — SECOND same-signature AV, on RETURN-TO-TITLE (mitigated; mechanism is opus's
ranked verdict, unproven until a recurrence).** After aa1606b, `Mgame_Result_C` (a PER-LEVEL pooled
widget) was probed via `first_on_screen` every tick in every state (fishresult registered first in
the sweep + screen_fishing's unconditional yield probe). `Core.valid` = raw `o:IsValid()` — on a
GC-freed widget that's the uncatchable 2026-07-04 teardown class, and the GameMode-notify transition
gate has TWO holes: the gap before the notify fires, and the post-GRACE re-FindAllOf that can
re-cache doomed old-world widgets before deferred GC frees them (next tick's IsValid then AVs).
RULES: (1) never probe a per-level pool unconditionally from an always-running adapter — gate the
probe on the owning feature being live/recently live (`_G.__KakarotMinigameLive` stamp pattern);
(2) persistent GameInstance-outer'd pools (Xcmn_*, subtitles) are why the OTHER always-on adapters
survive title returns — outer matters when judging this risk; (3) DIAGNOSTIC ANSWERED same evening (user had NOT relaunched — the 17:15 boot log WAS the crashed
session): the log's last line is "transition gate ON" at 17:16:06 → the AV came AFTER the gate
fired, i.e. the post-flush window (grace / post-grace re-find while deferred GC frees the old
world), NOT the pre-notify gap. On the NEXT unexplained AV still save UE4SS.log before relaunching
and get the exe+offset. Durable fix if it recurs: keep the transition
flush engaged while `Dir.root_ok("mm")` stays false (ui_registry's world predicate).

**2026-07-17 — FATAL AV while fishing (commit f18ff03, fixed same day): a member fetch evaluated as a
FUNCTION ARGUMENT is outside every pcall.** `screen_toasts` wrote `node_text(bar.TextBox) or
node_text(bar.Txt00)` — the helper pcalls its INSIDE, but `bar.Txt00` is evaluated at the call site,
naked; and `Info_Log_Bar02_C` has no `Txt00` (that member belongs to the SIBLING item-log bar class —
the assumption was carried over). Nonexistent-member fetch = the IsHidden uncatchable class; a blank
pooled Info_Log02 bar stayed on_screen after landing a fish, so the abort repeated EVERY tick → the
caught-error-retried-per-tick storm (raycast lesson) → EXCEPTION_ACCESS_VIOLATION deep in the Lua VM,
NO Lua traceback in UE4SS.log (the log just stops). RULES: (1) member fetches go INSIDE a pcall, never
as call arguments — `local x; pcall(function() x = obj.Member end)`; (2) never assume a member exists
from a LOOK-ALIKE sibling class's layout — check the class's own header/ObjectDump entry; (3) a fatal
AV with deep recursive UE4SS.dll frames and a log that ends silently = per-tick retried uncatchable in
an adapter sweep — diff the newest per-tick reader first. Diagnosis: opus adversarial review of the
commit against this ledger pinned it in one pass (header proved Txt00 absent).

**2026-07-16 — THIRD GetArrayNum failure mode: a FIXED C-array member pierces the array_of guard AND
every pcall, WITHOUT killing the process.** `screen_dialog.plates` passed `WL_LvTextList`
(`UCFUIXcmnMultiLineText*[7]`, a native fixed array — AT.hpp shows it as a pointer with size 0x38)
to `Core.array_of`: the collapsed RemoteObject PASSES `IsValid`, and `GetArrayNum` raises "Tried
calling a member function but the UObject instance is nullptr" as a C++ exception that unwinds
THROUGH every enclosing pcall; UE4SS catches it at its callback boundary (with a full Lua traceback
in UE4SS.log — read it, it names the exact line), so the game survives but the Lua tick dies
mid-function. CONSEQUENCE CLASS: half-updated module state — screen_dialog latched `notice_msg`
without rebuilding `notice_full` and spoke the PREVIOUS notice's stale text (the community-tutorial
"emblem reward mixes with Ki/PS" bug, rounds 1-13 chased symptoms; the traceback closed it in one
look). RULES: (1) never pass a fixed C-array member to array_of — in the CXX dump a pointer-typed
member with size > 0x8 is a fixed array (the WL_StartCharBarList collapse lesson, GetArrayNum
edition); (2) any multi-step state update in an adapter must assign its latch LAST (transactional),
because a pierced pcall can abort between assignments; (3) when a symptom is "spoke stale/previous
text once, then latched wrong", grep UE4SS.log for `Error:` + traceback before theorizing.

**2026-07-16 night addendum — `FSlateBrush.ResourceObject` is UNREADABLE from UE4SS, period (DEAD
END, tried twice — do not retry).** (1) `ResourceObject:GetName()` unguarded → pcall-piercing
nullptr error from inside the registry sweep: tips silenced MOD-WIDE, re-thrown EVERY tick because
build_bindings aborted mid-way. (2) Retried behind a PASSING `Core.valid(ro)` gate → STILL pierced
(the discover brush_of class: IsValid lies for these wrappers). (3) The reflected
`FSlateBrush.ResourceName` FName (SlateCore.hpp @0x50) reads safely but is None on every
CFTextIconData brush. Resolution: the IconList read was REMOVED; the numbered `Btn_Key_N` icons
render the NEUTRAL whole-d-pad glyph anyway (user screenshots), so the generic "cruceta" word is
faithful. Generalization: inside hot paths (registry sweep, markup_to_speech), NO UObject member
calls on struct-embedded asset refs — and a structural rule: never let a lazy cache BUILDER throw
after partial assignment (assign the cache first, enrich after, or one abort = retry-throw per tick).

**2026-07-14 EVENING — "crashes are fixed" was NOT credible: the array_of migration was only half-done.**
An adversarial sweep (all 58 .lua files) found raw reflected-TArray reads (`#arr` / `GetArrayNum` behind
pcall-only) in 10 MORE files the first migration missed — worst two were hot on every session:
`ui_archetypes.lua platbtn_token/ids_token` (the keyhelp bar calls it on EVERY screen — brand-new code) and
`screen_dialog.lua plates/choices` (every NPC line/confirm). Also screen_community, screen_battleresult,
screen_results, screen_cooking, screen_shoplist, screen_fishing (one LIVE read in the phase-1 fishing
prompt), nav_tracker's Ctrl+F5 dump lines, discover.lua. All migrated + luac-validated 2026-07-14.
LESSON: after declaring a migration done, run an independent sweep — "the files I remember touching" ≠
"the files with the pattern". Also fixed same session: raycast.lua UFunction arity ("expected 13, received
11") — UE4SS wants out-params passed as a Lua table placeholder (OutHit), it does NOT accept skipping them;
that error was confined to the Ctrl+F5 dump's probe stage, NOT gameplay. See
[dbz-kakarot-perf-architecture](dbz-kakarot-perf-architecture.md) for the same-evening performance restructure.

**2026-07-14 — mid-GAMEPLAY crash, SOLVED (this is the "if it STILL crashes… streamed sublevel teardown"
prediction at the bottom of this file coming true).** Symptom: uncatchable `0xe06d7363` throw, NO Lua error
logged, during free-roam after playing a while (the crash log's last line was a transition, a red herring —
transitions are just the only thing logged). Root cause: **`Transition.active()` fires only on a full map
load (new GameMode), NOT on streamed sublevels** (`transition.lua` comment) — so level streaming frees
world actors/TArrays during free-roam with no gate, and only per-object revalidation protects us.
`nav_tracker` read raw `#arr` / `.PathPoints` on objects that were only nil-checked (not `IsValid`'d):
`compute_route` (`path.PathPoints`, a reflected NavigationSystem result — prime site, ran every ~3 s with
route tracking), `aim_watch` (`comp.m_xActors`, 100 ms), `MapIconList` scans. Fix: ALL reflected TArray
reads now go through **`Core.array_of(owner, name)`** (validates owner+array via UE4SS-shadowed `IsValid`
BEFORE `GetArrayNum` — and `pcall` can NOT catch this throw, so the pre-call validity check is the only
defence). See [dbz-kakarot-event-driven-ui-cache](dbz-kakarot-event-driven-ui-cache.md) for `Core.array_of`. Same session also removed the
widget event feed (foreign-thread `lua_State` corruption) and simplified detection to fixed-cadence polling.

---

**Status 2026-07-04 (v3): crash VERIFIED FIXED in-game (return to title + Nimbus
cutscene, no crash). v3 pending verify:** R3 radar menu opens again + no cutscene lag.

**2nd instance (2026-07-14): F7 discover dump — two pcall-CAUGHT `brush_of` errors
("UObject instance is nullptr" from `GetName` DESPITE passing `IsValid`, i.e. the wrapper
died between calls), then fatal 0xe06d7363 (raw C++ throw, no Lua conversion). Fix: fuse in
discover.lua brush_of (stop probing brushes after 3 failures per dump run). RULE: any
repeated caught "nullptr instance" reflection error = dying UI state → STOP probing that
family immediately; the next throw may escape pcall entirely.**

**NEW CRASH CLASS (2026-07-13, fixed, pending verify): a pcall-CAUGHT reflected-call
error retried every tick can still hard-crash the game.** `raycast.lua`'s LineTrace
calls raise a UFunction param-count Lua error (expected 13, received 11); pcall catches
it, so nav_tracker's "testing→bad" abort fuse never trips (it only catches ticks that
DIE), steer_around retried the broken call every tick, and UE4SS eventually died with a
fatal EXCEPTION_ACCESS_VIOLATION (0x30 null deref, deep Lua VM stack). Fix:
`RAYCAST_AVOIDANCE = false` hard switch in nav_tracker (raycast avoidance is dead on
this game anyway — uncatchable aborts on Area02/04). LESSON: fuses must also count
REPEATED CAUGHT errors, not just mid-flight aborts; and UFunction param-count mismatch
errors are not benign — stop calling after the first one.

**ATTEMPT 1 FAILED — CRITICAL lesson:** `RegisterLoadMapPreHook/PostHook` crash THIS
game at the first map load (UE4SS 3.0.1 + SteamStub exe: registration "succeeds" but
the native trampoline is misplaced; UE4SS.log ends at "Event loop start"). NEVER use
LoadMap hooks on Kakarot.

**ATTEMPT 2 (v2, `NotifyOnNewObject("/Script/Engine.World")`) fixed the crash but was
too noisy:** every streamed sublevel constructs a UWorld → 127 fires in 3 min, gate on
half the time → R3 radar menu wouldn't open, and each flush forced FindAllOf refills →
cutscene lag. **v3 uses `NotifyOnNewObject("/Script/Engine.GameModeBase")`** — exactly
one GameMode per map load (AATTitleGameMode / BP_ATGameModeMain_C), streaming spawns
none, same no-Lua-tick-after-teardown timing guarantee. Crash + noise both VERIFIED
fixed in-game. All documented in reference/UE4ss study/docs/ue4ss-compatibility-and-crashes.md.

**REGRESSION lesson (2026-07-04): never enable a diagnostic that reflects `UniqueId`
(or GetOuter / InteractComponent) on ARBITRARY actors.** Turning on nav_tracker's
`RADAR_DEBUG` (which reads `TargetActor.UniqueId:ToString()` on navi targets, and the
legacy probe's UniqueId/GetOuter) raised the uncatchable C++ abort mid-`do_open`, so
the R3 menu silently stopped opening. UniqueId is only safe on a confirmed
QuestCharacter (see [dbz-kakarot-radar-picker](dbz-kakarot-radar-picker.md) npc_name). Replaced it with a light
`NAVI_DEBUG` dump that only reads UWidgetSwitcher.ActiveWidgetIndex + component type
(all pcall'd, no UniqueId). RADAR_DEBUG left OFF.

**Navigation route (2026-07-04, [dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md)):** the audio radar's
NavMesh route rarely computed because this game builds NavMesh ONLY around navigation
invokers (`bGenerateNavigationOnlyAroundNavigationInvokers`) — no tiles where the player
is. Fix: register the player pawn as an invoker (`UNavigationSystemV1::RegisterNavigation
Invoker`, live nav via `GetNavigationSystem`) + project endpoints (`ProjectPointToNaviga
tion`) before `FindPathToLocationSynchronously`. These native calls are guarded by a
persistent one-shot probe (`_G.__KakarotRouteNative` = testing/ok/bad): if the first
call aborts uncatchably it self-disables the route for the session (straight-line beacon)
after a single beacon blip, never looping. Auto-move (`StartAutoMoveToLocation`) exists
but user rejected auto-navigation.

**IN-GAME RESULT (2026-07-04, F5 nav dump, Area02):** invoker `registered=true`,
`building=false`, but `navmesh probe (10m ahead): no path`. **Area02 (overworld) has NO
NavMesh at all** — registering the player invoker generates nothing because there's no
nav data / NavMeshBoundsVolume to build from. So route guidance falls back to the
straight-line beacon in the overworld; it may still work in interiors/near-NPC tiles.
For true obstacle-avoidance in no-navmesh areas the only path is LineTrace/SphereTrace
(available) forward-ray obstacle detection.

**RAYCAST OBSTACLE AVOIDANCE (built 2026-07-04, PENDING in-game verify + trace-channel
tuning):** `raycast.lua` (KismetSystemLibrary.LineTraceSingleForObjects via the CDO) +
`nav_tracker.steer_around`. When there's no NavMesh route, cast a ray from the player
(chest height, AVOID_HEIGHT=1m) toward the objective for AVOID_LOOKAHEAD=4m; if blocked,
fan out ±25..±130° and steer the beacon to the nearest CLEAR bearing that still heads
toward the objective (no auto-move — player walks it). Says "rodeando obstáculo" when
steering starts. Part of route_mode (Shift+F3). Guarded by the same one-shot native
probe as the NavMesh calls (`_G.__KakarotRayNative`); if the trace API errors it silently
does nothing (non-breaking). **UNKNOWNS to confirm from the Ctrl+F5 dump's new "raycast
objType=N" lines:** (1) does UE4SS accept the LineTraceSingleForObjects calling
convention (OutHit skipped, returned) — if all lines say API-UNAVAILABLE, the convention
is wrong; (2) WHICH EObjectTypeQuery (1=WorldStatic default) detects walls — point the
camera at a wall vs open space and Ctrl+F5 each. Narrow Ray.OBJECT_TYPES once known.
NOTE: `pc.Pawn` in the overworld is `TwinFootBP_C` at (0,0,0) (a stale/vehicle pawn);
the REAL player is the minimap `PlayerIns` = `AT_Character_cpl001_BP_C` — player_pawn()
already prefers PlayerIns, correct.

**Super Attack training menu READ (2026-07-04):** talking to a training NPC (Krillin)
opens `Shop_Training_C` (`UAT_UIShopTraining`) — was unread (no adapter). New
`screen_training.lua` reads the detail panel (updates on cursor move via the game's
DetailUpdate): Txt_Cap00 skill name, Txt_Cap03 ki, Txt_Detail description,
Start_Quest_Bar00_00.Txt_Progress cost, title Xlist_List05_Lay7.Txt_Title. Registered
BEFORE the shop group (opens over Shop_Top which stays on_screen underneath). Also
confirmed main/sub navi fix works (switcher 1→main/2→sub). Pending in-game verify.

**SEPARATE BUG found via the v3 session log (UE4SS traceback): the R3 menu death was
NOT (only) the gate.** d1efdb2's collectibles scan called `actor:IsHidden()` —
**IsHidden is NOT reflected in this game** (0 hits in the CXX dump) and calling a
non-existent member raises the uncatchable C++ error (misleading UE4SS message
"UObject instance is nullptr"; same class as the CharacterName lesson). The dying
callback left the loop's `busy` guard stuck true → that loop (radar_menu, sometimes
nav) was DEAD for the rest of the session ("a veces suena, a veces no"). Fixes:
(1) read the reflected `AActor.bHidden` property (CXX dump @0x90) instead;
(2) ALL three loops (ui_core.loop / nav / radar_menu) now clear `busy` on ENTRY of
the game-thread callback, so an uncatchable death can never silence a loop again.

**Root cause (confirmed by code analysis):** "return to title" and cutscene-driven map
changes go through `UEngine::LoadMap`; the old level's objects are freed by GC, and the
mod held cached references into that level (nav's `mm_cache` minimap widget,
`navi_icons`, `target.actor`, ui_core's `live_cache`/`all_cache` per-level HUD widgets,
screen_community's `panel_cache`). Probing a freed UObject — even just `IsValid()` — is
an uncatchable C++ abort. The old ui_muted/world-gate pair had two holes: (1) the world
gate ITSELF probes `mm_cache` every tick, and (2) cutscenes run with NO adapter active,
so ui_muted never fired.

**The fix — global transition gate ([dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md)):**
- `transition.lua` (new): `RegisterLoadMapPreHook/PostHook` (UE4SS, verified in
  reference/UE4ss study/docs/ue4ss-api-reference.md). PRE → gate ON + run registered
  flush callbacks (each module nils its UObject caches; pure Lua + Audio.stop only).
  POST → gate stays on a 1.5 s grace, then loops re-find everything fresh. 120 s
  failsafe self-heal. State in `_G.__KakarotTransition` (hot-reload safe); hooks
  installed ONCE from main.lua (needs game restart); callbacks reach the current
  module instance via `_G.__KakarotTransitionBegin/End`.
- All loops check `Transition.active()` (pure Lua) FIRST: nav_tracker.step,
  ui_registry.step, radar_menu.step (also unblocks the pad), plus Nav.where /
  cycle_companion / field_ready / dump and App.read_keyhelp.
- Flush registrations: ui_core (both widget caches), nav_tracker (target, mm_cache,
  navi_icons, route state), screen_community (clear_state).

**How to verify:** UE4SS.log should show "LoadMap transition gate registered." at boot
and "LoadMap begin/end" lines on every map change. If it shows "LoadMap hooks FAILED",
this UE4SS build lacks the hook → need a fallback detector.

**If it STILL crashes after this:** the failing transition is NOT a LoadMap (streamed
sublevel teardown). Next step: get the crash frame offset (AT-Win64-Shipping+0x…) from
the UE4SS/crash log, and consider gating on a fade/loading widget or dropping actor
caches when the world gate has been closed >1 tick.
