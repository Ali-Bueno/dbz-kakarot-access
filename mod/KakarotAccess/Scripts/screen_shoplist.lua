-- Screen adapter: the Shop_Top_C top-level OPTION LIST (UAT_UIShopTop.ListBarCtn).
--
-- Distinct from screen_shop, which reads the shop's ITEM list (WidgetSkillList, a
-- MenuListBase with a reflected index). This handles the small mode/action list a
-- Shop_Top shows FIRST — e.g. the cooking entry "Make a dish" / "Make a full-course
-- meal" (verified via F7: host Shop_Top_C, rows Xlist_Bar01_00/01, label Txt_List), and
-- the generic shop "Buy / Sell" style menus.
--
-- Rows = ListBarCtn (TArray<UAT_UIXListBar01*>). Each row's label is Txt_List; the
-- selected row is the one showing its CursorWidget (UAT_UIXmenuCurs00), with the native
-- GetNotSelect() flag as a fallback discriminator. There's no reflected index on this
-- list, so the highlight IS the game's selection mechanism.
--
-- Multiple Shop_Top_C instances can exist, so we pick the on-screen one that actually
-- has visible ListBarCtn rows (first_on_screen locks onto whichever is live).

local Core = require("ui_core")
local A = require("ui_archetypes")

local ShopList = {}

-- DEBUG: per-entry dump of each row's label + GetState() selection signal to
-- dumps/dump_shoplist.txt, so a wrong highlight can be diagnosed. Selection via
-- GetState() confirmed in-game 2026-07-04 (cooking "Make a dish / full-course").
local DEBUG = false

local ann = Core.make_announcer()
local host = nil
local tick = 0
local last_sig = nil

local function clean(t) return t and A.markup_to_speech(t) or nil end

-- The on-screen rows of a host's ListBarCtn (raw array order = visual top-to-bottom).
-- Fallback: the blueprint's DIRECT row properties Xlist_Bar01_00..03 (UShop_Top_C,
-- Shop_Top.hpp) — the NPC-talk mode list ("Prepare a dish / Leave" on a second pooled
-- Shop_Top_C instance) showed rows the native ListBarCtn array did not carry
-- (user 2026-07-06), so read both sources.
local BAR_COUNT = 4   -- Xlist_Bar01_00..03 (UShop_Top_C, Shop_Top.hpp)
local function rows_of(h)
    local out = {}
    pcall(function()
        local arr = h.ListBarCtn
        for i = 1, #arr do
            local r = arr[i]
            if Core.valid(r) and Core.on_screen(r) then out[#out + 1] = r end
        end
    end)
    if #out == 0 then
        for i = 0, BAR_COUNT - 1 do
            local r
            pcall(function() r = h["Xlist_Bar01_" .. string.format("%02d", i)] end)
            if Core.valid(r) and Core.on_screen(r) then out[#out + 1] = r end
        end
    end
    return out
end

-- Selection = the row's own STATE (UAT_UIXListBar01.GetState() ->
-- UIXLISTBAR01_STATE_ID): the highlighted row reports a *_SELECT state, the others
-- UNSELECTED. CursorWidget visibility and GetNotSelect() do NOT distinguish rows here
-- (verified live: both mode rows read cursor=true, notSelect=false), so the state enum
-- is the reliable signal.
local SELECT_STATES = {
    [1] = true,   -- STATE_SELECT
    [6] = true,   -- STATE_SUBIN_SELECT
    [7] = true,   -- STATE_SUBOUT_SELECT
    [10] = true,  -- STATE_SELECT_NO_ENTER
}
local function row_state(r)
    local ok, s = pcall(function() return r:GetState() end)
    return ok and tonumber(s) or nil
end
local function row_selected(r)
    local s = row_state(r)
    return s ~= nil and SELECT_STATES[s] == true
end

local function row_label(r)
    local t
    pcall(function() t = clean(Core.read_text(r.Txt_List)) end)
    return t
end

-- Pick the on-screen Shop_Top_C that has visible ListBarCtn rows (the mode list is up).
local function acquire()
    for _, h in ipairs(Core.cached_all("Shop_Top_C", tick) or {}) do
        if Core.on_screen(h) and #rows_of(h) > 0 then return h end
    end
    return nil
end

local function dump(rows, sel_label)
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    local f = io.open(dir .. "\\dumps\\dump_shoplist.txt", "a")
    if not f then return end
    f:write(string.format("[%d] host=%s rows=%d sel=%s\n",
        os.time(), (host and host:GetFullName():match("%s(.+)$")) or "?", #rows, tostring(sel_label)))
    for i, r in ipairs(rows) do
        f:write(string.format("  [%d] %q state=%s selected=%s\n",
            i, tostring(row_label(r)), tostring(row_state(r)), tostring(row_selected(r))))
    end
    f:close()
end

local state = nil   -- { labels, sel } computed in is_active, reused by update

function ShopList.is_active()
    tick = tick + 1
    host = acquire()
    if not host then state = nil return false end
    local rows = rows_of(host)
    local labels, sel = {}, nil
    for _, r in ipairs(rows) do
        local label = row_label(r)
        if label then
            labels[#labels + 1] = label
            if row_selected(r) then sel = label end
        end
    end
    if #labels == 0 then state = nil return false end
    state = { labels = labels, sel = sel }
    if DEBUG then
        local sig = (sel or "?") .. "|" .. #labels
        if sig ~= last_sig then last_sig = sig; pcall(dump, rows, sel) end
    end
    return true
end

function ShopList.reset() ann:reset() last_sig = nil end

function ShopList.update()
    local s = state
    if not s then return end
    -- Choice-style: speak the highlighted option; re-announced as the cursor moves.
    ann:focus(nil, nil, s.sel or s.labels[1], nil, nil)
end

return ShopList
