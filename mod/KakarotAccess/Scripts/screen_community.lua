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
--   * COMMUNITY BOARD — `Start_Commu_Brd_C` (blueprint of AT_UICommunityBoard; the
--     native class is never a live instance). Its free analog cursor is UNREADABLE
--     (verified 2026-07-04: WL_PanelCursor and 0x8C0 bytes of host+frame memory all
--     stayed frozen across a full move pass — the state is in the native input
--     system, like the battle pause). So the board is NOT cursor-tracked; on entry it
--     reads the summary (`WL_CommuBrdDetail`: title, overall level, rank, active
--     skills) + the emblems already placed (`WL_BrdFrame.WL_PanelTbl` → each
--     `WL_Emblem` face) + a hint that confirm opens the accessible Soul Emblems grid.
--
-- The header logos are image fonts, so the mod supplies the screen names from the
-- game's own EXCmnHeaderFontType ids (I18n.header 5 = Soul Emblems, 1 = board).

local Core = require("ui_core")
local A = require("ui_archetypes")
local I18n = require("i18n")
local Speech = require("speech")
local Mem = require("mem")
local OFF = require("native_offsets")
local Transition = require("transition")
local Dir = require("ui_directory")
local Registry = require("ui_registry")

local BOARD = OFF.commuBoard
local GRID = OFF.commuGrid

local Commu = {}

-- Diagnostics (hunt-v2 memory diff + 2 s state snapshots to dumps/dump_community.txt).
-- The cursor mystery is SOLVED (2026-07-04, verified in-game: tutorial modes freeze
-- the cursor; hovered read from host+0x5D8 / the hit-test replica) — OFF unless a
-- regression needs ground truth again.
-- ON 2026-07-16: regression "moving the board cursor announces no panel" in the
-- story-tutorial replay — dump_state is the ground truth (mode/sub/hovered/cache).
-- Turn OFF once the tutorial pass is verified.
local DEBUG = false

local ann = Core.make_announcer()
local tick = 0
local det, grid, board = nil, nil, nil
local mode = nil            -- "detail" | "grid" | "board"; announcer resets on change
local last_idx = nil        -- selection held between cursor moves (signals are transient)
local label_cache, label_idx, label_tick = nil, nil, 0
local LABEL_REFRESH = 10    -- ticks (~1 s) between forced refreshes of a same slot's label
local last_title = nil      -- board title, gates the spoken board summary
local panel_cache = nil     -- { key, list = {panel...} } per board
-- Placing-mode hover debounce state (helper defined below near board_update; declared
-- HERE so clear_state — defined above that helper — resets the real upvalues, not
-- accidental globals, per the Lua "upvalue only for functions defined after" rule).
local hov_stable, hov_cand, hov_cand_n = 0, 0, 0
local HOVER_STABLE = 2

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

-- Index of the slot whose cursor anim started MOST RECENTLY (smallest playback time),
-- or nil if none is playing. On a fast cursor move the PREVIOUS slot's transient
-- AnimLoop is often still running, and the old require-a-UNIQUE-playing-anim rule held
-- the stale slot until that anim ran out — the "huge delay" on emblem moves (user
-- 2026-07-06). GetAnimationCurrentTime is reflected (UMG.hpp) and only called on
-- slots already known to be playing.
local function newest_playing(list)
    local hit, best_t
    for i, s in ipairs(list) do
        if anim_playing(s, s.AnimLoop) then
            local t
            pcall(function() t = s:GetAnimationCurrentTime(s.AnimLoop) end)
            t = t or math.huge
            if not best_t or t < best_t then hit, best_t = i, t end
        end
    end
    return hit
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
    local ro
    pcall(function() ro = emb.ImageFace.Brush.ResourceObject end)
    local tp, n = Core.array_of(ro, "TextureParameterValues")
    if not tp then return nil end
    pcall(function()
        for j = 1, n do
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
-- matches the visual left-to-right, top-to-bottom layout). Second return: map from
-- the 0-based EmbAry index to the visible-list position, because the NATIVE cursor
-- (GRID.cursorIndex) indexes the raw array.
local function slots()
    local out, byai = {}, {}
    local emblist
    pcall(function() emblist = grid.EmbList end)
    local arr, n = Core.array_of(emblist, "EmbAry")
    if not arr then return out, byai end
    pcall(function()
        for i = 1, n do
            local s = arr[i]
            if Core.valid(s) and Core.is_visible(s) then
                out[#out + 1] = s
                byai[i - 1] = #out
            end
        end
    end)
    return out, byai
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

-- Widget position for the board: RenderTransform.Translation read RAW via mem_bridge
-- (REFLECTION on RenderTransform hard-aborts — pause lesson 2026-07-01 — but raw
-- SEH-guarded reads are safe; UWidget.RenderTransform sits at 0x90 per the UMG.hpp
-- CXX dump.)

-- The board's sockets, cached per board. RAW WL_PanelTbl order — the game's hit-test
-- indexes the raw array (frame+0x548) bounded by activeCount, so the list must NOT be
-- visibility-filtered or the indices would misalign on partially-unlocked boards.
local function board_panels(frame, key)
    -- Cached list must still be alive (a pooled board REBUILDS its panels per visit).
    if panel_cache and panel_cache.key == key
       and panel_cache.list[1] and Core.valid(panel_cache.list[1]) then
        return panel_cache
    end
    local c = { key = key, list = {} }
    local arr, n = Core.array_of(frame, "WL_PanelTbl")
    if arr then pcall(function()
        for i = 1, n do
            local p = arr[i]
            if not Core.valid(p) then break end   -- keep positions aligned: stop, don't skip
            c.list[i] = p
        end
    end) end
    -- NEVER cache an empty/failed read as final (the items-rebuild lesson): the
    -- story-tutorial board's WL_PanelTbl populates ticks after the frame exists, and
    -- a cached empty list froze the hit-test replica at "no sockets" for the whole
    -- visit — reticle readable (700,760), activeCount 11, hov=nil (2026-07-16).
    if #c.list > 0 then panel_cache = c end
    return c
end

-- CURSOR SOLVED (Ghidra 2026-07-04 + opus RE 2026-07-16, code/decompiled/
-- manual_1414f2ab0.c): the game's own hit-test reads WL_PanelCursor's
-- RenderTransform.Translation (raw +0x90/+0x94) — which is a FIXED RETICLE
-- (~700,760): the stick pans the BOARD underneath it, and that pan lives in the
-- frame adjusts (+0x428/0x42C, leader +0x430/0x434) that both the game and this
-- replica re-read on every call. Each socket's hit position = PointerCenterOffset +
-- hidden board position (panel+0x550) + adjust, square-range per socket (leader
-- socket 1 has its own adjust/range), scanning LAST -> FIRST, first hit wins.
-- board_hovered() replicates it 1:1 (offsets in native_offsets.commuBoard).
-- While an emblem is HELD the game's own hover tracker FUN_1414e3170 early-outs
-- (gate = frame+0x7B8 != 0) and host+0x5D8 freezes — placing mode has NO native
-- per-tick cache (the game only hit-tests on the Decide press), so this replica is
-- the ONLY per-tick source then.

-- The game's socket hit-test, replicated exactly (first hit wins, LAST socket first).
-- Returns the 1-based WL_PanelTbl index of the socket under the cursor, or nil.
local function board_hovered(frame, pc)
    local cw
    pcall(function() cw = frame.WL_PanelCursor end)
    if not Core.valid(cw) then return nil end
    local cx = Mem.float(cw, BOARD.cursorX)
    local cy = Mem.float(cw, BOARD.cursorY)
    if not cx or not cy then return nil end
    local count = Mem.i32(frame, BOARD.activeCount) or 0
    if count > #pc.list then count = #pc.list end
    local ax = Mem.float(frame, BOARD.hitAdjX) or 0
    local ay = Mem.float(frame, BOARD.hitAdjY) or 0
    local ar = Mem.float(frame, BOARD.hitRange) or 0
    for i = count, 1, -1 do
        local p = pc.list[i]
        local px = Mem.float(p, BOARD.panelPosX)
        local py = Mem.float(p, BOARD.panelPosY)
        local bx = Mem.float(p, BOARD.panelBoardX)
        local by = Mem.float(p, BOARD.panelBoardY)
        if px and bx then
            local ox, oy, r = ax, ay, ar
            if i == 1 then
                ox = Mem.float(frame, BOARD.leaderAdjX) or 0
                oy = Mem.float(frame, BOARD.leaderAdjY) or 0
                r = Mem.float(frame, BOARD.leaderRange) or 0
            end
            local hx, hy = px + bx + ox, py + by + oy
            if math.abs(hx - cx) <= r and math.abs(hy - cy) <= r then return i end
        end
    end
    return nil
end

-- What the hovered socket sounds like: emblem name + level + leader, or empty.
local function socket_label(p, idx, count)
    local name
    local embr
    pcall(function() embr = p.WL_Emblem end)
    if Core.valid(embr) and Core.is_visible(embr) then
        pcall(function() name = face_char(embr.UIXCmnEmb) end)
    end
    local where = string.format(I18n.t("board_socket"), idx, count)
    if not name then
        return Core.phrase(I18n.t("empty_socket"), where)
    end
    local lv = read(p.WL_Lv)
    local leader = false
    pcall(function()
        leader = Core.is_visible(p.WL_Pnl_Pedestal_Leader) or Core.is_visible(p.WL_Ins_Icon_Leader)
    end)
    return Core.phrase(name,
        lv and string.format(I18n.t("lvl"), lv) or nil,
        leader and I18n.t("leader") or nil,
        where)
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

-- Community Skills list (user request 2026-07-04): the thresholds panel ("Support
-- Gauge Initial Value +5% …"). It lives in the detail pane's WL_SkillParts — a FIXED
-- C array of 10 UAT_UICommunityBoardDetailSkillParts* (detail+0x470, size 0x50).
-- UE4SS reflection collapses fixed arrays to element 0; elements 1..9 are recovered
-- with RegisterCustomProperty at base+i*8 (the screen_party technique), registered
-- lazily against the widget's RUNTIME class path.
local SKILL_PARTS_BASE = 0x470
local SKILL_PARTS_N = 10
local skills_registered = false

local function ensure_skill_props(d)
    if skills_registered then return end
    local cls
    pcall(function() cls = d:GetClass():GetFullName():match("%s(.+)$") end)
    if not cls then return end
    skills_registered = true
    for i = 1, SKILL_PARTS_N - 1 do
        pcall(function()
            RegisterCustomProperty({
                ["Name"] = "CommuSkillPart" .. i,
                ["Type"] = PropertyTypes.ObjectProperty,
                ["BelongsToClass"] = cls,
                ["OffsetInternal"] = SKILL_PARTS_BASE + i * 8,
            })
        end)
    end
end

-- One skill row: prefers the ACTIVE variant's text (unlocked skills render through
-- WL_Txt_Skill_Act/WL_Txt_Num_Act; locked ones through WL_Txt_Skill/WL_Txt_Num).
local function skill_row(p)
    local act = false
    pcall(function() act = Core.is_visible(p.WL_Pnl_Skill_Act) end)
    local name, num
    if act then
        name = read(p.WL_Txt_Skill_Act) or read(p.WL_Txt_Skill)
        num = read(p.WL_Txt_Num_Act) or read(p.WL_Txt_Num)
    else
        name = read(p.WL_Txt_Skill) or read(p.WL_Txt_Skill_Act)
        num = read(p.WL_Txt_Num) or read(p.WL_Txt_Num_Act)
    end
    if not name then return nil end
    return Core.phrase(name, num)
end

local function skills_text()
    local d = board_detail()
    if not d then return nil end
    ensure_skill_props(d)
    local parts = {}
    local hdr
    pcall(function() hdr = read(d.WL_Txt_Skill_Header00) or read(d.WL_Txt_Skill_Header01) end)
    if hdr then parts[#parts + 1] = hdr end
    for i = 0, SKILL_PARTS_N - 1 do
        local p
        if i == 0 then
            pcall(function() p = d.WL_SkillParts end)
        else
            pcall(function() p = d["CommuSkillPart" .. i] end)
        end
        if Core.valid(p) and Core.is_visible(p) then
            local row = skill_row(p)
            if row then parts[#parts + 1] = row end
        end
    end
    if #parts == 0 or (hdr and #parts == 1) then return nil end
    return table.concat(parts, ", ")
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
    last_sub = nil
    hov_stable, hov_cand, hov_cand_n = 0, 0, 0
end

-- Map-switch flush (transition.lua): panel_cache holds live widget refs across ticks —
-- drop them so a level change can't leave dangling pointers here. Pure Lua.
Transition.on_begin("screen_community", clear_state)

-- Grid slots for this tick, computed in is_active and reused by update (the grid and
-- the board ALTERNATE with the A "Switch" button and can both report on_screen — the
-- grid only wins when it actually shows slots, otherwise the board would be shadowed
-- into silence, which is exactly what happened live on the placement screen).
local grid_slots, grid_byai = nil, nil

-- First-visit latency diagnosis (2026-07-16). NOTE: declared ABOVE grid_host — the
-- first cut declared these below it, so inside grid_host they resolved as (nil)
-- GLOBALS, not upvalues, and the gate lines silently never printed (a Lua local is
-- only an upvalue of functions defined AFTER it).
local ENTRY_DEBUG = false   -- verified 2026-07-16 (first visit ~1.5s, re-entry, no stutters)
local entry_t0, entry_gates = nil, nil

-- BOTH grid host classes must be watched: run 2 (2026-07-16) caught the first-of-session
-- menu flow materializing the NATIVE-named instance (AT_UICommunityStart, live host in
-- the gates log) while the watch covered only the BP name — 13 scans of the wrong pool.
local GRID_CLASSES = { "AT_UICommunityStart", "Start_Commu_Emb_C" }
-- Staggered so the two per-class cadences interleave (combined one 65ms scan per
-- ~400ms, not two back-to-back — the navigation lag spikes, user 2026-07-16).
local WATCH_STAGGER = 4
local wait_clock = nil   -- os.clock of the last FRESH arm; anchors renewals + their cap

-- FRESH arm (ring close / ghost board): starts the wait window renewals extend.
local function watch_grid()
    wait_clock = os.clock()
    if ENTRY_DEBUG then entry_t0, entry_gates = os.clock(), nil end
    for i, cls in ipairs(GRID_CLASSES) do
        Core.watch_for(cls, nil, (i - 1) * WATCH_STAGGER)
    end
end
-- RENEWAL: extends the deadline only — no stagger reset, no wait_clock reset (a renewal
-- that re-anchored the cap would never expire).
local function renew_grid()
    for _, cls in ipairs(GRID_CLASSES) do Core.watch_for(cls) end
end
local function unwatch_grid()
    for _, cls in ipairs(GRID_CLASSES) do Core.watch_clear(cls) end
end
-- Exported for screen_field's ring-close arm (the class list lives here only).
Commu.watch_grid = watch_grid

-- The Soul Emblems grid host. The MENU-opened grid ("EMBLEMAS DE ALMA") is the
-- BLUEPRINT class Start_Commu_Emb_C (census 2026-07-15) — FindAllOf on the native
-- AT_UICommunityStart returns nothing for it (the board's Start_Commu_Brd_C lesson),
-- which is why the menu flow stayed silent even unmapped. The board flow's instance
-- keeps the native name; try both.
-- The live grid host. ENUMERATES the pool instead of first_on_screen: the game can
-- keep an older parked instance on-screen (pane_live false) next to the live one, and
-- taking the first match latched the parked ghost (the fishing pooled-ring lesson).
local function grid_host()
    local hit = nil
    local dbg = (ENTRY_DEBUG and entry_t0) and {} or nil
    for _, cls in ipairs(GRID_CLASSES) do
        for _, g in ipairs(Core.cached_all(cls, tick)) do
            local v = Core.valid(g)
            local on = v and Core.on_screen(g) or false
            local live = on and Core.pane_live(g) or false
            if dbg then
                dbg[#dbg + 1] = string.format("%s v=%s on=%s live=%s",
                    cls, tostring(v), tostring(on), tostring(live))
            end
            if live and not hit then hit = g end
        end
        if hit and not dbg then return hit end
    end
    -- Diagnosis only (see ENTRY_DEBUG): one line per GATE-STATE change between the
    -- entry edge and the grid commit — "none" means the pool has no instance at all.
    if dbg then
        local sig = #dbg > 0 and table.concat(dbg, " | ") or "none"
        if sig ~= entry_gates then
            entry_gates = sig
            print(string.format("[KakarotAccess] emb gates t=%.2f %s\n", os.clock(), sig))
        end
    end
    return hit
end

local ghost_refresh_done = false   -- one watch-lane arm per menu-flow visit
local handoff_armed = false        -- one fresh arm per board→grid (mode 10) handoff
local tut_last = nil               -- TEMP claim-state trace signature (see is_active)
local speech_deferred = false      -- board/grid readout held while a protected line plays

-- Menu-flow ENTRY SIGNAL (first visit of a session read ~5s late — user 2026-07-16):
-- the grid class is NEVER-seen then, so it waited out the full absent-scan backoff, and
-- the ghost-board signature doesn't exist yet on a first visit either. But the game's
-- menu CONTROLLERS are lazy (the UMenuManager family lesson): UMenuManager's
-- m_xSoulEmblemMenu (AT.hpp 0x158) / UICommManager's MenuSoulEmListIns (0x80) are null
-- until the Soul Emblems menu is first opened — and their widget pointer is NOT
-- reflected (USoulEmblemMenu reflects only GameHUD, AT.hpp:43512), which is exactly why
-- the class can't be directory-MAPPED. The controller flipping null→valid is the
-- "opening right now" edge: put the grid's BP class on the ui_core watch lane so the
-- pool re-scans every ~400ms until the widget exists, instead of eating the backoff.
local menu_ctrl_seen = false

-- The controller edge NO LONGER ARMS anything (2026-07-16 final): run 2/3 proved the
-- game creates it DURING THE SAVE LOAD, not at menu entry, so arming/renewing off it
-- scanned through post-load GAMEPLAY for up to 30s — the user's free-roam stutters.
-- The real entry signals are the ring-close grace arm (screen_field) and the ghost
-- board; this stays as a diagnostic edge print only.
local function menu_entry_signal()
    local seen = Dir.peek("mm", "m_xSoulEmblemMenu") ~= nil
        or Dir.peek("cm", "MenuSoulEmListIns") ~= nil
    if ENTRY_DEBUG and seen ~= menu_ctrl_seen then
        print(string.format("[KakarotAccess] soul-emblem menu controller %s t=%.2f\n",
            seen and "appeared" or "GONE", os.clock()))
    end
    menu_ctrl_seen = seen
end

-- Wait-window maintenance: after a FRESH arm (watch_grid), keep the watch alive while
-- the user is plausibly waiting for the grid — nothing of ours reading, no other screen
-- claiming — for at most WAIT_RENEW_S (run 2: a first-visit construction can take 5-13s
-- with the tutorial popup in between, far past one 3s watch window). CANCELLED the
-- moment gameplay is confirmed: free roam (minimap back) OR combat (battle HUD up — the
-- minimap hides in battle and no registry adapter is active there, so without this a
-- back-out arm kept scanning through COMBAT, the user's stutter report 2026-07-16).
local WAIT_RENEW_S = 30

-- Combat probe: the battle HUD class is directory-mapped ({"bm","BattleHudPlayer"}) —
-- pointer reads, never a scan, so this is safe to consult per tick while waiting.
local function battle_hud_up()
    for _, w in ipairs(Core.cached_all("Battle_Hud_P_Main_C", tick)) do
        if Core.valid(w) and Core.on_screen(w) then return true end
    end
    return false
end

local function maintain_wait()
    if not wait_clock then return end
    if os.clock() - wait_clock > WAIT_RENEW_S then
        wait_clock = nil
    elseif Core.free_roam(tick) or battle_hud_up() then
        unwatch_grid()
        wait_clock = nil
    elseif mode == nil and Registry.active_adapter() == nil then
        renew_grid()
    end
end

-- Board mode-machine values under which the BOARD genuinely owns input (Ghidra
-- FUN_1414c7de0): 7 free-cursor browse, 9 detail, 12/13/14/17 tutorial-popup waits,
-- 16 link-bonus demo. 10 = the grid handoff (handled separately). Anything else
-- (5 observed on close; a freshly created hidden pane) is a PARKED board — without
-- this gate the ghost board shadowed the menu-opened EMBLEMAS DE ALMA grid: it read
-- the board summary on entry and the grid reader never ran (user + screenshot 98 +
-- Ctrl+F5, 2026-07-15 night).
local BOARD_LIVE_MODES = { [7] = true, [9] = true, [12] = true, [13] = true,
                           [14] = true, [16] = true, [17] = true }

function Commu.is_active()
    tick = tick + 1
    grid_slots, grid_byai = nil, nil
    menu_entry_signal()
    maintain_wait()
    local ghost_board = false
    local board_rej = nil   -- DEBUG: why the board was rejected this tick (or nil)
    -- pane_live gate (the standing pooled-pane rule): a PARKED detail sheet from an
    -- earlier visit keeps reporting on_screen and, checked FIRST, it claimed the
    -- screen over the live board — the story-tutorial "cursor announces no panel"
    -- suspect (2026-07-16).
    det = Core.first_on_screen("Start_Commu_Detail_C", tick)
    if det and not Core.pane_live(det) then det = nil end
    local m = det and "detail" or nil
    if not m then
        -- BOARD before GRID: the emblem grid stays rendered (21 visible slots)
        -- UNDERNEATH the board, so grid-first read "Soul Emblems, Goku, 1 of 21"
        -- while the board was the real screen (live 2026-07-03). The board host is
        -- the BLUEPRINT class (pak index: Start_Commu/Start_Commu_Brd) — FindAllOf
        -- on the native AT_UICommunityBoard found NOTHING live. It only owns the
        -- screen while its panel frame is really rendered (pooled-closed collapses).
        board = Core.first_on_screen("Start_Commu_Brd_C", tick)
        if not board then board_rej = "not-found" end
        if board then
            local frame
            pcall(function() frame = board.WL_BrdFrame end)
            -- Liveness is the MODE MACHINE, not pane_live. pane_live wants strict
            -- ESlateVisibility Visible(0), but the STORY-TUTORIAL board renders under a
            -- non-Visible flag while the tutorial owns input — Start_Commu_Brd_C_0 was
            -- present + fully populated in the F7 dump and the keyhelp said "board", yet
            -- pane_live rejected it, so the board was NEVER claimed and the empty panels
            -- read nothing ("los huecos vacíos no funcionan", 2026-07-16). Memory is
            -- explicit that pane_live NEVER discriminated the ghost pane — the mode gate
            -- did — so dropping it here is safe: a parked/ghost board reads a non-live
            -- mode and falls through below.
            local mode_v = Core.valid(frame) and Mem.i32(board, BOARD.mode) or nil
            board_rej =
                (not Core.valid(frame) and "frame-invalid")
                or (not Core.on_screen(frame) and "frame-offscreen")
                or (not mode_v and "no-mode") or nil
            if Core.valid(frame) and Core.on_screen(frame) and mode_v then
                -- Board MODE 10 = the emblem GRID owns the input (confirm on a socket
                -- opens it; the board stays rendered beneath, which is why board-first
                -- shadowed it into silence — sockets read, emblems didn't, user
                -- 2026-07-06). Hand the tick to the grid reader while it has slots;
                -- the other LIVE modes stay with the board; a parked mode (see
                -- BOARD_LIVE_MODES) means a ghost pane — fall through to the grid.
                if mode_v == 10 then
                    grid = grid_host()
                    if grid then
                        grid_slots, grid_byai = slots()
                        if #grid_slots > 0 then m = "grid" end
                    end
                    if not m then
                        m = "board"
                        -- Board→grid HANDOFF arm (community STORY TUTORIAL,
                        -- 2026-07-16): mode 10 says the grid owns input, but on the
                        -- session's first grid the widget doesn't exist yet — and
                        -- this flow has NO other entry signal (no ring close, no
                        -- ghost board), so the fresh grid sat out the never-seen
                        -- scan backoff while the tutorial asked to pick Gohan.
                        -- Mode 10 itself is the positive "user is waiting" evidence:
                        -- arm the watch lane fresh once, then renew it while the
                        -- handoff persists (maintain_wait can't renew here — an
                        -- adapter IS active), capped by the arm's own WAIT_RENEW_S.
                        if not handoff_armed then
                            handoff_armed = true
                            watch_grid()
                        elseif wait_clock
                            and os.clock() - wait_clock < WAIT_RENEW_S then
                            renew_grid()
                        end
                    end
                elseif BOARD_LIVE_MODES[mode_v or -1] then
                    m = "board"
                    handoff_armed = false
                else
                    board = nil        -- parked/ghost pane: not this adapter's screen
                    ghost_board = true -- …but it IS the menu-flow signature (see below)
                    board_rej = "ghost-mode=" .. tostring(mode_v)
                end
            else
                board = nil   -- host up but frame not rendered (or wrong base class)
            end
        end
    end
    if not m then
        grid = grid_host()
        if grid then
            grid_slots, grid_byai = slots()
            if #grid_slots > 0 then m = "grid" end
        elseif ghost_board and not ghost_refresh_done then
            -- "Reads only after a mod reload" (user 2026-07-15 night): the menu flow
            -- spawns a FRESH grid widget per visit while the older parked one keeps
            -- the pool cache "alive", so the scan never re-runs and the new screen
            -- stays invisible until the ~30 s pool refresh — the items-rebuild
            -- lesson. A parked board with no live grid is that flow's signature:
            -- arm the watch lane (re-scans every ~400ms, not the old single shot —
            -- one early scan could still miss the widget mid-construction) per visit.
            ghost_refresh_done = true
            watch_grid()
        end
    end
    if m then ghost_refresh_done = false end
    -- The watched screen actually reads now — stop the scan lane (any_valid can't do
    -- it in ui_core: a VALID parked instance is not the fresh screen, pane_live is).
    if m == "grid" then
        unwatch_grid()
        handoff_armed = false
        wait_clock = nil   -- flow satisfied: no more renewals this visit
        if entry_t0 then
            print(string.format("[KakarotAccess] emb grid commit +%.2fs after entry edge\n",
                os.clock() - entry_t0))
            entry_t0, entry_gates = nil, nil
        end
    end
    if m ~= mode then
        ann:reset()
        clear_state()
    end
    -- TEMP trace (2026-07-16 tutorial regression): one log line per CLAIM-state
    -- change — which sub-mode owns the tick and the board's native mode. Names the
    -- shadower if the board isn't the one reading. Turn OFF with DEBUG.
    if DEBUG then
        local sig = string.format("claim=%s det=%s board=%s grid=%s mode_v=%s rej=%s",
            tostring(m), tostring(det ~= nil), tostring(board ~= nil),
            tostring(grid ~= nil),
            board and tostring(Mem.i32(board, BOARD.mode)) or "-",
            tostring(board_rej))
        if sig ~= tut_last then
            tut_last = sig
            print("[KakarotAccess] commu " .. sig .. "\n")
        end
    end
    mode = m
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

-- GRID CURSOR DEBUG (user bug 2026-07-15 evening: the MENU-opened grid reads on entry
-- but says nothing while moving — the native cursor offsets were mapped on the BOARD
-- flow and the menu flow may not drive them). One line per raw-value change to
-- dumps/dump_community.txt: raw slot / col / row / anim fallback / mapped position.
-- OFF since 2026-07-15 night: movement VERIFIED in-game — the native commuGrid cursor
-- IS driven in the menu flow (gridcurs: raw/col/row advanced on Start_Commu_Emb_C,
-- byai mapping correct, n=21). Re-enable only if a new grid flow goes silent.
local GRID_DEBUG = false
local grid_dbg_last = nil

local function grid_debug(el, list, byai, raw, idx)
    local col, row = Mem.i32(el, GRID.colIndex), Mem.i32(el, GRID.rowIndex)
    local np = newest_playing(list)
    local sig = string.format("raw=%s col=%s row=%s np=%s idx=%s n=%d",
        tostring(raw), tostring(col), tostring(row), tostring(np), tostring(idx), #list)
    if sig == grid_dbg_last then return end
    grid_dbg_last = sig
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    local f = io.open(dir .. "\\dumps\\dump_community.txt", "a")
    if not f then return end
    local fn = "?"
    pcall(function() fn = grid:GetFullName():match("%s(.+)$") or "?" end)
    f:write(string.format("[gridcurs %s] %s host=%s\n", os.date("%H:%M:%S"), sig, fn))
    f:close()
end

local function grid_update()
    local list, byai = grid_slots, grid_byai
    if not list then list, byai = slots() end
    if #list == 0 then return end
    -- PRIMARY selection source: the NATIVE cursor slot (EmbList+0x3EC = row*7+col,
    -- fully mapped by the two gridhunt passes 2026-07-06) — instant and complete.
    -- The AnimLoop heuristic stays only as fallback (it trails the input and its
    -- leave-anim made a row+anim hybrid alternate between adjacent slots).
    local idx
    local el
    pcall(function() el = grid.EmbList end)
    if Core.valid(el) then
        local raw = Mem.i32(el, GRID.cursorIndex)
        if raw and raw >= 0 then idx = (byai or {})[raw] end
        if GRID_DEBUG then pcall(grid_debug, el, list, byai, raw, idx) end
    end
    if not idx then idx = newest_playing(list) end
    if idx then last_idx = idx else idx = last_idx end
    if not idx or not list[idx] then return end
    local label = cached_label(idx, function() return slot_label(list[idx], idx, #list) end)
    ann:focus(I18n.header(5), nil, label, nil, nil)
end

-- The emblems already PLACED on the board, read on entry so the player knows the
-- current layout without a live cursor (which is unreadable — see the note above).
local function placed_emblems(pc)
    local parts = {}
    for i, p in ipairs(pc.list) do
        local embr
        pcall(function() embr = p.WL_Emblem end)
        local name
        if Core.valid(embr) and Core.is_visible(embr) then
            pcall(function() name = face_char(embr.UIXCmnEmb) end)
        end
        if name then
            local lv = read(p.WL_Lv)
            local leader = false
            pcall(function()
                leader = Core.is_visible(p.WL_Pnl_Pedestal_Leader) or Core.is_visible(p.WL_Ins_Icon_Leader)
            end)
            -- socket number first, so it correlates with the live cursor's "socket N"
            parts[#parts + 1] = Core.phrase(string.format("%d", i),
                name,
                lv and string.format(I18n.t("lvl"), lv) or nil,
                leader and I18n.t("leader") or nil)
        end
    end
    return parts
end

-- ---- CURSOR HUNT (DEBUG, v2 2026-07-04) ---------------------------------------------
-- The widget layer is exhausted (cursor widgets + slots + render transforms all frozen
-- across a full left-stick pass). v2 diffs the LOGIC layer the widgets point at, none
-- of which was ever scanned (offsets from the CXX header dump, AT.hpp/ATExt.hpp):
--   * the host's own non-reflected tail gap 0x4E8..0x510 (between WL_Dmy_Bg 0x4E0 and
--     TargetCommBoradIns 0x510 — where the old tail-diff once saw 0x4F8/0x500 move),
--   * UAT_UICmnInput  @ host+0x618 — 0x460-byte input helper, ZERO reflected members,
--   * UATCommunityBoard @ host+0x600 (TargetBoard) — 0x138-byte model, ZERO reflected,
--   * UCommunityBoardMenu via UAT_UICommunityManager(host+0x5F8)+0x88,
--   * the host's WL_PnlCurs canvas (host+0x490) + its slot (the cursor lives on the
--     HOST canvas too, separate from the frame's WL_PanelCursor).
local function dump_path()
    local src = debug.getinfo(1, "S").source:sub(2)
    return (src:match("^(.*)[/\\]") or ".") .. "\\dumps\\dump_community.txt"
end

local function le_i32(s, i)
    local a, b, c, d = s:byte(i, i + 3)
    if not d then return nil end
    local v = a + b * 256 + c * 65536 + d * 16777216
    return v >= 0x80000000 and v - 0x100000000 or v
end

local CURSOR_MEMBERS = { "WL_PanelCursor", "WL_Img_Curs_Fing00", "WL_Img_Curs_Fing01", "WL_EmbCursorFrame" }
local cur_prev = {}

-- The UWidget.Slot pointer offset (UMG.hpp) — read the slot object's VA to scan it too.
local SLOT_PTR_OFF = 0x0110

local function scan_addr(key, addr, from, len, lines)
    if not addr then return end
    local cur = Mem.at_bytes(addr, from, len)
    if not cur then return end
    local prev = cur_prev[key]
    cur_prev[key] = cur
    if not prev or prev == cur then return end
    for off = 0, len - 4, 4 do
        local a, b = le_i32(prev, off + 1), le_i32(cur, off + 1)
        if a and b and a ~= b then
            local fb = string.unpack("<f", cur, off + 1)
            local repr = (fb == fb and math.abs(fb) > 1e-4 and math.abs(fb) < 1e6)
                and string.format("%.1ff", fb) or tostring(b)
            lines[#lines + 1] = string.format("%s+0x%X=%s(was %d)", key, from + off, repr, a)
        end
    end
end

-- Host-relative pointers to the opaque logic objects (CXX header dump offsets).
local HOST_UICMNINPUT_OFF = 0x618   -- UAT_UICmnInput* (0x460 bytes, no reflection)
local HOST_TARGETBOARD_OFF = 0x600  -- UATCommunityBoard* (0x138 bytes, no reflection)
local HOST_UICOMMMNG_OFF = 0x5F8    -- UAT_UICommunityManager*
local UICOMMMNG_MENU_OFF = 0x88     -- .MenuCommunityBrdIns (UCommunityBoardMenu, 0x128)
local HOST_PNLCURS_OFF = 0x490      -- WL_PnlCurs (UCanvasPanel on the HOST)

-- Pointer stored at absolute addr+off (Mem.ptr only takes UObjects).
local function ptr_at(addr, off)
    local raw = Mem.at_bytes(addr, off, 8)
    if not raw then return nil end
    local p = 0
    for k = 8, 1, -1 do p = p * 256 + raw:byte(k) end
    if p == 0 then return nil end
    return p
end

local function dump_cursor_hunt(frame)
    local lines = {}
    for _, m in ipairs(CURSOR_MEMBERS) do
        local w
        pcall(function() w = frame[m] end)
        if Core.valid(w) then
            local wa = Mem.addr(w)
            scan_addr(m, wa, 0x88, 0x180, lines)                 -- the widget: transform + tail
            local slot = Mem.ptr(w, SLOT_PTR_OFF)                -- follow UWidget.Slot
            scan_addr(m .. ".slot", slot, 0x0, 0x80, lines)      -- the canvas slot's layout
        end
    end
    scan_addr("frame", Mem.addr(frame), 0x3F0, 0x568, lines)     -- frame incl. 0x618..0x868 gap
    local ha = Mem.addr(board)
    if ha then
        scan_addr("host", ha, 0x3F0, 0x2A0, lines)               -- full host incl. 0x4E8..0x510 gap
        scan_addr("input", Mem.ptr(board, HOST_UICMNINPUT_OFF), 0x28, 0x438, lines)
        scan_addr("model", Mem.ptr(board, HOST_TARGETBOARD_OFF), 0x28, 0x110, lines)
        local mng = Mem.ptr(board, HOST_UICOMMMNG_OFF)
        if mng then
            scan_addr("menu", ptr_at(mng, UICOMMMNG_MENU_OFF), 0x28, 0x100, lines)
        end
        local pnl = Mem.ptr(board, HOST_PNLCURS_OFF)
        scan_addr("pnlcurs", pnl, 0x88, 0x180, lines)            -- host cursor canvas
        if pnl then
            scan_addr("pnlcurs.slot", ptr_at(pnl, SLOT_PTR_OFF), 0x0, 0x80, lines)
        end
    end
    if #lines == 0 then return end
    local f = io.open(dump_path(), "a")
    if f then f:write(string.format("[%d] hunt %s\n", os.time(), table.concat(lines, " "))) f:close() end
end

-- DEBUG grid-cursor hunt (2026-07-06): the emblem grid's AnimLoop signal lags a beat
-- behind the input (spoken emblem trails the cursor even after the newest-anim fix),
-- so hunt the NATIVE cursor index in the non-reflected tail gaps (the board's
-- hoveredCache pattern): EmbList 0x398..0x418 (gap 0x3D0..0x3F0 between AnimOutL and
-- PageChangeType is the prime suspect: page + cursor) and the host 0x3A0..0x448 (CXX
-- dump layouts). Appends changed i32s to dumps/dump_community.txt per cursor move.
-- OFF since 2026-07-06 (third pass): fully mapped — EmbList+0x3D0 col, +0x3D4 row,
-- +0x3EC = the complete slot index (native_offsets.commuGrid), wired in grid_update.
local DEBUG_GRID = false
local function grid_hunt()
    local lines = {}
    local el
    pcall(function() el = grid.EmbList end)
    if Core.valid(el) then scan_addr("emblist", Mem.addr(el), 0x398, 0x80, lines) end
    if Core.valid(grid) then scan_addr("gridhost", Mem.addr(grid), 0x3A0, 0xA8, lines) end
    if #lines == 0 then return end
    local f = io.open(dump_path(), "a")
    if f then
        f:write(string.format("[%d] gridhunt %s\n", os.time(), table.concat(lines, " ")))
        f:close()
    end
end

local last_held = false      -- emblem-in-hand edge detector
local last_sub = nil         -- last spoken link-bonus subtitle
local snap_tick = 0          -- DEBUG state-snapshot throttle

-- PLACING-mode hover debounce (state declared up with the other board locals). While
-- an emblem is HELD the game freezes its own hover cache (opus RE 2026-07-16) and the
-- emblem rides a FREE analog cursor, so the hit-test replica returns a jittery
-- socket/gap/socket stream as the stick drifts; announcing every transient with
-- interrupt=true shreds the speech to nothing (user: "no lee" while placing). Only
-- accept a value that holds for HOVER_STABLE ticks; a differing sample is a transient
-- and keeps the last stable one. 0 = over a gap (no socket is index 0). Browse mode
-- is unaffected: it reads the game's already-stable cache, not the replica.
local function debounce_hover(raw)   -- raw: socket index >=1, or 0 for a gap
    raw = raw or 0
    if raw == hov_stable then hov_cand_n = 0; return hov_stable end
    if raw ~= hov_cand then hov_cand, hov_cand_n = raw, 1
    else hov_cand_n = hov_cand_n + 1 end
    if hov_cand_n >= HOVER_STABLE then hov_stable, hov_cand_n = raw, 0 end
    return hov_stable
end

-- Link-bonus voice subtitles (played on placement, board mode 16). The screen
-- registry gives the display to ONE adapter, so the dialogue reader below us never
-- runs while the board is active — the board reads its own subtitles widget
-- (host.LinkBonusSubtitles, UATUISubtitles: TextName + TextSelif, both reflected).
local function board_subtitles()
    local sub
    pcall(function() sub = board.LinkBonusSubtitles end)
    if not Core.valid(sub) or not Core.on_screen(sub) then
        last_sub = nil
        return
    end
    local line = read(sub.TextSelif)
    if not line then return end
    local msg = Core.phrase(read(sub.TextName), line)
    if msg ~= last_sub then
        last_sub = msg
        Speech.say(msg, false)
    end
end

-- DEBUG: periodic one-line state snapshot (board mode, sub-state, cursor, hovered) —
-- the ground truth for the next live pass if anything still reads wrong.
local function dump_state(frame, hovered, pc)
    if tick - snap_tick < 20 then return end
    snap_tick = tick
    local cw
    pcall(function() cw = frame.WL_PanelCursor end)
    local cx = Core.valid(cw) and Mem.float(cw, BOARD.cursorX) or nil
    local cy = Core.valid(cw) and Mem.float(cw, BOARD.cursorY) or nil
    -- instance count guards against reading a STALE pooled board (Start_Char lesson)
    local inst = 0
    pcall(function()
        for _, o in ipairs(Core.cached_all("Start_Commu_Brd_C", tick) or {}) do
            if Core.valid(o) then inst = inst + 1 end
        end
    end)
    local f = io.open(dump_path(), "a")
    if not f then return end
    f:write(string.format("[%d] state host=%x inst=%d mode=%s sub=%s held=%s curs=%s,%s n=%s panels=%d hov=%s cache=%s\n",
        os.time(),
        Mem.addr(board) or 0, inst,
        tostring(Mem.i32(board, BOARD.mode)),
        tostring(Mem.u8(board, BOARD.subState)),
        tostring((Mem.ptr(frame, BOARD.heldEmblem) or 0) ~= 0),
        cx and string.format("%.0f", cx) or "?", cy and string.format("%.0f", cy) or "?",
        tostring(Mem.i32(frame, BOARD.activeCount)),
        (pc and #pc.list) or -1,
        tostring(hovered),
        tostring(Mem.i32(board, BOARD.hoveredCache))))
    f:close()
end

local function board_update()
    local frame
    pcall(function() frame = board.WL_BrdFrame end)
    if not Core.valid(frame) then return end
    if DEBUG then dump_cursor_hunt(frame) end
    local title = board_title()
    local pc = board_panels(frame, title or "?")

    -- Live cursor — ONLY in board mode 7. The board tick (FUN_1414c7de0) runs its
    -- hover tracker exclusively in mode 7; in the tutorial-popup modes (12/13/14/17)
    -- the stick is dead by design and every cursor read is stale (this is why every
    -- earlier live pass saw frozen memory: the community tutorial holds those modes
    -- until its popups are dismissed).
    -- PRIMARY source: the game's own cached hovered index (host+0x5D8, written by
    -- FUN_1414e3170 each tick) — authoritative when >= 1 (0x80000001 = sentinel).
    -- FALLBACK: our replica of its hit test (board_hovered), which also detects
    -- EMPTY sockets (the cache stores -1 over those). The hovered socket is the
    -- announcer's focused NAME, so moves announce and idling stays silent; leaving
    -- every socket announces "free" once so a confirm there is known to open the grid.
    -- Emblem in hand? Computed FIRST: while held the game's hover tracker
    -- FUN_1414e3170 EARLY-OUTS (gate FUN_1414f91e0 = "frame+0x7B8 != 0") and never
    -- writes the cache — host+0x5D8 is FROZEN at its pre-pickup value, so it must be
    -- ignored and only the hit-test replica consulted (opus RE 2026-07-16; the game
    -- itself only hit-tests on the Decide press in placing — there is no per-tick
    -- native cache to read). The board PANS under a FIXED reticle (700,760): the
    -- replica works in both browse and placing because the pan lives in the frame
    -- adjusts it re-reads every call.
    local held = (Mem.ptr(frame, BOARD.heldEmblem) or 0) ~= 0
    local hovered = nil
    if Mem.i32(board, BOARD.mode) == 7 then
        if not held then
            hovered = Mem.i32(board, BOARD.hoveredCache)
        end
        if not hovered or hovered < 1 or hovered > #pc.list then
            hovered = board_hovered(frame, pc)
        end
        -- Placing rides a jittery free cursor → debounce (browse's cache is stable).
        if held then
            local s = debounce_hover((hovered and hovered >= 1) and hovered or 0)
            hovered = s >= 1 and s or nil
        end
    end
    if DEBUG then dump_state(frame, hovered, pc) end
    local n = Mem.i32(frame, BOARD.activeCount) or #pc.list
    if n > #pc.list then n = #pc.list end
    local item = title
    if hovered and pc.list[hovered] then
        item = cached_label(hovered, function()
            return socket_label(pc.list[hovered], hovered, n)
        end)
    elseif last_idx then
        item = I18n.t("board_free")   -- had a socket before, cursor now over open space
    end
    if hovered then last_idx = hovered end

    ann:focus(I18n.header(1), title, item, nil, nil)

    board_subtitles()

    -- An emblem was just picked up (or we returned from the grid carrying one):
    -- explain the placement gesture once. (`held` computed above, before the hover.)
    if held ~= last_held then
        last_held = held
        if held then Speech.say(I18n.t("board_holding"), false) end
    end

    if title and title ~= last_title then
        last_title = title
        panel_cache = nil                    -- socket layout changes with the board
        last_idx, label_cache, label_idx = nil, nil, nil   -- labels too
        -- Board entry summary — kept SHORT (user 2026-07-16: the full readout dumped
        -- all ~10 community skills in one breath, "todo junto"). Overall level/rank +
        -- placed emblems + the action hint only; the community-skills LIST is on demand
        -- via the game's "Y: Detalles" (skills_text() stays for a future dedicated key).
        local bits = {}
        local s = board_summary()
        if s then bits[#bits + 1] = s end
        local placed = placed_emblems(pc)
        if #placed > 0 then
            bits[#bits + 1] = I18n.t("placed") .. " " .. table.concat(placed, ", ")
        end
        bits[#bits + 1] = string.format(I18n.t("board_hint"), n)
        Speech.say(table.concat(bits, ". "), false)
    end
end

function Commu.update()
    -- DEFER the panel/slot readout while a protected instruction/notice is still
    -- playing (guide "select an empty panel, press A" / the reward emblems), so the
    -- board no longer shreds it (user 2026-07-16). When the protected line finishes,
    -- re-announce the CURRENT selection fresh (invalidate) — the readout that got cut
    -- when the instruction first fired must come back, not stay swallowed.
    if Speech.protected() then speech_deferred = true return end
    if speech_deferred then speech_deferred = false; ann:invalidate() end
    if mode == "detail" then
        local name = read(det.Txt_Name)
        if not name then return end
        ann:focus(I18n.header(5), nil, name, nil, detail_text)
    elseif mode == "grid" then
        if DEBUG_GRID then grid_hunt() end
        grid_update()
    elseif mode == "board" then
        board_update()
    end
end

return Commu
