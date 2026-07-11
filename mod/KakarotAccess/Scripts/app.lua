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
local Nav = require("nav_tracker")
local RadarMenu = require("radar_menu")
local Battle = require("battle_monitor")
local QuestObjective = require("quest_objective")

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
-- Field Memory / story-recap viewer (Field_Memory_C): a titled paged narration overlay.
-- High priority — it's a full-screen reader that pauses the game.
Registry.register(require("screen_memory"))
-- Story results (Quest_Main_Clear_C): the post-battle rank board. High priority — a
-- full-screen sequence; reads incrementally following the game's reveal animation.
Registry.register(require("screen_results"))
Registry.register(require("screen_dialog"))
Registry.register(require("screen_pause"))
-- Fishing/minigame/QTE prompts sit above the dialogue reader: their button prompts
-- are time-critical and a subtitle line must not shadow them.
Registry.register(require("screen_fishing"))
-- Dialogue choices sit ABOVE the line reader: when an NPC question shows options,
-- the choice (prompt + hovered option) must win over the lingering talk line.
Registry.register(require("screen_choice"))
Registry.register(require("screen_dialogue"))
-- Telop banners sit BELOW dialogue: a subtitle wins the tick and the banner (still
-- pending while on screen) speaks right after it, queued.
Registry.register(require("screen_telop"))
Registry.register(require("screen_tips"))
Registry.register(require("screen_loading"))
local Tutorial = require("screen_tutorial")
Registry.register(Tutorial)
Registry.register(require("screen_options"))
-- Area map + world map (free-cursor screens; hovered/current area name readout).
-- Kept in a local: its fast travel-selection loop is started/stopped with the app.
local MapScreen = require("screen_map")
Registry.register(MapScreen)
-- Cooking precedes the shop: the cooking menu embeds a UAT_UIShopTop (WL_CookingTop),
-- so the shop adapter could latch onto it while the cooking screen is the real context.
-- The Shop_Top mode/action list ("Make a dish / Make a full-course meal", shop Buy/Sell):
-- reads ListBarCtn (Xlist_Bar01 rows). Registered BEFORE screen_cooking AND screen_shop:
-- during the cooking course-select, Shop_Cook_C is already on screen with a stale recipe
-- index, so screen_cooking would otherwise activate and shadow this. Its rows go
-- Collapsed (on_screen=false) once you pick a mode, so it stays inactive over the recipe
-- list and shop item list — no overlap with those readers.
-- Super Attack training shop (Shop_Training_C) — talking to a training NPC (Krillin).
-- Registered BEFORE the shop group: the training menu opens OVER the shop-top, which can
-- stay on_screen underneath, so training (the foreground screen) must win. It gates on
-- Shop_Training_C being on_screen, so it never activates over a regular shop.
Registry.register(require("screen_training"))
-- Item/info BUY lists open OVER the shop-top (which can stay on_screen underneath), so
-- they must precede screen_shoplist/screen_shop:
--   Shop_Cmn_C  — the food/material item list (name, price, purchase count, owned);
--   Shop_Info_C — the information store (map-location intel: fishing/hunting/... spots).
Registry.register(require("screen_shopcmn"))
Registry.register(require("screen_shopinfo"))
Registry.register(require("screen_shoplist"))
Registry.register(require("screen_cooking"))
Registry.register(require("screen_shop"))
-- Overworld ring submenus that open their own screen (native class, no _C). The two list
-- screens share the generic MenuListBase factory; Characters/Party use their own getter/field.
local ListScreen = require("screen_list")
-- Items / Inventory. Class = the BLUEPRINT name Start_Item_C: FindAllOf on the native
-- base AT_UIItemMenu finds nothing on this game (community-board lesson), which kept
-- the inventory mute (F7 dump 2026-07-06). Xmenu_List00 is a Cmn01MenuList09 ->
-- MenuListBase01, whose TxtTitle carries the category tab ("Recovery").
-- Item palette registration (X in the inventory) opens OVER the Items list, which
-- stays on_screen underneath — the palette must precede it.
Registry.register(require("screen_palette"))
-- Detail pane (blueprint-only nodes, subtree-scanned): sell price, main location,
-- item info + description as the tooltip; Txt_Title00 (the pane's item title) is the
-- LIVE selection name — the list's reflected index freezes at 0 on this screen.
Registry.register(ListScreen.new("Start_Item_C", "Xmenu_List00",
    function() return I18n.startlist(2) end, "TxtTitle",
    { "Txt_Cap00", "Txt_Detail00", "Txt_Cap01", "Txt_Detail01",
      "Txt_Cap02", "Txt_Detail02", "Txt_Detail03" }, "Txt_Title00"))
Registry.register(ListScreen.new("AT_UIStartDragonBallMenu", "UICmn00MenuList",
    function() return I18n.startlist(1) end))                       -- Dragon Balls
Registry.register(require("screen_characters"))                    -- Characters
Registry.register(require("screen_party"))                         -- Party
-- Save / Load data-slot menu (one adapter for both — same native AT_UIStartSaveLoad).
Registry.register(require("screen_saveload"))
Registry.register(require("screen_tutorials"))                     -- System > Tutorials list
Registry.register(require("screen_community"))                     -- Community / Soul Emblems
Registry.register(require("screen_field"))
-- Bottom-of-stack NOTICE readers (speak once + release; never hold the screen):
-- post-battle result overlay (rank + EXP) and the gameplay toasts (item log,
-- level-ups). Any real menu above outranks them.
Registry.register(require("screen_battleresult"))
Registry.register(require("screen_toasts"))
Registry.register(require("screen_title"))

local App = {}

function App.start()
    Registry.start()
    Nav.start()
    RadarMenu.start()
    Battle.start()
    MapScreen.start()
    QuestObjective.start()
end

function App.stop()
    Registry.stop()
    Nav.stop()
    RadarMenu.stop()
    Battle.stop()
    MapScreen.stop()
    QuestObjective.stop()
end

-- F1: repeat the active screen's focus — or, with no screen owning the tick (the
-- resident controls guide RELEASES it after announcing once), re-arm the guide so
-- F1 in the open world re-reads the controls.
function App.repeat_current()
    if Registry.active_adapter() then
        Registry.repeat_current()
    else
        Tutorial.reannounce()
    end
end

-- Navigation radar (nav_tracker.lua) — keybinds in main.lua delegate here so the
-- whole tracker stays hot-reloadable.
function App.nav_toggle() Nav.toggle() end
function App.nav_route_toggle() Nav.toggle_route() end
function App.nav_where() Nav.where() end
function App.nav_companion() Nav.cycle_companion() end
function App.nav_dump() Nav.dump() end

-- F10: read the current quest objective text on demand (kept here so quest_objective
-- stays hot-reloadable; the keybind in main.lua only delegates).
function App.read_objective() QuestObjective.read() end

-- Toggle the menu reader and announce the new state in the game's language. Announced
-- here (not in main.lua) so the i18n layer stays reloadable.
function App.toggle()
    local on = Registry.toggle()
    Speech.say(I18n.t(on and "reader_on" or "reader_off"), true)
    return on
end

-- Read the on-screen contextual button prompts (the keyhelp bar) on demand.
-- Touches live UObjects, so it runs on the game thread — and stays inert during a
-- level transition (same teardown-abort risk as the loops).
function App.read_keyhelp()
    ExecuteInGameThread(function()
        if require("transition").active() then return end
        Keyhelp.announce()
    end)
end

-- Announce an overworld menu section from its EXCmnHeaderFontType enum value. Called by the
-- SetFontType hook in main.lua (already on the game thread, already pcall-guarded there).
-- Kept here so the enum→name mapping stays reloadable while the hook itself does not.
function App.header_font(ft) HeaderReader.announce(ft) end

return App
