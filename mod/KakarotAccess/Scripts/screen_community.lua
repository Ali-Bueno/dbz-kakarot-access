-- Screen adapter: Community — Soul Emblems (grid + per-character detail) and the
-- Community Board (the 2D emblem-placement editor).
--
-- Screens (user screenshots + live dumps, 2026-07-03):
--   * Emblem DETAIL — `Start_Commu_Detail_C` (blueprint known from the CXX dump):
--     character name (Txt_Name), community level (Txt_Commu_Lv), popularity
--     (Txt_Popular00), link bonus (Txt_Link + Txt_Link_Detail), description
--     (Txt_Char_Detail) and the reward bars (Xlist_Reward_Commu rows, have/max).
--   * Soul Emblem GRID — native `AT_UICommunityStart`: LB/RB pages of emblem slots
--     in `EmbList.EmbAry` (`UAT_UIXCmnEmb_Cursor` each; the emblem is its
--     `UIXCmnEmb`). LIVE-VERIFIED (dump_community.txt): selected slot = the one
--     whose AnimLoop is PLAYING (transient — the last resolved index is held while
--     the cursor idles); Txt_Commu = the emblem's COMMUNITY LEVEL; acquisition =
--     face brush is a MaterialInstanceDynamic (its texture parameter is the
--     character icon → CHAR_TOKENS name) vs the constant "?" mask — CAUTION: the
--     MID is ALSO named "Ins_Emb_Mask"; ImageUnacquired is never visible.
--   * COMMUNITY BOARD — native `AT_UICommunityBoard`: LB/RB boards (Z Warrior,
--     Cooking, …), a free cursor over emblem SOCKETS (`WL_BrdFrame.WL_PanelTbl`,
--     `UAT_UICommunityBoard_Panel` each: `WL_Emblem` → same face-texture name read,
--     `WL_Lv` level, leader pedestal) and the right-hand summary
--     (`WL_CommuBrdDetail`: title, overall level, rank, active skills). Hovered
--     socket: a panel's ActiveAnim if it singles one out, else the NEAREST socket
--     to the cursor widget by canvas-slot position (unverified live — DEBUG dumps
--     what's needed to pin them).
--
-- The header logos are image fonts, so the mod supplies the screen names from the
-- game's own EXCmnHeaderFontType ids (I18n.header 5 = Soul Emblems, 1 = board).
--
-- PERF: the user hit real input lag here. Rules: ONE reflected-anim scan per tick,
-- the focused label CACHED (recomputed on index change or ~1 s — fresh reads cost
-- GetFullName + texture-parameter walks per 100 ms), socket positions cached per
-- board, and file I/O only behind DEBUG on selection changes.

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")
local Speech = require("speech")
local Mem = require("mem")

local Commu = {}

-- Appends selection samples to dumps/dump_community.txt (grid pinned live 2026-07-03;
-- the BOARD cursor/socket mapping is still unverified). ON while the board is being
-- verified: dumps board state even when NO socket resolves (that silent case is
-- exactly what needs diagnosing), throttled to one entry per ~3 s.
local DEBUG = true

local ann = Core.make_announcer()
local tick = 0
local det, grid, board = nil, nil, nil
local mode = nil            -- "detail" | "grid" | "board"; announcer resets on change
local last_idx = nil        -- selection held between cursor moves (signals are transient)
local label_cache, label_idx, label_tick = nil, nil, 0
local LABEL_REFRESH = 10    -- ticks (~1 s) between forced refreshes of a same slot's label
local last_title = nil      -- board title, gates the spoken board summary
local panel_cache = nil     -- { key, list = {panel...}, xy = {{x,y}...} } per board
local last_detect_sig = nil -- DEBUG: last screen-classification signature dumped
local board_found = false   -- DEBUG: Start_Commu_Brd_C host seen (even if frame hidden)

local function clean(t) return t and A.markup_to_speech(t) or nil end

-- Cleaned text of a node; is_visible (not on_screen) — the host is already
-- on_screen-gated, so children don't need the ancestor walk every tick.
local function read(node)
    if not Core.valid(node) then return nil end
    if not Core.is_visible(node) then return nil end
    return clean(Core.read_text(node))
end

local function anim_playing(w, anim)
    local ok, p = pcall(function() return w:IsAnimationPlaying(anim) end)
    return ok and p == true
end

-- Index of the item a predicate singles out, or nil if it matches zero or several.
local function unique_match(list, test)
    local hit, n = nil, 0
    for i, s in ipairs(list) do
        if test(s) then
            n = n + 1
            if n > 1 then return nil end
            hit = i
        end
    end
    return n == 1 and hit or nil
end

-- ---- character identity from the emblem face --------------------------------

-- The emblem face's brush resource full name. An ACQUIRED emblem swaps the face
-- brush to a MaterialInstanceDynamic carrying the character icon as a texture
-- parameter; an unacquired one keeps the constant "?" mask material.
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

-- ---- grid: slots + labels -----------------------------------------------------

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

-- What a grid slot sounds like: character name (acquired), "not acquired" ("?"
-- mask), labeled community level, the new marker, and the grid position.
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

-- ---- board: sockets, cursor, summary -------------------------------------------

-- Canvas-slot position of a widget (Left/Top of its CanvasPanelSlot offsets), or nil.
-- Plain reflected data — NEVER read RenderTransform here (uncatchable abort, seen on
-- the pause menu 2026-07-01).
local function slot_xy(w)
    local x, y
    pcall(function()
        local off = w.Slot.LayoutData.Offsets
        x, y = off.Left + 0.0, off.Top + 0.0
    end)
    return x, y
end

-- The board's sockets + their positions, cached per board (they only change when
-- the board itself changes; re-scanning canvas slots per tick is wasted cost).
local function board_panels(frame, key)
    if panel_cache and panel_cache.key == key then return panel_cache end
    local c = { key = key, list = {}, xy = {} }
    pcall(function()
        local arr = frame.WL_PanelTbl
        for i = 1, #arr do
            local p = arr[i]
            if Core.valid(p) and Core.is_visible(p) then
                local x, y = slot_xy(p)
                c.list[#c.list + 1] = p
                c.xy[#c.list] = { x = x, y = y }
            end
        end
    end)
    panel_cache = c
    return c
end

-- The board cursor's position, from the first cursor widget with a readable slot.
-- 0,0 counts as UNSET (live 2026-07-03: every canvas slot on this screen reads 0,0 —
-- the board positions everything via render transforms, unreadable by reflection),
-- so a bogus "nearest to the origin" socket is never announced.
local CURSOR_MEMBERS = { "WL_PanelCursor", "WL_Img_Curs_Fing00", "WL_EmbCursorFrame" }
local function cursor_xy(frame)
    for _, m in ipairs(CURSOR_MEMBERS) do
        local w
        pcall(function() w = frame[m] end)
        if Core.valid(w) then
            local x, y = slot_xy(w)
            if x and not (x == 0 and y == 0) then return x, y, m end
        end
    end
end

-- The hovered socket: a panel's ActiveAnim when it singles one out, else the
-- nearest socket to the cursor position.
local function board_selected(frame, pc)
    local idx = unique_match(pc.list, function(p) return anim_playing(p, p.ActiveAnim) end)
    if idx then return idx, "anim" end
    local cx, cy = cursor_xy(frame)
    if not cx then return nil, nil end
    local best, bd
    for i, xy in ipairs(pc.xy) do
        if xy.x then
            local d = (xy.x - cx) ^ 2 + (xy.y - cy) ^ 2
            if not bd or d < bd then bd, best = d, i end
        end
    end
    return best, "near"
end

-- What a socket sounds like: its emblem's character + level (+ leader pedestal),
-- or "empty socket", plus the socket position.
local function panel_label(p, idx, count)
    local name
    local embr
    pcall(function() embr = p.WL_Emblem end)
    if Core.valid(embr) and Core.is_visible(embr) then
        pcall(function() name = face_char(embr.UIXCmnEmb) end)
    end
    local lv = read(p.WL_Lv)
    local plus = read(p.WL_Plus_Lv)
    local leader = false
    pcall(function()
        leader = Core.is_visible(p.WL_Pnl_Pedestal_Leader) or Core.is_visible(p.WL_Ins_Icon_Leader)
    end)
    return Core.phrase(
        name or I18n.t("empty_socket"),
        lv and string.format(I18n.t("lvl"), lv) or nil,
        plus,
        leader and I18n.t("leader") or nil,
        string.format(I18n.t("pos"), idx, count))
end

-- The right-hand pane: board title (used as the announcer tab) and the summary
-- (overall level, to-next-rank, rank, active skills) spoken when the board changes.
local function board_detail()
    local d
    pcall(function() d = board.WL_CommuBrdDetail end)
    return Core.valid(d) and d or nil
end

local function board_title()
    local d = board_detail()
    return d and read(d.WL_Txt_Titl00) or nil
end

local function board_summary()
    local d = board_detail()
    if not d then return nil end
    local parts = {}
    local function pair(a, b)
        local t = Core.phrase(read(d[a]), read(d[b]))
        if t ~= "" then parts[#parts + 1] = t end
    end
    pair("WL_Txt_Level00", "WL_Txt_Level_Num00")   -- overall level caption + value
    pair("WL_Txt_Level01", "WL_Txt_Level_Num01")   -- to-next-rank caption + value
    pair("WL_Txt_Rank", "WL_Txt_Rank_Num00_00")    -- rank caption + value
    pcall(function()
        for i = 0, 2 do
            local sk = d[string.format("WL_Brd_Activ_Skill%02d", i)]
            if Core.valid(sk) and Core.is_visible(sk) then
                local t = read(sk.WL_Txt_Activ_Skill)
                if t then parts[#parts + 1] = t end
            end
        end
    end)
    if #parts == 0 then return nil end
    return table.concat(parts, ", ")
end

-- ---- DEBUG dumps ----------------------------------------------------------------

local function dump_path()
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    return dir .. "\\dumps\\dump_community.txt"
end

-- DEBUG: the hovered-socket index is NOT reflected anywhere (every canvas slot on
-- this screen reads 0,0 — positions are render-transform driven), so it must live in
-- the unreflected memory of the board host or its panel frame. Sample both windows
-- and log whichever int32s change as the cursor moves (the F4-memdiff technique,
-- in-place): one user pass over a few sockets pins the offset for native_offsets.
local TAIL_WINDOWS = {
    { key = "board", from = 0x3F0, len = 0x248 },   -- Start_Commu_Brd_C gaps (0x520..0x5E0 incl.)
    { key = "frame", from = 0x520, len = 0x438 },   -- Brd frame: 0x618..0x868 gap + 0x8B8.. tail
}
local tail_prev = {}

local function le_i32(s, i)
    local a, b, c, d = s:byte(i, i + 3)
    if not d then return nil end
    local v = a + b * 256 + c * 65536 + d * 16777216
    if v >= 0x80000000 then v = v - 0x100000000 end
    return v
end

local function dump_tail_changes(frame)
    local objs = { board = board, frame = frame }
    local lines = {}
    for _, w in ipairs(TAIL_WINDOWS) do
        local o = objs[w.key]
        local cur = Core.valid(o) and Mem.bytes(o, w.from, w.len) or nil
        if cur then
            local prev = tail_prev[w.key]
            if prev and prev ~= cur then
                local n = 0
                for off = 0, w.len - 4, 4 do
                    local a, b = le_i32(prev, off + 1), le_i32(cur, off + 1)
                    if a and b and a ~= b and b >= -1 and b <= 2000 then
                        n = n + 1
                        if n <= 16 then
                            lines[#lines + 1] = string.format("%s+0x%X=%d(was %d)",
                                w.key, w.from + off, b, a)
                        end
                    end
                end
            end
            tail_prev[w.key] = cur
        end
    end
    if #lines > 0 then
        local f = io.open(dump_path(), "a")
        if f then
            f:write(string.format("[%d] tails %s\n", os.time(), table.concat(lines, " ")))
            f:close()
        end
    end
end

local function dump_board(frame, pc, idx, how, title)
    local f = io.open(dump_path(), "a")
    if not f then return end
    local cx, cy, src = cursor_xy(frame)
    f:write(string.format("[%d] board '%s' sockets=%d sel=%s how=%s cursor=%s,%s via %s\n",
        os.time(), tostring(title), #pc.list, tostring(idx), tostring(how),
        tostring(cx), tostring(cy), tostring(src)))
    for i, p in ipairs(pc.list) do
        local xy = pc.xy[i]
        f:write(string.format("  %02d xy=%s,%s active=%s label=%s\n", i,
            tostring(xy.x), tostring(xy.y),
            tostring(anim_playing(p, p.ActiveAnim)),
            panel_label(p, i, #pc.list)))
    end
    f:close()
end

-- ---- detail readout --------------------------------------------------------------

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

-- ---- adapter protocol -------------------------------------------------------------

local function clear_state()
    last_idx, label_cache, label_idx = nil, nil, nil
    last_title, panel_cache = nil, nil
end

-- Grid slots for this tick, computed in is_active and reused by update (the grid and
-- the board ALTERNATE with the A "Switch" button and can both report on_screen — the
-- grid only wins when it actually shows slots, otherwise the board would be shadowed
-- into silence, which is exactly what happened live on the placement screen).
local grid_slots = nil

function Commu.is_active()
    tick = tick + 1
    grid_slots = nil
    det = Core.first_on_screen("Start_Commu_Detail_C", tick)
    local m = det and "detail" or nil
    if not m then
        -- BOARD before GRID: the emblem grid stays rendered (21 visible slots)
        -- UNDERNEATH the board, so grid-first read "Soul Emblems, Goku, 1 of 21"
        -- while the board was the real screen (live 2026-07-03). The board host is
        -- the BLUEPRINT class (pak index: Start_Commu/Start_Commu_Brd) — FindAllOf
        -- on the native AT_UICommunityBoard found NOTHING live. It only owns the
        -- screen while its panel frame is really rendered (pooled-closed collapses).
        board = Core.first_on_screen("Start_Commu_Brd_C", tick)
        board_found = board ~= nil
        if board then
            local frame
            pcall(function() frame = board.WL_BrdFrame end)
            if Core.valid(frame) and Core.on_screen(frame) then
                m = "board"
            else
                board = nil   -- host up but frame not rendered (or wrong base class)
            end
        end
    end
    if not m then
        grid = Core.first_on_screen("AT_UICommunityStart", tick)
        if grid then
            grid_slots = slots()
            if #grid_slots > 0 then m = "grid" end
        end
    end
    if m ~= mode then
        ann:reset()
        clear_state()
    end
    mode = m
    -- DEBUG: record HOW the screen was classified whenever that changes — if the board
    -- never activates, this line is the evidence (host not found vs shadowed vs silent).
    if DEBUG then
        local sig = string.format("mode=%s det=%s grid=%s slots=%s board=%s(found=%s)",
            tostring(m), tostring(det ~= nil), tostring(grid ~= nil),
            grid_slots and #grid_slots or "-", tostring(board ~= nil), tostring(board_found))
        if sig ~= last_detect_sig then
            last_detect_sig = sig
            local f = io.open(dump_path(), "a")
            if f then f:write(string.format("[%d] detect %s\n", os.time(), sig)) f:close() end
        end
    end
    return m ~= nil
end

function Commu.reset()
    ann:reset()
    mode = nil
    clear_state()
end

-- The focused label, cached: recomputed only when the index changes or every
-- LABEL_REFRESH ticks (a fresh read per tick was a real input-lag source here).
local function cached_label(idx, make)
    if idx ~= label_idx or tick - label_tick >= LABEL_REFRESH then
        label_idx, label_tick = idx, tick
        label_cache = make()
    end
    return label_cache
end

local function grid_update()
    local list = grid_slots or slots()
    if #list == 0 then return end
    -- The AnimLoop signal only fires while the cursor MOVES — hold the last resolved
    -- slot while idle so the state stays stable (and F1 repeat keeps working).
    local idx = unique_match(list, function(s) return anim_playing(s, s.AnimLoop) end)
    if idx then last_idx = idx else idx = last_idx end
    if not idx or not list[idx] then return end
    local label = cached_label(idx, function() return slot_label(list[idx], idx, #list) end)
    ann:focus(I18n.header(5), nil, label, nil, nil)
end

local last_dump = 0

local function board_update()
    local frame
    pcall(function() frame = board.WL_BrdFrame end)
    if not Core.valid(frame) then return end
    local title = board_title()
    local pc = board_panels(frame, title or "?")
    local idx, how
    if #pc.list > 0 then idx, how = board_selected(frame, pc) end
    if DEBUG then dump_tail_changes(frame) end
    -- DEBUG must capture the FAILING case too (no socket resolves → silence): dump on
    -- every selection change AND periodically while unresolved.
    if DEBUG and (idx ~= last_idx or (idx == nil and tick - last_dump > 300)) then
        last_dump = tick
        dump_board(frame, pc, idx, how, title)
    end
    if idx then
        last_idx = idx
    else
        idx = last_idx
    end
    local label = (idx and pc.list[idx])
        and cached_label(idx, function() return panel_label(pc.list[idx], idx, #pc.list) end)
        or nil
    -- Screen name spoken on entry; the board title is the tab (LB/RB announces it);
    -- the summary (level/rank/active skills) queues after whenever the board changes.
    ann:focus(I18n.header(1), title, label, nil, nil)
    if title and title ~= last_title then
        last_title = title
        panel_cache = nil                    -- socket layout changes with the board
        local s = board_summary()
        if s then Speech.say(s, false) end
    end
end

function Commu.update()
    if mode == "detail" then
        local name = read(det.Txt_Name)
        if not name then return end
        ann:focus(I18n.header(5), nil, name, nil, detail_text)
    elseif mode == "grid" then
        grid_update()
    elseif mode == "board" then
        board_update()
    end
end

return Commu
