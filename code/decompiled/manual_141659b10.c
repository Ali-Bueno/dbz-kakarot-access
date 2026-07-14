// FUN_141659b10 @ 141659b10

char FUN_141659b10(longlong param_1,char param_2)

{
  char cVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  
  cVar1 = '\0';
  iVar2 = *(int *)(param_1 + 0x15f4);
  iVar4 = *(int *)(param_1 + 0x15fc) + 2;
  if (iVar4 <= iVar2) {
    piVar3 = (int *)(param_1 +
                    ((longlong)*(int *)(param_1 + 0x15f8) + 0x133 + (longlong)iVar4 * 0x1e) * 4);
    do {
      if ((0 < *piVar3) &&
         (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + -8 + (longlong)*piVar3 * 8) +
                   0x460) != '\x01')) {
        *(int *)(param_1 + 0x15fc) = iVar4;
        cVar1 = '\x01';
        break;
      }
      iVar4 = iVar4 + 2;
      piVar3 = piVar3 + 0x3c;
    } while (iVar4 <= iVar2);
  }
  if ((param_2 == '\0') || (cVar1 != '\0')) {
    return cVar1;
  }
  iVar4 = *(int *)(param_1 + 0x15fc);
  *(int *)(param_1 + 0x15fc) = iVar4 + 2;
  if (iVar2 < iVar4 + 2) {
    *(int *)(param_1 + 0x15fc) = iVar4;
    return '\0';
  }
  iVar2 = *(int *)(param_1 + 0x15f8) + 2;
  if (iVar2 <= *(int *)(param_1 + 0x15ec)) {
    piVar3 = (int *)(param_1 + (((longlong)iVar4 + 2) * 0x1e + (longlong)iVar2 + 0x133) * 4);
    do {
      if ((0 < *piVar3) &&
         (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + -8 + (longlong)*piVar3 * 8) +
                   0x460) != '\x01')) {
        *(int *)(param_1 + 0x15f8) = iVar2;
        return '\0';
      }
      iVar2 = iVar2 + 2;
      piVar3 = piVar3 + 2;
    } while (iVar2 <= *(int *)(param_1 + 0x15ec));
  }
  cVar1 = FUN_141659c90(param_1,0);
  if (cVar1 != '\0') {
    return '\0';
  }
  *(int *)(param_1 + 0x15fc) = iVar4;
  return '\0';
}


