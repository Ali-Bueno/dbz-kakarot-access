// FUN_14144bff0 @ 14144bff0

void FUN_14144bff0(longlong param_1,int param_2,longlong param_3)

{
  longlong *plVar1;
  longlong lVar2;
  int iVar3;
  longlong lVar4;
  longlong lVar5;
  longlong *plVar6;
  longlong *plVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  undefined8 *puVar10;
  longlong lVar11;
  longlong *plVar12;
  longlong *plVar13;
  longlong local_res20;
  longlong local_60;
  undefined8 local_58;
  longlong *local_50;
  int local_48;
  
  FUN_142cb8ad0(*(undefined8 *)(param_1 + 0x108),&local_50);
  plVar1 = local_50 + local_48;
  plVar13 = local_50;
joined_r0x00014144c038:
  if (plVar13 == plVar1) {
    if (local_50 != (longlong *)0x0) {
      FUN_141de9200(local_50);
    }
    return;
  }
  puVar9 = (undefined8 *)0x0;
  local_60 = 0;
  local_58 = 0xd;
  FUN_1407e2ee0(&local_60,0);
  lVar11 = local_60;
  FUN_1407e4a20(local_60,0xd,"SkillManager",0xd,0x3f);
  plVar6 = *(longlong **)(param_1 + 0x108);
  local_res20 = *plVar13;
  if (plVar6[5] != 0) {
    lVar4 = FUN_1419040a0();
    lVar2 = local_res20;
    for (lVar5 = plVar6[5]; lVar5 != 0; lVar5 = *(longlong *)(lVar5 + 0x30)) {
      if (lVar5 == lVar4) {
        if (local_res20 != 0) {
          plVar6 = (longlong *)(**(code **)(*plVar6 + 600))(plVar6);
          if ((int)plVar6[1] == *(int *)((longlong)plVar6 + 0x34)) goto LAB_14144c200;
          plVar7 = (longlong *)plVar6[8];
          if ((longlong *)plVar6[8] == (longlong *)0x0) {
            plVar7 = plVar6 + 7;
          }
          iVar3 = *(int *)((longlong)plVar7 +
                          ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                          (longlong)(int)plVar6[9] - 1U) * 4);
          if (iVar3 == -1) goto LAB_14144c200;
          goto LAB_14144c1e0;
        }
        break;
      }
    }
  }
  goto LAB_14144c0da;
  while (iVar3 = *(int *)(puVar8 + 2), iVar3 != -1) {
LAB_14144c1e0:
    puVar8 = (undefined8 *)(*plVar6 + (longlong)iVar3 * 0x18);
    if (*(longlong *)(*plVar6 + (longlong)iVar3 * 0x18) == lVar2) {
      if (iVar3 != -1) goto LAB_14144c203;
      break;
    }
  }
LAB_14144c200:
  puVar8 = puVar9;
LAB_14144c203:
  puVar10 = puVar8 + 1;
  if (puVar8 == (undefined8 *)0x0) {
    puVar10 = puVar9;
  }
  if (puVar10 != (undefined8 *)0x0) {
    puVar9 = (undefined8 *)*puVar10;
  }
LAB_14144c0da:
  if (lVar11 != 0) {
    FUN_141de9200(lVar11);
  }
  if (((*(int *)(puVar9 + 3) == param_2) && (puVar9[4] == param_3)) && (*(int *)(puVar9 + 5) == 1))
  {
    lVar11 = *plVar13;
    local_res20 = lVar11;
    lVar5 = FUN_14145e140(*(undefined8 *)(param_1 + 0x130));
    if (lVar5 != 0) {
      plVar6 = *(longlong **)(lVar5 + 0xf0);
      for (plVar7 = plVar6; plVar7 != plVar6 + *(int *)(lVar5 + 0xf8); plVar7 = plVar7 + 1) {
        if (*plVar7 == lVar11) {
          if ((int)((longlong)plVar7 - (longlong)plVar6 >> 3) != -1) {
            FUN_140b492f0(lVar5 + 0xf0,&local_res20);
            lVar11 = local_res20;
          }
          break;
        }
      }
      plVar12 = (longlong *)(lVar5 + 0x100);
      plVar6 = (longlong *)*plVar12;
      iVar3 = *(int *)(lVar5 + 0x108);
      for (plVar7 = plVar6; plVar7 != plVar6 + iVar3; plVar7 = plVar7 + 1) {
        if (*plVar7 == lVar11) {
          if ((int)((longlong)plVar7 - (longlong)plVar6 >> 3) != -1) goto LAB_14144c2e0;
          break;
        }
      }
      *(int *)(lVar5 + 0x108) = iVar3 + 1;
      if (*(int *)(lVar5 + 0x10c) < iVar3 + 1) {
        FUN_1407f1510(plVar12,iVar3);
      }
      *(longlong *)((longlong)iVar3 * 8 + *plVar12) = lVar11;
      thunk_FUN_1419ec840(&DAT_14543f210,1);
      thunk_FUN_1419ec840(&DAT_14543f220,1);
      thunk_FUN_1419ec840(&DAT_14543f230);
    }
  }
LAB_14144c2e0:
  plVar13 = plVar13 + 1;
  goto joined_r0x00014144c038;
}


