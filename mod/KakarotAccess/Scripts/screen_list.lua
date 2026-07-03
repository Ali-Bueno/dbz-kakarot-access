-- Factory for the simple overworld menus whose selectable list is a UAT_UIMenuListBase00/01/03
-- (or subclass), i.e. it exposes the reflected int32 GetSelectValue() index and rows in
-- ListPlateCtn with TxtName / TxtNum. One factory covers every such screen (Items, Dragon
-- Balls, …) via the generic A.list_selected_row reader — no per-screen selection heuristic.
--
-- These screens have NO blueprint `_C` (they are instantiated directly from the native class),
-- so `class` is the native short-name passed to FindAllOf (e.g. "AT_UIItemMenu"). Additive +
-- fully guarded: if the host/list member name differs live, the reflected read returns nil and
-- the adapter simply stays inactive (harmless).

local Core = require("ui_core")
local A = require("ui_archetypes")

local M = {}

-- class      : native short-name for FindAllOf (e.g. "AT_UIItemMenu")
-- list_member: the MenuListBase list property on the host (e.g. "Xmenu_List00")
-- name_fn    : () -> localized screen name, spoken on entry (evaluated live for language)
function M.new(class, list_member, name_fn)
    local S = {}
    local ann = Core.make_announcer()
    local host, list, tick = nil, nil, 0

    function S.is_active()
        tick = tick + 1
        host = Core.first_on_screen(class, tick)   -- picks the live pooled instance each tick
        if not host then return false end
        list = host[list_member]
        return A.list_select_index(list) ~= nil
    end

    function S.reset() ann:reset() end

    function S.update()
        local row = A.list_selected_row(list)
        if not row then return end
        -- screen name on entry; the focused item name + its number/count as it changes.
        ann:focus(name_fn(), nil, Core.phrase(row.name, row.num), nil, nil)
    end

    return S
end

return M
