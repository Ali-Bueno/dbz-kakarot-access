-- Screen adapter: Options menu (Start_Option_C).
--
-- Classifies the focused control (list row vs the "Guardar cambios" overflow
-- button) using the reusable Xlist_Bar03 archetype readers, then hands
-- (tab, name, value, tooltip) to the shared Announcer.
--
-- Model + quirks are documented in ui_archetypes.lua and the project memory.

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")
local Keyhelp = require("keyhelp")

local Options = {}

local GUARDAR_STABLE = 2   -- ticks the keyhelp must settle before the fad==0 case is Guardar

local ann = Core.make_announcer()
local opt = nil
local rows = {}
local tick = 0
local fadStable = 0
local lastFad0Tip = nil

-- The focused control's contextual tooltip is the keyhelp bar's help message. Reuse the
-- shared Keyhelp reader (single Xcmn_Keyhelp_C-finding path) rather than re-scanning here.
-- Passed to the Announcer as a lazy getter → only called on focus changes / while a
-- tooltip is pending, not every tick. The bar is recreated on context changes, so it is
-- never cached; each call reads it fresh.
local tooltip = Keyhelp.helpmsg

-- Rescan the option rows (pooled Xlist_Bar03_C filtered to this screen). Only called when
-- the menu is on screen and the cached rows are gone, so it runs ~once per entry.
local function refresh_rows()
    rows = {}
    for _, r in pairs(FindAllOf("Xlist_Bar03_C") or {}) do
        if Core.valid(r) and r:GetFullName():find("Start_Option_C", 1, true) then
            rows[#rows + 1] = r
        end
    end
end

function Options.is_active()
    tick = tick + 1
    opt = Core.cached_live("Start_Option_C", tick)   -- cheap: cached ref, no per-tick scan
    if not Core.on_screen(opt) then return false end
    if not (rows[1] and Core.valid(rows[1])) then refresh_rows() end
    return rows[1] ~= nil and Core.valid(rows[1])
end

function Options.reset()
    ann:reset()
    fadStable, lastFad0Tip = 0, nil
    -- Rebuild the KeyConfig->button map next time it's needed: a controller rebind
    -- done in this screen changes the mapping.
    A.clear_binding_cache()
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
        name = (anyRow and Core.text_of(anyRow.Txt_Enter)) or I18n.t("save_changes")
        value = nil
    end

    -- On entry the screen name is the current tab's title (localized); tab changes
    -- are announced via the same value.
    ann:focus(tab, tab, name, value, tooltip)
end

return Options
