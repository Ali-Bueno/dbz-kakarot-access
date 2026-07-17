// FUN_1416f0030 page-commit callback + callees. Map@0x508 Subject@0x3a8 page@0x5a8/0x5ac count@0x5b0.
// ======== SEED  @ 0x1416f0030 ========

void FUN_1416f0030(longlong param_1)

{
  int iVar1;
  undefined8 uVar2;
  longlong lVar3;
  ulonglong uVar4;
  
  FUN_141700200(param_1,*(undefined4 *)(param_1 + 0x5ac),0);
  iVar1 = *(int *)(param_1 + 0x5a8);
  FUN_1416f59d0(param_1,CONCAT31((int3)((uint)iVar1 >> 8),0 < iVar1),
                iVar1 < *(int *)(param_1 + 0x5b0) + -1);
  iVar1 = *(int *)(param_1 + 0x5a8);
  uVar4 = (ulonglong)(0 < iVar1 | 2);
  if (*(int *)(param_1 + 0x5b0) <= iVar1 + 1) {
    uVar4 = (ulonglong)(0 < iVar1);
  }
  FUN_141700970(param_1,*(undefined4 *)(&DAT_143f19c48 + uVar4 * 4));
  FUN_1416f29f0(param_1);
  uVar2 = (**(code **)(**(longlong **)(param_1 + 0x20) + 0x148))();
  lVar3 = FUN_140d23070(uVar2,0);
  if (lVar3 != 0) {
    lVar3 = FUN_1412b5ca0(lVar3);
    if ((lVar3 != 0) && (*(longlong *)(lVar3 + 0x248) != 0)) {
      *(undefined1 *)(*(longlong *)(lVar3 + 0x248) + 0x402) = 1;
    }
  }
  return;
}



// ======== d1<-0x1416f0030  @ 0x141700200 ========

void FUN_141700200(longlong param_1,int param_2,char param_3)

{
  int iVar1;
  longlong local_res8 [4];
  
  iVar1 = *(int *)(param_1 + 0x5b0) + -1;
  if (param_2 < 0) {
    iVar1 = 0;
  }
  else if (param_2 < iVar1) {
    iVar1 = param_2;
  }
  if ((*(int *)(param_1 + 0x5a8) != iVar1) || (param_3 != '\0')) {
    *(int *)(param_1 + 0x5a8) = iVar1;
    *(longlong *)(param_1 + 0x4b0) =
         param_1 + ((longlong)(iVar1 % *(int *)(param_1 + 0x4a8)) * 5 + 0x86) * 8;
    local_res8[0] = param_1;
    FUN_1416daef0(local_res8,iVar1);
    FUN_1416e67a0(param_1,*(undefined4 *)(*(longlong *)(param_1 + 0x4b0) + 0x20));
    iVar1 = *(int *)(param_1 + 0x5a8);
    if (0 < iVar1) {
      FUN_1416daef0(local_res8,iVar1 + -1,**(undefined8 **)(param_1 + 0x4b0));
      iVar1 = *(int *)(param_1 + 0x5a8);
    }
    if (iVar1 + 1 < *(int *)(param_1 + 0x5b0)) {
      FUN_1416daef0(local_res8,iVar1 + 1,*(undefined8 *)(*(longlong *)(param_1 + 0x4b0) + 8));
    }
  }
  return;
}



// ======== d1<-0x1416f0030  @ 0x1416f59d0 ========

void FUN_1416f59d0(longlong param_1,char param_2,char param_3)

{
  longlong *plVar1;
  undefined4 uVar2;
  
  plVar1 = *(longlong **)(param_1 + 0x3c0);
  if (plVar1 != (longlong *)0x0) {
    uVar2 = 1;
    if (param_2 != '\0') {
      uVar2 = 4;
    }
    (**(code **)(*plVar1 + 0x250))(plVar1,uVar2);
  }
  plVar1 = *(longlong **)(param_1 + 0x3c8);
  if (plVar1 != (longlong *)0x0) {
    uVar2 = 1;
    if (param_3 != '\0') {
      uVar2 = 4;
    }
    (**(code **)(*plVar1 + 0x250))(plVar1,uVar2);
  }
  return;
}



// ======== d1<-0x1416f0030  @ 0x141700970 ========

void FUN_141700970(longlong param_1,int param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  wchar_t *pwVar3;
  longlong local_18 [2];
  
  if (*(int *)(param_1 + 0x5bc) == param_2) {
    return;
  }
  *(int *)(param_1 + 0x5bc) = param_2;
  lVar1 = FUN_1411fd700();
  if (lVar1 == 0) {
    return;
  }
  FUN_141593540(lVar1,0);
  switch(*(undefined4 *)(param_1 + 0x5bc)) {
  case 0:
    local_18[1] = 0x17;
    local_18[0] = 0;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_Help_ConsentScreen",0x2e);
    FUN_14159e0a0(lVar1,0x15,local_18,0);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    local_18[0] = 0;
    local_18[1] = 0x18;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_Help_languageChange",0x30);
    FUN_14159e0a0(lVar1,0x16,local_18,0);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    local_18[0] = 0;
    local_18[1] = 0xd;
    FUN_1407e2ee0(local_18,0);
    pwVar3 = L"MSG_NextPage";
    goto LAB_141700c0c;
  case 1:
    local_18[1] = 0x17;
    local_18[0] = 0;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_Help_ConsentScreen",0x2e);
    FUN_14159e0a0(lVar1,0x15,local_18,0);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    local_18[0] = 0;
    local_18[1] = 0x18;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_Help_languageChange",0x30);
    FUN_14159e0a0(lVar1,0x16,local_18,0);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    local_18[0] = 0;
    local_18[1] = 0xd;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_PrevPage",0x1a);
    uVar2 = 0x18;
    break;
  case 2:
    local_18[1] = 0x17;
    local_18[0] = 0;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_Help_ConsentScreen",0x2e);
    FUN_14159e0a0(lVar1,0x15,local_18,0);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    local_18[0] = 0;
    local_18[1] = 0x18;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_Help_languageChange",0x30);
    FUN_14159e0a0(lVar1,0x16,local_18,0);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    local_18[0] = 0;
    local_18[1] = 0xd;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_PrevPage",0x1a);
    FUN_14159e0a0(lVar1,0x18,local_18,0);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    local_18[0] = 0;
    local_18[1] = 0xd;
    FUN_1407e2ee0(local_18,0);
    pwVar3 = L"MSG_NextPage";
LAB_141700c0c:
    FUN_141dd1c10(local_18[0],pwVar3,0x1a);
    uVar2 = 0x17;
    break;
  case 3:
    local_18[1] = 0x17;
    local_18[0] = 0;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_Help_ConsentScreen",0x2e);
    FUN_14159e0a0(lVar1,0x15,local_18,0);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    local_18[0] = 0;
    local_18[1] = 0x18;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_Help_languageChange",0x30);
    uVar2 = 0x16;
    break;
  case 4:
  case 5:
    local_18[1] = 0xc;
    local_18[0] = 0;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_Confirm",0x18);
    FUN_14159e0a0(lVar1,0x17,local_18,0);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    local_18[0] = 0;
    local_18[1] = 0xb;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_Cancel",0x16);
    FUN_14159e0a0(lVar1,0x18,local_18,0);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    local_18[0] = 0;
    local_18[1] = 0xb;
    FUN_1407e2ee0(local_18,0);
    FUN_141dd1c10(local_18[0],L"MSG_Select",0x16);
    FUN_14159e0a0(lVar1,0x12,local_18,0);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
    FUN_14159fd00(lVar1,0);
    return;
  default:
    goto switchD_1417009d6_default;
  }
  FUN_14159e0a0(lVar1,uVar2,local_18,0);
  if (local_18[0] != 0) {
    FUN_141de9200();
  }
switchD_1417009d6_default:
  FUN_14159fd20(lVar1,0);
  return;
}



// ======== d1<-0x1416f0030  @ 0x1416f29f0 ========

void FUN_1416f29f0(longlong param_1)

{
  longlong *plVar1;
  undefined4 uVar2;
  
  uVar2 = DAT_1450d6c88;
  if (*(longlong *)(param_1 + 0x3c0) != 0) {
    FUN_1429e4dc0();
    plVar1 = *(longlong **)(param_1 + 0x3c0);
    FUN_1429e4dc0(plVar1);
    (**(code **)(*plVar1 + 0x2e0))(plVar1,plVar1[0x78],0,0,0,uVar2);
  }
  if (*(longlong *)(param_1 + 0x3c8) != 0) {
    FUN_1429e4dc0();
    plVar1 = *(longlong **)(param_1 + 0x3c8);
    FUN_1429e4dc0(plVar1);
    (**(code **)(*plVar1 + 0x2e0))(plVar1,plVar1[0x78],0,0,0,uVar2);
  }
  return;
}



// ======== d1<-0x1416f0030  @ 0x140d23070 ========

longlong FUN_140d23070(void)

{
  bool bVar1;
  longlong lVar2;
  longlong lVar3;
  
  if (DAT_14592f0f0 != 0) {
    if ((DAT_14592f0f0 != 1) && (lVar2 = FUN_142d1f150(), lVar2 != 0)) {
      FUN_141863300();
    }
    lVar2 = DAT_14592f0e8;
    if (DAT_14592f0e8 == 0) {
      return 0;
    }
    FUN_140d1b280(DAT_14592f0e8);
    return *(longlong *)(lVar2 + 0x640);
  }
  lVar2 = FUN_142d1f150();
  if (lVar2 != 0) {
    lVar3 = FUN_141863300();
    if ((*(int *)(*(longlong *)(lVar2 + 0x10) + 0x90) < *(int *)(lVar3 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar2 + 0x10) + 0x88) + (longlong)*(int *)(lVar3 + 0x90) * 8)
        != lVar3 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) {
      return lVar2;
    }
  }
  return 0;
}



// ======== d1<-0x1416f0030  @ 0x1412b5ca0 ========

longlong FUN_1412b5ca0(longlong param_1)

{
  longlong lVar1;
  bool bVar2;
  longlong lVar3;
  
  lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x870) + 0x90);
  if (lVar1 != 0) {
    lVar3 = FUN_14187c230();
    if ((*(int *)(*(longlong *)(lVar1 + 0x10) + 0x90) < *(int *)(lVar3 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar1 + 0x10) + 0x88) + (longlong)*(int *)(lVar3 + 0x90) * 8)
        != lVar3 + 0x88)) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    if (bVar2) {
      return lVar1;
    }
  }
  return 0;
}



// ======== d2<-0x141700200  @ 0x1416daef0 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1416daef0(longlong *param_1,undefined4 param_2,longlong param_3)

{
  int *_Src;
  wint_t wVar1;
  wint_t wVar2;
  int iVar3;
  undefined2 **ppuVar4;
  longlong lVar5;
  int iVar6;
  wchar_t *pwVar7;
  int iVar8;
  longlong lVar9;
  wint_t *pwVar10;
  undefined2 *puVar11;
  ulonglong uVar12;
  int local_res8 [2];
  longlong local_res18;
  undefined1 local_res20 [8];
  undefined2 *local_58;
  undefined8 local_50;
  int *local_48;
  longlong *local_40;
  longlong local_38 [2];
  
  local_50 = (undefined2 *)0x27;
  local_58 = (undefined2 *)0x0;
  local_res18 = param_3;
  FUN_1407e2ee0(&local_58,0);
  FUN_141dd1c10(local_58,L"/Game/Art/UI_L10N/LANG/Xcmn_Agreement/",0x4e);
  puVar11 = local_50;
  if (*(int *)(*param_1 + 0x5b4) - 1U < 3) {
    iVar8 = (int)local_50;
    lVar9 = (longlong)(int)local_50;
    iVar6 = (6 - (uint)((int)local_50 != 0)) + (int)local_50;
    local_50 = (undefined2 *)CONCAT44(local_50._4_4_,iVar6);
    if (local_50._4_4_ < iVar6) {
      FUN_1407e2ee0(&local_58,(ulonglong)puVar11 & 0xffffffff);
    }
    lVar9 = lVar9 - (ulonglong)(iVar8 != 0);
    *(undefined8 *)(local_58 + lVar9) = _DAT_143f19cb0;
    local_58[lVar9 + 4] = DAT_143f19cb8;
    local_58[lVar9 + 5] = 0;
  }
  puVar11 = local_58;
  lVar9 = (longlong)(int)local_50;
  local_48 = (int *)0x0;
  local_40 = (longlong *)CONCAT44(local_40._4_4_,(int)local_50);
  iVar8 = 0;
  if ((int)local_50 == 0) {
    local_40 = (longlong *)((ulonglong)local_50 & 0xffffffff);
  }
  else {
    FUN_1407f1420(&local_48,(ulonglong)local_50 & 0xffffffff,0);
    memcpy(local_48,puVar11,lVar9 * 2);
  }
  ppuVar4 = (undefined2 **)FUN_141a69100(local_38,&local_48,*(undefined4 *)(*param_1 + 0x5b8),1);
  if (&local_58 != ppuVar4) {
    if (local_58 != (undefined2 *)0x0) {
      FUN_141de9200();
    }
    local_58 = *ppuVar4;
    *ppuVar4 = (undefined2 *)0x0;
    local_50 = ppuVar4[1];
    ppuVar4[1] = (undefined2 *)0x0;
  }
  if (local_38[0] != 0) {
    FUN_141de9200();
  }
  if (*(int *)(*param_1 + 0x5b4) == 1) {
    pwVar7 = L"Privacypolicy_%02d";
  }
  else if (*(int *)(*param_1 + 0x5b4) - 2U < 2) {
    pwVar7 = L"Kpi_%02d";
  }
  else {
    pwVar7 = L"Eula_%02d";
  }
  FUN_141dbbf60(&local_48,pwVar7,param_2);
  _Src = local_48;
  puVar11 = local_50;
  if ((int)local_40 != 0) {
    iVar8 = (int)local_40 + -1;
  }
  if (iVar8 != 0) {
    iVar6 = (int)local_50;
    lVar9 = (longlong)(int)local_50;
    iVar3 = (uint)((int)local_50 == 0) + iVar8 + (int)local_50;
    local_50 = (undefined2 *)CONCAT44(local_50._4_4_,iVar3);
    if (local_50._4_4_ < iVar3) {
      FUN_1407e2ee0(&local_58,(ulonglong)puVar11 & 0xffffffff);
    }
    puVar11 = local_58 + (lVar9 - (ulonglong)(iVar6 != 0));
    memcpy(puVar11,_Src,(longlong)iVar8 * 2);
    puVar11[iVar8] = 0;
  }
  if (local_48 != (int *)0x0) {
    FUN_141de9200(local_48);
  }
  pwVar10 = &DAT_14391f7d0;
  puVar11 = &DAT_14391f7d0;
  if ((int)local_50 != 0) {
    puVar11 = local_58;
  }
  local_res8[0] = FUN_1407e51a0(puVar11);
  if (*(int *)(local_res18 + 0x18) == 0) {
    puVar11 = &DAT_14391f7d0;
  }
  else {
    puVar11 = *(undefined2 **)(local_res18 + 0x10);
  }
  do {
    wVar1 = *(wint_t *)((longlong)(puVar11 + -0xa1c8fbe8) + (longlong)pwVar10);
    if ((wVar1 == 0) && (*pwVar10 == 0)) goto LAB_1416db2cb;
    if (wVar1 != *pwVar10) {
      wVar1 = towlower(wVar1);
      wVar2 = towlower(*pwVar10);
      if (wVar1 != wVar2) break;
    }
    pwVar10 = pwVar10 + 1;
  } while( true );
  if (wVar1 != wVar2) {
    iVar8 = *(int *)(local_res18 + 0x20);
    uVar12 = (ulonglong)iVar8;
    lVar9 = *param_1;
    if (iVar8 == local_res8[0]) {
      if (*(int *)(lVar9 + 0x560) != *(int *)(lVar9 + 0x58c)) {
        lVar5 = *(longlong *)(lVar9 + 0x598);
        if (*(longlong *)(lVar9 + 0x598) == 0) {
          lVar5 = lVar9 + 0x590;
        }
        iVar6 = *(int *)(lVar5 + ((longlong)*(int *)(lVar9 + 0x5a0) - 1U & uVar12) * 4);
        if (iVar6 != -1) {
          lVar5 = *(longlong *)(lVar9 + 0x558);
          do {
            if (*(int *)((longlong)iVar6 * 0x20 + lVar5) == iVar8) {
              if (iVar6 != -1) {
                FUN_14114e9f0((longlong *)(lVar9 + 0x558),iVar8);
              }
              break;
            }
            iVar6 = *(int *)((longlong)iVar6 * 0x20 + 0x18 + lVar5);
          } while (iVar6 != -1);
        }
      }
      goto LAB_1416db405;
    }
    if (*(int *)(lVar9 + 0x4c0) != *(int *)(lVar9 + 0x4ec)) {
      lVar5 = *(longlong *)(lVar9 + 0x4f8);
      if (*(longlong *)(lVar9 + 0x4f8) == 0) {
        lVar5 = lVar9 + 0x4f0;
      }
      iVar6 = *(int *)(lVar5 + ((longlong)*(int *)(lVar9 + 0x500) - 1U & uVar12) * 4);
      if (iVar6 != -1) {
        do {
          if (*(int *)(*(longlong *)(lVar9 + 0x4b8) + (longlong)iVar6 * 0x18) == iVar8) {
            if (iVar6 != -1) {
              if (*(int *)(lVar9 + 0x560) == *(int *)(lVar9 + 0x58c)) goto LAB_1416db23e;
              lVar5 = *(longlong *)(lVar9 + 0x598);
              if (*(longlong *)(lVar9 + 0x598) == 0) {
                lVar5 = lVar9 + 0x590;
              }
              iVar6 = *(int *)(lVar5 + ((longlong)*(int *)(lVar9 + 0x5a0) - 1U & uVar12) * 4);
              if (iVar6 == -1) goto LAB_1416db23e;
              lVar5 = *(longlong *)(lVar9 + 0x558);
              goto LAB_1416db221;
            }
            break;
          }
          iVar6 = *(int *)(*(longlong *)(lVar9 + 0x4b8) + 0x10 + (longlong)iVar6 * 0x18);
        } while (iVar6 != -1);
      }
    }
    goto LAB_1416db261;
  }
  goto LAB_1416db2cb;
  while (iVar6 = *(int *)((longlong)iVar6 * 0x20 + 0x18 + lVar5), iVar6 != -1) {
LAB_1416db221:
    if (*(int *)((longlong)iVar6 * 0x20 + lVar5) == iVar8) {
      if (iVar6 != -1) goto LAB_1416db261;
      break;
    }
  }
LAB_1416db23e:
  local_40 = (longlong *)(local_res18 + 0x10);
  local_48 = (int *)(local_res18 + 0x20);
  FUN_1416d8e10((longlong *)(lVar9 + 0x558),local_res20,&local_48,0);
LAB_1416db261:
  lVar9 = *param_1;
  if (*(int *)(lVar9 + 0x510) != *(int *)(lVar9 + 0x53c)) {
    lVar5 = *(longlong *)(lVar9 + 0x548);
    if (*(longlong *)(lVar9 + 0x548) == 0) {
      lVar5 = lVar9 + 0x540;
    }
    iVar8 = *(int *)(lVar5 + ((longlong)*(int *)(lVar9 + 0x550) - 1U &
                             (longlong)*(int *)(local_res18 + 0x20)) * 4);
    if (iVar8 != -1) {
      do {
        if (*(int *)(*(longlong *)(lVar9 + 0x508) + (longlong)iVar8 * 0x18) ==
            *(int *)(local_res18 + 0x20)) {
          if (iVar8 != -1) {
            FUN_141a8e600(lVar9,local_res18 + 0x10);
            FUN_1408c04e0(*param_1 + 0x508,*(undefined4 *)(local_res18 + 0x20));
          }
          break;
        }
        iVar8 = *(int *)(*(longlong *)(lVar9 + 0x508) + 0x10 + (longlong)iVar8 * 0x18);
      } while (iVar8 != -1);
    }
  }
LAB_1416db2cb:
  local_48 = local_res8;
  local_40 = &local_res18;
  FUN_140b3a5e0(*param_1 + 0x4b8,local_res20,&local_48,0);
  FUN_141a6eae0(*param_1,&local_58,0);
  puVar11 = local_58;
  *(int *)(local_res18 + 0x20) = local_res8[0];
  ppuVar4 = (undefined2 **)(local_res18 + 0x10);
  if (ppuVar4 != &local_58) {
    lVar9 = (longlong)(int)local_50;
    *(int *)(local_res18 + 0x18) = (int)local_50;
    if (((int)local_50 == 0) && (*(int *)(local_res18 + 0x1c) == 0)) {
      *(undefined4 *)(local_res18 + 0x1c) = 0;
    }
    else {
      FUN_1407f1420(ppuVar4,(ulonglong)local_50 & 0xffffffff);
      memcpy(*ppuVar4,puVar11,lVar9 * 2);
    }
  }
LAB_1416db405:
  if (local_58 != (undefined2 *)0x0) {
    FUN_141de9200();
  }
  return;
}



// ======== d2<-0x141700200  @ 0x1416e67a0 ========

void FUN_1416e67a0(longlong param_1,int param_2)

{
  int iVar1;
  longlong *plVar2;
  longlong lVar3;
  int iVar4;
  longlong lVar5;
  
  if (*(int *)(param_1 + 0x510) != *(int *)(param_1 + 0x53c)) {
    lVar3 = *(longlong *)(param_1 + 0x548);
    if (*(longlong *)(param_1 + 0x548) == 0) {
      lVar3 = param_1 + 0x540;
    }
    iVar1 = *(int *)(lVar3 + ((longlong)*(int *)(param_1 + 0x550) - 1U & (longlong)param_2) * 4);
    if (iVar1 != -1) {
      lVar3 = *(longlong *)(param_1 + 0x508);
      iVar4 = iVar1;
      do {
        if (*(int *)(lVar3 + (longlong)iVar4 * 0x18) == param_2) {
          if (iVar4 != -1) goto joined_r0x0001416e6832;
          break;
        }
        iVar4 = *(int *)(lVar3 + 0x10 + (longlong)iVar4 * 0x18);
      } while (iVar4 != -1);
    }
  }
  plVar2 = *(longlong **)(param_1 + 0x3a8);
  if (plVar2 == (longlong *)0x0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x0001416e681d. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*plVar2 + 0x250))(plVar2,1);
  return;
joined_r0x0001416e6832:
  if (iVar1 == -1) {
LAB_1416e6860:
    lVar5 = 0;
LAB_1416e6862:
    (**(code **)(**(longlong **)(param_1 + 0x3a8) + 0x2b0))
              (*(longlong **)(param_1 + 0x3a8),*(undefined8 *)(lVar5 + 8),0);
    plVar2 = *(longlong **)(param_1 + 0x3a8);
    if (plVar2 == (longlong *)0x0) {
      return;
    }
                    /* WARNING: Could not recover jumptable at 0x0001416e688f. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*plVar2 + 0x250))(plVar2,3);
    return;
  }
  lVar5 = lVar3 + (longlong)iVar1 * 0x18;
  if (*(int *)(lVar3 + (longlong)iVar1 * 0x18) == param_2) {
    if (iVar1 != -1) goto LAB_1416e6862;
    goto LAB_1416e6860;
  }
  iVar1 = *(int *)(lVar5 + 0x10);
  goto joined_r0x0001416e6832;
}



// ======== d2<-0x141700970  @ 0x1411fd700 ========

undefined8 FUN_1411fd700(void)

{
  if (DAT_14595f480 != 0) {
    return *(undefined8 *)(DAT_14595f480 + 0x1fb8);
  }
  return 0;
}



// ======== d2<-0x141700970  @ 0x141593540 ========

void FUN_141593540(longlong *param_1,char param_2)

{
  longlong *plVar1;
  
  (**(code **)(*param_1 + 0x250))(param_1,1);
  if (param_2 <= *(char *)((longlong)param_1 + 0x70c)) {
    *(char *)((longlong)param_1 + 0x70c) = param_2;
    FUN_141593290(param_1);
    plVar1 = (longlong *)param_1[0x8c];
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 0x250))(plVar1,2);
    }
  }
  return;
}



// ======== d2<-0x141700970  @ 0x1407e2ee0 ========

void FUN_1407e2ee0(longlong *param_1)

{
  uint uVar1;
  int iVar2;
  ulonglong uVar3;
  longlong lVar4;
  
  uVar1 = *(uint *)(param_1 + 1);
  lVar4 = 4;
  if ((*(int *)((longlong)param_1 + 0xc) != 0) || (4 < uVar1)) {
    lVar4 = (longlong)(int)uVar1 + 0x10 + ((ulonglong)((longlong)(int)uVar1 * 3) >> 3);
  }
  uVar3 = FUN_141df5250(lVar4 * 2,0);
  iVar2 = (int)(uVar3 >> 1);
  if (iVar2 < (int)uVar1) {
    iVar2 = 0x7fffffff;
  }
  *(int *)((longlong)param_1 + 0xc) = iVar2;
  if ((*param_1 != 0) || (iVar2 != 0)) {
    lVar4 = FUN_141df5850(*param_1,(longlong)iVar2 * 2,0);
    *param_1 = lVar4;
  }
  return;
}



// ======== d2<-0x141700970  @ 0x141dd1c10 ========

void * FUN_141dd1c10(void *param_1,void *param_2,size_t param_3)

{
  memcpy(param_1,param_2,param_3);
  return param_1;
}



// ======== d2<-0x141700970  @ 0x14159e0a0 ========

void FUN_14159e0a0(longlong param_1,byte param_2,undefined8 param_3,char param_4)

{
  int iVar1;
  char cVar2;
  undefined1 uVar3;
  int iVar4;
  longlong lVar5;
  int iVar6;
  byte *pbVar7;
  undefined8 uVar8;
  longlong local_28 [2];
  
  if (*(longlong *)(param_1 + 0x458) == 0) {
    return;
  }
  if (*(char *)(param_1 + 0x70c) < param_4) {
    return;
  }
  *(char *)(param_1 + 0x70c) = param_4;
  if (param_2 == 0x17) {
    if ((DAT_14569c148 == '\0') || (cVar2 = FUN_1416ed980(), cVar2 == '\0')) {
LAB_14159e129:
      param_2 = 0;
    }
    else {
      param_2 = 1;
    }
LAB_14159e12c:
    if (*(int *)(param_1 + 0x3c0) == *(int *)(param_1 + 0x3ec)) {
      return;
    }
    lVar5 = *(longlong *)(param_1 + 0x3f8);
    if (*(longlong *)(param_1 + 0x3f8) == 0) {
      lVar5 = param_1 + 0x3f0;
    }
    iVar4 = *(int *)(lVar5 + ((longlong)*(int *)(param_1 + 0x400) - 1U & (ulonglong)param_2) * 4);
    if (iVar4 == -1) {
      return;
    }
    while (*(byte *)(*(longlong *)(param_1 + 0x3b8) + (longlong)iVar4 * 0x10) != param_2) {
      iVar4 = *(int *)(*(longlong *)(param_1 + 0x3b8) + 8 + (longlong)iVar4 * 0x10);
      if (iVar4 == -1) {
        return;
      }
    }
    if (iVar4 == -1) {
      return;
    }
    if (*(int *)(param_1 + 0x410) == *(int *)(param_1 + 0x43c)) {
      return;
    }
    lVar5 = *(longlong *)(param_1 + 0x448);
    if (*(longlong *)(param_1 + 0x448) == 0) {
      lVar5 = param_1 + 0x440;
    }
    iVar4 = *(int *)(lVar5 + ((longlong)*(int *)(param_1 + 0x450) - 1U & (ulonglong)param_2) * 4);
    if (iVar4 == -1) {
      return;
    }
    while (*(byte *)(*(longlong *)(param_1 + 0x408) + (longlong)iVar4 * 0x18) != param_2) {
      iVar4 = *(int *)(*(longlong *)(param_1 + 0x408) + 0x10 + (longlong)iVar4 * 0x18);
      if (iVar4 == -1) {
        return;
      }
    }
    if (iVar4 == -1) {
      return;
    }
  }
  else {
    if (param_2 == 0x18) {
      if ((DAT_14569c148 != '\0') && (cVar2 = FUN_1416ed980(), cVar2 != '\0')) goto LAB_14159e129;
      param_2 = 1;
      goto LAB_14159e12c;
    }
    if (param_2 != 0x33) goto LAB_14159e12c;
  }
  if (8 < *(int *)(param_1 + 0x4d8)) {
    return;
  }
  iVar4 = 0x32;
  if (*(int *)(param_1 + 0x3c0) != *(int *)(param_1 + 0x3ec)) {
    lVar5 = *(longlong *)(param_1 + 0x3f8);
    if (*(longlong *)(param_1 + 0x3f8) == 0) {
      lVar5 = param_1 + 0x3f0;
    }
    iVar1 = *(int *)(lVar5 + ((longlong)*(int *)(param_1 + 0x400) - 1U & (ulonglong)param_2) * 4);
    if (iVar1 != -1) {
      lVar5 = *(longlong *)(param_1 + 0x3b8);
      iVar6 = iVar1;
      do {
        if (*(byte *)(lVar5 + (longlong)iVar6 * 0x10) == param_2) {
          if (iVar6 != -1) goto joined_r0x00014159e2a0;
          break;
        }
        iVar6 = *(int *)(lVar5 + 8 + (longlong)iVar6 * 0x10);
      } while (iVar6 != -1);
    }
  }
  goto LAB_14159e2d4;
joined_r0x00014159e2a0:
  if (iVar1 == -1) goto LAB_14159e2ce;
  pbVar7 = (byte *)((longlong)iVar1 * 0x10 + lVar5);
  if (*pbVar7 == param_2) {
    if (iVar1 != -1) goto LAB_14159e2d1;
    goto LAB_14159e2ce;
  }
  iVar1 = *(int *)(pbVar7 + 8);
  goto joined_r0x00014159e2a0;
LAB_14159e2ce:
  pbVar7 = (byte *)0x0;
LAB_14159e2d1:
  iVar4 = *(int *)(pbVar7 + 4);
LAB_14159e2d4:
  switch(param_2) {
  case 0x13:
    lVar5 = FUN_1411fc890();
    lVar5 = *(longlong *)(*(longlong *)(lVar5 + 0x1ea0) + 0x108);
    if ((lVar5 != 0) && (*(int *)(lVar5 + 0xe0) == 2)) goto LAB_14159e32d;
  default:
switchD_14159e2f3_caseD_15:
    FUN_14159e8d0(param_1,param_2,iVar4,1,param_3);
    goto LAB_14159e32d;
  case 0x14:
    lVar5 = FUN_1411fc890();
    lVar5 = *(longlong *)(*(longlong *)(lVar5 + 0x1ea0) + 0x108);
    if ((lVar5 != 0) && (*(int *)(lVar5 + 0xe0) == 2)) {
      FUN_14159e8d0(param_1,0x25,iVar4,1,param_3);
      local_28[0] = 0;
      local_28[1] = 0;
      FUN_14159e8d0(param_1,0x24,iVar4 + 1,0,local_28);
      if (local_28[0] != 0) {
        FUN_141de9200();
      }
      local_28[0] = 0;
      local_28[1] = 0;
      FUN_14159e8d0(param_1,0x23,iVar4 + 2,0,local_28);
      if (local_28[0] != 0) {
        FUN_141de9200();
      }
      iVar4 = iVar4 + 3;
      uVar8 = 0x22;
      goto LAB_14159e405;
    }
    goto switchD_14159e2f3_caseD_15;
  case 0x1c:
    FUN_14159e8d0(param_1,0,iVar4,1,param_3);
    uVar8 = 1;
    break;
  case 0x1d:
    uVar3 = FUN_141593940(param_1,0x140000018);
    FUN_14159e8d0(param_1,uVar3,iVar4,1,param_3);
    uVar8 = 0x16;
    break;
  case 0x1e:
    uVar3 = FUN_141593940(param_1,0x140000018);
    FUN_14159e8d0(param_1,uVar3,iVar4,1,param_3);
    uVar8 = 0x15;
    break;
  case 0x1f:
    uVar3 = FUN_141593940(param_1,0x140000017);
    FUN_14159e8d0(param_1,uVar3,iVar4,1,param_3);
    uVar8 = 6;
    break;
  case 0x20:
    uVar3 = FUN_141593940(param_1,0x140000018);
    FUN_14159e8d0(param_1,uVar3,iVar4,1,param_3);
    local_28[0] = 0;
    local_28[1] = 0;
    FUN_14159e8d0(param_1,0xd,iVar4 + 1,0,local_28);
    if (local_28[0] != 0) {
      FUN_141de9200();
    }
    iVar4 = iVar4 + 2;
    uVar8 = 9;
    goto LAB_14159e405;
  case 0x21:
    uVar3 = FUN_141593940(param_1,0x140000017);
    FUN_14159e8d0(param_1,uVar3,iVar4,1,param_3);
    local_28[0] = 0;
    local_28[1] = 0;
    FUN_14159e8d0(param_1,0xf,iVar4 + 1,0,local_28);
    if (local_28[0] != 0) {
      FUN_141de9200();
    }
    iVar4 = iVar4 + 2;
    uVar8 = 6;
    goto LAB_14159e405;
  }
  iVar4 = iVar4 + 1;
LAB_14159e405:
  local_28[1] = 0;
  local_28[0] = 0;
  FUN_14159e8d0(param_1,uVar8,iVar4,0,local_28);
  if (local_28[0] != 0) {
    FUN_141de9200();
  }
LAB_14159e32d:
  *(undefined1 *)(param_1 + 0x4e0) = 1;
  return;
}



// ======== d2<-0x141700970  @ 0x141de9200 ========

void FUN_141de9200(longlong param_1)

{
  if (param_1 != 0) {
    if (DAT_145a8c1f8 == (longlong *)0x0) {
      FUN_141de9840();
    }
    (**(code **)(*DAT_145a8c1f8 + 0x20))(DAT_145a8c1f8,param_1);
  }
  return;
}



// ======== d2<-0x141700970  @ 0x14159fd20 ========

void FUN_14159fd20(longlong *param_1,char param_2)

{
  longlong lVar1;
  longlong *plVar2;
  undefined4 uVar3;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  (**(code **)(*param_1 + 0x250))(param_1,3);
  uVar3 = DAT_1450d6c88;
  if (param_2 <= *(char *)((longlong)param_1 + 0x70c)) {
    *(char *)((longlong)param_1 + 0x70c) = param_2;
    *(undefined1 *)((longlong)param_1 + 0x709) = 1;
    if (param_2 == '\0') {
      uVar3 = (undefined4)param_1[0x75];
    }
    lVar1 = param_1[0x98];
    if (lVar1 != 0) {
      local_18 = *(undefined4 *)(lVar1 + 0x1b0);
      local_14 = *(undefined4 *)(lVar1 + 0x1b4);
      local_10 = *(undefined4 *)(lVar1 + 0x1b8);
      local_c = uVar3;
      FUN_1429ddbd0(lVar1,&local_18);
    }
    if ((char)param_1[0xe1] != '\x01') {
      *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | 8;
      plVar2 = (longlong *)param_1[0x8b];
      *(undefined1 *)(param_1 + 0xe1) = 1;
      if (plVar2 != (longlong *)0x0) {
        (**(code **)(*plVar2 + 0x250))(plVar2,3);
      }
    }
    *(undefined1 *)((longlong)param_1 + 0x70a) = 0;
  }
  return;
}



// ======== d2<-0x1416f29f0  @ 0x1429e4dc0 ========

void FUN_1429e4dc0(longlong param_1)

{
  byte bVar1;
  longlong lVar2;
  int iVar3;
  ulonglong uVar4;
  longlong *plVar5;
  ulonglong uVar6;
  
  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 0x40;
  uVar4 = 0;
  plVar5 = *(longlong **)(param_1 + 0x170);
  bVar1 = *(byte *)(param_1 + 0x1ac);
  uVar6 = (longlong)*(int *)(param_1 + 0x178) & 0x1fffffffffffffff;
  if (plVar5 + *(int *)(param_1 + 0x178) < plVar5) {
    uVar6 = uVar4;
  }
  if (uVar6 != 0) {
    do {
      lVar2 = *plVar5;
      iVar3 = (**(code **)(*(longlong *)(lVar2 + 0x28) + 0x38))(lVar2 + 0x28);
      if (iVar3 == 1) {
        FUN_1429e4c10(lVar2);
      }
      plVar5 = plVar5 + 1;
      uVar4 = uVar4 + 1;
    } while (uVar4 != uVar6);
    bVar1 = *(byte *)(param_1 + 0x1ac);
  }
  *(byte *)(param_1 + 0x1ac) = bVar1 & 0xbf;
  FUN_1429ef8d0(param_1);
  return;
}



// ======== d2<-0x140d23070  @ 0x142d1f150 ========

undefined8 FUN_142d1f150(undefined8 param_1,int param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  int iVar3;
  longlong *local_18;
  int local_10;
  
  lVar1 = FUN_1430daf70(DAT_145c8a740,param_1,1);
  if (lVar1 != 0) {
    iVar3 = 0;
    FUN_143132fc0(lVar1,&local_18);
    for (; (-1 < local_10 && (local_10 < (int)local_18[1])); local_10 = local_10 + 1) {
      uVar2 = FUN_142058d50(*local_18 + (longlong)local_10 * 8);
      if (iVar3 == param_2) {
        return uVar2;
      }
      iVar3 = iVar3 + 1;
    }
  }
  return 0;
}



// ======== d2<-0x140d23070  @ 0x141863300 ========

longlong FUN_141863300(void)

{
  if (DAT_1459e57b8 == 0) {
    FUN_141f291f0(L"/Script/AT",L"ATPlayerController",&DAT_1459e57b8,FUN_1418646b0,0x918,0x10000004,
                  0,L"Game",&LAB_1418636f0,FUN_1407e4520,FUN_142b0aa80,FUN_141862920,&LAB_14083c250,
                  0);
  }
  return DAT_1459e57b8;
}



// ======== d2<-0x140d23070  @ 0x140d1b280 ========

void FUN_140d1b280(longlong param_1)

{
  bool bVar1;
  longlong lVar2;
  longlong lVar3;
  
  if (*(longlong *)(param_1 + 0x628) != 0) {
    return;
  }
  lVar2 = FUN_142d1f150(param_1,0);
  *(longlong *)(param_1 + 0x628) = lVar2;
  if (lVar2 != 0) {
    lVar3 = FUN_141863300();
    if ((*(int *)(*(longlong *)(lVar2 + 0x10) + 0x90) < *(int *)(lVar3 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar2 + 0x10) + 0x88) + (longlong)*(int *)(lVar3 + 0x90) * 8)
        != lVar3 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) goto LAB_140d1b2e6;
  }
  lVar2 = 0;
LAB_140d1b2e6:
  *(longlong *)(param_1 + 0x640) = lVar2;
  return;
}



// ======== d2<-0x1412b5ca0  @ 0x14187c230 ========

longlong FUN_14187c230(void)

{
  if (DAT_1459ed670 == 0) {
    FUN_141f291f0(L"/Script/AT",L"ATUICommonInputComponent",&DAT_1459ed670,_guard_check_icall,600,
                  0x1000000c,0,L"Input",&LAB_14187e530,FUN_1407e4520,_guard_check_icall,
                  &LAB_1417ebdb0,&LAB_14083c250,0);
  }
  return DAT_1459ed670;
}



// ======== d3<-0x1416daef0  @ 0x1407f1420 ========

void FUN_1407f1420(longlong *param_1,int param_2,int param_3)

{
  int iVar1;
  ulonglong uVar2;
  longlong lVar3;
  
  iVar1 = 0;
  if (param_2 != 0) {
    uVar2 = FUN_141df5250((longlong)param_2 * 2,0);
    iVar1 = (int)(uVar2 >> 1);
    if (iVar1 < param_2) {
      iVar1 = 0x7fffffff;
    }
  }
  if ((iVar1 != param_3) && ((*param_1 != 0 || (iVar1 != 0)))) {
    lVar3 = FUN_141df5850(*param_1,(longlong)iVar1 * 2,0);
    *param_1 = lVar3;
  }
  *(int *)((longlong)param_1 + 0xc) = iVar1;
  return;
}



// ======== d3<-0x1416daef0  @ 0x1438246c8 ========

void * __cdecl memcpy(void *_Dst,void *_Src,size_t _Size)

{
  void *pvVar1;
  
                    /* WARNING: Could not recover jumptable at 0x0001438246c8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pvVar1 = memcpy(_Dst,_Src,_Size);
  return pvVar1;
}



// ======== d3<-0x1416daef0  @ 0x141a69100 ========
// <fail>

// ======== d3<-0x1416daef0  @ 0x141dbbf60 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 *
FUN_141dbbf60(undefined8 *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  short *psVar1;
  int iVar2;
  undefined8 *puVar3;
  short *psVar4;
  longlong lVar5;
  int iVar6;
  undefined8 local_res18;
  undefined8 local_res20;
  undefined1 auStack_478 [32];
  undefined8 local_458;
  undefined8 *local_450;
  short local_448 [512];
  ulonglong local_48;
  
  local_48 = DAT_145909780 ^ (ulonglong)auStack_478;
  iVar6 = 0x200;
  local_res18 = param_3;
  local_res20 = param_4;
  puVar3 = (undefined8 *)FUN_14078a190();
  psVar4 = (short *)0x0;
  local_458 = 0;
  local_450 = &local_res18;
  iVar2 = __stdio_common_vswprintf(*puVar3,local_448,0x1ff,param_2);
  lVar5 = -1;
  if (iVar2 < 0) {
    iVar2 = -1;
  }
  psVar1 = local_448;
  if (0x1ff < iVar2) goto LAB_141dbbff0;
  while (iVar2 == -1) {
LAB_141dbbff0:
    do {
      iVar6 = iVar6 * 2;
      psVar4 = (short *)FUN_141df5850(psVar4,(longlong)iVar6 * 2,0);
      local_450 = &local_res18;
      local_458 = 0;
      iVar2 = __stdio_common_vswprintf(*puVar3,psVar4,(longlong)(iVar6 + -1),param_2);
      if (iVar2 < 0) {
        iVar2 = -1;
      }
      psVar1 = psVar4;
    } while (iVar6 <= iVar2);
  }
  psVar1[iVar2] = 0;
  *param_1 = 0;
  param_1[1] = 0;
  if ((psVar1 != (short *)0x0) && (*psVar1 != 0)) {
    do {
      lVar5 = lVar5 + 1;
    } while (psVar1[lVar5] != 0);
    iVar2 = (int)lVar5 + 1;
    *(int *)(param_1 + 1) = iVar2;
    if (0 < iVar2) {
      FUN_1407e2ee0(param_1,0);
    }
    FUN_141dd1c10(*param_1,psVar1,(longlong)iVar2 * 2);
  }
  if (iVar6 != 0x200) {
    FUN_141de9200(psVar1);
  }
  return param_1;
}



// ======== d3<-0x1416daef0  @ 0x1407e51a0 ========

uint FUN_1407e51a0(ushort *param_1)

{
  ushort uVar1;
  int iVar2;
  uint uVar3;
  
  uVar3 = 0;
  uVar1 = *param_1;
  while (uVar1 != 0) {
    if (uVar1 < 0x80) {
      iVar2 = toupper((int)(char)uVar1);
      uVar1 = (ushort)iVar2;
    }
    else {
      uVar1 = towupper(uVar1);
    }
    param_1 = param_1 + 1;
    uVar3 = *(uint *)(&DAT_1456dc020 + (ulonglong)(byte)((byte)uVar3 ^ (byte)uVar1) * 4) ^
            uVar3 >> 8;
    uVar3 = *(uint *)(&DAT_1456dc020 + (((ulonglong)(uVar1 >> 8) ^ (ulonglong)uVar3) & 0xff) * 4) ^
            uVar3 >> 8;
    uVar1 = *param_1;
  }
  return uVar3;
}



// ======== d3<-0x1416daef0  @ 0x1416d8e10 ========

int * FUN_1416d8e10(longlong *param_1,int *param_2,undefined8 *param_3,undefined1 *param_4)

{
  int iVar1;
  int iVar2;
  undefined8 uVar3;
  uint uVar4;
  longlong *plVar5;
  int iVar6;
  int iVar7;
  void *_Dst;
  char cVar8;
  uint uVar9;
  uint *puVar10;
  bool bVar11;
  
  iVar7 = 1;
  if (*(int *)((longlong)param_1 + 0x34) == 0) {
    iVar6 = (int)param_1[1];
    *(int *)(param_1 + 1) = iVar6 + 1;
    if (*(int *)((longlong)param_1 + 0xc) < iVar6 + 1) {
      FUN_1407f1620(param_1,iVar6);
    }
    iVar1 = (int)param_1[5];
    FUN_1407f11f0(param_1 + 2,iVar1 + 1);
    *(int *)(param_1 + 5) = (int)param_1[5] + 1;
    plVar5 = (longlong *)param_1[4];
    if ((longlong *)param_1[4] == (longlong *)0x0) {
      plVar5 = param_1 + 2;
    }
    puVar10 = (uint *)((longlong)plVar5 +
                      (longlong)((int)(iVar1 + (iVar1 >> 0x1f & 0x1fU)) >> 5) * 4);
    *puVar10 = *puVar10 & ~(1 << ((byte)iVar1 & 0x1f));
  }
  else {
    iVar6 = (int)param_1[6];
    iVar2 = *(int *)((longlong)iVar6 * 0x20 + 4 + *param_1);
    iVar1 = *(int *)((longlong)param_1 + 0x34) + -1;
    *(int *)(param_1 + 6) = iVar2;
    *(int *)((longlong)param_1 + 0x34) = iVar1;
    if (iVar1 != 0) {
      *(undefined4 *)((longlong)iVar2 * 0x20 + *param_1) = 0xffffffff;
    }
  }
  plVar5 = (longlong *)param_1[4];
  *param_2 = iVar6;
  if (plVar5 == (longlong *)0x0) {
    plVar5 = param_1 + 2;
  }
  puVar10 = (uint *)((longlong)plVar5 + (longlong)((int)(iVar6 + (iVar6 >> 0x1f & 0x1fU)) >> 5) * 4)
  ;
  *puVar10 = *puVar10 | 1 << ((byte)iVar6 & 0x1f);
  puVar10 = (uint *)((longlong)iVar6 * 0x20 + *param_1);
  uVar3 = param_3[1];
  *puVar10 = *(uint *)*param_3;
  FUN_1407e5680(puVar10 + 2,uVar3);
  puVar10[6] = 0xffffffff;
  bVar11 = false;
  uVar9 = (int)param_1[1] - *(int *)((longlong)param_1 + 0x34);
  if (uVar9 != 1) {
    if (uVar9 != 0) {
      plVar5 = (longlong *)param_1[8];
      if ((longlong *)param_1[8] == (longlong *)0x0) {
        plVar5 = param_1 + 7;
      }
      iVar6 = *(int *)((longlong)plVar5 +
                      ((longlong)(int)param_1[9] - 1U & (longlong)(int)*puVar10) * 4);
      if (iVar6 != -1) {
        do {
          if (*(uint *)((longlong)iVar6 * 0x20 + *param_1) == *puVar10) goto LAB_1416d8fac;
          iVar6 = *(int *)((longlong)iVar6 * 0x20 + 0x18 + *param_1);
        } while (iVar6 != -1);
      }
    }
    iVar6 = -1;
LAB_1416d8fac:
    bVar11 = iVar6 != -1;
    if (bVar11) {
      _Dst = (void *)((longlong)iVar6 * 0x20 + *param_1);
      if (*(longlong *)((longlong)_Dst + 8) != 0) {
        FUN_141de9200();
      }
      memmove(_Dst,puVar10,0x18);
      FUN_14084d340(param_1,*param_2,1);
      *param_2 = iVar6;
      goto LAB_1416d90b1;
    }
  }
  if (3 < uVar9) {
    uVar4 = (uVar9 >> 1) + 8;
    iVar7 = 0x1f;
    if (uVar4 != 0) {
      for (; uVar4 >> iVar7 == 0; iVar7 = iVar7 + -1) {
      }
    }
    iVar6 = 0x20;
    cVar8 = '\0';
    if (uVar4 != 0) {
      iVar6 = 0x1f - iVar7;
    }
    uVar4 = (uVar9 >> 1) + 7;
    iVar7 = 0x1f;
    if (uVar4 != 0) {
      for (; uVar4 >> iVar7 == 0; iVar7 = iVar7 + -1) {
      }
    }
    if (uVar4 != 0) {
      cVar8 = '\x1f' - (char)iVar7;
    }
    iVar7 = 1 << (~(byte)((iVar6 << 0x1a) >> 0x1f) & 0x20U - cVar8 & 0x1f);
  }
  if (((int)uVar9 < 1) || (((int)param_1[9] != 0 && (iVar7 <= (int)param_1[9])))) {
    uVar9 = (int)param_1[9] - 1U & *puVar10;
    puVar10[7] = uVar9;
    plVar5 = (longlong *)param_1[8];
    if ((longlong *)param_1[8] == (longlong *)0x0) {
      plVar5 = param_1 + 7;
    }
    puVar10[6] = *(uint *)((longlong)plVar5 +
                          ((longlong)(int)param_1[9] - 1U & (longlong)(int)uVar9) * 4);
    plVar5 = (longlong *)param_1[8];
    if ((longlong *)param_1[8] == (longlong *)0x0) {
      plVar5 = param_1 + 7;
    }
    *(int *)((longlong)plVar5 + ((longlong)(int)param_1[9] - 1U & (longlong)(int)uVar9) * 4) =
         *param_2;
  }
  else {
    *(int *)(param_1 + 9) = iVar7;
    FUN_140823c90(param_1);
  }
LAB_1416d90b1:
  if (param_4 != (undefined1 *)0x0) {
    *param_4 = bVar11;
  }
  return param_2;
}



// ======== d3<-0x1416daef0  @ 0x140b3a5e0 ========

int * FUN_140b3a5e0(longlong *param_1,int *param_2,undefined8 *param_3,undefined1 *param_4)

{
  uint *puVar1;
  int iVar2;
  undefined8 *puVar3;
  uint uVar4;
  longlong *plVar5;
  int iVar6;
  int iVar7;
  char cVar8;
  uint uVar9;
  int iVar10;
  bool bVar11;
  
  iVar7 = 1;
  if (*(int *)((longlong)param_1 + 0x34) == 0) {
    iVar10 = (int)param_1[1];
    *(int *)(param_1 + 1) = iVar10 + 1;
    if (*(int *)((longlong)param_1 + 0xc) < iVar10 + 1) {
      FUN_1407f1590(param_1,iVar10);
    }
    iVar6 = (int)param_1[5];
    FUN_1407f11f0(param_1 + 2,iVar6 + 1);
    *(int *)(param_1 + 5) = (int)param_1[5] + 1;
    plVar5 = (longlong *)param_1[4];
    if ((longlong *)param_1[4] == (longlong *)0x0) {
      plVar5 = param_1 + 2;
    }
    puVar1 = (uint *)((longlong)plVar5 + (longlong)((int)(iVar6 + (iVar6 >> 0x1f & 0x1fU)) >> 5) * 4
                     );
    *puVar1 = *puVar1 & ~(1 << ((byte)iVar6 & 0x1f));
  }
  else {
    iVar10 = (int)param_1[6];
    iVar6 = *(int *)((longlong)param_1 + 0x34) + -1;
    iVar2 = *(int *)(*param_1 + 4 + (longlong)iVar10 * 0x18);
    *(int *)(param_1 + 6) = iVar2;
    *(int *)((longlong)param_1 + 0x34) = iVar6;
    if (iVar6 != 0) {
      *(undefined4 *)(*param_1 + (longlong)iVar2 * 0x18) = 0xffffffff;
    }
  }
  plVar5 = (longlong *)param_1[4];
  *param_2 = iVar10;
  if (plVar5 == (longlong *)0x0) {
    plVar5 = param_1 + 2;
  }
  bVar11 = false;
  puVar1 = (uint *)((longlong)plVar5 + (longlong)((int)(iVar10 + (iVar10 >> 0x1f & 0x1fU)) >> 5) * 4
                   );
  *puVar1 = *puVar1 | 1 << ((byte)iVar10 & 0x1f);
  puVar1 = (uint *)(*param_1 + (longlong)iVar10 * 0x18);
  puVar3 = (undefined8 *)param_3[1];
  *puVar1 = *(uint *)*param_3;
  *(undefined8 *)(puVar1 + 2) = *puVar3;
  puVar1[4] = 0xffffffff;
  uVar9 = (int)param_1[1] - *(int *)((longlong)param_1 + 0x34);
  if (uVar9 != 1) {
    if (uVar9 != 0) {
      plVar5 = (longlong *)param_1[8];
      if ((longlong *)param_1[8] == (longlong *)0x0) {
        plVar5 = param_1 + 7;
      }
      iVar6 = *(int *)((longlong)plVar5 +
                      ((longlong)(int)param_1[9] - 1U & (longlong)(int)*puVar1) * 4);
      if (iVar6 != -1) {
        do {
          if (*(uint *)(*param_1 + (longlong)iVar6 * 0x18) == *puVar1) goto LAB_140b3a77b;
          iVar6 = *(int *)(*param_1 + 0x10 + (longlong)iVar6 * 0x18);
        } while (iVar6 != -1);
      }
    }
    iVar6 = -1;
LAB_140b3a77b:
    bVar11 = iVar6 != -1;
    if (bVar11) {
      memmove((void *)(*param_1 + (longlong)iVar6 * 0x18),puVar1,0x10);
      FUN_1407f1140(param_1,*param_2,1);
      *param_2 = iVar6;
      goto LAB_140b3a86f;
    }
  }
  if (3 < uVar9) {
    uVar4 = (uVar9 >> 1) + 8;
    iVar7 = 0x1f;
    if (uVar4 != 0) {
      for (; uVar4 >> iVar7 == 0; iVar7 = iVar7 + -1) {
      }
    }
    iVar6 = 0x20;
    if (uVar4 != 0) {
      iVar6 = 0x1f - iVar7;
    }
    uVar4 = (uVar9 >> 1) + 7;
    iVar7 = 0x1f;
    if (uVar4 != 0) {
      for (; uVar4 >> iVar7 == 0; iVar7 = iVar7 + -1) {
      }
    }
    if (uVar4 == 0) {
      cVar8 = '\0';
    }
    else {
      cVar8 = '\x1f' - (char)iVar7;
    }
    iVar7 = 1 << (~(byte)((iVar6 << 0x1a) >> 0x1f) & 0x20U - cVar8 & 0x1f);
  }
  if (((int)uVar9 < 1) || (((int)param_1[9] != 0 && (iVar7 <= (int)param_1[9])))) {
    uVar9 = (int)param_1[9] - 1U & *puVar1;
    puVar1[5] = uVar9;
    plVar5 = (longlong *)param_1[8];
    if ((longlong *)param_1[8] == (longlong *)0x0) {
      plVar5 = param_1 + 7;
    }
    puVar1[4] = *(uint *)((longlong)plVar5 +
                         ((longlong)(int)param_1[9] - 1U & (longlong)(int)uVar9) * 4);
    plVar5 = (longlong *)param_1[8];
    if ((longlong *)param_1[8] == (longlong *)0x0) {
      plVar5 = param_1 + 7;
    }
    *(int *)((longlong)plVar5 + ((longlong)(int)param_1[9] - 1U & (longlong)(int)uVar9) * 4) =
         iVar10;
  }
  else {
    *(int *)(param_1 + 9) = iVar7;
    FUN_1408a7d30(param_1);
  }
LAB_140b3a86f:
  if (param_4 != (undefined1 *)0x0) {
    *param_4 = bVar11;
  }
  return param_2;
}



// ======== d3<-0x1416daef0  @ 0x141a6eae0 ========

ulonglong FUN_141a6eae0(longlong param_1,undefined8 *param_2,int param_3)

{
  int *piVar1;
  longlong *plVar2;
  longlong *plVar3;
  void *pvVar4;
  undefined8 uVar5;
  wchar_t *pwVar6;
  int iVar7;
  char cVar8;
  uint uVar9;
  ulonglong in_RAX;
  wchar_t **ppwVar10;
  wchar_t *pwVar11;
  undefined8 *puVar12;
  ulonglong uVar13;
  longlong *plVar14;
  undefined2 *puVar15;
  int iVar16;
  longlong *plVar17;
  longlong lVar18;
  longlong lVar19;
  void *pvVar20;
  longlong *plVar21;
  int iVar22;
  undefined4 uVar23;
  longlong local_res10;
  longlong local_res20;
  undefined2 uVar24;
  wchar_t *local_108;
  undefined8 local_100;
  wchar_t *local_f8;
  undefined8 local_f0;
  undefined8 local_e8;
  int iStack_e0;
  undefined4 uStack_dc;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined8 local_c8;
  int local_b8;
  void *local_b0;
  ulonglong local_a8;
  void *local_a0;
  int local_98;
  int local_94;
  undefined1 local_90;
  undefined4 local_8c;
  undefined8 local_88;
  longlong *plStack_80;
  wchar_t **local_78;
  undefined8 *local_70;
  undefined8 local_68;
  longlong local_60;
  undefined8 local_58;
  undefined1 local_50 [8];
  longlong *local_48;
  
  if (*(int *)(param_2 + 1) < 2) {
    return in_RAX & 0xffffffffffffff00;
  }
  FUN_141eb3490(&local_res20,*param_2,1);
  plVar3 = (longlong *)(param_1 + 0x230);
  plVar21 = (longlong *)0x0;
  if (*(int *)(param_1 + 0x238) != *(int *)(param_1 + 0x264)) {
    lVar18 = *(longlong *)(param_1 + 0x270);
    if (*(longlong *)(param_1 + 0x270) == 0) {
      lVar18 = param_1 + 0x268;
    }
    iVar16 = *(int *)(lVar18 + ((longlong)((int)((ulonglong)local_res20 >> 0x20) + (int)local_res20)
                               & (longlong)*(int *)(param_1 + 0x278) - 1U) * 4);
    if (iVar16 != -1) {
      do {
        if (*(longlong *)((longlong)iVar16 * 0x70 + *plVar3) == local_res20) {
          if (iVar16 != -1) {
            if (*(int *)(param_2 + 1) == 0) {
              puVar15 = &DAT_14391f7d0;
            }
            else {
              puVar15 = (undefined2 *)*param_2;
            }
            FUN_141eb3490(&local_res10,puVar15,1);
            if (*(int *)(param_1 + 0x238) == *(int *)(param_1 + 0x264)) goto LAB_141a6ec3e;
            lVar18 = *(longlong *)(param_1 + 0x270);
            if (*(longlong *)(param_1 + 0x270) == 0) {
              lVar18 = param_1 + 0x268;
            }
            iVar16 = *(int *)(lVar18 + ((longlong)
                                        ((int)((ulonglong)local_res10 >> 0x20) + (int)local_res10) &
                                       (longlong)*(int *)(param_1 + 0x278) - 1U) * 4);
            if (iVar16 == -1) goto LAB_141a6ec3e;
            goto LAB_141a6ec20;
          }
          break;
        }
        iVar16 = *(int *)((longlong)iVar16 * 0x70 + 0x68 + *plVar3);
      } while (iVar16 != -1);
    }
  }
  goto LAB_141a6ed0a;
  while (iVar16 = (int)plVar14[0xd], iVar16 != -1) {
LAB_141a6ec20:
    plVar14 = (longlong *)((longlong)iVar16 * 0x70 + *plVar3);
    if (*plVar14 == local_res10) {
      if (iVar16 != -1) goto LAB_141a6ec41;
      break;
    }
  }
LAB_141a6ec3e:
  plVar14 = plVar21;
LAB_141a6ec41:
  plVar17 = plVar14 + 1;
  if (plVar14 == (longlong *)0x0) {
    plVar17 = plVar21;
  }
  if (plVar17 != (longlong *)0x0) {
    if (((char)plVar17[9] == '\0') || (cVar8 = FUN_142044510(*plVar17), cVar8 == '\0')) {
      *(undefined1 *)(plVar17 + 9) = 0;
    }
    else if ((*plVar17 != 0) && (cVar8 = FUN_142044510(), cVar8 != '\0')) {
      uVar9 = *(int *)(param_1 + 0x238) - *(int *)(param_1 + 0x264);
      uVar13 = (ulonglong)uVar9;
      if (uVar9 != 0) {
        lVar18 = *(longlong *)(param_1 + 0x270);
        if (*(longlong *)(param_1 + 0x270) == 0) {
          lVar18 = param_1 + 0x268;
        }
        uVar9 = *(uint *)(lVar18 + ((longlong)
                                    ((int)((ulonglong)local_res20 >> 0x20) + (int)local_res20) &
                                   (longlong)*(int *)(param_1 + 0x278) - 1U) * 4);
        uVar13 = (ulonglong)uVar9;
        if (uVar9 != 0xffffffff) {
          while( true ) {
            plVar14 = (longlong *)((longlong)(int)uVar13 * 0x70 + *plVar3);
            if (*plVar14 == local_res20) break;
            uVar13 = (ulonglong)*(uint *)(plVar14 + 0xd);
            if (*(uint *)(plVar14 + 0xd) == 0xffffffff) {
              iRam0000000000000054 = iRam0000000000000054 + 1;
              return 0xffffff01;
            }
          }
          if ((int)uVar13 != -1) goto LAB_141a6ecfc;
        }
      }
      plVar14 = plVar21;
LAB_141a6ecfc:
      *(int *)((longlong)plVar14 + 0x54) = *(int *)((longlong)plVar14 + 0x54) + 1;
      return CONCAT71((int7)(uVar13 >> 8),1);
    }
  }
LAB_141a6ed0a:
  local_108 = (wchar_t *)0x0;
  local_100 = 0;
  local_f8 = (wchar_t *)0x0;
  local_f0 = 2;
  FUN_1407e2ee0(&local_f8,0);
  pwVar6 = local_f8;
  FUN_141dd1c10(local_f8,&DAT_1444fc544,4);
  uVar9 = 0;
  uVar24 = 0;
  FUN_14082b3c0(param_2,&local_f8,0,&local_108,1,1);
  if (pwVar6 != (wchar_t *)0x0) {
    FUN_141de9200(pwVar6);
  }
  pvVar20 = (void *)*param_2;
  local_b0 = (void *)0x0;
  local_a8 = 0;
  iVar16 = *(int *)(param_2 + 1) + -1;
  if (*(int *)(param_2 + 1) == 0) {
    iVar16 = 0;
  }
  local_a0 = (void *)0x0;
  local_98 = 0;
  local_94 = 0;
  local_88 = 0;
  plStack_80 = (longlong *)0x0;
  local_d8 = 0;
  uStack_d0 = 0;
  local_c8 = 0;
  local_b8 = param_3;
  if (iVar16 != 0) {
    local_a8 = (ulonglong)(iVar16 + 1U);
    if (0 < (int)(iVar16 + 1U)) {
      FUN_1407e2ee0(&local_b0,0);
    }
    pvVar4 = local_b0;
    memcpy(local_b0,pvVar20,(longlong)iVar16 * 2);
    *(undefined2 *)((longlong)iVar16 * 2 + (longlong)pvVar4) = 0;
  }
  iVar16 = local_98;
  pvVar20 = (void *)*param_2;
  iVar22 = *(int *)(param_2 + 1) + -1;
  if (*(int *)(param_2 + 1) == 0) {
    iVar22 = 0;
  }
  if (iVar22 != 0) {
    local_98 = (uint)(local_98 == 0) + local_98 + iVar22;
    if (local_94 < local_98) {
      FUN_1407e2ee0(&local_a0,iVar16);
    }
    pvVar4 = (void *)((longlong)local_a0 + ((longlong)iVar16 - (ulonglong)(iVar16 != 0)) * 2);
    memcpy(pvVar4,pvVar20,(longlong)iVar22 * 2);
    *(undefined2 *)((longlong)iVar22 * 2 + (longlong)pvVar4) = 0;
  }
  local_f8 = (wchar_t *)0x0;
  local_f0 = 2;
  FUN_1407e2ee0(&local_f8,0);
  pwVar6 = local_f8;
  FUN_1407e4a20(local_f8,2,&DAT_1444fc7a4,2,CONCAT22(uVar24,0x3f));
  iVar16 = local_98;
  if (((int)local_f0 != 0) && (iVar22 = (int)local_f0 + -1, iVar22 != 0)) {
    lVar18 = (longlong)local_98;
    local_98 = (uint)(local_98 == 0) + local_98 + iVar22;
    if (local_94 < local_98) {
      FUN_1407e2ee0(&local_a0,iVar16);
    }
    pvVar20 = (void *)((longlong)local_a0 + (lVar18 - (ulonglong)(iVar16 != 0)) * 2);
    memcpy(pvVar20,pwVar6,(longlong)iVar22 * 2);
    *(undefined2 *)((longlong)iVar22 * 2 + (longlong)pvVar20) = 0;
  }
  if (pwVar6 != (wchar_t *)0x0) {
    FUN_141de9200(pwVar6);
  }
  iVar16 = local_98;
  pwVar6 = local_108;
  if (((int)local_100 != 0) && (iVar22 = (int)local_100 + -1, iVar22 != 0)) {
    lVar18 = (longlong)local_98;
    local_98 = (uint)(local_98 == 0) + local_98 + iVar22;
    if (local_94 < local_98) {
      FUN_1407e2ee0(&local_a0,iVar16);
    }
    pvVar20 = (void *)((longlong)local_a0 + (lVar18 - (ulonglong)(iVar16 != 0)) * 2);
    memcpy(pvVar20,pwVar6,(longlong)iVar22 * 2);
    *(undefined2 *)((longlong)iVar22 * 2 + (longlong)pvVar20) = 0;
  }
  plVar21 = plStack_80;
  local_90 = 0;
  local_8c = 1;
  local_88 = 0;
  if (plStack_80 != (longlong *)0x0) {
    plStack_80 = (longlong *)0x0;
    plVar14 = plVar21 + 1;
    *(int *)plVar14 = (int)*plVar14 + -1;
    if ((int)*plVar14 == 0) {
      (**(code **)*plVar21)(plVar21);
      piVar1 = (int *)((longlong)plVar21 + 0xc);
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*plVar21 + 8))(plVar21,1);
      }
    }
  }
  if (local_b8 == 2) {
    iVar16 = local_98 + -1;
    if (local_98 == 0) {
      iVar16 = 0;
    }
    local_100._0_4_ = iVar16 + 0xb;
    local_108 = (wchar_t *)0x0;
    local_100._4_4_ = 0;
    if (0 < (int)local_100) {
      FUN_1407e2ee0(&local_108,0);
    }
    iVar7 = local_100._4_4_;
    iVar22 = (int)local_100;
    pwVar11 = local_108;
    uVar5 = u_DataTable__1444fd2f0._8_8_;
    *(undefined8 *)local_108 = u_DataTable__1444fd2f0._0_8_;
    *(undefined8 *)(local_108 + 4) = uVar5;
    *(undefined4 *)(local_108 + 8) = u_DataTable__1444fd2f0._16_4_;
    memcpy(local_108 + 10,local_a0,(longlong)iVar16 * 2);
    pwVar11[(longlong)iVar16 + 10] = L'\0';
    local_108 = pwVar11;
    local_100._4_4_ = iVar7;
    if (iVar7 < (int)(iVar22 + 1 + (uint)(iVar22 == 0))) {
      local_100._0_4_ = iVar22;
      FUN_1407f1af0(&local_108);
      iVar22 = (int)local_100;
    }
    iVar16 = (2 - (uint)(iVar22 != 0)) + iVar22;
    local_100 = CONCAT44(local_100._4_4_,iVar16);
    if (local_100._4_4_ < iVar16) {
      FUN_1407e2ee0(&local_108,iVar22);
      iVar16 = (int)local_100;
    }
    pwVar11 = local_108;
    local_108[(longlong)iVar22 - (ulonglong)(iVar22 != 0)] = DAT_1444fc7cc;
    (local_108 + ((longlong)iVar22 - (ulonglong)(iVar22 != 0)))[1] = L'\0';
    local_98 = iVar16;
    if ((iVar16 == 0) && (local_94 == 0)) {
      local_94 = 0;
    }
    else {
      FUN_1407f1420(&local_a0,iVar16);
      memcpy(local_a0,pwVar11,(longlong)iVar16 * 2);
    }
  }
  else {
    if (local_b8 != 3) goto LAB_141a6f1c0;
    local_f8 = (wchar_t *)0x0;
    local_f0 = 3;
    FUN_1407e2ee0(&local_f8,0);
    pwVar11 = local_f8;
    FUN_141dd1c10(local_f8,&DAT_1444fcbe4,6);
    iVar16 = local_98;
    if (((int)local_f0 != 0) && (iVar22 = (int)local_f0 + -1, iVar22 != 0)) {
      lVar18 = (longlong)local_98;
      local_98 = (uint)(local_98 == 0) + iVar22 + local_98;
      if (local_94 < local_98) {
        FUN_1407e2ee0(&local_a0,iVar16);
      }
      pvVar20 = (void *)((longlong)local_a0 + (lVar18 - (ulonglong)(iVar16 != 0)) * 2);
      memcpy(pvVar20,pwVar11,(longlong)iVar22 * 2);
      *(undefined2 *)((longlong)pvVar20 + (longlong)iVar22 * 2) = 0;
    }
  }
  if (pwVar11 != (wchar_t *)0x0) {
    FUN_141de9200(pwVar11);
  }
LAB_141a6f1c0:
  pvVar20 = (void *)0x0;
  uVar13 = DAT_145a33630;
  if ((DAT_145a33630 != 0) && (lVar18 = DAT_145a33630 + 0xf0, lVar18 != 0)) {
    local_f8 = (wchar_t *)0x0;
    local_f0 = 0x20;
    FUN_1407e2ee0(&local_f8,0);
    FUN_141dd1c10(local_f8,L"RequestAsyncLoad SingleDelegate",0x40);
    uVar23 = 0;
    iStack_e0 = 0;
    local_78 = (wchar_t **)0x0;
    local_e8 = &LAB_140ccda34;
    local_70._0_4_ = 0;
    ppwVar10 = (wchar_t **)FUN_141df5850(0,0x30);
    local_70 = (undefined8 *)CONCAT44(local_70._4_4_,3);
    local_78 = ppwVar10;
    if (ppwVar10 != (wchar_t **)0x0) {
      *ppwVar10 = (wchar_t *)&PTR_FUN_143968d20;
      FUN_1420575e0(ppwVar10 + 1,param_1);
      *(undefined4 *)(ppwVar10 + 2) = (undefined4)local_e8;
      *(undefined4 *)((longlong)ppwVar10 + 0x14) = local_e8._4_4_;
      *(int *)(ppwVar10 + 3) = iStack_e0;
      *(undefined4 *)((longlong)ppwVar10 + 0x1c) = uStack_dc;
      pwVar11 = (wchar_t *)FUN_141db8700();
      ppwVar10[5] = pwVar11;
      *ppwVar10 = (wchar_t *)&PTR_FUN_143968d70;
    }
    pvVar4 = local_a0;
    lVar19 = (longlong)local_98;
    local_e8 = (void *)0x0;
    iStack_e0 = local_98;
    if (local_98 != 0) {
      FUN_1407f1420(&local_e8,local_98,0);
      pvVar20 = local_e8;
      memcpy(local_e8,pvVar4,lVar19 * 2);
      uVar23 = uStack_dc;
    }
    local_68 = 0;
    local_60 = 0;
    local_58 = 0;
    local_e8 = pvVar20;
    uStack_dc = uVar23;
    FUN_14204b770(&local_68,&local_e8);
    puVar12 = (undefined8 *)
              FUN_1430a5730(lVar18,local_50,&local_68,&local_78,0,uVar9 & 0xffffff00,0,&local_f8);
    plVar21 = plStack_80;
    plVar14 = plStack_80;
    if (&local_88 != puVar12) {
      local_88 = *puVar12;
      *puVar12 = 0;
      plVar17 = (longlong *)puVar12[1];
      if ((plVar17 != plStack_80) &&
         (puVar12[1] = 0, plVar14 = plVar17, plStack_80 != (longlong *)0x0)) {
        plVar2 = plStack_80 + 1;
        *(int *)plVar2 = (int)*plVar2 + -1;
        if ((int)*plVar2 == 0) {
          puVar12 = (undefined8 *)*plStack_80;
          plStack_80 = plVar17;
          (*(code *)*puVar12)(plVar21);
          piVar1 = (int *)((longlong)plVar21 + 0xc);
          *piVar1 = *piVar1 + -1;
          plVar14 = plStack_80;
          if (*piVar1 == 0) {
            (**(code **)(*plVar21 + 8))(plVar21,1);
            plVar14 = plStack_80;
          }
        }
      }
    }
    plStack_80 = plVar14;
    if (local_48 != (longlong *)0x0) {
      plVar21 = local_48 + 1;
      *(int *)plVar21 = (int)*plVar21 + -1;
      if ((int)*plVar21 == 0) {
        (**(code **)*local_48)(local_48);
        piVar1 = (int *)((longlong)local_48 + 0xc);
        *piVar1 = *piVar1 + -1;
        if (*piVar1 == 0) {
          (**(code **)(*local_48 + 8))(local_48,1);
        }
      }
    }
    if (local_60 != 0) {
      FUN_141de9200();
    }
    if (local_f8 != (wchar_t *)0x0) {
      FUN_141de9200();
    }
    if (*(int *)(param_2 + 1) == 0) {
      puVar15 = &DAT_14391f7d0;
    }
    else {
      puVar15 = (undefined2 *)*param_2;
    }
    FUN_141eb3490(&local_108,puVar15,1);
    local_78 = &local_108;
    local_70 = &local_d8;
    uVar13 = FUN_141a48d80(plVar3,&local_res10,&local_78,0);
  }
  plVar3 = plStack_80;
  if (plStack_80 != (longlong *)0x0) {
    plVar21 = plStack_80 + 1;
    *(int *)plVar21 = (int)*plVar21 + -1;
    if ((int)*plVar21 == 0) {
      uVar13 = (**(code **)*plStack_80)(plStack_80);
      piVar1 = (int *)((longlong)plVar3 + 0xc);
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        uVar13 = (**(code **)(*plVar3 + 8))(plVar3,1);
      }
    }
  }
  if (local_a0 != (void *)0x0) {
    uVar13 = FUN_141de9200();
  }
  if (local_b0 != (void *)0x0) {
    uVar13 = FUN_141de9200();
  }
  if (pwVar6 != (wchar_t *)0x0) {
    uVar13 = FUN_141de9200(pwVar6);
  }
  return uVar13 & 0xffffffffffffff00;
}



// ======== d3<-0x1416daef0  @ 0x141a8e600 ========

void FUN_141a8e600(longlong param_1,undefined8 *param_2)

{
  longlong *plVar1;
  int iVar2;
  int iVar3;
  longlong lVar4;
  int iVar5;
  longlong *plVar6;
  undefined2 *puVar7;
  longlong local_res8 [4];
  
  if (*(int *)(param_2 + 1) == 0) {
    puVar7 = &DAT_14391f7d0;
  }
  else {
    puVar7 = (undefined2 *)*param_2;
  }
  FUN_141eb3490(local_res8,puVar7,1);
  if (*(int *)(param_1 + 0x238) != *(int *)(param_1 + 0x264)) {
    plVar1 = (longlong *)(param_1 + 0x230);
    iVar3 = (int)((ulonglong)local_res8[0] >> 0x20);
    lVar4 = *(longlong *)(param_1 + 0x270);
    if (*(longlong *)(param_1 + 0x270) == 0) {
      lVar4 = param_1 + 0x268;
    }
    iVar2 = *(int *)(lVar4 + ((longlong)(iVar3 + (int)local_res8[0]) &
                             (longlong)*(int *)(param_1 + 0x278) - 1U) * 4);
    if (iVar2 != -1) {
      lVar4 = *plVar1;
      iVar5 = iVar2;
      while (*(longlong *)((longlong)iVar5 * 0x70 + lVar4) != local_res8[0]) {
        iVar5 = *(int *)((longlong)iVar5 * 0x70 + 0x68 + lVar4);
        if (iVar5 == -1) {
          return;
        }
      }
      if (iVar5 != -1) {
        while (iVar2 != -1) {
          plVar6 = (longlong *)((longlong)iVar2 * 0x70 + lVar4);
          if (*plVar6 == local_res8[0]) {
            if (iVar2 != -1) goto LAB_141a8e6e0;
            break;
          }
          iVar2 = (int)plVar6[0xd];
        }
        plVar6 = (longlong *)0x0;
LAB_141a8e6e0:
        *(int *)((longlong)plVar6 + 0x54) = *(int *)((longlong)plVar6 + 0x54) + -1;
        if (*(int *)(param_1 + 0x238) != *(int *)(param_1 + 0x264)) {
          lVar4 = *(longlong *)(param_1 + 0x270);
          if (*(longlong *)(param_1 + 0x270) == 0) {
            lVar4 = param_1 + 0x268;
          }
          iVar3 = *(int *)(lVar4 + ((longlong)(iVar3 + (int)local_res8[0]) &
                                   (longlong)*(int *)(param_1 + 0x278) - 1U) * 4);
          if (iVar3 != -1) {
            do {
              plVar6 = (longlong *)((longlong)iVar3 * 0x70 + *plVar1);
              if (*plVar6 == local_res8[0]) {
                if (iVar3 != -1) goto LAB_141a8e750;
                break;
              }
              iVar3 = (int)plVar6[0xd];
            } while (iVar3 != -1);
          }
        }
        plVar6 = (longlong *)0x0;
LAB_141a8e750:
        if (*(int *)((longlong)plVar6 + 0x54) < 1) {
          FUN_141a7da70(plVar1);
        }
      }
    }
  }
  return;
}



// ======== d3<-0x1416daef0  @ 0x1408c04e0 ========

undefined8 FUN_1408c04e0(longlong *param_1,int param_2)

{
  longlong lVar1;
  undefined4 *puVar2;
  uint *puVar3;
  int iVar4;
  longlong lVar5;
  byte bVar6;
  undefined4 uVar7;
  longlong lVar8;
  int iVar9;
  longlong *plVar10;
  ulonglong uVar11;
  
  if ((int)param_1[1] == *(int *)((longlong)param_1 + 0x34)) {
    return 0;
  }
  plVar10 = (longlong *)param_1[8];
  if ((longlong *)param_1[8] == (longlong *)0x0) {
    plVar10 = param_1 + 7;
  }
  uVar11 = (longlong)(int)param_1[9] - 1;
  iVar9 = *(int *)((longlong)plVar10 + ((longlong)param_2 & uVar11) * 4);
  if (iVar9 != -1) {
    lVar5 = *param_1;
    do {
      lVar8 = (longlong)iVar9;
      if (*(int *)(lVar5 + lVar8 * 0x18) == param_2) {
        lVar8 = lVar8 * 0x18;
        uVar11 = (longlong)*(int *)(lVar8 + 0x14 + lVar5) & uVar11;
        puVar2 = (undefined4 *)((longlong)plVar10 + uVar11 * 4);
        iVar4 = *(int *)((longlong)plVar10 + uVar11 * 4);
        goto joined_r0x0001408c057f;
      }
      iVar9 = *(int *)(lVar5 + 0x10 + lVar8 * 0x18);
    } while (iVar9 != -1);
  }
  return 0;
joined_r0x0001408c057f:
  if (iVar4 == -1) {
LAB_1408c05a4:
    bVar6 = (byte)iVar9 & 0x1f;
    if (*(int *)((longlong)param_1 + 0x34) != 0) {
      *(int *)(*param_1 + (longlong)(int)param_1[6] * 0x18) = iVar9;
    }
    lVar5 = *param_1;
    uVar7 = 0xffffffff;
    *(undefined4 *)(lVar5 + lVar8) = 0xffffffff;
    if (0 < *(int *)((longlong)param_1 + 0x34)) {
      uVar7 = (undefined4)param_1[6];
    }
    ((undefined4 *)(lVar5 + lVar8))[1] = uVar7;
    *(int *)((longlong)param_1 + 0x34) = *(int *)((longlong)param_1 + 0x34) + 1;
    *(int *)(param_1 + 6) = iVar9;
    plVar10 = (longlong *)param_1[4];
    if ((longlong *)param_1[4] == (longlong *)0x0) {
      plVar10 = param_1 + 2;
    }
    puVar3 = (uint *)((longlong)plVar10 +
                     (longlong)((int)(iVar9 + (iVar9 >> 0x1f & 0x1fU)) >> 5) * 4);
    *puVar3 = *puVar3 & ~(1 << bVar6 | 1U >> 0x20 - bVar6);
    return 1;
  }
  if (iVar4 == iVar9) {
    *puVar2 = *(undefined4 *)(lVar8 + lVar5 + 0x10);
    goto LAB_1408c05a4;
  }
  lVar1 = (longlong)iVar4 * 3 + 2;
  iVar4 = *(int *)(lVar5 + lVar1 * 8);
  puVar2 = (undefined4 *)(lVar5 + lVar1 * 8);
  goto joined_r0x0001408c057f;
}



// ======== d3<-0x1416daef0  @ 0x14114e9f0 ========

undefined8 FUN_14114e9f0(longlong *param_1,int param_2)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  int *piVar4;
  int iVar5;
  longlong *plVar6;
  ulonglong uVar7;
  
  if ((int)param_1[1] == *(int *)((longlong)param_1 + 0x34)) {
    return 0;
  }
  plVar6 = (longlong *)param_1[8];
  if ((longlong *)param_1[8] == (longlong *)0x0) {
    plVar6 = param_1 + 7;
  }
  uVar7 = (longlong)(int)param_1[9] - 1;
  iVar5 = *(int *)((longlong)plVar6 + ((longlong)param_2 & uVar7) * 4);
  if (iVar5 != -1) {
    lVar2 = *param_1;
    do {
      lVar3 = (longlong)iVar5 * 0x20;
      if (*(int *)(lVar3 + lVar2) == param_2) {
        lVar3 = (longlong)iVar5 * 0x20;
        uVar7 = (longlong)*(int *)(lVar3 + 0x1c + lVar2) & uVar7;
        piVar4 = (int *)((longlong)plVar6 + uVar7 * 4);
        iVar1 = *(int *)((longlong)plVar6 + uVar7 * 4);
        goto joined_r0x00014114ea82;
      }
      iVar5 = *(int *)(lVar3 + 0x18 + lVar2);
    } while (iVar5 != -1);
  }
  return 0;
joined_r0x00014114ea82:
  if (iVar1 == -1) {
LAB_14114eab0:
    if (*(longlong *)(lVar3 + 8 + *param_1) != 0) {
      FUN_141de9200();
    }
    FUN_14084d340(param_1,iVar5,1);
    return 1;
  }
  if (iVar1 == iVar5) {
    *piVar4 = *(int *)(lVar3 + lVar2 + 0x18);
    goto LAB_14114eab0;
  }
  piVar4 = (int *)(lVar2 + 0x18 + (longlong)iVar1 * 0x20);
  iVar1 = *piVar4;
  goto joined_r0x00014114ea82;
}



// ======== d3<-0x141593540  @ 0x141593290 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_141593290(longlong param_1)

{
  ulonglong uVar1;
  longlong lVar2;
  ulonglong uVar3;
  longlong *plVar4;
  int iVar5;
  ulonglong uVar6;
  
  FUN_141a8ea00();
  iVar5 = 0;
  if (0 < *(int *)(param_1 + 0x4d8)) {
    plVar4 = (longlong *)(param_1 + 0x4e8);
    do {
      FUN_1429e1d30(*plVar4,_DAT_145a8af08);
      FUN_1429e1d30(plVar4[1]);
      if ((longlong *)plVar4[1] != (longlong *)0x0) {
        (**(code **)(*(longlong *)plVar4[1] + 0x250))();
      }
      if ((longlong *)*plVar4 != (longlong *)0x0) {
        (**(code **)(*(longlong *)*plVar4 + 0x250))();
      }
      iVar5 = iVar5 + 1;
      plVar4 = plVar4 + 6;
    } while (iVar5 < *(int *)(param_1 + 0x4d8));
  }
  if (*(longlong *)(param_1 + 0x4a8) != 0) {
    FUN_1429de070();
  }
  if (*(longlong **)(param_1 + 0x4c0) != (longlong *)0x0) {
    (**(code **)(**(longlong **)(param_1 + 0x4c0) + 0x250))();
  }
  *(undefined4 *)(param_1 + 0x4d8) = 0;
  FUN_1408b2db0();
  if (0 < (int)*(uint *)(param_1 + 0x6e0)) {
    uVar3 = 0;
    uVar6 = (ulonglong)*(uint *)(param_1 + 0x6e0);
    do {
      lVar2 = *(longlong *)(param_1 + 0x6d8);
      if (*(longlong *)(param_1 + 0x6d8) == 0) {
        lVar2 = param_1 + 0x6d0;
      }
      uVar1 = (longlong)*(int *)(param_1 + 0x6e0) - 1U & uVar3;
      uVar3 = uVar3 + 1;
      *(undefined4 *)(lVar2 + uVar1 * 4) = 0xffffffff;
      uVar6 = uVar6 - 1;
    } while (uVar6 != 0);
  }
  *(undefined4 *)(param_1 + 0x6f0) = 0;
  if (*(int *)(param_1 + 0x6f4) != 0) {
    FUN_140827c50(param_1 + 0x6e8,0);
  }
  return;
}



// ======== d3<-0x1407e2ee0  @ 0x141df5250 ========

undefined8 FUN_141df5250(undefined8 param_1,undefined4 param_2)

{
  undefined8 uVar1;
  
  if (DAT_145a8c1f8 == (longlong *)0x0) {
    return param_1;
  }
                    /* WARNING: Could not recover jumptable at 0x000141df526c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar1 = (**(code **)(*DAT_145a8c1f8 + 0x28))(DAT_145a8c1f8,param_1,param_2);
  return uVar1;
}



// ======== d3<-0x1407e2ee0  @ 0x141df5850 ========

void FUN_141df5850(undefined8 param_1,undefined8 param_2,undefined4 param_3)

{
  if (DAT_145a8c1f8 == (longlong *)0x0) {
    FUN_141de9840();
  }
                    /* WARNING: Could not recover jumptable at 0x000141df589b. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*DAT_145a8c1f8 + 0x18))(DAT_145a8c1f8,param_1,param_2,param_3);
  return;
}



// ======== d3<-0x14159e0a0  @ 0x1416ed980 ========

undefined8 FUN_1416ed980(void)

{
  longlong lVar1;
  
  lVar1 = FUN_1411fc890();
  if (lVar1 != 0) {
    lVar1 = FUN_1411fc890();
    if ((*(longlong *)(lVar1 + 0x1ea0) != 0) &&
       (lVar1 = *(longlong *)(*(longlong *)(lVar1 + 0x1ea0) + 0x108), lVar1 != 0)) {
      return CONCAT71((int7)((ulonglong)lVar1 >> 8),*(int *)(lVar1 + 0xe0) != 2);
    }
  }
  return 0;
}



// ======== d3<-0x14159e0a0  @ 0x1411fc890 ========

undefined8 FUN_1411fc890(void)

{
  return DAT_14595f480;
}



// ======== d3<-0x14159e0a0  @ 0x14159e8d0 ========

void FUN_14159e8d0(longlong param_1,byte param_2,undefined4 param_3,char param_4,undefined8 *param_5
                  )

{
  int *piVar1;
  ushort _C;
  int iVar2;
  void *_Src;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  wint_t wVar7;
  int iVar8;
  longlong lVar9;
  longlong *plVar10;
  undefined8 uVar11;
  undefined8 *puVar12;
  char *pcVar13;
  undefined1 *puVar14;
  ushort *puVar15;
  longlong local_128 [2];
  undefined1 local_118 [8];
  longlong *local_110;
  undefined1 local_100 [8];
  longlong *local_f8;
  undefined1 local_e8 [8];
  longlong *local_e0;
  undefined **local_d0;
  longlong local_c8;
  longlong local_b8;
  longlong local_a8;
  longlong local_98;
  longlong local_88 [4];
  longlong local_68;
  longlong local_58;
  undefined1 local_48 [32];
  
  if (param_4 == '\x01') {
    if (*(int *)(param_5 + 1) == 0) {
      puVar15 = &DAT_14391f7d0;
    }
    else {
      puVar15 = (ushort *)*param_5;
    }
    pcVar13 = "";
    do {
      _C = *puVar15;
      if ((_C == 0) && (*pcVar13 == '\0')) goto LAB_14159e96f;
      if ((uint)_C != (int)*pcVar13) {
        wVar7 = towlower(_C);
        iVar8 = tolower((int)*pcVar13);
        if ((uint)wVar7 != (int)(char)iVar8) goto LAB_14159e96b;
      }
      puVar15 = puVar15 + 1;
      pcVar13 = pcVar13 + 1;
    } while( true );
  }
  *(undefined1 *)(param_1 + 0x4f9 + (longlong)*(int *)(param_1 + 0x4d8) * 0x30) = 1;
LAB_14159eb5a:
  *(undefined4 *)(param_1 + 0x4fc + (longlong)*(int *)(param_1 + 0x4d8) * 0x30) = param_3;
  *(byte *)(param_1 + 0x500 + (longlong)*(int *)(param_1 + 0x4d8) * 0x30) = param_2;
  puVar12 = (undefined8 *)(param_1 + 0x508 + (longlong)*(int *)(param_1 + 0x4d8) * 0x30);
  if (puVar12 != param_5) {
    iVar8 = *(int *)(param_5 + 1);
    _Src = (void *)*param_5;
    *(int *)(puVar12 + 1) = iVar8;
    if ((iVar8 == 0) && (*(int *)((longlong)puVar12 + 0xc) == 0)) {
      *(undefined4 *)((longlong)puVar12 + 0xc) = 0;
    }
    else {
      FUN_1407f1420(puVar12,iVar8);
      memcpy((void *)*puVar12,_Src,(longlong)iVar8 * 2);
    }
  }
  if (param_2 != 0x33) {
    if (*(int *)(param_1 + 0x410) != *(int *)(param_1 + 0x43c)) {
      lVar9 = *(longlong *)(param_1 + 0x448);
      if (*(longlong *)(param_1 + 0x448) == 0) {
        lVar9 = param_1 + 0x440;
      }
      iVar8 = *(int *)(lVar9 + ((longlong)*(int *)(param_1 + 0x450) - 1U & (ulonglong)param_2) * 4);
      if (iVar8 != -1) {
        do {
          lVar9 = *(longlong *)(param_1 + 0x408) + (longlong)iVar8 * 0x18;
          if (*(byte *)(*(longlong *)(param_1 + 0x408) + (longlong)iVar8 * 0x18) == param_2) {
            if (iVar8 != -1) goto LAB_14159ec52;
            break;
          }
          iVar8 = *(int *)(lVar9 + 0x10);
        } while (iVar8 != -1);
      }
    }
    lVar9 = 0;
LAB_14159ec52:
    FUN_141045e20(local_88,*(longlong *)(lVar9 + 8) + 8);
    FUN_14171adf0(*(undefined8 *)(param_1 + 0x4e8 + (longlong)*(int *)(param_1 + 0x4d8) * 0x30),
                  local_88,0);
    if (local_58 != 0) {
      FUN_141de9200();
    }
    if (local_68 != 0) {
      FUN_141de9200();
    }
    if (local_88[0] != 0) {
      FUN_141de9200();
    }
    iVar8 = *(int *)(param_1 + 0x4d8);
    plVar10 = *(longlong **)(param_1 + 0x4e8 + (longlong)iVar8 * 0x30);
    if (plVar10 != (longlong *)0x0) {
      (**(code **)(*plVar10 + 0x250))(plVar10,CONCAT71((int7)(int3)((uint)iVar8 >> 8),3));
      iVar8 = *(int *)(param_1 + 0x4d8);
    }
    lVar9 = *(longlong *)(param_1 + 0x4e8 + (longlong)iVar8 * 0x30);
    FUN_14171ef40(lVar9,*(undefined8 *)(lVar9 + 0x448));
  }
  iVar8 = *(int *)(param_1 + 0x4d8);
  plVar10 = *(longlong **)(param_1 + 0x4f0 + (longlong)iVar8 * 0x30);
  if (plVar10 != (longlong *)0x0) {
    (**(code **)(*plVar10 + 0x250))(plVar10,CONCAT71((int7)(int3)((uint)iVar8 >> 8),3));
    iVar8 = *(int *)(param_1 + 0x4d8);
  }
  iVar2 = *(int *)(param_1 + 0x6f0);
  *(int *)(param_1 + 0x6f0) = iVar2 + 1;
  if (*(int *)(param_1 + 0x6f4) < iVar2 + 1) {
    FUN_1407f1510(param_1 + 0x6e8,iVar2);
  }
  *(longlong *)(*(longlong *)(param_1 + 0x6e8) + (longlong)iVar2 * 8) =
       param_1 + 0x4e8 + (longlong)iVar8 * 0x30;
  *(int *)(param_1 + 0x4d8) = *(int *)(param_1 + 0x4d8) + 1;
  return;
LAB_14159e96b:
  if ((uint)wVar7 == (int)(char)iVar8) {
LAB_14159e96f:
    local_128[0] = 0;
    puVar14 = local_e8;
    local_128[1] = 0;
    plVar10 = local_128;
    bVar6 = true;
    bVar5 = true;
    bVar4 = false;
    bVar3 = false;
  }
  else {
    puVar14 = local_100;
    lVar9 = FUN_1411fd400(&local_d0,param_5);
    plVar10 = (longlong *)(lVar9 + 0x38);
    bVar6 = false;
    bVar5 = false;
    bVar4 = true;
    bVar3 = true;
  }
  uVar11 = FUN_141e24190(puVar14,plVar10);
  FUN_141e08fe0(local_118,uVar11);
  if ((bVar3) && (local_f8 != (longlong *)0x0)) {
    LOCK();
    plVar10 = local_f8 + 1;
    lVar9 = *plVar10;
    *(int *)plVar10 = (int)*plVar10 + -1;
    UNLOCK();
    if ((int)lVar9 == 1) {
      (**(code **)*local_f8)(local_f8);
      LOCK();
      piVar1 = (int *)((longlong)local_f8 + 0xc);
      iVar8 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar8 == 1) {
        (**(code **)(*local_f8 + 8))(local_f8,1);
      }
    }
  }
  if (bVar4) {
    if (local_98 != 0) {
      FUN_141de9200();
    }
    if (local_a8 != 0) {
      FUN_141de9200();
    }
    if (local_b8 != 0) {
      FUN_141de9200();
    }
    if (local_c8 != 0) {
      FUN_141de9200();
    }
    local_d0 = &PTR_FUN_14397c0d8;
  }
  if ((bVar5) && (local_e0 != (longlong *)0x0)) {
    LOCK();
    plVar10 = local_e0 + 1;
    lVar9 = *plVar10;
    *(int *)plVar10 = (int)*plVar10 + -1;
    UNLOCK();
    if ((int)lVar9 == 1) {
      (**(code **)*local_e0)(local_e0);
      LOCK();
      piVar1 = (int *)((longlong)local_e0 + 0xc);
      iVar8 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar8 == 1) {
        (**(code **)(*local_e0 + 8))(local_e0,1);
      }
    }
  }
  if ((bVar6) && (local_128[0] != 0)) {
    FUN_141de9200();
  }
  FUN_141e08fe0(local_48,local_118);
  FUN_141a87ad0(*(undefined8 *)(param_1 + 0x4f0 + (longlong)*(int *)(param_1 + 0x4d8) * 0x30));
  FUN_141a54c00(*(undefined8 *)(param_1 + 0x4f0 + (longlong)*(int *)(param_1 + 0x4d8) * 0x30));
  *(undefined1 *)(param_1 + 0x4f9 + (longlong)*(int *)(param_1 + 0x4d8) * 0x30) = 0;
  if (local_110 != (longlong *)0x0) {
    LOCK();
    plVar10 = local_110 + 1;
    lVar9 = *plVar10;
    *(int *)plVar10 = (int)*plVar10 + -1;
    UNLOCK();
    if ((int)lVar9 == 1) {
      (**(code **)*local_110)(local_110);
      LOCK();
      piVar1 = (int *)((longlong)local_110 + 0xc);
      iVar8 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar8 == 1) {
        (**(code **)(*local_110 + 8))(local_110);
      }
    }
  }
  goto LAB_14159eb5a;
}



// ======== d3<-0x14159e0a0  @ 0x141593940 ========

char FUN_141593940(undefined8 param_1,char param_2)

{
  char cVar1;
  
  if (param_2 == '\x17') {
    if ((DAT_14569c148 != '\0') && (cVar1 = FUN_1416ed980(), cVar1 != '\0')) {
      return '\x01';
    }
    return '\0';
  }
  if (param_2 != '\x18') {
    return param_2;
  }
  if ((DAT_14569c148 != '\0') && (cVar1 = FUN_1416ed980(), cVar1 != '\0')) {
    return '\0';
  }
  return '\x01';
}



// ======== d3<-0x141de9200  @ 0x141de9840 ========

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_141de9840(void)

{
  char cVar1;
  longlong *plVar2;
  longlong unaff_GS_OFFSET;
  undefined1 local_88 [128];
  
  if ((*(int *)(*(longlong *)(*(longlong *)(unaff_GS_OFFSET + 0x58) + (ulonglong)_tls_index * 8) +
               0x10) < DAT_145a6a3d4) && (_Init_thread_header(&DAT_145a6a3d4), DAT_145a6a3d4 == -1))
  {
    FUN_141ebd5c0(local_88);
    DAT_145a8c1f8 = (longlong *)FUN_141eb5640();
    FUN_141dccd40(DAT_145a8c1f8);
    cVar1 = (**(code **)(*DAT_145a8c1f8 + 0x70))();
    if (cVar1 == '\0') {
      plVar2 = malloc(0x38);
      if (plVar2 == (longlong *)0x0) {
        DAT_145a8c1f8 = (longlong *)0x0;
      }
      else {
        plVar2[1] = (longlong)DAT_145a8c1f8;
        *plVar2 = (longlong)&PTR_FUN_144703b10;
        InitializeCriticalSection((LPCRITICAL_SECTION)(plVar2 + 2));
        SetCriticalSectionSpinCount((LPCRITICAL_SECTION)(plVar2 + 2),4000);
        DAT_145a8c1f8 = plVar2;
      }
    }
    _DAT_145a6a3d0 = 0;
    _Init_thread_footer(&DAT_145a6a3d4);
  }
  return;
}



// ======== d3<-0x14159fd20  @ 0x1429ddbd0 ========

void FUN_1429ddbd0(longlong param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined4 local_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  uVar1 = param_2[1];
  *(undefined8 *)(param_1 + 0x1b0) = *param_2;
  *(undefined8 *)(param_1 + 0x1b8) = uVar1;
  if (*(longlong *)(param_1 + 0x1e0) != 0) {
    local_18 = *(undefined4 *)param_2;
    uStack_14 = *(undefined4 *)((longlong)param_2 + 4);
    uStack_10 = *(undefined4 *)(param_2 + 1);
    uStack_c = *(undefined4 *)((longlong)param_2 + 0xc);
    FUN_14221dce0(*(longlong *)(param_1 + 0x1e0),&local_18);
  }
  return;
}



// ======== d3<-0x1429e4dc0  @ 0x1429e4c10 ========

void FUN_1429e4c10(longlong param_1)

{
  int iVar1;
  undefined4 uVar2;
  longlong lVar3;
  undefined1 *puVar4;
  longlong *plVar5;
  undefined1 local_b8 [4];
  undefined8 local_b4;
  undefined1 local_ac;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined4 local_98;
  undefined4 local_94;
  ulonglong local_90;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  byte local_78;
  undefined4 local_3c;
  undefined4 uStack_38;
  
  if ((*(byte *)(param_1 + 0x6d9) & 1) == 0) {
    *(undefined4 *)(param_1 + 0x6a0) = 0;
    lVar3 = FUN_142058d50(param_1 + 0x380);
    if ((lVar3 != 0) && (*(longlong *)(param_1 + 0x388) != 0)) {
      uVar2 = *(undefined4 *)(param_1 + 0x6a0);
      puVar4 = (undefined1 *)
               FUN_1428635f0(local_b8,*(undefined4 *)(param_1 + 0x688),
                             *(undefined8 *)(param_1 + 0x680));
      local_3c = 0;
      uStack_38 = 0;
      local_ac = puVar4[0xc];
      local_90 = (ulonglong)DAT_1450d6c88;
      local_a8 = *(undefined8 *)(puVar4 + 0x10);
      local_a0 = *(undefined8 *)(puVar4 + 0x18);
      local_98 = *(undefined4 *)(puVar4 + 0x20);
      local_b8[0] = *puVar4;
      local_b4 = *(undefined8 *)(puVar4 + 4);
      local_94 = *(undefined4 *)(puVar4 + 0x24);
      local_78 = local_78 & 0xc0;
      local_88 = 0;
      local_80 = 0x80000000;
      local_7c = 0;
      local_84 = uVar2;
      FUN_1428759b0(param_1 + 0x380,local_b8,param_1 + 0x28,DAT_145c627f0);
      FUN_14287a920(param_1 + 0x380,param_1 + 0x28);
    }
    plVar5 = (longlong *)FUN_142058d50(param_1 + 0x378);
    (**(code **)(*plVar5 + 0x2d8))(plVar5,param_1);
    FUN_1409475b0(param_1 + 0x6a8,param_1);
    FUN_1408573e0(*(longlong *)(param_1 + 0x370) + 0x358,0);
    *(undefined8 *)(param_1 + 0x68c) = 0;
  }
  else {
    iVar1 = *(int *)(param_1 + 0x6e8);
    *(int *)(param_1 + 0x6e8) = iVar1 + 1;
    if (*(int *)(param_1 + 0x6ec) < iVar1 + 1) {
      FUN_1407f1490(param_1 + 0x6e0,iVar1);
    }
    *(undefined4 *)(*(longlong *)(param_1 + 0x6e0) + (longlong)iVar1 * 4) = 0;
  }
  return;
}



// ======== d3<-0x142d1f150  @ 0x1430daf70 ========

longlong FUN_1430daf70(undefined8 param_1,longlong *param_2,int param_3)

{
  longlong lVar1;
  undefined2 *puVar2;
  undefined8 local_res10;
  undefined2 *local_28;
  int local_20;
  undefined2 *local_18;
  int local_10;
  
  if (param_2 == (longlong *)0x0) {
    if (param_3 == 1) {
      local_res10 = 0;
      FUN_1420448c0(L"A null object was passed as a world context object to UEngine::GetWorldFromContextObject()."
                    ,3,0);
    }
    return 0;
  }
  local_res10 = CONCAT71(local_res10._1_7_,1);
  if (param_3 == 2) {
    lVar1 = FUN_14200c290(param_2,&local_res10);
  }
  else {
    lVar1 = (**(code **)(*param_2 + 0x148))(param_2);
  }
  if ((char)local_res10 != '\0') {
    if ((lVar1 == 0) && (param_3 == 1)) {
      FUN_1420422b0(param_2,&local_18,0);
      puVar2 = &DAT_14391f7d0;
      if (local_10 != 0) {
        puVar2 = local_18;
      }
      FUN_141dbbf60(&local_28,
                    L"No world was found for object (%s) passed in to UEngine::GetWorldFromContextObject()."
                    ,puVar2);
      puVar2 = &DAT_14391f7d0;
      if (local_20 != 0) {
        puVar2 = local_28;
      }
      FUN_1420448c0(puVar2,3,0);
      if (local_28 != (undefined2 *)0x0) {
        FUN_141de9200();
      }
      if (local_18 != (undefined2 *)0x0) {
        FUN_141de9200();
      }
    }
    if ((char)local_res10 != '\0') {
      return lVar1;
    }
  }
  return DAT_145c95cd8;
}



// ======== d3<-0x142d1f150  @ 0x143132fc0 ========

longlong * FUN_143132fc0(longlong param_1,longlong *param_2)

{
  *(undefined4 *)(param_2 + 1) = 0;
  *param_2 = param_1 + 0x1d8;
  return param_2;
}



// ======== d3<-0x142d1f150  @ 0x142058d50 ========

undefined8 FUN_142058d50(int *param_1)

{
  undefined8 *puVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  
  if ((((param_1[1] != 0) && (iVar2 = *param_1, -1 < iVar2)) && (iVar2 < (int)DAT_145b9e804)) &&
     (((uVar4 = iVar2 >> 0x1f & 0xffff, uVar3 = iVar2 + uVar4,
       puVar1 = (undefined8 *)
                (*(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar3 >> 0x10) * 8) +
                (longlong)(int)((uVar3 & 0xffff) - uVar4) * 0x18), puVar1 != (undefined8 *)0x0 &&
       (*(int *)(puVar1 + 2) == param_1[1])) && ((*(uint *)(puVar1 + 1) & 0x30000000) == 0)))) {
    return *puVar1;
  }
  return 0;
}



// ======== d3<-0x141863300  @ 0x141f291f0 ========

void FUN_141f291f0(undefined8 param_1,undefined8 param_2,longlong *param_3,
                  code *UNRECOVERED_JUMPTABLE,undefined4 param_5,undefined4 param_6,
                  ulonglong param_7,undefined8 param_8,undefined8 param_9,undefined8 param_10,
                  undefined8 param_11,code *param_12,code *param_13,char param_14)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  longlong *plVar4;
  bool bVar5;
  undefined1 local_res18 [8];
  undefined8 in_stack_ffffffffffffff88;
  undefined4 uVar6;
  undefined8 in_stack_ffffffffffffffa0;
  undefined4 uVar7;
  
  uVar6 = (undefined4)((ulonglong)in_stack_ffffffffffffff88 >> 0x20);
  uVar7 = (undefined4)((ulonglong)in_stack_ffffffffffffffa0 >> 0x20);
  if (param_14 == '\0') {
    lVar1 = FUN_1420384d0(&DAT_145b9e540,0x208,8,1);
    *param_3 = lVar1;
    puVar2 = (undefined8 *)FUN_141eb3490(local_res18,param_2,1);
    puVar2 = (undefined8 *)
             FUN_141f1ece0(lVar1,0,*puVar2,param_5,CONCAT44(uVar6,param_6),param_7,param_8,
                           CONCAT44(uVar7,199),param_9,param_10,param_11);
  }
  else {
    puVar2 = (undefined8 *)FUN_1420384d0(&DAT_145b9e540,0x270,8,DAT_1456da000);
    *param_3 = (longlong)puVar2;
    bVar5 = DAT_1456da000 != '\0';
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,param_2,1);
    FUN_141f1ece0(puVar2,0,*puVar3,param_5,CONCAT44(uVar6,param_6) | 0x40000,param_7,param_8,
                  CONCAT44(uVar7,-(uint)bVar5) & 0xffffffff00000080 | 0x4000043,param_9,param_10,
                  param_11);
    *puVar2 = &PTR_FUN_144761630;
    puVar2[0x41] = 0;
    puVar2[0x42] = 0;
    puVar2[0x43] = 0;
    puVar2[0x44] = 0;
    puVar2[0x45] = 0;
    puVar2[0x46] = 0;
    puVar2[0x47] = 0;
    puVar2[0x48] = 0;
    puVar2[0x49] = 0;
    puVar2[0x4a] = 0;
    puVar2[0x4b] = 0;
    puVar2[0x4c] = 0;
    puVar2[0x4d] = 0;
  }
  *param_3 = (longlong)puVar2;
  lVar1 = (*param_13)();
  param_3 = (longlong *)*param_3;
  plVar4 = (longlong *)(*param_12)();
  param_7 = param_7 & 0xffffffffffffff00;
  uVar6 = 0;
  FUN_141f07780(param_1,param_2,0,1,0,param_7);
  if (plVar4 == param_3) {
    plVar4 = (longlong *)0x0;
  }
  (**(code **)(*param_3 + 0x2a0))(param_3,plVar4);
  param_3[0x18] = lVar1;
  (**(code **)(*param_3 + 8))(param_3);
  if ((*(uint *)(param_3 + 1) >> 0x1a & 1) == 0) {
    FUN_142049670(param_3,param_1,param_2);
  }
  else {
    if (DAT_145b9c8a0 == 0) {
      param_7 = CONCAT44((int)(param_7 >> 0x20),0x10000000);
      FUN_141f291f0(L"/Script/CoreUObject",L"DynamicClass",&DAT_145b9c8a0,_guard_check_icall,
                    CONCAT44(uVar6,0x270),param_7,0,L"Engine",FUN_141f1c610,FUN_141f1c930,
                    FUN_141f21430,FUN_141f28040,&LAB_141fd55f0,0);
    }
    (**(code **)(*param_3 + 0x10))(param_3,DAT_145b9c8a0,param_1,param_2);
  }
  FUN_141f07780(param_1,param_2,0,2,0,param_7 & 0xffffffffffffff00);
                    /* WARNING: Could not recover jumptable at 0x000141f29547. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}



