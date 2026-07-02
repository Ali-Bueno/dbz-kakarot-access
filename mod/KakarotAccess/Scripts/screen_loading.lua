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

function Loading.is_active()
    tick = tick + 1
    loadC = Core.cached_live("Loading_C", tick)   -- cheap: cached ref, no per-tick scan
    if not Core.on_screen(loadC) then cached = nil return false end
    cached = content()
    return cached ~= nil
end

function Loading.reset() ann:reset() end

function Loading.update()
    ann:focus(nil, nil, cached, nil, nil)
end

return Loading
