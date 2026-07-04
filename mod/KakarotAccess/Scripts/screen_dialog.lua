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
-- Member sets differ per window class (Win00/01: Txt_Title + Txt_Detail; the Win02
-- tutorial/notice box: TextBox_Caption + the old→new level numbers; item notices:
-- Txt_Detail_Item), so each candidate is probed guarded and on_screen-gated.
local MESSAGE_MEMBERS = {
    "Txt_Title", "Txt_Detail", "TextBox_Caption", "Txt_Detail_Item",
    "TextBox_OldNum", "TextBox_NewNum",
}
local function message(w)
    if not Core.valid(w) then return nil end
    local parts = {}
    for _, m in ipairs(MESSAGE_MEMBERS) do
        local t
        pcall(function() t = node_speech(w[m]) end)
        -- consecutive-duplicate guard: the Win02 info box repeats its header as the
        -- subtitle ("Z Warrior Community, Z Warrior Community" read live)
        if t and t ~= parts[#parts] then parts[#parts + 1] = t end
    end
    if #parts == 0 then return nil end
    return table.concat(parts, ", ")
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
    local labels, sel, items = {}, nil, false
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
    -- Choice lists OWNED by the window, NESTED under its tree (the top-level filter
    -- above never sees them). Two row shapes, handled with member fallbacks:
    --   * Win01's WL_TextPlateCtn (Sort popup) = Xcmn_Win00_Choice_C: Txt_Choice +
    --     Img_Xwin01_List highlight.
    --   * Win02's UIChoice_List (the brown field DECISION box — "Check out this area
    --     a bit more / Return home") = UAT_UISystemWindowChoice: ChoiceTxt label, and
    --     the selected row shows its BasePlate (the yellow bar).
    for _, pool in ipairs({ "WL_TextPlateCtn", "UIChoice_List" }) do
        pcall(function()
            local arr = win[pool]
            for i = 1, #arr do
                local ch = arr[i]
                if Core.valid(ch) and Core.on_screen(ch) then
                    local label
                    pcall(function() label = A.markup_to_speech(Core.read_text(ch.Txt_Choice)) end)
                    if not label then
                        pcall(function() label = A.markup_to_speech(Core.read_text(ch.ChoiceTxt)) end)
                    end
                    if label then
                        labels[#labels + 1] = label
                        local hi = false
                        pcall(function()
                            hi = Core.is_visible(ch.Img_Xwin01_List) or Core.is_visible(ch.BasePlate)
                        end)
                        if hi then sel = label end
                    end
                end
            end
        end)
    end
    -- SELECTABLE ITEM lists (the community GIFT picker, and any other window whose
    -- WL_ItemPlateCtn rows carry the cursor): each UAT_UIGameWindowChoice row is
    -- ItemName + ItemNum with the same Xwin01List highlight (native member name has
    -- no underscores). Only engage when SOME row is highlighted — reward/notice
    -- windows show the same rows with NO cursor, and those must keep the read-once
    -- notice behavior (plates() reads them as content).
    pcall(function()
        local arr = win.WL_ItemPlateCtn
        local ilabels, isel = {}, nil
        for i = 1, #arr do
            local row = arr[i]
            if Core.valid(row) and Core.on_screen(row) then
                local label = Core.phrase(
                    A.markup_to_speech(Core.read_text(row.ItemName)),
                    A.markup_to_speech(Core.read_text(row.ItemNum)))
                if label ~= "" then
                    ilabels[#ilabels + 1] = label
                    local hi = false
                    pcall(function() hi = Core.is_visible(row.ImgXwin01List) end)
                    if not hi then
                        pcall(function() hi = Core.is_visible(row.Img_Xwin01_List) end)
                    end
                    if hi then isel = label end
                end
            end
        end
        if isel then
            for _, l in ipairs(ilabels) do labels[#labels + 1] = l end
            sel = isel
            items = true
        end
    end)
    return labels, sel, items
end

-- Live description of the selected item row (the pane under the list — the gift
-- picker repopulates it per cursor move). Tooltip in item-list mode only.
local function item_detail()
    local t
    pcall(function() t = node_rt(win.WL_TxtDetailItem) end)
    return t
end

-- The shared message-window classes: Xcmn_Win01_C (dialogs/confirmations/notices),
-- Xcmn_Win00_C (startup/loading system messages — autosave notice, "Checking system
-- data…", "Saving game…") and Xcmn_Win02_C (the brown tutorial/level-up box —
-- TextBox_Caption + old/new numbers, choices in UIChoice_List).
local WINDOW_CLASSES = { "Xcmn_Win01_C", "Xcmn_Win00_C", "Xcmn_Win02_C" }

-- Does this window own any on-screen choice rows? A field DECISION box (the brown
-- Xcmn_Win02 "Check out this area a bit more / Return home" prompt) has options but NO
-- message text, so message() alone would miss it — acquire() also accepts choice-only
-- windows. Cheap: the pool is small and collapsed rows fail the on_screen check.
local function window_has_choices(w)
    if not Core.valid(w) then return false end
    for _, pool in ipairs({ "WL_TextPlateCtn", "UIChoice_List" }) do
        local found = false
        pcall(function()
            local arr = w[pool]
            for i = 1, #arr do
                local ch = arr[i]
                if Core.valid(ch) and Core.on_screen(ch) then found = true return end
            end
        end)
        if found then return true end
    end
    return false
end

-- Find a live window ACTUALLY on screen (on_screen, not IsVisible) that shows text OR
-- offers choices. Uses the persistent cache (these windows are pooled and persist), so
-- this is a cheap on_screen check on a cached ref, not a per-tick FindAllOf scan.
local function acquire()
    win = nil
    for _, cls in ipairs(WINDOW_CLASSES) do
        local w = Core.cached_live(cls, tick)
        if Core.on_screen(w) and (message(w) or window_has_choices(w)) then win = w return end
    end
end

-- Active ONLY when a message window is really on screen. The Yes/No choices are separate
-- top-level widgets, so we tie them to a shown window rather than trusting their own
-- IsVisible — otherwise pooled stale confirmations (still IsVisible under a collapsed
-- ancestor) would keep this adapter "active" over the pause menu / tutorials.
-- DEBUG: when a window is up, append its class, message and EVERY content/choice
-- pool row (class + texts + highlight flag) to dumps/dump_dialog.txt on each message
-- change — for pinning windows whose selectable rows aren't read yet (live case:
-- a first-time community notice with 2 side-by-side options that stayed silent).
-- Turn OFF once those rows are identified.
local DEBUG = false
local last_dumped = nil

local DUMP_POOLS = {
    "WL_ItemPlateCtn", "WL_TextCmuCtn", "WL_LinkListPlateCtn", "WL_CheckPlateCtn",
    "WL_TextPlateCtn", "UIChoice_List", "Relation_List", "WL_StatePlateCtn",
}
local ROW_TEXTS = {
    "Txt_Choice", "ChoiceTxt", "ItemName", "ItemNum", "WL_CharName", "WL_LevelTitle",
    "TxtList", "LinkNameText", "TxtName", "TxtNum",
}

local function dump_window(msg)
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    local f = io.open(dir .. "\\dumps\\dump_dialog.txt", "a")
    if not f then return end
    local wname = "?"
    pcall(function() wname = win:GetFullName():match("^%S+ (%S+)") or "?" end)
    f:write(string.format("[%d] win=%s msg=%s\n", os.time(), wname, tostring(msg)))
    for _, pool in ipairs(DUMP_POOLS) do
        pcall(function()
            local arr = win[pool]
            for i = 1, #arr do
                local row = arr[i]
                if Core.valid(row) and Core.on_screen(row) then
                    local cls = row:GetFullName():match("^(%S+)") or "?"
                    local texts = {}
                    for _, m in ipairs(ROW_TEXTS) do
                        local t
                        pcall(function() t = Core.read_text(row[m]) end)
                        if t then texts[#texts + 1] = m .. "=" .. t end
                    end
                    local hi = ""
                    pcall(function()
                        if Core.is_visible(row.Img_Xwin01_List) or Core.is_visible(row.BasePlate) then
                            hi = " HIGHLIGHT"
                        end
                    end)
                    f:write(string.format("  %s[%d] %s %s%s\n", pool, i, cls,
                        table.concat(texts, " "), hi))
                end
            end
        end)
    end
    f:close()
end

-- The last fully-announced no-choice notice still on screen. NOT cleared by reset()
-- (the registry resets adapters on every switch — clearing it there would make a
-- released notice re-claim and re-announce in a loop).
local spoken = nil

function Dialog.is_active()
    tick = tick + 1
    if not Core.on_screen(win) then acquire() end   -- acquire is now cheap (cached refs)
    if not Core.on_screen(win) then
        spoken = nil
        state = { msg = nil, labels = {}, sel = nil }
        return false
    end
    local msg = message(win)
    local labels, sel, items = choices()
    -- Nothing to say (no message AND no options): release.
    if not msg and #labels == 0 then
        spoken = nil
        state = { msg = nil, labels = {}, sel = nil }
        return false
    end
    if DEBUG then
        -- Signature covers choice-only windows (msg is nil for a field decision box).
        local sig = (msg or "nomsg") .. "|" .. #labels .. "|" .. (labels[1] or "")
        if sig ~= last_dumped then last_dumped = sig; dump_window(msg) end
    end
    -- A NOTICE (no choices) owns the screen only until it has been announced; then it
    -- RELEASES the tick so an interactive screen underneath keeps reading — the
    -- community board's tutorial popups PERSIST until the task is done and were muting
    -- the whole board (the cursor moves under them). A new text re-claims once.
    if #labels == 0 and msg == spoken then return false end
    state = { msg = msg, labels = labels, sel = sel, items = items }
    return true
end

function Dialog.reset() ann:reset() end

function Dialog.update()
    local s = state or {}
    if s.labels and #s.labels > 0 then
        -- Confirmation / selectable list: prompt spoken once (as the screen), the
        -- highlighted option as the focused item (re-announced as you move). Item
        -- lists (gift picker) add the live per-item description as the tooltip.
        ann:focus(s.msg, nil, s.sel or s.labels[1], nil,
            s.items and item_detail or nil)
    else
        -- Plain message: the prompt is the item; a text change re-announces. Content
        -- rows (rewards, received soul emblems, …) read as the tooltip, so a list that
        -- pops in a few ticks after the title is still spoken on its own.
        ann:focus(nil, nil, s.msg, nil, function() return plates(win) end)
        -- Mark announced only once the announcer has no pending tooltip, so late
        -- reward rows still get their window before the notice releases the screen.
        if not ann.pending then spoken = s.msg end
    end
end

return Dialog
