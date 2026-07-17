// UAT_UIXcmnAgreement (0x5e8) native impls + callees depth3.
// SubjectImage 0x3a8, HeadImage 0x3b0, LeftArrow 0x3c0, RightArrow 0x3c8, Map 0x508.
// ======== IMPL CheckHovered  @ 0x1416e4b40 ========

void FUN_1416e4b40(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  
  uVar2 = FUN_1411fc890();
  cVar1 = FUN_14120f070(uVar2);
  if (cVar1 != '\0') {
    uVar2 = 0;
    if (((*(longlong **)(param_1 + 0x3c0) != (longlong *)0x0) &&
        (*(longlong *)(param_1 + 0x3c8) != 0)) &&
       ((cVar1 = (**(code **)(**(longlong **)(param_1 + 0x3c0) + 600))(), cVar1 != '\0' ||
        (cVar1 = (**(code **)(**(longlong **)(param_1 + 0x3c8) + 600))(), cVar1 != '\0')))) {
      uVar2 = 1;
      FUN_141485f00(0,DAT_1450d6c88,0,0);
    }
    FUN_140d3c540(uVar2);
    return;
  }
  return;
}



// ======== IMPL Loop_Arrow  @ 0x1416f29f0 ========

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



// ======== IMPL MouseClickDecide  @ 0x1416eec40 ========

void FUN_1416eec40(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  char cVar3;
  byte bVar4;
  byte bVar5;
  int iVar6;
  undefined8 uVar7;
  
  uVar7 = FUN_1411fc890();
  cVar3 = FUN_14120f070(uVar7);
  if (cVar3 != '\0') {
    lVar2 = *(longlong *)(param_1 + 0x3c0);
    bVar4 = 0;
    if (lVar2 != 0) {
      bVar4 = FUN_1429b40b0(lVar2,*(undefined8 *)(lVar2 + 0x3b8));
    }
    lVar2 = *(longlong *)(param_1 + 0x3c8);
    if (lVar2 != 0) {
      bVar5 = FUN_1429b40b0(lVar2,*(undefined8 *)(lVar2 + 0x3b8));
      bVar4 = bVar4 | bVar5;
    }
    if ((bVar4 == 0) && (iVar1 = *(int *)(param_1 + 0x5b0), 0 < iVar1)) {
      iVar6 = *(int *)(param_1 + 0x5a8) + 1;
      if (iVar6 < iVar1) {
        *(int *)(param_1 + 0x5ac) = iVar6;
        FUN_1416f2640(param_1,0);
        uVar7 = 0;
      }
      else {
        uVar7 = CONCAT71((uint7)(uint3)((uint)iVar1 >> 8),3);
      }
      FUN_141485f00(uVar7,DAT_1450d6c88,0,0);
      return;
    }
  }
  return;
}



// ======== IMPL MouseClickTabLeft  @ 0x1416eed50 ========

void FUN_1416eed50(longlong param_1)

{
  int iVar1;
  char cVar2;
  undefined8 uVar3;
  
  uVar3 = FUN_1411fc890();
  cVar2 = FUN_14120f070(uVar3);
  if ((cVar2 != '\0') && (0 < *(int *)(param_1 + 0x5b0))) {
    iVar1 = *(int *)(param_1 + 0x5a8) + -1;
    if (*(int *)(param_1 + 0x5a8) < 1) {
      uVar3 = CONCAT71((uint7)(uint3)((uint)iVar1 >> 8),3);
    }
    else {
      *(int *)(param_1 + 0x5ac) = iVar1;
      FUN_1416f2640(param_1,1);
      uVar3 = 0;
    }
    FUN_141485f00(uVar3,DAT_1450d6c88,0,0);
    return;
  }
  return;
}



// ======== IMPL MouseClickTabRight  @ 0x1416eee20 ========

void FUN_1416eee20(longlong param_1)

{
  int iVar1;
  char cVar2;
  int iVar3;
  undefined8 uVar4;
  
  uVar4 = FUN_1411fc890();
  cVar2 = FUN_14120f070(uVar4);
  if ((cVar2 != '\0') && (iVar1 = *(int *)(param_1 + 0x5b0), 0 < iVar1)) {
    iVar3 = *(int *)(param_1 + 0x5a8) + 1;
    if (iVar3 < iVar1) {
      *(int *)(param_1 + 0x5ac) = iVar3;
      FUN_1416f2640(param_1,0);
      uVar4 = 0;
    }
    else {
      uVar4 = CONCAT71((uint7)(uint3)((uint)iVar1 >> 8),3);
    }
    FUN_141485f00(uVar4,DAT_1450d6c88,0,0);
    return;
  }
  return;
}



// ======== d1<-0x1416e4b40  @ 0x1411fc890 ========

undefined8 FUN_1411fc890(void)

{
  return DAT_14595f480;
}



// ======== d1<-0x1416e4b40  @ 0x14120f070 ========

undefined1 FUN_14120f070(longlong param_1)

{
  if (*(longlong *)(param_1 + 0x20a8) != 0) {
    return *(undefined1 *)(*(longlong *)(param_1 + 0x20a8) + 0x32);
  }
  return 0;
}



// ======== d1<-0x1416e4b40  @ 0x141485f00 ========

undefined8
FUN_141485f00(undefined1 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  bool bVar1;
  longlong lVar2;
  longlong lVar3;
  undefined8 uVar4;
  
  lVar2 = FUN_1411fc890();
  if (lVar2 != 0) {
    lVar3 = FUN_141760780();
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
      uVar4 = FUN_141486a60(*(undefined8 *)(lVar2 + 0x1ff0),param_1,param_2,param_3,param_4,0,1);
      return uVar4;
    }
  }
  return 0xffffffff;
}



// ======== d1<-0x1416e4b40  @ 0x140d3c540 ========

void FUN_140d3c540(char param_1)

{
  longlong lVar1;
  
  lVar1 = FUN_140d2a280();
  *(char *)(lVar1 + 0x571) = param_1 * '\b' + '\x01';
  return;
}



// ======== d1<-0x1416f29f0  @ 0x1429e4dc0 ========

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



// ======== d1<-0x1416eec40  @ 0x1429b40b0 ========

bool FUN_1429b40b0(longlong param_1,longlong param_2)

{
  longlong *plVar1;
  longlong lVar2;
  int iVar3;
  longlong *plVar4;
  
  if (param_2 != 0) {
    plVar4 = *(longlong **)(param_1 + 0x170);
    plVar1 = plVar4 + *(int *)(param_1 + 0x178);
    for (; plVar4 != plVar1; plVar4 = plVar4 + 1) {
      lVar2 = *plVar4;
      if (*(longlong *)(lVar2 + 0x370) == param_2) {
        if (plVar4 == (longlong *)0x0) {
          return false;
        }
        if (lVar2 == 0) {
          return false;
        }
        iVar3 = (**(code **)(*(longlong *)(lVar2 + 0x28) + 0x38))(lVar2 + 0x28);
        return iVar3 == 1;
      }
    }
  }
  return false;
}



// ======== d1<-0x1416eec40  @ 0x1416f2640 ========

void FUN_1416f2640(longlong param_1,char param_2)

{
  longlong *plVar1;
  longlong lVar2;
  undefined8 uVar3;
  longlong **pplVar4;
  longlong *local_28;
  int local_20;
  undefined8 local_18;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  local_28 = (longlong *)0x0;
  local_20 = 0;
  local_18 = FUN_1416f0030;
  uStack_10 = 0;
  plVar1 = (longlong *)FUN_141df5850(0,0x30,0);
  if (plVar1 == (longlong *)0x0) {
    if ((local_20 != 0) && (local_28 != (longlong *)0x0)) {
      (**(code **)(*local_28 + 0x30))(local_28,0);
      if (local_28 != (longlong *)0x0) {
        local_28 = (longlong *)FUN_141df5850(local_28,0,0);
      }
      local_20 = 0;
    }
  }
  else {
    *plVar1 = (longlong)&PTR_FUN_143968d20;
    FUN_1420575e0(plVar1 + 1,param_1);
    *(undefined4 *)(plVar1 + 2) = (undefined4)local_18;
    *(undefined4 *)((longlong)plVar1 + 0x14) = local_18._4_4_;
    *(undefined4 *)(plVar1 + 3) = uStack_10;
    *(undefined4 *)((longlong)plVar1 + 0x1c) = uStack_c;
    lVar2 = FUN_141db8700();
    plVar1[5] = lVar2;
    *plVar1 = (longlong)&PTR_FUN_143968d70;
    (*(code *)PTR_FUN_143968da8)(plVar1,&local_28);
  }
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x30))(plVar1,0);
    lVar2 = FUN_141df5850(plVar1,0,0);
    if (lVar2 != 0) {
      FUN_141de9200(lVar2);
    }
  }
  if ((*(longlong *)(param_1 + 0x3c0) != 0) &&
     (FUN_1429e4dc0(*(longlong *)(param_1 + 0x3c0)), param_2 != '\0')) {
    uVar3 = (**(code **)(**(longlong **)(param_1 + 0x20) + 0x148))();
    lVar2 = FUN_140d23070(uVar3,0);
    if ((lVar2 != 0) &&
       ((lVar2 = FUN_1412b5ca0(lVar2), lVar2 != 0 && (*(longlong *)(lVar2 + 0x248) != 0)))) {
      *(undefined1 *)(*(longlong *)(lVar2 + 0x248) + 0x402) = 0;
    }
    lVar2 = *(longlong *)(param_1 + 0x3c0);
    FUN_1416f3810(lVar2);
    pplVar4 = (longlong **)(lVar2 + 0x3c8);
    if (&local_28 != pplVar4) {
      if ((local_20 == 0) || (local_28 == (longlong *)0x0)) {
        if ((*(int *)(lVar2 + 0x3d0) != 0) && (plVar1 = *pplVar4, plVar1 != (longlong *)0x0)) {
          (**(code **)(*plVar1 + 0x30))(plVar1,0);
          if (*pplVar4 != (longlong *)0x0) {
            plVar1 = (longlong *)FUN_141df5850(*pplVar4,0,0);
            *pplVar4 = plVar1;
          }
          *(undefined4 *)(lVar2 + 0x3d0) = 0;
        }
      }
      else {
        (**(code **)(*local_28 + 0x38))(local_28,pplVar4);
      }
    }
  }
  if ((*(longlong *)(param_1 + 0x3c8) != 0) &&
     (FUN_1429e4dc0(*(longlong *)(param_1 + 0x3c8)), param_2 == '\0')) {
    uVar3 = (**(code **)(**(longlong **)(param_1 + 0x20) + 0x148))();
    lVar2 = FUN_140d23070(uVar3,0);
    if ((lVar2 != 0) &&
       ((lVar2 = FUN_1412b5ca0(lVar2), lVar2 != 0 && (*(longlong *)(lVar2 + 0x248) != 0)))) {
      *(undefined1 *)(*(longlong *)(lVar2 + 0x248) + 0x402) = 0;
    }
    lVar2 = *(longlong *)(param_1 + 0x3c8);
    FUN_1416f3810(lVar2);
    pplVar4 = (longlong **)(lVar2 + 0x3c8);
    if (&local_28 != pplVar4) {
      if ((local_20 == 0) || (local_28 == (longlong *)0x0)) {
        if ((*(int *)(lVar2 + 0x3d0) != 0) && (plVar1 = *pplVar4, plVar1 != (longlong *)0x0)) {
          (**(code **)(*plVar1 + 0x30))(plVar1,0);
          if (*pplVar4 != (longlong *)0x0) {
            plVar1 = (longlong *)FUN_141df5850(*pplVar4,0,0);
            *pplVar4 = plVar1;
          }
          *(undefined4 *)(lVar2 + 0x3d0) = 0;
        }
      }
      else {
        (**(code **)(*local_28 + 0x38))(local_28,pplVar4);
      }
    }
  }
  if (local_20 != 0) {
    if (local_28 == (longlong *)0x0) {
      return;
    }
    (**(code **)(*local_28 + 0x30))(local_28,0);
    if (local_28 != (longlong *)0x0) {
      local_28 = (longlong *)FUN_141df5850(local_28,0,0);
    }
    local_20 = 0;
  }
  if (local_28 != (longlong *)0x0) {
    FUN_141de9200();
  }
  return;
}



// ======== d2<-0x141485f00  @ 0x141760780 ========

longlong FUN_141760780(void)

{
  if (DAT_145998698 == 0) {
    FUN_141f291f0(L"/Script/AT",L"AT_GameInstance",&DAT_145998698,FUN_141763890,0x2148,0x10000008,0,
                  L"Game",&LAB_141760f70,FUN_1407e4520,_guard_check_icall,&LAB_141763880,
                  &LAB_14083c250,0);
  }
  return DAT_145998698;
}



// ======== d2<-0x141485f00  @ 0x141486a60 ========

int FUN_141486a60(longlong param_1,uint param_2,float param_3,float param_4,float param_5,
                 int param_6,uint param_7)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  longlong lVar4;
  longlong lVar5;
  undefined2 *puVar6;
  undefined4 uVar7;
  float fVar8;
  undefined8 local_res8;
  float fVar9;
  
  if ((*(longlong *)(param_1 + 0x48) == 0) || ((int)param_2 < 0)) {
    return -1;
  }
  if (param_4 <= DAT_143e439b8) {
    param_4 = DAT_143e439b8;
  }
  if (param_3 <= 0.0) {
    param_3 = 0.0;
  }
  if (param_5 <= DAT_1439fac60) {
    param_5 = DAT_1439fac60;
  }
  if (DAT_143a56870 <= param_4) {
    param_4 = DAT_143a56870;
  }
  if (DAT_1450d6c88 <= param_3) {
    param_3 = DAT_1450d6c88;
  }
  if (DAT_14391b480 <= param_5) {
    param_5 = DAT_14391b480;
  }
  if (param_6 == 0) {
    fVar8 = *(float *)(param_1 + 0x38);
    uVar7 = 2;
  }
  else {
    if ((param_6 != 1) || (0xe < param_7)) {
      return -1;
    }
    fVar8 = *(float *)(param_1 + 0x3c);
    uVar7 = 3;
  }
  if ((&DAT_14543f6d8)[(longlong)(int)param_7 * 0x10] == 0) {
    puVar6 = &DAT_14391f7d0;
  }
  else {
    puVar6 = (undefined2 *)(&DAT_14543f6d0)[(longlong)(int)param_7 * 8];
  }
  FUN_141eb3490(&local_res8,puVar6,1);
  cVar2 = FUN_140c51ef0(&local_res8,&DAT_143e3d7f8);
  if (cVar2 == '\0') {
    lVar4 = FUN_141478360(param_1,local_res8);
    goto LAB_141486bdb;
  }
  lVar4 = FUN_1411fc890();
  if (lVar4 == 0) {
LAB_141486bb9:
    lVar4 = 0;
  }
  else {
    lVar5 = FUN_141760780();
    if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) + (longlong)*(int *)(lVar5 + 0x90) * 8)
        != lVar5 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_141486bb9;
  }
  lVar4 = *(longlong *)(*(longlong *)(lVar4 + 0x2090) + 0x200);
LAB_141486bdb:
  if (lVar4 == 0) {
    iVar3 = -1;
  }
  else {
    fVar9 = param_4;
    iVar3 = FUN_141a8b460(*(undefined8 *)(param_1 + 0x48),lVar4,param_2 & 0xfff,uVar7,param_7,
                          param_3 * fVar8,param_4,param_5,DAT_14391b488,0);
    if (-1 < iVar3) {
      FUN_141470a60(param_1,param_2 & 0xfff,iVar3,param_6,param_4,param_3 * fVar8,
                    (uint)fVar9 & 0xffffff00);
      *(int *)(param_1 + 0x1c0c) = iVar3;
    }
  }
  return iVar3;
}



// ======== d2<-0x140d3c540  @ 0x140d2a280 ========

longlong FUN_140d2a280(void)

{
  bool bVar1;
  char cVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong lVar5;
  int local_res8 [8];
  
  lVar5 = DAT_14592f0e8;
  if (DAT_14592f0e8 != 0) {
    uVar3 = FUN_1418fb4b0();
    FUN_140cfa640(lVar5 + 0x550,local_res8,uVar3);
    if ((((local_res8[0] != -1) &&
         (lVar5 = *(longlong *)(lVar5 + 0x550) + (longlong)local_res8[0] * 0x28, lVar5 != 0)) &&
        (lVar5 != -8)) && (lVar5 = *(longlong *)(lVar5 + 0x10), lVar5 != 0)) {
      lVar4 = FUN_1418fb4b0();
      if ((*(int *)(*(longlong *)(lVar5 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
         (*(longlong *)
           (*(longlong *)(*(longlong *)(lVar5 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8
           ) != lVar4 + 0x88)) {
        bVar1 = false;
      }
      else {
        bVar1 = true;
      }
      if (((bVar1) && (cVar2 = FUN_141435950(lVar5), cVar2 != '\0')) &&
         ((cVar2 = FUN_1414359a0(lVar5), cVar2 == '\0' && (*(longlong *)(lVar5 + 0x130) != 0)))) {
        lVar5 = *(longlong *)(*(longlong *)(lVar5 + 0x130) + 0x370);
        if (lVar5 == 0) {
          return 0;
        }
        lVar4 = FUN_14323a770();
        if ((*(int *)(*(longlong *)(lVar5 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(lVar5 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar4 + 0x90) * 8) != lVar4 + 0x88)) {
          bVar1 = false;
        }
        else {
          bVar1 = true;
        }
        if (!bVar1) {
          return 0;
        }
        return lVar5;
      }
    }
  }
  uVar3 = FUN_1411fc890();
  if (DAT_14592f0f0 == 0) {
    lVar5 = FUN_142d1f150(uVar3,0);
    return lVar5;
  }
  if (DAT_14592f0f0 != 1) {
    FUN_142d1f150(uVar3,0);
  }
  lVar5 = DAT_14592f0e8;
  if (DAT_14592f0e8 == 0) {
    return 0;
  }
  FUN_140d1b280(DAT_14592f0e8);
  return *(longlong *)(lVar5 + 0x628);
}



// ======== d2<-0x1429e4dc0  @ 0x1429e4c10 ========

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



// ======== d2<-0x1416f2640  @ 0x141df5850 ========

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



// ======== d2<-0x1416f2640  @ 0x1420575e0 ========

void FUN_1420575e0(undefined4 *param_1,longlong param_2)

{
  undefined4 uVar1;
  
  if (param_2 != 0) {
    *param_1 = *(undefined4 *)(param_2 + 0xc);
    uVar1 = FUN_142038410(&DAT_145b9e7e0);
    param_1[1] = uVar1;
    return;
  }
  *param_1 = 0xffffffff;
  param_1[1] = 0;
  return;
}



// ======== d2<-0x1416f2640  @ 0x141db8700 ========

longlong FUN_141db8700(void)

{
  longlong lVar1;
  longlong lVar2;
  
  LOCK();
  UNLOCK();
  lVar2 = DAT_1456d85c0 + 1;
  lVar1 = DAT_1456d85c0 + 1;
  if (lVar2 == 0) {
    LOCK();
    UNLOCK();
    lVar2 = DAT_1456d85c0 + 2;
    lVar1 = DAT_1456d85c0 + 2;
  }
  DAT_1456d85c0 = lVar1;
  return lVar2;
}



// ======== d2<-0x1416f2640  @ 0x140861a90 ========

void FUN_140861a90(longlong param_1,longlong *param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  int iVar3;
  longlong lVar4;
  longlong *plVar5;
  
  iVar3 = (int)param_2[1];
  if (iVar3 == 0) {
    plVar5 = (longlong *)0x0;
  }
  else {
    plVar5 = (longlong *)*param_2;
  }
  if (plVar5 != (longlong *)0x0) {
    (**(code **)(*plVar5 + 0x30))(plVar5,0);
    iVar3 = (int)param_2[1];
  }
  if (iVar3 != 3) {
    lVar4 = FUN_141df5850(*param_2,0x30);
    *param_2 = lVar4;
    *(undefined4 *)(param_2 + 1) = 3;
  }
  puVar1 = (undefined8 *)*param_2;
  if (puVar1 != (undefined8 *)0x0) {
    *puVar1 = &PTR_FUN_143968d20;
    *(undefined4 *)(puVar1 + 1) = *(undefined4 *)(param_1 + 8);
    *(undefined4 *)((longlong)puVar1 + 0xc) = *(undefined4 *)(param_1 + 0xc);
    uVar2 = *(undefined8 *)(param_1 + 0x18);
    puVar1[2] = *(undefined8 *)(param_1 + 0x10);
    puVar1[3] = uVar2;
    puVar1[5] = *(undefined8 *)(param_1 + 0x28);
    *puVar1 = &PTR_FUN_143968d70;
  }
  return;
}



// ======== d2<-0x1416f2640  @ 0x1407fe110 ========

undefined8 * FUN_1407fe110(undefined8 *param_1,ulonglong param_2)

{
  *param_1 = &PTR__purecall_143a2de70;
  if ((param_2 & 1) != 0) {
    thunk_FUN_141de9200(param_1,0x30);
  }
  return param_1;
}



// ======== d2<-0x1416f2640  @ 0x141de9200 ========

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



// ======== d2<-0x1416f2640  @ 0x140d23070 ========

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



// ======== d2<-0x1416f2640  @ 0x1412b5ca0 ========

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



// ======== d2<-0x1416f2640  @ 0x1416f3810 ========

void FUN_1416f3810(longlong *param_1)

{
  longlong *plVar1;
  char cVar2;
  longlong lVar3;
  
  FUN_1429e4dc0();
  if (((int)param_1[0x7a] != 0) && ((longlong *)param_1[0x79] != (longlong *)0x0)) {
    cVar2 = (**(code **)(*(longlong *)param_1[0x79] + 0x20))();
    if ((cVar2 != '\0') &&
       (((int)param_1[0x7a] != 0 && (plVar1 = (longlong *)param_1[0x79], plVar1 != (longlong *)0x0))
       )) {
      (**(code **)(*plVar1 + 0x30))(plVar1,0);
      if (param_1[0x79] != 0) {
        lVar3 = FUN_141df5850(param_1[0x79],0,0);
        param_1[0x79] = lVar3;
      }
      *(undefined4 *)(param_1 + 0x7a) = 0;
    }
  }
  (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x77],0,1,0,DAT_1450d6c88);
  return;
}



// ======== d3<-0x141760780  @ 0x141f291f0 ========

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



// ======== d3<-0x141486a60  @ 0x141eb3490 ========

undefined8 * FUN_141eb3490(undefined8 *param_1,longlong param_2,undefined4 param_3)

{
  if (param_2 != 0) {
    FUN_141ebf920(param_1,param_2,0,param_3,1,0xffffffff);
    return param_1;
  }
  *param_1 = 0;
  return param_1;
}



// ======== d3<-0x141486a60  @ 0x140c51ef0 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

bool FUN_140c51ef0(longlong *param_1,ushort *param_2)

{
  ushort _C;
  char cVar1;
  wint_t wVar2;
  wint_t wVar3;
  int iVar4;
  byte *pbVar5;
  wint_t *pwVar6;
  int iVar7;
  longlong lVar8;
  undefined1 auStack_858 [32];
  undefined8 local_838;
  ushort local_828 [1024];
  ulonglong local_28;
  
  local_28 = DAT_145909780 ^ (ulonglong)auStack_858;
  if ((param_2 == (ushort *)0x0) || (*param_2 == 0)) {
    return *param_1 == 0;
  }
  pbVar5 = (byte *)FUN_141eb81a0();
  if (pbVar5 == (byte *)0x0) {
    return false;
  }
  iVar7 = 0;
  local_838 = (ulonglong)local_838._4_4_ << 0x20;
  iVar4 = 0;
  cVar1 = thunk_FUN_141eb2bc0(param_2,local_828,0x400,&local_838);
  if (cVar1 != '\0') {
    param_2 = local_828;
    iVar4 = (int)local_838 + 1;
  }
  if (iVar4 != *(int *)((longlong)param_1 + 4)) {
    return false;
  }
  pwVar6 = (wint_t *)(pbVar5 + 0x10);
  if ((*pbVar5 & 1) == 0) {
    do {
      _C = *param_2;
      if ((_C == 0) && ((byte)*pwVar6 == 0)) goto LAB_140c5201f;
      if ((uint)_C != (int)(char)(byte)*pwVar6) {
        wVar2 = towlower(_C);
        iVar4 = tolower((int)(char)(byte)*pwVar6);
        if ((uint)wVar2 != (int)(char)iVar4) goto LAB_140c5201b;
      }
      param_2 = param_2 + 1;
      pwVar6 = (wint_t *)((longlong)pwVar6 + 1);
    } while( true );
  }
  lVar8 = (longlong)param_2 - (longlong)pwVar6;
  do {
    wVar2 = *(wint_t *)(lVar8 + (longlong)pwVar6);
    if ((wVar2 == 0) && (iVar7 = 0, *pwVar6 == 0)) goto LAB_140c5201f;
    if (wVar2 != *pwVar6) {
      wVar2 = towlower(wVar2);
      wVar3 = towlower(*pwVar6);
      if (wVar2 != wVar3) goto LAB_140c51fd0;
    }
    pwVar6 = pwVar6 + 1;
  } while( true );
LAB_140c5201b:
  iVar7 = (uint)wVar2 - (int)(char)iVar4;
  goto LAB_140c5201f;
LAB_140c51fd0:
  iVar7 = (uint)wVar2 - (uint)wVar3;
LAB_140c5201f:
  return iVar7 == 0;
}



// ======== d3<-0x141486a60  @ 0x141478360 ========

longlong FUN_141478360(undefined8 param_1,undefined8 param_2)

{
  bool bVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  
  lVar2 = FUN_1411fc890();
  if (lVar2 != 0) {
    lVar3 = FUN_141760780();
    if ((*(int *)(*(longlong *)(lVar2 + 0x10) + 0x90) < *(int *)(lVar3 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar2 + 0x10) + 0x88) + (longlong)*(int *)(lVar3 + 0x90) * 8)
        != lVar3 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) goto LAB_1414783ba;
  }
  lVar2 = 0;
LAB_1414783ba:
  lVar2 = FUN_141478960(*(undefined8 *)(lVar2 + 0x2090),param_2);
  if (lVar2 == 0) {
    return 0;
  }
  lVar3 = lVar2 + 0x10;
  lVar4 = FUN_142058d50(lVar3);
  if (lVar4 == 0) {
    LOCK();
    if (DAT_1456e073c == 0) {
      DAT_1456e073c = 0;
    }
    UNLOCK();
    if (DAT_1456e073c == *(int *)(lVar2 + 0x18)) {
      return 0;
    }
    if (*(longlong *)(lVar2 + 0x20) == 0) {
      return 0;
    }
    lVar4 = FUN_14204ac70();
    FUN_1420575e0(lVar3,lVar4);
    if ((lVar4 != 0) || (DAT_145b9d472 == '\0')) {
      LOCK();
      if (DAT_1456e073c == 0) {
        DAT_1456e073c = 0;
      }
      UNLOCK();
      *(int *)(lVar2 + 0x18) = DAT_1456e073c;
    }
    lVar4 = FUN_142058d50(lVar3);
    if (lVar4 == 0) {
      return 0;
    }
  }
  lVar2 = FUN_140b77570();
  if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(lVar2 + 0x90)) ||
     (*(longlong *)
       (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) + (longlong)*(int *)(lVar2 + 0x90) * 8) !=
      lVar2 + 0x88)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  if (!bVar1) {
    return 0;
  }
  return lVar4;
}



// ======== d3<-0x141486a60  @ 0x141a8b460 ========

undefined4
FUN_141a8b460(longlong param_1,longlong param_2,undefined8 param_3,int param_4,int param_5,
             float param_6,undefined4 param_7,float param_8,undefined8 param_9,char param_10)

{
  undefined4 uVar1;
  undefined8 uVar2;
  
  if (param_2 == 0) {
    return 0xffffffff;
  }
  uVar1 = 0;
  if (*(longlong *)(param_2 + 0xf0) != 0) {
    if (param_4 == 2) {
      uVar2 = *(undefined8 *)(param_1 + 0xe0);
      if (param_10 != '\0') {
        uVar2 = *(undefined8 *)(param_1 + 0x100);
      }
      (*(code *)PTR_FUN_145418300)(uVar2);
      uVar1 = (*(code *)PTR_FUN_145418338)(uVar2);
      (*(code *)PTR_FUN_145418458)(uVar2,param_7);
      (*(code *)PTR_FUN_145418450)
                (uVar2,*(float *)(param_1 + 0x644) * *(float *)(param_1 + 0x638) * param_6);
      if (param_8 != 0.0) {
        (*(code *)PTR_FUN_145418468)(uVar2);
      }
      (*(code *)PTR_FUN_145418428)(uVar2,uVar1);
    }
    else if (param_4 == 3) {
      if ((-1 < param_5) && (param_5 < *(int *)(param_1 + 0x428))) {
        (*(code *)PTR_FUN_145418300)(*(undefined8 *)(param_1 + 0x3a8 + (longlong)param_5 * 8));
        uVar1 = (*(code *)PTR_FUN_145418338)
                          (*(undefined8 *)(param_1 + 0x3a8 + (longlong)param_5 * 8));
        FUN_141a87550(param_1,param_6);
        return uVar1;
      }
      return 0xffffffff;
    }
  }
  return uVar1;
}



// ======== d3<-0x141486a60  @ 0x141470a60 ========

void FUN_141470a60(longlong param_1,int param_2,int param_3,int param_4,int param_5,int param_6,
                  undefined1 param_7)

{
  int *piVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  
  if (-1 < param_2) {
    uVar2 = 0;
    piVar1 = (int *)(param_1 + 0x110);
    uVar3 = uVar2;
    while (-1 < *piVar1) {
      uVar3 = (ulonglong)((int)uVar3 + 1);
      uVar2 = uVar2 + 1;
      piVar1 = piVar1 + 0xc;
      if (0x7f < (longlong)uVar2) {
        return;
      }
    }
    piVar1 = (int *)(param_1 + 0x110 + (longlong)(int)uVar3 * 0x30);
    if (piVar1 != (int *)0x0) {
      *piVar1 = param_2;
      piVar1[3] = param_5;
      piVar1[4] = param_6;
      *(undefined1 *)(piVar1 + 10) = param_7;
      piVar1[1] = param_3;
      piVar1[2] = param_4;
      FUN_141a658b0(*(undefined8 *)(param_1 + 0x48),param_3,piVar1 + 6);
    }
  }
  return;
}



// ======== d3<-0x140d2a280  @ 0x1418fb4b0 ========

longlong FUN_1418fb4b0(void)

{
  if (DAT_145a0fa00 == 0) {
    FUN_141f291f0(L"/Script/AT",L"RideManager",&DAT_145a0fa00,FUN_1418fc480,0x1b0,0x10000004,0,
                  L"Engine",&LAB_1418fbac0,FUN_1407e4520,_guard_check_icall,&LAB_140a270a0,
                  &LAB_14083c250,0);
  }
  return DAT_145a0fa00;
}



// ======== d3<-0x140d2a280  @ 0x140cfa640 ========

int * FUN_140cfa640(longlong *param_1,int *param_2,ulonglong param_3)

{
  longlong *plVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  
  if ((int)param_1[1] != *(int *)((longlong)param_1 + 0x34)) {
    iVar4 = (int)(param_3 >> 4);
    uVar3 = 0x9e3779b9U - iVar4 ^ iVar4 << 8;
    uVar2 = -(uVar3 + iVar4) ^ uVar3 >> 0xd;
    uVar5 = (iVar4 - uVar3) - uVar2 ^ uVar2 >> 0xc;
    uVar3 = (uVar3 - uVar5) - uVar2 ^ uVar5 << 0x10;
    uVar2 = (uVar2 - uVar3) - uVar5 ^ uVar3 >> 5;
    uVar5 = (uVar5 - uVar3) - uVar2 ^ uVar2 >> 3;
    uVar3 = (uVar3 - uVar5) - uVar2 ^ uVar5 << 10;
    plVar1 = (longlong *)param_1[8];
    if ((longlong *)param_1[8] == (longlong *)0x0) {
      plVar1 = param_1 + 7;
    }
    iVar4 = *(int *)((longlong)plVar1 +
                    (((longlong)(int)((uVar2 - uVar3) - uVar5) ^ (ulonglong)(uVar3 >> 0xf)) &
                    (longlong)(int)param_1[9] - 1U) * 4);
    if (iVar4 != -1) {
      do {
        if (*(ulonglong *)(*param_1 + (longlong)iVar4 * 0x28) == param_3) {
          *param_2 = iVar4;
          return param_2;
        }
        iVar4 = *(int *)(*param_1 + 0x20 + (longlong)iVar4 * 0x28);
      } while (iVar4 != -1);
    }
  }
  *param_2 = -1;
  return param_2;
}



// ======== d3<-0x140d2a280  @ 0x141435950 ========

bool FUN_141435950(longlong param_1)

{
  return *(int *)(param_1 + 0x150) != 0;
}



// ======== d3<-0x140d2a280  @ 0x1414359a0 ========

bool FUN_1414359a0(longlong param_1)

{
  return *(int *)(param_1 + 0x150) == 3;
}



// ======== d3<-0x140d2a280  @ 0x14323a770 ========

longlong FUN_14323a770(void)

{
  if (DAT_145ca17f0 == 0) {
    FUN_141f291f0(L"/Script/Engine",L"PlayerController",&DAT_145ca17f0,FUN_14323d440,0x688,
                  0x10000004,0x2000000000,L"Game",&LAB_14323c050,FUN_1407e4520,FUN_142b0aa80,
                  FUN_143187340,&LAB_14083c250,0);
  }
  return DAT_145ca17f0;
}



// ======== d3<-0x140d2a280  @ 0x142d1f150 ========

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



// ======== d3<-0x140d2a280  @ 0x140d1b280 ========

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



// ======== d3<-0x1429e4c10  @ 0x1407f1490 ========

void FUN_1407f1490(longlong *param_1)

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
  uVar3 = FUN_141df5250(lVar4 * 4,0);
  iVar2 = (int)(uVar3 >> 2);
  if (iVar2 < (int)uVar1) {
    iVar2 = 0x7fffffff;
  }
  *(int *)((longlong)param_1 + 0xc) = iVar2;
  if ((*param_1 != 0) || (iVar2 != 0)) {
    lVar4 = FUN_141df5850(*param_1,(longlong)iVar2 << 2,0);
    *param_1 = lVar4;
  }
  return;
}



// ======== d3<-0x1429e4c10  @ 0x142058d50 ========

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



// ======== d3<-0x1429e4c10  @ 0x1428635f0 ========

undefined1 * FUN_1428635f0(undefined1 *param_1,undefined8 param_2,undefined8 param_3)

{
  *param_1 = 2;
  *param_1 = 1;
  *(undefined8 *)(param_1 + 4) = param_2;
  param_1[0xc] = 2;
  param_1[0xc] = 1;
  *(undefined8 *)(param_1 + 0x10) = param_2;
  *(undefined4 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x18) = param_3;
  *(undefined4 *)(param_1 + 0x24) = 0x80000000;
  return param_1;
}



// ======== d3<-0x1429e4c10  @ 0x1428759b0 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_1428759b0(longlong param_1,undefined8 param_2,longlong param_3,int param_4)

{
  longlong *plVar1;
  longlong lVar2;
  uint uVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined1 *puVar6;
  int iVar7;
  longlong *plVar8;
  longlong *plVar9;
  int local_res20 [2];
  undefined1 auStack_218 [32];
  longlong local_1f8;
  undefined1 *local_1e8;
  undefined4 uStack_1e0;
  longlong local_1d8;
  longlong *local_1d0;
  undefined8 local_1c8;
  undefined1 local_1c0 [16];
  undefined1 local_1b0;
  undefined1 local_1a8 [72];
  undefined1 local_160 [256];
  undefined1 *local_60;
  int iStack_58;
  ulonglong local_50;
  
  local_50 = DAT_145909780 ^ (ulonglong)auStack_218;
  local_res20[0] = param_4;
  FUN_142862090(param_1 + 0xd8,param_1 + 0x68);
  *(undefined4 *)(param_1 + 0xe0) = 0;
  if (0x7fffffff < *(uint *)(param_1 + 0xe4)) {
    FUN_140827be0(param_1 + 0xd8,0);
  }
  *(undefined4 *)(param_1 + 0xf0) = 0;
  if ((*(int *)(param_1 + 0xf4) < 0) && (*(int *)(param_1 + 0xf4) != 0)) {
    FUN_1407f1a80(param_1 + 0xe8,0);
  }
  if (*(int *)(param_1 + 0x10) != param_4) {
    FUN_14286d450(param_1,param_3);
    *(undefined4 *)(param_1 + 0x70) = 0;
    if ((*(int *)(param_1 + 0x74) < 0) && (*(int *)(param_1 + 0x74) != 0)) {
      FUN_140827be0(param_1 + 0x68,0);
    }
    *(undefined4 *)(param_1 + 0x80) = 0;
    if ((*(int *)(param_1 + 0x84) < 0) && (*(int *)(param_1 + 0x84) != 0)) {
      FUN_1407f1a80(param_1 + 0x78,0);
    }
  }
  lVar4 = FUN_14287fb30(param_1,local_res20);
  if (lVar4 == 0) {
    FUN_14286d450(param_1,param_3);
  }
  else {
    uVar5 = FUN_142862f00(local_1a8,param_2);
    lVar4 = FUN_142892c00(param_1,lVar4,param_4,uVar5);
    if (lVar4 == 0) {
      FUN_14286d450(param_1,param_3);
    }
    else {
      FUN_1428729f0(param_1,local_160);
      uStack_1e0 = 0;
      local_1e8 = local_160;
      for (iVar7 = 0; (-1 < iVar7 && (iVar7 < iStack_58)); iVar7 = iVar7 + 1) {
        puVar6 = local_60;
        if (local_60 == (undefined1 *)0x0) {
          puVar6 = local_160;
        }
        FUN_14286cbd0(param_3 + 8,puVar6 + (longlong)iVar7 * 0x20,
                      *(undefined8 *)(puVar6 + (longlong)iVar7 * 0x20 + 8),param_3);
      }
      local_1d0 = (longlong *)0x0;
      local_1c8 = 0;
      local_1d8 = param_3;
      FUN_14286d620(param_1,local_160,param_3,&local_1d8);
      FUN_142883570(param_3 + 8);
      local_1f8 = param_3;
      FUN_142876700(param_1,local_160,lVar4,param_2);
      FUN_14286c430(param_1 + 0x188,param_2,param_3);
      plVar9 = local_1d0;
      lVar2 = local_1d8;
      plVar1 = (longlong *)((longlong)local_1d0 + (longlong)(int)local_1c8 * 0xc);
      lVar4 = local_1c8;
      for (plVar8 = local_1d0; plVar8 != plVar1; plVar8 = (longlong *)((longlong)plVar8 + 0xc)) {
        local_1e8 = (undefined1 *)*plVar8;
        uStack_1e0 = (undefined4)plVar8[1];
        local_1b0 = 0;
        local_1c8 = lVar4;
        FUN_14288fc30(lVar2 + 0xf8,lVar2,&local_1e8,local_1c0);
        lVar4 = local_1c8;
      }
      local_1c8._4_4_ = (uint)((ulonglong)lVar4 >> 0x20);
      uVar3 = local_1c8._4_4_;
      if ((lVar4 < 0) &&
         (local_1c8 = (ulonglong)local_1c8._4_4_ << 0x20, lVar4 = local_1c8, uVar3 != 0)) {
        FUN_140827fb0(&local_1d0,0);
        plVar9 = local_1d0;
        lVar4 = local_1c8;
      }
      local_1c8 = lVar4;
      if (plVar9 != (longlong *)0x0) {
        FUN_141de9200(plVar9);
      }
      if (local_60 != (undefined1 *)0x0) {
        FUN_141de9200();
      }
    }
  }
  return;
}



// ======== d3<-0x1429e4c10  @ 0x14287a920 ========

void FUN_14287a920(longlong param_1,undefined8 param_2)

{
  FUN_142862090(param_1 + 0xd8,param_1 + 0x68);
  *(undefined4 *)(param_1 + 0xe0) = 0;
  if (0x7fffffff < *(uint *)(param_1 + 0xe4)) {
    FUN_140827be0(param_1 + 0xd8,0);
  }
  *(undefined4 *)(param_1 + 0xf0) = 0;
  if ((*(int *)(param_1 + 0xf4) < 0) && (*(int *)(param_1 + 0xf4) != 0)) {
    FUN_1407f1a80(param_1 + 0xe8,0);
  }
  FUN_14286d450(param_1,param_2);
  FUN_140cec6b0(param_1 + 0x18);
  return;
}



// ======== d3<-0x1429e4c10  @ 0x1409475b0 ========

void FUN_1409475b0(longlong *param_1,undefined8 param_2)

{
  longlong *plVar1;
  bool bVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  
  *(int *)((longlong)param_1 + 0x14) = *(int *)((longlong)param_1 + 0x14) + 1;
  iVar5 = *(int *)((longlong)param_1 + 0x14);
  bVar2 = false;
  iVar4 = (int)param_1[1] + -1;
  lVar7 = (longlong)iVar4;
  if (-1 < iVar4) {
    lVar6 = lVar7 << 4;
    bVar2 = false;
    do {
      if ((*(int *)(lVar6 + 8 + *param_1) == 0) ||
         (plVar1 = *(longlong **)(lVar6 + *param_1), plVar1 == (longlong *)0x0)) {
LAB_140947610:
        bVar2 = true;
      }
      else {
        cVar3 = (**(code **)(*plVar1 + 0x48))(plVar1,param_2);
        if (cVar3 == '\0') goto LAB_140947610;
      }
      lVar6 = lVar6 + -0x10;
      lVar7 = lVar7 + -1;
    } while (-1 < lVar7);
    iVar5 = *(int *)((longlong)param_1 + 0x14);
  }
  *(int *)((longlong)param_1 + 0x14) = iVar5 + -1;
  if (bVar2) {
    FUN_1407e7830(param_1,0);
    return;
  }
  return;
}



// ======== d3<-0x1429e4c10  @ 0x1408573e0 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1408573e0(longlong *param_1,undefined8 param_2)

{
  int iVar1;
  int *piVar2;
  char cVar3;
  longlong lVar4;
  longlong *plVar5;
  undefined8 uVar6;
  int iVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  undefined8 *puVar10;
  int iVar11;
  int *piVar12;
  int iVar13;
  longlong lVar14;
  int *piVar15;
  undefined1 auStack_c8 [32];
  undefined8 local_a8;
  undefined8 local_a0;
  undefined4 local_98 [16];
  undefined4 *local_58;
  int local_50;
  undefined4 local_4c;
  ulonglong local_48;
  
  local_48 = DAT_145909780 ^ (ulonglong)auStack_c8;
  iVar11 = (int)param_1[1];
  if (iVar11 < 1) {
    return;
  }
  lVar14 = *param_1;
  iVar13 = 0;
  local_58 = (undefined4 *)0x0;
  local_50 = iVar11;
  if (iVar11 == 0) {
    local_4c = 0;
    puVar9 = local_58;
  }
  else {
    FUN_140879300(local_98,iVar11,0);
    puVar10 = (undefined8 *)(lVar14 + 8);
    puVar8 = local_98;
    if (local_58 != (undefined4 *)0x0) {
      puVar8 = local_58;
    }
    do {
      *puVar8 = *(undefined4 *)(puVar10 + -1);
      puVar8[1] = *(undefined4 *)((longlong)puVar10 + -4);
      *(undefined8 *)(puVar8 + 2) = *puVar10;
      iVar11 = iVar11 + -1;
      puVar9 = local_58;
      puVar8 = puVar8 + 4;
      puVar10 = puVar10 + 2;
    } while (iVar11 != 0);
  }
  do {
    while( true ) {
      while( true ) {
        if ((iVar13 < 0) || (local_50 <= iVar13)) {
          if (puVar9 == (undefined4 *)0x0) {
            return;
          }
          FUN_141de9200();
          return;
        }
        lVar14 = (longlong)iVar13;
        local_a8 = 0;
        puVar8 = local_98;
        if (puVar9 != (undefined4 *)0x0) {
          puVar8 = puVar9;
        }
        puVar8 = puVar8 + lVar14 * 4;
        if (((*(longlong *)(puVar8 + 2) == 0) ||
            (lVar4 = FUN_142058d50(puVar8), puVar9 = local_58, lVar4 == 0)) ||
           (lVar4 = FUN_14203ed20(lVar4,*(undefined8 *)(puVar8 + 2)), puVar9 = local_58, lVar4 == 0)
           ) break;
        puVar8 = local_98;
        if (local_58 != (undefined4 *)0x0) {
          puVar8 = local_58;
        }
        plVar5 = (longlong *)FUN_142058d50(puVar8 + lVar14 * 4);
        uVar6 = FUN_14203ed30(plVar5,*(undefined8 *)(puVar8 + lVar14 * 4 + 2));
        (**(code **)(*plVar5 + 0x208))(plVar5,uVar6,param_2);
        iVar13 = iVar13 + 1;
        puVar9 = local_58;
      }
      local_a0 = 0;
      puVar8 = local_98;
      if (puVar9 != (undefined4 *)0x0) {
        puVar8 = puVar9;
      }
      if ((*(longlong *)(puVar8 + lVar14 * 4 + 2) == 0) ||
         (lVar4 = FUN_142058db0(puVar8 + lVar14 * 4,1), puVar9 = local_58, lVar4 == 0)) break;
LAB_140857654:
      iVar13 = iVar13 + 1;
    }
    piVar2 = (int *)*param_1;
    puVar8 = local_98;
    if (puVar9 != (undefined4 *)0x0) {
      puVar8 = puVar9;
    }
    piVar15 = puVar8 + lVar14 * 4;
    lVar14 = param_1[1];
    piVar12 = piVar2;
    if (piVar2 == piVar2 + (longlong)(int)lVar14 * 4) goto LAB_140857654;
    do {
      if ((((*piVar12 == *piVar15) && (piVar12[1] == piVar15[1])) ||
          ((cVar3 = FUN_14205a760(piVar12), cVar3 == '\0' &&
           (cVar3 = FUN_14205a760(piVar15), cVar3 == '\0')))) &&
         (*(longlong *)(piVar12 + 2) == *(longlong *)(piVar15 + 2))) {
        iVar11 = (int)((longlong)piVar12 - (longlong)piVar2 >> 4);
        puVar9 = local_58;
        if (iVar11 != -1) {
          iVar1 = (int)param_1[1];
          iVar7 = (iVar1 - iVar11) + -1;
          if (0 < iVar7) {
            iVar7 = 1;
          }
          if (iVar7 != 0) {
            memcpy((void *)((longlong)iVar11 * 0x10 + *param_1),
                   (void *)((longlong)(iVar1 - iVar7) * 0x10 + *param_1),(longlong)iVar7 << 4);
            iVar1 = (int)param_1[1];
          }
          *(int *)(param_1 + 1) = iVar1 + -1;
          FUN_1407f1810(param_1);
          puVar9 = local_58;
        }
        goto LAB_140857654;
      }
      piVar12 = piVar12 + 4;
    } while (piVar12 != piVar2 + (longlong)(int)lVar14 * 4);
    iVar13 = iVar13 + 1;
    puVar9 = local_58;
  } while( true );
}



// ======== d3<-0x141df5850  @ 0x141de9840 ========

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



// ======== d3<-0x1420575e0  @ 0x142038410 ========

int FUN_142038410(longlong param_1,int param_2)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  longlong lVar6;
  bool bVar7;
  
  if (param_2 < *(int *)(param_1 + 0x24)) {
    uVar4 = param_2 >> 0x1f & 0xffff;
    uVar2 = param_2 + uVar4;
    lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x10) + (longlong)((int)uVar2 >> 0x10) * 8) +
            (longlong)(int)((uVar2 & 0xffff) - uVar4) * 0x18;
  }
  else {
    lVar6 = 0;
  }
  iVar5 = *(int *)(lVar6 + 0x10);
  if (*(int *)(lVar6 + 0x10) == 0) {
    LOCK();
    piVar1 = (int *)(param_1 + 0x1a8);
    iVar5 = *piVar1;
    *piVar1 = *piVar1 + 1;
    UNLOCK();
    iVar5 = iVar5 + 1;
    if (iVar5 < 0x3e9) {
      FUN_141e49950("Unknown",0x146,
                    L"UObject serial numbers overflowed (trying to allocate serial number %d).",
                    iVar5);
      FUN_141e40cb0(L"","Unknown",0x146,
                    L"UObject serial numbers overflowed (trying to allocate serial number %d).",
                    iVar5);
    }
    LOCK();
    iVar3 = *(int *)(lVar6 + 0x10);
    bVar7 = iVar3 == 0;
    if (bVar7) {
      *(int *)(lVar6 + 0x10) = iVar5;
      iVar3 = 0;
    }
    UNLOCK();
    if (!bVar7) {
      iVar5 = iVar3;
    }
  }
  return iVar5;
}



// ======== d3<-0x1407fe110  @ 0x140b40c20 ========

void thunk_FUN_141de9200(longlong param_1)

{
  if (param_1 != 0) {
    if (DAT_145a8c1f8 == (longlong *)0x0) {
      FUN_141de9840();
    }
    (**(code **)(*DAT_145a8c1f8 + 0x20))(DAT_145a8c1f8,param_1);
  }
  return;
}



// ======== d3<-0x140d23070  @ 0x141863300 ========

longlong FUN_141863300(void)

{
  if (DAT_1459e57b8 == 0) {
    FUN_141f291f0(L"/Script/AT",L"ATPlayerController",&DAT_1459e57b8,FUN_1418646b0,0x918,0x10000004,
                  0,L"Game",&LAB_1418636f0,FUN_1407e4520,FUN_142b0aa80,FUN_141862920,&LAB_14083c250,
                  0);
  }
  return DAT_1459e57b8;
}



// ======== d3<-0x1412b5ca0  @ 0x14187c230 ========

longlong FUN_14187c230(void)

{
  if (DAT_1459ed670 == 0) {
    FUN_141f291f0(L"/Script/AT",L"ATUICommonInputComponent",&DAT_1459ed670,_guard_check_icall,600,
                  0x1000000c,0,L"Input",&LAB_14187e530,FUN_1407e4520,_guard_check_icall,
                  &LAB_1417ebdb0,&LAB_14083c250,0);
  }
  return DAT_1459ed670;
}



