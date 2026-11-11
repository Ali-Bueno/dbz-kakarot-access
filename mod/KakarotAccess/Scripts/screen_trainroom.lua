-- Screen adapter: the training panes — the Capsule Corp training room (the researcher next to the
-- training door) and the super trainings (Whis and the other DLC trainers).
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
--   Shop_Training02..05_C (UAT_UIShopTraining02..05, AT.hpp:35996-36132) — the super training
--     layouts (fm.ShopTraining02..05). Same list; their texts are native members, read by name.
--
-- Widget names for 00/01 are from the F7 census of 2026-10-21 (tester save, Android saga); 02..05
-- are from the header only, unplayed. Every text is on_screen-gated: a battle with fewer enemies
-- or rewards leaves the extra slots collapsed.
-- Notes: reference/dbz-kakarot/notes/dbz-kakarot-develop.md, dbz-kakarot-super-training.md

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

-- "Derrota a todos los enemigos, 0/1, Megamasa, 3" from one challenge row (UAT_UIXlistBar06).
local function challenge_parts(spec)
    local parts = {}
    local lay = Core.member(host, spec.list)
    if not Core.on_screen(lay) then return parts end
    for i = 0, spec.rows - 1 do
        local row = Core.member(lay, string.format("Xlist_Bar06_%02d", i))
        if Core.on_screen(row) then
            push(parts, Core.phrase(shown_text(row, "Txt_Challenge"), shown_text(row, "Txt_Progress"),
                shown_text(row, "Txt_Item"), shown_text(row, "Txt_Num")))
        end
    end
    if #parts > 0 then table.insert(parts, 1, shown_text(host, spec.cap)) end
    return parts
end

-- The detail of a battle pane, from its layout: which of the shared blocks the class declares.
-- Every member named through a layout is declared by that pane's class, so nothing is probed.
local function battle_detail(lay)
    local parts = {}
    local rec, lv = shown_text(host, "Txt_Recommend"), shown_text(host, "Txt_Num_Lv")
    push(parts, rec and lv and (rec .. " " .. lv) or lv)
    for i = 0, lay.tasks - 1 do push(parts, shown_text(host, string.format("Txt_Task_Detail%02d", i))) end
    for i = 0, lay.enemies - 1 do
        local s = string.format("%02d", i)
        push(parts, Core.phrase(shown_text(host, "Txt_Name_E" .. s), shown_text(host, "Txt_Lv_E" .. s)))
    end
    if lay.score then
        push(parts, shown_text(host, "Txt_Title"))
        push(parts, Core.phrase(shown_text(host, "Txt_Best"), shown_text(host, "Txt_Score")))
    end
    if lay.rewards then
        local rewards = {}
        for i = 0, lay.rewards - 1 do
            local bar = Core.member(host, string.format("Xlist_Bar02_%02d", i))
            if Core.on_screen(bar) then
                push(rewards, Core.phrase(shown_text(bar, "Txt_List"), shown_text(bar, "Txt_Num")))
            end
        end
        if #rewards > 0 then
            push(parts, shown_text(host, "Txt_Cap00"))
            for _, p in ipairs(rewards) do parts[#parts + 1] = p end
        end
    end
    if lay.skill then
        push(parts, Core.phrase(shown_text(host, lay.skill[1]),
            lay.skill[2] and shown_text(host, lay.skill[2]) or nil))
    end
    for _, spec in ipairs(lay.challenges or {}) do
        for _, p in ipairs(challenge_parts(spec)) do parts[#parts + 1] = p end
    end
    if lay.text then push(parts, shown_text(host, "Txt_Detail")) end
    return #parts > 0 and table.concat(parts, ", ") or nil
end

-- The two challenge lists the score layouts share (UAT_UIXlist_Bar06_Lay10 / _Lay3).
local CHALLENGES = {
    { cap = "Txt_Cap01", list = "Xlist_Bar06_Lay10", rows = 10 },
    { cap = "Txt_Cap02", list = "Xlist_Bar06_Lay3", rows = 3 },
}

local LIST = "Xlist_List05_Lay7"
local PANES = {
    remodel = { cls = "Shop_Training00_C", list = "Xlist_List04_Lay9", name = "Txt_Cap00",
                detail = remodel_detail },
    battle  = { cls = "Shop_Training01_C", list = LIST, name = "Txt_Cap03", tab = "Txt_Cap01",
                detail = battle_detail, tasks = 3, enemies = 3, rewards = 2,
                skill = { "Txt_Cap04", "Txt_Name_Skill" }, text = true },
    super02 = { cls = "Shop_Training02_C", list = LIST, name = "Txt_Cap03",
                detail = battle_detail, tasks = 1, enemies = 3, rewards = 5, text = true },
    super03 = { cls = "Shop_Training03_C", list = LIST, name = "Txt_Cap00",
                detail = battle_detail, tasks = 3, enemies = 1, score = true,
                challenges = CHALLENGES },
    super04 = { cls = "Shop_Training04_C", list = LIST, name = "Txt_Cap03",
                detail = battle_detail, tasks = 3, enemies = 3, rewards = 5,
                skill = { "Txt_Skill00" }, text = true },
    super05 = { cls = "Shop_Training05_C", list = LIST, name = "Txt_Cap00",
                detail = battle_detail, tasks = 3, enemies = 1, challenges = CHALLENGES },
}
local ORDER = { "remodel", "battle", "super02", "super03", "super04", "super05" }

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
    local detail = function() return pane.detail(pane) end
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
