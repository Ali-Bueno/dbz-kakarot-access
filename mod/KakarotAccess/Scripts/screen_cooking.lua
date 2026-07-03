-- Screen adapter: cooking menu (Shop_Cook_C -> UAT_UICookingMenu; campfire / cook NPCs).
--
-- The recipe list is CookMenuList.WL_Shop_Cmn_List, a UAT_UICmn00MenuList9 ->
-- UAT_UIMenuListBase00 exposing the reflected GetSelectValue() index — so the recipe
-- rows go through the generic A.list_selected_row reader (TxtName + TxtNum), same as
-- the shop. The detail pane lives on the blueprint itself and is read as the recipe's
-- "tooltip" following the crafting rules in reference/ui-accessibility/inventories.md:
--   * effect lines        Txt_Detail00_01..03 (and perpetual buff Txt_Detail01_01..03)
--   * effect time         Txt_Detail02
--   * ingredients         Shop_Cmn_Bar_01_00.. (Xlist_Bar00_C rows: Txt_List name +
--                         Txt_Num count — the have/needed readout)
--   * description         Txt_Detail03 (long text, spoken last)
-- The genre/category title (CookMenuList.WL_Txt_GenreTitle) is the announcer's tab, so
-- switching category (LB/RB) speaks the new genre. The cooking-complete banner and the
-- result window (WL_CookingComp.WL_Text / WL_CookingResult.TextBoxCtn) are spoken
-- diff-gated when they appear over the menu.
--
-- Additive + fully guarded: member names come from the CXX header dump (Shop_Cook.hpp /
-- AT.hpp, not yet verified live) — a wrong name reads nil and that part is skipped;
-- if the list itself differs the adapter simply stays inactive.

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")
local Speech = require("speech")

local Cooking = {}

local ann = Core.make_announcer()
local host, list, overlay = nil, nil, nil
local last_overlay = nil
local tick = 0

-- Detail-pane text members in on-screen (top-to-bottom) order; description handled
-- separately so the long text always comes last.
local DETAIL_MEMBERS = {
    "Txt_Detail00_01", "Txt_Detail00_02", "Txt_Detail00_03",   -- effect lines
    "Txt_Detail01_01", "Txt_Detail01_02", "Txt_Detail01_03",   -- perpetual (permanent) buff lines
    "Txt_Detail02",                                            -- effect duration
}
local DESCRIPTION_MEMBER = "Txt_Detail03"

-- Ingredient rows: contiguous blueprint members Shop_Cmn_Bar_01_00, _01, … — probe
-- until the first missing one instead of hardcoding how many the blueprint has.
local function ingredient_parts()
    local parts, i = {}, 0
    while true do
        local bar = host[string.format("Shop_Cmn_Bar_01_%02d", i)]
        if not Core.valid(bar) then break end
        if Core.on_screen(bar) then
            local p = Core.phrase(Core.read_text(bar.Txt_List), Core.read_text(bar.Txt_Num))
            if p ~= "" then parts[#parts + 1] = p end
        end
        i = i + 1
    end
    return parts
end

-- The selected recipe's full detail readout (lazy: the announcer calls this only when
-- the selection actually changes).
local function detail()
    local parts = {}
    for _, m in ipairs(DETAIL_MEMBERS) do
        local node = host[m]
        if Core.on_screen(node) then
            local t = Core.read_text(node)
            if t then parts[#parts + 1] = t end
        end
    end
    for _, p in ipairs(ingredient_parts()) do parts[#parts + 1] = p end
    local desc = host[DESCRIPTION_MEMBER]
    if Core.on_screen(desc) then
        local t = Core.read_text(desc)
        if t then parts[#parts + 1] = t end
    end
    if #parts == 0 then return nil end
    return table.concat(parts, ", ")
end

-- The current genre/category title, used as the announcer's tab (spoken on change).
local function genre()
    local cml
    pcall(function() cml = host.CookMenuList end)
    if not Core.valid(cml) then return nil end
    local t
    pcall(function()
        local node = cml.WL_Txt_GenreTitle
        if Core.on_screen(node) then t = Core.read_text(node) end
    end)
    return t
end

-- Text of whichever cooking overlay is up (complete banner / result window), or nil.
local function overlay_text()
    local comp
    pcall(function() comp = host.WL_CookingComp end)
    if Core.valid(comp) and Core.on_screen(comp) then
        local t = Core.read_text(comp.WL_Text)
        if t then return t end
    end
    local res
    pcall(function() res = host.WL_CookingResult end)
    if Core.valid(res) and Core.on_screen(res) then
        local parts = {}
        pcall(function()
            local boxes = res.TextBoxCtn
            for i = 1, #boxes do
                local b = boxes[i]
                if Core.on_screen(b) then
                    local t = Core.read_text(b)
                    if t then parts[#parts + 1] = t end
                end
            end
        end)
        if #parts > 0 then return table.concat(parts, ", ") end
    end
    return nil
end

function Cooking.is_active()
    tick = tick + 1
    host = Core.cached_live("Shop_Cook_C", tick)   -- pooled: cheap cached ref per tick
    if not Core.valid(host) then overlay = nil return false end
    -- The complete/result overlays can be up while the menu body is hidden (the cook
    -- demo collapses it), so they keep the adapter active on their own.
    overlay = overlay_text()
    if overlay then return true end
    if not Core.on_screen(host) then return false end
    local cml
    pcall(function() cml = host.CookMenuList end)
    list = Core.valid(cml) and cml.WL_Shop_Cmn_List or nil
    return A.list_select_index(list) ~= nil
end

function Cooking.reset()
    ann:reset()
    last_overlay = nil
end

function Cooking.update()
    if overlay then
        if overlay ~= last_overlay then
            last_overlay = overlay
            Speech.say(overlay, true)   -- context change: the result interrupts
        end
        return
    end
    last_overlay = nil
    local row = A.list_selected_row(list)
    if not row then return end
    -- "Cooking" on entry; genre as the tab; recipe name; its count as the value (so a
    -- post-cook count change speaks just the number); full detail as the tooltip.
    ann:focus(I18n.header(13), genre(), row.name, row.num, detail)
end

return Cooking
