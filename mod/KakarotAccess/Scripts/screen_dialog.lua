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

-- Like node_speech but via Core.read_text (reflected GetText() fallback), for nodes that
-- may be native CFUIXcmnMultiLineText rather than the blueprint wrapper.
local function node_rt(node)
    if not Core.on_screen(node) then return nil end
    return A.markup_to_speech(Core.read_text(node))
end

-- Content rows of the window's typed pools (native UAT_UIGameWindow, Xcmn_Win01_C's
-- base class): item plates (name + count), community/soul-emblem rows (character name +
-- "Community Level" title + level numbers), link-bonus rows, check rows, and the single
-- detail-reward line. Reward/notice windows ("Soul Emblems Received", quest "Rewards")
-- show these BELOW the prompt — without reading them only the title+body spoke. Each
-- pool is pcall-guarded, so Xcmn_Win00_C (which has none of these members) skips
-- harmlessly. WL_TextPlateCtn is NOT here: those rows are selectable choices, handled
-- with selection semantics in choices() below.
local POOLS = {
    { arr = "WL_ItemPlateCtn",     texts = { "ItemName", "ItemNum" } },
    { arr = "WL_TextCmuCtn",       texts = { "WL_CharName", "WL_LevelTitle" }, fixed = "WL_LvTextList" },
    { arr = "WL_LinkListPlateCtn", texts = { "LinkNameText" } },
    { arr = "WL_CheckPlateCtn",    texts = { "TxtList" } },
}

local function plates(w)
    if not Core.valid(w) then return nil end
    local parts = {}
    for _, pool in ipairs(POOLS) do
        pcall(function()
            local arr = w[pool.arr]
            for i = 1, #arr do
                local row = arr[i]
                if Core.valid(row) and Core.on_screen(row) then
                    local p = {}
                    for _, m in ipairs(pool.texts) do
                        p[#p + 1] = node_rt(row[m])
                    end
                    -- fixed-size text array (per-community level numbers on emblem rows)
                    if pool.fixed then
                        pcall(function()
                            local fx = row[pool.fixed]
                            for j = 1, #fx do p[#p + 1] = node_rt(fx[j]) end
                        end)
                    end
                    if #p > 0 then parts[#parts + 1] = table.concat(p, ", ") end
                end
            end
        end)
    end
    pcall(function()
        local dr = w.WL_DetailReward
        if Core.valid(dr) and Core.on_screen(dr) then
            local s = Core.phrase(node_rt(dr.TxtReward), node_rt(dr.TxtName), node_rt(dr.TxtNum))
            if s ~= "" then parts[#parts + 1] = s end
        end
    end)
    if #parts == 0 then return nil end
    return table.concat(parts, ", ")
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
    -- A choice list OWNED by the window (WL_TextPlateCtn — e.g. the Soul Emblems "Sort"
    -- popup) uses the same Xcmn_Win00_Choice_C rows but NESTED under the window's tree,
    -- so the top-level filter above never sees them. Same highlight rule.
    pcall(function()
        local arr = win.WL_TextPlateCtn
        for i = 1, #arr do
            local ch = arr[i]
            if Core.valid(ch) and Core.on_screen(ch) then
                local label = A.markup_to_speech(Core.read_text(ch.Txt_Choice))
                if label then
                    labels[#labels + 1] = label
                    if Core.is_visible(ch.Img_Xwin01_List) then sel = label end
                end
            end
        end
    end)
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
        -- Plain message: the prompt is the item; a text change re-announces. Content
        -- rows (rewards, received soul emblems, …) read as the tooltip, so a list that
        -- pops in a few ticks after the title is still spoken on its own.
        ann:focus(nil, nil, s.msg, nil, function() return plates(win) end)
    end
end

return Dialog
