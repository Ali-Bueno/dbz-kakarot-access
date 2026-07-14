// FUN_141655b50 @ 141655b50

undefined1 FUN_141655b50(longlong param_1)

{
  longlong lVar1;
  char cVar2;
  longlong lVar3;
  
  cVar2 = FUN_1429b40b0(param_1,*(undefined8 *)(param_1 + 0x4b8));
  if (cVar2 == '\0') {
    lVar1 = *(longlong *)(param_1 + 0x1548);
    lVar3 = FUN_140d2ba50();
    if (((lVar3 != 0) && (lVar3 = FUN_1415458f0(lVar3), lVar3 != 0)) &&
       (*(char *)(lVar3 + 0x402) == '\0')) {
      return 0;
    }
    cVar2 = FUN_1416ee150(*(undefined8 *)(lVar1 + 0x3d8));
    if (cVar2 != '\0') {
      return 1;
    }
  }
  return 0;
}


