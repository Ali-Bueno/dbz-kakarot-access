// FUN_14145e360 @ 14145e360

undefined8 * FUN_14145e360(longlong param_1,longlong param_2)

{
  char cVar1;
  int iVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *plVar5;
  undefined8 *puVar6;
  longlong lVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  undefined8 *puVar10;
  
  puVar9 = (undefined8 *)0x0;
  cVar1 = FUN_14205a760(param_1 + 0x28);
  puVar10 = puVar9;
  if ((cVar1 != '\0') && (plVar3 = (longlong *)FUN_142058d50(param_1 + 0x28), plVar3[5] != 0)) {
    lVar4 = FUN_1419040a0();
    lVar7 = plVar3[5];
    if (lVar7 != 0) {
      while (lVar7 != lVar4) {
        lVar7 = *(longlong *)(lVar7 + 0x30);
        if (lVar7 == 0) {
          return (undefined8 *)0x0;
        }
      }
      puVar10 = (undefined8 *)0x0;
      if (param_2 != 0) {
        plVar3 = (longlong *)(**(code **)(*plVar3 + 600))(plVar3);
        if ((int)plVar3[1] != *(int *)((longlong)plVar3 + 0x34)) {
          plVar5 = (longlong *)plVar3[8];
          if ((longlong *)plVar3[8] == (longlong *)0x0) {
            plVar5 = plVar3 + 7;
          }
          iVar2 = *(int *)((longlong)plVar5 +
                          ((longlong)((int)((ulonglong)param_2 >> 0x20) + (int)param_2) &
                          (longlong)(int)plVar3[9] - 1U) * 4);
          if (iVar2 != -1) {
            do {
              puVar8 = (undefined8 *)(*plVar3 + (longlong)iVar2 * 0x18);
              if (*(longlong *)(*plVar3 + (longlong)iVar2 * 0x18) == param_2) {
                if (iVar2 != -1) goto LAB_14145e463;
                break;
              }
              iVar2 = *(int *)(puVar8 + 2);
            } while (iVar2 != -1);
          }
        }
        puVar8 = puVar9;
LAB_14145e463:
        puVar6 = puVar8 + 1;
        if (puVar8 == (undefined8 *)0x0) {
          puVar6 = puVar9;
        }
        if (puVar6 != (undefined8 *)0x0) {
          puVar10 = (undefined8 *)*puVar6;
        }
      }
    }
  }
  return puVar10;
}


