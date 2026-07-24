# dbz-kakarot-radar-picker

> DBZ Kakarot R3 radar target picker — input_bridge.dll XInput IAT hook to read+block the pad; modal category menu feeding the audio radar; VERIFIED in-game


Controller feature for the DBZ Kakarot accessibility mod
([dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md)): a modal **R3 (right-stick click) target picker**
that chooses what the audio radar ([[nav_tracker]] / quest radar) tracks. Built +
VERIFIED in-game 2026-07-04 (needs a game RESTART to load the new DLL, not just reload).

**Open button = R3, NOT R2/L2** (R2 = descend / L2 = ascend on the Flying Nimbus, so
triggers stay free). Categories cycled with L1/R1; A selects; R3-again/B close.
**Categories:** Quests (unlimited dist), **Collectibles** (Field Memories = glowing
Z-Encyclopedia flashback items + item pickups + chests, scanned directly since they're
NOT minimap icons; hidden actors skipped via the REFLECTED `AActor.bHidden` property —
the original `IsHidden()` call was a non-reflected member and its uncatchable C++ throw
killed the R3 loop for whole sessions, fixed 2026-07-04, see [dbz-kakarot-crash-bug](dbz-kakarot-crash-bug.md)),
NPCs (300 m, Cpl*→Goku/Gohan names),
Fishing/Gathering/Shops/Minigames/DragonBalls (the game's minimap icons, ≤1 km),
Other. Hand-picked targets use a TIGHT 1.5 m arrival (auto quest = 8 m) and prompt the
action button on arrival. Field Memory recap viewer read by `screen_memory.lua`
(Field_Memory_C: Txt_Title + Txt_Msg). Commit d1efdb2, pushed.

**KEY ENABLER — `input_bridge.dll`** (`src/input_bridge/`, 5th native bridge alongside
prism/mem/audio). The game reads the pad via `XINPUT1_3.dll!XInputGetState` (verified in
the exe import table). The bridge **IAT-hooks** that import (exe first, then scans all
loaded modules): its wrapper calls the real fn, caches the TRUE state for Lua `poll()`,
and while `block(true)` returns a NEUTRAL pad to the GAME so no command triggers. This is
how "los comandos del juego no pueden triggerearse mientras el menú está abierto" is met
without hooking the game's own (CFramework-native, hook-hostile) input. No xinput link —
the real pointer is taken from the IAT slot. If the import can't be patched it degrades to
read-only (block = no-op); check `input_bridge loaded (hooked=true)` in the boot log.

**Interaction (modal toggle, user-chosen 2026-07-04):** R2 opens (blocks pad), L1/R1 =
category tabs, d-pad = items, **A/Cross = select+track**, **R2-again = cancel**, **B =
close + stop tracking**. On close the pad stays blocked (a "drain") until all
buttons/triggers release, so the closing press never reaches the game.

**RPG-ONLY / NEVER COMBAT:** gated on `Nav.field_ready()` = not ui_muted AND minimap
visible. The minimap is the game's own free-roam signal (hidden in battle/cutscene/menu),
so R2 there is untouched and the pad is never blocked; force-closes if it goes false while
open.

**Main-vs-sub classification fix (2026-07-04):** a navi-guided objective has
`ATMapIconComponent.bShowMapIcon=false` (the navi arrow replaces the static icon), so
the bShowMapIcon-gated type readers (`icon_info`/`map_icon_type`) returned nil and the
navi path fell to its MAINQUEST default → EVERY tracked quest (incl. sub/secondary
story) announced as "main quest". Fix = `map_icon_type_any()` reads `MapIconType`
ignoring bShowMapIcon, used in list_targets section 1 AND `quest_pri`. Keep the gated
readers where icon VISIBILITY matters (minimap-list section 2, the dump).

**Target source = the game's own minimap** (`AT_UIMiniMapRadar.MapIconList` + active
`AT_UIMiniMapNaviIcon`), classified by **EMapIcon** (AT_enums.hpp) into groups
(quests/npc/fishing/gathering/shops/minigames/dragonball/other). Non-quest items are
distance-limited by each icon's own `UATMapIconComponent.SearchRangeRadius` (reflected
@0x104 — the game's reveal radius, matches the sighted player's view), else a 300 m cap;
quests unlimited. All world reads live in nav_tracker behind the radar's safety gates;
radar_menu.lua is pure input+speech.

**Radar behavior tweak:** a hand-picked (manual) target now STOPS on arrival (beacon
quiet + dropped, re-pick to resume); auto quest targets keep the re-arm behavior.
`Nav.stop_tracking()` is the explicit stop (B button).

Files: `src/input_bridge/{input_bridge.c,build.ps1}`, `input.lua`, `radar_menu.lua`,
nav_tracker additions, i18n `radar_cat_*`/`cat_*`/`nav_stopped`. NOT committed (pending
verify + the user's standing "no commit/push until I confirm" boundary).

**2026-07-06 batch (built, PENDING in-game verify):**
- **Collected filter:** collectibles list now drops actors with
  `AAccessPointBase.InteractState == 11` (EAccessPointState::State_Taken) — `bHidden`
  stays false on collected items, which is why they kept showing. Respawns revert to
  State_Wait so they re-list. InteractState exists ONLY on AAccessPointBase-derived
  classes; the collectible scan carries per-class capability flags (action/item/state)
  because probing it on `FieldActionPointActor` (plain AActor; Field Memories =
  `AATFieldMemoriesActor : AFieldActionPointActor`) would be the uncatchable abort.
- **Item names:** FieldActionPointActor speaks its reflected `ActionName` FString;
  PlacementObjectInfo speaks its drop-table id (`ItemTableComponent.FieldItemDropData`
  FixedId/NormalId) only when it reads as words (3+ letter run), else generic noun.
  `ItemTableComponent` is declared on APlacementObjectInfo ONLY (not
  AccessPointItemBase).
- **New categories:** `sites` (EMapIcon: Bonfire=64 cooking campfire, TRAININGROOM=25,
  TURTLESCHOOL=27, PRACTICE=15/16, DEVELOPMENT=17, TIME_MACHINE=19, MedicalRoom=45,
  etc.) and `enemies` (EVIL_ENEMY 30/34, BOSS 31/35, ENEMIES_BASE 32, AIRSHIP 36, GRD
  49/50/52). GROUP_ORDER: quests, collectibles, npc, enemies, sites, fishing,
  gathering, shops, minigames, dragonball, other.
- **Pickup chaining:** arriving at a manual target from CHAIN_GROUPS (collectibles)
  arms `chain_wait`; when that actor is actually collected (invalid / bHidden /
  State_Taken), the radar auto-targets the nearest remaining same-group item
  (`chain_to_next`). Walking >12 m away without collecting cancels. Picker passes
  `grp`+`stateful` through `Nav.set_manual_target(actor,key,label,grp,stateful)`.
- **Enemy proximity alert:** every scan tick, nearest minimap enemy icon within 50 m →
  spoken "enemy noun, direction word, above/below, N meters"; edge-gated per actor +
  8 s throttle; re-arms when area clear; re-announces same enemy at half distance.

**2026-07-06 batch 2 (PENDING verify):**
- **Enemies rework:** MapIconList does NOT carry roaming field enemies (icon category +
  alert stayed silent in play). Real source = `AAT_Character.SpawnType`
  (E_ENEMYSPAWN_TYPE @0xC78: 1 ENCOUNT, 2 QUEST, 3 BOSS; player/companions are 0).
  `enemies_list()` caches the FindAllOf scan (RESCAN_CLASSES ~10s); feeds the enemies
  category, the proximity alert, AND excludes enemies from Shift+F5 companions.
- **Chaining v2 (sweep):** all picker groups except quests chain. Advance to next
  nearest when the reached/tracked item is collected (invalid / bHidden /
  InteractState==Taken) OR the player walks >REARM_DIST away; `chain_seen` visited-set
  guarantees progress when the collected flag never flips; mid-approach pickup also
  chains (checked per tick on the tracked manual target). Fresh player pick resets the
  set; internal `set_manual_target(..., keep_sweep=true)` keeps it.
- **Sites additions:** non-memory FieldActionPointActors (train/meditate spots, e.g.
  Piccolo's waterfall) now go to the `sites` group labeled by their `ActionName`;
  wind tunnels = direct `FindAllOf("ATWindRoad")` scan (cat_windroad).
- **Ctrl+F5 dump** now probes collectible classes within 150 m (cls/bHidden/
  InteractState) — collect an item then dump to identify the real "collected" signal —
  and lists cached field enemies.

**2026-07-06 batch 3 (animals VERIFIED in-game; rest working per user reports):**
- **Wild animals in the picker (VERIFIED)**: `FindAllOf("AT_MobBase")` + fallback
  `"AnimalMob_Pawn_C"` → enemies group, noun `cat_animal` (deer/wolves/robots; NOT
  AT_Character — see [dbz-kakarot-engine-facts](dbz-kakarot-engine-facts.md)). Picker only, NOT the proximity
  alert (passive-animal spam). Used successfully for the deer-hunt quest.
- **Spoken distance everywhere**: every direction/elevation cue appends "N metros";
  distance-only cue after 8 s of silence (DIST_CUE_MS) so a straight approach still
  reports progress.
- **Stealth facing cues**: tracking any enemies-group target speaks "por detrás / de
  lado / de frente, te puede ver" on zone change (K2_GetActorRotation of the target,
  ±70° cone, STEALTH_CUE_MS 2.5 s) — built for "approach the deer from behind".
- **NPC names live from the game**: `game_character_name()` (GetCharacterName CDO
  call, cached) first; CPL_NAMES pruned to VERIFIED only (Cpl001 Goku, Cpl002 Gohan,
  Cpl009 Yajirobe, Cpl011 Piccolo). NPC scan covers `QuestCharacterBase_C` too.
- **Dump additions**: navi-target sibling listing (same-class actors, sorted by name),
  staged per-overload raycast probes, navmesh coverage map (recast + navvol pos/scale),
  verbose FindPath probe (path obj / points / IsPartial / GetDebugString).
- **Raycast avoidance is DEAD on this game** (all trace overloads = out-param abort);
  route guidance pending the town navmesh test. Straight beacon + spoken distances is
  the field baseline.

**2026-07-08 — enemy filter CORRECTED + map-close bug fixed (dumps captured in-game):**
- **SpawnType is NOT a valid enemy filter.** The Ctrl+F5 `dump_enemies.txt` proved roaming
  field enemies (`AT_Character_cpl059cXX_BP_C`) report **SpawnType 0**, same as the player
  (cpl002) and companions (cpl001/cpl011) — so the old `enemies_list()` filter (SpawnType
  1/2/3) returned nothing for them. New robust signal in `enemies_list()`: **IsA(AT_Character)
  AND NOT IsA(AT_CharacterPlayableBase)**. Player+companions carry `AT_CharacterPlayableBase`
  (excluded); QuestCharacter NPCs derive `AT_CharacterBase` NOT `AT_Character`, and townsfolk/
  animals are `AT_MobBase` — all drop out. SpawnType 1/2/3 still only NAMES the kind
  (encounter/quest/boss noun), default `cat_enemy`. `ENEMY_PROBE` set false (class confirmed).
  Class paths: `/Script/AT.AT_Character`, `/Script/AT.AT_CharacterPlayableBase`. Caveat: a
  non-playable friendly/AI story character would also match — refine if that shows up.
- **Map-close latch fixed** (`screen_map.lua`): the fullscreen map's pooled widgets
  (`Map_World_C`/`Map_M_C`/`Map_World_Curs`) keep reporting `onScreen=true` AFTER the map is
  dismissed (host visibility never changes; the cursor stays "alive"), which latched the map
  adapter active → `ui_muted()` stuck → radar (R3/explore/F3) wouldn't arm. Happened after
  normal close, fast travel, AND load-save. Fix = authoritative **free-roam cross-check**: if
  `AT_UIMiniMapRadar` (the minimap) is on-screen, the player is in gameplay so the map is
  CLOSED — a widget latch can't defeat it (dump: minimap onScreen=true coincided with latched
  map widgets; minimap=false only when a map is genuinely open). World/area map then detected
  by host `first_on_screen` (area map host is enum 3 when open, NOT 0).
- **Snap-to-areas is blocked:** the world/fast-travel map is an **analog cursor with NO
  selection index and no area-list array** (confirmed static RE + dumps). Snapping needs a new
  capability — a guarded `write_i32` in mem_bridge (if a writable cursor index is found by
  memory-diff) or an "inject stick" mode in input_bridge — neither built yet. Fast-travel
  destinations exist as `FFastTravelDestinationTableRow{DestinationLocationX/Y/Z}`.

**2026-07-10 — new "Caza"/Hunt category + NPC name fallback (PENDING in-game verify; Lua
only → Ctrl+Shift+R, no restart):** user reported wild animals landed in **Enemies** and
NPCs read the raw **"Cpl013"** code. Fixes in `nav_tracker.lua` + `i18n.lua`:
- New group **`hunt`** in `GROUP_ORDER` (after `enemies`), label `radar_cat_hunt` =
  "Caza"/"Hunt". Wild animals (`AT_MobAnimalBase` scan, list_targets §5b) now add to `hunt`
  instead of `enemies`; the prey **map markers** moved there too — `ICON_GROUP[7]`
  (EMapIcon 7 hunt, noun `cat_hunt`="presa") gathering→hunt, and `FieldPointComponent`
  type 3 (Dino spot) gathering→hunt. Animals still spoken by species (`animal_type_N`).
- The aim "on target" + stealth facing cues (nav_tracker ~L1469/L1483) were gated on
  `grp == "enemies"` — extended to `enemies OR hunt` so hunting keeps those cues.
- **Name resolution deduped** into `resolve_char_id(raw)` (game `GetCharacterName` →
  strip `c%d+$` suffix → retry → CPL_NAMES); both `enemy_display_name` (CharacterName) and
  `npc_name` (UniqueId) call it. `npc_name` no longer speaks a bare `^%a+%d+$` internal
  code (Cpl013/Npc0NN) → generic "personaje" noun when the game has no real name. Real
  names still depend on `GetCharacterName` actually knowing the id (unverified per-id).

**2026-07-17 — SMART RADAR: objective auto-track + gathering chain fixes (PENDING in-game
verify; Lua only → Ctrl+Shift+R):** user request — use the R3 menu as little as possible.
- **Objective auto-track:** `quest_objective.lua` now keeps per-group (main/sub) objective
  SIGNATURES (title + `Txt_List_00` lines, counters EXCLUDED so "2/5" ticks don't fire;
  stored sigs survive the HUD hiding in battle → same text re-appearing ≠ change; first
  observation only seeds) and fires `Quest.set_on_change(kind)` → wired in app.lua to
  `Nav.notify_objective_change(kind)`. nav_tracker arms a one-shot preemption (`preempt`
  table: TRIES=10 scans TTL; `best_candidate(px,py,pz,want_pri)` prefers the changed
  quest kind's marker): the auto-scan may then steal a MANUAL pick / suppressed state /
  pending resume ONCE, stashing what the player was doing in `preempt.stash`. **B in the
  R3 menu restores the stash** (chain_to_next for chainable groups, exact re-acquire
  otherwise; B again = real stop). Explicit pick / F3 disarm.
- **Gathering chain fixes** (user: collected a spot, radar never advanced): (1) icons in
  gathering/collectibles whose class name matches AccessPoint*/MiningPoint*/
  PlacementObjectInfo (all < AAccessPointBase per the CXX dump sweep) are marked
  `stateful` → sweep advances on InteractState=Taken AND taken points drop from the
  picker; (2) fruit/small-fish spots are `ASpawnerFruitVolume`/`ASmallFishSpawnVolume`
  (< ASpawnerItemVolumeBase, volumes, NO taken state) → gathering picks use the WIDE
  8 m ARRIVE_DIST (was the 1.5 m interact radius — origin sits meters from the orbs, so
  arrival never fired and the sweep never armed).
- **Lua 200-local cap hit in nav_tracker's main chunk** (the file was exactly at it):
  new state must go into tables (`preempt`, `RADAR_CAP.npc/.map` merged) or inside
  functions — precedent comment at the compute_route constants.

**2026-07-15 — battle-interruption resume (`resume_pick`, PENDING in-game verify):** user
bug — a fight mid-trip dropped the manual pick (world gate `WORLD_DROP_TICKS` ~5 s) and
the auto-scan then re-tracked the STORY marker. Fix in `nav_tracker.lua`: forced drops
(world gate, map transition) stash `{key,label,grp,stateful}` as plain Lua data (safe
across teardown); when the world returns, re-acquire by category+key through
`Nav.list_targets()` (every 3rd scan, 10 tries) and hand back to `set_manual_target`
(keep_sweep). The quest auto-scan is gated off while a resume is pending. Deliberate
stops clear it (B / F3-off / a new pick). Companion targets (grp=nil) not resumable.
