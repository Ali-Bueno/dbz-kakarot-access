// D MEDAL / TREASURE taken-state RE - raw Ghidra output (2026-11-19)
// Ghidra 12.0 headless, project code/ghidra/KakarotAT, -readOnly -noanalysis, probe code/ghidra/dmedal_probe.java.
// Each section = one command file + its output verbatim. Findings: dmedal_findings.c

/* ================================ RUN 1 ================================
# run 1: D medal / treasure entry points
ufunc OnSearchStart
ufunc OnSearchEnd
ufunc OnEnableItem
ufunc OnActionChange
ufunc OnChangeProgress
ufunc OnChangePhase
ufunc GetTreasureManager
str TreasureAccessPoint
str TreasureAccessPointSaveTable
str TreasureAccessPointList
*/

######## ufunc OnSearchStart
table @0x14428af80 name@144284fb8 exec -> 14188eea0
// ---- FUN_14188eea0 @ 14188eea0 size=0xef

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14188eea0(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  undefined1 *puVar2;
  longlong lVar3;
  undefined1 auStack_148 [32];
  undefined1 local_128 [248];
  longlong local_30 [4];
  undefined1 local_10 [8];
  
  local_30[3] = DAT_145909780 ^ (ulonglong)auStack_148;
  FUN_140bd77e0(local_128);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_128);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_128);
  }
  puVar2 = local_128;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar2 = *(undefined1 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_140fbe5d0(param_1,puVar2);
  lVar3 = 8;
  puVar2 = local_10;
  do {
    plVar1 = (longlong *)(puVar2 + -0x20);
    puVar2 = puVar2 + -0x20;
    lVar3 = lVar3 + -1;
    if (*plVar1 != 0) {
      FUN_141de9200();
    }
  } while (lVar3 != 0);
  return;
}


  -> call 140bd77e0
// ---- FUN_140bd77e0 @ 140bd77e0 size=0x118

undefined8 * FUN_140bd77e0(undefined8 *param_1)

{
  longlong *plVar1;
  longlong lVar2;
  undefined4 *puVar3;
  longlong lVar4;
  longlong lVar5;
  undefined8 *puVar6;
  undefined8 local_30;
  longlong local_28;
  undefined8 local_20;
  
  puVar6 = param_1 + 1;
  lVar4 = 8;
  *param_1 = 0;
  lVar5 = 8;
  lVar2 = (longlong)param_1 + 0x24;
  do {
    *(undefined4 *)puVar6 = 0;
    *(undefined4 *)(lVar2 + -0x18) = 0;
    FUN_141eb3490(lVar2 + -0x14,&DAT_14391f7d0,1);
    *(undefined8 *)(lVar2 + -0xc) = 0;
    puVar6 = puVar6 + 4;
    *(undefined8 *)(lVar2 + -4) = 0;
    lVar2 = lVar2 + 0x20;
    lVar5 = lVar5 + -1;
  } while (lVar5 != 0);
  puVar3 = (undefined4 *)((longlong)param_1 + 0xc);
  do {
    FUN_141eb3490(&local_30,&DAT_14391f7d0,1);
    plVar1 = (longlong *)(puVar3 + 3);
    puVar3[-1] = 0;
    local_20._4_4_ = 0;
    *(undefined8 *)(puVar3 + 1) = local_30;
    *puVar3 = 0;
    local_28 = 0;
    local_20 = 0;
    if (plVar1 != &local_28) {
      local_20._0_4_ = 0;
      if (*plVar1 != 0) {
        FUN_141de9200();
      }
      *plVar1 = local_28;
      puVar3[5] = (undefined4)local_20;
      puVar3[6] = local_20._4_4_;
    }
    puVar3 = puVar3 + 8;
    lVar4 = lVar4 + -1;
  } while (lVar4 != 0);
  return param_1;
}


  -> call 14204e820
// ---- FUN_14204e820 @ 14204e820 size=0x4b

void FUN_14204e820(longlong param_1,undefined8 param_2,longlong *param_3)

{
  longlong *plVar1;
  longlong *plVar2;
  
  if ((*(uint *)(param_3 + 7) >> 8 & 1) != 0) {
    plVar1 = *(longlong **)(param_1 + 0x78);
    plVar2 = (longlong *)*plVar1;
    while (plVar2 != param_3) {
      plVar1 = (longlong *)plVar1[2];
      plVar2 = (longlong *)*plVar1;
    }
    *(longlong *)(param_1 + 0x38) = plVar1[1];
    return;
  }
  *(longlong *)(param_1 + 0x38) =
       (longlong)*(int *)((longlong)param_3 + 0x44) + *(longlong *)(param_1 + 0x28);
                    /* WARNING: Could not recover jumptable at 0x00014204e864. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_3 + 0x2c8))(param_3);
  return;
}


  -> call 140fbe5d0
// ---- FUN_140fbe5d0 @ 140fbe5d0 size=0x122

void FUN_140fbe5d0(longlong param_1)

{
  longlong lVar1;
  bool bVar2;
  longlong lVar3;
  undefined1 local_res8 [8];
  undefined8 local_28;
  undefined4 local_20;
  undefined8 local_18;
  undefined4 local_10;
  
  lVar1 = *(longlong *)(param_1 + 0xf0);
  if (lVar1 != 0) {
    lVar3 = FUN_141866f60();
    if ((*(int *)(*(longlong *)(lVar1 + 0x10) + 0x90) < *(int *)(lVar3 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar1 + 0x10) + 0x88) + (longlong)*(int *)(lVar3 + 0x90) * 8)
        != lVar3 + 0x88)) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    if (bVar2) {
      FUN_14135cdd0(lVar1,local_res8,0);
      lVar1 = *(longlong *)(lVar1 + 0x168);
      if (lVar1 == 0) {
        local_28 = DAT_145a8af10;
        local_20 = DAT_145a8af18;
      }
      else {
        local_28 = *(undefined8 *)(lVar1 + 0x1a0);
        local_20 = *(undefined4 *)(lVar1 + 0x1a8);
      }
      local_18 = local_28;
      local_10 = local_20;
      FUN_142c7dcb0(*(undefined8 *)(param_1 + 0x108),&local_18,0,0,0);
      FUN_142c7dac0(*(undefined8 *)(param_1 + 0x108),1);
      (**(code **)(**(longlong **)(param_1 + 0x108) + 0x328))(*(longlong **)(param_1 + 0x108),1);
      FUN_140fbae30(param_1);
    }
  }
  FUN_140fc0f90(param_1);
  return;
}


  -> call 141de9200
// ---- FUN_141de9200 @ 141de9200 size=0x34

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


table @0x144292bf0 name@14428d000 exec -> 141892520
// ---- FUN_141892520 @ 141892520 size=0xef

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141892520(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  undefined1 *puVar2;
  longlong lVar3;
  undefined1 auStack_148 [32];
  undefined1 local_128 [248];
  longlong local_30 [4];
  undefined1 local_10 [8];
  
  local_30[3] = DAT_145909780 ^ (ulonglong)auStack_148;
  FUN_140bd77e0(local_128);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_128);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_128);
  }
  puVar2 = local_128;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar2 = *(undefined1 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1410da870(param_1,puVar2);
  lVar3 = 8;
  puVar2 = local_10;
  do {
    plVar1 = (longlong *)(puVar2 + -0x20);
    puVar2 = puVar2 + -0x20;
    lVar3 = lVar3 + -1;
    if (*plVar1 != 0) {
      FUN_141de9200();
    }
  } while (lVar3 != 0);
  return;
}


  -> call 140bd77e0
(already decompiled above: FUN_140bd77e0)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 1410da870
// ---- FUN_1410da870 @ 1410da870 size=0x7c

void FUN_1410da870(longlong param_1)

{
  char cVar1;
  undefined4 uVar2;
  
  cVar1 = FUN_142b1d840();
  if (cVar1 == '\0') {
    uVar2 = *(undefined4 *)(param_1 + 0x508);
  }
  else {
    if (*(int *)(param_1 + 0x514) == 3) {
      *(float *)(param_1 + 0x504) =
           (DAT_1450d6c88 - *(float *)(param_1 + 0x504) / *(float *)(param_1 + 0x50c)) *
           *(float *)(param_1 + 0x508);
      *(undefined4 *)(param_1 + 0x514) = 1;
      return;
    }
    uVar2 = 0;
  }
  *(undefined4 *)(param_1 + 0x504) = uVar2;
  *(undefined4 *)(param_1 + 0x514) = 1;
  return;
}


  -> call 141de9200
(already decompiled above: FUN_141de9200)
table @0x1443104a0 name@14430bda0 exec -> 1418c8700
// ---- FUN_1418c8700 @ 1418c8700 size=0xef

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1418c8700(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  undefined1 *puVar2;
  longlong lVar3;
  undefined1 auStack_148 [32];
  undefined1 local_128 [248];
  longlong local_30 [4];
  undefined1 local_10 [8];
  
  local_30[3] = DAT_145909780 ^ (ulonglong)auStack_148;
  FUN_140bd77e0(local_128);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_128);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_128);
  }
  puVar2 = local_128;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar2 = *(undefined1 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_14118dcf0(param_1,puVar2);
  lVar3 = 8;
  puVar2 = local_10;
  do {
    plVar1 = (longlong *)(puVar2 + -0x20);
    puVar2 = puVar2 + -0x20;
    lVar3 = lVar3 + -1;
    if (*plVar1 != 0) {
      FUN_141de9200();
    }
  } while (lVar3 != 0);
  return;
}


  -> call 140bd77e0
(already decompiled above: FUN_140bd77e0)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14118dcf0
// ---- FUN_14118dcf0 @ 14118dcf0 size=0x51

void FUN_14118dcf0(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  
  iVar1 = 0;
  lVar2 = FUN_140d22f30(DAT_145c95cd8,0);
  if (lVar2 != 0) {
    iVar1 = FUN_140f8aba0(*(undefined8 *)(lVar2 + 0x8e0));
  }
  *(bool *)(*(longlong *)(param_1 + 0x550) + 0x171) = *(int *)(param_1 + 0x5d0) <= iVar1;
  return;
}


  -> call 141de9200
(already decompiled above: FUN_141de9200)
table @0x1443b1548 name@1443b17e0 exec -> 14190dbf0
// ---- FUN_14190dbf0 @ 14190dbf0 size=0xa2

longlong FUN_14190dbf0(void)

{
  if (DAT_145a15a20 == 0) {
    FUN_141f291f0(L"/Script/AT",L"Title",&DAT_145a15a20,_guard_check_icall,0x348,0x10000000,0,
                  L"Engine",&LAB_14190f1b0,FUN_1407e4520,FUN_142b0aa80,&LAB_140885460,&LAB_14083c250
                  ,0);
  }
  return DAT_145a15a20;
}


  -> call 141f291f0
// ---- FUN_141f291f0 @ 141f291f0 size=0x35a

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


table @0x1443b8090 name@1443b6fc8 exec -> 141910cc0
// ---- FUN_141910cc0 @ 141910cc0 size=0xef

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141910cc0(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  undefined1 *puVar2;
  longlong lVar3;
  undefined1 auStack_148 [32];
  undefined1 local_128 [248];
  longlong local_30 [4];
  undefined1 local_10 [8];
  
  local_30[3] = DAT_145909780 ^ (ulonglong)auStack_148;
  FUN_140bd77e0(local_128);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_128);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_128);
  }
  puVar2 = local_128;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar2 = *(undefined1 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1410da8f0(param_1,puVar2);
  lVar3 = 8;
  puVar2 = local_10;
  do {
    plVar1 = (longlong *)(puVar2 + -0x20);
    puVar2 = puVar2 + -0x20;
    lVar3 = lVar3 + -1;
    if (*plVar1 != 0) {
      FUN_141de9200();
    }
  } while (lVar3 != 0);
  return;
}


  -> call 140bd77e0
(already decompiled above: FUN_140bd77e0)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 1410da8f0
// ---- FUN_1410da8f0 @ 1410da8f0 size=0x7c

void FUN_1410da8f0(longlong param_1)

{
  char cVar1;
  undefined4 uVar2;
  
  cVar1 = FUN_142b1d840();
  if (cVar1 == '\0') {
    uVar2 = *(undefined4 *)(param_1 + 0x544);
  }
  else {
    if (*(int *)(param_1 + 0x550) == 3) {
      *(float *)(param_1 + 0x540) =
           (DAT_1450d6c88 - *(float *)(param_1 + 0x540) / *(float *)(param_1 + 0x548)) *
           *(float *)(param_1 + 0x544);
      *(undefined4 *)(param_1 + 0x550) = 1;
      return;
    }
    uVar2 = 0;
  }
  *(undefined4 *)(param_1 + 0x540) = uVar2;
  *(undefined4 *)(param_1 + 0x550) = 1;
  return;
}


  -> call 141de9200
(already decompiled above: FUN_141de9200)

######## ufunc OnSearchEnd
table @0x14428af70 name@144284e00 exec -> 14188edb0
// ---- FUN_14188edb0 @ 14188edb0 size=0xef

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14188edb0(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  undefined1 *puVar2;
  longlong lVar3;
  undefined1 auStack_148 [32];
  undefined1 local_128 [248];
  longlong local_30 [4];
  undefined1 local_10 [8];
  
  local_30[3] = DAT_145909780 ^ (ulonglong)auStack_148;
  FUN_140bd77e0(local_128);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_128);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_128);
  }
  puVar2 = local_128;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar2 = *(undefined1 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_140fbe4d0(param_1,puVar2);
  lVar3 = 8;
  puVar2 = local_10;
  do {
    plVar1 = (longlong *)(puVar2 + -0x20);
    puVar2 = puVar2 + -0x20;
    lVar3 = lVar3 + -1;
    if (*plVar1 != 0) {
      FUN_141de9200();
    }
  } while (lVar3 != 0);
  return;
}


  -> call 140bd77e0
(already decompiled above: FUN_140bd77e0)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 140fbe4d0
// ---- FUN_140fbe4d0 @ 140fbe4d0 size=0xf7

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140fbe4d0(longlong param_1)

{
  bool bVar1;
  longlong lVar2;
  longlong lVar3;
  undefined1 auStack_a8 [32];
  undefined1 local_88;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 local_70;
  undefined4 local_60;
  undefined1 local_58 [16];
  undefined1 local_48 [48];
  ulonglong local_18;
  
  local_18 = DAT_145909780 ^ (ulonglong)auStack_a8;
  FUN_140fc0f10();
  lVar3 = *(longlong *)(param_1 + 0xf0);
  if (lVar3 != 0) {
    lVar2 = FUN_141866f60();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar2 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar2 + 0x90) * 8)
        != lVar2 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) {
      FUN_14135cdd0(lVar3,local_58,0);
      lVar3 = FUN_142c6ea50(*(undefined8 *)(lVar3 + 0x168),local_48);
      local_88 = 0;
      local_78 = *(undefined4 *)(lVar3 + 0x10);
      uStack_74 = *(undefined4 *)(lVar3 + 0x14);
      local_70 = *(undefined4 *)(lVar3 + 0x18);
      local_60 = local_70;
      FUN_142c7dcb0(*(undefined8 *)(param_1 + 0x108),&local_78,0,0);
    }
  }
  return;
}


  -> call 141de9200
(already decompiled above: FUN_141de9200)
table @0x144285310 name@1442853d8 exec -> 14188b650
// ---- FUN_14188b650 @ 14188b650 size=0x2f

longlong FUN_14188b650(void)

{
  if (DAT_1459f21a0 == 0) {
    FUN_14203aad0(&DAT_1459f21a0,&PTR_FUN_1455d77e0);
  }
  return DAT_1459f21a0;
}


  -> call 14203aad0
// ---- FUN_14203aad0 @ 14203aad0 size=0x257

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


table @0x144292be0 name@14428cff0 exec -> 141892430
// ---- FUN_141892430 @ 141892430 size=0xef

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141892430(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  undefined1 *puVar2;
  longlong lVar3;
  undefined1 auStack_148 [32];
  undefined1 local_128 [248];
  longlong local_30 [4];
  undefined1 local_10 [8];
  
  local_30[3] = DAT_145909780 ^ (ulonglong)auStack_148;
  FUN_140bd77e0(local_128);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_128);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_128);
  }
  puVar2 = local_128;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar2 = *(undefined1 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1410da770(param_1,puVar2);
  lVar3 = 8;
  puVar2 = local_10;
  do {
    plVar1 = (longlong *)(puVar2 + -0x20);
    puVar2 = puVar2 + -0x20;
    lVar3 = lVar3 + -1;
    if (*plVar1 != 0) {
      FUN_141de9200();
    }
  } while (lVar3 != 0);
  return;
}


  -> call 140bd77e0
(already decompiled above: FUN_140bd77e0)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 1410da770
// ---- FUN_1410da770 @ 1410da770 size=0x7c

void FUN_1410da770(longlong param_1)

{
  char cVar1;
  undefined4 uVar2;
  
  cVar1 = FUN_142b1d840();
  if (cVar1 == '\0') {
    uVar2 = *(undefined4 *)(param_1 + 0x50c);
  }
  else {
    if (*(int *)(param_1 + 0x514) == 1) {
      *(float *)(param_1 + 0x504) =
           (DAT_1450d6c88 - *(float *)(param_1 + 0x504) / *(float *)(param_1 + 0x508)) *
           *(float *)(param_1 + 0x50c);
      *(undefined4 *)(param_1 + 0x514) = 3;
      return;
    }
    uVar2 = 0;
  }
  *(undefined4 *)(param_1 + 0x504) = uVar2;
  *(undefined4 *)(param_1 + 0x514) = 3;
  return;
}


  -> call 141de9200
(already decompiled above: FUN_141de9200)
table @0x14428dfb8 name@14428e800 exec -> 14188f7c0
// ---- FUN_14188f7c0 @ 14188f7c0 size=0x2f

longlong FUN_14188f7c0(void)

{
  if (DAT_1459f31e0 == 0) {
    FUN_14203aad0(&DAT_1459f31e0,&PTR_FUN_1455d8850);
  }
  return DAT_1459f31e0;
}


  -> call 14203aad0
(already decompiled above: FUN_14203aad0)
table @0x144310490 name@14430bd90 exec -> 1418c8610
// ---- FUN_1418c8610 @ 1418c8610 size=0xef

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1418c8610(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  undefined1 *puVar2;
  longlong lVar3;
  undefined1 auStack_148 [32];
  undefined1 local_128 [248];
  longlong local_30 [4];
  undefined1 local_10 [8];
  
  local_30[3] = DAT_145909780 ^ (ulonglong)auStack_148;
  FUN_140bd77e0(local_128);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_128);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_128);
  }
  puVar2 = local_128;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar2 = *(undefined1 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  _guard_check_icall(param_1,puVar2);
  lVar3 = 8;
  puVar2 = local_10;
  do {
    plVar1 = (longlong *)(puVar2 + -0x20);
    puVar2 = puVar2 + -0x20;
    lVar3 = lVar3 + -1;
    if (*plVar1 != 0) {
      FUN_141de9200();
    }
  } while (lVar3 != 0);
  return;
}


  -> call 140bd77e0
(already decompiled above: FUN_140bd77e0)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 141de9200
(already decompiled above: FUN_141de9200)
table @0x14430e7b8 name@14430ea58 exec -> 1418c6100
// ---- FUN_1418c6100 @ 1418c6100 size=0x2f

longlong FUN_1418c6100(void)

{
  if (DAT_145a00c38 == 0) {
    FUN_14203aad0(&DAT_145a00c38,&PTR_FUN_145610900);
  }
  return DAT_145a00c38;
}


  -> call 14203aad0
(already decompiled above: FUN_14203aad0)
table @0x1443b1538 name@1443b17d0 exec -> 14190ef20
// ---- FUN_14190ef20 @ 14190ef20 size=0x2f

longlong FUN_14190ef20(void)

{
  if (DAT_145a15bb8 == 0) {
    FUN_14203aad0(&DAT_145a15bb8,&PTR_FUN_1456635d0);
  }
  return DAT_145a15bb8;
}


  -> call 14203aad0
(already decompiled above: FUN_14203aad0)
table @0x1443b8080 name@1443b6fb8 exec -> 141910bd0
// ---- FUN_141910bd0 @ 141910bd0 size=0xef

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141910bd0(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  undefined1 *puVar2;
  longlong lVar3;
  undefined1 auStack_148 [32];
  undefined1 local_128 [248];
  longlong local_30 [4];
  undefined1 local_10 [8];
  
  local_30[3] = DAT_145909780 ^ (ulonglong)auStack_148;
  FUN_140bd77e0(local_128);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_128);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_128);
  }
  puVar2 = local_128;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar2 = *(undefined1 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1410da7f0(param_1,puVar2);
  lVar3 = 8;
  puVar2 = local_10;
  do {
    plVar1 = (longlong *)(puVar2 + -0x20);
    puVar2 = puVar2 + -0x20;
    lVar3 = lVar3 + -1;
    if (*plVar1 != 0) {
      FUN_141de9200();
    }
  } while (lVar3 != 0);
  return;
}


  -> call 140bd77e0
(already decompiled above: FUN_140bd77e0)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 1410da7f0
// ---- FUN_1410da7f0 @ 1410da7f0 size=0x7c

void FUN_1410da7f0(longlong param_1)

{
  char cVar1;
  undefined4 uVar2;
  
  cVar1 = FUN_142b1d840();
  if (cVar1 == '\0') {
    uVar2 = *(undefined4 *)(param_1 + 0x548);
  }
  else {
    if (*(int *)(param_1 + 0x550) == 1) {
      *(float *)(param_1 + 0x540) =
           (DAT_1450d6c88 - *(float *)(param_1 + 0x540) / *(float *)(param_1 + 0x544)) *
           *(float *)(param_1 + 0x548);
      *(undefined4 *)(param_1 + 0x550) = 3;
      return;
    }
    uVar2 = 0;
  }
  *(undefined4 *)(param_1 + 0x540) = uVar2;
  *(undefined4 *)(param_1 + 0x550) = 3;
  return;
}


  -> call 141de9200
(already decompiled above: FUN_141de9200)

######## ufunc OnEnableItem
table @0x1443b1528 name@1443b17c0 exec -> 14190eef0
// ---- FUN_14190eef0 @ 14190eef0 size=0x2f

longlong FUN_14190eef0(void)

{
  if (DAT_145a15bb0 == 0) {
    FUN_14203aad0(&DAT_145a15bb0,&PTR_FUN_145663550);
  }
  return DAT_145a15bb0;
}


  -> call 14203aad0
(already decompiled above: FUN_14203aad0)
table @0x1443b8070 name@1443b6fa8 exec -> 141910550
// ---- FUN_141910550 @ 141910550 size=0x82

void FUN_141910550(undefined8 param_1,longlong param_2)

{
  int local_res10 [2];
  
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
  FUN_1410d8790(param_1,local_res10[0] != 0);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 1410d8790
// ---- FUN_1410d8790 @ 1410d8790 size=0x119

void FUN_1410d8790(longlong param_1,byte param_2)

{
  char cVar1;
  undefined8 uVar2;
  longlong *plVar3;
  uint uVar4;
  
  uVar4 = (uint)param_2;
  if (*(uint *)(param_1 + 0x578) != uVar4) {
    uVar2 = FUN_142058d50(param_1 + 0x500);
    FUN_140d20b20(uVar4,uVar2);
    FUN_140d20b20(param_2,*(undefined8 *)(param_1 + 0x518));
    FUN_140d20b20(param_2,*(undefined8 *)(param_1 + 0x4e0));
    plVar3 = *(longlong **)(param_1 + 0x340);
    if (param_2 == 0) {
      (**(code **)(*plVar3 + 0x5e8))(plVar3,DAT_145c68c10);
      cVar1 = FUN_14205a760(param_1 + 0x500);
      if (cVar1 != '\0') {
        uVar2 = FUN_142058d50(param_1 + 0x500);
        FUN_142c7dac0(uVar2,0,1);
        plVar3 = (longlong *)FUN_142058d50(param_1 + 0x500);
        (**(code **)(*plVar3 + 0x328))(plVar3,0);
      }
    }
    else {
      (**(code **)(*plVar3 + 0x5e8))(plVar3,*(undefined8 *)(param_1 + 0x3b8));
      FUN_1410dd600(param_1,1);
    }
    *(uint *)(param_1 + 0x578) = uVar4;
    FUN_141a755e0(param_1,param_2);
    FUN_1430f2150(param_1 + 0x38,param_2);
    FUN_140d18360(param_1 + 0x400);
  }
  return;
}



######## ufunc OnActionChange
table @0x143fa5be0 name@143fa1398 exec -> 141767ab0
// ---- FUN_141767ab0 @ 141767ab0 size=0xfc

void FUN_141767ab0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [2];
  undefined4 local_res20 [2];
  undefined8 local_18 [2];
  
  local_18[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_18);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_18);
  }
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
  FUN_140f43830(param_1,local_18[0],local_res20[0],local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 140f43830
// ---- FUN_140f43830 @ 140f43830 size=0x16e

void FUN_140f43830(longlong param_1,longlong param_2,undefined8 param_3,int param_4)

{
  longlong lVar1;
  longlong lVar2;
  wchar_t *pwVar3;
  wchar_t *pwVar4;
  wchar_t *pwVar5;
  longlong lVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  longlong lVar10;
  longlong *plVar11;
  longlong local_res10;
  
  if (param_4 != 0xb7) {
    *(undefined2 *)(param_1 + 0x428) = 0x101;
    *(undefined1 *)(param_1 + 0x42a) = 1;
  }
  if ((param_2 != 0) && (lVar1 = *(longlong *)(param_2 + 0x910), lVar1 != 0)) {
    pwVar5 = wcsstr(L"&AAT_MobAnimalBase::OnActionChange",L"::");
    pwVar4 = (wchar_t *)0x0;
    while (pwVar3 = pwVar5, pwVar3 != (wchar_t *)0x0) {
      pwVar5 = wcsstr(pwVar3 + 1,L"::");
      pwVar4 = pwVar3;
    }
    FUN_141eb3490(&local_res10,pwVar4 + 2,1);
    plVar11 = (longlong *)(lVar1 + 0x138);
    lVar2 = *plVar11;
    iVar9 = *(int *)(lVar1 + 0x140);
    for (lVar10 = lVar2; lVar10 != (longlong)iVar9 * 0x10 + lVar2; lVar10 = lVar10 + 0x10) {
      if (((*(longlong *)(lVar10 + 8) == local_res10) && (param_1 != 0)) &&
         (lVar6 = FUN_142059030(lVar10), param_1 == lVar6)) {
        iVar9 = (int)(lVar10 - lVar2 >> 4);
        if (iVar9 != -1) {
          iVar8 = *(int *)(lVar1 + 0x140);
          iVar7 = (iVar8 - iVar9) + -1;
          if (0 < iVar7) {
            iVar7 = 1;
          }
          if (iVar7 != 0) {
            memcpy((void *)((longlong)iVar9 * 0x10 + *plVar11),
                   (void *)((longlong)(iVar8 - iVar7) * 0x10 + *plVar11),(longlong)iVar7 << 4);
            iVar8 = *(int *)(lVar1 + 0x140);
          }
          *(int *)(lVar1 + 0x140) = iVar8 + -1;
        }
        break;
      }
    }
    FUN_140bdefe0(plVar11);
  }
  return;
}


table @0x1441cef58 name@1441cf298 exec -> 1418438a0
// ---- FUN_1418438a0 @ 1418438a0 size=0x2f

longlong FUN_1418438a0(void)

{
  if (DAT_1459dc938 == 0) {
    FUN_14203aad0(&DAT_1459dc938,&PTR_FUN_14558b8f0);
  }
  return DAT_1459dc938;
}


  -> call 14203aad0
(already decompiled above: FUN_14203aad0)
table @0x1441d5ad0 name@1441d4000 exec -> 1418451c0
// ---- FUN_1418451c0 @ 1418451c0 size=0xfc

void FUN_1418451c0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [2];
  undefined4 local_res20 [2];
  undefined8 local_18 [2];
  
  local_18[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_18);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_18);
  }
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
  FUN_1410fdc50(param_1,local_18[0],local_res20[0],local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
table @0x1441d76b8 name@1441d78f0 exec -> 141847470
// ---- FUN_141847470 @ 141847470 size=0x2f

longlong FUN_141847470(void)

{
  if (DAT_1459de060 == 0) {
    FUN_14203aad0(&DAT_1459de060,&PTR_FUN_14558f030);
  }
  return DAT_1459de060;
}


  -> call 14203aad0
(already decompiled above: FUN_14203aad0)
table @0x1441e1930 name@1441d9a78 exec -> 14184a6e0
// ---- FUN_14184a6e0 @ 14184a6e0 size=0xfc

void FUN_14184a6e0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [2];
  undefined4 local_res20 [2];
  undefined8 local_18 [2];
  
  local_18[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_18);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_18);
  }
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
  FUN_1410adff0(param_1,local_18[0],local_res20[0],local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
table @0x1441e1920 name@1441df4b0 exec -> 14184a7e0
// ---- FUN_14184a7e0 @ 14184a7e0 size=0xfc

void FUN_14184a7e0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [2];
  undefined4 local_res20 [2];
  undefined8 local_18 [2];
  
  local_18[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_18);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_18);
  }
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
  FUN_1410ae000(param_1,local_18[0],local_res20[0],local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
table @0x1441e1ab0 name@1441df800 exec -> 14184a8e0
// ---- FUN_14184a8e0 @ 14184a8e0 size=0xfc

void FUN_14184a8e0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [2];
  undefined4 local_res20 [2];
  undefined8 local_18 [2];
  
  local_18[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_18);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_18);
  }
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
  FUN_1410d7410(param_1,local_18[0],local_res20[0],local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 1410d7410
// ---- FUN_1410d7410 @ 1410d7410 size=0x82

void FUN_1410d7410(longlong param_1,undefined8 param_2)

{
  char *pcVar1;
  char *pcVar2;
  
  if (*(char *)(param_1 + 0x60) != '\0') {
    pcVar1 = *(char **)(param_1 + 0x28);
    pcVar2 = pcVar1 + (longlong)*(int *)(param_1 + 0x30) * 0x30;
    for (; pcVar1 != pcVar2; pcVar1 = pcVar1 + 0x30) {
      if (*pcVar1 != '\0') {
        (**(code **)(&DAT_143c9c780 + (longlong)*pcVar1 * 8))(param_1,pcVar1,3,param_2);
      }
    }
  }
  return;
}


table @0x1442c8860 name@1442c7698 exec -> 1418a21a0
// ---- FUN_1418a21a0 @ 1418a21a0 size=0xfc

void FUN_1418a21a0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [2];
  undefined4 local_res20 [2];
  undefined8 local_18 [2];
  
  local_18[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_18);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_18);
  }
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
  FUN_140be9240(param_1,local_18[0],local_res20[0],local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 140be9240
// ---- FUN_140be9240 @ 140be9240 size=0x225

void FUN_140be9240(longlong param_1,longlong param_2,int param_3,int param_4)

{
  longlong lVar1;
  wchar_t *pwVar2;
  wchar_t *pwVar3;
  wchar_t *pwVar4;
  longlong lVar5;
  longlong lVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  longlong lVar10;
  longlong *plVar11;
  longlong local_res8;
  undefined4 *local_48;
  undefined8 local_40;
  undefined8 local_38;
  undefined8 uStack_30;
  
  if ((param_3 == 0xb8) && (param_4 == 0xba)) {
    *(undefined1 *)(param_1 + 0x368) = 1;
  }
  else if ((param_4 - 0xb8U & 0xfffffffd) != 0) {
    *(undefined2 *)(param_1 + 0x368) = 0x101;
    *(undefined1 *)(param_1 + 0x36a) = 1;
  }
  if ((*(char *)(param_1 + 0x368) != '\0') && (param_2 != 0)) {
    lVar6 = *(longlong *)(param_2 + 0x910);
    if (lVar6 != 0) {
      uStack_30 = 0;
      FUN_141eb3490(&local_res8,L"ActionPointGet",1);
      local_38 = local_res8;
      local_48 = (undefined4 *)0x0;
      local_40 = 1;
      FUN_1407f1790(&local_48,0);
      *local_48 = (undefined4)local_38;
      local_48[1] = local_38._4_4_;
      local_48[2] = (undefined4)uStack_30;
      local_48[3] = uStack_30._4_4_;
      FUN_140dd2810(lVar6,&local_48);
      pwVar4 = wcsstr(L"&ADragonBallStaticActor::OnActionChange",L"::");
      pwVar3 = (wchar_t *)0x0;
      while (pwVar2 = pwVar4, pwVar2 != (wchar_t *)0x0) {
        pwVar4 = wcsstr(pwVar2 + 1,L"::");
        pwVar3 = pwVar2;
      }
      FUN_141eb3490(&local_res8,pwVar3 + 2,1);
      plVar11 = (longlong *)(lVar6 + 0x138);
      lVar1 = *plVar11;
      iVar9 = *(int *)(lVar6 + 0x140);
      for (lVar10 = lVar1; lVar10 != (longlong)iVar9 * 0x10 + lVar1; lVar10 = lVar10 + 0x10) {
        if ((*(longlong *)(lVar10 + 8) == local_res8) &&
           (lVar5 = FUN_142059030(lVar10), param_1 == lVar5)) {
          iVar9 = (int)(lVar10 - lVar1 >> 4);
          if (iVar9 != -1) {
            iVar8 = *(int *)(lVar6 + 0x140);
            iVar7 = (iVar8 - iVar9) + -1;
            if (0 < iVar7) {
              iVar7 = 1;
            }
            if (iVar7 != 0) {
              memcpy((void *)((longlong)iVar9 * 0x10 + *plVar11),
                     (void *)((longlong)(iVar8 - iVar7) * 0x10 + *plVar11),(longlong)iVar7 << 4);
              iVar8 = *(int *)(lVar6 + 0x140);
            }
            *(int *)(lVar6 + 0x140) = iVar8 + -1;
          }
          break;
        }
      }
      FUN_140bdefe0(plVar11);
      if (local_48 != (undefined4 *)0x0) {
        FUN_141de9200();
      }
    }
    if ((*(char *)(param_1 + 0x36a) == '\0') && (*(char *)(param_1 + 0x3a0) != '\0')) {
      lVar6 = FUN_140d239b0();
      if (*(longlong *)(lVar6 + 0x1180) != 0) {
        FUN_140f61b80(*(longlong *)(lVar6 + 0x1180),1);
      }
    }
  }
  return;
}


table @0x1442f8908 name@1442f5f28 exec -> 1418bc8e0
// ---- FUN_1418bc8e0 @ 1418bc8e0 size=0xfc

void FUN_1418bc8e0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [2];
  undefined4 local_res20 [2];
  undefined8 local_18 [2];
  
  local_18[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_18);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_18);
  }
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
  FUN_140be9470(param_1,local_18[0],local_res20[0],local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 140be9470
// ---- FUN_140be9470 @ 140be9470 size=0xd5

void FUN_140be9470(longlong param_1,longlong param_2,undefined8 param_3,int param_4)

{
  longlong lVar1;
  wchar_t *pwVar2;
  wchar_t *pwVar3;
  wchar_t *pwVar4;
  undefined8 local_res8;
  code *local_18;
  ulonglong uStack_10;
  
  if (param_4 != (*(char *)(param_1 + 0x38c) != '\0') + 0xb4) {
    *(undefined1 *)(param_1 + 0x358) = 1;
  }
  if ((param_2 != 0) && (lVar1 = *(longlong *)(param_2 + 0x910), lVar1 != 0)) {
    uStack_10 = uStack_10 & 0xffffffff00000000;
    local_18 = FUN_140be9470;
    pwVar4 = wcsstr(L"&AFishingStartPoint::OnActionChange",L"::");
    pwVar3 = (wchar_t *)0x0;
    while (pwVar2 = pwVar4, pwVar2 != (wchar_t *)0x0) {
      pwVar4 = wcsstr(pwVar2 + 1,L"::");
      pwVar3 = pwVar2;
    }
    FUN_141eb3490(&local_res8,pwVar3 + 2,1);
    FUN_140bd5c20(lVar1 + 0x138,param_1,&local_18,local_res8);
  }
  return;
}


table @0x14431c660 name@1443163e8 exec -> 1418cc690
// ---- FUN_1418cc690 @ 1418cc690 size=0xfc

void FUN_1418cc690(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [2];
  undefined4 local_res20 [2];
  undefined8 local_18 [2];
  
  local_18[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_18);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_18);
  }
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
  FUN_14118b020(param_1,local_18[0],local_res20[0],local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14118b020
// ---- FUN_14118b020 @ 14118b020 size=0x181

void FUN_14118b020(longlong *param_1,longlong param_2,int param_3,int param_4)

{
  longlong lVar1;
  longlong lVar2;
  wchar_t *pwVar3;
  wchar_t *pwVar4;
  int iVar5;
  wchar_t *pwVar6;
  longlong *plVar7;
  int iVar8;
  int iVar9;
  longlong lVar10;
  longlong *plVar11;
  longlong local_28 [2];
  
  if ((param_3 != 0xb8) || (iVar5 = (**(code **)(*param_1 + 0x670))(), param_4 != iVar5)) {
    if (param_4 == 0xb8) {
      return;
    }
    iVar5 = (**(code **)(*param_1 + 0x670))(param_1);
    if (param_4 == iVar5) {
      return;
    }
    *(undefined1 *)(param_1 + 0x7c) = 1;
    *(undefined2 *)((longlong)param_1 + 0x3e2) = 0x101;
  }
  if ((param_2 != 0) && (lVar1 = *(longlong *)(param_2 + 0x910), lVar1 != 0)) {
    pwVar6 = wcsstr(L"&AMineralMiningPointNormal::OnActionChange",L"::");
    pwVar4 = (wchar_t *)0x0;
    while (pwVar3 = pwVar6, pwVar3 != (wchar_t *)0x0) {
      pwVar6 = wcsstr(pwVar3 + 1,L"::");
      pwVar4 = pwVar3;
    }
    FUN_141eb3490(local_28,pwVar4 + 2,1);
    plVar11 = (longlong *)(lVar1 + 0x138);
    lVar2 = *plVar11;
    iVar5 = *(int *)(lVar1 + 0x140);
    for (lVar10 = lVar2; lVar10 != (longlong)iVar5 * 0x10 + lVar2; lVar10 = lVar10 + 0x10) {
      if ((*(longlong *)(lVar10 + 8) == local_28[0]) &&
         (plVar7 = (longlong *)FUN_142059030(lVar10), param_1 == plVar7)) {
        iVar5 = (int)(lVar10 - lVar2 >> 4);
        if (iVar5 != -1) {
          iVar9 = *(int *)(lVar1 + 0x140);
          iVar8 = (iVar9 - iVar5) + -1;
          if (0 < iVar8) {
            iVar8 = 1;
          }
          if (iVar8 != 0) {
            memcpy((void *)((longlong)iVar5 * 0x10 + *plVar11),
                   (void *)((longlong)(iVar9 - iVar8) * 0x10 + *plVar11),(longlong)iVar8 << 4);
            iVar9 = *(int *)(lVar1 + 0x140);
          }
          *(int *)(lVar1 + 0x140) = iVar9 + -1;
        }
        break;
      }
    }
    FUN_140bdefe0(plVar11);
  }
  return;
}


table @0x144316888 name@144316a48 exec -> 1418c9250
// ---- FUN_1418c9250 @ 1418c9250 size=0x2f

longlong FUN_1418c9250(void)

{
  if (DAT_145a02300 == 0) {
    FUN_14203aad0(&DAT_145a02300,&PTR_FUN_1456131a0);
  }
  return DAT_145a02300;
}


  -> call 14203aad0
(already decompiled above: FUN_14203aad0)
table @0x1443b1518 name@1443b16f8 exec -> 14190eec0
// ---- FUN_14190eec0 @ 14190eec0 size=0x2f

longlong FUN_14190eec0(void)

{
  if (DAT_145a15ba8 == 0) {
    FUN_14203aad0(&DAT_145a15ba8,&PTR_FUN_1456634d0);
  }
  return DAT_145a15ba8;
}


  -> call 14203aad0
(already decompiled above: FUN_14203aad0)
table @0x1443b8060 name@1443b6f88 exec -> 14190ffb0
// ---- FUN_14190ffb0 @ 14190ffb0 size=0xfc

void FUN_14190ffb0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [2];
  undefined4 local_res20 [2];
  undefined8 local_18 [2];
  
  local_18[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_18);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_18);
  }
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
  FUN_1410d6f60(param_1,local_18[0],local_res20[0],local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 1410d6f60
// ---- FUN_1410d6f60 @ 1410d6f60 size=0x4a5

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1410d6f60(longlong *param_1,longlong param_2,int param_3,int param_4)

{
  int iVar1;
  longlong lVar2;
  undefined4 uVar3;
  wchar_t *pwVar4;
  wchar_t *pwVar5;
  char cVar6;
  int iVar7;
  wchar_t *pwVar8;
  longlong *plVar9;
  undefined8 uVar10;
  longlong lVar11;
  longlong lVar12;
  int iVar13;
  longlong lVar14;
  longlong *plVar15;
  undefined4 local_res8;
  undefined4 uStackX_c;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined4 *local_78;
  ulonglong local_70;
  longlong local_68;
  longlong lStack_60;
  undefined1 local_58 [16];
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  
  if ((param_3 != 0xb8) || (iVar7 = (**(code **)(*param_1 + 0x670))(), param_4 != iVar7)) {
    if ((param_4 == 0xb8) || (iVar7 = (**(code **)(*param_1 + 0x670))(param_1), param_4 == iVar7))
    goto LAB_1410d6fb9;
    *(undefined2 *)((longlong)param_1 + 0x3e2) = 0x101;
  }
  *(undefined1 *)(param_1 + 0x7c) = 1;
LAB_1410d6fb9:
  if (((char)param_1[0x7c] != '\0') && (param_2 != 0)) {
    FUN_141353fe0(0x13,7);
    lVar11 = *(longlong *)(param_2 + 0x910);
    if (lVar11 != 0) {
      uStack_80 = 0;
      FUN_141eb3490(&local_res8,L"ActionPointGet",1);
      local_88 = CONCAT44(uStackX_c,local_res8);
      local_78 = (undefined4 *)0x0;
      local_70 = 1;
      FUN_1407f1790(&local_78,0);
      *local_78 = (undefined4)local_88;
      local_78[1] = local_88._4_4_;
      local_78[2] = (undefined4)uStack_80;
      local_78[3] = uStack_80._4_4_;
      FUN_140dd2810(lVar11,&local_78);
      pwVar8 = wcsstr(L"&ATreasureAccessPoint::OnActionChange",L"::");
      pwVar5 = (wchar_t *)0x0;
      while (pwVar4 = pwVar8, pwVar4 != (wchar_t *)0x0) {
        pwVar8 = wcsstr(pwVar4 + 1,L"::");
        pwVar5 = pwVar4;
      }
      FUN_141eb3490(&local_res8,pwVar5 + 2,1);
      lVar2 = CONCAT44(uStackX_c,local_res8);
      plVar15 = (longlong *)(lVar11 + 0x138);
      lVar12 = *plVar15;
      iVar7 = *(int *)(lVar11 + 0x140);
      for (lVar14 = lVar12; lVar14 != (longlong)iVar7 * 0x10 + lVar12; lVar14 = lVar14 + 0x10) {
        if ((*(longlong *)(lVar14 + 8) == lVar2) &&
           (plVar9 = (longlong *)FUN_142059030(lVar14), param_1 == plVar9)) {
          iVar7 = (int)(lVar14 - lVar12 >> 4);
          if (iVar7 != -1) {
            iVar1 = *(int *)(lVar11 + 0x140);
            iVar13 = (iVar1 - iVar7) + -1;
            if (0 < iVar13) {
              iVar13 = 1;
            }
            if (iVar13 != 0) {
              memcpy((void *)((longlong)iVar7 * 0x10 + *plVar15),
                     (void *)((longlong)(iVar1 - iVar13) * 0x10 + *plVar15),(longlong)iVar13 << 4);
              iVar1 = *(int *)(lVar11 + 0x140);
            }
            *(int *)(lVar11 + 0x140) = iVar1 + -1;
          }
          break;
        }
      }
      FUN_140bdefe0(plVar15);
      if (local_78 != (undefined4 *)0x0) {
        FUN_141de9200();
      }
    }
    if (*(char *)((longlong)param_1 + 0x3e3) == '\0') {
      lVar11 = param_1[0xa2];
      if (lVar11 != 0) {
        lVar12 = param_1[0x2d];
        if (lVar12 == 0) {
          local_88 = DAT_145a8af10;
          uVar3 = DAT_145a8af18;
        }
        else {
          local_88 = *(undefined8 *)(lVar12 + 0x1a0);
          uVar3 = *(undefined4 *)(lVar12 + 0x1a8);
        }
        local_68 = _DAT_1456d9da0;
        lStack_60 = _DAT_1456d9da8;
        uStack_80 = CONCAT44(uStack_80._4_4_,uVar3);
        local_58._8_4_ = uVar3;
        local_58._0_8_ = local_88;
        local_58._12_4_ = 0;
        local_48 = _DAT_143973ef0;
        uStack_44 = _UNK_143973ef4;
        uStack_40 = _UNK_143973ef8;
        uStack_3c = _UNK_143973efc;
        uVar10 = (**(code **)(*param_1 + 0x148))(param_1);
        lVar11 = FUN_142d2f820(uVar10,lVar11,&local_68,0,0);
        lVar12 = FUN_140b40c10(0x170);
        if ((lVar12 != 0) &&
           (plVar15 = (longlong *)FUN_1411a0510(lVar12,param_1), plVar15 != (longlong *)0x0)) {
          plVar15[0xe] = local_68;
          plVar15[0xf] = lStack_60;
          *(undefined4 *)(plVar15 + 0x10) = local_58._0_4_;
          *(undefined4 *)((longlong)plVar15 + 0x84) = local_58._4_4_;
          *(undefined4 *)(plVar15 + 0x11) = local_58._8_4_;
          *(undefined4 *)((longlong)plVar15 + 0x8c) = local_58._12_4_;
          *(undefined4 *)(plVar15 + 0x12) = local_48;
          *(undefined4 *)((longlong)plVar15 + 0x94) = uStack_44;
          *(undefined4 *)(plVar15 + 0x13) = uStack_40;
          *(undefined4 *)((longlong)plVar15 + 0x9c) = uStack_3c;
          if (lVar11 != 0) {
            FUN_1420575e0(plVar15 + 0x23,lVar11);
          }
          iVar7 = (int)param_1[0x7e];
          *(int *)(param_1 + 0x7e) = iVar7 + 1;
          if (*(int *)((longlong)param_1 + 0x3f4) < iVar7 + 1) {
            FUN_1407f1510(param_1 + 0x7d,iVar7);
          }
          uStack_80 = CONCAT44(uStack_80._4_4_,DAT_143aaf3fc);
          local_88 = 0;
          *(longlong **)(param_1[0x7d] + (longlong)iVar7 * 8) = plVar15;
          (**(code **)(*plVar15 + 0x18))(plVar15,param_2,&local_88);
          FUN_1411c5000(plVar15);
          FUN_141485fc0(0x54,0,0,0,0);
        }
      }
      plVar15 = (longlong *)param_1[0x9e];
      if (plVar15 != (longlong *)0x0) {
        local_70 = local_70 & 0xffffffff00000000;
        uStack_80 = uStack_80 & 0xffffffff00000000;
        local_88 = 0;
        (**(code **)(*plVar15 + 0x3c8))(plVar15,&local_88);
      }
      cVar6 = FUN_14205a760(param_1 + 0xa0);
      if (cVar6 != '\0') {
        plVar15 = (longlong *)FUN_142058d50(param_1 + 0xa0);
        (**(code **)(*plVar15 + 0x380))(plVar15,0);
        FUN_1420575e0(&local_res8,0);
        *(undefined4 *)(param_1 + 0xa0) = local_res8;
        *(undefined4 *)((longlong)param_1 + 0x504) = uStackX_c;
      }
      if (*(char *)((longlong)param_1 + 0x561) != '\0') {
        lVar11 = FUN_140d239b0();
        lVar11 = *(longlong *)(lVar11 + 0x1180);
        if (lVar11 != 0) {
          if (*(char *)(lVar11 + 0x2ec) != '\0') {
            *(undefined1 *)(lVar11 + 0x2ec) = 0;
            *(undefined1 *)((longlong)param_1 + 0x562) = 1;
          }
          FUN_140f6ca20(lVar11);
          FUN_140f61a00(lVar11);
          FUN_140f61b80(lVar11,1);
        }
      }
    }
  }
  return;
}


table @0x14447e610 name@14447ac68 exec -> 1419fcad0
// ---- FUN_1419fcad0 @ 1419fcad0 size=0xfc

void FUN_1419fcad0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [2];
  undefined4 local_res20 [2];
  undefined8 local_18 [2];
  
  local_18[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_18);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_18);
  }
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
  FUN_141971fa0(param_1,local_18[0],local_res20[0],local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 141971fa0
// ---- FUN_141971fa0 @ 141971fa0 size=0x489

void FUN_141971fa0(longlong param_1,longlong param_2,int param_3,int param_4)

{
  longlong lVar1;
  bool bVar2;
  char cVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined1 local_res8 [8];
  float local_68;
  float local_64;
  float local_60;
  float local_58;
  float local_54;
  float local_50;
  undefined8 local_48;
  float local_40;
  
  if (param_1 == param_2) {
    if (((param_4 - 0x11eU & 0xffffffef) == 0) &&
       (cVar3 = FUN_142044510(*(undefined8 *)(param_1 + 0x13c0)), cVar3 != '\0')) {
      lVar4 = **(longlong **)(param_1 + 0x13c0);
      FUN_141eb3490(local_res8,L"Enemy",1);
      (**(code **)(lVar4 + 0x5e8))(*(undefined8 *)(param_1 + 0x13c0));
    }
    if ((param_4 - 0x11fU & 0xffffffef) == 0) {
      if ((*(int *)(param_1 + 0x1398) == 0) && (*(int *)(param_1 + 0x139c) == 0)) {
        bVar2 = true;
      }
      else {
        bVar2 = false;
      }
      uVar5 = DAT_145a1da40;
      if (!bVar2) {
        uVar5 = *(undefined8 *)(param_1 + 0x1398);
      }
      (**(code **)(**(longlong **)(param_1 + 0x398) + 0x3f8))
                (*(longlong **)(param_1 + 0x398),&local_58,uVar5);
      if ((*(int *)(param_1 + 0x1390) == 0) && (*(int *)(param_1 + 0x1394) == 0)) {
        bVar2 = true;
      }
      else {
        bVar2 = false;
      }
      uVar5 = DAT_145a1da38;
      if (!bVar2) {
        uVar5 = *(undefined8 *)(param_1 + 0x1390);
      }
      (**(code **)(**(longlong **)(param_1 + 0x398) + 0x3f8))
                (*(longlong **)(param_1 + 0x398),&local_68,uVar5);
      lVar4 = FUN_140d239b0();
      if (lVar4 != 0) {
        lVar1 = *(longlong *)(lVar4 + 0x168);
        if (lVar1 == 0) {
          local_48 = DAT_145a8af10;
          local_40 = DAT_145a8af18;
        }
        else {
          local_48 = *(undefined8 *)(lVar1 + 0x1a0);
          local_40 = *(float *)(lVar1 + 0x1a8);
        }
        local_58 = local_58 - (float)local_48;
        local_54 = local_54 - (float)((ulonglong)local_48 >> 0x20);
        local_50 = local_50 - local_40;
        if (lVar1 == 0) {
          local_48 = DAT_145a8af10;
          local_40 = DAT_145a8af18;
        }
        else {
          local_48 = *(undefined8 *)(lVar1 + 0x1a0);
          local_40 = *(float *)(lVar1 + 0x1a8);
        }
        local_68 = local_68 - (float)local_48;
        local_64 = local_64 - (float)((ulonglong)local_48 >> 0x20);
        if ((SQRT(local_64 * local_64 + local_68 * local_68 +
                  (local_60 - local_40) * (local_60 - local_40)) < DAT_14566e5a8) ||
           (SQRT(local_54 * local_54 + local_58 * local_58 + local_50 * local_50) < DAT_14566e5a8))
        {
          *(undefined1 *)(lVar4 + 0x8d0) = 1;
          FUN_140f176f0();
        }
      }
      *(undefined4 *)(param_1 + 0x1370) = 0;
    }
    else if ((param_3 - 0x120U & 0xffffffef) == 0) {
      if ((*(int *)(param_1 + 0x1398) == 0) && (*(int *)(param_1 + 0x139c) == 0)) {
        bVar2 = true;
      }
      else {
        bVar2 = false;
      }
      uVar5 = DAT_145a1da40;
      if (!bVar2) {
        uVar5 = *(undefined8 *)(param_1 + 0x1398);
      }
      (**(code **)(**(longlong **)(param_1 + 0x398) + 0x3f8))
                (*(longlong **)(param_1 + 0x398),&local_68,uVar5);
      if ((*(int *)(param_1 + 0x1390) == 0) && (*(int *)(param_1 + 0x1394) == 0)) {
        bVar2 = true;
      }
      else {
        bVar2 = false;
      }
      uVar5 = DAT_145a1da38;
      if (!bVar2) {
        uVar5 = *(undefined8 *)(param_1 + 0x1390);
      }
      (**(code **)(**(longlong **)(param_1 + 0x398) + 0x3f8))
                (*(longlong **)(param_1 + 0x398),&local_58,uVar5);
      lVar4 = FUN_140d239b0();
      if (lVar4 != 0) {
        lVar1 = *(longlong *)(lVar4 + 0x168);
        if (lVar1 == 0) {
          local_48 = DAT_145a8af10;
          local_40 = DAT_145a8af18;
        }
        else {
          local_48 = *(undefined8 *)(lVar1 + 0x1a0);
          local_40 = *(float *)(lVar1 + 0x1a8);
        }
        local_68 = local_68 - (float)local_48;
        local_64 = local_64 - (float)((ulonglong)local_48 >> 0x20);
        local_60 = local_60 - local_40;
        if (lVar1 == 0) {
          local_48 = DAT_145a8af10;
          local_40 = DAT_145a8af18;
        }
        else {
          local_48 = *(undefined8 *)(lVar1 + 0x1a0);
          local_40 = *(float *)(lVar1 + 0x1a8);
        }
        local_58 = local_58 - (float)local_48;
        local_54 = local_54 - (float)((ulonglong)local_48 >> 0x20);
        *(uint *)(param_1 + 0x1370) =
             (SQRT(local_64 * local_64 + local_68 * local_68 + local_60 * local_60) <
             SQRT(local_54 * local_54 + local_58 * local_58 +
                  (local_50 - local_40) * (local_50 - local_40))) + 1;
        *(undefined1 *)(lVar4 + 0x8d0) = 0;
        cVar3 = FUN_142044510(*(undefined8 *)(param_1 + 0x13c0));
        if (cVar3 != '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x13c0) + 0x5e8))
                    (*(longlong **)(param_1 + 0x13c0),DAT_145c68c10);
        }
      }
    }
  }
  return;
}


table @0x1444f9c98 name@1444f4a98 exec -> 141a452c0
// ---- FUN_141a452c0 @ 141a452c0 size=0xfc

void FUN_141a452c0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [2];
  undefined4 local_res20 [2];
  undefined8 local_18 [2];
  
  local_18[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_18);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_18);
  }
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
  FUN_141947480(param_1,local_18[0],local_res20[0],local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 141947480
// ---- FUN_141947480 @ 141947480 size=0x3bf

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141947480(longlong param_1,undefined8 param_2,undefined4 param_3,undefined4 param_4)

{
  wchar_t wVar1;
  char cVar2;
  wint_t wVar3;
  wint_t wVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  byte *pbVar8;
  longlong lVar9;
  wint_t *pwVar10;
  int iVar11;
  wchar_t *pwVar12;
  bool bVar13;
  undefined1 auStack_888 [32];
  int local_868 [2];
  undefined8 local_860;
  wchar_t local_858 [1024];
  ulonglong local_58;
  
  local_58 = DAT_145909780 ^ (ulonglong)auStack_888;
  lVar9 = *(longlong *)(param_1 + 0x118);
  if (lVar9 == 0) {
    return;
  }
  lVar6 = FUN_141758410();
  if ((*(int *)(*(longlong *)(lVar9 + 0x10) + 0x90) < *(int *)(lVar6 + 0x90)) ||
     (*(longlong *)
       (*(longlong *)(*(longlong *)(lVar9 + 0x10) + 0x88) + (longlong)*(int *)(lVar6 + 0x90) * 8) !=
      lVar6 + 0x88)) {
    bVar13 = false;
  }
  else {
    bVar13 = true;
  }
  if (!bVar13) {
    return;
  }
  lVar6 = FUN_140d22e90(param_1,0);
  if (lVar6 == 0) {
    return;
  }
  if (*(int *)(lVar6 + 0x2fd0) != 0x3f) {
    return;
  }
  lVar6 = FUN_140f87ff0(*(undefined8 *)(lVar9 + 0x8e0),2);
  if (lVar6 == 0) {
    return;
  }
  lVar6 = FUN_140f09fe0(param_2,param_3);
  iVar11 = 0;
  if (lVar6 != 0) {
    lVar7 = FUN_1417ed280();
    if ((*(int *)(*(longlong *)(lVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar13 = false;
    }
    else {
      bVar13 = true;
    }
    if (!bVar13) {
      return;
    }
    local_860 = *(undefined8 *)(lVar6 + 0xf8);
    pbVar8 = (byte *)FUN_141eb81a0(&local_860);
    if (pbVar8 != (byte *)0x0) {
      pwVar12 = L"C001D_G_SKL001";
      local_868[0] = 0;
      iVar5 = 0;
      cVar2 = thunk_FUN_141eb2bc0(L"C001D_G_SKL001",local_858,0x400,local_868);
      if (cVar2 != '\0') {
        pwVar12 = local_858;
        iVar5 = local_868[0] + 1;
      }
      if (iVar5 == local_860._4_4_) {
        pwVar10 = (wint_t *)(pbVar8 + 0x10);
        if ((*pbVar8 & 1) == 0) {
          do {
            wVar1 = *pwVar12;
            if ((wVar1 == L'\0') && ((byte)*pwVar10 == 0)) {
              iVar5 = 0;
              goto LAB_141947693;
            }
            if ((uint)(ushort)wVar1 != (int)(char)(byte)*pwVar10) {
              wVar3 = towlower(wVar1);
              iVar5 = tolower((int)(char)(byte)*pwVar10);
              if ((uint)wVar3 != (int)(char)iVar5) goto LAB_141947691;
            }
            pwVar12 = pwVar12 + 1;
            pwVar10 = (wint_t *)((longlong)pwVar10 + 1);
          } while( true );
        }
        lVar6 = (longlong)pwVar12 - (longlong)pwVar10;
        do {
          wVar3 = *(wint_t *)((longlong)pwVar10 + lVar6);
          if ((wVar3 == 0) && (*pwVar10 == 0)) {
            bVar13 = true;
            goto LAB_141947695;
          }
          if (wVar3 != *pwVar10) {
            wVar3 = towlower(wVar3);
            wVar4 = towlower(*pwVar10);
            if (wVar3 != wVar4) goto LAB_141947646;
          }
          pwVar10 = pwVar10 + 1;
        } while( true );
      }
    }
  }
  goto LAB_1419476a8;
LAB_141947691:
  iVar5 = (uint)wVar3 - (int)(char)iVar5;
LAB_141947693:
  bVar13 = iVar5 == 0;
LAB_141947695:
  if (bVar13) {
    *(undefined1 *)(param_1 + 0x3fd) = 0;
    return;
  }
LAB_1419476a8:
  cVar2 = FUN_140f10c20(lVar9);
  if (cVar2 == '\0') {
    return;
  }
  lVar9 = FUN_140f09fe0(param_2,param_4);
  if (lVar9 == 0) {
    return;
  }
  lVar6 = FUN_1417ed280();
  if ((*(int *)(*(longlong *)(lVar9 + 0x10) + 0x90) < *(int *)(lVar6 + 0x90)) ||
     (*(longlong *)
       (*(longlong *)(*(longlong *)(lVar9 + 0x10) + 0x88) + (longlong)*(int *)(lVar6 + 0x90) * 8) !=
      lVar6 + 0x88)) {
    bVar13 = false;
  }
  else {
    bVar13 = true;
  }
  if (!bVar13) {
    return;
  }
  local_860 = *(undefined8 *)(lVar9 + 0xf8);
  pbVar8 = (byte *)FUN_141eb81a0(&local_860);
  if (pbVar8 != (byte *)0x0) {
    pwVar12 = L"C001D_G_SKL001";
    local_868[0] = 0;
    iVar5 = 0;
    cVar2 = thunk_FUN_141eb2bc0(L"C001D_G_SKL001",local_858,0x400,local_868);
    if (cVar2 != '\0') {
      pwVar12 = local_858;
      iVar5 = local_868[0] + 1;
    }
    if (iVar5 == local_860._4_4_) {
      pwVar10 = (wint_t *)(pbVar8 + 0x10);
      if ((*pbVar8 & 1) == 0) {
        do {
          wVar1 = *pwVar12;
          if ((wVar1 == L'\0') && ((byte)*pwVar10 == 0)) goto LAB_1419477ef;
          if ((uint)(ushort)wVar1 != (int)(char)(byte)*pwVar10) {
            wVar3 = towlower(wVar1);
            iVar5 = tolower((int)(char)(byte)*pwVar10);
            if ((uint)wVar3 != (int)(char)iVar5) goto LAB_1419477eb;
          }
          pwVar12 = pwVar12 + 1;
          pwVar10 = (wint_t *)((longlong)pwVar10 + 1);
        } while( true );
      }
      lVar9 = (longlong)pwVar12 - (longlong)pwVar10;
      do {
        wVar3 = *(wint_t *)((longlong)pwVar10 + lVar9);
        if ((wVar3 == 0) && (iVar11 = 0, *pwVar10 == 0)) goto LAB_1419477ef;
        if (wVar3 != *pwVar10) {
          wVar3 = towlower(wVar3);
          wVar4 = towlower(*pwVar10);
          if (wVar3 != wVar4) goto LAB_1419477a2;
        }
        pwVar10 = pwVar10 + 1;
      } while( true );
    }
  }
  goto LAB_141947802;
LAB_141947646:
  bVar13 = wVar3 == wVar4;
  goto LAB_141947695;
LAB_1419477eb:
  iVar11 = (uint)wVar3 - (int)(char)iVar5;
  goto LAB_1419477ef;
LAB_1419477a2:
  iVar11 = (uint)wVar3 - (uint)wVar4;
LAB_1419477ef:
  if (iVar11 == 0) {
    *(undefined1 *)(param_1 + 0x3fd) = 1;
  }
  else {
LAB_141947802:
    *(undefined1 *)(param_1 + 0x3fd) = 0;
  }
  return;
}


table @0x1444f5978 name@1444f59b8 exec -> 141a414e0
// ---- FUN_141a414e0 @ 141a414e0 size=0x2f

longlong FUN_141a414e0(void)

{
  if (DAT_145a32df0 == 0) {
    FUN_14203aad0(&DAT_145a32df0,&PTR_FUN_14569a3e0);
  }
  return DAT_145a32df0;
}


  -> call 14203aad0
(already decompiled above: FUN_14203aad0)

######## ufunc OnChangeProgress
table @0x14403a970 name@144036468 exec -> 1417a5880
// ---- FUN_1417a5880 @ 1417a5880 size=0x8f

void FUN_1417a5880(undefined8 param_1,longlong param_2)

{
  undefined8 *puVar1;
  undefined8 local_res10;
  
  local_res10 = 0;
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  puVar1 = &local_res10;
  if (*(undefined8 **)(param_2 + 0x38) != (undefined8 *)0x0) {
    puVar1 = *(undefined8 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141611220(param_1,puVar1);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
table @0x144258720 name@144252108 exec -> 141876df0
// ---- FUN_141876df0 @ 141876df0 size=0x8f

void FUN_141876df0(undefined8 param_1,longlong param_2)

{
  undefined8 *puVar1;
  undefined8 local_res10;
  
  local_res10 = 0;
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  puVar1 = &local_res10;
  if (*(undefined8 **)(param_2 + 0x38) != (undefined8 *)0x0) {
    puVar1 = *(undefined8 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_14118b670(param_1,puVar1);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14118b670
// ---- FUN_14118b670 @ 14118b670 size=0x51

void FUN_14118b670(undefined8 param_1)

{
  char cVar1;
  
  cVar1 = FUN_140d2f9d0();
  if (cVar1 != '\0') {
    FUN_141197a30(param_1,0);
    return;
  }
  cVar1 = FUN_140d2f9e0();
  if (cVar1 == '\0') {
    cVar1 = FUN_140d2f9f0();
    if (cVar1 == '\0') {
      FUN_1411975d0(param_1,0);
      return;
    }
  }
  FUN_141197e90(param_1,0);
  return;
}


table @0x1442c87d0 name@1442bff60 exec -> 1418a22a0
// ---- FUN_1418a22a0 @ 1418a22a0 size=0x8f

void FUN_1418a22a0(undefined8 param_1,longlong param_2)

{
  undefined8 *puVar1;
  undefined8 local_res10;
  
  local_res10 = 0;
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  puVar1 = &local_res10;
  if (*(undefined8 **)(param_2 + 0x38) != (undefined8 *)0x0) {
    puVar1 = *(undefined8 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  thunk_FUN_141139500(param_1,puVar1);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
table @0x1442c27b8 name@1442c2e18 exec -> 14189ff30
// ---- FUN_14189ff30 @ 14189ff30 size=0x2f

longlong FUN_14189ff30(void)

{
  if (DAT_1459f7ef0 == 0) {
    FUN_14203aad0(&DAT_1459f7ef0,&PTR_FUN_1455e7a40);
  }
  return DAT_1459f7ef0;
}


  -> call 14203aad0
(already decompiled above: FUN_14203aad0)
table @0x14439b628 name@144397cf8 exec -> 141904980
// ---- FUN_141904980 @ 141904980 size=0x8f

void FUN_141904980(undefined8 param_1,longlong param_2)

{
  undefined8 *puVar1;
  undefined8 local_res10;
  
  local_res10 = 0;
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  puVar1 = &local_res10;
  if (*(undefined8 **)(param_2 + 0x38) != (undefined8 *)0x0) {
    puVar1 = *(undefined8 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1411b8380(param_1,puVar1);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 1411b8380
// ---- FUN_1411b8380 @ 1411b8380 size=0xc0

void FUN_1411b8380(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  char cVar2;
  longlong lVar3;
  int iVar4;
  
  lVar3 = FUN_140d2a6d0();
  if ((lVar3 != 0) && (iVar4 = 0, 0 < *(int *)(param_1 + 0x538))) {
    do {
      lVar1 = (longlong)iVar4 * 0x18;
      cVar2 = FUN_1412e1120(lVar3,*(longlong *)(param_1 + 0x530) + lVar1,param_2);
      if ((cVar2 != '\0') &&
         (cVar2 = FUN_1412e1120(lVar3,param_2,*(longlong *)(param_1 + 0x530) + 8 + lVar1),
         cVar2 != '\0')) {
        FUN_1411befa0(param_1,*(undefined8 *)(*(longlong *)(param_1 + 0x530) + 0x10 + lVar1));
        return;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < *(int *)(param_1 + 0x538));
  }
  return;
}


table @0x1443c13c8 name@1443b8438 exec -> 141914410
// ---- FUN_141914410 @ 141914410 size=0x8f

void FUN_141914410(undefined8 param_1,longlong param_2)

{
  undefined8 *puVar1;
  undefined8 local_res10;
  
  local_res10 = 0;
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  puVar1 = &local_res10;
  if (*(undefined8 **)(param_2 + 0x38) != (undefined8 *)0x0) {
    puVar1 = *(undefined8 **)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  thunk_FUN_1410ceb30(param_1,puVar1);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)

######## ufunc OnChangePhase
table @0x144258710 name@1442520f8 exec -> 141876d70
// ---- FUN_141876d70 @ 141876d70 size=0x7e

void FUN_141876d70(undefined8 param_1,longlong param_2)

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
  FUN_14118b610(param_1,local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 14118b610
// ---- FUN_14118b610 @ 14118b610 size=0x51

void FUN_14118b610(undefined8 param_1)

{
  char cVar1;
  
  cVar1 = FUN_140d2f9d0();
  if (cVar1 != '\0') {
    FUN_141197a30(param_1,1);
    return;
  }
  cVar1 = FUN_140d2f9e0();
  if (cVar1 == '\0') {
    cVar1 = FUN_140d2f9f0();
    if (cVar1 == '\0') {
      FUN_1411975d0(param_1,1);
      return;
    }
  }
  FUN_141197e90(param_1,1);
  return;
}


table @0x144252a58 name@144252db0 exec -> 141874d80
// ---- FUN_141874d80 @ 141874d80 size=0x2f

longlong FUN_141874d80(void)

{
  if (DAT_1459eaeb0 == 0) {
    FUN_14203aad0(&DAT_1459eaeb0,&PTR_FUN_1455c0ce0);
  }
  return DAT_1459eaeb0;
}


  -> call 14203aad0
(already decompiled above: FUN_14203aad0)
table @0x1443c13b8 name@1443b8428 exec -> 141914390
// ---- FUN_141914390 @ 141914390 size=0x7e

void FUN_141914390(undefined8 param_1,longlong param_2)

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
  FUN_1410d7a80(param_1,local_res10[0]);
  return;
}


  -> call 14204e820
(already decompiled above: FUN_14204e820)
  -> call 1410d7a80
// ---- FUN_1410d7a80 @ 1410d7a80 size=0xce

void FUN_1410d7a80(longlong param_1)

{
  longlong lVar1;
  char cVar2;
  undefined8 uVar3;
  ulonglong uVar4;
  longlong *plVar5;
  ulonglong uVar6;
  
  uVar4 = 0;
  plVar5 = *(longlong **)(param_1 + 0x128);
  uVar6 = (longlong)*(int *)(param_1 + 0x130) & 0x1fffffffffffffff;
  if (plVar5 + *(int *)(param_1 + 0x130) < plVar5) {
    uVar6 = uVar4;
  }
  if (uVar6 != 0) {
    do {
      lVar1 = *plVar5;
      if (lVar1 != 0) {
        cVar2 = FUN_1410cdfd0(*(undefined8 *)(lVar1 + 0x570),*(undefined8 *)(lVar1 + 0x4b8),
                              *(undefined4 *)(lVar1 + 0x4c0),*(undefined8 *)(lVar1 + 0x4c8),
                              *(undefined4 *)(lVar1 + 0x4d0));
        if (((cVar2 == '\0') || (*(char *)(param_1 + 0x108) != '\0')) ||
           (*(char *)(lVar1 + 0x4d6) == '\v')) {
          uVar3 = 0;
        }
        else {
          uVar3 = 1;
        }
        FUN_1410d8790(lVar1,uVar3);
      }
      plVar5 = plVar5 + 1;
      uVar4 = uVar4 + 1;
    } while (uVar4 != uVar6);
  }
  return;
}


table @0x1443b8930 name@1443b89e8 exec -> 141912e70
// ---- FUN_141912e70 @ 141912e70 size=0x2f

longlong FUN_141912e70(void)

{
  if (DAT_145a17010 == 0) {
    FUN_14203aad0(&DAT_145a17010,&PTR_FUN_1456644d0);
  }
  return DAT_145a17010;
}


  -> call 14203aad0
(already decompiled above: FUN_14203aad0)

######## ufunc GetTreasureManager

######## str TreasureAccessPoint
ascii 143c9cdb1
ascii 143c9cdc8
   xref 1410d5c65 in FUN_1410d5ab0@1410d5ab0
utf16 143b0a488
  dataptr @0x1454335b8
utf16 143c9cbb8
   xref 1410cb63b in FUN_1410cb580@1410cb580
utf16 143c9cbe0
   xref 1410cb67e in FUN_1410cb580@1410cb580
utf16 143d41318
   xref 1412269ff in FUN_1412269d0@1412269d0
utf16 1443b2052
utf16 1443b20ea
   xref 14190e51f in FUN_14190e4e0@14190e4e0
utf16 1443b211a

######## str TreasureAccessPointSaveTable
ascii 1443b1410
   xref 145663e48 in <none>
  dataptr @0x145663e48
utf16 1443b60f8
   xref 14190f98e in FUN_14190f960@14190f960
utf16 1443b6178
   xref 1405758c0 in FUN_1405758b0@1405758b0
utf16 1443b74e8
   xref 14057562a in FUN_1405755f0@1405755f0

######## str TreasureAccessPointList
ascii 1443b8a10
  dataptr @0x1456644a0
ascii 1443b8a28
  dataptr @0x145664518

/* ================================ RUN 2 ================================
# run 2: taken-state writers, list registration, OnChangeProgress body
dec 0x1410ceb30
dec 0x1410cdfd0
callers 0x1410d8790
callers 0x1410d7a80
dispr 0x4d6 0x1410c0000 0x1410e0000
dispr 0x4a8 0x1410c0000 0x1410e0000
dispr 0x578 0x1410c0000 0x1410e0000
dec 0x140d20b20
*/

######## dec 0x1410ceb30
// ---- FUN_1410ceb30 @ 1410ceb30 size=0x4db

void FUN_1410ceb30(longlong param_1)

{
  longlong *plVar1;
  undefined8 *puVar2;
  int iVar3;
  char cVar4;
  undefined8 uVar5;
  int *piVar6;
  longlong *plVar7;
  ulonglong uVar8;
  longlong lVar9;
  ulonglong uVar10;
  longlong lVar11;
  ulonglong uVar12;
  undefined1 local_res10 [8];
  longlong *local_48;
  undefined8 local_40;
  
  uVar8 = 0;
  plVar1 = (longlong *)(param_1 + 0x148);
  lVar11 = 0x5e;
  *(undefined4 *)(param_1 + 0x150) = 0;
  if (*(int *)(param_1 + 0x154) != 0x5e) {
    FUN_140827c50(plVar1,0x5e);
  }
  lVar9 = 0x5e;
  do {
    iVar3 = *(int *)(param_1 + 0x150);
    *(int *)(param_1 + 0x150) = iVar3 + 1;
    if (*(int *)(param_1 + 0x154) < iVar3 + 1) {
      FUN_1407f1510(plVar1,iVar3);
    }
    puVar2 = (undefined8 *)(*plVar1 + (longlong)iVar3 * 8);
    if (puVar2 != (undefined8 *)0x0) {
      *puVar2 = 0;
    }
    lVar9 = lVar9 + -1;
  } while (lVar9 != 0);
  plVar1 = (longlong *)(param_1 + 0x158);
  *(undefined4 *)(param_1 + 0x160) = 0;
  if (*(int *)(param_1 + 0x164) != 0x5e) {
    FUN_140827c50(plVar1,0x5e);
  }
  lVar9 = 0x5e;
  do {
    iVar3 = *(int *)(param_1 + 0x160);
    *(int *)(param_1 + 0x160) = iVar3 + 1;
    if (*(int *)(param_1 + 0x164) < iVar3 + 1) {
      FUN_1407f1510(plVar1,iVar3);
    }
    puVar2 = (undefined8 *)(*plVar1 + (longlong)iVar3 * 8);
    if (puVar2 != (undefined8 *)0x0) {
      *puVar2 = 0;
    }
    lVar9 = lVar9 + -1;
  } while (lVar9 != 0);
  plVar1 = (longlong *)(param_1 + 0x168);
  *(undefined4 *)(param_1 + 0x170) = 0;
  if (*(int *)(param_1 + 0x174) != 0x5e) {
    FUN_140827c50(plVar1,0x5e);
  }
  lVar9 = 0x5e;
  do {
    iVar3 = *(int *)(param_1 + 0x170);
    *(int *)(param_1 + 0x170) = iVar3 + 1;
    if (*(int *)(param_1 + 0x174) < iVar3 + 1) {
      FUN_1407f1510(plVar1,iVar3);
    }
    puVar2 = (undefined8 *)(*plVar1 + (longlong)iVar3 * 8);
    if (puVar2 != (undefined8 *)0x0) {
      *puVar2 = 0;
    }
    lVar9 = lVar9 + -1;
  } while (lVar9 != 0);
  plVar1 = (longlong *)(param_1 + 0x178);
  *(undefined4 *)(param_1 + 0x180) = 0;
  if (*(int *)(param_1 + 0x184) != 0x5e) {
    FUN_140827c50(plVar1,0x5e);
  }
  lVar9 = 0x5e;
  do {
    iVar3 = *(int *)(param_1 + 0x180);
    *(int *)(param_1 + 0x180) = iVar3 + 1;
    if (*(int *)(param_1 + 0x184) < iVar3 + 1) {
      FUN_1407f1510(plVar1,iVar3);
    }
    puVar2 = (undefined8 *)(*plVar1 + (longlong)iVar3 * 8);
    if (puVar2 != (undefined8 *)0x0) {
      *puVar2 = 0;
    }
    lVar9 = lVar9 + -1;
  } while (lVar9 != 0);
  plVar1 = (longlong *)(param_1 + 0x188);
  *(undefined4 *)(param_1 + 400) = 0;
  if (*(int *)(param_1 + 0x194) != 0x5e) {
    FUN_140827c50(plVar1,0x5e);
  }
  lVar9 = 0x5e;
  do {
    iVar3 = *(int *)(param_1 + 400);
    *(int *)(param_1 + 400) = iVar3 + 1;
    if (*(int *)(param_1 + 0x194) < iVar3 + 1) {
      FUN_1407f1510(plVar1,iVar3);
    }
    puVar2 = (undefined8 *)(*plVar1 + (longlong)iVar3 * 8);
    if (puVar2 != (undefined8 *)0x0) {
      *puVar2 = 0;
    }
    lVar9 = lVar9 + -1;
  } while (lVar9 != 0);
  plVar1 = (longlong *)(param_1 + 0x198);
  *(undefined4 *)(param_1 + 0x1a0) = 0;
  if (*(int *)(param_1 + 0x1a4) != 0x5e) {
    FUN_140827c50(plVar1,0x5e);
  }
  lVar9 = 0x5e;
  do {
    iVar3 = *(int *)(param_1 + 0x1a0);
    *(int *)(param_1 + 0x1a0) = iVar3 + 1;
    if (*(int *)(param_1 + 0x1a4) < iVar3 + 1) {
      FUN_1407f1510(plVar1,iVar3);
    }
    puVar2 = (undefined8 *)(*plVar1 + (longlong)iVar3 * 8);
    if (puVar2 != (undefined8 *)0x0) {
      *puVar2 = 0;
    }
    lVar9 = lVar9 + -1;
  } while (lVar9 != 0);
  plVar1 = (longlong *)(param_1 + 0x1a8);
  *(undefined4 *)(param_1 + 0x1b0) = 0;
  if (*(int *)(param_1 + 0x1b4) != 0x5e) {
    FUN_140827c50(plVar1,0x5e);
  }
  do {
    iVar3 = *(int *)(param_1 + 0x1b0);
    *(int *)(param_1 + 0x1b0) = iVar3 + 1;
    if (*(int *)(param_1 + 0x1b4) < iVar3 + 1) {
      FUN_1407f1510(plVar1,iVar3);
    }
    puVar2 = (undefined8 *)(*plVar1 + (longlong)iVar3 * 8);
    if (puVar2 != (undefined8 *)0x0) {
      *puVar2 = 0;
    }
    lVar11 = lVar11 + -1;
  } while (lVar11 != 0);
  lVar11 = FUN_1410d5080(param_1,0);
  lVar9 = FUN_1411fc890();
  if (lVar9 != 0) {
    lVar9 = FUN_1411fc890();
    if ((*(longlong *)(lVar9 + 0x1ea0) != 0) &&
       (lVar9 = *(longlong *)(*(longlong *)(lVar9 + 0x1ea0) + 0x100), uVar12 = lVar9 + 0x55350,
       lVar9 != 0)) goto LAB_1410cee2e;
  }
  uVar12 = uVar8;
LAB_1410cee2e:
  if ((lVar11 != 0) && (uVar12 != 0)) {
    cVar4 = FUN_140d2fa40();
    local_48 = (longlong *)0x0;
    local_40 = 0;
    if (cVar4 == '\0') {
      FUN_1410c57d0(lVar11,&DAT_143c9c1a0,&local_48);
      plVar1 = local_48;
      uVar10 = (ulonglong)((longlong)(local_48 + (int)local_40) + (7 - (longlong)local_48)) >> 3;
      if (local_48 + (int)local_40 < local_48) {
        uVar10 = uVar8;
      }
      plVar7 = local_48;
      if (uVar10 != 0) {
        do {
          lVar11 = *plVar7;
          if (*(uint *)(lVar11 + 8) < 500) {
            cVar4 = FUN_1410cdfd0(param_1,*(undefined8 *)(lVar11 + 0x10),
                                  *(undefined4 *)(lVar11 + 0x18),*(undefined8 *)(lVar11 + 0x20),
                                  *(undefined4 *)(lVar11 + 0x28));
            if (cVar4 != '\0') {
              lVar9 = FUN_1410d44d0(param_1,*(undefined1 *)(lVar11 + 0xc));
              *(int *)(lVar9 + 4) = *(int *)(lVar9 + 4) + 1;
            }
            cVar4 = FUN_1410d6320(uVar12,0,*(undefined4 *)(lVar11 + 8));
            if (cVar4 != '\0') {
              piVar6 = (int *)FUN_1410d44d0(param_1,*(undefined1 *)(lVar11 + 0xc));
              *piVar6 = *piVar6 + 1;
            }
          }
          uVar8 = uVar8 + 1;
          plVar7 = plVar7 + 1;
        } while (uVar8 != uVar10);
      }
    }
    else {
      FUN_1410c57d0(lVar11,&DAT_143c9c19c,&local_48);
      uVar10 = (ulonglong)((longlong)(local_48 + (int)local_40) + (7 - (longlong)local_48)) >> 3;
      if (local_48 + (int)local_40 < local_48) {
        uVar10 = uVar8;
      }
      plVar7 = local_48;
      plVar1 = local_48;
      if (uVar10 != 0) {
        do {
          lVar11 = *plVar7;
          if (*(uint *)(lVar11 + 8) < 500) {
            cVar4 = FUN_140bd2e60(lVar11 + 0x10,&DAT_143c9c19a);
            if ((cVar4 == '\0') &&
               (cVar4 = FUN_140bd2e60(lVar11 + 0x20,&DAT_143c9c19b), cVar4 == '\0')) {
              uVar5 = FUN_1412dc820(local_res10);
              cVar4 = FUN_1412d7850(uVar5,lVar11 + 0x10,lVar11 + 0x20);
              if (cVar4 != '\0') goto LAB_1410ceef1;
            }
            else {
LAB_1410ceef1:
              lVar9 = FUN_1410d44d0(param_1,*(undefined1 *)(lVar11 + 0xc));
              *(int *)(lVar9 + 4) = *(int *)(lVar9 + 4) + 1;
            }
            cVar4 = FUN_1410d6320(uVar12,0,*(undefined4 *)(lVar11 + 8));
            if (cVar4 != '\0') {
              piVar6 = (int *)FUN_1410d44d0(param_1,*(undefined1 *)(lVar11 + 0xc));
              *piVar6 = *piVar6 + 1;
            }
          }
          plVar7 = plVar7 + 1;
          uVar8 = uVar8 + 1;
          plVar1 = local_48;
        } while (uVar8 != uVar10);
      }
    }
    if (plVar1 != (longlong *)0x0) {
      FUN_141de9200(plVar1);
    }
  }
  return;
}



######## dec 0x1410cdfd0
// ---- FUN_1410cdfd0 @ 1410cdfd0 size=0xf8

undefined8
FUN_1410cdfd0(undefined8 param_1,undefined8 param_2,int param_3,undefined8 param_4,int param_5)

{
  char cVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined8 local_res10 [2];
  undefined8 local_res20;
  
  if (((((int)param_2 == 0) && ((int)((ulonglong)param_2 >> 0x20) == 0)) && ((int)param_4 == 0)) &&
     ((int)((ulonglong)param_4 >> 0x20) == 0)) {
    return 1;
  }
  local_res10[0] = param_2;
  local_res20 = param_4;
  lVar2 = FUN_140d2aa10();
  if (lVar2 == 0) {
    return 1;
  }
  cVar1 = FUN_1413cf190(lVar2,local_res10);
  if ((cVar1 == '\0') || (cVar1 = FUN_1413cf190(lVar2,&local_res20), cVar1 == '\0')) {
    uVar3 = FUN_141367710(local_res10,param_3,&local_res20,param_5);
    return uVar3;
  }
  cVar1 = FUN_1413cf190(lVar2,local_res10);
  if ((cVar1 != '\x01') || (cVar1 = FUN_1413cf190(lVar2,&local_res20), cVar1 != '\x01')) {
    return 1;
  }
  lVar2 = FUN_1413cf1b0(local_res10);
  if ((((lVar2 != 0) && (param_3 <= *(int *)(lVar2 + 8))) &&
      (lVar2 = FUN_1413cf1b0(&local_res20), lVar2 != 0)) && (*(int *)(lVar2 + 8) <= param_5)) {
    return 1;
  }
  return 0;
}



######## callers 0x1410d8790
  ref from 1460bbf0c (DATA) in <none>
  ref from 14519e060 (DATA) in <none>
  ref from 14519e070 (DATA) in <none>
  ref from 1410d3c4f (UNCONDITIONAL_CALL) in FUN_1410d3be0@1410d3be0
  ref from 1410d7b22 (UNCONDITIONAL_CALL) in FUN_1410d7a80@1410d7a80
  ref from 1410e12d7 (UNCONDITIONAL_CALL) in FUN_1410e11b0@1410e11b0
  ref from 1419105bd (UNCONDITIONAL_CALL) in FUN_141910550@141910550
  ref from 1410d7a62 (UNCONDITIONAL_CALL) in FUN_1410d7a00@1410d7a00
  ref from 1410d7a71 (UNCONDITIONAL_CALL) in FUN_1410d7a00@1410d7a00

######## callers 0x1410d7a80
  ref from 1460bbd98 (DATA) in <none>
  ref from 14519e460 (DATA) in <none>
  ref from 14519e470 (DATA) in <none>
  ref from 1419143f9 (UNCONDITIONAL_CALL) in FUN_141914390@141914390

######## dispr 0x4d6 0x1410c0000 0x1410e0000
  1410c7377  CMP byte ptr [RDI + 0x4d6],0xb    ; in FUN_1410c72b0@1410c72b0
  1410cb634  MOV byte ptr [RBX + 0x4d6],0xb    ; in FUN_1410cb580@1410cb580
  1410cd2cb  CMP byte ptr [RDI + 0x4d6],0xb    ; in FUN_1410cd1b0@1410cd1b0
  1410d3d49  CMP byte ptr [RCX + 0x4d6],0xb    ; in FUN_1410d3cf0@1410d3cf0
  1410d7b10  CMP byte ptr [RBX + 0x4d6],0xb    ; in FUN_1410d7a80@1410d7a80
  1410d9dfe  MOV byte ptr [RBX + 0x4d6],0xb    ; in FUN_1410d9df0@1410d9df0
  1410da012  MOV byte ptr [RSI + 0x4d6],0xb    ; in FUN_1410d9e10@1410d9e10
  1410dd67d  CMP byte ptr [RBX + 0x4d6],0xb    ; in FUN_1410dd600@1410dd600

######## dispr 0x4a8 0x1410c0000 0x1410e0000
  1410c6278  MOV qword ptr [RBX + 0x4a8],RCX    ; in FUN_1410c6130@1410c6130
  1410c6dc8  MOV qword ptr [RDI + 0x4a8],RAX    ; in FUN_1410c6c90@1410c6c90
  1410c6ddc  MOV RCX,qword ptr [RDI + 0x4a8]    ; in FUN_1410c6c90@1410c6c90
  1410c6df3  MOV RCX,qword ptr [RDI + 0x4a8]    ; in FUN_1410c6c90@1410c6c90
  1410c6e01  MOV RAX,qword ptr [RDI + 0x4a8]    ; in FUN_1410c6c90@1410c6c90
  1410c6e14  MOV RCX,qword ptr [RDI + 0x4a8]    ; in FUN_1410c6c90@1410c6c90
  1410c72fb  MOV dword ptr [RDI + 0x4a8],0xffffffff    ; in FUN_1410c72b0@1410c72b0
  1410cb5e7  MOV ESI,dword ptr [RBX + 0x4a8]    ; in FUN_1410cb580@1410cb580
  1410d9e18  MOVSXD RDI,dword ptr [RCX + 0x4a8]    ; in FUN_1410d9e10@1410d9e10
  1410d9f57  MOV R15D,dword ptr [RSI + 0x4a8]    ; in FUN_1410d9e10@1410d9e10
  1410de962  MOV EAX,dword ptr [R10 + 0x4a8]    ; in FUN_1410de0d0@1410de0d0

######## dispr 0x578 0x1410c0000 0x1410e0000
  1410c7392  MOV dword ptr [RDI + 0x578],0xffffffff    ; in FUN_1410c72b0@1410c72b0
  1410d87a7  CMP dword ptr [RCX + 0x578],EDI    ; in FUN_1410d8790@1410d8790
  1410d886d  MOV dword ptr [RBX + 0x578],ESI    ; in FUN_1410d8790@1410d8790

######## dec 0x140d20b20
// ---- FUN_140d20b20 @ 140d20b20 size=0xae

void FUN_140d20b20(char param_1,longlong *param_2)

{
  int iVar1;
  char cVar2;
  uint uVar3;
  longlong lVar4;
  uint uVar5;
  
  if ((param_2 != (longlong *)0x0) && (cVar2 = FUN_142044510(param_2), cVar2 != '\0')) {
    iVar1 = *(int *)((longlong)param_2 + 0xc);
    if (iVar1 < (int)DAT_145b9e804) {
      uVar5 = iVar1 >> 0x1f & 0xffff;
      uVar3 = iVar1 + uVar5;
      lVar4 = *(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar3 >> 0x10) * 8) +
              (longlong)(int)((uVar3 & 0xffff) - uVar5) * 0x18;
    }
    else {
      lVar4 = 0;
    }
    if ((*(uint *)(lVar4 + 8) >> 0x1d & 1) == 0) {
      cVar2 = (**(code **)(*param_2 + 0x430))(param_2);
      if (cVar2 != param_1) {
        FUN_142c7dac0(param_2,param_1,1);
      }
      cVar2 = FUN_142c186b0(param_2);
      if (cVar2 != param_1) {
        (**(code **)(*param_2 + 0x328))(param_2,param_1);
      }
    }
  }
  return;
}



/* ================================ RUN 3 ================================
# run 3: init / take paths
dec 0x1410cb580
callers 0x1410cb580
dec 0x1410d9df0
dec 0x1410d9e10
callers 0x1410d9e10
dec 0x1410d3be0
callers 0x1410d3be0
dec 0x1410e11b0
callers 0x1410e11b0
dec 0x1410d7a00
callers 0x1410d7a00
dec 0x1410dd600
dec 0x1410d6320
*/

######## dec 0x1410cb580
// ---- FUN_1410cb580 @ 1410cb580 size=0x176

void FUN_1410cb580(longlong param_1)

{
  undefined1 uVar1;
  int iVar2;
  undefined4 uVar3;
  char cVar4;
  longlong lVar5;
  undefined8 *puVar6;
  undefined1 local_res8 [8];
  
  FUN_142b0bd40();
  FUN_1411fedd0(param_1 + 0x3c0);
  *(undefined4 *)(param_1 + 0x3f8) = 0;
  lVar5 = FUN_140d2b8e0();
  *(longlong *)(param_1 + 0x570) = lVar5;
  iVar2 = *(int *)(lVar5 + 0x130);
  *(int *)(lVar5 + 0x130) = iVar2 + 1;
  if (*(int *)(lVar5 + 0x134) < iVar2 + 1) {
    FUN_1407f1510((longlong *)(lVar5 + 0x128),iVar2);
  }
  *(longlong *)(*(longlong *)(lVar5 + 0x128) + (longlong)iVar2 * 8) = param_1;
  uVar1 = *(undefined1 *)(param_1 + 0x4d4);
  uVar3 = *(undefined4 *)(param_1 + 0x4a8);
  lVar5 = FUN_1411fc890();
  if (lVar5 != 0) {
    lVar5 = FUN_1411fc890();
    if (((*(longlong *)(lVar5 + 0x1ea0) != 0) &&
        (lVar5 = *(longlong *)(*(longlong *)(lVar5 + 0x1ea0) + 0x100), lVar5 != 0)) &&
       (lVar5 = lVar5 + 0x55350, lVar5 != 0)) {
      cVar4 = FUN_1410d6320(lVar5,uVar1,uVar3);
      if (cVar4 != '\0') {
        *(undefined1 *)(param_1 + 0x4d6) = 0xb;
        FUN_141eb3490(local_res8,L"TreasureAccessPoint",1);
        FUN_140d2cdf0(param_1 + 0x400,local_res8,param_1);
        return;
      }
    }
  }
  FUN_1410d5ab0(param_1);
  FUN_141eb3490(local_res8,L"TreasureAccessPoint",1);
  FUN_140d2cdf0(param_1 + 0x400,local_res8,param_1);
  FUN_1410c9750(param_1);
  if (*(char *)(param_1 + 0x568) != '\0') {
    if (*(longlong *)(param_1 + 0x340) != 0) {
      puVar6 = (undefined8 *)FUN_142f9e760(*(longlong *)(param_1 + 0x340),local_res8);
      *(undefined8 *)(param_1 + 0x3b8) = *puVar6;
    }
    *(undefined1 *)(param_1 + 0x568) = 0;
  }
  FUN_140d18360(param_1 + 0x400);
  return;
}



######## callers 0x1410cb580
  ref from 1460baba4 (DATA) in <none>
  ref from 143ca5218 (DATA) in <none>
  dataptr @0x143ca5218

######## dec 0x1410d9df0
// ---- FUN_1410d9df0 @ 1410d9df0 size=0x1b

void FUN_1410d9df0(longlong param_1)

{
  FUN_1410d9b00();
  *(undefined1 *)(param_1 + 0x4d6) = 0xb;
  return;
}



######## dec 0x1410d9e10
// ---- FUN_1410d9e10 @ 1410d9e10 size=0x211

void FUN_1410d9e10(longlong param_1)

{
  undefined1 *puVar1;
  uint uVar2;
  int iVar3;
  undefined8 uVar4;
  longlong *plVar5;
  char cVar6;
  longlong lVar7;
  int *piVar8;
  ulonglong uVar9;
  ulonglong uVar10;
  longlong *plVar11;
  longlong *local_28;
  undefined8 local_20;
  
  uVar2 = *(uint *)(param_1 + 0x4a8);
  cVar6 = *(char *)(param_1 + 0x4d4);
  lVar7 = FUN_1411fc890();
  if (lVar7 != 0) {
    lVar7 = FUN_1411fc890();
    if (((*(longlong *)(lVar7 + 0x1ea0) != 0) &&
        (lVar7 = *(longlong *)(*(longlong *)(lVar7 + 0x1ea0) + 0x100), lVar7 != 0)) &&
       (lVar7 = lVar7 + 0x55350, lVar7 != 0)) {
      if (cVar6 == '\0') {
        if (uVar2 < 500) {
          puVar1 = (undefined1 *)(lVar7 + (int)uVar2);
          cVar6 = FUN_140d2fa30();
          if (cVar6 == '\0') {
            cVar6 = FUN_140d2f960();
            if (cVar6 == '\0') {
              cVar6 = FUN_140d2f970();
              if (cVar6 == '\0') {
                cVar6 = FUN_140d2f9d0();
                if (cVar6 == '\0') {
                  cVar6 = FUN_140d2f9e0();
                  if (cVar6 == '\0') {
                    cVar6 = FUN_140d2f9f0();
                    if (cVar6 == '\0') {
                      *puVar1 = 1;
                    }
                    else {
                      puVar1[3000] = 1;
                    }
                  }
                  else {
                    puVar1[0x9c4] = 1;
                  }
                }
                else {
                  puVar1[2000] = 1;
                }
              }
              else {
                puVar1[0x5dc] = 1;
              }
            }
            else {
              puVar1[1000] = 1;
            }
          }
          else {
            puVar1[500] = 1;
          }
        }
      }
      else if ((cVar6 == '\x01') && (uVar2 < 700)) {
        lVar7 = lVar7 + (int)uVar2;
        cVar6 = FUN_140d2f9e0();
        if (cVar6 == '\0') {
          cVar6 = FUN_140d2f9f0();
          if (cVar6 == '\0') {
            *(undefined1 *)(lVar7 + 0xdac) = 1;
          }
          else {
            *(undefined1 *)(lVar7 + 0x1324) = 1;
          }
        }
        else {
          *(undefined1 *)(lVar7 + 0x1068) = 1;
        }
      }
    }
  }
  if (*(char *)(param_1 + 0x4d4) == '\0') {
    uVar4 = *(undefined8 *)(param_1 + 0x570);
    iVar3 = *(int *)(param_1 + 0x4a8);
    lVar7 = FUN_1410d5080(uVar4,0);
    if (lVar7 != 0) {
      uVar9 = 0;
      local_28 = (longlong *)0x0;
      local_20 = 0;
      FUN_1410c57d0(lVar7,&DAT_143c9d52c,&local_28);
      plVar5 = local_28;
      uVar10 = (ulonglong)((longlong)(local_28 + (int)local_20) + (7 - (longlong)local_28)) >> 3;
      if (local_28 + (int)local_20 < local_28) {
        uVar10 = uVar9;
      }
      plVar11 = local_28;
      if (uVar10 != 0) {
        do {
          lVar7 = *plVar11;
          if ((lVar7 != 0) && (*(int *)(lVar7 + 8) == iVar3)) {
            piVar8 = (int *)FUN_1410d44d0(uVar4,*(undefined1 *)(lVar7 + 0xc));
            *piVar8 = *piVar8 + 1;
          }
          plVar11 = plVar11 + 1;
          uVar9 = uVar9 + 1;
        } while (uVar9 != uVar10);
      }
      if (plVar5 != (longlong *)0x0) {
        FUN_141de9200(plVar5);
      }
    }
  }
  FUN_1410d9b00(param_1);
  *(undefined1 *)(param_1 + 0x4d6) = 0xb;
  return;
}



######## callers 0x1410d9e10
  ref from 1460bc044 (DATA) in <none>
  ref from 14519e270 (DATA) in <none>
  ref from 14519e2bc (DATA) in <none>
  ref from 143ca55e8 (DATA) in <none>
  dataptr @0x143ca55e8

######## dec 0x1410d3be0
// ---- FUN_1410d3be0 @ 1410d3be0 size=0x95

void FUN_1410d3be0(longlong param_1,int param_2)

{
  longlong lVar1;
  longlong *plVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  
  if (param_2 != 0) {
    *(undefined1 *)(param_1 + 0x108) = 1;
    plVar2 = *(longlong **)(param_1 + 0x128);
    uVar4 = 0;
    uVar3 = (longlong)*(int *)(param_1 + 0x130) & 0x1fffffffffffffff;
    if (plVar2 + *(int *)(param_1 + 0x130) < plVar2) {
      uVar3 = uVar4;
    }
    if (uVar3 != 0) {
      do {
        lVar1 = *plVar2;
        if (lVar1 != 0) {
          *(ushort *)(lVar1 + 0x4a0) =
               *(ushort *)(lVar1 + 0x4a0) | (ushort)(1 << ((int)(short)param_2 & 0x1fU));
          FUN_1410d8790(lVar1,0);
        }
        plVar2 = plVar2 + 1;
        uVar4 = uVar4 + 1;
      } while (uVar4 != uVar3);
    }
  }
  return;
}



######## callers 0x1410d3be0
  ref from 1460bb648 (DATA) in <none>
  ref from 14519e43c (DATA) in <none>
  ref from 14519e44c (DATA) in <none>
  ref from 1410c9450 (DATA) in FUN_1410c9190@1410c9190
  ref from 1410c9457 (DATA) in FUN_1410c9190@1410c9190

######## dec 0x1410e11b0
// ---- FUN_1410e11b0 @ 1410e11b0 size=0x14a

void FUN_1410e11b0(longlong param_1,undefined4 param_2)

{
  longlong *plVar1;
  char cVar2;
  longlong lVar3;
  int iVar4;
  longlong lVar5;
  uint uVar6;
  
  FUN_142b28560();
  iVar4 = 0;
  *(undefined4 *)(param_1 + 0x3a4) = param_2;
  if (*(short *)(param_1 + 0x4a0) < 1) {
    uVar6 = *(uint *)(param_1 + 0x4d0);
    cVar2 = FUN_1410cdfd0(*(undefined8 *)(param_1 + 0x570),*(undefined8 *)(param_1 + 0x4b8),
                          *(undefined4 *)(param_1 + 0x4c0),*(undefined8 *)(param_1 + 0x4c8),uVar6);
    if (cVar2 != '\0') {
      FUN_140d43db0(param_1 + 0x400,param_2);
      if (0 < *(int *)(param_1 + 0x3f0)) {
        lVar5 = 0;
        do {
          plVar1 = *(longlong **)(lVar5 + *(longlong *)(param_1 + 1000));
          if (((plVar1 != (longlong *)0x0) &&
              ((**(code **)(*plVar1 + 8))(plVar1,param_2), (int)plVar1[4] == 1)) &&
             (lVar3 = FUN_142058d50(plVar1 + 0x23), lVar3 != 0)) {
            uVar6 = uVar6 & 0xffffff00;
            FUN_142c7e940(lVar3,plVar1 + 0x14,0,0,uVar6);
          }
          iVar4 = iVar4 + 1;
          lVar5 = lVar5 + 8;
        } while (iVar4 < *(int *)(param_1 + 0x3f0));
      }
      cVar2 = FUN_1410cde10(param_1);
      if (cVar2 != '\0') {
        *(undefined1 *)(param_1 + 0x3e1) = 1;
      }
      FUN_1410e2e00(param_1,param_2);
      return;
    }
  }
  FUN_1410d8790(param_1,0);
  FUN_140d43db0(param_1 + 0x400,param_2);
  return;
}



######## callers 0x1410e11b0
  ref from 1460bc908 (DATA) in <none>
  ref from 14519e03c (DATA) in <none>
  ref from 14519e04c (DATA) in <none>
  ref from 143ca5340 (DATA) in <none>
  ref from 144303dd8 (DATA) in <none>
  dataptr @0x143ca5340
  dataptr @0x144303dd8

######## dec 0x1410d7a00
// ---- FUN_1410d7a00 @ 1410d7a00 size=0x76

void FUN_1410d7a00(longlong param_1,ulonglong param_2)

{
  char cVar1;
  int iVar2;
  
  if (*(short *)(param_1 + 0x4a0) < 1) {
    param_2 = *(ulonglong *)(param_1 + 0x4b8);
    cVar1 = FUN_1410cdfd0(*(undefined8 *)(param_1 + 0x570),param_2,*(undefined4 *)(param_1 + 0x4c0),
                          *(undefined8 *)(param_1 + 0x4c8),*(undefined4 *)(param_1 + 0x4d0));
    if ((cVar1 != '\0') &&
       ((iVar2 = (*(int *)(param_1 + 0x43c) << 0x17) >> 0x1d, iVar2 == 0 || (iVar2 == 3)))) {
      FUN_1410d8790(param_1,1);
      return;
    }
  }
  FUN_1410d8790(param_1,param_2 & 0xffffffffffffff00);
  return;
}



######## callers 0x1410d7a00
  ref from 1460bbd8c (DATA) in <none>
  ref from 1410d3d66 (UNCONDITIONAL_CALL) in FUN_1410d3cf0@1410d3cf0
  ref from 1410cd716 (UNCONDITIONAL_CALL) in FUN_1410cd6c0@1410cd6c0
  ref from 1410cd732 (UNCONDITIONAL_CALL) in FUN_1410cd6c0@1410cd6c0
  ref from 1410cd785 (UNCONDITIONAL_CALL) in FUN_1410cd6c0@1410cd6c0

######## dec 0x1410dd600
// ---- FUN_1410dd600 @ 1410dd600 size=0xc3

void FUN_1410dd600(longlong *param_1,char param_2)

{
  char cVar1;
  undefined8 uVar2;
  longlong *plVar3;
  undefined4 local_res8;
  undefined4 local_resc;
  
  cVar1 = FUN_14205a760(param_1 + 0xa0);
  if (cVar1 != '\0') {
    uVar2 = FUN_142058d50(param_1 + 0xa0);
    FUN_142c7dac0(uVar2,param_2,1);
    plVar3 = (longlong *)FUN_142058d50(param_1 + 0xa0);
                    /* WARNING: Could not recover jumptable at 0x0001410dd665. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*plVar3 + 0x328))(plVar3,param_2);
    return;
  }
  if (((param_2 != '\0') && (param_1[0x9f] != 0)) && (*(char *)((longlong)param_1 + 0x4d6) != '\v'))
  {
    uVar2 = (**(code **)(*param_1 + 0x620))(param_1);
    FUN_1420575e0(&local_res8,uVar2);
    *(undefined4 *)(param_1 + 0xa0) = local_res8;
    *(undefined4 *)((longlong)param_1 + 0x504) = local_resc;
  }
  return;
}



######## dec 0x1410d6320
// ---- FUN_1410d6320 @ 1410d6320 size=0x134

undefined1 FUN_1410d6320(longlong param_1,int param_2,uint param_3)

{
  char cVar1;
  undefined1 *puVar2;
  undefined1 uVar3;
  
  uVar3 = 0;
  if (param_2 == 0) {
    if (param_3 < 500) {
      puVar2 = (undefined1 *)((int)param_3 + param_1);
      cVar1 = FUN_140d2fa30();
      if (cVar1 != '\0') {
        return puVar2[500];
      }
      cVar1 = FUN_140d2f960();
      if (cVar1 != '\0') {
        return puVar2[1000];
      }
      cVar1 = FUN_140d2f970();
      if (cVar1 != '\0') {
        return puVar2[0x5dc];
      }
      cVar1 = FUN_140d2f9d0();
      if (cVar1 != '\0') {
        return puVar2[2000];
      }
      cVar1 = FUN_140d2f9e0();
      if (cVar1 != '\0') {
        return puVar2[0x9c4];
      }
      cVar1 = FUN_140d2f9f0();
      if (cVar1 != '\0') {
        return puVar2[3000];
      }
      uVar3 = *puVar2;
    }
  }
  else if ((param_2 == 1) && (param_3 < 700)) {
    param_1 = (int)param_3 + param_1;
    cVar1 = FUN_140d2f9e0();
    if (cVar1 != '\0') {
      return *(undefined1 *)(param_1 + 0x1068);
    }
    cVar1 = FUN_140d2f9f0();
    if (cVar1 != '\0') {
      return *(undefined1 *)(param_1 + 0x1324);
    }
    return *(undefined1 *)(param_1 + 0xdac);
  }
  return uVar3;
}



/* ================================ RUN 4 ================================
# run 4: DLC predicates, non-taken setup, refresh paths, take tail
dec 0x140d2fa30
dec 0x140d2f9f0
dec 0x1410d5ab0
dec 0x1410d3cf0
callers 0x1410d3cf0
dec 0x1410cd6c0
callers 0x1410cd6c0
dec 0x1410d9b00
callers 0x1410d9df0
*/

######## dec 0x140d2fa30
// ---- FUN_140d2fa30 @ 140d2fa30 size=0xc

void FUN_140d2fa30(void)

{
  FUN_14107d9f0(DAT_14592ff18);
  return;
}



######## dec 0x140d2f9f0
// ---- FUN_140d2f9f0 @ 140d2f9f0 size=0xc

void FUN_140d2f9f0(void)

{
  FUN_14107d9f0(DAT_14592ff50);
  return;
}



######## dec 0x1410d5ab0
// ---- FUN_1410d5ab0 @ 1410d5ab0 size=0x443

void FUN_1410d5ab0(longlong *param_1)

{
  bool bVar1;
  longlong lVar2;
  char cVar3;
  int iVar4;
  undefined8 uVar5;
  longlong lVar6;
  longlong *plVar7;
  longlong lVar8;
  longlong lVar9;
  longlong *plVar10;
  longlong lVar11;
  longlong lVar12;
  longlong lVar13;
  undefined4 local_res8;
  undefined4 local_resc;
  undefined8 local_res10;
  longlong local_res18;
  undefined1 local_res20 [8];
  undefined8 local_58;
  undefined4 local_50;
  longlong local_48;
  undefined8 local_40;
  
  if (*(char *)((longlong)param_1 + 0x4d5) == '\0') {
    cVar3 = FUN_14205a760(param_1 + 0xa0);
    if (cVar3 == '\0') {
      if (param_1[0x9f] != 0) {
        uVar5 = (**(code **)(*param_1 + 0x620))(param_1);
        FUN_1420575e0(&local_res8,uVar5);
        *(undefined4 *)(param_1 + 0xa0) = local_res8;
        *(undefined4 *)((longlong)param_1 + 0x504) = local_resc;
      }
    }
    else {
      FUN_1410dd600(param_1,1);
    }
  }
  lVar6 = FUN_1411fc890();
  lVar13 = 0;
  if ((lVar6 == 0) || (lVar9 = *(longlong *)(lVar6 + 0x1ef0), lVar9 == 0)) goto LAB_1410d5e82;
  lVar12 = lVar9 + 0x4070;
  plVar7 = (longlong *)FUN_142058d50(lVar12);
  if (plVar7 == (longlong *)0x0) {
    LOCK();
    if (DAT_1456e073c == 0) {
      DAT_1456e073c = 0;
    }
    UNLOCK();
    if ((DAT_1456e073c != *(int *)(lVar9 + 0x4078)) &&
       (local_res10 = 0, *(longlong *)(lVar9 + 0x4080) != 0)) {
      lVar8 = FUN_14204ac70();
      FUN_1420575e0(lVar12,lVar8);
      if ((lVar8 != 0) || (DAT_145b9d472 == '\0')) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        *(int *)(lVar9 + 0x4078) = DAT_1456e073c;
      }
      plVar7 = (longlong *)FUN_142058d50(lVar12);
      if (plVar7 != (longlong *)0x0) goto LAB_1410d5be4;
    }
  }
  else {
LAB_1410d5be4:
    lVar9 = FUN_143188280();
    if ((*(int *)(plVar7[2] + 0x90) < *(int *)(lVar9 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar7[2] + 0x88) + (longlong)*(int *)(lVar9 + 0x90) * 8) !=
        lVar9 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) {
      local_48 = 0;
      local_40 = 0x15;
      FUN_1407e2ee0(&local_48,0);
      lVar9 = local_48;
      FUN_1407e4a20(local_48,0x15,"ATreasureAccessPoint",0x15,0x3f);
      plVar10 = (longlong *)FUN_141eb3410(local_res20,"TreasureAccessPoint",1);
      local_res18 = *plVar10;
      lVar12 = lVar13;
      if (plVar7[5] != 0) {
        lVar11 = FUN_141886430();
        lVar2 = local_res18;
        for (lVar8 = plVar7[5]; lVar8 != 0; lVar8 = *(longlong *)(lVar8 + 0x30)) {
          if (lVar8 == lVar11) {
            local_res10 = 0;
            if ((local_res18 != 0) &&
               (plVar7 = (longlong *)(**(code **)(*plVar7 + 600))(plVar7),
               (int)plVar7[1] != *(int *)((longlong)plVar7 + 0x34))) {
              plVar10 = (longlong *)plVar7[8];
              if ((longlong *)plVar7[8] == (longlong *)0x0) {
                plVar10 = plVar7 + 7;
              }
              iVar4 = *(int *)((longlong)plVar10 +
                              ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                              (longlong)(int)plVar7[9] - 1U) * 4);
              if (iVar4 != -1) {
                lVar8 = *plVar7;
                goto LAB_1410d5e20;
              }
            }
            break;
          }
        }
      }
      goto LAB_1410d5cb5;
    }
  }
  goto LAB_1410d5ce2;
  while (iVar4 = *(int *)(lVar8 + 0x10 + lVar11 * 0x18), iVar4 != -1) {
LAB_1410d5e20:
    lVar11 = (longlong)iVar4;
    if (*(longlong *)(lVar8 + lVar11 * 0x18) == lVar2) {
      if (((iVar4 != -1) && (lVar8 = lVar8 + lVar11 * 0x18, lVar8 != 0)) &&
         (plVar7 = (longlong *)(lVar8 + 8), plVar7 != (longlong *)0x0)) {
        lVar12 = *plVar7;
      }
      break;
    }
  }
LAB_1410d5cb5:
  if (lVar9 != 0) {
    FUN_141de9200(lVar9);
  }
  if (lVar12 != 0) {
    *(undefined4 *)((longlong)param_1 + 0x544) = *(undefined4 *)(lVar12 + 0xc);
    *(undefined4 *)(param_1 + 0xa9) = *(undefined4 *)(lVar12 + 0x10);
    *(undefined4 *)((longlong)param_1 + 0x54c) = *(undefined4 *)(lVar12 + 8);
  }
LAB_1410d5ce2:
  lVar6 = *(longlong *)(lVar6 + 0x1ef0);
  lVar12 = lVar6 + 0x26d8;
  lVar9 = FUN_142058d50(lVar12);
  if (lVar9 == 0) {
    LOCK();
    if (DAT_1456e073c == 0) {
      DAT_1456e073c = 0;
    }
    UNLOCK();
    if ((DAT_1456e073c != *(int *)(lVar6 + 0x26e0)) &&
       (local_res10 = 0, *(longlong *)(lVar6 + 0x26e8) != 0)) {
      lVar9 = FUN_14204ac70();
      FUN_1420575e0(lVar12,lVar9);
      if ((lVar9 != 0) || (DAT_145b9d472 == '\0')) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        *(int *)(lVar6 + 0x26e0) = DAT_1456e073c;
      }
      lVar9 = FUN_142058d50(lVar12);
      if (lVar9 != 0) goto LAB_1410d5d6d;
    }
LAB_1410d5e6e:
    lVar9 = lVar13;
  }
  else {
LAB_1410d5d6d:
    lVar6 = FUN_143186ca0();
    if ((*(int *)(*(longlong *)(lVar9 + 0x10) + 0x90) < *(int *)(lVar6 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar9 + 0x10) + 0x88) + (longlong)*(int *)(lVar6 + 0x90) * 8)
        != lVar6 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1410d5e6e;
  }
  param_1[0xab] = lVar9;
LAB_1410d5e82:
  *(undefined4 *)(param_1 + 0xaa) = 0;
  cVar3 = FUN_14205a760(param_1 + 0xa0);
  if (cVar3 != '\0') {
    plVar7 = (longlong *)FUN_142058d50(param_1 + 0xa0);
    local_40 = CONCAT44(local_40._4_4_,0x3f800000);
    local_50 = 0x3f800000;
    local_58 = CONCAT44(DAT_1450d6c88,DAT_1450d6c88);
    (**(code **)(*plVar7 + 0x3c8))(plVar7,&local_58);
  }
  return;
}



######## dec 0x1410d3cf0
// ---- FUN_1410d3cf0 @ 1410d3cf0 size=0x9c

void FUN_1410d3cf0(longlong param_1,int param_2)

{
  longlong lVar1;
  longlong *plVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  
  if (param_2 != 0) {
    *(undefined1 *)(param_1 + 0x108) = 0;
    plVar2 = *(longlong **)(param_1 + 0x128);
    uVar4 = 0;
    uVar3 = (longlong)*(int *)(param_1 + 0x130) & 0x1fffffffffffffff;
    if (plVar2 + *(int *)(param_1 + 0x130) < plVar2) {
      uVar3 = uVar4;
    }
    if (uVar3 != 0) {
      do {
        lVar1 = *plVar2;
        if ((lVar1 != 0) && (*(char *)(lVar1 + 0x4d6) != '\v')) {
          *(ushort *)(lVar1 + 0x4a0) =
               *(ushort *)(lVar1 + 0x4a0) & ~(ushort)(1 << ((int)(short)param_2 & 0x1fU));
          FUN_1410d7a00();
        }
        plVar2 = plVar2 + 1;
        uVar4 = uVar4 + 1;
      } while (uVar4 != uVar3);
    }
  }
  return;
}



######## callers 0x1410d3cf0
  ref from 1460bb678 (DATA) in <none>
  ref from 14519e410 (DATA) in <none>
  ref from 14519e420 (DATA) in <none>
  ref from 1410c9466 (DATA) in FUN_1410c9190@1410c9190
  ref from 1410c946d (DATA) in FUN_1410c9190@1410c9190

######## dec 0x1410cd6c0
// ---- FUN_1410cd6c0 @ 1410cd6c0 size=0xca

/* WARNING: Removing unreachable block (ram,0x0001410cd721) */

void FUN_1410cd6c0(longlong param_1)

{
  bool bVar1;
  longlong lVar2;
  longlong lVar3;
  
  lVar3 = *(longlong *)(param_1 + 0x30);
  if (lVar3 != 0) {
    lVar2 = FUN_14190e4e0();
    if ((*(int *)(lVar2 + 0x90) <= *(int *)(*(longlong *)(lVar3 + 0x10) + 0x90)) &&
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar2 + 0x90) * 8)
        == lVar2 + 0x88)) {
      FUN_1410d7a00(lVar3);
      return;
    }
    FUN_1410d7a00(0);
    return;
  }
  lVar3 = *(longlong *)(param_1 + 0x28);
  if (lVar3 != 0) {
    lVar2 = FUN_14190e4e0();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar2 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar2 + 0x90) * 8)
        != lVar2 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) goto LAB_1410cd778;
  }
  lVar3 = 0;
LAB_1410cd778:
  FUN_1410d7a00(lVar3);
  return;
}



######## callers 0x1410cd6c0
  ref from 1460bae38 (DATA) in <none>
  ref from 143ca5698 (DATA) in <none>
  dataptr @0x143ca5698

######## dec 0x1410d9b00
// ---- FUN_1410d9b00 @ 1410d9b00 size=0x214

void FUN_1410d9b00(ulonglong param_1)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  ulonglong uVar8;
  uint uVar9;
  ulonglong local_res8;
  undefined4 local_res10 [2];
  undefined8 local_res18 [2];
  ulonglong uVar10;
  
  local_res8 = param_1;
  FUN_1408573e0(*(longlong *)(param_1 + 0x570) + 0x110,&local_res8);
  uVar10 = 0;
  lVar6 = FUN_140d23070(DAT_145c95cd8,0);
  uVar8 = uVar10;
  if (lVar6 != 0) {
    uVar8 = *(ulonglong *)(lVar6 + 0x3d0);
    if (uVar8 != 0) {
      lVar6 = FUN_14175cbd0();
      if ((*(int *)(*(longlong *)(uVar8 + 0x10) + 0x90) < *(int *)(lVar6 + 0x90)) ||
         (*(longlong *)
           (*(longlong *)(*(longlong *)(uVar8 + 0x10) + 0x88) + (longlong)*(int *)(lVar6 + 0x90) * 8
           ) != lVar6 + 0x88)) {
        bVar1 = false;
      }
      else {
        bVar1 = true;
      }
      if (bVar1) goto LAB_1410d9b8d;
    }
    uVar8 = 0;
  }
LAB_1410d9b8d:
  lVar6 = FUN_140d28a90();
  iVar3 = FUN_141147080(*(undefined8 *)(param_1 + 0x528),1);
  iVar4 = FUN_141147080(*(undefined8 *)(param_1 + 0x528),0);
  if (0 < iVar4 + iVar3) {
    do {
      local_res18[0] = 0;
      cVar2 = FUN_141139790(*(undefined8 *)(param_1 + 0x528),local_res18,local_res10,&local_res8,
                            (int)uVar10 < iVar3);
      if (cVar2 != '\0') {
        if (uVar8 != 0) {
          FUN_1411e3090(uVar8,local_res18,local_res10[0],1);
        }
        if (lVar6 != 0) {
          FUN_141224670(lVar6,local_res18,local_res10[0],0);
          FUN_1410df6c0(param_1,local_res18[0],local_res10[0]);
          lVar7 = FUN_140d28b40();
          if (lVar7 != 0) {
            iVar5 = FUN_141237f40(lVar7,local_res18);
            if (*(int *)(param_1 + 0x3f8) < iVar5) {
              *(int *)(param_1 + 0x3f8) = iVar5;
            }
            if (*(char *)(param_1 + 0x4d4) == '\0') {
              *(undefined4 *)(param_1 + 0x3f8) = 4;
            }
          }
        }
      }
      uVar9 = (int)uVar10 + 1;
      uVar10 = (ulonglong)uVar9;
    } while ((int)uVar9 < iVar4 + iVar3);
  }
  if (*(char *)(param_1 + 0x4d4) == '\0') {
    cVar2 = -0x79;
  }
  else {
    cVar2 = (1 < *(int *)(param_1 + 0x3f8)) + '\x15';
  }
  FUN_141485f00(cVar2,DAT_1450d6c88,0,0);
  lVar6 = FUN_140d239b0();
  lVar6 = *(longlong *)(lVar6 + 0x1180);
  if ((lVar6 != 0) && (*(char *)(param_1 + 0x562) != '\0')) {
    *(undefined1 *)(lVar6 + 0x2ec) = 1;
    *(undefined1 *)(param_1 + 0x562) = 0;
  }
  FUN_14136d2b0(CONCAT71((int7)((ulonglong)lVar6 >> 8),0x18),param_1);
  FUN_14136d2b0(0x1a,param_1);
  return;
}



######## callers 0x1410d9df0
  ref from 1460bc038 (DATA) in <none>
  ref from 144304080 (DATA) in <none>
  dataptr @0x144304080

/* ================================ RUN 5 ================================
# run 5: confirm Super::BeginPlay identity, delegate binder
dec 0x142b0bd40
dref 0x142b0bd40
dref 0x1410cb580
dec 0x1410c9750
callers 0x143ca5698
str ReceiveBeginPlay
*/

######## dec 0x142b0bd40
// ---- FUN_142b0bd40 @ 142b0bd40 size=0x1c4

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_142b0bd40(longlong *param_1)

{
  longlong *plVar1;
  char cVar2;
  longlong lVar3;
  longlong *plVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  undefined1 auStack_128 [32];
  undefined1 local_108 [8];
  code *local_100;
  undefined1 *local_f8;
  longlong local_f0 [24];
  longlong *local_30;
  undefined8 local_28;
  ulonglong local_20;
  
  local_20 = DAT_145909780 ^ (ulonglong)auStack_128;
  (**(code **)(*param_1 + 0x370))(param_1,*(undefined4 *)((longlong)param_1 + 0xcc));
  cVar2 = FUN_1420444f0(param_1,0x30);
  uVar5 = 0;
  if ((cVar2 == '\0') && ((*(byte *)((longlong)param_1 + 0x93) & 0x20) == 0)) {
    local_108[0] = 0;
    local_f8 = local_108;
    local_100 = FUN_142b0ce60;
    lVar3 = FUN_141de9cd0(&local_100,&DAT_145755848);
    (**(code **)(*param_1 + 0x518))(param_1,1);
    *(byte *)((longlong)param_1 + 0x93) = *(byte *)((longlong)param_1 + 0x93) | 0x20;
    *(undefined8 *)(lVar3 + 0x10) = 0;
  }
  local_30 = (longlong *)0x0;
  local_28 = 0;
  FUN_141a49dc0(param_1,local_f0,0);
  *(byte *)((longlong)param_1 + 0x94) = *(byte *)((longlong)param_1 + 0x94) & 0xef;
  *(byte *)((longlong)param_1 + 0x94) = *(byte *)((longlong)param_1 + 0x94) | 8;
  plVar4 = local_f0;
  if (local_30 != (longlong *)0x0) {
    plVar4 = local_30;
  }
  uVar6 = (longlong)(int)local_28 & 0x1fffffffffffffff;
  if (plVar4 + (int)local_28 < plVar4) {
    uVar6 = uVar5;
  }
  if (uVar6 != 0) {
    do {
      plVar1 = (longlong *)*plVar4;
      if (((*(byte *)(plVar1 + 0x17) & 1) != 0) && ((*(byte *)((longlong)plVar1 + 0xbb) & 2) == 0))
      {
        FUN_142c22390(plVar1,1);
        (**(code **)(*plVar1 + 0x308))(plVar1);
      }
      plVar4 = plVar4 + 1;
      uVar5 = uVar5 + 1;
    } while (uVar5 != uVar6);
  }
  FUN_14314bb70(param_1);
  *(byte *)((longlong)param_1 + 0x94) = *(byte *)((longlong)param_1 + 0x94) & 0xf7 | 0x10;
  if (local_30 != (longlong *)0x0) {
    FUN_141de9200();
  }
  return;
}



######## dref 0x142b0bd40
  dataptr @0x143963b38
  dataptr @0x143964190
  dataptr @0x143964800
  dataptr @0x143964e40
  dataptr @0x1439668f0
  dataptr @0x143966ff8
  dataptr @0x143967ba8
  dataptr @0x143968220
  dataptr @0x143973900
  dataptr @0x1439f6830
  dataptr @0x1439f9348
  dataptr @0x143a1b708
  dataptr @0x143a1d838
  dataptr @0x143a54e30
  dataptr @0x143a55978
  dataptr @0x143a55f98
  dataptr @0x143a84508
  dataptr @0x143a954c8
  dataptr @0x143a9b078
  dataptr @0x143af33c8
  dataptr @0x143af3a38
  dataptr @0x143af4358
  dataptr @0x143af4c90
  dataptr @0x143af55b0
  dataptr @0x143b02330
  dataptr @0x143b266d8
  dataptr @0x143b26cf0
  dataptr @0x143b3b890
  dataptr @0x143bcfdb0
  dataptr @0x143bd6a40
  dataptr @0x143c0e860
  dataptr @0x143c0ee78
  dataptr @0x143c1db80
  dataptr @0x143c1e7d8
  dataptr @0x143c2b050
  dataptr @0x143c4f060
  dataptr @0x143c4f688
  dataptr @0x143c4fcb0
  dataptr @0x143c502e8
  dataptr @0x143c50d08
  dataptr @0x143c5d098
  dataptr @0x143c6c448
  dataptr @0x143caa4f0
  dataptr @0x143cab960
  dataptr @0x143cb88b8
  dataptr @0x143cbdb58
  dataptr @0x143ccf9d0
  dataptr @0x143cdb0b8
  dataptr @0x143cfe3d8
  dataptr @0x143d25a88
  dataptr @0x143d4be78
  dataptr @0x143d4c498
  dataptr @0x143db6620
  dataptr @0x143dfbb18
  dataptr @0x143dfd7b0
  dataptr @0x143e0c848
  dataptr @0x143f2e7b0
  dataptr @0x143f39800
  dataptr @0x143f41190
  dataptr @0x14421ad90
  dataptr @0x14427a160
  dataptr @0x14431d900
  dataptr @0x1443c1770
  dataptr @0x1443c1db8
  dataptr @0x1443dcb80
  dataptr @0x1443e2180
  dataptr @0x1443e27d8
  dataptr @0x1443e2e30
  dataptr @0x1443e3488
  dataptr @0x1443e3ae0
  dataptr @0x1443e4138
  dataptr @0x1443e4790
  dataptr @0x1443e4de8
  dataptr @0x1443e5440
  dataptr @0x1443e5a98
  dataptr @0x1443e60f0
  dataptr @0x1443e6748
  dataptr @0x1443e6dc8
  dataptr @0x1443f5668
  dataptr @0x1443f6318
  dataptr @0x1443f6970
  dataptr @0x1443f7620
  dataptr @0x1443f7c78
  dataptr @0x1443f8928
  dataptr @0x1443f8f88
  dataptr @0x1443fa2b0
  dataptr @0x1443fa908
  dataptr @0x1443faf60
  dataptr @0x1443fb5b8
  dataptr @0x1443fbc10
  dataptr @0x1443fc268
  dataptr @0x1443fc8c0
  dataptr @0x1443fcf18
  dataptr @0x1443fd570
  dataptr @0x1443fdbc8
  dataptr @0x1443fe220
  dataptr @0x1443fe878
  dataptr @0x1443ff528
  dataptr @0x1443ffb80
  dataptr @0x1444001d8
  dataptr @0x144401df0
  dataptr @0x144402448
  dataptr @0x144402aa0
  dataptr @0x1444030f8
  dataptr @0x144404400
  dataptr @0x144404a58
  dataptr @0x1444050b0
  dataptr @0x144405708
  dataptr @0x14440e9b8
  dataptr @0x14440f010
  dataptr @0x144411120
  dataptr @0x144411778
  dataptr @0x144411dd0
  dataptr @0x144412428
  dataptr @0x144412a80
  dataptr @0x1444130f8
  dataptr @0x144413770
  dataptr @0x144413dc8
  dataptr @0x144415db0
  dataptr @0x144416a70
  dataptr @0x144417720
  dataptr @0x144418a70
  dataptr @0x1444190c8
  dataptr @0x1444488d0
  dataptr @0x144448ef0
  dataptr @0x144449510
  dataptr @0x144449b30
  dataptr @0x1444f9fc8
  dataptr @0x1444fa628
  dataptr @0x1444faca8
  dataptr @0x144508028
  dataptr @0x144509c98
  dataptr @0x14450f328
  dataptr @0x14453c768
  dataptr @0x144668d08
  dataptr @0x1446e6e40
  dataptr @0x144949c30
  dataptr @0x14494a618
  dataptr @0x144953cb0
  dataptr @0x1449f9f68
  dataptr @0x1449fa580
  dataptr @0x144bad718
  dataptr @0x144bc01c8
  dataptr @0x144bc60c8
  dataptr @0x144bccdc8
  dataptr @0x144bcd408
  dataptr @0x144bce040
  dataptr @0x144bceb70
  dataptr @0x144becaf0
  dataptr @0x144bfc450
  dataptr @0x144bff218
  dataptr @0x144c031b8
  dataptr @0x144c048f8
  dataptr @0x144c19608
  dataptr @0x144c27770
  dataptr @0x144c27f30
  dataptr @0x144c28828
  dataptr @0x144c3d720
  dataptr @0x144c3f380
  dataptr @0x144c49ad0
  dataptr @0x144c4a1e8
  dataptr @0x144c4a888
  dataptr @0x144c4fde8
  dataptr @0x144c5cb88
  dataptr @0x144c63c10
  dataptr @0x144c6a3e0
  dataptr @0x144c6a9f8
  dataptr @0x144c6b4e0
  dataptr @0x144c6bdd0
  dataptr @0x144c6c3f0
  dataptr @0x144c6ca10
  dataptr @0x144c6d958
  dataptr @0x144c93250
  dataptr @0x144c9faa0
  dataptr @0x144ca1ac8
  dataptr @0x144caae30
  dataptr @0x144cb00f0
  dataptr @0x144ccded8
  dataptr @0x144cd8488
  dataptr @0x144ce0ef8
  dataptr @0x144ce1f08
  dataptr @0x144ce2670
  dataptr @0x144ce2d58
  dataptr @0x144ce3698
  dataptr @0x144ceaa48
  dataptr @0x144d1d850
  dataptr @0x144d1e560
  dataptr @0x144d4dff8
  dataptr @0x144ec7988
  dataptr @0x144ec84e8
  dataptr @0x144eca080
  dataptr @0x144ecae28
  dataptr @0x144ecb7c8
  dataptr @0x144ecf318
  dataptr @0x144ecf9b0
  dataptr @0x144ee1090
  dataptr @0x144ef7c40
  dataptr @0x144f012f0

######## dref 0x1410cb580
  dataptr @0x143ca5218

######## dec 0x1410c9750
// ---- FUN_1410c9750 @ 1410c9750 size=0xbb

void FUN_1410c9750(longlong param_1)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined1 local_res10 [24];
  code *local_18;
  undefined4 uStack_10;
  
  lVar1 = FUN_140d276d0();
  if (lVar1 != 0) {
    local_18 = FUN_1410da8f0;
    uStack_10 = 0;
    puVar2 = (undefined8 *)FUN_140bd3240(lVar1,local_res10,0x3d,param_1,&local_18);
    *(undefined8 *)(param_1 + 0x530) = *puVar2;
    local_18 = FUN_1410da7f0;
    uStack_10 = 0;
    puVar2 = (undefined8 *)FUN_140bd3240(lVar1,local_res10,0x3e,param_1,&local_18);
    *(undefined8 *)(param_1 + 0x538) = *puVar2;
  }
  return;
}



######## callers 0x143ca5698

######## str ReceiveBeginPlay
ascii 144d51660
  dataptr @0x144d50188
ascii 144d52728
  dataptr @0x14575f028
ascii 144d552f8
  dataptr @0x145761a88
ascii 144d56798
  dataptr @0x144d563a8
utf16 14474a124
utf16 14475440e
utf16 144d52558
   xref 14074d3e6 in <none>
utf16 144d543a8
   xref 14074d6b6 in <none>
