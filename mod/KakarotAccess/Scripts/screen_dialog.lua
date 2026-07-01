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

local REACQUIRE_EVERY = 3   -- dialogs pop in/out; re-find quickly when we have none

local ann = Core.make_announcer()
local win = nil
local tick = 0
local state = nil   -- {msg, labels, sel} computed once per tick in is_active, reused by update

-- The speakable prompt of a dialog window (title + body, markup resolved), or nil.
local function message(w)
    if not Core.valid(w) then return nil end
    local title = A.markup_to_speech(Core.text_of(w.Txt_Title))
    local body = A.markup_to_speech(Core.text_of(w.Txt_Detail))
    local text = Core.phrase(title, body)
    return text ~= "" and text or nil
end

-- The active Yes/No confirmation options + the currently selected one. Active options
-- are the TOP-LEVEL Xcmn_Win00_Choice_C (direct GameInstance children); selection is
-- the one whose Img_Xwin01_List highlight is visible.
local function choices()
    local labels, sel = {}, nil
    for _, ch in pairs(FindAllOf("Xcmn_Win00_Choice_C") or {}) do
        if Core.valid(ch) and Core.is_visible(ch)
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

-- Find a live, visible Xcmn_Win01_C that is actually showing a message.
local function acquire()
    win = nil
    for _, w in pairs(FindAllOf("Xcmn_Win01_C") or {}) do
        if Core.valid(w) and w:GetFullName():find("/Engine/Transient", 1, true)
           and Core.is_visible(w) and message(w) then
            win = w
            break
        end
    end
end

function Dialog.is_active()
    tick = tick + 1
    if (not Core.valid(win) or not Core.is_visible(win)) and tick % REACQUIRE_EVERY == 0 then
        acquire()
    end
    local msg = Core.valid(win) and Core.is_visible(win) and message(win) or nil
    local labels, sel = choices()
    state = { msg = msg, labels = labels, sel = sel }   -- reused by update() this tick
    return msg ~= nil or #labels > 0
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
