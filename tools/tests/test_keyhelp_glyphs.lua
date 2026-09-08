-- Regression: a visible stick icon must say which stick, not just "Move cursor".
local here = arg[0]:match("^(.*[\\/])") or "./"
package.path = here .. "..\\..\\mod\\KakarotAccess\\Scripts\\?.lua;" .. package.path

local Core = {
    valid = function(o) return type(o) == "table" end,
    nonnull = function(o) return type(o) == "table" end,
    is_visible = function(o) return type(o) == "table" and o.visible ~= false end,
    member = function(o, name) return o and o[name] end,
    member_path = function(o, ...)
        for _, name in ipairs({...}) do o = o and o[name] end
        return o
    end,
    text_of = function(o) return o and o.text end,
    slot_pos = function(o) return o and o.x end,
}
package.loaded.ui_core = Core
package.loaded.speech = {}
package.loaded.mem = {}
package.loaded.transition = {}
package.loaded.i18n = {
    t = function(key)
        return ({stick_l = "left stick", stick_r = "right stick", combo_join = " plus "})[key]
    end,
    keyhelp = function() return nil end,
    button = function(token) return token end,
}
local A = require("ui_archetypes")
-- These fixtures intentionally lack a semantic button ID. The texture is the
-- source of identity, as in the real board's navigation icons.
A.platbtn_token = function(o) return o.semantic end
local Keyhelp = require("keyhelp")
local function glyph(token)
    local texture = {GetFullName = function()
        return "Texture2D /Game/Art/UI/Xcmn/PLAT_X/" .. token .. "." .. token
    end}
    return {Dmy_Btn_00 = {Brush = {ResourceObject = texture}},
        GetClass = function() return {GetFName = function() return {
            ToString = function() return "Xcmn_Btn_Plat_C" end,
        } end} end}
end

local fails = 0
for _, case in ipairs({
    {"Stk_Nut_L", "left stick"}, {"Stk_L", "left stick"},
    {"Stk_Nut_R", "right stick"}, {"Stk_R", "right stick"},
    {"Unknown_Texture", nil},
}) do
    local actual = Keyhelp.glyph(glyph(case[1]))
    local ok = actual == case[2]
    print((ok and "ok   - " or "FAIL - ") .. case[1] .. " resolves to " .. tostring(case[2]))
    if not ok then fails = fails + 1 end
end

-- Exercise real glyph resolution, Keyhelp.read and Keyhelp.actions together.
-- Naming a texture must not bypass its semantic navigation classification and
-- make ordinary menus start announcing cursor movement as an action.
local bar = {GetFullName = function() return "Xcmn_Keyhelp_C /Engine/Transient.Bar" end}
Core.cached_all = function() return {bar} end
for _, case in ipairs({
    {"Btn_Key_Up", "Up", true}, {"Btn_Key_Dwn", "Down", true},
    {"Btn_Key_L", "Left", true}, {"Btn_Key_R", "Right", true},
    {"Stk_Nut_L", nil, true}, {"Stk_Nut_R", nil, true},
    {"Unknown_Texture", "A", false},
}) do
    local plat = glyph(case[1])
    plat.semantic = case[2]
    bar.Txt_Keyhelp_01 = {text = "Visible prompt", x = 0}
    bar.Xcmn_Btn_Plat_01 = plat
    local entries, actions = Keyhelp.read(true, 1), Keyhelp.actions(1)
    local ok = #entries == 1 and entries[1].nav == case[3]
        and #actions == (case[3] and 0 or 1)
    print((ok and "ok   - " or "FAIL - ") .. case[1]
        .. " preserves navigation classification through real keyhelp reads")
    if not ok then fails = fails + 1 end
end
if fails > 0 then os.exit(1) end
print("all checks passed")
