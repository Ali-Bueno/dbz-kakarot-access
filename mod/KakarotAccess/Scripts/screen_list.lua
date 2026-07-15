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
local I18n = require("i18n")
local Mem = require("mem")

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
-- empty_off (OPTIONAL): byte offset of a non-reflected int32 on the host that is != 0 while
-- the current category HAS items and 0 when it's EMPTY. The item list is a fixed pool and its
-- UI goes fully STALE on an empty category (row text, detail pane, visible count all retain the
-- last item), so this native flag is the only fresh "empty" signal. When set + readable, an
-- empty category is announced instead of silently keeping the stale item.
function M.new(class, list_member, name_fn, tab_member, detail_nodes, name_node, empty_off)
    local S = {}
    -- Deliberately opened sub-screens (inventory, Dragon Balls, …): no boot-flash risk,
    -- so one registry confirmation tick suffices — shaves ~100 ms off every entry (the
    -- global CONFIRM_TICKS=2 exists for screens that FLASH at boot).
    S.confirm_ticks = 1
    local ann = Core.make_announcer()
    local host, list, tick = nil, nil, 0
    local boxes = nil   -- detail-pane text boxes by detail_nodes index (lazy collect)

    -- True when empty_off is wired and the host reports the current category empty.
    local function category_empty()
        return empty_off ~= nil and Mem.is_loaded() and Mem.i32(host, empty_off) == 0
    end
    local empty_shown = false   -- gates the empty announcement to once per empty-category entry

    -- Diagnostic (palette-style): appends this screen's gate state to
    -- dumps/dump_items.txt on every change, so a mute list screen (the Items menu,
    -- 2026-07-06) is diagnosable from the file alone. Cheap: writes on change only.
    -- Diagnosed 2026-07-11: reflected index + detail-pane name read fine for populated
    -- categories; empty categories are told by the detail pane staying nil (nil_streak) since
    -- the list is a fixed pool. Keep OFF; flip on only for a short capture to dump_items.txt.
    local DEBUG = false   -- 2026-07-15 capture DIAGNOSED the mute items menu: name_box
                          -- sometimes latched Txt_Title00's empty SubTxt instead of
                          -- mainTxt (pool iteration order) — fixed by the is_maintxt
                          -- preference in collect_boxes.
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

    -- A node name matches either as a PATH SEGMENT (".Txt_Title00." — the usual wrapper
    -- widget whose inner mainTxt is the box) or as the SUFFIX (the node IS the text box:
    -- its full name simply ends with ".Txt_Title00", no closing dot).
    local function match_node(fn, nm)
        return fn:find("." .. nm .. ".", 1, true) ~= nil
            or fn:sub(-(#nm + 1)) == ("." .. nm)
    end

    -- A wrapper node (Txt_Title00) holds TWO text boxes: mainTxt (the real content, the
    -- one Core.text_of reads) and SubTxt (empty). The pool iterates in scan order, so
    -- without a preference the LAST match won — sometimes SubTxt — and liveName came
    -- back nil with the item name on screen: the intermittent mute items menu
    -- (dump_items capture, 2026-07-15). mainTxt always wins; anything else only fills
    -- an empty slot.
    local function is_maintxt(fn) return fn:sub(-8) == ".mainTxt" end

    local collect_info = "never-ran"   -- what the last collection saw (DEBUG dump)

    local function collect_boxes()
        boxes = {}
        name_box = nil
        if not ((detail_nodes or name_node) and Core.valid(host)) then return end
        local prefix = host:GetFullName():match("%s(.+)$") or host:GetFullName()
        local found = false
        local under, sample = 0, {}
        for _, o in pairs(Core.cached_all("CFUIMultiLineTextBox", tick)) do
            if Core.valid(o) then
                local fn = o:GetFullName()
                if fn:find(prefix, 1, true) then
                    under = under + 1
                    if #sample < 3 then sample[#sample + 1] = fn:sub(#prefix + 2, #prefix + 80) end
                    for i, nm in ipairs(detail_nodes or {}) do
                        if match_node(fn, nm) and (boxes[i] == nil or is_maintxt(fn)) then
                            boxes[i] = o
                            found = true
                        end
                    end
                    if name_node and match_node(fn, name_node)
                        and (name_box == nil or is_maintxt(fn)) then
                        name_box = o
                        found = true
                    end
                end
            end
        end
        collect_info = string.format("under=%d name_box=%s eg: %s",
            under, tostring(name_box ~= nil), table.concat(sample, " | "))
        -- Nothing of THIS host's subtree in the cached pool: the screen was just REBUILT
        -- and the pool is stale (its refresh was requested on entry — see is_active).
        -- Leave boxes nil so we re-collect next tick; a `{}` here would never retry and
        -- the menu would stay active-but-MUTE for the whole visit (the Items menu,
        -- 2026-07-15 — "worked once, never again").
        if not found then boxes = nil end
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

    local had_host = false
    function S.is_active()
        tick = tick + 1
        host = Core.first_on_screen(class, tick)   -- picks the live pooled instance each tick
        if not host then boxes = nil had_host = false return false end
        -- Entry event (host newly detected): this screen is rebuilt by the game on every
        -- open, so its detail-pane text boxes are NEW objects the always-alive
        -- CFUIMultiLineTextBox pool cache (30s refresh) doesn't hold yet. Ask for one
        -- re-scan now; collect_boxes retries until the fresh pool lands.
        if not had_host and (detail_nodes or name_node) then
            Core.refresh_all("CFUIMultiLineTextBox")
        end
        had_host = true
        list = host[list_member]
        local idx = A.list_select_index(list)
        if (detail_nodes or name_node) and not boxes then collect_boxes() end
        if DEBUG then
            local row = idx and A.list_selected_row(list)
            dbg(string.format("idx=%s empty=%s f620=%s f624=%s rowName=%s liveName=%s boxes=%s [%s]",
                tostring(idx), tostring(category_empty()),
                tostring(Mem.is_loaded() and Mem.i32(host, 0x620)),
                tostring(Mem.is_loaded() and Mem.i32(host, 0x624)),
                tostring(row and row.name), tostring(name_node and live_name()),
                tostring(boxes ~= nil), collect_info))
        end
        -- name_node screens (Items): stay active whenever the menu is up, so update() runs every
        -- tick — it reads the selected item, or (when the native flag says so) announces the
        -- category empty. An empty category leaves the whole list UI stale, so we can't gate on it.
        if name_node then return true end
        -- Plain list screens (e.g. Dragon Balls): activate only with a real selection.
        if idx == nil then return false end
        return true
    end

    function S.reset() ann:reset() boxes = nil name_box = nil empty_shown = false end

    function S.update()
        local tab = tab_member and Core.read_text(list[tab_member]) or nil
        -- Empty category: announce the section + "empty" (native flag; the UI itself is stale).
        -- reset() once on entry so the announcement isn't swallowed by the category-tab change
        -- (the announcer speaks a tab change on its own tick and drops the name that tick).
        if category_empty() then
            if not empty_shown then ann:reset() empty_shown = true end
            ann:focus(name_fn(), nil, Core.phrase(tab, I18n.t("list_empty")), nil, nil)
            return
        end
        empty_shown = false
        -- On a name_node screen (Items) the DETAIL PANE is the selection; the pooled list row
        -- carries a stale placeholder ("Ninguno") when empty, so DON'T fall back to it — an empty
        -- category is announced via nil_streak above instead. Plain screens use the indexed row.
        local name
        if name_node then
            name = live_name()
        else
            local row = A.list_selected_row(list)
            name = row and row.name
        end
        if not name then return end
        -- screen name on entry; category tab on change; the focused item name + its
        -- number/count as it changes; the detail pane as the tooltip.
        ann:focus(name_fn(), tab, name, nil, detail_nodes and tooltip or nil)
    end

    return S
end

return M
