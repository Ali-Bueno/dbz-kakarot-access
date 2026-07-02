// Callers of 0x1416f7bd0 (SetFontType impl)
// ---- FUN_1416d13e0 @ 1416d13e0 ----

void FUN_1416d13e0(longlong param_1)

{
  longlong *plVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  if (*(char *)(param_1 + 0x41e) == '\x02') {
    if (*(longlong *)(param_1 + 0x400) == 0) {
      lVar2 = FUN_140d2ba50();
      if (lVar2 != 0) {
        uVar3 = FUN_1415458f0(lVar2);
        *(undefined8 *)(param_1 + 0x400) = uVar3;
      }
    }
    if (*(longlong *)(param_1 + 0x400) != 0) {
      FUN_1416f7bd0(*(longlong *)(param_1 + 0x400),0x11,8,0);
    }
    plVar1 = *(longlong **)(param_1 + 0x3e0);
    if (plVar1 != (longlong *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0001416d1444. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(*plVar1 + 0x250))(plVar1,3);
      return;
    }
  }
  else {
    plVar1 = *(longlong **)(param_1 + 0x3e0);
    if (plVar1 != (longlong *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0001416d1461. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(*plVar1 + 0x250))(plVar1,1);
      return;
    }
  }
  return;
}



// ---- FUN_1411e8b70 @ 1411e8b70 ----

void FUN_1411e8b70(longlong param_1)

{
  uint *puVar1;
  longlong lVar2;
  longlong lVar3;
  longlong *plVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  uint uVar7;
  ulonglong uVar8;
  longlong *local_28;
  int local_20;
  undefined4 local_1c;
  code *local_18;
  undefined4 uStack_10;
  
  uVar6 = 0;
  if (*(char *)(param_1 + 0x7b0) == '\x01') {
    FUN_1411e6830();
  }
  else if ((byte)(*(char *)(param_1 + 0x7b0) - 2U) < 6) {
    FUN_1411e6430();
  }
  else {
    if (*(char *)(*(longlong *)(param_1 + 0x438) + 0x11c) == '\0') {
      return;
    }
    lVar2 = thunk_FUN_140d2ba50();
    if ((lVar2 != 0) && (lVar2 = FUN_1415457e0(lVar2), lVar2 != 0)) {
      local_18 = FUN_1411e6bd0;
      uStack_10 = 0;
      local_28 = (longlong *)0x0;
      local_20 = 0;
      FUN_141174140(&local_28,param_1,&local_18);
      FUN_1416409f0(lVar2,&local_28);
      local_18 = FUN_1411e3950;
      uStack_10 = 0;
      FUN_141174140(&local_28,param_1,&local_18);
      FUN_1416409a0(lVar2,&local_28);
      FUN_14163e4c0(lVar2,*(undefined8 *)(param_1 + 0x438),param_1,*(undefined8 *)(param_1 + 0x798),
                    1);
      lVar2 = FUN_140d24130();
      if (lVar2 != 0) {
        *(undefined1 *)(lVar2 + 0x339) = 0;
      }
      lVar2 = FUN_140d2a3f0();
      if (lVar2 != 0) {
        uVar5 = FUN_140d239b0();
        FUN_1412ea190(lVar2,uVar5);
      }
      if (local_20 != 0) {
        if (local_28 == (longlong *)0x0) goto LAB_1411e8ce9;
        (**(code **)(*local_28 + 0x30))(local_28,0);
        if (local_28 != (longlong *)0x0) {
          local_28 = (longlong *)FUN_141df5850(local_28,0,0);
        }
        local_20 = 0;
      }
      if (local_28 != (longlong *)0x0) {
        FUN_141de9200(local_28);
      }
    }
  }
LAB_1411e8ce9:
  lVar2 = FUN_140d28770();
  if ((lVar2 != 0) &&
     (*(uint *)(lVar2 + 0x110) = *(uint *)(lVar2 + 0x110) | 0x20, uVar8 = uVar6,
     0 < *(int *)(lVar2 + 0x108))) {
    do {
      if ((*(uint *)(*(longlong *)(lVar2 + 0x100) + 8 + uVar6) >> 5 & 1) != 0) {
        puVar1 = (uint *)(*(longlong *)(lVar2 + 0x100) + 0x10 + uVar6);
        *puVar1 = *puVar1 | 1;
        lVar3 = *(longlong *)(lVar2 + 0x100);
        if (*(int *)(lVar3 + 0x10 + uVar6) != 0) {
          plVar4 = *(longlong **)(lVar3 + uVar6);
          if (plVar4 != (longlong *)0x0) {
            (**(code **)(*plVar4 + 0x468))(plVar4,0);
            local_1c = 0x3f800000;
            lVar3 = *(longlong *)(uVar6 + *(longlong *)(lVar2 + 0x100));
            local_28 = *(longlong **)(lVar3 + 0x108);
            local_20 = *(int *)(lVar3 + 0x110);
            FUN_1429ddd00();
            lVar3 = *(longlong *)(lVar2 + 0x100);
          }
          *(undefined1 *)(lVar3 + 0xc + uVar6) = 0;
        }
      }
      uVar7 = (int)uVar8 + 1;
      uVar6 = uVar6 + 0x18;
      uVar8 = (ulonglong)uVar7;
    } while ((int)uVar7 < *(int *)(lVar2 + 0x108));
  }
  lVar2 = thunk_FUN_140d2ba50();
  if ((lVar2 != 0) && (lVar2 = FUN_1415458f0(lVar2), lVar2 != 0)) {
    FUN_1416f7bd0(lVar2,0x21,8,1);
  }
  plVar4 = (longlong *)FUN_140d283a0();
  uVar5 = (**(code **)(*plVar4 + 0x148))(plVar4);
  lVar2 = FUN_140d23070(uVar5,0);
  if (lVar2 != 0) {
    FUN_1412aec70(lVar2);
  }
  return;
}



// ---- FUN_14166d2d0 @ 14166d2d0 ----

void FUN_14166d2d0(longlong *param_1,undefined1 param_2,undefined4 *param_3,undefined8 param_4,
                  undefined8 param_5,char param_6)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined4 *puVar4;
  longlong *plVar5;
  
  *(char *)(param_1 + 0xb2) = param_6;
  if (((int)param_1[0xad] != 0) && (plVar5 = (longlong *)param_1[0xac], plVar5 != (longlong *)0x0))
  {
    (**(code **)(*plVar5 + 0x30))(plVar5,0);
    if (param_1[0xac] != 0) {
      lVar2 = FUN_141df5850(param_1[0xac],0,0);
      param_1[0xac] = lVar2;
    }
    *(undefined4 *)(param_1 + 0xad) = 0;
  }
  if (((int)param_1[0xaf] != 0) && (plVar5 = (longlong *)param_1[0xae], plVar5 != (longlong *)0x0))
  {
    (**(code **)(*plVar5 + 0x30))(plVar5,0);
    if (param_1[0xae] != 0) {
      lVar2 = FUN_141df5850(param_1[0xae],0,0);
      param_1[0xae] = lVar2;
    }
    *(undefined4 *)(param_1 + 0xaf) = 0;
  }
  FUN_1407fc730(param_1 + 0xb0,param_4);
  FUN_1407fc730(param_1 + 0xaa,param_5);
  *(undefined1 *)((longlong)param_1 + 0x591) = param_2;
  FUN_141a88520(param_1);
  FUN_14166ccf0(param_1);
  lVar2 = thunk_FUN_140d2ba50();
  if (lVar2 != 0) {
    uVar3 = FUN_1415458f0(lVar2);
    FUN_1416f7bd0(uVar3,0x22,8,0);
    uVar3 = FUN_1415458f0(lVar2);
    FUN_1416f2c70(uVar3);
  }
  FUN_141657d90(param_1,param_6 == '\x06');
  FUN_14166f500(param_1);
  lVar2 = param_1[0xb6];
  *(undefined4 *)(lVar2 + 0x350) = 0;
  if (*(int *)(lVar2 + 0x354) != 0) {
    FUN_1407f1a80(lVar2 + 0x348,0);
  }
  lVar2 = param_1[0xb6];
  plVar5 = (longlong *)(lVar2 + 0x348);
  iVar1 = FUN_140c87d90(plVar5,param_3);
  if (iVar1 == -1) {
    iVar1 = *(int *)(lVar2 + 0x350);
    *(int *)(lVar2 + 0x350) = iVar1 + 1;
    if (*(int *)(lVar2 + 0x354) < iVar1 + 1) {
      FUN_1407f1790(plVar5,iVar1);
    }
    puVar4 = (undefined4 *)((longlong)iVar1 * 0x10 + *plVar5);
    *puVar4 = *param_3;
    puVar4[1] = param_3[1];
    *(undefined8 *)(puVar4 + 2) = *(undefined8 *)(param_3 + 2);
  }
  FUN_140bdefe0(plVar5);
  (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0xb6],0,1,0,DAT_1450d6c88);
  FUN_1415c8220(param_1[0xbc]);
  return;
}



// ---- FUN_14166d4e0 @ 14166d4e0 ----

void FUN_14166d4e0(longlong *param_1,undefined1 param_2,undefined4 *param_3,longlong *param_4,
                  undefined8 param_5,undefined8 param_6)

{
  longlong *plVar1;
  int iVar2;
  longlong lVar3;
  undefined8 uVar4;
  undefined4 *puVar5;
  longlong *plVar6;
  
  *(undefined1 *)(param_1 + 0xb2) = 2;
  if (((int)param_1[0xad] != 0) && (plVar6 = (longlong *)param_1[0xac], plVar6 != (longlong *)0x0))
  {
    (**(code **)(*plVar6 + 0x30))(plVar6,0);
    if (param_1[0xac] != 0) {
      lVar3 = FUN_141df5850(param_1[0xac],0,0);
      param_1[0xac] = lVar3;
    }
    *(undefined4 *)(param_1 + 0xad) = 0;
  }
  if (((int)param_1[0xb1] != 0) && (plVar6 = (longlong *)param_1[0xb0], plVar6 != (longlong *)0x0))
  {
    (**(code **)(*plVar6 + 0x30))(plVar6,0);
    if (param_1[0xb0] != 0) {
      lVar3 = FUN_141df5850(param_1[0xb0],0,0);
      param_1[0xb0] = lVar3;
    }
    *(undefined4 *)(param_1 + 0xb1) = 0;
  }
  plVar6 = param_1 + 0xae;
  if (param_4 != plVar6) {
    if (((int)param_4[1] == 0) || (param_4 = (longlong *)*param_4, param_4 == (longlong *)0x0)) {
      if (((int)param_1[0xaf] != 0) && (plVar1 = (longlong *)*plVar6, plVar1 != (longlong *)0x0)) {
        (**(code **)(*plVar1 + 0x30))(plVar1,0);
        if (*plVar6 != 0) {
          lVar3 = FUN_141df5850(*plVar6,0,0);
          *plVar6 = lVar3;
        }
        *(undefined4 *)(param_1 + 0xaf) = 0;
      }
    }
    else {
      (**(code **)(*param_4 + 0x38))(param_4,plVar6);
    }
  }
  FUN_1407fc730(param_1 + 0xaa,param_5);
  FUN_1407fc730(param_1 + 0xa8,param_6);
  *(undefined1 *)((longlong)param_1 + 0x591) = param_2;
  FUN_141a88520(param_1);
  FUN_14166ccf0(param_1);
  lVar3 = thunk_FUN_140d2ba50();
  if (lVar3 != 0) {
    uVar4 = FUN_1415458f0(lVar3);
    FUN_1416f7bd0(uVar4,0x22,8,0);
    uVar4 = FUN_1415458f0(lVar3);
    FUN_1416f2c70(uVar4);
  }
  FUN_1416584d0(param_1);
  FUN_141670650(param_1);
  lVar3 = param_1[0xb6];
  *(undefined4 *)(lVar3 + 0x350) = 0;
  if (*(int *)(lVar3 + 0x354) != 0) {
    FUN_1407f1a80(lVar3 + 0x348,0);
  }
  lVar3 = param_1[0xb6];
  plVar6 = (longlong *)(lVar3 + 0x348);
  iVar2 = FUN_140c87d90(plVar6,param_3);
  if (iVar2 == -1) {
    iVar2 = *(int *)(lVar3 + 0x350);
    *(int *)(lVar3 + 0x350) = iVar2 + 1;
    if (*(int *)(lVar3 + 0x354) < iVar2 + 1) {
      FUN_1407f1790(plVar6,iVar2);
    }
    puVar5 = (undefined4 *)((longlong)iVar2 * 0x10 + *plVar6);
    *puVar5 = *param_3;
    puVar5[1] = param_3[1];
    *(undefined8 *)(puVar5 + 2) = *(undefined8 *)(param_3 + 2);
  }
  FUN_140bdefe0(plVar6);
  (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0xb6],0,1,0,DAT_1450d6c88);
  FUN_1415c8220(param_1[0xbc]);
  return;
}



// ---- FUN_14166db70 @ 14166db70 ----

void FUN_14166db70(longlong *param_1,undefined1 param_2,undefined4 *param_3,undefined8 param_4,
                  undefined8 param_5,undefined1 param_6)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined4 *puVar4;
  longlong *plVar5;
  
  *(undefined1 *)(param_1 + 0xb2) = param_6;
  if (((int)param_1[0xad] != 0) && (plVar5 = (longlong *)param_1[0xac], plVar5 != (longlong *)0x0))
  {
    (**(code **)(*plVar5 + 0x30))(plVar5,0);
    if (param_1[0xac] != 0) {
      lVar2 = FUN_141df5850(param_1[0xac],0,0);
      param_1[0xac] = lVar2;
    }
    *(undefined4 *)(param_1 + 0xad) = 0;
  }
  if (((int)param_1[0xaf] != 0) && (plVar5 = (longlong *)param_1[0xae], plVar5 != (longlong *)0x0))
  {
    (**(code **)(*plVar5 + 0x30))(plVar5,0);
    if (param_1[0xae] != 0) {
      lVar2 = FUN_141df5850(param_1[0xae],0,0);
      param_1[0xae] = lVar2;
    }
    *(undefined4 *)(param_1 + 0xaf) = 0;
  }
  FUN_1407fc730(param_1 + 0xb0,param_4);
  FUN_1407fc730(param_1 + 0xaa,param_5);
  *(undefined1 *)((longlong)param_1 + 0x591) = param_2;
  FUN_141a88520(param_1);
  FUN_14166ccf0(param_1);
  lVar2 = thunk_FUN_140d2ba50();
  if (lVar2 != 0) {
    uVar3 = FUN_1415458f0(lVar2);
    FUN_1416f7bd0(uVar3,0x22,8,0);
    uVar3 = FUN_1415458f0(lVar2);
    FUN_1416f2c70(uVar3);
  }
  FUN_141657d90(param_1,0);
  FUN_14166f500(param_1);
  lVar2 = param_1[0xb6];
  *(undefined4 *)(lVar2 + 0x350) = 0;
  if (*(int *)(lVar2 + 0x354) != 0) {
    FUN_1407f1a80(lVar2 + 0x348,0);
  }
  lVar2 = param_1[0xb6];
  plVar5 = (longlong *)(lVar2 + 0x348);
  iVar1 = FUN_140c87d90(plVar5,param_3);
  if (iVar1 == -1) {
    iVar1 = *(int *)(lVar2 + 0x350);
    *(int *)(lVar2 + 0x350) = iVar1 + 1;
    if (*(int *)(lVar2 + 0x354) < iVar1 + 1) {
      FUN_1407f1790(plVar5,iVar1);
    }
    puVar4 = (undefined4 *)((longlong)iVar1 * 0x10 + *plVar5);
    *puVar4 = *param_3;
    puVar4[1] = param_3[1];
    *(undefined8 *)(puVar4 + 2) = *(undefined8 *)(param_3 + 2);
  }
  FUN_140bdefe0(plVar5);
  (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0xb6],0,1,0,DAT_1450d6c88);
  FUN_1415c8220(param_1[0xbc]);
  return;
}



// ---- FUN_141613140 @ 141613140 ----

void FUN_141613140(longlong *param_1)

{
  longlong *plVar1;
  longlong *plVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *plVar5;
  
  if (((char)param_1[0x7e] == '\0') && (*(char *)((longlong)param_1 + 0x3f2) == '\0')) {
    (**(code **)(*param_1 + 0x250))(param_1,3);
    if ((param_1[0x7d] == 0) && (lVar4 = FUN_140d2ba50(), lVar4 != 0)) {
      lVar4 = FUN_1415458f0(lVar4);
      param_1[0x7d] = lVar4;
    }
    if (param_1[0x7d] != 0) {
      FUN_1416f7bd0(param_1[0x7d],6,8,0);
    }
    *(undefined2 *)((longlong)param_1 + 0x3f5) = 0;
    *(undefined1 *)(param_1 + 0x7f) = 0;
    plVar5 = (longlong *)param_1[0x79];
    plVar1 = plVar5 + (int)param_1[0x7a];
    for (; plVar5 != plVar1; plVar5 = plVar5 + 1) {
      plVar2 = (longlong *)*plVar5;
      if (((plVar2 != (longlong *)0x0) && ((char)plVar2[0x83] != '\0')) &&
         (*(char *)((longlong)plVar2 + 0x419) == '\0')) {
        plVar3 = (longlong *)plVar2[0x74];
        if (plVar3 != (longlong *)0x0) {
          (**(code **)(*plVar3 + 0x250))(plVar3,1);
        }
        (**(code **)(*plVar2 + 0x468))(plVar2,1);
      }
    }
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x7b],0,1,0,DAT_1450d6c88);
    (**(code **)(*param_1 + 0x468))(param_1,1);
    *(undefined1 *)(param_1 + 0x7e) = 1;
    *(undefined1 *)((longlong)param_1 + 0x3f2) = 1;
    lVar4 = FUN_140d2ba50();
    if (lVar4 != 0) {
      FUN_141543bf0(lVar4,0);
      return;
    }
  }
  return;
}



// ---- FUN_14163e1c0 @ 14163e1c0 ----

void FUN_14163e1c0(longlong *param_1)

{
  longlong *plVar1;
  undefined4 uVar2;
  char cVar3;
  longlong lVar4;
  undefined8 uVar5;
  uint uVar6;
  ulonglong uVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  undefined8 *puVar10;
  
  if (*(int *)((longlong)param_1 + 0x45c) != 0) {
    return;
  }
  FUN_141a88520();
  uVar2 = DAT_1450d6c88;
  FUN_141485f00(0x1d,DAT_1450d6c88,0,0);
  plVar1 = (longlong *)param_1[0x7c];
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,4);
  }
  *(undefined4 *)((longlong)param_1 + 0x45c) = 1;
  lVar4 = FUN_140d28770();
  if (lVar4 != 0) {
    FUN_1411e1dc0(lVar4,5,1);
  }
  uVar8 = 0;
  (**(code **)(*param_1 + 0x2e0))(param_1,*(undefined8 *)param_1[0x89],0,1,0,uVar2);
  if (param_1[0x7d] != 0) {
    FUN_1416f2cc0();
  }
  if (param_1[0x7e] != 0) {
    FUN_1416ca990();
  }
  if (param_1[0x7f] != 0) {
    FUN_141695b40();
  }
  lVar4 = param_1[0x82];
  if (lVar4 != 0) {
    switch((char)param_1[0x8d]) {
    case '\x02':
    case '\x03':
    case '\x04':
    case '\x0f':
      FUN_141616e20(lVar4,0);
      goto LAB_14163e378;
    default:
      cVar3 = FUN_140d2f9e0();
      if ((cVar3 == '\0') && (cVar3 = FUN_140d2f9f0(), cVar3 == '\0')) {
        uVar5 = 0;
      }
      else {
        uVar5 = 2;
      }
      lVar4 = param_1[0x82];
      break;
    case '\t':
      FUN_141617280(lVar4,0);
      FUN_141616e20(param_1[0x82],1);
      FUN_1416173d0(param_1[0x82]);
      FUN_141616e00(param_1[0x82],1);
      FUN_141617370(param_1[0x82]);
      goto LAB_14163e36c;
    case '\x0e':
      uVar5 = 1;
      break;
    case '\x10':
      uVar5 = 3;
    }
    FUN_141617280(lVar4,uVar5);
    FUN_141616e20(param_1[0x82],1);
    FUN_1416173d0(param_1[0x82]);
LAB_14163e36c:
    FUN_1416134d0(param_1[0x82]);
  }
LAB_14163e378:
  lVar4 = FUN_140d2ba50();
  if ((lVar4 == 0) || (lVar4 = FUN_1415458f0(lVar4), lVar4 == 0)) goto LAB_14163e3b9;
  cVar3 = (char)param_1[0x8d];
  if (cVar3 == '\x03') {
LAB_14163e3a9:
    uVar5 = 0xd;
  }
  else {
    if (cVar3 != '\v') {
      if (cVar3 != '\x0f') goto LAB_14163e3b9;
      goto LAB_14163e3a9;
    }
    uVar5 = 0x22;
  }
  FUN_1416f7bd0(lVar4,uVar5,8,1);
LAB_14163e3b9:
  uVar7 = uVar8;
  uVar9 = uVar8;
  if (0 < *(int *)((longlong)param_1 + 0x464)) {
    do {
      FUN_14171f270(*(undefined8 *)(param_1[0x87] + uVar9),0);
      uVar6 = (int)uVar7 + 1;
      uVar7 = (ulonglong)uVar6;
      uVar9 = uVar9 + 8;
    } while ((int)uVar6 < *(int *)((longlong)param_1 + 0x464));
  }
  puVar10 = (undefined8 *)param_1[0x87];
  uVar7 = (longlong)(int)param_1[0x88] & 0x1fffffffffffffff;
  if (puVar10 + (int)param_1[0x88] < puVar10) {
    uVar7 = uVar8;
  }
  if (uVar7 != 0) {
    do {
      plVar1 = (longlong *)*puVar10;
      (**(code **)(*plVar1 + 0x470))(plVar1,param_1,DAT_145984708);
      (**(code **)(*plVar1 + 0x498))(plVar1,param_1,DAT_145984748);
      uVar8 = uVar8 + 1;
      puVar10 = puVar10 + 1;
    } while (uVar8 != uVar7);
  }
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | 8;
  return;
}



// ---- FUN_141667620 @ 141667620 ----

void FUN_141667620(longlong *param_1,undefined1 param_2,undefined4 *param_3,undefined8 param_4,
                  undefined8 param_5)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  undefined8 uVar4;
  undefined4 *puVar5;
  longlong *plVar6;
  
  *(undefined1 *)(param_1 + 0xb2) = 1;
  FUN_1407fc730(param_1 + 0xac,param_4);
  if (((int)param_1[0xaf] != 0) && (plVar6 = (longlong *)param_1[0xae], plVar6 != (longlong *)0x0))
  {
    (**(code **)(*plVar6 + 0x30))(plVar6,0);
    if (param_1[0xae] != 0) {
      lVar3 = FUN_141df5850(param_1[0xae],0,0);
      param_1[0xae] = lVar3;
    }
    *(undefined4 *)(param_1 + 0xaf) = 0;
  }
  if (((int)param_1[0xb1] != 0) && (plVar6 = (longlong *)param_1[0xb0], plVar6 != (longlong *)0x0))
  {
    (**(code **)(*plVar6 + 0x30))(plVar6,0);
    if (param_1[0xb0] != 0) {
      lVar3 = FUN_141df5850(param_1[0xb0],0,0);
      param_1[0xb0] = lVar3;
    }
    *(undefined4 *)(param_1 + 0xb1) = 0;
  }
  FUN_1407fc730(param_1 + 0xaa,param_5);
  *(undefined1 *)((longlong)param_1 + 0x591) = param_2;
  FUN_141a88520(param_1);
  FUN_14166ccf0(param_1);
  lVar3 = thunk_FUN_140d2ba50();
  if (lVar3 != 0) {
    uVar4 = FUN_1415458f0(lVar3);
    FUN_1416f7bd0(uVar4,0x22,8,0);
    uVar4 = FUN_1415458f0(lVar3);
    FUN_1416f2c70(uVar4);
  }
  FUN_1416594c0(param_1);
  cVar1 = (char)param_1[0xb2];
  if (cVar1 == '\x01') {
    FUN_1416714d0(param_1);
  }
  else if (cVar1 == '\x02') {
    FUN_141670650(param_1);
  }
  else if ((byte)(cVar1 - 3U) < 7) {
    FUN_14166f500(param_1);
  }
  lVar3 = param_1[0xb6];
  *(undefined4 *)(lVar3 + 0x350) = 0;
  if (*(int *)(lVar3 + 0x354) != 0) {
    FUN_1407f1a80(lVar3 + 0x348,0);
  }
  lVar3 = param_1[0xb6];
  plVar6 = (longlong *)(lVar3 + 0x348);
  iVar2 = FUN_140c87d90(plVar6,param_3);
  if (iVar2 == -1) {
    iVar2 = *(int *)(lVar3 + 0x350);
    *(int *)(lVar3 + 0x350) = iVar2 + 1;
    if (*(int *)(lVar3 + 0x354) < iVar2 + 1) {
      FUN_1407f1790(plVar6,iVar2);
    }
    puVar5 = (undefined4 *)((longlong)iVar2 * 0x10 + *plVar6);
    *puVar5 = *param_3;
    puVar5[1] = param_3[1];
    *(undefined8 *)(puVar5 + 2) = *(undefined8 *)(param_3 + 2);
  }
  FUN_140bdefe0(plVar6);
  (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0xb6],0,1,0,DAT_1450d6c88);
  FUN_1415c8220(param_1[0xbc]);
  FUN_1407e59d0(param_4);
  FUN_1407e59d0(param_5);
  return;
}



// ---- FUN_141657610 @ 141657610 ----

void FUN_141657610(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  code *local_18;
  undefined4 uStack_10;
  
  FUN_141a88520();
  uVar2 = FUN_140d283a0();
  uVar2 = FUN_140d2ba00(uVar2);
  local_18 = FUN_14165d720;
  uStack_10 = 0;
  FUN_1410e5610(uVar2,param_1,&local_18,0x17);
  local_18 = FUN_14165d0f0;
  uStack_10 = 0;
  FUN_1410e5610(uVar2,param_1,&local_18,0x18);
  local_18 = FUN_14165ea40;
  uStack_10 = 0;
  FUN_1410e5610(uVar2,param_1,&local_18,9);
  local_18 = FUN_14165e0b0;
  uStack_10 = 0;
  FUN_1410e5610(uVar2,param_1,&local_18,10);
  local_18 = FUN_14165e2c0;
  uStack_10 = 0;
  FUN_1410e5610(uVar2,param_1,&local_18,0xc);
  local_18 = FUN_14165e870;
  uStack_10 = 0;
  FUN_1410e5610(uVar2,param_1,&local_18,0xb);
  if (*(char *)(param_1 + 0x520) == '\x03') {
    local_18 = FUN_14165ec00;
    uStack_10 = 0;
    FUN_1410e5610(uVar2,param_1,&local_18,3);
  }
  local_18 = FUN_14165d0f0;
  uStack_10 = 0;
  FUN_1410e5610(uVar2,param_1,&local_18,0x19);
  FUN_1414cb6d0(uVar2,param_1);
  lVar3 = thunk_FUN_140d2ba50();
  if (lVar3 != 0) {
    uVar2 = FUN_1415458f0(lVar3);
    FUN_1416f7bd0(uVar2,0x22,8,0);
    uVar2 = FUN_1415458f0(lVar3);
    FUN_1416f2c70(uVar2);
  }
  cVar1 = *(char *)(param_1 + 0x520);
  if (cVar1 == '\x01') {
    FUN_14166eca0(param_1);
  }
  else if (cVar1 == '\x02') {
    FUN_1416703b0(param_1);
  }
  else if (cVar1 == '\x03') {
    FUN_141671230(param_1);
  }
  return;
}



// ---- FUN_140fca0e0 @ 140fca0e0 ----

void FUN_140fca0e0(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  
  if (*(char *)(param_1 + 0xaa) == '\0') {
    FUN_140fda990(param_1,DAT_14594bb70,1);
    if (*(longlong *)(param_1 + 0xa0) == 0) {
      lVar1 = FUN_140d2ba50();
      if (lVar1 != 0) {
        uVar2 = FUN_1415458f0(lVar1);
        *(undefined8 *)(param_1 + 0xa0) = uVar2;
      }
    }
    if (*(longlong *)(param_1 + 0xa0) != 0) {
      FUN_1416f7bd0(*(longlong *)(param_1 + 0xa0),0xb,8,0);
    }
    *(undefined2 *)(param_1 + 0xaa) = 1;
  }
  return;
}



// ---- FUN_14163bd10 @ 14163bd10 ----

void FUN_14163bd10(longlong *param_1)

{
  char cVar1;
  longlong lVar2;
  
  if ((char)param_1[0x88] == '\0') {
    if (param_1[0x7b] == 0) {
      if (*(char *)((longlong)param_1 + 0x441) != '\0') {
        return;
      }
    }
    else {
      if (*(char *)((longlong)param_1 + 0x441) != '\0') {
        return;
      }
      cVar1 = FUN_1416ee150();
      if (cVar1 == '\0') {
        return;
      }
    }
    *(undefined1 *)((longlong)param_1 + 0x454) = 1;
    FUN_141443b50();
    if ((param_1[0x84] == 0) && (lVar2 = FUN_140d2ba50(), lVar2 != 0)) {
      lVar2 = FUN_1415458f0(lVar2);
      param_1[0x84] = lVar2;
    }
    if (param_1[0x84] != 0) {
      FUN_1416f7bd0(param_1[0x84],0xb,8,0);
    }
    if (param_1[0x7d] != 0) {
      FUN_1415981e0();
    }
    FUN_141643ff0(param_1);
    FUN_14164a4f0(param_1);
    FUN_141645b70(param_1);
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x82],0,1,0,DAT_1450d6c88);
    (**(code **)(*param_1 + 0x468))(param_1,1);
    param_1[0x8c] = 0;
    *(undefined2 *)(param_1 + 0x88) = 0x101;
    *(undefined1 *)(param_1 + 0x8b) = 0;
  }
  return;
}



// ---- FUN_1415c7ea0 @ 1415c7ea0 ----

void FUN_1415c7ea0(longlong *param_1,undefined8 param_2,undefined8 param_3)

{
  bool bVar1;
  undefined2 *puVar2;
  int iVar3;
  longlong lVar4;
  longlong lVar5;
  undefined8 uVar6;
  longlong *plVar7;
  undefined2 *puVar8;
  undefined2 *local_28;
  undefined8 local_20;
  longlong *local_18;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  FUN_140d1a360(4);
  lVar4 = FUN_140d2a280();
  if (lVar4 != 0) {
    lVar5 = FUN_141862920();
    if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) + (longlong)*(int *)(lVar5 + 0x90) * 8)
        != lVar5 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) {
      FUN_1412daf50(lVar4,0x400);
    }
  }
  FUN_141476bb0(1);
  iVar3 = FUN_1414792f0();
  if (iVar3 == 1) {
    FUN_1414837d0(5);
    FUN_1414837d0(3);
  }
  FUN_14147fc20(0xb);
  local_28 = (undefined2 *)0x0;
  local_20 = 0x1e;
  FUN_1407e2ee0(&local_28,0);
  puVar2 = local_28;
  FUN_1407e4a20(local_28,0x1e,"UAT_UIMapWorld::RequestPlayIn",0x1e,0x3f);
  puVar8 = &DAT_14391f7d0;
  if ((int)local_20 != 0) {
    puVar8 = puVar2;
  }
  FUN_141dbbf60(&local_18,&DAT_143eb3ca8,puVar8,0x96c);
  if (puVar2 != (undefined2 *)0x0) {
    FUN_141de9200(puVar2);
  }
  FUN_141483330(&local_18,0xb,0x9f);
  FUN_141483be0(0xb,0x9f,0);
  if (local_18 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_1407fc730(param_1 + 0xe0,param_2);
  FUN_1407fc730(param_1 + 0xe4,param_3);
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | 8;
  *(undefined1 *)(param_1 + 0xa2) = 0;
  *(undefined4 *)((longlong)param_1 + 0x514) = 0xffffffff;
  param_1[0xa1] = 1;
  lVar4 = FUN_14298e4d0(param_1[0x8c],param_1[0x8d]);
  if (lVar4 != 0) {
    local_18 = *(longlong **)((longlong)param_1 + 0x524);
    uStack_10 = *(undefined4 *)((longlong)param_1 + 0x52c);
    uStack_c = (undefined4)param_1[0xa6];
    FUN_1429e0f30(lVar4,&local_18);
  }
  FUN_1415c24b0(param_1);
  uVar6 = (**(code **)(*param_1 + 0x148))(param_1);
  lVar4 = FUN_140d23070(uVar6,0);
  if (lVar4 != 0) {
    FUN_1412c7360(lVar4,0xf,1);
  }
  *(undefined4 *)(param_1 + 0xa7) = 0x44700000;
  *(undefined4 *)((longlong)param_1 + 0x53c) = 0x44070000;
  if (param_1[0x93] != 0) {
    FUN_1429e1d30(param_1[0x93],param_1[0xa7]);
  }
  if (param_1[0x94] != 0) {
    FUN_1429e1d30(param_1[0x94],param_1[0xa7]);
  }
  plVar7 = (longlong *)param_1[0x8f];
  if (plVar7 != (longlong *)0x0) {
    (**(code **)(*plVar7 + 0x250))(plVar7,3);
  }
  lVar4 = thunk_FUN_140d2ba50();
  if (lVar4 != 0) {
    lVar5 = FUN_1415458a0(lVar4);
    param_1[0x99] = lVar5;
    lVar4 = FUN_1415458f0(lVar4);
    if (lVar4 != 0) {
      FUN_1416f7bd0(lVar4,8,8,1);
    }
  }
  lVar4 = FUN_1411fc890();
  *(undefined1 *)(*(longlong *)(lVar4 + 0x1ff8) + 0x28) = 0xf;
  if (param_1[0x8b] != 0) {
    plVar7 = (longlong *)FUN_141df5850(0,0x20);
    uStack_10 = 2;
    local_18 = plVar7;
    if (plVar7 != (longlong *)0x0) {
      plVar7[1] = (longlong)param_1;
      *plVar7 = (longlong)&PTR_FUN_143ec1c58;
      lVar4 = FUN_141db8700();
      plVar7[3] = lVar4;
      *plVar7 = (longlong)&PTR_FUN_143ec1ca8;
    }
    FUN_1415c7390(param_1[0x8b],&local_18);
    if (plVar7 != (longlong *)0x0) {
      (**(code **)(*plVar7 + 0x30))(plVar7,0);
      lVar4 = FUN_141df5850(plVar7,0,0);
      if (lVar4 != 0) {
        FUN_141de9200(lVar4);
      }
    }
  }
  FUN_1407e59d0(param_2);
  FUN_1407e59d0(param_3);
  return;
}



// ---- FUN_141525270 @ 141525270 ----

void FUN_141525270(longlong *param_1,char param_2)

{
  char cVar1;
  longlong lVar2;
  undefined1 local_res8 [8];
  
  (**(code **)(*param_1 + 0x250))(param_1,3);
  *(char *)((longlong)param_1 + 0x769) = param_2;
  if (param_2 == '\x02') {
    lVar2 = FUN_140d2a6d0();
    if (lVar2 != 0) {
      FUN_141eb3410(local_res8,"C01_EVT_MAIN_01003",1);
      cVar1 = FUN_1412e0e70(lVar2,local_res8);
      if (cVar1 == '\0') {
        *(undefined1 *)((longlong)param_1 + 0x769) = 2;
      }
    }
  }
  lVar2 = FUN_140d2ba50();
  if (lVar2 != 0) {
    lVar2 = FUN_1415458f0(lVar2);
    if (lVar2 != 0) {
      FUN_1416f7bd0(lVar2,0xd,8,1);
    }
  }
  *(undefined4 *)(param_1 + 0x75) = 1;
  param_1[0xc0] = 0;
  param_1[0xc1] = 0;
  FUN_141a6eae0(param_1,&DAT_145442610,0);
  if ((byte)(*(char *)((longlong)param_1 + 0x769) - 4U) < 2) {
LAB_141525376:
    FUN_141518890(param_1);
  }
  else {
    cVar1 = FUN_140d2f9e0();
    if (cVar1 == '\0') {
      cVar1 = FUN_140d2f9f0();
      if (cVar1 != '\0') goto LAB_141525344;
    }
    else {
LAB_141525344:
      if (*(char *)((longlong)param_1 + 0x769) == '\x06') goto LAB_141525376;
    }
    if (param_1[0xc3] == 0) {
      *(undefined4 *)(param_1 + 0xd2) = 1;
      (**(code **)(*param_1 + 0x4a8))(param_1);
    }
    else {
      FUN_141518ad0();
    }
  }
  if (param_1[0xa3] != 0) {
    FUN_1416f2cc0();
  }
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | 8;
  *(undefined1 *)((longlong)param_1 + 0x66d) = 0;
  *(undefined1 *)((longlong)param_1 + 0x76a) = 0;
  return;
}



// ---- FUN_1415ed270 @ 1415ed270 ----

void FUN_1415ed270(longlong *param_1,char param_2,char param_3)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  
  FUN_141a88520();
  FUN_1415f4ee0(param_1);
  (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x84],0,1,0,DAT_1450d6c88);
  *(undefined4 *)((longlong)param_1 + 0x414) = 0;
  FUN_1416f37c0(param_1[0x78]);
  FUN_1416f37c0(param_1[0x77]);
  *(char *)((longlong)param_1 + 0x41c) = param_2;
  if (param_2 == '\0') {
    return;
  }
  lVar1 = thunk_FUN_140d2ba50();
  if (lVar1 != 0) {
    if (param_3 == '\x02') {
      uVar2 = FUN_1415458f0(lVar1);
      uVar3 = 0x16;
    }
    else {
      if (param_3 != '\x03') goto LAB_1415ed343;
      uVar2 = FUN_1415458f0(lVar1);
      uVar3 = 0x17;
    }
    FUN_1416f7bd0(uVar2,uVar3,8,0);
    uVar2 = FUN_1415458f0(lVar1);
    FUN_1416f2c70(uVar2);
  }
LAB_1415ed343:
  if (param_1[0x7c] != 0) {
    FUN_1416f2cc0();
  }
  return;
}



// ---- FUN_1415ed370 @ 1415ed370 ----

void FUN_1415ed370(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined1 uVar3;
  
  lVar1 = thunk_FUN_140d2ba50();
  if (lVar1 != 0) {
    if (*(char *)(param_1 + 0x420) == '\b') {
      uVar2 = FUN_1415458f0(lVar1);
      uVar3 = 0x16;
    }
    else {
      if (*(char *)(param_1 + 0x420) != '\t') goto LAB_1415ed3d2;
      uVar2 = FUN_1415458f0(lVar1);
      uVar3 = 0x17;
    }
    FUN_1416f7bd0(uVar2,uVar3,8,0);
    uVar2 = FUN_1415458f0(lVar1);
    FUN_1416f2c70(uVar2);
  }
LAB_1415ed3d2:
  if (*(longlong *)(param_1 + 0x3a0) != 0) {
    FUN_1416f2cc0();
  }
  if (*(longlong *)(param_1 + 0x3a8) != 0) {
    FUN_1416ca990();
  }
  return;
}



// ---- FUN_141178300 @ 141178300 ----

void FUN_141178300(longlong *param_1,int *param_2)

{
  undefined1 uVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  undefined4 uVar7;
  undefined8 *local_48;
  undefined4 local_40;
  undefined8 local_38;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  
  uVar7 = 1;
  if (*param_2 == 0) {
    uVar7 = *(undefined4 *)(*(longlong *)(param_2 + 2) + (longlong)param_2[6] * 4);
  }
  else if (*param_2 == 1) {
    uVar7 = *(undefined4 *)(*(longlong *)(param_2 + 10) + (longlong)param_2[9] * 8);
  }
  uVar1 = *(undefined1 *)((longlong)param_1 + 0xc);
  lVar3 = param_1[1];
  lVar2 = *param_1;
  lVar4 = thunk_FUN_140d2ba50();
  if (lVar4 != 0) {
    uVar5 = FUN_1415458f0(lVar4);
    FUN_1416f7bd0(uVar5,0xf,8,0);
  }
  lVar4 = FUN_140d2aef0();
  if ((lVar4 != 0) && (*(longlong *)(lVar4 + 0x130) != 0)) {
    FUN_14144c490(*(longlong *)(lVar4 + 0x130),uVar7);
  }
  FUN_14165b960(*(undefined8 *)(lVar2 + 0x138),uVar7,0);
  local_48 = (undefined8 *)0x0;
  local_40 = 0;
  puVar6 = (undefined8 *)FUN_141df5850(0,0x20,0);
  local_40 = 2;
  local_48 = puVar6;
  if (puVar6 != (undefined8 *)0x0) {
    *puVar6 = &PTR__purecall_143a2de70;
    *puVar6 = &PTR_FUN_143d03cd8;
    puVar6[1] = lVar2;
    uVar5 = FUN_141db8700();
    puVar6[3] = uVar5;
    *puVar6 = &PTR_FUN_143d03d28;
  }
  FUN_14166c240(*(undefined8 *)(lVar2 + 0x138),&local_48);
  uStack_2c = CONCAT31(uStack_2c._1_3_,uVar1);
  local_48 = (undefined8 *)0x0;
  local_40 = 0;
  local_38 = lVar2;
  uStack_30 = (int)lVar3;
  puVar6 = (undefined8 *)FUN_141df5850(0,0x30);
  local_40 = 3;
  local_48 = puVar6;
  if (puVar6 != (undefined8 *)0x0) {
    *puVar6 = &PTR__purecall_143a2de70;
    *puVar6 = &PTR_FUN_143d03d78;
    *(undefined4 *)(puVar6 + 1) = (undefined4)local_38;
    *(undefined4 *)((longlong)puVar6 + 0xc) = local_38._4_4_;
    *(undefined4 *)(puVar6 + 2) = uStack_30;
    *(undefined4 *)((longlong)puVar6 + 0x14) = uStack_2c;
    uVar5 = FUN_141db8700();
    puVar6[4] = uVar5;
    *puVar6 = &PTR_FUN_143d03dc8;
  }
  FUN_14166c210(*(undefined8 *)(lVar2 + 0x138),&local_48);
  FUN_141177930(*param_1 + 0x2a8,param_2);
  *(undefined1 *)(*param_1 + 0x2e8) = 1;
  if (*(longlong *)(param_2 + 10) != 0) {
    FUN_141de9200();
  }
  if (*(longlong *)(param_2 + 2) != 0) {
    FUN_141de9200();
  }
  return;
}



// ---- FUN_14118f940 @ 14118f940 ----

void FUN_14118f940(longlong param_1)

{
  byte bVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 uVar6;
  int iVar7;
  longlong *plVar8;
  longlong lVar9;
  undefined8 *puVar10;
  undefined8 uVar11;
  longlong lVar12;
  wchar_t *pwVar13;
  longlong *plVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined1 local_res8 [8];
  undefined8 local_38;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  float local_28;
  float fStack_24;
  float fStack_20;
  float fStack_1c;
  
  if (*(char *)(param_1 + 0x33e) == '\n') {
    plVar8 = (longlong *)FUN_141634fb0();
  }
  else {
    plVar8 = (longlong *)FUN_141634fd0();
  }
  *(longlong **)(param_1 + 0x128) = plVar8;
  (**(code **)(*plVar8 + 0x4d8))(*(undefined8 *)(param_1 + 0x128),*(undefined1 *)(param_1 + 0x33e));
  FUN_14163e1c0(*(undefined8 *)(param_1 + 0x128));
  FUN_140804ba0(*(longlong *)(param_1 + 0x128) + 0x398);
  if (*(longlong *)(param_1 + 0x120) == 0) {
    FUN_14136f480(*(undefined8 *)(param_1 + 0xf0));
    goto LAB_14118fca1;
  }
  bVar1 = *(byte *)(param_1 + 0x33e);
  if (bVar1 == 10) {
    lVar12 = *(longlong *)(param_1 + 0xf0);
    *(undefined1 *)(param_1 + 0x3f4) = 1;
    lVar9 = *(longlong *)(lVar12 + 0x168);
    if (lVar9 == 0) {
      uVar15 = (undefined4)DAT_145a8af20;
      uVar16 = (undefined4)((ulonglong)DAT_145a8af20 >> 0x20);
      uStack_30 = DAT_145a8af28;
    }
    else {
      local_28 = *(float *)(lVar9 + 400);
      fStack_24 = *(float *)(lVar9 + 0x194);
      fStack_20 = *(float *)(lVar9 + 0x198);
      fStack_1c = *(float *)(lVar9 + 0x19c);
      auVar2._4_4_ = -(uint)(*(float *)(lVar9 + 0x1d4) != fStack_24);
      auVar2._0_4_ = -(uint)(*(float *)(lVar9 + 0x1d0) != local_28);
      auVar2._8_4_ = -(uint)(*(float *)(lVar9 + 0x1d8) != fStack_20);
      auVar2._12_4_ = -(uint)(*(float *)(lVar9 + 0x1dc) != fStack_1c);
      iVar7 = movmskps(10,auVar2);
      if (iVar7 != 0) {
        *(float *)(lVar9 + 0x1d0) = local_28;
        *(float *)(lVar9 + 0x1d4) = fStack_24;
        *(float *)(lVar9 + 0x1d8) = fStack_20;
        *(float *)(lVar9 + 0x1dc) = fStack_1c;
        puVar10 = (undefined8 *)FUN_141e4e960(&local_28,&local_38);
        *(undefined8 *)(lVar9 + 0x1e0) = *puVar10;
        *(undefined4 *)(lVar9 + 0x1e8) = *(undefined4 *)(puVar10 + 1);
        lVar12 = *(longlong *)(param_1 + 0xf0);
      }
      uVar15 = (undefined4)*(undefined8 *)(lVar9 + 0x1e0);
      uVar16 = (undefined4)((ulonglong)*(undefined8 *)(lVar9 + 0x1e0) >> 0x20);
      uStack_30 = *(undefined4 *)(lVar9 + 0x1e8);
    }
    local_38 = (code *)CONCAT44(uVar16,uVar15);
    *(code **)(param_1 + 1000) = local_38;
    *(undefined4 *)(param_1 + 0x3f0) = uStack_30;
    FUN_14136fb10(lVar12);
    goto LAB_14118fca1;
  }
  if (bVar1 == 6) {
    lVar12 = *(longlong *)(param_1 + 0xf0);
    *(undefined1 *)(param_1 + 0x3f4) = 0;
LAB_14118fc9c:
    FUN_14136f7c0(lVar12);
  }
  else {
    *(undefined1 *)(param_1 + 0x3f4) = 1;
    uVar15 = (undefined4)DAT_145a8af20;
    uVar16 = (undefined4)((ulonglong)DAT_145a8af20 >> 0x20);
    uStack_30 = DAT_145a8af28;
    if (bVar1 == 0xe) {
      lVar12 = *(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x168);
      if (lVar12 == 0) {
        pwVar13 = L"DLC_03_Researcher_Shop";
      }
      else {
        local_28 = *(float *)(lVar12 + 400);
        fStack_24 = *(float *)(lVar12 + 0x194);
        fStack_20 = *(float *)(lVar12 + 0x198);
        fStack_1c = *(float *)(lVar12 + 0x19c);
        auVar3._4_4_ = -(uint)(*(float *)(lVar12 + 0x1d4) != fStack_24);
        auVar3._0_4_ = -(uint)(*(float *)(lVar12 + 0x1d0) != local_28);
        auVar3._8_4_ = -(uint)(*(float *)(lVar12 + 0x1d8) != fStack_20);
        auVar3._12_4_ = -(uint)(*(float *)(lVar12 + 0x1dc) != fStack_1c);
        iVar7 = movmskps((int)*(longlong *)(param_1 + 0xf0),auVar3);
        if (iVar7 != 0) {
          *(float *)(lVar12 + 0x1d0) = local_28;
          *(float *)(lVar12 + 0x1d4) = fStack_24;
          *(float *)(lVar12 + 0x1d8) = fStack_20;
          *(float *)(lVar12 + 0x1dc) = fStack_1c;
          puVar10 = (undefined8 *)FUN_141e4e960(&local_28,&local_38);
          *(undefined8 *)(lVar12 + 0x1e0) = *puVar10;
          *(undefined4 *)(lVar12 + 0x1e8) = *(undefined4 *)(puVar10 + 1);
        }
        uVar15 = (undefined4)*(undefined8 *)(lVar12 + 0x1e0);
        uVar16 = (undefined4)((ulonglong)*(undefined8 *)(lVar12 + 0x1e0) >> 0x20);
        pwVar13 = L"DLC_03_Researcher_Shop";
        uStack_30 = *(undefined4 *)(lVar12 + 0x1e8);
      }
    }
    else {
      lVar12 = *(longlong *)(param_1 + 0xf0);
      lVar9 = *(longlong *)(lVar12 + 0x168);
      if (bVar1 != 0x10) {
        if (lVar9 != 0) {
          local_28 = *(float *)(lVar9 + 400);
          fStack_24 = *(float *)(lVar9 + 0x194);
          fStack_20 = *(float *)(lVar9 + 0x198);
          fStack_1c = *(float *)(lVar9 + 0x19c);
          auVar5._4_4_ = -(uint)(*(float *)(lVar9 + 0x1d4) != fStack_24);
          auVar5._0_4_ = -(uint)(*(float *)(lVar9 + 0x1d0) != local_28);
          auVar5._8_4_ = -(uint)(*(float *)(lVar9 + 0x1d8) != fStack_20);
          auVar5._12_4_ = -(uint)(*(float *)(lVar9 + 0x1dc) != fStack_1c);
          iVar7 = movmskps((uint)bVar1,auVar5);
          if (iVar7 != 0) {
            *(float *)(lVar9 + 0x1d0) = local_28;
            *(float *)(lVar9 + 0x1d4) = fStack_24;
            *(float *)(lVar9 + 0x1d8) = fStack_20;
            *(float *)(lVar9 + 0x1dc) = fStack_1c;
            puVar10 = (undefined8 *)FUN_141e4e960(&local_28,&local_38);
            *(undefined8 *)(lVar9 + 0x1e0) = *puVar10;
            *(undefined4 *)(lVar9 + 0x1e8) = *(undefined4 *)(puVar10 + 1);
            lVar12 = *(longlong *)(param_1 + 0xf0);
          }
          uVar15 = (undefined4)*(undefined8 *)(lVar9 + 0x1e0);
          uVar16 = (undefined4)((ulonglong)*(undefined8 *)(lVar9 + 0x1e0) >> 0x20);
          uStack_30 = *(undefined4 *)(lVar9 + 0x1e8);
        }
        local_38 = (code *)CONCAT44(uVar16,uVar15);
        *(code **)(param_1 + 1000) = local_38;
        *(undefined4 *)(param_1 + 0x3f0) = uStack_30;
        goto LAB_14118fc9c;
      }
      if (lVar9 != 0) {
        local_28 = *(float *)(lVar9 + 400);
        fStack_24 = *(float *)(lVar9 + 0x194);
        fStack_20 = *(float *)(lVar9 + 0x198);
        fStack_1c = *(float *)(lVar9 + 0x19c);
        auVar4._4_4_ = -(uint)(*(float *)(lVar9 + 0x1d4) != fStack_24);
        auVar4._0_4_ = -(uint)(*(float *)(lVar9 + 0x1d0) != local_28);
        auVar4._8_4_ = -(uint)(*(float *)(lVar9 + 0x1d8) != fStack_20);
        auVar4._12_4_ = -(uint)(*(float *)(lVar9 + 0x1dc) != fStack_1c);
        iVar7 = movmskps(0x10,auVar4);
        if (iVar7 != 0) {
          *(float *)(lVar9 + 0x1d0) = local_28;
          *(float *)(lVar9 + 0x1d4) = fStack_24;
          *(float *)(lVar9 + 0x1d8) = fStack_20;
          *(float *)(lVar9 + 0x1dc) = fStack_1c;
          puVar10 = (undefined8 *)FUN_141e4e960(&local_28,&local_38);
          *(undefined8 *)(lVar9 + 0x1e0) = *puVar10;
          *(undefined4 *)(lVar9 + 0x1e8) = *(undefined4 *)(puVar10 + 1);
        }
        uVar15 = (undefined4)*(undefined8 *)(lVar9 + 0x1e0);
        uVar16 = (undefined4)((ulonglong)*(undefined8 *)(lVar9 + 0x1e0) >> 0x20);
        uStack_30 = *(undefined4 *)(lVar9 + 0x1e8);
      }
      pwVar13 = L"DLC_07_VolcanrockTrader";
    }
    local_38 = (code *)CONCAT44(uVar16,uVar15);
    *(code **)(param_1 + 1000) = local_38;
    *(undefined4 *)(param_1 + 0x3f0) = uStack_30;
    FUN_141eb3490(local_res8,pwVar13,1);
    FUN_14136f580(*(undefined8 *)(param_1 + 0xf0),local_res8,1);
  }
LAB_14118fca1:
  FUN_140804ba0(*(longlong *)(param_1 + 0x128) + 0x3c8);
  lVar12 = *(longlong *)(param_1 + 0x128);
  plVar14 = (longlong *)(lVar12 + 0x3c8);
  plVar8 = (longlong *)FUN_141df5850(0,0x20);
  if (plVar8 != (longlong *)0x0) {
    plVar8[1] = param_1;
    *plVar8 = (longlong)&PTR_FUN_143d04bb0;
    lVar9 = FUN_141db8700();
    plVar8[3] = lVar9;
    *plVar8 = (longlong)&PTR_FUN_143d04c00;
    FUN_1407e7830(plVar14,1);
    (**(code **)(*plVar8 + 0x28))(plVar8,local_res8);
    iVar7 = *(int *)(lVar12 + 0x3d0);
    *(int *)(lVar12 + 0x3d0) = iVar7 + 1;
    if (*(int *)(lVar12 + 0x3d4) < iVar7 + 1) {
      FUN_1407f1790(plVar14,iVar7);
    }
    puVar10 = (undefined8 *)((longlong)iVar7 * 0x10 + *plVar14);
    *puVar10 = plVar8;
    *(undefined4 *)(puVar10 + 1) = 2;
  }
  uVar11 = FUN_140d283a0();
  lVar12 = FUN_140d2ba00(uVar11);
  local_38 = (code *)&LAB_141189bb0;
  uStack_30 = 0;
  local_28 = 9.53801;
  fStack_24 = 1.4013e-45;
  fStack_20 = 0.0;
  fStack_1c = (float)uStack_2c;
  FUN_1409f23b0(*(longlong *)(lVar12 + ((longlong)*(int *)(lVar12 + 0x2c) + 3) * 0x10) + 0x228,
                local_res8,param_1,&local_28);
  local_38 = (code *)&LAB_141189ba0;
  uStack_30 = 0;
  local_28 = 9.537994;
  fStack_24 = 1.4013e-45;
  fStack_20 = 0.0;
  fStack_1c = (float)uStack_2c;
  FUN_1409f23b0(*(longlong *)(lVar12 + ((longlong)*(int *)(lVar12 + 0x2c) + 3) * 0x10) + 0x240,
                local_res8,param_1,&local_28);
  uStack_30 = 0;
  local_38 = (code *)&LAB_141181130;
  local_28 = 9.504196;
  fStack_24 = 1.4013e-45;
  fStack_20 = 0.0;
  fStack_1c = (float)uStack_2c;
  FUN_1409f23b0(*(longlong *)(lVar12 + ((longlong)*(int *)(lVar12 + 0x2c) + 3) * 0x10) + 0xd8,
                local_res8,param_1,&local_28);
  uStack_30 = 0;
  local_38 = (code *)&LAB_141181110;
  local_28 = 9.504166;
  fStack_24 = 1.4013e-45;
  fStack_20 = 0.0;
  fStack_1c = (float)uStack_2c;
  FUN_1409f23b0(*(longlong *)(lVar12 + ((longlong)*(int *)(lVar12 + 0x2c) + 3) * 0x10) + 0xf0,
                local_res8,param_1,&local_28);
  local_38 = (code *)&LAB_141189ba0;
  uStack_30 = 0;
  local_28 = 9.537994;
  fStack_24 = 1.4013e-45;
  fStack_20 = 0.0;
  fStack_1c = (float)uStack_2c;
  FUN_1409f23b0(*(longlong *)(lVar12 + ((longlong)*(int *)(lVar12 + 0x2c) + 3) * 0x10) + 600,
                local_res8,param_1,&local_28);
  FUN_1414cb6d0(lVar12,param_1);
  lVar12 = *(longlong *)(param_1 + 0x128);
  iVar7 = *(int *)(param_1 + 0x1e0);
  plVar14 = (longlong *)(lVar12 + 0x398);
  plVar8 = (longlong *)FUN_141df5850(0,0x30,0);
  uStack_30 = 0;
  local_38 = FUN_14118dd90;
  if (iVar7 != 0) {
    local_38 = FUN_141190d20;
  }
  if (plVar8 != (longlong *)0x0) {
    *plVar8 = (longlong)&PTR_FUN_143968d20;
    FUN_1420575e0(plVar8 + 1,param_1);
    *(undefined4 *)(plVar8 + 2) = (undefined4)local_38;
    *(undefined4 *)((longlong)plVar8 + 0x14) = local_38._4_4_;
    *(undefined4 *)(plVar8 + 3) = uStack_30;
    *(undefined4 *)((longlong)plVar8 + 0x1c) = uStack_2c;
    lVar9 = FUN_141db8700();
    plVar8[5] = lVar9;
    *plVar8 = (longlong)&PTR_FUN_143968d70;
    FUN_1407e7830(plVar14,1);
    (**(code **)(*plVar8 + 0x28))(plVar8,local_res8);
    iVar7 = *(int *)(lVar12 + 0x3a0);
    *(int *)(lVar12 + 0x3a0) = iVar7 + 1;
    if (*(int *)(lVar12 + 0x3a4) < iVar7 + 1) {
      FUN_1407f1790(plVar14,iVar7);
    }
    puVar10 = (undefined8 *)((longlong)iVar7 * 0x10 + *plVar14);
    *puVar10 = plVar8;
    *(undefined4 *)(puVar10 + 1) = 3;
  }
  FUN_140804ba0(*(longlong *)(param_1 + 0x128) + 0x3b0);
  lVar12 = *(longlong *)(param_1 + 0x128);
  plVar14 = (longlong *)(lVar12 + 0x3b0);
  plVar8 = (longlong *)FUN_141df5850(0,0x20);
  if (plVar8 != (longlong *)0x0) {
    plVar8[1] = param_1;
    *plVar8 = (longlong)&PTR_FUN_143d04c50;
    lVar9 = FUN_141db8700();
    plVar8[3] = lVar9;
    *plVar8 = (longlong)&PTR_FUN_143d04ca0;
    FUN_1407e7830(plVar14,1);
    (**(code **)(*plVar8 + 0x28))(plVar8,local_res8);
    iVar7 = *(int *)(lVar12 + 0x3b8);
    *(int *)(lVar12 + 0x3b8) = iVar7 + 1;
    if (*(int *)(lVar12 + 0x3bc) < iVar7 + 1) {
      FUN_1407f1790(plVar14,iVar7);
    }
    puVar10 = (undefined8 *)((longlong)iVar7 * 0x10 + *plVar14);
    *puVar10 = plVar8;
    *(undefined4 *)(puVar10 + 1) = 2;
  }
  lVar12 = thunk_FUN_140d2ba50();
  if (lVar12 != 0) {
    uVar6 = FUN_141186d60(param_1);
    uVar11 = FUN_1415458f0(lVar12);
    FUN_1416f7bd0(uVar11,uVar6,8,0);
    uVar11 = FUN_1415458f0(lVar12);
    FUN_1416f2c70(uVar11);
  }
  return;
}



// ---- FUN_14163c3b0 @ 14163c3b0 ----

void FUN_14163c3b0(longlong *param_1)

{
  longlong *plVar1;
  undefined4 uVar2;
  longlong lVar3;
  undefined8 uVar4;
  int iVar5;
  code *local_78;
  undefined4 uStack_70;
  undefined1 local_68 [8];
  longlong local_60;
  longlong local_50;
  longlong local_40;
  longlong local_30;
  
  FUN_141a88520();
  plVar1 = (longlong *)param_1[0x7b];
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,4);
  }
  lVar3 = thunk_FUN_140d2ba50();
  if (lVar3 != 0) {
    uVar4 = FUN_1415458f0(lVar3);
    FUN_1416f7bd0(uVar4,0x1b,8,0);
    uVar4 = FUN_1415458f0(lVar3);
    FUN_1416f2c70(uVar4);
  }
  uVar4 = FUN_140d283a0();
  uVar4 = FUN_140d2ba00(uVar4);
  local_78 = FUN_141635ef0;
  uStack_70 = 0;
  FUN_1410e5610(uVar4,param_1,&local_78,0x17);
  local_78 = FUN_141635b90;
  uStack_70 = 0;
  FUN_1410e5610(uVar4,param_1,&local_78,0x18);
  local_78 = FUN_141633c30;
  uStack_70 = 0;
  FUN_1410e5610(uVar4,param_1,&local_78,9);
  local_78 = FUN_141633790;
  uStack_70 = 0;
  FUN_1410e5610(uVar4,param_1,&local_78,10);
  local_78 = FUN_141635b90;
  uStack_70 = 0;
  FUN_1410e5610(uVar4,param_1,&local_78,0x19);
  FUN_1414cb6d0(uVar4,param_1);
  *(undefined4 *)(param_1 + 0x8d) = 0;
  if (0x7fffffff < *(uint *)((longlong)param_1 + 0x46c)) {
    FUN_140827b80(param_1 + 0x8c,0);
  }
  FUN_1416173d0(param_1[0x85]);
  FUN_141642810(param_1);
  FUN_14164c060(param_1);
  uVar2 = DAT_1450d6c88;
  (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x7c],0,1,0,DAT_1450d6c88);
  FUN_1416f2cc0(param_1[0x84]);
  FUN_1416134d0(param_1[0x85]);
  if ((int)param_1[0x82] < (int)param_1[0x80]) {
    (**(code **)(*(longlong *)param_1[0x86] + 0x250))((longlong *)param_1[0x86],3);
    FUN_1415749a0(param_1[0x86]);
  }
  *(undefined4 *)(param_1 + 0x8b) = 0xffffffff;
  FUN_141485f00(0x1d,uVar2,0,0);
  if ((int)param_1[0x80] != 0) {
    *(int *)(param_1 + 0x8b) = (int)param_1[0x8b] + 1;
    iVar5 = (int)param_1[0x8b] % (int)param_1[0x80];
    *(int *)(param_1 + 0x8b) = iVar5;
    lVar3 = FUN_1411fd400(local_68,*(longlong *)
                                    (*(longlong *)(param_1[0x81] + (longlong)iVar5 * 8) + 0x430) +
                                   0x20);
    FUN_1416fb690(param_1[0x83],lVar3 + 0x38,0);
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
  }
  FUN_14163e980(param_1,(int)param_1[0x8b],0);
  return;
}



// ---- FUN_1411af020 @ 1411af020 ----

void FUN_1411af020(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  undefined8 *local_18;
  undefined4 local_10;
  
  lVar1 = thunk_FUN_140d2ba50();
  if (lVar1 != 0) {
    uVar2 = FUN_1415458f0(lVar1);
    FUN_1416f7bd0(uVar2,0xf,8,0);
  }
  lVar1 = FUN_140d2aef0();
  if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x130) != 0)) {
    FUN_14144c490(*(longlong *)(lVar1 + 0x130),*(undefined4 *)(param_1 + 0x10));
  }
  FUN_14165b960(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x120),*(undefined4 *)(param_1 + 0x10),
                0);
  uVar2 = *(undefined8 *)(param_1 + 8);
  local_18 = (undefined8 *)0x0;
  local_10 = 0;
  puVar3 = (undefined8 *)FUN_141df5850(0,0x20,0);
  local_10 = 2;
  local_18 = puVar3;
  if (puVar3 != (undefined8 *)0x0) {
    *puVar3 = &PTR__purecall_143a2de70;
    *puVar3 = &PTR_FUN_143d140b8;
    puVar3[1] = uVar2;
    uVar2 = FUN_141db8700();
    puVar3[3] = uVar2;
    *puVar3 = &PTR_FUN_143d14108;
  }
  FUN_14166c240(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x120),&local_18);
  uVar2 = *(undefined8 *)(param_1 + 8);
  local_18 = (undefined8 *)0x0;
  local_10 = 0;
  puVar3 = (undefined8 *)FUN_141df5850(0,0x20);
  local_10 = 2;
  local_18 = puVar3;
  if (puVar3 != (undefined8 *)0x0) {
    *puVar3 = &PTR__purecall_143a2de70;
    *puVar3 = &PTR_FUN_143d14158;
    puVar3[1] = uVar2;
    uVar2 = FUN_141db8700();
    puVar3[3] = uVar2;
    *puVar3 = &PTR_FUN_143d141a8;
  }
  FUN_14166c210(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x120),&local_18);
  return;
}



// ---- FUN_1411afe10 @ 1411afe10 ----

undefined8 FUN_1411afe10(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  undefined8 *local_18;
  undefined4 local_10;
  
  lVar1 = thunk_FUN_140d2ba50();
  if (lVar1 != 0) {
    uVar2 = FUN_1415458f0(lVar1);
    FUN_1416f7bd0(uVar2,0xf,8,0);
  }
  lVar1 = FUN_140d2aef0();
  if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x130) != 0)) {
    FUN_14144c490(*(longlong *)(lVar1 + 0x130),*(undefined4 *)(param_1 + 0x10));
  }
  FUN_14165b960(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x120),*(undefined4 *)(param_1 + 0x10),
                0);
  uVar2 = *(undefined8 *)(param_1 + 8);
  local_18 = (undefined8 *)0x0;
  local_10 = 0;
  puVar3 = (undefined8 *)FUN_141df5850(0,0x20,0);
  local_10 = 2;
  local_18 = puVar3;
  if (puVar3 != (undefined8 *)0x0) {
    *puVar3 = &PTR__purecall_143a2de70;
    *puVar3 = &PTR_FUN_143d140b8;
    puVar3[1] = uVar2;
    uVar2 = FUN_141db8700();
    puVar3[3] = uVar2;
    *puVar3 = &PTR_FUN_143d14108;
  }
  FUN_14166c240(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x120),&local_18);
  uVar2 = *(undefined8 *)(param_1 + 8);
  local_18 = (undefined8 *)0x0;
  local_10 = 0;
  puVar3 = (undefined8 *)FUN_141df5850(0,0x20);
  local_10 = 2;
  local_18 = puVar3;
  if (puVar3 != (undefined8 *)0x0) {
    *puVar3 = &PTR__purecall_143a2de70;
    *puVar3 = &PTR_FUN_143d14158;
    puVar3[1] = uVar2;
    uVar2 = FUN_141db8700();
    puVar3[3] = uVar2;
    *puVar3 = &PTR_FUN_143d141a8;
  }
  FUN_14166c210(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x120),&local_18);
  return 1;
}



// ---- FUN_1411d6170 @ 1411d6170 ----

void FUN_1411d6170(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  undefined8 *local_18;
  undefined4 local_10;
  
  lVar1 = thunk_FUN_140d2ba50();
  if (lVar1 != 0) {
    uVar2 = FUN_1415458f0(lVar1);
    FUN_1416f7bd0(uVar2,0xf,8,0);
  }
  lVar1 = FUN_140d2aef0();
  if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x130) != 0)) {
    FUN_14144c490(*(longlong *)(lVar1 + 0x130),*(undefined4 *)(param_1 + 0x10));
  }
  FUN_14165b960(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x448),*(undefined4 *)(param_1 + 0x10),
                0);
  uVar2 = *(undefined8 *)(param_1 + 8);
  local_18 = (undefined8 *)0x0;
  local_10 = 0;
  puVar3 = (undefined8 *)FUN_141df5850(0,0x20,0);
  local_10 = 2;
  local_18 = puVar3;
  if (puVar3 != (undefined8 *)0x0) {
    *puVar3 = &PTR__purecall_143a2de70;
    *puVar3 = &PTR_FUN_143d26d48;
    puVar3[1] = uVar2;
    uVar2 = FUN_141db8700();
    puVar3[3] = uVar2;
    *puVar3 = &PTR_FUN_143d26d98;
  }
  FUN_14166c240(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x448),&local_18);
  uVar2 = *(undefined8 *)(param_1 + 8);
  local_18 = (undefined8 *)0x0;
  local_10 = 0;
  puVar3 = (undefined8 *)FUN_141df5850(0,0x20);
  local_10 = 2;
  local_18 = puVar3;
  if (puVar3 != (undefined8 *)0x0) {
    *puVar3 = &PTR__purecall_143a2de70;
    *puVar3 = &PTR_FUN_143d26de8;
    puVar3[1] = uVar2;
    uVar2 = FUN_141db8700();
    puVar3[3] = uVar2;
    *puVar3 = &PTR_FUN_143d26e38;
  }
  FUN_14166c210(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x448),&local_18);
  return;
}



// ---- FUN_1411d6bb0 @ 1411d6bb0 ----

undefined8 FUN_1411d6bb0(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  undefined8 *local_18;
  undefined4 local_10;
  
  lVar1 = thunk_FUN_140d2ba50();
  if (lVar1 != 0) {
    uVar2 = FUN_1415458f0(lVar1);
    FUN_1416f7bd0(uVar2,0xf,8,0);
  }
  lVar1 = FUN_140d2aef0();
  if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x130) != 0)) {
    FUN_14144c490(*(longlong *)(lVar1 + 0x130),*(undefined4 *)(param_1 + 0x10));
  }
  FUN_14165b960(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x448),*(undefined4 *)(param_1 + 0x10),
                0);
  uVar2 = *(undefined8 *)(param_1 + 8);
  local_18 = (undefined8 *)0x0;
  local_10 = 0;
  puVar3 = (undefined8 *)FUN_141df5850(0,0x20,0);
  local_10 = 2;
  local_18 = puVar3;
  if (puVar3 != (undefined8 *)0x0) {
    *puVar3 = &PTR__purecall_143a2de70;
    *puVar3 = &PTR_FUN_143d26d48;
    puVar3[1] = uVar2;
    uVar2 = FUN_141db8700();
    puVar3[3] = uVar2;
    *puVar3 = &PTR_FUN_143d26d98;
  }
  FUN_14166c240(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x448),&local_18);
  uVar2 = *(undefined8 *)(param_1 + 8);
  local_18 = (undefined8 *)0x0;
  local_10 = 0;
  puVar3 = (undefined8 *)FUN_141df5850(0,0x20);
  local_10 = 2;
  local_18 = puVar3;
  if (puVar3 != (undefined8 *)0x0) {
    *puVar3 = &PTR__purecall_143a2de70;
    *puVar3 = &PTR_FUN_143d26de8;
    puVar3[1] = uVar2;
    uVar2 = FUN_141db8700();
    puVar3[3] = uVar2;
    *puVar3 = &PTR_FUN_143d26e38;
  }
  FUN_14166c210(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x448),&local_18);
  return 1;
}



// ---- FUN_141695b80 @ 141695b80 ----

void FUN_141695b80(longlong param_1)

{
  if (*(longlong *)(param_1 + 0x3b0) != 0) {
    FUN_1416f7bd0(*(longlong *)(param_1 + 0x3b0),0x23,8,0);
    if (*(longlong *)(param_1 + 0x3b0) != 0) {
      FUN_1416f2c70();
    }
  }
  FUN_1416a1580(param_1,1);
  return;
}



// ---- FUN_14128bea0 @ 14128bea0 ----

void FUN_14128bea0(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  undefined8 *local_18;
  undefined4 local_10;
  
  lVar1 = thunk_FUN_140d2ba50();
  if (lVar1 != 0) {
    uVar2 = FUN_1415458f0(lVar1);
    FUN_1416f7bd0(uVar2,0xf,8,0);
  }
  lVar1 = FUN_140d2aef0();
  if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x130) != 0)) {
    FUN_14144c490(*(longlong *)(lVar1 + 0x130),*(undefined4 *)(param_1 + 0x10));
  }
  FUN_14165b960(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x138),*(undefined4 *)(param_1 + 0x10),
                0);
  uVar2 = *(undefined8 *)(param_1 + 8);
  local_18 = (undefined8 *)0x0;
  local_10 = 0;
  puVar3 = (undefined8 *)FUN_141df5850(0,0x20,0);
  local_10 = 2;
  local_18 = puVar3;
  if (puVar3 != (undefined8 *)0x0) {
    *puVar3 = &PTR__purecall_143a2de70;
    *puVar3 = &PTR_FUN_143d03cd8;
    puVar3[1] = uVar2;
    uVar2 = FUN_141db8700();
    puVar3[3] = uVar2;
    *puVar3 = &PTR_FUN_143d03d28;
  }
  FUN_14166c240(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x138),&local_18);
  uVar2 = *(undefined8 *)(param_1 + 8);
  local_18 = (undefined8 *)0x0;
  local_10 = 0;
  puVar3 = (undefined8 *)FUN_141df5850(0,0x20);
  local_10 = 2;
  local_18 = puVar3;
  if (puVar3 != (undefined8 *)0x0) {
    *puVar3 = &PTR__purecall_143a2de70;
    *puVar3 = &PTR_FUN_143d67388;
    puVar3[1] = uVar2;
    uVar2 = FUN_141db8700();
    puVar3[3] = uVar2;
    *puVar3 = &PTR_FUN_143d673d8;
  }
  FUN_14166c210(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x138),&local_18);
  return;
}



// ---- FUN_14128cb00 @ 14128cb00 ----

undefined8 FUN_14128cb00(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  undefined8 *local_18;
  undefined4 local_10;
  
  lVar1 = thunk_FUN_140d2ba50();
  if (lVar1 != 0) {
    uVar2 = FUN_1415458f0(lVar1);
    FUN_1416f7bd0(uVar2,0xf,8,0);
  }
  lVar1 = FUN_140d2aef0();
  if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x130) != 0)) {
    FUN_14144c490(*(longlong *)(lVar1 + 0x130),*(undefined4 *)(param_1 + 0x10));
  }
  FUN_14165b960(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x138),*(undefined4 *)(param_1 + 0x10),
                0);
  uVar2 = *(undefined8 *)(param_1 + 8);
  local_18 = (undefined8 *)0x0;
  local_10 = 0;
  puVar3 = (undefined8 *)FUN_141df5850(0,0x20,0);
  local_10 = 2;
  local_18 = puVar3;
  if (puVar3 != (undefined8 *)0x0) {
    *puVar3 = &PTR__purecall_143a2de70;
    *puVar3 = &PTR_FUN_143d03cd8;
    puVar3[1] = uVar2;
    uVar2 = FUN_141db8700();
    puVar3[3] = uVar2;
    *puVar3 = &PTR_FUN_143d03d28;
  }
  FUN_14166c240(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x138),&local_18);
  uVar2 = *(undefined8 *)(param_1 + 8);
  local_18 = (undefined8 *)0x0;
  local_10 = 0;
  puVar3 = (undefined8 *)FUN_141df5850(0,0x20);
  local_10 = 2;
  local_18 = puVar3;
  if (puVar3 != (undefined8 *)0x0) {
    *puVar3 = &PTR__purecall_143a2de70;
    *puVar3 = &PTR_FUN_143d67388;
    puVar3[1] = uVar2;
    uVar2 = FUN_141db8700();
    puVar3[3] = uVar2;
    *puVar3 = &PTR_FUN_143d673d8;
  }
  FUN_14166c210(*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x138),&local_18);
  return 1;
}



// ---- FUN_1416d1370 @ 1416d1370 ----

void FUN_1416d1370(longlong param_1,char param_2)

{
  undefined8 uVar1;
  longlong lVar2;
  
  if (*(longlong *)(param_1 + 0x4d0) == 0) {
    lVar2 = FUN_140d2ba50();
    if (lVar2 != 0) {
      uVar1 = FUN_1415458f0(lVar2);
      *(undefined8 *)(param_1 + 0x4d0) = uVar1;
    }
  }
  *(bool *)(param_1 + 0x4da) = param_2 != '\a';
  if (*(longlong *)(param_1 + 0x4d0) != 0) {
    FUN_1416f7bd0(*(longlong *)(param_1 + 0x4d0),param_2,8,0);
  }
  return;
}



// ---- FUN_14140d220 @ 14140d220 ----

void FUN_14140d220(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  
  if (*(char *)(param_1 + 0xac) == '\0') {
    FUN_141418500(param_1,DAT_1459714d0,1);
    if (*(longlong *)(param_1 + 0x98) == 0) {
      lVar1 = FUN_140d2ba50();
      if (lVar1 != 0) {
        uVar2 = FUN_1415458f0(lVar1);
        *(undefined8 *)(param_1 + 0x98) = uVar2;
      }
    }
    if (*(longlong *)(param_1 + 0x98) != 0) {
      FUN_1416f7bd0(*(longlong *)(param_1 + 0x98),0x1c,8,0);
    }
    *(undefined2 *)(param_1 + 0xac) = 1;
  }
  return;
}



// ---- FUN_1416132b0 @ 1416132b0 ----

void FUN_1416132b0(longlong *param_1)

{
  int iVar1;
  bool bVar2;
  char cVar3;
  longlong lVar4;
  longlong *plVar5;
  longlong *plVar6;
  undefined8 uVar7;
  undefined1 local_res8 [8];
  longlong local_18;
  int local_10;
  
  if (*(char *)((longlong)param_1 + 0x7a5) != '\0') {
    return;
  }
  cVar3 = FUN_1429b4160();
  if (cVar3 != '\0') {
    return;
  }
  *(undefined1 *)(param_1 + 0xfb) = 1;
  if ((param_1[0xc6] == 0) && (lVar4 = FUN_140d2ba50(), lVar4 != 0)) {
    lVar4 = FUN_1415458f0(lVar4);
    param_1[0xc6] = lVar4;
  }
  if (param_1[0xc6] != 0) {
    cVar3 = FUN_140d2f960();
    if ((((cVar3 == '\0') && (cVar3 = FUN_140d2f970(), cVar3 == '\0')) &&
        (cVar3 = FUN_140d2f9d0(), cVar3 == '\0')) &&
       ((cVar3 = FUN_140d2f9e0(), cVar3 == '\0' && (cVar3 = FUN_140d2f9f0(), cVar3 == '\0')))) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    plVar5 = (longlong *)FUN_141414020(param_1[199]);
    if ((((int)*plVar5 != 0) || (*(int *)((longlong)plVar5 + 4) != 0)) &&
       (plVar6 = (longlong *)FUN_140d2a600(local_res8), *plVar5 != *plVar6)) {
      bVar2 = true;
    }
    if (bVar2) {
      FUN_1416f7bd0(param_1[0xc6],0x24,8,0);
      plVar5 = (longlong *)param_1[0xc5];
      if (plVar5 == (longlong *)0x0) goto LAB_1416133d4;
      uVar7 = 1;
    }
    else {
      FUN_1416f7bd0(param_1[0xc6],0x1c,8,0);
      plVar5 = (longlong *)param_1[0xc5];
      if (plVar5 == (longlong *)0x0) goto LAB_1416133d4;
      uVar7 = 3;
    }
    (**(code **)(*plVar5 + 0x250))(plVar5,uVar7);
  }
LAB_1416133d4:
  (**(code **)(*param_1 + 0x468))(param_1,1);
  FUN_14160ba60(param_1,&local_18);
  iVar1 = (local_10 + -1) / 10;
  if (param_1[0x8e] != 0) {
    FUN_1416f37c0();
    (**(code **)(*(longlong *)param_1[0x8e] + 0x468))((longlong *)param_1[0x8e],0 < iVar1);
  }
  if (param_1[0x8d] != 0) {
    FUN_1416f37c0();
    (**(code **)(*(longlong *)param_1[0x8d] + 0x468))((longlong *)param_1[0x8d],0 < iVar1);
  }
  (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0xb7],0,1,0,DAT_1450d6c88);
  *(undefined1 *)((longlong)param_1 + 0x7a5) = 1;
  lVar4 = FUN_140d28770();
  if (lVar4 != 0) {
    FUN_1411e1dc0(lVar4,5,1);
  }
  FUN_141a88520(param_1);
  if (local_18 == 0) {
    return;
  }
  FUN_141de9200();
  return;
}



// ---- FUN_1414cbba0 @ 1414cbba0 ----

void FUN_1414cbba0(longlong *param_1,undefined1 param_2,undefined1 param_3)

{
  undefined8 uVar1;
  longlong lVar2;
  
  *(undefined1 *)((longlong)param_1 + 0x4f1) = param_3;
  *(undefined1 *)((longlong)param_1 + 0x4f3) = 0;
  FUN_141504ed0(param_1[0x94],1);
  *(undefined1 *)((longlong)param_1 + 0x4ed) = 1;
  *(undefined1 *)((longlong)param_1 + 0x409) = param_2;
  *(undefined1 *)((longlong)param_1 + 0x4ea) = 0;
  uVar1 = FUN_140d282e0();
  lVar2 = FUN_1411d9d40(uVar1);
  param_1[0xbf] = lVar2;
  FUN_1416f2cc0(param_1[0x95]);
  if (((char)param_1[0x9d] != '\0') && (param_1[0xc4] != 0)) {
    FUN_1416f7bd0(param_1[0xc4],1,8,0);
    FUN_1416f2c70(param_1[0xc4]);
  }
  (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x97],0,1,0,DAT_1450d6c88);
  (**(code **)(*(longlong *)param_1[0x91] + 0x250))((longlong *)param_1[0x91],4);
  if (*(char *)((longlong)param_1 + 0x4f1) == '\0') {
    FUN_1414c8c70(param_1,1);
  }
  else {
    *(undefined1 *)((longlong)param_1 + 0x4f3) = 6;
  }
  (**(code **)(*(longlong *)param_1[0x94] + 0x250))((longlong *)param_1[0x94],3);
  FUN_1414fbf30(param_1[0x94],*(undefined1 *)((longlong)param_1 + 0x409),0);
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0xf7;
  *(undefined4 *)(param_1 + 0xa0) = 3;
  *(undefined4 *)(param_1 + 0xa1) = 8;
  return;
}



// ---- FUN_1414d75e0 @ 1414d75e0 ----

void FUN_1414d75e0(longlong *param_1,char param_2,longlong *param_3,char param_4,char param_5)

{
  longlong lVar1;
  longlong *plVar2;
  char cVar3;
  int iVar4;
  undefined8 uVar5;
  
  if (param_2 != '\0') {
    if ((param_4 != '\0') || (param_5 != '\0')) {
      FUN_1419a6f00(param_1[0xa2]);
      FUN_141501430(param_1[0x96],*(undefined1 *)((longlong)param_1 + 0x50c));
      FUN_1415060a0(param_1[0x94],*(undefined1 *)((longlong)param_1 + 0x50c));
      if (param_5 == '\0') {
        *(undefined4 *)(param_1 + 0xa0) = 10;
      }
    }
    if (*param_3 == 0) {
      cVar3 = FUN_1414f91e0(param_1[0x94]);
      if (cVar3 == '\0') {
        *(undefined4 *)(param_1 + 0xbb) = 0x80000001;
        FUN_1414e3170(param_1);
      }
      else {
        FUN_1414c8c70(param_1,4);
      }
    }
    else {
      uVar5 = FUN_141994880(param_1[0xbe],param_3);
      cVar3 = FUN_141991a70(uVar5);
      (**(code **)(*(longlong *)param_1[0x9c] + 0x250))((longlong *)param_1[0x9c],1);
      FUN_1414c8c70(param_1,4);
      if (cVar3 == '\0') {
        FUN_141504d40(param_1[0x94],param_3);
        FUN_1414f2e30(param_1[0xbf]);
      }
      else {
        if (cVar3 != *(char *)((longlong)param_1 + 0x50c)) {
          *(undefined1 *)(param_1 + 0x9e) = 1;
          FUN_1414c91b0(param_1,cVar3);
          return;
        }
        FUN_141504d40(param_1[0x94],param_3);
      }
      iVar4 = FUN_1414f2ab0(param_1[0x94],0);
      if (iVar4 == 1) {
        FUN_141504ed0(param_1[0x94],1);
      }
    }
    FUN_1416f2cc0(param_1[0x95]);
    if (((char)param_1[0x9d] != '\0') && (param_1[0xc4] != 0)) {
      FUN_1416f7bd0(param_1[0xc4],1,8,0);
      FUN_1416f2c70(param_1[0xc4]);
    }
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x97],0,1,0,DAT_1450d6c88);
    (**(code **)(*(longlong *)param_1[0x91] + 0x250))((longlong *)param_1[0x91],4);
    FUN_141501a70(param_1[0x94],1);
    (**(code **)(*(longlong *)param_1[0x94] + 0x250))((longlong *)param_1[0x94],3);
    FUN_1414fd6f0(param_1[0x94]);
    *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | 8;
    *(undefined4 *)(param_1 + 0xa1) = 1;
  }
  FUN_141a88520(param_1);
  (**(code **)(*(longlong *)param_1[0x91] + 0x498))((longlong *)param_1[0x91],param_1,DAT_145977e78)
  ;
  uVar5 = DAT_145977e68;
  lVar1 = param_1[0x91];
  plVar2 = *(longlong **)(lVar1 + 0x3a0);
  (**(code **)(*plVar2 + 0x470))(plVar2,param_1,DAT_145977e60);
  plVar2 = *(longlong **)(lVar1 + 0x3a8);
  (**(code **)(*plVar2 + 0x470))(plVar2,param_1,uVar5);
                    /* WARNING: Could not recover jumptable at 0x0001414d7884. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(longlong *)param_1[0x91] + 0x470))((longlong *)param_1[0x91],param_1,DAT_145977e80)
  ;
  return;
}



// ---- FUN_1414c9210 @ 1414c9210 ----

void FUN_1414c9210(longlong *param_1)

{
  longlong lVar1;
  longlong local_res10 [3];
  
  FUN_1414dd750(param_1[0x91],*(byte *)((longlong)param_1 + 0x50c) - 1);
  FUN_141501430(param_1[0x96],*(undefined1 *)((longlong)param_1 + 0x50c));
  lVar1 = FUN_141991f50(param_1[0xbe],*(undefined1 *)((longlong)param_1 + 0x50c));
  param_1[0xa2] = lVar1;
  FUN_14095b280(param_1[0xbf],local_res10);
  FUN_1414f2e30(param_1[0xbf]);
  if (local_res10[0] != 0) {
    FUN_141504d40(param_1[0x94],local_res10);
  }
  if ((int)param_1[0xa0] == 10) {
    FUN_1416f2cc0(param_1[0x95]);
    if (((char)param_1[0x9d] != '\0') && (param_1[0xc4] != 0)) {
      FUN_1416f7bd0(param_1[0xc4],1,8,0);
      FUN_1416f2c70(param_1[0xc4]);
    }
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x97],0,1,0,DAT_1450d6c88);
    (**(code **)(*(longlong *)param_1[0x91] + 0x250))((longlong *)param_1[0x91],4);
    FUN_141501a70(param_1[0x94],1);
    (**(code **)(*(longlong *)param_1[0x94] + 0x250))((longlong *)param_1[0x94],3);
    FUN_1414fd6f0(param_1[0x94]);
    *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | 8;
    *(undefined4 *)(param_1 + 0xa1) = 1;
  }
  return;
}



// ---- FUN_1414e24f0 @ 1414e24f0 ----

void FUN_1414e24f0(longlong param_1)

{
  if ((*(char *)(param_1 + 0x4e8) != '\0') && (*(longlong *)(param_1 + 0x620) != 0)) {
    FUN_1416f7bd0(*(longlong *)(param_1 + 0x620),1,8,0);
    FUN_1416f2c70(*(undefined8 *)(param_1 + 0x620));
    return;
  }
  return;
}



// ---- FUN_14158b7c0 @ 14158b7c0 ----

void FUN_14158b7c0(longlong param_1)

{
  undefined8 *puVar1;
  int iVar2;
  char cVar3;
  undefined8 uVar4;
  longlong *plVar5;
  longlong *plVar6;
  longlong *plVar7;
  wchar_t *pwVar8;
  int *piVar9;
  longlong lVar10;
  longlong lVar11;
  ulonglong local_res8;
  ulonglong local_res10;
  longlong local_a8 [2];
  undefined1 local_98 [24];
  undefined **local_80;
  longlong local_78;
  longlong local_68;
  longlong local_58;
  longlong local_48;
  
  uVar4 = FUN_140d28b40();
  *(undefined8 *)(param_1 + 0x6a0) = uVar4;
  uVar4 = FUN_140d28a90();
  *(undefined8 *)(param_1 + 0x6a8) = uVar4;
  FUN_141eb3410(&local_res8,&DAT_143ea3004,1);
  uVar4 = FUN_141a65110(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x398) = uVar4;
  FUN_141eb3410(&local_res8,&DAT_143ea301c,1);
  uVar4 = FUN_141a65110(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x3a0) = uVar4;
  FUN_141eb3410(&local_res8,"In_Select",1);
  uVar4 = FUN_141a65110(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x3a8) = uVar4;
  FUN_141eb3410(&local_res8,"Out_Select",1);
  uVar4 = FUN_141a65110(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x3b0) = uVar4;
  (**(code **)(**(longlong **)(param_1 + 0x3c0) + 0x250))(*(longlong **)(param_1 + 0x3c0),1);
  plVar5 = (longlong *)(param_1 + 0x448);
  lVar10 = 3;
  *(undefined4 *)(param_1 + 0x450) = 0;
  if (*(int *)(param_1 + 0x454) != 3) {
    FUN_140827c50(plVar5,3);
  }
  do {
    iVar2 = *(int *)(param_1 + 0x450);
    *(int *)(param_1 + 0x450) = iVar2 + 1;
    if (*(int *)(param_1 + 0x454) < iVar2 + 1) {
      FUN_1407f1510(plVar5,iVar2);
    }
    puVar1 = (undefined8 *)(*plVar5 + (longlong)iVar2 * 8);
    if (puVar1 != (undefined8 *)0x0) {
      *puVar1 = 0;
    }
    lVar10 = lVar10 + -1;
  } while (lVar10 != 0);
  FUN_141eb3410(&local_res8,"Start_Party_Bar00",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)*plVar5 = uVar4;
  (**(code **)(**(longlong **)*plVar5 + 0x250))(*(longlong **)*plVar5,1);
  FUN_141eb3410(&local_res8,"Start_Party_Bar01",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(*plVar5 + 8) = uVar4;
  (**(code **)(**(longlong **)(*plVar5 + 8) + 0x250))(*(longlong **)(*plVar5 + 8),1);
  FUN_141eb3410(&local_res8,"Start_Party_Bar02",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(*plVar5 + 0x10) = uVar4;
  (**(code **)(**(longlong **)(*plVar5 + 0x10) + 0x250))(*(longlong **)(*plVar5 + 0x10),1);
  FUN_141eb3410(&local_res8,"Ins_Icon_Group",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x578) = uVar4;
  FUN_141eb3410(&local_res8,"Txt_Title00",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x540) = uVar4;
  FUN_141eb3410(&local_res8,"Ins_Icon_Item",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x570) = uVar4;
  FUN_141eb3410(&local_res8,"Txt_Cap00",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x568) = uVar4;
  local_a8[0] = 0;
  local_a8[1] = 0xb;
  FUN_1407e2ee0(local_a8,0);
  FUN_141dd1c10(local_a8[0],L"sell_value",0x16);
  lVar10 = FUN_1411fd400(&local_80,local_a8);
  uVar4 = FUN_141e24190(local_98,lVar10 + 0x38);
  FUN_141a87ad0(*(undefined8 *)(param_1 + 0x568),uVar4);
  if (local_48 != 0) {
    FUN_141de9200();
  }
  if (local_58 != 0) {
    FUN_141de9200();
  }
  if (local_68 != 0) {
    FUN_141de9200();
  }
  if (local_78 != 0) {
    FUN_141de9200();
  }
  local_80 = &PTR_FUN_14397c0d8;
  if (local_a8[0] != 0) {
    FUN_141de9200();
  }
  FUN_141a54c00(*(undefined8 *)(param_1 + 0x568));
  FUN_141eb3410(&local_res8,"Txt_Detail00",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x548) = uVar4;
  FUN_141eb3410(&local_res8,"Txt_Cap01",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x528) = uVar4;
  local_a8[0] = 0;
  local_a8[1] = 0xd;
  FUN_1407e2ee0(local_a8,0);
  FUN_141dd1c10(local_a8[0],L"where_to_get",0x1a);
  lVar10 = FUN_1411fd400(&local_80,local_a8);
  uVar4 = FUN_141e24190(local_98,lVar10 + 0x38);
  FUN_141a87ad0(*(undefined8 *)(param_1 + 0x528),uVar4);
  if (local_48 != 0) {
    FUN_141de9200();
  }
  if (local_58 != 0) {
    FUN_141de9200();
  }
  if (local_68 != 0) {
    FUN_141de9200();
  }
  if (local_78 != 0) {
    FUN_141de9200();
  }
  local_80 = &PTR_FUN_14397c0d8;
  if (local_a8[0] != 0) {
    FUN_141de9200();
  }
  FUN_141a54c00(*(undefined8 *)(param_1 + 0x528));
  FUN_141eb3410(&local_res8,"Txt_Detail01",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x530) = uVar4;
  FUN_141eb3410(&local_res8,"Txt_Detail02",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x550) = uVar4;
  FUN_141eb3410(&local_res8,"Txt_Cap02",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x558) = uVar4;
  FUN_141eb3410(&local_res8,"Pnl_Icon_Cook",1);
  plVar5 = (longlong *)FUN_1429af1b0(param_1,&local_res8);
  *(longlong **)(param_1 + 0x588) = plVar5;
  if (plVar5 != (longlong *)0x0) {
    (**(code **)(*plVar5 + 0x250))(plVar5,1);
  }
  FUN_141eb3410(&local_res8,"Dmy_Img_Cook",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x590) = uVar4;
  (**(code **)(**(longlong **)(param_1 + 0x3d8) + 0x250))(*(longlong **)(param_1 + 0x3d8),1);
  (**(code **)(**(longlong **)(param_1 + 1000) + 0x250))(*(longlong **)(param_1 + 1000),1);
  FUN_1415d1700(*(undefined8 *)(param_1 + 1000),0);
  FUN_141eb3410(&local_res8,"Pnl_Detail",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x538) = uVar4;
  FUN_141eb3410(&local_res8,"Txt_Detail03",1);
  uVar4 = FUN_1429af1b0(param_1,&local_res8);
  *(undefined8 *)(param_1 + 0x560) = uVar4;
  FUN_141eb3410(&local_res8,"Pnl_Win_Fil",1);
  plVar5 = (longlong *)FUN_1429af1b0(param_1,&local_res8);
  *(longlong **)(param_1 + 0x580) = plVar5;
  (**(code **)(*plVar5 + 0x250))(plVar5,1);
  local_a8[0] = 0;
  local_a8[1] = 0xf;
  FUN_1407e2ee0(local_a8,0);
  FUN_141dd1c10(local_a8[0],L"possession_num",0x1e);
  lVar10 = FUN_1411fd400(&local_80,local_a8);
  plVar6 = (longlong *)(param_1 + 0x5e8);
  plVar5 = (longlong *)(lVar10 + 0x38);
  if (plVar6 != plVar5) {
    if (*plVar6 != 0) {
      FUN_141de9200();
    }
    *plVar6 = *plVar5;
    *plVar5 = 0;
    *(undefined4 *)(param_1 + 0x5f0) = *(undefined4 *)(lVar10 + 0x40);
    *(undefined4 *)(param_1 + 0x5f4) = *(undefined4 *)(lVar10 + 0x44);
    *(undefined8 *)(lVar10 + 0x40) = 0;
  }
  if (local_48 != 0) {
    FUN_141de9200();
  }
  if (local_58 != 0) {
    FUN_141de9200();
  }
  if (local_68 != 0) {
    FUN_141de9200();
  }
  if (local_78 != 0) {
    FUN_141de9200();
  }
  local_80 = &PTR_FUN_14397c0d8;
  if (local_a8[0] != 0) {
    FUN_141de9200();
  }
  cVar3 = FUN_140d2f9e0();
  if ((cVar3 == '\0') && (cVar3 = FUN_140d2f9f0(), cVar3 == '\0')) {
    local_a8[0] = 0;
    local_a8[1] = 0xb;
    FUN_1407e2ee0(local_a8,0);
    uVar4 = 0x16;
    pwVar8 = L"sell_price";
  }
  else {
    local_a8[0] = 0;
    local_a8[1] = 0x10;
    FUN_1407e2ee0(local_a8,0);
    uVar4 = 0x20;
    pwVar8 = L"DLC7_sell_price";
  }
  FUN_141dd1c10(local_a8[0],pwVar8,uVar4);
  lVar10 = FUN_1411fd400(&local_80,local_a8);
  plVar6 = (longlong *)(param_1 + 0x5f8);
  plVar5 = (longlong *)(lVar10 + 0x38);
  if (plVar6 != plVar5) {
    if (*plVar6 != 0) {
      FUN_141de9200();
    }
    *plVar6 = *plVar5;
    *plVar5 = 0;
    *(undefined4 *)(param_1 + 0x600) = *(undefined4 *)(lVar10 + 0x40);
    *(undefined4 *)(param_1 + 0x604) = *(undefined4 *)(lVar10 + 0x44);
    *(undefined8 *)(lVar10 + 0x40) = 0;
  }
  if (local_48 != 0) {
    FUN_141de9200();
  }
  if (local_58 != 0) {
    FUN_141de9200();
  }
  if (local_68 != 0) {
    FUN_141de9200();
  }
  if (local_78 != 0) {
    FUN_141de9200();
  }
  local_80 = &PTR_FUN_14397c0d8;
  if (local_a8[0] != 0) {
    FUN_141de9200();
  }
  local_a8[0] = 0;
  local_a8[1] = 0xc;
  FUN_1407e2ee0(local_a8,0);
  FUN_141dd1c10(local_a8[0],L"MSG_NotSell",0x18);
  lVar10 = FUN_1411fd400(&local_80);
  plVar6 = (longlong *)(param_1 + 0x608);
  plVar5 = (longlong *)(lVar10 + 0x38);
  if (plVar6 != plVar5) {
    if (*plVar6 != 0) {
      FUN_141de9200();
    }
    *plVar6 = *plVar5;
    *plVar5 = 0;
    *(undefined4 *)(param_1 + 0x610) = *(undefined4 *)(lVar10 + 0x40);
    *(undefined4 *)(param_1 + 0x614) = *(undefined4 *)(lVar10 + 0x44);
    *(undefined8 *)(lVar10 + 0x40) = 0;
  }
  if (local_48 != 0) {
    FUN_141de9200();
  }
  if (local_58 != 0) {
    FUN_141de9200();
  }
  if (local_68 != 0) {
    FUN_141de9200();
  }
  if (local_78 != 0) {
    FUN_141de9200();
  }
  local_80 = &PTR_FUN_14397c0d8;
  if (local_a8[0] != 0) {
    FUN_141de9200();
  }
  lVar10 = FUN_140d2ba50();
  if ((lVar10 != 0) && (lVar10 = FUN_1415458f0(lVar10), lVar10 != 0)) {
    FUN_1416f7bd0(lVar10,2,8,0);
  }
  FUN_1410770a0(param_1 + 0x648);
  plVar5 = (longlong *)(param_1 + 0x630);
  *(undefined4 *)(param_1 + 0x640) = 0;
  *(undefined4 *)(param_1 + 0x638) = 0;
  if (*(int *)(param_1 + 0x63c) != 0) {
    FUN_140827be0(plVar5);
  }
  plVar6 = (longlong *)FUN_141361010(local_a8);
  if (plVar5 != plVar6) {
    if (*plVar5 != 0) {
      FUN_141de9200();
    }
    *plVar5 = *plVar6;
    *plVar6 = 0;
    *(int *)(param_1 + 0x638) = (int)plVar6[1];
    *(undefined4 *)(param_1 + 0x63c) = *(undefined4 *)((longlong)plVar6 + 0xc);
    plVar6[1] = 0;
  }
  if (local_a8[0] != 0) {
    FUN_141de9200();
  }
  plVar6 = (longlong *)(param_1 + 0x458);
  *(undefined4 *)(param_1 + 0x644) = 0;
  iVar2 = *(int *)(param_1 + 0x460);
  if (iVar2 < 0x115) {
    *(undefined4 *)(param_1 + 0x460) = 0x115;
    if (*(int *)(param_1 + 0x464) < 0x115) {
      FUN_1407f1510(plVar6);
    }
    memset((void *)(*plVar6 + (longlong)iVar2 * 8),0,(longlong)(0x115 - iVar2) << 3);
  }
  else if ((0x115 < iVar2) && (iVar2 + -0x115 != 0)) {
    *(int *)(param_1 + 0x460) = iVar2 - (iVar2 + -0x115);
    FUN_1408277b0(plVar6);
  }
  plVar6 = (longlong *)(param_1 + 0x518);
  iVar2 = *(int *)(param_1 + 0x520);
  if (iVar2 < 0x115) {
    *(undefined4 *)(param_1 + 0x520) = 0x115;
    if (*(int *)(param_1 + 0x524) < 0x115) {
      FUN_1407f1790(plVar6);
    }
    memset((void *)((longlong)iVar2 * 0x10 + *plVar6),0,(longlong)(0x115 - iVar2) << 4);
  }
  else if (0x115 < iVar2) {
    FUN_140a96180(plVar6,0x115,iVar2 + -0x115,1);
  }
  piVar9 = (int *)*plVar5;
  local_res8 = 0;
  local_res10 = (longlong)*(int *)(param_1 + 0x638) & 0x3fffffffffffffff;
  if (piVar9 + *(int *)(param_1 + 0x638) < piVar9) {
    local_res10 = 0;
  }
  if (local_res10 != 0) {
    do {
      iVar2 = *piVar9;
      lVar10 = *plVar6;
      plVar7 = (longlong *)FUN_1416e98b0(local_a8,iVar2,1);
      lVar11 = (longlong)iVar2 * 0x10;
      plVar5 = (longlong *)(lVar10 + lVar11);
      if (plVar5 != plVar7) {
        if (*plVar5 != 0) {
          FUN_141de9200();
        }
        *plVar5 = *plVar7;
        *plVar7 = 0;
        *(int *)(plVar5 + 1) = (int)plVar7[1];
        *(undefined4 *)((longlong)plVar5 + 0xc) = *(undefined4 *)((longlong)plVar7 + 0xc);
        plVar7[1] = 0;
      }
      if (local_a8[0] != 0) {
        FUN_141de9200();
      }
      FUN_141a6eae0(param_1,*plVar6 + lVar11,0);
      piVar9 = piVar9 + 1;
      local_res8 = local_res8 + 1;
    } while (local_res8 != local_res10);
  }
  return;
}



// ---- FUN_14159c330 @ 14159c330 ----

void FUN_14159c330(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  
  if (*(longlong *)(param_1 + 0xbd8) != 0) {
    FUN_1416ca990();
  }
  lVar1 = thunk_FUN_140d2ba50();
  if (lVar1 != 0) {
    uVar2 = FUN_1415458f0(lVar1);
    FUN_1416f7bd0(uVar2,0x15,8,0);
    uVar2 = FUN_1415458f0(lVar1);
    FUN_1416f2c70(uVar2);
    return;
  }
  return;
}



// ---- FUN_1416123f0 @ 1416123f0 ----

void FUN_1416123f0(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  lVar1 = thunk_FUN_140d2ba50();
  if (lVar1 != 0) {
    lVar2 = FUN_1415458f0(lVar1);
    if (lVar2 != 0) {
      uVar3 = FUN_1415458f0(lVar1);
      FUN_1416f7bd0(uVar3,*(undefined1 *)(param_1 + 0x641),8,0);
      uVar3 = FUN_1415458f0(lVar1);
      FUN_1416f2c70(uVar3);
    }
  }
  if (*(longlong *)(param_1 + 0x438) != 0) {
    FUN_1416f2cc0();
  }
  return;
}



// ---- FUN_14163be40 @ 14163be40 ----

void FUN_14163be40(longlong *param_1,char param_2)

{
  undefined8 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  longlong lVar5;
  char cVar6;
  longlong lVar7;
  longlong *plVar8;
  longlong *plVar9;
  longlong *plVar10;
  undefined8 uVar11;
  int iVar12;
  undefined1 *puVar13;
  longlong *plVar14;
  undefined8 *puVar15;
  longlong lVar16;
  int iVar17;
  
  if ((char)param_1[0x8e] == '\0') {
    if (param_1[0x7d] == 0) {
      if (*(char *)((longlong)param_1 + 0x471) != '\0') {
        return;
      }
    }
    else {
      if (*(char *)((longlong)param_1 + 0x471) != '\0') {
        return;
      }
      cVar6 = FUN_1416ee150();
      if (cVar6 == '\0') {
        return;
      }
    }
    FUN_141443b50();
    if (param_2 == '\x03') {
      param_1[0x96] = 0;
      *(undefined4 *)(param_1 + 0x97) = 0;
      *(undefined1 *)((longlong)param_1 + 0x48c) = 3;
      (**(code **)(*(longlong *)param_1[0x81] + 0x250))((longlong *)param_1[0x81],0);
      (**(code **)(*(longlong *)param_1[0x80] + 0x250))((longlong *)param_1[0x80],3);
    }
    else {
      (**(code **)(*(longlong *)param_1[0x80] + 0x250))();
      (**(code **)(*(longlong *)param_1[0x81] + 0x250))((longlong *)param_1[0x81],3);
      *(undefined1 *)((longlong)param_1 + 0x48c) = 1;
    }
    param_1[0x92] = 0;
    param_1[0x93] = 0;
    if (param_1[0x7e] != 0) {
      FUN_141484e10();
    }
    if (param_1[0x80] != 0) {
      FUN_1415981c0();
    }
    if (param_1[0x81] != 0) {
      FUN_1415bd330();
    }
    if (param_1[0x82] != 0) {
      FUN_1415bd330();
    }
    plVar8 = (longlong *)param_1[0x83];
    if (plVar8 != (longlong *)0x0) {
      (**(code **)(*plVar8 + 0x250))(plVar8,3);
    }
    if (*(char *)((longlong)param_1 + 0x48c) == '\x01') {
      FUN_141643f60(param_1);
    }
    else {
      FUN_141596e00(param_1 + 0xd4,0);
      FUN_141596e00(param_1 + 0xda,0);
    }
    FUN_1416446b0(param_1);
    FUN_141643bc0(param_1);
    lVar7 = param_1[0x83];
    iVar17 = (int)param_1[0x91];
    if (lVar7 != 0) {
      plVar8 = (longlong *)(lVar7 + 0x3f0);
      if (plVar8 != param_1 + 0xe2) {
        iVar12 = *(int *)(lVar7 + 0x3f8);
        if (iVar12 != 0) {
          plVar14 = (longlong *)(*plVar8 + 0x10);
          do {
            if (*plVar14 != 0) {
              FUN_141de9200();
            }
            plVar14 = plVar14 + 6;
            iVar12 = iVar12 + -1;
          } while (iVar12 != 0);
        }
        iVar12 = (int)param_1[0xe3];
        lVar16 = param_1[0xe2];
        *(int *)(lVar7 + 0x3f8) = iVar12;
        if ((iVar12 == 0) && (*(int *)(lVar7 + 0x3fc) == 0)) {
          *(undefined4 *)(lVar7 + 0x3fc) = 0;
        }
        else {
          FUN_1407f1320(plVar8,iVar12);
          if (iVar12 != 0) {
            puVar15 = (undefined8 *)(lVar16 + 8);
            puVar13 = (undefined1 *)*plVar8;
            do {
              *puVar13 = *(undefined1 *)(puVar15 + -1);
              *(undefined4 *)(puVar13 + 4) = *(undefined4 *)((longlong)puVar15 + -4);
              *(undefined8 *)(puVar13 + 8) = *puVar15;
              FUN_1407e5680(puVar13 + 0x10,puVar15 + 1);
              puVar1 = puVar15 + 3;
              uVar2 = *(undefined4 *)((longlong)puVar15 + 0x1c);
              uVar3 = *(undefined4 *)(puVar15 + 4);
              uVar4 = *(undefined4 *)((longlong)puVar15 + 0x24);
              puVar15 = puVar15 + 6;
              *(undefined4 *)(puVar13 + 0x20) = *(undefined4 *)puVar1;
              *(undefined4 *)(puVar13 + 0x24) = uVar2;
              *(undefined4 *)(puVar13 + 0x28) = uVar3;
              *(undefined4 *)(puVar13 + 0x2c) = uVar4;
              iVar12 = iVar12 + -1;
              puVar13 = puVar13 + 0x30;
            } while (iVar12 != 0);
          }
        }
      }
      iVar12 = *(int *)(lVar7 + 0x3f8);
      *(int *)(lVar7 + 1000) = iVar12;
      *(int *)(lVar7 + 0x3e0) = iVar17;
      if (iVar12 <= iVar17) {
        iVar17 = iVar12 + -1;
        *(int *)(lVar7 + 0x3e0) = iVar17;
      }
      if (iVar17 < 0) {
        *(undefined4 *)(lVar7 + 0x3e0) = 0;
        iVar17 = 0;
      }
      *(int *)(lVar7 + 0x3e4) = iVar17;
      FUN_14164a970(lVar7);
    }
    FUN_14164a580(param_1,1);
    FUN_141645bf0(param_1);
    if ((param_1[0x8a] == 0) && (lVar7 = FUN_140d2ba50(), lVar7 != 0)) {
      lVar7 = FUN_1415458f0(lVar7);
      param_1[0x8a] = lVar7;
    }
    if (param_1[0x8a] != 0) {
      uVar11 = 10;
      if (*(char *)((longlong)param_1 + 0x48c) == '\x03') {
        uVar11 = 0xc;
      }
      FUN_1416f7bd0(param_1[0x8a],uVar11,8,0);
    }
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x86],0,1,0,DAT_1450d6c88);
    (**(code **)(*param_1 + 0x468))(param_1,1);
    *(undefined2 *)(param_1 + 0x8e) = 0x101;
    plVar14 = (longlong *)FUN_141433940(0);
    lVar16 = 4;
    lVar7 = 4;
    plVar8 = param_1 + 0xe9;
    do {
      plVar10 = plVar8;
      plVar9 = plVar14;
      plVar8 = plVar10 + 0x10;
      lVar5 = plVar9[1];
      *plVar10 = *plVar9;
      plVar10[1] = lVar5;
      lVar5 = plVar9[3];
      plVar10[2] = plVar9[2];
      plVar10[3] = lVar5;
      lVar5 = plVar9[5];
      plVar10[4] = plVar9[4];
      plVar10[5] = lVar5;
      lVar5 = plVar9[7];
      plVar10[6] = plVar9[6];
      plVar10[7] = lVar5;
      lVar5 = plVar9[9];
      plVar10[8] = plVar9[8];
      plVar10[9] = lVar5;
      lVar5 = plVar9[0xb];
      plVar10[10] = plVar9[10];
      plVar10[0xb] = lVar5;
      lVar5 = plVar9[0xd];
      plVar10[0xc] = plVar9[0xc];
      plVar10[0xd] = lVar5;
      lVar5 = plVar9[0xf];
      plVar10[0xe] = plVar9[0xe];
      plVar10[0xf] = lVar5;
      lVar7 = lVar7 + -1;
      plVar14 = plVar9 + 0x10;
    } while (lVar7 != 0);
    lVar7 = plVar9[0x11];
    *plVar8 = plVar9[0x10];
    plVar10[0x11] = lVar7;
    lVar7 = plVar9[0x13];
    plVar10[0x12] = plVar9[0x12];
    plVar10[0x13] = lVar7;
    lVar7 = plVar9[0x15];
    plVar10[0x14] = plVar9[0x14];
    plVar10[0x15] = lVar7;
    lVar7 = plVar9[0x17];
    plVar10[0x16] = plVar9[0x16];
    plVar10[0x17] = lVar7;
    uVar2 = *(undefined4 *)((longlong)plVar9 + 0xc4);
    lVar7 = plVar9[0x19];
    uVar3 = *(undefined4 *)((longlong)plVar9 + 0xcc);
    *(int *)(plVar10 + 0x18) = (int)plVar9[0x18];
    *(undefined4 *)((longlong)plVar10 + 0xc4) = uVar2;
    *(int *)(plVar10 + 0x19) = (int)lVar7;
    *(undefined4 *)((longlong)plVar10 + 0xcc) = uVar3;
    uVar2 = *(undefined4 *)((longlong)plVar9 + 0xd4);
    lVar7 = plVar9[0x1b];
    uVar3 = *(undefined4 *)((longlong)plVar9 + 0xdc);
    *(int *)(plVar10 + 0x1a) = (int)plVar9[0x1a];
    *(undefined4 *)((longlong)plVar10 + 0xd4) = uVar2;
    *(int *)(plVar10 + 0x1b) = (int)lVar7;
    *(undefined4 *)((longlong)plVar10 + 0xdc) = uVar3;
    plVar8 = (longlong *)FUN_141433940(CONCAT71((int7)((ulonglong)plVar8 >> 8),1));
    plVar14 = param_1 + 0x135;
    do {
      plVar10 = plVar14;
      plVar9 = plVar8;
      lVar7 = plVar9[1];
      *plVar10 = *plVar9;
      plVar10[1] = lVar7;
      lVar7 = plVar9[3];
      plVar10[2] = plVar9[2];
      plVar10[3] = lVar7;
      lVar7 = plVar9[5];
      plVar10[4] = plVar9[4];
      plVar10[5] = lVar7;
      lVar7 = plVar9[7];
      plVar10[6] = plVar9[6];
      plVar10[7] = lVar7;
      lVar7 = plVar9[9];
      plVar10[8] = plVar9[8];
      plVar10[9] = lVar7;
      lVar7 = plVar9[0xb];
      plVar10[10] = plVar9[10];
      plVar10[0xb] = lVar7;
      lVar7 = plVar9[0xd];
      plVar10[0xc] = plVar9[0xc];
      plVar10[0xd] = lVar7;
      lVar7 = plVar9[0xf];
      plVar10[0xe] = plVar9[0xe];
      plVar10[0xf] = lVar7;
      lVar16 = lVar16 + -1;
      plVar8 = plVar9 + 0x10;
      plVar14 = plVar10 + 0x10;
    } while (lVar16 != 0);
    lVar7 = plVar9[0x11];
    plVar10[0x10] = plVar9[0x10];
    plVar10[0x11] = lVar7;
    lVar7 = plVar9[0x13];
    plVar10[0x12] = plVar9[0x12];
    plVar10[0x13] = lVar7;
    lVar7 = plVar9[0x15];
    plVar10[0x14] = plVar9[0x14];
    plVar10[0x15] = lVar7;
    lVar7 = plVar9[0x17];
    plVar10[0x16] = plVar9[0x16];
    plVar10[0x17] = lVar7;
    lVar7 = plVar9[0x19];
    plVar10[0x18] = plVar9[0x18];
    plVar10[0x19] = lVar7;
    lVar7 = plVar9[0x1b];
    plVar10[0x1a] = plVar9[0x1a];
    plVar10[0x1b] = lVar7;
  }
  return;
}



// ---- FUN_14164fda0 @ 14164fda0 ----

void FUN_14164fda0(longlong *param_1)

{
  longlong *plVar1;
  undefined8 *puVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  ulonglong uVar6;
  longlong lVar7;
  longlong *plVar8;
  longlong lVar9;
  longlong *plVar10;
  undefined8 uVar11;
  longlong *plVar12;
  code *pcVar13;
  undefined2 *puVar14;
  ulonglong uVar15;
  int iVar16;
  undefined2 *puVar17;
  uint local_res8 [2];
  longlong local_res10;
  undefined1 local_res18 [8];
  undefined8 local_b8;
  undefined8 uStack_b0;
  code *local_98;
  undefined8 uStack_90;
  undefined **local_88;
  longlong local_80;
  longlong local_70;
  longlong local_60;
  longlong local_50;
  
  if ((char)param_1[0x92] == '\x01') {
    return;
  }
  FUN_14165a420(param_1 + 0x9f,0);
  uVar15 = 0;
  uStack_b0 = 0x11;
  local_b8 = (code *)0x0;
  FUN_1407e2ee0(&local_b8,0);
  FUN_141dd1c10(local_b8,L"MSG_active_skill",0x22);
  local_98 = (code *)local_res8;
  local_res8[0] = local_res8[0] & 0xffffff00;
  uStack_90 = &local_b8;
  FUN_14164ceb0(param_1 + 0x9f,&local_res10,&local_98,0);
  if (local_b8 != (code *)0x0) {
    FUN_141de9200();
  }
  local_b8 = (code *)0x0;
  uStack_b0 = 0x12;
  FUN_1407e2ee0(&local_b8,0);
  FUN_141dd1c10(local_b8,L"MSG_passive_skill",0x24);
  local_98 = (code *)local_res8;
  local_res8[0] = CONCAT31(local_res8[0]._1_3_,1);
  uStack_90 = &local_b8;
  FUN_14164ceb0(param_1 + 0x9f,&local_res10,&local_98,0);
  if (local_b8 != (code *)0x0) {
    FUN_141de9200();
  }
  local_b8 = (code *)0x0;
  uStack_b0 = 0x13;
  FUN_1407e2ee0(&local_b8,0);
  FUN_141dd1c10(local_b8,L"MSG_sparking_skill",0x26);
  local_98 = (code *)local_res8;
  local_res8[0] = CONCAT31(local_res8[0]._1_3_,2);
  uStack_90 = &local_b8;
  FUN_14164ceb0(param_1 + 0x9f,&local_res10,&local_98,0);
  if (local_b8 != (code *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3410(local_res8,"Xmenu_Bg",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_14164ff76:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_1417bfb40();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_14164ff76;
  }
  param_1[0x73] = uVar6;
  FUN_141eb3410(local_res8,"Xlist_List00_Lay7_R",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_14164ffdc:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_14176f790();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_14164ffdc;
  }
  param_1[0x74] = uVar6;
  FUN_141eb3410(local_res8,"Txt_Char_Name",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_141650042:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_141650042;
  }
  param_1[0x75] = uVar6;
  FUN_141eb3410(local_res8,"Txt_Cap01",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_1416500a8:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_1416500a8;
  }
  param_1[0x76] = uVar6;
  FUN_141eb3410(local_res8,"Txt_Tab_L",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_14165010e:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_14165010e;
  }
  param_1[0x77] = uVar6;
  FUN_141eb3410(local_res8,"Txt_Tab_R",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_141650174:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_141650174;
  }
  param_1[0x78] = uVar6;
  FUN_141eb3410(local_res8,"Txt_Detail",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_1416501da:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_1416501da;
  }
  param_1[0x79] = uVar6;
  FUN_141eb3410(local_res8,"Txt_Skillname",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_141650240:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_141650240;
  }
  param_1[0x7a] = uVar6;
  FUN_141eb3410(local_res8,"Txt_Lv",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_1416502a6:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_1416502a6;
  }
  param_1[0x7b] = uVar6;
  FUN_141eb3410(local_res8,"Txt_Lv_Num",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_14165030c:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_14165030c;
  }
  param_1[0x7c] = uVar6;
  FUN_141eb3410(local_res8,"Txt_Energy",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_141650372:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_141650372;
  }
  param_1[0x7d] = uVar6;
  FUN_141eb3410(local_res8,"Txt_Energy_Num",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_1416503d8:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_1416503d8;
  }
  param_1[0x7e] = uVar6;
  FUN_141eb3410(local_res8,"Xmenu_Arrow_L",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_14165043e:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_1417bf880();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_14165043e;
  }
  param_1[0x7f] = uVar6;
  FUN_141eb3410(local_res8,"Xmenu_Arrow_R",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_1416504a4:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_1417bf880();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_1416504a4;
  }
  param_1[0x80] = uVar6;
  FUN_141eb3410(local_res8,"Pnl_Txt_Detail",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_14165050a:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_1429f30c0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_14165050a;
  }
  param_1[0x83] = uVar6;
  FUN_141eb3410(local_res8,"Ins_Icon_Skill",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_141650570:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_1429f6b30();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_141650570;
  }
  param_1[0x84] = uVar6;
  FUN_141eb3410(local_res8,"Img_Black",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_1416505d6:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_1429f6b30();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_1416505d6;
  }
  param_1[0x85] = uVar6;
  FUN_141eb3410(local_res8,"Txt_00",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_14165063c:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_14165063c;
  }
  param_1[0x90] = uVar6;
  FUN_141eb3410(local_res8,"Txt_01",1);
  uVar6 = FUN_1429af1b0(param_1,local_res8);
  if (uVar6 == 0) {
LAB_1416506a2:
    uVar6 = uVar15;
  }
  else {
    lVar7 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(uVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8)
        != lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_1416506a2;
  }
  plVar1 = param_1 + 0x81;
  param_1[0x91] = uVar6;
  lVar7 = 8;
  *(undefined4 *)(param_1 + 0x82) = 0;
  if (*(int *)((longlong)param_1 + 0x414) != 8) {
    FUN_140827c50(plVar1,8);
  }
  do {
    iVar16 = (int)param_1[0x82];
    *(int *)(param_1 + 0x82) = iVar16 + 1;
    if (*(int *)((longlong)param_1 + 0x414) < iVar16 + 1) {
      FUN_1407f1510(plVar1,iVar16);
    }
    puVar2 = (undefined8 *)(*plVar1 + (longlong)iVar16 * 8);
    if (puVar2 != (undefined8 *)0x0) {
      *puVar2 = 0;
    }
    lVar7 = lVar7 + -1;
  } while (lVar7 != 0);
  local_res10 = 0;
  local_res8[0] = 0;
  do {
    lVar7 = local_res10;
    local_b8 = (code *)0x0;
    uStack_b0 = 0x11;
    FUN_1407e2ee0(&local_b8,0);
    puVar17 = (undefined2 *)local_b8;
    FUN_1407e4a20(local_b8,0x11,"Skillcustom_Bar0",0x11,0x3f);
    local_98 = (code *)0x0;
    uStack_90 = (undefined8 *)0x0;
    FUN_141db3e50(&local_98,uVar15);
    pcVar13 = local_98;
    uVar15 = uStack_b0;
    if ((int)uStack_90 == 0) {
      iVar16 = 0;
    }
    else {
      iVar16 = (int)uStack_90 + -1;
    }
    if (iVar16 != 0) {
      iVar4 = (int)uStack_b0;
      lVar7 = (longlong)(int)uStack_b0;
      iVar5 = (uint)((int)uStack_b0 == 0) + (int)uStack_b0 + iVar16;
      uStack_b0 = CONCAT44(uStack_b0._4_4_,iVar5);
      if (uStack_b0._4_4_ < iVar5) {
        FUN_1407e2ee0(&local_b8,uVar15 & 0xffffffff);
        puVar17 = (undefined2 *)local_b8;
      }
      memcpy(puVar17 + (lVar7 - (ulonglong)(iVar4 != 0)),pcVar13,(longlong)iVar16 * 2);
      (puVar17 + (lVar7 - (ulonglong)(iVar4 != 0)))[iVar16] = 0;
      lVar7 = local_res10;
    }
    plVar12 = (longlong *)0x0;
    if (local_98 != (code *)0x0) {
      FUN_141de9200(local_98);
    }
    puVar14 = &DAT_14391f7d0;
    if ((int)uStack_b0 != 0) {
      puVar14 = puVar17;
    }
    FUN_141eb3490(local_res18,puVar14,1);
    plVar8 = (longlong *)FUN_1429af1b0(param_1,local_res18);
    if (plVar8 == (longlong *)0x0) {
LAB_141650850:
      plVar8 = plVar12;
    }
    else {
      lVar9 = FUN_1417ad0c0();
      if ((*(int *)(plVar8[2] + 0x90) < *(int *)(lVar9 + 0x90)) ||
         (*(longlong *)(*(longlong *)(plVar8[2] + 0x88) + (longlong)*(int *)(lVar9 + 0x90) * 8) !=
          lVar9 + 0x88)) {
        bVar3 = false;
      }
      else {
        bVar3 = true;
      }
      if (!bVar3) goto LAB_141650850;
    }
    *(longlong **)(*plVar1 + lVar7) = plVar8;
    (**(code **)(**(longlong **)(*plVar1 + lVar7) + 0x470))
              (*(longlong **)(*plVar1 + lVar7),param_1,DAT_145985b38);
    (**(code **)(**(longlong **)(*plVar1 + lVar7) + 0x498))
              (*(longlong **)(*plVar1 + lVar7),param_1,DAT_145985b78);
    if (puVar17 != (undefined2 *)0x0) {
      FUN_141de9200(puVar17);
    }
    local_res10 = lVar7 + 8;
    local_res8[0] = local_res8[0] + 1;
    uVar15 = (ulonglong)local_res8[0];
  } while ((int)local_res8[0] < 8);
  FUN_141eb3410(local_res8,"Pnl_Skill00",1);
  plVar8 = (longlong *)FUN_1429af1b0(param_1,local_res8);
  if (plVar8 == (longlong *)0x0) {
LAB_141650920:
    plVar8 = plVar12;
  }
  else {
    lVar7 = FUN_1429f30c0();
    if ((*(int *)(plVar8[2] + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar8[2] + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8) !=
        lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_141650920;
  }
  FUN_141eb3410(&local_res10,"Pnl_Char_Name",1);
  plVar10 = (longlong *)FUN_1429af1b0(param_1,&local_res10);
  if (plVar10 == (longlong *)0x0) {
LAB_141650983:
    plVar10 = plVar12;
  }
  else {
    lVar7 = FUN_1429f2a50();
    if ((*(int *)(plVar10[2] + 0x90) < *(int *)(lVar7 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar10[2] + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8) !=
        lVar7 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_141650983;
  }
  if (plVar8 != (longlong *)0x0) {
    (**(code **)(*plVar8 + 0x250))(plVar8,3);
  }
  if (plVar10 != (longlong *)0x0) {
    (**(code **)(*plVar10 + 0x250))(plVar10,3);
  }
  (**(code **)(*(longlong *)param_1[0x74] + 0x470))((longlong *)param_1[0x74],param_1,DAT_145985b38)
  ;
  (**(code **)(*(longlong *)param_1[0x74] + 0x498))((longlong *)param_1[0x74],param_1,DAT_145985b78)
  ;
  FUN_1415c5fd0(param_1[0x74],param_1,DAT_145985b88);
  (**(code **)(*(longlong *)param_1[0x7f] + 0x470))((longlong *)param_1[0x7f],param_1,DAT_145985b60)
  ;
  (**(code **)(*(longlong *)param_1[0x80] + 0x470))((longlong *)param_1[0x80],param_1,DAT_145985b68)
  ;
  (**(code **)(*(longlong *)param_1[0x7f] + 0x498))((longlong *)param_1[0x7f],param_1,DAT_145985b78)
  ;
  (**(code **)(*(longlong *)param_1[0x80] + 0x498))((longlong *)param_1[0x80],param_1,DAT_145985b78)
  ;
  if (param_1[0x74] != 0) {
    FUN_1414decc0(param_1[0x74],DAT_143ef34f4);
  }
  FUN_141eb3410(local_res8,&DAT_143eea2ec,1);
  lVar7 = FUN_141a65110(param_1,local_res8);
  param_1[0x86] = lVar7;
  FUN_141eb3410(local_res8,&DAT_143eea35c,1);
  lVar7 = FUN_141a65110(param_1,local_res8);
  param_1[0x87] = lVar7;
  FUN_141eb3410(local_res8,"Slide_R_In",1);
  lVar7 = FUN_141a65110(param_1,local_res8);
  param_1[0x88] = lVar7;
  FUN_141eb3410(local_res8,"Slide_R_Out",1);
  lVar7 = FUN_141a65110(param_1,local_res8);
  param_1[0x89] = lVar7;
  FUN_141eb3410(local_res8,"Slide_L_In",1);
  lVar7 = FUN_141a65110(param_1,local_res8);
  param_1[0x8a] = lVar7;
  FUN_141eb3410(local_res8,"Slide_L_Out",1);
  lVar7 = FUN_141a65110(param_1,local_res8);
  param_1[0x8b] = lVar7;
  FUN_141eb3410(local_res8,"List_Cursor_In",1);
  lVar7 = FUN_141a65110(param_1,local_res8);
  param_1[0x8c] = lVar7;
  FUN_141eb3410(local_res8,"List_Cursor_Out",1);
  lVar7 = FUN_141a65110(param_1,local_res8);
  param_1[0x8d] = lVar7;
  if (param_1[0x7b] != 0) {
    local_98 = (code *)0x0;
    uStack_90 = (undefined8 *)&DAT_0000000b;
    FUN_1407e2ee0(&local_98,0);
    FUN_1407e4a20(local_98,0xb,"word_level",0xb,0x3f);
    lVar7 = FUN_1411fd400(&local_88,&local_98);
    uVar11 = FUN_141e24190(&local_b8,lVar7 + 0x38);
    FUN_141a87ad0(param_1[0x7b],uVar11);
    if (local_50 != 0) {
      FUN_141de9200();
    }
    if (local_60 != 0) {
      FUN_141de9200();
    }
    if (local_70 != 0) {
      FUN_141de9200();
    }
    if (local_80 != 0) {
      FUN_141de9200();
    }
    local_88 = &PTR_FUN_14397c0d8;
    if (local_98 != (code *)0x0) {
      FUN_141de9200();
    }
    FUN_141a54c00(param_1[0x7b]);
  }
  if (param_1[0x7d] != 0) {
    local_98 = (code *)0x0;
    uStack_90 = (undefined8 *)&DAT_0000000b;
    FUN_1407e2ee0(&local_98,0);
    FUN_1407e4a20(local_98,0xb,"Use_Energy",0xb,0x3f);
    lVar7 = FUN_1411fd400(&local_88,&local_98);
    uVar11 = FUN_141e24190(&local_b8,lVar7 + 0x38);
    FUN_141a87ad0(param_1[0x7d],uVar11);
    if (local_50 != 0) {
      FUN_141de9200();
    }
    if (local_60 != 0) {
      FUN_141de9200();
    }
    if (local_70 != 0) {
      FUN_141de9200();
    }
    if (local_80 != 0) {
      FUN_141de9200();
    }
    local_88 = &PTR_FUN_14397c0d8;
    if (local_98 != (code *)0x0) {
      FUN_141de9200();
    }
    FUN_141a54c00(param_1[0x7d]);
  }
  FUN_141eb3410(local_res8,"Txt_Cap00",1);
  lVar7 = FUN_1429af1b0(param_1,local_res8);
  if (lVar7 != 0) {
    lVar9 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(lVar7 + 0x10) + 0x90) < *(int *)(lVar9 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar7 + 0x10) + 0x88) + (longlong)*(int *)(lVar9 + 0x90) * 8)
        != lVar9 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (bVar3) {
      local_98 = (code *)0x0;
      uStack_90 = (undefined8 *)&DAT_0000000d;
      FUN_1407e2ee0(&local_98,0);
      FUN_1407e4a20(local_98,0xd,"Skill_Detail",0xd,0x3f);
      lVar9 = FUN_1411fd400(&local_88,&local_98);
      uVar11 = FUN_141e24190(&local_b8,lVar9 + 0x38);
      FUN_141a87ad0(lVar7,uVar11);
      if (local_50 != 0) {
        FUN_141de9200();
      }
      if (local_60 != 0) {
        FUN_141de9200();
      }
      if (local_70 != 0) {
        FUN_141de9200();
      }
      if (local_80 != 0) {
        FUN_141de9200();
      }
      local_88 = &PTR_FUN_14397c0d8;
      if (local_98 != (code *)0x0) {
        FUN_141de9200();
      }
      FUN_141a54c00(lVar7);
    }
  }
  lVar7 = FUN_140d2ba50();
  if ((lVar7 != 0) && (lVar7 = FUN_1415458f0(lVar7), lVar7 != 0)) {
    FUN_1416f7bd0(lVar7,0xf,8,0);
  }
  if (param_1[0x7f] != 0) {
    _guard_check_icall(param_1[0x7f],1);
  }
  if (param_1[0x80] != 0) {
    _guard_check_icall(param_1[0x80],0);
  }
  plVar12 = (longlong *)param_1[0x85];
  if (plVar12 != (longlong *)0x0) {
    (**(code **)(*plVar12 + 0x250))(plVar12,1);
  }
  lVar7 = FUN_140d2aef0();
  param_1[0x8e] = lVar7;
  if (lVar7 != 0) {
    param_1[0x8f] = *(longlong *)(lVar7 + 0x130);
  }
  (**(code **)(*param_1 + 0x250))(param_1,1);
  plVar12 = (longlong *)*plVar1;
  *(undefined1 *)((longlong)param_1 + 0x4ae) = 1;
  *(undefined4 *)((longlong)param_1 + 0x4b4) = 0;
  *(undefined1 *)(param_1 + 0x92) = 1;
  plVar8 = (longlong *)FUN_141045e20(&local_88,&DAT_145986e90);
  FUN_14171adf0(*(undefined8 *)(*plVar12 + 0x3e0),plVar8,0);
  if (plVar8[6] != 0) {
    FUN_141de9200();
  }
  if (plVar8[4] != 0) {
    FUN_141de9200();
  }
  if (*plVar8 != 0) {
    FUN_141de9200();
  }
  lVar7 = *plVar1;
  plVar12 = (longlong *)FUN_141045e20(&local_88,&DAT_145986ed0);
  FUN_14171adf0(*(undefined8 *)(*(longlong *)(lVar7 + 8) + 0x3e0),plVar12,0);
  if (plVar12[6] != 0) {
    FUN_141de9200();
  }
  if (plVar12[4] != 0) {
    FUN_141de9200();
  }
  if (*plVar12 != 0) {
    FUN_141de9200();
  }
  lVar7 = *plVar1;
  plVar12 = (longlong *)FUN_141045e20(&local_88,&DAT_145986f10);
  FUN_14171adf0(*(undefined8 *)(*(longlong *)(lVar7 + 0x10) + 0x3e0),plVar12,0);
  if (plVar12[6] != 0) {
    FUN_141de9200();
  }
  if (plVar12[4] != 0) {
    FUN_141de9200();
  }
  if (*plVar12 != 0) {
    FUN_141de9200();
  }
  lVar7 = *plVar1;
  plVar12 = (longlong *)FUN_141045e20(&local_88,&DAT_145986f50);
  FUN_14171adf0(*(undefined8 *)(*(longlong *)(lVar7 + 0x18) + 0x3e0),plVar12,0);
  if (plVar12[6] != 0) {
    FUN_141de9200();
  }
  if (plVar12[4] != 0) {
    FUN_141de9200();
  }
  if (*plVar12 != 0) {
    FUN_141de9200();
  }
  local_b8 = FUN_14165ee70;
  uStack_b0 = uStack_b0 & 0xffffffff00000000;
  lVar7 = *(longlong *)(*(longlong *)*plVar1 + 0x3e0);
  pcVar13 = (code *)FUN_141df5850(0,0x30);
  uStack_90 = (undefined8 *)CONCAT44(uStack_90._4_4_,3);
  local_98 = pcVar13;
  if (pcVar13 != (code *)0x0) {
    *(undefined ***)pcVar13 = &PTR_FUN_143968d20;
    FUN_1420575e0(pcVar13 + 8,param_1);
    *(undefined4 *)(pcVar13 + 0x10) = (undefined4)local_b8;
    *(undefined4 *)(pcVar13 + 0x14) = local_b8._4_4_;
    *(int *)(pcVar13 + 0x18) = (int)uStack_b0;
    *(int *)(pcVar13 + 0x1c) = uStack_b0._4_4_;
    lVar9 = FUN_141db8700();
    *(longlong *)(pcVar13 + 0x28) = lVar9;
    *(undefined ***)pcVar13 = &PTR_FUN_143968d70;
  }
  FUN_1409e14e0(lVar7 + 0x3b0,local_res8,&local_98);
  pcVar13 = local_98;
  if ((int)uStack_90 != 0) {
    if (local_98 == (code *)0x0) goto LAB_14165111a;
    (**(code **)(*(longlong *)local_98 + 0x30))(local_98,0);
    pcVar13 = (code *)FUN_141df5850(pcVar13,0,0);
  }
  if (pcVar13 != (code *)0x0) {
    FUN_141de9200(pcVar13);
  }
LAB_14165111a:
  local_98 = FUN_14165ee70;
  uStack_90._0_4_ = 0;
  FUN_1409f23b0(*(longlong *)(*(longlong *)(*plVar1 + 8) + 0x3e0) + 0x3b0,local_res8,param_1,
                &local_98);
  local_98 = FUN_14165ee70;
  uStack_90._0_4_ = 0;
  FUN_1409f23b0(*(longlong *)(*(longlong *)(*plVar1 + 0x10) + 0x3e0) + 0x3b0,local_res8,param_1,
                &local_98);
  local_98 = FUN_14165ee70;
  uStack_90 = (undefined8 *)((ulonglong)uStack_90._4_4_ << 0x20);
  FUN_1409f23b0(*(longlong *)(*(longlong *)(*plVar1 + 0x18) + 0x3e0) + 0x3b0,local_res8,param_1,
                &local_98);
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0xf7;
  *(undefined1 *)(param_1 + 0xb6) = 0;
  return;
}



// ---- FUN_141652240 @ 141652240 ----

void FUN_141652240(longlong *param_1)

{
  undefined2 *_Dst;
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  undefined8 uVar4;
  int iVar5;
  undefined2 *puVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  char cVar10;
  undefined2 *puVar11;
  undefined1 local_res8 [8];
  undefined2 *local_a8;
  undefined8 local_a0;
  undefined2 *local_98;
  undefined8 local_90;
  undefined2 *local_88;
  undefined8 local_80;
  undefined **local_70;
  longlong local_68;
  longlong local_58;
  longlong local_48;
  longlong local_38;
  
  if ((char)param_1[0x95] != '\x01') {
    (**(code **)(*(longlong *)param_1[0x80] + 0x250))((longlong *)param_1[0x80],1);
    FUN_141eb3410(local_res8,"Txt_Lv",1);
    lVar2 = FUN_1429af1b0(param_1,local_res8);
    if (lVar2 != 0) {
      local_a8 = (undefined2 *)0x0;
      local_a0 = 0xb;
      FUN_1407e2ee0(&local_a8,0);
      FUN_141dd1c10(local_a8,L"word_level",0x16);
      lVar3 = FUN_1411fd400(&local_70,&local_a8);
      uVar4 = FUN_141e24190(&local_88,lVar3 + 0x38);
      FUN_141a87ad0(lVar2,uVar4);
      if (local_38 != 0) {
        FUN_141de9200();
      }
      if (local_48 != 0) {
        FUN_141de9200();
      }
      if (local_58 != 0) {
        FUN_141de9200();
      }
      if (local_68 != 0) {
        FUN_141de9200();
      }
      local_70 = &PTR_FUN_14397c0d8;
      if (local_a8 != (undefined2 *)0x0) {
        FUN_141de9200();
      }
      FUN_141a54c00(lVar2);
    }
    param_1[0x8d] = param_1[0x81];
    param_1[0x8e] = param_1[0x82];
    local_a8 = (undefined2 *)0x0;
    local_a0 = 0xb;
    FUN_1407e2ee0(&local_a8,0);
    FUN_141dd1c10(local_a8,L"Use_Energy",0x16);
    FUN_1416fb690(param_1[0x8e],&local_a8,1);
    if (local_a8 != (undefined2 *)0x0) {
      FUN_141de9200();
    }
    param_1[0x8f] = param_1[0x83];
    param_1[0x8c] = param_1[0x84];
    param_1[0x90] = param_1[0x85];
    param_1[0x91] = param_1[0x86];
    FUN_141eb3410(local_res8,"Txt_Need_Zorb",1);
    lVar2 = FUN_1429af1b0(param_1,local_res8);
    if (lVar2 != 0) {
      local_a8 = (undefined2 *)0x0;
      local_a0 = 10;
      FUN_1407e2ee0(&local_a8,0);
      FUN_141dd1c10(local_a8,L"Need_Zorb",0x14);
      FUN_1416fb690(lVar2,&local_a8,1);
      if (local_a8 != (undefined2 *)0x0) {
        FUN_141de9200();
      }
    }
    FUN_141eb3410(local_res8,"Txt_Possess_Zorb",1);
    lVar2 = FUN_1429af1b0(param_1,local_res8);
    if (lVar2 != 0) {
      local_a8 = (undefined2 *)0x0;
      local_a0 = 0x10;
      FUN_1407e2ee0(&local_a8,0);
      FUN_141dd1c10(local_a8,L"MSG_HaveZorbNum",0x20);
      FUN_1416fb690(lVar2,&local_a8,1);
      if (local_a8 != (undefined2 *)0x0) {
        FUN_141de9200();
      }
    }
    cVar10 = '\0';
    do {
      local_88 = (undefined2 *)0x0;
      local_80 = 0x12;
      FUN_1407e2ee0(&local_88,0);
      puVar6 = local_88;
      FUN_1407e4a20(local_88,0x12,"Skilltree_Zorb00_",0x12,0x3f);
      FUN_141dbbf60(&local_a8,L"%02d",(int)cVar10);
      puVar11 = local_a8;
      iVar9 = (int)local_80;
      iVar8 = (int)local_a0;
      if ((int)local_80 < 2) {
        local_a8 = (undefined2 *)0x0;
        local_a0 = 0;
        if (puVar6 != (undefined2 *)0x0) {
          FUN_141de9200(puVar6);
        }
      }
      else {
        if ((int)local_a0 == 0) {
          iVar7 = 0;
        }
        else {
          iVar7 = (int)local_a0 + -1;
        }
        if (((int)local_80 != 0) || (iVar5 = 1, iVar7 == 0)) {
          iVar5 = 0;
        }
        iVar1 = local_80._4_4_;
        iVar5 = iVar5 + (int)local_80;
        local_98 = puVar6;
        local_88 = (undefined2 *)0x0;
        local_90._0_4_ = (int)local_80;
        local_90._4_4_ = local_80._4_4_;
        local_80 = 0;
        iVar8 = iVar9;
        if (iVar1 < iVar5 + iVar7) {
          FUN_1407f1af0(&local_98);
          iVar8 = (int)local_90;
        }
        puVar6 = local_a8;
        puVar11 = local_98;
        if (iVar7 != 0) {
          iVar9 = iVar7 + (uint)(iVar8 == 0) + iVar8;
          local_90 = CONCAT44(local_90._4_4_,iVar9);
          if (local_90._4_4_ < iVar9) {
            FUN_1407e2ee0(&local_98,iVar8);
            iVar9 = (int)local_90;
          }
          puVar11 = local_98;
          _Dst = local_98 + ((longlong)iVar8 - (ulonglong)(iVar8 != 0));
          memcpy(_Dst,puVar6,(longlong)iVar7 * 2);
          _Dst[iVar7] = 0;
          iVar8 = iVar9;
        }
        local_98 = (undefined2 *)0x0;
        local_90 = 0;
        if (local_a8 != (undefined2 *)0x0) {
          FUN_141de9200(local_a8);
        }
      }
      puVar6 = &DAT_14391f7d0;
      if (iVar8 != 0) {
        puVar6 = puVar11;
      }
      FUN_141eb3490(local_res8,puVar6,1);
      uVar4 = FUN_1429af1b0(param_1,local_res8);
      iVar8 = (int)param_1[0x8b];
      *(int *)(param_1 + 0x8b) = iVar8 + 1;
      if (*(int *)((longlong)param_1 + 0x45c) < iVar8 + 1) {
        FUN_1407f1510(param_1 + 0x8a,iVar8);
      }
      *(undefined8 *)(param_1[0x8a] + (longlong)iVar8 * 8) = uVar4;
      if (puVar11 != (undefined2 *)0x0) {
        FUN_141de9200(puVar11);
      }
      cVar10 = cVar10 + '\x01';
    } while (cVar10 < '\f');
    FUN_141eb3410(local_res8,&DAT_143ee9b74,1);
    lVar2 = FUN_141a65110(param_1,local_res8);
    param_1[0x87] = lVar2;
    FUN_141eb3410(local_res8,&DAT_143ee9bb4,1);
    lVar2 = FUN_141a65110(param_1,local_res8);
    param_1[0x88] = lVar2;
    lVar2 = FUN_140d2ba50();
    if ((lVar2 != 0) && (lVar2 = FUN_1415458f0(lVar2), lVar2 != 0)) {
      FUN_1416f7bd0(lVar2,0xe,8,0);
    }
    *(undefined1 *)(param_1 + 0x95) = 1;
    (**(code **)(*param_1 + 0x250))(param_1,1);
  }
  return;
}



// ---- FUN_141692240 @ 141692240 ----

void FUN_141692240(longlong *param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  
  lVar1 = (**(code **)(*param_1 + 0x148))();
  if (*(int *)(lVar1 + 0x6e0) != 2) {
    lVar1 = (**(code **)(*param_1 + 0x148))(param_1);
    if (*(int *)(lVar1 + 0x6e0) != 4) {
      lVar1 = FUN_140d2ba50();
      if (lVar1 != 0) {
        lVar1 = FUN_1415458f0(lVar1);
        if (lVar1 != 0) {
          uVar2 = FUN_140d2ba50();
          uVar2 = FUN_1415458f0(uVar2);
          FUN_1416f7bd0(uVar2,9,8,0);
          uVar2 = FUN_140d2ba50();
          uVar2 = FUN_1415458f0(uVar2);
          FUN_1416f2c70(uVar2);
          return;
        }
      }
    }
  }
  return;
}



// ---- FUN_141694d30 @ 141694d30 ----

void FUN_141694d30(longlong *param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  longlong local_28 [4];
  
  if (((char)param_1[0x86] == '\0') && (*(char *)((longlong)param_1 + 0x431) == '\0')) {
    FUN_141a88520();
    if (param_1[0x76] == 0) {
      lVar1 = FUN_140d2ba50();
      if (lVar1 != 0) {
        lVar1 = FUN_1415458f0(lVar1);
        param_1[0x76] = lVar1;
      }
    }
    if (param_1[0x76] != 0) {
      FUN_1416f7bd0(param_1[0x76],0x23,8,0);
    }
    (**(code **)(*param_1 + 0x468))(param_1,1);
    if (param_1[0x76] != 0) {
      FUN_1416f2c70();
    }
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x74],0,1,0,DAT_1450d6c88);
    *(undefined2 *)(param_1 + 0x86) = 0x101;
    local_28[0] = 0;
    local_28[1] = 0xe;
    FUN_1407e2ee0(local_28,0);
    FUN_141dd1c10(local_28[0],L"title_summary",0x1c);
    uVar2 = FUN_1412b5c20(local_28 + 2,local_28);
    FUN_1416fb690(param_1[0x7a],uVar2,0);
    if (local_28[2] != 0) {
      FUN_141de9200();
    }
    if (local_28[0] != 0) {
      FUN_141de9200();
    }
                    /* WARNING: Could not recover jumptable at 0x000141694e73. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*(longlong *)param_1[0x85] + 0x250))((longlong *)param_1[0x85],1);
    return;
  }
  return;
}



// ---- FUN_1416951a0 @ 1416951a0 ----

void FUN_1416951a0(longlong *param_1,undefined1 param_2)

{
  longlong lVar1;
  char cVar2;
  undefined1 uVar3;
  longlong lVar4;
  longlong lVar5;
  undefined8 uVar6;
  
  if (param_1[0x89] != 0) {
    *(undefined1 *)((longlong)param_1 + 0x482) = param_2;
    *(undefined4 *)((longlong)param_1 + 0x494) = 0xffffffff;
    if (((char)param_1[0x90] == '\0') &&
       (((param_1[0x7f] == 0 || (cVar2 = FUN_1415c1770(), cVar2 == '\0')) &&
        (*(char *)((longlong)param_1 + 0x481) == '\0')))) {
      *(undefined1 *)(param_1 + 0x93) = 1;
      if ((param_1[0x8d] == 0) && (lVar4 = FUN_140d2ba50(), lVar4 != 0)) {
        lVar4 = FUN_1415458f0(lVar4);
        param_1[0x8d] = lVar4;
      }
      if (param_1[0x8d] != 0) {
        FUN_1416f7bd0(param_1[0x8d],3,8,0);
      }
      if (param_1[0x7f] != 0) {
        FUN_1415bd310();
      }
      FUN_14169efd0(param_1);
      uVar3 = FUN_141479380(0);
      *(undefined1 *)(param_1 + 0x278) = uVar3;
      FUN_1416a2520(param_1);
      FUN_141a88520(param_1);
      (**(code **)(*param_1 + 0x468))(param_1,1);
      (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x83],0,1,0,DAT_1450d6c88);
      *(undefined2 *)(param_1 + 0x90) = 0x101;
      _guard_check_icall(8);
      if ((*(char *)((longlong)param_1 + 0x482) == '\x01') && (param_1[0x8f] == 0)) {
        if (param_1[0x8e] != 0) {
          lVar5 = FUN_1417bd0d0();
          lVar4 = param_1[0x8e];
          for (lVar1 = lVar4; lVar1 != 0; lVar1 = *(longlong *)(lVar1 + 0x30)) {
            if (lVar1 == lVar5) goto LAB_141695310;
          }
        }
        lVar4 = 0;
LAB_141695310:
        uVar6 = (**(code **)(*param_1 + 0x148))(param_1);
        lVar4 = FUN_140d18890(uVar6,lVar4);
        param_1[0x8f] = lVar4;
        if (lVar4 != 0) {
          FUN_141a58910(lVar4,0xa0);
          return;
        }
      }
    }
  }
  return;
}



// ---- FUN_141695bd0 @ 141695bd0 ----

void FUN_141695bd0(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  
  lVar1 = thunk_FUN_140d2ba50();
  if (lVar1 != 0) {
    uVar2 = FUN_1415458f0(lVar1);
    FUN_1416f7bd0(uVar2,0x13,8,0);
    uVar2 = FUN_1415458f0(lVar1);
    FUN_1416f2c70(uVar2);
  }
  if (*(longlong *)(param_1 + 0x410) != 0) {
    FUN_1416f2cc0();
  }
  return;
}



// ---- FUN_1416b6b00 @ 1416b6b00 ----

void FUN_1416b6b00(longlong param_1)

{
  int iVar1;
  
  if (*(longlong *)(param_1 + 0x4d0) != 0) {
    FUN_1416f7bd0(*(longlong *)(param_1 + 0x4d0),7,8,1);
  }
  iVar1 = *(int *)(param_1 + 0x4e4);
  if (((-1 < iVar1) && (iVar1 < *(int *)(param_1 + 0x430))) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x428) + (longlong)iVar1 * 8) != 0)) {
    FUN_1416c71b0();
    return;
  }
  return;
}



// ---- FUN_1416b6b60 @ 1416b6b60 ----

void FUN_1416b6b60(longlong param_1)

{
  int iVar1;
  
  if (*(longlong *)(param_1 + 0x4d0) != 0) {
    FUN_1416f7bd0(*(longlong *)(param_1 + 0x4d0),0,8,1);
  }
  iVar1 = *(int *)(param_1 + 0x4e8);
  if (((-1 < iVar1) && (iVar1 < *(int *)(param_1 + 0x440))) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x438) + (longlong)iVar1 * 8) != 0)) {
    FUN_1416c71b0();
    return;
  }
  return;
}



// ---- FUN_1416c38a0 @ 1416c38a0 ----

void FUN_1416c38a0(longlong param_1)

{
  longlong *plVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  lVar2 = FUN_140d2ba50();
  if (lVar2 != 0) {
    uVar3 = FUN_140d2ba50();
    lVar2 = FUN_1415458f0(uVar3);
    if (lVar2 != 0) {
      uVar3 = FUN_140d2ba50();
      uVar3 = FUN_1415458f0(uVar3);
      FUN_1416f7bd0(uVar3,9,8,0);
      uVar3 = FUN_140d2ba50();
      uVar3 = FUN_1415458f0(uVar3);
      FUN_1416f2c70(uVar3);
    }
  }
  plVar1 = *(longlong **)(param_1 + 0x420);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x250))(plVar1,3);
    (**(code **)(*plVar1 + 0x2e0))(plVar1,plVar1[0x73],0,1,0,DAT_1450d6c88);
  }
  if (*(longlong *)(param_1 + 0x428) != 0) {
    FUN_1416f2cc0();
  }
  if (*(longlong *)(param_1 + 0x430) != 0) {
    FUN_1416f2cc0();
  }
  return;
}



// ---- FUN_1416c7860 @ 1416c7860 ----

void FUN_1416c7860(longlong *param_1)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined1 local_res8 [32];
  
  if (((char)param_1[0xf0] == '\0') && (*(char *)((longlong)param_1 + 0x781) == '\0')) {
    *(undefined1 *)(param_1 + 0xf2) = 1;
    if (param_1[0xec] == 0) {
      lVar1 = FUN_140d2ba50();
      if (lVar1 != 0) {
        lVar1 = FUN_1415458f0(lVar1);
        param_1[0xec] = lVar1;
      }
    }
    if (param_1[0xec] != 0) {
      FUN_1416f7bd0(param_1[0xec],0x10,8,0);
    }
    if (param_1[0xce] != 0) {
      FUN_1415bd350();
    }
    FUN_1416d1f30(param_1);
    FUN_1416d78b0(param_1);
    FUN_1416d4940(param_1);
    FUN_1416c1500(param_1);
    (**(code **)(*param_1 + 0x468))(param_1,1);
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0xe4],0,1,0,DAT_1450d6c88);
    *(undefined2 *)(param_1 + 0xf0) = 0x101;
    (**(code **)(*(longlong *)param_1[0xce] + 0x498))
              ((longlong *)param_1[0xce],param_1,DAT_145988348);
    puVar2 = (undefined8 *)FUN_141eb3410(local_res8,"UpdateDetail",1);
    FUN_1415c5ef0(param_1[0xce],param_1,*puVar2);
    (**(code **)(*(longlong *)param_1[0xd0] + 0x498))
              ((longlong *)param_1[0xd0],param_1,DAT_145988348);
                    /* WARNING: Could not recover jumptable at 0x0001416c79b6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*(longlong *)param_1[0xd1] + 0x498))
              ((longlong *)param_1[0xd1],param_1,DAT_145988348);
    return;
  }
  return;
}



// ---- FUN_1416c79d0 @ 1416c79d0 ----

void FUN_1416c79d0(longlong *param_1,undefined1 param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  
  if (((char)param_1[0x81] == '\0') && (*(char *)((longlong)param_1 + 0x409) == '\0')) {
    *(undefined1 *)((longlong)param_1 + 0x40a) = param_2;
    *(undefined1 *)((longlong)param_1 + 0x41c) = 1;
    if (param_1[0x7e] == 0) {
      lVar1 = FUN_140d2ba50();
      if (lVar1 != 0) {
        lVar1 = FUN_1415458f0(lVar1);
        param_1[0x7e] = lVar1;
      }
    }
    if (param_1[0x7e] != 0) {
      uVar2 = 4;
      if (*(char *)((longlong)param_1 + 0x40a) == '\0') {
        uVar2 = 0x12;
      }
      FUN_1416f7bd0(param_1[0x7e],uVar2,8);
    }
    (**(code **)(*param_1 + 0x468))(param_1,1);
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x7c],0,1,0,DAT_1450d6c88);
    *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | 8;
    *(undefined2 *)(param_1 + 0x81) = 0x101;
    *(undefined1 *)(param_1 + 0x85) = 1;
    *(undefined4 *)((longlong)param_1 + 0x42c) = 1;
  }
  return;
}



// ---- FUN_1416c7ab0 @ 1416c7ab0 ----

void FUN_1416c7ab0(longlong *param_1,longlong param_2,char param_3)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  longlong local_res8;
  
  if (((char)param_1[0x85] == '\0') && (*(char *)((longlong)param_1 + 0x429) == '\0')) {
    if (param_2 == 0) {
      if (param_3 == '\0') {
        uVar3 = 1;
      }
      else {
        *(undefined4 *)(param_1 + 0x86) = 0;
        FUN_1416b6c90(&local_res8,param_3);
        if ((0 < (int)param_1[0x9b]) && (param_1[0x99] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0;
        }
        if ((0 < (int)param_1[0xa1]) && (param_1[0x9f] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 1;
        }
        if ((0 < (int)param_1[0xa7]) && (param_1[0xa5] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 2;
        }
        if ((0 < (int)param_1[0xad]) && (param_1[0xab] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 3;
        }
        if ((0 < (int)param_1[0xb3]) && (param_1[0xb1] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 4;
        }
        if ((0 < (int)param_1[0xb9]) && (param_1[0xb7] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 5;
        }
        if ((0 < (int)param_1[0xbf]) && (param_1[0xbd] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 6;
        }
        if ((0 < (int)param_1[0xc5]) && (param_1[0xc3] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 7;
        }
        if ((0 < (int)param_1[0xcb]) && (param_1[0xc9] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 8;
        }
        if ((0 < (int)param_1[0xd1]) && (param_1[0xcf] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 9;
        }
        if ((0 < (int)param_1[0xd7]) && (param_1[0xd5] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 10;
        }
        if ((0 < (int)param_1[0xdd]) && (param_1[0xdb] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0xb;
        }
        if ((0 < (int)param_1[0xe3]) && (param_1[0xe1] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0xc;
        }
        if ((0 < (int)param_1[0xe9]) && (param_1[0xe7] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0xd;
        }
        if ((0 < (int)param_1[0xef]) && (param_1[0xed] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0xe;
        }
        if ((0 < (int)param_1[0xf5]) && (param_1[0xf3] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0xf;
        }
        if ((0 < (int)param_1[0xfb]) && (param_1[0xf9] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x10;
        }
        if ((0 < (int)param_1[0x101]) && (param_1[0xff] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x11;
        }
        if ((0 < (int)param_1[0x107]) && (param_1[0x105] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x12;
        }
        if ((0 < (int)param_1[0x10d]) && (param_1[0x10b] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x13;
        }
        if ((0 < (int)param_1[0x113]) && (param_1[0x111] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x14;
        }
        if ((0 < (int)param_1[0x119]) && (param_1[0x117] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x15;
        }
        if ((0 < (int)param_1[0x11f]) && (param_1[0x11d] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x16;
        }
        if ((0 < (int)param_1[0x125]) && (param_1[0x123] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x17;
        }
        if ((0 < (int)param_1[299]) && (param_1[0x129] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x18;
        }
        if ((0 < (int)param_1[0x131]) && (param_1[0x12f] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x19;
        }
        if ((0 < (int)param_1[0x137]) && (param_1[0x135] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x1a;
        }
        if ((0 < (int)param_1[0x13d]) && (param_1[0x13b] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x1b;
        }
        if ((0 < (int)param_1[0x143]) && (param_1[0x141] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x1c;
        }
        if ((0 < (int)param_1[0x149]) && (param_1[0x147] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x1d;
        }
        if ((0 < (int)param_1[0x14f]) && (param_1[0x14d] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x1e;
        }
        if ((0 < (int)param_1[0x155]) && (param_1[0x153] == local_res8)) {
          *(undefined4 *)((longlong)param_1 + 0x42c) = 0x1f;
        }
        uVar3 = 0;
      }
    }
    else {
      param_1[0x84] = param_2;
      if (param_1[0x83] == 0) {
        lVar1 = FUN_140d2ba50();
        if (lVar1 != 0) {
          lVar1 = FUN_1415458f0(lVar1);
          param_1[0x83] = lVar1;
        }
      }
      if (param_1[0x83] != 0) {
        FUN_1416f7bd0(param_1[0x83],0x11,8,0);
      }
      if (param_1[0x79] != 0) {
        FUN_1415bd310();
      }
      puVar2 = (undefined8 *)FUN_1416b6c90(&local_res8,param_3);
      FUN_1416d3000(param_1,*puVar2);
      uVar3 = 0;
    }
    FUN_1416d7a20(param_1,uVar3);
    (**(code **)(*param_1 + 0x468))(param_1,1);
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x7d],0,1,0,DAT_1450d6c88);
    *(undefined2 *)(param_1 + 0x85) = 0x101;
  }
  return;
}



// ---- FUN_1416d4720 @ 1416d4720 ----

void FUN_1416d4720(longlong param_1)

{
  int iVar1;
  
  if (*(longlong *)(param_1 + 0x4d0) != 0) {
    FUN_1416f7bd0(*(longlong *)(param_1 + 0x4d0),0x1a,8,1);
  }
  iVar1 = *(int *)(param_1 + 0x4ec);
  if (((-1 < iVar1) && (iVar1 < *(int *)(param_1 + 0x450))) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x448) + (longlong)iVar1 * 8) != 0)) {
    FUN_1416c71b0();
    return;
  }
  return;
}



// ---- FUN_1417c2960 @ 1417c2960 ----

void FUN_1417c2960(undefined8 param_1,longlong param_2)

{
  undefined1 local_res10 [8];
  undefined1 local_res20 [8];
  int local_18 [4];
  
  local_res20[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res20);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res20);
  }
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  local_18[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_18);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_18);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416f7bd0(param_1,local_res20[0],local_res10[0],local_18[0] != 0);
  return;
}



// ---- FUN_1419ee500 @ 1419ee500 ----

void FUN_1419ee500(void)

{
  longlong lVar1;
  longlong lVar2;
  longlong *plVar3;
  
  lVar1 = FUN_140d2ba50();
  if (lVar1 != 0) {
    lVar2 = FUN_1415458f0(lVar1);
    if (lVar2 != 0) {
      FUN_1416f7bd0(lVar2,7,8,0);
    }
    plVar3 = (longlong *)FUN_141545800(lVar1);
    if (plVar3 != (longlong *)0x0) {
      (**(code **)(*plVar3 + 0x4d8))(plVar3);
                    /* WARNING: Could not recover jumptable at 0x0001419ee557. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(*plVar3 + 0x4e0))(plVar3);
      return;
    }
  }
  return;
}



// ---- FUN_141d008b0 @ 141d008b0 ----

longlong * FUN_141d008b0(undefined1 param_1,undefined4 param_2)

{
  char cVar1;
  longlong *plVar2;
  
  plVar2 = (longlong *)FUN_141cee000();
  if (plVar2 != (longlong *)0x0) {
    cVar1 = FUN_1429ab210(plVar2);
    if (cVar1 != '\0') {
      FUN_1429d9d20(plVar2);
    }
    (**(code **)(*plVar2 + 0x2b8))(plVar2,param_2);
    FUN_1416f7bd0(plVar2,param_1,8,0);
    return plVar2;
  }
  return (longlong *)0x0;
}



// ---- FUN_141ce4950 @ 141ce4950 ----

void FUN_141ce4950(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  longlong *plVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  int iVar7;
  undefined1 local_res8 [8];
  longlong local_18 [2];
  
  FUN_141a55c30();
  lVar2 = FUN_142058d50(param_1 + 0x3f8);
  if ((lVar2 != 0) &&
     (plVar3 = (longlong *)FUN_142058d50(param_1 + 0x3f8), plVar3 != (longlong *)0x0)) {
    (**(code **)(*plVar3 + 0x250))(plVar3,1);
  }
  iVar7 = 0;
  if (0 < *(int *)(param_1 + 0x408)) {
    do {
      lVar2 = FUN_142058d50(*(longlong *)(param_1 + 0x400) + (longlong)iVar7 * 8);
      if ((lVar2 != 0) &&
         (plVar3 = (longlong *)FUN_142058d50(*(longlong *)(param_1 + 0x400) + (longlong)iVar7 * 8),
         plVar3 != (longlong *)0x0)) {
        (**(code **)(*plVar3 + 0x250))(plVar3,1);
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < *(int *)(param_1 + 0x408));
  }
  FUN_141eb3490(local_res8,L"C_Deck_Info",1);
  uVar4 = FUN_141a67ba0(local_18,local_res8);
  lVar2 = FUN_142058d50(param_1 + 0x438);
  if (lVar2 != 0) {
    uVar5 = FUN_142058d50(param_1 + 0x438);
    FUN_141a87aa0(uVar5,uVar4);
    uVar4 = FUN_142058d50(param_1 + 0x438);
    FUN_141a54c00(uVar4);
  }
  if (local_18[0] != 0) {
    FUN_141de9200();
  }
  lVar2 = FUN_142058d50(param_1 + 0x450);
  if (lVar2 != 0) {
    FUN_141eb3490(local_res8,L"C_Deck_Setting",1);
    uVar4 = FUN_142058d50(param_1 + 0x450);
    uVar5 = FUN_141a67ba0(local_18,local_res8);
    FUN_141cc0820(uVar4,uVar5);
    if (local_18[0] != 0) {
      FUN_141de9200();
    }
  }
  FUN_141eb3490(local_res8,L"C_Deck_Main_Num",1);
  uVar4 = FUN_141a67ba0(local_18,local_res8);
  lVar2 = FUN_142058d50(param_1 + 0x458);
  if (lVar2 != 0) {
    uVar5 = FUN_142058d50(param_1 + 0x458);
    FUN_141a87aa0(uVar5,uVar4);
    uVar4 = FUN_142058d50(param_1 + 0x458);
    FUN_141a54c00(uVar4);
  }
  if (local_18[0] != 0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_res8,L"C_Deck_Side_Num",1);
  uVar4 = FUN_141a67ba0(local_18,local_res8);
  lVar2 = FUN_142058d50(param_1 + 0x488);
  if (lVar2 != 0) {
    uVar5 = FUN_142058d50(param_1 + 0x488);
    FUN_141a87aa0(uVar5,uVar4);
    uVar4 = FUN_142058d50(param_1 + 0x488);
    FUN_141a54c00(uVar4);
  }
  if (local_18[0] != 0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_res8,L"C_Deck_Graph_Card_Num",1);
  uVar4 = FUN_141a67ba0(local_18,local_res8);
  lVar2 = FUN_142058d50(param_1 + 0x468);
  if (lVar2 != 0) {
    uVar5 = FUN_142058d50(param_1 + 0x468);
    FUN_141a87aa0(uVar5,uVar4);
    uVar4 = FUN_142058d50(param_1 + 0x468);
    FUN_141a54c00(uVar4);
  }
  if (local_18[0] != 0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_res8,L"C_Deck_Graph_Cost",1);
  uVar4 = FUN_141a67ba0(local_18,local_res8);
  lVar2 = FUN_142058d50(param_1 + 0x470);
  if (lVar2 != 0) {
    uVar5 = FUN_142058d50(param_1 + 0x470);
    FUN_141a87aa0(uVar5,uVar4);
    uVar4 = FUN_142058d50(param_1 + 0x470);
    FUN_141a54c00(uVar4);
  }
  if (local_18[0] != 0) {
    FUN_141de9200();
  }
  plVar3 = (longlong *)FUN_141cee000();
  if (plVar3 == (longlong *)0x0) {
    plVar3 = (longlong *)0x0;
  }
  else {
    cVar1 = FUN_1429ab210(plVar3);
    if (cVar1 != '\0') {
      FUN_1429d9d20(plVar3);
    }
    (**(code **)(*plVar3 + 0x2b8))(plVar3,0x96);
    FUN_1416f7bd0(plVar3,0x2b,8,0);
  }
  FUN_1420575e0(param_1 + 0x498,plVar3);
  lVar2 = FUN_142058d50(param_1 + 0x420);
  if (lVar2 != 0) {
    lVar2 = FUN_142058d50(param_1 + 0x420);
    *(longlong *)(lVar2 + 0x3d8) = param_1;
  }
  puVar6 = (undefined8 *)FUN_141eb3490(local_res8,L"Wheel",1);
  FUN_141a7c7d0(param_1,param_1,*puVar6);
  return;
}



// ---- FUN_141ce6e60 @ 141ce6e60 ----

void FUN_141ce6e60(longlong param_1)

{
  char cVar1;
  longlong *plVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong lVar5;
  
  plVar2 = (longlong *)FUN_142058d50(param_1 + 0x3fc);
  if (plVar2 != (longlong *)0x0) {
    (**(code **)(*plVar2 + 0x250))(plVar2,0);
  }
  lVar4 = param_1 + 0x3d4;
  lVar5 = 5;
  do {
    uVar3 = FUN_142058d50(lVar4);
    FUN_141cff000(uVar3,0,0);
    lVar4 = lVar4 + 8;
    lVar5 = lVar5 + -1;
  } while (lVar5 != 0);
  plVar2 = (longlong *)FUN_141cee000();
  if (plVar2 == (longlong *)0x0) {
    plVar2 = (longlong *)0x0;
  }
  else {
    cVar1 = FUN_1429ab210(plVar2);
    if (cVar1 != '\0') {
      FUN_1429d9d20(plVar2);
    }
    (**(code **)(*plVar2 + 0x2b8))(plVar2,0x6e);
    FUN_1416f7bd0(plVar2,0x29,8,0);
  }
  FUN_1420575e0(param_1 + 0x404,plVar2);
  return;
}



// ---- FUN_14169ed10 @ 14169ed10 ----

void FUN_14169ed10(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  
  if (*(longlong *)(param_1 + 0x490) == 0) {
    lVar1 = FUN_140d2ba50();
    if (lVar1 != 0) {
      uVar2 = FUN_1415458f0(lVar1);
      *(undefined8 *)(param_1 + 0x490) = uVar2;
    }
  }
  if (*(longlong *)(param_1 + 0x490) != 0) {
    FUN_1416f7bd0(*(longlong *)(param_1 + 0x490),5,8,*(undefined1 *)(param_1 + 0x39c));
    return;
  }
  return;
}



// ---- FUN_141148630 @ 141148630 ----

void FUN_141148630(longlong param_1)

{
  int iVar1;
  char cVar2;
  longlong lVar3;
  undefined8 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  longlong *plVar8;
  bool bVar9;
  undefined1 local_res8 [8];
  undefined1 local_18 [16];
  
  lVar3 = *(longlong *)(param_1 + 0x30);
  if (*(longlong *)(lVar3 + 0x108) != 0) {
    if (*(int *)(lVar3 + 0x138) == 1) {
      if (*(longlong *)(lVar3 + 0x100) != 0) {
        FUN_14153b560(*(longlong *)(lVar3 + 0x108),
                      *(undefined8 *)(*(longlong *)(lVar3 + 0x100) + 0x100));
        lVar3 = *(longlong *)(param_1 + 0x30);
      }
      FUN_1415512b0(*(undefined8 *)(lVar3 + 0x108),
                    *(undefined8 *)(*(longlong *)(lVar3 + 0x100) + 0x100));
      FUN_141551ea0(*(undefined8 *)(*(longlong *)(param_1 + 0x30) + 0x108),
                    *(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x30) + 0x100) + 0x100));
    }
    else if ((*(int *)(lVar3 + 0x138) == 2) && (lVar3 = FUN_1411fc890(), lVar3 != 0)) {
      uVar4 = FUN_142d20f20(lVar3);
      uVar4 = FUN_142d1df00(local_18,uVar4);
      cVar2 = FUN_1419d3d20(uVar4);
      lVar3 = *(longlong *)(param_1 + 0x30);
      iVar6 = 0;
      iVar5 = -1;
      if (0 < *(int *)(lVar3 + 0x220)) {
        plVar8 = *(longlong **)(lVar3 + 0x218);
        iVar7 = iVar6;
        do {
          iVar1 = iVar5;
          if ((*(char *)(*plVar8 + 0x10) == cVar2) && (iVar7 = iVar7 + 1, iVar1 = iVar6, 1 < iVar7))
          break;
          iVar5 = iVar1;
          iVar6 = iVar6 + 1;
          plVar8 = plVar8 + 1;
        } while (iVar6 < *(int *)(lVar3 + 0x220));
        if ((-1 < iVar5) && (iVar7 == 1)) {
          FUN_14153b560(*(undefined8 *)(lVar3 + 0x108),
                        *(undefined8 *)((*(longlong **)(lVar3 + 0x218))[iVar5] + 8));
        }
      }
    }
    *(undefined4 *)(*(longlong *)(*(longlong *)(param_1 + 0x30) + 0x108) + 0x5a8) =
         *(undefined4 *)(*(longlong *)(param_1 + 0x30) + 0x138);
    uVar4 = *(undefined8 *)(*(longlong *)(param_1 + 0x30) + 0x108);
    if (*(char *)(param_1 + 0x3f1) == '\0') {
      bVar9 = false;
    }
    else {
      cVar2 = FUN_141547910(uVar4,local_res8);
      bVar9 = cVar2 != '\0';
      uVar4 = *(undefined8 *)(*(longlong *)(param_1 + 0x30) + 0x108);
    }
    FUN_14154c5f0(uVar4,bVar9);
  }
  lVar3 = FUN_140d2ba50();
  if ((lVar3 != 0) && (lVar3 = FUN_1415458f0(lVar3), lVar3 != 0)) {
    uVar4 = 0x28;
    if (*(char *)(param_1 + 0x3f1) == '\0') {
      uVar4 = 0x25;
    }
    FUN_1416f7bd0(lVar3,uVar4,8,1);
    return;
  }
  return;
}



// ---- FUN_1412e1450 @ 1412e1450 ----

void FUN_1412e1450(longlong param_1)

{
  FUN_141287180(*(undefined8 *)(param_1 + 0x100));
  FUN_1416f7bd0(*(undefined8 *)(param_1 + 0x108),0x12,8,1);
  return;
}



// ---- FUN_1416f7b30 @ 1416f7b30 ----

void FUN_1416f7b30(undefined8 param_1,undefined8 param_2,undefined1 param_3)

{
  undefined1 uVar1;
  
  uVar1 = 8;
  switch(param_3) {
  case 1:
    uVar1 = 6;
    break;
  case 2:
    FUN_1416f7bd0(param_1,param_2,0);
    return;
  case 3:
    FUN_1416f7bd0(param_1,param_2,2);
    return;
  case 4:
    FUN_1416f7bd0(param_1,param_2,1);
    return;
  case 5:
    FUN_1416f7bd0(param_1,param_2,3);
    return;
  case 6:
    FUN_1416f7bd0(param_1,param_2,4);
    return;
  case 7:
    FUN_1416f7bd0(param_1,param_2,5);
    return;
  }
  FUN_1416f7bd0(param_1,param_2,uVar1);
  return;
}



// ---- FUN_141521250 @ 141521250 ----

void FUN_141521250(longlong param_1,char param_2)

{
  longlong lVar1;
  
  if (param_2 == '\x01') {
    lVar1 = FUN_140d2ba50();
    if (lVar1 != 0) {
      lVar1 = FUN_1415458f0(lVar1);
      if (lVar1 != 0) {
        FUN_1416f7bd0(lVar1,0x1d,8,1);
        return;
      }
    }
  }
  else if (param_2 == '\x02') {
    FUN_141524db0(*(undefined8 *)(param_1 + 0x608));
    return;
  }
  return;
}



