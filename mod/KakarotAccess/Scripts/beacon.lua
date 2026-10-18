-- The radar ping's shape: how a direction and a distance become pan, pitch, volume and
-- cadence. Shared by every guide that plays beacon.wav (quest radar, race checkpoints) so
-- they all sound alike. Pure math, no engine reads.
--
-- The feel constants (cadence range, volume slope, behind-pitch) are the values validated
-- by ear in the XV2 mod (reference/audio-navigation); distances are converted to UE units
-- (1 m = 100 uu) and rescaled for Kakarot's larger fields.

local Beacon = {}

local M = 100                         -- UE units per meter
Beacon.NEAR_DIST = 5 * M              -- distance mapped to the FASTEST ping
Beacon.FAR_DIST = 200 * M             -- distance mapped to the SLOWEST ping
Beacon.MIN_INTERVAL_MS = 100          -- ping period when at NEAR_DIST (XV2 used 120;
                                      -- 100 = one ping per radar tick at the closest range)
Beacon.MAX_INTERVAL_MS = 850          -- ping period when at FAR_DIST (XV2 value)
Beacon.VOL_SLOPE = 0.6                -- volume = 1 - VOL_SLOPE * t (XV2 value)
Beacon.BEHIND_PITCH_SLOPE = 0.3       -- pitch = 1 + fb * slope when behind (XV2)
Beacon.PITCH_MIN = 0.6                -- clamp for the behind pitch (XV2 value)

-- Camera-relative pan (-1..1) and forward component fb (cos of the angle) of the ground
-- direction (dx, dy). (fx, fy) is the camera-forward unit vector; nil means centred.
function Beacon.cam_rel(dx, dy, fx, fy)
    local dh = math.sqrt(dx * dx + dy * dy)
    if dh < 1 or not fx then return 0, 1 end
    local nx, ny = dx / dh, dy / dh
    -- UE left-handed ground plane: forward(yaw) = (cos, sin), right = (-sin, cos).
    local pan = nx * -fy + ny * fx
    local fb = nx * fx + ny * fy
    if pan > 1 then pan = 1 elseif pan < -1 then pan = -1 end
    return pan, fb
end

-- Lower pitch the further behind the target is; 1.0 anywhere in front.
function Beacon.behind_pitch(fb)
    if fb >= 0 then return 1.0 end
    local p = 1.0 + fb * Beacon.BEHIND_PITCH_SLOPE
    return p < Beacon.PITCH_MIN and Beacon.PITCH_MIN or p
end

-- Volume and ping period (ms) for a distance in UE units.
function Beacon.cadence(dist)
    local t = (dist - Beacon.NEAR_DIST) / (Beacon.FAR_DIST - Beacon.NEAR_DIST)
    if t < 0 then t = 0 elseif t > 1 then t = 1 end
    local vol = 1.0 - Beacon.VOL_SLOPE * t
    local interval = Beacon.MIN_INTERVAL_MS + (Beacon.MAX_INTERVAL_MS - Beacon.MIN_INTERVAL_MS) * t
    return vol, interval
end

return Beacon
