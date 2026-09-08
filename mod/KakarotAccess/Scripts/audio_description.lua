-- UE4SS adapter for timed cinematic audio descriptions.
--
-- Prerecorded cinematics use CriWare's reflected UManaComponent clock. Real-
-- time cinematics use the master ALevelSequenceActor's sequence player clock.
-- Both paths were confirmed in the installed headers/native registration data;
-- no OCR, video upload, network service or guessed wall-clock timer is involved.

local Core = require("ui_core")
local Speech = require("speech")
local Dialogue = require("screen_dialogue")
local Settings = require("settings")
local Transition = require("transition")
local Poll = require("pad_poll")
local Cues = require("audio_description_cues")
local Engine = require("audio_description_engine")

local Description = {}
local STEP_NAME = "audio_description"
local STEP_MS = 100
local running = false
local logged = {}

-- An in-pattern demo with no catalog entry must be VISIBLE in the log: to a player a
-- silently skipped scene reads exactly like a broken one, and this line is what turns
-- "scene X said nothing" into an authorable key. Once per key per session.
local function log_no_cues(key)
    if logged[key] then return end
    logged[key] = true
    print(string.format("[KakarotAccess] audio description: no cues for %s\n", key))
end

local engine = Engine.new(Cues,
    function(text, interrupt, no_requeue)
        Speech.say(text, interrupt, no_requeue)
    end,
    function()
        -- This performs a fresh, guarded surface read only when a cue is due.
        -- A visible subtitle/talk window wins and permanently consumes the cue.
        return Dialogue.is_active()
    end)

local function object_name(o)
    if not Core.valid(o) then return nil end
    local s
    if pcall(function() s = o:GetFullName() end) and type(s) == "string" and s ~= "" then
        return s
    end
    if pcall(function() s = o:GetFName():ToString() end)
        and type(s) == "string" and s ~= "" then
        return s
    end
    return nil
end

local function movie_clock()
    local actors = Core.cached_all("PreRenderDemoActor")
    for i = 1, #actors do
        local actor = actors[i]
        if Core.valid(actor) then
            -- Exact property on APreRenderDemoActor @ 0x358 (AT.hpp).
            local player = Core.member(actor, "ManaPlayer")
            if Core.valid(player) then
                local ok_play, playing = pcall(function() return player:IsPlaying() end)
                if ok_play and playing == true then
                    local source, seconds
                    local ok_source = pcall(function() source = player:GetSource() end)
                    local ok_time = pcall(function() seconds = player:GetTime() end)
                    local name = ok_source and object_name(source) or nil
                    local key = Engine.normalize_source(name)
                    if key and Cues[key] and ok_time and type(seconds) == "number" then
                        return name, seconds, true, key
                    end
                    if key and not Cues[key] then log_no_cues(key) end
                end
            end
        end
    end
    return nil
end

local function sequence_clock()
    local actors = Core.cached_all("LevelSequenceActor")
    for i = 1, #actors do
        local actor = actors[i]
        if Core.valid(actor) then
            local name = object_name(actor)
            -- Any actor inside a demo level normalizes through the LEVEL package in
            -- its full name, so this is a cheap in-scope filter, not a master test.
            local key = name and Engine.normalize_source(name)
            if key and not Cues[key] then
                log_no_cues(key)
            elseif key and key:find("_GDM", 1, true) then
                -- Exact property on ALevelSequenceActor @ 0x388 (LevelSequence.hpp);
                -- methods are on the UMovieSceneSequencePlayer base.
                local player = Core.member(actor, "SequencePlayer")
                if Core.valid(player) then
                    -- Only the master owns the continuous scene clock the authored
                    -- GDM cue table uses; Event/Sound subsequences restart at zero.
                    -- Mastership comes from the sequence ASSET name (`Sequence`,
                    -- reflected on UMovieSceneSequencePlayer @ 0x3D0), NOT from the
                    -- actor label: the cooked C01_010_S020 level names its only
                    -- sequence actor "C01_010_S020_gdm_Maste" (sic, ObjectDump), so
                    -- an actor-name match silences that whole scene. The actor name
                    -- stays as the fallback when the asset is unreadable.
                    local master = object_name(Core.member(player, "Sequence")) or name
                    if master:upper():find("_GDM_MASTER", 1, true) then
                        local ok_play, playing = pcall(function() return player:IsPlaying() end)
                        if ok_play and playing == true then
                            local seconds
                            local ok_time = pcall(function()
                                seconds = player:GetPlaybackPosition()
                            end)
                            if ok_time and type(seconds) == "number" then
                                return name, seconds, true, key
                            end
                        end
                    end
                end
            end
        end
    end
    return nil
end

function Description.reset()
    engine:reset()
end

function Description.step()
    if Transition.active() then
        engine:reset()
        return
    end

    -- A demo only runs once the game has taken the screen over (minimap down), so
    -- free roam skips both clock probes. This is the playbook's cheap-precondition
    -- rule for naming scan classes: without it the two demo classes sit in the
    -- absent-scan rotation for the whole overworld — the documented stutter tax.
    if Core.free_roam() then
        engine:reset()
        return
    end

    -- Prefer Mana: a CDM master sequence can run alongside the prerecorded
    -- movie it presents. Choosing its Mana player prevents two clocks from
    -- describing the same shot.
    local source, seconds, playing, key = movie_clock()
    if not source then source, seconds, playing, key = sequence_clock() end

    if key and not logged[key] then
        logged[key] = true
        print(string.format("[KakarotAccess] audio description source: %s\n", key))
    end

    engine:update(source, seconds, playing, Settings.get("audio_descriptions") ~= false)
end

function Description.start()
    if running then return end
    running = true
    Poll.register_every(STEP_NAME, STEP_MS, Description.step, Description.reset)
end

function Description.stop()
    if not running then return end
    running = false
    Poll.unregister(STEP_NAME)
    engine:reset()
end

return Description
