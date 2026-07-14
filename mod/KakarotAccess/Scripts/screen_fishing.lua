-- Screen adapter: fishing minigame + generic minigame/QTE button prompts.
--
-- A blind player cannot see WHICH button the minigame demands, so this adapter
-- speaks it the moment it appears. Surfaces (from the CXX header dump):
--   * AT_UIMgameFishing   — the fishing minigame HUD: instruction text
--       (UIMgameTips/UIMgameTips01 -> TextBox_Tips) + the action button glyph
--       (Xcmn_Btn_Plat_00) + four configured FUIXcmnPlatBtnTypeInfo ActionBtns.
--   * AT_UIQteMashAlert   — the "mash this button" QTE alert (WL_Btn glyph).
--       Shared with other minigames/QTEs, so those get announced too.
--   * AT_UIMiniGamePop    — minigame popup banners (TextTitle/TextSub).
--
-- Buttons are resolved SEMANTICALLY (A.platbtn_name on the live glyph, then the
-- widget's ActionBtn00 config struct), never guessed from textures.
--
-- SELF-DIAGNOSIS (temporary, until the timing cue ships): the fishing HUD's live
-- cursor value ("aim for the center") is NOT reflected — it lives in the widget's
-- non-UPROPERTY tail (0x508..0x528, class size 0x528, last reflected member ends
-- at 0x508). While the minigame runs we append tail samples + button diagnostics
-- to dumps/dump_fishing.txt so the cursor offset can be pinned offline and turned
-- into an audio timing cue.

local Core = require("ui_core")
local Speech = require("speech")
local A = require("ui_archetypes")
local I18n = require("i18n")
local Mem = require("mem")
local Audio = require("audio")
local OFF = require("native_offsets")

local Fishing = {}

-- Prompts are time-critical: commit to this screen on the FIRST stable poll
-- (the default 2-tick debounce added ~200 ms before anything could speak).
Fishing.confirm_ticks = 1

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
Fishing.keyhelp_auto = false

-- Re-enable to append tail samples + button internals to dumps/dump_fishing.txt
-- (how the fishing offsets in native_offsets.lua were found). OFF since the full
-- flow was verified end-to-end in-game (user landed a fish, 2026-07-03).
local DEBUG = false

-- ACCESSIBILITY ASSIST (user request): the hook window is brutally short for
-- ear-driven play — the bar appears the instant the fish bites and allows only a
-- few cursor bounces. CursorSpeed / CursorLapLimit are REFLECTED and writable, so
-- each new attempt is slowed to half speed with twice the bounces. Gameplay stays
-- the same mechanic, just paced for audio reaction time.
local ASSIST = true
local ASSIST_SPEED_FACTOR = 0.5
local ASSIST_LAP_FACTOR = 2
local orig_speed, orig_laps = nil, nil

local tick = 0
local fish, mash, pop = nil, nil, nil
local said = {}   -- per-channel last spoken value (diff gate)

local function apply_assist()
    if not ASSIST or not Core.valid(fish) then return end
    pcall(function()
        local speed = fish.CursorSpeed
        if speed and speed > 0 then
            orig_speed = orig_speed or speed
            -- Only write when the game has (re)set the original value, so repeated
            -- attempts don't compound the slowdown.
            if speed > orig_speed * (ASSIST_SPEED_FACTOR + 0.05) then
                fish.CursorSpeed = orig_speed * ASSIST_SPEED_FACTOR
            end
        end
        local laps = fish.CursorLapLimit
        if laps and laps > 0 then
            orig_laps = orig_laps or laps
            if laps < orig_laps * ASSIST_LAP_FACTOR then
                fish.CursorLapLimit = orig_laps * ASSIST_LAP_FACTOR
            end
        end
    end)
end

-- ---- diagnostics -------------------------------------------------------------
-- Native class ends at 0x528; sample a bit beyond for possible blueprint-added
-- state (reads are SEH-guarded, garbage past the object is harmless in a dump).
local TAIL_FROM, TAIL_TO = 0x508, 0x54C
local MAX_SAMPLES = 600
local diag_done, samples = false, 0

local function dump_path()
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    return dir .. "\\dumps\\dump_fishing.txt"
end

local function fname_of(o)
    local ok, s = pcall(function() return o:GetFullName() end)
    return ok and s or "?"
end

-- One-shot: everything about the button widgets + the ActionBtn config structs.
local function diag_once(f)
    f:write("\n==== fishing session ====\n")
    f:write("widget: " .. fname_of(fish) .. "\n")
    local plat = fish.Xcmn_Btn_Plat_00
    f:write("plat on_screen=" .. tostring(Core.on_screen(plat)) .. "\n")
    pcall(function()
        f:write("plat CurrentActionID=" .. plat.CurrentActionID:ToString() .. "\n")
        local arr = plat.CurrentDynamicAssignInputControllerId
        for i = 1, #arr do f:write("plat ctrlId[" .. i .. "]=" .. arr[i]:ToString() .. "\n") end
        local ids = plat.CurrentKeyIds
        for i = 1, #ids do f:write("plat keyId[" .. i .. "]=" .. tostring(ids[i]) .. "\n") end
    end)
    for n = 0, 3 do
        pcall(function()
            local ab = fish["ActionBtn0" .. n]
            f:write(string.format("ActionBtn0%d KeyConfigActionId=%s Dyn=%s", n,
                ab.KeyConfigActionId:ToString(), ab.DynamicAssignInputId:ToString()))
            local ids = ab.KeyIdsForPad
            for i = 1, #ids do f:write(" pad[" .. i .. "]=" .. tostring(ids[i])) end
            f:write("\n")
        end)
    end
end

-- Per tick (~100 ms): the non-reflected tail as i32 + float (a live cursor value
-- shows as a smoothly changing float across samples) + the currently shown glyph.
local function diag_sample(f)
    local parts = { string.format("t=%d", tick) }
    for off = TAIL_FROM, TAIL_TO, 4 do
        parts[#parts + 1] = string.format("0x%X=%s/%.3f", off,
            tostring(Mem.i32(fish, off)), Mem.float(fish, off) or 0)
    end
    pcall(function()
        local imgs = fish.Xcmn_Btn_Plat_00.Image_List
        for i = 1, #imgs do
            if Core.is_visible(imgs[i]) then
                local ro = imgs[i].Brush.ResourceObject
                if ro and ro:IsValid() then
                    parts[#parts + 1] = "glyph=" .. (ro:GetFullName():match("([%w_]+)%.[%w_]+$") or "?")
                end
            end
        end
    end)
    -- Phase 2 (the closing ring): the RushSpeedCore's own non-reflected tail
    -- (class 0x400, last reflected member ends 0x3E0). The fishing widget's pointer
    -- to it is unset live, so find it as its own widget. Sampled even while HIDDEN
    -- (with its button token) to learn whether the phase-2 button is configured in
    -- advance — that would let us announce it early.
    pcall(function()
        local core = Core.cached_live("AT_UIBattleRushSpeedCore", tick)
        if Core.valid(core) then
            parts[#parts + 1] = "CORE vis=" .. tostring(Core.on_screen(core))
                .. " btn=" .. tostring(A.platbtn_token(core.Xcmn_Btn_Plat))
            for off = 0x3E0, 0x3FC, 4 do
                parts[#parts + 1] = string.format("c0x%X=%s/%.3f", off,
                    tostring(Mem.i32(core, off)), Mem.float(core, off) or 0)
            end
        end
    end)
    f:write(table.concat(parts, " ") .. "\n")
end

local function diagnose()
    if samples >= MAX_SAMPLES then return end
    local f = io.open(dump_path(), "a")
    if not f then return end
    if not diag_done then diag_done = true diag_once(f) end
    samples = samples + 1
    diag_sample(f)
    f:close()
end

-- ---- reading -------------------------------------------------------------------

local function say_once(channel, text, interrupt)
    if not text or text == "" then return end
    if said[channel] == text then return end
    said[channel] = text
    Speech.say(text, interrupt ~= false)
end

local function live_on_screen(cls)
    local w = Core.cached_live(cls, tick)
    if Core.on_screen(w) then return w end
    return nil
end

-- ---- "aim for the center" timing cue ---------------------------------------------
-- The minigame's live state (native_offsets.fishing, non-reflected): a cursor sweeps
-- back and forth (~127 units/s) and must be pressed inside a random static zone
-- [zoneLow, zoneHigh]. Audio mapping — a CONTINUOUS soft sine tone (user request:
-- gentle on the ears, no retriggered beeps), reshaped every tick (~100 ms):
--   outside the zone -> low tone whose PITCH rises smoothly as the cursor closes in;
--   inside (or entering within the next tick, to absorb reaction time) -> the tone
--   jumps to its highest pitch and slightly louder: PRESS NOW.
-- The cursor freezes between phases -> the tone stops (still-cursor gate).
local SWEEP_HALF = 36          -- half the observed sweep range (52..124), for proximity
local PRESS_PITCH = 2.0        -- "press now" (tone base 220 Hz -> 440 Hz)
local PRESS_VOL = 0.45
local FAR_PITCH, NEAR_PITCH = 0.6, 1.4   -- approach ramp (132..308 Hz)
local FAR_VOL, NEAR_VOL = 0.15, 0.3
local last_cursor, still_ticks = nil, 0

local function timing_cue()
    local cur = Mem.float(fish, OFF.fishing.cursor)
    local lo = Mem.float(fish, OFF.fishing.zoneLow)
    local hi = Mem.float(fish, OFF.fishing.zoneHigh)
    if not cur or not lo or not hi or hi <= lo then Audio.tone_stop() return end

    local vel = last_cursor and (cur - last_cursor) or 0
    if last_cursor and math.abs(vel) < 0.01 then
        still_ticks = still_ticks + 1
    else
        still_ticks = 0
    end
    last_cursor = cur
    if still_ticks >= 2 then Audio.tone_stop() return end   -- cursor parked: no active phase

    -- Two-tick lead (~200 ms): the user reacts to the FIRST high note, and the
    -- press must land while the cursor is still inside (dump showed presses
    -- arriving 1-2 ticks after zone exit when cued without lead).
    local predicted = cur + vel * 2
    if (cur >= lo and cur <= hi) or (predicted >= lo and predicted <= hi) then
        Audio.tone(PRESS_VOL, PRESS_PITCH)
    else
        local dist = cur < lo and (lo - cur) or (cur - hi)
        local prox = 1 - math.min(1, dist / SWEEP_HALF)
        Audio.tone(FAR_VOL + (NEAR_VOL - FAR_VOL) * prox,
                   FAR_PITCH + (NEAR_PITCH - FAR_PITCH) * prox)
    end
end

-- ---- phase 2: the closing-ring timing cue ------------------------------------------
-- native_offsets.fishing.ringSize on the RushSpeedCore shrinks (~304/s) toward
-- ringTarget (= the center button). The game rates a press by |size-target|/target:
-- the ratios captured live on the core (0.75 / 0.50 / 0.25) are the acceptance
-- tiers, and the one observed FAILED press sat at ratio 0.747 — right on the 0.75
-- boundary. So the PRESS band is generous: the tone rises as the ring closes and
-- goes high through the whole "good" band (|ratio| <= 0.5, ~±310 ms), with a
-- two-tick reaction lead on entry.
local RING_SPAN = 540          -- observed shrink range (start ~730 -> target 190)
local RING_CUE_BAND = 0.5      -- the core's mid rating tier: hold the press tone here
local ring_prev, ring_still = nil, 0

local function ring_cue(core)
    local size = Mem.float(core, OFF.fishing.ringSize)
    local target = Mem.float(core, OFF.fishing.ringTarget)
    if not size or not target or target <= 0 then Audio.tone_stop() return end

    local vel = ring_prev and (size - ring_prev) or 0
    if ring_prev and math.abs(vel) < 0.01 then
        ring_still = ring_still + 1
    else
        ring_still = 0
    end
    ring_prev = size
    if ring_still >= 2 then Audio.tone_stop() return end   -- ring parked (pressed/over)

    local hi_band = target * (1 + RING_CUE_BAND)
    local lo_band = target * (1 - RING_CUE_BAND)
    local predicted = size + vel * 2                       -- two-tick reaction lead
    if predicted <= hi_band and size >= lo_band then
        Audio.tone(PRESS_VOL, PRESS_PITCH)
    elseif size > target then
        local prox = 1 - math.min(1, (size - hi_band) / RING_SPAN)
        Audio.tone(FAR_VOL + (NEAR_VOL - FAR_VOL) * prox,
                   FAR_PITCH + (NEAR_PITCH - FAR_PITCH) * prox)
    else
        Audio.tone_stop()   -- already past the accept band
    end
end

-- The phase-1 action button TOKEN ("X"): the live glyph first, then the ActionBtn00
-- config struct's pad ids. Bare tokens keep the speech minimal (user request).
local function fishing_button()
    local tok = A.platbtn_token(fish.Xcmn_Btn_Plat_00)
    if tok then return tok end
    pcall(function()
        local ids = fish.ActionBtn00.KeyIdsForPad
        for i = 1, #ids do
            local t = A.platbtn_id_token(tonumber(ids[i]))
            if t then tok = t return end
        end
    end)
    return tok
end

-- Until any of the three widgets exists (pooled after its first appearance), each
-- lookup is a FULL object-array scan; probing three absent classes per tick costs
-- real frame time. While ALL are absent, retry only every ABSENT_TICKS.
local ABSENT_TICKS = 20   -- ~2 s at the registry's 100 ms poll
local absent_until = 0

function Fishing.is_active()
    tick = tick + 1
    if tick < absent_until then return false end
    fish = live_on_screen("AT_UIMgameFishing")
    mash = live_on_screen("AT_UIQteMashAlert")
    pop = live_on_screen("AT_UIMiniGamePop")
    if not (fish or mash or pop) then
        -- Back off only while the classes have never been seen; once pooled, the
        -- cached refs make these lookups cheap and instant.
        if not (Core.cached_live("AT_UIMgameFishing", tick)
                or Core.cached_live("AT_UIQteMashAlert", tick)
                or Core.cached_live("AT_UIMiniGamePop", tick)) then
            absent_until = tick + ABSENT_TICKS
        end
        return false
    end
    return true
end

local last_zone = nil

function Fishing.reset()
    said = {}
    last_cursor, still_ticks, last_zone = nil, 0, nil
    ring_prev, ring_still = nil, 0
    Audio.tone_stop()
end

function Fishing.update()
    -- Mash QTE — the most time-critical prompt.
    if mash then
        local btn = A.platbtn_name(mash.WL_Btn)
        say_once("mash", string.format(I18n.t("fish_mash"), btn or I18n.t("fish_button")), true)
    else
        said.mash = nil
    end

    -- Minigame popup banners (results, phase changes).
    if pop then
        local title = Core.read_text(pop.TextTitle)
        local sub = Core.read_text(pop.TextSub)
        say_once("pop", Core.phrase(title and A.markup_to_speech(title) or title,
                                    sub and A.markup_to_speech(sub) or sub), true)
    else
        said.pop = nil
    end

    -- Fishing HUD: the action button + the timing cue + the instruction text.
    if fish then
        if DEBUG then diagnose() end
        -- The zone is re-randomized per attempt, and the pooled HUD widget can stay
        -- "on screen" between attempts (the adapter never resets) — so a zone change
        -- IS the new-attempt signal: clear the diff gates and re-announce everything.
        local zone = Mem.float(fish, OFF.fishing.zoneLow)
        if zone and zone ~= last_zone then
            last_zone = zone
            said = {}
        end
        apply_assist()

        -- The phase-2 ring widget (found as its own instance — the fishing widget's
        -- pointer to it is unset live). Its button is configured IN ADVANCE, so both
        -- buttons can be announced up front and the tones alone carry the WHEN.
        local core = Core.cached_live("AT_UIBattleRushSpeedCore", tick)
        local tok2
        pcall(function() tok2 = A.platbtn_token(core.Xcmn_Btn_Plat) end)

        -- Bare letters only ("X, luego A") — minimal speech, maximal reaction time.
        local tok1 = fishing_button()
        if tok1 then
            local msg = tok2 and string.format(I18n.t("fish_buttons"), tok1, tok2) or tok1
            say_once("btn", msg, true)
        end

        if Core.on_screen(core) then
            -- Phase 2 running: instant confirmation (+ the letter again) and the
            -- closing-ring tone. TTS says only the bare token — it's time-critical.
            say_once("btn2", I18n.t("fish_hooked") .. (tok2 and (" " .. tok2) or ""), true)
            ring_cue(core)
        else
            said.btn2 = nil
            ring_prev, ring_still = nil, 0
            timing_cue()
        end
        for _, prop in ipairs({ "UIMgameTips", "UIMgameTips01" }) do
            local ok, tips = pcall(function() return fish[prop] end)
            if ok and Core.on_screen(tips) then
                local t = Core.read_text(tips.TextBox_Tips)
                if t then
                    local spoken = A.markup_to_speech(t) or t
                    -- A tips change = a PHASE change -> re-announce the button too.
                    if said["tips_" .. prop] ~= spoken then said.btn = nil end
                    say_once("tips_" .. prop, spoken, false)
                end
            end
        end
    else
        said.btn = nil
        Audio.tone_stop()
    end
end

return Fishing
