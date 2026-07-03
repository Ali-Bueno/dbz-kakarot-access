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

-- Re-enable to append tail samples + button internals to dumps/dump_fishing.txt
-- (how the fishing offsets in native_offsets.lua were found).
local DEBUG = false

local tick = 0
local fish, mash, pop = nil, nil, nil
local said = {}   -- per-channel last spoken value (diff gate)

-- ---- diagnostics -------------------------------------------------------------
local TAIL_FROM, TAIL_TO = 0x508, 0x524   -- non-reflected tail dwords of AT_UIMgameFishing
local MAX_SAMPLES = 300
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

    local predicted = cur + vel           -- next-tick position (reaction-time lead)
    if (cur >= lo and cur <= hi) or (predicted >= lo and predicted <= hi) then
        Audio.tone(PRESS_VOL, PRESS_PITCH)
    else
        local dist = cur < lo and (lo - cur) or (cur - hi)
        local prox = 1 - math.min(1, dist / SWEEP_HALF)
        Audio.tone(FAR_VOL + (NEAR_VOL - FAR_VOL) * prox,
                   FAR_PITCH + (NEAR_PITCH - FAR_PITCH) * prox)
    end
end

-- The fishing action button: the live glyph first, then the ActionBtn00 config
-- struct (KeyConfig action id, then its raw pad key ids).
local function fishing_button()
    local n = A.platbtn_name(fish.Xcmn_Btn_Plat_00)
    if n then return n end
    local name
    pcall(function()
        local ab = fish.ActionBtn00
        local kc = ab.KeyConfigActionId:ToString()
        if kc and kc ~= "" and kc ~= "None" then name = A.keyconfig_button(kc) end
        if not name then
            local ids = ab.KeyIdsForPad
            for i = 1, #ids do
                local nm = A.platbtn_id(tonumber(ids[i]))
                if nm then name = nm return end
            end
        end
    end)
    return name
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
        local btn = fishing_button()
        if btn then say_once("btn", string.format(I18n.t("fish_press"), btn), true) end
        timing_cue()
        for _, prop in ipairs({ "UIMgameTips", "UIMgameTips01" }) do
            local ok, tips = pcall(function() return fish[prop] end)
            if ok and Core.on_screen(tips) then
                local t = Core.read_text(tips.TextBox_Tips)
                if t then say_once("tips_" .. prop, A.markup_to_speech(t) or t, false) end
            end
        end
    else
        said.btn = nil
        Audio.tone_stop()
    end
end

return Fishing
