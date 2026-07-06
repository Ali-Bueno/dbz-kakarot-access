-- Notice reader: post-battle result overlay (Battle_Result_C -> UAT_UIBattleResult).
--
-- The field overlay shown when a battle ends: a RANK letter (RateIcon image — texture
-- name gives the letter, same technique as screen_results) + "EXP. gained" (ImgLangExp,
-- game text) + the EXP NUMBER drawn as per-digit images (CounterCtn, array order; each
-- digit recovered from its brush texture name). All members are native (AT.hpp).
--
-- NOTICE-RELEASE: speaks the whole line ONCE per battle ("S, EXP. gained, 756") and
-- releases the dispatcher tick immediately — a resident overlay must never hold the
-- screen (the tutorial-shadow lesson, 2026-07-06). nav_mute=false: it overlays free
-- roaming; the radar keeps running.

local Core = require("ui_core")
local I18n = require("i18n")
local Speech = require("speech")

local Result = {}

Result.nav_mute = false

local ann = Core.make_announcer()   -- unused for speech; kept for API symmetry
local host = nil
local tick = 0
local spoken = nil    -- last announced content (NOT cleared by reset — once per battle)
local pending = nil

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

local function rank_letter()
    for _, m in ipairs({ "RateIcon", "RateLowIcon" }) do
        local tok = texture_token(host[m])
        local letter = tok and (tok:match("[Rr]ank_(%u%u?)$") or tok:match("_(%u%u?)$"))
        if letter and RANKS[letter] then return letter end
    end
    return nil
end

-- The EXP amount from the digit-image counter (CounterCtn): each visible image's
-- texture name ends in its digit. Included only when EVERY visible digit parses.
local function exp_number()
    local digits, ok = {}, true
    pcall(function()
        local arr = host.CounterCtn
        for i = 1, #arr do
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

function Result.is_active()
    tick = tick + 1
    host = Core.first_on_screen("Battle_Result_C", tick)
    if not host then pending = nil return false end
    local exp_label
    pcall(function()
        if Core.on_screen(host.ImgLangExp) then
            exp_label = Core.read_text(host.ImgLangExp)
        end
    end)
    if not exp_label then pending = nil return false end
    pending = Core.phrase(rank_letter(), exp_label, exp_number())
    -- Speak once per distinct result, then RELEASE the tick (notice pattern).
    return pending ~= "" and pending ~= spoken
end

function Result.reset() ann:reset() end

function Result.update()
    if not pending or pending == spoken then return end
    spoken = pending
    Speech.say(spoken, false)   -- queued: don't cut a line the player is hearing
end

return Result
