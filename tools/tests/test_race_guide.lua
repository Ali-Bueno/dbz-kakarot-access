-- Offline tests for race_guide.lua: the beacon aims at the HUD's next checkpoint, the counter
-- is spoken once per change, flickers and pausing screens are absorbed, and the quest radar
-- gets the beacon back when the race HUD goes away.
local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local fails = 0
local function check(ok, message)
    if ok then io.write("ok   - " .. message .. "\n")
    else fails = fails + 1; io.write("FAIL - " .. message .. "\n") end
end
local function module(name, value)
    package.preload[name] = function() return value end
    return value
end

local clock = 0
os.clock = function() return clock end

local hud = {
    valid = true, shown = true,
    next = { X = 1000, Y = 11000, Z = 0 },  -- 100 m to the camera's right
    player = { X = 1000, Y = 1000, Z = 0 },
    WL_Txt_Num_Cp = { valid = true, text = "0/8" },
    WL_Mgame_Race_List00 = { valid = true, shown = true,
        WL_Txt_Title = { text = "Time" }, WL_Txt_Num_Time = { text = "0'12.480" } },
    WL_Mgame_Race_List01 = { valid = true, shown = true,
        WL_Txt_Title = { text = "Time limit" }, WL_Txt_Num_Time = { text = "0'41.250" } },
}
function hud:GetNextCheckPointLocation() return self.next end
function hud:GetPlayerLocation() return self.player end
local yaw = 0
local cam = { valid = true }
function cam:GetCameraRotation() return { Yaw = yaw } end
local pc = { valid = true, PlayerCameraManager = cam }

local hosts = { hud }
local muted = false
local pings, said, cues = {}, {}, {}
local obstacle = nil          -- what the fake sensor reports: {pan, dist}
local ring = nil              -- what the fake ring picker reports: {pan, dist}
local tick_fn

module("ui_core", {
    peek_all = function(cls) return cls == "AT_UIMiniGameRace" and hosts or {} end,
    valid = function(o) return type(o) == "table" and o.valid == true end,
    on_screen = function(o) return o.shown == true end,
    pane_rendered = function() return true end,
    begin_scan_tick = function() end,
    member = function(o, name) return o and o[name] end,
    read_text = function(node) return node and node.text end,
    minimap_peek = function() return false end,
})
module("ui_directory", { peek = function(root, field)
    if root == "hud" and field == "PlayerOwner" then return pc end
end })
module("ui_registry", { mutes_audio = function() return muted end })
module("transition", { active = function() return false end })
module("audio", { ping = function(pan, vol, pitch)
    pings[#pings + 1] = { pan = pan, vol = vol, pitch = pitch, at = clock }
end, cue = function(name, pan, vol)
    cues[#cues + 1] = { name = name, pan = pan, vol = vol, at = clock }
end })
module("race_rings", {
    reset = function() end, refresh = function() end,
    pick = function() if ring then return ring[1], ring[2] end end,
    cadence = function() return 1.0, 300 end,
})
module("obstacle_sensor", {
    new_race = function() end,
    scan = function() if obstacle then return obstacle[1], obstacle[2] end return nil end,
    cadence = function() return 1.0, 200 end,
})
module("speech", { say = function(text, interrupt)
    said[#said + 1] = { text = text, interrupt = interrupt }
end })
module("i18n", { t = function(key)
    if key == "race_checkpoint" then return "Checkpoint %s" end
    if key == "race_count" then return "%d of %d" end
    if key == "nav_meters" then return "%d meters" end
    if key == "race_time_s" then return "%d seconds" end
    if key == "race_time_ms" then return "%d minutes %d seconds" end
    return key
end })
module("mem", { mark = function() end })
module("pad_poll", { register_every = function(name, _, fn)
    if name == "race" then tick_fn = fn end
end, unregister = function() end })
print = function() end   -- the guide's transition log lines

local Race = require("race_guide")
Race.start()

local function run(n)
    for _ = 1, n do
        clock = clock + 0.1
        tick_fn()
    end
end

do
    check(not Race.owns_audio(), "no race: the quest radar keeps the beacon")
    run(5)
    check(Race.owns_audio(), "the race HUD is found on the idle probe and takes the beacon")
    check(#said == 1 and said[1].text == "Checkpoint 0 of 8" and said[1].interrupt == false,
        "the opening counter is spoken without interrupting")
    check(#pings == 1 and pings[1].pan > 0.99 and pings[1].pitch == 1.0,
        "the beacon pings hard right for a checkpoint on the camera's right")
end

do
    hud.WL_Txt_Num_Cp.text = "1/8"
    run(1)
    check(#said == 2 and said[2].text == "Checkpoint 1 of 8" and said[2].interrupt == true,
        "a passed checkpoint is announced, interrupting")
    run(3)
    check(#said == 2, "an unchanged counter is not repeated")
end

do
    hud.shown = false
    run(3)
    check(Race.owns_audio(), "a short HUD flicker does not end the race")
    hud.shown = true
    run(1)
    check(#said == 2, "the counter is not re-read after a flicker")
end

do
    muted = true
    hud.WL_Txt_Num_Cp.text = "2/8"
    local before = #pings
    run(10)
    check(#pings == before and #said == 2, "a pausing screen silences pings and announcements")
    check(Race.owns_audio(), "the race keeps the beacon from the quest radar while paused")
    muted = false
    run(1)
    check(said[#said].text == "Checkpoint 2 of 8", "the counter missed while paused is read on return")
end

do
    yaw = 180
    hud.next = { X = 11000, Y = 1000, Z = 0 }
    pings = {}
    run(10)
    local p = pings[#pings]
    check(p and math.abs(p.pan) < 1e-6 and p.pitch < 1.0, "a checkpoint behind the camera pings centred, lower")
    yaw = 0
end

do
    pc.PlayerCameraManager = nil
    pings = {}
    run(10)
    check(#pings > 0 and pings[#pings].pan == 0, "without a camera reading the beacon still pings, centred")
    pc.PlayerCameraManager = cam
end

do
    hud.next = { X = 0, Y = 0, Z = 0 }
    pings = {}
    run(10)
    check(#pings == 0 and Race.owns_audio(), "no next checkpoint yet: silent, but the race still owns the beacon")
    hud.next = { X = 1000, Y = 11000, Z = 0 }
end

do
    check(Race.read_status(), "the on-demand status answers while racing")
    local s = said[#said]
    check(s.text == "Checkpoint 2 of 8, 100 meters, Time 12 seconds, Time limit 41 seconds"
        and s.interrupt == true,
        "the status reads checkpoint, distance and the visible timer: " .. s.text)
end

do
    hosts = {}
    run(20)
    check(not Race.owns_audio(), "the HUD gone past the grace period hands the beacon back")
    check(not Race.read_status(), "the on-demand status declines once the race is over")
    hosts = { hud }
    said = {}
    run(5)
    check(#said == 1 and said[1].text == "Checkpoint 2 of 8", "a new race reads the counter afresh")
end

do
    clock = clock + 5
    check(not Race.owns_audio(), "a stalled loop expires the claim instead of muting the radar forever")
end

do
    local limit = hud.WL_Mgame_Race_List01.WL_Txt_Num_Time
    said = {}
    limit.text = "0'09.900"
    run(10)
    check(#said == 1 and said[1].text == "9 seconds" and said[1].interrupt == false,
        "the time limit dropping under 10 seconds is announced once")
    limit.text = "0'05.100"
    run(10)
    check(#said == 1, "the low-time warning is not repeated in the same segment")
    limit.text = "0'31.000"
    run(10)
    limit.text = "0'08.000"
    run(10)
    check(#said == 2 and said[2].text == "8 seconds", "a checkpoint that tops the limit up re-arms the warning")
    limit.text = "0'41.250"
end

do
    local pawn = { valid = true }
    function pawn:K2_GetActorRotation() return { Yaw = 90 } end
    pc.Pawn = pawn
    pings = {}
    run(10)
    local p = pings[#pings]
    check(p and math.abs(p.pan) < 1e-6 and p.pitch == 1.0,
        "the racer's own heading wins over the camera: facing +Y puts the checkpoint ahead")
    pc.Pawn = nil
end

do
    pings, cues = {}, {}
    hud.next = { X = 1000, Y = 2000, Z = 0 }   -- 10 m: the beacon pings every 140 ms or so
    obstacle = { -0.5, 800 }
    run(10)
    local walls = 0
    for _, c in ipairs(cues) do if c.name == "enemy" then walls = walls + 1 end end
    check(walls >= 4 and cues[1].pan == -0.5, "an obstacle on the left cues the danger sound from the left")
    check(pings[1].vol > 1.0, "the race beacon is lifted above the quest radar's level")
    obstacle, cues = nil, {}
    run(2)
    obstacle = { 0.4, 900 }
    run(1)
    obstacle = nil
    run(3)
    check(#cues == 0, "a one-scan blip (a terrain bump) is not cued")
    ring = { 0.3, 2000 }
    run(10)
    local rings, clash = 0, false
    for _, c in ipairs(cues) do
        if c.name == "item" then
            rings = rings + 1
            for _, p in ipairs(pings) do if p.at == c.at then clash = true end end
        end
    end
    check(rings >= 2 and not clash, "a ring ahead is cued, never on the beacon's own tick")
    cues = {}
    obstacle = { 0.0, 1000 }
    run(10)
    local any_ring = false
    for _, c in ipairs(cues) do if c.name == "item" then any_ring = true end end
    check(not any_ring, "rings go quiet while an obstacle is close")
    obstacle, ring = nil, nil
    hud.next = { X = 1000, Y = 11000, Z = 0 }
end

-- The spacing contract: ring and obstacle cues only in the gaps between beacon pings, never two
-- of them back to back, and both still heard when both are due.
local function spacing(min_after_ping)
    local bad, prev_ping = nil, nil
    for _, c in ipairs(cues) do
        for _, p in ipairs(pings) do
            if math.abs(c.at - p.at) < 0.1 - 1e-6 then bad = "on a ping" end
            if p.at <= c.at and (not prev_ping or p.at > prev_ping) then prev_ping = p.at end
        end
        if prev_ping and c.at - prev_ping < min_after_ping - 1e-6 then bad = "too soon after a ping" end
        prev_ping = nil
    end
    for i = 2, #cues do
        if cues[i].at - cues[i - 1].at < 0.2 - 1e-6 then bad = "two cues back to back" end
    end
    return bad
end

do
    hud.next = { X = 1000, Y = 16000, Z = 0 }     -- 150 m: the beacon pings every ~700 ms
    obstacle, ring = { -0.4, 3000 }, { 0.3, 2500 }
    pings, cues = {}, {}
    run(60)
    local walls, rings = 0, 0
    for _, c in ipairs(cues) do
        if c.name == "enemy" then walls = walls + 1 elseif c.name == "item" then rings = rings + 1 end
    end
    local bad = spacing(0.2)
    check(not bad, "far checkpoint: secondary cues wait 200 ms after a ping and keep clear of the next ("
        .. tostring(bad) .. ")")
    check(walls >= 3 and rings >= 3, "an obstacle 30 m off and a ring both get turns: "
        .. walls .. " obstacle, " .. rings .. " ring cues")
    check(#pings >= 7, "the beacon keeps its own cadence: " .. #pings .. " pings in 6 s")

    hud.next = { X = 1000, Y = 2000, Z = 0 }      -- 10 m: pings every 200 ms
    pings, cues = {}, {}
    run(30)
    bad = spacing(0.1)
    check(not bad and #cues > 0, "close to a checkpoint: cues take the one tick between pings ("
        .. tostring(bad) .. ")")

    hud.next = { X = 1000, Y = 1300, Z = 0 }      -- 3 m: a ping every tick
    pings, cues = {}, {}
    run(10)
    check(#cues == 0 and #pings == 10, "at the checkpoint the beacon has every tick to itself")
    obstacle, ring = nil, nil
    hud.next = { X = 1000, Y = 11000, Z = 0 }
end

do
    check(Race.timer_seconds("0'42.803") == 42, "the HUD timer is read as whole seconds")
    check(Race.timer_seconds("1'05.000") == 65, "minutes count in the timer")
    check(Race.timer_seconds("00:41") == nil, "a foreign timer shape is not guessed at")
    check(Race.format_seconds(65) == "1 minutes 5 seconds", "over a minute reads minutes and seconds")
    check(Race.format_count("03/08") == "3 of 8", "a zero-padded counter is read as numbers")
    check(Race.format_count("05") == "5", "a lone zero-padded number loses its zeros")
    check(Race.format_count("10") == "10", "a real zero is kept")
end

if fails > 0 then
    io.write(fails .. " failure(s)\n")
    os.exit(1)
end
io.write("all race guide tests passed\n")
