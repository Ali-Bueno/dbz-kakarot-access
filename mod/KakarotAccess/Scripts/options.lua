-- Options menu reader (Start_Option_C).
--
-- Lightweight design (like menu.lua): FindAllOf is EXPENSIVE (it scans every
-- UObject), so we call it only when (re)acquiring the menu and cache the widgets
-- (menu root, the row pool, the keyhelp). Each poll tick then only reads cached
-- objects — no per-tick FindAllOf. Announcements are diff-gated.
--
-- Widget model (verified on DBZ Kakarot, UE 4.21):
--   Start_Option_C_0
--     .Txt_Title_Steam / .Txt_Title      current tab title
--     rows: Xlist_Bar03_C (fixed pool, recycled on scroll)
--       .Ins_Cursor_Fad   VISIBLE on the selected row (0 rows visible = focus is
--                          on the "Guardar cambios" button below the list)
--       .Txt_List          option name
--       value, in priority order (recycled rows have STALE text on hidden nodes):
--         .Xlist_Bar_03_Gauge  -> volume slider; level = # visible Ins_System_OnNN
--         .Txt_Mode (if visible)        -> toggle / dropdown value
--         .Txt_Mode_Scroll (if visible) -> scrolling value
--       .Txt_Enter         holds the localized "Guardar cambios" label
--   Xcmn_Keyhelp_C_0 .Txt_Helpmsg_Main  tooltip/description of the focused item
--
-- Behaviour (reference/ui-accessibility/menus.md):
--   vertical move (name changes) -> name + value + tooltip
--   value change (same name)     -> only the new value
--   tab change                   -> only the tab name

local Speech = require("speech")

local Options = {}

local POLL_MS = 100
local REACQUIRE_EVERY = 10
local GAUGE_SEGMENTS = 20
-- fad must stay 0 this many ticks (x POLL_MS) before we treat focus as "Guardar
-- cambios". There is no stable selection index on this menu (the container exposes
-- none), and the fad cursor flickers off during scrolls, so a plain time debounce
-- either fires spuriously mid-scroll or goes silent. Instead we detect Guardar as
-- "fad==0 AND the keyhelp tooltip has settled (same non-nil value N ticks)":
-- during a scroll the keyhelp keeps changing, so it never settles; once the user
-- stops on Guardar it settles and we announce. Language-independent, no false hits.
local GUARDAR_STABLE = 2
local TOOLTIP_WINDOW = 6   -- ticks to keep polling for a late-arriving row tooltip

local enabled = false
local tick = 0

-- Cached widgets (refreshed only on acquire()). NOTE: the keyhelp bar is NOT
-- cached — it gets recreated on context changes, so a cached handle goes stale and
-- reads empty; we look it up fresh (only when needed).
local opt = nil
local rows = {}

-- Diff-gate state.
local isOpen = false
local lastName, lastValue, lastTab = nil, nil, nil
local fadStable = 0        -- consecutive ticks the keyhelp stayed put while fad==0
local lastFad0Tip = nil    -- keyhelp value during the current fad==0 stretch
local tipPending = false   -- waiting for a late tooltip to appear
local tipDeadline = 0

local function valid(o) return o ~= nil and o:IsValid() == true end

local function is_visible(o)
    if not valid(o) then return false end
    local ok, v = pcall(function() return o:IsVisible() end)
    return ok and v == true
end

local function text_of(node)
    if not valid(node) then return nil end
    local m = node.mainTxt
    if not valid(m) then return nil end
    local ok, s = pcall(function() return m.Text:ToString() end)
    if ok and s and s ~= "" then return s end
    return nil
end

-- Single expensive step: locate + cache the menu root and its row pool.
local function acquire()
    opt, rows = nil, {}

    local all = FindAllOf("Start_Option_C") or {}
    for _, o in pairs(all) do
        if valid(o) and o:GetFullName():find("/Engine/Transient", 1, true) then
            opt = o
            break
        end
    end
    if not opt then return end

    local rr = FindAllOf("Xlist_Bar03_C") or {}
    for _, r in pairs(rr) do
        if valid(r) and r:GetFullName():find("Start_Option_C", 1, true) then
            rows[#rows + 1] = r
        end
    end
end

local function ready()
    return valid(opt) and rows[1] ~= nil and valid(rows[1])
end

-- Fresh lookup (not cached): the focused item's tooltip. Scans keyhelp instances
-- and returns the first with text. Only called on focus changes / while a tooltip
-- is pending, so the FindAllOf cost stays occasional.
local function tooltip()
    local all = FindAllOf("Xcmn_Keyhelp_C") or {}
    for _, k in pairs(all) do
        if valid(k) and k:GetFullName():find("/Engine/Transient", 1, true) then
            local t = text_of(k.Txt_Helpmsg_Main)
            if t then return t end
        end
    end
    return nil
end

-- Volume gauge level as a percentage string, or nil if the gauge isn't shown.
local function gauge_value(row)
    local g = row.Xlist_Bar_03_Gauge
    if not is_visible(g) then return nil end
    local on = 0
    for i = 0, GAUGE_SEGMENTS - 1 do
        if is_visible(g["Ins_System_On" .. string.format("%02d", i)]) then on = on + 1 end
    end
    return math.floor(on / GAUGE_SEGMENTS * 100 + 0.5) .. "%"
end

local function row_value(row)
    local g = gauge_value(row)
    if g then return g end
    if is_visible(row.Txt_Mode) then
        local v = text_of(row.Txt_Mode)
        if v then return v end
    end
    if is_visible(row.Txt_Mode_Scroll) then
        local v = text_of(row.Txt_Mode_Scroll)
        if v then return v end
    end
    return nil
end

local function row_index(r)
    local s = r:GetFullName():match("Xlist_Bar03_(%d+)")
    return s and tonumber(s) or -1
end

-- Iterate the cached row pool (no FindAllOf). Returns: the lowest-index fad row
-- (the real selection), the highest row index present, any row, and a by-index map.
local function scan()
    local low, maxIdx, any, byIdx = nil, -1, nil, {}
    for _, r in ipairs(rows) do
        if valid(r) then
            any = any or r
            local idx = row_index(r)
            byIdx[idx] = r
            if idx > maxIdx then maxIdx = idx end
            if is_visible(r.Ins_Cursor_Fad) then
                if not low or idx < low.idx then low = { idx = idx, row = r } end
            end
        end
    end
    return low, maxIdx, any, byIdx
end

-- The "Guardar cambios" button is a phantom overflow row at the last slot: when
-- you scroll past the last real option, that slot DUPLICATES the option above it
-- (same Txt_List) and the fad sits only on it. A real option that merely happens
-- to render in the last slot does NOT duplicate its neighbour, so comparing the
-- last two rows' names distinguishes Guardar from a real bottom-slot option.
local function is_guardar_phantom(low, maxIdx, byIdx)
    if not low or low.idx ~= maxIdx then return false end
    local above = byIdx[maxIdx - 1]
    if not above then return false end
    return text_of(above.Txt_List) == text_of(low.row.Txt_List)
end

local function current_tab()
    return text_of(opt.Txt_Title_Steam) or text_of(opt.Txt_Title)
end

-- Join non-empty parts with commas, skipping nils in ANY position (ipairs would
-- stop at the first nil, so we walk by count).
local function phrase(...)
    local n = select("#", ...)
    local parts = {}
    for i = 1, n do
        local p = select(i, ...)
        if p and p ~= "" then parts[#parts + 1] = p end
    end
    return table.concat(parts, ", ")
end

local function reset()
    isOpen = false
    lastName, lastValue, lastTab = nil, nil, nil
    fadStable, lastFad0Tip = 0, nil
    tipPending = false
end

local function step()
    if not ready() then
        if tick % REACQUIRE_EVERY == 0 then acquire() end
        if not ready() then if isOpen then reset() end return end
    end

    if not is_visible(opt) then
        if isOpen then reset() end
        return
    end

    local tab = current_tab()
    local low, maxIdx, anyRow, byIdx = scan()

    local name, value, isGuardar
    if low then
        fadStable, lastFad0Tip = 0, nil
        if is_guardar_phantom(low, maxIdx, byIdx) then
            isGuardar = true
        else
            name, value = text_of(low.row.Txt_List), row_value(low.row)
        end
    else
        -- fad==0: could be a scroll flicker or the Guardar button. Only commit once
        -- the keyhelp tooltip settles (unchanged, non-nil) for N ticks — that happens
        -- when the user stops, not while scrolling.
        local tt = tooltip()
        if tt and tt == lastFad0Tip then
            fadStable = fadStable + 1
        else
            fadStable = tt and 1 or 0
            lastFad0Tip = tt
        end
        if fadStable < GUARDAR_STABLE then return end
        isGuardar = true
    end

    if isGuardar then
        name = (anyRow and text_of(anyRow.Txt_Enter)) or "Guardar cambios"
        value = nil
    end

    -- Classify what changed this tick.
    local event
    if not isOpen then event = "enter"
    elseif tab ~= lastTab then event = "tab"
    elseif name ~= lastName then event = "name"
    elseif value ~= lastValue then event = "value"
    end

    if event then
        isOpen = true
        lastTab, lastName, lastValue = tab, name, value
        if event == "tab" then
            if tab then Speech.say(tab, true) end
            tipPending = false
        elseif event == "value" then
            if value then Speech.say(value, true) end
            tipPending = false
        else -- enter / name: read name (+ value) + tooltip
            local tt = tooltip()
            if event == "enter" then
                Speech.say(phrase(tab, name, value, tt), true)
            else
                Speech.say(phrase(name, value, tt), true)
            end
            -- Some items (e.g. Guardar cambios) fill the keyhelp a few ticks late;
            -- keep polling briefly and speak it when it appears.
            tipPending = (tt == nil)
            tipDeadline = tick + TOOLTIP_WINDOW
        end
        return
    end

    -- No change: service a pending late tooltip.
    if tipPending then
        if tick > tipDeadline then
            tipPending = false
        else
            local tt = tooltip()
            if tt then
                Speech.say(tt, false)
                tipPending = false
            end
        end
    end
end

function Options.start()
    if enabled then return end
    enabled = true
    _G.__KakarotOptionsGen = (_G.__KakarotOptionsGen or 0) + 1
    local myGen = _G.__KakarotOptionsGen
    LoopAsync(POLL_MS, function()
        if not enabled or _G.__KakarotOptionsGen ~= myGen then return true end
        tick = tick + 1
        ExecuteInGameThread(step)
        return false
    end)
    print("[KakarotAccess] Options menu reader started\n")
end

function Options.stop()
    enabled = false
end

function Options.is_enabled() return enabled end

return Options
