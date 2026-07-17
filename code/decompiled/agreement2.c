// UAT_UIXcmnAgreement focused decompile (exec + impl + impl callees).
// Anchors: +0x3A8 SubjectImage, +0x3B0 HeadImage, +0x3C0 LeftArrow,
//   +0x3C8 RightArrow, +0x508 ImageTextureMap; tail 0x3E0..0x508 = state.

// ======== EXEC A:CheckHovered  @ 1417bd5a0 ========

longlong FUN_1417bd5a0(void)

{
  if (DAT_1459b2488 == 0) {
    FUN_14203aad0(&DAT_1459b2488,&PTR_FUN_1454ece60);
  }
  return DAT_1459b2488;
}



// ======== IMPL A:CheckHovered  @ 14203aad0 ========

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



// ======== CALLEE-of A:CheckHovered  @ 1407f7cf0 ========

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



// ======== CALLEE-of A:CheckHovered  @ 141eb3490 ========

undefined8 * FUN_141eb3490(undefined8 *param_1,longlong param_2,undefined4 param_3)

{
  if (param_2 != 0) {
    FUN_141ebf920(param_1,param_2,0,param_3,1,0xffffffff);
    return param_1;
  }
  *param_1 = 0;
  return param_1;
}



// ======== CALLEE-of A:CheckHovered  @ 141f282a0 ========

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



// ======== CALLEE-of A:CheckHovered  @ 14204bcd0 ========

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



// ======== CALLEE-of A:CheckHovered  @ 142035810 ========

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



// ======== CALLEE-of A:CheckHovered  @ 141f1eec0 ========

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



// ======== CALLEE-of A:CheckHovered  @ 1420361d0 ========

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



// ======== CALLEE-of A:CheckHovered  @ 141f28760 ========

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



// ======== CALLEE-of A:CheckHovered  @ 141de9200 ========

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



// ======== CALLEE-of A:CheckHovered  @ 14203ae50 ========

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



// ======== CALLEE-of A:CheckHovered  @ 141fd5900 ========

void FUN_141fd5900(longlong *param_1,undefined1 param_2)

{
  undefined1 local_98 [144];
  
  FUN_141eaae00(local_98);
  (**(code **)(*param_1 + 600))(param_1,local_98,param_2);
  FUN_141eab520(local_98);
  return;
}



// ======== CALLEE-of A:CheckHovered  @ 143820a10 ========

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



// ======== EXEC A:In_Headder  @ 1417bd5d0 ========

longlong FUN_1417bd5d0(void)

{
  if (DAT_1459b2490 == 0) {
    FUN_14203aad0(&DAT_1459b2490,&PTR_FUN_1454ecea0);
  }
  return DAT_1459b2490;
}



// ======== EXEC A:In_Win  @ 1417bd600 ========

longlong FUN_1417bd600(void)

{
  if (DAT_1459b2498 == 0) {
    FUN_14203aad0(&DAT_1459b2498,&PTR_FUN_1454ecee0);
  }
  return DAT_1459b2498;
}



// ======== EXEC A:Loop_Arrow  @ 1417bd630 ========

longlong FUN_1417bd630(void)

{
  if (DAT_1459b24a0 == 0) {
    FUN_14203aad0(&DAT_1459b24a0,&PTR_FUN_1454ecfa0);
  }
  return DAT_1459b24a0;
}



// ======== EXEC A:MouseClickDecide  @ 1417bd660 ========

longlong FUN_1417bd660(void)

{
  if (DAT_1459b24a8 == 0) {
    FUN_14203aad0(&DAT_1459b24a8,&PTR_FUN_1454ecfe0);
  }
  return DAT_1459b24a8;
}



// ======== EXEC A:MouseClickTabLeft  @ 1417bd690 ========

longlong FUN_1417bd690(void)

{
  if (DAT_1459b24b0 == 0) {
    FUN_14203aad0(&DAT_1459b24b0,&PTR_FUN_1454ed020);
  }
  return DAT_1459b24b0;
}



// ======== EXEC A:MouseClickTabRight  @ 1417bd6c0 ========

longlong FUN_1417bd6c0(void)

{
  if (DAT_1459b24b8 == 0) {
    FUN_14203aad0(&DAT_1459b24b8,&PTR_FUN_1454ed060);
  }
  return DAT_1459b24b8;
}



// ======== EXEC A:Out_Headder  @ 1417bd6f0 ========

longlong FUN_1417bd6f0(void)

{
  if (DAT_1459b24c0 == 0) {
    FUN_14203aad0(&DAT_1459b24c0,&PTR_FUN_1454ed0a0);
  }
  return DAT_1459b24c0;
}



// ======== EXEC A2:CheckHovered  @ 1417bd990 ========

longlong FUN_1417bd990(void)

{
  if (DAT_1459b26d8 == 0) {
    FUN_14203aad0(&DAT_1459b26d8,&PTR_FUN_1454f0450);
  }
  return DAT_1459b26d8;
}



// ======== EXEC A2:Loop_Arrow  @ 1417bd9c0 ========

longlong FUN_1417bd9c0(void)

{
  if (DAT_1459b26e0 == 0) {
    FUN_14203aad0(&DAT_1459b26e0,&PTR_FUN_1454f0510);
  }
  return DAT_1459b26e0;
}



// ======== EXEC A2:MouseClickDecide  @ 1417bd9f0 ========

longlong FUN_1417bd9f0(void)

{
  if (DAT_1459b26e8 == 0) {
    FUN_14203aad0(&DAT_1459b26e8,&PTR_FUN_1454f0550);
  }
  return DAT_1459b26e8;
}



// ======== EXEC A2:MouseClickTabLeft  @ 1417bda20 ========

longlong FUN_1417bda20(void)

{
  if (DAT_1459b26f0 == 0) {
    FUN_14203aad0(&DAT_1459b26f0,&PTR_FUN_1454f0590);
  }
  return DAT_1459b26f0;
}



// ======== EXEC B:CheckHovered  @ 1417befe0 ========

void FUN_1417befe0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416b6650();
  return;
}



// ======== EXEC B2:CheckHovered  @ 1417bf000 ========

void FUN_1417bf000(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416e49a0();
  return;
}



// ======== EXEC B3:CheckHovered  @ 1417bf020 ========

void FUN_1417bf020(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416e4b40();
  return;
}



// ======== EXEC B:DebugChangeLangage  @ 1417bf040 ========

void FUN_1417bf040(undefined8 param_1,longlong param_2)

{
  undefined1 local_res10 [16];
  undefined1 local_res20 [8];
  
  local_res20[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res20);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res20);
  }
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  _guard_check_icall(local_res20[0],local_res10[0]);
  return;
}



// ======== IMPL B:DebugChangeLangage  @ 14204e7f0 ========

void FUN_14204e7f0(longlong param_1,undefined8 param_2)

{
  byte bVar1;
  
  bVar1 = **(byte **)(param_1 + 0x20);
  *(byte **)(param_1 + 0x20) = *(byte **)(param_1 + 0x20) + 1;
                    /* WARNING: Could not recover jumptable at 0x00014204e812. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(&DAT_145b9d480)[bVar1])(param_2,param_1);
  return;
}



// ======== EXEC B:In_Headder  @ 1417bf0f0 ========

void FUN_1417bf0f0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416bea70();
  return;
}



// ======== EXEC B:In_Win  @ 1417bf110 ========

void FUN_1417bf110(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416beb70();
  return;
}



// ======== EXEC B2:In_Win  @ 1417bf130 ========

void FUN_1417bf130(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416ebb10();
  return;
}



// ======== EXEC B:Loop_Arrow  @ 1417bf150 ========

void FUN_1417bf150(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416c2180();
  return;
}



// ======== EXEC B2:Loop_Arrow  @ 1417bf170 ========

void FUN_1417bf170(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416ee780();
  return;
}



// ======== EXEC B3:Loop_Arrow  @ 1417bf190 ========

void FUN_1417bf190(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416f29f0();
  return;
}



// ======== EXEC B:MouseClickDecide  @ 1417bf1b0 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1417bf1b0(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  int *piVar2;
  longlong *plVar3;
  undefined1 *puVar4;
  undefined ***pppuVar5;
  undefined1 auStack_e8 [32];
  undefined **local_c8;
  ushort local_c0;
  undefined1 local_be;
  undefined4 local_bc;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 *local_98;
  undefined8 local_90;
  undefined8 local_88;
  longlong *plStack_80;
  undefined8 local_78;
  undefined4 local_70;
  undefined2 local_6c;
  undefined8 local_68;
  undefined1 local_60;
  undefined1 local_58 [56];
  ulonglong local_20;
  
  local_20 = DAT_145909780 ^ (ulonglong)auStack_e8;
  FUN_1420b2130(local_58);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_58);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_58);
  }
  local_be = 0;
  local_bc = 0;
  puVar4 = local_58;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar4 = *(undefined1 **)(param_2 + 0x38);
  }
  local_88 = 0;
  plStack_80 = (longlong *)0x0;
  local_c0 = local_c0 & 0xfe00;
  local_70 = DAT_1450d6c88;
  local_c8 = &PTR_FUN_143fd02c0;
  local_b8 = 0;
  local_98 = &DAT_145ba1000;
  local_b0 = 0;
  local_a8 = 0;
  uStack_a0 = 0;
  local_90 = 0;
  local_78 = 0;
  local_6c = 0;
  local_68 = 0;
  local_60 = 0;
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_c8);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_c8);
  }
  pppuVar5 = &local_c8;
  if (*(undefined ****)(param_2 + 0x38) != (undefined ***)0x0) {
    pppuVar5 = *(undefined ****)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416c2300(param_1,puVar4,pppuVar5);
  plVar3 = plStack_80;
  if (plStack_80 != (longlong *)0x0) {
    plVar1 = plStack_80 + 1;
    *(int *)plVar1 = (int)*plVar1 + -1;
    if ((int)*plVar1 == 0) {
      (**(code **)*plStack_80)(plStack_80);
      piVar2 = (int *)((longlong)plVar3 + 0xc);
      *piVar2 = *piVar2 + -1;
      if (*piVar2 == 0) {
        (**(code **)(*plVar3 + 8))(plVar3,1);
      }
    }
  }
  return;
}



// ======== IMPL B:MouseClickDecide  @ 1420b2130 ========

undefined8 * FUN_1420b2130(undefined8 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0x3f800000;
  *(undefined4 *)(param_1 + 2) = 0;
  *(undefined4 *)((longlong)param_1 + 0x24) = 0;
  *(undefined8 *)((longlong)param_1 + 0x1c) = 0x3f800000;
  param_1[5] = 0x3f800000;
  *(undefined4 *)(param_1 + 6) = 0;
  *(byte *)((longlong)param_1 + 0x34) = *(byte *)((longlong)param_1 + 0x34) & 0xfe;
  return param_1;
}



// ======== EXEC B2:MouseClickDecide  @ 1417bf340 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1417bf340(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  int *piVar2;
  longlong *plVar3;
  undefined1 *puVar4;
  undefined ***pppuVar5;
  undefined1 auStack_e8 [32];
  undefined **local_c8;
  ushort local_c0;
  undefined1 local_be;
  undefined4 local_bc;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 *local_98;
  undefined8 local_90;
  undefined8 local_88;
  longlong *plStack_80;
  undefined8 local_78;
  undefined4 local_70;
  undefined2 local_6c;
  undefined8 local_68;
  undefined1 local_60;
  undefined1 local_58 [56];
  ulonglong local_20;
  
  local_20 = DAT_145909780 ^ (ulonglong)auStack_e8;
  FUN_1420b2130(local_58);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_58);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_58);
  }
  local_be = 0;
  local_bc = 0;
  puVar4 = local_58;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar4 = *(undefined1 **)(param_2 + 0x38);
  }
  local_88 = 0;
  plStack_80 = (longlong *)0x0;
  local_c0 = local_c0 & 0xfe00;
  local_70 = DAT_1450d6c88;
  local_c8 = &PTR_FUN_143fd02c0;
  local_b8 = 0;
  local_98 = &DAT_145ba1000;
  local_b0 = 0;
  local_a8 = 0;
  uStack_a0 = 0;
  local_90 = 0;
  local_78 = 0;
  local_6c = 0;
  local_68 = 0;
  local_60 = 0;
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_c8);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_c8);
  }
  pppuVar5 = &local_c8;
  if (*(undefined ****)(param_2 + 0x38) != (undefined ***)0x0) {
    pppuVar5 = *(undefined ****)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  thunk_FUN_1416ed3c0(param_1,puVar4,pppuVar5);
  plVar3 = plStack_80;
  if (plStack_80 != (longlong *)0x0) {
    plVar1 = plStack_80 + 1;
    *(int *)plVar1 = (int)*plVar1 + -1;
    if ((int)*plVar1 == 0) {
      (**(code **)*plStack_80)(plStack_80);
      piVar2 = (int *)((longlong)plVar3 + 0xc);
      *piVar2 = *piVar2 + -1;
      if (*piVar2 == 0) {
        (**(code **)(*plVar3 + 8))(plVar3,1);
      }
    }
  }
  return;
}



// ======== EXEC B3:MouseClickDecide  @ 1417bf4d0 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1417bf4d0(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  int *piVar2;
  longlong *plVar3;
  undefined1 *puVar4;
  undefined ***pppuVar5;
  undefined1 auStack_e8 [32];
  undefined **local_c8;
  ushort local_c0;
  undefined1 local_be;
  undefined4 local_bc;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 *local_98;
  undefined8 local_90;
  undefined8 local_88;
  longlong *plStack_80;
  undefined8 local_78;
  undefined4 local_70;
  undefined2 local_6c;
  undefined8 local_68;
  undefined1 local_60;
  undefined1 local_58 [56];
  ulonglong local_20;
  
  local_20 = DAT_145909780 ^ (ulonglong)auStack_e8;
  FUN_1420b2130(local_58);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_58);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_58);
  }
  local_be = 0;
  local_bc = 0;
  puVar4 = local_58;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar4 = *(undefined1 **)(param_2 + 0x38);
  }
  local_88 = 0;
  plStack_80 = (longlong *)0x0;
  local_c0 = local_c0 & 0xfe00;
  local_70 = DAT_1450d6c88;
  local_c8 = &PTR_FUN_143fd02c0;
  local_b8 = 0;
  local_98 = &DAT_145ba1000;
  local_b0 = 0;
  local_a8 = 0;
  uStack_a0 = 0;
  local_90 = 0;
  local_78 = 0;
  local_6c = 0;
  local_68 = 0;
  local_60 = 0;
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_c8);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_c8);
  }
  pppuVar5 = &local_c8;
  if (*(undefined ****)(param_2 + 0x38) != (undefined ***)0x0) {
    pppuVar5 = *(undefined ****)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416eec40(param_1,puVar4,pppuVar5);
  plVar3 = plStack_80;
  if (plStack_80 != (longlong *)0x0) {
    plVar1 = plStack_80 + 1;
    *(int *)plVar1 = (int)*plVar1 + -1;
    if ((int)*plVar1 == 0) {
      (**(code **)*plStack_80)(plStack_80);
      piVar2 = (int *)((longlong)plVar3 + 0xc);
      *piVar2 = *piVar2 + -1;
      if (*piVar2 == 0) {
        (**(code **)(*plVar3 + 8))(plVar3,1);
      }
    }
  }
  return;
}



// ======== EXEC B:MouseClickTabLeft  @ 1417bf660 ========

void FUN_1417bf660(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416c2370();
  return;
}



// ======== EXEC B2:MouseClickTabLeft  @ 1417bf680 ========

void FUN_1417bf680(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416eed50();
  return;
}



// ======== EXEC B:MouseClickTabRight  @ 1417bf6a0 ========

void FUN_1417bf6a0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416c23a0();
  return;
}



// ======== EXEC B2:MouseClickTabRight  @ 1417bf6c0 ========

void FUN_1417bf6c0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416eee20();
  return;
}



// ======== EXEC B:Out_Headder  @ 1417bf6e0 ========

void FUN_1417bf6e0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416c6c00();
  return;
}



// ======== EXEC B:Out_Win  @ 1417bf700 ========

void FUN_1417bf700(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416c6ca0();
  return;
}



// ======== EXEC B2:Out_Win  @ 1417bf720 ========

void FUN_1417bf720(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416f25e0();
  return;
}



// ======== EXEC B:SettoUICommonInput  @ 1417bf740 ========

void FUN_1417bf740(undefined8 param_1,longlong param_2)

{
  undefined8 local_res10 [3];
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416fc420(local_res10[0]);
  return;
}



