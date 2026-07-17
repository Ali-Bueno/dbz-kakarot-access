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

local REL_TH = 25         -- trigger raw (0..255) below which it counts as released (drain)
local VOL_STEP = 10       -- cue-volume step per left/right press (percent)

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
end

local function force_release()
    if blocked then Input.block(false) end
    blocked, open, draining = false, false, false
    prev_btn = 0
    if _G.__KakarotPadModal == "config" then _G.__KakarotPadModal = nil end
end

-- ---- per-tick step (game thread) --------------------------------------------

local function step()
    if Transition.active() then force_release(); return end

    local snap = Input.read()
    if not snap then force_release(); return end

    -- Another overlay (the radar picker) owns the pad — stay out of its way.
    local modal = _G.__KakarotPadModal
    if modal and modal ~= "config" then prev_btn = snap.buttons; return end

    local function pressed(mask) return (snap.buttons & mask) ~= 0 and (prev_btn & mask) == 0 end

    if draining then
        if snap.buttons == 0 and snap.rt < REL_TH and snap.lt < REL_TH then
            Input.block(false); blocked = false; draining = false
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
