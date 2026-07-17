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
-- pane_live-gated: a parked pooled sheet keeps stale text (user directive 2026-07-15).

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")

local FishResult = {}

FishResult.nav_mute = false

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
FishResult.keyhelp_auto = false

local ROW_MAX = 10   -- Xlist_Bar02_00.. probe cap (census showed _00; rows contiguous)

local tick = 0
local spoken = {}    -- line -> true while the sheet stays up (cleared on close)
local queue = nil    -- lines to speak this tick (computed in is_active)

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
    local first = true
    for i = 0, ROW_MAX - 1 do
        local row
        pcall(function() row = host[string.format("Xlist_Bar02_%02d", i)] end)
        if not Core.valid(row) then break end
        if Core.on_screen(row) then
            local item = node_text(row, "Txt_List")
            if item then
                local n = node_text(row, "Txt_Num")
                local text = n and (item .. ", " .. n) or item
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
    local host = Core.first_on_screen("Mgame_Result_C", tick)
    if not host or not Core.pane_live(host) then
        spoken = {}
        return false
    end
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
