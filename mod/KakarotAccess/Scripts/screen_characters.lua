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
local I18n = require("i18n")

local Chars = {}

local ann = Core.make_announcer()
local host, tick = nil, 0

-- Slot 0 is reachable as the collapsed UPROPERTY; 1..5 are recovered by offset.
local BAR_BASE  = 0x3B0   -- AT.hpp:36402, UAT_UIStartChar.WL_StartCharBarList
local BAR_SLOTS = 6       -- its declared size 0x30 / 8 bytes per pointer
local SLOT_PROP = { [0] = "WL_StartCharBarList" }
for i = 1, BAR_SLOTS - 1 do SLOT_PROP[i] = "StartCharBar" .. i end

-- Registered lazily against the host's RUNTIME class path (screen_community's approach): the live
-- object is a Blueprint subclass, so registering on the native class name would not apply to it.
local registered = false
local function ensure_registered()
    if registered or not Core.valid(host) then return end
    local cls
    pcall(function() cls = host:GetClass():GetFullName():match("%s(.+)$") end)
    if not cls then return end
    registered = true
    for i = 1, BAR_SLOTS - 1 do
        pcall(function()
            RegisterCustomProperty({
                ["Name"] = SLOT_PROP[i],
                ["Type"] = PropertyTypes.ObjectProperty,
                ["BelongsToClass"] = cls,
                ["OffsetInternal"] = BAR_BASE + i * 8,
            })
        end)
        -- Custom properties are invisible to ForEachProperty, so Core.member's existence gate has
        -- to be told about them or it would refuse every slot but the first.
        Core.allow_member(SLOT_PROP[i])
    end
end

-- Is the screen interactive? GetCursorIndex is reflected (AT.hpp:36402) and returns -1 while the
-- list is not focused, which is the gate that keeps this adapter dormant behind the stats sheet /
-- palette / skill tree that open over it.
local function has_cursor()
    if not Core.valid(host) then return false end
    local ok, idx = pcall(function() return host:GetCursorIndex() end)
    return ok and type(idx) == "number" and idx >= 0
end

-- Does this bar carry the cursor? Either marker counts: which one the game drives is not
-- documented, and requiring both would silence the screen if only one is used.
local function bar_selected(bar)
    return Core.is_visible(Core.member(bar, "Pnl_Curs_All"))
        or Core.is_visible(Core.member(bar, "Img_Curs00"))
end

-- Every populated bar of the visible window, in slot order.
local function bars()
    local list = {}
    for i = 0, BAR_SLOTS - 1 do
        local bar = Core.member(host, SLOT_PROP[i])
        if Core.valid(bar) then list[#list + 1] = bar end
    end
    return list
end

-- Read one bar. Uses read_text (mainTxt, else GetText) since Txt_Lv/Txt_Power_Num render their
-- value via the parent FText. Txt_Name_Guest is the guest-character slot's own name node.
local function read_bar(bar)
    local name = Core.read_text(Core.member(bar, "Txt_Name"))
        or Core.read_text(Core.member(bar, "Txt_Name_Guest"))
    if not name then return nil end
    return {
        name  = name,
        lv    = Core.read_text(Core.member(bar, "Txt_Lv")),         -- e.g. "Lvl 2"
        pow   = Core.read_text(Core.member(bar, "Txt_Power")),      -- "BP" label
        num   = Core.read_text(Core.member(bar, "Txt_Power_Num")),  -- e.g. "365"
    }
end

-- The bar the cursor is on: the marked one, else the one the cursor index points at within the
-- visible window, else slot 0 (the old behaviour — so a screen that exposes neither marker nor a
-- usable index still reads exactly as much as it did before).
local function selected_row()
    if not Core.valid(host) then return nil end
    ensure_registered()
    local list = bars()
    if #list == 0 then return nil end
    for _, bar in ipairs(list) do
        if bar_selected(bar) then return read_bar(bar) end
    end
    local cur, view
    pcall(function() cur = host:GetCursorIndex() end)
    pcall(function() view = host:GetViewIndex() end)
    if type(cur) == "number" then
        local pos = cur - (type(view) == "number" and view or 0)
        if pos >= 0 and pos < #list then return read_bar(list[pos + 1]) end
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
