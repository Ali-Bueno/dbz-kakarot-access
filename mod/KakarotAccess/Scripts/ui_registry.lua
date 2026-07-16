-- Registry + dispatcher for the hybrid UI architecture.
--
-- Holds the registered screen adapters and runs the single poll engine: each tick
-- it finds the first active (visible) screen, and on a screen change resets the
-- announcers (so the new screen re-announces from scratch), then drives the active
-- adapter's update(). Adding a screen = register it; no engine changes.

local Core = require("ui_core")
local Transition = require("transition")
local KeyhelpWatch = require("keyhelp_watch")
local GuideWatch = require("guide_watch")
local Input = require("input")

local Registry = {}

local adapters = {}
local active = nil
local enabled = false

-- Switch debounce: a screen must be the top active adapter for this many consecutive
-- polls before we commit to it. Stops a screen that's active for only a moment from
-- announcing — e.g. the title menu flashing on-screen for a tick at boot right before the
-- "Checking system data…" dialog appears (which would blurt "Main menu, Continue" first).
local CONFIRM_TICKS = 2
local pending, pending_n = nil, 0

-- Sticky-active sweep cadence. Polling all ~33 adapters' is_active() every tick was the
-- measured 40ms-avg game-thread step (Ctrl+F5, 2026-07-14): most of a sweep re-probes
-- screens that cannot have changed. While an adapter is active we poll ONLY it, and run
-- the full priority sweep every SWEEP_EVERY ticks — enough to catch a higher-priority
-- overlay (a dialog popping over a menu) within ~300ms. The sweep runs EVERY tick when
-- nothing is active (free-roam: menu-open detection stays at one tick) and while a
-- pending switch is being confirmed (so CONFIRM_TICKS still counts consecutive polls).
local SWEEP_EVERY = 3
local tick_n = 0

-- Menu-open boost from the PAD. The registry's commit boost cannot see ring→submenu:
-- the ring/pause stays visible (and active) under the submenu, so no screen change ever
-- commits and a scan-path submenu waits out its full backoff (the "items/emblems take
-- forever" flow, 2026-07-15). A confirm/back/start PRESS is the missing event — it is
-- the exact moment a submenu may be appearing. Read at the registry's own 100ms cadence
-- (a real menu press holds longer than one tick, so the edge is caught), and gated on
-- "some adapter is active": in combat/free-roam (no active adapter) button mashing must
-- never trigger scan work.
local TRACE_COMMITS = false -- 2026-07-16 emblems diagnosis: log screen commits by adapter
                            -- index (cross-ref app.lua order). Re-enable to name a
                            -- shadowing adapter when a screen reads late/never.

local BOOST_BTNS = Input.BTN.A | Input.BTN.B | Input.BTN.X | Input.BTN.Y | Input.BTN.START
local BOOST_COOLDOWN = 3    -- registry ticks between pad-triggered boost windows. Not 1:
                            -- an uncooled boost per press turned list navigation into a
                            -- permanent scan storm (57k scans / 1s spikes, 2026-07-15).
                            -- Not 10 either: a menu is often opened by the SECOND of two
                            -- quick A presses (submenu pick → confirm), and a 1s cooldown
                            -- swallowed that press's window while the first press's scan
                            -- had already consumed the class credit BEFORE the screen
                            -- existed — the "3-4s to read" residue. 300ms lets the
                            -- opening press always start a fresh window.
local prev_btn = 0
local boost_cool = 0
local function pad_boost()
    local snap = Input.read()
    if not snap then return end
    local fresh = snap.buttons & ~prev_btn
    prev_btn = snap.buttons
    if active ~= nil and (fresh & BOOST_BTNS) ~= 0 and tick_n >= boost_cool then
        boost_cool = tick_n + BOOST_COOLDOWN
        Core.boost_missing()
    end
end

function Registry.register(adapter)
    adapters[#adapters + 1] = adapter
end

local function sweep()
    for _, a in ipairs(adapters) do
        if a.is_active() then return a end
    end
    return nil
end

local function step()
    -- Transition gate FIRST (pure Lua): during a map switch the adapters' cached
    -- widgets are dying, and even an is_active() probe can be an uncatchable abort.
    -- Drop the active adapter so the post-load screen announces fresh.
    if Transition.active() then
        if active and active.reset then active.reset() end
        active, pending, pending_n = nil, nil, 0
        KeyhelpWatch.screen_changed(nil)
        return
    end

    tick_n = tick_n + 1
    pad_boost()
    local cur
    if active and pending == nil and tick_n % SWEEP_EVERY ~= 0 then
        -- Sticky fast path: the active screen is still up → keep it, poll nobody else.
        -- The moment it stops being active, fall through to a full sweep THIS tick.
        cur = active.is_active() and active or sweep()
    else
        cur = sweep()
    end

    if cur ~= active then
        -- Wait for the switch to be stable before committing; during the wait we announce
        -- nothing (neither the old nor the candidate screen). An adapter may ask for a
        -- longer confirm via `confirm_ticks` — the title menu does, so it doesn't blurt
        -- "Main menu, Continue" in the short gaps between boot dialogs (Checking system
        -- data / do not turn off / Saving data) where it's briefly the only screen up.
        local need = (cur and cur.confirm_ticks) or CONFIRM_TICKS
        if cur ~= pending then pending, pending_n = cur, 1 return end
        pending_n = pending_n + 1
        if pending_n < need then return end
        -- Context change confirmed: reset both so nothing leaks across the switch and the
        -- newly focused screen announces its current control fresh.
        if active and active.reset then active.reset() end
        if cur and cur.reset then cur.reset() end
        active, pending, pending_n = cur, nil, 0
        -- TEMP diagnosis (2026-07-16 emblems latency): one line per screen change naming
        -- the winning adapter by its registration index (cross-ref app.lua order; 0 =
        -- none). Names any adapter silently shadowing a late-registered one. Turn OFF
        -- once the emblems first-visit timeline is closed.
        if TRACE_COMMITS then
            local idx = 0
            if cur then
                for i, a in ipairs(adapters) do
                    if a == cur then idx = i break end
                end
            end
            print(string.format("[KakarotAccess] screen commit -> adapter #%d t=%.2f\n",
                idx, os.clock()))
        end
        -- New context: the screen's actions ("X: assign", "Y: skill tree") are read once,
        -- queued behind its own announcement (keyhelp_watch.lua).
        KeyhelpWatch.screen_changed(cur)
        -- A screen just opened or closed — the one moment a not-yet-cached submenu may be
        -- appearing (ring → items, items → detail, close → reopen). Let missing pools skip
        -- their backoff for a short window so the new screen reads at once (ui_core).
        Core.boost_missing()
    else
        pending, pending_n = nil, 0
    end

    if active then
        active.update()
        KeyhelpWatch.update()
        -- Tutorial guidance line (guide_watch.lua): queued after the screen's own
        -- readout, same slot as the keyhelp read.
        GuideWatch.update()
    end
end

function Registry.start()
    if enabled then return end
    enabled = true
    active, pending, pending_n = nil, nil, 0
    Core.loop(step, function() return enabled end)
end

function Registry.stop()
    enabled = false
    KeyhelpWatch.screen_changed(nil)   -- reader off: nothing pending, nothing remembered
end

function Registry.is_enabled() return enabled end

-- The currently active screen adapter (nil while free-roaming). The nav tracker
-- gates its audio cues on this: a menu/dialog owning the screen silences the radar.
function Registry.active_adapter() return active end

-- Registration-order index of the active adapter (nil if none) — for diagnostics
-- (adapters are anonymous tables; the index maps back to the app.lua register order).
function Registry.active_index()
    for i, a in ipairs(adapters) do
        if a == active then return i end
    end
    return nil
end

function Registry.toggle()
    if enabled then Registry.stop() else Registry.start() end
    return enabled
end

-- Re-announce the current selection (F1): reset the active announcer so update()
-- speaks it again next tick. An adapter that suppresses repeats on its own (the
-- resident controls guide speaks each content only once) provides reannounce()
-- instead, so F1 can override its once-only gate.
function Registry.repeat_current()
    if not active then return end
    if active.reannounce then active.reannounce()
    elseif active.reset then active.reset() end
end

return Registry
