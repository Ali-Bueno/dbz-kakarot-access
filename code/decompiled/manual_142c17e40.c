// FUN_142c17e40 @ 142c17e40

longlong FUN_142c17e40(longlong param_1)

{
  longlong *plVar1;
  bool bVar2;
  longlong lVar3;
  longlong lVar4;
  
  plVar1 = *(longlong **)(param_1 + 0xf0);
  if (((plVar1 != (longlong *)0x0) && ((*(uint *)(plVar1 + 1) >> 4 & 1) == 0)) &&
     (lVar3 = (**(code **)(*plVar1 + 0x148))(), lVar3 != 0)) {
    return lVar3;
  }
  lVar3 = *(longlong *)(param_1 + 0x20);
  if (lVar3 != 0) {
    lVar4 = FUN_143283110();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
        != lVar4 + 0x88)) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    if (bVar2) {
      return lVar3;
    }
  }
  return 0;
}


