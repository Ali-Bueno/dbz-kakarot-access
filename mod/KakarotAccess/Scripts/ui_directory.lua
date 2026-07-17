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

local Directory = {}

local ROOT_BACKOFF = 20   -- ticks (~2s) between FindFirstOf retries for a missing root

local function now()
    return math.floor(os.clock() * (1000 / Core.POLL_MS))
end

-- Guarded property hop. `o[name]` never returns nil for a null/dead field — UE4SS hands
-- back an INVALID RemoteObject — so Core.valid is the only trustworthy check (the same
-- rule as Core.array_of).
local function prop(o, name)
    if not Core.valid(o) then return nil end
    local v
    if not pcall(function() v = o[name] end) then return nil end
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

local function find_root(key, cls)
    local r = roots[key]
    if Core.valid(r) then return r end
    roots[key] = nil
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
local function find_hud()
    local h = roots["hud"]
    if Core.valid(h) then return h end
    roots["hud"] = nil
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
                if hud ~= nil and prop(hud, "UIFieldManager") ~= nil then
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
local getters = {}
function getters.mm() return find_root("mm", "MenuManager") end
function getters.gi() return find_root("gi", "BP_ATGameInstance_C") end
-- The TITLE level script actor: owns the title menu's load/options flows through
-- TitleLoadMenuComponent / TitleOptionMenuComponent (AT.hpp:14119). Only exists on the
-- title map; in game it's absent and the mm chains serve those screens instead.
function getters.tt() return find_root("tt", "ATTitleLevelScriptActor") end
getters.hud = find_hud
function getters.fm() return prop(find_hud(), "UIFieldManager") end
function getters.bm() return prop(find_hud(), "UIBattleManager") end
function getters.cm() return prop(find_hud(), "UICommManager") end
function getters.wm() return prop(getters.gi(), "WindowManager") end

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
    ["Xcmn_Win01_C"]           = { {"wm", "GameWindowCore"} },
    ["Loading_C"]              = { {"gi", "LoadingScreen"} },
}
-- Deliberately UNMAPPED (they keep the scan path): pooled multi-instance widgets whose
-- copies the adapters must enumerate (Xcmn_Keyhelp_C, Xcmn_Header_C, list rows/bars,
-- CFUIMultiLineTextBox, Map_World_Icon_C, Quest_Main_Telop_C, Xcmn_Win00_Choice_C) and
-- the title screen (Gametitle_C — lives on the title level script actor, no HUD yet).

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
function Directory.resolve(cls_name)
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
    for _, ch in ipairs(chains) do
        -- Walk to the FINAL OWNER (all hops but the last); the last hop is the widget
        -- field itself. Only a valid owner can assert the screen's existence.
        local o = getters[ch[1]]()
        for i = 2, #ch - 1 do
            o = prop(o, ch[i])
        end
        if Core.valid(o) then
            owner_reached = true
            local w = prop(o, ch[#ch])
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
        memo[cls_name] = { t = tick, list = false }
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
