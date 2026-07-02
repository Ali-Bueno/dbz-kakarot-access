-- Screen adapter: tutorial / tip pages (Tips_C -> UAT_UITips).
--
-- Tips_C is the game's multi-page tutorial widget (loading screens, the intro tutorial
-- fight, pause "View Controls"). Reverse-engineered behaviour (CXX headers + runtime dumps):
--   * It holds TWO windows (Tips_Win00/01) shown together like a 2-page book; a 3-page
--     tutorial fills them across navigations. CRUCIALLY, consecutive pages are BYTE-IDENTICAL
--     in every readable widget property (enum/opacity/slot/Z-order/canvas) — only which one is
--     drawn on top changes — so which page is "current" CANNOT be detected by polling, and the
--     game exposes no page index (verified in the object dump: AT_UITips has only
--     MouseClickTabLeft/Right + anims, no CurrentPage).
--   * So instead of picking the current window, we read BOTH on-screen windows and speak each
--     UNIQUE page ONCE (dedup by content) for as long as Tips_C stays up — no page goes mute,
--     and a stale window (a previous page still loaded) is never re-read. The dedup set clears
--     when Tips_C closes.
--
-- Per window (UTips_Win_C): header Txt_Title/Subtitle/Page, then EITHER the command list
-- (Tips_List00_* -> Txt_Detail name + Txt_Btn glyph) when the controls canvas Canvas_KeyTips
-- is the shown layout, OR the explanation prose (Txt_Detail00/01/03/04) on the text layouts —
-- so a text page reads its explanation, not the persistent Guard/Burst/Charge footer. All text
-- goes through A.markup_to_speech (strips <span>, resolves <inputicon> glyphs to button names).

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")

local Tips = {}

local WINDOWS = { "Tips_Win00", "Tips_Win01" }
local DETAILS = { "Txt_Detail00", "Txt_Detail01", "Txt_Detail03", "Txt_Detail04" }  -- (no 02 in the widget)
local LIST_ITEMS = {
    "Tips_List00_L00_00", "Tips_List00_L00_01", "Tips_List00_L00_02",
    "Tips_List00_L01_00", "Tips_List00_L01_01", "Tips_List00_L01_02",
    "Tips_List00_R00_00", "Tips_List00_R00_01", "Tips_List00_R00_02",
    "Tips_List00_R01_00", "Tips_List00_R01_02", "Tips_List00_R01_03",
}

local tips = nil
local tick = 0
local announced = {}   -- page-content strings already spoken while Tips_C has been on screen
local current = nil    -- on-screen window contents collected this tick

-- Speakable text of an Xcmn_MultiLineText_C node: its full markup (plain label OR inline
-- glyph) via reflected GetText(), run through markup_to_speech. nil if empty.
local function node(n)
    if not Core.valid(n) then return nil end
    local ok, s = pcall(function() return n:GetText():ToString() end)
    return (ok and s and A.markup_to_speech(s)) or nil
end

-- One window's speakable content: header + EITHER the command list (controls layout) OR the
-- explanation prose (text layouts) — never both, so the prose isn't buried under the footer.
local function read_window(w)
    local parts = {}
    local function add(s) if s and s ~= "" then parts[#parts + 1] = s end end
    add(node(w.Txt_Title)); add(node(w.Txt_Subtitle)); add(node(w.Txt_Page))

    if Core.on_screen(w.Canvas_KeyTips) then
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
        -- Text layout: the explanation only (the Guard/Burst/Charge footer is a persistent
        -- reference on every page and would just repeat, so it's skipped here).
        for _, d in ipairs(DETAILS) do add(node(w[d])) end
    end
    return #parts > 0 and table.concat(parts, ", ") or nil
end

function Tips.is_active()
    tick = tick + 1
    tips = Core.cached_live("Tips_C", tick)   -- cheap: cached ref, no per-tick scan
    if not Core.on_screen(tips) then announced, current = {}, nil return false end
    current = {}
    for _, wn in ipairs(WINDOWS) do
        local w = tips[wn]
        if Core.on_screen(w) then
            local c = read_window(w)
            if c then current[#current + 1] = c end
        end
    end
    return #current > 0
end

-- Dedup set persists across focus switches (only cleared when Tips_C closes, in is_active), so
-- a brief interruption (a subtitle over the tutorial) doesn't make it re-read every page.
function Tips.reset() end

function Tips.update()
    local new = {}
    for _, c in ipairs(current or {}) do
        if not announced[c] then announced[c] = true; new[#new + 1] = c end
    end
    if #new > 0 then Speech.say(table.concat(new, ". "), true) end
end

return Tips
