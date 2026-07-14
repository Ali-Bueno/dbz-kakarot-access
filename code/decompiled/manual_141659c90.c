// FUN_141659c90 @ 141659c90

char FUN_141659c90(longlong param_1,char param_2)

{
  char cVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  
  cVar1 = '\0';
  iVar2 = *(int *)(param_1 + 0x15e8);
  iVar4 = *(int *)(param_1 + 0x15f8) + -2;
  if (iVar2 <= iVar4) {
    piVar3 = (int *)(param_1 +
                    ((longlong)iVar4 + (longlong)*(int *)(param_1 + 0x15fc) * 0x1e + 0x133) * 4);
    do {
      if ((0 < *piVar3) &&
         (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + -8 + (longlong)*piVar3 * 8) +
                   0x460) != '\x01')) {
        *(int *)(param_1 + 0x15f8) = iVar4;
        cVar1 = '\x01';
        break;
      }
      iVar4 = iVar4 + -2;
      piVar3 = piVar3 + -2;
    } while (iVar2 <= iVar4);
  }
  if ((param_2 == '\0') || (cVar1 != '\0')) {
    return cVar1;
  }
  iVar4 = *(int *)(param_1 + 0x15f8);
  *(int *)(param_1 + 0x15f8) = iVar4 + -2;
  if (iVar4 + -2 < iVar2) {
    *(int *)(param_1 + 0x15f8) = iVar4;
    return '\0';
  }
  iVar2 = *(int *)(param_1 + 0x15fc) + 2;
  if (iVar2 <= *(int *)(param_1 + 0x15f4)) {
    piVar3 = (int *)(param_1 + ((longlong)iVar2 * 0x1e + (longlong)iVar4 + 0x131) * 4);
    do {
      if ((0 < *piVar3) &&
         (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + -8 + (longlong)*piVar3 * 8) +
                   0x460) != '\x01')) {
        *(int *)(param_1 + 0x15fc) = iVar2;
        return '\0';
      }
      iVar2 = iVar2 + 2;
      piVar3 = piVar3 + 0x3c;
    } while (iVar2 <= *(int *)(param_1 + 0x15f4));
  }
  cVar1 = FUN_141659f20(param_1,0);
  if (cVar1 != '\0') {
    return '\0';
  }
  *(int *)(param_1 + 0x15f8) = iVar4;
  return '\0';
}


