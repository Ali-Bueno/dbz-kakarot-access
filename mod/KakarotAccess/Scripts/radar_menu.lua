-- R2 radar target picker (controller overlay, modal toggle).
--
-- Press R2 in the RPG overworld to OPEN a target list; the game is frozen while it's
-- open (input_bridge hides the pad from the game, see input.lua) so no field command
-- fires. It's modal — you don't hold anything:
--   * L1 / R1  — previous / next CATEGORY (like menu tabs): Quests, Characters,
--                Fishing, Gathering, Shops, Minigames, Dragon Balls, Other.
--   * D-pad up / down — move through the items in the category (nearest first).
--   * A / Cross — SELECT the focused item: it becomes the audio-radar target, menu closes.
--   * R2 again  — close WITHOUT changing anything (cancel).
--   * B / Circle — close AND stop tracking the current target (the "stop" shortcut).
--
-- RPG-ONLY, NEVER COMBAT: opening is gated on Nav.field_ready() (world alive AND no
-- pausing menu). The minimap — the game's own "free-roaming" signal — is hidden in
-- battle/cutscenes/menus, so R2 there does nothing and the pad is never blocked. If a
-- battle/cutscene starts while the menu is open, it closes and unblocks at once.
--
-- On close the pad stays blocked until every button/trigger is released (a "drain"),
-- so the game never receives the residual A/R2/B press that closed the menu.
--
-- All world reads live in nav_tracker (Nav.list_targets / Nav.set_manual_target /
-- Nav.stop_tracking), behind the radar's safety gates; this file is pure input + speech.

local Input = require("input")
local Speech = require("speech")
local I18n = require("i18n")
local Nav = require("nav_tracker")

local Menu = {}

local TICK_MS = 60
local OPEN_TH = 45        -- R2 raw (0..255) to register a press
local REL_TH = 25         -- R2 raw to register a release (hysteresis)

local running = false
local open = false        -- menu currently open (and pad blocked)
local blocked = false
local draining = false    -- closed, waiting for a neutral pad before unblocking
local r2_state = false    -- debounced R2 pressed/released state (for edge detection)
local cats = {}           -- [{ key, name, items={{actor,key,dist,noun},...} }]
local ci, ii = 1, 1       -- current category / item index
local prev_btn = 0        -- previous button bitmask (edge detection)

local B = Input.BTN

-- ---- announcements -----------------------------------------------------------------

local function announce_item(with_cat)
    local cat = cats[ci]
    if not cat then Speech.say(I18n.t("radar_nothing"), true) return end
    local it = cat.items[ii]
    if not it then Speech.say(cat.name .. ", " .. I18n.t("radar_empty_cat"), true) return end
    local parts = {}
    if with_cat then parts[#parts + 1] = cat.name end
    parts[#parts + 1] = Nav.item_phrase(it)
    parts[#parts + 1] = string.format(I18n.t("pos"), ii, #cat.items)
    Speech.say(table.concat(parts, ", "), true)
end

-- ---- open / close ------------------------------------------------------------------

local function do_open()
    -- Block the pad FIRST so the game never sees this R2 press, THEN read the world.
    Input.block(true)
    blocked = true
    open = true
    cats = Nav.list_targets()
    ci, ii = 1, 1
    if #cats == 0 then
        Speech.say(I18n.t("radar_nothing"), true)
    else
        announce_item(true)
    end
end

-- Close the menu. mode:
--   "select" — commit the focused item to the radar
--   "stop"   — stop tracking the current target (B / Circle)
--   "cancel" — leave current tracking unchanged (R2 again, or lost field/pad)
-- The pad stays blocked (draining) until the player releases everything, so the
-- residual press that closed the menu never reaches the game.
local function do_close(mode)
    open = false
    if mode == "select" then
        local cat = cats[ci]
        local it = cat and cat.items[ii]
        if it and it.actor then
            Nav.set_manual_target(it.actor, it.key, I18n.t(it.noun))
        end
    elseif mode == "stop" then
        Nav.stop_tracking()
    end
    cats = {}
    -- keep blocked; drain until neutral (see step). If the pad is already gone we
    -- unblock immediately (handled by the caller for the pad-lost path).
    draining = true
end

-- ---- per-tick step (game thread) ----------------------------------------------------

local function step()
    local snap = Input.read()
    if not snap then
        -- pad lost — never leave the game blocked
        if blocked then Input.block(false) end
        blocked, open, draining, r2_state = false, false, false, false
        prev_btn = 0
        return
    end

    -- Debounced R2 edge (rising = a fresh press this tick).
    local r2_rise = false
    if not r2_state and snap.rt >= OPEN_TH then
        r2_state = true; r2_rise = true
    elseif r2_state and snap.rt < REL_TH then
        r2_state = false
    end

    -- Draining: menu closed, waiting for a fully neutral pad before handing control
    -- back — so the A/R2/B that closed it isn't delivered to the game.
    if draining then
        if snap.buttons == 0 and snap.rt < REL_TH and snap.lt < REL_TH then
            Input.block(false); blocked = false; draining = false
        end
        prev_btn = snap.buttons
        return
    end

    if not open then
        if r2_rise and Nav.field_ready() then do_open() end
        prev_btn = snap.buttons
        return
    end

    -- Safety: a battle/cutscene/menu started while open -> bail out (drain-unblock).
    if not Nav.field_ready() then
        do_close("cancel")
        prev_btn = snap.buttons
        return
    end

    local function pressed(mask) return (snap.buttons & mask) ~= 0 and (prev_btn & mask) == 0 end

    if pressed(B.A) then                       -- A / Cross -> select + track
        do_close("select")
    elseif r2_rise or pressed(B.B) then        -- R2 again / B -> close
        do_close(pressed(B.B) and "stop" or "cancel")
    elseif #cats > 0 then
        local n = #cats
        if pressed(B.RIGHT_SHOULDER) then      -- R1 -> next category
            ci = ci % n + 1; ii = 1; announce_item(true)
        elseif pressed(B.LEFT_SHOULDER) then   -- L1 -> previous category
            ci = (ci - 2) % n + 1; ii = 1; announce_item(true)
        else
            local m = #cats[ci].items
            if m > 0 then
                if pressed(B.DPAD_DOWN) then
                    ii = ii % m + 1; announce_item(false)
                elseif pressed(B.DPAD_UP) then
                    ii = (ii - 2) % m + 1; announce_item(false)
                end
            end
        end
    end

    prev_btn = snap.buttons
end

-- ---- loop management (same generation/busy guard as nav_tracker) --------------------

function Menu.start()
    if running then return end
    if not Input.is_loaded() then
        print("[KakarotAccess] radar_menu: input bridge not loaded, disabled\n")
        return
    end
    running = true
    _G.__KakarotRadarMenuGen = (_G.__KakarotRadarMenuGen or 0) + 1
    local myGen = _G.__KakarotRadarMenuGen
    local busy = false
    LoopAsync(TICK_MS, function()
        if _G.__KakarotRadarMenuGen ~= myGen then return true end
        if not busy then
            busy = true
            ExecuteInGameThread(function()
                local ok, err = pcall(step)
                busy = false
                if not ok then
                    -- never strand the pad in a blocked state on an error
                    if blocked then Input.block(false) end
                    blocked, open, draining = false, false, false
                    print("[KakarotAccess] radar_menu step error: " .. tostring(err) .. "\n")
                end
            end)
        end
        return false
    end)
end

function Menu.stop()
    running = false
    _G.__KakarotRadarMenuGen = (_G.__KakarotRadarMenuGen or 0) + 1
    if blocked then Input.block(false) end
    blocked, open, draining, r2_state = false, false, false, false
    cats = {}
end

return Menu
