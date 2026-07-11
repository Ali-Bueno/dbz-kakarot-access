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
-- name_node  : OPTIONAL detail-pane TITLE widget name used as the LIVE selection name
--              (subtree-scanned like detail_nodes). Needed when GetSelectValue() is
--              FROZEN on a screen (the Items menu stuck at index 0 while the cursor
--              moved — telemetry 2026-07-06, same disease as the cooking list): the
--              detail pane is repopulated by the game on every cursor move, so its
--              title IS the selection. Falls back to the indexed row when absent.
function M.new(class, list_member, name_fn, tab_member, detail_nodes, name_node)
    local S = {}
    local ann = Core.make_announcer()
    local host, list, tick = nil, nil, 0
    local boxes = nil   -- detail-pane text boxes by detail_nodes index (lazy collect)

    -- Diagnostic (palette-style): appends this screen's gate state to
    -- dumps/dump_items.txt on every change, so a mute list screen (the Items menu,
    -- 2026-07-06) is diagnosable from the file alone. Cheap: writes on change only.
    -- RE-ARMED 2026-07-11: user reports the Items list is mute when the category HAS
    -- items (only reads when empty). This logs host enum / list valid / idx / rowName /
    -- liveName on every change to dumps/dump_items.txt, so one capture (open Items with
    -- items present, move the cursor) pins the failing path. Turn OFF once diagnosed.
    local DEBUG = true
    local last_dbg = nil
    local function dbg(s)
        if not DEBUG then return end
        s = class .. ": " .. s
        if s == last_dbg then return end
        last_dbg = s
        pcall(function()
            local src = debug.getinfo(1, "S").source:sub(2)
            local dir = src:match("^(.*)[/\\]") or "."
            local f = io.open(dir .. "\\dumps\\dump_items.txt", "a")
            if f then f:write(string.format("[%d] %s\n", os.time(), s)) f:close() end
        end)
    end

    local name_box = nil   -- the name_node text box (lazy collect, with boxes)

    local function collect_boxes()
        boxes = {}
        name_box = nil
        if not ((detail_nodes or name_node) and Core.valid(host)) then return end
        local prefix = host:GetFullName():match("%s(.+)$") or host:GetFullName()
        for _, o in pairs(Core.cached_all("CFUIMultiLineTextBox", tick)) do
            if Core.valid(o) then
                local fn = o:GetFullName()
                if fn:find(prefix, 1, true) then
                    for i, nm in ipairs(detail_nodes or {}) do
                        if fn:find("." .. nm .. ".", 1, true) then boxes[i] = o end
                    end
                    if name_node and fn:find("." .. name_node .. ".", 1, true) then
                        name_box = o
                    end
                end
            end
        end
    end

    -- The live selection name from the detail-pane title (see name_node above).
    local function live_name()
        if not (name_box and Core.valid(name_box)) then return nil end
        local ok, s = pcall(function() return name_box.Text:ToString() end)
        return (ok and s and s ~= "") and A.markup_to_speech(s) or nil
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
        local idx = A.list_select_index(list)
        if (detail_nodes or name_node) and not boxes then collect_boxes() end
        if DEBUG then
            local he = "?"
            pcall(function() he = tostring(tonumber(host:GetVisibility())) end)
            local row = idx and A.list_selected_row(list)
            dbg(string.format("host enum=%s list=%s idx=%s rowName=%s liveName=%s",
                he, tostring(Core.valid(list)), tostring(idx),
                tostring(row and row.name), tostring(live_name())))
        end
        if idx == nil and live_name() == nil then return false end
        return true
    end

    function S.reset() ann:reset() boxes = nil name_box = nil end

    function S.update()
        local row = A.list_selected_row(list)
        -- Live detail-pane name first (follows the cursor even when the reflected
        -- index is frozen); the indexed row is the fallback + count source.
        local name = live_name() or (row and row.name) or nil
        if not name then return end
        local tab = tab_member and Core.read_text(list[tab_member]) or nil
        -- screen name on entry; category tab on change; the focused item name + its
        -- number/count as it changes; the detail pane as the tooltip.
        ann:focus(name_fn(), tab, name, nil, detail_nodes and tooltip or nil)
    end

    return S
end

return M
