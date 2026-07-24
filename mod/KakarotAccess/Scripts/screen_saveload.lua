-- Screen adapter: Save / Load data-slot menu (AT_UIStartSaveLoad).
--
-- ONE adapter covers BOTH screens: Save and Load are the same native widget
-- (UAT_UIStartSaveLoad) — only the header differs, and the shared SetFontType
-- header hook already announces "Guardar"/"Cargar" on entry, so this reader only
-- speaks the focused slot. No blueprint `_C` exists for it (object dump: the class
-- is instantiated natively, like AT_UIStartParty), so FindAllOf uses the native name.
--
-- VIRTUALIZED LIST: UISaveLoadBar_List is NOT the logical save list — it is a FIXED
-- window of 3 visual bars that the game scrolls data through. So a bar's pool-position
-- is NOT the save's ordinal (it used to stick at "3" for slots 3, 4, 5…). The real
-- selection lives in the class's non-reflected tail, recovered via mem_bridge (runtime
-- diff, 2026-07-11 — see native_offsets.saveLoad):
--   +0x410 = ABSOLUTE selected index (0-based)   -> ordinal shown = +1
--   +0x418 = highlighted bar WITHIN the 3-bar window (0..2) -> which widget to read
--   +0x414 = scroll offset (unused here; index = scroll + windowPos)
-- Reading the index from memory makes the ordinal correct whether or not the list has
-- scrolled, and picks the exact cursor bar without relying on the flickering highlight.
--
-- Filled vs empty is gated on the bar's PANELS, not caption text: a filled slot renders
-- Canvas_Detail/Canvas_Title; an empty one renders Canvas_None with the "no data" caption.
-- The caption nodes are POOLED and keep stale text on filled bars, so reading them
-- unconditionally made filled slots read as "empty" — hence the panel gate.
--
-- SETTLE DEBOUNCE: while the list scrolls, a reused bar repopulates over a few frames
-- (title first, then name/level/…; and it flashes Canvas_None mid-animation). We only
-- announce a (ordinal, text) that has held steady for SETTLE_TICKS polls, so those
-- transient scroll frames — which never repeat — are silently skipped.

local Core = require("ui_core")
local I18n = require("i18n")
local Mem = require("mem")
local OFF = require("native_offsets").saveLoad

local SaveLoad = {}

-- This screen is DESTROYED and recreated on a quick close+reopen, which used to leave the
-- cached class list holding only the dead instance. Core.first_on_screen now notices that (a
-- list with nothing valid left in it) and asks for a re-scan on the next tick, so the recreated
-- instance is picked up in a tick or two with no per-adapter opt-in. (The event feed that once
-- did this was removed on 2026-07-14 — it ran Lua on a foreign thread and crashed the game.)

local ann = Core.make_announcer()
local host, tick = nil, 0

-- Deliberately opened sub-screen: no boot-flash risk, so one confirmation tick in the
-- registry is enough (the global CONFIRM_TICKS=2 exists for screens that FLASH at boot).
-- Shaves ~100 ms off every entry (entry-latency brief 2026-07-13).
SaveLoad.confirm_ticks = 1

-- Polls a (ordinal, text) must hold steady before it is spoken — filters the transient
-- title-only / flashing-empty frames the scroll animation produces. 1 poll: bar_text
-- already nils half-built rows, so the longer hold was redundant on entry and cost
-- ~100 ms every time the screen opened (entry-latency brief 2026-07-13).
local SETTLE_TICKS = 1
local pend_key, pend_n = nil, 0

-- Diagnostic: logs, on change, the native index/window state + focused text to
-- dumps/dump_saveload.txt. NOTE: while true, the host-nil branch of is_active runs a full
-- FindAllOf EVERY tick the screen is closed — a per-frame UObject scan that lags the game.
-- Keep OFF; only flip on for a short capture. Native index + churn re-detection confirmed
-- in game 2026-07-11.
local DEBUG = false
local last_dbg = nil
local function dbg(s)
    if not DEBUG or s == last_dbg then return end
    last_dbg = s
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_saveload.txt", "a")
        if f then f:write(string.format("[%d] %s\n", os.time(), s)) f:close() end
    end)
end

-- A bar is filled when its detail/title canvas is rendered (see panel-gate note above).
local function bar_filled(bar)
    return Core.is_visible(Core.member(bar, "Canvas_Detail")) or Core.is_visible(Core.member(bar, "Canvas_Title"))
end

-- The slot's spoken content. Empty and filled are BOTH read (an empty slot is real
-- info — never hide it). Order matters: check the EMPTY panel (Canvas_None) FIRST,
-- because Canvas_Title is pooled and can stay rendered on an empty bar, which would
-- otherwise make the "wait for name" gate below skip it. Empty slot → its "no data"
-- caption; filled slot → title + character/level + area + play time + timestamp
-- (on-screen order). A filled bar still missing its Name (mid-scroll repopulation)
-- returns nil so the caller waits for the full row instead of speaking a half-built one.
local function bar_text(bar)
    if not Core.valid(bar) then return nil end
    if Core.is_visible(Core.member(bar, "Canvas_None")) then
        return Core.read_text(Core.member(bar, "Txt_Nodata")) or Core.read_text(Core.member(bar, "TextBox_CaptionNone"))
            or I18n.t("saveload_empty")
    end
    if bar_filled(bar) then
        local title = Core.read_text(Core.member(bar, "TextBox_Title"))
        local name = Core.read_text(Core.member(bar, "TextBox_Name"))
        if not title or not name then return nil end
        return Core.phrase(title, name,
            Core.read_text(Core.member(bar, "TextBox_Level")),
            Core.read_text(Core.member(bar, "TextBox_Area")),
            Core.read_text(Core.member(bar, "TextBox_PlayTime")),
            Core.read_text(Core.member(bar, "TextBox_TimeStamp")))
    end
    return nil
end

-- The visible bar-window list, or nil. Core.array_of is mandatory here: a raw `#list`
-- on a reflected TArray is the uncatchable C++ throw when the host is dying (pcall
-- cannot catch it — the validity checks BEFORE the length read are the only defence).
local function bar_list()
    local list, n = Core.array_of(host, "UISaveLoadBar_List")
    if not list or n < 1 then return nil end
    return list, n
end

-- Returns (cursorBar, ordinal). Primary path reads the native selection: ordinal =
-- selectedIndex+1, and the cursor bar is the window slot at windowPos. Fallback (memory
-- reader unavailable): the single bar whose highlight border is rendered, with its raw
-- window position as the ordinal (imperfect once scrolled, but never worse than before).
local function scan()
    local list, n = bar_list()
    if not list then return nil end

    local idx = Mem.is_loaded() and Mem.i32(host, OFF.selectedIndex) or nil
    local wpos = Mem.is_loaded() and Mem.i32(host, OFF.windowPos) or nil
    if idx and wpos and wpos >= 0 and wpos < n then
        local bar = list[wpos + 1]
        if Core.valid(bar) then
            if DEBUG then dbg(string.format("NATIVE idx=%d wpos=%d n=%d", idx, wpos, n)) end
            return bar, idx + 1
        end
    end

    -- Fallback: rendered highlight border.
    local cbar, cpos, ccount = nil, nil, 0
    for i = 1, n do
        local bar = list[i]
        if Core.valid(bar) and Core.is_visible(bar.Border_Cursor) then
            cbar, cpos, ccount = bar, i, ccount + 1
        end
    end
    if ccount ~= 1 then return nil end
    if DEBUG then dbg(string.format("FALLBACK cpos=%d n=%d memLoaded=%s idx=%s",
        cpos, n, tostring(Mem.is_loaded()), tostring(idx))) end
    return cbar, cpos
end

function SaveLoad.is_active()
    tick = tick + 1
    -- Two names for the same screen: in game the directory serves the native-class
    -- instance (pause → System); the TITLE menu's copy is the BLUEPRINT subclass
    -- Start_Save_Load_C (census 2026-07-15) which only the _C-name scan can find
    -- (FindAllOf on the native name returns nothing for it — the 2026-07-06 gotcha).
    host = Core.first_on_screen("AT_UIStartSaveLoad", tick)
        or Core.first_on_screen("Start_Save_Load_C", tick)
    if DEBUG and host == nil then
        -- Distinguish the two entry-blindness causes: a FRESH FindAllOf (bypassing the
        -- cached-list) vs how many pass on_screen. raw>0 while host nil => the cached class
        -- list is stale-empty (cached_all refresh delay). raw>0 & onscreen=0 => the widget
        -- exists but on_screen rejects it (open-anim collapsed ancestor / not-in-viewport).
        local raw = FindAllOf("AT_UIStartSaveLoad") or {}
        local nvalid, nonscr = 0, 0
        for _, o in ipairs(raw) do
            if o and o:IsValid() then
                nvalid = nvalid + 1
                if Core.on_screen(o) then nonscr = nonscr + 1 end
            end
        end
        dbg(string.format("host nil: raw=%d valid=%d onscreen=%d", #raw, nvalid, nonscr))
    end
    return host ~= nil
end

function SaveLoad.reset()
    ann:reset()
    pend_key, pend_n = nil, 0
end

function SaveLoad.update()
    local bar, ordinal = scan()
    if not bar then pend_key, pend_n = nil, 0 return end
    local text = bar_text(bar)
    if not text then pend_key, pend_n = nil, 0 return end
    -- Debounce: only speak once the same (ordinal, text) has held for SETTLE_TICKS polls,
    -- so mid-scroll transient frames (title-only, flashing-empty) are never announced.
    local key = ordinal .. "|" .. text
    if key == pend_key then pend_n = pend_n + 1 else pend_key, pend_n = key, 1 end
    if pend_n < SETTLE_TICKS then return end
    -- Prefix the slot ordinal so moving between identical-looking slots always
    -- re-announces (the announcer diffs on the name string). No screen prefix: the
    -- header hook already says Save vs Load on entry.
    ann:focus(nil, nil, string.format("%d, %s", ordinal, text), nil, nil)
end

return SaveLoad
