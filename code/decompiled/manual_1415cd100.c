// FUN_1415cd100 @ 1415cd100

void FUN_1415cd100(longlong param_1,int param_2)

{
  longlong lVar1;
  longlong lVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  int *piVar6;
  longlong lVar7;
  longlong *plVar8;
  int *local_28;
  int local_20;
  
  lVar1 = FUN_1411fd700();
  if ((lVar1 != 0) &&
     (FUN_141593540(lVar1,0), *(int *)(param_1 + 0x6a8) != *(int *)(param_1 + 0x6d4))) {
    piVar6 = (int *)0x0;
    lVar2 = *(longlong *)(param_1 + 0x6e0);
    if (*(longlong *)(param_1 + 0x6e0) == 0) {
      lVar2 = param_1 + 0x6d8;
    }
    iVar5 = *(int *)(lVar2 + ((longlong)*(int *)(param_1 + 0x6e8) - 1U & (longlong)param_2) * 4);
    if (iVar5 != -1) {
      lVar2 = *(longlong *)(param_1 + 0x6a0);
      iVar3 = iVar5;
      while (*(int *)((longlong)iVar3 * 0x20 + lVar2) != param_2) {
        iVar3 = *(int *)((longlong)iVar3 * 0x20 + 0x18 + lVar2);
        if (iVar3 == -1) {
          return;
        }
      }
      if (iVar3 != -1) {
        while (iVar5 != -1) {
          piVar4 = (int *)((longlong)iVar5 * 0x20 + lVar2);
          if (*piVar4 == param_2) {
            if (iVar5 != -1) goto LAB_1415cd1e0;
            break;
          }
          iVar5 = piVar4[6];
        }
        piVar4 = piVar6;
LAB_1415cd1e0:
        iVar5 = piVar4[4];
        lVar2 = *(longlong *)(piVar4 + 2);
        local_28 = (int *)0x0;
        local_20 = iVar5;
        if (iVar5 != 0) {
          FUN_140827070(&local_28,iVar5,0);
          piVar6 = local_28;
          lVar7 = lVar2 + 8;
          do {
            *(undefined1 *)((longlong)piVar6 + ((lVar7 + -8) - lVar2)) = *(undefined1 *)(lVar7 + -8)
            ;
            FUN_1407e5680((undefined1 *)((longlong)piVar6 + (lVar7 - lVar2)),lVar7);
            lVar7 = lVar7 + 0x18;
            iVar5 = iVar5 + -1;
          } while (iVar5 != 0);
        }
        iVar5 = local_20;
        lVar2 = (longlong)local_20;
        for (piVar4 = piVar6; piVar4 != piVar6 + lVar2 * 6; piVar4 = piVar4 + 6) {
          FUN_14159e0a0(lVar1,(char)*piVar4,piVar4 + 2,0);
        }
        FUN_14159fd20(lVar1,0);
        if (iVar5 != 0) {
          plVar8 = (longlong *)(piVar6 + 2);
          do {
            if (*plVar8 != 0) {
              FUN_141de9200();
            }
            plVar8 = plVar8 + 3;
            iVar5 = iVar5 + -1;
          } while (iVar5 != 0);
        }
        if (piVar6 != (int *)0x0) {
          FUN_141de9200(piVar6);
        }
      }
    }
  }
  return;
}


