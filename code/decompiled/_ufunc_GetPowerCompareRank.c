// UFunction 'GetPowerCompareRank' — exec thunk(s) + native impl (via UE native-registration table)
// ---- FUN_14188e6b0 @ 14188e6b0 ----

void FUN_14188e6b0(undefined8 param_1,longlong param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  uVar1 = FUN_140fb95a0();
  *param_3 = uVar1;
  return;
}



// ---- FUN_140fb95a0 @ 140fb95a0 ----

undefined8 FUN_140fb95a0(longlong param_1)

{
  bool bVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  longlong *plVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  int iVar8;
  float fVar9;
  
  uVar7 = 0;
  lVar3 = FUN_140fb7710();
  iVar2 = *(int *)(param_1 + 0x148);
  iVar8 = 0;
  if (iVar2 != 1) {
    if (iVar2 < 2) {
      return 0;
    }
    if (iVar2 < 4) {
      lVar4 = *(longlong *)(param_1 + 0xf8);
      if (lVar4 == 0) {
        lVar4 = FUN_142c17e40(param_1);
      }
      plVar5 = (longlong *)FUN_140d22f30(lVar4,0);
      iVar2 = iVar8;
      if (plVar5 != (longlong *)0x0) {
        uVar6 = FUN_14181d6b0();
        lVar4 = (**(code **)(*plVar5 + 0x600))(plVar5,uVar6);
        if (lVar4 != 0) {
          iVar2 = FUN_140f8aba0(lVar4);
        }
      }
      if (*(longlong *)(param_1 + 0x158) == 0) {
        plVar5 = *(longlong **)(param_1 + 0xf0);
        if (plVar5 != (longlong *)0x0) {
          lVar4 = FUN_141758410();
          if ((*(int *)(plVar5[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
             (*(longlong *)(*(longlong *)(plVar5[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
              != lVar4 + 0x88)) {
            bVar1 = false;
          }
          else {
            bVar1 = true;
          }
          if (bVar1) {
            uVar6 = FUN_14181d6b0();
            lVar4 = (**(code **)(*plVar5 + 0x600))(plVar5,uVar6);
            if (lVar4 != 0) {
              iVar8 = FUN_140f8aba0(lVar4);
            }
          }
        }
      }
      else {
        iVar8 = (int)*(float *)(*(longlong *)(param_1 + 0x158) + 0x30);
      }
      fVar9 = (float)(iVar2 - iVar8);
      if ((*(float *)(lVar3 + 0x9c) <= fVar9) && (DAT_143aaf458 < *(float *)(lVar3 + 0x9c))) {
        uVar7 = 1;
      }
      if ((*(float *)(lVar3 + 0xa0) <= fVar9) && (DAT_143aaf458 < *(float *)(lVar3 + 0xa0))) {
        uVar7 = 2;
      }
      if ((*(float *)(lVar3 + 0xa4) <= fVar9) && (DAT_143aaf458 < *(float *)(lVar3 + 0xa4))) {
        uVar7 = 3;
      }
      if ((*(float *)(lVar3 + 0xa8) <= fVar9) && (DAT_143aaf458 < *(float *)(lVar3 + 0xa8))) {
        uVar7 = 4;
      }
      if ((*(float *)(lVar3 + 0xac) <= fVar9) && (DAT_143aaf458 < *(float *)(lVar3 + 0xac))) {
        uVar7 = 5;
      }
      if (fVar9 < *(float *)(lVar3 + 0xb0)) {
        return uVar7;
      }
      if (*(float *)(lVar3 + 0xb0) <= DAT_143aaf458) {
        return uVar7;
      }
      return 6;
    }
    if (6 < iVar2) {
      return 0;
    }
  }
  lVar4 = *(longlong *)(param_1 + 0xf8);
  iVar2 = -100;
  if (lVar4 == 0) {
    lVar4 = FUN_142c17e40(param_1);
  }
  plVar5 = (longlong *)FUN_140d22f30(lVar4,0);
  if (plVar5 != (longlong *)0x0) {
    uVar6 = FUN_14181d6b0();
    lVar4 = (**(code **)(*plVar5 + 0x600))(plVar5,uVar6);
    if (lVar4 != 0) {
      iVar2 = FUN_140f8aba0(lVar4);
    }
  }
  if (*(longlong *)(param_1 + 0x158) == 0) {
    plVar5 = *(longlong **)(param_1 + 0xf0);
    if (plVar5 != (longlong *)0x0) {
      lVar4 = FUN_141758410();
      if ((*(int *)(plVar5[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
         (*(longlong *)(*(longlong *)(plVar5[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
          lVar4 + 0x88)) {
        bVar1 = false;
      }
      else {
        bVar1 = true;
      }
      if (bVar1) {
        uVar6 = FUN_14181d6b0();
        lVar4 = (**(code **)(*plVar5 + 0x600))(plVar5,uVar6);
        if (lVar4 != 0) {
          iVar8 = FUN_140f8aba0(lVar4);
        }
      }
    }
  }
  else {
    iVar8 = (int)*(float *)(*(longlong *)(param_1 + 0x158) + 0x30);
  }
  fVar9 = (float)(iVar2 - iVar8);
  if ((*(float *)(lVar3 + 0x6c) <= fVar9) && (DAT_143aaf458 < *(float *)(lVar3 + 0x6c))) {
    uVar7 = 1;
  }
  if ((*(float *)(lVar3 + 0x70) <= fVar9) && (DAT_143aaf458 < *(float *)(lVar3 + 0x70))) {
    uVar7 = 2;
  }
  if ((*(float *)(lVar3 + 0x74) <= fVar9) && (DAT_143aaf458 < *(float *)(lVar3 + 0x74))) {
    uVar7 = 3;
  }
  if ((*(float *)(lVar3 + 0x78) <= fVar9) && (DAT_143aaf458 < *(float *)(lVar3 + 0x78))) {
    uVar7 = 4;
  }
  if ((*(float *)(lVar3 + 0x7c) <= fVar9) && (DAT_143aaf458 < *(float *)(lVar3 + 0x7c))) {
    uVar7 = 5;
  }
  if ((*(float *)(lVar3 + 0x80) <= fVar9) && (DAT_143aaf458 < *(float *)(lVar3 + 0x80))) {
    uVar7 = 6;
  }
  return uVar7;
}



// ---- FUN_14188b5f0 @ 14188b5f0 ----

longlong FUN_14188b5f0(void)

{
  if (DAT_1459f2198 == 0) {
    FUN_14203aad0(&DAT_1459f2198,&PTR_FUN_1455d7760);
  }
  return DAT_1459f2198;
}



// ---- FUN_14203aad0 @ 14203aad0 ----

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



