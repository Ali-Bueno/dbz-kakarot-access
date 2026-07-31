-- Gamepad reader + game-input blocker (thin Lua wrapper over input_bridge.dll).
--
-- The R3 radar target picker (radar_menu.lua) needs to read the pad WHILE
-- keeping the game from reacting to the same buttons. input_bridge.dll IAT-hooks
-- the game's XInputGetState (XINPUT1_3.dll) so we read the true pad and, while
-- Input.block(true) is set, the game receives a neutral pad. See input_bridge.c.
--
-- This module holds NO menu logic — just polling, the XINPUT button constants,
-- and edge/state helpers. All behaviour stays in radar_menu.lua.

local Input = {}

local ib = nil
local loaded = false
local hooked = false

-- XINPUT_GAMEPAD_* button bitmask (public XInput constants).
Input.BTN = {
    DPAD_UP        = 0x0001,
    DPAD_DOWN      = 0x0002,
    DPAD_LEFT      = 0x0004,
    DPAD_RIGHT     = 0x0008,
    START          = 0x0010,
    BACK           = 0x0020,
    LEFT_THUMB     = 0x0040,
    RIGHT_THUMB    = 0x0080,
    LEFT_SHOULDER  = 0x0100,   -- L1 / LB
    RIGHT_SHOULDER = 0x0200,   -- R1 / RB
    A              = 0x1000,
    B              = 0x2000,
    X              = 0x4000,
    Y              = 0x8000,
}

-- Analog trigger press threshold (0..255). R2/L2 are triggers, not buttons.
Input.TRIGGER_ON = 45

function Input.init()
    local ok, mod = pcall(require, "input_bridge")
    if ok and mod and mod.poll then
        ib = mod
        loaded = true
        hooked = mod.install() == true
        print(string.format("[KakarotAccess] input_bridge loaded (hooked=%s)\n", tostring(hooked)))
    else
        loaded = false
        print("[KakarotAccess] input_bridge FAILED to load: " .. tostring(mod) .. "\n")
    end
    return loaded
end

function Input.is_loaded() return loaded end
function Input.is_hooked() return hooked end

-- Snapshot of the pad, or nil if it can't be read. Fields:
--   buttons (bitmask), lt/rt (0..255), lx/ly/rx/ry (-1..1),
--   plus convenience: r2/l2 (booleans), and helpers below use it.
function Input.read()
    if not loaded then return nil end
    local b, lt, rt, lx, ly, rx, ry = ib.poll(0)
    if not b then return nil end
    return {
        buttons = b, lt = lt, rt = rt,
        lx = lx, ly = ly, rx = rx, ry = ry,
        r2 = rt >= Input.TRIGGER_ON,
        l2 = lt >= Input.TRIGGER_ON,
    }
end

-- Is a button held in this snapshot?
function Input.down(snap, mask)
    return snap ~= nil and (snap.buttons & mask) ~= 0
end

-- ---- rising-edge latch ------------------------------------------------------------
-- `Input.read()` reports a LEVEL — what is held at the instant of the call — so a press whose
-- whole down-up cycle falls between two polls is invisible. That is not a theoretical gap: the
-- 20 ms pad dispatch drops a tick whenever the game thread is busy, throttles itself to 100 ms
-- during loads and cutscenes, and every step that speaks blocks the game thread for the length
-- of the screen-reader call. Presses got eaten, and on the world map that read as the d-pad
-- "skipping" destinations (user, 2026-07-31).
--
-- input_bridge.dll therefore accumulates rising edges inside the XInput hook, at the game's own
-- frame rate, ahead of all of that. The native latch is DESTRUCTIVE — draining it takes the
-- edges — so it is drained exactly ONCE per pad tick, by pad_poll.lua, and republished here for
-- every stepper in that tick. A stepper that drained it itself would steal the others' presses.
local edge_bits = 0

-- Drain the native latch into this tick's shared edge set. PAD_POLL ONLY.
function Input.begin_tick()
    if loaded and ib.take_edges then
        local e = ib.take_edges()
        edge_bits = (type(e) == "number") and e or 0
    else
        edge_bits = 0   -- older input_bridge.dll: callers fall back to their level compare
    end
end

-- Buttons that went down during (or since) this tick, as an XINPUT bitmask.
function Input.edges() return edge_bits end

-- Did `mask` go down this tick? Callers should OR this with their own level-based edge test,
-- so an input_bridge.dll without the latch keeps the previous behaviour instead of going dead.
function Input.pressed(mask)
    return (edge_bits & mask) ~= 0
end

-- Hide/show the pad from the GAME (no-op if the hook isn't installed).
function Input.block(on)
    if loaded then ib.block(on and true or false) end
end

-- Hide the KEYBOARD from the game for `ms` milliseconds — the keyboard twin of block(),
-- used while the radar picker is open so its keys don't also reach the game (the arrows
-- are mount/dismount in the game's default layout).
--
-- It is a LEASE, not a switch: it EXPIRES unless renewed, so call it every tick while the
-- menu is open and pass 0 (or simply stop calling) to release. That is deliberate — a
-- latched keyboard block would survive a mod crash and leave the player unable to press
-- anything, pause and Escape included, with no way back but killing the game. Renewing is
-- one cheap native call per tick. Returns true if the block can actually take effect.
-- Guarded with `ib.kb_block` present so an older input_bridge.dll simply does nothing.
function Input.kb_block(ms)
    if loaded and ib.kb_block then return ib.kb_block(ms or 0) == true end
    return false
end

-- Drive the game's LEFT stick to (lx, ly) in -1..1 for the next few frames (auto-releases
-- if not refreshed). Used by the world-map cursor auto-move. Returns true if it can take
-- effect (hook installed). No-op / false otherwise.
function Input.inject(lx, ly)
    if loaded and ib.inject then return ib.inject(lx, ly) == true end
    return false
end

function Input.inject_off()
    if loaded and ib.inject_off then ib.inject_off() end
end

return Input
