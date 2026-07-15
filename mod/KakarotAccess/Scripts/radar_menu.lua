-- R3 radar target picker (controller overlay, modal toggle).
--
-- Click the RIGHT STICK (R3) in the RPG overworld to OPEN a target list; the game is
-- frozen while it's open (input_bridge hides the pad from the game, see input.lua) so
-- no field command fires. R3 (not a trigger) is used so the flight controls stay free
-- (R2 = descend / L2 = ascend on the Flying Nimbus). It's modal — you don't hold
-- anything:
--   * L1 / R1  — previous / next CATEGORY (like menu tabs): Quests, Collectibles,
--                Characters, Enemies, Sites, Fishing, Gathering, Shops, Minigames,
--                Dragon Balls, Other (GROUP_ORDER in nav_tracker).
--   * D-pad up / down — move through the items in the category (nearest first).
--   * A / Cross — SELECT the focused item: it becomes the audio-radar target, menu closes.
--   * R3 again  — close WITHOUT changing anything (cancel).
--   * B / Circle — close AND stop tracking the current target (the "stop" shortcut).
--
-- RPG-ONLY, NEVER COMBAT: opening is gated on Nav.field_ready() (world alive AND no
-- pausing menu). The minimap — the game's own "free-roaming" signal — is hidden in
-- battle/cutscenes/menus, so R3 there does nothing and the pad is never blocked. If a
-- battle/cutscene starts while the menu is open, it closes and unblocks at once.
--
-- On close the pad stays blocked until every button/trigger is released (a "drain"),
-- so the game never receives the residual A/R3/B press that closed the menu.
--
-- All world reads live in nav_tracker (Nav.list_targets / Nav.set_manual_target /
-- Nav.stop_tracking), behind the radar's safety gates; this file is pure input + speech.

local Input = require("input")
local Speech = require("speech")
local I18n = require("i18n")
local Nav = require("nav_tracker")
local Transition = require("transition")
local PadPoll = require("pad_poll")

local Menu = {}

-- Runs on the shared 20ms pad scheduler (pad_poll.lua): catches the R3 press within ~1
-- frame so the game barely sees it before we block (the opening click could otherwise
-- leak a frame to R3's field action).
local REL_TH = 25         -- trigger raw (0..255) below which it counts as released (drain)
local DOUBLE_TAP_TICKS = 14   -- ~280 ms at the 20ms pad tick: window for a 2nd R3 tap (= explore toggle)

local running = false
local open = false        -- menu currently open (and pad blocked)
local blocked = false
local draining = false    -- closed, waiting for a neutral pad before unblocking
local cats = {}           -- [{ key, name, items={{actor,key,dist,noun},...} }]
local ci, ii = 1, 1       -- current category / item index
local prev_btn = 0        -- previous button bitmask (edge detection)
local tk = 0              -- step counter (double-tap timing)
local last_r3_tk = nil    -- tick of the previous R3 press
local pending_open_tk = nil   -- R3 pressed; holding to see if a 2nd tap makes it a toggle

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
    -- Build the target list BEFORE blocking. If a world read faults (an uncatchable C++
    -- abort on this game — e.g. a bad property access), it must NOT strand the pad in a
    -- blocked state and freeze the game. The one-tick R3 leak before we block is
    -- harmless (R3 = ki-sense). Safety over the micro-leak.
    local list = Nav.list_targets()
    Input.block(true)
    blocked = true
    open = true
    cats = list
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
            -- grp/stateful drive the arrival chaining for pickup categories.
            Nav.set_manual_target(it.actor, it.key, Nav.item_label(it),
                it.grp, it.stateful)
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
    tk = tk + 1
    -- Transition gate FIRST (pure Lua + native pad calls only): a map switch is in
    -- progress — close the menu, drop the actor refs in `cats`, and NEVER leave the
    -- pad blocked across a level change.
    if Transition.active() then
        if blocked then Input.block(false) end
        blocked, open, draining = false, false, false
        cats = {}
        prev_btn = 0
        last_r3_tk, pending_open_tk = nil, nil
        return
    end

    local snap = Input.read()
    if not snap then
        -- pad lost — never leave the game blocked
        if blocked then Input.block(false) end
        blocked, open, draining = false, false, false
        prev_btn = 0
        last_r3_tk, pending_open_tk = nil, nil
        return
    end

    local function pressed(mask) return (snap.buttons & mask) ~= 0 and (prev_btn & mask) == 0 end

    -- Draining: menu closed, waiting for a fully neutral pad before handing control
    -- back — so the A/R3/B that closed it isn't delivered to the game.
    if draining then
        if snap.buttons == 0 and snap.rt < REL_TH and snap.lt < REL_TH then
            Input.block(false); blocked = false; draining = false
        end
        prev_btn = snap.buttons
        return
    end

    if not open then
        -- R3: a SINGLE click opens the target menu; a DOUBLE-tap toggles the passive
        -- explore radar. We hold the open ~280 ms to see if a 2nd tap arrives (the menu
        -- is the fallback now, so the small delay is fine); the R3 leak to the game in
        -- that window is harmless (R3 = ki-sense, as when opening).
        if pressed(B.RIGHT_THUMB) and Nav.field_ready() then
            if last_r3_tk and (tk - last_r3_tk) <= DOUBLE_TAP_TICKS then
                last_r3_tk, pending_open_tk = nil, nil
                Nav.toggle_explore()
            else
                last_r3_tk, pending_open_tk = tk, tk
            end
        elseif pending_open_tk and (tk - pending_open_tk) > DOUBLE_TAP_TICKS then
            pending_open_tk = nil
            if Nav.field_ready() then do_open() end
        end
        prev_btn = snap.buttons
        return
    end

    -- Safety: a battle/cutscene/menu started while open -> bail out (drain-unblock).
    if not Nav.field_ready() then
        do_close("cancel")
        prev_btn = snap.buttons
        return
    end

    if pressed(B.A) then                        -- A / Cross -> select + track
        do_close("select")
    elseif pressed(B.RIGHT_THUMB) or pressed(B.B) then   -- R3 again / B -> close
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
    -- Shared 20ms scheduler (pad_poll.lua) — one dispatch serves every pad stepper.
    PadPoll.register("radar_menu", step, function()
        -- never strand the pad in a blocked state on an error
        if blocked then Input.block(false) end
        blocked, open, draining = false, false, false
    end)
end

function Menu.stop()
    running = false
    PadPoll.unregister("radar_menu")
    if blocked then Input.block(false) end
    blocked, open, draining = false, false, false
    cats = {}
end

return Menu
