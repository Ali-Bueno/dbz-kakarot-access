-- Offline tests for race_path.lua: the leg is matched by the HUD's next checkpoint, the beacon aims
-- a lookahead along the route, an off-route racer is picked up again, a walking route hands over
-- to the checkpoint at its end, and a launch route says jump under its last point.
local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local fails = 0
local function check(ok, message)
    if ok then io.write("ok   - " .. message .. "\n")
    else fails = fails + 1; io.write("FAIL - " .. message .. "\n") end
end
print = function() end

local M = 100
local Path = require("race_path")
-- An L-shaped walking leg (east 100 m, then north 100 m) and a launch leg ending under a booster.
Path.load({ {
    name = "test course",
    legs = {
        { to = { 10000, 12000, 0 }, pts = { { 0, 0, 0 }, { 10000, 0, 0 }, { 10000, 10000, 0 } } },
        { to = { 30000, 0, 9000 }, jump = true, pts = { { 10000, 12000, 0 }, { 25000, 12000, 0 } } },
        { to = { 50000, 9000, 5000 }, pts = { { 40000, 0, 0 }, { 50000, 0, 0, "launch" },
            { 50000, 2000, 5000 }, { 50000, 9000, 5000 } } },
        { to = { 0, 40000, 900 }, pts = { { 0, 20000, 0 }, { 0, 30000, 0, true }, { 0, 31000, 600 }, { 0, 40000, 600 } } },
    },
} })

do
    Path.reset()
    check(Path.aim(0, 0, 0, 99999, 0, 0) == nil, "a checkpoint no route leads to: no route")
    local ax, ay, _, info = Path.aim(0, 0, 0, 10000, 12000, 0)
    check(ax and math.abs(ax - Path.LOOK) < 1 and ay == 0, "at the start: aim LOOK metres along the first segment")
    check(info.remaining > 199 * M and info.remaining < 201 * M, "the whole route is still to go")
    ax, ay = Path.aim(9500, 0, 0, 10000, 12000, 0)
    check(ax == 10000 and ay > 900 and ay < 1100, "near the corner: the aim has turned the corner")
    ax, ay = Path.aim(9800, 4000, 0, 10000, 12000, 0)
    check(ax == 10000 and ay > 5000, "beside the second segment: aim further up it")
    ax, ay = Path.aim(-3000, -3000, 0, 10000, 12000, 0)
    check(ax and ay == 0 and ax > 0, "off the route: back onto its start")
    check(Path.aim(10000, 9500, 0, 10000, 12000, 0) == nil, "the end of a walking route hands over to the checkpoint")
end

do
    Path.reset()
    local ax, _, _, info = Path.aim(10000, 12000, 0, 30000, 0, 9000)
    check(ax and info.jump_ahead == false and info.jump_now == false, "far from the launch: no prompt")
    ax, _, _, info = Path.aim(22000, 12000, 0, 30000, 0, 9000)
    check(ax > 23000 and info.jump_ahead == true and info.jump_now == false, "30 m before it: launch ahead")
    ax, _, _, info = Path.aim(24800, 12100, 0, 30000, 0, 9000)
    check(info.jump_now == true, "under the booster: jump")
    check(Path.aim(24800, 12100, 2000, 30000, 0, 9000) == nil, "launched: the route lets the checkpoint guide")
    check(Path.aim(24800, 12100, 0, 30000, 0, 9000) == nil, "and stays released for that leg")
    check(Path.aim(0, 0, 0, 10000, 12000, 0) ~= nil, "a new leg starts its own route")
end

do
    Path.reset()
    local _, _, _, info = Path.aim(0, 20000, 0, 0, 40000, 900)
    check(info.ledge == false, "far from the ledge: no jump")
    _, _, _, info = Path.aim(0, 29600, 0, 0, 40000, 900)
    check(info.ledge == true, "at the foot of the ledge: jump")
    _, _, _, info = Path.aim(0, 29800, 0, 0, 40000, 900)
    check(info.ledge == false, "the same ledge is not prompted twice")
end

do
    Path.reset()
    local ax, ay, _, info = Path.aim(47000, 0, 0, 50000, 9000, 5000)
    check(ax < 50000 and ay == 0 and info.jump_ahead, "30 m before a launch: announced, steering along the route")
    ax, ay = Path.aim(49000, 0, 0, 50000, 9000, 5000)
    check(ax == 50000 and ay == 0, "10 m before it: steer at it, never past it to the landing")
    _, _, _, info = Path.aim(49800, 300, 0, 50000, 9000, 5000)
    check(info.jump_now == true, "under the mid-route booster: jump")
    ax, ay, _, info = Path.aim(49800, 300, 2000, 50000, 9000, 5000)
    check(ax == 50000 and ay > 300 and not info.jump_now, "launched: the route goes on to the landing")
end

do
    Path.reset()
    Path.aim(41000, 0, 0, 50000, 9000, 5000)
    local x, y = Path.ahead(41000, 0, 2500)
    check(x == 43500 and y == 0, "ahead: 25 m on along the route")
    x, y = Path.ahead(41000, 0, 50000)
    check(x == 50000 and y == 0, "ahead: never past a launch that has not happened")
    Path.aim(49800, 300, 2000, 50000, 9000, 5000)
    x, y = Path.ahead(50000, 2500, 2500)
    check(x == 50000 and y == 5000, "ahead: past the launch once it happened")
end

if fails > 0 then
    io.write(fails .. " failure(s)\n")
    os.exit(1)
end
io.write("all race path tests passed\n")
