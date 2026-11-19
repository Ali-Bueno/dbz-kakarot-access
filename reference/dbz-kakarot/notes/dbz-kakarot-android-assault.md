# Android Assault drones (Trunks DLC)

Player report 2026-11-19: "the only thing not accessible is the android assault mechanic in
Trunks's DLC, where you have to destroy the drones to not raise the alarm". Coded from headers and
pak name tables only, **unplayed** (Ghidra was not needed and not run).

## What the game does

| Fact | Source | Confidence |
|---|---|---|
| Drones are `AAndroidAssaultDroneBase : APawn` (AT.hpp:14907), not `AAT_Character`, so the radar's character scan never saw them | header | high |
| Kinds: `BP_AndroidAssaultDroneSearcher/Commander/Chaser` (`DLC/08/Blueprints/AndroidAssault/Drone/`); enum `EAndroidAssaultDrone` None/Searcher/Supporter/Commander (AT_enums.hpp:1378). The Chaser BP is the native Supporter | pak names, `BP_AndroidAssaultArea.DroneClassTbl` | medium |
| Only the Commander has a map marker: `EMapIcon::DRONE_COMMANDER` (41), on its `MapIconComponent` @0x468 | header | high |
| Searcher and Commander carry `UAndroidAssaultSearchComponent` (AT.hpp:38537: `SearchDistance`, `AlertDistance`, `IsInSearchArea()`, `IsInAlertArea()`); the behaviour tree raises the alert when the player is inside `AlertDistance` | BT asset names | high / medium |
| Destroying a drone (any hit on `DamageCollisionComponent`) lowers the alert gauge (`DefeatDroneSubAlertGauge`) and drops `BP_AndroidAssaultDropItem` (< `PlacementObjectInfo`) rolled from `LotteryDroneItem` — the drone parts | BP superclass, param rows | high |
| The running gauge value is NOT reflected; only its tuning rows in `DT_AndroidAssaultParamTable` | header sweep | high |
| HUD: `fm.AssaultEvent_Gauge` = `UAT_UIAssaultEvent_Gauge` (AT.hpp:30344): `GaugeHeaderText`, `AreaEntryWarningText`, `CaughtWarningText`, `DroneCountText` (→ `UAT_UIAssaultEvent_AlertGauge_AlertNum.DroneCountText`), fill only in `GaugeMaterial` | header | high |

## What the mod does

- **R3 group *Drones*** (`drones`, after the villainous enemies): `FindAllOf("AndroidAssaultDroneBase")`,
  named by Blueprint (search / commander / chaser drone), no distance cap. Scanned only when the
  sweep saw a commander marker (41, now in the group instead of *Personajes*) or the alarm gauge is
  on screen, so the class never joins the scan set elsewhere. Tracking a drone gets the enemy aids:
  "on target" and the in-front / behind side of its facing.
- **Drone parts** land in *Coleccionables* through the existing `PlacementObjectInfo` scan, with
  `cat_drone_part` as the fallback noun.
- **Alarm HUD** (`screen_assault`): speaks the header when the gauge appears, the area-entry and
  caught warnings when they show (caught interrupts) and the drone counter when it changes.
  Passive notice, 3 s absence before a line can repeat.

## Not covered / to check live

- The gauge LEVEL: it lives in native state or the MID's scalar parameter (name unknown). The
  game's own siren / caution sounds already play.
- Whether `DroneCountText` counts drones left, drones alerting, or something else — speak it as
  shown, then look at what it says.
- Checklist: in the Trunks DLC, go back with the time machine to a drone area. R3 → *Drones* must
  list them with kind and distance; destroy one, the next must chain; the parts must show in
  *Coleccionables*; entering the area and getting caught must be spoken.

## Second report, 2026-11-19: "they only appear once they spot me"

The v0.1.15 gate scanned drones only when a commander's marker or the alarm gauge was up, and
the gauge appears only once the alarm is raised. And the drones are not level-placed:
`AAndroidAssaultArea` (AT.hpp:14891) spawns them from `SpawnPattern` / `DroneClassTbl` once the
player is within `SpawnDistanceMin/Max`, and each drone hides itself beyond its own
`VisibleDistance` (`AAndroidAssaultDroneBase` @0x3E4). So nothing is there to list from far away.

Now (unplayed):

- The **area** is listed in *Drones* as "zona de drones", any distance. It has its own
  `MapIconComponent` (@0x358), so the icon scans reach it; it is matched by class name
  (`AndroidAssaultArea`) on any icon type the radar does not already group, because its
  `MapIconType` is not in the Blueprint defaults (`BP_AndroidAssaultArea.uasset` strings, pak).
- An area marker also arms the drone scan, so drones are listed as soon as they spawn, alarm or
  not. Hidden drones are listed too (beyond `VisibleDistance`).
- To check: does the area show from afar, and is its type in the log's sweep diagnostics; do
  drones appear before the alarm.
