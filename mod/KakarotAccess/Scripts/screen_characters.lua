-- Screen adapter: overworld Characters menu (Start_Char_C -> UAT_UIStartChar).
--
-- The screen is a single-bar carousel: WL_StartCharBarList is declared as a fixed array of
-- UAT_UIStartCharBar, but UE4SS COLLAPSES it to a single object = the CURRENTLY shown bar
-- (confirmed: host.WL_StartCharBarList.Txt_Name reads the selected character directly, and it
-- updates as you move). So we read that bar's Txt_Name / Txt_Lv directly — NO indexing (arr[i]
-- yields garbage) and NO array methods (GetArrayNum/#arr ABORT the game). GetCursorIndex() is
-- used only to confirm the screen is interactive.
--
-- The screen has SEVERAL pooled instances (Start_Char_C_N) and only one is on_screen at a time,
-- so we pick the live one each tick. Fully guarded: any failed read -> inactive (harmless).

local Core = require("ui_core")
local I18n = require("i18n")

local Chars = {}

local ann = Core.make_announcer()
local host, tick = nil, 0

local function has_cursor()
    if not Core.valid(host) then return false end
    local ok, idx = pcall(function() return host:GetCursorIndex() end)
    return ok and type(idx) == "number" and idx >= 0
end

-- The currently shown character bar, read directly off the collapsed member. Uses read_text
-- (mainTxt, else GetText) since Txt_Lv/Txt_Power_Num render their value via the parent FText.
local function selected_row()
    if not Core.valid(host) then return nil end
    local bar = host.WL_StartCharBarList
    if not Core.valid(bar) then return nil end
    local name = Core.read_text(bar.Txt_Name)
    if not name then return nil end
    return {
        name  = name,
        lv    = Core.read_text(bar.Txt_Lv),         -- e.g. "Lvl 2"
        pow   = Core.read_text(bar.Txt_Power),      -- "BP" label
        num   = Core.read_text(bar.Txt_Power_Num),  -- e.g. "365"
    }
end

function Chars.is_active()
    tick = tick + 1
    host = Core.first_on_screen("AT_UIStartChar", tick)
    if not host then return false end
    return has_cursor()
end

function Chars.reset() ann:reset() end

function Chars.update()
    local row = selected_row()
    if not row then return end
    -- "Characters" on entry; then name, level, BP label + value (diff-gated on the whole phrase,
    -- so it re-reads when the selected character changes).
    ann:focus(I18n.startlist(5), nil,
        Core.phrase(row.name, row.lv, Core.phrase(row.pow, row.num)), nil, nil)
end

return Chars
