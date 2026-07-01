# 10 — Porting Guide: SF6 / 7 Days to Die / Xenoverse 2 / RE7

This maps every AHC technique to your four target games. The strategy is always the same:

> Implement one **engine adapter** that yields `PlayerPose { Vector2/3 pos, float headingDeg }`, a
> `Raycast(origin, dir, maxDist) -> (hit?, point, blockingKind)`, and a `Speak(text, interrupt)` +
> `PlaySound(id, pan|pos, pitch)` primitive. Then copy the engine-free `Ian` logic (math, raycaster,
> radar, POI scan, beacon math) on top of it.

The only conceptual shift: AHC is a **2D tile grid**; SF6/XV2/RE7/7DTD are **continuous 3D**. So
(a) replace tile `GetTile/GetLineToTileCollision` with the engine's physics raycast/nav queries, and
(b) quantize to integers *only* where the algorithm needs it (the radar pitch bucket = `floor(distance)`
→ replace with `clamp(floor(distance/step),0,2)`).

---

## 0. Shared adapter skeleton (pseudo-C#)

```csharp
interface INavAdapter {
    Vector3 PlayerPos { get; }           // world units
    float   HeadingDeg { get; }          // compass: 0=forward/N, CW; derive from camera/facing
    // first blocking hit along a ray on the "wall/obstacle" layer:
    bool RaycastWall(Vector3 origin, Vector3 dir, float maxDist, out Vector3 hit);
    // sum of obstacle thickness between two points (for occlusion); cheap version = count*assumedThk:
    float Occlusion(Vector3 a, Vector3 b);
    // nearby interactables/enemies for scans:
    IEnumerable<INavTarget> Targets { get; }
    void Speak(string text, bool interrupt);
    void PlayPanned(string id, float pan /*-1..1*/, float pitch);   // radar cue
    void Play3D(string id, Vector3 worldPos, float pitch);          // positioned cue
}
```

Port `GetUnitVectorFromCompassDegrees`, `GetCompassAngle`, `atan2(dx,-dy)` bearing, the 8 direction
vectors, `Line.IsPointLeftOfLine`, and the radar state machine verbatim against this adapter.

---

## 1. Resident Evil 7 (RE Engine, first-person) — **closest match**

RE7 is almost a 1:1 conceptual port: first-person, rooms, corridors, doors, pick-up items, objectives.

| AHC feature | RE7 mapping |
|-------------|-------------|
| Player pos/heading | Camera/world transform; `HeadingDeg` from camera yaw |
| `GetTile`/wall raycast | RE Engine physics raycast on world/collision layer (via REFramework `sdk`/Lua or native plugin) |
| **Reactive radar** | Cast Front/Left/Right rays each tick; bucket distance to nearest wall into 3 pitches; open/closed via `IsPointLeftOfLine` on successive hit points. Tune `PitchRadarReach` to ~2–3 m |
| Occlusion | `Occlusion(listener, src)` = physics linecast; muffle item/enemy sounds behind walls; mute ≥ ~1 wall |
| **POI scan** (front/left/right) | Fan a few rays; classify hits: door, item, enemy, wall; speak `"{name} {round(m)}"`; 2.5 m focus / 5 m act split for the interact prompt |
| Examine/Interact | Bind to RE7 interact; speak item description / aim assist via focused target |
| Region/zone announce | Use RE7 room ids / area triggers → "entering {room}"; reverb already engine-side |
| Beacons | Set objective as beacon; simple `atan2` bearing + distance readout; advanced beacon needs a nav graph (RE7 has no exposed navmesh → use simple bearing) |
| Speech | REFramework is native/Lua → **PRISM** (your default) directly |

Tooling: **REFramework** (not BepInEx). A native plugin can host PRISM (your verified prebuilt path) and
do the raycasts via RE Engine's `via.physics`. This is the project where you can reuse the most code.

---

## 2. 7 Days to Die (Unity Mono, BepInEx) — **best tooling fit**

Full 3D open world with a real **NavMesh** and physics — the advanced beacon shines here.

| AHC feature | 7DTD mapping |
|-------------|--------------|
| Player pos/heading | `EntityPlayerLocal.position`, camera yaw → `HeadingDeg` |
| Wall raycast | `Physics.Raycast` on terrain/block layer; for voxel walls, raycast or sample block grid |
| **Reactive radar** | Front/Left/Right `Physics.Raycast`; 3-bucket pitch by meters; open/closed transitions for doorways/cover. Great for indoor POIs/buildings |
| Occlusion | `Physics.Linecast` listener→source; muffle zombies/loot behind walls (Unity `AudioLowPassFilter`) |
| **POI scan** | `Physics.SphereCast`/`OverlapSphere` + raycast; classify: zombie, lootable, door, vehicle, workstation; speak name+distance; diff-gated auto-forward scan |
| Region/zone | Biome / POI (prefab) name → "entering {POI/biome}"; Unity reverb zones for acoustics |
| **Advanced beacon** | `NavMesh.CalculatePath(player, questPOI)`; walk path corners, line-of-sight clamp, 3D beep at next corner + spoken 8-point dir; auto-degrade < 5 m, throttle 2 s |
| Speech | Legacy .NET/BepInEx → **Tolk** via `TolkDotNet` (`DavyKager`) is acceptable fallback; PRISM if you do a native bridge |

Tooling: **BepInEx 5 + Harmony**. `Tolk.dll`+`nvdaControllerClient64.dll` in game root, `TolkDotNet.dll`
in plugins (per your standing setup). The radar + scan + advanced beacon all map cleanly to Unity APIs.

---

## 3. Street Fighter 6 (RE Engine, 2.5D fighting) — **reinterpret "navigation" as spacing**

A fighting game has no exploration, but the *same primitives* answer the questions a blind player needs:
**range to opponent, which side they're on, corner proximity, jump/projectile spatialization.** This is
the highest-value reuse of the radar idea in a non-exploration game.

| AHC concept | SF6 reinterpretation |
|-------------|----------------------|
| Player position (1D axis matters most) | Your X on the stage axis; opponent X; `dx = opp.X - me.X` |
| "Front wall" pitch radar | **Corner-proximity radar**: distance from you to the nearest stage corner → 3-bucket pitch (closer to corner = higher). Left/right beam = which side the wall/corner is |
| "Open/closed" transition | Crossing-up detection: when opponent flips to your other side, play the L↔R "open/closed" cue panned to the new side (reuse `IsPointLeftOfLine` sign on the X axis) |
| Opponent as an `IAudibleObject` | Continuous panned tone whose **pan = sign(dx)**, **pitch/volume = f(range)** → instant "they're far right / point-blank left" |
| POI scan | On-demand: speak `"distance {round}, {left|right}, {state}"` (state = jumping/crouching/blocking/throwing) |
| Beacon bearing (`atan2`) | Not needed (1D), but vertical: announce opponent airborne via a higher-pitched/elevated cue |
| Occlusion | N/A (no walls between fighters) — drop it |
| Speech | RE Engine → **PRISM** native; or REFramework Lua bridging to a native TTS plugin |

Concrete SF6 radar adaptation: each frame compute `dx`, `range=|dx|`, `side=sign(dx)`; emit a panned
tone (`pan=side`, `pitch=lerp(range)`); separately, a corner-radar: `distToCorner = stageHalfWidth - |me.X|`,
bucket to 3 pitches, pan to the corner side. Throttle like AHC (10 Hz, one cue/tick) to avoid mush.

Tooling: **REFramework** + native PRISM plugin. Many SF6 values (positions, states) are readable via
RE Engine reflection; community accessibility mods already pull battle state.

---

## 4. Dragon Ball Xenoverse 2 (native, XV Patcher) — **3D arena spacing + lock-on**

3D arena brawler with a lock-on target. Reuse the beacon/bearing math and a 3D-relative radar.

| AHC concept | XV2 mapping |
|-------------|-------------|
| Player pos/heading | Player world transform; facing/camera yaw → heading |
| Target as beacon | Locked-on enemy = active beacon: `bearing = atan2(dx, -dz)` (use the ground-plane axes), speak `"{round(dist)} {8-point dir}"`, or continuous 3D-panned tone |
| **Radar** | Front/Left/Right rays for arena walls/ring-out edges → 3-bucket pitch (ring-out proximity is the "wall approach"!); open/closed for losing/gaining target line-of-sight |
| Vertical axis | XV2 has flight: add an up/down cue (pitch-shift or a distinct vertical tone) for target elevation `dy` |
| Occlusion | Optional: muffle target sound when scenery blocks line-of-sight (native raycast) |
| POI scan | Speak nearest enemy/item, distance, relative direction, elevation |
| Speech | Native → **PRISM** (host the prebuilt DLL in your XV Patcher plugin) |

Tooling: **XV Patcher / native plugin** (not BepInEx). Use Ghidra (your DBXV2 workflow) to find player
& target structs; host PRISM natively. The bearing/beacon and 3-bucket radar port directly; add the Y
(elevation) dimension AHC didn't need.

---

## 5. Per-technique portability cheat-sheet

| Technique | Engine-free? | Needs from adapter | Notes |
|-----------|--------------|--------------------|-------|
| Compass math, 8 dir vectors, bearings | ✅ pure | nothing | copy `MyMath`/`VectorHelpers`/`VectorExtensions` |
| `Line` geometry, `IsPointLeftOfLine` | ✅ pure | nothing | radar open/closed + crossup detection |
| Grid raycaster (`GetLineToTileCollision`) | tile-only | replace with physics raycast | output contract = ordered hits |
| **Reactive radar** state machine | ✅ pure | `RaycastWall`, dir vectors, `PlayPanned` | keep 10 Hz, 1 cue/tick, travel gate, 3-bucket pitch |
| Occlusion depth → muffle | ✅ logic | `Occlusion(a,b)` | `mute if ≥1 wall; else LPF by (1-depth)` |
| POI scan + diff-gate | ✅ logic | `Raycast`, `Targets`, `Speak` | 2.5 focus / 5 act; range-collapse names |
| Region/zone announce | ✅ logic | room/area id, `Speak(interrupt)` | interrupt on change |
| Simple beacon (bearing+dist) | ✅ pure | target pos, `Speak` | `atan2(dx,-dy)`+8-bin |
| Advanced beacon (A*) | needs navmesh | engine pathfinder | LOS clamp + 2 s throttle + 3D beep |
| Speech routing | ✅ logic | PRISM/Tolk | one sink, `interrupt` on context change |

---

## 6. Recommended build order (per game)

1. **Adapter + Speech** first: get `Speak("test", true)` working through PRISM/Tolk and read out player
   position/heading on a key. Validates the whole pipeline.
2. **Simple beacon / opponent bearing**: cheapest high-value cue (`atan2` + 8-bin + distance).
3. **Reactive radar**: port the state machine; wire `RaycastWall` + `PlayPanned`. Tune `PitchRadarReach`
   and the distance→pitch buckets to the game's scale. This is the feature you came for.
4. **POI scan** (RE7/7DTD) or **spacing/corner radar** (SF6/XV2).
5. **Occlusion** and **zone reverb** last (polish).

Start with **RE7** (most code reuse) or **7DTD** (best tooling), validate the radar there, then carry the
tuned constants into the harder native targets (SF6/XV2).
