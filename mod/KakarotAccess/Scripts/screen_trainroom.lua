-- Screen adapter: the Capsule Corp training room (the researcher next to the training door).
--
-- Her top menu (Shop_Top01_C: Modificar / Entrenar) is a UAT_UIShopTop and is read by
-- screen_shoplist. This file reads the two panes it opens, both owned by the field manager
-- (ui_directory: fm.ShopTraining00 / fm.ShopTraining01, AT.hpp:32913), never a scan:
--
--   Shop_Training00_C (UAT_UIShopTraining00) — remodel levels. List Xlist_List04_Lay9, selected
--     level Txt_Cap00, description Txt_Detail, requirements Txt_Cond + Start_Quest_Bar00List rows
--     (Txt_Progress name, Txt_Num "have/need").
--   Shop_Training01_C (UAT_UIShopTraining01) — training battles. List Xlist_List05_Lay7,
--     character tab Txt_Cap01 (LB/RB), selected battle Txt_Cap03, recommended level
--     Txt_Recommend + Txt_Num_Lv, goals Txt_Task_Detail00..02, enemies Txt_Name_E00..02 +
--     Txt_Lv_E00..02, rewards Txt_Cap00 + Xlist_Bar02_00/01 (Txt_List, Txt_Num), skill taught
--     Txt_Cap04 + Txt_Name_Skill + Txt_Detail.
--
-- Widget names are from the F7 census of 2026-10-21 (tester save, Android saga). Every text is
-- on_screen-gated: a battle with fewer enemies or rewards leaves the extra slots collapsed.
-- Notes: reference/dbz-kakarot/notes/dbz-kakarot-develop.md

local Core = require("ui_core")
local A = require("ui_archetypes")

local TrainRoom = {}

local ann = Core.make_announcer()
local host, kind = nil, nil
local tick = 0
local last_sig = nil
local cur_tab = nil

local function clean(t) return t and A.markup_to_speech(t) or nil end

local function shown_text(owner, name)
    local node = Core.member(owner, name)
    if Core.on_screen(node) then return clean(Core.read_text(node)) end
    return nil
end

local function push(parts, p) if p and p ~= "" then parts[#parts + 1] = p end end

-- "Zeni, 403640/4500, Medalla D, 89/5, ..." from the remodel requirement rows.
local function requirement_parts()
    local parts = {}
    local arr, n = Core.array_of(host, "Start_Quest_Bar00List")
    if arr then pcall(function()
        for i = 1, n do
            local r = arr[i]
            if Core.valid(r) and Core.on_screen(r) then
                push(parts, Core.phrase(shown_text(r, "Txt_Progress"), shown_text(r, "Txt_Num")))
            end
        end
    end) end
    if #parts > 0 then table.insert(parts, 1, shown_text(host, "Txt_Cond")) end
    return parts
end

local function remodel_detail()
    local parts = {}
    push(parts, shown_text(host, "Txt_Detail"))
    for _, p in ipairs(requirement_parts()) do push(parts, p) end
    return #parts > 0 and table.concat(parts, ", ") or nil
end

local function battle_detail()
    local parts = {}
    local rec, lv = shown_text(host, "Txt_Recommend"), shown_text(host, "Txt_Num_Lv")
    push(parts, rec and lv and (rec .. " " .. lv) or lv)
    for i = 0, 2 do push(parts, shown_text(host, string.format("Txt_Task_Detail%02d", i))) end
    for i = 0, 2 do
        local s = string.format("%02d", i)
        push(parts, Core.phrase(shown_text(host, "Txt_Name_E" .. s), shown_text(host, "Txt_Lv_E" .. s)))
    end
    local rewards = {}
    for i = 0, 1 do
        local bar = Core.member(host, string.format("Xlist_Bar02_%02d", i))
        if Core.on_screen(bar) then
            push(rewards, Core.phrase(shown_text(bar, "Txt_List"), shown_text(bar, "Txt_Num")))
        end
    end
    if #rewards > 0 then
        push(parts, shown_text(host, "Txt_Cap00"))
        for _, p in ipairs(rewards) do parts[#parts + 1] = p end
    end
    push(parts, Core.phrase(shown_text(host, "Txt_Cap04"), shown_text(host, "Txt_Name_Skill")))
    push(parts, shown_text(host, "Txt_Detail"))
    return #parts > 0 and table.concat(parts, ", ") or nil
end

local PANES = {
    remodel = { cls = "Shop_Training00_C", list = "Xlist_List04_Lay9", name = "Txt_Cap00",
                detail = remodel_detail },
    battle  = { cls = "Shop_Training01_C", list = "Xlist_List05_Lay7", name = "Txt_Cap03",
                tab = "Txt_Cap01", detail = battle_detail },
}
local ORDER = { "remodel", "battle" }

local function item_list() return Core.member(host, PANES[kind].list) end

local function list_title() return clean(Core.read_text(Core.member(item_list(), "Txt_Title"))) end

local function selected_name()
    local name = shown_text(host, PANES[kind].name)
    if name then return name end
    local row = A.list_selected_row(item_list())
    return row and clean(row.name) or nil
end

function TrainRoom.is_active()
    tick = tick + 1
    for _, k in ipairs(ORDER) do
        local h = Core.cached_live(PANES[k].cls, tick)
        if Core.on_screen(h) and Core.pane_live(h) then
            host, kind = h, k
            -- Claim only with something to say (CLAUDE.md §8: readable text, never a live handle).
            if selected_name() then return true end
        end
    end
    host, kind = nil, nil
    return false
end

function TrainRoom.reset() last_sig, cur_tab = nil, nil; ann:reset() end

function TrainRoom.update()
    if not host then return end
    local pane = PANES[kind]
    local name = selected_name()
    if not name then return end
    local list = item_list()
    -- The selection index and the requirement counts join the signature: two rows can share a
    -- name across character tabs, and a remodel changes what is left.
    local detail = pane.detail
    local sig = Core.phrase(kind, tostring(A.list_select_index(list)), name)
    if sig ~= last_sig then
        if last_sig ~= nil then ann:invalidate() end
        last_sig = sig
    end
    local tab = pane.tab and shown_text(host, pane.tab) or nil
    if tab ~= cur_tab then
        -- A character switch (LB/RB) swaps the whole list: read it like an entry.
        if cur_tab ~= nil then ann:reset() end
        cur_tab = tab
    end
    ann:focus(Core.phrase(list_title(), tab), nil, name, nil, detail)
end

return TrainRoom
