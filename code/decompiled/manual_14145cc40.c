// FUN_14145cc40 @ 14145cc40

undefined8 * FUN_14145cc40(longlong param_1,longlong param_2)

{
  longlong lVar1;
  int iVar2;
  longlong lVar3;
  longlong *plVar4;
  longlong *plVar5;
  undefined8 *puVar6;
  undefined8 *puVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  longlong local_18 [2];
  
  local_18[1] = 0x16;
  puVar8 = (undefined8 *)0x0;
  local_18[0] = 0;
  FUN_1407e2ee0(local_18,0);
  FUN_1407e4a20(local_18[0],0x16,"PassiveSkillDataTable",0x16,0x3f);
  plVar4 = *(longlong **)(param_1 + 0x118);
  puVar9 = puVar8;
  if (plVar4[5] != 0) {
    lVar3 = FUN_1418daf50();
    for (lVar1 = plVar4[5]; lVar1 != 0; lVar1 = *(longlong *)(lVar1 + 0x30)) {
      if (lVar1 == lVar3) {
        puVar9 = (undefined8 *)0x0;
        if (param_2 != 0) {
          plVar4 = (longlong *)(**(code **)(*plVar4 + 600))(plVar4);
          if ((int)plVar4[1] == *(int *)((longlong)plVar4 + 0x34)) goto LAB_14145cd60;
          plVar5 = (longlong *)plVar4[8];
          if ((longlong *)plVar4[8] == (longlong *)0x0) {
            plVar5 = plVar4 + 7;
          }
          iVar2 = *(int *)((longlong)plVar5 +
                          ((longlong)((int)((ulonglong)param_2 >> 0x20) + (int)param_2) &
                          (longlong)(int)plVar4[9] - 1U) * 4);
          if (iVar2 == -1) goto LAB_14145cd60;
          goto LAB_14145cd40;
        }
        break;
      }
    }
  }
  goto LAB_14145cd76;
  while (iVar2 = *(int *)(puVar7 + 2), iVar2 != -1) {
LAB_14145cd40:
    puVar7 = (undefined8 *)(*plVar4 + (longlong)iVar2 * 0x18);
    if (*(longlong *)(*plVar4 + (longlong)iVar2 * 0x18) == param_2) {
      if (iVar2 != -1) goto LAB_14145cd63;
      break;
    }
  }
LAB_14145cd60:
  puVar7 = puVar8;
LAB_14145cd63:
  puVar6 = puVar7 + 1;
  if (puVar7 == (undefined8 *)0x0) {
    puVar6 = puVar8;
  }
  if (puVar6 != (undefined8 *)0x0) {
    puVar9 = (undefined8 *)*puVar6;
  }
LAB_14145cd76:
  if (local_18[0] != 0) {
    FUN_141de9200(local_18[0]);
  }
  return puVar9;
}


