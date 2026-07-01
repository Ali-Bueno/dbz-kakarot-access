-- Screen adapter: Options menu (Start_Option_C).
--
-- Classifies the focused control (list row vs the "Guardar cambios" overflow
-- button) using the reusable Xlist_Bar03 archetype readers, then hands
-- (tab, name, value, tooltip) to the shared Announcer.
--
-- Model + quirks are documented in ui_archetypes.lua and the project memory.

local Core = require("ui_core")
local A = require("ui_archetypes")

local Options = {}

local REACQUIRE_EVERY = 10
local GUARDAR_STABLE = 2   -- ticks the keyhelp must settle before the fad==0 case is Guardar

local ann = Core.make_announcer()
local opt = nil
local rows = {}
local tick = 0
local fadStable = 0
local lastFad0Tip = nil

-- Fresh keyhelp lookup (the keyhelp bar is recreated on context changes, so it is
-- never cached). Passed to the Announcer as a lazy getter → only called on focus
-- changes / while a tooltip is pending, not every tick.
local function tooltip()
    local all = FindAllOf("Xcmn_Keyhelp_C") or {}
    for _, k in pairs(all) do
        if Core.valid(k) and k:GetFullName():find("/Engine/Transient", 1, true) then
            local t = Core.text_of(k.Txt_Helpmsg_Main)
            if t then return t end
        end
    end
    return nil
end

local function acquire()
    opt, rows = nil, {}
    opt = Core.first_live("Start_Option_C")
    if not opt then return end
    local rr = FindAllOf("Xlist_Bar03_C") or {}
    for _, r in pairs(rr) do
        if Core.valid(r) and r:GetFullName():find("Start_Option_C", 1, true) then
            rows[#rows + 1] = r
        end
    end
end

local function ready() return Core.valid(opt) and rows[1] ~= nil and Core.valid(rows[1]) end

function Options.is_active()
    tick = tick + 1
    if not ready() and tick % REACQUIRE_EVERY == 0 then acquire() end
    return ready() and Core.is_visible(opt)
end

function Options.reset()
    ann:reset()
    fadStable, lastFad0Tip = 0, nil
end

function Options.update()
    local tab = Core.text_of(opt.Txt_Title_Steam) or Core.text_of(opt.Txt_Title)
    local low, maxIdx, anyRow, byIdx = A.scan_list(rows)

    local name, value, isGuardar
    if low then
        fadStable, lastFad0Tip = 0, nil
        if A.is_overflow_button(low, maxIdx, byIdx) then
            isGuardar = true
        else
            name, value = A.row_name(low.row), A.row_value(low.row)
        end
    else
        -- fad==0: Guardar or a scroll flicker. Commit only once the keyhelp tooltip
        -- settles (unchanged, non-nil) for N ticks — i.e. the user stopped moving.
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
        name = (anyRow and Core.text_of(anyRow.Txt_Enter)) or "Guardar cambios"
        value = nil
    end

    -- On entry the screen name is the current tab's title (localized); tab changes
    -- are announced via the same value.
    ann:focus(tab, tab, name, value, tooltip)
end

return Options
