-- Screen adapter: Community — Soul Emblems (grid + per-character detail).
--
-- Two screens (verified against the user's screenshots, 2026-07-03):
--   * Soul Emblem GRID — native `AT_UICommunityStart` (no blueprint in the CXX dump
--     session): LB/RB pages of emblem slots in `EmbList.EmbAry`
--     (`UAT_UIXCmnEmb_Cursor` each; the emblem itself is its `UIXCmnEmb`).
--     LIVE-VERIFIED (dump_community.txt 2026-07-03): the selected slot is the one
--     whose AnimLoop is PLAYING (transient — fires while the cursor moves, so the
--     last resolved index is held while idle); Txt_Commu is the emblem's COMMUNITY
--     LEVEL; acquisition comes from the face brush (acquired = MaterialInstanceDynamic
--     whose texture parameter is the character icon → the CHAR_TOKENS name;
--     unacquired = the constant "?" mask) — ImageUnacquired is never visible.
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

-- Appends grid-slot signal samples to dumps/dump_community.txt. OFF since selection,
-- acquisition and character names were all pinned live (2026-07-03); re-enable if the
-- grid misbehaves again.
local DEBUG = false

local ann = Core.make_announcer()
local tick = 0
local grid, det = nil, nil
local mode = nil            -- "detail" | "grid"; announcer resets on change
local sticky_sig = nil      -- once a signal proved unique, test only that one (perf)
local last_idx = nil        -- selection held between cursor moves (the signal is transient)

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

-- Cursor signals (live-verified in dump_community.txt, 2026-07-03): the selected slot
-- is the one whose AnimLoop is PLAYING. It's transient (stops when the cursor idles),
-- so update() holds on to the last resolved index while the grid stays up. The
-- visibility candidates were useless live (Ins_Frame_Set false everywhere, the effect
-- canvases true everywhere), so they're gone; AnimEnter kept as a fallback.
local SIGNALS = {
    { key = "anim",  test = function(s)
        local ok, p = pcall(function() return s:IsAnimationPlaying(s.AnimLoop) end)
        return ok and p == true
    end },
    { key = "enter", test = function(s)
        local ok, p = pcall(function() return s:IsAnimationPlaying(s.AnimEnter) end)
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

-- The emblem face's brush resource full name. Live-verified (dump_community.txt):
-- an ACQUIRED slot swaps the face brush to a MaterialInstanceDynamic carrying the
-- character icon as a texture parameter; an unacquired one keeps the constant "?"
-- mask material. CAUTION: the MID is confusingly ALSO named "Ins_Emb_Mask", so
-- acquisition is decided by the MaterialInstanceDynamic part of the name, never by
-- the mask name (that bug made every emblem read "not acquired").
local function face_resource(emb)
    local name
    pcall(function()
        local ro = emb.ImageFace.Brush.ResourceObject
        if ro and ro:IsValid() then name = ro:GetFullName() end
    end)
    return name
end

-- Character-icon token → spoken name. The token is the game's own asset-naming
-- abbreviation (textures /Game/Art/UI/Charicon_Ev/Ev_<Tok>NN_…; full token list
-- extracted from the pak index, 2026-07-03). Proper nouns, so no localization.
-- An UNMAPPED token is spoken raw (e.g. "Bnw") — still identifiable, and it tells
-- us exactly which entry to add.
local CHAR_TOKENS = {
    Gok = "Goku", Ghn = "Gohan", Gtn = "Goten", Vgt = "Vegeta", Trk = "Trunks",
    Pcl = "Piccolo", Krn = "Krillin", Ymc = "Yamcha", Tsh = "Tien",
    Coz = "Chiaotzu", Chi = "Chi-Chi", Brm = "Bulma", Vdl = "Videl",
    Mrs = "Mr. Satan", Mbo = "Majin Buu", Dbl = "Dabura", Bbd = "Babidi",
    Spp = "Spopovich", Ymu = "Yamu", Ers = "Erasa",
    Frz = "Frieza", Mfr = "Mecha Frieza", Kcl = "King Cold",
    Cel = "Cell", Cej = "Cell Jr.", Rdt = "Raditz", Npa = "Nappa", Sbm = "Saibaman",
    Gny = "Ginyu", Rcm = "Recoome", Brt = "Burter", Jes = "Jeice", Gld = "Guldo",
    Cui = "Cui", Ddr = "Dodoria", Zbn = "Zarbon", Apr = "Appule",
    Dnd = "Dende", Nil = "Nail", Kbt = "Kibito", Kki = "King Kai", Kym = "King Yemma",
    Mpp = "Mr. Popo", Mst = "Master Roshi", Okg = "Ox-King", Oln = "Oolong",
    Lnc = "Launch", Yjr = "Yajirobe", Krr = "Korin", Upa = "Upa", Mrn = "Marron",
    Plf = "Pilaf", Mai = "Mai", Shu = "Shu", Bee = "Bee", Bbl = "Bubbles",
    Geg = "Gregory", Srn = "Shenron", Tpp = "Mercenary Tao", Pui = "Pui Pui",
    Ykn = "Yakon", Wis = "Whis",
}

-- Character name of an ACQUIRED emblem: the face MID's texture parameter is the
-- character icon (…/Charicon_Ev/Ev_Gok00_00_00 → token "Gok" → "Goku").
local function face_char(emb)
    local tok
    pcall(function()
        local tp = emb.ImageFace.Brush.ResourceObject.TextureParameterValues
        for j = 1, #tp do
            local tex = tp[j].ParameterValue
            if tex and tex:IsValid() then
                tok = tex:GetFullName():match("/Charicon_%w+/%a-_(%a+)")
                if tok then return end
            end
        end
    end)
    return tok and (CHAR_TOKENS[tok] or tok) or nil
end

-- What a slot sounds like: character name (acquired), "not acquired" ("?" mask),
-- labeled community level (Txt_Commu is the bare level number — live-verified),
-- the new marker, and the position in the visible grid.
local function slot_label(s, idx, count)
    local emb
    pcall(function() emb = s.UIXCmnEmb end)
    local face = Core.valid(emb) and face_resource(emb) or nil
    local acquired = face ~= nil and face:find("MaterialInstanceDynamic", 1, true) ~= nil
    local unacq = face ~= nil and not acquired
    local new = false
    if Core.valid(emb) then new = Core.is_visible(emb.ImageNew) end
    pcall(function() new = new or Core.is_visible(s.Icon_New) end)
    local name = acquired and face_char(emb) or nil
    local lv = acquired and read(s.Txt_Commu) or nil
    return Core.phrase(
        name,
        unacq and I18n.t("not_acquired") or nil,
        lv and string.format(I18n.t("commu_lv"), lv) or nil,
        new and I18n.t("new_label") or nil,
        string.format(I18n.t("pos"), idx, count))
end

-- ---- grid: DEBUG dump ---------------------------------------------------------

local function dump_path()
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    return dir .. "\\dumps\\dump_community.txt"
end

-- The face material's texture parameters (acquired slots use a MaterialInstanceDynamic;
-- its texture full names should identify the CHARACTER — the missing piece for speaking
-- names in the grid). Also its parent material, in case the id lives there.
local function face_debug(emb)
    local out = "-"
    pcall(function()
        local ro = emb.ImageFace.Brush.ResourceObject
        if not (ro and ro:IsValid()) then return end
        out = ro:GetFullName():match("([%w_%.]+)$") or "?"
        pcall(function()
            local p = ro.Parent
            if p and p:IsValid() then out = out .. " parent=" .. (p:GetFullName():match("([%w_%.]+)$") or "?") end
        end)
        pcall(function()
            local tp = ro.TextureParameterValues
            for j = 1, #tp do
                local tex = tp[j].ParameterValue
                if tex and tex:IsValid() then out = out .. " tex=" .. tex:GetFullName() end
            end
        end)
    end)
    return out
end

local function dump_grid(list, sel_idx, sig_key)
    local f = io.open(dump_path(), "a")
    if not f then return end
    f:write(string.format("[%d] grid slots=%d sel=%s sig=%s\n",
        os.time(), #list, tostring(sel_idx), tostring(sig_key)))
    for i, s in ipairs(list) do
        local emb; pcall(function() emb = s.UIXCmnEmb end)
        f:write(string.format("  %02d commu=%s face: %s\n", i,
            tostring(Core.read_text(s.Txt_Commu)),
            Core.valid(emb) and face_debug(emb) or "-"))
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
    -- The anim signal only fires while the cursor MOVES — keep the last resolved slot
    -- while idle so the state stays stable (and F1 repeat keeps working).
    if idx then
        if DEBUG and idx ~= last_idx then dump_grid(list, idx, sig) end
        last_idx = idx
    else
        idx = last_idx
    end
    if not idx or not list[idx] then return end
    ann:focus(I18n.header(5), nil, slot_label(list[idx], idx, #list), nil, nil)
end

return Commu
