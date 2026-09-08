-- Regression: a free-cursor menu must explain its movement controls as well as actions.
local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local spoken, protected, entries = {}, false, {}
package.loaded.speech = {
    say = function(text, interrupt) spoken[#spoken + 1] = { text, interrupt } end,
    protected = function() return protected end,
}
package.loaded.ui_core = {}
package.loaded.ui_archetypes = {}
package.loaded.i18n = {}
local Keyhelp = require("keyhelp")
-- The live Unreal widget reads are the external boundary. Keep the real action
-- filter and phrase formatter, so the tests catch dropped or reordered controls.
Keyhelp.read = function(visible_only, tick)
    assert(visible_only == true and type(tick) == "number", "automatic help must use the live budgeted bar")
    return entries
end
Keyhelp.label_sig = function()
    local labels = {}
    for _, e in ipairs(entries) do labels[#labels + 1] = e.label end
    return table.concat(labels, "|")
end

local fails = 0
local function check(ok, name)
    print((ok and "ok   - " or "FAIL - ") .. name)
    if not ok then fails = fails + 1 end
end
local function start(adapter)
    package.loaded.keyhelp_watch = nil
    local watcher = require("keyhelp_watch")
    spoken, protected = {}, false
    entries = {
        {button = "Left stick", label = "Move cursor", nav = true},
        {button = "A", label = "Confirm", nav = false},
        {button = "B", label = "Back", nav = false},
    }
    watcher.screen_changed(adapter)
    return watcher
end
local function poll(watcher, n)
    for _ = 1, n or 10 do watcher.update() end
end

local w = start({})
poll(w)
check(#spoken == 1 and spoken[1][1] == "A: Confirm, B: Back",
    "ordinary menus retain action-only help")

w = start({keyhelp_navigation = true})
poll(w)
check(#spoken == 1 and spoken[1][1] == "Left stick: Move cursor, A: Confirm, B: Back",
    "free-cursor menus announce movement and actions in visible order")
check(#spoken == 1 and spoken[1][2] == false, "control help does not interrupt selection speech")
poll(w, 20)
check(#spoken == 1, "an unchanged control bar stays quiet")
entries[2].label = "Place emblem"
poll(w)
check(#spoken == 2 and spoken[2][1] == "Left stick: Move cursor, A: Place emblem, B: Back",
    "a placement action change announces the new controls")

w = start({keyhelp_navigation = true})
protected = true
poll(w, 20)
check(#spoken == 0, "tutorial instructions retain speech priority")
protected = false
poll(w)
check(#spoken == 1 and spoken[1][1]:find("Move cursor", 1, true) ~= nil,
    "deferred movement help is announced when the instruction finishes")

w = start({keyhelp_auto = false, keyhelp_navigation = true})
poll(w, 20)
check(#spoken == 0, "passive-dialog opt-out still suppresses automatic help")

w = start({keyhelp_navigation = true})
w.screen_changed({})
poll(w)
check(#spoken == 1 and spoken[1][1] == "A: Confirm, B: Back",
    "leaving a free-cursor menu restores action-only help")

if fails > 0 then os.exit(1) end
print("all checks passed")
