-- Offline tests for race_route.lua: course arrows grouped into waypoints, steered to on the way
-- to the checkpoint, dropped once passed. Geometry from the Area03 robot race (live 2026-11-19).
local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local fails = 0
local function check(ok, message)
    if ok then io.write("ok   - " .. message .. "\n")
    else fails = fails + 1; io.write("FAIL - " .. message .. "\n") end
end

-- Fake arrows: a StaticMeshActor list with names and locations.
local function actor(name, x, y, z)
    local a = { valid = true }
    function a:GetFName() return { ToString = function() return name end } end
    function a:K2_GetActorLocation() return { X = x, Y = y, Z = z } end
    return a
end
local world = {
    actor("STM_mg_racing_Arrow_25", -166800, 135900, 8300),
    actor("STM_mg_racing_Arrow_26", -166900, 136200, 8000),
    actor("STM_mg_racing_Arrow_27", -169400, 153100, 2300),
    actor("STM_mg_racing_Arrow_28", -169900, 153300, 2300),
    actor("STM_mg_racing_Arrow_12", -183300, 145500, 3000),
    actor("SM_Rock_01", -165000, 136000, 8000),
}
package.preload["ui_core"] = function()
    return { findall = function(cls) return cls == "StaticMeshActor" and world or {} end }
end

local Route = require("race_route")
local start = { -163844, 135760, 8364 }
local cp = { -171143, 151614, 1708 }

Route.refresh(0)
check(Route.count() == 3, "arrows grouped into waypoints, other meshes ignored")
local x, y, _, d, i = Route.next(start[1], start[2], start[3], cp[1], cp[2], cp[3])
check(i ~= nil and x < -166000 and y < 137000 and d < 35 * 100,
    "from the start the beacon goes to the first arrow group (30 m), not straight off the cliff")
local _, _, _, _, i2 = Route.next(-167000, 137000, 8000, cp[1], cp[2], cp[3])
check(i2 ~= i, "past the first group it is not steered to again")
local i3 = select(5, Route.next(-168500, 145000, 4000, cp[1], cp[2], cp[3]))
check(i3 == nil, "a group lying past the checkpoint is left for the checkpoint itself")
check(Route.next(-171000, 151400, 1700, cp[1], cp[2], cp[3]) == nil, "at the checkpoint: no waypoint")
Route.reset()
check(Route.count() == 0, "reset forgets the course")

if fails > 0 then
    io.write(fails .. " failure(s)\n")
    os.exit(1)
end
io.write("all race route tests passed\n")
