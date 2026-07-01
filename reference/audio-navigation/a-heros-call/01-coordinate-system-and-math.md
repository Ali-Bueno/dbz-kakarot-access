# 01 — Coordinate System & Math Primitives

This is the foundation. Every other document assumes these conventions.

---

## 1. Vectors: `V2` / `V3`

`Core/Ian/V2.cs`, `V3.cs` — standard OpenTK-style structs (`public float X, Y[, Z]`). Nothing exotic;
the only members navigation uses heavily:

```csharp
// V2 (Core/Ian/V2.cs)
public float Length        => (float)Math.Sqrt(X*X + Y*Y);
public float LengthSquared => X*X + Y*Y;          // prefer for comparisons
public float LengthFast     // 1f / InverseSqrtFast(...) — used for footstep speed test
public V2 Normalized();     // returns a copy, unit length
public void Normalize();    // in place: num = 1/Length; X*=num; Y*=num;

// THE perpendiculars used to derive Left/Right from Front:
public V2 PerpendicularRight => new V2(Y, -X);   // +90° in screen (Y-down) space
public V2 PerpendicularLeft  => new V2(-Y, X);   // -90°

public static float Dot(V2 a, V2 b)     => a.X*b.X + a.Y*b.Y;
public static float PerpDot(V2 a, V2 b) => a.X*b.Y - a.Y*b.X;   // 2D cross (z of cross)
```

Equality is **exact float equality** (`==` calls `Equals`, compares X and Y with `==`). The radar and
movement code therefore use a separate epsilon comparison (below) for "almost equal".

```csharp
// CoreExtensionMethods.cs
public static bool AlmostEquals(this float f1, float f2) => Math.Abs(f1 - f2) <= 1e-5;
// SoundExtensionMethods.cs
public static bool AlmostEquals(this V2 a, V2 b) => a.X.AlmostEquals(b.X) && a.Y.AlmostEquals(b.Y);
public static bool AlmostEquals(this V3 a, V3 b) => …X &&…Y &&…Z;
// CoreExtensionMethods.cs
public static V3 AsV3(this V2 v) => new V3(v.X, v.Y, 0f);
```

> **Porting note:** in Unity/most engines you'll use `UnityEngine.Vector2`/`Vector3`. The only thing
> you must preserve is the **Y-down, compass-clockwise** convention used by the angle math below, or
> consistently flip it everywhere. Mixing conventions is the #1 source of "left/right is mirrored" bugs.

---

## 2. The compass-angle system

The world uses a **compass heading**: degrees clockwise, `0° = North`, and the world is **Y-down**
(`+Y` = South). Three conversion functions matter.

### Heading (degrees) → direction unit vector — the one you'll use most

```csharp
// MyMath.cs
public static V2 GetUnitVectorFromCompassDegrees(float degrees)
{
    float r = DegreesToRadians(degrees);
    float x =  (float)Math.Sin(r);
    float y = -(float)Math.Cos(r);     // NOTE the minus: Y-down world
    V2 v = new V2(x, y);
    v.Normalize();
    return v;
}
//   0° -> ( 0,-1) N     45° -> ( .707,-.707) NE     90° -> ( 1, 0) E
// 135° -> ( .707,.707) SE    180° -> ( 0, 1) S      270° -> (-1, 0) W
```

`VectorHelpers.cs` has the same math in three flavours (pick by your engine's Y direction):

```csharp
public static V3 FromCompassDegrees(float d)         // y = +cos  (Y-up compass)
public static V3 FromFlippedYCompassDegrees(float d) // y = -cos  (Y-down — what the game uses)
public static V3 FromMathDegrees(float d)            // standard math: x=cos, y=sin, 0=East CCW
```

### Direction unit vector → heading (degrees) — the inverse

```csharp
// VectorExtensions.cs
public static float GetCompassAngle(this V3 v)
{
    v.Z = 0f; v.Normalize();
    float deg = RadiansToDegrees((float)Math.Acos(v.Y));   // angle from +Y (north) axis
    return (Math.Asin(v.X) < 0.0) ? (360f - deg) : deg;    // disambiguate hemisphere by sign of X
}
```

There is also `GetMathAngleInDegrees` (angle from +X, CCW) if you ever need standard math angles.

### The beacon bearing (used to say "45, NorthEast")

`FPExploring` computes the bearing from a delta vector with `atan2` directly:

```csharp
// vector pointing FROM target TO player (player - target)
double a   = Math.Atan2(toBeacon.X, -toBeacon.Y);          // X first, -Y second -> compass
float dir  = MyMath.RadiansToDegrees((float)a) + 180f;     // shift into [0,360)
RoughDirection rd = DirectionHelper.GetRoughDirectionEnum(dir);   // 8-point bin (Doc 07)
```

> **Why `atan2(X, -Y)`?** Standard `atan2(y, x)` gives the math angle (0=East, CCW). Passing
> `(X, -Y)` swaps/negates so that 0 means North and the angle grows clockwise — i.e. a compass
> bearing — for a Y-down world. Memorize this; you will reuse it for any "which way is target" readout.

---

## 3. Degrees / radians helpers

```csharp
// MyMath.cs
public static float DegreesToRadians(float d) => (float)(d * Math.PI / 180.0);
public static float RadiansToDegrees(float r) => (float)(r * 180f / Math.PI);
```

---

## 4. The eight direction vectors (player-relative)

`Sound/Ian/MapAndPlayer.cs` — the per-frame bundle handed to the radar. `Front` is precomputed from
the heading; the rest are derived. **This is the exact basis the radar and scans use.**

```csharp
public readonly V2 Front;     // = GetUnitVectorFromCompassDegrees(Facing)
public V2 Left      => Front.PerpendicularRight;          // (Front.Y, -Front.X)
public V2 Right     => Front.PerpendicularLeft;           // (-Front.Y, Front.X)
public V2 Back      => Front * -1f;
public V2 FrontLeft  => V2.Normalize(Front + Left);
public V2 FrontRight => V2.Normalize(Front + Right);
public V2 BackLeft   => V2.Normalize(Back + Left);
public V2 BackRight  => V2.Normalize(Back + Right);
```

Worked example, facing **East** (`Facing = 90°`, `Front = (1,0)`):
- `Left  = PerpendicularRight(1,0) = (0,-1)` → North. ✓ (East-facing player's left hand points North)
- `Right = PerpendicularLeft(1,0)  = (0, 1)` → South. ✓
- `FrontLeft = normalize((1,0)+(0,-1)) = (.707,-.707)` → NorthEast. ✓

> If your engine is Y-up, either negate the Y of every direction vector once at the adapter boundary,
> or swap `Left`/`Right` definitions. Verify with the worked example above in your own frame before
> trusting any radar output.

---

## 5. `Line` — analytic line / segment geometry

`Sound/Ian/Line.cs`. The raycaster and the radar's "open vs closed" test use this. A line is stored
in implicit form `A·x + B·y = C` plus its two defining points and a normalized `Slope` (a direction).

```csharp
public float A, B, C;          // implicit form A*x + B*y = C
public V3 Point1, Point2, Slope;   // Slope = normalize(Point2 - Point1)
public bool IsHorizontal => Slope.Y == 0f;
public bool IsVertical   => Slope.X == 0f;

public static Line FromPoints(V3 p1, V3 p2, bool isSegment)
{
    A = p2.Y - p1.Y;
    B = p1.X - p2.X;
    C = A*p1.X + B*p1.Y;
    Slope = V3.Normalize(p2 - p1);
}

public float SolveForX(float y) => (C - B*y) / A;   // x on the line at given y
public float SolveForY(float x) => (C - A*x) / B;   // y on the line at given x

// Sign test: is point c strictly left of the directed line P1->P2 ?  (2D cross product sign)
public bool IsPointLeftOfLine(V3 c)
    => (Point2.X-Point1.X)*(c.Y-Point1.Y) - (Point2.Y-Point1.Y)*(c.X-Point1.X) > 0f;

// Are two (infinite) lines collinear/parallel in either direction?
public bool Equivalent(Line o) => Slope.AlmostEquals(o.Slope) || Slope.AlmostEquals(o.Slope * -1f);
```

Two members are the workhorses elsewhere:
- **`SolveForX` / `SolveForY`** drive the grid raycaster (Doc 03): step across each integer gridline and
  solve for the crossing on the other axis.
- **`IsPointLeftOfLine`** drives the radar's open/closed decision (Doc 02): compare which side of the
  previous wall-edge line the player vs the new collision point fall on.

`DistanceToPoint`, `GetIntersection`, and segment-range clamping also exist but navigation doesn't use
them on the hot path.

---

## 6. Quick reference card

```
World:    tile (i,j) = [i,i+1)x[j,j+1) ; center (i+0.5, j+0.5) ; playerTile = (floor X, floor Y)
Axes:     +X East, +Y South (Y-DOWN)
Heading:  compass degrees, clockwise, 0=N 90=E 180=S 270=W
deg->vec: ( sin, -cos ).normalize()                         [GetUnitVectorFromCompassDegrees]
vec->deg: acos(Y); if asin(X)<0 -> 360-that                 [GetCompassAngle]
bearing:  deg = RadToDeg(atan2(dX, -dY)) + 180               [beacon / "which way" readout]
Left  = Front.PerpRight = (Front.Y, -Front.X)
Right = Front.PerpLeft  = (-Front.Y, Front.X)
Cardinals: N(0,-1) E(1,0) S(0,1) W(-1,0)
eps = 1e-5
```
