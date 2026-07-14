// FUN_140d2aef0 @ 140d2aef0

longlong FUN_140d2aef0(void)

{
  longlong lVar1;
  bool bVar2;
  undefined8 uVar3;
  longlong lVar4;
  int local_res8 [8];
  
  lVar1 = DAT_14592f0e8;
  if (DAT_14592f0e8 != 0) {
    uVar3 = FUN_1418fe4e0();
    FUN_140cfa640(lVar1 + 0x550,local_res8,uVar3);
    if (local_res8[0] != -1) {
      lVar1 = *(longlong *)(lVar1 + 0x550) + (longlong)local_res8[0] * 0x28;
      if (((lVar1 != 0) && (lVar1 != -8)) && (lVar1 = *(longlong *)(lVar1 + 0x10), lVar1 != 0)) {
        lVar4 = FUN_1418fe4e0();
        if ((*(int *)(*(longlong *)(lVar1 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(lVar1 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar4 + 0x90) * 8) != lVar4 + 0x88)) {
          bVar2 = false;
        }
        else {
          bVar2 = true;
        }
        if (bVar2) {
          return lVar1;
        }
      }
    }
  }
  return 0;
}


