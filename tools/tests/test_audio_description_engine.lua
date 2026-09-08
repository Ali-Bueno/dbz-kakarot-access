-- Offline regression tests for the cutscene audio-description scheduler.
--
-- The engine is intentionally pure Lua: the live adapter supplies the current
-- movie/sequence name and playback clock, while these tests pin the priority
-- rules that keep descriptions from talking over dialogue or replaying stale
-- action after a skip.

local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local Engine = require("audio_description_engine")

local fails = 0
local function check(cond, what)
    if cond then
        print("ok   - " .. what)
    else
        fails = fails + 1
        print("FAIL - " .. what)
    end
end

local CATALOG = {
    C01_000_S010_MOV = {
        { at = 0.20, text = "First action" },
        { at = 1.00, text = "Second action" },
        { at = 3.00, text = "Third action" },
    },
    C01_010_S010_GDM = {
        { at = 0.25, text = "A different scene" },
    },
}

local function harness()
    local spoken = {}
    local dialogue = false
    local engine = Engine.new(CATALOG, function(text, interrupt, no_requeue)
        spoken[#spoken + 1] = {
            text = text,
            interrupt = interrupt,
            no_requeue = no_requeue,
        }
    end, function()
        return dialogue
    end)
    return engine, spoken, function(v) dialogue = v end
end

check(Engine.normalize_source("ManaMovie /Game/Art/DEMO/C01/000/S010/mov/C01_000_S010_mov")
        == "C01_000_S010_MOV",
    "normalizes a Mana movie object path")
check(Engine.normalize_source("LevelSequence C01_010_S010_gdm_Master")
        == "C01_010_S010_GDM",
    "normalizes an in-engine sequence path")
check(Engine.normalize_source("unrelated title movie") == nil,
    "ignores sources without a catalog-style demo id")

do
    local engine, spoken = harness()
    engine:update("C01_000_S010_mov", 0.00, true, true)
    engine:update("C01_000_S010_mov", 0.21, true, true)
    engine:update("C01_000_S010_mov", 0.30, true, true)
    check(#spoken == 1 and spoken[1].text == "First action",
        "fires a crossed cue exactly once")
    check(spoken[1] and spoken[1].interrupt == false and spoken[1].no_requeue == true,
        "queues descriptions as non-interrupting and disposable")
end

do
    local engine, spoken, set_dialogue = harness()
    engine:update("C01_000_S010_mov", 0.00, true, true)
    set_dialogue(true)
    engine:update("C01_000_S010_mov", 0.21, true, true)
    set_dialogue(false)
    engine:update("C01_000_S010_mov", 0.50, true, true)
    check(#spoken == 0,
        "drops a cue that collides with dialogue instead of replaying it later")
end

do
    local engine, spoken = harness()
    engine:update("C01_000_S010_mov", 0.00, true, true)
    engine:update("C01_000_S010_mov", 2.70, true, true)
    check(#spoken == 0,
        "does not dump missed descriptions after a seek or skipped section")
    engine:update("C01_000_S010_mov", 3.02, true, true)
    check(#spoken == 1 and spoken[1].text == "Third action",
        "continues with the next timely cue after a seek")
end

do
    local engine, spoken = harness()
    engine:update("C01_000_S010_mov", 0.00, true, true)
    engine:update("C01_000_S010_mov", 0.21, true, true)
    engine:update("C01_000_S010_mov", 1.20, true, true)
    engine:update("C01_000_S010_mov", 0.00, true, true)
    engine:update("C01_000_S010_mov", 0.21, true, true)
    check(#spoken == 3 and spoken[3].text == "First action",
        "rewinding or restarting a scene re-arms its cues")
end

do
    local engine, spoken = harness()
    engine:update("C01_000_S010_mov", 0.00, true, true)
    engine:update("C01_000_S010_mov", 0.30, true, false)
    engine:update("C01_000_S010_mov", 0.31, true, true)
    engine:update("C01_000_S010_mov", 1.01, true, true)
    check(#spoken == 1 and spoken[1].text == "Second action",
        "disabled descriptions stay consumed and do not burst on when enabled")
end

do
    local engine, spoken = harness()
    engine:update("C01_000_S010_mov", 0.00, true, true)
    engine:update("C01_000_S010_mov", 0.21, true, true)
    engine:update("C01_010_S010_gdm_Master", 0.00, true, true)
    engine:update("C01_010_S010_gdm_Master", 0.26, true, true)
    check(#spoken == 2 and spoken[2].text == "A different scene",
        "a source change resets scheduling for the new scene")
end

if fails > 0 then
    print(("%d check(s) FAILED"):format(fails))
    os.exit(1)
end
print("all checks passed")
