-- Screen adapter: loading / story-recap screen (Loading_C).
--
-- After actions like choosing difficulty the game shows a load screen whose USEFUL text
-- (Story Recap + Adventure Tips body) lives under Loading_C. The word "Loading" itself is a
-- baked texture (Lang_Loading), not text, so there is nothing to read there — we read every
-- on-screen text box under Loading_C and announce it, re-announcing when the recap changes.
--
-- The separate multi-page tutorial widget Tips_C (control pages, "View Controls") is handled
-- structurally by screen_tips, NOT here — reading it flat mixed pages and buried the prose.
--
-- Text is only scanned while Loading_C is actually on screen, so this costs nothing in play.

local Core = require("ui_core")
local A = require("ui_archetypes")

local Loading = {}

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
Loading.keyhelp_auto = false

local ann = Core.make_announcer()
local loadC = nil
local tick = 0
local cached = nil

-- All on-screen text under Loading_C, markup-cleaned, consecutive duplicates collapsed.
local function content()
    if not Core.on_screen(loadC) then return nil end
    local prefix = loadC:GetFullName():match("%s(.+)$")
    if not prefix then return nil end
    local parts, last = {}, nil
    for _, o in pairs(Core.cached_all("Xcmn_MultiLineText_C", tick)) do
        if Core.on_screen(o) and o:GetFullName():find(prefix, 1, true) then
            local ok, s = pcall(function() return o:GetText():ToString() end)
            local clean = ok and s and A.markup_to_speech(s)
            if clean and clean ~= last then parts[#parts + 1] = clean; last = clean end
        end
    end
    return #parts > 0 and table.concat(parts, ", ") or nil
end

-- TEMP diagnosis (2026-07-16): the parked pane held the adapter ACTIVE for ~10s of
-- gameplay after a save load (silenced the ring, starved the sweep below — the emblems
-- latency run 2). One line per activation flip with the pane's real state, so if the
-- pane_live gate below fails to discriminate (the cooking ghost read vis=0 op=1.0!)
-- the log names the state we must gate on instead.
local LOAD_DEBUG = false
local last_active = false
local function trace(active)
    if not LOAD_DEBUG or active == last_active then return end
    last_active = active
    local vis, op = "?", "?"
    pcall(function() vis = tostring(loadC:GetVisibility()) end)
    pcall(function() op = string.format("%.2f", loadC.RenderOpacity) end)
    print(string.format("[KakarotAccess] loading pane active=%s vis=%s op=%s t=%.2f\n",
        tostring(active), vis, op, os.clock()))
end

function Loading.is_active()
    tick = tick + 1
    loadC = Core.cached_live("Loading_C", tick)   -- cheap: cached ref, no per-tick scan
    -- NO pane_live gate here (tried 2026-07-16, REVERTED same day): the run-2 "ghost"
    -- was a REAL 10s load (user), and run 3 showed the loading adapter never activating
    -- with the gate on — a non-interactive overlay like this likely renders under a
    -- non-Visible ESlateVisibility, which pane_live rejects by design. The trace stays
    -- to characterize the pane's states if it ever misbehaves.
    if not Core.on_screen(loadC) then cached = nil trace(false) return false end
    cached = content()
    trace(cached ~= nil)
    return cached ~= nil
end

function Loading.reset() ann:reset() end

function Loading.update()
    ann:focus(nil, nil, cached, nil, nil)
end

return Loading
