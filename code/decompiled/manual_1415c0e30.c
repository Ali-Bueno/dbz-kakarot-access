// FUN_1415c0e30 @ 1415c0e30

void FUN_1415c0e30(longlong param_1,int param_2)

{
  ulonglong uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  
  *(undefined1 *)(param_1 + 0x551) = 0;
  if (*(int *)(param_1 + 0x508) == 0xc) {
    uVar2 = *(uint *)(param_1 + 0x514);
    uVar6 = (ulonglong)(int)uVar2;
    if (uVar2 != 0xffffffff) {
      if (param_2 == 2) {
        do {
          uVar4 = (int)uVar6 - 1;
          if ((int)uVar4 < 0) {
            uVar4 = *(int *)(param_1 + 0x4d8) - 1;
          }
          uVar6 = (ulonglong)uVar4;
        } while (*(char *)((longlong)(int)uVar4 * 0x20 + 0x11 + *(longlong *)(param_1 + 0x4d0)) ==
                 '\0');
      }
      else {
        if (param_2 != 3) {
          return;
        }
        uVar5 = uVar6;
        do {
          uVar1 = uVar5 + 1;
          iVar3 = (int)uVar6;
          uVar5 = 0;
          if ((longlong)uVar1 < (longlong)*(int *)(param_1 + 0x4d8)) {
            uVar5 = uVar1;
          }
          uVar6 = 0;
          if ((longlong)uVar1 < (longlong)*(int *)(param_1 + 0x4d8)) {
            uVar6 = (ulonglong)(iVar3 + 1);
          }
          uVar4 = (uint)uVar6;
        } while (*(char *)(uVar5 * 0x20 + 0x11 + *(longlong *)(param_1 + 0x4d0)) == '\0');
      }
      if (uVar4 != uVar2) {
        FUN_1415cfff0(param_1);
        return;
      }
    }
  }
  return;
}


