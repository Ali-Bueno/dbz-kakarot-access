// ################ callers of 0x1414f2ab0 (7) ################
// ======== FUN_1414f3aa0 @ 1414f3aa0 ========

void FUN_1414f3aa0(longlong param_1)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  longlong local_res10;
  
  if ((((*(char *)(param_1 + 0x79c) != '\0') && (*(int *)(param_1 + 0x508) == 0)) &&
      (*(int *)(param_1 + 0x50c) == 0)) && (*(int *)(param_1 + 0x510) == 0)) {
    uVar2 = FUN_140d25b00();
    uVar3 = FUN_140d2ba50();
    uVar3 = FUN_1415451b0(uVar3);
    if (*(longlong *)(param_1 + 0x7b8) == 0) {
      uVar4 = FUN_141991f50(uVar2,*(undefined1 *)(param_1 + 0x625));
      iVar1 = FUN_1414f2ab0(param_1,0);
      if ((0 < iVar1) && (FUN_141994820(uVar4,&local_res10,iVar1), local_res10 != 0)) {
        uVar2 = FUN_141994880(uVar2,&local_res10);
        FUN_14169b3c0(uVar3,uVar2);
        FUN_1416949b0(uVar3,1);
        *(undefined1 *)(param_1 + 0x7ec) = 1;
      }
    }
    else {
      uVar2 = FUN_141994880();
      FUN_14169b3c0(uVar3,uVar2);
      FUN_1416949b0(uVar3,1);
      *(undefined1 *)(param_1 + 0x7ec) = 1;
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



// ======== FUN_1414e3170 @ 1414e3170 ========

void FUN_1414e3170(longlong param_1)

{
  int iVar1;
  char cVar2;
  int iVar3;
  undefined8 uVar4;
  
  cVar2 = FUN_1414f91e0(*(undefined8 *)(param_1 + 0x4a0));
  if (cVar2 != '\0') {
    FUN_141504ed0(*(undefined8 *)(param_1 + 0x4a0),0);
    return;
  }
  iVar3 = FUN_1414f2ab0(*(undefined8 *)(param_1 + 0x4a0),1);
  iVar1 = *(int *)(param_1 + 0x5d8);
  if (iVar3 == 1) {
    if (iVar1 == 1) goto LAB_1414e31e3;
    FUN_1414c8c70(param_1,3);
    uVar4 = 1;
  }
  else {
    if (iVar3 < 2) {
      if (iVar3 == iVar1) goto LAB_1414e31e3;
      uVar4 = 1;
    }
    else {
      if (iVar3 == iVar1) goto LAB_1414e31e3;
      uVar4 = 2;
    }
    FUN_1414c8c70(param_1,uVar4);
    uVar4 = 0;
  }
  FUN_141504ed0(*(undefined8 *)(param_1 + 0x4a0),uVar4);
LAB_1414e31e3:
  *(int *)(param_1 + 0x5d8) = iVar3;
  return;
}



// ======== FUN_1414d5bb0 @ 1414d5bb0 ========

void FUN_1414d5bb0(longlong param_1)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  
  if (*(int *)(param_1 + 0x500) == 0x10) {
    FUN_1414d79e0();
    *(undefined4 *)(param_1 + 0x634) = 0;
    return;
  }
  if (*(int *)(param_1 + 0x500) == 9) {
    FUN_141692420(*(undefined8 *)(param_1 + 0x628));
    return;
  }
  if (*(char *)(param_1 + 0x4f3) == '\x06') {
    iVar2 = FUN_1414f2ab0(*(undefined8 *)(param_1 + 0x4a0),0);
    if (iVar2 == 1) {
      FUN_141485f00(3,DAT_1450d6c88,0,0);
      return;
    }
    FUN_1415047f0(*(undefined8 *)(param_1 + 0x5f8),1);
    FUN_1414d59c0(param_1);
    *(undefined1 *)(param_1 + 0x4f3) = 7;
    return;
  }
  if (*(char *)(param_1 + 0x4f3) == '\a') {
    iVar2 = FUN_1414f2ab0(*(undefined8 *)(param_1 + 0x4a0),0);
    if ((iVar2 == 2) || (iVar2 - 6U < 3)) {
      FUN_1414f38f0(*(undefined8 *)(param_1 + 0x4a0));
      *(undefined1 *)(param_1 + 0x4f3) = 0;
      return;
    }
    cVar1 = FUN_1414f93b0(*(undefined8 *)(param_1 + 0x4a0));
    if (cVar1 == '\0') {
      FUN_1414c9ea0(param_1);
      return;
    }
  }
  else {
    cVar1 = FUN_1414c9d60();
    if (cVar1 != '\0') {
      cVar1 = FUN_1414f91e0(*(undefined8 *)(param_1 + 0x4a0));
      uVar3 = *(undefined8 *)(param_1 + 0x4a0);
      if (cVar1 == '\0') {
        iVar2 = FUN_1414f2ab0(uVar3,1);
        if (iVar2 < 1) {
          FUN_1414d59c0(param_1);
          return;
        }
        uVar3 = *(undefined8 *)(param_1 + 0x4a0);
      }
      FUN_1414f38f0(uVar3);
      return;
    }
  }
  return;
}



// ======== FUN_1414f38f0 @ 1414f38f0 ========

void FUN_1414f38f0(longlong *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined8 uVar3;
  longlong local_res8 [2];
  
  if (*(char *)((longlong)param_1 + 0x79c) != '\0') {
    local_res8[0] = 0;
    if (param_1[0xf7] != 0) {
      FUN_1414f8ae0();
      return;
    }
    uVar3 = FUN_140d25b00();
    uVar3 = FUN_141991f50(uVar3,*(undefined1 *)((longlong)param_1 + 0x625));
    iVar2 = FUN_1414f2ab0(param_1,0);
    if (0 < iVar2) {
      if ((((iVar2 == 1) || ((int)param_1[0xa1] != 0)) || (*(int *)((longlong)param_1 + 0x50c) != 0)
          ) || ((int)param_1[0xa2] != 0)) {
        FUN_141485f00(3,DAT_1450d6c88,0,0);
      }
      else {
        *(undefined1 *)((longlong)param_1 + 0x77b) = 1;
        FUN_141994820(uVar3,local_res8,iVar2);
        uVar1 = DAT_1450d6c88;
        local_res8[1] = 0;
        if (local_res8[0] != 0) {
          FUN_141485f00(1,DAT_1450d6c88,0,0);
          *(undefined1 *)(param_1 + 0xf9) = *(undefined1 *)((longlong)param_1 + 0x625);
          *(int *)(param_1 + 0xf8) = iVar2;
          (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0xbe],0,1,0,uVar1);
          FUN_141505090(*(undefined8 *)(param_1[0xa9] + (longlong)(iVar2 + -1) * 8));
          FUN_141501b00(param_1,local_res8);
          param_1[0xf7] = local_res8[0];
          if (param_1[0xc2] != 0) {
            FUN_1414c8c70(param_1[0xc2],4);
            FUN_1414dee00(param_1[0xc2],local_res8);
          }
        }
      }
    }
  }
  return;
}



// ======== FUN_1414f8ae0 @ 1414f8ae0 ========

void FUN_1414f8ae0(longlong *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  longlong local_res10;
  
  uVar3 = FUN_140d25b00();
  uVar4 = FUN_141991f50(uVar3,*(undefined1 *)((longlong)param_1 + 0x625));
  iVar2 = FUN_1414f2ab0(param_1,0);
  uVar1 = DAT_1450d6c88;
  if (0 < iVar2) {
    if ((((iVar2 == 1) || ((int)param_1[0xa1] != 0)) || (*(int *)((longlong)param_1 + 0x50c) != 0))
       || ((int)param_1[0xa2] != 0)) {
      FUN_141485f00(3,DAT_1450d6c88,0,0);
      return;
    }
    FUN_141485f00(5,DAT_1450d6c88,0,0);
    *(undefined1 *)((longlong)param_1 + 0x77b) = 0;
    FUN_141994820(uVar4,&local_res10,iVar2);
    if (local_res10 == 0) {
      (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0xbd],0,1,0,uVar1);
      if (0 < (int)param_1[0xf8]) {
        if (*(char *)((longlong)param_1 + 0x625) == (char)param_1[0xf9]) {
          FUN_1414f3650(param_1,(int)param_1[0xf8],0);
        }
        else {
          *(undefined1 *)(param_1 + 0xef) = 1;
          uVar3 = FUN_141991f50(uVar3);
          FUN_14199c750(uVar3,(int)param_1[0xf8]);
        }
      }
      if (param_1[0xc2] != 0) {
        FUN_1414c8c70(param_1[0xc2],4);
      }
      FUN_1414ee4e0(param_1,iVar2,param_1 + 0xf7,0);
      param_1[0xf7] = 0;
      *(undefined4 *)(param_1 + 0xf8) = 0;
      *(undefined1 *)(param_1 + 0xf9) = 0;
    }
    else if (iVar2 == (int)param_1[0xf8]) {
      FUN_141505050(*(undefined8 *)(param_1[0xa9] + (longlong)(iVar2 + -1) * 8));
      (**(code **)(*(longlong *)param_1[0xb0] + 0x250))((longlong *)param_1[0xb0],1);
      param_1[0xf7] = 0;
      *(undefined4 *)(param_1 + 0xf8) = 0;
      *(undefined1 *)(param_1 + 0xf9) = 0;
      FUN_1414c91a0(param_1[0xc2]);
    }
    else {
      *(int *)((longlong)param_1 + 0x7c4) = iVar2;
      (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0xbc],0,1,0,uVar1);
    }
  }
  return;
}



// ======== FUN_1414f9200 @ 1414f9200 ========

ulonglong FUN_1414f9200(longlong param_1)

{
  ulonglong uVar1;
  longlong local_res10;
  
  if (*(longlong *)(param_1 + 0x7b8) != 0) {
    return 1;
  }
  uVar1 = FUN_1414f2ab0(param_1,0);
  if (0 < (int)uVar1) {
    FUN_141994820(*(undefined8 *)(param_1 + 0x600),&local_res10,uVar1 & 0xffffffff);
    return (ulonglong)(local_res10 != 0);
  }
  return uVar1 & 0xffffffffffffff00;
}



// ################ callers of 0x1414f38f0 (1) ################
// ======== FUN_1414d5bb0 @ 1414d5bb0 ========

void FUN_1414d5bb0(longlong param_1)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  
  if (*(int *)(param_1 + 0x500) == 0x10) {
    FUN_1414d79e0();
    *(undefined4 *)(param_1 + 0x634) = 0;
    return;
  }
  if (*(int *)(param_1 + 0x500) == 9) {
    FUN_141692420(*(undefined8 *)(param_1 + 0x628));
    return;
  }
  if (*(char *)(param_1 + 0x4f3) == '\x06') {
    iVar2 = FUN_1414f2ab0(*(undefined8 *)(param_1 + 0x4a0),0);
    if (iVar2 == 1) {
      FUN_141485f00(3,DAT_1450d6c88,0,0);
      return;
    }
    FUN_1415047f0(*(undefined8 *)(param_1 + 0x5f8),1);
    FUN_1414d59c0(param_1);
    *(undefined1 *)(param_1 + 0x4f3) = 7;
    return;
  }
  if (*(char *)(param_1 + 0x4f3) == '\a') {
    iVar2 = FUN_1414f2ab0(*(undefined8 *)(param_1 + 0x4a0),0);
    if ((iVar2 == 2) || (iVar2 - 6U < 3)) {
      FUN_1414f38f0(*(undefined8 *)(param_1 + 0x4a0));
      *(undefined1 *)(param_1 + 0x4f3) = 0;
      return;
    }
    cVar1 = FUN_1414f93b0(*(undefined8 *)(param_1 + 0x4a0));
    if (cVar1 == '\0') {
      FUN_1414c9ea0(param_1);
      return;
    }
  }
  else {
    cVar1 = FUN_1414c9d60();
    if (cVar1 != '\0') {
      cVar1 = FUN_1414f91e0(*(undefined8 *)(param_1 + 0x4a0));
      uVar3 = *(undefined8 *)(param_1 + 0x4a0);
      if (cVar1 == '\0') {
        iVar2 = FUN_1414f2ab0(uVar3,1);
        if (iVar2 < 1) {
          FUN_1414d59c0(param_1);
          return;
        }
        uVar3 = *(undefined8 *)(param_1 + 0x4a0);
      }
      FUN_1414f38f0(uVar3);
      return;
    }
  }
  return;
}



