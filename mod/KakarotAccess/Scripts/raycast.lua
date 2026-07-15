-- Raycast helper: line-of-sight probing for the audio-radar obstacle avoidance.
--
-- Area02 (the overworld) has no NavMesh (verified 2026-07-04), so the "route around
-- obstacles" guidance can't come from pathfinding. Instead we cast collision rays with
-- the engine's KismetSystemLibrary and steer the beacon toward a clear bearing (see
-- nav_tracker.steer_around). This module ONLY does the trace; the steering math lives
-- in nav_tracker.
--
-- CAUTION: LineTrace is a reflected engine call, unverified on this game, and a bad
-- reflected call here is an uncatchable C++ abort. Every call is pcall-guarded AND the
-- caller wraps the first use in a one-shot native-safety probe (like the NavMesh calls).
--
-- Collision channel: EObjectTypeQuery is an opaque per-game enum. Default UE mapping is
-- 1 = WorldStatic (level geometry: buildings, walls, terrain), 2 = WorldDynamic. We
-- trace against both by default; the Ctrl+F5 nav dump reports each so the right one can
-- be confirmed in-game and narrowed if needed.

local Core = require("ui_core")

local Ray = {}

Ray.OBJECT_TYPES = { 1, 2 }              -- WorldStatic + WorldDynamic (tunable)
local NO_COLOR = { R = 0, G = 0, B = 0, A = 0 }

local ksl_cache
local function ksl()
    if Core.valid(ksl_cache) then return ksl_cache end
    ksl_cache = StaticFindObject("/Script/Engine.Default__KismetSystemLibrary")
    if Core.valid(ksl_cache) then return ksl_cache end
    ksl_cache = nil
    return nil
end

-- Cast a ray start->end against `objtypes` (defaults to WorldStatic+WorldDynamic).
-- Returns: blocked(boolean), distance(number|nil), or nil when the trace API is
-- unavailable / the call didn't behave (so the caller can disable the feature).
--   ctx = a world-context UObject (the player pawn).
-- UE4SS wants EVERY UFunction parameter passed, out-params included — skipping OutHit
-- raised "expected 13 parameters, received 11" (UE4SS.log 2026-07-14). An out-param is
-- passed as a Lua table the call fills in (signature per Engine.hpp:11636 in the CXX
-- dump: ..., FHitResult& OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime).
function Ray.probe(ctx, sx, sy, sz, ex, ey, ez, objtypes)
    local k = ksl()
    if not k or not Core.valid(ctx) then return nil end
    local a = { X = sx, Y = sy, Z = sz }
    local b = { X = ex, Y = ey, Z = ez }
    local types = objtypes or Ray.OBJECT_TYPES
    local out = {}
    local hit
    local ok = pcall(function()
        hit = k:LineTraceSingleForObjects(ctx, a, b, types, false, {}, 0,
            out, true, NO_COLOR, NO_COLOR, 0.0)
    end)
    if not ok or type(hit) ~= "boolean" then return nil end
    local dist
    if hit then pcall(function() dist = tonumber(out.Distance) end) end
    return hit, dist
end

-- Convenience: is the straight segment CLEAR (nothing blocking)? Returns true/false,
-- or nil if the trace API is unavailable (distinct from "clear" so the caller can tell).
function Ray.clear(ctx, sx, sy, sz, ex, ey, ez, objtypes)
    local blocked = Ray.probe(ctx, sx, sy, sz, ex, ey, ez, objtypes)
    if blocked == nil then return nil end
    return not blocked
end

-- Channel-based variant (KismetSystemLibrary.LineTraceSingle): same shape as probe()
-- but traces ONE ETraceTypeQuery channel (0 = TraceTypeQuery1 = Visibility in the
-- default UE mapping) instead of an object-type array. Separate entry point so the
-- caller can fuse-test each reflected overload on its own: ForObjects aborts
-- uncatchably on some maps (Area02 + Area04, 2026-07-06) and its
-- TArray<EObjectTypeQuery> marshalling is the prime suspect — this overload swaps
-- that for a plain enum.
function Ray.probe_channel(ctx, sx, sy, sz, ex, ey, ez, channel)
    local k = ksl()
    if not k or not Core.valid(ctx) then return nil end
    local a = { X = sx, Y = sy, Z = sz }
    local b = { X = ex, Y = ey, Z = ez }
    local out = {}
    local hit
    local ok = pcall(function()
        hit = k:LineTraceSingle(ctx, a, b, channel or 0, false, {}, 0,
            out, true, NO_COLOR, NO_COLOR, 0.0)
    end)
    if not ok or type(hit) ~= "boolean" then return nil end
    local dist
    if hit then pcall(function() dist = tonumber(out.Distance) end) end
    return hit, dist
end

-- clear() over the channel overload; same nil-vs-boolean contract.
function Ray.clear_channel(ctx, sx, sy, sz, ex, ey, ez, channel)
    local blocked = Ray.probe_channel(ctx, sx, sy, sz, ex, ey, ez, channel)
    if blocked == nil then return nil end
    return not blocked
end

return Ray
