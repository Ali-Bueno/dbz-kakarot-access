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

local TICK_MS = 100                   -- radar loop period. 50 ms felt smoother but its
                                      -- per-tick reflection reads cost real frame time
                                      -- (user-visible lag, 2026-07-03); at 100 ms the
                                      -- radar is inaudibly different and half the cost.
local M = 100                         -- UE units per meter
local NEAR_DIST = 5 * M               -- distance mapped to the FASTEST ping
local FAR_DIST = 200 * M              -- distance mapped to the SLOWEST ping
local MIN_INTERVAL_MS = 100           -- ping period when at NEAR_DIST (XV2 used 120;
                                      -- 100 = one ping per tick at the closest range)
local MAX_INTERVAL_MS = 850           -- ping period when at FAR_DIST (XV2 value)
local VOL_SLOPE = 0.6                 -- volume = 1 - VOL_SLOPE * t (XV2 value)
local BEHIND_PITCH_SLOPE = 0.3        -- pitch = 1 + fb * slope when behind (XV2)
local PITCH_MIN = 0.6                 -- clamp for the behind pitch (XV2 value)
local ARRIVE_DIST = 8 * M             -- 3D arrival radius (XV2 used 4 m; quest
                                      -- triggers here are larger areas)
local REARM_DIST = 12 * M             -- re-arm the arrival cue past this
local WAYPOINT_DIST = 5 * M           -- horizontal radius to advance a route point
local WAYPOINT_VERT = 6 * M           -- vertical slack for waypoint advance (flight)
local SCAN_EVERY = 15                 -- ticks between target re-scans (~1.5 s)
local RESCAN_CLASSES = 100            -- ticks between FindAllOf refreshes (~10 s)
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

-- ---- radar target-picker categories (hold-R2 menu, radar_menu.lua) ---------------
-- EMapIcon (AT_enums.hpp) grouped into the player-facing categories the hold-R2 menu
-- cycles with L1/R1. Anything unmapped falls into "other". The group order is the
-- L1/R1 tab order (empty groups are skipped when cycling).
local GROUP_ORDER = {
    "quests", "npc", "fishing", "gathering", "shops", "minigames", "dragonball", "other",
}
local ICON_GROUP = {
    -- quests (no distance limit)
    [24] = "quests", [71] = "quests", [26] = "quests", [40] = "quests",
    [47] = "quests", [53] = "quests", [59] = "quests", [72] = "quests",
    -- story / community NPCs
    [4] = "npc", [10] = "npc", [42] = "npc", [56] = "npc", [61] = "npc", [41] = "npc",
    -- fishing
    [5] = "fishing",
    -- gathering (collect / hunt / ore / bugs / rocks)
    [6] = "gathering", [7] = "gathering", [8] = "gathering", [18] = "gathering",
    [63] = "gathering", [68] = "gathering", [69] = "gathering",
    -- shops / stalls / eateries
    [1] = "shops", [2] = "shops", [3] = "shops", [51] = "shops", [54] = "shops",
    [55] = "shops", [58] = "shops", [66] = "shops",
    -- minigames
    [11] = "minigames", [12] = "minigames", [13] = "minigames", [14] = "minigames",
    [37] = "minigames",
    -- dragon balls
    [28] = "dragonball",
}
-- Per-EMapIcon spoken noun (i18n key) for the focused item; falls back to the group
-- name when the type has no specific noun. Keeps items in a group distinguishable
-- beyond distance (e.g. "ore" vs "gathering point").
local ICON_NOUN = {
    [24] = "nav_main", [71] = "nav_main",
    [26] = "nav_sub", [40] = "nav_sub", [47] = "nav_sub",
    [53] = "nav_sub", [59] = "nav_sub", [72] = "nav_sub",
    [5] = "cat_fishing", [6] = "cat_collect", [7] = "cat_hunt", [8] = "cat_ore",
    [28] = "cat_dragonball",
    [1] = "cat_food_shop", [2] = "cat_cooking_shop", [3] = "cat_material_shop",
    [58] = "cat_restaurant",
}
local RADAR_GLOBAL_CAP = 300 * 100   -- fallback distance limit (m) for non-quest icons

-- ---- state ---------------------------------------------------------------------
local on = true                -- master switch (F3); radar auto-tracks while on
local route_mode = true        -- NavMesh route guidance (Shift+F3)
local running = false
local tick = 0                 -- loop tick counter; tick * TICK_MS is our clock

local target = nil             -- { actor, key, pri, label, manual }
local target_missing = 0       -- consecutive scans where the target wasn't found
local companion_idx = 0        -- Shift+F5 cycle: 0 = quest mode; 1..n = that companion
local auto_suppressed = false  -- after reaching / stopping a target the auto-scan stays
                               -- quiet (no re-acquire) until you re-pick (R2 menu) or F3.
                               -- Fixes: a reached manual target re-arming when you walk
                               -- away because the auto-scan grabbed it again.
local arrived = false
local last_ping_ms = 0
local last_dir_cue, last_dir_ms = nil, -DIR_CUE_MS
local last_elev_ms = -ELEV_CUE_MS
local gated_prev = false
local world_gone = 0           -- consecutive ticks with the world gate closed
local WORLD_DROP_TICKS = 50    -- ~5 s hidden -> assume level change/battle, drop target

local route = nil              -- array of {x,y,z} NavMesh corners, or nil
local route_idx = 0
local last_path_ms = -REPATH_MS
local route_fails = 0          -- consecutive pathfinding failures
local ROUTE_FAIL_MAX = 8       -- give up after this many (no NavMesh in this game?);
                               -- Shift+F3 re-arms

local icon_comp_class = nil    -- UClass ATMapIconComponent (cached)
local navi_icons, navi_next = nil, 0

local function now_ms() return tick * TICK_MS end

-- ---- engine reads (all pcall-guarded, game thread only) -------------------------

-- UI gate — comfort, not safety: a menu/dialog adapter owning the screen mutes the
-- radar audio (menus.md rule) AND blocks any object-array scan (see minimap below).
-- Adapters that DON'T pause the game (ambient chatter, the resident controls guide)
-- opt out with `nav_mute = false`.
local function ui_muted()
    local a = Registry.active_adapter and Registry.active_adapter()
    return a ~= nil and a.nav_mute ~= false
end

-- The minimap radar widget: its visibility is the game's own "you are free-roaming"
-- signal (hidden in battle, cutscenes and full-screen menus) — we gate the beacon on
-- it, and it also carries the game's OWN pointers to the field character (PlayerIns)
-- and the camera manager (CameraMng), both verified reflected.
--
-- PERFORMANCE-critical acquisition: FindAllOf scans EVERY UObject, and doing that
-- once per second while a save was LOADING stretched a 15 s load past a minute
-- (2026-07-03). So: own cache, NEVER scan while another screen owns the display
-- (title / loading / menus — the world is gated there anyway), and back off 5 s
-- between scans while the widget doesn't exist yet.
local mm_cache = nil
local mm_retry = 0
local MM_RETRY_TICKS = 50    -- ~5 s between acquisition scans

local function minimap()
    if Core.valid(mm_cache) then return mm_cache end
    mm_cache = nil
    if ui_muted() then return nil end
    if tick < mm_retry then return nil end
    mm_retry = tick + MM_RETRY_TICKS
    mm_cache = Core.first_live("AT_UIMiniMapRadar")
    return mm_cache
end

-- The field player actor. PlayerController.Pawn proved UNRELIABLE in the overworld
-- (nil / a stale pawn at the origin — seen live 2026-07-03), so the PRIMARY source is
-- the minimap radar's own PlayerIns (AAT_Character); the pawn is only a fallback.
local function player_pawn()
    local mm = minimap()
    if Core.valid(mm) then
        local ok, ins = pcall(function() return mm.PlayerIns end)
        if ok and Core.valid(ins) then return ins end
    end
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
-- Same source preference as the player: the radar's CameraMng first.
local function camera_forward()
    local rot
    local mm = minimap()
    if Core.valid(mm) then
        local ok, r = pcall(function() return mm.CameraMng:GetCameraRotation() end)
        if ok then rot = r end
    end
    if not rot then
        local pc = FindFirstOf("PlayerController")
        if not Core.valid(pc) then return nil end
        local ok, r = pcall(function() return pc.PlayerCameraManager:GetCameraRotation() end)
        if not ok or not r then return nil end
        rot = r
    end
    local yaw = math.rad(rot.Yaw or 0)
    return math.cos(yaw), math.sin(yaw)
end

-- ---- target discovery ------------------------------------------------------------

local function icon_component_class()
    if icon_comp_class == nil then
        local ok, cls = pcall(function() return StaticFindObject("/Script/AT.ATMapIconComponent") end)
        icon_comp_class = (ok and cls) or false
    end
    return icon_comp_class or nil
end

-- Raw EMapIcon of an actor's ATMapIconComponent, or nil (no component / hidden icon —
-- the game clears finished markers via bShowMapIcon/RemoveMapIcon).
local function map_icon_type(actor)
    local cls = icon_component_class()
    if not cls then return nil end
    local t
    pcall(function()
        local comp = actor:GetComponentByClass(cls)
        if comp and comp:IsValid() then
            if comp.bShowMapIcon == false then return end
            t = tonumber(comp.MapIconType)
        end
    end)
    return t
end

-- Map icon type AND search-range radius in one component fetch. SearchRangeRadius is
-- the game's own "distance at which this icon appears" (UATMapIconComponent, reflected
-- @0x104) — we use it as the per-item visibility limit so the target list matches what
-- a sighted player would see. Returns type, range (both may be nil).
local function icon_info(actor)
    local cls = icon_component_class()
    if not cls then return nil, nil end
    local t, range
    pcall(function()
        local comp = actor:GetComponentByClass(cls)
        if comp and comp:IsValid() then
            if comp.bShowMapIcon == false then return end
            t = tonumber(comp.MapIconType)
            range = tonumber(comp.SearchRangeRadius)
        end
    end)
    return t, range
end

-- MAIN/SUB quest priority from the icon type, or nil for anything else.
local function quest_pri(actor)
    local t = map_icon_type(actor)
    if t and MAIN_QUEST_ICONS[t] then return PRI_MAIN end
    if t and SUB_QUEST_ICONS[t] then return PRI_SUB end
    return nil
end

-- Priority class of a NAVI marker's target actor. Actors without a quest icon type
-- still rank as PRI_OTHER: the game only creates a NAVI icon for things it is
-- actively guiding the player to.
local function classify(actor)
    return quest_pri(actor) or PRI_OTHER
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

-- Scan the game's guidance markers and return the best candidate.
-- Source 1 — NAVI icons: what the game is ACTIVELY guiding to (quest arrow/marker).
--   Any in-use navi icon is a candidate; highest priority class, then nearest (3D).
-- Source 2 (only when no navi icon exists, e.g. between quest steps) — the regular
--   minimap icon list, accepting ONLY actors whose map icon type is a quest type
--   (never shops/fishing/etc.). No visibility requirement: a quest giver beyond
--   minimap range still counts.
local function best_candidate(px, py, pz)
    if navi_icons == nil or tick >= navi_next then
        navi_icons = FindAllOf("AT_UIMiniMapNaviIcon") or {}
        navi_next = tick + RESCAN_CLASSES
    end
    local best, best_pri, best_d2 = nil, 0, math.huge
    local function consider(ta, pri)
        local x, y, z = actor_pos(ta)
        if not x then return end
        local d2 = (x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2
        if pri > best_pri or (pri == best_pri and d2 < best_d2) then
            best, best_pri, best_d2 = ta, pri, d2
        end
    end

    for _, icon in pairs(navi_icons) do
        if Core.valid(icon) and icon_in_use(icon) then
            local ok, ta = pcall(function() return icon.TargetActor end)
            if ok and Core.valid(ta) then consider(ta, classify(ta)) end
        end
    end

    if not best then
        local mm = minimap()
        if Core.valid(mm) then
            pcall(function()
                local arr = mm.MapIconList
                for i = 1, #arr do
                    local icon = arr[i]
                    if Core.valid(icon) then
                        local ta = icon.TargetActor
                        if Core.valid(ta) then
                            local pri = quest_pri(ta)
                            if pri then consider(ta, pri) end
                        end
                    end
                end
            end)
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
    if not route_mode or route_fails >= ROUTE_FAIL_MAX then route = nil return end
    if not force and now_ms() - last_path_ms < REPATH_MS then return end
    last_path_ms = now_ms()
    local pts = compute_route(pawn, px, py, pz, tx, ty, tz)
    if pts then
        route = pts
        route_idx = 2  -- pts[1] is the player's own position
        route_fails = 0
    else
        route = nil
        route_fails = route_fails + 1
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
    companion_idx = 0   -- a dropped manual target reverts to quest mode
end

-- WORLD GATE — safety-critical. The minimap is the game's own "the world is alive
-- and I'm free-roaming" signal; while it is missing/hidden (loading, LEVEL TEARDOWN,
-- battle, cutscene, full-screen UI) the tracker must not touch ANY world actor:
-- dereferencing a just-destroyed actor is an uncatchable C++ abort that pcall cannot
-- catch (crashed the game on "return to title", 2026-07-03). Pooled UI widgets (the
-- minimap itself) are safe to probe; world actors are only safe behind this gate.
local function world_alive()
    local mm = minimap()
    return Core.valid(mm) and Core.on_screen(mm)
end


local function step()
    tick = tick + 1
    if not on then return end

    -- UI gate FIRST, touching NO engine object at all (it's a pure Lua check): while
    -- a pausing menu/dialog/loading screen owns the display, this loop goes fully
    -- inert. SAFETY-CRITICAL, not just comfort: "return to title" tears the level
    -- down BEHIND its confirm dialog + loading screen, and even probing the
    -- minimap widget then (it lives under the per-level HUD, unlike the pooled
    -- GameInstance menus) is an uncatchable abort — that was the return-to-title
    -- crash that survived the first world gate (2026-07-03). Non-pausing adapters
    -- (chatter, controls guide) opt out via nav_mute=false and don't gate us.
    if ui_muted() then
        if not gated_prev then
            gated_prev = true
            Audio.stop()
        end
        route, route_idx = nil, 0
        return
    end

    -- World gate second: minimap missing/hidden (loading, battle, cutscene) -> no
    -- world-actor reads. Routes die with the level; after ~5 s gated the target
    -- actor is presumed gone too and is dropped — the scan re-acquires afterwards.
    if not world_alive() then
        if not gated_prev then
            gated_prev = true
            Audio.stop()
        end
        route, route_idx = nil, 0
        world_gone = world_gone + 1
        if world_gone >= WORLD_DROP_TICKS and target then drop_target() end
        return
    end
    world_gone = 0
    if gated_prev then gated_prev = false end

    local pawn = player_pawn()
    if not pawn then return end
    local px, py, pz = actor_pos(pawn)
    if not px then return end

    -- Re-scan the game's guidance markers ~1/s (even when idle — scanning every
    -- tick would hammer reflection): picks up a fresh quest marker automatically
    -- (auto-track on accept), retargets when the current step is done, and drops
    -- the target when the game stops guiding. A MANUAL target (companion tracking,
    -- Shift+F5) is never overridden by the auto-scan.
    if tick % SCAN_EVERY == 0 and not auto_suppressed and not (target and target.manual) then
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
            -- A hand-picked (manual) target STOPS on arrival: the beacon goes quiet, the
            -- target is dropped, AND the auto-scan is suppressed so it can't immediately
            -- re-acquire the same spot when you walk away (the bug the user hit). To
            -- track again, re-pick from the R2 menu or press F3. (Auto quest targets
            -- keep the re-arm behavior — the quest system moves the marker.)
            if target.manual then
                drop_target()
                auto_suppressed = true
            end
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
        companion_idx = 0
        Speech.say(I18n.t("nav_off"), true)
    else
        gated_prev = false
        auto_suppressed = false   -- F3 on resumes auto quest tracking after a stop/arrival
        Speech.say(I18n.t("nav_on"), true)
    end
    return on
end

-- ---- companion tracking (Shift+F5) -----------------------------------------------

-- Field party members: live AT_Character actors other than the player. The player
-- himself is one (AT_Character_cpl001_BP_C), so exclude him by address. Also exclude
-- far-away actors: the game parks a POOL of ~100 preloaded characters at one distant
-- spot (seen live: everything but Gohan at 2651 m), and those are not companions.
local COMPANION_MAX_DIST = 300 * M   -- a real party member is nearby

local function companions(px, py, pz)
    local me = player_pawn()
    local me_key = me and tostring(me:GetAddress()) or ""
    local out = {}
    for _, c in pairs(FindAllOf("AT_Character") or {}) do
        if Core.valid(c) and tostring(c:GetAddress()) ~= me_key then
            local x, y, z = actor_pos(c)
            if x then
                local d = math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
                if d <= COMPANION_MAX_DIST then
                    out[#out + 1] = { actor = c, d = d }
                end
            end
        end
    end
    table.sort(out, function(a, b) return a.d < b.d end)
    return out
end

-- Shift+F5: cycle nearest companion -> next -> ... -> back to the quest objective.
-- A companion target is MANUAL: the auto-scan won't steal it back.
function Nav.cycle_companion()
    ExecuteInGameThread(function()
        -- ui_muted FIRST (pure Lua), then the world gate — see Nav.where.
        if ui_muted() or not world_alive() then
            Speech.say(I18n.t("nav_no_companion"), true)
            return
        end
        local pawn = player_pawn()
        if not pawn then return end
        local px, py, pz = actor_pos(pawn)
        if not px then return end
        local list = companions(px, py, pz)
        if #list == 0 then
            companion_idx = 0
            Speech.say(I18n.t("nav_no_companion"), true)
            return
        end
        companion_idx = companion_idx + 1
        if companion_idx > #list then
            -- Wrapped past the last companion: back to automatic quest tracking.
            companion_idx = 0
            drop_target()
            Speech.say(I18n.t("nav_quest_mode"), true)
            return
        end
        local c = list[companion_idx]
        local label = string.format(I18n.t("nav_companion"), companion_idx, #list)
        target = { actor = c.actor, key = tostring(c.actor:GetAddress()),
                   pri = PRI_OTHER, label = label, manual = true }
        arrived, target_missing = false, 0
        route, route_idx = nil, 0
        Speech.say(string.format(I18n.t("nav_tracking"), label, meters(c.d)), true)
    end)
end

-- Shift+F3: NavMesh route guidance on/off (falls back to straight-line beacon).
function Nav.toggle_route()
    route_mode = not route_mode
    route_fails = 0   -- re-arm pathfinding attempts
    if not route_mode then route, route_idx = nil, 0 end
    Speech.say(I18n.t(route_mode and "nav_route_on" or "nav_route_off"), true)
    return route_mode
end

-- F5: on-demand "where is it?" — type, distance, clock direction, above/below.
function Nav.where()
    ExecuteInGameThread(function()
        -- ui_muted FIRST (pure Lua — probing ANY engine object during a menu-covered
        -- level teardown can abort), then the world gate, then the target.
        if ui_muted() or not world_alive() or not target or not Core.valid(target.actor) then
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

-- ---- hold-R2 target picker support (radar_menu.lua) --------------------------------
-- All world-actor reads for the picker live HERE, behind the same safety gates and
-- caches as the radar, so radar_menu.lua stays pure input + UI.

-- Free-roam gate for the picker: TRUE only in the RPG overworld. ui_muted (a pausing
-- menu/dialog owns the screen) OR the world gate closed (minimap hidden = BATTLE,
-- cutscene, loading, full-screen menu) both return false — so the R2 menu can NEVER
-- open in combat or block combat input. Pure-Lua ui_muted is checked first (probing
-- the world during a menu-covered teardown can abort).
function Nav.field_ready()
    return not ui_muted() and world_alive()
end

-- Snapshot the currently navigable field targets, grouped into the L1/R1 categories.
-- MUST be called on the game thread (radar_menu wraps it in ExecuteInGameThread).
-- Source = the game's OWN minimap icons (MapIconList) + active navi icons, so it
-- matches what a sighted player sees. Non-quest icons are distance-limited by each
-- icon's SearchRangeRadius (the game's own reveal distance) with a global cap;
-- quests have no limit. Returns an ordered array of non-empty groups:
--   { { key=<group>, name=<localized>, items={ {actor,key,label,dist}, ... } }, ... }
function Nav.list_targets()
    if not Nav.field_ready() then return {} end
    local pawn = player_pawn()
    if not pawn then return {} end
    local px, py, pz = actor_pos(pawn)
    if not px then return {} end

    local seen = {}
    local groups = {}
    local function add(actor, t)
        if not t or not Core.valid(actor) then return end
        local key = tostring(actor:GetAddress())
        if seen[key] then return end
        local grp = ICON_GROUP[t] or "other"
        local x, y, z = actor_pos(actor)
        if not x then return end
        local d = math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
        if grp ~= "quests" then
            -- distance limit = the game's own reveal radius for this icon, else the cap
            local _, range = icon_info(actor)
            local cap = (range and range > 0) and range or RADAR_GLOBAL_CAP
            if d > cap then return end
        end
        seen[key] = true
        groups[grp] = groups[grp] or { items = {} }
        groups[grp].items[#groups[grp].items + 1] = {
            actor = actor, key = key, dist = d,
            noun = ICON_NOUN[t] or ("radar_cat_" .. grp),
        }
    end

    -- 1) active navi guidance (quest arrows) — always quest-classified
    do
        local icons = FindAllOf("AT_UIMiniMapNaviIcon") or {}
        for _, icon in pairs(icons) do
            if Core.valid(icon) and icon_in_use(icon) then
                local ok, ta = pcall(function() return icon.TargetActor end)
                if ok and Core.valid(ta) then
                    local t = (icon_info(ta)) or 24   -- default MAINQUEST if untyped
                    add(ta, t)
                end
            end
        end
    end
    -- 2) every minimap icon with a typed component
    local mm = minimap()
    if Core.valid(mm) then
        pcall(function()
            local arr = mm.MapIconList
            for i = 1, #arr do
                local icon = arr[i]
                if Core.valid(icon) then
                    local ta = icon.TargetActor
                    if Core.valid(ta) then
                        add(ta, (icon_info(ta)))
                    end
                end
            end
        end)
    end

    local out = {}
    for _, gk in ipairs(GROUP_ORDER) do
        local g = groups[gk]
        if g and #g.items > 0 then
            table.sort(g.items, function(a, b) return a.dist < b.dist end)
            out[#out + 1] = { key = gk, name = I18n.t("radar_cat_" .. gk), items = g.items }
        end
    end
    return out
end

-- Localized "noun, N meters" for a picker item (used by radar_menu for announcements).
function Nav.item_phrase(item)
    return string.format("%s, %s", I18n.t(item.noun),
        string.format(I18n.t("nav_meters"), meters(item.dist)))
end

-- Stop tracking the current target on demand (the R2 menu's B button). Silences the
-- beacon and drops the target; if a quest is active the auto-scan may re-acquire it
-- next tick (that's the base radar), but a hand-picked target stays gone until
-- re-picked. Does NOT turn the whole radar off (F3 does that).
function Nav.stop_tracking()
    local had = target ~= nil
    drop_target()
    auto_suppressed = true   -- stay quiet until re-pick / F3 (don't auto-grab a quest)
    Audio.stop()
    if had then Speech.say(I18n.t("nav_stopped"), true) end
end

-- Commit a picker choice: track this actor as a MANUAL target (auto-scan won't steal
-- it) and make sure the radar is on. Game-thread only (radar_menu calls it there).
function Nav.set_manual_target(actor, key, label)
    if not Core.valid(actor) then return false end
    on = true
    auto_suppressed = false   -- an explicit pick resumes normal tracking
    gated_prev = false
    companion_idx = 0
    target = { actor = actor, key = key or tostring(actor:GetAddress()),
               pri = PRI_OTHER, label = label, manual = true }
    arrived, target_missing = false, 0
    route, route_idx = nil, 0
    local pawn = player_pawn()
    local d = nil
    if pawn then
        local px, py, pz = actor_pos(pawn)
        local tx, ty, tz = actor_pos(actor)
        if px and tx then
            d = math.sqrt((tx - px) ^ 2 + (ty - py) ^ 2 + (tz - pz) ^ 2)
        end
    end
    if d then
        Speech.say(string.format(I18n.t("nav_tracking"), label, meters(d)), true)
    else
        Speech.say(string.format(I18n.t("nav_tracking"), label, 0), true)
    end
    return true
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
        -- Full radar state, so a silent radar can be diagnosed from this file alone.
        -- ui_muted is checked FIRST and short-circuits: with a menu up we must not
        -- probe the minimap at all (level teardown hides behind menus).
        local muted = ui_muted()
        f:write(string.format("on=%s route_mode=%s ui_muted=%s world_alive=%s adapter_index=%s\n",
            tostring(on), tostring(route_mode), tostring(muted),
            tostring(not muted and world_alive()),
            tostring(Registry.active_index and Registry.active_index())))
        -- KeyConfig button-resolver state (for the fishing/QTE button announcements).
        pcall(function()
            f:write("keyconfig bindings: " .. require("ui_archetypes").bindings_status() .. "\n")
        end)
        -- Reader-tick cost telemetry (see ui_core.loop) — resets after each dump.
        local st = _G.__KakarotStepStats
        if st and st.n > 0 then
            f:write(string.format("ui step ms: max=%.1f avg=%.2f over %d ticks\n",
                st.max, st.sum / st.n, st.n))
            _G.__KakarotStepStats = nil
        end
        -- Gates: with a menu up or the world hidden, actor reads below could abort.
        if muted or not world_alive() then
            f:write("GATED (" .. (muted and "ui" or "world") .. ") — actor sections skipped\n")
            f:write("current target: " .. (target and (target.label .. " " .. target.key) or "none") .. "\n")
            f:close()
            Speech.say("nav dump written (gated)", true)
            return
        end
        local mm = minimap()
        f:write("minimap on_screen: " .. tostring(mm and Core.on_screen(mm)) .. "\n")
        -- Player position from BOTH sources (the radar's PlayerIns is the primary).
        if Core.valid(mm) then
            local ok, ins = pcall(function() return mm.PlayerIns end)
            if ok and Core.valid(ins) then
                local x, y, z = actor_pos(ins)
                f:write(string.format("radar.PlayerIns: %s pos=%s %s %s\n",
                    ins:GetFullName(), tostring(x), tostring(y), tostring(z)))
            else
                f:write("radar.PlayerIns: INVALID\n")
            end
        end
        do
            local pc = FindFirstOf("PlayerController")
            local okp, pw = pcall(function() return pc.Pawn end)
            if Core.valid(pc) and okp and Core.valid(pw) then
                local x, y, z = actor_pos(pw)
                f:write(string.format("pc.Pawn: %s pos=%s %s %s\n",
                    pw:GetFullName(), tostring(x), tostring(y), tostring(z)))
            else
                f:write("pc.Pawn: INVALID\n")
            end
        end
        local fx, fy = camera_forward()
        f:write(string.format("camera_forward: %s %s\n", tostring(fx), tostring(fy)))
        local pawn = player_pawn()
        local px, py, pz
        if pawn then px, py, pz = actor_pos(pawn) end
        f:write(string.format("resolved player: %s pos=%s %s %s\n",
            pawn and pawn:GetFullName() or "NONE", tostring(px), tostring(py), tostring(pz)))
        px, py, pz = px or 0, py or 0, pz or 0
        local icons = FindAllOf("AT_UIMiniMapNaviIcon") or {}
        local n = 0
        for _, icon in pairs(icons) do
            n = n + 1
            if Core.valid(icon) then
                local ok, ta = pcall(function() return icon.TargetActor end)
                local line = string.format("navi[%d] in_use=%s", n, tostring(icon_in_use(icon)))
                if ok and Core.valid(ta) then
                    local x, y, z = actor_pos(ta)
                    local raw
                    pcall(function()
                        local comp = ta:GetComponentByClass(icon_component_class())
                        if comp and comp:IsValid() then raw = tonumber(comp.MapIconType) end
                    end)
                    line = line .. string.format(" target=%s pri=%d icontype=%s pos=%.0f %.0f %.0f",
                        ta:GetFullName(), classify(ta), tostring(raw), x or 0, y or 0, z or 0)
                end
                f:write(line .. "\n")
            end
        end
        f:write(string.format("navi icon instances: %d\n", n))
        -- Regular minimap icons (the fallback source): list every entry that has a
        -- typed map-icon component; count the untyped rest.
        if Core.valid(mm) then
            local ok, cnt = pcall(function() return #mm.MapIconList end)
            f:write("radar.MapIconList: " .. tostring(ok and cnt or "unreadable") .. "\n")
            pcall(function()
                local arr = mm.MapIconList
                local untyped = 0
                for i = 1, #arr do
                    local icon = arr[i]
                    if Core.valid(icon) then
                        local ta = icon.TargetActor
                        if Core.valid(ta) then
                            local t = map_icon_type(ta)
                            if t then
                                local x, y, z = actor_pos(ta)
                                f:write(string.format(
                                    "  mapicon[%d] icontype=%d quest_pri=%s pos=%.0f %.0f %.0f %s\n",
                                    i, t, tostring(quest_pri(ta)), x or 0, y or 0, z or 0,
                                    ta:GetFullName()))
                            else
                                untyped = untyped + 1
                            end
                        end
                    end
                end
                f:write(string.format("  (+%d icons with no typed component)\n", untyped))
            end)
        end
        -- Field characters (companion-tracking source): every AT_Character + distance.
        do
            local list = companions(px, py, pz)
            f:write(string.format("companions (AT_Character, excl. player): %d\n", #list))
            for i, c in ipairs(list) do
                f:write(string.format("  companion[%d] d=%.0fm %s\n",
                    i, c.d / M, c.actor:GetFullName()))
            end
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
