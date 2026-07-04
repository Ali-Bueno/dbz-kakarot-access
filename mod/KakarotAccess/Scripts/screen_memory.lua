-- Screen adapter: Field Memory / story-recap viewer (Field_Memory_C).
--
-- The illustrated recap panels that a Field Memory collectible opens (e.g. "The Dash
-- for Dinner" — unlocks a Z Encyclopedia entry). A titled, paged narration: press A to
-- advance. Verified via F7 (dump_1783150124): host Field_Memory_C, title Txt_Title,
-- narration Txt_Msg (both CFUIMultiLineTextBox wrappers).
--
-- Read the title as the screen name (once on entry) and the current page's narration as
-- the focused text, re-announced whenever it changes (each A-Next page turn).

local Core = require("ui_core")
local A = require("ui_archetypes")

local Memory = {}

local ann = Core.make_announcer()
local host = nil
local tick = 0
local msg = nil   -- current page narration, computed in is_active and reused by update

local function read(node)
    if not Core.valid(node) or not Core.on_screen(node) then return nil end
    return A.markup_to_speech(Core.read_text(node))
end

function Memory.is_active()
    tick = tick + 1
    host = Core.cached_live("Field_Memory_C", tick)
    if not Core.on_screen(host) then msg = nil return false end
    msg = read(host.Txt_Msg)
    return msg ~= nil
end

function Memory.reset() ann:reset() end

function Memory.update()
    -- Title = screen name (spoken once on entry); narration = the item (re-announced on
    -- each page turn, when Txt_Msg changes).
    local title
    pcall(function() title = read(host.Txt_Title) end)
    ann:focus(title, nil, msg, nil, nil)
end

return Memory
