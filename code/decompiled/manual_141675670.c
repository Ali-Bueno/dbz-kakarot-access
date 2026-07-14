// FUN_141675670 @ 141675670

void FUN_141675670(longlong param_1,int param_2,undefined8 param_3)

{
  longlong lVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  int iVar5;
  longlong lVar6;
  undefined8 uVar7;
  longlong lVar8;
  undefined8 uVar9;
  undefined8 *puVar10;
  longlong *plVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  char cVar14;
  int iVar15;
  undefined2 *puVar16;
  ulonglong uVar17;
  undefined4 uVar18;
  undefined8 local_res8;
  undefined8 local_res18;
  longlong local_res20;
  undefined1 local_58 [16];
  longlong local_48 [2];
  
  lVar1 = (longlong)param_2 * 8;
  *(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x418) = param_3;
  local_res18 = param_3;
  lVar6 = FUN_140d2aef0();
  if (lVar6 == 0) {
    uVar9 = 0;
  }
  else {
    uVar9 = *(undefined8 *)(lVar6 + 0x130);
  }
  cVar3 = '\0';
  uVar18 = 0;
  uVar7 = FUN_140d2aef0();
  lVar6 = FUN_14145e360(uVar9,local_res18);
  cVar2 = FUN_141462a10(uVar9,local_res18);
  if (cVar2 == '\0') {
    lVar8 = FUN_14145d670(uVar7,*(undefined8 *)(lVar6 + 0x20));
    if (lVar8 != 0) {
      if (*(int *)(lVar6 + 0x28) == 1) {
        cVar3 = *(char *)(lVar8 + 100);
      }
      goto LAB_141675722;
    }
  }
  else {
    lVar8 = FUN_14145cc40();
    uVar18 = 0;
    if (lVar8 != 0) {
      if (*(int *)(lVar6 + 0x28) == 1) {
        cVar3 = *(char *)(lVar8 + 0x62);
      }
LAB_141675722:
      uVar18 = *(undefined4 *)(lVar6 + 0x88);
    }
  }
  plVar11 = *(longlong **)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x3c8);
  if (plVar11 != (longlong *)0x0) {
    if (cVar3 == '\0') {
      (**(code **)(*plVar11 + 0x260))();
    }
    else {
      (**(code **)(*plVar11 + 0x250))(plVar11,3);
    }
  }
  FUN_141a83cf0(*(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x3d0),
                uVar18);
  cVar3 = FUN_14145c4b0(uVar9,*(undefined4 *)(param_1 + 0x1584),&local_res18);
  if (cVar3 == '\0') {
    cVar3 = FUN_141462ac0(uVar9);
    FUN_14166c2a0(*(undefined8 *)(lVar1 + *(longlong *)(param_1 + 0x438)),cVar3 != '\0');
    lVar6 = FUN_1411fc870();
    if (lVar6 != 0) {
      local_res8 = 0;
      local_res20 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x470);
      if ((local_res20 != 0) && (cVar3 = FUN_14107d720(lVar6,&local_res20), cVar3 == '\0')) {
        FUN_14166c2a0(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),1);
      }
    }
    cVar3 = FUN_141367d80();
    if ((((cVar3 == '\0') &&
         (local_res8 = 0,
         *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x478) != 0)) &&
        (lVar6 = FUN_140d2aa10(), lVar6 != 0)) &&
       (cVar3 = FUN_1413dd4c0(lVar6,*(undefined8 *)
                                     (*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x478)
                             ), cVar3 == '\0')) {
      FUN_14166c2a0(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),1);
    }
    lVar6 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x470);
    if (lVar6 == DAT_145986928) {
      if (1 < *(int *)(param_1 + 0x1584) - 0x2aU) {
        return;
      }
      cVar3 = FUN_1412e08a0();
      if (cVar3 == '\0') goto LAB_1416758f1;
      cVar3 = FUN_1413a34d0();
    }
    else {
      if (lVar6 != DAT_145986940) {
        return;
      }
      lVar6 = FUN_140d2a6d0();
      if (lVar6 == 0) {
        return;
      }
      if (*(int *)(param_1 + 0x1584) != 1) {
        return;
      }
      cVar3 = FUN_1412e0630();
      if (cVar3 == '\0') goto LAB_1416758f1;
      uVar9 = FUN_1412ddc30(lVar6);
      cVar3 = FUN_140c51ef0(uVar9,L"MainEp_Finish");
    }
    if (cVar3 != '\0') {
      return;
    }
LAB_1416758f1:
    FUN_14166c2a0(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),1);
    return;
  }
  cVar2 = FUN_14165eda0(param_1,local_res18);
  cVar4 = FUN_141461e60(uVar9,local_res18);
  if (cVar4 == '\0') {
    cVar14 = '\0';
    cVar4 = FUN_141462ac0(uVar9,local_res18);
    if (cVar4 == '\0') {
      if (cVar3 == '\x01') {
        cVar14 = (cVar2 == '\0') + '\x06';
      }
      else if (cVar3 == '\x02') {
        cVar14 = '\x05';
        FUN_14166bc40(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),1);
      }
      uVar7 = 4;
      goto LAB_141675b1a;
    }
    local_res8 = local_res18;
    lVar6 = FUN_140d2aef0();
    if ((lVar6 == 0) || (lVar6 = *(longlong *)(lVar6 + 0x130), lVar6 == 0)) {
LAB_141675a53:
      cVar14 = '\0';
      uVar7 = 5;
    }
    else {
      iVar5 = FUN_14145dff0(lVar6,local_res8);
      if (iVar5 != 1) {
        iVar5 = FUN_14145dff0(lVar6,local_res8);
        if ((iVar5 < 2) ||
           ((cVar3 = FUN_141462040(lVar6,local_res8), cVar3 != '\0' &&
            (cVar3 = FUN_141461fc0(lVar6,*(undefined4 *)(param_1 + 0x1584),local_res8),
            cVar3 != '\0')))) goto LAB_141675a94;
        goto LAB_141675a53;
      }
      cVar3 = FUN_141461fc0(lVar6,*(undefined4 *)(param_1 + 0x1584),local_res8);
      if (cVar3 == '\0') goto LAB_141675a53;
LAB_141675a94:
      cVar3 = FUN_14145c4b0(lVar6,*(undefined4 *)(param_1 + 0x1584),&local_res8);
      if (cVar3 == '\x01') {
        cVar3 = FUN_14165eda0(param_1,local_res8);
        uVar7 = 5;
        cVar14 = (cVar3 == '\0') + '\t';
      }
      else {
        if (cVar3 != '\x02') goto LAB_141675a53;
        cVar14 = '\b';
        FUN_14166bc40(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),1);
        uVar7 = 5;
      }
    }
LAB_141675b1a:
    FUN_141a83cf0(*(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x3c0),
                  uVar7);
    cVar3 = FUN_14166d180(*(undefined8 *)(lVar1 + *(longlong *)(param_1 + 0x438)));
    if (cVar3 != '\0') {
      cVar14 = '\x01';
    }
    FUN_14166c2a0(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),cVar14);
    goto LAB_141675b5f;
  }
  if (cVar3 == '\x01') {
    cVar3 = (cVar2 == '\0') + '\x03';
    uVar7 = *(undefined8 *)(lVar1 + *(longlong *)(param_1 + 0x438));
LAB_14167596b:
    FUN_14166c2a0(uVar7,cVar3);
  }
  else if (cVar3 == '\x02') {
    FUN_14166bc40(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),1);
    cVar3 = '\x02';
    uVar7 = *(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1);
    goto LAB_14167596b;
  }
  lVar6 = FUN_141459960(uVar9,local_res18);
  if (lVar6 == 0) {
    puVar10 = (undefined8 *)FUN_141eca8b0(&local_res18,local_48);
    if (*(int *)(puVar10 + 1) == 0) {
      puVar16 = &DAT_14391f7d0;
    }
    else {
      puVar16 = (undefined2 *)*puVar10;
    }
    FUN_141dbbf60(local_58,L"FSkillDataTable : Not Found Skill - %s",puVar16);
    FUN_1416e6620(2,local_58);
    if (local_48[0] != 0) {
      FUN_141de9200();
    }
  }
  else {
    FUN_141a83cf0(*(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x3c0),
                  *(byte *)(lVar6 + 0x20) - 1);
  }
LAB_141675b5f:
  lVar6 = FUN_14145e360(uVar9,local_res18);
  uVar13 = 0;
  iVar5 = 0;
  if (*(longlong *)(lVar6 + 0x30) != 0) {
    iVar15 = iVar5;
    if (0 < *(int *)(param_1 + 0x1540)) {
      plVar11 = *(longlong **)(param_1 + 0x1538);
      uVar12 = uVar13;
      uVar17 = uVar13;
      do {
        iVar15 = (int)uVar17;
        if (*plVar11 == *(longlong *)(lVar6 + 0x30)) break;
        uVar17 = (ulonglong)((int)uVar17 + 1);
        uVar12 = uVar12 + 1;
        plVar11 = plVar11 + 1;
        iVar15 = iVar5;
      } while ((longlong)uVar12 < (longlong)*(int *)(param_1 + 0x1540));
    }
    *(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + (longlong)iVar15 * 8) + 0x408) =
         *(undefined8 *)(lVar1 + *(longlong *)(param_1 + 0x438));
  }
  if (*(longlong *)(lVar6 + 0x38) != 0) {
    iVar15 = iVar5;
    if (0 < *(int *)(param_1 + 0x1540)) {
      plVar11 = *(longlong **)(param_1 + 0x1538);
      uVar12 = uVar13;
      uVar17 = uVar13;
      do {
        iVar15 = (int)uVar17;
        if (*plVar11 == *(longlong *)(lVar6 + 0x38)) break;
        uVar17 = (ulonglong)((int)uVar17 + 1);
        uVar12 = uVar12 + 1;
        plVar11 = plVar11 + 1;
        iVar15 = iVar5;
      } while ((longlong)uVar12 < (longlong)*(int *)(param_1 + 0x1540));
    }
    *(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + (longlong)iVar15 * 8) + 0x408) =
         *(undefined8 *)(lVar1 + *(longlong *)(param_1 + 0x438));
  }
  if (*(longlong *)(lVar6 + 0x40) != 0) {
    iVar15 = iVar5;
    if (0 < *(int *)(param_1 + 0x1540)) {
      plVar11 = *(longlong **)(param_1 + 0x1538);
      uVar12 = uVar13;
      do {
        iVar15 = (int)uVar12;
        if (*plVar11 == *(longlong *)(lVar6 + 0x40)) break;
        uVar12 = (ulonglong)((int)uVar12 + 1);
        uVar13 = uVar13 + 1;
        plVar11 = plVar11 + 1;
        iVar15 = iVar5;
      } while ((longlong)uVar13 < (longlong)*(int *)(param_1 + 0x1540));
    }
    *(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + (longlong)iVar15 * 8) + 0x408) =
         *(undefined8 *)(lVar1 + *(longlong *)(param_1 + 0x438));
  }
  *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x410) = param_1;
  return;
}


