-- Screen adapter: the shared QUANTITY window (Xcmn_Win02_C -> UAT_UIXcmnWin02, the window
-- manager's NumberWindowCore, AT.hpp:37737) while it is asking for a number.
--
-- screen_dialog reads the same window class as a notice (caption + old/new numbers) and lets go
-- on the next poll, which is right for the brown level-up box but left the quantity prompt
-- silent: "arriba y abajo: Cambiar cantidad" changed a number nobody heard. First seen on Bulma's
-- "Subes de nivel" (feeding materials to a machine, census 2026-10-21):
--   caption   Txt_Detail_Item  "Subes de nivel";  level  Txt_Num_Lv00 "1 → " + Txt_Num_Lv01 "1"
--   input     UIListItem_Input  (Xcmn_Win02_List00_C: Txt_Item_List "Mineral de hierro",
--                                Txt_Num_Mole "2" of Txt_Num_Denomi "86")
--   relations Relation_List rows (Txt_List "EXP. ganada" / "Siguiente nivel:", Txt_Num_Denomi)
-- It claims only while the input row is on screen with a number in it, so every other use of the
-- window still goes to screen_dialog. Registered BEFORE screen_dialog.

local Core = require("ui_core")
local A = require("ui_archetypes")
local Mem = require("mem")

local NumberWin = {}

local ann = Core.make_announcer()
local win, input = nil, nil
local tick = 0

local function clean(t) return t and A.markup_to_speech(t) or nil end

local function shown_text(owner, name)
    local node = Core.member(owner, name)
    if Core.on_screen(node) then return clean(Core.read_text(node)) end
    return nil
end

-- "Siguiente nivel: 100" rather than "Siguiente nivel:, 100": a label that already ends in a colon
-- takes its figure after a space.
local function labelled(label, num)
    if label and num and label:find(":%s*$") then return label:gsub("%s+$", "") .. " " .. num end
    return Core.phrase(label, num)
end

-- "2/86", or the bare figure when the row shows only one side.
local function fraction(row)
    local mole, denomi = shown_text(row, "Txt_Num_Mole"), shown_text(row, "Txt_Num_Denomi")
    if mole and denomi then return mole .. "/" .. denomi end
    return mole or denomi
end

-- Every numbered row under the input, top to bottom ("EXP. ganada, 80", "Siguiente nivel:, 100").
-- Relation_List holds only SOME of them (live: just the EXP row), so the window's own vertical box
-- is walked instead. Its children are mixed — number rows, the hint row (Xcmn_Win02_List01_C),
-- spacers, choices — so every member is fetched STRICT (a spacer declares none of these names),
-- and a row counts only when it carries a number, which also leaves the static hint out.
local function relation_parts()
    local parts = {}
    local box = Core.member(win, "VBox_Container")
    if not Core.valid(box) then return parts end
    local n = 0
    pcall(function() n = box:GetChildrenCount() end)
    local input_addr = Mem.raw_addr(input)
    for i = 0, (tonumber(n) or 0) - 1 do
        local r
        pcall(function() r = box:GetChildAt(i) end)
        if Core.valid(r) and Core.on_screen(r) and Mem.raw_addr(r) ~= input_addr then
            local function txt(name)
                local node = Core.member(r, name, true)
                if Core.on_screen(node) then return clean(Core.read_text(node)) end
                return nil
            end
            local mole, denomi = txt("Txt_Num_Mole"), txt("Txt_Num_Denomi")
            local num = (mole and denomi) and (mole .. "/" .. denomi) or mole or denomi
            if num then parts[#parts + 1] = labelled(txt("Txt_List"), num) end
        end
    end
    return parts
end

function NumberWin.is_active()
    tick = tick + 1
    win = Core.cached_live("Xcmn_Win02_C", tick)
    if not (Core.on_screen(win) and Core.pane_rendered(win)) then return false end
    input = Core.member(win, "UIListItem_Input")
    if not Core.on_screen(input) then return false end
    return shown_text(input, "Txt_Num_Mole") ~= nil
end

function NumberWin.reset() ann:reset() end

function NumberWin.update()
    local parts = {}
    local q = fraction(input)
    if q then parts[#parts + 1] = q end
    local old, new = shown_text(win, "Txt_Num_Lv00"), shown_text(win, "Txt_Num_Lv01")
    -- "1 → " + "1": the arrow's trailing space does not survive the text read.
    if old or new then parts[#parts + 1] = Core.phrase(old, new):gsub(",%s*", " ") end
    for _, p in ipairs(relation_parts()) do parts[#parts + 1] = p end
    -- The quantity and everything it moves is the VALUE: a change speaks only that.
    ann:focus(shown_text(win, "Txt_Detail_Item"), nil, shown_text(input, "Txt_Item_List"),
        table.concat(parts, ", "), nil)
end

return NumberWin
