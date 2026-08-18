-- dev_charnames.lua — DEV-ONLY probe: which field, if any, yields a DISPLAY NAME for a
-- character actor in the world?
--
-- WHY THIS EXISTS. dbz-kakarot-npc-names.md (2026-07-10) concluded there is no crash-safe
-- automatic code -> localized-name resolver, and listed the live-call paths that CRASH
-- (MessageManager:GetNounParamFromName / GetNounParam). That conclusion stands and this probe
-- does NOT retry any of it. What it does is re-ask a narrower question with two things the
-- 2026-07-10 pass did not have: the live Inspector (which showed `speakerID` (StrProperty) and
-- `m_param_id` (EnumProperty) declared on AT_CharacterBase — neither appears in that note), and
-- the nameplate widget the game itself puts over a character (Info_Name_Parent_C), which is a
-- name the game has ALREADY localized on screen.
--
-- READ-ONLY. Property reads and text reads, no function calls on character actors: the crashing
-- family on this build is reflected CALLS with FName params / struct returns, not member fetches.
--
-- EVERY name goes through the STRICT property gate (Core.member(o, n, true)). This is a
-- multi-candidate probe — most candidates are EXPECTED to be absent on any given class — which
-- is exactly the contract where failing open is a licence to fetch a name we have positive
-- reason to believe is missing, i.e. the uncatchable abort. The note is explicit that
-- `CharacterName` does not exist on QuestCharacter and that reading it there aborts. Failing
-- closed here is bounded: one skipped line, logged.
--
-- The file is written INCREMENTALLY with a step marker before each risky call, so if a read does
-- abort the trail on disk names the actor and the field that killed it (playbook: a diagnostic
-- that can die destroys the evidence it exists to collect).

local M = {}

-- Candidate name-ish fields, in the order we want them reported. Each was seen declared in a
-- live class dump (kak_class) — nothing here is guessed.
local FIELDS = {
    { prop = "UniqueId",      note = "NameProperty on QuestCharacter — raw CplNNN/NpcNNN (proven safe)" },
    { prop = "speakerID",     note = "StrProperty on AT_CharacterBase — dialogue speaker key" },
    { prop = "m_param_id",    note = "EnumProperty on AT_CharacterBase — parameter-table id" },
    { prop = "CharacterName", note = "StrProperty on AT_Character actors — CplNNNcNN (enemies only)" },
}

-- RESOLVER FUNCTIONS (added 2026-08-18). The 2026-07-10 note concluded there is no crash-safe
-- id -> localized-name call and listed the family that CRASHES: `MessageManager:GetNounParam*`,
-- i.e. reflected calls on a Blueprint object taking an FName / returning a struct. These two are
-- a DIFFERENT shape and were never found by that pass — they are not in the note at all:
--
--   UEventBlueprintFunctionLibrary::GetSpeakerFromID(const FString StringID) -> FString  AT.hpp:41256
--   UEventBlueprintFunctionLibrary::GetMessageFromID(const FString StringID) -> FString  AT.hpp:41265
--
-- FString in, FString out, on a static BlueprintFunctionLibrary — the same shape as
-- `UAT_BlueprintFunctionLibrary::GetCharacterName`, which that note records as SAFE (it returns ""
-- for every id, but it does not crash). That is the whole risk argument: same shape as a proven
-- safe call, different shape from the proven fatal one. It is an argument, not a guarantee, which
-- is why this lives in a dev probe behind a command and not in the radar.
--
-- WHY IT MATTERS: `FMessageInfoTbl` (AT.hpp:7725) has per-language columns including
-- `Speaker_esmx` / `Message_esmx`, and the game has `/Game/Message/PLAT_W/es_MX/messageData`
-- loaded. So the speaker names exist as data IN THE PLAYER'S LANGUAGE. If GetSpeakerFromID
-- answers, the radar can name characters from the game's own localized text instead of from a
-- hand-written English table.
-- Each library is paired with ONLY the functions the header shows it declares. Asking a class for
-- a function it does not have is the same uncatchable abort as an undeclared property, and this is
-- a multi-candidate probe — the contract where failing open is a licence to fetch a name we have
-- positive reason to believe is absent. So: no cross-probing, ever.
local RESOLVERS = {
    { path = "/Script/AT.Default__EventBlueprintFunctionLibrary",
      fns  = { "GetSpeakerFromID", "GetMessageFromID" } },          -- AT.hpp:41256, 41265
    { path = "/Script/AT.Default__AT_BlueprintFunctionLibrary",
      fns  = { "GetCharacterName" } },                              -- AT.hpp:29953 (known: returns "")
}

-- Ids probed even when the world walk finds nothing, so a run always tests the CALL itself.
-- speakerID is UniqueId + a variant letter (Cpl003 -> Cpl003A), proven on 2026-08-15.
local EXTRA_IDS = { "Cpl002", "Cpl002A", "Cpl002B", "Cpl019A", "Npc004A",
                    -- class-name-derived ids for the enemy branch (2026-08-18): speakerID is
                    -- empty there, so these come from AT_Character_cplNNN..._BP_C.
                    "Cpl003A", "Cpl057A", "Cpl064A", "Cpl065A",
                    -- UATEnemiesBaseBehaviour.EnemiesBaseId read live off
                    -- BP_EnemiesBaseFreezer_C_0 — testing whether a base id has a name row.
                    "142", "EnemiesBase142", "EB142" }

-- QuestCharacter ONLY, by default (narrowed 2026-08-15 after this probe killed the game).
--
-- The first version also walked `AT_CharacterBase`, the common base that includes every combat
-- character. That returned ~60 live instances and the probe read four members on each. It ran
-- once cleanly and then took the process down on the second run two minutes later, with a
-- dialogue and a batch of newly-streamed `AT_Character_cpl0xx` actors in between: enumerating a
-- LIVE, streaming actor list and then dereferencing the handles is the most dangerous thing that
-- can be done on this engine, and `Core.valid`'s SEH pre-check narrows that window without
-- closing it — an actor freed between the enumeration and the fetch is still fatal.
--
-- QuestCharacters are level-placed and stable (4 instances here, not 60), and they are also the
-- ones the radar actually needs to name. They already answered the question this probe was
-- written for: `speakerID` = `UniqueId` + a variant letter (Cpl003 -> Cpl003A).
-- Set WALK_COMBAT_CHARACTERS = true only with a throwaway save and a reason.
local WALK_COMBAT_CHARACTERS = false
local CLASSES = WALK_COMBAT_CHARACTERS
    and { "AT_CharacterBase", "QuestCharacter" }
    or  { "QuestCharacter" }

-- Level-placed NPC counts are small; a cap this low bounds the exposure window rather than
-- merely bounding the output.
local MAX_PER_CLASS = 12

-- The nameplate the game draws over a character. Two slots, each an Info_Name_C whose text nodes
-- carry a name the game has already resolved AND localized.
local PLATE_HOST  = "Info_Name_Parent_C"
local PLATE_SLOTS = { "Ins_Info00", "Ins_Info01" }
local PLATE_TEXTS = { "Txt_Name", "Txt_Name01", "Txt_Name02", "Txt_Name03" }
-- Native-side twins on AT_UIInfoNameCore (same widget, reflected under the C++ names).
local PLATE_NATIVE = { "NameTxt", "NameTxt_Large", "PopularNameTxt", "PopularNameTxt_Large" }

local function dump_path()
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    return dir .. "\\dumps\\dump_char_names.txt"
end

-- One writer with a flush per line: the point of the step marker is that it is ON DISK before
-- the call it describes runs.
local function writer(f)
    return function(line)
        f:write(line, "\n")
        f:flush()
    end
end

-- Read a widget's text through whichever accessor answers. Core.text_of covers this game's
-- wrapper text nodes; the raw Text member is the fallback for plain UMG nodes.
local function text_of_node(Core, node)
    if not Core.valid(node) then return nil end
    local t = Core.text_of(node)
    if t and t ~= "" then return t end
    return Core.name_str(Core.member(node, "Text", true))
end

function M.run()
    local okc, Core = pcall(require, "ui_core")
    if not okc or not Core then return end
    local okm, Mem = pcall(require, "mem")   -- module is "mem"; the DLL it wraps is mem_bridge

    ExecuteInGameThread(function()
        -- CRASH-TRAIL MARK. Without it this probe is invisible to the black box and a death
        -- inside it is blamed on whichever loop marked last — which is exactly what happened on
        -- 2026-08-15: the trail read as an ordinary tick and the dump file was the only clue.
        if okm and Mem and Mem.mark then pcall(Mem.mark, "dev.charnames") end
        local f = io.open(dump_path(), "a")
        if not f then return end
        local w = writer(f)

        -- RADAR TARGET LIST (2026-08-18). The light replacement for the target section of
        -- `navdump`, which FREEZES the game (see the crash-bug note) and so cannot be used on a
        -- live session. This calls `Nav.list_targets()` — the very same build R3 triggers, with its
        -- own `Mem.mark("nav.sweep")` — so it adds no exposure the picker does not already have,
        -- and it answers the only question that matters when something is "not in its category":
        -- which group did each actor actually land in.
        local nok, Nav = pcall(require, "nav_tracker")
        w("")
        w("======== radar target list ========")
        if not nok or not Nav or not Nav.list_targets then
            w("(nav_tracker unavailable: " .. tostring(Nav) .. ")")
        else
            local cats
            w("   step: Nav.list_targets()")
            local lok = pcall(function() cats = Nav.list_targets() end)
            if not lok or type(cats) ~= "table" then
                w("   (list_targets raised or returned nothing — field not ready?)")
            elseif #cats == 0 then
                w("   (EMPTY: field_ready false, no pawn, or no targets at all)")
            else
                for _, c in ipairs(cats) do
                    w(("-- group %-14s (%d items)"):format(tostring(c.key), #(c.items or {})))
                    for i, it in ipairs(c.items or {}) do
                        if i > 12 then w("      … more") break end
                        local cls = "?"
                        pcall(function() cls = it.actor:GetClass():GetFName():ToString() end)
                        w(("      %-28s %-22s %6.0fm  %s"):format(
                            tostring(it.name or "-"), tostring(it.noun), (it.dist or 0) / 100, cls))
                    end
                end
            end
        end
        w("======== end radar target list ========")

        w("")
        w("======== char name probe ========")
        w("(strict property gate on every field; a 'gate' line means the class does not")
        w(" declare that name, or its property set was not available this tick)")
        for _, fd in ipairs(FIELDS) do
            w(("  candidate %-14s %s"):format(fd.prop, fd.note))
        end

        local seen_ids = {}
        for _, cls in ipairs(CLASSES) do
            w("")
            w("-- class " .. cls)
            local all
            w("   step: FindAllOf(" .. cls .. ")")
            pcall(function() all = FindAllOf(cls) end)
            if not all then
                w("   (FindAllOf returned nothing)")
            else
                local n = 0
                for _, o in pairs(all) do
                    if n >= MAX_PER_CLASS then break end
                    if Core.valid(o) then
                        n = n + 1
                        local full = "?"
                        pcall(function() full = o:GetFullName() end)
                        -- Actor path only: the level name is long and the leaf (Cpl003_S050) is
                        -- the part that identifies the character.
                        w(("   [%d] %s"):format(n, full:match("[^%.]+$") or full))
                        for _, fd in ipairs(FIELDS) do
                            w(("        step: %s.%s"):format(cls, fd.prop))
                            local v = Core.member(o, fd.prop, true)
                            if v == nil then
                                w(("        %-14s (gate)"):format(fd.prop))
                            else
                                -- THREE outcomes, kept distinct on purpose. The first cut folded
                                -- them together through Core.name_str, which returns nil both for
                                -- an EMPTY string and for one it could not read — so a genuinely
                                -- empty `CharacterName` printed as a raw FString handle and looked
                                -- like a failure. "the field exists and is blank" and "the field
                                -- would not convert" lead to completely different conclusions.
                                local s
                                if type(v) == "userdata" then
                                    local ok, r = pcall(function() return v:ToString() end)
                                    if ok and type(r) == "string" then
                                        s = (r == "") and "(empty)" or r
                                    else
                                        s = "(not readable: " .. tostring(v) .. ")"
                                    end
                                else
                                    s = tostring(v)
                                end
                                w(("        %-14s %s"):format(fd.prop, s))
                                -- keep the raw value for the resolver section below
                                if s ~= "(empty)" and not s:match("^%(") then
                                    seen_ids[#seen_ids + 1] = s
                                end
                            end
                        end
                    end
                end
                w(("   (%d valid instances reported)"):format(n))
            end
        end

        -- RESOLVER SECTION. Every call is preceded by a step marker on disk, so if one of them is
        -- the fatal shape after all, the trail names the exact library and function that killed it.
        w("")
        w("-- id -> name resolvers")
        for _, res in ipairs(RESOLVERS) do
            local path = res.path
            w("   step: StaticFindObject(" .. path .. ")")
            local lib
            pcall(function() lib = StaticFindObject(path) end)
            if not Core.valid(lib) then
                w("   " .. path .. " -> not found")
            else
                w("   " .. path .. " -> found")
                -- Probe the ids the walk actually saw first, then the fixed list.
                local ids = {}
                for _, v in ipairs(seen_ids) do ids[#ids + 1] = v end
                for _, v in ipairs(EXTRA_IDS) do ids[#ids + 1] = v end
                for _, fn in ipairs(res.fns) do
                    for _, id in ipairs(ids) do
                        w(("      step: %s(\"%s\")"):format(fn, id))
                        local out_s, called
                        pcall(function()
                            local r = lib[fn](lib, id)
                            called = true
                            if type(r) == "string" then
                                out_s = r
                            elseif r ~= nil then
                                local ok2, t = pcall(function() return r:ToString() end)
                                out_s = (ok2 and type(t) == "string") and t or nil
                            end
                        end)
                        if not called then
                            w(("      %-18s(%-10s) -> (no such function here)"):format(fn, id))
                        elseif out_s == nil then
                            w(("      %-18s(%-10s) -> (unreadable return)"):format(fn, id))
                        elseif out_s == "" then
                            w(("      %-18s(%-10s) -> (empty)"):format(fn, id))
                        else
                            w(("      %-18s(%-10s) -> \"%s\"   <== HIT"):format(fn, id, out_s))
                        end
                    end
                end
            end
        end

        -- ITEM IDS (2026-08-18). GetMessageFromID resolved CHARACTER ids straight to localized
        -- text, and it is a general message-id lookup, not a character one — so the same call may
        -- name collectibles. Collect whatever ids the world actors carry and feed them in.
        -- Sources, all already read by nav_tracker so none of them is a new risk:
        --   PlacementObjectInfo -> ItemTableComponent.FieldItemDropData.FixedId/NormalId (FName)
        --   AccessPointItemBase -> TreasureSaveName (e.g. L11_DMEDAL_310) — a save key, probably
        --     not a name key, but it costs one call to find out.
        w("")
        w("-- item ids")
        local item_ids = {}
        for _, spec in ipairs({
            -- AItemStaticActor (AT.hpp:15780) declares `FName ItemId`, and
            -- ADragonBallStaticActor (AT.hpp:15370) derives from it — so a Dragon Ball carries its
            -- own item id as a reflected FName. That is the id worth resolving: the drop-table and
            -- save keys below were tried on 2026-08-18 and every one came back empty, because they
            -- are not message ids. Neither class was loaded on the Namek map where this was written,
            -- so ItemId has never actually been read — run this near a Dragon Ball or any static
            -- item pickup. Scan the DERIVED class too: FindAllOf on a native base returns nothing
            -- on this game when a subclass exists (the community-board lesson).
            { cls = "DragonBallStaticActor", kind = "itemid" },
            { cls = "ItemStaticActor", kind = "itemid" },
            { cls = "PlacementObjectInfo", kind = "drop" },
            { cls = "AccessPointItemBase", kind = "treasure" },
        }) do
            w("   step: FindAllOf(" .. spec.cls .. ")")
            local all
            pcall(function() all = FindAllOf(spec.cls) end)
            local n = 0
            for _, a in pairs(all or {}) do
                if n >= 8 then break end
                if Core.valid(a) then
                    n = n + 1
                    if spec.kind == "itemid" then
                        w("      step: " .. spec.cls .. ".ItemId")
                        local v = Core.name_str(Core.member(a, "ItemId", true))
                        if v and v ~= "None" then
                            w(("      itemid %s"):format(v))
                            item_ids[#item_ids + 1] = v
                        else
                            w("      itemid (gate/empty)")
                        end
                    elseif spec.kind == "treasure" then
                        w("      step: " .. spec.cls .. ".TreasureSaveName")
                        local v = Core.name_str(Core.member(a, "TreasureSaveName", true))
                        if v then
                            w(("      treasure %s"):format(v))
                            item_ids[#item_ids + 1] = v
                        end
                    else
                        w("      step: " .. spec.cls .. ".ItemTableComponent")
                        local comp = Core.member(a, "ItemTableComponent", true)
                        if Core.valid(comp) then
                            local d = Core.member(comp, "FieldItemDropData", true)
                            -- valid_REF: an FStruct handle. Core.valid would call GetAddress on it,
                            -- which UE4SS raises THROUGH pcall (see nav_tracker.drop_item_name).
                            if Core.valid_ref(d) then
                                for _, fld in ipairs({ "FixedId", "NormalId" }) do
                                    w("      step: FieldItemDropData." .. fld)
                                    local v
                                    pcall(function() v = Core.name_str(d[fld]) end)
                                    if v and v ~= "None" then
                                        w(("      drop %-10s %s"):format(fld, v))
                                        item_ids[#item_ids + 1] = v
                                    end
                                end
                            end
                        end
                    end
                end
            end
            w(("   (%d %s actors read)"):format(n, spec.cls))
        end

        w("")
        w("-- item id -> name")
        for _, res in ipairs(RESOLVERS) do
            local lib
            pcall(function() lib = StaticFindObject(res.path) end)
            if Core.valid(lib) then
                for _, fn in ipairs(res.fns) do
                    for _, id in ipairs(item_ids) do
                        w(("      step: %s(\"%s\")"):format(fn, id))
                        local out_s, called
                        pcall(function()
                            local r = lib[fn](lib, id)
                            called = true
                            if type(r) == "string" then
                                out_s = r
                            elseif r ~= nil then
                                local ok2, t = pcall(function() return r:ToString() end)
                                out_s = (ok2 and type(t) == "string") and t or nil
                            end
                        end)
                        if called and out_s and out_s ~= "" then
                            w(("      %-18s(%-18s) -> \"%s\"   <== ITEM HIT"):format(fn, id, out_s))
                        else
                            w(("      %-18s(%-18s) -> (empty)"):format(fn, id))
                        end
                    end
                end
            end
        end

        -- The nameplate: whatever it holds RIGHT NOW is a name the game itself resolved.
        w("")
        w("-- nameplate " .. PLATE_HOST)
        local hosts
        w("   step: FindAllOf(" .. PLATE_HOST .. ")")
        pcall(function() hosts = FindAllOf(PLATE_HOST) end)
        for _, host in pairs(hosts or {}) do
            if Core.valid(host) then
                w("   host on_screen=" .. tostring(Core.on_screen(host)))
                for _, slot in ipairs(PLATE_SLOTS) do
                    w("        step: slot " .. slot)
                    local s = Core.member(host, slot, true)
                    if not Core.valid(s) then
                        w(("        %-22s (gate/invalid)"):format(slot))
                    else
                        w(("        %-22s on_screen=%s"):format(slot, tostring(Core.on_screen(s))))
                        for _, tn in ipairs(PLATE_TEXTS) do
                            w("           step: " .. slot .. "." .. tn)
                            local node = Core.member(s, tn, true)
                            local txt = text_of_node(Core, node)
                            if txt then w(("           %-14s \"%s\""):format(tn, txt)) end
                        end
                        for _, tn in ipairs(PLATE_NATIVE) do
                            w("           step: " .. slot .. "." .. tn)
                            local node = Core.member(s, tn, true)
                            local txt = text_of_node(Core, node)
                            if txt then w(("           %-14s \"%s\""):format(tn, txt)) end
                        end
                    end
                end
            end
        end

        w("======== end char name probe ========")
        f:close()
    end)
end

return M
