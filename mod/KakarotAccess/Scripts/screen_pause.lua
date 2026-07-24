-- Screen adapter: in-battle pause menu (Xcmn_Pause_C).
--
-- A short fixed list — Return to Battle / View Battle Conditions / View Controls — held in
-- List_Bar00..04 (each row's label is Txt_List) / the reflected ListBarArray.
--
-- PER-ITEM SELECTION (2026-07-02): the selected row IS now readable. The index is a
-- non-UPROPERTY C++ member (invisible to reflection; every widget highlight signal reads
-- identical across rows), so we read it NATIVELY via mem_bridge at UAT_UIXCmnPause + 0x43C
-- (an int32 that steps 0->1->2 and wraps — pinned by Ghidra + the F4 runtime-diff tool; see
-- native_offsets.lua / code/hybrid-mod-feasibility.md). The index refers to ListBarArray, so
-- we map index->row through that same array. If the native read is unavailable we fall back
-- to reading the WHOLE menu on entry (never worse than before).
--
-- The pause widget stays RESIDENT as HitTestInvisible (enum 3) the whole battle, so we gate
-- on it being genuinely OPEN (ESlateVisibility.Visible == 0); without that it would be
-- "active" all battle and (registered above the tutorial guide) shadow its reads.

local Core = require("ui_core")
local I18n = require("i18n")
local Mem = require("mem")
local OFF = require("native_offsets")

local Pause = {}

local BAR_COUNT = 5   -- List_Bar00..04 (only the first few carry a label)

local ann = Core.make_announcer()
local pause = nil
local tick = 0
local cached = nil   -- item string computed in is_active, reused by update this tick

-- Ordered on-screen menu labels (collapsed / empty rows skipped) — fallback path.
local function items()
    local list = {}
    for i = 0, BAR_COUNT - 1 do
        local bar = Core.member(pause, "List_Bar0" .. i)
        if Core.on_screen(bar) then
            local label = Core.text_of(Core.member(bar, "Txt_List"))
            if label then list[#list + 1] = label end
        end
    end
    return list
end

-- The selected row's label via the native selection index, or nil if unavailable.
local function selected_label()
    local idx = Mem.i32(pause, OFF.pause.selectedIndex)
    if not idx or idx < 0 or idx >= BAR_COUNT then return nil end
    -- Prefer the reflected ListBarArray (the array the native index refers to);
    -- fall back to the positional List_Bar0N row.
    local row
    local ok = pcall(function()
        local arr = pause.ListBarArray
        if arr then row = arr[idx + 1] end   -- UE4SS Lua arrays are 1-based
    end)
    if not (ok and Core.valid(row)) then row = Core.member(pause, "List_Bar0" .. idx) end
    if not Core.on_screen(row) then return nil end
    return Core.text_of(Core.member(row, "Txt_List"))
end

function Pause.is_active()
    tick = tick + 1
    pause = Core.cached_live("Xcmn_Pause_C", tick)   -- cheap: cached ref, no per-tick scan
    if not Core.on_screen(pause) then cached = nil return false end
    -- Only when genuinely OPEN (interactive, enum 0). During battle the pause widget stays
    -- resident as HitTestInvisible (enum 3); reading it then would shadow the tutorial/HUD.
    local ok, v = pcall(function() return pause:GetVisibility() end)
    if not (ok and tonumber(v) == 0) then cached = nil return false end

    local sel = selected_label()          -- native per-item selection
    if sel then cached = sel return true end

    -- Fallback: no native read -> announce the whole menu on entry.
    local list = items()
    if #list == 0 then cached = nil return false end
    cached = table.concat(list, ", ")
    return true
end

function Pause.reset() ann:reset() end

function Pause.update()
    -- screen = "Paused" (spoken on entry only); name = the selected row (re-spoken on move).
    ann:focus(I18n.t("paused"), nil, cached, nil, nil)
end

return Pause
