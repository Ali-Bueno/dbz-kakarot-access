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

local function build_bindings()
    local m = { configToCtrl = {}, configToDyn = {}, dynToCtrl = {} }
    local ico = StaticFindObject(ICON_DATA)
    if not Core.valid(ico) then return m end
    local arr = ico.KeyConfigList
    local n = 0
    pcall(function() n = #arr end)
    for i = 1, n do
        local e = arr[i]
        if e then
            local cn = struct_str(e, "ConfigName")
            local dyn = struct_str(e, "DynamicAssignInputId")
            local ctrl = struct_str(e, "DynamicAssignInputControllerId")
            if cn ~= "" and ctrl ~= "" then m.configToCtrl[cn] = ctrl end
            if cn ~= "" and dyn ~= "" then m.configToDyn[cn] = dyn end
            if dyn ~= "" and ctrl ~= "" and not m.dynToCtrl[dyn] then m.dynToCtrl[dyn] = ctrl end
        end
    end
    return m
end

-- Drop the cached map so the next resolve rebuilds it (call on options-screen entry,
-- since rebinding a controller button changes the mapping).
function A.clear_binding_cache() bindings = nil end

-- The controller button (e.g. "Controller_Btn_L3") an action-alias KeyConfigId maps to,
-- or nil. Direct button ids are handled by button_name, not here.
local function resolve_ctrl(configId)
    if not bindings then bindings = build_bindings() end
    local ctrl = bindings.configToCtrl[configId]
    if ctrl then return ctrl end
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

-- Turn CFramework rich-text markup into speakable text: resolve <inputicon> tags to
-- button names, drop other tags (<span color=…>, </>), and collapse whitespace. Reused
-- by any inline-icon text (dialogs, move lists, tutorials). nil if empty.
function A.markup_to_speech(s)
    if not s or s == "" then return nil end
    s = s:gsub('<inputicon%s+KeyConfigId="([^"]+)"%s*/>', function(id)
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
