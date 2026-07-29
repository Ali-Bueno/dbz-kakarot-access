-- Screen adapter: selection list inside a system window (Xcmn_Win01_C + Xcmn_Win01_List_C).
--
-- Used by pick-one dialogs such as the New Game "Difficulty Settings" screen
-- (Easy/Normal/Hard). Each option is an Xcmn_Win01_List_C with a Txt_Item label; the
-- SELECTED option is the one whose Img_Xwin01_List (highlight background) is visible.
-- The window's Txt_Title is the heading and Txt_Help is the selected option's
-- description (updates live as you move).
--
-- Registered BEFORE screen_dialog so it wins over the plain-message reader while a
-- choice list is up (the window's Txt_Detail prompt stays visible underneath).

local Core = require("ui_core")
local A = require("ui_archetypes")

local Choice = {}

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
Choice.keyhelp_auto = false

local ann = Core.make_announcer()
local win = nil
local tick = 0

-- On-screen option items (on_screen, not IsVisible — pooled list rows stay IsVisible
-- under a collapsed window) with a non-empty label.
local function options()
    local list = {}
    for _, it in pairs(Core.cached_all("Xcmn_Win01_List_C", tick)) do
        -- GUARD FIRST, name second (fixed 2026-07-29 — crash sweep). `GetFullName()`
        -- DEREFERENCES the handle, and this pool is not directory-mapped: `Core.cached_all`
        -- serves it from the FindAllOf cache, which keeps the whole list on its ~30 s refresh
        -- as long as ONE entry is still alive, so freed rows sit in it. `Xcmn_Win01_List_C` is
        -- not private to this screen — the substory reward sheet builds fresh instances of the
        -- same class and its host is destroyed on close, no map switch involved — so opening
        -- any system window shortly after one dereferenced those freed rows. `Core.on_screen`
        -- routes through Core.valid -> Mem.alive (and is memoised per tick), so testing it
        -- first is both the guard and a saving: the path string is now built only for rows
        -- that survive. Every other GetFullName-on-a-cached-pool site in the mod already does
        -- this; this was the sole outlier.
        if Core.on_screen(it) and it:GetFullName():find("/Engine/Transient", 1, true) then
            local label = Core.text_of(Core.member(it, "Txt_Item"))
            if label then list[#list + 1] = { it = it, label = label } end
        end
    end
    return list
end

-- The selected option = the one whose highlight background is visible.
local function selected(list)
    for _, e in ipairs(list) do
        if Core.is_visible(Core.member(e.it, "Img_Xwin01_List")) then return e.label end
    end
    return nil
end

function Choice.is_active()
    tick = tick + 1
    win = Core.cached_live("Xcmn_Win01_C", tick)   -- cheap: cached ref, no per-tick scan
    if not Core.on_screen(win) then return false end
    return selected(options()) ~= nil
end

function Choice.reset() ann:reset() end

function Choice.update()
    local name = selected(options())
    local title = Core.text_of(Core.member(win, "Txt_Title"))
    local desc = A.markup_to_speech(Core.text_of(Core.member(win, "Txt_Help")))
    -- title spoken on entry; moving selection → new option + its description.
    ann:focus(title, nil, name, desc, nil)
end

return Choice
