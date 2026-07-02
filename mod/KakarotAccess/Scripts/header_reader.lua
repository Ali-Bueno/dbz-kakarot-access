-- Field-menu section reader (hook-driven).
--
-- The overworld Start menu (Start_Top_C) and its sub-screens label their options with
-- IMAGE fonts — there is NO readable text and no stored selection index anywhere (verified
-- in the CXX headers + object dump). The one readable signal is the shared header widget
-- UAT_UIXCmnHeader: when the current section changes it calls
--   SetFontType(EXCmnHeaderFontType FontType, EXCmnHeaderIconType IconType, bool)
-- with an ENUM naming the section (Item / Option / World / Shop / Quest / …). main.lua hooks
-- that reflected UFunction and hands us the FontType value; we map it to a spoken name.
--
-- This module is reloadable (the hook in main.lua is not); it holds only the diff-gate + the
-- i18n lookup, so the enum→name table can be tuned with Ctrl+Shift+R without re-hooking.

local Speech = require("speech")
local I18n = require("i18n")

local HeaderReader = {}

local last_ft = nil   -- last announced FontType enum value (diff-gate against refresh spam)

-- Announce the section for an EXCmnHeaderFontType value. Called from the (guarded) hook, so
-- it must never raise. Speaks only on change; unmapped enum values are silent (better than a
-- wrong guess). interrupt=true — entering a section is a context change.
function HeaderReader.announce(ft)
    if type(ft) ~= "number" or ft == last_ft then return end
    last_ft = ft
    local name = I18n.header(ft)
    if name then Speech.say(name, true) end
end

return HeaderReader
