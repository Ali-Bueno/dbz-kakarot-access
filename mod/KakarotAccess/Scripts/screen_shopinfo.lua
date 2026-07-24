-- Screen adapter: information store (Shop_Info_C -> UAT_UIShopInfo).
--
-- The NPC "info shop" that sells map-location intel (fishing spots, hunting grounds,
-- fruit trees, mineral deposits). Verified from the F7 dump (2026-07-06) + the CXX
-- headers: rows are the blueprint's DIRECT properties Xlist_Bar05_00..07 (UShop_Info_C,
-- Shop_Info.hpp), each a UXlist_Bar05_C with Txt_List (name), Txt_Num (price),
-- Txt_Sold_Out ("Sold out") and the Ins_Cursor_Fad highlight image — the same
-- selected-row marker the options rows use. The detail panel (Txt_Detail) refreshes
-- with the selection and is the per-item tooltip; Txt_Title ("Buy") is the screen name.
--
-- UAT_UIShopInfo has NO reflected selection index (header: only widgets + CheckHovered),
-- so the cursor image IS the game's selection mechanism here.

local Core = require("ui_core")
local A = require("ui_archetypes")

local ShopInfo = {}

local ROW_COUNT = 8   -- Xlist_Bar05_00..07 (UShop_Info_C, Shop_Info.hpp)

local ann = Core.make_announcer()
local host = nil
local tick = 0
local state = nil     -- { title, name, value } computed in is_active, reused by update

local function clean(t) return t and A.markup_to_speech(t) or nil end

local function node_text(node)
    local t
    pcall(function() t = clean(Core.read_text(node)) end)
    return t
end

-- The selected row: the one showing its Ins_Cursor_Fad highlight. STRICT — no
-- first-row fallback: a pooled list that lingers after closing must not keep this
-- adapter active and shadow the screens registered below it (the ring pause).
local function selected_row()
    for i = 0, ROW_COUNT - 1 do
        local row
        pcall(function() row = host["Xlist_Bar05_" .. string.format("%02d", i)] end)
        if Core.valid(row) and Core.on_screen(row) and node_text(Core.member(row, "Txt_List")) then
            local ok, sel = pcall(function() return Core.is_visible(row.Ins_Cursor_Fad) end)
            if ok and sel then return row end
        end
    end
    return nil
end

function ShopInfo.is_active()
    tick = tick + 1
    host = Core.cached_live("Shop_Info_C", tick)
    if not Core.on_screen(host) then state = nil return false end
    local row = selected_row()
    if not row then state = nil return false end
    local value = node_text(Core.member(row, "Txt_Num"))
    -- Sold-out marker only when its node is actually shown on this row.
    local sold
    pcall(function()
        if Core.is_visible(row.Txt_Sold_Out) then sold = node_text(row.Txt_Sold_Out) end
    end)
    state = {
        title = node_text(Core.member(host, "Txt_Title")),
        name = node_text(Core.member(row, "Txt_List")),
        value = Core.phrase(value, sold),
    }
    return true
end

function ShopInfo.reset() ann:reset() end

function ShopInfo.update()
    local s = state
    if not s then return end
    -- title ("Buy") on entry; item + price on move; the live detail panel as tooltip.
    ann:focus(s.title, nil, s.name, s.value ~= "" and s.value or nil,
        function() return node_text(Core.member(host, "Txt_Detail")) end)
end

return ShopInfo
