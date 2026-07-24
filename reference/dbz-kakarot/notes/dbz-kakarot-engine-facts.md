# dbz-kakarot-engine-facts

> DBZ Kakarot engine/install facts — UE 4.21, exe path, modding framework UE4SS


Target game for the accessibility mod: **DRAGON BALL Z KAKAROT** (Bandai Namco / CyberConnect2).

- Install path: `D:\games\steam\steamapps\common\DRAGON BALL Z KAKAROT` (Steam library is at `d:\games\steam`).
- Engine: **Unreal Engine 4.21** (confirmed by reading `UE4+Release-4.21` from the shipping exe; paks use the UE4 `WindowsNoEditor` naming).
- UE project name: `AT`. Real executable: `AT\Binaries\Win64\AT-Win64-Shipping.exe` (~99 MB). The top-level `AT.exe` is a launcher wrapper.
- Content: `AT\Content\Paks\pakchunk0-WindowsNoEditor.pak` + `pakchunk1` (~14 GB total, packaged, no editor).
- Modding framework: **UE4SS** (supports UE 4.12+, so 4.21 works). UE4SS goes in `AT\Binaries\Win64\`. See [dbz-kakarot-accessibility-plan](dbz-kakarot-accessibility-plan.md).
- As of project start (2026-06-30): UE4SS NOT yet installed; no PRISM/Tolk binaries present.

**Reflection rules learned the hard way (2026-07-06):**
- **Reflected UFunction calls with OUT-params ABORT UNCATCHABLY** on this game/UE4SS build (pcall can't catch): `KismetSystemLibrary.LineTraceSingleForObjects` AND `LineTraceSingle` (FHitResult& out), `K2_ProjectPointToNavigation` (FVector& out) all killed the Lua callback mid-flight. Use the non-out-param overloads (`ProjectPointToNavigation` returning FVector — NOTE it returns the INPUT unchanged on failure) or native mem_bridge reads. Raycast obstacle avoidance is therefore DEAD from Lua on this game.
- **`FindAllOf` on a NATIVE base class can return NOTHING while the blueprint `_C` subclass name works** (bit us twice: AT_UICommunityBoard, then QuestCharacter vs `QuestCharacterBase_C` — Yajirobe invisible to the NPC scan). Always scan both names and dedupe by address.
- **`FindAllOf("PlayerController")`'s FIRST instance is often the WRONG controller** (2026-07-15, cost the whole ui_directory HUD branch): in the field it returned `TwinFootController_0` (the ride/mount PC, `MyHUD` null) and at the title `ATTitleController` (plain `HUD`). To get the real game HUD, iterate ALL candidates and pick the one whose `MyHUD` exposes `UIFieldManager` (i.e. is the `AAT_GameHUD`) — see `ui_directory.find_hud`. `FindFirstOf("PlayerController")` for a mere pawn read is fine (nav_tracker), but never assume it's THE player's controller.
- **NPC display names**: `UAT_BlueprintFunctionLibrary::GetCharacterName(FString ID) -> FString` (static, call on CDO `/Script/AT.Default__AT_BlueprintFunctionLibrary`) resolves UniqueId→localized name — the authoritative source; never guess Cpl0NN mappings (the guessed table announced Trunks/Goten for wrong characters).
- **Wild field animals/mobs are NOT AT_Character**: separate tree `A0NN_BP_C < AnimalMob_Pawn_C < AT_MobAnimalBase < AT_MobBase < NpcPawn`. Position/bHidden reads only (no SpawnType/InteractState there).
- **NavMesh**: maps ship 3 agent meshes (RecastNavMesh-Human/-Dinosaur/-Animal) + ~47 NavMeshBoundsVolumes, but `FindPathToLocationSynchronously` returns a valid-but-EMPTY path (0 points, "Invalid path") everywhere tested in Area04 field, even pinning each mesh actor as PathfindingContext. Pending: test inside a town (NPCs pathfind there, so tiles must exist someplace).
- **CXX header dump** lives at `<game>\AT\Binaries\Win64\CXXHeaderDump\` (AT.hpp, AT_enums.hpp, NavigationSystem.hpp, UMG.hpp…) — grep it BEFORE guessing any member/signature.
