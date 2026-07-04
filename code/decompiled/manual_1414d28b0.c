// FUN_1414d28b0 @ 1414d28b0

void FUN_1414d28b0(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  
  uVar2 = FUN_1411fc890();
  cVar1 = FUN_14120f070(uVar2);
  if (cVar1 != '\0') {
    cVar1 = FUN_1429b4160(param_1);
    if (cVar1 == '\0') {
      cVar1 = (**(code **)(**(longlong **)(param_1 + 0x488) + 600))();
      if (cVar1 == '\0') {
        FUN_1414d5bb0(param_1);
        return;
      }
    }
  }
  return;
}


