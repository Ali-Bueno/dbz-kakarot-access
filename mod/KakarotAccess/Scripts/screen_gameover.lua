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

-- DETECTION (2026-07-24, first in-game test: the screen stayed SILENT). Gameover_C is
-- unmapped (no known manager field points at it — the F7 "GAME OVER" probe hunts for one),
-- so it can only be found by a scan; and the defeat state is EXACTLY the state cinematic
-- quiet mode defers scans in: the gameplay world is up (`Dir.root_ok("mm")`), the battle
-- HUD is gone, there is no minimap and the player pressed nothing — the menu appears on
-- its own. So the steady-state scan never runs and the adapter never sees its host.
-- The fix is the watch lane (CLAUDE.md §8: arm only on a REAL entry signal): the battle
-- HUD LEAVING is the one event that always precedes a defeat menu, so its falling edge
-- arms a bounded watch. Cost is paid only after a battle ends, never in the steady state.
local BAR_COUNT = 6   -- List_Bar00..05 probe cap (the defeat menu shows 3, leave headroom)
local WATCH_TICKS = 80   -- ~8s of watch after a battle ends: the defeat sequence (K.O.
                         -- animation + fade) runs well past the HUD leaving.

local ann = Core.make_announcer()
local host = nil
local tick = 0
local cached = nil   -- item string computed in is_active, reused by update this tick
local had_hud = false   -- battle HUD seen on the previous poll (falling edge = battle end)
local diagnosed = false -- one-shot log line when the host is live but unreadable

-- Scan-free (Core.peek_all): Battle_Hud_P_Main_C is directory-mapped, so this is a
-- pointer read. Its falling edge arms the watch for our own class.
local function arm_on_battle_end()
    local hud = false
    for _, w in ipairs(Core.peek_all("Battle_Hud_P_Main_C")) do
        if Core.valid(w) and Core.on_screen(w) then hud = true break end
    end
    if had_hud and not hud then Core.watch_for("Gameover_C", WATCH_TICKS) end
    had_hud = hud
end

-- Ordered on-screen menu labels (collapsed / empty rows skipped) — entry / fallback list.
local function items()
    local list = {}
    for i = 0, BAR_COUNT - 1 do
        local bar = Core.member(host, "List_Bar0" .. i)
        if Core.on_screen(bar) then
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
    if not Core.on_screen(row) then return nil end
    return Core.text_of(Core.member(row, "Txt_List"))
end

function GameOver.is_active()
    tick = tick + 1
    arm_on_battle_end()
    -- cached_all-backed (Core.first_on_screen): the watch lane only drives THAT path.
    host = Core.first_on_screen("Gameover_C", tick)
    if not Core.on_screen(host) then cached = nil return false end
    if not Core.pane_live(host) then cached = nil return false end
    Core.watch_clear("Gameover_C")        -- found: stop the watch immediately

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
