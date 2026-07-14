// FUN_14145e140 @ 14145e140

undefined8 FUN_14145e140(longlong param_1,int param_2)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  
  cVar1 = FUN_14205a760(param_1 + 0x30);
  if (cVar1 == '\0') {
    return 0;
  }
  lVar3 = FUN_142058d50(param_1 + 0x30);
  uVar5 = 0;
  if (*(int *)(lVar3 + 0x30) != *(int *)(lVar3 + 0x5c)) {
    lVar4 = *(longlong *)(lVar3 + 0x68);
    if (*(longlong *)(lVar3 + 0x68) == 0) {
      lVar4 = lVar3 + 0x60;
    }
    iVar2 = *(int *)(lVar4 + ((longlong)*(int *)(lVar3 + 0x70) - 1U & (longlong)param_2) * 4);
    if (iVar2 != -1) {
      lVar3 = *(longlong *)(lVar3 + 0x28);
      while (lVar4 = (longlong)iVar2, *(int *)(lVar3 + lVar4 * 0x18) != param_2) {
        iVar2 = *(int *)(lVar3 + 0x10 + lVar4 * 0x18);
        if (iVar2 == -1) {
          return 0;
        }
      }
      if ((iVar2 != -1) && (lVar3 = lVar3 + lVar4 * 0x18, uVar5 = 0, lVar3 != 0)) {
        uVar5 = *(undefined8 *)(lVar3 + 8);
      }
    }
  }
  return uVar5;
}


