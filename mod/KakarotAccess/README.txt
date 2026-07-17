KakarotAccess — Accessibility mod for DRAGON BALL Z: KAKAROT
============================================================

KakarotAccess reads the game's menus, dialogue and HUD out loud through your
screen reader, and adds a 3-D audio radar so you can navigate the open world and
track quest objectives, enemies and collectibles. It doesn't change how the game
plays for sighted players — it only reads and guides.


IMPORTANT — WHICH VERSION TO LAUNCH ON STEAM
--------------------------------------------
When you start the game, Steam asks which version to play: "DRAGON BALL Z:
KAKAROT HD" and the normal version. The mod ONLY works with the NORMAL version,
so always choose that one. The chooser has a "Do not ask again" checkbox — tick
it so Steam launches straight into the normal version every time (more
comfortable, no menu to deal with each launch).


REQUIREMENTS
------------
- A CONTROLLER (required — the mod is built around it: the radar picker, the
  config menu, stat stepping and the map are all on the controller).
- A screen reader running: NVDA, JAWS, or the built-in Windows SAPI voice.


FIRST LAUNCH
------------
When the game finishes loading you should hear "Kakarot accessibility online."
If you don't, check that a screen reader is running and that you launched the
NORMAL version (see above). Press F8 at any time to test speech.


HOW ANNOUNCEMENTS WORK
----------------------
- Menus are read as you move. Entering a screen reads its name and the focused
  item; moving the cursor reads the new item; changing a value reads just what
  changed. A change of context interrupts the voice; small updates do not.
- NPC dialogue and subtitles are read as they appear.
- The current quest objective is announced automatically whenever it changes.
- Level-ups and item pickups are announced.
- On-screen button prompts ("A: use", "X: assign", ...) can be announced
  automatically when you enter a screen (toggle with Ctrl+F2). Press F2 any time
  to hear them.
- Press F1 to repeat the focused item / last announcement. Press Ctrl+F8 to
  silence the voice immediately.


THE NAVIGATION RADAR
--------------------
The radar helps you move through the open world with sound.

- When you have a target, a repeating 3-D audio BEACON plays from the direction
  of that target: it sounds to your left, right or front as you turn, so you can
  walk toward it. An ARRIVAL sound plays when you reach it.
- Press F5 to hear the tracked target in words: what it is, its distance, the
  clock direction (for example "2 o'clock") and whether it is above or below you.
- The radar automatically resumes after a battle and, for gathering, advances to
  the next spot (mining, fruit, fishing) as you collect the current one.
- Turn the radar on or off with F3 (off = instant silence).


CHOOSING WHAT TO TRACK — THE TARGET PICKER (R3)
-----------------------------------------------
- Click the RIGHT STICK (R3) to open the picker. It freezes game input while
  open, so you can browse safely.
- L1 / R1: previous / next CATEGORY. Categories: Quests, Collectibles,
  Characters, Companions, Enemies, Sites, Fishing, Gathering, Shops, Minigames,
  Dragon Balls.
- D-pad up / down: move through the items in that category (nearest first).
- A: lock onto the highlighted item — it becomes your radar target.
- R3 again: close without changing the current target.
- B: close AND stop tracking.
- Double-tap R3: toggle "explore" mode — a passive radar that points out nearby
  things as you wander.

Enemies are announced with their LEVEL. Press Shift+F5 to cycle tracking through
your nearest party members and back to the quest objective.


THE CONFIG MENU (L3 + R3)
-------------------------
Hold L3 and tap R3 in the overworld to open the mod's settings:
- Audio cues on / off (mutes the radar beacon and pings).
- Cue volume 0-100% in 10% steps (plays a sample as you change it).
- Radar auto-activation on / off (manual R3 picks still work when off).
- Language: "auto" or any of the 13 supported languages.
Move with the d-pad (up/down between options, left/right to change a value; A
also advances the value). Press B to close.


CONTROLS — QUICK REFERENCE
--------------------------
Controller (primary):
  R3 ................ open the radar target picker
  R3 (double-tap) ... toggle the explore radar
  L1 / R1 ........... picker: previous / next category
  D-pad up/down ..... picker: move through targets
  A ................. picker: lock onto the target
  B ................. picker: close and stop tracking
  L3 + R3 ........... open / close the config menu
  D-pad ............. config menu: move between options / change a value
  D-pad down/up ..... status sheet: step through the stat blocks
  Select + Y ........ world map: open zone travel (Y = Triangle)
  D-pad up/down ..... world map: move between zones
  X ................. world map: travel to the selected zone (X = A on Xbox)
  Left stick ........ Community board: move around (d-pad support is planned,
                      the way the map already works)

Keyboard (optional extras — not required to play):
  F1 ................ repeat / re-announce the focused item
  F2 ................ read the on-screen button-help bar
  Ctrl+F2 ........... toggle automatic button-help announcements
  F3 ................ toggle the navigation radar
  Shift+F3 .......... toggle NavMesh route guidance
  F5 ................ announce the tracked objective (distance, direction)
  Shift+F5 .......... cycle companion tracking
  F8 ................ speech test (also names your screen reader)
  Ctrl+F8 ........... silence the voice
  F11 / Shift+F11 ... status sheet: next / previous stat block
  Ctrl+M ............ toggle the menu reader on / off
  Ctrl+Shift+R ...... reload the mod (handy after editing a translation file)


TRANSLATIONS
------------
Every spoken line comes from Scripts\lang\<code>.txt (en, es, fr, de, it, pt,
ru, pl, ja, ko, zh, ar, th). Open your language's file in a plain-text editor,
change the text on the RIGHT of each "key = value" line, keep the %s and %d
placeholders exactly, and save as UTF-8. Changes apply on the next launch (or
instantly with Ctrl+Shift+R). If a line is missing it falls back to English, so
you can't break it. See Scripts\lang\README.txt for the full rules.


NOT YET SUPPORTED (coming)
--------------------------
Encyclopedia Z, the baseball minigame, the Story menu, and talking to your party
members in towns. If something you need isn't covered, let me know on Discord.


LINKS
-----
- Latest version: https://github.com/Ali-Bueno/dbz-kakarot-access/releases/latest
- Discord:        https://discord.gg/TDBvUxeRX
- Patreon:        https://www.patreon.com/alibueno

Thanks for playing!
