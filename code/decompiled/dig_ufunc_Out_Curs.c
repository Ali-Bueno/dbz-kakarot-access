// ---- FUN_1417b9dd0 @ 1417b9dd0 ----

longlong FUN_1417b9dd0(void)

{
  if (DAT_1459b1838 == 0) {
    FUN_14203aad0(&DAT_1459b1838,&PTR_FUN_1454e8c20);
  }
  return DAT_1459b1838;
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



// ---- FUN_1417bb440 @ 1417bb440 ----

void FUN_1417bb440(longlong param_1,longlong param_2)

{
  int iVar1;
  longlong *plVar2;
  longlong *plVar3;
  
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  iVar1 = *(int *)(param_1 + 0x4e4);
  if (((-1 < iVar1) && (iVar1 < *(int *)(param_1 + 0x430))) &&
     (plVar2 = *(longlong **)(*(longlong *)(param_1 + 0x428) + (longlong)iVar1 * 8),
     plVar2 != (longlong *)0x0)) {
    if ((char)plVar2[0x80] != '\0') {
      (**(code **)(*plVar2 + 0x2e8))(plVar2,plVar2[0x7d]);
      (**(code **)(*plVar2 + 0x2e8))(plVar2,plVar2[0x7e]);
      plVar3 = (longlong *)plVar2[0x74];
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0x250))(plVar3,1);
      }
      plVar3 = (longlong *)plVar2[0x75];
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0x250))(plVar3,4);
      }
      plVar3 = (longlong *)plVar2[0x76];
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0x250))(plVar3,1);
      }
      plVar3 = (longlong *)plVar2[0x77];
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0x250))(plVar3,1);
      }
      plVar3 = (longlong *)plVar2[0x78];
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0x250))(plVar3,1);
      }
      plVar3 = (longlong *)plVar2[0x7c];
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0x250))(plVar3,1);
        (**(code **)(*(longlong *)plVar2[0x7b] + 0x250))((longlong *)plVar2[0x7b],0);
      }
      (**(code **)(*plVar2 + 0x2e0))(plVar2,plVar2[0x7f],0,1,0,DAT_1450d6c88);
      *(undefined2 *)(plVar2 + 0x80) = 0;
    }
    return;
  }
  return;
}



