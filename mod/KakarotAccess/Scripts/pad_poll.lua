-- Shared 20 ms tick bus.
--
-- ONE LoopAsync + ONE ExecuteInGameThread dispatch serves EVERY periodic job in the mod: the fast
-- pad consumers that must read the gamepad faster than the registry poll (radar_menu's R3 picker,
-- config_menu's L3+R3 chord, quest_objective's L3+Y objective re-read, the map travel d-pad, the
-- status-page d-pad), and — since 2026-08-02 — the four slow subsystems that each used to own a
-- LoopAsync of their own (the menu registry at 100 ms, the radar at 100 ms, the battle monitor at
-- 250 ms, the objective narrator at 300 ms).
--
-- WHY EVERYTHING IS ON ONE LOOP (2026-08-02, from four user crashes). Every `ExecuteInGameThread`
-- call creates a BRAND-NEW lua_State — UE4SS `LuaMod.cpp:3057-3091` → `make_hook_state` (:701,
-- whose dedup is commented out) → `Lua::new_thread` → `lua_newthread` — and inserts it into a
-- process-wide `static std::unordered_map lua_instances` (`LuaMadeSimple.cpp:11`) that has NO
-- mutex and from which nothing is ever erased. The insert runs on the ASYNC thread and OUTSIDE
-- the mutex taken at :3080, while the GAME thread reads that same map from `process_lua_function`
-- (:872) on every single reflected member access. Five loops meant ~77 of those races per second,
-- forever, plus ~555k dead map entries in a two-hour session. When a read loses the race it
-- throws; the throw PIERCES pcall (Lua is built as C, so its protection is setjmp, not
-- try/catch); and on 2026-08-01 one such throw cost a whole session — see the guard note in
-- mem.lua. Merging the five loops removes four of the five generators and takes the rate down to
-- the pad's own 50/s. It also stops four extra `lua_newthread` calls per cycle from mutating the
-- main state's stack, allocator and GC from the worker thread while the game thread runs Lua on
-- the same `global_State`.
--
-- Each stepper keeps its own cheap early-out gate and its own error handling; this module owns
-- only the dispatch and the schedule. Same generation/busy guard as before: a Ctrl+Shift+R reload
-- bumps the global generation so the stale loop retires itself, and `busy` is cleared on ENTRY so
-- an uncatchable C++ abort in a stepper can never silence the bus for the session.

local Core = require("ui_core")   -- Core.drop_memos only (see the dispatch below)
local Mem = require("mem")        -- crash black box only (Mem.mark)
local Input = require("input")    -- Input.begin_tick only (the edge latch drain below)
-- The optional preferred heartbeat; this loop is its watchdog. `pcall`'d ON PURPOSE: tick_hook's
-- own header promises that DELETING that file is the rollback if hooking ever destabilises the
-- game, and a hard require would have turned that rollback into a mod which fails to load at all —
-- i.e. total silence for a blind player. An absent or broken module simply means this loop is the
-- only driver, which is the behaviour the mod had before it existed. (Caught in review of the
-- 2026-08-03 boot crash: the rollback the comment promised did not actually work.)
local ok_hook, TickHook = pcall(require, "tick_hook")
if not ok_hook then TickHook = nil end

local Poll = {}

local TICK_MS = 20
-- Published so a stepper can convert its own tick counter into wall time without restating the
-- period (radar_menu times its deferred picker open against Nav.targets_build_ms this way).
Poll.TICK_MS = TICK_MS
-- ---- the dispatch grid --------------------------------------------------------------------
--
-- SLOW BY DEFAULT (2026-08-03). The 20 ms grid exists for exactly one thing — a menu the player
-- is driving with the d-pad — and that is a small fraction of a session. Everything else on this
-- bus is a 100 ms-or-slower narrator.
--
-- WHY A SLOWER GRID COSTS NO PRESSES. The native latch accumulates rising edges from the GAME's
-- own XInput call, at render-frame rate, in an interlocked accumulator (`note_buttons`,
-- `input_bridge.c:220-224`), and `Input.begin_tick` below drains it. Its own comment says it:
-- "A press cannot be lost however late or irregular the drain is; at worst it is served a frame
-- or two later." So a 100 ms grid delays the RESPONSE, it does not drop the INPUT — provided the
-- stepper asks `Input.pressed`, which is why the three steppers that used to roll their own
-- two-tick level compare were converted on 2026-08-03.
--
-- WHAT THIS BUYS. Every dispatch is one `ExecuteInGameThread`, and every one of those is a
-- `lua_newthread` plus an unsynchronized `lua_instances` insert racing the game thread (see the
-- header). Slow-by-default takes that from ~50/s to ~10/s for most of a session, at no cost to
-- the d-pad, because the fast grid is still there the moment a menu asks for it.
local GRID_EVERY = 5   -- 5 x 20 ms = the 100 ms default grid
local grid_n = 0
-- The same 100 ms expressed in seconds, for the wall-clock gate in Poll.pump. The counter gate
-- above cannot serve on its own once the tick hook drives us: it counts THIS loop's ticks, and
-- the hook arrives on the game's frame cadence instead.
local SLOW_GRID_S = GRID_EVERY * TICK_MS / 1000
local last_pump = 0
-- PER-DISPATCH TIME CEILING (2026-08-03, from the hang post-mortem). Merging the five loops did
-- not increase the total work, but it CONCENTRATED it: `Core.begin_scan_tick` refills the scan
-- budget on wall time (`ui_core.lua:908`, 2 scans per 100 ms) and has three callers that now land
-- in ONE callback — the registry stepper, battle_monitor's step and quest_objective's step. Two
-- FindAllOf at ~65 ms already exceed the 100 ms refill window, so the second and third callers
-- legitimately refill, and what used to be three separate ~130 ms stalls became one unbroken
-- ~390 ms block. Duration is what matters here: the longer the game thread sits inside an
-- object-table walk, the wider the window for the async-loader deadlock this codebase has already
-- documented twice (`ui_core.lua:1185-1191`, "mid-session, no crash, log just stops").
--
-- So the merge pays for its own concentration: once a dispatch has spent a whole grid period, the
-- remaining periodic steppers are deferred to the next one. Their `next_at` is NOT advanced, so
-- they stay due and run first thing next dispatch — this delays a narrator, it never drops it.
-- The ceiling is the dispatch period itself rather than a picked number: a dispatch that has
-- already outlasted its own grid is by definition late. `SCANS_PER_TICK`/`REFILL_EVERY_S` are
-- deliberately NOT touched — ui_core.lua:880-887 records that both 1-scan and time-gated variants
-- silenced whole menus, and the user has just play-verified the current behaviour.
local DISPATCH_BUDGET_S = SLOW_GRID_S
-- Worker ticks of silence before the watchdog concludes the hook is not driving and takes over.
-- Same magnitude and same reasoning as every other falling-edge debounce in this codebase
-- (ui_registry's ABSENT_TICKS = 5 polls): a map load, or a screen whose GameMode does not own
-- this hook, is exactly when a frame or two goes missing, and flapping between the two drivers
-- would double-dispatch.
local HOOK_GRACE_TICKS = 5
-- name -> true, for the steppers currently asking for the 20 ms grid. The verdict is published
-- to _G as a single boolean because the LoopAsync callback reads it on the WORKER thread, where
-- a plain global read is the access class already used by the generation guard and the relax
-- gate — walking a table from there is not.
local fast_demand = {}
-- Half a bus tick of slack on the "is it due?" test. Without it a 100 ms stepper ALIASES to
-- 200 ms whenever a dispatch lands a hair before its deadline — and under the relax gate the
-- dispatch grid IS exactly 100 ms, so for the menu reader that would be the common case, not a
-- rare one. `next_at` is re-anchored to the actual run time below, so the slack delays nothing
-- and cannot accumulate into an early-firing drift.
local DUE_SLACK_S = (TICK_MS / 2) / 1000
-- name -> { fn, on_error, period_s, next_at, should_run }. `period_s == 0` means every dispatch.
local steppers = {}
-- Dispatch order. APPEND-ONLY on purpose: `pairs()` is nondeterministic and the merged
-- subsystems are not independent (the quest and battle steps both read verdicts the registry
-- step computed), so the order in which they run must be reproducible between sessions.
-- `unregister` only nils the map entry and leaves the name here, which also means a stepper may
-- unregister itself — or another — from inside its own step without disturbing this traversal,
-- and a re-registered name keeps its original slot. The name set is fixed and tiny (<= 9).
local order = {}
local in_order = {}
local running = false

local function add(name, entry)
    if not in_order[name] then
        in_order[name] = true
        order[#order + 1] = name
    end
    steppers[name] = entry
    Poll.start()
end

-- Register a per-dispatch stepper (the 20 ms pad consumers). `on_error` (optional) runs before
-- the error is logged — for cleanup that must not be skipped (radar_menu unblocking the pad).
function Poll.register(name, step_fn, on_error)
    add(name, { fn = step_fn, on_error = on_error, period_s = 0 })
end

-- Register a slower periodic stepper. `period_ms` is a floor, not a guarantee: a dispatch dropped
-- by the busy guard or the relax gate simply serves it on the next one. `should_run` (optional)
-- is the old `Core.loop` predicate — when it returns false the stepper retires itself, which is
-- how Registry.stop() ends the reader without this module knowing anything about it.
function Poll.register_every(name, period_ms, step_fn, on_error, should_run)
    add(name, { fn = step_fn, on_error = on_error, should_run = should_run,
                period_s = period_ms / 1000, next_at = 0 })
end

function Poll.unregister(name)
    steppers[name] = nil
    Poll.demand_fast(name, false)   -- a retired stepper must never pin the fast grid
end

-- A stepper declares, once per dispatch, whether it currently needs the 20 ms grid. Only the
-- menus the player drives with the d-pad ever ask. Call it from the TOP of the step, above every
-- early return, so a bail-out can never leave the grid pinned fast; the cost of doing so is that
-- the demand reflects the previous dispatch's state, i.e. the fast grid arrives up to one slow
-- tick after a menu opens — which is imperceptible, and is the safe direction to be wrong in.
-- Cheap and idempotent: the global is only rewritten when the set actually changes.
function Poll.demand_fast(name, on)
    if on then
        if fast_demand[name] then return end
        fast_demand[name] = true
    else
        if not fast_demand[name] then return end
        fast_demand[name] = nil
    end
    _G.__KakarotPadFast = next(fast_demand) ~= nil
end

local function run_stepper(name, s)
    local ok, err = pcall(s.fn)
    if not ok then
        if s.on_error then pcall(s.on_error) end
        print("[KakarotAccess] tick step error (" .. name .. "): " .. tostring(err) .. "\n")
    end
end

local function dispatch()
    -- ONE breadcrumb for the whole dispatch, and the memo drop.
    --
    -- The mark: this was the mod's biggest instrumentation blind spot — a 50 Hz game-thread loop
    -- whose steppers reach real dereferences (radar_menu's R3 picker and config_menu both call
    -- Nav.field_ready() -> world_alive() on every edge and every tick while open, and do_open runs
    -- the whole target sweep). ONE mark per dispatch, not one per stepper: the ring is 256 slots
    -- and several marks per 20 ms would consume it. The periodic steppers still write their own
    -- marks from inside their steps (nav.step, battle.step, quest.step, ui.tick), so the trail
    -- still attributes a death to the right subsystem.
    --
    -- The memo drop: `Core.valid`/`Core.on_screen` verdicts are memoized in tables that only
    -- `Core.poll_world` and `Core.begin_scan_tick` clear — and this dispatch must not call
    -- begin_scan_tick itself, because that would refill the scan budget 5x faster than the
    -- wall-clock ceiling allows (the 2026-07-26 "a scan slot is not a rate limit" bug). The
    -- registry stepper still calls it, but only on its own 100 ms schedule, which is exactly the
    -- ceiling it is supposed to have. Without the drop here, a pad stepper's `Core.valid` could be
    -- answered from a verdict computed up to ~100 ms earlier, skipping `Mem.alive` — the only
    -- guard that runs outside the scripting VM — on exactly the handles streaming is most likely
    -- to have freed. Dropping the memos costs two empty tables and is strictly fail-safe.
    local t0 = os.clock()   -- start of the per-dispatch time ceiling (see DISPATCH_BUDGET_S)
    Mem.mark("pad.tick")
    Core.drop_memos()
    -- Drain the pad's rising-edge latch ONCE for the whole dispatch and republish it
    -- (Input.edges()). The native latch is destructive, so a stepper draining it itself would
    -- swallow every other stepper's presses; and doing it here means a press survives the relax
    -- gate above, a dropped busy tick, and any stepper that blocks the game thread speaking. See
    -- the note in input.lua.
    Input.begin_tick()

    -- PASS 1 — the pad consumers, ALWAYS first. They are what the player feels: a narrator step
    -- sharing this dispatch must never sit between a button press and the response to it.
    for i = 1, #order do
        local name = order[i]
        local s = steppers[name]
        if s and s.period_s == 0 then run_stepper(name, s) end
    end

    -- PASS 2 — the periodic subsystems, due by WALL CLOCK rather than by a dispatch count, so a
    -- tick dropped by `busy` or by the relax gate delays a step instead of shifting its cadence.
    -- `os.clock()` is wall time here (MSVC's clock() counts since process start), the same source
    -- the retry throttles in mem.lua and every step-cost telemetry block already use.
    local now = os.clock()
    local due_by = now + DUE_SLACK_S
    local ran_one = false
    for i = 1, #order do
        local name = order[i]
        local s = steppers[name]
        if s and s.period_s > 0 and due_by >= s.next_at then
            -- `ran_one` guarantees PROGRESS: the ceiling may never stop a dispatch from serving at
            -- least one periodic stepper, or a single expensive one could starve the queue forever.
            if ran_one and (os.clock() - t0) >= DISPATCH_BUDGET_S then break end
            if s.should_run and not s.should_run() then
                Poll.unregister(name)
            else
                s.next_at = now + s.period_s
                run_stepper(name, s)
                ran_one = true
            end
        end
    end
end

-- THE GAME-THREAD ENTRY POINT, shared by BOTH drivers: the tick hook (preferred) and the
-- LoopAsync watchdog below. Everything that decides CADENCE lives here rather than in either
-- driver, because the two arrive at different rates — the hook on the game's frame cadence
-- (~60 Hz), the loop at TICK_MS — and only a wall-clock gate makes the result the same either way.
function Poll.pump()
    -- Keep trying to install the hook until it takes; once installed this is one boolean read.
    -- It lives here rather than in Poll.start because the Blueprint class it hooks does not exist
    -- yet at boot — the title screen runs a different GameMode entirely.
    if TickHook then TickHook.arm(Poll.pump) end
    -- Nothing registered (the window between App.stop and App.start during a reload): skip the
    -- prologue, and in particular do NOT drain the pad's edge latch, which would swallow presses
    -- that belong to the steppers about to be registered.
    if next(steppers) == nil then return end
    local now = os.clock()
    if not (_G.__KakarotPadFast and not _G.__KakarotPadRelax) then
        -- SLOW GRID: the exact gate. The worker's counter gate below is only a cheap pre-filter
        -- that stops it QUEUEING work it does not need to; this is what actually sets the rate,
        -- and it is the only one that applies when the hook is the driver.
        if now + DUE_SLACK_S < last_pump + SLOW_GRID_S then return end
    end
    -- FAST GRID: every call. From the hook that is ~16.7 ms, which is better than the 20 ms the
    -- LoopAsync can offer and comfortably inside what the d-pad wants, so there is nothing to
    -- throttle — the fast grid is a FLOOR on responsiveness, not a budget.
    last_pump = now
    dispatch()
end

function Poll.start()
    if running then return end
    running = true
    _G.__KakarotPadPollGen = (_G.__KakarotPadPollGen or 0) + 1
    local myGen = _G.__KakarotPadPollGen
    local busy = false
    local last_beat, hook_idle = -1, 0
    LoopAsync(TICK_MS, function()
        if _G.__KakarotPadPollGen ~= myGen then return true end
        -- Empty stepper table → idle tick, but NEVER retire the loop: retiring on empty
        -- opened a cross-thread register/retire race (worker reads `steppers` while the
        -- game thread refills it), and the steppers are session-permanent anyway. Only
        -- the generation bump above (reload) ends this loop.
        if next(steppers) == nil then return false end
        -- THE HOOK IS THE PREFERRED DRIVER, and while it is beating this loop must queue NOTHING.
        -- That is the whole point of tick_hook.lua: no ExecuteInGameThread means no lua_newthread
        -- and no unsynchronized lua_instances insert, so the race that has been ending sessions
        -- stops having a source. Comparing a COUNTER keeps this to an integer read and an integer
        -- compare — no C call on the shared lua_State, which is the only access class this worker
        -- thread is allowed. `last_beat` starts at -1 so a hook that never installs (its counter
        -- sits at 0) still reads as "changed once", then goes quiet and hands over normally.
        local beat = _G.__KakarotTickBeat or 0
        if beat ~= last_beat then
            last_beat, hook_idle = beat, 0
            return false
        end
        if hook_idle < HOOK_GRACE_TICKS then
            hook_idle = hook_idle + 1
            return false
        end
        -- The grid gate (plain global reads, the same worker-side access class as the
        -- generation guard above). Slow by default; a stepper lifts it to every tick with
        -- Poll.demand_fast. `relax` OUTRANKS the demand: the reader publishes
        -- __KakarotPadRelax during cutscene subtitles and map loads, which is exactly when the
        -- game thread is busiest and queued callbacks hurt most — so a menu left open across a
        -- load rides the 100 ms grid for that stretch, like every other consumer there. It also
        -- means the old relax gate is subsumed rather than deleted: its grid and the new default
        -- grid are the same 100 ms, so the only thing it still decides is whether fast can win.
        if not (_G.__KakarotPadFast and not _G.__KakarotPadRelax) then
            grid_n = grid_n + 1
            if grid_n % GRID_EVERY ~= 0 then return false end
        end
        if not busy then
            busy = true
            ExecuteInGameThread(function()
                -- Cleared on ENTRY, not exit: some engine errors on this game are C++ exceptions
                -- pcall cannot catch — they kill this callback mid-flight, and a still-true
                -- `busy` would silence every subsystem for the whole session. Clearing here keeps
                -- the anti-pile-up purpose (the game thread runs this atomically, so at most one
                -- extra dispatch queues while we run).
                busy = false
                -- Through `pump`, not straight to `dispatch`: the cadence gate and the hook's
                -- install retry both live there, so the fallback path behaves identically to the
                -- hook path and keeps trying to hand the job over.
                Poll.pump()
            end)
        end
        return false
    end)
end

return Poll
