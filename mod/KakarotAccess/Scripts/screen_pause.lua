-- Screen adapter: in-battle pause menu (Xcmn_Pause_C).
--
-- A short fixed list — Return to Battle / View Battle Conditions / View Controls — held in
-- List_Bar00..04 (each row's label is Txt_List). The SELECTED row is drawn by an animated
-- material (the container's Loop animation) and is NOT distinguishable by any readable
-- widget property: every visible List_Bar reports identical visibility / opacity / cursor
-- state / transform. So this reads the WHOLE menu on entry — robust and colour/animation-
-- independent, which is what we want. (Per-row selection tracking would need the material
-- signal; added later if a reliable discriminator turns up.)

local Core = require("ui_core")
local I18n = require("i18n")

local Pause = {}

local BAR_COUNT = 5   -- List_Bar00..04 (only the first few carry a label)

local ann = Core.make_announcer()
local pause = nil
local tick = 0
local cached = nil   -- menu string computed in is_active, reused by update this tick

-- Ordered on-screen menu labels (collapsed / empty rows skipped).
local function items()
    local list = {}
    for i = 0, BAR_COUNT - 1 do
        local bar = pause["List_Bar0" .. i]
        if Core.on_screen(bar) then
            local label = Core.text_of(bar.Txt_List)
            if label then list[#list + 1] = label end
        end
    end
    return list
end

function Pause.is_active()
    tick = tick + 1
    pause = Core.cached_live("Xcmn_Pause_C", tick)   -- cheap: cached ref, no per-tick scan
    if not Core.on_screen(pause) then cached = nil return false end
    local list = items()
    if #list == 0 then cached = nil return false end
    -- "Paused" (localized) followed by the options in on-screen order.
    cached = Core.phrase(I18n.t("paused"), table.concat(list, ", "))
    return true
end

function Pause.reset() ann:reset() end

function Pause.update()
    -- Whole menu as the item; only re-announced if the option set changes.
    ann:focus(nil, nil, cached, nil, nil)
end

return Pause
