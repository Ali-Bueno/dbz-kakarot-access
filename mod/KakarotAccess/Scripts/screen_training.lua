-- Screen adapter: Super Attack training shop (Shop_Training_C -> UAT_UIShopTraining).
--
-- Talking to a training NPC (e.g. Krillin) or using a field training point opens the
-- skill-acquisition menu: a list of Super Attacks (Xlist_List05_Lay7, a
-- UAT_UIMenuListBase00) with a DETAIL PANEL. SELECTION (revised 2026-07-06, wasteland
-- training point, "reads only on entry"): the list's reflected GetSelectValue() FREEZES
-- after entry on this screen family (cooking / Items disease), and the detail panel's
-- skill name (Txt_Cap00) can be EMPTY — a locked/task skill shows the Task / How-to-
-- acquire / recommended-level panel instead. So the spoken name is the detail title
-- WITH the highlighted list row as fallback, and a selection SIGNATURE over every text
-- the game rewrites on a cursor move re-arms the announcer even when the spoken name
-- source goes stale (the cooking fix).
--
-- Widget names verified from the F7 dump (2026-07-04, Krillin) + the CXX header
-- UAT_UIShopTraining: Txt_Cap00 skill name, Txt_Cap03 ki value, Txt_Detail description,
-- Txt_Cap01 current character, Txt_Task / Txt_Cond / Txt_Recommend the locked-skill
-- panel (visible-text dump 2026-07-06), Start_Quest_Bar00_00.Txt_Progress acquisition
-- cost ("D Medals: N"), and the list title Xlist_List05_Lay7.Txt_Title.
-- All reads are valid()/pcall-guarded (a bad deref on this game is an uncatchable abort).

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")

local Training = {}

-- Appends a line per selection-signature change to dumps/dump_training.txt (index vs
-- detail title vs row name) — remote ground truth if cursor moves are still silent.
-- Turn OFF once the user confirms cursor moves are spoken.
local DEBUG = false

local ann = Core.make_announcer()
local host = nil
local tick = 0
local last_sig = nil   -- selection signature, re-arms the announcer on cursor move
local empty_ticks = 0  -- consecutive update ticks with no skill name (empty-list settle)

-- Ticks the detail panel must stay empty before we commit to "empty list": right after
-- the menu opens the panel is briefly blank even when skills exist, and announcing
-- "empty" then reading the first skill a tick later would be noise.
local EMPTY_SETTLE_TICKS = 4

-- Read a text node hanging off the host by field name, guarded. nil if absent/empty.
local function field_text(name)
    local t
    pcall(function() t = Core.read_text(host[name]) end)
    return t
end

-- The list title ("Super Attack List"), read live from the game so it follows the
-- game's language. nil while the title widget has no text (menu not really up yet).
local function list_title()
    local t
    pcall(function() t = Core.read_text(host.Xlist_List05_Lay7.Txt_Title) end)
    return t
end

-- The screen name = the live list title, falling back to the localized "Super Attacks".
local function screen_name()
    return list_title() or I18n.t("training")
end

-- Acquisition cost line ("D Medals: N"), nested in the first quest bar. Guarded.
local function cost_text()
    local t
    pcall(function() t = Core.read_text(host.Start_Quest_Bar00_00.Txt_Progress) end)
    return t
end

-- Rich text -> speech (drop <span>, resolve <inputicon>); nil if empty.
local function clean(t) return t and A.markup_to_speech(t) or nil end

-- The skill list widget (guarded property access).
local function skill_list()
    local l
    pcall(function() l = host.Xlist_List05_Lay7 end)
    return l
end

-- The locked-skill panel texts (Task / How to acquire / recommended level), in
-- on-screen order — the detail readout when the skill isn't purchasable yet.
local LOCKED_MEMBERS = { "Txt_Task", "Txt_Cond", "Txt_Recommend" }

-- Full detail readout for the announcer's tooltip: description first (purchasable
-- skill), then the locked-skill panel lines that are showing.
local function detail()
    local parts = {}
    parts[#parts + 1] = clean(field_text("Txt_Detail"))
    for _, m in ipairs(LOCKED_MEMBERS) do
        local node
        pcall(function() node = host[m] end)
        if Core.on_screen(node) then
            parts[#parts + 1] = clean(Core.read_text(node))
        end
    end
    if #parts == 0 then return nil end
    return table.concat(parts, ", ")
end

-- Signature of the current selection: every text the game rewrites on a cursor move
-- (raw, only compared, never spoken) + the reflected index and highlighted row name
-- (in case only those move). A change re-arms the announcer.
local function selection_sig(row, idx)
    return Core.phrase(tostring(idx), row and row.name or nil,
        field_text("Txt_Cap00"), field_text("Txt_Cap03"), field_text("Txt_Detail"),
        field_text("Txt_Task"), field_text("Txt_Cond"), field_text("Txt_Recommend"),
        cost_text())
end

local function dump_sample(sig, row, idx)
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_training.txt", "a")
        if not f then return end
        f:write(string.format("[%d] idx=%s row=%s cap00=%s sig=%s\n",
            os.time(), tostring(idx), tostring(row and row.name),
            tostring(field_text("Txt_Cap00")), (sig or ""):sub(1, 160)))
        f:close()
    end)
end

function Training.is_active()
    tick = tick + 1
    host = Core.cached_live("Shop_Training_C", tick)
    if not Core.on_screen(host) then return false end
    -- Active once the list title renders. Gating on the detail panel's skill name (as
    -- before) meant a character with NO acquirable Super Attacks yet read NOTHING at
    -- all (empty list -> empty detail panel, user report 2026-07-06 with Piccolo).
    return list_title() ~= nil
end

function Training.reset() last_sig = nil; empty_ticks = 0; ann:reset() end

function Training.update()
    local list = skill_list()
    local row = A.list_selected_row(list)
    local idx = A.list_select_index(list)
    -- Detail-pane title first; the highlighted list row as fallback (the detail name
    -- is EMPTY for locked/task skills — visible-text dump 2026-07-06).
    local name = clean(field_text("Txt_Cap00")) or (row and clean(row.name)) or nil
    if not name then
        -- Empty skill list: after a short settle (the panel is briefly blank while the
        -- menu opens), announce the screen + character with an explicit "empty list".
        empty_ticks = empty_ticks + 1
        if empty_ticks >= EMPTY_SETTLE_TICKS then
            ann:focus(screen_name(), clean(field_text("Txt_Cap01")), I18n.t("list_empty"), nil, nil)
        end
        return
    end
    empty_ticks = 0
    -- Selection moved but the spoken name key didn't (stale name source) → force the
    -- announcer to re-speak name + value + detail anyway (cooking-fix pattern; the
    -- reflected index alone proved frozen on this screen family).
    local sig = selection_sig(row, idx)
    if sig ~= last_sig then
        if last_sig ~= nil then ann:invalidate() end
        last_sig = sig
        if DEBUG then dump_sample(sig, row, idx) end
    end
    local ki = field_text("Txt_Cap03")
    local value = Core.phrase(
        ki and string.format(I18n.t("ki_required_fmt"), ki) or nil,
        cost_text())
    -- screen (on entry) = the list title; tab = the current character (Txt_Cap01), spoken
    -- on change; name = the skill; value = ki + cost; tooltip = description or the
    -- locked-skill Task / How-to-acquire / recommended-level panel.
    ann:focus(screen_name(), clean(field_text("Txt_Cap01")), name,
        value ~= "" and value or nil, detail)
end

return Training
