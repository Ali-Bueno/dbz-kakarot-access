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
-- (how the fishing offsets in native_offsets.lua were found, and how the pooled
-- ring-core alternation was caught). OFF since the full flow was re-verified
-- end-to-end in-game (user landing fish consistently, 2026-07-15).
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
        local arr, na = Core.array_of(plat, "CurrentDynamicAssignInputControllerId")
        if arr then
            for i = 1, na do f:write("plat ctrlId[" .. i .. "]=" .. arr[i]:ToString() .. "\n") end
        end
        local ids, ni = Core.array_of(plat, "CurrentKeyIds")
        if ids then
            for i = 1, ni do f:write("plat keyId[" .. i .. "]=" .. tostring(ids[i]) .. "\n") end
        end
    end)
    for n = 0, 3 do
        pcall(function()
            local ab = fish["ActionBtn0" .. n]
            f:write(string.format("ActionBtn0%d KeyConfigActionId=%s Dyn=%s", n,
                ab.KeyConfigActionId:ToString(), ab.DynamicAssignInputId:ToString()))
            local ids, ni = Core.array_of(ab, "KeyIdsForPad")
            if ids then
                for i = 1, ni do f:write(" pad[" .. i .. "]=" .. tostring(ids[i])) end
            end
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
    do
        local plat
        pcall(function() plat = fish.Xcmn_Btn_Plat_00 end)
        local imgs, ni = Core.array_of(plat, "Image_List")
        if imgs then pcall(function()
            for i = 1, ni do
                if Core.is_visible(imgs[i]) then
                    local ro = imgs[i].Brush.ResourceObject
                    if Core.nonnull(ro) then                  -- never ro:IsValid(), see Core.nonnull
                        parts[#parts + 1] = "glyph=" .. (ro:GetFullName():match("([%w_]+)%.[%w_]+$") or "?")
                    end
                end
            end
        end) end
    end
    -- Phase 2 (the closing ring): the RushSpeedCore's own non-reflected tail
    -- (class 0x400, last reflected member ends 0x3E0). The game pools SEVERAL
    -- instances and alternates per catch, so sample EVERY one (index + vis + button
    -- + tail) — that alternation is what the 2026-07-15 dump proved.
    pcall(function()
        local list = Core.cached_all("AT_UIBattleRushSpeedCore", tick)
        if not list then return end
        local i = 0
        for _, core in pairs(list) do
            if Core.valid(core) then
                i = i + 1
                parts[#parts + 1] = string.format("CORE%d vis=%s btn=%s", i,
                    tostring(Core.on_screen(core)), tostring(A.platbtn_token(core.Xcmn_Btn_Plat)))
                for off = 0x3E0, 0x3FC, 4 do
                    parts[#parts + 1] = string.format("c%d_0x%X=%s/%.3f", i, off,
                        tostring(Mem.i32(core, off)), Mem.float(core, off) or 0)
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

-- The ACTIVE phase-2 ring core. The game keeps SEVERAL pooled ring-core widgets and
-- alternates which one it drives per catch (dump 2026-07-15: half the reels ran with
-- the previously-cached instance stuck vis=false and its ringSize frozen — no tone,
-- no letter, the "only caught one fish" report). A single pinned cached_live ref is
-- therefore wrong half the time: enumerate the pool and take the on-screen instance.
local function ring_core()
    local list = Core.cached_all("AT_UIBattleRushSpeedCore", tick)
    if list then
        for _, w in pairs(list) do
            if Core.on_screen(w) then return w end
        end
    end
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
local boosted2 = false         -- one core-discovery boost per phase-2 entry

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
    local ab
    pcall(function() ab = fish.ActionBtn00 end)
    local ids, ni = Core.array_of(ab, "KeyIdsForPad")
    if ids then pcall(function()
        for i = 1, ni do
            local t = A.platbtn_id_token(tonumber(ids[i]))
            if t then tok = t return end
        end
    end) end
    return tok
end

-- No adapter-level absence backoff (removed 2026-07-15). The old ABSENT_TICKS gate
-- paused ALL detection here for ~2 s at a time while the minigame widgets didn't
-- exist — on a hook bar that lasts ~3 s (dump_fishing: phase 1 = 30 ticks), a bite
-- landing inside that dead window made the cue start near the end or not at all
-- (the reported inconsistency). It existed to avoid full per-class scans, which is
-- ui_core's job now: these classes resolve through the screen directory (pointer
-- reads), and the scan fallback is budget- and backoff-gated centrally.
local live_stamp = nil   -- os.clock() while any minigame surface is on screen
local sheet_seen = nil   -- os.clock() of the last result-sheet sighting

function Fishing.is_active()
    tick = tick + 1
    fish = live_on_screen("AT_UIMgameFishing")
    mash = live_on_screen("AT_UIQteMashAlert")
    pop = live_on_screen("AT_UIMiniGamePop")
    local any = (fish or mash or pop) ~= nil
    if any then
        live_stamp = os.clock()
        -- Published for screen_fishresult: the result-sheet pool may only be
        -- probed AROUND a live/recent minigame (see below).
        _G.__KakarotMinigameLive = live_stamp
    end
    -- The minigame is OVER while the result sheet (Mgame_Result_C, read by
    -- screen_fishresult) is up — but the pooled HUD lingers on_screen beneath
    -- it and would re-speak the phase-1 prompt. Yield during the sheet AND for
    -- ~3 s after it (the "Siguiente" close leaves the HUD lingering too — the
    -- prompt re-read the user heard). The sheet probe itself is GATED on a
    -- live/recent minigame: Mgame_Result_C is a per-level pool, and probing it
    -- unconditionally every tick in every state was the prime suspect of the
    -- 2026-07-17 return-to-title AV (freed pooled widget hit by IsValid during
    -- world teardown — crash ledger).
    if any or (live_stamp and os.clock() - live_stamp < 30) then
        if Core.first_on_screen("Mgame_Result_C", tick) ~= nil then
            sheet_seen = os.clock()
        end
        if sheet_seen and os.clock() - sheet_seen < 3 then
            fish, mash, pop = nil, nil, nil
            Audio.tone_stop()
            return false
        end
    end
    return any
end

local last_zone = nil

function Fishing.reset()
    said = {}
    last_cursor, still_ticks, last_zone = nil, 0, nil
    ring_prev, ring_still, boosted2 = nil, 0, false
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

        -- Phase from the game's own state machine (fishing.phase, CONFIRMED native
        -- byte on the fish widget). The reel is SHORT — dump 2026-07-15: the ring
        -- falls ~420 u/s, well under a second from start to target — so this branch
        -- is built for ZERO speech latency: the bare letter is the only utterance,
        -- the tone carries the WHEN. The reel button is random per catch and only
        -- knowable once the active core is on screen — never pre-announce it.
        local phase = Mem.i32(fish, OFF.fishing.phase)
        local core = ring_core()
        if phase == 2 or core ~= nil then
            local tok2
            if core then pcall(function() tok2 = A.platbtn_token(core.Xcmn_Btn_Plat) end) end
            if tok2 then
                -- The letter, alone, the instant it's known; re-spoken only if the
                -- game swaps it mid-reel (say_once diff-gates on the text).
                say_once("btn2", tok2, true)
            elseif said.btn2 == nil then
                -- Hooked but the active core isn't visible yet: confirm the bite;
                -- the letter will cut this off the moment it resolves.
                say_once("btn2hooked", I18n.t("fish_hooked"), true)
            end
            if core then
                ring_cue(core)
            else
                -- Reel started and no on-screen core in the pool: the game may have
                -- built a NEW instance — force one pool re-scan + boost, ONCE per
                -- phase entry (boost_missing opens a whole window generation; calling
                -- it per tick is the 2026-07-15 pad-boost scan storm).
                if not boosted2 then
                    boosted2 = true
                    Core.refresh_all("AT_UIBattleRushSpeedCore")
                    Core.boost_missing()
                end
                Audio.tone_stop()
            end
        else
            said.btn2, said.btn2hooked = nil, nil
            boosted2 = false
            ring_prev, ring_still = nil, 0

            -- Outside the reel: announce only the PHASE-1 button (random per attempt,
            -- re-announced via the zone-change reset). The old "X, then Y" pair is
            -- gone: the second letter isn't decided until the reel starts — the dump
            -- caught it announcing a STALE core's button ("X luego B", real reel = Y).
            local tok1 = fishing_button()
            if tok1 then
                say_once("btn", tok1, true)
            end
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
