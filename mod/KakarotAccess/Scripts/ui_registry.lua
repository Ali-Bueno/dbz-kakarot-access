-- Registry + dispatcher for the hybrid UI architecture.
--
-- Holds the registered screen adapters and runs the single poll engine: each tick
-- it finds the first active (visible) screen, and on a screen change resets the
-- announcers (so the new screen re-announces from scratch), then drives the active
-- adapter's update(). Adding a screen = register it; no engine changes.

local Core = require("ui_core")
local Mem = require("mem")            -- crash black box only (Mem.mark)
local Transition = require("transition")
local KeyhelpWatch = require("keyhelp_watch")
local GuideWatch = require("guide_watch")
local Input = require("input")

local Registry = {}

local adapters = {}
local active = nil
local enabled = false

-- Switch debounce: a screen must be the top active adapter for this many consecutive
-- polls before we commit to it. Stops a screen that's active for only a moment from
-- announcing — e.g. the title menu flashing on-screen for a tick at boot right before the
-- "Checking system data…" dialog appears (which would blurt "Main menu, Continue" first).
local CONFIRM_TICKS = 2
local pending, pending_n = nil, 0

-- Sticky-active sweep cadence. Polling all ~33 adapters' is_active() every tick was the
-- measured 40ms-avg game-thread step (Ctrl+F5, 2026-07-14): most of a sweep re-probes
-- screens that cannot have changed. While an adapter is active we poll ONLY it, and run
-- the full priority sweep every SWEEP_EVERY ticks — enough to catch a higher-priority
-- overlay (a dialog popping over a menu) within ~300ms. The sweep runs EVERY tick when
-- nothing is active (free-roam: menu-open detection stays at one tick) and while a
-- pending switch is being confirmed (so CONFIRM_TICKS still counts consecutive polls).
local SWEEP_EVERY = 3
local tick_n = 0

-- Idle-sweep hot window (cinematics-lag pass, 2026-07-16). With nothing active the
-- sweep used to run EVERY tick — free-roam paid the full ~33-adapter probe at 100ms,
-- and so did every camera cutscene (no adapter active while the game thread is at its
-- busiest). Now the idle sweep also runs every SWEEP_EVERY ticks, EXCEPT inside a hot
-- window (~1s after any fresh pad press, or after a screen commit): those are the only
-- instants a user-opened menu can be appearing, so open latency is unchanged. Only
-- event-less appearances (a dialog the game opens on its own) can detect up to ~200ms
-- later — within the reader's existing CONFIRM_TICKS envelope.
local HOT_TICKS = 10
local hot_until = 0

-- Whole-cutscene quiet (2026-07-16 night, the per-class Ctrl+F5 dump): the classes
-- still scanning through a cinematic were all MENU screens that cannot appear there
-- without a press (community trio, dragon-ball menu, map icons, title, choice
-- windows) — one ~73ms hitch every ~2.5s. A camera cutscene shows no active adapter,
-- no minimap and no battle HUD: treat that whole state as quiet. Guards: any fresh
-- press lifts it for the hot window (skip confirms), ui_core exempts the
-- auto-appearing narrative classes (subtitles/talk/telop/notice), and it engages
-- ONLY in a gameplay world — `Dir.root_ok("mm")`, the gameplay GameMode's own
-- MenuManager root, unreachable at boot/title — so title/boot detection keeps its
-- scans. (2026-07-17: this world predicate replaced two broken session-history
-- heuristics, free-roam-seen and a dialogue grace: neither armed on saves that load
-- DIRECTLY into a paused cinematic — the 666-scans/109s dump — and the grace
-- additionally died the moment the subtitles-off gate silenced the very dialogue
-- adapter it keyed on.)
local Dir = require("ui_directory")

-- Battle-end hot window: the results screens appear EVENT-LESS right as the battle
-- HUD leaves (no press, no minimap yet) — exactly when quiet would engage. The
-- falling edge opens a scan window so they read on their own.
local prev_battle = false
-- DEBOUNCE (2026-07-25). Making the edge unconditional exposed that `battle_hud_live()` FLAPS:
-- the pooled HUD collapses and uncollapses during cutscenes and battle phases, so the raw signal
-- produced ~5 "battle ended" events where there was one. Each false edge opened a 3 s hot window
-- (defeating cinematic quiet mode) and re-armed a per-class watch, and the user's log showed the
-- cost: 124 `watch Gameover_C` lines where one window should give ~25 — a ~65 ms FindAllOf every
-- 0.8 s, which is audible as the audio cutting every second and a half during cutscenes, and
-- which ate the 2-scans-per-tick budget the other screens live on. So the HUD must be absent for
-- several consecutive polls before we believe the battle ended. Only the FALLING edge is
-- debounced; appearing is taken at face value (a battle starting is never ambiguous).
local ABSENT_TICKS = 5      -- ~500 ms of continuous absence
local battle_absent = 0

-- Scan-free on purpose (Core.peek_all): this runs per tick while no adapter is
-- active, and its cached_all form was itself scanning the absent HUD class every
-- backoff through the cinematic (n=24 in the 21:11 dump) — a probe must never cost
-- what it exists to prevent. In a real battle the directory ({"bm"}) serves the HUD.
local function battle_hud_live()
    for _, w in ipairs(Core.peek_all("Battle_Hud_P_Main_C")) do
        if Core.valid(w) and Core.on_screen(w) then return true end
    end
    return false
end

-- Menu-open boost from the PAD. The registry's commit boost cannot see ring→submenu:
-- the ring/pause stays visible (and active) under the submenu, so no screen change ever
-- commits and a scan-path submenu waits out its full backoff (the "items/emblems take
-- forever" flow, 2026-07-15). A confirm/back/start PRESS is the missing event — it is
-- the exact moment a submenu may be appearing. Read at the registry's own 100ms cadence
-- (a real menu press holds longer than one tick, so the edge is caught), and gated on
-- "some adapter is active": in combat/free-roam (no active adapter) button mashing must
-- never trigger scan work.
local TRACE_COMMITS = false -- 2026-07-16 emblems diagnosis: log screen commits by adapter
                            -- index (cross-ref app.lua order). Re-enable to name a
                            -- shadowing adapter when a screen reads late/never.

local BOOST_BTNS = Input.BTN.A | Input.BTN.B | Input.BTN.X | Input.BTN.Y | Input.BTN.START
local BOOST_COOLDOWN = 3    -- registry ticks between pad-triggered boost windows. Not 1:
                            -- an uncooled boost per press turned list navigation into a
                            -- permanent scan storm (57k scans / 1s spikes, 2026-07-15).
                            -- Not 10 either: a menu is often opened by the SECOND of two
                            -- quick A presses (submenu pick → confirm), and a 1s cooldown
                            -- swallowed that press's window while the first press's scan
                            -- had already consumed the class credit BEFORE the screen
                            -- existed — the "3-4s to read" residue. 300ms lets the
                            -- opening press always start a fresh window.
local prev_btn = 0
local boost_cool = 0
local function pad_boost()
    local snap = Input.read()
    if not snap then return end
    local fresh = snap.buttons & ~prev_btn
    prev_btn = snap.buttons
    if fresh ~= 0 then hot_until = tick_n + HOT_TICKS end
    -- No boost while a quiet overlay owns the screen (subtitles): mashing A through
    -- dialogue is "advance the line", not "open a menu" — each press used to open a
    -- boost window and re-scan every missing pool once per window, a steady scan
    -- drip exactly during cutscenes. The windows a dialogue CAN open (skip confirm,
    -- choices) live on pooled, already-cached widgets — no scan needed to see them.
    if active ~= nil and not active.scan_quiet
        and (fresh & BOOST_BTNS) ~= 0 and tick_n >= boost_cool then
        boost_cool = tick_n + BOOST_COOLDOWN
        -- NOT marked, and the reason is worth keeping: a mark was added here on 2026-07-29 (e) on
        -- the theory that this was the prologue's dangerous part. It is not — `Core.boost_missing`
        -- is three lines of pure Lua arithmetic (ui_core.lua:1038-1042) that only widen a time
        -- window; the FindAllOf it eventually permits happens inside `Core.cached_all`, called from
        -- adapter probes, every one of which `probe()` already marks. Marking inert code costs ring
        -- capacity (64 slots, ~172 ms of history) and, worse, points the next investigation at the
        -- wrong place. The prologue's one real dereference is the world-epoch read, marked
        -- `core.world` in `Core.poll_world`.
        Core.boost_missing()
    end
end

-- `name` is the adapter's module name, used only for the crash breadcrumb below. Optional, so
-- an adapter registered without one still works — it just shows up as "?" in the log.
local adapter_name = {}
function Registry.register(adapter, name)
    adapters[#adapters + 1] = adapter
    if name then adapter_name[adapter] = name end
end

-- FAULT ISOLATION (2026-07-26). Every adapter probe used to run bare, so one adapter raising an
-- ordinary Lua error aborted the WHOLE sweep for that tick — every adapter below it in the list
-- went unpolled, and the only trace was the loop's outer pcall swallowing it with no idea which
-- one was at fault. Two things this buys, both of which the generic-strategy doc asks for and the
-- code did not do: a broken screen now degrades LOCALLY instead of silencing everything under it,
-- and the log NAMES the culprit.
--
-- Honest limit: this cannot contain the UNCATCHABLE aborts (a fetch on a freed handle, a member
-- the class does not declare) — those unwind below the Lua boundary and kill the process whatever
-- pcall is on the stack. Guarding the call sites is the only defence against those. What this
-- catches is the ordinary-error case, and — the reason it earns its place while we are hunting a
-- crash — it turns "the reader went quiet" into a named line in the user's log.
local fault_logged = {}
local function probe(a)
    -- Black-box mark BEFORE the call, not after: the whole value is that if this adapter's
    -- is_active is the one that kills the process, the last thing in the ring is its name.
    -- One memcpy; the sweep does ~36 of these per tick and that is still nothing.
    Mem.mark("ui.is_active " .. (adapter_name[a] or "?"))
    local ok, r = pcall(a.is_active)
    if ok then return r end
    local nm = adapter_name[a] or "?"
    if not fault_logged[nm] then
        fault_logged[nm] = true       -- once per adapter per session: never a per-tick storm
        print(string.format("[KakarotAccess] adapter '%s' faulted in is_active: %s\n",
            nm, tostring(r)))
    end
    return false
end

-- The THIRD adapter entry point. `is_active` and `update` have been fault-isolated since
-- 2026-07-26; `reset` was left bare — and it is the most dangerous of the three, because it is
-- called from the COMMIT branch BEFORE `active` is assigned. An error there unwinds to Core.loop's
-- pcall with `active` still pointing at the outgoing adapter and `pending`/`pending_n` still
-- holding the candidate, so the next tick recomputes the same candidate, re-enters the same
-- branch and raises again — forever, skipping the quiet gate, `active.update` and both watchers
-- every time. Latent today (no adapter's reset raises) and permanent the day one does.
local function safe_reset(a)
    if not (a and a.reset) then return end
    local ok, err = pcall(a.reset)
    if ok then return end
    local nm = adapter_name[a] or "?"
    if not fault_logged[nm .. ":reset"] then   -- lint:plain-table
        fault_logged[nm .. ":reset"] = true    -- lint:plain-table
        print(string.format("[KakarotAccess] adapter '%s' faulted in reset: %s\n",
            nm, tostring(err)))
    end
end

-- The two watchers were the last unguarded calls in the step, and they touch live widgets on
-- every tick. One raise in either aborts the rest of the dispatch at Core.loop's pcall — so they
-- get the same once-per-name log the adapters get, instead of taking the reader down silently.
local function safe_call(what, fn, arg)
    local ok, err = pcall(fn, arg)
    if ok then return end
    if not fault_logged[what] then   -- lint:plain-table
        fault_logged[what] = true    -- lint:plain-table
        print(string.format("[KakarotAccess] %s faulted: %s\n", what, tostring(err)))
    end
end

local function sweep()
    for _, a in ipairs(adapters) do
        if probe(a) then return a end
    end
    return nil
end

local function step()
    -- BLACK-BOX MARK FOR THE REGISTRY PROLOGUE (2026-07-29 (e)). Everything from here down to
    -- the first `probe()` used to be UNINSTRUMENTED, and that blind spot cost a whole round of
    -- analysis: a reporter's trail ended in `nav.explore`, which reads as "the radar killed it"
    -- — but the active adapter was `screen_gameover`, which mutes the nav loop, so BOTH `step()`
    -- and `explore_tick` had returned at their gates without touching a single engine object.
    -- The real death window was this prologue (the transition check, then `pad_boost` →
    -- `Core.boost_missing`, which re-scans missing pools), invisible because the next mark only
    -- gets written once the adapter sweep starts. Exactly the situation that made `nav.explore`
    -- its own mark in 2026-07-26 (c): when two candidates share an unmarked window, the fix is a
    -- mark, not a theory. One memcpy per tick against the ~36 the sweep already writes.
    Mem.mark("ui.tick")
    -- Transition gate FIRST (pure Lua): during a map switch the adapters' cached
    -- widgets are dying, and even an is_active() probe can be an uncatchable abort.
    -- Drop the active adapter so the post-load screen announces fresh.
    if Transition.active() then
        safe_reset(active)
        active, pending, pending_n = nil, nil, 0
        safe_call("KeyhelpWatch.screen_changed", KeyhelpWatch.screen_changed, nil)
        -- Map load in progress: the game thread is at its busiest — relax the 20ms
        -- pad dispatcher too (pad_poll reads this global at its own cadence).
        _G.__KakarotPadRelax = true
        return
    end

    tick_n = tick_n + 1
    pad_boost()
    local cur
    if active and pending == nil and tick_n % SWEEP_EVERY ~= 0 then
        -- Sticky fast path: the active screen is still up → keep it, poll nobody else.
        -- The moment it stops being active, fall through to a full sweep THIS tick.
        cur = probe(active) and active or sweep()
    elseif active == nil and pending == nil and tick_n > hot_until
        and tick_n % SWEEP_EVERY ~= 0 then
        -- Idle throttle (see HOT_TICKS): nothing active, no switch being confirmed,
        -- no recent press — skip the sweep this tick. cur == active (nil), so the
        -- commit logic below is a no-op and GuideWatch still runs.
        cur = nil
    else
        cur = sweep()
    end

    if cur ~= active then
        -- Wait for the switch to be stable before committing; during the wait we announce
        -- nothing (neither the old nor the candidate screen). An adapter may ask for a
        -- longer confirm via `confirm_ticks` — the title menu does, so it doesn't blurt
        -- "Main menu, Continue" in the short gaps between boot dialogs (Checking system
        -- data / do not turn off / Saving data) where it's briefly the only screen up.
        local need = (cur and cur.confirm_ticks) or CONFIRM_TICKS
        -- `confirm_ticks = 1` used to be INERT (fixed 2026-08-03). The first-sighting branch
        -- returned unconditionally, so a candidate could never commit before its SECOND tick
        -- and `= 1` behaved exactly like the default 2. Five adapters set it believing they had
        -- opted out of the debounce — fishing, saveload, skillcustom, skilltree and every
        -- screen_list instance (items, dragon balls) — and each was paying a 100 ms tax it had
        -- declined. The playbook's rule is explicit that a deliberately-opened sub-screen sets
        -- it to 1 because the global debounce exists only for screens that FLASH AT BOOT, so
        -- honour that: one tick means commit on sight.
        if cur ~= pending then
            pending, pending_n = cur, 1
            if pending_n < need then return end
        else
            pending_n = pending_n + 1
            if pending_n < need then return end
        end
        -- Context change confirmed: reset both so nothing leaks across the switch and the
        -- newly focused screen announces its current control fresh.
        local prev = active
        -- Both resets are isolated (safe_reset) so the commit below ALWAYS lands: a raise here
        -- used to leave the dispatcher wedged on the same candidate forever. See safe_reset.
        safe_reset(active)
        safe_reset(cur)
        active, pending, pending_n = cur, nil, 0
        -- TEMP diagnosis (2026-07-16 emblems latency): one line per screen change naming
        -- the winning adapter by its registration index (cross-ref app.lua order; 0 =
        -- none). Names any adapter silently shadowing a late-registered one. Turn OFF
        -- once the emblems first-visit timeline is closed.
        -- CRASH BREADCRUMB — deliberately always on (2026-07-26).
        --
        -- Every crash reported by a player so far has been un-diagnosable for the same reason:
        -- the mod is silent in the steady state (correctly — never log every frame), so the
        -- tail of their UE4SS.log says nothing about what was on screen when the process died.
        -- We were reasoning about user crashes from a developer log that shows no fault at all.
        -- A screen commit happens a few times a minute, not a few times a second, so one line
        -- per commit is well inside the no-spam rule and it turns the log tail into the single
        -- most useful fact a report can carry: the last screen the reader was on.
        --
        -- The NAME, not the index the old trace printed: adapter indices shift whenever another
        -- adapter is registered, so a number is unreadable across versions.
        local nm = (cur and (adapter_name[cur] or "?")) or "none"
        print(string.format("[KakarotAccess] screen -> %s\n", nm))
        if TRACE_COMMITS then
            local idx = 0
            if cur then
                for i, a in ipairs(adapters) do
                    if a == cur then idx = i break end
                end
            end
            print(string.format("[KakarotAccess] screen commit -> adapter #%d t=%.2f\n",
                idx, os.clock()))
        end
        -- New context: the screen's actions ("X: assign", "Y: skill tree") are read once,
        -- queued behind its own announcement (keyhelp_watch.lua).
        safe_call("KeyhelpWatch.screen_changed", KeyhelpWatch.screen_changed, cur)
        -- A screen just opened or closed — the one moment a not-yet-cached submenu may be
        -- appearing (ring → items, items → detail, close → reopen). Let missing pools skip
        -- their backoff for a short window so the new screen reads at once (ui_core).
        -- EXCEPT quiet flips (2026-07-16 night, THE cinematic stutter): the subtitles
        -- adapter commits IN on every line and OUT in every gap, and each commit
        -- re-armed a fresh boost generation — one scan per absent class every ~2s for
        -- the whole cinematic (measured: 572 scans/90s, avg step 34ms, dump 21:11).
        -- A flip between nil and a scan_quiet adapter (either direction) is dialogue
        -- cadence, not menu navigation — no boost, no hot window. Real menu flips
        -- (dialogue → pause, nil → menu) keep the boost.
        local flip_quiet = (prev == nil or prev.scan_quiet == true)
            and (cur == nil or cur.scan_quiet == true)
        if not flip_quiet then
            Core.boost_missing()
            hot_until = tick_n + HOT_TICKS   -- a follow-up screen may appear right after
        end
    else
        pending, pending_n = nil, 0
    end

    -- Cinematic quiet gate: while the committed adapter is a passive overlay that
    -- rides on top of heavy engine work (subtitles/talk window sets scan_quiet) —
    -- or the whole camera-cutscene state (see the world predicate above) — ui_core
    -- defers steady-state scans and the 20ms pad dispatcher relaxes.
    -- The battle-HUD edge is tracked UNCONDITIONALLY (2026-07-25). It used to be evaluated only
    -- in the `active == nil` branch below, so the falling edge was invisible whenever any adapter
    -- held the screen — and a battle ending with subtitles up is exactly that case, which meant
    -- this gate's own hot window for the result screens missed the one moment it exists for.
    -- Making it unconditional is also what exposed the flapping the debounce above now absorbs.
    local battle_raw = battle_hud_live()
    battle_absent = battle_raw and 0 or (battle_absent + 1)
    -- Debounced view of the signal: present the moment it appears, absent only after
    -- ABSENT_TICKS of continuous absence (see the note above — the raw signal flaps).
    local battle = battle_raw or battle_absent < ABSENT_TICKS
    if battle ~= prev_battle then
        prev_battle = battle
        -- Falling edge = a battle ended: open a scan window so the result screens, which appear
        -- EVENT-LESS right then (no press, no minimap yet), are not deferred by quiet mode.
        -- (The `_G.__KakarotBattleLeftAt` stamp that used to be published here is gone:
        -- screen_gameover was its only consumer and no longer needs an arming signal at all now
        -- that Gameover_C is directory-mapped.)
        if not battle then hot_until = tick_n + HOT_TICKS * 3 end
    end
    local quiet = (active ~= nil and active.scan_quiet == true) or false
    if not quiet and active == nil then
        quiet = Dir.root_ok("mm") and not battle and tick_n > hot_until
            and not Core.free_roam(tick_n)
    end
    Core.set_quiet(quiet)
    _G.__KakarotPadRelax = quiet

    if active then
        -- Isolated like the probe above: an adapter that raises while speaking must not take
        -- the keyhelp and guidance readers down with it on the same tick.
        Mem.mark("ui.update " .. (adapter_name[active] or "?"))
        local ok, err = pcall(active.update)
        if not ok then
            local nm = adapter_name[active] or "?"
            if not fault_logged[nm .. ":update"] then   -- lint:plain-table
                fault_logged[nm .. ":update"] = true   -- lint:plain-table
                print(string.format("[KakarotAccess] adapter '%s' faulted in update: %s\n",
                    nm, tostring(err)))
            end
        end
        safe_call("KeyhelpWatch.update", KeyhelpWatch.update)
    end
    -- Tutorial guidance line (guide_watch.lua): runs with OR without an active
    -- adapter (a guide box can be pinned over free roam), queued after whatever
    -- else this tick announced.
    safe_call("GuideWatch.update", GuideWatch.update)
end

function Registry.start()
    if enabled then return end
    enabled = true
    active, pending, pending_n = nil, nil, 0
    Core.loop(step, function() return enabled end)
end

function Registry.stop()
    enabled = false
    -- Restore the same invariant start() establishes: the reader being off means NO screen owner.
    -- `active` used to be left pointing at whatever was committed, and step() — its only writer —
    -- never runs again once Core.loop retires the loop, so Registry.active_adapter() served that
    -- stale pointer for the rest of the session. nav_tracker's ui_muted() is then permanently
    -- true: the radar audio stays muted, world_alive() and so Nav.field_ready() stay false, and
    -- THAT gate is what opens both the R3 radar picker and the L3+R3 config menu. So Ctrl+M
    -- pressed with any screen committed — during a dialogue or with the pause ring up, the most
    -- natural moment to want quiet — disabled all of it, with nothing in the log to say why.
    safe_reset(active)
    active, pending, pending_n = nil, nil, 0
    safe_call("KeyhelpWatch.screen_changed", KeyhelpWatch.screen_changed, nil)
    Core.set_quiet(false)              -- never leave the scan/pad throttles latched on
    _G.__KakarotPadRelax = false
end

function Registry.is_enabled() return enabled end

-- The currently active screen adapter (nil while free-roaming). The nav tracker
-- gates its audio cues on this: a menu/dialog owning the screen silences the radar.
function Registry.active_adapter() return active end

-- Inside the hot window (~1s after a fresh pad press or a screen commit)? Every
-- legit "a screen is being opened right now" signal rides one — entry-signal watch
-- arms require this, so a stale pointer read mid-cutscene can never arm a scan lane
-- (the 21:57 clean dump: a parked community board claiming the line gaps armed the
-- emblem watch twice per scene with no press anywhere).
function Registry.hot() return tick_n <= hot_until end

-- The active adapter's MODULE NAME (nil if none, "?" if it registered without one). `adapter_name`
-- is otherwise private and only ever reached the crash breadcrumb, so the one question asked most
-- often when a screen goes quiet — "which adapter owns the tick right now?" — could only be answered
-- by grepping the log for a `screen ->` line that may be minutes old. Diagnostics only; nothing in
-- the read path should branch on an adapter's name.
function Registry.active_name()
    if not active then return nil end
    return adapter_name[active] or "?"
end

-- Registration-order index of the active adapter (nil if none) — for diagnostics
-- (adapters are anonymous tables; the index maps back to the app.lua register order).
function Registry.active_index()
    for i, a in ipairs(adapters) do
        if a == active then return i end
    end
    return nil
end

function Registry.toggle()
    if enabled then Registry.stop() else Registry.start() end
    return enabled
end

-- Re-announce the current selection (F1): reset the active announcer so update()
-- speaks it again next tick. An adapter that suppresses repeats on its own (the
-- resident controls guide speaks each content only once) provides reannounce()
-- instead, so F1 can override its once-only gate.
function Registry.repeat_current()
    if not active then return end
    if active.reannounce then active.reannounce()
    elseif active.reset then active.reset() end
    -- And the pinned tutorial instruction, if one is showing — on the community
    -- board it is the step's task ("with an empty panel selected, press A…"), and
    -- it must be recoverable on demand, not only on its once-per-change read.
    GuideWatch.reannounce()
end

return Registry
