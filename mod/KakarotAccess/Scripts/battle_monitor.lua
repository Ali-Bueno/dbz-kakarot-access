-- Battle HP narrator: speaks HP thresholds for the player and the boss/enemy.
--
-- Sources = the battle HUDs' own HP number texts (blueprint-declared, verified in the
-- F7 dumps: "5317" live): Battle_Hud_P_Main_C.Txt_Num_Hp / Txt_Name_Char and
-- Battle_Hud_E_Main_C likewise (the boss bar). The game exposes NO MaxHP for battle
-- characters (only the wild-animal components carry one), so the PERCENT baseline is
-- the highest HP seen this battle per side — story fights start both sides at full,
-- and any heal above the baseline simply raises it.
--
-- Narration (user request 2026-07-06, the Raditz fight): the thresholds
-- 100/75/50/25/5/1/0 — whenever a side's bucket changes, speak "health N percent" /
-- "<enemy name> N percent" (queued, never interrupting combat lines). 0 = the side
-- fell. State resets when the battle HUD leaves the screen.
--
-- Same loop pattern as nav_tracker (generation + busy guard, transition gate first).

local Core = require("ui_core")
local Speech = require("speech")
local I18n = require("i18n")
local Transition = require("transition")

local Battle = {}

local TICK_MS = 250    -- HP thresholds don't need finer sampling
local BUCKETS = { 100, 75, 50, 25, 5, 1, 0 }

local running = false
local tick = 0
local sides = {}       -- key -> { max, bucket } per side ("p" / "e")

-- Bucket = the band the HP sits in, bounded by the spoken thresholds. 99% must stay
-- in the "100" band — announcing "75" the moment any damage lands (pct 99 -> the old
-- largest-threshold-below logic) read as the health crashing instantly (user
-- 2026-07-06). A band is only entered by genuinely crossing its boundary.
--   pct > 75 -> 100 | > 50 -> 75 | > 25 -> 50 | > 5 -> 25 | > 1 -> 5 | > 0 -> 1 | 0
local function bucket_of(pct)
    if pct > 75 then return 100 end
    if pct > 50 then return 75 end
    if pct > 25 then return 50 end
    if pct > 5 then return 25 end
    if pct > 1 then return 5 end
    if pct > 0 then return 1 end
    return 0
end

-- Current HP number of a HUD (its Txt_Num_Hp text), or nil while hidden/empty.
-- Both text nodes are fetched through Core.member: the battle HUDs are POOLED and the
-- game recycles them the instant a fight ends or an enemy falls, so `hud.Txt_Num_Hp`
-- at the call site is a property __index on a possibly-dangling UObject — the
-- uncatchable access violation (user crash mid-combat, 2026-07-24: AV reading
-- 0x00000010 = ClassPrivate of a NULL UObject). This loop is the one that runs at
-- 250ms THROUGH combat, so it is the most exposed caller in the mod.
local function hud_hp(hud)
    if not (Core.valid(hud) and Core.on_screen(hud)) then return nil end
    local n
    pcall(function()
        local t = Core.read_text(Core.member(hud, "Txt_Num_Hp"))
        if t then n = tonumber((t:gsub("[^%d]", ""))) end
    end)
    return n
end

local function hud_name(hud)
    local t
    pcall(function() t = Core.read_text(Core.member(hud, "Txt_Name_Char")) end)
    return t
end

-- Track one side's HP against its running baseline; announce bucket changes.
local function track(key, hud, is_player)
    local hp = hud_hp(hud)
    if not hp then sides[key] = nil return end
    local s = sides[key]
    if not s then
        s = { max = hp, bucket = nil }
        sides[key] = s
    end
    if hp > s.max then s.max = hp end   -- heals above the baseline raise it
    if s.max <= 0 then return end
    local pct = math.floor(hp * 100 / s.max + 0.5)
    local b = bucket_of(pct)
    if b ~= s.bucket then
        -- First reading of the battle: set the bucket silently (announcing "health
        -- 100 percent" at every battle start would be noise).
        if s.bucket ~= nil then
            local spoken
            if b < s.bucket then
                spoken = b                       -- fell AT/BELOW this threshold
            elseif pct >= 100 then
                spoken = 100                     -- genuinely back to full
            else
                spoken = nil                     -- healed into a higher band: speak the
            end                                  -- real rounded percent, not a fake 100
            local text
            if is_player then
                text = spoken and string.format(I18n.t("hp_self_fmt"), spoken)
                    or string.format(I18n.t("hp_self_fmt"), pct)
            else
                local nm = hud_name(hud) or I18n.t("cat_enemy")
                text = string.format(I18n.t("hp_enemy_fmt"), nm, spoken or pct)
            end
            Speech.say(text, false)
        end
        s.bucket = b
    end
end

local function step()
    tick = tick + 1
    if Transition.active() then sides = {} return end
    -- Own scan budget/time window (like quest_objective): this loop runs outside the
    -- registry step, and without this the shared budget gate would see the registry's
    -- stale step start time and starve the battle-HUD FindAllOf forever.
    Core.begin_scan_tick()
    local p = Core.first_on_screen("Battle_Hud_P_Main_C", tick)
    if not p then
        -- No battle HUD: out of combat. Drop all baselines for the next fight.
        if next(sides) then sides = {} end
        return
    end
    track("p", p, true)
    local e = Core.first_on_screen("Battle_Hud_E_Main_C", tick)
    if e then track("e", e, false) end
end

function Battle.start()
    if running then return end
    running = true
    _G.__KakarotBattleGen = (_G.__KakarotBattleGen or 0) + 1
    local myGen = _G.__KakarotBattleGen
    local busy = false
    LoopAsync(TICK_MS, function()
        if _G.__KakarotBattleGen ~= myGen then return true end
        if not busy then
            busy = true
            ExecuteInGameThread(function()
                busy = false   -- cleared on ENTRY (see ui_core.loop rationale)
                local t0 = os.clock()
                local ok, err = pcall(step)
                if not ok then
                    print("[KakarotAccess] battle monitor error: " .. tostring(err) .. "\n")
                end
                -- Cost telemetry (own loop, outside the registry step — printed by
                -- the Ctrl+F5 dump; the last unmeasured game-thread work).
                local dt = (os.clock() - t0) * 1000
                local st = _G.__KakarotBattleStats
                if not st then st = { n = 0, ms = 0, max = 0 } _G.__KakarotBattleStats = st end
                st.n = st.n + 1
                st.ms = st.ms + dt
                if dt > st.max then st.max = dt end
            end)
        end
        return false
    end)
end

function Battle.stop()
    running = false
    _G.__KakarotBattleGen = (_G.__KakarotBattleGen or 0) + 1
    sides = {}
end

return Battle
