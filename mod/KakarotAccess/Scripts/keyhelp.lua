-- Keyhelp bar reader: speaks the on-screen contextual button prompts on demand.
--
-- Xcmn_Keyhelp_C is the shared help bar shown in almost every menu (and in-game):
-- a row of (button glyph + action label) pairs, exposed as index-aligned
-- Txt_Keyhelp_NN / Xcmn_Btn_Plat_NN (NN = 01..09).
--
-- The glyph widget (Xcmn_Btn_Plat_C) carries NO semantic button id — only up to
-- four Dmy_Btn image nodes (a glyph can be a combo, e.g. L1 + R1). So the button
-- is identified by the brush texture the game actually displays. BUTTON_NAMES
-- centralizes texture-token -> spoken word; unknown tokens (the device-dependent
-- indexed face-button glyphs like Btn00/Btn01) are left unmapped so we speak the
-- action label alone rather than fabricate a physical button.

local Core = require("ui_core")
local Speech = require("speech")
local I18n = require("i18n")

local Keyhelp = {}

local MAX_ENTRIES = 9   -- Xcmn_Keyhelp_C exposes Txt_Keyhelp_01..09 / Xcmn_Btn_Plat_01..09
local MAX_GLYPH_IMAGES = 4   -- Xcmn_Btn_Plat_C exposes Dmy_Btn_00..03 (combo glyphs)

-- Button glyphs are named from the real texture the game shows in
-- /Art/UI/Xcmn/<platform>/; only confident semantic tokens have a spoken name (the
-- device-dependent indexed face-button glyphs are intentionally unnamed → the action
-- label alone is spoken). Spoken words come from the i18n layer (I18n.keyhelp).

-- The live, visible keyhelp bar (there may be stale hidden instances from other
-- screens, so prefer one that is actually on screen).
local function keyhelp_bar()
    local all = FindAllOf("Xcmn_Keyhelp_C") or {}
    local fallback
    for _, k in pairs(all) do
        if Core.valid(k) and k:GetFullName():find("/Engine/Transient", 1, true) then
            fallback = fallback or k
            if Core.is_visible(k) then return k end
        end
    end
    return fallback
end

-- The last path segment of a widget's brush texture: ".../Btn_L1.Btn_L1" -> "Btn_L1".
local function texture_token(img)
    if not Core.valid(img) then return nil end
    local res
    pcall(function()
        local ro = img.Brush.ResourceObject
        if ro and ro:IsValid() then res = ro:GetFullName() end
    end)
    return res and res:match("([%w_]+)%.[%w_]+$") or nil
end

-- Spoken name for one glyph widget: join the mapped names of its visible Dmy_Btn
-- images (a combo glyph shows several). nil if none of the tokens is known.
local function glyph_name(plat)
    if not Core.valid(plat) then return nil end
    local parts = {}
    for i = 0, MAX_GLYPH_IMAGES - 1 do
        local img = plat["Dmy_Btn_" .. string.format("%02d", i)]
        if Core.is_visible(img) then
            local nm = I18n.keyhelp(texture_token(img) or "")
            if nm then parts[#parts + 1] = nm end
        end
    end
    return #parts > 0 and table.concat(parts, I18n.t("combo_join")) or nil
end

-- The current keyhelp entries, in on-screen order: { {label=, button=}, ... }.
function Keyhelp.read()
    local kh = keyhelp_bar()
    if not kh then return {} end
    local entries = {}
    for n = 1, MAX_ENTRIES do
        local suffix = string.format("%02d", n)
        local txt = kh["Txt_Keyhelp_" .. suffix]
        if Core.is_visible(txt) then
            local label = Core.text_of(txt)
            if label then
                entries[#entries + 1] = {
                    label = label,
                    button = glyph_name(kh["Xcmn_Btn_Plat_" .. suffix]),
                }
            end
        end
    end
    return entries
end

-- Speak the whole bar on demand as "button: action, ...", falling back to the bare
-- action label for entries whose glyph we can't name.
function Keyhelp.announce()
    local entries = Keyhelp.read()
    if #entries == 0 then Speech.say(I18n.t("no_keyhelp"), true) return end
    local parts = {}
    for _, e in ipairs(entries) do
        parts[#parts + 1] = e.button and (e.button .. ": " .. e.label) or e.label
    end
    Speech.say(table.concat(parts, ", "), true)
end

return Keyhelp
