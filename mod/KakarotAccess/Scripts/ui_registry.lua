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

function Registry.register(adapter)
    adapters[#adapters + 1] = adapter
end

local function step()
    local cur = nil
    for _, a in ipairs(adapters) do
        if a.is_active() then cur = a break end
    end

    if cur ~= active then
        -- Context change: reset both so nothing leaks across the switch and the
        -- newly focused screen announces its current control fresh.
        if active and active.reset then active.reset() end
        if cur and cur.reset then cur.reset() end
        active = cur
    end

    if cur then cur.update() end
end

function Registry.start()
    if enabled then return end
    enabled = true
    active = nil
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
