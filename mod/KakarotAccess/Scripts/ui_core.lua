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

-- Per-tick memo for on_screen: adapters and first_on_screen probe the same
-- headers/containers many times per step, and each probe is an ancestor walk of up
-- to MAX_ANCESTORS × 2 pcalls. Keyed by the UObject address (two RemoteObjects may
-- wrap the same object), cleared in begin_scan_tick — never carried across ticks.
local os_memo = {}
local function memo_key(o)
    local ok, a = pcall(function() return o:GetAddress() end)
    if ok and type(a) == "number" then return a end
    return nil
end

function Core.on_screen(o)
    if not Core.is_visible(o) then return false end
    local key = memo_key(o)
    if key and os_memo[key] ~= nil then return os_memo[key] end
    local r = Core.on_screen_uncached(o)
    if key then os_memo[key] = r end
    return r
end

function Core.on_screen_uncached(o)
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

-- FindAllOf, timed: every full-object scan is accounted in _G.__KakarotScanStats so the
-- Ctrl+F5 dump can attribute step cost to scans vs widget walks with data, not guesses.
local function timed_findall(cls_name)
    local t0 = os.clock()
    local r = FindAllOf(cls_name) or {}
    local dt = (os.clock() - t0) * 1000
    local s = _G.__KakarotScanStats
    if not s then s = { n = 0, ms = 0, max = 0 } _G.__KakarotScanStats = s end
    s.n = s.n + 1
    s.ms = s.ms + dt
    if dt > s.max then s.max = dt end
    return r
end

-- First live (runtime, not archetype/CDO) instance of a class. PREFERS the shared
-- top-level widget — a DIRECT child of the GameInstance (…BP_ATGameInstance_C_0.<Name>) —
-- over a nested/pooled copy inside another screen's WidgetTree. This matters for classes
-- with several live instances (e.g. Xcmn_Win01_C exists both as the real message window
-- and as a collapsed copy under a quest screen): the adapters want the top-level one, and
-- caching the nested copy would make the screen never detect as on-screen.
function Core.first_live(cls_name)
    local all = timed_findall(cls_name)
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
local ABSENT_BACKOFF = 40   -- ticks (~4s) between scans for a NEVER-seen class. Was 80
                            -- while ~25 absent classes scanned (30% of the game thread);
                            -- with the screen directory serving most classes by pointer,
                            -- the absent scan set is small and the safety net can be
                            -- fast again. Menu-driven appearances don't wait for this —
                            -- the boost windows (registry commits + pad presses) cover
                            -- them; this cadence is only the no-event fallback.

-- Per-tick FindAllOf budget — the fix for the periodic menu-navigation lag SPIKE
-- (2026-07-04). Each absent class backs off for ABSENT_BACKOFF ticks, but ~20 adapters
-- probe on the SAME poll loop, so their back-offs expire on the SAME tick and that one
-- tick runs 20 full UObject-array scans at once (felt as a stutter every few seconds
-- while navigating). Capping scans per tick spreads that work: deferred classes get no
-- back-off, so they simply retry next tick until a slot frees. This ALSO self-staggers
-- steady state — once served, each class's fresh back-off expires on a different tick.
-- Reset once per poll tick by Core.loop (before the step runs).
local scan_budget = 0
local SCANS_PER_TICK = 2   -- was 3: three FindAllOf in one tick was the measured 153ms
                           -- game-thread spike (Ctrl+F5, 2026-07-14). NOT 1 and NOT
                           -- time-gated: a first cut added a 10ms elapsed-time gate here
                           -- and the late-registry adapters (items/saveload/characters/
                           -- skill tree — they poll at the tail of the sweep, after the
                           -- walk cost) got their scans systematically denied → those
                           -- menus went SILENT (2026-07-15). Budget only; deferred
                           -- classes retry next tick.
-- NOTE: the registry loop is NOT the only caller — battle_monitor and quest_objective run
-- their own loops and call this at their step tops (they must, or they'd inherit a spent
-- budget). All callers are serialized on the game thread, so the shared per-tick state
-- (budget + os_memo) being reset by any of them is harmless — but don't assume "per tick"
-- means "per registry tick" here.
function Core.begin_scan_tick()
    scan_budget = SCANS_PER_TICK
    os_memo = {}
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

-- The directory's root lookups (FindFirstOf) are full-array walks too, so they draw from
-- the same per-tick budget as FindAllOf — one shared cap on scan work per tick.
function Core.take_scan_slot() return scan_allowed() end

-- ---- screen directory (ui_directory.lua): the game's own screen registry -----
-- Mapped classes resolve to their live instance through 2-3 guarded pointer reads on the
-- game's HUD/menu managers — no FindAllOf, no backoff, a just-opened submenu is seen the
-- same tick. Lazily required (ui_directory requires ui_core back, so a top-level require
-- here would cycle). Returns nil for unmapped classes — those keep the scan path.
local Dir = nil
local function directory_list(cls_name)
    if Dir == nil then
        local ok, m = pcall(require, "ui_directory")
        Dir = (ok and m) or false
        if Dir == false then
            print("[KakarotAccess] ui_directory unavailable, scan-only mode\n")
        end
    end
    if not Dir then return nil end
    return Dir.resolve(cls_name)
end

-- Real-time tick clock (100ms units). Backoffs used to run on each ADAPTER's private tick
-- counter, which only advances when that adapter is polled — with the sticky registry
-- (ui_registry) idle adapters are polled less often, so their counters would stretch every
-- backoff. os.clock is wall time under MSVC, the same source the step telemetry uses.
local function now()
    return math.floor(os.clock() * (1000 / Core.POLL_MS))
end

-- Event-driven scan boost: for a short window after a REGISTRY EVENT (a screen was
-- committed or closed — i.e. the exact moments a new screen may be appearing), classes
-- whose pool holds nothing live are due immediately instead of waiting out their backoff.
-- Measured on this game a FindAllOf costs ~65ms (Ctrl+F5 scan stats, 2026-07-15), so the
-- steady-state cadence must stay SLOW — the boost concentrates the scans at user-driven
-- navigation instants, where a submenu must read at once. This can never become the
-- 2026-07-14 per-tick forcing: the window is short, the per-tick budget still applies,
-- and the trigger is a discrete user action, not a poll.
local boost_until = 0
local boost_gen = 0       -- each window is a generation: a class gets ONE boosted scan per
                          -- window (without this, every missing pool re-scanned every tick
                          -- of the window — a 65ms×2 per-tick storm for 1.5s per event)
local boosted_gen = {}    -- cls -> last generation this class was boost-scanned in
local BOOST_TICKS = 15   -- ~1.5s window
function Core.boost_missing(ticks)
    local t = now() + (ticks or BOOST_TICKS)
    if t > boost_until then boost_until = t end
    boost_gen = boost_gen + 1
end

-- Stable per-class jitter (0..15 ticks) added to scan backoffs so classes that went absent
-- together don't all expire on the same tick (the old clustered-expiry spike).
local jitter_cache = {}
local function jitter(cls_name)
    local j = jitter_cache[cls_name]
    if j then return j end
    local h = 0
    for i = 1, #cls_name do h = (h * 31 + cls_name:byte(i)) % 997 end
    j = h % 16
    jitter_cache[cls_name] = j
    return j
end

-- ---- resurrect probe: re-find a recreated screen WITHOUT FindAllOf -----------------
-- A destroyed screen used to wait out DEAD_BACKOFF (~4s) for a full-object scan before it
-- read again — the "~2s to start reading the pause/items/palette" latency. But this game
-- names its top-level menu widgets deterministically under the persistent GameInstance
-- (…GameEngine_0:BP_ATGameInstance_C_0.Start_Top_C_0, per the Ctrl+F5 dump), and a
-- recreated instance just increments the trailing _N. StaticFindObject by full path is a
-- hash lookup (O(1), microseconds) — so for every class we RECORD the path of a live
-- instance once seen, and while the class is dead we probe path_N..path_N+4 every few
-- ticks. A hit resurrects the cache in ~300ms; a miss costs nothing and the FindAllOf
-- backoff remains the safety net (so this is purely additive — worst case = old behavior).
-- Paths are DERIVED from observed instances at runtime, never hardcoded.
local probe_info = {}   -- cls -> { base = ".../BP_ATGameInstance_C_0.Start_Top_C_", idx = 0 }
local probe_next = {}   -- cls -> earliest tick for the next probe attempt
local PROBE_EVERY = 3       -- ticks between probe attempts per dead class (~300ms)
local PROBE_MAX_GAP = 20    -- misses back off exponentially up to this (~2s): a class that
                            -- stays closed must not keep hammering the object tables
local PROBE_RANGE = 4       -- how many name suffixes past the last known one to try
local POST_PROBE_SCAN = 10  -- ticks until a real scan completes the pool after a probe hit
local probe_gap = {}        -- cls -> current miss backoff (nil = PROBE_EVERY)

-- The probe ONLY runs for classes seen alive this session (recorded path). A first cut
-- also probed NEVER-seen classes via constructed GameInstance paths — that hung the game
-- at the logo screen on boot (2026-07-15): every class was unseen, so ~30 classes probed
-- StaticFindObject every 300ms THROUGH the whole initial async load, contending the
-- engine's object tables against the loader until the load never finished. The log
-- showed probes "resurrecting" boot windows seconds into the logo — do not bring
-- constructed-path probing back; never-seen classes wait for the budgeted FindAllOf.

local function remember_path(cls_name, o)
    local ok, fn = pcall(function() return o:GetFullName() end)
    if not ok or type(fn) ~= "string" then return end
    local path = fn:match("%s(/.+)$") or fn
    local base, idx = path:match("^(.-_)(%d+)$")
    if not base then return end
    probe_info[cls_name] = { base = base, idx = tonumber(idx) }
    probe_gap[cls_name] = nil   -- seen alive again: next death starts probing fast
end

-- Record the best (GameInstance-child preferred) live instance of a freshly scanned list.
local function remember_from_list(cls_name, list)
    local fallback
    for _, o in ipairs(list) do
        if Core.valid(o) then
            local ok, fn = pcall(function() return o:GetFullName() end)
            if ok and type(fn) == "string" and fn:match("BP_ATGameInstance_C_%d+%.[%w_]+$") then
                remember_path(cls_name, o)
                return
            end
            fallback = fallback or o
        end
    end
    if fallback then remember_path(cls_name, fallback) end
end

-- PROBE DISABLED (2026-07-15). The boot-gating above was not enough: free-roam streams
-- sublevels asynchronously too, so there is NO window in which StaticFindObject is provably
-- safe on this game — the second freeze (mid-session, no crash, log just stops) matches a
-- game-thread deadlock against the async loader just like the boot hang. Do NOT re-enable by
-- flipping this flag alone; a safe version needs a native-side "is the loader idle" check
-- (mem_bridge) or must move the lookup itself into a native bridge. Menu re-detection falls
-- back to the budgeted FindAllOf at DEAD_BACKOFF (~4s) — yesterday's proven behavior.
local PROBE_ENABLED = false
local probes_armed = false   -- stays false through the whole boot/logo phase: set by the
                             -- first transition (first GameMode). Before that the engine
                             -- is mid-initial-load and probing is off the table entirely.
local function probe_class(cls_name)
    if not PROBE_ENABLED then return nil end
    if not probes_armed then return nil end
    local pi = probe_info[cls_name]
    if not pi then return nil end                 -- never seen alive: no path to probe
    if Transition.active() then return nil end    -- never touch object tables mid-load
    local tries = {}
    for i = pi.idx, pi.idx + PROBE_RANGE do tries[#tries + 1] = pi.base .. i end
    for _, p in ipairs(tries) do
        local o
        pcall(function() o = StaticFindObject(p) end)
        if Core.valid(o) then
            local okc, cn = pcall(function() return o:GetClass():GetFName():ToString() end)
            if okc and cn == cls_name then
                remember_path(cls_name, o)
                print("[KakarotAccess] probe resurrected " .. cls_name .. "\n")
                return o
            end
        end
    end
    return nil
end

-- Probe with its own cadence: PROBE_EVERY between attempts, doubling to PROBE_MAX_GAP on
-- consecutive misses (reset whenever the class is seen alive). The single throttle both
-- cache paths share.
local function try_probe(cls_name, tick)
    if (probe_next[cls_name] or 0) > tick then return nil end
    local o = probe_class(cls_name)
    if o then
        probe_next[cls_name] = tick + PROBE_EVERY
        return o
    end
    local g = math.min((probe_gap[cls_name] or PROBE_EVERY) * 2, PROBE_MAX_GAP)
    probe_gap[cls_name] = g
    probe_next[cls_name] = tick + g
    return nil
end

-- NOTE (2026-07-14): the `tick` parameter is kept for the ~30 adapter call sites but is no
-- longer the clock — backoffs run on the shared real-time tick (now()), so an adapter that
-- is polled less often (sticky registry) doesn't stretch its own re-detection cadence.
function Core.cached_live(cls_name, tick)
    -- Directory fast path: pointer reads, no scans, no backoffs. Prefer the on-screen
    -- candidate — a class can have several live copies (field tips vs pause tips) and
    -- returning a collapsed one would silence the adapter.
    local d = directory_list(cls_name)
    if d then
        for i = 1, #d do
            if Core.on_screen(d[i]) then return d[i] end
        end
        return d[1]
    end
    local c = live_cache[cls_name]
    if Core.valid(c) then return c end
    tick = now()
    -- Cheap resurrect probe, ONLY when the cached ref died IN PLACE (c is an invalidated
    -- object). A nil slot means "never seen" or "cleared by a transition" — in both cases
    -- probing would run while the engine may still be loading (the boot-hang lesson), so
    -- those wait for the budgeted FindAllOf like before.
    if c ~= nil then
        c = try_probe(cls_name, tick)
        if c then
            live_cache[cls_name] = c
            return c
        end
    end
    local boosted = false
    if live_backoff[cls_name] and tick < live_backoff[cls_name] then
        -- Boost window: ONE bypass scan per window generation, same rule as cached_all.
        -- (The first cut bypassed the backoff EVERY tick of the window — with the pad
        -- boost refreshing the window on every press, menu navigation became a
        -- permanent 65ms-scan storm: 57k scans, 1s step spikes, 2026-07-15 dump.)
        if not (tick < boost_until and boosted_gen[cls_name] ~= boost_gen) then
            return nil
        end
        boosted = true
    end
    if not scan_allowed() then return nil end   -- budget spent: defer, retry next tick
                                                -- (boost credit NOT consumed yet)
    if boosted then boosted_gen[cls_name] = boost_gen end
    c = Core.first_live(cls_name)
    live_cache[cls_name] = c
    if c then
        if PROBE_ENABLED then remember_path(cls_name, c) end
    else
        live_backoff[cls_name] = tick + ABSENT_BACKOFF + jitter(cls_name)
    end
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

-- A pool is DEAD when its cached list holds entries but none is valid anymore — the
-- screen was destroyed and its reopen will build a new instance. first_on_screen spots
-- this for free during its normal walk and marks it here, which (a) arms the resurrect
-- probe and (b) pulls the next FindAllOf forward to the DEAD_BACKOFF cadence in case the
-- list was sitting on the slow 30s refresh. An EMPTY list is NOT dead — that's the normal
-- idle state of nearly every menu (the 2026-07-14 lesson) — but it does get the cheap
-- probe, since a constructed-path lookup costs microseconds, not a scan.
local pool_dead = {}
local function mark_pool_dead(cls_name)
    if pool_dead[cls_name] then return end
    pool_dead[cls_name] = true
    local t = now() + DEAD_BACKOFF + jitter(cls_name)
    if not all_next[cls_name] or all_next[cls_name] > t then all_next[cls_name] = t end
end

-- Mark a cached pool due for a re-scan NOW (budget-gated, so it lands within a tick or
-- two). For ALWAYS-ALIVE pools (CFUIMultiLineTextBox: thousands of instances) that idle
-- on the ~30s refresh: a screen that is REBUILT on open (the Items menu — the game NULLs
-- its manager field on close) brings new text boxes the stale pool doesn't have, and an
-- adapter that subtree-scans the pool for them goes active-but-MUTE (2026-07-15). Call
-- on the entry event of such a screen; useless for dead/absent pools (they already
-- re-scan on their own backoff).
function Core.refresh_all(cls_name)
    all_next[cls_name] = 0
end

function Core.cached_all(cls_name, tick)
    -- Directory fast path (see cached_live): the returned list holds every valid live
    -- candidate; an empty list means the game's own manager field is null — the screen
    -- does not exist, and no scan can say otherwise.
    local d = directory_list(cls_name)
    if d then return d end
    tick = now()
    local c = all_cache[cls_name]
    local due = not c or not all_next[cls_name] or tick >= all_next[cls_name]
    -- Boost window: a missing pool (empty or all-invalid) skips its backoff ONCE per
    -- window generation (credit consumed only when the scan actually runs, so a budget
    -- denial retries next tick). any_valid exits at the first live entry, so alive pools
    -- pay one IsValid here at most.
    local boost_due = not due and tick < boost_until
        and boosted_gen[cls_name] ~= boost_gen and not any_valid(c)
    if due or boost_due then
        -- Honour the per-tick scan budget (see Core.cached_live): if it's spent, reuse
        -- the stale list (or {} on first sight) and try the refresh on a later tick.
        -- Callers already validity/visibility-check every entry, so stale is harmless.
        if scan_allowed() then
            if boost_due then boosted_gen[cls_name] = boost_gen end
            c = timed_findall(cls_name)
            all_cache[cls_name] = c
            -- Alive pool → slow idle refresh. Nothing live: a DESTROYED pool (has entries,
            -- all invalid) re-detects on the fast ~4s cadence; a NEVER-seen class (empty)
            -- on the slow ~8s one — its appearance is normally caught by the boost window,
            -- and its steady scans are the main free-roam cost (65ms each). Jitter keeps
            -- simultaneous expiries from clustering. No per-tick forcing anywhere.
            local alive = any_valid(c)
            pool_dead[cls_name] = not alive and #c > 0 or nil
            -- remember_from_list walks the whole list calling GetFullName (it can be a
            -- hundreds-strong pool like CFUIMultiLineTextBox) — only pay it if the probe
            -- that consumes the recorded paths is actually on.
            if alive and PROBE_ENABLED then remember_from_list(cls_name, c) end
            all_next[cls_name] = tick + (alive and REFRESH_EVERY
                or ((#c > 0 and DEAD_BACKOFF or ABSENT_BACKOFF) + jitter(cls_name)))
            return c
        end
        c = c or {}
        all_cache[cls_name] = c
    end
    -- Probe ONLY a pool marked dead — i.e. its instances were watched on-screen and went
    -- invalid IN PLACE while the world was alive (a destroyed menu, the user's reopen
    -- case). An EMPTY list is never probed: that's "never seen" or "cleared by a
    -- transition", both states where the engine may still be async-loading and probing
    -- the object tables is what hung the boot (2026-07-15). Transitions clear pool_dead.
    if pool_dead[cls_name] then
        local o = try_probe(cls_name, tick)
        if o then
            c = { o }
            all_cache[cls_name] = c
            pool_dead[cls_name] = nil
            all_next[cls_name] = tick + POST_PROBE_SCAN
        end
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
    pool_dead, probe_next = {}, {}
    probes_armed = true   -- a map load happened: from here on, in-place deaths may probe
    -- probe_info / gi_prefix survive on purpose: GameInstance-child menus persist across
    -- maps, so their recorded paths let the probe rebuild the caches without scans right
    -- after a load. Per-level classes just miss and re-record on their next sighting.
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
-- The minimap is the game's own "free-roaming" signal: it is hidden the moment ANY
-- real menu/battle/cutscene owns the screen and back the moment control returns (the
-- screen_map lesson, re-used by nav_tracker's world gate). Adapters whose pooled
-- widgets never collapse on close (the cook-NPC flow keeps the cooking pane visible
-- with its last dish) gate on this: minimap up = that menu is genuinely CLOSED.
function Core.free_roam(tick)
    local mm = Core.cached_live("AT_UIMiniMapRadar", tick)
    return Core.valid(mm) and Core.on_screen(mm)
end

-- A pooled pane that is GENUINELY live, not parked (the cooking-latch rule,
-- CLAUDE.md §8): ESlateVisibility Visible(0) — a parked pooled widget keeps rendering
-- under another state — AND RenderOpacity > ~0 (close animations fade to 0 while the
-- visibility flags lag). Both pcall-guarded: an unreadable signal counts as live.
function Core.pane_live(h)
    local ok, v = pcall(function() return h:GetVisibility() end)
    if ok and tonumber(v) ~= nil and tonumber(v) ~= 0 then return false end
    local ok2, op = pcall(function() return h:GetRenderOpacity() end)
    if ok2 and type(op) == "number" and op < 0.05 then return false end
    return true
end

-- The overworld pause RING (Start_Top_C), GENUINELY open: rendered AND
-- ESlateVisibility Visible (0) — the pooled widget lingers on_screen in other
-- visibility states while closed, so on_screen alone over-triggers (screen_field's
-- live_ring test). Returns the open instance or nil. Adapters whose pooled panes
-- linger visible (screen_cooking/screen_shoplist) yield on this: ring open = a real
-- menu owns the screen.
function Core.ring_open(tick)
    for _, o in ipairs(Core.cached_all("Start_Top_C", tick)) do
        if Core.on_screen(o) then
            local ok, v = pcall(function() return o:GetVisibility() end)
            if ok and tonumber(v) == 0 then return o end
        end
    end
    return nil
end

function Core.first_on_screen(cls_name, tick)
    local list = Core.cached_all(cls_name, tick)
    local saw_valid = false
    for _, o in ipairs(list) do
        if Core.valid(o) then
            saw_valid = true
            if Core.on_screen(o) then return o end
        end
    end
    -- Nonempty list with nothing valid = the screen was destroyed: arm the resurrect
    -- probe and the DEAD_BACKOFF scan (no extra reflection — the walk above already paid).
    if not saw_valid and #list > 0 then mark_pool_dead(cls_name) end
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
