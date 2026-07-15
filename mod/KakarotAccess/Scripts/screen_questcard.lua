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

local Card = {}

-- Cards overlay gameplay/cinematics and never pause the game.
Card.nav_mute = false

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
Card.keyhelp_auto = false

local tick = 0
local seen = nil      -- card text already spoken while its card stays on screen
local pending = nil   -- text to speak this tick (computed in is_active)

function Card.is_active()
    tick = tick + 1
    pending = nil
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
    Speech.say(pending, false)   -- queued: never cuts a subtitle/menu announcement
    pending = nil
end

return Card
