-- Screen adapter: overworld / field Start menu (Start_Top_C -> UAT_UIStartTop).
--
-- This was DEAD END 2 from Lua: the ring items (Start_Top_List00..06) are pure image-
-- font (no text) and the header shows a FIXED "Main Menu" title, so nothing was
-- reflectable. We now read it NATIVELY via mem_bridge:
--   * selected ring index = int32 at UAT_UIStartTop + 0x4e4  (Ghidra: In_Curs reads it).
--   * per-item section id  = byte at (UIStartTopList[idx] + 0x404) = its EXCmnHeaderFontType
--     (Ghidra: FUN_1416bca00 returns exactly this). That id -> localized name via I18n.header.
--
-- So moving the ring announces each item's section (Map / Character / Items / ...), in the
-- game's language. Fully guarded: any failed read -> the reader stays silent (no regression).

local Core = require("ui_core")
local Mem = require("mem")
local OFF = require("native_offsets")
local I18n = require("i18n")

local Field = {}

local ann = Core.make_announcer()
local top, tick = nil, 0
local cached = nil
local last_dbg = nil

-- The selected ring item's section name, or nil. Also returns (idx, sid) for logging.
local function selected_section()
    local idx = Mem.i32(top, OFF.startTop.selectedIndex)
    if not idx or idx < 0 then return nil, idx, nil end
    local item
    pcall(function()
        local arr = top.UIStartTopList         -- reflected TArray; UE4SS Lua is 1-based
        if arr then item = arr[idx + 1] end
    end)
    if not Core.valid(item) then return nil, idx, nil end
    local sid = Mem.u8(item, OFF.startTopList.sectionId)
    if not sid then return nil, idx, nil end
    return I18n.startlist(sid), idx, sid
end

function Field.is_active()
    tick = tick + 1
    top = Core.cached_live("Start_Top_C", tick)
    if not Core.on_screen(top) then cached = nil return false end
    local ok, v = pcall(function() return top:GetVisibility() end)
    if not (ok and tonumber(v) == 0) then cached = nil return false end

    local name, idx, sid = selected_section()

    -- Diagnostic (one line per change) so the idx/section id mapping is verifiable.
    local dbg = tostring(idx) .. "/" .. tostring(sid)
    if dbg ~= last_dbg then
        last_dbg = dbg
        pcall(function()
            require("dev_log").write(string.format("[field] idx=%s sid=%s name=%s",
                tostring(idx), tostring(sid), tostring(name)))
        end)
    end

    if not name then cached = nil return false end
    cached = name
    return true
end

function Field.reset() ann:reset() last_dbg = nil end

function Field.update()
    -- screen = "Main menu" (spoken on entry); name = the selected section (spoken on move).
    ann:focus(I18n.t("main_menu"), nil, cached, nil, nil)
end

return Field
