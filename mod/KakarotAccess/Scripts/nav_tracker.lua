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
local Transition = require("transition")
local Ray = require("raycast")

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
local ARRIVE_DIST = 8 * M             -- 3D arrival radius for AUTO quest targets (large
                                      -- trigger areas)
local ARRIVE_DIST_MANUAL = 1.5 * M   -- TIGHT arrival for hand-picked targets: a
                                      -- collectible / NPC must be reached precisely
                                      -- (within interact range) — 8 m would announce
                                      -- "arrived" while still metres away (the user hit
                                      -- this: a 6 m collectible read as reached).
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
    "quests", "collectibles", "npc", "fishing", "gathering", "shops", "minigames",
    "dragonball", "other",
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
local RADAR_NPC_CAP = 300 * 100   -- distance limit for the NPC direct-scan (drops the far
                                  -- parked-character pool ~2600 m away)
local RADAR_MAP_CAP = 1000 * 100  -- cap for the game's OWN minimap icons: shows the LOCAL
                                  -- area's gather/shop/minigame/fishing markers without
                                  -- listing the regional ones 2-3 km away (clutter). A
                                  -- hard 300 m was too tight; 5 km too loose. Quests: no
                                  -- limit at all.

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

-- Raw EMapIcon of an actor's ATMapIconComponent, IGNORING bShowMapIcon. A NAVI-guided
-- objective usually has bShowMapIcon=false (the navi arrow replaces the static map
-- icon), yet the component still carries the real MapIconType. Reading it through the
-- bShowMapIcon-gated map_icon_type/icon_info returned nil for every navi'd quest, which
-- then fell through to the MAINQUEST default — so EVERY tracked quest, including sub /
-- secondary story, was announced as "main quest" (user report 2026-07-04). Use this for
-- CLASSIFICATION (type only); keep the gated readers where icon VISIBILITY matters.
local function map_icon_type_any(actor)
    local cls = icon_component_class()
    if not cls then return nil end
    local t
    pcall(function()
        local comp = actor:GetComponentByClass(cls)
        if comp and comp:IsValid() then t = tonumber(comp.MapIconType) end
    end)
    return t
end

-- The guided objective's MAIN/SUB kind, read from the navi WIDGET (not the target
-- actor's EMapIcon). The game drives a UWidgetSwitcher whose active index is the
-- EMapNaviIcon (AT_enums: 0 PLAYER_NAVI, 1 MAIN_QUEST, 2 SUB_QUEST, 3 DLC6_QUEST). The
-- navi target actor often has no ATMapIconComponent type (or a main-coded one), so
-- reading the actor announced EVERY tracked quest as "main" even after switching to the
-- bShowMapIcon-agnostic actor read — the WIDGET is the game's real main/sub signal.
-- Returns an equivalent EMapIcon quest type (24 main / 26 sub) or nil if not a quest.
local function navi_quest_icon(icon)
    local idx
    pcall(function()
        local sw = icon.WL_NaviIconSw
        if not (sw and sw:IsValid()) then return end
        local s = sw.WL_NaviIconSwitcher
        if s and s:IsValid() then idx = tonumber(s.ActiveWidgetIndex) end
        if not (idx and idx >= 1 and idx <= 3) then
            local b = sw.WL_NaviBaseSwitcher
            if b and b:IsValid() then idx = tonumber(b.ActiveWidgetIndex) end
        end
    end)
    if idx == 1 then return 24 end   -- MAIN_QUEST
    if idx == 2 then return 26 end   -- SUB_QUEST
    if idx == 3 then return 59 end   -- DLC6_QUEST (sub story)
    return nil
end

-- MAIN/SUB quest priority from the icon type, or nil for anything else. Uses the
-- bShowMapIcon-agnostic reader so a navi'd quest (static icon hidden) still classifies.
local function quest_pri(actor)
    local t = map_icon_type_any(actor)
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

-- Snap box for projecting an off-mesh point onto the NavMesh (half-extents, UE units).
-- Wide + TALL on Z: the player often stands slightly off the walkable surface and an
-- item can sit on a ledge/step, so a point a few metres off the mesh still resolves.
-- This is the key reason FindPath returned nothing (endpoints off-mesh) in the tight
-- on-foot spots where a route matters most (user 2026-07-04).
local NAV_PROJECT_EXTENT = { X = 250, Y = 250, Z = 600 }

-- Project a world point onto the NavMesh; returns {x,y,z} or nil if it can't snap.
local function project_to_nav(navsys, ctx, x, y, z)
    local out
    pcall(function()
        local p = navsys:ProjectPointToNavigation(ctx, { X = x, Y = y, Z = z }, nil, nil,
            NAV_PROJECT_EXTENT)
        -- A failed projection returns (0,0,0); reject that so we keep the raw point.
        if p and not (p.X == 0 and p.Y == 0 and p.Z == 0) then
            out = { x = p.X, y = p.Y, z = p.Z }
        end
    end)
    return out
end

-- ROOT CAUSE of "no path" in this game: it builds NavMesh ONLY around navigation
-- invokers (bGenerateNavigationOnlyAroundNavigationInvokers = true) — i.e. tiles exist
-- only near active AI/NPCs. Out in the field, away from mobs, there are NO tiles, so
-- FindPath returns nothing no matter how we project. The fix is to register the PLAYER
-- as a navigation invoker, which forces tiles to generate around him. Radius covers the
-- nearby-obstacle case (the tight on-foot spots where a route matters); far objectives
-- keep the straight-line beacon until you get close and tiles fill in. Tiles take a few
-- frames to build after registering, so the first path may be empty — the periodic
-- repath then succeeds.
local NAV_INVOKER_GEN = 6000       -- uu (~60 m) tile-generation radius around the player
local NAV_INVOKER_REMOVAL = 8000   -- uu; must be >= gen (tiles past this are freed)
local invoker_key = nil            -- pawn address we've registered (nil = not registered)
local invoker_nav = nil            -- the live nav system we registered with (for unregister)

-- The LIVE UNavigationSystemV1 for the pawn's world (NOT the CDO): registering an invoker
-- and generating tiles need the world instance. Falls back to the CDO for the stateless
-- static calls (project / findpath) if the getter fails.
local function live_navsys(ctx)
    local cdo = StaticFindObject("/Script/NavigationSystem.Default__NavigationSystemV1")
    if not (cdo and cdo:IsValid()) then return nil end
    local nav
    pcall(function() nav = cdo:GetNavigationSystem(ctx) end)
    if nav and nav:IsValid() then return nav end
    return cdo
end

-- Register the player pawn as a nav invoker once (per pawn). Idempotent by address.
-- Mark registered BEFORE the reflected call: if RegisterNavigationInvoker ever raised
-- an uncatchable C++ abort on this game, marking first bounds the damage to a SINGLE
-- dead tick instead of aborting the nav loop every tick (which would silence the
-- beacon). Worst case the invoker isn't actually set → route falls back to straight line.
local function ensure_invoker(navsys, pawn)
    local key = tostring(pawn:GetAddress())
    if invoker_key == key then return end
    invoker_key, invoker_nav = key, navsys
    pcall(function() navsys:RegisterNavigationInvoker(pawn, NAV_INVOKER_GEN, NAV_INVOKER_REMOVAL) end)
end

-- Stop forcing tile generation (route turned off / level change). Best-effort.
local function clear_invoker()
    if invoker_key and Core.valid(invoker_nav) then
        local pc = FindFirstOf("PlayerController")
        pcall(function()
            local pw = pc and pc.Pawn
            if pw then invoker_nav:UnregisterNavigationInvoker(pw) end
        end)
    end
    invoker_key, invoker_nav = nil, nil
end

-- Is it safe to make the NavMesh reflected calls on this game? They are unverified and
-- this game turns bad property/function access into an UNCATCHABLE C++ abort that pcall
-- can't trap — which, in the per-3s route loop, would blip the beacon repeatedly. So a
-- persistent one-shot probe (survives hot reload via _G): mark "testing" BEFORE the
-- first call; if we ever re-enter still "testing", the previous attempt aborted mid-way
-- → mark "bad" and never touch the NavMesh again this session (straight-line fallback).
-- Reaching the end once marks it "ok" (proven safe) forever.
local function nav_native_bad()
    local st = _G.__KakarotRouteNative
    if st == "bad" then return true end
    if st == "testing" then _G.__KakarotRouteNative = "bad"; return true end  -- prev abort
    return false
end

local function compute_route(pawn, px, py, pz, tx, ty, tz)
    if nav_native_bad() then return nil end
    local probing = _G.__KakarotRouteNative == nil
    if probing then _G.__KakarotRouteNative = "testing" end
    local ok, pts = pcall(function()
        local navsys = live_navsys(pawn)
        if not navsys then return nil end
        ensure_invoker(navsys, pawn)
        -- Snap both endpoints onto the mesh first — FindPath returns nothing when either
        -- end is off-mesh, which was the common failure. Fall back to the raw point.
        local s = project_to_nav(navsys, pawn, px, py, pz) or { x = px, y = py, z = pz }
        local e = project_to_nav(navsys, pawn, tx, ty, tz) or { x = tx, y = ty, z = tz }
        local path = navsys:FindPathToLocationSynchronously(pawn,
            { X = s.x, Y = s.y, Z = s.z }, { X = e.x, Y = e.y, Z = e.z }, nil, nil)
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
    -- Reached here → the reflected calls did NOT abort the process (even if pcall caught a
    -- plain Lua error). Mark the NavMesh path verified-safe so the probe never disables it.
    _G.__KakarotRouteNative = "ok"
    if ok and pts then return pts end
    return nil
end

-- ---- raycast obstacle avoidance (used when there is no NavMesh route) --------------
-- Area02 has no NavMesh, so when pathfinding yields nothing we steer the beacon around
-- obstacles with collision rays: probe straight at the objective; if blocked within
-- LOOKAHEAD, fan out and point the beacon at the nearest CLEAR bearing that still heads
-- roughly toward the objective. No auto-move — the player still walks it themselves.
local AVOID_LOOKAHEAD = 4 * M     -- ray length: detect a wall ~4 m ahead
local AVOID_HEIGHT = 1.0 * M      -- cast from chest height so flat ground isn't hit
local AVOID_OFFSETS = { 25, -25, 50, -50, 75, -75, 100, -100, 130, -130 }  -- deg to test

-- Rotate a unit ground vector by `deg` (left-handed UE ground plane).
local function rotate2d(nx, ny, deg)
    local r = math.rad(deg)
    local c, s = math.cos(r), math.sin(r)
    return nx * c - ny * s, nx * s + ny * c
end

-- Is the segment from the player (at chest height) along (nx,ny) for `dist` clear?
-- Returns true/false, or nil if the trace API is unavailable.
local function bearing_clear(pawn, px, py, pz, nx, ny, dist)
    local sz = pz + AVOID_HEIGHT
    return Ray.clear(pawn, px, py, sz, px + nx * dist, py + ny * dist, sz)
end

-- The steered guidance direction to reach (tx,ty) around obstacles, or nil to go direct.
-- One-shot native-safety probe (shared _G flag with the NavMesh calls' sibling): if the
-- first trace aborts uncatchably, disable avoidance for the session.
local avoid_steering = false      -- currently routing around something (for the spoken cue)
local avoid_cued = false           -- edge-gate for the "going around" announcement
local function steer_around(pawn, px, py, pz, tx, ty, tz)
    if _G.__KakarotRayNative == "bad" then return nil end
    if _G.__KakarotRayNative == "testing" then _G.__KakarotRayNative = "bad"; return nil end
    local dx, dy = tx - px, ty - py
    local dh = math.sqrt(dx * dx + dy * dy)
    if dh < AVOID_LOOKAHEAD then avoid_steering = false; return nil end  -- close: go direct
    local nx, ny = dx / dh, dy / dh
    if _G.__KakarotRayNative == nil then _G.__KakarotRayNative = "testing" end
    local direct = bearing_clear(pawn, px, py, pz, nx, ny, AVOID_LOOKAHEAD)
    _G.__KakarotRayNative = "ok"   -- reached here → the trace call didn't abort
    if direct == nil then return nil end            -- trace unavailable → go direct
    if direct then avoid_steering = false; return nil end  -- clear ahead → go direct
    -- Blocked: find the nearest clear bearing that still heads toward the objective.
    for _, off in ipairs(AVOID_OFFSETS) do
        local rx, ry = rotate2d(nx, ny, off)
        if bearing_clear(pawn, px, py, pz, rx, ry, AVOID_LOOKAHEAD) then
            avoid_steering = true
            return px + rx * AVOID_LOOKAHEAD, py + ry * AVOID_LOOKAHEAD, pz
        end
    end
    avoid_steering = false
    return nil   -- boxed in on all sides → fall back to the direct beacon
end

-- Current guidance point: the next route corner, else a raycast-steered point around an
-- obstacle, else the target itself. Advances corners as the player passes them.
local function guidance_point(pawn, px, py, pz, tx, ty, tz)
    if route_mode and route then
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
    -- No NavMesh route: raycast obstacle avoidance (route_mode is the master switch).
    if route_mode and pawn then
        local gx, gy, gz = steer_around(pawn, px, py, pz, tx, ty, tz)
        if gx then return gx, gy, gz end
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

-- Map-switch flush (transition.lua): the tracked actor, the minimap widget and the
-- navi icon list all lived in the dead level — nil them before any tick could probe
-- a freed object. Pure Lua + native audio stop only.
Transition.on_begin("nav_tracker", function()
    drop_target()
    mm_cache, navi_icons = nil, nil
    mm_retry, navi_next = 0, 0
    route_fails = 0          -- the new level may have a NavMesh even if this one didn't
    world_gone = 0
    invoker_key, invoker_nav = nil, nil   -- pawn/nav system are new next level; re-register
    avoid_steering, avoid_cued = false, false
    gated_prev = true        -- audio is being stopped right here
    Audio.stop()
end)


local function step()
    tick = tick + 1
    if not on then return end

    -- TRANSITION GATE FIRST (pure Lua): a map switch just happened (return to title,
    -- cutscene-driven map change) — every cached engine reference is dying or already
    -- freed, and even probing one is an uncatchable abort. Fully inert until the new
    -- level has settled (transition.lua's grace period).
    if Transition.active() then
        if not gated_prev then
            gated_prev = true
            Audio.stop()
        end
        route, route_idx = nil, 0
        return
    end

    -- UI gate next, touching NO engine object at all (it's a pure Lua check): while
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

    -- Register the player as a NavMesh invoker as soon as we're free-roaming (route mode
    -- only), so tiles are already building before a target is picked. Runs only until
    -- registered (guarded by pawn key), so it's a one-off cost, not per-tick. Goes through
    -- the same one-shot native-safety probe as compute_route (this may be the FIRST nav
    -- reflected call, before any target): if it aborts, the route feature self-disables.
    if route_mode and not invoker_key and not nav_native_bad() then
        if _G.__KakarotRouteNative == nil then _G.__KakarotRouteNative = "testing" end
        local ns = live_navsys(pawn)
        if ns then ensure_invoker(ns, pawn) end
        _G.__KakarotRouteNative = "ok"
    end

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
    local arrive_r = target.manual and ARRIVE_DIST_MANUAL or ARRIVE_DIST
    if d3 <= arrive_r then
        if not arrived then
            arrived = true
            Audio.arrival()
            -- Hand-picked target: you're right on it — prompt the interact button so a
            -- collectible/NPC can be grabbed/talked to. Auto quest target: plain arrival.
            Speech.say(target.manual and I18n.t("nav_arrived_pickup") or I18n.t("nav_arrived"), false)
            -- A hand-picked (manual) target STOPS on arrival: the beacon goes quiet, the
            -- target is dropped, AND the auto-scan is suppressed so it can't immediately
            -- re-acquire the same spot when you walk away. To track again, re-pick from
            -- the R3 menu or press F3. (Auto quest targets keep the re-arm behavior.)
            if target.manual then
                drop_target()
                auto_suppressed = true
            end
        end
        return
    end
    if arrived and d3 > REARM_DIST then arrived = false end

    refresh_route(pawn, px, py, pz, tx, ty, tz, false)
    local gx, gy, gz = guidance_point(pawn, px, py, pz, tx, ty, tz)

    -- Spoken cue when the beacon starts routing around an obstacle (so a sudden change in
    -- the beacon's direction is understood as "go around", not a wrong turn). Edge-gated:
    -- announced once when steering begins, re-armed when the way is clear again.
    if avoid_steering and not avoid_cued then
        avoid_cued = true
        Speech.say(I18n.t("nav_around"), false)
    elseif not avoid_steering and avoid_cued then
        avoid_cued = false
    end

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
                -- busy cleared on ENTRY: an uncatchable C++ error killing this callback
                -- must not leave the guard stuck and the radar silent for the session
                -- (see ui_core.loop for the full rationale).
                busy = false
                local ok, err = pcall(step)
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
        -- transition + ui_muted FIRST (pure Lua), then the world gate — see Nav.where.
        if Transition.active() or ui_muted() or not world_alive() then
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
    if not route_mode then
        route, route_idx = nil, 0
        clear_invoker()   -- stop forcing tile generation when route guidance is off
    end
    Speech.say(I18n.t(route_mode and "nav_route_on" or "nav_route_off"), true)
    return route_mode
end

-- F5: on-demand "where is it?" — type, distance, clock direction, above/below.
function Nav.where()
    ExecuteInGameThread(function()
        -- transition + ui_muted FIRST (pure Lua — probing ANY engine object during a
        -- level teardown can abort), then the world gate, then the target.
        if Transition.active() or ui_muted() or not world_alive()
            or not target or not Core.valid(target.actor) then
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
    return not Transition.active() and not ui_muted() and world_alive()
end

-- Snapshot the currently navigable field targets, grouped into the L1/R1 categories.
-- MUST be called on the game thread (radar_menu wraps it in ExecuteInGameThread).
-- Source = the game's OWN minimap icons (MapIconList) + active navi icons, so it
-- matches what a sighted player sees. Non-quest icons are distance-limited by each
-- icon's SearchRangeRadius (the game's own reveal distance) with a global cap;
-- quests have no limit. Returns an ordered array of non-empty groups:
--   { { key=<group>, name=<localized>, items={ {actor,key,label,dist}, ... } }, ... }
-- Diagnostic: dump every minimap candidate (type/group/dist/range/kept) to
-- dumps/dump_radar.txt each time the R2 menu opens — so "only fishing + quests show"
-- can be diagnosed offline (is a category ABSENT from the minimap, or being FILTERED?).
local RADAR_DEBUG = false  -- heavy diagnostic: FindAllOf probes + UniqueId/InteractComponent
                          -- reflection. Reads UniqueId on arbitrary actors, which can raise
                          -- the uncatchable C++ abort — leave OFF in normal play (turning it
                          -- on stranded do_open and R3 stopped opening, 2026-07-04).
local NAVI_DEBUG = false   -- light + SAFE navi main/sub diagnostic. CONFIRMED 2026-07-04:
                          -- switcher index 1→MAIN(24), 2→SUB(26); actorEMapIcon was always
                          -- nil (why it used to default to main). navi_quest_icon is correct.
                          -- Only safe reads (switcher index + component type); no UniqueId.

-- Named NPC UniqueId → spoken name. The game identifies field characters by codes:
--   Cpl0NN = the main/party characters (Cpl001 Goku, Cpl002 Gohan, …) — recognizable.
--   Npc0NN = anonymous townsfolk — no meaningful name, spoken as the generic "character"
--            noun (distance disambiguates them).
-- Story NPCs may carry a descriptive UniqueId; NPC_NAMES word-matches those. Extend as
-- the diagnostic (dump_radar.txt `name=`) reveals the real ids.
local CPL_NAMES = {
    Cpl001 = "Goku", Cpl002 = "Gohan", Cpl003 = "Piccolo", Cpl004 = "Krillin",
    Cpl005 = "Vegeta", Cpl006 = "Trunks", Cpl007 = "Gotenks", Cpl008 = "Goten",
}
local NPC_NAMES = {
    chichi = "Chi-Chi", gohan = "Gohan", goku = "Goku", krillin = "Krillin",
    kuririn = "Krillin", piccolo = "Piccolo", bulma = "Bulma", roshi = "Master Roshi",
    kame = "Master Roshi", ox = "Ox-King", gyumao = "Ox-King", yamcha = "Yamcha",
    tenshinhan = "Tien", tien = "Tien", chaozu = "Chiaotzu", vegeta = "Vegeta",
    trunks = "Trunks", korin = "Korin", karin = "Korin", yajirobe = "Yajirobe",
    dende = "Dende", mrpopo = "Mr. Popo", popo = "Mr. Popo", kamisama = "Kami",
}

-- Best spoken name for a field NPC from its UniqueId (FName). nil = fall back to the
-- generic "character" noun (anonymous townsfolk — the game has no real name for them).
-- IMPORTANT: read ONLY UniqueId — it's a reflected member. Do NOT touch CharacterName:
-- QuestCharacterBase does NOT declare it, and reading a non-existent property on this
-- game raises a C++ exception pcall CANNOT catch — which froze the game (it aborted
-- right after the menu had blocked the pad, leaving a stuck neutral pad).
local function npc_name(npc)
    local raw
    pcall(function()
        local u = npc.UniqueId
        if u then raw = u:ToString() end
    end)
    if type(raw) ~= "string" or raw == "" or raw == "None" then return nil end
    if CPL_NAMES[raw] then return CPL_NAMES[raw] end
    -- descriptive story ids: word-match known characters
    local key = raw:lower():gsub("[^%a]", "")
    for tok, nm in pairs(NPC_NAMES) do
        if key:find(tok, 1, true) then return nm end
    end
    -- anonymous "Npc0NN": no useful name → generic "character" noun (nil)
    if raw:match("^[Nn]pc%d") then return nil end
    -- some other descriptive id: clean it and speak it raw
    local cleaned = raw:gsub("^[%a]-_", ""):gsub("_", " ")
    return cleaned ~= "" and cleaned or nil
end

function Nav.list_targets()
    if not Nav.field_ready() then return {} end
    local pawn = player_pawn()
    if not pawn then return {} end
    local px, py, pz = actor_pos(pawn)
    if not px then return {} end

    local seen = {}
    local groups = {}
    local diag = RADAR_DEBUG and {} or nil
    -- Core add: place an actor into a group with a spoken noun. range = the icon's own
    -- reveal radius (nil for non-icon sources like NPCs). Distance-limited for non-quest
    -- groups by max(global cap, range) — never tighter than the cap, so an already-shown
    -- icon farther than its small reveal radius isn't wrongly dropped.
    local function add_target(actor, grp, noun, range, src, name)
        if not Core.valid(actor) then return end
        local key = tostring(actor:GetAddress())
        if seen[key] then return end
        local x, y, z = actor_pos(actor)
        if not x then return end
        local d = math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
        local kept = true
        if grp ~= "quests" then
            -- Direct actor scans (NPCs, collectibles) are capped tight (drops the far
            -- parked pool / collectibles across the map); minimap icons get the wide cap
            -- (the game curated them for the sighted player, often km away).
            local cap = (src == "questchar" or src == "collectible") and RADAR_NPC_CAP
                or math.max(RADAR_MAP_CAP, range or 0)
            if d > cap then kept = false end
        end
        if diag then
            local cls = "?"
            pcall(function() cls = actor:GetClass():GetFName():ToString() end)
            local hid, ht = "?", "?"
            pcall(function() hid = tostring(actor.bHidden) end)
            pcall(function() ht = tostring(tonumber(actor.CurrentHiddenType)) end)
            diag[#diag + 1] = string.format(
                "  grp=%s noun=%s name=%s cls=%s d=%.0f range=%s kept=%s src=%s bHidden=%s hiddenType=%s",
                grp, noun, tostring(name), cls, d, tostring(range), tostring(kept), src, hid, ht)
        end
        if not kept then return end
        seen[key] = true
        groups[grp] = groups[grp] or { items = {} }
        groups[grp].items[#groups[grp].items + 1] =
            { actor = actor, key = key, dist = d, noun = noun, name = name }
    end
    -- Add by EMapIcon type (icons): derive group + noun from the type.
    local function add_icon(actor, t, src)
        if not t then return end
        local grp = ICON_GROUP[t] or "other"
        local noun = ICON_NOUN[t] or ("radar_cat_" .. grp)
        local _, range = icon_info(actor)
        add_target(actor, grp, noun, range, src)
    end

    -- 1) active navi guidance (quest arrows) — always quest-classified
    do
        local icons = FindAllOf("AT_UIMiniMapNaviIcon") or {}
        for _, icon in pairs(icons) do
            if Core.valid(icon) and icon_in_use(icon) then
                local ok, ta = pcall(function() return icon.TargetActor end)
                if ok and Core.valid(ta) then
                    -- Classify from the navi WIDGET's EMapNaviIcon switcher (the game's
                    -- real main/sub signal); fall back to the target's EMapIcon type
                    -- (bShowMapIcon-agnostic), then the MAINQUEST default only if both
                    -- are unavailable. Reading the actor alone made every quest "main".
                    add_icon(ta, navi_quest_icon(icon) or map_icon_type_any(ta) or 24, "navi")
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
                        add_icon(ta, (icon_info(ta)), "mapicon")
                    end
                end
            end
        end)
    end
    -- 3) talkable field NPCs (Chi-Chi, shopkeepers, quest givers): AQuestCharacter uses
    -- a MobIconComponent, NOT the ATMapIconComponent, so it's absent from MapIconList
    -- above. Scan them directly. Beyond the distance cap, filter by PRESENCE: the game
    -- keeps a pool of preloaded characters (Trunks, etc.) who are NOT part of the current
    -- story beat and are kept HIDDEN by its "absence observer" (AQuestCharacter.
    -- CurrentHiddenType != 0, or bHidden). Skipping those stops future characters like
    -- Trunks from being tracked before they appear in the story (user report 2026-07-04).
    do
        local function npc_present(npc)
            local hidden = false
            pcall(function() hidden = npc.bHidden end)
            if hidden == true or hidden == 1 then return false end
            local ht = 0
            pcall(function() ht = tonumber(npc.CurrentHiddenType) or 0 end)
            return ht == 0
        end
        local npcs = FindAllOf("QuestCharacter") or {}
        for _, npc in pairs(npcs) do
            if Core.valid(npc) and npc_present(npc) then
                add_target(npc, "npc", "cat_npc", nil, "questchar", npc_name(npc))
            end
        end
    end
    -- 4) collectibles / field points that are NOT minimap icons: Field Memories (the
    -- glowing white flashback items that unlock Z Encyclopedia entries — e.g. the one in
    -- Goku's house after cooking), item pickups, and access-point items (chests, ore).
    -- Classify by class name; distance-capped like NPCs so far collectibles don't clutter.
    do
        -- A collected / inactive collectible is typically hidden — skip hidden actors so
        -- a just-collected Field Memory drops out of the list. Read the REFLECTED AActor
        -- property bHidden (CXX dump @0x90). Do NOT call IsHidden(): it is NOT reflected
        -- in this game, and calling a non-existent member raises the uncatchable C++
        -- error that killed this whole loop (seen live 2026-07-04 — the R3 menu went
        -- dead for the session; same lesson as CharacterName in npc_name above).
        local function visible_actor(a)
            local hidden = false
            pcall(function() hidden = a.bHidden end)
            return not (hidden == true or hidden == 1)
        end
        for _, cls in ipairs({ "FieldActionPointActor", "PlacementObjectInfo", "AccessPointItemBase" }) do
            for _, a in pairs(FindAllOf(cls) or {}) do
                if Core.valid(a) and visible_actor(a) then
                    local cn = "?"
                    pcall(function() cn = a:GetClass():GetFName():ToString() end)
                    local noun = cn:find("Memories", 1, true) and "cat_memory" or "cat_item"
                    add_target(a, "collectibles", noun, nil, "collectible")
                end
            end
        end
    end

    -- SAFE navi-type diagnostic (NAVI_DEBUG): for each in-use quest arrow, the two
    -- switcher indices (EMapNaviIcon: 1 MAIN, 2 SUB, 3 DLC6), what navi_quest_icon
    -- resolved, and the target's own EMapIcon — so a wrong main/sub label is pinned to
    -- the exact source (widget switcher vs actor type). ONLY safe pcall'd reads; NO
    -- UniqueId (that abort is what killed R3 when RADAR_DEBUG was on).
    if NAVI_DEBUG then
        local lines = {}
        local icons = FindAllOf("AT_UIMiniMapNaviIcon") or {}
        for _, icon in pairs(icons) do
            if Core.valid(icon) and icon_in_use(icon) then
                local iidx, bidx = "?", "?"
                pcall(function()
                    local sw = icon.WL_NaviIconSw
                    if sw and sw:IsValid() then
                        local s = sw.WL_NaviIconSwitcher
                        if s and s:IsValid() then iidx = tostring(tonumber(s.ActiveWidgetIndex)) end
                        local b = sw.WL_NaviBaseSwitcher
                        if b and b:IsValid() then bidx = tostring(tonumber(b.ActiveWidgetIndex)) end
                    end
                end)
                local resolved, atype = "nil", "nil"
                pcall(function() resolved = tostring(navi_quest_icon(icon)) end)
                pcall(function()
                    local ta = icon.TargetActor
                    if Core.valid(ta) then atype = tostring(map_icon_type_any(ta)) end
                end)
                lines[#lines + 1] = string.format(
                    "  NAVI iconSwitcher=%s baseSwitcher=%s resolved=%s actorEMapIcon=%s",
                    iidx, bidx, resolved, atype)
            end
        end
        if #lines > 0 then
            local src = debug.getinfo(1, "S").source:sub(2)
            local dir = src:match("^(.*)[/\\]") or "."
            local f = io.open(dir .. "\\dumps\\dump_radar.txt", "a")
            if f then
                f:write(string.format("[%d] navi types: %d\n", os.time(), #lines))
                f:write(table.concat(lines, "\n") .. "\n")
                f:close()
            end
        end
    end

    if diag then
        -- Probe nearby actor classes that are NOT minimap icons, to see what's actually
        -- trackable near the player (e.g. gatherable field items — apples/herbs/ore —
        -- and the raw NPC ids so their cryptic names can be mapped or bettered).
        local function probe(cls, off)
            local out = {}
            pcall(function()
                for _, a in pairs(FindAllOf(cls) or {}) do
                    if Core.valid(a) then
                        local x, y, z = actor_pos(a)
                        if x then
                            local d = math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
                            if d <= 500 * 100 then
                                local nm = ""
                                if off then pcall(function() nm = a.UniqueId:ToString() end) end
                                out[#out + 1] = string.format("%.0fm %s %s",
                                    d, a:GetClass():GetFName():ToString(), nm)
                            end
                        end
                    end
                end
            end)
            table.sort(out)
            return out
        end
        local pr = {}
        for _, c in ipairs({ "PlacementObjectInfo", "PlacementObjectInfo_Recyclable",
                             "AccessPointItemBase", "AccessPointBase", "ChestAccessPoint",
                             "TreasureAccessPoint", "FieldActionPointActor", "QuestCharacter" }) do
            local list = probe(c, c == "QuestCharacter")
            pr[#pr + 1] = string.format("  probe %s: %d within 500m", c, #list)
            for i = 1, math.min(#list, 12) do pr[#pr + 1] = "    " .. list[i] end
        end
        -- The player's InteractComponent = the game's OWN "what can I examine/talk to
        -- right now" list (class-agnostic). If "search the house" examine spots aren't
        -- scannable actors, THIS is how we'd detect them: stand next to one and see if
        -- TalkObjectArray populates. Dump its state.
        pcall(function()
            local ic = pawn.InteractComponent
            if not Core.valid(ic) then pr[#pr + 1] = "  InteractComponent: nil" return end
            local n = 0
            pcall(function() n = #ic.TalkObjectArray end)
            pr[#pr + 1] = string.format("  InteractComponent.TalkObjectArray: %d", n)
            pcall(function()
                for i = 1, n do
                    local o = ic.TalkObjectArray[i]
                    local cls, owner = "?", "?"
                    pcall(function() cls = o:GetClass():GetFName():ToString() end)
                    pcall(function() owner = o:GetOuter():GetFName():ToString() end)
                    pr[#pr + 1] = string.format("    talkobj[%d] %s outer=%s", i, cls, owner)
                end
            end)
        end)
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_radar.txt", "a")
        if f then
            f:write(string.format("[%d] list_targets: %d candidates\n", os.time(), #diag))
            f:write(table.concat(diag, "\n") .. "\n")
            f:write(table.concat(pr, "\n") .. "\n")
            f:close()
        end
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

-- The spoken label of a picker item: the resolved NPC name if we have one, else the
-- localized category noun.
function Nav.item_label(item)
    return item.name or I18n.t(item.noun)
end

-- "<label>, N meters" for a picker item (used by radar_menu for announcements).
function Nav.item_phrase(item)
    return string.format("%s, %s", Nav.item_label(item),
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
        local trans = Transition.active()
        local muted = ui_muted()
        f:write(string.format("on=%s route_mode=%s transition=%s ui_muted=%s world_alive=%s adapter_index=%s\n",
            tostring(on), tostring(route_mode), tostring(trans), tostring(muted),
            tostring(not trans and not muted and world_alive()),
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
        -- Gates: mid-transition, with a menu up or the world hidden, actor reads
        -- below could abort.
        if trans or muted or not world_alive() then
            f:write("GATED (" .. (trans and "transition" or muted and "ui" or "world")
                .. ") — actor sections skipped\n")
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
        -- NavMesh probe: invoker state + whether tiles are still building + can the
        -- engine path a short hop from the player? (10 m ahead.) If "building=true",
        -- retry the probe a moment later; if invoker=nil the registration never ran.
        if pawn and px then
            local ns = live_navsys(pawn)
            local building = "?"
            if ns then pcall(function() building = tostring(ns:IsNavigationBeingBuilt(pawn)) end) end
            f:write(string.format("nav invoker registered=%s building=%s\n",
                tostring(invoker_key ~= nil), building))
            local pts = compute_route(pawn, px, py, pz, px + 10 * M, py, pz)
            f:write("navmesh probe (10m ahead): " .. (pts and (#pts .. " points") or "no path") .. "\n")
        end
        -- RAYCAST probe (obstacle avoidance): cast a ray CAMERA-FORWARD at chest height,
        -- 5 m, against each object type separately, so we can confirm the trace API works
        -- and WHICH collision channel detects walls. Point the camera at a wall up close
        -- and at open space, then Ctrl+F5 each — the wall should read blocked, open clear.
        if pawn and px then
            local fx, fy = camera_forward()
            if fx then
                local sz = pz + AVOID_HEIGHT
                local d = 5 * M
                for _, ot in ipairs({ 1, 2, 3, 4, 5, 6 }) do
                    local blocked, dist = Ray.probe(pawn, px, py, sz,
                        px + fx * d, py + fy * d, sz, { ot })
                    f:write(string.format("raycast objType=%d -> %s%s\n", ot,
                        (blocked == nil and "API-UNAVAILABLE"
                            or blocked and "BLOCKED" or "clear"),
                        (dist and string.format(" dist=%.0f", dist) or "")))
                end
            else
                f:write("raycast probe: no camera forward\n")
            end
        end
        f:write("current target: " .. (target and (target.label .. " " .. target.key) or "none") .. "\n")
        f:close()
        Speech.say("nav dump written", true)
    end)
end

function Nav.is_on() return on end

return Nav
