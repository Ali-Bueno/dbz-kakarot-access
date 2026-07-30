# dbz-kakarot-crash-bug

> **2026-07-29 (d) — THE BLACK BOX WAS DECODED OFFLINE FOR THE FIRST TIME, AND IT MEASURED THE
> EXPLORE SWEEP AT 438 ms IN ONE TICK. The double-R3 "freeze" was an UNBOUNDED hang, not the known
> stutter: the rescan committed its own "I ran" state AFTER the work, so any fault inside the sweep
> restarted a ~1.2 s 17-scan burst every 100 ms forever.** Two user reports:
> (S1) a crash "just from being in WEST CITY", ordinary free roam; (S2) "activating explore mode with
> double-R3 hung the game" — **no crash message, had to kill the process**, which is the detail that
> classified it.
>
> **METHOD BREAKTHROUGH, and it is the most reusable thing in this entry: `crash_trail.bin` can be
> read WITHOUT relaunching the game.** Until now the only reader was `main.lua`'s boot print
> (2026-07-26 (b)), so getting a trail out of a reporting player meant talking them through a restart,
> and a second crash overwrote the evidence first. The format is fixed and self-describing
> (`src/mem_bridge/mem_bridge.c:245-255`: 32-byte header, 128-byte slots at offset 32, ring indexed
> `(seq-1) % slots`, `seq == 0` = never written), so it decodes from the file alone. Now
> **`tools/read-crash-trail.ps1`**. The ask to a reporter changed from "reboot and send your next log"
> to "send this 16 KB file". Use it before theorising — same lesson as READ THE HOST LOG FIRST, one
> layer down.
>
> **WHAT THE TRAIL SAID.** Local session, ring full, last op **`nav.explore`**, and the gaps are the
> point: `nav.explore` at −0.516 s → next mark 438 ms later. The mark is written BEFORE the call, so
> those 438 ms were spent *inside* `explore_rescan`, which runs in `ExecuteInGameThread` — i.e. **the
> first direct measurement of the burst this ledger had only ever estimated at ~68 ms × 17.** That
> number is what the 2026-07-29 (c) entry said it was waiting for before redesigning the sweep;
> it no longer needs asking for.
>
> **ROOT CAUSE OF S2.** `explore_tick`'s gate is `if explore_sx == nil or (moved far enough AND 4 s
> elapsed)`, and `explore_rescan` wrote `explore_sx` / `explore_scan_ms` at its END. `explore_sx == nil`
> therefore means BOTH "never scanned" and "scan unconditionally", with no floor to fall back on — so
> a fault anywhere in `Nav.list_targets` (a catchable Lua error swallowed by `pcall(explore_tick)`, or
> a pcall-piercing abort that unwinds the callback) left the state untouched and the **entire ~1.2 s
> 17-scan sweep re-ran on every 100 ms nav tick for the rest of the session**. Saturated game thread,
> no frames, no input, no way out but Task Manager — exactly what the user described, and exactly why
> there is **no UE4 crash dump for it**: a hang never reaches the crash reporter.
>
> **THE RULE, and it generalises past input handling: COMMIT THE ATTEMPT BEFORE DOING THE WORK.** The
> 2026-07-28 map-d-pad entry below states this for input edges ("commit input edges BEFORE any early
> return"); the same shape kills you when the thing you commit is *"I already did this expensive
> job"*. Any periodic job whose should-I-run gate is the same state the job writes **on success** will
> spin at full loop frequency the first time it fails. Corollary worth its own line: **a sentinel that
> means both "never done" and "do it NOW" has no fallback** — pair the nil case with the same floor
> the normal case uses, or commit first so the floor always exists. Fixed by committing
> `explore_sx`/`explore_scan_ms` at the TOP; `explore_pois` deliberately still commits at the end, so
> a faulting sweep serves the last good list instead of going silent.
>
> **THE BURST IS NOW VISIBLE.** It was invisible for its whole life — it takes no scan slot and never
> routes through `Core.timed_findall`, so `__KakarotScanStats` could not see it either, and the only
> reason we know the number is a trail gap measured after the fact. `explore_rescan` now logs the
> worst sweep once per session, thresholded at `TICK_MS` (derived, not picked: a sweep longer than one
> whole nav tick has by definition eaten the tick it ran in plus another).
>
> **S1 (WEST CITY) IS NOT CLOSED, and the honest reasons matter.** A 17-agent adversarial pass (11
> candidates, 4 survived, 7 refuted) put the manual R3-picked `target.actor` first: in free roam NO
> gate edge ever fires, so `Nav.release_world_refs` never runs, and `target_missing` is incremented
> only for NON-manual targets (`elseif target and not target.manual`) — so an auto target is bounded
> to ≤ `LOST_SCANS`×`SCAN_EVERY` ≈ 4.5 s of exposure while a **manual** pointer is aged by nothing at
> all and is dereferenced at 10 Hz for as long as the player walks. Two reasons not to declare it:
> the precondition (did the reporter hand-pick a target?) is **unverified**, and the local trail says
> `nav.explore`, not `nav.step` — so whatever killed the local session, it was not that path. Applied
> the one fix that needs no precondition: while `target_missing > 0` the tracker **coasts on the last
> position it successfully read** instead of touching a handle the last world sweep failed to find
> (plain numbers on `target.lx/ly/lz`, never a handle — metadata survives a world change, pointers do
> not). Deliberately NOT applied: dropping the manual pointer by displacement (`resume_pick.key` is an
> ADDRESS, so a genuinely freed actor can never re-match and `RESUME_TRIES` burns out — a
> hand-picked beacon going permanently silent is worse for a blind player than the crash it prevents;
> needs a `grp`+`label` fallback match first), and displacement-dropping `enemy_cache`/`navi_icons`
> (`navi_icons` is shared substrate for ALL auto-tracking — emptying it makes `best_candidate` lose
> the marker, `target_missing` climbs, and flying across a city would silence your objective).
>
> **VERSION FACTS, checked against the REMOTE and not the local tag list — a local `git tag` was
> four commits stale and produced a wrong reading of this whole episode first time round.**
> **v0.1.4** is the current release (tag `cb4a30f`, published 2026-07-29 15:23Z) and it DOES contain
> `9a7a869`, `114b980`, `a785abb`, `5d1963f`, `cb4a30f`. So "players are missing the two big crash
> fixes" is FALSE — they have them. Two consequences that matter more than the correction itself:
> (a) **the local trail session ran at 16:10Z, i.e. on v0.1.4 code**, so the 438 ms sweep and the
> `nav.explore` death were measured on the SHIPPED build, not on something already superseded — the
> `Core.scan_quiet()` guard that landed in v0.1.4 does not prevent either, because the hang lives in
> the ORDER of the state commits, not in the guard; and (b) the hang is therefore live for every
> player right now and needs a v0.1.5. Note neither of those two fixes would have addressed S1 as
> diagnosed anyway: `9a7a869` acts on gate EDGES and free roam has none, and `114b980` fixes a
> SILENCE class, not a crash class — the adversarial pass read the current tree (which includes both)
> and reached that conclusion independently of the version question. **Check `git ls-remote --tags` /
> `gh release list`, never the local tag list, before reasoning about what players are running.**
>
> **THE SEVEN THAT DIED (do not re-open):** the double-tap/rescue trio (`do_open`'s sweep on the 20 ms
> pad dispatch burning both gesture windows; a frozen `g_last` making the toggle misfire; the
> `DOUBLE_RESCUE_TICKS` path double-sweeping); `chain_to_next`'s `list_targets` call; the
> `quest_objective`/`guide_watch` non-strict multi-candidate fetches (killed by dump evidence plus
> three shipped releases); and `screen_status`'s `cached_all` on the pad loop. The absent-class scan
> tax survived only as a HALVED micro-stutter (~8 classes ≈ 115 ms/s) and explains neither symptom —
> `ABSENT_BACKOFF` is byte-identical in v0.1.1/v0.1.2/v0.1.3, so it cannot be part of a v0.1.3
> regression and must not travel in the same batch.
>
> **STILL OPEN.** (a) Whether the LOCAL session crashed at all: there is no 07-29 dump and a clean
> quit leaves an identically full ring and an identically truncated log — the trail proves the session
> ended during a `nav.explore` tick, not that something killed it. (b) The reporter's West City trail,
> which discriminates `nav.step` (handle-lifetime family) from `nav.explore` (same bug as the hang).
> (c) The ~1.2 s burst itself: with the hang bounded it is back to being a stutter, and the redesign
> (slice the 17 classes across ticks, flattening each to plain POIs immediately and swapping the list
> atomically — NOT `take_scan_slot`, which starves the shared picker) is a deliberate decision still
> to be taken.

> **2026-07-29 (b) — MULTI-AGENT CRASH SWEEP: ALL 74 LUA FILES + ALL 4 NATIVE BRIDGES AGAINST THE
> LEDGER'S OWN MECHANISM CATALOGUE, 7 OF 14 CANDIDATES CONFIRMED AND FIXED.** Not driven by a user
> report: a systematic re-read of every Lua file and every native bridge against the crash mechanisms
> this ledger has already catalogued (dangling handles, keybind-thread races, truncated reflection,
> fail-open/fail-closed guard mistakes, pooled-widget staleness…). 14 candidates were raised; each was
> handed to an independent OPUS agent whose job was to REFUTE it, not confirm it. **7 died, 7
> survived** — a 50% kill rate, consistent with the 07-27 pass (37 of 48 died). All 7 survivors are
> **FIXED IN THE WORKING TREE, SOURCE-ONLY, UNVERIFIED IN GAME** — needs a full restart, not a
> Ctrl+Shift+R (`input_bridge.dll` was rebuilt).
>
> **THE SEVEN CONFIRMED AND FIXED:**
>
> 1. **`nav_tracker.lua` `step()` — world-actor handles were never released when a MENU was already
>    open as the world went away.** All three gates (Transition, UI-mute, world) share ONE
>    `gated_prev` latch, but the world-actor release lived only inside the world gate's
>    `not gated_prev` edge. So when a muting adapter was already on screen the tick the world closed,
>    the UI branch set the latch FIRST and the world branch's release never ran for the whole gated
>    period — `enemy_cache`, `navi_icons`, `target.actor` and `chain_wait.actor` rode across an entire
>    battle or cutscene, which is exactly what destroys those actors. Reachable via `screen_fishing`
>    (pooled HUD lingers ~3 s), `screen_loading` (parked pane holds the adapter ~10 s) and
>    `screen_choice` (an NPC Yes/No answering straight into a battle) — a battle/cutscene is
>    explicitly NOT a `Transition`, so `Transition.on_begin` never covered it either. Re-validating
>    per use is not a defence: a recycled address passes both `Mem.alive` and `IsValid`, and
>    `actor_pos` then makes a raw `K2_GetActorLocation` on it. This is a REGRESSION of a fix
>    originally written for a confirmed user crash (see 07-26 (c) above). Fix: hoisted the release
>    into `Nav.release_world_refs()`, called from the UI branch too, and extended to release
>    `chain_wait.actor` (nothing below `WORLD_DROP_TICKS` was clearing it). Deliberately does NOT
>    release `mm_cache` (a pooled widget, not a world actor — dropping it would arm the 5 s
>    MM_RETRY backoff on every menu).
> 2. **`nav_tracker.lua` `Nav.toggle_route` (Shift+F3) ran on UE4SS's keybind thread.** Not a pure
>    flag flip: `clear_invoker` does `FindFirstOf("PlayerController")` (a `GUObjectArray` walk),
>    `Core.valid`/`Core.member` (mutating the shared `valid_memo`/`prop_sets`/`prop_budget` tables
>    the 100 ms `step()` reads in the same instant), and `UnregisterNavigationInvoker` — a
>    `ProcessEvent` into the live navigation system — all on the same `lua_State` as the poll loop.
>    Shipped defaults (`on=true`, `route_mode=true`) put `invoker_key` set within a tick or two of
>    free roam, so the path is open in ordinary play. The 07-27 keybind sweep concluded "only 1 of
>    ~19 handlers was wrong" and MISSED this one because it is delegated through `app.lua` rather
>    than bound directly. Fix: body wrapped in `ExecuteInGameThread`, matching `Nav.where` /
>    `Nav.cycle_companion`; the unused `return route_mode` dropped.
> 3. **`main.lua` Ctrl+Shift+R (the mod reload) ran on the keybind thread.** Not dev-only — it sits
>    above the `if Build.debug` block and the README advertises it to players, so it shipped in
>    every release. The fatal part is not the teardown (every `stop()` body is pure Lua plus bridge
>    calls) but `App = require("app")`: it reparses ~60 modules — string interning, proto/closure/
>    table allocation, incremental-GC steps — for tens of milliseconds on the keybind thread while
>    ui_core, nav, battle, quest and pad_poll all execute Lua on the SAME `global_State`. Same
>    allocator+GC race as the construction-notify episode. Fix: whole handler body wrapped in
>    `ExecuteInGameThread`. NOTE the sweep originally filed this against `pad_poll.lua:35`
>    (`steppers[name] = nil`); the refuter showed that write is a one-instruction torn window that
>    is a no-op on an absent key and will essentially never fire — `pad_poll.lua` needs NO change.
>    Recorded here so nobody "fixes" pad_poll later.
> 4. **`screen_choicelist.lua` called `GetFullName()` BEFORE the validity guard.** The conjunction
>    `if it:GetFullName():find(...) and Core.on_screen(it)` dereferences the handle first.
>    `Xcmn_Win01_List_C` is not directory-mapped, so `Core.cached_all` serves it from the
>    `FindAllOf` pool cache on its ~30 s refresh as long as ONE entry lives, so freed rows sit in it.
>    Not private to this screen: the substory reward sheet builds fresh instances and its host is
>    destroyed on close (no map switch), so opening any system window shortly after dereferenced the
>    freed rows. Every other `GetFullName`-on-a-cached-pool site in the mod already guards first;
>    this was the sole outlier. Fix: conjunction reordered (also a saving — the path string is now
>    built only for rows that survive the on-screen test).
> 5. **`ui_archetypes.lua` `A.shop_money` was a multi-candidate probe using NON-STRICT
>    `Core.member`.** `MONEY_HOLDERS` is documented as exactly-one-exists-per-host, so the other
>    candidates are names with positive reason to be ABSENT — and non-strict `Core.member` falls
>    through to a RAW fetch whenever the per-tick property-set budget is unavailable
>    (`PROP_SETS_PER_TICK = 1`, shared by ~40 adapters) or the class's set came back partial.
>    Opening a shop presents TWO never-seen classes on the same tick, so the budget is
>    deterministically exhausted exactly when these probes run — the normal path on every shop
>    open, repeating every 100 ms for the whole visit. The function's own comment claimed the
>    existence gate made an inapplicable name "a quiet nil instead of the uncatchable abort", which
>    is precisely the false belief the 07-28 (a) strict-gate rule exists to prevent; that comment is
>    now corrected in place. Fix: all probes routed through `Core.first_member` (`strict = true`).
> 6. **`nav_tracker.lua` `explore_rescan` — a ~1.2 s game-thread freeze every 4 s.**
>    `Nav.list_targets` issues SEVENTEEN raw `FindAllOf` calls, none taking a scan slot, checking
>    quiet mode, or routing through `timed_findall` (invisible to `__KakarotScanStats`). At the
>    repo's own measured 68.2 ms per scan that is ~1.2 s in ONE tick, fired every 4 s indefinitely
>    while running or flying with explore mode on (double-R3, a shipped feature). NOT a crash and
>    NOT a pcall pierce — the refuter confirmed `explore_pois` stores only plain values, no handles
>    survive the rescan. PARTIAL FIX ONLY: an early `Core.scan_quiet()` return, so it cannot fire
>    during a load or cutscene. STILL OPEN — the 1.2 s burst itself needs a cheaper source (the
>    minimap pointer walk plus the already-cached enemies list) instead of the 17-class sweep; that
>    changes what explore mode can FIND, so it wants a Ctrl+F5 measurement and a deliberate decision
>    first. Explicitly do NOT budget-gate `list_targets` with `take_scan_slot`: it is shared with the
>    R3 picker and this loop only refills the PROPERTY budget, so that would hand the picker a
>    near-empty list whenever the registry loop is stopped — the fail-closed-on-shared-substrate trap.
> 7. **`src/input_bridge/input_bridge.c` — `g_haveLast` was a ONE-WAY latch, and losing the pad made
>    the game permanently deaf to the KEYBOARD.** Set on the first successful `XInputGetState` and
>    never cleared, while `g_last` only refreshes on `ERROR_SUCCESS`. After a disconnect (a wireless
>    pad sleeping or running out of battery — routine in a long session) `pad_snapshot` served the
>    FROZEN last frame forever and `Input.read()` never returned nil, breaking `l_poll`'s own
>    documented contract. That silently disabled the only pad-loss recovery both pad menus have
>    (`if not snap then ... Input.block(false)`), and the radar picker — still latched `open` —
>    went on renewing `Input.kb_block(300)` every 20 ms, so the keyboard was blocked for the rest of
>    the session with no in-game way out. Nothing errored at all, which is why it was invisible. Fix:
>    clear the latch on a failed read for user 0 (`if (idx == 0 && r != 0) InterlockedExchange(&g_haveLast, 0);`),
>    routing `l_poll` to its direct-read fallback which correctly reports a disconnected pad; the set
>    is now interlocked too, matching the reader. DLL rebuilt. **This one needs a full game RESTART,
>    not a Ctrl+Shift+R.**
>
> **THE SEVEN THAT DIED (recorded so they are not re-opened):**
> - `screen_community.lua:1201` `read(det.Txt_Name)` — raw fetch, but `Txt_Name` IS a reflected
>   property of `Start_Commu_Detail_C` (CXX header dump + an F7 census showing the value), and the
>   owner has already survived several full dereferences in the same atomic game-thread callback. An
>   absent name would be a deterministic 100%-reproducible kill on a screen marked verified, not an
>   intermittent crash.
> - `nav_tracker.lua` `Nav.toggle` (plain F3) — on the keybind thread, but touches NO engine object:
>   `drop_target` and every preempt/chain write is a plain Lua upvalue, `Audio.stop` is a native
>   bridge call. Wrapping it would also defer the documented "F3 off = immediate silence". Contrast
>   with item 2 above — the distinction is whether the handler dereferences anything.
> - `ui_directory.lua:66` `prop()` non-strict — the fail-open mechanism is real but the fatal step
>   was refuted by a live in-game trace.
> - `ui_core.lua:701` — `Core.array_of` has no `strict` parameter, but release history refutes the
>   crash: the existence gate is not even in shipped v0.1.2 while the pcall has been there far
>   longer, so the released mod already ran exactly this path.
> - `ui_core.lua:1430` — `custom_props` is not flushed on transition, which IS the documented
>   `RegisterCustomProperty` trap, but the only consumer (`screen_community.skills_text`) is a
>   `local function` with ZERO call sites: dead code in every build.
> - `src/input_bridge/input_bridge.c:345` `pad_block_renew()` in `l_poll` — "any reader renews the
>   lease" is real (worse than claimed: `ui_registry.pad_boost` calls `Input.read()` ungated every
>   100 ms, so a live lease never lapses) but the refuter showed it does not produce the claimed
>   failure.
> - `src/audio_bridge/audio_bridge.cpp:159` — the `SetOutputMatrix` channel clamp is real and
>   release-reachable, but it fails on SEVERITY, not reachability.
>
> **METHOD NOTE.** The sweep's value came from grouping files by hot-path risk and giving every
> candidate an independent adversarial refuter — a 50% kill rate, consistent with the 07-27 pass (37
> of 48 died). A candidate list without refutation would have produced seven unnecessary edits to
> working code, on a mod where a wrong "fix" means a blind player hears nothing.

> **2026-07-29 — A CRASH REPORT NAMED THE WRONG SCREEN, AND FOLLOWING THE LOG DOWN FOUND A NEW
> SUBSTRATE BUG: A REFLECTION WALK CAN TRUNCATE SILENTLY AND GET CACHED AS AUTHORITY.** User reported
> a crash "on the party screen." `UE4SS.log` (00:58:56 → 01:25:12) ends with no shutdown line, no
> traceback, no `faulted in is_active` — a hard process death, nothing caught it. **It was not the
> party screen**: `screen_party` was entered and left cleanly twice (01:23:58, 01:24:56). The process
> died 8 s later, the same second the SKILL TREE was entered, and the log's LAST line is
> `array gate: Start_Skilltree_C has no 'WL_Skilltree_Zorb00' (not read)`.
>
> **Why that one line was the whole investigation.** The gate log dedupes per `(class address,
> member)`, so it prints only on the FIRST refusal of a given member all session — and the skill tree
> had opened cleanly three times earlier in the same log (01:24:02, 01:24:11, 01:24:36). `array_of`
> fails OPEN while no property set exists yet for a class (budget is 1 class/tick shared by ~40
> adapters), so those three earlier visits read `WL_Skilltree_Zorb00` RAW and succeeded — proof the
> member is real, corroborated independently by `STATUS.md:291` (the 12 orbs verified in-game
> 2026-07-14). So between visit 3 and visit 4, something turned a real, previously-readable member
> into a permanently refused one. Nothing about the member changed; the CACHE did.
>
> **ROOT CAUSE.** `prop_set`'s `GetSuperStruct()` walk in `ui_core.lua` (the one that builds the
> per-class property-name set `Core.member`/`Core.array_of` gate against, per the 2026-07-25 (b)
> entry below) could stop EARLY and SILENTLY, short of the actual root of the class hierarchy —
> typically at the Blueprint generated class, without ever reaching its native base. Four ways it
> happened, all now named in code: (a) `GetSuperStruct()` raised, (b) the walk hit a super with
> nothing usable below the declared root, (c) the `SUPER_MAX` depth cap was hit, (d)
> `ForEachProperty` itself raised partway through a level. Whichever fired, the truncated set was
> cached exactly like a complete one — same cache, same lifetime, flushed only on map transition —
> and every later gated read of a member declared on the missing NATIVE base was refused forever,
> with a single deduped log line and no error. A screen goes quiet after having worked, with nothing
> in the log to say the cache is the reason.
>
> **FIX SHIPPED — SOURCE-ONLY, UNVERIFIED IN GAME, 31 files, +682/-299, needs a full RESTART**
> (`ui_core.lua` changed, not hot-reloadable). A walk that does not reach the hierarchy root is now
> marked PARTIAL rather than complete. A name ABSENT from a partial set means "don't know" and now
> FAILS OPEN in both `Core.member` and `Core.array_of` — exactly the standing fail-open rule, just
> applied to a set the code previously trusted as exhaustive. A name PRESENT in a partial set is
> unaffected: the walk still recorded its real property TYPE, so `array_of`'s fixed-array check keeps
> working. `strict` gates (the multi-candidate probes from 2026-07-28 (a)) still fail CLOSED on a
> partial set — their whole contract is that most candidates are *expected* absent, so "don't know"
> there stays a refusal, not a licence to fetch. Partial sets are logged once per class (depth reached
> + which of the four cases fired), re-derived up to `PARTIAL_RETRIES = 3` times, wall-clock throttled
> `PARTIAL_RETRY_S = 5.0`, drawn from the existing `PROP_SETS_PER_TICK` budget rather than a new one;
> a completing retry logs itself so a transient truncation is visible as resolved, not just silent.
> All partial state is flushed on map transition together with `prop_sets`.
>
> **THE NEAR-MISS, worth its own paragraph because it would have been worse than the bug it fixed.**
> The first implementation treated `GetSuperStruct() == nil` as the natural end of the chain — the
> obvious reading of "no more super". It is wrong: UE4SS wraps the raw pointer, so the TOP of a real
> hierarchy answers an INVALID handle, not nil — every other super-walk already in this codebase
> already terminates on `not IsValid` (`discover.lua:562`, `tools/ue4ss-inspector/Scripts/main.lua:136`
> — this file just hadn't been checked against them). Had it shipped, `sup == nil` would never fire on
> a genuine root, so EVERY class's walk would report "not yet at root", every set would be marked
> PARTIAL, and both existence gates would go permanently open MOD-WIDE — including for
> `screen_dialog`'s by-design multi-candidate array probe, which depends on the gate refusing a name
> it knows is absent. That reintroduces the exact `GetAddress`-pierces-pcall class from 2026-07-25 (d),
> on every dialogue tick. Caught in adversarial review before it ran. The shipped discriminator is the
> ROOT NAME: a walk whose last resolved struct is `Object` is complete; anything else is truncated,
> regardless of why the walk stopped.
>
> **Adversarial review also reverted three fixes from the same pass that would have SILENCED
> screens** — same shape as every other round in this ledger, recorded so the pattern stays visible:
> a `strict` gate applied to an ordinary (non-multi-candidate) member drops the whole row on a
> budget-starved tick; an added `Core.on_screen` requirement silenced the skill-palette slot cursor
> (a case the 2026-07-28 pane-liveness rule below already covers — different pane, same trap); and a
> bare `nil` check standing in for `Core.valid` would have let a dead handle reach a native read.
>
> **STILL OPEN: whether the mod actually killed the process that session.** The log proves the LAST
> thing it printed, not the cause of death — no traceback, no uncatchable-throw signature, just
> silence where a shutdown line should be. The array-gate refusal is a strong correlate (it fired in
> the same second) but is not proof: `array_of`'s refusal itself fails open into "not read", it does
> not fetch. The decisive evidence is the mod's own crash black box (`mem_bridge.mark()` ring →
> `crash_trail.bin`, printed at the next boot by `main.lua:34-43`, granularity `ui.update <adapter>`
> from `ui_registry.lua:310`) — not yet seen for this session. Asked the user for the head of the next
> session's log. **Do not write this up as solved until that trail is read.**
>
> **DIAGNOSTIC RECIPE for this bug class, for the next time a screen goes quiet after having worked:**
> grep the log for `partial property set:` first — if a class the silent screen depends on shows up
> there, the cache is truncated and every inherited member on it is now suspect, not just the one
> that happened to log. Independently, grep for `array gate:` / `member gate:` naming a member you
> have OTHER evidence is real (a prior successful read in the same log, a STATUS.md derived-fact row,
> a CXX header declaring it) — a refusal of a member you can prove exists is the signature of this
> bug, as opposed to a genuinely absent one.

> **2026-07-28 — THE AUDIT'S OWN FIX BROKE A FEATURE: `IsValid` IS NOT UNIVERSAL, AND AN FName
> RAISES THROUGH pcall.** The user reported the crashes gone and, in the same breath, that the radar
> picker no longer opened **on either bind** (R3 and V). Both binds funnel through
> `do_open()` → `Nav.list_targets()`, and that is exactly where the log's 16 identical tracebacks
> stopped — with the mod's own boot lines confirming `input_bridge loaded (hooked=true)`, so input
> was never a candidate.
>
> **Mechanism.** The 07-27 audit added result-validation to `Core.member` (validate the RESULT, not
> just the owner). Its dispatch was: object-family → `Core.valid`; **anything else with a known type
> string → `Core.valid_ref`**. That second clause is the bug. `Core.valid_ref` is one pcall'd
> `IsValid()`, and `IsValid` is a **RemoteObject** method — UE4SS's FName binding does not have it.
> The call instead resolves against the global `FName` **constructor**, which raises
> `No overload found for function 'FName'. Overloads: #1 FName(string…) #2 FName(integer…)` — and
> like the `GetAddress`-on-polymorphic-type error, **it pierces pcall**: it unwinds to UE4SS's own
> callback boundary, so the calling function dies mid-flight while every enclosing `pcall` reports
> success. The traceback's double-print is the tell (`ui_core.lua:84` in a pcall, printed twice).
>
> **Blast radius, and why it was total rather than partial.** `npc_name` reads `UniqueId`, a
> `NameProperty`, and it is the FIRST thing the target list asks about a field NPC. So the list died
> on the first NPC in range, `do_open()` never received a list, and the picker never opened at all —
> a whole verified feature, from a guard that was only ever meant to catch dangling handles.
>
> **Fix.** The result gate now checks **only `ArrayProperty` and `StructProperty`** — the two
> handle-shaped types the two-tier rule was actually invented for. Every other property type is a
> VALUE that cannot dangle, so it fails OPEN, per the standing rule. Plus `Core.name_str(o)`:
> convert with `ToString()` and let the conversion be the test, never ask a value type for a
> validity method. It replaces the five direct `Core.valid_ref`-on-an-FName calls that were already
> sitting in `nav_tracker` (2286/2301/2596/2622/2694) — latent, and about to become live the moment
> the gate above them stopped aborting first.
>
> **THE RULE: a guard that widens its own scope by ASSUMPTION will take out a working feature.**
> The 07-27 clause `type(pt) == "string" and pt ~= "" and pt ~= "?"` reads as prudence — "check
> everything we know the type of" — but it silently asserted that every non-object property yields a
> handle that answers `IsValid`. Nothing verified that. Guards belong on an **enumerated
> whitelist of cases you have evidence for**, never on "everything else"; the whole point of
> fail-open is that the unknown case is the one you must not act on. Corollary for this codebase:
> `Core.valid` is UObject-only, `Core.valid_ref` is **array/struct-only** (its header now says so),
> and value types (`FName`, `FText`, strings, numbers, enums) get **no validity call whatsoever**.
>
> **And the cheap lesson, again:** three tool calls into the session the log had already named the
> file, the line and the mechanism. The standing rule — *when several things go quiet at once, READ
> THE HOST LOG BEFORE THEORISING* — paid for itself for the third time.

> **2026-07-27 — FULL-CODEBASE CRASH AUDIT (multi-agent), 11 confirmed holes closed.** Not driven by
> a crash report: a systematic sweep of all 71 Lua files plus the four native bridges against the
> ten crash mechanisms this ledger has accumulated. 48 candidates found, each then handed to an
> independent verifier whose default was to REFUTE it; **37 died** (behind debug flags that ship
> off, or genuinely guarded upstream) and 11 survived. **SOURCE-ONLY, UNVERIFIED IN GAME**; needs a
> full restart (`main.lua`/`app.lua` + all four rebuilt DLLs). The method is worth repeating: the
> adversarial second pass is what made the result usable — a raw finder pass would have been 77%
> noise.
>
> **THE FIVE GENERALIZABLE RULES (the reason this entry exists):**
>
> 1. **Two shared guards against the same hazard must be equally strong, or call sites silently
>    migrate to the weak one.** `Core.member` refused both an undeclared member AND a wrong-typed
>    one; `Core.array_of` only refused the wrong TYPE — when the class was known and the member did
>    not exist at all, `declared` was nil and it fell through to the raw fetch. Live every tick:
>    `screen_dialog` probes `{WL_TextPlateCtn, UIChoice_List}` against the on-screen window and by
>    design Win01 declares one and Win02 the other, so **every ordinary dialogue window fetched a
>    non-existent member on every tick.** Nobody noticed because the call site *looked* guarded.
>    When you add a gate to one helper, audit its siblings the same day.
> 2. **A field battle or cutscene is NOT a `Transition`.** It closes the world gate WITHOUT a
>    `LoadMap`, so the world-epoch hook that flushes actor caches never fires. Three caches lived in
>    that gap (`enemy_cache`, `navi_icons`, `chain_wait.actor`). The 07-26 fix ("expired +
>    unrefreshable ⇒ drop") only covered the QUIET path, so a fight shorter than `RESCAN_CLASSES`
>    (100 ticks ≈ 10 s) left the cache neither expired nor refreshed and the first post-battle
>    `enemy_alert` served handles combat had just destroyed — which is precisely the 07-26 black box
>    (last op `nav.step`, AV reading 0x10). **Every actor cache must drop on BOTH edges, not just on
>    transition.** Drop to `nil`, never `{}`: an empty table is served as "no enemies" instead of
>    forcing a real rescan.
> 3. **A grace period on a raw handle is the same defect with a timer on it.** `target.actor`
>    survived the gate for `WORLD_DROP_TICKS` (~5 s), so any battle shorter than that left a dead
>    pointer. Resolved by separating pointer from metadata: the handle is released on the falling
>    edge, the record (`key`, `grp`, `label`, `manual`) survives, and `remember_pick()` re-acquires
>    by category+key — which is what the battle-interruption-resume design always actually used.
>    **The metadata is what survives a world change; the pointer never does.** Note the companion
>    change this forced: `target.actor == nil` had to be distinguished from an *invalid* handle,
>    because "we released it" and "the actor died" deserve opposite responses — conflating them
>    marked interrupted collectibles as visited and silently dropped manual picks.
> 4. **UE4SS runs keybind callbacks on its OWN keyboard thread (`UE4SS-UpdateThread`, ~200 Hz), not
>    the game thread.** F1 (`App.repeat_current`) reached `GuideWatch.reannounce()` →
>    `Core.cached_all` — a pooled-widget walk, and a ~65 ms `FindAllOf` at boot — concurrent with
>    `step()` on the SAME `lua_State`, mutating `os_memo` / `valid_memo` / `prop_budget` /
>    `all_cache`. Same allocator+GC race as the construction-notify episode, reached by a different
>    door. It was the ONLY unwrapped handler out of ~19 (swept and confirmed). **This is a
>    mechanically checkable invariant — every keybind that touches an engine object wraps in
>    `ExecuteInGameThread` — and it should be linted, not remembered.**
> 5. **Diagnostics drift back to unsafe shapes after every sweep.** `discover.lua` (the F7 census)
>    still had **nine** bare `:IsValid()` after the 07-25 conversion — including a private
>    `o ~= nil and o:IsValid()` helper gating the `Map_World_C` scan that feeds the memory-WRITE
>    path. `Nav.dump_levels` was the only `enemies_list()` consumer with neither per-entry
>    `Core.valid` nor a transition/world gate, while its sibling `Nav.dump` had both — and it ships
>    bound to Ctrl+Shift+F5 with no debug flag. Tools written *after* a sweep never get swept.
>
> **Recurring false belief, now killed in the comments too:** *"the surrounding `pcall` protects
> this fetch."* It does not. An undeclared-member fetch is an uncatchable C++ throw (0xe06d7363),
> and in a chain `host.A.B` the inner hop is evaluated at the CALL SITE, outside the pcall. Two
> comments asserting the opposite (`screen_dialog`, `screen_training`'s header claiming all its
> reads were guarded) were corrected rather than deleted — a wrong comment is worse than none.
>
> **NATIVE BRIDGES — 8 items, all 8 confirmed, none refuted.** The two that mattered:
> `audio_bridge`'s RIFF bounds check (`off + 8 + len > size`) was 32-bit unsigned and **wrapped**,
> so a `len` near `0xFFFFFFFF` passed the guard into a ~4 GB `memcpy` and the advance could loop
> forever — reachable with any truncated WAV in `Scripts/sounds/`, which the file's own header
> invites users to add. And `mem_bridge`'s `WRITE_FN` validated nothing but `p != 0`: an in-process
> arbitrary-write primitive. Bounded now by `MAX_WRITE_OFFSET`, **derived** (largest real offset in
> `native_offsets.lua` is `skillTree.cursorRow = 0x15FC` → next power of two), not guessed.
> **But an offset bound does not stop a right-offset-WRONG-OBJECT write**, which corrupts silently
> and crashes later with no trace — so `mem.lua`'s `writer()` gained an optional `expect_class`,
> checked via a bounded `GetSuperStruct()` walk so a legitimate SUBCLASS is not refused, failing
> OPEN on an unreadable chain and logging once per `(expected, actual)` pair. The class name is the
> same string the caller already looks the host up by (`FT_HOST_CLASS`), so lookup and assertion
> cannot drift.
> Also: `l_mark_open` could longjmp on `LUA_ERRMEM` past its own cleanup, leaving handle+mapping
> leaked with `g_mark` NULL and the "already open" guard blind to the retry — in the crash-trail
> recorder, the one component whose whole job is to survive a crash. And `input_bridge`'s `g_block`
> was a latch with **no lease**, unlike the keyboard's `g_kbUntil`: a Lua error unwinding with
> `Input.block(true)` set left the gamepad dead until the game was killed. Now deadline-based, so
> it self-heals.
>
> **Three further native findings, outside the audited mechanism set — two fixed, one left open by
> decision:**
> - `audio_bridge`'s `do_init()` released nothing on any of its nine failure returns, so a retry
>   after a failed init stacked a second engine + mastering voice; `load_wav` also overwrote
>   `out->pcm` without freeing. Fixed via `release_all()` + `init_fail()`. **Reachability was
>   narrower than first reported and that is worth recording:** `Audio.init()` is called once from
>   `main.lua`, which is explicitly outside the hot-reload set, so **Ctrl+Shift+R does NOT re-run
>   it** — the only live path is a UE4SS-level mod restart *after an init that already failed*.
>   Real, but nowhere near "leaks on every reload".
> - `g_last` was written by `hookGetState` on the game's input-pump thread and copied by `l_poll` on
>   the game thread with no synchronisation. Now a **seqlock** (odd = write in flight), chosen
>   because the pump path runs inside the game's own input hook and must stay non-blocking — two
>   barriered increments on the writer, a bounded retry on the reader, falling through to a direct
>   `g_realGetState` read if the snapshot is contended, so a busy tick returns the true pad rather
>   than nothing.
> - **`prism_bridge` still never calls the `p_shutdown`/`p_backend_free` it resolves — deliberately
>   left open.** `DllMain`/`DLL_PROCESS_DETACH` is ruled out (calling into another DLL under the
>   loader lock is exactly the class of bug this audit exists to remove), and there is no other
>   place to put it: the mod registers no unload callback, and `App.stop()` is the RELOAD path,
>   where PRISM must stay alive. Closing it needs a genuine process-exit hook in `main.lua` that
>   does not exist yet, plus `Speech.shutdown()` forwarding to a new `prism_funcs.shutdown`
>   (`p_stop` → `p_backend_free` → `p_shutdown`, clearing `g_ready`).
> - Same reasoning killed an `audio` shutdown path: `Audio.stop()` means "silence cues" and is
>   called ~8 times per session from `nav_tracker`, and audio is designed to survive a reload, so
>   tearing XAudio2 down there would leave the mod permanently mute after Ctrl+Shift+R. **A shutdown
>   API nobody calls is worse than a leak the OS reclaims at process exit** — the fix stopped at the
>   leak on purpose.

> **2026-07-26 (c) — SECOND CRASH, SECOND TIME THE BLACK BOX NAMED THE SUBSYSTEM.** Player exited
> a combat into a story dialogue. Last trail entry: `nav.step`. Error:
> `EXCEPTION_ACCESS_VIOLATION reading address 0x00000010` = `GetClassPrivate()` on a dead handle.
> Callstack: all UE4SS.dll, with the same ~8 offsets repeating three times (chained/nested
> property resolution), and its low digits match the 07-24 12:05 signature exactly
> (`b27d28 > b354b6 > b340ee > b3373c`) — so this is a known family, not something new.
>
> **CAUSE (the handle-lifetime audit had predicted this exact site and it was not acted on):**
> `enemies_list()` and `best_candidate()` refused to refresh their cached lists while
> `Core.scan_quiet()` is true — and **the dialogue adapter is what sets `scan_quiet`**. The
> deferral is right about the cost (a ~65 ms `FindAllOf` during a cutscene is what quiet mode
> exists to prevent) but drew the wrong conclusion: it kept *serving* the expired list. So
> "finish a battle → story dialogue opens" pinned a list of **just-destroyed enemy actors** and
> the radar dereferenced it for as long as the conversation lasted.
> Fix: when the list is expired and a refresh is forbidden, **DROP it**. Dropping costs nothing;
> a radar that announces no enemies during a cutscene is correct, one that announces enemies that
> no longer exist is wrong even when it does not crash. Same fix applied to the `navi_icons`
> minimap-widget list (emptied in place, not returned from — that function has other candidate
> sources further down).
>
> **THE RULE:** *"the stale list keeps serving, entries are re-validated by every user"* — written
> in the code as a justification — **is unsound**. Re-validation cannot detect an address the
> engine has recycled; `mem.lua` says so in as many words. **A cache that cannot be refreshed must
> be discarded, never served.**
>
> **THE OTHER HALF, and it is a whole CLASS: `Core.member` validated the OWNER, never the
> RESULT.** An adversarial trace of everything reachable from `step()` found the only live fetch
> whose receiver never reached `Core.valid` — `drop_item_name`:
> `local d = Core.member(comp,"FieldItemDropData"); if not d then return end; local id = d[fld]`.
> **`o[name]` on a null or dead field does not return nil** — UE4SS hands back an INVALID
> RemoteObject — so `if not d` passed a dead handle to the next hop, which is a `__index`
> property resolution, which is the `+0x10` read. That matches every piece of evidence: an AV on
> a MEMBER FETCH (not a method call), zero `Mem.alive` rejections (it never went through the
> guard), and a callstack whose offsets repeat three times (a chained resolution). It is armed by
> the reported path specifically: `Nav.list_targets` has **no `scan_quiet` deferral at all**, so
> it runs its full class sweep while a dialogue is up.
> Five call sites fixed the same way, plus `compute_route`'s array elements.
>
> **Then fixed at the source, because ~200 call sites cannot be trusted to remember:**
> `Core.member` now validates its RESULT, choosing the check from the property TYPE the existence
> gate already records — `Core.valid` (memory pre-check first) for the UObject family,
> `Core.valid_ref` (IsValid only) for struct/array/name handles, since `Core.valid` would call
> `GetAddress` on those and UE4SS raises that THROUGH pcall. Fails open on non-userdata, unknown
> types, `RegisterCustomProperty` members (invisible to `ForEachProperty`, so unaffected) and a
> spent budget; kill switch Ctrl+G with the existence gate. **The two-tier rule is now applied by
> the substrate instead of being remembered at every call site.**
>
> Also: `explore_tick` runs in the same game-thread callback right after `step()` and wrote no
> mark, so a trail ending in `nav.step` could not distinguish them. It marks `nav.explore` now.
>
> **Swept for the same shape.** Only those two caches keyed their staleness on `scan_quiet`
> (`screen_community:522` is an *arming* gate, not a cache-serving path). `ui_core`'s own pool
> refresh also defers under quiet (`:851`, `:1006`) and `cached_all` keeps serving — the same
> shape — but that is left ALONE deliberately, and the distinction is the point: those are
> **pooled WIDGETS**, which the game keeps alive and merely collapses when a screen closes,
> whereas `enemies_list` holds **ACTORS**, which the engine genuinely destroys. Widgets survive
> being stale; actors do not. Changing the substrate would blind every menu during cutscenes for
> a risk that has never fired there. If it ever does, the fix is per-class, not global.
>
> **AND A WARNING ABOUT THE GUARD:** `Mem.alive` was active all session and logged **ZERO
> rejections**. It only catches garbage/unmapped memory; a freed UObject whose pool memory is
> still mapped passes every check. So the pre-check is a backstop against *garbage*, not a
> liveness proof — the actual defence is **not holding handles across ticks for objects the engine
> destroys**. Do not read a quiet rejection counter as "no dangling handles".

> **2026-07-26 (b) — THE BLACK BOX PAID FOR ITSELF ON ITS FIRST CRASH.** The player crashed right
> after a map change. The trail's last entry was `ui.is_active screen_toasts`, and the next adapter
> in registration order (`screen_title`) never got its mark — and the mark is written BEFORE the
> call. So the crash was inside `screen_toasts.is_active()`, named in one read, with no guessing.
> The UE4SS.log agreed on the surroundings (`transition gate ON (new world)` at 12:43:08, loading
> screen, then the log simply stops at 12:43:16 with no error, which is what an uncatchable abort
> looks like) but on its own it could never have named the adapter.
>
> **The bug: `screen_toasts.lua` fetched `bar.Txt00` naked** — a member the SAME FILE's comment,
> nine lines below, records as ABSENT on `Info_Log_Bar02_C`, naming it as the 2026-07-17 fishing
> crash. The 2026-07-24 "fix" moved the fetch inside a `pcall` and stopped there. **A pcall cannot
> catch this.** The bar classes in that pool are recycled and a map transition rebuilds the pool,
> so the wrong subclass eventually lands in `Info_Log_Bar00..04` and the fetch aborts. The twin
> loop below it had been fixed properly (TextBox); this one was, in its own words, "left behind".
>
> **Why the guard sweep missed it:** the lint checked `IsValid` / `GetArrayNum` / `GetAddress` but
> **not member fetches**, and screen_toasts was not in the hand-picked file list. Added rule
> `dynamic-member-fetch` (a subscript built with `..` or `string.format` — the shape that means
> "iterating candidate names", which is exactly when a name may not exist). It immediately found
> **13 more live sites** in screen_community, screen_fishing, screen_fishresult, screen_results,
> screen_shopinfo, screen_tutorial, keyhelp and ui_archetypes. All fixed. Plain-Lua-table false
> positives opt out with an explicit `-- lint:plain-table` marker rather than being silently
> excluded.
>
> **THE LESSON, and it is the important one:** every previous round of this ledger reasoned from
> code to a suspect. This round read the answer off a file. *A hand-picked sweep is a guess about
> where the bug is; a lint rule is a decision about what the bug IS.* When a crash class recurs,
> the deliverable is not the fix — it is the mechanical check that makes the fix complete.
> Corollary already earned twice: **"I wrapped it in a pcall" is not a fix for this family**, and a
> comment saying so ten lines away does not protect the code that ignores it.

> DBZ Kakarot mod crash ledger. LATEST (2026-07-25 d): the batch of silenced menus was
> `GetAddress()` called on a **non-UObject** RemoteObject (the TArray in `Core.array_of`) — UE4SS
> raises "polymorphic type is not allowed" and that error **pierces pcall**, killing the adapter's
> function mid-flight. 510 tracebacks in the user's log named every affected screen; three rounds of
> code-only reasoning had got the mechanism wrong. Fix: `Core.valid_ref` (IsValid only) for array
> and struct handles; `Core.valid`/`Mem.alive` are for UObjects only. **Read UE4SS.log before
> theorising about a silent screen.** Also 2026-07-25:
 the root cause of the whole access-violation
> family is settled from the UE4SS source — **`IsValid()` dereferences the object before it checks
> anything**, so it faults on exactly the handles it exists to reject, and UE4SS never clears the
> raw pointer inside a Lua handle. Fix: `Mem.alive()`, an SEH-guarded memory pre-check that runs
> BEFORE UE4SS touches the object (class-pointer offset derived at runtime). `bUseUObjectArrayCache
> = false` is a documented dead end. Same day: the 07-24 hardening had silenced screens (load-game,
> Options save confirmations) because `Core.valid` failed CLOSED on an unanswerable `GetAddress` and
> `ui_directory` gates every pointer hop on it — guards must fail OPEN on "don't know".
>
> Previously (2026-07-24 c): AV mid-COMBAT reading 0x10 after a huge lag spike (= engine GC/streaming). KEY BREAKTHROUGH: crash CLASS A is guardable after all — obj:GetAddress() returns the stored pointer WITHOUT dereferencing, so new Core.nonnull() gates every Brush.ResourceObject read (keyhelp first, as this ledger predicted); Core.valid also rejects NULL-handle wrappers; battle_monitor (the only 250ms combat loop) finally migrated to Core.member. Earlier: end-user AV browsing COOKING recipes = SAME 07-21 dangling-UObject __index class but in steady-state menu browsing (pooled ListView/detail widget recycled on scroll), NOT a transition — menu adapters ARE exposed; migrated screen_cooking + shared A.list_selected_row to Core.member. Earlier: two end-user AVs from the ExecuteInGameThread flush during teardown; naked member fetch as a call argument; fixed C-array through array_of pierces pcall; nav_tracker raw #arr on streaming-freed objects.


**2026-07-26 — THE HARDENING WAS NEVER APPLIED OUTSIDE THE MENU SUBSTRATE. That is why users still
crash while the dev log stays clean.**

Users kept reporting random crashes and slow/silent menus on v0.1.2 — the release that contains
every fix in this ledger. The developer's own `UE4SS.log` from 2026-07-25 has **zero errors** across
22 minutes.

**A first reading of that contradiction was WRONG and is recorded here so nobody repeats it:** it is
NOT that the developer only tests menus while users play. The developer plays too — long sessions,
including a crash in the middle of COMBAT on 2026-07-25 — and the 29 crash dumps below prove it, with
runtimes of 647 s, 1018 s, 1792 s, 2384 s, 7912 s on the dev machine. **The real gap was that a clean
`UE4SS.log` was read as "no crashes", when the log only records what the mod PRINTED; how the process
died was sitting unread in the crash dumps the whole time.**

What IS true is where the guards had reached. Every fix this ledger describes had been applied to the
UI substrate and the menu adapters. **`nav_tracker.lua` — 3,848 lines, running every tick in free roam
AND in combat, over actors that level streaming frees and that combat destroys as they die — was never
swept.** It held 24 bare `IsValid()` calls and ~95 naked member fetches, including on `target.actor`:
a handle the radar picks and then holds for MINUTES across streaming boundaries. Since 2026-07-25 we
have known that a bare `IsValid()` does not merely fail to catch a freed handle, it **faults on it**.
So every one of those sites was a live crash on the mod's hottest, most dangling-prone path — and the
scattered runtimes are exactly the fingerprint of a race that fires whenever the engine happens to
free the object the reader is holding.

Fixed: all 24 `IsValid()` sites → `Core.valid`; 37 fetches → `Core.member`, prioritising handles
cached across ticks and the hot per-tick component paths. Also `keyhelp.lua`, which loops fetching
`Txt_Keyhelp_01..09` — a bound taken from ONE class's member count, on a bar found by scan, i.e. the
exact `bar.Txt00` shape that killed the process on 2026-07-17, waiting for the first player to open
a screen with a shorter bar.

**THE MID-COMBAT CRASH — `quest_objective.lua` is the strongest candidate, and it was hiding behind
a comment that asserted the opposite of the rule.** `first_text()` carried the note *"blueprint
members that may be absent read as nil — safe"*. They do not: an absent member is the uncatchable
abort. And the function is built ENTIRELY around trying candidate names expected to be absent
(`Txt_Main00` / `Txt_Title` / `WL_MainQuestListTitle`, …), at 300 ms, on a host **the game hides and
rebuilds when a battle starts**. `row_line` had the same shape for the M/S rows. Both now gated,
along with `guide_watch.lua`, which runs on every registry tick with or without an active adapter —
so it is live during combat and cutscenes — and fetched `win[m]` from a candidate list **as a call
argument**, the worst available shape (evaluated at the call site, outside everything).

Coverage check done the same day: the things that run CONTINUOUSLY are `battle_monitor` (already
clean — its own comment documents the 07-24 mid-combat AV and its fix), `nav_tracker`,
`quest_objective`, `guide_watch`/`keyhelp_watch`, the registry/`ui_core` substrate, and `pad_poll`
(zero UObject contact, pure native bridge). All are now gated.

**`ui_registry` had no fault isolation**, contrary to the generic-strategy doc: `is_active()` and
`update()` ran bare, so one adapter raising an ordinary error aborted the whole sweep — every
adapter below it went unpolled that tick, and the outer pcall swallowed it with no idea which one.
Both are wrapped now, and log the adapter's NAME once per session. This cannot contain uncatchable
aborts (nothing in Lua can), but it turns "the reader went quiet" into a named line.

**Three substrate holes found the same day:**
* **`Core.pane_live` was itself unguarded.** The playbook makes it the mandatory liveness test for
  every pooled-pane adapter, called with the handle the adapter cached on entry — the handle most
  likely to be dead — and it went straight to `GetVisibility()` on it. The guard everything is told
  to rely on was the unguarded call.
* **`ui_directory.prop()` had the validity half but not the property-existence half**, while two of
  its callers fetch undeclared members BY DESIGN (`find_hud` probes `UIFieldManager` precisely to
  reject the title's plain `AHUD`; the mapped chains try alternatives, so the losing branch is
  always a member the object does not declare). Now routed through `Core.member`.
* **`Mem.alive`'s transactional guard is a single point of failure**: one `GetAddress` pierce
  anywhere latches `guard.pending` and **permanently disables the memory pre-check for the rest of
  the session**. It fails open, so the mod keeps working — unprotected, with one line in the log.

**And the reason menus got SLOWER: the crash fix did it, and the scan budget was never a rate limit.**
1. `Core.valid` went from one pcall'd `IsValid` to `GetAddress` + two native reads + `IsValid`, and
   `Core.first_on_screen` runs it over the WHOLE cached pool on every call, several times per tick,
   for pools like `CFUIMultiLineTextBox` that hold hundreds of entries. Hardening the reader is what
   made the reader slow. Fixed with a per-tick memo keyed by the handle (cleared in
   `begin_scan_tick`; a validity verdict cannot change within one game-thread tick).
2. `Core.begin_scan_tick` has **six call sites** — the registry, `battle_monitor`, `quest_objective`
   (three times in its own step) and `ui_directory` — and every one refilled `scan_budget = 2`
   outright. So "2 scans per tick" was really up to a dozen, at ~65 ms each. **This is the
   playbook's own rule broken inside the substrate that enforces it: A SCAN SLOT IS NOT A RATE
   LIMIT.** It explains the measurement nobody could account for — 1576 scans in 5.5 min = 31% of
   the game thread inside `FindAllOf`, at a nominal ceiling that made that arithmetic impossible.
   Fixed: the refill is keyed to WALL TIME (`REFILL_EVERY_S`), so the ceiling means what it says.

**NEW RULE, learned by nearly shipping the bug: A PER-TICK CACHE IS ONLY PER-TICK FOR THE LOOPS
THAT CLEAR IT — enumerate the callers before you add one.** The validity memo above was added in
`Core.valid` and cleared in `Core.begin_scan_tick`, which looks airtight until you notice that
**`nav_tracker` never calls `begin_scan_tick`** — it calls `Core.poll_world()` alone, deliberately,
and says so in a comment. So the radar's cross-tick handles (the same userdata every tick, i.e.
exactly what hits a memo) would have had their verdict answered from a lookup computed one to three
nav ticks earlier, with the clear depending on foreign loops that all have early-return paths: the
registry at 100 ms (Ctrl+M stops it outright), `battle_monitor` at 250 ms (returns early during a
transition), `quest_objective` at 300 ms (returns early while an adapter is active). `Mem.alive`
would have been skipped for up to 300 ms on the handles most likely to be freed inside that
window — **enemy actors, destroyed the instant they die.** A performance fix would have become a
strictly worse version of the crash it was shipped alongside, on the exact path being fixed.
Caught by an adversarial audit, not by testing; it would have been invisible until it killed
someone's game. `Core.poll_world` now clears both memos as well. Note the precedent was already
sitting in that function: `prop_budget` is refilled there *for this very reason*, with a comment
explaining it. The lesson generalises — **when you add per-tick state, grep for every entry point
that begins a tick, not just the one you were looking at.**

**Investigated and REJECTED, do not spend a session on it:** turning off `HookProcessInternal` and
friends in `UE4SS-settings.ini`. The mod registers no hooks (`header_hook.lua` is a documented dead
end), so the per-UFunction-call detours look like free stability. They are not removable: UE4SS
drains the `ExecuteInGameThread` queue through `ProcessEvent`, which is the mod's entire poll loop.
Disabling it would produce a mod that is silent from boot.

**Upstream, from a research pass over the RE-UE4SS repo:** there is no stable release after v3.0.1
(July 2026) — only the rolling `experimental-latest`, which carries real fixes but also shipped a
new boot-crash class (#1233). There is no clean "upgrade and the crashes stop" story. Worth knowing:
**#397** (values returned from Lua-invoked UFunctions are backed by stack-scoped memory and are
unsafe to hold past the call — checked, we do not hold any) and the **string-intern-pool UAF**, not
fixed in 3.0.1, which we cannot guard against from Lua.

**WE HAD 29 CRASH DUMPS ALL ALONG AND NOBODY HAD OPENED THEM.** `%LOCALAPPDATA%\AT\Saved\Crashes\`
(UE 4.21.2) held 29 `UE4CC-*` reports from 2026-06-30 to 2026-07-25 — on the DEV machine, i.e. the
same machine whose `UE4SS.log` we had just called "clean". A clean log does not mean no crashes; the
log records what the mod printed, the dump records how the process died. **Check this folder every
session.** Note `ErrorMessage` is EMPTY and `IsAssert=false` on these, which is why they read as
"no access violation": they are not AVs, they are unhandled C++ exceptions.

Normalising each `PCallStack` to `module+offset` (the module base moves per run, the offset does
not) gives a fingerprint table. Use it to classify any future dump, ours or a player's:

| Signature (top 4) | n | Runtimes | Reading |
|---|---|---|---|
| `UE4SS+64723f > UE4SS+41e05e > UE4SS+b22544 > UE4SS+b27dc6` | 7 | 145 s – 7912 s | The dominant family. Pure UE4SS. `b22544 > b27dc6` recurs under other top frames, so it is the shared dispatch boundary and the top frame is the varying call site |
| `KERNELBASE+c1c0a > VCRUNTIME140+55a9 > UE4SS+b28684 > UE4SS+b28619` | 4 (all 07-14) | 286–2435 s | **`RaiseException` ← `_CxxThrowException`** — the uncatchable `0xe06d7363` throw, named in machine code at last. The widget-feed era |
| `VCRUNTIME140+1e4fd > UE4SS+42482b > UE4SS+42449b > UE4SS+41e89e` | 2 | 647 s, 3169 s | Includes the MOST RECENT crash (07-25 10:13). Same throw path, different site |
| `UE4SS+b27d28 > UE4SS+b354b6 > …` | 2 (07-24) | 1018 s, 1792 s | The end-user-report era |
| `AT-Win64-Shipping+1f0b83b > …` | 2 (07-21) | 18 s, 27 s | **NOT us** — the documented `Could not find SuperStruct AutoDebugUIBase` dumper crash (`LoadAllAssetsBefore*`), dev-only |
| `AT-Win64-Shipping+15cd992 > …` | 1 (07-03) | 7919 s | **NOT us** — base-game crash after 2.2 h |
| `VCRUNTIME140+10c5e/10aa7/1062a > …` | 5 (06-30, 07-01) | all 0 s | Solved long ago: the UE4SS address-resolution startup saga |

So **26 of 29 died inside `UE4SS.dll`** — which is where the mod's Lua executes, so they are ours —
and the throw path is confirmed to be a C++ exception raised by UE4SS, not a stray memcpy. Runtimes
scatter from 145 s to 7912 s, which is what a dangling-handle race looks like: it fires whenever
streaming happens to free the object the reader is holding. Exactly the family this batch targets.

**Prevention — this class stops being rediscovered:** `tools/lint-lua.ps1` now enforces all three
checks (syntax, globals, guards) over all 70 files and `package.ps1` runs it as a hard gate before
staging. Nothing validated the Lua on the way out before; a release could ship a file that did not
compile. `raw-GetAddress` is a WARNING, not an error, deliberately: on a UObject it is the safe
call, a regex cannot tell a UObject receiver from a TArray one, and a lint people learn to ignore
protects nothing.

**SHIPPED: A CRASH BLACK BOX — the next crash names its own site.** `mem_bridge` gained
`mark_open(path)` / `mark(text)` / `mark_flush()`: a 64-slot ring in a MEMORY-MAPPED FILE
(`Scripts/crash_trail.bin`, gitignored and stripped from releases). `mark()` is a memcpy, cheap
enough to call on **every adapter probe** — so the ring holds the last ~180 ms of what the mod was
doing. Because the page is file-backed, the memory manager writes it out when the process dies.
`main.lua` opens it at boot, prints the PREVIOUS session's trail into `UE4SS.log` under *PREVIOUS
SESSION ENDED HERE*, then resets it. Marked so far: every `is_active`/`update` by adapter name, and
the `nav`/`quest`/`battle`/`guide` loop steps.

*Rejected alternative:* a vectored exception handler. It fires on every first-chance exception, and
both the game and UE4SS throw-and-catch routinely — noisy, and a new way to destabilise a process
that is already dying.

**TESTED STANDALONE, AND THE TEST IS WHY IT WORKS.** Built `lua.exe` from the vendored 5.4.4,
wrote 71 marks from one process, killed it with `TerminateProcess` (no cleanup — exactly a hard
crash), and recovered all 64 ring entries from a second process, `LAST_THING_BEFORE_CRASH` last.
The first build **faulted on recovery**: 64 slots × 128 B = 8192 plus a 32 B header = 8224, mapped
into 8192, so the last two slots wrote past the view. That would have been a crash diagnostic that
crashed the game at boot, on every launch, shipped to fix crashes. The size is now derived with a
compile-time assert. **Rule: a diagnostic gets tested like a feature — compiling is not evidence.**

**Also shipped: a crash breadcrumb.** `ui_registry` now prints one `screen -> <adapter>` line per
screen commit (a few per minute, well inside the no-spam rule). Every user crash so far has been
un-diagnosable because the mod is silent in the steady state, so the tail of a player's log said
nothing about what was on screen. README gained a *Reporting a crash* section with the log path and
the warning that **UE4SS.log is overwritten on every launch** — copy it before restarting.

---

**2026-07-25 (c) — NEW VALIDATION STEP: `luac -p` IS NOT ENOUGH. Lint the compiled GLOBALS.**
The adversarial review of the same day's work caught a change that would have made the mod
**completely silent from boot**: `Core.poll_world` called `dir_mod()` 74 lines ABOVE the
`local function dir_mod` that defines it. In Lua a local's scope starts at its own statement, so
that call compiled to a GLOBAL lookup, was nil at runtime, and raised from a spot ABOVE the
loops' `pcall(step)` — every tick dead, nothing in the log. **`luac -p` accepts it: a call to an
undefined global is valid syntax.** The check that finds it reads the bytecode:
```
luac.exe -l -p FILE.lua | grep -oE '_ENV "[A-Za-z_][A-Za-z0-9_]*"' | sort -u
```
Everything listed is a global access; anything not a Lua builtin or a UE4SS global (`FindAllOf`,
`FindFirstOf`, `StaticFindObject`, `LoadAsset`, `LoopAsync`, `ExecuteInGameThread`,
`NotifyOnNewObject`, `RegisterHook`, `RegisterKeyBind`, `RegisterCustomProperty`,
`PropertyTypes`, `Key`, `ModifierKey`) is a bug. **Run it over every file after any edit, next to
`luac -p`.** The first sweep found two PRE-EXISTING ones nobody had noticed:
* `screen_community.lua`: `clear_state()` assigned `last_sub`, declared local ~530 lines later —
  so it wrote a global and the link-bonus subtitle latch was NEVER cleared. A link bonus shown
  twice in a session announced only the first time. Fixed with a forward declaration.
* `header_hook.lua`: the dead `_install_experimental` still iterated `CANDIDATES`, renamed to
  `TRIED` long ago — harmless today (never called), fatal the day someone re-enables it.
This is the same failure shape as the guards themselves: **the tool you trust does not check the
thing you assumed it checked.** Same lesson as `IsValid()`.

**2026-07-25 (b) — THE OTHER TWO UNCATCHABLE CLASSES ARE NOW GUARDED BY THE ENGINE'S OWN
METADATA, and the last construction notify is GONE.**
1. **"member the class does not have"** (the 2026-07-17 `bar.Txt00` kill, a member of the SIBLING
   bar class): `Core.member` now consults a per-class PROPERTY-NAME SET built with
   `UStruct:ForEachProperty` plus a `GetSuperStruct()` walk — ForEachProperty lists a class's OWN
   properties only, so the walk is what makes inherited members resolvable. Cached by class
   address, cleared on transition (a map switch is exactly when BP classes unload and an address
   can be reused), at most ONE class enumerated per tick (this codebase's whole perf history is
   about not letting per-class work cluster), fails open. Blocked fetches log
   `member gate: <Class> has no '<Member>'`, capped at 50 — an over-rejection must be visible.
   **TRAP FOUND WHILE BUILDING IT: `RegisterCustomProperty` members are INVISIBLE to
   ForEachProperty** (UE4SS keeps them in its own map, consulted by `__index` as a fallback; they
   are never added to the UClass). The mod uses them to recover collapsed FIXED C arrays —
   screen_party's party slots 1/2, screen_community's skill parts 1..9. Those sites fetch raw
   today so nothing was broken, but the standing "route fetches through Core.member" rule would
   have silently killed both screens. They now declare themselves with `Core.allow_member()` at
   registration time; **anyone adding a custom property must do the same.**
2. **The FIXED C-ARRAY pierce** (2026-07-16, `screen_dialog.WL_LvTextList`): the property walk
   records each member's TYPE for free (it already asked `prop:GetClass():GetFName()`), and a real
   TArray is an `ArrayProperty` while a fixed C array is one ObjectProperty with ArrayDim > 1. So
   `Core.array_of` refuses anything the class declares as a non-ArrayProperty. That code's own
   comment used to read "there is no runtime check for this: the caller must never pass a
   fixed-array member here — check the CXX header dump." There is one now, and it does not depend
   on a human remembering. Logs `array gate: '<Member>' is a <Type>, not a TArray`.
3. **`NotifyOnNewObject("/Script/Engine.GameModeBase")` REMOVED** — the last construction notify
   in the mod, i.e. the last place mod Lua could run on the engine's async loading thread and race
   the poll loop on the shared `lua_State`. It also armed too LATE (the GameMode is constructed
   after the old world's objects are freed, and our tick DOES run in between — LoadMap fires dozens
   of UFunctions through ProcessEvent, each draining our queue). Replaced by
   `Directory.world_epoch()`: GameInstance (never dies, cached after one lookup) → `GetWorld()` →
   address, polled by `Core.poll_world()` from `begin_scan_tick` (every loop) and directly from
   nav_tracker (the only loop touching UObjects outside it — with the reader toggled off the
   registry loop stops and the gate would never arm). Deliberately NOT the PlayerController: this
   game has several and they swap mid-world when mounting, so the epoch would flap and flush caches
   spuriously. 60 s failsafe releases the gate if no world is ever found — a MUTE mod is worse than
   an unguarded one, and the memory pre-check now protects the reads. This also retires the
   "root_ok is CIRCULAR" blocker from 2026-07-21: probing a dead root is safe now.

**2026-07-25 — ROOT CAUSE OF THE WHOLE AV FAMILY, READ OFF THE UE4SS SOURCE. `IsValid()` IS
ITSELF THE DEREFERENCE.** Ten months of this ledger say "IsValid lies". It does not lie — it
*faults*. RE-UE4SS v3.0.1, `UE4SS/include/LuaType/LuaUObject.hpp:610`:
`m_cpp_object && !m_cpp_object->IsUnreachable() && is_object_in_global_unreal_object_map(m_cpp_object)`.
`IsUnreachable()` READS THE OBJECT, and it is evaluated **before** the only part that could
catch a freed handle — the lookup in UE4SS's own object set. So on a dangling pointer the
access violation happens inside the very call that was supposed to reject it, and no pcall can
catch it. Two more facts from the same source:
* UE4SS never clears the raw pointer inside a Lua handle. Its delete listener
  (`LuaUObject.cpp:59-66`) only erases a hash from `s_lua_unreal_objects`. A freed object — or
  one whose ADDRESS a new object recycled, which re-inserts the same hash — passes `IsValid()`
  and dies at `prepare_to_handle` → `GetClassPrivate()`, i.e. **UObjectBase+0x10: the exact
  faulting address in every user report.**
* There is NO safe liveness call in the Lua API. `GetAddress()` is the only method that does not
  dereference; `IsA`, `GetFullName`, `HasAnyInternalFlags`, `GetClass` all touch the object.
  `FWeakObjectPtr` is exposed but cannot be constructed from a UObject in Lua.
**DEAD END, do not spend a session on it: `bUseUObjectArrayCache = false` is NOT a fix.**
`IsValid()` never walks GUObjectArray in either mode; the setting only gates the LiveView
listeners, its own ini text scopes it to *startup* crashes, and issue #772 shows Lua
`IsA(string)` silently stops honouring inheritance when it is off. Likely a net negative.
**THE FIX (applied): pre-check the memory OURSELVES, before UE4SS touches it.** `Mem.alive(obj)`
(mem.lua) asks the handle only for its stored pointer (`GetAddress`, no deref), then reads the
object's class pointer and that class's own class pointer **through mem_bridge**, whose reads
are SEH-guarded and return nil for an unreadable address instead of faulting the process. Freed
or garbage memory fails one of those reads or yields something that is not a UObject. It is not
a liveness proof — an address recycled by another *live* object still reads fine — but it turns
the dangling/garbage class from "kills the process" into "returns nil", and it is the ONLY guard
that can run before `IsValid()`. `Core.valid` and `Core.nonnull` now go through it, and so does
`Mem.addr` (every native offset read). The `ClassPrivate` offset is **derived at runtime**, not
hardcoded: take a UClass found by path, get its class pointer by reflection, and find that value
in the object's first bytes — a game/engine patch re-derives it. Fails OPEN everywhere (no
bridge, no offset, unanswerable handle ⇒ proceed as before). Boot log says
`UObject class pointer at +0xN (derived)`; rejections are counted and logged every 200.

**2026-07-25 (g) — CRASH RECURRENCE, known signature, and the trigger was a per-tick scan I had
just introduced.** Stack: `VCRUNTIME140` → UE4SS **+0x1482b / +0x1449b / +0xe89e** (subtract the
UE4SS.dll base ≈ 0x7fffa3ed0000) → ~25 recursive frames in **+0x71xxxx–0x73xxxx** (the Lua-VM range)
→ dispatcher → `AT-Win64-Shipping`. That is BYTE-IDENTICAL to the 2026-07-21 signature: property
`__index` on a **dangling** UObject. Faulting address `0xdb6a46e8` = recycled garbage, not null and
not 0x10 — i.e. **precisely the residual hole `Mem.alive` documents about itself**: an address
recycled by another LIVE object passes every readability check. The pre-check is not at fault; it
never claimed this case.
What made it fire: `screen_options.refresh_rows` had just been changed (round 1 of the re-entry fix)
to run whenever the staleness test failed — and the test fails for as long as the pooled host
lingers `on_screen` after the menu closes, which is forever. **A once-per-session `FindAllOf` became
a ~65 ms full-object scan every tick.** Two consequences, both observed: story dialogue went silent
(it is scan-detected, and the shared 2-per-tick budget was being drained), and the dangling-object
exposure multiplied — a full object-array walk every 100 ms is the per-tick-retry escalation this
ledger has warned about since 2026-07-13. It also survived a mod reload, which was the tell: the
state causing it belonged to the GAME (a parked pooled widget), not to the mod.
Fixes: `screen_options` now gates the host on `Core.pane_live` as well as `on_screen` (it was
violating the CLAUDE.md §8 pooled-pane rule — the only adapter of its family that did), and
`refresh_rows` has an explicit `RESCAN_EVERY = 10` backoff.
RULES: (1) **a scan SLOT is not a rate limit.** `Core.take_scan_slot` grants 2 per tick — it
apportions a budget between competing callers, it does not stop one caller from asking every tick.
Anything that can be called every tick needs its own backoff, in addition to the slot. (2) When a
guard's failure mode is "re-scan", ask what happens if the condition is permanently true. (3) The
residual crash hole is now precisely known and needs the CLASS-POINTER STAMP: record each cached
object's `ClassPrivate` when it is first validated and require it to still match. A recycled address
holding a different class is then rejected; recycled-into-the-same-class remains readable, which is
safe. That is the next durable step.

**2026-07-25 (f) — SAME BUG SHAPE, THREE PLACES IN ONE BATCH: a derived engine signal FLAPS.**
The day's own fixes introduced two new symptoms — the result screen going silent and the audio
cutting every ~1.5 s in cutscenes — and both were one shape:
1. **`battle_hud_live()` in ui_registry.** Making the falling edge unconditional (needed, so a
   battle ending behind subtitles is seen) exposed that the pooled HUD collapses/uncollapses, so the
   raw signal fired ~5 "battle ended" events per battle. Each one opened a 3 s hot window that
   DEFEATED cinematic quiet mode and re-armed a per-class watch: the log shows **124
   `watch Gameover_C` lines** where one window gives ~25, i.e. a ~65 ms `FindAllOf` every 0.8 s —
   audible as the audio stutter — while eating the 2-scans-per-tick budget. Fixed with
   `ABSENT_TICKS = 5` on the falling edge only.
2. **`note_epoch` in transition.lua — the same shape, one layer down and far worse.** It armed the
   gate on a SINGLE nil epoch, and every later nil re-extended the grace. And **while the gate is
   up, `Directory.resolve` returns an EMPTY list for EVERY mapped class** (by design: no scans
   mid-load), so one flicker of `Core.valid(w)` around a battle end silences the entire screen
   directory for `GRACE_S`. That is the leading explanation for the result screen. Fixed with
   `DOWN_CONFIRM_S = 0.3` measured in WALL TIME — `note_epoch` runs from every loop several times
   per tick, so a call counter would confirm in milliseconds and confirm nothing. Epoch CHANGES are
   never debounced (two different valid worlds is unambiguous).
3. `begin_transition` now LOGS its trigger (`transition gate ON (world gone|new world)`). A false
   arm was indistinguishable from a real one in the log, which is exactly why this cost a round.
RULES: **a derived engine signal flaps — debounce the edge that costs something, in wall time, and
log when it fires.** And: a cheap signal that gates expensive machinery deserves more scepticism
than the machinery does.

**2026-07-25 (e) — the SEVENTH silent screen was NOT the pierce: a stale hand-rolled cache.**
Six of the user's seven reports are the `GetAddress`-pierce below, but **Options-on-re-entry appears
ZERO times in the 510 tracebacks** — and that absence is what identified it as a separate bug
instead of another symptom. Cause: `screen_options` hand-rolls a module-level `rows` cache (it has
to filter the shared `Xlist_Bar03_C` pool by owner rather than use `first_on_screen`), its staleness
test asked only `Core.valid(rows[1])`, and `Options.reset()` never cleared it. **This game only
COLLAPSES a submenu's widgets on close** — so visit #1's rows stay valid forever, the test passes,
`A.scan_list` sees no visible `Ins_Cursor_Fad` on the orphaned rows, `low` is nil every tick, and
`update()` returns before reaching the announcer. It read on the FIRST entry only because the cache
starts empty, which is exactly what made it look intermittent. Fix: staleness test also requires
`Core.on_screen(rows[1])` (what `first_on_screen` does for every other list screen), `reset()`
clears `rows`, and the re-scan is budgeted via `Core.take_scan_slot()` (raw `FindAllOf` ~65 ms, and
the test can be false several ticks running). LESSON, now a playbook rule: **a hand-rolled widget
cache must be invalidated by `on_screen`, not by validity** — and the diagnostic signature of this
bug class is "works the first time, silent every time after", with NOTHING in the log.

**2026-07-25 (d) — THE ACTUAL MECHANISM BEHIND THE SILENCED MENUS, FROM THE USER'S LOG.
`GetAddress()` on a NON-UObject RemoteObject raises an error that PIERCES pcall.** This
CORRECTS the entry below, whose diagnosis (a guard failing closed on a nil answer) was the wrong
mechanism — reasoned from the code without the log. The log says it plainly:
```
Error: Call to RemoteObject:GetAddress on polymorphic type is not allowed, please override GetAddress.
  [C]: in method 'GetAddress'  →  mem.raw_addr  →  mem.alive  →  ui_core.valid
  →  ui_core.array_of  →  screen_saveload.bar_list → … → screen_saveload.update
```
510 occurrences in one session, and **every single one entered through `Core.array_of`'s validity
check on the ARRAY**. `GetAddress` is overridden only on the UObject family; on a TArray wrapper
UE4SS's base implementation raises — and that error does NOT stop at `pcall`, it unwinds to
UE4SS's own callback boundary, so the adapter's `update`/`is_active` **dies mid-function** while
every enclosing pcall reports success. The screens named in the tracebacks are exactly the ones
the user reported silent: `screen_saveload` (1248 frames), `screen_status` (1104 — the d-pad stat
walk), `nav_tracker` aim_watch (468), `screen_skillcustom` (208 — the super-attack palette),
`screen_community` (148 — the emblem grid), `screen_tutorials` (136), `keyhelp` (102),
`screen_skilltree` (88 — the skill tree), `screen_dialog.choices` (56 — which is the "save
changes?" confirm in Options). The 2026-07-24 cut put the same `GetAddress` call in `Core.valid`,
so this is ALSO what silenced them that day — the "fail closed on nil" story was wrong, and the
fail-open fix was correct but for the wrong reason and could not have restored them.
FIX: **`Core.valid_ref(o)`** — `IsValid()` only, no address — and `Core.array_of` uses it for the
ARRAY (the owner is a UObject and keeps the full check). `IsValid` on a non-UObject RemoteObject is
a bare null-check (`LuaUObject.hpp:178`), so it is safe and cheap; the dead-owner case is already
caught one line above. Plus a **self-disabling guard** in `Mem.alive`: the attempt is marked
pending in `_G` before the call and cleared after, so if a future stray call pierces again, the
next tick sees the pending mark, disables the pre-check for the session and logs it — worst case
one lost tick and a log line instead of seven menus dead.
RULES: (1) **only UObject-family handles go through `Core.valid`/`Mem.alive`; TArray and struct
handles go through `Core.valid_ref`.** (2) When a batch of screens goes quiet, READ UE4SS.log
BEFORE theorising — a pcall-piercing error always leaves a traceback naming the exact line, and
this one named it 510 times while three rounds of code reasoning got the mechanism wrong.
(3) `pcall` proves nothing on this game: treat "wrapped in pcall" as no protection at all.

**2026-07-25 — THE 07-24 HARDENING SILENCED SCREENS (SUPERSEDED DIAGNOSIS — see the entry above
for the real mechanism; kept because the fail-open RULE it produced is still right).**
User report: after ecf29f3 several menus stopped reading
(load-game screen, the save confirmations in Options). Mechanism — the new gate was
`if oka and (a == nil or a == 0) then return false end`, i.e. it treated a `GetAddress()` call
that RETURNS NOTHING as "the pointer is null". Those are different questions: nil means the
wrapper cannot answer, null means the object is not there. And the blast radius is not one
widget: **`ui_directory` gates EVERY pointer hop of every chain on `Core.valid`**
(`prop()` at ui_directory.lua:52-58, `resolve` at :296-311), so a single unanswerable hop makes
a directory-mapped screen resolve to NOTHING — silent, with no error logged anywhere, which
reads exactly like "the adapter never found its host". `AT_UIStartSaveLoad` is directory-mapped
through `mm.m_xSaveMenu/m_xLoadMenu.m_UIStartSaveLoad` (ui_directory.lua:166-175) — that is the
load screen. FIX: reject only on `type(a) == "number" and a == 0`; `Core.nonnull` keeps the
fail-CLOSED variant for brush resources (there the question always applies and a miss costs one
glyph name, not a screen). Rate-limited log line added: `valid(): N NULL-handle rejections so
far` — if the gate is ever doing real work it now says so in UE4SS.log, and if a screen goes
silent again the absence of that line rules the gate out in one look.
RULE (general, worth more than this fix): **a guard added to prevent a crash must fail OPEN on
an unanswerable signal.** A crash costs one session; a guard that silently fails closed on a
shared substrate costs every screen that depends on it, invisibly. Two other things landed the
same day compound this: pin/dedup state scoped to the whole map epoch, and a new always-on
probe — see the two entries below.
**Same day, second regression — `screen_dialog`'s `pinned_set` was EPOCH-scoped.** Pins added
07-24 to stop a long-PARKED notice window re-firing after the 24-entry recent FIFO evicted its
marks were cleared only on a map transition, so every notice that legitimately repeats inside
one map was mute for the rest of the session — the Options save confirmations ("¿Guardar?",
"Guardando…"). FIX: pins are scoped to the window PRESENCE, released after the window has been
continuously off screen for `PIN_CLEAR_S = 2.0 s` (far longer than the tick-to-tick blink the
recent-set exists for, so a blink can never unpin and a genuine reopen always can). The parked
window the pin was for stays ON screen, so it is still covered.
**Same day, third — `screen_gameover` probed an unmapped per-level class in every state, from
registry slot 4.** `Core.first_on_screen("Gameover_C")` ran on every poll: a full `FindAllOf`
every `ABSENT_BACKOFF` forever, competing for the 2-scans-per-tick budget that the
late-registry menus depend on (the 2026-07-15 precedent names items/**saveload**/characters/
skill-tree going SILENT when that budget is denied), and it re-violated the 2026-07-17 rule
"never probe a per-level pool unconditionally from an always-running adapter" — the
return-to-title AV class. FIX: the probe runs only inside the ~8 s wall-clock window armed by
the battle-HUD falling edge, or while the adapter already holds a live host; outside that the
adapter costs nothing and cannot claim the tick. (Note for the diagnosis record: saveload is
directory-mapped, so budget starvation is NOT what silenced it — the `Core.valid` gate above
is. The gameover probe was a real cost and a real crash-surface regression regardless.)

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
