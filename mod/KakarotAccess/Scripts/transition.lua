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
-- Signal (2026-07-25): a WORLD EPOCH polled on the GAME THREAD. `Directory.world_epoch()`
-- returns the address of the current UWorld, reached through the GameInstance (the one object
-- that never dies, so it costs a cached validity check and a `GetWorld()` call). `Core.poll_world`
-- hands it to `note_epoch` below on every poll of every loop. A changed address means a new
-- world; no world at all means the old one is gone. Both arm the gate and flush the caches.
-- NOT the PlayerController, which was the obvious candidate and is wrong: this game has several
-- and they swap mid-world when the player mounts, so the epoch would flap and flush caches for
-- no reason. `false` is a THIRD answer meaning "could not look this tick" — it must never arm
-- the gate, or a spent scan budget would freeze the reader on a loop.
--
-- THIS REPLACED NotifyOnNewObject("/Script/Engine.GameModeBase"), which was the LAST
-- construction notify in the mod. Two reasons, both from the ledger:
--   * THREAD. UE4SS delivers object-construction notifies on the engine's async LOADING
--     thread as well as the game thread (MEASURED 2026-07-14 with mem_bridge.thread_id:
--     5744 foreign vs 38620 game). Any mod Lua on a foreign thread runs the SAME lua_State
--     concurrently with the poll loop — allocator and incremental GC race, and a userdata
--     the game thread still holds gets freed under it. That is the documented
--     lua_State-corruption crash, and the callback here did real work: a require, a
--     thread_id call, a print, and every registered flush. The GameMode is an actor, so it
--     was *probably* always on the game thread — "probably" was the whole problem. A poll
--     from the game thread has no thread question at all.
--   * TIMING. The GameMode is constructed AFTER the old world's objects are freed, so the
--     gate armed too LATE. The old comment here claimed no Lua tick can run in between;
--     that is FALSE (2026-07-21 review): LoadMap fires dozens of UFunctions through
--     ProcessEvent and EVERY one drains our ExecuteInGameThread queue, so our tick DOES
--     run mid-teardown, against caches full of freed pointers. The epoch poll notices the
--     OLD controller dying, which is strictly earlier than the new GameMode appearing.
-- Rejected alternatives, still rejected (verified live 2026-07-04): RegisterLoadMapPre/
-- PostHook hard-crashes THIS game on the first map load (SteamStub exe, misplaced
-- trampoline), and NotifyOnNewObject("/Script/Engine.World") fires for every streamed
-- sublevel (127 in 3 minutes) — the gate was on half the time.
--
-- Probing a dead controller every tick is exactly the abort this gate exists to prevent,
-- which is why the "harden root_ok" idea was called CIRCULAR in 2026-07-21. It is not
-- circular any more: `Core.valid` now pre-checks the object's memory through mem_bridge
-- under SEH before UE4SS ever dereferences it (see `Mem.alive`). That fix is what makes
-- this design possible at all.
--
--   * epoch changed or gone -> gate ON + run every registered flush (each module nils its
--     UObject caches, pure Lua only — never touch the engine).
--   * the gate self-clears after a grace period; loops then re-find everything fresh.
-- Every loop checks Transition.active() FIRST, before touching any engine object.
--
-- All state lives in _G so a Ctrl+Shift+R hot reload can't orphan it, and flush callbacks
-- are keyed by module name so a reloaded module replaces its old entry instead of leaking.

local M = {}

local S = _G.__KakarotTransition
if not S then
    S = { off_clock = nil, flush = {} }
    _G.__KakarotTransition = S
end

local GRACE_S = 2.0   -- gate length after the new map's world appears: long enough
                      -- to skip the teardown/BeginPlay storm, short enough that the
                      -- loading screen reader (tips) comes back while the load runs

-- FAILSAFE. The gate is held for as long as no world can be found, so a bug or an engine
-- change that makes the epoch permanently unreadable would mute the ENTIRE mod with no
-- error anywhere — the worst possible failure for an accessibility tool. After this long
-- with no world the gate gives up and lets the loops run unguarded: the memory pre-check
-- (Mem.alive) protects every read now, so degrading to "unguarded but talking" is strictly
-- better than "silent and safe". Says so once in the log when it happens.
local WORLD_DOWN_MAX_S = 60.0

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

-- `why` names the trigger. It is LOGGED because a false arm is otherwise indistinguishable from a
-- real one, and the blast radius of a false arm is total: while the gate is up,
-- `Directory.resolve` returns an EMPTY list for every mapped class (ui_directory: "resolve to
-- absent, not fallback"), so EVERY directory-mapped screen reports that it does not exist and goes
-- silent. That is not a hypothetical — it is the most likely reason the result screen stopped
-- narrating on 2026-07-25. Transitions are rare, so one line each costs nothing.
function M.begin_transition(why)
    S.off_clock = os.clock() + GRACE_S
    print("[KakarotAccess] transition gate ON (" .. tostring(why or "?") .. ")\n")
    for _, fn in pairs(S.flush) do pcall(fn) end
end

-- ---- the world-epoch poll (the gate's only signal) --------------------------------------
--
-- Called from ui_core.begin_scan_tick, i.e. on the GAME THREAD, with the value of
-- Directory.world_epoch():
--   number -> this is the live world's identity
--   nil    -> no live world (boot, or a map switch in progress)
--   false  -> a world exists but its identity could not be read → say nothing, change nothing
-- Pure Lua: it runs mid-map-switch, so it must never touch the engine.
-- DEBOUNCE on the "no world" answer (2026-07-25). A single nil used to arm the gate immediately,
-- and every nil after it re-extended the grace — the exact flapping-falling-edge shape that had
-- just been found and fixed for the battle-HUD signal in ui_registry, left unfixed here. It is far
-- more dangerous in this spot: an armed gate makes `Directory.resolve` answer "absent" for EVERY
-- mapped class, so one flicker of `Core.valid(w)` around a battle ending silences the whole
-- directory for GRACE_S — the leading explanation for the result screen going quiet. So a nil must
-- PERSIST before it counts. Measured in WALL TIME, not calls: note_epoch runs from every loop,
-- several times per 100 ms tick, so a call counter would confirm in milliseconds and confirm
-- nothing. The cost of waiting is ~0.3 s of ticks running against a dying world at a real map
-- switch, which the memory pre-check (Mem.alive) now makes survivable — a trade that did not exist
-- before today. An epoch CHANGE is never debounced: two different valid worlds is unambiguous.
local DOWN_CONFIRM_S = 0.3

function M.note_epoch(e)
    if e == false then return end                 -- unreadable: not an event
    if e == nil then
        S.nil_since = S.nil_since or os.clock()
        if (os.clock() - S.nil_since) < DOWN_CONFIRM_S then return end   -- not confirmed yet
        if S.epoch ~= nil then                    -- the world we knew really is gone
            S.epoch = nil
            S.down_since = os.clock()
            M.begin_transition("world gone")
        elseif S.down_since == nil then           -- boot, before any world exists
            S.down_since = os.clock()
            S.off_clock = os.clock() + GRACE_S
        elseif (os.clock() - S.down_since) < WORLD_DOWN_MAX_S then
            S.off_clock = os.clock() + GRACE_S    -- hold the gate while it stays down
        elseif not S.down_warned then
            S.down_warned = true
            print("[KakarotAccess] no world for " .. math.floor(WORLD_DOWN_MAX_S)
                .. "s — releasing the transition gate (failsafe)\n")
        end
        return
    end
    -- A readable world clears the debounce AND the down state: whatever flicker we saw is over.
    S.nil_since, S.down_since, S.down_warned = nil, nil, nil
    if S.epoch == nil then                        -- a world appeared: let the grace run out
        S.epoch = e
        S.off_clock = os.clock() + GRACE_S
    elseif e ~= S.epoch then                      -- a DIFFERENT world: a map switch we missed
        S.epoch = e
        M.begin_transition("new world")
    end
end

-- Kept so main.lua's one-time boot call stays valid. There is nothing to install any more: the
-- gate is driven by note_epoch above, polled from the game thread. It just announces itself so
-- the boot log still says which gate is running.
function M.install()
    print("[KakarotAccess] Transition gate: world-epoch poll (game thread, no notify).\n")
end

return M
