-- Offline tests for cinema.lua: the scan-free movie probe and its debounced flag.

local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local clock = 0
os.clock = function() return clock end

local gameplay = false
local in_transition = false
local on_begin = {}
local lib_calls = 0
local playing = false

local function stub(name, members)
    package.preload[name] = function() return members end
end

stub("ui_core", {
    valid = function(o) return type(o) == "table" and o.valid ~= false end,
    member = function(o, name) return o[name] end,
})
stub("transition", {
    active = function() return in_transition end,
    on_begin = function(name, fn) on_begin[name] = fn end,
})
stub("ui_directory", {
    root_cached = function(key)
        assert(key == "mm", "cinema asked for an unexpected root")
        return gameplay
    end,
})

local player = { IsPlaying = function() return playing end }
local actor = { ManaPlayer = player }
local ctrl = { DemoActor = actor }
local comp = { GetPreRenderDemoCtrl = function() return ctrl end }
_G.StaticFindObject = function()
    return { GetDemoManageComponent = function()
        lib_calls = lib_calls + 1
        return comp
    end }
end

local Cinema = require("cinema")

local fails = 0
local function check(cond, what)
    if cond then
        print("ok   - " .. what)
    else
        fails = fails + 1
        print("FAIL - " .. what)
    end
end

local function tick(dt)
    clock = clock + (dt or 0.1)
    return Cinema.update()
end

do
    playing = true
    local p, known = Cinema.movie()
    check(p == nil and known == false and lib_calls == 0,
        "outside a gameplay world the demo manager is never called")
end

do
    gameplay = true
    clock = clock + 2
    check(tick() == true, "a playing movie turns movie mode on at once")
    local p, known = Cinema.movie()
    check(p == player and known == true, "the probe hands back the movie's own player")
    local calls = lib_calls
    tick(); tick()
    check(lib_calls == calls, "the resolved controller is cached, not looked up every poll")
end

do
    playing = false
    check(tick(0.1) == true, "one 'not playing' poll does not drop movie mode")
    playing = true
    check(tick(0.1) == true, "a flicker inside the debounce keeps it on")
    playing = false
    tick(0.2); tick(0.2); tick(0.2)
    check(tick(0.2) == false, "a stop that persists past the debounce turns it off")
    local p, known = Cinema.movie()
    check(p == nil and known == true, "a reachable manager with no movie says 'known, none'")
end

do
    playing = true
    tick()
    actor.valid = false
    clock = clock + 1
    local p, known = Cinema.movie()
    check(p == nil and known == true, "a stale movie actor is rejected by the validity guard")
    actor.valid = nil
end

do
    playing = true
    tick()
    in_transition = true
    on_begin.cinema()
    check(Cinema.active() == false, "a map transition clears movie mode")
    local p, known = Cinema.movie()
    check(p == nil and known == false, "during a transition the probe does not look")
    in_transition = false
end

if fails > 0 then
    print(("%d check(s) FAILED"):format(fails))
    os.exit(1)
end
print("all checks passed")
