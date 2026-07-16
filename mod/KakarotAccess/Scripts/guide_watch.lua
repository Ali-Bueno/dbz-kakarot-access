-- Automatic reader for the game's TUTORIAL GUIDANCE line: the persistent instruction
-- box the game pins over a screen while a tutorial task is pending ("After selecting
-- Gohan, press A to make him available." — the community-tutorial episode,
-- 2026-07-16). Without it the task is invisible and the tutorial is unfinishable.
--
-- WHERE the text lives (dumps 2026-07-16 + AT.hpp:33299 UAT_UIGameWindow): the shared
-- message window Xcmn_Win01_C is REUSED for these boxes, with the instruction in the
-- RICH side (ExMainTxt) of its text wrappers (WL_TxtDetail / WL_TxtHelp / WL_WorkText).
-- The PLAIN side (mainTxt, what screen_dialog reads) keeps STALE text from the
-- window's previous use (Txt_Work.mainTxt still said "Sí" twenty minutes later), and
-- every visibility signal on this game lies for pooled widgets — so ONLY the rich
-- side is read here, gated on the rich block itself being visible and non-empty,
-- which held for exactly one node in the captured tutorial moment.
--
-- Cross-screen by design (the keyhelp_watch pattern): hangs off the registry
-- dispatcher, so the instruction reads over WHATEVER screen it covers (board, grid,
-- future tutorials), with no adapter of its own that could shadow one. Announced
-- queued (interrupt=false) behind the active screen's readout, and ONLY when the text
-- CHANGES — the latch is text-keyed and survives screen changes on purpose: rich
-- nodes on this game may keep stale text after the task completes, and clearing the
-- latch per screen would re-blurt it on every menu.

local Core = require("ui_core")
local A = require("ui_archetypes")
local Speech = require("speech")

local W = {}

-- The BLUEPRINT text-wrapper widgets on Xcmn_Win01_C (NOT the native UAT_UIGameWindow
-- WL_* fields — that was the bug: guide_watch read WL_TxtDetail and found nothing,
-- so instructions NEVER reached speech; the F7 path is Xcmn_Win01_C_0.WidgetTree_0.
-- **Txt_Detail**.WidgetTree_0.ExMainTxt, and screen_dialog reads the same blueprint
-- names). The tutorial instruction lives in Txt_Detail's rich side.
local MEMBERS = { "Txt_Detail", "Txt_Help", "Txt_Work" }

-- Same cost rules as keyhelp_watch: runs inside the reader's poll step, touches only
-- the cached/directory-mapped window (Xcmn_Win01_C = wm.GameWindowCore, pointer read).
local POLL_EVERY = 5     -- ticks (Core.POLL_MS = 100 ms) between probes
local STABLE_POLLS = 2   -- a new text must survive one more poll (boxes animate in)

local spoken_sig = nil   -- last guidance announced; text-keyed, never cleared per screen
local cand, cand_n = nil, 0
local tick = 0           -- monotonic: doubles as the cache clock (keyhelp_watch lesson)

-- The GUIDANCE text of one wrapper, speakable, or nil. A node is a guidance strip
-- when its PLAIN side (mainTxt) is EMPTY and the text lives solely in the rich side
-- (ExMainTxt) AND the rich block itself reports IsVisible()==true. Two load-bearing
-- facts (2026-07-16, speech log + F7 dumps):
--  * A NOTICE (the "Emblemas de alma recibidos" reward) carries its text in mainTxt —
--    so the plain-empty test keeps notices out of here (reading their rich mirror
--    would misread them and make screen_dialog yield them into silence).
--  * The instruction box renders through a Win01 whose CONTAINER reports Collapsed
--    (F7 census: Xcmn_Win01_C_0 enum=1:Collapsed) while its ExMainTxt rich block is
--    IsVisible==true — so an on_screen() ancestor-walk WRONGLY rejects it and the
--    guide never fired (user: instructions never spoken). Gate on the rich block's OWN
--    IsVisible, exactly how discover.lua finds "visible rich text", NOT on_screen.
local function rich_text(node)
    if not Core.valid(node) then return nil end
    local rich
    if not pcall(function() rich = node.ExMainTxt end) or not Core.valid(rich) then
        return nil
    end
    local okv, vis = pcall(function() return rich:IsVisible() end)
    if not okv or vis ~= true then return nil end
    -- Discriminator by VISIBILITY, not text: a NOTICE renders through the PLAIN block
    -- (mainTxt IsVisible) and a GUIDE through the RICH block. The old "plain text is
    -- empty" test failed because the pooled node keeps STALE plain text (so the guide
    -- was never detected — instructions never spoke, user 2026-07-16). If the plain
    -- block is itself shown, this is a notice; skip it (screen_dialog reads it).
    local plain
    pcall(function() plain = node.mainTxt end)
    if Core.valid(plain) then
        local okp, pv = pcall(function() return plain:IsVisible() end)
        if okp and pv == true then return nil end
    end
    local ok, s = pcall(function() return rich.Text:ToString() end)
    if ok and s and s ~= "" then return A.markup_to_speech(s) end
    return nil
end

-- The guidance currently showing on a window (concatenated rich texts), or nil. No
-- on_screen gate on the window — its container may be Collapsed while the rich block
-- renders (see rich_text); the per-block IsVisible is the real signal.
local function guidance_of(win)
    if not Core.valid(win) then return nil end
    local parts = {}
    for _, m in ipairs(MEMBERS) do
        local t
        pcall(function() t = rich_text(win[m]) end)
        if t then parts[#parts + 1] = t end
    end
    if #parts == 0 then return nil end
    return table.concat(parts, ", ")
end

-- Scan ALL Xcmn_Win01_C instances (the class is pooled — cached_live's single pick
-- was often the wrong/stale one) and return the guidance any of them is showing.
local function current_guidance()
    for _, w in ipairs(Core.cached_all("Xcmn_Win01_C", tick)) do
        local g = guidance_of(w)
        if g then return g end
    end
    return nil
end

-- Is a guide layer showing right now? screen_dialog yields on this (a rich-side
-- instruction means any plain text on the same pooled window is stale leftovers). The
-- `win` arg is ignored — the guide box is often a DIFFERENT Win01 instance than the
-- notice window screen_dialog acquired, so we scan them all.
function W.showing(_win)
    return current_guidance() ~= nil
end

-- TEMP trace (2026-07-16: instructions never reached speech). One line per detected
-- guidance change. Turn OFF once instructions read.
local GUIDE_TRACE = false
local guide_trace_last = nil

-- Called every registry tick (with or without an active adapter — a guide box can be
-- pinned over free roam too), transition-gated by the registry.
function W.update()
    tick = tick + 1
    if tick % POLL_EVERY ~= 0 then return end

    local sig = current_guidance()
    if GUIDE_TRACE and sig ~= guide_trace_last then
        guide_trace_last = sig
        print("[KakarotAccess] guide " .. tostring(sig and sig:sub(1, 60)) .. "\n")
    end
    -- Nothing showing, or already spoken: keep watching. An empty read never clears
    -- the latch (teardowns blank first; stale text must not re-announce later).
    if not sig or sig == spoken_sig then cand, cand_n = nil, 0 return end

    if sig ~= cand then cand, cand_n = sig, 1 return end
    cand_n = cand_n + 1
    if cand_n < STABLE_POLLS then return end

    spoken_sig, cand, cand_n = sig, nil, 0
    -- Protected: this is the actionable task ("select an empty panel, press A"); it
    -- must win over and not be shredded by the board/grid panel readout underneath it.
    Speech.say_protected(sig)
end

-- On-demand repeat (F1): re-speak the guidance currently showing, if any. Live read,
-- not the latch — F1 must reflect what is on screen NOW. Returns true if spoken.
function W.reannounce()
    local sig = current_guidance()
    if not sig then return false end
    Speech.say_protected(sig)
    return true
end

return W
