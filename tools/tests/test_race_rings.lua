-- Offline tests for race_rings.lua: positions are scanned once per race (retried until the
-- course loads), deduped, archetypes skipped; the cue goes to the nearest ring ahead.
local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local fails = 0
local function check(ok, message)
    if ok then io.write("ok   - " .. message .. "\n")
    else fails = fails + 1; io.write("FAIL - " .. message .. "\n") end
end
local function module(name, value)
    package.preload[name] = function() return value end
    return value
end

local M = 100
local world = {}           -- class name -> list of fake actors
local scans = {}

local function actor(addr, x, y, z, name)
    return { valid = true, addr = addr, name = name or ("BP_BoostPoint_C /Game/Race.R_" .. addr),
        K2_GetActorLocation = function() return { X = x, Y = y, Z = z } end,
        GetFullName = function(self) return self.name end }
end

module("ui_core", {
    valid = function(o) return type(o) == "table" and o.valid == true end,
    findall = function(cls) scans[#scans + 1] = cls return world[cls] or {} end,
})
module("mem", { raw_addr = function(o) return o.addr end })
-- The ground under each ring: a fake downward trace answering from `ground` (x -> ground z).
local ground = {}
module("raycast", { probe_surface = function(_, x, _, sz)
    local g = ground[x]
    if not g then return false end
    return true, sz - g, 1
end })
print = function() end

local Rings = require("race_rings")

do
    Rings.reset()
    Rings.refresh(0)
    check(#scans == 2 and Rings.count() == 0, "course not loaded: both class names tried, nothing found")
    Rings.refresh(1)
    check(#scans == 2, "no rescan before the retry period")
    world.BP_BoostPoint_C = {
        actor(1, 3000, 0, 0), actor(1, 3000, 0, 0),
        actor(2, 0, 0, 0, "BP_BoostPoint_C /Game/Race.Default__BP_BoostPoint_C"),
        actor(3, 8000, 2000, 0),
    }
    Rings.refresh(3.5)
    check(Rings.count() == 2, "a later try finds the rings, deduped by address, archetype skipped")
    scans = {}
    Rings.refresh(10)
    check(#scans == 0, "known rings are not rescanned")
end

do
    world.BoostPointActor = { actor(9, 100, 0, 0) }
    world.BP_BoostPoint_C = { actor(10, 200, 0, 0) }
    Rings.reset()
    scans = {}
    Rings.refresh(0)
    check(#scans == 1 and Rings.count() == 1, "the native class answering skips the Blueprint scan")
    world.BoostPointActor = nil
end

do
    Rings._set({ { 3000, 0, 0 }, { 1000, 2000, 0 }, { -500, 0, 0 }, { 9000, 0, 0 } })
    local pan, dist = Rings.pick(0, 0, 0, 1, 0)
    check(pan and math.abs(pan) < 1e-6 and math.abs(dist - 30 * M) < 1,
        "the nearest ring AHEAD is cued; one off to the side and one behind are not")
    pan = Rings.pick(0, 0, 0, 0, -1)
    check(pan == nil, "facing away from all of them: nothing")
    Rings._set({ { 2000, 1000, 0 } })
    pan = Rings.pick(0, 0, 0, 1, 0)
    check(pan and pan > 0.3, "a ring ahead and to the right pans right")
    Rings._set({ { 7000, 0, 0 } })
    check(Rings.pick(0, 0, 0, 1, 0) == nil, "a ring past 60 m is not cued yet")
end

do
    local v1, i1 = Rings.cadence(5 * M)
    local v2, i2 = Rings.cadence(50 * M)
    check(v1 > v2 and i1 < i2, "a nearer ring cues louder and faster")
end

do
    -- Checkpoint 100 m straight ahead (+x). Rings: on the line 30 m out, 10 m to the side 20 m out,
    -- 30 m to the side, and one behind.
    Rings._set({ { 3000, 0, 0 }, { 2000, 1000, 0 }, { 2500, 3000, 0 }, { -1000, 0, 0 } })
    local x, y, z, d, i = Rings.on_route(0, 0, 0, 10000, 0, 0)
    check(i == 2 and math.abs(d - math.sqrt(2000 ^ 2 + 1000 ^ 2)) < 1,
        "on route: the nearest ring near the line to the checkpoint, not one far to the side")
    Rings._set({ { 3000, 6000, 0 } })
    check(Rings.on_route(0, 0, 0, 10000, 0, 0) == nil, "a ring costing a 59 m detour is not on the way")
    Rings._set({ { 3000, 2500, 0 } })
    check(Rings.on_route(0, 0, 0, 10000, 0, 0) ~= nil, "25 m off a straight line is still on the way")
    Rings._set({ { 5000, 4500, 0 } })
    check(Rings.on_route(0, 0, 0, 10000, 0, 0) == nil, "a 35 m detour is too much for a new ring")
    check(select(5, Rings.on_route(1000, 2000, 0, 10000, 0, 0)) == 1, "picked from nearer the line")
    check(select(5, Rings.on_route(0, 0, 0, 10000, 0, 0)) == 1,
        "the ring being steered to keeps a wider detour budget")
    check(Rings.on_route(5050, 4500, 0, 10000, 0, 0) == nil, "gone through: it drops out")
    check(Rings.on_route(0, 0, 0, 10000, 0, 0) == nil, "and is not picked again this race")
    Rings._set({ { 3000, 0, 0 } })
    check(Rings.on_route(0, 0, 0, 2000, 0, 0) == nil, "a ring beyond the checkpoint waits for the next leg")
    Rings._set({ { 14000, 0, 0 } })
    check(Rings.on_route(0, 0, 0, 30000, 0, 0) ~= nil, "a ring 140 m ahead is steered to")
    Rings._set({ { 21000, 0, 0 } })
    check(Rings.on_route(0, 0, 0, 30000, 0, 0) == nil, "one past the reach is not")
    Rings._set({ { 16000, 0, 0 } })
    check(Rings.nearest_unused(0, 0, 0) == 16000, "nearest unused ring distance for the log")

    -- Live geometry, Area03 robot race 2026-11-19 (cm): checkpoint 1 -> checkpoint 2, which is
    -- 36 m above ring 6; the boost zone is 24 m wide (inner radius 8.4 m read from the bounds).
    local cp2x, cp2y, cp2z = -187344, 156807, 8690
    Rings._set({ { -185800, 155000, 5100, 840, 100 } })
    check(select(5, Rings.on_route(-171143, 151614, 1708, cp2x, cp2y, cp2z)) == 1,
        "from checkpoint 1 the ring 149 m ahead is picked")
    check(select(5, Rings.on_route(-187800, 155600, 5100, cp2x, cp2y, cp2z)) == 1,
        "level with the checkpoint underneath it, 21 m beside the ring: still steering to it")
    check(Rings.on_route(-185300, 155000, 5100, cp2x, cp2y, cp2z) == nil,
        "5 m inside the boost zone: gone through")
    Rings._set({ { -185800, 155000, 5100, 840, 100 } })
    check(select(5, Rings.on_route(-171143, 151614, 1708, cp2x, cp2y, cp2z)) == 1, "picked again")
    check(Rings.on_route(-185800, 155000, 1500, cp2x, cp2y, cp2z) ~= nil,
        "36 m under the zone does not count as through it")
    check(Rings.on_route(-190000, 159500, 5100, cp2x, cp2y, cp2z) == nil,
        "left 50 m behind without going through: released")
end

do
    -- Launch pads (live 2026-11-20, Area03 robot race): booster 17 floats 11 m over the ground at
    -- (-1858, 1550); a ring on the ground and a 26 m second stage are not jumped into.
    world.BoostPointActor = {
        actor(40, -185781, 154958, 5106),   -- 11 m up: jump into it
        actor(41, 10000, 0, 150),           -- on the ground: walk through it
        actor(42, -188657, 154007, 6646),   -- 26 m up: a mid-air second stage
        actor(43, 50000, 0, 3000),          -- no ground found under it
    }
    world.BP_BoostPoint_C = nil
    ground = { [-185781] = 4000, [10000] = 0, [-188657] = 4000 }
    Rings.reset()
    Rings.refresh(0)
    check(Rings.jump_under(1) == true, "a booster 11 m over the ground is a launch pad")
    check(Rings.jump_under(2) == nil, "a booster on the ground is not")
    check(Rings.jump_under(3) == nil, "a booster 26 m up is out of jumping reach")
    check(Rings.jump_under(4) == nil, "no ground under it: not called a launch pad")
    check(Rings.jump_under(1, -185800, 154900, 4100) == true, "under it on the ground: jump")
    check(Rings.jump_under(1, -185800, 153000, 4100) == false, "20 m short of it: not yet")
    check(Rings.jump_under(1, -185800, 154900, 9000) == false, "already above it: no jump")
end

if fails > 0 then
    io.write(fails .. " failure(s)\n")
    os.exit(1)
end
io.write("all race ring tests passed\n")
