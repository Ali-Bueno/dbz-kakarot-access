# Dragon Ball Z: Kakarot Accessibility - Changelog

---

## v0.1.4 - July 29, 2026

**Crashes only.** The last version said it should not crash, and for some people it still did. So
this one is nothing but the hunt for why: every file in the mod was re-read against the list of
ways this game can kill itself, and each suspect was then argued against by someone whose job was
to prove it harmless. Seven survived that and are fixed here. Nothing else was touched - no new
screens, no new features.

The honest caveat: these were found by reading the code, not by reproducing your crash. If one
still gets through, the log and the flight recorder will say more than before - see *For bug
reports* at the bottom.

### Stability

- **The radar kept holding on to things the game had already destroyed - but only if a menu was
  open at the wrong moment.** When the world goes away, the radar is supposed to let go of
  everything it was tracking: enemies, map icons, your current target. It did - unless a menu, a
  loading screen or a dialogue was already on screen at the instant the world disappeared. Then
  that letting-go step was skipped entirely, and the radar carried dead references through a whole
  battle or cutscene and used them the moment you came back. This is the shape a crash takes when
  it seems to have no trigger: it needed a menu and a fight to overlap, so it happened to some
  people constantly and to others never. Fishing, loading screens and answering an NPC's yes/no
  question straight into a fight were the three easiest ways to hit it.
- **Two keyboard shortcuts were doing real work at the wrong moment.** Shift+F3 (route guidance)
  and Ctrl+Shift+R (reload the mod) both ran instantly on the key press instead of waiting for the
  game's own turn. Shift+F3 talks to the game's navigation system while the radar is reading it,
  and Ctrl+Shift+R rebuilds the entire mod - about sixty files - while every other part of it is
  still running. Both could quietly corrupt the mod's state and bring the game down minutes later,
  somewhere with no apparent connection to the key you pressed. Both now wait their turn.
- **Opening a shop asked the game for something that might not be there.** The wallet is stored
  under a different name on each kind of shop, so the mod tried each name in turn - and asking for
  a name that does not exist on that screen is one of the ways this game closes without warning.
  It now checks before asking instead of asking and hoping.
- **A list reader was checking whether an item still existed only after it had already used it.**
  Every other reader in the mod checks first; this one had the two steps the wrong way round. The
  list it walks is shared with the side-story reward sheet, so a reward popup followed by any
  confirmation window was enough to reach it.

### Fixed

- **Losing your controller no longer leaves the keyboard dead too.** If the pad disconnected -
  a wireless controller falling asleep, a flat battery - the mod never noticed. It kept replaying
  the last thing the pad had said, so it believed a controller was still there, and if the radar
  picker happened to be open it went on blocking the keyboard from the game for the rest of the
  session. There was no way out from inside the game. The mod now notices the pad is gone and
  releases everything.
- **Explore mode no longer scans during loading screens and cutscenes.** Its sweep of the area is
  by far the most expensive thing the mod does, and it was running regardless of what was on
  screen. It now waits. (The sweep itself is still heavy while you are actually exploring - that
  is a separate piece of work.)

### Performance

- **The button-prompt bar is cheaper to read.** It is checked a couple of times a second in
  practically every menu, and it was asking the game for the full identity of every prompt bar it
  had ever seen before checking whether any of them was even on screen. It now checks first.
- **The story results screen stops re-reading ranks it has already announced.** It was resolving
  every rank letter and every digit image on every check, for as long as the screen stayed up,
  and then discarding the ones it had already spoken.

### For people who build the mod

- **A release can no longer ship with the debug console switched on.** Packaging copied a settings
  file straight from whoever was building it, and the only thing keeping the console out of a
  release was a comment asking a person to remember. It is now set by the packaging script itself.

---

## v0.1.3 - July 28, 2026

**This version should not crash.** That was the point of the release: earlier versions protected
the parts of the mod that read menus, and this one gives the same protection to the part that reads
the world while you explore and fight, which is where the crashes that were left came from. A full
day of play on this build produced none. If one does slip through, the log now names the screen
that was being read - see *For bug reports* at the bottom.

### Screens that now read

- **The Story menu.** Entries, the recommended level, the summary, and the completion rewards. The
  X button's progress checklist is read too - the mod tells the two panels apart from the game's
  own state rather than guessing.
- **The Z Encyclopedia.** All three levels: the contents index, the category lists across both
  pages of the spread, the entry sheets, and the note slips. Locked entries read the unlock
  condition the game shows in place of the summary.
- **The character list inside the Party menu.** Confirming Player or Support opens a roster, and
  that half of the screen - the half where you actually choose someone - had never been read.
- **The defeat menu.** It had never once announced itself, in any version.
- **The reward sheet after finishing a side story.** Same: present in the mod, never actually ran.
- **Item quantities.** Lists now say how many of each item you own, and re-say it on its own when
  it changes.
- **Your Zeni.** The shops announce your balance, and repeat it whenever it changes - so every
  purchase and sale tells you what you have left.
- **The required level in the training menu.** The screen said "recommended level" and never the
  number, because the label and the figure are two separate pieces of text.

### Fixed

- **The radar no longer abandons a side story.** With a multi-phase side story in progress it kept
  snapping back to the main quest between phases, so you had to open the picker again at every
  step. It now recognises which kind of quest you are actually working on and stays with it,
  including across a loading screen if the story sends you to another area. It lets go when the
  quest log shows that story is finished - then the main quest takes over again by itself.
- **The item menu's recovery tab was silent.** Every other tab read. Two separate causes: another
  reader was holding that screen without saying anything, and a value the mod had been treating as
  "this category is empty" turned out to be the tab number, so the first tab always looked empty.
- **Registering an item to the palette no longer repeats endlessly.**
- **Shops no longer say every item is sold out.** The "sold out" label exists for every item and
  the game simply hides it when there is stock; the mod was reading it without checking whether it
  was on screen.
- **The objective is no longer re-announced over and over.** It repeated on opening and closing the
  world map. It is now read when it changes, or on the area map when you want to review it.
- **The world map's travel d-pad.** It was slow to start responding and worked only sometimes.
  Presses made while the destination list was still loading were being dropped instead of
  remembered, and the mod was competing with itself for the work needed to build that list.
  Declining a trip also used to send you back to the top of the list - it now leaves you on the
  point you were looking at, so you can keep moving from there.

### Performance

- **Small stutters while exploring are gone.** The two new menu readers were making the mod search
  the game's object list for screens that cannot possibly be open while you are walking around.
  They now stay completely idle in the field.

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
