-- Global level-transition gate — the ONE signal that a map change is in progress.
--
-- Why: "return to title" and some cutscenes switch UWorlds: the old level is torn
-- down and the GC frees its objects. Any cached reference into that level (the
-- minimap widget, navi icons, the tracked target actor, cached HUD widgets) becomes
-- a dangling pointer, and even probing it (IsValid) is an uncatchable C++ abort
-- that pcall cannot catch — the recurring crash on return-to-title and after the
-- Nimbus/Dragon Ball cutscene (2026-07-04). The old ui_muted/world-gate pair had
-- two holes: the world gate itself probes the cached minimap widget, and cutscenes
-- run with NO adapter active (ui_muted never fires).
--
-- Signal: NotifyOnNewObject("/Script/Engine.GameModeBase") — the engine spawns
-- exactly ONE GameMode per map load (this game: AATTitleGameMode on the title map,
-- BP_ATGameModeMain_C in the field — both AGameMode subclasses, CXX dump), and it
-- uses UE4SS's core object-construction notify (the same machinery the whole mod
-- already relies on). Two rejected alternatives, both verified live 2026-07-04:
--   * RegisterLoadMapPreHook/PostHook: on THIS game (UE 4.21, SteamStub-packed exe,
--     UE4SS 3.0.1) registration "succeeds" but the native trampoline is misplaced
--     and the game hard-crashes on the FIRST map load (log ends at "Event loop
--     start").
--   * NotifyOnNewObject("/Script/Engine.World"): every STREAMED sublevel package
--     constructs its own UWorld too — 127 fires in 3 minutes of play, bursts of ~25
--     during loads/cutscenes. The gate was on half the time (the R3 radar menu
--     wouldn't open) and each fire's cache flush forced FindAllOf re-scans
--     (cutscene lag). Streaming never spawns a GameMode, so this signal is quiet.
--
-- Timing: the new GameMode spawns inside the map switch, AFTER the old world's
-- objects were freed — but no Lua tick can have run in between (teardown + GC +
-- world init all happen in one game-thread block; UE4SS queued ticks only pump
-- before or after it). So flushing here guarantees the FIRST tick that could touch
-- a freed pointer instead sees empty caches, which is all the safety a LoadMap
-- pre-hook would have bought us.
--   * on new GameMode -> gate ON + run every registered flush (each module nils its
--     UObject caches, pure Lua only — never touch the engine, and never touch the
--     half-constructed actor passed to the callback).
--   * the gate self-clears after a grace period; loops then re-find everything
--     fresh via FindAllOf.
-- Every loop checks Transition.active() FIRST, before touching any engine object.
--
-- All state lives in _G so a Ctrl+Shift+R hot reload can't orphan it: the notify
-- callback (registered ONCE, install() from main.lua) reaches the current module
-- instance through _G, and flush callbacks are keyed by module name so a reloaded
-- module replaces its old entry instead of leaking it.

local M = {}

local S = _G.__KakarotTransition
if not S then
    S = { off_clock = nil, flush = {} }
    _G.__KakarotTransition = S
end

local GRACE_S = 2.0   -- gate length after the new map's GameMode appears: long enough
                      -- to skip the teardown/BeginPlay storm, short enough that the
                      -- loading screen reader (tips) comes back while the load runs

-- Register (or replace, by name) a flush callback run when a transition starts.
-- MUST be pure Lua / native-bridge only: it runs mid-map-switch.
function M.on_begin(name, fn)
    S.flush[name] = fn
end

function M.active()
    if not S.off_clock then return false end
    if os.clock() >= S.off_clock then
        S.off_clock = nil
        return false
    end
    return true
end

function M.begin_transition()
    S.off_clock = os.clock() + GRACE_S
    for _, fn in pairs(S.flush) do pcall(fn) end
end

-- Repointed on every (re)load so the once-registered notify always calls the
-- current module instance.
_G.__KakarotTransitionBegin = M.begin_transition

-- Install the GameMode-construction notify. Called once from main.lua (NOT on hot
-- reload); guarded so a second call is a no-op. Returns true when installed.
function M.install()
    if _G.__KakarotTransitionHooked then return true end
    local ok, err = pcall(function()
        NotifyOnNewObject("/Script/Engine.GameModeBase", function(_gm)
            -- _gm is half-constructed — never touch it. Pure Lua only.
            print("[KakarotAccess] New game mode — transition gate ON\n")
            pcall(_G.__KakarotTransitionBegin)
        end)
    end)
    if not ok then
        print("[KakarotAccess] game mode notify FAILED: " .. tostring(err) .. "\n")
        return false
    end
    _G.__KakarotTransitionHooked = true
    return true
end

return M
