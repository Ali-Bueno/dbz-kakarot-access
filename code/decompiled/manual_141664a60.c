// FUN_141664a60 @ 141664a60

void FUN_141664a60(longlong param_1)

{
  char cVar1;
  
  if ((*(char *)(param_1 + 0x1650) == '\0') && (*(char *)(param_1 + 0x1550) != '\0')) {
    cVar1 = FUN_141655b50();
    if ((cVar1 != '\0') && ((*(int *)(param_1 + 0x157c) == 0 || (*(int *)(param_1 + 0x157c) == 7))))
    {
      FUN_141659f20(param_1,1);
      FUN_141672080(param_1);
      return;
    }
  }
  return;
}


