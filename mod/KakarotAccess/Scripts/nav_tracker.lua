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
local ELEV_DEADBAND_DEG = 18          -- elevation half-angle (deg) still read as "level"
local ELEV_CUE_MS = 7000              -- min gap between elevation words (XV2)
local DIST_CUE_MS = 8000              -- max silence before a distance-only cue: every
                                      -- spoken cue carries the distance (user request
                                      -- 2026-07-06, "am I getting closer?"), and when
                                      -- nothing else fires (straight approach, no
                                      -- direction change) a bare "N meters" fills in
local STEALTH_CUE_MS = 2500           -- min gap between facing-zone words (deer hunt)
local STEALTH_CONE = 0.34             -- |dot| above this = clearly front/behind (~±70°)
local AIM_ALIGN_PAN = 0.12            -- |pan| below this = camera centered on the target
                                      -- (~±7°, matches the field Ki-blast reticle slack)
local AIM_RANGE = 60 * M              -- aim cue only within Ki-blast range
local ENEMY_ALERT_DIST = 50 * M       -- proximity radius for the spoken enemy warning
local ENEMY_CUE_MS = 8000             -- min gap between enemy warnings (anti-spam)
local ENEMY_CLOSER_FACTOR = 0.5       -- re-announce the SAME enemy only when this much closer

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
    "quests", "collectibles", "npc", "enemies", "sites", "fishing", "gathering",
    "shops", "minigames", "dragonball", "other",
}
local ICON_GROUP = {
    -- quests (no distance limit)
    [24] = "quests", [71] = "quests", [26] = "quests", [40] = "quests",
    [47] = "quests", [53] = "quests", [59] = "quests", [72] = "quests",
    -- story / community NPCs
    [4] = "npc", [42] = "npc", [56] = "npc", [61] = "npc", [41] = "npc",
    -- INFORMATION_STORE is a shop (sells map intel), not a character (was
    -- miscategorized under npc — Area02 dump 2026-07-06)
    [10] = "shops",
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
    -- interactable SITES (EMapIcon names in AT_enums.hpp): places you use rather than
    -- pick up — the cooking bonfire, training grounds/room, Turtle School, facilities.
    [15] = "sites",   -- PRACTICE
    [16] = "sites",   -- PRACTICE_BATTLE
    [17] = "sites",   -- DEVELOPMENT
    [19] = "sites",   -- TIME_MACHINE
    [25] = "sites",   -- TRAININGROOM (learn Super Attacks)
    [27] = "sites",   -- TURTLESCHOOL
    [39] = "sites",   -- GroupBattlePractice
    [43] = "sites",   -- Submarine
    [44] = "sites",   -- SaiyanHangout
    [45] = "sites",   -- MedicalRoom
    [46] = "sites",   -- StoryReplay
    [48] = "sites",   -- ClearedPractice
    [57] = "sites",   -- ReturnToMain
    [62] = "sites",   -- SkyseedPlatform
    [64] = "sites",   -- Bonfire (campfire cooking)
    [67] = "sites",   -- ToFloatingIsland
    -- field ENEMIES (EMapIcon names in AT_enums.hpp)
    [30] = "enemies", -- EVIL_ENEMY
    [31] = "enemies", -- EVIL_ENEMY_BOSS
    [32] = "enemies", -- ENEMIES_BASE
    [34] = "enemies", -- EVIL_ENEMY_2
    [35] = "enemies", -- EVIL_ENEMY_BOSS_2
    [36] = "enemies", -- ENEMIES_AIRSHIP
    [49] = "enemies", -- GRD_Enemy
    [50] = "enemies", -- GRD_SuperEnemyGroup
    [52] = "enemies", -- GRD_EnemyGroup
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
    [58] = "cat_restaurant", [10] = "cat_info_shop",
    -- sites
    [15] = "cat_practice", [16] = "cat_practice_battle", [17] = "cat_develop",
    [19] = "cat_timemachine", [25] = "cat_trainroom", [27] = "cat_turtleschool",
    [43] = "cat_submarine", [45] = "cat_medical", [46] = "cat_replay",
    [62] = "cat_skyplatform", [64] = "cat_bonfire",
    -- enemies
    [30] = "cat_enemy", [34] = "cat_enemy", [49] = "cat_enemy",
    [31] = "cat_enemy_boss", [35] = "cat_enemy_boss",
    [32] = "cat_enemy_base", [36] = "cat_enemy_airship",
    [50] = "cat_enemy_group", [52] = "cat_enemy_group",
}
-- EMapIcon types that are field enemies (for the proximity alert): derived from
-- ICON_GROUP so the two can never drift apart.
local ENEMY_ICONS = {}
for t, g in pairs(ICON_GROUP) do
    if g == "enemies" then ENEMY_ICONS[t] = true end
end
-- EAccessPointState::State_Taken (AT_enums.hpp): a collected access-point item keeps
-- its actor until respawn with InteractState == Taken (bHidden stays false, which is
-- why the old hidden-only filter kept listing collected items — user 2026-07-06); a
-- respawn puts it back to State_Wait, so filtering on Taken also re-lists respawns.
local STATE_TAKEN = 11
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
local last_elev_zone = nil     -- last spoken vertical zone; gates elevation by CHANGE
local last_dist_ms = 0         -- last spoken cue that included the distance
local last_stealth_zone, last_stealth_ms = nil, -STEALTH_CUE_MS
local was_on_target = false    -- aim-alignment cue edge-gate
local gated_prev = false
local world_gone = 0           -- consecutive ticks with the world gate closed
local WORLD_DROP_TICKS = 50    -- ~5 s hidden -> assume level change/battle, drop target

-- Dynamic chaining (user request 2026-07-06): after reaching a hand-picked item, sweep
-- the category — retarget the nearest remaining item of the same group, on and on,
-- until the player picks something else (R3) or stops (B / F3). Works for EVERY picker
-- category except quests (those already have the auto-radar re-arm behavior).
-- The advance trigger must NOT depend on a reliable "collected" flag (the game's taken
-- state proved inconsistent across item kinds): advance when the reached actor is
-- gone/hidden/Taken OR when the player simply walks away from the reached spot. A
-- visited-key set guarantees forward progress even when an item never reports collected.
local function chainable(grp) return grp ~= nil and grp ~= "quests" end
local chain_wait = nil         -- { actor, key, grp, stateful } while at the reached item
local chain_seen = {}          -- keys already visited in this sweep (never re-targeted)

local last_enemy_key = nil     -- enemy proximity alert: edge-gate per enemy actor
local last_enemy_d = 0
local last_enemy_ms = -ENEMY_CUE_MS
local enemy_cache, enemy_next = nil, 0   -- cached field-enemy actor list (sparse rescan)
local last_aim_key = nil       -- field aim announcer: edge-gate per locked actor
local mob_base_cls = nil       -- cached UClass AT_MobBase (wild animals/robots tree)

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

-- PUBLIC: the localized POI noun for an actor (its EMapIcon type -> ICON_NOUN, else the
-- group name), or nil if the actor carries no map-icon type. Same classification the radar
-- uses, exposed so the area-map reader (screen_map) can announce the focused POI in the same
-- vocabulary. Example: an ore actor -> I18n.t("cat_ore").
function Nav.icon_noun(actor)
    if not actor then return nil end
    local t = map_icon_type_any(actor)
    if not t then return nil end
    local grp = ICON_GROUP[t] or "other"
    return I18n.t(ICON_NOUN[t] or ("radar_cat_" .. grp))
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

-- The game's floating story-destination markers are plain Actors NAMED
-- Map_Icon_Mission* (Area02 dump 2026-07-06) carrying a generic icon type: they are
-- the quest DESTINATION whenever the navi arrow has no TargetActor (location-guided
-- beats like "find Gohan"). Recognized by the game's own actor name.
local function is_mission_marker(actor)
    local nm
    pcall(function() nm = actor:GetFName():ToString() end)
    return nm ~= nil and nm:find("Mission", 1, true) ~= nil
end

-- MAIN/SUB quest priority from the icon type, or nil for anything else. Uses the
-- bShowMapIcon-agnostic reader so a navi'd quest (static icon hidden) still classifies.
-- A named mission marker counts as a quest objective regardless of its icon type.
local function quest_pri(actor)
    local t = map_icon_type_any(actor)
    if t and MAIN_QUEST_ICONS[t] then return PRI_MAIN end
    if t and SUB_QUEST_ICONS[t] then return PRI_SUB end
    if is_mission_marker(actor) then return PRI_OTHER end
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
-- navdata (optional): pin the projection to ONE RecastNavMesh — with three agent
-- meshes on a map, projecting against "any" can resolve on a mesh the path query
-- won't use.
-- CAUTION: this MUST stay on the FVector-returning overload. K2_ProjectPointToNavigation
-- (bool + FVector& out) ABORTS UNCATCHABLY on this game — tried 2026-07-06, it killed
-- the dump mid-file; same pattern as the LineTrace overloads (FHitResult& out): on this
-- game/UE4SS 3.0.1, reflected calls with out-params are the abort family. Trade-off:
-- this overload returns the INPUT point unchanged on failure, so a "projection" is not
-- proof of mesh under the point — only the (0,0,0) case is a detectable failure.
local function project_to_nav(navsys, ctx, x, y, z, navdata)
    local out
    pcall(function()
        local p = navsys:ProjectPointToNavigation(ctx, { X = x, Y = y, Z = z }, navdata,
            nil, NAV_PROJECT_EXTENT)
        if p and not (p.X == 0 and p.Y == 0 and p.Z == 0) then
            out = { x = p.X, y = p.Y, z = p.Z }
        end
    end)
    return out
end

-- The level's HUMAN RecastNavMesh actor. This game ships one mesh per agent type
-- (RecastNavMesh-Human / -Dinosaur / -Animal, dump 2026-07-06) and a plain pawn
-- PathfindingContext produced an EMPTY path (PathPoints=0, partial=false) even with
-- both endpoints projected — the query ran on the wrong mesh. PathfindingContext
-- also accepts a NavigationData ACTOR, which pins the query to that exact mesh.
-- Cached (a level actor: MUST be flushed on transition, see the flush below).
local human_mesh = nil
local function human_nav_data()
    if Core.valid(human_mesh) then return human_mesh end
    human_mesh = nil
    pcall(function()
        for _, a in pairs(FindAllOf("RecastNavMesh") or {}) do
            if Core.valid(a) then
                local nm = a:GetFName():ToString()
                if nm:find("Human", 1, true) then human_mesh = a return end
                human_mesh = human_mesh or a   -- any mesh beats none
            end
        end
    end)
    return human_mesh
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
        -- Pin projection AND query to the Human mesh (see human_nav_data): with a
        -- plain pawn context the query came back EMPTY on the multi-agent maps.
        local mesh = human_nav_data()
        -- Snap both endpoints onto the mesh first — FindPath returns nothing when either
        -- end is off-mesh, which was the common failure. Fall back to the raw point.
        local s = project_to_nav(navsys, pawn, px, py, pz, mesh) or { x = px, y = py, z = pz }
        local e = project_to_nav(navsys, pawn, tx, ty, tz, mesh) or { x = tx, y = ty, z = tz }
        local path = navsys:FindPathToLocationSynchronously(pawn,
            { X = s.x, Y = s.y, Z = s.z }, { X = e.x, Y = e.y, Z = e.z }, mesh or pawn, nil)
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
-- Returns true/false, or nil if the trace API is unavailable. `api` picks the trace
-- overload (see ray_api): "objects" = LineTraceSingleForObjects, "channel" =
-- LineTraceSingle on the Visibility channel.
local RAY_CHANNEL = 0   -- ETraceTypeQuery: 0 = TraceTypeQuery1 = Visibility (default UE map)
local function bearing_clear(pawn, px, py, pz, nx, ny, dist, api)
    local sz = pz + AVOID_HEIGHT
    if api == "channel" then
        return Ray.clear_channel(pawn, px, py, sz, px + nx * dist, py + ny * dist, sz,
            RAY_CHANNEL)
    end
    return Ray.clear(pawn, px, py, sz, px + nx * dist, py + ny * dist, sz)
end

-- Which trace overload is still usable here? ForObjects first (validated where it
-- works), else the channel overload: ForObjects aborts uncatchably on some maps
-- (Area02 + Area04, 2026-07-06 — TArray marshalling suspected) where the simpler
-- LineTraceSingle may survive. EACH overload has its own abort fuse, so fusing one
-- falls through to the other instead of killing avoidance outright.
local function ray_api()
    if _G.__KakarotRayNative ~= "bad" then return "__KakarotRayNative", "objects" end
    if _G.__KakarotRayChan ~= "bad" then return "__KakarotRayChan", "channel" end
    return nil
end

-- The steered guidance direction to reach (tx,ty) around obstacles, or nil to go direct.
local avoid_steering = false      -- currently routing around something (for the spoken cue)
local avoid_cued = false           -- edge-gate for the "going around" announcement
local function steer_around(pawn, px, py, pz, tx, ty, tz)
    local fuse, api = ray_api()
    if not fuse then return nil end   -- every overload fused on this map
    -- Re-entered while still "testing": the PREVIOUS tick's trace aborted uncatchably
    -- mid-flight — fuse THIS overload. Armed around EVERY trace batch (not only the
    -- first): the abort proved area-dependent (worked on the 07-04 map, aborts in
    -- Area02/Area04) — a per-session one-shot "ok" would leave the loop aborting
    -- every tick in a bad area, silencing the beacon. Next tick ray_api falls
    -- through to the next overload, if any survives.
    if _G[fuse] == "testing" then _G[fuse] = "bad"; return nil end
    local dx, dy = tx - px, ty - py
    local dh = math.sqrt(dx * dx + dy * dy)
    if dh < AVOID_LOOKAHEAD then avoid_steering = false; return nil end  -- close: go direct
    local nx, ny = dx / dh, dy / dh
    _G[fuse] = "testing"
    local direct = bearing_clear(pawn, px, py, pz, nx, ny, AVOID_LOOKAHEAD, api)
    if direct == nil then
        _G[fuse] = "ok"
        return nil                                  -- trace unavailable → go direct
    end
    if direct then
        _G[fuse] = "ok"
        avoid_steering = false
        return nil                                  -- clear ahead → go direct
    end
    -- Blocked: find the nearest clear bearing that still heads toward the objective.
    local gx, gy
    for _, off in ipairs(AVOID_OFFSETS) do
        local rx, ry = rotate2d(nx, ny, off)
        if bearing_clear(pawn, px, py, pz, rx, ry, AVOID_LOOKAHEAD, api) then
            avoid_steering = true
            gx, gy = px + rx * AVOID_LOOKAHEAD, py + ry * AVOID_LOOKAHEAD
            break
        end
    end
    _G[fuse] = "ok"   -- the whole batch completed without aborting
    if gx then return gx, gy, pz end
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

-- Vertical zone from the ELEVATION ANGLE, not the raw height gap: a far target with a
-- modest height difference reads as "level", so "above"/"below" only fire when climbing
-- or diving actually matters. Deadband = the half-angle that still counts as level.
local function elev_zone(dz, dh)
    local ang = math.deg(math.atan(dz, dh))   -- +up / -down; atan(y,x) handles dh == 0
    if ang >= ELEV_DEADBAND_DEG then return "nav_up" end
    if ang <= -ELEV_DEADBAND_DEG then return "nav_down" end
    return "nav_level"
end

local function announce_tracking(label, dist)
    Speech.say(string.format(I18n.t("nav_tracking"), label, meters(dist)), false)
end

-- ---- the radar tick -----------------------------------------------------------------

local function drop_target()
    target, route, route_idx = nil, nil, 0
    arrived, target_missing = false, 0
    companion_idx = 0   -- a dropped manual target reverts to quest mode
    last_stealth_zone = nil
    was_on_target = false
    last_elev_zone = nil
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
    -- The trace API is AREA-dependent (works on some maps, aborts uncatchably on
    -- others): a "bad" fuse tripped in the old map must not keep obstacle avoidance
    -- dead in the new one — clear BOTH overload fuses so the new area re-probes
    -- (steer_around arms the testing fuse around every batch anyway, so a bad area
    -- re-trips in one tick).
    _G.__KakarotRayNative, _G.__KakarotRayChan = nil, nil
    world_gone = 0
    invoker_key, invoker_nav = nil, nil   -- pawn/nav system are new next level; re-register
    human_mesh = nil         -- a LEVEL actor: probing it after teardown is the abort
    avoid_steering, avoid_cued = false, false
    chain_wait, chain_seen = nil, {}   -- the sweep's actors lived in the dead level
    last_enemy_key = nil
    last_aim_key = nil
    enemy_cache, enemy_next = nil, 0
    gated_prev = true        -- audio is being stopped right here
    Audio.stop()
end)

-- ---- dynamic target chaining + enemy proximity alert --------------------------------

-- Retarget the nearest remaining item of `grp` (chained sweep), skipping every key
-- already visited this sweep. Speaks the new target via set_manual_target; announces
-- the sweep done (and ends it) when nothing is left. Game thread only; caller has
-- already passed the world gates this tick.
local function chain_to_next(grp)
    local cats = Nav.list_targets()
    for _, c in ipairs(cats) do
        if c.key == grp then
            for _, it in ipairs(c.items) do   -- nearest first
                if not chain_seen[it.key] then
                    Nav.set_manual_target(it.actor, it.key, Nav.item_label(it),
                        it.grp, it.stateful, true)   -- true: keep the sweep's seen-set
                    return
                end
            end
        end
    end
    chain_seen = {}
    Speech.say(I18n.t("radar_chain_done"), false)
end

-- While chain_wait is set: the player reached a hand-picked target. Advance the sweep
-- when the reached actor is gone (freed / hidden / InteractState Taken — collected) OR
-- when the player walks away from the spot: that "I'm done here" signal works for every
-- category, including ones with no collected state at all (gathering volumes, NPCs).
-- InteractState is read ONLY when the item was flagged stateful (an AAccessPointBase —
-- see add_target), never on plain actors.
local function chain_step(px, py, pz)
    local a = chain_wait.actor
    local advance = not Core.valid(a)
    if not advance then
        local hidden, st = false, nil
        pcall(function() hidden = a.bHidden end)
        if chain_wait.stateful then
            pcall(function() st = tonumber(a.InteractState) end)
        end
        advance = (hidden == true or hidden == 1) or st == STATE_TAKEN
    end
    if not advance then
        local x, y, z = actor_pos(a)
        advance = x ~= nil
            and math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2) > REARM_DIST
    end
    if advance then
        local grp = chain_wait.grp
        chain_wait = nil
        chain_to_next(grp)
    end
end

-- Field enemies = AAT_Character combat actors that are NOT the player/companions.
-- SpawnType (AT_enums: 1 ENCOUNT, 2 QUEST, 3 BOSS; reflected @0xC78) NAMES the kind, but
-- roaming field enemies report SpawnType 0 just like the player (verified 2026-07-08:
-- cpl059cXX enemies all spawnType 0), so it cannot be the FILTER. The robust signal from
-- the class dump: enemies derive from AT_Character but NOT AT_CharacterPlayableBase (the
-- player + companions cpl001/002/011 do); QuestCharacter NPCs and AT_MobBase townsfolk/
-- animals aren't AT_Characters at all, so they drop out. The minimap MapIconList proved
-- NOT to carry roaming field enemies, so they're scanned directly on the sparse cadence.
local ENEMY_NOUN_BY_SPAWN = { [1] = "cat_enemy", [2] = "cat_enemy_quest", [3] = "cat_enemy_boss" }
local playable_cls = nil   -- AT_CharacterPlayableBase (player + companions) — excluded
local enemy_display_name   -- forward decl: assigned below game_character_name (it needs it)

local function enemy_spawn_type(c)
    local st
    pcall(function() st = tonumber(c.SpawnType) end)
    return st
end

-- True for the player and companions (they inherit AT_CharacterPlayableBase); those are
-- the only AT_Characters we must NOT flag as enemies.
local function is_playable_char(c)
    if playable_cls == nil then
        local ok, k = pcall(function() return StaticFindObject("/Script/AT.AT_CharacterPlayableBase") end)
        playable_cls = (ok and k) or false
    end
    if not playable_cls then return false end
    local ok, r = pcall(function() return c:IsA(playable_cls) end)
    return ok and r == true
end

-- The live field-enemy list { {actor, noun}, ... } (cached; entries re-validated by
-- every user). Player/companions are excluded by the playable-base check.
local function enemies_list()
    if enemy_cache == nil or tick >= enemy_next then
        enemy_cache = {}
        enemy_next = tick + RESCAN_CLASSES
        -- Distance-cap the scan: a level loads its ENTIRE enemy roster parked/inactive at a
        -- shared default position — the Red Ribbon base dump (2026-07-09) showed 102 enemies,
        -- almost all at an identical ~2030 m. Keep only enemies actually near the player so the
        -- cache stays small and the alert/picker don't iterate the whole dormant roster.
        local pawn = player_pawn()
        local px, py, pz
        if pawn then px, py, pz = actor_pos(pawn) end
        for _, c in pairs(FindAllOf("AT_Character") or {}) do
            if Core.valid(c) and not is_playable_char(c) then
                local near = true
                if px then
                    local x, y, z = actor_pos(c)
                    near = x ~= nil
                        and ((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2) <= RADAR_NPC_CAP ^ 2
                end
                if near then
                    -- SpawnType 1/2/3 → specific noun; roaming enemies are SpawnType 0 → generic.
                    local noun = ENEMY_NOUN_BY_SPAWN[enemy_spawn_type(c) or 0] or "cat_enemy"
                    -- The game's own display name (CharacterName id -> GetCharacterName),
                    -- so the radar says "Soldier X" instead of the generic "enemy".
                    local name = enemy_display_name and enemy_display_name(c) or nil
                    enemy_cache[#enemy_cache + 1] = { actor = c, noun = noun, name = name }
                end
            end
        end
    end
    return enemy_cache
end

-- Animal species: wild animals are the AT_MobAnimalBase subtree of AT_MobBase (other
-- AT_MobBase pawns are static townsfolk — StandStaticMob_C — which must NOT be radar
-- targets). The species lives on the pawn's NpcComponent (declared on AAT_MobBase,
-- CXX dump): a UAnimalComponentBase whose reflected AnimalType is E_ANIMAL_TYPE
-- (AT_enums.hpp:10648 — 1 flying dragon … 9 small dinosaur; i18n key = value).
local animal_base_cls = nil   -- AT_MobAnimalBase (the animal subtree)
local animal_comp_cls = nil   -- AnimalComponentBase (declares AnimalType)

local function is_animal(a)
    if animal_base_cls == nil then
        local ok, c = pcall(function() return StaticFindObject("/Script/AT.AT_MobAnimalBase") end)
        animal_base_cls = (ok and c) or false
    end
    if not animal_base_cls then return false end
    local ok, r = pcall(function() return a:IsA(animal_base_cls) end)
    return ok and r == true
end

-- i18n species key for an animal pawn, or nil (not an animal / species unavailable —
-- the caller falls back to the generic "animal" noun for true animals).
local function animal_species(a)
    if not is_animal(a) then return nil end
    local key
    pcall(function()
        local comp = a.NpcComponent
        if not (comp and comp:IsValid()) then return end
        if animal_comp_cls == nil then
            local ok, c = pcall(function() return StaticFindObject("/Script/AT.AnimalComponentBase") end)
            animal_comp_cls = (ok and c) or false
        end
        if not animal_comp_cls or not comp:IsA(animal_comp_cls) then return end
        local t = tonumber(comp.AnimalType)
        if t and t >= 1 and t <= 9 then key = "animal_type_" .. t end
    end)
    return key
end

-- TEMP passive probe (2026-07-06, remove once the field-enemy class is confirmed):
-- combat starts too fast for a manual Ctrl+F5 near enemies, so this logs every Pawn
-- within 150 m — class name + SpawnType (AT_Characters only; reading it on other
-- pawn classes is the uncatchable abort) — to dumps/dump_enemies.txt, only when the
-- nearby set CHANGES and only on the sparse rescan tick. Unbuffered writes.
local ENEMY_PROBE = false   -- field-enemy class confirmed 2026-07-08 (AT_Character, not PlayableBase)
local at_char_cls = nil
local probe_next, probe_sig = 0, nil
local function enemy_probe(px, py, pz)
    if not ENEMY_PROBE or tick < probe_next then return end
    probe_next = tick + RESCAN_CLASSES
    if at_char_cls == nil then
        local ok, c = pcall(function() return StaticFindObject("/Script/AT.AT_Character") end)
        at_char_cls = (ok and c) or false
    end
    local lines = {}
    for _, p in pairs(FindAllOf("Pawn") or {}) do
        if Core.valid(p) then
            local x, y, z = actor_pos(p)
            if x then
                local d = math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
                if d <= 150 * M then
                    local cn, st = "?", "-"
                    pcall(function() cn = p:GetClass():GetFName():ToString() end)
                    if at_char_cls then
                        local ok, isat = pcall(function() return p:IsA(at_char_cls) end)
                        if ok and isat then
                            pcall(function() st = tostring(tonumber(p.SpawnType)) end)
                        end
                    end
                    -- Class ancestry (UE4SS-level reads, not game reflection — safe):
                    -- the wild field creatures are A0NN_BP_C pawns, NOT AT_Characters
                    -- (probe 2026-07-06), and the radar needs their common BASE class
                    -- to track them without fragile name patterns.
                    local chain = {}
                    pcall(function()
                        local c2 = p:GetClass()
                        for _ = 1, 5 do
                            c2 = c2:GetSuperStruct()
                            if not (c2 and c2:IsValid()) then break end
                            chain[#chain + 1] = c2:GetFName():ToString()
                        end
                    end)
                    lines[#lines + 1] = string.format("  %s d=%.0fm spawnType=%s super=%s",
                        cn, d / M, st, table.concat(chain, "<"))
                end
            end
        end
    end
    table.sort(lines)
    local sig = table.concat(lines, "|")
    if sig == probe_sig then return end
    probe_sig = sig
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_enemies.txt", "a")
        if not f then return end
        pcall(function() f:setvbuf("no") end)
        f:write(string.format("[%d] pawns within 150m: %d\n", os.time(), #lines))
        f:write(table.concat(lines, "\n") .. "\n")
        f:close()
    end)
end

-- Field AIM announcer ("am I aiming at the dinosaur?" — user request 2026-07-06, the
-- dino-hunt quest): the field targeting locks its victim into the player's
-- ULockonComponent (AAT_Character.LockonList @0x920; m_xActors TArray @0x100 — both
-- reflected, CXX dump). Speak whenever the LOCKED actor appears or changes:
-- "apuntando a <noun>, N metros" (noun: AT_MobBase tree = animal; AT_Character
-- SpawnType = the enemy noun; else generic character). Silent when the lock clears;
-- re-locking the same actor re-announces (the edge-gate resets on empty).
local function aim_watch(pawn, px, py, pz)
    local locked
    pcall(function()
        local comp = pawn.LockonList
        if comp and comp:IsValid() then
            local arr = comp.m_xActors
            if arr and #arr > 0 then
                local a = arr[1]
                if a and a:IsValid() then locked = a end
            end
        end
    end)
    if not locked then last_aim_key = nil return end
    local key = tostring(locked:GetAddress())
    if key == last_aim_key then return end
    last_aim_key = key
    local label
    if mob_base_cls == nil then
        local ok, c = pcall(function() return StaticFindObject("/Script/AT.AT_MobBase") end)
        mob_base_cls = (ok and c) or false
    end
    local ismob = false
    if mob_base_cls then
        local ok, v = pcall(function() return locked:IsA(mob_base_cls) end)
        ismob = ok and v == true
    end
    if ismob then
        -- species when the pawn is a true animal; other mobs are townsfolk
        label = I18n.t(animal_species(locked) or (is_animal(locked) and "cat_animal" or "cat_npc"))
    else
        if at_char_cls == nil then
            local ok, c = pcall(function() return StaticFindObject("/Script/AT.AT_Character") end)
            at_char_cls = (ok and c) or false
        end
        if at_char_cls then
            local ok, isat = pcall(function() return locked:IsA(at_char_cls) end)
            if ok and isat then
                -- the enemy's real display name when the game has one
                label = enemy_display_name and enemy_display_name(locked) or nil
                if not label then
                    label = I18n.t(ENEMY_NOUN_BY_SPAWN[enemy_spawn_type(locked) or 0] or "cat_npc")
                end
            end
        end
        label = label or I18n.t("cat_npc")
    end
    local x, y, z = actor_pos(locked)
    local d = x and math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
    Speech.say(string.format("%s %s%s", I18n.t("nav_aiming"), label,
        d and (", " .. string.format(I18n.t("nav_meters"), meters(d))) or ""), true)
end

-- Spoken proximity warning for the nearest field enemy: "<enemy noun>, <direction>,
-- <above/below>, N meters". Edge-gated per enemy actor (a NEW nearest enemy, or the
-- same one at half the distance) + a hard time throttle.
local function enemy_alert(px, py, pz)
    local best, best_d2 = nil, ENEMY_ALERT_DIST * ENEMY_ALERT_DIST
    for _, e in ipairs(enemies_list()) do
        if Core.valid(e.actor) then
            local x, y, z = actor_pos(e.actor)
            if x then
                local d2 = (x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2
                if d2 < best_d2 then
                    best = { actor = e.actor, x = x, y = y, z = z, noun = e.noun, name = e.name }
                    best_d2 = d2
                end
            end
        end
    end
    if not best then last_enemy_key = nil return end   -- re-arm once the area is clear
    local ms = now_ms()
    local key = tostring(best.actor:GetAddress())
    local d = math.sqrt(best_d2)
    if key == last_enemy_key and d > last_enemy_d * ENEMY_CLOSER_FACTOR then return end
    if ms - last_enemy_ms < ENEMY_CUE_MS then return end
    last_enemy_key, last_enemy_d, last_enemy_ms = key, d, ms
    local parts = { best.name or I18n.t(best.noun) }
    local dx, dy, dz = best.x - px, best.y - py, best.z - pz
    local dh = math.sqrt(dx * dx + dy * dy)
    local fx, fy = camera_forward()
    if fx and dh > 1 then
        local nx, ny = dx / dh, dy / dh
        local pan = nx * -fy + ny * fx
        local fb = nx * fx + ny * fy
        parts[#parts + 1] = I18n.t(math.abs(fb) >= math.abs(pan)
            and (fb >= 0 and "nav_ahead" or "nav_behind")
            or (pan >= 0 and "nav_right" or "nav_left"))
    end
    local ez = elev_zone(dz, dh)
    if ez ~= "nav_level" then
        parts[#parts + 1] = I18n.t(ez)
    end
    parts[#parts + 1] = string.format(I18n.t("nav_meters"), meters(d))
    Speech.say(table.concat(parts, ", "), false)
end


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

    -- Field aim announcer: per tick (one guarded property chain — cheap), so the
    -- "aiming at X" cue lands the instant the game locks a target.
    aim_watch(pawn, px, py, pz)

    -- Chained pickup sweep: waiting for the reached collectible to be taken, then
    -- retarget the next one (see chain_step). Enemy proximity warning on the same
    -- sparse cadence as the scans (a per-tick icon sweep would hammer reflection).
    if chain_wait then chain_step(px, py, pz) end
    if tick % SCAN_EVERY == 0 then
        enemy_alert(px, py, pz)
        enemy_probe(px, py, pz)
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

    -- Sweep continuation for a hand-picked chainable target that is collected or
    -- despawns MID-APPROACH (grabbed before the arrival radius fires, or the game
    -- removed it): mark it visited and go straight to the next one — this is the
    -- "pick it up -> the radar marks the next nearest by itself" flow.
    local function chain_over()
        chain_seen[target.key] = true
        local grp = target.grp
        drop_target()
        auto_suppressed = true
        chain_to_next(grp)
    end
    local sweeping = target.manual and chainable(target.grp)

    if not Core.valid(target.actor) then
        if sweeping then chain_over() else drop_target() end
        return
    end
    if sweeping then
        local hidden, st = false, nil
        pcall(function() hidden = target.actor.bHidden end)
        if target.stateful then
            pcall(function() st = tonumber(target.actor.InteractState) end)
        end
        if (hidden == true or hidden == 1) or st == STATE_TAKEN then
            chain_over()
            return
        end
    end
    local tx, ty, tz = actor_pos(target.actor)
    if not tx then
        if sweeping then chain_over() else drop_target() end
        return
    end

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
            -- Chainable categories instead arm the sweep: mark this spot visited and,
            -- once it's collected or the player walks off, retarget the next one
            -- (chain_step).
            if target.manual then
                if chainable(target.grp) then
                    chain_seen[target.key] = true
                    chain_wait = { actor = target.actor, key = target.key,
                                   grp = target.grp, stateful = target.stateful }
                end
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
    -- Every cue carries the CURRENT distance ("ahead, 120 meters" — user request
    -- 2026-07-06), and a distance-only cue fills long silences (straight approach,
    -- nothing changing) so the player can always tell whether they're closing in.
    local dist_txt = string.format(I18n.t("nav_meters"), meters(d3))
    local dir
    if math.abs(fb) >= math.abs(pan) then
        dir = fb >= 0 and "nav_ahead" or "nav_behind"
    else
        dir = pan >= 0 and "nav_right" or "nav_left"
    end
    if dir ~= last_dir_cue and ms - last_dir_ms >= DIR_CUE_MS then
        last_dir_cue, last_dir_ms = dir, ms
        last_dist_ms = ms
        Speech.say(string.format("%s, %s", I18n.t(dir), dist_txt), false)
    end
    -- Vertical: angle-based zone (up / level / down), spoken only when the zone CHANGES
    -- (like the horizontal word) — a steady climb announces once, not on a timer, and a
    -- correction back to level is confirmed.
    local thd = math.sqrt((tx - px) ^ 2 + (ty - py) ^ 2)
    local ez = elev_zone(tz - pz, thd)
    if ez ~= last_elev_zone and ms - last_elev_ms >= ELEV_CUE_MS then
        local prev = last_elev_zone
        last_elev_zone, last_elev_ms = ez, ms
        if ez ~= "nav_level" then
            last_dist_ms = ms
            Speech.say(string.format("%s, %s", I18n.t(ez), dist_txt), false)
        elseif prev then   -- returned to level from above/below (not the first read)
            last_dist_ms = ms
            Speech.say(I18n.t("nav_level"), false)
        end
    end
    if ms - last_dist_ms >= DIST_CUE_MS then
        last_dist_ms = ms
        Speech.say(dist_txt, false)
    end

    -- AIM-ALIGNMENT cue (user request 2026-07-06, the dino hunt: "how do I know I'm
    -- aiming at it?"): field Ki blasts go where the camera points, and the game's
    -- LockonComponent stays EMPTY during field aiming (tried first) — so this is
    -- OUR geometric reticle: while tracking an enemies-group target, the moment the
    -- camera yaw centers on it (|pan| within the reticle slack) inside Ki-blast
    -- range, say "on target" (interrupting — the shot window is short); re-arms
    -- when the aim drifts off. pan/fb here are already camera-relative.
    if target.grp == "enemies" then
        local on_target = fb > 0 and math.abs(pan) <= AIM_ALIGN_PAN and d3 <= AIM_RANGE
        if on_target and not was_on_target then
            Speech.say(I18n.t("nav_on_target"), true)
        end
        was_on_target = on_target
    end

    -- Stealth aid for hunted animals / field enemies (user request 2026-07-06, the
    -- deer hunt: "approach from behind so they don't see you"): while tracking a
    -- target picked from the enemies category, speak which side of ITS facing the
    -- player is on whenever that zone changes — "behind" = safe approach, "in front"
    -- = it can see you. K2_GetActorRotation is the reflected sibling of the
    -- K2_GetActorLocation call the whole tracker already relies on.
    if target.grp == "enemies" then
        local rot
        pcall(function() rot = target.actor:K2_GetActorRotation() end)
        if rot then
            local yaw = math.rad(rot.Yaw or 0)
            local fwx, fwy = math.cos(yaw), math.sin(yaw)   -- the animal's forward
            local vx, vy = px - tx, py - ty                 -- animal -> player
            local vh = math.sqrt(vx * vx + vy * vy)
            if vh > 1 then
                local dot = (vx * fwx + vy * fwy) / vh
                local zone = dot > STEALTH_CONE and "nav_stealth_front"
                    or dot < -STEALTH_CONE and "nav_stealth_back"
                    or "nav_stealth_side"
                if zone ~= last_stealth_zone and ms - last_stealth_ms >= STEALTH_CUE_MS then
                    last_stealth_zone, last_stealth_ms = zone, ms
                    Speech.say(I18n.t(zone), false)
                end
            end
        end
    end
end

-- ---- explore mode: passive "look-to-discover" radar (no menu) -----------------------
-- Toggled by double-tapping R3 (radar_menu). While ON and free-roaming, it sonifies
-- nearby points of interest WITHOUT picking anything from a menu: the POI your CAMERA
-- faces is named by the screen reader and pinged with its category cue, and the nearest
-- POI overall is marked by a softer ping so you sense what's off to the side / behind.
-- Cost is bounded: the heavy Nav.list_targets scan runs ONLY after you've travelled far
-- enough (POIs don't move if you don't), never per tick; each tick just re-evaluates the
-- cached positions against the live camera (pure math), the same primitive the beacon uses.
local EXPLORE_CONE_DEG = 35            -- half-angle (deg) of the camera "focus" cone
local EXPLORE_RESCAN_DIST = 25 * M     -- re-run the POI scan after moving this far
local EXPLORE_RESCAN_MS = 4000         -- ...but never more often than this (boost floor)
local EXPLORE_MAX_DIST = 200 * M       -- ignore POIs farther than this while exploring
local EXPLORE_FOCUS_PING_MS = 900      -- min gap between focused-POI pings
local EXPLORE_NEAR_PING_MS = 1500      -- min gap between nearest-overall pings
local EXPLORE_CUE = { enemies = "enemy" }   -- grp -> named audio cue (default "item")

local explore_on = false
local explore_pois = {}
local explore_sx, explore_sy, explore_sz = nil, nil, nil   -- player pos at last scan
local explore_scan_ms = -EXPLORE_RESCAN_MS
local explore_focus_key = nil
local explore_focus_ms, explore_near_ms = 0, 0

local function explore_cue(grp) return EXPLORE_CUE[grp] or "item" end

-- Camera-relative pan (-1..1) and forward component fb (cos of the angle) for a POI.
local function explore_cam_rel(p, px, py, fx, fy)
    local dx, dy = p.x - px, p.y - py
    local dh = math.sqrt(dx * dx + dy * dy)
    if dh < 1 or not fx then return 0, 1 end
    local nx, ny = dx / dh, dy / dh
    local pan = nx * -fy + ny * fx
    local fb = nx * fx + ny * fy
    if pan > 1 then pan = 1 elseif pan < -1 then pan = -1 end
    return pan, fb
end

local function explore_pitch(fb)
    if fb >= 0 then return 1.0 end
    local p = 1.0 + fb * BEHIND_PITCH_SLOPE
    return p < PITCH_MIN and PITCH_MIN or p
end

local function explore_vol(d2, scale)
    local t = math.sqrt(d2) / EXPLORE_MAX_DIST
    if t > 1 then t = 1 end
    return (0.9 - 0.5 * t) * scale
end

-- Heavy POI scan (Nav.list_targets), flattened with each POI's current position cached.
-- Displacement-gated by the caller so this is infrequent, never per tick.
local function explore_rescan(px, py, pz, ms)
    local pois = {}
    for _, cat in ipairs(Nav.list_targets()) do
        for _, it in ipairs(cat.items or {}) do
            local x, y, z = actor_pos(it.actor)
            if x then
                pois[#pois + 1] = { key = it.key, grp = it.grp,
                                    label = Nav.item_label(it), x = x, y = y, z = z }
            end
        end
    end
    explore_pois = pois
    explore_sx, explore_sy, explore_sz = px, py, pz
    explore_scan_ms = ms
end

local function explore_tick()
    if not explore_on or not Nav.field_ready() then return end
    local pawn = player_pawn()
    if not pawn then return end
    local px, py, pz = actor_pos(pawn)
    if not px then return end
    local ms = now_ms()

    -- Rescan only after travelling far enough (time floor so boost can't spam it).
    local moved2 = explore_sx
        and ((px - explore_sx) ^ 2 + (py - explore_sy) ^ 2 + (pz - explore_sz) ^ 2)
        or math.huge
    if explore_sx == nil or (moved2 > EXPLORE_RESCAN_DIST ^ 2 and ms - explore_scan_ms >= EXPLORE_RESCAN_MS) then
        explore_rescan(px, py, pz, ms)
    end

    local fx, fy = camera_forward()
    local cone = math.cos(math.rad(EXPLORE_CONE_DEG))
    local maxd2 = EXPLORE_MAX_DIST ^ 2
    local near, near_d2 = nil, math.huge
    local focus, focus_d2 = nil, math.huge
    for _, p in ipairs(explore_pois) do
        local dx, dy, dz = p.x - px, p.y - py, p.z - pz
        local d2 = dx * dx + dy * dy + dz * dz
        if d2 <= maxd2 then
            if d2 < near_d2 then near, near_d2 = p, d2 end
            if fx then
                local dh = math.sqrt(dx * dx + dy * dy)
                if dh > 1 and ((dx / dh) * fx + (dy / dh) * fy) >= cone and d2 < focus_d2 then
                    focus, focus_d2 = p, d2
                end
            end
        end
    end

    -- Focused POI (camera cone): name it when it CHANGES, and ping its category cue.
    if focus then
        local pan, fb = explore_cam_rel(focus, px, py, fx, fy)
        if focus.key ~= explore_focus_key then
            explore_focus_key = focus.key
            local dir = (math.abs(fb) >= math.abs(pan))
                and (fb >= 0 and "nav_ahead" or "nav_behind")
                or (pan >= 0 and "nav_right" or "nav_left")
            local parts = { focus.label, I18n.t(dir) }
            local ez = elev_zone(focus.z - pz, math.sqrt((focus.x - px) ^ 2 + (focus.y - py) ^ 2))
            if ez ~= "nav_level" then parts[#parts + 1] = I18n.t(ez) end
            parts[#parts + 1] = string.format(I18n.t("nav_meters"), meters(math.sqrt(focus_d2)))
            Speech.say(table.concat(parts, ", "), false)
        end
        if ms - explore_focus_ms >= EXPLORE_FOCUS_PING_MS then
            explore_focus_ms = ms
            Audio.cue(explore_cue(focus.grp), pan, explore_vol(focus_d2, 1.0), explore_pitch(fb))
        end
    else
        explore_focus_key = nil
    end

    -- Nearest POI overall (when not the focused one): a softer ping so you feel things
    -- to the side / behind without facing them.
    if near and (not focus or near.key ~= focus.key)
       and ms - explore_near_ms >= EXPLORE_NEAR_PING_MS then
        explore_near_ms = ms
        local pan, fb = explore_cam_rel(near, px, py, fx, fy)
        Audio.cue(explore_cue(near.grp), pan, explore_vol(near_d2, 0.5), explore_pitch(fb))
    end
end

-- Double-tap R3 (radar_menu): toggle the passive explore radar. Independent of the F3
-- beacon master switch — you can explore with or without a tracked objective.
function Nav.toggle_explore()
    explore_on = not explore_on
    explore_focus_key = nil
    if explore_on then
        explore_sx = nil   -- force a fresh scan on the next tick
        Speech.say(I18n.t("explore_on"), true)
    else
        Speech.say(I18n.t("explore_off"), true)
    end
    return explore_on
end

function Nav.explore_on() return explore_on end

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
                -- Passive explore radar runs in its OWN pcall so an error here can never
                -- take down the beacon tick (or vice versa).
                local ok2, err2 = pcall(explore_tick)
                if not ok2 then print("[KakarotAccess] nav explore error: " .. tostring(err2) .. "\n") end
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
        chain_wait, chain_seen = nil, {}
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
        -- Exclude field ENEMIES (SpawnType != 0): Shift+F5 must cycle party members,
        -- not the mob you're about to fight (they're AT_Character too).
        if Core.valid(c) and tostring(c:GetAddress()) ~= me_key
            and not ENEMY_NOUN_BY_SPAWN[enemy_spawn_type(c) or 0] then
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
    else
        -- Manual escape hatch (Shift+F3 off->on): also re-arm the raycast fuses, so
        -- obstacle avoidance can be re-probed on the spot without a map change.
        _G.__KakarotRayNative, _G.__KakarotRayChan = nil, nil
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
        parts[#parts + 1] = I18n.t(elev_zone(dz, dh))
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
-- ONLY ids verified in play — the old guessed table (Cpl003..Cpl008 assumed to follow
-- the character-select order) announced "Trunks/Goten/Gotenks" for characters who
-- can't even appear yet (user report 2026-07-06). The game's own resolver
-- (game_character_name below) is consulted FIRST; this map is the offline fallback.
-- Verified: Cpl001/Cpl002 = the intro/wasteland player pawns; Cpl009 = the quest
-- character at Gohan's bonfire (Yajirobe); Cpl011 = the playable pawn of the
-- "Gohan's guardian" chapter and the C011 half of the Gohan/Piccolo demo pair.
local CPL_NAMES = {
    Cpl001 = "Goku", Cpl002 = "Gohan", Cpl009 = "Yajirobe", Cpl011 = "Piccolo",
}
local NPC_NAMES = {
    chichi = "Chi-Chi", gohan = "Gohan", goku = "Goku", krillin = "Krillin",
    kuririn = "Krillin", piccolo = "Piccolo", bulma = "Bulma", roshi = "Master Roshi",
    kame = "Master Roshi", ox = "Ox-King", gyumao = "Ox-King", yamcha = "Yamcha",
    tenshinhan = "Tien", tien = "Tien", chaozu = "Chiaotzu", vegeta = "Vegeta",
    trunks = "Trunks", korin = "Korin", karin = "Korin", yajirobe = "Yajirobe",
    dende = "Dende", mrpopo = "Mr. Popo", popo = "Mr. Popo", kamisama = "Kami",
}

-- The game's OWN id -> display-name resolver: UAT_BlueprintFunctionLibrary::
-- GetCharacterName(FString) -> FString (CXX dump, AT.hpp — a static blueprint library,
-- called on its CDO). This is the authoritative source for what a sighted player sees;
-- returns "" for ids it doesn't know. Cached per id (it's a reflected call into a
-- game-side table lookup).
local name_lib, name_cache = nil, {}
local function game_character_name(id)
    local hit = name_cache[id]
    if hit ~= nil then return hit or nil end   -- false = known miss
    if name_lib == nil then
        local ok, lib = pcall(function()
            return StaticFindObject("/Script/AT.Default__AT_BlueprintFunctionLibrary")
        end)
        name_lib = (ok and lib and lib:IsValid()) and lib or false
    end
    local nm
    if name_lib then
        pcall(function()
            local s = name_lib:GetCharacterName(id)
            if type(s) == "string" then nm = s elseif s then nm = s:ToString() end
        end)
    end
    if nm == "" then nm = nil end
    name_cache[id] = nm or false
    return nm
end

-- Real display name for a field ENEMY: AAT_Character declares CharacterName (reflected
-- FString @0x9E8, CXX dump) — the character id (e.g. "Cpl059c02") — which the game's own
-- GetCharacterName resolver turns into what a sighted player sees. Variation suffixes
-- ("...c02") are retried without the suffix when the full id has no entry. nil = no name
-- (callers fall back to the generic enemy noun). ONLY call on AT_Character actors:
-- CharacterName is not declared elsewhere and reading it would be the uncatchable abort.
enemy_display_name = function(c)
    local raw
    pcall(function()
        local s = c.CharacterName
        if type(s) == "string" then raw = s elseif s then raw = s:ToString() end
    end)
    if type(raw) ~= "string" or raw == "" or raw == "None" then return nil end
    local live = game_character_name(raw)
    if live then return live end
    local base = raw:gsub("[cC]%d+$", "")   -- "Cpl059c02" -> "Cpl059"
    if base ~= raw then
        live = game_character_name(base)
        if live then return live end
        if CPL_NAMES[base] then return CPL_NAMES[base] end
    end
    return CPL_NAMES[raw]
end

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
    -- The game's own resolver first (authoritative); hand-verified map second.
    local live = game_character_name(raw)
    if live then return live end
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
    local dropped = {}   -- per-group candidates cut by the distance cap (rescue below)
    local diag = RADAR_DEBUG and {} or nil
    -- Core add: place an actor into a group with a spoken noun. range = the icon's own
    -- reveal radius (nil for non-icon sources like NPCs). Distance-limited for non-quest
    -- groups by max(global cap, range) — never tighter than the cap, so an already-shown
    -- icon farther than its small reveal radius isn't wrongly dropped. stateful = the
    -- actor carries AAccessPointBase.InteractState (safe to read Taken on it later —
    -- the arrival-chaining check needs to know).
    local function add_target(actor, grp, noun, range, src, name, stateful)
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
            local cap = (src == "questchar" or src == "collectible" or src == "enemy")
                and RADAR_NPC_CAP or math.max(RADAR_MAP_CAP, range or 0)
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
        local item = { actor = actor, key = key, dist = d, noun = noun, name = name,
                       grp = grp, stateful = stateful or nil }
        if not kept then
            -- Over the cap: remember it for the empty-group rescue — but ONLY for the
            -- game-curated MAP-ICON sources. Direct actor scans (NPCs, collectibles,
            -- enemies) use the tight cap precisely because beyond it lies the game's
            -- parked preload pool (Trunks km away resurfaced through the rescue,
            -- user 2026-07-06) — never rescue those.
            if src ~= "questchar" and src ~= "collectible" and src ~= "enemy" then
                dropped[grp] = dropped[grp] or {}
                dropped[grp][#dropped[grp] + 1] = item
            end
            return
        end
        seen[key] = true
        groups[grp] = groups[grp] or { items = {} }
        groups[grp].items[#groups[grp].items + 1] = item
    end
    -- Add by EMapIcon type (icons): derive group + noun from the type. A named
    -- mission marker (see is_mission_marker) is a QUEST destination whatever its
    -- icon type says — no distance cap, listed under Quests.
    local function add_icon(actor, t, src)
        if not t then return end
        if is_mission_marker(actor) then
            add_target(actor, "quests", "nav_other", nil, src)
            return
        end
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
    -- 2b) EVERY area POI with a map-icon component, found DIRECTLY (no map open needed). The
    -- minimap MapIconList above only carries NEARBY icons; scanning ATMapIconComponent instances
    -- reaches ALL loaded POIs at any distance — shops, fishing, fruit/ore gathering, training,
    -- sites — the same set the area map shows. Owner = the POI actor; MapIconType is set even
    -- when bShowMapIcon is false (the game hides FAR icons). add_target dedupes by address.
    for _, comp in pairs(FindAllOf("ATMapIconComponent") or {}) do
        if Core.valid(comp) then
            local owner, t
            pcall(function() owner = comp:GetOwner() end)
            pcall(function() t = tonumber(comp.MapIconType) end)
            if Core.valid(owner) and t then add_icon(owner, t, "mapicon2") end
        end
    end
    -- 2c) ENEMY BASES carry NO ATMapIconComponent (so 2b misses them) but a UATEnemiesBaseBehaviour
    -- component. Find those and classify their owner as ENEMIES_BASE (EMapIcon 32). Scan the base
    -- AND the known subclass (a native-base FindAllOf can return nothing when a subclass exists).
    for _, cls in ipairs({ "ATEnemiesBaseBehaviour", "ATExterminationBastionComponent" }) do
        for _, comp in pairs(FindAllOf(cls) or {}) do
            if Core.valid(comp) then
                local owner
                pcall(function() owner = comp:GetOwner() end)
                if Core.valid(owner) then add_icon(owner, 32, "enemybase") end
            end
        end
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
        -- Scan the native base AND the blueprint subclass: FindAllOf on a native base
        -- can find NOTHING on this game (community-board lesson) — Yajirobe by the
        -- bonfire was a QuestCharacterBase_C the "QuestCharacter" scan never returned
        -- (dump 2026-07-06). add_target dedupes by address when both scans hit.
        for _, cls in ipairs({ "QuestCharacter", "QuestCharacterBase_C" }) do
            for _, npc in pairs(FindAllOf(cls) or {}) do
                if Core.valid(npc) and npc_present(npc) then
                    add_target(npc, "npc", "cat_npc", nil, "questchar", npc_name(npc))
                end
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
        -- Collected filter for AAccessPointBase-derived actors (see STATE_TAKEN above).
        -- InteractState is declared on AAccessPointBase ONLY — FieldActionPointActor is
        -- a plain AActor, and reading a non-existent property is the uncatchable abort,
        -- so the caller must only pass access-point classes here.
        local function not_taken(a)
            local st
            pcall(function() st = tonumber(a.InteractState) end)
            return st ~= STATE_TAKEN
        end
        -- Spoken name for an action spot: the game's own ActionName (reflected FString
        -- on AFieldActionPointActor @0x340). nil when empty -> generic noun.
        local function action_name(a)
            local s
            pcall(function()
                local v = a.ActionName
                if type(v) == "string" then s = v
                elseif v then s = v:ToString() end
            end)
            return (type(s) == "string" and s ~= "") and s or nil
        end
        -- Spoken name for a placed item: its drop-table id (APlacementObjectInfo.
        -- ItemTableComponent.FieldItemDropData, FFieldItemData FixedId/NormalId — all
        -- reflected, CXX dump). The id is game data, not localized text, so it is only
        -- spoken when it reads as words (has a 3+ letter run); cryptic numeric codes
        -- fall back to the generic "item" noun.
        local function drop_item_name(a)
            local raw
            pcall(function()
                local comp = a.ItemTableComponent
                if not (comp and comp:IsValid()) then return end
                local d = comp.FieldItemDropData
                if not d then return end
                for _, fld in ipairs({ "FixedId", "NormalId" }) do
                    local id = d[fld]
                    local s = id and id:ToString()
                    if s and s ~= "" and s ~= "None" then raw = s return end
                end
            end)
            if not raw then return nil end
            local cleaned = raw:gsub("_", " ")
            return cleaned:match("%a%a%a") and cleaned or nil
        end
        -- Per-class capabilities (which reflected properties EXIST — reading a property
        -- a class doesn't declare is the uncatchable abort, so never probe blindly):
        --   FieldActionPointActor (plain AActor + ActionName; Field Memories derive
        --     from it) -> spoken name from ActionName, NO InteractState;
        --   PlacementObjectInfo (AAccessPointBase + ItemTableComponent) -> item-id
        --     name + Taken filter;
        --   AccessPointItemBase (AAccessPointBase only) -> Taken filter, generic noun.
        for _, c in ipairs({
            { cls = "FieldActionPointActor", action = true },
            { cls = "PlacementObjectInfo", item = true, state = true },
            { cls = "AccessPointItemBase", state = true },
        }) do
            for _, a in pairs(FindAllOf(c.cls) or {}) do
                if Core.valid(a) and visible_actor(a)
                    and (not c.state or not_taken(a)) then
                    local cn = "?"
                    pcall(function() cn = a:GetClass():GetFName():ToString() end)
                    local is_memory = cn:find("Memories", 1, true) ~= nil
                    local grp = "collectibles"
                    local noun = is_memory and "cat_memory" or "cat_item"
                    if c.action and not is_memory then
                        -- Non-memory action points are interactable SPOTS you use
                        -- (train/meditate/examine — e.g. Piccolo's waterfall), not
                        -- pickups: list them under Sites, labeled by their ActionName.
                        grp, noun = "sites", "radar_cat_sites"
                    end
                    local name = (c.action and action_name(a))
                        or (c.item and drop_item_name(a)) or nil
                    add_target(a, grp, noun, nil, "collectible", name, c.state)
                end
            end
        end
    end
    -- 4b) wind tunnels (AATWindRoad — the flight-boost spline routes; the actor origin
    -- is the entrance). Direct scan: they carry a FieldPointComponent (WindPath), not a
    -- minimap icon. Requested 2026-07-06.
    for _, a in pairs(FindAllOf("ATWindRoad") or {}) do
        if Core.valid(a) then
            add_target(a, "sites", "cat_windroad", nil, "collectible")
        end
    end
    -- 4c) FIELD POINTS (UFieldPointComponent.FieldPointIconType — FIELD_POINT_TYPE,
    -- AT_enums.hpp): the glowing interaction pillars that are NOT minimap icons —
    -- fish-catching spots (Fish=1), wind-path entrances (WindPath=2, dedup'd with 4b
    -- via seen), dino spots (Dino=3), the floating-island jump (9). Requested
    -- 2026-07-06 ("cosas para atrapar peces"). Component scan -> owning actor.
    do
        local FIELD_POINT = {
            [1] = { grp = "fishing", noun = "cat_fishing" },
            [2] = { grp = "sites", noun = "cat_windroad" },
            [3] = { grp = "gathering", noun = "cat_hunt" },
            [9] = { grp = "sites", noun = "radar_cat_sites" },
        }
        for _, comp in pairs(FindAllOf("FieldPointComponent") or {}) do
            if comp and comp:IsValid() then
                local t
                pcall(function() t = tonumber(comp.FieldPointIconType) end)
                local m = t and FIELD_POINT[t]
                if m then
                    local owner
                    pcall(function() owner = comp:GetOwner() end)
                    if Core.valid(owner) then
                        add_target(owner, m.grp, m.noun, nil, "collectible")
                    end
                end
            end
        end
    end
    -- 4d) AREA EXITS: the transfer points to other maps. TWO actor classes (AT.hpp):
    -- APortal (TeleportToArea) and ALevelNavigator (NavigateLevelName + the travel
    -- confirm window) — Area02's exits are LevelNavigators (Portal scan came up empty,
    -- user 2026-07-06). Story beats often continue on another area ("Aldea Lucca added
    -- to the world map"), so these are quest-critical: wide-cap source (they're few
    -- and far) + eligible for the empty-group rescue.
    for _, cls in ipairs({ "Portal", "LevelNavigator" }) do
        for _, a in pairs(FindAllOf(cls) or {}) do
            if Core.valid(a) then
                add_target(a, "sites", "cat_portal", nil, "portal")
            end
        end
    end

    -- 5) field enemies: the direct SpawnType scan (enemies_list) — the minimap icon
    -- list does not carry the roaming enemies. Tight distance cap like NPCs.
    for _, e in ipairs(enemies_list()) do
        add_target(e.actor, "enemies", e.noun, nil, "enemy", e.name)
    end
    -- 5b) wild field animals (deer, wolves…): the AT_MobAnimalBase subtree of
    -- AT_MobBase — a SEPARATE class tree from AT_Character (lineage probe 2026-07-06:
    -- A009b_BP_C < A009_BP_C < AnimalMob_Pawn_C < AT_MobAnimalBase < AT_MobBase <
    -- NpcPawn), which is why the SpawnType scan never saw them. Needed for the hunt
    -- quests ("catch N deer"). is_animal FILTERS the scan: non-animal AT_MobBase pawns
    -- are static townsfolk (StandStaticMob_C, dump_enemies 2026-07-08) and must not be
    -- radar targets. Spoken as their species (AnimalType) when available. Native-base
    -- FindAllOf can come up empty on this game (community-board lesson) → also scan the
    -- blueprint animal base; add_target dedupes by address.
    -- SAFETY: position/bHidden/NpcComponent only — SpawnType/InteractState are NOT
    -- declared on this tree and reading them is the uncatchable abort. Picker only,
    -- NOT the proximity alert (passive deer everywhere would make it spam).
    for _, cls in ipairs({ "AT_MobBase", "AnimalMob_Pawn_C" }) do
        for _, a in pairs(FindAllOf(cls) or {}) do
            if Core.valid(a) and is_animal(a) then
                local hidden = false
                pcall(function() hidden = a.bHidden end)
                if not (hidden == true or hidden == 1) then
                    add_target(a, "enemies", animal_species(a) or "cat_animal", nil, "enemy")
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

    -- EMPTY-GROUP RESCUE: on the big open maps EVERY POI of a category can sit beyond
    -- the distance cap (Area02, 2026-07-06: nearest training point / shop / Kame House
    -- marker all > 1 km) and the whole picker read "nothing to track" — worse than a
    -- far target. A group that kept nothing lists its nearest capped-out candidates.
    local RESCUE_N = 3
    for gk, list in pairs(dropped) do
        if not groups[gk] or #groups[gk].items == 0 then
            table.sort(list, function(a, b) return a.dist < b.dist end)
            groups[gk] = groups[gk] or { items = {} }
            local added = {}
            for _, it in ipairs(list) do
                if #groups[gk].items >= RESCUE_N then break end
                if not seen[it.key] and not added[it.key] then
                    added[it.key] = true
                    groups[gk].items[#groups[gk].items + 1] = it
                end
            end
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
    local had = target ~= nil or chain_wait ~= nil
    drop_target()
    chain_wait, chain_seen = nil, {}   -- also ends a pending sweep
    auto_suppressed = true   -- stay quiet until re-pick / F3 (don't auto-grab a quest)
    Audio.stop()
    if had then Speech.say(I18n.t("nav_stopped"), true) end
end

-- Commit a picker choice: track this actor as a MANUAL target (auto-scan won't steal
-- it) and make sure the radar is on. Game-thread only (radar_menu calls it there).
-- grp/stateful come from the picker item (Nav.list_targets) and drive the arrival
-- chaining (chainable + chain_step); both may be nil (companion tracking).
-- keep_sweep = internal (chain_to_next): keep the sweep's visited set; a player pick
-- starts a FRESH sweep instead.
function Nav.set_manual_target(actor, key, label, grp, stateful, keep_sweep)
    if not Core.valid(actor) then return false end
    on = true
    auto_suppressed = false   -- an explicit pick resumes normal tracking
    gated_prev = false
    companion_idx = 0
    chain_wait = nil          -- an explicit pick replaces any pending sweep wait
    if not keep_sweep then chain_seen = {} end
    target = { actor = actor, key = key or tostring(actor:GetAddress()),
               pri = PRI_OTHER, label = label, manual = true,
               grp = grp, stateful = stateful }
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
    -- Version beacon BEFORE any engine work: hearing it proves the reload applied and
    -- the keybind fired; the file then shows how far the dump got (unbuffered writes).
    Speech.say("dump v2", true)
    ExecuteInGameThread(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_nav_targets.txt", "w")
        if not f then Speech.say("nav dump: cannot open file", true) return end
        -- UNBUFFERED: an uncatchable abort mid-dump otherwise loses EVERYTHING written
        -- so far (a 0-byte file, seen 2026-07-06 in the Raditz-road area) — with no
        -- buffer the file shows exactly how far the dump got before dying.
        pcall(function() f:setvbuf("no") end)
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
        -- Ring-pause diagnostic (mute-pause reports): every Start_Top_C instance with
        -- its own visibility enum + effective on_screen, so a wrong-instance latch vs a
        -- shadowing adapter (adapter_index above) can be told apart from this dump
        -- alone. Pooled GameInstance widgets are safe to probe behind menus; NOT during
        -- a transition (hence the gate).
        if not trans then
            pcall(function()
                for i, o in pairs(FindAllOf("Start_Top_C") or {}) do
                    if Core.valid(o) then
                        local e = "?"
                        pcall(function() e = tostring(tonumber(o:GetVisibility())) end)
                        f:write(string.format("  Start_Top_C[%s] enum=%s on_screen=%s %s\n",
                            tostring(i), e, tostring(Core.on_screen(o)), o:GetFullName()))
                    end
                end
            end)
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
        -- Sibling probe for each ACTIVE navi target: every actor of the target's class,
        -- sorted by NAME, with position + distance. Story events often mark the intended
        -- walking route with a numbered trigger CHAIN (C01_03002_AutoMoveTrigger_001..N):
        -- listing the whole chain maps the walkable route offline when NavMesh and
        -- raycast can't guide (walk-only Gohan beat stuck 42 m short, 2026-07-06).
        do
            local done = {}
            for _, icon in pairs(icons) do
                if Core.valid(icon) and icon_in_use(icon) then
                    local ok, ta = pcall(function() return icon.TargetActor end)
                    if ok and Core.valid(ta) then
                        local cls
                        pcall(function() cls = ta:GetClass():GetFName():ToString() end)
                        if cls and not done[cls] then
                            done[cls] = true
                            local sib = {}
                            for _, a in pairs(FindAllOf(cls) or {}) do
                                if Core.valid(a) then
                                    local x, y, z = actor_pos(a)
                                    if x then
                                        local nm = "?"
                                        pcall(function() nm = a:GetFName():ToString() end)
                                        sib[#sib + 1] = { nm = nm, x = x, y = y, z = z,
                                            d = math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2) }
                                    end
                                end
                            end
                            table.sort(sib, function(a, b) return a.nm < b.nm end)
                            f:write(string.format("navi target siblings (%s): %d\n", cls, #sib))
                            for _, s in ipairs(sib) do
                                f:write(string.format("  sibling %s d=%.0fm pos=%.0f %.0f %.0f\n",
                                    s.nm, s.d / M, s.x, s.y, s.z))
                            end
                        end
                    end
                end
            end
        end
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
            -- SAFE coverage reads FIRST — the FindPath probe below can abort the dump
            -- mid-file (it did on 2026-07-06 when the projection used the K2_ overload),
            -- and losing the coverage map with it left the diagnosis blind.
            for _, cls in ipairs({ "RecastNavMesh", "NavMeshBoundsVolume" }) do
                local cnt = 0
                pcall(function()
                    for _, a in pairs(FindAllOf(cls) or {}) do
                        if Core.valid(a) then cnt = cnt + 1 end
                    end
                end)
                f:write(string.format("navdata %s: %d\n", cls, cnt))
            end
            pcall(function()
                for _, a in pairs(FindAllOf("RecastNavMesh") or {}) do
                    if Core.valid(a) then
                        local x, y, z = actor_pos(a)
                        local nm = "?"
                        pcall(function() nm = a:GetFName():ToString() end)
                        f:write(string.format("  recast %s d=%s pos=%s %s %s\n", nm,
                            x and string.format("%.0fm", math.sqrt((x - px) ^ 2
                                + (y - py) ^ 2 + (z - pz) ^ 2) / M) or "?",
                            tostring(x), tostring(y), tostring(z)))
                    end
                end
            end)
            -- NavMeshBoundsVolume coverage map: distance + position + scale of every
            -- volume. If they cluster around towns/NPC areas instead of covering the
            -- map, the baked meshes are partial — the route feature then works inside
            -- those zones and falls back to the straight beacon elsewhere.
            pcall(function()
                for _, a in pairs(FindAllOf("NavMeshBoundsVolume") or {}) do
                    if Core.valid(a) then
                        local x, y, z = actor_pos(a)
                        local sc = "?"
                        pcall(function()
                            local s = a:GetActorScale3D()
                            sc = string.format("%.0f %.0f %.0f", s.X, s.Y, s.Z)
                        end)
                        f:write(string.format("  navvol d=%s pos=%s %s %s scale=%s\n",
                            x and string.format("%.0fm", math.sqrt((x - px) ^ 2
                                + (y - py) ^ 2 + (z - pz) ^ 2) / M) or "?",
                            tostring(x), tostring(y), tostring(z), sc))
                    end
                end
            end)
            local pp = ns and project_to_nav(ns, pawn, px, py, pz)
            f:write("project player -> " .. (pp and string.format("%.0f %.0f %.0f",
                pp.x, pp.y, pp.z) or "nil") .. "\n")
            local pts = compute_route(pawn, px, py, pz, px + 10 * M, py, pz)
            f:write("navmesh probe (10m ahead): " .. (pts and (#pts .. " points") or "no path") .. "\n")
            -- VERBOSE FindPath probe: projection resolves on this map yet FindPath
            -- yields nothing even with the pawn PathfindingContext (2026-07-06) — log
            -- every stage (unbuffered) so the exact failure is visible: nil path
            -- object vs empty PathPoints vs a Lua-level call error, plus the path's
            -- own GetDebugString/IsPartial verdict when one comes back.
            if ns and not nav_native_bad() then
                local okp, perr = pcall(function()
                    local s = project_to_nav(ns, pawn, px, py, pz) or { x = px, y = py, z = pz }
                    local e = project_to_nav(ns, pawn, px + 10 * M, py, pz)
                    f:write("  findpath proj_end=" .. (e and "ok" or "NIL(using raw)") .. "\n")
                    e = e or { x = px + 10 * M, y = py, z = pz }
                    local path = ns:FindPathToLocationSynchronously(pawn,
                        { X = s.x, Y = s.y, Z = s.z }, { X = e.x, Y = e.y, Z = e.z },
                        pawn, nil)
                    if not path then f:write("  findpath -> NIL path object\n") return end
                    f:write("  findpath obj=" .. path:GetFullName() .. "\n")
                    local n2 = "?"
                    pcall(function() n2 = tostring(#path.PathPoints) end)
                    f:write("  findpath PathPoints=" .. n2 .. "\n")
                    pcall(function()
                        local ds = path:GetDebugString()
                        f:write("  findpath partial=" .. tostring(path:IsPartial())
                            .. " debug=" .. tostring(ds and ds:ToString()) .. "\n")
                    end)
                    -- Per-mesh query: PathfindingContext accepts a NavigationData
                    -- ACTOR, pinning the query to that exact mesh — this tells which
                    -- of the agent meshes (Human/Dinosaur/Animal) can actually answer
                    -- a walk query here.
                    for _, a in pairs(FindAllOf("RecastNavMesh") or {}) do
                        if Core.valid(a) then
                            local nm = "?"
                            pcall(function() nm = a:GetFName():ToString() end)
                            local s2 = project_to_nav(ns, pawn, px, py, pz, a) or s
                            local e2 = project_to_nav(ns, pawn, px + 10 * M, py, pz, a) or e
                            local p2 = ns:FindPathToLocationSynchronously(pawn,
                                { X = s2.x, Y = s2.y, Z = s2.z },
                                { X = e2.x, Y = e2.y, Z = e2.z }, a, nil)
                            local c2 = "nil"
                            if p2 then pcall(function() c2 = tostring(#p2.PathPoints) end) end
                            f:write(string.format("  findpath via %s -> points=%s\n", nm, c2))
                        end
                    end
                end)
                if not okp then f:write("  findpath LUA-ERROR: " .. tostring(perr) .. "\n") end
            end
        end
        -- RAYCAST probes (obstacle avoidance): cast a ray CAMERA-FORWARD at chest
        -- height, 5 m, through BOTH reflected trace overloads, EACH behind its own
        -- one-shot fuse (an abort marks only that overload bad; the next dump skips it
        -- and still runs the other). ForObjects aborted uncatchably on Area02 AND
        -- Area04 even freshly re-armed (2026-07-06) — the TArray<EObjectTypeQuery>
        -- marshalling is the prime suspect, which the channel overload avoids. The
        -- unbuffered writes show exactly which call died.
        if pawn and px then
            local rfx, rfy = camera_forward()
            if not rfx then
                f:write("raycast probe: no camera forward\n")
            else
                local sz = pz + AVOID_HEIGHT
                local d = 5 * M
                local function stage(fuse, name, values, call)
                    if _G[fuse] == "bad" then
                        f:write(string.format("raycast %s: SKIPPED (aborted on this map)\n", name))
                        return
                    end
                    if _G[fuse] == "testing" then
                        _G[fuse] = "bad"
                        f:write(string.format("raycast %s: SKIPPED (previous trace aborted mid-flight)\n", name))
                        return
                    end
                    _G[fuse] = "testing"
                    for _, v in ipairs(values) do
                        local blocked, dist = call(v)
                        f:write(string.format("raycast %s=%d -> %s%s\n", name, v,
                            (blocked == nil and "API-UNAVAILABLE"
                                or blocked and "BLOCKED" or "clear"),
                            (dist and string.format(" dist=%.0f", dist) or "")))
                    end
                    _G[fuse] = "ok"
                end
                stage("__KakarotRayNative", "ForObjects objType", { 1, 2, 3, 4, 5, 6 },
                    function(ot)
                        return Ray.probe(pawn, px, py, sz,
                            px + rfx * d, py + rfy * d, sz, { ot })
                    end)
                stage("__KakarotRayChan", "LineTraceSingle channel", { 0, 1, 2, 3 },
                    function(ch)
                        return Ray.probe_channel(pawn, px, py, sz,
                            px + rfx * d, py + rfy * d, sz, ch)
                    end)
            end
        end
        -- Collectible-state probe: every collectible-class actor within 150 m with its
        -- hidden/state fields. Diagnoses the "collected item still listed" reports:
        -- collect the item, Ctrl+F5, and see which field (bHidden / InteractState /
        -- actor gone) the game actually flips for THAT item kind.
        for _, c in ipairs({
            { cls = "FieldActionPointActor" },
            { cls = "PlacementObjectInfo", state = true },
            { cls = "AccessPointItemBase", state = true },
        }) do
            f:write("probing " .. c.cls .. "...\n")
            local n = 0
            for _, a in pairs(FindAllOf(c.cls) or {}) do
                if Core.valid(a) then
                    local x, y, z = actor_pos(a)
                    if x then
                        local dd = math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
                        if dd <= 150 * M then
                            n = n + 1
                            local cn, hid, st = "?", "?", "-"
                            pcall(function() cn = a:GetClass():GetFName():ToString() end)
                            pcall(function() hid = tostring(a.bHidden) end)
                            if c.state then
                                pcall(function() st = tostring(tonumber(a.InteractState)) end)
                            end
                            f:write(string.format(
                                "  collectible[%s] d=%.0fm cls=%s bHidden=%s state=%s\n",
                                c.cls, dd / M, cn, hid, st))
                        end
                    end
                end
            end
            f:write(string.format("collectible probe %s: %d within 150m\n", c.cls, n))
        end
        -- Field-enemy probe (SpawnType scan): who is around and how far.
        do
            local list = enemies_list()
            f:write(string.format("field enemies (SpawnType != 0): %d cached\n", #list))
            for i, e in ipairs(list) do
                if Core.valid(e.actor) then
                    local x, y, z = actor_pos(e.actor)
                    local dd = x and math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
                    f:write(string.format("  enemy[%d] %s d=%s %s\n", i, e.noun,
                        dd and string.format("%.0fm", dd / M) or "?", e.actor:GetFullName()))
                end
            end
        end
        -- RAW AT_Character probe: EVERY character within 300 m with its class + raw
        -- SpawnType. If roaming field enemies (the pre-Raditz robots read as nothing,
        -- 2026-07-06) are NOT SpawnType!=0 AT_Characters, this shows what they ARE.
        do
            f:write("probing AT_Character (SpawnType)...\n")
            local n = 0
            for _, c in pairs(FindAllOf("AT_Character") or {}) do
                if Core.valid(c) then
                    local x, y, z = actor_pos(c)
                    if x then
                        local dd = math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
                        if dd <= 300 * M then
                            n = n + 1
                            local cn, st = "?", "?"
                            pcall(function() cn = c:GetClass():GetFName():ToString() end)
                            pcall(function() st = tostring(tonumber(c.SpawnType)) end)
                            f:write(string.format("  atchar d=%.0fm spawnType=%s cls=%s\n",
                                dd / M, st, cn))
                        end
                    end
                end
            end
            f:write(string.format("AT_Character within 300m: %d\n", n))
        end
        -- Mob probe (2026-07-09: "you confused the enemies with animals"). AT_MobBase covers
        -- wild animals (AnimalMob_Pawn), townsfolk mobs AND possibly MOB-TYPE ENEMIES. List
        -- every mob's class + distance so we can tell real animals from enemies and fix the
        -- classification (the enemy scan only looks at AT_Character and may miss mob enemies).
        do
            f:write("probing AT_MobBase...\n")
            local n = 0
            for _, m in pairs(FindAllOf("AT_MobBase") or {}) do
                if Core.valid(m) then
                    local x, y, z = actor_pos(m)
                    if x then
                        local dd = math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
                        if dd <= 300 * M then
                            n = n + 1
                            local cn = "?"
                            pcall(function() cn = m:GetClass():GetFName():ToString() end)
                            f:write(string.format("  mob d=%.0fm cls=%s\n", dd / M, cn))
                        end
                    end
                end
            end
            f:write(string.format("AT_MobBase within 300m: %d\n", n))
        end
        -- Wind-tunnel probe (user 2026-07-09: the túnel de viento isn't showing). The radar
        -- scans FindAllOf("ATWindRoad") + FieldPointComponent type 2 (WindPath); log what's
        -- actually near so we can see the REAL class/type/distance of the wind tunnel.
        do
            f:write("probing ATWindRoad...\n")
            local nw = 0
            for _, a in pairs(FindAllOf("ATWindRoad") or {}) do
                if Core.valid(a) then
                    nw = nw + 1
                    local x, y, z = actor_pos(a)
                    local dd = x and math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
                    local cn = "?"
                    pcall(function() cn = a:GetClass():GetFName():ToString() end)
                    f:write(string.format("  windroad d=%s cls=%s\n",
                        dd and string.format("%.0fm", dd / M) or "?", cn))
                end
            end
            f:write(string.format("ATWindRoad total: %d\n", nw))
            f:write("probing FieldPointComponent...\n")
            local nf = 0
            for _, comp in pairs(FindAllOf("FieldPointComponent") or {}) do
                if comp and comp:IsValid() then
                    local t, ocls, dd = "?", "?", nil
                    pcall(function() t = tostring(tonumber(comp.FieldPointIconType)) end)
                    local owner
                    pcall(function() owner = comp:GetOwner() end)
                    if Core.valid(owner) then
                        pcall(function() ocls = owner:GetClass():GetFName():ToString() end)
                        local x, y, z = actor_pos(owner)
                        dd = x and math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2)
                    end
                    if not dd or dd <= 400 * M then
                        nf = nf + 1
                        f:write(string.format("  fieldpoint type=%s owner=%s d=%s\n", t, ocls,
                            dd and string.format("%.0fm", dd / M) or "?"))
                    end
                end
            end
            f:write(string.format("FieldPointComponent (within 400m): %d\n", nf))
        end
        f:write("current target: " .. (target and (target.label .. " " .. target.key) or "none") .. "\n")
        f:close()
        Speech.say("nav dump written", true)
    end)
end

function Nav.is_on() return on end

return Nav
