# Dragon Ball radar — September 8, 2026

Status: implemented with offline regression tests and code review; positive
in-game pickup verification is still pending. The existing R3 / V category uses
the game's ordinary displayed markers, direction/distance and beacon. No unlocks,
save changes, debug spawning, placement-table inspection or new world scans.

## Root cause

The radar already classified EMapIcon::DRAGON_BALL (28), but its minimap walk
classified TargetActor through ATMapIconComponent. The game registers Dragon Ball
markers directly with the pickup actor, so a component-free actor was discarded.
The old item-names note's working-status claim came from classification code,
not a captured world pickup.

## Native evidence

Ghidra 12.1.2 analyzed the unpacked normal-version executable read-only, without
reanalysis. The original installed executable matched the copied original:
SHA-256 8DDDFE8B35C6612B7755936675231D1C87F309245ABB6A878C6A80E97EDDA826.
That is not the unpacked program's hash. Reflected declarations are from the
UE4SS AT.hpp and AT_enums.hpp dumps.

| Native function | Evidence |
|---|---|
| 0x1411e8920 | HUD OnSpawnedDragonball delegates to the field manager. |
| 0x141552c30 | Field manager passes type 0x1c and the actor to AddIcon. |
| 0x141587930 | Map manager adds the marker to minimap and area map. |
| 0x1415d5510 | Type 28 uses the ordinary minimap icon pool. |
| 0x1415e80a0 | Pool at +0x6A8, count +0x6B0; reuses a slot when +0x88 is zero or TargetActor null, then sets TargetActor and +0x88=1. |
| 0x1415f50d0 / 0x1415e6950 | Write/read the EMapIcon byte at +0x89. |
| 0x141545380 / 0x14159f290 | Collection removes type 28 plus the actor through the map manager. |
| 0x1415ee940 | Minimap removal matches type/TargetActor and invokes retirement. |

UAT_UIMiniMapIcon is size 0xA8: reflected TargetActor +0x28, WL_Owner +0x30,
WL_Icon_ImgSw +0x48. UAT_UIMiniMapRadar.MapIconList is the TArray at +0x6A8.
Navigation/range icons have separate fixed arrays, not TArrays.

MapDBIns and AT_UIStartDragonBallMenu describe owned balls, not world locations.
DragonBallManager.m_dragonBallInfo is a placement table and is not consulted.

## Important native limits

Independent native review traced retirement through 0x1415e4130 to 0x1415e47f0.
This path does not clear active, type or TargetActor; it writes +0x8A=1 with no
proven reset path, so the reader does not use +0x8A. Rendered switcher visibility
is load-bearing, not optional. Actual post-collection visibility and fade timing
still need gameplay verification.

Vtables 0x143ecb408, 0x143ecb6a8 and 0x143ecb948 share the type getter at +0x248
and retirement thunk at +0x250. The initial indirect-call argument reconstruction
was unreliable; the sibling tables supplied the cross-check. 0x1415e03e0 rebuilds
ordering, not occupancy. +0x8C is a fixed-position override (0x1415e4180), not
collection. The pool constructor was not recovered; use native-base IsA, not
an exact class-name match that could silently reject a subclass.

## Reader contract

dragonball_marker.lua validates native-base inheritance before private reads,
then type 28, pool-in-use, owner identity, rendered switcher, valid TargetActor
and actor visibility. Native reads use Mem; offsets are in native_offsets.lua.
Only the process-lifetime UClass is cached, never world actors or widgets.

Only this displayed-marker path may nominate a Dragon Ball; component scans
cannot restore a hidden marker. Selection revalidates the current list instead
of trusting a cached picker row. Approach and arrival-wait consumers recheck
availability. contains returns true / false / nil, so unreadable native state
is not treated as collection. A rejected resume keeps the existing bounded retry.

There are no new world scans, but active tracking walks the minimap list every
100 ms. Representative late-game cost is not measured.

## Verification and live checklist

The offline suite exercises real enumeration, selection, beacon ticks, collection
chaining and menu resume with engine-boundary doubles. It covers component-free
and derived icons, duplicate/order handling, inactive/hidden/reused markers,
stale selection, unreadable state and a temporary failure during resume. Failing
regressions were observed before the corresponding corrections.

Still test with an ordinary save where a Dragon Ball is available:

1. R3 / V → Dragon Balls; choose a ball and verify F5 direction/distance and beacon.
2. Collect before and after the arrival cue; check retirement and next-ball chaining.
3. Pause/resume while approaching and while standing on the ball; it must not skip.
4. Reload/change areas and check current-area-only markers.
5. Confirm before-unlock parity, rendered-marker retirement and acceptable tick cost.

Offline tests cannot certify Unreal visibility or actor lifetime. Cross-region
world-map summaries and new unlocking mechanics are outside this change.
