-- Reactive reader: quest telop banners (Quest_Main_Telop_C — "Sub Story" captions,
-- quest names, start/clear banners that slide over gameplay).
--
-- From the CXX dump: each Quest_Main_Telop_C has 12 fixed Telop_Txt_C slots
-- (L/R × Top/Bottom × 00..02), each with Txt_Caption (the small label) + Txt_Name
-- (the quest title; Txt_Name_Foreign_Lang00 is the alt-language duplicate). Slots are
-- pooled: a visible slot's text is spoken ONCE (dedup by content) and its entry is
-- dropped when the banner leaves the screen, so a later identical banner re-announces.
--
-- Registered BELOW the dialogue reader: a subtitle line wins the tick, and the banner
-- is still pending (its widget stays on screen a few seconds) so it speaks right after.
-- Banners never pause the game → nav_mute = false keeps the radar running (menus.md).

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")

local Telop = {}

Telop.nav_mute = false

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
Telop.keyhelp_auto = false

local tick = 0
local seen = {}      -- text -> true while its banner is still on screen
local pending = {}   -- unspoken texts, in discovery order

local SLOTS = {
    "Telop_Txt_L_Top00", "Telop_Txt_L_Top01", "Telop_Txt_L_Top02",
    "Telop_Txt_R_Top00", "Telop_Txt_R_Top01", "Telop_Txt_R_Top02",
    "Telop_Txt_L_Bottom00", "Telop_Txt_L_Bottom01", "Telop_Txt_L_Bottom02",
    "Telop_Txt_R_Bottom00", "Telop_Txt_R_Bottom01", "Telop_Txt_R_Bottom02",
}

local function slot_text(s)
    if not Core.valid(s) or not Core.is_visible(s) then return nil end
    local cap, name
    pcall(function()
        cap = A.markup_to_speech(Core.read_text(s.Txt_Caption))
        name = A.markup_to_speech(Core.read_text(s.Txt_Name))
            or A.markup_to_speech(Core.read_text(s.Txt_Name_Foreign_Lang00))
    end)
    local t = Core.phrase(cap, name)
    return t ~= "" and t or nil
end

-- Texts currently shown by any on-screen telop container. Idle cost is one cheap
-- on_screen check per pooled container; slots are read only while a banner is up.
local function current_texts()
    local out = {}
    for _, tp in ipairs(Core.cached_all("Quest_Main_Telop_C", tick)) do
        if Core.on_screen(tp) then
            for _, m in ipairs(SLOTS) do
                local t
                pcall(function() t = slot_text(tp[m]) end)
                if t then out[#out + 1] = t end
            end
        end
    end
    return out
end

function Telop.is_active()
    tick = tick + 1
    local texts = current_texts()
    local alive = {}
    for _, t in ipairs(texts) do
        alive[t] = true
        if not seen[t] then
            seen[t] = true
            pending[#pending + 1] = t
        end
    end
    -- Forget banners that left the screen (a later repeat re-announces), and drop
    -- pending entries whose banner vanished before we got to speak (stale info).
    for k in pairs(seen) do
        if not alive[k] then seen[k] = nil end
    end
    local live = {}
    for _, t in ipairs(pending) do
        if alive[t] then live[#live + 1] = t end
    end
    pending = live
    return #pending > 0
end

function Telop.reset() end

function Telop.update()
    for _, t in ipairs(pending) do
        Speech.say(t, false)   -- queued: never cuts a subtitle/menu announcement
        -- keep it in `seen` so it won't re-queue while the banner stays up
    end
    pending = {}
end

return Telop
