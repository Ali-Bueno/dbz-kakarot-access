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

-- Overworld chatter/subtitles do NOT pause the game, so they must not silence the
-- navigation radar (menus.md: keep cues running when the game doesn't pause). The
-- nav tracker checks this flag; every other adapter (menus, dialogs) keeps muting.
Dialogue.nav_mute = false

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
Dialogue.keyhelp_auto = false

local ann = Core.make_announcer()
local tick = 0
local cached = nil   -- current "Speaker: line" computed in is_active, reused by update

-- The game's OWN subtitles option gates the subtitle surface (user bug 2026-07-15:
-- voice-line subtitles were spoken with the option off). The value lives in the system
-- save object: ATSaveSystem.Option.EnableSubtitle (int32; FATSaveSystemOption 0x1C,
-- held at UATSaveSystem+0xB0 — ATExt.hpp:815/4670; both are reflected UPROPERTYs, so
-- this is a plain property read, no raw offsets). The save object is found once
-- (FindAllOf, budget-gated, CDO skipped) and cached — it lives as long as the game.
-- FAIL-OPEN: while it can't be found/read, lines keep speaking — a lookup failure must
-- never silence the story for a blind player. Only Xcmn_Subtitles_C is gated: the NPC
-- talk window (Field_Talk_Win_C) is a dialogue box every player sees regardless.
local savesys = nil
local savesys_next = 0   -- next tick allowed to retry the lookup (~10 s backoff)
local sub_logged = nil   -- last logged option value / "miss" (log on CHANGE, never per tick)

local function sub_log(what)
    if sub_logged == what then return end
    sub_logged = what
    print("[KakarotAccess] subtitles option: " .. what .. "\n")
end

local function subtitles_on()
    if not Core.valid(savesys) then
        savesys = nil
        if tick < savesys_next then return true end
        savesys_next = tick + 100
        if not Core.take_scan_slot() then return true end
        local all
        if pcall(function() all = FindAllOf("ATSaveSystem") end) and all then
            for _, o in pairs(all) do
                if Core.valid(o) then
                    local ok, fn = pcall(function() return o:GetFullName() end)
                    if ok and type(fn) == "string" and not fn:find("Default__", 1, true) then
                        savesys = o
                        break
                    end
                end
            end
        end
        if not Core.valid(savesys) then
            sub_log("ATSaveSystem not found (fail-open, subtitles keep reading)")
            return true
        end
    end
    local v
    if not pcall(function() v = savesys.Option.EnableSubtitle end) then
        sub_log("EnableSubtitle unreadable (fail-open)")
        return true
    end
    local n = tonumber(v)
    if n == nil then
        sub_log("EnableSubtitle non-numeric (fail-open)")
        return true
    end
    sub_log("EnableSubtitle = " .. n)
    return n ~= 0
end

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
-- ATOMICITY: the body is read BEFORE and AFTER the speaker — if it changed in
-- between, the window is mid-transition and the read is retried next tick. Reading
-- name and body at slightly different moments paired one line's speaker with the
-- NEXT line's text (the "mixed subtitles" reports, 2026-07-06).
local function line_from(w, name_prop, body_prop)
    if not Core.on_screen(w) then return nil end
    local body = node_text(w[body_prop])
    if not body then return nil end
    local speaker = node_text(w[name_prop])
    if node_text(w[body_prop]) ~= body then return nil end   -- changed mid-read
    return speaker and (speaker .. ": " .. body) or body
end

-- The first instance of `cls` currently showing a line. The game pools SEVERAL
-- instances of each dialogue widget (Xcmn_Subtitles_C_0.._2 live) and swaps which one
-- it drives across scene changes — a single cached instance missed every line spoken
-- through the others (narrator lines unread, 2026-07-06; the multi-instance lesson).
local function line_from_any(cls, name_prop, body_prop)
    for _, w in ipairs(Core.cached_all(cls, tick)) do
        if Core.valid(w) then
            local line = line_from(w, name_prop, body_prop)
            if line then return line end
        end
    end
    return nil
end

function Dialogue.is_active()
    tick = tick + 1
    -- An OPEN overworld menu owns the screen. Every field menu shows the shared
    -- Xcmn_Header_C section header; meanwhile a paused/ambient talk window keeps
    -- reporting on_screen underneath and would shadow the whole menu family
    -- (registered below us) — seen live 2026-07-03: the field menu was unreadable
    -- while an NPC dialogue was paused behind it. Yield while the header is up.
    local hdr = Core.cached_live("Xcmn_Header_C", tick)
    if Core.on_screen(hdr) then cached = nil return false end

    cached = (subtitles_on() and line_from_any("Xcmn_Subtitles_C", "Txt_Name", "Txt_Selif") or nil)
        or line_from_any("Field_Talk_Win_C", "Txt_Speaker", "Txt_Msg")
    return cached ~= nil
end

function Dialogue.reset() ann:reset() end

function Dialogue.update()
    -- The line is the "name": a new line re-announces (interrupt), an unchanged one stays silent.
    ann:focus(nil, nil, cached, nil, nil)
end

return Dialogue
