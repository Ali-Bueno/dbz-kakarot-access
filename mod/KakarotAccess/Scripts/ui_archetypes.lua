-- Widget-archetype readers (the reusable "HOW each control sounds" layer of the
-- hybrid architecture — see reference/ui-accessibility/generic-strategy.md).
--
-- Keyed on THIS game's shared widget framework (Xcmn_*/Xlist_*), so any screen
-- built from the same widgets reuses the same reader. Screen adapters classify a
-- focused control and call in here; no per-screen branching lives in a reader.

local Core = require("ui_core")
local I18n = require("i18n")

local A = {}

local GAUGE_SEGMENTS = 20   -- volume gauge = 20 on/off segments

-- ---- Xlist_Bar03_C list row (options-style list) ---------------------------

function A.row_index(r)
    local s = r:GetFullName():match("Xlist_Bar03_(%d+)")
    return s and tonumber(s) or -1
end

-- The volume gauge value as a percentage, or nil if the row isn't a slider.
local function gauge_value(row)
    local g = row.Xlist_Bar_03_Gauge
    if not Core.is_visible(g) then return nil end
    local on = 0
    for i = 0, GAUGE_SEGMENTS - 1 do
        if Core.is_visible(g["Ins_System_On" .. string.format("%02d", i)]) then on = on + 1 end
    end
    return math.floor(on / GAUGE_SEGMENTS * 100 + 0.5) .. "%"
end

-- ---- Button binding (controller-config "Ajustes del mando" tab) ------------

-- Localized name for a controller-button KeyConfigId (e.g. "KeyConfig_Controller_Btn_B"
-- -> "botón B"). Returns nil if the id carries no Btn_ token. The token after "Btn_" is
-- the game's canonical (Xbox-style) button id, the SAME regardless of the connected
-- device — only the on-screen glyph changes. Spoken words come from the i18n layer.
function A.button_name(keyconfigid)
    local tok = keyconfigid:match("Btn_(.+)$")
    if not tok then return nil end
    return I18n.button(tok)
end

-- ---- KeyConfig resolver (controller button behind an action alias) ---------

-- The mando tab's markup id IS the button (KeyConfig_Controller_Btn_X). The combat /
-- exploration tabs use an ACTION alias (KeyConfig_Battle_HighBoost) whose physical
-- button lives in the shared icon-data asset: the alias entry gives a DynamicAssignInputId
-- (Battle_HighBoost), and another entry with that same dyn id carries the semantic
-- controller button (DynamicAssignInputControllerId = Controller_Btn_L3). We build both
-- lookups once from that asset (159 structs) and cache them. The KEYBOARD key is NOT in
-- any readable data (the game uses a proprietary input system, and PlayerInput's
-- ActionMappings are empty), so only the controller button is recoverable.
local ICON_DATA = "/Game/CFramework/DataAssets/CFTextIconData.CFTextIconData"
local bindings -- { configToCtrl = {}, configToDyn = {}, dynToCtrl = {} } or nil

local function struct_str(e, field)
    local ok, v = pcall(function() return e[field] end)
    if not ok or v == nil then return "" end
    if type(v) == "userdata" then
        local oks, s = pcall(function() return v:ToString() end)
        return oks and s or ""
    end
    return tostring(v)
end

-- Returns nil (NOT an empty map) while the icon-data asset isn't loaded yet, so the
-- cache retries later — StaticFindObject only sees LOADED assets, and caching an
-- empty build made every button resolution silently fail for the whole session
-- (seen live 2026-07-03: the fishing button spoke in one session and not another).
local function build_bindings()
    local m = { configToCtrl = {}, configToDyn = {}, dynToCtrl = {}, idxToCtrl = {} }
    local ico = StaticFindObject(ICON_DATA)
    if not Core.valid(ico) then
        -- The asset may simply not be loaded yet this session (StaticFindObject only
        -- sees LOADED objects) — load it ourselves; all callers run on the game thread.
        pcall(function() LoadAsset(ICON_DATA) end)
        ico = StaticFindObject(ICON_DATA)
    end
    if not Core.valid(ico) then return nil end
    local arr = ico.KeyConfigList
    local n = 0
    pcall(function() n = #arr end)
    for i = 1, n do
        local e = arr[i]
        if e then
            local cn = struct_str(e, "ConfigName")
            local dyn = struct_str(e, "DynamicAssignInputId")
            local ctrl = struct_str(e, "DynamicAssignInputControllerId")
            local icon = struct_str(e, "IconName")
            if cn ~= "" and ctrl ~= "" then m.configToCtrl[cn] = ctrl end
            if cn ~= "" and dyn ~= "" then m.configToDyn[cn] = dyn end
            if dyn ~= "" and ctrl ~= "" and not m.dynToCtrl[dyn] then m.dynToCtrl[dyn] = ctrl end
            -- Face buttons are device-INDEXED everywhere (glyph "Btn_N", EATPlatBtnId
            -- Btn00..03); the asset pairs that index with the semantic button — the
            -- only data-driven index->button mapping in the game.
            local idx = icon:match("^Btn_?(%d+)$")
            if idx and ctrl ~= "" and not m.idxToCtrl[tonumber(idx)] then
                m.idxToCtrl[tonumber(idx)] = ctrl
            end
        end
    end
    return m
end

-- Drop the cached map so the next resolve rebuilds it (call on options-screen entry,
-- since rebinding a controller button changes the mapping).
function A.clear_binding_cache() bindings = nil end

-- Diagnostic: state of the KeyConfig resolver ("not built" / entry counts + the
-- face-button index map). Reported in the Ctrl+F5 nav dump.
function A.bindings_status()
    if not bindings then bindings = build_bindings() end
    if not bindings then return "NOT BUILT (asset not loaded)" end
    local n = 0
    for _ in pairs(bindings.configToCtrl) do n = n + 1 end
    local idx = {}
    for i = 0, 3 do idx[#idx + 1] = i .. "=" .. tostring(bindings.idxToCtrl[i]) end
    return string.format("ctrl entries=%d, face map: %s", n, table.concat(idx, " "))
end

-- The controller button (e.g. "Controller_Btn_L3") an action-alias KeyConfigId maps to,
-- or nil. Direct button ids are handled by button_name, not here.
local function resolve_ctrl(configId)
    if not bindings then bindings = build_bindings() end
    if not bindings then return nil end
    local ctrl = bindings.configToCtrl[configId]
    if ctrl then return ctrl end
    -- The id may already BE a dynamic-input id (tutorials / HUD guides use the raw dyn id,
    -- e.g. "Battle_Guard", not the "KeyConfig_Battle_Guard" ConfigName the options tab uses),
    -- so try the dyn->ctrl map directly before the config->dyn->ctrl chain.
    if bindings.dynToCtrl[configId] then return bindings.dynToCtrl[configId] end
    local dyn = bindings.configToDyn[configId]
    return dyn and bindings.dynToCtrl[dyn] or nil
end

-- The KeyConfigId in a row's RICH text markup (Txt_Mode.ExMainTxt, a
-- CFExtendRichTextBlock): <inputicon KeyConfigId="KeyConfig_Controller_Btn_X"/>.
-- Only the tab that shows this glyph has ExMainTxt visible (other tabs show a plain
-- value in mainTxt and leave stale markup here), so gate on visibility to avoid
-- recycled-row bleed. Returns the raw KeyConfigId, or nil.
function A.row_keyconfig(row)
    local tm = row.Txt_Mode
    if not Core.valid(tm) then return nil end
    local rich = tm.ExMainTxt
    if not Core.is_visible(rich) then return nil end
    local ok, s = pcall(function() return rich.Text:ToString() end)
    if not ok or not s then return nil end
    return s:match('KeyConfigId="([^"]+)"')
end

-- The controller button an action is bound to, spoken. On the mando tab the id IS the
-- button → spoken plainly ("botón A"), since that tab configures the controller. On the
-- combat/exploration tabs the id is an action alias whose on-screen glyph is the KEYBOARD
-- key (not recoverable); we resolve the controller EQUIVALENT and label it "mando: ..."
-- so it isn't mistaken for the literal key. nil if the row carries no binding.
function A.row_binding(row)
    local kc = A.row_keyconfig(row)
    if not kc then return nil end
    local direct = A.button_name(kc)
    if direct then return direct end
    local ctrl = resolve_ctrl(kc)
    local name = ctrl and A.button_name(ctrl)
    return name and (I18n.t("controller_prefix") .. name) or nil
end

-- The spoken button for ANY KeyConfigId (direct button id or action alias resolved
-- through the icon-data asset), with NO "controller:" prefix. For inline-icon markup
-- (dialogs, tutorials, prompts) where we just want the button name. nil if unresolvable.
function A.keyconfig_button(kc)
    return A.button_name(kc) or (resolve_ctrl(kc) and A.button_name(resolve_ctrl(kc))) or nil
end

-- The spoken button for a live UAT_UIXcmnPlatBtn glyph widget (QTE / minigame / prompt
-- glyphs). Semantic sources, most authoritative first:
--   1. CurrentDynamicAssignInputControllerId ("Controller_Btn_B" -> "botón B");
--   2. CurrentActionID resolved through the KeyConfig asset (action alias -> button);
--   3. the semantic EATPlatBtnId entries in CurrentKeyIds (bumpers/triggers/d-pad —
--      the indexed face buttons Btn00..03 are device-dependent and stay unmapped).
-- nil if none resolves (caller speaks its label alone rather than fabricate a button).
local PLATBTN_TOKEN = {  -- EATPlatBtnId -> canonical Btn_ token (AT_enums.hpp)
    [4] = "RB", [5] = "RT", [6] = "R3", [7] = "LB", [8] = "LT", [9] = "L3",
    [11] = "Left", [12] = "Up", [13] = "Right", [14] = "Down",
    [19] = "Start", [20] = "Back",
}
-- Face buttons by glyph index (EATPlatBtnId Btn00..Btn03), spoken in the Xbox names the
-- game DRAWS. The enum itself is PlayStation-flavoured (its shoulders are BtnR1/BtnL1 and
-- it has BtnOptions/BtnTouchPad — AT_enums.hpp), and it enumerates the faces in the
-- Japanese order RIGHT, BOTTOM, LEFT, TOP = ○ ✕ □ △, which in Xbox names is B A X Y.
-- CONFIRMED live off the keyhelp bar (dump_keyhelp 2026-07-14): the item menu's "Usar"
-- (= A on the pad) carries index 1 and "Atrás" (= B) carries index 0 — so the A/B pair is
-- the mirror of the naive Xbox order, while index 2/3 (X/Y) already matched. Fixing this
-- also corrects the item quick-slot palette, whose four slots ARE these four buttons.
-- The KeyConfig asset's IconName pairing is NOT a substitute (it resolved index 3 to "B"
-- and a press inside the timing zone still failed — dump_fishing 2026-07-03); it stays a
-- fallback for exotic devices only.
local FACE_TOKEN = { [0] = "B", [1] = "A", [2] = "X", [3] = "Y" }

-- Canonical Btn_ token for a raw EATPlatBtnId value: semantic ids directly; the
-- indexed face buttons (Btn00..03) via the standard layout, then the asset pairing.
local function platbtn_id_token(id)
    if id == nil then return nil end
    local tok = PLATBTN_TOKEN[id]
    if tok then return tok end
    if id >= 0 and id <= 3 then
        if FACE_TOKEN[id] then return FACE_TOKEN[id] end
        if not bindings then bindings = build_bindings() end
        local ctrl = bindings and bindings.idxToCtrl[id]
        return ctrl and ctrl:match("Btn_(.+)$") or nil
    end
    return nil
end

-- Spoken name for a raw EATPlatBtnId value.
function A.platbtn_id(id)
    local tok = platbtn_id_token(id)
    return tok and I18n.button(tok) or nil
end

-- Raw token form, for time-critical prompts that speak the bare letter.
A.platbtn_id_token = platbtn_id_token

-- Tokens from an EATPlatBtnId TArray member; first resolvable wins.
local function ids_token(owner, name)
    local arr, n = Core.array_of(owner, name)
    if not arr then return nil end
    local tok
    pcall(function()
        for i = 1, n do
            local t = platbtn_id_token(tonumber(arr[i]))
            if t then tok = t return end
        end
    end)
    return tok
end

-- Canonical (Xbox-style) button token for a live UAT_UIXcmnPlatBtn glyph widget —
-- e.g. "A", "B", "RB". Use this raw for time-critical prompts (a bare letter is much
-- faster to hear than "botón A"); platbtn_name below gives the full spoken form.
function A.platbtn_token(plat)
    if not Core.valid(plat) then return nil end
    local tok
    local arr, n = Core.array_of(plat, "CurrentDynamicAssignInputControllerId")
    if arr then
        pcall(function()
            for i = 1, n do
                local s = arr[i]:ToString()
                local t = s and s:match("Btn_(.+)$")
                if t then tok = t return end
            end
        end)
    end
    if tok then return tok end
    pcall(function()
        local act = plat.CurrentActionID:ToString()
        if act and act ~= "" and act ~= "None" then
            local ctrl = resolve_ctrl(act) or act
            tok = ctrl:match("Btn_(.+)$")
        end
    end)
    if tok then return tok end
    -- The pad id lists (KeyIdsForPad is the controller set; CurrentKeyIds may hold the
    -- keyboard set when playing with KB) — indexed face buttons resolve via the asset.
    tok = ids_token(plat, "KeyIdsForPad")
    if tok then return tok end
    tok = ids_token(plat, "CurrentKeyIds")
    if tok then return tok end
    -- Last resort: the glyph texture actually displayed (indexed too -> same resolver).
    local imgs, ni = Core.array_of(plat, "Image_List")
    if imgs then
        pcall(function()
            for i = 1, ni do
                local img = imgs[i]
                if Core.is_visible(img) then
                    local ro = img.Brush.ResourceObject
                    if ro and ro:IsValid() then
                        local idx = ro:GetFullName():match("Btn_?(%d+)%.[%w_]+$")
                        if idx then
                            local t = platbtn_id_token(tonumber(idx))
                            if t then tok = t return end
                        end
                    end
                end
            end
        end)
    end
    return tok
end

function A.platbtn_name(plat)
    local tok = A.platbtn_token(plat)
    return tok and I18n.button(tok) or nil
end

-- Turn CFramework rich-text markup into speakable text: resolve <inputicon> tags to
-- button names, drop other tags (<span color=…>, </>), and collapse whitespace. Reused
-- by any inline-icon text (dialogs, move lists, tutorials). nil if empty.
function A.markup_to_speech(s)
    if not s or s == "" then return nil end
    -- Match the WHOLE inputicon tag and pull KeyConfigId out of its attributes, so extra
    -- attributes (e.g. ForceIconScale="2.7", present in tutorial/HUD glyphs) don't stop the
    -- match and let the generic tag-strip below delete the glyph. "Plus" is the combo
    -- separator (A + B), spoken as the localized join word.
    s = s:gsub('<inputicon%s+([^>]-)/>', function(attrs)
        -- the id is usually KeyConfigId="…" but combo separators use id="Icon_Plus".
        local id = attrs:match('KeyConfigId="([^"]+)"') or attrs:match('id="([^"]+)"')
        if not id then return " " end
        if id == "Plus" or id == "Icon_Plus" then return I18n.t("combo_join") end
        local b = A.keyconfig_button(id)
        return b and (" " .. b .. " ") or " "
    end)
    s = s:gsub("<[^>]->", " ")                    -- drop remaining tags
    s = s:gsub("%s+", " "):gsub("^%s+", ""):gsub("%s+$", "")
    return s ~= "" and s or nil
end

-- The row's current value. A button-config row exposes its binding via the rich
-- text (checked first). Otherwise only ADJUSTABLE rows (a slider gauge, or the
-- left/right value arrows) carry a real inline value; plain action rows leave STALE
-- text in Txt_Mode from a previously-viewed tab (rows are recycled across tabs), so
-- reading it would bleed e.g. a graphics value onto a button action. Gate on
-- adjustability, not just node visibility.
function A.row_value(row)
    local b = A.row_binding(row)
    if b then return b end
    local g = gauge_value(row)
    if g then return g end
    local adjustable = Core.is_visible(row.Xmenu_Arrow01_L)
        or Core.is_visible(row.Xmenu_Arrow01_R)
    if not adjustable then return nil end
    if Core.is_visible(row.Txt_Mode) then
        local v = Core.text_of(row.Txt_Mode)
        if v then return v end
    end
    if Core.is_visible(row.Txt_Mode_Scroll) then
        local v = Core.text_of(row.Txt_Mode_Scroll)
        if v then return v end
    end
    return nil
end

function A.row_name(row)
    return Core.text_of(row.Txt_List)
end

-- Scan a pool of Xlist_Bar03_C rows: lowest-index fad row (the real selection),
-- the highest index present, a by-index map, and any valid row.
function A.scan_list(rows)
    local low, maxIdx, any, byIdx = nil, -1, nil, {}
    for _, r in ipairs(rows) do
        if Core.valid(r) then
            any = any or r
            local idx = A.row_index(r)
            byIdx[idx] = r
            if idx > maxIdx then maxIdx = idx end
            if Core.is_visible(r.Ins_Cursor_Fad) then
                if not low or idx < low.idx then low = { idx = idx, row = r } end
            end
        end
    end
    return low, maxIdx, any, byIdx
end

-- ---- Common menu-list (UAT_UIMenuListBase00 / 01 / 03, MenuBarBase03) -------
-- The game's ONE generic, robust selection mechanism. These list widgets expose a
-- reflected `GetSelectValue() -> int32` (the highlighted row index) and hold their rows
-- in the `ListPlateCtn` array (each row a UAT_UIList0NChoice with `TxtName` + optional
-- `TxtNum`). Reading the index avoids the fragile per-row highlight-image heuristics these
-- lists would otherwise need — so any screen built on one of these bases (shop, item,
-- cooking, skill customize, Dragon Ball menu, options bar, …) reuses this reader.
--
-- NOTE (from the CXX/object-dump study): MenuListBase06 (quest list) is the exception —
-- it has NO GetSelectValue, so those stay highlight-only. Widgets outside this family
-- (Xlist_Bar*, Xcmn_Win*_List, tabs, cursors, map, skill tree) have no reflected index.

-- The selected index of a MenuListBase list, or nil. Calling a member fn on an invalid
-- UObject is an UNCATCHABLE abort, so validate first, then pcall the reflected call.
function A.list_select_index(list)
    if not Core.valid(list) then return nil end
    local ok, idx = pcall(function() return list:GetSelectValue() end)
    if ok and type(idx) == "number" and idx >= 0 then return idx end
    return nil
end

-- The selected row's { name, num } from a MenuListBase list: GetSelectValue() indexes
-- ListPlateCtn (a TArray — 1-based in UE4SS Lua, so engine index + 1). Row label is TxtName
-- on List00/01 rows and Txt_List on List03 rows (tutorials), so try both; num = TxtNum
-- (count/price/level, nil if the row has none). nil if unreadable.
function A.list_selected_row(list)
    local idx = A.list_select_index(list)
    if not idx then return nil end
    -- Bounds-check against the live array count BEFORE indexing. When a list rebuilds (e.g. a
    -- tutorials tab switch), GetSelectValue can momentarily exceed the repopulated ListPlateCtn,
    -- and an out-of-range index into a TArray ABORTS UNCATCHABLY (pcall can't save it).
    -- Core.array_of also validates the array itself — an invalid one throws the same way.
    local plates, num = Core.array_of(list, "ListPlateCtn")
    if not plates or idx < 0 or idx >= num then return nil end
    local row
    if not pcall(function() row = plates[idx + 1] end) or not Core.valid(row) then return nil end
    return {
        name = Core.read_text(row.TxtName) or Core.read_text(row.Txt_List),
        num  = Core.read_text(row.TxtNum),
    }
end

-- A confirm button (e.g. "Guardar cambios") is a phantom overflow row at the last
-- slot that DUPLICATES the last real option's name once you scroll past it. A real
-- option merely rendered in the last slot won't duplicate its neighbour.
function A.is_overflow_button(low, maxIdx, byIdx)
    if not low or low.idx ~= maxIdx then return false end
    local above = byIdx[maxIdx - 1]
    if not above then return false end
    return A.row_name(above) == A.row_name(low.row)
end

return A
