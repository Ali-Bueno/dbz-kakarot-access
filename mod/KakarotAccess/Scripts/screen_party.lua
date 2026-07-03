-- Screen adapter: overworld Party menu (Start_Party_C -> UAT_UIStartParty).
--
-- The LEFT column (the slots the cursor navigates: Player / Support #1 / Support #2) lives in
-- WL_Start_Party_List (Start_Party_List_C) -> WL_Start_Party_Bar (0x398, a FIXED array of 3
-- UAT_UIStartPartyBar*). UE4SS reflection COLLAPSES a fixed array to element 0, so only slot 0
-- is reachable normally; slots 1/2 are recovered with UE4SS's RegisterCustomProperty (each raw
-- pointer at 0x398 + i*8 registered as an ObjectProperty). partySelectData.cursorIndex (0x420 on
-- the host) selects. Each bar exposes Txt_Player/Txt_Suport (role), Txt_Name (character) or
-- Txt_None ("Not Set"/"Cannot Change"), and Txt_Lv. Fully guarded throughout.
--
-- (The RIGHT "Standby Characters" panel is a separate array, WL_Start_Char_Bar — not read here.)

local Core = require("ui_core")
local Mem = require("mem")
local OFF = require("native_offsets")
local I18n = require("i18n")

local Party = {}

local ann = Core.make_announcer()
local host, list, cur_idx, tick = nil, nil, nil, 0

-- Slot index -> property yielding its UAT_UIStartPartyBar (0 reflected, 1/2 custom-registered).
local SLOT_PROP  = { [0] = "WL_Start_Party_Bar", [1] = "PartyListBar1", [2] = "PartyListBar2" }
local SLOT_OFF   = { [1] = 0x3A0, [2] = 0x3A8 }
local LIST_CLASS = "/Game/Art/UI/Start_Party/Start_Party_List.Start_Party_List_C"

local registered = false
local function ensure_registered()
    if registered then return end
    registered = true
    for i, off in pairs(SLOT_OFF) do
        pcall(function()
            RegisterCustomProperty({
                ["Name"] = SLOT_PROP[i],
                ["Type"] = PropertyTypes.ObjectProperty,
                ["BelongsToClass"] = LIST_CLASS,
                ["OffsetInternal"] = off,
            })
        end)
    end
end

local function cursor_index()
    if not Core.valid(host) then return nil end
    local idx
    pcall(function() idx = host.partySelectData.cursorIndex end)
    if type(idx) ~= "number" then idx = Mem.i32(host, OFF.startParty.cursorIndex) end
    if type(idx) == "number" and idx >= 0 then return idx end
    return nil
end

-- The UAT_UIStartPartyBar for a slot index via its (reflected or custom) property.
local function slot_bar(idx)
    if not Core.valid(list) then return nil end
    local prop = SLOT_PROP[idx]
    if not prop then return nil end
    local bar
    pcall(function() bar = list[prop] end)
    if Core.valid(bar) then return bar end
    return nil
end

local function selected_row()
    local bar = slot_bar(cur_idx)
    if not bar then return nil end
    local role = Core.read_text(bar.Txt_Player) or Core.read_text(bar.Txt_Suport)
    local name = Core.read_text(bar.Txt_Name) or Core.read_text(bar.Txt_None)
    if not role and not name then return nil end
    return { role = role, name = name, lv = Core.read_text(bar.Txt_Lv) }
end

function Party.is_active()
    tick = tick + 1
    host = Core.first_on_screen("AT_UIStartParty", tick)
    if not host then return false end
    ensure_registered()
    pcall(function() list = host.WL_Start_Party_List end)
    cur_idx = cursor_index()
    return Core.valid(list) and cur_idx ~= nil
end

function Party.reset() ann:reset() end

function Party.update()
    local row = selected_row()
    if not row then return end
    -- "Party" on entry; then role (Player / Support), character (or "Not Set"), level as it moves.
    ann:focus(I18n.startlist(3), nil, Core.phrase(row.role, row.name, row.lv), nil, nil)
end

return Party
