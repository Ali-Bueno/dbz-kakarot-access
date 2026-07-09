// FUN_1415b9720 @ 1415b9720

void FUN_1415b9720(longlong param_1)

{
  int iVar1;
  char cVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  
  cVar2 = FUN_1415c1d70();
  if (cVar2 == '\0') {
    return;
  }
  if (*(char *)(param_1 + 0x551) == '\0') {
    return;
  }
  cVar2 = '\0';
  iVar4 = 0;
  if (0 < *(int *)(param_1 + 0x4d8)) {
    lVar5 = 0;
    do {
      cVar3 = (**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x4d0) + 8 + lVar5) + 600))();
      if (cVar3 != '\0') {
        iVar1 = *(int *)(param_1 + 0x508);
        if (5 < iVar1) {
          if (iVar1 < 8) {
            if (*(char *)(*(longlong *)(param_1 + 0x4d0) + 0x11 + lVar5) != '\0')
            goto LAB_1415b97eb;
            FUN_1415cf800(param_1,iVar4);
            *(undefined8 *)(param_1 + 0x548) =
                 *(undefined8 *)(*(longlong *)(param_1 + 0x4d0) + 0x14 + lVar5);
          }
          else if (iVar1 - 0xbU < 2) {
            if (*(char *)(*(longlong *)(param_1 + 0x4d0) + 0x11 + lVar5) == '\0')
            goto LAB_1415b97eb;
            FUN_1415cfff0(param_1,iVar4);
          }
        }
        cVar2 = '\x01';
      }
LAB_1415b97eb:
      iVar4 = iVar4 + 1;
      lVar5 = lVar5 + 0x20;
    } while (iVar4 < *(int *)(param_1 + 0x4d8));
    if (cVar2 != '\0') {
      FUN_141485f00(0,DAT_1450d6c88,0,0);
      *(undefined8 *)(param_1 + 0x538) = *(undefined8 *)(param_1 + 0x548);
      if (*(longlong *)(param_1 + 0x498) != 0) {
        FUN_1429e1d30(*(longlong *)(param_1 + 0x498),*(undefined8 *)(param_1 + 0x548));
      }
      if (*(longlong *)(param_1 + 0x4a0) != 0) {
        FUN_1429e1d30(*(longlong *)(param_1 + 0x4a0),*(undefined8 *)(param_1 + 0x538));
      }
      goto LAB_1415b986b;
    }
  }
  FUN_1415cf800(param_1,0xffffffff);
LAB_1415b986b:
  FUN_140d3c540(cVar2);
  return;
}


