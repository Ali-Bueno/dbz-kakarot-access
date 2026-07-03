-- Navigation tracker: audio radar that guides the player to the active quest
-- objective (or any marker the game is currently pointing at).
--
-- Model (ported from the validated DBXV2 accessibility mod, see
-- reference/audio-navigation): a repeating beacon ping whose CADENCE encodes
-- distance (closer = faster, like a parking sensor), whose STEREO PAN encodes
-- left/right, and whose PITCH drops when the target is behind the camera.
-- An arrival cue + spoken confirmation fires on reaching the objective.
--
-- Target source — the game's own guidance system, so it always matches what a
-- sighted player sees: the minimap navi icons (UAT_UIMiniMapIcon.TargetActor,
-- verified reflected), classified through the target actor's ATMapIconComponent
-- (EMapIcon). A new quest marker is picked up automatically (auto-track on
-- quest accept); with no quest the same scan finds whatever navi marker the
-- game shows (e.g. a marked map point).
--
-- Improvement over XV2: optional route guidance — the beacon points at the next
-- corner of a NavMesh path (NavigationSystemV1.FindPathToLocationSynchronously,
-- verified present + reflected in this game) instead of straight at the target,
-- while cadence/volume still track the FINAL objective. Falls back to the
-- straight line whenever no path is available.
--
-- All engine access runs on the game thread; every touch of a live UObject is
-- pcall-guarded. The audio math lives here; audio.lua only plays the cues.

local Core = require("ui_core")
local Speech = require("speech")
local Audio = require("audio")
local I18n = require("i18n")
local Registry = require("ui_registry")

local Nav = {}

-- ---- tunables ----------------------------------------------------------------
-- The feel constants (cadence range, volume slope, behind-pitch) are the values
-- validated by ear in the XV2 mod (reference/audio-navigation); distances are
-- converted to UE units (1 m = 100 uu) and rescaled for Kakarot's larger fields.

local TICK_MS = 50                    -- radar loop period (XV2 ticked at 30 ms)
local M = 100                         -- UE units per meter
local NEAR_DIST = 5 * M               -- distance mapped to the FASTEST ping
local FAR_DIST = 200 * M              -- distance mapped to the SLOWEST ping
local MIN_INTERVAL_MS = 120           -- ping period when at NEAR_DIST (XV2 value)
local MAX_INTERVAL_MS = 850           -- ping period when at FAR_DIST (XV2 value)
local VOL_SLOPE = 0.6                 -- volume = 1 - VOL_SLOPE * t (XV2 value)
local BEHIND_PITCH_SLOPE = 0.3        -- pitch = 1 + fb * slope when behind (XV2)
local PITCH_MIN = 0.6                 -- clamp for the behind pitch (XV2 value)
local ARRIVE_DIST = 8 * M             -- 3D arrival radius (XV2 used 4 m; quest
                                      -- triggers here are larger areas)
local REARM_DIST = 12 * M             -- re-arm the arrival cue past this
local WAYPOINT_DIST = 5 * M           -- horizontal radius to advance a route point
local WAYPOINT_VERT = 6 * M           -- vertical slack for waypoint advance (flight)
local SCAN_EVERY = 20                 -- ticks between target re-scans (~1 s)
local RESCAN_CLASSES = 100            -- ticks between FindAllOf refreshes (~5 s)
local LOST_SCANS = 3                  -- missed scans before dropping the target
local REPATH_MS = 3000                -- NavMesh path refresh period
local DIR_CUE_MS = 3000               -- min gap between spoken direction words (XV2)
local ELEV_DIST = 12 * M              -- |dz| that counts as above/below
local ELEV_CUE_MS = 7000              -- min gap between elevation words (XV2)

-- EMapIcon values (AT_enums.hpp) that mark QUEST objectives on the map.
local MAIN_QUEST_ICONS = { [24] = true, [71] = true }   -- MAINQUEST_NO_BATTLE, MAINQUEST_RANGE_ONLY
local SUB_QUEST_ICONS = {                               -- SUBQUEST_* + DLC story sub quests
    [26] = true, [40] = true, [47] = true, [53] = true, [59] = true, [72] = true,
}

-- Target priority classes (higher wins; ties broken by distance).
local PRI_MAIN, PRI_SUB, PRI_OTHER = 3, 2, 1

-- ---- state ---------------------------------------------------------------------
local on = true                -- master switch (F3); radar auto-tracks while on
local route_mode = true        -- NavMesh route guidance (Shift+F3)
local running = false
local tick = 0                 -- loop tick counter; tick * TICK_MS is our clock

local target = nil             -- { actor, key, pri, label }
local target_missing = 0       -- consecutive scans where the target wasn't found
local arrived = false
local last_ping_ms = 0
local last_dir_cue, last_dir_ms = nil, -DIR_CUE_MS
local last_elev_ms = -ELEV_CUE_MS
local gated_prev = false

local route = nil              -- array of {x,y,z} NavMesh corners, or nil
local route_idx = 0
local last_path_ms = -REPATH_MS

local icon_comp_class = nil    -- UClass ATMapIconComponent (cached)
local navi_icons, navi_next = nil, 0

local function now_ms() return tick * TICK_MS end

-- ---- engine reads (all pcall-guarded, game thread only) -------------------------

local function player_pawn()
    local pc = FindFirstOf("PlayerController")
    if not Core.valid(pc) then return nil end
    local ok, pawn = pcall(function() return pc.Pawn end)
    if ok and Core.valid(pawn) then return pawn end
    return nil
end

local function actor_pos(actor)
    local ok, loc = pcall(function() return actor:K2_GetActorLocation() end)
    if ok and loc then return loc.X, loc.Y, loc.Z end
    return nil
end

-- Horizontal camera-forward unit vector (the game is camera-relative, like XV2).
local function camera_forward()
    local pc = FindFirstOf("PlayerController")
    if not Core.valid(pc) then return nil end
    local ok, rot = pcall(function() return pc.PlayerCameraManager:GetCameraRotation() end)
    if not ok or not rot then return nil end
    local yaw = math.rad(rot.Yaw or 0)
    return math.cos(yaw), math.sin(yaw)
end

-- The minimap radar widget: its visibility is the game's own "you are free-roaming"
-- signal (hidden in battle, cutscenes and full-screen menus) — we gate the beacon on it.
local function minimap()
    return Core.cached_live("AT_UIMiniMapRadar", tick)
end

-- ---- target discovery ------------------------------------------------------------

local function icon_component_class()
    if icon_comp_class == nil then
        local ok, cls = pcall(function() return StaticFindObject("/Script/AT.ATMapIconComponent") end)
        icon_comp_class = (ok and cls) or false
    end
    return icon_comp_class or nil
end

-- Priority class of a marker's target actor, from its ATMapIconComponent.MapIconType.
-- Actors without the component (or with a non-quest icon) still rank as PRI_OTHER:
-- the game only creates a NAVI icon for things it is actively guiding the player to.
local function classify(actor)
    local cls = icon_component_class()
    if cls then
        local ok, t = pcall(function()
            local comp = actor:GetComponentByClass(cls)
            if comp and comp:IsValid() then return tonumber(comp.MapIconType) end
            return nil
        end)
        if ok and t then
            if MAIN_QUEST_ICONS[t] then return PRI_MAIN end
            if SUB_QUEST_ICONS[t] then return PRI_SUB end
        end
    end
    return PRI_OTHER
end

-- Is this pooled minimap icon actually in use (one of its widgets rendered)?
local function icon_in_use(icon)
    local ok, vis = pcall(function()
        return Core.on_screen(icon.WL_QuestIconSw) or Core.on_screen(icon.WL_NaviIconSw)
            or Core.on_screen(icon.WL_Icon_ImgSw)
    end)
    return ok and vis == true
end

local function label_for(pri)
    if pri == PRI_MAIN then return I18n.t("nav_main") end
    if pri == PRI_SUB then return I18n.t("nav_sub") end
    return I18n.t("nav_other")
end

-- Scan the game's guidance markers and return the best candidate:
-- highest priority class first, then nearest (3D). px/py/pz = player position.
local function best_candidate(px, py, pz)
    if navi_icons == nil or tick >= navi_next then
        navi_icons = FindAllOf("AT_UIMiniMapNaviIcon") or {}
        navi_next = tick + RESCAN_CLASSES
    end
    local best, best_pri, best_d2 = nil, 0, math.huge
    for _, icon in pairs(navi_icons) do
        if Core.valid(icon) and icon_in_use(icon) then
            local ok, ta = pcall(function() return icon.TargetActor end)
            if ok and Core.valid(ta) then
                local x, y, z = actor_pos(ta)
                if x then
                    local pri = classify(ta)
                    local d2 = (x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2
                    if pri > best_pri or (pri == best_pri and d2 < best_d2) then
                        best, best_pri, best_d2 = ta, pri, d2
                    end
                end
            end
        end
    end
    if best then
        return { actor = best, key = tostring(best:GetAddress()), pri = best_pri,
                 label = label_for(best_pri) }
    end
    return nil
end

-- ---- NavMesh route (the "perfect path" improvement) --------------------------------

local function compute_route(pawn, px, py, pz, tx, ty, tz)
    local ok, pts = pcall(function()
        local navsys = StaticFindObject("/Script/NavigationSystem.Default__NavigationSystemV1")
        if not navsys or not navsys:IsValid() then return nil end
        local path = navsys:FindPathToLocationSynchronously(pawn,
            { X = px, Y = py, Z = pz }, { X = tx, Y = ty, Z = tz }, nil, nil)
        -- NOTE: we don't call the path's reflected IsValid() — UE4SS shadows it with
        -- its own UObject validity check. An unusable path just has <2 PathPoints.
        if not path then return nil end
        local arr = path.PathPoints
        local n = #arr
        if n < 2 then return nil end
        local out = {}
        for i = 1, n do
            local p = arr[i]
            out[i] = { x = p.X, y = p.Y, z = p.Z }
        end
        return out
    end)
    if ok and pts then return pts end
    return nil
end

-- Current guidance point: the next route corner, or the target itself.
-- Advances corners as the player passes them.
local function guidance_point(px, py, pz, tx, ty, tz)
    if not route_mode or not route then return tx, ty, tz end
    while route_idx <= #route do
        local wp = route[route_idx]
        local dh = math.sqrt((wp.x - px) ^ 2 + (wp.y - py) ^ 2)
        if dh <= WAYPOINT_DIST and math.abs(wp.z - pz) <= WAYPOINT_VERT then
            route_idx = route_idx + 1
        else
            return wp.x, wp.y, wp.z
        end
    end
    return tx, ty, tz
end

local function refresh_route(pawn, px, py, pz, tx, ty, tz, force)
    if not route_mode then route = nil return end
    if not force and now_ms() - last_path_ms < REPATH_MS then return end
    last_path_ms = now_ms()
    local pts = compute_route(pawn, px, py, pz, tx, ty, tz)
    if pts then
        route = pts
        route_idx = 2  -- pts[1] is the player's own position
    else
        route = nil
    end
end

-- ---- announcements -----------------------------------------------------------------

local function meters(d) return math.floor(d / M + 0.5) end

-- Camera-relative clock hour (12 = ahead, 3 = right, 6 = behind, 9 = left), from XV2.
local function clock_hour(pan, fb)
    local deg = math.deg(math.atan(pan, fb))
    local h = math.floor(deg / 30 + 0.5)
    if h <= 0 then h = h + 12 end
    return h
end

local function announce_tracking(label, dist)
    Speech.say(string.format(I18n.t("nav_tracking"), label, meters(dist)), false)
end

-- ---- the radar tick -----------------------------------------------------------------

local function drop_target()
    target, route, route_idx = nil, nil, 0
    arrived, target_missing = false, 0
end

local function gated()
    -- Silence the radar whenever a menu/dialog adapter owns the screen, or the game
    -- itself hides the minimap (battle, cutscene, full-screen UI) — menus.md rule.
    if Registry.active_adapter and Registry.active_adapter() ~= nil then return true end
    local mm = minimap()
    if not mm or not Core.on_screen(mm) then return true end
    return false
end

local function step()
    tick = tick + 1
    if not on then return end

    local g = gated()
    if g ~= gated_prev then
        gated_prev = g
        if g then Audio.stop() end
    end
    if g then return end

    local pawn = player_pawn()
    if not pawn then return end
    local px, py, pz = actor_pos(pawn)
    if not px then return end

    -- Re-scan the game's guidance markers ~1/s (even when idle — scanning every
    -- tick would hammer reflection): picks up a fresh quest marker automatically
    -- (auto-track on accept), retargets when the current step is done, and drops
    -- the target when the game stops guiding.
    if tick % SCAN_EVERY == 0 then
        local best = best_candidate(px, py, pz)
        if best then
            if not target or target.key ~= best.key then
                local was = target and target.key
                target = best
                arrived, target_missing = false, 0
                route, route_idx = nil, 0
                local x, y, z = actor_pos(best.actor)
                if x then
                    refresh_route(pawn, px, py, pz, x, y, z, true)
                    local d = math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
                    -- Announce a new objective; stay quiet when merely re-resolving
                    -- the same spot after a scan hiccup.
                    if was ~= best.key then announce_tracking(best.label, d) end
                end
            else
                target_missing = 0
            end
        elseif target then
            target_missing = target_missing + 1
            if target_missing >= LOST_SCANS then drop_target() end
        end
    end

    if not target then return end
    if not Core.valid(target.actor) then drop_target() return end
    local tx, ty, tz = actor_pos(target.actor)
    if not tx then drop_target() return end

    -- Arrival on TRUE 3D distance to the objective (not the route corner).
    local d3 = math.sqrt((tx - px) ^ 2 + (ty - py) ^ 2 + (tz - pz) ^ 2)
    if d3 <= ARRIVE_DIST then
        if not arrived then
            arrived = true
            Audio.arrival()
            Speech.say(I18n.t("nav_arrived"), false)
        end
        return
    end
    if arrived and d3 > REARM_DIST then arrived = false end

    refresh_route(pawn, px, py, pz, tx, ty, tz, false)
    local gx, gy, gz = guidance_point(px, py, pz, tx, ty, tz)

    -- Direction to the CURRENT guidance point, camera-relative (ground plane).
    local dx, dy = gx - px, gy - py
    local dh = math.sqrt(dx * dx + dy * dy)
    local nx, ny = 0, 1
    if dh > 1 then nx, ny = dx / dh, dy / dh end

    local pan, fb = 0, 1
    local fx, fy = camera_forward()
    if fx then
        -- UE left-handed ground plane: forward(yaw) = (cos, sin), right = (-sin, cos).
        local rx, ry = -fy, fx
        pan = nx * rx + ny * ry
        fb = nx * fx + ny * fy
        if pan > 1 then pan = 1 elseif pan < -1 then pan = -1 end
    end

    -- Cadence/volume from the FINAL objective distance; pan/pitch from the
    -- guidance direction. Formulas validated in the XV2 mod.
    local t = (d3 - NEAR_DIST) / (FAR_DIST - NEAR_DIST)
    if t < 0 then t = 0 elseif t > 1 then t = 1 end
    local vol = 1.0 - VOL_SLOPE * t
    local pitch = 1.0
    if fb < 0 then
        pitch = 1.0 + fb * BEHIND_PITCH_SLOPE
        if pitch < PITCH_MIN then pitch = PITCH_MIN end
    end
    local interval = MIN_INTERVAL_MS + (MAX_INTERVAL_MS - MIN_INTERVAL_MS) * t

    local ms = now_ms()
    if ms - last_ping_ms >= interval then
        last_ping_ms = ms
        Audio.ping(pan, vol, pitch)
    end

    -- Sparse spoken cues, XV2 style: the dominant direction word when it CHANGES
    -- (throttled), and above/below when the objective is clearly off our level.
    local dir
    if math.abs(fb) >= math.abs(pan) then
        dir = fb >= 0 and "nav_ahead" or "nav_behind"
    else
        dir = pan >= 0 and "nav_right" or "nav_left"
    end
    if dir ~= last_dir_cue and ms - last_dir_ms >= DIR_CUE_MS then
        last_dir_cue, last_dir_ms = dir, ms
        Speech.say(I18n.t(dir), false)
    end
    local dz = tz - pz
    if math.abs(dz) >= ELEV_DIST and ms - last_elev_ms >= ELEV_CUE_MS then
        last_elev_ms = ms
        Speech.say(I18n.t(dz > 0 and "nav_up" or "nav_down"), false)
    end
end

-- ---- loop management (same generation/busy-guard pattern as ui_core.loop) ------------

function Nav.start()
    if running then return end
    running = true
    _G.__KakarotNavGen = (_G.__KakarotNavGen or 0) + 1
    local myGen = _G.__KakarotNavGen
    local busy = false
    LoopAsync(TICK_MS, function()
        if _G.__KakarotNavGen ~= myGen then return true end
        if not busy then
            busy = true
            ExecuteInGameThread(function()
                local ok, err = pcall(step)
                busy = false
                if not ok then print("[KakarotAccess] nav step error: " .. tostring(err) .. "\n") end
            end)
        end
        return false
    end)
end

function Nav.stop()
    running = false
    _G.__KakarotNavGen = (_G.__KakarotNavGen or 0) + 1  -- kills the loop next fire
    Audio.stop()
end

-- ---- user commands --------------------------------------------------------------------

-- F3: master toggle. Off = immediate silence; on = re-acquire and announce.
function Nav.toggle()
    on = not on
    if not on then
        Audio.stop()
        drop_target()
        Speech.say(I18n.t("nav_off"), true)
    else
        gated_prev = false
        Speech.say(I18n.t("nav_on"), true)
    end
    return on
end

-- Shift+F3: NavMesh route guidance on/off (falls back to straight-line beacon).
function Nav.toggle_route()
    route_mode = not route_mode
    if not route_mode then route, route_idx = nil, 0 end
    Speech.say(I18n.t(route_mode and "nav_route_on" or "nav_route_off"), true)
    return route_mode
end

-- F5: on-demand "where is it?" — type, distance, clock direction, above/below.
function Nav.where()
    ExecuteInGameThread(function()
        if not target or not Core.valid(target.actor) then
            Speech.say(I18n.t("nav_no_target"), true)
            return
        end
        local pawn = player_pawn()
        if not pawn then return end
        local px, py, pz = actor_pos(pawn)
        local tx, ty, tz = actor_pos(target.actor)
        if not px or not tx then return end
        local dx, dy, dz = tx - px, ty - py, tz - pz
        local d3 = math.sqrt(dx * dx + dy * dy + dz * dz)
        local dh = math.sqrt(dx * dx + dy * dy)
        local pan, fb = 0, 1
        local fx, fy = camera_forward()
        if fx and dh > 1 then
            local nx, ny = dx / dh, dy / dh
            pan = nx * -fy + ny * fx
            fb = nx * fx + ny * fy
        end
        local parts = {
            target.label,
            string.format(I18n.t("nav_meters"), meters(d3)),
            string.format(I18n.t("nav_clock"), clock_hour(pan, fb)),
        }
        if math.abs(dz) >= ELEV_DIST then
            parts[#parts + 1] = I18n.t(dz > 0 and "nav_up" or "nav_down")
        end
        Speech.say(table.concat(parts, ", "), true)
    end)
end

-- Ctrl+F5 (dev): dump every guidance candidate + a NavMesh probe to
-- Scripts/dumps/dump_nav_targets.txt so a failing scan can be diagnosed offline.
function Nav.dump()
    ExecuteInGameThread(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_nav_targets.txt", "w")
        if not f then Speech.say("nav dump: cannot open file", true) return end
        f:write("== nav target dump ==\n")
        local pawn = player_pawn()
        local px, py, pz = 0, 0, 0
        if pawn then px, py, pz = actor_pos(pawn) end
        f:write(string.format("player: %.0f %.0f %.0f\n", px or 0, py or 0, pz or 0))
        local mm = minimap()
        f:write("minimap on_screen: " .. tostring(mm and Core.on_screen(mm)) .. "\n")
        local icons = FindAllOf("AT_UIMiniMapNaviIcon") or {}
        local n = 0
        for _, icon in pairs(icons) do
            n = n + 1
            if Core.valid(icon) then
                local ok, ta = pcall(function() return icon.TargetActor end)
                local line = string.format("navi[%d] in_use=%s", n, tostring(icon_in_use(icon)))
                if ok and Core.valid(ta) then
                    local x, y, z = actor_pos(ta)
                    line = line .. string.format(" target=%s pri=%d pos=%.0f %.0f %.0f",
                        ta:GetFullName(), classify(ta), x or 0, y or 0, z or 0)
                end
                f:write(line .. "\n")
            end
        end
        f:write(string.format("navi icon instances: %d\n", n))
        -- Regular minimap icons too (fallback source if navi icons turn out empty).
        local radar = mm
        if Core.valid(radar) then
            local ok, cnt = pcall(function() return #radar.MapIconList end)
            f:write("radar.MapIconList: " .. tostring(ok and cnt or "unreadable") .. "\n")
        end
        -- NavMesh probe: can the engine path from the player to the player?
        if pawn and px then
            local pts = compute_route(pawn, px, py, pz, px + 10 * M, py, pz)
            f:write("navmesh probe: " .. (pts and (#pts .. " points") or "no path") .. "\n")
        end
        f:write("current target: " .. (target and (target.label .. " " .. target.key) or "none") .. "\n")
        f:close()
        Speech.say("nav dump written", true)
    end)
end

function Nav.is_on() return on end

return Nav
