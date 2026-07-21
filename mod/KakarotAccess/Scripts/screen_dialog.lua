-- Screen adapter: system dialog / message window + Yes/No confirmations.
--
-- Two shared, game-wide patterns handled together:
--   * MESSAGE popup  — Xcmn_Win01_C with a prompt in Txt_Detail (+ optional Txt_Title),
--     e.g. the New Game "Select the battle difficulty…" notice.
--   * CONFIRMATION   — the same window plus Yes/No option buttons (Xcmn_Win00_Choice_C),
--     e.g. "Skip the movie?". Each choice is a Txt_Choice label; the SELECTED one has
--     its Img_Xwin01_List highlight visible. Only the TOP-LEVEL choice instances
--     (direct children of the GameInstance) are the active shared confirmation — the
--     ones nested under another screen are that screen's own pooled/stale widgets.
--
-- Text may carry CFramework markup (<span color=…>, <inputicon…>), resolved by
-- A.markup_to_speech. Registered FIRST so it overlays other screens while up.

local Core = require("ui_core")
local A = require("ui_archetypes")
local Guide = require("guide_watch")
local Speech = require("speech")
local I18n = require("i18n")
local Transition = require("transition")

local Dialog = {}

-- Passive / time-critical reader: excluded from the automatic keyhelp read
-- (keyhelp_watch.lua) — its prompts are either urgent or already spoken here.
Dialog.keyhelp_auto = false

local ann = Core.make_announcer()
local win = nil
local tick = 0
local state = nil   -- {msg, labels, sel} computed once per tick in is_active, reused by update

-- Speakable text of a window text node, but ONLY if it's really on screen — a node that
-- IsVisible under a collapsed ancestor keeps STALE text (e.g. a prior "Difficulty
-- Settings" title bleeding into an unrelated "Skip the movie?" confirmation), so gate on
-- on_screen, not raw text.
local function node_speech(node)
    if not Core.on_screen(node) then return nil end
    return A.markup_to_speech(Core.text_of(node))
end

-- Like node_speech but via Core.read_text (reflected GetText() fallback), for nodes that
-- may be native CFUIXcmnMultiLineText rather than the blueprint wrapper.
local function node_rt(node)
    if not Core.on_screen(node) then return nil end
    return A.markup_to_speech(Core.read_text(node))
end

-- The speakable prompt of a dialog window (title + body, markup resolved), or nil.
-- The pooled window has TWO notice LAYOUTS on the same class (win01 enum 2026-07-16):
--   * TITLED (rewards): Txt_Title + Txt_Help — e.g. "Emblemas de alma recibidos" +
--     "Recibiste los siguientes emblemas de alma:".
--   * UNTITLED (simple notices / confirmations): Txt_Detail — e.g. "¡El Ki y los PS…",
--     "¿Confirmas que…".
-- CRUCIAL: the game does NOT clear Txt_Detail between layouts, so a TITLED reward
-- window keeps the PREVIOUS untitled notice's text stale in Txt_Detail ("¡El Ki y los
-- PS…" bleeding under "Emblemas de alma recibidos") — reading both mixed them and the
-- stale one often won (user 2026-07-16). So: title present ⇒ read ONLY Title + Help
-- (Txt_Detail is the other layout's slot, stale here); title absent ⇒ read Txt_Detail
-- + the special captions. WARNING (round 15): Txt_Title is NOT reliably cleared
-- between layouts either (the 20:04 líder notice reopened with the prior reward's
-- title still set) — so this composite is only good for "the window has something"
-- (acquire) and as the choice-window prompt. What a NOTICE should actually SAY is
-- decided by fresh_notice() (per-node novelty), never by this function.
local UNTITLED_MEMBERS = {
    "Txt_Detail", "TextBox_Caption", "Txt_Detail_Item", "TextBox_OldNum", "TextBox_NewNum",
}
local function message(w)
    if not Core.valid(w) then return nil end
    -- Title/help gated on their OWN rendered state (node_rt → on_screen), round 19:
    -- the pooled window keeps a stale "Ajustes de dificultad" title in TEXT long after
    -- the difficulty box, but the untitled layouts COLLAPSE the title block — the
    -- rendered state is the discriminator text presence never was (the food confirm
    -- read the difficulty title+help glued around its question).
    local title
    pcall(function() title = node_rt(w.Txt_Title) end)
    if title and title ~= "" then
        local body
        pcall(function() body = node_rt(w.Txt_Help) end)
        return (body and body ~= "") and (title .. ", " .. body) or title
    end
    local parts = {}
    for _, m in ipairs(UNTITLED_MEMBERS) do
        local t
        pcall(function() t = node_speech(w[m]) end)
        -- consecutive-duplicate guard: the Win02 info box repeats its header as the
        -- subtitle ("Z Warrior Community, Z Warrior Community" read live)
        if t and t ~= parts[#parts] then parts[#parts + 1] = t end
    end
    if #parts == 0 then return nil end
    return table.concat(parts, ", ")
end

-- Content-row text via IsVisible (own flag), NOT on_screen (ancestor walk): the reward
-- window's Goku/Gohan rows flicker on/off the on_screen test during its transition, so
-- the ancestor-gated read returned nil at the single tick the notice speaks and the
-- names were lost (user 2026-07-16). The rows are the reward's own children — their own
-- IsVisible is the right, flicker-stable signal.
local function cell(node)
    if not Core.valid(node) or not Core.is_visible(node) then return nil end
    return A.markup_to_speech(Core.read_text(node))
end

-- Content rows of the window's typed pools (native UAT_UIGameWindow, Xcmn_Win01_C's
-- base class): item plates (name + count), community/soul-emblem rows (character name +
-- "Community Level" title + level numbers), link-bonus rows, check rows, and the single
-- detail-reward line. Reward/notice windows ("Soul Emblems Received", quest "Rewards")
-- show these BELOW the prompt — without reading them only the title+body spoke. Each
-- pool is pcall-guarded, so Xcmn_Win00_C (which has none of these members) skips
-- harmlessly. WL_TextPlateCtn is NOT here: those rows are selectable choices, handled
-- with selection semantics in choices() below.
-- WL_TextCmuCtn is NOT here: its rows are the soul-emblem reward rows, read with their
-- own name+level formatting by emblems_received(). Its old pool entry also read the row
-- member WL_LvTextList — a native FIXED C array (UCFUIXcmnMultiLineText*[7],
-- AT.hpp:33412), which UE4SS collapses into a RemoteObject that PASSES IsValid yet whose
-- GetArrayNum throws the pcall-piercing C++ error: it aborted the whole update() tick
-- mid-notice, leaving the notice latch half-set — the reward then spoke the PREVIOUS
-- notice's stale text, or the bare title with no emblem names (UE4SS.log 2026-07-16
-- 19:43:37 / 19:45:19). Never hand a fixed-array member to Core.array_of.
local POOLS = {
    { arr = "WL_ItemPlateCtn",     texts = { "ItemName", "ItemNum" } },
    { arr = "WL_LinkListPlateCtn", texts = { "LinkNameText" } },
    { arr = "WL_CheckPlateCtn",    texts = { "TxtList" } },
}

-- The soul-emblem REWARD rows ("Emblemas de alma recibidos"): the window's
-- WL_TextCmuCtn TArray (AT.hpp:33362) points at the pooled top-level Xcmn_Emb_List_C
-- widgets, one per received emblem (_0 = Goku, _1 = Gohan). (The round-12 claim that
-- this pool is EMPTY came from a parked dump moment — the 19:43:37 traceback proves it
-- is populated while the notice is live.) Read Txt_Name + Txt_Num00 (the
-- community-level NUMBER; Txt_Commu_Lv is only the fixed caption "Nivel de
-- comunidad"), falling back to the native WL_CharName if the row isn't the BP
-- subclass. Read by IsVisible (own flag) — pooled rows keep stale content and the
-- ancestor-gated on_screen flickers during the notice transition.
-- Returns a LIST of row strings (not a joined text): the caller filters out rows
-- already spoken recently — the pooled rows stay visible with stale content, so a
-- later titled notice on the same window would otherwise re-fold them (the reward's
-- Goku/Gohan rows re-read under "Bonificaciones"/"Objetos ganados", 20:23:57).
local function emblems_received(w)
    local parts = {}
    local arr, n = Core.array_of(w, "WL_TextCmuCtn")
    if not arr then return parts end
    pcall(function()
        for i = 1, n do
            local row = arr[i]
            if Core.valid(row) and Core.is_visible(row) then
                local name, lv
                pcall(function() name = cell(row.Txt_Name) end)
                if not name then pcall(function() name = cell(row.WL_CharName) end) end
                pcall(function() lv = cell(row.Txt_Num00) end)
                if name then
                    parts[#parts + 1] = lv
                        and (name .. ", " .. string.format(I18n.t("commu_lv"), lv))
                        or name
                end
            end
        end
    end)
    return parts
end

-- Same list contract as emblems_received (see its note).
local function plates(w)
    local parts = {}
    if not Core.valid(w) then return parts end
    for _, pool in ipairs(POOLS) do
        local arr, n = Core.array_of(w, pool.arr)
        if arr then pcall(function()
            for i = 1, n do
                local row = arr[i]
                if Core.valid(row) and Core.is_visible(row) then
                    local p = {}
                    for _, m in ipairs(pool.texts) do
                        p[#p + 1] = cell(row[m])
                    end
                    if #p > 0 then parts[#parts + 1] = table.concat(p, ", ") end
                end
            end
        end) end
    end
    pcall(function()
        local dr = w.WL_DetailReward
        if Core.valid(dr) and Core.on_screen(dr) then
            local s = Core.phrase(node_rt(dr.TxtReward), node_rt(dr.TxtName), node_rt(dr.TxtNum))
            if s ~= "" then parts[#parts + 1] = s end
        end
    end)
    return parts
end

-- The active Yes/No confirmation options + the currently selected one. Active options
-- are the TOP-LEVEL Xcmn_Win00_Choice_C (direct GameInstance children) that are actually
-- on screen (on_screen, not just IsVisible — the pool keeps stale "Yes/No" choices whose
-- collapsed ancestor IsVisible ignores); selection is the one whose Img_Xwin01_List
-- highlight is visible.
local function choices()
    local labels, sel, items = {}, nil, false
    for _, ch in pairs(Core.cached_all("Xcmn_Win00_Choice_C", tick)) do
        if Core.on_screen(ch)
           and ch:GetFullName():match("BP_ATGameInstance_C_%d+%.Xcmn_Win00_Choice_C_%d+$") then
            local label = Core.text_of(Core.member(ch, "Txt_Choice"))
            if label then
                labels[#labels + 1] = label
                if Core.is_visible(Core.member(ch, "Img_Xwin01_List")) then sel = label end
            end
        end
    end
    -- Choice lists OWNED by the window, NESTED under its tree (the top-level filter
    -- above never sees them). Two row shapes, handled with member fallbacks:
    --   * Win01's WL_TextPlateCtn (Sort popup) = Xcmn_Win00_Choice_C: Txt_Choice +
    --     Img_Xwin01_List highlight.
    --   * Win02's UIChoice_List (the brown field DECISION box — "Check out this area
    --     a bit more / Return home") = UAT_UISystemWindowChoice: ChoiceTxt label, and
    --     the selected row shows its BasePlate (the yellow bar).
    for _, pool in ipairs({ "WL_TextPlateCtn", "UIChoice_List" }) do
        local arr, n = Core.array_of(win, pool)
        if arr then pcall(function()
            for i = 1, n do
                local ch = arr[i]
                if Core.valid(ch) and Core.on_screen(ch) then
                    local label
                    pcall(function() label = A.markup_to_speech(Core.read_text(ch.Txt_Choice)) end)
                    if not label then
                        pcall(function() label = A.markup_to_speech(Core.read_text(ch.ChoiceTxt)) end)
                    end
                    if label then
                        labels[#labels + 1] = label
                        local hi = false
                        pcall(function()
                            hi = Core.is_visible(ch.Img_Xwin01_List) or Core.is_visible(ch.BasePlate)
                        end)
                        if hi then sel = label end
                    end
                end
            end
        end) end
    end
    -- SELECTABLE ITEM lists (the community GIFT picker, and any other window whose
    -- WL_ItemPlateCtn rows carry the cursor): each UAT_UIGameWindowChoice row is
    -- ItemName + ItemNum with the same Xwin01List highlight (native member name has
    -- no underscores). Only engage when SOME row is highlighted — reward/notice
    -- windows show the same rows with NO cursor, and those must keep the read-once
    -- notice behavior (plates() reads them as content).
    local iarr, icount = Core.array_of(win, "WL_ItemPlateCtn")
    if iarr then pcall(function()
        local ilabels, isel = {}, nil
        for i = 1, icount do
            local row = iarr[i]
            if Core.valid(row) and Core.on_screen(row) then
                local label = Core.phrase(
                    A.markup_to_speech(Core.read_text(row.ItemName)),
                    A.markup_to_speech(Core.read_text(row.ItemNum)))
                if label ~= "" then
                    ilabels[#ilabels + 1] = label
                    local hi = false
                    pcall(function() hi = Core.is_visible(row.ImgXwin01List) end)
                    if not hi then
                        pcall(function() hi = Core.is_visible(row.Img_Xwin01_List) end)
                    end
                    if hi then isel = label end
                end
            end
        end
        if isel then
            for _, l in ipairs(ilabels) do labels[#labels + 1] = l end
            sel = isel
            items = true
        end
    end) end
    return labels, sel, items
end

-- Live description of the selected item row (the pane under the list — the gift
-- picker repopulates it per cursor move). Tooltip in item-list mode only.
local function item_detail()
    local t
    pcall(function() t = node_rt(win.WL_TxtDetailItem) end)
    return t
end

-- The shared message-window classes: Xcmn_Win01_C (dialogs/confirmations/notices),
-- Xcmn_Win00_C (startup/loading system messages — autosave notice, "Checking system
-- data…", "Saving game…") and Xcmn_Win02_C (the brown tutorial/level-up box —
-- TextBox_Caption + old/new numbers, choices in UIChoice_List).
local WINDOW_CLASSES = { "Xcmn_Win01_C", "Xcmn_Win00_C", "Xcmn_Win02_C" }

-- Does this window own any on-screen choice rows? A field DECISION box (the brown
-- Xcmn_Win02 "Check out this area a bit more / Return home" prompt) has options but NO
-- message text, so message() alone would miss it — acquire() also accepts choice-only
-- windows. Cheap: the pool is small and collapsed rows fail the on_screen check.
local function window_has_choices(w)
    if not Core.valid(w) then return false end
    for _, pool in ipairs({ "WL_TextPlateCtn", "UIChoice_List" }) do
        local found = false
        local arr, n = Core.array_of(w, pool)
        if arr then pcall(function()
            for i = 1, n do
                local ch = arr[i]
                if Core.valid(ch) and Core.on_screen(ch) then found = true return end
            end
        end) end
        if found then return true end
    end
    return false
end

-- TEMP (2026-07-16): enumerate EVERY Xcmn_Win01_C instance with its per-node text, to
-- see whether the reward ("Emblemas de alma recibidos") lives on a DIFFERENT pooled
-- instance than the stale "Ki y PS" notice that keeps winning. cached_live/directory
-- return only ONE instance; a real FindAllOf shows them all. Prints on change only.
local enum_last = nil
local function enum_windows()
    local rows = {}
    for _, w in ipairs(FindAllOf("Xcmn_Win01_C") or {}) do
        if Core.valid(w) then
            local on = Core.on_screen(w)
            -- per-node: text (raw) + its OWN rendered state (v1/v0) — the round-19
            -- discriminator; the flags say whether the visibility gate can be trusted
            local function cellinfo(node)
                local t, v
                pcall(function() t = Core.text_of(node) end)
                pcall(function() v = Core.on_screen(node) end)
                return string.format("%s(v%s)", tostring(t and t:sub(1, 18)), v and "1" or "0")
            end
            local ti, de, he
            pcall(function() ti = cellinfo(w.Txt_Title) end)
            pcall(function() de = cellinfo(w.Txt_Detail) end)
            pcall(function() he = cellinfo(w.Txt_Help) end)
            local nm = "?"
            pcall(function() nm = w:GetFullName():match("(Xcmn_Win01_C_%d+)") or "?" end)
            rows[#rows + 1] = string.format("%s on=%s ti=%s de=%s he=%s",
                nm, tostring(on), tostring(ti), tostring(de), tostring(he))
        end
    end
    local sig = table.concat(rows, " | ")
    if sig ~= enum_last then
        enum_last = sig
        print("[KakarotAccess] win01 " .. (sig == "" and "(none)" or sig) .. "\n")
    end
end

-- Find a live window ACTUALLY on screen (on_screen, not IsVisible) that shows text OR
-- offers choices. Uses the persistent cache (these windows are pooled and persist), so
-- this is a cheap on_screen check on a cached ref, not a per-tick FindAllOf scan.
local function acquire()
    win = nil
    for _, cls in ipairs(WINDOW_CLASSES) do
        local w = Core.cached_live(cls, tick)
        if Core.on_screen(w) and (message(w) or window_has_choices(w)) then win = w return end
    end
end

-- Active ONLY when a message window is really on screen. The Yes/No choices are separate
-- top-level widgets, so we tie them to a shown window rather than trusting their own
-- IsVisible — otherwise pooled stale confirmations (still IsVisible under a collapsed
-- ancestor) would keep this adapter "active" over the pause menu / tutorials.
-- DEBUG: when a window is up, append its class, message and EVERY content/choice
-- pool row (class + texts + highlight flag) to dumps/dump_dialog.txt on each message
-- change — for pinning windows whose selectable rows aren't read yet (live case:
-- a first-time community notice with 2 side-by-side options that stayed silent).
-- Turn OFF once those rows are identified.
local DEBUG = false
local last_dumped = nil
local DLG_TRACE = false    -- log dialogue acquire/skip decisions (rounds 5-21 diagnosis)
local ENUM_WINDOWS = false -- win01 enum incl. per-node rendered flags (round 19-20 evidence)
local dlg_last = nil

local DUMP_POOLS = {
    "WL_ItemPlateCtn", "WL_TextCmuCtn", "WL_LinkListPlateCtn", "WL_CheckPlateCtn",
    "WL_TextPlateCtn", "UIChoice_List", "Relation_List", "WL_StatePlateCtn",
}
local ROW_TEXTS = {
    "Txt_Choice", "ChoiceTxt", "ItemName", "ItemNum", "WL_CharName", "WL_LevelTitle",
    "TxtList", "LinkNameText", "TxtName", "TxtNum",
}

local function dump_window(msg)
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    local f = io.open(dir .. "\\dumps\\dump_dialog.txt", "a")
    if not f then return end
    local wname = "?"
    pcall(function() wname = win:GetFullName():match("^%S+ (%S+)") or "?" end)
    f:write(string.format("[%d] win=%s msg=%s\n", os.time(), wname, tostring(msg)))
    for _, pool in ipairs(DUMP_POOLS) do
        local arr, n = Core.array_of(win, pool)
        if arr then pcall(function()
            for i = 1, n do
                local row = arr[i]
                if Core.valid(row) and Core.on_screen(row) then
                    local cls = row:GetFullName():match("^(%S+)") or "?"
                    local texts = {}
                    for _, m in ipairs(ROW_TEXTS) do
                        local t
                        pcall(function() t = Core.read_text(row[m]) end)
                        if t then texts[#texts + 1] = m .. "=" .. t end
                    end
                    local hi = ""
                    pcall(function()
                        if Core.is_visible(row.Img_Xwin01_List) or Core.is_visible(row.BasePlate) then
                            hi = " HIGHLIGHT"
                        end
                    end)
                    f:write(string.format("  %s[%d] %s %s%s\n", pool, i, cls,
                        table.concat(texts, " "), hi))
                end
            end
        end) end
    end
    f:close()
end

-- The last fully-announced no-choice notice still on screen. NOT cleared by reset()
-- (the registry resets adapters on every switch — clearing it there would make a
-- released notice re-claim and re-announce in a loop). NOT cleared on an off-screen
-- BLINK either (community tutorial, 2026-07-16): the parked reward window kept every
-- visibility signal true for 20+ minutes and flickered off/on with each R1/L1 page
-- flip in the emblems grid — clearing the latch on the first off tick re-announced
-- the stale "Received the community leader Goku" notice on every blink, drowning the
-- grid reader. Only a window that STAYS off for SPOKEN_GRACE_S is genuinely closed;
-- a genuinely NEW notice re-claims anyway because its text differs.
-- NEVER cleared by absence (time or blink) — only overwritten when a DIFFERENT
-- message is announced (update). The community-tutorial notice "¡Recibiste al líder…
-- Goku!" stays up through the whole board step and BLINKS off-screen for 40+ seconds
-- at a time; any time-based clear (the earlier SPOKEN_GRACE_S=3 s) let it re-announce
-- on every blink, interrupting "Coloca a Gohan" / "elige un panel vacío" — the notice
-- was EATING the other dialogues (user 2026-07-16, dlg trace). Latching on the text
-- alone means a blink of the SAME notice is suppressed while any genuinely new text
-- still announces (its msg differs from `spoken`). A repeat of the exact same notice
-- much later not re-announcing is the accepted trade.
local spoken = nil        -- last spoken notice (kept only for the DLG_TRACE line)
local notice_msg = nil    -- msg whose folded title+content is cached in notice_full

-- Appearance edge of the window (off-screen → on-screen). For a short window after it,
-- fresh CONTENT ROWS may prove a notice fresh even when title+help repeat verbatim (the
-- Milk-emblem hole, 2026-07-16: a SECOND emblem reward in the same epoch repeats both
-- word-for-word — the recent-set correctly saw nothing fresh and the whole reward went
-- SILENT; the only new text was the row with the emblem's name). Bounded to the edge so
-- the rows walk never runs on the parked steady state (the 20-min parked window).
local ROWS_FRESH_S = 3.0
local appear_t = -1e9
local was_on = false
local notice_full = nil
local notice_extra = nil  -- folded row texts of notice_full, marked recent after speaking
local choice_marked = nil -- last choice prompt whose window nodes were marked recent
local choice_prompt = nil -- per-appearance latched prompt of the current choice window
local choice_key = nil    -- signature (labels+msg) the latched prompt belongs to

-- RECENTLY-spoken notice texts (FIFO). The reward window's Txt_Title FLICKERS present/
-- empty tick-to-tick (cold-boot log 2026-07-16), so message() alternates between the
-- titled "Emblemas de alma recibidos…" and the UNTITLED stale Txt_Detail "¡El Ki y los
-- PS…" (a notice ALREADY spoken moments earlier). A single last-spoken latch can't stop
-- the stale one (the title flips spoken back and forth); a small recent-SET does — any
-- notice said in the last RECENT_MAX is never repeated, so the stale Ki/PS is ignored
-- while the genuinely new "Emblemas…" still speaks the first time it appears.
-- Sized for ~8 notices of history: fresh_notice() marks each spoken NODE text plus the
-- composed utterance (up to ~3 marks per notice), not one entry per notice.
local RECENT_MAX = 24
local recent, recent_set = {}, {}
local function was_recent(m) return m ~= nil and recent_set[m] == true end
local function mark_recent(m)
    if not m or recent_set[m] then return end
    recent_set[m] = true
    recent[#recent + 1] = m
    if #recent > RECENT_MAX then
        local old = table.remove(recent, 1)
        recent_set[old] = nil
    end
end

-- What the notice should SAY now — or nil when it has nothing new. The pooled window is
-- REUSED across notice layouts and clears NEITHER slot reliably: round 11 saw Txt_Title
-- nil'd between titled notices, but at 20:04 (2026-07-16) the untitled "¡Recibiste al
-- líder…" notice reopened with the previous reward's Txt_Title AND Txt_Help still set —
-- the title-presence rule then composed the exact string just spoken and the recent-set
-- swallowed the notice entirely (the one unread dialog of the flow). Text presence lies
-- and visibility lies on this window (stale nodes keep every signal); what discriminates
-- the LIVE layout is per-NODE novelty against the recent-set:
--   * Txt_Title fresh (not recently spoken) — or its Txt_Help fresh (recurring title,
--     new body) ⇒ TITLED notice: Title + Help. Txt_Detail is the other layout's slot,
--     stale here even right after a reload (rounds 12/13) — never read it with a title.
--   * otherwise ⇒ UNTITLED notice: the untitled members that are fresh.
-- Returns composed, titled?, parts (the node texts to mark as recent after speaking).
-- A verbatim repeat re-reads only after RECENT_MAX marks evict it — the trade the
-- recent-set already made. EXCEPTION (rows_ok, see appear_t): fresh content rows
-- rescue a verbatim-repeated titled notice — a new reward carries its news in the
-- rows, not in the boilerplate title/help.
local function fresh_notice(w, rows_ok)
    if not Core.valid(w) then return nil end
    -- Title/help via node_rt — gated on their OWN rendered state (round 19): the
    -- recent-set alone couldn't stop a stale title from re-composing once the FIFO
    -- evicted its mark minutes later ("Ajustes de dificultad" glued around the food
    -- confirm). Rendered-state first, novelty second.
    local title, help
    pcall(function() title = node_rt(w.Txt_Title) end)
    if title == "" then title = nil end
    pcall(function() help = node_rt(w.Txt_Help) end)
    if help == "" then help = nil end
    -- Round 16: there is no clean titled/untitled split — "Bonificaciones" carried a
    -- fresh title AND its live body in Txt_Detail while Txt_Help was stale from the
    -- reward ("Bonificaciones, Recibiste los siguientes emblemas de alma:" glued,
    -- 20:23:57). So EVERY node contributes independently by its own novelty; Txt_Help
    -- additionally requires Txt_Title to be PRESENT (help is the titled layout's
    -- subtitle — during the reward's title flicker the title-less ticks must not
    -- speak the help as a headless fragment).
    local title_fresh = title ~= nil and not was_recent(title)
    local parts = {}
    if title_fresh then parts[#parts + 1] = title end
    for _, m in ipairs(UNTITLED_MEMBERS) do
        local t
        pcall(function() t = node_speech(w[m]) end)
        if t and t ~= "" and not was_recent(t) and t ~= parts[#parts] then
            parts[#parts + 1] = t
        end
    end
    if title and help and not was_recent(help) then
        parts[#parts + 1] = help
    end
    if #parts == 0 then
        -- Verbatim-repeat reward: nothing above is fresh, but a rendered title plus a
        -- fresh content row (item plate / received-emblem row) means a NEW notice that
        -- reuses the previous one's exact wording. Re-speak the stale title as the
        -- header; update() folds the fresh rows in (its notice_extra pass drops the
        -- recently-spoken ones, so only the news reads). Gated on rows_ok — only for
        -- a few seconds after the window's appearance edge, never per-tick on the
        -- parked window.
        if rows_ok and title then
            for _, list in ipairs({ plates(w), emblems_received(w) }) do
                for _, t in ipairs(list) do
                    if not was_recent(t) then return title, true, {} end
                end
            end
        end
        return nil
    end
    return table.concat(parts, ", "), title_fresh, parts
end

-- Mark a window's CURRENT node texts as recently spoken. Called once per choice
-- prompt: the choice branch speaks the window through the announcer (not through
-- fresh_notice), so without this its texts stay unmarked and resurface later under a
-- fresh title — the gift picker's stale "Selecciona un regalo." Txt_Help re-composed
-- into "Subida de rango de comunidad, Selecciona un regalo." (20:26:41).
local function mark_window_nodes(w)
    if not Core.valid(w) then return end
    local title, help
    pcall(function() title = node_rt(w.Txt_Title) end)
    if title and title ~= "" then mark_recent(title) end
    pcall(function() help = node_rt(w.Txt_Help) end)
    if help and help ~= "" then mark_recent(help) end
    for _, m in ipairs(UNTITLED_MEMBERS) do
        local t
        pcall(function() t = node_speech(w[m]) end)
        if t and t ~= "" then mark_recent(t) end
    end
end

local function window_gone()
    state = { msg = nil, labels = {}, sel = nil }
end

function Dialog.is_active()
    tick = tick + 1
    if ENUM_WINDOWS and tick % 5 == 0 then pcall(enum_windows) end
    if not Core.on_screen(win) then acquire() end   -- acquire is now cheap (cached refs)
    if not Core.on_screen(win) then
        window_gone()
        was_on = false
        return false
    end
    if not was_on then
        was_on = true
        appear_t = os.clock()
        -- A fresh appearance may be a NEW notice whose composed text equals the
        -- previous one (rows-only rewards) — drop the fold latch so update()
        -- rebuilds. Safe on blinks: update() only runs when something is fresh.
        notice_msg, notice_full = nil, nil
    end
    local msg = message(win)
    local labels, sel, items = choices()
    -- TEMP dialogue trace (2026-07-16: reward popup not read + tutorial dialogues
    -- "mixing"/skipped). One line per state change: which window, its message, choice
    -- count, whether the guide-yield fires, and the current latch — names exactly why
    -- a window is skipped. Turn OFF (DLG_TRACE=false) once the flow reads right.
    local fresh
    local rows_ok = (os.clock() - appear_t) < ROWS_FRESH_S
    if #labels == 0 then fresh = fresh_notice(win, rows_ok) end
    if DLG_TRACE then
        local wn = "?"
        pcall(function() wn = win:GetFullName():match("([%w_]+_C)_%d+") or "?" end)
        local sig = string.format("%s msg=%s lbl=%d guide=%s new=%s spoken=%s",
            wn, tostring(msg and msg:sub(1, 40)), #labels,
            tostring(Guide.showing(win)), tostring(fresh and fresh:sub(1, 24)),
            tostring(spoken and spoken:sub(1, 24)))
        if sig ~= dlg_last then
            dlg_last = sig
            print("[KakarotAccess] dlg " .. sig .. "\n")
        end
    end
    -- GUIDE layer: a rich-side instruction with no choices means the window is the
    -- game's pinned tutorial-guidance box, not a notice — and its PLAIN text nodes
    -- hold STALE content from the window's previous use (community-tutorial dumps
    -- 2026-07-16: Txt_Detail.mainTxt still said "Received the community leader Goku"
    -- while Txt_Detail.ExMainTxt showed "With an empty panel selected, press A…").
    -- Yield: guide_watch reads the instruction, the screen underneath keeps reading,
    -- and the stale plain junk never speaks. Choice windows never yield (their
    -- selection semantics live here).
    if #labels == 0 and Guide.showing(win) then
        state = { msg = nil, labels = {}, sel = nil }
        return false
    end
    -- Nothing to say (no message AND no options): release. The latch keeps its text —
    -- a mid-animation blank read must not turn the next flicker into a re-announce.
    if not msg and #labels == 0 then
        state = { msg = nil, labels = {}, sel = nil }
        return false
    end
    if DEBUG then
        -- Signature covers choice-only windows (msg is nil for a field decision box).
        local sig = (msg or "nomsg") .. "|" .. #labels .. "|" .. (labels[1] or "")
        if sig ~= last_dumped then last_dumped = sig; dump_window(msg) end
    end
    -- A NOTICE (no choices) owns the screen only until it has been announced; then it
    -- RELEASES the tick so an interactive screen underneath keeps reading — the
    -- community board's tutorial popups PERSIST until the task is done and were muting
    -- the whole board (the cursor moves under them). Nothing FRESH per fresh_notice
    -- (every node's text recently spoken / stale) ⇒ nothing new to say ⇒ release.
    if #labels == 0 and not fresh then return false end
    -- CHOICE PROMPT (round 17): message()'s title-presence composite lies on this
    -- pooled window for prompts too — the movie-skip confirm read the difficulty
    -- box's leftover "Ajustes de dificultad, Nivel de dificultad" while the live
    -- question sat in a fresher node (20:44:47). Compose the prompt with the same
    -- per-node novelty as notices, ONCE per window appearance (latched: the tick
    -- after mark_window_nodes marks the prompt, fresh_notice would return nil and
    -- re-computing would flip the announcer back to the stale composite). message()
    -- remains the fallback for a verbatim REPEAT of a confirm whose node texts are
    -- all still recently marked.
    local prompt = msg
    if #labels > 0 then
        local key = #labels .. "|" .. tostring(labels[1]) .. "|" .. tostring(msg)
        if key ~= choice_key then
            choice_key = key
            choice_prompt = fresh_notice(win) or msg
        end
        prompt = choice_prompt
    end
    state = { msg = prompt, labels = labels, sel = sel, items = items }
    return true
end

function Dialog.reset() ann:reset() end

function Dialog.update()
    local s = state or {}
    if s.labels and #s.labels > 0 then
        -- Confirmation / selectable list: prompt spoken once (as the screen), the
        -- highlighted option as the focused item (re-announced as you move). Item
        -- lists (gift picker) add the live per-item description as the tooltip.
        ann:focus(s.msg, nil, s.sel or s.labels[1], nil,
            s.items and item_detail or nil)
        if s.msg ~= choice_marked then
            choice_marked = s.msg
            mark_window_nodes(win)
        end
    else
        -- Plain message / notice. Speak it DIRECTLY, not through the shared menu
        -- announcer: the announcer's stateful name-dedup was SWALLOWING reward notices
        -- — "Emblemas de alma recibidos" was detected every time (dlg trace) but never
        -- reached PRISM (speech log), user 2026-07-16. is_active gates update() to the
        -- single tick where the text changes (msg==spoken releases it afterwards), so
        -- this is exactly one utterance per notice. Content rows (reward emblems:
        -- Goku/Gohan + their levels) are FOLDED into the title so the deferred tooltip
        -- can't lose them when the next notice replaces the window.
        -- Re-read FRESH here, NOT the state captured in is_active: the pooled Win01
        -- flickers between a stale prior notice and the new one, and the registry's
        -- confirm debounce can land this update() a few ticks after is_active, on a
        -- STALE frame (round 10). fresh_notice picks the live layout by per-node
        -- novelty (see its note) — nothing fresh means the flicker landed on stale
        -- content this tick, so say nothing and wait.
        local composed, titled, node_texts =
            fresh_notice(win, (os.clock() - appear_t) < ROWS_FRESH_S)
        if not composed then return end
        if composed ~= notice_msg then
            -- Build the folded text FIRST and latch LAST: the WL_LvTextList fixed-array
            -- throw (2026-07-16) aborted this block after notice_msg was set but before
            -- notice_full was rebuilt, so the next tick skipped the rebuild and spoke
            -- the PREVIOUS notice's stale notice_full (or nil → bare title). With the
            -- latch assigned only after a complete build, an abort just retries the
            -- rebuild next tick instead of latching half-updated state.
            -- Content rows (item plates, link-bonus rows, the Goku/Gohan emblem rows)
            -- belong to the TITLED layouts — folding them into an untitled notice
            -- would append the still-visible stale rows to e.g. "¡Recibiste al
            -- líder…". Rows already spoken recently are dropped (pooled rows keep
            -- stale content across notices), so "Bonificaciones" folds its fresh
            -- link-bonus name but not the reward's Goku/Gohan rows.
            notice_extra = {}
            if titled then
                for _, list in ipairs({ plates(win), emblems_received(win) }) do
                    for _, t in ipairs(list) do
                        if not was_recent(t) then notice_extra[#notice_extra + 1] = t end
                    end
                end
            end
            notice_full = #notice_extra > 0
                and (composed .. ", " .. table.concat(notice_extra, ", "))
                or composed
            notice_msg = composed
        end
        if DLG_TRACE then
            print(string.format("[KakarotAccess] dlg-notice speak=%s\n",
                tostring((notice_full or composed):sub(1, 60))))
        end
        Speech.say_protected(notice_full or composed)
        -- Mark the composed utterance, each node text AND each folded row: per-part
        -- marks are what keeps a stale slot or a stale pooled row from re-composing
        -- as "new" later.
        mark_recent(composed)
        for _, t in ipairs(node_texts or {}) do mark_recent(t) end
        for _, t in ipairs(notice_extra or {}) do mark_recent(t) end
        ann:reset()          -- keep the shared announcer clean for a following choice
        spoken = composed    -- for the DLG_TRACE line only
    end
end

-- A map TRANSITION (return to title, area travel) starts a new notice EPOCH: the
-- startup notices ("Revisando contenido descargable…", "Guardando partida…") repeat
-- VERBATIM at the title screen, and after a mid-game return-to-title they were still
-- inside the recent-set FIFO from the session boot — silently suppressed (round 21;
-- the 20:42:57 log line: msg=Revisando contenido… new=nil). The blink/flicker
-- suppression the recent-set exists for never spans a map load, so clearing here
-- costs nothing. Pure Lua only (runs mid-map-switch).
Transition.on_begin("screen_dialog", function()
    recent, recent_set = {}, {}
    spoken, notice_msg, notice_full, notice_extra = nil, nil, nil, nil
    choice_prompt, choice_key, choice_marked = nil, nil, nil
    dlg_last, enum_last, state, win = nil, nil, nil, nil
    was_on, appear_t = false, -1e9
end)

return Dialog
