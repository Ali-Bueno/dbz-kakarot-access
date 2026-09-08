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
local dialogue = false
local spoken = {}
local probes = 0

local function stub(name, members)
    package.preload[name] = function() return members end
end

stub("ui_core", {
    cached_all = function(class_name)
        probes = probes + 1
        if class_name == "PreRenderDemoActor" then return movie_actors end
        if class_name == "LevelSequenceActor" then return sequence_actors end
        error("unexpected class scan: " .. tostring(class_name))
    end,
    free_roam = function() return roaming end,
    valid = function(o) return type(o) == "table" and o.valid ~= false end,
    member = function(o, name) return o[name] end,
})
stub("speech", {
    say = function(text, interrupt, no_requeue)
        spoken[#spoken + 1] = { text, interrupt, no_requeue }
    end,
})
stub("screen_dialogue", {
    is_active = function() return dialogue end,
})
stub("settings", {
    get = function(key)
        assert(key == "audio_descriptions", "adapter asked for an unexpected setting")
        return enabled
    end,
})
stub("transition", {
    active = function() return in_transition end,
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
    registered.fn()
    clock = 0.20
    registered.fn()
    enabled = true
    registered.fn()
    check(#spoken == 0, "honors the live audio-description setting without a backlog")
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

Description.stop()
check(registered == nil, "unregisters cleanly on hot reload or shutdown")

if fails > 0 then
    print(("%d check(s) FAILED"):format(fails))
    os.exit(1)
end
print("all checks passed")
