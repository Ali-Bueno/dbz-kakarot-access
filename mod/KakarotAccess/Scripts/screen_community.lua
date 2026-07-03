-- Screen adapter: Community — Soul Emblems (grid + per-character detail).
--
-- Two screens (verified against the user's screenshots, 2026-07-03):
--   * Soul Emblem GRID — native `AT_UICommunityStart` (no blueprint in the CXX dump
--     session): LB/RB pages of emblem slots in `EmbList.EmbAry`
--     (`UAT_UIXCmnEmb_Cursor` each; the emblem itself is its `UIXCmnEmb`, whose
--     `ImageUnacquired` is the "?" overlay). The yellow selection frame's exact live
--     signal is UNVERIFIED — several candidates (Ins_Frame_Set / effect canvases /
--     the loop anim) are evaluated across all slots and the first one that singles
--     out EXACTLY ONE slot wins, so a wrong candidate yields silence, never a wrong
--     announcement. A DEBUG dump records every slot's signals to
--     dumps/dump_community.txt to pin the real one.
--   * Emblem DETAIL — `Start_Commu_Detail_C` (blueprint known from the CXX dump):
--     character name (Txt_Name), community level (Txt_Commu_Lv), popularity
--     (Txt_Popular00), link bonus (Txt_Link + Txt_Link_Detail), description
--     (Txt_Char_Detail) and the reward bars (Xlist_Reward_Commu rows, have/max).
--
-- The header logo is an image font, so the mod supplies the screen name
-- (I18n.header(5) = "Soul Emblems", the game's own EXCmnHeaderFontType id).

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")

local Commu = {}

-- Appends grid-slot signal samples to dumps/dump_community.txt (which visual marks the
-- selected slot, per-slot texts/face textures). Turn OFF once the grid reads correctly.
local DEBUG = true

local ann = Core.make_announcer()
local tick = 0
local grid, det = nil, nil
local mode = nil            -- "detail" | "grid"; announcer resets on change
local sticky_sig = nil      -- once a signal proved unique, test only that one (perf)
local last_idx, last_dump = nil, 0

local function clean(t) return t and A.markup_to_speech(t) or nil end

-- Cleaned text of a node; is_visible (not on_screen) — the host is already
-- on_screen-gated, so children don't need the ancestor walk every tick.
local function read(node)
    if not Core.valid(node) then return nil end
    if not Core.is_visible(node) then return nil end
    return clean(Core.read_text(node))
end

-- ---- grid: slots + selection ------------------------------------------------

-- The grid's emblem slots (on-screen entries of EmbList.EmbAry, in array order —
-- matches the visual left-to-right, top-to-bottom layout).
local function slots()
    local out = {}
    pcall(function()
        local arr = grid.EmbList.EmbAry
        for i = 1, #arr do
            local s = arr[i]
            if Core.valid(s) and Core.is_visible(s) then out[#out + 1] = s end
        end
    end)
    return out
end

-- Candidate cursor signals, most-likely first (cheap visibility probes before the
-- reflected anim call).
local SIGNALS = {
    { key = "frame", test = function(s) return Core.is_visible(s.Ins_Frame_Set) end },
    { key = "effA",  test = function(s) return Core.is_visible(s.CanvasEffectA) end },
    { key = "effB",  test = function(s) return Core.is_visible(s.CanvasEffectB) end },
    { key = "anim",  test = function(s)
        local ok, p = pcall(function() return s:IsAnimationPlaying(s.AnimLoop) end)
        return ok and p == true
    end },
}

-- Index of the slot a signal singles out, or nil if it matches zero or several.
local function unique_match(list, sig)
    local hit, n = nil, 0
    for i, s in ipairs(list) do
        local ok, v = pcall(sig.test, s)
        if ok and v then
            n = n + 1
            if n > 1 then return nil end
            hit = i
        end
    end
    return n == 1 and hit or nil
end

-- The selected slot's index: the sticky signal first (fast path), else probe all
-- candidates and remember whichever one is unique this tick.
local function selected(list)
    if sticky_sig then
        local hit = unique_match(list, sticky_sig)
        if hit then return hit, sticky_sig.key end
    end
    for _, sig in ipairs(SIGNALS) do
        local hit = unique_match(list, sig)
        if hit then sticky_sig = sig return hit, sig.key end
    end
    return nil, nil
end

-- What a slot sounds like: its own text if any (Txt_Commu), "not acquired" when the
-- emblem shows the "?" overlay, the new marker, and its position in the visible grid.
local function slot_label(s, idx, count)
    local emb
    pcall(function() emb = s.UIXCmnEmb end)
    local unacq, new = false, false
    if Core.valid(emb) then
        unacq = Core.is_visible(emb.ImageUnacquired)
        new = Core.is_visible(emb.ImageNew)
    end
    pcall(function() new = new or Core.is_visible(s.Icon_New) end)
    return Core.phrase(
        read(s.Txt_Commu),
        unacq and I18n.t("not_acquired") or nil,
        new and I18n.t("new_label") or nil,
        string.format(I18n.t("pos"), idx, count))
end

-- ---- grid: DEBUG dump ---------------------------------------------------------

local function dump_path()
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    return dir .. "\\dumps\\dump_community.txt"
end

local function dump_grid(list, sel_idx, sig_key)
    local f = io.open(dump_path(), "a")
    if not f then return end
    f:write(string.format("[%d] grid slots=%d sel=%s sig=%s\n",
        os.time(), #list, tostring(sel_idx), tostring(sig_key)))
    for i, s in ipairs(list) do
        local flags = {}
        for _, sg in ipairs(SIGNALS) do
            local ok, v = pcall(sg.test, s)
            flags[#flags + 1] = sg.key .. "=" .. tostring(ok and v == true)
        end
        local emb; pcall(function() emb = s.UIXCmnEmb end)
        local unacq = Core.valid(emb) and Core.is_visible(emb.ImageUnacquired)
        local face = ""
        pcall(function()
            local ro = emb.ImageFace.Brush.ResourceObject
            if ro and ro:IsValid() then face = ro:GetFullName():match("([%w_]+)$") or "" end
        end)
        f:write(string.format("  %02d %s unacq=%s commu=%s face=%s\n", i,
            table.concat(flags, " "), tostring(unacq),
            tostring(Core.read_text(s.Txt_Commu)), face))
    end
    f:close()
end

-- ---- detail readout -----------------------------------------------------------

-- The detail pane's reward bars (Xlist_Reward_Commu: reward text + have/max numbers).
local function reward_parts()
    local parts = {}
    for i = 0, 2 do
        local bar
        pcall(function() bar = det[string.format("Reward_Bar%02d", i)] end)
        if Core.valid(bar) and Core.is_visible(bar) then
            local p = Core.phrase(read(bar.Txt_Reward), read(bar.Txt_Num), read(bar.Txt_Max00))
            if p ~= "" then parts[#parts + 1] = p end
        end
    end
    return parts
end

local DETAIL_MEMBERS = {
    "Txt_Commu_Lv", "Txt_Popular00", "Txt_Link", "Txt_Link_Detail", "Txt_Char_Detail",
}

-- Full character detail (lazy: the announcer calls this only on selection change).
local function detail_text()
    local parts = {}
    for _, m in ipairs(DETAIL_MEMBERS) do
        local t
        pcall(function() t = read(det[m]) end)
        if t then parts[#parts + 1] = t end
    end
    for _, p in ipairs(reward_parts()) do parts[#parts + 1] = p end
    if #parts == 0 then return nil end
    return table.concat(parts, ", ")
end

-- ---- adapter protocol ----------------------------------------------------------

function Commu.is_active()
    tick = tick + 1
    det = Core.first_on_screen("Start_Commu_Detail_C", tick)
    grid = det == nil and Core.first_on_screen("AT_UICommunityStart", tick) or nil
    local m = (det and "detail") or (grid and "grid") or nil
    if m ~= mode then
        ann:reset()
        sticky_sig, last_idx = nil, nil
    end
    mode = m
    return m ~= nil
end

function Commu.reset()
    ann:reset()
    mode, sticky_sig, last_idx = nil, nil, nil
end

function Commu.update()
    if mode == "detail" then
        local name = read(det.Txt_Name)
        if not name then return end
        ann:focus(I18n.header(5), nil, name, nil, detail_text)
        return
    end
    local list = slots()
    if #list == 0 then return end
    local idx, sig = selected(list)
    if DEBUG and (idx ~= last_idx or tick - last_dump > 50) then
        last_dump = tick
        dump_grid(list, idx, sig)
    end
    last_idx = idx
    if not idx then return end
    ann:focus(I18n.header(5), nil, slot_label(list[idx], idx, #list), nil, nil)
end

return Commu
