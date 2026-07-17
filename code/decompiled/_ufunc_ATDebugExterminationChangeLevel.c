// UFunction 'ATDebugExterminationChangeLevel' — exec thunk(s) + native impl (via UE native-registration table)
// ---- FUN_141822ae0 @ 141822ae0 ----

void FUN_141822ae0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [2];
  
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
  _guard_check_icall(param_1,local_res10[0]);
  return;
}



// ---- FUN_14204e7f0 @ 14204e7f0 ----

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



// ---- FUN_14181eaf0 @ 14181eaf0 ----

longlong FUN_14181eaf0(void)

{
  if (DAT_1459d5110 == 0) {
    FUN_14203aad0(&DAT_1459d5110,&PTR_FUN_1455627d0);
  }
  return DAT_1459d5110;
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



