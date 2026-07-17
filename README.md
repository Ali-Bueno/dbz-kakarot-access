# KakarotAccess — Accessibility mod for DRAGON BALL Z: KAKAROT

**KakarotAccess** makes *Dragon Ball Z: Kakarot* playable by blind and low-vision players. It reads
the game's menus, dialogue and HUD out loud through your screen reader, and adds a 3‑D audio radar so
you can navigate the open world, track quest objectives and locate enemies, NPCs and collectibles —
without changing how the game plays for everyone else.

- **Screen-reader output** via **PRISM** (works with NVDA, JAWS and Windows SAPI).
- **Audio navigation radar** with a directional beacon and arrival cues.
- **Faithful gameplay** — nothing about the game itself is altered; the mod only reads and guides.
- **Fully translatable** — every spoken line lives in an editable text file (13 languages).

> Engine: Unreal Engine 4.21 · Loader: **UE4SS** · Screen reader: **PRISM** (`prism.dll`)

---

## Requirements

- **DRAGON BALL Z: KAKAROT** (Steam).
- A **screen reader running** on Windows — NVDA, JAWS, or the built-in SAPI voice. PRISM speaks
  through whichever one is active.
- **UE4SS** (the Unreal script loader) installed in the game — the installer sets this up for you.

You do **not** need to install PRISM, Tolk or any voice separately: the mod ships its own
`prism.dll` / `tolk.dll` and talks to the screen reader you already use.

---

## Installation

### Recommended — the installer

Download **KakarotAccess Installer** from the [Releases page](https://github.com/Ali-Bueno/dbz-kakarot-access/releases),
run it, and press **Install**. It finds your game folder automatically, downloads the latest release,
installs UE4SS and the mod, and tells you when it's done. Then just launch the game from Steam — you
should hear *"Kakarot accessibility online."*

The installer can also update the mod, roll back to an older version, install the newest development
build, or cleanly uninstall (your game saves are never touched).

### Manual install

If you prefer to install by hand, see [BUILD.md](BUILD.md) for the exact layout
(`…\DRAGON BALL Z KAKAROT\AT\Binaries\Win64\Mods\KakarotAccess\`) and how to enable the mod in
`mods.txt`.

---

## What it reads

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
- A target picker to lock onto anything tracked, sorted by category: **Quests, Collectibles,
  Characters, Companions, Enemies, Sites, Fishing, Gathering, Shops, Minigames, Dragon Balls**.
- Enemy announcements include the enemy's **level**.
- Auto-retargets when the objective changes, and auto-advances through gathering points as you take
  them.

---

## Controls

### Gamepad (overworld)

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
| **D-pad ↑ / ↓** (world map) | Choose a fast-travel destination |

### Keyboard

| Key | Action |
|---|---|
| **F1** | Repeat / re-announce the focused menu item |
| **F2** | Read the on-screen button-help bar |
| **Ctrl+F2** | Toggle automatic button-help announcements |
| **F3** | Toggle the navigation radar |
| **Shift+F3** | Toggle NavMesh route guidance |
| **F5** | Announce the tracked objective (distance, clock direction, above/below) |
| **Shift+F5** | Cycle companion tracking |
| **F8** | Speech test (also names the active screen reader) |
| **Ctrl+F8** | Silence speech immediately |
| **F10** | Read the current quest objective |
| **F11 / Shift+F11** | Next / previous stat block on the status sheet |
| **Ctrl+M** | Toggle the menu reader on/off |

<sub>Developer keys (F4, F6, F7, F9, Ctrl+F5, Ctrl+Shift+F5, Ctrl+Shift+R) are for mod development and diagnostics; end users can ignore them.</sub>

---

## Configuration

Open the in-game config menu with **L3 + R3** in the overworld:

- **Audio cues** — on / off (mutes the radar beacon and pings).
- **Cue volume** — 0–100 % in 10 % steps (plays a sample as you change it).
- **Radar auto-activation** — on / off (manual R3 picks still work when off).
- **Language** — `auto` or any of the 13 supported languages, independent of the game's language.

### Translating the mod

Every spoken line comes from a plain-text file under
`…\Mods\KakarotAccess\Scripts\lang\<code>.txt` (`en es fr de it pt ru pl ja ko zh ar th`). Edit the
value on the right of each `key = value` line, keep the `%s` / `%d` placeholders, and save as UTF-8.
Changes apply on the next launch (or instantly with **Ctrl+Shift+R** in-game). If a line is missing
it falls back to English, so you can't break it. See `lang/README.txt` for the full rules.

---

## Known limitations

- Story/battle **results** numeric values are still being decoded.
- The **status sheet**, **episode title cards** and the **Companions** radar category are being
  finalized.
- **Skill Tree**: jumping straight to a level-2/3 node without visiting its level-1 node first won't
  be reported as locked.

---

## For developers

Build, architecture and iteration details are in [BUILD.md](BUILD.md). The reusable accessibility
playbook and the reverse-engineering reference library live in [CLAUDE.md](CLAUDE.md),
[PRINCIPLES.md](PRINCIPLES.md) and [`reference/`](reference/).

## License

TBD.
