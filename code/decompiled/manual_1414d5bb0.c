// FUN_1414d5bb0 @ 1414d5bb0

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


