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
local Mem = require("mem")   -- Mem.alive (the SEH-guarded pre-check every guard here runs first)
                             -- and Mem.thread_id (stamping the game thread, see begin_scan_tick)

local Core = {}

Core.POLL_MS = 100
local TOOLTIP_WINDOW = 6   -- ticks to keep polling for a late-arriving tooltip

-- ---- widget helpers --------------------------------------------------------

-- Does this handle point at a REAL, readable object? Asks the STORED pointer (never a deref) and
-- then checks that memory through mem_bridge — so, unlike IsValid(), it cannot fault on a handle
-- that wraps NULL. That is what makes `FSlateBrush.ResourceObject` readable at last: the crash
-- ledger's CLASS A ("no safe guard exists") assumed the only way to ask was `ro:IsValid()`, which
-- IS the deref that pierces pcall on a null resource (the 0x10 access violations). Never ask
-- IsValid on a brush resource.
-- Fails CLOSED (cannot answer = treat as null): the callers are cosmetic glyph/texture names, and
-- losing one is infinitely cheaper than the crash.
function Core.nonnull(o)
    if o == nil then return false end
    -- Both questions in ONE call: Mem.alive already asks for the stored pointer and returns it,
    -- so this no longer calls GetAddress itself. That matters beyond tidiness — a direct call
    -- here would sit outside Mem.alive's transactional guard, and a GetAddress on a non-UObject
    -- handle pierces pcall and kills the caller's tick (Core.valid_ref explains the whole story).
    -- Fails CLOSED by requiring a real address: on any path where the pre-check cannot answer,
    -- Mem.alive returns no address, and a cosmetic glyph is not worth a guess.
    local ok, a = Mem.alive(o)
    return ok and type(a) == "number" and a ~= 0
end

-- Validity, with the NULL-handle guard.
--
-- A UE4SS RemoteObject can wrap a NULL UObject and STILL answer IsValid() == true —
-- the documented failure mode of this game (CLAUDE.md §8: "a cached widget passes
-- IsValid() and reports a NULL UObject on the very next member call"). The next
-- property fetch then makes UE4SS read ClassPrivate at UObjectBase+0x10 off that null
-- base, which is an ACCESS VIOLATION inside the VM: uncatchable by pcall, it kills the
-- process. That is literally the user's 2026-07-24 mid-combat report
-- (EXCEPTION_ACCESS_VIOLATION reading address 0x00000010) — the fault address IS the
-- ClassPrivate offset, so the base was null.
--
-- GetAddress() returns the STORED pointer without dereferencing it, so it is the one
-- question we can safely ask, and the only pre-check that can see the null before we
-- touch a member. Anything that fails to answer counts as dead.
-- REGRESSION NOTE (2026-07-25). The 07-24 cut of this function rejected a handle whose
-- `GetAddress()` answered NIL, treating "the wrapper can't tell you" as "the pointer is null".
-- Those are different questions, and the blast radius is not one widget: ui_directory gates
-- EVERY pointer hop of every chain on Core.valid (mm -> m_xLoadMenu -> m_UIStartSaveLoad …),
-- so one unanswerable hop makes a directory-mapped screen resolve to nothing and go silent
-- with no error logged anywhere — the load-game screen and the Options save confirmations.
-- The null case is now handled where it belongs, inside Mem.alive, which fails OPEN on every
-- "don't know" and closed only on memory it has actually read.
-- Validity for a RemoteObject that is NOT a UObject — a TArray wrapper, a struct handle. It does
-- `IsValid()` and NOTHING else, and that restriction is the whole point.
--
-- `GetAddress` is overridden only on the UObject family. On any other RemoteObject UE4SS's base
-- implementation RAISES `Call to RemoteObject:GetAddress on polymorphic type is not allowed`, and
-- that error PIERCES pcall: it unwinds to UE4SS's own callback boundary, so the adapter's update
-- dies mid-function while `pcall` reports nothing. Proven from the user's UE4SS.log on 2026-07-25
-- — 510 occurrences, every single one entering through `Core.array_of`'s validity check on the
-- ARRAY, and the screens in the tracebacks are exactly the screens the user reported silent:
-- the save/load slot list, the status page's stat rows, the skill palette's plates, the emblem
-- grid, the tutorials list, the skill tree's orbs, keyhelp's glyph list, the dialog's choices.
-- The 2026-07-24 cut had the same call in Core.valid, which is what silenced them then too.
-- RULE: a TArray or struct handle goes through HERE; only a UObject goes through Core.valid.
-- SCOPE, narrowed 2026-07-28: ARRAY and STRUCT handles ONLY. `IsValid` is a RemoteObject
-- method; it does not exist on UE4SS's FName/FText bindings, and calling it there does not
-- fail politely — the call resolves against the global FName CONSTRUCTOR and raises
-- "No overload found for function 'FName'", an error that PIERCES pcall exactly like the
-- GetAddress-on-polymorphic-type one above. See Core.name_str for the incident.
function Core.valid_ref(o)
    if o == nil then return false end
    local ok, v = pcall(function() return o:IsValid() end)
    return ok and v == true
end

-- Read an FName/FString-ish member as a Lua string. nil = not readable.
--
-- WHY THIS EXISTS (2026-07-28). An FName is a VALUE (two indices), not a RemoteObject: it
-- wraps no heap pointer we could outlive, so there is nothing a validity check could catch —
-- and asking anyway is fatal, because `IsValid` is not one of its methods (see Core.valid_ref).
-- That raise killed `Nav.list_targets` mid-flight on the FIRST field NPC the radar picker
-- looked at, so `do_open()` never got a list and the R3 / V radar menu did not open AT ALL.
-- 16 tracebacks in the user's log, every one entering through Core.member's result gate on
-- the NameProperty `UniqueId`.
-- So: no validity call here. The conversion IS the test — a handle that cannot answer
-- `ToString()` yields nil, which is what every caller already treats as "no name".
--
-- CONTRACT: pass only Name/Str/Text property results. A UObject is survivable (indexing it with
-- `ToString` yields an empty wrapper whose `__call` raises a pcall-CATCHABLE error), but a TArray
-- wrapper is NOT: UE4SS coerces the method name to an integer index, gets 0, and reads element -1
-- — a silent out-of-bounds read one slot before the buffer, with no error at all. Every call site
-- today is a NameProperty or an FString verified against the CXX header; keep it that way.
function Core.name_str(o)
    if o == nil then return nil end
    if type(o) == "string" then return o end
    if type(o) ~= "userdata" then return nil end
    local s
    if not pcall(function() s = o:ToString() end) then return nil end
    return (type(s) == "string" and s ~= "") and s or nil
end

-- Per-tick memo for Core.valid, keyed by the HANDLE itself (see Core.valid). Declared here,
-- above its only two users, because a local referenced above its own declaration compiles to a
-- global read, is nil at runtime and takes the mod down from boot — the 2026-07-25 lesson.
-- Cleared in begin_scan_tick alongside os_memo.
local valid_memo = {}

function Core.valid(o)
    if o == nil then return false end
    -- Memo FIRST, and keyed by the handle rather than by the object's address, because the
    -- address is not free: obtaining it is most of what this function costs. Two RemoteObjects
    -- wrapping one object therefore each pay once (unlike os_memo, which can share by address
    -- since it has already paid for one) — and the case that matters is the same CACHED handle
    -- being re-checked, which is what the pool walks do.
    --
    -- Why this became load-bearing (2026-07-26): before the crash fix this function was one
    -- pcall'd IsValid. It is now GetAddress + two native reads + IsValid, and `first_on_screen`
    -- runs it over the WHOLE cached pool on every call, several times per tick, for pools like
    -- CFUIMultiLineTextBox that hold hundreds of entries. Hardening the reader is what made the
    -- reader slow; the verdict simply cannot change within one game-thread tick, so caching it
    -- costs nothing in correctness and takes the walk back to roughly its old price.
    local memo = valid_memo[o]
    if memo ~= nil then return memo end
    -- BEFORE IsValid, never after: IsValid is ITSELF a dereference of the object it is being
    -- asked about (UE4SS evaluates IsUnreachable() before its own object-set lookup), so this
    -- SEH-guarded memory pre-check is the only guard that can run first. See Mem.alive.
    local r = false
    if Mem.alive(o) then
        local ok, v = pcall(function() return o:IsValid() end)
        r = ok and v == true
    end
    valid_memo[o] = r
    return r
end

-- Guarded member fetch. `o.Name` is evaluated at the CALL SITE, so handing it
-- straight to a helper (`read_text(bar.Txt00)`) leaves the fetch OUTSIDE every
-- pcall: on a stale pooled widget, or on a class that doesn't have that member,
-- that is the uncatchable abort — it crashed the game on 2026-07-17 (fishing) and
-- it is the access-violation class in the two user crash reports of 2026-07-21
-- (property __index on a dangling UObject, deep in the Lua VM, no traceback).
-- Any code that can run while the world is being torn down fetches through here.
-- ---- property-existence gate ------------------------------------------------------------
--
-- Fetching a member a class does NOT declare is one of this game's UNCATCHABLE aborts —
-- UE4SS raises it below the Lua boundary, so the `pcall` in Core.member below cannot save
-- you. It killed the process on 2026-07-17: `node_text(bar.Txt00)` where `Txt00` belongs to
-- the SIBLING item-log bar class, retried every tick on a blank pooled bar. The rule written
-- then ("never assume a member exists from a look-alike sibling class") is a rule for humans;
-- this is the same rule enforced by the code. The only real defence is not to ask.
--
-- So the first time a (class, name) pair is needed, enumerate the class's REAL property names
-- and cache them. `ForEachProperty` lists a class's OWN properties only, so the
-- `GetSuperStruct()` walk is what makes inherited members resolvable (a UMG widget's
-- BP-declared `Txt_*` live on the generated `_C` class, `Slot`/`RenderOpacity` on the native
-- bases) — the same idiom discover.lua already uses to dump classes.
--
-- FAILS OPEN, like every other guard here (the 2026-07-25 rule): if the class cannot be
-- identified, or the enumeration yields nothing, the fetch proceeds exactly as before. A gate
-- on the hottest path in the mod must never become the reason a screen goes quiet — and when
-- it DOES block something it says so in the log, capped, with the class and member named, so
-- an over-eager rejection is visible instead of silent.
-- KILL SWITCH for BOTH reflection gates (member existence + the array type check), restoring the
-- pre-2026-07-25 behaviour exactly. It exists because the failure mode of a gate on this path is a
-- screen that goes quiet with no error, and whoever is testing must be able to rule it out
-- immediately rather than wait for a code change.
--
-- Toggled IN GAME with Ctrl+G (a dev keybind; see main.lua) — editing this file was the first
-- design and it was worse: it needs a text editor mid-session, and the state reset to the default
-- on every reload. The value therefore lives in `_G`: Ctrl+Shift+R DOES re-require this module, so a
-- plain local would silently turn the gates back on at every reload, which is the opposite of what a
-- diagnostic switch is for. Mirrored into a local because prop_set reads it on the hottest path.
if _G.__KakarotReflectionGates == nil then _G.__KakarotReflectionGates = true end
local REFLECTION_GATES = _G.__KakarotReflectionGates

-- Returns the new state. If flipping this makes a screen read again, the `member gate:` /
-- `array gate:` log lines name exactly what was being refused.
function Core.toggle_gates()
    REFLECTION_GATES = not REFLECTION_GATES
    _G.__KakarotReflectionGates = REFLECTION_GATES
    return REFLECTION_GATES
end

local prop_sets = {}      -- class address -> { [name] = "<PropertyType>" } | false if unavailable
local SUPER_MAX = 16      -- inheritance-chain depth cap. This game's widget chains run ~6-10
                          -- (WidgetBlueprintGeneratedClass → game base → UUserWidget → UWidget →
                          -- UVisual → UObject); 16 leaves margin, and stopping SHORT would drop
                          -- inherited members and make the gate reject them.
local blocked_seen = {}   -- "classaddr:name" -> true (log each pair once)
local blocked_logged = 0
local BLOCKED_LOG_MAX = 50
-- Building a set walks the whole inheritance chain and stringifies every property name — a
-- one-off per class, but a screen opening for the first time can present a dozen new classes in
-- ONE tick, and this codebase's whole performance history is about not letting per-class work
-- cluster on a single tick (SCANS_PER_TICK exists for exactly that). So the enumeration draws
-- from its own small per-tick budget; a denied call simply skips the gate that once (fail open)
-- and the set gets built a tick later.
-- ACCEPTED LIMIT, stated plainly so nobody reads this as full coverage: because a set is built
-- at most once per tick, the FIRST fetch of a member on a never-seen class is always ungated.
-- The 2026-07-17 abort was a fetch REPEATED every tick, so it is caught from tick two — but a
-- one-shot bad fetch can still get through by construction. Raising this constant trades that
-- for per-tick spikes on screen entry, which is the tradeoff SCANS_PER_TICK already lost once.
local PROP_SETS_PER_TICK = 1
local prop_budget = 0

-- CUSTOM PROPERTIES ARE INVISIBLE TO ForEachProperty. `RegisterCustomProperty` stores the
-- member in UE4SS's own map, consulted by `__index` as a fallback — it is NOT added to the
-- UClass, so the enumeration above will never see it and the gate would refuse a member that
-- reads perfectly. The mod uses this to recover collapsed FIXED C arrays (screen_party's
-- party slots 1/2, screen_community's skill parts 1..9 — UE4SS reflection collapses a fixed
-- array to element 0). Those call sites fetch raw today, so nothing is broken right now, but
-- the standing rule is "route member fetches through Core.member", and following it would
-- have silently killed those screens. So: whoever registers a custom property declares it
-- here in the same breath, and the gate always lets it through.
local custom_props = {}
function Core.allow_member(name)
    if type(name) == "string" and name ~= "" then custom_props[name] = true end
end

local function prop_set(o)
    if not REFLECTION_GATES then return nil end
    local cls
    if not pcall(function() cls = o:GetClass() end) or cls == nil then return nil end
    local key = math.tointeger(Mem.raw_addr(cls) or 0)
    if key == nil or key == 0 then return nil end
    local cached = prop_sets[key]
    if cached ~= nil then return cached or nil end
    if prop_budget <= 0 then return nil end     -- not this tick: gate stays open
    prop_budget = prop_budget - 1
    local set, count = {}, 0
    local s, depth = cls, 0
    while s ~= nil and depth < SUPER_MAX do
        pcall(function()
            s:ForEachProperty(function(p)
                local pn, pt
                if pcall(function() pn = p:GetFName():ToString() end)
                    and type(pn) == "string" and pn ~= "" and not set[pn] then
                    -- The property's TYPE comes free with this walk, and it is what finally
                    -- makes Core.array_of safe (see there): only "ArrayProperty" is a real
                    -- TArray. Stored as a string, so `set[name]` stays truthy for the
                    -- existence gate.
                    pcall(function() pt = p:GetClass():GetFName():ToString() end)
                    set[pn] = (type(pt) == "string" and pt ~= "" and pt) or "?"
                    count = count + 1
                end
            end)
        end)
        local sup
        if not pcall(function() sup = s:GetSuperStruct() end) then break end
        if not Core.valid(sup) then break end
        s, depth = sup, depth + 1
    end
    if count == 0 then
        prop_sets[key] = false     -- introspection told us nothing: never gate on it
        return nil
    end
    prop_sets[key] = set
    return set, key
end

-- `strict` (2026-07-28): refuse the fetch when the property set is UNAVAILABLE, instead of
-- falling open to a raw `o[name]`.
--
-- Fail-open is right for a single name the caller has positive reason to believe exists — the
-- cost of being wrong is one ungated fetch on a class we know nothing about. It is exactly WRONG
-- for a multi-candidate probe (Core.first_text / Core.first_member), whose contract is that most
-- candidates DO NOT exist: there, an open gate is a licence to fetch names we have positive
-- reason to believe are absent, which is the uncatchable abort. The set is unavailable whenever
-- the per-tick enumeration budget is spent (PROP_SETS_PER_TICK = 1, shared by ~40 adapters, so a
-- screen presenting several new classes needs several ticks) or the class introspected to nothing.
--
-- Failing closed here is BOUNDED, unlike the Options regression the standing rule warns about:
-- one tick of silence per newly-seen class, self-healing the moment the set is built. The one
-- unbounded case — `prop_sets[key] = false`, set permanently when a class enumerates to nothing —
-- is logged, because that would mean permanent silence and we want it to name itself.
local strict_warned = {}
function Core.member(o, name, strict)
    if not Core.valid(o) then return nil end
    local set, key = prop_set(o)
    if strict and not set and not custom_props[name] then
        local cls
        pcall(function() cls = o:GetClass():GetFName():ToString() end)
        cls = cls or "?"
        if not strict_warned[cls] then
            strict_warned[cls] = true
            print(string.format(
                "[KakarotAccess] strict gate: no property set for %s (candidate '%s' skipped)\n",
                cls, tostring(name)))
        end
        return nil
    end
    if set and not set[name] and not custom_props[name] then
        local mark = tostring(key) .. ":" .. tostring(name)
        if not blocked_seen[mark] and blocked_logged < BLOCKED_LOG_MAX then
            blocked_seen[mark] = true
            blocked_logged = blocked_logged + 1
            local cn = "?"
            pcall(function() cn = o:GetClass():GetFName():ToString() end)
            print(string.format("[KakarotAccess] member gate: %s has no '%s' (not fetched)\n",
                cn, tostring(name)))
        end
        return nil
    end
    local v
    if not pcall(function() v = o[name] end) then return nil end
    -- VALIDATE THE RESULT, not just the owner (2026-07-26 (c)). `o[name]` on a null or dead
    -- field does NOT return nil — UE4SS hands back an INVALID RemoteObject — so every caller
    -- that wrote `local d = Core.member(...) ; if not d then return end` was passing a dead
    -- handle straight to its next hop. That is a whole CLASS of the +0x10 access violation, and
    -- `ui_directory.prop` was the only place in the codebase that already knew it.
    --
    -- Which check applies depends on the property TYPE, and we already paid to learn it: the
    -- gate above stores each member's type string. UObject-family results go through Core.valid
    -- (memory pre-check first, because IsValid would dereference); struct/array handles go
    -- through Core.valid_ref (IsValid only — Core.valid would call GetAddress on them, which
    -- UE4SS raises THROUGH pcall). That distinction is the two-tier rule, applied automatically
    -- instead of being remembered at ~200 call sites.
    --
    -- CORRECTED 2026-07-28. The first cut sent EVERY other named property type to valid_ref as
    -- well ("anything with a known type string"), on the assumption that `IsValid` is universal
    -- on RemoteObjects. It is not: an FName/FText is a VALUE, has no `IsValid`, and the call
    -- resolves against the FName CONSTRUCTOR and raises through pcall. That took out the radar
    -- picker entirely — `UniqueId` is a NameProperty and it is the first thing the target list
    -- reads about an NPC. Only the two handle-shaped types are checked now; every other type is
    -- a value that cannot dangle, so it FAILS OPEN, per the standing rule.
    --
    -- FAILS OPEN, like every guard here: a non-userdata value (string/number/bool), an unknown
    -- type, or a spent property-set budget all return the value unchanged, so nothing that reads
    -- today stops reading. It can only reject a handle we positively know the type of AND that
    -- fails its own type-appropriate check. Kill switch: Ctrl+G (REFLECTION_GATES), same as the
    -- existence gate.
    if REFLECTION_GATES and type(v) == "userdata" and set then
        local pt = set[name]
        if pt == "ObjectProperty" or pt == "ClassProperty" or pt == "WeakObjectProperty"
            or pt == "SoftObjectProperty" or pt == "InterfaceProperty" then
            if not Core.valid(v) then return nil end
        elseif pt == "ArrayProperty" or pt == "StructProperty" then
            if not Core.valid_ref(v) then return nil end
        end
    end
    return v
end

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
    local m = Core.member(node, "mainTxt")
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
--
-- SECOND failure mode (2026-07-16, screen_dialog WL_LvTextList): a native FIXED C-array member
-- (e.g. `UCFUIXcmnMultiLineText* X[7]`) collapses to a RemoteObject that PASSES the IsValid
-- check below, and GetArrayNum on it raises the "UObject instance is nullptr" C++ error THROUGH
-- the pcall — the process survives (UE4SS catches it at its callback boundary) but the whole Lua
-- tick aborts mid-function, through every enclosing pcall, leaving half-updated module state
-- (that is how a stale notice got spoken for 13 debugging rounds).
-- This note used to end "there is no runtime check for this: the caller must never pass a
-- fixed-array member here — check the CXX header dump". THERE IS ONE NOW, and it comes from the
-- engine's own metadata rather than from a human remembering to check a dump: the per-class
-- property walk (see prop_set) records each member's PROPERTY TYPE, and a real TArray is an
-- `ArrayProperty` while a fixed C array is a single ObjectProperty with ArrayDim > 1. So if the
-- class is known and the member is not an ArrayProperty, we refuse before touching it. Fails
-- open like every other guard here: an unknown class or an unenumerated member proceeds as before.
function Core.array_of(owner, name)
    if not Core.valid(owner) then return nil, nil end
    local set, key = prop_set(owner)
    -- EXISTENCE gate, the same one Core.member applies (added 2026-07-27; it was missing here).
    -- Fetching a member the owner's class does not declare is one of this game's UNCATCHABLE
    -- aborts: UE4SS raises it below the Lua boundary, so the `pcall` around `owner[name]` below
    -- does NOT catch it and the process dies. The type check that follows only rejects a member
    -- declared with the WRONG type, so a member that does not exist AT ALL used to fall straight
    -- through to that raw fetch — live every tick, not theoretical: screen_dialog probes
    -- {WL_TextPlateCtn, UIChoice_List} against whatever dialogue window is on screen and by design
    -- each window class declares only one of the two.
    -- FAILS OPEN when the class is unknown (`set` nil: gates off, per-tick budget spent, or the
    -- enumeration yielded nothing) — a guard that fails closed on this shared path takes out every
    -- screen at once with nothing in the log. Blocks only on positive evidence of absence.
    -- custom_props is consulted because `RegisterCustomProperty` members live in UE4SS's own map
    -- and are INVISIBLE to ForEachProperty, so they must never be rejected here.
    if set and not set[name] and not custom_props[name] then
        local mark = tostring(key) .. ":arr:" .. tostring(name)
        if not blocked_seen[mark] and blocked_logged < BLOCKED_LOG_MAX then
            blocked_seen[mark] = true
            blocked_logged = blocked_logged + 1
            local cn = "?"
            pcall(function() cn = owner:GetClass():GetFName():ToString() end)
            print(string.format("[KakarotAccess] array gate: %s has no '%s' (not read)\n",
                cn, tostring(name)))
        end
        return nil, nil
    end
    local declared = set and set[name]
    if declared and declared ~= "ArrayProperty" and not custom_props[name] then
        local mark = "arr:" .. tostring(name) .. ":" .. declared
        if not blocked_seen[mark] and blocked_logged < BLOCKED_LOG_MAX then
            blocked_seen[mark] = true
            blocked_logged = blocked_logged + 1
            print(string.format("[KakarotAccess] array gate: '%s' is a %s, not a TArray (not read)\n",
                tostring(name), declared))
        end
        return nil, nil
    end
    local arr
    -- valid_REF, not valid: `arr` is a TArray wrapper, and asking one for its address raises an
    -- error that pierces pcall and kills this whole tick (see Core.valid_ref). The OWNER above is
    -- a UObject and does go through the full check.
    if not pcall(function() arr = owner[name] end) or not Core.valid_ref(arr) then return nil, nil end
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
    -- Per-class attribution (2026-07-16: a Ctrl+F5 measured 3310 scans / 225s total in
    -- one session with no way to tell WHICH classes drained it). The dump prints the
    -- top offenders so they get directory-mapped or re-tuned with data, not guesses.
    if not s.by then s.by = {} end
    local b = s.by[cls_name]
    if not b then b = { n = 0, ms = 0 } s.by[cls_name] = b end
    b.n = b.n + 1
    b.ms = b.ms + dt
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
-- (os_memo) being reset by any of them is harmless.
--
-- The BUDGET is a different matter, and the note that used to stand here called that
-- harmless too. It is not (2026-07-26). `begin_scan_tick` has SIX call sites — this loop,
-- battle_monitor, quest_objective (three times in its own step), and ui_directory — and each
-- one refilled the budget outright, so "2 scans per tick" was really up to a dozen. This is
-- the playbook's own rule violated in the substrate that enforces it: A SCAN SLOT IS NOT A
-- RATE LIMIT. A budget apportions work BETWEEN competing callers; it does nothing to stop the
-- callers themselves asking again, and a refill counted in CALLS is confirmed in milliseconds
-- by a poll invoked from several loops. It matches the measurement nobody could explain:
-- 1576 scans in 5.5 min = 31% of the game thread spent inside FindAllOf, at a nominal ceiling
-- that should have made that arithmetic impossible.
--
-- So the refill is now keyed to WALL TIME, and the ceiling means what it says: SCANS_PER_TICK
-- per REFILL_EVERY_S, however many loops ask. Nothing else changes — deferred classes still
-- just retry, and the event lanes (watch + boost) still run first, so screen-entry latency is
-- untouched. What goes away is only the steady-state multiplication.
local REFILL_EVERY_S = 0.1   -- the registry poll period: one refill per tick, not per caller
local last_refill = 0
local service_watches   -- forward decl — assigned below the watch lane (Core.watch_for)

-- WORLD-EPOCH POLL — the transition gate's only signal since 2026-07-25 (it replaced the
-- GameMode construction notify: see transition.lua for why a notify was the wrong door).
-- MUST run even while the gate is already up: that is how the new world gets noticed.
-- Cost: one validity check on the cached GameInstance plus a GetWorld() call, no scan.
--
-- Kept SEPARATE from begin_scan_tick (which also resets the per-tick scan budget) because
-- every loop that touches UObjects has to poll it, and not every such loop wants to reset
-- the budget: nav_tracker runs on its own cadence outside the registry step. begin_scan_tick
-- calls this first thing; nav_tracker calls it directly.
-- Forward declaration. `dir_mod` is DEFINED further down, next to the directory cache it
-- belongs to, but it is USED here — and a `local function` only enters scope at its own
-- statement, so without this line the call below would compile to a GLOBAL lookup, be nil at
-- runtime, and raise on every tick from a spot ABOVE the loops' pcall: the whole mod silent
-- from boot, with nothing in the log. `luac -p` cannot see it (a global call is valid syntax).
local dir_mod

function Core.poll_world()
    -- Refilled HERE and not in begin_scan_tick, because nav_tracker calls only this one — and
    -- it is the loop that fetches members on the pooled per-level minimap icons that level
    -- streaming frees, i.e. the path with the most to gain from the member gate. Refilling in
    -- begin_scan_tick left that budget permanently at zero whenever the reader was toggled off,
    -- so the gate was open exactly where it mattered most.
    prop_budget = PROP_SETS_PER_TICK
    -- The per-tick memos are dropped HERE as well as in begin_scan_tick, for the same reason the
    -- budget above is refilled here: THE NAV LOOP CALLS ONLY THIS FUNCTION. Miss this and the
    -- validity memo added on 2026-07-26 becomes a bug that is strictly worse than the cost it was
    -- meant to save — nav's cross-tick handles are the same userdata every tick, so they are
    -- exactly what hits the memo, and their verdict would be answered from a lookup computed one
    -- to three nav ticks earlier (the clear would depend on foreign loops: the registry at 100 ms,
    -- which Ctrl+M stops entirely; battle_monitor at 250 ms, which returns early during a
    -- transition; quest_objective at 300 ms, which returns early while an adapter is active).
    -- `Mem.alive` would then be skipped for up to 300 ms on the handles most likely to be freed
    -- in that window: enemy actors, which the engine destroys the moment they die. A cached
    -- "valid" is not a cheap answer, it is the dangling-handle bug with extra steps.
    -- Cost of clearing twice per registry tick: two empty tables. The sweep runs after this call,
    -- so it still gets the full benefit of the memo.
    os_memo = {}
    valid_memo = {}
    local d = dir_mod()
    if not d then return end
    local ok, e = pcall(d.world_epoch)
    if ok then Transition.note_epoch(e) end
end

function Core.begin_scan_tick()
    -- Wall-clock refill (see REFILL_EVERY_S): whichever loop gets here first in a window
    -- opens the budget, the rest of that window's callers inherit whatever is left. Never
    -- top up mid-window — that was the bug.
    local t = os.clock()
    if t - last_refill >= REFILL_EVERY_S then
        scan_budget = SCANS_PER_TICK
        last_refill = t
    end
    -- The on_screen and validity memos are per-CALL correctness caches, not budgets: they must
    -- still be dropped every time, or a caller could read a verdict computed for a previous
    -- tick — and a validity verdict that outlives its tick is exactly the dangling-handle bug
    -- these guards exist to prevent.
    os_memo = {}
    valid_memo = {}
    -- Stamp the game thread's id once (this runs inside ExecuteInGameThread, so it IS the game
    -- thread). Kept now that the mod registers NO construction notify at all (2026-07-25): it is
    -- the reference any future callback must check itself against, so we can never again ASSUME
    -- a callback is delivered on the right thread — that assumption cost two crashed sessions.
    if _G.__KakarotGameTid == nil then _G.__KakarotGameTid = Mem.thread_id() end
    -- Serve armed watches FIRST, before the adapter sweep can spend the budget: a
    -- watched class queried at the tail of the sweep was starved on every contended
    -- tick (the 10ms-time-gate lesson, budget edition — measured on the soul-emblems
    -- flow 2026-07-16: zero watched scans ran in the whole 5s window), and the sweep's
    -- first-active short-circuit can stop its adapter being polled at all.
    if service_watches then service_watches() end
    -- The world poll goes AFTER the watch lane, not before it. It can need a scan slot (the
    -- GameInstance root is re-found once per map, since a transition wipes the root cache), and
    -- taking that slot ahead of an armed watch is precisely the starvation measured on the
    -- soul-emblems flow in 2026-07-16. Being one tick late to notice a map switch costs nothing;
    -- world_epoch answers `false` ("could not look"), which is not an event, when it is denied.
    Core.poll_world()
end
local function scan_allowed()
    if scan_budget <= 0 then return false end
    scan_budget = scan_budget - 1
    return true
end

-- Cinematic quiet mode (ui_registry sets it each tick from the committed adapter):
-- while a passive overlay that coexists with heavy engine work owns the screen
-- (cutscene subtitles, the NPC talk window), STEADY-STATE scans are deferred — a
-- backoff expiring mid-cutscene must not cost a ~65ms FindAllOf exactly when the
-- game thread is busiest (the cinematics-lag pass, 2026-07-16). Event-driven scans
-- keep running: the boost window (a press/commit means a real screen may be
-- appearing) and the watch lane (an armed entry signal). Deferred classes simply
-- retry when quiet lifts — no backoff is written, so nothing is pushed out.
local quiet_mode = false
function Core.set_quiet(q) quiet_mode = q and true or false end
function Core.scan_quiet() return quiet_mode end

-- Auto-appearing NARRATIVE surfaces stay outside quiet mode: they open with no user
-- press (a cutscene's first subtitle, an NPC talk window, an area telop, a notice),
-- so quiet deferral would silence exactly what a cinematic must keep reading. Their
-- cost is bounded: pooled-alive after first sight (30s refresh), absent only right
-- after a transition. Every other class CAN wait for a press (the hot window lifts
-- quiet) or an entry-signal watch.
local QUIET_EXEMPT = {
    ["Xcmn_Subtitles_C"]     = true, -- cutscene/voice subtitles (screen_dialogue)
    ["Field_Talk_Win_C"]     = true, -- NPC talk window (screen_dialogue)
    ["Quest_Main_Telop_C"]   = true, -- area/quest telop banners (screen_telop)
    ["Xcmn_Win01_C"]         = true, -- notice window (boot notices, rewards — screen_dialog)
    -- Loading screens: the post-transition state (mm reachable, no minimap, no
    -- adapter, no presses) IS the quiet state, and the transition flush empties
    -- these pools — without the exemption nothing could scan, the loading adapter
    -- never claimed, and the story recap/tips went UNREAD (user regression,
    -- 2026-07-17). Nothing-live refinement keeps them free during cutscenes.
    ["Loading_C"]            = true, -- the loading/recap host (screen_loading)
    ["Xcmn_MultiLineText_C"] = true, -- the text pool its content() reads
    -- Info_Name_C was here 2026-07-17..17: the intro-card reader now detects via
    -- the fm.InfoName pointer (no scans) — the pooled class is a dead end.
    ["Mgame_Result_C"]       = true, -- minigame "¡BRAVO!" result sheet (screen_fishresult)
}

-- The directory's root lookups (FindFirstOf) are full-array walks too, so they draw from
-- the same per-tick budget as FindAllOf — one shared cap on scan work per tick.
function Core.take_scan_slot() return scan_allowed() end

-- ---- screen directory (ui_directory.lua): the game's own screen registry -----
-- Mapped classes resolve to their live instance through 2-3 guarded pointer reads on the
-- game's HUD/menu managers — no FindAllOf, no backoff, a just-opened submenu is seen the
-- same tick. Lazily required (ui_directory requires ui_core back, so a top-level require
-- here would cycle). Returns nil for unmapped classes — those keep the scan path.
local Dir = nil
-- Assignment, not `local function`: the local is declared far above (see the forward
-- declaration next to Core.poll_world, which calls this).
dir_mod = function()
    if Dir == nil then
        local ok, m = pcall(require, "ui_directory")
        Dir = (ok and m) or false
        if Dir == false then
            print("[KakarotAccess] ui_directory unavailable, scan-only mode\n")
        end
    end
    return Dir or nil
end
local function directory_list(cls_name)
    local d = dir_mod()
    if not d then return nil end
    return d.resolve(cls_name)
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
    -- Quiet mode: a plain backoff-expiry scan waits for quiet to lift; a boost-window
    -- scan (user action) keeps its claim, and the narrative classes are exempt.
    -- Deferred = retry next tick, like a budget miss.
    if quiet_mode and not QUIET_EXEMPT[cls_name] and not boosted
        and tick >= boost_until then return nil end
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

-- Targeted watch lane: an adapter that KNOWS a class is being opened right now (an entry
-- signal — e.g. the game's LAZY menu controller just flipped null→valid) registers a
-- short watch. While it lasts, the class re-scans every WATCH_EVERY ticks regardless of
-- its backoff — including when the cached pool still holds a VALID parked instance (the
-- soul-emblems flow spawns a FRESH widget per visit next to the old parked one, so
-- any_valid() alone can't say "done"; the ADAPTER clears the watch when its screen
-- actually reads, ui_core only caps it). Bounded by design: one named class, a ~5s
-- deadline, the shared per-tick budget, and an explicit clear — it can never become the
-- per-tick forcing or the boost-window storm.
local watch_until, watch_next = {}, {}
local WATCH_EVERY = 8     -- ~800ms between re-scans PER CLASS. Was 4: with two classes
                          -- armed that was one 65ms FindAllOf per 200ms (~30% of the
                          -- game thread) — felt as navigation lag spikes (user,
                          -- 2026-07-16). Callers watching several classes stagger them
                          -- via `delay` so the combined cadence stays even (~400ms).
local WATCH_TICKS = 30    -- ~3s cap (was 5): the widget is born ~1s after the confirm
                          -- (run-3 timeline); long constructions (first-visit tutorial)
                          -- are covered by the caller RENEWING the watch, not by one
                          -- long window — a false arm must stay cheap.
-- `delay` (ticks): postpone the FIRST scan — used to interleave multi-class watches.
-- Applied ONLY on a FRESH arm: renewals (callers re-arm every poll while waiting) must
-- never touch watch_next or the delay would be pushed forward forever and the class
-- would never scan.
function Core.watch_for(cls_name, ticks, delay)
    local fresh = watch_until[cls_name] == nil
    watch_until[cls_name] = now() + (ticks or WATCH_TICKS)
    if fresh and delay and delay > 0 then
        watch_next[cls_name] = now() + delay
    end
end
function Core.watch_clear(cls_name)
    watch_until[cls_name] = nil
end

-- The watch pump, called from begin_scan_tick (see there): querying each armed class
-- through cached_all both runs the watch cadence and gives it FIRST claim on the fresh
-- per-tick budget. Never during a transition — a watch can survive a map switch armed,
-- and scans mid-load are the boot-hang class of bugs. (Assigned, not local: forward-
-- declared above begin_scan_tick; cached_all resolves through Core at call time.)
service_watches = function()
    if next(watch_until) == nil then return end
    if Transition.active() then return end
    for cls in pairs(watch_until) do   -- cached_all may nil an expired key: legal in-loop
        Core.cached_all(cls)
    end
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
    -- Watch lane (Core.watch_for): expired watches drop here; a live one makes the class
    -- due every WATCH_EVERY ticks even past its backoff and even with valid cached
    -- entries (see the watch_for comment — a parked instance is not the fresh screen).
    local watching = watch_until[cls_name]
    if watching and tick >= watching then
        watch_until[cls_name] = nil
        watching = nil
    end
    local watch_due = (watching and not due and not boost_due
        and tick >= (watch_next[cls_name] or 0)) or false
    if due or boost_due or watch_due then
        -- Honour the per-tick scan budget (see Core.cached_live): if it's spent, reuse
        -- the stale list (or {} on first sight) and try the refresh on a later tick.
        -- Callers already validity/visibility-check every entry, so stale is harmless.
        -- Quiet mode defers plain backoff-expiry scans the same way; boost- and
        -- watch-driven scans (user action / entry signal) keep their claim. An
        -- EXEMPT class bypasses quiet ONLY while its pool holds nothing live:
        -- first detection must never wait, but an ALIVE pool already serves
        -- detection from cache, and its 30s refresh can wait out the scene (those
        -- refreshes were the residual cutscene hitches — Subtitles/Talk/Telop at
        -- ~75ms each in the 2026-07-17 clean dump).
        local exempt_due = QUIET_EXEMPT[cls_name] and not (c and any_valid(c))
        local allowed = boost_due or watch_due or not quiet_mode
            or exempt_due or tick < boost_until
        if allowed and scan_allowed() then
            if boost_due then boosted_gen[cls_name] = boost_gen end
            if watching then watch_next[cls_name] = tick + WATCH_EVERY end
            c = timed_findall(cls_name)
            -- Watched scans are rare, event-driven and the whole point of the lane —
            -- log each one so a first-visit latency report says WHEN the instance
            -- became findable (bounded: <= WATCH_TICKS/WATCH_EVERY lines per arm).
            if watching then
                print(string.format("[KakarotAccess] watch %s: %d found t=%.2f\n",
                    cls_name, #c, os.clock()))
            end
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

-- Scan-free peek: the directory list if the class is mapped and its root reachable,
-- else whatever the cached pool holds — NEVER triggers a scan or touches backoffs.
-- For per-tick predicates (the registry's cutscene gate) that must not cost anything:
-- a probe that scans would defeat the very quiet it computes (the Battle_Hud_P_Main_C
-- n=24 lesson, dump 2026-07-16 21:11).
function Core.peek_all(cls_name)
    local d = directory_list(cls_name)
    if d then return d end
    return all_cache[cls_name] or {}
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
    -- The property-name sets are keyed by CLASS ADDRESS, and a map switch is exactly when
    -- Blueprint classes get unloaded — a new class could land on a freed one's address and
    -- inherit its member list, which would make the gate reject real members. Cheap to drop
    -- (one enumeration per class on first use) and it removes the whole staleness question.
    prop_sets, blocked_seen = {}, {}
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
    -- The playbook makes this the mandatory liveness test for EVERY pooled-pane adapter, and
    -- an adapter calls it with the handle it cached on entry — i.e. precisely the handle most
    -- likely to have been freed since. Yet until 2026-07-26 it went straight to a method call
    -- on that handle: `GetVisibility()` dereferences, the pcall below cannot catch what UE4SS
    -- raises under the Lua boundary, and the whole point of Mem.alive is that the check has to
    -- run BEFORE the engine touches the object. A guard the whole codebase is told to rely on
    -- must not itself be the unguarded call. `Core.valid` fails OPEN on "cannot tell", so a
    -- pane that merely cannot answer still reports live and nothing goes quiet; only a handle
    -- proven dead returns false here, which is the right answer for a dead pane anyway.
    if not Core.valid(h) then return false end
    local ok, v = pcall(function() return h:GetVisibility() end)
    if ok and tonumber(v) ~= nil and tonumber(v) ~= 0 then return false end
    local ok2, op = pcall(function() return h:GetRenderOpacity() end)
    if ok2 and type(op) == "number" and op < 0.05 then return false end
    return true
end

-- First readable text among several CANDIDATE member names on `owner`, or nil.
--
-- The candidates are ALTERNATIVES — a native spelling and its Blueprint-tree twin, say
-- (`TextBox_Label` / `Txt_List`) — so most of them are expected NOT to exist on any given class.
-- That is exactly why this must go through Core.member: asking a class for a member it does not
-- declare is an uncatchable abort that no pcall on the stack can contain (the 2026-07-26
-- screen_toasts crash), and Core.member's existence gate is the only thing that turns "this class
-- does not have that node" into a quiet nil.
--
-- Adapters kept rolling this by hand (screen_questreward, screen_fishresult, and both 2026-07-28
-- newcomers), which is three chances to omit a guard; it lives here now.
-- STRICT by construction: see the `strict` argument of Core.member. A candidate whose existence
-- the gate cannot confirm is SKIPPED, never fetched.
local function first_text_by(pred, owner, names)
    if not Core.valid(owner) then return nil end
    for _, name in ipairs(names) do
        local node = Core.member(owner, name, true)
        if Core.valid(node) and pred(node) then
            local t = Core.read_text(node)
            if t and t ~= "" then return t end
        end
    end
    return nil
end

-- Default: ON-SCREEN, so stale text on a parked/pooled page does not answer.
function Core.first_text(owner, ...)
    return first_text_by(Core.on_screen, owner, { ... })
end

-- OFF-VIEWPORT variant, for widgets the game renders into a TEXTURE instead of the viewport.
--
-- This is a third liveness domain, discovered 2026-07-28 on the Z Encyclopedia and worth stating
-- plainly because no amount of guard-tightening finds it: that book's pages
-- (`UAT_UICompZPageBase.RenderTarget`, driven by `UCompZMenu.UMGRender` onto an `AZCW_BookActor`)
-- are drawn into render targets and mapped onto a 3D book mesh. They are never parented into the
-- viewport widget tree, so `on_screen` — an ancestor walk ending at the viewport — returns false
-- for a page the player is looking at, and `IsInViewport` is false too. The adapter found no live
-- page, said nothing, and logged nothing: no error, because nothing went wrong.
--
-- So for these hosts the widget's OWN slate visibility is the only signal there is. That is
-- genuinely weaker (a parked page keeps reporting its children visible), so callers must earn the
-- screen some other way — readable text plus a marked cursor row, in screen_compz's case.
-- Never reach for this on an ordinary viewport widget; `Core.first_text` is the default for a
-- reason.
function Core.first_text_offviewport(owner, ...)
    return first_text_by(Core.is_visible, owner, { ... })
end

-- First member of `owner` that exists and is a live object, among candidate names. Same
-- alternatives-not-requirements contract as Core.first_text, for pointer hops rather than text.
function Core.first_member(owner, ...)
    if not Core.valid(owner) then return nil end
    for _, name in ipairs({ ... }) do
        local m = Core.member(owner, name, true)
        if Core.valid(m) then return m end
    end
    return nil
end

-- Liveness for a PASSIVE OVERLAY — rendered and not fading out, WITHOUT pane_live's
-- `GetVisibility() == Visible(0)` requirement.
--
-- Why this is a separate gate and not a laxer pane_live: the two answer different questions.
-- pane_live asks "does this pooled INTERACTIVE pane genuinely own the screen", and there the
-- visibility check is load-bearing — a parked cooking/shop pane keeps rendering under another
-- ESlateVisibility and would otherwise shadow every adapter below it. A passive notice cannot
-- shadow anything (it speaks once and releases the dispatcher on the same tick), and in this game
-- passive overlays render as HitTestInvisible / SelfHitTestInvisible — the Xcmn_Subtitles
-- precedent — so applying the interactive gate to one holds it SILENT FOREVER.
--
-- Learned twice, which is why it now lives here: screen_fishresult 2026-07-17 (the "¡BRAVO!"
-- catch sheet read only after pressing "Siguiente", because that press flipped the visibility
-- state) and screen_questreward 2026-07-28 (the substory "Recompensas de historia" sheet never
-- read at all — the adapter was registered and correct, but `pane_live` rejected the host on
-- every tick, so it never appeared in the log once while the F7 census proved its title and all
-- four reward rows were on screen and fully opaque).
--
-- `on_screen` already drops Collapsed/Hidden; the opacity check still drops the close-animation
-- ghost (opacity fades to 0 while the visibility flags lag).
function Core.pane_rendered(h)
    if not Core.valid(h) then return false end
    local ok, op = pcall(function() return h:GetRenderOpacity() end)
    if ok and type(op) == "number" and op < 0.05 then return false end
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
-- DEFER the whole readout while an IMPORTANT line is still playing (Speech.protected()).
-- This is the shared half of the priority rule that speech.lua documents and that only two
-- adapters (quest_objective, screen_community) had wired up by hand. Why it belongs here
-- (user report 2026-07-25: reward/area notices cut off in the skill tree and on the map):
-- a notice is spoken by screen_dialog with say_protected (interrupt=true + a protection
-- window), and screen_dialog then RELEASES the screen the very next poll. The adapter below
-- it re-commits, the registry calls its reset() — which clears Announcer.open — and the next
-- focus() therefore takes the "entering the screen" branch and speaks with interrupt=true,
-- which clears the reader's whole queue and shreds the notice mid-sentence. The offender is
-- never the notice's own screen, so fixing it adapter by adapter is whack-a-mole; every menu
-- reader goes through this one function. Deferring (rather than dropping) costs nothing:
-- focus() is called every tick with the CURRENT state, so the moment the window clears the
-- announcement fires with whatever is focused then — and moves made during a notice collapse
-- into one readout instead of a burst. Bounded by speak_seconds' 6 s cap, so it cannot hang.
function Announcer:focus(screen, tab, name, value, tooltip_fn)
    if Speech.protected() then return end
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
