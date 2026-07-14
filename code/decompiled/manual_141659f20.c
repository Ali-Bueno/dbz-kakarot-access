// FUN_141659f20 @ 141659f20

char FUN_141659f20(longlong param_1,char param_2)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  char cVar4;
  
  cVar4 = '\0';
  iVar1 = *(int *)(param_1 + 0x15f0);
  iVar3 = *(int *)(param_1 + 0x15fc) + -2;
  if (iVar1 <= iVar3) {
    piVar2 = (int *)(param_1 +
                    ((longlong)*(int *)(param_1 + 0x15f8) + 0x133 + (longlong)iVar3 * 0x1e) * 4);
    do {
      if ((0 < *piVar2) &&
         (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + -8 + (longlong)*piVar2 * 8) +
                   0x460) != '\x01')) {
        *(int *)(param_1 + 0x15fc) = iVar3;
        cVar4 = '\x01';
        break;
      }
      iVar3 = iVar3 + -2;
      piVar2 = piVar2 + -0x3c;
    } while (iVar1 <= iVar3);
  }
  if ((param_2 != '\0') && (cVar4 == '\0')) {
    iVar3 = *(int *)(param_1 + 0x15fc);
    *(int *)(param_1 + 0x15fc) = iVar3 + -2;
    if (iVar1 <= iVar3 + -2) {
      iVar1 = *(int *)(param_1 + 0x15f8) + 2;
      if (iVar1 <= *(int *)(param_1 + 0x15ec)) {
        piVar2 = (int *)(param_1 + (((longlong)iVar3 + -2) * 0x1e + (longlong)iVar1 + 0x133) * 4);
        do {
          if ((0 < *piVar2) &&
             (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + -8 + (longlong)*piVar2 * 8) +
                       0x460) != '\x01')) goto LAB_14165a093;
          iVar1 = iVar1 + 2;
          piVar2 = piVar2 + 2;
        } while (iVar1 <= *(int *)(param_1 + 0x15ec));
      }
      iVar1 = *(int *)(param_1 + 0x15f8) + -2;
      if (*(int *)(param_1 + 0x15e8) <= iVar1) {
        piVar2 = (int *)(param_1 + (((longlong)iVar3 + -2) * 0x1e + (longlong)iVar1 + 0x133) * 4);
        do {
          if ((0 < *piVar2) &&
             (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + -8 + (longlong)*piVar2 * 8) +
                       0x460) != '\x01')) {
LAB_14165a093:
            *(int *)(param_1 + 0x15f8) = iVar1;
            return '\0';
          }
          iVar1 = iVar1 + -2;
          piVar2 = piVar2 + -2;
        } while (*(int *)(param_1 + 0x15e8) <= iVar1);
      }
    }
    *(int *)(param_1 + 0x15fc) = iVar3;
  }
  return cVar4;
}


