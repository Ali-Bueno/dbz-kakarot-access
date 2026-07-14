// FUN_14145c4b0 @ 14145c4b0

ulonglong FUN_14145c4b0(undefined8 param_1,undefined8 param_2,longlong *param_3)

{
  longlong *plVar1;
  longlong lVar2;
  longlong lVar3;
  longlong *plVar4;
  
  lVar3 = FUN_14145e140();
  if (lVar3 != 0) {
    plVar1 = *(longlong **)(lVar3 + 0x100);
    lVar2 = *param_3;
    for (plVar4 = plVar1; plVar4 != plVar1 + *(int *)(lVar3 + 0x108); plVar4 = plVar4 + 1) {
      if (*plVar4 == lVar2) {
        if ((int)((longlong)plVar4 - (longlong)plVar1 >> 3) != -1) {
          return CONCAT71((int7)((ulonglong)lVar2 >> 8),2);
        }
        break;
      }
    }
    plVar1 = *(longlong **)(lVar3 + 0xf0);
    for (plVar4 = plVar1; plVar4 != plVar1 + *(int *)(lVar3 + 0xf8); plVar4 = plVar4 + 1) {
      if (*plVar4 == lVar2) {
        return (ulonglong)((int)((longlong)plVar4 - (longlong)plVar1 >> 3) != -1);
      }
    }
  }
  return 0;
}


