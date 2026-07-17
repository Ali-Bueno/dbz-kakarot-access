// FUN_140f8af30 @ 140f8af30

longlong FUN_140f8af30(longlong param_1)

{
  longlong lVar1;
  bool bVar2;
  longlong lVar3;
  
  if (*(int *)(param_1 + 0x120) < 1) {
    return 0;
  }
  lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x118) + 8);
  if (lVar1 != 0) {
    lVar3 = FUN_141818cd0();
    if ((*(int *)(*(longlong *)(lVar1 + 0x10) + 0x90) < *(int *)(lVar3 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar1 + 0x10) + 0x88) + (longlong)*(int *)(lVar3 + 0x90) * 8)
        != lVar3 + 0x88)) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    if (bVar2) {
      return lVar1;
    }
  }
  return 0;
}


