# Far map markers and DLC sites in the radar

Player report 2026-11-19 (tester save, Trunks DLC and 23rd World Tournament done). Coded from
the source and headers, **unplayed**.

## "Tracking prey, tracking prey, no items left in this category"

Same words for far enemies in the 23rd World Tournament. Both are hand-picked targets from a far
**map marker** (hunting area EMapIcon 7, enemy group 50 / 52), and both chain to the next and then
to "no items left" without the player moving.

Cause, from the code: a picked target in a chainable group advances when its actor is invalid,
hidden, Taken or unpositionable (`step`, `chain_step`). Map markers are listed WITHOUT a `bHidden`
filter, and a far marker can sit on a hidden actor (a spawner; the villainous group already
skipped `bHidden` for that reason). So the first tick read `bHidden` as "collected".

Fix: `set_manual_target` records `hidden0` (hidden when picked); for such a target `bHidden` is
never an advance signal. Arrival and walking away still advance it. Each advance now logs
`nav sweep advance (<reason>) grp=… hidden0=…`; if the next log says `invalid` or `no-position`
instead, the cause is elsewhere.

## DLC sites said only "site", and only up close

- EMapIcon types in the `sites` group with no noun (AT_enums.hpp:6746): 39 GroupBattlePractice,
  44 SaiyanHangout, 48 ClearedPractice, 57 ReturnToMain, 67 ToFloatingIsland. Now named. 48 is
  called "tough training ground" (the player's words) — an inference from the DLC
  `ETrainingMenuType` "ClearTraining" modes, not confirmed.
- A training point is named by its `OpenMode` (`ATrainingPointActor` @0x7B0,
  `Nav.TRAINING_MODE_NOUN`): group, tough (DLC4–8 ClearTraining) or extra (MainStoryEx).
- A site icon the radar cannot name tries the actor's travel-window title: `ALevelNavigator`
  (AT.hpp:15815) `SelectableWindowTitleId` / `NotifyMessageId` through `GetMessageFromID`, at most
  40 characters. The DLC story-replay point is a navigator (`DLC/09/.../Navigator_StoryReplay`).
- Unrecognised action points kept the 300 m character cap. The whole `sites` group is now
  uncapped: level facilities, never the parked character pool.

Not known: which icon type or class the tester's "site" entries really were. A sweep with
`RADAR_DEBUG` on, or one `kak_dev navdump` near one, names the class.
