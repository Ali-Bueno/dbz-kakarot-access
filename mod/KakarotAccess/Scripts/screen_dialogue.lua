-- Screen adapter: story dialogue & subtitles (reactive line reader).
--
-- The single highest-value feature for a blind player: following the story. Unlike a
-- menu, this has no selection — it's a STREAM of lines. We poll the two dialogue
-- surfaces and let the shared Announcer speak each new "Speaker: line" on change
-- (diff-gated, interrupt per line, so a lingering identical line never repeats).
--
-- Surfaces (from the CXX header dump + object dump):
--   * Xcmn_Subtitles_C  -> UATUISubtitles     — cutscene / skill / link-bonus voice lines.
--       speaker = Txt_Name, line = Txt_Selif   (both Xcmn_MultiLineText_C nodes)
--   * Field_Talk_Win_C  -> UAT_UIFieldTalkWin  — overworld NPC talk window.
--       speaker = Txt_Speaker, line = Txt_Msg
-- Only one is up at a time; subtitles take priority when both somehow read.
--
-- Text is read plain (mainTxt), then via the wrapper's reflected GetText(), then via
-- the rich renderer (markup resolved) — some lines route through the rich block only.

local Core = require("ui_core")
local A = require("ui_archetypes")

local Dialogue = {}

local ann = Core.make_announcer()
local tick = 0
local subs, talk = nil, nil
local cached = nil   -- current "Speaker: line" computed in is_active, reused by update

-- Speakable text of an Xcmn_MultiLineText_C node, with fallbacks: the plain mainTxt
-- first (the common case), then the wrapper's own reflected FText (GetText — set even
-- when a rich/tagged line leaves mainTxt empty), then the rich renderer's markup
-- resolved to speech. nil if the node carries nothing.
local function node_text(node)
    if not Core.valid(node) then return nil end
    local t = Core.text_of(node)                 -- plain mainTxt.Text
    if t then return t end
    local ok, s = pcall(function() return node:GetText():ToString() end)
    if ok and s and s ~= "" then return A.markup_to_speech(s) or s end
    local rich = node.ExMainTxt
    if Core.valid(rich) then
        local okr, sr = pcall(function() return rich.Text:ToString() end)
        if okr and sr and sr ~= "" then return A.markup_to_speech(sr) end
    end
    return nil
end

-- The current "Speaker: line" from a dialogue window, or nil when it isn't really on
-- screen or has no line. on_screen (not raw IsVisible) so a pooled/faded-out window's
-- stale text doesn't keep this adapter active and shadow whatever is underneath.
local function line_from(w, name_prop, body_prop)
    if not Core.on_screen(w) then return nil end
    local body = node_text(w[body_prop])
    if not body then return nil end
    local speaker = node_text(w[name_prop])
    return speaker and (speaker .. ": " .. body) or body
end

function Dialogue.is_active()
    tick = tick + 1
    subs = Core.cached_live("Xcmn_Subtitles_C", tick)   -- cheap: cached refs, no per-tick scan
    talk = Core.cached_live("Field_Talk_Win_C", tick)
    cached = line_from(subs, "Txt_Name", "Txt_Selif")
        or line_from(talk, "Txt_Speaker", "Txt_Msg")
    return cached ~= nil
end

function Dialogue.reset() ann:reset() end

function Dialogue.update()
    -- The line is the "name": a new line re-announces (interrupt), an unchanged one stays silent.
    ann:focus(nil, nil, cached, nil, nil)
end

return Dialogue
