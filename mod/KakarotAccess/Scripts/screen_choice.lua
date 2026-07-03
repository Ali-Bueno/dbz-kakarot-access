-- Screen adapter: dialogue / quest choices (Choice_Win_C prompt window and its
-- Choice_Cmd_C option rows — also used standalone under quest windows).
--
-- The selected option is the row whose hover highlight is visible
-- (blueprint `Dmy_Choice_Hover`, native `HoverImage` as fallback) — that IS the
-- game's selection mechanism for this family (no reflected index; from the CXX dump:
-- UAT_UIChoiceCmd has TextBox_Choice + HoverImage, Choice_Cmd_C adds Txt_Choice +
-- Dmy_Choice_Hover). Spoken like a confirmation dialog: the surrounding PROMPT once
-- as the screen (the Choice_Win message, or the sub-story window's title+detail when
-- the choice belongs to a Quest_Sub_C accept box), then the highlighted option as it
-- changes.

local Core = require("ui_core")
local A = require("ui_archetypes")

local Choice = {}

local ann = Core.make_announcer()
local tick = 0
local state = nil   -- { labels, sel, ctx } computed once per tick in is_active

local function node_speech(node)
    if not Core.on_screen(node) then return nil end
    return A.markup_to_speech(Core.read_text(node))
end

-- All on-screen choice rows + the hovered one. The pool is small and collapsed rows
-- fail on_screen at the first cheap visibility check, so this is light when idle.
local function options()
    local labels, sel = {}, nil
    for _, c in ipairs(Core.cached_all("Choice_Cmd_C", tick)) do
        if Core.on_screen(c) then
            local label
            pcall(function() label = node_speech(c.Txt_Choice) end)
            if label then
                labels[#labels + 1] = label
                local hov = false
                pcall(function()
                    hov = Core.is_visible(c.Dmy_Choice_Hover) or Core.is_visible(c.HoverImage)
                end)
                if hov then sel = label end
            end
        end
    end
    return labels, sel
end

-- The prompt around the choice, spoken once on entry: the Choice_Win message, else
-- the sub-story accept window's quest title + description.
local function context()
    local win = Core.cached_live("Choice_Win_C", tick)
    if Core.on_screen(win) then
        local t
        pcall(function() t = node_speech(win.Txt_Choice) or node_speech(win.TextBox_Message) end)
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

function Choice.is_active()
    tick = tick + 1
    local labels, sel = options()
    if #labels == 0 then state = nil return false end
    -- The prompt is resolved once per activation (it doesn't change mid-choice) so a
    -- tick doesn't pay window text reads for an already-announced context.
    local ctx = (state and state.ctx) or context()
    state = { labels = labels, sel = sel, ctx = ctx }
    return true
end

function Choice.reset() ann:reset() end

function Choice.update()
    local s = state
    if not s then return end
    ann:focus(s.ctx, nil, s.sel or s.labels[1], nil, nil)
end

return Choice
