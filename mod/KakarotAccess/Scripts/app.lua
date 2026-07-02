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
local Keyhelp = require("keyhelp")
local HeaderReader = require("header_reader")
local Speech = require("speech")
local I18n = require("i18n")

-- Register most-specific screens FIRST: overlays win over the screens beneath them,
-- which can still report visible underneath (the dispatcher picks the first active
-- adapter). Order: choice-list > dialog > pause > dialogue > loading > tutorial >
-- options > shop > title. Notes on the ordering:
--  * the choice list precedes the dialog: the difficulty window shows both a Txt_Detail
--    prompt (dialog) and the option list, and the list reading is the useful one;
--  * a confirmation dialog can pop over the pause menu, so dialog precedes pause;
--  * pause precedes dialogue/loading/tutorial: the battle pause widget stays RESIDENT as
--    HitTestInvisible all battle (its rows would keep the tutorial guide "active" and shadow
--    the pause), so the pause adapter gates on being genuinely open (enum 0) and, being above
--    them, wins when open — while staying dormant (so tutorial/subtitles read) when closed;
--  * dialogue (story subtitles / NPC talk) sits above the info overlays so a line beats an
--    idle loading/tutorial screen.
Registry.register(require("screen_choicelist"))
Registry.register(require("screen_dialog"))
Registry.register(require("screen_pause"))
Registry.register(require("screen_dialogue"))
Registry.register(require("screen_tips"))
Registry.register(require("screen_loading"))
Registry.register(require("screen_tutorial"))
Registry.register(require("screen_options"))
Registry.register(require("screen_shop"))
Registry.register(require("screen_field"))
Registry.register(require("screen_title"))

local App = {}

function App.start() Registry.start() end
function App.stop() Registry.stop() end
function App.repeat_current() Registry.repeat_current() end

-- Toggle the menu reader and announce the new state in the game's language. Announced
-- here (not in main.lua) so the i18n layer stays reloadable.
function App.toggle()
    local on = Registry.toggle()
    Speech.say(I18n.t(on and "reader_on" or "reader_off"), true)
    return on
end

-- Read the on-screen contextual button prompts (the keyhelp bar) on demand.
-- Touches live UObjects, so it runs on the game thread.
function App.read_keyhelp() ExecuteInGameThread(Keyhelp.announce) end

-- Announce an overworld menu section from its EXCmnHeaderFontType enum value. Called by the
-- SetFontType hook in main.lua (already on the game thread, already pcall-guarded there).
-- Kept here so the enum→name mapping stays reloadable while the hook itself does not.
function App.header_font(ft) HeaderReader.announce(ft) end

return App
