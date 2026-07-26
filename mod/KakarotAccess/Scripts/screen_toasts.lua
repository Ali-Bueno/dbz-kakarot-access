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
                -- CONFIRMED CRASH SITE, 2026-07-26. The black box caught it: the last thing the
                -- mod did before the process died — right after a map change — was
                -- `screen_toasts.is_active`. Two defects, both here:
                --
                -- 1. `bar.Txt00` was a NAKED fetch of a member this file's own comment (in the
                --    Info_Log02 loop below) records as ABSENT on `Info_Log_Bar02_C`. That comment
                --    even names it as the 2026-07-17 fishing crash. The 2026-07-24 fix moved the
                --    fetch inside a `pcall` and called it done — but a pcall CANNOT catch this:
                --    an undeclared-member fetch aborts below the Lua boundary. The bar classes in
                --    this pool are recycled, so which subclass lands in `Info_Log_Bar00..04` is
                --    not fixed, and a map transition is exactly when the pool gets rebuilt.
                -- 2. The bar name itself is composed, so the pool host is being asked for members
                --    it may not declare either.
                --
                -- Both now go through the property gate, which answers nil instead of aborting.
                local bar = Core.member(host, "Info_Log_Bar" .. string.format("%02d", i))
                if Core.valid(bar) and Core.on_screen(bar) then
                    local box = Core.member(bar, "Txt00")
                    local t = node_text(box)
                    if t then out[#out + 1] = t end
                end
            end
        end
    end
    for _, host in ipairs(Core.cached_all("Info_Log02_C", tick)) do
        if Core.valid(host) and Core.on_screen(host) then
            for i = 0, BAR_COUNT - 1 do
                -- Same gate as the twin loop above. TextBox is the right member here
                -- (`AT_UIInfoLog02Bar` 0x3C0, its only text member — `Info_Log_Bar02_C` has no
                -- Txt00), but "the right member for the class we expect" is not a guarantee
                -- about the class actually in the pool, which is what the gate checks.
                local bar = Core.member(host, "Info_Log_Bar_" .. string.format("%02d", i))
                if Core.valid(bar) and Core.on_screen(bar) then
                    local box = Core.member(bar, "TextBox")
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
