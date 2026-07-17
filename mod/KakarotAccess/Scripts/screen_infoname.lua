-- Notice reader: cinematic character-introduction cards ("Gohan — hijo de Goku",
-- "Piccolo — rival de Goku"), driven by the sequencer's
-- ATMovieSceneCharaIntroductionSection.
--
-- DETECTION IS A POINTER READ — no scans (settled by the round-6 trace,
-- 2026-07-17): the display path is the fm-owned CONTAINER `fm.InfoName`
-- (AT_UIInfoName @0x668), which flips Collapsed(1) <-> rendered(3) per card and
-- whose `InfoCoreCtn` (@0x3A0) points at the ACTIVE AT_UIInfoNameCore. The core
-- carries the texts: NameTxt/PopularNameTxt + _Large twins (all reflected).
--
-- The pooled `Info_Name_C` instances are a DEAD END — the trace showed 4 parked
-- hosts that NEVER hold text and NEVER render while cards display (the active
-- core is not even in that pool). Do not scan that class; it was removed from
-- QUIET_EXEMPT along with the scan path.
--
-- Read rules:
--   * Card text must be STABLE for 2 consecutive ticks (name and popular title
--     animate in separately — never speak a bare "Goku" partial).
--   * APPEARANCE-EDGE dedup with a WALL-CLOCK absence window (GONE_S): camera
--     cuts hide the card briefly and the idle-throttled sweep runs ~300 ms per
--     tick in cinematics, so tick-based windows re-armed repeats (rounds 3-4).
--     A text speaks once per genuine appearance; `spoken` is set in update() so
--     a pending line survives losing ticks to a subtitle and speaks exactly once.
--   * Liveness: container rendered (on_screen — Collapsed when hidden) and not
--     opacity-faded. Speech is queued: never cuts a subtitle line.

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")
local Dir = require("ui_directory")

local Card = {}

-- Cards overlay cinematics and never pause the game.
Card.nav_mute = false

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua).
Card.keyhelp_auto = false

local STABLE_TICKS = 2   -- text must hold this many consecutive ticks before speaking
local GONE_S = 30        -- seconds of absence before a card counts as genuinely gone

local tick = 0
local seen = {}      -- text -> { first=tick, last=os.clock, spoken=bool } per appearance
local queue = nil    -- texts to speak this tick (computed in is_active)

-- Fade-out ghost: opacity drops to 0 while visibility flags lag.
local function faded(h)
    local ok, op = pcall(function() return h:GetRenderOpacity() end)
    return ok and type(op) == "number" and op < 0.05
end

-- TEMP TRACE (round 6 verification): one log line per state change. Turn OFF
-- once the user confirms the cards read correctly.
local TRACE = true
local last_trace = nil

local function trace(line)
    if TRACE and line ~= last_trace then
        last_trace = line
        print("[KakarotAccess] cards " .. line .. "\n")
    end
end

local function node_text(host, member)
    local t
    pcall(function()
        local node = host[member]
        if Core.valid(node) and Core.is_visible(node) then
            t = A.markup_to_speech(Core.read_text(node))
        end
    end)
    return (t and t ~= "") and t or nil
end

-- "Name, popular title" from whichever size variant is visible.
local function card_text(core)
    local name = node_text(core, "NameTxt") or node_text(core, "NameTxt_Large")
    local pop  = node_text(core, "PopularNameTxt") or node_text(core, "PopularNameTxt_Large")
    if not name and not pop then return nil end
    return Core.phrase(name, pop)
end

function Card.is_active()
    tick = tick + 1
    queue = nil
    local ctn = Dir.peek("fm", "InfoName")
    if not Core.valid(ctn) or not Core.on_screen(ctn) or faded(ctn) then
        trace("hidden")
        return false
    end
    local core
    pcall(function() core = ctn.InfoCoreCtn end)
    if not Core.valid(core) then
        trace("shown core=nil")
        return false
    end
    local t = card_text(core)
    if not t then
        trace("shown no-text")
        return false
    end
    local s = seen[t]
    local fresh = not s or (os.clock() - s.last) > GONE_S
    if fresh then
        s = { first = tick, spoken = false }   -- a fresh appearance
        seen[t] = s
    end
    s.last = os.clock()
    local stable = (tick - s.first + 1) >= STABLE_TICKS
    trace(string.format("shown txt=%q fresh=%s stable=%s spoken=%s",
        t, tostring(fresh), tostring(stable), tostring(s.spoken)))
    if not s.spoken and stable then
        queue = { t }
    end
    -- Active only while there is something new to say; released right after (notice
    -- pattern — the card must never hold the screen).
    return queue ~= nil
end

function Card.reset() end

function Card.update()
    if not queue then return end
    for _, t in ipairs(queue) do
        if seen[t] then seen[t].spoken = true end
        if TRACE then print("[KakarotAccess] cards SPOKE " .. t .. "\n") end
        -- Queued + no_requeue: never cuts a subtitle line, and a subtitle cutting
        -- US must NOT re-append the card (the 2-3x repeats the user heard — each
        -- neighboring subtitle interrupt re-queued the unfinished card line).
        Speech.say(t, false, true)
    end
    queue = nil
end

return Card
