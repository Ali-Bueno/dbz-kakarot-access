-- Notice reader: minigame result sheet (Mgame_Result_C -> AT_UIMiniGameResult) —
-- the "¡BRAVO!" panel after fishing (and other minigames): the caught-fish header
-- + species, then the "items obtained" rows.
--
-- Structure (F7 census dump_1784297214_001, 2026-07-17 + ObjectDump): the native
-- class reflects TxtCap00/TxtName/TxtCap01 (0x478/0x480/0x488) but the census tree
-- names carry underscores (Txt_Cap00, Txt_Name), so every node is fetched under
-- BOTH names. The obtained-items rows are the BP tree's Xlist_Bar02_NN, each with
-- Txt_List (item name) + Txt_Num (count).
--
-- fm.MiniGameResult (AT_UIFieldManager 0x630) is deliberately NOT directory-mapped:
-- the fm minigame/toast fields have twice proven to stay null while the pooled
-- widget lives (AT_UIBattleRushSpeedCore, InfoLevelUp) — the scan path serves it,
-- kept scannable at fishing's end by the QUIET_EXEMPT entry in ui_core (the sheet
-- auto-appears with no press, possibly inside a quiet gap).
--
-- Notice pattern: each line speaks ONCE per appearance (queued, never interrupting)
-- and the dispatcher tick is released immediately, so any real menu outranks it.
-- Liveness gate is OPACITY-ONLY: the full pane_live demands GetVisibility()==0
-- (Visible), but passive overlays in this game render as HitTestInvisible (the
-- Xcmn_Subtitles precedent) — the strict gate held this sheet silent until the
-- "Siguiente" press flipped states (user bug 2026-07-17). on_screen already drops
-- Collapsed/Hidden; the opacity check still drops the fading close-anim ghost.

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")

local FishResult = {}

FishResult.nav_mute = false

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
FishResult.keyhelp_auto = false

local ROW_MAX = 10   -- Xlist_Bar02_00.. probe cap (census showed _00; rows contiguous)

local LIVE_WINDOW_S = 60   -- how long after the last minigame surface the pool may be probed
local GONE_TICKS = 5       -- ticks of absence before the spoken set is forgotten

local tick = 0
local spoken = {}    -- line -> true while the sheet stays up (cleared on close)
local queue = nil    -- lines to speak this tick (computed in is_active)
local gone = 0       -- consecutive ticks without a live sheet (absence debounce)

-- Close-animation ghost: opacity fades to 0 while visibility flags lag.
local function faded(h)
    local ok, op = pcall(function() return h:GetRenderOpacity() end)
    return ok and type(op) == "number" and op < 0.05
end

-- First readable text among the given member names (native vs BP tree spelling).
local function node_text(owner, ...)
    for _, m in ipairs({ ... }) do
        local t
        pcall(function()
            local node = owner[m]
            if Core.valid(node) and Core.is_visible(node) then
                t = A.markup_to_speech(Core.read_text(node))
            end
        end)
        if t and t ~= "" then return t end
    end
    return nil
end

-- All currently-readable lines, in on-screen order.
local function lines(host)
    local out = {}
    local cap0 = node_text(host, "TxtCap00", "Txt_Cap00")   -- "Peces capturados"
    local name = node_text(host, "TxtName", "Txt_Name")     -- species ("Pescado rojo")
    if name then out[#out + 1] = Core.phrase(cap0, name) end
    local cap1 = node_text(host, "TxtCap01", "Txt_Cap01")   -- "Objetos ganados"
    -- Obtained-item rows: the native InfoLogBarList TArray (AT.hpp @0x408,
    -- TArray<UAT_UIInfoLogBar02*>) via the array_of guard; the census tree names
    -- (Xlist_Bar02_NN) only as fallback, each fetch pcall-guarded.
    local rows = {}
    local arr, n = Core.array_of(host, "InfoLogBarList")
    if arr then
        pcall(function()
            for i = 1, n do rows[#rows + 1] = arr[i] end
        end)
    end
    if #rows == 0 then
        for i = 0, ROW_MAX - 1 do
            local row
            pcall(function() row = host[string.format("Xlist_Bar02_%02d", i)] end)
            if not Core.valid(row) then break end
            rows[#rows + 1] = row
        end
    end
    local first = true
    for _, row in ipairs(rows) do
        if Core.valid(row) and Core.on_screen(row) then
            local item = node_text(row, "Txt_List")
            if item then
                local num = node_text(row, "Txt_Num")
                local text = num and (item .. ", " .. num) or item
                if first and cap1 then
                    text = cap1 .. ": " .. text
                    first = false
                end
                out[#out + 1] = text
            end
        end
    end
    return out
end

function FishResult.is_active()
    tick = tick + 1
    queue = nil
    -- Probe the per-level result pool ONLY around a live/recent minigame (stamp
    -- published by screen_fishing). Registered at the top of the stack, an
    -- unconditional probe here ran every tick in EVERY state — prime suspect of
    -- the 2026-07-17 return-to-title AV (freed pooled widget hit by IsValid
    -- during world teardown — crash ledger).
    local stamp = _G.__KakarotMinigameLive
    if not stamp or os.clock() - stamp > LIVE_WINDOW_S then
        spoken = {}
        gone = 0
        return false
    end
    local host = Core.first_on_screen("Mgame_Result_C", tick)
    if not host or faded(host) then
        -- Debounced forget: the "Siguiente" close/page-flip can drop the sheet
        -- for a tick or two — clearing `spoken` right away re-read the whole
        -- sheet (user bug 2026-07-17). Forget only after a genuine absence.
        gone = gone + 1
        if gone >= GONE_TICKS then spoken = {} end
        return false
    end
    gone = 0
    for _, line in ipairs(lines(host)) do
        if not spoken[line] then
            queue = queue or {}
            queue[#queue + 1] = line
        end
    end
    -- Active only while there is something NEW to say; released right after (notice
    -- pattern — the sheet must never hold the screen).
    return queue ~= nil
end

function FishResult.reset() end

function FishResult.update()
    if not queue then return end
    for _, line in ipairs(queue) do
        spoken[line] = true
        Speech.say(line, false)   -- queued: follows the sheet's reveal animation
    end
    queue = nil
end

return FishResult
