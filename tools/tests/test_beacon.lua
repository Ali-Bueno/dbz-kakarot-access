-- Offline tests for beacon.lua, the radar ping shape shared by the quest radar and the race
-- guide: pan side, behind pitch, and the XV2-validated cadence endpoints.
local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local Beacon = require("beacon")

local fails = 0
local function check(ok, message)
    if ok then print("ok   - " .. message)
    else fails = fails + 1; print("FAIL - " .. message) end
end
local function near(a, b) return math.abs(a - b) < 1e-9 end

-- Camera looking down +X (yaw 0): forward (1, 0), right is +Y in UE's left-handed plane.
do
    local pan, fb = Beacon.cam_rel(0, 1000, 1, 0)
    check(near(pan, 1) and near(fb, 0), "a target on +Y is hard right of a yaw-0 camera")
    pan, fb = Beacon.cam_rel(0, -1000, 1, 0)
    check(near(pan, -1), "a target on -Y is hard left")
    pan, fb = Beacon.cam_rel(1000, 0, 1, 0)
    check(near(pan, 0) and near(fb, 1), "a target straight ahead is centred, in front")
    pan, fb = Beacon.cam_rel(-1000, 0, 1, 0)
    check(near(pan, 0) and near(fb, -1), "a target straight behind is centred, behind")
    pan, fb = Beacon.cam_rel(0, 1000, 0, 1)
    check(near(pan, 0) and near(fb, 1), "turning the camera to yaw 90 puts +Y ahead")
end

do
    local pan, fb = Beacon.cam_rel(0, 1000, nil, nil)
    check(pan == 0 and fb == 1, "no camera reading centres the beacon")
    pan, fb = Beacon.cam_rel(0.5, 0.5, 1, 0)
    check(pan == 0 and fb == 1, "standing on the point centres the beacon")
end

do
    check(Beacon.behind_pitch(0.5) == 1.0, "no pitch drop in front")
    check(near(Beacon.behind_pitch(-0.5), 1.0 - 0.5 * Beacon.BEHIND_PITCH_SLOPE), "pitch drops behind")
    check(Beacon.behind_pitch(-1) >= Beacon.PITCH_MIN, "the behind pitch never goes under PITCH_MIN")
end

do
    local vol, interval = Beacon.cadence(0)
    check(vol == 1.0 and interval == Beacon.MIN_INTERVAL_MS, "closer than NEAR_DIST: loudest, fastest")
    vol, interval = Beacon.cadence(Beacon.FAR_DIST * 10)
    check(near(vol, 1.0 - Beacon.VOL_SLOPE) and interval == Beacon.MAX_INTERVAL_MS,
        "beyond FAR_DIST: quietest, slowest")
    local v1, i1 = Beacon.cadence(Beacon.NEAR_DIST + 1000)
    local v2, i2 = Beacon.cadence(Beacon.NEAR_DIST + 5000)
    check(v1 > v2 and i1 < i2, "closer means louder and faster")
end

if fails > 0 then
    print(fails .. " failure(s)")
    os.exit(1)
end
print("all beacon tests passed")
