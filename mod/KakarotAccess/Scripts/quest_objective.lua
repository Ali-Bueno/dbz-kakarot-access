-- Quest objective HUD reader (Quest_Navi_C -> UAT_UIQuestNavigation).
--
-- The on-screen quest tracker shows the current objective(s) ("investigate the
-- house", "cook a meal", …). It is a HUD element, NOT a menu the player navigates,
-- so it is NOT in the menu Registry (which picks a single active adapter and would
-- let this shadow real menus). Instead it runs its own light poll loop, like
-- nav_tracker: it announces the objective whenever it CHANGES (diff-gated), so the
-- player hears each update, and F10 repeats the current objective on demand.
--
-- Structure (CXX dump): the host holds up to three main rows (Quest_Navi_M00..M02)
-- and three sub rows (Quest_Navi_S00..S02), each a Quest_Navi_M/S_C whose
-- Txt_List_00 is the objective line and Txt_List_01 an optional counter ("0/3").
-- Quest titles are in Txt_Title / Txt_Main00 / Txt_Sub00 (blueprint) with the
-- native WL_MainQuestListTitle / WL_SubQuestListTitle as fallbacks. Every read is
-- validity/visibility guarded; unreadable nodes are simply skipped.

local Core = require("ui_core")
local Speech = require("speech")
local I18n = require("i18n")
local Registry = require("ui_registry")
local Transition = require("transition")

local Quest = {}

local TICK_MS = 300          -- objectives change rarely; a slow loop keeps HUD reads cheap
local HOST_CLASS = "Quest_Navi_C"

-- Objective rows, in the on-screen order (main quest first, then sub quest).
local MAIN_ROWS = { "Quest_Navi_M00", "Quest_Navi_M01", "Quest_Navi_M02" }
local SUB_ROWS  = { "Quest_Navi_S00", "Quest_Navi_S01", "Quest_Navi_S02" }
-- Title candidates for each group, most specific first.
local MAIN_TITLE = { "Txt_Main00", "Txt_Title", "WL_MainQuestListTitle" }
local SUB_TITLE  = { "Txt_Sub00", "WL_SubQuestListTitle" }

local running = false
local last = nil             -- last announced objective string (diff gate)
local tick = 0              -- monotonic tick for Core's cache/back-off bookkeeping

-- First readable text among a host's candidate member names (blueprint members that
-- may be absent read as nil — safe).
local function first_text(host, names)
    for _, nm in ipairs(names) do
        local node
        if pcall(function() node = host[nm] end) and Core.valid(node) then
            local t = Core.read_text(node)
            if t then return t end
        end
    end
    return nil
end

-- One objective row's spoken line: the objective text plus its counter, or nil if the
-- row is absent/collapsed/blank.
local function row_line(host, member)
    local row
    if not pcall(function() row = host[member] end) or not Core.valid(row) then return nil end
    if not Core.on_screen(row) then return nil end
    local obj = Core.read_text(row.Txt_List_00)
    if not obj then return nil end
    return Core.phrase(obj, Core.read_text(row.Txt_List_01))
end

-- The whole current objective as one string (title + each visible objective line for
-- both quest groups), or nil when the tracker shows nothing.
local function objective_text()
    local host = Core.first_on_screen(HOST_CLASS, tick)
    if not host then return nil end
    local parts = {}
    local function group(title_names, rows)
        local lines = {}
        for _, m in ipairs(rows) do
            local l = row_line(host, m)
            if l then lines[#lines + 1] = l end
        end
        if #lines == 0 then return end
        local title = first_text(host, title_names)
        parts[#parts + 1] = Core.phrase(title, table.concat(lines, ", "))
    end
    group(MAIN_TITLE, MAIN_ROWS)
    group(SUB_TITLE, SUB_ROWS)
    if #parts == 0 then return nil end
    return table.concat(parts, ". ")
end

-- Poll step: announce the objective only when it changes, and only when no menu is
-- open and no level transition is in flight (never talk over a menu or a cutscene load).
local function step()
    if Transition.active() then return end
    if Registry.active_adapter() then return end
    -- This loop is independent of the menu Registry loop, so it must seed its own
    -- per-tick FindAllOf budget (Core.begin_scan_tick), or first_on_screen could find
    -- no budget left and never locate the HUD host.
    tick = tick + 1
    Core.begin_scan_tick()
    local text = objective_text()
    if text == last then return end
    last = text
    if text then Speech.say(text, true) end
end

function Quest.start()
    if running then return end
    running = true
    _G.__KakarotQuestGen = (_G.__KakarotQuestGen or 0) + 1
    local myGen = _G.__KakarotQuestGen
    local busy = false
    LoopAsync(TICK_MS, function()
        if _G.__KakarotQuestGen ~= myGen then return true end
        if not busy then
            busy = true
            ExecuteInGameThread(function()
                busy = false   -- cleared on entry (see ui_core.loop rationale)
                local ok, err = pcall(step)
                if not ok then print("[KakarotAccess] quest step error: " .. tostring(err) .. "\n") end
            end)
        end
        return false
    end)
end

function Quest.stop()
    running = false
    _G.__KakarotQuestGen = (_G.__KakarotQuestGen or 0) + 1
end

-- F10: speak the current objective on demand (interrupts). Runs on the game thread
-- (touches live UObjects) and stays inert during a level transition.
function Quest.read()
    ExecuteInGameThread(function()
        if Transition.active() then return end
        tick = tick + 1
        Core.begin_scan_tick()
        local text = objective_text()
        Speech.say(text or I18n.t("objective_none"), true)
    end)
end

return Quest
