# KakarotAccess — Accessibility mod for DRAGON BALL Z: KAKAROT

**KakarotAccess** makes *Dragon Ball Z: Kakarot* playable by blind and low-vision players. It reads
the game's menus, dialogue and HUD out loud through your screen reader, and adds a 3‑D audio radar so
you can navigate the open world and track quest objectives, enemies and collectibles — without
changing how the game plays for everyone else.

> ### 🎮 A game controller is **required**
> This mod is designed to be played with a **controller** (Xbox / DualShock / DualSense). Core
> features — the radar target picker, the config menu, stat stepping, the world map — are on the
> controller, and the game itself plays far better with one. Keyboard shortcuts exist as extra
> conveniences, but they are **not** a full substitute. Plug in a controller before you play.

> ### ⚠️ Launch the **normal** version on Steam
> When you start the game, Steam asks which version to play — **DRAGON BALL Z: KAKAROT HD** and the
> normal version. The mod only works with the **normal** version, so always choose that one. Tick
> **"Do not ask again"** in the chooser so Steam launches straight into it every time.

- **Screen-reader output** via **PRISM** (works with NVDA, JAWS and Windows SAPI).
- **Braille display support** — every spoken line also goes to a refreshable braille display,
  through the same screen reader. On by default when your reader supports it.
- **Audio navigation radar** with a directional beacon and arrival cues.
- **Faithful gameplay** — nothing about the game is altered; the mod only reads and guides.
- **Fully translatable** — every spoken line lives in an editable text file (13 languages).

> Engine: Unreal Engine 4.21 · Loader: **UE4SS** · Screen reader: **PRISM**

---

## Requirements

- **DRAGON BALL Z: KAKAROT** (Steam).
- A **controller** (still recommended — see above). Keyboard support is growing: the button-config
  screen now reads your actual keyboard keys, and the radar target picker can be driven entirely
  from the keyboard (see Controls). The config menu and the status sheet still want a pad.
- A **screen reader** running on Windows — NVDA, JAWS, or the built-in SAPI voice.
- **UE4SS** (the Unreal script loader) — the installer sets this up for you.

You don't need to install PRISM, Tolk or any voice separately: the mod ships its own screen-reader
bridge and talks to whatever screen reader you already use.

---

## Installation

### Recommended — the installer

Download **KakarotAccessInstaller.exe** from the
[latest release](https://github.com/Ali-Bueno/dbz-kakarot-access/releases/latest), run it, and press
**Install**. It finds your game folder automatically, downloads the latest version, sets up UE4SS and
the mod, and tells you when it's done. Then launch the game from Steam — you should hear
*"Kakarot accessibility online."*

The installer can also update the mod, pick an older version, or cleanly uninstall (your game saves
are never touched).

### Manual install

See [BUILD.md](BUILD.md) for the exact layout
(`…\DRAGON BALL Z KAKAROT\AT\Binaries\Win64\Mods\KakarotAccess\`) and how to enable the mod in
`mods.txt`.

---

## What works

**Menus & UI**
- Main menu, pause menu, options / system / title screens, confirmation and choice popups.
- Shops (food, materials, info), the item quick-slot palette, and the full Items / Party /
  Characters inventory (empty categories are announced as such).
- Save / Load slots, including empty slots, in the correct order.
- Character **status sheet** — name, level, EXP to next, HP, Ki, Battle Power, and every stat block.
- **Skill Palette** (Super Attack equip slots) and the **Skill Tree** (node name, level, Ki cost,
  description, owned orbs, locked/acquired state).
- The Soul Emblems grid and the Community board.
- Cooking, fishing (with hook-timing cues and the catch-result sheet), and results screens.
- On-screen **button-prompt bar** ("A: use", "X: assign", …), announced with the real button glyphs.

**Story & world**
- NPC dialogue and subtitles, tutorial guidance, quest-objective text.
- Episode / chapter title cards and cinematic character-intro cards.
- Pickup and level-up notifications.

**Navigation radar**
- A directional 3‑D audio beacon that tracks your current quest objective and plays an arrival cue,
  auto-resuming after battles.
- A target picker (**R3**) to lock onto anything tracked, sorted by category: **Quests, Collectibles,
  Characters, Companions, Enemies, Sites, Fishing, Gathering, Shops, Minigames, Dragon Balls**.
- Enemy announcements include the enemy's **level**.
- Auto-retargets when the objective changes, and auto-advances through gathering points as you take
  them.

---

## Not yet supported (coming)

These parts of the game aren't accessible yet — they're on the roadmap:

- **Encyclopedia Z** menu.
- **Baseball** minigame.
- **Story** menu.
- **Talking to your party / group members** while in a town or village.
- Story/battle **results** numeric values (still being decoded).
- Final polish on the **character status sheet**, **episode title cards** and the **Companions**
  radar category.

If something you need isn't listed, let me know (see the links at the bottom).

---

## Controls

### Controller (primary)

| Input | Action |
|---|---|
| **R3** (right-stick click) | Open the radar **target picker** |
| **R3** (double-tap) | Toggle passive **"explore"** radar |
| **L1 / R1** (picker open) | Previous / next category |
| **D-pad ↑ / ↓** (picker open) | Move through targets (nearest first) |
| **A** (picker open) | Lock onto the focused target |
| **B** (picker open) | Close and stop tracking |
| **L3 + R3** | Open / close the **config menu** |
| **D-pad ↑↓ / ←→** (config menu) | Move between options / change a value |
| **D-pad ↓ / ↑** (status sheet) | Step through the stat blocks |
| **Select + Y** (world map) | Open zone travel (Y = Triangle) |
| **D-pad ↑ / ↓** (world map) | Move between zones — **hold** to keep stepping |
| **X** (world map) | Travel to the selected zone (X = A on Xbox) |
| **Xbox X / PS Square** (either map) | Describe what's selected: destination + position in the list + what the cursor is over. On the area map, re-reads the point of interest under the cursor |
| **Left stick** (Community board) | Move around (d-pad support planned, like the map) |

### Keyboard (optional extras)

Handy shortcuts that mirror or complement the controller — not required to play:

| Key | Action |
|---|---|
| **F1** | Repeat / re-announce the focused menu item |
| **F2** | Read the on-screen button-help bar |
| **Ctrl+F2** | Toggle automatic button-help announcements |
| **F3** | Toggle the navigation radar |
| **Shift+F3** | Toggle NavMesh route guidance |
| **V** | Open / close the radar **target picker** (the R3 menu, from the keyboard) |
| **↑ / ↓** (picker open) | Move through targets (nearest first) |
| **← / →** (picker open) | Previous / next category |
| **Enter** (picker open) | Lock onto the focused target |
| **Escape** (picker open) | Close the picker and stop tracking |
| **↑ / ↓** (world map) | Move between travel points — the arrows act as the d-pad here |
| **Enter** (world map) | Travel to the selected point |
| **F5** | Announce the tracked objective (distance, clock direction) |
| **Shift+F5** | Cycle companion tracking |
| **F8** | Speech test (also names the active screen reader) |
| **Ctrl+F8** | Silence speech immediately |
| **F11 / Shift+F11** | Next / previous stat block on the status sheet |
| **Ctrl+M** | Toggle the menu reader on/off |
| **Ctrl+Shift+R** | Reload the mod (useful after editing a translation file) |

While the radar picker is open the game does not receive your keystrokes at all — the same thing
the mod already did with the controller — so plain, unmodified keys are safe here even where the
game uses them itself (V is the skill palette by default, the arrows are mount/dismount).

---

## Configuration

Open the in-game config menu with **L3 + R3** in the overworld:

- **Audio cues** — on / off (mutes the radar beacon and pings).
- **Cue volume** — 0–100 % in 10 % steps (plays a sample as you change it).
- **Radar auto-activation** — on / off (manual R3 picks still work when off).
- **Braille output** — `auto` / `on` / `off`. Everything the mod speaks is also sent to a
  refreshable **braille display** through your screen reader. `auto` (the default) turns it on when
  the reader reports braille support, so a connected display just works; set it to `off` if you
  don't use one and want to save the extra call per line.
- **Language** — `auto` or any of the 13 supported languages, independent of the game's language.

### Translating the mod

Every spoken line comes from a plain-text file under
`…\Mods\KakarotAccess\Scripts\lang\<code>.txt` (`en es fr de it pt ru pl ja ko zh ar th`). Edit the
value on the right of each `key = value` line, keep the `%s` / `%d` placeholders, and save as UTF-8.
Changes apply on the next launch (or instantly with **Ctrl+Shift+R** in-game). If a line is missing
it falls back to English, so you can't break it. See `lang/README.txt` for the full rules.

---

## Known limitations

- **Skill Tree**: jumping straight to a level-2/3 node without visiting its level-1 node first won't
  be reported as locked.
- See *Not yet supported* above for the systems still being worked on.

---

## Reporting a crash

If the game closes on its own, **one file tells us almost everything** — and without it a crash
report can only be guessed at. Please send it along with your report.

**The file:** `UE4SS.log`, in your game folder, next to the game's executable:

```
…\steamapps\common\DRAGON BALL Z KAKAROT\AT\Binaries\Win64\UE4SS.log
```

The quickest way to get there: press <kbd>Windows</kbd>+<kbd>R</kbd>, paste the path below, and
press Enter — the folder opens with the file in it.

```
%ProgramFiles(x86)%\Steam\steamapps\common\DRAGON BALL Z KAKAROT\AT\Binaries\Win64
```

(If Steam is on another drive, open the game's folder from Steam: *Library → right-click DRAGON
BALL Z KAKAROT → Manage → Browse local files*, then go into `AT`, `Binaries`, `Win64`.)

**The order matters, because the file is overwritten every time the game starts:**

1. **Right after the crash, copy `UE4SS.log` somewhere safe** (your desktop is fine). This copy
   records which screen the reader was on as you played, up to the moment it stopped.
2. **Then start the game once more and quit normally.** The mod keeps a small flight recorder of
   its last operations, which survives a crash; on the next launch it writes them into the new
   `UE4SS.log` under *PREVIOUS SESSION ENDED HERE*. That section says what the mod was doing in
   the final fraction of a second.
3. **Send us both copies.**

Neither file contains personal information beyond your game folder's path.

**Also useful, if it exists:** Windows may have written a crash report to
`%LOCALAPPDATA%\AT\Saved\Crashes\` — if there is a folder there with a recent date, zip it up
too.

**What to tell us:** what you were doing (which menu, or free roam), whether it had been running
long, and whether it is repeatable. Report on
[GitHub Issues](https://github.com/Ali-Bueno/dbz-kakarot-access/issues) or the Discord below.

---

## Links & community

- **Latest release:** https://github.com/Ali-Bueno/dbz-kakarot-access/releases/latest
- **Discord:** https://discord.gg/TDBvUxeRX
- **Patreon:** https://www.patreon.com/alibueno

---

## For developers

Build, architecture and iteration details are in [BUILD.md](BUILD.md). The reusable accessibility
playbook and the reverse-engineering reference library live in [CLAUDE.md](CLAUDE.md),
[PRINCIPLES.md](PRINCIPLES.md) and [`reference/`](reference/). Developer/diagnostic keybinds are
compiled out of releases (see `build_flags.lua` + `package.ps1`).

## License

TBD.
