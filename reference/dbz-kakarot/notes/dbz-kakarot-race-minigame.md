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
