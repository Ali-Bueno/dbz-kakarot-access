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

-- Reflected native UPROPERTYs of UAT_UIGameWindow that hold guidance-capable text
-- wrappers (AT.hpp:33329 / 33344 / 33345 — letter-for-letter).
local MEMBERS = { "WL_TxtDetail", "WL_TxtHelp", "WL_WorkText" }

-- Same cost rules as keyhelp_watch: runs inside the reader's poll step, touches only
-- the cached/directory-mapped window (Xcmn_Win01_C = wm.GameWindowCore, pointer read).
local POLL_EVERY = 5     -- ticks (Core.POLL_MS = 100 ms) between probes
local STABLE_POLLS = 2   -- a new text must survive one more poll (boxes animate in)

local spoken_sig = nil   -- last guidance announced; text-keyed, never cleared per screen
local cand, cand_n = nil, 0
local tick = 0           -- monotonic: doubles as the cache clock (keyhelp_watch lesson)

-- The rich text of one wrapper, speakable, or nil. The wrapper AND the rich block must
-- be on screen/visible — the plain side is never consulted (stale by design here).
local function rich_text(node)
    if not Core.valid(node) or not Core.on_screen(node) then return nil end
    local rich
    if not pcall(function() rich = node.ExMainTxt end) or not Core.valid(rich) then
        return nil
    end
    local okv, vis = pcall(function() return rich:IsVisible() end)
    if not okv or vis ~= true then return nil end
    local ok, s = pcall(function() return rich.Text:ToString() end)
    if ok and s and s ~= "" then return A.markup_to_speech(s) end
    return nil
end

-- Called after the active adapter's update(), transition-gated by the registry.
function W.update()
    tick = tick + 1
    if tick % POLL_EVERY ~= 0 then return end

    local win = Core.cached_live("Xcmn_Win01_C", tick)
    local sig = nil
    if Core.on_screen(win) then
        local parts = {}
        for _, m in ipairs(MEMBERS) do
            local t
            pcall(function() t = rich_text(win[m]) end)
            if t then parts[#parts + 1] = t end
        end
        if #parts > 0 then sig = table.concat(parts, ", ") end
    end
    -- Nothing showing, or already spoken: keep watching. An empty read never clears
    -- the latch (teardowns blank first; stale text must not re-announce later).
    if not sig or sig == spoken_sig then cand, cand_n = nil, 0 return end

    if sig ~= cand then cand, cand_n = sig, 1 return end
    cand_n = cand_n + 1
    if cand_n < STABLE_POLLS then return end

    spoken_sig, cand, cand_n = sig, nil, 0
    Speech.say(sig, false)
end

return W
