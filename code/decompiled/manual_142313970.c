// FUN_142313970 @ 142313970

void FUN_142313970(longlong param_1,longlong param_2,ulonglong param_3)

{
  longlong lVar1;
  int iVar2;
  uint uVar3;
  undefined8 uVar4;
  int iVar5;
  
  lVar1 = param_1 + (param_3 & 0xffffffff) * 8;
  if (*(longlong *)(lVar1 + 0xc08) != param_2) {
    uVar4 = 0;
    if (param_2 == 0) {
      *(ulonglong *)(param_1 + 0x1b18) =
           *(ulonglong *)(param_1 + 0x1b18) & ~(1L << (param_3 & 0x3f));
    }
    else {
      *(undefined1 *)(param_1 + 0x34) = 0;
      *(ulonglong *)(param_1 + 0x1b18) = *(ulonglong *)(param_1 + 0x1b18) | 1L << (param_3 & 0x3f);
      uVar4 = *(undefined8 *)(param_2 + 0x48);
    }
    *(undefined8 *)(param_1 + 0x1508 + (param_3 & 0xffffffff) * 8) = uVar4;
    if (*(longlong *)(param_1 + 0x1b18) == 0) {
      iVar5 = -1;
    }
    else {
      iVar2 = 0x1f;
      uVar3 = (uint)*(longlong *)(param_1 + 0x1b18);
      if (uVar3 != 0) {
        for (; uVar3 >> iVar2 == 0; iVar2 = iVar2 + -1) {
        }
      }
      iVar5 = 0;
      if (uVar3 != 0) {
        iVar5 = iVar2;
      }
    }
    *(int *)(param_1 + 0x1b40) = iVar5;
    FUN_142317120(lVar1 + 0xc08);
    *(ulonglong *)(param_1 + 0x8e8) = *(ulonglong *)(param_1 + 0x8e8) | 1L << (param_3 & 0x3f);
  }
  return;
}


