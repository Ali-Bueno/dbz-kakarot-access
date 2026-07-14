// FUN_141731110 @ 141731110

undefined4 FUN_141731110(undefined8 param_1,byte param_2)

{
  char cVar1;
  longlong lVar2;
  ulonglong uVar3;
  
  uVar3 = (ulonglong)param_2;
  lVar2 = FUN_1411fc890();
  lVar2 = *(longlong *)(*(longlong *)(lVar2 + 0x1ea0) + 0x100);
  cVar1 = FUN_14107d9f0(DAT_14598d340);
  if (cVar1 != '\0') {
    return *(undefined4 *)(lVar2 + 0xe4 + uVar3 * 4);
  }
  cVar1 = FUN_14107d9f0(DAT_14598d338);
  if (cVar1 != '\0') {
    return *(undefined4 *)(lVar2 + 0xcc + uVar3 * 4);
  }
  cVar1 = FUN_14107d9f0(DAT_14598d320);
  if (cVar1 != '\0') {
    return *(undefined4 *)(lVar2 + 0xb4 + uVar3 * 4);
  }
  cVar1 = FUN_14107d9f0(DAT_14598d318);
  if (cVar1 != '\0') {
    return *(undefined4 *)(lVar2 + 0x9c + uVar3 * 4);
  }
  cVar1 = FUN_14107d9f0(DAT_14598d310);
  if (cVar1 != '\0') {
    return *(undefined4 *)(lVar2 + 0x84 + uVar3 * 4);
  }
  cVar1 = FUN_14107d9f0(DAT_14598d308);
  if (cVar1 != '\0') {
    return *(undefined4 *)(lVar2 + 0x6c + uVar3 * 4);
  }
  return *(undefined4 *)(lVar2 + 0x54 + uVar3 * 4);
}


