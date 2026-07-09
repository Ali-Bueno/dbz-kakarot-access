// FUN_1415c0890 @ 1415c0890

void FUN_1415c0890(longlong param_1)

{
  undefined4 uVar1;
  int iVar2;
  void *_Src;
  bool bVar3;
  char cVar4;
  wint_t wVar5;
  wint_t wVar6;
  longlong lVar7;
  longlong lVar8;
  undefined8 uVar9;
  undefined1 *puVar10;
  longlong *plVar11;
  wint_t *pwVar12;
  undefined2 *puVar13;
  undefined2 *puVar14;
  undefined1 local_res8 [8];
  undefined4 uVar15;
  undefined2 uVar16;
  void *local_d8;
  undefined8 local_d0;
  longlong local_c8;
  longlong local_c0;
  longlong local_b8 [2];
  undefined **local_a8;
  longlong local_a0;
  undefined8 local_98;
  longlong local_90 [13];
  
  if ((*(int *)(param_1 + 0x508) != 6) && (*(int *)(param_1 + 0x508) != 0xc)) {
    return;
  }
  if (*(char *)(param_1 + 0x51d) == '\0') {
    return;
  }
  if (*(int *)(param_1 + 0x514) == -1) {
    FUN_141485f00(3,DAT_1450d6c88,0,0);
    return;
  }
  *(undefined1 *)(param_1 + 0x51d) = 0;
  uVar1 = DAT_1450d6c88;
  FUN_141485f00(1,DAT_1450d6c88,0,0);
  uVar15 = 0;
  plVar11 = *(longlong **)
             ((longlong)*(int *)(param_1 + 0x514) * 0x20 + 8 + *(longlong *)(param_1 + 0x4d0));
  (**(code **)(*plVar11 + 0x2e0))(plVar11,*(undefined8 *)(plVar11[0x78] + 0x10),0,1,0,uVar1);
  *(undefined4 *)(plVar11 + 0x7e) = 2;
  uVar1 = *(undefined4 *)
           ((longlong)*(int *)(param_1 + 0x514) * 0x20 + 0x1c + *(longlong *)(param_1 + 0x4d0));
  *(undefined8 *)(param_1 + 0x508) = 0xe;
  lVar7 = FUN_1411fd700();
  if (lVar7 != 0) {
    FUN_141593540(lVar7,0);
    FUN_14159fd00(lVar7,0);
  }
  lVar7 = FUN_1411fd780();
  if (lVar7 == 0) {
    return;
  }
  cVar4 = FUN_1416ed9d0(lVar7);
  if (cVar4 == '\0') {
    return;
  }
  local_b8[0] = 0;
  local_b8[1] = 0;
  local_a8 = (undefined **)CONCAT71(local_a8._1_7_,2);
  local_a0 = 0;
  local_98 = 0;
  local_90[0] = 0;
  local_90[1] = 0;
  local_90[2] = 0;
  local_90[3] = 0;
  local_90[4] = 0;
  local_90[5] = 0;
  local_90[6] = 0;
  local_90[7] = 0;
  local_90[8] = 0;
  FUN_140fba270(local_b8,&local_a8,2);
  if (local_90[7] != 0) {
    FUN_141de9200();
  }
  if (local_90[4] != 0) {
    FUN_141de9200();
  }
  if (local_90[2] != 0) {
    FUN_141de9200();
  }
  FUN_140ca06c0(local_90);
  if (local_a0 != 0) {
    FUN_141de9200();
  }
  lVar8 = *(longlong *)
           ((longlong)*(int *)(param_1 + 0x514) * 0x20 + 8 + *(longlong *)(param_1 + 0x4d0));
  local_d8 = (void *)0x0;
  iVar2 = *(int *)(lVar8 + 0x400);
  _Src = *(void **)(lVar8 + 0x3f8);
  local_d0 = CONCAT44(local_d0._4_4_,iVar2);
  if (iVar2 == 0) {
    local_d0 = 0;
  }
  else {
    FUN_1407f1420(&local_d8,iVar2,0);
    memcpy(local_d8,_Src,(longlong)iVar2 * 2);
  }
  FUN_141a87860(local_b8[0],&local_d8);
  if (local_d8 != (void *)0x0) {
    FUN_141de9200();
  }
  bVar3 = false;
  cVar4 = FUN_140d2f9d0();
  if ((cVar4 != '\0') && (lVar8 = FUN_140d2a6d0(), lVar8 != 0)) {
    uVar9 = FUN_1412dd890(lVar8,local_res8);
    cVar4 = FUN_140c51ef0(uVar9,L"DLC_06_03003");
    if (cVar4 != '\0') {
      bVar3 = true;
    }
  }
  uVar16 = (undefined2)((uint)uVar15 >> 0x10);
  local_c8 = 0;
  local_c0 = 0;
  if (bVar3) {
    puVar14 = &DAT_14391f7d0;
    if (*(int *)(local_b8[0] + 0x40) == 0) {
      puVar13 = &DAT_14391f7d0;
    }
    else {
      puVar13 = *(undefined2 **)(local_b8[0] + 0x38);
    }
    pwVar12 = &DAT_143eb9e18;
    do {
      uVar16 = (undefined2)((uint)uVar15 >> 0x10);
      wVar5 = *(wint_t *)((longlong)(puVar13 + -0xa1f5cf0c) + (longlong)pwVar12);
      if ((wVar5 == 0) && (*pwVar12 == 0)) goto LAB_1415c0bfe;
      if (wVar5 != *pwVar12) {
        wVar5 = towlower(wVar5);
        wVar6 = towlower(*pwVar12);
        uVar16 = (undefined2)((uint)uVar15 >> 0x10);
        if (wVar5 != wVar6) goto LAB_1415c0b99;
      }
      pwVar12 = pwVar12 + 1;
    } while( true );
  }
LAB_1415c0c89:
  lVar8 = local_b8[0];
  puVar10 = (undefined1 *)FUN_1415bd150(param_1,uVar1);
  uVar9 = FUN_1415bcf40(&local_d8,*puVar10,0);
  FUN_141a83c70(lVar8 + 0x60,uVar9);
  if (local_d8 != (void *)0x0) {
    FUN_141de9200();
  }
  local_d8 = (void *)0x0;
  local_d0 = 0xc;
  FUN_1407e2ee0(&local_d8,0);
  FUN_1407e4a20(local_d8,0xc,"WM_MoveConf",0xc,CONCAT22(uVar16,0x3f));
  lVar8 = FUN_1411fd400(&local_a8,&local_d8);
  plVar11 = (longlong *)FUN_141215960(local_90 + 9,lVar8 + 0x38,local_b8,0);
  if (&local_c8 != plVar11) {
    if (local_c8 != 0) {
      FUN_141de9200();
    }
    local_c8 = *plVar11;
    *plVar11 = 0;
    local_c0 = plVar11[1];
    plVar11[1] = 0;
  }
  if (local_90[9] != 0) {
    FUN_141de9200();
  }
LAB_1415c0d62:
  if (local_90[4] != 0) {
    FUN_141de9200();
  }
  if (local_90[2] != 0) {
    FUN_141de9200();
  }
  if (local_90[0] != 0) {
    FUN_141de9200();
  }
  if (local_a0 != 0) {
    FUN_141de9200();
  }
  local_a8 = &PTR_FUN_14397c0d8;
  if (local_d8 != (void *)0x0) {
    FUN_141de9200();
  }
  FUN_1416fd040(lVar7,&local_c8,0);
  if (local_c8 != 0) {
    FUN_141de9200();
  }
  FUN_140ca06c0(local_b8);
  return;
LAB_1415c0b99:
  if (wVar5 != wVar6) {
    if (*(int *)(local_b8[0] + 0x40) != 0) {
      puVar14 = *(undefined2 **)(local_b8[0] + 0x38);
    }
    pwVar12 = &DAT_143eb9e40;
    do {
      uVar16 = (undefined2)((uint)uVar15 >> 0x10);
      wVar5 = *(wint_t *)((longlong)(puVar14 + -0xa1f5cf20) + (longlong)pwVar12);
      if ((wVar5 == 0) && (*pwVar12 == 0)) break;
      if (wVar5 != *pwVar12) {
        wVar5 = towlower(wVar5);
        wVar6 = towlower(*pwVar12);
        uVar16 = (undefined2)((uint)uVar15 >> 0x10);
        if (wVar5 != wVar6) goto LAB_1415c0bf0;
      }
      pwVar12 = pwVar12 + 1;
    } while( true );
  }
LAB_1415c0bfe:
  local_d0 = 0x1e;
  local_d8 = (void *)0x0;
  FUN_1407e2ee0(&local_d8,0);
  FUN_1407e4a20(local_d8,0x1e,"DLC6_03002_Area04_DuelConfirm",0x1e,CONCAT22(uVar16,0x3f));
  lVar8 = FUN_1411fd400(&local_a8,&local_d8);
  plVar11 = (longlong *)(lVar8 + 0x38);
  if (&local_c8 != plVar11) {
    if (local_c8 != 0) {
      FUN_141de9200();
    }
    local_c8 = *plVar11;
    *plVar11 = 0;
    local_c0 = *(longlong *)(lVar8 + 0x40);
    *(undefined8 *)(lVar8 + 0x40) = 0;
  }
  goto LAB_1415c0d62;
LAB_1415c0bf0:
  if (wVar5 == wVar6) goto LAB_1415c0bfe;
  goto LAB_1415c0c89;
}


