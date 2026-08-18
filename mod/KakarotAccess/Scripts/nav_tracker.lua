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
local Mem = require("mem")
-- NOTE: pad_poll (the shared tick bus) is required INSIDE Nav.start/Nav.stop, not here. This
-- file's main chunk sits at Lua's hard ceiling of 200 locals, so one more top-level `local`
-- fails to compile outright ("too many local variables") — the lint catches it, but the cheaper
-- lesson is not to add one. `require` is a cached table lookup after the first call.

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
local ENEMY_ALERT_DIST = 160 * M      -- proximity radius for the spoken enemy warning
                                      -- (50m originally; raised to ~the game's own aim/lock
                                      -- detection reach at the user's request, 2026-07-17)
local ENEMY_CUE_MS = 8000             -- min gap between enemy warnings (anti-spam)
local ENEMY_CLOSER_FACTOR = 0.5       -- re-announce the SAME enemy only when this much closer

-- EMapIcon values (AT_enums.hpp) that mark QUEST objectives on the map.
local MAIN_QUEST_ICONS = { [24] = true, [71] = true }   -- MAINQUEST_NO_BATTLE, MAINQUEST_RANGE_ONLY
local SUB_QUEST_ICONS = {                               -- SUBQUEST_* + DLC story sub quests
    [26] = true, [40] = true, [47] = true, [53] = true, [59] = true, [72] = true,
}

-- Target priority classes (higher wins; ties broken by distance).
local PRI_MAIN, PRI_SUB, PRI_OTHER = 3, 2, 1

-- ---- radar target-picker categories (R3 menu, radar_menu.lua) ---------------
-- EMapIcon (AT_enums.hpp) grouped into the player-facing categories the R3 menu
-- cycles with L1/R1. Anything unmapped falls into "other". The group order is the
-- L1/R1 tab order (empty groups are skipped when cycling).
-- "exit" sits SECOND, right after quests. It is the get-me-out-of-here category (user 2026-07-25:
-- stuck inside Goku's house with no way to find the door), so it has to be reachable in one R1 —
-- but making it FIRST would displace quests as the default tab everywhere, since doors exist all
-- over the outdoors too. Inside a building this costs nothing anyway: nearly every other group is
-- empty there and empty groups are skipped when cycling.
local GROUP_ORDER = {
    "quests", "exit", "collectibles", "npc", "companions", "enemies", "hunt", "sites", "fishing",
    "gathering", "shops", "minigames", "dragonball", "other",
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
    -- gathering (collect / ore / bugs / rocks). HUNT (7 = prey marker) gets its own
    -- "hunt" category alongside the wild animals, not lumped with fruit/ore gathering.
    [6] = "gathering", [7] = "hunt", [8] = "gathering", [18] = "gathering",
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
-- One table (not two locals: the main chunk is at Lua's 200-local limit).
local RADAR_CAP = {
    npc = 300 * 100,   -- distance limit for the NPC direct-scan (drops the far
                       -- parked-character pool ~2600 m away)
    map = 1000 * 100,  -- cap for the game's OWN minimap icons: shows the LOCAL
                       -- area's gather/shop/minigame/fishing markers without
                       -- listing the regional ones 2-3 km away (clutter). A
                       -- hard 300 m was too tight; 5 km too loose. Quests: no
                       -- limit at all.
}

-- ---- state ---------------------------------------------------------------------
local on = true                -- master switch (F3); radar auto-tracks while on
local route_mode = true        -- NavMesh route guidance (Shift+F3)
local running = false
local tick = 0                 -- loop tick counter; tick * TICK_MS is our clock

local target = nil             -- { actor, key, pri, label, manual }
local target_missing = 0       -- consecutive scans where the target wasn't found
local companion_idx = 0        -- Shift+F5 cycle: 0 = quest mode; 1..n = that companion
local auto_suppressed = false  -- after reaching / stopping a target the auto-scan stays
                               -- quiet (no re-acquire) until you re-pick (R3 menu) or F3.
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
-- "exit" is NOT chainable either, and that exclusion is load-bearing twice over (user 2026-07-25:
-- "dice salidas pero el radar no las rastrea"). `sweeping` = manual + chainable, and it gates the
-- ghost filter below at line ~1531, which drops a reached target whose `bHidden` is true. That
-- filter was added 2026-07-17 for the parked future-story CHARACTERS the game hides near the
-- player — but a door is an `ATriggerBox`, i.e. an INVISIBLE VOLUME whose bHidden is true as its
-- normal state. So the first tick after picking a door chained straight over it to the next door,
-- also hidden, also dropped: a door could never be tracked at all. Making exits non-chainable also
-- gets the semantics right — on reaching an exit you walk THROUGH it and the world changes, so
-- sweeping to "the next door" is not a thing anyone wants.
local function chainable(grp) return grp ~= nil and grp ~= "quests" and grp ~= "exit" end
local chain_wait = nil         -- { actor, key, grp, stateful } while at the reached item
local chain_seen = {}          -- keys already visited in this sweep (never re-targeted)

-- Fresh-objective auto-track (user request 2026-07-17): when the quest HUD's objective
-- TEXT changes (quest_objective.lua's signature diff — the game's own "new objective"
-- event: talking to an NPC, grabbing a quest item, or a story battle all update it),
-- the auto-scan may acquire the new quest marker even over a manual pick, a stop, or a
-- pending battle-resume — so the player never needs the R3 menu for story progress.
-- Whatever they were doing is stashed in `preempt.stash` (plain Lua data, survives
-- battles and map switches; stale seen-keys never match a new level's actors,
-- harmless) and B (stop_tracking) returns to it: nearest remaining item for chainable
-- categories, the exact pick otherwise.
-- One table (not separate locals: the main chunk is at Lua's 200-local limit).
local preempt = {
    TRIES = 10,    -- scans (~15 s of world time) to find the new marker — it can lag
                   -- the HUD text by a few seconds; after that the signal is
                   -- forgotten, never stealing a pick later
    scans = 0,     -- >0: the next scans may preempt (counts down on misses)
    pri = nil,     -- PRI_MAIN/PRI_SUB: prefer the changed quest kind's marker
    stash = nil,   -- { key, grp, stateful, seen } — the interrupted pick/sweep
}

-- Battle interruption resume (user bug 2026-07-15): the world gate / a map switch
-- DROPS a hand-picked target (the actor dies with the gate), and the auto-scan then
-- re-acquired the STORY marker instead of the user's pick. Remember the pick as plain
-- Lua data (no object refs — safe across teardown) and re-acquire it by key through
-- the picker's own enumeration once the world is back; the quest auto-scan stays
-- quiet while a resume is pending. Companion targets (grp == nil) are not resumable
-- through the picker and keep the old behavior.
local resume_pick = nil        -- { key, label, grp, stateful, tries }
local RESUME_TRIES = 10        -- attempts before giving up. The FIRST lands as soon as the
                               -- shared snapshot is ready (usually the tick after the world
                               -- returns); retries are spaced SCAN_EVERY*3 ticks (~4.5 s) and
                               -- an attempt that found no snapshot at all does not count.

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
local MM_RETRY_TICKS = 10    -- ~1 s between acquisition attempts. Was 50 (~5 s), chosen when
                             -- this WAS a raw FindAllOf; `Core.cached_live` now resolves it
                             -- through the screen directory (a pointer read) and owns the cost
                             -- of the scan fallback itself — a budget slot plus its own absent
                             -- backoff. A second, longer lockout stacked on top of that only
                             -- delays the one signal every other gate in this file depends on.

local function minimap()
    if Core.valid(mm_cache) then return mm_cache end
    mm_cache = nil
    if ui_muted() then return nil end
    if tick < mm_retry then return nil end
    -- cached_live resolves this through the screen directory (UIFieldManager.MapMng
    -- .MinimapIns — a pointer read), so acquisition costs no object scan at all.
    --
    -- Arm the backoff ONLY ON A MISS (2026-08-03). It used to be written BEFORE the probe
    -- ran, so a single unlucky attempt — the HUD root not yet re-resolved after a load, which
    -- is precisely when this is first called — locked the minimap out for a full 5 s. And
    -- `world_alive()` is false for every one of those ticks, so as far as the rest of this
    -- file is concerned the world does not exist yet: no auto-acquire, no beacon, nothing.
    -- That is the largest single contributor to "the radar takes a long time to start
    -- tracking after loading a save".
    mm_cache = Core.cached_live("AT_UIMiniMapRadar")
    if not mm_cache then mm_retry = tick + MM_RETRY_TICKS end
    return mm_cache
end

-- The field player actor. PlayerController.Pawn proved UNRELIABLE in the overworld
-- (nil / a stale pawn at the origin — seen live 2026-07-03), so the PRIMARY source is
-- the minimap radar's own PlayerIns (AAT_Character); the pawn is only a fallback.
local function player_pawn()
    local mm = minimap()
    if Core.valid(mm) then
        -- mm is the ACROSS-TICK cache: gate the fetch itself, not just the handle.
        local ok, ins = pcall(function() return Core.member(mm, "PlayerIns") end)
        if ok and Core.valid(ins) then return ins end
    end
    local pc = FindFirstOf("PlayerController")
    if not Core.valid(pc) then return nil end
    local ok, pawn = pcall(function() return Core.member(pc, "Pawn") end)
    if ok and Core.valid(pawn) then return pawn end
    return nil
end

-- SELF-GUARDING (2026-07-31, the streamer's pad.tick trail). This is the exact call the whole
-- AV-at-0x10 crash family dies on: a raw reflected call on an actor the engine has freed. It used
-- to rely ENTIRELY on ~35 call sites each remembering to `Core.valid` first, and that discipline
-- had already broken in the one place it mattered — `explore_rescan` fed it actors straight out of
-- the shared TTL snapshot with no check at all, so a handle up to EXPLORE_RESCAN_MS old was
-- dereferenced raw. A convention that must hold at 35 sites is not a guard; put it in the callee.
-- COST: `Core.valid` is memoized per tick (Core.drop_memos), so for the callers that already
-- validated — nearly all of them — this is a table hit, not a second Mem.alive + IsValid.
local function actor_pos(actor)
    if not Core.valid(actor) then return nil end
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
        local ok, r = pcall(function()
            local cm = Core.member(mm, "CameraMng")   -- mm is cached across ticks
            if Core.valid(cm) then return cm:GetCameraRotation() end
        end)
        if ok then rot = r end
    end
    if not rot then
        local pc = FindFirstOf("PlayerController")
        if not Core.valid(pc) then return nil end
        local ok, r = pcall(function()
            local cam = Core.member(pc, "PlayerCameraManager")
            if Core.valid(cam) then return cam:GetCameraRotation() end
        end)
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
        if Core.valid(comp) then
            if Core.member(comp, "bShowMapIcon") == false then return end
            t = tonumber(Core.member(comp, "MapIconType"))
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
        if Core.valid(comp) then
            if Core.member(comp, "bShowMapIcon") == false then return end
            t = tonumber(Core.member(comp, "MapIconType"))
            range = tonumber(Core.member(comp, "SearchRangeRadius"))
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
        if Core.valid(comp) then t = tonumber(Core.member(comp, "MapIconType")) end
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
        local sw = Core.member(icon, "WL_NaviIconSw")
        if not (Core.valid(sw)) then return end
        local s = Core.member(sw, "WL_NaviIconSwitcher")
        if Core.valid(s) then idx = tonumber(Core.member(s, "ActiveWidgetIndex")) end
        if not (idx and idx >= 1 and idx <= 3) then
            local b = Core.member(sw, "WL_NaviBaseSwitcher")
            if Core.valid(b) then idx = tonumber(Core.member(b, "ActiveWidgetIndex")) end
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
-- `icon` (optional) is the NAVI widget the actor came from, and when present it WINS.
--
-- This is the fix for "the radar goes back to the main quest while I am doing a side story"
-- (user, 2026-07-28). `navi_quest_icon` has existed since the 07-04 report and its own comment
-- says the actor's ATMapIconComponent is main-coded or absent on sub quests — but it was only
-- ever wired into `Nav.list_targets` (the R3 picker) and the debug dump, never into the auto-scan.
-- So picking a side-story marker BY HAND worked while auto-tracking classified the same marker as
-- PRI_MAIN or PRI_OTHER: `notify_objective_change("sub")` armed `preempt.pri = PRI_SUB`, no
-- candidate ever matched it, and `best_candidate` fell through to the overall best — a
-- concurrently active MAIN arrow. The intermittency (only when a main quest is also live) is the
-- tell. Fails open: an unreadable switcher classifies exactly as before.
--
-- Folded into this function rather than given its own helper because this chunk is at Lua's
-- 200-local cap and one more `local function` is a hard syntax error (the lint caught it).
local function classify(actor, icon)
    if icon then
        local t = navi_quest_icon(icon)
        if t then
            if MAIN_QUEST_ICONS[t] then return PRI_MAIN end
            if SUB_QUEST_ICONS[t] then return PRI_SUB end
        end
    end
    return quest_pri(actor) or PRI_OTHER
end

-- Is this pooled minimap icon actually in use (one of its widgets rendered)?
-- Each sub-widget goes through Core.member: a naked `icon.WL_*` at the call site is a
-- property __index on a pooled icon the game may have just recycled — the uncatchable
-- AV class (CLAUDE.md §8), and this runs on every radar sweep.
local function icon_in_use(icon)
    local ok, vis = pcall(function()
        return Core.on_screen(Core.member(icon, "WL_QuestIconSw"))
            or Core.on_screen(Core.member(icon, "WL_NaviIconSw"))
            or Core.on_screen(Core.member(icon, "WL_Icon_ImgSw"))
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
-- want_pri (optional, fresh-objective scans): prefer a marker of THAT quest class —
-- when a SUB objective just advanced while a main arrow is also active, the sub marker
-- must win even though main outranks it normally. Falls back to the overall best.
local function best_candidate(px, py, pz, want_pri)
    -- The refresh is a raw full-array FindAllOf (~65ms): never spend it while a
    -- cutscene/dialogue overlay owns the screen (Core.scan_quiet). But an EXPIRED list is
    -- DROPPED rather than kept (2026-07-26, the same fix as enemies_list — see the long note
    -- there). "The stale list keeps serving, entries are re-validated below" was the old
    -- reasoning and it is unsound: these are per-level pooled minimap widgets, the engine frees
    -- them on teardown, and re-validating cannot detect an address the engine has recycled.
    -- Dropping costs nothing and `navi_next` stays in the past, so the first non-quiet tick
    -- does the real refresh.
    -- Emptied, NOT returned from: this function draws candidates from other sources further
    -- down (the minimap icon list), and those are still valid. The loop below simply iterates
    -- nothing.
    -- `tick >= navi_next` REMOVED (2026-07-31, the Krillin-cutscene report). The reasoning above
    -- is that these handles are freed by the engine on teardown and re-validation cannot see a
    -- recycled address — and that argument does not depend on the list having EXPIRED. With the
    -- expiry conjunct in place, a list refreshed a few seconds before a cutscene began was kept
    -- and dereferenced for the cutscene's whole duration, which is exactly the window where the
    -- sub-levels holding those widgets stream out. A cutscene is not a Transition, so nothing
    -- else releases them either. Quiet alone is the right condition: it means an overlay owns
    -- the screen, i.e. both that the list cannot be refreshed AND that the world may be moving
    -- under it. Still emptied to {} rather than nil, so the branch below cannot read this as
    -- "never fetched" and force a 65 ms FindAllOf mid-cutscene.
    if navi_icons ~= nil and Core.scan_quiet() then
        navi_icons = {}
    end
    if navi_icons == nil or (tick >= navi_next and not Core.scan_quiet()) then
        navi_icons = Core.findall("AT_UIMiniMapNaviIcon")
        navi_next = tick + RESCAN_CLASSES
    end
    local best, best_pri, best_d2 = nil, 0, math.huge
    local want, want_d2 = nil, math.huge
    local function consider(ta, pri)
        local x, y, z = actor_pos(ta)
        if not x then return end
        local d2 = (x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2
        if want_pri and pri == want_pri and d2 < want_d2 then
            want, want_d2 = ta, d2
        end
        if pri > best_pri or (pri == best_pri and d2 < best_d2) then
            best, best_pri, best_d2 = ta, pri, d2
        end
    end

    -- BLACK-BOX MARKS (2026-07-31). best_candidate's two walks were an unmarked window inside
    -- `nav.step`: both dereference per-level pooled widgets, and if the process dies in one of
    -- them the trail blames whichever loop wrote the last mark. That is the same blind spot the
    -- registry prologue had, and the rule earned there was that when two candidates share an
    -- unmarked window the deliverable is a MARK, not a hypothesis. One memcpy each, on a ~1.5 s
    -- cadence — nothing next to the ~36 the sweep already writes per tick.
    Mem.mark("nav.markers")
    for _, icon in pairs(navi_icons) do
        if Core.valid(icon) and icon_in_use(icon) then
            -- navi_icons is a module-level list held for RESCAN_CLASSES ticks.
            local ok, ta = pcall(function() return Core.member(icon, "TargetActor") end)
            if ok and Core.valid(ta) then consider(ta, classify(ta, icon)) end
        end
    end

    if not best then
        Mem.mark("nav.mapicons")   -- the second half of the unmarked window; see nav.markers above
        local mm = minimap()
        if Core.valid(mm) then
            pcall(function()
                local arr, n = Core.array_of(mm, "MapIconList")   -- raw #arr = uncatchable throw
                if not arr then return end
                for i = 1, n do
                    local icon = arr[i]
                    if Core.valid(icon) then
                        -- Gated hop (was a raw `icon.TargetActor`): icon is a pooled minimap
                        -- widget the engine can recycle, and a naked property __index on one
                        -- is the same uncatchable-abort shape icon_in_use's comment warns
                        -- about (CLAUDE.md §8) — Core.valid(icon) proves the handle is alive
                        -- right now, not that the fetch itself is safe.
                        local ta = Core.member(icon, "TargetActor")
                        if Core.valid(ta) then
                            local pri = quest_pri(ta)
                            if pri then consider(ta, pri) end
                        end
                    end
                end
            end)
        end
    end

    if want then
        return { actor = want, key = tostring(want:GetAddress()), pri = want_pri,
                 label = label_for(want_pri) }
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
        for _, a in pairs(Core.findall("RecastNavMesh")) do
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
    if not (Core.valid(cdo)) then return nil end
    local nav
    pcall(function() nav = cdo:GetNavigationSystem(ctx) end)
    if Core.valid(nav) then return nav end
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
            -- Gated fetch + validity: this runs when the radar is switched off, which can be
            -- during a teardown, and the pawn was being handed unvalidated into a reflected
            -- engine call on a nav system cached across ticks.
            local pw = Core.member(pc, "Pawn")
            if Core.valid(pw) then invoker_nav:UnregisterNavigationInvoker(pw) end
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
        -- The path is a REFLECTED NavigationSystem result and level streaming can hand back
        -- one whose object is already dead — reading .PathPoints or #arr on it is the
        -- uncatchable C++ throw that crashed free-roam (2026-07-14; pcall does NOT catch it,
        -- see Core.array_of). So validate the path (UE4SS-shadowed IsValid) and the array
        -- before touching its length. An unusable path just yields <2 points.
        local arr, n = Core.array_of(path, "PathPoints")
        if not arr or n < 2 then return nil end
        local out = {}
        for i = 1, n do
            -- Route corners are FVector STRUCT handles out of a reflected array; an entry the
            -- engine has freed under us is an invalid handle, not nil, so read it only after
            -- valid_ref (IsValid only — GetAddress on a struct pierces pcall).
            local p = arr[i]
            if not Core.valid_ref(p) then return nil end
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

-- Raycast obstacle avoidance is DISABLED on this game. Both LineTrace overloads are
-- broken through reflection here: the calls raise a UFunction param-count error
-- (expected 13, received 11) that the pcall CATCHES — so the abort fuse below never
-- trips, steer_around retried every tick, and the repeated UE4SS error path ended in a
-- fatal EXCEPTION_ACCESS_VIOLATION (2026-07-13, in-game crash). ForObjects had already
-- aborted uncatchably on Area02/Area04 (2026-07-06). Guidance goes direct-to-target;
-- only re-enable if a reflected trace is someday proven safe on this game.
local RAYCAST_AVOIDANCE = false

-- Which trace overload is still usable here? ForObjects first (validated where it
-- works), else the channel overload: ForObjects aborts uncatchably on some maps
-- (Area02 + Area04, 2026-07-06 — TArray marshalling suspected) where the simpler
-- LineTraceSingle may survive. EACH overload has its own abort fuse, so fusing one
-- falls through to the other instead of killing avoidance outright.
local function ray_api()
    if not RAYCAST_AVOIDANCE then return nil end
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

-- `why` is DIAGNOSTIC ONLY and optional: the deliberate drops (arrival, B, a fresh pick) pass
-- nothing and stay silent, because the player asked for those and can hear the result. The two
-- AUTOMATIC ones name themselves, because an unasked-for drop is followed by a re-acquisition
-- that RE-ANNOUNCES the objective, and three rounds of reasoning about "the radar untracks by
-- itself while I walk" (2026-08-03) produced two fixes and one regression without ever seeing
-- which branch fired. One line per drop, never per tick, and it carries the state that decides
-- the branch — whether the handle was already gone, and whether the scan that "missed" was
-- allowed to look at all.
local function drop_target(why)
    if why and target then
        print(string.format(
            "[KakarotAccess] nav drop target (%s) key=%s actor=%s missing=%d quiet=%s t=%.2f\n",
            why, tostring(target.key), target.actor ~= nil and "live" or "NIL",
            target_missing, tostring(Core.scan_quiet()), os.clock()))
    end
    target, route, route_idx = nil, nil, 0
    arrived, target_missing = false, 0
    companion_idx = 0   -- a dropped manual target reverts to quest mode
    last_stealth_zone = nil
    was_on_target = false
    last_elev_zone = nil
end

-- Stash the current hand-picked target right before a FORCED drop (world gate closed,
-- map switch) so the post-battle tick can re-acquire it. Deliberate drops (arrival,
-- B, F3 off, a new pick) must NOT call this — those clear resume_pick instead.
local function remember_pick()
    if target and target.manual and target.grp then
        resume_pick = { key = target.key, label = target.label, grp = target.grp,
                        stateful = target.stateful, tries = 0 }
    end
end

-- Stash whatever the radar was doing (manual pick, mid-sweep wait, pending resume)
-- right before a fresh quest objective preempts it — B (stop_tracking) restores it.
-- See the `preempt` block comment for the lifecycle. (A field, not a local: the main
-- chunk is at Lua's 200-local limit.)
function preempt.stash_now()
    if target and target.manual and target.grp then
        preempt.stash = { key = target.key, grp = target.grp,
                          stateful = target.stateful, seen = chain_seen }
    elseif chain_wait then
        preempt.stash = { key = chain_wait.key, grp = chain_wait.grp,
                          stateful = chain_wait.stateful, seen = chain_seen }
    elseif resume_pick then
        preempt.stash = { key = resume_pick.key, grp = resume_pick.grp,
                          stateful = resume_pick.stateful, seen = chain_seen }
    end
    chain_wait = nil
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
    remember_pick()   -- resume_pick is plain Lua data: it survives the map switch
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
    -- The shared picker snapshot is a whole LIST of world-actor handles (Nav.targets_cached):
    -- every actor in it lived in the level that is being torn down.
    Nav.targets_snap, Nav.targets_snap_at = nil, 0
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
    -- A NIL handle is NOT the same event as a dead one — the same distinction step() makes for
    -- `target.actor`, which this branch never got (user report 2026-07-31: during a collectible
    -- sweep, opening any pausing menu — or getting pulled into a battle — makes the radar skip
    -- the item the player is standing on and point at the next one, forever). `release_world_refs`
    -- nils this handle on the falling edge of EVERY gate, so nil means only "the world went away
    -- while we were waiting here", never "it was collected". Advancing on it ran chain_to_next
    -- while the arrival path had already written chain_seen[key], so the skipped item could never
    -- be re-targeted for the rest of the sweep. Stash it as plain data in the same resume slot
    -- step() uses instead: the resume scan re-acquires it by category+key once the world is back,
    -- chain_seen is left untouched, and arriving on it re-arms chain_wait with a live handle. (No
    -- label: chain_wait never carried one, and the resume scan speaks Nav.item_label(found).)
    if a == nil then
        resume_pick = { key = chain_wait.key, grp = chain_wait.grp,
                        stateful = chain_wait.stateful, tries = 0 }
        chain_wait = nil
        return
    end
    -- Only a handle that is non-nil AND fails validation counts as collected/despawned.
    local advance = not Core.valid(a)
    if not advance then
        local hidden, st = false, nil
        -- chain_wait.actor was captured on an earlier tick: streaming can free it.
        pcall(function() hidden = Core.member(a, "bHidden") end)
        if chain_wait.stateful then
            pcall(function() st = tonumber(Core.member(a, "InteractState")) end)
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
    pcall(function() st = tonumber(Core.member(c, "SpawnType")) end)
    return st
end

-- True for the player and companions (they inherit AT_CharacterPlayableBase); those are
-- the only AT_Characters we must NOT flag as enemies.
-- Presence filter shared by the enemy and companion scans: the game parks preloaded
-- future-story characters HIDDEN in the world (the "absence observer" — same reason
-- QuestCharacters check bHidden/CurrentHiddenType). AT_Character does NOT declare
-- CurrentHiddenType (AQuestCharacter-only, AT.hpp:17553 — reading it here would be the
-- uncatchable abort), so bHidden (AActor-level, safe) is the whole check. Fixes the
-- ghost enemies inside Goku's house at game start (user report 2026-07-17).
local function char_visible(c)
    local hidden = false
    pcall(function() hidden = Core.member(c, "bHidden") end)
    return not (hidden == true or hidden == 1)
end

local function is_playable_char(c)
    if playable_cls == nil then
        local ok, k = pcall(function() return StaticFindObject("/Script/AT.AT_CharacterPlayableBase") end)
        playable_cls = (ok and k) or false
    end
    if not playable_cls then return false end
    local ok, r = pcall(function() return c:IsA(playable_cls) end)
    return ok and r == true
end

-- Live ENEMY level: reflected hops AAT_Character.AttributeComponent -> .StatusInstance,
-- then the native int32 at statusInstance.level — ENEMY instances only (ATEnemyStatus);
-- the player's instance keeps a POINTER in that slot (see native_offsets). Only ever call
-- this on enemies_list members (non-playable AT_Characters), never the player/companions.
-- nil when unreadable (mem_bridge missing, component gone, or an implausible misread).
local function enemy_level(c)
    local off = (require("native_offsets").statusInstance or {}).level
    if not off or not Mem.is_loaded() then return nil end
    local si
    pcall(function()
        local a = Core.member(c, "AttributeComponent")
        if Core.valid(a) then
            local s = Core.member(a, "StatusInstance")
            if Core.valid(s) then si = s end
        end
    end)
    if not si then return nil end
    local lv = Mem.i32(si, off)
    -- Plausibility guard, not a game rule: the getter's own floor is 1 (Ghidra); the top
    -- bound only rejects garbage from a stale/foreign block (same band the dump filter used).
    local LV_GUARD_MAX = 999
    if lv and lv >= 1 and lv <= LV_GUARD_MAX then return lv end
    return nil
end

-- The live field-enemy list { {actor, noun}, ... } (cached; entries re-validated by
-- every user). Player/companions are excluded by the playable-base check.
local function enemies_list()
    -- EXPIRED + CANNOT REFRESH => DROP THE LIST (2026-07-26). The quiet-mode deferral below is
    -- right about the cost — a ~65 ms FindAllOf during a cutscene is exactly what quiet mode
    -- exists to prevent — but it drew the wrong conclusion from it: it kept SERVING the expired
    -- list instead. These entries are ENEMY ACTORS, the one object family the engine destroys
    -- as a matter of course, and `scan_quiet` is set by the dialogue adapter. So the sequence
    -- "finish a battle, story dialogue opens" pinned a list of just-destroyed enemies and the
    -- radar kept dereferencing it, for as long as the conversation lasted. That is the user's
    -- 2026-07-26 crash (black box: last op `nav.step`; AV reading 0x10 = GetClassPrivate on a
    -- dead handle) and the handle-lifetime audit had flagged this exact list as the most likely
    -- mid-combat site.
    --
    -- Dropping is the correct answer, not scanning anyway: it costs NOTHING, and a radar that
    -- announces no enemies during a cutscene is right, whereas one that announces enemies that
    -- no longer exist is wrong even when it does not crash. `enemy_next` is deliberately left in
    -- the past so the first non-quiet tick does a real refresh.
    -- Re-validating per use is NOT a substitute: a freed address the engine has already recycled
    -- passes every check we can make (mem.lua says so in as many words).
    -- `tick >= enemy_next` REMOVED (2026-07-31), the same sweep best_candidate got on the Krillin
    -- report. The argument above — these are enemy actors, the engine frees them as a matter of
    -- course, and re-validation cannot see a recycled address — never depended on the list having
    -- EXPIRED. With the expiry conjunct here, a list refreshed less than RESCAN_CLASSES ticks
    -- before a cutscene or dialogue began was KEPT and dereferenced for that whole scene, which is
    -- precisely the window the actors die in. Quiet alone is the right condition.
    if enemy_cache ~= nil and Core.scan_quiet() then
        enemy_cache = {}
        return enemy_cache
    end
    -- Same quiet-mode deferral as best_candidate: no raw FindAllOf during subtitles.
    if enemy_cache == nil or (tick >= enemy_next and not Core.scan_quiet()) then
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
            if Core.valid(c) and not is_playable_char(c) and char_visible(c) then
                local near = true
                if px then
                    local x, y, z = actor_pos(c)
                    near = x ~= nil
                        and ((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2) <= RADAR_CAP.npc ^ 2
                end
                if near then
                    -- SpawnType 1/2/3 → specific noun; roaming enemies are SpawnType 0 → generic.
                    local noun = ENEMY_NOUN_BY_SPAWN[enemy_spawn_type(c) or 0] or "cat_enemy"
                    -- The game's own display name (CharacterName id -> GetCharacterName),
                    -- so the radar says "Soldier X" instead of the generic "enemy".
                    local name = enemy_display_name and enemy_display_name(c) or nil
                    -- disp = spoken label with the live level baked in ("Soldado, nivel 12");
                    -- used by the proximity alert and the radar picker. Falls back to name/noun.
                    local lvl = enemy_level(c)
                    local disp
                    if lvl then
                        disp = string.format("%s, %s", name or I18n.t(noun),
                            string.format(I18n.t("nav_level"), lvl))
                    end
                    enemy_cache[#enemy_cache + 1] = { actor = c, noun = noun, name = name, disp = disp }
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
        local comp = Core.member(a, "NpcComponent")
        if not (Core.valid(comp)) then return end
        if animal_comp_cls == nil then
            local ok, c = pcall(function() return StaticFindObject("/Script/AT.AnimalComponentBase") end)
            animal_comp_cls = (ok and c) or false
        end
        if not animal_comp_cls or not comp:IsA(animal_comp_cls) then return end
        local t = tonumber(Core.member(comp, "AnimalType"))
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
                            if not (Core.valid(c2)) then break end
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
        local comp = Core.member(pawn, "LockonList")
        if Core.valid(comp) then
            local arr, n = Core.array_of(comp, "m_xActors")   -- raw #arr here is the uncatchable throw
            if arr and n > 0 then
                local a = arr[1]
                if Core.valid(a) then locked = a end
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
                    best = { actor = e.actor, x = x, y = y, z = z, noun = e.noun,
                             name = e.disp or e.name }
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


-- Release every WORLD-ACTOR handle this loop is holding, on the falling edge of ANY gate.
--
-- Why this is a function and not inline in the world branch (2026-07-29 crash sweep): all three
-- gates share ONE `gated_prev` latch, but this release used to live only inside the world gate's
-- `not gated_prev` edge. So if a muting adapter was already on screen on the tick the world went
-- away — a fishing HUD that lingers ~3 s, a loading pane that holds the adapter ~10 s, an NPC
-- Yes/No that answers straight into a battle — the UI branch above set the latch FIRST, and the
-- world branch's edge block could then never run for the whole gated period. The loop carried
-- `enemy_cache`, `navi_icons` and `chain_wait.actor` across an entire battle or cutscene, which
-- is precisely what destroys those actors. Re-validating per use is NOT a defence: an address the
-- engine has recycled passes both `Mem.alive` and `IsValid` (mem.lua says so in as many words),
-- and `actor_pos` then makes a raw `K2_GetActorLocation` call on it.
--
-- nil, NOT {}: an empty list would be SERVED as "nothing there" — nil is what forces a real
-- FindAllOf on the first call once the world is back.
--
-- Deliberately NOT released here: `mm_cache`. It is a pooled WIDGET, not a world actor, and
-- dropping it would arm the 5 s MM_RETRY FindAllOf backoff on every menu that mutes us.
-- On the module table rather than a `local`: this file is at Lua's 200-local ceiling for a main
-- chunk, and one more would not compile. Not part of the public API — nothing outside calls it.
function Nav.release_world_refs()
    enemy_cache, enemy_next = nil, 0
    navi_icons, navi_next = nil, 0
    -- ONLY the handle is released; the rest of the record (key, grp, label, manual, stateful) is
    -- plain Lua data, so WORLD_DROP_TICKS, remember_pick() and the resume path behave exactly as
    -- before — that design never needed the live pointer to survive the gate, only the metadata
    -- to re-acquire the pick by category+key once the world is back.
    if target then target.actor = nil end
    -- Same defect, same fix: `chain_step` dereferences this the moment the gate reopens, and
    -- nothing below WORLD_DROP_TICKS was clearing it, so a gated period SHORTER than the timer
    -- still handed it a dead pointer.
    if chain_wait then chain_wait.actor = nil end
    -- The shared target snapshot (Nav.targets_cached, added 2026-07-31) is the same hazard in
    -- bulk: a whole LIST of world-actor handles collected on an earlier tick. Dropping it here
    -- is what makes the TTL safe — a served snapshot can then only ever contain handles gathered
    -- inside the SAME uninterrupted free-roam window as the caller asking for it.
    Nav.targets_snap, Nav.targets_snap_at = nil, 0
    -- Both of these are PER-WORLD engine objects that were released only by the map-transition
    -- hook (2026-07-31 audit). A battle, a cutscene or a streaming boundary closes this gate with
    -- no LoadMap, so the transition flush never ran and they survived — and unlike a Lua-side
    -- handle these get marshalled straight INTO reflected engine calls (ProjectPointToNavigation /
    -- FindPathToLocationSynchronously take the mesh by pointer, RegisterNavigationInvoker holds
    -- the system), where no Core.valid on our side can protect the parameter.
    human_mesh = nil
    invoker_key, invoker_nav = nil, nil
end

local function step()
    Mem.mark("nav.step")
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
            -- Release the world-actor handles HERE TOO (2026-07-29). This branch shares
            -- `gated_prev` with the world gate below, so whichever gate closes FIRST owns the
            -- edge — and when a muting adapter is already up as the world goes away, that is
            -- this one. Without this call the world branch's release could never run for the
            -- rest of the gated period. See release_world_refs.
            Nav.release_world_refs()
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
            -- TRANSITION LOG (2026-08-03, diagnostic). This edge runs `release_world_refs`, i.e.
            -- it nils every world handle the tracker holds — `target.actor` included, which then
            -- needs a SUCCESSFUL scan to be repaired. If it fires spuriously while the player is
            -- just walking, the objective is dropped and re-acquired seconds later, and a
            -- re-acquisition RE-ANNOUNCES: the open "the radar untracks by itself and starts
            -- tracking again" report. The gate has three distinct closing conditions and the
            -- fixes so far have been guesses about which one fires, so name it. One line per
            -- transition, never per tick.
            print(string.format("[KakarotAccess] nav world gate -> CLOSED (%s) t=%.2f\n",
                (ui_muted() and "ui muted")
                or (not Core.valid(mm_cache) and "minimap widget gone")
                or "minimap off screen", os.clock()))
            Audio.stop()
            -- DROP THE WORLD-ACTOR CACHES ON THE FALLING EDGE. A field battle or a cutscene
            -- is NOT a Transition: it closes this gate with no LoadMap, so the map-switch
            -- flush above never runs — yet combat destroys exactly the actors these lists
            -- hold. After a SHORT fight (under RESCAN_CLASSES ticks) neither list has expired
            -- and neither gets refreshed, so the first post-battle tick would hand
            -- enemy_alert / best_candidate handles to actors the fight has already freed.
            Nav.release_world_refs()
        end
        route, route_idx = nil, 0
        world_gone = world_gone + 1
        -- No `and target` guard here: in a chained sweep `target` is nil BY CONSTRUCTION
        -- (the arrival path arms chain_wait and calls drop_target in the same breath), so
        -- the old guard could never fire for the one case that matters and chain_wait kept
        -- a handle to the reached actor across the whole battle — dereferenced by
        -- chain_step the moment the gate reopened. drop_target/remember_pick are no-ops
        -- when there is no target, so dropping the condition costs nothing.
        if world_gone >= WORLD_DROP_TICKS then
            remember_pick()   -- a battle stole it; re-acquire when the world is back
            drop_target("world-gone")
            chain_wait = nil  -- the sweep's reached actor may have died with the fight
        end
        return
    end
    if gated_prev then
        gated_prev = false
        -- Paired with the CLOSED line above: the tick count tells a real battle or cutscene
        -- (many ticks) apart from a one-tick flicker of the minimap, which is the difference
        -- between "the world went away" and "we believed it did".
        print(string.format("[KakarotAccess] nav world gate -> open after %d ticks t=%.2f\n",
            world_gone, os.clock()))
        -- RISING EDGE: scan on THIS tick instead of waiting for the next multiple of
        -- SCAN_EVERY. `tick` free-runs while every gate is shut, so that modulo carries an
        -- arbitrary phase across a battle or a load — up to 1.4 s of silence with control
        -- already back. Same defect the manual resume had; on the module table because this
        -- file is at Lua's 200-local ceiling.
        --
        -- A WINDOW, NOT ONE SHOT (2026-08-03, "re-tracking after a battle still takes 2-4 s or
        -- more"). One immediate scan is not enough: right after a fight the quest marker's own
        -- icon is usually not back yet, so that scan finds nothing and every retry then waits a
        -- full SCAN_EVERY — two or three of those IS the reported delay. Retry every nav tick
        -- until something is acquired, bounded. The bound is DERIVED, not picked:
        -- `LOST_SCANS * SCAN_EVERY` is this file's own statement of how long a target may be
        -- unfindable before it is given up on, so it is exactly as long as looking for one is
        -- worth. It ends the moment a target is acquired.
        Nav.auto_until, Nav.auto_t0 = tick + LOST_SCANS * SCAN_EVERY, os.clock()
    end
    world_gone = 0   -- after the edge block: the line above reports how long the gate was shut

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

    -- Field aim announcer: every 3rd tick (~300ms). It reads a reflected actor array each
    -- run, and "aiming at X" landing 0.3s after lock is imperceptible — running it per
    -- tick was free-roam reflection load for nothing (perf audit 2026-07-14).
    if tick % 3 == 0 then aim_watch(pawn, px, py, pz) end

    -- Chained pickup sweep: waiting for the reached collectible to be taken, then
    -- retarget the next one (see chain_step). Enemy proximity warning on the same
    -- sparse cadence as the scans (a per-tick icon sweep would hammer reflection).
    if chain_wait then chain_step(px, py, pz) end
    if tick % SCAN_EVERY == 0 then
        enemy_alert(px, py, pz)
        enemy_probe(px, py, pz)
    end

    -- Re-acquire a hand-picked target stolen by a battle/level change: search the
    -- picker's own enumeration for the remembered category+key — from the SHARED
    -- SNAPSHOT, never a fresh sweep, so this costs nothing on the tick control comes
    -- back and the first attempt no longer waits out a window. While a
    -- resume is pending the quest auto-scan below stays quiet, so the story marker
    -- can't replace the user's pick during the window; if the pick is gone for good
    -- (RESUME_TRIES misses) the auto-scan resumes as before.
    if resume_pick and not target and tick >= (resume_pick.next or 0) then
        -- NEVER sweep inline here (2026-08-03, user report: "after a battle or leaving a menu
        -- the radar takes a while to pick the last target back up"). This called
        -- `Nav.list_targets()` directly — ~17 unbudgeted FindAllOf, ~1.2 s of blocked game
        -- thread — on the first tick after the world came back, i.e. exactly when the player
        -- has just regained control. It also never called `Nav.sweep_partial()`, leaving the
        -- build open (see the self-healing guard there). The shared snapshot with `no_build`
        -- arms explore_tick's CHUNKED builder instead, which runs every nav tick whether or not
        -- explore mode is on, and answers within a tick or two at no cost to this one.
        local snap = Nav.targets_cached(true)
        if snap then
            -- Cadence measured from the LAST ATTEMPT rather than a modulo on a free-running
            -- counter. `tick % (SCAN_EVERY * 3)` put the first attempt at a random phase inside
            -- a 4.5 s window — and its own comment said "~3 s", stale since SCAN_EVERY changed
            -- under it. Now the first attempt lands on the first tick the snapshot is ready and
            -- only the retries are spaced out.
            resume_pick.next = tick + SCAN_EVERY * 3
            resume_pick.tries = resume_pick.tries + 1
            local found = nil
            for _, c in ipairs(snap) do
                if c.key == resume_pick.grp then
                    for _, it in ipairs(c.items) do
                        if it.key == resume_pick.key then found = it break end
                    end
                    break
                end
            end
            if found then
                resume_pick = nil
                Nav.set_manual_target(found.actor, found.key, Nav.item_label(found),
                    found.grp, found.stateful, true)   -- keep_sweep: resume, not a fresh pick
            elseif resume_pick.tries >= RESUME_TRIES then
                resume_pick = nil
            end
        end
    end

    -- Re-scan the game's guidance markers ~1/s (even when idle — scanning every
    -- tick would hammer reflection): picks up a fresh quest marker automatically
    -- (auto-track on accept), retargets when the current step is done, and drops
    -- the target when the game stops guiding. A MANUAL target (companion tracking,
    -- Shift+F5) is never overridden by the auto-scan — nor is a pending resume —
    -- EXCEPT while a fresh-objective signal is armed (notify_objective_change):
    -- the game just issued a NEW objective, and that preempts once, stashing the
    -- interrupted pick so B brings it back.
    local fresh = preempt.scans > 0
    -- Mod config: "radar automático" off → the radar never auto-activates an objective;
    -- only manual R3 picks track. Read live from the settings store (no new local — this
    -- chunk is at Lua's 200-local cap). Absent store = default on.
    local cfg = _G.__KakarotSettings
    local autotrack = (cfg == nil) or cfg.autotrack_enabled()
    -- Expire a re-acquire window that never found anything, so the next one reports its OWN
    -- elapsed time rather than a leftover, and the per-tick retries stop at the bound.
    --
    -- The expiry is LOGGED, and it is the other half of the `nav re-acquired` line: measured
    -- 2026-08-03, a SHORT gate closure (9 ticks) re-acquired in 0.08 s, while a 208-tick one
    -- (a 20 s cutscene) produced no line at all — the window ran out. Those are two different
    -- worlds and only one of them is ours to fix, so the line carries the discriminator:
    -- `icons` is the size of the navi-icon pool. Empty means the game has not rebuilt its HUD
    -- yet and no amount of looking would have helped; non-empty means the icons were back and
    -- none of them resolved to a marker, which WOULD be ours.
    if Nav.auto_until and tick >= Nav.auto_until then
        print(string.format(
            "[KakarotAccess] nav re-acquire window expired after %.2f s (icons=%d, target=%s)\n",
            os.clock() - (Nav.auto_t0 or os.clock()),
            navi_icons and #navi_icons or -1, target and "held" or "none"))
        Nav.auto_until, Nav.auto_t0 = nil, nil
    end
    if autotrack and (tick % SCAN_EVERY == 0
        or (Nav.auto_until and tick < Nav.auto_until)) and (fresh
        or (not resume_pick and not auto_suppressed and not (target and target.manual))) then
        -- The fresh preempt biases once; `preempt.focus` is the standing context and therefore
        -- applies to EVERY auto-scan, which is what carries a multi-phase side story from one
        -- objective to the next without the player opening the picker.
        local best = best_candidate(px, py, pz, fresh and preempt.pri or preempt.focus)
        if best then
            -- Close the re-acquire window and MEASURE it. This line is the whole point of the
            -- window being a hypothesis rather than a hunch: it says how long the marker
            -- actually took to come back after a battle, so "still slow" can be answered with a
            -- number instead of a fourth guess. Printed only when a window was open, so it is
            -- one line per battle/objective change, never per tick.
            if Nav.auto_until then
                print(string.format("[KakarotAccess] nav re-acquired after %.2f s key=%s\n",
                    os.clock() - (Nav.auto_t0 or os.clock()), tostring(best.key)))
                Nav.auto_until, Nav.auto_t0 = nil, nil
            end
            if fresh then
                preempt.scans, preempt.pri = 0, nil   -- consumed, whatever happens below
                if not target or target.key ~= best.key then
                    preempt.stash_now()
                    auto_suppressed = false
                    resume_pick = nil
                end
            end
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
                -- RE-ADOPT THE FRESH HANDLE (2026-08-03, "the radar is still slow to start
                -- after a battle"). `release_world_refs` nils `target.actor` on every
                -- world-gate close — a battle, a cutscene — deliberately, because the actor
                -- may not survive it, while keeping the rest of the record so nothing has to
                -- be re-announced. But this branch is the "same objective as before" case and
                -- it updated NOTHING, so the one thing the record was missing was the one
                -- thing the sweep had just produced: the radar came back from a battle holding
                -- a nil actor, and could not guide to it. It could not recover either —
                -- `target_missing` only counts up when the sweep finds NOTHING, and here it
                -- finds the objective every time.
                -- Silent by design: this is the "merely re-resolving the same spot" case the
                -- announce above already declines to speak. The route is dropped with it
                -- because it was pathed against the handle that just died.
                if target.actor == nil and best.actor ~= nil then
                    target.actor = best.actor
                    route, route_idx = nil, 0
                end
            end
        elseif fresh then
            -- No marker yet (it can lag the HUD text) — burn one try, touch nothing.
            preempt.scans = preempt.scans - 1
            if preempt.scans == 0 then preempt.pri = nil end
        elseif target and not target.manual then
            -- ASK ABOUT THE TARGET WE HOLD, NOT ABOUT THE SWEEP'S YIELD (2026-08-03). Settled by
            -- the diagnostic added the same day, which named it in one line:
            --   `nav drop target (lost-scans) key=… actor=live missing=3 quiet=false`
            -- The world gate was open, the scan WAS allowed, and the tracked actor was LIVE —
            -- yet the objective was dropped and then re-acquired, which re-announces. That is
            -- the whole of "the radar untracks by itself while I walk".
            --
            -- `best_candidate` returns nil only when BOTH its source loops yield nothing, and
            -- every way that happens is a per-tick transient: the navi icon fails `icon_in_use`
            -- because one of its sub-widgets is momentarily not on screen, its `TargetActor`
            -- pointer is unreadable for a tick, or `actor_pos` raises during a transform update.
            -- The minimap-icon fallback loop only runs when no navi icon exists, so it is not a
            -- safety net here. Three such blips inside 4.5 s is not rare.
            --
            -- The counter's real question is "has my objective gone away?", and an empty
            -- candidate sweep is weak evidence for that while we are holding a live handle to
            -- the thing itself. So ask the handle. The genuine case this must still catch — the
            -- objective completes and the game destroys its marker — makes the actor invalid or
            -- unpositionable, and still counts exactly as before. Ditto the post-gate-close case
            -- where the handle was nil'd: nothing to ask, so it counts and LOST_SCANS hands the
            -- objective back to ordinary acquisition. The previous cut gated this on
            -- `Core.scan_quiet()` instead, which is now redundant: whether we bothered to LOOK
            -- for candidates says nothing either way about the actor we already have.
            if target.actor ~= nil and Core.valid(target.actor)
                and actor_pos(target.actor) then
                target_missing = 0
            else
                -- The handle is gone or unreadable. Note the `target.actor == nil` half is not
                -- optional: leaving it out was a REGRESSION the same day ("after the battle it
                -- never tracked again, I had to restart tracking by hand").
                -- `release_world_refs` nils the handle at every world-gate close, and BOTH paths
                -- that restore it need a candidate — re-adoption when the key matches, a fresh
                -- target when it does not — so a target that is never counted and never dropped
                -- is stuck for good, with a manual re-pick the only way out. Counting here is
                -- what hands it back to ordinary acquisition, and the coast on `target.lx`
                -- keeps the beacon alive through that window exactly as designed.
                target_missing = target_missing + 1
                if target_missing >= LOST_SCANS then drop_target("lost-scans") end
            end
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
        -- A nil handle is NOT the same event as a dead one. nil means the world gate
        -- released the pointer while it was closed (see the falling edge above), so all we
        -- know is that the pointer stopped being trustworthy — not that the actor is gone.
        -- Treating that as "collected / despawned" would mark a swept item visited and skip
        -- it for the rest of the sweep, and would drop a hand-picked target without arming
        -- the resume. So: stash the pick as plain data and drop, exactly like the
        -- WORLD_DROP_TICKS path — the resume scan re-acquires it by category+key, and
        -- chain_seen is left alone. (Only reachable when the fight was shorter than
        -- WORLD_DROP_TICKS; past that the gate has already dropped the target itself.)
        if target.actor == nil then
            remember_pick()
            drop_target()
        elseif sweeping then chain_over() else drop_target() end
        return
    end
    -- A MISSED SCAN IS THE ONE MOMENT NOT TO TOUCH THE HANDLE. `target_missing > 0` means the
    -- last scan swept the world and did NOT find this actor, which is the strongest evidence
    -- available that it is being (or has been) freed — and `Core.valid` cannot tell us, because
    -- `Mem.alive` only rejects unmapped garbage and a recycled address passes every check
    -- (ledger 2026-07-26 (c): the pre-check logged ZERO rejections all session). So for the
    -- ≤ LOST_SCANS window before the target is dropped we coast on the last position we
    -- successfully read instead of dereferencing a handle we have reason to distrust. The
    -- beacon keeps speaking from cached coordinates for a few seconds rather than going quiet,
    -- and 1647 still drops the target when the misses reach LOST_SCANS.
    local coasting = target_missing > 0 and target.lx ~= nil
    if sweeping and not coasting then
        local hidden, st = false, nil
        -- target.actor is a handle picked minutes ago; the actor can be freed under it.
        pcall(function() hidden = Core.member(target.actor, "bHidden") end)
        if target.stateful then
            pcall(function() st = tonumber(Core.member(target.actor, "InteractState")) end)
        end
        if (hidden == true or hidden == 1) or st == STATE_TAKEN then
            chain_over()
            return
        end
    end
    local tx, ty, tz
    if coasting then
        tx, ty, tz = target.lx, target.ly, target.lz
    else
        tx, ty, tz = actor_pos(target.actor)
        -- Remember every position we DID read: this is what makes coasting possible, and it is
        -- plain numbers, never a handle — the metadata survives a world change, the pointer
        -- never does (ledger 2026-07-27, rule 3).
        if tx then target.lx, target.ly, target.lz = tx, ty, tz end
    end
    if not tx then
        if sweeping then chain_over() else drop_target() end
        return
    end

    -- Arrival on TRUE 3D distance to the objective (not the route corner).
    -- Manual picks use the tight interact radius — except GATHERING points: the
    -- fruit/small-fish spots are spawner VOLUMES (ASpawner*Volume, CXX dump
    -- 2026-07-17) whose actor origin can sit meters away from the orbs the player
    -- actually collects, so the tight radius never fired, the sweep never armed, and
    -- the radar never advanced to the next spot (user report 2026-07-17). The wide
    -- auto radius arms the sweep from anywhere inside the patch; the walk-away
    -- trigger (chain_step) then advances it.
    local d3 = math.sqrt((tx - px) ^ 2 + (ty - py) ^ 2 + (tz - pz) ^ 2)
    local arrive_r = (target.manual and target.grp ~= "gathering")
        and ARRIVE_DIST_MANUAL or ARRIVE_DIST
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
    -- OUR geometric reticle: while tracking an enemy OR a hunt target, the moment the
    -- camera yaw centers on it (|pan| within the reticle slack) inside Ki-blast
    -- range, say "on target" (interrupting — the shot window is short); re-arms
    -- when the aim drifts off. pan/fb here are already camera-relative.
    if target.grp == "enemies" or target.grp == "hunt" then
        local on_target = fb > 0 and math.abs(pan) <= AIM_ALIGN_PAN and d3 <= AIM_RANGE
        if on_target and not was_on_target then
            Speech.say(I18n.t("nav_on_target"), true)
        end
        was_on_target = on_target
    end

    -- Stealth aid for hunted animals / field enemies (user request 2026-07-06, the
    -- deer hunt: "approach from behind so they don't see you"): while tracking a
    -- target picked from the enemies OR hunt category, speak which side of ITS facing the
    -- player is on whenever that zone changes — "behind" = safe approach, "in front"
    -- = it can see you. K2_GetActorRotation is the reflected sibling of the
    -- K2_GetActorLocation call the whole tracker already relies on.
    if (target.grp == "enemies" or target.grp == "hunt") and not coasting then
        local rot
        -- Method call on the long-lived target handle: re-check the actor first. Skipped while
        -- coasting for the same reason as the position read above — an enemy that a scan just
        -- stopped finding is an enemy that combat probably destroyed, and this is a UFunction
        -- call on it, which no pcall can make safe.
        pcall(function()
            if Core.valid(target.actor) then rot = target.actor:K2_GetActorRotation() end
        end)
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
    -- Never during a load/cutscene (added 2026-07-29 — crash sweep). This is the single most
    -- expensive thing the mod does: `Nav.list_targets` issues SEVENTEEN raw `FindAllOf` calls,
    -- none of which takes a scan slot or routes through `timed_findall`, and at this repo's own
    -- measured 68.2 ms per scan that is roughly 1.2 s of game-thread work in ONE tick — fired
    -- every 4 s, indefinitely, while the player runs or flies with explore mode on. Quiet mode is
    -- the game's own "something else owns the screen" signal, which is exactly when a freeze is
    -- least affordable. Deliberately NOT budget-gated with `take_scan_slot`: `list_targets` is
    -- shared with the R3 picker, and this loop only refills the PROPERTY budget, so a scan-budget
    -- gate here would silently hand the picker a near-empty list whenever the registry loop is
    -- stopped — the fail-closed-on-shared-substrate trap. STILL OPEN: the 1.2 s burst itself.
    -- It needs a cheaper source (the minimap pointer walk + the already-cached enemies list)
    -- rather than the full 17-class sweep; that changes what explore mode can find, so it wants
    -- a measurement (Ctrl+F5 nav avg/max with explore on) and a deliberate decision first.
    if Core.scan_quiet() then return end
    -- COMMIT THE ATTEMPT BEFORE DOING THE WORK. These three used to be written at the END,
    -- which made a fault inside the sweep unbounded: `explore_tick`'s gate treats
    -- `explore_sx == nil` as "rescan unconditionally" (there is no distance or time floor to
    -- fall back on), so a catchable Lua error anywhere in `Nav.list_targets` — swallowed by the
    -- `pcall(explore_tick)` in `Nav.start` — left the state untouched and the whole ~1.2 s
    -- 17-scan sweep restarted on EVERY 100 ms nav tick, for the rest of the session. That is a
    -- saturated game thread with no frames and no input: the player's only way out is killing
    -- the process. Committing first turns that into one stale cycle and a retry in
    -- EXPLORE_RESCAN_MS. `explore_pois` deliberately stays at its PREVIOUS value on a fault, so
    -- the cues keep serving the last good list instead of going silent.
    -- This is the playbook's "commit the edge before any early return" rule (learned on the map
    -- d-pad handler, 2026-07-28) — the same shape, reached through a fault path instead of a
    -- bail-out.
    explore_sx, explore_sy, explore_sz = px, py, pz
    explore_scan_ms = ms
    local t0 = os.clock()
    local pois = {}
    -- The shared snapshot too (crash audit RANK 3, 2026-07-31). Behaviour here is unchanged in the
    -- normal case — this function is already rate-limited to EXPLORE_RESCAN_MS and the TTL is that
    -- same constant, so when it is due the snapshot is due as well and it sweeps. What it buys is
    -- the other direction: if the R3 picker swept a moment ago, this reuses that instead of paying
    -- a second ~1.2 s sweep for the same answer.
    for _, cat in ipairs(Nav.targets_cached()) do
        for _, it in ipairs(cat.items or {}) do
            local x, y, z = actor_pos(it.actor)
            if x then
                pois[#pois + 1] = { key = it.key, grp = it.grp,
                                    label = Nav.item_label(it), x = x, y = y, z = z }
            end
        end
    end
    explore_pois = pois
    -- Cost telemetry. The 17-scan burst was INVISIBLE in the log for its whole life: it takes no
    -- scan slot and never routes through `Core.timed_findall`, so `__KakarotScanStats` cannot see
    -- it either, and the only reason we know it is ~0.4-1.2 s is a crash-trail gap measured after
    -- the fact (2026-07-29: 438 ms between the `nav.explore` mark and the next mark). Logged ONCE
    -- per session, on the worst sweep so far over the threshold, so a player's log carries the
    -- number without ever logging per tick.
    -- Threshold is TICK_MS, not a picked number: a sweep longer than one whole nav tick has by
    -- definition eaten the tick it ran in plus at least one more, which is the thing worth
    -- reporting. (No new file-scope local: this file sits at Lua's 200-local ceiling, which is
    -- also why `Nav.explore_sweep_max` lives on the module table.)
    local dt = (os.clock() - t0) * 1000
    if dt > TICK_MS and dt > (Nav.explore_sweep_max or 0) then
        Nav.explore_sweep_max = dt
        print(string.format("[KakarotAccess] explore sweep blocked the game thread %.0f ms (%d POIs)\n",
            dt, #pois))
    end
end

local function explore_tick()
    -- Marked separately from nav.step: this runs in the SAME game-thread callback right after
    -- step(), so without its own mark a trail ending in `nav.step` could not tell the two apart
    -- (noted while root-causing the 2026-07-26 (c) crash). It reaches the same target sweep.
    Mem.mark("nav.explore")
    if not Nav.field_ready() then return end
    -- DEFERRED TARGET BUILD for the R3 picker (2026-07-31). radar_menu asks for the snapshot with
    -- no_build on the 20 ms pad dispatch and is answered nil when it is cold; the sweep is paid
    -- HERE, on the 100 ms nav loop, which is where every other list_targets call already lives.
    -- Ordered ABOVE the explore_on gate on purpose: the picker is available whether or not explore
    -- mode is on, so this must not depend on it. Measured into the same telemetry as
    -- explore_rescan, which is what makes Nav.targets_build_ms a derivation rather than a guess.
    if Nav.targets_want then
        local t0 = os.clock()
        -- BOXED (2026-08-03): this build may stop when it has spent its slice of the tick and
        -- continue on the next one. Keep `targets_want` set until it completes, and publish
        -- nothing before then — a partial list would open the picker on a short set of targets.
        local list = Nav.list_targets(true)
        local dt = (os.clock() - t0) * 1000
        if Nav.sweep_partial() then
            Nav.explore_sweep_chunks = (Nav.explore_sweep_chunks or 0) + 1
        else
            Nav.targets_want = nil
            Nav.targets_snap, Nav.targets_snap_at = list, os.clock()
            -- Chunks are counted so targets_build_ms can still bound the WHOLE build rather than
            -- one slice of it; a pad caller that waited only one slice would give up too early.
            Nav.explore_sweep_last_chunks = (Nav.explore_sweep_chunks or 0) + 1
            Nav.explore_sweep_chunks = 0
        end
        if dt > TICK_MS and dt > (Nav.explore_sweep_max or 0) then
            Nav.explore_sweep_max = dt
            print(string.format("[KakarotAccess] picker sweep blocked the game thread %.0f ms\n", dt))
        end
    end
    if not explore_on then return end
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

-- ---- loop management (a stepper on the shared tick bus — see pad_poll.lua) ------------

function Nav.start()
    if running then return end
    running = true
    require("pad_poll").register_every("nav", TICK_MS, function()
        -- This subsystem is the only one that touches UObjects without going through
        -- Core.begin_scan_tick, so it polls the world epoch itself — otherwise, with
        -- the menu reader toggled off (Ctrl+M retires the registry stepper), the radar
        -- would keep walking cached actors with the transition gate never arming.
        Core.poll_world()
        -- Cost telemetry (like the registry step's stats): this runs OUTSIDE the
        -- registry step, so its game-thread cost was invisible to the Ctrl+F5 numbers
        -- until the 2026-07-16 mods.txt A/B forced the question "what else is
        -- unmeasured?".
        local t0 = os.clock()
        local ok, err = pcall(step)
        if not ok then print("[KakarotAccess] nav step error: " .. tostring(err) .. "\n") end
        -- Passive explore radar runs in its OWN pcall so an error here can never
        -- take down the beacon tick (or vice versa).
        local ok2, err2 = pcall(explore_tick)
        if not ok2 then print("[KakarotAccess] nav explore error: " .. tostring(err2) .. "\n") end
        local dt = (os.clock() - t0) * 1000
        local st = _G.__KakarotNavStats
        if not st then st = { n = 0, ms = 0, max = 0 } _G.__KakarotNavStats = st end
        st.n = st.n + 1
        st.ms = st.ms + dt
        if dt > st.max then st.max = dt end
    end)
end

function Nav.stop()
    running = false
    require("pad_poll").unregister("nav")
    Audio.stop()
end

-- ---- user commands --------------------------------------------------------------------

-- F3: master toggle. Off = immediate silence; on = re-acquire and announce.
--
-- SAME WRAPPER AND SAME REASON AS Nav.toggle_route BELOW (crash audit RANK 1, 2026-07-31).
-- A RegisterKeyBind callback runs on UE4SS's KEYBOARD thread, and this body is not a flag flip:
-- `drop_target()` rewrites the very module upvalues (target / route / chain_wait / resume_pick)
-- that the 100 ms step() reads, and `Speech.say` does a read-modify-write of speech.lua's shared
-- `pending` queue — concurrently, on the SAME lua_State as the poll loop. That is the allocator +
-- incremental-GC race that kills the process minutes later somewhere unrelated. The 2026-07-27
-- keybind sweep missed this one for the same reason it missed toggle_route: it is delegated
-- through app.lua (App.nav_toggle) rather than bound directly, so it did not look like a handler.
-- F3 is the advertised radar master switch, so a player presses it constantly.
function Nav.toggle()
    ExecuteInGameThread(function()
    on = not on
    if not on then
        Audio.stop()
        drop_target()
        chain_wait, chain_seen = nil, {}
        resume_pick = nil
        preempt.stash = nil
        preempt.scans, preempt.pri = 0, nil
        -- F3 off is the only full reset of the quest focus. A manual pick or a B deliberately
        -- KEEP it: the auto-scan is suspended while a hand-picked target is live, so the focus
        -- costs nothing there, and when the player releases the pick they should land back on the
        -- side story they were working — not on the main quest.
        preempt.focus = nil
        companion_idx = 0
        Speech.say(I18n.t("nav_off"), true)
    else
        gated_prev = false
        auto_suppressed = false   -- F3 on resumes auto quest tracking after a stop/arrival
        Speech.say(I18n.t("nav_on"), true)
    end
    end)
    -- No return value: the wrapper defers the body, so `on` is not yet decided here. App.nav_toggle
    -- discards it anyway — exactly as App.nav_route_toggle already does for Nav.toggle_route.
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
    for _, c in pairs(Core.findall("AT_Character")) do
        -- POSITIVE test: a companion IS an AT_CharacterPlayableBase. This used to be the
        -- negative `not ENEMY_NOUN_BY_SPAWN[enemy_spawn_type(c) or 0]`, and that filter is
        -- BLIND TO ROAMING ENEMIES: the table only has entries for SpawnType 1/2/3 (the
        -- enemy scan says as much — it needs `or "cat_enemy"` for the rest), and a free-roaming
        -- mob is SpawnType 0, so `ENEMY_NOUN_BY_SPAWN[0]` is nil and every one of them passed
        -- as a party member. User report 2026-08-18: the Namek alien mobs were listed under
        -- Compañeros, and Shift+F5 would target one as if it were Gohan.
        --
        -- `is_playable_char` is the SAME helper the enemy scan already uses to exclude the
        -- party (line ~1283), so the two scans now share one definition of "party member" and
        -- cannot disagree. It also covers the player, who is dropped by me_key just below.
        if Core.valid(c) and tostring(c:GetAddress()) ~= me_key
            and is_playable_char(c)
            and char_visible(c) then
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
-- MUST run on the game thread (fixed 2026-07-29 — crash sweep). UE4SS runs keybind handlers on
-- its own UpdateThread, and this one is not a pure flag flip: `clear_invoker` does a
-- `FindFirstOf("PlayerController")` (a GUObjectArray walk), `Core.valid`/`Core.member` (which
-- MUTATE the shared valid_memo / prop_sets / prop_budget tables the 100 ms step() is reading in
-- the same instant), and finally `UnregisterNavigationInvoker` — a ProcessEvent into the live
-- navigation system. All of that on the SAME lua_State as the poll loop, which is the allocator +
-- GC race that kills the process minutes later somewhere unrelated. Same wrapper as Nav.where and
-- Nav.cycle_companion. The 2026-07-27 keybind sweep found "only 1 of ~19 handlers wrong" and
-- missed this one, because it is delegated through app.lua rather than bound directly.
function Nav.toggle_route()
    ExecuteInGameThread(function()
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
    end)
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

-- ---- R3 target picker support (radar_menu.lua) --------------------------------------
-- All world-actor reads for the picker live HERE, behind the same safety gates and
-- caches as the radar, so radar_menu.lua stays pure input + UI.

-- Free-roam gate for the picker: TRUE only in the RPG overworld. ui_muted (a pausing
-- menu/dialog owns the screen) OR the world gate closed (minimap hidden = BATTLE,
-- cutscene, loading, full-screen menu) both return false — so the R3 menu can NEVER
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
-- dumps/dump_radar.txt each time the R3 menu opens — so "only fishing + quests show"
-- can be diagnosed offline (is a category ABSENT from the minimap, or being FILTERED?).
local RADAR_DEBUG = false  -- heavy diagnostic: FindAllOf probes + UniqueId/InteractComponent
                          -- reflection. Reads UniqueId on arbitrary actors, which can raise
                          -- the uncatchable C++ abort — leave OFF in normal play (turning it
                          -- on stranded do_open and R3 stopped opening, 2026-07-04).
local NAVI_DEBUG = false   -- light + SAFE navi main/sub diagnostic. CONFIRMED 2026-07-04:
                          -- switcher index 1→MAIN(24), 2→SUB(26); actorEMapIcon was always
                          -- nil (why it used to default to main). navi_quest_icon is correct.
                          -- Only safe reads (switcher index + component type); no UniqueId.
local NPC_ID_DUMP = false  -- SAFE, TARGETED: reads ONLY UniqueId (the one member proven
                          -- safe on the QuestCharacter classes — see npc_name) for the
                          -- present NPCs, to dumps/dump_npc_ids.txt each time the R3 picker
                          -- opens. For mapping real names from real ids (never guessed).
                          -- Ids already captured 2026-07-10 (see dumps); left off.
-- NOTE: do NOT call MessageManager GetNounParamFromName / GetNounParam / GetSpeaker with a
-- raw id — it caused an EXCEPTION_ACCESS_VIOLATION (deref 0x70) that crashed the game
-- (2026-07-10). Those reflected calls are the uncatchable-abort family on this build (bad
-- param marshalling), like the LineTrace/ProjectPoint out-param calls. Character names must
-- come from the hand-verified CPL_NAMES/NPC_NAMES maps below, not a live resolver.

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
    -- Cpl019 = Zarbon. PROVEN live 2026-08-18, not inferred: entering the Namek boss fight spawned
    -- `AT_Character_cpl019_A_BP_C`, whose CharacterType read 35 = `Zarbon_A_C01`. The enemy class
    -- name carries the cpl id and the enum carries the real name, so one actor pairs them outright.
    -- This is the ONLY way an entry here should be earned — interpolating from the enum's order put
    -- Zarbon at cpl017, and it was wrong, because the runs have gaps (see the notes file).
    Cpl019 = "Zarbon",
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
        name_lib = (ok and Core.valid(lib)) and lib or false
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

-- Resolve a character id ("Cpl059c02", "Cpl013") to a display name: the game's own
-- GetCharacterName resolver first, retried without a trailing variation suffix ("...c02")
-- when the full id has no entry, then the hand-verified CPL_NAMES fallback. nil = the game
-- has no name for it (the caller picks the fallback — a generic noun, never the raw code).
-- Shared by field enemies (CharacterName) and talkable NPCs (UniqueId): both use the same
-- "CplNNN" id scheme, so the resolution logic must not drift between the two.
local function resolve_char_id(raw)
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

-- Real display name for a field ENEMY: AAT_Character declares CharacterName (reflected
-- FString @0x9E8, CXX dump) — the character id (e.g. "Cpl059c02"). nil = no name (callers
-- fall back to the generic enemy noun). ONLY call on AT_Character actors: CharacterName is
-- not declared elsewhere and reading it would be the uncatchable abort.
enemy_display_name = function(c)
    -- SPEAKER ID FIRST (2026-08-18) — the game's own localized name, and it covers everyone.
    -- `speakerID` is the UniqueId plus a variant letter (Cpl019 -> Cpl019A), and fed THAT,
    -- GetCharacterName answers where it returns "" for the bare code. See char_types.speaker_id
    -- for the live evidence. This outranks every other source here: it is the string the GAME
    -- shows, in the PLAYER'S language, so it needs no table of ours and never drifts from the
    -- game's own wording. Everything below stays as the fallback chain for ids it has no row for.
    local ctok, CT = pcall(require, "char_types")
    if ctok and CT then
        local sid = CT.speaker_id(Core, c)
        local nm = sid and game_character_name(sid)
        if nm then return nm end
        -- `speakerID` is EMPTY on this branch (measured live 2026-08-18), so the id has to come
        -- from the generated class name instead — see char_types.speaker_ids_from_class. Without
        -- this the Namek mobs stayed "alien" from the enum while the game's own table had
        -- "Oficial del Ejercito de Freezer" waiting under Cpl064A.
        for _, id in ipairs(CT.speaker_ids_from_class(Core, c) or {}) do
            nm = game_character_name(id)
            if nm then return nm end
        end
    end
    -- CharacterType THIRD (2026-08-15). It is a reflected ENUM on AAT_CharacterBase whose value
    -- names are the characters themselves, so it names ~107 of them outright — against the four
    -- that CPL_NAMES holds and the zero that the game's own GetCharacterName ever returns. An
    -- enum is a value read: cheaper and safer than the FString hop below. `require` is a
    -- package.loaded lookup, not a load, and this file is at Lua's 200-local ceiling so the
    -- module cannot be held in a new upvalue.
    -- pcall'd: a bare require RAISES if the module is missing or has a syntax error, and that
    -- would take out naming for every enemy instead of quietly falling through to the old path.
    local ct_name, ct_code
    if ctok and CT then ct_name, ct_code = CT.of_actor(Core, c) end
    if ct_name then return ct_name end
    local raw
    pcall(function()
        -- Gated fetch + a validity check on the RESULT. This runs on post-combat AT_Characters,
        -- i.e. actors the engine is in the middle of destroying.
        raw = Core.name_str(Core.member(c, "CharacterName"))
    end)
    -- ct_code is the enum's raw CplNNN for the unnamed tail (DLC/late content): a second id to
    -- try against the hand-verified map when CharacterName itself was unreadable.
    return resolve_char_id(raw) or (ct_code and resolve_char_id(ct_code)) or nil
end

-- Best spoken name for a field NPC from its UniqueId (FName). nil = fall back to the
-- generic "character" noun (anonymous townsfolk — the game has no real name for them).
-- IMPORTANT: read ONLY UniqueId — it's a reflected member. Do NOT touch CharacterName:
-- QuestCharacterBase does NOT declare it, and reading a non-existent property on this
-- game raises a C++ exception pcall CANNOT catch — which froze the game (it aborted
-- right after the menu had blocked the pad, leaving a stuck neutral pad).
local function npc_name(npc)
    -- SPEAKER ID FIRST (2026-08-18) — the game's own localized name, and it covers everyone.
    -- `speakerID` is the UniqueId plus a variant letter (Cpl019 -> Cpl019A), and fed THAT,
    -- GetCharacterName answers where it returns "" for the bare code. See char_types.speaker_id
    -- for the live evidence. This outranks every other source here: it is the string the GAME
    -- shows, in the PLAYER'S language, so it needs no table of ours and never drifts from the
    -- game's own wording. Everything below stays as the fallback chain for ids it has no row for.
    local ctok, CT = pcall(require, "char_types")
    if ctok and CT then
        local sid = CT.speaker_id(Core, npc)
        local nm = sid and game_character_name(sid)
        if nm then return nm end
    end
    -- NO CharacterType HERE (2026-08-18, measured in the live game). It is declared on
    -- AAT_CharacterBase, so QuestCharacter really does inherit it — but nothing on this branch
    -- ever AUTHORS it, and the Blueprint default is 1, which is Goku. Read live: the actor
    -- `Itm098c01` of class QuestCharacterBase_C — an ITEM, UniqueId "None" — answered
    -- CharacterType = 1. QuestCharacterBase_C is the GENERIC class the game reuses for NPCs,
    -- items and quest markers alike, so asking it for a character name renames every field NPC
    -- on the radar to Goku. That is exactly what the player heard: Dodoria at 300 m announced as
    -- Goku. The enemy branch is different and keeps the enum — its actors are character-specific
    -- classes (AT_Character_cpl004p1c02_BP_C answered 11 = Apuru, a real authored value).
    -- A wrong name is worse than no name, so this branch stays on UniqueId alone.
    local raw
    pcall(function()
        raw = Core.name_str(Core.member(npc, "UniqueId"))
    end)
    if type(raw) ~= "string" or raw == "" or raw == "None" then return nil end
    -- The game's own resolver (+ hand-verified map) first — authoritative names.
    local name = resolve_char_id(raw)
    if name then return name end
    -- descriptive story ids: word-match known characters
    local key = raw:lower():gsub("[^%a]", "")
    for tok, nm in pairs(NPC_NAMES) do
        if key:find(tok, 1, true) then return nm end
    end
    -- Anonymous "Npc0NN" townsfolk: no useful name → generic "character" noun (nil).
    -- Story-character codes (Cpl013…) ARE spoken as the raw code so they stay
    -- distinguishable until a verified CPL_NAMES/NPC_NAMES entry gives the real name
    -- (user choice 2026-07-11: keep codes for now — the game exposes no safe resolver,
    -- see the npc-names note). No live name lookup here: it crashes on this build.
    if raw:match("^[Nn]pc%d") then return nil end
    -- some other descriptive id (or a Cpl code): clean it and speak it raw
    local cleaned = raw:gsub("^[%a]-_", ""):gsub("_", " ")
    return cleaned ~= "" and cleaned or nil
end

-- TTL-shared view of list_targets, for the two callers that must not pay for it on demand
-- (crash audit RANK 3, 2026-07-31). list_targets issues ~17 raw FindAllOf calls — the file's own
-- measurement is ~68 ms each, ~1.2 s for the sweep — and it takes no scan slot. It DOES route
-- through the meter since 2026-08-03 (`Core.findall` in `Nav.SW.class_list`), so the Ctrl+F5 dump
-- and the ghost ledger now see it; the budget exemption stands. Two callers turned that into a
-- felt freeze: explore mode re-sweeps every few seconds while the player is simply walking around,
-- and radar_menu.do_open runs it SYNCHRONOUSLY on the shared 20 ms pad dispatch, so opening the R3
-- picker in a dense area locks the game on the keypress. They now share one snapshot, so whichever
-- of them swept last pays and the other is free.
--
-- The TTL is DERIVED from EXPLORE_RESCAN_MS, not picked: that constant is already this mod's own
-- statement of how stale a target list may be before it must be re-swept, so reusing it keeps one
-- definition instead of inventing a second. Written inline rather than as a named local on
-- purpose — this file sits at Lua's 200-locals-per-function ceiling (settings.lua's `_G` handoff
-- comment records the same constraint), and one more declaration here fails to COMPILE.
--
-- SAFETY — this holds live world-actor handles in bulk. It is dropped by release_world_refs and by
-- the map-transition hook, so a snapshot can only ever be served to a caller inside the SAME
-- uninterrupted free-roam window it was gathered in. Callers still validity-check every actor, as
-- they always did with a fresh list.
-- no_build: the caller is on the shared 20 ms PAD dispatch, where the sweep behind list_targets
-- must never run. Answer nil and arm the nav loop to build it instead (see explore_tick).
--
-- THE STREAMER'S CRASH, 2026-07-31. The trail decoded to a death inside `pad.tick`, ~11 minutes
-- into ordinary free roam. The only pad path that enters the engine with nothing pressed on that
-- tick is radar_menu's deferred single-tap open -> do_open -> here; explore mode toggles on an R3
-- DOUBLE-tap, so a player using explore taps R3 constantly and every uncompleted tap falls through
-- to it. Serving it from this function was already half the fix; the other half is that a cold or
-- expired snapshot still fell through to the inline sweep below — 17 unbudgeted FindAllOf, ~1.2 s
-- of blocked game thread, dereferencing every candidate — ON THAT 20 ms DISPATCH. pad_poll wraps
-- each stepper in pcall, which buys nothing here: this engine's aborts are uncatchable.
function Nav.targets_cached(no_build)
    local t = os.clock()
    if Nav.targets_snap and (t - (Nav.targets_snap_at or 0)) < EXPLORE_RESCAN_MS / 1000 then
        return Nav.targets_snap
    end
    if no_build then
        Nav.targets_want = true
        return nil
    end
    local list = Nav.list_targets()   -- NOT boxed: a synchronous caller cannot use a partial list
    Nav.sweep_partial()               -- closes the build (always false on the unboxed path)
    Nav.targets_snap, Nav.targets_snap_at = list, t
    return list
end

-- How long a pad-side caller should wait for a deferred build before giving up and opening with
-- whatever it has. DERIVED, not picked: one nav tick to notice the request, plus the worst sweep
-- actually MEASURED this session (explore_sweep_max, fed by both sweep sites). Floored at one more
-- nav tick so a session that has never swept still waits a whole tick.
function Nav.targets_build_ms()
    -- CHUNK-AWARE (2026-08-03): a boxed build now spans several nav ticks, so this must cover the
    -- WHOLE build and not one slice of it, or the pad caller gives up early and opens the picker
    -- on a stale set. `explore_sweep_last_chunks` is what the last COMPLETED build actually took —
    -- measured like everything else here — floored at 1 so a session that has never swept behaves
    -- exactly as it did before.
    local chunks = math.max(Nav.explore_sweep_last_chunks or 1, 1)
    return TICK_MS + chunks * math.max(Nav.explore_sweep_max or 0, TICK_MS)
end

-- The longest a pad caller should EVER wait for a build that is still making progress, as
-- opposed to the estimate above of how long it should take. The two are different questions
-- and conflating them is what made the picker say "nothing to track" on the first R3 of a
-- session while the main quest was actively guiding (user, 2026-08-03): `targets_build_ms`
-- derives from telemetry that BY DEFINITION does not exist yet the first time it is called —
-- no build has completed, so `explore_sweep_last_chunks` and `explore_sweep_max` both floor
-- and it answers 200 ms — while a cold chunked build genuinely spans ~15 classes at ~65 ms,
-- i.e. the better part of a second. The caller gave up at 200 ms, `Nav.targets_snap` is only
-- published when the WHOLE build completes, so it opened on an empty list.
--
-- DERIVED, not picked: EXPLORE_RESCAN_MS is this file's own statement of how stale a target
-- list may be before it must be re-swept, so it is equally the longest a FRESH one can be
-- worth waiting for. Past it the nav loop is gated (a menu, a cutscene, a load) and waiting
-- longer cannot help.
function Nav.targets_wait_cap_ms()
    return EXPLORE_RESCAN_MS
end

-- ---- the sweep's scan indirection (2026-08-03) --------------------------------------------
--
-- ONE place where every class list the sweep needs is obtained, so two separate problems each get
-- their fix without touching the ten straight-line blocks below.
--
-- (1) MANAGERS INSTEAD OF SCANS. The first in-gameplay dump of this game (2026-08-03; every
--     earlier one was taken at the title screen, which is why nobody had seen this) showed the
--     GameMode carries a component per subsystem, each already holding the very list this sweep
--     was walking the whole object table to rebuild. All the members below are REAL TArrays —
--     checked twice, header layout AND live `ArrayProperty` reflection — because a fixed C array
--     would collapse to element 0 and `GetArrayNum` on it is the uncatchable abort. Acquisition is
--     FindAllOf-once-and-cache, exactly the pattern `ui_directory` already uses for MenuManager:
--     no new UE4SS surface, and each manager is a confirmed singleton under the GameMode.
--
-- (2) A TIME BOX. A `FindAllOf` has unbounded DURATION and this mod has only ever bounded its
--     FREQUENCY. `ui_core.lua:1185-1191` already records what the cost of that looks like — "the
--     second freeze (mid-session, no crash, log just stops) matches a game-thread deadlock against
--     the async loader" — and it was fixed for `StaticFindObject` only, while `FindAllOf` walks
--     the same tables. A 578 ms `nav.explore` stall sat one second before the 2026-08-03 freeze.
--     So a build now STOPS once it has spent its budget and RESUMES on the next nav tick, reusing
--     what it already scanned. Same total work; never again in one unbroken block.
--
-- The equivalence of a manager list to the scan it replaces is ASSERTED BY THE ENGINE, not by us,
-- so it is checked once per session with real data rather than assumed: the first use of each
-- mapped class does both and logs the two counts. One extra scan per class per session buys the
-- evidence, and a mismatch shows up as a log line instead of as targets silently going missing.
-- ALL of this lives in ONE table on purpose. This file's main chunk sits at Lua's hard ceiling of
-- 200 locals (the same wall `require("pad_poll")` hit on 2026-08-03), so a run of eight scalars
-- and two functions simply does not compile. The lint catches it; the cheaper lesson is not to
-- reach for a new top-level `local` in this file at all.
Nav.SW = {
    -- cls scanned by the sweep -> the GameMode component that already holds that list.
    MANAGERS = {
        ["ATWindRoad"]          = { mgr = "ATWindRoadManager",          list = "WindRoadList" },
        ["PlacementObjectInfo"] = { mgr = "PlacementObjectInfoManager", list = "PlacementObjectInfoList" },
        -- REVERTED 2026-08-03 on its own evidence: the first run logged
        --   `sweep source QuestCharacter: manager QuestManager.QuestCharacterFindList = 13,
        --    scan = 12  <- MISMATCH`
        -- so `QuestCharacterFindList` is NOT the same set as the scan (it held one entry the
        -- scan did not). Which set is the correct one for the radar is unknown, and a silently
        -- different target list is exactly the failure this check exists to prevent, so the
        -- class goes back to scanning until someone establishes what the extra entry is.
        -- ["QuestCharacter"]   = { mgr = "QuestManager", list = "QuestCharacterFindList" },
    },
    mgr = {},         -- manager class -> object, or false = looked for and not present
    checked = {},     -- manager class -> true once the equivalence line has been logged
    -- Accumulated across the ticks of ONE build; nil between builds. This IS the resume: a class
    -- already scanned in this build is served from here, so continuing costs nothing already paid.
    lists = nil,
    t0 = 0,
    partial = false,
    -- The time box applies ONLY to the deferred build on the nav loop — the path whose 578 ms
    -- stall is in the trail. The handful of synchronous callers (chain_to_next, the beacon resume,
    -- the F5 dump) still get a complete list in one call, exactly as today: they are rare, they
    -- cannot use a partial answer, and bounding them was never what the evidence asked for.
    boxed = false,
    -- One nav tick of scanning per nav tick of wall clock. Derived rather than picked: this loop
    -- has TICK_MS to spend, and a sweep that outruns its own loop period is the shape that
    -- produced the stall. At ~65 ms a scan this admits one or two classes per tick, so a cold
    -- build settles over a handful of ticks instead of blocking for 1.2 s in one go.
    BUDGET_S = TICK_MS / 1000,
}

function Nav.SW.manager_list(cls)
    local spec = Nav.SW.MANAGERS[cls]
    if not spec then return nil end
    local m = Nav.SW.mgr[spec.mgr]
    if m == false then return nil end             -- looked for this session and it is not there
    if not Core.valid(m) then
        m = nil
        -- Budgeted like every other acquisition scan in the mod: a miss just retries later.
        if not Core.take_scan_slot() then return nil end
        for _, o in pairs(Core.findall(spec.mgr)) do
            if Core.valid(o) then m = o break end
        end
        Nav.SW.mgr[spec.mgr] = m or false
        if not m then return nil end
    end
    local arr, n = Core.array_of(m, spec.list)
    if not arr or not n then return nil end
    local out = {}
    for i = 1, n do
        local e = arr[i]
        if Core.valid(e) then out[#out + 1] = e end
    end
    if not Nav.SW.checked[spec.mgr] then
        Nav.SW.checked[spec.mgr] = true
        local scanned = 0
        for _, a in pairs(Core.findall(cls)) do
            if Core.valid(a) then scanned = scanned + 1 end
        end
        print(string.format("[KakarotAccess] sweep source %s: manager %s.%s = %d, scan = %d%s\n",
            cls, spec.mgr, spec.list, #out, scanned,
            (#out == scanned) and "" or "  <- MISMATCH, the manager list is not the same set"))
    end
    return out
end

-- Every class the sweep needs goes through here. Returns a (possibly empty) list, never nil.
function Nav.SW.class_list(cls)
    if Nav.SW.lists == nil then return Core.findall(cls) end      -- not inside a build: as before
    local have = Nav.SW.lists[cls]
    if have then return have end
    local got = Nav.SW.manager_list(cls)
    if got == nil then
        -- Must scan. Stop if this build has already had its slice of the tick — but only once
        -- something has been scanned, so a build always makes progress and can never livelock.
        if Nav.SW.boxed and next(Nav.SW.lists) ~= nil and (os.clock() - Nav.SW.t0) >= Nav.SW.BUDGET_S then
            Nav.SW.partial = true
            return {}
        end
        got = Core.findall(cls)
    end
    Nav.SW.lists[cls] = got
    return got
end

-- Call IMMEDIATELY after Nav.list_targets(): true when the time box cut the build short, so the
-- partial result must NOT be published as the snapshot and the caller has to ask again next tick.
-- False also CLOSES the build, which is why it must be called on every path — leaving one open
-- would serve the next build stale lists from a level that may since have streamed out.
function Nav.sweep_partial()
    if Nav.SW.partial then return true end
    Nav.SW.lists = nil
    return false
end

function Nav.list_targets(boxed)
    -- ITS OWN MARK (2026-07-31). This is the mod's single most expensive and most dangerous
    -- operation — 17 unbudgeted FindAllOf plus a dereference of every candidate — and it is
    -- reachable from BOTH the 100 ms nav loop and (before today) the 20 ms pad dispatch. The
    -- streamer's trail ended at `pad.tick` and it took a full audit to work out that this was what
    -- ran inside it. One memcpy on a call that happens every few seconds at most: the next trail
    -- names the sweep outright instead of naming whichever loop invoked it.
    Mem.mark("nav.sweep")
    if not Nav.field_ready() then return {} end
    local pawn = player_pawn()
    if not pawn then return {} end
    local px, py, pz = actor_pos(pawn)
    if not px then return {} end

    -- OPEN, or CONTINUE, a build. `sweep_lists` deliberately survives between ticks: that is what
    -- makes the time box a RESUME rather than a restart, so continuing costs nothing this build
    -- has already paid for. Opened after the gates above so an early return never leaves one open.
    -- SELF-HEALING (2026-08-03). Only a boxed build that reported PARTIAL may be resumed;
    -- anything else still open here was left behind by a caller that did not call
    -- `Nav.sweep_partial()`, and THREE of the five call sites did exactly that. Reusing it is
    -- not merely stale — `class_list` returns a cached list without re-scanning, so after one
    -- such call the sweep never scanned again and kept serving actor handles gathered before
    -- a battle or a level change: the "radar takes a while to pick its target back up" report,
    -- and the dangling-handle class this mod is built to avoid. Discarding here makes the
    -- contract structural instead of a rule every caller has to remember.
    if Nav.SW.lists ~= nil and not Nav.SW.partial then Nav.SW.lists = nil end
    if Nav.SW.lists == nil then Nav.SW.lists = {} end
    Nav.SW.t0, Nav.SW.partial, Nav.SW.boxed = os.clock(), false, boxed and true or false

    local seen = {}
    local groups = {}
    local dropped = {}   -- per-group candidates cut by the distance cap (rescue below)
    local diag = RADAR_DEBUG and {} or nil
    -- Does this actor's CLASS declare AAccessPointBase.InteractState? (CXX dump
    -- 2026-07-17: the AccessPoint* family — ATreasureAccessPoint, AInsectAccessPoint,
    -- AAccessPointItemBase… — plus AMineralMiningPointNormal/Rare and
    -- APlacementObjectInfo all derive from AAccessPointBase. The fruit/small-fish
    -- GATHERING spots do NOT: those are ASpawner*Volume spawners with no taken state
    -- at all.) Matched by class NAME: reading a property a class doesn't declare is
    -- the uncatchable abort, so never probe blindly — and the gathering/collect icons
    -- reach the picker without a class scan.
    local function access_point_class(actor)
        local cn
        pcall(function() cn = actor:GetClass():GetFName():ToString() end)
        return cn ~= nil and (cn:find("AccessPoint", 1, true) ~= nil
            or cn:find("MiningPoint", 1, true) ~= nil
            or cn:find("PlacementObjectInfo", 1, true) ~= nil)
    end
    -- Collected filter for access-point actors (callers guarantee the class declares
    -- InteractState — see access_point_class).
    local function point_taken(a)
        local st
        pcall(function() st = tonumber(Core.member(a, "InteractState")) end)
        return st == STATE_TAKEN
    end
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
            -- Doors take the TIGHT cap: they are level geometry scattered over the whole
            -- outdoors, and a door a kilometre away is noise. Unlike the other tight-capped
            -- sources they ARE eligible for the empty-group rescue below — that exclusion exists
            -- because beyond the cap lies the parked CHARACTER preload pool, and a door volume is
            -- not a pooled character. Rescuing the nearest door is exactly right for someone who
            -- cannot find the way out.
            local cap = (src == "questchar" or src == "collectible" or src == "enemy"
                or src == "door")
                and RADAR_CAP.npc or math.max(RADAR_CAP.map, range or 0)
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
        -- Gathering/collect spots that are access-point actors (mining points, bug
        -- nests, chests) DO carry the Taken state: mark them stateful so the chained
        -- sweep advances the moment they're collected, and drop already-taken ones
        -- from the list (a mined-out rock kept listing until respawn otherwise —
        -- user report 2026-07-17).
        local stateful = (grp == "gathering" or grp == "collectibles")
            and access_point_class(actor) or nil
        if stateful and point_taken(actor) then return end
        add_target(actor, grp, noun, range, src, nil, stateful)
    end

    -- 1) active navi guidance (quest arrows) — always quest-classified
    do
        local icons = Nav.SW.class_list("AT_UIMiniMapNaviIcon")
        for _, icon in pairs(icons) do
            if Core.valid(icon) and icon_in_use(icon) then
                -- Gated hop (was `pcall(function() return icon.TargetActor end)`): the pcall
                -- wrapper does NOT make a naked property fetch on a pooled widget safe (an
                -- undeclared/stale-handle abort pierces pcall, CLAUDE.md §8) — Core.member is
                -- the actual guard.
                local ta = Core.member(icon, "TargetActor")
                if Core.valid(ta) then
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
            local arr, n = Core.array_of(mm, "MapIconList")   -- raw #arr = uncatchable throw
            if not arr then return end
            for i = 1, n do
                local icon = arr[i]
                if Core.valid(icon) then
                    -- Gated hop (was a raw `icon.TargetActor`) — same pooled-widget hazard
                    -- as best_candidate's mapicon fallback above.
                    local ta = Core.member(icon, "TargetActor")
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
    for _, comp in pairs(Nav.SW.class_list("ATMapIconComponent")) do
        if Core.valid(comp) then
            local owner, t
            pcall(function() owner = comp:GetOwner() end)
            pcall(function() t = tonumber(Core.member(comp, "MapIconType")) end)
            if Core.valid(owner) and t then add_icon(owner, t, "mapicon2") end
        end
    end
    -- 2c) ENEMY BASES carry NO ATMapIconComponent (so 2b misses them) but a UATEnemiesBaseBehaviour
    -- component. Find those and classify their owner as ENEMIES_BASE (EMapIcon 32). Scan the base
    -- AND the known subclass (a native-base FindAllOf can return nothing when a subclass exists).
    for _, cls in ipairs({ "ATEnemiesBaseBehaviour", "ATExterminationBastionComponent" }) do
        for _, comp in pairs(Nav.SW.class_list(cls)) do
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
            pcall(function() hidden = Core.member(npc, "bHidden") end)
            if hidden == true or hidden == 1 then return false end
            local ht = 0
            pcall(function() ht = tonumber(Core.member(npc, "CurrentHiddenType")) or 0 end)
            return ht == 0
        end
        -- Scan the native base AND the blueprint subclass: FindAllOf on a native base
        -- can find NOTHING on this game (community-board lesson) — Yajirobe by the
        -- bonfire was a QuestCharacterBase_C the "QuestCharacter" scan never returned
        -- (dump 2026-07-06). add_target dedupes by address when both scans hit.
        local iddump, id_seen = (NPC_ID_DUMP and {} or nil), {}
        for _, cls in ipairs({ "QuestCharacter", "QuestCharacterBase_C" }) do
            for _, npc in pairs(Nav.SW.class_list(cls)) do
                if Core.valid(npc) and npc_present(npc) then
                    add_target(npc, "npc", "cat_npc", nil, "questchar", npc_name(npc))
                    if iddump then
                        local addr = tostring(npc:GetAddress())
                        if not id_seen[addr] then
                            id_seen[addr] = true
                            -- SAME safe read npc_name uses (UniqueId only); no other member.
                            local raw = "?"
                            pcall(function()
                                local u = npc.UniqueId
                                if u then raw = u:ToString() end
                            end)
                            local x, y, z = actor_pos(npc)
                            local d = x and math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2) / M or -1
                            iddump[#iddump + 1] = string.format("  %s uid=%s d=%.0fm resolved=%s",
                                cls, tostring(raw), d, tostring(npc_name(npc)))
                        end
                    end
                end
            end
        end
        if iddump and #iddump > 0 then
            -- Resolver-FORMAT probe: GetCharacterName returned nothing for the raw uids,
            -- so either the key needs a different format or the game has no data. Try
            -- common key variants on known chars + nearby unknowns; ANY hit reveals the
            -- format so every character resolves automatically (no manual map, no guessing).
            local function probe(uid)
                local vs = { uid, uid:upper(), uid:lower(),
                    uid .. "c00", uid .. "c01", uid .. "c02", uid .. "C00" }
                local hits = {}
                for _, v in ipairs(vs) do
                    local nm = game_character_name(v)
                    if nm then hits[#hits + 1] = v .. "=" .. nm end
                end
                return #hits > 0 and table.concat(hits, " ") or "none"
            end
            local probes = {}
            for _, id in ipairs({ "Cpl001", "Cpl002", "Cpl011", "Cpl003", "Cpl013", "Npc019" }) do
                probes[#probes + 1] = "  probe " .. id .. ": " .. probe(id)
            end
            local src = debug.getinfo(1, "S").source:sub(2)
            local dir = src:match("^(.*)[/\\]") or "."
            local f = io.open(dir .. "\\dumps\\dump_npc_ids.txt", "a")
            if f then
                f:write(string.format("[%d] present NPCs: %d\n", os.time(), #iddump))
                f:write(table.concat(iddump, "\n") .. "\n")
                f:write(table.concat(probes, "\n") .. "\n")
                f:close()
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
        -- Spoken noun per collectible CLASS (2026-08-18, user request: name the collectibles).
        -- Every AAccessPointItemBase pickup shares the same reflected fields — a D Medal and an
        -- event chest both read TreasureType 0 and InteractItemType 0 live, and their only
        -- distinguishing member is TreasureSaveName, which is a SAVE key (L11_DMEDAL_310) — so the
        -- CLASS NAME is the only thing that tells the types apart. Without this they all spoke the
        -- generic "objeto". Same technique the Field Memory check already used, table-driven now.
        --
        -- These nouns are DERIVED FROM THE CLASS NAME, not from the game's own text: the proper
        -- source is ItemId -> the item table's JName -> GetMessageFromID, which returns the real
        -- localized name, but no ItemStaticActor was loaded to test that hop (see
        -- reference/dbz-kakarot/notes/dbz-kakarot-item-names.md). When that lands it becomes the
        -- primary and this table stays as the fallback.
        --
        -- FIRST MATCH WINS, so specific patterns come before generic ones: DMedalTreasure_BP_C
        -- contains both "DMedal" and "Treasure" and must not be announced as a plain treasure.
        local COLLECTIBLE_NOUN = {
            { pat = "Memories", noun = "cat_memory" },
            { pat = "Memory",   noun = "cat_memory" },
            { pat = "DMedal",   noun = "cat_dmedal" },
            { pat = "Treasure", noun = "cat_treasure" },
        }
        -- Spoken noun and GROUP per action-point CLASS (2026-08-18, user report: "Sitios está
        -- detectando las tiendas / máquinas de cocina y la tienda que puso Bulma en Namek").
        --
        -- Non-memory action points all used to land in `sites` with the group's own label as the
        -- noun, named by `ActionName` — and `ActionName` is EMPTY on every one of them (read live on
        -- the bonfire and the food stall). So they announced as an anonymous "Sitio". Measured on
        -- Namek, the seven live AFieldActionPointActors and where they belong:
        --
        --   BP_BonfireActor_C            no map icon  -> the cooking campfire
        --   FoodNobody_Store_BP_C        no map icon  -> a SHOP (Bulma's unattended stall)
        --   RestaurantNobody_Store_BP_C  no map icon  -> a SHOP
        --   TCGMachine_BP_C              HAS an icon  -> already Minigames via the icon, deduped
        --   TrainingPoint_BP_C x3        HAS an icon  -> already Sites/cat_practice, deduped
        --
        -- The shops DO have map icons, but on SEPARATE icon-only actors (FoodMapIcon_010,
        -- CookingMapIcon_010) rather than on the store itself — which is exactly why the store
        -- actor came through this path instead and was misgrouped. Routing it to `shops` keeps it
        -- detected (skipping it would lose any store whose area has no icon actor) and puts it where
        -- the player looks for it; add_target dedupes by ADDRESS, so the icon actor and the store
        -- actor stay two entries, both now in the right group and adjacent by distance.
        --
        -- FIRST MATCH WINS: `RestaurantNobody_Store` contains both "Restaurant" and "Store", and
        -- `FoodNobody_Store` contains both "Food" and "Store", so the specific ones come first.
        -- The trailing generic "Store" is not speculation about a class that exists — it is the
        -- honest default for a name that says shop, and a string match costs nothing (unlike naming
        -- a CLASS, which would join the absent-scan set).
        local ACTION_POINT = {
            { pat = "Bonfire",       grp = "sites",  noun = "cat_bonfire" },
            { pat = "TrainingPoint", grp = "sites",  noun = "cat_practice" },
            { pat = "Restaurant",    grp = "shops",  noun = "cat_restaurant" },
            { pat = "Cooking",       grp = "shops",  noun = "cat_cooking_shop" },
            { pat = "Food",          grp = "shops",  noun = "cat_food_shop" },
            { pat = "Store",         grp = "shops",  noun = "radar_cat_shops" },
            { pat = "Shop",          grp = "shops",  noun = "radar_cat_shops" },
        }
        local function visible_actor(a)
            -- Gated hop (was `pcall(function() hidden = a.bHidden end)`): `a` comes straight
            -- out of the FindAllOf over the actor classes just below (an on-demand scan -
            -- NOT per tick), so it is fresh, but a raw fetch still bypasses the gate Core.member
            -- provides for free — route it through the same helper as every other member
            -- read in this scan instead of trusting the pcall wrapper alone.
            local hidden = Core.member(a, "bHidden")
            return not (hidden == true or hidden == 1)
        end
        -- Collected filter: point_taken (module level). InteractState is declared on
        -- AAccessPointBase ONLY — FieldActionPointActor is a plain AActor, and reading
        -- a non-existent property is the uncatchable abort, so only the access-point
        -- classes in this scan list carry `state = true`.
        -- Spoken name for an action spot: the game's own ActionName (reflected FString
        -- on AFieldActionPointActor @0x340). nil when empty -> generic noun.
        local function action_name(a)
            local s
            pcall(function()
                -- Truthiness is not a validity check on a Core.member result (see drop_item_name);
                -- Core.name_str is, and it never asks an FName for a method it does not have.
                s = Core.name_str(Core.member(a, "ActionName"))
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
                local comp = Core.member(a, "ItemTableComponent")
                if not (Core.valid(comp)) then return end
                local d = Core.member(comp, "FieldItemDropData")
                -- Core.member validates the OWNER, not the RESULT. A null/empty field yields an
                -- INVALID RemoteObject, which is NOT nil — so `if not d` let it straight through
                -- and the next hop resolved a property on a dead handle: the +0x10 fault. This is
                -- the prime suspect for the 2026-07-26 (c) crash (it is the only live fetch in the
                -- whole step() graph whose receiver never reached Core.valid, which is exactly
                -- what "Mem.alive logged zero rejections" demands).
                -- valid_REF, not valid: `d` is an FStruct handle, and Core.valid would call
                -- GetAddress on it, which UE4SS raises THROUGH pcall.
                if not Core.valid_ref(d) then return end
                for _, fld in ipairs({ "FixedId", "NormalId" }) do
                    -- FName members of the struct: read them as names, never IsValid them.
                    local s = Core.name_str(d[fld])
                    if s and s ~= "None" then raw = s return end
                end
            end)
            if not raw then return nil end
            -- Strip the drop-table bookkeeping before speaking it. The keys are `<Item>_Fixed` and
            -- `<Item>_AreaNN_NN` (live 2026-08-18: `Lost_Seaweed_Fixed`, `Lost_Seaweed_Area11_01`),
            -- so those suffixes are pure noise to a listener — "Lost Seaweed Area11 01" was being
            -- read out in full. Still the game's INTERNAL English id, not its localized name; see
            -- the note above on COLLECTIBLE_NOUN for the real fix.
            local base = raw:gsub("_Fixed$", ""):gsub("_Normal$", "")
                            :gsub("_Area%d+_%d+$", ""):gsub("_%d+$", "")
            local cleaned = ((base ~= "" and base) or raw):gsub("_", " ")
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
            for _, a in pairs(Nav.SW.class_list(c.cls)) do
                if Core.valid(a) and visible_actor(a)
                    and (not c.state or not point_taken(a)) then
                    local cn = "?"
                    pcall(function() cn = a:GetClass():GetFName():ToString() end)
                    local grp = "collectibles"
                    local noun = "cat_item"
                    for _, m in ipairs(COLLECTIBLE_NOUN) do
                        if cn:find(m.pat, 1, true) then noun = m.noun break end
                    end
                    -- Derived from the resolved noun, not from a second `find("Memories")`, so the
                    -- two can never disagree about what a Field Memory is. The DLC variants
                    -- (BP_DLC6_/BP_DLC7_FieldMemoriesActor) match the same pattern.
                    local is_memory = (noun == "cat_memory")
                    if c.action and not is_memory then
                        -- Non-memory action points are interactable SPOTS you use
                        -- (train/meditate/examine — e.g. Piccolo's waterfall), not
                        -- pickups. Route by class name; anything unrecognised keeps the
                        -- old behaviour, so this can only improve a classification.
                        grp, noun = "sites", "radar_cat_sites"
                        for _, m in ipairs(ACTION_POINT) do
                            if cn:find(m.pat, 1, true) then grp, noun = m.grp, m.noun break end
                        end
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
    for _, a in pairs(Nav.SW.class_list("ATWindRoad")) do
        if Core.valid(a) then
            add_target(a, "sites", "cat_windroad", nil, "collectible")
        end
    end
    -- 4b2) DOORS / area transitions (AATDoorVolume < ATriggerBox, AT.hpp:13004). The thing you
    -- walk into to leave a building — requested 2026-07-25 after being trapped inside Goku's house
    -- with no way to find the exit. The class is an ACTOR, so it drops straight into add_target and
    -- needs no new tracking machinery (the radar cannot follow a bare position — see the target
    -- shape in set_manual_target — which is why the "remember where you came in" fallback was NOT
    -- built: it would have meant rewriting that core).
    --
    -- No filtering beyond distance and validity, deliberately. The doors are bidirectional (a
    -- single AATDoorVolume pairs with its `DestinationDoor`), so the nearest one when you are
    -- indoors IS the way out, and outdoors the same list reads as "ways in" — both useful. There
    -- are flags here whose meaning is not established (`bOnlyUsedInRoom`, `bUseDialog`), and
    -- guessing at them could hide the very door the player needs.
    for _, a in pairs(Nav.SW.class_list("ATDoorVolume")) do
        if Core.valid(a) then
            -- `AreaName` (FName @0x378) names the destination, so the picker can say WHERE the
            -- door goes instead of just "exit". It may be an internal identifier rather than
            -- display text; that is still far better than nothing when you are lost, and it falls
            -- back to DoorName and then to the bare category noun.
            local label
            for _, prop in ipairs({ "AreaName", "DoorName" }) do
                if label == nil then
                    pcall(function()
                        local s = Core.name_str(Core.member(a, prop))
                        if s and s ~= "None" then label = s end
                    end)
                end
            end
            add_target(a, "exit", "radar_cat_exit", nil, "door", label)
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
            [3] = { grp = "hunt", noun = "cat_hunt" },
            [9] = { grp = "sites", noun = "radar_cat_sites" },
        }
        for _, comp in pairs(Nav.SW.class_list("FieldPointComponent")) do
            if Core.valid(comp) then
                local t
                pcall(function() t = tonumber(Core.member(comp, "FieldPointIconType")) end)
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
        for _, a in pairs(Nav.SW.class_list(cls)) do
            if Core.valid(a) then
                add_target(a, "sites", "cat_portal", nil, "portal")
            end
        end
    end

    -- 5) field enemies: the direct SpawnType scan (enemies_list) — the minimap icon
    -- list does not carry the roaming enemies. Tight distance cap like NPCs.
    for _, e in ipairs(enemies_list()) do
        add_target(e.actor, "enemies", e.noun, nil, "enemy", e.disp or e.name)
    end
    -- 5b) wild field animals (deer, wolves, dinosaurs, dragons…): the AT_MobAnimalBase subtree of
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
        for _, a in pairs(Nav.SW.class_list(cls)) do
            if Core.valid(a) and is_animal(a) then
                local hidden = false
                pcall(function() hidden = Core.member(a, "bHidden") end)
                if not (hidden == true or hidden == 1) then
                    -- Wild animals are HUNT targets (prey), not combat enemies — their own
                    -- category so the deer/wolves/dinos/dragons don't clutter "Enemies".
                    add_target(a, "hunt", animal_species(a) or "cat_animal", nil, "enemy")
                end
            end
        end
    end

    -- 5c) field party members: the same collector Shift+F5 cycles (excludes the player,
    -- SpawnType enemies and the far parked pool via COMPANION_MAX_DIST), so the two
    -- features can never disagree on who counts as a companion. Named via the same
    -- CharacterName resolver as enemies (companions are AT_Character too).
    for _, c in ipairs(companions(px, py, pz)) do
        add_target(c.actor, "companions", "cat_companion", nil, "companion",
            enemy_display_name(c.actor))
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
                    if Core.valid(sw) then
                        local s = sw.WL_NaviIconSwitcher
                        if Core.valid(s) then iidx = tostring(tonumber(s.ActiveWidgetIndex)) end
                        local b = sw.WL_NaviBaseSwitcher
                        if Core.valid(b) then bidx = tostring(tonumber(b.ActiveWidgetIndex)) end
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
            local _, n = Core.array_of(ic, "TalkObjectArray")
            n = n or 0
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

-- Stop tracking the current target on demand (the R3 menu's B button). Silences the
-- beacon and drops the target; if a quest is active the auto-scan may re-acquire it
-- next tick (that's the base radar), but a hand-picked target stays gone until
-- re-picked. Does NOT turn the whole radar off (F3 does that).
function Nav.stop_tracking()
    preempt.scans, preempt.pri = 0, nil   -- an explicit B always disarms the fresh signal
    -- A fresh quest objective PREEMPTED a hand-picked target/sweep: the first B
    -- returns to that pick — nearest remaining item for a chainable category, the
    -- exact one otherwise. B again (no stash left) stops for real.
    if preempt.stash and target and not target.manual then
        local p = preempt.stash
        preempt.stash = nil
        drop_target()
        Audio.stop()
        chain_wait, chain_seen = nil, p.seen or {}
        auto_suppressed = true   -- the quest must not re-grab; a successful re-target
                                 -- clears this again (set_manual_target)
        if chainable(p.grp) then
            chain_to_next(p.grp)   -- announces the re-target (or the sweep-done phrase)
        else
            local found
            for _, c in ipairs(Nav.list_targets()) do
                if c.key == p.grp then
                    for _, it in ipairs(c.items) do
                        if it.key == p.key then found = it break end
                    end
                    break
                end
            end
            if found then
                Nav.set_manual_target(found.actor, found.key, Nav.item_label(found),
                    found.grp, found.stateful, true)
            else
                Speech.say(I18n.t("nav_stopped"), true)
            end
        end
        return
    end
    local had = target ~= nil or chain_wait ~= nil or resume_pick ~= nil
    drop_target()
    chain_wait, chain_seen = nil, {}   -- also ends a pending sweep
    resume_pick = nil        -- an explicit stop also forgets the battle-interrupted pick
    preempt.stash = nil
    auto_suppressed = true   -- stay quiet until re-pick / F3 (don't auto-grab a quest)
    Audio.stop()
    if had then Speech.say(I18n.t("nav_stopped"), true) end
end

-- Objective-advanced signal (quest_objective.lua, wired in app.lua): the quest HUD's
-- objective text genuinely changed. Arm the auto-scan's one-shot preemption (see the
-- `preempt` block for the whole design). kind = "main" | "sub" biases the
-- marker search toward that quest class. Cheap flag set — safe from any thread the
-- quest loop runs on (it's the game thread anyway).
function Nav.notify_objective_change(kind)
    if not on then return end   -- F3 off = the radar is fully off; respect it
    -- Mod config: "radar automático" off → don't auto-activate or disturb the manual pick.
    local cfg = _G.__KakarotSettings
    if cfg and not cfg.autotrack_enabled() then return end
    local want = (kind == "main" and PRI_MAIN) or (kind == "sub" and PRI_SUB) or nil
    -- QUEST FOCUS (`preempt.focus`, 2026-07-28) — the preempt is a one-shot, and that is the
    -- other half of the "it goes back to the main quest" report: even once the classification is
    -- right, the bias lasted ~15 s and then the normal order (main outranks sub) took over again,
    -- so a multi-phase side story lost the radar between phases.
    --
    -- The focus is a CONTEXT rather than a nudge: whichever quest class the player is actually
    -- working stays preferred on EVERY auto-scan, so each new phase is picked up without opening
    -- the picker. Consequences, both deliberate:
    --   * a MAIN objective advancing in the background while a side story is focused does NOT
    --     steal the radar (that is the whole complaint), so an off-focus kind returns here;
    --   * the focus survives a map change, because it is released only by evidence — see
    --     Nav.notify_objective_gone, which fires only when the quest HUD is READABLE and no
    --     longer lists that class. A load screen, a fight or an open menu simply hide the HUD and
    --     produce no verdict at all.
    if preempt.focus and want and want ~= preempt.focus then return end
    if want and want ~= preempt.focus then
        preempt.focus = want
        print(string.format("[KakarotAccess] quest focus -> %s\n", tostring(kind)))
    end
    -- ARM FROM ZERO ONLY (2026-07-31, the Krillin-cutscene report). The consumer drains this
    -- counter at most once per SCAN_EVERY (~1.5 s), while this callback can fire as fast as the
    -- quest HUD reader polls (300 ms) — so any caller that flaps held the preempt PERMANENTLY
    -- armed, and an armed preempt deliberately bypasses the suppressors, keeping the marker walk
    -- running straight through a cutscene over per-level handles the engine is freeing. The flap
    -- is fixed at its source too (quest_objective now settles before signalling); this is the
    -- defence in depth, because "armed" is a state no upstream bug should be able to pin.
    -- Re-arming was never the meaningful transition anyway: only going from spent to armed is.
    -- A second genuine change while armed still retargets through preempt.pri just below.
    if preempt.scans <= 0 then
        preempt.scans = preempt.TRIES
        -- LOOK FOR THE MARKER ON THE NEXT NAV TICK, not on the next multiple of SCAN_EVERY
        -- (2026-08-03, user: "the radar also takes a while when the objective changes"). `tick`
        -- free-runs, so that modulo is an arbitrary 0-1.5 s of doing nothing between the game
        -- issuing a new objective and the radar going to look for it — the third place in this
        -- file where a modulo on a free-running counter was silently acting as a random delay.
        --
        -- Armed INSIDE this branch deliberately, and for exactly the reason the branch itself
        -- exists: the zero->armed transition is the genuine event, while this callback can fire
        -- as fast as the quest HUD reader polls. A flapping caller that pinned this window would
        -- turn the 1.5 s cadence into a permanent per-tick marker walk — the scan storm the
        -- 2026-07-31 fix above is written to prevent. One event, one bounded window (see the
        -- world-gate edge for the derivation), and it closes as soon as a target is acquired.
        Nav.auto_until, Nav.auto_t0 = tick + LOST_SCANS * SCAN_EVERY, os.clock()
    end
    preempt.pri = want
    -- IDLE radar (no hand-picked target, no battle-interrupted resume pending): a
    -- freshly activated objective must be tracked and KEEP being tracked even if its
    -- marker lags past the ~15 s preempt window — the classic case is waiting for an
    -- NPC to trigger the step (Gohan finds the apples) so the marker spawns seconds
    -- after the HUD text changes. The preempt alone bypasses auto_suppressed only for
    -- its TRIES scans; here we also lift the post-B / post-arrival silence so the base
    -- auto-scan (step) owns the objective persistently. A MANUAL pick is left alone: it
    -- goes through the preempt-and-stash path so B still brings it back.
    if not (target and target.manual) and not resume_pick then
        if auto_suppressed then
            print("[KakarotAccess] objective change while idle: re-arming auto-track\n")
        end
        auto_suppressed = false
    end
end

-- The quest HUD is readable and no longer lists this class — the side story finished (or was
-- abandoned), so release the focus and let the normal priority order take the radar back to the
-- main quest. Called from quest_objective, which only reaches this verdict with the HUD host on
-- screen and after a settle, so a load / battle / open menu can never trigger it.
function Nav.notify_objective_gone(kind)
    local pri = (kind == "main" and PRI_MAIN) or (kind == "sub" and PRI_SUB) or nil
    if pri and preempt.focus == pri then
        print(string.format("[KakarotAccess] quest focus released (%s ended)\n", tostring(kind)))
        preempt.focus = nil
    end
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
    resume_pick = nil         -- and replaces any battle-interrupted pick
    gated_prev = false
    companion_idx = 0
    chain_wait = nil          -- an explicit pick replaces any pending sweep wait
    if not keep_sweep then
        chain_seen = {}
        preempt.stash = nil                    -- a player pick replaces the stashed one
        preempt.scans, preempt.pri = 0, nil    -- and disarms a pending fresh signal
    end
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

-- Ctrl+Shift+F5 (dev, ONE capture round): pin the native LEVEL member. Ghidra
-- (2026-07-17, code/decompiled/manual_140f8aba0.c + _ufunc_GetPowerCompareRank.c)
-- proved the live level is served virtually — AttributeComponent (char+0x8E8)
-- vtable[0x3E8]() -> int32, minimum 1 — but the member it reads is unreachable
-- statically (RTTI-stripped binary, ~200 candidate vtables). Hypothesis A: a cached
-- int in the component's unreflected tail (reflected members end at StatusInstance
-- @0x100; window below). Hypothesis B: the StatusInstance body (UATStatusInstanceBase
-- 0x390 / enemy subclass 0x3A0 — reflects NOTHING). Dump both windows for the PLAYER
-- (level always known from the menu) + the nearby enemies; the offset matching every
-- displayed "Lv N" goes to native_offsets.lua. All reads SEH-guarded via mem_bridge.
function Nav.dump_levels()
    -- Constants live INSIDE the function: the file-level chunk is at Lua's 200-local cap.
    local ATTR_TAIL_FROM, ATTR_TAIL_TO = 0x104, 0x140  -- component tail after StatusInstance @0x100
    local SI_BODY_SIZE = 0x3A0                         -- UATEnemyStatus instance size (CXX dump)
    local LV_MIN, LV_MAX = 1, 999                      -- dump plausibility filter only (getter's
                                                       -- own default is 1; NOT a game rule)
    Speech.say("level dump v2", true)   -- version beacon: hearing "v2" proves this code runs
    ExecuteInGameThread(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_enemy_level.txt", "a")
        if not f then Speech.say("level dump: cannot open file", true) return end
        pcall(function() f:setvbuf("no") end)   -- see Nav.dump: survive a mid-dump abort
        f:write(string.format("\n== level dump v2 @ %s ==\n", os.date("%H:%M:%S")))
        if not Mem.is_loaded() then f:write("mem_bridge NOT loaded\n") f:close() return end
        -- SAME GATE AS Nav.dump. Mid-transition, with a menu up, or with the world hidden
        -- (battle, cutscene), every actor read below could abort uncatchably — and the enemy
        -- cache this dump iterates is precisely the list a battle has just invalidated: it is
        -- only refreshed every RESCAN_CLASSES ticks, so pressing the key within ~10 s of a
        -- fight serves handles to destroyed AT_Characters. This is bound to a shipping keybind
        -- with no debug flag, and a diagnostic that can kill the process destroys the very
        -- evidence it exists to collect.
        local trans, muted = Transition.active(), ui_muted()
        if trans or muted or not world_alive() then
            f:write("GATED (" .. (trans and "transition" or muted and "ui" or "world")
                .. ") — actor sections skipped\n")
            f:close()
            Speech.say("level dump written (gated)", true)
            return
        end
        -- vtable[LEVEL_SLOT] on the AttributeComponent = the game's level getter (Ghidra
        -- 2026-07-17, manual_140f8aba0.c: level = attrib->vtable[0x3E8]()). Statically the
        -- concrete vtable was unreachable (RTTI stripped); at RUNTIME it's two pointer
        -- reads, and its RVA hands Ghidra the exact function to decompile.
        -- Engine-free info FIRST, engine lookups pcall'd with progress markers: the
        -- 11:23 captures died silently between the header and the first engine call
        -- (header-only dumps), so every step now leaves a trace.
        local LEVEL_SLOT = 0x3E8
        -- exe base, three ways: the mem.lua wrapper (nil after a hot reload — the boot
        -- snapshot restores the OLD mem.lua without it), the boot-time C module directly
        -- (its table always has module_base — the DLL predates the wrapper), and as a
        -- last resort an MZ scan downward from a code VA (SEH-guarded reads, 64K steps).
        local exe_base = (Mem.module_base and Mem.module_base()) or 0
        if exe_base == 0 then
            local mb = package.loaded and package.loaded["mem_bridge"]
            if mb and mb.module_base then exe_base = mb.module_base() or 0 end
        end
        local function mz_scan(code_va)
            local probe = code_va - (code_va % 0x10000)
            for _ = 1, 8192 do   -- up to 512 MB below the code address
                local sig = Mem.at_bytes(probe, 0, 2)
                if sig == "MZ" then return probe end
                probe = probe - 0x10000
                if probe <= 0 then break end
            end
            return 0
        end
        f:write(string.format("exe base (pre-scan): %X (ghidra addr = 0x140000000 + rva)\n", exe_base))
        -- Raw opcode bytes of a function: a trivial getter (mov eax,[rcx+imm]; ret)
        -- exposes its member offset right in the hex, no decompiler needed.
        local function code_hex(va, n)
            local raw = va and Mem.at_bytes(va, 0, n)
            if not raw then return "?" end
            local out = {}
            for i = 1, #raw do out[#out + 1] = string.format("%02X", raw:byte(i)) end
            return table.concat(out, " ")
        end
        local pawn, px, py, pz
        local okp, errp = pcall(function() pawn = player_pawn() end)
        if not okp then f:write("player_pawn ERROR: " .. tostring(errp) .. "\n") end
        if pawn then
            local oka, erra = pcall(function() px, py, pz = actor_pos(pawn) end)
            if not oka then f:write("actor_pos(player) ERROR: " .. tostring(erra) .. "\n") end
        else
            f:write("no player pawn\n")
        end
        local function cls_name(o)
            local s = "?"
            pcall(function() s = o:GetClass():GetFName():ToString() end)
            return s
        end
        -- Follow the virtual getter chain LIVE, decoding the idioms seen in this exe
        -- (2026-07-17 capture): forwarder `mov rcx,[rcx+i32]; mov rax,[rcx]; jmp [rax+i32]`
        -- (48 8B 89 .. / 48 8B 01 / 48 FF A0 ..) and leaf loads `mov eax,[rcx+disp]; ret`
        -- (8B 81/8B 41) or `movss xmm0,[rcx+disp]; ret` (F3 0F 10 81). Returns the hop
        -- trail plus, when a leaf is reached, the final holder VA + member offset + type.
        local function follow_getter(obj_addr, slot)
            local hops = {}
            local function i32at(s, i)
                local b1, b2, b3, b4 = s:byte(i, i + 3)
                local v = b1 + b2 * 256 + b3 * 65536 + b4 * 16777216
                if v >= 2 ^ 31 then v = v - 2 ^ 32 end
                return v
            end
            for _ = 1, 4 do
                local vt = Mem.at_ptr(obj_addr, 0)
                local fn = vt and Mem.at_ptr(vt, slot)
                local b = fn and Mem.at_bytes(fn, 0, 24)
                if not b then return hops end
                hops[#hops + 1] = string.format("obj=%X slot=0x%X fn=%X rva=%X",
                    obj_addr, slot, fn, exe_base ~= 0 and (fn - exe_base) or 0)
                if b:byte(1) == 0x8B and b:byte(2) == 0x81 and b:byte(7) == 0xC3 then
                    return hops, obj_addr, i32at(b, 3), "i32"
                elseif b:byte(1) == 0x8B and b:byte(2) == 0x41 and b:byte(4) == 0xC3 then
                    return hops, obj_addr, b:byte(3), "i32"
                elseif b:byte(1) == 0xF3 and b:byte(2) == 0x0F and b:byte(3) == 0x10
                    and b:byte(4) == 0x81 and b:byte(9) == 0xC3 then
                    return hops, obj_addr, i32at(b, 5), "f32"
                elseif b:byte(1) == 0x48 and b:byte(2) == 0x8B and b:byte(3) == 0x89
                    and b:byte(8) == 0x48 and b:byte(9) == 0x8B and b:byte(10) == 0x01
                    and b:byte(11) == 0x48 and b:byte(12) == 0xFF and b:byte(13) == 0xA0 then
                    local next_obj = Mem.at_ptr(obj_addr, i32at(b, 4))
                    if not next_obj or next_obj == 0 then return hops end
                    obj_addr, slot = next_obj, i32at(b, 14)
                else
                    hops[#hops + 1] = "unknown idiom: " .. code_hex(fn, 24)
                    return hops
                end
            end
            return hops
        end
        local function dump_char(tag, c)
            -- The handle may be minutes old (the enemy cache is sparse) — check it before
            -- the first hop, and fetch every member through Core.member: a naked
            -- `c.AttributeComponent` on a class that does not declare it is the uncatchable
            -- abort no pcall can catch. Same shape as enemy_level.
            if not Core.valid(c) then f:write(tag .. ": dead handle\n") return end
            local attrib
            pcall(function()
                local a = Core.member(c, "AttributeComponent")
                if Core.valid(a) then attrib = a end
            end)
            if not attrib then f:write(tag .. ": no AttributeComponent\n") return end
            f:write(string.format("%s  attrib=%s @%X\n", tag, cls_name(attrib), Mem.addr(attrib) or 0))
            local a_addr = Mem.addr(attrib)
            local vt = a_addr and Mem.at_ptr(a_addr, 0)
            local getter = vt and Mem.at_ptr(vt, LEVEL_SLOT)
            if getter and exe_base == 0 then exe_base = mz_scan(getter) end
            f:write(string.format("  vtable=%X level_getter_va=%X exe_base=%X rva=%X\n",
                vt or 0, getter or 0, exe_base,
                (getter and exe_base ~= 0) and (getter - exe_base) or 0))
            if getter then f:write("  getter code: " .. code_hex(getter, 64) .. "\n") end
            if a_addr then
                local hops, holder, memb, ty = follow_getter(a_addr, LEVEL_SLOT)
                for _, h in ipairs(hops) do f:write("  hop: " .. h .. "\n") end
                if holder and memb then
                    local vi = Mem.at_i32(holder, memb)
                    local vf
                    if ty == "f32" then
                        local raw = Mem.at_bytes(holder, memb, 4)
                        if raw and #raw == 4 then vf = string.unpack("<f", raw) end
                    end
                    f:write(string.format("  LEVEL RESOLVED: holder=%X member=+0x%X type=%s i32=%s f32=%s\n",
                        holder, memb, ty, tostring(vi), tostring(vf)))
                end
            end
            local tail = {}
            for off = ATTR_TAIL_FROM, ATTR_TAIL_TO - 4, 4 do
                local v = Mem.i32(attrib, off)
                if v then tail[#tail + 1] = string.format("0x%X=%d", off, v) end
            end
            f:write("  attrib tail i32: " .. table.concat(tail, " ") .. "\n")
            local si
            pcall(function()
                local s = Core.member(attrib, "StatusInstance")
                if Core.valid(s) then si = s end
            end)
            if not si then f:write("  no StatusInstance\n") return end
            f:write(string.format("  si=%s @%X, slots with i32 in %d..%d:\n",
                cls_name(si), Mem.addr(si) or 0, LV_MIN, LV_MAX))
            local ints, floats = {}, {}
            for off = 0, SI_BODY_SIZE - 4, 4 do
                local v = Mem.i32(si, off)
                if v and v >= LV_MIN and v <= LV_MAX then
                    ints[#ints + 1] = string.format("0x%X=%d", off, v)
                end
                local fv = Mem.float(si, off)
                if fv and fv >= LV_MIN and fv <= LV_MAX and fv == math.floor(fv) then
                    floats[#floats + 1] = string.format("0x%X=%.0f", off, fv)
                end
            end
            f:write("    i32: " .. table.concat(ints, " ") .. "\n")
            f:write("    f32(whole): " .. table.concat(floats, " ") .. "\n")
        end
        if pawn then
            local ok, err = pcall(dump_char, "PLAYER", pawn)
            if not ok then f:write("PLAYER dump ERROR: " .. tostring(err) .. "\n") end
        end
        local n = 0
        local oke, erre = pcall(function()
            for _, e in ipairs(enemies_list()) do
                if n >= 6 then break end
                -- Per-entry validity, like every other consumer of this list (enemy_alert,
                -- Nav.dump): the cache is only rebuilt every RESCAN_CLASSES ticks, so an
                -- entry can name an actor the last fight destroyed.
                if Core.valid(e.actor) then
                    n = n + 1
                    local d = "?"
                    local x, y, z = actor_pos(e.actor)
                    if x and px then
                        d = string.format("%.0fm", math.sqrt((x - px) ^ 2 + (y - py) ^ 2 + (z - pz) ^ 2) / M)
                    end
                    dump_char(string.format("ENEMY %d (%s, %s)", n, e.name or I18n.t(e.noun), d), e.actor)
                end
            end
        end)
        if not oke then f:write("enemy dump ERROR: " .. tostring(erre) .. "\n") end
        if n == 0 then f:write("no enemies in range\n") end
        f:close()
        Speech.say("level dump done", true)
    end)
end

-- Ctrl+F5 (dev): dump every guidance candidate + a NavMesh probe to
-- Scripts/dumps/dump_nav_targets.txt so a failing scan can be diagnosed offline.
function Nav.dump()
    -- How often the census writes a step marker while walking one anchor's objects. Derived
    -- from the observed census size rather than picked: the run of 2026-08-15 reported
    -- total=4640 for the CFUIMultiLineTextBox anchor, so a stride of 200 costs ~23 extra lines
    -- per anchor (noise against a 280-line dump) while pinning an abort to a 200-object window
    -- the markers name both ends of. Declared HERE, not at module scope: this file already sits
    -- at Lua's 200-local ceiling for the main chunk, and one more breaks the whole file.
    local CENSUS_STEP_STRIDE = 200
    -- Version beacon BEFORE any engine work: hearing it proves the reload applied and
    -- the keybind fired; the file then shows how far the dump got (unbuffered writes).
    Speech.say("dump v2", true)
    ExecuteInGameThread(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        -- APPEND (was "w"): the user often takes one dump per broken screen in a session,
        -- and overwrite mode lost all but the last (2026-07-15, the items dump). The
        -- timestamped header separates runs.
        -- CRASH-TRAIL MARK (2026-08-15). This dump was the one heavy game-thread operation in the
        -- mod with NO mark, so when the boot crash happened the trail ended in an ordinary
        -- `quest.step` and read as "a normal tick killed it" — the exact blind spot nav.markers /
        -- nav.mapicons / nav.sweep were each added to close. Without this, the dump is invisible to
        -- the black box and every crash inside it is misattributed to whichever loop marked last.
        Mem.mark("nav.dump")
        local f = io.open(dir .. "\\dumps\\dump_nav_targets.txt", "a")
        if not f then Speech.say("nav dump: cannot open file", true) return end
        -- UNBUFFERED: an uncatchable abort mid-dump otherwise loses EVERYTHING written
        -- so far (a 0-byte file, seen 2026-07-06 in the Raditz-road area) — with no
        -- buffer the file shows exactly how far the dump got before dying.
        pcall(function() f:setvbuf("no") end)
        f:write(string.format("\n== nav target dump @ %s ==\n", os.date("%H:%M:%S")))
        -- Full radar state, so a silent radar can be diagnosed from this file alone.
        -- ui_muted is checked FIRST and short-circuits: with a menu up we must not
        -- probe the minimap at all (level teardown hides behind menus).
        local trans = Transition.active()
        local muted = ui_muted()
        f:write(string.format("on=%s route_mode=%s transition=%s ui_muted=%s world_alive=%s adapter_index=%s\n",
            tostring(on), tostring(route_mode), tostring(trans), tostring(muted),
            tostring(not trans and not muted and world_alive()),
            tostring(Registry.active_index and Registry.active_index())))

        -- EARLY GATE (2026-08-15 — the boot crash). This dump already HAD a safety gate for
        -- "mid-transition / no world", but it sat ~290 lines below, AFTER the screen directory and
        -- the visible-screen census — i.e. after thousands of widget reads had already happened.
        -- So a dump triggered while the game was still loading did all the dangerous work first and
        -- consulted the gate only if it survived. It did not: a stale command replayed one second
        -- into boot took the process down with the world not yet up.
        --
        -- `muted` is deliberately NOT part of this early gate: a dump taken with a menu open is the
        -- single most useful one there is (that is what the census is FOR), and a menu being up is
        -- not by itself unsafe. Only the two conditions that mean "there is nothing valid to read"
        -- bail here. `world_alive()` is asked only when NOT muted, matching the short-circuit the
        -- header line above already relies on — with a menu up the minimap must not be probed.
        if trans or (not muted and not world_alive()) then
            f:write("GATED EARLY (" .. (trans and "transition" or "world")
                .. ") — no world to read; census and actor sections skipped\n")
            f:close()
            Speech.say("nav dump written (gated early)", true)
            return
        end
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
        -- FindAllOf cost telemetry (ui_core.timed_findall) — attributes step cost to
        -- full-object scans vs widget walks. Resets with each dump, like the step stats.
        local sc = _G.__KakarotScanStats
        if sc and sc.n > 0 then
            f:write(string.format("findall scans: n=%d total_ms=%.0f max_ms=%.1f avg_ms=%.1f\n",
                sc.n, sc.ms, sc.max, sc.ms / sc.n))
            -- Per-class attribution (ui_core.timed_findall): the classes draining the
            -- scan budget, worst first — the shortlist for directory mapping.
            local rows = {}
            for cls, b in pairs(sc.by or {}) do rows[#rows + 1] = { cls, b.n, b.ms } end
            table.sort(rows, function(x, y) return x[3] > y[3] end)
            for i = 1, math.min(#rows, 12) do
                f:write(string.format("  scan %-40s n=%d ms=%.0f\n",
                    rows[i][1], rows[i][2], rows[i][3]))
            end
            _G.__KakarotScanStats = nil
        end
        -- GHOST CLASSES — the offender list. A class still at found=0 after a session of
        -- play is a name no object in this game ever answers to, yet every adapter that
        -- names one pays a full FindAllOf (~65 ms) every ABSENT_BACKOFF for the whole
        -- session. The perf note forbids tuning that backoff without this list ("it
        -- starves event-less popups", 2026-07-17), and the four native/Blueprint TWIN
        -- pairs the adapters carry (Choice_Win_C/AT_UIChoiceWin, Choice_Cmd_C/
        -- AT_UIChoiceCmd, Xcmn_Subtitles_C/ATUISubtitles, Quest_Sub_Reward_C/
        -- AT_UIQuestSubReward) can only be settled here: UE4SS's docs say FindAllOf
        -- matches subclasses, this file's own 2026-07 comments record it returning
        -- nothing for a native base, and one measured session decides which half is dead.
        -- Unlike the scan stats above, the ledger is NOT reset by a dump: "never found"
        -- is only meaningful over a whole session.
        local led = Core.scan_ledger and Core.scan_ledger()
        if led then
            local ghosts, gms, gn = {}, 0, 0
            for cls, g in pairs(led) do
                if g.found == 0 then
                    ghosts[#ghosts + 1] = { cls, g.n, g.ms }
                    gn, gms = gn + g.n, gms + g.ms
                end
            end
            table.sort(ghosts, function(x, y) return x[3] > y[3] end)
            f:write(string.format("ghost classes: %d never found (%d scans, %.0f ms burned)\n",
                #ghosts, gn, gms))
            for i = 1, #ghosts do
                f:write(string.format("  ghost %-40s n=%d ms=%.0f\n",
                    ghosts[i][1], ghosts[i][2], ghosts[i][3]))
            end
        end
        -- Speech-backend cost (speech.lua timed_say): each prism call runs on the game
        -- thread — cinematics are the densest speech state, so a slow backend shows
        -- here as a per-line hitch the scan stats can never see. Resets per dump.
        local sp = _G.__KakarotSpeechStats
        if sp and sp.n > 0 then
            f:write(string.format("speech calls: n=%d total_ms=%.0f max_ms=%.1f avg_ms=%.1f\n",
                sp.n, sp.ms, sp.max, sp.ms / sp.n))
            _G.__KakarotSpeechStats = nil
        end
        -- Nav-loop cost (nav_tracker's own 100ms loop — outside the registry step).
        local nv = _G.__KakarotNavStats
        if nv and nv.n > 0 then
            f:write(string.format("nav step ms: max=%.1f avg=%.2f over %d ticks\n",
                nv.max, nv.ms / nv.n, nv.n))
            _G.__KakarotNavStats = nil
        end
        -- Battle/quest loop cost (their own 250/300ms loops — same reason).
        for _, e in ipairs({ { "battle", "__KakarotBattleStats" },
                             { "quest", "__KakarotQuestStats" } }) do
            local s = _G[e[2]]
            if s and s.n > 0 then
                f:write(string.format("%s step ms: max=%.1f avg=%.2f over %d ticks\n",
                    e[1], s.max, s.ms / s.n, s.n))
                _G[e[2]] = nil
            end
        end
        -- Subtitles-option probe (2026-07-16: user has subtitles OFF in the game
        -- options yet lines still read — the gate reads EnableSubtitle=1). Prints
        -- every non-CDO ATSaveSystem with a cross-checkable slice of Option (the
        -- user knows their real volume/vibration settings, so a SHIFTED layout
        -- names itself), plus the GameState's own subtitle-widget pointers
        -- (AT.hpp:14685 Subtitles 0x590 / InMenuSubtitles 0x598) and their render
        -- state — the data to pick the right gate. Take the dump DURING a cutscene
        -- with the option off.
        pcall(function()
            for _, o in pairs(FindAllOf("ATSaveSystem") or {}) do
                if Core.valid(o) then
                    local fn = "?"
                    pcall(function() fn = o:GetFullName() end)
                    local vals = {}
                    for _, k in ipairs({ "EnableSubtitle", "LanguageVoice", "VolumeBgm",
                                         "VolumeSe", "VolumeVoice", "VolumeMovie",
                                         "PadVibration", "HiddenMinimap" }) do
                        local v = "?"
                        pcall(function() v = tostring(o.Option[k]) end)
                        vals[#vals + 1] = k .. "=" .. v
                    end
                    f:write("savesys " .. fn .. "\n  " .. table.concat(vals, " ") .. "\n")
                end
            end
            -- Which save system does each SaveManager point to? (2026-07-17: the
            -- manager-resolved gate STILL read EnableSubtitle=1 while instance _4
            -- held the user's real 0 — either several managers exist too, or the
            -- pointer targets a template.)
            for _, m in pairs(FindAllOf("ATSaveManager") or {}) do
                if Core.valid(m) then
                    local fn, tgt = "?", "?"
                    pcall(function() fn = m:GetFullName() end)
                    pcall(function()
                        local s = m.SaveSystem
                        tgt = Core.valid(s) and s:GetFullName() or "null"
                    end)
                    f:write("savemgr " .. fn .. " -> " .. tgt .. "\n")
                end
            end
            -- Render state of every pooled subtitle widget (is pane_live the
            -- discriminator for the option-off state?).
            for _, w in pairs(FindAllOf("Xcmn_Subtitles_C") or {}) do
                if Core.valid(w) then
                    local wn, vis, op, onscr, lv = "?", "?", "?", "?", "?"
                    pcall(function() wn = w:GetFName():ToString() end)
                    pcall(function() vis = tostring(w:GetVisibility()) end)
                    pcall(function() op = string.format("%.2f", w:GetRenderOpacity()) end)
                    pcall(function() onscr = tostring(Core.on_screen(w)) end)
                    pcall(function() lv = tostring(Core.pane_live(w)) end)
                    f:write(string.format("subwidget %s vis=%s op=%s on=%s live=%s\n",
                        wn, vis, op, onscr, lv))
                end
            end
            local gs = FindFirstOf("ATGameState")
            for _, fld in ipairs({ "Subtitles", "InMenuSubtitles" }) do
                local line = fld .. "=unreadable"
                pcall(function()
                    local w = gs[fld]
                    if Core.valid(w) then
                        local wn, vis, op, onscr = "?", "?", "?", "?"
                        pcall(function() wn = w:GetFullName() end)
                        pcall(function() vis = tostring(w:GetVisibility()) end)
                        pcall(function() op = string.format("%.2f", w:GetRenderOpacity()) end)
                        pcall(function() onscr = tostring(Core.on_screen(w)) end)
                        line = string.format("%s: %s vis=%s op=%s on=%s", fld, wn, vis, op, onscr)
                    else
                        line = fld .. "=null/invalid"
                    end
                end)
                f:write("gamestate " .. line .. "\n")
            end
        end)
        -- Screen-directory trace (ui_directory): every mapped class, hop by hop. For a
        -- silent screen, open it FIRST and dump — the broken link names itself. Gated
        -- like the widget probes below: never during a transition.
        if not trans then
            pcall(function()
                f:write("screen directory:\n")
                for _, line in ipairs(require("ui_directory").debug_lines()) do
                    f:write(line .. "\n")
                end
            end)
            -- Visible-screen CENSUS: every currently ON-SCREEN text box, grouped by the
            -- blueprint/native widget ancestors that own it. This names the REAL class
            -- of a screen no adapter is detecting (FindAllOf on a native base can return
            -- NOTHING while the _C blueprint name works — the 2026-07-06 gotcha — so a
            -- silent screen may simply be scanned under the wrong name). Text boxes are
            -- the census anchor because FindAllOf("CFUIMultiLineTextBox") is proven to
            -- enumerate fully (screen_list / discover.lua rely on it).
            pcall(function()
                f:write("visible-screen census (owner chains of on-screen text):\n")
                -- Several anchors: the first census used only CFUIMultiLineTextBox and
                -- came back EMPTY with the items menu open and read aloud (2026-07-15) —
                -- either the native-name gotcha or screens built on the wrapper/plain
                -- text classes. Per-anchor totals tell which anchor sees anything at
                -- all; owner lines are written the moment a chain is first seen, so a
                -- mid-walk abort still leaves the partial census on disk.
                local seen = {}
                for _, anchor in ipairs({ "CFUIMultiLineTextBox", "CFUIXcmnMultiLineText",
                                          "Xcmn_MultiLineText_C", "TextBlock" }) do
                    -- STEP MARKERS (2026-08-15). This census died silently mid-run — the dump
                    -- ended right after the first anchor's summary with no actor section and no
                    -- gate line, and UE4SS.log carried "Tried calling a member function but the
                    -- UObject instance is nullptr", one of the errors that PIERCE pcall and
                    -- unwind past every guard here. The file is unbuffered (see the open above),
                    -- so whatever marker is last on disk is genuinely the last thing that ran:
                    -- these lines turn "it stopped somewhere in the census" into a named anchor
                    -- and a named object.
                    f:write("  [step] anchor " .. anchor .. " FindAllOf\n")
                    local all
                    pcall(function() all = FindAllOf(anchor) end)
                    f:write("  [step] anchor " .. anchor .. " walking\n")
                    local total, on = 0, 0
                    local walked = 0
                    for _, t in pairs(all or {}) do
                        -- One marker per STRIDE objects, not per object: a per-object line would
                        -- add thousands of lines to every healthy dump, and the stride still
                        -- narrows the killer to a short, named window.
                        walked = walked + 1
                        if walked % CENSUS_STEP_STRIDE == 0 then
                            local at = "?"
                            pcall(function() at = t:GetFullName() end)
                            f:write(("  [step] anchor %s at #%d %s\n"):format(anchor, walked, at))
                        end
                        if Core.valid(t) then
                            total = total + 1
                            local ok_os, os_r = pcall(Core.on_screen, t)
                            if ok_os and os_r then
                                on = on + 1
                                -- Owner = the full-name OUTER chain (GetParent walks only
                                -- slate panels and missed cross-UserWidget owners — the v2
                                -- census printed "(no owner)" for everything at the title).
                                -- The path up to .WidgetTree. IS the owning root widget.
                                local fn = "?"
                                pcall(function() fn = t:GetFullName() end)
                                local key = fn:match("^(.-)%.WidgetTree%.") or fn:sub(1, 100)
                                if not seen[key] then
                                    seen[key] = true
                                    -- GATED (2026-08-15). This was `t:GetText():ToString()` with
                                    -- a raw `t.Text` fallback, both merely pcall'd — and that is
                                    -- what killed the dump. The anchors are NOT the same shape:
                                    -- CFUIMultiLineTextBox is a real text box that declares
                                    -- `Text`, while CFUIXcmnMultiLineText is this game's WRAPPER,
                                    -- whose text lives in `mainTxt`. Fetching an undeclared
                                    -- member aborts uncatchably, so the wrapper anchor died on
                                    -- one of its first objects every single run — taking every
                                    -- actor section of the dump with it. Core.text_of is the
                                    -- substrate helper for exactly this wrapper; the strict
                                    -- `Text` fetch covers the plain nodes it does not know.
                                    -- STRICT ON BOTH CANDIDATES. The first cut of this fix used
                                    -- Core.text_of, which is the right helper for an ADAPTER —
                                    -- it fetches `mainTxt` NON-strict, i.e. fail-open, because
                                    -- its caller believes the member is there. This loop has the
                                    -- opposite contract: it walks thousands of heterogeneous
                                    -- classes and most of them are EXPECTED not to declare the
                                    -- name being tried, which is precisely where fail-open turns
                                    -- into a licence to make the uncatchable fetch — and the
                                    -- property-set budget is 1 class per tick, so during a walk
                                    -- this size almost everything is ungated. Both reads are
                                    -- therefore strict; a class whose set is not ready yet is
                                    -- skipped for this run rather than risked.
                                    local txt = Core.name_str(Core.member(t, "Text", true))
                                    if not txt or txt == "" then
                                        local m = Core.member(t, "mainTxt", true)
                                        if Core.valid(m) then
                                            txt = Core.name_str(Core.member(m, "Text", true))
                                        end
                                    end
                                    f:write(string.format("  %s   e.g. \"%s\"\n",
                                        key, tostring(txt or ""):sub(1, 40)))
                                end
                            end
                        end
                    end
                    f:write(string.format("  [anchor %s: total=%d on_screen=%d]\n",
                        anchor, total, on))
                end
            end)
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
        -- DOORS (2026-07-25). The user reports the picked exit sits exactly where the player is, so
        -- the target is useless as a "walk here" beacon. This section answers WHY without guessing.
        -- The suspicion: AATDoorVolume is a TriggerBox, and a volume large enough to cover the room
        -- has its ORIGIN near the middle of that room — i.e. near the player — while the door itself
        -- is at the edge. If that is what the numbers show, the right target is not the actor origin
        -- but `PlayerStartTransform` (@0x390), the spot the game itself places the player on when
        -- arriving through this door. FTransform is {FQuat Rotation 0x00, FVector Translation 0x10,
        -- FVector Scale3D 0x20} (size 0x30, which the header confirms), so the translation is at
        -- 0x390+0x10 = 0x3A0 — read NATIVELY through mem_bridge rather than as a nested struct
        -- property, because chained struct reads are a documented uncatchable abort here.
        pcall(function()
            local doors = FindAllOf("ATDoorVolume") or {}
            f:write(string.format("doors: n=%d (player at %.0f %.0f %.0f)\n", #doors, px, py, pz))
            for i, a in ipairs(doors) do
                if Core.valid(a) then
                    local ax, ay, az = actor_pos(a)
                    local nm = {}
                    for _, prop in ipairs({ "AreaName", "DoorName" }) do
                        local s
                        pcall(function()
                            local v = Core.member(a, prop)
                            if v then s = v:ToString() end
                        end)
                        nm[#nm + 1] = prop .. "=" .. tostring(s)
                    end
                    local sx = Mem.float(a, 0x3A0)
                    local sy = Mem.float(a, 0x3A4)
                    local sz = Mem.float(a, 0x3A8)
                    local d = ax and math.sqrt((ax - px) ^ 2 + (ay - py) ^ 2 + (az - pz) ^ 2) or -1
                    local room
                    pcall(function() room = tostring(a.bOnlyUsedInRoom) end)
                    f:write(string.format(
                        "  door[%d] d=%.0f origin=%s %s %s  playerStart=%s %s %s  onlyInRoom=%s %s\n",
                        i, d / 100, tostring(ax), tostring(ay), tostring(az),
                        tostring(sx), tostring(sy), tostring(sz), tostring(room),
                        table.concat(nm, " ")))
                end
            end
        end)
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
                        if Core.valid(comp) then raw = tonumber(comp.MapIconType) end
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
            local iconarr, cnt = Core.array_of(mm, "MapIconList")
            f:write("radar.MapIconList: " .. tostring(iconarr and cnt or "unreadable") .. "\n")
            pcall(function()
                local arr, n = Core.array_of(mm, "MapIconList")
                if not arr then return end
                local untyped = 0
                for i = 1, n do
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
                    local _, npts = Core.array_of(path, "PathPoints")
                    if npts then n2 = tostring(npts) end
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
                            if p2 then
                                local _, np2 = Core.array_of(p2, "PathPoints")
                                if np2 then c2 = tostring(np2) end
                            end
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
                if Core.valid(comp) then
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
