# 02 — The Reactive Radar (centerpiece)

Source: `Sound/Ian/ReactiveRadar.cs` (full class), driven by `MapSoundHandler.RunRadar` →
`ReactiveRadar.RunRadar(MapAndPlayer)`. Uses the raycaster in Doc 03 and the panning primitive in Doc 08.

This is the system you specifically wanted. It is what makes corridors "feel" navigable: as you move,
it continuously tells you (a) **how close the wall ahead is** (rising pitch within 3 tiles) and
(b) **whether the space to your front/left/right is opening up or closing in** (two distinct
"open"/"closed" cues), all panned to the correct ear.

> **"Wall" means *any impassable obstacle*, not just literal walls.** Note the raycast in §4 is
> `GetWallOrImpassableOrDoorCollision` — A Hero's Call itself radars against everything the player
> cannot pass through. When porting, the beams must hit impassable rocks, trees, fences, furniture,
> **invisible walls** / out-of-bounds barriers, etc. The test is *"does this block movement?"*, never
> *"is this rendered/named as a wall?"*.

---

## 1. Mental model

Three **beams** are cast every radar tick: `Left`, `Front`, `Right`, pointing along the player's
`Left`/`Front`/`Right` unit vectors. Each beam does two independent jobs:

1. **Pitch / proximity** — if the wall the beam hits is within `PitchRadarReach = 3` tiles *and* the
   player is actually travelling along that beam's direction, play one of three "Wall Approach" sounds.
   Closer = higher pitch (`Wall Approach 3` at 0 tiles, `2` at 1 tile, `1` at ≥2 tiles).
2. **Open / closed transition** — track the *line* formed by successive collision points along the
   beam. When that wall-line's orientation changes (you pass a corner / opening), decide whether the
   space just **opened** (play "Open space") or **closed** (play "Closed space"), panned to that beam's
   side.

Only **one** cue is emitted per radar tick (the loop `break`s after the first beam that plays
something), and ticks are throttled to `delay = 0.1 s` (10 Hz). This keeps the soundscape sparse.

Panning is **discrete**: each beam is constructed with a fixed FMOD pan value —
`Left = 0f (left ear)`, `Front = 1f (center)`, `Right = 2f (right ear)` — fed to the FMOD event's
`"Panning"` parameter (Doc 08 §1). The radar does **not** use 3D positioning for these cues; it uses
flat stereo pan, which is cheaper and unambiguous.

---

## 2. Data structures

```csharp
internal sealed class ReactiveRadar : DiagSO
{
    // Three "Wall Approach" pitches for the proximity cue, all on one beam's pan.
    private sealed class RadarPitches {
        I2DSoundSource Pitch1, Pitch3, Pitch5;
        public RadarPitches(FModSoundContext sc, float pan) {
            Pitch1 = sc.GetRadarSound("Wall Approach 1", pan);   // farthest -> lowest
            Pitch3 = sc.GetRadarSound("Wall Approach 2", pan);
            Pitch5 = sc.GetRadarSound("Wall Approach 3", pan);   // closest -> highest
        }
        public void Play(int distance) =>
            (distance switch { 0 => Pitch5, 1 => Pitch3, _ => Pitch1 }).Play();
        //  0 tiles -> Approach3 (highest) ; 1 -> Approach2 ; >=2 -> Approach1 (lowest)
    }

    private sealed class RadarBeam {
        public V2  Direction;              // set each tick to ent.Front/Left/Right
        public V2  LastCollisionPoint;     // wall hit point from previous tick
        public Line LastCollisionLine;     // line through (prevPoint, prevPrevPoint)
        public int LastRoundedDistance;    // floored distance to wall last tick
        public bool TravellingThisDirection; // is the player actually moving along this beam?
        public readonly I2DSoundSource Open, Close;   // "Open space" / "Closed space" (this pan)
        private readonly RadarPitches Pitch;
        public I2DSoundSource LastOpenOrCloseSoundPlayed;

        public RadarBeam(FModSoundContext sc, float pan) {
            Open  = sc.GetRadarSound("Open space",   pan);
            Close = sc.GetRadarSound("Closed space", pan);
            Pitch = new RadarPitches(sc, pan);
        }
        public void PlayOpen (V2 p){ Open.Play();  LastOpenOrCloseSoundPlayed = Open;  }
        public void PlayClose(V2 p){ Close.Play(); LastOpenOrCloseSoundPlayed = Close; }
        public void PlayPitch(V2 p, int d){ Pitch.Play(d); }
    }

    public bool Enabled = true;
    private RadarBeam Front, Right, Left;
    private List<RadarBeam> AllBeams = new();
    private double NextRadarTime;
    private const int    PitchRadarReach     = 3;      // tiles
    private const int    OpenCloseRadarReach = 1000;   // tiles (effectively "to the far wall")
    private const double delay               = 0.1;    // seconds between cues
    private readonly Stopwatch mRealTime = new();

    public ReactiveRadar(FModSoundContext sc) {
        mRealTime.Start();
        Left  = new RadarBeam(sc, 0f);    // pan hard left
        Front = new RadarBeam(sc, 1f);    // pan center
        Right = new RadarBeam(sc, 2f);    // pan hard right
        AllBeams.Add(Left); AllBeams.Add(Front); AllBeams.Add(Right);
    }
}
```

> `ResetRadar()` / `ResetAllButFrontRadar()` clear each beam's `LastCollisionPoint`/`LastCollisionLine`.
> They are called on intentional facing changes (turns, snaps, teleports) so the open/closed history
> doesn't produce a spurious transition from stale data. (See callers in Doc 04.)

---

## 3. The tick dispatcher — `RunRadar`

```csharp
public void RunRadar(MapAndPlayer ent)
{
    if (!Enabled) return;
    double now = mRealTime.Elapsed.TotalSeconds;
    if (NextRadarTime >= now) return;                 // throttle: at most one tick / 0.1s

    V2 v = ent.Velocity.Normalized();                 // normalized travel heading
    Front.Direction = ent.Front;
    Right.Direction = ent.Right;
    Left.Direction  = ent.Left;

    // Is the player moving (roughly) along each beam? Compare normalized velocity to the
    // 8 direction vectors. A beam counts as "travelling" if velocity equals that direction
    // OR either adjacent diagonal.
    Front.TravellingThisDirection =
        ent.Front == v || ent.FrontLeft == v || ent.FrontRight == v;
    Left.TravellingThisDirection  =
        ent.Left.Normalized() == v || ent.FrontLeft == v || ent.BackLeft  == v;
    Right.TravellingThisDirection =
        ent.Right.Normalized() == v || ent.FrontRight == v || ent.BackRight == v;

    foreach (RadarBeam b in AllBeams) {
        if (PlayRadarBeamSound(ent, b, ent.PlayerPosition, v)) {
            NextRadarTime = now + delay;              // a cue played -> reset throttle
            break;                                    // ONE cue per tick
        }
    }
}
```

Note the equality tests use `V2.operator==` (exact). Because `v` and the direction vectors are both
produced by the same normalization paths, exact equality works in practice when the player is moving
exactly along a cardinal/intercardinal; off-axis motion simply yields `TravellingThisDirection=false`,
which suppresses the pitch cue (you only get proximity pitch when you're heading at the wall).

---

## 4. The per-beam logic — `PlayRadarBeamSound` (the heart)

```csharp
private bool PlayRadarBeamSound(MapAndPlayer ent, RadarBeam rb, V2 position, V2 travelHeading)
{
    // (A) Raycast this beam to the first wall/impassable/door, up to 1000 tiles.
    AudibleTileCollision? hit =
        CollisionHelperCopy.GetWallOrImpassableOrDoorCollision(ent.Map, position, rb.Direction, 1000f);

    V2 point = V2.Zero; double dist = double.MaxValue; int rounded = int.MaxValue;
    if (hit.HasValue) {
        point   = hit.Value.Point;
        dist    = (position - point).Length;
        rounded = (int)Math.Floor(dist);
    }

    // (B) Shift history: remember this tick's collision point; recall last tick's.
    V2 lastPoint = rb.LastCollisionPoint;
    rb.LastCollisionPoint = point;

    int lastRounded = rb.LastRoundedDistance;
    rb.LastRoundedDistance = rb.TravellingThisDirection ? rounded : int.MaxValue;

    bool played = false;

    // (C) PROXIMITY PITCH: wall within 3 tiles, distance changed, and moving along this beam.
    if (rounded < PitchRadarReach && rounded != lastRounded && rb.TravellingThisDirection) {
        rb.PlayPitch(position, rounded);              // 0->highest .. 2->lowest
        played = true;
    }

    // (D) OPEN/CLOSED only makes sense for beams that are NOT (anti)parallel to travel.
    //     If this beam points the way you're going (or directly behind), skip the transition test.
    if (rb.Direction.AlmostEquals(travelHeading))       return played;
    if (rb.Direction.AlmostEquals(travelHeading * -1f)) return played;

    // (E) Need two valid consecutive collision points to form a wall-edge line.
    if (point == V2.Zero || lastPoint == V2.Zero) { rb.LastCollisionLine = null; return played; }
    if (point == lastPoint)                          return played;   // wall didn't move; no event

    // (F) Build the line through the previous two collision points (the local wall edge).
    Line line = Line.FromPoints(point.AsV3(), lastPoint.AsV3(), isSegment:false);
    Line lastLine = rb.LastCollisionLine;
    rb.LastCollisionLine = line;
    if (lastLine == null)            return played;   // first time; nothing to compare
    if (line.Equivalent(lastLine))   return played;   // same orientation -> no transition

    // (G) Orientation changed: we turned a corner along this beam. Decide open vs closed
    //     by comparing which side of the PREVIOUS wall-line the player and the new hit are on.
    rb.LastCollisionLine = null;                      // consume the transition
    bool playerSide = lastLine.IsPointLeftOfLine(position.AsV3());
    bool hitSide    = lastLine.IsPointLeftOfLine(point.AsV3());
    if (playerSide == hitSide) rb.PlayClose(position); // new wall on same side as you -> closing in
    else                       rb.PlayOpen(position);  // new wall flipped to other side -> opening
    return true;
}
```

### Why (G) works — the open/closed geometry

`lastLine` is the wall edge sampled on previous ticks. `IsPointLeftOfLine` returns the sign of the 2D
cross product, i.e. **which half-plane** a point sits in. If, after the wall's orientation changes,
the *new* collision point sits on the **same side** of the old wall-line as the player, the wall has
effectively swung toward you → **closed**. If it flips to the **opposite side**, the wall receded →
**open**. This is a cheap, robust corner detector that needs only the previous two hits — no global
map analysis.

---

## 5. Exact behavior summary (for re-implementation)

Per radar tick (max 10 Hz), for beams in order **Left, Front, Right**, stop at first that emits:

```
hit = raycast(playerPos, beamDir, far)
rounded = floor(distance to hit)
travelling = velocity ~ beamDir or its two adjacent diagonals

# proximity pitch
if rounded < 3 and rounded != lastRounded and travelling:
    play  WallApproach[ {0:3(high), 1:2(mid), >=2:1(low)} ]  panned to beam side
    emitted = true

# open/closed (skip if beam is parallel/antiparallel to travel)
if beamDir !~ travel and beamDir !~ -travel:
    if have two valid consecutive hits and they differ:
        line = through(thisHit, lastHit)
        if lastLine exists and orientation(line) != orientation(lastLine):
            same = sign(lastLine, playerPos) == sign(lastLine, thisHit)
            play (same ? "Closed space" : "Open space")  panned to beam side
            emitted = true
update beam.lastHit, beam.lastRounded, beam.lastLine
return emitted
```

Pan values: Left beam → 0 (L), Front → 1 (C), Right → 2 (R).

---

## 6. Porting the radar (engine-agnostic recipe)

The radar needs only three things from your engine adapter:

1. `playerPos` (Vector2 in some consistent unit) and `velocity` (or last-frame delta).
2. `front/left/right` unit vectors (derive from heading; Doc 01 §4).
3. `Raycast(origin, dir, maxDist) -> (hitPoint, isBlocking?)` against **every movement-blocking
   obstacle** — walls, impassable rocks/trees, invisible barriers — i.e. the same colliders/tiles the
   player's own movement collides with, not just a "walls" layer (Doc 03 gives the tile version; in a
   3D game use the physics raycast and project to the navigation plane).
4. A "play short sound, pitch p, stereo pan ∈ {L,C,R}" primitive. If you don't have FMOD, three
   pre-pitched WAVs per side and a stereo-balance call is enough.

Keep the **0.1 s throttle**, the **one-cue-per-tick** rule, and the **`TravellingThisDirection`
gate** — without them the cues become noise. The `PitchRadarReach = 3` and the distance→pitch table
are the two knobs you'll tune first for a new game's scale (a tile = 1 unit here; in a 3D brawler you
might set "reach" to e.g. 2–3 meters).

For continuous (non-grid) games, the only place that *needs* integers is the pitch bucket
(`floor(distance)`); everything else is float. Replace `floor(distance)` with a bucketizer suited to
your scale, e.g. `bucket = clamp(floor(distanceMeters / stepMeters), 0, 2)`.

See Doc 10 for concrete SF6 / RE7 / 7DTD / XV2 mappings (e.g. using the engine's nav-mesh or physics
sweep instead of a tile raycast, and quantizing enemy/wall distance into the same 3-bucket pitch).
