-- Screen adapter: loading / story-recap screen (Loading_C + Tips_C).
--
-- After actions like choosing difficulty the game shows a load screen whose USEFUL text
-- (Story Recap, Adventure Tips + the tip body) lives under Tips_C, alongside the Loading_C
-- bar. The word "Loading" itself is a baked texture (Lang_Loading), not text, so there is
-- nothing to read there — we read every on-screen text box under those containers and
-- announce it, re-announcing when the recap/tip changes.
--
-- Text is only scanned while a loading container is actually on screen, so this costs
-- nothing during normal play.

local Core = require("ui_core")

local Loading = {}

local ann = Core.make_announcer()
local loadC, tipsC = nil, nil
local tick = 0
local cached = nil

local function up() return Core.on_screen(loadC) or Core.on_screen(tipsC) end

-- All on-screen text under whichever loading containers are up, joined in widget order.
local function content()
    local prefixes = {}
    for _, c in ipairs({ loadC, tipsC }) do
        if Core.on_screen(c) then prefixes[#prefixes + 1] = c:GetFullName():match("%s(.+)$") end
    end
    if #prefixes == 0 then return nil end
    local parts = {}
    for _, o in pairs(Core.cached_all("CFUIMultiLineTextBox", tick)) do
        if Core.is_visible(o) then   -- container already on_screen-gated → is_visible is enough (cheap)
            local fn = o:GetFullName()
            for _, p in ipairs(prefixes) do
                if p and fn:find(p, 1, true) then
                    local ok, s = pcall(function() return o.Text:ToString() end)
                    if ok and s and s ~= "" then parts[#parts + 1] = s end
                    break
                end
            end
        end
    end
    return #parts > 0 and table.concat(parts, ", ") or nil
end

function Loading.is_active()
    tick = tick + 1
    loadC = Core.cached_live("Loading_C", tick)   -- cheap: cached refs, no per-tick scan
    tipsC = Core.cached_live("Tips_C", tick)
    if not up() then cached = nil return false end
    cached = content()
    return cached ~= nil
end

function Loading.reset() ann:reset() end

function Loading.update()
    ann:focus(nil, nil, cached, nil, nil)
end

return Loading
