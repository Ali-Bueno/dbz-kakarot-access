-- Widget-archetype readers (the reusable "HOW each control sounds" layer of the
-- hybrid architecture — see reference/ui-accessibility/generic-strategy.md).
--
-- Keyed on THIS game's shared widget framework (Xcmn_*/Xlist_*), so any screen
-- built from the same widgets reuses the same reader. Screen adapters classify a
-- focused control and call in here; no per-screen branching lives in a reader.

local Core = require("ui_core")

local A = {}

local GAUGE_SEGMENTS = 20   -- volume gauge = 20 on/off segments

-- ---- Xlist_Bar03_C list row (options-style list) ---------------------------

function A.row_index(r)
    local s = r:GetFullName():match("Xlist_Bar03_(%d+)")
    return s and tonumber(s) or -1
end

-- The volume gauge value as a percentage, or nil if the row isn't a slider.
local function gauge_value(row)
    local g = row.Xlist_Bar_03_Gauge
    if not Core.is_visible(g) then return nil end
    local on = 0
    for i = 0, GAUGE_SEGMENTS - 1 do
        if Core.is_visible(g["Ins_System_On" .. string.format("%02d", i)]) then on = on + 1 end
    end
    return math.floor(on / GAUGE_SEGMENTS * 100 + 0.5) .. "%"
end

-- The row's current value. Only ADJUSTABLE rows (a slider gauge, or the left/right
-- value arrows) carry a real inline value; action rows (button remapping, submenu
-- links) have no value and leave STALE text in Txt_Mode from a previously-viewed
-- tab (rows are recycled across tabs), so reading it would bleed e.g. a graphics
-- value onto a button action. Gate on adjustability, not just node visibility.
function A.row_value(row)
    local g = gauge_value(row)
    if g then return g end
    local adjustable = Core.is_visible(row.Xmenu_Arrow01_L)
        or Core.is_visible(row.Xmenu_Arrow01_R)
    if not adjustable then return nil end
    if Core.is_visible(row.Txt_Mode) then
        local v = Core.text_of(row.Txt_Mode)
        if v then return v end
    end
    if Core.is_visible(row.Txt_Mode_Scroll) then
        local v = Core.text_of(row.Txt_Mode_Scroll)
        if v then return v end
    end
    return nil
end

function A.row_name(row)
    return Core.text_of(row.Txt_List)
end

-- Scan a pool of Xlist_Bar03_C rows: lowest-index fad row (the real selection),
-- the highest index present, a by-index map, and any valid row.
function A.scan_list(rows)
    local low, maxIdx, any, byIdx = nil, -1, nil, {}
    for _, r in ipairs(rows) do
        if Core.valid(r) then
            any = any or r
            local idx = A.row_index(r)
            byIdx[idx] = r
            if idx > maxIdx then maxIdx = idx end
            if Core.is_visible(r.Ins_Cursor_Fad) then
                if not low or idx < low.idx then low = { idx = idx, row = r } end
            end
        end
    end
    return low, maxIdx, any, byIdx
end

-- A confirm button (e.g. "Guardar cambios") is a phantom overflow row at the last
-- slot that DUPLICATES the last real option's name once you scroll past it. A real
-- option merely rendered in the last slot won't duplicate its neighbour.
function A.is_overflow_button(low, maxIdx, byIdx)
    if not low or low.idx ~= maxIdx then return false end
    local above = byIdx[maxIdx - 1]
    if not above then return false end
    return A.row_name(above) == A.row_name(low.row)
end

return A
