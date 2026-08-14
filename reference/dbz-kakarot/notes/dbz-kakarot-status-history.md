# STATUS history — DBZ Kakarot accessibility mod (archive up to 2026-08-14)

> **What this is.** The verbatim contents of `STATUS.md` as it stood on 2026-08-14, before it was
> pruned back to the dashboard the playbook asks for (§11: *"Keep it short — a dashboard, not
> documentation"*). It had grown to 3,456 lines / 355 KB, which made it unreadable in one pass and
> therefore useless as a resume-the-session tool.
>
> **Nothing here was deleted, only moved.** Everything below is the original text, unedited. The
> live dashboard is [`STATUS.md`](../../../STATUS.md); this file is the diary behind it — the
> derivations, the rounds, the refuted hypotheses and the in-game reports. Grep it when you need to
> know *why* something is the way it is, or before re-deriving anything.
>
> The two sections worth knowing about: the long narrative log that used to sit at the top (the
> `Last updated` entries, roughly the first 1,270 lines) and the per-batch **test plans** that
> accumulated under `## Next step`. Both are history; the current state lives in `STATUS.md`.

---

# STATUS — Dragon Ball Z Kakarot

> Per-mod status ledger / dashboard. Open this first when resuming the mod so progress isn't re-derived from the code each session. Keep it short — a dashboard, not docs. Update the **Next step** line and the section table whenever you finish a chunk. Derive every value from the game's real data — no guessed offsets.

**Architecture — read before changing how UI state is read:** [`reference/UE4ss study/docs/ue4ss-mod-architecture.md`](<reference/UE4ss study/docs/ue4ss-mod-architecture.md>) — *resolve, don't scan*, synthesised across this mod and the Sparking ZERO one: scan cost measured on both (~65 ms here vs ~115 ms there), the decision ladder, and the `RegisterBeginPlayPostHook` acquisition this mod has **not** tried yet (the ini ships with BeginPlay hooking off). Game-specific counterpart: `reference/dbz-kakarot/notes/dbz-kakarot-perf-architecture.md`.

**Last updated:** 2026-08-03 (g) — **THE POST-BATTLE RADAR: THE SWEEP FOUND THE OBJECTIVE EVERY
TIME AND THREW THE ANSWER AWAY BECAUSE IT WAS THE SAME ONE.** 1 Lua file (`nav_tracker.lua`).
**SOURCE-ONLY, UNVERIFIED IN GAME.** Lint clean over 75 files.

**VERIFIED IN GAME 2026-08-03 (user), from the (f) batch:** the R3 picker now opens with the full
lists on the first press of a session (about a second on a cold cache, which is it waiting for the
sweep instead of guessing), and **the radar no longer goes quiet during gameplay dialogue**. Both
fixes stand. Still reported: the radar is slow to start again after a battle.

**And that last one was a one-line bug, found by reading rather than measuring.**
`Nav.release_world_refs()` nils `target.actor` on every world-gate close — a battle, a cutscene —
deliberately, because the actor may not survive it, while keeping the rest of the record so a
surviving objective never has to be re-announced. The auto-scan then re-finds the objective and:

```lua
if not target or target.key ~= best.key then
    target = best        -- new objective: fresh handle
else
    target_missing = 0   -- SAME objective: updates nothing at all
end
```

So the one field the record was missing was the one field the sweep had just produced. The radar
came back from every battle holding a **nil actor** for a target it believed it had, and could not
recover on its own either: `target_missing` only counts up when the sweep finds NOTHING, and here it
found the objective on every pass. Fixed by re-adopting the fresh handle in that branch (and
dropping the route with it, since it was pathed against the handle that just died). Silent by
design — this is the "merely re-resolving the same spot" case the announce beside it already
declines to speak.

**Same batch, reported before it was tested: "the radar is also slow when the objective changes."**
Third appearance of the same shape. Between the game issuing a new objective
(`Nav.notify_objective_change`, armed by the quest HUD reader) and the radar going to look for its
marker sits `tick % SCAN_EVERY == 0` on a free-running counter — an arbitrary 0–1.5 s of doing
nothing. The arm now also sets `Nav.auto_now`, so the marker walk happens on the next nav tick.
Placed INSIDE the existing `preempt.scans <= 0` branch on purpose, and for that branch's own reason:
the callback can fire as fast as the quest HUD polls (300 ms), so a flapping caller that pinned this
flag would turn a 1.5 s cadence into a marker walk every nav tick — the scan storm the 2026-07-31
"arm from zero only" fix exists to prevent. One event, one immediate scan.
**Running tally of that defect**: the manual resume (d), the world-gate reopen (e), and now the
objective change. A modulo on a counter that keeps ticking while the thing it gates is blocked is
not a cadence, it is a random delay — worth a sweep for any that remain.
**VERIFIED IN GAME (user):** the story objective now tracks noticeably faster.

**"Sometimes the radar untracks by itself, then tracks again and announces the objective at me
again" — and this one was CAUSED by the mute fix in (f), which is why it appeared only now.**
Quiet mode does not defer the navi-icon refresh, it **empties the pool** (`navi_icons = {}`, the
release beside the rescan), so while a passive overlay owns the screen `best_candidate` is blind by
construction and returns nothing however present the marker is. `target_missing` counted every one
of those, three of them (~4.5 s) dropped the target, and the moment quiet lifted it was re-acquired
— which re-announces, because after a drop the announce test sees a new key. It never surfaced
before because ambient chatter used to MUTE the nav loop entirely; now that the loop runs through
chatter (`Dialogue.scan_quiet = true` is still set, only the mute changed), it started counting.
**Fixed** by not counting a scan we declined to make. RULE, and it is the general one: **evidence you
refused to collect is not evidence of absence** — any counter that drops state on "not found" must
first ask whether it actually looked.

**…and the first cut of that fix was itself a REGRESSION, caught the same day** (user: "after the
battle it never tracked again, I had to restart tracking by hand"). Suppressing the counter whenever
the scan was blind removed the only escape hatch for a target whose HANDLE had been nil'd:
`release_world_refs` nils `target.actor` at every world-gate close, and both paths that restore it —
re-adoption when the key matches, a fresh target when it does not — need a candidate the blind scan
cannot produce. So the target could no longer be repaired AND no longer be dropped: permanently
stuck, manual re-pick the only way out. The guard now carries both halves — skip the count only
while `target.actor` is still valid, i.e. only while there is something to protect; with the handle
gone, counting resumes and `LOST_SCANS` hands the objective back to ordinary acquisition, with the
`target.lx` coast covering the window. LESSON: **a "stop dropping state" fix must be checked against
the state that could only ever be repaired BY the drop.** The suppressed path was load-bearing.

**BOTH RADAR SYMPTOMS SURVIVED THE FIXES, SO THE GUESSING STOPS HERE (2026-08-03, diagnostics
only).** Post-battle re-acquisition is still slow and the self-untrack/re-announce still happens
while simply walking. Three rounds of reasoning produced two fixes and one regression without ever
observing WHICH branch fires, which is the exact failure mode this repo has a rule about. So this
batch adds no behaviour, only evidence — three transition lines, none of them per-tick:
- `nav world gate -> CLOSED (<reason>)` naming which of the gate's three conditions closed it —
  `ui muted` / `minimap widget gone` / `minimap off screen`. This edge runs `release_world_refs`,
  which nils `target.actor`, so a spurious close is the leading hypothesis for both symptoms: the
  gate has **no debounce on the falling edge**, and the playbook's own rule is that any signal
  derived from engine state flaps and the costly edge must be debounced.
- `nav world gate -> open after N ticks` — N separates a real battle from a one-tick flicker.
- `nav drop target (world-gone | lost-scans) key=… actor=live|NIL missing=N quiet=…` on the two
  AUTOMATIC drops only; deliberate ones (arrival, B, a fresh pick) stay silent because the player
  asked for those. An unasked-for drop is what re-announces.
**ONE SESSION LATER THE LOG SETTLED IT, AND IT FALSIFIED THE HYPOTHESIS.** The line:

```
nav drop target (lost-scans) key=1988316711648 actor=live missing=3 quiet=false
```

World gate OPEN, scanning ALLOWED, tracked actor **live** — and the objective dropped anyway. The
gate lines in the same log show only legitimate closes (`open after 519 ticks` = a 52 s cutscene,
`open after 10 ticks` = a load); there was no one-tick flicker at all, so the falling-edge debounce
this batch was written to justify would have fixed nothing. Worth stating plainly: three rounds of
reasoning had produced two fixes and a regression, and one log line ended it.
(Benign, in case it is seen again: `open after 0 ticks` with no preceding CLOSED is the map-transition
hook setting `gated_prev` directly, bypassing the branch that prints.)

**Root cause.** `best_candidate` returns nil only when BOTH its source loops yield nothing, and
every way that happens is a per-tick transient — the navi icon fails `icon_in_use` because one of
its sub-widgets is momentarily off screen, its `TargetActor` pointer is unreadable for a tick, or
`actor_pos` raises during a transform update. The minimap-icon fallback loop only runs when no navi
icon exists, so it is no safety net. Three blips inside 4.5 s is not rare. Also ruled out by the
same trace, and worth recording so nobody re-derives them: there is **no distance cap on quests**
(`if grp ~= "quests"`, "Quests: no limit at all" — so walking cannot push the marker across one),
and `preempt.focus` cannot empty the set (it steers which candidate wins, never whether one exists).

**VERIFIED IN GAME (user): the random self-untracking is GONE.** What remains is only the
post-battle re-acquisition, still 2–4 s or more.

**Post-battle, next hypothesis — and this one is instrumented so it can be refuted.** The rising
edge armed ONE immediate scan (`Nav.auto_now`). That is not enough: right after a fight the quest
marker's own navi icon is usually not back yet, so that scan finds nothing and every retry then
waits a full `SCAN_EVERY` — two or three of those IS the reported delay. It is now a bounded WINDOW
(`Nav.auto_until`): retry every nav tick until something is acquired, for at most
`LOST_SCANS * SCAN_EVERY` (4.5 s) — derived, not picked, because that is this file's own statement
of how long a target may be unfindable before it is given up on. The window closes the instant a
target is acquired, and closing it prints **`nav re-acquired after X.XX s key=…`**.
**MEASURED, same day: `nav re-acquired after 0.08 s`** after a 9-tick gate closure — the window
does what it was written to do, and the user confirms the delay is shorter.
**But the log also shows the case that is NOT solved**: a 208-tick closure (a 20 s cutscene, target
dropped by `world-gone` meanwhile) produced no `re-acquired` line at all — the 4.5 s window ran out.
Two possibilities and they are not both ours: the game had not rebuilt its HUD yet, or the icons were
back and nothing resolved. The expiry now logs the discriminator —
`nav re-acquire window expired after X.XX s (icons=N, target=…)` — where an EMPTY icon pool means no
amount of looking would have helped, and a non-empty one means the miss is ours. Next report settles
it; do not extend the window bound before that line has been read.

**Fixed by changing the question.** `target_missing` was counting "the sweep found nothing", which
is weak evidence about our objective while we are holding a live handle to the thing itself. It now
asks the handle: valid and positionable → not missing; nil, invalid or unpositionable → count, and
`LOST_SCANS` still drops it. The genuine case is untouched — an objective that completes has its
marker destroyed, so the actor fails exactly as before. The `Core.scan_quiet()` gate from the
previous cut is gone as redundant: whether we bothered to LOOK for candidates says nothing either
way about the actor we already have. GENERAL RULE: **when you hold the thing, ask the thing — not
the search that would have found it.**

**Still open: the post-battle start is "a little" slow.** Improved by the handle re-adoption but not
gone. Best remaining hypothesis, unconfirmed and NOT coded: post-battle chatter sets `scan_quiet`,
and with the target dropped during a long fight (`world_gone >= WORLD_DROP_TICKS`) the blind
marker scan cannot re-acquire until the chatter ends. Allowing ONE icon refresh under quiet when
there is no target at all would fix it, at the cost of a 65 ms scan inside a scene — measure before
coding it.

**Last updated:** 2026-08-03 (f) — **THE RADAR MUTE WAS ASKING THE WRONG QUESTION, AND THE PICKER
WAS WAITING ON A MEASUREMENT THAT CANNOT EXIST YET.** 3 Lua files (`screen_dialogue.lua`,
`radar_menu.lua`, `nav_tracker.lua`). **SOURCE-ONLY, UNVERIFIED IN GAME.** Lint clean over 75 files.
User feedback on (e): radar much better after a map load, still slow after a battle; **the first R3
picker of a session always says "nothing to track"** while the main quest is actively being tracked,
and the second shows everything; **the radar still mutes for a couple of seconds after gameplay
dialogue** ("that cannot happen").

**The mute: the grace was working, the question was wrong.** The log settles the magnitude — five
arms in three minutes of walking, each 0–1 s, every one released as `free roam: ambient chatter`
(the one 30 s mute in the same log was a real scene, minimap down, correct). The (d) design armed on
the SURFACE and only then decided, over `AMBIENT_GRACE_S`, whether it had been chatter; the gap is
that decision window, and it is audible every time. **Now it arms only while `Core.free_roam` is
FALSE.** The argument: that is the same predicate `nav_tracker`'s own world gate runs on, so while
the minimap is up the radar is going to dereference those actors regardless of this flag — muting
buys no safety there and costs the gap. The instant the minimap drops, the mute arms on that tick
and every later behaviour (the hold that bridges the 1–3 s gaps between subtitle lines, the ambient
release) is untouched. **KNOWINGLY TRADED AWAY** and recorded in the file: the few frames at a
cutscene's start where the subtitle is up and the minimap has not gone yet. That window was the
mute's original purpose (2026-07-31) — but it was a REASONED fix for a crash never reproduced with
it off, against a confirmed, repeated, audible cost. First thing to revert if cutscene-start crashes
return.
**This is also the best candidate for "still slow after a battle"** (unconfirmed): battle results
and toasts both set `nav_mute = false`, so they never gated the radar — but post-battle CHATTER is a
dialogue surface and armed exactly this mute.

**The picker's "nothing to track": it gave up 1 s early on a deadline it could not compute.**
`Nav.targets_build_ms()` derives the wait from `explore_sweep_last_chunks` / `explore_sweep_max` —
telemetry produced by a COMPLETED build, which by definition does not exist on the first open of a
session. Both floor, it answers 200 ms, and a cold chunked build is ~15 classes at ~65 ms. Since
`Nav.targets_snap` is published only when the WHOLE build finishes, the picker opened on `{}` and
announced there was nothing to track — with the quest marker actively guiding, because the picker
has no fast path for the current target either. The second open works purely because the first
attempt's build finished in the background. **Fixed** by waiting on `Nav.targets_want`, which the
build sets for its whole duration and clears the moment it publishes — the build's own statement
about itself instead of an estimate. The old estimate stays as the FLOOR (it covers the ticks before
the nav loop has noticed the request) and a new `Nav.targets_wait_cap_ms()` bounds the case the
estimate was really for — a gated or stuck nav loop — derived from `EXPLORE_RESCAN_MS`, this file's
own statement of how stale a target list may be, so nothing is picked.

**Last updated:** 2026-08-03 (e) — **THE EMBLEMS REGRESSION WAS A PRECEDENCE BUG, NOT THE SCAN
CHANGE — AND THE RADAR'S COLD START HAD A 5-SECOND LOCKOUT ARMED BEFORE THE PROBE EVEN RAN.**
3 Lua files (`screen_community.lua`, `nav_tracker.lua`, `ui_core.lua`). **SOURCE-ONLY, UNVERIFIED
IN GAME.** Lint clean over 75 files. User feedback on (d): menus a little faster, **Soul Emblems
stopped reading**, radar still slow to start after a save load and after a battle.

**The emblems regression: the log named it, and it was NOT the ghost backoff.** The obvious suspect
was the new escalation starving the grid class — `screen_community` itself documents that exact
failure from 2026-07-16. The log says otherwise: `watch Start_Commu_Emb_C: 1 found` — the watch lane
bypassed the backoff and found the grid in ~1.8 s, exactly as designed. What it also caught:

```
commu claim=grid  board=false grid=true  rej=not-found
screen -> screen_community
commu claim=board board=true  grid=true  mode_v=2
```

The board appears one tick AFTER the grid and takes the screen. **Mode 2 was added to
`BOARD_LIVE_MODES` on 2026-07-31** to fix the opposite report (the board refusing to read until a
press moved it to 7). But mode 2 means *frame built and visible, input handlers not yet bound* — so
it cannot say whether the board or the emblem GRID (which renders underneath it) is the screen the
player is on, and treating "don't know" as "the board owns it" shadowed the grid into silence: the
2026-07-15 bug by a new route. **Fixed** with a `BOARD_OPENING_MODES` set: at mode 2 the grid is
asked first and wins if it is live with slots; otherwise the board claims exactly as before, so the
07-31 report stays fixed. The input-bound modes (7/9/12/13/14/16/17) keep absolute precedence over
the grid — that ordering is the 2026-07-03 lesson and is untouched. The same log supports the
reading: when the player really was on the board it showed `mode_v=7` with `grid=false`.

**Radar cold start, three causes, largest first.**
1. **`minimap()` armed its 5 s retry lockout BEFORE running the probe** (`mm_retry = tick +
   MM_RETRY_TICKS` above the `cached_live` call). One unlucky attempt — the HUD root not yet
   re-resolved after a load, which is precisely when this is first called — locked the minimap out
   for a full 5 s, and `world_alive()` is false for every one of those ticks, so the whole nav loop
   is dead: no auto-acquire, no beacon. Now armed only on a MISS, and `MM_RETRY_TICKS` cut 50 → 10
   (~1 s): the 5 s was chosen when this was a raw `FindAllOf`, whereas `Core.cached_live` now
   resolves it by pointer through the directory and owns the fallback's cost itself.
2. **The escalation was hitting the mod's own PREDICATE classes.** `AT_UIMiniMapRadar`,
   `Start_Top_C` and `Battle_Hud_P_Main_C` are resolved by pointer, so their SCAN only ever runs
   when the screen is genuinely absent — which parks them in the ghost ledger at `found = 0` looking
   like dead weight while being the exact opposite. Backing the minimap's fallback off to 16 s right
   after a load, when the directory roots have just been flushed and the pointer path is the one
   thing that cannot answer, would stall the radar by itself. New `NEVER_ESCALATE` set in `ui_core`.
3. **The auto-acquire carried an arbitrary phase across the gate.** `tick % SCAN_EVERY == 0` on a
   counter that free-runs while every gate is shut = up to 1.4 s of silence after control is already
   back. The rising edge of the world gate now sets `Nav.auto_now`, consumed by the first scan. Same
   defect as the manual resume fixed in (d).

**Last updated:** 2026-08-03 (d) — **THE MEASUREMENT CAME BACK AND IT IS BRUTAL: 71% OF ALL SCAN
TIME GOES TO CLASSES THAT ARE NEVER THERE.** 6 Lua files (`ui_core.lua`, `ui_registry.lua`,
`nav_tracker.lua`, `screen_title.lua`, `screen_agreement.lua`); no bridge rebuilt. **SOURCE-ONLY,
UNVERIFIED IN GAME.** Lint clean over 75 files.

**The dump (Ctrl+F5, 6.8 minutes of ordinary play).** Three lines carry the whole session:

```
ui step ms: max=925.0 avg=35.48 over 4102 ticks
findall scans: n=1982 total_ms=119463 max_ms=455.0 avg_ms=60.3
ghost classes: 42 never found (1397 scans, 84783 ms burned)
```

**119.5 s of game thread inside `FindAllOf` in a 410 s session — 29% — and 84.8 s of it (71%) spent
on 42 classes that were never once present.** A single scan averages 60 ms and peaked at 455 ms;
`SCANS_PER_TICK` is 2 on a 100 ms grid, so the scan machinery is allowed to spend up to 120% of a
tick, and that is exactly what a 925 ms `ui step` max is. **This is the same root cause behind BOTH
of today's player reports** — menus taking about a second before they can be navigated, and the
radar being slow to resume tracking: a real screen's scan queues behind the ghosts.
Worst offenders: `AT_UIQteMashAlert` 10.0 s, `Gametitle_C` 9.6 s, `AT_UIXcmnAgreement` 8.5 s,
`Map_World_Icon_C` 6.5 s, the five `CompZ_Page_*`/`Memo` classes ~25 s together, the three
`Start_Commu_*`/`AT_UICommunityStart` ~15.5 s.

**Read the list with its caveat.** "Ghost" here means "no scan of that name ever returned an
instance", which catches two different things: names the game truly never instantiates, and
DIRECTORY-MAPPED classes whose scan is only the fallback — those legitimately scan just when the
screen is absent, so they are wasted work too, but the fix for them is different. `AT_UIMiniMapRadar`
is in the list while the radar plainly works, which is the tell.

**Fixed (1): escalating backoff for proven ghosts.** `ui_core.absent_backoff` — a class the ledger
shows has NEVER been found doubles its backoff every 6 empty scans, 4 s → 8 s → 16 s, then flat.
Three carve-outs keep this inside the perf note's rule rather than breaking it: one single sighting
(`found > 0`, a session high-water mark) disarms it permanently; `QUIET_EXEMPT` — the surfaces that
appear with no user press, i.e. the "event-less popups" the rule was written about — never escalate;
and the boost window still bypasses the backoff entirely, so a screen the player OPENS is detected
exactly as fast as before. Only the no-event fallback slows down.

**Fixed (2): the title family stops scanning during gameplay — 18.1 s of the 84.8.** `Gametitle_C`
and `AT_UIXcmnAgreement` are the boot menu and the consent documents; they cannot exist once a save
is loaded, yet they scanned 137 and 122 times. The reason is structural and worth remembering: a
directory-mapped class whose OWN ROOT is unreachable correctly falls back to scanning (an
unreachable root cannot assert absence) — so a title-rooted screen scans forever precisely while it
is impossible. New `Core.gameplay_world()` (the `mm` root, which `ui_directory` documents as existing
only in playable worlds and `ui_registry` already trusts) is now the first line of both adapters. It
is the POSITIVE test — "we are in gameplay" — never "the title is gone", and it fails to false.

**Fixed (3): the radar's sweep build was being left OPEN, and had been since the (b) batch.**
`Nav.SW.lists` is cleared only by `Nav.sweep_partial()`, and **three of the five `list_targets`
callers never called it**. `class_list` serves a cached list without re-scanning, so after ONE such
call the sweep never scanned again and kept handing out actor handles gathered before the battle —
both the "radar takes a while to pick the last target back up" report and a dangling-handle hazard.
Now self-healing: only a build that reported PARTIAL may be resumed; anything else open at entry is
discarded, so the contract is structural instead of a rule every caller must remember.

**Fixed (4): the radar's resume is prompt and no longer sweeps inline.** It ran the full ~17-scan,
~1.2 s sweep on the first tick after the world came back — the worst possible moment — and only on
`tick % (SCAN_EVERY*3)`, a random phase inside a **4.5 s** window (its own comment claimed 3 s, stale
since `SCAN_EVERY` moved). It now asks the shared snapshot with `no_build`, which arms `explore_tick`'s
chunked builder (it runs every nav tick whether or not explore mode is on), makes its first attempt
as soon as that answers, and does not burn one of its 10 tries on a tick where no list existed yet.

**Fixed (5): `confirm_ticks = 1` was INERT — a 100 ms tax five adapters had explicitly declined.**
The first-sighting branch in `ui_registry` returned unconditionally, so nothing could commit before
its second tick and `= 1` behaved identically to the default 2. `screen_fishing`, `screen_saveload`,
`screen_skillcustom`, `screen_skilltree` and every `screen_list` instance (items, dragon balls) set
it. The playbook's rule — a deliberately-opened sub-screen sets it to 1, because the global debounce
exists only for screens that FLASH AT BOOT — now actually holds.

**Diagnosed, NOT changed:** the Z-Encyclopedia probes six scan-path classes in a fixed order with
the page shown on open (`CompZ_Page_Contents00_C`) checked LAST, so at 2 scans/tick it needs 3 ticks
just to reach it; `screen_palette` retries an incomplete collect only every 5 ticks (500 ms); the
sticky sweep can add 200 ms when the screen underneath does not notice it has been covered. All
three are real, and all three are dominated by the ghost queue — measure again before touching them.

**Last updated:** 2026-08-03 (c) — **GHOST HUNT, PHASE 1: THE METER NOW SEES EVERY SCAN, AND IT
NAMES THE CLASSES THAT NEVER EXIST.** 6 Lua files (`ui_core.lua`, `nav_tracker.lua`,
`screen_options.lua`, `screen_pause.lua`, `screen_title.lua`, `screen_palette.lua`); no bridge
rebuilt. **SOURCE-ONLY, UNVERIFIED IN GAME.** Lint clean over 75 files. **No behaviour change is
intended anywhere in this batch** — it adds accounting, and moves three copies of one test into the
substrate.

**Why phase 1 is a measurement and not a fix.** A sweep of every scanning call site found **35
classes that no `ui_directory` chain resolves**, across 19 adapters. Each class an adapter names but
the game never instantiates costs a full `FindAllOf` (~65 ms) every `ABSENT_BACKOFF` (~4 s) for the
whole session, and `ui_core`'s own comment records that a cluster of those expiring on one tick IS
the periodic stutter. The two obvious moves — delete names, or widen the backoff — are both
forbidden by evidence already in this repo. The perf note says outright: **"do NOT blind-tune
ABSENT_BACKOFF without an offender list (it starves event-less popups)."** And the four
native/Blueprint **TWIN pairs** the adapters carry (`Choice_Win_C`/`AT_UIChoiceWin`,
`Choice_Cmd_C`/`AT_UIChoiceCmd`, `Xcmn_Subtitles_C`/`ATUISubtitles`,
`Quest_Sub_Reward_C`/`AT_UIQuestSubReward`) cannot be settled by reading anything: **UE4SS's own
docs say `FindAllOf` matches subclasses, while this repo's `nav_tracker` comments record it
returning NOTHING for a native base when a subclass exists** (`:2925`, `:2951`). One half of each
pair is dead weight; only a measurement says which half.

**So the meter got fixed first.** (1) **`Core.findall`** — the timed scan, for the call sites that
cannot use the cache. `timed_findall` only ever saw scans routed through
`Core.cached_all`/`first_live`, which left the mod's single biggest scanner invisible to it;
`nav_tracker`'s own comment admitted that `list_targets`' ~17 raw scans "never route through
timed_findall, so `__KakarotScanStats` cannot even see it". The sweep (`Nav.SW.class_list` /
`manager_list`), the navi-icon pool, `AT_Character`, `RecastNavMesh` and the Options rows all report
now; the dev tools (F7 `discover`, F4 `dev_memdiff`, `ENEMY_PROBE`) deliberately still do not.
Accounting only — no budget taken, no backoff written, no behaviour changed. (2) A session-persistent
**scan ledger** (`Core.scan_ledger`) records, per class, the most instances a scan ever returned.
**`found = 0` after a session of play = a ghost.** Unlike `__KakarotScanStats` it is NOT reset by a
dump, because "never found" only means something across a whole session. (3) Ctrl+F5 prints a
`ghost classes: N never found (M scans, X ms burned)` block, worst first. That block is exactly the
offender list the perf note demands, and phase 2 is what spends it.

**Also shipped: a liveness test that had been copied into three files went back into the
substrate.** `screen_pause`, `screen_title` and `screen_palette` had each hand-rolled
`pcall(GetVisibility) == 0` rather than calling `Core.pane_live` — the exact shape the fishresult
lesson warns about ("when a fix is about the shared substrate, put it in the substrate") — and every
copy silently dropped `pane_live`'s opacity half, so a pane fading out could still be read. They
could not just call it as-is: `pane_live` fails OPEN on an unreadable enum and all three
deliberately fail CLOSED, each for a documented reason (the title must not blurt "Main menu" over
the intro movie, where the widget is on screen as HitTestInvisible; the pause pane stays resident
through battle). So `Core.pane_live(h, strict)` grew that single option and the three adapters use
it — same visibility contract as before, plus the fade check they were missing.

**Known and NOT changed (phase 2 candidates, no user report yet):** four adapters decide `is_active`
on a bare `host ~= nil` with no readable-text check — `screen_skilltree`, `screen_skillcustom`,
`screen_status`, `screen_saveload` — which is the documented "holds the tick in silence and shadows
everything below it" failure. They are listed rather than rewritten because that fix needs per-screen
evidence, and rewriting four working screens blind is how the Options regression happened.

**Next step:** one measured session (below), then the ghosts die with data behind it.

**Last updated:** 2026-08-03 — **THE INSERT RATE CAME DOWN 5× MORE AND THE D-PAD LOST NOTHING,
BECAUSE THE MOD ALREADY OWNED THE FIX AND THREE FILES NEVER ADOPTED IT.** 5 Lua files
(`pad_poll.lua`, `config_menu.lua`, `radar_menu.lua`, `screen_status.lua`, `screen_map.lua`);
**VERIFIED IN GAME 2026-08-03** (user): menus and the d-pad behave normally, cutscenes behave
normally, and the log/trail confirm both mechanisms directly — `dialogue nav_mute -> true
(Xcmn_Subtitles_C)` followed one second later by `-> false (free roam: ambient chatter)`, and
dispatches spaced 100-125 ms apart in free roam instead of the old fixed 20 ms. Lint passes.
Not separately observable and therefore still only reasoned: the `mem.lua` guard change (it only
shows itself when a throw happens) and the tick hook (`ENABLED = false`, see below).

The native latch in `input_bridge.c:220-224` is fed by the IAT hook on the **game's own**
`XInputGetState`, at frame rate, into an interlocked accumulator — its comment says it outright:
*"A press cannot be lost however late or irregular the drain is."* So the dispatch rate never
caught presses. Except that only `quest_read` and `map_travel` asked `Input.pressed`;
`config_menu:179`, `radar_menu:317` and `screen_status:237` each rolled a private two-tick LEVEL
compare, which only sees a button still HELD at dispatch time. **Those three were the whole reason
the bus "needed" 50 Hz** — and they were already dropping any tap that began and ended between two
dispatches whenever the busy guard skipped a tick.

**Shipped.** (1) The three holdouts now use `Input.pressed(mask) or (level compare)`, the form
`screen_map`'s `ft_pressed` always used. (2) `radar_menu`'s gesture windows converted to
`os.clock()` seconds (`DOUBLE_TAP_S = 0.40`, `DOUBLE_RESCUE_S = 0.68`); `tk` deleted. (3)
`pad_poll.lua` is **slow by default** — a 100 ms grid, lifted to 20 ms only while a stepper calls
`Poll.demand_fast(name, true)`; the four pad menus declare it at the TOP of their step, above every
early return, and `Poll.unregister` clears it, so nothing can pin the fast grid. `relax` outranks
the demand. **Steady state: ~10 dispatches/s in ordinary play, 50/s only while a d-pad menu is
open** — versus 77/s before this batch began.

**Pre-existing bug fixed on the way:** `radar_menu`'s double-tap window was counted in dispatches
(`DOUBLE_TAP_TICKS = 20`, "~400 ms at the 20 ms pad tick"), but the relax gate already drops the bus
to 100 ms during cutscenes and loads — so **in that state the window was really 2 seconds**.

**THE COMPLETE FIX IS NOW UNBLOCKED — the per-frame Blueprint tick exists and is named** (details
and caveats in the notes). A fresh IN-GAMEPLAY dump (2026-08-03, `Area11_P`) found
**`/Game/System/BP_ATGameModeMain.BP_ATGameModeMain_C:ReceiveTick(float DeltaSeconds)`**
(`BP_ATGameModeMain.hpp:11`): Blueprint bytecode (so `RegisterHook` takes UE4SS's mutex-protected,
`TRY`-wrapped `script_hook` path), engine-guaranteed singleton, exactly 1 live instance, doing real
per-tick work. `ExecuteInGameThread` is the only Lua API that allocates a state per call, so a
heartbeat on this takes the insert rate to **zero** and leaves the game thread as the sole Lua
executor. **IMPLEMENTED AND THEN DISABLED THE SAME DAY — IT CRASHED THE GAME AT BOOT**
(`EXCEPTION_ACCESS_VIOLATION reading 0x00000010`, the `UObjectBase::ClassPrivate` read). Evidence
is unambiguous about WHERE: `crash_trail.bin` recorded **0 marks** that session and `UE4SS.log`
ends at UE4SS's own `Event loop start`, so the process died before the FIRST `dispatch()` — and the
only new code above the first mark is `Hook.arm`'s `StaticFindObject` existence probe (`RegisterHook`
was never reached; no "tick hook installed" line). Not yet proven WHICH call faulted; the working
hypothesis is that at boot `Mem`'s class-pointer offset is not derived yet, so `Mem.alive`
correctly fails OPEN, `Core.valid` degrades to a bare `IsValid()`, and `IsValid` dereferences
whatever the lookup returned mid-async-load. `ENABLED = false` in `tick_hook.lua` until that is
settled; **the FINDING is still correct — only the arming path is wrong.** When retried, arm from
`Core.free_roam` (gameplay, class loaded, pre-check armed), never from boot, and do not probe by
object path. Also fixed in the same pass: `pad_poll`'s `require("tick_hook")` was HARD, so the
"delete the file to roll back" the header promised would have produced a mod that fails to load at
all — it is `pcall`'d now. Original design below, still accurate for the parts that stay:
in a new `tick_hook.lua` (its own file, like `header_hook.lua`,
so deleting it is the rollback). `pad_poll.Poll.pump` is now the single game-thread entry point for
both drivers and owns the cadence gate; the LoopAsync became a **watchdog** that queues NOTHING
while the hook's beat counter is advancing and takes over after 5 quiet ticks. A GameMode is
per-map (the title screen runs `BP_GameModeTitle_C`), which is exactly why the loop stays as the
fallback rather than being replaced. Registration is retried until the Blueprint class loads, gated
on a `Core.valid(StaticFindObject(path))` existence probe first — UE4SS's `RegisterHook` binding
throws on an unresolved path and that throw pierces pcall, which would have aborted the rest of the
steppers twice a second for as long as the player sat at the title screen. Watch for
`tick hook installed on …` in the log; its absence means the fallback is driving, which is simply
the behaviour of the previous batch.
Two corrections worth keeping: **never set `LoadAllAssetsBefore*=1`** — the installed ini records it
as a fatal `AutoDebugMainUI_C` crash, and it was not needed; and the earlier dumps were not missing,
they were **captured at the title screen**, which is the only reason the search had come back empty.

**Also fixed 2026-08-03 (user report): the radar went silent when characters talk while WALKING
AROUND.** `screen_dialogue`'s `nav_mute` classified by SURFACE, and no surface list can work —
this game draws the same subtitle surface for a cutscene and for the party's chatter during free
roam, and the story-call pop-up is a gameplay overlay by nature. The game's own discriminator is
the minimap (`Core.free_roam`), but it cannot just replace the surface test: at the START of a
cutscene the minimap is still up for a moment, and that moment is exactly the crash window the
mute exists to cover. So the two are combined by TIME — a scene surface still arms the mute
immediately, and it is released again only once free roam has held continuously for
`AMBIENT_GRACE_S` (= `MUTE_HOLD_S`, the registry's own debounce). Net: a brief mute at the start
of ambient chatter instead of one lasting the whole conversation; the cutscene safety case is
untouched. The transition log line now names the reason (`free roam: ambient chatter`) as well as
the surface. Note `commit_nav_mute` takes `tick` as a PARAMETER — the file-local of that name is
declared below it, so reading it directly would have compiled to a global.

**Next step:** see the test plan under *Next step* below — the risk in this batch is timing, not
crashes.

**Last updated:** 2026-08-02 — **FOUR CRASHES, ONE ENGINE BUG UNDERNEATH: `ExecuteInGameThread`
RACES UE4SS'S OWN `lua_instances` MAP, AND OUR GUARD TURNED ONE SURVIVABLE HIT INTO A DEAD SESSION.**
Four `UE4SS.log`s + three `crash_trail.bin`s from one player; **only one log has a Lua error at all**.
**SOURCE-ONLY, UNVERIFIED IN GAME** — 6 Lua files, no bridge rebuilt, but `pad_poll.lua` is now the
mod's single loop so a **full RESTART** is needed, not Ctrl+Shift+R. Full write-up (with the UE4SS
`file:line` chain) in [`reference/dbz-kakarot/notes/dbz-kakarot-crash-bug.md`](reference/dbz-kakarot/notes/dbz-kakarot-crash-bug.md).

**Root cause (source-verified vs RE-UE4SS `7d6f790`).** Every `ExecuteInGameThread` call creates a
new `lua_State` (`LuaMod.cpp:3057-3091` → `make_hook_state` :701, dedup commented out → `lua_newthread`)
and inserts it into `static std::unordered_map lua_instances` (`LuaMadeSimple.cpp:11`) — **no mutex,
no `erase` anywhere** — on the **async thread, outside** the mutex taken at `:3080`, while the **game
thread** reads that same map from `process_lua_function` (`:872`) on every reflected member access.
Five loops = ~77 races/s forever. A lost read throws (`:813-819`), and because Lua is compiled as C
(`setjmp`, no `catch`) the throw **pierces `pcall`**.

**The amplifier — ours, and what actually cost the session.** `Mem.alive`'s transactional
`guard.pending` was never cleared when the throw unwound past it, so the next call set
`guard.disabled = true` **permanently**: the mod's only out-of-VM memory guard went off 3 minutes into
a 2-hour session, `Core.valid` decayed to a bare `IsValid`, and the game ran unguarded until it died
of exactly the dangling-handle class the guard exists to stop. The log line even blamed the wrong
cause ("a non-UObject handle reached Mem.alive — find the caller"): the object was a live
`UGameInstance` and the throw landed in the `__index` metamethod, before the UObject was touched.

**Fixed.** (1) `mem.lua`: the guard is self-healing — an unwound attempt is logged and the pre-check
**stays on**; only 8 *consecutive* unwound attempts disable it (the signature of a real bad handle,
unreachable by the race). (2) `pad_poll.lua` is now the **single tick bus** — one `LoopAsync` + one
`ExecuteInGameThread` for everything, with `Poll.register_every(name, period_ms, fn, on_error,
should_run)`. `Core.loop`/`Nav`/`Battle`/`Quest` are steppers on it and their `_G.__Kakarot*Gen`
guards are gone. **~77 → ~50 races/s**; pad steppers dispatch first, periodic ones after, in
registration order, due by wall clock with half a tick of slack (a 100 ms stepper must not alias to
200 ms on the relaxed grid).

**Still open — the bigger half.** The bus still calls `ExecuteInGameThread` 50×/s, so 50
unsynchronized inserts/s remain and two threads still execute Lua on one shared `global_State`
(allocator + incremental GC) with no mutual exclusion. That is the best explanation for the **three**
crashes that had no Lua error and a working pre-check, and it fits the two-hour fuse. Going lower
means slowing the 20 ms pad loop — an input-latency trade that is the player's call.

**Next step:** play this batch and watch for **timing** regressions, because the loop merge is the
risky part, not the guard. Specifically: menus should still respond at 100 ms (if the reader feels
half-speed, the due-slack is wrong), the R3 picker and map/status d-pads should feel unchanged, and
the battle/objective narrators should still fire on time during cutscenes and loads (when the relax
gate makes the dispatch grid 100 ms). If a crash still happens, grab `crash_trail.bin` + `UE4SS.log`
before relaunching and check whether `memory pre-check` now logs *trips without a streak* — that
line appearing at all confirms the `lua_instances` race is live on the player's machine.

**Last updated:** 2026-07-31 (c) — **THE STREAMER'S CRASH TRAIL NAMED THE SITE OUTRIGHT: DIED INSIDE
`pad.tick`, INSIDE THE RADAR PICKER'S OWN DEFERRED TARGET SWEEP.** Two threads this session: the
crash trail below, and separate player feedback that the quest objective goes stale/unreadable.
**SOURCE-ONLY, UNVERIFIED IN GAME** — 6 Lua files plus this README and the mod's own
`README.txt`/`package.ps1`, no bridge rebuilt, but `main.lua` changed so a full RESTART is needed.

**1. The crash.** `crash_trail.bin` decoded with `tools/read-crash-trail.ps1`: 201,275 marks (~11
min), ordinary free-roam walking, died inside `pad.tick`. Off the trail alone: no adapter had
committed; **explore mode was ON** (`nav.explore` fires every `nav.step`); the pad was dispatching at
a genuine 16 ms (`__KakarotPadRelax` false, so not a cutscene/load); the last two `pad.tick` marks
are 16 ms apart — death landed inside an ordinary tick, not after a hang.
**Build correction:** the player was NOT on v0.1.4 — `Mem.mark("pad.tick")`/`Core.drop_memos()`
(`pad_poll.lua:91-92`) shipped in `93d539c`, AFTER the v0.1.4 tag (`cb4a30f`), so the build is in
`93d539c..HEAD`, almost certainly `0b60eaf`. **Date a trail's marks against the commit history,
never the tag list.**
**Root cause:** of the five pad-dispatch steppers (`config_menu`, `quest_read`, `radar_menu`,
`map_travel`, `status_pad`), four early-out on pure-Lua gates. The fifth — `radar_menu`'s deferred
single-tap open — called `Nav.list_targets()` inline on a cold/expired cache: **17 unbudgeted
`FindAllOf`, ~1.2 s of blocked game thread**, synchronously on the shared 20 ms pad dispatch. Explore
mode toggles on an R3 double-tap, so a player using it taps R3 continuously and every uncompleted
tap fell through to that open; `pad_poll.lua`'s `pcall` protects nothing against this engine's
uncatchable aborts. **Rule: a fast input loop must never invoke work costed in seconds, and a TTL
cache is only half a fix until the MISS path is checked too** — the earlier `targets_cached` TTL
change removed the sweep from the warm-cache path but left it on the cold-cache fallback, exactly
the state on a player's first press.
**Refuted:** `mm_cache`/`Nav.field_ready()` being re-walked by the pad loop was NOT the cause — every
stepper short-circuits first. Did settle a real contradiction: `AT_UIMiniMapRadar` IS per-level and
HUD-rooted (`ui_directory.lua:330`, `:535-537`), so `nav_tracker.lua:1501`'s "pooled widget, not a
world actor" comment is wrong about its own reason, though it still correctly stays unreleased.
**Fixed** (full mechanism in the [crash ledger](reference/dbz-kakarot/notes/dbz-kakarot-crash-bug.md)
2026-07-31 (c)): `actor_pos` self-guards with `Core.valid` (`nav_tracker.lua:362-367`); the 17-scan
sweep moved off the pad dispatch onto the nav loop via `Nav.targets_cached`/`Nav.targets_want`
(`:2587-2607`, `:2081-2096`); `do_open()` now returns true/false and both callers retry instead of
dropping the press (`radar_menu.lua:108-131`, `:214-218`, `:358-365`); `enemies_list()` dropped its
expiry conjunct, the same fix `best_candidate` already got on the Krillin report (`:1227-1238`);
`release_world_refs()` now also drops `human_mesh`/`invoker_key`/`invoker_nav` (`:1505-1529`);
`Nav.list_targets()` gets its own `Mem.mark("nav.sweep")` (`:2609-2616`); and `screen_dialogue.lua`'s
`nav_mute` is now derived per tick from which surface answered, closing the UI gate (and running
`release_world_refs`) for scene surfaces while leaving the overworld talk window/bubbles alone
(`:35-103`) — the reasoned, not-yet-confirmed fix for a separate report of a crash right as a
cutscene starts.

**2. Stale / unreadable objective** (user: "go to Lucca Village" only announced after several
restarts; stale at cutscene start; no way to re-hear it). `quest_objective.lua` now diffs a
`sig_main|sig_sub` signature instead of the composed string, so an HUD repaint that only changes the
COMPOSITION (which title candidate answered, how many rows passed `on_screen`) no longer reads as a
new goal (`:203-205`, `:348-371`); gated on `Core.scan_quiet()`, the one signal that survives the
adapter flipping in/out between subtitle lines (`:311`); `Speech.say_protected`'s commit moved to
AFTER the say, so a shredded line is no longer marked announced and lost (`:370-371`); `Quest.read()`
falls back to the last known text when a live read comes back empty (`:509-517`). `main.lua`: F10
moved out of the `Build.debug` block — it had never actually shipped (`:177-186`; `package.ps1`'s
comment updated to match) — and the `version.txt` lookup was reading one folder too deep, so every
packaged release logged itself as `dev` (`:38-47`). New **L3 + Y** pad chord for the same on-demand
re-read (`quest_objective.lua:519-577`, wired through `pad_poll.lua`, documented in
`mod/KakarotAccess/README.txt`). `README.md`'s crash-reporting section now tells reporters to copy
`crash_trail.bin` and `UE4SS.log` **before** relaunching, since the next launch resets the ring
(`:197-239`).

**Still open:** the cutscene-start crash has no trail confirming it yet (reasoned, not verified);
`screen_dialogue`'s `ui_muted()` still reads false once the registry drops `active` to nil in a
between-lines gap (a small window at scene START only — the world gate covers the body of a scene);
the explore sweep's ~1.2 s cost is unchanged, just relocated to the nav loop where it belongs; a
class-pointer stamp for `Mem.alive` was considered and deliberately NOT built (the premise — a stable
per-object Lua handle table — is unverified, and the mod's most shared guard is the wrong place to
risk a fail-closed regression).

**Next step:** get this batch played for real — the crash fix has no in-game confirmation yet and the
cutscene-start crash specifically has no trail of its own. Watch for the picker opening a tick late
while explore mode is active, for a battle/cutscene starting on a manual radar pick, and for F10/L3+Y
repeating the objective mid-scene. If another crash happens, get `crash_trail.bin` + `UE4SS.log`
before the reporter relaunches, and check any `pad.tick`-ending trail's build against `93d539c..HEAD`
before trusting a tag.

**Last updated:** 2026-07-31 (b) — **THE PICKER'S PAD LEAK, AND THE COMMUNITY BOARD TRACED TO THE
AUDIT ITSELF.** Two user reports. Everything below is SOURCE-ONLY and UNVERIFIED IN GAME, but it is
Lua-only: the bridges are already built and deployed (all four DLLs newer than their sources), so a
plain restart is enough.

**1. The radar/config pad leak** (user: *"al presionar L1 para cambiar de categoría empieza a sentir
el ki y al pulsar X para confirmar un objetivo empieza a saltar… ocurre a veces, no siempre"*).
The pad block is a 1 s LEASE that only `poll()` renews, and renewal deliberately cannot resurrect an
expired one — yet both pad menus asserted it exactly ONCE, in `do_open`. Any gap over a second in the
20 ms dispatch (a streaming level, a long speech call) killed the lease with the menu still open, and
from then on every button reached the game. Both menus now RE-ASSERT every tick; `Input.block(true)`
writes the deadline unconditionally (`input_bridge.c:474`), so re-asserting genuinely re-arms.
`blocked` is set alongside it, or a keyboard-opened picker leaves the Transition teardown unable to
release. **The morning's own anti-lockout fix was a SECOND cause**: a flat 2 s drain deadline forced
the release regardless of pad state, so a button still held at the deadline was handed straight to
the game — the anti-lockout *caused* the jump. Replaced by a stuck-detector whose deadline restarts
on every CHANGE of the button bitmask (a stuck button reports a constant mask; a pad in use never
does), triggers excluded because they jitter. `REL_TH` 25 → 30 to match XInput's own threshold.

**2. The community board went silent — and the cause was the audit's own resolver fix.** `bb1c85e`
repaired `is_userwidget`'s boot-miss latch, and that turned ON, for the first time in the mod's
entire shipped history, the `IsInViewport() == false` rejection at the tail of `Core.on_screen`
(`ui_core.lua:639`, root-only) — the predicate `screen_community` reaches ALL THREE of its hosts
through. It shipped with no log at all, which is why the screen went quiet with nothing to grep.
**`Core.VIEWPORT_GATE` now defaults FALSE** — that is the behaviour of every release the player has
ever run — and the rejection LOGS in both modes, so a session's play is the evidence for turning it
back on. `screen_community`'s claim trace (`board_rej` already distinguishes
`not-found`/`frame-invalid`/`frame-offscreen`/`no-mode`/`ghost-mode=N`) was split out of `DEBUG`
into `CLAIM_DEBUG`, because `DEBUG` also enables two dumps that WRITE FILES. **THE RULE: fixing a
resolver can silently ENABLE a gate that had never once run. A dead branch coming back to life is a
behaviour change, not a bug fix — ship it with a log and a switch, or it is indistinguishable from a
new bug in whatever it gates.**

**3. Ki-sense (LB) classification, source-verified** — from a tutorial the user quoted. The colour is
`UKiSearchComponent.HighlightObjectType` (`AT.hpp:41709`), enum `EHighlightObjectType`
(`AT_enums.hpp:6118`): `None, Animal, RareAnimal, Tree, Pier, MiningPoint, LevelLimit, Enemy,
TrainingPoint, Memories`. Yellow = Animal+Tree, red = RareAnimal, purple = MiningPoint, plus five the
tutorial never mentions. **Coverage is already complete**: `AAccessPointBase → AAccessPointItemBase →
{ChestAccessPoint, MineralMiningPointNormal→{Rare, WideUseBreakablePoint},
PlacementObjectInfo→{Recyclable}, TreasureAccessPoint→InsectAccessPoint}`, and `FindAllOf` includes
derived classes, so `list_targets`' `AccessPointItemBase` scan already sweeps all of it. TWO OPEN
OPPORTUNITIES, deliberately NOT applied (not the reported bugs): `PlacementObjectInfo` is a strict
SUBSET of that same scan, so one of the seventeen object-array walks is redundant (~68 ms of a
~1.2 s burst); and reading `HighlightObjectType` on the four families that declare it would replace
generic nouns with "rare mineral" / "breakable box" / "training point" — the same information the
colour gives a sighted player.

**4. "RESULTADOS DE LA HISTORIA" (story results after a major fight) — the census settled WHAT, the
gates are now instrumented for WHY.** The user captured an F7 census WITH the screen up
(`dumps/dump_1785513949_001.txt:435-458`), which named the whole structure outright:
`Quest_Main_Clear_C_4` → `Clear_Bar_Dummy00/01` (one per battle, `Txt_List` = "Gohan contra Oficial
del Ejército de Freezer" / "Vegeta contra Cui") → `Quest_Main_Clear_Detail00/01/02` (`Txt_List` =
"Tiempo de finalización" / "Combo máximo" / "Daño recibido"), each with a rank letter, plus a TOTAL
rank. That is `screen_results`' own host and layout — so the adapter targets the right screen.
- **PROVEN defect, fixed:** it read the battle title from `TextBox_Item` and each criterion from
  `TextBox_Detail`, but the text hangs off the Blueprint node **`Txt_List`**. Both now go through
  `Core.first_text` — the shared STRICT multi-candidate helper whose own comment names this exact
  `TextBox_*`/`Txt_List` alternation. (Reaching for it rather than a private probe is the standing
  "put the fix in the substrate" rule.)
- **ROOT CAUSE — `pane_live`, for the THIRD time.** `screen -> screen_results` appears in no log,
  ever, which puts the fault in `is_active`'s gates rather than in `lines()`. Of the three that can
  refuse there, the culprit is the liveness gate: `pane_live` demands ESlateVisibility `Visible(0)`,
  and a result sheet in this game renders HitTestInvisible/SelfHitTestInvisible (confirmed on its
  sibling `Gameover_C` at `screen_gameover.lua:47-50`), so it rejected the host on EVERY tick.
  Now `Core.pane_rendered`. The fingerprint is quoted verbatim in `screen_questreward.lua:29-37`,
  which hit this on 2026-07-28 — *"that is why this adapter NEVER RAN … `screen -> screen_questreward`
  never appears, not once, while the user's F7 census taken WITH the sheet on screen shows its title
  and all four reward rows rendered"* — and `screen_fishresult` hit it on 2026-07-17. This file's
  gate dates from 2026-07-24 and neither correction ever swept it.
- **The sweep that should have happened twice, done now:** every remaining `pane_live` call site was
  checked. All are genuinely INTERACTIVE pooled panes (`screen_cooking`, `screen_options`,
  `screen_dialogue`, `screen_agreement`, community's detail sheet), each documented as deliberate.
  `screen_results` was the last sheet left on the wrong gate. **RULE: `ui_core.lua:1669` already
  said "learned twice, which is why it now lives here" — but moving the HELPER into the substrate
  does not migrate the CALLERS. A substrate fix is not finished until the call sites are swept, and
  the sweep is the cheap half.**
- `is_active` also logs which gate refuses (`results gate: host-not-on-screen | pane-not-rendered |
  free-roam`), one line per CHANGE of reason — so if the remaining silence is one of the other two
  (note `first_on_screen` → `on_screen` → the viewport gate, item 2 above), the next log names it
  instead of costing another round.
- **The dump tool's fuse was never armed.** `discover.lua`'s `brush_of` fuse quotes its own episode —
  *"two caught brush_of errors, then 0xe06d7363"* — and then triggered at `>= 3`, i.e. it permitted
  the exact third probe that was fatal. Today's log carries those same two caught errors again.
  Threshold is now 2. **RULE: a fuse whose threshold sits above the failure it cites has never once
  fired in anger; check the arithmetic against the incident, not against the intent.**

**5. The "222" digit bug — SOLVED, and it needed no Ghidra.** Open since 2026-07-15; STATUS said the
values were *"presumably in the unreflected tail 0x3C0..0x418… pin the native value via F4/Ghidra"*.
**That line is superseded.** The moment the screen read for the first time, `screen_results`' own
`DEBUG` dump wrote `dumps/dump_results.txt` — the capture the ledger had been waiting two weeks for,
which was unobtainable only because the screen never activated. The digit is **not in any name**: all
digit images share ONE atlas material (`Ins_Num_Result02`, parent `Mat_Switch`, `columns=5 rows=2` =
ten cells) and the game selects the glyph with the scalar parameter **`Num`**, as a fraction of the
cell count. Decoder: `digit = round(Num * columns * rows)`, geometry read off the material so nothing
is hardcoded; order comes from the widget name (`_00` is most significant — the TArray runs the other
way, so reading it by index would say "04" for 40).
- **Verified twice against the user's own screenshot**: Gohan's max combo `_01`=0.4 `_02`=0.0 → **40**;
  Vegeta's `_01`=0.7 `_02`=0.6 → **76**. And both WRONG strings are reproduced digit-for-digit from the
  same dump — the old parse took the last digit of the shared atlas name `Ins_Num_Result02` → "2"×3 =
  **"222"**, and on the combo row took the MID's trailing widget index → **"21"**. Reproducing the
  bug's exact output is what makes this evidence rather than a story.
- An image still on the shared MaterialInstanceConstant is the UNDRIVEN template (baked `Num=0`,
  `Alpha=0` — a transparent placeholder), so it is skipped, and a row with none decodes to nil: the
  line reads label + rank with **no** number rather than "000".
- **RULE: a screen that cannot be reached cannot be diagnosed. The two-week-old "needs Ghidra" note
  was not a hard problem, it was a blocked one — the tool to answer it had been sitting in the file
  the whole time, behind a gate that never opened. Before pricing a deep RE task, check whether the
  cheap instrument simply never got to run.**
- **THE RANK — also solved, same session, and the cause is a NAME COLLISION.** The extended dump
  answered it in one capture: the rank image's material is a MID over `Ins_Rate_S` (criterion rows) /
  `Ins_Rate_M` (battle bar), carrying the same scalar `Num`. `rank_letter` was taking the trailing
  uppercase letters of the ASSET NAME — and `Ins_Rate_S` ends in `_S`, which is the icon's **SIZE**
  (small; the bar's `_M` is medium), not a rank. The size suffix simply collided with a valid rank
  letter, so every row of every battle read "S"; and on the bar it parsed "M", which is not a rank,
  so **the per-battle rank line was never spoken at all** — a second symptom nobody had connected to
  the first. Now decoded from `Num` like the digits.
- **The scale is S, A, B — three levels, no Z** (the user expected Z from the wider series).
  Source-verified four independent ways: `CrowdResultRank` (`AT_enums.hpp:414`) and `RankAnimType`
  (`:11648`) are both S=0/A=1/B=2; `UAT_UIQuestMainClearBar` and `UAT_UIQuestMainClearRank` declare
  exactly three animations each (`Anim_StartRankS/A/B`); and `FRankConditions` (`AT.hpp:9691`) is
  **0x6 bytes = three uint8 pairs**, so there is physically no room for a fourth tier. The capture
  agrees: `Num`=0.0 → S everywhere, `Num`=1/3 → A on the one row the screenshot shows an "A".
  `texture_token` and the `RANKS` letter set were deleted — nothing else used them.
- **RULE: when a parse reads an identifier the code did not author, spell out what each part of that
  identifier MEANS before trusting it.** `_S` was a size and `Num` was the value; the old reader had
  them exactly backwards, and both bugs looked like "the value is unavailable" rather than "we are
  reading the wrong field". A suffix that happens to be a member of your value set is not evidence.
- **VERIFIED IN GAME 2026-07-31** (user readout): *"Gohan contra Oficial del Ejército de Freezer, S.
  Vegeta contra Cui, S. Total, S. … Combo máximo, 40, A. … Combo máximo, 76, S."* Both digit values
  and both ranks match the screenshot exactly, the varying rank (A) comes through, and the three
  per-battle/total rank lines — which had never been spoken once — now appear. Digits and rank both
  closed.
- **Time / damage rows carry NO number, and that is correct.** In BOTH dump passes their digit images
  still point at the shared template at Alpha=0, i.e. they draw nothing, and the screenshot
  description shows only a rank icon on those rows. Two independent signals; the reader now says
  label + rank and no figure. Residual doubt, not chased: `UAT_UIQuestMainClearDetail.Canvas_Number`
  @0x398 is not walked by the dump, so a time rendered as "1:23" by other glyphs inside it would be
  invisible to us. One screenshot settles it if it ever matters.
- **Open, cosmetic:** the spoken ORDER is the reveal order, not the screen order — the three rank
  lines land first (they become readable first), then every detail row. Each detail is prefixed with
  its battle, so nothing is ambiguous, but the playbook's "follow the game's on-screen order" rule is
  not strictly met. Left alone deliberately: fixing it means holding lines back through the reveal
  animation, which trades a real property (say it as soon as it is true) for a cosmetic one.
- **NOTE FOR THE NEXT SESSION: the user is BLIND — never ask them to confirm what is on screen.**
  Ask for a SCREENSHOT (they can capture without seeing) and have it described, which is exactly how
  the 40/76 values were pinned. This was asked the wrong way twice in one session.

**6. Community board, mode 2 is treated as a ghost** (user: *"me empezó a leer solo después de que
pulsé sobre un slot vacío"*). The claim trace caught it live: `rej=ghost-mode=2`, then the press moved
it to `mode_v=7` and it claimed instantly. `BOARD_LIVE_MODES` is `{7,9,12,13,14,16,17}`, derived from
the game's own mode machine (`FUN_1414c7de0`), and **2 appears nowhere** — not in the set, not in the
comment that lists what the other values mean. A parked ghost is not interactable, and the user was
interacting with it, so 2 looks like a real browse state. NOT added blind: the set is Ghidra-derived
and a wrong entry re-opens the ghost-board shadowing that cost a session on 2026-07-15. **Re-read in
Ghidra, and 2 is now IN the set.** The step dispatcher `FUN_1414d6380` writes 2 at step 8 — frame
built, root set Visible — and the board then waits on the community manager's two-pane rendezvous
(`FUN_141504c30` → `FUN_1414fc090`), which binds the input handlers (`FUN_1414c8a40`, called ONLY on
the transition into 7) and writes 7. So 2 means *the board owns the screen but the cursor is not
bound yet* — exactly the shape of 12/13/14/17, which were already in the set. Verified it claims with
something to say: `board_update` gates only the hover read on `mode == 7`, while the header, title and
entry summary do not (`screen_community.lua:1181-1191`, `:1202-1213`), so this cannot recreate the
"holds the tick in silence" failure. No route back to the ghost bug: a parked board sits at 0 or 5,
and 2 is only ever written during an active open. **5 confirmed as the out animation**
(`FUN_1414ca430` sets 5, plays it, collapses the frame, then 0) — stays out. **3 deliberately NOT
added**: the same analysis calls it the twin of 1 (frame still constructing, deliberately absent) yet
marks it live, and it has never been observed — an unresolved contradiction is not evidence.
**Ghidra artifacts from this pass — KEPT IN PLACE, not copied into the repo** (user's call,
2026-07-31), at
`C:\Users\ali-b\AppData\Local\Temp\claude\D--code-unreal-dragon-ball-kakarot-access\1a3bd350-0c6c-4a6c-b98a-bf3c3283136c\scratchpad\`:
27 decompiled bodies under `dec\f_<addr>.c` (the ones that matter: `f_1414d6380.c` the step
dispatcher, `f_1414c8a40.c` the input binder, `f_1414ca430.c` the close routine, `f_141504c30.c` and
`f_1414fc090.c` the two-pane rendezvous), the scans `commu_mode_scan.txt` / `commu_anim_scan.txt` /
`callers.txt`, and the headless scripts `commu_mode.java`, `commu_anim.java`, `callers.java`,
`dec_tmp.java`.
> **This pointer WILL rot.** That directory is under `%LOCALAPPDATA%\Temp` and keyed to one
> assistant session id — Windows cleans Temp, and nothing recreates the folder. The findings
> themselves are safe because they are written out above; treat the path as a bonus, and if the
> decompiled bodies are ever wanted again, either copy them into `code/decompiled/` (where
> `commu_tick_callers.c` already lives) before that happens, or just re-run the headless pass, which
> is minutes now that the analysis is cached.

**7. Reading ORDER on the results sheet** (user request). It spoke in REVEAL order — the three rank
lines first, then every detail row — because each line was said the instant it became readable.
`lines()` already builds in screen order, so the fix is to HOLD the sheet until it stops growing and
release it in one go: settle `SETTLE_S` = 0.6 s of no new lines, ceiling `HOLD_MAX_S` = 5 s, both in
wall time. Deliberately NOT a per-line "wait for the previous one" rule: `DETAIL_COUNT` is 6 while
this screen uses 3, so that rule must tell "row does not exist" from "row is not ready", and getting
it wrong stalls the reader forever — worse than the cosmetic defect. Fails open twice (settle, then
ceiling), so the worst case is the old reveal order, never silence. **UNVERIFIED IN GAME** — the only
thing in v0.1.5 that is.

**Released v0.1.5** (2026-07-31) — braille, the crash audit, the map d-pad, and this session's
results/board/radar fixes.

**Next step:** confirm the results sheet reads in screen order (the one untested change). Then, if
anything is still off: `results gate:` in the log names a refusing gate, `commu claim=` names a board
mode — watch for `ghost-mode=` values other than 5, since 2 has now been reclassified as live.
Still open, both deliberately deferred: `UAT_UIQuestMainClearDetail.Canvas_Number` @0x398 is never
walked, so a completion time drawn there by other glyphs would be invisible to us (one screenshot
settles it); and the two radar opportunities from the ki-sense work — the redundant
`PlacementObjectInfo` scan, and `HighlightObjectType` for precise nouns.

**Last updated:** 2026-07-31 — **BRAILLE DISPLAYS, THE MAP D-PAD, AND A 23-ITEM CRASH AUDIT (22
applied, 1 refused).** Three user requests plus a fresh crash report; everything below is
SOURCE-ONLY, UNVERIFIED IN GAME, and needs a full RESTART (every bridge but `mem_bridge` was
rebuilt, and `main.lua`/`app.lua` changed).

**1. Braille output (user request).** `speech.lua` called `prism.say` → `prism_backend_speak`, which
is SPEECH ONLY, and the bridge never even resolved the braille export — so a braille display saw
nothing the mod said. `prism_bridge.c` gains `braille()` and `features()` (bit values from
`PrismBackendFeature` in `prism.h`, nothing hardcoded), both resolved with a new `RESOLVE_OPT` so an
older `prism.dll` cannot cost the player their reader. Every utterance now also goes to
`prism_backend_braille` as a SEPARATE, ADDITIVE call. **Deliberately NOT routed through
`prism_backend_output`** (the library's combined speak+braille path): one call instead of two, but it
puts ALL speech on an entry point whose semantics we cannot verify without the game, and speech is
the mod's lifeline. Setting `braille = auto|on|off` in `config.txt` + the L3+R3 menu; `auto` fails
OPEN when the backend cannot answer the capability query. 13 languages, README updated.

**2. Map: three symptoms, three different causes** (user: *"navigating the map can feel a bit laggy,
or it will sometimes miss places when you use the DPad… not sure what pressing X does, but I just
notice it reading the names for the different locations. I was expecting to have more info"*).
- *Missed presses* — `input_bridge.c` only stored the pad's LEVEL (`g_last`), so a tap whose whole
  down-up cycle fell between two polls was INVISIBLE; and the polls are neither fast nor evenly
  spaced (the 20 ms dispatch drops ticks when the game thread is busy, throttles to 100 ms during
  loads via its RELAX gate, and every step that speaks blocks the game thread). The HOOK now
  accumulates RISING EDGES at frame rate (`note_buttons`/`g_edgeAcc`/`take_edges`); `pad_poll`
  drains it ONCE per tick and republishes (`Input.edges()`) — the latch is destructive, so a second
  drainer would steal every other stepper's presses.
- *Laggy* — no d-pad auto-repeat (a 20-destination list was 20 presses), and `ft_write_sel` ran
  every 20 ms through `Mem.write_i32` → `class_ok` → `class_chain`, i.e. a full reflected super-walk
  50×/s on the game thread. Now: hold-to-repeat, and the index is READ BACK (a plain guarded native
  read, no reflection) and only written when it has drifted — same guarantee, a fraction of the cost.
- *X* — **X was never a mod bind.** It is one of `ui_registry`'s `BOOST_BTNS`, so pressing any face
  button lifted the scan quiet window, the travel-icon pool finally scanned, and the once-per-opening
  "N travel points: …" line fired right then: an info key by accident. Now it is a real one
  (`ft_describe`): selected destination + position in the list + what the free cursor is over, and on
  the AREA map it re-reads the focused POI. Announced on entry so it is discoverable.
- **What we canNOT say about a destination**: an inventory pass over the headers/dumps found that
  only the NAME is source-verified. `entryUnlocked` (+0x11) reads 0 on points known to be accessible
  — contradicted, unusable; `entryAreaId` (+0x14) exists but has no id→text table; `entryId` (+0x1c)
  feeds `FUN_1415bd150` to build the game's own "Go to X?" string, which is unreversed. Do not
  promise more without doing that RE.

**3. The crash audit.** A 13-subsystem sweep with adversarial per-finding verification: 66 findings
judged, 40 survived, 26 refuted; deduped to 23 ranked fixes. **22 applied.** The substrate ones worth
remembering: `Core.array_of` gained the `strict` twin of `Core.member`'s (a multi-candidate probe
EXPECTS its candidates to be absent, so fail-open there is a licence for the uncatchable abort) and
`ui_directory`'s chain walk passes it automatically for any class with ALTERNATIVE chains, derived
from the table rather than annotated; `prop_sets[key] = false` is now written only for a walk that
COMPLETED (a transient `ForEachProperty` raise used to freeze a guard closed for the whole map, with
no error and no log); `custom_props` is flushed on map transition like every other reflection cache
beside it; `Registry.stop()` clears `active` (Ctrl+M with a screen committed left a stale pointer
that muted the radar and locked out the R3 picker AND the config menu for the session); adapter
`reset()` and both watchers are now fault-isolated like `is_active`/`update`; `Core.peek_all`'s
"never scans" contract is TRUE at the source via a `no_scan` mode threaded to the directory roots;
and four keybind handlers (F3, Ctrl+M, Ctrl+F2, F8/Ctrl+F8) were running their bodies on UE4SS's
KEYBOARD thread — the 2026-07-27 sweep missed them because they are delegated through `app.lua`
rather than bound directly.
**ONE FIX REFUSED, and the reason generalises:** the plan wanted `pad_block_renew()` removed from
`l_poll` "because the owning menus already call `Input.block(true)` every tick". They do NOT — it is
called ONCE at open (`radar_menu.lua:87`, `config_menu.lua:133`); only `kb_block` is renewed. Applying
it would have expired the pad block after 1 s with the menu still open. The half that was real — the
close-drain can be UNSATISFIABLE because `REL_TH = 25` sits below XInput's own
`XINPUT_GAMEPAD_TRIGGER_THRESHOLD` of 30, so a worn trigger resting at 26-29 strands the player with
no pad and no keyboard — is fixed with a 2 s wall-clock ceiling in both menus. **Verify a plan's
premise against the code before trusting it, even a verified one.**

**4. The Krillin-cutscene report** (*objective re-announced during and after the cutscene; game got
slow; Fatal error*). Both symptoms are the SAME defect. The objective line is a COMPOSITION — which
of three title candidates answers, how many rows pass `on_screen`, whether the sub group contributed
— diff-gated as ONE string, so any repaint that changes only the composition reads as a new
objective. The settle that existed counted POLLS, and `step`'s three gates return WITHOUT touching
the counter: during a cutscene the dialogue adapter commits IN on every subtitle line and OUT in
every gap, so two "consecutive" polls could be SECONDS apart with half a cutscene between them,
confirming a transient reading as stable. (The playbook's *a debounce measured in calls is not a
debounce* rule, reached from the STARVED side rather than the over-called one — worth remembering as
its own shape.) The same flicker fed the radar's "objective changed" signal with no debounce at all,
re-arming `preempt.scans` every 300 ms while its consumer drains it once per ~1.5 s → PERMANENTLY
armed → an armed preempt bypasses the suppressors by design → the marker walk kept dereferencing
per-level pooled minimap widgets straight through the cutscene, and **a cutscene is not a
Transition**, so nothing released them while sub-levels streamed out. Fixed: settle in WALL TIME +
candidate dropped on every unobserved poll; the change signal only fires from a settled reading;
`preempt.scans` arms from zero only; and `navi_icons` is dropped whenever an overlay owns the screen,
not only when it has ALSO expired (the existing comment's own argument — the engine frees these and
re-validation cannot see a recycled address — never depended on expiry). **The crash SITE is not
pinned and was not guessed**: added `Mem.mark("nav.markers")`/`Mem.mark("nav.mapicons")`, because
`best_candidate`'s two walks were an unmarked window inside `nav.step`, plus `objective ->` /
`objective change ->` log lines naming which composition flipped. One trail now settles both halves.
Independent candidate for the slowdown, fixed separately as RANK 6: `build_bindings` re-issued
`StaticFindObject` + a synchronous `LoadAsset` on EVERY call while unresolved, and every dialogue
line carrying an `<inputicon>` glyph goes through it — i.e. loader work at several hertz inside the
async-load window that has deadlocked this game twice.

**Last updated:** 2026-07-29 (d) — **the double-R3 "freeze" was an UNBOUNDED HANG, now fixed, and the
crash black box can be read OFFLINE.** Two user reports: a crash just walking
around **West City**, and explore mode (double-R3) **hanging the game with no crash message, needing
the process killed**. Current release is **v0.1.4** (`cb4a30f`, 07-29 15:23Z) and it already contains
`9a7a869` + `114b980`; the trail session below ran at 16:10Z, i.e. **on v0.1.4 code**, so the hang is
live for every player and needs a v0.1.5. (Check `gh release list` / `git ls-remote --tags`, not the
local tag list — a stale local `git tag` produced a wrong reading of this episode first time round.) New capability first, because it is the reusable part:
**`tools/read-crash-trail.ps1`** decodes `crash_trail.bin` without relaunching the game (format is
fixed at `src/mem_bridge/mem_bridge.c:245-255`), so a reporter sends a 16 KB file instead of being
talked through a restart. It immediately measured what this ledger had only estimated: last op
`nav.explore`, and a **438 ms gap between that mark and the next** — the mark is written before the
call, so that is `explore_rescan` blocking the game thread in ONE tick. ROOT CAUSE of the hang:
`explore_rescan` wrote `explore_sx`/`explore_scan_ms` at its END, while `explore_tick`'s gate treats
`explore_sx == nil` as "rescan unconditionally" — so any fault inside `Nav.list_targets` re-ran the
whole ~1.2 s 17-scan sweep **every 100 ms for the rest of the session** (saturated game thread, no
frames, no input, no dump — a hang never reaches the crash reporter). Fixed by committing the attempt
BEFORE the work; `explore_pois` still commits last so a faulting sweep serves the last good list
instead of going silent. **THE RULE (generalises the 07-28 "commit input edges before any early
return" past input): any periodic job whose should-I-run gate is the state the job writes ON SUCCESS
will spin at full loop frequency the first time it fails — and a sentinel meaning both "never done"
and "do it NOW" has no fallback.** The burst also logs itself now (worst sweep once per session,
thresholded at `TICK_MS`, derived not picked); it was invisible for its whole life because it takes no
scan slot and never routes through `timed_findall`, so `__KakarotScanStats` could not see it either.
West City is **NOT closed**: a 17-agent adversarial pass (11 candidates, 4 survived, 7 refuted) ranks
the hand-picked `target.actor` first — free roam fires no gate edge so `release_world_refs` never
runs, and `target_missing` is only incremented for NON-manual targets, so an auto target is bounded to
~4.5 s while a manual pointer is aged by nothing and dereferenced at 10 Hz indefinitely — but the
precondition (did the reporter pick a target?) is unverified and the local trail says `nav.explore`,
not `nav.step`. Applied the fix that needs no precondition: while `target_missing > 0` the tracker
**coasts on the last position it read** (`target.lx/ly/lz`, plain numbers, never a handle) instead of
touching a handle the last sweep failed to find. Deliberately NOT applied, with reasons in the
[crash ledger](reference/dbz-kakarot/notes/dbz-kakarot-crash-bug.md): displacement-dropping the manual
pointer (`resume_pick.key` is an ADDRESS — a freed actor can never re-match, and a hand-picked beacon
going permanently silent is worse than the crash) and displacement-dropping
`enemy_cache`/`navi_icons` (`navi_icons` is shared substrate for all auto-tracking). Lint clean over
74 files. **SOURCE-ONLY, UNVERIFIED IN GAME.**
**THE REPORTER'S TRAIL THEN ARRIVED AND EXONERATED THE PRIME SUSPECT** (2026-07-29 (e)): it ends in
`nav.explore` too, but the committed adapter was `screen_gameover` (**the player had DIED**), and
`screen_gameover` MUTES the nav loop — so `step()` and `explore_tick` both returned at their gates
having touched no engine object, and the cadence was a flat 94-109 ms with no stall, so no sweep
either. `nav.explore` is a **BLIND SPOT, not a culprit**: nothing marks the registry PROLOGUE (the
transition check, then `pad_boost` -> `Core.boost_missing`, which re-scans every missing pool while
the world tears down). Added `Mem.mark("ui.tick")` + `Mem.mark("ui.boost")` so the next trail is
decisive. **RULE, earned twice now: when two candidates share an unmarked window the deliverable is a
MARK, not a hypothesis.** So West City is still open, the manual-`target.actor` candidate is NOT what
this crash was, and the next round should look at what the mod does during a death/respawn teardown.
**THEN A 14-CANDIDATE PASS ON THE SECOND TRAIL (2026-07-29 (f)) KILLED 9, INCLUDING THE UNIFYING
THEORY AND THE WHOLE BATTLE_MONITOR LEAD.** Second reporter trail: last mark `battle.step`, free roam,
no adapter, player collecting pod parts with the R3 picker. 5 survived of 14 — and **4 of the 5 are
INSTRUMENTATION findings, not crash mechanisms.** Refuted (do not re-open): `ui_directory.roots` and
`all_cache` serving freed handles (my shared-substrate theory, filed with instructions to attack it),
all three `battle_monitor` candidates, the teardown-edge gate, `screen_gameover`'s debug probe, and
`chain_wait.actor` — which looked tailor-made for the pod-parts context and still died.
**THE ONE REAL DEFECT (process-death) is this ledger's own rule applied to 4 of 6 callers:**
`valid_memo`/`os_memo` are cleared only by `Core.poll_world` and `Core.begin_scan_tick`, so
`pad_poll`'s **50 Hz** dispatch and every keybind handler read validity verdicts computed up to
~100 ms earlier in another loop — **skipping `Mem.alive`, the only guard outside the VM** — while
`radar_menu`/`config_menu` really do dereference on every R3 edge. Fixed with `Core.drop_memos()`
called once per pad dispatch (NOT `begin_scan_tick`: that would refill the scan budget 5x faster than
the wall-clock ceiling). Honest limit: refuters rate it "explains NEITHER trail" — hardening, not the
proven cause. **STRUCTURAL FINDING: the black box attributes at LOOP-ENTRY granularity** — every loop
marks its entry then runs an unmarked body, and `begin_scan_tick`/`poll_world` run BEFORE their loop's
mark. Yesterday's `ui.boost` mark was on inert code (`boost_missing` is 3 lines of Lua arithmetic);
removed, replaced by `core.world` at the epoch read and **`pad.tick` in `pad_poll`, which had zero
marks in the whole file** — the dominant blind lane. **CORRECTION to my own trail reading:**
`GetTickCount64` granularity is ~15.6 ms, so trail deltas are quantized; bucket 311679703 holds TWO
registry ticks and TWO nav ticks, i.e. a queue drain after ~100 ms of serialization, and the fatal
`battle.step` ran 156 ms after the previous on a 250 ms loop — LATE. Death followed a stall.
**DONE THE SAME DAY (2026-07-30), both needing a FULL RESTART:**
(1) **`MARK_SLOTS` 64 → 256** — the black box goes from ~0.21 s of history to ~0.85 s (the mod writes
~300 marks/s; both reporter trails measured 172 ms end to end, too short to show the tick BEFORE the
fatal one, which is exactly the tick you want when death follows a stall). `mem_bridge.dll` rebuilt.
Two things fell out of it: **`MARK_BYTES` was never actually derived** despite its own comment saying
so — a hardcoded 16384 with an assert checking only "large enough", the same latent class as the
original 8192-vs-8224 bug, and this change would have tripped it (32 + 256*128 = 32800); it is now the
expression itself. And **recovery now accepts a SMALLER ring and reads it with its own slot count**,
because the strict `hdr.slots == MARK_SLOTS` check would have discarded the pending trail on the very
boot a player upgrades. Reader verified against a synthesized 256-slot file (300 written, 256
recovered, oldest 44 overwritten, wrap arithmetic correct).
(2) **The mod now names its build in the first line of the log**, read from the `version.txt`
`package.ps1` already stages — not a hardcoded constant, which would drift from the tag. A player's log
did not say which release produced it, and that is not hypothetical: a whole round of this
investigation was framed against v0.1.3 because the only version evidence was a stale local `git tag`.
An unpackaged tree prints `dev (unpackaged, no version.txt)`.
NOTE the game install's `Mods\KakarotAccess\Scripts` is a **junction to the repo**, so builds and Lua
edits are live there with no deploy step.
Next: verify in game, cut **v0.1.5** (the (d) hang is live in v0.1.4). **West City is STILL
unexplained** — the next trail should name it now that `pad.tick`/`core.world` closed the blind lanes
and the ring covers several full loop cycles either side of the death.
Previous entry: 2026-07-29 (c) — **full-codebase crash sweep, second pass: 14 candidates, 7
confirmed, 7 killed by refutation — all 7 fixed, SOURCE-ONLY and UNVERIFIED IN GAME.** Prompted by
the user still hitting random crashes on v0.1.3. All 74 Lua files + the 4 native bridges re-read
against the crash ledger's mechanism catalogue, grouped by hot-path risk, every candidate given an
independent adversarial refuter. Full writeup in the
[crash ledger](reference/dbz-kakarot/notes/dbz-kakarot-crash-bug.md) — **including the 7 that DIED,
so nobody re-opens them** (notably: `pad_poll.lua`'s `steppers[name]=nil` is NOT the bug and needs
no lock; `Nav.toggle` on plain F3 is safe because it touches no engine object; `screen_community`'s
`det.Txt_Name` is a real reflected property; `ui_core.lua:1430`'s `custom_props` trap has a dead-code
consumer). THE BIG ONE: **`nav_tracker`'s world-actor release never ran if a MENU was on screen when
the world went away.** All three gates share one `gated_prev` latch, but the release lived only in
the world gate's edge — so a muting adapter (fishing HUD lingering ~3 s, loading pane ~10 s, an NPC
yes/no answered straight into a fight) latched it first and the loop carried `enemy_cache`,
`navi_icons`, `target.actor` and `chain_wait.actor` through the whole battle. A battle is NOT a
`Transition`, so `Transition.on_begin` never covered it, and re-validating cannot save you — a
recycled address passes both `Mem.alive` and `IsValid`. That is a REGRESSION of a fix written for a
confirmed user crash, and its need-a-menu-AND-a-fight-to-overlap shape is exactly why the crash
looked triggerless. Fixed via `Nav.release_world_refs()` called from both edges (on the module table,
not a `local` — this file is at Lua's 200-local ceiling). Also fixed: **two keybinds ran engine work
on UE4SS's own thread** — Shift+F3 (`Nav.toggle_route` → `FindFirstOf` + `UnregisterNavigationInvoker`
+ mutation of the shared memo tables) and **Ctrl+Shift+R, which is NOT dev-gated and the README
advertises it**, where `require("app")` reparses ~60 modules on the keybind thread against the same
`global_State`; the 2026-07-27 sweep missed both because they are delegated through `app.lua`.
Plus `screen_choicelist` calling `GetFullName()` before its guard (sole outlier in the mod),
`A.shop_money` probing multi-candidate names non-strict (its own comment asserted the false safety
property), `explore_rescan` now bailing on `Core.scan_quiet()`, and in the native bridge
**`g_haveLast` was a one-way latch: a pad disconnect froze the last frame forever, so pad-loss
recovery never ran and an open radar picker kept the keyboard blocked for the session with no way
out from inside the game** (`input_bridge.dll` rebuilt — needs a full RESTART, not Ctrl+Shift+R).
KNOWN RESIDUAL, deliberately not fixed here: `explore_rescan` still fires 17 raw `FindAllOf`
(~1.2 s of game thread) every 4 s while exploring; the cheaper source changes what explore mode can
find, so it wants a Ctrl+F5 measurement and a decision first. Do NOT budget-gate `list_targets` —
it is shared with the R3 picker and this loop refills only the property budget.
Previous entry: 2026-07-29 (b) — **an external UE4SS performance audit reviewed claim by claim:
four of its five code proposals REJECTED, and the one finding worth having was in the section it
marked as unverified.** Full writeup, with the reasons, in
[the audit review note](reference/dbz-kakarot/notes/dbz-kakarot-ue4ss-perf-audit-2026-07-28.md) — read
that before re-opening any of it. Its Part 0 premise (every Lua property access does a fresh
`FindProperty`, never memoised) is TRUE at v3.0.1 but its citation was **fabricated**
(`LuaUObject.hpp:817-833` is `push_integer`; the quoted "hot code paths" comment exists nowhere in
RE-UE4SS) — the real funnel is `prepare_to_handle`, `:644-681`. And the number that decides everything:
one property access ≈ **a microsecond** against the **~65 ms** `FindAllOf` this mod already measures, so
every "hoist the chain / cache the sub-widget" proposal was trading a real staleness risk for an
unmeasurable gain. All four rejected proposals (`nav_tracker` navi-icon switcher, `quest_objective` rows,
`screen_map` area_poi, `ui_directory` cross-tick memo) were killed on the SAME ground: each adds a
cross-tick cache on pooled widgets, the failure class that has silenced this mod four times, and two of
them were on modules with **no `reset()` to hook**. `screen_map`'s item additionally described code that
`114b980` had already fixed. APPLIED instead — three zero-new-state changes plus a packaging fix:
(1) `keyhelp.lua:63-73` tests visibility BEFORE `GetFullName()`, so the ~2 Hz auto-reader (armed in
nearly every menu) pays for the path string once instead of once per pooled instance; (2)
`screen_results.lua` skips rank/digit texture resolution for lines already in `spoken` — reusing the
existing set, no new lifetime, and `name` is still read because the first detail row is prefixed with it;
(3) `screen_map.lua:424` `cached_all` → `peek_all` (found by adversarial review, NOT by the audit):
`ft_build` is reachable from the 20 ms pad loop and was calling a scanning helper there, the exact rule
`ft_host`'s own comment forbids — harmless in practice (the class is never `due` at that point) but one
backoff expiry from a 65 ms scan on the input loop; (4) **`package.ps1` now forces the release debug
profile by CONSTRUCTION** — it copied `UE4SS-settings.ini` verbatim from the packaging machine, and the
only thing keeping the debug console out of releases was a comment asking a human to remember. The three
console keys default to 1 when absent, so a missing key is written in, inside `[Debug]`. Six-case test
passed. Docs corrected too: our `ue4ss-api-reference.md` copy had the `FindObject` flag order
**reversed** (it is `RequiredFlags, BannedFlags` — code, `Types.lua` and the official docs all agree; the
audit's claim that the official docs are wrong is false), `--disable-ue4ss` is documented as
non-existent at v3.0.1, and the busy-guard **clear-on-ENTRY** rule and its reasoning are now in
`accessibility-patterns.md` (both the repo copy and the playbook source). SOURCE-ONLY, UNVERIFIED IN
GAME; the Lua changes need a reload, `package.ps1` only affects the next package.
Previous entry: 2026-07-29 (a) — **a crash report named the wrong screen, and chasing it down the
log found a NEW substrate bug: partial property-set caching.** User reported a crash "on the party
screen" (UE4SS.log 00:58:56→01:25:12, ends with no shutdown line, no traceback — a hard process
death). It was NOT the party screen: `screen_party` was entered and left cleanly twice (01:23:58,
01:24:56). The process died 8 s later, the same second the SKILL TREE was entered, on the log's last
line: `array gate: Start_Skilltree_C has no 'WL_Skilltree_Zorb00' (not read)`. That gate line dedupes
per (class address, member), so it was the FIRST refusal of that member all session — even though the
skill tree had opened fine three times earlier (01:24:02, :11, :36) and read the orbs correctly (the
member is real: these are the same 12 orbs verified in-game 2026-07-14, row below). ROOT CAUSE, and it
generalizes: `ui_core`'s `prop_set` super-struct walk could stop EARLY and SILENTLY — typically at the
Blueprint generated class WITHOUT its native base — and the truncated set was then cached as AUTHORITY
for the rest of the session (flushed only on map transition), so every gated read of an INHERITED
member was refused forever behind one quiet log line. FIX SHIPPED, **SOURCE-ONLY / UNVERIFIED IN
GAME, 31 files, +682/-299 — needs a full RESTART** (`ui_core.lua` changed): a walk that does not reach
the hierarchy root is marked PARTIAL; a name ABSENT from a partial set now means "don't know" and
FAILS OPEN (a name PRESENT still carries a trustworthy type); `strict` multi-candidate gates still
fail CLOSED — opposite contract, candidates there are expected absent. Re-derived up to 3× per class,
5 s apart, logged once. NEAR-MISS caught by adversarial review before it ran: the first cut treated
`GetSuperStruct() == nil` as "reached the root" — wrong, UE4SS answers an INVALID handle there, not
nil, which would have marked EVERY class partial and switched both existence gates off mod-wide; the
shipped discriminator is the hierarchy root class being named `Object`. Full writeup: the 2026-07-29
entry in the [crash ledger](reference/dbz-kakarot/notes/dbz-kakarot-crash-bug.md). **STILL OPEN:
whether the mod actually killed the process.** The log alone cannot prove it — no traceback, just
silence — and the decisive evidence is the mod's own crash-trail black box (`mem.lua Mem.mark` ring
→ `crash_trail.bin`, printed at the next boot). Asked the user for the head of the next session's log;
do not close this out until that lands. Previous entry: 2026-07-28 (l) — **six more screens leave the scan set (screen directory sweep).**
Prompted by a "could we use the Sparking Zero menu approach here" question, which the CXX dump
answered NO (see `notes/sparkingzero-reference-scope.md`: `UCFUIUserWidget` has 428 subclasses and
declares ONE member, `UATUIUserWidget` 131 and one function — there is no uniform focus property to
build a generic reader on, so the per-screen adapters are forced by the class layout, not by us).
The half that DOES port is the directory, and a sweep of every still-scanning host against AT.hpp
found six with a real, unused owner field: `Start_Quest_C` (mm.m_xQuestMenu.m_UIStartQuest —
screen_story's own comment already complained about being an ABSENT class),
`AT_UIStartDragonBallMenu`, `Xcmn_Win00_C` + `Xcmn_Win02_C` (the two `UAT_UIWindowManager` cores
sitting NEXT TO the already-mapped `GameWindowCore`), `Gametitle_C`, and `AT_UIXcmnAgreement` (two
chains). **`Gametitle_C`'s "deliberately unmapped" note had EXPIRED** — it said "no HUD yet", written
before the `tt` root existed; that root is the very actor declaring the pointer. Four reachable
hosts were deliberately NOT mapped and the reasons are in-file: the two dialogue surfaces are
pooled multi-instance (mapping them re-opens the 07-06 "narrator lines unread" bug — the
multi-instance rule outranks the pointer), `Quest_Sub_Reward_C` is a freshly-fixed screen not yet
verified in game, and `Map_World_Curs_C` needs its raw `FindAllOf` call site moved to Core first.
VERIFIED IN GAME the same day (user: "se lee todo") — none of the six hit the documented failure
mode (owner reachable + field never set = asserted absent = silent screen), so the mapping stands
and those classes no longer pay a FindAllOf per ABSENT_BACKOFF. Previous entry: 2026-07-28 (k) — **shops announce the Zeni balance; map remembers the travel
point.** (1) No adapter read the wallet at all. Every shop embeds the same `UAT_UIShopCmnMoney`
sub-widget (AT.hpp:35622, figure in `WL_Txt_Num_Money`) but under a DIFFERENT member per host
(`Shop_Cmn_Money` / `WL_Shop_Cmn_Money` / `WL_CmnMoney`), and `UAT_UIShopCommon` skips the
sub-widget and inlines its own — hence one shared `A.shop_money(host)` trying each name through the
existence gate. Wired into `screen_shop`, `screen_shopcmn` and `screen_shopinfo` in the announcer's
VALUE slot, which is what makes it re-speak on its own the moment the balance changes, i.e. on
every purchase and sale. New key `shop_money_fmt`. (2) The world-map d-pad selection now survives
`reset()`, remembered BY NAME (`ft_last_name`) rather than by index: pressing A opens the travel
prompt, which is a different adapter, so the registry reset used to drop you back at the top of the
list when you declined. Restored silently; a name absent from the rebuilt list restores nothing, so
it self-expires across maps with no explicit clearing. Previous entry: 2026-07-28 (j) — **map fast-travel d-pad, round 3 (the intermittency).** An opus
investigation found the fast loop was hostage to the slow one in three independent ways, all fixed:
(1) **self-sustaining quiet deadlock** — quiet mode deferred the `Map_World_Icon_C` scan, but
screen_map only commits once it sees those icons, and with no committed adapter the quiet
heuristic stayed satisfied (an open world map looks exactly like a camera cutscene to it). Broken
only by the player pressing a face button, hence "sometimes". Added to `QUIET_EXEMPT`. (2) **the
pad loop was stealing the scan budget** — `ft_host()` used `Core.cached_all` on a 20 ms loop that
never calls `begin_scan_tick`, so it drained the budget 5× faster than the registry refills it,
starving the icon scan it was waiting for; now `Core.peek_all` (never scans). (3) **presses were
swallowed during warm-up** — `ft_prevbtn` was committed at the END of the handler, after the "no
list yet" early return, so every press made while the list was building vanished; edges are now
computed and committed at the top, and a direction pressed too early is LATCHED and replayed. Plus
`ft_guidance` now builds the destination list ITSELF (wall-clock throttled, 250 ms) instead of
waiting on the 100 ms poll. KNOWN RESIDUAL, not fixed: a PARTIAL build still latches — if the
native InfoIcon block is populated while the icon pool is stale, every entry degrades to "map point
N" and no later build replaces it (`ft_build` keeps whatever it got). Previous entry: 2026-07-28 (i) — **the mute Recovery tab was a WRONG DERIVED FACT, not a bug.**
The debug capture settled it: `0x620` is the category TAB INDEX (values 0-4, always equal to
`0x624`), never a has-items flag, so the first tab always read "empty". Corrected in *Derived
facts*; the native read is now only a hint and is corroborated against the detail-pane name.
**Lesson worth keeping: a derived fact confirmed on a single sample can encode the sample instead
of the field** — 0-vs-nonzero looked like a boolean because the one category checked in the 07-11
F4 session happened to be both first AND empty. Also fixed: the map's on-demand objective re-read
fired from `reset()`, which runs on EVERY screen change, so opening/closing the WORLD map
re-announced the objective repeatedly — now area-map only and rate-limited to once per 20 s.
STILL OPEN: the fast-travel d-pad is usable but takes a long time to become responsive after the
map opens; the per-tick cost is fixed, so the remaining latency is the `ft_points` build waiting on
the budget-gated `Map_World_Icon_C` re-scan — not yet measured, do not guess at it. Previous entry: 2026-07-28 (h) — **map fast-travel d-pad: slow, and dead from the second open.**
Both bugs were in one function, `screen_map.lua:ft_host()`. (1) It picked the first `Core.valid`
instance with a Transient path and never checked `on_screen` — but these hosts are POOLED and
multi-instance, and a closed pooled widget stays valid forever, so the second open returned the
PREVIOUS off-screen instance and every native `selIndex` write went to a host the confirm core is
not reading. Textbook case of the standing "invalidate by `on_screen`, never by validity alone"
rule, and the works-once-then-dead intermittency is its signature. (2) It runs on the 20 ms pad
loop and walked every pooled instance calling `GetFullName()` on each — a reflection call per
instance per tick, growing as instances accumulate through a session: that was the sluggish d-pad.
Now memoised (with `Core.valid` still on every use — that guard is never skipped — and the costlier
`on_screen` re-check throttled to 250 ms) and cleared in `reset()`, so each map entry resolves
afresh. Also cleared `ft_kb_cmd` in `reset()`: an unconsumed keyboard command could survive a visit
and fire on the next one. Previous entry: 2026-07-28 (g) — **performance regression from the two new menu adapters, fixed.**
User reported small stutters. Cause is documented in `ui_core` itself: every class an adapter names
joins the ABSENT scan set and costs a full `FindAllOf` every ~4 s forever, and the comment beside
`SCANS_PER_TICK` records that a cluster of those expiring on one tick IS the periodic-stutter
symptom. `screen_compz` + `screen_story` had added **eleven** class names to a set the code keeps
deliberately small. Fixed two ways: dropped the speculative native fallbacks (never once observed —
the census names every live host as a Blueprint `_C`), 11 names → 6; and both adapters now bail out
of `is_active` on `Core.free_roam(tick)` BEFORE any scan, so they cost nothing while the player is
in the field, which is where the stutter was felt. Note the radar change of the same day adds only
a few guarded member reads per in-use navi icon per scan and is not a plausible cause on its own;
if stutters persist, `_G.__KakarotScanStats` / Ctrl+F5 gives per-class attribution. Previous entry: 2026-07-28 (f) — **four fixes, each settled by a measurement rather than a
guess.** (1) **GAME OVER, finally diagnosed.** The probe caught the menu genuinely on screen at
last: `on_screen=true IsVisible=true enum=4 opacity=1.0 inVP=true pane_live=FALSE`. Enum 4 is
SelfHitTestInvisible and `pane_live` demands Visible(0), so the adapter never once claimed —
`screen -> screen_gameover` appears in no log ever. **Third screen this same gate has silenced**
(after fishresult and questreward). Gate is now `on_screen` (rejects the parked, Collapsed host) +
`pane_rendered`. The file's old header claim that `on_screen` rejected the live host is refuted by
the same sample and is kept, corrected, in place. (2) **Training screen never said the required
level**: `Txt_Recommend` is only the LABEL ("Nvl. recomendado:"), the figure lives in `Txt_Num_Lv`,
and only the label was read — buried in the tooltip. Now a proper pair spoken with the skill; also
fixed a raw `host[m]` fetch there. (3) **The item menu's RECOVERY tab was silent** because
`screen_itemuse` claimed it forever: it returned active on any rendered party bar, and that tab
renders the whole party strip permanently. Now requires a UNIQUE on-screen bar AND readable text.
(4) **Pressing X on an item re-announced everything continuously**: `screen_palette.reset()` nils
its node cache while the re-collect is throttled, so a reset inside that window left it nil and
`is_active` indexed nil — 23 palette/list flips in 10 s in the log. Previous entry: 2026-07-28 (e) — **user-VERIFIED this session:** the Party roster + Characters
lists, the Story menu (entries, summaries and the X progress checklist) and the whole Z
Encyclopedia (index, category lists, entry sheets, note slips). Still UNVERIFIED and awaiting a
report: the radar picker regression fix (R3 / V) and the substory reward sheet — neither was
explicitly retested, so do not read this entry as covering them. Not merged to `main`: the user
reports something still to fix. Previous entry: 2026-07-28 (d) — **two new screens: the Story menu and the Z Encyclopedia.** User
confirmed the character lists and asked for both, with dumps and screenshots. Neither needed a
capture round beyond the ones supplied, and neither uses a guessed offset. `screen_story.lua`
sidesteps the missing cursor index entirely by reading the DETAIL PANE, which mirrors the
selection. `screen_compz.lua` covers all three book levels in one adapter and is the first list
screen here that needed no guesswork at all: `UAT_UICompZList` declares its own `Canvas_Cursor`
@0x3A0, and the rows live in a REAL `TArray` (`UAT_UICompZListController.Item_List` @0x0030), so
both the enumeration and the selection are plain reflected reads. Also shared out of three private
copies: `Core.first_text` / `Core.first_member` (try candidate member names through the existence
gate — most candidates are expected NOT to exist, which is exactly why they must not be raw
fetches). **Adversarially reviewed before shipping; six findings, all applied.** The critical one
generalizes: those very helpers turned fail-OPEN into a hazard, because `Core.member` falls back to
a raw fetch whenever the property set is unavailable (budget is 1 set/tick shared by ~40 adapters,
and a class that introspects to nothing is marked un-gateable permanently) — fine for one
believed-good name, a licence to fetch known-absent names for a multi-candidate probe. Hence
`Core.member(o, name, strict)`: skip the candidate rather than fetch it, bounded to one tick of
silence per new class and self-healing, with the permanent case logged. Also applied: Story's
rewards now read the header-verified `UAT_UIStartQuest.UIRewardBar_List` @0x6D0 TArray instead of
six Blueprint node names; both adapters gate their pooled hosts on `Core.pane_rendered`; compz
claims the screen on readable TEXT rather than on a live handle (a textless page would have
shadowed save/load, tutorials, community and field); both heading/tab edges got a 2-poll settle
plus a log line (the screen_party flip-flop shape); and `m_Detail` is documented as a fixed array
of 3 that collapses to element 0, so the Blueprint name goes first there. SOURCE-ONLY, UNVERIFIED
IN GAME. Previous entry: 2026-07-28 (c) — **the substory reward sheet had never run, and the reason was a
gate we had already been burned by.** User verified the character lists read correctly, then
reported the "¡FELICITACIONES! / Recompensas de historia" sheet silent. `screen_questreward` was
registered and its widget map was correct — but it gated on `Core.pane_live`, which requires
`GetVisibility() == Visible(0)`, and a passive overlay in this game renders HitTestInvisible. So
the host was rejected on every tick and `screen -> screen_questreward` never appeared in the log
ONCE, while the F7 census taken with the sheet on screen showed the title and all four reward rows
rendered at full opacity. `screen_fishresult` hit this identical wall on 2026-07-17 and solved it
with a PRIVATE 4-line helper, so the knowledge never reached the next adapter — the opacity-only
test is now `Core.pane_rendered` in the substrate and both callers use it. **Rule added: the
liveness gate for a passive NOTICE is not the one for an interactive PANE.** Open, not started:
the substory briefing card `Quest_Sub_C` (Dmy_Title / Txt_Recommend / Txt_Detail / List_Parts00..03)
is on screen at the same time and has no adapter. Previous entry: 2026-07-28 (b) — **round 2 on the two character lists; the user's F7 census killed
the whole `RegisterCustomProperty` mechanism.** Two symptoms, one root each, and an adversarial
review of the round-1 diff predicted BOTH before the retest was analysed. (1) PARTY, "el anuncio se
solapa porque se lee nombre de personaje": `CharBar.marked` tested the cursor markers with
`Core.is_visible`, which reports a widget's OWN slate visibility — a row in an unfocused panel keeps
reporting itself visible, so the roster looked focused permanently and the side flipped one tick
after every left-column move, speaking a stale roster row over the slot. Fixed with `Core.on_screen`
(ancestor walk) plus a WALL-CLOCK settle (`SIDE_SETTLE_S` = 0.30 s) on the panel change, bypassed
only by a left-cursor move (a real user action), and one log line per accepted change. (2)
CHARACTERS, "no se lee el personaje al navegar": the list read only its FIRST row. Cause is
generalizable and now a rule — **`RegisterCustomProperty` + `Core.allow_member` is a trap**:
`allow_member` whitelists a NAME, so a registration that silently failed left the name past
`Core.member`'s existence gate (protection → permission, i.e. the uncatchable abort), and
`RegisterCustomProperty` resolves `BelongsToClass` ONCE and matches by raw `UClass*` — BP classes
unload on a map transition and their addresses get reused, and `custom_props` is never flushed
(unlike `prop_sets`), so after the first map load every slot but 0 stops resolving. **The mechanism
is withdrawn entirely, not hardened**: the F7 census showed these rows are Blueprint WidgetTree
children (`Start_Party_C_0.WidgetTree_0.Start_Char_Bar00..03`,
`Start_Char_C_1.WidgetTree_0.Start_Char_Bar00..05`, and the left column's
`Start_Party_List.{Start_Party_Bar00,Start_Party_Spo00,Start_Party_Spo01}`), so the engine already
exposes each row as its own reflected property BY NAME. No offsets, no whitelist, nothing to
invalidate on a transition, and an undeclared name is refused by the gate instead of aborting.
Characters selection also flipped to INDEX-FIRST (`GetCursorIndex`/`GetViewIndex`, reflected
UFunctions, deterministic) with the markers demoted to fallback. SOURCE-ONLY, UNVERIFIED; needs a
restart (new shared module). Previous entry: 2026-07-28 — **crashes look fixed (user), but the crash audit shipped one
REGRESSION that killed the radar picker outright, and the Party screen's character list got its
first reader.** (1) RADAR: R3 and V both did nothing. Not an input problem — the log named it 16
times. The 07-27 result-validation added to `Core.member` sent EVERY typed property to
`Core.valid_ref`, on the assumption that `IsValid` is universal on RemoteObjects. It is not: an
FName is a VALUE with no `IsValid`, the call resolves against the FName CONSTRUCTOR
(`No overload found for function 'FName'`) and that error PIERCES pcall — so `Nav.list_targets`
died on the FIRST NPC it looked at, `do_open()` never got a list, and the picker never opened.
`UniqueId` is a NameProperty and it is the first thing the target list reads about an NPC, which
is why the blast radius was the whole feature. Fix: the result gate now checks ONLY
`ArrayProperty`/`StructProperty` (the two handle-shaped types the two-tier rule was actually
invented for); every other type is a value that cannot dangle, so it fails OPEN. Plus a new
`Core.name_str` (convert, never validate) replacing the five direct `valid_ref`-on-an-FName calls
in `nav_tracker`. **Generalizable rule, now in the crash ledger: a guard that widens its own scope
by assumption is a guard that will take out a working feature.** (2) PARTY: the right-hand roster
(`WL_Start_Char_Bar` @0x3C0, 4 × `UAT_UIStartCharBar`) was never read, so confirming Player or
Support opened a silent list. Now read via the new shared `ui_charbar.lua` — the same row class the
Characters menu lists, so registration/cursor-marker/text logic is one copy instead of two, and
`screen_characters` was migrated onto it. Panel focus comes from the cursor MARKER (the hierarchy
stores no focus flag), with a left-cursor-moved tiebreak so the verified slot column can't be
silenced. SOURCE-ONLY, UNVERIFIED IN GAME; needs a full RESTART (nav_tracker + i18n + new module).
Previous entry: 2026-07-27 — **full-codebase crash audit (multi-agent), 11 holes closed, SOURCE-ONLY
and UNVERIFIED IN GAME.** Not crash-driven: a systematic sweep of all 71 Lua files + the 4 native
bridges against the ten accumulated crash mechanisms. 48 candidates → each adversarially verified by
an independent refuter → **37 died** (debug-flagged off, or genuinely guarded upstream), 11 real.
Five root causes, all now documented in the [crash ledger](reference/dbz-kakarot/notes/dbz-kakarot-crash-bug.md):
(1) **asymmetric guards** — `Core.array_of` refused a wrong-TYPED member but not an UNDECLARED one,
so every ordinary dialogue window fetched a non-existent member every tick; gate now identical to
`Core.member`'s, fail-open, `custom_props`-aware. (2) **a field battle is not a `Transition`** — it
closes the world gate without `LoadMap`, so `enemy_cache`/`navi_icons`/`chain_wait` survived it; a
fight shorter than ~10 s served just-destroyed actors, which IS the 07-26 black box. Dropped on the
gate's falling edge too, to `nil` not `{}`. (3) `target.actor`'s 5 s grace was the same defect with a
timer — handle released at the gate, metadata kept (which is all `remember_pick()` ever needed).
(4) **UE4SS runs keybinds on its own thread**: F1 walked pooled widgets on the poll loop's
`lua_State`; wrapped (only 1 of ~19 handlers was wrong — swept and confirmed). (5) diagnostics drift:
9 bare `IsValid()` left in `discover.lua` after the 07-25 sweep, and `Nav.dump_levels` shipped with
neither validation nor gate while its sibling `Nav.dump` had both. Plus raw two-hop `host.A.B` chains
migrated to `Core.member` in `screen_choice`/`screen_training`/`screen_itemuse` — the inner hop is
evaluated at the CALL SITE, outside the `pcall` two comments claimed was protecting it (corrected).
**Native: 8 findings, all 8 confirmed, none refuted** — `audio_bridge`'s RIFF bounds check overflowed
in 32-bit (4 GB `memcpy` from any truncated WAV), `mem_bridge`'s `WRITE_FN` was an unvalidated
arbitrary-write primitive (now bounded by a DERIVED `MAX_WRITE_OFFSET`, plus an `expect_class`
assertion in `mem.lua` since a bound stops a wild offset but not a wrong-object write),
`l_mark_open` could longjmp past its own cleanup (in the crash recorder), and `input_bridge`'s pad
block was a latch with no lease (a Lua unwind left the gamepad dead until the game was killed).
All 4 DLLs rebuilt clean; `tools/lint-lua.ps1` exits 0 over 71 files. **Needs a full RESTART.**
Previous entry: 2026-07-26 — why crashes persisted on v0.1.2 while the dev `UE4SS.log` stayed
clean: **a clean log was read as "no crashes", but the log only records what the mod PRINTED** — the
dev machine had been crashing all along (647 s, 1018 s, 1792 s, 7912 s runtimes, one mid-COMBAT on
07-25) and the evidence sat unread in 29 crash dumps. Cause: every guard in the crash ledger had
only ever been applied to the MENU substrate. `nav_tracker.lua` — every tick, in free roam AND
combat, over actors that streaming frees and that combat destroys — still held 24 bare `IsValid()`
(which *faults* on a freed handle rather than rejecting it) and ~95 naked member fetches, including
on a `target.actor` held for minutes. Swept, plus three
substrate holes (`Core.pane_live` unguarded, `ui_directory.prop()` had no property gate,
`begin_scan_tick`'s budget was refilled by six callers so "2 scans/tick" was really a dozen — which
also explains the old unaccountable 31%-of-game-thread `FindAllOf` measurement). Menu slowness was
partly self-inflicted: the 07-25 crash fix made `Core.valid` ~3× costlier on per-tick full-pool
walks; now memoised per tick. **Also: 29 real crash dumps were sitting unopened in
`%LOCALAPPDATA%\AT\Saved\Crashes\`** — 26 of 29 died inside `UE4SS.dll`, and the callstacks name the
uncatchable C++ throw outright (`RaiseException ← _CxxThrowException`). Fingerprint table in the
ledger. New `tools/lint-lua.ps1` (syntax + globals + guards) is a hard gate in `package.ps1`.
SOURCE-ONLY, UNVERIFIED IN GAME, needs a full restart. Previous entry: 2026-07-25 (c) — the user's
retest found 7 screens silent; the UE4SS.log settled
all 7. Six were ONE bug: `GetAddress()` on the TArray wrapper inside `Core.array_of` raises
"polymorphic type is not allowed", an error that **pierces pcall** and kills the adapter's function
mid-flight (510 tracebacks naming exactly those screens) → fixed with `Core.valid_ref` (IsValid
only) for array/struct handles. The seventh (Options on re-entry) was a hand-rolled `rows` cache
invalidated by validity instead of `on_screen`. Both fixes source-only, unverified in game. Earlier
the same day — crash ROOT CAUSE settled from the UE4SS source (`IsValid()`
dereferences before it checks, so it faults on the handles it exists to reject) → new SEH-guarded
`Mem.alive()` pre-check runs before UE4SS ever touches an object; plus the three reading
regressions the 07-24 hardening introduced (load-game screen, Options save dialogs, and an
always-on probe of the defeat menu). Source-only, luac-validated, needs a full restart to test —
see *Next step*. Previous entry: 2026-07-24 (end-user crash + stale-read batch, source-only, UNVERIFIED in game). CRASH: cooking-browse AV = the 07-21 dangling-UObject `__index` class in steady-state menu browsing (pooled ListView/detail recycled on scroll); migrated screen_cooking + shared `A.list_selected_row` to `Core.member`, then swept EVERY adapter and migrated 18 more files with the same naked-fetch-as-call-argument pattern (all luac). UI fixes (round 2 after user retest): (1) quest objective — SETTLE debounce (2 stable polls) so progressive HUD fill / pooled-instance flicker no longer speaks it "several times" or re-narrates on map close; map re-read DEFERRED + queued in `screen_map` so it no longer overlaps the map's area/help readout. (2) TOAST — presence dedup with a ~1.5 s GRACE (Info_Log02 is a flashing banner whose blink re-armed the dedup). (3) the persisting "desbloqueaste superataque" was actually `screen_dialog`'s post-confirm window re-firing once the 24-entry recent-set FIFO evicted its marks — added a `pinned_set` immune to FIFO eviction, cleared only on map transition. NEW: **defeat menu** (`screen_gameover.lua`, Gameover_C → UAT_UIGameover) — was silent (no adapter); reads "Fin de la partida" + the selected row (Reintentar/Cargar/Volver al título) via CurrentSelectIndex (reflected, mem_bridge @0x3E0 fallback).

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

**Next step (dev loop):** create `UE4SS-settings.dev.ini`; smoke-test the MCP against a running game
(`kak_alive` → `kak_screen` → `kak_census` on one open menu).

## Section status
`done` = works with the screen reader on; `wip` = started; `todo` = not begun.

| Section / feature | Status | Notes |
|---|---|---|
| Speech pipeline (PRISM) | done | Logs chosen backend on boot |
| Overworld main menu (native selection) | done | `screen_field.lua`, reads via `UAT_UIStartTop` offsets |
| Battle-pause menu (native selection) | done | `screen_pause.lua`, `UAT_UIXCmnPause +0x43C` |
| Defeat / game-over menu | wip | **2026-07-25 round 3.** Directory-mapped now (`fm.Gameover`, owner field found by the F7 probe) so detection is a pointer read. The probe FIRED in the user's log but at the wrong moment: it was one-shot, so it sampled at 12:28 — minutes before any defeat — and reported the PARKED state (`valid=true on_screen=false IsVisible=false enum=1 opacity=1.0 inViewport=true`). Two facts still worth having from it: `inViewport=true`, so `IsInViewport` is NOT what rejects this host, and `on_screen=false` while `enum=1:Collapsed` — consistent with the parked state, so it says nothing about the open state. Probe now logs on signal CHANGE (and includes `pane_live`), so the next defeat prints the state that matters. |
| Defeat / game-over menu — earlier rounds | wip | `screen_gameover.lua` (NEW 2026-07-24) — Gameover_C → UAT_UIGameover. Reads "Fin de la partida" + selected row (Reintentar/Cargar/Volver al título). Index = CurrentSelectIndex (reflected first, mem_bridge @0x3E0 fallback — ObjectDump wasn't regenerated to confirm reflection); rows via SelectionWidgetArray / positional List_Bar0N. Pending in-game verify (needs game RESTART: new adapter + native_offsets) |
| Dialog / message / confirm popups | done | `screen_dialog.lua`. VERIFIED 2026-07-16 night after the 21-round saga: per-NODE composition (rendered state via node_rt first, recent-set novelty second, help requires rendered title), row-filtered content folds on titled notices, latched per-appearance choice prompts, transition-epoch clear, emblem reward via the window's own WL_TextCmuCtn. See the Next step closure + git log for the derivation. 2026-07-16 night: verbatim-repeat rewards (Milk emblem) rescued by fresh CONTENT ROWS within ~3s of the appearance edge — pending verify |
| Tutorial guidance line | done | `guide_watch.lua` — the pinned instruction box, read from the RICH side (`Txt_Detail/Txt_Help/Txt_Work` ExMainTxt; plain keeps stale text), every registry tick, F1 repeats. VERIFIED 2026-07-16 night ("oprime Confirmar para…" — abstract Decide glyph spoken as the action) |
| NPC subtitles / dialogue — FOUR surfaces | done | **2026-07-25: the missing one was `Field_Navi_Win_C`** (`UAT_UIFieldNaviWin`, AT.hpp:33020, pointer `fm.FieldNaviWin` @0x550, now directory-mapped) — the PORTRAIT pop-up a character speaks through when not physically present. The player described it as Krillin being "on the phone"; there is no phone/Denwa/Keitai class in this game, this is it (`Image_Char` is the portrait). Nodes: `Txt_Speaker`/`Txt_Msg` in the BP tree, `TextBox_Name` @0x400 / `TextBox_Message` @0x408 natively — both tried. PROVEN, not guessed: three F7 dumps 15 s apart showed its `Txt_Msg` advancing through the conversation ("Ya sé que quieres relajarte…" → "Así que quería recordarte que sigas entrenando." → "Te mantuviste firme durante el combate…") while `Txt_Speaker` read "Krillin", and the talk window sat on a STALE "¿Krillin? ¿Qué sucede?" for two of the three. Order tried: subtitles → talk window → navi window → bubbles. **It took three wrong guesses before asking for the dump; the dump answered in one pass.** Also added: the subtitle surface now tries the native class `ATUISubtitles` with native names, since searching only the BP class `Xcmn_Subtitles_C` would miss an instance of any other subclass. |
| NPC subtitles / dialogue — earlier rounds | done | `screen_dialogue.lua`. 2026-07-15: `Xcmn_Subtitles_C` gated on the game's OWN subtitles option (FAIL-OPEN if unreadable); `Field_Talk_Win_C` (dialogue box) never gated. 2026-07-16 night: the gate read a DEAD TEMPLATE — the object array holds several ATSaveSystem instances (_0/_1 pristine EnableSubtitle=1, _4 = the user's real settings) and first-non-CDO picked _0, so it always read 1. Now resolved through the game's own pointer `UATSaveManager.SaveSystem` @0x108 (AT.hpp:29391), manager cached, pointer re-read per query. Pending verify with the option off |
| Boot agreement viewer (EULA / privacy / data analysis) | done | `screen_agreement.lua` — `AT_UIXcmnAgreement` (AT.hpp:37902): ONE viewer cycles the consent documents; every page is a TEXTURE, no readable text exists (two F7 censuses), so it announces a localized screen label + "shown as an image" note; the actions bar comes from the automatic keyhelp read. DOC + PAGE named NATIVELY (2026-07-17 Ghidra, `native_offsets.agreement`): docId @0x5B4 → "Política de privacidad"/"Análisis de datos"/"Acuerdo de licencia", "página N de M" from @0x5A8/@0x5B0, sanity-checked, silent-degrade if a patch moves them. The earlier brush-texture heuristic is REMOVED (piercing nullptr — see Derived facts). Detection = pointer reads: cached `Gametitle_C` → `ActorTitle` → `AgreementDialog` @0x340, fallback budgeted scan; `pane_live`-gated. Registered below screen_dialog — the Accept/Reject consent popup is a pooled `Xcmn_Win01` and belongs to the dialog reader. VERIFIED in-game 2026-07-17 (user): entry, native document naming ("Política de privacidad" / "Análisis de datos") and page N-of-M readout |
| Difficulty / choice lists | done | `screen_choicelist.lua`, `screen_choice.lua` |
| Notices no longer cut off by the screen underneath | done | **VERIFIED in game 2026-07-25 (user).** `Announcer:focus` + `keyhelp_watch` defer while `Speech.protected()` — the shared fix for "the map/skill-tree readout talks over a popup". The offender is never the notice's own screen, so this belongs in the substrate, not per adapter |
| Dialog re-trigger reads again | done (unverified) | 2026-07-25. `recent_set`/`pinned_set` are now scoped to the window PRESENCE (cleared after `CLOSED_CLEAR_S` = 0.7 s of continuous absence), not to the map epoch. Suppression was keyed per TEXT while every "new open" signal was per APPEARANCE |
| Keyboard: radar picker + key names + INPUT BLOCK | done (unverified) | 2026-07-25. `radar_menu.Menu.key(cmd)` (queued, consumed on the game thread; `kb_open` so "no pad" ≠ "pad lost") + keybinds Ctrl+F3 / PageUp-PageDown / arrows / Enter — **needs a game RESTART** (main.lua + a rebuilt DLL). Button-config keyboard tabs speak the literal key via `A.action_key` + `I18n.key`. The mount/dismount conflict on ↑/↓ is GONE: `input_bridge.kb_block(ms)` now hides the keyboard from the game exactly like the pad block does |
| Controller remap — LIVE, before "Save changes" | done | **VERIFIED in game 2026-07-25 (user).** Reported as "solo se actualizan al guardar". The pending layout is **not a member of any object** — a process-global `TMap<FName,FName>` at exe base + **0x569C3B0**, proven by Ghidra: the option screen's save case calls `FUN_1419e48a0(saveSystem+0x720, &DAT_14569c3b0)`, i.e. it BUILDS the saved struct from that map, writing exactly the 12 slots at 0x180..0x1D8 and skipping `Keyboard_Type`. The same map is what the game's OWN glyph resolver reads (`FUN_141a66300`), which is why the icon changes instantly — so reading it is reading what is on screen. Reader walks the element array comparing FName COMPARISON INDICES (no string conversion), and refuses unless all 12 slots are present and their values form a PERMUTATION of the 12 physical-key FNames → a moved global degrades to the saved copy. Offsets + evidence in `native_offsets.inputAssignMap` |
| Controller REMAP honoured everywhere | done (unverified) | 2026-07-25. Every controller id the game exposes names a **SLOT**, not a button, and a slot keeps its factory name forever — so we were announcing the pre-remap button (user: melee B→X, ki X→B, both still read as B/X). F7 dump proved it in one pass: save `Controller_Btn_B = Gamepad_FaceButton_Left` / `Controller_Btn_X = Gamepad_FaceButton_Right` while the asset still pairs melee with slot `Controller_Btn_B`; the asset is NOT rewritten on rebind (its only live-vs-default delta is `ctrl` being filled in at runtime), so cache-clearing there could never have fixed it. New slot→physical layer in `ui_archetypes` reading `UATSaveSystem.InputAssign` via `gi.SaveManager→SaveSystem` (0.5 s TTL, fail-open to the slot name), applied in the single funnel `A.button_name` plus the three raw-token paths (`platbtn_token` ×2, `platbtn_id_token`'s idxToCtrl fallback). Identity under a default config, so it can only change a remapped pad. Fixes Options rows AND every `<inputicon>` prompt, keyhelp and tutorial glyph |
| Options / System / Title / Tutorials / Tips | done | `screen_options/title/tutorials/tutorial/tips.lua`. Title 2026-07-17: SETTLE GATE — the boot-check dialogs re-commit the title on every gap and each re-commit re-announced "Menú principal, continuar"; the title now stays silent until it holds the screen 2.5 s with no dispatcher reset (no reflected boot-phase field exists on AAT_Title/AATTitleLevelScriptActor — swept), a cursor move lifts the gate at once, F1 bypasses it via `reannounce()`. Verified in-game 2026-07-17 (announced once, after the boot dialogs; 4 s felt sluggish → tuned to 2.5 s) |
| Shops (food/material/info) + item palette | done | `screen_shop*.lua`, `screen_palette.lua` (verified in-game) |
| Party menu — character list (right panel) | done | **VERIFIED in game 2026-07-28 (user)**, together with the Characters list, after round 2 fixed the panel flip-flop and the one-row-only bug. **NEW 2026-07-28** (user: "al entrar a los submenús de jugador o apoyo no me lee la lista de personajes"). The half of the Party screen where you actually pick someone had no reader: `screen_party.lua` covered only the LEFT slot column and its own header said the right panel was "not read here". It is `UAT_UIStartParty.WL_Start_Char_Bar` @0x3C0, declared size 0x20 → a **4-slot fixed C array** of `UAT_UIStartCharBar` (AT.hpp:36660) — the SAME row class the Characters menu lists, so the fixed-array collapse workaround, the cursor markers (`Pnl_Curs_All` @0x410 / `Img_Curs00` @0x438) and the text nodes (`Txt_Name`/`Txt_Name_Guest`/`Txt_None`/`Txt_Lv`/`Txt_Power(_Num)`) are now ONE copy in the new `ui_charbar.lua`, with `screen_characters.lua` migrated onto it. WHICH PANEL HAS FOCUS is not stored anywhere: the entire hierarchy exposes exactly one small integer (`partySelectData.cursorIndex` @0x420, and it belongs to the left column — the rest of that 0xB0 struct is two TMaps of icon textures), because the game drives the panels with `Focus()`/`LoseFocus()` on the bars instead. So focus is inferred from the cursor MARKER, with a tiebreak: if the left cursorIndex moved this tick the left column wins regardless, so a stale highlight can never silence the verified slot column. Panel switch re-announces through the announcer's screen-entry branch (its `tab` parameter speaks the label alone and would swallow the first row). Roster placeholders (`Txt_None` = empty / "Remove") count as selectable rows here, unlike on the Characters screen. New i18n key `party_charlist` |
| Items inventory + Party + Characters | done | Party/Characters done. Items list reads populated categories (via `Txt_Title00` detail-pane live name; reflected index tracks). EMPTY categories: the whole item UI goes STALE (row/detail/visible-count keep the last item). **CORRECTION 2026-07-28: `itemMenu.hasItems = 0x620` is NOT a has-items flag — it is the category TAB INDEX.** The 07-28 capture shows it taking the values 0,1,2,3,4 (one per tab) and always equal to 0x624, so it reads 0 on the FIRST tab whatever that tab holds; the item menu's first tab is Recovery, which therefore announced "empty" once and then went mute for the whole visit while the row and detail names were reading fine. The 2026-07-11 F4 session only ever confirmed it against a tab that happened to be both first and empty. The read survives as a HINT (on a genuinely empty category the UI really does go stale, and nothing fresher exists), but it is now corroborated: a category with a readable detail-pane name is not empty, whatever the byte says and announced ("vacío") via `screen_list.lua` factory `empty_off` param. Verified in-game 2026-07-11 |
| Item submenu (use-item char select) | done | `screen_itemuse.lua` — A on a usable item → pick who uses it. Reads the on-screen `AT_UIItemMenu.WL_Start_Party_Bars` bar (the selected char; only it animates in): `Txt_Name01` + `Txt_Lv01→Txt_Lv02` level-up preview, with the "choose character" prompt. Registered before the item list reader. Verified in-game 2026-07-11 |
| Save / Load data slots | done | `screen_saveload.lua` — `AT_UIStartSaveLoad`. VIRTUALIZED 3-bar window (`UISaveLoadBar_List`), so pool-position ≠ ordinal. Ordinal from native index `saveLoad.selectedIndex = 0x410` (+1), cursor bar = `windowPos = 0x418` (F4-confirmed over ~11 saves); reads FILLED and EMPTY slots (Canvas_None checked first); SETTLE_TICKS debounce drops mid-scroll frames. Slow re-entry (widget destroyed+recreated → stale class-list cache) FIXED by `ui_core.first_on_screen` churn-force (re-scan a recently-on-screen class immediately, budget-gated). Verified in-game 2026-07-11 (reads all slots, correct index, fast entry, no lag). DEBUG off |
| Character status page (stats sheet) | wip | `screen_status.lua` — `UAT_UIStartStatus`, the sheet you get by confirming a character in Personajes. ALL reflected: `WL_Txt_Name/_Lv/_Title/_Num/_Power/_Power_Num`, the two `UAT_UIStartStatusHud` gauges (`HpGauge`/`SpGauge` → `TextBox_Number`) with their `TxtStatus`/`TxtStatusNum` row TArrays, and the five `UAT_UIStartStatusList01` attribute blocks (`WL_Txt_Power(_Num)` = total, `TxtStatusList`/`TxtStatusNumList` = rows). `WL_Start_Char_List01` is a fixed C array → UE4SS COLLAPSES it (never index it — the `WL_StartCharBarList` lesson), so the blocks come from the class's cached instance list, on-screen ones only, ordered by `Core.slot_pos(w,"Y")`. Entry reads the header (name, level, next, HP, Ki, BP); the 7 blocks (total + base/estado/comida) are walked with the **d-pad down/up** on the gamepad (own 20 ms `Input.read` loop — the 100 ms registry poll misses quick taps — pad only READ, never blocked; gated on `Registry.active_adapter()` so the palette/tree opening OVER the page don't inherit it) and with **F11 / Shift+F11** on the keyboard. Registered below the palette+tree, above the Characters list. Pending in-game verify (needs a full RESTART) |
| Skill Palette / Super Attack equip | done | `screen_skillcustom.lua` — selected slot plate = `SelectActiveBorder` visible AND `BaseBlinkImage` hidden (structural plates 4/7 have both always ON); slot button from plate `ButtonIconImage` → `A.platbtn_name`; empty slot = literal "---" → "ranura vacía"; level/Ki/desc from the detail pane only while it names the same skill (pane lags and goes stale on empty). `SkillListMenu:GetSelectValue()` is DEAD here (frozen 0) — never use it. Verified in-game 2026-07-13 |
| Skill Tree / learn super attacks | done | **2026-07-14: the lock is read NATIVELY** from the game's own per-node state byte (offsets in *Derived facts*), so "bloqueada"/"adquirida" is correct on EVERY node whatever the browsing path — the old KNOWN LIMIT (noted below) is GONE. The name-propagation heuristic survives only as the fallback if the native read fails its bounds/FName self-check. Entry was slow (~30 s) until the feed's storm guard was fixed (see *Known issues*). Verified in-game 2026-07-14. — `screen_skilltree.lua` — `Start_Skilltree_C` < `UAT_UISkillTreeMenu`, ALL reflected (`Txt_Skillname/Txt_Lv_Num/Txt_Energy_Num/Txt_Detail/Txt_Name`); orbs = `WL_Skilltree_Zorb00` TArray of 12 `UAT_UISkilltreeZorb`: entries 1–6 = REQUIRED cost, 7–12 = OWNED. Orb color = POSITION in its 6-orb grid (red/blue/green/purple/silver/gold — verified twice 2026-07-14; textures don't encode color, all named "Ins_Item"). Reads name+lvl+Ki+non-zero cost, desc, owned orbs at the end. "How to acquire" text is NOT on screen while browsing (7-node full-text scan) — it only exists in the post-A message window, which already reads (decision: leave as-is). LOCKED nodes: the ONLY marker the game exposes is the tree cursor's padlock (`UISkillTree.Skilltree_Cursor.WL_ImgIconMicon` visible), and only on a skill's ENTRY (lv-1) node when the skill is unowned — 6 capture rounds proved there is nothing else: no readable panel index/pointer/position (cursor grid coords do live at tree+0x15F8/+0x15FC as grid×95), key-help bar identical on every node, no cover/lock widget on screen; the char-level gate ("need level 10") exists ONLY in the post-confirm message window (dialog reader speaks it). So the lock is PROPAGATED by skill name within a screen visit (`locked_skills`, cleared in reset): once the entry node reads locked, every level of that skill announces "bloqueada". KNOWN LIMIT (accepted): reaching a level-2/3 node WITHOUT passing its level-1 node first announces no lock — unfixable by reflection. Round-7 panel sweep closed the last door: all 108 `WL_Ins_Panel_Cover` are visible always (a frame, not a padlock) and every skill icon is an anonymous `MaterialInstanceDynamic`, so panels can't be matched to skills. The real data (`USkillManager`/`USkillTree`) exposes ZERO reflected functions and owned levels live in private save memory (`USkillSave`) — a full fix needs native RE (Ghidra), deferred. Verified in-game 2026-07-14. **2026-07-29:** a session crash investigation traced a silent refusal of `WL_Skilltree_Zorb00` to a NEW substrate bug (partial property-set caching in `ui_core`'s super-struct walk, truncating at the BP class and getting cached as authority) — not to this screen; the member is confirmed real (these are the 12 orbs verified above) and the fix is source-only / unverified, needs a full restart. Whether the mod actually crashed the process that session is STILL OPEN, pending the crash trail — see the *Last updated* entry and the crash ledger |
| Contextual actions (keyhelp) | done | `keyhelp_watch.lua` — the screen's ACTION prompts ("X: asignar", "Y: árbol de habilidades", "A: usar") read once on entering any menu and again only when the set CHANGES (diff-gated, queued behind the screen's own readout). Hangs off the `ui_registry` dispatcher, so every menu (incl. future ones) is covered; passive/time-critical readers opt out with `keyhelp_auto = false` (13 adapters). Face buttons are finally NAMED: `keyhelp.lua` now falls through to `A.platbtn_token` (the palettes' resolver) when the bar's device-INDEXED textures (Btn00..03) can't name themselves. Nav entries ("mover", "cambiar pestaña") are dropped. Ctrl+F2 toggles it (needs a game RESTART — main.lua); F2 still reads the whole bar. 2026-07-17: SAME-PHRASE COOLDOWN (30 s, `os.clock`, survives screen changes) — the dispatcher's screen flips cleared the diff gate and re-announced an identical bar within seconds, and an A↔B phrase alternation on cursor moves re-spoke each move; identical phrases now wait out the cooldown, new phrases speak at once, F2 unaffected. Verified in-game 2026-07-17. Read LEFT-TO-RIGHT as on screen: the bar is a CanvasPanel the game lays out itself, so the widget number is a slot id, NOT a position — the place comes from the slot's `GetPosition()` (its `LayoutData` offsets reflect back as 0.0), falling back to the render transform / ancestors. COST RULE (learned the hard way: the first cut lagged the item + skill-palette menus to a crawl): inside the poll step it may ONLY use `Core.cached_all` (tick passed — a raw `FindAllOf` per poll stalls the game thread) and it polls the bar's LABELS, resolving the glyphs just once, on the poll where they changed. Verified in-game 2026-07-14 |
| Quest objective HUD (text) | done | `quest_objective.lua`, VERIFIED in-game 2026-07-15 night. Two-step fix: (1) directory-mapped `{"fm","QuestNavigation"}` (detection was scan-starved); (2) single-objective quests put the text in the TITLE node (`Txt_Main00`/`WL_MainQuestListTitle`) with every M/S row hidden (F10 dump proved it) — groups now speak on title OR rows. F10 diagnostic kept but `DUMP=false`. **2026-07-31 (c), PENDING in-game verify:** diff moved from the composed string to a `sig_main|sig_sub` signature so a repaint that only changes the composition no longer re-announces; gated on `Core.scan_quiet()`; `Speech.say_protected` commit moved after the say so a shredded line isn't lost; `Quest.read()` now falls back to the last known text; new **L3+Y** pad chord repeats it on demand (F10's controller twin) |
| Episode title cards | wip | `screen_questcard.lua` (NEW 2026-07-15) — `AT_UIQuestMainStart.TitleText` (0x3E0) via `{"fm","QuestMainStart"}` (0x558); telop pattern (once per appearance, queued), registered below telop. `fm.QuestMainLogo` is image-only (ChapterTitleImage) — unread, by design. 2026-07-17 night (user): a story-chapter card in a cinematic ("Detén la invasión saiyan") went UNREAD — this reader was never verified and its `on_screen` gate may be blind like the intro cards' was; TRACE armed (`qcard` lines: both fm cards' on/vis/op/text per state change) — next episode/chapter card in the log decides the fix |
| Cooking menu | done | `screen_cooking.lua`, VERIFIED in-game 2026-07-15 night end-to-end (entry menu via the second `Shop_Top_C` chain; dish list; cook — latch spoke "Bollo jugoso al vapor" legitimately). HONEST CAVEAT from the latch log: the ghost pane read `vis=0 opacity=1.0` — `pane_live` did NOT discriminate it; the shadowing was actually killed by the yields (ring/entry-rows) + spoken-key suppression + the game parking the pane a while later. `LATCH_DEBUG` stays ON (one line per activation) to catch any residual window (e.g. emblems right after cooking) |
| Fishing minigame | done | `screen_fishing.lua`. RE-VERIFIED in-game 2026-07-15 (user landing fish consistently) after FOUR fixes that day: (1) directory regression — `AT_UIBattleRushSpeedCore` mapped via a pointer the game never sets → phase 2 dead; unmapped. (2) adapter's own 2 s absence backoff on a ~3 s hook bar → phase-1 cue late/absent since forever; removed (throttling is ui_core's job). (3) the game ALTERNATES between several pooled ring cores — the single cached_live pin was stale half the reels (vis=false, ringSize frozen; caught in the dump); now `ring_core()` picks the on-screen pool instance. (4) reel is <1 s (~420 u/s) and both buttons are random per catch → speech redesigned: phase 2 = bare letter only, on the phase byte (`fishing.phase == 2`), first tick; the "X, luego Y" pre-pair removed (the second letter was the stale core's). DEBUG off |
| Fishing catch result ("¡BRAVO!" sheet) | done | NEW 2026-07-17 `screen_fishresult.lua` — `Mgame_Result_C` < `AT_UIMiniGameResult`; header+species via TxtCap00/TxtName (BP-tree twins `Txt_Cap00`/`Txt_Name` — both spellings tried), obtained rows via the native `InfoLogBarList` TArray @0x408 (Txt_List/Txt_Num on each bar, census tree names as guarded fallback). fm.MiniGameResult @0x630 deliberately UNMAPPED (fm minigame fields twice proven unset); scan path + QUIET_EXEMPT; notice pattern. ROUND 2 (user: read only after pressing "Siguiente"): the pane_live gate demands GetVisibility()==Visible(0) but passive overlays render HitTestInvisible (Xcmn_Subtitles precedent) — gate now OPACITY-ONLY (fade-ghost still dropped). ROUND 3 (user: still silent; F7 dump_1784307940 shows the sheet + texts on screen): the reader was SHADOWED — the pooled fishing HUD lingers on_screen under the sheet and screen_fishing (registered far above) claimed every tick; "Siguiente" was merely the release. Fix: fishresult registered ABOVE screen_fishing (a notice, it releases immediately) AND screen_fishing yields while Mgame_Result_C is on screen (same feature/flow — also stops flip-flop prompt chatter over the sheet). Roshi token fix VERIFIED in-game same day. Pending re-verify |
| Gameplay toasts (item log + level-up) | done | `screen_toasts.lua`. 2026-07-17: LEVEL-UP BANNER FIXED — the real class is `Info_Log02_C` (bars `Info_Log_Bar_00..04`, text on bar `TextBox` @0x3C0, its ONLY text member); the old `Info_Log_Level_C` exists NOWHERE in the ObjectDump, which is why level-ups were silent forever (F7 census dump_1784302864_002 pinned it). SAME DAY the first cut CRASHED the game while fishing: a naked `bar.Txt00` ARGUMENT fetch (member that doesn't exist on `Info_Log_Bar02_C`). **The 2026-07-17 "fix" — move the fetch inside a `pcall`, TextBox only — WAS NOT A FIX, and this row said it was for nine days.** A pcall cannot catch an undeclared-member abort; and the fetch was only corrected in the `Info_Log02_C` loop, leaving the `Info_Log_C` twin still doing `bar.Txt00`. That twin killed the game on **2026-07-26**, right after a map change (the black box's last entry was `screen_toasts.is_active`; a map transition rebuilds the pooled bars, so the wrong subclass lands in `Info_Log_Bar00..04`). REAL fix: every fetch here goes through `Core.member`'s property gate. Pending in-game verify |
| Cinematic character-intro cards | wip | NEW 2026-07-17 `screen_infoname.lua` — the "Gohan, hijo de Goku" cards: `Info_Name_C` < `AT_UIInfoNameCore` (`NameTxt`/`PopularNameTxt` + `_Large` twins, all reflected), driven by sequencer track `ATMovieSceneCharaIntroductionSection`. fm.InfoName @0x668 deliberately UNMAPPED (same lazy-field risk); scan path + QUIET_EXEMPT. ROUND 2 (user: only a bare "Goku" heard, Gohan/Piccolo silent): rewritten — reads ALL on-screen instances, 2-tick text-stability gate (name/popular animate in separately), opacity-only liveness (pane_live's visibility==0 blocks passive overlays). ROUND 3 (user: cards READ now, but re-announced on every subtitle flip — long-lived cards outlived the 10 s dedup window): APPEARANCE-EDGE dedup — per-text {first,last,spoken} state, speaks once per continuous presence (stability 2 ticks, `spoken` set in update so a pending line survives subtitle ticks). ROUNDS 4-6 (repeats, then silence): SOLVED by the round-6 trace — the pooled `Info_Name_C` class is a DEAD END (4 parked hosts, never any text, never rendered); the REAL display is the fm-owned container `fm.InfoName` (AT_UIInfoName) flipping Collapsed↔rendered per card, `InfoCoreCtn` @0x3A0 → the active core with the texts. Reader rewritten as PURE POINTER READS (no scans; QUIET_EXEMPT entry removed); wall-clock GONE_S=30 dedup + 2-tick stability kept. FINAL round: the 2-3x repeats near subtitles were the SPEECH sink re-queuing the cut card after each subtitle interrupt (adapter provably spoke once — trace) — `Speech.say` gained a `no_requeue` flag for queued ephemeral overlays (speech.lua does NOT hot-reload: full restart). **VERIFIED in-game 2026-07-17 night (user): three cards, one read each, no repeats.** TRACE off |
| Soul Emblems grid / Community | done | 2026-07-17: CHAR_TOKENS `Mrs`/`Mst` were SWAPPED (user: Roshi spoke as "Mr. Satan" on board AND menu) — pak scan + romaji naming (Tpp=Tao Pai Pai): `Mrs`=Muten **R**o**s**hi, `Mst`=**M**i**s**u**t**ā Satan; `Ev_Msn` is a third unidentified token, left unmapped so it speaks raw. — `screen_community.lua`. 2026-07-15 saga, ALL VERIFIED in-game by night (entry on the normal path ~5 s first visit of a session, then instant; MOVEMENT verified — the native commuGrid cursor IS driven in the menu flow on `Start_Commu_Emb_C`, gridcurs dump; `GRID_DEBUG` back OFF): (1) unmapped from the directory (no trustworthy owner — two flows, `MenuSoulEmListIns` = `USoulEmblemMenu` reflects nothing); (2) menu-flow instance is the BP class **`Start_Commu_Emb_C`** (census) — `grid_host()` scans both names; (3) the GHOST BOARD was claiming the screen (`BOARD_LIVE_MODES` gate + `Core.pane_live` fixed it — screenshot 98); (4) "reads only after reload" = stale pool + parked-first pick — `grid_host()` enumerates the pool picking the live instance, and a ghost board with no live grid forces ONE budgeted rescan per visit. **ENTRY READS on the normal path now (user-verified)**. 2026-07-16: the ~5 s FIRST-visit lag (never-seen class waiting out ABSENT_BACKOFF; no ghost-board signature exists yet then) fixed with an ENTRY SIGNAL — the game's lazy menu controller (`mm.m_xSoulEmblemMenu` @0x158 / `cm.MenuSoulEmListIns` @0x80, both reflected; the controller's WIDGET pointer is not, AT.hpp:43512, which is why the class can't be directory-mapped) flips null→valid on first open and arms `Core.watch_for("Start_Commu_Emb_C")` (~400 ms budgeted re-scans, ~5 s cap, cleared when the grid reads); the ghost-board path arms the same lane instead of the old single-shot refresh. PENDING in-game verify (fresh session → open emblems: should read in ~1 s, log line "soul-emblem menu controller appeared"). STILL PENDING: cursor MOVEMENT verify (`GRID_DEBUG=true` writes `gridcurs` lines to `dumps/dump_community.txt` — if movement is silent, that dump says whether the native commuGrid offsets are driven in the menu flow). Bonus fact: each slot (`UAT_UIXCmnEmb_Cursor`) reflects `UnlockState` u8 @0x408 |
| Community Board cursor (story tutorial) | done | Verified in-game 2026-07-04, unblocked story; offsets in `native_offsets.commuBoard` |
| Story menu ("Historia") | done | **VERIFIED in game 2026-07-28 (user)** — reads on entry and on every cursor move, summary included, and the X progress checklist reads. **VERIFIED READING 2026-07-28** (user), with two gaps closed the same day. (1) Some entries read no summary: the pane lands the summary slightly after the title, so the settle went 2 → 3 polls. (2) X ("Mostrar progreso de la historia") flips the right pane to a checklist that was not read at all. The game names the two modes itself — `All_Win_Story` @0x3A0 / `All_Win_Task` @0x3A8 (AT.hpp:36897-98) — so the mode is a read, not a guess; TASK mode now speaks `Txt_Story_Task00..12` behind the heading + `Txt_Progress`, and the mode is part of the settled context line so flipping it re-announces once. Pending re-verify. Earlier: **NEW 2026-07-28** `screen_story.lua` — `Start_Quest_C` < `UAT_UIStartQuest` (AT.hpp:36894). Tab strip + scrolling quest list + detail pane. **Reads the DETAIL PANE, never the list**: the rows (`Xlist_List06_00..06` in `Xlist_List06_Lay9`, F7 census) are a virtualised window with no cursor index anywhere in the hierarchy, but the pane mirrors the selection — move the cursor and `Txt_Quest_title` follows. So the selection costs no index, no marker guess and no scroll arithmetic (the screen_skillcustom trick). Reads: current tab `Txt_Cap01`, kind `Txt_Cap00`, title `Txt_Quest_title(01)`, `Txt_Recommend`+`Txt_Num_Lv`, summary `Txt_Cap02`+`Txt_Detail(00)`, and rewards `Xlist_Reward_Bar00..05` (`Txt_Reward`+`Txt_Num`). 2-poll settle on the title so a fast scroll does not announce intermediate entries; tab change re-announces through the screen-entry branch (not the announcer's `tab`, which swallows the first row). Locked entries read the game's own "¿?" |
| Z Encyclopedia | done | **VERIFIED in game 2026-07-28 (user)**: index, category lists, entry sheets AND the note slip all read. **ROUND 3, 2026-07-28** — user: index, category lists and entry sheets all read correctly now; only the NOTE was missing. It is a slip that overlays the entry spread (A on an entry) with the detail text still showing behind it: `CompZ_Memo_C` < `UAT_UICompZPageMemo` (AT.hpp:32086), `TextBox_Title` @0x468 / `TextBox_Memo` @0x470 natively, `Txt_Title` / `Txt_Detail` in the BP tree — the BP body node is `Txt_Detail`, NOT the `Txt_Memo` the native name suggests, which is why both spellings are tried. Checked BEFORE the entry pages (it is what the player just opened) and claimed only when its BODY reads, so an empty or parked slip cannot shadow the entry underneath. Pending verify. Earlier: **ROUND 2, 2026-07-28** (user: "la enciclopedia no se leyó"). Round 1 never activated ONCE and left nothing in the log — no `screen -> screen_compz`, no gate line, no error — because nothing errored. Cause: **the book is not in the viewport.** `UAT_UICompZPageBase` owns a `RenderTarget` (AT.hpp:31974) that `UCompZMenu.UMGRender` draws onto an `AZCW_BookActor` (AT.hpp:40272-73) — the pages are rendered into TEXTURES and mapped onto the 3D book mesh, so they are never parented into the viewport widget tree and `Core.on_screen` / `first_on_screen` / `IsInViewport` all report false for a page the player is reading. The F7 census listing their text proved nothing: `discover.lua:830` tests bare `IsVisible()`, not the ancestor walk. Fixed by moving the whole adapter to own-slate-visibility (`Core.is_visible`, new `Core.first_text_offviewport`) and earning the screen another way — readable text AND a row whose `Canvas_Cursor` is showing. Pending re-verify. Earlier: **NEW 2026-07-28** `screen_compz.lua` — ONE adapter, three levels, five host classes: index `CompZ_Page_Contents00_C` < `UAT_UICompZPageContents` (AT.hpp:31999), category spread `CompZ_Page_Items_L/R_C` < `UAT_UICompZPageItems` (32080), entry spread `CompZ_Page_Img_C` < `UAT_UICompZPageImg` (32069) + `CompZ_Page_Detail_R_C` < `UAT_UICompZPageDetail` (32051). **The only list screen in this mod that needed NO guesswork**: the row class `UAT_UICompZList` (31861) DECLARES its own cursor node `Canvas_Cursor` @0x3A0, so the selected row is a reflected read rather than a hypothesis about which image the game drives; and the rows come from `UAT_UICompZListController.Item_List` @0x0030, a REAL `TArray` (size 0x10) — no fixed-array collapse, no RegisterCustomProperty, no offsets, just `Core.array_of`. Both list levels share the controller, so one code path. The L/R page split is irrelevant: rows are gathered from every live page and the marked one wins. Texts native-first with BP twins (`TextBox_Label`/`Txt_List`, `TextBox_Num`/`Txt_Num`, `TextBox_Contents`/`Txt_Caterory00` — the game misspells it —, `TextBox_Items`/`Txt_Category01`, `m_Name`/`Txt_Name`, `m_Text`/`Txt_Category`, `m_Title`/`Txt_Title_Detail`, `m_TextCond`/`Txt_Cond`). A locked entry has no summary, only `m_TextCond`, which is read in its place. Every heading comes from the game's own text; only the book's own name is an i18n key (`compz_title`). Logs one line per visit if a list page has rows but none marked, so a bad cursor node names itself instead of costing a round |
| Substory clear rewards | wip | **2026-07-28 round 2 — it had NEVER RUN.** User: the rewards sheet is not read. The adapter was registered (`app.lua:40`) and its widget map was right, but `screen -> screen_questreward` does not appear in the log **once**, while the F7 census taken with the sheet on screen shows `Quest_Sub_Reward_C_5.Xcmn_Win01.Txt_Title` = "Recompensas de historia" and rows `Xcmn_Win01_List_C_36..39` (`4583/EXP`, `1/Medalla D`, `1/Pan de la inteligencia`, `2/Megamasa de nivel 1`) all `vis=true enum=0:Visible op=1.00`. Cause: it gated on `Core.pane_live`, which demands `GetVisibility()==Visible(0)`; a passive overlay here renders HitTestInvisible, so the host was rejected every tick. **This is the exact bug `screen_fishresult` hit on 2026-07-17 and fixed privately**, so the opacity-only test is now shared as `Core.pane_rendered` and both adapters use it. Pending in-game verify. Earlier: **NEW 2026-07-26** `screen_questreward.lua` — the "¡FELICITACIONES! / Recompensas de historia" sheet after a side story. Was completely uncovered (the session log showed NO adapter active in that window); `screen_results` never applied, it reads `Quest_Main_Clear_C`, the MAIN-quest sheet. Identified from a user screenshot + F7 census: `Quest_Sub_Reward_C` < `UAT_UIQuestSubReward` (AT.hpp:35501), title `Txt_Title` on its nested `Xcmn_Win01` @0x540, rows are `Xcmn_Win01_List_C` (`Txt_Item` @0x428 + `Txt_Num` @0x430) ordered by `Core.slot_pos`. Registered ABOVE `screen_choicelist` — same row class as the difficulty picker, which would otherwise announce the rewards as options. `Lang_Txt_Congrats` is a UImage (no text), unread by design |
| Crash hardening — full-codebase audit | done (unverified) | **NEW 2026-07-27.** Systematic multi-agent sweep of all 71 Lua files + the 4 native bridges against the ten accumulated crash mechanisms; every candidate adversarially re-verified (48 → 11 real, 37 refuted). Fixed: `Core.array_of`'s missing existence gate (`ui_core.lua:419-443`); actor caches surviving the world gate — which a field battle closes WITHOUT a `LoadMap` — for `enemy_cache`/`navi_icons`/`chain_wait`/`target.actor` (`nav_tracker.lua:1447-1471`); F1 running engine work on UE4SS's keyboard thread (`app.lua:195-207`); `Nav.dump_levels` shipping with neither validation nor gate (`nav_tracker.lua:3056-3243`); 9 bare `IsValid()` in `discover.lua`; raw two-hop `host.A.B` chains in `screen_choice`/`screen_training`/`screen_itemuse`; and 8 native findings incl. a 32-bit overflow in `audio_bridge`'s RIFF bounds check and an unvalidated arbitrary-write in `mem_bridge` (now bounded + `expect_class` assertion in `mem.lua`). Full reasoning and the five generalizable rules in the [crash ledger](reference/dbz-kakarot/notes/dbz-kakarot-crash-bug.md). `lint-lua.ps1` exits 0 over 71 files; 4 DLLs rebuilt clean. Needs a full RESTART — see *Next step* for the retest checklist |
| Crash diagnostics (black box + breadcrumb) | done (unverified) | **NEW 2026-07-26.** `mem_bridge.mark()` — a 64-slot ring in a memory-mapped file (`crash_trail.bin`, gitignored, stripped from releases); the OS flushes it when the process dies and `main.lua` prints the previous session's trail into `UE4SS.log` at boot. Marks every adapter `is_active`/`update` by name plus the nav/explore/quest/battle/guide loop steps. Named the crash site on BOTH crashes it has seen. Plus one `screen -> <adapter>` line per screen commit, and per-adapter pcall isolation that logs the faulting adapter's name. Tested standalone with TerminateProcess |
| Story / battle results | wip | `screen_results.lua`, `screen_battleresult.lua` (rank from brush textures). Constant-"222" value bug: round-1 dump (2026-07-17, user) CONFIRMED all digit images share ONE atlas material `Ins_Num_Result02` (mix of the shared MaterialInstanceConstant and per-widget MaterialInstanceDynamics) → the digit must be a MATERIAL PARAMETER on the MID. `DEBUG=true` round 2 now also dumps each brush material's Scalar/Vector parameter values + parent to `dumps/dump_results.txt`; decoder fix follows the next results-screen dump |
| Quest navigation radar | done | **2026-07-26 HARDENED — this file was the mod's biggest crash surface and had never been swept.** 24 bare `:IsValid()` (which FAULT on a freed handle rather than rejecting it) + ~95 naked member fetches, on actors that streaming frees and combat destroys, incl. `target.actor` held for MINUTES. All migrated. Also: `enemies_list`/`navi_icons` refused to refresh while `Core.scan_quiet()` (set by the DIALOGUE adapter), so "battle ends → dialogue opens" pinned a list of just-destroyed enemies — an expired-but-unrefreshable list is DROPPED now, never served. — `nav_tracker.lua` + `audio_bridge`; auto-tracks quest markers, arrival cue confirmed. 2026-07-15: battle-interruption resume — a world-gate/transition drop of a MANUAL pick stashes `resume_pick` (plain data) and re-acquires it by category+key when the world returns (10 tries, ~3 s apart); the quest auto-scan stays quiet while pending; cleared by B / F3 off / a new pick. Pending in-game verify |
| Radar categories 2.0 (sites/enemies/collectibles) | done | Verified in-game 2026-07-15 (user: "funciona perfecto") |
| R3 radar target picker (modal) | done (regression fixed, unverified) | **2026-07-28: dead on both binds** (user: "el radar no se está abriendo ni con R3, ni pulsando v"). Nothing wrong with input — `input_bridge loaded (hooked=true)` in the log, and both binds funnel through `do_open()` → `Nav.list_targets()`, which is where the log's 16 identical tracebacks stopped: `Core.member`'s 07-27 result gate called `Core.valid_ref` on `UniqueId`, a **NameProperty**, and `IsValid` on an FName raises `No overload found for function 'FName'` THROUGH pcall. Fixed in the substrate (`Core.member` gate narrowed to Array/Struct, new `Core.name_str`) — see the *Last updated* entry and the crash ledger. Earlier: verified in-game 2026-07-15 together with the categories batch (bind is R3 — early docs said "hold R2", stale) |
| Telepathic messages (King Kai) | todo | Player asked how to "answer" one (2026-07-25). Dump sweep verdict: **there is probably nothing to answer.** No Telepathy widget class, no message-kind enum, no Accept/Reply/Open function, no `EATPlatBtnId` / `ConfirmationMessageId` / KeyConfig reference anywhere in the headers. The one concrete hit is `ATDebugSendTelepathyNotice()` (AT.hpp:26783), sitting in a debug block beside `ATDebugSendReusableNotice` / `ATDebugSendNormalCrossTalk` / `ATDebugSendFeverNotice` / `ATDebugSendEventCrossTalk` — so telepathy belongs to the game's ambient **CrossTalk / Notice** family (auto-playing banter), not to a menu. Voiced line exists: `AT/Content/Sound/Voice/{en,ja}/V_Telepathy.uasset`; the subtitle text is presumably a row in a CrossTalk table (`FCrossTalkTableRow`, AT.hpp:4372) whose asset name was not locatable in the pak index. **GAP:** zero mod coverage for telepathy/CrossTalk by name — if the line does NOT route through `Xcmn_Subtitles_C` (which `screen_dialogue` does read, gated on the game's own EnableSubtitle option) it is silent. To settle it: call `ATDebugSendTelepathyNotice()` live while logging the widget classes that appear. NOT done — it is a debug entry point on a build whose debug blueprints are stripped (the `LoadAllAssetsBefore*` lesson), so it needs the user's go-ahead |
| Radar: Exits category (get out of a building) | wip | **2026-07-25 round 2** (user: "dice salidas pero el radar no las rastrea" — the tab announces, so the SCAN works and the failure was in TRACKING). Cause: a head-on collision between two earlier fixes. `sweeping = target.manual and chainable(grp)` gates the ghost filter at nav_tracker ~1531, which drops a reached target whose `bHidden` is true — added 2026-07-17 for the parked future-story CHARACTERS the game hides near the player. But a door is an `ATriggerBox`: **an invisible volume whose `bHidden` is true as its normal state.** So the first tick after picking a door chained over it to the next door, also hidden, also dropped — a door could never be tracked at all. Fix: `chainable()` now excludes `"exit"`, which kills the ghost filter for this group AND gets the semantics right (you walk THROUGH an exit and the world changes; sweeping to "the next door" is not a thing anyone wants). LESSON: a filter written for one actor family (hidden = "ghost/parked") is wrong for another (hidden = "trigger volume, by definition") — when adding a radar category, check every filter the tracking loop applies, not just the listing path. Pending re-verify. |
| Radar: Exits category (get out of a building) — round 1 | done | 2026-07-25 user request — trapped inside Goku's house with no way to find the door. `AATDoorVolume` < `ATriggerBox` (AT.hpp:13004) is the game's own door/area-transition volume: `AreaName` FName @0x378 (destination), `DoorName` @0x370, `AreaMessageId` @0x358, `PlayerStartTransform` @0x390, `DestinationDoor*` @0x560, plus `OnActorBeginOverlap` (you walk into it). It is an ACTOR, so it goes straight through `add_target` — no new tracking machinery. New `"exit"` group, SECOND in `GROUP_ORDER` (one R1 from the default; making it first would displace quests everywhere, and indoors nearly every other group is empty anyway so it is reachable regardless). Tight 300 m cap (`src == "door"`), but NOT excluded from the empty-group rescue — that exclusion exists for the parked CHARACTER preload pool, and a door volume is level geometry. Label = `AreaName` → `DoorName` → the bare category noun. No filtering on `bOnlyUsedInRoom`/`bUseDialog`: their meaning is not established and guessing could hide the very door the player needs. Doors are bidirectional, so the same list reads as "ways out" indoors and "ways in" outdoors. `radar_cat_exit` added to i18n.lua es/en + lang/es.txt + lang/en.txt. **Pending in-game verify** (needs a full restart: nav_tracker + i18n; also NOT verified whether `AreaName` reads as display text or an internal id) |
| Radar: Companions category | done | 2026-07-17: `companions` group in the R3 picker (between Characters and Enemies) reusing the Shift+F5 `companions()` collector verbatim (player/SpawnType-enemy/parked-pool exclusion, 300 m cap) + `enemy_display_name` for real names. radar_menu untouched (generic). Pending in-game verify |
| Radar: ghost-enemy fix (hidden actors) | done | 2026-07-17 VERIFIED in-game (user: gone at Goku's house). `enemies_list()` + `companions()` now filter `bHidden` via `char_visible` — the game parks future-story characters hidden nearby. `CurrentHiddenType` is AQuestCharacter-ONLY (AT.hpp:17553): reading it on AT_Character = uncatchable abort, so bHidden is the whole safe check |
| Radar: enemy levels | done | 2026-07-17 SOLVED by LIVE getter-chain decoding (the dump v2 follows the game's own virtual getters in machine code — no Ghidra round needed for the final hop): enemy level = **`ATEnemyStatus+0x390` (int32)**; player differs (`[[si+0x390]+0x328]`, 0x390 is a pointer there) — reader is enemy-only. Cross-checked: resolved 3 on an enemy a level-6 player beat easily (the earlier +0x1C=94 was a per-character id, REFUTED). Announcements re-enabled (proximity alert + R3 picker: "Soldado, nivel 3"). Pending final in-game listen. Chain evidence, HP find (`SI+0x394` f32), GetPowerCompareRank bonus: No reflected level exists (haiku dump sweep); Ghidra (opus, 17 queries) proved the chain — `AttributeComponent`@0x8E8 vtable[0x3E8]() → int32, floor 1 — but the member is statically unreachable (RTTI-stripped, ~200 candidate vtables); PINNED at runtime with the new Ctrl+Shift+F5 dump (2 saves): **`StatusInstance+0x1C`** (player 4/64 per save, enemies 2/5/6/93; corroborated live: SI+0x394 float = current HP fell 500→287 under attack). Reader `enemy_level()` (reflected hops + `Mem.i32`), level baked into enemy `disp` label → proximity alert + R3 picker say "Soldado, nivel 12". Bonus found: reflected `GetPowerCompareRank` (UCharacterAuraComponent, 0–6 stronger/weaker rank) unused for now. Pending in-game verify |
| Radar: quest FOCUS (stay on the side story) | done (unverified) | **NEW 2026-07-28** (user: "al rastrear una misión secundaria que tiene varias fases, el radar me vuelve a rastrear la principal"). TWO independent causes. (1) **The auto-scan classified with the wrong source.** `classify()` read the target ACTOR's `ATMapIconComponent.MapIconType`, and this file's own 07-04 comment already says that is main-coded or absent on sub quests — the real signal is the navi WIDGET's switcher (`navi_quest_icon`, 1=main / 2=sub / 3=DLC6-sub), which was wired only into `Nav.list_targets` (the R3 picker) and the debug dump, never into auto-tracking. So hand-picking a side-story marker worked while the auto-scan classified the same marker PRI_MAIN/PRI_OTHER: `preempt.pri = PRI_SUB` matched nothing and `best_candidate` fell through to a concurrently active MAIN arrow. `classify(actor, icon)` now asks the widget first, failing open to the old path. (2) **The bias was a one-shot** (~15 s), so even once correct it lapsed between phases. New `preempt.focus`: a standing quest CONTEXT applied to every auto-scan, so each new phase is picked up without opening the picker; an off-focus class (a main objective advancing in the background) no longer steals the radar. Released ONLY by evidence — `Nav.notify_objective_gone(kind)`, which `quest_objective` fires after 3 polls of that class being absent **from a READABLE HUD**, so a level load, a fight or an open menu produce no verdict and the focus survives a side story that sends you to another map. F3 off is the only full reset; a manual pick or B deliberately keep it. Also: `signal_check` now reports main and sub SEPARATELY instead of `kind = cm or cs`, which used to drop a sub advance whenever a main line moved in the same pass |
| Radar: objective auto-track (smart radar) | done | 2026-07-17: quest HUD objective-text SIGNATURE diff (quest_objective.lua — title + objective lines, counters excluded; stored sigs survive the HUD hiding in battle, so re-appearing unchanged text never fires) → `Nav.notify_objective_change(kind)` (wired in app.lua) → one-shot auto-scan preemption (`preempt` table in nav_tracker: ~10-scan TTL, `best_candidate` prefers the changed main/sub marker kind). The interrupted manual pick/sweep/resume is stashed (`preempt.stash`, plain data) and B in the R3 menu RESTORES it (nearest remaining item for chainable categories, the exact pick otherwise; B again = real stop). Explicit pick / F3 disarm everything. VERIFIED in-game 2026-07-17 (user: "parece que funciona bien"). 2026-07-17 IDLE RE-ARM (VERIFIED in-game, user): an objective signal while the radar is IDLE (no manual pick, no resume) now also lifts `auto_suppressed` in `notify_objective_change`, so a freshly activated objective is auto-tracked PERSISTENTLY, not just for the ~15 s preempt window — fixes the "wait for Gohan to find the apples, marker spawns seconds after the HUD text changes, radar stayed silent because it was suppressed post-B/post-arrival" case. Manual picks untouched (still preempt-and-stash, B restores). One log line on the idle re-arm |
| Radar: gathering chain fixes | done | 2026-07-17 (user report: collected a spot, radar never advanced): (1) mining/bug-nest/chest icons marked STATEFUL by class name (AccessPoint*/MiningPoint*/PlacementObjectInfo < AAccessPointBase, CXX dump) → the chained sweep advances on InteractState=Taken and taken points drop from the picker; (2) fruit/small-fish spots are `ASpawner*Volume` (NO taken state exists) → gathering picks use the wide ARRIVE_DIST (8 m) instead of the 1.5 m interact radius, so the sweep arms anywhere inside the patch and the walk-away trigger advances it. VERIFIED in-game 2026-07-17 (user, same session as the auto-track) |
| Localization (external TXT + all game languages) | done | `i18n.lua` now overlays editable `Scripts/lang/<code>.txt` on top of the built-in es/en tables (external wins → editable source of truth). 13 base languages (the game's own `ELanguageType`: ja en fr de es it ko pl pt ru zh ar th; es_ES/es_MX→es, zh_CN/zh_TW→zh). TXT format: `key = value`, `#` comments, dotted prefixes `buttons.` `keyhelp.` `header.N` `startlist.N`, `\n` newline, `\s` edge-space (combo_join/controller_prefix). Resolution: ext[lang] → S[lang] → S.en → key. Language override in the config menu (`I18n.force_language`, re-applied by app.start on reload). Verified in-game 2026-07-17 |
| Mod config menu (L3+R3) | done | `config_menu.lua` — screen-reader modal like the radar picker; opened with **L3+R3** in the overworld (gated on `Nav.field_ready()`). Options: audio cues on/off, cue volume 0-100% (±10, samples a ping), radar auto-activate on/off, language (auto + 13 codes). Persists via `settings.lua` → `Scripts/config.txt` (user-editable, gitignored). D-pad up/down move, left/right (or A) change, B closes. Mutex `_G.__KakarotPadModal` with radar_menu; radar ignores R3 while L3 held so the chord never opens the picker. Verified in-game 2026-07-17 |
| Explore-radar toggle (double-R3) fix | done | `radar_menu.lua` — the double-R3 "explore other objects" toggle didn't turn OFF: a double slower than the 280 ms window let the single-tap fallback open the picker on tap 1, and tap 2 only cancelled it (`Nav.toggle_explore` never fired). Fix: window widened to ~400 ms + a slow-double RESCUE (tap 2 within ~680 ms of tap 1, no picker navigation in between, closes+toggles). Verified in-game 2026-07-17 |
| Battle monitor | wip | `battle_monitor.lua` present |
| Cinematics / transition fluidity | wip | **CAVEAT 2026-07-26: quiet mode is correct about COST but must never mean "keep serving stale handles"** — that reading of it caused a crash (see the radar row). Deferring a scan is fine; continuing to dereference what the scan would have replaced is not. — 2026-07-16 pass, PENDING in-game verify. (1) ui_core QUIET MODE (`Core.set_quiet`, published by ui_registry from the committed adapter's `scan_quiet` flag — set on `screen_dialogue`): steady-state backoff-expiry scans (~65ms each) defer while subtitles/talk own the screen; boost- and watch-driven scans still run. (2) ui_registry IDLE THROTTLE: with no active adapter the ~33-adapter sweep now runs every SWEEP_EVERY (300ms) instead of every tick, except inside a ~1s HOT window after any pad press or screen commit (menu-open latency unchanged). (3) pad_boost no longer opens boost windows while quiet (mashing A through dialogue was a steady scan drip). (4) pad_poll RELAX: 20ms dispatcher drops to every 5th tick (100ms) while `_G.__KakarotPadRelax` (quiet or map transition). (5) nav_tracker's two raw FindAllOf refreshes (navi icons / AT_Character) defer while quiet. (6) screen_loading content() pool walk throttled to ~300ms wall clock. (7) game INI: `HookAActorTick=0`, `HookBeginPlay=0` (mod registers no hooks; needs game RESTART). Verify: cutscenes + menu↔cinematic flips feel smoother; menus still read instantly; subtitles unaffected |

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

## Next step

**2026-08-03 (g): THE POST-BATTLE RADAR, AND THEN THE MEASUREMENT THAT IS STILL OWED.**
Full RESTART.

1. **Finish a battle with the radar tracking a quest objective.** It should resume guiding within
   about a tick of regaining control, with no re-announcement (the objective did not change). Try it
   with a HAND-PICKED target too (R3 → pick something → get into a fight): that path is the resume
   lane, which is separate, so if only one of the two is still slow, say which.
2. **Advance a quest so the objective CHANGES.** The radar should start guiding to the new marker
   almost at once. Two caveats that are NOT bugs: the game sometimes spawns the marker seconds after
   the HUD text changes (waiting on an NPC to trigger the step), and a marker that never appears
   still burns the preempt's tries over ~15 s. What would be a bug is a consistent beat of silence
   with the marker already there.
3. **Ctrl+F5 after ~10 minutes** — this is still owed from (d) and it is the only way to know whether
   the ghost work actually paid. Compare against `ui step ms: avg 35.48 / max 925`,
   `findall scans: n=1982 / 119463 ms`, `ghost classes: 42 / 84783 ms`.
4. **Soul Emblems** (from (e), still unconfirmed): the grid must read, and the board alone must still
   read.

**2026-08-03 (f): THREE REPORTED BEHAVIOURS, IN THIS ORDER.** Full RESTART.

1. **The radar must NOT go quiet during gameplay chatter.** Walk around until the party talks. The
   beacon should keep running throughout. Then confirm the opposite case still holds: during a real
   CUTSCENE it must go quiet, and come back when control returns. The log prints one line per
   transition — `dialogue nav_mute -> true (…)` — and there should now be none at all while the
   minimap is up.
2. **First R3 of a session.** It may take up to about a second to open on a cold cache (it is
   waiting for the sweep instead of guessing), but it must open with the full lists. "Nothing to
   track" is now only correct when there genuinely is nothing.
3. **After a battle.** Say whether it improved — if it did, the mute was the cause; if it did not,
   the next suspect is the resume path's chunked rebuild and that needs its own measurement.
4. Still open from (e), please confirm: Soul Emblems reads the GRID, and the board alone still
   reads. And the Ctrl+F5 comparison (`ui step ms` / `findall scans` / `ghost classes`) whenever
   there is a quiet ten minutes.

**2026-08-03 (e): THE TWO REPORTED BUGS, THEN THE SAME MEASUREMENT.** Full RESTART.

1. **Soul Emblems must read again.** Open the emblems grid from the community flow and confirm it
   announces the grid (emblem names / slots), not the board summary. Then open the BOARD on its own
   and confirm it still reads — that is the 2026-07-31 report this must not undo. If either is
   wrong, the claim trace names it in one line: grep the log for `commu claim=` and send the lines;
   `claim=board … grid=true` is the grid being shadowed, `claim=grid` while you are on the board is
   the new rule overshooting.
2. **The radar after a save load, and after a battle.** It should pick up guidance within about a
   second of regaining control, not several. These are two different paths (a load flushes the
   directory roots, a battle does not), so please say which of the two is still slow if either is.
3. **Ctrl+F5 after ~10 minutes** as before, and compare `ui step ms` / `findall scans` /
   `ghost classes` against 35.48 avg, 119463 ms and 84783 ms.
4. Unchanged from (d) and still worth a glance: the title screen must read on a fresh boot and after
   quitting to title, and the five adapters that commit a tick earlier must not announce something
   that only flashed.

**2026-08-03 (d): PLAY IT, THEN MEASURE THE SAME THREE LINES AGAIN.** Full RESTART. This batch is
meant to be FELT, unlike (c) — the two reported symptoms are what it targets.

1. **The two reported symptoms.** After a battle and after closing a menu, the radar should pick
   the last hand-picked target back up promptly instead of after a beat. And the skill tree, the
   super-attack palette and the Z-Encyclopedia should start reading sooner.
2. **Ctrl+F5 again after ~10 minutes**, same spread of activity, and send the dump. Compare:
   `ui step ms` (avg was 35.48, max 925), `findall scans` (n=1982, 119463 ms) and `ghost classes`
   (42 / 84783 ms). If the ghost total has not dropped by roughly half, the escalation is not
   biting and the reason will be in which names are still at the top of the list.
3. **The title screen is the one thing that could regress.** Quit to the title from a save, and
   also restart the game to the title: the main menu and the boot consent screens must read exactly
   as before. If the title has gone SILENT, `Core.gameplay_world()` is answering true when it must
   not — say so and it is a one-line revert.
4. **The five adapters that now commit a tick earlier** — items, dragon balls, save/load, super
   attacks (skillcustom), skill tree, fishing. They should feel snappier; what would be wrong is
   one of them announcing something that only flashed on screen.
5. Anything that reads STALE radar targets (a picked target that no longer exists, a picker listing
   things that are gone) is fix (3) being wrong — that one changes what the sweep is allowed to
   reuse.

**2026-08-03 (c): ONE MEASURED SESSION, THEN THE GHOSTS DIE.** Nothing in this batch should be
audible. What it needs is a dump, not a verdict. It rides on top of the (b) batch below, which is
still unverified — test them together, full RESTART.

1. **Play normally for ~10 minutes, then press Ctrl+F5** and send the newest
   `Scripts/dumps/dump_*.txt`. Two lines matter: `findall scans:` (now complete — it finally
   includes the radar sweep) and the new `ghost classes:` block. Cover a normal spread on the way
   — walk around, open the ring and a couple of submenus, talk to somebody, take a battle —
   because a class can only prove itself NOT a ghost by being present at least once, so anything
   never visited will show up as a false ghost.
2. **The three re-gated screens must behave exactly as before.** The pause menu (in the field AND
   during a battle, where it must stay silent), the title screen (must NOT speak over the intro
   movie, must speak once the menu is up), and the item palette / customise screen.
3. If a screen goes quiet that did not before, it is one of those three, and `Ctrl+G` will not
   help (this is not a reflection gate) — just say which screen and when.

**2026-08-03 (b): IN-GAME TEST OF THE RADAR SWEEP REWORK.** `nav_tracker.lua` + `pad_poll.lua`.
Two changes behind one new indirection, `Nav.SW.class_list`, which every one of the sweep's ten
class lookups now goes through: (1) three of them are served from the GameMode's own manager
components instead of a `FindAllOf`; (2) the DEFERRED build (the nav-loop one) stops when it has
spent a nav tick and RESUMES next tick, reusing what it already scanned — the synchronous callers
are deliberately unchanged. Plus a per-dispatch time ceiling in `pad_poll` so the three
`begin_scan_tick` callers the merge put in one callback can no longer produce one unbroken ~390 ms
block. State hangs off `Nav.SW` because this file is at Lua's hard 200-local ceiling.

1. **Watch for the equivalence lines.** On the first sweep of a session the log prints, once per
   mapped class: `sweep source ATWindRoad: manager ATWindRoadManager.WindRoadList = N, scan = M`.
   **N must equal M.** A `MISMATCH` suffix means the manager list is not the same set as the scan
   and that class must go back to scanning — this is the check that makes the swap evidence-based
   rather than assumed, so please copy those three lines out.
2. **The R3 picker must still open with the full set.** Wind tunnels, gathering points and quest
   NPCs are the three classes that changed source. Compare against what you remember; anything
   missing points at (1).
3. **Explore mode + R3 spam.** The build is now spread over several nav ticks, so the picker may
   open a beat later on a cold cache — but it must never open on a SHORT list. `targets_build_ms`
   was made chunk-aware for exactly this.
4. **The stall should be gone.** The 578 ms `nav.explore` gap in the last trail is the thing this
   targets; a fresh trail should show no gap anywhere near it.

**2026-08-03: IN-GAME TEST OF THE ADAPTIVE DISPATCH GRID.** 5 Lua files on top of the 2026-08-02
batch below; test them together, full RESTART. Everything here is about **input feel** — if any of
these is wrong the symptom is a control that responds late or not at all, never a crash.

1. **Every pad gesture, in order.** The R3 single tap (opens the picker), the R3 **double** tap
   (toggles explore — this is the one most at risk, it now times against `os.clock()` instead of a
   dispatch count), the slow-double rescue, L3+R3 (config menu), L3+Y (repeat objective), and the
   travel-list and status-page d-pads including **hold-to-repeat**. All must feel as before.
2. **A tap that starts and ends fast.** Flick R3 / the d-pad as briefly as you can. This used to be
   droppable on three of the five steppers and should now always register — that is the change that
   makes the slow grid safe.
3. **The escalation seam.** The fast grid arrives up to one slow tick (~100 ms) AFTER a menu opens,
   by design. Opening a menu and immediately hammering the d-pad is the worst case: the first input
   or two ride the 100 ms grid. Confirm nothing is lost — delayed is fine, dropped is not.
4. **A menu open across a cutscene or a load.** `relax` deliberately outranks the fast demand there,
   so the d-pad drops to 100 ms for that stretch. Confirm it recovers when the scene ends.
5. **Ordinary free roam and combat.** This is where the win is: nothing should have changed at all
   from the player's side, while the dispatch rate is ~5× lower.
6. **The tick hook (`tick_hook.lua`).** Look for `tick hook installed on …` in the log once you are
   in gameplay — it should NOT appear at the title screen, and should appear within ~2 s of a save
   loading. With it installed the mod's steady-state `ExecuteInGameThread` rate is ZERO. Then check
   the handover both ways: go back to the title screen or through a map load (the hook's GameMode
   is per-map, so its beat stops and the LoopAsync watchdog must pick the mod straight back up
   within ~100 ms), and confirm nothing goes silent or doubles up. If the line never appears at
   all, nothing is broken — the mod is simply running exactly as it did in the previous batch.

**2026-08-02: IN-GAME TEST OF THE SINGLE TICK BUS AND THE SELF-HEALING MEMORY GUARD.** 6 Lua files
(`mem.lua`, `pad_poll.lua`, `ui_core.lua`, `nav_tracker.lua`, `battle_monitor.lua`,
`quest_objective.lua`); no bridge rebuilt. **Full RESTART required** — every loop moved.
`tools/lint-lua.ps1` passes (syntax, globals, guards).

The guard change is low risk; **the loop merge is the risky part**, and every check below is about
timing rather than crashes:

1. **Menu reading at full rate.** Open any menu and move the cursor. It must feel exactly as before.
   If it feels ~half speed, the 100 ms stepper is aliasing to 200 ms on the dispatch grid — the
   `DUE_SLACK_S` half-tick in `pad_poll.lua` is what prevents that.
2. **Pad responsiveness under load.** The R3 radar picker, the L3+R3 config chord, the map travel
   d-pad and the status-page d-pad. Pad steppers dispatch BEFORE the periodic ones precisely so a
   narrator step can never sit between a press and its response — confirm nothing feels laggier.
3. **The relax gate (the case most likely to regress).** During a cutscene with subtitles and during
   a map load, `__KakarotPadRelax` drops the dispatch grid to 100 ms and the slow steppers now ride
   that grid. The battle HP narrator (250 ms) and the objective narrator (300 ms) must still fire on
   time there, and F10 / L3+Y must still repeat the objective mid-scene.
4. **Reader off/on (Ctrl+M) and reload (Ctrl+Shift+R).** Off must silence the reader (the `ui`
   stepper retires itself via `should_run`), on must bring it straight back, and a reload must not
   leave two of anything running or any subsystem dead.
5. **The new log line.** `memory pre-check: attempt N unwound … (streak 1 of 8) — guard STAYS ON`
   appearing at all is confirmation that the `lua_instances` race is live on that machine — and,
   unlike before, the session should now continue with the guard intact.

**2026-07-31 (c): IN-GAME TEST OF THE CRASH-TRAIL FIX AND THE OBJECTIVE-ANNOUNCEMENT FIXES.** 6 Lua
files plus `README.md`/`README.txt`/`package.ps1`; no bridge rebuilt. **Full RESTART required**
(`main.lua` changed, so Ctrl+Shift+R is not enough).

1. **Explore mode (double-R3) plus the R3 picker, back to back, repeatedly.** This is the reported
   crash site: the picker's target sweep used to run inline on the pad dispatch on a cold cache. It
   should still open (now possibly a tick later while the nav loop builds the list), however fast
   R3 is tapped, and nothing should freeze.
2. **A field battle or cutscene starting while a manual radar target is picked, and a cutscene
   starting mid-conversation.** The reasoned-but-unconfirmed fix for "crash right as a cutscene
   starts" depends on `screen_dialogue`'s new per-tick `nav_mute`.
3. **The quest objective**: a fresh objective should announce promptly, an HUD repaint that doesn't
   change it should stay silent, and F10 / L3+Y should repeat it on demand — including mid-cutscene
   and inside a menu.
4. **If a crash still happens**, get `crash_trail.bin` and `UE4SS.log` **before relaunching** (the
   README now says so explicitly) — the next launch resets the ring. Check any `pad.tick`-ending
   trail's build against `93d539c..HEAD`, never the tag list.

---

### Previous next step (2026-07-31 (a), superseded)

**2026-07-31: IN-GAME TEST OF THIS SESSION'S BATCH.** ~25 files changed and three bridges rebuilt
(`prism_bridge`, `input_bridge`, `audio_bridge` — `mem_bridge.dll` is untouched; `mem.lua` changed,
not the DLL). **Full RESTART required**, Ctrl+Shift+R is not enough. Lint clean over 74 files. What
to check, in order of how much it could take down:

1. **Does the mod still SPEAK AT ALL, and does the boot line name your reader?** F8. `prism_bridge`
   changed. If it is silent, that is the whole session's biggest risk and nothing else matters.
2. **`Core.array_of`'s new `strict` and `ui_directory`'s automatic strict on alternative chains** —
   these sit under every screen and their failure mode is SILENCE, not a crash. If several screens
   go quiet at once, press **Ctrl+G** to disable the reflection gates; if they come back, the gates
   are the cause. The log prints one `strict array gate: …` / `member gate: …` line per distinct
   case, which is the evidence. Failing closed here is meant to be BOUNDED (a tick or two while the
   class is enumerated) — if it is permanent, that is a bug, not the design.
3. **`Core.peek_all` no longer resolves directory roots by scanning.** Watch the world-map travel
   d-pad and the status sheet, the two fast loops that depend on it.
4. **The map, against the reporter's three complaints**: hold the d-pad (auto-repeat), tap it fast
   (no press should be dropped now), press **X** on both maps (should describe, not just list).
5. **Braille**: with a display connected, `braille = auto` should just work; the boot log prints
   `braille output ON/off (mode=…, backend=…)`. If speech stutters more than before, set
   `braille = off` in `config.txt` — it is one extra backend call per utterance and
   `__KakarotSpeechStats` (Ctrl+F5) now times it.
6. **The Krillin cutscene**, if it can be reproduced: the objective should NOT re-announce during it.
   Whatever happens, the log now carries `objective -> …` / `objective change -> …` lines, and a
   crash leaves `nav.markers` / `nav.mapicons` in `crash_trail.bin` — `tools/read-crash-trail.ps1`
   decodes it without relaunching. **Ask a reporter for that file before anything else.**

---

### Previous next step (2026-07-27, superseded)

**RETEST THE CRASH-AUDIT BATCH.** 14 files changed (10 Lua + all 4 native bridges),
source-only and unverified in game. **A full game RESTART is required** — `app.lua`/`main.lua`
changed and every DLL was rebuilt, so Ctrl+Shift+R is not enough. What to watch, in order of how
much of the mod it could take down:

1. **`Core.array_of`'s new existence gate is the riskiest change** — it sits under every screen, and
   its failure mode is *silence, not a crash*. If several screens go quiet at once, press **Ctrl+G**
   to disable the reflection gates; if they come back, the gate is the cause and the property-type
   map is what to look at. The gate fails open by construction (`set == nil` ⇒ allow) and consults
   `custom_props`, so a `RegisterCustomProperty` member should not be blocked — the `UE4SS.log`
   prints one `array gate: <Class> has no '<name>'` line per distinct case, which is the evidence.
2. **Radar behaviour right after a short field battle.** The caches now drop at the world gate, so
   the first post-battle scan is a real `FindAllOf`. Expect: enemies re-announced correctly instead
   of ghosts, and a manual pick interrupted by a battle *shorter than 5 s* still resuming (it now
   goes through `remember_pick()` rather than being silently lost). A collectible you were walking
   to when the battle started must NOT be marked as visited.
3. **F1 (repeat current readout)** — now deferred to the game thread, so it may feel a frame later.
   It must still work in every screen, and must do nothing during a map transition.
4. **Audio cues and the gamepad.** Both bridges changed: WAV loading is stricter (a malformed file
   now degrades to "missing" instead of reading out of bounds) and the pad block is lease-based, so
   the radar/config modals must still block the pad while open AND the pad must free itself if a
   modal dies unexpectedly (this used to require killing the game).
5. **The world map's d-pad travel** — `screen_map`'s four writes now go through one `ft_write_sel`
   with a `Map_World_C` class assertion. If travel selection stops responding, check the log for a
   refused-write line before suspecting anything else.

If a crash still happens, the black box (`crash_trail.bin` → printed into `UE4SS.log` at next boot)
names the last op as before; `mem_bridge`'s recorder was itself hardened in this batch.

**Three further native findings (2026-07-27, second pass) — two fixed, one open by decision:**
`audio_bridge`'s `do_init()` now releases everything on all nine failure returns (`release_all()` +
`init_fail()`) and `load_wav` frees before reallocating — note the leak was narrower than first
reported: `Audio.init()` runs once from `main.lua`, which is outside the hot-reload set, so
Ctrl+Shift+R does NOT re-run it. `g_last` is now a **seqlock** (non-blocking writer, bounded reader
retry falling through to a direct `g_realGetState` read) since the pump thread runs inside the
game's own input hook. **Still open:** `prism_bridge` never calls the `p_shutdown`/`p_backend_free`
it resolves — `DllMain` is ruled out (loader lock), `App.stop()` is the reload path where PRISM must
stay alive, so closing it needs a real process-exit hook in `main.lua` that does not exist yet, plus
`Speech.shutdown()` → a new `prism_funcs.shutdown`. Harmless at process exit; only matters for a
backend holding an OS resource. Same reasoning left `audio` without a shutdown API: `Audio.stop()`
means "silence cues", not teardown, and a shutdown nobody calls is worse than a leak the OS reclaims.

**2026-07-26 (d): second crash, root-caused from the black box again — and the fix is now in the
SUBSTRATE, not in call sites.** Player exited combat into a story dialogue; trail's last op
`nav.step`; AV reading 0x10. Two independent causes, both fixed:
1. **Stale caches served because they could not be refreshed.** `enemies_list` / `navi_icons`
   skipped their refresh while `Core.scan_quiet()` is true — and the DIALOGUE adapter is what
   sets `scan_quiet` — so "battle ends → dialogue opens" pinned a list of just-destroyed enemy
   ACTORS and the radar kept dereferencing it. Now an expired-but-unrefreshable list is
   **DROPPED** (costs nothing) instead of served. The code's own justification — *"the stale list
   keeps serving, entries are re-validated by every user"* — is unsound: revalidation cannot
   detect a recycled address.
2. **`Core.member` validated the OWNER, never the RESULT.** `o[name]` on a null field yields an
   INVALID RemoteObject, NOT nil, so `if not d then return end` passed dead handles to the next
   hop. Fixed at 5 call sites AND at the source: `Core.member` now validates its result, picking
   `Core.valid` vs `Core.valid_ref` from the property TYPE the gate already records. Fails open;
   Ctrl+G kill switch. **This closes the class rather than the instance.**

**Also new:** `screen_questreward.lua` — the substory clear rewards sheet (`Quest_Sub_Reward_C`,
"Recompensas de historia"), identified from a user screenshot + F7 census; the log proved NO
adapter was active in that window. Registered above `screen_choicelist` (its rows are the same
`Xcmn_Win01_List_C` class the difficulty picker uses). `screen_results` never covered it — that
one reads `Quest_Main_Clear_C`, the MAIN-quest sheet.

**WATCH ON RETEST:** the `Core.member` result-validation is the riskiest change of the batch — its
failure mode is a screen going quiet, not a crash. If any screen stops reading, press **Ctrl+G**
to disable both reflection gates; if it comes back, that is the cause and the property-type map is
what to look at.


**2026-07-26 (c): USER RETEST — no crash, radar smoother. One regression-shaped report: the world
map's d-pad travel selection "works sometimes, especially not when I open the map several times".**
Cause found and it is PRE-EXISTING (verified identical at HEAD): `Map.update` set `dests_said = true`
BEFORE knowing whether `ft_build` returned anything, and `ft_guidance` rebuilt only when
`ft_points` was `nil` — an empty TABLE is not nil. One unlucky first tick (travel icons not
materialised, or the native InfoIcon block not yet populated) latched an empty list for the WHOLE
visit: no destinations announced, dead d-pad, everything else about the map fine. Re-opening the
map re-rolls that dice. This is the items-menu rule again: **never cache an empty collection as
final.** Fixed: the latch closes only on success (retries each 100 ms poll); `ft_guidance` is now
passive (it must not rebuild at 20 ms); and `Map.reset` asks for a `Map_World_Icon_C` pool refresh
on entry, since ft_build matches icons BY ADDRESS and the game recreates them every opening.
**HONEST CAVEAT:** the (b) scan-budget fix cut real `FindAllOf` throughput from ~12/tick to a
genuine 2/tick, and this class is on the scan path — so that change plausibly made a latent bug
fire more often. The entry refresh is the direct counter to it. Watch for other scan-path screens
getting slower to populate; if any appear, the fix is per-screen entry refreshes, not raising the
budget back.


**2026-07-26 (b): CRASH IDENTIFIED FROM EVIDENCE, not inference — the black box worked first time.**
Player crashed just after a map change. Trail's last entry: `ui.is_active screen_toasts`; the next
adapter's mark was never written (marks are written BEFORE the call) → the crash was inside
`screen_toasts.is_active()`. Cause: a naked `bar.Txt00` fetch — a member the same file's own comment
records as ABSENT on `Info_Log_Bar02_C` and names as the 2026-07-17 fishing crash. The 07-24 "fix"
put it inside a `pcall`, which cannot catch an undeclared-member abort. A map transition rebuilds
the pooled bars, so the wrong subclass lands in `Info_Log_Bar00..04` and it aborts.
The lint had missed it because it checked `IsValid`/`GetArrayNum`/`GetAddress` but not member
fetches → new `dynamic-member-fetch` rule (subscripts built with `..`/`string.format`), which
immediately found **13 more live sites** (screen_community, screen_fishing, screen_fishresult,
screen_results, screen_shopinfo, screen_tutorial, keyhelp, ui_archetypes) — all fixed. Also named
the 5 factory-registered adapters that were logging as `?`. Needs a restart; re-test the same
map-change flow.

**2026-07-26: THE ANSWER TO "users still crash on v0.1.2 while my log is clean" — the hardening was
only ever applied to the MENU substrate.** Full write-up in
[the crash ledger](reference/dbz-kakarot/notes/dbz-kakarot-crash-bug.md); the short version:

* **Crashes.** `nav_tracker.lua` (3,848 lines, every tick in free roam, over actors level streaming
  frees) was never swept: 24 bare `IsValid()` — which since 07-25 we know *faults* on a freed handle
  rather than rejecting it — plus ~95 naked member fetches, including on `target.actor`, a handle
  held for MINUTES across streaming boundaries. The dev tests menus; users play the game. That is
  the entire discrepancy. All 24 → `Core.valid`, 37 fetches → `Core.member`. Same shape fixed in
  `keyhelp.lua` (a `Txt_Keyhelp_01..09` loop bounded by ONE class's member count).
* **Substrate holes:** `Core.pane_live` — the playbook's mandatory pooled-pane liveness test, called
  with the adapter's own cached handle — was itself unguarded; `ui_directory.prop()` had no
  property-existence gate while two callers fetch undeclared members by design.
* **Slow menus, two causes, both ours.** (1) The 07-25 crash fix made `Core.valid` ~3× costlier and
  `first_on_screen` runs it over entire pools every tick → per-tick memo added. (2) `begin_scan_tick`
  had SIX callers and each refilled the budget, so "2 scans/tick" was really a dozen at ~65 ms —
  the playbook's own *a scan slot is not a rate limit* rule, broken inside the substrate that
  enforces it. Refill is now wall-clock keyed. This explains the old unaccountable 31%-of-game-thread
  measurement.
* **Prevention:** `tools/lint-lua.ps1` (syntax + globals + guards, all 70 files) is now a hard gate
  in `package.ps1`. Nothing validated the Lua on the way out before.
* **Combat specifically:** `quest_objective.first_text` was the strongest mid-combat candidate —
  it tries candidate member names *expected to be absent*, at 300 ms, on a host the game hides and
  rebuilds when a battle starts, behind a comment claiming absent members "read as nil — safe"
  (they abort). Same shape in `guide_watch` (`win[m]` from a candidate list, as a call argument).
  `battle_monitor` was already clean. Coverage checked: every continuously-running loop is now
  gated (`pad_poll` touches no UObject at all).
* **CRASH BLACK BOX (new, `mem_bridge`):** a 64-slot ring in a memory-mapped file records what the
  mod was doing (~180 ms of trail); the OS flushes it when the process dies, and `main.lua` prints
  the previous session's trail into `UE4SS.log` at boot. Every adapter `is_active`/`update` is
  marked by name, plus the nav/quest/battle/guide loop steps. **Tested standalone** (kill with
  TerminateProcess, recover from a second process) — which is how its first build was caught
  faulting on recovery from an 8224-into-8192 overflow. So the next crash names its own site.
* **Diagnosability:** `ui_registry` prints one `screen -> <adapter>` breadcrumb per commit, adapters
  are pcall-isolated and log their name on fault, and the README tells players to copy `UE4SS.log`
  after a crash, relaunch once, and send that second log too (it carries the black-box trail).
* **Rejected:** disabling the UE4SS hook flags. UE4SS drains `ExecuteInGameThread` through
  `ProcessEvent` — the mod's whole poll loop — so turning them off would silence the mod from boot.

**ALL OF THIS IS SOURCE-ONLY AND UNVERIFIED IN GAME.** It is luac- and lint-clean. `main.lua`,
`i18n.lua` and `ui_registry.lua` changed, so testing needs a **full game restart**, not Ctrl+Shift+R.
What to check: free-roam for several minutes with the radar tracking a target across a streaming
boundary (the crash path), and menu entry latency (should be no worse, likely better).

**Still open, deliberately not done in this batch:** 17 shared pooled classes
(`Xcmn_Keyhelp_C`, `Xcmn_Header_C`, `CFUIMultiLineTextBox`, `Info_Log*`, …) cannot be
directory-mapped and remain on the ~65 ms `FindAllOf` path, contending for the budget. That is the
remaining structural cause of menu latency and wants its own pass — after this batch is verified,
not stacked on top of it.

**2026-07-25 (p): release hygiene.** Sweeping for diagnostics before packaging found TWO left
enabled that were NOT gated on the dev build, i.e. they shipped: `screen_cooking.LATCH_DEBUG`
and `screen_results.DEBUG`. The second matters — it walks brush MATERIALS (the uncatchable-abort
family) and writes a file on every results screen. Neither was turned off (both still have work
depending on them); both now read `require("build_flags").debug`, so they stay live in the dev
tree and vanish from the package. That is what build_flags is for, and v0.1.0 already shipped a
per-frame trace by accident once. Verified inside the zip.

**2026-07-25 (o): round 3 — VERIFIED by the user. The LOG settled it in one read, and neither round-2 theory was
right.** `DLG_TRACE` had been on for the user's session, so this was one grep instead of more
reasoning (the rule earns its keep again). The decisive pair of lines:
```
22:43:52  speak=¡La batalla por el futuro contra los androides del terror!
22:44:31  speak=Cómo adquirir, ¡La batalla por el futuro contra los androi…
```
The same notice, first HEADLESS and then in full. Cause: `fresh_notice` judged novelty per NODE
**and composed from the fresh nodes only**, so "Cómo adquirir" — boilerplate shared by every
skill-tree "how to acquire" notice — was marked recent by the first one and silently dropped
from the next. That single defect produced BOTH user reports: a fragment reads as a repeat when
the full text follows, and a notice whose every node is boilerplate goes mute.
Fix: separate the two questions the function was conflating. **Novelty answers "is there news?";
composition answers "what does this notice say?"** — a RENDERED title now always heads the
utterance, while `news` (any fresh body/help node, or the rows rescue) still decides whether the
window speaks at all. A parked window with only a stale title still says nothing.
Consequence: the round-2 threshold split is REVERTED to one 0.7 s value. The pins were never the
repeat source; raising them to 2 s only re-created a window where deliberately re-opening a
notice stayed silent. The scenario the pins exist for happens with the window STAYING ON SCREEN,
so no absence is measured for it and the threshold's only job is to outlast a flicker.
Latency fix from round 2 (`Speech.release_protection` on dismissal) VERIFIED by the user
("el árbol se lee rápido"). `DLG_TRACE` stays ON for one more round.

**2026-07-25 (n): both halves of the notice fix needed a round 2 — and both were MY trade-offs
biting back.** User: dialogs repeat now, and after dismissing one the menu underneath takes a
while to come back.
1. **The latency is the protection window outliving its relevance.** `say_protected` protects a
   line for its ESTIMATED spoken duration (up to 6 s), and `Announcer:focus` defers on that. But
   the player DISMISSES the notice with a button press — at which point the line is no longer the
   most important thing on the reader, yet every other adapter kept politely waiting out the
   remaining seconds. New `Speech.release_protection()`, called from `screen_dialog`'s falling
   edge (window went off screen = dismissed). The protection still covers the whole time the
   notice is up, which is when something would actually have talked over it. LESSON: a priority
   window keyed to a TIME ESTIMATE needs an explicit end event, or it becomes a stall.
2. **The repeats came from lowering the PIN threshold.** Round 1 collapsed both suppressors onto
   one 0.7 s "real close". The pins were at 2.0 s for a reason — they guard the parked window
   that re-fires on its own, and that window FLICKERS, so 0.7 s let a flicker read as a close.
   Split again: recent FIFO 0.7 s (it is what blocked a deliberate re-trigger), pins back to
   2.0 s. When the two goals conflict, the one that adds noise loses. **`DLG_TRACE` is ON** so
   the next report is one grep instead of another round of reasoning.
**speech.lua does NOT hot-reload** (it is required before main.lua's protected snapshot), so
this one needs a full game restart, not Ctrl+Shift+R.

**2026-07-25 (m): the live (pre-save) key config — VERIFIED in game by the user, and it
re-proves the §2 rule.** Worth keeping as the cleanest example of it in this repo: reflected
data said "nothing here", one Ghidra pass said "it is a global TMap, and it is the same one
the game's own glyph resolver reads", and the fix worked first try with no capture round in
between.
User: the remap fix works, but the readout only changes after "Save changes", which is
confusing. Reflected data was a dead end (ONE field of that struct type in the whole game,
the saved one; no working copy on `UOptionMenu` / `UOptionMenuComponent` / `UAT_UIStartOption`;
no Apply/Commit function taking it). That is precisely the §2 tell — private state, nothing
reflected — so it went straight to Ghidra instead of another capture round, and the answer
came back in one pass: **the pending layout is not a member of anything.** It is a
process-global `TMap<FName,FName>` at exe base + `0x569C3B0`; the option screen's save case
*builds* `UATSaveSystem+0x720` from it (`FUN_1419e48a0`, writing 0x180..0x1D8 and skipping
`Keyboard_Type` — the offset match is what proves the destination is the real struct), and
the game's own glyph resolver reads the same map, which is why the icon updates instantly.
Reader: compare FName comparison INDICES (never convert to strings), refuse unless all 12
slots resolve to a permutation of the 12 physical keys, fall back to the saved struct on any
doubt. Note for next time: `FName("...")` as a Lua global is undocumented in our reference —
it is used inside a pcall so an absent global degrades instead of breaking.
Also: **Escape** now closes the picker and stops tracking (the pad's B).

**2026-07-25 (l): the rest of the batch — one user-VERIFIED fix, three new ones.**

- **VERIFIED by the user: the notice-interruption fix works** (entry (k) below). That is the
  first confirmation that `Speech.protected()` wired into `Announcer:focus` does what it was
  designed to do. Keep it in mind as the standard lever for "X talked over Y".
- **A re-triggered dialog stayed silent** (user, straight after that fix). Cause found by
  mapping the novelty machine: suppression is keyed **per TEXT** (`recent_set` FIFO of 24, no
  TTL, wiped only by a map transition) while every "this is a new open" signal the adapter owns
  (`was_on`, `appear_t`, `gone_since`) is **per APPEARANCE**, and the two were never joined —
  so identical words could never be spoken twice however deliberately the player asked. Fix:
  a REAL CLOSE (window continuously off screen for `CLOSED_CLEAR_S`) now clears the recent set
  as well as the pins, and the threshold drops 2.0 s → 0.7 s so a normal dismiss-and-retrigger
  qualifies. Everything those tables exist for happens WITHIN one presence, so presence scope
  loses nothing. Lesson worth keeping: **a dedup key must carry the thing that makes the event
  new.** Text alone never does.
- **Keyboard bindings are readable after all** — the same `InputAssign` struct holds them
  (`Battle_MeleeAtk = LeftMouseButton`, `Jump = SpaceBar`, …). `A.action_key` + `I18n.key`
  (new `keys` table, es/en, `keys.` prefix in the lang TXT overlay) and the keyboard tabs of the
  button-config screen now say the literal key instead of the controller equivalent. The
  "keyboard key is NOT recoverable" note in the reference doc was true only of the ICON data
  and has been corrected.
- **Radar target picker from the keyboard** (user request: people are asking to play on KB).
  It was a pad modal whose `step()` bails out with no pad snapshot, so a keyboard-only player
  could not open it at all. `Menu.key(cmd)` queues a command that `step()` consumes ON THE GAME
  THREAD (a keybind callback is not the game thread and `do_open` reads the world); `kb_open`
  distinguishes "no pad because there is none" from "the pad went away mid-menu", which the old
  teardown could not tell apart. Keys: Ctrl+F3 toggle, PageUp/PageDown + ↑/↓ move, ←/→ category,
  Enter select. **Needs a game RESTART** (main.lua + a rebuilt DLL). Keys settled with the user:
  **V** toggles the picker, ↑/↓ move, ←/→ change category, Enter selects — plain unmodified keys,
  which only became safe once the keyboard block below existed (V is the skill palette and the
  arrows are mount/dismount in the game's own layout). The opening keypress is swallowed too: the
  lease is taken BEFORE `do_open`, which is safe here precisely because it expires by itself (the
  pad block is taken after, so a fault in the world read can never strand it). The **world map's
  travel list** takes the same commands (`screen_map.Map.key`, consumed in `ft_guidance` above the
  no-pad early return), so ↑/↓ act as the d-pad there and Enter travels. One dispatcher,
  `App.nav_key`, feeds both — they can never be up at once (the picker needs free roam).
- **The keyboard is now hidden from the GAME while the picker is open**, the exact twin of the
  pad block (user asked "¿no podemos hacer lo mismo que con el mando?" — yes, and it removes the
  ↑/↓ mount-dismount conflict entirely instead of documenting it). Two facts had to be settled
  first, and both were, from sources rather than assumption:
  1. **UE4SS POLLS `GetAsyncKeyState`** on its own `UE4SS-UpdateThread`
     (`deps/first/Input/src/Handler.cpp:91/108/135`, v3.0.1; `UE4SSProgram.cpp:922` at ~200 Hz).
     It never reads window messages. So draining the game's message queue cannot break our own
     keybinds — which is what makes this safe at all, since the key that CLOSES the picker is one
     of them. Corollary: **NEVER hook `GetAsyncKeyState`** — that is precisely what UE4SS polls,
     and hooking it would silence the mod process-wide.
  2. **The exe pumps with `user32!PeekMessageW`** (6 call sites; `GetMessageW` is not imported)
     and registers RawInput for the **mouse only** (its single `RegisterRawInputDevices` passes
     usUsagePage=1/usUsage=2), with no DirectInput. So every keystroke arrives as a posted
     `WM_KEY…`/`WM_CHAR` and draining the pump hides the keyboard completely.
  Implementation: `input_bridge.kb_block(ms)` + an IAT hook on `PeekMessageW` that removes and
  discards keyboard messages while the lease is live (three of the exe's peeks are PM_NOREMOVE,
  so a filter that merely hides them would dump the whole burst on the game at unblock).
  **It is a LEASE, not a latch** — a wall-clock deadline renewed every 20 ms tick by
  `radar_menu.step`; if the mod dies mid-block the keyboard frees itself in ~300 ms instead of
  leaving the player unable to press anything. Alt+F4 is never swallowed, and the module is
  PINNED (`GET_MODULE_HANDLE_EX_FLAG_PIN`) because the IAT slot points into our DLL.
  Trap that cost a build: `WM_KEY*/WM_CHAR` inside a C block comment — the `*/` closes it.

**2026-07-25 (k): two user reports — one FIXED centrally, one waiting on a single capture.**

1. **Notices cut off mid-sentence** (user: the map's "zone available" popup and the skill-tree
   acquire / "what you need" popups get interrupted, in the tree by the focused node being
   re-spoken). Mechanism, no guessing needed: `screen_dialog` speaks a notice with
   `say_protected` (interrupt=true **+ a protection window**) and then RELEASES the screen on the
   very next poll; the adapter below re-commits, the registry calls its `reset()` — which clears
   `Announcer.open` — and the next `focus()` takes the "entering the screen" branch and speaks
   with `interrupt=true`, **which clears the reader's whole queue**. `Speech.protected()` was
   written for exactly this and NOTHING consulted it except two hand-wired adapters
   (`quest_objective`, `screen_community`). Fixed in the shared substrate, not per screen:
   `Announcer:focus` and `keyhelp_watch.update` now DEFER while the window is open (both are
   polled every tick, so the readout fires with the CURRENT state the moment it clears; bounded
   by `speak_seconds`' 6 s cap). Source-only, luac + globals-lint clean, **unverified in game**.
2. **The button-config rows announced the DEFAULT button after a remap — FIXED, and the F7
   capture settled it in ONE pass** (the rule earns its keep again: three sessions of guessing
   were on the table, one dump answered everything). What the dump
   (`dumps/dump_keyconfig_1784989557_001.txt`, taken with melee on X and ki on B) proved:
   - The row markup is **`KeyConfig_Controller_Btn_B`** for the melee row *even after the
     rebind* — because every controller id in this game names a **SLOT**, and a slot keeps its
     factory name forever. `row_binding` faithfully spoke the slot: "botón B".
   - The asset is **not** rewritten on a rebind. All 68 live-vs-default differences are the same
     one thing — the game fills `DynamicAssignInputControllerId` in at runtime, and
     `DefaultKeyConfigList` simply has it blank. `Battle_Attack` still pairs with slot
     `Controller_Btn_B`. So no cache TTL or `clear_binding_cache` timing could ever have fixed
     this; the data source was wrong.
   - **`UATSaveSystem.InputAssign` IS reflected** (60 members, read off the user's real
     `ATSaveSystem_4`) and carries the live layout: `Controller_Btn_B = Gamepad_FaceButton_Left`
     (physically X), `Controller_Btn_X = Gamepad_FaceButton_Right` (physically B) — exactly the
     swap the player made. The 48 action FNames alongside them are the **keyboard** side
     (`Battle_MeleeAtk = LeftMouseButton`), which is also the first time the keyboard binding has
     ever been readable in this project — the old "keyboard key is NOT recoverable" note in
     [input-icons-and-keyconfig.md](reference/dbz-kakarot/input-icons-and-keyconfig.md) is
     obsolete and a keyboard readout is now possible if wanted.
   Fix: a slot→physical layer in `ui_archetypes` (`A.physical_token`, save-backed, 0.5 s TTL,
   fail-open) applied in the one funnel every controller id passes through, `A.button_name`,
   plus the three paths that return a raw token. Identity under a default config.
   Source-only, luac + globals-lint clean, **unverified in game**.

**2026-07-25 (j): the bubble trace settled two things and left ONE open — and the open one needs a
dump, not another guess.** The log for the Krillin scene contains exactly ONE `bubble[…]` line in the
whole session — `bubble[EventSpeechWidgetArray]: ¡La carne de bestia que puedes obtener de estos
animales es la mejor!` — with the markup correctly stripped (so that fix works), zero errors, zero
gate/pre-check rejections. And the player heard THAT line spoken while Krillin was talking.
Therefore:
1. **A spent bubble lingers with its text and `IsEnd()` does not reject it.** That stale hint was
   being re-announced whenever the adapter re-committed. Fixed by requiring `Core.pane_live(core)` —
   these cores fade out through `AnimOut`, so a spent one drops below the opacity floor even while
   its visibility flags lag. This is the project's OWN pooled-pane rule (CLAUDE.md §8) and I simply
   did not apply it to a new adapter. That is the third time today the same rule caught something.
2. **Krillin's line is on a surface still not found.** It never appears in the trace, so the bubble
   surface never sees it. Static data is exhausted: there is no phone/Denwa/Keitai class anywhere in
   the headers; `UATUISubtitles` (AT.hpp:29657) has a single line node (`TextSelif` @0x400 +
   `TextName` @0x3F8), so it is not a split-conversation widget; the only other `Selif`/`Msg` owners
   are `UAT_UIFieldMemory` (story recap) and the Dragon Ball menu.
   Closed one real gap found while looking: `screen_dialogue` searched ONLY the Blueprint class
   `Xcmn_Subtitles_C` with Blueprint member names, so a subtitle instance of a different BP subclass
   — or of the native class itself — was invisible. It now also tries `ATUISubtitles` with the native
   `TextName`/`TextSelif`. Cheap and correct, but NOT a claimed fix for Krillin.
**WHAT IS NEEDED: an F7 dump taken WHILE Krillin's line is on screen.** The census section names the
widget path outright — that is exactly how the defeat menu was solved in one shot after three rounds
of guessing. Until then this stays open; three guesses is enough.

**2026-07-25 (i): bubbles now READ (the new surface works) but two mistakes of mine were in the way.**
Evidence: a bubble spoke — and spoke the raw tag: `¡La <span color="#ffba00ff">carne de bestia</> que
puedes obtener…`. So the surface is live; the faults were elsewhere.
1. **`node_text` never ran the markup stripper on the PLAIN side.** It returned `Core.text_of(node)`
   untouched, on the assumption that only the RICH node carries tags. False — and no surface had
   exercised that branch until the bubbles arrived. The two branches below it already called
   `A.markup_to_speech`; the plain one simply did not. Fixed (with the raw text kept as fallback,
   since markup_to_speech returns nil when it cannot parse).
2. **I excluded the wrong array, and that cost the target line.** The first cut read
   `EventSpeechWidgetArray` only, skipping `FreeTalkWidgetAry` as "ambient chatter that would talk
   over the story". Krillin's conversation — which the player describes as being "on the phone" —
   still did not read. There is NO phone class anywhere in the headers; what exists is
   `SpecialFrameBorder` / `SpecialIconImage` on the bubble core plus
   `RequestOpenEventSpeak(…, bool bInUseSpecialFrame, …)`, i.e. **the "phone" is a special-framed
   bubble**, and the game routes it through the other array. Now BOTH arrays are read, event speech
   first. LESSON: pre-empting a hypothetical noise problem by dropping a whole data source cost the
   exact line the work existed for. Throttle with evidence, after seeing the noise — not before.
**New decisive artefact: `bubble[<array>]: <text>` in the log** — every DISTINCT bubble line seen,
capped at 20. If a line the player did NOT hear appears there, the surface saw it and the fault is
downstream (priority / announcer / dedup). If it never appears, there is a FOURTH surface. Either way
the next report is one grep instead of another guess.

**2026-07-25 (h): "Gohan's lines read, Krillin's did not" — NOT a regression. The mod knew about TWO
dialogue surfaces and this game has THREE.** The log for that session is completely clean: zero
errors, zero `member gate:`, zero `array gate:`, **zero `memory pre-check:` rejections** (the reject
log now prints the first 5, so even one would show). That exonerates the whole 07-25 read-path batch
and pointed at a gap instead. Found in the header dump:
- `UAT_UIFieldTalkWin` (AT.hpp:33229) — covered as `Field_Talk_Win_C`.
- **`UAT_UIFieldTalkFree` (AT.hpp:33181) — was NOT covered.** Reached by pointer:
  `UAT_UIFieldManager.FieldTalkFree` @0x658, now mapped in `ui_directory`. Holds TWO TArrays of
  **`UAT_UIFieldTalkFreeCore`** (AT.hpp:33202) whose `TextBox` @0x3E0 carries the line:
  `EventSpeechWidgetArray` @0x458 and `FreeTalkWidgetAry` @0x3A0.
`screen_dialogue` now reads **`EventSpeechWidgetArray` only**, checked LAST (a formal window or a
cutscene subtitle is always the more important surface). Deliberate scoping: the event array is fed by
`RequestOpenEventSpeak(… FName InSpeakerUniqueId, AQuestCharacter* InOwner, FString inString …)` —
quest/script-driven speech, which is Krillin's story line — while `FreeTalkWidgetAry` is ambient NPC
chatter that would talk OVER the story through an interrupting reader. Ambient stays unread until
asked for. Liveness = the core's own reflected **`IsEnd()`**, with `on_screen` only as the fallback
when IsEnd is unreadable — `on_screen` has now twice rejected hosts that were plainly up. These lines
read BARE: the core has no speaker node, the speaker is an ICON via the parent's `SpeakerIconMap`
(FName→Texture2D); naming them would need the `CHAR_TOKENS` icon-mapping trick from
`screen_community` and was not attempted. **This is very likely also the answer to the unread
"telepathic message" content** — same CrossTalk/ambient family.
**Two dev KILL SWITCHES exist now for bisecting the read path** (state in `_G`, survive Ctrl+Shift+R):
**Ctrl+Shift+G** = memory pre-check off/on, **Ctrl+G** = the two reflection gates off/on. Together they
restore pre-07-25 behaviour exactly. The pre-check reject log now carries `addr=0x… cls=…` (first 5,
then every 200): a REPEATING address means a live object refused (false positive); `cls=0` on a
plausible address would mean `GetAddress()` is not a UObject base and the whole check is misconceived.

**2026-07-25 (g): STORY DIALOGUE STILL SILENT after the Options fix — my Options theory was WRONG,
and the real symptom shape is "works ONCE, then never again".** User: the FIRST story line read
("Gohan: ¿Krillin? ¿Qué sucede?") and no later one did; in Options the save CONFIRM reads but the
following "all changes saved" notice does not. That pattern spans TWO different adapters
(`screen_dialogue` for story, `screen_dialog` for the Options notices), so the cause is almost
certainly in something SHARED that starts refusing after first use — i.e. in what the 07-25 batch
added to the read path: `Mem.alive` (via `Core.valid`), the `Core.member` property gate, the
`Core.array_of` type gate, or `Core.nonnull`.
The prime suspect is **`Mem.alive`**, because its failure mode is invisible: refuse a live object and
whatever was reading it goes quiet with no error, while the log shows a rising "N rejected" count
that is indistinguishable from the guard working (the user's session logged 401 — could be all real,
could be all false positives; the count cannot tell).
**Two KILL SWITCHES now bisect it in one press each** (dev keys, state in `_G`, survive Ctrl+Shift+R):
- **Ctrl+Shift+G** → memory pre-check (`Mem.alive`) off/on. Says "Memory pre-check off".
- **Ctrl+G** → the two reflection gates off/on. Says "Reflection gates off".
Together they switch off everything this batch added to the read path, restoring the pre-07-25
behaviour exactly. **Procedure:** with dialogue silent, press Ctrl+Shift+G. If it comes back, the
pre-check is the cause. If not, also press Ctrl+G. Whichever restores it names the culprit.
**The rejection log now carries evidence, not just a count**: `memory pre-check: N rejected
(addr=0x… cls=…)`, first 5 then every 200. A REPEATING address = a live object refused over and over
(false positive). `cls=0` on a plausible address = `GetAddress()` is not pointing at a UObject base,
which would make the whole pre-check wrong rather than unlucky. Both readings are decisive; neither
requires a dereference.
**JUDGEMENT if the pre-check is the culprit:** turn it off and leave it off. It guards a crash class
that existed for months; losing story dialogue is a total loss of the mod's core function. Fix the
guard properly (class-pointer stamp) before re-enabling it.

**2026-07-25 (f): I CAUSED A REGRESSION AND A CRASH — both from one change, both fixed. Read this
before touching `screen_options` again.** Round 1 of the Options re-entry fix made `refresh_rows`
run whenever the staleness test failed. That test requires `on_screen`, and the pooled
`Start_Option_C` host lingers `on_screen` forever after the menu closes — so **a once-per-session
`FindAllOf` became a ~65 ms full-object scan EVERY TICK.** Consequences, both reported by the user
within minutes: story dialogue went silent (it is scan-detected, and the shared 2-per-tick budget was
drained) and the game crashed with the 2026-07-21 dangling-UObject signature (UE4SS +0x1482b/+0x1449b
/+0xe89e, faulting address `0xdb6a46e8` = recycled garbage — the case `Mem.alive` documents that it
cannot catch). A full object-array walk every 100 ms is the per-tick-retry escalation the ledger has
warned about since 2026-07-13. **The tell that it was game state, not mod state: it survived a mod
reload** — a parked pooled widget belongs to the game.
Fixed two ways: `screen_options` gates the host on `Core.pane_live` as well as `on_screen` (it was
the only adapter of its family violating the §8 pooled-pane rule), and `refresh_rows` got an explicit
`RESCAN_EVERY = 10` backoff. **RULE, now in CLAUDE.md: a scan SLOT is not a rate limit** —
`take_scan_slot` apportions 2 per tick between competing callers, it does not stop one caller asking
every tick. Anything callable per tick needs its own backoff as well.
**NEXT DURABLE STEP (the residual crash hole is now precisely known): the CLASS-POINTER STAMP.**
Record each cached object's `ClassPrivate` when it first validates, and require it to still match on
later use. A recycled address holding a DIFFERENT class is then rejected; recycled-into-the-same-class
stays readable, which is safe. This is the one case `Mem.alive` cannot see today.
**ALSO READY, NOT APPLIED (deliberately held while the above is verified):** the banquet stat-increase
dialog. Identified from the user's screenshot + F7 dump: class **`Shop_Cook_Result_C`**, nodes
`Txt_Title` ("Aumento de estado"), `Txt_Up00/01/02` ("Los PS aumentan 1625" / "El Ki aumenta 1" /
"El ATQ cuerpo a cuerpo aumenta 10"), text carries `<span color=…>` markup so it needs
`A.markup_to_speech`. **No adapter exists for it** (grep: zero hits) and it is NOT directory-mapped;
`UAT_UIFieldManager` has `CookingMenu` @0x530 but no cooking-RESULT pointer, so it needs the notice
pattern with a scan, registered ABOVE `screen_cooking` (the pooled cooking pane lingers on_screen and
would shadow it — the `screen_fishresult` lesson) and gated on `pane_live`. Sibling
`Shop_Cook_Comp_C` also appeared in the census and is probably the "dish complete" screen.

**2026-07-25 (e): the DEFEAT MENU is SOLVED by the F7 owner hunt, and it also explains the two new
symptoms the user reported (result screen silent + audio cutting in cutscenes).**

*Defeat menu — everything was reflected all along, and the game hands us a pointer.* The F7 probe
(dump_1784980443_002.txt lines 12-33) printed:
`HOST …BP_ATGameInstance_C_0.Gameover_C_0  vis=false enum=1:Collapsed op=1.0` /
`List_Bar00/01/02 = "Reintentar" / "Cargar" / "Volver al título"` /
`SelectionWidgetArray = 3` / `CurrentSelectIndex reflected=0` / **`OWNER FIELD:
AT_UIFieldManager.Gameover -> this host`**. So `Gameover_C` is now mapped in `ui_directory` as
`{"fm","Gameover"}` — detection is two guarded pointer reads, no FindAllOf, no backoff, no watch
lane, immune to quiet mode. `screen_gameover` lost the armed window, the watch and the arming
signal entirely: it peeks the directory list each tick and picks the instance that passes
`Core.pane_live`. Liveness is `pane_live`, NOT `on_screen`: the parked host reads
`enum=1:Collapsed`, which pane_live rejects cleanly, while `on_screen` rejected the host even with
the menu UP (it also demands the root UserWidget's `IsInViewport`, and this widget is shown through
the field manager). The native `mem_bridge` offset stays only as a fallback. Selection index and
rows are all reflected. Also note the member gate earned its keep here: `member gate: Gameover_C
has no 'List_Bar03/04/05'` — three fetches of a member the class does not declare, refused.

*The audio cutting every ~1.5 s in cutscenes, and the result screen going silent: SAME cause, and
it was mine.* Making the battle-HUD edge unconditional in `ui_registry` exposed that
`battle_hud_live()` **FLAPS** — the pooled HUD collapses/uncollapses during cutscenes and battle
phases. The user's log proves the cost: **124 `watch Gameover_C` lines** where one 20 s window at
`WATCH_EVERY` should give ~25, i.e. the watch was re-armed ~5 times, each re-arm running a ~65 ms
`FindAllOf` every 0.8 s (audible as the stutter) and eating the 2-scans-per-tick budget the other
screens live on. Each false edge also opened a 3 s `hot_until` window, defeating cinematic quiet
mode — the exact hitching quiet mode was built to remove. Fixed two ways, both of which stand on
their own: the falling edge is now DEBOUNCED (`ABSENT_TICKS = 5`, ~500 ms of continuous absence;
appearing is still taken at face value), and mapping `Gameover_C` means a watch on it can never
cost a scan at all. `_G.__KakarotBattleLeftAt` removed — screen_gameover was its only consumer.
*The RESULT screen: the same flapping shape, one layer down — in the new transition gate itself.*
`Quest_Main_Clear_C` and `Battle_Result_C` are both directory-mapped, so scan starvation could not
be it. The mechanism found instead: **while `Transition.active()`, `Directory.resolve` returns an
EMPTY list for EVERY mapped class** (ui_directory: "resolve to absent, NOT fallback" — deliberate,
so no scan runs mid-load). So one spurious arm silences the whole directory for `GRACE_S`. And
`note_epoch` armed on a SINGLE nil read, with every later nil re-extending the grace — precisely the
flapping-falling-edge bug that had just been diagnosed and fixed for `battle_hud_live()` in the same
batch, left unfixed in the more dangerous place. One flicker of `Core.valid(w)` around a battle
ending (streaming + camera work) is enough. Fixed: the "no world" answer must PERSIST
`DOWN_CONFIRM_S = 0.3 s` before it counts, measured in WALL TIME because `note_epoch` runs from
every loop several times per tick (a call counter would confirm in milliseconds and confirm
nothing); an epoch CHANGE is never debounced, since two different valid worlds is unambiguous. The
0.3 s of ticks against a dying world at a real map switch is affordable only because `Mem.alive`
now makes those reads survivable — that trade did not exist before today.
**`begin_transition` now LOGS its trigger** (`transition gate ON (world gone|new world)`). A false
arm was indistinguishable from a real one, which is why this took a round to find; transitions are
rare, so the line costs nothing. If a screen goes quiet again, count these lines first.
*Ruled out for the result screen, with reasons:* the `Core.member`/`array_of` gates (they log every
first rejection, and the log shows those lines only for `Gameover_C`); the `valid_ref` fix (none of
the 510 tracebacks named these classes); `Core.nonnull` failing closed (it feeds only the rank/EXP
TEXTURE names, and neither adapter's `is_active` depends on it — so it could cost content, never
total silence).
*CHARACTERS menu — moving between characters now narrates (user request 2026-07-25).* Cause: the
adapter read `host.WL_StartCharBarList`, and its header ASSERTED that UE4SS's fixed-array collapse
yields "the currently shown bar". It does not — the collapse yields **element 0**. With one
character that is the same thing, which is why it looked verified; with several, nothing in the code
could ever change what it read, so the diff gate swallowed every cursor move. No new capture was
needed: the CXX header had it (`AT.hpp:36402` `UAT_UIStartCharBar* WL_StartCharBarList; // 0x03B0
(size: 0x30)` → base **0x3B0**, 0x30/8 = **6** slots; `AT.hpp:36433` gives the bar's members,
including the cursor markers `UBorder* Pnl_Curs_All` @0x410 and `UImage* Img_Curs00` @0x438).
Fix = the screen_party recipe: each slot's pointer registered with `RegisterCustomProperty` against
the host's RUNTIME class path (a BP subclass, so the native class name would not match — the
screen_community approach), declared to the member gate with `Core.allow_member`, and the selected
bar found by its **cursor marker** rather than by arithmetic. Marker-first is deliberate: the host
exposes `SetViewIndexAndCursorIndex`/`GetViewIndex`/`GetCursorIndex`, i.e. the list is VIRTUALISED,
so the cursor index is absolute over all characters while the bars are only the visible window —
index maths would need the scroll offset and break at the ends. `GetCursorIndex() - GetViewIndex()`
is kept as the fallback, and slot 0 as the last resort, so the screen can never read LESS than it
did before. Guest characters read `Txt_Name_Guest` when `Txt_Name` is empty.

**2026-07-25 (d): the batch WORKS in game (user: "funcionó aparentemente") — only the DEFEAT MENU
is still silent, and the log narrowed it to one signal.** The retest log (boot 11:32:56) shows the
new gate lines and, crucially, **zero `polymorphic type` errors, zero `member gate:`, zero
`array gate:`** — the pierce is gone and the reflection gates are rejecting nothing. User is
continuing to play to see whether the crashes stop.
*Defeat menu, what the log proves:* `watch Gameover_C: 1 found` repeats every ~0.8 s for the whole
~20 s window, so the watch lane WORKS and the class IS found — but the adapter never reached
`Core.watch_clear`, which means it bailed on **`Core.on_screen(host)`**. The F7 dump could not say
which signal inside that walk said no: its GAME OVER section ENDS at the header, i.e. the block
died between the `flush()` after the header and the next one 35 lines later (an abort there pierces
pcall, so nothing was logged). Applied:
1. `screen_gameover` no longer uses `Core.first_on_screen` (which demands the ancestor walk +
   root `IsInViewport`). It takes the pool from `Core.cached_all` and picks the instance that is
   `Core.pane_live` (ESlateVisibility Visible AND RenderOpacity > 0), preferring one that also
   passes the full walk when there is a choice. Staleness is already excluded twice here — by
   `pane_live` and by the ~20 s battle-end window — so the walk was pure loss. The row readers
   (`items`, `selected_label`) likewise moved from `on_screen` to `Core.is_visible` (the row's own
   flag), or they would fail for the same reason the host did.
2. **One-shot probe line** (`gameover probe:`) printing, per pooled instance, `valid / on_screen /
   IsVisible / GetVisibility enum / RenderOpacity / IsInViewport`. Next defeat names the culprit
   signal outright — no dump needed.
3. `discover.lua`'s GAME OVER section is now **step-marked with a flush after every risky call**
   (`[step] …` lines), so if it dies again the file itself says where. Prime suspect flagged in the
   code: `SelectionWidgetArray` may be a FIXED C array, and `GetArrayNum` on one is the 2026-07-16
   pierce — `Core.array_of` refuses non-ArrayProperty members now, but only once the class's
   property set exists, and F7 runs outside the poll where that budget is refilled.
4. **Where the dump died is now KNOWN** (log 11:40:55): `Error: Tried calling a member function but
   the UObject instance is nullptr`, `[C]: in method 'IsVisible'` ← `discover.lua:125 isvis` ←
   `discover.lua:453`. So it was NOT the array — it was `isvis(bar)` on a `List_Bar0N` that is
   **non-nil but wraps a NULL UObject**. Two consequences. (a) The dev tool had its OWN private
   helpers (`local valid = o ~= nil and o:IsValid()`, plus `isvis`/`vis_enum`/`opacity`/`color_a`
   with a bare pcall) that bypassed the mod's guards — all now go through `Core.valid` first, which
   runs the memory pre-check that catches a NULL-wrapping handle. **A diagnostic that can die is
   worse than useless: it destroys the evidence it exists to collect.** (b) The real finding:
   `List_Bar0N` ARE declared properties on `Gameover_C` but are **NULL on the instance FindAllOf
   returns** — so that single instance is very likely a parked/unpopulated one, not the menu on
   screen, which is consistent with `on_screen` rejecting it. If the probe confirms that, the
   displayed defeat menu is a different instance or a different class than the 07-24 census
   assumed, and the owner-field hunt in the F7 section (now reachable, since the block no longer
   dies before it) is the way to find it.
**Next defeat: send the `gameover probe:` lines, and press F7 for the new step-marked dump.**

**2026-07-25 (c): THE SILENCED MENUS ARE EXPLAINED AND FIXED — from the user's UE4SS.log, not from
reasoning.** The user retested and reported 7 things still silent (load/save data, community
emblems, Options on RE-ENTRY, the Options "save changes" dialog, skill tree, super-attack palette,
character stats on the d-pad). The log had the answer in one line, repeated 510 times:
`Error: Call to RemoteObject:GetAddress on polymorphic type is not allowed`, with a traceback
`mem.raw_addr → mem.alive → ui_core.valid → ui_core.array_of → <adapter>.update`.
**`GetAddress` is overridden only on the UObject family.** On a TArray wrapper UE4SS raises, and
that error **PIERCES pcall** — it unwinds to UE4SS's callback boundary, so the adapter's function
dies mid-flight while every enclosing pcall reports success. Every one of the 510 entered through
`Core.array_of`'s validity check ON THE ARRAY, and the traceback frame counts name the user's list
exactly: screen_saveload 1248, screen_status 1104 (the d-pad stat walk), nav_tracker aim_watch 468,
screen_skillcustom 208, screen_community 148, screen_tutorials 136, keyhelp 102, screen_skilltree
88, screen_dialog.choices 56 (= the Options "save changes?" confirm). The 07-24 batch put the same
call in `Core.valid`, so this is what silenced them that day too — **my two earlier diagnoses of
this regression were wrong about the mechanism**, and the ledger now records that.
FIX: **`Core.valid_ref(o)`** — `IsValid()` only, no address — used by `Core.array_of` for the ARRAY
(the owner stays on the full check; `IsValid` on a non-UObject wrapper is a bare null-check, so it
is safe, and a dead owner is already caught one line earlier). Plus a **self-disabling guard** in
`Mem.alive`: the attempt is marked pending in `_G` before the call and cleared after, so a future
stray pierce costs one tick and a log line instead of a broken mod. Boot diagnostics from that
session all came out right: `mem_bridge loaded`, `world-epoch poll`, `UObject class pointer at
+0x10 (derived)`, and one `memory pre-check: 1 dead handles rejected` — the crash guard is working.
**Options on RE-ENTRY: separate cause, FOUND AND FIXED.** `screen_options` appears ZERO times in
the tracebacks, and it was its own bug: the adapter hand-rolls a module-level `rows` cache (it has
to filter the shared `Xlist_Bar03_C` pool by owner), its staleness test asked only
`Core.valid(rows[1])`, and `Options.reset()` never cleared it. **This game only COLLAPSES a
submenu's widgets on close, it does not destroy them** — so visit #1's rows stayed valid forever,
the staleness test passed, `A.scan_list` found no visible `Ins_Cursor_Fad` on the orphaned rows,
`low` was nil on every tick, and `update()` returned before ever reaching the announcer. First
entry worked only because the cache starts empty. Fix: the staleness test now also requires
`Core.on_screen(rows[1])` (the same check `Core.first_on_screen` makes for every other list
screen), `reset()` clears `rows`, and `refresh_rows` clears the list first and takes a
`Core.take_scan_slot()` — its raw `FindAllOf` is ~65 ms and the test can be false for several
consecutive ticks. **SWEEP DONE (all 46 adapters + helpers): `screen_options` was the ONLY one.**
Do not re-run it. Every other adapter reacquires its host each tick through
`Core.first_on_screen`/`Core.cached_live`, which do the on-screen check internally. Three files
have a hand-rolled cache with a `Core.valid`-only staleness test but are NOT vulnerable, because
the cache is cleared on every genuine close — verified, so nobody "fixes" them later:
`screen_shopcmn` (`rows` nil'd at :71 the tick `host` goes nil, and in `reset()` at :95),
`screen_palette` (`nodes` nil'd in `reset()` at :173), `screen_community` (`panel_cache` cleared by
`clear_state()` from `reset()` and from the per-tick mode-change branch, plus on board-title change).

**2026-07-25: ROOT CAUSE OF THE CRASHES FOUND IN THE UE4SS SOURCE + the 07-24 hardening's
regressions fixed. Source-only, luac-validated, NEEDS A FULL GAME RESTART AND AN IN-GAME TEST.**

*The crash.* `IsValid()` does not lie, it **faults**: RE-UE4SS v3.0.1 `LuaUObject.hpp:610` runs
`!m_cpp_object->IsUnreachable()` — a read of the object — **before** the object-set lookup that
could have caught a freed handle, and UE4SS never clears the raw pointer inside a Lua handle
(`LuaUObject.cpp:59-66` only erases a hash). So the access violation lands inside the guard
itself, at `GetClassPrivate()` = UObjectBase+0x10 — the faulting address in every user report.
Fix applied: **`Mem.alive(obj)`** (mem.lua) — ask the handle only for its stored pointer
(`GetAddress`, the one method that does not dereference), then read the object's class pointer
and that class's class pointer **through mem_bridge**, whose reads are SEH-guarded and return
nil instead of faulting. `Core.valid`, `Core.nonnull` and `Mem.addr` all go through it, so it
covers every reader and every native offset read. The `ClassPrivate` offset is DERIVED at
runtime (UClass by path → its class pointer by reflection → find that value in the object's
first bytes), never hardcoded. Fails OPEN everywhere. **Also settled: `bUseUObjectArrayCache =
false` is a DEAD END** — it never touches `IsValid`, is scoped to startup crashes, and breaks
Lua `IsA` (issue #772). Do not spend a session on it.

*The regressions* (user: "se dejaron de leer algunos menús"): (1) `Core.valid`'s 07-24 gate
rejected a handle whose `GetAddress()` answered NIL — and `ui_directory` gates EVERY pointer hop
on `Core.valid`, so one unanswerable hop makes a directory-mapped screen resolve to nothing and
go silent with no error anywhere; `AT_UIStartSaveLoad` is directory-mapped, hence the load-game
screen. Now fails open on "don't know". (2) `screen_dialog`'s `pinned_set` was EPOCH-scoped, so
any notice that legitimately repeats inside one map was mute for the session — the Options save
confirmations; now scoped to the window presence (released after 2 s of continuous absence).
(3) `screen_gameover` probed the unmapped per-level `Gameover_C` on every poll from registry
slot 4 (a `FindAllOf` every backoff forever + the 2026-07-17 "never probe a per-level pool
unconditionally" rule); now only inside the ~8 s window armed by the battle-HUD falling edge.

*The transition gate is no longer a notify.* The `NotifyOnNewObject("/Script/Engine.GameModeBase")`
gate is GONE — it was the last construction notify in the mod, i.e. the last place mod Lua could
run on the engine's async loading thread and race the poll loop on the shared `lua_State` (the
measured 2026-07-14 corruption). It also armed too LATE: the GameMode is constructed after the old
world's objects are freed, and our tick DOES run in between (LoadMap fires dozens of UFunctions
through ProcessEvent, each draining our queue). Replaced by **`Directory.world_epoch()`** — the
GameInstance (never dies, cached after one lookup) → `GetWorld()` → its address — polled by
**`Core.poll_world()`**, called from `Core.begin_scan_tick` (so every loop) and directly by
nav_tracker (the only loop touching UObjects outside it: with the reader toggled off the registry
loop stops and the gate would otherwise never arm). Epoch changed, or a world that cannot be read
⇒ gate ON + flush. Deliberately NOT the PlayerController: this game has several and they swap
mid-world when mounting, which would flush caches spuriously. A 60 s failsafe releases the gate if
no world is ever found — a mute mod is worse than an unguarded one, especially now that the memory
pre-check protects the reads.

*Property-existence gate in `Core.member`.* Fetching a member the class does not declare is an
uncatchable abort (it killed the game on 2026-07-17: `bar.Txt00`, a member of the SIBLING bar
class). `pcall` cannot catch it, so the fix is not to ask: per-class property-name sets built with
`UStruct:ForEachProperty` + a `GetSuperStruct()` walk (ForEachProperty lists a class's OWN
properties only), cached by class address, cleared on transition, at most one class enumerated per
tick, fails open. Blocked fetches are logged (capped at 50, class + member named) so an over-eager
rejection is visible instead of silent. **`RegisterCustomProperty` members are invisible to
ForEachProperty** — screen_party's party slots 1/2 and screen_community's skill parts 1..9 (both
recovering collapsed fixed C arrays) now declare themselves with `Core.allow_member()` at
registration; anyone adding a custom property must do the same.

*Adversarial review of the same batch (opus, independent) found 6 real defects — all fixed before
this was written down.* Worth recording because they are the recurring shapes: (1) the
forward-reference that would have muted the mod from boot (see the crash ledger's new lint step);
(2) the epoch poll took a scan slot AHEAD of the watch lane, the 2026-07-16 starvation bug again —
now polled after it, and `world_epoch` answers `false` ("could not look") instead of `nil` ("no
world"), so a spent budget can no longer arm the gate; (3) `class_off`'s retry budget was per-CALL
and `Mem.alive` runs dozens of times per tick, so all 20 tries burned inside one tick — now
time-based (0.5 s apart, 30 s deadline) and it LOGS the give-up, which the first cut never did;
(4) `prop_budget` was refilled only in `begin_scan_tick`, which nav_tracker never calls — so the
member gate was permanently open on the pooled minimap icons that level streaming frees, the
highest-exposure path; refilled in `Core.poll_world` instead; (5) the defeat menu's arming watched
the battle HUD from inside its own adapter, but the registry is STICKY — dying in a battle with
subtitles up meant `is_active` was never called across the falling edge and the menu would be
silent forever; `ui_registry` now tracks that edge unconditionally and stamps
`_G.__KakarotBattleLeftAt` (which also survives a hot reload), window widened to 20 s; (6) stale
comments. Two limits are ACCEPTED, not fixed, and say so in the code: a class's FIRST member fetch
is ungated (one set built per tick), and a map switch is missed if no poll lands while the world
pointer is null AND the new UWorld reuses the freed address.

*Kill switch = **Ctrl+G** (dev keybind).* Flips BOTH reflection gates (member existence + array
type) off/on in game, restoring the pre-07-25 behaviour exactly — because the failure mode of a gate
on this path is a screen going quiet with no error, and whoever is testing must be able to rule it
out in one press. State lives in `_G`, so it survives Ctrl+Shift+R (a plain local would have turned
the gates back on at every reload — the opposite of what a diagnostic switch is for). The memory
pre-check (`Mem.alive`) is deliberately NOT behind it: it fails open on every "don't know", has no
silent-failure mode of that kind, and is the actual crash fix.

*What Ctrl+Shift+R does and does NOT reload (verified 2026-07-25 against `main.lua:39-43,101-109`).*
The reload drops every module NOT in the protected snapshot and re-requires `app.lua`. Protected =
everything loaded before line 43: `speech`, **`mem`**, `audio`, `input`, `settings`, `build_flags`
and their bridge DLLs. So:
- **Reloadable** (Ctrl+Shift+R is enough): `ui_core`, `ui_registry`, `ui_directory`, `transition`,
  `ui_archetypes`, `i18n`, every `screen_*`, `keyhelp*`, `nav_tracker`, `discover`.
- **Needs a FULL RESTART**: `main.lua` (keybinds) and **`mem.lua`** — it is protected precisely so
  the bridge handle and the derived class-pointer offset survive reloads, which means edits to it
  are invisible to a reload. The 07-25 batch touches both, so the first run after it must be a full
  restart; after that, Ctrl+Shift+R covers the adapters.

**VERIFY (needs a full RESTART — mem.lua/ui_core/transition changes):** boot log must show
`UObject class pointer at +0xN (derived)` and `Transition gate: world-epoch poll (game thread, no
notify)`; load-game screen and the Options save dialogs read again; a repeated save dialog reads
EVERY time; map changes and a return to title still flush cleanly (no crash, no stale reads);
play a long session incl. combat with dialogue and cooking browse. Report any
`memory pre-check: N dead handles rejected` (the guard catching real crashes) and any
`member gate: <Class> has no '<Member>'` line (a possible over-rejection to look at).
**FREE DIAGNOSTIC WE HAVE NOT BEEN USING:** the shipped `UE4SS-settings.ini` has
`[CrashDump] EnableDumping = 1`, so **every crashing user already has a `.dmp` next to
`AT-Win64-Shipping.exe`**. Ask one of them for it — it replaces all the stack-offset archaeology.
*The fixed-C-array pierce is guarded too, for free.* The property walk records each member's
PROPERTY TYPE (it already had to ask `prop:GetClass():GetFName()`), and a real TArray is an
`ArrayProperty` while a fixed C array is a single ObjectProperty with ArrayDim > 1. So
`Core.array_of` now refuses any member the class declares as something other than an
ArrayProperty — the 2026-07-16 crash mode whose own code comment said "there is no runtime check
for this: the caller must never pass a fixed-array member here — check the CXX header dump".
There is one now, and it comes from the engine's metadata instead of from a human remembering.
Blocked reads log `array gate: '<Member>' is a <Type>, not a TArray`.

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
