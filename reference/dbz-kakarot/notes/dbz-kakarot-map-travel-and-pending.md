# dbz-kakarot-map-travel-and-pending

> DBZ Kakarot world-map fast-travel d-pad guidance (stick injection) + the open TODO list as of commit a566c63 (2026-07-09)


State of the DBZ Kakarot accessibility mod as of **commit a566c63 (pushed to main, 2026-07-09)**.
Builds on [dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md), [dbz-kakarot-radar-picker](dbz-kakarot-radar-picker.md), [dbz-kakarot-engine-facts](dbz-kakarot-engine-facts.md).

## World-map fast travel (built this session)
- Fast travel = **analog cursor + a confirm dialog** (`Xcmn_Win01_C`: "¿Ir a X en <área>?" + Sí/No).
  There is **NO list menu and no selection index** (`LAUNCH_FAST_TRAVEL_MENU` is just the confirm).
- Destinations are `Map_World_Icon_C` widgets, each with `.Txt_Name` (point name, e.g. "Casa de Goku").
- **Positions (verified 2026-07-09, screen pixels, same space):** cursor = the live `Map_World_Curs`
  (under `BP_ATGameInstance`) `.RenderTransform.Translation` (X,Y); icons render at 0,0 so their pos is
  `ic.Slot.LayoutData.Offsets.Left/Top`. When the cursor sits on an icon, CURS render == ICON slot.
  Icons/cursor MOVE as the map pans — read fresh each tick.
- **Feature (`screen_map.lua`):** open world map → announces reachable points; **d-pad up/down picks a
  destination**, the mod **drives the analog cursor onto it via LEFT-STICK injection**, then says
  "encima de X, pulsa confirmar" → player presses X. Constants: `ON_POINT_PX`, `DRIVE_FAR/NEAR`.
- **`input_bridge` gained `inject(lx,ly)` / `inject_off()`** (input.lua wraps them): drives ONLY the left
  stick, with a self-expiring TTL (~12 frames) so the stick can NEVER stay stuck if the Lua driver stops.
  Needs `hooked=true`. **Requires a game RESTART to load (native DLL).**

## Map open/close detection (the recurring hard part — current rule)
The pooled map widgets LATCH `onScreen=true` after close (host `Map_World_C` is permanently visible;
`Map_World_Curs` stays "alive"). Rules that WORK (dumps 2026-07-08/09):
- **Minimap = the free-roam signal:** `AT_UIMiniMapRadar` on_screen ⇒ in gameplay ⇒ map CLOSED (authoritative).
- **Area map** open = `Map_M_C` host on_screen (it is enum **3** when open, NOT 0 — old `enum==0` gate meant it was never detected).
- **World map** open = ≥1 `Map_World_Icon_C` **on_screen** (NOT the always-on host, and NOT just
  "minimap hidden" — the minimap is ALSO hidden in COMBAT, which false-positived the world map mid-fight
  and injected the stick; gating on real rendered icons fixes it). `world_icons_on_screen()`.

## OPEN TODO (updated 2026-07-10 — items 2/4/5/6 SHIPPED, pending in-game verify)
1. **VERIFY the map combat false-positive is dead** (`world_icons_on_screen` gate). `MAP_DEBUG=true` still on
   in screen_map. If it still fires in combat, get `dump_map_state.txt`.
2. ~~Enemies detected as animals~~ **SHIPPED 2026-07-10:** enemies_list already classified all non-playable
   AT_Character as enemies; NOW they also get their REAL NAME — `AAT_Character.CharacterName` (reflected
   FString @0x9E8) → `GetCharacterName` resolver (`enemy_display_name` in nav_tracker; retries without the
   `cNN` variation suffix). Animals speak their SPECIES: `AT_MobAnimalBase` pawns' NpcComponent is a
   `UAnimalComponentBase` with reflected `AnimalType` (E_ANIMAL_TYPE, AT_enums.hpp:10648: 1 flying dragon,
   2 deer, 3 chicken, 4 migratory bird, 5 small bird, 6 dinosaur, 7 wolf, 8 dog, 9 small dinosaur) →
   i18n `animal_type_N` keys. Non-animal AT_MobBase (StandStaticMob_C townsfolk) NO LONGER radar targets.
   VERIFY in game: enemy names resolve (CharacterName might be empty on some), species spoken.
3. **Wind tunnel (túnel de viento) not in radar:** `FindAllOf("ATWindRoad")` + FieldPointComponent type 2
   both find nothing — class name/type probably wrong (unverified batch). A probe (ATWindRoad + all
   FieldPointComponents with type/owner/dist) is in Ctrl+F5. Get the dump AT a wind tunnel, find the
   real class, fix. User also wants a **proximity cue** for wind tunnels/sites (only enemies alert today).
4. ~~Map responsiveness~~ **SHIPPED 2026-07-10:** world-map d-pad on its own 20 ms LoopAsync + lazy-icon
   wake nudge (see [dbz-kakarot-fasttravel-internals](dbz-kakarot-fasttravel-internals.md)). VERIFY: d-pad works immediately on opening the
   world map with NO manual stick nudge.
5. ~~Instant cursor snap~~ **DONE earlier** (selIndex 0x514 write + InputConfirm — no cursor movement at all).
6. ~~Animals by name~~ → shipped as SPECIES via AnimalType (see 2). If the user wants finer names
   (per-variant), pawn also has reflected `EAnimalID AnimalID` @0x440 (52 ids, AT_enums.hpp:1436).
7. **Turn OFF the temporary diagnostics** once verified: `MAP_DEBUG` (screen_map), the wind + mob probes in
   `Nav.dump`. `ENEMY_PROBE` already false.

## Confirmed working (user)
Ctrl+M toggle fix, elevation cue, map open/close (normal), fast-travel d-pad drive ("funciona"),
map combat false-positive fix pending re-verify.
