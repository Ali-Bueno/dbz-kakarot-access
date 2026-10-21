-- Screen adapter: Bulma's I+D build menu (Shop_Develop_Dev_C -> UAT_UIShopDevelop) and its
-- result sheet (Shop_Develop_Result_C -> UAT_UIShopDevelopResult).
--
-- Both hosts hang off the GameMode's MechaDevManager (ui_directory), never a scan. Widget names
-- are from the F7 census of 2026-10-18: tabs = the list title (Xlist_List_00_Lay9.Txt_Title),
-- selection = the detail title Txt_Title00, stats = UIDevParam_List rows (Txt_List + whichever of
-- Txt_Num_Param_L / Txt_Status_00_01 the tab shows), materials = UIDevMaterial_List rows
-- (Txt_List + Txt_Num "have/need"). Rows left over from a longer recipe stay valid but
-- collapsed, so every row is on_screen-gated. Notes: reference/dbz-kakarot/notes/dbz-kakarot-develop.md
--
-- Personalización and Subes de nivel are ONE host, Shop_Develop_Custom_C (UAT_UIShopDevelopCustom,
-- MenuManager.UIShopDevelopCustom), census 2026-10-21: machine Txt_Titl00 + Txt_Lv, next level
-- Txt_Titl01 + Txt_Num01, memory Txt_Memory + Txt_Num_Memory, stats UIDevParam_List. Subes de
-- nivel shows the materials list Xlist_List01_Lay9_00 (a MenuListBase01: title "Materiales", rows
-- name + owned count); Personalización shows the chip slots Xlist_Custom00_Lay10 (Item_List rows,
-- label Txt_List00) and the colour (Shop_Custom_Color: Txt_Cap00 "Color:" + Txt_Color). The quantity
-- prompt that feeding a material opens is screen_numberwin's.

local Core = require("ui_core")
local A = require("ui_archetypes")

local Develop = {}

local ann = Core.make_announcer()       -- the build list
local res_ann = Core.make_announcer()   -- the result sheet (its own focus, so closing it is silent)
local host, result, custom = nil, nil, nil
local cust_ann = Core.make_announcer()  -- Personalización / Subes de nivel
local tick = 0
local last_sig = nil
local cur_tab = nil

local function clean(t) return t and A.markup_to_speech(t) or nil end

local function shown_text(owner, name)
    local node = Core.member(owner, name)
    if Core.on_screen(node) then return clean(Core.read_text(node)) end
    return nil
end

-- On-screen rows of a native TArray member, top to bottom where the layout says so.
local function shown_rows(owner, arr_name)
    local rows = {}
    local arr, n = Core.array_of(owner, arr_name)
    if not arr then return rows end
    pcall(function()
        for i = 1, n do
            local r = arr[i]
            if Core.valid(r) and Core.on_screen(r) then
                rows[#rows + 1] = { w = r, i = i, y = Core.slot_pos(r, "Y") }
            end
        end
    end)
    table.sort(rows, function(a, b)
        if a.y and b.y and a.y ~= b.y then return a.y < b.y end
        return a.i < b.i
    end)
    return rows
end

-- "Batería, 50" per stat the item changes; zero stats are skipped (a part moves one or two).
local function stat_parts(owner)
    local parts = {}
    for _, r in ipairs(shown_rows(owner, "UIDevParam_List")) do
        local v = shown_text(r.w, "Txt_Num_Param_L") or shown_text(r.w, "Txt_Status_00_01")
        if v and v ~= "0" then
            local p = Core.phrase(shown_text(r.w, "Txt_List"), v)
            if p ~= "" then parts[#parts + 1] = p end
        end
    end
    return parts
end

-- "Materiales necesarios, Tienes / Necesitas, Zeni, 2500, Hierro, 10/1".
local function material_parts()
    local parts = {}
    for _, r in ipairs(shown_rows(host, "UIDevMaterial_List")) do
        local p = Core.phrase(shown_text(r.w, "Txt_List"), shown_text(r.w, "Txt_Num"))
        if p ~= "" then parts[#parts + 1] = p end
    end
    if #parts == 0 then return parts end
    table.insert(parts, 1, Core.phrase(shown_text(host, "Txt_Titl01"), shown_text(host, "Txt_Titl01_02")))
    return parts
end

local function detail()
    local parts = stat_parts(host)
    for _, p in ipairs(material_parts()) do parts[#parts + 1] = p end
    if #parts == 0 then return nil end
    return table.concat(parts, ", ")
end

local function item_list() return Core.member(host, "Xlist_List_00_Lay9") end

local function list_title()
    return clean(Core.read_text(Core.member(item_list(), "Txt_Title")))
end

local function result_live()
    result = Core.cached_live("Shop_Develop_Result_C", tick)
    return Core.on_screen(result) and Core.pane_rendered(result)
        and shown_text(result, "Txt_Detail00") ~= nil
end

-- "Siguiente nivel: 100": a label ending in a colon takes its figure after a space.
local function labelled(label, num)
    if label and num and label:find(":%s*$") then return label:gsub("%s+$", "") .. " " .. num end
    return Core.phrase(label, num)
end

local function custom_live()
    custom = Core.cached_live("Shop_Develop_Custom_C", tick)
    return Core.on_screen(custom) and Core.pane_live(custom)
        and shown_text(custom, "Txt_Titl00") ~= nil
end

-- Machine, level, next level and memory: spoken on entry.
local function custom_header()
    return Core.phrase(shown_text(custom, "Txt_Titl00"), shown_text(custom, "Txt_Lv"),
        labelled(shown_text(custom, "Txt_Titl01"), shown_text(custom, "Txt_Num01")),
        Core.phrase(shown_text(custom, "Txt_Memory"), shown_text(custom, "Txt_Num_Memory")))
end

-- The chip slot under the cursor: the row whose Border_Cursor is on screen, else the only/first
-- labelled row (the test save has a single slot, so the marker is unverified with several).
local function selected_slot()
    local list = Core.member(custom, "Xlist_Custom00_Lay10")
    local arr, n = Core.array_of(list, "Item_List")
    local first, marked = nil, nil
    if arr then pcall(function()
        for i = 1, n do
            local r = arr[i]
            if Core.valid(r) and Core.on_screen(r) then
                local label = shown_text(r, "Txt_List00")
                if label then
                    first = first or label
                    if not marked and Core.on_screen(Core.member(r, "Border_Cursor")) then marked = label end
                end
            end
        end
    end) end
    return marked or first
end

local function custom_update()
    local stats = function()
        local parts = stat_parts(custom)
        return #parts > 0 and table.concat(parts, ", ") or nil
    end
    -- The mode is whichever sub-list is interactive: leaving Subes de nivel parks its list
    -- HitTestInvisible but still on_screen (measured live), and it would shadow Personalización.
    local mats = Core.member(custom, "Xlist_List01_Lay9_00")
    if Core.on_screen(mats) and Core.pane_live(mats) then
        -- Subes de nivel: the materials list, "Mineral de hierro, 86".
        local row = A.list_selected_row(mats)
        local title = clean(Core.read_text(Core.member(mats, "Txt_Title")))
        if not row or not row.name then return end
        cust_ann:focus(Core.phrase(custom_header(), title), nil, clean(row.name), clean(row.num), nil)
        return
    end
    -- Personalización: the slot, with the colour as its value so a colour change speaks alone.
    local color = Core.member(custom, "Shop_Custom_Color")
    local cval = Core.on_screen(color)
        and labelled(shown_text(color, "Txt_Cap00"), shown_text(color, "Txt_Color")) or nil
    cust_ann:focus(custom_header(), nil, selected_slot(), cval ~= "" and cval or nil, stats)
end

function Develop.is_active()
    tick = tick + 1
    if result_live() then return true end
    if custom_live() then return true end
    host = Core.cached_live("Shop_Develop_Dev_C", tick)
    if not (Core.on_screen(host) and Core.pane_live(host)) then return false end
    return shown_text(host, "Txt_Title00") ~= nil or list_title() ~= nil
end

function Develop.reset() last_sig, cur_tab = nil, nil; ann:reset(); res_ann:reset(); cust_ann:reset() end

function Develop.update()
    if result_live() then
        res_ann:focus(shown_text(result, "Txt_Titl00"), nil, shown_text(result, "Txt_Detail00"), nil,
            function()
                local parts = stat_parts(result)
                return #parts > 0 and table.concat(parts, ", ") or nil
            end)
        return
    end
    res_ann:reset()
    if custom_live() then custom_update() return end
    cust_ann:reset()
    local list = item_list()
    local row = A.list_selected_row(list)
    local name = shown_text(host, "Txt_Title00") or (row and clean(row.name))
    if not name then return end
    -- A build changes the material counts but not the name, so they join the signature: the
    -- item is re-read with what is left.
    local sig = Core.phrase(tostring(A.list_select_index(list)), name, table.concat(material_parts(), "|"))
    if sig ~= last_sig then
        if last_sig ~= nil then ann:invalidate() end
        last_sig = sig
    end
    -- A tab (LB/RB) swaps the whole list, so it is read like an entry: tab, item AND its detail.
    local title = list_title()
    if title and title ~= cur_tab then
        if cur_tab ~= nil then ann:reset() end
        cur_tab = title
    end
    ann:focus(cur_tab, nil, name, nil, detail)
end

return Develop
