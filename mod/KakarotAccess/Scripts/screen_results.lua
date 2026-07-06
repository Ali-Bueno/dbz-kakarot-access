-- Screen adapter: story results (Quest_Main_Clear_C -> UAT_UIQuestMainClear).
--
-- The post-battle "Story Results" sequence: one bar per battle ("Piccolo vs. Raditz")
-- with a RANK LETTER shown as an IMAGE, then a TOTAL rank; "View details" (X) expands
-- per-battle detail rows (Completion time / Max combo / Damage taken), each with its
-- own rank image. Verified from the F7 dump (2026-07-06) + CXX headers: host
-- UIClearBar_List (TArray<UAT_UIQuestMainClearBar>), bar TextBox_Item + Image_Rank +
-- UIClearDetail_List (TextBox_Detail + Image_Rank), total UIQuestMainClearRank.
--
-- The rank letter is recovered from Image_Rank's brush TEXTURE name (same technique as
-- the keyhelp glyphs) — there is no text anywhere. Because the ranks pop in one by one
-- with the game's animation, the reader is INCREMENTAL: each line (battle+rank, detail
-- rows, total) is queued exactly once as soon as it becomes readable, following the
-- game's own pacing instead of re-reading the whole board on every change.

local Core = require("ui_core")
local Speech = require("speech")
local I18n = require("i18n")

local Results = {}

local ann = Core.make_announcer()   -- unused for speech (incremental), kept for API symmetry
local host = nil
local tick = 0
local spoken = {}    -- line -> true. Cleared when the screen closes (host off), NOT on
                     -- reset(): an adapter flicker must not re-read the whole board.
local queue = {}     -- lines to speak this tick, computed in is_active

-- Valid rank letters (validates the texture-name suffix so an unrelated image never
-- reads as a rank).
local RANKS = { S = true, A = true, B = true, C = true, D = true, Z = true, SS = true }

-- The rank letter shown by a rank Image, from its brush texture name
-- (".../Ins_Rank_S.Ins_Rank_S" -> "S"), or nil while hidden / not yet set.
local function rank_letter(img)
    if not Core.valid(img) or not Core.is_visible(img) then return nil end
    local res
    pcall(function()
        local ro = img.Brush.ResourceObject
        if ro and ro:IsValid() then res = ro:GetFullName() end
    end)
    local tok = res and res:match("([%w_]+)%.[%w_]+$") or nil
    if not tok then return nil end
    local letter = tok:match("[Rr]ank_(%u%u?)$") or tok:match("_(%u%u?)$")
    return (letter and RANKS[letter]) and letter or nil
end

-- All currently-readable lines, in on-screen order. Names come from the game's own
-- text; only the "Total" word and the screen name are mod-supplied (i18n).
local function lines()
    local out = { I18n.header(6) }   -- "Result(s)" — the screen name, spoken first
    pcall(function()
        local bars = host.UIClearBar_List
        for i = 1, #bars do
            local bar = bars[i]
            if Core.valid(bar) and Core.on_screen(bar) then
                local name = Core.read_text(bar.TextBox_Item)
                local r = rank_letter(bar.Image_Rank)
                if name and r then out[#out + 1] = name .. ", " .. r end
                -- Expanded details (after "View details"): prefix the first row with
                -- its battle so the groups stay distinguishable in the spoken stream.
                pcall(function()
                    local dets = bar.UIClearDetail_List
                    for j = 1, #dets do
                        local d = dets[j]
                        if Core.valid(d) and Core.on_screen(d) then
                            local dn = Core.read_text(d.TextBox_Detail)
                            local dr = rank_letter(d.Image_Rank)
                            if dn and dr then
                                local line = dn .. ", " .. dr
                                if j == 1 and name then line = name .. ": " .. line end
                                out[#out + 1] = line
                            end
                        end
                    end
                end)
            end
        end
        local tot = host.UIQuestMainClearRank
        if Core.valid(tot) and Core.on_screen(tot) then
            local r = rank_letter(tot.Image_Rank)
            if r then out[#out + 1] = I18n.t("results_total") .. ", " .. r end
        end
    end)
    return out
end

function Results.is_active()
    tick = tick + 1
    host = Core.first_on_screen("Quest_Main_Clear_C", tick)   -- pooled, multi-instance
    if not host then
        spoken = {}
        queue = {}
        return false
    end
    queue = lines()
    return true
end

function Results.reset() ann:reset() end

function Results.update()
    for _, line in ipairs(queue) do
        if not spoken[line] then
            spoken[line] = true
            Speech.say(line, false)   -- queued: follows the game's reveal pacing
        end
    end
end

return Results
