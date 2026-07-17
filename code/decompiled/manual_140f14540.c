// FUN_140f14540 @ 140f14540

void FUN_140f14540(longlong param_1,longlong param_2)

{
  undefined8 uVar1;
  bool bVar2;
  int iVar3;
  longlong lVar4;
  longlong lVar5;
  undefined8 *puVar6;
  longlong lVar7;
  undefined1 local_res10 [8];
  undefined8 local_98;
  undefined4 local_90;
  undefined8 local_88;
  undefined4 local_80;
  undefined8 local_7c;
  undefined4 local_74;
  undefined8 local_68;
  undefined4 local_60;
  undefined8 local_58;
  undefined4 local_50;
  undefined8 local_48;
  undefined4 local_40;
  undefined1 local_38 [12];
  undefined1 local_2c [20];
  
  if (param_1 != param_2) {
    return;
  }
  lVar4 = FUN_1411fc890();
  lVar4 = *(longlong *)(lVar4 + 0x1ef0);
  lVar7 = lVar4 + 0x530;
  lVar5 = FUN_142058d50(lVar7);
  if (lVar5 == 0) {
    LOCK();
    iVar3 = 0;
    if (DAT_1456e073c != 0) {
      iVar3 = DAT_1456e073c;
    }
    DAT_1456e073c = iVar3;
    UNLOCK();
    if (DAT_1456e073c == *(int *)(lVar4 + 0x538)) {
      return;
    }
    if (*(longlong *)(lVar4 + 0x540) == 0) {
      return;
    }
    lVar5 = FUN_14204ac70();
    FUN_1420575e0(lVar7,lVar5);
    if ((lVar5 != 0) || (DAT_145b9d472 == '\0')) {
      LOCK();
      if (DAT_1456e073c == 0) {
        DAT_1456e073c = 0;
      }
      UNLOCK();
      *(int *)(lVar4 + 0x538) = DAT_1456e073c;
    }
    lVar5 = FUN_142058d50(lVar7);
    if (lVar5 == 0) {
      return;
    }
  }
  lVar4 = FUN_14322dc90();
  if ((*(int *)(*(longlong *)(lVar5 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
     (*(longlong *)
       (*(longlong *)(*(longlong *)(lVar5 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
      lVar4 + 0x88)) {
    bVar2 = false;
  }
  else {
    bVar2 = true;
  }
  if (bVar2) {
    if (*(longlong *)(param_1 + 0x168) == 0) {
      local_80 = DAT_1456d9d30;
      puVar6 = &local_88;
      local_88 = DAT_1456d9d28;
    }
    else {
      puVar6 = (undefined8 *)FUN_142c6fc50(*(longlong *)(param_1 + 0x168),local_38);
    }
    local_7c = *puVar6;
    local_74 = *(undefined4 *)(puVar6 + 1);
    FUN_141e4e750(&local_7c,&local_98);
    uVar1 = *(undefined8 *)(param_1 + 0x398);
    local_68 = CONCAT44(DAT_1450d6c88,DAT_1450d6c88);
    local_98 = CONCAT44((int)((ulonglong)local_98 >> 0x20),(float)local_98 + DAT_143aa06dc);
    local_74 = 0x3f800000;
    local_60 = 0x3f800000;
    local_50 = local_90;
    local_58 = local_98;
    puVar6 = (undefined8 *)FUN_141eb3490(local_res10,L"spine_02",1);
    puVar6 = (undefined8 *)FUN_142c6bec0(uVar1,local_2c,*puVar6,0);
    local_48 = *puVar6;
    local_40 = *(undefined4 *)(puVar6 + 1);
    FUN_142d2f9c0(param_1,lVar5,&local_48,&local_58,&local_68,1,0);
  }
  return;
}


