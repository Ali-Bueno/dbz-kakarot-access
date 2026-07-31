-- Shared 20ms pad scheduler.
--
-- ONE LoopAsync + ONE ExecuteInGameThread dispatch serves every feature that must read
-- the gamepad faster than the 100ms registry poll (radar_menu's R3 picker, the map
-- travel d-pad, the status-page d-pad). They used to run three separate 20ms loops —
-- 150 game-thread callbacks queued per second even with all three idle — and that queue
-- pressure hurts exactly when the game thread is busiest (cinematics, loads). Each
-- stepper keeps its own cheap early-out gate; this module only owns the dispatch.
--
-- Same generation/busy guard as ui_core.loop: a Ctrl+Shift+R reload bumps the global
-- generation so the stale loop retires itself, and `busy` is cleared on ENTRY so an
-- uncatchable C++ abort in a stepper can never silence the pad for the session.

local Core = require("ui_core")   -- Core.drop_memos only (see the dispatch below)
local Mem = require("mem")        -- crash black box only (Mem.mark)
local Input = require("input")    -- Input.begin_tick only (the edge latch drain below)

local Poll = {}

local TICK_MS = 20
local RELAX_EVERY = 5  -- during cutscene subtitles / map loads (ui_registry publishes
                       -- _G.__KakarotPadRelax) dispatch only every 5th tick (100ms):
                       -- every 20ms consumer is inert there (R3 picker closed, no menu
                       -- d-pad), and 50 queued game-thread callbacks/s are pressure
                       -- exactly when the game thread is busiest.
local relax_n = 0
local steppers = {}   -- name -> { fn = step_fn, on_error = cleanup_fn | nil }
local running = false

-- Register a per-tick stepper. `on_error` (optional) runs before the error is logged —
-- for cleanup that must not be skipped (radar_menu unblocking the pad).
function Poll.register(name, step_fn, on_error)
    steppers[name] = { fn = step_fn, on_error = on_error }
    Poll.start()
end

-- Clearing an existing key during pairs() traversal is legal Lua, so a stepper may
-- unregister itself (or another) from inside its own step.
function Poll.unregister(name)
    steppers[name] = nil
end

function Poll.start()
    if running then return end
    running = true
    _G.__KakarotPadPollGen = (_G.__KakarotPadPollGen or 0) + 1
    local myGen = _G.__KakarotPadPollGen
    local busy = false
    LoopAsync(TICK_MS, function()
        if _G.__KakarotPadPollGen ~= myGen then return true end
        -- Empty stepper table → idle tick, but NEVER retire the loop: retiring on empty
        -- opened a cross-thread register/retire race (worker reads `steppers` while the
        -- game thread refills it), and the steppers are session-permanent anyway. Only
        -- the generation bump above (reload) ends this loop.
        if next(steppers) == nil then return false end
        -- Relax gate (a plain global read, same worker-side access class as the
        -- generation guard above): skip most dispatches while the reader says the
        -- pad has nothing time-critical to serve.
        if _G.__KakarotPadRelax then
            relax_n = relax_n + 1
            if relax_n % RELAX_EVERY ~= 0 then return false end
        end
        if not busy then
            busy = true
            ExecuteInGameThread(function()
                busy = false
                -- ONE breadcrumb for the whole dispatch, and the memo drop this loop never did.
                --
                -- The mark: this was the mod's biggest instrumentation blind spot — a 50 Hz
                -- game-thread loop, five times more frequent than any marked loop, whose steppers
                -- reach real dereferences (radar_menu's R3 picker and config_menu both call
                -- Nav.field_ready() -> world_alive() on every edge and every tick while open, and
                -- do_open runs the whole target sweep). A death in here was attributed to whichever
                -- slow loop had written the last mark. ONE mark per dispatch, not one per stepper:
                -- the ring is 64 slots (~172 ms), so four marks per 20 ms would consume it.
                --
                -- The memo drop: `Core.valid`/`Core.on_screen` verdicts are memoized in tables that
                -- ONLY `Core.poll_world` and `Core.begin_scan_tick` clear — neither of which this
                -- loop calls, deliberately (calling begin_scan_tick here would refill the scan
                -- budget 5x faster than the wall-clock ceiling allows, which is the 2026-07-26
                -- "a scan slot is not a rate limit" bug). So without this, a stepper's `Core.valid`
                -- could be answered from a verdict computed up to ~100 ms earlier in another loop,
                -- skipping `Mem.alive` — the only guard that runs outside the scripting VM — on
                -- exactly the handles streaming is most likely to have freed. Dropping the memos
                -- costs two empty tables and is strictly fail-safe: the worst case is that a
                -- stepper re-computes a verdict it could have reused.
                Mem.mark("pad.tick")
                Core.drop_memos()
                -- Drain the pad's rising-edge latch ONCE for the whole dispatch and republish it
                -- (Input.edges()). The native latch is destructive, so a stepper draining it
                -- itself would swallow every other stepper's presses; and doing it here means a
                -- press survives the relax gate above, a dropped busy tick, and any stepper that
                -- blocks the game thread speaking. See the note in input.lua.
                Input.begin_tick()
                for name, s in pairs(steppers) do
                    local ok, err = pcall(s.fn)
                    if not ok then
                        if s.on_error then pcall(s.on_error) end
                        print("[KakarotAccess] pad step error (" .. name .. "): " .. tostring(err) .. "\n")
                    end
                end
            end)
        end
        return false
    end)
end

return Poll
