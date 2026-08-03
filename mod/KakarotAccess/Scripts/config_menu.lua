-- Mod configuration menu (controller overlay, modal), opened with L3 + R3 in the
-- overworld. A screen-reader-navigable list of the mod's own settings — it reads and
-- writes the persistent store in settings.lua, so changes take effect at once and
-- survive a restart.
--
--   * L3 + R3         — open / close (also B to close).
--   * D-pad up / down  — move between options (name + current value).
--   * D-pad left/right — change the focused option (toggle, ±10 % volume, cycle language).
--   * A / Cross        — same as right (advance the value), for convenience.
--   * B / Circle       — close.
--
-- Like the radar picker (radar_menu.lua) it is modal: the pad is hidden from the game
-- while open and drained on close so the closing press never leaks. The two share the
-- pad through the `_G.__KakarotPadModal` mutex (only one may own a blocking overlay),
-- and radar_menu ignores R3 while L3 is held so the opening chord never opens the picker.
--
-- RPG-ONLY: opening is gated on Nav.field_ready() (world alive, no pausing menu), the
-- same safe surface as the radar picker.

local Input = require("input")
local Speech = require("speech")
local I18n = require("i18n")
local Nav = require("nav_tracker")
local Audio = require("audio")
local Settings = require("settings")
local Transition = require("transition")
local PadPoll = require("pad_poll")

local Menu = {}

-- Trigger raw (0..255) below which it counts as released (drain). RAISED FROM 25 to XInput's own
-- XINPUT_GAMEPAD_TRIGGER_THRESHOLD (30) on 2026-07-31 — the mismatch WAS the lock-out bug: a worn
-- trigger resting at 26-29 reads as released to XInput and to the game but never to this line, so
-- the drain could not complete and the pad stayed hidden from the game for good.
local REL_TH = 30
-- Stuck-pad rescue for the close-drain — anti-lockout only, and identical in shape to radar_menu's
-- (read the long note there for the reasoning). A flat deadline was tried first the same day and
-- REGRESSED: forcing the release while a button was still held handed that button to the game.
-- The discriminator is CHANGE, not time — a stuck button reports a constant bitmask, a pad in use
-- keeps changing — so the deadline restarts on every change of the digital button state.
local DRAIN_STUCK_S = 3.0
local drain_until = nil
local drain_btn = nil     -- digital button state the stuck-detector last observed
local VOL_STEP = 10       -- cue-volume step per left/right press (percent)
local MODES = { "auto", "on", "off" }   -- cycle order of a tri-state setting (settings.lua "mode")

local running = false
local open = false
local blocked = false
local draining = false
local prev_btn = 0
local idx = 1             -- focused option

local B = Input.BTN

-- ---- options ----------------------------------------------------------------
-- Each option renders its value and mutates it by a signed step (dir = -1 / +1).
-- change() returns the localized value string to announce.

local function on_off(v) return I18n.t(v and "cfg_on" or "cfg_off") end

-- Tri-state ("auto" adds a third value to the on/off pair, so the two existing words are
-- reused rather than translating a second set of them).
local function mode_name(m)
    if m == "auto" then return I18n.t("cfg_auto") end
    return on_off(m == "on")
end

-- Advance a tri-state setting by `dir` and return its new localized name.
local function cycle_mode(key, dir)
    local cur = Settings.get(key)
    local at = 1
    for i, m in ipairs(MODES) do if m == cur then at = i break end end
    return Settings.set(key, MODES[(at - 1 + dir) % #MODES + 1])
end

local OPTIONS = {
    {   -- Audio cues on/off
        name = function() return I18n.t("cfg_audio_cues") end,
        value = function() return on_off(Settings.get("audio_cues")) end,
        change = function(dir)
            local nv = not Settings.get("audio_cues")
            Settings.set("audio_cues", nv)
            if not nv then Audio.stop() end   -- cut any beacon/tone playing right now
            return on_off(nv)
        end,
    },
    {   -- Cue volume 0..100 %
        name = function() return I18n.t("cfg_cue_volume") end,
        value = function() return string.format(I18n.t("cfg_pct"), Settings.get("cue_volume")) end,
        change = function(dir)
            local nv = Settings.get("cue_volume") + dir * VOL_STEP
            nv = Settings.set("cue_volume", nv)   -- clamps 0..100
            -- Sample the new level so the change is audible (centered ping, if cues on).
            if Settings.cues_enabled() then Audio.ping(0, 1.0, 1.0) end
            return string.format(I18n.t("cfg_pct"), nv)
        end,
    },
    {   -- Radar auto-activation on/off
        name = function() return I18n.t("cfg_radar_autotrack") end,
        value = function() return on_off(Settings.get("radar_autotrack")) end,
        change = function(dir)
            local nv = not Settings.get("radar_autotrack")
            Settings.set("radar_autotrack", nv)   -- nav_tracker reads it live via _G
            return on_off(nv)
        end,
    },
    {   -- Braille display output: auto (on when the screen reader reports it) / on / off
        name = function() return I18n.t("cfg_braille") end,
        value = function() return mode_name(Settings.get("braille")) end,
        change = function(dir)
            local nv = cycle_mode("braille", dir)
            Speech.set_braille(nv)   -- re-resolve the sink now, not on the next launch
            return mode_name(nv)
        end,
    },
    {   -- Language (auto or a game language code)
        name = function() return I18n.t("cfg_language") end,
        value = function() return I18n.lang_name(Settings.get("language")) end,
        change = function(dir)
            local langs = I18n.LANGS
            local cur = Settings.get("language")
            local at = 1
            for i, c in ipairs(langs) do if c == cur then at = i break end end
            at = (at - 1 + dir) % #langs + 1
            local nv = langs[at]
            Settings.set("language", nv)
            I18n.force_language(nv)   -- take effect for every following string
            I18n.refresh()
            return I18n.lang_name(nv)
        end,
    },
}

-- ---- announcements ----------------------------------------------------------

local function option_line(i)
    local o = OPTIONS[i]
    return o.name() .. ", " .. o.value()
end

-- ---- open / close -----------------------------------------------------------

local function do_open()
    Input.block(true)
    blocked, open, draining = true, true, false
    idx = 1
    _G.__KakarotPadModal = "config"
    Speech.say(I18n.t("cfg_title") .. ". " .. option_line(idx), true)
    Speech.say(I18n.t("cfg_hint"), false)
end

local function do_close()
    open = false
    Speech.say(I18n.t("cfg_closed"), true)
    draining = true   -- keep blocked until the pad is neutral (see step)
    drain_btn, drain_until = nil, os.clock() + DRAIN_STUCK_S
end

local function force_release()
    if blocked then Input.block(false) end
    blocked, open, draining = false, false, false
    prev_btn = 0
    if _G.__KakarotPadModal == "config" then _G.__KakarotPadModal = nil end
end

-- ---- per-tick step (game thread) --------------------------------------------

local function step()
    -- The dispatch grid this menu needs, declared BEFORE every early return so a bail-out can
    -- never leave the fast grid pinned (pad_poll.lua: slow by default, fast only on demand).
    PadPoll.demand_fast("config_menu", open or draining)
    if Transition.active() then force_release(); return end

    local snap = Input.read()
    if not snap then force_release(); return end

    -- Another overlay (the radar picker) owns the pad — stay out of its way.
    local modal = _G.__KakarotPadModal
    if modal and modal ~= "config" then prev_btn = snap.buttons; return end

    -- NATIVE LATCH FIRST, level compare as the fallback (2026-08-03, the form screen_map has
    -- always used). The bare two-tick compare this used to be could only see a button that was
    -- still HELD at dispatch time, so a tap that started and ended between two dispatches was
    -- lost outright — already possible whenever the busy guard dropped a tick, and routine now
    -- that the bus runs a 100 ms grid by default. `Input.pressed` answers from the accumulator
    -- the game's own XInput call feeds at frame rate, so the press survives any grid.
    local function pressed(mask)
        return Input.pressed(mask) or ((snap.buttons & mask) ~= 0 and (prev_btn & mask) == 0)
    end

    if draining then
        local neutral = snap.buttons == 0 and snap.rt < REL_TH and snap.lt < REL_TH
        -- Restart the rescue deadline on every change of the button state: while the player is
        -- doing anything at all, the release stays courteous and cannot steal a real press.
        if snap.buttons ~= drain_btn then
            drain_btn, drain_until = snap.buttons, os.clock() + DRAIN_STUCK_S
        end
        if neutral or (drain_until and os.clock() >= drain_until) then
            Input.block(false); blocked = false; draining = false
            drain_until, drain_btn = nil, nil
            if _G.__KakarotPadModal == "config" then _G.__KakarotPadModal = nil end
        end
        prev_btn = snap.buttons
        return
    end

    if not open then
        -- L3 + R3 chord opens the menu (R3 edge while L3 is held).
        if pressed(B.RIGHT_THUMB) and Input.down(snap, B.LEFT_THUMB) and Nav.field_ready() then
            do_open()
        end
        prev_btn = snap.buttons
        return
    end

    -- RE-ASSERT the pad block every tick while open — identical defect and fix to radar_menu's
    -- (read the long note there). The block is a 1 s LEASE renewed only as a side effect of
    -- polling, renewal cannot resurrect an expired lease, and this menu asserted it once in
    -- do_open; a dispatch gap longer than a second therefore left the menu open with the pad
    -- live to the game. One interlocked write per tick removes the whole failure class.
    -- (unconditional: the `draining` branch above returns, so this line is only ever
    -- reached with the menu open and the pad ours.)
    Input.block(true)

    -- Open: a battle/cutscene/menu started -> bail out (drain-unblock).
    if not Nav.field_ready() then
        do_close()
        prev_btn = snap.buttons
        return
    end

    -- Close: B, or the L3 + R3 chord again.
    if pressed(B.B) or (pressed(B.RIGHT_THUMB) and Input.down(snap, B.LEFT_THUMB)) then
        do_close()
    elseif pressed(B.DPAD_DOWN) then
        idx = idx % #OPTIONS + 1
        Speech.say(option_line(idx), true)
    elseif pressed(B.DPAD_UP) then
        idx = (idx - 2) % #OPTIONS + 1
        Speech.say(option_line(idx), true)
    elseif pressed(B.DPAD_RIGHT) or pressed(B.A) then
        local val = OPTIONS[idx].change(1)
        Speech.say(OPTIONS[idx].name() .. ", " .. val, true)
    elseif pressed(B.DPAD_LEFT) then
        local val = OPTIONS[idx].change(-1)
        Speech.say(OPTIONS[idx].name() .. ", " .. val, true)
    end

    prev_btn = snap.buttons
end

-- ---- loop management --------------------------------------------------------

function Menu.is_open() return open end

function Menu.start()
    if running then return end
    if not Input.is_loaded() then
        print("[KakarotAccess] config_menu: input bridge not loaded, disabled\n")
        return
    end
    running = true
    PadPoll.register("config_menu", step, force_release)
end

function Menu.stop()
    running = false
    PadPoll.unregister("config_menu")
    force_release()
end

return Menu
