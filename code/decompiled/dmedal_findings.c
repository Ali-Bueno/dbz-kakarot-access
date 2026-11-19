// D MEDAL (DMedalTreasure_BP_C < ATreasureAccessPoint) taken-state findings, static RE 2026-11-19.
// Raw evidence: dmedal_raw.c (runs 1-5). Probe: code/ghidra/dmedal_probe.java.
//
// ATreasureAccessPoint::BeginPlay = FUN_1410cb580 (vtable slot @0x143ca5218; first call FUN_142b0bd40 =
//   AActor::BeginPlay, referenced by ~200 vtables). Order:
//   1) Super::BeginPlay (BP ReceiveBeginPlay fires here, BEFORE the taken check)
//   2) this+0x570 = TreasureManager (FUN_140d2b8e0); APPEND this to Manager.TreasureAccessPointList (+0x128/+0x130)
//      -> taken medals ARE registered in the list.
//   3) taken = FUN_1410d6320(GameInstance+0x1ea0 ->+0x100 +0x55350 (= FATSaveGameTreasure), TreasureCategory@0x4D4,
//      SaveDataIndex@0x4A8)
//   4) if taken: TreasureState@0x4D6 = 11; register event component (+0x400, "TreasureAccessPoint"); RETURN.
//      Skipped for taken: FUN_1410d5ab0 (spawns search aura / trail particles into +0x500 etc.),
//      FUN_1410c9750 (binds event-system delegates 0x3D/0x3E -> FUN_1410da8f0/FUN_1410da7f0 = search listeners),
//      root collision-profile cache (+0x568 -> +0x3B8), FUN_140d18360(+0x400).
//   No DestroyActor / SetActorHiddenInGame on this path. Actor survives with TreasureState 11.
//
// IsTaken FUN_1410d6320(save, cat, idx): cat 0 (DMedal), idx<500 -> byte save[idx + 500*k], k picked by GLOBAL
//   DLC predicates (FUN_140d2fa30=DLC3 +500, f960=DLC4 +1000, f970=DLC5 +1500, f9d0=DLC6 +2000, f9e0=DLC7 +2500,
//   f9f0=DLC8 +3000, none = base DMedal). cat 1 (EventItem) idx<700 -> +0xDAC / DLC7 +0x1068 / DLC8 +0x1324.
//   cat 2 (Insect) -> always 0 (never "taken" via save). Predicates are FUN_14107d9f0(DAT_14592ffxx): world/content
//   state, not actor data. Writer (on collect) FUN_1410d9e10 mirrors the same mapping, then FUN_1410d9b00
//   (grant ItemTableComponent items, add to Manager+0x110, SE) and sets 0x4D6 = 11.
//
// OnEnableItem(bool) impl FUN_1410d8790 (exec 0x141910550): cached at +0x578 (ctor FUN_1410c72b0 sets -1).
//   false: root/collision comp (+0x340) vfunc+0x5E8 (SetCollisionProfileName, DAT_145c68c10) ; SetVisibility+SetActive
//   (FUN_140d20b20) false on +0x500 aura, +0x518 item particle, +0x4E0 skeletal mesh. true: restore profile +0x3B8.
// UTreasureManager::OnChangePhase FUN_1410d7a80 (exec 0x141914390): for every list entry
//   OnEnableItem(QuestWindowValid(FUN_1410cdfd0) && !Manager+0x108 && TreasureState != 11).
// Re-enable-all FUN_1410d3cf0 skips state 11; lock-all FUN_1410d3be0 sets bit in +0x4A0 and disables.
// UTreasureManager::OnChangeProgress -> FUN_1410ceb30: rebuilds per-area (0x5E=94 areas) counters from the
//   TreasureAccessPointSaveTable (FUN_1410d5080(mgr,0)), world-level, independent of which actors are streamed in.
