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
    end

    -- Ring level: screen is "Main menu"; the item is the hovered section.
    return I18n.t("main_menu"), parent_name,
        string.format("ring=%s sid=%s", tostring(ring_idx), tostring(parent_sid))
end

function Field.is_active()
    tick = tick + 1
    top = Core.cached_live("Start_Top_C", tick)
    if not Core.on_screen(top) then cached_screen, cached_name = nil, nil return false end
    local ok, v = pcall(function() return top:GetVisibility() end)
    if not (ok and tonumber(v) == 0) then cached_screen, cached_name = nil, nil return false end

    local screen, name, dbg = resolve()

    -- Diagnostic (one line per change) so the ring/submenu index->name mapping is verifiable.
    if dbg ~= last_dbg then
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
