-- Screen adapter: main title menu (Gametitle_C).
--
-- Thin per-screen layer of the hybrid architecture: it knows which widget is the
-- title menu and how to find the focused option, then hands (name) to the shared
-- Announcer. No spoken-text logic lives here.
--
-- Model: Gametitle_C_0 .Gametitle_List00..05 ; the selected option is the one
-- whose .Txt_Select is visible; its label is in .Txt_List.

local Core = require("ui_core")
local I18n = require("i18n")

local Title = {}

-- The title is the fallback screen and stays on-screen behind boot dialogs, the intro
-- cinematic, and the New Game → load transition, so require a longer stable window before
-- announcing it — otherwise it blurts "Main menu, Continue" in those gaps. (Gaps longer
-- than this still leak; a movie/fade-aware suppression is the proper fix, pending a dump.)
Title.confirm_ticks = 15

local OPTION_COUNT = 6

local ann = Core.make_announcer()
local gt = nil
local tick = 0

function Title.is_active()
    tick = tick + 1
    gt = Core.cached_live("Gametitle_C", tick)   -- cheap: cached ref, no per-tick scan
    if not Core.on_screen(gt) then return false end
    -- Only when the title is truly INTERACTIVE (ESlateVisibility.Visible == 0). During the
    -- intro cinematic the title is on-screen but HitTestInvisible (enum 3) — rendered, not
    -- usable — so we must not blurt "Main menu" over the movie.
    local ok, v = pcall(function() return gt:GetVisibility() end)
    return ok and tonumber(v) == 0
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
    -- The title screen has no on-screen name → a mod-added, localized label.
    ann:focus(I18n.t("main_menu"), nil, name, nil, nil)
end

return Title
