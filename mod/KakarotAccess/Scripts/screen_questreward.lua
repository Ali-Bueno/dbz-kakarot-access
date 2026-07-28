-- Substory CLEAR reward panel: the "¡FELICITACIONES! / Recompensas de historia" sheet shown
-- when a side story completes, listing EXP and the items awarded.
--
-- IDENTIFIED FROM A CAPTURE, NOT GUESSED (2026-07-26, user screenshot + F7 census taken while
-- the sheet was on screen — the playbook's rule: dumps answer "what is on screen right now",
-- headers answer "what could exist", and "which widget is the live one" is only ever a dump
-- question). The census named it outright:
--   Quest_Sub_Reward_C_1 . Txt_Title            = "Recompensas de historia"
--   Xcmn_Win01_List_C_12..16                    = "4583|EXP", "1|Medalla D",
--                                                 "1|Pan de la inteligencia", …
-- and the log for that session proved no adapter was active at all in that window, so the
-- screen was simply uncovered. `screen_results` does NOT cover it: that one reads
-- `Quest_Main_Clear_C` (the MAIN-quest clear sheet), a different widget.
--
-- STRUCTURE (CXXHeaderDump):
--   * `Quest_Sub_Reward_C` < `UAT_UIQuestSubReward` (AT.hpp:35501). Its nested window
--     `Xcmn_Win01` @0x540 carries the title; `UIRewardWindow` @0x3C8 is the native twin.
--   * The reward ROWS are `Xcmn_Win01_List_C` instances — `Txt_Item` @0x428 (the name) and
--     `Txt_Num` @0x430 (the amount). Same row class the difficulty picker uses, which is why
--     this adapter must sit ABOVE screen_choicelist: otherwise the choice reader claims the
--     rows and announces the rewards as if they were options to pick from.
--   * `Lang_Txt_Congrats` is a UImage — the "¡FELICITACIONES!" banner carries NO text, so it
--     is deliberately unread. The title says the same thing in words the game itself localizes.
--
-- NOTICE PATTERN (screen_fishresult): speaks each line ONCE per appearance, queued so it never
-- interrupts, and releases the dispatcher immediately — a reward sheet must never hold the
-- screen. The forget is DEBOUNCED so the sheet's own fade/flip does not re-read the whole list.
--
-- LIVENESS, corrected 2026-07-28. The first cut gated on `Core.pane_live`, and that is why this
-- adapter NEVER RAN: pane_live demands `GetVisibility() == Visible(0)`, but a passive overlay in
-- this game renders as HitTestInvisible (the Xcmn_Subtitles precedent), so the host was rejected
-- on every single tick. The proof was a one-line log check — `screen -> screen_questreward` never
-- appears, not once, while the user's F7 census taken WITH the sheet on screen shows its title and
-- all four reward rows rendered and fully opaque. Now `Core.pane_rendered` (opacity-only), the
-- same gate screen_fishresult needed for the same reason nine days earlier; the visibility half of
-- pane_live exists to stop a pooled INTERACTIVE pane shadowing the adapters below it, and a notice
-- that releases the dispatcher on the same tick cannot shadow anything.

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")

local Reward = {}

-- Appears mid free-roam right after a substory ends: the radar keeps running.
Reward.nav_mute = false
-- Passive notice: its prompts are not choices the player makes.
Reward.keyhelp_auto = false

local HOST_BP     = "Quest_Sub_Reward_C"
local HOST_NATIVE = "AT_UIQuestSubReward"   -- in case an instance of another subclass shows up
local ROW_CLASS   = "Xcmn_Win01_List_C"
local GONE_TICKS  = 3   -- polls the sheet must be genuinely absent before the dedup re-arms

local tick = 0
local spoken = {}
local queue = nil
local gone = 0

local function clean(t) return t and A.markup_to_speech(t) or nil end

-- First readable text among candidate member names. Every fetch goes through Core.member: the
-- names below are ALTERNATIVES, so most of them are expected NOT to exist on any given class,
-- and asking a class for a member it does not declare is an uncatchable abort that no pcall on
-- this stack can contain (the 2026-07-26 screen_toasts crash).
local function node_text(owner, ...)
    if not Core.valid(owner) then return nil end
    for _, m in ipairs({ ... }) do
        local node = Core.member(owner, m)
        if Core.valid(node) and Core.is_visible(node) then
            local t = clean(Core.read_text(node))
            if t and t ~= "" then return t end
        end
    end
    return nil
end

-- The sheet's heading ("Recompensas de historia"). It lives on the nested window, with the
-- host's own node and the native twin as fallbacks.
local function title_of(host)
    local t = node_text(host, "Txt_Title")
    if t then return t end
    for _, w in ipairs({ "Xcmn_Win01", "UIRewardWindow" }) do
        local win = Core.member(host, w)
        t = node_text(win, "Txt_Title", "WL_TxtTitle")
        if t then return t end
    end
    return nil
end

-- The reward rows, in ON-SCREEN order. The rows are separate pooled widgets rather than
-- children the host exposes as an array, so they are collected from the class pool and ordered
-- by where they actually sit (Core.slot_pos — widget NUMBERS are slot ids, not positions; the
-- keyhelp lesson). Only rows that are genuinely rendered count, which is what keeps a stale
-- pooled row from a previous window out of the list.
local function reward_rows()
    local rows = {}
    for _, r in ipairs(Core.cached_all(ROW_CLASS, tick)) do
        if Core.valid(r) and Core.on_screen(r) then
            local item = node_text(r, "Txt_Item")
            if item then
                rows[#rows + 1] = { y = Core.slot_pos(r, "Y") or 0,
                                    item = item, num = node_text(r, "Txt_Num") }
            end
        end
    end
    table.sort(rows, function(a, b) return a.y < b.y end)
    return rows
end

local function lines(host)
    local out = {}
    local title = title_of(host)
    if title then out[#out + 1] = title end
    for _, r in ipairs(reward_rows()) do
        -- Spoken the way it reads on screen: name first, amount second ("EXP, 4583").
        out[#out + 1] = Core.phrase(r.item, r.num)
    end
    -- A title with no rows is the sheet mid-animation, not a reward: say nothing yet.
    if #out < 2 then return {} end
    return out
end

function Reward.is_active()
    tick = tick + 1
    queue = nil
    local host = Core.first_on_screen(HOST_BP, tick)
        or Core.first_on_screen(HOST_NATIVE, tick)
    if not host or not Core.pane_rendered(host) then
        -- Debounced forget (the fishresult lesson): the close/fade can drop the sheet for a
        -- tick or two, and clearing `spoken` at once would re-read the whole list.
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
    -- Active only while there is something NEW to say; released right after.
    return queue ~= nil
end

function Reward.reset() end

function Reward.update()
    if not queue then return end
    for _, line in ipairs(queue) do
        spoken[line] = true
        Speech.say(line, false)   -- queued: follows the sheet's reveal animation
    end
    queue = nil
end

return Reward
