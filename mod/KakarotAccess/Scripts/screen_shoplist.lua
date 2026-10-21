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
local header = nil  -- spoken on entry; only the training room's top (Shop_Top01_C) has one
local tick = 0
local last_sig = nil

local function clean(t) return t and A.markup_to_speech(t) or nil end

-- The on-screen rows (ListBarCtn, else the blueprint's direct Xlist_Bar01_00..03 —
-- the NPC-talk instance only shows rows through the latter, user 2026-07-06). Shared
-- helper: screen_cooking uses it too, to yield while the entry menu is up.
local function rows_of(h) return A.shoptop_rows(h) end

-- Selection = the row's own STATE (UAT_UIXListBar01.GetState() ->
-- UIXLISTBAR01_STATE_ID): the highlighted row reports a *_SELECT state, the others
-- UNSELECTED. CursorWidget visibility and GetNotSelect() do NOT distinguish rows here
-- (verified live: both mode rows read cursor=true, notSelect=false), so the state enum
-- is the reliable signal.
-- The ENTER states are the row just confirmed: it stays the selection while the list animates out
-- (measured 2026-10-21: "Entrenar" goes 1 -> 2 on A, every other row stays 0).
local SELECT_STATES = {
    [1] = true,   -- STATE_SELECT
    [2] = true,   -- STATE_ENTER
    [6] = true,   -- STATE_SUBIN_SELECT
    [7] = true,   -- STATE_SUBOUT_SELECT
    [8] = true,   -- STATE_SUBENTER
    [10] = true,  -- STATE_SELECT_NO_ENTER
    [12] = true,  -- STATE_END_ENTER
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
    return clean(Core.read_text(Core.member(r, "Txt_List")))
end

-- Pick the on-screen Shop_Top_C that has visible ListBarCtn rows (the mode list is up).
-- The Capsule Corp training room's researcher uses its own subclass, Shop_Top01_C
-- (fm.ShopTop01: Modificar / Entrenar), with a header the plain shop top does not declare:
-- Txt_List00 "Sala de entrenamiento", Txt_Cap01 "Nivel de instalación", Txt_Num_Lv "Nvl. 1"
-- (census 2026-10-21). Returns the host and that header, read only on a Shop_Top01_C.
local function acquire()
    for _, h in ipairs(Core.cached_all("Shop_Top_C", tick) or {}) do
        if Core.on_screen(h) and #rows_of(h) > 0 then return h, nil end
    end
    for _, h in ipairs(Core.cached_all("Shop_Top01_C", tick) or {}) do
        if Core.on_screen(h) and #rows_of(h) > 0 then
            local function txt(n) return clean(Core.read_text(Core.member(h, n))) end
            return h, Core.phrase(txt("Txt_List00"), txt("Txt_Cap01"), txt("Txt_Num_Lv"))
        end
    end
    return nil, nil
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
    -- Free-roam cross-check (same as screen_cooking): the cook-NPC's embedded
    -- Shop_Top_C can keep its rows visible after the flow closes; minimap up = closed.
    if Core.free_roam(tick) then state = nil return false end
    -- And yield to the pause RING family (all registered below — see screen_cooking).
    if Core.ring_open(tick) then state = nil return false end
    host, header = acquire()
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

-- While the list animates in or out, no row reports a select state for a tick or two, and the
-- first-row fallback below then spoke the WRONG option (training room 2026-10-21: "Modificar"
-- between pressing A on "Entrenar" and its pane opening, and again on coming back, when every row
-- reads STATE_UNSELECTED for a few polls). The fallback now waits NOSEL_TICKS polls for a real
-- selection; it only exists for a list whose rows never report one.
local NOSEL_TICKS = 8
local nosel = 0

function ShopList.reset() ann:reset() last_sig = nil nosel = 0 end

function ShopList.update()
    local s = state
    if not s then return end
    if s.sel then nosel = 0 else nosel = nosel + 1 end
    if not s.sel and nosel < NOSEL_TICKS then return end
    -- Choice-style: speak the highlighted option; re-announced as the cursor moves.
    ann:focus(header ~= "" and header or nil, nil, s.sel or s.labels[1], nil, nil)
end

return ShopList
