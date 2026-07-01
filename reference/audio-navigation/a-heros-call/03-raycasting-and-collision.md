# 03 — Grid Raycaster & Wall/Door Occlusion

Source: `Sound/Ian/CollisionHelperCopy.cs`. This is the single primitive the whole system stands on:
**"march a ray across a tile grid and report every tile boundary it crosses, in order."** The radar,
the POI scans, the audio occlusion, and the beacon line-of-sight checks are all thin wrappers over it.
`FirstPerson/Ian/CH.cs` is a near-identical copy used by gameplay (objects, sliding collision); the
math is the same, so this doc covers the canonical `CollisionHelperCopy` version.

---

## 1. Public entry points

```csharp
// First wall/impassable/door tile the ray hits (null if none within scanDistance). Used by the radar.
public static AudibleTileCollision? GetWallOrImpassableOrDoorCollision(
        IAudibleMap map, V2 startPosition, V2 slope /*unit dir*/, float scanDistance)
{
    foreach (AudibleTileCollision c in GetLineToTileCollision(map, startPosition, slope, scanDistance))
        if (c.Tile.IsWall || c.Tile.IsImpassable || c.Tile.IsDoor)
            return c;
    return null;
}

// Total thickness (in tile-units) of walls + doors lying between source and target. Used by audio occlusion.
public static float GetWallAndDoorDepth(
        IAudibleMap map, V2 source, V2 target,
        bool heardThroughWalls = false, bool heardThroughDoors = false)
{
    float length = (source - target).Length;
    V2 slope = (target - source); slope.Normalize();
    List<AudibleTileCollision> hits = GetLineToTileCollision(map, source, slope, length);
    float depth = 0f;
    for (int i = 0; i < hits.Count - 1; i++) {
        var t = hits[i].Tile;
        if ((!heardThroughWalls && t.IsWall) || (!heardThroughDoors && t.IsDoor))
            depth += (hits[i + 1].Point - hits[i].Point).Length;   // span of this blocking tile along the ray
    }
    return depth;
}
```

`GetWallAndDoorDepth` returns **how many tile-units of solid wall/door the ray passes through**, by
summing the segment length the ray spends inside each blocking tile (distance from this crossing to
the next). This is the "obstruction" number that drives muffling and muting (Doc 08 §3).

---

## 2. The core: `GetLineToTileCollision` (a grid DDA)

This is a classic **grid traversal / DDA**: it finds every point where the ray crosses an integer
horizontal gridline (`y = k`) and every point where it crosses an integer vertical gridline (`x = k`),
identifies the tile on the entering side of each crossing, then sorts all crossings by distance from
the start. The result is the ordered list of tiles the ray passes through, with the exact entry point
of each.

```csharp
private static List<AudibleTileCollision> GetLineToTileCollision(
        IAudibleMap map, V2 startPosition, V2 slope, float scanDistance)
{
    V2 second = startPosition + slope;
    Line line = Line.FromPoints(startPosition.AsV3(), second.AsV3(), isSegment:false);
    V3 reach = line.Slope * scanDistance;            // ray extent as a vector
    float dy = Math.Abs(reach.Y);                    // how far in Y we travel
    float dx = Math.Abs(reach.X);                    // how far in X we travel
    var list = new List<AudibleTileCollision>();

    // --- horizontal gridline crossings (y = integer) -> gives tile column from solved X ---
    GetGridLineStartAndIncrement(startPosition.Y, slope.Y, dy, out int start, out int inc, out int end);
    if (!line.IsHorizontal)
        for (int i = start; i != end; i += inc) {
            float x = line.SolveForX(i);             // x where ray crosses y=i
            int tx = (int)Math.Floor(x);
            int ty = (inc == -1) ? (i - 1) : i;      // tile is below/above the line depending on direction
            if (!IsInRangeWithExclusiveMax(tx, ty, map.TileWidth, map.TileHeight)) break;
            list.Add(new AudibleTileCollision { Point = new V2(x, i), Tile = map.GetTile(tx, ty) });
        }

    // --- vertical gridline crossings (x = integer) -> gives tile row from solved Y ---
    GetGridLineStartAndIncrement(startPosition.X, slope.X, dx, out int start2, out int inc2, out int end2);
    if (!line.IsVertical)
        for (int j = start2; j != end2; j += inc2) {
            float y = line.SolveForY(j);             // y where ray crosses x=j
            int tx = (inc2 == -1) ? (j - 1) : j;
            int ty = (int)Math.Floor(y);
            if (!IsInRangeWithExclusiveMax(tx, ty, map.TileWidth, map.TileHeight)) break;
            list.Add(new AudibleTileCollision { Point = new V2(j, y), Tile = map.GetTile(tx, ty) });
        }

    // order all crossings by distance from the start (interleaves the H and V crossings correctly)
    return list.OrderBy(p => (p.Point - startPosition).LengthSquared).ToList();
}
```

### The stepping helper

```csharp
private static void GetGridLineStartAndIncrement(
        float startCoord, float slopeComponent, float distance, out int start, out int inc, out int end)
{
    if (slopeComponent > 0f) {                       // moving in +axis
        start = (int)Math.Ceiling(startCoord);       // first integer line ahead
        inc   = 1;
        end   = (int)Math.Floor(startCoord + distance);
    } else {                                          // moving in -axis
        start = (int)Math.Floor(startCoord);
        inc   = -1;
        end   = (int)Math.Ceiling(startCoord - distance);
    }
    end += inc;                                       // make 'end' exclusive for the != loop
}

private static bool IsInRangeWithExclusiveMax(int x, int y, int maxX, int maxY)
    => 0 <= x && x < maxX && 0 <= y && y < maxY;
```

### Notes that bite you when porting

- **Two separate sweeps + sort**, not a single incremental DDA. Simpler to reason about, slightly more
  allocation. For hot paths you can replace with an Amanatides–Woo single-pass DDA, but keep the output
  contract identical: ordered `(entryPoint, tile)` crossings.
- The `ty = (inc == -1) ? i-1 : i` (and the X analogue) chooses **which tile owns the crossing** based
  on travel direction — the tile you're *entering*. Get this wrong and occlusion off-by-one-tiles.
- Bounds check **breaks** the sweep (stops the ray at the map edge). `EPSILON = 0.0001f` exists in the
  class but the canonical paths above don't need it; `CH.cs` uses it for degenerate slopes.
- `EPSILON`, `IsHorizontal`/`IsVertical` guard the divide-by-zero cases (`SolveForX` divides by `A`,
  `SolveForY` by `B`).

---

## 3. `AudibleTileCollision` and the tile contract

```csharp
// Sound/Ian/AudibleTileCollision.cs
public struct AudibleTileCollision { public V2 Point; public IAudibleTile Tile; }

// Sound/Ian/IAudibleTile.cs  — the per-tile flags the raycaster consults
public interface IAudibleTile {
    int ID; string IndexedTerrainName; string StepSound;
    string ConstantSound; string SecondarySharedSound; bool UniqueEmit;
    string ZoneName;
    bool IsWall; bool IsImpassable; bool IsIndoors; bool IsDoor;
    int X; int Y;
}
// Sound/Ian/IAudibleMap.cs
public interface IAudibleMap {
    int TileWidth; int TileHeight; List<IAudibleZone> Zones; IAudibleTile GetTile(int x, int y);
}
```

So the **entire world model** the navigation system needs is: a width/height, `GetTile(x,y)`, and per
tile the booleans `IsWall / IsImpassable / IsDoor / IsIndoors`, a `ZoneName`, and the terrain/sound
strings. That is the whole adapter surface for a tile game.

---

## 4. Occlusion → audio (how `GetWallAndDoorDepth` is consumed)

`Sound/Ian/ObstructionHelpers.cs`:

```csharp
public static void PositionSoundForObstruction(
        MapAndPlayer ent, V3 basePosition, IMovableSound sound,
        bool heardThroughWalls = false, bool silentInMenus = false, bool heardThroughDoors = false)
{
    if (silentInMenus && ent.AreWeInMenu) { sound.Mute = true; return; }

    float depth = CollisionHelperCopy.GetWallAndDoorDepth(
        ent.Map, ent.PlayerPosition, basePosition.Xy, heardThroughWalls, heardThroughDoors);

    sound.Mute = depth >= 1f;                         // >= 1 tile of wall/door -> silent

    int px = (int)Math.Floor(ent.PlayerPosition.X), py = (int)Math.Floor(ent.PlayerPosition.Y);
    int sx = (int)Math.Floor(basePosition.X),       sy = (int)Math.Floor(basePosition.Y);
    if (sx >= 0 && sy >= 0 && sx < ent.Map.TileWidth && sy < ent.Map.TileHeight) {
        bool playerIndoors = ent.Map.GetTile(px, py).IsIndoors;
        bool soundIndoors  = ent.Map.GetTile(sx, sy).IsIndoors;
        sound.SetObstructionAndIndoorStatus(depth, playerIndoors, soundIndoors);  // -> FMOD params
        sound.Update(basePosition);                   // -> FMOD 3D position
    }
}
```

- **Mute threshold**: `depth >= 1.0`. Less than a full tile of obstruction is *not* muted but the
  `depth` value (0..1) is still handed to FMOD as the `"Obstruction"` parameter to drive a low-pass
  muffle (Doc 08 §3). One tile or more → fully silent.
- `heardThroughWalls` / `heardThroughDoors` let specific emitters ignore walls/doors (e.g. a quest
  beacon you should always hear).
- `IMovableSound.SetObstructionAndIndoorStatus(obstruction, playerIndoors, soundIndoors)` maps to FMOD
  event parameters `"Obstruction"`, `"PlayerIndoors"`, `"SoundIndoors"` — the FMOD project's DSP graph
  turns those into the actual filtering/reverb.

---

## 5. Cookbook — what to call for each need

| You want… | Call | Returns |
|-----------|------|---------|
| First wall ahead along a beam (radar) | `GetWallOrImpassableOrDoorCollision(map, pos, dir, far)` | `AudibleTileCollision?` (point + tile) |
| All tiles a ray passes (scans) | `GetLineToTileCollision(map, pos, dir, dist)` (private; mirrored by `CH.GetLineToTileCollision`) | ordered `List<…>` |
| How muffled is a source | `GetWallAndDoorDepth(map, listener, src)` | `float` thickness in tiles |
| Distance to wall in a direction (overview) | walk tiles via `Get(x,y).Terrain.CanPass` (Doc 04 §map overview) | `int` tiles |

For a **non-tile game**, replace `GetLineToTileCollision` with your physics raycast and synthesize the
two derived helpers: "first blocker" = first raycast hit on the wall layer; "depth" = sum of
penetration spans (sphere/box-cast and accumulate hit thicknesses, or simply count discrete blockers ×
an assumed thickness). The downstream radar/occlusion code does not care how you produced the numbers.
