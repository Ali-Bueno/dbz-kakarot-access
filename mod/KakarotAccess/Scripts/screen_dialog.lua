-- Screen adapter: system dialog / message window + Yes/No confirmations.
--
-- Two shared, game-wide patterns handled together:
--   * MESSAGE popup  — Xcmn_Win01_C with a prompt in Txt_Detail (+ optional Txt_Title),
--     e.g. the New Game "Select the battle difficulty…" notice.
--   * CONFIRMATION   — the same window plus Yes/No option buttons (Xcmn_Win00_Choice_C),
--     e.g. "Skip the movie?". Each choice is a Txt_Choice label; the SELECTED one has
--     its Img_Xwin01_List highlight visible. Only the TOP-LEVEL choice instances
--     (direct children of the GameInstance) are the active shared confirmation — the
--     ones nested under another screen are that screen's own pooled/stale widgets.
--
-- Text may carry CFramework markup (<span color=…>, <inputicon…>), resolved by
-- A.markup_to_speech. Registered FIRST so it overlays other screens while up.

local Core = require("ui_core")
local A = require("ui_archetypes")

local Dialog = {}

local ann = Core.make_announcer()
local win = nil
local tick = 0
local state = nil   -- {msg, labels, sel} computed once per tick in is_active, reused by update

-- Speakable text of a window text node, but ONLY if it's really on screen — a node that
-- IsVisible under a collapsed ancestor keeps STALE text (e.g. a prior "Difficulty
-- Settings" title bleeding into an unrelated "Skip the movie?" confirmation), so gate on
-- on_screen, not raw text.
local function node_speech(node)
    if not Core.on_screen(node) then return nil end
    return A.markup_to_speech(Core.text_of(node))
end

-- The speakable prompt of a dialog window (title + body, markup resolved), or nil.
local function message(w)
    if not Core.valid(w) then return nil end
    local text = Core.phrase(node_speech(w.Txt_Title), node_speech(w.Txt_Detail))
    return text ~= "" and text or nil
end

-- The active Yes/No confirmation options + the currently selected one. Active options
-- are the TOP-LEVEL Xcmn_Win00_Choice_C (direct GameInstance children) that are actually
-- on screen (on_screen, not just IsVisible — the pool keeps stale "Yes/No" choices whose
-- collapsed ancestor IsVisible ignores); selection is the one whose Img_Xwin01_List
-- highlight is visible.
local function choices()
    local labels, sel = {}, nil
    for _, ch in pairs(Core.cached_all("Xcmn_Win00_Choice_C", tick)) do
        if Core.on_screen(ch)
           and ch:GetFullName():match("BP_ATGameInstance_C_%d+%.Xcmn_Win00_Choice_C_%d+$") then
            local label = Core.text_of(ch.Txt_Choice)
            if label then
                labels[#labels + 1] = label
                if Core.is_visible(ch.Img_Xwin01_List) then sel = label end
            end
        end
    end
    return labels, sel
end

-- The shared message-window classes: Xcmn_Win01_C (dialogs/confirmations) and
-- Xcmn_Win00_C (startup/loading system messages — autosave notice, "Checking system
-- data…", "Saving game…"). Both hold the prompt in Txt_Detail (+ Txt_Title).
local WINDOW_CLASSES = { "Xcmn_Win01_C", "Xcmn_Win00_C" }

-- Find a live message window ACTUALLY on screen (on_screen, not IsVisible) showing text.
-- Uses the persistent cache (these windows are pooled and persist), so this is a cheap
-- on_screen check on a cached ref, not a per-tick FindAllOf scan.
local function acquire()
    win = nil
    for _, cls in ipairs(WINDOW_CLASSES) do
        local w = Core.cached_live(cls, tick)
        if Core.on_screen(w) and message(w) then win = w return end
    end
end

-- Active ONLY when a message window is really on screen. The Yes/No choices are separate
-- top-level widgets, so we tie them to a shown window rather than trusting their own
-- IsVisible — otherwise pooled stale confirmations (still IsVisible under a collapsed
-- ancestor) would keep this adapter "active" over the pause menu / tutorials.
function Dialog.is_active()
    tick = tick + 1
    if not Core.on_screen(win) then acquire() end   -- acquire is now cheap (cached refs)
    local msg = Core.on_screen(win) and message(win) or nil
    if not msg then state = { msg = nil, labels = {}, sel = nil } return false end
    local labels, sel = choices()
    state = { msg = msg, labels = labels, sel = sel }   -- reused by update() this tick
    return true
end

function Dialog.reset() ann:reset() end

function Dialog.update()
    local s = state or {}
    if s.labels and #s.labels > 0 then
        -- Confirmation: prompt spoken once (as the screen), the highlighted option as
        -- the focused item (re-announced when you move between Yes/No).
        ann:focus(s.msg, nil, s.sel or s.labels[1], nil, nil)
    else
        -- Plain message: the prompt is the item; a text change re-announces.
        ann:focus(nil, nil, s.msg, nil, nil)
    end
end

return Dialog
