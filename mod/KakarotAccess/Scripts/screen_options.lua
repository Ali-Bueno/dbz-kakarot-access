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
-- the menu is on screen and the cached rows are stale, so it runs ~once per entry.
--
-- The list is CLEARED FIRST and the scan is BUDGETED (2026-07-25): a raw FindAllOf costs ~65ms
-- here, and the staleness test below can stay false for several consecutive ticks, so an
-- unbudgeted scan per tick is the navigation-lag bug this codebase keeps relearning. On a budget
-- denial the list stays empty, is_active reports not-ready, and the next tick retries — which is
-- strictly better than the old behaviour of proceeding with rows from a previous visit.
-- BACKOFF (2026-07-25 round 2, and this one was a real regression I caused). Round 1 made the
-- staleness test require `on_screen`, which was right — but it also meant the test can be FALSE for
-- as long as the pooled host lingers, and `refresh_rows` was called on every such tick. A scan slot
-- is not a rate limit: `Core.take_scan_slot` allows 2 per tick, so this turned a once-per-session
-- FindAllOf into a ~65 ms scan EVERY TICK. That starved the shared budget (story dialogue is
-- scan-detected, so it went silent — and stayed silent across a mod reload, because the state
-- causing it belongs to the GAME, not the mod) and multiplied our exposure to dangling objects,
-- which is the per-tick-retry escalation the crash ledger warns about. A slot is a fair-share
-- device; the rate limit has to be explicit.
local RESCAN_EVERY = 10   -- ticks (~1 s) between attempts, worst case
local rescan_next = 0

local function refresh_rows()
    rows = {}
    if tick < rescan_next then return end
    rescan_next = tick + RESCAN_EVERY
    if not Core.take_scan_slot() then return end
    for _, r in pairs(Core.findall("Xlist_Bar03_C")) do
        if Core.valid(r) and r:GetFullName():find("Start_Option_C", 1, true) then
            rows[#rows + 1] = r
        end
    end
end

-- Are the cached rows the rows of THIS visit? `Core.valid` alone is not that question: this game
-- only COLLAPSES a submenu's widgets on close, so a row from the previous visit stays valid
-- indefinitely while being nowhere on screen. That is why Options read on first entry and was
-- SILENT on every later one (user, 2026-07-25): `rows` survived, the staleness test passed, and
-- A.scan_list found no visible `Ins_Cursor_Fad` on the orphaned rows, so `low` was nil forever
-- and update() returned before ever reaching the announcer. `on_screen` is the same check
-- Core.first_on_screen makes for every other list screen; this adapter hand-rolls its cache
-- (it has to filter the shared Xlist_Bar03_C pool by owner) and had simply skipped it.
local function rows_current()
    return rows[1] ~= nil and Core.valid(rows[1]) and Core.on_screen(rows[1])
end

function Options.is_active()
    tick = tick + 1
    opt = Core.cached_live("Start_Option_C", tick)   -- cheap: cached ref, no per-tick scan
    if not Core.on_screen(opt) then return false end
    -- pane_live as well as on_screen, per CLAUDE.md §8: this host is a POOLED full-screen pane, and
    -- a parked one keeps reporting on_screen with stale content long after the menu closed. Without
    -- this the adapter stayed half-awake after every visit to Options, re-scanning for rows that
    -- would never be on screen again. Cheap (two guarded reads) and it makes the closed state
    -- genuinely inert instead of merely unproductive.
    if not Core.pane_live(opt) then return false end
    if not rows_current() then refresh_rows() end
    return rows_current()
end

function Options.reset()
    ann:reset()
    fadStable, lastFad0Tip = 0, nil
    -- Drop the row list too: reset() runs on every screen change (both leaving and re-entering,
    -- ui_registry), so this guarantees a fresh scan per visit even if the staleness test above
    -- were ever fooled again. It was the one latch this function forgot.
    -- The backoff clears with it, so a genuine entry re-scans on the very next poll rather than
    -- waiting out a timer left over from the previous visit.
    rows, rescan_next = {}, 0
    -- Rebuild the KeyConfig->button map next time it's needed: a controller rebind
    -- done in this screen changes the mapping.
    A.clear_binding_cache()
end

function Options.update()
    local tab = Core.text_of(Core.member(opt, "Txt_Title_Steam")) or Core.text_of(Core.member(opt, "Txt_Title"))
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
        name = (anyRow and Core.text_of(Core.member(anyRow, "Txt_Enter"))) or I18n.t("save_changes")
        value = nil
    end

    -- On entry the screen name is the current tab's title (localized); tab changes
    -- are announced via the same value.
    ann:focus(tab, tab, name, value, tooltip)
end

return Options
