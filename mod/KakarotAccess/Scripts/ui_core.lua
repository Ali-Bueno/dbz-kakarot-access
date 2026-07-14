-- UI core: the reusable substrate for the hybrid menu architecture
-- (see reference/ui-accessibility/generic-strategy.md).
--
-- Provides, with no per-screen knowledge:
--   * widget helpers (valid / is_visible / text_of / first_live / phrase)
--   * an Announcer (the diff-gated speaking rules from menus.md), shared by every
--     screen adapter so the "what to say when name/value/tab/tooltip changes"
--     logic lives in ONE place
--   * the single poll engine (LoopAsync + generation guard) the registry drives
--
-- This game exposes no reliable focus events (RegisterHook is fragile, menus have
-- no selection index), so "subscription" is a shared diff-gated poll — the
-- polling-fallback path the strategy doc allows.

local Speech = require("speech")
local Transition = require("transition")
local Mem = require("mem")   -- only for Mem.thread_id (stamping the game thread, see begin_scan_tick)

local Core = {}

Core.POLL_MS = 100
local TOOLTIP_WINDOW = 6   -- ticks to keep polling for a late-arriving tooltip

-- ---- widget helpers --------------------------------------------------------

function Core.valid(o) return o ~= nil and o:IsValid() == true end

function Core.is_visible(o)
    if not Core.valid(o) then return false end
    local ok, v = pcall(function() return o:IsVisible() end)
    return ok and v == true
end

-- ESlateVisibility values whose subtree is NOT rendered: an ancestor set to either
-- hides everything below it, even though each child's own IsVisible() still returns
-- true. (1 = Collapsed, 2 = Hidden.)
local NOT_RENDERED = { [1] = true, [2] = true }

-- Robust "actually on screen" test. IsVisible() reflects only a widget's OWN slate
-- visibility, so in this game a POOLED/closed window (collapsed at some ancestor) keeps
-- reporting its children as visible — stale "Yes/No", "Saving…", difficulty prompts, etc.
-- look on-screen and make an adapter latch on, shadowing the screen that's really up.
-- So on top of IsVisible() we walk the parent chain and reject if any ancestor is
-- Collapsed/Hidden. Cheap: a handful of GetParent()/GetVisibility() calls, only for the
-- few widgets an adapter probes per tick.
-- Widget-ancestor walk depth. 8 could stop short of a collapsed ancestor on the deeper
-- trees (a stale child then reads as on-screen); 24 covers any real UI depth here.
local MAX_ANCESTORS = 24

-- UClass UserWidget (cached): IsInViewport below may only be called on UserWidgets —
-- calling a member a class doesn't have is the uncatchable C++ abort.
local userwidget_cls = nil
local function is_userwidget(o)
    if userwidget_cls == nil then
        local ok, c = pcall(function() return StaticFindObject("/Script/UMG.UserWidget") end)
        userwidget_cls = (ok and c) or false
    end
    if not userwidget_cls then return false end
    local ok, r = pcall(function() return o:IsA(userwidget_cls) end)
    return ok and r == true
end

function Core.on_screen(o)
    if not Core.is_visible(o) then return false end
    local cur, depth = o, 0
    while depth < MAX_ANCESTORS do
        local okp, p = pcall(function() return cur:GetParent() end)
        if not okp or not Core.valid(p) then break end
        local oke, e = pcall(function() return p:GetVisibility() end)
        if oke and NOT_RENDERED[tonumber(e) or -1] then return false end
        cur, depth = p, depth + 1
    end
    -- The walk topped out (no parent): if the top is a ROOT UserWidget it must also be
    -- IN the viewport. This game closes some screens by REMOVING the root from the
    -- viewport while its own slate enum stays Visible — a stale Shop_Cook_C then kept
    -- every on_screen gate true and silently shadowed the ring pause (Ctrl+F5
    -- adapter_index=17, 2026-07-06). IsInViewport is called ONLY on UserWidgets and is
    -- proven safe here (discover.lua's inVP column uses it on every container).
    if depth < MAX_ANCESTORS and is_userwidget(cur) then
        local okv, invp = pcall(function() return cur:IsInViewport() end)
        if okv and invp == false then return false end
    end
    return true
end

-- Text out of an Xcmn_MultiLineText_C node (its inner CFUIMultiLineTextBox is
-- `mainTxt`). Empty string counts as nil (recycled rows keep stale hidden text).
function Core.text_of(node)
    if not Core.valid(node) then return nil end
    local m = node.mainTxt
    if not Core.valid(m) then return nil end
    local ok, s = pcall(function() return m.Text:ToString() end)
    if ok and s and s ~= "" then return s end
    return nil
end

-- Robust text read: the plain mainTxt (Core.text_of), else the reflected GetText() on the
-- node itself — some CFUIXcmnMultiLineText nodes render their value only through the parent
-- FText, not through the inner mainTxt box. Returns nil if both are empty/unreadable.
function Core.read_text(node)
    local t = Core.text_of(node)
    if t then return t end
    if not Core.valid(node) then return nil end
    local ok, s = pcall(function() return node:GetText():ToString() end)
    if ok and s and s ~= "" then return s end
    return nil
end

-- Guarded TArray access: returns the array and its length, or nil, nil.
--
-- `owner[name]` does NOT return nil when things are wrong — UE4SS hands back an INVALID
-- RemoteObject (api-reference §1), and `arr:GetArrayNum()` on that is not a Lua error: UE4SS
-- raises a C++ exception that pcall CANNOT catch and that takes the process down (0xe06d7363).
-- That is exactly how the skill palette killed the game on 2026-07-14, once per tick until the
-- throw escaped. So `arr ~= nil` is NOT a validity check — the array (a RemoteObject, so it has
-- IsValid) and its owner must both be checked before the call. Every TArray read goes through
-- here; never call GetArrayNum directly.
function Core.array_of(owner, name)
    if not Core.valid(owner) then return nil, nil end
    local arr
    if not pcall(function() arr = owner[name] end) or not Core.valid(arr) then return nil, nil end
    local n
    if not pcall(function() n = arr:GetArrayNum() end) or type(n) ~= "number" or n < 0 then
        return nil, nil
    end
    return arr, n
end

-- First live (runtime, not archetype/CDO) instance of a class. PREFERS the shared
-- top-level widget — a DIRECT child of the GameInstance (…BP_ATGameInstance_C_0.<Name>) —
-- over a nested/pooled copy inside another screen's WidgetTree. This matters for classes
-- with several live instances (e.g. Xcmn_Win01_C exists both as the real message window
-- and as a collapsed copy under a quest screen): the adapters want the top-level one, and
-- caching the nested copy would make the screen never detect as on-screen.
function Core.first_live(cls_name)
    local all = FindAllOf(cls_name) or {}
    local fallback
    for _, o in pairs(all) do
        if Core.valid(o) then
            local fn = o:GetFullName()
            if fn:find("/Engine/Transient", 1, true) then
                if fn:match("BP_ATGameInstance_C_%d+%.[%w_]+$") then return o end
                fallback = fallback or o
            end
        end
    end
    return fallback
end

-- Persistent live-instance cache — the PERFORMANCE-critical path for screen detection.
--
-- FindAllOf scans EVERY UObject (tens of thousands), so calling it every few ticks in
-- each adapter (to re-detect a screen) stalls the game thread → input lag. But these
-- container widgets are POOLED: once created they persist for the session (closing a
-- screen only Collapses it, never destroys it). So we find each class ONCE and keep the
-- reference; per-tick detection becomes a cheap on_screen(cached) check. We only re-scan
-- when the cached ref is gone, and when a class isn't present yet we back off (a screen
-- that hasn't appeared shouldn't cost a full scan every tick).
local live_cache, live_backoff = {}, {}
local ABSENT_BACKOFF = 40   -- ticks (~4s at 100ms) between scans for a not-yet-present class
                            -- (raised from 20: the periodic absent-class FindAllOf scans
                            -- were the main source of gameplay lag spikes, 2026-07-03)

-- Per-tick FindAllOf budget — the fix for the periodic menu-navigation lag SPIKE
-- (2026-07-04). Each absent class backs off for ABSENT_BACKOFF ticks, but ~20 adapters
-- probe on the SAME poll loop, so their back-offs expire on the SAME tick and that one
-- tick runs 20 full UObject-array scans at once (felt as a stutter every few seconds
-- while navigating). Capping scans per tick spreads that work: deferred classes get no
-- back-off, so they simply retry next tick until a slot frees. This ALSO self-staggers
-- steady state — once served, each class's fresh back-off expires on a different tick.
-- Reset once per poll tick by Core.loop (before the step runs).
local scan_budget = 0
local SCANS_PER_TICK = 3
function Core.begin_scan_tick()
    scan_budget = SCANS_PER_TICK
    -- Stamp the game thread's id once (this runs inside ExecuteInGameThread, so it IS the game
    -- thread). It is the reference the remaining NotifyOnNewObject — the transition gate — checks
    -- itself against, so we can never again ASSUME a callback is on the right thread.
    if _G.__KakarotGameTid == nil then _G.__KakarotGameTid = Mem.thread_id() end
end
local function scan_allowed()
    if scan_budget <= 0 then return false end
    scan_budget = scan_budget - 1
    return true
end

function Core.cached_live(cls_name, tick)
    local c = live_cache[cls_name]
    if Core.valid(c) then return c end
    if tick and live_backoff[cls_name] and tick < live_backoff[cls_name] then return nil end
    if not scan_allowed() then return nil end   -- budget spent: defer, retry next tick (no back-off)
    c = Core.first_live(cls_name)
    live_cache[cls_name] = c
    if not c and tick then live_backoff[cls_name] = tick + ABSENT_BACKOFF end
    return c
end

-- Persistent FindAllOf-LIST cache. FindAllOf(cls) scans EVERY UObject on each call, so an
-- adapter that iterates a class every tick (dialog choices, tutorial text boxes, list
-- items) pays a full-array scan every 100ms → game-thread stall. These widget POOLS are
-- essentially static, so we cache the list and only re-scan every REFRESH_EVERY ticks to
-- pick up any newly-created pooled instances. Callers still validity/visibility-check each
-- returned widget, so stale entries are harmless. Returns the cached Lua array.
local all_cache, all_next = {}, {}
local REFRESH_EVERY = 300  -- ticks (~30s) between re-scans of a class whose cached pool is
                           -- ALIVE. A screen that merely CLOSED keeps its (collapsed)
                           -- instances, so for the steady state this can stay slow and cheap.
local DEAD_BACKOFF = 40    -- ticks (~4s) between scans for a class whose cached list holds NO
                           -- live instance — a menu not opened yet, OR one destroyed on close.
                           -- This is the whole re-detection mechanism now (the event feed is
                           -- gone): a destroyed screen is re-found within ~4s of its recreation,
                           -- and a class costs a scan only every ~4s, never per tick. It MUST be
                           -- a plain fixed backoff: an earlier cut gave freshly-dead classes a
                           -- 1s "fast lane" and forced a rescan the very next tick, and as a play
                           -- session accumulated closed screens they all demanded scans at once,
                           -- saturated the 3-per-tick budget, and starved live detection — the
                           -- whole reader went sluggish the longer you played (2026-07-14).

-- Does this cached list still hold anything live? Only ever called right after a scan (a screen
-- class has a handful of pooled instances), never per tick over a long list.
local function any_valid(list)
    for i = 1, #list do
        if Core.valid(list[i]) then return true end
    end
    return false
end

function Core.cached_all(cls_name, tick)
    local c = all_cache[cls_name]
    if not c or (tick and (not all_next[cls_name] or tick >= all_next[cls_name])) then
        -- Honour the per-tick scan budget (see Core.cached_live): if it's spent, reuse
        -- the stale list (or {} on first sight) and try the refresh on a later tick.
        -- Callers already validity/visibility-check every entry, so stale is harmless.
        if not scan_allowed() then return c or {} end
        c = FindAllOf(cls_name) or {}
        all_cache[cls_name] = c
        -- Alive pool → slow idle refresh; nothing live (absent or destroyed) → the ~4s
        -- re-detection cadence. One fixed cadence for both: no per-tick forcing anywhere,
        -- so a session full of closed screens can never saturate the scan budget.
        if tick then all_next[cls_name] = tick + (any_valid(c) and REFRESH_EVERY or DEAD_BACKOFF) end
    end
    return c
end

-- REMOVED (2026-07-14): the event-driven cache feed (NotifyOnNewObject → stash → drain).
-- DO NOT BRING IT BACK IN LUA. It is what crashed the game, twice.
--
-- The idea was sound — a widget-construction notify fed the caches, so a reopened screen was
-- detected within one tick with no FindAllOf. The flaw is not in the design but in WHERE the
-- callback runs: UE4SS delivers it on the engine's ASYNC LOADING thread as well as the game
-- thread. MEASURED, not assumed (mem_bridge.thread_id, 2026-07-14):
--     widget notify thread(s): 5744 (FOREIGN!), 38620 (game thread) | game thread: 38620
-- Any Lua on that foreign thread — even the two table writes the stash did — runs the SAME
-- lua_State concurrently with the poll step: the allocator and the incremental GC race, and a
-- userdata the game thread still holds gets freed/overwritten. The symptom was a cached widget
-- that passed IsValid() and then reported a NULL UObject on the very next member call:
--     Error: Tried calling a member function but the UObject instance is nullptr
--     [C]: in method 'GetArrayNum' → screen_skillcustom.lua:92
-- repeated every tick (the poisoned entry was inserted at the HEAD of the cached list, so it
-- shadowed the real screen forever) until UE4SS's uncatchable C++ throw killed the process
-- (0xe06d7363). The earlier AV at a garbage address was the same corruption, one draw earlier.
--
-- Two dead ends, so nobody retries them: wrapping the callback body in ExecuteInGameThread does
-- NOT help (the wrapper itself is Lua, already running on the foreign thread), and neither does
-- moving the stash into a mutex-protected C bridge (calling into C still executes Lua bytecode
-- to get there). A safe event feed would have to be armed from a native UE4SS C++ mod, outside
-- this lua_State — that is the only door left if the scan net below ever proves too slow.

-- Drop every cached widget reference. Run at each map switch (transition.lua): some
-- cached widgets are per-level (the field HUD family), and probing a freed one after
-- the level died — even just IsValid — is an uncatchable C++ abort. Pure Lua.
Transition.on_begin("ui_core", function()
    live_cache, live_backoff = {}, {}
    all_cache, all_next = {}, {}
end)

-- First currently on-screen instance of a class, or nil. Use this instead of cached_live
-- when a class has SEVERAL pooled instances and the active (visible) one ALTERNATES between
-- them (e.g. Start_Char_C exists as _3 and _4; only one is on_screen at a time). cached_live
-- would lock onto one and go silent whenever the OTHER is the live one; this picks the live
-- one each tick from the cached list (still cheap — the list is cached, not re-scanned).
--
-- Re-detection is driven entirely by cached_all's backoff: a destroyed screen leaves its list
-- with no live instance, so cached_all re-scans it on the ~4s DEAD_BACKOFF cadence and picks up
-- the recreated one. This just walks the current list and returns the first on-screen instance.
-- It does NOT force any re-scan — an earlier version did (all_next=0 for a dead-but-nonempty
-- list) and that per-tick forcing, multiplied across every screen closed during a play session,
-- saturated the scan budget and made the whole reader slow (2026-07-14).
function Core.first_on_screen(cls_name, tick)
    for _, o in ipairs(Core.cached_all(cls_name, tick)) do
        if Core.valid(o) and Core.on_screen(o) then return o end
    end
    return nil
end

-- ---- where a widget sits on screen -----------------------------------------
-- Widget NUMBERS are slot ids, not places: this game lays its rows/columns out itself
-- (the keyhelp bar is a CanvasPanel the game positions; the status page's stat blocks are
-- pooled instances found in construction order), so reading them in index order can
-- announce them in an order matching nothing on screen. Ask the widget where it IS.
--
-- Guarded on the slot's real class: calling a member a UObject does NOT have is an
-- UNCATCHABLE abort on this game, so GetPosition() is only ever called on a CanvasPanelSlot.
local function slot_of(w)
    if not Core.valid(w) then return nil, nil end
    local s, cn
    pcall(function()
        s = w.Slot
        if s and s:IsValid() then cn = s:GetClass():GetFName():ToString() end
    end)
    if not cn then return nil, nil end
    return s, cn
end

-- One coordinate ("X" or "Y") of a widget, from every source the engine exposes, most
-- authoritative first. The canvas slot's LayoutData offsets read back as 0.0 on the keyhelp
-- bar (dump_keyhelp 2026-07-14) — that row is not laid out through them — so GetPosition()
-- and the render transform are tried too.
local LAYOUT_EDGE = { X = "Left", Y = "Top" }
local function widget_axis(w, axis)
    local s, cn = slot_of(w)
    if s and cn == "CanvasPanelSlot" then
        local v
        pcall(function()
            local p = s:GetPosition()
            if p then v = p[axis] end
        end)
        if type(v) == "number" and v ~= 0 then return v end
        v = nil
        pcall(function() v = s.LayoutData.Offsets[LAYOUT_EDGE[axis]] end)
        if type(v) == "number" and v ~= 0 then return v end
    end
    local rt
    pcall(function() rt = w.RenderTransform.Translation[axis] end)
    if type(rt) == "number" and rt ~= 0 then return rt end
    return nil
end

-- Walk up until some ancestor knows where it is (bounded): on this UI a leaf often sits at 0
-- inside a per-entry container that carries the real placement. nil if nothing knows — the
-- caller then keeps its own (at least stable) order.
local POS_ANCESTORS = 4
Core.slot_of = slot_of   -- exposed for the keyhelp dump (it reports the raw sources)
function Core.slot_pos(w, axis)
    local cur, depth = w, 0
    while Core.valid(cur) and depth < POS_ANCESTORS do
        local v = widget_axis(cur, axis)
        if v then return v end
        local p
        pcall(function() p = cur:GetParent() end)
        cur, depth = p, depth + 1
    end
    return nil
end

-- Join non-empty parts with commas, skipping nils in ANY position.
function Core.phrase(...)
    local n = select("#", ...)
    local parts = {}
    for i = 1, n do
        local p = select(i, ...)
        if p and p ~= "" then parts[#parts + 1] = p end
    end
    return table.concat(parts, ", ")
end

-- ---- Announcer: the shared diff-gated speaking rules ------------------------
-- An adapter calls announcer:focus(tab, name, value, tooltip_fn) every tick with
-- the currently focused control. tooltip_fn is a lazy getter (called only when a
-- tooltip is actually needed, so screens without one pass nil and screens with a
-- costly lookup don't pay it every tick). Rules (menus.md):
--   enter / name change -> name (+ value) (+ tooltip), interrupt
--   tab change          -> tab name only
--   value change        -> only the new value
--   tooltip arriving late-> spoken on its own, queued

local Announcer = {}
Announcer.__index = Announcer

function Core.make_announcer()
    return setmetatable({ t = 0, open = false }, Announcer)
end

function Announcer:reset()
    self.open = false
    self.tab, self.name, self.value, self.tip = nil, nil, nil, nil
    self.pending = false
end

-- Force the next focus() to treat the focused name as changed (re-announce name +
-- value + tooltip, without the screen prefix). For screens whose SELECTION can move
-- while the readable name text stays the same — e.g. a stale list index with a live
-- detail pane (screen_cooking).
function Announcer:invalidate()
    if self.open then self.name = {} end   -- sentinel: never equal to any string
end

-- screen = the menu's own name, spoken ONLY when entering the screen (context
-- change), before the focused item. tab = the current tab/section, spoken on tab
-- change. Either may be nil.
function Announcer:focus(screen, tab, name, value, tooltip_fn)
    self.t = self.t + 1
    local function tip() return tooltip_fn and tooltip_fn() or nil end

    if not self.open then
        self.open = true
        self.tab, self.name, self.value = tab, name, value
        local tt = tip()
        Speech.say(Core.phrase(screen, name, value, tt), true)
        self.tip = tt
        self.pending = (tooltip_fn ~= nil and tt == nil)
        self.deadline = self.t + TOOLTIP_WINDOW
        return
    end

    if tab ~= self.tab then
        self.tab, self.name, self.value = tab, name, value
        if tab then Speech.say(tab, true) end
        self.pending = false
    elseif name ~= self.name then
        self.name, self.value = name, value
        local tt = tip()
        Speech.say(Core.phrase(name, value, tt), true)
        self.tip = tt
        self.pending = (tooltip_fn ~= nil and tt == nil)
        self.deadline = self.t + TOOLTIP_WINDOW
    elseif value ~= self.value then
        self.value = value
        if value then Speech.say(value, true) end
    elseif self.pending then
        if self.t > self.deadline then
            self.pending = false
        else
            local tt = tip()
            if tt then Speech.say(tt, false); self.pending = false end
        end
    end
end

-- ---- poll engine -----------------------------------------------------------
-- One loop for the whole UI. A generation guard stops any loop left over from a
-- prior load (so a reload never leaves two loops racing on a stale Lua state).
--
-- BUSY GUARD: LoopAsync fires every POLL_MS on a worker thread and queues `step` onto
-- the game thread WITHOUT waiting for it. When the game thread is saturated (loading,
-- a fullscreen movie, etc.), those queued steps pile up into a backlog that then runs
-- late and in bursts — felt as long delays when navigating a menu. So we only queue a
-- new step once the previous one has finished, keeping the reader on the CURRENT state.
function Core.loop(step, should_run)
    _G.__KakarotUiGen = (_G.__KakarotUiGen or 0) + 1
    local myGen = _G.__KakarotUiGen
    local busy = false
    LoopAsync(Core.POLL_MS, function()
        if _G.__KakarotUiGen ~= myGen then return true end
        -- Stop polling when the owner disables the reader (Registry.stop → Ctrl+M
        -- "reader off"): returning true ends this LoopAsync. Registry.start re-arms
        -- it with a fresh Core.loop. Without this the loop kept announcing after off.
        if should_run and not should_run() then return true end
        if not busy then
            busy = true
            ExecuteInGameThread(function()
                -- Clear the queue guard on ENTRY, not exit: some engine errors on this
                -- game are C++ exceptions pcall cannot catch — they kill this callback
                -- mid-flight, and a still-true `busy` would silence the loop for the
                -- whole session (seen live 2026-07-04 with the radar menu). Clearing
                -- here keeps the anti-pile-up purpose (the game thread runs this
                -- atomically, so at most one extra step queues while we run).
                busy = false
                -- One FindAllOf budget per poll tick (see Core.cached_live) — bounds the
                -- per-tick scan cost so simultaneous back-off expiries can't spike.
                Core.begin_scan_tick()
                -- Step timing telemetry (read via Ctrl+F5's nav dump): the max/avg
                -- game-thread cost of one reader tick, to pin lag spikes with data.
                local t0 = os.clock()
                local ok, err = pcall(step)
                local dt = (os.clock() - t0) * 1000
                local st = _G.__KakarotStepStats
                if not st then st = { max = 0, n = 0, sum = 0 } _G.__KakarotStepStats = st end
                if dt > st.max then st.max = dt end
                st.n = st.n + 1
                st.sum = st.sum + dt
                if not ok then print("[KakarotAccess] UI step error: " .. tostring(err) .. "\n") end
            end)
        end
        return false
    end)
end

return Core
