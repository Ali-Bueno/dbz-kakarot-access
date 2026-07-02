-- Screen adapter: battle tutorial / control-reference dialog (Battle_Tips_Tutorial_C).
--
-- A paged reference screen ("Battle Controls / Basic Controls, 1/2"): a title/subtitle/
-- page header plus a set of control labels (Battle_Tips_List rows). The header nodes
-- are NOT direct properties of the container, so instead of binding to exact node names
-- we read EVERY visible text box under the container (robust to the nested layout) and
-- announce the whole page, re-announcing on page change.
--
-- The button glyph per control is image-only (no KeyConfigId text), so only the action
-- names are read — the player can cross-reference the controller-config tab for buttons.

local Core = require("ui_core")

local Tutorial = {}

local SCAN_EVERY = 3   -- recompute the (heavy) 870-textbox scan only every Nth tick

local ann = Core.make_announcer()
local win = nil
local tick = 0
local cached = nil     -- committed content the announcer sees (only updated once stable)
local last_scan = nil  -- previous raw scan, to require a page's content stabilize before announcing
local boxes = nil      -- the container's text boxes, collected ONCE (not scanned per tick)

-- Collect the container's text boxes once (the pool is static). CRITICAL for perf: the
-- Battle_Tips_Tutorial_C container stays on_screen (HitTestInvisible) for the WHOLE battle
-- even with no tutorial up, so scanning all ~870 CFUIMultiLineTextBox every tick to find
-- its children would stall the game thread throughout combat/pause. We instead grab its
-- boxes once and just check their visibility each tick.
local function collect_boxes()
    boxes = {}
    if not Core.valid(win) then return end
    local prefix = win:GetFullName():match("%s(.+)$") or win:GetFullName()
    for _, o in pairs(Core.cached_all("CFUIMultiLineTextBox", tick)) do
        if Core.valid(o) and o:GetFullName():find(prefix, 1, true) then boxes[#boxes + 1] = o end
    end
end

-- Visible text under the container, joined in widget order — over the CACHED box set, so
-- no per-tick FindAllOf. A box is genuinely shown when is_visible (the container is already
-- on_screen-gated); when no tutorial is up all boxes are collapsed → returns nil.
local function content()
    local parts = {}
    for _, o in ipairs(boxes or {}) do
        if Core.is_visible(o) then
            local ok, s = pcall(function() return o.Text:ToString() end)
            if ok and s and s ~= "" then parts[#parts + 1] = s end
        end
    end
    return #parts > 0 and table.concat(parts, ", ") or nil
end

function Tutorial.is_active()
    tick = tick + 1
    win = Core.cached_live("Battle_Tips_Tutorial_C", tick)   -- cheap: cached ref
    if not Core.on_screen(win) then cached, last_scan, boxes = nil, nil, nil return false end
    -- Collect boxes once per container; retry occasionally if the pool wasn't ready yet.
    if not boxes then collect_boxes()
    elseif #boxes == 0 and tick % 10 == 0 then collect_boxes() end
    -- Commit a page only when a NON-nil scan repeats (stable). Crucially, while the tutorial
    -- is on screen we NEVER drop `cached` back to nil on a transient empty/mixed scan during
    -- a page turn — doing so would deactivate the screen and re-announce page 1 when it came
    -- back. So a turn goes page1 → (held) → page2, without re-reading page 1.
    if cached == nil or tick % SCAN_EVERY == 0 then
        local c = content()
        if c and c == last_scan then cached = c end
        last_scan = c
    end
    return cached ~= nil
end

function Tutorial.reset() ann:reset() end

function Tutorial.update()
    -- Whole page as the item; a page change (new text) re-announces.
    ann:focus(nil, nil, cached, nil, nil)
end

return Tutorial
