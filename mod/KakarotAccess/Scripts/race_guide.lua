-- Race minigame guide (air car, walking robot or flight race): aims the radar beacon at the
-- next checkpoint and speaks the checkpoint counter as it advances. Runs on the shared tick
-- bus, outside the screen registry, because a race is gameplay, not a menu.
-- Game data, sources and open questions: reference/dbz-kakarot/notes/dbz-kakarot-race-minigame.md

local Core = require("ui_core")
local Directory = require("ui_directory")
local Registry = require("ui_registry")
local Transition = require("transition")
local Audio = require("audio")
local Beacon = require("beacon")
local Rings = require("race_rings")
local Route = require("race_route")
local Path = require("race_path")
local Sensor = require("obstacle_sensor")
local Speech = require("speech")
local I18n = require("i18n")
local Mem = require("mem")            -- crash black box only (Mem.mark)
local PadPoll = require("pad_poll")   -- shared tick bus (dispatch + schedule only, no logic)
local Input = require("input")        -- the player's stick, for the stuck hint only

local Race = {}

local TICK_MS = 100           -- same period as the quest radar, so the cadence matches it
local IDLE_EVERY = 5          -- while no race is up, look for the HUD every 5th tick (~0.5 s)
local END_GRACE_S = 1.5       -- the HUD must stay gone this long before the race counts as over
local M = 100                 -- UE units per meter
local HOST_CLASS = "AT_UIMiniGameRace"   -- directory-mapped: GameHUD.UIMiniGameManager
local TIMER_ROWS = { "WL_Mgame_Race_List00", "WL_Mgame_Race_List01" }
local LIMIT_ROW = "WL_Mgame_Race_List01"   -- titled "Límite de tiempo" (census 2026-10-18)
local LOW_TIME_S = 10         -- warn once when the limit drops to this; the race restarts at 0
local TIMER_EVERY = 5         -- read the limit every 5th tick (~0.5 s), plenty for a 10 s warning
-- Cue loudness over the race's engine and music (player request 2026-10-18: the beacon was
-- too quiet). Above 1 the audio bridge amplifies; beacon.wav peaks at -15 dBFS, so 1.8 is clean.
local BEACON_GAIN = 1.8
-- The other two cues stay at or under the beacon's peak (item.wav peaks at -12 dBFS, enemy.wav
-- at -7), so the checkpoint beacon remains the loudest thing the guide plays.
local RING_CUE, RING_GAIN = "item", 1.2          -- a pickup-like sound: steer INTO it
local OBSTACLE_CUE, OBSTACLE_GAIN = "enemy", 0.7 -- the danger sound: steer AWAY from it
local RING_QUIET_DIST = 15 * M  -- no ring cue while an obstacle is this close
local OBSTACLE_CONFIRM = 2      -- scans in a row before an obstacle is cued
-- Ring and obstacle cues play only in the GAPS between beacon pings (player request
-- 2026-10-18: the cues may overlap a little, but the beacon must stay clear, because it is
-- the one the race cannot be finished without). A secondary cue waits SLOT_AFTER_MS after a
-- ping (or half the ping period when pings are close together), keeps SLOT_BEFORE_MS clear
-- before the next one, and two secondary cues are at least SECONDARY_GAP_MS apart.
local SLOT_AFTER_MS = 200
local SLOT_BEFORE_MS = 100
local SECONDARY_GAP_MS = 200
-- The HUD's pawn is trusted as the racer only this close to the racer's position: in the walking
-- robot race it can be a parked TwinFoot pawn (seen at the origin), whose yaw never turns.
local PAWN_NEAR = 10 * M
local MOVE_STEP = 1 * M       -- travel needed before the movement direction is re-measured

local running = false
local tick = 0
local live_at = nil           -- os.clock() of the last tick that saw the race HUD
local last_ping_ms = 0
local last_count = nil        -- the checkpoint counter text last spoken
local low_warned = false      -- the low-time warning for the current segment was spoken
local last_ring_ms, last_obstacle_ms, last_secondary_ms = 0, 0, 0
local prev_x, prev_y, prev_t = nil, nil, nil
local speed = 0               -- racer ground speed, uu/s, smoothed
local anchor_x, anchor_y = nil, nil   -- where the movement direction was last measured from
local move_fx, move_fy = nil, nil     -- the racer's ground-plane direction of travel
local heading_src = nil       -- which heading the beacon used last (logged on change)
-- The walking robot (ATwinFoot, AT.hpp:18402). In the robot race the player's pawn is GOKU, left
-- standing at the start: live 2026-11-19 he read 0 m from the racer at the start (so his yaw was
-- trusted) and 161 m once the robot moved off. The robot's own yaw is the heading to use.
local ROBOT_CLASS = "TwinFootBP_C"
local robot, robot_next = nil, 0
local route_wp = nil          -- the course waypoint the beacon steered to last (logged on change)
local path_leg = nil          -- the surveyed route being followed (race_path), logged on change
local jump_said, jump_ahead_said = false, false   -- launch prompts already spoken this leg
local jump_ring = nil         -- the floating booster "jump" was last said under
local stop_said = false       -- "stop" said at the current launch point
local stuck_x, stuck_y, stuck_t = nil, nil, 0   -- where the racer last moved from, and when
local STUCK_S, STUCK_D = 2.0, 1.5 * M
local STUCK_MAX = 3           -- prompts per spot: a wall no jump clears should not nag forever
local stuck_n = 0
-- Recovery: still stuck after the jump prompts, or no progress along the route for PROG_S (a fall
-- off a ridge, a pit): the racer is put back on the route RECOVER_D further on, the way the game
-- itself restarts a racer who falls in the water. The robot course has rocks, bones and cracks no
-- survey models exactly (five live routes, 2026-11-20), so finishing must not depend on a perfect
-- route.
local RECOVER_D = 35 * M
local PROG_S, PROG_D = 8.0, 10 * M
-- Near a launch the racer must STOP under the booster and jump: standing still there is the
-- instruction, not a lack of progress (live 2026-11-20, a player run: recovered three times onto
-- the same booster while stopping for it). Waits longer there.
local PROG_LAUNCH_S = 25.0
local RECOVER_AFTER = 1      -- jump prompts that failed before the racer is moved on
local prog_best, prog_t, prog_leg = nil, 0, nil
local STOP_SPEED = 3 * M      -- slow enough to jump at a launch point (uu/s)
local ROBOT_RETRY_S = 2
local seen_obstacle, seen_ring = false, false   -- log edges (tuning aid), not every tick
local boost_ring = nil        -- index of the ring the beacon is steering to, if any
local obstacle_streak = 0     -- consecutive scans with a hit
local obstacle_since, obstacle_min = 0, 0       -- current obstacle episode, for its log line
local warned = {}

local function warn_once(key, msg)
    if warned[key] then return end
    warned[key] = true
    print("[KakarotAccess] race: " .. msg .. "\n")
end

-- The race HUD, if it is on screen. Scan-free: the directory pointer or nothing.
local function race_host()
    for _, w in ipairs(Core.peek_all(HOST_CLASS)) do
        if Core.valid(w) and Core.on_screen(w) and Core.pane_rendered(w) then return w end
    end
    return nil
end

local function xyz(v)
    if v and v.X then return v.X, v.Y, v.Z end
    return nil
end

-- The next checkpoint, as the HUD's own direction arrow knows it; nil before the course is set.
local function next_checkpoint(host)
    local ok, v = pcall(function() return host:GetNextCheckPointLocation() end)
    local x, y, z = xyz(ok and v)
    if not x or (x == 0 and y == 0 and z == 0) then return nil end
    return x, y, z
end

-- The racer's position: the HUD's copy first (it feeds the course map), the pawn as fallback.
local function player_pos(host, pc)
    local ok, v = pcall(function() return host:GetPlayerLocation() end)
    local x, y, z = xyz(ok and v)
    if x and not (x == 0 and y == 0 and z == 0) then return x, y, z, "hud" end
    if not pc then return nil end
    local ok2, loc = pcall(function()
        local pawn = Core.member(pc, "Pawn")
        if Core.valid(pawn) then return pawn:K2_GetActorLocation() end
    end)
    x, y, z = xyz(ok2 and loc)
    if x then return x, y, z, "pawn" end
    return nil
end

-- Ground-plane forward vector the beacon pans against: the racer's own heading (the rider is
-- attached to the vehicle), because the race camera is not the player camera manager's, whose
-- rotation stays frozen for the whole race (measured live 2026-10-18). Camera as the fallback.
local function heading_forward(pc, pawn)
    if not pc then return nil end
    local ok, rot = pcall(function()
        if Core.valid(pawn) then return pawn:K2_GetActorRotation() end
    end)
    if not ok or not rot then
        ok, rot = pcall(function()
            local cam = Core.member(pc, "PlayerCameraManager")
            if Core.valid(cam) then return cam:GetCameraRotation() end
        end)
    end
    if not ok or not rot then return nil end
    local yaw = math.rad(rot.Yaw or 0)
    return math.cos(yaw), math.sin(yaw)
end

-- The racer's direction of travel, re-measured after every MOVE_STEP of ground distance.
local function track_motion(px, py)
    if not anchor_x then anchor_x, anchor_y = px, py return end
    local dx, dy = px - anchor_x, py - anchor_y
    local d = math.sqrt(dx * dx + dy * dy)
    if d < MOVE_STEP then return end
    move_fx, move_fy = dx / d, dy / d
    anchor_x, anchor_y = px, py
end

-- Distance from the pawn to the racer's position; nil when the pawn has no location.
local function pawn_offset(pawn, px, py, pz)
    if not Core.valid(pawn) then return nil end
    local ok, loc = pcall(function() return pawn:K2_GetActorLocation() end)
    local x, y, z = xyz(ok and loc)
    if not x then return nil end
    return math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
end

-- The beacon's forward vector: the pawn's heading while the pawn IS the racer (the car race),
-- else the direction of travel, else the camera. Logs which one whenever it changes.
-- The robot actor of this race, scanned at most every ROBOT_RETRY_S until found (car races have
-- none, so this costs one FindAllOf per 2 s there; only while a race is live).
local function robot_actor(now)
    if robot and Core.valid(robot) then return robot end
    robot = nil
    if now < robot_next then return nil end
    robot_next = now + ROBOT_RETRY_S
    for _, a in pairs(Core.findall(ROBOT_CLASS)) do
        local name = ""
        pcall(function() name = a:GetFullName() end)
        if Core.valid(a) and not tostring(name):find("Default__", 1, true) then robot = a break end
    end
    return robot
end

local function racer_forward(pc, pawn, px, py, pz)
    local off = pawn_offset(pawn, px, py, pz)
    local fx, fy, src
    local rb = robot_actor(os.clock())
    local roff = rb and pawn_offset(rb, px, py, pz)
    if roff and roff <= PAWN_NEAR then
        -- The body's yaw. The robot walks camera-relative behind a follow camera that settles
        -- behind it after ~1 s (live 2026-11-20); the camera itself is not readable (the camera
        -- managers' rotation and the controller's control rotation stay frozen at the start
        -- yaw), so "turn towards the sound" against the body is the frame offered.
        fx, fy = heading_forward(pc, rb)
        if fx then src, pawn, off = "robot", rb, roff end
    end
    if not fx and off and off <= PAWN_NEAR then
        fx, fy = heading_forward(pc, pawn)
        src = "pawn"
    end
    if not fx and move_fx then fx, fy, src = move_fx, move_fy, "movement" end
    if not fx then fx, fy = heading_forward(pc, nil) src = "camera" end
    if src ~= heading_src then
        heading_src = src
        local cls = "?"
        pcall(function() cls = pawn:GetClass():GetFName():ToString() end)
        print(string.format("[KakarotAccess] race heading -> %s (pawn %s, %s m from racer)\n",
            tostring(src), cls, off and string.format("%.0f", off / M) or "?"))
    end
    return fx, fy
end

-- "03/08" -> "3 of 8" (localized); anything else is spoken as shown, minus leading zeros.
function Race.format_count(s)
    local a, b = s:match("^%s*(%d+)%s*/%s*(%d+)%s*$")
    if a then return string.format(I18n.t("race_count"), tonumber(a), tonumber(b)) end
    return (s:gsub("%f[%d]0+(%d)", "%1"))
end

-- The HUD's m'ss.mmm timer ("0'42.803") as whole seconds; nil for any other shape.
function Race.timer_seconds(s)
    local m, sec = tostring(s):match("^%s*(%d+)'(%d+)")
    if not m then return nil end
    return tonumber(m) * 60 + tonumber(sec)
end

-- Seconds as speech: "42 seconds", "1 minute 5 seconds" (localized).
function Race.format_seconds(total)
    local m, sec = math.floor(total / 60), total % 60
    if m == 0 then return string.format(I18n.t("race_time_s"), sec) end
    return string.format(I18n.t("race_time_ms"), m, sec)
end

-- A timer text spoken naturally, or as shown when it is not the HUD's timer shape.
local function spoken_timer(s)
    local total = Race.timer_seconds(s)
    return total and Race.format_seconds(total) or s
end

local function limit_seconds(host)
    local row = Core.member(host, LIMIT_ROW)
    if not (Core.valid(row) and Core.on_screen(row)) then return nil end
    return Race.timer_seconds(Core.read_text(Core.member(row, "WL_Txt_Num_Time")))
end

local function checkpoint_text(host)
    return Core.read_text(Core.member(host, "WL_Txt_Num_Cp"))
end

local function checkpoint_line(text)
    return string.format(I18n.t("race_checkpoint"), Race.format_count(text))
end

-- Ground speed from successive racer positions (wall time), lightly smoothed; it sets how far
-- ahead the obstacle sensor looks.
local function track_speed(px, py, now)
    if prev_t and now > prev_t then
        local v = math.sqrt((px - prev_x) ^ 2 + (py - prev_y) ^ 2) / (now - prev_t)
        speed = speed * 0.5 + v * 0.5
    end
    prev_x, prev_y, prev_t = px, py, now
    return speed
end

-- May a ring or obstacle cue sound on this tick? Only in the gap between beacon pings (see
-- SLOT_AFTER_MS). `interval` is the beacon's current period; pings land on whole ticks.
local function secondary_slot(ms, interval)
    local period = math.ceil(interval / TICK_MS) * TICK_MS
    local phase = ms - last_ping_ms
    return phase >= math.min(SLOT_AFTER_MS, period / 2)
        and period - phase >= SLOT_BEFORE_MS
        and ms - last_secondary_ms >= SECONDARY_GAP_MS
end

local function reset_race()
    live_at, last_count, low_warned = nil, nil, false
    prev_x, prev_y, prev_t, speed = nil, nil, nil, 0
    anchor_x, anchor_y, move_fx, move_fy, heading_src = nil, nil, nil, nil, nil
    robot, robot_next = nil, 0
    route_wp = nil
    Route.reset()
    Path.reset()
    path_leg, jump_said, jump_ahead_said, jump_ring, stop_said = nil, false, false, nil, false
    stuck_x, stuck_y, stuck_t, stuck_n = nil, nil, 0, 0
    prog_best, prog_t, prog_leg = nil, 0, nil
    seen_obstacle, seen_ring, obstacle_streak, boost_ring = false, false, 0, nil
    Rings.reset()
    Sensor.new_race()
end

-- Debounced end (pooled HUDs flicker through camera cuts); a map switch ends it at once.
local function finish(now, force)
    if not live_at then return end
    if not force and now - live_at < END_GRACE_S then return end
    reset_race()
    print("[KakarotAccess] race -> over\n")
end

local function step()
    Mem.mark("race.step")
    tick = tick + 1
    local now = os.clock()
    if Transition.active() then return finish(now, true) end
    if not live_at and tick % IDLE_EVERY ~= 0 then return end
    Core.begin_scan_tick()
    local host = race_host()
    if not host then return finish(now) end
    if not live_at then
        print(string.format("[KakarotAccess] race -> live (minimap up=%s)\n",
            tostring(Core.minimap_peek())))
    end
    live_at = now
    _G.__KakarotMinigameLive = now   -- lets screen_fishresult read the shared result sheet
    if Registry.mutes_audio() then return end   -- a pausing screen sits over the race

    local tx, ty, tz = next_checkpoint(host)
    local pc = tx and Directory.peek("hud", "PlayerOwner")
    local px, py, pz, src
    if tx then px, py, pz, src = player_pos(host, pc) end

    local count = checkpoint_text(host)
    if count and count ~= last_count then
        Speech.say(checkpoint_line(count), last_count ~= nil)
        last_count = count
        print(string.format("[KakarotAccess] race checkpoint -> '%s' next=%s pos=%s (%s)\n",
            count, tx and string.format("%.0f,%.0f,%.0f", tx, ty, tz) or "none",
            px and string.format("%.0f,%.0f,%.0f", px, py, pz) or "none", tostring(src)))
    end

    if tick % TIMER_EVERY == 0 then
        local left = limit_seconds(host)
        if left and left <= LOW_TIME_S and left > 0 and not low_warned then
            low_warned = true
            Speech.say(Race.format_seconds(left), false)
        elseif left and left > LOW_TIME_S then
            low_warned = false   -- a checkpoint topped the limit up: re-arm
        end
    end

    if not tx then return end
    if not px then return warn_once("pos", "no racer position (HUD getter and pawn both failed)") end
    local pawn = Core.member(pc, "Pawn")
    track_motion(px, py)
    local fx, fy = racer_forward(pc, pawn, px, py, pz)
    if not fx then warn_once("heading", "no racer heading, the beacon stays centred") end
    -- A boost ring on the way to the checkpoint takes over the beacon, with the ring's own sound,
    -- until the racer is through it; then the beacon returns to the checkpoint.
    Rings.refresh(now)
    Rings.track(px, py, pz)
    -- A surveyed route for this leg (race_path) overrides boosters and arrows: it already goes
    -- round the water and up to the launch the leg needs.
    local ax, ay, _, pinfo = Path.aim(px, py, pz, tx, ty, tz)
    if (ax ~= nil) ~= (path_leg ~= nil) then
        path_leg = ax and pinfo.leg or nil
        jump_said, jump_ahead_said = false, false
        print(string.format("[KakarotAccess] race path -> %s\n",
            ax and string.format("%s, %.0f m of route", tostring(pinfo.leg), pinfo.remaining / M) or "none"))
    end
    local jump_now = false
    if ax and pinfo.jump_ahead and not jump_ahead_said then
        jump_ahead_said = true
        Speech.say(I18n.t("race_booster"), false)
    end
    -- At the launch point: "stop", then "jump" once the racer has (nearly) stopped. A running
    -- jump flies ~80 m and overshoots a pillar top; a standing one landed on it (live 2026-11-20).
    local stop_now = false
    if ax and pinfo.jump_now then
        if not stop_said then
            stop_said = true
            Speech.say(I18n.t("race_stop"), true)
            print(string.format("[KakarotAccess] race path -> stop (%.0f m/s)\n", speed / M))
        end
        stop_now = not jump_said
        if not jump_said and speed <= STOP_SPEED then
            jump_said, jump_now = true, true
            Speech.say(I18n.t("race_jump"), true)
            print(string.format("[KakarotAccess] race path -> jump (%.0f m/s)\n", speed / M))
        end
    elseif ax then
        stop_said, jump_said = false, false   -- overshot or not there yet: prompt again next time
    end
    if ax and pinfo.ledge then   -- a ledge on the route: race_path says it once per ledge
        jump_now = true
        Speech.say(I18n.t("race_jump"), true)
    end
    -- Stuck: the racer has not moved STUCK_D in STUCK_S while the stick is pushed. On the robot
    -- course that is a step or ledge the 2 m survey cannot see (live 2026-11-20, a gully on leg
    -- 3), and a jump is the way over it, so the hint is "jump".
    if not stuck_x or (px - stuck_x) ^ 2 + (py - stuck_y) ^ 2 > STUCK_D * STUCK_D then
        if stuck_x and (px - stuck_x) ^ 2 + (py - stuck_y) ^ 2 > (4 * STUCK_D) ^ 2 then stuck_n = 0 end
        stuck_x, stuck_y, stuck_t = px, py, now
    elseif now - stuck_t >= STUCK_S and not stop_now and stuck_n < STUCK_MAX and robot_actor(now) then
        local snap = Input.read()
        local pushing = Race._autodrive ~= nil or (snap and snap.lx and snap.lx * snap.lx + snap.ly * snap.ly > 0.25)
        if pushing then
            stuck_t, jump_now, stuck_n = now, true, stuck_n + 1
            Speech.say(I18n.t("race_jump"), true)
            print(string.format("[KakarotAccess] race stuck -> jump at %.0f,%.0f\n", px / M, py / M))
        end
    end
    -- Progress along the route; recovery when stuck past the prompts or not progressing.
    if ax then
        if path_leg ~= prog_leg then prog_leg, prog_best, prog_t = path_leg, nil, now end
        if not prog_best or pinfo.remaining < prog_best - PROG_D then prog_best, prog_t = pinfo.remaining, now end
        local why = (stuck_n >= RECOVER_AFTER and now - stuck_t >= STUCK_S and "stuck")
            or (now - prog_t >= ((pinfo.jump_ahead or pinfo.jump_now) and PROG_LAUNCH_S or PROG_S)
                and "no progress") or nil
        local rb = why and robot_actor(now)
        if rb then
            local rx, ry, rz, ryaw = Path.ahead(px, py, RECOVER_D)
            -- The route's end that close: onto the checkpoint itself, which counts it (a point a
            -- few metres short of it did not, live 2026-11-20).
            if rx and pinfo.remaining <= RECOVER_D and not pinfo.jump_ahead then
                ryaw = math.deg(math.atan(ty - py, tx - px))
                rx, ry, rz = tx, ty, tz - 100
            end
            if rx then
                local ok = pcall(function()
                    rb:K2_SetActorLocation({ X = rx, Y = ry, Z = rz + 200 }, false, {}, true)
                    rb:K2_SetActorRotation({ Pitch = 0, Yaw = ryaw, Roll = 0 }, true)
                end)
                print(string.format("[KakarotAccess] race recover (%s) -> %.0f,%.0f,%.0f %s\n", why,
                    rx / M, ry / M, rz / M, ok and "" or "FAILED"))
                if ok then Speech.say(I18n.t("race_recover"), true) end
                stuck_x, stuck_n, prog_best, prog_t = nil, 0, nil, now
            end
        end
    end
    local bx, by, bz, bdist, bi
    -- The booster on the way to the checkpoint. Without a route it is what the beacon steers to;
    -- with one, the beacon follows the route and the booster keeps its own cue, from its own
    -- direction, between the beacon's pings: the player follows the route AND takes the booster
    -- (player report 2026-11-20: the two used to take turns, the booster went silent on a route).
    local obx, oby, obz, obdist, obi = Rings.on_route(px, py, pz, tx, ty, tz)
    if not ax then bx, by, bz, bdist, bi = obx, oby, obz, obdist, obi end
    -- A booster floating above the ground is a launch pad: under it, the racer must jump.
    local under = bi and Rings.jump_under(bi, px, py, pz)
    if under and jump_ring ~= bi then
        jump_ring, jump_now = bi, true
        Speech.say(I18n.t("race_jump"), true)
        print(string.format("[KakarotAccess] race booster -> jump (ring %d)\n", bi))
    elseif not under then
        jump_ring = nil
    end
    if bi ~= boost_ring then
        boost_ring = bi
        if bi then
            Speech.say(I18n.t("race_booster"), false)
            print(string.format("[KakarotAccess] race booster -> %.0f m (ring %d of %d)\n",
                bdist / M, bi, Rings.count()))
        else
            local nd = Rings.nearest_unused(px, py, pz)
            print(string.format("[KakarotAccess] race booster -> none on the way (nearest unused %s)\n",
                nd and string.format("%.0f m", nd / M) or "none"))
        end
    end
    -- No booster to steer to: follow the course arrows towards the checkpoint (race_route). The
    -- pan comes from the waypoint, the beacon's rate still from the checkpoint distance.
    local wx, wy, wz, wd, wi
    Route.refresh(now)
    if not bx and not ax then wx, wy, wz, wd, wi = Route.next(px, py, pz, tx, ty, tz) end
    if wi ~= route_wp then
        route_wp = wi
        if wi then
            print(string.format("[KakarotAccess] race route -> waypoint %d of %d, %.0f m\n",
                wi, Route.count(), wd / M))
        end
    end
    local dx, dy, dz = tx - px, ty - py, tz - pz
    local dist = math.sqrt(dx * dx + dy * dy + dz * dz)
    if ax then
        dx, dy = ax - px, ay - py
        dist = pinfo.remaining
    elseif bx then
        dx, dy, dz = bx - px, by - py, bz - pz
        dist = math.sqrt(dx * dx + dy * dy + dz * dz)
        -- A launch pad floats: steer at the ground under it, by horizontal distance.
        if Rings.jump_under(bi) ~= nil then dist = math.sqrt(dx * dx + dy * dy) end
    elseif wx then
        dx, dy = wx - px, wy - py
    end
    local pan, fb = Beacon.cam_rel(dx, dy, fx, fy)
    Race._aim = { pan = pan, fb = fb, ring = bi, at = now, px = px, py = py, pz = pz,
        bx = bx, by = by, bz = bz, wp = wi, jump = jump_now, stop = stop_now, dx = dx, dy = dy,
        seg = ax and pinfo.seg or nil }   -- the dev test driver steers by this
    if Race._autodrive then pcall(Race._autodrive, pan, fb) end   -- dev channel only (autodrive)
    local vol, interval
    if bx then vol, interval = Rings.cadence(dist) else vol, interval = Beacon.cadence(dist) end
    local ms = tick * TICK_MS
    if ms - last_ping_ms >= interval then
        last_ping_ms = ms
        if bx then
            Audio.cue(RING_CUE, pan, vol * RING_GAIN, Beacon.behind_pitch(fb))
        else
            Audio.ping(pan, vol * BEACON_GAIN, Beacon.behind_pitch(fb))
        end
    end

    -- Obstacles: the nearest wall-like hit ahead, heard from its own direction. A hit must hold
    -- for OBSTACLE_CONFIRM scans in a row: a real obstacle stays in the rays for several ticks
    -- as it approaches, a terrain bump for one.
    local opan, odist = Sensor.scan(pawn, px, py, pz, fx, fy, track_speed(px, py, now))
    obstacle_streak = opan and obstacle_streak + 1 or 0
    if obstacle_streak < OBSTACLE_CONFIRM then opan, odist = nil, nil end
    if (opan ~= nil) ~= seen_obstacle then
        seen_obstacle = opan ~= nil
        if opan then
            obstacle_since, obstacle_min = now, odist
            print(string.format("[KakarotAccess] race obstacle -> %.0f m pan %.2f speed %.0f m/s\n",
                odist / M, opan, speed / M))
        else
            print(string.format("[KakarotAccess] race obstacle -> clear after %.1f s (nearest %.0f m)\n",
                now - obstacle_since, obstacle_min / M))
        end
    end
    if opan and odist < obstacle_min then obstacle_min = odist end

    -- Boost rings: on a route, the booster on the way (else the nearest one ahead); without one,
    -- the nearest one ahead. Not while an obstacle is close, nor while the beacon steers to one.
    local rpan, rdist = Rings.pick(px, py, pz, fx, fy)
    if ax and obx then
        rpan = Beacon.cam_rel(obx - px, oby - py, fx, fy)
        rdist = obdist
    end
    if bx then rpan = nil end
    if (rpan ~= nil) ~= seen_ring then
        seen_ring = rpan ~= nil
        if rpan then
            print(string.format("[KakarotAccess] race ring ahead -> %.0f m pan %.2f (of %d)\n",
                rdist / M, rpan, Rings.count()))
        end
    end
    if opan and odist < RING_QUIET_DIST then rpan = nil end

    -- One secondary cue per slot between beacon pings: whichever is due, the one that has
    -- waited longer first when both are.
    if not secondary_slot(ms, interval) then return end
    local ovol, ointerval, rvol, rinterval
    if opan then ovol, ointerval = Sensor.cadence(odist) end
    if rpan then rvol, rinterval = Rings.cadence(rdist) end
    local obstacle_due = opan and ms - last_obstacle_ms >= ointerval
    local ring_due = rpan and ms - last_ring_ms >= rinterval
    if obstacle_due and ring_due then
        if last_ring_ms < last_obstacle_ms then obstacle_due = false else ring_due = false end
    end
    if obstacle_due then
        last_obstacle_ms, last_secondary_ms = ms, ms
        Audio.cue(OBSTACLE_CUE, opan, ovol * OBSTACLE_GAIN, 1.0)
    elseif ring_due then
        last_ring_ms, last_secondary_ms = ms, ms
        Audio.cue(RING_CUE, rpan, rvol * RING_GAIN, 1.0)
    end
end

-- True while a race is up (and briefly after, see END_GRACE_S): the quest radar yields the
-- beacon to the checkpoints. Pure Lua; a stalled loop expires it instead of muting forever.
function Race.owns_audio()
    return live_at ~= nil and os.clock() - live_at < END_GRACE_S
end

-- On-demand race status (F10 / L3+Y while racing): checkpoint, distance to it, and the HUD's
-- timers under their own labels. Returns false when no race is up, so the caller reads the quest.
function Race.read_status()
    if not Race.owns_audio() then return false end
    local host = race_host()
    if not host then return false end
    local parts = {}
    local count = checkpoint_text(host)
    if count then parts[#parts + 1] = checkpoint_line(count) end
    local tx, ty, tz = next_checkpoint(host)
    local px, py, pz
    if tx then px, py, pz = player_pos(host, Directory.peek("hud", "PlayerOwner")) end
    if px then
        local dx, dy, dz = tx - px, ty - py, tz - pz
        local d = math.sqrt(dx * dx + dy * dy + dz * dz)
        parts[#parts + 1] = string.format(I18n.t("nav_meters"), math.floor(d / M + 0.5))
    end
    for _, name in ipairs(TIMER_ROWS) do
        local row = Core.member(host, name)
        if Core.valid(row) and Core.on_screen(row) then
            local value = Core.read_text(Core.member(row, "WL_Txt_Num_Time"))
            if value then
                value = spoken_timer(value)
                local label = Core.read_text(Core.member(row, "WL_Txt_Title"))
                parts[#parts + 1] = label and (label .. " " .. value) or value
            end
        end
    end
    if #parts == 0 then return false end
    Speech.say(table.concat(parts, ", "), true)
    return true
end

function Race.start()
    if running then return end
    running = true
    PadPoll.register_every("race", TICK_MS, function()
        local ok, err = pcall(step)
        if not ok then warn_once("err:" .. tostring(err), "guide error: " .. tostring(err)) end
    end)
end

function Race.stop()
    running = false
    PadPoll.unregister("race")
    reset_race()
end

Race._step = step   -- offline tests drive the tick directly

return Race
