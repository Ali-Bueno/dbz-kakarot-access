-- Screen adapter: Super Attack training shop (Shop_Training_C -> UAT_UIShopTraining).
--
-- Talking to a training NPC (e.g. Krillin) opens the skill-acquisition menu: a list of
-- Super Attacks (Xlist_List05_Lay7, a UAT_UIMenuListBase00) with a DETAIL PANEL that the
-- game refreshes on every cursor move (its own DetailUpdate()). So no highlight/index
-- heuristic is needed — the detail panel IS the selection readout. We read the selected
-- skill's name / ki cost / acquisition cost / description straight from it, diff-gated on
-- the skill name. Every string is game-authored → language-agnostic (only the "Super
-- Attacks" fallback title and the "Ki required" glue come from i18n).
--
-- Widget names verified from the F7 dump (2026-07-04, Krillin) + the CXX header
-- UAT_UIShopTraining: Txt_Cap00 skill name, Txt_Cap03 ki value, Txt_Detail description,
-- Txt_Cap01 current character, Start_Quest_Bar00_00.Txt_Progress acquisition cost
-- ("D Medals: N"), and the list title Xlist_List05_Lay7.Txt_Title ("Super Attack List").
-- All reads are valid()/pcall-guarded (a bad deref on this game is an uncatchable abort).

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")

local Training = {}

local ann = Core.make_announcer()
local host = nil
local tick = 0
local last_idx = nil   -- last list selection index, to force a re-announce on cursor move

-- Read a text node hanging off the host by field name, guarded. nil if absent/empty.
local function field_text(name)
    local t
    pcall(function() t = Core.read_text(host[name]) end)
    return t
end

-- The list title ("Super Attack List") = the screen name, read live from the game so it
-- follows the game's language; falls back to the localized "Super Attacks".
local function screen_name()
    local t
    pcall(function() t = Core.read_text(host.Xlist_List05_Lay7.Txt_Title) end)
    return t or I18n.t("training")
end

-- Acquisition cost line ("D Medals: N"), nested in the first quest bar. Guarded.
local function cost_text()
    local t
    pcall(function() t = Core.read_text(host.Start_Quest_Bar00_00.Txt_Progress) end)
    return t
end

function Training.is_active()
    tick = tick + 1
    host = Core.cached_live("Shop_Training_C", tick)
    if not Core.on_screen(host) then return false end
    -- Active only once the detail panel has a skill name (avoids reading a half-open menu).
    return field_text("Txt_Cap00") ~= nil
end

function Training.reset() last_idx = nil; ann:reset() end

function Training.update()
    local name = field_text("Txt_Cap00")
    if not name then return end
    -- Force a re-announce when the LIST selection index moves, even if the detail panel's
    -- skill NAME happens to repeat — so cursor movement is always spoken once more skills
    -- (or characters via LB/RB) are available. The list is a MenuListBase00 (GetSelectValue).
    local idx = A.list_select_index(host.Xlist_List05_Lay7)
    if idx ~= last_idx then
        last_idx = idx
        ann:invalidate()
    end
    local ki = field_text("Txt_Cap03")
    local value = Core.phrase(
        ki and string.format(I18n.t("ki_required_fmt"), ki) or nil,
        cost_text())
    local desc = field_text("Txt_Detail")
    -- screen (on entry) = the list title; tab = the current character (Txt_Cap01), spoken
    -- on change; name = the skill; value = ki + cost; tooltip = the description.
    ann:focus(screen_name(), field_text("Txt_Cap01"), name,
        value ~= "" and value or nil, function() return desc end)
end

return Training
