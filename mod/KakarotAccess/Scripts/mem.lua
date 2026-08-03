-- Native memory reader: the single place that reads non-UPROPERTY C++ members the
-- UE reflection (and therefore UE4SS Lua) can't see. Used ONLY for the handful of
-- menu selection indices that aren't reflected (field-menu ring, battle pause).
--
-- Transport: mem_bridge.dll — an in-process Lua C module (statically links Lua 5.4.4,
-- same ABI trick as prism_bridge) that does GUARDED reads (SEH: a bad address returns
-- nil, never crashes the game). It holds NO mod logic; all behaviour stays in Lua.
-- DELIBERATELY SEPARATE from prism_bridge.dll (that one is the screen reader only).
--
-- Offsets live in native_offsets.lua (data), recovered via Ghidra (see
-- code/hybrid-mod-feasibility.md). A game patch => re-run Ghidra, edit that file.

local Mem = {}

local m = nil
local loaded = false

function Mem.init()
    local ok, mod = pcall(require, "mem_bridge")
    if ok and mod and mod.read_i32 then
        m = mod
        loaded = true
        print("[KakarotAccess] mem_bridge loaded (native reads available)\n")
    else
        loaded = false
        print("[KakarotAccess] mem_bridge FAILED to load: " .. tostring(mod) .. "\n")
    end
    return loaded
end

function Mem.is_loaded() return loaded end

-- The OS thread this call runs on. Used to answer a question UE4SS does not document: whether
-- a NotifyOnNewObject callback is delivered on the GAME thread or on the engine's async loading
-- thread (Lua on a foreign thread races the poll loop on the same lua_State — the suspect for
-- the intermittent AV inside UE4SS.dll). See the widget feed in ui_core.lua.
function Mem.thread_id()
    if loaded and m.thread_id then return m.thread_id() end
    return nil
end

-- ---- crash black box ---------------------------------------------------------------------
--
-- The mod's crashes kill the process from inside UE4SS with an uncatchable C++ throw: no Lua
-- traceback, and the UE crash dump only gives offsets into a stripped 19 MB DLL. So every round
-- of diagnosis has been inference from code reading. This records what the mod was DOING, in a
-- memory-mapped ring the OS writes out when the process dies, so the next crash names its own
-- site. See the block comment in mem_bridge.c for the mechanism and its honest limits.
--
-- `Mem.mark` must stay free enough to call on hot paths (it is one memcpy) and must never be
-- able to throw: the whole point is a diagnostic that cannot become a new failure mode.
local marking = false

-- Opens the ring and returns the PREVIOUS session's trail (oldest -> newest) before resetting
-- it. If the last run crashed, this is its final few hundred milliseconds.
function Mem.mark_open(path)
    if not (loaded and m.mark_open) then return nil end
    local ok, trail = pcall(m.mark_open, path)
    if not ok then return nil end
    marking = trail ~= nil
    return trail
end

function Mem.mark(text)
    if not marking then return end
    pcall(m.mark, text)
end

function Mem.marking() return marking end

-- The STORED pointer behind a handle, WITHOUT dereferencing it — the only question that is
-- safe to ask about a possibly-freed UObject. `Mem.addr` below cannot be used for that: it
-- calls `obj:IsValid()` first, and IsValid is ITSELF a dereference — UE4SS evaluates
-- `!obj->IsUnreachable()` (a read of the object's flags) BEFORE consulting its own set of
-- known objects (RE-UE4SS `UE4SS/include/LuaType/LuaUObject.hpp:610`), so on a freed pointer
-- the access violation happens inside the very call that was supposed to detect it. That is
-- why the mod's guards kept failing: the safe check sat behind the unsafe one.
-- `GetAddress()` is the one UObject method that touches nothing (same header) — everything
-- else in the Lua API derefs. Returns nil when the handle cannot answer at all, which callers
-- must treat as "unknown", never as "dead" (see Core.alive).
function Mem.raw_addr(obj)
    if not obj then return nil end
    local ok, a = pcall(function() return obj:GetAddress() end)
    if ok and type(a) == "number" then return a end
    return nil
end

-- ---- the memory pre-check ---------------------------------------------------------------
--
-- The guard that runs BEFORE UE4SS dereferences anything. Source-verified against RE-UE4SS
-- v3.0.1: Lua `UObject:IsValid()` is `m_cpp_object && !m_cpp_object->IsUnreachable() &&
-- is_object_in_global_unreal_object_map(m_cpp_object)` (`LuaType/LuaUObject.hpp:610`), so the
-- only part that could catch a freed handle — the lookup in UE4SS's own object set — sits
-- BEHIND `IsUnreachable()`, which reads the object. On a freed pointer the access violation
-- happens inside the very call meant to detect it. And UE4SS never clears the raw pointer in a
-- Lua handle: its delete listener only erases a hash from a set (`LuaUObject.cpp:59-66`), so a
-- freed object — or one whose ADDRESS a new object recycled, re-inserting the same hash —
-- passes and dies later at `prepare_to_handle` → `GetClassPrivate()` (UObjectBase+0x10: the
-- 0x10 access violations in every user crash report).
--
-- So the check must happen outside UE4SS, in memory we read ourselves under SEH — mem_bridge
-- returns nil for an unreadable address instead of faulting the process. Ask the handle for its
-- stored pointer (no deref), then read the object's class pointer and that class's own class
-- pointer through the bridge. Freed or garbage memory fails one of those reads or yields
-- something that is not a UObject; a live object passes both. Not a liveness PROOF (an address
-- recycled by another live object still reads fine), but it converts the whole dangling/garbage
-- class from "kills the process" into "returns nil", which is the crash users actually hit.
--
-- FAILS OPEN by design (the 2026-07-25 regression rule): no bridge, no derivable offset, or a
-- handle that cannot answer GetAddress ⇒ true, and the old path runs unchanged. A guard that
-- fails closed on a shared substrate silently blanks whole screens.
local CLASS_OFF = nil       -- UObjectBase::ClassPrivate offset — DERIVED at runtime, never guessed
-- The probe needs the bridge loaded AND a live UObject to derive from. A single early attempt
-- would latch "unavailable" for the session and silently disable the pre-check, so it
-- retries — but the retry budget is measured in TIME, not in calls: Mem.alive runs dozens of
-- times per tick from Core.valid, so a per-call budget would burn through inside one or two
-- ticks, which is exactly the boot window where the probe is expected to fail. Retry at most
-- once every RETRY_EVERY_S, give up after DEADLINE_S, and SAY SO — a silent give-up would leave
-- the whole crash fix inert with nothing in the log but the absence of a line nobody looks for.
local class_off_next = 0
local class_off_deadline = nil
local class_off_gave_up = false
local CLASS_OFF_RETRY_EVERY_S = 0.5
local CLASS_OFF_DEADLINE_S = 30.0

-- Derive it by asking the engine for both halves of the answer and matching them: take the
-- object Mem.alive was ALREADY handed, get its class pointer through reflection, and find that
-- exact value in the first bytes of the object. Whatever offset holds it IS ClassPrivate on this
-- build — so an engine or game patch re-derives it instead of trusting a stale constant.
--
-- HANG FIX (crash audit RANK 5, 2026-07-31): the probe used to derive from
-- StaticFindObject("/Script/UMG.UserWidget"), retried every RETRY_EVERY_S for the whole
-- DEADLINE_S — i.e. a speculative object lookup on each of the first sixty attempts, every one
-- of them inside the boot window while the engine is async-loading. That is the exact API and
-- the exact window that deadlocked the game twice; ui_core's PROBE_ENABLED = false records the
-- same verdict (there is NO time on this game when a StaticFindObject is provably safe). The
-- object being checked answers both halves of the question, so no lookup is needed at all.
--
-- `base` is the caller's ALREADY-obtained GetAddress result, deliberately passed in rather than
-- re-read here: GetAddress is the call that PIERCES pcall on a non-UObject handle, so it must
-- happen once, inside Mem.alive's transactional guard. Its having answered an integer is also
-- what makes the GetClass() hop below safe to attempt — the handle is proven UObject-family.
local function class_off(obj, base)
    if CLASS_OFF then return CLASS_OFF end
    if class_off_gave_up or not loaded then return nil end
    local t = os.clock()
    if class_off_deadline == nil then class_off_deadline = t + CLASS_OFF_DEADLINE_S end
    if t < class_off_next then return nil end
    class_off_next = t + CLASS_OFF_RETRY_EVERY_S
    -- Every exit below is a RETRY, never a give-up: only the deadline stops the probe, and it
    -- announces itself when it does.
    local function give_up_check()
        if os.clock() >= class_off_deadline and not class_off_gave_up then
            class_off_gave_up = true
            print("[KakarotAccess] class-pointer offset NOT derivable after "
                .. math.floor(CLASS_OFF_DEADLINE_S) .. "s — MEMORY PRE-CHECK DISABLED\n")
        end
    end
    -- Type-checked before math.tointeger, which RAISES on a nil/non-number: the caller reaches
    -- here even when its GetAddress came back nil, and an ordinary Lua error thrown from inside
    -- the guard's pending window would leave the mark set and disable the pre-check for good.
    if type(base) ~= "number" then give_up_check() return nil end
    local want
    if not pcall(function() want = obj:GetClass():GetAddress() end)
        or type(want) ~= "number" then give_up_check() return nil end
    base, want = math.tointeger(base), math.tointeger(want)
    if base == nil or want == nil or base == 0 or want == 0 then give_up_check() return nil end
    for off = 0, 0x40, 8 do
        local ok, v = pcall(function() return m.read_ptr(base, off) end)
        if ok and v == want then
            CLASS_OFF = off
            print(string.format("[KakarotAccess] UObject class pointer at +0x%X (derived)\n", off))
            return CLASS_OFF
        end
    end
    give_up_check()
    return nil
end

-- ---- the shared engine-object resolver ---------------------------------------------------
--
-- The ONE retry-throttled StaticFindObject in the mod (crash audit RANK 5, 2026-07-31). Two
-- modules needed to resolve an engine object by path and each rolled its own policy, both wrong
-- in opposite directions: this file retried the lookup at the probe cadence for the whole
-- deadline, i.e. dozens of speculative lookups inside the boot async-load window that has
-- deadlocked the game twice; ui_core latched `false` on a SINGLE early miss and thereby dropped
-- the IsInViewport half of on_screen for the rest of the session. Neither failure says anything
-- in a log. So the policy lives here, once, and callers just ask: at most one attempt per
-- FIND_RETRY_EVERY_S, only a SUCCESS is ever cached, and past FIND_DEADLINE_S it stops trying
-- and prints why. A caller must treat nil as "not yet", never as "no".
--
-- `r ~= nil` is not a validity check on this API — UE4SS answers an INVALID RemoteObject, not
-- nil — so the result is confirmed with IsValid. Safe here specifically because a lookup MISS
-- yields a null cpp object, which IsValid tests before it dereferences anything; the handles
-- IsValid faults on are freed ones, which StaticFindObject cannot return.
local FIND_RETRY_EVERY_S = CLASS_OFF_RETRY_EVERY_S   -- same cadence as the offset probe above
local FIND_DEADLINE_S = CLASS_OFF_DEADLINE_S         -- and the same give-up horizon
local find_state = {}                                -- path -> { obj, next, deadline, gave_up }

function Mem.find_object(path)
    if type(path) ~= "string" then return nil end
    local st = find_state[path]
    if st == nil then st = { next = 0 }; find_state[path] = st end
    if st.obj then return st.obj end
    if st.gave_up then return nil end
    local t = os.clock()
    if st.deadline == nil then st.deadline = t + FIND_DEADLINE_S end
    if t < st.next then return nil end
    st.next = t + FIND_RETRY_EVERY_S
    local found
    pcall(function()
        local o = StaticFindObject(path)
        if o ~= nil and o:IsValid() then found = o end
    end)
    if found then st.obj = found return found end
    if os.clock() >= st.deadline then
        st.gave_up = true
        print("[KakarotAccess] object NOT resolvable after "
            .. math.floor(FIND_DEADLINE_S) .. "s: " .. path .. "\n")
    end
    return nil
end

-- Rejections are the interesting signal in both directions: silence means the pre-check never
-- fires (and a screen going quiet is NOT its fault), a rising count means it is catching
-- handles that would have killed the process. One line per REJECT_LOG_EVERY, so it costs
-- nothing in the normal case and still shows up in a user's UE4SS.log.
local rejects = 0
local REJECT_LOG_EVERY = 200

-- SELF-HEALING GUARD. `Mem.raw_addr`'s `GetAddress` can be unwound by a C++ throw that PIERCES
-- pcall, leaving this function dead mid-flight with nothing catchable anywhere. There are TWO
-- causes, and they need OPPOSITE policies:
--
--  (a) A NON-UObject handle. `GetAddress` is overridden only on the UObject family; on anything
--      else UE4SS raises `Call to RemoteObject:GetAddress on polymorphic type is not allowed`.
--      That is a MOD BUG, it fires on every call from the offending site, and it is what the
--      original transactional mark was written for (2026-07-25: one such call site, the array
--      check in Core.array_of, silenced seven menus for a whole session).
--  (b) UE4SS LOSING ITS OWN lua_State. `process_lua_function` throws `The lua state '<ptr>' has
--      no instance inside lua_instances unordered map` (LuaMadeSimple.cpp:872) when the game
--      thread reads `static std::unordered_map lua_instances` (:11 — no mutex, and nothing ever
--      erases from it) while the async thread inserts into it. EVERY `ExecuteInGameThread` call
--      does one such insert (LuaMod.cpp:3057-3091 → make_hook_state → Lua::new_thread →
--      lua_instances.emplace, OUTSIDE the mutex taken at :3080), so this mod generates one per
--      dispatch forever. The miss is a TRANSIENT false negative — the entry is present before,
--      during and after — and it lands on an arbitrary handle, typically a perfectly live one.
--
-- The original policy (one unwound call disables the pre-check for the whole session) is right for
-- (a) and catastrophic for (b). On 2026-08-01 a single (b) turned the mod's only out-of-VM memory
-- guard OFF three minutes into a session; the game then ran unguarded for two hours and died of
-- exactly the dangling-handle class this guard exists to stop, because with Mem.alive neutralised
-- `Core.valid` decays to a bare `IsValid` — which dereferences before its own lookup (see the
-- block comment above). One survivable, already-logged engine hiccup cost the whole session.
--
-- So the mark stays TRANSACTIONAL, but recovery is a STREAK, not a hair trigger: an unwound
-- attempt is cleared, counted, logged, and the pre-check STAYS ON. Only CONSECUTIVE failures —
-- with no completed check in between — disable it. That is the signature of (a) and is
-- unreachable by (b). Any completed check resets the streak.
local guard = _G.__KakarotAliveGuard
if not guard then
    guard = { pending = false, disabled = false, trips = 0, streak = 0 }
    _G.__KakarotAliveGuard = guard
end

-- A repeating bad call site (a) raises on EVERY call and reaches this within one tick of adapter
-- work, so the streak — not its exact length — is the discriminator. Kept small so (a) still
-- degrades quickly; (b) was observed once in a two-hour session, i.e. ~500k dispatches apart, so
-- it cannot chain even one step of this.
local MAX_ALIVE_STREAK = 8

-- MANUAL KILL SWITCH (Ctrl+Shift+G, 2026-07-25). Separate from `guard.disabled`, which is the
-- automatic self-protection. This one exists because the pre-check's failure mode is invisible: if
-- it ever rejects a LIVE object it silences whatever was reading it, with no error and nothing in
-- the log but a rising rejection count that looks exactly like success. The user hit "first dialogue
-- reads, later ones do not" — the shape of a guard that starts refusing — and a count of 401
-- rejections cannot distinguish real dead handles from false positives. One keypress can: turn this
-- off, and if the screen comes back, the pre-check was the cause. State in _G so it survives
-- Ctrl+Shift+R (mem.lua itself is protected from reloads, but the flag should be readable anyway).
if _G.__KakarotPrecheck == nil then _G.__KakarotPrecheck = true end
function Mem.set_precheck(on) _G.__KakarotPrecheck = on and true or false end
function Mem.toggle_precheck()
    _G.__KakarotPrecheck = not _G.__KakarotPrecheck
    return _G.__KakarotPrecheck
end
function Mem.precheck_on() return _G.__KakarotPrecheck end

-- Is this handle safe to hand to UE4SS at all? See the block comment above.
-- CONTRACT: pass only UObject-family handles. TArray/struct wrappers go through Core.valid_ref.
function Mem.alive(obj)
    if obj == nil then return false end
    if not _G.__KakarotPrecheck then return true end   -- Ctrl+Shift+G: off = pre-2026-07-25 behaviour
    if guard.disabled then return true end
    if guard.pending then
        -- The PREVIOUS attempt never reached its `pending = false`, so a throw unwound through it.
        -- Clear the mark and carry on with the pre-check still armed — see the streak rationale
        -- above. Only a run of these with no completed check in between is evidence of a bad
        -- handle rather than the engine's lua_instances race.
        guard.pending = false
        guard.trips = guard.trips + 1
        guard.streak = guard.streak + 1
        if guard.streak >= MAX_ALIVE_STREAK then
            guard.disabled = true
            print(string.format("[KakarotAccess] memory pre-check DISABLED after %d CONSECUTIVE "
                .. "unwound attempts (%d this session): a non-UObject handle is reaching "
                .. "Mem.alive on every call. Reads continue unguarded; find the caller.\n",
                guard.streak, guard.trips))
            return true
        end
        print(string.format("[KakarotAccess] memory pre-check: attempt %d unwound by an "
            .. "uncatchable throw (streak %d of %d) — guard STAYS ON. Isolated trips are the "
            .. "UE4SS lua_instances race, not a bad handle; only a full streak disables it.\n",
            guard.trips, guard.streak, MAX_ALIVE_STREAK))
    end
    -- ORDER MATTERS (2026-07-31, crash audit RANK 5). The address is taken FIRST, inside the
    -- transactional guard, and only then handed to the offset probe: GetAddress is the call that
    -- pierces pcall on a non-UObject handle, so it stays inside the pending window, and the probe
    -- now derives ClassPrivate from THIS object instead of doing its own StaticFindObject. The
    -- probe is inside the window too — it is raise-free by construction (every engine hop is
    -- pcall'd and every value type-checked), and its GetAddress hop belongs under the same mark.
    -- Once CLASS_OFF is known, class_off() returns on its first line and costs nothing.
    guard.pending = true
    local a = Mem.raw_addr(obj)
    local off = class_off(obj, a)
    guard.pending = false
    -- Reaching here means the attempt COMPLETED, which is exactly what the streak measures. Reset
    -- it so only an UNBROKEN run of unwound attempts can ever disable the pre-check.
    guard.streak = 0
    if a == nil then return true end                      -- handle cannot answer → do not block
    a = math.tointeger(a)
    if a == nil then return true end                      -- not an address we can read → open
    if a == 0 then return false end                       -- a real null: never touch it
    if off == nil then return true end                    -- cannot check → do not block
    -- No pcall around these two: read_ptr is SEH-guarded (an unreadable address returns nil,
    -- it cannot fault), and its only Lua-level failure mode is a non-integer argument, which
    -- is ruled out above (`a` went through math.tointeger, `off` is our own derived integer,
    -- and read_ptr itself pushes an integer). This is the hottest path in the mod — each
    -- `pcall(function() … end)` allocates a fresh closure, so the check must not add any.
    local bad
    local cls = m.read_ptr(a, off)
    if cls == nil or cls == 0 then                        -- base unreadable or classless
        bad = true
    else
        local meta = m.read_ptr(cls, off)
        bad = (meta == nil or meta == 0)                  -- that "class" is not a UObject
    end
    if bad then
        rejects = rejects + 1
        -- The COUNT alone was useless: 401 rejections reads the same whether they were genuinely
        -- dead handles or live objects being wrongly refused. So log the raw evidence — the stored
        -- address and the class pointer we read off it. No dereference, so this is as safe as the
        -- check itself. A REPEATING address means a live object is being rejected over and over
        -- (a false positive); addresses that never repeat are ordinary churn. `cls == 0` with a
        -- plausible-looking address is the signature of GetAddress not pointing at a UObject base,
        -- which would make the whole pre-check wrong rather than unlucky.
        if rejects % REJECT_LOG_EVERY == 1 or rejects <= 5 then
            print(string.format(
                "[KakarotAccess] memory pre-check: %d rejected (addr=0x%X cls=%s)\n",
                rejects, a, tostring(m.read_ptr(a, off))))
        end
        return false
    end
    -- The address comes back as a SECOND value so callers that need it (Core.nonnull) never have
    -- to call GetAddress themselves — every such call in the mod then sits behind this function's
    -- transactional guard, which is the point. It is nil on every "cannot check" path above, so a
    -- caller that must fail CLOSED simply requires it to be present.
    return true, a
end

-- Absolute base VA of a live UObject, or nil. Guards every hop (stale objects error) — and
-- pre-checks the memory first, because the IsValid below is itself a dereference.
function Mem.addr(obj)
    if not loaded or not obj then return nil end
    if not Mem.alive(obj) then return nil end
    local ok, a = pcall(function()
        if obj.IsValid and not obj:IsValid() then return nil end
        return obj:GetAddress()
    end)
    if ok and a and a ~= 0 then return a end
    return nil
end

-- Read helpers taking a live UObject + byte offset. Return nil on any failure.
local function reader(fn_name)
    return function(obj, off)
        local a = Mem.addr(obj)
        if not a then return nil end
        return m[fn_name](a, off or 0)
    end
end

Mem.i8    = reader("read_i8")
Mem.u8    = reader("read_u8")
Mem.i16   = reader("read_i16")
Mem.u16   = reader("read_u16")
Mem.i32   = reader("read_i32")
Mem.u32   = reader("read_u32")
Mem.i64   = reader("read_i64")
Mem.ptr   = reader("read_ptr")
Mem.float = reader("read_float")

-- Raw bytes at obj+off (string of n bytes) — used by the offset-finding diff tool.
function Mem.bytes(obj, off, n)
    local a = Mem.addr(obj)
    if not a then return nil end
    return m.read_bytes(a, off or 0, n)
end

-- Base VA of a loaded module (default: the game exe). Lets a dump translate a live code
-- pointer (e.g. a vtable slot) into an RVA Ghidra can decompile (image base 0x140000000).
function Mem.module_base(name)
    if loaded and m.module_base then return m.module_base(name) end
end

-- Read directly from an absolute address (when you already have the VA) — needed to walk a
-- native array whose data pointer we just read (the element addresses aren't UObject bases).
function Mem.at_i32(addr, off) if loaded and addr then return m.read_i32(addr, off or 0) end end
function Mem.at_u8(addr, off) if loaded and addr then return m.read_u8(addr, off or 0) end end
function Mem.at_ptr(addr, off) if loaded and addr then return m.read_ptr(addr, off or 0) end end
function Mem.at_bytes(addr, off, n) if loaded and addr then return m.read_bytes(addr, off or 0, n) end end

-- ---- writes: the HOST-CLASS assertion ----------------------------------------------------
--
-- The bridge bounds the OFFSET, which stops a wild offset from landing outside the object.
-- It cannot stop the other half of the problem: a right-offset-WRONG-OBJECT write. Every
-- offset in native_offsets.lua was derived for ONE class, so storing an int at that offset on
-- any other object overwrites a field nobody can name — silently, with no bad address and no
-- error, and a crash somewhere unrelated much later. A read on the wrong object is a wrong
-- number; a write on the wrong object is corruption.
--
-- So a writer takes the class the offset belongs to and refuses when the host positively is
-- not it. The name is not invented here: callers pass the SAME class name they already looked
-- the host up by (Core.cached_all / FindAllOf), so the assertion is "the object I write to is
-- the object I asked for".
--
-- FAILS OPEN on "don't know", CLOSED only on evidence (the 2026-07-25 rule): no expected class,
-- or a host whose class chain cannot be read, writes exactly as before — a guard that failed
-- closed here would silently break world-map fast travel, which is precisely the failure mode
-- this codebase keeps paying for. It refuses only on a chain we DID read that does not contain
-- the expected name.
local CLASS_CHAIN_MAX = 16   -- inheritance-chain bound, same as ui_core's SUPER_MAX: this
                             -- game's widget chains run ~6-10 (BP _C -> game base ->
                             -- UUserWidget -> UWidget -> UVisual -> UObject).

-- The host's class name and its ancestors as a set, plus the leaf name for logging. Returns
-- nil when nothing could be read at all — the "don't know" case, which must not block.
-- The SUPER walk matters: a lookup by class name can legitimately return a SUBCLASS instance,
-- and rejecting that would be a false refusal.
local function class_chain(obj)
    local cls
    if not pcall(function() cls = obj:GetClass() end) then return nil end
    local names, leaf, d = {}, nil, 0
    while cls ~= nil and d < CLASS_CHAIN_MAX do
        -- Mem.alive BEFORE IsValid, for the reason this whole file exists: IsValid is itself a
        -- dereference (see the block comment above), so on a freed super-struct handle it would
        -- fault instead of rejecting. Inlined rather than calling Core.valid because ui_core
        -- requires THIS module — a require back would be a cycle.
        if not Mem.alive(cls) then break end
        local okv, v = pcall(function() return cls:IsValid() end)
        if not (okv and v == true) then break end
        local n
        if not pcall(function() n = cls:GetFName():ToString() end) or type(n) ~= "string" then break end
        names[n] = true
        leaf = leaf or n
        local sup
        if not pcall(function() sup = cls:GetSuperStruct() end) then break end
        cls = sup
        d = d + 1
    end
    if leaf == nil then return nil end
    return names, leaf
end

-- One line per distinct (expected, actual) pair — never per tick. A game patch that renames or
-- re-parents the host then shows up in UE4SS.log instead of silently disabling the feature.
local refused_seen = {}
local function class_ok(obj, expect)
    if expect == nil then return true end               -- caller said nothing: do not block
    local names, leaf = class_chain(obj)
    if names == nil then return true end                -- cannot tell: do not block
    if names[expect] then return true end
    local mark = expect .. "<-" .. tostring(leaf)
    if not refused_seen[mark] then
        refused_seen[mark] = true
        print(string.format("[KakarotAccess] memory WRITE refused: expected a %s, host is a %s "
            .. "(offset belongs to another class)\n", expect, tostring(leaf)))
    end
    return false
end

-- Guarded WRITES. Same SEH guard as reads (a bad address is a no-op returning false).
-- Used to snap a game cursor onto a target by overwriting the member the game reads back
-- as its own source (world-map fast travel). Return true only if the store succeeded.
-- `expect_class` is the class the offset was derived for (see the note above) — optional only
-- so the guard can fail open, NOT so new call sites can skip it.
local function writer(fn_name)
    return function(obj, off, v, expect_class)
        local a = Mem.addr(obj)
        if not a or not m[fn_name] then return false end
        if not class_ok(obj, expect_class) then return false end
        return m[fn_name](a, off or 0, v) == true
    end
end

Mem.write_i32   = writer("write_i32")
Mem.write_u32   = writer("write_u32")
Mem.write_float = writer("write_float")

-- Read a non-reflected UE FString member at obj+off. FString = { TCHAR* Data; int32
-- Num; int32 Max }; Data is UTF-16LE, Num includes the null terminator. Returns a
-- UTF-8 Lua string (BMP only), or nil. Used to read hidden label members the game
-- keeps outside reflection (e.g. the field-menu header section label).
function Mem.fstring(obj, off)
    local a = Mem.addr(obj)
    if not a then return nil end
    off = off or 0
    local data = m.read_ptr(a, off)
    local num = m.read_i32(a, off + 8)
    if not data or data == 0 or not num or num <= 1 or num > 1024 then return nil end
    local raw = m.read_bytes(data, 0, num * 2)   -- num includes the trailing null
    if not raw then return nil end
    local out = {}
    for i = 1, #raw - 1, 2 do
        local lo, hi = raw:byte(i, i + 1)
        if not hi then break end
        local cp = lo + hi * 256
        if cp == 0 then break end
        out[#out + 1] = utf8.char(cp)
    end
    local s = table.concat(out)
    if s == "" then return nil end
    return s
end

return Mem
