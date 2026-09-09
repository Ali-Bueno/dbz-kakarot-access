-- dev_questdump.lua — DEV-ONLY probe: what does the LIVE quest system expose that a radar could
-- route on? (quest-objective-routing note, 2026-09-08)
--
-- QUESTIONS IT ANSWERS, in file order:
--   1. QuestManager scalars (NavigatedSubQuestId) and the CreatedQuestList: one line per live quest
--      actor with its class and quest_id, then — behind a step marker — the no-arg reflected calls
--      GetPhaseId() / GetPhaseIdImmediate(), whose FName return is the first reflected CALL this
--      mod makes on a quest actor.
--   2. The live phase objects, one FindAllOf per actionable phase class, with the plain FName /
--      object members each class declares (AT.hpp: TargetAreaName, TargetDoorName, BonfireActor…).
--   3. Every QuestActorFindListComponent: its m_id and the owner actor's class + position. This is
--      the name -> actor route that needs NO FName marshalling: match the phase's target name
--      against m_id and take the owner.
--
-- WHAT IT DELIBERATELY NEVER TOUCHES. Every TMap (CurrentQuestMap, QuestPhaseMap, FoundActorList,
-- TargetActorsId, TargetFishes). RE-UE4SS 3.0.1 registers FMapProperty only as a TYPE TABLE
-- (LuaMod.cpp:797) and has no value pusher for it, so a member read reaches LuaUObject.cpp's
-- "Tried getting without a registered handler" throw — the pcall-piercing family. Verified in the
-- source before this file was written; do not "just try it".
--
-- Reflected calls WITH an FName parameter (GetActorByFindListId, GetQuestPhaseParamObjectName) are
-- behind CALL_FNAME_PARAM, default OFF: the crashing family on this build is FName-param / struct
-- return marshalling (npc-names note, 2026-07-10), and the find-list scan in section 3 makes the
-- call unnecessary for routing. Flip it only to measure, in free roam, with nothing else at stake.
--
-- EVERY member read is STRICT (Core.member(o, n, true)). This is a multi-class probe: `quest_id`
-- is declared on each C02_*_C quest class and NOT on BP_PdmCheck_C (also a Quest_General), and
-- the fail-open gate fetched it there on 2026-09-08 — an undeclared name yields a null UObject
-- wrapper whose ToString raises "UObject instance is nullptr", which PIERCES pcall and killed the
-- dev channel for the session. Strict fails closed and bounded: a skipped line, logged.
--
-- Written INCREMENTALLY with a step marker before every risky call (playbook: a diagnostic that
-- can die must leave a trail naming the line that killed it).

local M = {}

local CALL_FNAME_PARAM = false

-- Phase classes a player acts on (EQUEST_PHASE_TYPE, AT_enums.hpp:7521), each with the plain
-- members its class declares in AT.hpp — nothing here is guessed, and no TMap is listed.
local PHASE_CLASSES = {
    { cls = "QuestPhase_Arrival",        names = {} },
    { cls = "QuestPhase_SearchItem",     names = {} },
    { cls = "QuestPhase_GetItem",        names = {} },
    { cls = "QuestPhase_TalkNpc",        names = {} },
    { cls = "QuestPhase_GetItemTalkNpc", names = {} },
    { cls = "QuestPhase_GetFish",        names = { "TargetAreaName" } },
    { cls = "QuestPhase_Hunting",        names = {}, bools = { "bCompleted" } },
    { cls = "QuestPhase_Door",           names = { "TargetDoorName" } },
    { cls = "QuestPhase_Minigame",       names = { "MinigameName", "TargetActorName" } },
    { cls = "QuestPhase_Camping",        names = {}, objects = { "BonfireActor" } },
    { cls = "QuestPhase_Battle",         names = {} },
    { cls = "QuestPhase_DestroyEnemiesBase", names = {} },
}

local function dump_path()
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    return dir .. "\\dumps\\dump_quest_state.txt"
end

local function writer(f)
    return function(line)
        f:write(line, "\n")
        f:flush()
    end
end

local function cls_of(Core, o)
    if not Core.valid(o) then return "?" end
    local n
    pcall(function() n = o:GetClass():GetFName():ToString() end)
    return n or "?"
end

local function full_name(o)
    local n
    pcall(function() n = o:GetFullName() end)
    return n or "?"
end

local function pos_of(Core, actor)
    if not Core.valid(actor) then return "no actor" end
    local ok, loc = pcall(function() return actor:K2_GetActorLocation() end)
    if ok and loc then return string.format("%.0f %.0f %.0f", loc.X, loc.Y, loc.Z) end
    return "pos unreadable"
end

-- A reflected no-arg call, step-marked, returning the FName as text or the failure.
local function call_name(Core, w, o, fn)
    w("   step: call " .. fn)
    local ok, r = pcall(function() return o[fn](o) end)
    if not ok then return "raised: " .. tostring(r) end
    return Core.name_str(r) or ("(" .. type(r) .. ")")
end

local function section_manager(Core, w)
    w("======== QuestManager ========")
    w("   step: findall QuestManager")
    local mgr
    for _, o in pairs(Core.findall("QuestManager")) do
        if Core.valid(o) then mgr = o break end
    end
    if not mgr then return w("   (no QuestManager — not in a playable world)") end
    Core.prop_warm(mgr)
    w("   NavigatedSubQuestId = " .. tostring(Core.name_str(Core.member(mgr, "NavigatedSubQuestId", true))))
    w("   step: array CreatedQuestList")
    local arr, n = Core.array_of(mgr, "CreatedQuestList")
    if not arr then return w("   (CreatedQuestList unreadable)") end
    w("   CreatedQuestList n=" .. tostring(n))
    for i = 1, (n or 0) do
        local q = arr[i]
        if Core.valid(q) then
            Core.prop_warm(q)
            w(string.format("   [%d] %s  quest_id=%s  bDisabledNavigation=%s", i, cls_of(Core, q),
                tostring(Core.name_str(Core.member(q, "quest_id", true))),
                tostring(Core.member(q, "bDisabledNavigation", true))))
            w("       GetPhaseId          = " .. call_name(Core, w, q, "GetPhaseId"))
            w("       GetPhaseIdImmediate = " .. call_name(Core, w, q, "GetPhaseIdImmediate"))
        else
            w(string.format("   [%d] (invalid entry)", i))
        end
    end
    return mgr
end

local function section_phases(Core, w)
    w("")
    w("======== live phase objects ========")
    for _, spec in ipairs(PHASE_CLASSES) do
        w("   step: findall " .. spec.cls)
        local hits = 0
        for _, p in pairs(Core.findall(spec.cls)) do
            if Core.valid(p) then
                hits = hits + 1
                Core.prop_warm(p)
                w(string.format("   %s  %s  CurrentQuestId=%s", spec.cls, full_name(p),
                    tostring(Core.name_str(Core.member(p, "CurrentQuestId", true)))))
                for _, nm in ipairs(spec.names) do
                    w(string.format("       %s = %s", nm, tostring(Core.name_str(Core.member(p, nm, true)))))
                end
                for _, nm in ipairs(spec.bools or {}) do
                    w(string.format("       %s = %s", nm, tostring(Core.member(p, nm, true))))
                end
                for _, nm in ipairs(spec.objects or {}) do
                    local o = Core.member(p, nm, true)
                    w(string.format("       %s = %s @ %s", nm, cls_of(Core, o), pos_of(Core, o)))
                end
            end
        end
        if hits == 0 then w("   (none)") end
    end
end

local function section_findlist(Core, w, mgr)
    w("")
    w("======== QuestActorFindListComponent (m_id -> owner) ========")
    w("   step: findall QuestActorFindListComponent")
    local first_id
    for _, c in pairs(Core.findall("QuestActorFindListComponent")) do
        if Core.valid(c) then
            Core.prop_warm(c)
            local id = Core.name_str(Core.member(c, "m_id", true))
            local owner
            pcall(function() owner = c:GetOwner() end)
            w(string.format("   %-28s -> %s @ %s", tostring(id), cls_of(Core, owner), pos_of(Core, owner)))
            first_id = first_id or id
        end
    end
    if CALL_FNAME_PARAM and mgr and first_id then
        w("   step: call GetActorByFindListId(FName " .. first_id .. ")   [CALL_FNAME_PARAM]")
        local ok, a = pcall(function() return mgr:GetActorByFindListId(FName(first_id)) end)
        w("   -> " .. (ok and (cls_of(Core, a) .. " @ " .. pos_of(Core, a)) or ("raised: " .. tostring(a))))
    end
end

function M.run()
    local okc, Core = pcall(require, "ui_core")
    if not okc or not Core then return end
    local okm, Mem = pcall(require, "mem")
    ExecuteInGameThread(function()
        if okm and Mem and Mem.mark then pcall(Mem.mark, "dev.questdump") end
        local f = io.open(dump_path(), "a")
        if not f then return end
        local w = writer(f)
        w("")
        w("== quest state dump @ " .. os.date("%H:%M:%S") .. " ==")
        local ok, err = pcall(function()
            local mgr = section_manager(Core, w)
            section_phases(Core, w)
            section_findlist(Core, w, mgr)
        end)
        w(ok and "== end ==" or ("== raised: " .. tostring(err) .. " =="))
        f:close()
    end)
end

return M
