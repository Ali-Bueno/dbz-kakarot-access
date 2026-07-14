-- Automatic reader for the contextual button prompts: the actions a screen offers
-- ("X: assign to a slot", "Y: skill tree", "A: use"). A sighted player reads them off the
-- keyhelp bar at a glance; without this they are invisible, and the choices they announce
-- (the skill palette, the skill tree, using an item) are whole features you'd never find.
--
-- Cross-screen by design: it hangs off the registry's dispatcher, NOT off each adapter, so
-- every menu — including ones not written yet — reads its actions for free (CLAUDE.md §8:
-- classify widgets, never screens). A passive/time-critical reader opts OUT with
-- `keyhelp_auto = false` (its prompts are urgent, or it already speaks them itself).
--
-- What it speaks and when, per the verbosity rules in CLAUDE.md §9:
--   * ACTION entries only — the cursor-movement ones ("move", "change tab") are dropped by
--     Keyhelp.actions(); they carry no choice;
--   * once on entering a screen, then ONLY when the set of actions CHANGES (diff-gated):
--     hovering an item you can't use drops "A: use", and that IS news;
--   * always queued (interrupt=false): the screen's own readout (title + focused item) and
--     any cursor move the player makes both outrank it.

local Keyhelp = require("keyhelp")
local Speech = require("speech")

local W = {}

-- COST RULE (the 2026-07-14 lag regression): this runs inside the reader's poll step, so it
-- may only touch the budgeted widget cache (Core.cached_all via Keyhelp, tick passed) and it
-- polls the bar's LABELS, never its glyphs — resolving 9 glyphs per poll stalled the game
-- thread and left the item / skill-palette menus reading late or not at all. The glyphs are
-- resolved ONCE, on the poll where the labels actually changed.
local POLL_EVERY = 5     -- ticks (Core.POLL_MS = 100 ms) between bar probes: a menu's actions
                         -- change on a cursor move, which no human makes twice in half a second
local STABLE_POLLS = 2   -- a new set must survive one more poll before it's spoken: a menu
                         -- animates in, so the first frames of its bar can be half-built

local enabled = true
local armed = false      -- the active screen wants the automatic read
local spoken_sig = nil   -- label signature of the last bar announced (nil = announce the next)
local spoken_phrase = nil   -- and what it actually said (a nav-only label change must not repeat it)
local cand, cand_n = nil, 0
local tick = 0           -- MONOTONIC (never reset on a screen change): it doubles as the
                         -- cache's clock, and Core.cached_all schedules its next re-scan at
                         -- tick + REFRESH_EVERY — a counter that restarts would never reach it

local function clear()
    spoken_sig, spoken_phrase, cand, cand_n = nil, nil, nil, 0
end

-- The registry calls this on a CONFIRMED screen change (and on a map transition, with no
-- adapter): forget what the old screen said, and decide whether the new one auto-reads.
function W.screen_changed(adapter)
    clear()
    armed = enabled and adapter ~= nil and adapter.keyhelp_auto ~= false
end

-- Called after the active adapter's update(), on the game thread, transition-gated by the
-- registry (probing a widget mid-teardown is an uncatchable abort).
function W.update()
    if not armed then return end
    tick = tick + 1
    if tick % POLL_EVERY ~= 0 then return end

    local sig = Keyhelp.label_sig(tick)
    -- No bar yet (it builds a beat after the screen) or nothing new: keep watching. An
    -- EMPTY bar is never announced as a change — a screen mid-teardown blanks it first.
    if sig == "" or sig == spoken_sig then cand, cand_n = nil, 0 return end

    if sig ~= cand then cand, cand_n = sig, 1 return end
    cand_n = cand_n + 1
    if cand_n < STABLE_POLLS then return end

    spoken_sig, cand, cand_n = sig, nil, 0
    -- Only now (the labels really moved) is it worth resolving the glyphs.
    local phrase = Keyhelp.phrase(Keyhelp.actions(tick))
    if phrase and phrase ~= spoken_phrase then
        spoken_phrase = phrase
        Speech.say(phrase, false)
    end
end

-- Toggle the automatic read (the on-demand F2 read is unaffected). Returns the new state.
function W.toggle()
    enabled = not enabled
    armed = false          -- re-armed by the next screen change; no mid-screen blurt
    clear()
    return enabled
end

function W.is_enabled() return enabled end

return W
