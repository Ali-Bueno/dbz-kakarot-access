-- Boost rings ("anillos propulsores") on a race course: optional speed boosts the racer steers
-- through. race_guide cues the nearest one ahead with its own sound, beside the checkpoint
-- beacon. Positions are read ONCE per race (a scan, retried until the course has loaded) and
-- kept as plain numbers, so no actor handle outlives the race.
-- Game data: reference/dbz-kakarot/notes/dbz-kakarot-race-minigame.md

local Core = require("ui_core")
local Beacon = require("beacon")
local Mem = require("mem")

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
local RETRY_S = 3             -- the course has not loaded yet: scan again this often
local MAX_TRIES = 10          -- per race; each try is one or two ~65 ms scans

local rings = nil             -- { {x, y, z}, ... } for the current race
local tries, next_try = 0, 0

function Rings.reset()
    rings, tries, next_try = nil, 0, 0
end

local function position(a)
    local ok, loc = pcall(function() return a:K2_GetActorLocation() end)
    if ok and loc and loc.X then return { loc.X, loc.Y, loc.Z } end
    return nil
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
        print(string.format("[KakarotAccess] race rings -> %d (try %d)\n", #rings, tries))
    end
end

-- The ring to cue: the nearest one ahead within range. Returns pan and 3D distance, or nil.
-- A ring the racer has gone through falls behind and drops out on its own.
function Rings.pick(px, py, pz, fx, fy)
    local best, best_pan
    for _, r in ipairs(rings or {}) do
        local dx, dy, dz = r[1] - px, r[2] - py, r[3] - pz
        local d = math.sqrt(dx * dx + dy * dy + dz * dz)
        if d <= Rings.RANGE and (not best or d < best) then
            local pan, fb = Beacon.cam_rel(dx, dy, fx, fy)
            if fb >= Rings.AHEAD then best, best_pan = d, pan end
        end
    end
    return best_pan, best
end

-- Volume and period of the ring cue: louder and faster as the ring nears.
function Rings.cadence(dist)
    local t = (dist - NEAR) / (Rings.RANGE - NEAR)
    if t < 0 then t = 0 elseif t > 1 then t = 1 end
    return 1.0 - (1.0 - MIN_VOL) * t, MIN_MS + (MAX_MS - MIN_MS) * t
end

function Rings.count() return rings and #rings or 0 end

Rings._set = function(list) rings = list end   -- offline tests

return Rings
