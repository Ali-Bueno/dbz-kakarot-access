// FUN_1414d79e0 @ 1414d79e0

void FUN_1414d79e0(longlong param_1)

{
  longlong *plVar1;
  ushort uVar2;
  int iVar3;
  wint_t wVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  longlong lVar8;
  undefined8 uVar9;
  ulonglong uVar10;
  ushort *puVar11;
  undefined8 *puVar12;
  undefined8 *puVar13;
  char *pcVar14;
  undefined8 local_res8;
  undefined8 *local_98;
  undefined8 local_90;
  ushort *local_88;
  int local_80;
  undefined **local_78;
  longlong local_70;
  longlong local_60;
  ushort *local_50;
  int local_48;
  longlong local_40;
  
  iVar5 = *(int *)(param_1 + 0x63c);
  if (iVar5 < 1) {
    if (iVar5 + 6U < 2) {
      *(undefined4 *)(param_1 + 0x63c) = 0xfffffffb;
      local_98 = (undefined8 *)0x0;
      local_90 = 1;
      FUN_141eb3410(&local_res8,"Cpl021A",1);
      FUN_1407f1510(&local_98,0);
      puVar13 = local_98;
      *local_98 = local_res8;
      FUN_141eb3410(&local_res8,"Cpl022A",1);
      lVar8 = (longlong)(int)local_90;
      iVar5 = (int)local_90 + 1;
      if (local_90._4_4_ < iVar5) {
        uVar10 = local_90 & 0xffffffff;
        local_90._0_4_ = iVar5;
        FUN_1407f1510(&local_98,uVar10);
        puVar13 = local_98;
        iVar5 = (int)local_90;
      }
      local_90._0_4_ = iVar5;
      iVar7 = local_90._4_4_;
      iVar5 = (int)local_90;
      puVar13[lVar8] = local_res8;
      FUN_141eb3410(&local_res8,"Cpl023A",1);
      local_90._0_4_ = iVar5 + 1;
      if (iVar7 < (int)local_90) {
        FUN_1407f1510(&local_98,iVar5);
        puVar13 = local_98;
        iVar7 = local_90._4_4_;
      }
      iVar6 = (int)local_90;
      puVar13[iVar5] = local_res8;
      FUN_141eb3410(&local_res8,"Cpl024A",1);
      local_90._0_4_ = iVar6 + 1;
      if (iVar7 < (int)local_90) {
        FUN_1407f1510(&local_98,iVar6);
        puVar13 = local_98;
        iVar7 = local_90._4_4_;
      }
      iVar3 = (int)local_90;
      puVar13[iVar6] = local_res8;
      FUN_141eb3410(&local_res8,"Cpl025A",1);
      iVar5 = iVar3 + 1;
      local_90 = CONCAT44(local_90._4_4_,iVar5);
      if (iVar7 < iVar5) {
        FUN_1407f1510(&local_98,iVar3);
        puVar13 = local_98;
        iVar7 = local_90._4_4_;
        iVar5 = (int)local_90;
      }
      puVar13[iVar3] = local_res8;
      if (*(int *)(param_1 + 0x63c) == -6) {
        FUN_141eb3410(&local_res8,"Cpl071A",1);
        lVar8 = (longlong)iVar5;
        iVar6 = iVar5 + 1;
        local_90 = CONCAT44(local_90._4_4_,iVar6);
        if (iVar7 < iVar6) {
          FUN_1407f1510(&local_98,iVar5);
          puVar13 = local_98;
          iVar6 = (int)local_90;
        }
        iVar5 = iVar6;
        puVar13[lVar8] = local_res8;
      }
      for (puVar12 = puVar13; puVar12 != puVar13 + iVar5; puVar12 = puVar12 + 1) {
        lVar8 = FUN_141994960(*(undefined8 *)(param_1 + 0x5f0),puVar12);
        if (lVar8 != 0) {
          local_res8 = *(undefined8 *)(lVar8 + 0x38);
          iVar7 = FUN_141993860(*(undefined8 *)(param_1 + 0x510),&local_res8);
          if (0 < iVar7) {
            FUN_141505510(*(undefined8 *)(param_1 + 0x4a0),iVar7);
          }
        }
      }
      if (puVar13 != (undefined8 *)0x0) {
        FUN_141de9200(puVar13);
      }
    }
  }
  else {
    FUN_141505510(*(undefined8 *)(param_1 + 0x4a0),iVar5);
  }
  if (*(longlong *)(param_1 + 0x518) == 0) {
    return;
  }
  local_res8 = 0;
  plVar1 = (longlong *)(param_1 + ((longlong)*(int *)(param_1 + 0x638) + 200) * 8);
  if (*plVar1 == 0) {
    if (0 < *(int *)(param_1 + 0x63c)) {
      FUN_141505510(*(undefined8 *)(param_1 + 0x4a0));
    }
    *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 8;
    *(undefined8 *)(param_1 + 0x638) = 0;
    *(undefined4 *)(param_1 + 0x500) = 7;
    *(undefined4 *)(param_1 + 0x508) = 0;
    FUN_141501a70(*(undefined8 *)(param_1 + 0x4a0),1);
    FUN_14170e590(*(undefined8 *)(param_1 + 0x518));
    (**(code **)(**(longlong **)(param_1 + 0x518) + 0x468))(*(longlong **)(param_1 + 0x518),0);
    lVar8 = FUN_140d282e0();
    if (lVar8 == 0) {
      return;
    }
    *(undefined1 *)(lVar8 + 0x5c4) = 0;
    FUN_1411d45e0(lVar8);
    *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) & 0xf7;
    return;
  }
  FUN_141eca8b0(plVar1,&local_88);
  FUN_1411fd400(&local_78,&local_88);
  pcVar14 = "CommunityLink_41_12";
  puVar11 = &DAT_14391f7d0;
  if (local_80 != 0) {
    puVar11 = local_88;
  }
  do {
    uVar2 = *puVar11;
    if ((uVar2 == 0) && (*pcVar14 == '\0')) goto LAB_1414d7e5f;
    if ((uint)uVar2 != (int)*pcVar14) {
      wVar4 = towlower(uVar2);
      iVar5 = tolower((int)*pcVar14);
      if ((uint)wVar4 != (int)(char)iVar5) break;
    }
    puVar11 = puVar11 + 1;
    pcVar14 = pcVar14 + 1;
  } while( true );
  if ((uint)wVar4 != (int)(char)iVar5) {
    pcVar14 = "CommunityLink_88_07";
    puVar11 = &DAT_14391f7d0;
    if (local_80 != 0) {
      puVar11 = local_88;
    }
    do {
      uVar2 = *puVar11;
      if ((uVar2 == 0) && (*pcVar14 == '\0')) break;
      if ((uint)uVar2 != (int)*pcVar14) {
        wVar4 = towlower(uVar2);
        iVar5 = tolower((int)*pcVar14);
        if ((uint)wVar4 != (int)(char)iVar5) goto LAB_1414d7dc0;
      }
      pcVar14 = pcVar14 + 1;
      puVar11 = puVar11 + 1;
    } while( true );
  }
LAB_1414d7e5f:
  *(undefined4 *)(param_1 + 0x63c) = 0xfffffffb;
  local_98 = (undefined8 *)0x0;
  local_90 = 1;
  FUN_141eb3410(&local_res8,"Cpl021A",1);
  FUN_1407f1510(&local_98,0);
  puVar13 = local_98;
  *local_98 = local_res8;
  FUN_141eb3410(&local_res8,"Cpl022A",1);
  lVar8 = (longlong)(int)local_90;
  iVar5 = (int)local_90 + 1;
  if (local_90._4_4_ < iVar5) {
    uVar10 = local_90 & 0xffffffff;
    local_90._0_4_ = iVar5;
    FUN_1407f1510(&local_98,uVar10);
    puVar13 = local_98;
    iVar5 = (int)local_90;
  }
  local_90._0_4_ = iVar5;
  iVar7 = local_90._4_4_;
  iVar5 = (int)local_90;
  puVar13[lVar8] = local_res8;
  FUN_141eb3410(&local_res8,"Cpl023A",1);
  local_90._0_4_ = iVar5 + 1;
  if (iVar7 < (int)local_90) {
    FUN_1407f1510(&local_98,iVar5);
    puVar13 = local_98;
    iVar7 = local_90._4_4_;
  }
  iVar6 = (int)local_90;
  puVar13[iVar5] = local_res8;
  FUN_141eb3410(&local_res8,"Cpl024A",1);
  local_90._0_4_ = iVar6 + 1;
  if (iVar7 < (int)local_90) {
    FUN_1407f1510(&local_98,iVar6);
    puVar13 = local_98;
    iVar7 = local_90._4_4_;
  }
  iVar3 = (int)local_90;
  puVar13[iVar6] = local_res8;
  FUN_141eb3410(&local_res8,"Cpl025A",1);
  iVar5 = iVar3 + 1;
  local_90 = CONCAT44(local_90._4_4_,iVar5);
  if (iVar7 < iVar5) {
    FUN_1407f1510(&local_98,iVar3);
    puVar13 = local_98;
    iVar7 = local_90._4_4_;
    iVar5 = (int)local_90;
  }
  pcVar14 = "CommunityLink_88_07";
  puVar13[iVar3] = local_res8;
  puVar11 = &DAT_14391f7d0;
  if (local_80 != 0) {
    puVar11 = local_88;
  }
  do {
    uVar2 = *puVar11;
    if ((uVar2 == 0) && (*pcVar14 == '\0')) goto LAB_1414d800f;
    if ((uint)uVar2 != (int)*pcVar14) {
      wVar4 = towlower(uVar2);
      iVar6 = tolower((int)*pcVar14);
      if ((uint)wVar4 != (int)(char)iVar6) goto LAB_1414d800b;
    }
    puVar11 = puVar11 + 1;
    pcVar14 = pcVar14 + 1;
  } while( true );
LAB_1414d7dc0:
  if ((uint)wVar4 == (int)(char)iVar5) goto LAB_1414d7e5f;
  puVar11 = &DAT_14391f7d0;
  if (local_48 != 0) {
    puVar11 = local_50;
  }
  uVar9 = FUN_141eb3490(&local_res8,puVar11,1);
  lVar8 = FUN_141994960(*(undefined8 *)(param_1 + 0x5f0),uVar9);
  if (lVar8 == 0) {
    puVar11 = &DAT_14391f7d0;
    if (local_48 != 0) {
      puVar11 = local_50;
    }
    FUN_141dbbf60(&local_98,L"Not Found NameId From SoulEmChar Table =%s",puVar11);
    FUN_1416e6620(2,&local_98);
    goto LAB_1414d80f3;
  }
  local_res8 = *(undefined8 *)(lVar8 + 0x38);
  iVar5 = FUN_141993860(*(undefined8 *)(param_1 + 0x510),&local_res8);
  if (0 < iVar5) {
    *(int *)(param_1 + 0x63c) = iVar5;
    FUN_1414fd2b0(*(undefined8 *)(param_1 + 0x4a0),iVar5);
  }
  goto LAB_1414d80c5;
LAB_1414d800b:
  if ((uint)wVar4 == (int)(char)iVar6) {
LAB_1414d800f:
    *(undefined4 *)(param_1 + 0x63c) = 0xfffffffa;
    FUN_141eb3410(&local_res8,"Cpl071A",1);
    lVar8 = (longlong)iVar5;
    iVar6 = iVar5 + 1;
    local_90 = CONCAT44(local_90._4_4_,iVar6);
    if (iVar7 < iVar6) {
      FUN_1407f1510(&local_98,iVar5);
      puVar13 = local_98;
      iVar6 = (int)local_90;
    }
    iVar5 = iVar6;
    puVar13[lVar8] = local_res8;
  }
  for (puVar12 = puVar13; puVar12 != puVar13 + iVar5; puVar12 = puVar12 + 1) {
    lVar8 = FUN_141994960(*(undefined8 *)(param_1 + 0x5f0),puVar12);
    if (lVar8 != 0) {
      local_res8 = *(undefined8 *)(lVar8 + 0x38);
      iVar7 = FUN_141993860(*(undefined8 *)(param_1 + 0x510),&local_res8);
      if (0 < iVar7) {
        FUN_1414fd2b0(*(undefined8 *)(param_1 + 0x4a0),iVar7);
      }
    }
  }
  if (puVar13 != (undefined8 *)0x0) {
    FUN_141de9200(puVar13);
  }
LAB_1414d80c5:
  if (*(int *)(param_1 + 0x638) < 1) {
    FUN_1417160f0(*(undefined8 *)(param_1 + 0x518),&local_88,0,1,0);
  }
  else {
    FUN_14170dc80(*(undefined8 *)(param_1 + 0x518),&local_88,1);
  }
LAB_1414d80f3:
  *(int *)(param_1 + 0x638) = *(int *)(param_1 + 0x638) + 1;
  if (local_40 != 0) {
    FUN_141de9200();
  }
  if (local_50 != (ushort *)0x0) {
    FUN_141de9200();
  }
  if (local_60 != 0) {
    FUN_141de9200();
  }
  if (local_70 != 0) {
    FUN_141de9200();
  }
  local_78 = &PTR_FUN_14397c0d8;
  if (local_88 != (ushort *)0x0) {
    FUN_141de9200();
  }
  return;
}


