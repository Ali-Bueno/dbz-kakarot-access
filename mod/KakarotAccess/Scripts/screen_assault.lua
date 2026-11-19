-- Notice reader: the Android Assault alarm HUD in the Trunks DLC (fm.AssaultEvent_Gauge,
-- UAT_UIAssaultEvent_Gauge, AT.hpp:30344). Speaks the gauge header when it appears, each
-- warning line when it shows (entering the drones' area, being caught) and the drone counter
-- when it changes. A passive overlay: it speaks and releases the tick at once.
-- Evidence and open questions: reference/dbz-kakarot/notes/dbz-kakarot-android-assault.md

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")
local I18n = require("i18n")
local Dir = require("ui_directory")

local Gauge = {}

Gauge.nav_mute = false      -- the event is gameplay: the radar keeps running
Gauge.keyhelp_auto = false  -- passive reader, no automatic key help

-- Text lines in on-screen order; a caught warning interrupts, the rest queue.
local LINES = {
    { member = "GaugeHeaderText" },
    { member = "AreaEntryWarningText" },
    { member = "CaughtWarningText", urgent = true },
}
-- A line must stay hidden this long before its next appearance speaks again: pooled
-- widgets flicker through camera work, and one warning must not be read three times.
local GONE_S = 3

local seen = {}      -- member -> { text, at } of the last visible reading
local queue = nil

local function visible_text(host, member)
    local node = Core.member(host, member)
    if not (Core.valid(node) and Core.on_screen(node)) then return nil end
    local t = A.markup_to_speech(Core.read_text(node))
    return (t and t ~= "") and t or nil
end

-- Queue `text` for `key` unless it is the same line, still (or just recently) visible.
local function offer(key, text, now, urgent)
    local s = seen[key]
    if not (s and s.text == text and now - s.at <= GONE_S) then
        queue = queue or {}
        queue[#queue + 1] = { text = text, urgent = urgent }
    end
    seen[key] = { text = text, at = now }
end

function Gauge.is_active()
    queue = nil
    local host = Dir.peek("fm", "AssaultEvent_Gauge")
    if not (Core.valid(host) and Core.on_screen(host) and Core.pane_rendered(host)) then
        return false
    end
    local now = os.clock()
    for _, line in ipairs(LINES) do
        local t = visible_text(host, line.member)
        if t then offer(line.member, t, now, line.urgent) end
    end
    local counter = Core.member(host, "DroneCountText")
    local n = Core.valid(counter) and visible_text(counter, "DroneCountText") or nil
    if n then offer("count", string.format(I18n.t("assault_drone_count"), n), now) end
    return queue ~= nil
end

function Gauge.reset() end

function Gauge.update()
    if not queue then return end
    for _, q in ipairs(queue) do Speech.say(q.text, q.urgent == true) end
    queue = nil
end

return Gauge
