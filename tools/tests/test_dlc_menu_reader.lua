-- Behavioral regression coverage for the downloadable-content menu reader.
--
-- The adapter itself is real. Only the UE4SS-facing ui_core and localization
-- boundaries are replaced so this can run under the vendored desktop Lua.

local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local fails = 0
local function check(cond, what)
    if cond then
        print("ok   - " .. what)
    else
        fails = fails + 1
        print("FAIL - " .. what)
    end
end

local current_host = nil
local requested_class = nil
local last_focus = nil
local reset_count = 0
local speak_count = 0
local in_free_roam = false
local pane_is_rendered = true
local pane_is_live = true

local Core = {}
function Core.first_on_screen(class_name)
    requested_class = class_name
    return current_host
end

function Core.free_roam() return in_free_roam end
function Core.pane_rendered() return pane_is_rendered end
function Core.pane_live() return pane_is_live and pane_is_rendered end

function Core.first_text(owner, ...)
    if not owner then return nil end
    for i = 1, select("#", ...) do
        local value = owner.texts[select(i, ...)]
        if value and value ~= "" then return value end
    end
    return nil
end

function Core.phrase(...)
    local parts = {}
    for i = 1, select("#", ...) do
        local value = select(i, ...)
        if value and value ~= "" then parts[#parts + 1] = value end
    end
    return table.concat(parts, ", ")
end

function Core.make_announcer()
    local open = false
    local last_tab, last_name, last_value = nil, nil, nil

    local function record(screen, tab, name, value, tooltip)
        speak_count = speak_count + 1
        last_focus = {
            screen = screen,
            tab = tab,
            name = name,
            value = value,
            tooltip = tooltip,
        }
    end

    return {
        focus = function(_, screen, tab, name, value, tooltip)
            if not open then
                open = true
                last_tab, last_name, last_value = tab, name, value
                record(screen, tab, name, value, tooltip)
            elseif tab ~= last_tab then
                last_tab, last_name, last_value = tab, name, value
                if tab then record(screen, tab, name, value, tooltip) end
            elseif name ~= last_name then
                last_name, last_value = name, value
                record(screen, tab, name, value, tooltip)
            elseif value ~= last_value then
                last_value = value
                if value then record(screen, tab, name, value, tooltip) end
            end
        end,
        reset = function()
            reset_count = reset_count + 1
            open = false
            last_tab, last_name, last_value = nil, nil, nil
        end,
    }
end

package.preload.ui_core = function() return Core end
package.preload.i18n = function()
    return {
        startlist = function(id)
            if id == 14 then return "Downloadable content" end
            return nil
        end,
    }
end

local loaded, Dlc = pcall(require, "screen_dlc")
check(loaded, "the DLC screen adapter module exists")

if loaded then
    current_host = { texts = {
        Txt_Cap00 = "Downloadable content",
        Txt_Title = "A New Power Awakens",
        Txt_Story = "Fight Beerus",
        Txt_Ditaile = "Raises the level cap",
        Txt_State = "Installed",
    } }
    last_focus = nil
    check(Dlc.confirm_ticks == 1, "the deliberate full-screen menu activates without an extra delay")
    check(Dlc.is_active() == true, "the live DLC host claims the screen")
    check(requested_class == "Start_DLC_C", "the adapter resolves the confirmed live blueprint class")
    Dlc.update()
    check(last_focus == nil, "the first repaint poll does not speak a potentially stale pane")
    check(Dlc.is_active() == true, "the repainting pane keeps ownership on its second poll")
    Dlc.update()
    check(last_focus == nil, "the second repaint poll remains silent")
    check(Dlc.is_active() == true, "the stable pane keeps ownership on its third poll")
    Dlc.update()
    check(last_focus and last_focus.screen == "Downloadable content", "entry names the menu")
    check(last_focus and last_focus.tab == nil, "the DLC screen does not invent a tab")
    check(last_focus and last_focus.name == "A New Power Awakens", "entry names the visible selection")
    check(last_focus and last_focus.value == "Installed", "entry includes the visible state")
    check(last_focus and last_focus.tooltip and
        last_focus.tooltip() == "Fight Beerus, Raises the level cap",
        "entry includes visible story and details")

    local steady_speaks, steady_resets = speak_count, reset_count
    for _ = 1, 5 do
        check(Dlc.is_active() == true, "an unchanged settled pane remains active")
        Dlc.update()
    end
    check(speak_count == steady_speaks, "an unchanged settled pane is not re-announced on every poll")
    check(reset_count == steady_resets, "the steady state never resets the shared announcer")

    current_host = { texts = { Txt_Cap00 = "Downloadable content" } }
    Dlc.reset()
    last_focus = nil
    check(Dlc.is_active() == true, "a live host claims the screen while its title is still loading")
    Dlc.update()
    check(last_focus == nil, "an unreadable title never fabricates a selection")

    current_host = { texts = { Txt_Title = "Trunks: The Warrior of Hope" } }
    Dlc.reset()
    last_focus = nil
    check(Dlc.is_active() == true, "a title remains readable without optional fields")
    Dlc.update()
    check(Dlc.is_active() == true, "the title-only pane remains active while settling")
    Dlc.update()
    check(Dlc.is_active() == true, "the title-only pane reaches a stable third poll")
    Dlc.update()
    check(last_focus and last_focus.screen == "Downloadable content",
        "the localized heading is the fallback when the visible caption is absent")
    check(last_focus and last_focus.name == "Trunks: The Warrior of Hope",
        "missing optional fields do not suppress the title")
    check(last_focus and last_focus.value == nil, "missing state remains absent")
    check(last_focus and last_focus.tooltip and last_focus.tooltip() == nil,
        "missing story and details produce no empty tooltip")

    current_host = { texts = {
        TxtCap00 = "DLC",
        TxtTitle = "Bardock: Alone Against Fate",
        TxtSummary = "The story of Goku's father",
        TxtDetails = "A new story arc",
    } }
    Dlc.reset()
    last_focus = nil
    check(Dlc.is_active() == true, "native text aliases remain a guarded fallback")
    Dlc.update()
    check(Dlc.is_active() == true, "native aliases remain active while settling")
    Dlc.update()
    check(Dlc.is_active() == true, "native aliases reach a stable third poll")
    Dlc.update()
    check(last_focus and last_focus.screen == "DLC", "the native caption fallback is read")
    check(last_focus and last_focus.name == "Bardock: Alone Against Fate",
        "the native title fallback is read")
    check(last_focus and last_focus.tooltip and
        last_focus.tooltip() == "The story of Goku's father, A new story arc",
        "native story and detail fallbacks are read")

    current_host = { texts = {
        Txt_Title = "Goku's Next Journey",
        Txt_Story = "Ten years later",
    } }
    last_focus = nil
    check(Dlc.is_active() == true, "a changed visible selection remains active")
    Dlc.update()
    check(last_focus == nil, "a changed title waits for its detail pane to repaint")
    check(Dlc.is_active() == true, "the changed selection keeps ownership while settling")
    Dlc.update()
    check(last_focus == nil, "the changed selection remains silent on its second poll")
    check(Dlc.is_active() == true, "the changed selection reaches a stable third poll")
    Dlc.update()
    check(last_focus and last_focus.name == "Goku's Next Journey",
        "selection movement forwards the newly visible title")

    last_focus = nil
    check(type(Dlc.reannounce) == "function", "the adapter exposes a repeat path that preserves settled state")
    if type(Dlc.reannounce) == "function" then
        Dlc.reannounce()
        check(Dlc.is_active() == true, "F1 repeat keeps the current selection active")
        Dlc.update()
        check(last_focus and last_focus.name == "Goku's Next Journey",
            "F1 repeats immediately without waiting through selection settling again")
    end

    pane_is_live = false
    pane_is_rendered = true
    check(Dlc.is_active() == false, "a parked interactive DLC pane cannot shadow the ring")
    pane_is_live = true

    in_free_roam = true
    check(Dlc.is_active() == false, "the minimap signal releases a resident DLC pane after closing")
    in_free_roam = false

    pane_is_rendered = false
    check(Dlc.is_active() == false, "a fully faded DLC pane cannot shadow later menus")
    pane_is_rendered = true

    current_host = nil
    check(Dlc.is_active() == false, "the adapter releases when the DLC screen closes")
    local before = reset_count
    Dlc.reset()
    check(reset_count == before + 1, "reset delegates to the shared announcer")
end

-- ---- directory integration -------------------------------------------------
-- Exercise the real ui_directory resolver. This fake graph mirrors the complete
-- ownership path established by the live header/object dump; only the UE4SS root
-- scan itself is replaced.

Core.POLL_MS = 100
Core.valid = function(o) return type(o) == "table" and o.live == true end
Core.member = function(o, name) return o and o[name] or nil end
Core.take_scan_slot = function() return true end
Core.begin_scan_tick = function() end
Core.on_screen = function(o) return Core.valid(o) end

local function object(full_name, address, fields)
    local o = fields or {}
    o.live = true
    function o:GetFullName() return full_name end
    function o:GetAddress() return address end
    return o
end

local live_widget = object("Start_DLC_C /Engine/Transient.Start_DLC_C_1", 0x3000)
local dlc_menu = object("DlcMenu /Engine/Transient.DlcMenu_0", 0x2000, {
    m_UIStartDlc = live_widget,
})
local menu_manager = object("MenuManager /Engine/Transient.MenuManager_0", 0x1000, {
    m_xDlcMenu = dlc_menu,
})
local root_scans = 0
_G.FindAllOf = function(class_name)
    root_scans = root_scans + 1
    if class_name == "MenuManager" then return { menu_manager } end
    return {}
end

package.loaded.ui_directory = nil
package.preload.transition = function()
    return {
        active = function() return false end,
        on_begin = function() end,
    }
end
package.preload.mem = function()
    return { raw_addr = function(o) return o:GetAddress() end }
end

local Directory = require("ui_directory")
local resolved = Directory.resolve("Start_DLC_C")
check(resolved and #resolved == 1 and resolved[1] == live_widget,
    "the directory resolves the exact manager-owned DLC widget")
check(root_scans == 1, "the DLC directory path needs only the cached MenuManager root scan")

-- ---- registry integration --------------------------------------------------
-- Load the real app.lua while replacing each feature module at its external
-- boundary. Registry order is the behavior under test: the DLC screen must own
-- the foreground before screen_field can announce the world underneath it.

local registrations = {}
package.loaded.app = nil
package.loaded.ui_registry = nil
package.preload.ui_registry = function()
    return {
        register = function(adapter, name)
            registrations[#registrations + 1] = { adapter = adapter, name = name }
        end,
    }
end

local empty_modules = {
    "keyhelp", "keyhelp_watch", "header_reader", "speech", "nav_tracker",
    "radar_menu", "config_menu", "settings", "battle_monitor",
}
for _, name in ipairs(empty_modules) do
    package.loaded[name] = nil
    package.preload[name] = function() return {} end
end

package.loaded.quest_objective = nil
package.preload.quest_objective = function()
    return {
        set_on_change = function() end,
        set_on_gone = function() end,
    }
end
package.loaded.native_offsets = nil
package.preload.native_offsets = function()
    return { itemMenu = { hasItems = 0x620 } }
end
package.loaded.screen_list = nil
package.preload.screen_list = function()
    return { new = function() return {} end }
end

-- Preloaded modules above win first. Every other screen adapter is inert here;
-- app.lua's explicit registration names and order remain real.
table.insert(package.searchers, 2, function(name)
    if name:match("^screen_") then
        return function() return { test_name = name } end
    end
    return nil
end)

local app_loaded = pcall(require, "app")
check(app_loaded, "the real app registry loads under the test boundary")
local dlc_index, field_index, dlc_count = nil, nil, 0
for i, entry in ipairs(registrations) do
    if entry.name == "screen_dlc" then dlc_index, dlc_count = i, dlc_count + 1 end
    if entry.name == "screen_field" then field_index = i end
end
check(dlc_count == 1, "the DLC adapter is registered exactly once")
check(dlc_index and field_index and dlc_index < field_index,
    "the DLC menu owns the foreground before the field reader")

if fails > 0 then
    print(("%d check(s) FAILED"):format(fails))
    os.exit(1)
end
print("all checks passed")
