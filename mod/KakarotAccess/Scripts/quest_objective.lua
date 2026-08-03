-- Quest objective HUD reader (Quest_Navi_C -> UAT_UIQuestNavigation).
--
-- The on-screen quest tracker shows the current objective(s) ("investigate the
-- house", "cook a meal", …). It is a HUD element, NOT a menu the player navigates,
-- so it is NOT in the menu Registry (which picks a single active adapter and would
-- let this shadow real menus). Instead it runs its own light poll loop, like
-- nav_tracker: it announces the objective whenever it CHANGES (diff-gated), so the
-- player hears each update, and F10 (keyboard) or L3 + Y (pad) repeats the current
-- objective on demand — including while the HUD is hidden, from the last known text.
--
-- Structure (CXX dump): the host holds up to three main rows (Quest_Navi_M00..M02)
-- and three sub rows (Quest_Navi_S00..S02), each a Quest_Navi_M/S_C whose
-- Txt_List_00 is the objective line and Txt_List_01 an optional counter ("0/3").
-- Quest titles are in Txt_Title / Txt_Main00 / Txt_Sub00 (blueprint) with the
-- native WL_MainQuestListTitle / WL_SubQuestListTitle as fallbacks. Every read is
-- validity/visibility guarded; unreadable nodes are simply skipped.

local Core = require("ui_core")
local Mem = require("mem")            -- crash black box only (Mem.mark)
local Speech = require("speech")
local I18n = require("i18n")
local Registry = require("ui_registry")
local Transition = require("transition")
local A = require("ui_archetypes")
local Input = require("input")        -- the on-demand read's pad chord (see pad_step)
local PadPoll = require("pad_poll")   -- shared 20 ms pad dispatch (dispatch only, no logic)

local Quest = {}

-- Objective text carries CFramework markup (<span color=…>fogata</>, <inputicon…>) —
-- read it THROUGH markup_to_speech so the tags/glyphs resolve to speech instead of
-- being spoken literally ("Cocina un pez en una <span color=...>fogata</>", user
-- 2026-07-16). Same sink the dialogue/menu readers use.
local function read_clean(node)
    if not Core.valid(node) then return nil end
    return A.markup_to_speech(Core.read_text(node))
end

local TICK_MS = 300          -- objectives change rarely; a slow loop keeps HUD reads cheap
local HOST_CLASS = "Quest_Navi_C"

-- Objective rows, in the on-screen order (main quest first, then sub quest).
local MAIN_ROWS = { "Quest_Navi_M00", "Quest_Navi_M01", "Quest_Navi_M02" }
local SUB_ROWS  = { "Quest_Navi_S00", "Quest_Navi_S01", "Quest_Navi_S02" }
-- Title candidates for each group, most specific first.
local MAIN_TITLE = { "Txt_Main00", "Txt_Title", "WL_MainQuestListTitle" }
local SUB_TITLE  = { "Txt_Sub00", "WL_SubQuestListTitle" }

local running = false
-- WHAT WAS ANNOUNCED, in two parts (2026-07-31). `last_key` is the SIGNATURE the diff gate
-- compares (see sig_key / the note in step); `last_text` is the composed line that was actually
-- spoken, kept for the on-demand re-reads, which want words rather than a comparison key. They
-- used to be one variable, which forced the gate to diff the spoken text — the bug that
-- re-announced a stale objective whenever the HUD repainted.
local last_key = nil
local last_text = nil
local cand = nil             -- candidate text while it SETTLES (see step)
local cand_since = nil       -- os.clock() of the poll that FIRST observed `cand` (see STABLE_S)
local STABLE_TICKS = 2       -- observations the objective text must hold before it may speak:
                             -- the HUD repopulates progressively (title first, then rows)
                             -- and pools several Quest_Navi_C instances, so a single read
                             -- flickers between partial strings — announcing each one gave
                             -- the "objective spoken several times" + "re-narrated on map
                             -- close" reports (user 2026-07-24). Settling first collapses
                             -- those to one announce of the final text.
-- The settle window in WALL TIME, DERIVED from the two constants above rather than written as
-- its own number: holding for STABLE_TICKS consecutive polls of TICK_MS is (STABLE_TICKS - 1)
-- poll periods of ELAPSED time, so this is the same delay the poll counter gave in the steady
-- state — the change is structural, not a re-tune.
--
-- Why the counter had to go (user report 2026-07-31: "during and after the cutscene where
-- Goku's brother arrives, the mod would still occasionally announce that I needed to speak to
-- Krillin"). `cand_hold` counted POLLS, and the three gates at the top of step() return WITHOUT
-- touching it — so a partial reading taken in one gap between subtitle lines was "confirmed" by
-- the next gap, potentially seconds later with a whole cutscene in between (the dialogue adapter
-- commits IN on every line and OUT in every gap — ui_registry.lua:311-317). Two "consecutive"
-- polls that are seconds apart confirm nothing about stability, which is the playbook's own rule
-- (a debounce measured in CALLS is not a debounce) reached from the other side: not a poll called
-- from too many loops, but a poll STARVED by its own gates. Wall time, plus dropping the
-- candidate on every poll that did not observe the HUD, makes the window mean what it says.
local STABLE_S = (STABLE_TICKS - 1) * TICK_MS / 1000
local tick = 0              -- monotonic tick for Core's cache/back-off bookkeeping

-- "Objective advanced" signal for the radar (wired in app.lua -> Nav). Each quest
-- group keeps a SIGNATURE — title + objective lines WITHOUT the counters — and the
-- callback fires when a non-nil signature differs from the last non-nil one:
--  * counters ("2/5") tick without moving the marker, so they are excluded;
--  * the HUD hiding in battle/menus reads nil and must NOT look like a change
--    (stored signatures survive nil reads), so the same objective re-appearing
--    after a fight stays silent here;
--  * the very first observation of a session only seeds (nothing "changed" yet).
-- The kind ("main"/"sub") tells the radar which quest class advanced, so it can
-- prefer that marker when both a main and a sub arrow are active.
local last_sig = { main = nil, sub = nil }
local sig_seeded = false
local on_change = nil        -- fn(kind) set via Quest.set_on_change
-- fn(kind) set via Quest.set_on_gone. Declared HERE, above its only user (presence_check): a
-- local referenced above its own declaration compiles to a global read, is nil at runtime and
-- takes the mod down from boot — the 2026-07-25 lesson the globals lint exists to catch.
local on_gone = nil

-- First readable text among a host's candidate member names.
--
-- The comment that used to sit here said absent blueprint members "read as nil — safe". That is
-- the opposite of true on this engine, and it is the single most expensive wrong belief in this
-- codebase's history: fetching a member a class does NOT declare is an UNCATCHABLE abort that
-- unwinds below the Lua boundary, so the `pcall` on the next line cannot contain it. It killed the
-- process on 2026-07-17 (`bar.Txt00`). This function is built ENTIRELY around trying names that
-- are expected to be absent, at 300 ms, on a host the game hides and rebuilds when a battle
-- starts — i.e. the most likely mid-combat crash site in the mod (2026-07-26). `Core.member` asks
-- the class first and returns nil for a name it does not declare, which is what this loop wanted
-- the raw fetch to do all along.
local function first_text(host, names)
    for _, nm in ipairs(names) do
        local node
        -- STRICT (2026-07-31 audit): `names` is a candidate list, so absence is the EXPECTED
        -- answer for most of it — the one contract where falling open on "no property set yet"
        -- is a licence for the uncatchable abort rather than a harmless guess.
        if pcall(function() node = Core.member(host, nm, true) end) and Core.valid(node) then
            local t = read_clean(node)
            -- The WINNING name comes back as a second return (2026-07-31). This is a FALLBACK
            -- CHAIN, so which entry answers is itself a signal: a title that silently moves from
            -- Txt_Main00 to WL_MainQuestListTitle while the quest stands still changes the
            -- composed reading, and the diff gate below cannot tell that apart from a new
            -- objective. `shape` reports it with every announcement so the next report names the
            -- part that flipped instead of leaving it to be reasoned about from the source.
            if t then return t, nm end
        end
    end
    return nil
end

-- One objective row's spoken line: the objective text plus its counter, or nil if the
-- row is absent/collapsed/blank. Second return: the bare objective text (no counter),
-- the row's contribution to the change signature.
local function row_line(host, member)
    local row
    -- Gated fetch, same reason as first_text above: the M/S row members are not all declared on
    -- every host variant, and an undeclared name here is an abort, not a nil.
    -- STRICT for the reason the comment above states: the M/S row members are NOT all declared on
    -- every host variant, so this is an absence probe and must refuse rather than fall open when
    -- the property set is unavailable (2026-07-31 audit).
    if not pcall(function() row = Core.member(host, member, true) end) or not Core.valid(row) then return nil end
    if not Core.on_screen(row) then return nil end
    local obj = read_clean(Core.member(row, "Txt_List_00"))
    if not obj then return nil end
    return Core.phrase(obj, read_clean(Core.member(row, "Txt_List_01"))), obj
end

-- The whole current objective as one string (title + each visible objective line for
-- both quest groups), or nil when the tracker shows nothing.
--
-- IMPORTANT (F10 diagnostic, 2026-07-15): for a single-objective quest the game shows
-- the objective IN THE TITLE NODE (Txt_Main00 / WL_MainQuestListTitle) and keeps every
-- M/S row hidden (valid, on_screen=false, empty) — the rows are the sub-task checklist
-- ("0/3" lines) and only populate for multi-step objectives. So a group must speak
-- when EITHER the title or the rows carry text; requiring rows muted the whole reader.
-- Returns the spoken text, plus the main/sub change SIGNATURES (see last_sig above).
--
-- WHICH SOURCES produced the last reading, per quest group: "Txt_Main00:1" means that title
-- candidate answered and one row was on screen, "-:0" means neither. Written here, printed with
-- each announcement (see step). It exists because the reading is a COMPOSITION — one of three
-- title candidates, plus however many rows pass Core.on_screen, plus whether the sub group
-- contributed at all — while the diff gate downstream compares the composed STRING. A repaint
-- that changes only the composition therefore reads as a brand-new objective and re-announces an
-- objective the player already completed, which is the 2026-07-31 report. One line per
-- announcement names the flipping part; announcements are rare by design, and the whole
-- complaint is that they are not, so this log is bounded by the very thing it measures.
local shape = { main = "-", sub = "-" }

local function objective_text()
    shape.main, shape.sub = "-", "-"
    local host = Core.first_on_screen(HOST_CLASS, tick)
    if not host then return nil end
    local parts, sigs = {}, {}
    local function group(gkey, title_names, rows)
        local lines, sig = {}, {}
        for _, m in ipairs(rows) do
            local l, raw = row_line(host, m)
            if l then lines[#lines + 1] = l end
            if raw then sig[#sig + 1] = raw end
        end
        local title, tname = first_text(host, title_names)
        if not title and #lines == 0 then return end
        shape[gkey] = (tname or "-") .. ":" .. #lines
        parts[#parts + 1] = Core.phrase(title,
            #lines > 0 and table.concat(lines, ", ") or nil)
        sigs[gkey] = (title or "") .. "|" .. table.concat(sig, "|")
    end
    group("main", MAIN_TITLE, MAIN_ROWS)
    group("sub", SUB_TITLE, SUB_ROWS)
    -- The 4th return is "the quest HUD was READABLE this poll", which is a different fact from
    -- "there was an objective on it" and the only safe basis for concluding that a quest class
    -- ENDED (see presence_check). A host with nothing on it is still a readable host.
    if #parts == 0 then return nil, nil, nil, true end
    return table.concat(parts, ". "), sigs.main, sigs.sub, true
end

-- The two group signatures as ONE comparable key — what the speech diff gate stores and
-- compares (see step). Declared here, above every user, and shared by the three of them so the
-- announce path and the two on-demand re-reads can never disagree about what "the same
-- objective" means.
local function sig_key(sig_main, sig_sub)
    return (sig_main or "") .. "|" .. (sig_sub or "")
end

-- Diff one group's signature against its last non-nil value; returns the group key
-- when it genuinely changed (see the last_sig comment for the nil/seed rules).
local function sig_changed(gkey, sig, was_seeded)
    if not sig then return nil end
    local prev = last_sig[gkey]
    last_sig[gkey] = sig
    if sig ~= prev and (prev ~= nil or was_seeded) then return gkey end
    return nil
end

-- Polls a class must be absent FROM A READABLE HUD before we call it finished. The HUD repaints
-- between phases, so one empty frame means nothing.
local GONE_POLLS = 3
local gone_hold = { main = 0, sub = 0 }

-- Did this quest class DISAPPEAR? Only ever answered when the HUD host itself was readable this
-- poll: the HUD is also hidden by a level load, a fight and any open menu, and treating those as
-- "the side story ended" would hand the radar back to the main quest exactly when the player is
-- mid-quest — including the case the user asked about, a side story that sends you to another
-- map. No host, no verdict.
local function presence_check(gkey, sig, host_ok)
    if not host_ok or sig then gone_hold[gkey] = 0 return end
    gone_hold[gkey] = gone_hold[gkey] + 1
    if gone_hold[gkey] == GONE_POLLS and last_sig[gkey] ~= nil then
        -- Forget the stored signature too, so a NEW quest of this class reads as a change and
        -- re-arms the focus instead of being swallowed as "same as last time".
        last_sig[gkey] = nil
        if on_gone then pcall(on_gone, gkey) end
    end
end

-- `settled` = the reading this signature came from has held for the settle window (see step).
--
-- THE CHANGE SIGNAL IS GATED ON IT, AND THIS IS THE HALF THAT CRASHES (user report 2026-07-31:
-- the objective re-announced through a cutscene, the game went progressively slower, then "Fatal
-- error"). Until today `signal_check` ran on the RAW reading, above the settle, so it had no
-- debounce of any kind: one row flickering out of Core.on_screen while the HUD repaints, or a
-- title falling through to the next candidate, rewrote the signature and fired on_change. That is
-- not a cosmetic extra event. on_change is Nav.notify_objective_change, which resets
-- `preempt.scans = preempt.TRIES` unconditionally (nav_tracker.lua:3224) — and nav drains that
-- counter at most once per SCAN_EVERY, ~1.5 s (nav_tracker.lua:1669), while this loop can re-arm
-- it every 300 ms. Flicker therefore holds the radar preempt PERMANENTLY armed, which keeps
-- nav's marker block running past the suppressors that would normally skip it
-- (nav_tracker.lua:1636-1637) and walks MapIconList icons and their TargetActors every 1.5 s
-- (nav_tracker.lua:605-628) — dereferencing world actors, through a cutscene and its level
-- streaming, which is NOT a Transition (nav_tracker.lua:1540) and so releases nothing. Repeated
-- work on the game thread over handles the engine is freeing is exactly "a little slowly, then
-- Fatal error". A settled reading fires once per genuine advance, which is the contract nav was
-- written against.
--
-- presence_check is deliberately left on the RAW reading: it is the "this quest class left a
-- READABLE HUD" detector, its evidence is the empty-but-readable poll that never produces a
-- settled text at all, and it already has its own GONE_POLLS debounce. It only ever RELEASES a
-- focus, so it cannot drive the re-arm loop above.
local function signal_check(sig_main, sig_sub, host_ok, settled)
    if settled then
        local was = sig_seeded
        if sig_main or sig_sub then sig_seeded = true end
        -- Evaluate BOTH groups (no short-circuit): each must update its stored signature
        -- every pass.
        local cm = sig_changed("main", sig_main, was)
        local cs = sig_changed("sub", sig_sub, was)
        -- Report each changed class SEPARATELY rather than picking a winner here. The old
        -- `kind = cm or cs` silently dropped a sub advance whenever a main line moved in the same
        -- pass, which is one of the ways a multi-phase side story lost the radar. Which class
        -- should win is a radar decision — nav_tracker holds the quest focus and arbitrates.
        if cm and on_change then
            print("[KakarotAccess] objective change -> main\n")
            pcall(on_change, "main")
        end
        if cs and on_change then
            print("[KakarotAccess] objective change -> sub\n")
            pcall(on_change, "sub")
        end
    end
    presence_check("main", sig_main, host_ok)
    presence_check("sub", sig_sub, host_ok)
end

-- Drop the in-flight settle candidate. Called from every poll that did NOT get to look at the
-- HUD, because an unobserved poll is not evidence that anything held steady — see STABLE_S for
-- the report this comes from. It deliberately does NOT touch `last_key`: the diff gate staying
-- stale across a gate is the documented behaviour that makes a deferred announcement fire once
-- the gate lifts, and nothing here changes that.
local function forget_candidate()
    cand, cand_since = nil, nil
end

-- Poll step: announce the objective only when it changes, and only when no menu is
-- open and no level transition is in flight (never talk over a menu or a cutscene load).
local function step()
    Mem.mark("quest.step")
    if Transition.active() then forget_candidate() return end
    if Registry.active_adapter() then forget_candidate() return end
    -- CUTSCENES AND CONVERSATIONS (user report 2026-07-31: a stale "go back and talk to Krillin"
    -- spoken once at the start of the Raditz cutscene). The adapter gate above does NOT cover
    -- them: the dialogue adapter commits IN on every subtitle line and OUT in every gap between
    -- them (ui_registry.lua:311-317), so `active_adapter()` is nil for a good part of any
    -- cinematic and this reader runs right inside it — on a HUD the game is tearing down.
    -- `Core.scan_quiet()` is the mod's only signal that stays true ACROSS those gaps (ui_registry
    -- sets it from a scan_quiet adapter, and with no adapter from "no minimap, no battle, not
    -- free roam" — i.e. the whole cinematic state), so it is the gate that matches the report.
    -- Drops the settle candidate like its siblings: a reading taken before a cutscene is not
    -- evidence about anything observed after it.
    if Core.scan_quiet() then forget_candidate() return end
    -- Don't cut a PROTECTED line (a reward notice / tutorial instruction still
    -- playing): the objective would interrupt "Emblemas de alma recibidos…" a few
    -- seconds in (user 2026-07-16). Deferring keeps `last_key` stale, so it re-announces
    -- once the protected line finishes (diff gate still fires).
    if Speech.protected() then forget_candidate() return end
    -- This loop is independent of the menu Registry loop, so it must seed its own
    -- per-tick FindAllOf budget (Core.begin_scan_tick), or first_on_screen could find
    -- no budget left and never locate the HUD host.
    tick = tick + 1
    Core.begin_scan_tick()
    -- Second black-box mark, four lines below the first, and it earns its ring slot: everything
    -- above this point is pure Lua that returns on three gates, so a trail ending at "quest.step"
    -- cannot say whether this loop was merely ticking or was inside the HUD when the process
    -- died. From here down it dereferences the quest widget. One memcpy per 300 ms against the
    -- ~360/s the registry sweep already writes.
    Mem.mark("quest.read")
    local text, sig_main, sig_sub, host_ok = objective_text()
    -- SETTLE FIRST, then let anything act on the reading. Both consumers below — the radar
    -- change signal and the speech diff gate — compare a value composed from several
    -- independently flickering sources (which title candidate answered, which rows passed
    -- on_screen, whether the sub group contributed), so a bare repaint reads as a new objective
    -- to both of them. The candidate is dropped on a nil read for the reason the old comment
    -- gave: the HUD hides in combat/menus/transitions, and letting that overwrite the announced
    -- state made the SAME objective re-announce every time the player left a battle (user
    -- 2026-07-24). A completed quest simply leaves `last_key` at the old goal until a new one
    -- shows.
    local settled = false
    if not text then
        forget_candidate()
    elseif text == cand then
        settled = cand_since ~= nil and (os.clock() - cand_since) >= STABLE_S
    else
        cand, cand_since = text, os.clock()
    end
    signal_check(sig_main, sig_sub, host_ok, settled)
    if not text or not settled then return end
    -- DIFF THE SIGNATURE, SPEAK THE TEXT (2026-07-31). The composed reading is exactly that — a
    -- COMPOSITION: whichever of three title candidates answered, plus however many rows happened
    -- to pass Core.on_screen, plus whether the sub group contributed at all. So a repaint that
    -- changes only the composition (the HUD collapsing for a cutscene, a title falling back from
    -- Txt_Main00 to WL_MainQuestListTitle) rewrites the string while the objective itself has NOT
    -- moved, and a gate that diffs the string cannot tell that apart from a new goal: it
    -- re-announces the old one. The signatures are title + the bare objective lines, without the
    -- volatile counters — the same value the radar's change signal has always been gated on.
    local key = sig_key(sig_main, sig_sub)
    if key == last_key then return end
    -- One line per announcement, naming the COMPOSITION that produced it (see `shape`). The
    -- 2026-07-31 report arrived with no log at all, and the open question it left — which part of
    -- the reading flipped while the objective itself stood still — is answerable from two of
    -- these lines and nothing else.
    print(string.format("[KakarotAccess] objective -> [%s|%s] %s\n", shape.main, shape.sub, text))
    -- SPEAK PROTECTED, COMMIT AFTERWARDS. An objective is an actionable instruction — the same
    -- class of line as a tutorial or a reward notice — so it gets the protection every reader in
    -- the mod already defers to (Speech.protected), instead of being shredded mid-sentence by the
    -- next interrupt=true from a lower-priority readout. And the commit moves BELOW the say: an
    -- objective marked as announced by a line that never finished is an objective the player
    -- never hears again, which is the half of the 2026-07-31 report that reads as "it was said
    -- once and never repeated".
    Speech.say_protected(text)
    last_key, last_text = key, text
end

-- Re-announce the current objective on demand — called when the map opens, so the
-- player can review the goal there instead of the HUD reader repeating it during
-- free-roam (user 2026-07-24: the objective must speak only on change / first sight,
-- and on the map when the player wants it again). Prefers the cached `last_text` because the
-- quest HUD (Quest_Navi_C) is usually hidden on the map screen even though the objective
-- is unchanged; falls back to a live read only when nothing has been announced yet.
-- Runs on the game thread (the caller already runs inside the registry step). `interrupt`
-- (default true) is passed to Speech: the map opener passes FALSE so the objective QUEUES
-- after the map's own area/help readout instead of talking over it (user 2026-07-24).
function Quest.reannounce(interrupt)
    if interrupt == nil then interrupt = true end
    if last_text then Speech.say(last_text, interrupt) return end
    tick = tick + 1
    Core.begin_scan_tick()
    local text, sig_main, sig_sub = objective_text()
    if text then
        -- Commit the signature too, not just the text: this reading has now been spoken, so the
        -- poll loop must not repeat it the moment the map closes.
        last_key, last_text = sig_key(sig_main, sig_sub), text
        Speech.say(text, interrupt)
    end
end

-- Radar wiring (app.lua): fn(kind) fires once per genuine objective change,
-- kind = "main" | "sub". Runs on the game thread, inside this loop's pcall.
function Quest.set_on_change(fn)
    on_change = fn
end

-- Radar wiring (app.lua): fn(kind) fires once when a quest class LEAVES a readable HUD, i.e. that
-- story finished or was abandoned. kind = "main" | "sub". Lets the radar release its quest focus
-- and fall back to the normal priority order.
function Quest.set_on_gone(fn)
    on_gone = fn
end

function Quest.start()
    if running then return end
    running = true
    -- The pad chord for the on-demand read (L3 + Y). Reached through the Quest table because its
    -- stepper is defined BELOW this function: a direct local reference here would compile to a
    -- global read and be nil at runtime, which is the mistake the globals lint exists to catch.
    Quest.start_pad()
    PadPoll.register_every("quest", TICK_MS, function()
        local t0 = os.clock()
        local ok, err = pcall(step)
        if not ok then print("[KakarotAccess] quest step error: " .. tostring(err) .. "\n") end
        -- Cost telemetry (outside the registry step — printed by the Ctrl+F5 dump).
        local dt = (os.clock() - t0) * 1000
        local st = _G.__KakarotQuestStats
        if not st then st = { n = 0, ms = 0, max = 0 } _G.__KakarotQuestStats = st end
        st.n = st.n + 1
        st.ms = st.ms + dt
        if dt > st.max then st.max = dt end
    end)
end

function Quest.stop()
    running = false
    Quest.stop_pad()
    PadPoll.unregister("quest")
end

-- Diagnostic dump (dumps/dump_quest.txt), appended on every F10 press while DUMP is
-- on. It solved the 2026-07-15 silence (single-objective quests put the text in the
-- TITLE node, rows hidden); OFF since the reader was verified in-game that night.
local DUMP = false
local function dump_path()
    local src = debug.getinfo(1, "S").source:sub(2)
    local dir = src:match("^(.*)[/\\]") or "."
    return dir .. "\\dumps\\dump_quest.txt"
end

local function dump_state(text)
    local f = io.open(dump_path(), "a")
    if not f then return end
    f:write(string.format("== F10 @ %s text=%s\n", os.date("%H:%M:%S"), tostring(text)))
    f:write(string.format("  active_adapter=%s transition=%s\n",
        tostring(Registry.active_adapter()), tostring(Transition.active())))
    local host = Core.first_on_screen(HOST_CLASS, tick)
    if not host then
        f:write("  host: first_on_screen = nil\n")
        f:close()
        return
    end
    local fn = "?"
    pcall(function() fn = host:GetFullName() end)
    f:write("  host = " .. fn .. "\n")
    for _, rows in ipairs({ MAIN_ROWS, SUB_ROWS }) do
        for _, m in ipairs(rows) do
            local row
            local okr = pcall(function() row = Core.member(host, m) end)
            local valid = okr and Core.valid(row) or false
            local on = valid and Core.on_screen(row) or false
            local t0, t1
            if valid then
                t0 = Core.read_text(Core.member(row, "Txt_List_00"))
                t1 = Core.read_text(Core.member(row, "Txt_List_01"))
            end
            f:write(string.format("  %s valid=%s on_screen=%s txt0=%s txt1=%s\n",
                m, tostring(valid), tostring(on), tostring(t0), tostring(t1)))
        end
    end
    for _, nm in ipairs({ "Txt_Main00", "Txt_Sub00", "Txt_Title", "Txt_Navi_Detail",
                          "WL_MainQuestListTitle", "WL_SubQuestListTitle" }) do
        local node, t
        pcall(function() node = Core.member(host, nm) end)
        if Core.valid(node) then pcall(function() t = Core.read_text(node) end) end
        f:write(string.format("  title %s = %s\n", nm, tostring(t)))
    end
    f:close()
end

-- Speak the current objective on demand (interrupts). THE GAME-THREAD BODY: it touches live
-- UObjects, so only callers that are already on the game thread may call it directly (the pad
-- stepper below); everything else goes through Quest.read().
--
-- FALLS BACK TO THE LAST KNOWN TEXT (2026-07-31). It used to be a live read or nothing, so it
-- answered "no active objective" whenever the quest HUD was not up — during a cutscene, inside
-- any menu, in battle — which is precisely when a player reaches for it, and the answer was not
-- merely unhelpful but WRONG: the mod knows the objective, it announced it. Live read first (it
-- is the freshest), the last announced line second, the "nothing" string only when we genuinely
-- have never seen one.
local function read_now()
    if Transition.active() then return end
    tick = tick + 1
    Core.begin_scan_tick()
    local text, sig_main, sig_sub = objective_text()
    if DUMP then pcall(dump_state, text) end
    if text then last_key, last_text = sig_key(sig_main, sig_sub), text end
    Speech.say(text or last_text or I18n.t("objective_none"), true)
end

-- F10 / the pad chord, from OFF the game thread (a UE4SS keybind callback runs on its own
-- thread — see main.lua).
function Quest.read()
    ExecuteInGameThread(read_now)
end

-- ---- gamepad: read the objective on demand -----------------------------------------
-- L3 + Y (hold the left stick click, tap Y / Triangle) is the controller twin of F10 — the
-- player asked for a way to re-hear the current objective, and the mod is designed to be played
-- with a pad, so the keyboard key alone does not ship this feature.
--
-- WHY THAT CHORD. It must not collide with anything the mod already reads, and it must be safe
-- to leak to the game (nothing is blocked here — the mod only READS the pad):
--   * R3 alone is the radar picker and L3 + R3 is the config menu, so no R3 chord is available;
--     radar_menu only checks that L3 is NOT held, so any other modifier + R3 would open it.
--   * L3 alone is out for the same reason: its rising edge is the first half of the config chord.
--   * The d-pad, A, B and X are all consumed by mod overlays and by the map/status readers.
-- L3 + Y leaves one modifier gesture the mod already asks players for (hold L3, tap something)
-- and a face button no mod screen binds. It stays available everywhere — free roam, menus,
-- cutscenes — because the fallback above is exactly what makes it worth pressing there.
local pad_running = false
local pad_prev = 0           -- button mask last seen (level-compare edge)

local function pad_step()
    -- Read the pad ONCE, decide the edge, and COMMIT `pad_prev` before any early return
    -- (screen_map's 2026-07-28 lesson: bookkeeping at the end swallows presses made while a gate
    -- is closed, and that reads to the player as "sometimes it works").
    local snap = Input.read()
    local buttons = snap and snap.buttons or 0
    local B = Input.BTN
    -- Rising edge for Y from the NATIVE LATCH or the level compare. The latch matters most
    -- exactly where this bind does: during cutscenes and loads the dispatch relaxes to 100 ms,
    -- and a tap that begins and ends inside one of those windows is invisible to a level compare.
    -- The L3 half is deliberately a plain level read of the SAME snapshot — a chord's modifier is
    -- a held state, not an edge.
    local hit = (Input.pressed(B.Y) or ((buttons & B.Y) ~= 0 and (pad_prev & B.Y) == 0))
        and (buttons & B.LEFT_THUMB) ~= 0
    pad_prev = buttons
    if not hit then return end
    -- A mod overlay owning the pad (radar picker / config menu) gets it to itself, as everywhere
    -- else in this codebase.
    if _G.__KakarotPadModal then return end
    -- Called DIRECTLY, not through Quest.read: pad_poll already dispatches inside
    -- ExecuteInGameThread, so this is the game thread. read_now calls Core.begin_scan_tick, which
    -- the "a 20 ms loop must not seed the scan budget" rule forbids PER TICK — this is one seed
    -- per button press, the same one-shot the F10 key has always done, not a 50 Hz refill.
    read_now()
end

function Quest.start_pad()
    if pad_running then return end
    if not Input.is_loaded() then
        print("[KakarotAccess] quest_objective: input bridge not loaded, L3+Y read disabled\n")
        return
    end
    pad_running = true
    -- Shared 20 ms scheduler (pad_poll.lua): pad_step early-outs on one bit test per tick.
    PadPoll.register("quest_read", pad_step)
end

function Quest.stop_pad()
    pad_running = false
    PadPoll.unregister("quest_read")
end

return Quest
