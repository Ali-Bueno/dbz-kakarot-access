# Dragon Ball Z: Kakarot Accessibility - Changelog

---

## v0.1.2 - July 25, 2026

### Bug Fixes
- **Remapped controller buttons are now announced correctly.** If you changed your button
  layout in Options, the mod kept naming the buttons the game shipped with - melee moved
  from B to X was still read as "B". This affected far more than the settings screen:
  every button prompt in the game, the on-screen action bar and every tutorial line was
  telling you the wrong button. The mod now reads your actual layout from your save.
- **Messages are no longer cut off mid-sentence.** When a notice appeared - a new area
  unlocked on the map, a skill acquired, the popup explaining what a skill needs - the
  screen underneath announced itself again a moment later and silenced the message before
  it finished. Notices are now allowed to finish; whatever was going to be read waits its
  turn instead of interrupting.
- **A dialog you trigger again is read again.** The same message shown a second time - by
  picking the same option, or a different option with the same text - stayed silent,
  because the mod remembered having said those words. It now only remembers for as long
  as that window is on screen: closing it and opening it again is a new message.

### Improvements
- **Keyboard players can use the radar target picker.** It used to be reachable only by
  clicking the right stick. **V** opens and closes it, the **up and down arrows** move
  through targets, **left and right** change category, and **Enter** locks onto the
  focused one. While it is open the game no longer receives your keystrokes at all -
  exactly what the mod already did with the controller - so plain keys are safe here even
  though the game itself uses them (V is the skill palette, the arrows are
  mount/dismount).
- **The world map's travel list responds to the arrow keys**, exactly as it already did to
  the d-pad: up and down move between travel points, **Enter** travels to the selected one.
- **The button-config screen reads keyboard keys.** On the keyboard tabs it used to
  announce the equivalent controller button, because the key itself looked unreadable.
  It now says the real key - "tecla: barra espaciadora", "tecla: clic izquierdo".

### Notes
- This version updates one of the mod's own libraries (the input bridge), so it needs the
  game **restarted once** after updating - the new keyboard shortcuts are registered at
  startup too.
- If anything ever goes wrong while the picker is open, the keyboard block releases itself
  automatically within a fraction of a second. It cannot leave you unable to type, and
  Alt+F4 is never blocked.

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
