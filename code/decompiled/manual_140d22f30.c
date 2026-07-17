// FUN_140d22f30 @ 140d22f30

longlong FUN_140d22f30(void)

{
  bool bVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  
  lVar2 = DAT_14592f0e8;
  if (DAT_14592f0f0 == 0) {
    lVar2 = FUN_142d1f0d0();
    if (lVar2 != 0) {
      lVar3 = FUN_141758410();
      if ((*(int *)(*(longlong *)(lVar2 + 0x10) + 0x90) < *(int *)(lVar3 + 0x90)) ||
         (*(longlong *)
           (*(longlong *)(*(longlong *)(lVar2 + 0x10) + 0x88) + (longlong)*(int *)(lVar3 + 0x90) * 8
           ) != lVar3 + 0x88)) {
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
  if (DAT_14592f0f0 == 1) {
    if (DAT_14592f0e8 == 0) {
      return 0;
    }
    FUN_140d1b200(DAT_14592f0e8);
    return *(longlong *)(lVar2 + 0x648);
  }
  lVar2 = FUN_142d1f0d0();
  lVar3 = 0;
  if (lVar2 != 0) {
    lVar4 = FUN_141758410();
    if ((*(int *)(*(longlong *)(lVar2 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar2 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
        != lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) goto LAB_140d23005;
  }
  lVar2 = lVar3;
LAB_140d23005:
  lVar4 = DAT_14592f0e8;
  lVar5 = lVar3;
  if (DAT_14592f0e8 != 0) {
    FUN_140d1b200(DAT_14592f0e8);
    lVar5 = *(longlong *)(lVar4 + 0x648);
  }
  lVar4 = DAT_14592f0e8;
  if ((lVar2 != lVar5) && (lVar5 = lVar3, DAT_14592f0e8 != 0)) {
    FUN_140d1b200(DAT_14592f0e8);
    return *(longlong *)(lVar4 + 0x648);
  }
  return lVar5;
}


