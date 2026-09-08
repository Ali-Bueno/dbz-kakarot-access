-- Screen adapter: Downloadable Content (Start_DLC_C -> UAT_UIStartDlc).
--
-- The live 2026-09-04 header/object capture established two important facts:
--   * MenuManager.m_xDlcMenu.m_UIStartDlc owns the live Start_DLC_C instance.
--   * That host exposes the SELECTED detail pane directly (title, story, details,
--     state). The three list bars and DlcItem_List are pooled/hidden data and are
--     therefore neither necessary nor appropriate selection sources.
--
-- Read only the on-screen selected pane. In particular, do not infer selection
-- from PnlCurs: other menus in this game keep every pooled cursor visual visible.

local Core = require("ui_core")
local I18n = require("i18n")

local Dlc = {}
local SETTLE_POLLS = 3

-- A deliberately opened full-screen submenu with a manager-owned instance does
-- not need the global two-tick boot-flash confirmation delay.
Dlc.confirm_ticks = 1

local ann = Core.make_announcer()
local host, tick = nil, 0
local heading, title, story, details, state = nil, nil, nil, nil, nil
local seen_title, seen_count = nil, 0

local function clear_snapshot()
    host = nil
    heading, title, story, details, state = nil, nil, nil, nil, nil
end

local function clear_observation()
    clear_snapshot()
    seen_title, seen_count = nil, 0
end

local function tooltip()
    local text = Core.phrase(story, details)
    return text ~= "" and text or nil
end

function Dlc.is_active()
    tick = tick + 1
    -- The DLC widget is manager-owned and may remain resident after closing. The
    -- minimap is the game's own "back in free roam" signal and prevents a parked
    -- pane from shadowing every lower-priority adapter.
    if Core.free_roam(tick) then
        clear_observation()
        return false
    end

    host = Core.first_on_screen("Start_DLC_C", tick)
    -- This is a pooled interactive pane, so require genuine Visible(0) ownership as
    -- well as opacity. A parked instance can retain its text while the ring is open;
    -- pane_live prevents that stale instance from shadowing every adapter below it.
    if host and not Core.pane_live(host) then host = nil end
    if not host then
        clear_observation()
        return false
    end

    -- Blueprint names first: these exact nodes exist on the captured live widget.
    -- Native aliases are inherited pointers to the same visible pane and provide a
    -- guarded fallback if a packaged blueprint changes which binding is populated.
    heading = Core.first_text(host, "Txt_Cap00", "TxtCap00") or I18n.startlist(14)
    title = Core.first_text(host, "Txt_Title", "TxtTitle")
    story = Core.first_text(host, "Txt_Story", "TxtSummary")
    details = Core.first_text(host, "Txt_Ditaile", "TxtDetails")
    state = Core.first_text(host, "Txt_State")

    -- This virtualized menu repaints the selected detail pane behind the cursor.
    -- Wait for the title to remain unchanged for three 100-ms registry polls before
    -- speaking, giving story/details time to catch up to the new title. Still claim
    -- the foreground during settling so the open-world reader cannot speak through.
    if title ~= seen_title then
        seen_title, seen_count = title, title and 1 or 0
    elseif title and seen_count < SETTLE_POLLS then
        seen_count = seen_count + 1
    end

    -- Claim the foreground as soon as the real screen is present. Its text can be
    -- populated a tick later; yielding here would let the underlying field reader
    -- speak through the DLC menu. update() remains silent until a visible title exists.
    return true
end

function Dlc.update()
    if not title or seen_count < SETTLE_POLLS then return end
    ann:focus(heading, nil, title, state, tooltip)
end

-- F1 repeats the already-settled current selection on the next registry tick.
-- Unlike reset(), this deliberately preserves the pane-settle state.
function Dlc.reannounce()
    ann:reset()
end

function Dlc.reset()
    ann:reset()
    clear_observation()
end

return Dlc
