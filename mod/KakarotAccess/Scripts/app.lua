-- App controller: the single place that owns every accessibility feature.
--
-- main.lua (the bootstrap) is loaded once and never reloaded; it wires keybinds
-- that delegate here. This module and everything it requires (menu, and future
-- features like options/gameplay) ARE reloadable at runtime, so we can iterate
-- on the whole mod from inside the game without restarting. The PRISM bridge
-- (speech/prism_bridge) is deliberately NOT touched by a reload.

local Menu = require("menu")

local App = {}

-- Every feature module. Each should expose start()/stop(). Add new ones here as
-- the mod grows; the reload machinery in main.lua picks them up automatically.
local features = { Menu }

function App.start()
    for _, f in ipairs(features) do
        if f.start then f.start() end
    end
end

function App.stop()
    for _, f in ipairs(features) do
        if f.stop then f.stop() end
    end
end

-- Passthroughs for the dev/diagnostic keybinds registered in main.lua, so they
-- always hit the freshly reloaded code.
function App.menu_read_current()
    Menu.read_current()
end

function App.menu_toggle()
    if Menu.is_enabled() then
        Menu.stop()
        return false
    else
        Menu.start()
        return true
    end
end

return App
