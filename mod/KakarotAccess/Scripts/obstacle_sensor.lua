-- Forward obstacle sensor: a fan of collision rays ahead of a moving racer, reporting the
-- nearest wall-like hit as a direction and a distance. Ground and climbable slopes are ignored
-- by their surface normal. The caller turns the result into a cue (race_guide).
--
-- Safety: KismetSystemLibrary.LineTraceSingle (channel overload) was verified live on
-- 2026-10-18. Its July verdict ("dead", engine facts) came from an arity bug and from the
-- ForObjects overload, which stays unused. Two fuses still guard it:
--   * any caught error or odd result disables the sensor for the session (a caught error
--     retried every tick once ended in an access violation, crash-bug note 2026-07-13);
--   * the first trace of every race runs with "testing" written to ray_fuse.txt. If the
--     process dies inside it, the next session reads "testing" and never traces again
--     (delete the file to re-arm).

local Core = require("ui_core")
local Ray = require("raycast")
local Beacon = require("beacon")
local Mem = require("mem")            -- crash black box only (Mem.mark)

local Sensor = {}

local M = 100                         -- UE units per meter
local CHANNEL = 0                     -- ETraceTypeQuery1 = Visibility
Sensor.RAYS = {                       -- degrees off the heading (UE yaw: + = right), length scale
    { deg = 0, scale = 1.0 },
    { deg = -25, scale = 0.7 },
    { deg = 25, scale = 0.7 },
}
Sensor.START_AHEAD = 3 * M            -- start past the vehicle's own body
Sensor.HEIGHT = 0.5 * M               -- above the racer's origin
Sensor.WALL_NZ = 0.7                  -- ImpactNormal.Z above this is ground or a slope (~45 deg)
Sensor.MIN_REACH = 15 * M
Sensor.MAX_REACH = 45 * M
Sensor.LOOKAHEAD_S = 1.6              -- reach = speed * this, clamped to MIN..MAX

local state = nil                     -- nil = unknown, "ok", "bad"
local armed_race = false              -- the first trace of this race has passed the disk fuse

local fuse_file = nil                 -- tests point it elsewhere
local function fuse_path()
    if fuse_file then return fuse_file end
    local src = debug.getinfo(1, "S").source:sub(2)
    return (src:match("^(.*)[/\\]") or ".") .. "\\ray_fuse.txt"
end

local function read_fuse()
    local f = io.open(fuse_path(), "r")
    if not f then return nil end
    local s = f:read("*l")
    f:close()
    return s
end

local function write_fuse(s)
    local f = io.open(fuse_path(), "w")
    if f then f:write(s, "\n") f:close() end
end

local function disable(why)
    state = "bad"
    print("[KakarotAccess] obstacle sensor disabled: " .. why .. "\n")
end

-- A new race: the next trace goes through the disk fuse again.
function Sensor.new_race()
    armed_race = false
end

function Sensor.available()
    if state == nil then
        local fuse = read_fuse()
        if fuse == "testing" then
            write_fuse("bad")
            disable("the game died inside a trace last session (ray_fuse.txt)")
        elseif fuse == "bad" then
            disable("ray_fuse.txt says bad")
        else
            state = "ok"
        end
    end
    return state == "ok"
end

-- How far ahead to look at this speed (uu/s).
function Sensor.reach(speed)
    local r = (speed or 0) * Sensor.LOOKAHEAD_S
    if r < Sensor.MIN_REACH then return Sensor.MIN_REACH end
    if r > Sensor.MAX_REACH then return Sensor.MAX_REACH end
    return r
end

local function trace(ctx, sx, sy, sz, ex, ey, ez)
    if armed_race then return Ray.probe_surface(ctx, sx, sy, sz, ex, ey, ez, CHANNEL) end
    write_fuse("testing")
    local hit, dist, nz = Ray.probe_surface(ctx, sx, sy, sz, ex, ey, ez, CHANNEL)
    write_fuse("ok")
    armed_race = true
    return hit, dist, nz
end

-- The nearest wall-like obstacle ahead: pan (-1..1, camera-style), distance in uu (from the
-- racer), or nil when the way is clear. Returns false when the sensor cannot run.
-- ctx = world-context UObject (the racer pawn); (fx, fy) = unit heading on the ground plane.
function Sensor.scan(ctx, px, py, pz, fx, fy, speed)
    if not fx or not Core.valid(ctx) or not Sensor.available() then return false end
    Mem.mark("sensor.scan")
    local reach = Sensor.reach(speed)
    local sz = pz + Sensor.HEIGHT
    local best, best_pan
    for _, r in ipairs(Sensor.RAYS) do
        local a = math.rad(r.deg)
        local c, s = math.cos(a), math.sin(a)
        local dx, dy = fx * c - fy * s, fx * s + fy * c
        local sx, sy = px + dx * Sensor.START_AHEAD, py + dy * Sensor.START_AHEAD
        local len = reach * r.scale
        local hit, dist, nz = trace(ctx, sx, sy, sz, sx + dx * len, sy + dy * len, sz)
        if hit == nil then
            disable("a trace failed")
            return false
        end
        if hit and dist and (nz == nil or nz <= Sensor.WALL_NZ) then
            local d = dist + Sensor.START_AHEAD
            if not best or d < best then
                best = d
                best_pan = Beacon.cam_rel(dx, dy, fx, fy)
            end
        end
    end
    return best_pan, best
end

-- Volume and period of the obstacle cue: louder and faster as the obstacle nears.
function Sensor.cadence(dist)
    local near = Sensor.START_AHEAD + 2 * M
    local t = (dist - near) / (Sensor.MAX_REACH - near)
    if t < 0 then t = 0 elseif t > 1 then t = 1 end
    return 1.0 - 0.4 * t, 100 + 400 * t
end

Sensor._reset = function(path) state, armed_race, fuse_file = nil, false, path end   -- offline tests

return Sensor
