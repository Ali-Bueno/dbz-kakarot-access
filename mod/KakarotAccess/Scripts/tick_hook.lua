-- The mod's PREFERRED heartbeat: the game's own per-frame Blueprint tick.
--
-- WHY THIS EXISTS. Every `ExecuteInGameThread` call creates a brand-new lua_State and inserts it
-- into UE4SS's `lua_instances` map — a `static std::unordered_map` with no mutex from which
-- nothing is ever erased — on the ASYNC thread, while the GAME thread reads that same map on
-- every reflected member access. That race is what has been killing sessions (see pad_poll.lua's
-- header and the crash notes). `ExecuteInGameThread` is the ONLY Lua API in UE4SS's `LuaMod.cpp`
-- that allocates a state per call; `RegisterHook` creates its state ONCE, at registration time
-- (`LuaMod.cpp:2977`), and reuses it forever. So a hook on a UFunction the game already calls
-- every frame hands us the game thread for free and takes the steady-state insert rate to ZERO.
--
-- WHY THIS EXACT FUNCTION, and why it must be a BLUEPRINT one. UE4SS dispatches a Blueprint hook
-- through `script_hook` (`LuaMod.cpp:3463`), which holds `m_thread_actions_mutex` AND wraps the
-- call in `TRY` (`:3587`) — the same protection `ExecuteInGameThread` has today. The NATIVE
-- branch (`:111-215` / `:216-360`) has neither, so an uncatchable throw there kills the process
-- outright; hooking a native tick would have been worse than the problem. `BP_ATGameModeMain_C`
-- carries `UberGraphFrame` + `ExecuteUbergraph_BP_ATGameModeMain`, i.e. real compiled Blueprint
-- bytecode, and the 2026-08-03 in-gameplay dump found exactly ONE live instance of it
-- (`Area11_P:PersistentLevel`) — a GameMode is an engine-enforced singleton. Every other
-- Blueprint tick in that dump was rejected on evidence: `Icon_New_C:Tick` swings between 0 and
-- 51+ concurrent instances depending which list menu is open, and the anim-blueprint and
-- fishing-pawn candidates had no live instance at all.
--
-- THIS IS AN OPTIMISATION, NEVER A DEPENDENCY. A GameMode is per MAP — the title screen runs
-- `BP_GameModeTitle_C`, so this hook does not fire there, and another sub-map may well use a
-- third one. pad_poll therefore keeps its LoopAsync as a watchdog and takes over the moment the
-- beat stops. Worst case for everything in this file is the behaviour the mod already had.
--
-- KEPT IN ITS OWN FILE ON PURPOSE, exactly like header_hook.lua: if hooking ever destabilises
-- this game, the rollback is deleting this file — pad_poll falls back on its own, and nothing
-- else in the mod references it.

local Core = require("ui_core")   -- Core.valid only (the existence probe below)

local Hook = {}

-- OFF UNTIL DIAGNOSED (2026-08-03). The first build of this file CRASHED THE GAME AT BOOT:
-- `EXCEPTION_ACCESS_VIOLATION reading address 0x00000010` — the `UObjectBase::ClassPrivate` read,
-- i.e. a dereference of a handle that was not a live object. Evidence, and it is unambiguous
-- about WHERE even though not about WHICH call:
--   * `crash_trail.bin` recorded **0 marks for that session**. The first thing `dispatch()` does
--     is `Mem.mark("pad.tick")`, so the process died BEFORE any dispatch ran.
--   * `UE4SS.log` ends at UE4SS's own `Event loop start`, immediately after the mod printed
--     "Lua loaded. Accessibility active." — so it died on the first pass of the event loop.
--   * `Poll.pump` calls `Hook.arm` FIRST, above the mark. That is the only new code in that
--     window, and there is no "tick hook installed" line, so `RegisterHook` was never reached —
--     which leaves the `StaticFindObject` existence probe below.
-- The likely mechanism, NOT yet proven: at boot `Mem`'s class-pointer offset has not been derived
-- yet, so `Mem.alive` correctly FAILS OPEN, `Core.valid` degrades to a bare `IsValid()`, and
-- `IsValid` dereferences — on whatever that lookup returned during async load. The probe was
-- supposed to be the SAFE half of this design and is instead the part that fired.
-- Do not simply flip this back on: the fix is to stop probing by object path at boot at all —
-- arm only once the mod is demonstrably in gameplay (`Core.free_roam`), by which point the
-- Blueprint class is loaded and the memory pre-check is armed. Kept here rather than deleted
-- because the FINDING (BP_ATGameModeMain_C:ReceiveTick, and why it is the right target) cost a
-- dump pass and is correct; only the arming path is wrong.
local ENABLED = false

-- Dump-proven, not guessed and not a family probe: `CXXHeaderDump\BP_ATGameModeMain.hpp:11`
-- declares `void ReceiveTick(float DeltaSeconds);` and `UE4SS_ObjectDump.txt:155301` carries the
-- Function object under exactly this path (2026-08-03, captured in-gameplay in `Area11_P`).
local HOOK_PATH = "/Game/System/BP_ATGameModeMain.BP_ATGameModeMain_C:ReceiveTick"

-- Registration can only succeed once that Blueprint class is LOADED, and it is not at boot: the
-- title screen runs a different GameMode entirely. So the attempt is retried until it takes and
-- then never again.
--
-- DELIBERATELY NOT `Mem.find_object`, which is otherwise the mod's one throttled resolver: its
-- policy is to GIVE UP permanently past a deadline, which is right for an object that should
-- already exist and wrong here, where absence is the expected state for as long as the player
-- sits at the title screen. This retry must never expire.
local RETRY_EVERY_S = 2.0
local next_try = 0

-- STATE LIVES IN _G, and the hook closure captures NOTHING from this module. That is what makes
-- Ctrl+Shift+R safe: a reload replaces the function behind `__KakarotTickFn` while the installed
-- hook — which cannot be unregistered and re-registered without leaking a second one — keeps
-- reading the current value and so keeps calling live code. `__KakarotTickInstalled` is in _G for
-- the same reason: a reload must not install a second hook on the same function.
--
-- `__KakarotTickBeat` is the watchdog signal pad_poll reads from its worker thread. A COUNTER
-- rather than a timestamp on purpose: the reader is then an integer compare with no C call on the
-- shared lua_State, which is the access class that loop is already limited to.
if _G.__KakarotTickBeat == nil then _G.__KakarotTickBeat = 0 end

-- Point the heartbeat at `fn` and, if it is not installed yet, try to install it. Safe to call on
-- every dispatch: once installed this is one boolean read.
function Hook.arm(fn)
    if not ENABLED then return false end
    _G.__KakarotTickFn = fn
    if _G.__KakarotTickInstalled then return true end
    if type(RegisterHook) ~= "function" then return false end

    local now = os.clock()
    if now < next_try then return false end
    next_try = now + RETRY_EVERY_S

    -- EXISTENCE FIRST, and this ordering is the load-bearing part. UE4SS's `RegisterHook` binding
    -- calls `throw_error` when the path does not resolve, and that is a C++ throw which PIERCES
    -- pcall — from inside our dispatch it would abort the rest of the steppers on every retry,
    -- twice a second, for as long as the player stayed at the title screen. `StaticFindObject` has
    -- no such failure mode: a MISS yields a null cpp object, and `r ~= nil` is not the test for
    -- that (UE4SS answers an INVALID RemoteObject, never nil) — `Core.valid` is, and it is safe on
    -- exactly this shape for the reason mem.lua:192-195 records for the mod's own resolver: the
    -- handles the validity test faults on are FREED ones, which StaticFindObject cannot return.
    -- So we only ever hand `RegisterHook` a path we have just proven resolves.
    local found = false
    pcall(function() found = Core.valid(StaticFindObject(HOOK_PATH)) end)
    if not found then return false end

    local ok = pcall(function()
        RegisterHook(HOOK_PATH, function()
            -- Bumped BEFORE the callback, and unconditionally: the watchdog must be able to tell
            -- "the hook is alive" apart from "the callback did nothing this frame", and a beat
            -- that only advanced on useful work would read as a dead hook the moment the grid
            -- throttled us.
            _G.__KakarotTickBeat = (_G.__KakarotTickBeat or 0) + 1
            local f = _G.__KakarotTickFn
            if f then f() end
        end)
    end)
    if not ok then return false end

    _G.__KakarotTickInstalled = true
    -- Announce it once. The whole point of this file is that its absence is invisible — the
    -- LoopAsync fallback keeps the mod working — so "did it install?" must not need a debugger.
    print("[KakarotAccess] tick hook installed on " .. HOOK_PATH
        .. " — ExecuteInGameThread is no longer the heartbeat\n")
    return true
end

function Hook.installed() return _G.__KakarotTickInstalled == true end
function Hook.path() return HOOK_PATH end

return Hook
