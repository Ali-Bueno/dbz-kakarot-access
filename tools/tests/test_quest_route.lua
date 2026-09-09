-- Regression: precise quest-objective routing (quest_route.lua + quest_phase.lua).
--
-- BOTH modules are real here: quest_route holds the index, the caches and the deferral
-- state machine, quest_phase the engine reads it drives (which quest, which phase, what
-- actor does it name). Only the Unreal boundary below is doubled, so the split is exercised
-- end to end rather than mocked at the seam.
--
-- The radar follows the game's navi marker, which for a "fish here" / "go through that
-- door" objective guides to the centre of a RANGE CIRCLE. quest_route resolves the actor
-- the objective actually names — the fishing bank, the door, the minigame host, the
-- campfire — so the beacon can guide there instead.
--
-- The module is real; only the Unreal boundary is doubled (ui_core's reflection helpers and
-- mem's address read). What is pinned here is the part a live session can only probe by
-- playing the exact quest: the kind dispatch, the one-scan-per-call budget, the find-list
-- index with its bounded miss retries, the main/sub selection, the phase poll, and the two
-- ways the resolver must forget everything (a world change, and the routed actor dying).
--
-- The doubled Core.member/array_of honour STRICT and can be told that the property set is
-- not available this tick, because that is the sharpest edge in the module: a strict nil
-- means EITHER "the class has no such member" OR "nothing was askable this tick", and every
-- verdict here is permanent for the objective. Believing the second is how a fishing quest
-- gets written off, or how the water volume gets committed instead of the bank.
--
-- Runs OFFLINE under the vendored Lua 5.4 (tools/run-lua-tests.ps1).

local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local fails = 0
local function check(ok, message)
    if ok then print("ok   - " .. message)
    else fails = fails + 1; print("FAIL - " .. message) end
end

-- Wall clock under test control: every backoff in the module is measured in seconds. Both
-- this and `print` are restored at the end of the file — the runner loads each test into a
-- fresh state, but a leaked override is the kind of thing that makes the NEXT test lie.
local real_clock, real_print = os.clock, print
local clock_s = 100
os.clock = function() return clock_s end
local logged = {}
print = function(message, ...)
    logged[#logged + 1] = tostring(message)
    real_print(message, ...)
end
local function logged_count(pattern)
    local n = 0
    for _, l in ipairs(logged) do if l:find(pattern) then n = n + 1 end end
    return n
end

-- ---- the doubled engine boundary -------------------------------------------------
-- `decl` is the class's DECLARED member set. Core.member refuses anything outside it,
-- which is what the real existence gate does — and the reason it exists is that fetching
-- an undeclared member on this engine is an uncatchable abort, so a test that let the
-- module read whatever it liked would pin the opposite of the contract.
--
-- `ready` (per object) and `sets_ready` (global) model the OTHER half: the property-set
-- enumeration budget is one set per tick shared by every adapter, and every set is flushed
-- on a map transition, so a STRICT read legitimately answers nil on a class it could read a
-- moment ago. Core.prop_ready is what separates that from real absence.
local next_addr = 1000
local function object(class, decl, values)
    local o = values or {}
    next_addr = next_addr + 1
    o.addr, o.class, o.valid = next_addr, class, true
    o.decl = {}
    for _, n in ipairs(decl or {}) do o.decl[n] = true end
    function o:GetClass()
        return { GetFName = function() return { ToString = function() return self.class end } end }
    end
    return o
end

local pool, scans = {}, {}
local in_free_roam = true
local sets_ready = true
local function askable(o)
    return type(o) == "table" and o.valid == true and sets_ready and o.ready ~= false
end
local Core = {
    valid = function(o) return type(o) == "table" and o.valid == true end,
    valid_ref = function(o) return type(o) == "table" end,
    prop_ready = askable,
    -- FNames are VALUES on this engine: name_str converts and the conversion is the test.
    name_str = function(v)
        if type(v) == "string" and v ~= "" then return v end
        return nil
    end,
    member = function(o, name, strict)
        if type(o) ~= "table" or o.valid ~= true then return nil end
        if strict and not askable(o) then return nil end   -- refuse, never guess
        if not o.decl[name] then return nil end
        local v = o[name]
        -- A null/dead object property comes back as an INVALID handle, never as nil.
        if type(v) == "table" and v.valid ~= true then return nil end
        return v
    end,
    array_of = function(o, name, strict)
        if type(o) ~= "table" or o.valid ~= true then return nil, nil end
        if strict and not askable(o) then return nil, nil end
        if not o.decl[name] then return nil, nil end
        local a = o[name]
        if type(a) ~= "table" then return nil, nil end
        return a, #a
    end,
    findall = function(cls)
        scans[cls] = (scans[cls] or 0) + 1
        return pool[cls] or {}
    end,
    free_roam = function() return in_free_roam end,
}
package.preload.ui_core = function() return Core end
package.preload.mem = function() return { raw_addr = function(o) return o and o.addr end } end

local Route = require("quest_route")

-- ---- the world -------------------------------------------------------------------
local MAIN_ID, MAIN_PHASE = "C02_EVT_MAIN_04002", "C02_EVT_MAIN_04002_130"
local SUB_ID, SUB_PHASE = "C02_SUB_00110", "C02_SUB_00110_020"

local function quest_actor(qid, phase)
    local q = object("C02_Quest_C", { "quest_id" }, { quest_id = qid })
    q.phase = phase
    function q:GetPhaseId() return self.phase end
    return q
end
-- The real CreatedQuestList carries a dummy BP_PdmCheck_C whose phase id is None and which
-- declares no quest_id of its own: skipping it is a live-verified requirement, not a guess.
local dummy = object("BP_PdmCheck_C", {})
function dummy:GetPhaseId() return "None" end

local main_q = quest_actor(MAIN_ID, MAIN_PHASE)
local sub_q = quest_actor(SUB_ID, SUB_PHASE)
-- Sub BEFORE main on purpose: with no sub quest navigated the resolver must pick the MAIN
-- quest out of the list, not merely the first entry that has a live phase.
local qm = object("QuestManager", { "NavigatedSubQuestId", "CreatedQuestList" },
    { NavigatedSubQuestId = "None", CreatedQuestList = { dummy, sub_q, main_q } })
pool.QuestManager = { qm }

local start_point = object("FishingStartPoint_BP_C", {})
local area = object("FishingArea_BP_C", { "FishingStartPoint" }, { FishingStartPoint = start_point })
local door = object("BP_Door_C", {})
local minigame = object("MinigameHost_BP_C", {})
local bonfire = object("FieldActionBonfireActor", {})

local function findcomp(id, owner)
    local c = object("QuestActorFindListComponent", { "m_id" }, { m_id = id })
    function c:GetOwner() return owner end
    return c
end
pool.QuestActorFindListComponent = {
    findcomp("FishingArea_BP_01_0", area),
    findcomp("Area11_Door", door),
    findcomp("Minigame_Host_01", minigame),
}

local function phase_obj(cls, qid, member, value)
    local decl, vals = { "CurrentQuestId" }, { CurrentQuestId = qid }
    if member then decl[#decl + 1] = member; vals[member] = value end
    return object(cls, decl, vals)
end
local function only_phase(cls, qid, member, value)
    pool.QuestPhase_GetFish, pool.QuestPhase_Door = nil, nil
    pool.QuestPhase_Minigame, pool.QuestPhase_Camping = nil, nil
    if cls then pool[cls] = { phase_obj(cls, qid, member, value) } end
end

local tick = 0
local function pump(n)
    local r
    for _ = 1, (n or 1) do
        tick = tick + 1
        clock_s = clock_s + 0.05
        r = Route.resolve(tick)
    end
    return r
end
local function snapshot()
    return { qmgr = scans.QuestManager, fish = scans.QuestPhase_GetFish,
             door = scans.QuestPhase_Door, mini = scans.QuestPhase_Minigame,
             camp = scans.QuestPhase_Camping, idx = scans.QuestActorFindListComponent }
end
-- The game advancing the objective. The verdict is MEMOIZED per "<quest>|<phase>", so this
-- is the only thing that makes the resolver decide again — a bare invalidate() (which is
-- what a HUD counter ticking produces) deliberately does not.
local phase_n = 130
local function bump_phase(quiet)
    phase_n = phase_n + 10
    main_q.phase = MAIN_ID .. "_" .. phase_n
    if not quiet then Route.invalidate() end
end

-- ---- GetFish: the case the feature was asked for ---------------------------------
only_phase("QuestPhase_GetFish", MAIN_ID, "TargetAreaName", "FishingArea_BP_01_0")
local r = pump(6)
check(r ~= nil and r.actor == start_point,
    "a fishing objective routes to the area's FishingStartPoint, not the water volume")
check(r ~= nil and r.kind == "GetFish" and r.label_key == "cat_fishing"
    and r.quest == "main" and r.key == tostring(start_point.addr),
    "the record carries the kind, the spoken label key, the quest side and an address key")
check(scans.QuestManager == 1 and scans.QuestPhase_GetFish == 1
    and scans.QuestActorFindListComponent == 1,
    "the resolve costs one scan of each source it actually needed")
check(scans.QuestPhase_Door == nil and scans.QuestPhase_Camping == nil,
    "the kind sweep stops at the first matching phase class")

area.FishingStartPoint = nil
bump_phase()
r = pump(4)
check(r ~= nil and r.actor == area,
    "an area with no start point falls back to the area actor rather than losing the route")

-- ---- nothing changed: nothing is scanned -----------------------------------------
local before = snapshot()
local held = pump(1)
for _ = 1, 60 do pump(1) end     -- 3 s of wall time: the phase poll fires, and finds nothing
local after = snapshot()
check(after.qmgr == before.qmgr and after.fish == before.fish and after.idx == before.idx,
    "a settled route re-scans nothing, and the phase poll itself costs no scan")
check(Route.resolve(tick) == held, "the same record is served while the objective stands still")

-- A collection counter ticking ("2/5") is an objective-TEXT change with the SAME phase id,
-- and the quest HUD signals it exactly like a real advance.
before = snapshot()
Route.invalidate()
for _ = 1, 10 do pump(1) end
after = snapshot()
check(after.fish == before.fish and after.idx == before.idx and Route.resolve(tick) == held,
    "a counter tick does not re-run the four-class sweep: the verdict is memoized per phase")

-- ---- a phase change with no objective-text change --------------------------------
bump_phase(true)                -- the phase moves, nothing signals it
clock_s = clock_s + 3           -- past the poll period
before = snapshot()
r = pump(3)
check(r ~= nil and r.actor == area and scans.QuestPhase_GetFish > before.fish,
    "the slow phase poll notices a new phase id and re-resolves without an objective signal")

-- ---- main / sub selection --------------------------------------------------------
check(r ~= nil and r.quest == "main",
    "with no sub quest navigated the MAIN quest is picked out of the created list")
qm.NavigatedSubQuestId = SUB_ID
pool.QuestPhase_Door = { phase_obj("QuestPhase_Door", SUB_ID, "TargetDoorName", "Area11_Door") }
Route.invalidate()
r = pump(4)
check(r ~= nil and r.actor == door and r.kind == "Door" and r.label_key == "cat_door"
    and r.quest == "sub",
    "a navigated sub quest routes to ITS phase's target, and reports the sub side")
check(scans.QuestPhase_GetFish > before.fish,
    "a phase class whose instance belongs to another quest does not match and is swept past")

-- The side is DERIVED from the quest id, not asserted from "no sub is navigated": a side
-- story can be the only quest with a live phase, and calling it a main quest would hand the
-- election a PRI_MAIN record that a sub-quest focus then declines.
qm.NavigatedSubQuestId = "None"
local saved_list = qm.CreatedQuestList
qm.CreatedQuestList = { dummy, sub_q }
only_phase("QuestPhase_GetFish", SUB_ID, "TargetAreaName", "FishingArea_BP_01_0")
Route.release()
r = pump(6)
check(r ~= nil and r.quest == "sub",
    "a quest whose id is not a main-story id is reported as a sub even with none navigated")
qm.CreatedQuestList = saved_list

-- ---- the other two kinds ---------------------------------------------------------
only_phase("QuestPhase_Camping", MAIN_ID, "BonfireActor", bonfire)
before = snapshot()
bump_phase()
r = pump(8)
check(r ~= nil and r.actor == bonfire and r.kind == "Camping" and r.label_key == "cat_bonfire",
    "a camping objective routes to the BonfireActor pointer the phase carries")
check(scans.QuestActorFindListComponent == before.idx,
    "a direct actor pointer needs no find-list lookup at all")

before = snapshot()
Route.release()
r = pump(4)
check(r ~= nil and r.actor == bonfire and scans.QuestPhase_Camping == before.camp + 1
    and scans.QuestPhase_GetFish == before.fish,
    "the kind that answered last is swept FIRST after a world change, not rediscovered")

only_phase("QuestPhase_Minigame", MAIN_ID, "TargetActorName", "Minigame_Host_01")
bump_phase()
r = pump(8)
check(r ~= nil and r.actor == minigame and r.kind == "Minigame"
    and r.label_key == "cat_minigame",
    "a minigame objective routes to the named host actor")

-- ---- an objective with no precise target -----------------------------------------
only_phase(nil)
bump_phase()
r = pump(8)
before = snapshot()
for _ = 1, 20 do pump(1) end
after = snapshot()
check(r == nil and after.fish == before.fish and after.door == before.door
    and after.mini == before.mini and after.camp == before.camp,
    "an objective no phase class claims yields no route and is not swept again")
before = snapshot()
Route.invalidate()
for _ = 1, 10 do pump(1) end
after = snapshot()
check(after.fish == before.fish and after.camp == before.camp,
    "and a text change on that same objective does not re-run the sweep either")

-- ---- an unaskable property set is not an absence ---------------------------------
-- The whole H1 class: one property set per tick, shared by every adapter, flushed on every
-- map transition — the first live questdump runs read nil for every name in this module.
only_phase("QuestPhase_GetFish", MAIN_ID, "TargetAreaName", "FishingArea_BP_01_0")
Route.release()
sets_ready = false
before = snapshot()
r = pump(4)
check(r == nil, "an unaskable tick yields no route rather than a wrong one")
sets_ready = true
r = pump(4)
check(r ~= nil and r.actor == area,
    "and the objective is NOT written off: it resolves as soon as the sets are askable")

before = snapshot()
sets_ready = false
for _ = 1, 30 do pump(1) end
after = snapshot()
check(after.fish == before.fish and after.idx == before.idx,
    "a permanently unaskable gate is BOUNDED: the resolver stops instead of spinning scans")
sets_ready = true

-- The user's exact symptom as a unit test: FishingStartPoint reads nil only because the
-- area's set is unavailable, and committing the area sends the player to the water.
area.FishingStartPoint = start_point
Route.release()
area.ready = false
r = pump(6)
check(r == nil, "an unreadable FishingStartPoint never commits the water volume instead")
area.ready = true
r = pump(3)
check(r ~= nil and r.actor == start_point, "once the set is askable the bank wins")

-- H2: a component scan that found components but could read no id at all must not cache an
-- EMPTY index. The proof is that the very next askable call resolves — a cached empty map
-- would have armed the 5 s miss backoff instead.
Route.release()
for _, c in ipairs(pool.QuestActorFindListComponent) do c.ready = false end
r = pump(5)
check(r == nil, "an unreadable component sweep resolves nothing yet")
for _, c in ipairs(pool.QuestActorFindListComponent) do c.ready = true end
r = pump(3)
check(r ~= nil and r.actor == start_point,
    "an index whose ids were all unreadable is never cached as empty (no 5 s miss backoff)")

-- ---- duplicate find-ids ----------------------------------------------------------
-- Live, `C02_04002_Cpl006` tags two actors. The first valid owner wins, and it is said once.
local twin = object("QuestCharacterBase_C", {})
pool.QuestActorFindListComponent[#pool.QuestActorFindListComponent + 1] =
    findcomp("FishingArea_BP_01_0", twin)
Route.release()
r = pump(6)
check(r ~= nil and r.actor == start_point,
    "a duplicated find-id keeps the FIRST valid owner, not whichever the scan yields last")
check(logged_count("duplicate find%-id") == 1,
    "the duplicate is reported exactly once, not on every rebuild")
Route.release()
pump(6)
check(logged_count("duplicate find%-id") == 1, "and not again on the next world either")

-- ---- find-list miss: bounded rebuilds, then a verdict ----------------------------
Route.release()
only_phase("QuestPhase_GetFish", MAIN_ID, "TargetAreaName", "Unloaded_Area")
before = snapshot()
pump(6)
check(scans.QuestActorFindListComponent == (before.idx or 0) + 1,
    "a find-id the index does not answer rebuilds the index exactly once, not per call")
for _ = 1, 20 do pump(1) end
check(scans.QuestActorFindListComponent == (before.idx or 0) + 1,
    "further calls inside the miss backoff do not rebuild it again")
clock_s = clock_s + 6
pump(2)
clock_s = clock_s + 6
r = pump(2)
check(r == nil and scans.QuestActorFindListComponent == (before.idx or 0) + 3,
    "the rebuild is retried past the backoff and then the objective is written off")
local settled = scans.QuestActorFindListComponent
clock_s = clock_s + 30
for _ = 1, 20 do pump(1) end
check(scans.QuestActorFindListComponent == settled,
    "a written-off objective never scans again — the miss cannot become a permanent sweep")

-- ---- the free-roam gate, and the bounded stale hold ------------------------------
Route.release()
only_phase("QuestPhase_GetFish", MAIN_ID, "TargetAreaName", "FishingArea_BP_01_0")
r = pump(6)
check(r ~= nil and r.actor == start_point, "the route is available again once the target resolves")
in_free_roam = false
before = snapshot()
Route.invalidate()
local gated = pump(5)
after = snapshot()
check(gated == r and after.qmgr == before.qmgr and after.fish == before.fish,
    "a menu or a battle serves the held route untouched and scans nothing")
check(Route.resolve(tick) == r,
    "the hold is deliberate: it stops the beacon flip-flopping between marker and spot")
clock_s = clock_s + 5
check(Route.resolve(tick) == nil,
    "but it is bounded — past the window the caller gets nil, not the old objective's spot")
in_free_roam = true

-- ---- world change ----------------------------------------------------------------
r = pump(6)
before = snapshot()
Route.release()
check(Route.resolve(tick + 1) == nil,
    "a world change drops the held route immediately — a recycled address must not be served")
r = pump(6)
check(r ~= nil and r.actor == start_point and scans.QuestManager == before.qmgr + 1
    and scans.QuestActorFindListComponent == before.idx + 1,
    "the manager and the find-list index are rebuilt from scratch on the other side")

-- ---- the routed actor dies with its sub-level ------------------------------------
before = snapshot()
-- The whole target chain dies together, which is what a sub-level streaming out does: the
-- bank AND the area it hangs off. (Killing only the bank is not this case — the resolver
-- correctly falls back to the area, which is the branch tested further up.)
start_point.valid, area.valid = false, false
check(pump(1) == nil, "a dead routed actor is never handed to the beacon")
start_point.valid, area.valid = true, true
r = pump(6)
check(r ~= nil and r.actor == start_point and scans.QuestPhase_GetFish > before.fish,
    "and the resolver sweeps again instead of answering from the memo it just invalidated")

os.clock, print = real_clock, real_print
if fails > 0 then print(fails .. " check(s) FAILED"); os.exit(1) end
print("all checks passed")
