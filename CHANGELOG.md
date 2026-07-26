# Dragon Ball Z: Kakarot Accessibility - Changelog

---

## Unreleased

### Stability

- **Fixed the crashes that happened while playing, not in menus.** Previous releases hardened the
  parts of the mod that read menus, and that work held. The part that reads the *world* - the one
  driving the radar, which runs constantly while you explore and while you fight - had never been
  given the same protection. It was reading game objects without first checking they still existed,
  and the game frees those objects all the time: as you cross from one area to another, and as
  enemies die. That is why a crash could arrive after ten minutes or after two hours, in the middle
  of combat or just walking, with no obvious trigger. Every one of those reads is now checked.
  The tracked radar target got particular attention: it is a single object the mod holds on to for
  minutes at a time, which made it the most exposed thing in the mod.
- **Fixed the crash when moving between areas.** The reader that announces pickup and level-up
  notifications was asking those little banners for a piece of text that some of them do not have.
  Loading a new area rebuilds them, which is why the crash landed just after a map change. This one
  was not deduced - the mod now keeps a flight recorder, and it named the culprit outright.
- **Fixed the same fault in twelve more places.** Once the cause was known, a mechanical check
  found every other reader asking for a piece of text that might not exist: the shop, the community
  board, fishing and its results sheet, battle results, tutorials and the action bar.
- **Fixed a crash waiting to happen on the on-screen action bar.** The mod read nine button slots
  from that bar because that is how many the bar it was written against has. On a screen with a
  shorter bar, asking for the tenth would have closed the game.
- **A screen that fails now fails alone.** If one screen reader hits a problem, it no longer takes
  the rest of the readout down with it for that moment, and it writes its own name into the log
  instead of failing anonymously.

- **Fixed the world map's travel selection not responding.** Choosing a destination with the
  d-pad worked on some visits and not others - more often not, the more times you opened the map.
  If the map's travel points were not ready on the exact moment the mod first looked, it recorded
  "there are none" for the rest of that visit: no destination list read out, and the d-pad did
  nothing. It now keeps looking until they are there.

### Performance

- **Menus should respond faster again.** Two things had been slowing them down, both introduced by
  the mod itself. The safety check added in v0.1.2 was roughly three times more expensive than what
  it replaced, and it was being run over entire lists of on-screen elements many times a second;
  it is now calculated once per cycle and reused. Separately, the limit meant to stop the mod
  searching the game's object list too often was being reset by six different parts of the mod, so
  a cap of two searches per cycle was really allowing about a dozen - each one costing a visible
  fraction of a frame. The limit now measures real time and means what it says.

### For bug reports

- **The log now records which screen you were on.** One line each time the reader moves to a new
  screen. If the game closes unexpectedly, the end of the log says what was being read.
- **The README explains how to send a crash report** - where the log lives, and the warning that it
  is replaced every time the game starts, so it needs copying before relaunching.

---

## v0.1.2 - July 25, 2026

### Stability
- **Fixed the crash that could close the game while browsing menus.** v0.1.1 reduced this
  fault without eliminating it, as its notes warned. The root cause is now understood and
  closed off: the mod could read a user-interface object the game had already freed - most
  easily reproduced by scrolling the cooking menu, where the game recycles the rows as you
  move - and the check that was supposed to catch that turned out to inspect the object
  *before* deciding whether it still existed. Every reader now runs a memory check that
  cannot fault, before anything touches the object.
- **Fixed the crash on map changes, cutscenes and returning to the title screen.** The mod
  used to be told about a level change through a callback that the engine could run on its
  own loading thread, at the same time as the mod's own loop. It now notices the change on
  its own schedule instead, which removes the whole class of fault and also reacts sooner.
- **The mod no longer runs its own reading logic while a level is being torn down.**

### Bug Fixes
- **Remapped controller buttons are now announced correctly.** If you changed your button
  layout in Options, the mod kept naming the buttons the game shipped with - melee moved
  from B to X was still read as "B". This affected far more than the settings screen:
  every button prompt in the game, the on-screen action bar and every tutorial line was
  telling you the wrong button. The mod now reads your actual layout from your save.
- **The button-config screen updates as you rebind it**, not only after pressing "Save
  changes". Assign a new button and move to the next row: what you hear is already the new
  assignment. Reading only the saved layout meant the screen said one thing and the mod
  said another until you confirmed, which is exactly when a blind player most needs them
  to agree.
- **Messages are no longer cut off mid-sentence.** When a notice appeared - a new area
  unlocked on the map, a skill acquired, the popup explaining what a skill needs - the
  screen underneath announced itself again a moment later and silenced the message before
  it finished. Notices are now allowed to finish; whatever was going to be read waits its
  turn instead of interrupting.
- **A dialog you trigger again is read again.** The same message shown a second time - by
  picking the same option, or a different option with the same text - stayed silent,
  because the mod remembered having said those words. It now only remembers for as long
  as that window is on screen: closing it and opening it again is a new message.
- **Seven screens that had gone silent read again.** Six of them were a single fault: a
  shared helper used to read the game's lists asked an object a question it refuses to
  answer, and the resulting error killed the reader mid-sentence with nothing in the log
  a player could see. The seventh was the Options screen, which read correctly the first
  time you opened it and stayed silent on every later visit.
- **Conversations where a character speaks without being present are now read.** Some
  story dialogue arrives through a portrait pop-up rather than the usual dialogue box -
  a character calling you from elsewhere - and that surface had never been covered, so
  those lines were simply missing.
- **The quest objective is no longer announced several times.** The on-screen text fills in
  progressively, and each step was being read as if it were new. It is also no longer
  re-read when you close the map, and no longer talks over the map's own readout.
- **Pickup and level-up notifications no longer repeat.** The banner they use blinks, and
  each blink was counted as a new notification.
- **A skill-unlock message no longer comes back on its own** minutes later, in the middle
  of free roam.

### Improvements
- **Keyboard players can use the radar target picker.** It used to be reachable only by
  clicking the right stick. **V** opens and closes it, the **up and down arrows** move
  through targets, **left and right** change category, **Enter** locks onto the focused
  one and **Escape** closes it and stops tracking. While it is open the game no longer
  receives your keystrokes at all - exactly what the mod already did with the controller -
  so plain keys are safe here even though the game itself uses them (V is the skill
  palette, the arrows are mount/dismount).
- **The world map's travel list responds to the arrow keys**, exactly as it already did to
  the d-pad: up and down move between travel points, **Enter** travels to the selected one.
- **The button-config screen reads keyboard keys.** On the keyboard tabs it used to
  announce the equivalent controller button, because the key itself looked unreadable.
  It now says the real key - "key: space bar", "key: left click".
- **New radar category: Exits.** Finding the way out of a building was a real problem -
  the door is an invisible trigger volume with no map icon of its own. It is now the
  second category in the R3 picker, one press from the default, and reads the destination
  area's name where the game provides one.
- **The defeat screen is read.** When you are beaten, the mod now announces the screen and
  the highlighted option (Retry / Load / Return to title); before, it was silent.

### Notes
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
