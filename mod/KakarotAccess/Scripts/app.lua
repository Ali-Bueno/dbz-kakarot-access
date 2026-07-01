-- App controller: registers the screen adapters and drives the UI registry.
--
-- main.lua (the bootstrap, loaded once) wires keybinds that delegate here. This
-- module and everything it requires (ui_core, ui_archetypes, ui_registry, the
-- screen_* adapters) are reloadable at runtime via Ctrl+Shift+R; only the PRISM
-- bridge (speech/prism_bridge) survives a reload.
--
-- Adding a new screen = write a screen_<name>.lua adapter (is_active/update/reset)
-- reusing the archetype readers, then register it below. No engine changes.

local Registry = require("ui_registry")

-- Register most-specific screens FIRST: a submenu (Options) opens on top of the
-- title, and the base title widget can still report visible underneath, so the
-- registry must prefer the submenu. The title is the base/fallback → registered
-- last. (The dispatcher picks the first active adapter.)
Registry.register(require("screen_options"))
Registry.register(require("screen_title"))

local App = {}

function App.start() Registry.start() end
function App.stop() Registry.stop() end
function App.toggle() return Registry.toggle() end
function App.repeat_current() Registry.repeat_current() end

return App
