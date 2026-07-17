// FUN_142bfe8d0 @ 142bfe8d0

void FUN_142bfe8d0(longlong param_1)

{
  longlong *plVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  
  if ((*(uint *)(param_1 + 8) >> 4 & 1) == 0) {
    lVar4 = *(longlong *)(param_1 + 0x20);
  }
  else {
    lVar4 = 0;
  }
  lVar4 = *(longlong *)(lVar4 + 0x468);
  if (lVar4 != 0) {
    plVar1 = (longlong *)FUN_1431b68d0();
    lVar2 = plVar1[0x20];
    if (lVar2 == 0) {
      (**(code **)(*plVar1 + 0x340))(plVar1);
      lVar2 = plVar1[0x20];
    }
    plVar3 = *(longlong **)(lVar2 + 0x28);
    plVar1 = plVar3 + (longlong)*(int *)(lVar2 + 0x30) * 3;
    for (; plVar3 != plVar1; plVar3 = plVar3 + 3) {
      if (*plVar3 == DAT_145b9ee30) {
        uVar5 = FUN_143111e90(lVar4);
        uVar6 = *(undefined4 *)((longlong)plVar3 + 0xc);
LAB_142bfe977:
        FUN_143120aa0(lVar4,uVar6,uVar5);
      }
      else if (*plVar3 == DAT_145b9ee48) {
        uVar6 = FUN_143111e20(lVar4);
        uVar5 = *(undefined4 *)((longlong)plVar3 + 0xc);
        goto LAB_142bfe977;
      }
    }
  }
  return;
}


