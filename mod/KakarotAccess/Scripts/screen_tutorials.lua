-- Screen adapter: System > Tutorials list (Start_Tips_C -> UAT_UIStartTips).
--
-- The topic list UIXlistTutorial (UAT_UICmn03MenuBar10 -> MenuBarBase03) is a SCROLLING list:
-- only ~9 rows (UAT_UIBar03Choice) are pooled in ListPlateCtn, but GetSelectValue() returns the
-- ABSOLUTE index (which exceeds the pool once scrolled), so indexing ListPlateCtn by it misses.
-- Instead we scan the pooled rows and read the one whose cursor highlight (Ins_Cursor_Fad /
-- CursorFade) is visible — the actual selection, robust to scrolling and left/right navigation.
-- The category tab (Adventure / Controls / The Main Menu) is the plain text TextBox_Category.
--
-- Confirming a topic opens the Tips book (UAT_UITips), handled by screen_tips. Fully guarded.

local Core = require("ui_core")
local I18n = require("i18n")

local Tut = {}

local ann = Core.make_announcer()
local host, tick = nil, 0
local cached_name = nil   -- selected topic label, computed once per tick in is_active

-- The selected topic's label: the pooled row whose cursor highlight (Ins_Cursor_Fad) is visible.
-- Scans the ~9 pooled rows; returns on the first match so non-selected rows cost one check each.
local function selected_topic()
    local list
    if not pcall(function() list = host.UIXlistTutorial end) or not Core.valid(list) then return nil end
    local plates, num = Core.array_of(list, "ListPlateCtn")
    if not plates then return nil end
    for i = 1, num do
        local row
        if pcall(function() row = plates[i] end) and Core.valid(row) then
            local sel
            pcall(function() sel = Core.is_visible(row.Ins_Cursor_Fad) end)
            if sel then return Core.read_text(Core.member(row, "Txt_List")) end
        end
    end
    return nil
end

function Tut.is_active()
    tick = tick + 1
    host = Core.first_on_screen("AT_UIStartTips", tick)
    if not host then cached_name = nil return false end
    cached_name = selected_topic()   -- computed ONCE here; update() reuses it (no second scan)
    return cached_name ~= nil
end

function Tut.reset() ann:reset() end

function Tut.update()
    if not cached_name then return end
    local tab = Core.read_text(host.TextBox_Category)
    -- screen name on entry; tab (category) on tab change; the focused topic as it moves.
    ann:focus(I18n.startlist(12), tab, cached_name, nil, nil)
end

return Tut
