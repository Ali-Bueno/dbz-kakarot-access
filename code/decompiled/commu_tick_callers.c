// ################ callers of 0x1414e3170 (2) ################
// ======== FUN_1414c7de0 @ 1414c7de0 ========

void FUN_1414c7de0(longlong param_1,undefined8 param_2,float param_3)

{
  void *pvVar1;
  float fVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  undefined8 *puVar8;
  longlong *plVar9;
  undefined8 uVar10;
  void *pvVar11;
  float fVar12;
  undefined8 local_res8;
  void *local_88;
  undefined8 local_80;
  undefined **local_78;
  longlong local_70;
  longlong local_60;
  longlong local_50;
  longlong local_40;
  
  if (*(char *)(param_1 + 0x4e9) == '\0') {
    cVar3 = FUN_1414f8320(*(undefined8 *)(param_1 + 0x4a0));
    if (cVar3 == '\0') {
      return;
    }
    *(undefined1 *)(param_1 + 0x4e9) = 1;
    (**(code **)(**(longlong **)(param_1 + 0x4a0) + 0x250))(*(longlong **)(param_1 + 0x4a0),3);
    plVar9 = *(longlong **)(param_1 + 0x4e0);
    if (plVar9 != (longlong *)0x0) {
      (**(code **)(*plVar9 + 0x250))(plVar9,3);
      (**(code **)(**(longlong **)(param_1 + 0x4e0) + 0x2b0))
                (*(longlong **)(param_1 + 0x4e0),
                 *(undefined8 *)
                  (*(longlong *)(param_1 + 0x4a0) + 0x590 +
                  (ulonglong)*(byte *)(param_1 + 0x409) * 8),0);
    }
    FUN_1414fbf30(*(undefined8 *)(param_1 + 0x4a0),*(undefined1 *)(param_1 + 0x409),1);
    *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) & 0xf7;
    *(undefined4 *)(param_1 + 0x500) = 1;
    *(undefined4 *)(param_1 + 0x508) = 8;
    return;
  }
  if (*(char *)(param_1 + 0x4ef) != '\0') {
    if (0 < *(int *)(param_1 + 0x178)) {
      return;
    }
    *(undefined1 *)(param_1 + 0x4ef) = 0;
    FUN_1414c91b0(param_1,*(undefined1 *)(param_1 + 0x50c));
    return;
  }
  iVar4 = *(int *)(param_1 + 0x500);
  if (iVar4 == 0xc) {
    cVar3 = FUN_1416c0a50();
    if (cVar3 == '\0') {
      return;
    }
    if (*(char *)(param_1 + 0x4f1) == '\0') {
      if (*(char *)(param_1 + 0x4ed) != '\0') {
        FUN_1414e1c50(param_1,*(undefined1 *)(param_1 + 0x409));
        *(undefined1 *)(param_1 + 0x4ed) = 0;
        return;
      }
      FUN_1414c8c70(param_1,1);
      FUN_1414c8a40(param_1,1);
      *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 8;
      *(undefined4 *)(param_1 + 0x500) = 7;
      *(undefined4 *)(param_1 + 0x508) = 0;
      return;
    }
    if (*(char *)(param_1 + 0x4ed) != '\0') {
      FUN_1414e1c50(param_1,*(undefined1 *)(param_1 + 0x409));
      *(undefined1 *)(param_1 + 0x4ed) = 0;
      return;
    }
    lVar6 = FUN_1411fd780();
    if (lVar6 == 0) {
      return;
    }
    local_80 = 0x24;
    local_88 = (void *)0x0;
    FUN_1407e2ee0(&local_88,0);
    FUN_1407e4a20(local_88,0x24,"Comm_TutorialEvent_EmptyPanetChoose",0x24,0x3f);
    lVar7 = FUN_1411fd400(&local_78,&local_88);
    pvVar1 = *(void **)(lVar7 + 0x38);
    *(undefined8 *)(lVar7 + 0x38) = 0;
    iVar4 = *(int *)(lVar7 + 0x40);
    *(undefined8 *)(lVar7 + 0x40) = 0;
    if (local_40 != 0) {
      FUN_141de9200();
    }
    if (local_50 != 0) {
      FUN_141de9200();
    }
    if (local_60 != 0) {
      FUN_141de9200();
    }
    if (local_70 != 0) {
      FUN_141de9200();
    }
    local_78 = &PTR_FUN_14397c0d8;
    if (local_88 != (void *)0x0) {
      FUN_141de9200();
    }
    local_88 = (void *)0x0;
    local_80 = CONCAT44(local_80._4_4_,iVar4);
    if (iVar4 == 0) {
      local_80 = 0;
    }
    else {
      FUN_1407f1420(&local_88,iVar4,0);
      memcpy(local_88,pvVar1,(longlong)iVar4 * 2);
    }
    FUN_1416fcc70(lVar6,&local_88,0,1);
    *(undefined4 *)(param_1 + 0x500) = 0xe;
    if (pvVar1 == (void *)0x0) {
      return;
    }
    FUN_141de9200(pvVar1);
    return;
  }
  if (iVar4 == 0xe) {
    lVar6 = FUN_1411fd780();
    if ((lVar6 != 0) && (cVar3 = FUN_1416ed9d0(lVar6), cVar3 != '\0')) {
      if (*(char *)(param_1 + 0x4ed) != '\0') {
        FUN_1414e1c50(param_1,*(undefined1 *)(param_1 + 0x409));
        *(undefined1 *)(param_1 + 0x4ed) = 0;
        return;
      }
      FUN_1414c8c70(param_1,6);
      FUN_1414c8a40(param_1,1);
      *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 8;
      *(undefined4 *)(param_1 + 0x500) = 7;
      *(undefined4 *)(param_1 + 0x508) = 0;
    }
  }
  else if (((iVar4 == 0xd) && (cVar3 = FUN_1416c0a50(), fVar2 = DAT_14391b46c, cVar3 != '\0')) &&
          (fVar12 = param_3 + *(float *)(param_1 + 0x634), *(float *)(param_1 + 0x634) = fVar12,
          fVar2 < fVar12)) {
    *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 8;
    *(undefined4 *)(param_1 + 0x634) = 0;
    *(undefined4 *)(param_1 + 0x500) = 7;
    *(undefined4 *)(param_1 + 0x508) = 0;
    *(undefined1 *)(*(longlong *)(param_1 + 0x5f8) + 0x3b) = 1;
    FUN_1414d5a90(param_1);
  }
  iVar4 = *(int *)(param_1 + 0x508);
  if (iVar4 == 1) {
    if (0 < *(int *)(*(longlong *)(param_1 + 0x4a8) + 0x178)) goto LAB_1414c8232;
    uVar10 = 1;
  }
  else if (iVar4 == 2) {
    if (0 < *(int *)(*(longlong *)(param_1 + 0x4a8) + 0x178)) goto LAB_1414c8232;
    uVar10 = 2;
  }
  else if (iVar4 == 0xc) {
    cVar3 = FUN_14168ff80(*(undefined8 *)(param_1 + 0x628));
    if (cVar3 != '\x01') goto LAB_1414c8232;
    uVar10 = 0xc;
  }
  else if (iVar4 == 0xd) {
    cVar3 = FUN_1414f9280(*(undefined8 *)(param_1 + 0x4a0));
    if (cVar3 != '\0') goto LAB_1414c8232;
    uVar10 = 0xd;
  }
  else {
    if ((iVar4 != 0xe) || (cVar3 = FUN_1416a3080(*(undefined8 *)(param_1 + 0x628)), cVar3 != '\0'))
    goto LAB_1414c8232;
    uVar10 = 0xe;
  }
  FUN_1414d6380(param_1,uVar10);
LAB_1414c8232:
  iVar4 = *(int *)(param_1 + 0x500);
  if (iVar4 == 0x11) {
    lVar6 = FUN_1411fd780();
    if ((lVar6 != 0) && (cVar3 = FUN_1416ed9d0(lVar6), cVar3 != '\0')) {
      if (*(char *)(param_1 + 0x4f1) == '\0') {
        *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 8;
        *(undefined4 *)(param_1 + 0x500) = 7;
        *(undefined4 *)(param_1 + 0x508) = 0;
      }
      else {
        puVar8 = (undefined8 *)FUN_141eb3490(&local_res8,L"TUTO_MNU_24_01",1);
        cVar3 = FUN_1416bf790(*puVar8);
        if (cVar3 == '\0') {
          puVar8 = (undefined8 *)FUN_141eb3490(&local_res8,L"TUTO_MNU_24_01",1);
          FUN_1416c6980(*puVar8,1);
        }
        *(undefined2 *)(param_1 + 0x4f1) = 0x100;
        *(undefined1 *)(param_1 + 0x4f3) = 0;
        *(undefined4 *)(param_1 + 0x500) = 0xd;
        *(undefined4 *)(param_1 + 0x634) = 0;
      }
    }
  }
  else if (iVar4 == 0x10) {
    if (((*(longlong *)(param_1 + 0x518) != 0) &&
        (*(int *)(*(longlong *)(param_1 + 0x518) + 0x178) < 1)) &&
       ((cVar3 = FUN_14170e070(), cVar3 == '\0' &&
        (param_3 = param_3 + *(float *)(param_1 + 0x634), *(float *)(param_1 + 0x634) = param_3,
        *(float *)(param_1 + 0x630) <= param_3)))) {
      *(undefined4 *)(param_1 + 0x634) = 0;
      FUN_1414d79e0(param_1);
    }
  }
  else if (iVar4 == 7) {
    FUN_1414e3170(param_1);
    cVar3 = FUN_1414f91e0(*(undefined8 *)(param_1 + 0x4a0));
    if (((cVar3 != '\0') && (cVar3 = FUN_1414f9360(*(undefined8 *)(param_1 + 0x4b0)), cVar3 == '\0')
        ) && (cVar3 = FUN_1414f93a0(*(undefined8 *)(param_1 + 0x4b0)), cVar3 == '\0')) {
      plVar9 = (longlong *)FUN_1414f7f50(*(undefined8 *)(param_1 + 0x4a0));
      local_res8 = 0;
      if (*plVar9 != 0) {
        uVar10 = FUN_141994880(*(undefined8 *)(param_1 + 0x5f0),plVar9);
        iVar4 = FUN_141991f60(uVar10,*(undefined1 *)(param_1 + 0x50c));
        cVar3 = FUN_141991a70(uVar10);
        if (cVar3 == *(char *)(param_1 + 0x50c)) {
          cVar3 = FUN_141997f10(uVar10);
          if (cVar3 != '\0') {
            local_88 = (void *)0x0;
            local_80 = 0;
            FUN_141991aa0(uVar10,&local_88);
            uVar10 = FUN_141992b50(*(undefined8 *)(param_1 + 0x5f0));
            pvVar1 = (void *)((longlong)local_88 + (longlong)(int)local_80 * 8);
            for (pvVar11 = local_88; pvVar11 != pvVar1; pvVar11 = (void *)((longlong)pvVar11 + 8)) {
              cVar3 = FUN_141998240(uVar10,pvVar11);
              if (cVar3 != '\0') {
                iVar5 = FUN_141992d50(uVar10,pvVar11);
                iVar4 = iVar4 + iVar5;
              }
            }
            if (local_88 != (void *)0x0) {
              FUN_141de9200(local_88);
            }
          }
          FUN_141504620(*(undefined8 *)(param_1 + 0x4b0),*(undefined1 *)(param_1 + 0x50c),iVar4);
          *(undefined4 *)(param_1 + 0x5d8) = 0x80000001;
        }
      }
    }
  }
  return;
}



// ======== FUN_1414d75e0 @ 1414d75e0 ========

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



