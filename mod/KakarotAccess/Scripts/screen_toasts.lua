-- Notice reader: gameplay TOASTS — the item/quest log (Info_Log_C) and the
-- level-up / important-notice banner (Info_Log02_C).
--
-- Structures (blueprint headers + F7 dumps):
--   * Info_Log_C : UAT_UIInfoLog — bars Info_Log_Bar00..04, each with Txt00
--     ("Herb x 1", "Rainbow orb x 2", quest-update lines). Multi-instance pool.
--   * Info_Log02_C : UAT_UIInfoLog02 — the highlighted log ("¡Gohan alcanzó el
--     nvl. 7!", skill-tree unlocks — it carries a LevelUpColor + skill-tree icon):
--     bars Info_Log_Bar_00..04 (Info_Log02.hpp — note the extra underscore), text
--     on the bar's native TextBox (AT_UIInfoLog02Bar 0x3C0) — its ONLY text member.
--     (The old Info_Log_Level_C loop is gone: that class exists NOWHERE in the
--     ObjectDump, so level-ups were never announced — user bug 2026-07-17; the
--     real banner was pinned by the F7 census dump_1784302864_002.)
--
-- NOTICE-RELEASE with a PRESENCE dedup: each toast line is spoken ONCE (queued, never
-- interrupting) and the dispatcher tick is released immediately; a line re-announces only
-- after it LEAVES the screen and comes back (toasts legitimately repeat — two herb
-- pickups reopen a fresh bar). Dedup is by CONTENT, re-armed on disappearance. The old
-- time-window dedup (re-speak after N seconds) re-read a stale banner that lingered
-- on_screen — e.g. a skill/super-attack unlock banner still parked after a fight was
-- re-announced in the middle of free-roam every few seconds (user 2026-07-24). Rows
-- recycle/scroll, so the gate is per content, not per bar. nav_mute=false: toasts happen
-- mid-roam; the radar keeps running. Registered near the BOTTOM: any real menu outranks
-- a toast.

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")

local Toasts = {}

Toasts.nav_mute = false

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
Toasts.keyhelp_auto = false

local BAR_COUNT = 5    -- Info_Log_Bar00..04 / Info_Log_Bar_00..04 (Info_Log.hpp, Info_Log02.hpp)
local GRACE_TICKS = 15 -- a spoken toast must be OFF screen this many polls (~1.5 s) before it
                       -- may re-announce. Info_Log02 is a FLASHING "highlighted" banner
                       -- (LevelUpColor) whose emphasis animation can drop on_screen for a
                       -- poll; clearing the dedup on the FIRST absent tick let one banner
                       -- re-read every few seconds in free-roam (user 2026-07-24). The grace
                       -- bridges the blink; a genuine repeat toast (banner fully closes, then
                       -- reopens > 1.5 s later) still re-announces.

local tick = 0
local spoken = {}      -- content -> tick last seen on screen while spoken; the line re-arms
                       -- only after GRACE_TICKS absent (blink-tolerant). Set in update(), so
                       -- a toast deferred by an open menu is not lost (like screen_results)
local queue = nil      -- lines to speak this tick (computed in is_active)

local function clean(t) return t and A.markup_to_speech(t) or nil end

local function node_text(node)
    local t
    pcall(function() t = clean(Core.read_text(node)) end)
    return t
end

-- All currently visible toast lines, in on-screen order.
local function lines()
    local out = {}
    for _, host in ipairs(Core.cached_all("Info_Log_C", tick)) do
        if Core.valid(host) and Core.on_screen(host) then
            for i = 0, BAR_COUNT - 1 do
                local bar
                pcall(function() bar = host["Info_Log_Bar" .. string.format("%02d", i)] end)
                if Core.valid(bar) and Core.on_screen(bar) then
                    -- Fetch INSIDE a pcall, never as a call argument: on a stale
                    -- pooled bar a naked member fetch is the uncatchable AV class
                    -- (the 2026-07-17 fishing crash — fixed then only in the
                    -- Info_Log02_C loop below; this twin was left behind).
                    local box
                    pcall(function() box = bar.Txt00 end)
                    local t = node_text(box)
                    if t then out[#out + 1] = t end
                end
            end
        end
    end
    for _, host in ipairs(Core.cached_all("Info_Log02_C", tick)) do
        if Core.valid(host) and Core.on_screen(host) then
            for i = 0, BAR_COUNT - 1 do
                local bar
                pcall(function() bar = host["Info_Log_Bar_" .. string.format("%02d", i)] end)
                if Core.valid(bar) and Core.on_screen(bar) then
                    -- Fetch INSIDE pcall, and TextBox ONLY: Info_Log_Bar02_C
                    -- reflects TextBox (AT_UIInfoLog02Bar 0x3C0) and has NO Txt00 —
                    -- a naked `bar.Txt00` argument here was the 2026-07-17 fishing
                    -- crash (nonexistent-member fetch outside pcall = uncatchable,
                    -- retried per tick on every blank pooled bar).
                    local box
                    pcall(function() box = bar.TextBox end)
                    local t = node_text(box)
                    if t then out[#out + 1] = t end
                end
            end
        end
    end
    return out
end

function Toasts.is_active()
    tick = tick + 1
    queue = nil
    local ordered = lines()
    local cur = {}
    for _, line in ipairs(ordered) do cur[line] = true end
    -- Refresh the last-seen tick for spoken lines still on screen; re-arm (forget) only the
    -- ones absent for a sustained grace, so a one-poll blink of the flashing banner doesn't
    -- re-trigger it. A line still on screen is NOT re-queued (kills the stale re-read).
    for line, seen in pairs(spoken) do
        if cur[line] then spoken[line] = tick
        elseif tick - seen > GRACE_TICKS then spoken[line] = nil end
    end
    local qseen = {}   -- avoid double-queuing the same text shown on two bars this tick
    for _, line in ipairs(ordered) do
        if not spoken[line] and not qseen[line] then
            qseen[line] = true
            queue = queue or {}
            queue[#queue + 1] = line
        end
    end
    -- Active only while there is something NEW to say; released right after (notice
    -- pattern — a toast must never hold the screen).
    return queue ~= nil
end

function Toasts.reset() end

function Toasts.update()
    if not queue then return end
    for _, line in ipairs(queue) do
        spoken[line] = tick       -- marked here (not in is_active) so a toast deferred by
        Speech.say(line, false)   -- an open menu still speaks once the menu closes
    end
    queue = nil
end

return Toasts
