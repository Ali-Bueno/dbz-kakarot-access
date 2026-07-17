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
--   * APPEARANCE-EDGE dedup (round 3, user bug 2026-07-17: cards re-announced on
--     every subtitle flip): a card that stays continuously on screen speaks
--     exactly ONCE. Round 4 (user: still repeated 2-3x per card): absence is
--     measured in WALL CLOCK, not ticks — the idle-throttled sweep runs ~300 ms
--     per tick in cinematics and camera cuts hide the card longer than any small
--     tick window, so a tick-based gap re-armed the same card on every cut. A
--     card text may only re-announce after GONE_S of genuine absence.

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")

local Card = {}

-- Cards overlay cinematics and never pause the game.
Card.nav_mute = false

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua).
Card.keyhelp_auto = false

local STABLE_TICKS = 2   -- text must hold this many consecutive ticks before speaking
local GONE_S = 30        -- seconds of absence before a card counts as genuinely gone
                         -- (cinematic camera cuts hide the card for shorter spans)

local tick = 0
local seen = {}      -- text -> { first=tick, last=os.clock, spoken=bool } per appearance
local queue = nil    -- texts to speak this tick (computed in is_active)

-- Fade-out ghost: opacity drops to 0 while visibility flags lag.
local function faded(h)
    local ok, op = pcall(function() return h:GetRenderOpacity() end)
    return ok and type(op) == "number" and op < 0.05
end

-- TEMP TRACE (2026-07-17 round 5, user: cards mostly silent again): one log line
-- whenever the observed card state CHANGES — pins where the pipeline loses them
-- (detection, gate, text read, dedup, or commit). Turn OFF once diagnosed.
local TRACE = true
local last_trace = nil

local function vis_of(h)
    local v = "?"
    pcall(function() v = tostring(h:GetVisibility()) end)
    return v
end

local function op_of(h)
    local o = "?"
    pcall(function() o = string.format("%.2f", h:GetRenderOpacity()) end)
    return o
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

-- "Name, popular title" from whichever size variant is visible. Second return =
-- trace detail (which member supplied each part).
local function card_text(host)
    local name, nsrc = nil, "-"
    for _, m in ipairs({ "NameTxt", "NameTxt_Large" }) do
        name = node_text(host, m)
        if name then nsrc = m break end
    end
    local pop, psrc = nil, "-"
    for _, m in ipairs({ "PopularNameTxt", "PopularNameTxt_Large" }) do
        pop = node_text(host, m)
        if pop then psrc = m break end
    end
    if not name and not pop then return nil, "no-text" end
    return Core.phrase(name, pop),
        string.format("name=%s@%s pop=%s@%s", tostring(name), nsrc, tostring(pop), psrc)
end

function Card.is_active()
    tick = tick + 1
    queue = nil
    local tr = TRACE and {} or nil
    local n = 0
    for _, host in ipairs(Core.cached_all("Info_Name_C", tick)) do
        if Core.valid(host) then
            n = n + 1
            local on = Core.on_screen(host)
            local fd = faded(host)
            local t, dbg = nil, nil
            if on and not fd then t, dbg = card_text(host) end
            if tr and n <= 3 then
                tr[#tr + 1] = string.format("h%d on=%s vis=%s op=%s %s",
                    n, tostring(on), vis_of(host), op_of(host), dbg or "-")
            end
            if t then
                local s = seen[t]
                local fresh = not s or (os.clock() - s.last) > GONE_S
                if fresh then
                    s = { first = tick, spoken = false }   -- a fresh appearance
                    seen[t] = s
                end
                s.last = os.clock()
                -- Stable and not yet spoken THIS appearance. `spoken` is set in
                -- update(), so a pending line survives losing ticks to a subtitle
                -- and speaks on the first free tick — exactly once.
                local stable = (tick - s.first + 1) >= STABLE_TICKS
                if tr then
                    tr[#tr + 1] = string.format("txt=%q fresh=%s stable=%s spoken=%s",
                        t, tostring(fresh), tostring(stable), tostring(s.spoken))
                end
                if not s.spoken and stable then
                    queue = queue or {}
                    queue[#queue + 1] = t
                end
            end
        end
    end
    if tr then
        local line = "cards n=" .. n .. " " .. table.concat(tr, " | ") ..
            (queue and " QUEUED" or "")
        if line ~= last_trace then
            last_trace = line
            print("[KakarotAccess] " .. line .. "\n")
        end
    end
    -- Active only while there is something new to say; released right after (notice
    -- pattern — the card must never hold the screen).
    return queue ~= nil
end

function Card.reset() end

function Card.update()
    if not queue then return end
    for _, t in ipairs(queue) do
        if seen[t] then seen[t].spoken = true end
        if TRACE then print("[KakarotAccess] cards SPOKE " .. t .. "\n") end
        Speech.say(t, false)   -- queued: never cuts a subtitle line
    end
    queue = nil
end

return Card
