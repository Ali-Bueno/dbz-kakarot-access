-- Screen adapter: overworld Characters menu (Start_Char_C -> UAT_UIStartChar).
--
-- WL_StartCharBarList is a FIXED C ARRAY of UAT_UIStartCharBar, and UE4SS collapses a fixed array
-- to element 0 — so `host.WL_StartCharBarList` is slot 0, NOT "the bar you are on". The original
-- header here claimed the collapsed member "updates as you move"; that held with a single
-- character (slot 0 is the only one) and was wrong the moment there were several, which is exactly
-- the user's report (2026-07-25: "cuando hay más de un personaje, al moverme entre ellos no se
-- narra"). Nothing in the old code could ever change what it read, so the diff gate swallowed
-- every cursor move. `arr[i]` yields garbage and `GetArrayNum`/`#arr` ABORT the game, so the array
-- cannot be walked either.
--
-- FIX = the screen_party recipe, with every constant taken from the CXX header rather than guessed
-- (AT.hpp:36402 `UAT_UIStartCharBar* WL_StartCharBarList; // 0x03B0 (size: 0x30)` → base 0x3B0,
-- 0x30/0x8 = 6 slots): register each slot's raw pointer as its own property with
-- RegisterCustomProperty, then pick the SELECTED bar by the cursor marker the bar class itself
-- exposes (AT.hpp:36433 `UBorder* Pnl_Curs_All` @0x410, `UImage* Img_Curs00` @0x438) — the same
-- "find the highlight, don't compute the index" pattern as screen_saveload's Border_Cursor and
-- screen_skillcustom's SelectActiveBorder. Marker-first matters because the list is VIRTUALISED:
-- the host exposes SetViewIndexAndCursorIndex / GetViewIndex / GetCursorIndex, so the cursor index
-- is absolute over all characters while the bars are only the visible window — index arithmetic
-- would need the scroll offset and would break at the ends. The index is kept as the fallback.
--
-- The screen has SEVERAL pooled instances (Start_Char_C_N) and only one is on_screen at a time,
-- so we pick the live one each tick. Fully guarded: any failed read -> inactive (harmless).

local Core = require("ui_core")
local CharBar = require("ui_charbar")
local I18n = require("i18n")

local Chars = {}

local ann = Core.make_announcer()
local host, tick = nil, 0

-- Six rows (AT.hpp:36402, WL_StartCharBarList @0x3B0, declared size 0x30 / 8 = 6), reached by
-- their Blueprint WidgetTree names — `Start_Char_C_1.WidgetTree_0.Start_Char_Bar00..05` in the
-- 2026-07-28 F7 census. See ui_charbar.lua for why the earlier RegisterCustomProperty route was
-- withdrawn: it stopped resolving after a map transition and left this list reading only row 1,
-- which is exactly the "moving the cursor announces nothing" bug.
local roster = CharBar.new({ name = "Start_Char_Bar%02d", count = 6 })

-- Is the screen interactive? GetCursorIndex is reflected (AT.hpp:36402) and returns -1 while the
-- list is not focused, which is the gate that keeps this adapter dormant behind the stats sheet /
-- palette / skill tree that open over it.
local function has_cursor()
    if not Core.valid(host) then return false end
    local ok, idx = pcall(function() return host:GetCursorIndex() end)
    return ok and type(idx) == "number" and idx >= 0
end

-- A bar with no name is skipped here (verified behaviour): on this screen an unnamed bar is an
-- empty tail slot, not a selectable row.
local function read_bar(bar)
    local r = CharBar.read(bar)
    if not r or not r.name then return nil end
    return r
end

-- The bar the cursor is on: the marked one, else the one the cursor index points at within the
-- visible window, else slot 0 (the old behaviour — so a screen that exposes neither marker nor a
-- usable index still reads exactly as much as it did before).
local function selected_row()
    if not Core.valid(host) then return nil end
    local list = roster:bars(host)
    if #list == 0 then return nil end
    -- INDEX FIRST on this screen (order flipped 2026-07-28). GetCursorIndex/GetViewIndex are
    -- reflected UFunctions on UAT_UIStartChar (AT.hpp:36425-36427), so the selection is a
    -- deterministic read; the visual markers are a guess about which node the game drives, and a
    -- marker that never clears is unfalsifiable. Absolute cursor minus the scroll offset gives the
    -- position within the visible window.
    local cur, view
    pcall(function() cur = host:GetCursorIndex() end)
    pcall(function() view = host:GetViewIndex() end)
    if type(cur) == "number" then
        local pos = cur - (type(view) == "number" and view or 0)
        if pos >= 0 and pos < #list then return read_bar(list[pos + 1]) end
    end
    for _, bar in ipairs(list) do
        if CharBar.marked(bar) then return read_bar(bar) end
    end
    return read_bar(list[1])
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
