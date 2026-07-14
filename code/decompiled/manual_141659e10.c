// FUN_141659e10 @ 141659e10

char FUN_141659e10(longlong param_1,char param_2)

{
  int iVar1;
  char cVar2;
  int *piVar3;
  int iVar4;
  
  cVar2 = '\0';
  iVar1 = *(int *)(param_1 + 0x15ec);
  iVar4 = *(int *)(param_1 + 0x15f8) + 2;
  if (iVar4 <= iVar1) {
    piVar3 = (int *)(param_1 +
                    ((longlong)iVar4 + (longlong)*(int *)(param_1 + 0x15fc) * 0x1e + 0x133) * 4);
    do {
      if ((0 < *piVar3) &&
         (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + -8 + (longlong)*piVar3 * 8) +
                   0x460) != '\x01')) {
        *(int *)(param_1 + 0x15f8) = iVar4;
        cVar2 = '\x01';
        break;
      }
      iVar4 = iVar4 + 2;
      piVar3 = piVar3 + 2;
    } while (iVar4 <= iVar1);
  }
  if ((param_2 != '\0') && (cVar2 == '\0')) {
    iVar4 = *(int *)(param_1 + 0x15f8);
    *(int *)(param_1 + 0x15f8) = iVar4 + 2;
    if (iVar1 < iVar4 + 2) {
      *(int *)(param_1 + 0x15f8) = iVar4;
    }
    else {
      cVar2 = FUN_141659b10(param_1,0);
      if (cVar2 == '\0') {
        cVar2 = FUN_141659f20(param_1,0);
        if (cVar2 == '\0') {
          *(int *)(param_1 + 0x15f8) = iVar4;
        }
        return '\0';
      }
    }
    return '\0';
  }
  return cVar2;
}


