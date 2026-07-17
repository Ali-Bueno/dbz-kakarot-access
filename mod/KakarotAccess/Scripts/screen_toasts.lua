-- Notice reader: gameplay TOASTS — the item/quest log (Info_Log_C) and the
-- level-up / important-notice banner (Info_Log02_C).
--
-- Structures (blueprint headers + F7 dumps):
--   * Info_Log_C : UAT_UIInfoLog — bars Info_Log_Bar00..04, each with Txt00
--     ("Herb x 1", "Rainbow orb x 2", quest-update lines). Multi-instance pool.
--   * Info_Log02_C : UAT_UIInfoLog02 — the highlighted log ("¡Gohan alcanzó el
--     nvl. 7!", skill-tree unlocks — it carries a LevelUpColor + skill-tree icon):
--     bars Info_Log_Bar_00..04 (Info_Log02.hpp — note the extra underscore), text
--     on the bar's native TextBox (AT_UIInfoLog02Bar 0x3C0), BP twin Txt00.
--     (The old Info_Log_Level_C loop is gone: that class exists NOWHERE in the
--     ObjectDump, so level-ups were never announced — user bug 2026-07-17; the
--     real banner was pinned by the F7 census dump_1784302864_002.)
--
-- NOTICE-RELEASE with a recency dedup: each toast line is spoken ONCE (queued, never
-- interrupting) and the dispatcher tick is released immediately; the same line can
-- re-announce after RECENT_S (toasts legitimately repeat — two herb pickups). Rows
-- recycle/scroll, so dedup is by CONTENT + time, not by bar. nav_mute=false: toasts
-- happen mid-roam; the radar keeps running. Registered near the BOTTOM: any real menu
-- outranks a toast.

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")

local Toasts = {}

Toasts.nav_mute = false

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
Toasts.keyhelp_auto = false

local RECENT_S = 8     -- seconds before an identical toast line may re-announce
local BAR_COUNT = 5    -- Info_Log_Bar00..04 / Info_Log_Bar_00..04 (Info_Log.hpp, Info_Log02.hpp)

local tick = 0
local recent = {}      -- content -> os.time() last spoken
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
                    local t = node_text(bar.Txt00)
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
                    local t = node_text(bar.TextBox) or node_text(bar.Txt00)
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
    local now = os.time()
    for _, line in ipairs(lines()) do
        if not recent[line] or now - recent[line] >= RECENT_S then
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
    local now = os.time()
    for _, line in ipairs(queue) do
        recent[line] = now
        Speech.say(line, false)   -- queued: toasts never cut anything off
    end
    queue = nil
end

return Toasts
