-- Notice reader: episode title cards (AT_UIQuestMainStart, fm.QuestMainStart).
--
-- The card shown at an episode/quest start ("Goku vs. Nappa and Vegeta"): its text is
-- TitleText (UCFUIXcmnMultiLineText, AT.hpp 0x3E0). Resolved by the screen directory
-- ({"fm","QuestMainStart"}), so detection is a pointer read. The text is spoken ONCE
-- per appearance (dedup while the card stays on screen, forgotten when it leaves —
-- the telop pattern), queued so it never cuts a subtitle line. The chapter LOGO card
-- (fm.QuestMainLogo) is image-only (ChapterTitleImage) and stays unread.

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")
local Dir = require("ui_directory")

local Card = {}

-- Cards overlay gameplay/cinematics and never pause the game.
Card.nav_mute = false

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
Card.keyhelp_auto = false

local tick = 0
local seen = nil      -- card text already spoken while its card stays on screen
local pending = nil   -- text to speak this tick (computed in is_active)

-- TEMP TRACE (2026-07-17, user: a story-chapter card in a cinematic — "Detén la
-- invasión saiyan" — went unread; this reader has never been in-game verified and
-- its on_screen gate may be blind to the display path, like the intro cards were).
-- One log line per state change of BOTH fm cards. Turn OFF once diagnosed.
-- OFF since 2026-07-21: it shipped ON in the release and trace_state() ran on
-- EVERY tick in EVERY state (two per-level fm pointer chains + widget probes),
-- including during world teardown — crash surface for zero player value.
local TRACE = false
local last_trace = nil

local function trace(line)
    if TRACE and line ~= last_trace then
        last_trace = line
        print("[KakarotAccess] qcard " .. line .. "\n")
    end
end

local function vis_of(h)
    local v = "?"
    pcall(function() v = tostring(h:GetVisibility()) end)
    return v
end

local function op_of(h)
    local o = "?"
    pcall(function() o = string.format("%.2f", h:GetRenderOpacity()) end)
    return o
end

local function trace_state()
    if not TRACE then return end
    local st = {}
    local qs = Dir.peek("fm", "QuestMainStart")
    if Core.valid(qs) then
        local t
        pcall(function() t = A.markup_to_speech(Core.read_text(qs.TitleText)) end)
        st[#st + 1] = string.format("QS on=%s vis=%s op=%s txt=%s",
            tostring(Core.on_screen(qs)), vis_of(qs), op_of(qs), tostring(t))
    else
        st[#st + 1] = "QS nil"
    end
    local lg = Dir.peek("fm", "QuestMainLogo")
    if Core.valid(lg) then
        st[#st + 1] = string.format("LOGO on=%s vis=%s op=%s",
            tostring(Core.on_screen(lg)), vis_of(lg), op_of(lg))
    else
        st[#st + 1] = "LOGO nil"
    end
    trace(table.concat(st, " | "))
end

function Card.is_active()
    tick = tick + 1
    pending = nil
    trace_state()
    local host = Core.first_on_screen("AT_UIQuestMainStart", tick)
    if not host then
        seen = nil
        return false
    end
    local t
    pcall(function() t = A.markup_to_speech(Core.read_text(host.TitleText)) end)
    if not t or t == "" or t == seen then return false end
    pending = t
    -- Active only while there is something new to say; released right after (notice
    -- pattern — the card must never hold the screen).
    return true
end

function Card.reset() end

function Card.update()
    if not pending then return end
    seen = pending
    if TRACE then print("[KakarotAccess] qcard SPOKE " .. pending .. "\n") end
    Speech.say(pending, false)   -- queued: never cuts a subtitle line
    pending = nil
end

return Card
