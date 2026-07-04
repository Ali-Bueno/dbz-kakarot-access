-- Screen adapter: dialogue / quest response choices (the menu that pops up to answer
-- an NPC — e.g. Gohan's "go home?" Yes/No, and multi-option talk choices).
--
-- Widget family (CXX dump): the prompt window is UAT_UIChoiceWin (blueprint
-- Choice_Win_C) — TextBox_Message + two fixed options UIChoiceCmd_Yes / UIChoiceCmd_No;
-- each option is a UAT_UIChoiceCmd (blueprint Choice_Cmd_C) with TextBox_Choice (native)
-- / Txt_Choice (blueprint) and the HoverImage (native) / Dmy_Choice_Hover (blueprint)
-- highlight = the game's selection marker (no reflected index). Multi-option lists reuse
-- the same UAT_UIChoiceCmd rows.
--
-- ROBUSTNESS (community-board lesson): these are often instantiated from the NATIVE
-- class with NO `_C` at runtime, so we scan BOTH the blueprint and native short names
-- and read either member spelling. When a choice window is present we read its
-- Yes/No IN ORDER (Yes then No); otherwise we gather the standalone rows.
--
-- Spoken like a confirmation: the PROMPT once as the screen, then the highlighted
-- option as it changes.

local Core = require("ui_core")
local A = require("ui_archetypes")

local Choice = {}

-- Diagnostics: if the NPC response menu still reads wrong/silent, this writes what it
-- found (window class + message, each row's class/label/hover) to dumps/dump_choice.txt
-- on every prompt change. Turn OFF once confirmed working in-game.
local DEBUG = false

local ann = Core.make_announcer()
local tick = 0
local state = nil   -- { labels, sel, ctx } computed once per tick in is_active

local ROW_CLASSES = { "Choice_Cmd_C", "AT_UIChoiceCmd" }
local WIN_CLASSES = { "Choice_Win_C", "AT_UIChoiceWin" }

local function node_speech(node)
    if not Core.on_screen(node) then return nil end
    return A.markup_to_speech(Core.read_text(node))
end

-- A choice row's label (native TextBox_Choice or blueprint Txt_Choice) and whether it's
-- the hovered/selected one (native HoverImage or blueprint Dmy_Choice_Hover visible).
local function row_label(c)
    local label
    pcall(function() label = node_speech(c.TextBox_Choice) end)
    if not label then pcall(function() label = node_speech(c.Txt_Choice) end) end
    return label
end
local function row_hover(c)
    local hov = false
    pcall(function() hov = Core.is_visible(c.HoverImage) or Core.is_visible(c.Dmy_Choice_Hover) end)
    return hov
end

-- The active choice window (message prompt + fixed Yes/No), if one is on screen.
local function choice_window()
    for _, cls in ipairs(WIN_CLASSES) do
        local w = Core.cached_live(cls, tick)
        if Core.on_screen(w) then return w end
    end
    return nil
end

-- All on-screen choice rows across both class spellings, de-duplicated by address.
local function all_rows()
    local rows, seen = {}, {}
    for _, cls in ipairs(ROW_CLASSES) do
        for _, c in ipairs(Core.cached_all(cls, tick)) do
            if Core.on_screen(c) then
                local key = tostring(c:GetAddress())
                if not seen[key] then seen[key] = true; rows[#rows + 1] = c end
            end
        end
    end
    return rows
end

-- Labels (in order) + the hovered label. Prefers the window's Yes/No pair (stable
-- order); falls back to whatever standalone rows are on screen.
local function options(win)
    local labels, sel = {}, nil
    local rows = {}
    if Core.valid(win) then
        for _, m in ipairs({ "UIChoiceCmd_Yes", "UIChoiceCmd_No" }) do
            local r
            pcall(function() r = win[m] end)
            if Core.valid(r) and Core.on_screen(r) then rows[#rows + 1] = r end
        end
    end
    if #rows == 0 then rows = all_rows() end
    for _, c in ipairs(rows) do
        local label = row_label(c)
        if label then
            labels[#labels + 1] = label
            if row_hover(c) then sel = label end
        end
    end
    return labels, sel
end

-- The prompt around the choice, spoken once on entry: the choice window's message,
-- else the sub-story accept window's quest title + description.
local function context(win)
    if Core.valid(win) then
        local t
        pcall(function() t = node_speech(win.TextBox_Message) or node_speech(win.Txt_Choice) end)
        if t then return t end
    end
    local qs = Core.cached_live("Quest_Sub_C", tick)
    if Core.on_screen(qs) then
        local t
        pcall(function()
            t = Core.phrase(node_speech(qs.TextBox_Title), node_speech(qs.TextBox_Detail))
        end)
        if t and t ~= "" then return t end
    end
    return nil
end

local last_dumped = nil
local function dump(win, labels, sel, ctx)
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    local f = io.open(dir .. "\\dumps\\dump_choice.txt", "a")
    if not f then return end
    local wn = "none"
    if Core.valid(win) then pcall(function() wn = win:GetFullName():match("^%S+ (%S+)") or "?" end) end
    f:write(string.format("[%d] win=%s ctx=%s sel=%s labels=[%s]\n",
        os.time(), wn, tostring(ctx), tostring(sel), table.concat(labels, " | ")))
    for _, cls in ipairs(ROW_CLASSES) do
        for _, c in ipairs(Core.cached_all(cls, tick)) do
            if Core.on_screen(c) then
                f:write(string.format("  %s label=%s hover=%s\n",
                    cls, tostring(row_label(c)), tostring(row_hover(c))))
            end
        end
    end
    f:close()
end

function Choice.is_active()
    tick = tick + 1
    local win = choice_window()
    local labels, sel = options(win)
    if #labels == 0 then state = nil return false end
    -- The prompt is resolved once per activation (it doesn't change mid-choice).
    local ctx = (state and state.ctx) or context(win)
    state = { labels = labels, sel = sel, ctx = ctx }
    if DEBUG and ctx ~= last_dumped then
        last_dumped = ctx
        pcall(dump, win, labels, sel, ctx)
    end
    return true
end

function Choice.reset() ann:reset() end

function Choice.update()
    local s = state
    if not s then return end
    ann:focus(s.ctx, nil, s.sel or s.labels[1], nil, nil)
end

return Choice
