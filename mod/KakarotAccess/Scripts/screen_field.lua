-- Screen adapter: overworld / field Start menu (Start_Top_C -> UAT_UIStartTop).
--
-- This was DEAD END 2 from Lua: the ring items (Start_Top_List00..06) are pure image-
-- font (no text) and the header shows a FIXED "Main Menu" title, so nothing was
-- reflectable. We now read it NATIVELY via mem_bridge:
--   * selected ring index = int32 at UAT_UIStartTop + 0x4e4  (Ghidra: In_Curs reads it).
--   * per-item section id  = byte at (UIStartTopList[idx] + 0x404) = its START_TOP_LIST_ID
--     (Ghidra: FUN_1416bca00 returns it). That id -> localized name via I18n.startlist.
--
-- SUBMENUS: the same Start_Top_C hosts the submenus behind some ring entries (System, Story).
-- Their selected row is a distinct native member at +0x4ec, and which submenu is open is the
-- parent ring section at +0x4e4. The RELIABLE "am I inside a submenu?" signal is the native
-- depth flag at +0x4dc (1 = ring / top level, 0 = inside a submenu) — confirmed by labeled F4
-- snapshots. (The _Sub arrays and on_screen() both LIE on the ring: they retain/mirror stale
-- items, so an earlier attempt to gate on them read the wrong thing and silenced the ring.)
--
-- So moving the ring announces each section, and drilling into System/Story announces the
-- section as the screen name plus each row (e.g. Save / Load / Options / Tutorial / Title).
-- Fully guarded: any failed read -> falls back to the ring (never a regression).

local Core = require("ui_core")
local Mem = require("mem")
local OFF = require("native_offsets")
local I18n = require("i18n")

local Field = {}

-- START_TOP_LIST_ID of a ring section -> its Start_Top sub-list TArray member (reflected).
-- Only sections whose submenu is genuinely a Start_Top sub-list belong here. System (6) is
-- CONFIRMED (depth flag drops to 0, 06_Sub carries the rows). NOTE: Story (4) is NOT here —
-- it opens a SEPARATE screen (UAT_UIStartQuest, a Base06 highlight-only list): inside it the
-- depth flag stays 1 (Start_Top is frozen at ring level), so it needs its own adapter, not
-- this path (04_Sub is unused for our purposes).
local SUBMENU_BY_SID = {
    [6] = "UIStartTopList06_Sub",  -- System: Save/Load/Options/Tutorial/Title
}

-- Submenus with NO reflected sub-list: the Community section has no
-- UIStartTopList00_Sub member (CXX dump: only 04_Sub/06_Sub exist). Its rows are
-- FIXED and follow the START_TOP_LIST_ID enum order (AT_enums.hpp: COMMUNITY_BOARD=7,
-- COMMUNITY_EMBLEM=8 — matches the on-screen order, screenshot 2026-07-06); the
-- native subIndex (+0x4ec) picks the row. The labels are image fonts, so the spoken
-- names come from I18n.startlist like the ring's.
local FIXED_SUB_BY_SID = {
    [0] = { 7, 8 },   -- Community: Community Board / Soul Emblems
}

local ann = Core.make_announcer()
local top, tick = nil, 0
local cached_screen, cached_name = nil, nil
local last_dbg = nil

-- The UAT_UIStartTopList item at 0-based `idx` of a reflected TArray member of `top`, or nil.
local function list_item(arr_name, idx)
    if not idx or idx < 0 then return nil end
    local item
    pcall(function()
        local arr = top[arr_name]          -- UE4SS Lua TArray is 1-based
        if arr then item = arr[idx + 1] end
    end)
    if Core.valid(item) then return item end
    return nil
end

-- A menu item's START_TOP_LIST_ID byte (0x404) -> localized name. Returns name, id.
local function item_name(item)
    local sid = Mem.u8(item, OFF.startTopList.sectionId)
    if not sid then return nil, nil end
    return I18n.startlist(sid), sid
end

-- DEBUG hunt for the community submenu's REAL cursor. OFF since 2026-07-06: FOUND —
-- +0x4E8 toggled 0<->1 exactly with the two rows (startTop.fixedSubIndex). Kept for
-- the next fixed-rows submenu that needs mapping.
local HUNT_SUB = false
local hunt_prev = nil
local function hunt_submenu()
    local addr = Mem.addr(top)
    if not addr then return end
    local cur = Mem.at_bytes(addr, 0x4B0, 0x90)
    if not cur then return end
    local prev = hunt_prev
    hunt_prev = cur
    if not prev or prev == cur then return end
    local parts = {}
    for off = 0, 0x90 - 4, 4 do
        local a = string.unpack("<i4", prev, off + 1)
        local b = string.unpack("<i4", cur, off + 1)
        if a ~= b then
            parts[#parts + 1] = string.format("+0x%X=%d(was %d)", 0x4B0 + off, b, a)
        end
    end
    if #parts > 0 then
        pcall(function() require("dev_log").write("[subhunt] " .. table.concat(parts, " ")) end)
    end
end

-- Resolve what is currently selected. The depth flag decides ring vs submenu.
-- Returns (screen_name, item_name, debug_string).
local function resolve()
    local ring_idx = Mem.i32(top, OFF.startTop.selectedIndex)   -- 0x4e4
    local ring_item = list_item("UIStartTopList", ring_idx)
    local parent_name, parent_sid = nil, nil
    if ring_item then parent_name, parent_sid = item_name(ring_item) end

    -- Inside a submenu (depth flag 0)? Read its sub-list row instead of the ring section.
    local depth = Mem.i32(top, OFF.startTop.depthFlag)          -- 0x4dc: 1 ring, 0 submenu
    if depth == 0 and parent_sid then
        local arr = SUBMENU_BY_SID[parent_sid]
        if arr then
            local sub_idx = Mem.i32(top, OFF.startTop.subIndex)  -- 0x4ec
            local item = list_item(arr, sub_idx)
            if item then
                local name, sid = item_name(item)
                if name then
                    return parent_name, name,           -- screen = the section; name = the row
                        string.format("SUB %s sub=%s sid=%s", arr, tostring(sub_idx), tostring(sid))
                end
            end
        end
        local fixed = FIXED_SUB_BY_SID[parent_sid]
        if fixed then
            if HUNT_SUB then hunt_submenu() end
            -- These submenus use their OWN cursor field (+0x4e8) — subIndex stays 0
            -- here (subhunt 2026-07-06).
            local sub_idx = Mem.i32(top, OFF.startTop.fixedSubIndex)
            local sid = sub_idx and fixed[sub_idx + 1]
            local name = sid and I18n.startlist(sid)
            if name then
                return parent_name, name,
                    string.format("FIXEDSUB sub=%s sid=%s", tostring(sub_idx), tostring(sid))
            end
        end
    end

    -- Ring level: screen is "Main menu"; the item is the hovered section.
    -- dbg carries depth + raw subIndex: the community submenu stayed silent even with
    -- the FIXEDSUB mapping (2026-07-06) — this shows whether its depth flag ever
    -- drops to 0 (System-like) or stays 1 (Story-like, needs another signal).
    return I18n.t("main_menu"), parent_name,
        string.format("ring=%s sid=%s depth=%s sub=%s", tostring(ring_idx),
            tostring(parent_sid), tostring(depth),
            tostring(Mem.i32(top, OFF.startTop.subIndex)))
end

-- The OPEN ring instance. The game pools SEVERAL Start_Top_C and can open the ring on
-- a NEW one (e.g. right after a shop — Shop_Top_C_1 style; the ring went mute after
-- leaving the food shop, user 2026-07-06), while a stale instance can linger
-- HitTestInvisible. cached_live locked onto one instance forever, and first_on_screen
-- could still return the stale enum-3 one — so pick the instance that is genuinely
-- open (its own visibility is Visible(0)) among all on-screen ones.
local function live_ring()
    -- Shared with the cooking/shoplist yield gates (Core.ring_open): rendered AND
    -- ESlateVisibility Visible — the pooled ring lingers on_screen while closed.
    return Core.ring_open(tick)
end

function Field.is_active()
    tick = tick + 1
    top = live_ring()
    if not top then cached_screen, cached_name = nil, nil return false end

    local screen, name, dbg = resolve()

    -- Diagnostic (one line per change) so the ring/submenu index->name mapping is
    -- verifiable. DISABLED by default: the file append runs on the GAME THREAD per
    -- cursor move, and wrapping the ring quickly caused visible lag spikes
    -- (2026-07-03). The mapping is long since verified; re-enable only to re-map.
    local DEBUG_LOG = false  -- community submenu mapped 2026-07-06 (fixedSubIndex)
    if DEBUG_LOG and dbg ~= last_dbg then
        last_dbg = dbg
        pcall(function()
            require("dev_log").write("[field] " .. dbg .. " name=" .. tostring(name))
        end)
    end

    if not name then cached_screen, cached_name = nil, nil return false end

    -- Context change (ring <-> submenu) -> re-announce the screen name + item (menus.md).
    if screen ~= cached_screen then ann:reset() end
    cached_screen, cached_name = screen, name
    return true
end

function Field.reset() ann:reset() cached_screen, cached_name, last_dbg = nil, nil, nil end

function Field.update()
    -- screen (Main menu / the section) spoken on entry; name = the selected row/section on move.
    ann:focus(cached_screen, nil, cached_name, nil, nil)
end

return Field
