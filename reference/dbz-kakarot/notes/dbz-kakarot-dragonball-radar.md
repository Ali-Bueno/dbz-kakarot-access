# Dragon Ball radar

Status (2026-10-06): reader corrected after players reported that Dragon Balls were still
impossible to find. Offline suite green (the v0.1.6 reader fails 25 of its checks); positive
in-game pickup verification is still pending. The R3 / V category uses the game's own minimap
markers, direction/distance and beacon. No unlocks, save changes, debug spawning, placement-table
inspection or new world scans.

Full native evidence: `code/decompiled/_dragonball_findings.txt` (raw decompiler output in
`_dragonball_raw.txt`, reusable probe `code/ghidra/dragonball_probe.java`).

## What a sighted player has

- The **world map** marks every area that holds a ball (orange ball next to the area).
- The **area map** shows every ball in the current area, at any distance.
- The **minimap** shows a ball only inside its circle, and flashes icons that overlap.

The radar must therefore list every ball in the current area at any distance, and must never
read the minimap's drawing state as availability.

## Why v0.1.6 never found a ball (both bugs, proven)

1. **Wrong byte for the type.** The 2026-09-08 RE took the icon vtable as 0x143ecb408; the
   constructors store **0x143ecb3f0** (nothing references 0x408), so every slot was read three
   off. +0x89 is `SetShown` / `IsShown` — the "inside the minimap circle" flag, 0 or 1 — not the
   EMapIcon. The reader rejected every icon whose +0x89 was not 28, i.e. all of them.
2. **Drawing state used as availability.** It also required `WL_Icon_ImgSw` to be on screen and
   rendered. Outside the circle the radar tick collapses that switch; overlapping icons flash
   its `ColorAndOpacity` alpha. So even with the right type, only near balls would list, and a
   tracked ball would have been "collected" on every dark phase of the flash.

## Native facts (UAT_UIMiniMapIcon, vtable 0x143ecb3f0)

| Slot | Function | Meaning |
|---|---|---|
| +0x240 Init | 0x1415f1070 | SetShown(1); switch type; switch.Target = actor |
| +0x248 GetType | 0x1415e38c0 | `u8(WL_Icon_ImgSw + 0x398)` — **the EMapIcon** |
| +0x250 Release | 0x1415df920 | +0x88 = 0, TargetActor = 0, SetShown(0) |
| +0x258 SetShown | 0x1415f50d0 | +0x89 = in circle; collapses / re-adds the switch |

- Slot allocation (0x1415e80a0) happens when the ball spawns, **whatever the distance**; the
  slot keeps +0x88 = 1, TargetActor = ball and type 28 while the ball is out of range.
- Pickup (0x14114b850): minimap **Release** + area-map removal → `m_dragonBallInfo[i]` zeroed
  → save entry cleared → actor destroyed. So `+0x88 == 1` with a valid TargetActor is exactly
  "spawned and not collected". +0x8A is "layout applied" and is read by nothing.
- Ball actors exist only while their `ADragonballSpawner` has begun play, and new placements
  skip the area the player is in — a ball is normally in ANOTHER area until the player goes
  there. Spawning is gated by stone cooldown, ownership and progress data, so a reader that
  follows the game's own markers cannot reveal a ball early.

## Reader contract (dragonball_marker.lua)

Native-base `IsA` → `+0x88 == 1` → `u8(WL_Icon_ImgSw + 0x398) == 28` → owner is this minimap →
TargetActor valid and not `bHidden`. Offsets live in `native_offsets.lua` (`miniMapIcon`). The
switch's visibility, alpha and +0x89 are never read. `contains` returns true / false / nil, so an
unreadable read is never taken as collection. The `dragonball` group has no radar distance cap
(like quests). The type byte is read before the owner hop so the 100 ms tracking walk rejects
non-ball icons cheaply.

## Known limits

- **Pending-kill window:** a ball destroyed by a level unload reads non-null until GC. The
  transition gate covers the unload; the game's own check is GUObjectArray flag bit 29.
- **Icon never registered:** if the HUD/map manager is null when a ball spawns, the game makes no
  minimap or area-map icon (sighted players would not see it either). Not seen in practice.
- **Streaming:** if a spawner sits in a distance-streamed sublevel, its ball has no actor (and no
  marker) until that sublevel loads. Not decidable statically; the area map shares the limit.
- **Enemy-held balls (RAND_ENEMY):** no native path registers one; a Blueprint could. Unknown.

## World map: which area to travel to (screen_map.lua)

The travel list says "<point>, Dragon Ball" for every point the world map marks. Source:
`UAT_UIMapWorldIcon + 0x3F4` (`native_offsets.mapWorldIcon`), read on the icon `ft_build`
already matches by address. FUN_1415cbd50 sets it together with showing `ImageCtn[2]`
(`Img_Micon27`, the orange ball) for each area whose ball is placed and uncollected
(`!IsStone && save[i].bSpawned && point row.WorldMapLocation`, findings Q4).

Limit (findings Q8, proven): nothing ever CLEARS that mark except the icon's one-time setup, so
if the game reuses the icon widget across openings the mark survives a pickup, a wish (stone
cooldown) or a DLC story. That is what the screen draws, so it is parity with a sighted player,
but it can point at an area the R3 category then finds empty. Whether the widget is rebuilt per
opening is unknown (Blueprint creation). If players report stale marks, compute the predicate
from the save instead (raw `SaveGame+0x52CF0+i*0x20`, plus the point-table lookup).

## Live checklist

0. Open the world map: the travel list must say "Dragon Ball" after the marked points. Collect
   that ball, reopen the map, and note whether the mark is gone (answers the Q8 unknown).
1. In an area the world map marks: R3 / V → Dragon Balls lists the ball from far away; pick it
   and follow the beacon all the way, through the minimap's flashing near other icons.
2. Collect it: the radar must chain to the next ball or say the sweep is done, within a tick.
3. Pause / resume while approaching and while standing on the ball; it must not skip.
4. Change areas: only the current area's balls list.
