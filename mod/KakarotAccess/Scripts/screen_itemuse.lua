-- Screen adapter: item "use" character-select (AT_UIItemMenu char-select mode).
--
-- When you confirm a usable item in the inventory, the item menu enters a mode where you
-- pick WHICH character uses it: the item list stays, but a character card animates in on the
-- right (WL_Start_Party_Bars, a TArray<UAT_UIStartPartyBarEx*> on the AT_UIItemMenu host) and
-- the prompt "Select which character will use the item" shows at the bottom. Only the SELECTED
-- character's bar is animated on-screen at a time, so the on-screen bar IS the current
-- selection — no cursor index needed (verified from the screenshot: a single Gohan card).
--
-- Reads the character name + the level-up preview (Txt_Lv01 -> Txt_Lv02, e.g. "9 -> 11") the
-- card shows for the food item. Registered BEFORE the Start_Item_C list reader so it wins while
-- the char-select is up; when no bar is on-screen it stays inactive and the item list reader runs.

local Core = require("ui_core")
local I18n = require("i18n")

local ItemUse = {}

local ann = Core.make_announcer()
local host, tick = nil, 0

-- Diagnostic: on change, logs each bar's on-screen state + field values to
-- dumps/dump_itemuse.txt, so one capture (open an item's char-select, move if multiple chars)
-- confirms the detection signal and which field carries the name. Verified 2026-07-11; OFF.
local DEBUG = false
local last_dbg = nil
local function dbg(s)
    if not DEBUG or s == last_dbg then return end
    last_dbg = s
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_itemuse.txt", "a")
        if f then f:write(string.format("[%d] %s\n", os.time(), s)) f:close() end
    end)
end

local function bars_of()
    local bars
    if not pcall(function() bars = host.WL_Start_Party_Bars end) or bars == nil then return nil end
    local n
    if not pcall(function() n = #bars end) or type(n) ~= "number" then return nil end
    return bars, n
end

-- The on-screen character bar = the current selection (only the picked one animates in).
local function selected_bar()
    local bars, n = bars_of()
    if not bars then return nil end
    for i = 1, n do
        local bar = bars[i]
        if Core.valid(bar) and Core.on_screen(bar) then return bar end
    end
    return nil
end

-- Character name + level-up preview from a bar (UAT_UIStartPartyBarEx fields).
local function bar_text(bar)
    local name = Core.read_text(bar.Txt_Name01)
    if not name then return nil end
    local lv1 = Core.read_text(bar.Txt_Lv01)
    local lv2 = Core.read_text(bar.Txt_Lv02)
    local lvl
    if lv1 and lv2 then lvl = I18n.t("itemuse_levelup"):format(lv1, lv2)
    elseif lv1 then lvl = lv1 end
    return Core.phrase(name, lvl)
end

function ItemUse.is_active()
    tick = tick + 1
    host = Core.first_on_screen("Start_Item_C", tick)
    if not host then return false end
    local bar = selected_bar()
    if DEBUG and bar then
        dbg(string.format("bar name01=%s lv01=%s lv02=%s next=%s",
            tostring(Core.read_text(bar.Txt_Name01)), tostring(Core.read_text(bar.Txt_Lv01)),
            tostring(Core.read_text(bar.Txt_Lv02)), tostring(Core.read_text(bar.Txt_Next_Num))))
    end
    return bar ~= nil
end

function ItemUse.reset() ann:reset() end

function ItemUse.update()
    local bar = selected_bar()
    if not bar then return end
    local text = bar_text(bar)
    if not text then return end
    -- The "choose character" prompt on entry; the character + level-up preview as it changes.
    ann:focus(I18n.t("itemuse_prompt"), nil, text, nil, nil)
end

return ItemUse
