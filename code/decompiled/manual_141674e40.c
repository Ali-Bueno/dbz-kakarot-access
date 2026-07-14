// FUN_141674e40 @ 141674e40

void FUN_141674e40(longlong param_1,undefined8 *param_2)

{
  longlong lVar1;
  undefined8 *puVar2;
  char cVar3;
  char cVar4;
  longlong lVar5;
  longlong lVar6;
  undefined8 uVar7;
  longlong lVar8;
  longlong *plVar9;
  undefined4 uVar10;
  ulonglong uVar11;
  int iVar12;
  void *local_c8;
  undefined8 local_c0;
  longlong local_b8 [2];
  undefined1 local_a8 [8];
  longlong local_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined8 local_88;
  longlong local_80;
  undefined8 local_78;
  longlong local_70;
  undefined8 local_68;
  undefined4 local_60;
  undefined4 local_5c;
  longlong local_58;
  undefined8 local_50;
  longlong local_48 [2];
  
  lVar5 = FUN_140d2aef0();
  if (lVar5 == 0) {
    return;
  }
  lVar1 = *(longlong *)(lVar5 + 0x130);
  if (lVar1 == 0) {
    return;
  }
  lVar6 = FUN_14145e360(lVar1,*param_2);
  cVar3 = FUN_141461e60(lVar1,*param_2);
  uVar7 = FUN_14145e010(lVar1,&local_c8,*param_2);
  FUN_1416fb690(*(undefined8 *)(param_1 + 0x480),uVar7,0);
  if (local_c8 != (void *)0x0) {
    FUN_141de9200();
  }
  uVar10 = 1;
  if (cVar3 == '\0') {
    lVar8 = FUN_14145cda0();
    if ((lVar8 != 0) && (*(char *)(lVar8 + 0x2c) == '\x0f')) {
      local_a8[0] = 2;
      local_c8 = (void *)0x0;
      local_5c = 0;
      local_c0 = 0;
      local_a0 = 0;
      local_98 = 0;
      local_90 = 0;
      local_88 = 0;
      local_80 = 0;
      local_78 = 0;
      local_70 = 0;
      local_68 = 0;
      local_60 = 0;
      local_58 = 0;
      local_50 = 0;
      FUN_140fba270(&local_c8,local_a8,1);
      if (local_58 != 0) {
        FUN_141de9200();
      }
      if (local_70 != 0) {
        FUN_141de9200();
      }
      if (local_80 != 0) {
        FUN_141de9200();
      }
      FUN_140ca06c0(&local_90);
      if (local_a0 != 0) {
        FUN_141de9200();
      }
      FUN_141a82f90(local_c8,*(undefined4 *)(lVar6 + 0x28));
      uVar7 = FUN_14145e010(lVar1,local_b8,*param_2);
      uVar7 = FUN_141215960(local_48,uVar7,&local_c8,0);
      FUN_1416fb690(*(undefined8 *)(param_1 + 0x480),uVar7,0);
      if (local_48[0] != 0) {
        FUN_141de9200();
      }
      if (local_b8[0] != 0) {
        FUN_141de9200();
      }
      FUN_140ca06c0(&local_c8);
    }
    lVar8 = FUN_14145cda0(lVar1,*param_2);
    if (lVar8 == 0) goto LAB_1416750ee;
    FUN_1416fb040(*(undefined8 *)(param_1 + 0x460),*(undefined1 *)(lVar8 + 0x2c),
                  *(undefined4 *)(lVar6 + 0x88));
    if ((longlong *)(param_1 + 0x4c0) == (longlong *)(lVar8 + 8)) goto LAB_1416750ee;
    iVar12 = *(int *)(lVar8 + 0x10);
    uVar11 = (ulonglong)iVar12;
    local_c8 = *(void **)(lVar8 + 8);
    *(int *)(param_1 + 0x4c8) = iVar12;
    if ((iVar12 == 0) && (*(int *)(param_1 + 0x4cc) == 0)) {
      *(undefined4 *)(param_1 + 0x4cc) = 0;
      goto LAB_1416750ee;
    }
  }
  else {
    lVar8 = FUN_141459960(lVar1,*param_2);
    if (lVar8 == 0) goto LAB_1416750ee;
    FUN_1416f5900(*(undefined8 *)(param_1 + 0x460),*(undefined1 *)(lVar8 + 0x20),
                  *(undefined4 *)(lVar6 + 0x88));
    if ((longlong *)(param_1 + 0x4c0) == (longlong *)(lVar8 + 0x28)) goto LAB_1416750ee;
    iVar12 = *(int *)(lVar8 + 0x30);
    uVar11 = (ulonglong)iVar12;
    local_c8 = *(void **)(lVar8 + 0x28);
    *(int *)(param_1 + 0x4c8) = iVar12;
    if ((iVar12 == 0) && (*(int *)(param_1 + 0x4cc) == 0)) {
      *(undefined4 *)(param_1 + 0x4cc) = 0;
      goto LAB_1416750ee;
    }
  }
  FUN_1407f1420((undefined8 *)(param_1 + 0x4c0),uVar11 & 0xffffffff);
  memcpy(*(void **)(param_1 + 0x4c0),local_c8,uVar11 * 2);
LAB_1416750ee:
  cVar4 = FUN_1429aef50(*(undefined8 *)(param_1 + 1000));
  if ((cVar4 == '\x03') ||
     ((cVar4 = FUN_140d2f9e0(), cVar4 != '\0' &&
      (cVar4 = FUN_140d2f3c0(*(undefined4 *)(param_1 + 0x4b0)), cVar4 == '\0')))) {
    (**(code **)(**(longlong **)(param_1 + 0x478) + 0x250))(*(longlong **)(param_1 + 0x478),1);
    plVar9 = *(longlong **)(param_1 + 0x470);
  }
  else {
    uVar10 = 1;
    if (cVar3 != '\0') {
      uVar10 = 3;
    }
    (**(code **)(**(longlong **)(param_1 + 0x470) + 0x250))(*(longlong **)(param_1 + 0x470),uVar10);
    if (cVar3 != '\0') {
      lVar8 = FUN_141459960(lVar1,*param_2);
      if (lVar8 == 0) {
        iVar12 = 0;
      }
      else {
        iVar12 = (int)*(float *)(lVar8 + 0x1c);
      }
      cVar3 = FUN_140c51ef0(lVar6 + 0x20,L"C003A_S_SKL015");
      if (cVar3 != '\0') {
        iVar12 = FUN_14145d560(lVar5,*(undefined4 *)(param_1 + 0x4b0),0x1e);
      }
      local_c8 = (void *)0x0;
      local_c0 = 0;
      FUN_141db3e50(&local_c8,iVar12);
      FUN_1416fb690(*(undefined8 *)(param_1 + 0x478),&local_c8,0);
      if (local_c8 != (void *)0x0) {
        FUN_141de9200();
      }
    }
    plVar9 = *(longlong **)(param_1 + 0x478);
  }
  (**(code **)(*plVar9 + 0x250))(plVar9,uVar10);
  uVar7 = FUN_14145d7d0(lVar1,local_b8,*param_2);
  FUN_1416fb690(*(undefined8 *)(param_1 + 0x488),uVar7,0);
  if (local_b8[0] != 0) {
    FUN_141de9200();
  }
  local_c8 = (void *)0x0;
  local_c0 = 0;
  FUN_141db3e50(&local_c8,*(undefined4 *)(lVar6 + 0x28));
  FUN_1416fb690(*(undefined8 *)(param_1 + 0x468),&local_c8,0);
  if (local_c8 != (void *)0x0) {
    FUN_141de9200();
  }
  puVar2 = *(undefined8 **)(param_1 + 0x450);
  *(undefined4 *)(param_1 + 0x490) = *(undefined4 *)(lVar6 + 0x70);
  *(undefined4 *)(param_1 + 0x494) = *(undefined4 *)(lVar6 + 0x74);
  *(undefined4 *)(param_1 + 0x498) = *(undefined4 *)(lVar6 + 0x78);
  *(undefined4 *)(param_1 + 0x49c) = *(undefined4 *)(lVar6 + 0x7c);
  *(undefined4 *)(param_1 + 0x4a0) = *(undefined4 *)(lVar6 + 0x80);
  *(undefined4 *)(param_1 + 0x4a4) = *(undefined4 *)(lVar6 + 0x84);
  cVar3 = FUN_1410023d0(lVar6,1);
  FUN_14169a760(*puVar2,1,*(undefined4 *)(lVar6 + 0x70),cVar3 == '\0');
  lVar5 = *(longlong *)(param_1 + 0x450);
  cVar3 = FUN_1410023d0(lVar6,0);
  FUN_14169a760(*(undefined8 *)(lVar5 + 8),0,*(undefined4 *)(lVar6 + 0x74),cVar3 == '\0');
  lVar5 = *(longlong *)(param_1 + 0x450);
  cVar3 = FUN_1410023d0(lVar6,2);
  FUN_14169a760(*(undefined8 *)(lVar5 + 0x10),2,*(undefined4 *)(lVar6 + 0x78),cVar3 == '\0');
  lVar5 = *(longlong *)(param_1 + 0x450);
  cVar3 = FUN_1410023d0(lVar6,3);
  FUN_14169a760(*(undefined8 *)(lVar5 + 0x18),3,*(undefined4 *)(lVar6 + 0x7c),cVar3 == '\0');
  lVar5 = *(longlong *)(param_1 + 0x450);
  cVar3 = FUN_1410023d0(lVar6,4);
  FUN_14169a760(*(undefined8 *)(lVar5 + 0x20),4,*(undefined4 *)(lVar6 + 0x80),cVar3 == '\0');
  lVar5 = *(longlong *)(param_1 + 0x450);
  cVar3 = FUN_1410023d0(lVar6,5);
  FUN_14169a760(*(undefined8 *)(lVar5 + 0x28),5,*(undefined4 *)(lVar6 + 0x84),cVar3 == '\0');
  FUN_141675fc0(param_1);
  return;
}


