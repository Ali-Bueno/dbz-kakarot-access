-- Regression: an in-flight BOXED sweep build must not survive a world gate.
--
-- STATUS.md backlog: "Nav.SW.lists survives release_world_refs". The raw FindAllOf
-- results accumulated by a time-boxed build (Nav.SW.lists, partial=true) were not
-- dropped when the world gate closed — only targets_snap was — so a build interrupted
-- by a battle resumed on the other side re-walking pre-gate actor handles, and a
-- recycled address passes Core.valid. The map-transition flush (Transition.on_begin)
-- has the same contract for targets_snap and the same hole for the build, so both
-- lanes are pinned here.
--
-- Runs OFFLINE under the vendored Lua 5.4 (tools/run-lua-tests.ps1) with every
-- UE4SS-side module stubbed; nothing here touches the game.

local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

-- ---- stubs -----------------------------------------------------------------
-- Each stub raises on any member the harness has not declared, so a new dependency
-- fails with a NAME instead of "attempt to call a nil value" somewhere downstream.
local function stub(name, members)
    setmetatable(members, { __index = function(_, k)
        error(("harness stub '%s' has no member '%s' - declare it in the test"):format(name, k), 2)
    end })
    package.preload[name] = function() return members end
    return members
end

local PRE_GATE  = { "pre-gate actor handle" }   -- identity is the assertion
local POST_GATE = { "post-gate actor handle" }
local served = PRE_GATE

local Core = stub("ui_core", {
    findall = function() return { served } end,
})
stub("speech", {})
stub("audio", { stop = function() end })
stub("i18n", {})
stub("ui_registry", {})
stub("raycast", {})
stub("mem", { mark = function() end })
local transition_cb
stub("transition", {
    on_begin = function(_, fn) transition_cb = fn end,
})

local Nav = require("nav_tracker")

-- ---- helpers ---------------------------------------------------------------
local fails = 0
local function check(cond, what)
    if cond then
        print("ok   - " .. what)
    else
        fails = fails + 1
        print("FAIL - " .. what)
    end
end

-- Arrange an in-flight boxed build: open it the way Nav.list_targets does, scan one
-- class through the real code path so its handle list is cached, then age t0 so the
-- next class trips the time box and marks the build partial (explore_tick's chunked
-- resume state, exactly as a battle would interrupt it).
local function open_partial_build()
    served = PRE_GATE
    Nav.SW.lists = {}
    Nav.SW.t0, Nav.SW.partial, Nav.SW.boxed = os.clock(), false, true
    local got = Nav.SW.class_list("KakTestActorA")
    assert(got[1] == PRE_GATE, "harness: class_list must cache the scanned list")
    Nav.SW.t0 = os.clock() - (Nav.SW.BUDGET_S + 1)
    local cut = Nav.SW.class_list("KakTestActorB")
    assert(#cut == 0 and Nav.SW.partial, "harness: the time box must cut the build short")
    assert(Nav.sweep_partial() == true, "harness: a partial build stays open to resume")
end

-- ---- lane 1: the world gate (battle / cutscene / full-screen UI) -----------
open_partial_build()
Nav.SW.mgr.KakTestManager = PRE_GATE
served = POST_GATE
Nav.release_world_refs()
check(Nav.SW.lists == nil,
    "release_world_refs drops the in-flight build's scan lists")
check(Nav.SW.partial == false,
    "release_world_refs clears partial, so the next sweep restarts instead of resuming")
check(next(Nav.SW.mgr) == nil,
    "release_world_refs drops cached per-world manager handles")
local got = Nav.SW.class_list("KakTestActorA")
check(got[1] == POST_GATE and got[1] ~= PRE_GATE,
    "a post-gate scan serves fresh handles, never the pre-gate list")

-- ---- lane 2: the map-transition flush (LoadMap tears the level down) -------
check(type(transition_cb) == "function",
    "nav_tracker registered its map-transition flush")
open_partial_build()
Nav.SW.mgr.KakTestManager = PRE_GATE
served = POST_GATE
transition_cb()
check(Nav.SW.lists == nil,
    "the map-transition flush drops the in-flight build's scan lists")
check(Nav.SW.partial == false,
    "the map-transition flush clears partial")
check(next(Nav.SW.mgr) == nil,
    "the map-transition flush drops cached per-world manager handles")

if fails > 0 then
    print(("%d check(s) FAILED"):format(fails))
    os.exit(1)
end
print("all checks passed")
