// ======== 0x141a9fcc0 ========

longlong FUN_141a9fcc0(void)

{
  if (DAT_145a354a0 == 0) {
    FUN_14203a4b0(&DAT_145a354a0,&PTR_LAB_144529500);
  }
  return DAT_145a354a0;
}



// ======== 0x141752b00 ========

longlong FUN_141752b00(void)

{
  if (DAT_1459950e8 == 0) {
    FUN_14203ad30(&DAT_1459950e8,&PTR_s__Script_AT_143f868c8);
  }
  return DAT_1459950e8;
}



// ======== 0x1417bd960 ========

longlong FUN_1417bd960(void)

{
  if (DAT_1459b26d0 == 0) {
    FUN_14203aad0(&DAT_1459b26d0,&PTR_FUN_1454f0410);
  }
  return DAT_1459b26d0;
}



// ======== 0x1417bd990 ========

longlong FUN_1417bd990(void)

{
  if (DAT_1459b26d8 == 0) {
    FUN_14203aad0(&DAT_1459b26d8,&PTR_FUN_1454f0450);
  }
  return DAT_1459b26d8;
}



// ======== 0x1417bd9c0 ========

longlong FUN_1417bd9c0(void)

{
  if (DAT_1459b26e0 == 0) {
    FUN_14203aad0(&DAT_1459b26e0,&PTR_FUN_1454f0510);
  }
  return DAT_1459b26e0;
}



// ======== 0x1417bd9f0 ========

longlong FUN_1417bd9f0(void)

{
  if (DAT_1459b26e8 == 0) {
    FUN_14203aad0(&DAT_1459b26e8,&PTR_FUN_1454f0550);
  }
  return DAT_1459b26e8;
}



// ======== 0x1417bda20 ========

longlong FUN_1417bda20(void)

{
  if (DAT_1459b26f0 == 0) {
    FUN_14203aad0(&DAT_1459b26f0,&PTR_FUN_1454f0590);
  }
  return DAT_1459b26f0;
}



// ======== 0x1417bd0d0 ========

longlong FUN_1417bd0d0(void)

{
  if (DAT_1459b2700 == 0) {
    FUN_141f291f0(L"/Script/AT",L"AT_UIXcmnAgreement",&DAT_1459b2700,FUN_1417be950,0x5e8,0x10000000,
                  0,L"Engine",&LAB_1417be1d0,FUN_1407e4520,_guard_check_icall,&LAB_14176a290,
                  &LAB_14083c250,0);
  }
  return DAT_1459b2700;
}



// ======== 0x14203a4b0 ========

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



// ======== 0x14203ad30 ========

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



// ======== 0x14203aad0 ========

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



// ======== 0x141f291f0 ========

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



