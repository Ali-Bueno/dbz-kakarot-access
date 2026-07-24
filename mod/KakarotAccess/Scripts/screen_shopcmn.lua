-- Screen adapter: common buy/sell item list (Shop_Cmn_C -> UAT_UIShopCommon).
--
-- The food/material shop list: item rows with a price, an editable purchase count
-- (d-pad left/right) and the owned count. Verified from the F7 dump (2026-07-06,
-- "Tienda de comida"): rows are Xlist_Bar04_NN (UXlist_Bar04_C, Xlist_Bar04.hpp) with
-- Txt_List (name), Txt_Num_Price, Txt_Num (purchase count, arrows), Txt_Num_Have
-- (owned), Txt_Sold_Out, and the Ins_Cursor_Fad selected-row highlight. The detail
-- panel (host Txt_Detail + Txt_Stock) refreshes with the selection; Txt_Title = "Buy".
--
-- The Shop_Cmn_C BLUEPRINT class is NOT in the CXX header dump, so the rows are NOT
-- accessed as host properties (reading an undeclared property is the uncatchable C++
-- abort) — they're collected from the Xlist_Bar04_C pool by full-name prefix, like
-- screen_tutorial's text boxes. Row-level fields ARE declared (Xlist_Bar04.hpp) → safe.
-- UAT_UIShopCommon exposes no reflected selection index (header: widgets + GetShopItem
-- only), so Ins_Cursor_Fad is the selection signal.

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")

local ShopCmn = {}

local ann = Core.make_announcer()
local host = nil
local tick = 0
local rows = nil      -- cached row widgets of THIS host (prefix-matched pool entries)
local state = nil     -- { title, name, value } computed in is_active, reused by update

local function clean(t) return t and A.markup_to_speech(t) or nil end

local function node_text(node)
    local t
    pcall(function() t = clean(Core.read_text(node)) end)
    return t
end

-- Collect this host's Xlist_Bar04_C rows once (object-PATH prefix match — the full
-- name starts with the row's own class, so match on the path segment; see the
-- screen_tutorial needle lesson). Sorted by name so _00.._NN keeps screen order.
local function collect_rows()
    rows = {}
    if not Core.valid(host) then return end
    local prefix = host:GetFullName():match("%s(.+)$") or host:GetFullName()
    local named = {}
    for _, o in pairs(Core.cached_all("Xlist_Bar04_C", tick)) do
        if Core.valid(o) then
            local fn = o:GetFullName()
            if fn:find(prefix, 1, true) then named[#named + 1] = { fn = fn, o = o } end
        end
    end
    table.sort(named, function(a, b) return a.fn < b.fn end)
    for _, e in ipairs(named) do rows[#rows + 1] = e.o end
end

-- The selected row (Ins_Cursor_Fad highlight). STRICT — no first-row fallback: a
-- pooled list that lingers after closing must not keep this adapter active and shadow
-- the screens registered below it (the ring pause).
local function selected_row()
    for _, row in ipairs(rows or {}) do
        if Core.valid(row) and Core.on_screen(row) and node_text(Core.member(row, "Txt_List")) then
            local ok, sel = pcall(function() return Core.is_visible(row.Ins_Cursor_Fad) end)
            if ok and sel then return row end
        end
    end
    return nil
end

function ShopCmn.is_active()
    tick = tick + 1
    host = Core.first_on_screen("Shop_Cmn_C", tick)   -- pooled; the live instance may vary
    if not host then rows, state = nil, nil return false end
    if not rows or #rows == 0 then collect_rows() end
    local row = selected_row()
    if not row then state = nil return false end
    local sold
    pcall(function()
        if Core.is_visible(row.Txt_Sold_Out) then sold = node_text(row.Txt_Sold_Out) end
    end)
    local qty = node_text(Core.member(row, "Txt_Num"))
    local have = node_text(Core.member(row, "Txt_Num_Have"))
    state = {
        -- WL_* = the native BindWidget names (UAT_UIShopCommon, AT.hpp) — the blueprint
        -- class isn't in the header dump, so only native-declared properties are safe.
        title = node_text(Core.member(host, "WL_Txt_Title")),
        name = node_text(Core.member(row, "Txt_List")),
        value = Core.phrase(
            node_text(row.Txt_Num_Price),
            qty and string.format(I18n.t("shop_buy_fmt"), qty) or nil,
            have and string.format(I18n.t("shop_have_fmt"), have) or nil,
            sold),
    }
    return true
end

function ShopCmn.reset() ann:reset() rows = nil end

function ShopCmn.update()
    local s = state
    if not s then return end
    -- title ("Buy"/"Sell") on entry; item on move; the count/price as the value (so a
    -- d-pad quantity change re-speaks just the value); detail + stock as tooltip.
    ann:focus(s.title, nil, s.name, s.value ~= "" and s.value or nil,
        function()
            return Core.phrase(node_text(Core.member(host, "WL_Txt_Detail")), node_text(Core.member(host, "WL_Txt_Stock")))
        end)
end

return ShopCmn
