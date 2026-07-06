-- Screen adapter: item palette registration (Start_Item_Customize_C).
--
-- Opened from the Items inventory with X ("Register palette"): pick which face-button
-- palette (Y/X/B/A rows) the selected item is bound to for quick battle use.
--
-- Structure VERIFIED from the game's own ASSET (extracted from pakchunk0:
-- Art/UI/Start_Item/Start_Item_Customize + Start_Item_Bar, name tables read offline —
-- the live pool/reflection sweeps abort on this game):
--   * host Start_Item_Customize_C : AT_UIItemPaletteCustomize; tree has Txt_Title,
--     Txt_Msg and the rows Start_Item_Bar00..03;
--   * each row is a Start_Item_Bar_C : AT_UIItemPaletteBar (AT.hpp) -> its NATIVE
--     members are safe: Txt_List (bound item), Txt_Num (count), Xcmn_Btn_Plat (the
--     row's face-button glyph) and the cursor images Img_Win01_Curs00 (+ Pnl_Curs_All
--     border), materials Ins_Cursor03/_Eff.
-- Rows are found as Start_Item_Bar_C INSTANCES (FindAllOf on a blueprint class name is
-- the reliable lookup on this game), filtered under the open host. Title/msg are read
-- from the text-box pool by path (proven live). The row's button resolves through
-- A.platbtn_token (controller ids -> action -> indexed face buttons -> texture).

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")

local Palette = {}

-- Diagnostic: appends collection state to dumps/dump_palette.txt whenever the popup
-- is on screen but the reader can't serve it (missing msg/bars) — so a silent palette
-- is diagnosable from the file alone. Cheap: writes only on state change.
-- OFF since 2026-07-06: gate (host enum 0) + selection (Pnl_Curs_All) verified live.
local DEBUG = false

local ann = Core.make_announcer()
local host = nil
local tick = 0
local nodes = nil     -- { title=box, msg=box, bars={ {bar=, key=} sorted by name } }
local state = nil     -- { title, name, msg } computed in is_active
local next_collect = 0   -- throttle for re-collection while incomplete
local last_dbg = nil

local function dbg(s)
    if not DEBUG or s == last_dbg then return end
    last_dbg = s
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_palette.txt", "a")
        if f then f:write(string.format("[%d] %s\n", os.time(), s)) f:close() end
    end)
end

-- Version stamp on (re)load, so the diagnostic file proves WHICH build is running
-- (a missed Ctrl+Shift+R is indistinguishable from silence otherwise).
if DEBUG then
    pcall(function()
        local src = debug.getinfo(1, "S").source:sub(2)
        local dir = src:match("^(.*)[/\\]") or "."
        local f = io.open(dir .. "\\dumps\\dump_palette.txt", "a")
        if f then f:write(string.format("[%d] === palette reader v4 loaded ===\n", os.time())) f:close() end
    end)
end

local function clean(t) return t and A.markup_to_speech(t) or nil end

local function box_text(box)
    if not Core.valid(box) then return nil end
    local s
    pcall(function() s = box.Text:ToString() end)
    return (s and s ~= "") and clean(s) or nil
end

-- Collect the host's title/msg text boxes + its Start_Item_Bar_C rows once per
-- activation. Only proven-safe enumeration: the text-box pool by path, and the row
-- INSTANCES by their blueprint class name.
local function collect()
    nodes = { bars = {} }
    local prefix = host:GetFullName():match("%s(.+)$") or host:GetFullName()
    for _, o in pairs(Core.cached_all("CFUIMultiLineTextBox", tick)) do
        if Core.valid(o) then
            local fn = o:GetFullName()
            if fn:find(prefix, 1, true) then
                if fn:find(".Txt_Title.", 1, true) then
                    nodes.title = nodes.title or o
                elseif fn:find(".Txt_Msg.", 1, true) then
                    nodes.msg = o
                end
            end
        end
    end
    for _, o in pairs(Core.cached_all("Start_Item_Bar_C", tick)) do
        if Core.valid(o) then
            local fn = o:GetFullName()
            if fn:find(prefix, 1, true) then
                nodes.bars[#nodes.bars + 1] = { bar = o, key = fn }
            end
        end
    end
    table.sort(nodes.bars, function(a, b) return a.key < b.key end)
end

-- "<button>: <item or empty slot>, <count>" for one row. All native members
-- (AT_UIItemPaletteBar, AT.hpp).
local function bar_phrase(bar)
    local t = clean(Core.read_text(bar.Txt_List))
    if not t then return nil end
    if t:match("^[%-—–_ ]+$") then t = I18n.t("empty_socket") end
    local tok = A.platbtn_token(bar.Xcmn_Btn_Plat)
    local line = Core.phrase(t, clean(Core.read_text(bar.Txt_Num)))
    return tok and (I18n.button(tok) .. ": " .. line) or line
end

-- The selected row: the one showing its Pnl_Curs_All cursor border — VERIFIED live
-- (telemetry 2026-07-06): exactly one row true, moves with the cursor.
-- (Img_Win01_Curs00 is visible on EVERY row — not a selection signal.)
local function selected_bar()
    local hit, count = nil, 0
    for _, e in ipairs(nodes.bars) do
        if Core.valid(e.bar) and Core.is_visible(e.bar.Pnl_Curs_All) then
            count = count + 1
            hit = e.bar
        end
    end
    return count == 1 and hit or nil
end

function Palette.is_active()
    tick = tick + 1
    host = Core.first_on_screen("Start_Item_Customize_C", tick)
    if not host then nodes, state = nil, nil return false end
    -- (Re)collect until COMPLETE: the first attempt can run on a tick where the scan
    -- budget is spent (cached_all then returns an empty/stale list) — caching that
    -- empty result forever left the whole popup mute (user 2026-07-06). Throttled.
    if (not nodes or not nodes.msg or #nodes.bars == 0) and tick >= next_collect then
        next_collect = tick + 5
        collect()
        if DEBUG then
            dbg(string.format("collect: title=%s msg=%s bars=%d",
                tostring(nodes.title ~= nil), tostring(nodes.msg ~= nil), #nodes.bars))
        end
    end
    -- Live-signal telemetry (DEBUG): the popup activated while still CLOSED (user
    -- 2026-07-06 — it read over the Items menu), so log the candidate open/closed
    -- discriminators on every change: host enum + opacities + per-bar cursor
    -- visibility. One line per state change in dumps/dump_palette.txt.
    -- OPEN GATE (from live telemetry 2026-07-06): the popup's own slate enum is 0
    -- (Visible) ONLY while it is genuinely open; closed it parks at 3
    -- (HitTestInvisible). The earlier Txt_Msg on_screen gate read false even when
    -- open (and true in one earlier run) — unreliable, dropped.
    local he
    pcall(function() he = tonumber(host:GetVisibility()) end)
    if he ~= 0 then state = nil return false end
    local msg = nodes.msg and box_text(nodes.msg) or nil
    local sel = selected_bar()
    local name
    if sel then
        name = bar_phrase(sel)
    else
        -- No unique cursor: read every row once (never mute).
        local parts = {}
        for _, e in ipairs(nodes.bars) do
            if Core.valid(e.bar) then parts[#parts + 1] = bar_phrase(e.bar) end
        end
        name = #parts > 0 and table.concat(parts, ", ") or nil
    end
    if not name then
        dbg(string.format("no readable rows (bars=%d, sel=%s)", #nodes.bars, tostring(sel ~= nil)))
        state = nil
        return false
    end
    state = { title = box_text(nodes.title), name = name, msg = msg }
    return true
end

function Palette.reset() ann:reset() nodes = nil end

function Palette.update()
    local s = state
    if not s then return end
    -- title on entry; the focused row (or all rows, fallback) as it moves; the
    -- instruction line as the tooltip.
    ann:focus(s.title, nil, s.name, nil, function() return s.msg end)
end

return Palette
