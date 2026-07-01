-- Screen adapter: main title menu (Gametitle_C).
--
-- Thin per-screen layer of the hybrid architecture: it knows which widget is the
-- title menu and how to find the focused option, then hands (name) to the shared
-- Announcer. No spoken-text logic lives here.
--
-- Model: Gametitle_C_0 .Gametitle_List00..05 ; the selected option is the one
-- whose .Txt_Select is visible; its label is in .Txt_List.

local Core = require("ui_core")

local Title = {}

local OPTION_COUNT = 6
local REACQUIRE_EVERY = 10

-- The title screen has no on-screen name, so this is a mod-added label. Kept here
-- as the one hardcoded string; move to a shared strings layer if more accumulate.
local SCREEN_NAME = "Menú principal"

local ann = Core.make_announcer()
local gt = nil
local tick = 0

local function acquire() gt = Core.first_live("Gametitle_C") end

function Title.is_active()
    tick = tick + 1
    if not Core.valid(gt) and tick % REACQUIRE_EVERY == 0 then acquire() end
    return Core.valid(gt) and Core.is_visible(gt)
end

function Title.reset() ann:reset() end

function Title.update()
    local name
    for i = 0, OPTION_COUNT - 1 do
        local item = gt["Gametitle_List0" .. i]
        if Core.valid(item) and Core.is_visible(item.Txt_Select) then
            name = Core.text_of(item.Txt_List) or Core.text_of(item.Txt_Select)
            break
        end
    end
    ann:focus(SCREEN_NAME, nil, name, nil, nil)
end

return Title
