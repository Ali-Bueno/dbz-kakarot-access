-- Registry + dispatcher for the hybrid UI architecture.
--
-- Holds the registered screen adapters and runs the single poll engine: each tick
-- it finds the first active (visible) screen, and on a screen change resets the
-- announcers (so the new screen re-announces from scratch), then drives the active
-- adapter's update(). Adding a screen = register it; no engine changes.

local Core = require("ui_core")

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

function Registry.register(adapter)
    adapters[#adapters + 1] = adapter
end

local function step()
    local cur = nil
    for _, a in ipairs(adapters) do
        if a.is_active() then cur = a break end
    end

    if cur ~= active then
        -- Wait for the switch to be stable before committing; during the wait we announce
        -- nothing (neither the old nor the candidate screen). An adapter may ask for a
        -- longer confirm via `confirm_ticks` — the title menu does, so it doesn't blurt
        -- "Main menu, Continue" in the short gaps between boot dialogs (Checking system
        -- data / do not turn off / Saving data) where it's briefly the only screen up.
        local need = (cur and cur.confirm_ticks) or CONFIRM_TICKS
        if cur ~= pending then pending, pending_n = cur, 1 return end
        pending_n = pending_n + 1
        if pending_n < need then return end
        -- Context change confirmed: reset both so nothing leaks across the switch and the
        -- newly focused screen announces its current control fresh.
        if active and active.reset then active.reset() end
        if cur and cur.reset then cur.reset() end
        active, pending, pending_n = cur, nil, 0
    else
        pending, pending_n = nil, 0
    end

    if active then active.update() end
end

function Registry.start()
    if enabled then return end
    enabled = true
    active, pending, pending_n = nil, nil, 0
    Core.loop(step, function() return enabled end)
end

function Registry.stop()
    enabled = false
end

function Registry.is_enabled() return enabled end

function Registry.toggle()
    if enabled then Registry.stop() else Registry.start() end
    return enabled
end

-- Re-announce the current selection (F1): reset the active announcer so update()
-- speaks it again next tick.
function Registry.repeat_current()
    if active and active.reset then active.reset() end
end

return Registry
