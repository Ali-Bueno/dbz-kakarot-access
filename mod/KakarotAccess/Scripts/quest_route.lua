-- Precise quest-objective routing: the ACTOR the current objective needs, not the game's
-- range marker. The caches, the deferral rules and the nav-facing API; the engine reads that
-- answer "which quest, which phase, what does it name" live in `quest_phase.lua`.
--
-- WHY (user report, 2026-09-08: "the radar does not take me to the fishing spot"). The radar
-- follows the game's own navi marker, and a MAINQUEST/SUBQUEST_RANGE_ONLY marker guides to
-- the CENTRE OF A CIRCLE — for "fish here" that centre is a body of water, not the metre of
-- bank the game lets you cast from. Collection quests already had their own override
-- (Nav._quest_item_target, which guides to the required ITEM); this generalises it to every
-- objective kind whose phase class NAMES a precise target.
--
-- NAME -> ACTOR is a scan of ONE component class: every routable actor carries a
-- QuestActorFindListComponent whose `m_id` is the name a phase uses, so the index below is
-- built once per world and dropped with it.
--
-- COST. A resolve runs only when the objective CHANGES (the quest HUD's own signal, via
-- Nav.notify_objective_change) or when a >= 2 s poll of GetPhaseId sees the phase move
-- without the text moving — and both the poll and the resolve sit BELOW the free-roam gate,
-- so neither touches a quest actor while a cutscene is freeing them. A resolve spends AT MOST
-- ONE FindAllOf per call and continues on the next one, so a phase-class sweep can never
-- stall a tick, and none of these class names joins Core.cached_all's ABSENT scan set.
--
-- EVERY NEGATIVE CONCLUSION HERE IS PERMANENT for the objective, which is why quest_phase
-- reports whether a strict nil was trustworthy (see its header) and why an untrustworthy one
-- DEFERS exactly like a spent scan budget, bounded by SET_TRIES.

local Core = require("ui_core")
local Mem = require("mem")
local Phase = require("quest_phase")

local Route = {}

-- Wall-time floors. The miss backoff is the playbook's "rebuild on a lookup miss, not on
-- every miss"; the poll period is the "catch a phase change with no text change" cadence the
-- objective-change signal cannot see.
local MISS_BACKOFF_S = 5
local POLL_S = 2
-- How many times ONE objective may rebuild the find-list index before it is written off.
-- Three builds MISS_BACKOFF_S apart is ~10 s — long enough for a target that was still
-- streaming when the objective changed, and BOUNDED, which is the point: a "rebuild on every
-- miss" rule whose miss becomes PERMANENT (a target in a region that is not loaded) is a
-- full world scan every 5 s for the session — the Options regression (CLAUDE.md §8). One
-- that streams in later is picked up on the next objective change or world gate.
local INDEX_TRIES = 3
-- The same idea for the OTHER unbounded wait: resolve calls one objective may spend waiting
-- for a property set to become askable. One set per tick globally, one call per nav election
-- (~1.5 s), so six is ~9 s of real refills. Past that the strict nil is taken at face value:
-- a class that introspects to nothing is un-gateable PERMANENTLY (ui_core), and waiting for
-- ever would freeze the resolver instead of handing the tick back to the game's marker.
local SET_TRIES = 6
-- How long a DEFERRED call may keep answering with the previous objective's route. Two nav
-- elections (SCAN_EVERY 15 x TICK_MS 100 ms = ~1.5 s each) is the window a normal resolve
-- lands in, and holding inside it is what stops the beacon flip-flopping between the marker
-- and the routed spot. Past it the hold is the WRONG objective's spot announced as the
-- current one, and the marker is the better answer until the resolve lands.
local HOLD_S = 3

local qm = nil          -- QuestManager, cached for this world
local qm_next = 0       -- wall-clock floor for the next QuestManager scan
local index = nil       -- find-id -> owning actor, built once per world
local index_at = 0
local dup_seen = {}     -- find-ids already reported as duplicated (log dedupe only)
local route = nil       -- the resolved record, or nil = this objective is not routable
local sig = nil         -- "<quest id>|<phase id>" the current verdict was reached against
local dirty = true      -- a resolve is owed (objective changed / world changed / never ran)
local dirty_at = nil    -- os.clock() when that became true (see hold)
local kind_i = 0        -- how far the phase-class sweep has got, as an index into `order`
local order = nil       -- the order this sweep tries the kinds in (Phase.sweep_order)
local hint = nil        -- the kind index that answered last; survives release(), see below
local cur_sig = nil     -- the signature that sweep cursor belongs to (see resolve)
local pending = nil     -- { spec, phase } — a matched phase whose actor lookup deferred
local poll_next = 0
local miss_tries = 0    -- index rebuilds spent on the objective being resolved
local set_tries = 0     -- calls spent waiting for a property set on that same objective
local budget = 0        -- FindAllOf allowance for THIS call (see the header: one)

-- One full-object scan per resolve call. A false answer means "come back next call", never
-- "there is nothing there" — every caller keeps `dirty` set and returns the held route.
local function take_scan()
    if budget <= 0 then return false end
    budget = budget - 1
    return true
end

-- A strict nil this call could not TRUST. True = defer and ask again; false = the objective
-- has waited long enough (SET_TRIES) and the nil must be taken at face value.
local function defer_set()
    set_tries = set_tries + 1
    return set_tries < SET_TRIES
end

-- Arm a re-resolve, stamping WHEN — only on the false->true edge, so a caller that fires
-- repeatedly (the quest HUD signal can) cannot keep extending the stale-route hold window.
local function mark_dirty()
    if not dirty then dirty, dirty_at = true, os.clock() end
end

local function class_of(o)
    local cn
    pcall(function() cn = o:GetClass():GetFName():ToString() end)
    return cn or "?"
end

-- find-id -> actor, over the one component class that tags every routable actor. nil (rather
-- than an empty map) when the scan DID find components but not one of them could be asked for
-- its id: caching that would be caching "the world has no routable actors", which is false,
-- permanent, and would send every objective to the marker for the rest of the world.
local function build_index()
    local map, seen, named = {}, 0, 0
    for _, c in pairs(Core.findall("QuestActorFindListComponent")) do
        if Core.valid(c) then
            seen = seen + 1
            local id = Phase.name_of(c, "m_id")
            if id then
                named = named + 1
                local owner
                pcall(function() owner = c:GetOwner() end)
                if Core.valid(owner) then
                    -- DUPLICATE IDS ARE REAL (live: C02_04002_Cpl006 tags two actors). Keep
                    -- the FIRST valid owner — else the target changes with FindAllOf's
                    -- ordering — and say so once per id, so a wrong twin is diagnosable.
                    if map[id] == nil then
                        map[id] = owner
                    elseif not dup_seen[id] then
                        dup_seen[id] = true
                        print(string.format(
                            "[KakarotAccess] nav quest route: duplicate find-id %s (kept %s)\n",
                            id, class_of(map[id])))
                    end
                end
            end
        end
    end
    if seen > 0 and named == 0 then return nil end
    index, index_at = map, os.clock()
    return map
end

-- Second return: WHY there is no actor — nil = a real miss (the index answered and the id is
-- not in it), "wait" = no scan budget or inside the miss backoff, "unready" = the component
-- class could not be enumerated, so any index built now would be meaningless.
local function actor_for(id)
    if not id then return nil end
    if index then
        local a = index[id]
        if Core.valid(a) then return a end
        if os.clock() - index_at < MISS_BACKOFF_S then return nil, "wait" end
    end
    if not take_scan() then return nil, "wait" end
    local map = build_index()
    if not map then return nil, "unready" end
    local a = map[id]
    if Core.valid(a) then return a end
    return nil
end

-- What a DEFERRED call answers with: the held route while the hold is still smoothing over a
-- resolve that spans calls, nil once it would merely be stale (see HOLD_S).
local function hold()
    if route and dirty_at and (os.clock() - dirty_at) > HOLD_S then return nil end
    return route
end

-- Commit a verdict for signature `s`: `r` (a record) or nil (this objective is not one we can
-- route). One log line per CHANGE, never per call.
local function finish(r, s)
    if r and (route == nil or route.key ~= r.key) then
        print(string.format("[KakarotAccess] nav quest route: %s -> %s (%s)\n",
            r.kind, class_of(r.actor), tostring(r.id)))
    elseif route and not r then
        print("[KakarotAccess] nav quest route released\n")
    end
    route, sig, dirty, dirty_at = r, s, false, nil
    -- The cursor is INVALIDATED rather than merely rewound: a later re-resolve of this same
    -- signature (the routed actor died with its sub-level) must sweep the kinds again from
    -- the start, not resume where this one stopped.
    kind_i, cur_sig, order, pending, miss_tries, set_tries = 0, nil, nil, nil, 0, 0
    return r
end

-- The ≥2 s phase poll: a quest can advance to a new phase without the HUD text changing, and
-- the objective-change signal is the only other thing that would tell us. Cheap by
-- construction — cached handles, a few reflected reads, no scan of any kind — and called only
-- from below the free-roam gate.
local function poll()
    if dirty or os.clock() < poll_next then return end
    poll_next = os.clock() + POLL_S
    if not Core.valid(qm) then return end
    local _, phase, _, qid, blocked = Phase.navigated(qm)
    -- A blocked read is not a phase change. Arming one here would re-run the whole class
    -- sweep every time the property budget happened to be spent when the poll came round.
    if blocked then return end
    if ((qid or "-") .. "|" .. (phase or "-")) ~= sig then mark_dirty() end
end

-- THE PRECISE OBJECTIVE TARGET, or nil when this objective has none (the caller then keeps
-- the game's navi marker, exactly as before this feature existed). `tick` is the caller's own
-- tick counter, for Core's cache bookkeeping.
function Route.resolve(tick)
    -- THE FIELD GATE COMES FIRST, above the phase poll as well as above every scan. A menu, a
    -- battle or a cutscene is not evidence that the objective moved, and the poll is not free
    -- of the world either: it calls GetPhaseId on every created quest, i.e. reflected work on
    -- actors that a streaming teardown is in the middle of freeing.
    if not Core.free_roam(tick) then return hold() end
    poll()
    if not dirty then
        if route == nil then return nil end
        if Core.valid(route.actor) then return route end
        -- The routed actor died with its sub-level. DROP it rather than merely marking the
        -- resolve dirty: every deferred exit below answers with `route`, and serving a handle
        -- just rejected is the recycled-address hazard the check exists to catch. `sig` goes
        -- with it, so the memo below cannot answer from the verdict that just died.
        print("[KakarotAccess] nav quest route released (target gone)\n")
        route, sig = nil, nil
        mark_dirty()
    end
    budget = 1
    if not Core.valid(qm) then
        qm = nil
        if os.clock() < qm_next or not take_scan() then return hold() end
        qm_next = os.clock() + MISS_BACKOFF_S
        for _, o in pairs(Core.findall("QuestManager")) do
            if Core.valid(o) then qm = o break end
        end
        if not qm then return hold() end
    end
    local _, phase, side, qid, blocked = Phase.navigated(qm)
    if not phase then
        if blocked and defer_set() then return hold() end
        return finish(nil, "-|-")
    end
    local s = qid .. "|" .. phase
    -- MEMO, keyed on the signature. `invalidate()` fires on every objective-TEXT change, and a
    -- collection counter ticking ("2/5") is a text change with the SAME phase id — so without
    -- this a fruit quest re-runs the whole four-class sweep, four full-object scans, on every
    -- pickup. The target derives from the phase and nothing else, so while the signature
    -- stands the verdict stands; both paths that falsify one clear `sig` (release, dead actor).
    if s == sig and (route == nil or Core.valid(route.actor)) then return finish(route, s) end
    -- A resolve spans several calls (one scan each), so the cursor and any half-done lookup
    -- carry over — but ONLY while they belong to this same objective. `sig` cannot stand in
    -- for `cur_sig` here: it still names the PREVIOUS verdict for the whole sweep, so testing
    -- against it would rewind the cursor on every call and re-scan the first class forever.
    if cur_sig ~= s then
        kind_i, pending, cur_sig, miss_tries, set_tries = 0, nil, s, 0, 0
    end
    if not order then order = Phase.sweep_order(hint) end
    local spec, p = pending and pending.spec, pending and pending.phase
    if not spec then
        while kind_i < #order do
            if not take_scan() then return hold() end
            kind_i = kind_i + 1
            local cand = Phase.KINDS[order[kind_i]]
            local found, unaskable = Phase.find(cand, qid)
            if found then
                hint, spec, p = order[kind_i], cand, found
                break
            end
            if unaskable and defer_set() then
                -- Instances exist but none could be asked. Rewind the cursor so this kind is
                -- scanned again next call instead of being skipped for the whole objective.
                kind_i = kind_i - 1
                return hold()
            end
        end
        if not spec then return finish(nil, s) end   -- no routable kind: the marker stays
    end
    local a, id, ok = Phase.target_of(spec, p)
    if not a and not id then
        if not ok and defer_set() then
            pending = { spec = spec, phase = p }
            return hold()
        end
        return finish(nil, s)                       -- the phase genuinely names nothing
    end
    if not a then
        local why
        a, why = actor_for(id)
        if why == "wait" or (why == "unready" and defer_set()) then
            pending = { spec = spec, phase = p }
            return hold()
        end
        if not a then
            -- The id is real and the index does not answer it: the target actor has not
            -- streamed in yet, or it appeared after the index was built. Rebuild a BOUNDED
            -- number of times (see INDEX_TRIES) rather than concluding on the first miss —
            -- and rather than retrying forever.
            miss_tries = miss_tries + 1
            if miss_tries < INDEX_TRIES then
                pending = { spec = spec, phase = p }
                return hold()
            end
        end
    end
    if not a then return finish(nil, s) end
    -- The finer spot, when the kind has one. An untrustworthy nil here is the user's own
    -- symptom — it would commit the water volume as the objective, for the whole objective —
    -- so it defers like every other unaskable read (see Phase.stand).
    local stand, stand_ok = Phase.stand(spec, a)
    if not stand_ok and defer_set() then
        pending = { spec = spec, phase = p }
        return hold()
    end
    a = stand
    local addr = Mem.raw_addr(a)
    if not addr then return finish(nil, s) end
    return finish({ actor = a, key = tostring(addr), kind = spec.kind,
                    label_key = spec.label, id = id, quest = side }, s)
end

-- The quest HUD says the objective advanced (nav_tracker.notify_objective_change). Costs a
-- flag: the work happens on the next election, inside the free-roam gate. `sig` is left alone
-- on purpose — the memo in resolve() is what makes a counter tick cheap.
function Route.invalidate()
    kind_i, cur_sig, order, pending = 0, nil, nil, nil
    mark_dirty()
end

-- Every handle here is a WORLD actor or a per-world manager/component, so all of it dies with
-- the world (nav_tracker.release_world_refs calls this on every gate close).
function Route.release()
    if route then print("[KakarotAccess] nav quest route released\n") end
    qm, index, route, pending = nil, nil, nil, nil
    qm_next, index_at, poll_next = 0, 0, 0
    sig, cur_sig, order, kind_i, miss_tries, set_tries = nil, nil, nil, 0, 0, 0
    dirty, dirty_at = false, nil     -- so mark_dirty stamps the window from HERE
    mark_dirty()
    -- `hint` is deliberately NOT cleared: it is an integer, not a handle, and it is what
    -- makes the re-resolve on the other side of a battle start at the right class.
end

return Route
