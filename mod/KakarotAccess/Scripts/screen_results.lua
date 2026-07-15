-- Screen adapter: story results (Quest_Main_Clear_C -> UAT_UIQuestMainClear).
--
-- The post-battle "Story Results" sequence: one bar per battle ("Goku vs. Raditz")
-- with a RANK LETTER shown as an IMAGE, then a TOTAL rank; "View details" (X) expands
-- per-battle detail rows (Completion time / Max combo / Damage taken), each with its
-- own rank image and — for counters like max combo — a DIGIT-IMAGE number
-- (Image_PercentageList). Verified from the F7 dumps (2026-07-06) + CXX headers:
--   host UIClearBar_List (native TArray of bars); per bar TextBox_Item + Image_Rank
--   (native) and the DETAIL rows as the blueprint's DIRECT children
--   Quest_Main_Clear_Detail00..05 (Quest_Main_Clear_Bar.hpp — the native
--   UIClearDetail_List array stayed empty live, which left the expanded details
--   unread); total UIQuestMainClearRank.Image_Rank.
--
-- Rank letters and digit counters come from brush TEXTURE names (keyhelp technique).
-- The reader is INCREMENTAL: each line is queued exactly once as it becomes readable,
-- following the game's reveal animation. Lines are KEYED by bar index + content so
-- two battles with the same title (Goku vs. Raditz twice) both read.

local Core = require("ui_core")
local Speech = require("speech")
local I18n = require("i18n")

local Results = {}

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
Results.keyhelp_auto = false

local ann = Core.make_announcer()   -- unused for speech (incremental), kept for API symmetry
local host = nil
local tick = 0
local spoken = {}    -- line KEY -> true. Cleared when the screen closes, NOT on reset().
local queue = {}     -- { {key=, text=}, ... } computed in is_active

-- DIAGNOSIS (user bug 2026-07-15): every detail row reads a CONSTANT "222" for its
-- value while labels and ranks are right — the digit images likely all share ONE
-- atlas texture whose name ends in a digit (texture-name parsing can't work then; the
-- real value would live in the row's unreflected tail, AT.hpp: Detail 0x3C0..0x418).
-- While ON, each detail row's Image_PercentageList is dumped once (visibility + brush
-- texture full name + widget path) to dumps/dump_results.txt during a real results
-- screen. Turn OFF once the value source is fixed and re-verified.
local DEBUG = true
local dumped = {}    -- detail KEY -> true (reset with `spoken` when the screen closes)

local function dump_path()
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    return dir .. "\\dumps\\dump_results.txt"
end

local function debug_dump_detail(d, key, label)
    if dumped[key] then return end
    dumped[key] = true
    local f = io.open(dump_path(), "a")
    if not f then return end
    f:write(string.format("== %s label=%s\n", key, tostring(label)))
    local arr, n = Core.array_of(d, "Image_PercentageList")
    if arr then
        pcall(function()
            for i = 1, n do
                local img = arr[i]
                local vis, full, tex = "?", "?", "(none)"
                pcall(function() vis = tostring(Core.is_visible(img)) end)
                pcall(function() full = img:GetFullName() end)
                pcall(function()
                    local ro = img.Brush.ResourceObject
                    if ro and ro:IsValid() then tex = ro:GetFullName() end
                end)
                f:write(string.format("  [%d] vis=%s tex=%s\n      widget=%s\n", i, vis, tex, full))
            end
        end)
    else
        f:write("  (Image_PercentageList unreadable)\n")
    end
    f:close()
end

local DETAIL_COUNT = 6   -- Quest_Main_Clear_Detail00..05 (Quest_Main_Clear_Bar.hpp)

-- Valid rank letters (validates the texture-name suffix so an unrelated image never
-- reads as a rank).
local RANKS = { S = true, A = true, B = true, C = true, D = true, Z = true, SS = true }

local function texture_token(img)
    if not Core.valid(img) or not Core.is_visible(img) then return nil end
    local res
    pcall(function()
        local ro = img.Brush.ResourceObject
        if ro and ro:IsValid() then res = ro:GetFullName() end
    end)
    return res and res:match("([%w_]+)%.[%w_]+$") or nil
end

local function rank_letter(img)
    local tok = texture_token(img)
    if not tok then return nil end
    local letter = tok:match("[Rr]ank_(%u%u?)$") or tok:match("_(%u%u?)$")
    return (letter and RANKS[letter]) and letter or nil
end

-- A number drawn as digit images (each texture name ends in its digit), or nil
-- unless EVERY visible digit parses.
local function image_number(arr_owner, member)
    local digits, ok = {}, true
    local arr, n = Core.array_of(arr_owner, member)
    if not arr then return nil end
    pcall(function()
        for i = 1, n do
            local img = arr[i]
            if Core.valid(img) and Core.is_visible(img) then
                local tok = texture_token(img)
                local d = tok and tok:match("(%d)$")
                if d then digits[#digits + 1] = d else ok = false end
            end
        end
    end)
    return (ok and #digits > 0) and table.concat(digits) or nil
end

-- All currently-readable lines with stable keys, in on-screen order.
local function lines()
    local out = { { key = "screen", text = I18n.header(6) } }
    local bars, nbars = Core.array_of(host, "UIClearBar_List")
    if not bars then return out end
    pcall(function()
        for i = 1, nbars do
            local bar = bars[i]
            if Core.valid(bar) and Core.on_screen(bar) then
                local name = Core.read_text(bar.TextBox_Item)
                local r = rank_letter(bar.Image_Rank)
                if name and r then
                    out[#out + 1] = { key = "bar" .. i, text = name .. ", " .. r }
                end
                -- Expanded details (after "View details"): blueprint children; the
                -- first row is prefixed with its battle so groups stay apart.
                for j = 0, DETAIL_COUNT - 1 do
                    local d
                    pcall(function()
                        d = bar["Quest_Main_Clear_Detail" .. string.format("%02d", j)]
                    end)
                    if Core.valid(d) and Core.on_screen(d) then
                        local dn = Core.read_text(d.TextBox_Detail)
                        local dr = rank_letter(d.Image_Rank)
                        if dn and dr then
                            local key = string.format("d%d.%d", i, j)
                            if DEBUG then debug_dump_detail(d, key, dn) end
                            local text = Core.phrase(dn,
                                image_number(d, "Image_PercentageList"), dr)
                            if j == 0 and name then text = name .. ": " .. text end
                            out[#out + 1] = { key = key, text = text }
                        end
                    end
                end
            end
        end
        local tot = host.UIQuestMainClearRank
        if Core.valid(tot) and Core.on_screen(tot) then
            local r = rank_letter(tot.Image_Rank)
            if r then
                out[#out + 1] = { key = "total", text = I18n.t("results_total") .. ", " .. r }
            end
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
        dumped = {}
        return false
    end
    queue = lines()
    return true
end

function Results.reset() ann:reset() end

function Results.update()
    for _, e in ipairs(queue) do
        if not spoken[e.key] then
            spoken[e.key] = true
            Speech.say(e.text, false)   -- queued: follows the game's reveal pacing
        end
    end
end

return Results
