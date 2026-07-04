// FUN_1414c9ea0 @ 1414c9ea0

undefined8 FUN_1414c9ea0(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  ulonglong *puVar3;
  int iVar4;
  void *pvVar5;
  void *pvVar6;
  void *local_88;
  void *local_80;
  ulonglong local_78 [2];
  undefined **local_68;
  longlong local_60;
  longlong local_50;
  longlong local_40;
  longlong local_30;
  
  if ((*(char *)(param_1 + 0x4f1) == '\0') || (1 < (byte)(*(char *)(param_1 + 0x4f3) - 6U))) {
    return 0;
  }
  lVar1 = FUN_1411fd780();
  if (lVar1 == 0) {
    return 1;
  }
  pvVar5 = (void *)0x0;
  pvVar6 = pvVar5;
  if (*(char *)(param_1 + 0x4f3) == '\x06') {
    local_88 = (void *)0x0;
    local_80 = (void *)0x24;
    FUN_1407e2ee0(&local_88,0);
    FUN_1407e4a20(local_88,0x24,"Comm_TutorialEvent_EmptyPanetChoose",0x24,0x3f);
    lVar2 = FUN_1411fd400(&local_68,&local_88);
    puVar3 = (ulonglong *)(lVar2 + 0x38);
    if (local_78 != puVar3) {
      pvVar6 = (void *)*puVar3;
      *puVar3 = 0;
      pvVar5 = (void *)(ulonglong)*(uint *)(lVar2 + 0x40);
      *(undefined8 *)(lVar2 + 0x40) = 0;
    }
    iVar4 = (int)pvVar5;
    if (local_30 != 0) {
      FUN_141de9200();
    }
    if (local_40 != 0) {
      FUN_141de9200();
    }
    if (local_50 != 0) {
      FUN_141de9200();
    }
    if (local_60 != 0) {
      FUN_141de9200();
    }
    local_68 = &PTR_FUN_14397c0d8;
    if (local_88 != (void *)0x0) {
      FUN_141de9200();
    }
    local_88 = (void *)0x0;
    local_80 = (void *)CONCAT44(local_80._4_4_,iVar4);
    if (iVar4 == 0) goto LAB_1414ca0ec;
    FUN_1407f1420(&local_88,pvVar5,0);
  }
  else {
    if (*(char *)(param_1 + 0x4f3) != '\a') {
      return 1;
    }
    local_88 = (void *)0x0;
    local_80 = (void *)0x1d;
    FUN_1407e2ee0(&local_88,0);
    FUN_1407e4a20(local_88,0x1d,"Comm_TutorialEvent_EmblemSet",0x1d,0x3f);
    lVar2 = FUN_1411fd400(&local_68,&local_88);
    puVar3 = (ulonglong *)(lVar2 + 0x38);
    if (local_78 != puVar3) {
      pvVar6 = (void *)*puVar3;
      *puVar3 = 0;
      pvVar5 = (void *)(ulonglong)*(uint *)(lVar2 + 0x40);
      *(undefined8 *)(lVar2 + 0x40) = 0;
    }
    iVar4 = (int)pvVar5;
    if (local_30 != 0) {
      FUN_141de9200();
    }
    if (local_40 != 0) {
      FUN_141de9200();
    }
    if (local_50 != 0) {
      FUN_141de9200();
    }
    if (local_60 != 0) {
      FUN_141de9200();
    }
    local_68 = &PTR_FUN_14397c0d8;
    if (local_88 != (void *)0x0) {
      FUN_141de9200();
    }
    local_88 = (void *)0x0;
    local_80 = (void *)CONCAT44(local_80._4_4_,iVar4);
    if (iVar4 == 0) goto LAB_1414ca0ec;
    FUN_1407f1420(&local_88,pvVar5,0);
  }
  memcpy(local_88,pvVar6,(longlong)iVar4 * 2);
  pvVar5 = local_80;
LAB_1414ca0ec:
  local_80 = pvVar5;
  FUN_1416fcc70(lVar1,&local_88,0,1);
  if (pvVar6 != (void *)0x0) {
    FUN_141de9200(pvVar6);
  }
  return 1;
}


