-- Notice reader: cinematic character-introduction cards (Info_Name_C ->
-- AT_UIInfoNameCore, driven by the sequencer's ATMovieSceneCharaIntroductionSection).
--
-- The little card some cinematics show when a character first appears ("Gohan —
-- hijo de Goku", "Piccolo — rival de Goku"): NameTxt + PopularNameTxt, or their
-- _Large twins when the section sets IsLargeSize — all reflected (ObjectDump,
-- AT_UIInfoNameCore 0x3A0..0x3C0; the visible pair is picked by the Switchars).
--
-- Instances are the BP subclass Info_Name_C (WidgetBlueprintGeneratedClass, super =
-- AT_UIInfoNameCore) — scanned by that name (the FindAllOf native-base gotcha).
-- fm.InfoName (AT_UIFieldManager 0x668) is deliberately NOT directory-mapped: the
-- fm auto-overlay fields have twice proven to stay null while the pooled widget
-- lives (InfoLevelUp, BattleRushSpeedCore) — the scan path serves it, and the
-- QUIET_EXEMPT entry in ui_core keeps it scannable mid-cinematic (cards appear
-- with no press, exactly when quiet mode reigns).
--
-- Read rules (first in-game round, 2026-07-17: only a bare "Goku" was heard):
--   * ALL on-screen instances are read, not just the first (cards can coexist).
--   * A card text must be STABLE for 2 consecutive ticks before speaking — the
--     name and the popular title animate in separately, so a one-tick partial
--     ("Goku") must never be spoken over the full line.
--   * Liveness is OPACITY-ONLY: passive overlays render as HitTestInvisible (the
--     Xcmn_Subtitles precedent), so the strict pane_live visibility==0 gate would
--     hold every card silent; the opacity check still drops the fade-out ghost.
--   * RECENT_S dedup instead of a single latch: the pooled card swaps texts in
--     place, and a re-shown card may legitimately repeat later.

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")

local Card = {}

-- Cards overlay cinematics and never pause the game.
Card.nav_mute = false

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua).
Card.keyhelp_auto = false

local RECENT_S = 10   -- seconds before an identical card text may re-announce

local tick = 0
local prev = {}      -- text -> true (present last tick; stability edge)
local recent = {}    -- text -> os.time() last spoken
local queue = nil    -- texts to speak this tick (computed in is_active)

-- Fade-out ghost: opacity drops to 0 while visibility flags lag.
local function faded(h)
    local ok, op = pcall(function() return h:GetRenderOpacity() end)
    return ok and type(op) == "number" and op < 0.05
end

local function node_text(host, member)
    local t
    pcall(function()
        local node = host[member]
        if Core.valid(node) and Core.is_visible(node) then
            t = A.markup_to_speech(Core.read_text(node))
        end
    end)
    return (t and t ~= "") and t or nil
end

-- "Name, popular title" from whichever size variant is visible.
local function card_text(host)
    local name = node_text(host, "NameTxt") or node_text(host, "NameTxt_Large")
    local pop  = node_text(host, "PopularNameTxt") or node_text(host, "PopularNameTxt_Large")
    if not name and not pop then return nil end
    return Core.phrase(name, pop)
end

function Card.is_active()
    tick = tick + 1
    queue = nil
    local cur = {}
    for _, host in ipairs(Core.cached_all("Info_Name_C", tick)) do
        if Core.valid(host) and Core.on_screen(host) and not faded(host) then
            local t = card_text(host)
            if t then cur[t] = true end
        end
    end
    local now = os.time()
    for t in pairs(cur) do
        -- Stable (seen last tick too) and not spoken recently.
        if prev[t] and (not recent[t] or now - recent[t] >= RECENT_S) then
            queue = queue or {}
            queue[#queue + 1] = t
        end
    end
    prev = cur
    -- Active only while there is something new to say; released right after (notice
    -- pattern — the card must never hold the screen).
    return queue ~= nil
end

function Card.reset() end

function Card.update()
    if not queue then return end
    local now = os.time()
    for _, t in ipairs(queue) do
        recent[t] = now
        Speech.say(t, false)   -- queued: never cuts a subtitle line
    end
    queue = nil
end

return Card
