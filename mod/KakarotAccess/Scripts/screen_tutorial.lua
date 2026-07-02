-- Screen adapter: battle tutorial / control-reference dialog (Battle_Tips_Tutorial_C).
--
-- Authoritative structure (from the CXX header dump, AT.hpp): the container exposes its
-- control rows as DIRECT properties Battle_Tips_List_00..08, and each row
-- (UAT_UIBattleTipsList) has Txt_Btn (the button glyph, inline <inputicon> markup) +
-- Txt_Operat (the action name). So we read each row as "button: action" — recovering the
-- button the old whole-subtree text scan dropped — and read the page header
-- (title/subtitle/page number) from the container's OTHER text nodes.
--
-- Perf: the container stays on_screen (HitTestInvisible) all battle, so we read the 9 named
-- rows directly (no per-tick FindAllOf) and cache the header text boxes once. Content is
-- committed only when stable across two scans and kept sticky while on screen, so a page
-- turn (1/2 -> 2/2) never re-reads page 1.

local Core = require("ui_core")
local A = require("ui_archetypes")

local Tutorial = {}

local ROW_COUNT = 9    -- Battle_Tips_List_00..08
local SCAN_EVERY = 3

local ann = Core.make_announcer()
local win = nil
local tick = 0
local cached = nil      -- committed content the announcer sees (updated only when stable)
local last_scan = nil   -- previous raw scan, for the 2-scan stability gate
local boxes = nil       -- cached HEADER text boxes (under the container, not inside a row)

-- Collect the container's header text boxes once (title/subtitle/page live outside the
-- control rows). Excludes the row boxes — those are read structurally below.
local function collect_boxes()
    boxes = {}
    if not Core.valid(win) then return end
    local prefix = win:GetFullName():match("%s(.+)$") or win:GetFullName()
    for _, o in pairs(Core.cached_all("CFUIMultiLineTextBox", tick)) do
        if Core.valid(o) then
            local fn = o:GetFullName()
            if fn:find(prefix, 1, true) and not fn:find("Battle_Tips_List", 1, true) then
                boxes[#boxes + 1] = o
            end
        end
    end
end

-- The row's button glyph resolved to speech: Txt_Btn's rich text (ExMainTxt) carries the
-- inline <inputicon KeyConfigId=…> markup, resolved by A.markup_to_speech. nil if none.
local function row_button(row)
    local node = Core.valid(row) and row.Txt_Btn
    if not Core.valid(node) then return nil end
    local rich = node.ExMainTxt
    if Core.valid(rich) then
        local ok, s = pcall(function() return rich.Text:ToString() end)
        if ok and s and s ~= "" then return A.markup_to_speech(s) end
    end
    return nil
end

local function content()
    -- Gate: the tutorial is only really showing if at least one control row is ON SCREEN
    -- (on_screen, not IsVisible). The container itself stays on_screen (HitTestInvisible) for
    -- the whole battle, so without this gate stale header/row text could falsely activate the
    -- tutorial and shadow the pause menu underneath.
    local rows_present = false
    for i = 0, ROW_COUNT - 1 do
        if Core.on_screen(win["Battle_Tips_List_0" .. i]) then rows_present = true break end
    end
    if not rows_present then return nil end

    local parts = {}
    -- 1) page header (title / subtitle / page number) — container text that isn't a row.
    for _, o in ipairs(boxes or {}) do
        if Core.is_visible(o) then
            local ok, s = pcall(function() return o.Text:ToString() end)
            if ok and s and s ~= "" then parts[#parts + 1] = s end
        end
    end
    -- 2) control rows, structural: "button: action".
    local rowlines = 0
    for i = 0, ROW_COUNT - 1 do
        local row = win["Battle_Tips_List_0" .. i]
        if Core.on_screen(row) then
            local action = Core.text_of(row.Txt_Operat)
            local button = row_button(row)
            local line = (button and action and (button .. ": " .. action)) or button or action
            if line then parts[#parts + 1] = line ; rowlines = rowlines + 1 end
        end
    end
    -- 3) safety fallback: if the structural row read yielded nothing (e.g. the native row
    -- props weren't reachable), fall back to the old behaviour so we never regress below the
    -- bare action names.
    if rowlines == 0 then
        local prefix = win:GetFullName():match("%s(.+)$") or win:GetFullName()
        for _, o in pairs(Core.cached_all("CFUIMultiLineTextBox", tick)) do
            if Core.is_visible(o) then
                local fn = o:GetFullName()
                if fn:find(prefix, 1, true) and fn:find("Battle_Tips_List", 1, true) then
                    local ok, s = pcall(function() return o.Text:ToString() end)
                    if ok and s and s ~= "" then parts[#parts + 1] = s end
                end
            end
        end
    end
    return #parts > 0 and table.concat(parts, ", ") or nil
end

function Tutorial.is_active()
    tick = tick + 1
    win = Core.cached_live("Battle_Tips_Tutorial_C", tick)   -- cheap: cached ref
    if not Core.on_screen(win) then cached, last_scan, boxes = nil, nil, nil return false end
    if not boxes then collect_boxes() end
    -- Commit content only when a NON-nil scan repeats (stable); never drop cached to nil on
    -- a transient scan while on screen, so a page turn doesn't deactivate + re-read page 1.
    if cached == nil or tick % SCAN_EVERY == 0 then
        local c = content()
        if c and c == last_scan then cached = c end
        last_scan = c
    end
    return cached ~= nil
end

function Tutorial.reset() ann:reset() end

function Tutorial.update()
    ann:focus(nil, nil, cached, nil, nil)
end

return Tutorial
