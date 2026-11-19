# Villainous enemies and mining in the radar

Player request 2026-11-11: "add the locations of the villainous enemies on each map, with NVDA
announcing on which location they are, similar to the dragon balls", and "markers for the spots
to mine to find refined ores". Both are coded and **unplayed**: the game was not run that day.

Native evidence: `code/decompiled/_villain_supertraining_findings.txt` (raw output in
`_villain_supertraining_raw.txt`, probe `code/ghidra/villain_probe.java`).

## Villainous enemies ("Atrocious" in the code)

| Fact | Confidence |
|---|---|
| The marker is registered natively, with no `ATMapIconComponent`: `AddEventIcon` (`FUN_14153bbb0`) → MapManager AddIcon `0x141587930` → minimap slot, the Dragon Ball path | proven |
| Its target is the **spawned enemy character** (the owner of the atrocious `AttributeComponent`), registered by the spawn routine `FUN_140f39dd0` | proven |
| Type by phase (`FUN_140d29110`): 0 → 30, 1 → 31, 2 → 34 (35 in `Area16_P`); DLC5 uses 52 and 50 | proven |
| Registered when the enemy SPAWNS, not when the area loads. What fills the spawn queue (`FUN_140d91b00`, one spawn per 3 ticks) was not traced | proven / unknown |
| Removal: per enemy on EndPlay (`FUN_140f392b0` → Release), and in bulk when a phase completes (`RemoveAtrociousIcon`, types 30/31/34/35 only) | proven |
| World map: `UAT_UIMapWorldIcon.Map_Icon_Switch` @0x3B8 is used only for this mark. `FUN_1415ca6f0` makes it Visible with the type at `switch+0x398`; nothing collapses it again | proven |

Why the radar never showed them from afar: the minimap walk asked the target actor for a
map-icon component, found none and skipped the slot. Only the enemy scan (300 m) could see one,
and it called it "enemy".

What the mod does now:

- **R3 group *Enemigos supervillanos*** (`villainous`, after Enemies). `dragonball_marker.villain`
  reads the slot's type from the icon switch, like the balls. No distance cap. The entry is
  named like an enemy ("<name>, nivel N"), guarded by `Nav._char_side`'s atrocious test.
- The group's name is the game's own (`Tutorial_Adv_Title_63`), with our string as fallback.
- A tracked villain is dropped when its marker is released (`Nav._villain_available`), but only
  for targets that came from a marker (`Nav.villain_marked`).
- A villainous character the enemy scan finds first is filed in the same group, 300 m cap.
- **World map:** the travel list says "<point>, Enemigos supervillanos" (`screen_map.ft_villain`,
  `native_offsets.mapWorldIcon.villainTypes`). Read at speaking time, because the game sets the
  mark after the map's open animation.

Limits:

- A villain that has not spawned has no marker and no actor, so the radar lists only spawned
  ones. The world-map mark is what says which area to go to.
- The world-map mark can be stale, like the Dragon Ball one. If players report it, the save has
  the truth: `SaveGame+0xDE760`, `PhaseComplete` int32[32] (2 = cleared), or the static
  `UEventBlueprintFunctionLibrary:IsCompletedAtrociousPhase(FName, uint8)`.
- DLC5 markers (50 / 52) are read natively too. Until 2026-11-19 they stayed in *Enemigos* as
  "grupo enemigo" with the 1 km map cap, so the 23rd World Tournament's **patrol-event hordes**
  were not in the villainous group although the world map called them villainous (player report).
  Now a 50 / 52 marker whose actor carries the atrocious attribute joins *Enemigos supervillanos*,
  no cap (`native_offsets.miniMapIcon.groupVillainTypes`); one on a non-atrocious actor (e.g. a
  Trunks-DLC enemy group) stays where it was.
- Second report the same day: the hordes still never reached the group, and tracking a far
  "grupo enemigo" said "no items left" at once. Both fit a marker on a HIDDEN, non-character
  actor (a horde spawner): the atrocious test needs `AttributeComponent`, and the sweep read
  `bHidden` as "beaten". Now a 50 / 52 minimap slot whose actor has **no map-icon component** is
  treated as the atrocious event icon (`AddEventIcon` registers no component, unlike the
  component-based icons) and joins the group; and a target hidden when picked is never advanced
  on `bHidden` (see [far markers](dbz-kakarot-radar-far-markers.md)). Unplayed. Risk: if a
  Trunks-DLC group also has no component, it is now called villainous too.

Live checklist:

1. World map: an area with villainous enemies must say so after its name.
2. In that area: R3 → *Enemigos supervillanos* lists them from far away, with name and level.
3. Beat one: the radar must chain to the next or say the sweep is done.
4. If the group is empty while the area map shows them, they are not spawned yet: note how far
   away the nearest one is when it first appears.

## Mining

- New R3 group ***Minería*** (`mining`, after Gathering): the ORE map marker (EMapIcon 8) and the
  deposits themselves (`AMineralMiningPointNormal` / `Rare`, AT.hpp:15887).
- Each deposit is named with **every ore it can yield** (`Nav._drop_names_of`: both drop slugs,
  primary id plus `item_drop_alts.lua`), up to four names. A rare deposit says "mineral raro"
  first. The refined ores are items 51003 / 51006 / 51009 (*Hierro refinado*, *Plata refinada*,
  *Oro refinado*) and come from the `Ore_Rare*_Fixed` rows.
- Deposits take the 1 km map cap and the empty-group rescue instead of the 300 m collectible
  cap. A mined-out deposit drops out (`InteractState` Taken), as before.
- The deposit group marker has no taken state and arrives with the wide radius.
