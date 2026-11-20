-- Boost rings ("anillos propulsores") on a race course: optional speed boosts the racer steers
-- through. race_guide cues the nearest one ahead with its own sound, beside the checkpoint
-- beacon. Positions are read ONCE per race (a scan, retried until the course has loaded) and
-- kept as plain numbers, so no actor handle outlives the race.
-- Game data: reference/dbz-kakarot/notes/dbz-kakarot-race-minigame.md

local Core = require("ui_core")
local Beacon = require("beacon")
local Mem = require("mem")
local Ray = require("raycast")

local Rings = {}

local M = 100                 -- UE units per meter
-- ABoostPointActor (AT.hpp:15145) is the native class; BP_BoostPoint is its course placement
-- (pak index, Blueprints/MiniGame/Race). Native first: a FindAllOf on a native base can come
-- back empty while the BP name works (engine facts), so the BP name is the fallback.
local CLASSES = { "BoostPointActor", "BP_BoostPoint_C" }
Rings.RANGE = 60 * M          -- cue a ring only this close...
Rings.AHEAD = 0.5             -- ...and within 60 degrees of the heading (cam_rel fb)
local NEAR = 5 * M            -- fastest cue at this distance
local MIN_MS, MAX_MS = 150, 600
local MIN_VOL = 0.6
-- A ring counts as ON THE WAY to the checkpoint when it lies ahead (along the racer->checkpoint
-- line, short of the checkpoint), is nearer the checkpoint than the racer, and going through it
-- costs little extra path: d(racer, ring) + d(ring, checkpoint) - d(racer, checkpoint) at most
-- DETOUR, or DETOUR_FRAC of the way to the checkpoint if that is more. A detour budget follows a
-- curving course; the first version's straight 20 m corridor and 60 m reach missed the boosters
-- after the first robot-race checkpoint (player report 2026-11-19, second round). The ring
-- already being steered to keeps a wider budget, so steering towards it does not drop it.
Rings.REACH = 200 * M
Rings.DETOUR = 30 * M
Rings.DETOUR_FRAC = 0.25
local KEEP = 1.5
-- A ring whose boost zone size is unknown counts as gone through this close. Live, a robot-race
-- boost zone (UBoostPointComponent box) measured 24 x 24 m and 2 m tall (GetActorBounds extent
-- 1200, 1200, 100), so the real zone is read at scan time and used instead (see position).
local USED_R = 4 * M
local USED_DZ = 4 * M         -- vertical slack over the zone's own half-height
-- COMMIT: once steering to a ring, keep it until the racer has been through it or has left it this
-- far behind. Live 2026-11-19 (Area03 robot race): ring 6 sits 23 m short of checkpoint 2, which is
-- 36 m HIGHER, so the boost is what gets the robot up there. The line rule dropped the ring the
-- moment the robot drew level with the checkpoint underneath it, 15 m beside the ring, and the
-- race timed out: the player's exact report.
Rings.HOLD = 40 * M
-- Launch pads. Live 2026-11-20 (Area03 robot race): boosters 17 and 20 float 11-12 m over the
-- ground; jumping (A) into one threw the robot ~60 m straight up (booster 17 onto checkpoint 2's
-- pillar), walking under one did nothing. Boosters 26 m or more up are mid-air second stages.
local FLOAT_MIN = 4 * M       -- lower than this over the ground: a ring you walk through
local JUMP_MAX = 20 * M       -- higher than this: not reachable with a jump from the ground
local JUMP_R = 10 * M         -- "jump" this close to it: a running jump rises into the zone ~10 m on
local GROUND_REACH = 80 * M   -- the downward trace length
local RETRY_S = 3             -- the course has not loaded yet: scan again this often
local MAX_TRIES = 10          -- per race; each try is one or two ~65 ms scans

local rings = nil             -- { {x, y, z}, ... } for the current race
local tries, next_try = 0, 0
local locked = nil            -- index of the ring on_route returned last
local used = {}               -- indices of rings the racer has been through this race
local last = nil              -- the racer's previous position, for the swept "taken" test
local TELEPORT = 60 * M       -- a longer step than this between ticks is a teleport, not a pass

function Rings.reset()
    rings, tries, next_try, locked, used, last = nil, 0, 0, nil, {}, nil
end

-- Position plus the boost zone's usable radius and half-height: 0.7 of the smaller horizontal
-- half-extent of the actor bounds (an axis-aligned box of a possibly rotated zone, so the inner
-- part only), falling back to USED_R when the bounds cannot be read.
local function position(a)
    local ok, loc = pcall(function() return a:K2_GetActorLocation() end)
    if not (ok and loc and loc.X) then return nil end
    local origin, ext = {}, {}
    local bok = pcall(function() a:GetActorBounds(false, origin, ext) end)
    local r, h = nil, nil
    if bok and tonumber(ext.X) and tonumber(ext.Y) and ext.X > 0 and ext.Y > 0 then
        r = 0.7 * math.min(ext.X, ext.Y)
        h = tonumber(ext.Z)
    end
    -- The ground under it (one Visibility trace down, started below the zone): a booster that
    -- floats is a launch pad the racer must JUMP into (live 2026-11-20, Area03 robot race).
    local ground
    local hit, dist = Ray.probe_surface(a, loc.X, loc.Y, loc.Z - 150, loc.X, loc.Y, loc.Z - GROUND_REACH, 0)
    if hit and tonumber(dist) then ground = loc.Z - 150 - dist end
    return { loc.X, loc.Y, loc.Z, r, h, ground }
end

-- Floating ring i: nil when it is not a launch pad to jump into (on the ground, too high to reach
-- with a jump, or the ground under it unknown). With the racer's position: true when the racer is
-- under it, within the zone's radius, and below it.
function Rings.jump_under(i, px, py, pz)
    local r = rings and rings[i]
    if not (r and r[6]) then return nil end
    local lift = r[3] - r[6]
    if lift < FLOAT_MIN or lift > JUMP_MAX then return nil end
    if not px then return true end
    local zr = math.max(r[4] or USED_R, JUMP_R)
    return (px - r[1]) ^ 2 + (py - r[2]) ^ 2 <= zr * zr and pz < r[3]
end

-- A mid-air second stage: floating higher than a jump reaches. The racer cannot steer to it; a
-- launch from the pad below carries it through (live 2026-11-20: pad 10 -> 15 stacked, 17 -> 19).
-- Never cued: "only the boosters you can take sound" (player report 2026-11-20).
local function stage(r)
    return r[6] ~= nil and r[3] - r[6] > JUMP_MAX
end

-- Marks every ring the racer passed through since the previous call, by the SEGMENT between the
-- two positions, not the point: during a launch the robot moves tens of metres per tick and a
-- point test missed the zone. Taken rings go silent for the rest of the race. Call every tick.
function Rings.track(px, py, pz)
    local a = last
    last = { px, py, pz }
    if not (a and rings) then return end
    local sx, sy, sz = px - a[1], py - a[2], pz - a[3]
    local L2 = sx * sx + sy * sy
    if L2 > TELEPORT * TELEPORT then return end
    for i, r in ipairs(rings) do
        if not used[i] then
            local t = L2 > 0 and math.max(0, math.min(1, ((r[1] - a[1]) * sx + (r[2] - a[2]) * sy) / L2)) or 1
            local qx, qy, qz = a[1] + t * sx, a[2] + t * sy, a[3] + t * sz
            local zr = r[4] or USED_R
            if (qx - r[1]) ^ 2 + (qy - r[2]) ^ 2 <= zr * zr and math.abs(qz - r[3]) <= (r[5] or 0) + USED_DZ then
                used[i] = true
                print(string.format("[KakarotAccess] race booster -> taken (ring %d)\n", i))
            end
        end
    end
end

-- Every boost ring in the world right now, deduped by address, archetypes skipped.
local function scan()
    local seen, list = {}, {}
    for _, cls in ipairs(CLASSES) do
        for _, a in pairs(Core.findall(cls)) do
            if Core.valid(a) then
                local addr = Mem.raw_addr(a)
                local ok, name = pcall(function() return a:GetFullName() end)
                local archetype = ok and type(name) == "string" and name:find("Default__", 1, true)
                if addr and not seen[addr] and not archetype then
                    seen[addr] = true
                    list[#list + 1] = position(a)
                end
            end
        end
        if #list > 0 then break end
    end
    return list
end

-- Called every race tick: scans until the rings are known (a course without rings stops
-- asking after MAX_TRIES).
function Rings.refresh(now)
    if rings and #rings > 0 then return end
    if tries >= MAX_TRIES or now < next_try then return end
    tries, next_try = tries + 1, now + RETRY_S
    rings = scan()
    if #rings > 0 or tries == MAX_TRIES then
        local r1 = rings[1]
        print(string.format("[KakarotAccess] race rings -> %d (try %d, zone r=%s m)\n", #rings, tries,
            r1 and r1[4] and string.format("%.1f", r1[4] / M) or "?"))
    end
end

-- The ring to cue: the nearest one ahead within range. Returns pan and 3D distance, or nil.
-- A ring the racer has gone through falls behind and drops out on its own.
function Rings.pick(px, py, pz, fx, fy)
    local best, best_pan
    for i, r in ipairs(rings or {}) do
        local dx, dy, dz = r[1] - px, r[2] - py, r[3] - pz
        local d = math.sqrt(dx * dx + dy * dy + dz * dz)
        if d <= Rings.RANGE and (not best or d < best) and not used[i] and not stage(r) then
            local pan, fb = Beacon.cam_rel(dx, dy, fx, fy)
            if fb >= Rings.AHEAD then best, best_pan = d, pan end
        end
    end
    return best_pan, best
end

-- The ring to steer through on the way to the checkpoint (player report 2026-11-19: the robot
-- race cannot be won without boosts, and a side cue was not enough to reach them). Returns its
-- x, y, z, 3D distance and index, or nil. A ring the racer has been through is never picked again
-- this race; one passed without touching it is behind the racer and drops out on its own.
function Rings.on_route(px, py, pz, tx, ty, tz)
    local cx, cy = tx - px, ty - py
    local cd = math.sqrt(cx * cx + cy * cy)
    if cd < 1 then locked = nil return nil end
    local ux, uy = cx / cd, cy / cd
    local budget = math.max(Rings.DETOUR, Rings.DETOUR_FRAC * cd)
    local best, best_d
    for i, r in ipairs(rings or {}) do
        local dx, dy, dz = r[1] - px, r[2] - py, r[3] - pz
        local d = math.sqrt(dx * dx + dy * dy + dz * dz)
        local zr = r[4] or USED_R
        if dx * dx + dy * dy <= zr * zr and math.abs(dz) <= (r[5] or 0) + USED_DZ then
            used[i] = true
        end
        if i == locked and not used[i] and d <= Rings.HOLD then
            return r[1], r[2], r[3], d, i
        end
        if not used[i] and not stage(r) and d <= Rings.REACH then
            local along = dx * ux + dy * uy
            local rcx, rcy = tx - r[1], ty - r[2]
            local rc = math.sqrt(rcx * rcx + rcy * rcy)
            local detour = math.sqrt(dx * dx + dy * dy) + rc - cd
            local allow = (i == locked) and budget * KEEP or budget
            if along > 0 and along < cd and rc < cd and detour <= allow
                and (not best_d or d < best_d) then
                best, best_d = i, d
            end
        end
    end
    locked = best
    if not best then return nil end
    local r = rings[best]
    return r[1], r[2], r[3], best_d, best
end

-- Distance to the nearest ring not yet gone through, for the log when none is on the way.
function Rings.nearest_unused(px, py, pz)
    local best
    for i, r in ipairs(rings or {}) do
        if not used[i] then
            local d = math.sqrt((r[1] - px) ^ 2 + (r[2] - py) ^ 2 + (r[3] - pz) ^ 2)
            if not best or d < best then best = d end
        end
    end
    return best
end

-- Volume and period of the ring cue: louder and faster as the ring nears.
function Rings.cadence(dist)
    local t = (dist - NEAR) / (Rings.RANGE - NEAR)
    if t < 0 then t = 0 elseif t > 1 then t = 1 end
    return 1.0 - (1.0 - MIN_VOL) * t, MIN_MS + (MAX_MS - MIN_MS) * t
end

function Rings.count() return rings and #rings or 0 end

Rings._dump = function() return rings, used end   -- dev channel (rings)
Rings._set = function(list) rings, used, locked = list, {}, nil end   -- offline tests

return Rings
