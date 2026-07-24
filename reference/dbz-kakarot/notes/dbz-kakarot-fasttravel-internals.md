# dbz-kakarot-fasttravel-internals

> DBZ Kakarot map internals — world map (AT_UIMapWorld) instant fast-travel DONE; area map (AT_UIMapM) POI reader DONE; radar-from-area-POIs = NEARBY-ONLY by user decision (2026-07-10); world-map icon lazy-spawn wake + 20ms d-pad loop shipped (pending in-game verify)


## CRITICAL (2026-07-09): the player's world map is `UAT_UIMapWorld`, NOT the FastTravel* system
Live class chains (from the F7 fast-travel probe added to discover.lua): host `Map_World_C < AT_UIMapWorld < ATUIUserWidget`; icons `Map_World_Icon_C < AT_UIMapWorldIcon < CFUIUserWidget`; cursors `AT_UIMapWorldCurs`/`AT_UIMapWorldCurs01`. This is a DIFFERENT system from `UAT_UIFastTravelMap`/`UFastTravelManager`/`FFastTravelDestinationTableRow` (that one is likely demon-realm/DLC — `FindAllOf("FastTravelManager")`/`FindFirstOf` found NO live instance on the world map; the actual warp MAY still route through fast-travel code internally, but the mod's ENTRY is AT_UIMapWorld).

`UAT_UIMapWorld` (AT.hpp ~34192) — REFLECTION-CALLABLE UFUNCTIONs (the clean path):
`InputConfirm()`, `InputCancel()`, `InputUp/Down/Left/Right()`, `InputX()`, `InputY()`,
`MouseClickDecide(FGeometry,FPointerEvent)`, `CheckHovered()`, `OnCursorMove(...)`, `OnClosedInfoWindow()`.
Members: `Map_World_Curs`/`Map_World_Curs01`, `Pnl_Icon_Map` (UCanvasPanel), `MapInfoCtn` TArray<UAT_UIMapListInfo*>,
`WorldMapActiveParam` (UDataTable → `FWorldMapActiveParamRow{StageId(EWorldStageId), Start/End Episode/Chapter/Part, IsIconChange}` — area activation by STORY progress), `WorldMapSymbolPos` TMap<EWorldStageId,FVector2D>, `MouseMoveFactor`, `LoadMapIconCtn`. Class size 0x748; last reflected member ~0x568 → the hovered-selection index is likely a non-UPROPERTY tail member (0x568..0x748); the subagent is decompiling InputConfirm/CheckHovered to find it (cf. community board hoveredCache@0x5D8). PLAN for instant travel: write the hovered icon/index + call `InputConfirm()` via reflection → travel with NO cursor move (reflection-only, no memory teleport).
Icon `UAT_UIMapWorldIcon` (~34295): `Img_Micon25`, `Ins_Icon_Item`, `Map_Icon_Switch` (UAT_UIMapIconSwitchImproved = lock/state switch → likely the greyed/locked signal), `ImageCtn`, `TextBoxCtn`; BP `Map_World_Icon_C` adds a `Txt_Name` widget (destination name — read via GetText, WORKS). NOTE: blind ForEachProperty on the icon class ABORTS uncatchably (truncated the probe) — read named fields only.

## ✅ IMPLEMENTED & VALIDATED (2026-07-09)
In-game test (discover.lua F7, since removed): writing `selIndex(0x514)=N` then reflection-calling
`host:InputConfirm()` **travels to point N with NO cursor movement** — opens the "¿Ir a X?" YesNo for the
right point. Index map confirmed: **0=Casa de Goku, 1=Ciénaga Blake** (InfoIcon native order). The
world-map screen is reached via Select → map menu (Mapa del Área, `Map_M_C`) → Triángulo/Y → **Mapa del
Mundo** (`Map_World_C`/AT_UIMapWorld). state was **6** there (accepted), ready=1, and **selIndex tracks the
hovered point and HOLDS while the cursor is idle** (stable 13s in the diagnostic — CheckHovered updates on
cursor MOVE, not every frame).
**Shipped in `screen_map.lua`**: on the world map, d-pad up/down picks a point by native InfoIcon index
(name via matching InfoIcon[i] icon ptr @entry+0x08 to a live Map_World_Icon_C's Txt_Name), writes
`selIndex`, announces "encima de X, pulsa confirmar"; re-pins selIndex each tick + on A also calls
InputConfirm() so the game's own Confirm targets the chosen point. NO stick injection / no cursor math.
OPEN: `entry+0x11` read 0 for both (accessible) points → NOT the unlock flag as the subagent guessed;
currently rely on "InfoIcon = points the game renders" (undiscovered not shown) for the unlock filter —
revisit if an undiscovered/greyed point ever appears in the list. Also the d-pad-vs-stick pin can fight if
the user moves the analog stick (edge case; accessible flow is d-pad-only).

## AREA MAP + RADAR-FROM-MAP (2026-07-09, commit 6348d90 on main)
The **Area Map** (Mapa del Área, Select) = `UAT_UIMapM` / BP `Map_M_C` (native parent AT_UIMapM). The
**World Map** (Mapa del Mundo, Triángulo from the area map) = AT_UIMapWorld (above). Flow: gameplay →
Select → Mapa del Área → Triángulo/Y → Mapa del Mundo.
- **Detection fix (DONE):** the AREA map does NOT hide the minimap radar (the WORLD map does), so the old
  "minimap up ⇒ map closed" gate preempted it → nothing read. Now `is_active` checks the area map FIRST by
  its own signal (`Map_M_C` on_screen = enum 3 open / enum 1 closed) before the minimap gate. WORKING.
- **POI-under-cursor reader (DONE, WORKING):** `Map_M_C.FocusTarget` (reflected) = the focused POI icon
  (UAT_MapIconSwitchBase); `.Target` = the world actor. Announce type (`Nav.icon_noun(actor)`, new public
  fn reusing the radar's EMapIcon→noun) + name (`WL_MapDetailTxt.WL_Detail_Txt`). Limited: only reads what
  the analog cursor happens to focus (user got only 2 of ~44 POIs by sweeping).
- **Radar sources area POIs directly (DONE, streaming-limited):** nav_tracker list_targets now scans
  `FindAllOf("ATMapIconComponent")` → Owner + MapIconType (shops t=1/2/3/10, fishing 5, fruit 6 ×19,
  gathering, minerals 8, training 15, animals 7) and enemy bases via `UATEnemiesBaseBehaviour` /
  `ATExterminationBastionComponent` component (→ type 32; `BP_EnemiesBaseRR_C` has NO ATMapIconComponent).
  Class/type breakdown of one area's 44 POIs captured 2026-07-09.
- **DEAD ENDS:** `UAT_UIMapManager.MapIconInfoCacheList` is EMPTY in free-roam (count=0). Snapshotting the
  area map's `WL_MapIconList[].Target` while open works (got 44) but needs opening the map each area (user
  rejected the toggle) — code was added then removed.

### DECIDED 2026-07-10 + shipped this session (pending in-game verify)
1. **Streaming decision RESOLVED: NEARBY-ONLY** (user: "vamos a dejar solo cercanos") — matches the
   sighted minimap; no position-cache, no coordinate targeting. RADAR_MAP_CAP=1000m, RADAR_NPC_CAP=300m.
2. **World-map "stick nudge" bug ROOT-CAUSED + FIXED:** dump_map_state 2026-07-09 shows Map_World_Icon_C
   widgets are created LAZILY — 0 instances at open until the game sees a cursor move — so the icons gate
   kept is_active false and the d-pad dead until the user nudged the stick. Fix in screen_map.lua:
   arm a wake window when the AREA map closes WITHOUT the minimap returning (only route to the world map;
   minimap-seen cancels ⇒ combat can't be reached inside a window), then micro `Input.inject(0.5,0)` pulse
   until icons materialize. Plus **d-pad now runs on its own 20 ms LoopAsync** (radar_menu pattern,
   `Map.start/stop` wired in app.lua) with a `Registry.active_adapter()==Map` guard (else the d-pad would
   keep retargeting UNDER the travel YesNo dialog, where the registry stops polling Map.is_active).
3. **VERIFY (still open):** enemy bases appear in the R3 radar (UATEnemiesBaseBehaviour scan never
   confirmed in-game). If not, scan `BP_EnemiesBaseRR_C` by class.
4. Still-on temp diagnostic: `MAP_DEBUG=true` in screen_map — now also verifies the icon-wake; turn off
   once the combat false-positive AND the wake are confirmed.

## CONFIRM MECHANISM (Ghidra, high confidence, 2026-07-09 — subagent)
`UAT_UIMapWorld::InputConfirm` (exec 0x141798670) and `MouseClickDecide` (0x141798850) both funnel into
the **confirm core `FUN_1415c0890`** (code/decompiled/manual_1415c0890.c). It requires ALL of:
`state(this+0x508) ∈ {6,0xc}` AND `this+0x51d != 0` (ready) AND `this+0x514 != -1` (a selected index) —
else error beep / no-op. Then it reads `InfoIcon[this+0x514]`, sets state=0xe, opens the **"WM_MoveConf"
YesNo**; the warp runs on Yes (state-0xe handler), NOT inside the core.
- `this+0x514` int32 = **selected InfoIcon index** (-1 none). Setter `FUN_1415cf800(this,idx)`.
- `this+0x508` int32 = mode: **6=area-select, 0xc=destination-select, 0xe=warping, 9/10=info window**.
- `this+0x51d` u8 = confirm-ready gate.
- **`InfoIcon` UPROPERTY** @0x4D0 data / 0x4D8 count, `TArray<FWorldMapsSymbol>` stride 0x20: entry+0x08
  icon widget, **entry+0x11 u8 = UNLOCKED flag (0=locked)**, entry+0x14 area id, entry+0x1c int id.
- d-pad Left/Right = `FUN_1415c0e30(this,2|3)` — only at state 0xc, skips locked (entry+0x11==0).
All offsets are in `native_offsets.mapWorld`. Files: code/decompiled/manual_1415c0890.c,
manual_1415cf800.c, manual_1415c0e30.c, manual_1415b9720.c (CheckHovered), _mapworld_index.txt,
_mapworld_fasttravel_findings.txt; scripts code/ghidra/find_mapworld.java.

**INSTANT-TRAVEL RECIPE (implement this):** at state 0xc, pick index N with `InfoIcon[N]+0x11 != 0`
(UNLOCKED — mandatory, core does NOT re-validate), write `this+0x514 = N` (mem_bridge write_i32 — hidden
member), ensure `this+0x51d != 0`, reflection-call `InputConfirm()` (param-less → SAFE on this game),
answer Yes on the "WM_MoveConf" YesNo. Reflection-only alt for stepping: `InputLeft()`/`InputRight()` at
state 0xc. IN-GAME BARE-CALL TEST (2026-07-09): calling InputConfirm/InputUp/etc. with the cursor on an
AREA (state 6, selIndex -1) = callable=true but NO effect (confirms the guards). NEED to test: reaching
state 0xc, and set-0x514 + InputConfirm reproducing the YesNo. Host live addr via `Mem.addr(Map_World_C
instance)`.

--- everything below is the SEPARATE FastTravel* map system (kept for reference; may be the warp backend) ---

Fast-travel internals for the DBZ Kakarot accessibility mod, recovered 2026-07-09 from the
game's **CXX header dump** at `D:\games\steam\steamapps\common\DRAGON BALL Z KAKAROT\AT\Binaries\Win64\CXXHeaderDump\{AT,ATExt,AT_enums}.hpp`
(Ctrl+H, generated 2026-07-01 — I can read it directly, no need to regenerate). Builds on
[dbz-kakarot-map-travel-and-pending](dbz-kakarot-map-travel-and-pending.md), [dbz-kakarot-engine-facts](dbz-kakarot-engine-facts.md), [dbz-kakarot-decompilation-setup](dbz-kakarot-decompilation-setup.md).

## The RenderTransform-write dead-end (verified, do NOT retry)
Writing the world-map cursor's `RenderTransform.Translation` (UWidget+0x90/+0x94, added
`mem_bridge` write_* funcs + `native_offsets.mapCursor`) **holds in memory (dump_map_snap: held=true)
but only moves the VISUAL sprite**. The authoritative cursor is an internal WORLD/map-space variable
(drives both the map PAN and the fast-travel hit-test); the widget RenderTransform is just its visual
reflection. So fast travel stayed on the old point ("se queda en el mismo punto"). The stick-injection
drive (goes through the game's real input) IS functionally correct — reverted screen_map to it.
Unlike the community board, where UWidget+0x90/0x94 IS authoritative, on the world map it is not.
`mem_bridge` now has guarded write_i32/u32/i64/float/double (kept as infra; SEH-guarded like reads).

## Fast-travel data model (CXX)
- **`UFastTravelManager`** (UActorComponent): `CurrentTargetComponent` (UFastTravelComponent*, the
  destination currently pointed at), `UiMap` (UAT_UIFastTravelMap*), `FtParam`
  (UATDataAssetFastTravelParameter*). **No reflected UFUNCTIONs** → execute logic is native C++.
- **`UFastTravelComponent`** (UActorComponent): `PlatformId` (FName) = a destination's id in the world;
  `DemoTargetPoint`.
- **`UAT_UIFastTravelMap`** (the world map widget; BP is Map_World_C): `DestinationIconList`
  TArray<FDestinationIconInfo{Icon: UAT_UIFastTravelMapDestinationIcon*, Slot: UCanvasPanelSlot*}> @0x5E8;
  `WL_Cursor` (UAT_UIFastTravelMapCursor), `WL_Text_DestinationName`, `WL_Text_AreaName`;
  **`CursorSnapThreshold`** FVector2D (the GAME snaps the cursor onto a destination within this — so the
  stick drive only needs to get CLOSE), `CursorMoveSpeed`, `CursorCollisionSize`; `Pnl_Icon/Sub/Main/Rumour`;
  `DestinationOriginalTexture` vs **`DestinationGreyoutTexture`** (LOCKED destinations render GREYED,
  NOT hidden → the current travel_points() may list locked ones).
  - Icon widget `UAT_UIFastTravelMapDestinationIcon`: `WL_Text_Name`, `WL_Icon_New`, `WL_Icon_FastTravel`.
    (BP name in the widget tree = `Map_World_Icon_C`; cursor BP = `Map_World_Curs_C` + a second `Map_World_Curs01_C`.)
- **`FFastTravelDestinationTableRow`** (FTableRowBase): `Name` (FName id), `AreaID` (EAreaIndex),
  unlock conds `CondEpisode`/`CondProgress`/`CondPhase`/`CondQuest`, world transform
  `DestinationLocationX/Y/Z` + `DestinationRotationZ`, `SkyseedFallHeight`, `UnlockByPlatformId`.
- **Unlock state** `FATSaveGameFastTravel` (ATExt.hpp): **`PlatformAccessedList` TArray<FName>** = the
  accessible/unlocked destination ids (filter the list against this to respect gameplay);
  `DestinationIconCheckedList`, `DestinationIconUnlockedCount`, `IsCompletedSkyseedBattleList`.
- Debug funcs exist (`ATDebugUnlockAllFastTravelDestinations`, `ATDebugOpenFastTravelMap`) — NOT for
  shipping (they cheat-unlock).

## Consequence for the "direct function" path (user chose this)
There is **no reflected execute UFUNCTION** → truly-instant travel needs either (a) Ghidra to find the
native execute fn (I have the exe at code/AT-Win64-Shipping.exe + Ghidra) and a native trampoline bridge
to call it, or (b) in-game Live View discovery of a callable path (e.g. set CurrentTargetComponent +
trigger). Pragmatic ship WITHOUT native RE = pure Lua: (1) **unlock-filter** the list via
PlatformAccessedList (or greyout-texture/enabled state), (2) **auto drive+confirm** — keep the working
stick drive (game's CursorSnapThreshold helps it land) and auto-inject the confirm button after arrival,
so picking a destination becomes hands-free. User's hard rule: NEVER list or allow travel to a
non-discovered zone.
