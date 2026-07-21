# Dragon Ball Z: Kakarot Accessibility - Changelog

---

## v0.1.1 - July 21, 2026

### Bug Fixes
- Fixed a crash that could close the game during map changes, cutscenes or returning
  to the title screen. Two separate crash reports from the same player turned out to
  be one single bug: while the game frees the old level, the mod could still read a
  user-interface object that no longer existed.
- Fixed the same fault in the toast reader (the "obtained item" and quest-update
  lines). The equivalent bug was fixed in one of its two loops back in v0.1.0, but
  the twin loop was left untouched.
- Removed a diagnostic trace that was accidentally left enabled in v0.1.0. It ran on
  every single frame, in every game state including level loading, and did nothing
  for the player.

### Improvements
- Every reader in the mod now fetches interface fields through a single guarded
  helper, so a screen that disappears mid-read degrades into silence instead of
  risking the game.

### Known Issues
- The root cause is reduced, not eliminated: the mod can still run for a brief moment
  while the game is tearing a level down. Closing that window for good is the next
  planned change.
- If the game does close unexpectedly, please send `UE4SS.log` (it lives next to the
  game executable, in `AT\Binaries\Win64\`) **before relaunching the game** - it is
  overwritten on every launch, and it is what identifies the exact cause.

### Notes
- No gameplay features, keybinds or translations changed in this version. If v0.1.0
  works fine for you, this update only makes it more resistant to crashing.

---

## v0.1.0 - July 17, 2026

First public release.

### Features
- Screen-reader output through PRISM (NVDA, JAWS and SAPI supported).
- Menus read out: title, options, save/load, items, party, characters, shops,
  cooking, skill palette, skill tree, community board and soul emblems.
- Story dialogue, subtitles, notices, tutorials and on-screen prompts.
- Quest objective tracking and an audio radar for quests, points of interest,
  enemies, companions and gathering spots (R3 opens the target picker).
- Fishing minigame cues and result readout.
- In-game configuration menu (L3+R3): audio cues, cue volume, radar auto-tracking
  and language.
- 13 languages, editable by the community in `Scripts/lang/*.txt`.
