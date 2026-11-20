# Race routes from a terrain survey

Offline tools that turn a terrain survey of a race course into the walking routes the mod ships in
`mod/KakarotAccess/Scripts/race_courses.lua` (read by `race_path.lua`). Findings, limits and the
live results are in `reference/dbz-kakarot/notes/dbz-kakarot-race-minigame.md`.

## 1. Survey the course in the game (dev build)

Take it **during a race**, from the pause menu: the race map adds rock walls and ridges the open
world does not have. Collision was loaded 1 km away from the racer, so no teleporting is needed.

- `course` (start of a race): checkpoints, boosters, arrows → `dumps/dump_race_course.txt`.
- `volgeom ATWaterVolume Area03` (any time in the area): water volumes → `dumps/volumes_ATWaterVolume.txt`.
- `survey x0 y0 x1 y1 2 race03` in 200 m blocks (metres): one downward Visibility trace per 2 m,
  ~0.7 s per block → `dumps/survey_race03.txt`. The Area03 course used x −2500..−1502,
  y 1300..2498 (30 blocks).

The dumps folder is gitignored; `load.py` reads it.

- `msurvey x0 y0 x1 y1 2 race03 3` (same 200 m blocks): every surface per cell (arch decks, ground
  under bones) with its headroom -> `dumps/msurvey_race03.txt`. This is what `mplan.py` reads.
- `racelevel` lists the race level's own actors and meshes (bones, rock arches, cameras).

## 2. Plan and generate

```
python mplan.py [legs]              # layered planner over the msurvey: writes mlegs.json
python routecheck.py L3             # flags climbs over 45 degrees along a leg of legs.json
python legs.py                      # first planner (top-down survey) + hand-placed legs: legs.json
python make_course_data.py legs_ship.json   # writes race_courses.lua from the legs to ship
python zoom.py x0 y0 x1 y1 out.png  # close-up height map (metres), steps in red
```

`mplan.py` holds the measured robot model (45 degree climbs, 7 m clearance, a 15 m jump arc
simulated at 8-12 m/s, launches from floating boosters, water hulls forbidden). `legs.py` keeps
the first planner and the hand route of leg 3 along the rock arch. `legs_ship.json` picks the
legs to ship (currently L1-L3 from `legs.json`, L4-L5 from `mlegs.json`).

Routes are never perfect on this course (bones, cracks, lips): the guide's recovery moves a stuck
racer on along the route. Verify a change with one full `autodrive stick` race from the start.
