// FUN_1415c1d70 @ 1415c1d70

ulonglong FUN_1415c1d70(longlong param_1)

{
  int iVar1;
  undefined8 uVar2;
  ulonglong uVar3;
  
  uVar2 = FUN_1411fc890();
  uVar3 = FUN_14120f070(uVar2);
  if ((char)uVar3 != '\0') {
    iVar1 = *(int *)(param_1 + 0x508);
    uVar3 = (ulonglong)iVar1;
    switch(iVar1) {
    case 6:
    case 7:
    case 0xb:
    case 0xc:
      return CONCAT71((int7)(int3)((uint)iVar1 >> 8),1);
    }
  }
  return uVar3 & 0xffffffffffffff00;
}


