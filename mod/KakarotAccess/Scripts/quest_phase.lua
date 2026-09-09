-- Quest / phase DISCOVERY: which quest the game is navigating, which phase object is live
-- for it, and what actor that phase names.
--
-- This is the half of precise objective routing that talks to the engine. `quest_route.lua`
-- holds the other half — the find-id index, the caches and the state machine that decides
-- WHEN to ask and what to do with the answer — and is the only caller. Nothing here keeps
-- state: every function takes its handles and answers about this instant, so the caller can
-- cache and invalidate in one place.
--
-- HOW THE GAME EXPOSES IT — all live-verified 2026-09-08; offsets, AT.hpp lines and the
-- capture behind each fact are in
-- reference/dbz-kakarot/notes/dbz-kakarot-quest-objective-routing.md. In short: the
-- QuestManager component names the navigated quest, Quest_General:GetPhaseId() names its
-- phase, the live objective is a TRANSIENT object whose CLASS IS THE KIND (carrying
-- CurrentQuestId), and each kind names its target as an FName.
--
-- FORBIDDEN HERE, and not as a style preference: every TMap member (TargetFishes,
-- TargetActorsId, FoundActorList, CurrentQuestMap, QuestPhaseMap) and every UFunction taking
-- an FName (GetActorByFindListId). UE4SS 3.0.1 registers FMapProperty with no value pusher,
-- so that read is the uncatchable abort. Only the plain FName / object members below.
--
-- A STRICT NIL IS TWO DIFFERENT ANSWERS, and the caller draws PERMANENT conclusions from
-- them, so every function here says WHICH one it got. `Core.member(o, n, true)` returns nil
-- both when the class does not declare `n` AND when the property set could not be enumerated
-- this tick — the budget is ONE set per tick shared by every adapter, and every set is
-- flushed on a map transition. That second case is not exotic: the first live `questdump`
-- runs read nil for every name below. Believing it would skip a phase class for the rest of
-- an objective, or commit the water volume because `FishingStartPoint` was merely unaskable.
-- `Core.prop_ready` is what separates them. (`Core.prop_warm` is dev-only: never from a loop.)

local Core = require("ui_core")

local Phase = {}

-- The FName spelling of "empty". The game answers this for an unset id, so it is a value to
-- reject, not a name to look up.
local NONE = "None"

-- Objective kinds whose phase class names a precise target. `member` is an FName the caller
-- resolves through its find-list index; `actor_member` is a direct object pointer (Camping is
-- the only one). `stand` is a finer spot ON the resolved actor — AFishingArea.FishingStartPoint
-- is where the player actually casts from, while the area itself is the whole water volume.
-- Offsets/types for every name are in the routing note (AT.hpp lines cited there).
Phase.KINDS = {
    { kind = "GetFish",  cls = "QuestPhase_GetFish",  member = "TargetAreaName",
      stand = "FishingStartPoint", label = "cat_fishing" },
    { kind = "Door",     cls = "QuestPhase_Door",     member = "TargetDoorName",
      label = "cat_door" },
    { kind = "Minigame", cls = "QuestPhase_Minigame", member = "TargetActorName",
      label = "cat_minigame" },
    { kind = "Camping",  cls = "QuestPhase_Camping",  actor_member = "BonfireActor",
      label = "cat_bonfire" },
}

-- The main quest's id convention, observed live (C02_EVT_MAIN_04002). CreatedQuestList can
-- hold several quests with a live phase — the main story plus every accepted side story —
-- and NavigatedSubQuestId only names the SUB one, so when the game is navigating "not a sub
-- quest" this is what picks the main story out of the list. It also decides which SIDE the
-- answer is reported as: "NavigatedSubQuestId is None" does not prove the winner is a main
-- quest, and asserting that would hand the radar's election a PRI_MAIN record for a side
-- story and have it declined by a sub-quest focus. A quest whose id does not follow the
-- convention still qualifies (first one wins) and is reported as a sub; the phase match then
-- either confirms it or yields no route at all, so a wrong pick costs a missing override.
local MAIN_TOKEN = "_MAIN_"

-- STRICT read of an FName-ish member, as a Lua string. Every read in this file and in the
-- caller's find-id index goes through here: these are members of several different classes,
-- most of which do not declare most of the names, and an undeclared fetch is the uncatchable
-- abort. Second return: is a NIL answer TRUSTWORTHY? (see the header — this is the whole rule)
function Phase.name_of(o, member)
    local v = Core.name_str(Core.member(o, member, true))
    if v == NONE then v = nil end
    if v ~= nil then return v, true end
    return nil, Core.prop_ready(o)
end

-- "<quest id>_<phase number>" for a live quest, or nil for the dummy / an unreadable one. A
-- reflected no-arg CALL, not a member read, so the property gate does not apply to it.
local function phase_id_of(q)
    local id
    if not pcall(function() id = q:GetPhaseId() end) then return nil end
    id = Core.name_str(id)
    if id == nil or id == NONE then return nil end
    return id
end

-- The quest the game is currently navigating, off the caller's cached QuestManager: its
-- actor, phase id, side ("main"/"sub") and quest id. No scans. 5th return on failure: "the
-- QuestManager could not be ASKED", which the caller must not read as "there is no quest".
function Phase.navigated(qm)
    local sub, sub_ok = Phase.name_of(qm, "NavigatedSubQuestId")
    if not sub and not sub_ok then return nil, nil, nil, nil, true end
    local arr, n = Core.array_of(qm, "CreatedQuestList", true)
    -- array_of's strict gate refuses on an unavailable OR PARTIAL set, so the same ambiguity
    -- applies: ask before believing this quest manager has no quest list.
    if not arr or not n then return nil, nil, nil, nil, not Core.prop_ready(qm) end
    local bq, bphase, bid, bmain
    for i = 1, n do
        local q = arr[i]
        if Core.valid(q) then
            local phase = phase_id_of(q)
            -- `quest_id` is declared only on the concrete C02_*_C classes, so it is a strict
            -- read that may legitimately answer nil; the phase id carries the same value with
            -- the phase number appended, so an unaskable set costs nothing here.
            local qid = phase and (Phase.name_of(q, "quest_id") or phase:match("^(.+)_%d+$"))
            if qid then
                if sub then
                    if qid == sub then return q, phase, "sub", qid end
                else
                    local is_main = qid:find(MAIN_TOKEN, 1, true) ~= nil
                    if bq == nil or (is_main and not bmain) then
                        bq, bphase, bid, bmain = q, phase, qid, is_main
                    end
                end
            end
        end
    end
    -- The game navigates a sub quest whose actor is not in the list: answer nothing rather
    -- than routing to the main quest, which is not what the player is being guided to.
    if sub then return nil end
    if bq then return bq, bphase, bmain and "main" or "sub", bid end
    return nil
end

-- What this phase names: a direct actor, or a find-id for the caller to resolve. 3rd return:
-- whether a doubly-nil answer is trustworthy (the phase genuinely names nothing) or merely
-- unaskable this tick.
function Phase.target_of(spec, p)
    if spec.actor_member then
        local a = Core.member(p, spec.actor_member, true)
        if Core.valid(a) then return a, nil, true end
        return nil, nil, Core.prop_ready(p)
    end
    local id, ok = Phase.name_of(p, spec.member)
    return nil, id, ok
end

-- The live phase object of this kind for this quest. COSTS ONE FindAllOf — the caller owns
-- the scan budget and spends it before calling.
--
-- Two instances can exist for one quest (the previous phase and the current one), and nothing
-- reflected says which phase the game is on — so prefer one that actually NAMES a target and
-- keep a bare match as the fallback. Second return: the class HAS instances but none of them
-- could be asked, so the caller must retry this kind rather than sweep past it for good.
function Phase.find(spec, qid)
    local fallback, unread, blocked = nil, nil, false
    for _, p in pairs(Core.findall(spec.cls)) do
        if Core.valid(p) then
            local cur, cur_ok = Phase.name_of(p, "CurrentQuestId")
            if cur == nil and not cur_ok then
                blocked = true
            elseif cur == qid then
                local a, id, ok = Phase.target_of(spec, p)
                if a or id then return p end
                -- An instance whose target could not be ASKED outranks one that genuinely
                -- names nothing: the caller defers on it instead of writing the objective off.
                if ok then fallback = fallback or p else unread = unread or p end
            end
        end
    end
    if unread then return unread end
    if fallback then return fallback end
    if blocked then return nil, true end
    return nil
end

-- The finer spot ON a resolved target, when the kind has one: the fishing bank rather than
-- the water. Returns the actor to guide to, plus whether a nil read was trustworthy.
--
-- THAT SECOND RETURN IS THE USER'S REPORTED SYMPTOM. A strict nil because the area's property
-- set was merely unavailable this tick commits the WATER VOLUME as the objective — and the
-- caller commits verdicts for the whole objective, so "the radar does not take me to the
-- fishing spot" would be reproduced by the very fix meant to end it.
function Phase.stand(spec, a)
    if not spec.stand then return a, true end
    local sp = Core.member(a, spec.stand, true)
    if Core.valid(sp) then return sp, true end
    return a, Core.prop_ready(a)
end

-- The order a sweep should try the kinds in: whichever kind answered LAST goes first.
-- `hint` is a plain integer, so unlike every handle in this feature it survives a world
-- change — and after a battle or a cutscene (which drops everything else) the objective is
-- almost always still the same one. Starting the round-robin from scratch there costs up to
-- three extra scans, i.e. three more elections during which the game's own marker wins and
-- the radar announces the range circle before correcting itself to the real spot. A stale
-- hint costs exactly one scan that does not match.
function Phase.sweep_order(hint)
    local o = {}
    if hint then o[1] = hint end
    for i = 1, #Phase.KINDS do
        if i ~= hint then o[#o + 1] = i end
    end
    return o
end

return Phase
