-- KakarotAccess — bootstrap (loaded once, never reloaded).
--
-- Responsibilities: init the PRISM bridge, register the keybinds, and hand all
-- accessibility logic to app.lua. Everything app.lua requires is reloadable at
-- runtime via Ctrl+Shift+R (see below), so we can iterate on the whole mod from
-- inside the game. Only the speech bridge survives a reload.
--
-- discover.lua stays in the repo as the dev tool for mapping future screens; it
-- is intentionally NOT wired here so the shipping mod stays lean.

local Speech = require("speech")
local Mem = require("mem")
local Audio = require("audio")
local Input = require("input")
local Settings = require("settings")
local Build = require("build_flags")

local MOD = "KakarotAccess"
print("[" .. MOD .. "] Lua loading...\n")

Speech.init()
-- Native memory reader (mem_bridge.dll) for the non-reflected menu selection indices.
-- Loaded here (before the protected snapshot) so it survives Ctrl+Shift+R like the
-- speech bridge. Kept SEPARATE from prism_bridge (screen reader only).
Mem.init()
-- Native audio-cue player (audio_bridge.dll, XAudio2) for the navigation radar.
-- Same lifecycle as the other bridges: loaded once, survives hot reloads.
Audio.init()
-- Native gamepad reader + game-input blocker (input_bridge.dll, XInput IAT hook) for
-- the R3 radar target picker. Installed once (the IAT hook must NOT be reinstalled
-- on a reload), so it lives before the protected snapshot like the other bridges.
Input.init()
-- User config (settings.lua → Scripts/config.txt): audio-cue master switch + volume,
-- radar auto-activation, language override. Loaded here (before the protected snapshot)
-- so the in-memory values survive a Ctrl+Shift+R reload; the config menu writes changes
-- straight to disk. app.start() re-applies the language override to the (reloaded) i18n.
Settings.init()

-- Snapshot everything loaded so far (stdlib + speech + prism_bridge + mem_bridge). These must
-- survive a reload; anything required AFTER this point is our own logic and is
-- cleared/re-required on reload. New feature modules are picked up automatically.
local protected = {}
for name in pairs(package.loaded) do protected[name] = true end

local App = require("app")

-- === Keybinds (registered once; they delegate to App so a reload takes effect) ==
-- These are accessibility conveniences; the mod is designed to be played with a
-- CONTROLLER (see the radar picker, config menu, status stepping, etc.). The
-- developer/diagnostic keys are gathered at the bottom behind Build.debug and do
-- NOT ship in releases.

-- F8: speech test through the full pipeline (announces which screen reader PRISM picked).
RegisterKeyBind(Key.F8, function()
    Speech.say("Kakarot accessibility online. Screen reader: " .. Speech.backend_name(), true)
end)

-- Ctrl+F8: silence the screen reader.
RegisterKeyBind(Key.F8, { ModifierKey.CONTROL }, function()
    Speech.stop()
end)

-- F1: repeat the currently focused menu item.
RegisterKeyBind(Key.F1, function() App.repeat_current() end)

-- F2: read the on-screen button prompts (the contextual keyhelp bar) in full.
RegisterKeyBind(Key.F2, function() App.read_keyhelp() end)

-- Ctrl+F2: toggle the AUTOMATIC announcement of a screen's actions ("X: assign to a slot",
-- "Y: skill tree"), spoken on entering a menu and whenever they change. F2 still works.
RegisterKeyBind(Key.F2, { ModifierKey.CONTROL }, function() App.keyhelp_auto_toggle() end)

-- F3: toggle the quest navigation radar (off = immediate silence).
RegisterKeyBind(Key.F3, function() App.nav_toggle() end)

-- Shift+F3: toggle NavMesh route guidance (beacon follows path corners vs straight line).
RegisterKeyBind(Key.F3, { ModifierKey.SHIFT }, function() App.nav_route_toggle() end)

-- F5: announce the tracked objective on demand (type, distance, clock direction).
RegisterKeyBind(Key.F5, function() App.nav_where() end)

-- Shift+F5: cycle companion tracking (nearest party member -> next -> quest objective).
RegisterKeyBind(Key.F5, { ModifierKey.SHIFT }, function() App.nav_companion() end)

-- F11 / Shift+F11: on the character status page (confirm a character in Characters), step
-- forward/back through the stat blocks — HP, Ki and the five attributes — reading each as its
-- total plus the breakdown (base, state boost, food effect). Entering the page already speaks
-- the header (name, level, EXP to next, HP, Ki, BP); these keys are the detail on demand.
-- The GAMEPAD does the same with d-pad down/up (screen_status.lua) — no keyboard needed.
RegisterKeyBind(Key.F11, function() App.status_step(1) end)
RegisterKeyBind(Key.F11, { ModifierKey.SHIFT }, function() App.status_step(-1) end)

-- Ctrl+M: toggle the menu reader on/off (App.toggle announces the state, localized).
RegisterKeyBind(Key.M, { ModifierKey.CONTROL }, function() App.toggle() end)

-- Ctrl+Shift+R: reload the WHOLE mod's logic from inside the game (no UE4SS
-- console needed). We stop all features, drop every non-protected module from
-- the require cache, then re-require app.lua (which re-requires its features
-- fresh) and start again. The PRISM bridge is untouched; leftover poll loops
-- stop themselves via each feature's generation guard.
RegisterKeyBind(Key.R, { ModifierKey.CONTROL, ModifierKey.SHIFT }, function()
    App.stop()
    for name in pairs(package.loaded) do
        if not protected[name] then package.loaded[name] = nil end
    end
    App = require("app")
    App.start()
    Speech.say("Mod reloaded", true)
end)

-- === Developer / diagnostic keybinds — NOT shipped in releases =================
-- build_flags.debug is true in the dev tree and false in packaged releases
-- (package.ps1 rewrites build_flags.lua and drops the dev-only modules), so none
-- of these register for players.
if Build.debug then
    -- F9: read the player's world position (pipeline test for live UObject reads).
    RegisterKeyBind(Key.F9, function()
        ExecuteInGameThread(function()
            local pc = FindFirstOf("PlayerController")
            if not pc or not pc:IsValid() then Speech.say("No player controller", true) return end
            local pawn = pc.Pawn
            if not pawn or not pawn:IsValid() then Speech.say("No player pawn", true) return end
            local loc = pawn:K2_GetActorLocation()
            Speech.say(string.format("X %d, Y %d, Z %d",
                math.floor(loc.X + 0.5), math.floor(loc.Y + 0.5), math.floor(loc.Z + 0.5)), true)
        end)
    end)

    -- F10: read the current quest objective text on demand (also auto-announced on change).
    RegisterKeyBind(Key.F10, function() App.read_objective() end)

    -- Ctrl+F5: dump the guidance candidates + a NavMesh probe to dumps/dump_nav_targets.txt.
    RegisterKeyBind(Key.F5, { ModifierKey.CONTROL }, function() App.nav_dump() end)

    -- Ctrl+Shift+F5: dump the level-offset hunt windows to dumps/dump_enemy_level.txt.
    RegisterKeyBind(Key.F5, { ModifierKey.CONTROL, ModifierKey.SHIFT }, function() App.nav_dump_levels() end)

    -- F6: diagnostic — SetFontType section hook status + fire count.
    RegisterKeyBind(Key.F6, function()
        local ok, s = pcall(function() return require("header_hook").status() end)
        Speech.say(ok and s or "hook status unavailable", true)
    end)

    -- F4: runtime memory diff — finds a non-reflected selection offset (fresh each press).
    RegisterKeyBind(Key.F4, function()
        package.loaded.dev_memdiff = nil
        require("dev_memdiff").run()
    end)

    -- F7: discovery — re-requires discover.lua fresh each press and runs its current step.
    RegisterKeyBind(Key.F7, function()
        package.loaded.discover = nil
        require("discover").run()
        Speech.say("Discover", true)
    end)
end

-- Global transition gate: a new-UWorld notify flags "map switch in progress" so every
-- loop goes inert and every UObject cache is flushed before any tick could probe a
-- freed object of the old level (an uncatchable abort — the recurring return-to-title
-- / post-cutscene crash). NOT a LoadMap hook: that trampoline crashes this game (see
-- transition.lua). Registered ONCE here (a reload must not re-register); the callback
-- reaches the current module instance through _G.
pcall(function()
    if require("transition").install() then
        print("[" .. MOD .. "] Transition gate (world notify) registered.\n")
    end
end)

-- Field-menu section reader — the mod's ONE RegisterHook, in its own file so it can be
-- disabled by simply deleting header_hook.lua (see that file). Registered ONCE here (a reload
-- must not re-register it); the whole thing is pcall'd so even a failure to install can't stop
-- the mod from loading. Needs a full game restart to take effect (main.lua isn't hot-reloaded).
pcall(function()
    if require("header_hook").install() then
        print("[" .. MOD .. "] Header section hook registered.\n")
    end
end)

-- Start all accessibility features.
App.start()

print("[" .. MOD .. "] Lua loaded. Accessibility active.\n")
