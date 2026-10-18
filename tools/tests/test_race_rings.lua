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

if fails > 0 then
    io.write(fails .. " failure(s)\n")
    os.exit(1)
end
io.write("all race ring tests passed\n")
