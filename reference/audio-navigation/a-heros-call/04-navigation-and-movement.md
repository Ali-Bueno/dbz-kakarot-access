# 04 — First-Person Movement, Turning & Cardinal Cues

Source: `FirstPerson/Ian/FPExploring.cs` (state + input loop), `FPMapLogic.cs` (movement + collision),
`FPInput.cs` / `FPKeys.cs` (bindings), `FPMapOverview.cs` (look-around mode). Position/heading live on
the FP world (`mWorld.Position : V2`, `mWorld.Facing : float` degrees, `mWorld.Velocity : V2`).

---

## 1. State representation

| State | Type | Notes |
|-------|------|-------|
| Position | `V2` | continuous (not grid-locked); grid tile = `(floor X, floor Y)` |
| Facing | `float` degrees | compass, 0=N CW; `mWorld.LastFacing` holds previous frame for crossing detection |
| Velocity | `V2` | set by movement; `= travel * CalcMovementSpeed * speedMod` |
| Front | `V2` | `FPMapLogic.UpdateFrontVector` → `GetUnitVectorFromCompassDegrees(Facing)`; recomputed on every facing change |
| LastTravel | `V2` | last frame's normalized travel direction (used to suppress scans while strafing) |

Heading is always clamped to `[0,360)` after any change:

```csharp
if (mWorld.Facing < 0f) mWorld.Facing += 3600f;   // +10 turns so a single subtract can't go negative
mWorld.Facing %= 360f;
```

---

## 2. Movement (continuous, per-tick, collision-aware)

`FPExploring.HandleKeys()` accumulates a travel vector from held movement keys, in the player's basis:

```csharp
V2 travel = V2.Zero;
if (input.IsHeld(FPMoveForward))  travel += FPState.Front;
if (input.IsHeld(FPMoveBackward)) travel += FPState.Back;
if (input.IsHeld(FPMoveLeft))     travel += FPState.Left;
if (input.IsHeld(FPMoveRight))    travel += FPState.Right;

float speedMod = 1f;
AffectTravelFunc?.Invoke(ref travel, ref speedMod);   // hook for slows/hastes
IsTravelling = travel.Length != 0f;
if (IsTravelling) {
    travel.Normalize();
    mWorld.Player.Travel = travel;
    FSO.FPML.MoveUnitBasedOnTravelVector(mWorld, mWorld.Player, travel, speedMod);
    FSO.FPML.AfterUpdatePosition(mWorld, travel);
    mWorld.FPState.LastTravel = travel;
} else { mWorld.Player.Travel = V2.Zero; mWorld.Velocity = V2.Zero; }
```

`FPMapLogic.MoveUnitBasedOnTravelVector`:

```csharp
ent.Velocity = travel * u.CalcMovementSpeed(ent) * speedMod;
V2 change = ent.Velocity * (float)ent.GameTimeSinceLastTick;          // dt-scaled
FPLBumpResult bump = HandleUnitPointToWallCollision(ent, u, ref change);  // may zero an axis
if (bump.BumpOccured) PlayBumpSoundIfPlayer(ent, u, bump.BumpSound);
u.Position += change;
ent.FPState.DistanceTravelled += change.Length;
```

### Collision / wall blocking (`HandleUnitPointToWallCollision`)

- Compute target tile from `Position + change`. If same tile as now → no check.
- Inspect the 4 orthogonal neighbors (N `y-1`, S `y+1`, W `x-1`, E `x+1`). If the entered neighbor is
  the target and **`Terrain.CanPass == false`**, zero the offending axis of `change` (slide along the
  wall) and return that tile's `BumpSound`.
- For diagonal moves, it raycasts (`CH.GetLineToTileCollision`) and tries X-then-Y / Y-then-X sliding.
- **Bump sound throttle**: `PlayBumpSound` ignores calls within `0.6 s` of the last bump (no spam).

### Footsteps (`PlayStepSound`)

- Interval between steps = `BaseStepSoundSpeed / Velocity.Length` → **faster movement = more frequent
  steps** (inverse relationship).
- "Running" footstep variant chosen when `Velocity.LengthFast > 2f`.
- Sound comes from the current tile's `Terrain.StepSound` (per-terrain footsteps).

---

## 3. Turning

`FPExploring.HandleMouseTurning` blends mouse and key turning into a continuous heading delta:

```csharp
float m = mWorld.RawInput.MouseXChange / 8f;     // mouse sensitivity: raw delta / 8
float k = 0f;
if (input.IsHeld(FPTurnLeft))  k += -1f;          // key turn: ±1 degree/frame, accumulative
if (input.IsHeld(FPTurnRight)) k += +1f;
mWorld.LastFacing = mWorld.Facing;
mWorld.Facing += m + k;
ClampPlayerDirectionBetween0And360();
PlayCardinalDirectionSoundIfCrossed();
FSO.FPML.UpdateFrontVector(mWorld);               // recompute Front immediately
mTurnedWithMouseThisLoop = (m != 0f || k != 0f);
if (mTurnedWithMouseThisLoop) mWorld.SC.ResetAllButFrontRadar();  // clear stale open/close history
```

> **Radar interaction:** any intentional turn calls `ResetAllButFrontRadar()` (mouse/key) or
> `ResetRadar()` (snaps/teleports) so the open/closed transition logic (Doc 02 §4G) doesn't fire from
> the heading change itself.

### Snapping to cardinals

`SnapRight()` / `SnapLeft()` jump `Facing` to the next/previous cardinal (`0/90/180/270`), update
Front, announce the new direction, and **`ResetRadar()`**. Boundaries are biased so a snap always
moves you to the *next* cardinal in that rotational direction. Bound to `FPSnapLeft` / `FPSnapRight`.

---

## 4. Cardinal-direction audio cues — `AnnounceDirection`

This plays up to four short **positional** cues (north/south/east/west sound emitters placed 1 unit
from the player) for whichever cardinals your heading is within `67°` of. It's the "which way am I
roughly facing" feedback, distinct from the radar.

```csharp
ClampPlayerDirectionBetween0And360();
float f = mWorld.Facing;
float dN = (f > 270f) ? (360f - f) : f;     // angular distance to North(0)
float dE = Math.Abs(90f  - f);
float dS = Math.Abs(180f - f);
float dW = Math.Abs(270f - f);
int thresh = 67;
if (!NoSoundsOrMusic && SC.RadarEnabled) {
    if (dN < thresh) { mNorthSE.Update((North + Position).AsV3()); mNorthSE.Play(); }
    if (dS < thresh) { mSouthSE.Update((South + Position).AsV3()); mSouthSE.Play(); }
    if (dW < thresh) { mWestSE .Update((West  + Position).AsV3()); mWestSE .Play(); }
    if (dE < thresh) { mEastSE .Update((East  + Position).AsV3()); mEastSE .Play(); }
}
// cardinal vectors (Y-down): North(0,-1) South(0,1) East(1,0) West(-1,0)
```

Because `67 > 45`, near an intercardinal you'll hear **two** cardinal cues (e.g. facing NE plays both
North and East), which encodes the diagonal. Mouse-turn crossings additionally trigger
`PlayCardinalDirectionSoundIfCrossed()` which uses `DirectionHelper.GetQuadrantDirectionEnum` to fire a
cue only when you cross between quadrants/exact cardinals (avoids continuous spam while sweeping).

### Direction binning (`DirectionHelper`)

Two granularities (both consume a compass angle in degrees):

- **`GetQuadrantDirectionEnum`** → `ExactNorth/East/South/West` (at 0/90/180/270 precisely) or the four
  quadrants in between. Used for "did I cross a cardinal while turning".
- **`GetRoughDirectionEnum`** → 8-point compass (N, NE, E, SE, S, SW, W, NW), each a 45°-wide bin
  centered on the compass point (N = 337.5–22.5°, NE = 22.5–67.5°, …). Used for beacon/"which way"
  readouts (Doc 07).

---

## 5. Map overview / look-around mode (`FPMapOverview`)

A non-moving "review cursor" the player drives over the grid to survey surroundings without walking.

- Enter at the player's current tile; announce it (`AnnounceTile`).
- Arrow movement steps the review cursor one tile; **horizontal step delay `0.1 s`, vertical `0.13 s`**
  (so diagonal sweeps sound ordered).
- Blocked tiles (`!Terrain.CanPass`) stop the cursor, play the terrain `BumpSound`, and speak the
  terrain's `FriendlyName`; map edges speak "That is the edge of the map."
- It sonifies space the same way as the radar: when the cursor crosses open↔closed it plays "Open
  space"/"Closed space" (panned up/down or left/right relative to cursor motion), and plays
  `Wall Approach 3/2/1` timed by how many tiles remain to the nearest wall in that direction.
- `GetXDistanceToWall(x,y,dir)` / Y analogue: step `+dir` counting passable tiles until `!CanPass`.

This mode is essentially the radar applied to a virtual cursor instead of the player — a great pattern
for a "scan the room" key in an action game.

---

## 6. Full input map (`FPInput` enum, registered in `FPKeys`)

```
FPAnnouncePosition  FPAnnounceRegion  FPAnnounceDirection
FPSnapLeft  FPSnapRight
FPMoveForward FPMoveBackward FPMoveLeft FPMoveRight
FPTurnLeft FPTurnRight
FPScanForward FPScanLeft FPScanRight
OverheadMap (look-around)  BeaconMenu AnnounceBeacon DisableBeacon
ToggleRadarEnabled  ToggleAutoForwardScanEnabled  ToggleViewMode
FPInteract  FPExamine   SaveGame LoadGame  …
```

Registered like `AddInput("fp_move_forward", FPInput.FPMoveForward)` — string action ids map to enum
members, so rebinding is data-driven.

---

## 7. Region enter/leave (speech) — `HandleRegion`

Checked every `Process()`; **regions are a navigation concept (named areas), distinct from audio
"zones"** (reverb volumes, Doc 06).

```csharp
int x = floor(Position.X), y = floor(Position.Y);
FPRegion region = map.Tiles.Get(x,y).Region;
if (region != mCurrentRegion) {
    mWorld.Speech.CancelAllSpeech();
    if (region.IsNone() && mCurrentRegion != null) TranslateSO.TSay("leaving {0}",  mCurrentRegion.FriendlyName);
    else                                           TranslateSO.TSay("entering {0}", region.FriendlyName);
    mCurrentRegion = region;
    return true;   // signals "region changed" -> forces a fresh forward POI scan this frame
}
```

Portals (`GetPortal`/`HandlePortalAnnouncement`) work the same way: on stepping onto a portal tile,
switch maps and snap the player to `(DestX+0.5, DestY+0.5)`, optionally setting `DestFacing` and
re-announcing direction.

---

## 8. Porting checklist

- Maintain your own `position (Vector2 tile-units)`, `headingDegrees (compass)`, `velocity`.
- On any heading change: recompute `Front`, clamp to `[0,360)`, and **reset the radar's open/close
  history**.
- Movement: scale by `dt`, run a collision check that can zero one axis (slide), throttle bump sounds.
- Cardinal cues: the `67°` window with positional N/S/E/W emitters is a cheap, effective "facing"
  readout; or fall back to speaking the 8-point bin from `GetRoughDirectionEnum`.
- Add a "review cursor" mode (overview) if your game has a surveyable space; it reuses the radar logic.
