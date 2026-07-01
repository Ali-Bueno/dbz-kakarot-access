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

local MOD = "KakarotAccess"
print("[" .. MOD .. "] Lua loading...\n")

Speech.init()

-- Snapshot everything loaded so far (stdlib + speech + prism_bridge). These must
-- survive a reload; anything required AFTER this point is our own logic and is
-- cleared/re-required on reload. New feature modules are picked up automatically.
local protected = {}
for name in pairs(package.loaded) do protected[name] = true end

local App = require("app")

-- === Keybinds (registered once; they delegate to App so a reload takes effect) ==

-- F8: speech test through the full pipeline (announces which screen reader PRISM picked).
RegisterKeyBind(Key.F8, function()
    Speech.say("Kakarot accessibility online. Screen reader: " .. Speech.backend_name(), true)
end)

-- Ctrl+F8: silence the screen reader.
RegisterKeyBind(Key.F8, { ModifierKey.CONTROL }, function()
    Speech.stop()
end)

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

-- F1: diagnostic — speak the current title selection (or why nothing is read).
RegisterKeyBind(Key.F1, function() App.menu_read_current() end)

-- Ctrl+M: toggle the title menu reader on/off.
RegisterKeyBind(Key.M, { ModifierKey.CONTROL }, function()
    Speech.say(App.menu_toggle() and "Menu reader on" or "Menu reader off", true)
end)

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

-- Start all accessibility features.
App.start()

print("[" .. MOD .. "] Lua loaded. Accessibility active.\n")
