// UAT_UIXcmnAgreement cluster-B impls + callees (depth 3).
// this@RCX. Reflected: +0x3A8 SubjectImage, +0x3B0 HeadImage,
//   +0x3C0 LeftArrow, +0x3C8 RightArrow, +0x508 ImageTextureMap.
//   Tail 0x3E0..0x508 = unreflected state.

// ======== SEED CheckHovered  @ 1416b6650 ========

void FUN_1416b6650(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  
  uVar2 = FUN_1411fc890();
  cVar1 = FUN_14120f070(uVar2);
  if (cVar1 != '\0') {
    uVar2 = 0;
    cVar1 = (**(code **)(**(longlong **)(param_1 + 0x3b8) + 600))();
    if ((cVar1 != '\0') ||
       (cVar1 = (**(code **)(**(longlong **)(param_1 + 0x3c0) + 600))(), cVar1 != '\0')) {
      uVar2 = 1;
    }
    FUN_140d3c540(uVar2);
    return;
  }
  return;
}



// ======== SEED In_Headder  @ 1416bea70 ========

void FUN_1416bea70(longlong param_1)

{
  if (*(char *)(param_1 + 0x41e) == '\x02') {
    if (*(longlong *)(param_1 + 0x400) != 0) {
      FUN_1416f2c70();
    }
    if (*(longlong *)(param_1 + 0x3e0) != 0) {
      FUN_1416ca990();
      return;
    }
  }
  return;
}



// ======== SEED In_Win  @ 1416beb70 ========

void FUN_1416beb70(longlong param_1)

{
  int iVar1;
  char cVar2;
  
  iVar1 = *(int *)(param_1 + 0x420);
  if (((-1 < iVar1) && (iVar1 < *(int *)(param_1 + 0x3d0))) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x3c8) + (longlong)iVar1 * 8) != 0)) {
    cVar2 = FUN_1429aef50();
    if (cVar2 == '\x03') {
      FUN_1416f2c30(*(undefined8 *)
                     (*(longlong *)(param_1 + 0x3c8) + (longlong)*(int *)(param_1 + 0x420) * 8));
      return;
    }
  }
  return;
}



// ======== SEED Loop_Arrow  @ 1416c2180 ========

void FUN_1416c2180(longlong param_1)

{
  longlong *plVar1;
  uint uVar2;
  int iVar3;
  
  if (*(char *)(param_1 + 0x41e) != '\x03') {
    plVar1 = *(longlong **)(param_1 + 0x3b8);
    if (plVar1 != (longlong *)0x0) {
      uVar2 = *(int *)(param_1 + 0x424) - 1;
      if ((int)uVar2 < 0) {
        uVar2 = 0xffffffff;
      }
      (**(code **)(*plVar1 + 0x468))(plVar1,uVar2 >> 0x1f ^ 1);
    }
    plVar1 = *(longlong **)(param_1 + 0x3c0);
    if (plVar1 != (longlong *)0x0) {
      iVar3 = *(int *)(param_1 + 0x424) + 1;
      if (*(int *)(param_1 + 0x448) <= iVar3) {
        iVar3 = -1;
      }
      (**(code **)(*plVar1 + 0x468))(plVar1,(byte)((uint)iVar3 >> 0x1f) ^ 1);
    }
  }
  FUN_1416d6cb0(param_1,1);
  return;
}



// ======== SEED MouseClickDecide  @ 1416c2300 ========

void FUN_1416c2300(longlong param_1)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  
  uVar3 = FUN_1411fc890();
  cVar1 = FUN_14120f070(uVar3);
  if (cVar1 == '\0') {
    return;
  }
  iVar2 = *(int *)(param_1 + 0x424) + 1;
  if ((iVar2 < *(int *)(param_1 + 0x448)) && (-1 < iVar2)) {
    if ((*(byte *)(param_1 + 0x1ac) & 8) != 0) {
      return;
    }
    FUN_1416c3c20(param_1);
    *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 8;
    *(undefined4 *)(param_1 + 0x418) = *(undefined4 *)(param_1 + 0x3a0);
  }
  FUN_1416c3980(param_1);
  return;
}



// ======== SEED MouseClickTabLeft  @ 1416c2370 ========

void FUN_1416c2370(undefined8 param_1)

{
  undefined8 uVar1;
  char cVar2;
  
  uVar1 = FUN_1411fc890();
  cVar2 = FUN_14120f070(uVar1);
  if (cVar2 != '\0') {
    FUN_1416c3a70(param_1);
    return;
  }
  return;
}



// ======== SEED MouseClickTabRight  @ 1416c23a0 ========

void FUN_1416c23a0(undefined8 param_1)

{
  undefined8 uVar1;
  char cVar2;
  
  uVar1 = FUN_1411fc890();
  cVar2 = FUN_14120f070(uVar1);
  if (cVar2 != '\0') {
    FUN_1416c3c20(param_1);
    return;
  }
  return;
}



// ======== SEED Out_Headder  @ 1416c6c00 ========

void FUN_1416c6c00(longlong param_1)

{
  if (*(char *)(param_1 + 0x41e) == '\x02') {
    if (*(longlong *)(param_1 + 0x400) != 0) {
      FUN_1416f34e0();
    }
    if (*(longlong *)(param_1 + 0x3e0) != 0) {
      FUN_1416ca9e0();
      return;
    }
  }
  return;
}



// ======== d1 callee-of 0x1416b6650  @ 1411fc890 ========

undefined8 FUN_1411fc890(void)

{
  return DAT_14595f480;
}



// ======== d1 callee-of 0x1416b6650  @ 14120f070 ========

undefined1 FUN_14120f070(longlong param_1)

{
  if (*(longlong *)(param_1 + 0x20a8) != 0) {
    return *(undefined1 *)(*(longlong *)(param_1 + 0x20a8) + 0x32);
  }
  return 0;
}



// ======== d1 callee-of 0x1416b6650  @ 140d3c540 ========

void FUN_140d3c540(char param_1)

{
  longlong lVar1;
  
  lVar1 = FUN_140d2a280();
  *(char *)(lVar1 + 0x571) = param_1 * '\b' + '\x01';
  return;
}



// ======== d1 callee-of 0x1416bea70  @ 1416f2c70 ========

void FUN_1416f2c70(longlong *param_1)

{
  char cVar1;
  
  (**(code **)(*param_1 + 0x250))(param_1,3);
  cVar1 = FUN_1416f3230(param_1);
  if (cVar1 == '\0') {
    *(undefined2 *)((longlong)param_1 + 0x401) = 0x100;
    *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | 8;
    return;
  }
  *(undefined1 *)((longlong)param_1 + 0x402) = 1;
  return;
}



// ======== d1 callee-of 0x1416beb70  @ 1429aef50 ========

longlong * FUN_1429aef50(longlong param_1)

{
  int *piVar1;
  char cVar2;
  longlong *plVar3;
  longlong *plVar4;
  char local_res10 [24];
  longlong local_18;
  longlong *local_10;
  
  FUN_1429a8760(param_1,&local_18);
  if (local_18 == 0) {
    plVar4 = (longlong *)(ulonglong)*(byte *)(param_1 + 0xb7);
  }
  else {
    plVar4 = (longlong *)0x0;
    if (((*(int *)(local_18 + 0xb0) == 0) || (*(longlong **)(local_18 + 0xa8) == (longlong *)0x0))
       || (cVar2 = (**(code **)(**(longlong **)(local_18 + 0xa8) + 0x20))(), cVar2 == '\0')) {
      local_res10[0] = *(char *)(local_18 + 0xa0);
    }
    else {
      plVar3 = plVar4;
      if (*(int *)(local_18 + 0xb0) != 0) {
        plVar3 = *(longlong **)(local_18 + 0xa8);
      }
      (**(code **)(*plVar3 + 0x40))(plVar3,local_res10);
      *(char *)(local_18 + 0xa0) = local_res10[0];
    }
    if (local_res10[0] != DAT_1456e8e2c) {
      if (local_res10[0] == DAT_1456e8e2d) {
        plVar4 = (longlong *)0x1;
      }
      else if (local_res10[0] == DAT_1456e8e2e) {
        plVar4 = (longlong *)0x2;
      }
      else if (local_res10[0] == DAT_1456e8e2f) {
        plVar4 = (longlong *)0x3;
      }
      else if (local_res10[0] == DAT_1456e8e30) {
        plVar4 = (longlong *)&DAT_00000004;
      }
    }
  }
  if (local_10 != (longlong *)0x0) {
    plVar3 = local_10 + 1;
    *(int *)plVar3 = (int)*plVar3 + -1;
    if ((int)*plVar3 == 0) {
      (**(code **)*local_10)(local_10);
      piVar1 = (int *)((longlong)local_10 + 0xc);
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*local_10 + 8))(local_10,1);
      }
    }
  }
  return plVar4;
}



// ======== d1 callee-of 0x1416c2300  @ 1416c3c20 ========

void FUN_1416c3c20(longlong *param_1)

{
  longlong lVar1;
  longlong *plVar2;
  bool bVar3;
  undefined4 uVar4;
  int iVar5;
  undefined8 uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  uVar4 = DAT_1450d6c88;
  iVar7 = 0;
  if ((*(char *)((longlong)param_1 + 0x412) != '\0') &&
     (*(char *)((longlong)param_1 + 0x413) == '\0')) {
    if (param_1[0x78] != 0) {
      FUN_1416f3810();
    }
    iVar9 = *(int *)((longlong)param_1 + 0x424) + 1;
    if ((iVar9 < (int)param_1[0x89]) && (-1 < iVar9)) {
      iVar5 = (int)param_1[0x84] + 1;
      lVar1 = param_1[0xa0];
      iVar8 = iVar7;
      if (iVar5 < (int)param_1[0x7a]) {
        iVar8 = iVar5;
      }
      if (lVar1 != 0) {
        uVar6 = (**(code **)(*param_1 + 0x148))(param_1);
        FUN_142dd9f90(uVar6,lVar1,0,1);
        param_1[0xa0] = 0;
      }
      FUN_1416d15e0(param_1,iVar8,iVar9);
      if (((-1 < iVar8) && (iVar8 < (int)param_1[0x7a])) &&
         (plVar2 = *(longlong **)(param_1[0x79] + (longlong)iVar8 * 8), plVar2 != (longlong *)0x0))
      {
        FUN_1416f3780(plVar2);
        (**(code **)(*plVar2 + 0x468))(plVar2,1);
      }
      iVar5 = (int)param_1[0x84];
      if (((-1 < iVar5) && (iVar5 < (int)param_1[0x7a])) &&
         (*(longlong *)(param_1[0x79] + (longlong)iVar5 * 8) != 0)) {
        FUN_1416141a0();
      }
      *(int *)(param_1 + 0x84) = iVar8;
      *(int *)((longlong)param_1 + 0x424) = iVar9;
      (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x7f],0,1,0,uVar4);
      if ((longlong *)param_1[0xa0] != (longlong *)0x0) {
        (**(code **)(*(longlong *)param_1[0xa0] + 0x618))();
      }
      *(undefined1 *)((longlong)param_1 + 0x413) = 1;
      bVar3 = true;
      goto LAB_1416c3da0;
    }
  }
  bVar3 = false;
LAB_1416c3da0:
  if (!bVar3) {
    iVar7 = 3;
  }
  FUN_141485f00(iVar7,uVar4,0,0);
  return;
}



// ======== d1 callee-of 0x1416c6c00  @ 1416f34e0 ========

void FUN_1416f34e0(longlong *param_1)

{
  longlong lVar1;
  
  if ((char)param_1[0x80] == '\0') {
    lVar1 = param_1[0x7c];
  }
  else {
    lVar1 = param_1[0x7f];
  }
  (**(code **)(*param_1 + 0x2e0))(DAT_1450d6c88,lVar1,0,1,0,DAT_1450d6c88);
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0xf7;
  *(undefined2 *)((longlong)param_1 + 0x401) = 1;
  return;
}



// ======== d2 callee-of 0x140d3c540  @ 140d2a280 ========

longlong FUN_140d2a280(void)

{
  bool bVar1;
  char cVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong lVar5;
  int local_res8 [8];
  
  lVar5 = DAT_14592f0e8;
  if (DAT_14592f0e8 != 0) {
    uVar3 = FUN_1418fb4b0();
    FUN_140cfa640(lVar5 + 0x550,local_res8,uVar3);
    if ((((local_res8[0] != -1) &&
         (lVar5 = *(longlong *)(lVar5 + 0x550) + (longlong)local_res8[0] * 0x28, lVar5 != 0)) &&
        (lVar5 != -8)) && (lVar5 = *(longlong *)(lVar5 + 0x10), lVar5 != 0)) {
      lVar4 = FUN_1418fb4b0();
      if ((*(int *)(*(longlong *)(lVar5 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
         (*(longlong *)
           (*(longlong *)(*(longlong *)(lVar5 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8
           ) != lVar4 + 0x88)) {
        bVar1 = false;
      }
      else {
        bVar1 = true;
      }
      if (((bVar1) && (cVar2 = FUN_141435950(lVar5), cVar2 != '\0')) &&
         ((cVar2 = FUN_1414359a0(lVar5), cVar2 == '\0' && (*(longlong *)(lVar5 + 0x130) != 0)))) {
        lVar5 = *(longlong *)(*(longlong *)(lVar5 + 0x130) + 0x370);
        if (lVar5 == 0) {
          return 0;
        }
        lVar4 = FUN_14323a770();
        if ((*(int *)(*(longlong *)(lVar5 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(lVar5 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar4 + 0x90) * 8) != lVar4 + 0x88)) {
          bVar1 = false;
        }
        else {
          bVar1 = true;
        }
        if (!bVar1) {
          return 0;
        }
        return lVar5;
      }
    }
  }
  uVar3 = FUN_1411fc890();
  if (DAT_14592f0f0 == 0) {
    lVar5 = FUN_142d1f150(uVar3,0);
    return lVar5;
  }
  if (DAT_14592f0f0 != 1) {
    FUN_142d1f150(uVar3,0);
  }
  lVar5 = DAT_14592f0e8;
  if (DAT_14592f0e8 == 0) {
    return 0;
  }
  FUN_140d1b280(DAT_14592f0e8);
  return *(longlong *)(lVar5 + 0x628);
}



// ======== d2 callee-of 0x1416f2c70  @ 1416f3230 ========

undefined1 FUN_1416f3230(longlong *param_1)

{
  char cVar1;
  longlong lVar2;
  
  if ((int)param_1[0x47] != *(int *)((longlong)param_1 + 0x264)) {
    cVar1 = FUN_141a59cd0();
    if (cVar1 == '\0') {
      return 0;
    }
  }
  if ((char)param_1[0x80] == '\0') {
    lVar2 = param_1[0x7a];
  }
  else {
    lVar2 = param_1[0x7d];
  }
  (**(code **)(*param_1 + 0x2e0))(param_1,lVar2,0,1,0,DAT_1450d6c88);
  (**(code **)(*param_1 + 0x468))(param_1,1);
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0xf7;
  *(undefined1 *)((longlong)param_1 + 0x401) = 0;
  return 1;
}



// ======== d2 callee-of 0x1429aef50  @ 1429a8760 ========

undefined8 * FUN_1429a8760(longlong param_1,undefined8 *param_2)

{
  longlong *plVar1;
  int *piVar2;
  int iVar3;
  longlong lVar4;
  undefined8 uVar5;
  longlong lVar6;
  longlong *plVar7;
  
  if (((*(longlong *)(param_1 + 0xe0) != 0) &&
      (plVar7 = *(longlong **)(param_1 + 0xe8), plVar7 != (longlong *)0x0)) &&
     (iVar3 = (int)plVar7[1], 0 < iVar3)) {
    uVar5 = 0;
    if (plVar7 != (longlong *)0x0) {
      if (iVar3 == 0) {
        plVar7 = (longlong *)0x0;
      }
      else {
        *(int *)(plVar7 + 1) = iVar3 + 1;
      }
    }
    if (plVar7 != (longlong *)0x0) {
      uVar5 = *(undefined8 *)(param_1 + 0xe0);
    }
    *param_2 = uVar5;
    param_2[1] = plVar7;
    if (plVar7 != (longlong *)0x0) {
      *(int *)(plVar7 + 1) = (int)plVar7[1] + 1;
      plVar1 = plVar7 + 1;
      *(int *)plVar1 = (int)*plVar1 + -1;
      if ((int)*plVar1 == 0) {
        (**(code **)*plVar7)(plVar7);
        piVar2 = (int *)((longlong)plVar7 + 0xc);
        *piVar2 = *piVar2 + -1;
        if (*piVar2 == 0) {
          (**(code **)(*plVar7 + 8))(plVar7,1);
        }
      }
    }
    return param_2;
  }
  lVar6 = 0;
  *param_2 = 0;
  lVar4 = *(longlong *)(param_1 + 0xd8);
  param_2[1] = lVar4;
  if (lVar4 != 0) {
    if (*(int *)(lVar4 + 8) == 0) {
      param_2[1] = 0;
    }
    else {
      *(int *)(lVar4 + 8) = *(int *)(lVar4 + 8) + 1;
      lVar6 = param_2[1];
    }
    if (lVar6 != 0) {
      *param_2 = *(undefined8 *)(param_1 + 0xd0);
    }
  }
  return param_2;
}



// ======== d2 callee-of 0x1416c3c20  @ 1416f3810 ========

void FUN_1416f3810(longlong *param_1)

{
  longlong *plVar1;
  char cVar2;
  longlong lVar3;
  
  FUN_1429e4dc0();
  if (((int)param_1[0x7a] != 0) && ((longlong *)param_1[0x79] != (longlong *)0x0)) {
    cVar2 = (**(code **)(*(longlong *)param_1[0x79] + 0x20))();
    if ((cVar2 != '\0') &&
       (((int)param_1[0x7a] != 0 && (plVar1 = (longlong *)param_1[0x79], plVar1 != (longlong *)0x0))
       )) {
      (**(code **)(*plVar1 + 0x30))(plVar1,0);
      if (param_1[0x79] != 0) {
        lVar3 = FUN_141df5850(param_1[0x79],0,0);
        param_1[0x79] = lVar3;
      }
      *(undefined4 *)(param_1 + 0x7a) = 0;
    }
  }
  (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x77],0,1,0,DAT_1450d6c88);
  return;
}



// ======== d2 callee-of 0x1416c3c20  @ 142dd9f90 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_142dd9f90(longlong param_1,longlong *param_2,char param_3,undefined1 param_4)

{
  char cVar1;
  uint uVar2;
  ulonglong uVar3;
  longlong *plVar4;
  longlong lVar5;
  byte bVar6;
  uint uVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  int iVar10;
  ulonglong uVar11;
  ulonglong uVar12;
  undefined1 auStack_148 [32];
  undefined1 local_128;
  longlong local_120;
  undefined8 local_118;
  undefined8 local_110 [24];
  undefined8 *local_50;
  undefined8 local_48;
  ulonglong local_40;
  
  local_40 = DAT_145909780 ^ (ulonglong)auStack_148;
  local_128 = param_4;
  (**(code **)(*param_2 + 0x148))(param_2);
  if ((*(byte *)((longlong)param_2 + 0x94) & 2) == 0) {
    iVar10 = *(int *)((longlong)param_2 + 0xc);
    uVar12 = 0;
    uVar3 = uVar12;
    if (iVar10 < (int)DAT_145b9e804) {
      uVar7 = iVar10 >> 0x1f & 0xffff;
      uVar2 = iVar10 + uVar7;
      uVar3 = *(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar2 >> 0x10) * 8) +
              (longlong)(int)((uVar2 & 0xffff) - uVar7) * 0x18;
    }
    if ((*(uint *)(uVar3 + 8) >> 0x1d & 1) == 0) {
      cVar1 = FUN_143135f20(param_1);
      if (cVar1 == '\0') {
        (**(code **)(*param_2 + 0x68))(param_2,1);
        bVar6 = *(byte *)((longlong)param_2 + 0x94);
      }
      else {
        plVar4 = (longlong *)FUN_143133a90(param_1,0,1);
        if ((plVar4 == param_2) ||
           (((((char)param_2[0x25] != '\x03' && (param_3 == '\0')) &&
             ((*(byte *)(param_2 + 0x12) & 2) == 0)) ||
            (cVar1 = (**(code **)(*param_2 + 0x490))(param_2), cVar1 != '\0')))) {
          return 0;
        }
        bVar6 = *(byte *)((longlong)param_2 + 0x94);
        if ((bVar6 & 0x18) == 8) {
          *(byte *)((longlong)param_2 + 0x94) = bVar6 | 4;
          return 1;
        }
      }
      *(byte *)((longlong)param_2 + 0x94) = bVar6 | 2;
      plVar4 = (longlong *)FUN_142c950f0();
      (**(code **)(*plVar4 + 0x60))(plVar4,param_2);
      (**(code **)(*param_2 + 0x520))(param_2);
      local_120 = 0;
      local_118 = 0;
      FUN_142b15b70(param_2,&local_120);
      if (0 < (int)local_118) {
        local_50 = (undefined8 *)0x0;
        local_48 = 0;
        FUN_14121ba40(param_2,local_110,0);
        puVar8 = local_50;
        uVar3 = uVar12;
        while ((iVar10 = (int)uVar3, -1 < iVar10 && (iVar10 < (int)local_118))) {
          lVar5 = *(longlong *)(local_120 + (longlong)iVar10 * 8);
          if (lVar5 != 0) {
            puVar9 = local_110;
            if (puVar8 != (undefined8 *)0x0) {
              puVar9 = puVar8;
            }
            uVar3 = (longlong)(int)local_48 & 0x1fffffffffffffff;
            if (puVar9 + (int)local_48 < puVar9) {
              uVar3 = uVar12;
            }
            uVar11 = uVar12;
            if (uVar3 != 0) {
              do {
                FUN_142b0fd90(lVar5,*puVar9);
                uVar11 = uVar11 + 1;
                puVar9 = puVar9 + 1;
                puVar8 = local_50;
              } while (uVar11 != uVar3);
            }
          }
          uVar3 = (ulonglong)(iVar10 + 1);
        }
        param_4 = local_128;
        if (puVar8 != (undefined8 *)0x0) {
          FUN_141de9200(puVar8);
          param_4 = local_128;
        }
      }
      if ((param_2[0x2d] != 0) && (lVar5 = *(longlong *)(param_2[0x2d] + 0x110), lVar5 != 0)) {
        plVar4 = *(longlong **)(lVar5 + 0xf0);
        if (plVar4 != (longlong *)0x0) {
          (**(code **)(*plVar4 + 0x68))(plVar4,1);
        }
        FUN_142b0fe90(param_2,&DAT_145756070);
      }
      FUN_142b0e120(param_2);
      if (param_2[0x23] != 0) {
        (**(code **)(*param_2 + 0x428))(param_2,0);
      }
      lVar5 = FUN_142b1a260(param_2);
      if (lVar5 != 0) {
        FUN_142db4110(lVar5,param_2);
      }
      lVar5 = FUN_1430daee0(DAT_145c8a740,param_1);
      if ((lVar5 != 0) &&
         (plVar4 = (longlong *)FUN_1430d8da0(DAT_145c8a740,param_1,param_2[0x24]),
         plVar4 != (longlong *)0x0)) {
        (**(code **)(*plVar4 + 0x310))(plVar4,param_2,0);
      }
      plVar4 = *(longlong **)(param_1 + 0x108);
      if (plVar4 != (longlong *)0x0) {
        (**(code **)(*plVar4 + 0x310))(plVar4,param_2,0);
      }
      FUN_14313b240(param_1,param_2,param_4);
      (**(code **)(*param_2 + 0x4b0))(param_2,0);
      iVar10 = *(int *)((longlong)param_2 + 0xc);
      if (iVar10 < (int)DAT_145b9e804) {
        uVar7 = iVar10 >> 0x1f & 0xffff;
        uVar2 = iVar10 + uVar7;
        uVar12 = *(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar2 >> 0x10) * 8) +
                 (longlong)(int)((uVar2 & 0xffff) - uVar7) * 0x18;
      }
      *(uint *)(uVar12 + 8) = *(uint *)(uVar12 + 8) | 0x20000000;
      FUN_142046040(param_2);
      (**(code **)(*param_2 + 0x4c8))(param_2);
      FUN_142b22500(param_2,0,1);
      if (local_120 != 0) {
        FUN_141de9200();
      }
    }
  }
  return 1;
}



// ======== d2 callee-of 0x1416c3c20  @ 1416d15e0 ========

void FUN_1416d15e0(longlong *param_1,int param_2,int param_3)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  longlong *plVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  longlong lVar7;
  longlong *plVar8;
  undefined8 in_stack_ffffffffffffff78;
  undefined4 uVar9;
  longlong local_78 [2];
  undefined1 local_68 [16];
  undefined1 local_58 [48];
  
  uVar9 = (undefined4)((ulonglong)in_stack_ffffffffffffff78 >> 0x20);
  if (-1 < param_2) {
    if (param_2 < (int)param_1[0x7a]) {
      lVar7 = *(longlong *)((longlong)param_2 * 8 + param_1[0x79]);
      if ((((lVar7 != 0) && (-1 < param_3)) && (param_3 < (int)param_1[0x89])) &&
         (plVar8 = (longlong *)((longlong)param_3 * 0x50 + param_1[0x88]), plVar8 != (longlong *)0x0
         )) {
        if (*plVar8 != 0) {
          param_1[0x86] = *plVar8;
        }
        uVar3 = FUN_141eca8b0(param_1 + 0x86,local_78);
        FUN_1416fbfb0(lVar7,uVar3);
        uVar3 = FUN_141eca8b0(plVar8 + 1,local_78);
        FUN_1416fb4f0(lVar7,uVar3);
        FUN_1416fae30(lVar7,param_3 + 1,(int)param_1[0x89]);
        *(undefined1 *)((longlong)param_1 + 0x41c) = 0;
        uVar3 = FUN_141eca8b0(plVar8 + 5,local_78);
        uVar5 = CONCAT44(uVar9,0xffffffff);
        iVar2 = FUN_141db7f10(uVar3,L"TUTO_CTR",1,0,uVar5);
        uVar9 = (undefined4)((ulonglong)uVar5 >> 0x20);
        if (local_78[0] != 0) {
          FUN_141de9200();
        }
        if (iVar2 == -1) {
          uVar3 = FUN_141eca8b0(plVar8 + 3,local_78);
          iVar2 = FUN_141db7f10(uVar3,L"Img_Cmn00_00",1,0,CONCAT44(uVar9,0xffffffff));
          if (local_78[0] != 0) {
            FUN_141de9200();
          }
          if (iVar2 == -1) {
            if (plVar8[3] == 0) {
              cVar1 = FUN_14205a760(plVar8 + 9);
              if (cVar1 == '\0') {
                uVar3 = FUN_141eca8b0(plVar8 + 2,local_68);
                FUN_1416fbac0(lVar7,uVar3);
              }
              else {
                uVar3 = FUN_142058d50(plVar8 + 9);
                uVar5 = FUN_1407e5680(local_68,plVar8 + 7);
                FUN_1416fb9b0(lVar7,uVar5,uVar3);
              }
            }
            else {
              uVar3 = FUN_141eca8b0(plVar8 + 3,local_68);
              uVar5 = FUN_141eca8b0(plVar8 + 2,local_78);
              FUN_1416fb7d0(lVar7,uVar5,uVar3);
            }
          }
          else {
            uVar3 = FUN_141eca8b0(plVar8 + 3,local_78);
            uVar5 = FUN_141eca8b0(plVar8 + 2,local_68);
            FUN_1416f5a50(lVar7,uVar5,uVar3);
          }
        }
        else {
          uVar3 = FUN_141eca8b0(plVar8 + 3,local_78);
          FUN_1416f9930(lVar7,uVar3,plVar8[5]);
        }
        if (((int)plVar8[6] != 0) || (*(int *)((longlong)plVar8 + 0x34) != 0)) {
          plVar4 = (longlong *)FUN_141eca8b0(plVar8 + 6,local_68);
          uVar3 = FUN_143126c80(local_58);
          uVar5 = (**(code **)(*param_1 + 0x148))(param_1);
          uVar6 = FUN_14189efb0();
          lVar7 = FUN_142de92d0(uVar5,uVar6,0,0,uVar3);
          param_1[0xa0] = lVar7;
          if ((lVar7 != 0) &&
             ((param_2 < (int)param_1[0x7a] &&
              (lVar7 = *(longlong *)((longlong)param_2 * 8 + param_1[0x79]), lVar7 != 0)))) {
            uVar3 = *(undefined8 *)(lVar7 + 0x3e0);
            uVar5 = FUN_1407e5680(local_78,plVar4);
            FUN_140befdd0(param_1[0xa0],uVar5,uVar3,1);
          }
          if (*plVar4 != 0) {
            FUN_141de9200();
          }
        }
        *(undefined1 *)(plVar8 + 4) = 1;
        FUN_1416d8670(param_1);
      }
    }
  }
  return;
}



// ======== d2 callee-of 0x1416c3c20  @ 1416f3780 ========

void FUN_1416f3780(longlong *param_1)

{
  (**(code **)(*param_1 + 0x2e0))(DAT_1450d6c88,param_1[0x96],0,1,0,DAT_1450d6c88);
  return;
}



// ======== d2 callee-of 0x1416c3c20  @ 1416141a0 ========

void FUN_1416141a0(longlong *param_1)

{
  (**(code **)(*param_1 + 0x2e0))(DAT_1450d6c88,param_1[0x97],0,1,0,DAT_1450d6c88);
  return;
}



// ======== d3 callee-of 0x140d2a280  @ 1418fb4b0 ========

longlong FUN_1418fb4b0(void)

{
  if (DAT_145a0fa00 == 0) {
    FUN_141f291f0(L"/Script/AT",L"RideManager",&DAT_145a0fa00,FUN_1418fc480,0x1b0,0x10000004,0,
                  L"Engine",&LAB_1418fbac0,FUN_1407e4520,_guard_check_icall,&LAB_140a270a0,
                  &LAB_14083c250,0);
  }
  return DAT_145a0fa00;
}



// ======== d3 callee-of 0x140d2a280  @ 140cfa640 ========

int * FUN_140cfa640(longlong *param_1,int *param_2,ulonglong param_3)

{
  longlong *plVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  
  if ((int)param_1[1] != *(int *)((longlong)param_1 + 0x34)) {
    iVar4 = (int)(param_3 >> 4);
    uVar3 = 0x9e3779b9U - iVar4 ^ iVar4 << 8;
    uVar2 = -(uVar3 + iVar4) ^ uVar3 >> 0xd;
    uVar5 = (iVar4 - uVar3) - uVar2 ^ uVar2 >> 0xc;
    uVar3 = (uVar3 - uVar5) - uVar2 ^ uVar5 << 0x10;
    uVar2 = (uVar2 - uVar3) - uVar5 ^ uVar3 >> 5;
    uVar5 = (uVar5 - uVar3) - uVar2 ^ uVar2 >> 3;
    uVar3 = (uVar3 - uVar5) - uVar2 ^ uVar5 << 10;
    plVar1 = (longlong *)param_1[8];
    if ((longlong *)param_1[8] == (longlong *)0x0) {
      plVar1 = param_1 + 7;
    }
    iVar4 = *(int *)((longlong)plVar1 +
                    (((longlong)(int)((uVar2 - uVar3) - uVar5) ^ (ulonglong)(uVar3 >> 0xf)) &
                    (longlong)(int)param_1[9] - 1U) * 4);
    if (iVar4 != -1) {
      do {
        if (*(ulonglong *)(*param_1 + (longlong)iVar4 * 0x28) == param_3) {
          *param_2 = iVar4;
          return param_2;
        }
        iVar4 = *(int *)(*param_1 + 0x20 + (longlong)iVar4 * 0x28);
      } while (iVar4 != -1);
    }
  }
  *param_2 = -1;
  return param_2;
}



// ======== d3 callee-of 0x140d2a280  @ 141435950 ========

bool FUN_141435950(longlong param_1)

{
  return *(int *)(param_1 + 0x150) != 0;
}



// ======== d3 callee-of 0x140d2a280  @ 1414359a0 ========

bool FUN_1414359a0(longlong param_1)

{
  return *(int *)(param_1 + 0x150) == 3;
}



// ======== d3 callee-of 0x140d2a280  @ 14323a770 ========

longlong FUN_14323a770(void)

{
  if (DAT_145ca17f0 == 0) {
    FUN_141f291f0(L"/Script/Engine",L"PlayerController",&DAT_145ca17f0,FUN_14323d440,0x688,
                  0x10000004,0x2000000000,L"Game",&LAB_14323c050,FUN_1407e4520,FUN_142b0aa80,
                  FUN_143187340,&LAB_14083c250,0);
  }
  return DAT_145ca17f0;
}



// ======== d3 callee-of 0x140d2a280  @ 142d1f150 ========

undefined8 FUN_142d1f150(undefined8 param_1,int param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  int iVar3;
  longlong *local_18;
  int local_10;
  
  lVar1 = FUN_1430daf70(DAT_145c8a740,param_1,1);
  if (lVar1 != 0) {
    iVar3 = 0;
    FUN_143132fc0(lVar1,&local_18);
    for (; (-1 < local_10 && (local_10 < (int)local_18[1])); local_10 = local_10 + 1) {
      uVar2 = FUN_142058d50(*local_18 + (longlong)local_10 * 8);
      if (iVar3 == param_2) {
        return uVar2;
      }
      iVar3 = iVar3 + 1;
    }
  }
  return 0;
}



// ======== d3 callee-of 0x140d2a280  @ 140d1b280 ========

void FUN_140d1b280(longlong param_1)

{
  bool bVar1;
  longlong lVar2;
  longlong lVar3;
  
  if (*(longlong *)(param_1 + 0x628) != 0) {
    return;
  }
  lVar2 = FUN_142d1f150(param_1,0);
  *(longlong *)(param_1 + 0x628) = lVar2;
  if (lVar2 != 0) {
    lVar3 = FUN_141863300();
    if ((*(int *)(*(longlong *)(lVar2 + 0x10) + 0x90) < *(int *)(lVar3 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar2 + 0x10) + 0x88) + (longlong)*(int *)(lVar3 + 0x90) * 8)
        != lVar3 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) goto LAB_140d1b2e6;
  }
  lVar2 = 0;
LAB_140d1b2e6:
  *(longlong *)(param_1 + 0x640) = lVar2;
  return;
}



// ======== d3 callee-of 0x1416f3230  @ 141a59cd0 ========

undefined8 FUN_141a59cd0(longlong param_1)

{
  longlong *plVar1;
  int iVar2;
  bool bVar3;
  int iVar4;
  longlong lVar5;
  uint uVar6;
  undefined8 uVar7;
  uint *puVar8;
  uint local_78;
  uint uStack_74;
  uint uStack_70;
  uint uStack_6c;
  undefined8 local_68;
  uint uStack_60;
  int iStack_5c;
  undefined1 local_58 [16];
  longlong *local_48;
  undefined2 local_40;
  longlong *local_38;
  uint uStack_30;
  uint uStack_2c;
  uint *puStack_28;
  undefined1 auStack_20 [16];
  undefined8 local_10;
  
  if (*(int *)(param_1 + 0x238) == *(int *)(param_1 + 0x264)) {
    return 0;
  }
  uVar7 = 1;
  plVar1 = (longlong *)(param_1 + 0x230);
  iVar2 = *(int *)(param_1 + 600);
  puStack_28 = (uint *)(param_1 + 0x240);
  local_78 = 0;
  uStack_74 = 1;
  local_68._4_4_ = 0;
  uStack_60 = 0;
  if (iVar2 != 0) {
    puVar8 = *(uint **)(param_1 + 0x250);
    if (*(uint **)(param_1 + 0x250) == (uint *)0x0) {
      puVar8 = puStack_28;
    }
    uVar6 = *puVar8;
    local_78 = 0;
    uStack_60 = 0;
    while (uVar6 == 0) {
      lVar5 = (longlong)(int)local_78;
      uStack_60 = uStack_60 + 0x20;
      uVar6 = local_78 + 1;
      bVar3 = (int)((iVar2 + -1 >> 0x1f & 0x1fU) + iVar2 + -1) >> 5 <= (int)local_78;
      local_68._4_4_ = iVar2;
      local_78 = uVar6;
      if (bVar3) goto LAB_141a59dc8;
      uVar6 = puVar8[lVar5 + 1];
    }
    uStack_74 = -uVar6 & uVar6;
    iVar4 = 0x1f;
    if (uStack_74 != 0) {
      for (; uStack_74 >> iVar4 == 0; iVar4 = iVar4 + -1) {
      }
    }
    if (uStack_74 == 0) {
      iVar4 = 0x20;
    }
    else {
      iVar4 = 0x1f - iVar4;
    }
    local_68._4_4_ = (uStack_60 - iVar4) + 0x1f;
    if (iVar2 < local_68._4_4_) {
      local_68._4_4_ = iVar2;
    }
  }
LAB_141a59dc8:
  auStack_20._4_4_ = local_68._4_4_;
  auStack_20._0_4_ = 0xffffffff;
  auStack_20._8_4_ = uStack_60;
  auStack_20._12_4_ = iStack_5c;
  uStack_30 = local_78;
  uStack_2c = uStack_74;
  local_40 = 0;
  local_10._0_4_ = SUB84(plVar1,0);
  local_10._4_4_ = (undefined4)((ulonglong)plVar1 >> 0x20);
  uStack_70 = local_78;
  uStack_6c = uStack_74;
  uStack_60 = 0xffffffff;
  iStack_5c = local_68._4_4_;
  local_58._8_4_ = (undefined4)local_10;
  local_58._0_8_ = auStack_20._8_8_;
  local_58._12_4_ = local_10._4_4_;
  bVar3 = local_68._4_4_ < *(int *)(param_1 + 600);
  local_48 = plVar1;
  local_38 = plVar1;
  local_10 = plVar1;
  local_68 = puStack_28;
  if (bVar3) {
    do {
      if (*(char *)((longlong)iStack_5c * 0x70 + 0x50 + *plVar1) == '\0') {
        uVar7 = 0;
        break;
      }
      uStack_60 = uStack_60 & ~uStack_6c;
      FUN_1407e9ca0(&uStack_70);
    } while (iStack_5c < (int)local_68[6]);
    if (((char)local_40 != '\0') && (local_40._1_1_ != '\0')) {
      FUN_1415165e0(local_48,(int)local_48[1] - *(int *)((longlong)local_48 + 0x34),1);
    }
  }
  return uVar7;
}



// ======== d3 callee-of 0x1416f3810  @ 1429e4dc0 ========

void FUN_1429e4dc0(longlong param_1)

{
  byte bVar1;
  longlong lVar2;
  int iVar3;
  ulonglong uVar4;
  longlong *plVar5;
  ulonglong uVar6;
  
  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 0x40;
  uVar4 = 0;
  plVar5 = *(longlong **)(param_1 + 0x170);
  bVar1 = *(byte *)(param_1 + 0x1ac);
  uVar6 = (longlong)*(int *)(param_1 + 0x178) & 0x1fffffffffffffff;
  if (plVar5 + *(int *)(param_1 + 0x178) < plVar5) {
    uVar6 = uVar4;
  }
  if (uVar6 != 0) {
    do {
      lVar2 = *plVar5;
      iVar3 = (**(code **)(*(longlong *)(lVar2 + 0x28) + 0x38))(lVar2 + 0x28);
      if (iVar3 == 1) {
        FUN_1429e4c10(lVar2);
      }
      plVar5 = plVar5 + 1;
      uVar4 = uVar4 + 1;
    } while (uVar4 != uVar6);
    bVar1 = *(byte *)(param_1 + 0x1ac);
  }
  *(byte *)(param_1 + 0x1ac) = bVar1 & 0xbf;
  FUN_1429ef8d0(param_1);
  return;
}



// ======== d3 callee-of 0x1416f3810  @ 141df5850 ========

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



// ======== d3 callee-of 0x142dd9f90  @ 143135f20 ========

longlong FUN_143135f20(longlong param_1)

{
  uint uVar1;
  uint7 uVar2;
  
  uVar1 = *(uint *)(param_1 + 0x6e0);
  uVar2 = (uint7)(uint3)(uVar1 >> 8);
  if ((uVar1 < 7) && ((0x6aU >> (uVar1 & 0x1f) & 1) != 0)) {
    return CONCAT71(uVar2,1);
  }
  return (ulonglong)uVar2 << 8;
}



// ======== d3 callee-of 0x142dd9f90  @ 143133a90 ========

undefined8 FUN_143133a90(longlong param_1,char param_2,undefined1 param_3)

{
  longlong lVar1;
  bool bVar2;
  undefined8 uVar3;
  longlong lVar4;
  
  if (*(longlong *)(param_1 + 0x30) == 0) {
    return 0;
  }
  uVar3 = FUN_142c08180(*(longlong *)(param_1 + 0x30),param_3);
  if ((param_2 != '\0') && (0 < *(int *)(param_1 + 0x90))) {
    lVar1 = **(longlong **)(param_1 + 0x88);
    if (lVar1 != 0) {
      lVar4 = FUN_1432068b0();
      lVar1 = *(longlong *)(lVar1 + 0x10);
      if ((*(int *)(lVar1 + 0x90) < *(int *)(lVar4 + 0x90)) ||
         (*(longlong *)(*(longlong *)(lVar1 + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
          lVar4 + 0x88)) {
        bVar2 = false;
      }
      else {
        bVar2 = true;
      }
      if ((bVar2) && (lVar4 = *(longlong *)(**(longlong **)(param_1 + 0x88) + 0x120), lVar4 != 0)) {
        uVar3 = FUN_142c08180(lVar4,CONCAT71((int7)((ulonglong)lVar1 >> 8),1));
        return uVar3;
      }
    }
    return uVar3;
  }
  return uVar3;
}



// ======== d3 callee-of 0x142dd9f90  @ 142c950f0 ========

undefined8 * FUN_142c950f0(void)

{
  int iVar1;
  longlong *plVar2;
  char cVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  longlong lVar6;
  undefined8 *puVar7;
  undefined2 *puVar8;
  undefined2 *local_18;
  int local_10;
  
  puVar7 = DAT_145c6a2b0;
  if (DAT_145c6a2b0 == (undefined8 *)0x0) {
    puVar4 = (undefined8 *)FUN_140b40c10(0x40);
    puVar7 = (undefined8 *)0x0;
    if (puVar4 != (undefined8 *)0x0) {
      puVar4[1] = 0;
      *puVar4 = &PTR_FUN_144c03fa8;
      puVar4[2] = 0;
      puVar4[3] = 0;
      puVar4[4] = 1;
      *(undefined4 *)(puVar4 + 5) = 0x40a00000;
      puVar4[6] = 0;
      uVar5 = FUN_141e46970();
      cVar3 = FUN_141e977e0(uVar5,L"NoTextureStreaming");
      plVar2 = DAT_145c8a338;
      if (cVar3 != '\0') {
        FUN_141dbbf60(&local_18,&DAT_14391f7e8,0);
        puVar8 = &DAT_14391f7d0;
        if (local_10 != 0) {
          puVar8 = local_18;
        }
        (**(code **)(*plVar2 + 0x60))(plVar2,puVar8,0x7000000);
        if (local_18 != (undefined2 *)0x0) {
          FUN_141de9200();
        }
      }
      FUN_142c8a460(puVar4,1);
      lVar6 = FUN_140b40c10(0x120);
      if (lVar6 != 0) {
        puVar7 = (undefined8 *)FUN_142ba6330(lVar6);
      }
      puVar4[7] = puVar7;
      iVar1 = *(int *)(puVar4 + 3);
      *(int *)(puVar4 + 3) = iVar1 + 1;
      if (*(int *)((longlong)puVar4 + 0x1c) < iVar1 + 1) {
        FUN_1407f1510(puVar4 + 2,iVar1);
      }
      *(undefined8 **)(puVar4[2] + (longlong)iVar1 * 8) = puVar7;
      puVar7 = puVar4;
    }
  }
  DAT_145c6a2b0 = puVar7;
  return puVar7;
}



// ======== d3 callee-of 0x142dd9f90  @ 142b15b70 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_142b15b70(longlong param_1,longlong *param_2)

{
  longlong *plVar1;
  longlong lVar2;
  uint uVar3;
  ulonglong uVar4;
  int iVar5;
  int iVar6;
  longlong *plVar7;
  longlong *plVar8;
  undefined8 *puVar9;
  longlong lVar10;
  longlong *plVar11;
  int iVar12;
  ulonglong uVar13;
  ulonglong uVar14;
  undefined1 auStack_1f8 [32];
  undefined8 local_1d8 [24];
  undefined8 *local_118;
  undefined8 local_110;
  longlong local_108 [24];
  longlong *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_145909780 ^ (ulonglong)auStack_1f8;
  *(undefined4 *)(param_2 + 1) = 0;
  if (0x7fffffff < *(uint *)((longlong)param_2 + 0xc)) {
    FUN_140827c50(param_2,0);
  }
  if (*(longlong *)(param_1 + 0x168) != 0) {
    local_118 = (undefined8 *)0x0;
    local_110 = 1;
    local_48 = (longlong *)0x0;
    local_40 = 0;
    FUN_140bed440(local_1d8,0);
    puVar9 = local_1d8;
    if (local_118 != (undefined8 *)0x0) {
      puVar9 = local_118;
    }
    *puVar9 = *(undefined8 *)(param_1 + 0x168);
    uVar13 = local_110 & 0xffffffff;
    plVar11 = local_48;
    uVar3 = (uint)local_110;
    while (0 < (int)uVar3) {
      iVar6 = (int)uVar13;
      puVar9 = local_1d8;
      if (local_118 != (undefined8 *)0x0) {
        puVar9 = local_118;
      }
      iVar12 = iVar6 + -1;
      lVar2 = puVar9[iVar12];
      iVar5 = (iVar6 - iVar12) + -1;
      if (iVar5 != 0) {
        memmove(puVar9 + iVar12,puVar9 + iVar6,(longlong)iVar5 << 3);
        plVar11 = local_48;
        iVar6 = (uint)local_110;
      }
      uVar13 = local_40;
      local_110 = CONCAT44(local_110._4_4_,iVar6 - 1U);
      plVar8 = local_108;
      if (plVar11 != (longlong *)0x0) {
        plVar8 = plVar11;
      }
      plVar7 = plVar8 + (int)local_40;
      for (; plVar8 != plVar7; plVar8 = plVar8 + 1) {
        uVar3 = iVar6 - 1U;
        if (*plVar8 == lVar2) goto LAB_142b15d8f;
      }
      lVar10 = (longlong)(int)local_40;
      iVar6 = (int)local_40 + 1;
      local_40 = CONCAT44(local_40._4_4_,iVar6);
      if (local_40._4_4_ < iVar6) {
        FUN_140bed440(local_108,uVar13 & 0xffffffff);
        plVar11 = local_48;
      }
      uVar13 = 0;
      plVar8 = local_108;
      if (plVar11 != (longlong *)0x0) {
        plVar8 = plVar11;
      }
      plVar8[lVar10] = lVar2;
      lVar10 = *(longlong *)(lVar2 + 0xf0);
      plVar11 = local_48;
      if (lVar10 != 0) {
        if (lVar10 == param_1) {
          plVar8 = *(longlong **)(lVar2 + 0x120);
          uVar14 = (longlong)*(int *)(lVar2 + 0x128) & 0x1fffffffffffffff;
          if (plVar8 + *(int *)(lVar2 + 0x128) < plVar8) {
            uVar14 = uVar13;
          }
          if (uVar14 != 0) {
            do {
              uVar4 = local_110;
              lVar2 = *plVar8;
              if (lVar2 != 0) {
                plVar7 = local_108;
                if (plVar11 != (longlong *)0x0) {
                  plVar7 = plVar11;
                }
                plVar1 = plVar7 + (int)local_40;
                for (; plVar7 != plVar1; plVar7 = plVar7 + 1) {
                  if (*plVar7 == lVar2) goto LAB_142b15e9a;
                }
                lVar10 = (longlong)(int)(uint)local_110;
                iVar6 = (uint)local_110 + 1;
                local_110 = CONCAT44(local_110._4_4_,iVar6);
                if (local_110._4_4_ < iVar6) {
                  FUN_140bed440(local_1d8,uVar4 & 0xffffffff);
                }
                puVar9 = local_1d8;
                if (local_118 != (undefined8 *)0x0) {
                  puVar9 = local_118;
                }
                puVar9[lVar10] = lVar2;
                plVar11 = local_48;
              }
LAB_142b15e9a:
              plVar8 = plVar8 + 1;
              uVar13 = uVar13 + 1;
            } while (uVar13 != uVar14);
          }
        }
        else {
          plVar8 = (longlong *)*param_2;
          iVar6 = (int)param_2[1];
          for (plVar7 = plVar8; plVar7 != plVar8 + iVar6; plVar7 = plVar7 + 1) {
            if (*plVar7 == lVar10) {
              if ((int)((longlong)plVar7 - (longlong)plVar8 >> 3) != -1) goto LAB_142b15d8c;
              break;
            }
          }
          *(int *)(param_2 + 1) = iVar6 + 1;
          if (*(int *)((longlong)param_2 + 0xc) < iVar6 + 1) {
            FUN_1407f1510(param_2,iVar6);
          }
          *(longlong *)((longlong)iVar6 * 8 + *param_2) = lVar10;
          plVar11 = local_48;
        }
      }
LAB_142b15d8c:
      uVar3 = (uint)local_110;
LAB_142b15d8f:
      uVar13 = (ulonglong)uVar3;
    }
    if (plVar11 != (longlong *)0x0) {
      FUN_141de9200(plVar11);
    }
    if (local_118 != (undefined8 *)0x0) {
      FUN_141de9200();
    }
  }
  return;
}



// ======== d3 callee-of 0x142dd9f90  @ 14121ba40 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14121ba40(longlong param_1,longlong param_2,char param_3)

{
  uint uVar1;
  bool bVar2;
  int iVar3;
  longlong lVar4;
  longlong *plVar5;
  uint uVar6;
  undefined1 *_Src;
  ulonglong uVar7;
  longlong lVar8;
  longlong *plVar9;
  uint uVar10;
  uint *puVar11;
  int iVar12;
  longlong *plVar13;
  uint uVar14;
  ulonglong uVar15;
  undefined1 auStack_1b8 [32];
  char local_198;
  longlong *local_190;
  uint local_188;
  uint uStack_184;
  uint *puStack_180;
  undefined4 local_178;
  uint uStack_174;
  uint uStack_170;
  undefined4 uStack_16c;
  longlong *local_168;
  uint local_160;
  uint local_15c;
  longlong *local_158;
  uint uStack_150;
  uint uStack_14c;
  uint *local_148;
  uint uStack_140;
  uint uStack_13c;
  undefined8 local_138;
  longlong *local_130;
  undefined8 uStack_128;
  uint *puStack_120;
  uint uStack_118;
  uint uStack_114;
  undefined8 uStack_110;
  undefined1 local_108 [192];
  undefined1 *local_48;
  ulonglong local_40;
  ulonglong local_38;
  
  local_38 = DAT_145909780 ^ (ulonglong)auStack_1b8;
  plVar13 = (longlong *)0x0;
  *(undefined4 *)(param_2 + 200) = 0;
  local_198 = param_3;
  if (0x7fffffff < *(uint *)(param_2 + 0xcc)) {
    FUN_140bed4d0(param_2,0);
  }
  local_190 = (longlong *)(param_1 + 0x2c0);
  local_168 = (longlong *)0x0;
  uVar1 = *(uint *)(param_1 + 0x2e8);
  puStack_180 = (uint *)(param_1 + 0x2d0);
  local_15c = 0;
  local_188 = 0;
  uStack_184 = 1;
  uStack_174 = 0;
  uStack_170 = 0;
  if (uVar1 != 0) {
    puVar11 = *(uint **)(param_1 + 0x2e0);
    if (*(uint **)(param_1 + 0x2e0) == (uint *)0x0) {
      puVar11 = puStack_180;
    }
    uVar14 = *puVar11;
    plVar5 = plVar13;
    plVar9 = plVar13;
    while (uVar14 == 0) {
      iVar3 = (int)plVar5;
      uStack_170 = (int)plVar9 + 0x20;
      plVar9 = (longlong *)(ulonglong)uStack_170;
      local_188 = iVar3 + 1;
      plVar5 = (longlong *)(ulonglong)local_188;
      uStack_174 = uVar1;
      if ((int)((uVar1 - 1) + ((int)(uVar1 - 1) >> 0x1f & 0x1fU)) >> 5 <= iVar3) goto LAB_14121bb7a;
      uVar14 = puVar11[(longlong)iVar3 + 1];
    }
    uStack_184 = uVar14 - 1 & uVar14 ^ uVar14;
    iVar3 = 0x1f;
    if (uStack_184 != 0) {
      for (; uStack_184 >> iVar3 == 0; iVar3 = iVar3 + -1) {
      }
    }
    if (uStack_184 == 0) {
      iVar3 = 0x20;
    }
    else {
      iVar3 = 0x1f - iVar3;
    }
    uStack_174 = (uStack_170 - iVar3) + 0x1f;
    if ((int)uVar1 < (int)uStack_174) {
      uStack_174 = uVar1;
    }
  }
LAB_14121bb7a:
  local_178 = 0xffffffff;
  uVar14 = *(uint *)(param_1 + 0x2e8);
  uStack_128 = CONCAT44(uStack_184,local_188);
  uStack_140 = 0xffffffff;
  uStack_13c = uStack_174;
  local_138 = CONCAT44(uStack_16c,uStack_170);
  uVar10 = uVar14 & 0xffffffe0;
  uStack_150 = local_188;
  uStack_14c = uStack_184;
  plVar5 = plVar13;
  plVar9 = plVar13;
  local_148 = puStack_180;
  local_130 = local_190;
  puStack_120 = puStack_180;
  uStack_110 = local_138;
  local_158 = local_190;
  uStack_118 = uStack_140;
  uStack_114 = uStack_13c;
  if (uVar14 != uVar1) {
    puVar11 = *(uint **)(param_1 + 0x2e0);
    if (*(uint **)(param_1 + 0x2e0) == (uint *)0x0) {
      puVar11 = puStack_180;
    }
    uVar6 = puVar11[(int)uVar14 >> 5] & -1 << ((byte)uVar14 & 0x1f);
    iVar3 = (int)uVar14 >> 5;
    while (uVar6 == 0) {
      uVar10 = uVar10 + 0x20;
      uVar14 = uVar1;
      if ((int)((uVar1 - 1) + ((int)(uVar1 - 1) >> 0x1f & 0x1fU)) >> 5 <= iVar3) goto LAB_14121bc45;
      uVar6 = puVar11[(longlong)iVar3 + 1];
      iVar3 = iVar3 + 1;
    }
    uVar6 = uVar6 - 1 & uVar6 ^ uVar6;
    iVar3 = 0x1f;
    if (uVar6 != 0) {
      for (; uVar6 >> iVar3 == 0; iVar3 = iVar3 + -1) {
      }
    }
    if (uVar6 == 0) {
      iVar3 = 0x20;
    }
    else {
      iVar3 = 0x1f - iVar3;
    }
    uVar14 = (uVar10 - iVar3) + 0x1f;
    plVar5 = (longlong *)0x0;
    plVar9 = (longlong *)0x0;
    if ((int)uVar1 < (int)uVar14) {
      uVar14 = uVar1;
    }
  }
LAB_14121bc45:
  while( true ) {
    if ((uStack_13c == uVar14) && (local_148 == puStack_180)) {
      bVar2 = true;
    }
    else {
      bVar2 = false;
    }
    if ((bVar2) && (local_158 == local_190)) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    iVar3 = (int)plVar9;
    if (!bVar2) break;
    lVar8 = *(longlong *)(*local_158 + (longlong)(int)uStack_13c * 0x10);
    if (lVar8 != 0) {
      lVar4 = FUN_143255d90();
      if ((*(int *)(*(longlong *)(lVar8 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
         (*(longlong *)
           (*(longlong *)(*(longlong *)(lVar8 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8
           ) != lVar4 + 0x88)) {
        bVar2 = false;
      }
      else {
        bVar2 = true;
      }
      if (bVar2) {
        iVar12 = *(int *)(param_2 + 200);
        *(int *)(param_2 + 200) = iVar12 + 1;
        if (*(int *)(param_2 + 0xcc) < iVar12 + 1) {
          FUN_140bed440(param_2,iVar12);
        }
        lVar4 = *(longlong *)(param_2 + 0xc0);
        if (*(longlong *)(param_2 + 0xc0) == 0) {
          lVar4 = param_2;
        }
        *(longlong *)(lVar4 + (longlong)iVar12 * 8) = lVar8;
      }
    }
    if ((local_198 != '\0') && (lVar8 != 0)) {
      lVar4 = FUN_143183a90();
      if ((*(int *)(*(longlong *)(lVar8 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
         (*(longlong *)
           (*(longlong *)(*(longlong *)(lVar8 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8
           ) != lVar4 + 0x88)) {
        bVar2 = false;
      }
      else {
        bVar2 = true;
      }
      if (bVar2) {
        local_160 = iVar3 + 1;
        if ((int)plVar5 < (int)local_160) {
          FUN_1407f1510(&local_168,plVar9);
          plVar5 = (longlong *)(ulonglong)local_15c;
          plVar13 = local_168;
        }
        plVar9 = (longlong *)(ulonglong)local_160;
        plVar13[iVar3] = lVar8;
      }
    }
    uStack_140 = uStack_140 & ~uStack_14c;
    FUN_1407e9ca0(&uStack_150);
    local_158 = local_158;
  }
  if (local_198 != '\0') {
    uVar7 = 0;
    local_48 = (undefined1 *)0x0;
    local_40 = 0;
    uVar15 = (ulonglong)((longlong)(plVar13 + iVar3) + (7 - (longlong)plVar13)) >> 3;
    if (plVar13 + iVar3 < plVar13) {
      uVar15 = uVar7;
    }
    plVar5 = plVar13;
    if (uVar15 != 0) {
      do {
        if (*(longlong *)(*plVar5 + 0x250) != 0) {
          FUN_14121ba40(*(longlong *)(*plVar5 + 0x250),local_108,1);
          iVar3 = (int)local_40;
          lVar8 = (longlong)(int)local_40;
          if ((int)local_40 != 0) {
            iVar12 = *(int *)(param_2 + 200);
            if (*(int *)(param_2 + 0xcc) < iVar12 + (int)local_40) {
              FUN_140bed4d0(param_2);
              iVar12 = *(int *)(param_2 + 200);
            }
            _Src = local_108;
            if (local_48 != (undefined1 *)0x0) {
              _Src = local_48;
            }
            lVar4 = *(longlong *)(param_2 + 0xc0);
            if (*(longlong *)(param_2 + 0xc0) == 0) {
              lVar4 = param_2;
            }
            memmove((void *)(lVar4 + (longlong)iVar12 * 8),_Src,lVar8 << 3);
            *(int *)(param_2 + 200) = *(int *)(param_2 + 200) + iVar3;
            local_40 = local_40 & 0xffffffff00000000;
          }
        }
        uVar7 = uVar7 + 1;
        plVar5 = plVar5 + 1;
      } while (uVar7 != uVar15);
      if (local_48 != (undefined1 *)0x0) {
        FUN_141de9200();
      }
    }
  }
  if (plVar13 != (longlong *)0x0) {
    FUN_141de9200(plVar13);
  }
  return;
}



// ======== d3 callee-of 0x142dd9f90  @ 142b0fd90 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_142b0fd90(undefined8 param_1,longlong param_2,undefined8 param_3)

{
  longlong *plVar1;
  longlong *plVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  undefined1 auStack_118 [32];
  longlong local_f8 [24];
  longlong *local_38;
  undefined8 local_30;
  ulonglong local_28;
  
  if (param_2 != 0) {
    local_28 = DAT_145909780 ^ (ulonglong)auStack_118;
    uVar3 = 0;
    local_38 = (longlong *)0x0;
    local_30 = 0;
    FUN_14121ba40(param_1,local_f8,0);
    plVar2 = local_f8;
    if (local_38 != (longlong *)0x0) {
      plVar2 = local_38;
    }
    uVar4 = (longlong)(int)local_30 & 0x1fffffffffffffff;
    if (plVar2 + (int)local_30 < plVar2) {
      uVar4 = uVar3;
    }
    if (uVar4 != 0) {
      do {
        plVar1 = (longlong *)*plVar2;
        if (plVar1[0x22] == param_2) {
          (**(code **)(*plVar1 + 1000))(plVar1,param_3);
        }
        plVar2 = plVar2 + 1;
        uVar3 = uVar3 + 1;
      } while (uVar3 != uVar4);
    }
    if (local_38 != (longlong *)0x0) {
      FUN_141de9200();
    }
  }
  return;
}



// ======== d3 callee-of 0x142dd9f90  @ 141de9200 ========

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



// ======== d3 callee-of 0x142dd9f90  @ 142b0fe90 ========

void FUN_142b0fe90(longlong param_1)

{
  if (*(longlong **)(param_1 + 0x168) != (longlong *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x000142b0fe9f. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(**(longlong **)(param_1 + 0x168) + 1000))();
    return;
  }
  return;
}



// ======== d3 callee-of 0x142dd9f90  @ 142b0e120 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_142b0e120(undefined8 param_1)

{
  longlong *plVar1;
  int iVar2;
  longlong lVar3;
  undefined1 *puVar4;
  uint uVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  longlong *plVar10;
  undefined1 auStack_2e8 [32];
  undefined1 local_2c8 [432];
  undefined1 *local_118;
  uint local_110;
  uint local_10c;
  longlong local_108 [24];
  longlong *local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_38 = DAT_145909780 ^ (ulonglong)auStack_2e8;
  puVar9 = (undefined1 *)0x0;
  local_48 = (longlong *)0x0;
  local_40 = 0;
  FUN_140bd47c0(param_1,local_108,0);
  local_118 = (undefined1 *)0x0;
  plVar10 = local_108;
  if (local_48 != (longlong *)0x0) {
    plVar10 = local_48;
  }
  local_110 = 0;
  local_10c = 0;
  plVar1 = plVar10 + (int)local_40;
  puVar4 = puVar9;
  puVar8 = puVar9;
  if (plVar10 != plVar1) {
    do {
      lVar3 = *plVar10;
      local_110 = 0;
      puVar6 = puVar9;
      if (((int)puVar4 < 0) && (puVar6 = (undefined1 *)0x0, (int)puVar4 != 0)) {
        FUN_142b24250(local_2c8);
        puVar4 = (undefined1 *)(ulonglong)local_10c;
        puVar6 = (undefined1 *)(ulonglong)local_110;
        puVar8 = local_118;
      }
      uVar5 = (uint)puVar6;
      iVar2 = *(int *)(lVar3 + 0x2c0);
      if (iVar2 != 0) {
        if ((int)puVar4 < (int)(uVar5 + iVar2)) {
          FUN_142b24250(local_2c8,uVar5 + iVar2);
          puVar8 = local_118;
          uVar5 = local_110;
        }
        puVar4 = local_2c8;
        if (puVar8 != (undefined1 *)0x0) {
          puVar4 = puVar8;
        }
        memcpy(puVar4 + (longlong)(int)uVar5 * 0x90,*(void **)(lVar3 + 0x2b8),(longlong)iVar2 * 0x90
              );
        puVar4 = (undefined1 *)(ulonglong)local_10c;
        uVar5 = local_110 + iVar2;
        puVar8 = local_118;
        local_110 = uVar5;
      }
      puVar6 = local_2c8;
      if (puVar8 != (undefined1 *)0x0) {
        puVar6 = puVar8;
      }
      puVar7 = puVar6 + (longlong)(int)uVar5 * 0x90;
      if (puVar6 != puVar7) {
        do {
          FUN_142c40950(lVar3,puVar6,1,0);
          puVar6 = puVar6 + 0x90;
        } while (puVar6 != puVar7);
        puVar4 = (undefined1 *)(ulonglong)local_10c;
        puVar8 = local_118;
      }
      plVar10 = plVar10 + 1;
    } while (plVar10 != plVar1);
    if (puVar8 != (undefined1 *)0x0) {
      FUN_141de9200(puVar8);
    }
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200(local_48);
  }
  return;
}



// ======== d3 callee-of 0x142dd9f90  @ 142b1a260 ========

void FUN_142b1a260(undefined8 param_1)

{
  undefined8 uVar1;
  
  uVar1 = FUN_143200a40();
  FUN_142042c00(param_1,uVar1);
  return;
}



// ======== d3 callee-of 0x142dd9f90  @ 142db4110 ========

void FUN_142db4110(longlong param_1,longlong *param_2)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  char cVar4;
  longlong lVar5;
  undefined8 local_48;
  undefined4 local_40;
  undefined4 local_24;
  undefined4 local_20;
  longlong local_18;
  
  if ((((param_2 != (longlong *)0x0) &&
       (cVar4 = (**(code **)(*param_2 + 0x1c0))(param_2), cVar4 != '\0')) &&
      (cVar4 = (**(code **)(*param_2 + 0x1c8))(param_2), cVar4 != '\0')) &&
     (*(char *)((longlong)param_2 + 0x95) != '\0')) {
    lVar2 = param_2[3];
    lVar5 = param_2[0x2d];
    local_24 = 0xffffffff;
    local_20 = 0;
    if (lVar5 == 0) {
      local_48 = DAT_145a8af10;
      local_40 = DAT_145a8af18;
    }
    else {
      local_48 = *(undefined8 *)(lVar5 + 0x1a0);
      local_40 = *(undefined4 *)(lVar5 + 0x1a8);
    }
    FUN_1420575e0(&local_24,param_2[4]);
    local_18 = param_2[2];
    iVar1 = *(int *)(param_1 + 0x280);
    lVar5 = (longlong)iVar1;
    *(int *)(param_1 + 0x280) = iVar1 + 1;
    if (*(int *)(param_1 + 0x284) < iVar1 + 1) {
      FUN_1408271e0((longlong *)(param_1 + 0x278),iVar1);
    }
    lVar3 = *(longlong *)(param_1 + 0x278);
    *(longlong *)(lVar3 + lVar5 * 0x28) = lVar2;
    *(undefined8 *)(lVar3 + 8 + lVar5 * 0x28) = local_48;
    *(undefined4 *)(lVar3 + 0x10 + lVar5 * 0x28) = local_40;
    *(undefined4 *)(lVar3 + 0x14 + lVar5 * 0x28) = local_24;
    *(undefined4 *)(lVar3 + 0x18 + lVar5 * 0x28) = local_20;
    *(longlong *)(lVar3 + 0x20 + lVar5 * 0x28) = local_18;
  }
  return;
}



// ======== d3 callee-of 0x142dd9f90  @ 1430daee0 ========

longlong FUN_1430daee0(longlong param_1,longlong param_2)

{
  longlong *plVar1;
  longlong *plVar2;
  
  plVar2 = *(longlong **)(param_1 + 0xbe8);
  plVar1 = plVar2 + *(int *)(param_1 + 0xbf0);
  while( true ) {
    if (plVar2 == plVar1) {
      return 0;
    }
    if (*(longlong *)(*plVar2 + 0x290) == param_2) break;
    plVar2 = plVar2 + 1;
  }
  return *plVar2;
}



// ======== d3 callee-of 0x142dd9f90  @ 1430d8da0 ========

longlong FUN_1430d8da0(longlong param_1,longlong param_2,longlong param_3)

{
  longlong lVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong lVar4;
  longlong *plVar5;
  
  plVar2 = *(longlong **)(param_1 + 0xbe8);
  plVar5 = plVar2 + *(int *)(param_1 + 0xbf0);
  for (; plVar2 != plVar5; plVar2 = plVar2 + 1) {
    lVar3 = *plVar2;
    if (*(longlong *)(lVar3 + 0x290) == param_2) goto LAB_1430d8de9;
  }
  lVar3 = FUN_1430cea00(DAT_145c8a740,0);
LAB_1430d8de9:
  if (0 < *(int *)(lVar3 + 0x250)) {
    plVar5 = *(longlong **)(lVar3 + 0x248);
    lVar4 = 0;
    do {
      lVar1 = *plVar5;
      if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x178) == param_3)) {
        return lVar1;
      }
      lVar4 = lVar4 + 1;
      plVar5 = plVar5 + 2;
    } while (lVar4 < *(int *)(lVar3 + 0x250));
  }
  return 0;
}



// ======== d3 callee-of 0x142dd9f90  @ 14313b240 ========

void FUN_14313b240(longlong param_1,longlong param_2,char param_3)

{
  longlong *plVar1;
  longlong lVar2;
  int iVar3;
  longlong *plVar4;
  longlong *plVar5;
  
  plVar1 = (longlong *)FUN_142b1a260(param_2);
  plVar5 = (longlong *)plVar1[0x14];
  plVar4 = plVar5;
  do {
    if (plVar4 == plVar5 + (int)plVar1[0x15]) {
LAB_14313b326:
      if (((param_2 != 0) && (DAT_145c8a740 != 0)) &&
         ((param_1 != 0 && (lVar2 = FUN_1430daee0(DAT_145c8a740,param_1), lVar2 != 0)))) {
        plVar5 = *(longlong **)(lVar2 + 0x248);
        plVar4 = plVar5 + (longlong)*(int *)(lVar2 + 0x250) * 2;
        for (; plVar5 != plVar4; plVar5 = plVar5 + 2) {
          if (*plVar5 != 0) {
            FUN_142e59d20(*plVar5,param_2);
          }
        }
      }
      return;
    }
    if (*plVar4 == param_2) {
      iVar3 = (int)((longlong)plVar4 - (longlong)plVar5 >> 3);
      if (iVar3 != -1) {
        if ((((param_3 != '\0') && (DAT_145a8c218 != 0)) && (plVar1 != (longlong *)0x0)) &&
           ((*(uint *)(plVar1 + 1) >> 3 & 1) != 0)) {
          (**(code **)(*plVar1 + 0x68))(plVar1,0);
          (**(code **)(*(longlong *)plVar1[0x19] + 0x68))((longlong *)plVar1[0x19],0);
        }
        if ((6 < *(uint *)(param_1 + 0x6e0)) ||
           ((0x6aU >> (*(uint *)(param_1 + 0x6e0) & 0x1f) & 1) == 0)) {
          plVar5 = *(longlong **)(plVar1[0x14] + (longlong)iVar3 * 8);
          (**(code **)(*plVar5 + 0x68))(plVar5,1);
        }
        *(undefined8 *)(plVar1[0x14] + (longlong)iVar3 * 8) = 0;
      }
      goto LAB_14313b326;
    }
    plVar4 = plVar4 + 1;
  } while( true );
}



// ======== d3 callee-of 0x142dd9f90  @ 142046040 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_142046040(longlong param_1)

{
  longlong *plVar1;
  bool bVar2;
  int iVar3;
  char cVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  byte bVar8;
  
  if ((*(uint *)(param_1 + 8) >> 6 & 1) == 0) {
    lVar6 = *(longlong *)(param_1 + 0x20);
    while (lVar7 = lVar6, lVar7 != 0) {
      param_1 = lVar7;
      lVar6 = *(longlong *)(lVar7 + 0x20);
    }
    if (DAT_145a98235 == '\0') {
      return 0;
    }
    bVar8 = *(byte *)(param_1 + 0x28) & 1;
    if (bVar8 == 0) {
      FUN_142019350(param_1,1);
    }
    bVar2 = false;
    DAT_1456dff4c = DAT_1456dff4c + 1;
    lVar6 = (longlong)(DAT_1456dff40 + -1);
    if (-1 < DAT_1456dff40 + -1) {
      lVar7 = lVar6 << 4;
      bVar2 = false;
      do {
        if (((*(int *)(lVar7 + 8 + DAT_1456dff38) == 0) ||
            (plVar1 = *(longlong **)(lVar7 + DAT_1456dff38), plVar1 == (longlong *)0x0)) ||
           (cVar4 = (**(code **)(*plVar1 + 0x48))(plVar1,param_1,bVar8), cVar4 == '\0')) {
          bVar2 = true;
        }
        lVar7 = lVar7 + -0x10;
        lVar6 = lVar6 + -1;
      } while (-1 < lVar6);
    }
    iVar3 = DAT_1456dff40;
    DAT_1456dff4c = DAT_1456dff4c + -1;
    if ((bVar2) && (DAT_1456dff4c < 1)) {
      iVar5 = 0;
      if (0 < DAT_1456dff40) {
        lVar6 = 0;
        do {
          if (((*(int *)(lVar6 + 8 + DAT_1456dff38) == 0) ||
              (*(longlong **)(lVar6 + DAT_1456dff38) == (longlong *)0x0)) ||
             (cVar4 = (**(code **)(**(longlong **)(lVar6 + DAT_1456dff38) + 0x18))(), cVar4 != '\0')
             ) {
            FUN_1407f1040(&DAT_1456dff38,iVar5,1,1);
          }
          else {
            iVar5 = iVar5 + 1;
            lVar6 = lVar6 + 0x10;
          }
        } while (iVar5 < DAT_1456dff40);
      }
      _DAT_1456dff48 = DAT_1456dff40 * 2;
      if (DAT_1456dff40 * 2 < 3) {
        _DAT_1456dff48 = 2;
      }
      if ((_DAT_1456dff48 < iVar3) && (DAT_1456dff44 != DAT_1456dff40)) {
        FUN_1407f1a80(&DAT_1456dff38);
      }
    }
  }
  return 1;
}



// ======== d3 callee-of 0x142dd9f90  @ 142b22500 ========

void FUN_142b22500(longlong *param_1,byte param_2,char param_3)

{
  uint *puVar1;
  longlong *plVar2;
  uint uVar3;
  uint uVar4;
  bool bVar5;
  char cVar6;
  longlong lVar7;
  int iVar8;
  uint uVar9;
  uint *puVar10;
  uint uVar11;
  undefined1 local_res20 [8];
  undefined8 local_98;
  uint *puStack_90;
  uint local_88;
  uint uStack_84;
  int iStack_80;
  undefined4 uStack_7c;
  longlong *local_78;
  int iStack_70;
  uint uStack_6c;
  uint *local_68;
  uint uStack_60;
  uint uStack_5c;
  undefined8 local_58;
  longlong *local_50;
  int iStack_48;
  uint uStack_44;
  uint *puStack_40;
  uint uStack_38;
  uint uStack_34;
  undefined8 uStack_30;
  
  cVar6 = FUN_1420444f0(param_1,0x30);
  if (cVar6 == '\0') {
    if ((*(byte *)((longlong)param_1 + 0x93) >> 5 & 1) != param_2) {
      local_res20[0] = 0;
      puStack_90 = (uint *)local_res20;
      local_98 = FUN_142b0ce60;
      lVar7 = FUN_141de9cd0(&local_98,&DAT_145755848);
      (**(code **)(*param_1 + 0x518))(param_1,param_2);
      *(byte *)((longlong)param_1 + 0x93) =
           *(byte *)((longlong)param_1 + 0x93) & 0xdf | param_2 << 5;
      *(undefined8 *)(lVar7 + 0x10) = 0;
    }
    if (param_3 != '\0') {
      plVar2 = param_1 + 0x58;
      uVar3 = *(uint *)(param_1 + 0x5d);
      puVar1 = (uint *)(param_1 + 0x5a);
      local_98._0_4_ = 0;
      local_98._4_4_ = 1;
      uStack_84 = 0;
      iStack_80 = 0;
      if (uVar3 != 0) {
        puVar10 = (uint *)param_1[0x5c];
        if ((uint *)param_1[0x5c] == (uint *)0x0) {
          puVar10 = puVar1;
        }
        uVar4 = *puVar10;
        iStack_80 = 0;
        local_98._0_4_ = 0;
        while (uVar4 == 0) {
          lVar7 = (longlong)(int)local_98;
          iStack_80 = iStack_80 + 0x20;
          iVar8 = (int)local_98 + 1;
          bVar5 = (int)((uVar3 - 1) + ((int)(uVar3 - 1) >> 0x1f & 0x1fU)) >> 5 <= (int)local_98;
          uStack_84 = uVar3;
          local_98._0_4_ = iVar8;
          if (bVar5) goto LAB_142b22675;
          uVar4 = puVar10[lVar7 + 1];
        }
        local_98._4_4_ = -uVar4 & uVar4;
        iVar8 = 0x1f;
        if (local_98._4_4_ != 0) {
          for (; local_98._4_4_ >> iVar8 == 0; iVar8 = iVar8 + -1) {
          }
        }
        if (local_98._4_4_ == 0) {
          iVar8 = 0x20;
        }
        else {
          iVar8 = 0x1f - iVar8;
        }
        uStack_84 = (iStack_80 - iVar8) + 0x1f;
        if ((int)uVar3 < (int)uStack_84) {
          uStack_84 = uVar3;
        }
      }
LAB_142b22675:
      uVar4 = *(uint *)(param_1 + 0x5d);
      uStack_60 = 0xffffffff;
      uStack_5c = uStack_84;
      local_58 = CONCAT44(uStack_7c,iStack_80);
      local_88 = -1 << ((byte)uVar4 & 0x1f);
      iStack_48 = (int)local_98;
      uStack_44 = local_98._4_4_;
      uVar11 = uVar4 & 0xffffffe0;
      iStack_70 = (int)local_98;
      uStack_6c = local_98._4_4_;
      puStack_90 = puVar1;
      uStack_84 = uVar4;
      local_68 = puVar1;
      local_50 = plVar2;
      puStack_40 = puVar1;
      uStack_30 = local_58;
      local_78 = plVar2;
      uStack_38 = uStack_60;
      uStack_34 = uStack_5c;
      if (uVar4 != uVar3) {
        puVar10 = (uint *)param_1[0x5c];
        if ((uint *)param_1[0x5c] == (uint *)0x0) {
          puVar10 = puVar1;
        }
        uVar9 = puVar10[(int)uVar4 >> 5] & local_88;
        iVar8 = (int)uVar4 >> 5;
        while (uVar9 == 0) {
          uVar11 = uVar11 + 0x20;
          uStack_84 = uVar3;
          uVar4 = uVar3;
          if ((int)((uVar3 - 1) + ((int)(uVar3 - 1) >> 0x1f & 0x1fU)) >> 5 <= iVar8)
          goto LAB_142b22760;
          uVar9 = puVar10[(longlong)iVar8 + 1];
          local_88 = 0xffffffff;
          iVar8 = iVar8 + 1;
        }
        uVar9 = -uVar9 & uVar9;
        iVar8 = 0x1f;
        if (uVar9 != 0) {
          for (; uVar9 >> iVar8 == 0; iVar8 = iVar8 + -1) {
          }
        }
        if (uVar9 == 0) {
          iVar8 = 0x20;
        }
        else {
          iVar8 = 0x1f - iVar8;
        }
        uStack_84 = (uVar11 - iVar8) + 0x1f;
        uVar4 = uStack_84;
        if ((int)uVar3 < (int)uStack_84) {
          uStack_84 = uVar3;
          uVar4 = uVar3;
        }
      }
LAB_142b22760:
      while (((uStack_5c != uVar4 || (local_68 != puVar1)) || (local_78 != plVar2))) {
        lVar7 = *(longlong *)(*local_78 + (longlong)(int)uStack_5c * 0x10);
        if (lVar7 != 0) {
          FUN_142c22390(lVar7,param_2);
        }
        uStack_60 = uStack_60 & ~uStack_6c;
        FUN_1407e9ca0(&iStack_70);
      }
    }
  }
  return;
}



// ======== d3 callee-of 0x142dd9f90  @ 143820a10 ========

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



// ======== d3 callee-of 0x1416d15e0  @ 141eca8b0 ========

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



// ======== d3 callee-of 0x1416d15e0  @ 1416fbfb0 ========

void FUN_1416fbfb0(longlong param_1,longlong *param_2)

{
  longlong *plVar1;
  int *piVar2;
  int iVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined1 local_88 [8];
  longlong *local_80;
  undefined1 local_70 [8];
  longlong local_68;
  longlong local_58;
  longlong local_48;
  longlong local_38;
  undefined1 local_28 [32];
  
  if (*(longlong *)(param_1 + 0x3f0) != 0) {
    lVar4 = FUN_1411fd400(local_70);
    FUN_141e24190(local_88,lVar4 + 0x38);
    if (local_38 != 0) {
      FUN_141de9200();
    }
    if (local_48 != 0) {
      FUN_141de9200();
    }
    if (local_58 != 0) {
      FUN_141de9200();
    }
    if (local_68 != 0) {
      FUN_141de9200();
    }
    uVar5 = FUN_141e08fe0(local_28,local_88);
    FUN_141a87ad0(*(undefined8 *)(param_1 + 0x3f0),uVar5);
    FUN_141a54c00(*(undefined8 *)(param_1 + 0x3f0));
    if (local_80 != (longlong *)0x0) {
      LOCK();
      plVar1 = local_80 + 1;
      lVar4 = *plVar1;
      *(int *)plVar1 = (int)*plVar1 + -1;
      UNLOCK();
      if ((int)lVar4 == 1) {
        (**(code **)*local_80)(local_80);
        LOCK();
        piVar2 = (int *)((longlong)local_80 + 0xc);
        iVar3 = *piVar2;
        *piVar2 = *piVar2 + -1;
        UNLOCK();
        if (iVar3 == 1) {
          (**(code **)(*local_80 + 8))(local_80,1);
        }
      }
    }
  }
  if (*param_2 != 0) {
    FUN_141de9200();
  }
  return;
}



// ======== d3 callee-of 0x1416d15e0  @ 1416fb4f0 ========

void FUN_1416fb4f0(longlong param_1,longlong *param_2)

{
  int *piVar1;
  longlong *plVar2;
  undefined1 uVar3;
  longlong lVar4;
  undefined8 uVar5;
  int iVar6;
  undefined1 local_88 [8];
  longlong *local_80;
  undefined1 local_70 [8];
  longlong local_68;
  longlong local_58;
  longlong local_48;
  longlong local_38;
  undefined1 local_28 [32];
  
  plVar2 = *(longlong **)(param_1 + 0x3f8);
  if (plVar2 != (longlong *)0x0) {
    iVar6 = (int)param_2[1] + -1;
    if ((int)param_2[1] == 0) {
      iVar6 = 0;
    }
    uVar3 = 1;
    if (0 < iVar6) {
      uVar3 = 3;
    }
    (**(code **)(*plVar2 + 0x250))(plVar2,uVar3);
    lVar4 = FUN_1411fd400(local_70,param_2);
    FUN_141e24190(local_88,lVar4 + 0x38);
    if (local_38 != 0) {
      FUN_141de9200();
    }
    if (local_48 != 0) {
      FUN_141de9200();
    }
    if (local_58 != 0) {
      FUN_141de9200();
    }
    if (local_68 != 0) {
      FUN_141de9200();
    }
    uVar5 = FUN_141e08fe0(local_28,local_88);
    FUN_141a87ad0(*(undefined8 *)(param_1 + 0x3f8),uVar5);
    FUN_141a54c00(*(undefined8 *)(param_1 + 0x3f8));
    if (local_80 != (longlong *)0x0) {
      LOCK();
      plVar2 = local_80 + 1;
      lVar4 = *plVar2;
      *(int *)plVar2 = (int)*plVar2 + -1;
      UNLOCK();
      if ((int)lVar4 == 1) {
        (**(code **)*local_80)(local_80);
        LOCK();
        piVar1 = (int *)((longlong)local_80 + 0xc);
        iVar6 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar6 == 1) {
          (**(code **)(*local_80 + 8))(local_80,1);
        }
      }
    }
  }
  if (*param_2 != 0) {
    FUN_141de9200();
  }
  return;
}



// ======== d3 callee-of 0x1416d15e0  @ 1416fae30 ========

void FUN_1416fae30(longlong param_1,undefined4 param_2,int param_3)

{
  longlong *plVar1;
  void *_Src;
  undefined8 uVar2;
  longlong lVar3;
  undefined2 *puVar4;
  void *local_38;
  int local_30;
  undefined4 local_2c;
  undefined2 *local_28;
  int local_20;
  
  plVar1 = *(longlong **)(param_1 + 0x400);
  if (plVar1 != (longlong *)0x0) {
    if (param_3 == 1) {
                    /* WARNING: Could not recover jumptable at 0x0001416fae76. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(*plVar1 + 0x250))(plVar1,1);
      return;
    }
    local_38 = (void *)0x0;
    local_30 = *(int *)(DAT_14598a548 + 0x28);
    lVar3 = (longlong)local_30;
    _Src = *(void **)(DAT_14598a548 + 0x20);
    if (local_30 == 0) {
      local_2c = 0;
    }
    else {
      FUN_1407f1420(&local_38,local_30,0);
      memcpy(local_38,_Src,lVar3 * 2);
    }
    FUN_141dbbf60(&local_28,&DAT_143f0e1fc,param_2);
    puVar4 = &DAT_14391f7d0;
    if (local_20 != 0) {
      puVar4 = local_28;
    }
    FUN_141dbe400(&local_38,L"<var 1>",puVar4,1);
    if (local_28 != (undefined2 *)0x0) {
      FUN_141de9200();
    }
    FUN_141dbbf60(&local_28,&DAT_143f0e218,param_3);
    puVar4 = &DAT_14391f7d0;
    if (local_20 != 0) {
      puVar4 = local_28;
    }
    FUN_141dbe400(&local_38,L"<var 2>",puVar4,1);
    if (local_28 != (undefined2 *)0x0) {
      FUN_141de9200();
    }
    uVar2 = FUN_141e241b0(&local_28,&local_38);
    FUN_141a87ad0(*(undefined8 *)(param_1 + 0x400),uVar2);
    FUN_141a54c00(*(undefined8 *)(param_1 + 0x400));
    (**(code **)(**(longlong **)(param_1 + 0x400) + 0x250))(*(longlong **)(param_1 + 0x400),3);
    if (local_38 != (void *)0x0) {
      FUN_141de9200();
    }
  }
  return;
}



// ======== d3 callee-of 0x1416d15e0  @ 141db7f10 ========

ulonglong FUN_141db7f10(longlong *param_1,wchar_t *param_2,int param_3,int param_4,int param_5)

{
  ushort uVar1;
  void *pvVar2;
  wint_t wVar3;
  int iVar4;
  uint uVar5;
  wchar_t *pwVar6;
  longlong lVar7;
  wchar_t wVar8;
  int iVar9;
  int iVar10;
  undefined2 *puVar11;
  undefined2 *puVar12;
  int iVar13;
  longlong lVar14;
  undefined2 *puVar15;
  void *local_48;
  int local_40;
  undefined4 local_3c;
  undefined2 *local_38;
  int local_30;
  undefined4 local_2c;
  
  if (param_2 == (wchar_t *)0x0) {
    return 0xffffffffffffffff;
  }
  if (param_4 == 0) {
    iVar9 = (int)param_1[1];
    puVar12 = &DAT_14391f7d0;
    if (iVar9 == 0) {
      pwVar6 = L"";
    }
    else {
      pwVar6 = (wchar_t *)*param_1;
    }
    if (param_5 != -1) {
      iVar10 = iVar9 + -2;
      if (iVar9 == 0) {
        iVar10 = -1;
      }
      if (param_5 < 0) {
        iVar10 = 0;
      }
      else if (param_5 < iVar10) {
        iVar10 = param_5;
      }
      pwVar6 = pwVar6 + iVar10;
    }
    if (param_3 == 1) {
      pwVar6 = (wchar_t *)FUN_1407f26d0();
    }
    else {
      pwVar6 = wcsstr(pwVar6,param_2);
    }
    if (pwVar6 != (wchar_t *)0x0) {
      if ((int)param_1[1] != 0) {
        puVar12 = (undefined2 *)*param_1;
      }
      return (longlong)pwVar6 - (longlong)puVar12 >> 1;
    }
    return 0xffffffffffffffff;
  }
  if (param_3 != 1) {
    lVar14 = -1;
    do {
      lVar14 = lVar14 + 1;
    } while (param_2[lVar14] != L'\0');
    iVar9 = (int)lVar14;
    if (iVar9 < 2) {
      iVar9 = 1;
    }
    if (param_5 != -1) {
      iVar10 = (int)param_1[1] + -1;
      if ((int)param_1[1] == 0) {
        iVar10 = 0;
      }
      if (param_5 < iVar10) goto LAB_141db81d4;
    }
    param_5 = 0;
    if ((int)param_1[1] != 0) {
      param_5 = (int)param_1[1] + -1;
    }
LAB_141db81d4:
    uVar5 = param_5 - iVar9;
    if (-1 < (int)uVar5) {
      lVar14 = (longlong)(int)uVar5;
      do {
        lVar7 = 0;
        if (*param_2 != L'\0') {
          pwVar6 = (wchar_t *)(*param_1 + lVar14 * 2);
          wVar8 = *param_2;
          do {
            if (*pwVar6 != wVar8) break;
            wVar8 = param_2[lVar7 + 1];
            lVar7 = lVar7 + 1;
            pwVar6 = pwVar6 + 1;
          } while (wVar8 != L'\0');
        }
        if (param_2[lVar7] == L'\0') {
          return (ulonglong)uVar5;
        }
        uVar5 = uVar5 - 1;
        lVar14 = lVar14 + -1;
      } while (-1 < lVar14);
    }
    return 0xffffffff;
  }
  puVar15 = (undefined2 *)0x0;
  iVar9 = 0;
  iVar10 = 0;
  local_38 = (undefined2 *)0x0;
  local_2c = 0;
  puVar12 = puVar15;
  if (*param_2 != L'\0') {
    lVar14 = -1;
    do {
      lVar14 = lVar14 + 1;
    } while (param_2[lVar14] != L'\0');
    iVar13 = (int)lVar14 + 1;
    local_30 = iVar13;
    if (0 < iVar13) {
      FUN_1407e2ee0(&local_38,0);
    }
    iVar9 = local_30;
    puVar12 = local_38;
    FUN_141dd1c10(local_38,param_2,(longlong)iVar13 * 2);
    iVar13 = iVar9 + -1;
    if (iVar9 != 0) goto LAB_141db8058;
  }
  iVar13 = iVar10;
LAB_141db8058:
  puVar11 = puVar15;
  if (0 < iVar13) {
    do {
      uVar1 = puVar12[(longlong)puVar11];
      if (uVar1 < 0x80) {
        iVar4 = toupper((int)(char)uVar1);
        wVar3 = (wint_t)iVar4;
      }
      else {
        wVar3 = towupper(uVar1);
      }
      puVar12[(longlong)puVar11] = wVar3;
      puVar11 = (undefined2 *)((longlong)puVar11 + 1);
    } while ((longlong)puVar11 < (longlong)iVar13);
  }
  local_40 = (int)param_1[1];
  lVar14 = (longlong)local_40;
  pvVar2 = (void *)*param_1;
  local_48 = (void *)0x0;
  if (local_40 == 0) {
    local_3c = 0;
  }
  else {
    FUN_1407f1420(&local_48,local_40,0);
    memcpy(local_48,pvVar2,lVar14 * 2);
  }
  pvVar2 = local_48;
  if (local_40 != 0) {
    iVar10 = local_40 + -1;
  }
  if (0 < (longlong)iVar10) {
    do {
      uVar1 = *(ushort *)((longlong)pvVar2 + (longlong)puVar15 * 2);
      if (uVar1 < 0x80) {
        iVar13 = toupper((int)(char)uVar1);
        wVar3 = (wint_t)iVar13;
      }
      else {
        wVar3 = towupper(uVar1);
      }
      *(wint_t *)((longlong)pvVar2 + (longlong)puVar15 * 2) = wVar3;
      puVar15 = (undefined2 *)((longlong)puVar15 + 1);
    } while ((longlong)puVar15 < (longlong)iVar10);
  }
  puVar15 = &DAT_14391f7d0;
  if (iVar9 != 0) {
    puVar15 = puVar12;
  }
  uVar5 = FUN_141db7f10(&local_48,puVar15,0,param_4,param_5);
  if (local_48 != (void *)0x0) {
    FUN_141de9200();
  }
  if (puVar12 != (undefined2 *)0x0) {
    FUN_141de9200(puVar12);
  }
  return (ulonglong)uVar5;
}



// ======== d3 callee-of 0x1416d15e0  @ 1416f9930 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1416f9930(longlong param_1,void **param_2,longlong param_3)

{
  uint *puVar1;
  int iVar2;
  bool bVar3;
  ulonglong uVar4;
  int iVar5;
  uint uVar6;
  longlong lVar7;
  undefined8 uVar8;
  longlong *plVar9;
  void *pvVar10;
  uint uVar11;
  uint *puVar12;
  int iVar13;
  void *pvVar14;
  uint uVar15;
  wchar_t *_Src;
  uint uVar16;
  undefined1 auStack_1e8 [32];
  undefined4 local_1c8;
  undefined4 local_1c0;
  undefined8 local_1b8;
  uint *puStack_1b0;
  uint local_1a8;
  uint uStack_1a4;
  uint uStack_1a0;
  undefined4 uStack_19c;
  void *local_198;
  ulonglong local_190;
  void *local_188;
  undefined8 local_180;
  void **local_178;
  longlong *local_170;
  undefined8 uStack_168;
  uint *local_160;
  undefined8 uStack_158;
  undefined8 local_150;
  longlong local_148 [2];
  longlong *local_138;
  void *pvStack_130;
  uint *puStack_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  ulonglong local_48;
  
  local_48 = DAT_145909780 ^ (ulonglong)auStack_1e8;
  pvVar10 = (void *)0x0;
  iVar13 = 0;
  pvVar14 = pvVar10;
  local_178 = param_2;
  do {
    FUN_141eb3410(&local_188,&DAT_143f0c55e,1);
    FUN_141eb3410(&local_1b8,&DAT_143f0c55f,1);
    pvVar14 = (void *)((longlong)pvVar14 + 8);
  } while ((longlong)pvVar14 < 0x60);
  plVar9 = *(longlong **)(param_1 + 0x3a0);
  if (plVar9 != (longlong *)0x0) {
    (**(code **)(*plVar9 + 0x250))(plVar9,1);
  }
  plVar9 = *(longlong **)(param_1 + 0x3a8);
  if (plVar9 != (longlong *)0x0) {
    (**(code **)(*plVar9 + 0x250))(plVar9,1);
  }
  plVar9 = *(longlong **)(param_1 + 0x3b0);
  if (plVar9 != (longlong *)0x0) {
    (**(code **)(*plVar9 + 0x250))(plVar9,3);
  }
  plVar9 = *(longlong **)(param_1 + 0x3b8);
  if (plVar9 != (longlong *)0x0) {
    (**(code **)(*plVar9 + 0x250))(plVar9,1);
  }
  plVar9 = *(longlong **)(param_1 + 0x3c0);
  if (plVar9 != (longlong *)0x0) {
    (**(code **)(*plVar9 + 0x250))(plVar9,1);
  }
  local_1c8 = 0xffffffff;
  iVar5 = FUN_141db7f10(param_2,L"PLAT/",1);
  if (iVar5 == -1) {
LAB_1416f9a68:
    bVar3 = false;
  }
  else {
    local_1c8 = 0xffffffff;
    iVar5 = FUN_141db7f10(param_2,L"/PLAT/",1);
    if (iVar5 != -1) goto LAB_1416f9a68;
    bVar3 = true;
  }
  local_198 = (void *)0x0;
  local_190 = 0;
  if (bVar3) {
    local_188 = (void *)0x0;
    local_180 = 0;
    local_1b8 = (wchar_t *)0x0;
    puStack_1b0 = (uint *)0x2;
    FUN_1407e2ee0(&local_1b8,0);
    lVar7 = (longlong)local_1b8;
    FUN_141dd1c10(local_1b8,&DAT_143f0dc94,4);
    local_1c0 = 1;
    local_1c8 = 1;
    FUN_14082b3c0(param_2,&local_1b8,&local_188,&local_198);
    if (lVar7 != 0) {
      FUN_141de9200(lVar7);
    }
    if (local_188 != (void *)0x0) {
      FUN_141de9200();
    }
  }
  else {
    iVar5 = iVar13;
    if (&local_198 == param_2) goto LAB_1416f9b42;
    uVar16 = *(uint *)(param_2 + 1);
    pvVar14 = *param_2;
    local_190 = (ulonglong)uVar16;
    iVar5 = 0;
    if (uVar16 == 0) goto LAB_1416f9b42;
    FUN_1407f1420(&local_198,uVar16,0);
    memcpy(local_198,pvVar14,(longlong)(int)uVar16 * 2);
  }
  iVar5 = (int)local_190;
LAB_1416f9b42:
  iVar2 = *(int *)(param_2 + 1);
  local_188 = (void *)0x0;
  local_180 = CONCAT44(local_180._4_4_,iVar2);
  local_1b8 = *local_178;
  pvVar14 = pvVar10;
  if (iVar2 != 0) {
    FUN_1407f1420(&local_188,iVar2,0);
    pvVar14 = local_188;
    memcpy(local_188,local_1b8,(longlong)iVar2 * 2);
  }
  if (bVar3) {
    if (iVar5 != 0) {
      iVar13 = iVar5 + -1;
    }
    uVar16 = iVar13 + 0x20;
    local_1b8 = (wchar_t *)0x0;
    puStack_1b0 = (uint *)(ulonglong)uVar16;
    if (0 < (int)uVar16) {
      FUN_1407e2ee0(&local_1b8,0);
      uVar16 = (uint)puStack_1b0;
    }
    _Src = local_1b8;
    uVar8 = u__Game_Art_UI_Tips_Tex_Img_PLAT__143f14590._8_8_;
    *(undefined8 *)local_1b8 = u__Game_Art_UI_Tips_Tex_Img_PLAT__143f14590._0_8_;
    *(undefined8 *)(local_1b8 + 4) = uVar8;
    uVar8 = u__Game_Art_UI_Tips_Tex_Img_PLAT__143f14590._24_8_;
    *(undefined8 *)(local_1b8 + 8) = u__Game_Art_UI_Tips_Tex_Img_PLAT__143f14590._16_8_;
    *(undefined8 *)(local_1b8 + 0xc) = uVar8;
    uVar8 = u__Game_Art_UI_Tips_Tex_Img_PLAT__143f14590._40_8_;
    *(undefined8 *)(local_1b8 + 0x10) = u__Game_Art_UI_Tips_Tex_Img_PLAT__143f14590._32_8_;
    *(undefined8 *)(local_1b8 + 0x14) = uVar8;
    *(undefined8 *)(local_1b8 + 0x18) = u__Game_Art_UI_Tips_Tex_Img_PLAT__143f14590._48_8_;
    *(undefined4 *)(local_1b8 + 0x1c) = u__Game_Art_UI_Tips_Tex_Img_PLAT__143f14590._56_4_;
    local_1b8[0x1e] = u__Game_Art_UI_Tips_Tex_Img_PLAT__143f14590[0x1e];
    memcpy(local_1b8 + 0x1f,local_198,(longlong)iVar13 * 2);
    _Src[(longlong)iVar13 + 0x1f] = L'\0';
  }
  else {
    if (iVar5 != 0) {
      iVar13 = iVar5 + -1;
    }
    uVar16 = iVar13 + 0x1b;
    local_1b8 = (wchar_t *)0x0;
    puStack_1b0 = (uint *)(ulonglong)uVar16;
    if (0 < (int)uVar16) {
      FUN_1407e2ee0(&local_1b8,0);
      uVar16 = (uint)puStack_1b0;
    }
    _Src = local_1b8;
    uVar8 = u__Game_Art_UI_Tips_Tex_Img__143f14618._8_8_;
    *(undefined8 *)local_1b8 = u__Game_Art_UI_Tips_Tex_Img__143f14618._0_8_;
    *(undefined8 *)(local_1b8 + 4) = uVar8;
    uVar8 = u__Game_Art_UI_Tips_Tex_Img__143f14618._24_8_;
    *(undefined8 *)(local_1b8 + 8) = u__Game_Art_UI_Tips_Tex_Img__143f14618._16_8_;
    *(undefined8 *)(local_1b8 + 0xc) = uVar8;
    uVar8 = u__Game_Art_UI_Tips_Tex_Img__143f14618._40_8_;
    *(undefined8 *)(local_1b8 + 0x10) = u__Game_Art_UI_Tips_Tex_Img__143f14618._32_8_;
    *(undefined8 *)(local_1b8 + 0x14) = uVar8;
    *(undefined4 *)(local_1b8 + 0x18) = u__Game_Art_UI_Tips_Tex_Img__143f14618._48_4_;
    memcpy(local_1b8 + 0x1a,local_198,(longlong)iVar13 * 2);
    _Src[(longlong)iVar13 + 0x1a] = L'\0';
  }
  if (pvVar14 != (void *)0x0) {
    FUN_141de9200(pvVar14);
  }
  local_1b8 = (void *)0x0;
  puStack_1b0 = (uint *)CONCAT44(puStack_1b0._4_4_,uVar16);
  if (uVar16 == 0) {
    puStack_1b0 = (uint *)0x0;
  }
  else {
    FUN_1407f1420(&local_1b8,uVar16,0);
    memcpy(local_1b8,_Src,(longlong)(int)uVar16 * 2);
  }
  FUN_141a690a0(local_148,&local_1b8,0);
  FUN_14159f740(param_1 + 0x520,local_148);
  FUN_1416f55f0(param_1);
  if ((*(longlong *)(param_1 + 0x4c0) == 0) && (lVar7 = FUN_140d283a0(), lVar7 != 0)) {
    lVar7 = FUN_140d283a0();
    lVar7 = *(longlong *)(*(longlong *)(lVar7 + 0x498) + 0x3b0);
    if (lVar7 != 0) {
      uVar8 = FUN_14102b570(lVar7,&DAT_14598a540);
      *(undefined8 *)(param_1 + 0x4c0) = uVar8;
    }
  }
  if (*(longlong **)(param_1 + 0x4c0) != (longlong *)0x0) {
    plVar9 = (longlong *)(**(code **)(**(longlong **)(param_1 + 0x4c0) + 600))();
    uVar16 = *(uint *)(plVar9 + 5);
    local_1b8 = (void *)0x100000000;
    puVar1 = (uint *)(plVar9 + 2);
    uStack_1a4 = 0;
    uStack_1a0 = 0;
    if (uVar16 != 0) {
      puVar12 = (uint *)plVar9[4];
      if ((uint *)plVar9[4] == (uint *)0x0) {
        puVar12 = puVar1;
      }
      uVar6 = *puVar12;
      pvVar14 = pvVar10;
      while (uVar6 == 0) {
        iVar13 = (int)pvVar10;
        uStack_1a0 = (int)pvVar14 + 0x20;
        pvVar14 = (void *)(ulonglong)uStack_1a0;
        pvVar10 = (void *)(ulonglong)(iVar13 + 1U);
        uVar4 = (ulonglong)local_1b8 >> 0x20;
        local_1b8 = (wchar_t *)CONCAT44((int)uVar4,iVar13 + 1U);
        uStack_1a4 = uVar16;
        if ((int)((uVar16 - 1) + ((int)(uVar16 - 1) >> 0x1f & 0x1fU)) >> 5 <= iVar13)
        goto LAB_1416f9e3a;
        uVar6 = puVar12[(longlong)iVar13 + 1];
      }
      uVar6 = -uVar6 & uVar6;
      iVar13 = 0x1f;
      if (uVar6 != 0) {
        for (; uVar6 >> iVar13 == 0; iVar13 = iVar13 + -1) {
        }
      }
      local_1b8 = (wchar_t *)CONCAT44(uVar6,SUB84(local_1b8,0));
      if (uVar6 == 0) {
        iVar13 = 0x20;
      }
      else {
        iVar13 = 0x1f - iVar13;
      }
      uStack_1a4 = (uStack_1a0 - iVar13) + 0x1f;
      if ((int)uVar16 < (int)uStack_1a4) {
        uStack_1a4 = uVar16;
      }
    }
LAB_1416f9e3a:
    uVar6 = *(uint *)(plVar9 + 5);
    uStack_158 = CONCAT44(uStack_1a4,0xffffffff);
    local_150 = CONCAT44(uStack_19c,uStack_1a0);
    local_1a8 = -1 << ((byte)uVar6 & 0x1f);
    local_188 = (void *)CONCAT44(local_188._4_4_,local_1a8);
    pvStack_130 = local_1b8;
    uVar15 = uVar6 & 0xffffffe0;
    uStack_168 = local_1b8;
    puStack_1b0 = puVar1;
    uStack_1a4 = uVar6;
    local_170 = plVar9;
    local_160 = puVar1;
    local_138 = plVar9;
    puStack_128 = puVar1;
    uStack_120 = uStack_158;
    uStack_118 = local_150;
    if (uVar6 != uVar16) {
      puVar12 = (uint *)plVar9[4];
      if ((uint *)plVar9[4] == (uint *)0x0) {
        puVar12 = puVar1;
      }
      uVar11 = puVar12[(int)uVar6 >> 5] & local_1a8;
      iVar13 = (int)uVar6 >> 5;
      while (uVar11 == 0) {
        uVar15 = uVar15 + 0x20;
        uStack_1a4 = uVar16;
        uVar6 = uVar16;
        if ((int)((uVar16 - 1) + ((int)(uVar16 - 1) >> 0x1f & 0x1fU)) >> 5 <= iVar13)
        goto LAB_1416f9f30;
        uVar11 = puVar12[(longlong)iVar13 + 1];
        local_1a8 = 0xffffffff;
        iVar13 = iVar13 + 1;
      }
      uVar11 = -uVar11 & uVar11;
      iVar13 = 0x1f;
      if (uVar11 != 0) {
        for (; uVar11 >> iVar13 == 0; iVar13 = iVar13 + -1) {
        }
      }
      if (uVar11 == 0) {
        iVar13 = 0x20;
      }
      else {
        iVar13 = 0x1f - iVar13;
      }
      uStack_1a4 = (uVar15 - iVar13) + 0x1f;
      uVar6 = uStack_1a4;
      if ((int)uVar16 < (int)uStack_1a4) {
        uStack_1a4 = uVar16;
        uVar6 = uVar16;
      }
    }
LAB_1416f9f30:
    while( true ) {
      if (((uStack_158._4_4_ == uVar6) && (local_160 == puVar1)) && (local_170 == plVar9)) break;
      lVar7 = *(longlong *)(*local_170 + (longlong)(int)uStack_158._4_4_ * 0x18 + 8);
      if ((lVar7 != 0) && (*(longlong *)(lVar7 + 8) == param_3)) {
        FUN_1416fa000(param_1,*(undefined8 *)(lVar7 + 0x10),*(undefined8 *)(lVar7 + 0x18),
                      *(undefined8 *)(lVar7 + 0x20));
      }
      uStack_158 = CONCAT44(uStack_158._4_4_,(uint)uStack_158 & ~uStack_168._4_4_);
      FUN_1407e9ca0(&uStack_168);
    }
  }
  if (local_148[0] != 0) {
    FUN_141de9200();
  }
  if (_Src != (wchar_t *)0x0) {
    FUN_141de9200(_Src);
  }
  if (local_198 != (void *)0x0) {
    FUN_141de9200(local_198);
  }
  if (*local_178 != (void *)0x0) {
    FUN_141de9200();
  }
  return;
}



// ======== d3 callee-of 0x1416d15e0  @ 1416f5a50 ========

void FUN_1416f5a50(longlong param_1,longlong *param_2,longlong *param_3)

{
  longlong *plVar1;
  undefined8 uVar2;
  longlong lVar3;
  longlong local_68 [2];
  undefined1 local_58 [8];
  longlong local_50;
  longlong local_40;
  longlong local_30;
  longlong local_20;
  
  plVar1 = *(longlong **)(param_1 + 0x3a0);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,1);
  }
  plVar1 = *(longlong **)(param_1 + 0x3a8);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,1);
  }
  plVar1 = *(longlong **)(param_1 + 0x3b0);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,1);
  }
  plVar1 = *(longlong **)(param_1 + 0x3b8);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,3);
  }
  plVar1 = *(longlong **)(param_1 + 0x3c0);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,1);
  }
  uVar2 = FUN_1416e3c30(local_68,param_3);
  FUN_14159f740(param_1 + 0x578,uVar2);
  if (local_68[0] != 0) {
    FUN_141de9200();
  }
  if (*(longlong *)(param_1 + 0x418) != 0) {
    lVar3 = FUN_1411fd400(local_58,param_2);
    FUN_141a87aa0(*(undefined8 *)(param_1 + 0x418),lVar3 + 0x38);
    if (local_20 != 0) {
      FUN_141de9200();
    }
    if (local_30 != 0) {
      FUN_141de9200();
    }
    if (local_40 != 0) {
      FUN_141de9200();
    }
    if (local_50 != 0) {
      FUN_141de9200();
    }
    FUN_141a54c00(*(undefined8 *)(param_1 + 0x418));
  }
  if (*param_2 != 0) {
    FUN_141de9200();
  }
  if (*param_3 != 0) {
    FUN_141de9200();
  }
  return;
}



// ======== d3 callee-of 0x1416d15e0  @ 1416fb7d0 ========

void FUN_1416fb7d0(longlong param_1,longlong *param_2,longlong *param_3)

{
  int *piVar1;
  int iVar2;
  longlong *plVar3;
  undefined8 uVar4;
  longlong lVar5;
  longlong local_98 [2];
  undefined1 local_88 [8];
  longlong *local_80;
  undefined1 local_70 [8];
  longlong local_68;
  longlong local_58;
  longlong local_48;
  longlong local_38;
  undefined1 local_28 [32];
  
  plVar3 = *(longlong **)(param_1 + 0x3a0);
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 0x250))(plVar3,1);
  }
  plVar3 = *(longlong **)(param_1 + 0x3a8);
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 0x250))(plVar3,3);
  }
  plVar3 = *(longlong **)(param_1 + 0x3b0);
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 0x250))(plVar3,1);
  }
  plVar3 = *(longlong **)(param_1 + 0x3b8);
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 0x250))(plVar3,1);
  }
  plVar3 = *(longlong **)(param_1 + 0x3c0);
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 0x250))(plVar3,1);
  }
  uVar4 = FUN_1416e3c30(local_98,param_3);
  FUN_14159f740(param_1 + 0x4c8,uVar4);
  if (local_98[0] != 0) {
    FUN_141de9200();
  }
  if (*(longlong *)(param_1 + 0x410) != 0) {
    lVar5 = FUN_1411fd400(local_70,param_2);
    FUN_141e24190(local_88,lVar5 + 0x38);
    if (local_38 != 0) {
      FUN_141de9200();
    }
    if (local_48 != 0) {
      FUN_141de9200();
    }
    if (local_58 != 0) {
      FUN_141de9200();
    }
    if (local_68 != 0) {
      FUN_141de9200();
    }
    uVar4 = FUN_141e08fe0(local_28,local_88);
    FUN_141a87ad0(*(undefined8 *)(param_1 + 0x410),uVar4);
    FUN_141a54c00(*(undefined8 *)(param_1 + 0x410));
    if (local_80 != (longlong *)0x0) {
      LOCK();
      plVar3 = local_80 + 1;
      lVar5 = *plVar3;
      *(int *)plVar3 = (int)*plVar3 + -1;
      UNLOCK();
      if ((int)lVar5 == 1) {
        (**(code **)*local_80)(local_80);
        LOCK();
        piVar1 = (int *)((longlong)local_80 + 0xc);
        iVar2 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar2 == 1) {
          (**(code **)(*local_80 + 8))(local_80,1);
        }
      }
    }
  }
  if (*param_2 != 0) {
    FUN_141de9200();
  }
  if (*param_3 != 0) {
    FUN_141de9200();
  }
  return;
}



// ======== d3 callee-of 0x1416d15e0  @ 14205a760 ========

ulonglong FUN_14205a760(uint *param_1)

{
  uint uVar1;
  ulonglong in_RAX;
  uint uVar2;
  
  if (param_1[1] != 0) {
    uVar1 = *param_1;
    in_RAX = (ulonglong)uVar1;
    if ((((-1 < (int)uVar1) && ((int)uVar1 < (int)DAT_145b9e804)) &&
        (uVar2 = (int)uVar1 >> 0x1f & 0xffff, uVar1 = uVar1 + uVar2,
        in_RAX = *(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar1 >> 0x10) * 8) +
                 (longlong)(int)((uVar1 & 0xffff) - uVar2) * 0x18, in_RAX != 0)) &&
       ((*(uint *)(in_RAX + 0x10) == param_1[1] && ((*(uint *)(in_RAX + 8) & 0x30000000) == 0)))) {
      return CONCAT71((int7)(in_RAX >> 8),1);
    }
  }
  return in_RAX & 0xffffffffffffff00;
}



// ======== d3 callee-of 0x1416d15e0  @ 142058d50 ========

undefined8 FUN_142058d50(int *param_1)

{
  undefined8 *puVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  
  if ((((param_1[1] != 0) && (iVar2 = *param_1, -1 < iVar2)) && (iVar2 < (int)DAT_145b9e804)) &&
     (((uVar4 = iVar2 >> 0x1f & 0xffff, uVar3 = iVar2 + uVar4,
       puVar1 = (undefined8 *)
                (*(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar3 >> 0x10) * 8) +
                (longlong)(int)((uVar3 & 0xffff) - uVar4) * 0x18), puVar1 != (undefined8 *)0x0 &&
       (*(int *)(puVar1 + 2) == param_1[1])) && ((*(uint *)(puVar1 + 1) & 0x30000000) == 0)))) {
    return *puVar1;
  }
  return 0;
}



// ======== d3 callee-of 0x1416d15e0  @ 1407e5680 ========

undefined8 * FUN_1407e5680(undefined8 *param_1,undefined8 *param_2)

{
  int iVar1;
  void *_Src;
  
  *param_1 = 0;
  iVar1 = *(int *)(param_2 + 1);
  _Src = (void *)*param_2;
  *(int *)(param_1 + 1) = iVar1;
  if (iVar1 == 0) {
    *(undefined4 *)((longlong)param_1 + 0xc) = 0;
    return param_1;
  }
  FUN_1407f1420(param_1,iVar1,0);
  memcpy((void *)*param_1,_Src,(longlong)iVar1 * 2);
  return param_1;
}



// ======== d3 callee-of 0x1416d15e0  @ 1416fb9b0 ========

void FUN_1416fb9b0(longlong param_1,longlong *param_2,undefined8 param_3)

{
  longlong *plVar1;
  
  plVar1 = *(longlong **)(param_1 + 0x3a0);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,1);
  }
  plVar1 = *(longlong **)(param_1 + 0x3a8);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,1);
  }
  plVar1 = *(longlong **)(param_1 + 0x3b0);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,1);
  }
  plVar1 = *(longlong **)(param_1 + 0x3b8);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,1);
  }
  plVar1 = *(longlong **)(param_1 + 0x3c0);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,3);
  }
  plVar1 = *(longlong **)(param_1 + 1000);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x2b0))(plVar1,param_3,0);
    (**(code **)(**(longlong **)(param_1 + 1000) + 0x250))(*(longlong **)(param_1 + 1000),3);
  }
  plVar1 = *(longlong **)(param_1 + 0x420);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,3);
    FUN_141a87aa0(*(undefined8 *)(param_1 + 0x420),param_2);
    FUN_141a54c00(*(undefined8 *)(param_1 + 0x420));
  }
  if (*param_2 != 0) {
    FUN_141de9200();
  }
  return;
}



// ======== d3 callee-of 0x1416d15e0  @ 1416fbac0 ========

void FUN_1416fbac0(longlong param_1,longlong *param_2)

{
  int *piVar1;
  int iVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined1 local_88 [8];
  longlong *local_80;
  undefined1 local_70 [8];
  longlong local_68;
  longlong local_58;
  longlong local_48;
  longlong local_38;
  undefined1 local_28 [32];
  
  plVar3 = *(longlong **)(param_1 + 0x3a0);
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 0x250))(plVar3,3);
  }
  plVar3 = *(longlong **)(param_1 + 0x3a8);
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 0x250))(plVar3,1);
  }
  plVar3 = *(longlong **)(param_1 + 0x3b0);
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 0x250))(plVar3,1);
  }
  plVar3 = *(longlong **)(param_1 + 0x3b8);
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 0x250))(plVar3,1);
  }
  plVar3 = *(longlong **)(param_1 + 0x3c0);
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 0x250))(plVar3,1);
  }
  if (*(longlong *)(param_1 + 0x408) != 0) {
    lVar4 = FUN_1411fd400(local_70,param_2);
    FUN_141e24190(local_88,lVar4 + 0x38);
    if (local_38 != 0) {
      FUN_141de9200();
    }
    if (local_48 != 0) {
      FUN_141de9200();
    }
    if (local_58 != 0) {
      FUN_141de9200();
    }
    if (local_68 != 0) {
      FUN_141de9200();
    }
    uVar5 = FUN_141e08fe0(local_28,local_88);
    FUN_141a87ad0(*(undefined8 *)(param_1 + 0x408),uVar5);
    FUN_141a54c00(*(undefined8 *)(param_1 + 0x408));
    if (local_80 != (longlong *)0x0) {
      LOCK();
      plVar3 = local_80 + 1;
      lVar4 = *plVar3;
      *(int *)plVar3 = (int)*plVar3 + -1;
      UNLOCK();
      if ((int)lVar4 == 1) {
        (**(code **)*local_80)(local_80);
        LOCK();
        piVar1 = (int *)((longlong)local_80 + 0xc);
        iVar2 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar2 == 1) {
          (**(code **)(*local_80 + 8))(local_80,1);
        }
      }
    }
  }
  if (*param_2 != 0) {
    FUN_141de9200();
  }
  return;
}



// ======== d3 callee-of 0x1416d15e0  @ 143126c80 ========

undefined8 * FUN_143126c80(undefined8 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  *(undefined1 *)(param_1 + 5) = 0;
  *(ushort *)((longlong)param_1 + 0x2a) = *(ushort *)((longlong)param_1 + 0x2a) & 0xfff0;
  *(undefined4 *)((longlong)param_1 + 0x2c) = 8;
  return param_1;
}



// ======== d3 callee-of 0x1416d15e0  @ 14189efb0 ========

longlong FUN_14189efb0(void)

{
  if (DAT_1459f8020 == 0) {
    FUN_141f291f0(L"/Script/AT",L"EmbeddedMoviePlayerActor",&DAT_1459f8020,_guard_check_icall,0x378,
                  0x10000000,0,L"Engine",&LAB_1418a01d0,FUN_1407e4520,FUN_142b0aa80,&LAB_140885460,
                  &LAB_14083c250,0);
  }
  return DAT_1459f8020;
}



// ======== d3 callee-of 0x1416d15e0  @ 142de92d0 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_142de92d0(undefined8 param_1,undefined8 param_2,undefined1 (*param_3) [12],longlong param_4
                  ,undefined8 param_5)

{
  undefined4 *puVar1;
  undefined1 auStack_88 [32];
  undefined1 local_68 [16];
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  uint uStack_4c;
  undefined1 local_48 [16];
  uint local_38;
  uint uStack_34;
  uint uStack_30;
  uint uStack_2c;
  ulonglong local_28;
  
  local_28 = DAT_145909780 ^ (ulonglong)auStack_88;
  local_38 = _DAT_145c72760 & _DAT_145c72770;
  uStack_34 = uRam0000000145c72764 & uRam0000000145c72774;
  uStack_30 = uRam0000000145c72768 & uRam0000000145c72778;
  uStack_2c = uRam0000000145c7276c & uRam0000000145c7277c;
  local_58 = 0;
  uStack_54 = 0;
  uStack_50 = 0;
  uStack_4c = uRam0000000145c7276c;
  local_48 = ZEXT816(0);
  if (param_3 != (undefined1 (*) [12])0x0) {
    local_48._12_4_ = 0;
    local_48._0_12_ = *param_3;
  }
  if (param_4 != 0) {
    puVar1 = (undefined4 *)FUN_141e4db90(param_4,local_68);
    local_58 = *puVar1;
    uStack_54 = puVar1[1];
    uStack_50 = puVar1[2];
    uStack_4c = puVar1[3];
  }
  FUN_142de87e0(param_1,param_2,&local_58,param_5);
  return;
}



// ======== d3 callee-of 0x1416d15e0  @ 140befdd0 ========

void FUN_140befdd0(longlong *param_1,longlong *param_2,undefined8 param_3,undefined1 param_4)

{
  longlong *plVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined4 local_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  if ((char)param_1[0x6a] != '\0') {
    (**(code **)(*param_1 + 0x620))();
  }
  *(undefined1 *)(param_1 + 0x6e) = param_4;
  FUN_1420575e0(param_1 + 0x69,param_3);
  plVar1 = (longlong *)FUN_142058d50(param_1 + 0x69);
  (**(code **)(*plVar1 + 0x468))(plVar1,1);
  local_18 = DAT_1456d9c60;
  uStack_14 = DAT_1456d9c64;
  uStack_10 = DAT_1456d9c68;
  uStack_c = DAT_1456d9c6c;
  lVar2 = FUN_142058d50(param_1 + 0x69);
  FUN_1429ddbd0(*(undefined8 *)(lVar2 + 0x398),&local_18);
  uVar3 = FUN_1407e5680(&local_18,param_2);
  FUN_140be5de0(param_1,uVar3);
  if (*param_2 != 0) {
    FUN_141de9200();
  }
  return;
}



// ======== d3 callee-of 0x1416d15e0  @ 1416d8670 ========

void FUN_1416d8670(longlong param_1)

{
  longlong *plVar1;
  undefined1 uVar2;
  ulonglong uVar3;
  undefined8 *puVar4;
  ulonglong uVar5;
  
  uVar3 = 0;
  puVar4 = *(undefined8 **)(param_1 + 0x3c8);
  uVar5 = (longlong)*(int *)(param_1 + 0x3d0) & 0x1fffffffffffffff;
  if (*(char *)(param_1 + 0x41c) == '\0') {
    if (puVar4 + *(int *)(param_1 + 0x3d0) < puVar4) {
      uVar5 = uVar3;
    }
    if (uVar5 != 0) {
      do {
        (**(code **)(*(longlong *)*puVar4 + 0x250))((longlong *)*puVar4,3);
        uVar3 = uVar3 + 1;
        puVar4 = puVar4 + 1;
      } while (uVar3 != uVar5);
    }
    (**(code **)(**(longlong **)(param_1 + 0x3b8) + 0x250))(*(longlong **)(param_1 + 0x3b8),4);
    (**(code **)(**(longlong **)(param_1 + 0x3c0) + 0x250))(*(longlong **)(param_1 + 0x3c0),4);
    plVar1 = *(longlong **)(param_1 + 0x3e0);
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 0x250))(plVar1,3);
    }
    if (*(char *)(param_1 + 0x41e) != '\x02') {
      (**(code **)(**(longlong **)(param_1 + 0x3b0) + 0x250))(*(longlong **)(param_1 + 0x3b0),3);
    }
    uVar2 = 1;
  }
  else {
    if (puVar4 + *(int *)(param_1 + 0x3d0) < puVar4) {
      uVar5 = uVar3;
    }
    if (uVar5 != 0) {
      do {
        (**(code **)(*(longlong *)*puVar4 + 0x250))((longlong *)*puVar4,1);
        uVar3 = uVar3 + 1;
        puVar4 = puVar4 + 1;
      } while (uVar3 != uVar5);
    }
    (**(code **)(**(longlong **)(param_1 + 0x3b8) + 0x250))(*(longlong **)(param_1 + 0x3b8),1);
    (**(code **)(**(longlong **)(param_1 + 0x3c0) + 0x250))(*(longlong **)(param_1 + 0x3c0),1);
    plVar1 = *(longlong **)(param_1 + 0x3e0);
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 0x250))(plVar1,1);
    }
    (**(code **)(**(longlong **)(param_1 + 0x3b0) + 0x250))(*(longlong **)(param_1 + 0x3b0),1);
    uVar2 = 3;
  }
                    /* WARNING: Could not recover jumptable at 0x0001416d87c8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(longlong **)(param_1 + 0x3d8) + 0x250))(*(longlong **)(param_1 + 0x3d8),uVar2);
  return;
}



