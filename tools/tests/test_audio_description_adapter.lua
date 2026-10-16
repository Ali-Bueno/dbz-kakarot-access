-- Offline UE4SS-adapter regression tests. Fake UObjects exercise the exact
-- reflected methods used by the live movie and sequence clocks.

local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local movie_actors = {}
local sequence_actors = {}
local registered
local enabled = true
local in_transition = false
local roaming = false
local in_battle = false
local dialogue = false
local spoken = {}
local probes = 0
local movie_probes = 0

local function stub(name, members)
    package.preload[name] = function() return members end
end

stub("ui_core", {
    cached_all = function(class_name)
        probes = probes + 1
        if class_name == "PreRenderDemoActor" then
            movie_probes = movie_probes + 1
            return movie_actors
        end
        if class_name == "LevelSequenceActor" then return sequence_actors end
        error("unexpected class scan: " .. tostring(class_name))
    end,
    free_roam = function() return roaming end,
    battle_hud_peek = function() return in_battle end,
    valid = function(o) return type(o) == "table" and o.valid ~= false end,
    member = function(o, name) return o[name] end,
})
stub("speech", {
    say = function(text, interrupt, no_requeue)
        spoken[#spoken + 1] = { text, interrupt, no_requeue }
    end,
})
-- Mirrors I18n.t: a key with no entry in the active language answers the key itself.
local translations = {}
stub("i18n", {
    t = function(key) return translations[key] or key end,
})
stub("screen_dialogue", {
    is_active = function() return dialogue end,
})
stub("settings", {
    get = function(key)
        assert(key == "audio_descriptions", "adapter asked for an unexpected setting")
        return enabled
    end,
    -- Mirrors settings.lua: the adapter reads the toggle through this accessor.
    descriptions_enabled = function() return enabled ~= false end,
})
stub("transition", {
    active = function() return in_transition end,
})
-- The demo manager's pointer path (cinema.lua). `cinema_known = false` is the title-screen
-- case, where the adapter must fall back to the class scan.
local cinema_player, cinema_known = nil, false
stub("cinema", {
    movie = function() return cinema_player, cinema_known end,
})
stub("pad_poll", {
    register_every = function(name, period, fn)
        registered = { name = name, period = period, fn = fn }
    end,
    unregister = function(name)
        if registered and registered.name == name then registered = nil end
    end,
})

local Description = require("audio_description")

local fails = 0
local function check(cond, what)
    if cond then
        print("ok   - " .. what)
    else
        fails = fails + 1
        print("FAIL - " .. what)
    end
end

local function named(full_name)
    return { GetFullName = function() return full_name end }
end

Description.start()
check(registered and registered.name == "audio_description" and registered.period == 100,
    "registers one 100 ms step on the shared game-thread tick bus")

do
    local clock = 0
    local source = named("ManaMovie /Game/Art/DEMO/C01/000/S010/mov/C01_000_S010_mov")
    local player = {
        IsPlaying = function() return true end,
        GetTime = function() return clock end,
        GetSource = function() return source end,
    }
    movie_actors = { { ManaPlayer = player } }
    sequence_actors = {}
    registered.fn()
    clock = 0.11
    registered.fn()
    check(#spoken == 1 and spoken[1][1] == "A bamboo forest.",
        "reads the exact Mana playback clock and emits the matching movie cue")
    check(spoken[1] and spoken[1][2] == false and spoken[1][3] == true,
        "preserves non-interrupting, no-requeue speech priority")
end

do
    -- The same cue in a language that carries the line under its lang-file key.
    Description.reset()
    spoken = {}
    translations["ad_C01_000_S010_MOV_10"] = "Un bosque de bambú."
    local clock = 0
    local source = named("ManaMovie /Game/Art/DEMO/C01/000/S010/mov/C01_000_S010_mov")
    movie_actors = { { ManaPlayer = {
        IsPlaying = function() return true end,
        GetTime = function() return clock end,
        GetSource = function() return source end,
    } } }
    sequence_actors = {}
    registered.fn()
    clock = 0.11
    registered.fn()
    check(#spoken == 1 and spoken[1][1] == "Un bosque de bambú.",
        "speaks the localized line when the active language carries the cue's key")
    translations["ad_C01_000_S010_MOV_10"] = nil
end

do
    Description.reset()
    spoken = {}
    movie_actors = {}
    local clock = 0
    local actor = named("LevelSequenceActor /Game/Art/DEMO/C01/020/S010/gdm/seq/C01_020_S010_gdm_Master")
    actor.SequencePlayer = {
        IsPlaying = function() return true end,
        GetPlaybackPosition = function() return clock end,
    }
    sequence_actors = { actor }
    registered.fn()
    clock = 0.26
    registered.fn()
    check(#spoken == 1 and spoken[1][1]:match("Flying Nimbus"),
        "falls back to the master LevelSequence clock for an in-engine demo")
end

do
    Description.reset()
    spoken = {}
    local clock = 0
    -- The cooked C01_010_S020 level names its only sequence actor with a
    -- truncated "Maste" (sic, ObjectDump): mastership must come from the
    -- correctly spelled sequence ASSET, not the actor label.
    local actor = named("LevelSequenceActor /Game/Maps/Area03/DEMO/gdm/C01_010_S020_gdm.C01_010_S020_gdm:PersistentLevel.C01_010_S020_gdm_Maste")
    actor.SequencePlayer = {
        IsPlaying = function() return true end,
        GetPlaybackPosition = function() return clock end,
        Sequence = named("LevelSequence /Game/Art/DEMO/C01/010/S020/gdm/seq/C01_010_S020_gdm_Master.C01_010_S020_gdm_Master"),
    }
    sequence_actors = { actor }
    registered.fn()
    clock = 0.26
    registered.fn()
    check(#spoken == 1 and spoken[1][1]:match("open sky"),
        "identifies the master by its sequence asset when the cooked actor label is truncated")
end

do
    Description.reset()
    spoken = {}
    local clock = 0
    -- An in-pattern demo without a catalog entry must stay silent and leave a
    -- log line instead of erroring or speaking.
    local actor = named("LevelSequenceActor /Game/Maps/Test/DEMO/gdm/C01_010_S999_gdm.C01_010_S999_gdm:PersistentLevel.C01_010_S999_gdm_Master")
    actor.SequencePlayer = {
        IsPlaying = function() return true end,
        GetPlaybackPosition = function() return clock end,
    }
    sequence_actors = { actor }
    registered.fn()
    clock = 0.40
    registered.fn()
    check(#spoken == 0,
        "stays silent on an in-pattern demo with no catalog entry")
end

do
    Description.reset()
    spoken = {}
    enabled = false
    local clock = 0
    local source = named("ManaMovie C01_000_S010_mov")
    movie_actors = { { ManaPlayer = {
        IsPlaying = function() return true end,
        GetTime = function() return clock end,
        GetSource = function() return source end,
    } } }
    sequence_actors = {}
    local before = probes
    registered.fn()
    clock = 5.0
    registered.fn()
    check(probes == before and #spoken == 0,
        "turned off, the adapter probes nothing (no scans) and says nothing")
    enabled = true
    registered.fn()
    check(#spoken == 0, "re-enabled mid-scene, a cue that passed while off is not replayed")
end

do
    -- In a gameplay world the demo manager answers by pointer: no class scan at all,
    -- whether a movie is playing or not.
    Description.reset()
    spoken = {}
    local clock = 0
    local source = named("ManaMovie /Game/Art/DEMO/C01/000/S010/mov/C01_000_S010_mov")
    cinema_known = true
    cinema_player = {
        IsPlaying = function() return true end,
        GetTime = function() return clock end,
        GetSource = function() return source end,
    }
    movie_actors, sequence_actors = {}, {}
    local before = probes
    registered.fn()
    clock = 0.11
    registered.fn()
    check(probes == before and #spoken == 1 and spoken[1][1] == "A bamboo forest.",
        "reads the movie through the demo manager without scanning")
    cinema_player = nil
    local movie_before = movie_probes
    registered.fn()
    check(movie_probes == movie_before,
        "no movie and a reachable demo manager: the movie class is never scanned")
    cinema_known = false
end

do
    -- A playing movie with no cues still owns the scene: the sequence probe, and the
    -- absent-class scan it would cost under a movie, stays out of the way.
    Description.reset()
    spoken = {}
    local source = named("ManaMovie /Game/Art/DEMO/C09/000/S010/mov/C09_000_S010_mov")
    cinema_known = true
    cinema_player = {
        IsPlaying = function() return true end,
        GetTime = function() return 1.0 end,
        GetSource = function() return source end,
    }
    local before = probes
    registered.fn()
    check(probes == before and #spoken == 0,
        "an uncatalogued movie does not fall through to the sequence scan")
    cinema_player, cinema_known = nil, false
end

do
    Description.reset()
    spoken = {}
    in_transition = true
    registered.fn()
    in_transition = false
    check(#spoken == 0, "stays inert while the world-transition gate is active")
end

do
    Description.reset()
    spoken = {}
    roaming = true
    local before = probes
    registered.fn()
    roaming = false
    check(probes == before and #spoken == 0,
        "free roam skips the demo class probes entirely (no absent-scan tax)")
end

-- A missing active clock is not evidence that the scene restarted. The old
-- adapter reset its consumed-cue history here and repeated the same timely line
-- after pausing or one unreadable poll. Keep the real adapter and engine coupled.
for _, gap in ipairs({ "paused", "source", "clock", "actors" }) do
    Description.reset()
    spoken = {}
    local missing = false
    local source = named("ManaMovie C01_000_S010_mov")
    local actors = { { ManaPlayer = {
        IsPlaying = function() return not (missing and gap == "paused") end,
        GetTime = function()
            if missing and gap == "clock" then error("unreadable clock") end
            return 0.11
        end,
        GetSource = function()
            if missing and gap == "source" then error("unreadable source") end
            return source
        end,
    } } }
    movie_actors, sequence_actors = actors, {}
    registered.fn()
    missing = true
    if gap == "actors" then movie_actors = {} end
    registered.fn()
    missing, movie_actors = false, actors
    registered.fn()
    check(#spoken == 1 and spoken[1][1] == "A bamboo forest.",
        "does not repeat a consumed movie cue after a " .. gap .. " gap")
end

do
    Description.reset()
    spoken = {}
    local playing = true
    local actor = named("LevelSequenceActor C01_020_S010_gdm_Master")
    actor.SequencePlayer = {
        IsPlaying = function() return playing end,
        GetPlaybackPosition = function() return 0.26 end,
    }
    movie_actors, sequence_actors = {}, { actor }
    registered.fn()
    playing = false
    registered.fn()
    playing = true
    registered.fn()
    check(#spoken == 1 and spoken[1][1]:match("Flying Nimbus"),
        "does not repeat a consumed master-sequence cue after pause/resume")

    local before_transition = #spoken
    in_transition = true
    registered.fn()
    in_transition = false
    registered.fn()
    check(#spoken == before_transition + 1, "a positive world transition still resets scene history")
    local before_roam = #spoken
    roaming = true
    registered.fn()
    roaming = false
    registered.fn()
    check(#spoken == before_roam + 1, "returning to free roam still resets scene history")
end

do
    Description.reset()
    spoken = {}
    in_battle = true
    movie_actors, sequence_actors = {}, {}
    local before = probes - movie_probes
    registered.fn()
    in_battle = false
    check(probes - movie_probes == before, "the live battle HUD keeps the sequence probe out of fights")
end

Description.stop()
check(registered == nil, "unregisters cleanly on hot reload or shutdown")

if fails > 0 then
    print(("%d check(s) FAILED"):format(fails))
    os.exit(1)
end
print("all checks passed")
