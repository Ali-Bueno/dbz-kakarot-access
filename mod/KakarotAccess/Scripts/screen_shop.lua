-- Screen adapter: shop (Shop_Top_C -> UAT_UIShopTop).
--
-- The shop's item list is WidgetSkillList, a UAT_UICmn00MenuList11 -> UAT_UIMenuListBase00,
-- which exposes the reflected GetSelectValue() index — so this reader uses the generic
-- A.list_selected_row archetype (robust index read) rather than any highlight heuristic.
-- The selected row's TxtName (item) + TxtNum (price/count) are announced; the category tab
-- bars are highlight-only (no reflected index) and left for a later pass.
--
-- Additive + fully guarded: if a widget name differs at runtime (WidgetSkillList /
-- ListPlateCtn / TxtName were read from the header dump, not yet verified live), the
-- reflected read simply returns nil, the adapter stays inactive, and nothing else changes.

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")

local Shop = {}

local ann = Core.make_announcer()
local host, list = nil, nil
local tick = 0

function Shop.is_active()
    tick = tick + 1
    host = Core.cached_live("Shop_Top_C", tick)   -- cheap: cached ref, no per-tick scan
    if not Core.on_screen(host) then return false end
    list = host.WidgetSkillList
    return A.list_select_index(list) ~= nil
end

function Shop.reset() ann:reset() end

function Shop.update()
    local row = A.list_selected_row(list)
    if not row then return end
    -- "Shop" on entry (mod-added screen name); the focused item + its number as it changes.
    ann:focus(I18n.t("shop"), nil, Core.phrase(row.name, row.num), nil, nil)
end

return Shop
