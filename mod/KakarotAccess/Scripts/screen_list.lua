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

-- class      : class short-name for FindAllOf. NOTE: on this game FindAllOf on a
--              NATIVE base can find NOTHING while the blueprint subclass name works
--              (AT_UICommunityBoard lesson) — pass the blueprint `_C` name when the
--              screen has one (the Items menu is Start_Item_C, not AT_UIItemMenu).
-- list_member: the MenuListBase list property on the host (e.g. "Xmenu_List00")
-- name_fn    : () -> localized screen name, spoken on entry (evaluated live for language)
-- tab_member : OPTIONAL text member on the LIST holding the current category/tab title
--              (e.g. "TxtTitle" on MenuListBase01 — the Items menu's "Recovery").
--              Only pass a member VERIFIED on that list's class: reading a property a
--              class doesn't declare is the uncatchable C++ abort.
-- detail_nodes: OPTIONAL array of the host's detail-pane text WIDGET NAMES, read as the
--              focused item's tooltip in this order. These are usually blueprint-only
--              members (no property access allowed) — they're found by full-name
--              subtree scan instead (the screen_tutorial technique), so any name is
--              safe here; missing ones are simply skipped.
function M.new(class, list_member, name_fn, tab_member, detail_nodes)
    local S = {}
    local ann = Core.make_announcer()
    local host, list, tick = nil, nil, 0
    local boxes = nil   -- detail-pane text boxes by detail_nodes index (lazy collect)

    local function collect_boxes()
        boxes = {}
        if not (detail_nodes and Core.valid(host)) then return end
        local prefix = host:GetFullName():match("%s(.+)$") or host:GetFullName()
        for _, o in pairs(Core.cached_all("CFUIMultiLineTextBox", tick)) do
            if Core.valid(o) then
                local fn = o:GetFullName()
                if fn:find(prefix, 1, true) then
                    for i, nm in ipairs(detail_nodes) do
                        if fn:find("." .. nm .. ".", 1, true) then boxes[i] = o end
                    end
                end
            end
        end
    end

    -- The visible detail-pane lines, in detail_nodes order (lazy: called by the
    -- announcer only when the focused item changes).
    local function tooltip()
        if not boxes then return nil end
        local parts = {}
        for i = 1, #detail_nodes do
            local o = boxes[i]
            if o and Core.valid(o) and Core.is_visible(o) then
                local ok, s = pcall(function() return o.Text:ToString() end)
                if ok and s and s ~= "" then parts[#parts + 1] = A.markup_to_speech(s) end
            end
        end
        return #parts > 0 and table.concat(parts, ", ") or nil
    end

    function S.is_active()
        tick = tick + 1
        host = Core.first_on_screen(class, tick)   -- picks the live pooled instance each tick
        if not host then boxes = nil return false end
        list = host[list_member]
        if A.list_select_index(list) == nil then return false end
        if detail_nodes and not boxes then collect_boxes() end
        return true
    end

    function S.reset() ann:reset() boxes = nil end

    function S.update()
        local row = A.list_selected_row(list)
        if not row then return end
        local tab = tab_member and Core.read_text(list[tab_member]) or nil
        -- screen name on entry; category tab on change; the focused item name + its
        -- number/count as it changes; the detail pane as the tooltip.
        ann:focus(name_fn(), tab, Core.phrase(row.name, row.num), nil,
            detail_nodes and tooltip or nil)
    end

    return S
end

return M
