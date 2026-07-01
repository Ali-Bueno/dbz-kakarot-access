# Audio Navigation — Cross-Game Reference

Engine-agnostic playbook for building **screen-reader / audio navigation** into a game accessibility
mod. It documents **three navigation styles** distilled from real games (two decompiled, one from our
own mods) plus the shared model underneath them — so for any new game you can pick **one style, a
combination of two, or all three**.

---

## The three navigation styles (pick / combine per game)

| # | Style | One-line | Feedback shape | Reference |
|---|-------|----------|----------------|-----------|
| 1 | **Reactive radar** | Cues fire *as you move* when a wall closes in or space opens/closes | Sparse, event-driven (one cue per tick) — tells you *what changed* | [`a-heros-call/02-reactive-radar.md`](a-heros-call/02-reactive-radar.md) |
| 2 | **Polar scan radar** | On-command sweep of N rays around you, each ray's first wall sounded | On-demand burst — tells you *what's all around, in detail* | [`swamp/navigation-spec.md`](swamp/navigation-spec.md) §3 |
| 3 | **Wall sonification** | One continuous tone per direction, volume = wall proximity | Always-on ambient bed — tells you *the box you're in, right now* | [`wall-sonification/`](wall-sonification/) |

They are **complementary, not competing** — each owns a different audio "lane":

- **Wall sonification** = quiet continuous bed answering *"what's around me?"* passively.
- **Reactive radar** = sparse louder events answering *"what just changed?"* (corners, doorways, openings).
- **Polar / POI scan** = user-triggered detail answering *"what's down there?"* (often with speech).

Layer their volumes (ambient bed quietest, events louder, speech interrupts) and they don't turn to mush.

### Which to use — decision guide

| Game shape | Recommended | Why |
|------------|-------------|-----|
| **Top-down grid explorer** (Core Keeper, A Hero's Call, Swamp) | **All three.** Wall sonification with **world-fixed** N/S/E/W as the bed + reactive radar for openings + on-demand scan/speech for detail | Coarse grid + clear cardinal walls make every style cheap and legible |
| **3D first-person explorer** (RE7, DRG, 7 Days to Die) | **Wall sonification (camera-relative) as the backbone** + reactive radar for doorways/openings + simple beacon to objectives | Constant layout sense matters most when you can't see the corridor |
| **3D action / arena / fighting** (SF6, Xenoverse 2, Megabonk) | **Wall sonification → corner / ring-out proximity** + opponent-as-beacon; skip the polar scan | "Navigation" becomes *spacing*: distance to edge + to opponent |
| **Fast platforming / roguelite** (Megabonk) | Wall sonification + **collision thump** + **vertical-space / drop cues** | Movement is quick and 3D; you need drop/gap warnings, not a slow scan |
| **Minimal / proof-of-concept** | **Simple beacon only** (bearing + distance to target) | Cheapest high-value cue; validates the speech+audio pipeline first |

> Rule of thumb: **start with the cheapest cue that proves the pipeline** (a simple beacon or the wall
> bed), then add styles. Wall sonification is usually the highest value-per-effort backbone in 3D;
> reactive radar is the standout in tile corridors.

---

## Studies behind the styles

| Study | Folder | Engine / language | How recovered | Teaches |
|-------|--------|-------------------|---------------|---------|
| **A Hero's Call** | [`a-heros-call/`](a-heros-call/) | MonoGame / .NET 4.0 | Full managed decompile | Reactive radar (style 1) + scanning, beacons, zones, spatial audio, speech — the complete suite |
| **Swamp** | [`swamp/`](swamp/navigation-spec.md) | Native VB6 (Ghidra) | Decompile + map format | Polar scan radar (style 2) + collision sounds + zones, from a totally different engine |
| **Wall sonification** | [`wall-sonification/`](wall-sonification/) | Unity Mono **and** IL2CPP (BepInEx) | **Our own mods** (Core Keeper, DRG, Megabonk) | Continuous directional wall tones (style 3) + collision/vertical cues + IL2CPP NAudio synth |

> A Hero's Call is the **primary reference** for the shared subsystems (its code was fully recovered);
> Swamp **corroborates** the model from native VB6; wall-sonification is **battle-tested in our own
> shipped mods** across both Unity backends.

---

## The shared model (every style sits on this — learn it first)

Everything reduces to **a world you can query + raycasting it + turning hits into panned/pitched sound**:

1. **World** = tile grid (2D games) or physics colliders / navmesh (3D games). Walls/obstacles are
   queryable.
2. **Player state** = position + a **heading / camera forward**.
3. **Every cue is a ray query turned into sound**:
   - *Reactive radar* — short panned cues as you move (wall ahead / space opening/closing).
   - *Polar scan* — N rays around you on command; first wall per ray → panned wall/open sound.
   - *Wall sonification* — one ray per direction every tick; proximity → continuous tone volume.
   - *Collision* — step blocked → positioned bump/thump, **no speech**.
   - *Scanning / POI* — on-demand ray → spoken list of what's ahead, with distances.
   - *Zones/regions* — announce on crossing a boundary; swap reverb on acoustic zones.
   - *Beacons* — bearing + distance to a target (optionally A\*-routed).
   - *Spatial audio* — 3D pan/attenuation + occlusion = raycast wall thickness → muffle/mute.
   - *Speech* — one sink, many backends, interrupt on context change.

> **Porting kernel:** expose `PlayerPosition`, `Heading / CameraForward`, and
> `Raycast(origin, dir, maxDist) -> firstBlockingHit` (tile step-scan or `Physics.Raycast`). Everything
> else is engine-free logic copied on top. Quantize to integers only where an algorithm needs it (the
> radar pitch bucket).

---

## Subsystem map — read this doc for that mod section

Build a navigation mod **section by section**; for each section, the authoritative reference:

| Mod subsystem | A Hero's Call doc | Swamp | Wall-sonification | Notes |
|---------------|-------------------|-------|-------------------|-------|
| Coordinate system & angle math | [01](a-heros-call/01-coordinate-system-and-math.md) | §0, §4–5 | §4 | **Read first.** Compass convention, deg↔vector, bearings, dir vectors |
| Grid raycaster / collision primitive | [03](a-heros-call/03-raycasting-and-collision.md) | §0, §1 | §4 | The one primitive everything stands on |
| Movement, turning, cardinal cues | [04](a-heros-call/04-navigation-and-movement.md) | §1, §4 | §6 (collision thump) | Continuous move + slide collision + heading clamp |
| **Reactive radar** (style 1) | [02](a-heros-call/02-reactive-radar.md) | §3 | — | 3-beam open/closed + pitch |
| **Polar scan radar** (style 2) | — | §3 | — | 16-ray sweep, modes select dirs/range |
| **Wall sonification** (style 3) | — | — | [whole doc](wall-sonification/) | Continuous per-direction tones, volume = proximity |
| POI scanning / examine / interact | [05](a-heros-call/05-scanning-pois-and-interactables.md) | — | §6 | Diff-gated auto-scan; 2.5 focus / 5 act |
| Zones, regions & announcements | [06](a-heros-call/06-zones-regions-and-announcements.md) | §2 | §5 (state gating) | Spoken *region* vs acoustic *zone* |
| Beacons & pathfinding | [07](a-heros-call/07-beacons-and-pathfinding.md) | §5 | — | Simple bearing+distance, or A\*-routed |
| Spatial audio / occlusion / synthesis | [08](a-heros-call/08-spatial-audio-fmod.md) | §1, §3 | §3 | Pan + attenuation + occlusion; procedural tone synth (Unity/NAudio) |
| Speech / screen-reader output | [09](a-heros-call/09-speech-output-layer.md) | §2, §4 | — | One sink, many backends; map Tolk→**PRISM** |
| Big picture & frame loop | [00](a-heros-call/00-overview-and-architecture.md) | §6 | §1, §5 | Architecture, per-frame data flow, constants |
| Porting to real target games | [10](a-heros-call/10-porting-guide.md) | §6 | §7 | Concrete SF6 / 7DTD / XV2 / RE7 mappings + adapter surface |

---

## Convention differences to watch (they cause mirrored-direction bugs)

| | A Hero's Call | Swamp | Wall sonification |
|--|---------------|-------|-------------------|
| Heading zero / rotation | **0° = North**, clockwise | **0° = East**, clockwise | n/a (uses dir vectors / camera) |
| Direction frame | player-relative beams | player-relative rays | **world-fixed** (grid) **or camera-relative** (3D) |
| Axes | +X East, **+Y South** (Y-down) | +X East, +Y South (Y-down) | engine-native (Unity Y-up, raycast on XZ plane) |
| Radar shape | 3 beams (L/F/R), open/closed + 3-pitch | 16 rays, 22.5° apart, first hit | 4 channels (F/B/L/R), continuous volume |
| Cue timing | event on transition | on command | continuous, every tick |
| Spoken distance | rounded tiles | **feet = tiles × 24** | n/a (non-verbal bed) |

> Pin your convention **once at the engine-adapter boundary** and verify with a worked example
> ("facing East, my left points North"; "wall on my right pans right") before trusting any output.

---

## Recommended build order (any target game)

1. **Adapter + Speech** — get `Speak("test", interrupt)` through PRISM and read player position/heading
   on a key. Validates the whole pipeline.
2. **Simple beacon / bearing** — cheapest high-value cue (`atan2` + 8-bin + distance).
3. **Wall sonification** (3D) or **reactive radar** (tile corridors) — the navigation backbone. Wire
   `Raycast` + a panned/continuous-tone primitive; tune range, base volume, and per-direction
   frequencies to the game's scale.
4. **POI scan** (exploration) or **spacing/corner radar** (action), plus **collision / drop cues** if 3D.
5. **Occlusion**, **zone reverb**, **polar scan** last (polish / detail-on-demand).

See [`a-heros-call/10-porting-guide.md`](a-heros-call/10-porting-guide.md) and
[`wall-sonification/README.md`](wall-sonification/README.md) for per-engine mappings (RE Engine, Unity
Mono/IL2CPP, native/XV Patcher) and the adapter surface each style needs.
