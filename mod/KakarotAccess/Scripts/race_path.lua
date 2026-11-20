-- Walking routes for race legs whose straight line cannot be walked (water, cliffs, a checkpoint
-- on top of a pillar). The beacon steers to a point LOOK metres ahead along the route instead of
-- at the checkpoint ("pure pursuit"), so a slow-turning walker follows the bends.
--
-- Live 2026-11-19/20 (Area03 walking-robot race): the straight line from the start to checkpoint 1
-- crosses a lake (ATWaterVolume_WaterC; entering it restarts the race), and checkpoint 2 stands on a
-- rock pillar with 40 m sheer walls that is reached only by JUMPING into a booster beside it (a
-- booster is a vertical launch pad, not a speed ring). The routes in race_courses.lua come from a
-- terrain survey of the course (A* over the ground heights, water and too-steep climbs excluded);
-- a leg whose route ends in a launch is marked `jump`.
-- Pure Lua over plain numbers: no engine calls. Notes: dbz-kakarot-race-minigame.md

local Path = {}

local M = 100
Path.MATCH_R = 15 * M      -- the HUD's next checkpoint must lie this close to a leg's `to`
Path.LOOK = 15 * M         -- steer at the route point this far ahead of the racer
Path.OFF = 40 * M          -- further than this from the window of segments: search the whole route
Path.JUMP_R = 6 * M        -- the launch point: stop within this, then jump (a running jump overshoots a
                           -- pillar top by ~60 m; a standing jump landed on it, live 2026-11-20)
Path.NEAR_JUMP = 40 * M    -- announce the coming launch from this far along the route
Path.LAUNCHED_DZ = 8 * M   -- the racer this far above the launch point has been launched
local WINDOW = 8           -- segments searched ahead of the current one

Path.LEDGE_R = 6 * M       -- "jump" this close to the foot of a ledge on the route
-- A booster taken AT A RUN ("boost" point: the pad itself): the jump goes this far before it. The
-- jump winds up ~0.5 s and peaks ~0.9 s later (live 2026-11-20), so at speed the racer must leave
-- the ground well short of the pad to be in the air under it.
Path.BOOST_R = 12 * M
local courses = nil
local leg, seg = nil, 1
local launched = {}         -- route point indices whose launch has happened (this leg)
local cur_limit = nil       -- the route point the racer may not be steered past yet (a launch)
local ledges_done = {}     -- route point indices whose jump was prompted (this leg)

function Path.load(list) courses = list end   -- offline tests inject their own data

local function data()
    if courses == nil then
        local ok, t = pcall(require, "race_courses")
        courses = ok and type(t) == "table" and t or {}
    end
    return courses
end

function Path.reset()
    leg, seg, launched, ledges_done = nil, 1, {}, {}
end

local function find_leg(tx, ty, tz)
    for _, c in ipairs(data()) do
        for _, l in ipairs(c.legs or {}) do
            local t = l.to
            if (t[1] - tx) ^ 2 + (t[2] - ty) ^ 2 <= Path.MATCH_R ^ 2 and math.abs(t[3] - tz) <= Path.MATCH_R then
                return l, c.name
            end
        end
    end
end

-- Projection of (px, py) on segment i of the route: squared distance, parameter t in [0, 1].
local function project(pts, i, px, py)
    local a, b = pts[i], pts[i + 1]
    local dx, dy = b[1] - a[1], b[2] - a[2]
    local L2 = dx * dx + dy * dy
    local t = 0
    if L2 > 0 then t = ((px - a[1]) * dx + (py - a[2]) * dy) / L2 end
    if t < 0 then t = 0 elseif t > 1 then t = 1 end
    local qx, qy = a[1] + t * dx, a[2] + t * dy
    return (px - qx) ^ 2 + (py - qy) ^ 2, t
end

local function seg_len(pts, i)
    return math.sqrt((pts[i + 1][1] - pts[i][1]) ^ 2 + (pts[i + 1][2] - pts[i][2]) ^ 2)
end

-- A point's flag: true = the foot of a ledge (jump there), "launch" = a floating booster above it
-- (stop, then jump into it). A leg's `jump` (older data) makes its last point a launch.
local function flag(l, k)
    local f = l.pts[k][4]
    if f == nil and l.jump and k == #l.pts then return "launch" end
    return f
end

-- Where to steer for the checkpoint at (tx, ty, tz), or nil when no route applies (no data for
-- this course, the route is finished): steer at the checkpoint then. Returns ax, ay, az and
-- { remaining = route left (uu), jump_ahead = a launch within NEAR_JUMP, jump_now = under the
-- launch point, ledge = at the foot of a ledge (once per ledge), leg = course name, seg }.
-- The route is never followed past a launch that has not happened: the landing beyond it lies
-- close by on the map but out of reach without it.
function Path.aim(px, py, pz, tx, ty, tz)
    local l, cname = find_leg(tx, ty, tz)
    if l ~= leg then leg, seg, launched, ledges_done = l, 1, {}, {} end
    if not l then return nil end
    local pts = l.pts
    local n = #pts
    if n < 2 then return nil end
    -- The first launch still to come bounds the route for now.
    local limit = n
    for k = 1, n do
        if flag(l, k) == "launch" and not launched[k] then
            local w = pts[k]
            if pz > w[3] + Path.LAUNCHED_DZ and (px - w[1]) ^ 2 + (py - w[2]) ^ 2 <= (3 * Path.JUMP_R) ^ 2 then
                launched[k] = true
                print(string.format("[KakarotAccess] race path -> launched (point %d)\n", k))
            else
                limit = k
                break
            end
        end
    end
    if launched[n] then return nil end   -- launched at the route's end: the checkpoint guides
    cur_limit = limit
    local top = math.max(1, limit - 1)    -- last usable segment
    -- Nearest segment in a window ahead of the current one; the whole route when far off it.
    local best, best_d, best_t
    for i = math.max(1, math.min(seg, top) - 1), math.min(top, seg + WINDOW) do
        local d, t = project(pts, i, px, py)
        if not best_d or d < best_d then best, best_d, best_t = i, d, t end
    end
    if best_d > Path.OFF * Path.OFF then
        for i = 1, top do
            local d, t = project(pts, i, px, py)
            if d < best_d then best, best_d, best_t = i, d, t end
        end
    end
    seg = best
    -- Walk LOOK ahead along the route from the projection, stopping at the launch still to come
    -- and at a ledge not yet jumped: those are steered at exactly.
    local left = Path.LOOK
    local i, t = best, best_t
    local ax, ay, az
    while true do
        local L = seg_len(pts, i)
        local rest = L * (1 - t)
        local stop_here = (i + 1 == limit and flag(l, limit) == "launch")
            or ((flag(l, i + 1) == true or flag(l, i + 1) == "boost") and not ledges_done[i + 1])
        if rest >= left or i >= top or stop_here then
            local f = (L > 0) and math.min(1, t + left / L) or 1
            if rest < left then f = 1 end
            local a, b = pts[i], pts[i + 1]
            ax, ay, az = a[1] + f * (b[1] - a[1]), a[2] + f * (b[2] - a[2]), a[3] + f * (b[3] - a[3])
            break
        end
        left = left - rest
        i, t = i + 1, 0
    end
    -- Route length still to go, and to the launch still to come.
    local remaining = seg_len(pts, best) * (1 - best_t)
    local to_launch = nil
    for k = best + 1, n - 1 do
        if k == limit and flag(l, limit) == "launch" then to_launch = remaining end
        remaining = remaining + seg_len(pts, k)
    end
    if limit == n and flag(l, n) == "launch" then to_launch = remaining end
    if best + 1 == limit and flag(l, limit) == "launch" then to_launch = seg_len(pts, best) * (1 - best_t) end
    -- A ledge on the way: jump at its foot, once per ledge.
    local ledge = false
    for k = math.max(1, best - 1), math.min(n - 1, best + 2) do
        local w = pts[k]
        local f = flag(l, k)
        local r = (f == "boost") and Path.BOOST_R or Path.LEDGE_R
        if (f == true or f == "boost") and not ledges_done[k] and (px - w[1]) ^ 2 + (py - w[2]) ^ 2 <= r * r then
            ledges_done[k], ledge = true, true
            print(string.format("[KakarotAccess] race path -> ledge %d, jump\n", k))
        end
    end
    local jump_now = false
    if flag(l, limit) == "launch" and not launched[limit] then
        local w = pts[limit]
        jump_now = (px - w[1]) ^ 2 + (py - w[2]) ^ 2 <= Path.JUMP_R * Path.JUMP_R
    end
    -- The end of a walking route: hand over to the checkpoint itself.
    if not to_launch and remaining < Path.LOOK then return nil end
    return ax, ay, az, {
        remaining = remaining,
        jump_ahead = to_launch ~= nil and to_launch <= Path.NEAR_JUMP,
        jump_now = jump_now,
        ledge = ledge,
        leg = cname,
        seg = best,
    }
end

function Path.active() return leg ~= nil end

-- A point `dist` (uu) further along the current leg's route from the racer, never past a launch
-- that has not happened (the racer is put under it instead), plus the route's heading there in
-- degrees. For the stuck recovery: race_guide moves the racer there when it cannot get on.
-- Call after Path.aim in the same tick. nil when no route applies.
function Path.ahead(px, py, dist)
    local l = leg
    if not l or #l.pts < 2 then return nil end
    local pts, n = l.pts, #l.pts
    local stop = math.min(n, cur_limit or n)
    local i = math.min(seg, stop - 1)
    local _, t = project(pts, i, px, py)
    while t >= 1 and i + 1 < stop do       -- already past this segment's end
        i = i + 1
        _, t = project(pts, i, px, py)
    end
    local left = dist
    while true do
        local L = seg_len(pts, i)
        local rest = L * (1 - t)
        local a, b = pts[i], pts[i + 1]
        if rest >= left or i + 1 >= stop then
            local f = (L > 0) and math.min(1, t + left / L) or 1
            local yaw = math.deg(math.atan(b[2] - a[2], b[1] - a[1]))
            return a[1] + f * (b[1] - a[1]), a[2] + f * (b[2] - a[2]), a[3] + f * (b[3] - a[3]), yaw
        end
        left = left - rest
        i, t = i + 1, 0
    end
end

return Path
