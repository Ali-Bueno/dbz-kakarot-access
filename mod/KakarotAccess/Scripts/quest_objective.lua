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
local A = require("ui_archetypes")

local Quest = {}

-- Objective text carries CFramework markup (<span color=…>fogata</>, <inputicon…>) —
-- read it THROUGH markup_to_speech so the tags/glyphs resolve to speech instead of
-- being spoken literally ("Cocina un pez en una <span color=...>fogata</>", user
-- 2026-07-16). Same sink the dialogue/menu readers use.
local function read_clean(node)
    if not Core.valid(node) then return nil end
    return A.markup_to_speech(Core.read_text(node))
end

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

-- "Objective advanced" signal for the radar (wired in app.lua -> Nav). Each quest
-- group keeps a SIGNATURE — title + objective lines WITHOUT the counters — and the
-- callback fires when a non-nil signature differs from the last non-nil one:
--  * counters ("2/5") tick without moving the marker, so they are excluded;
--  * the HUD hiding in battle/menus reads nil and must NOT look like a change
--    (stored signatures survive nil reads), so the same objective re-appearing
--    after a fight stays silent here;
--  * the very first observation of a session only seeds (nothing "changed" yet).
-- The kind ("main"/"sub") tells the radar which quest class advanced, so it can
-- prefer that marker when both a main and a sub arrow are active.
local last_sig = { main = nil, sub = nil }
local sig_seeded = false
local on_change = nil        -- fn(kind) set via Quest.set_on_change

-- First readable text among a host's candidate member names (blueprint members that
-- may be absent read as nil — safe).
local function first_text(host, names)
    for _, nm in ipairs(names) do
        local node
        if pcall(function() node = host[nm] end) and Core.valid(node) then
            local t = read_clean(node)
            if t then return t end
        end
    end
    return nil
end

-- One objective row's spoken line: the objective text plus its counter, or nil if the
-- row is absent/collapsed/blank. Second return: the bare objective text (no counter),
-- the row's contribution to the change signature.
local function row_line(host, member)
    local row
    if not pcall(function() row = host[member] end) or not Core.valid(row) then return nil end
    if not Core.on_screen(row) then return nil end
    local obj = read_clean(row.Txt_List_00)
    if not obj then return nil end
    return Core.phrase(obj, read_clean(row.Txt_List_01)), obj
end

-- The whole current objective as one string (title + each visible objective line for
-- both quest groups), or nil when the tracker shows nothing.
--
-- IMPORTANT (F10 diagnostic, 2026-07-15): for a single-objective quest the game shows
-- the objective IN THE TITLE NODE (Txt_Main00 / WL_MainQuestListTitle) and keeps every
-- M/S row hidden (valid, on_screen=false, empty) — the rows are the sub-task checklist
-- ("0/3" lines) and only populate for multi-step objectives. So a group must speak
-- when EITHER the title or the rows carry text; requiring rows muted the whole reader.
-- Returns the spoken text, plus the main/sub change SIGNATURES (see last_sig above).
local function objective_text()
    local host = Core.first_on_screen(HOST_CLASS, tick)
    if not host then return nil end
    local parts, sigs = {}, {}
    local function group(gkey, title_names, rows)
        local lines, sig = {}, {}
        for _, m in ipairs(rows) do
            local l, raw = row_line(host, m)
            if l then lines[#lines + 1] = l end
            if raw then sig[#sig + 1] = raw end
        end
        local title = first_text(host, title_names)
        if not title and #lines == 0 then return end
        parts[#parts + 1] = Core.phrase(title,
            #lines > 0 and table.concat(lines, ", ") or nil)
        sigs[gkey] = (title or "") .. "|" .. table.concat(sig, "|")
    end
    group("main", MAIN_TITLE, MAIN_ROWS)
    group("sub", SUB_TITLE, SUB_ROWS)
    if #parts == 0 then return nil end
    return table.concat(parts, ". "), sigs.main, sigs.sub
end

-- Diff one group's signature against its last non-nil value; returns the group key
-- when it genuinely changed (see the last_sig comment for the nil/seed rules).
local function sig_changed(gkey, sig, was_seeded)
    if not sig then return nil end
    local prev = last_sig[gkey]
    last_sig[gkey] = sig
    if sig ~= prev and (prev ~= nil or was_seeded) then return gkey end
    return nil
end

local function signal_check(sig_main, sig_sub)
    local was = sig_seeded
    if sig_main or sig_sub then sig_seeded = true end
    -- Evaluate BOTH groups (no short-circuit): each must update its stored signature
    -- every pass. Main outranks sub when both changed in the same pass.
    local cm = sig_changed("main", sig_main, was)
    local cs = sig_changed("sub", sig_sub, was)
    local kind = cm or cs
    if kind and on_change then pcall(on_change, kind) end
end

-- Poll step: announce the objective only when it changes, and only when no menu is
-- open and no level transition is in flight (never talk over a menu or a cutscene load).
local function step()
    if Transition.active() then return end
    if Registry.active_adapter() then return end
    -- Don't cut a PROTECTED line (a reward notice / tutorial instruction still
    -- playing): the objective would interrupt "Emblemas de alma recibidos…" a few
    -- seconds in (user 2026-07-16). Deferring keeps `last` stale, so it re-announces
    -- once the protected line finishes (diff gate still fires).
    if Speech.protected() then return end
    -- This loop is independent of the menu Registry loop, so it must seed its own
    -- per-tick FindAllOf budget (Core.begin_scan_tick), or first_on_screen could find
    -- no budget left and never locate the HUD host.
    tick = tick + 1
    Core.begin_scan_tick()
    local text, sig_main, sig_sub = objective_text()
    signal_check(sig_main, sig_sub)
    if text == last then return end
    last = text
    if text then Speech.say(text, true) end
end

-- Radar wiring (app.lua): fn(kind) fires once per genuine objective change,
-- kind = "main" | "sub". Runs on the game thread, inside this loop's pcall.
function Quest.set_on_change(fn)
    on_change = fn
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
                local t0 = os.clock()
                local ok, err = pcall(step)
                if not ok then print("[KakarotAccess] quest step error: " .. tostring(err) .. "\n") end
                -- Cost telemetry (own loop, outside the registry step — printed by
                -- the Ctrl+F5 dump; the last unmeasured game-thread work).
                local dt = (os.clock() - t0) * 1000
                local st = _G.__KakarotQuestStats
                if not st then st = { n = 0, ms = 0, max = 0 } _G.__KakarotQuestStats = st end
                st.n = st.n + 1
                st.ms = st.ms + dt
                if dt > st.max then st.max = dt end
            end)
        end
        return false
    end)
end

function Quest.stop()
    running = false
    _G.__KakarotQuestGen = (_G.__KakarotQuestGen or 0) + 1
end

-- Diagnostic dump (dumps/dump_quest.txt), appended on every F10 press while DUMP is
-- on. It solved the 2026-07-15 silence (single-objective quests put the text in the
-- TITLE node, rows hidden); OFF since the reader was verified in-game that night.
local DUMP = false
local function dump_path()
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    return dir .. "\\dumps\\dump_quest.txt"
end

local function dump_state(text)
    local f = io.open(dump_path(), "a")
    if not f then return end
    f:write(string.format("== F10 @ %s text=%s\n", os.date("%H:%M:%S"), tostring(text)))
    f:write(string.format("  active_adapter=%s transition=%s\n",
        tostring(Registry.active_adapter()), tostring(Transition.active())))
    local host = Core.first_on_screen(HOST_CLASS, tick)
    if not host then
        f:write("  host: first_on_screen = nil\n")
        f:close()
        return
    end
    local fn = "?"
    pcall(function() fn = host:GetFullName() end)
    f:write("  host = " .. fn .. "\n")
    for _, rows in ipairs({ MAIN_ROWS, SUB_ROWS }) do
        for _, m in ipairs(rows) do
            local row
            local okr = pcall(function() row = host[m] end)
            local valid = okr and Core.valid(row) or false
            local on = valid and Core.on_screen(row) or false
            local t0, t1
            if valid then
                pcall(function() t0 = Core.read_text(row.Txt_List_00) end)
                pcall(function() t1 = Core.read_text(row.Txt_List_01) end)
            end
            f:write(string.format("  %s valid=%s on_screen=%s txt0=%s txt1=%s\n",
                m, tostring(valid), tostring(on), tostring(t0), tostring(t1)))
        end
    end
    for _, nm in ipairs({ "Txt_Main00", "Txt_Sub00", "Txt_Title", "Txt_Navi_Detail",
                          "WL_MainQuestListTitle", "WL_SubQuestListTitle" }) do
        local node, t
        pcall(function() node = host[nm] end)
        if Core.valid(node) then pcall(function() t = Core.read_text(node) end) end
        f:write(string.format("  title %s = %s\n", nm, tostring(t)))
    end
    f:close()
end

-- F10: speak the current objective on demand (interrupts). Runs on the game thread
-- (touches live UObjects) and stays inert during a level transition.
function Quest.read()
    ExecuteInGameThread(function()
        if Transition.active() then return end
        tick = tick + 1
        Core.begin_scan_tick()
        local text = objective_text()
        if DUMP then pcall(dump_state, text) end
        Speech.say(text or I18n.t("objective_none"), true)
    end)
end

return Quest
