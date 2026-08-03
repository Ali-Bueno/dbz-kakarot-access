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

-- Settle gate (user report 2026-07-17: "Main menu, Continue" repeated through the whole
-- boot-check dialog sequence). The boot dialogs outrank the title, so each one flips the
-- dispatcher dialog↔title and every flip back RE-COMMITS the title — reset() wipes the
-- announcer and the same menu re-announces in every gap longer than confirm_ticks. The
-- title actor exposes NO reflected boot-phase field (AAT_Title / AATTitleLevelScriptActor
-- swept 2026-07-17), so the gate is temporal: after a dispatcher reset the title stays
-- SILENT until it has held the screen for SETTLE_S with no further reset — i.e. the boot
-- dialogs are really over — EXCEPT that a cursor move lifts the gate at once (navigating
-- IS the proof the menu is live, and it must never feel muted). F1 re-reads bypass the
-- gate via reannounce(). SETTLE_S: the dialog gaps outlived confirm_ticks (~1.5 s), so
-- past that with some margin; pure announce-delay on entering the title, nothing else.
-- Tuned in-game: 4 s worked but felt sluggish, 2.5 s is the user's pick (2026-07-17).
local SETTLE_S = 2.5

local OPTION_COUNT = 6

local ann = Core.make_announcer()
local gt = nil
local tick = 0
local reset_t = 0        -- os.clock() of the last dispatcher reset (commit churn marker)
local settled = false
local gate_name = nil    -- focused option first seen while settling; a different one lifts

function Title.is_active()
    tick = tick + 1
    -- Cheap precondition, FIRST (2026-08-03). The title screen cannot exist in a playable
    -- world, but `Gametitle_C` is directory-mapped under the TITLE root — and a mapped class
    -- whose root is unreachable correctly falls back to scanning, so during gameplay this
    -- adapter paid a full FindAllOf every backoff for the whole session: the measured dump
    -- caught 137 scans / 9.6 s, every one of them empty and none of them possible.
    if Core.gameplay_world() then return false end
    gt = Core.cached_live("Gametitle_C", tick)   -- cheap: cached ref, no per-tick scan
    if not Core.on_screen(gt) then return false end
    -- Only when the title is truly INTERACTIVE (ESlateVisibility.Visible == 0). During the
    -- intro cinematic the title is on-screen but HitTestInvisible (enum 3) — rendered, not
    -- usable — so we must not blurt "Main menu" over the movie. Strict, for that reason:
    -- an unreadable enum must not claim. The substrate adds the fade check for free, which
    -- also keeps us quiet while the title fades in.
    return Core.pane_live(gt, true)
end

function Title.reset()
    ann:reset()
    reset_t, settled, gate_name = os.clock(), false, nil
end

-- F1 re-read: repeat the current option WITHOUT re-arming the settle gate (a plain
-- reset() here would mute the title for SETTLE_S right when the user asked to hear it).
function Title.reannounce() ann:reset() end

function Title.update()
    local name
    for i = 0, OPTION_COUNT - 1 do
        local item = Core.member(gt, "Gametitle_List0" .. i)
        if Core.valid(item) and Core.is_visible(Core.member(item, "Txt_Select")) then
            name = Core.text_of(Core.member(item, "Txt_List")) or Core.text_of(Core.member(item, "Txt_Select"))
            break
        end
    end
    if not settled then
        if os.clock() - reset_t >= SETTLE_S then
            settled = true
        else
            if gate_name == nil then gate_name = name or false end
            if (name or false) == gate_name then return end
            settled = true   -- the cursor moved: the menu is live, speak at once
        end
    end
    -- The title screen has no on-screen name → a mod-added, localized label.
    ann:focus(I18n.t("main_menu"), nil, name, nil, nil)
end

return Title
