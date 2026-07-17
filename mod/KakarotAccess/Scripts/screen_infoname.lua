-- Notice reader: cinematic character-introduction cards (Info_Name_C ->
-- AT_UIInfoNameCore, driven by the sequencer's ATMovieSceneCharaIntroductionSection).
--
-- The little card some cinematics show when a character first appears ("Gohan —
-- hijo de Goku", "Piccolo — rival de Goku"): NameTxt + PopularNameTxt, or their
-- _Large twins when the section sets IsLargeSize — all reflected (ObjectDump,
-- AT_UIInfoNameCore 0x3A0..0x3C0; the visible pair is picked by the Switchars).
--
-- Instances are the BP subclass Info_Name_C (WidgetBlueprintGeneratedClass, super =
-- AT_UIInfoNameCore) — scanned by that name (the FindAllOf native-base gotcha).
-- fm.InfoName (AT_UIFieldManager 0x668) is deliberately NOT directory-mapped: the
-- fm auto-overlay fields have twice proven to stay null while the pooled widget
-- lives (InfoLevelUp, BattleRushSpeedCore) — the scan path serves it, and the
-- QUIET_EXEMPT entry in ui_core keeps it scannable mid-cinematic (cards appear
-- with no press, exactly when quiet mode reigns).
--
-- Telop pattern: each card speaks ONCE per appearance (dedup while it stays live,
-- forgotten when it leaves), queued so it never cuts a subtitle line. pane_live-
-- gated: the card animates out by fading (OutAnim), and a parked pooled instance
-- keeps its last text.

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")

local Card = {}

-- Cards overlay cinematics and never pause the game.
Card.nav_mute = false

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua).
Card.keyhelp_auto = false

local tick = 0
local seen = nil      -- card text already spoken while its card stays live
local pending = nil   -- text to speak this tick (computed in is_active)

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
local function card_text(host)
    local name = node_text(host, "NameTxt") or node_text(host, "NameTxt_Large")
    local pop  = node_text(host, "PopularNameTxt") or node_text(host, "PopularNameTxt_Large")
    if not name and not pop then return nil end
    return Core.phrase(name, pop)
end

function Card.is_active()
    tick = tick + 1
    pending = nil
    local host = Core.first_on_screen("Info_Name_C", tick)
    if not host or not Core.pane_live(host) then
        seen = nil
        return false
    end
    local t = card_text(host)
    if not t or t == seen then return false end
    pending = t
    -- Active only while there is something new to say; released right after (notice
    -- pattern — the card must never hold the screen).
    return true
end

function Card.reset() end

function Card.update()
    if not pending then return end
    seen = pending
    Speech.say(pending, false)   -- queued: never cuts a subtitle line
    pending = nil
end

return Card
