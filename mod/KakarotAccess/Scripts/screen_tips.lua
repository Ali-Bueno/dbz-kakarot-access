-- Screen adapter: tutorial / tip pages (Tips_C -> UAT_UITips).
--
-- Tips_C is the game's multi-page tutorial widget (loading screens, the intro tutorial fight,
-- pause "View Controls", the System > Tutorials viewer). It is a 2-page book: BOTH windows
-- (Tips_Win00/01) stay loaded, and which one is the visible FRONT page is a non-reflected C++
-- member. The widgets' own page numbers / canvases lag by a navigation step, so instead we read
-- the native front-window index (UAT_UITips + 0x420: 0 = Tips_Win00, 1 = Tips_Win01, CONFIRMED
-- by F4) and read THAT window directly — correct immediately on both forward and back navigation.
--
-- Within a window, each page uses ONE layout shown via its own canvas; the matching native
-- TextBox holds that page's text (inactive layouts keep stale text), so we read only the layout
-- whose canvas is on. Header Txt_Title/Subtitle/Page; the controls layout reads its command rows
-- as "name: button". All text goes through A.markup_to_speech (strips <span>, resolves glyphs).

local Core = require("ui_core")
local A = require("ui_archetypes")
local Mem = require("mem")
local OFF = require("native_offsets")
local Speech = require("speech")

local Tips = {}

local WINDOWS = { "Tips_Win00", "Tips_Win01" }
local LAYOUTS = {
    { canvas = "Canvas_TextOnly",     box = "TextBox_TextOnly" },
    { canvas = "Canvas_TextAndImage", box = "TextBox_TextAndImage" },
    { canvas = "Canvas_BNID",         box = "TextBox_BNID" },
    { canvas = "Canvas_BigImage",     box = "TextBox_Detail03" },
}
local LIST_ITEMS = {
    "Tips_List00_L00_00", "Tips_List00_L00_01", "Tips_List00_L00_02",
    "Tips_List00_L01_00", "Tips_List00_L01_01", "Tips_List00_L01_02",
    "Tips_List00_R00_00", "Tips_List00_R00_01", "Tips_List00_R00_02",
    "Tips_List00_R01_00", "Tips_List00_R01_02", "Tips_List00_R01_03",
}

local tips = nil
local tick = 0
-- The native front-window index gives the current page directly, so we track a single "spoken"
-- content: navigating BACK to an already-seen page reads it again (front content changes), while
-- sitting on a page doesn't repeat it. A short debounce absorbs the 1-tick page-turn transition.
local STABLE_TICKS = 2
local spoken = nil     -- last announced front-page content
local pend = nil       -- { c = candidate content, n = consecutive ticks seen }
local current = nil    -- the front window's content this tick

-- Speakable text of an Xcmn_MultiLineText node: its full markup via reflected GetText(), run
-- through markup_to_speech. nil if empty.
local function node(n)
    if not Core.valid(n) then return nil end
    local ok, s = pcall(function() return n:GetText():ToString() end)
    return (ok and s and A.markup_to_speech(s)) or nil
end

-- The window's "X/Y" page line, but ONLY when it is fresh: single-page tutorials leave a stale
-- count (e.g. "3/3") from the previous tutorial, so we keep it only when its page X matches the
-- native current-page index (0-based) + 1. nil when stale or a 1-page tutorial.
local function page_line(w, pidx)
    local s = node(w.TextBox_Page)
    if not s then return nil end
    if not pidx then return s end
    local x = tonumber(s:match("^(%d+)"))
    if x and (x - 1) == pidx then return s end
    return nil
end

-- One window's speakable content: header + ONLY the currently-active layout (the controls list,
-- or the one text box whose canvas is on) so stale text from other layouts is never mixed in.
-- pidx = native current-page index (0-based) used to drop a stale page count.
local function read_window(w, pidx)
    if not Core.valid(w) then return nil end
    local controls = Core.on_screen(w.Canvas_KeyTips)
    local body = nil
    if not controls then
        for _, L in ipairs(LAYOUTS) do
            if Core.on_screen(w[L.canvas]) then body = node(w[L.box]); break end
        end
    end

    local parts = {}
    local function add(s) if s and s ~= "" then parts[#parts + 1] = s end end
    add(node(w.TextBox_Title)); add(node(w.TextBox_SubTitle)); add(page_line(w, pidx))

    if controls then
        -- Controls-reference layout: read the command rows as "name: button".
        for _, it in ipairs(LIST_ITEMS) do
            local item = w[it]
            if Core.valid(item) then
                local name, btn = node(item.Txt_Detail), node(item.Txt_Btn)
                if name and btn then add(name .. ": " .. btn)
                elseif name then add(name) end
            end
        end
    else
        add(body)
    end
    return #parts > 0 and table.concat(parts, ", ") or nil
end

function Tips.is_active()
    tick = tick + 1
    tips = Core.cached_live("Tips_C", tick)   -- cheap: cached ref, no per-tick scan
    if not Core.on_screen(tips) then spoken, pend, current = nil, nil, nil return false end
    current = nil
    -- Native front-window index (0x420): 0 = Tips_Win00, 1 = Tips_Win01. Read that window; the
    -- native page index (0x424) drops a stale page count on single-page tutorials.
    local fw = Mem.i32(tips, OFF.tips.frontWindow)
    local pidx = Mem.i32(tips, OFF.tips.pageIndex)
    local wn = WINDOWS[(fw or 0) + 1]
    if wn then
        local w = tips[wn]
        if Core.on_screen(w) then current = read_window(w, pidx) end
    end
    return current ~= nil
end

function Tips.reset() end

function Tips.update()
    if not current then return end
    if current == spoken then pend = nil return end
    -- New front page: require it stable across a couple of ticks (absorbs the page-turn
    -- transition), then speak it once.
    if pend and pend.c == current then
        pend.n = pend.n + 1
        if pend.n >= STABLE_TICKS then spoken = current; pend = nil; Speech.say(current, true) end
    else
        pend = { c = current, n = 1 }
    end
end

return Tips
