-- ui_directory.lua — the game's own screen registry, read instead of scanned.
--
-- Screen detection used to be per-class FindAllOf polling, and one FindAllOf costs ~65ms
-- on this game (Ctrl+F5 scan stats, 2026-07-15) — the absent-class rescan cycle alone was
-- ~30% of the game thread, and a just-opened submenu waited out a multi-second backoff
-- before it read. But the game already tracks every top-level screen in NAMED POINTER
-- FIELDS on a handful of persistent manager objects (CXX header dump, field names
-- verified per-class against AT.hpp on 2026-07-15):
--
--   PlayerController.MyHUD (Engine.hpp:7011) -> AAT_GameHUD (AT.hpp:14676)
--     .UIFieldManager  (AT.hpp:32792) — one field per field/shop/map/quest/info screen
--     .UIBattleManager (AT.hpp:30911) — battle HUD, palette, results, QTEs
--     .UICommManager   (AT.hpp:31730) — community board / emblem list
--     .UIPause                        — the pause screen
--   GameMode component UMenuManager (AT.hpp:41837) — one UBaseMenu controller per pause
--     SUBMENU (items, palette, skill tree/custom, characters, save/load, options, tips,
--     party, status), each holding its widget pointer. These are the screens that had no
--     manager field on the HUD — and the ones that were slowest to read.
--   GameInstance (BP_ATGameInstance_C) — WindowManager (system/game/number dialogs),
--     LoadingScreen (UAT_GameInstance: 0x1FB0 / 0x1F88).
--
-- A directory hit answers "does this screen exist right now?" with 2-3 guarded property
-- reads: a null field means the screen was never created (no scan needed); a valid one IS
-- the instance the adapters want. ui_core consults this first; unmapped classes keep the
-- budgeted FindAllOf path, so the worst case is the old behavior. NOTE: while a chain's
-- OWNER is reachable, a mapped class never falls back to scans — that is what removes
-- the cost — so every field name here must come from the dump, never from guessing. If
-- NO owner is reachable (the title menu has no GameMode → no UMenuManager) the resolver
-- returns nil and the class falls back to the scan path, so screens opened from exotic
-- contexts keep reading (the 2026-07-15 title load/options lesson).
--
-- Roots are scanned for once (skipping CDOs) and cached: the GameInstance never dies,
-- MenuManager and the PlayerController die per map switch (cleared on transition,
-- re-found after). Root lookups respect the shared per-tick scan budget and back off
-- while absent (the title screen has no GameMode/HUD, and must not pay a full-array
-- miss every tick).

local Core = require("ui_core")
local Transition = require("transition")
local Mem = require("mem")   -- Mem.raw_addr: an object's stored pointer without dereferencing it

local Directory = {}

local ROOT_BACKOFF = 20   -- ticks (~2s) between FindFirstOf retries for a missing root

local function now()
    return math.floor(os.clock() * (1000 / Core.POLL_MS))
end

-- Guarded property hop. `o[name]` never returns nil for a null/dead field — UE4SS hands
-- back an INVALID RemoteObject — so Core.valid is the only trustworthy check (the same
-- rule as Core.array_of).
-- The fetch goes through Core.member (2026-07-26), not a raw `o[name]`. The validity half was
-- always here; the PROPERTY-EXISTENCE half was not, and this file needs it more than any other:
-- asking a class for a member it does not declare is an uncatchable abort, and two things here
-- do exactly that BY DESIGN. `find_hud` probes `UIFieldManager` on every PlayerController-family
-- HUD precisely to reject the title's plain AHUD, which does not have it; and the mapped chains
-- try ALTERNATIVES (SkillTreeMenu vs SkillCustomize, m_xSaveMenu vs m_xLoadMenu), so the losing
-- branch is by definition a member the object does not declare. The pcall below cannot contain
-- either — UE4SS raises them under the Lua boundary. Core.member asks the class first and
-- returns nil for a name it does not declare, which is the answer both call sites already want.
-- It fails OPEN when the class cannot be identified, so nothing that resolves today stops.
-- `strict` (2026-07-31 audit) — pass it for the hops that are DELIBERATE ABSENCE PROBES, i.e.
-- the ones the comment above describes as trying a name the object is expected not to declare.
-- Core.member falls open when the property set is unavailable, and that is right for an ordinary
-- hop; on an absence probe it is precisely backwards, because the probe's whole purpose is to name
-- something that is usually not there. And "unavailable" is the common case here, not a rare one:
-- PROP_SETS_PER_TICK is ONE set per tick shared by every adapter, and find_hud below spends that
-- single slot on `MyHUD` one line before probing `UIFieldManager` — so the probe was GUARANTEED
-- ungated, on every PlayerController-family object, at the title screen and for several ticks
-- after every map load (a transition flushes prop_sets and roots together).
local function prop(o, name, strict)
    if not Core.valid(o) then return nil end
    local v
    if not pcall(function() v = Core.member(o, name, strict) end) then return nil end
    if not Core.valid(v) then return nil end
    return v
end

-- ---- roots -----------------------------------------------------------------
local roots = {}       -- key -> cached object (valid-checked on every use)
local root_next = {}   -- key -> earliest tick to retry a failed scan

-- First live INSTANCE of a class, skipping class-default objects. FindFirstOf can hand
-- back the CDO (Default__MenuManager), whose manager fields are all null — caching that
-- would make every mapped screen read as permanently absent. Core.first_live is not
-- usable here: it filters for /Engine/Transient, and MenuManager lives on the LEVEL's
-- GameMode actor, not in the transient package.
local function first_instance(cls)
    local all
    if not pcall(function() all = FindAllOf(cls) end) or not all then return nil end
    for _, o in pairs(all) do
        if Core.valid(o) then
            local ok, fn = pcall(function() return o:GetFullName() end)
            if ok and type(fn) == "string" and not fn:find("Default__", 1, true) then
                return o
            end
        end
    end
    return nil
end

-- `no_scan` (crash audit RANK 16, 2026-07-31): answer ONLY from the cached root, never scan.
-- Core.peek_all documents itself as scan-free and fast loops rely on that — screen_map's 20 ms
-- travel-list step among them — but a directory-mapped class routed straight through here, and
-- an unresolved root meant a full FindAllOf. Worse on the loop that does it: the 20 ms dispatch
-- never calls Core.begin_scan_tick, so it DRAINS the shared scan budget without ever refilling
-- it, starving the registry's own detection. Unresolved-and-not-allowed-to-scan answers nil,
-- which every caller already handles as "not right now".
local function find_root(key, cls, no_scan)
    local r = roots[key]
    if Core.valid(r) then return r end
    roots[key] = nil
    if no_scan then return nil end
    local tick = now()
    if (root_next[key] or 0) > tick then return nil end
    -- The scan walks the whole object array, so it counts against the shared per-tick
    -- scan budget; a budget denial just retries next tick (no backoff).
    if not Core.take_scan_slot() then return nil end
    local o = first_instance(cls)
    if o then
        roots[key] = o
        return o
    end
    -- Small per-key stagger so the roots that failed together don't all retry on the
    -- same tick (the same anti-cluster idea as ui_core's per-class jitter).
    root_next[key] = tick + ROOT_BACKOFF + ((key:byte(1) + key:byte(2)) % 7)
    return nil
end

-- The game HUD root. FindAllOf("PlayerController") returns EVERY PC-family instance and
-- the first one is often the WRONG one — measured 2026-07-15: it handed back
-- TwinFootController_0 (the ride/mount controller, MyHUD null) in the field and
-- ATTitleController (plain HUD) at the title, so the whole hud/fm/bm/cm branch silently
-- never resolved. The functional test for the RIGHT controller is the one whose MyHUD
-- exposes UIFieldManager (i.e. it IS the AAT_GameHUD) — probe every candidate.
-- `no_scan`: see find_root above. This one matters most — it is the costliest root (a
-- FindAllOf("PlayerController") plus a probe of every candidate) and it can NEVER resolve at the
-- title screen, so before this it re-scanned on that cadence forever there.
local function find_hud(no_scan)
    local h = roots["hud"]
    if Core.valid(h) then return h end
    roots["hud"] = nil
    if no_scan then return nil end
    local tick = now()
    if (root_next["hud"] or 0) > tick then return nil end
    if not Core.take_scan_slot() then return nil end
    local all
    if pcall(function() all = FindAllOf("PlayerController") end) and all then
        for _, pc in pairs(all) do
            if Core.valid(pc) then
                local hud = prop(pc, "MyHUD")
                -- prop() returns nil unless the field read succeeds AND the value is a
                -- valid object; the UIFieldManager probe rejects the title's plain AHUD
                -- and any vehicle controller with an empty HUD slot.
                -- STRICT: this is the absence probe itself — it exists to reject the title's
                -- plain AHUD, which does not declare UIFieldManager. See prop().
                if hud ~= nil and prop(hud, "UIFieldManager", true) ~= nil then
                    roots["hud"] = hud
                    return hud
                end
            end
        end
    end
    root_next["hud"] = tick + ROOT_BACKOFF + 3
    return nil
end

-- Root getters, by the short key the MAP chains use. fm/bm/cm/wm are property hops
-- off a found root (no scan of their own).
-- Every getter forwards `ns` (no_scan) explicitly rather than reading a module-level flag: a flag
-- set around the resolve body would LEAK if anything unwound past the clear — and on this engine
-- an error can pierce pcall — leaving the directory permanently unable to scan, which is the
-- fail-closed-on-shared-substrate failure this codebase has been bitten by before.
local getters = {}
function getters.mm(ns) return find_root("mm", "MenuManager", ns) end
function getters.gi(ns) return find_root("gi", "BP_ATGameInstance_C", ns) end
-- The TITLE level script actor: owns the title menu's load/options flows through
-- TitleLoadMenuComponent / TitleOptionMenuComponent (AT.hpp:14119). Only exists on the
-- title map; in game it's absent and the mm chains serve those screens instead.
function getters.tt(ns) return find_root("tt", "ATTitleLevelScriptActor", ns) end
getters.hud = find_hud
function getters.fm(ns) return prop(find_hud(ns), "UIFieldManager") end
function getters.bm(ns) return prop(find_hud(ns), "UIBattleManager") end
function getters.cm(ns) return prop(find_hud(ns), "UICommManager") end
function getters.wm(ns) return prop(getters.gi(ns), "WindowManager") end

-- ---- the world epoch (the transition gate's only signal) ---------------------------------
--
-- Identity of the world we are in right now, as a plain number, polled from the game thread
-- by ui_core.begin_scan_tick and handed to Transition.note_epoch. Returns:
--   number -> this world's identity
--   nil    -> no world (boot, or a map switch in progress) → arm the gate
--   false  -> could not read it → not an event, change nothing
--
-- WHY THE GameInstance's WORLD and not a controller. The obvious candidate was the
-- PlayerController, but this game has SEVERAL (`find_hud` above documents handing back
-- TwinFootController, the ride/mount controller) and they come and go DURING a world as the
-- player mounts and dismounts. An epoch that flaps mid-world would fire a spurious cache
-- flush every time — worse than no gate. The GameInstance is the one object that never dies
-- (so it is cached after a single lookup and costs nothing per poll) and `GetWorld()` on it
-- answers with the CURRENT world, which changes only on a real map switch. During a switch
-- it either still points at the freed old world — which Core.valid now rejects safely, since
-- Mem.alive pre-checks the memory before UE4SS dereferences — or at the new one, and both
-- outcomes are the event we want.
-- The nil/false distinction is load-bearing and easy to get wrong: `nil` ARMS THE GATE, so it
-- must mean "there is genuinely no world", never "I could not look this tick". A budget denial,
-- a root back-off or an unreadable address all return FALSE — otherwise a spent scan budget
-- would flush every cache and freeze the reader for the grace period, repeatedly.
local world_warned = false
function Directory.world_epoch()
    local gi = roots["gi"]
    if not Core.valid(gi) then
        roots["gi"] = nil
        -- Never scan while the gate is up: object-array walks mid-load are the boot-hang class
        -- of bugs, and the answer would not change anything (the gate is already armed).
        if Transition.active() then return false end
        gi = getters.gi()                 -- budgeted + backed off like every other root
        if not Core.valid(gi) then return false end
    end
    local w, ok
    ok = pcall(function() w = gi:GetWorld() end)
    if not ok then
        -- Losing this signal is not fatal (the memory pre-check protects the reads) but it
        -- does leave the gate inert, so it must never be silent.
        if not world_warned then
            world_warned = true
            print("[KakarotAccess] GetWorld() unavailable — transition gate is INERT\n")
        end
        return false
    end
    if not Core.valid(w) then return nil end
    -- ACCEPTED HOLE: if no poll ever lands in the window where the WorldContext's world is null
    -- (UE4 nulls it before CollectGarbage) AND the new UWorld is allocated at the freed one's
    -- address, the epoch reads unchanged and the gate never arms for that switch. It is the one
    -- hole the removed GameModeBase notify did not have. Judged acceptable: the notify's own
    -- holes (foreign-thread delivery, arming after the objects were already freed) were worse
    -- and NOT rare, and the memory pre-check now catches the freed handles this would leak.
    return math.tointeger(Mem.raw_addr(w)) or false
end

-- ---- the map: adapter class -> pointer chains --------------------------------
-- Chain = { root_key, field, [field2] }. Several chains when the game keeps several
-- copies (save vs load menu, field tips vs pause tips) — the resolver returns every
-- valid one and the callers pick the on-screen instance. Field names/offsets all from
-- AT.hpp (class body line refs in the header comment).
local MAP = {
    -- Pause ring + its submenus (the UMenuManager family — the slow ones)
    ["Start_Top_C"]            = { {"mm", "m_xRootMenu", "m_UIStartTop"}, {"fm", "StartTop"} },
    ["Start_Item_C"]           = { {"mm", "m_xItemInventoryMenu", "ItemMenu"} },
    ["Start_Item_Customize_C"] = { {"mm", "m_xItemInventoryMenu", "ItemPalette"} },
    ["Start_Skilltree_C"]      = { {"mm", "m_xStartSkillTreeMenu", "SkillTreeMenu"},
                                   {"mm", "m_xPartyMenu", "SkillTreeMenu"} },
    ["Start_Skillcustom_C"]    = { {"mm", "m_xStartSkillCustomize", "SkillCustomize"},
                                   {"mm", "m_xPartyMenu", "SkillCustomize"} },
    ["AT_UIStartChar"]         = { {"mm", "m_xCharacterMenu", "MenuUI"} },
    ["AT_UIStartStatus"]       = { {"mm", "m_xStatusMenu", "MenuUI"} },
    ["AT_UIStartSaveLoad"]     = { {"mm", "m_xSaveMenu", "m_UIStartSaveLoad"},
                                   {"mm", "m_xLoadMenu", "m_UIStartSaveLoad"},
                                   {"tt", "TitleLoadMenuComponent", "m_xLoadMenu", "m_UIStartSaveLoad"} },
    -- Same widget, BLUEPRINT class name: the TITLE menu's load screen instance is
    -- Start_Save_Load_C (census 2026-07-15, "GUARDADO AUTOMÁTICO") and FindAllOf on the
    -- native name returns nothing for it (the native-base gotcha) — screen_saveload
    -- scans this name as its title-screen fallback; the tt chain serves it by pointer.
    ["Start_Save_Load_C"]      = { {"mm", "m_xSaveMenu", "m_UIStartSaveLoad"},
                                   {"mm", "m_xLoadMenu", "m_UIStartSaveLoad"},
                                   {"tt", "TitleLoadMenuComponent", "m_xLoadMenu", "m_UIStartSaveLoad"} },
    ["AT_UIStartParty"]        = { {"mm", "m_xPartyMenu", "MenuUI"},
                                   {"mm", "m_xPartyMenu", "StartPartyMenu"} },
    ["Start_Option_C"]         = { {"mm", "m_xOptionMenu", "m_UIStartOption"},
                                   {"tt", "TitleOptionMenuComponent", "m_xOptionMenu", "m_UIStartOption"} },
    ["AT_UIStartTips"]         = { {"mm", "m_xTipsMenu", "m_UIStartTips"},
                                   {"hud", "UIPause", "UIStartTips"} },
    -- Dragon Ball menu: UMenuManager.m_xDragonBallMenu @0x110 (AT.hpp:41840) ->
    -- UDragonBallMenu.DragonBallMenuUI @0x130 (AT.hpp:40985). Same two-hop shape as every
    -- other submenu here; registered in app.lua as a plain ListScreen, one instance.
    ["AT_UIStartDragonBallMenu"] = { {"mm", "m_xDragonBallMenu", "DragonBallMenuUI"} },
    -- STORY menu: UMenuManager.m_xQuestMenu @0x140 (AT.hpp:41846) -> UQuestMenu.m_UIStartQuest
    -- @0x170 (AT.hpp:42935). screen_story's own comment (line 96) records that Start_Quest_C is
    -- an ABSENT class paying a full FindAllOf every backoff, which is exactly what this retires.
    -- The mapped key is the BLUEPRINT name because that is what the adapter asks for; the field's
    -- declared type is the native UAT_UIStartQuest and the live instance is its BP subclass, which
    -- the resolver does not care about — it returns the pointer, not a name match (same as
    -- Start_Save_Load_C above).
    ["Start_Quest_C"]          = { {"mm", "m_xQuestMenu", "m_UIStartQuest"} },
    ["Tips_C"]                 = { {"fm", "Tips"}, {"mm", "m_xTipsMenu", "m_UITips"},
                                   {"hud", "UIPause", "UITips"} },

    -- HUD-held screens
    ["Xcmn_Pause_C"]           = { {"hud", "UIPause"} },
    -- Start_Commu_Brd_C deliberately NOT mapped (2026-07-16, story-tutorial replay):
    -- the field names are real (AAT_GameHUD.UICommBoardIns @0x5D0, cm.UICommBrdIns
    -- @0x58) but the STORY-flow board leaves BOTH null (it hangs off the
    -- cm.MenuCommunityBrdIns CONTROLLER, whose widget ptr is not reflected — the
    -- soul-emblem-menu hole again), so the mapping asserted "absent", killed the
    -- scan fallback, and the tutorial board read NOTHING for the whole session
    -- (claim trace: board=false on the open board). Scan path serves it; entry
    -- costs one ABSENT_BACKOFF on a fresh session.
    -- AT_UICommunityStart (Soul Emblems grid) deliberately NOT mapped — third strike
    -- 2026-07-15: even after the find_hud fix the user reports the MENU-opened grid
    -- ("EMBLEMAS DE ALMA") silent. The manager has TWO flows: cm.UIEmbListIns (board
    -- flow) and cm.MenuSoulEmListIns (menu flow) — but the latter is a USoulEmblemMenu
    -- CONTROLLER whose only reflected member is GameHUD, and UMenuObjectBase (the
    -- UCommunityMenu's m_xStart_Community) reflects NOTHING (AT.hpp:41863), so no
    -- trustworthy owner field exists for the menu flow. Mapped, a null UIEmbListIns
    -- asserts "absent" and kills the scan fallback — exactly the reported silence.
    -- Scan path serves it instead, made fast by an ENTRY SIGNAL: the lazy controller
    -- (mm.m_xSoulEmblemMenu / cm.MenuSoulEmListIns) flipping null→valid arms a
    -- ui_core watch lane (screen_community.menu_entry_signal, via Directory.peek).
    ["Start_Commu_Detail_C"]   = { {"fm", "CommunityDetail"} },
    ["Battle_Hud_P_Main_C"]    = { {"bm", "BattleHudPlayer"} },
    ["Battle_Hud_E_Main_C"]    = { {"bm", "BattleHudEnemy"} },
    ["Battle_Result_C"]        = { {"bm", "BattleResult"} },
    ["AT_UIQteMashAlert"]      = { {"bm", "QteMash", "WL_Alert"} },

    -- Field manager screens (shops, maps, quest/info, minigames)
    -- Shop_Top_C lives in TWO places: the regular shop top (fm.ShopTop) and the
    -- cook-NPC's embedded mode list ("Preparar un platillo"/"Salir") at
    -- CookingMenu.WL_CookingTop (AT.hpp:32237). Mapping only fm.ShopTop silenced the
    -- cook menu (user bug 2026-07-15) — screen_shoplist enumerates candidates and
    -- picks the one with visible rows, so both chains must be here.
    ["Shop_Top_C"]             = { {"fm", "ShopTop"},
                                   {"fm", "CookingMenu", "WL_CookingTop"} },
    ["Shop_Cmn_C"]             = { {"fm", "ShopCommon"} },
    ["Shop_Cook_C"]            = { {"fm", "CookingMenu"} },
    ["Shop_Info_C"]            = { {"fm", "ShopInfo"} },
    ["Shop_Training_C"]        = { {"fm", "ShopTraining"} },
    ["Map_World_C"]            = { {"fm", "MapWorld"} },
    ["Map_M_C"]                = { {"fm", "MapMng", "AreamapIns"} },
    ["AT_UIMiniMapRadar"]      = { {"fm", "MapMng", "MinimapIns"} },
    ["Field_Memory_C"]         = { {"fm", "FieldMemory"} },
    ["Quest_Main_Clear_C"]     = { {"fm", "QuestMainClear"} },
    -- Defeat menu. The owner field was FOUND by the F7 probe's owner hunt on 2026-07-25
    -- (`OWNER FIELD: AT_UIFieldManager.Gameover -> this host`), which retires the whole scan
    -- path for this screen: no FindAllOf, no watch lane, no ABSENT_BACKOFF, and immune to
    -- cinematic quiet mode — which is what kept it silent, since a defeat happens with the
    -- gameplay world up, no battle HUD, no minimap and no user press, i.e. exactly the state
    -- quiet mode defers scans in. This is also what makes the flapping battle-HUD edge harmless:
    -- a watch on a MAPPED class resolves through the directory and never costs a scan.
    ["Gameover_C"]             = { {"fm", "Gameover"} },
    -- Free/event SPEECH BUBBLES (UAT_UIFieldTalkFree, AT.hpp:33181) — the third dialogue surface,
    -- and the one that was missing: `fm.FieldTalkFree` @0x658. Holds two TArrays of
    -- UAT_UIFieldTalkFreeCore, whose `TextBox` carries the line.
    ["AT_UIFieldTalkFree"]     = { {"fm", "FieldTalkFree"} },
    -- The NAVI WINDOW (UAT_UIFieldNaviWin, AT.hpp:33020) — the portrait pop-up a character speaks
    -- through when they are not physically present, which the player reads as "on the phone".
    -- `fm.FieldNaviWin` @0x550. This was the FOURTH dialogue surface and the mod never looked at it.
    ["Field_Navi_Win_C"]       = { {"fm", "FieldNaviWin"} },
    ["Quest_Sub_C"]            = { {"fm", "QuestSub"} },
    -- The quest-objective HUD (flag panel above the minimap). fm.QuestNavigation
    -- (AT.hpp UIFieldManager 0x568); Quest_Navi_C is its BP subclass (Quest_Navi.hpp).
    -- quest_objective.lua polled this via the scan path and starved (user bug 2026-07-15).
    ["Quest_Navi_C"]           = { {"fm", "QuestNavigation"} },
    -- The episode/quest title card ("Goku vs. Nappa and Vegeta") shown at an episode
    -- start: fm.QuestMainStart (0x558), TitleText is the card's text (AT.hpp 0x3E0).
    -- (QuestMainLogo 0x700 is image-only — ChapterTitleImage — nothing to read.)
    ["AT_UIQuestMainStart"]    = { {"fm", "QuestMainStart"} },
    -- Info_Log_C deliberately NOT mapped: fm.InfoLog is one instance, but overflow
    -- toasts spawn EXTRA pooled Info_Log_C hosts that screen_toasts enumerates via
    -- cached_all — a single-pointer mapping would silently drop those lines.
    -- Info_Log_Level_C deliberately NOT mapped (was {"fm","InfoLevelUp"}): level-ups
    -- were never announced with the mapping on (user bug 2026-07-15) — the fishing
    -- ring-core pattern (owner reachable + field never set = asserted absent, reader
    -- silently dead). The toast is pooled/short-lived; the scan path reads it.
    ["Battle_Tips_Tutorial_C"] = { {"fm", "BattleTipsTutorial"} },
    ["AT_UIMgameFishing"]      = { {"fm", "MiniGameFishing"} },
    ["AT_UIMiniGamePop"]       = { {"fm", "MiniGamePop"} },
    -- AT_UIBattleRushSpeedCore deliberately NOT mapped: AT.hpp declares the pointer on
    -- the fishing widget (UAT_UIMgameFishing.UIBattleRushSpeedCore, 0x3F0) but live
    -- sampling (dump_fishing, 2026-07-03) showed the game never sets it — mapping it made
    -- the resolver assert "does not exist" while fishing, killing the phase-2 reel cue
    -- (screen_fishing finds the core as its own pooled instance via the scan path).

    -- GameInstance-held screens
    -- The window manager holds all three cores side by side (UAT_UIWindowManager, AT.hpp):
    -- GameWindowCore, SystemWindowCore @0x40 (AT.hpp:37735), NumberWindowCore @0x50
    -- (AT.hpp:37737). Only the first was ever mapped; the other two are the exact classes
    -- screen_dialog lists in WINDOW_CLASSES and fetches with Core.cached_live (one instance
    -- per class), so the single-pointer mapping matches how they are read.
    ["Xcmn_Win01_C"]           = { {"wm", "GameWindowCore"} },
    ["Xcmn_Win00_C"]           = { {"wm", "SystemWindowCore"} },
    ["Xcmn_Win02_C"]           = { {"wm", "NumberWindowCore"} },
    ["Loading_C"]              = { {"gi", "LoadingScreen"} },

    -- Title-level screens (the `tt` root — ATTitleLevelScriptActor, AT.hpp:14119)
    -- Gametitle_C was listed as deliberately unmapped for a reason that EXPIRED: the note said
    -- "lives on the title level script actor, no HUD yet", written before the tt root existed.
    -- It does now (added for the title's load/options flows), and that same actor declares the
    -- pointer outright: AATTitleLevelScriptActor.UIGameTitleWidget @0x380 (AT.hpp:14131).
    -- In a gameplay world the tt root is absent, no owner is reached, the resolver returns nil
    -- and the scan path serves it as before — the fallback this file's header describes.
    ["Gametitle_C"]            = { {"tt", "UIGameTitleWidget"} },
    -- Boot agreement / privacy viewer. Two independent owners, both from the dump:
    --   AAT_Title.AgreementDialog @0x340 (AT.hpp:14780), reached through the title widget's
    --   own back-pointer UAT_UIGameTitle.ActorTitle @0x430 (AT.hpp:33258); and
    --   UAT_UIStartOption.AgreementDialog @0x478 (AT.hpp:36643), i.e. the already-mapped
    --   options host, which serves the in-game "view the agreement again" route.
    ["AT_UIXcmnAgreement"]     = { {"tt", "UIGameTitleWidget", "ActorTitle", "AgreementDialog"},
                                   {"mm", "m_xOptionMenu", "m_UIStartOption", "AgreementDialog"} },
}
-- Deliberately UNMAPPED (they keep the scan path): pooled multi-instance widgets whose
-- copies the adapters must enumerate (Xcmn_Keyhelp_C, Xcmn_Header_C, list rows/bars,
-- CFUIMultiLineTextBox, Map_World_Icon_C, Quest_Main_Telop_C, Xcmn_Win00_Choice_C).
--
-- The 2026-07-28 sweep re-checked every scan-based host against AT.hpp for an owner field.
-- Six gained a chain (above). These four have a real, verified pointer and STILL stay on the
-- scan path — a reachable owner is not a reason to map, it is only a precondition:
--   * Xcmn_Subtitles_C (AAT_GameHUD.Subtitles @0x590 / .InMenuSubtitles @0x598,
--     AT.hpp:14685-86) and Field_Talk_Win_C (UAT_UIFieldManager.FieldTalkWin @0x548,
--     AT.hpp:32869) — screen_dialogue reads these through line_from_any/cached_all
--     precisely BECAUSE the game pools several instances (Xcmn_Subtitles_C_0.._2 live) and
--     swaps which one it drives across scene changes. Two HUD pointers cannot represent
--     three instances, so mapping would re-open the 2026-07-06 "narrator lines unread" bug.
--     The multi-instance rule outranks the pointer.
--   * Quest_Sub_Reward_C (UAT_UIQuestSub.UIReward @0x478, AT.hpp:35487) — the owner is the
--     already-mapped fm.QuestSub, so a null UIReward while the sheet is up would assert
--     "absent" and kill the fallback: the fishing ring-core failure shape exactly. This
--     adapter only started running on 2026-07-28 (the pane_rendered fix) and is still
--     pending in-game verification; re-risking it before it has been seen working is
--     backwards. Revisit once it is confirmed good.
--   * Map_World_Curs_C (UAT_UIMapWorld.Map_World_Curs @0x498, AT.hpp:34210) — reachable,
--     but screen_map.lua:115 calls raw FindAllOf and so bypasses the directory entirely.
--     Mapping alone would change nothing; it needs that call site moved to Core first, and
--     the map d-pad path has its own regression history worth not disturbing in the same
--     batch.
-- Also checked and genuinely NOT reachable: the CompZ_Page_* family (Z-Encyclopedia). No
-- class in the dump declares a pointer to those pages — UCompZMenu exposes only CompZMgr /
-- BookActor / UMGRender — which matches the adapter's own note that they are found through
-- the list controller. Scanning is correct there, not a gap.

-- ---- resolver ----------------------------------------------------------------
-- Per-tick memo: the registry sweep resolves the same classes many times per step. The
-- tick stamp (100ms buckets) makes explicit clearing unnecessary. `list = false` memoizes
-- an "owner unreachable" result (distinct from {} = owner said the screen doesn't exist).
local memo = {}   -- cls -> { t = tick, list = {...} | false }

-- nil  -> class not mapped, OR mapped but NO chain reached a valid final owner — the
--         directory cannot answer, so the caller falls back to the scan path. This is
--         what keeps exotic contexts alive: the TITLE menu opens load-game/options with
--         no field GameMode, so UMenuManager doesn't exist there (2026-07-15, found in
--         game) — asserting "absent" then silenced those screens for good.
-- {}   -> a chain DID reach its owner and the widget field is null: the screen does not
--         exist right now (NO scan — the game's own registry is the answer)
-- list -> every currently valid candidate instance (callers pick the on-screen one)
-- `no_scan` (crash audit RANK 16, 2026-07-31): resolve from CACHED roots only — see find_root.
-- Passed by Core.peek_all, whose documented contract is that it never scans and which fast loops
-- depend on. Core.cached_all / cached_live keep the scanning path unchanged.
function Directory.resolve(cls_name, no_scan)
    local chains = MAP[cls_name]
    if not chains then return nil end
    local tick = now()
    local m = memo[cls_name]
    if m and m.t == tick then return m.list or nil end
    local list = {}
    local owner_reached = false
    -- During a map switch every hop below touches dying objects — resolve to "absent"
    -- (NOT fallback: scans mid-load are exactly what the transition gate exists to stop)
    -- and let the post-load ticks rebuild from fresh roots.
    if Transition.active() then
        memo[cls_name] = { t = tick, list = list }
        return list
    end
    local seen = {}
    -- A class with MORE THAN ONE chain is by definition an ALTERNATIVES probe (2026-07-31 audit):
    -- the chains name different owners for the same widget — SkillTreeMenu under
    -- m_xStartSkillTreeMenu vs under m_xPartyMenu, AgreementDialog under the title actor vs under
    -- the option menu — so at most one can be right, and the losing branches are fetching a member
    -- the object is expected not to declare, which is the uncatchable abort. Those hops take the
    -- STRICT gate, which refuses instead of falling open when the property set is unavailable —
    -- and unavailable is the COMMON case, not a rare one: PROP_SETS_PER_TICK is one set per tick
    -- shared by every adapter, so a load or a busy screen leaves several consecutive ticks ungated.
    -- Derived from the table rather than annotated per entry, so a chain added later inherits it.
    -- SINGLE-chain entries keep failing OPEN, deliberately: those are ordinary hops the caller has
    -- positive reason to believe exist, and closing them is the 2026-07-25 Options regression.
    -- Refusing is bounded here in any case — resolve() answering nil (as opposed to an EMPTY list)
    -- falls through to the scan path in Core.cached_all, and the memo is per-tick, so the next tick
    -- retries and the ordinary gate takes over as soon as the class has been enumerated.
    local alt = #chains > 1
    for _, ch in ipairs(chains) do
        -- Walk to the FINAL OWNER (all hops but the last); the last hop is the widget
        -- field itself. Only a valid owner can assert the screen's existence.
        local o = getters[ch[1]](no_scan)
        for i = 2, #ch - 1 do
            o = prop(o, ch[i], alt)
        end
        if Core.valid(o) then
            owner_reached = true
            local w = prop(o, ch[#ch], alt)
            if w ~= nil then
                local ok, a = pcall(function() return w:GetAddress() end)
                local key = (ok and a) or tostring(w)
                if not seen[key] then
                    seen[key] = true
                    list[#list + 1] = w
                end
            end
        end
    end
    if not owner_reached then
        -- Do NOT memoise a no_scan miss. The memo is per-tick and shared, so a fast-loop peek that
        -- failed only because it was not allowed to resolve the root would otherwise poison the
        -- registry's own resolve for the rest of that tick — turning a cheap peek into a cause of
        -- the very absence it was asking about. A no_scan SUCCESS is memoised normally below: it
        -- came from cached roots, so it is the same answer a full resolve would have given.
        if not no_scan then memo[cls_name] = { t = tick, list = false } end
        return nil
    end
    memo[cls_name] = { t = tick, list = list }
    return list
end

-- Guarded one-field look at a root, for adapters that need an ENTRY SIGNAL from a
-- controller object the directory cannot map as a screen (its widget pointer is not
-- reflected — the soul-emblems USoulEmblemMenu case). Returns the valid object or nil.
-- Costs the same guarded hops the mapped chains pay; the root lookup is shared/cached.
function Directory.peek(root_key, field)
    local g = getters[root_key]
    if not g then return nil end
    return prop(g(), field)
end

-- Is a root currently reachable? The `mm` root (the gameplay GameMode's MenuManager)
-- exists ONLY in playable worlds — never at boot/title — which makes it the honest
-- "this is a gameplay world" predicate for the registry's cutscene-quiet gate
-- (2026-07-17: every session-history heuristic — free-roam-seen, dialogue grace —
-- broke on saves that load DIRECTLY into a cinematic).
function Directory.root_ok(root_key)
    local g = getters[root_key]
    if not g then return false end
    return Core.valid(g())
end

-- Map switch: the PlayerController, HUD and MenuManager die with the level. Drop
-- everything (the GameInstance root survives but one re-find after a load is cheap).
Transition.on_begin("ui_directory", function()
    roots, root_next, memo = {}, {}, {}
end)

-- ---- diagnostics (Ctrl+F5 dump only — never per tick) -------------------------
-- One line per mapped class, each chain traced hop by hop, so a silent screen names
-- its exact broken link: "mm(ok).m_xItemInventoryMenu(NULL)" vs "...ItemMenu(ok)".
-- Open the broken screen FIRST, then press Ctrl+F5.
local function class_name_of(o)
    local n
    pcall(function() n = o:GetClass():GetFName():ToString() end)
    return n or "?"
end

function Directory.debug_lines()
    local lines = {}
    -- Dev dump: lift the throttles so a MISSING below means the scan truly found
    -- nothing, not a backoff/budget denial (runs on the game thread, F5 keybind).
    Core.begin_scan_tick()
    root_next = {}
    -- Roots first: what each FindAllOf-based root actually resolved to (class + path),
    -- so "mm(MISSING)" below can be told apart as scan-found-nothing vs backoff-waiting.
    for _, r in ipairs({ { "pc", "PlayerController" }, { "mm", "MenuManager" },
                         { "gi", "BP_ATGameInstance_C" } }) do
        -- Direct scan (no budget/backoff): a "NOT FOUND" here must mean the scan really
        -- returned nothing, not that find_root was throttled this tick.
        local o = roots[r[1]]
        if not Core.valid(o) then o = first_instance(r[2]) end
        if o then
            local fn = "?"
            pcall(function() fn = o:GetFullName() end)
            lines[#lines + 1] = string.format("  root %s = %s", r[1], fn:sub(1, 110))
        else
            lines[#lines + 1] = string.format("  root %s: NOT FOUND (FindAllOf %q)", r[1], r[2])
        end
    end
    for _, d in ipairs({ { "hud", getters.hud }, { "fm", getters.fm }, { "bm", getters.bm },
                         { "cm", getters.cm }, { "wm", getters.wm } }) do
        local o = d[2]()
        lines[#lines + 1] = string.format("  root %s = %s", d[1],
            o and class_name_of(o) or "nil")
    end
    local keys = {}
    for k in pairs(MAP) do keys[#keys + 1] = k end
    table.sort(keys)
    for _, cls in ipairs(keys) do
        local parts = {}
        for _, ch in ipairs(MAP[cls]) do
            local trace = { ch[1] }
            local o = getters[ch[1]]()
            if not Core.valid(o) then
                trace[1] = ch[1] .. "(MISSING)"
            else
                trace[1] = ch[1] .. "(ok)"
                for i = 2, #ch do
                    local nxt = prop(o, ch[i])
                    if nxt == nil then
                        trace[#trace + 1] = ch[i] .. "(NULL)"
                        o = nil
                        break
                    end
                    trace[#trace + 1] = ch[i] .. "(ok)"
                    o = nxt
                end
                if o ~= nil then
                    trace[#trace + 1] = "= " .. class_name_of(o)
                        .. (Core.on_screen(o) and " ON-SCREEN" or " off")
                end
            end
            parts[#parts + 1] = table.concat(trace, ".")
        end
        lines[#lines + 1] = string.format("  %-26s %s", cls, table.concat(parts, "  |  "))
    end
    return lines
end

return Directory
