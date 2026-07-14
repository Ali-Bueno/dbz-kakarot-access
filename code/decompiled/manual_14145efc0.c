// FUN_14145efc0 @ 14145efc0

undefined8 FUN_14145efc0(longlong param_1,int param_2,longlong param_3)

{
  longlong *plVar1;
  longlong lVar2;
  longlong lVar3;
  int iVar4;
  longlong lVar5;
  longlong *plVar6;
  longlong *plVar7;
  undefined8 *puVar8;
  longlong lVar9;
  undefined8 *puVar10;
  undefined8 *puVar11;
  undefined8 *puVar12;
  longlong *plVar13;
  longlong local_48 [2];
  
  lVar2 = *(longlong *)(param_1 + 0x138);
  if ((lVar2 == 0) || (*(longlong *)(param_1 + 0x108) == 0)) {
    return 0;
  }
  if (*(int *)(lVar2 + 0x30) != *(int *)(lVar2 + 0x5c)) {
    puVar12 = (undefined8 *)0x0;
    lVar9 = *(longlong *)(lVar2 + 0x68);
    if (*(longlong *)(lVar2 + 0x68) == 0) {
      lVar9 = lVar2 + 0x60;
    }
    iVar4 = *(int *)(lVar9 + ((longlong)*(int *)(lVar2 + 0x70) - 1U & (longlong)param_2) * 4);
    if (iVar4 != -1) {
      lVar2 = *(longlong *)(lVar2 + 0x28);
      do {
        lVar9 = (longlong)iVar4;
        if (*(int *)(lVar2 + lVar9 * 0x18) == param_2) {
          if (iVar4 == -1) {
            return 0;
          }
          lVar2 = lVar2 + lVar9 * 0x18;
          if (lVar2 == 0) {
            return 0;
          }
          lVar2 = *(longlong *)(lVar2 + 8);
          if (lVar2 == 0) {
            return 0;
          }
          plVar13 = *(longlong **)(lVar2 + 0x100);
          plVar1 = plVar13 + *(int *)(lVar2 + 0x108);
          goto LAB_14145f0bf;
        }
        iVar4 = *(int *)(lVar2 + 0x10 + lVar9 * 0x18);
      } while (iVar4 != -1);
    }
  }
  return 0;
LAB_14145f0bf:
  if (plVar13 == plVar1) {
    return 0;
  }
  lVar2 = *plVar13;
  local_48[0] = 0;
  local_48[1] = 0x13;
  FUN_1407e2ee0(local_48,0);
  lVar9 = local_48[0];
  FUN_1407e4a20(local_48[0],0x13,"SkillTreeDataTable",0x13,0x3f);
  plVar6 = *(longlong **)(param_1 + 0x108);
  puVar10 = puVar12;
  if (plVar6[5] != 0) {
    lVar5 = FUN_1419040a0();
    for (lVar3 = plVar6[5]; lVar3 != 0; lVar3 = *(longlong *)(lVar3 + 0x30)) {
      if (lVar3 == lVar5) {
        if (lVar2 != 0) {
          plVar6 = (longlong *)(**(code **)(*plVar6 + 600))(plVar6);
          if ((int)plVar6[1] == *(int *)((longlong)plVar6 + 0x34)) goto LAB_14145f1f0;
          plVar7 = (longlong *)plVar6[8];
          if ((longlong *)plVar6[8] == (longlong *)0x0) {
            plVar7 = plVar6 + 7;
          }
          iVar4 = *(int *)((longlong)plVar7 +
                          ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                          (longlong)(int)plVar6[9] - 1U) * 4);
          if (iVar4 == -1) goto LAB_14145f1f0;
          goto LAB_14145f1d0;
        }
        break;
      }
    }
  }
  goto LAB_14145f141;
  while (iVar4 = *(int *)(puVar8 + 2), iVar4 != -1) {
LAB_14145f1d0:
    puVar8 = (undefined8 *)(*plVar6 + (longlong)iVar4 * 0x18);
    if (*(longlong *)(*plVar6 + (longlong)iVar4 * 0x18) == lVar2) {
      if (iVar4 != -1) goto LAB_14145f1f3;
      break;
    }
  }
LAB_14145f1f0:
  puVar8 = puVar12;
LAB_14145f1f3:
  puVar11 = puVar8 + 1;
  if (puVar8 == (undefined8 *)0x0) {
    puVar11 = puVar12;
  }
  if (puVar11 != (undefined8 *)0x0) {
    puVar10 = (undefined8 *)*puVar11;
  }
LAB_14145f141:
  if (lVar9 != 0) {
    FUN_141de9200(lVar9);
  }
  if ((puVar10 != (undefined8 *)0x0) && (puVar10[4] == param_3)) {
    return 1;
  }
  plVar13 = plVar13 + 1;
  goto LAB_14145f0bf;
}


