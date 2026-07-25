-- Screen adapter: game-over / defeat menu (Gameover_C -> UAT_UIGameover).
--
-- The "Fin de la partida" screen shown when the player is defeated: a short vertical list —
-- Reintentar / Cargar / Volver al título — held in the native SelectionWidgetArray of
-- UAT_UIGameoverBar rows (each row's label is Txt_List), also reachable positionally as
-- List_Bar00.. (F7 census 2026-07-24, the reader was silent because there was no adapter).
--
-- SELECTION: the BP class Gameover_C dumps EMPTY, so the selected row is the native
-- CurrentSelectIndex (AT.hpp UAT_UIGameover @0x3E0 — the §2 "native selection index" case).
-- Read reflected first (the property is named in the header); fall back to the mem_bridge
-- offset if reflection can't see it. Same pattern as the battle-pause menu (screen_pause).
--
-- The host is GameInstance-outer'd (a persistent pool), so it can stay parked on_screen
-- after closing — gate on pane_live so a stale defeat menu can't re-claim the tick.

local Core = require("ui_core")
local I18n = require("i18n")
local Mem = require("mem")
local OFF = require("native_offsets")

local GameOver = {}

-- DETECTION — settled 2026-07-25 by the F7 owner hunt, and it turned three rounds of
-- machinery into two lines. The probe printed:
--     -- HOST …BP_ATGameInstance_C_0.Gameover_C_0  vis=false enum=1:Collapsed op=1.0
--        .List_Bar00/01/02 = "Reintentar" / "Cargar" / "Volver al título"
--        SelectionWidgetArray = 3      CurrentSelectIndex reflected=0
--        OWNER FIELD: AT_UIFieldManager.Gameover -> this host
-- So: everything is reflected (the native mem_bridge offset is only a fallback now), and the
-- game keeps a POINTER to this widget on the field manager. `Gameover_C` is therefore mapped in
-- ui_directory as `{"fm","Gameover"}`, and detection costs two guarded pointer reads — no
-- FindAllOf, no backoff, no watch lane, and immune to cinematic quiet mode, which is what kept
-- this screen silent in the first place (a defeat happens with the gameplay world up, no battle
-- HUD, no minimap and no user press: exactly the state quiet mode defers scans in).
--
-- The dead ends this replaced, so nobody rebuilds them: an unconditional
-- `Core.first_on_screen` probe (a FindAllOf forever, and the 2026-07-17 "never probe a
-- per-level pool from an always-running adapter" rule); then a watch lane armed by the
-- battle-HUD falling edge, which the sticky registry could never deliver reliably and whose
-- flapping cost 124 scans in one session — audible as the audio cutting during cutscenes.
--
-- The LIVENESS test is `Core.pane_live`, not `Core.on_screen`. Two reasons, both measured:
-- the parked host reads `enum=1:Collapsed op=1.0`, so pane_live rejects it cleanly; and
-- `on_screen` rejected the host even while the menu WAS up (the 2026-07-25 log: the watch found
-- it for 20 s and the adapter never claimed), because that walk also demands the root
-- UserWidget's `IsInViewport()` and this widget is shown through the field manager.
local BAR_COUNT = 6   -- List_Bar00..05 probe cap (the menu shows 3; 03-05 are not even declared,
                      -- and the member gate now refuses them instead of fetching a null)

local ann = Core.make_announcer()
local host = nil
local tick = 0
local cached = nil   -- item string computed in is_active, reused by update this tick
local diagnosed = false -- one-shot log line when the host is live but unreadable

-- Ordered menu labels (collapsed / empty rows skipped) — entry / fallback list.
-- `is_visible` (the row's OWN slate flag), not `on_screen`: the ancestor walk is what rejected
-- this whole widget tree in the 2026-07-25 log, so applying it to the rows too would leave the
-- menu unreadable for the same reason. Liveness is established once, at the host, in is_active.
local function items()
    local list = {}
    for i = 0, BAR_COUNT - 1 do
        local bar = Core.member(host, "List_Bar0" .. i)
        if Core.is_visible(bar) then
            local label = Core.text_of(Core.member(bar, "Txt_List"))
            if label then list[#list + 1] = label end
        end
    end
    return list
end

-- The selected index: reflected CurrentSelectIndex if exposed, else the native member.
-- The reflected fetch goes through Core.member — a naked `host.X` is evaluated at the
-- call site, outside every pcall, and that is the uncatchable-AV class (CLAUDE.md §8).
local function select_index()
    local idx = Core.member(host, "CurrentSelectIndex")
    if type(idx) == "number" then return idx end
    return Mem.i32(host, OFF.gameover.selectedIndex)
end

-- The selected row's label via the native index; nil if unavailable (falls back to the
-- whole-menu read in is_active).
local function selected_label()
    local idx = select_index()
    if not idx or idx < 0 or idx >= BAR_COUNT then return nil end
    -- SelectionWidgetArray is the array the index refers to; guarded so an invalid TArray
    -- can't pierce (array_of validates before GetArrayNum). Fall back to the positional row.
    local row
    local arr, n = Core.array_of(host, "SelectionWidgetArray")
    if arr and idx < n then pcall(function() row = arr[idx + 1] end) end   -- 1-based Lua array
    if not Core.valid(row) then row = Core.member(host, "List_Bar0" .. idx) end
    if not Core.is_visible(row) then return nil end   -- own flag, not the walk (see items())
    return Core.text_of(Core.member(row, "Txt_List"))
end

-- ONE-SHOT signal probe. The 2026-07-25 log proved the watch lane FINDS the class
-- (`watch Gameover_C: 1 found`, repeating for the full ~20s window) while the adapter never
-- reached watch_clear — so the host existed and `Core.on_screen` said no. Which of the signals
-- inside that walk says no is the one thing the log could not say, and the F7 dump died before
-- printing it. One line per pooled instance, once per session, answers it on the next defeat.
-- Logs on CHANGE, not once (fixed 2026-07-25). The one-shot version fired on the first poll that
-- had a directory hit and then went quiet forever — in the user's log that was 12:28, minutes
-- before any defeat, so it reported the PARKED state (`enum=1` Collapsed) and told us nothing about
-- the state we actually need. Keying on the signal tuple means a line appears exactly when the
-- widget changes state, i.e. when the menu really opens, and stays bounded because a widget only
-- has a handful of distinct states.
local last_sig = nil
local function probe_signals(list)
    local function ask(f) local ok, v = pcall(f) return ok and tostring(v) or "-" end
    local parts = { tostring(#list) }
    for _, h in ipairs(list) do
        parts[#parts + 1] = table.concat({
            tostring(Core.valid(h)), tostring(Core.on_screen(h)),
            ask(function() return h:IsVisible() end),
            ask(function() return h:GetVisibility() end),
            ask(function() return h:GetRenderOpacity() end),
            ask(function() return h:IsInViewport() end),
            tostring(Core.pane_live(h)),
        }, " ")
    end
    local sig = table.concat(parts, " | ")
    if sig == last_sig then return end
    last_sig = sig
    print(string.format("[KakarotAccess] gameover probe: %d instance(s)\n", #list))
    for i = 2, #parts do
        print(string.format("[KakarotAccess]   [%d] valid/on_screen/IsVisible/enum/opacity/inVP/pane_live = %s\n",
            i - 1, parts[i]))
    end
end

function GameOver.is_active()
    tick = tick + 1
    -- Scan-free by construction: Gameover_C is directory-mapped, so peek_all is a pointer read
    -- and this costs the same whether the menu is up or not. That is why there is no window, no
    -- watch and no arming signal left to get wrong.
    local list = Core.peek_all("Gameover_C")
    probe_signals(list)
    host = nil
    for _, h in ipairs(list) do
        if Core.valid(h) and Core.pane_live(h) then
            host = h
            break
        end
    end
    if host == nil then cached = nil return false end

    local sel = selected_label()          -- native per-item selection
    if sel then cached = sel return true end

    -- Fallback: no native read -> announce the whole menu on entry.
    local list = items()
    if #list == 0 then
        -- Live host, nothing readable: say so ONCE. Distinguishes "never detected"
        -- (no line at all) from "detected but the BP exposes no reflected rows", which
        -- is the open question this screen's F7 probe answers.
        if not diagnosed then
            diagnosed = true
            print(string.format(
                "[KakarotAccess] gameover: host live but unreadable (idx=%s, rows=0)\n",
                tostring(select_index())))
        end
        cached = nil
        return false
    end
    cached = table.concat(list, ", ")
    return true
end

function GameOver.reset() ann:reset() end

function GameOver.update()
    -- screen = "Fin de la partida" (spoken on entry only); name = the selected row
    -- (re-spoken as the d-pad moves).
    ann:focus(I18n.t("gameover"), nil, cached, nil, nil)
end

return GameOver
