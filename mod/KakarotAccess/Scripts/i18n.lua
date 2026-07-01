-- Localization for the mod's OWN spoken strings.
--
-- Game-authored text (menu labels, action names, tooltips) is ALREADY localized and
-- read live from the game's widgets, so it is never translated here — only the mod's
-- own connective/descriptor words (button names, "controller:" label, screen names…).
--
-- The active language follows the GAME's language, NOT UE culture (which reports "en"
-- regardless). It is detected from the loaded message table path:
--   CFGameInstance.MessageManager.DataTable = .../Message/PLAT_*/<lang>/messageData
-- e.g. ".../Message/PLAT_W/es_ES/messageData" → base language "es". Detected once on
-- the game thread and cached (call I18n.refresh() to re-detect, e.g. after a language
-- change). Unknown/undetected → DEFAULT.

local I18n = {}

local DEFAULT = "en"
local lang = nil -- cached base code ("es", "en", …) once resolved

-- ---- language detection ----------------------------------------------------

local function detect()
    local gi = FindFirstOf("BP_ATGameInstance_C") or FindFirstOf("GameInstance")
    if not gi or not gi:IsValid() then return nil end
    local mm
    pcall(function() mm = gi.MessageManager end)
    if not mm or not mm:IsValid() then return nil end
    local path
    pcall(function()
        local dt = mm.DataTable
        if dt and dt:IsValid() then path = dt:GetFullName() end
    end)
    -- ".../Message/PLAT_W/es_ES/messageData…" → "es" (es_ES and es_MX both → es).
    return path and path:match("/Message/[%w_]+/(%a%a)_%u%u") or nil
end

-- Active base language (cached; retries detection until the GameInstance is ready).
function I18n.language()
    if not lang then lang = detect() end
    return lang or DEFAULT
end

-- Drop the cached language so the next lookup re-detects (call after a language change).
function I18n.refresh() lang = nil end

-- ---- string tables ---------------------------------------------------------
-- Keys are stable ids; add a language by adding a table. Missing key → English → key.

local S = {
    es = {
        -- Controller buttons, by canonical (KeyConfig/Xbox-style) token.
        buttons = {
            A = "botón A", B = "botón B", X = "botón X", Y = "botón Y",
            LB = "botón LB", RB = "botón RB",
            LT = "gatillo LT", RT = "gatillo RT",
            L3 = "clic del stick izquierdo", R3 = "clic del stick derecho",
            Start = "botón Start", Back = "botón Select",
            Up = "cruceta arriba", Down = "cruceta abajo",
            Left = "cruceta izquierda", Right = "cruceta derecha",
        },
        button_fallback = "botón %s",
        controller_prefix = "mando: ",
        -- Keyhelp-bar glyphs, by texture token.
        keyhelp = {
            Btn_L1 = "L1", Btn_R1 = "R1", Btn_L2 = "L2", Btn_R2 = "R2",
            Btn_L3 = "L3", Btn_R3 = "R3",
            Btn_Options = "Options", Btn_Pad = "panel táctil",
            Btn_Key_Ud = "arriba y abajo", Btn_Key_Lr = "izquierda y derecha",
            Btn_Key_All = "dirección",
        },
        combo_join = " más ",
        no_keyhelp = "No hay ayuda de botones",
        save_changes = "Guardar cambios",
        main_menu = "Menú principal",
        reader_on = "Lector de menús activado",
        reader_off = "Lector de menús desactivado",
    },
    en = {
        buttons = {
            A = "A button", B = "B button", X = "X button", Y = "Y button",
            LB = "LB button", RB = "RB button",
            LT = "LT trigger", RT = "RT trigger",
            L3 = "left stick click", R3 = "right stick click",
            Start = "Start button", Back = "Select button",
            Up = "d-pad up", Down = "d-pad down",
            Left = "d-pad left", Right = "d-pad right",
        },
        button_fallback = "%s button",
        controller_prefix = "controller: ",
        keyhelp = {
            Btn_L1 = "L1", Btn_R1 = "R1", Btn_L2 = "L2", Btn_R2 = "R2",
            Btn_L3 = "L3", Btn_R3 = "R3",
            Btn_Options = "Options", Btn_Pad = "touchpad",
            Btn_Key_Ud = "up and down", Btn_Key_Lr = "left and right",
            Btn_Key_All = "direction",
        },
        combo_join = " plus ",
        no_keyhelp = "No button help",
        save_changes = "Save changes",
        main_menu = "Main menu",
        reader_on = "Menu reader on",
        reader_off = "Menu reader off",
    },
}

local function tbl() return S[I18n.language()] or S[DEFAULT] end

-- ---- accessors -------------------------------------------------------------

-- A plain localized string by key (falls back to English, then the key itself).
function I18n.t(key)
    local t = tbl()
    local v = t[key]
    if v ~= nil then return v end
    local e = S[DEFAULT][key]
    if e ~= nil then return e end
    return key
end

-- Localized controller-button name for a canonical token (A, LB, L3, …). Unknown
-- tokens use the language's "%s" fallback template so nothing is dropped.
function I18n.button(token)
    local t = tbl()
    local b = t.buttons and t.buttons[token]
    if b then return b end
    return string.format(t.button_fallback or "%s", token)
end

-- Localized keyhelp glyph name for a texture token (Btn_L1, Btn_Options, …), or nil
-- if the token has no descriptive name in any table.
function I18n.keyhelp(token)
    local t = tbl()
    return (t.keyhelp and t.keyhelp[token])
        or (S[DEFAULT].keyhelp and S[DEFAULT].keyhelp[token]) or nil
end

return I18n
