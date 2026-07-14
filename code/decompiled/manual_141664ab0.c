// FUN_141664ab0 @ 141664ab0

void FUN_141664ab0(longlong param_1)

{
  longlong lVar1;
  byte bVar2;
  int iVar3;
  char cVar4;
  
  if ((*(char *)(param_1 + 0x1650) == '\0') && (*(char *)(param_1 + 0x1550) != '\0')) {
    cVar4 = FUN_141655b50();
    if ((cVar4 != '\0') &&
       (((*(int *)(param_1 + 0x157c) == 0 || (*(int *)(param_1 + 0x157c) == 7)) &&
        (iVar3 = *(int *)(param_1 + 0x4cc +
                         ((longlong)*(int *)(param_1 + 0x15fc) * 0x1e +
                         (longlong)*(int *)(param_1 + 0x15f8)) * 4), 0 < iVar3)))) {
      lVar1 = (longlong)iVar3 * 8 + -8;
      bVar2 = *(byte *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x460);
      if ((8 < bVar2) || ((0x124U >> (bVar2 & 0x1f) & 1) == 0)) {
        FUN_141656060(*(undefined8 *)(param_1 + 0x1548),*(undefined4 *)(param_1 + 0x1584),
                      *(longlong *)(param_1 + 0x1538) + lVar1);
        return;
      }
    }
  }
  return;
}


