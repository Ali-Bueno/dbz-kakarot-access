# Race minigame ("driving") — how it works and how the mod guides it

Player report (2026-10-17): driving is not accessible. A player far into the game sent their
save. The design below came from the CXX header dump, the pak index and the mod's own code, and was
**verified live on 2026-10-18** in the story's driving-licence race. The results are at the end.

## What the game calls it

The game never says "drive". The minigame is the **race** (`EMiniGameType::MINIGAME_TYPE_RACE = 2`,
`AT_enums.hpp:6874`). The other minigame types are fishing, baseball and the training room. A
receptionist NPC starts a race (`Racegame_Receptionist01_BP`). Each course is a row of
`MinigameRaceCourseParam` (`FMinigameRaceCourseParam`, `AT.hpp:7895`) with these fields:

| Field | Meaning |
|---|---|
| `cAreaName` | Area the course is in |
| `cRaceType` | Vehicle (see below) |
| `cCheckPoint` | Number of checkpoints |
| `cCheckPointTime` | Checkpoint times |
| `cRewardGroup` | Rewards |
| `cCost` | Entry cost |

There are three vehicles (`MiniGameRaceMode`, `AT_enums.hpp:11536`): the walking robot
(`ATwinFoot`), the **air car** (`AAirCar < ARideCharacterBase`, `AT.hpp:14846`) and flying
(levitation technique). There are also four clear types (`MiniGameRaceClearType`): rank, clear time,
**checkpoint time** and play count.

**What players call "rings" are checkpoints.** A checkpoint is a light tower (`ACheckPointActor`,
`AT.hpp:15208`). Each one carries its own `RaceGameLimitTime` @0x340, so time is probably added per
checkpoint (unverified). You pass a checkpoint by overlapping its sphere (`URaceCheckPoint` <
`USphereComponent`). The course owner is `ACheckPointManageActor`. Its `m_CheckPointActorList`
@0x338 is an ordered `TArray<ACheckPointActor*>`. `URaceGameManager` (`AT.hpp:43253`) owns the race.
Its current index, timer and state are not reflected (native tail), so we do not read them.

## What the mod reads

The race HUD already does all the work. `UAT_UIMiniGameRace` (`AT.hpp:34711`) is fed by native
code through `SetNextCheckPointLocation`, `SetPlayerLocation` and `SetCheckPoint(Now, Max)`. It also
exposes reflected getters, **`GetNextCheckPointLocation()`** and **`GetPlayerLocation()`**, which the
on-screen direction arrow (`WL_Enc_Direction`) uses.

So the guide asks the HUD for those two values and never needs the checkpoint index. The
checkpoint actors stay untouched, so there are no live actor handles to go stale.

| Read | Source |
|---|---|
| HUD instance | `AAT_GameHUD.UIMiniGameManager` @0x580 (`AT.hpp:14683`) → `UAT_UIMiniGameManager.UIMiniGameRace` @0x40 (`AT.hpp:34691`). Directory-mapped, read by `Core.peek_all`, never scanned |
| Next checkpoint / racer position | the two HUD getters above; the racer falls back to `HUD.PlayerOwner.Pawn` if the HUD answers zero |
| Heading | `HUD.PlayerOwner` (`AHUD` @0x338) → `Pawn:K2_GetActorRotation()`. The pawn is the rider, attached to the vehicle. The camera manager is only the fallback: its rotation stays frozen for the whole race |
| Checkpoint counter | `WL_Txt_Num_Cp` @0x3C0 |
| Timers | `WL_Mgame_Race_List00` = "Tiempo" (elapsed) and `WL_Mgame_Race_List01` = "Límite de tiempo" (`UAT_UIMiniGameRaceList`): `WL_Txt_Title` + `WL_Txt_Num_Time`. The text shape is `m'ss.mmm` (`0'42.803`) |

Other surfaces, not read yet:

- **Countdown cut-in** `UAT_UIMiniGameCutinRace` (`AT.hpp:34627`, manager @0x48): 3/2/1/Go/Goal/Time
  up/Failed are all images. The game voices them, so they were left for now.
- **Race tips** `WL_Mgame_Tips`. The two-page tutorial before a race is `Tips_C`, already read by
  `screen_tips`.
- **Result sheet**: it should be the shared `Mgame_Result_C`, which `screen_fishresult` reads once
  `_G.__KakarotMinigameLive` is stamped. The race guide stamps it.

## Design (2026-10-17)

- `race_guide.lua` runs on the shared 100 ms tick bus, outside the screen registry. A race is
  gameplay, and an adapter holding the tick would shadow the pause menu and the dialogs.
  - It probes every ~0.5 s while idle.
  - The race ends only after the HUD has been gone 1.5 s (flicker debounce, wall time).
  - It goes silent under any screen that mutes audio (`Registry.mutes_audio()`).
- **The beacon is the quest radar's own.** `beacon.lua` holds the pan, behind-pitch and
  cadence/volume shape that `nav_tracker` used inline, and both now call it. It is aimed at the
  next checkpoint, using 3D distance for the cadence.
- **The quest radar yields** while a race is up: `Race.owns_audio()` joins nav's `ui_muted()`. The
  claim expires on its own if the loop stalls, so it can never mute the radar forever.
- **Speech**:
  - "Punto de control N de M" when the counter changes. The opening one is spoken without
    interrupting.
  - **F10 / L3+Y** during a race reads the checkpoint, the distance to it and the visible timers,
    each under the game's own label. Timers are spoken as "42 segundos", not "0'42.803".
  - **Low-time warning:** when the limit drops to 10 s it is spoken once ("9 segundos"). A
    checkpoint that tops the limit back up re-arms it. At 0 the game restarts the race without a
    word.

## Live results (2026-10-18)

**How to reach a race.** The first race is in the story, during the three-year wait for the
androids. Goku and Piccolo take the **driving-licence exam**, a hover-car time trial: 10
checkpoints, a time limit, and optional boost rings. It comes right after Vegeta's gravity-chamber
fight against his own simulation.

The receptionist races ("Oficial de contrarreloj", `Racegame_Receptionist01_BP`, in Area03 town
031, plus areas 01, 04, 06, 07, 09 and 10) need a car, and she says so. Bulma builds the car
through **I+D**, which is unlocked after the licence, from gathered materials.

The story offers **no free roam** between Vegeta's fight and the race, so no manual save can sit
right before it. Story scenes do not autosave either; only travel does.

| Question | Answer |
|---|---|
| HUD pointer set while racing? | **Yes.** `race -> live` fires as the race tutorial opens |
| `GetPlayerLocation` follows the car? | **Yes**, every read (`pos=… (hud)`) |
| `race -> over` at the end? | **Yes**, ~5 s after `10/10`; the story continues and the radar resumes |
| Counter text | `0/10` → spoken "Punto de control 0 de 10" |
| Minimap during the race | **Up** (`minimap up=true`), so it cannot serve as a liveness signal; nav must be muted explicitly, as designed |
| Checkpoint heights | Checkpoint Z sits up to ~45 m above the racer at range and converges near the gate. Ground-plane pan was enough to finish the course |
| Camera vs. heading | **Camera manager frozen** (yaw −57/−180 the whole race). Switched to the rider pawn's yaw |

**End-to-end proof.** A dev driver (`drive` on the dev channel) steered with nothing but the same
`Beacon.cam_rel` pan the guide plays, plus RT. It completed all 10 checkpoints and the licence. So
the beacon's information alone is enough to drive the course.

Not yet covered:

- **The receptionist races and their result sheet**: no car on the test save.
- **The cut-in images**: 3-2-1, Go and Goal are voiced by the game.

## Rings, obstacles and loudness (2026-10-18, second pass)

Player feedback after driving it: "it works", but asked for sounds for the rings and the
obstacles, and a louder beacon in the race.

| Cue | Source | Sound |
|---|---|---|
| Checkpoint | as above | `beacon.wav`, now at 1.8x (`BEACON_GAIN`) |
| Boost ring | `race_rings.lua`: `ABoostPointActor` (`AT.hpp:15145`, placed as `BP_BoostPoint`). Positions are scanned once per race, retried every 3 s up to 10 times until the course loads, and kept as numbers. The nearest ring within 60 m and 60 degrees of the heading is cued | `item` cue, 1.2x; quiet while an obstacle is within 15 m |
| Obstacle | `obstacle_sensor.lua`: three Visibility traces (0 and ±25 degrees) from 3 m ahead of the racer, reach = speed × 1.6 s clamped to 15–45 m. A hit counts only if `ImpactNormal.Z` ≤ 0.7, so ground and slopes are ignored | `enemy` cue from the hit's direction, 0.7x, faster as it nears |

**The beacon has priority.** The player asked for cues that may overlap a little but never mask
the checkpoint beacon. So ring and obstacle cues play only in the gaps between pings: at least
200 ms after a ping (or half the period when pings are close), at least 100 ms before the next,
and two secondary cues at least 200 ms apart. One cue per gap; when both are due, the one that
has waited longer goes first. Within ~5 m of a checkpoint the beacon pings every tick and plays
alone. The secondary gains keep both cues at or under the beacon's peak. This is covered offline
(`test_race_guide.lua`, spacing contract) and not yet heard live.

- **Loudness:** `audio_bridge` now accepts cue volumes up to 4 (XAudio2 amplifies above 1).
  The WAVs have the headroom: `beacon.wav` peaks at −15 dBFS, `item.wav` at −12 and `enemy.wav`
  at −7.
- **The trace works.** `KismetSystemLibrary.LineTraceSingle` (the channel overload) returned
  live on 2026-10-18 with OutHit filled in as a table: `Distance`, `ImpactNormal`,
  `ImpactPoint` and the rest. A downward test hit the floor at 1.7 m with normal (0, 0, 1). The
  July "dead" verdict ([engine facts](dbz-kakarot-engine-facts.md)) predates the arity fix and
  was mostly about the ForObjects overload, which stays unused. The sensor still keeps two
  fuses: any caught error turns it off for the session, and the first trace of every race runs
  with `ray_fuse.txt` set to `testing`. A process that dies inside it leaves `testing`, and the
  next session never traces (delete the file to re-arm).

**Live run (2026-10-18, licence race again, dev driver):**
- `race rings -> 14 (try 1)`: the native class name answered on the first scan as the course
  started.
- Ring cues started at ~59 m as designed.
- The fuse file read `ok` after the first trace. Traces ran at 30 a second for the whole
  3 min 12 s course with no crash, and the race finished 10/10.
- About 50 obstacle episodes. Most were 25–45 m at pan 0 or ±0.42. A few first appeared at
  3–5 m, one of them with speed falling from 91 to 42 m/s, i.e. a real collision the rays had
  not seen coming.
- Reaction: an obstacle now needs **two scans in a row** (`OBSTACLE_CONFIRM`) to cue, to drop
  one-tick terrain blips. Each episode's end is logged with its length and nearest distance
  (`race obstacle -> clear after …`), for tuning by ear.
- Still open: whether the 0.5 m ray height (the racer's origin is ~0.8 m above the checkpoint
  ground) catches low rocks, and whether the obstacle cue is too busy in canyons.

### Dev-channel test tools added for this

All are compiled out of releases, like the rest of `dev_channel.lua`.

| Command | What it does |
|---|---|
| `where` | F5 read, echoed |
| `keys` | F2 read, echoed |
| `objective` | F10 read, echoed |
| `tp <Class> [name]` / `tp @<radar group>` | Teleport in front of an actor, facing it |
| `enemyhp [v] [name]` | Read or set enemy HP at `StatusInstance+0x394`, to get a test save past a story fight |
| `trigger <lt> <rt> [ms]` | Virtual triggers; a new lease in `input_bridge` |
| `drive [ms]` | The beacon-steered test driver |
| `ray [m] [ch] [dz_m]` | One LineTraceSingle ahead of the pawn; prints every OutHit field. A breadcrumb file (`kakarot_ray_crumb.txt`) is written before and after the call |

Dead end: `UATCheatManager::ATMiniGameTestStart(2)` (and any type) does nothing in the shipping
build.

## Open questions from before the live session (answered above)

1. Does `UIMiniGameManager.UIMiniGameRace` get set while racing? If not, the guide is silent
   (the fishing ring-core shape), and the log never prints `race -> live`.
2. Does the HUD's `GetPlayerLocation` follow the car every frame? The `race checkpoint ->` log
   line prints `pos=… (hud|pawn)` for each checkpoint.
3. Does `race -> over` print when the race ends? If the pooled HUD lingers on screen, the quest
   radar stays muted after the race.
4. What do the counter text (`'1/8'`?) and the timer texts look like spoken?
5. Is the minimap up during a race? It is logged on `race -> live`. If it is hidden, it could
   become the liveness cross-check for question 3.
6. Do checkpoints sit at different heights (air car, flight)? If so, the beacon needs an
   elevation cue; today it is ground-plane only, like the quest radar.
7. Camera vs. car heading. During the countdown the camera cuts around the car, so pans are
   meaningless for those ~3 s. If chase-cam lag hurts while steering, switch the guide to the pawn's
   yaw.

## Walking robot race (player report 2026-11-19, unplayed)

"The radar beeps and ring sounds play during the robot race, but following them doesn't do
anything." Static findings: the robot race is the SAME minigame (`URaceGameManager`, mode
`MiniGameRaceMode::BIPEDWALKINGROBOT` = 0, AT_enums.hpp:11536), same `BP_CheckPoint_C` and HUD
getters, on its own maps (`Maps/AreaNN/MiniGame/AreaNN_RaceGame_TwinFoot.umap`, 6-7 checkpoints).
The robot is `ATwinFoot` (`TwinFootBP_C`, AT.hpp:18402) with its own `ATwinFootController`.

So the checkpoint position is right and the HEADING is the suspect: the guide panned against
`HUD.PlayerOwner.Pawn`'s yaw, and that pawn has been seen as a TwinFoot parked at the origin
(dbz-kakarot-crash-bug.md). A frozen yaw makes the pan meaningless while the distance cadence still
sounds plausible. Fix: the pawn's yaw is used only while the pawn is within 10 m of the racer's HUD
position; otherwise the direction of travel (re-measured every metre), then the camera. The log
says `race heading -> pawn|movement|camera (pawn <class>, N m from racer)` on every change — the
first robot race's line settles the hypothesis. If it says `pawn` and steering still fails, the
robot's actor yaw is not its travel direction: try the movement heading for it too.

## Boosters on the way (player report 2026-11-19, unplayed)

"I can't reach the boosters to go faster to reach the checkpoints." The robot race uses the same
boost actor as the car race (`BP_BoostPoint_C` < `ABoostPointActor`, both names in the TwinFoot
umap's name table; trigger `UBoostPointComponent : UBoxComponent` overlap, no button), so
`race_rings` already found them. The cue was the problem: a quieter sound in the gaps of a beacon
that kept pointing at the checkpoint.

Now `Rings.on_route` picks the nearest ring ahead along the racer→checkpoint line, nearer than the
checkpoint and within 20 m of the line (30 m once picked); the **beacon itself** steers to it with
the ring sound and says "Propulsor" once, then returns to the checkpoint when the ring is behind.
The line is measured from positions only, so it does not depend on the robot's heading. Log:
`race booster -> N m (ring i of n)`. Tuning: `Rings.ON_ROUTE`. Not known: whether the boost box is
sized for the robot, and the boost's effect (no reflected speed state).

## Third report, 2026-11-19: only the first checkpoint

"I get through the first checkpoint, then struggle to find another booster and reach the
checkpoint before the boost runs out." No race was played in the attached logs, so this is from
the code: `on_route` only looked 60 m out and 20 m either side of the straight racer→checkpoint
line. Boosters along a curving course, or the next one further than 60 m, were never offered.

Now a ring qualifies when it is ahead along the line and short of the checkpoint, nearer the
checkpoint than the racer, within 150 m (`Rings.REACH`), and its detour
`d(racer, ring) + d(ring, checkpoint) - d(racer, checkpoint)` is at most 30 m or 25 % of the way
(`DETOUR`, `DETOUR_FRAC`; the locked ring 1.5x). A ring the racer came within 4 m of is marked
used for the race. When no ring qualifies the log says
`race booster -> none on the way (nearest unused N m)` — with `race checkpoint` lines that is
enough to tune the budget from one player log.

## Live session 2026-11-19 (night): Area03 walking-robot race, tester save

Driven with a dev autopilot (`autodrive [rt|stick|both|off]`, dev channel) that steers every guide
tick by the pan the guide itself plays, so it tests exactly what the player hears.

**Verified:**
- **The receptionist's menu was silent** (`Mgame_Top_C` < `UAT_UIMiniGameTop`, owner
  `fm.MiniGameTop` @0x618, AT.hpp:32895). Now read by `screen_shoplist`: rows "Conducir
  aerodeslizador" / "Conducir Caminante robot bípedo", each with "Jugar Contrarreloj / Ver récords /
  Ver recompensas", from the reflected `GetSelectItem()` (UI_MINIGAME_TOP_ESELECT_ID 7/8 rows,
  9..14 sub-options), plus best rank, best time and the 500 zeni fee. Log `minigame top: select=…`.
- **Robot controls:** left stick forward walks along the robot's own yaw; stick sideways TURNS it,
  slowly (~17°/s). The player pawn is Goku, left standing at the start (0 m from the racer at the
  start, 160 m once the robot walks off), so the guide now takes the heading from the robot actor
  (`TwinFootBP_C`, one per level) — log `race heading -> robot`.
- **Course geometry** (`course` dev dump → `Scripts/dumps/dump_race_course.txt`): 6 checkpoints
  (the first is the start line), 10 boost points, 52 arrow meshes in 23 groups, an
  `UnderWaterRestartVolume`. Big height changes: start 83 m, checkpoint 1 at 17 m, checkpoint 2 at
  87 m; boosters at 51 m and 66 m between them. A boost zone measures 24 x 24 x 2 m.
- **"Te caíste al agua. La carrera se reiniciará."** is a box that waits for A (the timer stops
  until then). It is spoken by `screen_dialog`.

**What the runs showed:**
1. Straight to checkpoint 1 from the start: into the water at the cliff, three times out of three.
2. After a water restart (facing another way) checkpoint 1 was reached; the guide then picked the
   booster 149 m ahead (the longer reach works) and DROPPED it when the robot drew level with
   checkpoint 2 underneath it, 15 m beside the booster. Time-out — the player's report exactly.
   Now the booster is held while within `Rings.HOLD` (40 m) until the robot is inside its zone
   (radius read from the bounds). With the hold, the robot then spent 66 s near that booster without
   entering it: it is probably on a ledge reached by another way (arrows 38-40 climb 40-44 m towards
   the 66 m booster).
3. With arrow waypoints (`race_route.lua`) the start leg follows the arrows down the slope
   (84 m → 12 m) but the robot still fell into the water ~36 m short of checkpoint 1.

**Not solved:** this course is a narrow path over water with ledges. A straight beacon (to a
checkpoint, booster or arrow group) is not enough for a slow-turning walker; it needs the actual
path. Next idea: record a path by driving the course (positions every few metres) and ship it per
course, or find the game's own course spline if one exists. The tester reaches checkpoint 1 on
their own, so a human copes better than the autopilot on the first leg.

## Live session 2026-11-20 (early): what the robot course really is

**NavMesh paths: dead end here too.** `FindPathToLocationSynchronously` from the robot, with both
ends projected onto each of the three meshes and the robot registered as a 150 m invoker, returned a
valid but EMPTY path (0 points), even for a 30 m hop. Same as engine-facts' Area04 result. The dev
`racepath` command stays for the record.

**Terrain survey works and is cheap.** `survey x0 y0 x1 y1 step name [channel]` (dev channel) casts
one downward Visibility trace per cell: ~0.07 ms per trace, 10,000 traces in 0.7 s. The whole Area03
course box (-2300..-1502 m, 1300..2298 m) at 2 m is in `Scripts/dumps/survey_a03twin.txt`
("x y z normalZ 0", cm), taken in free roam with Goku teleported to the centre of each 200 m block
so collision was loaded. Heights match the checkpoints (start 83.6 m).

**Water is invisible to traces.** Visibility and Camera channels both hit the lake BED (6.6 m at
-1736,1480); no channel sees a surface. In free roam Goku stands on that bed: the water exists only
in the race, as `UnderWaterRestartVolume_TwinFoot` (AT.hpp:18426, overlap-only, `RestartPointActor`
@0x370). Its AABB is origin (-1813, 869, 18 m), extent (745, 662, 30 m), so tops out at 48 m,
which cannot be its real top (the robot walked at 16-22 m inside that box). Probably a tilted or
non-box brush. **`AVolume.EncompassesPoint` aborts through pcall** (float& out-param) and killed the
mod's loop; never call it. The `vol` dev command (rotation + scale + bounds) was written to solve
the box but not run yet. Empirical: the robot restarted at (-1717,1480) z 12 (ground 10.6) and was
dry at (-1706,1443) z 16, so the lake surface is about 13-15 m.

**BOOSTERS ARE LAUNCH PADS, not speed rings.** They float above the ground (booster 17 at 51 m over
40 m ground, 19 at 66 m, 20 at 53 m over 41 m). Checkpoint 2 sits on a rock pillar with sheer 40 m
walls (41 m → 79 m in one 2 m cell) and no ramp. Test: robot teleported under booster 17
(-1858,1530, 42 m, yaw 90), stick forward + A (jump): it reached 53 m at 0.9 s, was launched to
108 m at 2.5 s, turned by itself from yaw 90 to 130 in the air, and landed at (-1872,1567) 88 m,
on checkpoint 2. **The robot jumps with A** (`ACharacter`, `Jump()` reflected). Not yet known:
whether walking under a booster without jumping also launches it.

**The arrows' yaw is their pointing direction** (each group points at the next one) and the numbers
roughly follow the course. Leg 2's designed route: CP1 → arrows 27-29 (yaw 152) → 30-32 (172) →
33-34 (-108) → 36-37/12 (122) → 38-40 (117, under boosters 19/17) → launch → 92-93 on the pillar
(CP2). Leg 1: arrows 25-26 sit on the start cliff edge (z 80-83 over 44-57 m ground: walk off it).

**Next:** (1) run `vol` in a race and solve the volume's true box; (2) test booster entry without A;
(3) guide = ground path over the survey grid (A*: no water, no steep climbs, drops allowed) to
the point under the next booster, then say "jump"; ship the survey-derived path data per course.
Renders and the loader are throwaway scripts (scratchpad), not in the repo.

## Live session 2026-11-20: recorded routes and the first finished race

Done as planned above. The planner is now in the repo (`tools/race-paths/`, its README has the
workflow); the guide follows `race_courses.lua` through `race_path.lua` (pure pursuit, 15 m
lookahead) and only falls back to the checkpoint beacon where no route is shipped.

**Facts established live (autopilot `autodrive stick`, Area03 robot race):**
- **Water** is real only as `ATWaterVolume` hulls; their geometry is
  `BrushComponent.BrushBodySetup.AggGeom.ConvexElems[i].VertexData` (local space: rotate by the
  actor yaw, add the location). Use one hull per convex element; a merged hull swallowed dry land.
  The dev `volgeom` command dumps them.
- **Survey during the race**, not in free roam: the race map adds the rock walls the robot got
  stuck on. Collision is loaded ~1 km around the racer, so no teleport is needed.
- **Launch pads need a STANDING jump.** A running jump into booster 17 flies ~80 m and overshoots
  the CP2 pillar; stopping under it (speed under 3 m/s) and then jumping lands on the pillar every
  time. The guide says "Para" in the launch zone (6 m) and "Salta" once the racer has stopped.
  Verified: CP1 → CP2 completed three times by the autopilot on these cues alone.
- **A jump lifts the robot 15 m** (`rtrack`, standing and running: peak ~0.9 s after take-off,
  ~0.5 s wind-up after A, ~20 m forward on the way up). Against a wall it only gains 5-6 m, which
  is what the first estimate was based on. Movement limits (`rmove`): capsule r 1 m / half-height
  1.3 m, MaxStepHeight 0.45 m, WalkableFloorAngle 45 degrees; but robot + rider catch on ceilings
  ~5 m up, so plan with 7 m of clearance.
- **Camera and ControlRotation are frozen in the race**; the stick is camera-relative and the
  follow camera settles behind the robot in ~1 s. The beacon pans on the robot BODY yaw.
- **Test harness quirks:** the game ignores pad input when its window is not focused; after a mod
  reload the virtual pad needs `unplug`, `stick 0 0 100`, `press A` (sometimes twice); the
  receptionist talks only when Goku walks INTO her zone facing her (`walkto` twice, then B).
  `robot x y z yaw` teleports the racer and the checkpoint counts, so a leg can be tested alone.

**Why one top-down survey was not enough (leg 3, five failed routes).** The race level
(`/Game/Maps/Area03/MiniGame/Area03_RaceGame_TwinFoot`, dev `racelevel`) places dinosaur bones
(`S_Fossli_DinoBoneParts*`, 50-60 m across) right on the course and the "plateau" north of CP2
is the deck of a rock arch (`S_Area03_Cliff_ArchE10`). A downward trace from the sky reads the
TOP of whatever is highest, so bone tops and decks looked like ground and the ground under them
was invisible. No trace channel sees anything the Visibility channel misses (channels 0-5
compared over the CP3 bone). The course has no route spline; the arrows (92/93 on CP2 pointing
north, 43/44 on the arch, 45/46 at CP3) confirm the designed line runs over the arch.

**Multi-layer survey + layered planner.** `msurvey` traces down from the sky, then again from
0.5 m under each surface it meets (a trace starting inside a solid passes its back faces), up to
3 surfaces per cell, and measures each lower surface's headroom with an upward trace: the whole
course box at 2 m is ~300 k cells, 435 k surfaces, ~70 s, taken in a paused race.
`tools/race-paths/mplan.py` searches (cell, surface) nodes with the robot's real rules (45
degree climbs, 7 m clearance, drops up to 30 m, water hulls forbidden, launches from floating
boosters, jumps simulated on the measured arc and only where walking on is impossible, a cost
for running near an edge). `routecheck.py` flags climbs over 45 degrees along a route: it
predicted every place the robot got stuck live. Trace normals on rough rock are noise; walkability
is judged by the slope between cells instead.

**Recovery is what makes the race finishable.** Even the best route leaves small snags (a rib, a
pit, a lip) that no survey models exactly, and each fix exposed the next. So the guide does what
the game itself does when the racer falls in the water: after one failed "Salta" (stuck ~4 s) or
8 s without 10 m of progress along the route, it moves the robot 35 m on along the route (onto
the checkpoint itself when that is closer: a point a few metres short of it did not count it),
facing the route, and says "De vuelta en la ruta" (log `race recover (why) -> x,y,z`). It never
skips a booster launch that has not happened. With it the autopilot FINISHED the race twice in a
row (5/5, "¡Increíble!", 2'50 and 3'20), with 6-9 recoveries per run.

**Shipped routes** (`legs_ship.json`): L1-L2 from the first planner (verified), L3 the hand
route along the arch (`legs.py`, ridge, two crack jumps), L4-L5 from `mplan.py`.

**Boosters with a route:** the route beacon steers and the booster on the way keeps its own cue
(`item`, from its own direction, between the beacon's pings). Until 2026-11-20 a route silenced
it, and the player heard the two take turns.

**Result sheet:** the race fills the shared `Mgame_Result_C` (`AT_UIMiniGameResult`): "Récord
actual" + time, "Récord personal" + time, skills learned. `screen_fishresult` reads them; "new
record" is derived from run time == best time, because `IconNewRecord` stays on screen on a
slower run. The headline is an image that flips `Lang_Result01` -> `Lang_Result00` on a won
race (logged, not spoken). The receptionist's "Grado actual" box showed "No hay récord" after
the win: that is the game's own text (screenshot), probably the next grade's empty records.

**First player run (same day): finished**, 2 boosters taken. The no-progress recovery fired three
times while the player was stopping under booster 17 as told, putting them back on the same spot:
near a pending launch it now waits 25 s (`PROG_LAUNCH_S`).

**Boosters are launch CHAINS** (height over the ground from the msurvey): 17 (10 m), 20 (11 m),
10 (16 m) and 16 (6 m) are pads a standing jump reaches; 19 (26 m), 15 (34 m, right above 10),
13 (25 m) and 7/8/9 (27-44 m) are mid-air second stages a launch carries the robot through
(17 -> 19 -> CP2 is the one the route uses). The routes of legs 3-5 take none of them, so the
race is finishable but slower than designed; 10 -> 15 sits beside leg 4.

**Measured launches (same day):** from a standstill a pad launch is purely VERTICAL (pad 10:
52 -> 124 m, falls onto 15 above it -> 145 m, x/y unchanged). Taken at a run it carries the
robot forward (booster 17: ~80 m). Boosters 7/8/9 lie on the line pad 10 -> CP4, so the designed
leg-4 shortcut is a RUNNING jump into 10 heading for CP4. A hand-steered run-up missed the pad by
10 m (camera-relative stick), so the guide must aim and time it.

**Player request (2026-11-20, IN PROGRESS, unverified live):** routes should catch the boosters,
and only boosters still takeable should sound. Coded, tests pass, not yet run in the game:
- `race_path`: route points flagged `"boost"` = a pad taken at a run; "Salta" fires 12 m before
  it (`Path.BOOST_R`), no stop. No shipped route uses the flag yet.
- `race_rings`: `Rings.track` marks a ring taken when the segment between two ticks crosses its
  zone (teleports over 60 m ignored); taken rings and mid-air stages (lift > 20 m) are never cued
  by `pick`/`on_route`. Logs `race booster -> taken (ring N)`.

**Next:** add `"boost"` support to `make_course_data.py`, build leg 4 as CP3 -> run-up -> pad 10
("boost", heading CP4) -> CP4, run it once with `autodrive stick`, ship if it beats walking; check
the `taken` log in a player run; tune RECOVER_D / PROG_S from feedback; map the result headline.
