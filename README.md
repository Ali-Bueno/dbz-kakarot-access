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

- **Screen-reader output** via **PRISM** (works with NVDA, JAWS and Windows SAPI).
- **Audio navigation radar** with a directional beacon and arrival cues.
- **Faithful gameplay** — nothing about the game is altered; the mod only reads and guides.
- **Fully translatable** — every spoken line lives in an editable text file (13 languages).

> Engine: Unreal Engine 4.21 · Loader: **UE4SS** · Screen reader: **PRISM**

---

## Requirements

- **DRAGON BALL Z: KAKAROT** (Steam).
- A **controller** (required — see above).
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
| **D-pad ↑ / ↓** (world map) | Choose a fast-travel destination |

### Keyboard (optional extras)

Handy shortcuts that mirror or complement the controller — not required to play:

| Key | Action |
|---|---|
| **F1** | Repeat / re-announce the focused menu item |
| **F2** | Read the on-screen button-help bar |
| **Ctrl+F2** | Toggle automatic button-help announcements |
| **F3** | Toggle the navigation radar |
| **Shift+F3** | Toggle NavMesh route guidance |
| **F5** | Announce the tracked objective (distance, clock direction) |
| **Shift+F5** | Cycle companion tracking |
| **F8** | Speech test (also names the active screen reader) |
| **Ctrl+F8** | Silence speech immediately |
| **F11 / Shift+F11** | Next / previous stat block on the status sheet |
| **Ctrl+M** | Toggle the menu reader on/off |
| **Ctrl+Shift+R** | Reload the mod (useful after editing a translation file) |

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

- **Skill Tree**: jumping straight to a level-2/3 node without visiting its level-1 node first won't
  be reported as locked.
- See *Not yet supported* above for the systems still being worked on.

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
