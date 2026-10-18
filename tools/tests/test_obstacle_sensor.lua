-- Offline tests for obstacle_sensor.lua: the ray fan reports the nearest wall-like hit with its
-- direction, ignores ground, scales its reach with speed, and both fuses (session and disk)
-- shut the sensor off instead of retrying a failing trace.
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

-- The fake world: `walls` maps a ray's bearing (degrees off +X, rounded) to {dist, nz}.
local walls = {}
local traces = {}
local fail_next = false
local fuse_seen = nil      -- the fuse file's content observed during the first trace
local fuse = os.tmpname()

local function read(path)
    local f = io.open(path, "r")
    if not f then return nil end
    local s = f:read("*l")
    f:close()
    return s
end

module("ui_core", { valid = function(o) return type(o) == "table" and o.valid == true end })
module("mem", { mark = function() end })
module("raycast", { probe_surface = function(_, sx, sy, _, ex, ey)
    if #traces == 0 then fuse_seen = read(fuse) end
    local dx, dy = ex - sx, ey - sy
    traces[#traces + 1] = { len = math.sqrt(dx * dx + dy * dy) }
    if fail_next then return nil end
    local w = walls[math.floor(math.deg(math.atan(dy, dx)) + 0.5)]
    if not w then return false end
    return true, w.dist, w.nz
end })
print = function() end   -- the disable log line

local Sensor = require("obstacle_sensor")
local ctx = { valid = true }
local M = 100

local function fresh(contents)
    local f = io.open(fuse, "w")
    if contents then f:write(contents, "\n") end
    f:close()
    if not contents then os.remove(fuse) end
    Sensor._reset(fuse)
    walls, traces, fail_next, fuse_seen = {}, {}, false, nil
end

do
    fresh(nil)
    local pan, dist = Sensor.scan(ctx, 0, 0, 0, 1, 0, 0)
    check(pan == nil and #traces == 3, "a clear way: three rays, nothing reported")
    check(fuse_seen == "testing" and read(fuse) == "ok",
        "the race's first trace runs with the disk fuse at 'testing', then 'ok'")
    check(math.abs(traces[1].len - 15 * M) < 1 and math.abs(traces[2].len - 10.5 * M) < 1,
        "standing still: 15 m ahead, side rays shorter")
end

do
    fresh(nil)
    walls[0] = { dist = 10 * M, nz = 0.1 }
    local pan, dist = Sensor.scan(ctx, 0, 0, 0, 1, 0, 0)
    check(pan and math.abs(pan) < 1e-6 and math.abs(dist - 13 * M) < 1,
        "a wall straight ahead: centred, distance counted from the racer")
end

do
    fresh(nil)
    walls[0] = { dist = 12 * M, nz = 0.0 }
    walls[25] = { dist = 4 * M, nz = 0.0 }
    local pan = Sensor.scan(ctx, 0, 0, 0, 1, 0, 0)
    check(pan and pan > 0.4, "the nearer hit on the right ray wins and pans right")
end

do
    fresh(nil)
    walls[0] = { dist = 8 * M, nz = 0.95 }
    walls[-25] = { dist = 6 * M, nz = 0.5 }
    local pan = Sensor.scan(ctx, 0, 0, 0, 1, 0, 0)
    check(pan and pan < -0.4, "ground ahead is ignored; a steep face on the left is reported")
end

do
    fresh(nil)
    Sensor.scan(ctx, 0, 0, 0, 1, 0, 5000)
    check(math.abs(traces[1].len - 45 * M) < 1, "fast: the reach is capped at 45 m")
end

do
    fresh(nil)
    check(Sensor.scan({ valid = false }, 0, 0, 0, 1, 0, 0) == false and #traces == 0,
        "no racer pawn: nothing traced")
    check(Sensor.available(), "...and the sensor is not blamed for it")
    check(Sensor.scan(ctx, 0, 0, 0, nil, nil, 0) == false, "no heading: nothing traced")
end

do
    fresh(nil)
    fail_next = true
    check(Sensor.scan(ctx, 0, 0, 0, 1, 0, 0) == false, "a failing trace reports the sensor down")
    fail_next = false
    traces = {}
    Sensor.scan(ctx, 0, 0, 0, 1, 0, 0)
    check(#traces == 0 and not Sensor.available(), "...and it is never retried this session")
end

do
    fresh("testing")
    check(Sensor.scan(ctx, 0, 0, 0, 1, 0, 0) == false and #traces == 0,
        "the game died inside a trace last session: no traces at all")
    check(read(fuse) == "bad", "...and the fuse stays blown")
end

do
    fresh(nil)
    Sensor.scan(ctx, 0, 0, 0, 1, 0, 0)
    fuse_seen = nil
    os.remove(fuse)
    traces = {}
    Sensor.scan(ctx, 0, 0, 0, 1, 0, 0)
    check(read(fuse) == nil, "later traces in the same race skip the disk")
    Sensor.new_race()
    traces = {}
    Sensor.scan(ctx, 0, 0, 0, 1, 0, 0)
    check(fuse_seen == "testing" and read(fuse) == "ok", "a new race re-arms the disk fuse")
end

do
    local v1, i1 = Sensor.cadence(5 * M)
    local v2, i2 = Sensor.cadence(40 * M)
    check(v1 > v2 and i1 < i2, "closer obstacles cue louder and faster")
end

os.remove(fuse)
if fails > 0 then
    io.write(fails .. " failure(s)\n")
    os.exit(1)
end
io.write("all obstacle sensor tests passed\n")
