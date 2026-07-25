-- Screen adapter: story dialogue & subtitles (reactive line reader).
--
-- The single highest-value feature for a blind player: following the story. Unlike a
-- menu, this has no selection — it's a STREAM of lines. We poll the two dialogue
-- surfaces and let the shared Announcer speak each new "Speaker: line" on change
-- (diff-gated, interrupt per line, so a lingering identical line never repeats).
--
-- Surfaces (from the CXX header dump + object dump). There are FOUR, not two — the last two were
-- added 2026-07-25 after a Gohan/Krillin scene where only Gohan read:
--   * Xcmn_Subtitles_C  -> UATUISubtitles      — cutscene / skill / link-bonus voice lines.
--       speaker = Txt_Name, line = Txt_Selif   (BP names; native: TextName / TextSelif)
--   * Field_Talk_Win_C  -> UAT_UIFieldTalkWin  — overworld NPC talk window.
--       speaker = Txt_Speaker, line = Txt_Msg
--   * Field_Navi_Win_C  -> UAT_UIFieldNaviWin (AT.hpp:33020, fm.FieldNaviWin @0x550) — the PORTRAIT
--       pop-up a character speaks through when they are not physically present. The player described
--       it as Krillin being "on the phone"; there is no phone class in this game, this is it.
--       speaker = Txt_Speaker, line = Txt_Msg  (BP names; native: TextBox_Name / TextBox_Message).
--       PROVEN by three F7 dumps 15s apart: its Txt_Msg advanced through the conversation
--       ("Ya sé que quieres relajarte…" → "Así que quería recordarte que sigas entrenando." → "Te
--       mantuviste firme durante el combate…") while Txt_Speaker read "Krillin".
--   * AT_UIFieldTalkFree -> speech BUBBLES (see the block above event_speech_line).
-- Several can hold text at once — the talk window sat on a stale "¿Krillin? ¿Qué sucede?" across two
-- dumps while the navi window advanced — so each surface is gated on being genuinely on screen
-- (Core.on_screen checks the widget's OWN visibility first, so a Collapsed one is rejected) and they
-- are tried in importance order: subtitles, talk window, navi window, bubbles.
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

-- Cutscenes/dialogue own the screen for minutes while the game thread is at its
-- busiest (sequencer, streaming): ui_registry reads this flag to defer steady-state
-- scans (ui_core quiet mode) and relax the 20ms pad dispatcher. Everything a
-- dialogue can open (skip confirm, choices) lives on pooled, already-cached widgets.
Dialogue.scan_quiet = true

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
local Dir = require("ui_directory")
local sub_logged = nil   -- last logged option value / "miss" (log on CHANGE, never per tick)

local function sub_log(what)
    if sub_logged == what then return end
    sub_logged = what
    print("[KakarotAccess] subtitles option: " .. what .. "\n")
end

local function subtitles_on()
    -- The LIVE option comes through the game's OWN ownership chain, pure pointer
    -- reads: GameInstance.SaveManager (UAT_GameInstance @0x1EA0, AT.hpp:30078) →
    -- SaveSystem (@0x108) → Option.EnableSubtitle. Any "first instance found" pick
    -- is a trap on this game: the object array holds SEVERAL managers AND several
    -- save systems (probe 2026-07-17 00:28 — manager _0 → template _1 with pristine
    -- defaults/EnableSubtitle=1, manager _2 → the real _4 with the user's settings),
    -- which is why both prior FindAllOf-based gates read 1 forever. The gi root is
    -- served by the screen directory (no scans); re-resolved every query so a
    -- save/load repoint is followed automatically. FAIL-OPEN as always.
    local savesys
    local mgr = Dir and Dir.peek("gi", "SaveManager")
    if Core.valid(mgr) then
        pcall(function() savesys = mgr.SaveSystem end)
    end
    if not Core.valid(savesys) then
        sub_log("GameInstance.SaveManager.SaveSystem unreachable (fail-open)")
        return true
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
    -- MARKUP ON THE PLAIN SIDE TOO (fixed 2026-07-25). This path returned the string untouched, on
    -- the assumption that only the RICH node carries tags. It does not: a speech bubble spoke
    -- `¡La <span color="#ffba00ff">carne de bestia</> que puedes obtener…` out loud, tag and all.
    -- The other two branches below already run it; the plain one simply never did, and no surface
    -- had exercised it until the bubbles arrived. markup_to_speech returns nil when it cannot make
    -- sense of a string, so keep the raw text as the fallback rather than losing the line.
    if t then return A.markup_to_speech(t) or t end
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
    local body = node_text(Core.member(w, body_prop))
    if not body then return nil end
    local speaker = node_text(Core.member(w, name_prop))
    if node_text(Core.member(w, body_prop)) ~= body then return nil end   -- changed mid-read
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
            if line then return line, w end
        end
    end
    return nil
end

-- THIRD SURFACE — EVENT SPEECH BUBBLES (added 2026-07-25). User: in a Gohan+Krillin scene, Gohan's
-- lines read and Krillin's did not. Not a regression — the log for that session is completely clean
-- (no error, no gate rejection, no pre-check rejection) — but a GAP: the mod knew about two dialogue
-- surfaces and this game has three.
--   UAT_UIFieldTalkFree (AT.hpp:33181), reached by pointer through `fm.FieldTalkFree` @0x658, holds
--   TArrays of UAT_UIFieldTalkFreeCore (AT.hpp:33202) whose `TextBox` @0x3E0 is the line.
-- Only `EventSpeechWidgetArray` @0x458 is read, NOT `FreeTalkWidgetAry` @0x3A0. That is deliberate:
-- the event array is fed by `RequestOpenEventSpeak(... FName InSpeakerUniqueId, AQuestCharacter*
-- InOwner, FString inString ...)`, i.e. quest/script-driven speech — Krillin's story line. The other
-- array is ambient NPC chatter, and piping that into an interrupting reader would talk over the
-- story it is meant to serve. Ambient stays unread until it is asked for on purpose.
-- Liveness uses the widget's OWN `IsEnd()` rather than `on_screen`: it is the game's authoritative
-- "this bubble has finished" signal, and on_screen has twice today rejected hosts that were plainly
-- up. on_screen is the fallback only when IsEnd cannot be read.
-- No speaker node exists on the core (the speaker is an ICON, via the parent's `SpeakerIconMap`
-- FName→Texture2D), so these lines read bare. Naming them would mean mapping icon FNames to
-- character names — the CHAR_TOKENS trick from screen_community — and is not attempted here.
local function bubble_text(core)
    if not Core.valid(core) then return nil end
    -- pane_live FIRST (added 2026-07-25 after the user's log proved it necessary): the trace showed
    -- exactly ONE bubble line in a whole session — a hunting hint — and the player heard THAT line
    -- read out while Krillin was speaking. So a finished bubble lingers with its text intact and
    -- `IsEnd()` alone does not reject it. `pane_live` is the project's own rule for pooled panes
    -- (CLAUDE.md §8) and the right test here: these cores fade out through AnimOut, so a spent
    -- bubble drops below the opacity floor even while its visibility flags lag. Omitting it was my
    -- mistake — the rule exists precisely for this shape and I did not apply it to a new adapter.
    if not Core.pane_live(core) then return nil end
    local ended
    local ok = pcall(function() ended = core:IsEnd() end)
    if ok then
        if ended == true then return nil end
    elseif not Core.on_screen(core) then
        return nil
    end
    return node_text(Core.member(core, "TextBox"))
end

-- BOUNDED TRACE of every DISTINCT bubble line seen. This is the artefact that decides the next
-- report: if a line the player did not hear appears here, the surface saw it and the fault is
-- downstream (priority, announcer, dedup); if it never appears, the surface still cannot see it and
-- there is a fourth one. Capped, and distinct-only, so it cannot become log spam.
local seen_bubbles, seen_n = {}, 0
local BUBBLE_LOG_MAX = 20
local function trace_bubble(src, t)
    if seen_bubbles[t] or seen_n >= BUBBLE_LOG_MAX then return end
    seen_bubbles[t] = true
    seen_n = seen_n + 1
    print(string.format("[KakarotAccess] bubble[%s]: %s\n", src, t:sub(1, 70)))
end

-- BOTH arrays (reversed 2026-07-25, same day). The first cut read only EventSpeechWidgetArray and
-- deliberately skipped FreeTalkWidgetAry as "ambient chatter that would talk over the story". Then a
-- bubble DID read — a hunting hint about beast meat — while Krillin's conversation, which the player
-- describes as happening "on the phone", still did not. The class has SpecialFrameBorder /
-- SpecialIconImage and `RequestOpenEventSpeak(..., bool bInUseSpecialFrame, ...)`: the "phone" is a
-- special-framed bubble, and the game evidently routes it through the other array. Excluding a whole
-- array to pre-empt a noise problem cost the very line this work exists to read — the wrong trade.
-- Event speech still has PRIORITY (checked first); ambient is read only when there is nothing else,
-- and the diff-gated announcer means a stable bubble speaks once. If ambient does turn out to be
-- chatty in play, throttle it THEN, with evidence.
local function event_speech_line()
    for _, host in ipairs(Core.cached_all("AT_UIFieldTalkFree", tick)) do
        if Core.valid(host) then
            for _, prop in ipairs({ "EventSpeechWidgetArray", "FreeTalkWidgetAry" }) do
                local arr, n = Core.array_of(host, prop)
                if arr then
                    for i = 1, n do
                        local core
                        if pcall(function() core = arr[i] end) then
                            local t = bubble_text(core)
                            if t then
                                trace_bubble(prop, t)
                                return t, core
                            end
                        end
                    end
                end
            end
        end
    end
    return nil
end

-- TEMP trace (2026-07-17): one log line per NEW spoken line naming the SOURCE surface,
-- its render state (vis/opacity/pane_live) and the widget instance — the data to close
-- the "subtitles read though the game option is OFF" bug: it says whether the lines
-- come from Xcmn_Subtitles_C (gate broken) or Field_Talk_Win_C (never gated, by
-- design), and whether pane_live discriminates the option-off state. Turn OFF after.
local SUB_TRACE = false
local trace_last = nil
local function trace_line(src, w, line)
    if not SUB_TRACE or line == trace_last then return end
    trace_last = line
    local wn, vis, op = "?", "?", "?"
    pcall(function() wn = w:GetFName():ToString() end)
    pcall(function() vis = tostring(w:GetVisibility()) end)
    pcall(function() op = string.format("%.2f", w:GetRenderOpacity()) end)
    print(string.format("[KakarotAccess] line src=%s(%s) vis=%s op=%s live=%s: %s\n",
        src, wn, vis, op, tostring(Core.pane_live(w)), line:sub(1, 40)))
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

    local line, w, src
    if subtitles_on() then
        line, w = line_from_any("Xcmn_Subtitles_C", "Txt_Name", "Txt_Selif")
        if line then src = "Xcmn_Subtitles_C" end
        -- ALSO the NATIVE class, with the NATIVE member names (added 2026-07-25). The BP names above
        -- came from an old ObjectDump; the header says the native parent `UATUISubtitles`
        -- (AT.hpp:29657) declares `TextName` @0x3F8 and `TextSelif` @0x400. Two ways that matters:
        -- a subtitle instance of a DIFFERENT Blueprint subclass (or of the native class itself) is
        -- invisible to a search for `Xcmn_Subtitles_C`, and a BP subclass may expose only the native
        -- bindings. Searching the base class costs one cached lookup and covers both. It does NOT
        -- explain the Krillin case on its own — that still needs the dump asked for below — but it is
        -- a real gap found while looking, and closing it is free.
        if not line then
            line, w = line_from_any("ATUISubtitles", "TextName", "TextSelif")
            if line then src = "ATUISubtitles" end
        end
    end
    if not line then
        line, w = line_from_any("Field_Talk_Win_C", "Txt_Speaker", "Txt_Msg")
        if line then src = "Field_Talk_Win_C" end
    end
    -- The NAVI WINDOW — the remote-speaker portrait pop-up. Tried after the talk window (a
    -- face-to-face line is the more immediate one) and before the bubbles. Both name sets: the BP
    -- tree exposes Txt_Speaker/Txt_Msg (what the F7 census showed) while the native parent declares
    -- TextBox_Name @0x400 / TextBox_Message @0x408, and a differently-generated Blueprint may only
    -- carry the native bindings — the same trap the subtitles surface has just above.
    if not line then
        line, w = line_from_any("Field_Navi_Win_C", "Txt_Speaker", "Txt_Msg")
        if not line then
            line, w = line_from_any("Field_Navi_Win_C", "TextBox_Name", "TextBox_Message")
        end
        if line then src = "Field_Navi_Win_C" end
    end
    -- Event speech bubbles LAST: a formal dialogue window or a cutscene subtitle is always the more
    -- important surface, so a bubble may only speak when neither of those has a line.
    if not line then
        line, w = event_speech_line()
        if line then src = "AT_UIFieldTalkFree" end
    end
    if line and w then trace_line(src, w, line) end
    cached = line
    return cached ~= nil
end

function Dialogue.reset() ann:reset() end

function Dialogue.update()
    -- The line is the "name": a new line re-announces (interrupt), an unchanged one stays silent.
    ann:focus(nil, nil, cached, nil, nil)
end

return Dialogue
