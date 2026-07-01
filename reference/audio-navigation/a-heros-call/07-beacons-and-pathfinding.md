# 07 — Beacons & Pathfinding-Guided Navigation

Source: `FirstPerson/Ian/FPBeaconMenu.cs`, `Beacon.cs`, beacon logic in `FPExploring.cs`,
`GraphSearch.AStar` (the `GraphSearch` assembly), `DirectionHelper` (Doc 04 §4).

Beacons are the game's **waypoint guidance**: pick a named destination, then get repeated audio/speech
telling you which way to go. Two modes: a simple distance+direction readout, and an "advanced beacon"
that uses A* pathfinding to place a directional beep on the next reachable step toward the goal.

---

## 1. Beacon data & selection

```csharp
// Beacon.cs
public sealed class Beacon {
    public string  IndexedName;     // internal id
    public TString FriendlyName;    // spoken name
    public float X, Y;              // tile-centered position (X+0.5, Y+0.5)
    public V2 Position => new V2(X, Y);
}
```

`FPBeaconMenu` lists `mWorld.Map.Beacons` as menu items; selecting one sets `map.ActiveBeacon`:

```csharp
foreach (Beacon b in mWorld.Map.Beacons) menu.Items.Add(new MyMenuItem(b.FriendlyName, HandleClick, null, b));
…
void HandleClick(MyMenuItem mi){ mWorld.Map.ActiveBeacon = (Beacon)mi.Tag; mWorld.Scene.Up(); }
```

Bound keys: `BeaconMenu` (open list), `AnnounceBeacon` (speak distance+direction now), `DisableBeacon`.

---

## 2. Simple readout — distance + 8-point direction

`FPExploring.AnnounceDistanceAndDirectionToBeacon`:

```csharp
Beacon b = mWorld.Map.ActiveBeacon;
if (b == null) { DiagSO.Say("No active beacon"); return; }
V2 toBeacon = mWorld.Position - b.Position;             // vector FROM beacon TO player
int dist = (int)Math.Round(toBeacon.Length);
RoughDirection rd = GetRoughDirection(ref toBeacon);
DiagSO.Say(dist + " " + Enum.GetName(typeof(RoughDirection), rd));   // e.g. "45 NorthEast"

static RoughDirection GetRoughDirection(ref V2 toBeacon) {
    double a = Math.Atan2(toBeacon.X, -toBeacon.Y);     // compass bearing (Doc 01 §2)
    float dir = MyMath.RadiansToDegrees((float)a) + 180f;
    return DirectionHelper.GetRoughDirectionEnum(dir);  // 45°-wide bins
}
```

> The `atan2(X, -Y) + 180°` converts the player↔beacon delta into a compass bearing and then the
> `+180` flips it from "direction to player" into "direction to beacon" before binning into the
> 8-point `RoughDirection`. (If you instead compute `beacon - player`, drop the `+180`.)

`DirectionHelper.GetRoughDirectionEnum` bins (Doc 04 §4): N 337.5–22.5, NE 22.5–67.5, E 67.5–112.5,
SE 112.5–157.5, S 157.5–202.5, SW 202.5–247.5, W 247.5–292.5, NW 292.5–337.5.

---

## 3. Advanced beacon — A*-guided directional beeps

When `GameConfig.UseAdvancedBeacon`, instead of a raw bearing the game pathfinds and plays a
directional beacon sound positioned on the **next reachable tile along the path**, so you're guided
around walls rather than into them.

```csharp
float dist = (mWorld.Position - b.Position).Length;
if (dist < 5f) { PlaySimpleBeacon(); }                 // close: fall back to simple cue

SinceLastAdvancedBeaconSound += GameTimeSinceLastTick;
if (SinceLastAdvancedBeaconSound < 2.0) return;        // throttle: one beep / 2 s
SinceLastAdvancedBeaconSound = 0.0;

FPTile playerTile = FSO.FPML.GetPlayerTile(mWorld);
FPTile goalTile   = FSO.FPML.GetTile(mWorld, b.Position);
if (playerTile == goalTile) {                          // arrived
    mWorld.SC.PlayPlain2DIfExists("arrived at beacon"); DisableBeacon(); return;
}

// A* on the tile graph, NON-diagonal moves.
SEquatableEdge<FPTile>[] path = GraphSearch.AStar(mWorld.Map, playerTile, goalTile, includeDiagonals:false);
if (path == null) { DiagSO.Say("You can't get there from here"); return; }

// Walk up to 10 path tiles; keep the farthest one with clear line-of-sight from the player.
FPTile beepTile = null; int n = 0;
foreach (var edge in path) {
    if (++n > 10) break;
    FPTile t = edge.Target;
    float len = (t.Center - mWorld.Position).Length;
    V2 slope = (t.Center - mWorld.Position); slope.Normalize();
    if (CH.GetWallOrImpassableCollision(mWorld.Map, mWorld.Position, slope, len) != null) break; // blocked -> stop
    beepTile = t;
}
if (beepTile == null) { DiagSO.Say("Could not find a tile to play the sound from"); return; }

// Direction from that tile back to player -> choose one of 8 pre-positioned directional sounds.
V2 toBeacon = mWorld.Position - beepTile.Center;
RoughDirection rd = GetRoughDirection(ref toBeacon);
I3DSoundSource se = rd switch {
    North=>mABNorthSE, South=>mABSouthSE, East=>mABEastSE, West=>mABWestSE,
    NorthEast=>mABNorthEastSE, NorthWest=>mABNorthWestSE, SouthEast=>mABSouthEastSE, SouthWest=>mABSouthWestSE };
se.Update(beepTile.Center.AsV3());                     // position at the path tile
se.Play();
```

The eight `mAB*SE` sounds are preloaded with effectively infinite min/max distance and no reverb so
they always play clearly:

```csharp
mABNorthSE = mWorld.SC.Get3DSound("ab north", "map", 10000f, 10000f, useEnvironmentalReverb:false);
// … ab south / east / west / northeast / northwest / southeast / southwest
```

### Why this design is good (and worth copying)

- **A* keeps guidance honest** — it routes you around obstacles; a raw bearing would point you at walls.
- **Line-of-sight clamp (the 10-tile walk)** places the beep as far ahead as you can actually hear in a
  straight line, so the cue sits "down the corridor" at the next turn, not on an unreachable diagonal.
- **2-second throttle** prevents nagging.
- **Auto-degrade to simple cue under 5 tiles** and **auto-disable on arrival**.

`GraphSearch.AStar(map, from, to, includeDiagonals)` returns an array of `SEquatableEdge<FPTile>`
(graph edges); `edge.Target` is the next tile. The `GraphSearch` assembly is a small generic A* over
the tile adjacency — portable as-is, or replace with your engine's nav-mesh path.

---

## 4. Porting notes

- **Simple beacon** is trivial anywhere: `bearing = atan2(dx, -dy) deg`, bin to 8 points, speak
  `"{round(distance)} {dir}"`. Great for "where's the objective/exit/enemy" in any game.
- **Advanced beacon**: use your engine's pathfinder (Unity `NavMesh.CalculatePath` for 7DTD; in
  RE-Engine games without exposed nav, fall back to simple bearing or a coarse waypoint graph you
  build). Keep the **line-of-sight clamp** and **throttle**. Position the beep in 3D at the chosen
  waypoint so spatial audio does the pointing; speak the 8-point direction as a fallback for users who
  prefer words.
- Tile-center convention `(X+0.5, Y+0.5)` matters for distance accuracy; in continuous games just use
  the world positions directly.
