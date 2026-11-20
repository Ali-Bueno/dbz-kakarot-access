-- Course arrows as route waypoints for the race guide.
--
-- Live 2026-11-19 (Area03 walking-robot race, tester save): the straight line from the start to
-- the first checkpoint runs off a cliff into the water ("Te caíste al agua. La carrera se
-- reiniciará"), three runs out of three for a test driver steering by the beacon alone. The course
-- marks its way with arrow meshes (StaticMeshActor STM_mg_racing_Arrow_*, 52 on that course, in
-- groups of two or three), so the beacon steers to the next arrow group on the way to the
-- checkpoint, then to the checkpoint itself.
--
-- Positions only: the arrows' yaw was not verified to be their pointing direction, so it is not
-- used. A group is ON THE WAY by the same rule as a boost ring (race_rings.on_route): ahead along
-- the racer->checkpoint line, nearer the checkpoint than the racer, small detour. It is PASSED once
-- the racer is within PASS_R of it or nearer the checkpoint than the group is.
-- Read once per race (one StaticMeshActor scan, only while a race is live) and kept as numbers.
-- Notes: reference/dbz-kakarot/notes/dbz-kakarot-race-minigame.md

local Core = require("ui_core")

local Route = {}

local M = 100
local NAME = "racing_Arrow"
local GROUP_R = 8 * M          -- arrows this close together are one waypoint
local PASS_R = 8 * M
Route.REACH = 120 * M
Route.DETOUR = 30 * M
Route.DETOUR_FRAC = 0.25
local RETRY_S, MAX_TRIES = 3, 5

local points = nil             -- { {x, y, z}, ... }
local passed = {}
local tries, next_try = 0, 0

function Route.reset()
    points, passed, tries, next_try = nil, {}, 0, 0
end

local function scan()
    local groups = {}
    for _, a in pairs(Core.findall("StaticMeshActor")) do
        local name
        pcall(function() name = a:GetFName():ToString() end)
        if type(name) == "string" and name:find(NAME, 1, true) then
            local loc
            pcall(function() loc = a:K2_GetActorLocation() end)
            if loc and loc.X then
                local g
                for _, p in ipairs(groups) do
                    if (p.x - loc.X) ^ 2 + (p.y - loc.Y) ^ 2 <= GROUP_R * GROUP_R then g = p break end
                end
                if g then
                    g.n = g.n + 1
                    g.sx, g.sy, g.sz = g.sx + loc.X, g.sy + loc.Y, g.sz + loc.Z
                    g.x, g.y = g.sx / g.n, g.sy / g.n
                else
                    groups[#groups + 1] = { n = 1, sx = loc.X, sy = loc.Y, sz = loc.Z, x = loc.X, y = loc.Y }
                end
            end
        end
    end
    local list = {}
    for _, g in ipairs(groups) do list[#list + 1] = { g.sx / g.n, g.sy / g.n, g.sz / g.n } end
    return list
end

-- Called every race tick; scans until the arrows are known (a course without any stops asking).
function Route.refresh(now)
    if points and #points > 0 then return end
    if tries >= MAX_TRIES or now < next_try then return end
    tries, next_try = tries + 1, now + RETRY_S
    points = scan()
    if #points > 0 or tries == MAX_TRIES then
        print(string.format("[KakarotAccess] race route -> %d arrow groups (try %d)\n", #points, tries))
    end
end

-- The waypoint to steer to on the way to the checkpoint at (tx, ty, tz): x, y, z, distance and
-- index, or nil (steer to the checkpoint itself).
function Route.next(px, py, pz, tx, ty, tz)
    local cx, cy = tx - px, ty - py
    local cd = math.sqrt(cx * cx + cy * cy)
    if cd < 1 then return nil end
    local ux, uy = cx / cd, cy / cd
    local budget = math.max(Route.DETOUR, Route.DETOUR_FRAC * cd)
    local best, best_d
    for i, w in ipairs(points or {}) do
        local dx, dy = w[1] - px, w[2] - py
        local d = math.sqrt(dx * dx + dy * dy)
        local wc = math.sqrt((tx - w[1]) ^ 2 + (ty - w[2]) ^ 2)
        if d <= PASS_R then passed[i] = true end
        if not passed[i] and d <= Route.REACH then
            local along = dx * ux + dy * uy
            if along > 0 and along < cd and wc < cd and d + wc - cd <= budget
                and (not best_d or d < best_d) then
                best, best_d = i, d
            end
        end
    end
    if not best then return nil end
    local w = points[best]
    return w[1], w[2], w[3], best_d, best
end

function Route.count() return points and #points or 0 end

Route._set = function(list) points, passed = list, {} end   -- offline tests

return Route
