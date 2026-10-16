-- Scan-free movie detection through the game's own demo manager.
--
-- UAT_BlueprintFunctionLibrary::GetDemoManageComponent -> GetPreRenderDemoCtrl -> DemoActor
-- (AT.hpp:29950, 28163, 29221) -> ManaPlayer (AT.hpp:16125). Two reflected calls and two
-- pointer hops, no object-array walk. Evidence: reference/dbz-kakarot/notes/dbz-kakarot-movie-stutter.md.

local Core = require("ui_core")
local Transition = require("transition")
local Dir = require("ui_directory")

local Cinema = {}

local LIB_PATH = "/Script/AT.Default__AT_BlueprintFunctionLibrary"
-- Both callers (the registry and the description clock) poll on the same 100 ms dispatch.
local MEMO_S = 0.05
-- A movie can read "not playing" for a poll at a loop point or a pause flicker; the off edge
-- releases every deferred scan at once, so it must persist before it counts.
local OFF_AFTER_S = 0.5
-- Where the manager cannot be reached (title, boot), ask again at this pace, not every poll.
local RESOLVE_RETRY_S = 1.0

local lib = nil          -- nil = not looked up, false = unavailable this session
local ctrl = nil         -- cached UATPreRenderDemoCtrl (per world)
local next_resolve = 0
local memo_at, memo_player, memo_known = -1, nil, false
local on, off_since = false, nil

local function library()
    if lib == nil then
        local ok, l = pcall(function() return StaticFindObject(LIB_PATH) end)
        lib = (ok and Core.valid(l)) and l or false
        if not lib then print("[KakarotAccess] cinema: demo library unavailable\n") end
    end
    return lib or nil
end

local function prerender_ctrl()
    if Core.valid(ctrl) then return ctrl end
    ctrl = nil
    local now = os.clock()
    if now < next_resolve then return nil end
    next_resolve = now + RESOLVE_RETRY_S
    local l = library()
    if not l then return nil end
    local comp, c
    pcall(function() comp = l:GetDemoManageComponent() end)
    if not Core.valid(comp) then return nil end
    pcall(function() c = comp:GetPreRenderDemoCtrl() end)
    if not Core.valid(c) then return nil end
    ctrl = c
    return c
end

local function probe()
    -- The getter reads a global the gameplay world sets (code/decompiled/_democtrl_*.c); only
    -- trust it while that world's MenuManager is alive, never at the title or across a load.
    if not Dir.root_cached("mm") then return nil, false end
    local c = prerender_ctrl()
    if not c then return nil, false end
    local actor = Core.member(c, "DemoActor")
    if not Core.valid(actor) then return nil, true end
    local player = Core.member(actor, "ManaPlayer")
    if not Core.valid(player) then return nil, true end
    local ok, playing = pcall(function() return player:IsPlaying() end)
    if ok and playing == true then return player, true end
    return nil, true
end

-- The ManaComponent of the movie playing right now (or nil), and whether the demo manager
-- could be reached at all: `known == false` means "could not look", never "no movie".
function Cinema.movie()
    if Transition.active() then return nil, false end
    local now = os.clock()
    if now - memo_at < MEMO_S then return memo_player, memo_known end
    memo_at = now
    memo_player, memo_known = probe()
    return memo_player, memo_known
end

-- Debounced "a movie owns the screen" for the scan gate; logs each real flip.
function Cinema.update()
    local playing = Cinema.movie() ~= nil
    local now = os.clock()
    if playing then
        off_since = nil
        if not on then
            on = true
            print("[KakarotAccess] cinema -> movie\n")
        end
    elseif on then
        off_since = off_since or now
        if now - off_since >= OFF_AFTER_S then
            on, off_since = false, nil
            print("[KakarotAccess] cinema -> none\n")
        end
    end
    return on
end

function Cinema.active() return on end

Transition.on_begin("cinema", function()
    ctrl, next_resolve = nil, 0
    memo_at, memo_player, memo_known = -1, nil, false
    on, off_since = false, nil
end)

return Cinema
