// UAT_UIXcmnAgreement (size 0x5e8) ctor + register-natives + exec impls + callees.
// Members: RootPanel 0x398, FadeBlack 0x3a0, SubjectImage 0x3a8, HeadImage 0x3b0,
//   LeftArrow 0x3c0, RightArrow 0x3c8, AppearAnim 0x3d0, DisappearAnim 0x3d8, Map 0x508.
// ======== SEED  @ 0x1417be950 ========

void FUN_1417be950(void)

{
  undefined8 uVar1;
  
  uVar1 = FUN_1417bd0d0();
  FUN_141f2d810(uVar1,&PTR_s_CheckHovered_14407d8a0,5);
  return;
}



// ======== SEED  @ 0x1417bd0d0 ========

longlong FUN_1417bd0d0(void)

{
  if (DAT_1459b2700 == 0) {
    FUN_141f291f0(L"/Script/AT",L"AT_UIXcmnAgreement",&DAT_1459b2700,FUN_1417be950,0x5e8,0x10000000,
                  0,L"Engine",FUN_1417be1d0,FUN_1407e4520,_guard_check_icall,&LAB_14176a290,
                  &LAB_14083c250,0);
  }
  return DAT_1459b2700;
}



// ======== SEED  @ 0x1417be1d0 ========

void FUN_1417be1d0(longlong *param_1)

{
  if (*param_1 != 0) {
    FUN_1416da7a0(*param_1,param_1);
    return;
  }
  return;
}



// ======== SEED  @ 0x1407e4520 ========

undefined8 FUN_1407e4520(void)

{
  return 0;
}



// ======== WIN  @ 0x1417bd930 ========

longlong FUN_1417bd930(void)

{
  if (DAT_1459b26f8 == 0) {
    FUN_14203a4b0(&DAT_1459b26f8,&PTR_FUN_14407acf0);
  }
  return DAT_1459b26f8;
}



// ======== WIN  @ 0x1417bd960 ========

longlong FUN_1417bd960(void)

{
  if (DAT_1459b26d0 == 0) {
    FUN_14203aad0(&DAT_1459b26d0,&PTR_FUN_1454f0410);
  }
  return DAT_1459b26d0;
}



// ======== WIN  @ 0x1417bd990 ========

longlong FUN_1417bd990(void)

{
  if (DAT_1459b26d8 == 0) {
    FUN_14203aad0(&DAT_1459b26d8,&PTR_FUN_1454f0450);
  }
  return DAT_1459b26d8;
}



// ======== WIN  @ 0x1417bd9c0 ========

longlong FUN_1417bd9c0(void)

{
  if (DAT_1459b26e0 == 0) {
    FUN_14203aad0(&DAT_1459b26e0,&PTR_FUN_1454f0510);
  }
  return DAT_1459b26e0;
}



// ======== WIN  @ 0x1417bd9f0 ========

longlong FUN_1417bd9f0(void)

{
  if (DAT_1459b26e8 == 0) {
    FUN_14203aad0(&DAT_1459b26e8,&PTR_FUN_1454f0550);
  }
  return DAT_1459b26e8;
}



// ======== WIN  @ 0x1417bda20 ========

longlong FUN_1417bda20(void)

{
  if (DAT_1459b26f0 == 0) {
    FUN_14203aad0(&DAT_1459b26f0,&PTR_FUN_1454f0590);
  }
  return DAT_1459b26f0;
}



// ======== WIN  @ 0x1417bda50 ========

longlong FUN_1417bda50(void)

{
  if (DAT_1459b2598 == 0) {
    FUN_141f291f0(L"/Script/AT",L"AT_UITPSSkill",&DAT_1459b2598,_guard_check_icall,0x48,0x10000000,0
                  ,L"Engine",FUN_1417bddf0,FUN_1407e4520,_guard_check_icall,&LAB_141740240,
                  &LAB_14083c250,0);
  }
  return DAT_1459b2598;
}



// ======== WIN  @ 0x1417bdb00 ========

longlong FUN_1417bdb00(void)

{
  if (DAT_1459b25a8 == 0) {
    FUN_141f291f0(L"/Script/AT",L"AT_UITPSSkillCursor",&DAT_1459b25a8,_guard_check_icall,0x3f0,
                  0x10000000,0,L"Engine",&LAB_1417bde20,FUN_1407e4520,_guard_check_icall,
                  &LAB_141766140,&LAB_14083c250,0);
  }
  return DAT_1459b25a8;
}



// ======== WIN  @ 0x1417bdbb0 ========

longlong FUN_1417bdbb0(void)

{
  if (DAT_1459b2478 == 0) {
    FUN_141f291f0(L"/Script/AT",L"AT_UIThankYou",&DAT_1459b2478,_guard_check_icall,0x3f8,0x10000000,
                  0,L"Engine",&LAB_1417bdeb0,FUN_1407e4520,_guard_check_icall,&LAB_141766140,
                  &LAB_14083c250,0);
  }
  return DAT_1459b2478;
}



// ======== WIN  @ 0x1417bdc60 ========

void FUN_1417bdc60(void)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)FUN_140b40c10(0x10);
  if (puVar1 != (undefined8 *)0x0) {
    *(undefined4 *)(puVar1 + 1) = 0x28;
    *puVar1 = &PTR_FUN_14407d610;
    *(undefined4 *)((longlong)puVar1 + 0xc) = 8;
    return;
  }
  return;
}



// ======== WIN  @ 0x1417bdca0 ========

longlong FUN_1417bdca0(void)

{
  if (DAT_1459b25a0 == 0) {
    FUN_14203a4b0(&DAT_1459b25a0,&PTR_FUN_14407b5a0);
  }
  return DAT_1459b25a0;
}



// ======== WIN  @ 0x1417bdcd0 ========

longlong FUN_1417bdcd0(void)

{
  if (DAT_1459b2548 == 0) {
    FUN_14203a4b0(&DAT_1459b2548,&PTR_FUN_14407b6b0);
  }
  return DAT_1459b2548;
}



// ======== WIN  @ 0x1417bdd00 ========

longlong FUN_1417bdd00(void)

{
  if (DAT_1459b2540 == 0) {
    FUN_14203aad0(&DAT_1459b2540,&PTR_FUN_1454ee4f0);
  }
  return DAT_1459b2540;
}



// ======== WIN  @ 0x1417bdd30 ========

void FUN_1417bdd30(void)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)FUN_140b40c10(0x10);
  if (puVar1 != (undefined8 *)0x0) {
    *(undefined4 *)(puVar1 + 1) = 0x28;
    *puVar1 = &PTR_FUN_14407bc50;
    *(undefined4 *)((longlong)puVar1 + 0xc) = 8;
    return;
  }
  return;
}



// ======== WIN  @ 0x1417bdd70 ========

longlong FUN_1417bdd70(void)

{
  if (DAT_1459b26b0 == 0) {
    FUN_14203a4b0(&DAT_1459b26b0,&PTR_FUN_144076570);
  }
  return DAT_1459b26b0;
}



// ======== WIN  @ 0x1417bdda0 ========

longlong FUN_1417bdda0(void)

{
  if (DAT_1459b26a8 == 0) {
    FUN_14203aad0(&DAT_1459b26a8,&PTR_FUN_1454f0010);
  }
  return DAT_1459b26a8;
}



// ======== WIN  @ 0x1417bddf0 ========

void FUN_1417bddf0(longlong *param_1)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)*param_1;
  if (puVar1 != (undefined8 *)0x0) {
    FUN_140d487c0(puVar1,param_1);
    *puVar1 = &PTR_FUN_14407d240;
  }
  return;
}



// ======== WIN  @ 0x1417bde70 ========

void FUN_1417bde70(longlong *param_1)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)*param_1;
  if (puVar1 != (undefined8 *)0x0) {
    FUN_141a4f560(puVar1,param_1);
    *puVar1 = &PTR_FUN_14407bd80;
    puVar1[0x20] = &PTR_LAB_143e61198;
  }
  return;
}



// ======== WIN  @ 0x1417bdf10 ========

void FUN_1417bdf10(longlong *param_1)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)*param_1;
  if (puVar1 != (undefined8 *)0x0) {
    FUN_141a4f560(puVar1,param_1);
    *puVar1 = &PTR_FUN_14407c330;
    puVar1[0x20] = &PTR_LAB_14407c808;
    FUN_141584550(puVar1 + 0x7d);
    return;
  }
  return;
}



// ======== WIN  @ 0x1417bdf60 ========

void FUN_1417bdf60(longlong *param_1)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)*param_1;
  if (puVar1 != (undefined8 *)0x0) {
    FUN_141a4f560(puVar1,param_1);
    *puVar1 = &PTR_FUN_14407c830;
    puVar1[0x20] = &PTR_LAB_14407cd08;
    puVar1[0x73] = 0;
    puVar1[0x74] = 0;
    puVar1[0x75] = 2;
    *(undefined4 *)(puVar1 + 0x76) = 0x3f800000;
  }
  return;
}



// ======== WIN  @ 0x1417bdfc0 ========

void FUN_1417bdfc0(longlong *param_1)

{
  undefined8 *puVar1;
  
  puVar1 = (undefined8 *)*param_1;
  if (puVar1 != (undefined8 *)0x0) {
    FUN_141a4f560(puVar1,param_1);
    *puVar1 = &PTR_FUN_14407cd40;
    puVar1[0x20] = &PTR_LAB_14407d218;
    puVar1[0x85] = 0;
    puVar1[0x86] = 0;
    puVar1[0x89] = 0;
    puVar1[0x8a] = 0;
    *(undefined4 *)(puVar1 + 0x8b) = 0xffffffff;
    *(undefined4 *)((longlong)puVar1 + 0x45c) = 0;
    puVar1[0x8d] = 0;
    *(undefined4 *)(puVar1 + 0x8e) = 0;
    FUN_141584550(puVar1 + 0x99);
    FUN_141584550(puVar1 + 0xa4);
    FUN_141584550(puVar1 + 0xaf);
    puVar1[0xba] = 0;
    puVar1[0xbb] = 0;
    puVar1[0xbe] = 0;
    puVar1[0xbf] = 0;
    *(undefined4 *)((longlong)puVar1 + 0x604) = 0;
    *(undefined4 *)(puVar1 + 0xc0) = 0xffffffff;
    puVar1[0xc2] = 0;
    *(undefined4 *)(puVar1 + 0xc3) = 0;
  }
  return;
}



// ======== WIN  @ 0x1417be210 ========

undefined8 * FUN_1417be210(undefined8 *param_1,ulonglong param_2)

{
  if (param_1[5] != 0) {
    FUN_141de9200();
  }
  if (param_1[1] != 0) {
    FUN_141de9200();
  }
  *param_1 = &PTR_FUN_14397c0d8;
  if ((param_2 & 1) != 0) {
    thunk_FUN_141de9200(param_1,0x38);
  }
  return param_1;
}



// ======== WIN  @ 0x1417be270 ========

longlong FUN_1417be270(longlong param_1,ulonglong param_2)

{
  FUN_141586720(param_1 + 1000);
  FUN_14103c060(param_1);
  if ((param_2 & 1) != 0) {
    thunk_FUN_141de9200(param_1,0x440);
  }
  return param_1;
}



// ======== WIN  @ 0x1417be2c0 ========

longlong FUN_1417be2c0(longlong param_1,ulonglong param_2)

{
  FUN_1407e5820(param_1 + 0x398);
  FUN_14103c060(param_1);
  if ((param_2 & 1) != 0) {
    thunk_FUN_141de9200(param_1,0x3e0);
  }
  return param_1;
}



// ======== WIN  @ 0x1417be310 ========

longlong FUN_1417be310(longlong param_1,ulonglong param_2)

{
  *(undefined4 *)(param_1 + 0x618) = 0;
  if (*(longlong *)(param_1 + 0x610) != 0) {
    FUN_141de9200();
  }
  FUN_1407fa390(param_1 + 0x5d0);
  FUN_141586720(param_1 + 0x578);
  FUN_141586720(param_1 + 0x520);
  FUN_141586720(param_1 + 0x4c8);
  *(undefined4 *)(param_1 + 0x470) = 0;
  if (*(longlong *)(param_1 + 0x468) != 0) {
    FUN_141de9200();
  }
  FUN_1407fa390(param_1 + 0x428);
  FUN_14103c060(param_1);
  if ((param_2 & 1) != 0) {
    thunk_FUN_141de9200(param_1,0x620);
  }
  return param_1;
}



// ======== WIN  @ 0x1417be430 ========

void FUN_1417be430(longlong *param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  
  lVar1 = *param_1;
  uVar2 = FUN_14203ed30(param_1,DAT_1459b2400);
                    /* WARNING: Could not recover jumptable at 0x0001417be466. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(lVar1 + 0x208))(param_1,uVar2,0);
  return;
}



// ======== WIN  @ 0x1417be470 ========

undefined8 FUN_1417be470(undefined8 param_1,longlong param_2,longlong param_3,int param_4)

{
  undefined8 *puVar1;
  int iVar2;
  void *_Src;
  undefined8 *puVar3;
  
  if (param_4 != 0) {
    puVar3 = (undefined8 *)(param_3 + 0x10);
    do {
      puVar1 = (undefined8 *)(param_2 + 0x10);
      *(undefined4 *)(param_2 + 8) = *(undefined4 *)(puVar3 + -1);
      if (puVar1 != puVar3) {
        iVar2 = *(int *)(puVar3 + 1);
        _Src = (void *)*puVar3;
        *(int *)(param_2 + 0x18) = iVar2;
        if ((iVar2 == 0) && (*(int *)(param_2 + 0x1c) == 0)) {
          *(undefined4 *)(param_2 + 0x1c) = 0;
        }
        else {
          FUN_1407f1420(puVar1,iVar2);
          memcpy((void *)*puVar1,_Src,(longlong)iVar2 * 2);
        }
      }
      puVar1 = puVar3 + 2;
      puVar3 = puVar3 + 5;
      *(undefined4 *)(param_2 + 0x20) = *(undefined4 *)puVar1;
      param_4 = param_4 + -1;
      param_2 = param_2 + 0x28;
    } while (param_4 != 0);
  }
  return 1;
}



// ======== WIN  @ 0x1417be540 ========

undefined8 FUN_1417be540(undefined8 param_1,longlong param_2,longlong param_3,int param_4)

{
  undefined8 *puVar1;
  int iVar2;
  void *pvVar3;
  undefined8 *puVar4;
  
  if (param_4 != 0) {
    puVar4 = (undefined8 *)(param_3 + 8);
    do {
      puVar1 = (undefined8 *)(param_2 + 8);
      if (puVar1 != puVar4) {
        iVar2 = *(int *)(puVar4 + 1);
        pvVar3 = (void *)*puVar4;
        *(int *)(param_2 + 0x10) = iVar2;
        if ((iVar2 == 0) && (*(int *)(param_2 + 0x14) == 0)) {
          *(undefined4 *)(param_2 + 0x14) = 0;
        }
        else {
          FUN_1407f1420(puVar1,iVar2);
          memcpy((void *)*puVar1,pvVar3,(longlong)iVar2 * 2);
        }
      }
      puVar1 = (undefined8 *)(param_2 + 0x28);
      *(undefined4 *)(param_2 + 0x18) = *(undefined4 *)(puVar4 + 2);
      *(undefined4 *)(param_2 + 0x1c) = *(undefined4 *)((longlong)puVar4 + 0x14);
      *(undefined4 *)(param_2 + 0x20) = *(undefined4 *)(puVar4 + 3);
      if (puVar1 != puVar4 + 4) {
        iVar2 = *(int *)(puVar4 + 5);
        pvVar3 = (void *)puVar4[4];
        *(int *)(param_2 + 0x30) = iVar2;
        if ((iVar2 == 0) && (*(int *)(param_2 + 0x34) == 0)) {
          *(undefined4 *)(param_2 + 0x34) = 0;
        }
        else {
          FUN_1407f1420(puVar1,iVar2);
          memcpy((void *)*puVar1,pvVar3,(longlong)iVar2 * 2);
        }
      }
      puVar4 = puVar4 + 7;
      param_4 = param_4 + -1;
      param_2 = param_2 + 0x38;
    } while (param_4 != 0);
  }
  return 1;
}



// ======== WIN  @ 0x1417be660 ========

void FUN_1417be660(void)

{
  undefined8 uVar1;
  
  if (DAT_1459b1a20 == 0) {
    uVar1 = FUN_141752b00();
    DAT_1459b1a20 = FUN_1420429a0(FUN_1417bee30,uVar1,L"EEpisodeId");
  }
  return;
}



// ======== WIN  @ 0x1417be6a0 ========

void FUN_1417be6a0(void)

{
  undefined8 uVar1;
  
  if (DAT_1459b1a10 == 0) {
    uVar1 = FUN_141752b00();
    DAT_1459b1a10 = FUN_1420429a0(FUN_1417bee60,uVar1,L"EPlayerBattleHUDAppearance");
  }
  return;
}



// ======== WIN  @ 0x1417be6e0 ========

void FUN_1417be6e0(void)

{
  undefined8 uVar1;
  
  if (DAT_1459b1a18 == 0) {
    uVar1 = FUN_141752b00();
    DAT_1459b1a18 = FUN_1420429a0(FUN_1417bee90,uVar1,L"ESpGaugeOverlayAppearance");
  }
  return;
}



// ======== WIN  @ 0x1417be720 ========

void FUN_1417be720(void)

{
  undefined8 uVar1;
  
  if (DAT_1459b1a28 == 0) {
    uVar1 = FUN_141752b00();
    DAT_1459b1a28 = FUN_1420429a0(FUN_1417beec0,uVar1,L"ETelopViewType");
  }
  return;
}



// ======== WIN  @ 0x1417be760 ========

undefined8 FUN_1417be760(void)

{
  return 0xc4de4c4a;
}



// ======== WIN  @ 0x1417be770 ========

undefined8 FUN_1417be770(void)

{
  return 0x428e348;
}



// ======== WIN  @ 0x1417be780 ========

undefined8 FUN_1417be780(void)

{
  return 0x4b91ce6e;
}



// ======== WIN  @ 0x1417be790 ========

undefined8 FUN_1417be790(void)

{
  return 0x2f50139;
}



// ======== WIN  @ 0x1417be7a0 ========

undefined8 FUN_1417be7a0(void)

{
  return 0xe7f8d3d0;
}



// ======== WIN  @ 0x1417be7b0 ========

void FUN_1417be7b0(longlong *param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  
  lVar1 = *param_1;
  uVar2 = FUN_14203ed30(param_1,DAT_1459b2420);
                    /* WARNING: Could not recover jumptable at 0x0001417be7e6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(lVar1 + 0x208))(param_1,uVar2,0);
  return;
}



// ======== WIN  @ 0x1417be7f0 ========

void FUN_1417be7f0(longlong *param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  
  lVar1 = *param_1;
  uVar2 = FUN_14203ed30(param_1,DAT_1459b2428);
                    /* WARNING: Could not recover jumptable at 0x0001417be826. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(lVar1 + 0x208))(param_1,uVar2,0);
  return;
}



// ======== WIN  @ 0x1417be830 ========

void FUN_1417be830(void)

{
  undefined8 uVar1;
  
  uVar1 = FUN_1417bc720();
  FUN_141f2d810(uVar1,&PTR_s_CheckHovered_14407c260,9);
  return;
}



// ======== WIN  @ 0x1417be860 ========

void FUN_1417be860(void)

{
  undefined8 uVar1;
  
  uVar1 = FUN_1417bcec0();
  FUN_141f2d810(uVar1,&PTR_s_CheckHovered_14407c2f0,4);
  return;
}



// ======== WIN  @ 0x1417be890 ========

void FUN_1417be890(void)

{
  undefined8 uVar1;
  
  uVar1 = FUN_1417bd3a0();
  FUN_141f2d810(uVar1,&PTR_s_StartInputAccepting_14407cd30,1);
  return;
}



// ======== WIN  @ 0x1417be8c0 ========

void FUN_1417be8c0(void)

{
  undefined8 uVar1;
  
  uVar1 = FUN_1417bcc50();
  FUN_141f2d810(uVar1,&PTR_s_PlayIn_14407d4a8,1);
  return;
}



// ======== WIN  @ 0x1417be8f0 ========

void FUN_1417be8f0(void)

{
  undefined8 uVar1;
  
  uVar1 = FUN_1417bcdb0();
  FUN_141f2d810(uVar1,&PTR_s_DebugChangeLangage_14407d870,2);
  return;
}



// ======== WIN  @ 0x1417be920 ========

void FUN_1417be920(void)

{
  undefined8 uVar1;
  
  uVar1 = FUN_1417bc880();
  FUN_141f2d810(uVar1,&PTR_s_MouseClickDecide_14407d890,1);
  return;
}



// ======== WIN  @ 0x1417be980 ========

void FUN_1417be980(void)

{
  undefined4 uVar1;
  undefined8 uVar2;
  
  if (DAT_1459b2618 == 0) {
    uVar1 = FUN_1417be760();
    uVar2 = FUN_141752b00();
    DAT_1459b2618 = FUN_142042ad0(FUN_1417beef0,uVar2,L"CharacterIconCodeList",0x28,uVar1);
  }
  return;
}



// ======== WIN  @ 0x1417be9e0 ========

void FUN_1417be9e0(void)

{
  undefined4 uVar1;
  undefined8 uVar2;
  
  if (DAT_1459b2608 == 0) {
    uVar1 = FUN_1417be770();
    uVar2 = FUN_141752b00();
    DAT_1459b2608 = FUN_142042ad0(FUN_1417bef20,uVar2,L"CharacterIconCodeToPath",0x18,uVar1);
  }
  return;
}



// ======== WIN  @ 0x1417bea40 ========

void FUN_1417bea40(void)

{
  undefined4 uVar1;
  undefined8 uVar2;
  
  if (DAT_1459b2638 == 0) {
    uVar1 = FUN_1417be780();
    uVar2 = FUN_141752b00();
    DAT_1459b2638 = FUN_142042ad0(FUN_1417bef50,uVar2,L"CharacterIconInfo",0x38,uVar1);
  }
  return;
}



// ======== WIN  @ 0x1417beaa0 ========

void FUN_1417beaa0(void)

{
  undefined4 uVar1;
  undefined8 uVar2;
  
  if (DAT_1459b2628 == 0) {
    uVar1 = FUN_1417be790();
    uVar2 = FUN_141752b00();
    DAT_1459b2628 = FUN_142042ad0(FUN_1417bef80,uVar2,L"CharacterResourceInfo",0x20,uVar1);
  }
  return;
}



// ======== WIN  @ 0x1417beb00 ========

void FUN_1417beb00(void)

{
  undefined4 uVar1;
  undefined8 uVar2;
  
  if (DAT_1459b2440 == 0) {
    uVar1 = FUN_1417be7a0();
    uVar2 = FUN_141752b00();
    DAT_1459b2440 = FUN_142042ad0(FUN_1417befb0,uVar2,L"TelopInfo",0x28,uVar1);
  }
  return;
}



// ======== WIN  @ 0x1417beb60 ========

longlong FUN_1417beb60(void)

{
  if (DAT_1459b25c0 == 0) {
    FUN_14203a4b0(&DAT_1459b25c0,&PTR_FUN_144076860);
  }
  return DAT_1459b25c0;
}



// ======== WIN  @ 0x1417beb90 ========

longlong FUN_1417beb90(void)

{
  if (DAT_1459b2590 == 0) {
    FUN_14203a4b0(&DAT_1459b2590,&PTR_FUN_14407b280);
  }
  return DAT_1459b2590;
}



// ======== WIN  @ 0x1417bebc0 ========

longlong FUN_1417bebc0(void)

{
  if (DAT_1459b25b0 == 0) {
    FUN_14203a4b0(&DAT_1459b25b0,&PTR_FUN_1440761c0);
  }
  return DAT_1459b25b0;
}



// ======== WIN  @ 0x1417bebf0 ========

longlong FUN_1417bebf0(void)

{
  if (DAT_1459b2450 == 0) {
    FUN_14203a4b0(&DAT_1459b2450,&PTR_FUN_14407b7b0);
  }
  return DAT_1459b2450;
}



// ======== WIN  @ 0x1417bec20 ========

longlong FUN_1417bec20(void)

{
  if (DAT_1459b2460 == 0) {
    FUN_14203a4b0(&DAT_1459b2460,&PTR_FUN_14407a350);
  }
  return DAT_1459b2460;
}



// ======== WIN  @ 0x1417bec50 ========

longlong FUN_1417bec50(void)

{
  if (DAT_1459b2470 == 0) {
    FUN_14203a4b0(&DAT_1459b2470,&PTR_FUN_14407b0c0);
  }
  return DAT_1459b2470;
}



// ======== WIN  @ 0x1417bec80 ========

longlong FUN_1417bec80(void)

{
  if (DAT_1459b2508 == 0) {
    FUN_14203a4b0(&DAT_1459b2508,&PTR_FUN_144078de0);
  }
  return DAT_1459b2508;
}



// ======== WIN  @ 0x1417becb0 ========

longlong FUN_1417becb0(void)

{
  if (DAT_1459b2530 == 0) {
    FUN_14203a4b0(&DAT_1459b2530,&PTR_FUN_144079580);
  }
  return DAT_1459b2530;
}



// ======== WIN  @ 0x1417bece0 ========

longlong FUN_1417bece0(void)

{
  if (DAT_1459b2560 == 0) {
    FUN_14203a4b0(&DAT_1459b2560,&PTR_FUN_144076740);
  }
  return DAT_1459b2560;
}



// ======== WIN  @ 0x1417bed10 ========

longlong FUN_1417bed10(void)

{
  if (DAT_1459b2580 == 0) {
    FUN_14203a4b0(&DAT_1459b2580,&PTR_FUN_14407a170);
  }
  return DAT_1459b2580;
}



// ======== WIN  @ 0x1417bed40 ========

longlong FUN_1417bed40(void)

{
  if (DAT_1459b2570 == 0) {
    FUN_14203a4b0(&DAT_1459b2570,&PTR_FUN_144079a50);
  }
  return DAT_1459b2570;
}



// ======== WIN  @ 0x1417bed70 ========

longlong FUN_1417bed70(void)

{
  if (DAT_1459b2698 == 0) {
    FUN_14203a4b0(&DAT_1459b2698,&PTR_FUN_14407a4c0);
  }
  return DAT_1459b2698;
}



// ======== WIN  @ 0x1417beda0 ========

longlong FUN_1417beda0(void)

{
  if (DAT_1459b26c0 == 0) {
    FUN_14203a4b0(&DAT_1459b26c0,&PTR_FUN_144076fd0);
  }
  return DAT_1459b26c0;
}



// ======== WIN  @ 0x1417bedd0 ========

longlong FUN_1417bedd0(void)

{
  if (DAT_1459b2558 == 0) {
    FUN_14203aad0(&DAT_1459b2558,&PTR_FUN_1454ee800);
  }
  return DAT_1459b2558;
}



// ======== WIN  @ 0x1417bee00 ========

longlong FUN_1417bee00(void)

{
  if (DAT_1459b25d0 == 0) {
    FUN_14203aad0(&DAT_1459b25d0,&PTR_FUN_1454ef1f0);
  }
  return DAT_1459b25d0;
}



// ======== WIN  @ 0x1417bee30 ========

longlong FUN_1417bee30(void)

{
  if (DAT_1459b2600 == 0) {
    FUN_14203a700(&DAT_1459b2600,&PTR_FUN_1454f0950);
  }
  return DAT_1459b2600;
}



// ======== WIN  @ 0x1417bee60 ========

longlong FUN_1417bee60(void)

{
  if (DAT_1459b25f0 == 0) {
    FUN_14203a700(&DAT_1459b25f0,&PTR_FUN_1454f08b0);
  }
  return DAT_1459b25f0;
}



// ======== WIN  @ 0x1417bee90 ========

longlong FUN_1417bee90(void)

{
  if (DAT_1459b25f8 == 0) {
    FUN_14203a700(&DAT_1459b25f8,&PTR_FUN_1454f0900);
  }
  return DAT_1459b25f8;
}



// ======== WIN  @ 0x1417beec0 ========

longlong FUN_1417beec0(void)

{
  if (DAT_1459b2438 == 0) {
    FUN_14203a700(&DAT_1459b2438,&PTR_FUN_1454ec720);
  }
  return DAT_1459b2438;
}



// ======== WIN  @ 0x1417beef0 ========

longlong FUN_1417beef0(void)

{
  if (DAT_1459b2620 == 0) {
    FUN_14203c8c0(&DAT_1459b2620,&PTR_FUN_1454ef650);
  }
  return DAT_1459b2620;
}



// ======== WIN  @ 0x1417bef20 ========

longlong FUN_1417bef20(void)

{
  if (DAT_1459b2610 == 0) {
    FUN_14203c8c0(&DAT_1459b2610,&PTR_FUN_1454ef4d0);
  }
  return DAT_1459b2610;
}



// ======== WIN  @ 0x1417bef50 ========

longlong FUN_1417bef50(void)

{
  if (DAT_1459b2640 == 0) {
    FUN_14203c8c0(&DAT_1459b2640,&PTR_FUN_1454ef800);
  }
  return DAT_1459b2640;
}



// ======== WIN  @ 0x1417bef80 ========

longlong FUN_1417bef80(void)

{
  if (DAT_1459b2630 == 0) {
    FUN_14203c8c0(&DAT_1459b2630,&PTR_FUN_1454ef6a0);
  }
  return DAT_1459b2630;
}



// ======== WIN  @ 0x1417befb0 ========

longlong FUN_1417befb0(void)

{
  if (DAT_1459b2448 == 0) {
    FUN_14203c8c0(&DAT_1459b2448,&PTR_FUN_1454ec910);
  }
  return DAT_1459b2448;
}



// ======== WIN  @ 0x1417befe0 ========

void FUN_1417befe0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416b6650();
  return;
}



// ======== d1<-0x1417bd0d0  @ 0x141f291f0 ========

void FUN_141f291f0(undefined8 param_1,undefined8 param_2,longlong *param_3,
                  code *UNRECOVERED_JUMPTABLE,undefined4 param_5,undefined4 param_6,
                  ulonglong param_7,undefined8 param_8,undefined8 param_9,undefined8 param_10,
                  undefined8 param_11,code *param_12,code *param_13,char param_14)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  longlong *plVar4;
  bool bVar5;
  undefined1 local_res18 [8];
  undefined8 in_stack_ffffffffffffff88;
  undefined4 uVar6;
  undefined8 in_stack_ffffffffffffffa0;
  undefined4 uVar7;
  
  uVar6 = (undefined4)((ulonglong)in_stack_ffffffffffffff88 >> 0x20);
  uVar7 = (undefined4)((ulonglong)in_stack_ffffffffffffffa0 >> 0x20);
  if (param_14 == '\0') {
    lVar1 = FUN_1420384d0(&DAT_145b9e540,0x208,8,1);
    *param_3 = lVar1;
    puVar2 = (undefined8 *)FUN_141eb3490(local_res18,param_2,1);
    puVar2 = (undefined8 *)
             FUN_141f1ece0(lVar1,0,*puVar2,param_5,CONCAT44(uVar6,param_6),param_7,param_8,
                           CONCAT44(uVar7,199),param_9,param_10,param_11);
  }
  else {
    puVar2 = (undefined8 *)FUN_1420384d0(&DAT_145b9e540,0x270,8,DAT_1456da000);
    *param_3 = (longlong)puVar2;
    bVar5 = DAT_1456da000 != '\0';
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,param_2,1);
    FUN_141f1ece0(puVar2,0,*puVar3,param_5,CONCAT44(uVar6,param_6) | 0x40000,param_7,param_8,
                  CONCAT44(uVar7,-(uint)bVar5) & 0xffffffff00000080 | 0x4000043,param_9,param_10,
                  param_11);
    *puVar2 = &PTR_FUN_144761630;
    puVar2[0x41] = 0;
    puVar2[0x42] = 0;
    puVar2[0x43] = 0;
    puVar2[0x44] = 0;
    puVar2[0x45] = 0;
    puVar2[0x46] = 0;
    puVar2[0x47] = 0;
    puVar2[0x48] = 0;
    puVar2[0x49] = 0;
    puVar2[0x4a] = 0;
    puVar2[0x4b] = 0;
    puVar2[0x4c] = 0;
    puVar2[0x4d] = 0;
  }
  *param_3 = (longlong)puVar2;
  lVar1 = (*param_13)();
  param_3 = (longlong *)*param_3;
  plVar4 = (longlong *)(*param_12)();
  param_7 = param_7 & 0xffffffffffffff00;
  uVar6 = 0;
  FUN_141f07780(param_1,param_2,0,1,0,param_7);
  if (plVar4 == param_3) {
    plVar4 = (longlong *)0x0;
  }
  (**(code **)(*param_3 + 0x2a0))(param_3,plVar4);
  param_3[0x18] = lVar1;
  (**(code **)(*param_3 + 8))(param_3);
  if ((*(uint *)(param_3 + 1) >> 0x1a & 1) == 0) {
    FUN_142049670(param_3,param_1,param_2);
  }
  else {
    if (DAT_145b9c8a0 == 0) {
      param_7 = CONCAT44((int)(param_7 >> 0x20),0x10000000);
      FUN_141f291f0(L"/Script/CoreUObject",L"DynamicClass",&DAT_145b9c8a0,_guard_check_icall,
                    CONCAT44(uVar6,0x270),param_7,0,L"Engine",FUN_141f1c610,FUN_141f1c930,
                    FUN_141f21430,FUN_141f28040,&LAB_141fd55f0,0);
    }
    (**(code **)(*param_3 + 0x10))(param_3,DAT_145b9c8a0,param_1,param_2);
  }
  FUN_141f07780(param_1,param_2,0,2,0,param_7 & 0xffffffffffffff00);
                    /* WARNING: Could not recover jumptable at 0x000141f29547. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}



// ======== d1<-0x1417bd930  @ 0x14203a4b0 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14203a4b0(longlong *param_1,undefined8 *param_2)

{
  longlong *plVar1;
  undefined1 uVar2;
  int iVar3;
  undefined4 uVar4;
  longlong lVar5;
  longlong lVar6;
  undefined8 *puVar7;
  ulonglong uVar8;
  ulonglong *puVar9;
  ulonglong uVar10;
  ulonglong uVar11;
  undefined8 *puVar12;
  undefined1 auStack_188 [32];
  int local_168 [2];
  undefined8 local_160;
  undefined1 local_158 [256];
  longlong local_58;
  ulonglong local_38;
  
  local_38 = DAT_145909780 ^ (ulonglong)auStack_188;
  if ((*param_1 == 0) || ((*(uint *)(*param_1 + 0xb4) & 0x20000000) == 0)) {
    puVar7 = (undefined8 *)param_2[1];
    uVar11 = 0;
    uVar8 = (longlong)*(int *)(param_2 + 2) & 0x1fffffffffffffff;
    if (puVar7 + *(int *)(param_2 + 2) < puVar7) {
      uVar8 = uVar11;
    }
    uVar10 = uVar11;
    if (uVar8 != 0) {
      do {
        (*(code *)*puVar7)();
        uVar10 = uVar10 + 1;
        puVar7 = puVar7 + 1;
      } while (uVar10 != uVar8);
    }
    lVar5 = (*(code *)*param_2)();
    *param_1 = lVar5;
    if ((*(uint *)(lVar5 + 0xb4) & 0x20000000) == 0) {
      FUN_14204f930(lVar5);
      *(uint *)(lVar5 + 0xb4) =
           *(uint *)(lVar5 + 0xb4) | *(uint *)((longlong)param_2 + 0x14) | 0x20000000;
      if ((*(uint *)(lVar5 + 0xb4) & 0x10000000) == 0) {
        *(undefined4 *)(lVar5 + 0x1c8) = 0;
        if (*(int *)(lVar5 + 0x1cc) != 0) {
          FUN_140827be0(lVar5 + 0x1c0,0);
        }
      }
      FUN_141f24110(lVar5,param_2[3],*(undefined4 *)(param_2 + 4));
      local_168[0] = *(int *)(param_2 + 6);
      local_160 = param_2[5];
      while (local_168[0] != 0) {
        FUN_14203ae50(lVar5,&local_160,local_168);
      }
      if (param_2[7] != 0) {
        lVar6 = FUN_1407f7cf0(local_158);
        puVar7 = (undefined8 *)FUN_141eb3490(&local_160,*(undefined8 *)(lVar6 + 0x108),1);
        *(undefined8 *)(lVar5 + 0xd8) = *puVar7;
        if (local_58 != 0) {
          FUN_141de9200();
        }
      }
      iVar3 = *(int *)(param_2 + 10);
      if (iVar3 != 0) {
        plVar1 = (longlong *)(lVar5 + 0x1b0);
        if (*(int *)(lVar5 + 0x1bc) < iVar3) {
          FUN_1407f1a80(plVar1,iVar3);
        }
        puVar7 = (undefined8 *)param_2[9];
        puVar12 = puVar7 + (longlong)iVar3 * 2;
        for (; puVar7 != puVar12; puVar7 = puVar7 + 2) {
          uVar8 = uVar11;
          if ((code *)*puVar7 != (code *)0x0) {
            uVar8 = (*(code *)*puVar7)();
          }
          iVar3 = *(int *)(lVar5 + 0x1b8);
          *(int *)(lVar5 + 0x1b8) = iVar3 + 1;
          if (*(int *)(lVar5 + 0x1bc) < iVar3 + 1) {
            FUN_1407f1790(plVar1,iVar3);
          }
          uVar2 = *(undefined1 *)((longlong)puVar7 + 0xc);
          uVar4 = *(undefined4 *)(puVar7 + 1);
          puVar9 = (ulonglong *)((longlong)iVar3 * 0x10 + *plVar1);
          *puVar9 = uVar8;
          *(undefined4 *)(puVar9 + 1) = uVar4;
          *(undefined1 *)((longlong)puVar9 + 0xc) = uVar2;
        }
      }
      FUN_141fd5900(lVar5,0);
    }
  }
  return;
}



// ======== d1<-0x1417bd960  @ 0x14203aad0 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14203aad0(longlong *param_1,undefined8 *param_2)

{
  undefined4 uVar1;
  code *pcVar2;
  longlong *plVar3;
  longlong *plVar4;
  longlong lVar5;
  undefined8 *puVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  longlong *plVar9;
  undefined1 auStack_578 [32];
  undefined8 local_558;
  undefined1 local_550;
  undefined8 local_548;
  int local_538 [2];
  undefined8 local_530;
  undefined1 local_528 [8];
  undefined1 local_520 [8];
  undefined1 local_518 [256];
  longlong local_418;
  undefined1 local_3f8 [256];
  longlong local_2f8;
  undefined1 local_2d8 [336];
  undefined1 local_188 [336];
  ulonglong local_38;
  
  local_38 = DAT_145909780 ^ (ulonglong)auStack_578;
  plVar9 = (longlong *)0x0;
  local_538[0] = 0;
  pcVar2 = (code *)param_2[3];
  plVar3 = plVar9;
  if ((code *)*param_2 != (code *)0x0) {
    plVar3 = (longlong *)(*(code *)*param_2)();
  }
  plVar4 = plVar9;
  if (pcVar2 != (code *)0x0) {
    plVar4 = (longlong *)(*pcVar2)();
  }
  if (*param_1 == 0) {
    if ((*(uint *)(param_2 + 4) & 0x100000) == 0) {
      lVar5 = FUN_1407f7cf0(local_3f8,param_2[1]);
      uVar1 = *(undefined4 *)(param_2 + 2);
      puVar6 = (undefined8 *)FUN_141eb3490(local_520,*(undefined8 *)(lVar5 + 0x108),1);
      uVar8 = *puVar6;
      uVar7 = FUN_141f28760();
      local_548 = 0;
      local_550 = 0;
      local_558 = (ulonglong)local_558._4_4_ << 0x20;
      lVar5 = FUN_14204bcd0(uVar7,plVar3,uVar8,uVar1);
      plVar3 = plVar9;
      if (lVar5 != 0) {
        uVar8 = FUN_142035810(local_188);
        local_558 = param_2[5];
        plVar9 = (longlong *)FUN_141f1eec0(lVar5,uVar8,plVar4,*(undefined4 *)(param_2 + 4));
        plVar3 = (longlong *)0x2;
      }
      if (plVar3 != (longlong *)0x0) {
        FUN_1420361d0(local_188);
      }
    }
    else {
      lVar5 = FUN_1407f7cf0(local_518,param_2[1]);
      uVar1 = *(undefined4 *)(param_2 + 2);
      puVar6 = (undefined8 *)FUN_141eb3490(local_528,*(undefined8 *)(lVar5 + 0x108),1);
      uVar8 = *puVar6;
      uVar7 = FUN_141f282a0();
      local_548 = 0;
      local_550 = 0;
      local_558 = (ulonglong)local_558._4_4_ << 0x20;
      lVar5 = FUN_14204bcd0(uVar7,plVar3,uVar8,uVar1);
      local_2f8 = local_418;
      if (lVar5 != 0) {
        uVar8 = FUN_142035810(local_2d8);
        local_558 = param_2[5];
        plVar9 = (longlong *)FUN_141f1eec0(lVar5,uVar8,plVar4,*(undefined4 *)(param_2 + 4));
        FUN_1420361d0(local_2d8);
        local_2f8 = local_418;
      }
    }
    if (local_2f8 != 0) {
      FUN_141de9200();
    }
    *param_1 = (longlong)plVar9;
    local_538[0] = *(int *)(param_2 + 7);
    local_530 = param_2[6];
    while (local_538[0] != 0) {
      FUN_14203ae50(plVar9,&local_530,local_538);
    }
    (**(code **)(*plVar9 + 0x248))(plVar9);
    FUN_141fd5900(plVar9,0);
  }
  return;
}



// ======== d1<-0x1417bdc60  @ 0x140b40c10 ========

void FUN_140b40c10(undefined8 param_1)

{
  FUN_141df35c0(param_1,0);
  return;
}



// ======== d1<-0x1417bddf0  @ 0x140d487c0 ========

undefined8 * FUN_140d487c0(undefined8 *param_1)

{
  undefined8 uVar1;
  
  FUN_142035bd0();
  param_1[5] = 0;
  *param_1 = &PTR_FUN_143b23e28;
  uVar1 = FUN_143149bc0();
  uVar1 = FUN_142042c00(param_1,uVar1);
  param_1[5] = uVar1;
  return param_1;
}



// ======== d1<-0x1417bde70  @ 0x141a4f560 ========

undefined8 * FUN_141a4f560(undefined8 *param_1)

{
  FUN_142984df0();
  param_1[0x45] = 0;
  *param_1 = &PTR_FUN_144037570;
  param_1[0x20] = &PTR_LAB_143e79a30;
  param_1[0x46] = 0;
  param_1[0x47] = 0;
  param_1[0x4a] = 0;
  param_1[0x4b] = 0;
  *(undefined4 *)(param_1 + 0x4c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x264) = 0;
  param_1[0x4e] = 0;
  *(undefined4 *)(param_1 + 0x4f) = 0;
  param_1[0x51] = 0;
  param_1[0x52] = 0;
  param_1[0x55] = 0;
  param_1[0x56] = 0;
  *(undefined4 *)(param_1 + 0x57) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 700) = 0;
  param_1[0x59] = 0;
  *(undefined4 *)(param_1 + 0x5a) = 0;
  FUN_1420575e0(param_1 + 0x5d,0);
  param_1[0x5e] = 0;
  param_1[0x5f] = 0;
  *(undefined4 *)(param_1 + 0x60) = 0;
  param_1[0x61] = 0;
  *(undefined4 *)(param_1 + 0x62) = 0;
  param_1[99] = 0;
  *(undefined4 *)(param_1 + 100) = 0;
  FUN_1420575e0(param_1 + 0x65,0);
  param_1[0x66] = 0;
  *(undefined4 *)(param_1 + 0x67) = 0;
  *(undefined4 *)((longlong)param_1 + 0x33c) = 0x41200000;
  *(undefined1 *)(param_1 + 0x68) = 1;
  param_1[0x69] = 0;
  param_1[0x6a] = 0;
  param_1[0x6d] = 0;
  param_1[0x6e] = 0;
  *(undefined4 *)(param_1 + 0x6f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x37c) = 0;
  param_1[0x71] = 0;
  *(undefined4 *)(param_1 + 0x72) = 0;
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0xf7;
  *(undefined1 *)((longlong)param_1 + 0xb7) = 3;
  FUN_141a61380(param_1 + 0x46,0);
  return param_1;
}



// ======== d1<-0x1417bdfc0  @ 0x141584550 ========

longlong FUN_141584550(longlong param_1)

{
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x38) = 0;
  *(undefined8 *)(param_1 + 0x40) = 0;
  *(undefined8 *)(param_1 + 0x48) = 0;
  *(undefined8 *)(param_1 + 0x50) = 0;
  *(undefined1 *)(param_1 + 0x1c) = 0;
  return param_1;
}



// ======== d1<-0x1417be210  @ 0x141de9200 ========

void FUN_141de9200(longlong param_1)

{
  if (param_1 != 0) {
    if (DAT_145a8c1f8 == (longlong *)0x0) {
      FUN_141de9840();
    }
    (**(code **)(*DAT_145a8c1f8 + 0x20))(DAT_145a8c1f8,param_1);
  }
  return;
}



// ======== d1<-0x1417be210  @ 0x140b40c20 ========

void thunk_FUN_141de9200(longlong param_1)

{
  if (param_1 != 0) {
    if (DAT_145a8c1f8 == (longlong *)0x0) {
      FUN_141de9840();
    }
    (**(code **)(*DAT_145a8c1f8 + 0x20))(DAT_145a8c1f8,param_1);
  }
  return;
}



// ======== d1<-0x1417be270  @ 0x141586720 ========

void FUN_141586720(longlong param_1)

{
  if (*(longlong *)(param_1 + 0x40) != 0) {
    FUN_141de9200();
  }
  if (*(longlong *)(param_1 + 0x30) != 0) {
    FUN_141de9200();
  }
  if (*(longlong *)(param_1 + 0x20) != 0) {
    FUN_141de9200();
    return;
  }
  return;
}



// ======== d1<-0x1417be270  @ 0x14103c060 ========

void FUN_14103c060(longlong param_1)

{
  undefined8 uVar1;
  longlong *plVar2;
  
  plVar2 = (longlong *)0x0;
  *(undefined4 *)(param_1 + 0x390) = 0;
  if (*(longlong *)(param_1 + 0x388) != 0) {
    FUN_141de9200();
  }
  FUN_140bba510(param_1 + 0x348,0);
  if (*(longlong *)(param_1 + 0x368) != 0) {
    FUN_141de9200();
  }
  if (*(longlong *)(param_1 + 0x348) != 0) {
    FUN_141de9200();
  }
  FUN_1407e59d0(param_1 + 0x318);
  FUN_1407e59d0(param_1 + 0x308);
  if (*(int *)(param_1 + 0x300) != 0) {
    plVar2 = *(longlong **)(param_1 + 0x2f8);
  }
  if ((plVar2 != (longlong *)0x0) ||
     ((*(int *)(param_1 + 0x300) != 0 &&
      (plVar2 = *(longlong **)(param_1 + 0x2f8), plVar2 != (longlong *)0x0)))) {
    (**(code **)(*plVar2 + 0x30))(plVar2,0);
    if (*(longlong *)(param_1 + 0x2f8) != 0) {
      uVar1 = FUN_141df5850(*(longlong *)(param_1 + 0x2f8),0,0);
      *(undefined8 *)(param_1 + 0x2f8) = uVar1;
    }
    *(undefined4 *)(param_1 + 0x300) = 0;
  }
  if (*(longlong *)(param_1 + 0x2f8) != 0) {
    FUN_141de9200();
  }
  *(undefined4 *)(param_1 + 0x2d0) = 0;
  if (*(longlong *)(param_1 + 0x2c8) != 0) {
    FUN_141de9200();
  }
  FUN_1407e5a40(param_1 + 0x288);
  *(undefined4 *)(param_1 + 0x278) = 0;
  if (*(longlong *)(param_1 + 0x270) != 0) {
    FUN_141de9200();
  }
  FUN_14103f670(param_1 + 0x230,0);
  if (*(longlong *)(param_1 + 0x250) != 0) {
    FUN_141de9200();
  }
  if (*(longlong *)(param_1 + 0x230) != 0) {
    FUN_141de9200();
  }
  FUN_14103c1c0(param_1);
  return;
}



// ======== d1<-0x1417be2c0  @ 0x1407e5820 ========

void FUN_1407e5820(undefined8 *param_1)

{
  longlong *plVar1;
  longlong lVar2;
  longlong *plVar3;
  int iVar4;
  
  plVar3 = (longlong *)*param_1;
  iVar4 = *(int *)(param_1 + 1);
  if (iVar4 != 0) {
    do {
      if (((int)plVar3[1] != 0) && (plVar1 = (longlong *)*plVar3, plVar1 != (longlong *)0x0)) {
        (**(code **)(*plVar1 + 0x30))(plVar1,0);
        if (*plVar3 != 0) {
          lVar2 = FUN_141df5850(*plVar3,0,0);
          *plVar3 = lVar2;
        }
        *(undefined4 *)(plVar3 + 1) = 0;
      }
      if (*plVar3 != 0) {
        FUN_141de9200();
      }
      plVar3 = plVar3 + 2;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
    plVar3 = (longlong *)*param_1;
  }
  if (plVar3 == (longlong *)0x0) {
    return;
  }
  FUN_141de9200(plVar3);
  return;
}



// ======== d1<-0x1417be310  @ 0x1407fa390 ========

void FUN_1407fa390(longlong *param_1)

{
  *(undefined4 *)(param_1 + 1) = 0;
  if (*(int *)((longlong)param_1 + 0xc) != 0) {
    FUN_1407f18a0(param_1,0);
  }
  *(undefined4 *)(param_1 + 6) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x34) = 0;
  *(undefined4 *)(param_1 + 5) = 0;
  if (*(int *)((longlong)param_1 + 0x2c) != 0) {
    *(undefined4 *)((longlong)param_1 + 0x2c) = 0;
    FUN_1407f0ad0(param_1 + 2,0);
  }
  if (param_1[4] != 0) {
    FUN_141de9200();
  }
  if (*param_1 != 0) {
    FUN_141de9200();
  }
  return;
}



// ======== d1<-0x1417be430  @ 0x14203ed30 ========

longlong FUN_14203ed30(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined2 *puVar3;
  undefined2 *puVar4;
  undefined2 *puVar5;
  undefined8 local_res10;
  longlong local_28 [2];
  longlong local_18 [2];
  
  local_res10 = param_2;
  lVar1 = FUN_141f25a00(*(undefined8 *)(param_1 + 0x10),param_2,1);
  if (lVar1 == 0) {
    puVar2 = (undefined8 *)FUN_142041d60(param_1,local_18,0);
    puVar3 = &DAT_14391f7d0;
    if (*(int *)(puVar2 + 1) == 0) {
      puVar4 = &DAT_14391f7d0;
    }
    else {
      puVar4 = (undefined2 *)*puVar2;
    }
    puVar2 = (undefined8 *)FUN_141eca8b0(&local_res10,local_28);
    if (*(int *)(puVar2 + 1) == 0) {
      puVar5 = &DAT_14391f7d0;
    }
    else {
      puVar5 = (undefined2 *)*puVar2;
    }
    FUN_141e49950("Unknown",0x4cc,L"Failed to find function %s in %s",puVar5,puVar4);
    if (local_28[0] != 0) {
      FUN_141de9200();
    }
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    puVar2 = (undefined8 *)FUN_142041d60(param_1,local_28,0);
    if (*(int *)(puVar2 + 1) == 0) {
      puVar4 = &DAT_14391f7d0;
    }
    else {
      puVar4 = (undefined2 *)*puVar2;
    }
    puVar2 = (undefined8 *)FUN_141eca8b0(&local_res10,local_18);
    if (*(int *)(puVar2 + 1) != 0) {
      puVar3 = (undefined2 *)*puVar2;
    }
    FUN_141e40cb0(&DAT_144783e1f,"Unknown",0x4cc,L"Failed to find function %s in %s",puVar3,puVar4);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    if (local_28[0] != 0) {
      FUN_141de9200();
    }
  }
  return lVar1;
}



// ======== d1<-0x1417be470  @ 0x1407f1420 ========

void FUN_1407f1420(longlong *param_1,int param_2,int param_3)

{
  int iVar1;
  ulonglong uVar2;
  longlong lVar3;
  
  iVar1 = 0;
  if (param_2 != 0) {
    uVar2 = FUN_141df5250((longlong)param_2 * 2,0);
    iVar1 = (int)(uVar2 >> 1);
    if (iVar1 < param_2) {
      iVar1 = 0x7fffffff;
    }
  }
  if ((iVar1 != param_3) && ((*param_1 != 0 || (iVar1 != 0)))) {
    lVar3 = FUN_141df5850(*param_1,(longlong)iVar1 * 2,0);
    *param_1 = lVar3;
  }
  *(int *)((longlong)param_1 + 0xc) = iVar1;
  return;
}



// ======== d1<-0x1417be470  @ 0x1438246c8 ========

void * __cdecl memcpy(void *_Dst,void *_Src,size_t _Size)

{
  void *pvVar1;
  
                    /* WARNING: Could not recover jumptable at 0x0001438246c8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pvVar1 = memcpy(_Dst,_Src,_Size);
  return pvVar1;
}



// ======== d1<-0x1417be660  @ 0x141752b00 ========

longlong FUN_141752b00(void)

{
  if (DAT_1459950e8 == 0) {
    FUN_14203ad30(&DAT_1459950e8,&PTR_s__Script_AT_143f868c8);
  }
  return DAT_1459950e8;
}



// ======== d1<-0x1417be660  @ 0x1420429a0 ========

undefined8 FUN_1420429a0(code *param_1,longlong param_2,undefined8 param_3)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  undefined8 uVar4;
  undefined2 *puVar5;
  undefined8 local_res8;
  undefined2 *local_18;
  int local_10;
  
  lVar3 = *(longlong *)(param_2 + 0x20);
  lVar2 = param_2;
  while (lVar1 = lVar3, lVar1 != 0) {
    lVar2 = lVar1;
    lVar3 = *(longlong *)(lVar1 + 0x20);
  }
  local_res8 = *(undefined8 *)(lVar2 + 0x18);
  FUN_141eca8b0(&local_res8,&local_18);
  puVar5 = &DAT_14391f7d0;
  if (local_10 != 0) {
    puVar5 = local_18;
  }
  FUN_141f07780(puVar5,param_3,3,1,0,0);
  if (local_18 != (undefined2 *)0x0) {
    FUN_141de9200();
  }
  uVar4 = (*param_1)();
  lVar2 = *(longlong *)(param_2 + 0x20);
  while (lVar3 = lVar2, lVar3 != 0) {
    param_2 = lVar3;
    lVar2 = *(longlong *)(lVar3 + 0x20);
  }
  local_res8 = *(undefined8 *)(param_2 + 0x18);
  FUN_141eca8b0(&local_res8,&local_18);
  puVar5 = &DAT_14391f7d0;
  if (local_10 != 0) {
    puVar5 = local_18;
  }
  FUN_141f07780(puVar5,param_3,3,2,0,0);
  if (local_18 != (undefined2 *)0x0) {
    FUN_141de9200();
  }
  return uVar4;
}



// ======== d1<-0x1417be830  @ 0x1417bc720 ========

longlong FUN_1417bc720(void)

{
  if (DAT_1459b24d0 == 0) {
    FUN_141f291f0(L"/Script/AT",L"AT_UITips",&DAT_1459b24d0,FUN_1417be830,0x508,0x10000000,0,
                  L"Engine",&LAB_1417bded0,FUN_1407e4520,_guard_check_icall,&LAB_14176a290,
                  &LAB_14083c250,0);
  }
  return DAT_1459b24d0;
}



// ======== d1<-0x1417be860  @ 0x1417bcec0 ========

longlong FUN_1417bcec0(void)

{
  if (DAT_1459b2500 == 0) {
    FUN_141f291f0(L"/Script/AT",L"AT_UITipsDLC6ExtraMode",&DAT_1459b2500,FUN_1417be860,0x400,
                  0x10000000,0,L"Engine",&LAB_1417bdef0,FUN_1407e4520,_guard_check_icall,
                  &LAB_14176a290,&LAB_14083c250,0);
  }
  return DAT_1459b2500;
}



// ======== d1<-0x1417be890  @ 0x1417bd3a0 ========

longlong FUN_1417bd3a0(void)

{
  if (DAT_1459b2528 == 0) {
    FUN_141f291f0(L"/Script/AT",L"AT_UITipsNew",&DAT_1459b2528,FUN_1417be890,0x3e0,0x10000000,0,
                  L"Engine",FUN_1417bdf60,FUN_1407e4520,_guard_check_icall,&LAB_14176a290,
                  &LAB_14083c250,0);
  }
  return DAT_1459b2528;
}



// ======== d1<-0x1417be8c0  @ 0x1417bcc50 ========

longlong FUN_1417bcc50(void)

{
  if (DAT_1459b25e8 == 0) {
    FUN_141f291f0(L"/Script/AT",L"AT_UITrialClear",&DAT_1459b25e8,FUN_1417be8c0,0x420,0x10000000,0,
                  L"Engine",&LAB_1417be120,FUN_1407e4520,_guard_check_icall,&LAB_141766140,
                  &LAB_14083c250,0);
  }
  return DAT_1459b25e8;
}



// ======== d1<-0x1417be8f0  @ 0x1417bcdb0 ========

longlong FUN_1417bcdb0(void)

{
  if (DAT_1459b2660 == 0) {
    FUN_141f291f0(L"/Script/AT",L"AT_UIUtil",&DAT_1459b2660,FUN_1417be8f0,0x28,0x10000000,0,
                  L"Engine",FUN_140a4ac60,FUN_1407e4520,_guard_check_icall,&LAB_140885480,
                  &LAB_14083c250,0);
  }
  return DAT_1459b2660;
}



// ======== d1<-0x1417be920  @ 0x1417bc880 ========

longlong FUN_1417bc880(void)

{
  if (DAT_1459b26b8 == 0) {
    FUN_141f291f0(L"/Script/AT",L"AT_UIWindowManager",&DAT_1459b26b8,FUN_1417be920,0x90,0x10000000,0
                  ,L"Engine",&LAB_1417be1b0,FUN_1407e4520,_guard_check_icall,&LAB_14083c250,
                  &LAB_14083c250,0);
  }
  return DAT_1459b26b8;
}



// ======== d1<-0x1417be980  @ 0x142042ad0 ========

undefined8 FUN_142042ad0(code *param_1,longlong param_2,undefined8 param_3)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  undefined8 uVar4;
  undefined2 *puVar5;
  undefined8 local_res8;
  undefined2 *local_18;
  int local_10;
  
  lVar3 = *(longlong *)(param_2 + 0x20);
  lVar2 = param_2;
  while (lVar1 = lVar3, lVar1 != 0) {
    lVar2 = lVar1;
    lVar3 = *(longlong *)(lVar1 + 0x20);
  }
  local_res8 = *(undefined8 *)(lVar2 + 0x18);
  FUN_141eca8b0(&local_res8,&local_18);
  puVar5 = &DAT_14391f7d0;
  if (local_10 != 0) {
    puVar5 = local_18;
  }
  FUN_141f07780(puVar5,param_3,2,1,0,0);
  if (local_18 != (undefined2 *)0x0) {
    FUN_141de9200();
  }
  uVar4 = (*param_1)();
  lVar2 = *(longlong *)(param_2 + 0x20);
  while (lVar3 = lVar2, lVar3 != 0) {
    param_2 = lVar3;
    lVar2 = *(longlong *)(lVar3 + 0x20);
  }
  local_res8 = *(undefined8 *)(param_2 + 0x18);
  FUN_141eca8b0(&local_res8,&local_18);
  puVar5 = &DAT_14391f7d0;
  if (local_10 != 0) {
    puVar5 = local_18;
  }
  FUN_141f07780(puVar5,param_3,2,2,0,0);
  if (local_18 != (undefined2 *)0x0) {
    FUN_141de9200();
  }
  return uVar4;
}



// ======== d1<-0x1417bee30  @ 0x14203a700 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14203a700(longlong *param_1,undefined8 *param_2)

{
  uint uVar1;
  undefined4 uVar2;
  short *_Src;
  undefined1 *puVar3;
  int iVar4;
  longlong *plVar5;
  longlong lVar6;
  undefined8 *puVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  longlong lVar10;
  ulonglong uVar11;
  longlong lVar13;
  int iVar14;
  longlong *plVar15;
  longlong *plVar16;
  longlong *plVar17;
  undefined1 auStack_448 [32];
  undefined4 local_428;
  undefined1 local_420;
  undefined8 local_418;
  undefined1 *local_408;
  longlong local_400;
  undefined8 local_3f8;
  longlong *local_3f0;
  undefined8 *local_3e8;
  undefined1 local_3e0 [8];
  undefined1 local_3d8 [256];
  undefined1 *local_2d8;
  undefined1 *local_2d0;
  int local_2c8;
  undefined1 local_2b8 [256];
  longlong local_1b8;
  undefined1 local_198 [336];
  ulonglong local_48;
  ulonglong uVar12;
  
  local_48 = DAT_145909780 ^ (ulonglong)auStack_448;
  plVar15 = (longlong *)0x0;
  iVar14 = 0;
  local_408 = (undefined1 *)((ulonglong)local_408 & 0xffffffff00000000);
  plVar5 = plVar15;
  local_3e8 = param_2;
  if ((code *)*param_2 != (code *)0x0) {
    plVar5 = (longlong *)(*(code *)*param_2)();
  }
  if (*param_1 == 0) {
    lVar6 = FUN_1407f7cf0(local_2b8,param_2[2]);
    uVar2 = *(undefined4 *)(param_2 + 3);
    puVar7 = (undefined8 *)FUN_141eb3490(local_3e0,*(undefined8 *)(lVar6 + 0x108),1);
    uVar9 = *puVar7;
    uVar8 = FUN_141fd65d0();
    local_418 = 0;
    local_420 = 0;
    local_428 = 0;
    lVar6 = FUN_14204bcd0(uVar8,plVar5,uVar9,uVar2);
    local_3f0 = plVar15;
    plVar5 = plVar15;
    if (lVar6 != 0) {
      uVar9 = FUN_142035810(local_198);
      plVar5 = (longlong *)FUN_141fd8850(lVar6,uVar9);
      local_3f0 = plVar5;
      FUN_1420361d0(local_198);
    }
    if (local_1b8 != 0) {
      FUN_141de9200();
    }
    *param_1 = (longlong)plVar5;
    iVar4 = *(int *)(param_2 + 8);
    local_400 = 0;
    local_3f8 = 0;
    if (0 < iVar4) {
      FUN_1407f1a80(&local_400,iVar4);
      iVar4 = *(int *)(param_2 + 8);
    }
    plVar17 = (longlong *)param_2[7];
    lVar6 = -1;
    plVar16 = plVar17 + (longlong)iVar4 * 2;
    puVar7 = local_3e8;
    for (; local_3e8 = puVar7, plVar17 != plVar16; plVar17 = plVar17 + 2) {
      lVar13 = *plVar17;
      local_2d8 = (undefined1 *)0x0;
      if (lVar13 == 0) {
        local_2d0 = (undefined1 *)0x0;
        local_2c8 = 0;
      }
      else {
        uVar11 = 0xffffffffffffffff;
        do {
          uVar12 = uVar11;
          uVar11 = uVar12 + 1;
        } while (*(char *)(lVar13 + uVar11) != '\0');
        local_408 = (undefined1 *)((ulonglong)local_408 & 0xffffffff00000000);
        FUN_1407f3d80(&local_408,0x7fffffff,lVar13,uVar11 & 0xffffffff);
        local_2c8 = (int)local_408;
        uVar1 = (int)local_408 + 1;
        if (uVar1 < 0x81) {
          if ((local_2d8 != (undefined1 *)0x0) &&
             (memmove(local_3d8,local_2d8,0), local_2d8 != (undefined1 *)0x0)) {
            lVar10 = 0;
LAB_14203a913:
            local_2d8 = (undefined1 *)FUN_141df5850(local_2d8,lVar10,0);
          }
        }
        else {
          if (local_2d8 != (undefined1 *)0x0) {
            lVar10 = (longlong)(int)uVar1 * 2;
            goto LAB_14203a913;
          }
          if (uVar1 != 0) {
            local_2d8 = (undefined1 *)FUN_141df5850(0,(longlong)(int)uVar1 * 2);
          }
          memmove(local_2d8,local_3d8,0);
        }
        local_408 = local_3d8;
        if (local_2d8 != (undefined1 *)0x0) {
          local_408 = local_2d8;
        }
        local_2d0 = local_408;
        FUN_1407f37f0(&local_408,uVar1,lVar13,(int)uVar12 + 2);
      }
      puVar3 = local_2d0;
      uVar11 = local_3f8;
      lVar13 = (longlong)(int)local_3f8;
      iVar4 = (int)local_3f8 + 1;
      local_3f8 = CONCAT44(local_3f8._4_4_,iVar4);
      if (local_3f8._4_4_ < iVar4) {
        FUN_1407f1790(&local_400,uVar11 & 0xffffffff);
      }
      lVar13 = lVar13 * 0x10 + local_400;
      FUN_141eb3490(lVar13,puVar3,1);
      *(longlong *)(lVar13 + 8) = plVar17[1];
      if (local_2d8 != (undefined1 *)0x0) {
        FUN_141de9200();
      }
      puVar7 = local_3e8;
      plVar5 = local_3f0;
    }
    (**(code **)(*plVar5 + 0x268))
              (plVar5,&local_400,*(undefined1 *)(puVar7 + 5),*(int *)(puVar7 + 1) == 0);
    lVar13 = FUN_1407f7cf0(local_3d8,puVar7[6]);
    _Src = *(short **)(lVar13 + 0x108);
    if ((short *)plVar5[6] != _Src) {
      if ((_Src != (short *)0x0) && (*_Src != 0)) {
        do {
          lVar6 = lVar6 + 1;
        } while (_Src[lVar6] != 0);
        plVar15 = (longlong *)(ulonglong)((int)lVar6 + 1);
      }
      *(undefined4 *)(plVar5 + 7) = 0;
      iVar4 = (int)plVar15;
      if (*(int *)((longlong)plVar5 + 0x3c) != iVar4) {
        FUN_1407f1af0(plVar5 + 6,plVar15);
        iVar14 = (int)plVar5[7];
      }
      *(int *)(plVar5 + 7) = iVar14 + iVar4;
      if (*(int *)((longlong)plVar5 + 0x3c) < iVar14 + iVar4) {
        FUN_1407e2ee0(plVar5 + 6,iVar14);
      }
      if (iVar4 != 0) {
        memcpy((void *)plVar5[6],_Src,(longlong)iVar4 * 2);
      }
    }
    if (local_2d8 != (undefined1 *)0x0) {
      FUN_141de9200();
    }
    if (puVar7[4] != 0) {
      plVar5[0xb] = puVar7[4];
    }
    if (local_400 != 0) {
      FUN_141de9200();
    }
  }
  return;
}



// ======== d1<-0x1417beef0  @ 0x14203c8c0 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14203c8c0(longlong *param_1,undefined8 *param_2)

{
  undefined4 uVar1;
  code *pcVar2;
  code *pcVar3;
  longlong lVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  undefined8 *puVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  longlong lVar11;
  undefined1 auStack_318 [32];
  undefined4 local_2f8;
  ulonglong local_2f0;
  undefined8 local_2e8;
  int local_2d8 [2];
  undefined8 local_2d0;
  undefined1 local_2c8 [16];
  undefined1 local_2b8 [256];
  longlong local_1b8;
  undefined1 local_198 [336];
  ulonglong local_48;
  
  local_48 = DAT_145909780 ^ (ulonglong)auStack_318;
  lVar11 = 0;
  local_2d8[0] = 0;
  pcVar2 = (code *)param_2[1];
  pcVar3 = (code *)param_2[2];
  lVar4 = lVar11;
  if ((code *)*param_2 != (code *)0x0) {
    lVar4 = (*(code *)*param_2)();
  }
  lVar5 = lVar11;
  if (pcVar2 != (code *)0x0) {
    lVar5 = (*pcVar2)();
  }
  lVar6 = lVar11;
  if (pcVar3 != (code *)0x0) {
    lVar6 = (*pcVar3)();
  }
  if (*param_1 == 0) {
    lVar7 = FUN_1407f7cf0(local_2b8,param_2[3]);
    uVar1 = *(undefined4 *)(param_2 + 4);
    puVar8 = (undefined8 *)FUN_141eb3490(local_2c8,*(undefined8 *)(lVar7 + 0x108),1);
    uVar10 = *puVar8;
    uVar9 = FUN_141f28cb0();
    local_2e8 = 0;
    local_2f0 = local_2f0 & 0xffffffffffffff00;
    local_2f8 = 0;
    lVar4 = FUN_14204bcd0(uVar9,lVar4,uVar10,uVar1);
    if (lVar4 != 0) {
      uVar10 = FUN_142035810(local_198);
      local_2e8 = param_2[6];
      local_2f0 = param_2[5];
      local_2f8 = *(undefined4 *)((longlong)param_2 + 0x24);
      lVar11 = FUN_141f1efe0(lVar4,uVar10,lVar5,lVar6);
      FUN_1420361d0(local_198);
    }
    if (local_1b8 != 0) {
      FUN_141de9200();
    }
    *param_1 = lVar11;
    local_2d8[0] = *(int *)(param_2 + 8);
    local_2d0 = param_2[7];
    while (local_2d8[0] != 0) {
      FUN_14203ae50(lVar11,&local_2d0,local_2d8);
    }
    FUN_141fd5900(lVar11,0);
  }
  return;
}



// ======== d2<-0x141f291f0  @ 0x1420384d0 ========

void FUN_1420384d0(int *param_1,int param_2,undefined8 param_3,char param_4)

{
  bool bVar1;
  ulonglong uVar2;
  
  if ((param_4 == '\0') ||
     ((ulonglong)((longlong)*param_1 + *(longlong *)(param_1 + 2)) <
      (*(longlong *)(param_1 + 4) + 0xfU & 0xfffffffffffffff0) + (longlong)param_2)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  if (param_4 == '\0') {
    if (bVar1) goto LAB_142038532;
  }
  else {
    if (bVar1) {
LAB_142038532:
      uVar2 = (longlong)param_2 + (*(longlong *)(param_1 + 4) + 0xfU & 0xfffffffffffffff0);
      *(ulonglong *)(param_1 + 4) = uVar2;
      if (*(ulonglong *)(param_1 + 6) < uVar2) {
        *(ulonglong *)(param_1 + 6) = uVar2;
        return;
      }
      return;
    }
    *(ulonglong *)(param_1 + 6) =
         (*(longlong *)(param_1 + 6) + 0xfU & 0xfffffffffffffff0) + (longlong)param_2;
  }
  FUN_141df35c0((longlong)(int)(param_2 + 0xfU & 0xfffffff0),0);
  return;
}



// ======== d2<-0x141f291f0  @ 0x141eb3490 ========

undefined8 * FUN_141eb3490(undefined8 *param_1,longlong param_2,undefined4 param_3)

{
  if (param_2 != 0) {
    FUN_141ebf920(param_1,param_2,0,param_3,1,0xffffffff);
    return param_1;
  }
  *param_1 = 0;
  return param_1;
}



// ======== d2<-0x141f291f0  @ 0x141f1ece0 ========

undefined8 *
FUN_141f1ece0(undefined8 *param_1,undefined8 param_2,undefined8 param_3,undefined4 param_4,
             uint param_5,undefined8 param_6,undefined8 param_7,undefined4 param_8,
             undefined8 param_9,undefined8 param_10,undefined8 param_11)

{
  FUN_141fff3c0(param_1,0,param_8);
  *(undefined4 *)(param_1 + 8) = param_4;
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[7] = 0;
  *(undefined4 *)((longlong)param_1 + 0x44) = 1;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  param_1[0x10] = 0;
  param_1[0x11] = 0;
  *(undefined4 *)(param_1 + 0x12) = 0xffffffff;
  *param_1 = &PTR_FUN_1447612e8;
  param_1[0x13] = param_9;
  param_1[0x14] = param_10;
  param_1[0x15] = param_11;
  *(undefined4 *)(param_1 + 0x16) = 0;
  *(uint *)((longlong)param_1 + 0xb4) = param_5 | 0x80;
  param_1[0x17] = param_6;
  param_1[0x18] = 0;
  param_1[0x19] = 0;
  param_1[0x1b] = 0;
  param_1[0x1c] = 0;
  param_1[0x1d] = 0;
  param_1[0x1e] = 0;
  param_1[0x1f] = 0;
  param_1[0x20] = 0;
  param_1[0x21] = 0;
  param_1[0x22] = 0;
  param_1[0x25] = 0;
  param_1[0x26] = 0;
  *(undefined4 *)(param_1 + 0x27) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x13c) = 0;
  param_1[0x29] = 0;
  *(undefined4 *)(param_1 + 0x2a) = 0;
  param_1[0x2b] = 0;
  param_1[0x2c] = 0;
  param_1[0x2f] = 0;
  param_1[0x30] = 0;
  *(undefined4 *)(param_1 + 0x31) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x18c) = 0;
  param_1[0x33] = 0;
  *(undefined4 *)(param_1 + 0x34) = 0;
  InitializeSRWLock((PSRWLOCK)(param_1 + 0x35));
  param_1[0x36] = 0;
  param_1[0x37] = 0;
  param_1[0x38] = 0;
  param_1[0x39] = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x3a));
  SetCriticalSectionSpinCount((LPCRITICAL_SECTION)(param_1 + 0x3a),4000);
  param_1[0x3f] = 0;
  param_1[0x40] = 0;
  param_1[0x1b] = param_7;
  return param_1;
}



// ======== d2<-0x141f291f0  @ 0x141f07780 ========

void FUN_141f07780(undefined8 param_1,undefined8 param_2,undefined4 param_3,undefined4 param_4,
                  undefined8 param_5,undefined1 param_6)

{
  undefined8 uVar1;
  
  uVar1 = FUN_141f01c60();
  FUN_141f077f0(uVar1,param_1,param_2,param_3,param_4,param_5,param_6);
  return;
}



// ======== d2<-0x141f291f0  @ 0x142049670 ========

void FUN_142049670(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  longlong *plVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  undefined1 local_res8 [8];
  undefined8 local_res10;
  undefined8 local_38;
  undefined8 local_30;
  undefined8 *local_28;
  undefined8 *local_20;
  
  uVar3 = FUN_142041f50();
  puVar4 = (undefined8 *)FUN_140b40c10(0x10);
  if (puVar4 == (undefined8 *)0x0) {
    puVar4 = (undefined8 *)0x0;
  }
  else {
    *puVar4 = param_1;
    puVar4[1] = 0;
  }
  local_28 = &local_res10;
  local_20 = &local_38;
  local_res10 = param_1;
  local_38 = param_3;
  local_30 = param_2;
  FUN_142034390(uVar3,local_res8,&local_28,0);
  puVar2 = puVar4;
  if (DAT_145b9e670 != (undefined8 *)0x0) {
    plVar1 = (longlong *)((longlong)DAT_145b9e670 + 8);
    DAT_145b9e670 = puVar4;
    *plVar1 = (longlong)puVar4;
    puVar4 = DAT_145b9e668;
    puVar2 = DAT_145b9e670;
  }
  DAT_145b9e670 = puVar2;
  DAT_145b9e668 = puVar4;
  return;
}



// ======== d2<-0x14203a4b0  @ 0x14204f930 ========

void FUN_14204f930(longlong *param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  longlong *plVar3;
  undefined8 uVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  int local_res10 [2];
  
  plVar3 = (longlong *)FUN_142041f50();
  FUN_14088d620(plVar3,local_res10,param_1);
  puVar6 = (undefined8 *)0x0;
  if (local_res10[0] != -1) {
    puVar6 = (undefined8 *)((longlong)local_res10[0] * 0x20 + *plVar3);
  }
  puVar5 = puVar6 + 1;
  if (puVar6 == (undefined8 *)0x0) {
    puVar5 = (undefined8 *)0x0;
  }
  if (puVar5 != (undefined8 *)0x0) {
    uVar1 = puVar5[1];
    uVar2 = *puVar5;
    FUN_140d0d6b0(plVar3,param_1);
    uVar4 = FUN_141f28040();
    (**(code **)(*param_1 + 0x10))(param_1,uVar4,uVar1,uVar2);
  }
  return;
}



// ======== d2<-0x14203a4b0  @ 0x140827be0 ========

void FUN_140827be0(longlong *param_1,int param_2)

{
  int iVar1;
  ulonglong uVar2;
  longlong lVar3;
  
  iVar1 = 0;
  if (param_2 != 0) {
    uVar2 = FUN_141df5250((longlong)param_2 << 2,0);
    iVar1 = (int)(uVar2 >> 2);
    if (iVar1 < param_2) {
      iVar1 = 0x7fffffff;
    }
  }
  if (iVar1 != *(int *)((longlong)param_1 + 0xc)) {
    *(int *)((longlong)param_1 + 0xc) = iVar1;
    if ((*param_1 != 0) || (iVar1 != 0)) {
      lVar3 = FUN_141df5850(*param_1,(longlong)iVar1 << 2,0);
      *param_1 = lVar3;
    }
  }
  return;
}



// ======== d2<-0x14203a4b0  @ 0x141f24110 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141f24110(longlong param_1,undefined8 *param_2,int param_3)

{
  uint uVar1;
  longlong lVar2;
  longlong lVar3;
  undefined1 *_Dst;
  undefined8 *puVar4;
  ulonglong uVar5;
  undefined1 auStack_1c8 [32];
  undefined1 *local_1a8;
  undefined1 local_1a0 [8];
  longlong local_198;
  undefined8 local_190;
  longlong local_188;
  undefined8 *local_180;
  longlong *local_178;
  undefined1 local_170 [8];
  undefined1 local_168 [256];
  undefined1 *local_68;
  undefined1 *local_60;
  int local_58;
  ulonglong local_48;
  ulonglong uVar6;
  
  if (param_3 != 0) {
    local_48 = DAT_145909780 ^ (ulonglong)auStack_1c8;
    local_180 = &local_190;
    local_178 = &local_188;
    local_198 = param_1;
    do {
      _Dst = (undefined1 *)0x0;
      lVar2 = param_2[1];
      lVar3 = (*(code *)*param_2)();
      local_68 = (undefined1 *)0x0;
      *(undefined8 *)(lVar3 + 0x28) = *(undefined8 *)(param_1 + 0x38);
      *(longlong *)(param_1 + 0x38) = lVar3;
      if (lVar2 == 0) {
        local_58 = 0;
        local_60 = (undefined1 *)0x0;
      }
      else {
        uVar5 = 0xffffffffffffffff;
        do {
          uVar6 = uVar5;
          uVar5 = uVar6 + 1;
        } while (*(char *)(lVar2 + uVar5) != '\0');
        local_1a8 = (undefined1 *)((ulonglong)local_1a8 & 0xffffffff00000000);
        FUN_1407f3d80(&local_1a8,0x7fffffff,lVar2,uVar5 & 0xffffffff);
        local_58 = (int)local_1a8;
        uVar1 = (int)local_1a8 + 1;
        if (0x80 < uVar1) {
          if (uVar1 != 0) {
            _Dst = (undefined1 *)FUN_141df5850(0,(longlong)(int)uVar1 * 2);
            local_68 = _Dst;
          }
          memmove(_Dst,local_168,0);
          _Dst = local_68;
        }
        local_1a8 = local_168;
        if (_Dst != (undefined1 *)0x0) {
          local_1a8 = _Dst;
        }
        local_60 = local_1a8;
        FUN_1407f37f0(&local_1a8,uVar1,lVar2,(int)uVar6 + 2);
        param_1 = local_198;
      }
      puVar4 = (undefined8 *)FUN_141eb3490(local_170,local_60,1);
      local_190 = *puVar4;
      local_188 = lVar3;
      FUN_141f1b8b0(param_1 + 0x108,local_1a0,&local_180,0);
      if (local_68 != (undefined1 *)0x0) {
        FUN_141de9200();
      }
      param_2 = param_2 + 2;
      param_3 = param_3 + -1;
    } while (param_3 != 0);
  }
  return;
}



// ======== d2<-0x14203a4b0  @ 0x14203ae50 ========

/* WARNING: Function: _alloca_probe replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14203ae50(undefined8 param_1,undefined8 *param_2,int *param_3)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  code *pcVar3;
  ulonglong uVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  bool bVar9;
  bool bVar10;
  bool bVar11;
  bool bVar12;
  bool bVar13;
  bool bVar14;
  bool bVar15;
  undefined8 *puVar16;
  longlong lVar17;
  undefined8 *puVar18;
  undefined8 uVar19;
  undefined8 *puVar20;
  undefined8 *puVar21;
  undefined8 uVar22;
  int iVar23;
  undefined8 *puVar24;
  undefined1 auStack_4bd8 [32];
  undefined8 local_4bb8;
  undefined8 *local_4bb0;
  undefined8 local_4ba8;
  undefined1 local_4ba0;
  int local_4b98 [2];
  uint local_4b90 [2];
  int *local_4b88;
  undefined8 *local_4b80;
  undefined1 local_4b78 [8];
  undefined1 local_4b70 [8];
  undefined1 local_4b68 [8];
  undefined1 local_4b60 [8];
  undefined1 local_4b58 [8];
  undefined1 local_4b50 [8];
  undefined1 local_4b48 [8];
  undefined1 local_4b40 [8];
  undefined1 local_4b38 [8];
  undefined1 local_4b30 [8];
  undefined1 local_4b28 [8];
  undefined1 local_4b20 [8];
  undefined1 local_4b18 [8];
  undefined1 local_4b10 [8];
  undefined1 local_4b08 [8];
  undefined1 local_4b00 [8];
  undefined1 local_4af8 [8];
  undefined1 local_4af0 [8];
  undefined1 local_4ae8 [8];
  undefined1 local_4ae0 [8];
  undefined1 local_4ad8 [8];
  undefined1 local_4ad0 [8];
  undefined1 local_4ac8 [8];
  undefined1 local_4ac0 [8];
  undefined1 local_4ab8 [8];
  undefined1 local_4ab0 [8];
  undefined1 local_4aa8 [8];
  undefined1 local_4aa0 [8];
  undefined1 local_4a98 [16];
  undefined1 local_4a88 [256];
  longlong local_4988;
  undefined1 local_4968 [256];
  longlong local_4868;
  undefined1 local_4848 [256];
  longlong local_4748;
  undefined1 local_4728 [256];
  longlong local_4628;
  undefined1 local_4608 [256];
  longlong local_4508;
  undefined1 local_44e8 [256];
  longlong local_43e8;
  undefined1 local_43c8 [256];
  longlong local_42c8;
  undefined1 local_42a8 [256];
  longlong local_41a8;
  undefined1 local_4188 [256];
  longlong local_4088;
  undefined1 local_4068 [256];
  longlong local_3f68;
  undefined1 local_3f48 [256];
  longlong local_3e48;
  undefined1 local_3e28 [256];
  longlong local_3d28;
  undefined1 local_3d08 [256];
  longlong local_3c08;
  undefined1 local_3be8 [256];
  longlong local_3ae8;
  undefined1 local_3ac8 [256];
  longlong local_39c8;
  undefined1 local_39a8 [256];
  longlong local_38a8;
  undefined1 local_3888 [256];
  longlong local_3788;
  undefined1 local_3768 [256];
  longlong local_3668;
  undefined1 local_3648 [256];
  longlong local_3548;
  undefined1 local_3528 [256];
  longlong local_3428;
  undefined1 local_3408 [256];
  longlong local_3308;
  undefined1 local_32e8 [256];
  longlong local_31e8;
  undefined1 local_31c8 [256];
  longlong local_30c8;
  undefined1 local_30a8 [256];
  longlong local_2fa8;
  undefined1 local_2f88 [256];
  longlong local_2e88;
  undefined1 local_2e68 [256];
  longlong local_2d68;
  undefined1 local_2d48 [256];
  longlong local_2c48;
  undefined1 local_2c28 [256];
  longlong local_2b28;
  undefined1 local_2b08 [256];
  longlong local_2a08;
  undefined1 local_29e8 [256];
  longlong local_28e8;
  undefined1 local_28c8 [256];
  longlong local_27c8;
  undefined1 local_27a8 [336];
  undefined1 local_2658 [336];
  undefined1 local_2508 [336];
  undefined1 local_23b8 [336];
  undefined1 local_2268 [336];
  undefined1 local_2118 [336];
  undefined1 local_1fc8 [336];
  undefined1 local_1e78 [336];
  undefined1 local_1d28 [336];
  undefined1 local_1bd8 [336];
  undefined1 local_1a88 [336];
  undefined1 local_1938 [336];
  undefined1 local_17e8 [336];
  undefined1 local_1698 [336];
  undefined1 local_1548 [336];
  undefined1 local_13f8 [336];
  undefined1 local_12a8 [336];
  undefined1 local_1158 [336];
  undefined1 local_1008 [336];
  undefined1 local_eb8 [336];
  undefined1 local_d68 [336];
  undefined1 local_c18 [336];
  undefined1 local_ac8 [336];
  undefined1 local_978 [336];
  undefined1 local_828 [336];
  undefined1 local_6d8 [336];
  undefined1 local_588 [336];
  undefined1 local_438 [336];
  undefined1 local_2e8 [336];
  undefined1 local_198 [336];
  ulonglong local_48;
  
  local_48 = DAT_145909780 ^ (ulonglong)auStack_4bd8;
  puVar21 = (undefined8 *)0x0;
  bVar15 = false;
  bVar14 = false;
  bVar13 = false;
  bVar12 = false;
  bVar11 = false;
  bVar10 = false;
  bVar9 = false;
  bVar8 = false;
  bVar7 = false;
  bVar6 = false;
  bVar5 = false;
  local_4b90[0] = 0;
  local_4b98[0] = 0;
  puVar2 = *(undefined4 **)*param_2;
  *param_2 = (undefined8 *)*param_2 + 1;
  local_4b88 = param_3;
  local_4b80 = param_2;
  switch(*puVar2) {
  default:
    lVar17 = FUN_1407f7cf0(local_29e8,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4aa0,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_141ffb390();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar20 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    puVar18 = puVar21;
    if (puVar20 != (undefined8 *)0x0) {
      if (*(code **)(puVar2 + 0xe) != (code *)0x0) {
        puVar21 = (undefined8 *)(**(code **)(puVar2 + 0xe))();
      }
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_198);
      local_4bb8 = uVar4 | 0x8001040000200;
      FUN_141fff470(puVar20,uVar22,0,uVar1);
      puVar20[0xe] = puVar21;
      *puVar20 = &PTR_FUN_144777270;
      *(undefined4 *)((longlong)puVar20 + 0x34) = 1;
      puVar21 = puVar20;
      puVar18 = (undefined8 *)0x1;
    }
    local_2b28 = local_28e8;
    if (puVar18 != (undefined8 *)0x0) {
      FUN_1420361d0(local_198);
      local_2b28 = local_28e8;
    }
    break;
  case 1:
    lVar17 = FUN_1407f7cf0(local_4a88,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b90,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201dad0();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar20 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    puVar18 = puVar21;
    if (puVar20 != (undefined8 *)0x0) {
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_27a8);
      local_4bb8 = uVar4 | 0x8001040000200;
      FUN_141fff470(puVar20,uVar22,0,uVar1);
      *(undefined4 *)((longlong)puVar20 + 0x34) = 1;
      *puVar20 = &PTR_FUN_14477ff30;
      puVar21 = puVar20;
      puVar18 = (undefined8 *)0x2;
    }
    local_2b28 = local_4988;
    if (puVar18 != (undefined8 *)0x0) {
      FUN_1420361d0(local_27a8);
      local_2b28 = local_4988;
    }
    break;
  case 2:
    lVar17 = FUN_1407f7cf0(local_4968,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b78,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d970();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar20 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    puVar18 = puVar21;
    if (puVar20 != (undefined8 *)0x0) {
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_2658);
      local_4bb8 = uVar4 | 0x8001040000200;
      FUN_141fff470(puVar20,uVar22,0,uVar1);
      *(undefined4 *)((longlong)puVar20 + 0x34) = 2;
      *puVar20 = &PTR_FUN_1447802d0;
      puVar21 = puVar20;
      puVar18 = (undefined8 *)&DAT_00000004;
    }
    local_2b28 = local_4868;
    if (puVar18 != (undefined8 *)0x0) {
      FUN_1420361d0(local_2658);
      local_2b28 = local_4868;
    }
    break;
  case 3:
    lVar17 = FUN_1407f7cf0(local_4848,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b70,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_141ffba80();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar20 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    puVar18 = puVar21;
    if (puVar20 != (undefined8 *)0x0) {
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_2508);
      local_4bb8 = uVar4 | 0x8001040000200;
      FUN_141fff470(puVar20,uVar22,0,uVar1);
      *(undefined4 *)((longlong)puVar20 + 0x34) = 4;
      *puVar20 = &PTR_FUN_144777610;
      puVar21 = puVar20;
      puVar18 = (undefined8 *)&DAT_00000008;
    }
    local_2b28 = local_4748;
    if (puVar18 != (undefined8 *)0x0) {
      FUN_1420361d0(local_2508);
      local_2b28 = local_4748;
    }
    break;
  case 4:
    lVar17 = FUN_1407f7cf0(local_4728,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b68,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201da20();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar20 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    puVar18 = puVar21;
    if (puVar20 != (undefined8 *)0x0) {
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_23b8);
      local_4bb8 = uVar4 | 0x8001040000200;
      FUN_141fff470(puVar20,uVar22,0,uVar1);
      *(undefined4 *)((longlong)puVar20 + 0x34) = 8;
      *puVar20 = &PTR_FUN_144780670;
      puVar21 = puVar20;
      puVar18 = (undefined8 *)&DAT_00000010;
    }
    local_2b28 = local_4628;
    if (puVar18 != (undefined8 *)0x0) {
      FUN_1420361d0(local_23b8);
      local_2b28 = local_4628;
    }
    break;
  case 5:
    lVar17 = FUN_1407f7cf0(local_4608,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b60,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d1e0();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar20 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    puVar18 = puVar21;
    if (puVar20 != (undefined8 *)0x0) {
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_2268);
      local_4bb8 = uVar4 | 0x8001040000200;
      FUN_141fff470(puVar20,uVar22,0,uVar1);
      *(undefined4 *)((longlong)puVar20 + 0x34) = 2;
      *puVar20 = &PTR_FUN_144780a10;
      puVar21 = puVar20;
      puVar18 = (undefined8 *)&DAT_00000020;
    }
    local_2b28 = local_4508;
    if (puVar18 != (undefined8 *)0x0) {
      FUN_1420361d0(local_2268);
      local_2b28 = local_4508;
    }
    break;
  case 6:
    lVar17 = FUN_1407f7cf0(local_44e8,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b58,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d290();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar20 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    puVar18 = puVar21;
    if (puVar20 != (undefined8 *)0x0) {
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_2118);
      local_4bb8 = uVar4 | 0x8001040000200;
      FUN_141fff470(puVar20,uVar22,0,uVar1);
      *(undefined4 *)((longlong)puVar20 + 0x34) = 4;
      *puVar20 = &PTR_FUN_144780db0;
      puVar21 = puVar20;
      puVar18 = (undefined8 *)&DAT_00000040;
    }
    local_2b28 = local_43e8;
    if (puVar18 != (undefined8 *)0x0) {
      FUN_1420361d0(local_2118);
      local_2b28 = local_43e8;
    }
    break;
  case 7:
    lVar17 = FUN_1407f7cf0(local_43c8,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b50,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d3f0();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    lVar17 = FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    puVar18 = puVar21;
    if (lVar17 != 0) {
      uVar22 = FUN_142035810(local_1fc8);
      local_4bb8 = *(ulonglong *)(puVar2 + 6);
      puVar21 = (undefined8 *)FUN_142035ea0(lVar17,uVar22,0,puVar2[0xc]);
      puVar18 = (undefined8 *)0x80;
    }
    local_2b28 = local_42c8;
    if ((char)puVar18 < '\0') {
      FUN_1420361d0(local_1fc8);
      local_2b28 = local_42c8;
    }
    break;
  case 8:
    lVar17 = FUN_1407f7cf0(local_42a8,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b48,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d3f0();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    lVar17 = FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    local_2b28 = local_41a8;
    if (lVar17 != 0) {
      uVar22 = FUN_142035810(local_1e78);
      local_4bb8 = *(ulonglong *)(puVar2 + 6);
      puVar21 = (undefined8 *)FUN_142035ea0(lVar17,uVar22,0,puVar2[0xc]);
      FUN_1420361d0(local_1e78);
      local_2b28 = local_41a8;
    }
    break;
  case 9:
    lVar17 = FUN_1407f7cf0(local_4188,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b40,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d3f0();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    lVar17 = FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    local_2b28 = local_4088;
    if (lVar17 != 0) {
      uVar22 = FUN_142035810(local_1d28);
      local_4bb8 = *(ulonglong *)(puVar2 + 6);
      puVar21 = (undefined8 *)FUN_142035ea0(lVar17,uVar22,0,puVar2[0xc]);
      FUN_1420361d0(local_1d28);
      local_2b28 = local_4088;
    }
    break;
  case 10:
    lVar17 = FUN_1407f7cf0(local_4068,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b38,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_141ffb9a0();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    local_2b28 = local_3f68;
    if (puVar18 != (undefined8 *)0x0) {
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_1bd8);
      local_4bb8 = uVar4 | 0x8001040000200;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      *(undefined4 *)((longlong)puVar18 + 0x34) = 4;
      *puVar18 = &PTR_FUN_1447779b0;
      FUN_1420361d0(local_1bd8);
      local_2b28 = local_3f68;
      puVar21 = puVar18;
    }
    break;
  case 0xb:
    lVar17 = FUN_1407f7cf0(local_3f48,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b30,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_141ffb850();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    local_2b28 = local_3e48;
    if (puVar18 != (undefined8 *)0x0) {
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_1a88);
      local_4bb8 = uVar4 | 0x8001040000200;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      *(undefined4 *)((longlong)puVar18 + 0x34) = 8;
      *puVar18 = &PTR_FUN_144777d50;
      FUN_1420361d0(local_1a88);
      local_2b28 = local_3e48;
      puVar21 = puVar18;
    }
    break;
  case 0xc:
    pcVar3 = *(code **)(puVar2 + 0x10);
    local_4b90[0] = 0;
    puVar18 = puVar21;
    if (pcVar3 != (code *)0x0) {
      puVar20 = *(undefined8 **)(puVar2 + 0xe);
      puVar16 = (undefined8 *)FUN_140b40c10(puVar20);
      puVar24 = puVar21;
      if (puVar16 != (undefined8 *)0x0) {
        memset(puVar16,0,(size_t)puVar20);
        puVar24 = puVar16;
      }
      (*pcVar3)(puVar24);
      puVar16 = puVar21;
      if (puVar20 != (undefined8 *)0x0) {
        do {
          if (*(byte *)((longlong)puVar24 + (longlong)puVar16) != 0) {
            local_4b90[0] = (uint)*(byte *)((longlong)puVar24 + (longlong)puVar16);
            puVar18 = puVar16;
            break;
          }
          puVar16 = (undefined8 *)(ulonglong)((int)puVar16 + 1);
          puVar18 = (undefined8 *)0x0;
        } while (puVar16 < puVar20);
      }
      thunk_FUN_141de9200(puVar24);
    }
    lVar17 = FUN_1407f7cf0(local_3e28,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar20 = (undefined8 *)FUN_141eb3490(local_4b28,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar20;
    uVar19 = FUN_141ffa950();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    lVar17 = FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    local_2b28 = local_3d28;
    if (lVar17 != 0) {
      iVar23 = puVar2[0xd];
      uVar22 = FUN_142035810(local_1938);
      local_4ba8 = CONCAT44(local_4ba8._4_4_,puVar2[0xc]);
      local_4bb0 = (undefined8 *)CONCAT44(local_4bb0._4_4_,local_4b90[0]);
      local_4bb8 = *(ulonglong *)(puVar2 + 6);
      local_4ba0 = iVar23 == 1;
      puVar21 = (undefined8 *)FUN_141fff2f0(lVar17,uVar22,0,puVar18);
      FUN_1420361d0(local_1938);
      local_2b28 = local_3d28;
    }
    break;
  case 0xd:
    lVar17 = FUN_1407f7cf0(local_3768,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4af8,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d4a0();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (puVar18 != (undefined8 *)0x0) {
      if (*(code **)(puVar2 + 0xe) != (code *)0x0) {
        puVar21 = (undefined8 *)(**(code **)(puVar2 + 0xe))();
      }
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_1158);
      uVar19 = FUN_141f28040();
      local_4bb8 = uVar4 | 0x8000000000000;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      puVar18[0xf] = puVar21;
      *puVar18 = &PTR_FUN_144781ef8;
      bVar10 = true;
      *(undefined4 *)((longlong)puVar18 + 0x34) = 0x28;
      puVar18[0xe] = uVar19;
      puVar21 = puVar18;
    }
    local_2b28 = local_3668;
    if (bVar10) {
      FUN_1420361d0(local_1158);
      local_2b28 = local_3668;
    }
    break;
  case 0xe:
    lVar17 = FUN_1407f7cf0(local_3be8,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b18,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d600();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (puVar18 != (undefined8 *)0x0) {
      if (*(code **)(puVar2 + 0xe) != (code *)0x0) {
        puVar21 = (undefined8 *)(**(code **)(puVar2 + 0xe))();
      }
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_1698);
      local_4bb8 = uVar4 | 0x8001040000200;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      puVar18[0xe] = puVar21;
      *puVar18 = &PTR_FUN_1447814f0;
      bVar6 = true;
      *(undefined4 *)((longlong)puVar18 + 0x34) = 8;
      puVar21 = puVar18;
    }
    local_2b28 = local_3ae8;
    if (bVar6) {
      FUN_1420361d0(local_1698);
      local_2b28 = local_3ae8;
    }
    break;
  case 0xf:
    lVar17 = FUN_1407f7cf0(local_3ac8,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b10,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d6b0();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (puVar18 != (undefined8 *)0x0) {
      if (*(code **)(puVar2 + 0xe) != (code *)0x0) {
        puVar21 = (undefined8 *)(**(code **)(puVar2 + 0xe))();
      }
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_1548);
      local_4bb8 = uVar4 | 0x8001040000000;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      puVar18[0xe] = puVar21;
      *puVar18 = &PTR_FUN_144781848;
      bVar7 = true;
      *(undefined4 *)((longlong)puVar18 + 0x34) = 0x1c;
      puVar21 = puVar18;
    }
    local_2b28 = local_39c8;
    if (bVar7) {
      FUN_1420361d0(local_1548);
      local_2b28 = local_39c8;
    }
    break;
  case 0x10:
    lVar17 = FUN_1407f7cf0(local_39a8,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b08,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d760();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    lVar17 = FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (lVar17 != 0) {
      if (*(code **)(puVar2 + 0xe) != (code *)0x0) {
        puVar21 = (undefined8 *)(**(code **)(puVar2 + 0xe))();
      }
      uVar22 = FUN_142035810(local_13f8);
      local_4bb8 = *(ulonglong *)(puVar2 + 6);
      local_4bb0 = puVar21;
      puVar21 = (undefined8 *)FUN_142035e50(lVar17,uVar22,0,puVar2[0xc]);
      bVar8 = true;
    }
    local_2b28 = local_38a8;
    if (bVar8) {
      FUN_1420361d0(local_13f8);
      local_2b28 = local_38a8;
    }
    break;
  case 0x11:
    lVar17 = FUN_1407f7cf0(local_3888,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b00,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_141ffb5d0();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (puVar18 != (undefined8 *)0x0) {
      puVar20 = puVar21;
      if (*(code **)(puVar2 + 0x10) != (code *)0x0) {
        puVar20 = (undefined8 *)(**(code **)(puVar2 + 0x10))();
      }
      if (*(code **)(puVar2 + 0xe) != (code *)0x0) {
        puVar21 = (undefined8 *)(**(code **)(puVar2 + 0xe))();
      }
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_12a8);
      if (puVar20 == (undefined8 *)0x0) {
        puVar20 = (undefined8 *)FUN_141f28040();
      }
      local_4bb8 = uVar4 | 0x8001040000200;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      puVar18[0xf] = puVar21;
      *puVar18 = &PTR_FUN_144778b28;
      bVar9 = true;
      *(undefined4 *)((longlong)puVar18 + 0x34) = 8;
      puVar18[0xe] = puVar20;
      puVar21 = puVar18;
    }
    local_2b28 = local_3788;
    if (bVar9) {
      FUN_1420361d0(local_12a8);
      local_2b28 = local_3788;
    }
    break;
  case 0x12:
    lVar17 = FUN_1407f7cf0(local_3d08,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b20,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d810();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    lVar17 = FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (lVar17 != 0) {
      if (*(code **)(puVar2 + 0xe) != (code *)0x0) {
        puVar21 = (undefined8 *)(**(code **)(puVar2 + 0xe))();
      }
      uVar22 = FUN_142035810(local_17e8);
      local_4bb8 = *(ulonglong *)(puVar2 + 6);
      local_4bb0 = puVar21;
      puVar21 = (undefined8 *)FUN_142035e00(lVar17,uVar22,0,puVar2[0xc]);
      bVar5 = true;
    }
    local_2b28 = local_3c08;
    if (bVar5) {
      FUN_1420361d0(local_17e8);
      local_2b28 = local_3c08;
    }
    break;
  case 0x13:
    lVar17 = FUN_1407f7cf0(local_3648,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4af0,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d340();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (puVar18 != (undefined8 *)0x0) {
      if (*(code **)(puVar2 + 0xe) != (code *)0x0) {
        puVar21 = (undefined8 *)(**(code **)(puVar2 + 0xe))();
      }
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_1008);
      local_4bb8 = uVar4 & 0xffffff7ffff7fff7 | 0x1040000200;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      puVar18[0xe] = puVar21;
      *puVar18 = &PTR_FUN_144782250;
      bVar11 = true;
      *(undefined4 *)((longlong)puVar18 + 0x34) = 0x10;
      puVar21 = puVar18;
    }
    local_2b28 = local_3548;
    if (bVar11) {
      FUN_1420361d0(local_1008);
      local_2b28 = local_3548;
    }
    break;
  case 0x14:
    lVar17 = FUN_1407f7cf0(local_3528,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4ae8,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d130();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    local_2b28 = local_3428;
    if (puVar18 != (undefined8 *)0x0) {
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_eb8);
      local_4bb8 = uVar4 | 0x8001040000200;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      *(undefined4 *)((longlong)puVar18 + 0x34) = 8;
      *puVar18 = &PTR_FUN_144782580;
      FUN_1420361d0(local_eb8);
      local_2b28 = local_3428;
      puVar21 = puVar18;
    }
    break;
  case 0x15:
    lVar17 = FUN_1407f7cf0(local_3408,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4ae0,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d8c0();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    local_2b28 = local_3308;
    if (puVar18 != (undefined8 *)0x0) {
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_d68);
      local_4bb8 = uVar4 | 0x8000000000200;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      *(undefined4 *)((longlong)puVar18 + 0x34) = 0x10;
      *puVar18 = &PTR_FUN_1447828b0;
      FUN_1420361d0(local_d68);
      local_2b28 = local_3308;
      puVar21 = puVar18;
    }
    break;
  case 0x16:
    lVar17 = FUN_1407f7cf0(local_32e8,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4b98,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_141ffb280();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    local_2a08 = local_31e8;
    if (puVar18 != (undefined8 *)0x0) {
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_c18);
      local_4bb8 = uVar4 | 0x200;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      *(undefined4 *)((longlong)puVar18 + 0x34) = 0x10;
      *puVar18 = &PTR_FUN_144778e80;
      FUN_1420361d0(local_c18);
      local_2a08 = local_31e8;
      puVar21 = puVar18;
    }
    goto LAB_14203c696;
  case 0x17:
    lVar17 = FUN_1407f7cf0(local_31c8,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4ad8,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201d080();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    lVar17 = FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (lVar17 != 0) {
      uVar22 = FUN_142035810(local_ac8);
      local_4bb8 = *(ulonglong *)(puVar2 + 6);
      puVar21 = (undefined8 *)FUN_14201f190(lVar17,uVar22,0,puVar2[0xc]);
      FUN_1420361d0(local_ac8);
    }
    if (local_30c8 != 0) {
      FUN_141de9200();
    }
    iVar23 = 2;
    goto LAB_14203c794;
  case 0x18:
    lVar17 = FUN_1407f7cf0(local_30a8,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4ad0,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201dc30();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    lVar17 = FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    local_2a08 = local_2fa8;
    if (lVar17 != 0) {
      uVar22 = FUN_142035810(local_978);
      local_4bb8 = *(ulonglong *)(puVar2 + 6);
      puVar21 = (undefined8 *)FUN_14201f1e0(lVar17,uVar22,0,puVar2[0xc]);
      FUN_1420361d0(local_978);
      local_2a08 = local_2fa8;
    }
    goto LAB_14203c696;
  case 0x19:
    lVar17 = FUN_1407f7cf0(local_2f88,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4ac8,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201db80();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    lVar17 = FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (lVar17 != 0) {
      if (*(code **)(puVar2 + 0xe) != (code *)0x0) {
        puVar21 = (undefined8 *)(**(code **)(puVar2 + 0xe))();
      }
      uVar22 = FUN_142035810(local_828);
      local_4bb8 = *(ulonglong *)(puVar2 + 6);
      local_4bb0 = puVar21;
      puVar21 = (undefined8 *)FUN_14201f230(lVar17,uVar22,0,puVar2[0xc]);
      bVar12 = true;
    }
    local_2b28 = local_2e88;
    if (bVar12) {
      FUN_1420361d0(local_828);
      local_2b28 = local_2e88;
    }
    break;
  case 0x1a:
    lVar17 = FUN_1407f7cf0(local_2e68,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4ac0,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_141ffb6c0();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (puVar18 != (undefined8 *)0x0) {
      if (*(code **)(puVar2 + 0xe) != (code *)0x0) {
        puVar21 = (undefined8 *)(**(code **)(puVar2 + 0xe))();
      }
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_6d8);
      local_4bb8 = uVar4 | 0x1000000200;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      puVar18[0xe] = puVar21;
      *puVar18 = &PTR_FUN_1447791b0;
      bVar13 = true;
      *(undefined4 *)((longlong)puVar18 + 0x34) = 0x10;
      puVar21 = puVar18;
    }
    local_2b28 = local_2d68;
    if (bVar13) {
      FUN_1420361d0(local_6d8);
      local_2b28 = local_2d68;
    }
    break;
  case 0x1b:
    lVar17 = FUN_1407f7cf0(local_2d48,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4ab8,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_14201dce0();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (puVar18 != (undefined8 *)0x0) {
      if (*(code **)(puVar2 + 0xe) != (code *)0x0) {
        puVar21 = (undefined8 *)(**(code **)(puVar2 + 0xe))();
      }
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_588);
      local_4bb8 = uVar4 | 0x200;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      puVar18[0xe] = puVar21;
      *puVar18 = &PTR_FUN_144783570;
      bVar14 = true;
      *(undefined4 *)((longlong)puVar18 + 0x34) = 0x10;
      puVar21 = puVar18;
    }
    local_2b28 = local_2c48;
    if (bVar14) {
      FUN_1420361d0(local_588);
      local_2b28 = local_2c48;
    }
    break;
  case 0x1c:
    lVar17 = FUN_1407f7cf0(local_2c28,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4ab0,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_142033a90();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    puVar18 = (undefined8 *)FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (puVar18 != (undefined8 *)0x0) {
      uVar4 = *(ulonglong *)(puVar2 + 6);
      uVar1 = puVar2[0xc];
      uVar22 = FUN_142035810(local_438);
      local_4bb8 = uVar4;
      FUN_141fff470(puVar18,uVar22,0,uVar1);
      *(undefined4 *)((longlong)puVar18 + 0x34) = 0x18;
      *puVar18 = &PTR_FUN_14478bfa8;
      FUN_1420361d0(local_438);
      puVar21 = puVar18;
    }
    break;
  case 0x1d:
    lVar17 = FUN_1407f7cf0(local_2b08,*(undefined8 *)(puVar2 + 2));
    uVar1 = puVar2[4];
    puVar18 = (undefined8 *)FUN_141eb3490(local_4aa8,*(undefined8 *)(lVar17 + 0x108),1);
    uVar22 = *puVar18;
    uVar19 = FUN_141fd6730();
    local_4ba8 = 0;
    local_4bb0 = (undefined8 *)((ulonglong)local_4bb0 & 0xffffffffffffff00);
    local_4bb8 = (ulonglong)local_4bb8._4_4_ << 0x20;
    lVar17 = FUN_14204bcd0(uVar19,param_1,uVar22,uVar1);
    if (lVar17 != 0) {
      if (*(code **)(puVar2 + 0xe) != (code *)0x0) {
        puVar21 = (undefined8 *)(**(code **)(puVar2 + 0xe))();
      }
      uVar22 = FUN_142035810(local_2e8);
      local_4bb8 = *(ulonglong *)(puVar2 + 6);
      local_4bb0 = puVar21;
      puVar21 = (undefined8 *)FUN_141fd8890(lVar17,uVar22,0,puVar2[0xc]);
      bVar15 = true;
    }
    if (bVar15) {
      FUN_1420361d0(local_2e8);
    }
LAB_14203c696:
    if (local_2a08 != 0) {
      FUN_141de9200();
    }
    iVar23 = 1;
    goto LAB_14203c794;
  }
  iVar23 = local_4b98[0];
  if (local_2b28 != 0) {
    FUN_141de9200();
    iVar23 = local_4b98[0];
  }
LAB_14203c794:
  *(undefined4 *)(puVar21 + 6) = puVar2[8];
  if (*(longlong *)(puVar2 + 10) != 0) {
    lVar17 = FUN_1407f7cf0(local_28c8);
    puVar18 = (undefined8 *)FUN_141eb3490(local_4a98,*(undefined8 *)(lVar17 + 0x108),1);
    puVar21[9] = *puVar18;
    if (local_27c8 != 0) {
      FUN_141de9200();
    }
  }
  puVar18 = local_4b80;
  *local_4b88 = *local_4b88 + -1;
  for (; iVar23 != 0; iVar23 = iVar23 + -1) {
    FUN_14203ae50(puVar21,puVar18,local_4b88);
  }
  return;
}



// ======== d2<-0x14203a4b0  @ 0x1407f7cf0 ========

ulonglong FUN_1407f7cf0(ulonglong param_1,longlong param_2)

{
  int iVar1;
  ulonglong uVar2;
  ulonglong local_res8;
  ulonglong uVar3;
  
  *(undefined8 *)(param_1 + 0x100) = 0;
  if (param_2 != 0) {
    uVar2 = 0xffffffffffffffff;
    do {
      uVar3 = uVar2;
      uVar2 = uVar3 + 1;
    } while (*(char *)(uVar2 + param_2) != '\0');
    local_res8 = local_res8 & 0xffffffff00000000;
    FUN_1407f3d80(&local_res8,0x7fffffff,param_2,uVar2 & 0xffffffff);
    *(int *)(param_1 + 0x110) = (int)local_res8;
    iVar1 = (int)local_res8 + 1;
    FUN_140826e60(param_1,0,iVar1,2);
    local_res8 = *(ulonglong *)(param_1 + 0x100);
    if (*(ulonglong *)(param_1 + 0x100) == 0) {
      local_res8 = param_1;
    }
    *(ulonglong *)(param_1 + 0x108) = local_res8;
    FUN_1407f37f0(&local_res8,iVar1,param_2,(int)uVar3 + 2);
    return param_1;
  }
  *(undefined8 *)(param_1 + 0x108) = 0;
  *(undefined4 *)(param_1 + 0x110) = 0;
  return param_1;
}



// ======== d2<-0x14203a4b0  @ 0x1407f1a80 ========

void FUN_1407f1a80(longlong *param_1,int param_2)

{
  int iVar1;
  ulonglong uVar2;
  longlong lVar3;
  
  iVar1 = 0;
  if (param_2 != 0) {
    uVar2 = FUN_141df5250((longlong)param_2 << 4,0);
    iVar1 = (int)(uVar2 >> 4);
    if (iVar1 < param_2) {
      iVar1 = 0x7fffffff;
    }
  }
  if (iVar1 != *(int *)((longlong)param_1 + 0xc)) {
    *(int *)((longlong)param_1 + 0xc) = iVar1;
    if ((*param_1 != 0) || (iVar1 != 0)) {
      lVar3 = FUN_141df5850(*param_1,(longlong)iVar1 << 4,0);
      *param_1 = lVar3;
    }
  }
  return;
}



// ======== d2<-0x14203a4b0  @ 0x1407f1790 ========

void FUN_1407f1790(longlong *param_1)

{
  uint uVar1;
  int iVar2;
  ulonglong uVar3;
  longlong lVar4;
  
  uVar1 = *(uint *)(param_1 + 1);
  lVar4 = 4;
  if ((*(int *)((longlong)param_1 + 0xc) != 0) || (4 < uVar1)) {
    lVar4 = (longlong)(int)uVar1 + 0x10 + ((ulonglong)((longlong)(int)uVar1 * 3) >> 3);
  }
  uVar3 = FUN_141df5250(lVar4 << 4,0);
  iVar2 = (int)(uVar3 >> 4);
  if (iVar2 < (int)uVar1) {
    iVar2 = 0x7fffffff;
  }
  *(int *)((longlong)param_1 + 0xc) = iVar2;
  if ((*param_1 != 0) || (iVar2 != 0)) {
    lVar4 = FUN_141df5850(*param_1,(longlong)iVar2 << 4,0);
    *param_1 = lVar4;
  }
  return;
}



// ======== d2<-0x14203a4b0  @ 0x141fd5900 ========

void FUN_141fd5900(longlong *param_1,undefined1 param_2)

{
  undefined1 local_98 [144];
  
  FUN_141eaae00(local_98);
  (**(code **)(*param_1 + 600))(param_1,local_98,param_2);
  FUN_141eab520(local_98);
  return;
}



// ======== d2<-0x14203a4b0  @ 0x143820a10 ========

/* WARNING: This is an inlined function */
/* Library Function - Single Match
    __security_check_cookie
   
   Libraries: Visual Studio 2015, Visual Studio 2017, Visual Studio 2019 */

void __cdecl __security_check_cookie(uintptr_t _StackCookie)

{
  if ((_StackCookie == DAT_145909780) && ((short)(_StackCookie >> 0x30) == 0)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __report_gsfailure(_StackCookie);
}



// ======== d2<-0x14203aad0  @ 0x141f282a0 ========

longlong * FUN_141f282a0(void)

{
  longlong *plVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  longlong lVar4;
  longlong *plVar5;
  undefined1 local_res8 [8];
  undefined8 in_stack_ffffffffffffffa8;
  undefined4 uVar6;
  ulonglong uVar7;
  undefined8 in_stack_ffffffffffffffc0;
  undefined4 uVar8;
  
  uVar6 = (undefined4)((ulonglong)in_stack_ffffffffffffffa8 >> 0x20);
  uVar8 = (undefined4)((ulonglong)in_stack_ffffffffffffffc0 >> 0x20);
  if (DAT_145b9c890 == (longlong *)0x0) {
    uVar2 = FUN_1420384d0(&DAT_145b9e540,0x208,8,1);
    DAT_145b9c890 = (longlong *)uVar2;
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,L"DelegateFunction",1);
    uVar7 = 0x100000000000;
    DAT_145b9c890 =
         (longlong *)
         FUN_141f1ece0(uVar2,0,*puVar3,0xb8,CONCAT44(uVar6,0x10000000),0x100000000000,L"Engine",
                       CONCAT44(uVar8,199),FUN_141f1c5d0,FUN_141f1c890,_guard_check_icall);
    lVar4 = FUN_14205cf60();
    plVar5 = (longlong *)FUN_141f28760();
    plVar1 = DAT_145b9c890;
    uVar7 = uVar7 & 0xffffffffffffff00;
    uVar6 = 0;
    FUN_141f07780(L"/Script/CoreUObject",L"DelegateFunction",0,1,0,uVar7);
    if (plVar5 == plVar1) {
      plVar5 = (longlong *)0x0;
    }
    (**(code **)(*plVar1 + 0x2a0))(plVar1,plVar5);
    plVar1[0x18] = lVar4;
    (**(code **)(*plVar1 + 8))(plVar1);
    if ((*(uint *)(plVar1 + 1) >> 0x1a & 1) == 0) {
      FUN_142049670(plVar1,L"/Script/CoreUObject",L"DelegateFunction");
    }
    else {
      if (DAT_145b9c8a0 == 0) {
        uVar7 = CONCAT44((int)(uVar7 >> 0x20),0x10000000);
        FUN_141f291f0(L"/Script/CoreUObject",L"DynamicClass",&DAT_145b9c8a0,_guard_check_icall,
                      CONCAT44(uVar6,0x270),uVar7,0,L"Engine",FUN_141f1c610,FUN_141f1c930,
                      FUN_141f21430,FUN_141f28040,&LAB_141fd55f0,0);
      }
      (**(code **)(*plVar1 + 0x10))(plVar1,DAT_145b9c8a0,L"/Script/CoreUObject",L"DelegateFunction")
      ;
    }
    FUN_141f07780(L"/Script/CoreUObject",L"DelegateFunction",0,2,0,uVar7 & 0xffffffffffffff00);
  }
  return DAT_145b9c890;
}



// ======== d2<-0x14203aad0  @ 0x14204bcd0 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

longlong *
FUN_14204bcd0(longlong param_1,longlong param_2,longlong param_3,ulonglong param_4,uint param_5,
             char param_6,undefined1 *param_7)

{
  uint *puVar1;
  int iVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  undefined1 uVar6;
  char cVar7;
  DWORD DVar8;
  longlong *plVar9;
  undefined8 *puVar10;
  uint uVar11;
  uint uVar12;
  ulonglong uVar13;
  undefined2 *puVar14;
  longlong *plVar15;
  longlong lVar16;
  uint uVar17;
  ulonglong uVar18;
  undefined2 *puVar19;
  undefined2 *puVar20;
  longlong *plVar21;
  longlong lVar22;
  undefined2 *puVar23;
  longlong local_res8;
  longlong local_res10;
  longlong local_res18;
  uint local_res20;
  LARGE_INTEGER local_98;
  undefined2 *local_90;
  undefined8 local_88;
  longlong local_80;
  undefined2 *local_78;
  int local_70;
  longlong local_68 [2];
  longlong local_58 [3];
  
  uVar17 = (uint)param_4;
  plVar15 = (longlong *)0x0;
  uVar18 = param_4 & 0xffffffff;
  uVar13 = param_4 >> 4 & 0xfffff01;
  local_90 = (undefined2 *)((ulonglong)local_90 & 0xffffffff00000000);
  local_98.s.LowPart = (DWORD)uVar13;
  local_res8 = param_1;
  local_res10 = param_2;
  local_res20 = uVar17;
  if ((param_4 >> 4 & 1) != 0) {
    local_res18 = param_3;
    plVar9 = (longlong *)FUN_141f27040(param_1,&local_80);
    local_res20 = uVar17 & 0xffffcfff;
    uVar18 = (ulonglong)local_res20;
    param_3 = *plVar9;
  }
  local_90 = (undefined2 *)0x0;
  local_res18 = param_3;
  if (param_3 == 0) {
    local_res8 = 0;
    plVar9 = (longlong *)FUN_142045cb0(&local_res8,param_2,param_1);
    local_res18 = *plVar9;
    plVar9 = plVar15;
  }
  else {
    plVar9 = (longlong *)FUN_14205b720(0);
    if (plVar9 != (longlong *)0x0) {
      for (lVar16 = plVar9[2]; lVar16 != 0; lVar16 = *(longlong *)(lVar16 + 0x30)) {
        if (lVar16 == param_1) goto LAB_14204bfac;
      }
      if (param_2 == 0) {
        plVar21 = plVar15;
        puVar23 = (undefined2 *)&DAT_144783fdc;
      }
      else {
        local_90 = (undefined2 *)0x0;
        local_88 = 0;
        FUN_142042130(param_2,0,&local_90);
        plVar21 = (longlong *)0x1;
        puVar23 = &DAT_14391f7d0;
        if ((int)local_88 != 0) {
          puVar23 = local_90;
        }
      }
      puVar10 = (undefined8 *)FUN_142041d60(plVar9,local_58,0);
      if (*(int *)(puVar10 + 1) == 0) {
        puVar20 = &DAT_14391f7d0;
      }
      else {
        puVar20 = (undefined2 *)*puVar10;
      }
      puVar10 = (undefined8 *)FUN_141eca8b0(&local_res18,local_68);
      if (*(int *)(puVar10 + 1) == 0) {
        puVar19 = &DAT_14391f7d0;
      }
      else {
        puVar19 = (undefined2 *)*puVar10;
      }
      local_80 = *(longlong *)(local_res8 + 0x18);
      FUN_141eca8b0(&local_80,&local_78);
      puVar14 = &DAT_14391f7d0;
      if (local_70 != 0) {
        puVar14 = local_78;
      }
      FUN_141e49950("Unknown",0x959,
                    L"Objects have the same fully qualified name but different paths.\n\tNew Object: %s %s.%s\n\tExisting Object: %s"
                    ,puVar14,puVar23,puVar19,puVar20);
      if (local_78 != (undefined2 *)0x0) {
        FUN_141de9200();
      }
      if (local_68[0] != 0) {
        FUN_141de9200();
      }
      if (local_58[0] != 0) {
        FUN_141de9200();
      }
      if ((plVar21 != (longlong *)0x0) && (local_90 != (undefined2 *)0x0)) {
        FUN_141de9200();
      }
      bVar4 = false;
      if (local_res10 == 0) {
        puVar23 = (undefined2 *)&DAT_14478403c;
      }
      else {
        local_90 = (undefined2 *)0x0;
        local_88 = 0;
        bVar4 = true;
        FUN_142042130(local_res10,0,&local_90);
        puVar23 = &DAT_14391f7d0;
        if ((int)local_88 != 0) {
          puVar23 = local_90;
        }
      }
      puVar10 = (undefined8 *)FUN_142041d60(plVar9,local_68,0);
      if (*(int *)(puVar10 + 1) == 0) {
        puVar20 = &DAT_14391f7d0;
      }
      else {
        puVar20 = (undefined2 *)*puVar10;
      }
      puVar10 = (undefined8 *)FUN_141eca8b0(&local_res18,local_58);
      if (*(int *)(puVar10 + 1) == 0) {
        puVar19 = &DAT_14391f7d0;
      }
      else {
        puVar19 = (undefined2 *)*puVar10;
      }
      local_80 = *(longlong *)(local_res8 + 0x18);
      FUN_141eca8b0(&local_80,&local_78);
      puVar14 = &DAT_14391f7d0;
      if (local_70 != 0) {
        puVar14 = local_78;
      }
      FUN_141e40cb0(&DAT_144783f6d,"Unknown",0x959,
                    L"Objects have the same fully qualified name but different paths.\n\tNew Object: %s %s.%s\n\tExisting Object: %s"
                    ,puVar14,puVar23,puVar19,puVar20);
      if (local_78 != (undefined2 *)0x0) {
        FUN_141de9200();
      }
      if (local_58[0] != 0) {
        FUN_141de9200();
      }
      if (local_68[0] != 0) {
        FUN_141de9200();
      }
      if ((bVar4) && (local_90 != (undefined2 *)0x0)) {
        FUN_141de9200();
      }
      uVar18 = (ulonglong)local_res20;
      uVar13 = (ulonglong)local_98.s.LowPart;
      param_1 = local_res8;
    }
  }
LAB_14204bfac:
  cVar7 = (char)uVar13;
  iVar2 = *(int *)(param_1 + 0x40);
  lVar16 = (longlong)iVar2;
  bVar4 = false;
  bVar5 = false;
  local_res8._0_4_ = -1;
  local_res20 = local_res20 & 0xffffff00;
  local_80 = 0;
  local_90 = (undefined2 *)CONCAT44(local_90._4_4_,iVar2);
  uVar17 = (uint)uVar18;
  if (plVar9 == (longlong *)0x0) {
    if ((DAT_1456da000 == '\0') ||
       ((ulonglong)(DAT_145b9e540 + DAT_145b9e548) <
        (DAT_145b9e550 + 0xf & 0xfffffffffffffff0) + lVar16)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (DAT_1456da000 == '\0') {
      if (bVar3) goto LAB_14204c053;
LAB_14204c036:
      plVar9 = (longlong *)FUN_141df35c0((longlong)(int)(iVar2 + 0xfU & 0xfffffff0));
    }
    else {
      if (!bVar3) {
        DAT_145b9e558 = (DAT_145b9e558 + 0xf & 0xfffffffffffffff0) + lVar16;
        goto LAB_14204c036;
      }
LAB_14204c053:
      plVar9 = (longlong *)(DAT_145b9e550 + 0xf & 0xfffffffffffffff0);
      DAT_145b9e550 = (longlong)plVar9 + lVar16;
      uVar13 = DAT_145b9e550;
      if (DAT_145b9e558 < DAT_145b9e550) goto LAB_14204c18b;
    }
    bVar4 = false;
    uVar13 = DAT_145b9e558;
LAB_14204c18b:
    DAT_145b9e558 = uVar13;
    bVar3 = false;
    if (cVar7 != '\0') goto LAB_14204c251;
  }
  else {
    DAT_145b9e678 = (**(code **)(*plVar9 + 0x168))(plVar9);
    local_80 = FUN_1420590d0(plVar9);
    local_res8._0_4_ = FUN_142059130(plVar9);
    iVar2 = *(int *)((longlong)plVar9 + 0xc);
    plVar21 = plVar15;
    if (iVar2 < (int)DAT_145b9e804) {
      uVar11 = iVar2 >> 0x1f & 0xffff;
      uVar12 = iVar2 + uVar11;
      plVar21 = (longlong *)
                (*(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar12 >> 0x10) * 8) +
                (longlong)(int)((uVar12 & 0xffff) - uVar11) * 0x18);
    }
    param_5 = param_5 | *(uint *)(plVar21 + 1) & 0x42000000;
    if (cVar7 != '\0') {
      *(uint *)(plVar9 + 1) = *(uint *)(plVar9 + 1) | uVar17;
      if (iVar2 < (int)DAT_145b9e804) {
        uVar11 = iVar2 >> 0x1f & 0xffff;
        uVar12 = iVar2 + uVar11;
        puVar1 = (uint *)(*(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar12 >> 0x10) * 8) + 8 +
                         (longlong)(int)((uVar12 & 0xffff) - uVar11) * 0x18);
        *puVar1 = *puVar1 | param_5;
        *(uint *)(plVar9 + 1) = *(uint *)(plVar9 + 1) & 0xffffcfff;
      }
      else {
        _DAT_00000008 = _DAT_00000008 | param_5;
        *(uint *)(plVar9 + 1) = *(uint *)(plVar9 + 1) & 0xffffcfff;
      }
LAB_14204c184:
      bVar4 = true;
      local_res20 = CONCAT31(local_res20._1_3_,1);
      uVar13 = DAT_145b9e558;
      goto LAB_14204c18b;
    }
    if ((param_6 != '\0') && (plVar9[4] != 0)) {
      if ((*(uint *)(plVar9[4] + 8) >> 4 & 1) == 0) {
        lVar16 = *(longlong *)(plVar9[2] + 0x100);
        lVar22 = FUN_142040b00(plVar9);
        if (lVar22 == lVar16) goto LAB_14204c198;
        cVar7 = (char)local_98.s.LowPart;
      }
      goto LAB_14204c184;
    }
LAB_14204c198:
    if ((*(byte *)((longlong)plVar9 + 10) & 1) == 0) {
      FUN_142041d60(plVar9,local_58,0);
      FUN_142003f60(plVar9);
      bVar4 = false;
      cVar7 = (**(code **)(*plVar9 + 0x90))(plVar9);
      if (cVar7 == '\0') {
        do {
          if ((!bVar4) && (DAT_145a98232 == '\0')) {
            QueryPerformanceCounter(&local_98);
            bVar4 = true;
          }
          FUN_141ec7e50(0);
          cVar7 = (**(code **)(*plVar9 + 0x90))(plVar9);
        } while (cVar7 == '\0');
        if (bVar4) {
          QueryPerformanceCounter(&local_98);
        }
      }
      FUN_142003f90(plVar9);
      if (local_58[0] != 0) {
        FUN_141de9200();
      }
    }
    (**(code **)*plVar9)(plVar9);
    bVar4 = false;
    bVar3 = true;
  }
  bVar5 = bVar3;
  if (((*(uint *)(param_1 + 0xb4) >> 3 & 1) != 0) && ((uVar18 >> 5 & 1) == 0)) {
    uVar17 = uVar17 | 0x40;
  }
LAB_14204c251:
  if (bVar4) {
    *(uint *)(plVar9 + 1) = *(uint *)(plVar9 + 1) | uVar17;
    iVar2 = *(int *)((longlong)plVar9 + 0xc);
    plVar21 = plVar15;
    if (iVar2 < (int)DAT_145b9e804) {
      uVar12 = iVar2 >> 0x1f & 0xffff;
      uVar17 = iVar2 + uVar12;
      plVar21 = (longlong *)
                (*(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar17 >> 0x10) * 8) +
                (longlong)(int)((uVar17 & 0xffff) - uVar12) * 0x18);
    }
    *(uint *)(plVar21 + 1) = *(uint *)(plVar21 + 1) | param_5;
  }
  else {
    memset(plVar9,0,(longlong)(int)local_90);
    *plVar9 = (longlong)&PTR_FUN_144776d98;
    *(uint *)(plVar9 + 1) = uVar17 | 0x200;
    *(undefined4 *)((longlong)plVar9 + 0xc) = 0xffffffff;
    plVar9[2] = param_1;
    plVar9[3] = 0;
    plVar9[4] = local_res10;
    FUN_142037040(plVar9,local_res18,param_5);
  }
  lVar16 = local_80;
  if (bVar5) {
    lVar22 = (longlong)(int)local_res8;
    FUN_14205b290(plVar9,local_80,(int)local_res8,0);
    if (lVar16 != 0) {
      *(longlong **)(lVar22 * 0x88 + 0x48 + *(longlong *)(lVar16 + 0x18)) = plVar9;
    }
  }
  cVar7 = (*(code *)PTR_caseD_42_1456d8920)();
  if (cVar7 == '\0') {
    iVar2 = *(int *)((longlong)plVar9 + 0xc);
    plVar21 = plVar15;
    if (iVar2 < (int)DAT_145b9e804) {
      uVar12 = iVar2 >> 0x1f & 0xffff;
      uVar17 = iVar2 + uVar12;
      plVar21 = (longlong *)
                (*(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar17 >> 0x10) * 8) +
                (longlong)(int)((uVar17 & 0xffff) - uVar12) * 0x18);
    }
    *(uint *)(plVar21 + 1) = *(uint *)(plVar21 + 1) & 0xf7ffffff;
    iVar2 = *(int *)((longlong)plVar9 + 0xc);
    plVar21 = plVar15;
    if (iVar2 < (int)DAT_145b9e804) {
      uVar12 = iVar2 >> 0x1f & 0xffff;
      uVar17 = iVar2 + uVar12;
      plVar21 = (longlong *)
                (*(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar17 >> 0x10) * 8) +
                (longlong)(int)((uVar17 & 0xffff) - uVar12) * 0x18);
    }
    if (((*(uint *)(plVar21 + 1) >> 0x1a & 1) != 0) &&
       ((DAT_145a8c1d0 == '\0' || (DVar8 = GetCurrentThreadId(), DVar8 == DAT_145a8c1c0)))) {
      iVar2 = *(int *)((longlong)plVar9 + 0xc);
      if (iVar2 < (int)DAT_145b9e804) {
        uVar12 = iVar2 >> 0x1f & 0xffff;
        uVar17 = iVar2 + uVar12;
        plVar15 = (longlong *)
                  (*(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar17 >> 0x10) * 8) +
                  (longlong)(int)((uVar17 & 0xffff) - uVar12) * 0x18);
      }
      *(uint *)(plVar15 + 1) = *(uint *)(plVar15 + 1) & 0xfbffffff;
    }
  }
  else {
    uVar6 = (undefined1)local_res20;
    FUN_141f07180(plVar9,(undefined1)local_res20);
    local_res20._0_1_ = uVar6;
  }
  if (param_7 != (undefined1 *)0x0) {
    *param_7 = (undefined1)local_res20;
  }
  return plVar9;
}



// ======== d2<-0x14203aad0  @ 0x142035810 ========

undefined8 * FUN_142035810(undefined8 *param_1)

{
  int iVar1;
  longlong lVar2;
  undefined1 local_res8 [8];
  code *local_18;
  undefined1 *local_10;
  
  *(undefined1 *)((longlong)param_1 + 0x12) = 1;
  *param_1 = 0;
  param_1[1] = 0;
  *(undefined2 *)(param_1 + 2) = 0;
  param_1[3] = 0;
  param_1[0x14] = 0;
  param_1[0x15] = 0;
  param_1[0x26] = 0;
  param_1[0x27] = 0;
  *(byte *)(param_1 + 0x29) = *(byte *)(param_1 + 0x29) & 0xfe;
  param_1[0x28] = 0;
  local_res8[0] = 0;
  local_10 = local_res8;
  local_18 = FUN_14088d010;
  lVar2 = FUN_141de9cd0(&local_18,&DAT_1453eae98);
  *(int *)(lVar2 + 100) = *(int *)(lVar2 + 100) + 1;
  param_1[0x28] = *(undefined8 *)(lVar2 + 0x68);
  *(undefined8 *)(lVar2 + 0x68) = *param_1;
  iVar1 = *(int *)(lVar2 + 0x18);
  *(int *)(lVar2 + 0x18) = iVar1 + 1;
  if (*(int *)(lVar2 + 0x1c) < iVar1 + 1) {
    FUN_1407f1510((longlong *)(lVar2 + 0x10),iVar1);
  }
  *(undefined8 **)(*(longlong *)(lVar2 + 0x10) + (longlong)iVar1 * 8) = param_1;
  return param_1;
}



// ======== d2<-0x14203aad0  @ 0x141f1eec0 ========

undefined8 *
FUN_141f1eec0(undefined8 *param_1,undefined8 param_2,longlong param_3,undefined4 param_4,
             ulonglong param_5)

{
  int iVar1;
  int iVar2;
  
  FUN_142035bd0();
  param_1[6] = param_3;
  *param_1 = &PTR_FUN_144760a48;
  param_1[7] = 0;
  if (param_5 == 0) {
    if (param_3 == 0) {
      param_5 = 0;
    }
    else {
      param_5 = (ulonglong)*(uint *)(param_3 + 0x40);
    }
  }
  *(int *)(param_1 + 8) = (int)param_5;
  iVar2 = 1;
  if (param_3 != 0) {
    iVar2 = *(int *)(param_3 + 0x44);
  }
  iVar1 = 1;
  if (0 < iVar2) {
    iVar1 = iVar2;
  }
  *(int *)((longlong)param_1 + 0x44) = iVar1;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  param_1[0x10] = 0;
  *param_1 = &PTR_FUN_144761018;
  *(undefined4 *)(param_1 + 0x11) = param_4;
  *(undefined4 *)((longlong)param_1 + 0x92) = 0;
  param_1[0x13] = 0;
  param_1[0x14] = 0;
  *(undefined4 *)(param_1 + 0x15) = 0;
  return param_1;
}



// ======== d2<-0x14203aad0  @ 0x1420361d0 ========

void FUN_1420361d0(longlong *param_1)

{
  longlong *plVar1;
  char cVar2;
  longlong lVar3;
  longlong *plVar4;
  undefined1 local_res8 [8];
  code *local_18;
  undefined1 *local_10;
  
  if ((*(byte *)(param_1 + 0x29) & 1) == 0) {
    local_res8[0] = 0;
    local_10 = local_res8;
    local_18 = FUN_14088d010;
    lVar3 = FUN_141de9cd0(&local_18,&DAT_1453eae98);
    *(int *)(lVar3 + 0x18) = *(int *)(lVar3 + 0x18) + -1;
    *(int *)(lVar3 + 100) = *(int *)(lVar3 + 100) + -1;
    *(longlong *)(lVar3 + 0x68) = param_1[0x28];
    lVar3 = *param_1;
  }
  else {
    lVar3 = *param_1;
    if (lVar3 == 0) goto LAB_14203629b;
  }
  plVar1 = *(longlong **)(lVar3 + 0x10);
  plVar4 = (longlong *)FUN_14205cf60();
  if (((plVar1 != plVar4) && (param_1[1] == 0)) && (plVar1[2] != 0)) {
    lVar3 = plVar1[0x20];
    if (lVar3 == 0) {
      (**(code **)(*plVar1 + 0x340))(plVar1);
      lVar3 = plVar1[0x20];
    }
    param_1[1] = lVar3;
  }
  cVar2 = FUN_141f03e90();
  if (cVar2 == '\0') {
    lVar3 = FUN_141ef7730(param_1);
    if (lVar3 != 0) {
      *(byte *)(lVar3 + 0x148) = *(byte *)(lVar3 + 0x148) | 1;
      goto LAB_14203629b;
    }
  }
  FUN_142046c20(param_1);
LAB_14203629b:
  if (param_1[0x26] != 0) {
    FUN_141de9200();
  }
  if (param_1[0x14] != 0) {
    FUN_141de9200();
    return;
  }
  return;
}



// ======== d2<-0x14203aad0  @ 0x141f28760 ========

longlong * FUN_141f28760(void)

{
  longlong lVar1;
  longlong *plVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  longlong *plVar5;
  undefined1 local_res8 [8];
  undefined8 in_stack_ffffffffffffff98;
  undefined4 uVar6;
  ulonglong uVar7;
  undefined8 in_stack_ffffffffffffffb0;
  undefined4 uVar8;
  
  uVar6 = (undefined4)((ulonglong)in_stack_ffffffffffffff98 >> 0x20);
  uVar8 = (undefined4)((ulonglong)in_stack_ffffffffffffffb0 >> 0x20);
  if (DAT_145b9c880 == (longlong *)0x0) {
    uVar3 = FUN_1420384d0(&DAT_145b9e540,0x208,8,1);
    DAT_145b9c880 = (longlong *)uVar3;
    puVar4 = (undefined8 *)FUN_141eb3490(local_res8,L"Function",1);
    uVar7 = 0x80000;
    DAT_145b9c880 =
         (longlong *)
         FUN_141f1ece0(uVar3,0,*puVar4,0xb8,CONCAT44(uVar6,0x10000000),0x80000,L"Engine",
                       CONCAT44(uVar8,199),FUN_141f1c5d0,FUN_141f1caa0,_guard_check_icall);
    if (DAT_145b9c7e0 == 0) {
      uVar7 = CONCAT44((int)(uVar7 >> 0x20),0x10000000);
      FUN_141f291f0(L"/Script/CoreUObject",L"Class",&DAT_145b9c7e0,_guard_check_icall,0x208,uVar7,
                    0x20,L"Engine",&LAB_141f1c5b0,FUN_141f1c810,FUN_141f21150,&LAB_141f1a370,
                    &LAB_141fd55f0,0);
    }
    lVar1 = DAT_145b9c7e0;
    plVar5 = (longlong *)FUN_141f28f10();
    plVar2 = DAT_145b9c880;
    uVar7 = uVar7 & 0xffffffffffffff00;
    uVar6 = 0;
    FUN_141f07780(L"/Script/CoreUObject",L"Function",0,1,0,uVar7);
    if (plVar5 == plVar2) {
      plVar5 = (longlong *)0x0;
    }
    (**(code **)(*plVar2 + 0x2a0))(plVar2,plVar5);
    plVar2[0x18] = lVar1;
    (**(code **)(*plVar2 + 8))(plVar2);
    if ((*(uint *)(plVar2 + 1) >> 0x1a & 1) == 0) {
      FUN_142049670(plVar2,L"/Script/CoreUObject",L"Function");
    }
    else {
      if (DAT_145b9c8a0 == 0) {
        uVar7 = CONCAT44((int)(uVar7 >> 0x20),0x10000000);
        FUN_141f291f0(L"/Script/CoreUObject",L"DynamicClass",&DAT_145b9c8a0,_guard_check_icall,
                      CONCAT44(uVar6,0x270),uVar7,0,L"Engine",FUN_141f1c610,FUN_141f1c930,
                      FUN_141f21430,FUN_141f28040,&LAB_141fd55f0,0);
      }
      (**(code **)(*plVar2 + 0x10))(plVar2,DAT_145b9c8a0,L"/Script/CoreUObject",L"Function");
    }
    FUN_141f07780(L"/Script/CoreUObject",L"Function",0,2,0,uVar7 & 0xffffffffffffff00);
  }
  return DAT_145b9c880;
}



// ======== d2<-0x140d487c0  @ 0x142035bd0 ========

undefined8 * FUN_142035bd0(undefined8 *param_1,longlong *param_2)

{
  *param_1 = &PTR_FUN_143945ba8;
  FUN_142006940();
  if (((undefined8 *)*param_2 != (undefined8 *)0x0) && ((undefined8 *)*param_2 != param_1)) {
    FUN_141e49950("Unknown",0xa21,
                  L"UObject(const FObjectInitializer&) constructor called but it\'s not the object that\'s currently being constructed with NewObject. Maybe you are trying to construct it on the stack, which is not supported."
                 );
    FUN_141e40cb0(&DAT_1447840af,"Unknown",0xa21,
                  L"UObject(const FObjectInitializer&) constructor called but it\'s not the object that\'s currently being constructed with NewObject. Maybe you are trying to construct it on the stack, which is not supported."
                 );
  }
  *param_2 = (longlong)param_1;
  *(undefined1 *)((longlong)param_2 + 0x12) = 0;
  return param_1;
}



// ======== d2<-0x140d487c0  @ 0x143149bc0 ========

longlong FUN_143149bc0(void)

{
  if (DAT_145c96410 == 0) {
    FUN_141f291f0(L"/Script/Engine",L"Actor",&DAT_145c96410,FUN_14314c170,0x338,0x10000000,
                  0x1000000000,L"Engine",&LAB_14314b440,FUN_1407e4520,FUN_142b0aa80,&LAB_14083c250,
                  &LAB_14083c250,0);
  }
  return DAT_145c96410;
}



// ======== d2<-0x140d487c0  @ 0x142042c00 ========

void FUN_142042c00(longlong param_1,longlong param_2)

{
  bool bVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  
  lVar2 = 0;
  lVar4 = *(longlong *)(param_1 + 0x20);
  do {
    if (lVar4 == 0) {
      return;
    }
    if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(param_2 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) + (longlong)*(int *)(param_2 + 0x90) * 8
         ) != param_2 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    lVar3 = lVar4;
    if (!bVar1) {
      lVar3 = lVar2;
    }
    lVar2 = lVar3;
    lVar4 = *(longlong *)(lVar4 + 0x20);
  } while (lVar3 == 0);
  return;
}



// ======== d2<-0x141a4f560  @ 0x142984df0 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 * FUN_142984df0(undefined8 *param_1)

{
  longlong *plVar1;
  int *piVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  int iVar7;
  longlong *plVar8;
  longlong lVar9;
  undefined8 *puVar10;
  bool bVar11;
  undefined8 local_28 [2];
  
  FUN_142985330();
  *param_1 = &PTR_FUN_1449c4fe0;
  param_1[0x20] = &PTR_LAB_1449c5448;
  FUN_1420575e0(param_1 + 0x23,0);
  puVar10 = param_1 + 0x28;
  param_1[0x24] = 0;
  param_1[0x25] = 0x3f800000;
  *(undefined4 *)(param_1 + 0x26) = 0x3f800000;
  *(undefined4 *)((longlong)param_1 + 0x134) = 0x3f800000;
  *(undefined1 *)(param_1 + 0x27) = 0;
  *puVar10 = 0;
  param_1[0x29] = 0;
  FUN_1420575e0(param_1 + 0x2a,0);
  param_1[0x2b] = 0;
  param_1[0x2e] = 0;
  param_1[0x2c] = 0;
  param_1[0x2d] = 0;
  param_1[0x2f] = 0;
  param_1[0x30] = 0;
  param_1[0x31] = 0;
  param_1[0x32] = 0;
  param_1[0x33] = 0;
  *(undefined1 *)(param_1 + 0x37) = 1;
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0x9f | 0x18;
  param_1[0x39] = 0;
  param_1[0x3a] = 0;
  param_1[0x3b] = 0;
  param_1[0x3c] = 0;
  param_1[0x40] = 0;
  param_1[0x3d] = 0;
  param_1[0x3e] = 0;
  param_1[0x41] = 0;
  FUN_142dd46f0(param_1 + 0x42);
  uVar6 = _UNK_1447b7898;
  uVar5 = _DAT_1447b7890;
  uVar4 = _UNK_143a79a58;
  uVar3 = _DAT_143a79a50;
  *(undefined4 *)(param_1 + 0x44) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x224) = 0;
  *(undefined1 *)((longlong)param_1 + 0xb7) = 4;
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0xfd | 1;
  param_1[0x3b] = uVar3;
  param_1[0x3c] = uVar4;
  uVar3 = _DAT_1456d9c48;
  param_1[0x21] = _DAT_1456d9c40;
  param_1[0x22] = uVar3;
  param_1[0x25] = uVar5;
  param_1[0x26] = uVar6;
  *(undefined1 *)(param_1 + 0x27) = 2;
  if (local_28 != puVar10) {
    plVar8 = (longlong *)param_1[0x29];
    *puVar10 = 0;
    param_1[0x29] = 0;
    if (plVar8 != (longlong *)0x0) {
      plVar1 = plVar8 + 1;
      *(int *)plVar1 = (int)*plVar1 + -1;
      if ((int)*plVar1 == 0) {
        (**(code **)*plVar8)(plVar8);
        piVar2 = (int *)((longlong)plVar8 + 0xc);
        *piVar2 = *piVar2 + -1;
        if (*piVar2 == 0) {
          (**(code **)(*plVar8 + 8))(plVar8,1);
        }
      }
    }
  }
  bVar11 = DAT_145c6381c == '\0';
  param_1[0x36] = 0;
  if (bVar11) {
    DAT_145c6381c = '\x01';
    plVar8 = (longlong *)FUN_141df5850(0,0x20);
    if (plVar8 != (longlong *)0x0) {
      *plVar8 = (longlong)&PTR_FUN_143d84530;
      plVar8[1] = (longlong)FUN_1429bc120;
      lVar9 = FUN_141db8700();
      plVar8[3] = lVar9;
      *plVar8 = (longlong)&PTR_FUN_143d84580;
      FUN_1407e7830(&DAT_1457569f0,1);
      (**(code **)(*plVar8 + 0x28))(plVar8,local_28);
      iVar7 = DAT_1457569f8;
      lVar9 = (longlong)DAT_1457569f8;
      DAT_1457569f8 = DAT_1457569f8 + 1;
      if (DAT_1457569fc < DAT_1457569f8) {
        FUN_1407f1790(&DAT_1457569f0,iVar7);
      }
      puVar10 = (undefined8 *)(lVar9 * 0x10 + DAT_1457569f0);
      *puVar10 = plVar8;
      *(undefined4 *)(puVar10 + 1) = 2;
    }
  }
  return param_1;
}



// ======== d2<-0x141a4f560  @ 0x1420575e0 ========

void FUN_1420575e0(undefined4 *param_1,longlong param_2)

{
  undefined4 uVar1;
  
  if (param_2 != 0) {
    *param_1 = *(undefined4 *)(param_2 + 0xc);
    uVar1 = FUN_142038410(&DAT_145b9e7e0);
    param_1[1] = uVar1;
    return;
  }
  *param_1 = 0xffffffff;
  param_1[1] = 0;
  return;
}



// ======== d2<-0x141a4f560  @ 0x141a61380 ========

void FUN_141a61380(longlong param_1,undefined4 param_2)

{
  char cVar1;
  longlong lVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  ulonglong uVar5;
  
  FUN_14103f670();
  cVar1 = FUN_1415165e0(param_1,param_2);
  if ((cVar1 == '\0') && (0 < (int)*(uint *)(param_1 + 0x48))) {
    uVar4 = 0;
    uVar5 = (ulonglong)*(uint *)(param_1 + 0x48);
    do {
      uVar3 = (longlong)*(int *)(param_1 + 0x48) - 1U & uVar4;
      lVar2 = *(longlong *)(param_1 + 0x40);
      if (*(longlong *)(param_1 + 0x40) == 0) {
        lVar2 = param_1 + 0x38;
      }
      uVar4 = uVar4 + 1;
      *(undefined4 *)(lVar2 + uVar3 * 4) = 0xffffffff;
      uVar5 = uVar5 - 1;
    } while (uVar5 != 0);
  }
  return;
}



// ======== d2<-0x141de9200  @ 0x141de9840 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_141de9840(void)

{
  char cVar1;
  longlong *plVar2;
  longlong unaff_GS_OFFSET;
  undefined1 local_88 [128];
  
  if ((*(int *)(*(longlong *)(*(longlong *)(unaff_GS_OFFSET + 0x58) + (ulonglong)_tls_index * 8) +
               0x10) < DAT_145a6a3d4) && (_Init_thread_header(&DAT_145a6a3d4), DAT_145a6a3d4 == -1))
  {
    FUN_141ebd5c0(local_88);
    DAT_145a8c1f8 = (longlong *)FUN_141eb5640();
    FUN_141dccd40(DAT_145a8c1f8);
    cVar1 = (**(code **)(*DAT_145a8c1f8 + 0x70))();
    if (cVar1 == '\0') {
      plVar2 = malloc(0x38);
      if (plVar2 == (longlong *)0x0) {
        DAT_145a8c1f8 = (longlong *)0x0;
      }
      else {
        plVar2[1] = (longlong)DAT_145a8c1f8;
        *plVar2 = (longlong)&PTR_FUN_144703b10;
        InitializeCriticalSection((LPCRITICAL_SECTION)(plVar2 + 2));
        SetCriticalSectionSpinCount((LPCRITICAL_SECTION)(plVar2 + 2),4000);
        DAT_145a8c1f8 = plVar2;
      }
    }
    _DAT_145a6a3d0 = 0;
    _Init_thread_footer(&DAT_145a6a3d4);
  }
  return;
}



// ======== d2<-0x14103c060  @ 0x140bba510 ========

void FUN_140bba510(longlong *param_1,int param_2)

{
  uint *puVar1;
  int iVar2;
  bool bVar3;
  uint uVar4;
  longlong lVar5;
  int iVar6;
  uint *puVar7;
  int local_68;
  uint uStack_64;
  int iStack_54;
  int iStack_50;
  int local_40;
  uint uStack_3c;
  uint *puStack_38;
  uint local_30;
  int iStack_2c;
  int iStack_28;
  
  iVar2 = (int)param_1[5];
  puVar1 = (uint *)(param_1 + 2);
  local_68 = 0;
  uStack_64 = 1;
  iStack_54 = 0;
  iStack_50 = 0;
  if (iVar2 != 0) {
    puVar7 = (uint *)param_1[4];
    if ((uint *)param_1[4] == (uint *)0x0) {
      puVar7 = puVar1;
    }
    uVar4 = *puVar7;
    iStack_50 = 0;
    local_68 = 0;
    while (uVar4 == 0) {
      lVar5 = (longlong)local_68;
      iStack_50 = iStack_50 + 0x20;
      iVar6 = local_68 + 1;
      bVar3 = (int)(iVar2 + -1 + (iVar2 + -1 >> 0x1f & 0x1fU)) >> 5 <= local_68;
      local_68 = iVar6;
      iStack_54 = iVar2;
      if (bVar3) goto LAB_140bba5e9;
      uVar4 = puVar7[lVar5 + 1];
    }
    uStack_64 = -uVar4 & uVar4;
    iVar6 = 0x1f;
    if (uStack_64 != 0) {
      for (; uStack_64 >> iVar6 == 0; iVar6 = iVar6 + -1) {
      }
    }
    if (uStack_64 == 0) {
      iVar6 = 0x20;
    }
    else {
      iVar6 = 0x1f - iVar6;
    }
    iStack_54 = (iStack_50 - iVar6) + 0x1f;
    if (iVar2 < iStack_54) {
      iStack_54 = iVar2;
    }
  }
LAB_140bba5e9:
  local_40 = local_68;
  uStack_3c = uStack_64;
  local_30 = 0xffffffff;
  iStack_2c = iStack_54;
  iStack_28 = iStack_50;
  puStack_38 = puVar1;
  if (iStack_54 < iVar2) {
    do {
      lVar5 = (longlong)iStack_2c * 0x30 + *param_1;
      if (*(longlong *)(lVar5 + 0x18) != 0) {
        FUN_141de9200();
      }
      if (*(longlong *)(lVar5 + 8) != 0) {
        FUN_141de9200();
      }
      local_30 = local_30 & ~uStack_3c;
      FUN_1407e9ca0(&local_40);
    } while (iStack_2c < (int)puStack_38[6]);
  }
  *(undefined4 *)(param_1 + 1) = 0;
  if (*(int *)((longlong)param_1 + 0xc) != param_2) {
    FUN_140827cc0(param_1,param_2);
  }
  *(undefined4 *)((longlong)param_1 + 0x34) = 0;
  *(undefined4 *)(param_1 + 6) = 0xffffffff;
  uVar4 = param_2 + 0x1f + (param_2 + 0x1f >> 0x1f & 0x1fU) & 0xffffffe0;
  *(undefined4 *)(param_1 + 5) = 0;
  if (*(uint *)((longlong)param_1 + 0x2c) != uVar4) {
    *(uint *)((longlong)param_1 + 0x2c) = uVar4;
    FUN_1407f0ad0(puVar1,0);
  }
  return;
}



// ======== d2<-0x14103c060  @ 0x1407e59d0 ========

void FUN_1407e59d0(longlong *param_1)

{
  longlong lVar1;
  longlong *plVar2;
  
  if ((int)param_1[1] == 0) {
    plVar2 = (longlong *)0x0;
  }
  else {
    plVar2 = (longlong *)*param_1;
  }
  if ((plVar2 != (longlong *)0x0) ||
     (((int)param_1[1] != 0 && (plVar2 = (longlong *)*param_1, plVar2 != (longlong *)0x0)))) {
    (**(code **)(*plVar2 + 0x30))(plVar2,0);
    if (*param_1 != 0) {
      lVar1 = FUN_141df5850(*param_1,0,0);
      *param_1 = lVar1;
    }
    *(undefined4 *)(param_1 + 1) = 0;
  }
  if (*param_1 != 0) {
    FUN_141de9200();
    return;
  }
  return;
}



// ======== d2<-0x14103c060  @ 0x141df5850 ========

void FUN_141df5850(undefined8 param_1,undefined8 param_2,undefined4 param_3)

{
  if (DAT_145a8c1f8 == (longlong *)0x0) {
    FUN_141de9840();
  }
                    /* WARNING: Could not recover jumptable at 0x000141df589b. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*DAT_145a8c1f8 + 0x18))(DAT_145a8c1f8,param_1,param_2,param_3);
  return;
}



// ======== d2<-0x14103c060  @ 0x1407e5a40 ========

void FUN_1407e5a40(longlong *param_1)

{
  uint *puVar1;
  int iVar2;
  uint uVar3;
  bool bVar4;
  longlong lVar5;
  int iVar6;
  uint *puVar7;
  int local_58;
  uint uStack_54;
  int iStack_44;
  int iStack_40;
  int local_30;
  uint uStack_2c;
  uint *puStack_28;
  uint local_20;
  int iStack_1c;
  int iStack_18;
  
  iVar2 = (int)param_1[5];
  puVar1 = (uint *)(param_1 + 2);
  uStack_54 = 1;
  local_58 = 0;
  iStack_44 = 0;
  iStack_40 = 0;
  if (iVar2 != 0) {
    puVar7 = (uint *)param_1[4];
    if ((uint *)param_1[4] == (uint *)0x0) {
      puVar7 = puVar1;
    }
    uVar3 = *puVar7;
    iStack_40 = 0;
    local_58 = 0;
    while (uVar3 == 0) {
      lVar5 = (longlong)local_58;
      iStack_40 = iStack_40 + 0x20;
      iVar6 = local_58 + 1;
      bVar4 = (int)(iVar2 + -1 + (iVar2 + -1 >> 0x1f & 0x1fU)) >> 5 <= local_58;
      local_58 = iVar6;
      iStack_44 = iVar2;
      if (bVar4) goto LAB_1407e5b0a;
      uVar3 = puVar7[lVar5 + 1];
    }
    uStack_54 = -uVar3 & uVar3;
    iVar6 = 0x1f;
    if (uStack_54 != 0) {
      for (; uStack_54 >> iVar6 == 0; iVar6 = iVar6 + -1) {
      }
    }
    if (uStack_54 == 0) {
      iVar6 = 0x20;
    }
    else {
      iVar6 = 0x1f - iVar6;
    }
    iStack_44 = (iStack_40 - iVar6) + 0x1f;
    if (iVar2 < iStack_44) {
      iStack_44 = iVar2;
    }
  }
LAB_1407e5b0a:
  local_30 = local_58;
  uStack_2c = uStack_54;
  local_20 = 0xffffffff;
  iStack_1c = iStack_44;
  iStack_18 = iStack_40;
  puStack_28 = puVar1;
  if (iStack_44 < iVar2) {
    do {
      if (*(longlong *)((longlong)iStack_1c * 0x20 + *param_1 + 8) != 0) {
        FUN_141de9200();
      }
      local_20 = local_20 & ~uStack_2c;
      FUN_1407e9ca0(&local_30);
    } while (iStack_1c < (int)puStack_28[6]);
  }
  *(undefined4 *)(param_1 + 1) = 0;
  if (*(int *)((longlong)param_1 + 0xc) != 0) {
    FUN_1407f1920(param_1,0);
  }
  *(undefined4 *)(param_1 + 6) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x34) = 0;
  *(undefined4 *)(param_1 + 5) = 0;
  if (*(int *)((longlong)param_1 + 0x2c) != 0) {
    *(undefined4 *)((longlong)param_1 + 0x2c) = 0;
    FUN_1407f0ad0(puVar1,0);
  }
  if (param_1[4] != 0) {
    FUN_141de9200();
  }
  if (*param_1 != 0) {
    FUN_141de9200();
  }
  return;
}



// ======== d2<-0x14103c060  @ 0x14103f670 ========

void FUN_14103f670(longlong *param_1,int param_2)

{
  uint *puVar1;
  longlong *plVar2;
  int *piVar3;
  int iVar4;
  longlong *plVar5;
  bool bVar6;
  uint uVar7;
  longlong lVar8;
  int iVar9;
  uint *puVar10;
  int local_68;
  uint uStack_64;
  int iStack_54;
  int iStack_50;
  int local_40;
  uint uStack_3c;
  uint *puStack_38;
  uint local_30;
  int iStack_2c;
  int iStack_28;
  
  iVar4 = (int)param_1[5];
  puVar1 = (uint *)(param_1 + 2);
  local_68 = 0;
  uStack_64 = 1;
  iStack_54 = 0;
  iStack_50 = 0;
  if (iVar4 != 0) {
    puVar10 = (uint *)param_1[4];
    if ((uint *)param_1[4] == (uint *)0x0) {
      puVar10 = puVar1;
    }
    uVar7 = *puVar10;
    iStack_50 = 0;
    local_68 = 0;
    while (uVar7 == 0) {
      lVar8 = (longlong)local_68;
      iStack_50 = iStack_50 + 0x20;
      iVar9 = local_68 + 1;
      bVar6 = (int)(iVar4 + -1 + (iVar4 + -1 >> 0x1f & 0x1fU)) >> 5 <= local_68;
      local_68 = iVar9;
      iStack_54 = iVar4;
      if (bVar6) goto LAB_14103f749;
      uVar7 = puVar10[lVar8 + 1];
    }
    uStack_64 = -uVar7 & uVar7;
    iVar9 = 0x1f;
    if (uStack_64 != 0) {
      for (; uStack_64 >> iVar9 == 0; iVar9 = iVar9 + -1) {
      }
    }
    if (uStack_64 == 0) {
      iVar9 = 0x20;
    }
    else {
      iVar9 = 0x1f - iVar9;
    }
    iStack_54 = (iStack_50 - iVar9) + 0x1f;
    if (iVar4 < iStack_54) {
      iStack_54 = iVar4;
    }
  }
LAB_14103f749:
  local_40 = local_68;
  uStack_3c = uStack_64;
  local_30 = 0xffffffff;
  iStack_2c = iStack_54;
  iStack_28 = iStack_50;
  puStack_38 = puVar1;
  if (iStack_54 < iVar4) {
    do {
      lVar8 = (longlong)iStack_2c * 0x70 + *param_1;
      plVar5 = *(longlong **)(lVar8 + 0x60);
      if (plVar5 != (longlong *)0x0) {
        plVar2 = plVar5 + 1;
        *(int *)plVar2 = (int)*plVar2 + -1;
        if ((int)*plVar2 == 0) {
          (**(code **)*plVar5)(plVar5);
          piVar3 = (int *)((longlong)plVar5 + 0xc);
          *piVar3 = *piVar3 + -1;
          if (*piVar3 == 0) {
            (**(code **)(*plVar5 + 8))(plVar5,1);
          }
        }
      }
      if (*(longlong *)(lVar8 + 0x40) != 0) {
        FUN_141de9200();
      }
      if (*(longlong *)(lVar8 + 0x30) != 0) {
        FUN_141de9200();
      }
      local_30 = local_30 & ~uStack_3c;
      FUN_1407e9ca0(&local_40);
    } while (iStack_2c < (int)puStack_38[6]);
  }
  *(undefined4 *)(param_1 + 1) = 0;
  if (*(int *)((longlong)param_1 + 0xc) != param_2) {
    FUN_140827eb0(param_1,param_2);
  }
  *(undefined4 *)((longlong)param_1 + 0x34) = 0;
  *(undefined4 *)(param_1 + 6) = 0xffffffff;
  uVar7 = param_2 + 0x1f + (param_2 + 0x1f >> 0x1f & 0x1fU) & 0xffffffe0;
  *(undefined4 *)(param_1 + 5) = 0;
  if (*(uint *)((longlong)param_1 + 0x2c) != uVar7) {
    *(uint *)((longlong)param_1 + 0x2c) = uVar7;
    FUN_1407f0ad0(puVar1,0);
  }
  return;
}



// ======== d2<-0x1407fa390  @ 0x1407f18a0 ========

void FUN_1407f18a0(longlong *param_1,int param_2)

{
  ulonglong uVar1;
  longlong lVar2;
  int iVar3;
  
  iVar3 = 0;
  if (param_2 != 0) {
    uVar1 = FUN_141df5250((longlong)param_2 * 0x18,0);
    iVar3 = (int)(uVar1 / 0x18);
    if (iVar3 < param_2) {
      iVar3 = 0x7fffffff;
    }
  }
  if (iVar3 != *(int *)((longlong)param_1 + 0xc)) {
    *(int *)((longlong)param_1 + 0xc) = iVar3;
    if ((*param_1 != 0) || (iVar3 != 0)) {
      lVar2 = FUN_141df5850(*param_1,(longlong)iVar3 * 0x18,0);
      *param_1 = lVar2;
    }
  }
  return;
}



// ======== d2<-0x1407fa390  @ 0x1407f0ad0 ========

void FUN_1407f0ad0(void *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  void *_Dst;
  undefined8 uVar4;
  void *pvVar5;
  longlong lVar6;
  longlong lVar7;
  
  _Dst = (void *)0x0;
  iVar1 = (int)(param_2 + 0x1f + (param_2 + 0x1f >> 0x1f & 0x1fU)) >> 5;
  lVar7 = (longlong)iVar1;
  pvVar5 = *(void **)((longlong)param_1 + 0x10);
  iVar2 = *(int *)((longlong)param_1 + 0x1c) + 0x1f;
  uVar3 = (int)(iVar2 + (iVar2 >> 0x1f & 0x1fU)) >> 5;
  if (uVar3 < 5) {
    if (pvVar5 == (void *)0x0) goto LAB_1407f0b7d;
    memmove(param_1,pvVar5,lVar7 << 2);
    pvVar5 = *(void **)((longlong)param_1 + 0x10);
    if (pvVar5 == (void *)0x0) goto LAB_1407f0b7d;
    lVar6 = 0;
  }
  else {
    if (pvVar5 == (void *)0x0) {
      if (uVar3 != 0) {
        _Dst = (void *)FUN_141df5850(0,(longlong)(int)uVar3 << 2,0);
        *(void **)((longlong)param_1 + 0x10) = _Dst;
      }
      memmove(_Dst,param_1,lVar7 << 2);
      goto LAB_1407f0b7d;
    }
    lVar6 = (longlong)(int)uVar3 << 2;
  }
  uVar4 = FUN_141df5850(pvVar5,lVar6,0);
  *(undefined8 *)((longlong)param_1 + 0x10) = uVar4;
LAB_1407f0b7d:
  if (uVar3 != 0) {
    pvVar5 = *(void **)((longlong)param_1 + 0x10);
    if (*(void **)((longlong)param_1 + 0x10) == (void *)0x0) {
      pvVar5 = param_1;
    }
    memset((void *)((longlong)pvVar5 + lVar7 * 4),0,(longlong)(int)(uVar3 - iVar1) << 2);
  }
  return;
}



// ======== d2<-0x14203ed30  @ 0x141f25a00 ========

undefined8 * FUN_141f25a00(longlong param_1,longlong param_2,int param_3)

{
  PSRWLOCK SRWLock;
  int iVar1;
  longlong lVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  longlong *plVar5;
  longlong *plVar6;
  longlong lVar7;
  undefined8 *puVar8;
  undefined1 local_res8 [8];
  longlong local_res10;
  undefined8 *local_res20;
  longlong *local_38;
  undefined8 **local_30;
  
  puVar8 = (undefined8 *)0x0;
  local_res20 = puVar8;
  if (*(int *)(param_1 + 0x110) != *(int *)(param_1 + 0x13c)) {
    lVar2 = *(longlong *)(param_1 + 0x148);
    if (*(longlong *)(param_1 + 0x148) == 0) {
      lVar2 = param_1 + 0x140;
    }
    iVar1 = *(int *)(lVar2 + ((longlong)((int)((ulonglong)param_2 >> 0x20) + (int)param_2) &
                             (longlong)*(int *)(param_1 + 0x150) - 1U) * 4);
    if (iVar1 != -1) {
      lVar2 = *(longlong *)(param_1 + 0x108);
      do {
        lVar7 = (longlong)iVar1;
        if (*(longlong *)(lVar2 + lVar7 * 0x18) == param_2) {
          if ((iVar1 != -1) && (lVar2 = lVar2 + lVar7 * 0x18, lVar2 != 0)) {
            local_res20 = *(undefined8 **)(lVar2 + 8);
          }
          break;
        }
        iVar1 = *(int *)(lVar2 + 0x10 + lVar7 * 0x18);
      } while (iVar1 != -1);
    }
  }
  if (((local_res20 == (undefined8 *)0x0) && (param_3 == 1)) &&
     ((lVar2 = *(longlong *)(param_1 + 0x30), lVar2 != 0 || (0 < *(int *)(param_1 + 0x1b8))))) {
    SRWLock = (PSRWLOCK)(param_1 + 0x1a8);
    local_res10 = param_2;
    AcquireSRWLockShared(SRWLock);
    if (*(int *)(param_1 + 0x160) != *(int *)(param_1 + 0x18c)) {
      lVar7 = *(longlong *)(param_1 + 0x198);
      if (*(longlong *)(param_1 + 0x198) == 0) {
        lVar7 = param_1 + 400;
      }
      iVar1 = *(int *)(lVar7 + ((longlong)((int)((ulonglong)local_res10 >> 0x20) + (int)local_res10)
                               & (longlong)*(int *)(param_1 + 0x1a0) - 1U) * 4);
      if (iVar1 != -1) {
        lVar7 = *(longlong *)(param_1 + 0x158);
        do {
          puVar4 = (undefined8 *)(lVar7 + (longlong)iVar1 * 0x18);
          if (*(longlong *)(lVar7 + (longlong)iVar1 * 0x18) == local_res10) {
            if (iVar1 != -1) goto LAB_141f25b66;
            break;
          }
          iVar1 = *(int *)(puVar4 + 2);
        } while (iVar1 != -1);
      }
    }
    puVar4 = puVar8;
LAB_141f25b66:
    puVar3 = puVar4 + 1;
    if (puVar4 == (undefined8 *)0x0) {
      puVar3 = puVar8;
    }
    if (puVar3 != (undefined8 *)0x0) {
      local_res20 = (undefined8 *)*puVar3;
    }
    ReleaseSRWLockShared(SRWLock);
    if (puVar3 == (undefined8 *)0x0) {
      plVar5 = *(longlong **)(param_1 + 0x1b0);
      plVar6 = plVar5 + (longlong)*(int *)(param_1 + 0x1b8) * 2;
      puVar4 = local_res20;
      for (; plVar5 != plVar6; plVar5 = plVar5 + 2) {
        puVar4 = puVar8;
        if (*plVar5 != 0) {
          puVar4 = (undefined8 *)FUN_141f25a00(*plVar5,local_res10,1);
        }
        if (puVar4 != (undefined8 *)0x0) break;
        local_res20 = (undefined8 *)0x0;
      }
      local_res20 = puVar4;
      if ((lVar2 != 0) && (local_res20 == (undefined8 *)0x0)) {
        local_res20 = (undefined8 *)FUN_141f25a00(lVar2,local_res10,1);
      }
      AcquireSRWLockExclusive(SRWLock);
      local_38 = &local_res10;
      local_30 = &local_res20;
      FUN_141f1b8b0((longlong *)(param_1 + 0x158),local_res8,&local_38,0);
      ReleaseSRWLockExclusive(SRWLock);
    }
  }
  return local_res20;
}



// ======== d2<-0x14203ed30  @ 0x142041d60 ========

longlong * FUN_142041d60(longlong param_1,longlong *param_2,undefined8 param_3)

{
  undefined2 *puVar1;
  int iVar2;
  wchar_t *pwVar3;
  int iVar4;
  undefined8 local_res8;
  
  *param_2 = 0;
  param_2[1] = 0;
  if (param_1 != 0) {
    *(undefined4 *)(param_2 + 1) = 0;
    FUN_1407f1af0(param_2,0x80);
    local_res8 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x18);
    FUN_141eb52d0(&local_res8,param_2);
    iVar2 = (int)param_2[1];
    iVar4 = (2 - (uint)(iVar2 != 0)) + iVar2;
    *(int *)(param_2 + 1) = iVar4;
    if (*(int *)((longlong)param_2 + 0xc) < iVar4) {
      FUN_1407e2ee0(param_2,iVar2);
    }
    puVar1 = (undefined2 *)(*param_2 + ((longlong)iVar2 - (ulonglong)(iVar2 != 0)) * 2);
    *puVar1 = DAT_144784094;
    puVar1[1] = 0;
    FUN_142042130(param_1,param_3,param_2);
    return param_2;
  }
  *(undefined4 *)(param_2 + 1) = 5;
  FUN_1407e2ee0(param_2,0);
  pwVar3 = (wchar_t *)*param_2;
  *(undefined8 *)pwVar3 = u_None_144786bb8._0_8_;
  pwVar3[4] = L'\0';
  return param_2;
}



// ======== d2<-0x14203ed30  @ 0x141eca8b0 ========

undefined8 * FUN_141eca8b0(int *param_1,undefined8 *param_2)

{
  uint uVar1;
  longlong lVar2;
  uint uVar3;
  undefined8 local_18;
  undefined8 local_10;
  
  if (param_1[1] == 0) {
    lVar2 = FUN_141ebaac0();
    uVar3 = *param_1 >> 0x1f & 0x3fff;
    uVar1 = *param_1 + uVar3;
    lVar2 = *(longlong *)
             (*(longlong *)(lVar2 + (longlong)((int)uVar1 >> 0xe) * 8) +
             (longlong)(int)((uVar1 & 0x3fff) - uVar3) * 8);
    if (lVar2 != 0) {
      FUN_141ebc470(lVar2,param_2);
      return param_2;
    }
  }
  local_18 = 0;
  local_10 = 0;
  FUN_141eca950(param_1,&local_18);
  *param_2 = local_18;
  *(undefined4 *)(param_2 + 1) = (undefined4)local_10;
  *(undefined4 *)((longlong)param_2 + 0xc) = local_10._4_4_;
  return param_2;
}



// ======== d2<-0x14203ed30  @ 0x141e49950 ========

/* WARNING: Function: _alloca_probe replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141e49950(undefined8 param_1,undefined4 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 *puVar1;
  undefined8 local_res20;
  undefined1 auStack_2068 [32];
  ulonglong local_2048;
  undefined8 *local_2040;
  undefined1 local_2038 [8192];
  ulonglong local_38;
  undefined8 uStack_28;
  
  uStack_28 = 0x141e49968;
  local_38 = DAT_145909780 ^ (ulonglong)auStack_2068;
  local_res20 = param_4;
  puVar1 = (undefined8 *)FUN_14078a190();
  local_2048 = 0;
  local_2040 = &local_res20;
  __stdio_common_vswprintf(*puVar1,local_2038,0xfff,param_3);
  local_2040 = (undefined8 *)CONCAT44(local_2040._4_4_,1);
  local_2048 = local_2048 & 0xffffffffffffff00;
  FUN_141e51550(L"LowLevelFatalError",param_1,param_2,local_2038);
  return;
}



// ======== d2<-0x14203ed30  @ 0x141e40cb0 ========

/* WARNING: Function: _alloca_probe replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141e40cb0(undefined8 param_1,undefined8 param_2,undefined4 param_3,undefined8 param_4)

{
  undefined8 *puVar1;
  undefined1 auStack_2988 [32];
  undefined8 local_2968;
  undefined1 *local_2960;
  undefined1 local_2958 [256];
  longlong local_2858;
  undefined8 local_2850;
  undefined1 local_2838 [2048];
  undefined1 local_2038 [8192];
  ulonglong local_38;
  
  local_38 = DAT_145909780 ^ (ulonglong)auStack_2988;
  if (DAT_145a9823b == '\0') {
    DAT_145a6abe0 = 1;
    puVar1 = (undefined8 *)FUN_14078a190();
    local_2968 = 0;
    local_2960 = &stack0x00000028;
    __stdio_common_vswprintf(*puVar1,local_2038,0xfff,param_4);
    FUN_1407f7a90(local_2958,param_1);
    FUN_141e50f30(local_2838,&DAT_14470cec0,local_2850);
    if (local_2858 != 0) {
      FUN_141de9200();
    }
    FUN_1407f7a90(local_2958,param_2);
    local_2960 = local_2038;
    local_2968 = CONCAT44(local_2968._4_4_,param_3);
    FUN_141e96340(DAT_145a98c58,L"Assertion failed: %s [File:%s] [Line: %i] \n%s\n",local_2838,
                  local_2850);
    if (local_2858 != 0) {
      FUN_141de9200();
    }
  }
  return;
}



// ======== d2<-0x1407f1420  @ 0x141df5250 ========

undefined8 FUN_141df5250(undefined8 param_1,undefined4 param_2)

{
  undefined8 uVar1;
  
  if (DAT_145a8c1f8 == (longlong *)0x0) {
    return param_1;
  }
                    /* WARNING: Could not recover jumptable at 0x000141df526c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar1 = (**(code **)(*DAT_145a8c1f8 + 0x28))(DAT_145a8c1f8,param_1,param_2);
  return uVar1;
}



// ======== d2<-0x141752b00  @ 0x14203ad30 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14203ad30(longlong *param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  undefined1 auStack_1a8 [32];
  undefined1 local_188;
  undefined4 local_180;
  undefined4 local_178;
  undefined4 local_168;
  undefined4 uStack_164;
  undefined8 uStack_160;
  undefined1 local_158 [16];
  undefined1 local_148 [256];
  longlong local_48;
  ulonglong local_28;
  
  local_28 = DAT_145909780 ^ (ulonglong)auStack_1a8;
  if (*param_1 == 0) {
    lVar2 = FUN_1407f7cf0(local_148,*param_2);
    uVar1 = *(undefined8 *)(lVar2 + 0x108);
    uVar3 = FUN_141ffb180();
    puVar4 = (undefined8 *)FUN_141eb3490(local_158,uVar1,1);
    uVar5 = 0;
    local_178 = 0;
    local_180 = 0;
    local_188 = 0;
    lVar2 = FUN_14204d430(uVar3,0,*puVar4,0);
    if (local_48 != 0) {
      FUN_141de9200();
    }
    *param_1 = lVar2;
    *(uint *)(lVar2 + 0x50) = *(uint *)(lVar2 + 0x50) | *(uint *)(param_2 + 1);
    uStack_164 = *(undefined4 *)(param_2 + 2);
    local_168 = *(undefined4 *)((longlong)param_2 + 0xc);
    uStack_160 = 0;
    *(undefined4 *)(lVar2 + 0x30) = local_168;
    *(undefined4 *)(lVar2 + 0x34) = uStack_164;
    *(undefined4 *)(lVar2 + 0x38) = 0;
    *(undefined4 *)(lVar2 + 0x3c) = 0;
    puVar4 = (undefined8 *)param_2[3];
    uVar6 = (longlong)*(int *)(param_2 + 4) & 0x1fffffffffffffff;
    if (puVar4 + *(int *)(param_2 + 4) < puVar4) {
      uVar6 = uVar5;
    }
    if (uVar6 != 0) {
      do {
        (*(code *)*puVar4)();
        uVar5 = uVar5 + 1;
        puVar4 = puVar4 + 1;
      } while (uVar5 != uVar6);
    }
  }
  return;
}



// ======== d2<-0x14203a700  @ 0x141fd65d0 ========

longlong FUN_141fd65d0(void)

{
  if (DAT_145b9ccd8 == 0) {
    FUN_141f291f0(L"/Script/CoreUObject",L"Enum",&DAT_145b9ccd8,_guard_check_icall,0x60,0x10000000,4
                  ,L"Engine",FUN_141fd7620,FUN_141fd76a0,_guard_check_icall,FUN_141f28500,
                  &LAB_14083c250,0);
  }
  return DAT_145b9ccd8;
}



// ======== d2<-0x14203a700  @ 0x141fd8850 ========

undefined8 * FUN_141fd8850(undefined8 *param_1)

{
  FUN_142035bd0();
  *param_1 = &PTR_FUN_14476c108;
  param_1[6] = 0;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  *(undefined4 *)(param_1 + 10) = 0;
  param_1[0xb] = 0;
  return param_1;
}



// ======== d2<-0x14203a700  @ 0x1407f3d80 ========

void FUN_1407f3d80(int *param_1,int param_2,byte *param_3,int param_4)

{
  byte bVar1;
  byte *pbVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  byte *pbVar6;
  
  pbVar2 = param_3;
  do {
    if ((param_3 + param_4 <= pbVar2) || (param_2 < 1)) {
      return;
    }
    bVar1 = *pbVar2;
    uVar5 = (uint)bVar1;
    uVar3 = (int)(param_3 + param_4) - (int)pbVar2;
    if (bVar1 < 0x80) {
      pbVar6 = pbVar2 + 1;
LAB_1407f3f56:
      if ((0xfffff < uVar5 - 0x10000) || (param_2 < 2)) goto LAB_1407f403c;
      *param_1 = *param_1 + 2;
      iVar4 = -2;
    }
    else {
      if (bVar1 < 0xc0) {
        pbVar6 = pbVar2 + 1;
      }
      else {
        uVar5 = (uint)bVar1;
        if (bVar1 < 0xe0) {
          if (uVar3 < 2) {
            pbVar6 = pbVar2 + uVar3;
          }
          else {
            if (((pbVar2[1] & 0xc0) == 0x80) &&
               (uVar5 = (uVar5 - 0xc0) * 0x40 | pbVar2[1] - 0x80, uVar5 - 0x80 < 0x780)) {
              pbVar6 = pbVar2 + 2;
              goto LAB_1407f3f56;
            }
LAB_1407f3dfc:
            pbVar6 = pbVar2 + 1;
          }
        }
        else if (bVar1 < 0xf0) {
          if (uVar3 < 3) {
            pbVar6 = pbVar2 + uVar3;
          }
          else {
            pbVar6 = pbVar2 + 1;
            if (((pbVar2[1] & 0xc0) == 0x80) && ((pbVar2[2] & 0xc0) == 0x80)) {
              uVar5 = ((uVar5 - 0xe0) * 0x40 | pbVar2[1] - 0x80) << 6 | pbVar2[2] - 0x80;
              if ((uVar5 - 0xd800 < 0x400) || ((uVar5 - 0xdc00 < 0x400 || (0xf7fd < uVar5 - 0x800)))
                 ) goto LAB_1407f3dfc;
              pbVar6 = pbVar2 + 3;
              goto LAB_1407f3f56;
            }
          }
        }
        else if (bVar1 < 0xf8) {
          if (uVar3 < 4) {
            pbVar6 = pbVar2 + uVar3;
          }
          else {
            pbVar6 = pbVar2 + 1;
            if ((((pbVar2[1] & 0xc0) == 0x80) && ((pbVar2[2] & 0xc0) == 0x80)) &&
               ((pbVar2[3] & 0xc0) == 0x80)) {
              uVar5 = (((uVar5 - 0xf0) * 0x40 | pbVar2[1] - 0x80) << 6 | pbVar2[2] - 0x80) << 6 |
                      pbVar2[3] - 0x80;
              if (0xfffff < uVar5 - 0x10000) goto LAB_1407f3dfc;
              pbVar6 = pbVar2 + 4;
              goto LAB_1407f3f56;
            }
          }
        }
        else if (bVar1 < 0xfc) {
          if (uVar3 < 5) {
            pbVar6 = pbVar2 + uVar3;
          }
          else if ((pbVar2[1] & 0xc0) == 0x80) {
            if ((pbVar2[2] & 0xc0) == 0x80) {
              if ((pbVar2[3] & 0xc0) == 0x80) {
                if ((pbVar2[4] & 0xc0) == 0x80) {
                  pbVar6 = pbVar2 + 5;
                }
                else {
                  pbVar6 = pbVar2 + 1;
                }
              }
              else {
                pbVar6 = pbVar2 + 1;
              }
            }
            else {
              pbVar6 = pbVar2 + 1;
            }
          }
          else {
            pbVar6 = pbVar2 + 1;
          }
        }
        else if (uVar3 < 6) {
          pbVar6 = pbVar2 + uVar3;
        }
        else if ((pbVar2[1] & 0xc0) == 0x80) {
          if ((pbVar2[2] & 0xc0) == 0x80) {
            if ((pbVar2[3] & 0xc0) == 0x80) {
              if ((pbVar2[4] & 0xc0) == 0x80) {
                if ((pbVar2[5] & 0xc0) == 0x80) {
                  pbVar6 = pbVar2 + 6;
                }
                else {
                  pbVar6 = pbVar2 + 1;
                }
              }
              else {
                pbVar6 = pbVar2 + 1;
              }
            }
            else {
              pbVar6 = pbVar2 + 1;
            }
          }
          else {
            pbVar6 = pbVar2 + 1;
          }
        }
        else {
          pbVar6 = pbVar2 + 1;
        }
      }
LAB_1407f403c:
      *param_1 = *param_1 + 1;
      iVar4 = -1;
    }
    param_2 = param_2 + iVar4;
    pbVar2 = pbVar6;
  } while( true );
}



// ======== d2<-0x14203a700  @ 0x1438246bc ========

void * __cdecl memmove(void *_Dst,void *_Src,size_t _Size)

{
  void *pvVar1;
  
                    /* WARNING: Could not recover jumptable at 0x0001438246bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pvVar1 = memmove(_Dst,_Src,_Size);
  return pvVar1;
}



// ======== d2<-0x14203a700  @ 0x1407f37f0 ========

void FUN_1407f37f0(longlong *param_1,int param_2,byte *param_3,int param_4)

{
  byte bVar1;
  byte *pbVar2;
  undefined2 uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  byte *pbVar8;
  
  pbVar2 = param_3;
  do {
    if ((param_3 + param_4 <= pbVar2) || (param_2 < 1)) {
      return;
    }
    bVar1 = *pbVar2;
    uVar7 = (int)(param_3 + param_4) - (int)pbVar2;
    uVar5 = 0x3f;
    if (bVar1 < 0x80) {
      pbVar8 = pbVar2 + 1;
      uVar5 = (uint)bVar1;
    }
    else if (bVar1 < 0xc0) {
      pbVar8 = pbVar2 + 1;
    }
    else {
      uVar4 = (uint)bVar1;
      if (bVar1 < 0xe0) {
        if (uVar7 < 2) {
          pbVar8 = pbVar2 + uVar7;
        }
        else if (((pbVar2[1] & 0xc0) == 0x80) &&
                (uVar7 = (uVar4 - 0xc0) * 0x40 | pbVar2[1] - 0x80, uVar7 - 0x80 < 0x780)) {
          pbVar8 = pbVar2 + 2;
          uVar5 = uVar7;
        }
        else {
LAB_1407f3899:
          pbVar8 = pbVar2 + 1;
        }
      }
      else if (bVar1 < 0xf0) {
        if (uVar7 < 3) {
          pbVar8 = pbVar2 + uVar7;
        }
        else {
          pbVar8 = pbVar2 + 1;
          if (((pbVar2[1] & 0xc0) == 0x80) && ((pbVar2[2] & 0xc0) == 0x80)) {
            uVar7 = ((uVar4 - 0xe0) * 0x40 | pbVar2[1] - 0x80) << 6 | pbVar2[2] - 0x80;
            if ((uVar7 - 0xd800 < 0x400) || ((uVar7 - 0xdc00 < 0x400 || (0xf7fd < uVar7 - 0x800))))
            goto LAB_1407f3899;
            pbVar8 = pbVar2 + 3;
            uVar5 = uVar7;
          }
        }
      }
      else if (bVar1 < 0xf8) {
        if (uVar7 < 4) {
          pbVar8 = pbVar2 + uVar7;
        }
        else {
          pbVar8 = pbVar2 + 1;
          if ((((pbVar2[1] & 0xc0) == 0x80) && ((pbVar2[2] & 0xc0) == 0x80)) &&
             ((pbVar2[3] & 0xc0) == 0x80)) {
            uVar7 = (((uVar4 - 0xf0) * 0x40 | pbVar2[1] - 0x80) << 6 | pbVar2[2] - 0x80) << 6 |
                    pbVar2[3] - 0x80;
            if (0xfffff < uVar7 - 0x10000) goto LAB_1407f3899;
            pbVar8 = pbVar2 + 4;
            uVar5 = uVar7;
          }
        }
      }
      else if (bVar1 < 0xfc) {
        if (uVar7 < 5) {
          pbVar8 = pbVar2 + uVar7;
        }
        else if ((pbVar2[1] & 0xc0) == 0x80) {
          if ((pbVar2[2] & 0xc0) == 0x80) {
            if ((pbVar2[3] & 0xc0) == 0x80) {
              uVar5 = 0x3f;
              if ((pbVar2[4] & 0xc0) == 0x80) {
                pbVar8 = pbVar2 + 5;
              }
              else {
                pbVar8 = pbVar2 + 1;
              }
            }
            else {
              pbVar8 = pbVar2 + 1;
            }
          }
          else {
            pbVar8 = pbVar2 + 1;
          }
        }
        else {
          pbVar8 = pbVar2 + 1;
        }
      }
      else if (uVar7 < 6) {
        pbVar8 = pbVar2 + uVar7;
      }
      else if ((pbVar2[1] & 0xc0) == 0x80) {
        if ((pbVar2[2] & 0xc0) == 0x80) {
          if ((pbVar2[3] & 0xc0) == 0x80) {
            if ((pbVar2[4] & 0xc0) == 0x80) {
              if ((pbVar2[5] & 0xc0) == 0x80) {
                pbVar8 = pbVar2 + 6;
              }
              else {
                pbVar8 = pbVar2 + 1;
              }
            }
            else {
              pbVar8 = pbVar2 + 1;
            }
          }
          else {
            pbVar8 = pbVar2 + 1;
          }
        }
        else {
          pbVar8 = pbVar2 + 1;
        }
      }
      else {
        pbVar8 = pbVar2 + 1;
      }
    }
    uVar7 = uVar5 - 0x10000;
    if (uVar7 < 0x100000) {
      if (param_2 < 2) {
        uVar3 = 0x3f;
        goto LAB_1407f3b11;
      }
      *(short *)*param_1 = (short)(uVar7 >> 10) + -0x2800;
      *param_1 = *param_1 + 2;
      *(ushort *)*param_1 = ((ushort)uVar7 & 0x3ff) + 0xdc00;
      iVar6 = -2;
    }
    else {
      uVar3 = (undefined2)uVar5;
      if (0x10ffff < uVar5) {
        uVar3 = 0x3f;
      }
LAB_1407f3b11:
      *(undefined2 *)*param_1 = uVar3;
      iVar6 = -1;
    }
    *param_1 = *param_1 + 2;
    param_2 = param_2 + iVar6;
    pbVar2 = pbVar8;
  } while( true );
}



// ======== d2<-0x14203a700  @ 0x1407f1af0 ========

void FUN_1407f1af0(longlong *param_1,int param_2)

{
  int iVar1;
  ulonglong uVar2;
  longlong lVar3;
  
  iVar1 = 0;
  if (param_2 != 0) {
    uVar2 = FUN_141df5250((longlong)param_2 * 2,0);
    iVar1 = (int)(uVar2 >> 1);
    if (iVar1 < param_2) {
      iVar1 = 0x7fffffff;
    }
  }
  if (iVar1 != *(int *)((longlong)param_1 + 0xc)) {
    *(int *)((longlong)param_1 + 0xc) = iVar1;
    if ((*param_1 != 0) || (iVar1 != 0)) {
      lVar3 = FUN_141df5850(*param_1,(longlong)iVar1 * 2,0);
      *param_1 = lVar3;
    }
  }
  return;
}



// ======== d2<-0x14203a700  @ 0x1407e2ee0 ========

void FUN_1407e2ee0(longlong *param_1)

{
  uint uVar1;
  int iVar2;
  ulonglong uVar3;
  longlong lVar4;
  
  uVar1 = *(uint *)(param_1 + 1);
  lVar4 = 4;
  if ((*(int *)((longlong)param_1 + 0xc) != 0) || (4 < uVar1)) {
    lVar4 = (longlong)(int)uVar1 + 0x10 + ((ulonglong)((longlong)(int)uVar1 * 3) >> 3);
  }
  uVar3 = FUN_141df5250(lVar4 * 2,0);
  iVar2 = (int)(uVar3 >> 1);
  if (iVar2 < (int)uVar1) {
    iVar2 = 0x7fffffff;
  }
  *(int *)((longlong)param_1 + 0xc) = iVar2;
  if ((*param_1 != 0) || (iVar2 != 0)) {
    lVar4 = FUN_141df5850(*param_1,(longlong)iVar2 * 2,0);
    *param_1 = lVar4;
  }
  return;
}



// ======== d2<-0x14203c8c0  @ 0x141f28cb0 ========

longlong * FUN_141f28cb0(void)

{
  longlong *plVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  longlong lVar4;
  longlong *plVar5;
  undefined1 local_res8 [8];
  undefined8 in_stack_ffffffffffffffa8;
  undefined4 uVar6;
  ulonglong uVar7;
  undefined8 in_stack_ffffffffffffffc0;
  undefined4 uVar8;
  
  uVar6 = (undefined4)((ulonglong)in_stack_ffffffffffffffa8 >> 0x20);
  uVar8 = (undefined4)((ulonglong)in_stack_ffffffffffffffc0 >> 0x20);
  if (DAT_145b9c740 == (longlong *)0x0) {
    uVar2 = FUN_1420384d0(&DAT_145b9e540,0x208,8,1);
    DAT_145b9c740 = (longlong *)uVar2;
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,L"ScriptStruct",1);
    uVar7 = 0;
    DAT_145b9c740 =
         (longlong *)
         FUN_141f1ece0(uVar2,0,*puVar3,0x98,CONCAT44(uVar6,0x10000020),0x10,L"Engine",
                       CONCAT44(uVar8,199),FUN_141f1c770,FUN_141f1cc40,_guard_check_icall);
    lVar4 = FUN_14205cf60();
    plVar5 = (longlong *)FUN_141f28f10();
    plVar1 = DAT_145b9c740;
    uVar7 = uVar7 & 0xffffffffffffff00;
    uVar6 = 0;
    FUN_141f07780(L"/Script/CoreUObject",L"ScriptStruct",0,1,0,uVar7);
    if (plVar5 == plVar1) {
      plVar5 = (longlong *)0x0;
    }
    (**(code **)(*plVar1 + 0x2a0))(plVar1,plVar5);
    plVar1[0x18] = lVar4;
    (**(code **)(*plVar1 + 8))(plVar1);
    if ((*(uint *)(plVar1 + 1) >> 0x1a & 1) == 0) {
      FUN_142049670(plVar1,L"/Script/CoreUObject",L"ScriptStruct");
    }
    else {
      if (DAT_145b9c8a0 == 0) {
        uVar7 = CONCAT44((int)(uVar7 >> 0x20),0x10000000);
        FUN_141f291f0(L"/Script/CoreUObject",L"DynamicClass",&DAT_145b9c8a0,_guard_check_icall,
                      CONCAT44(uVar6,0x270),uVar7,0,L"Engine",FUN_141f1c610,FUN_141f1c930,
                      FUN_141f21430,FUN_141f28040,&LAB_141fd55f0,0);
      }
      (**(code **)(*plVar1 + 0x10))(plVar1,DAT_145b9c8a0,L"/Script/CoreUObject",L"ScriptStruct");
    }
    FUN_141f07780(L"/Script/CoreUObject",L"ScriptStruct",0,2,0,uVar7 & 0xffffffffffffff00);
  }
  return DAT_145b9c740;
}



// ======== d2<-0x14203c8c0  @ 0x141f1efe0 ========

undefined8 *
FUN_141f1efe0(undefined8 *param_1,undefined8 param_2,undefined8 param_3,longlong param_4,
             uint param_5)

{
  FUN_141f1f060();
  *(undefined1 *)((longlong)param_1 + 0x8c) = 0;
  *param_1 = &PTR_FUN_144760d20;
  param_1[0x12] = param_4;
  *(uint *)(param_1 + 0x11) = param_4 != 0 | param_5;
  FUN_141f2bfc0(param_1);
  return param_1;
}



