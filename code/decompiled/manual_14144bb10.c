// FUN_14144bb10 @ 14144bb10

void FUN_14144bb10(longlong param_1,undefined4 param_2)

{
  longlong lVar1;
  char cVar2;
  
  lVar1 = *(longlong *)(param_1 + 0x138);
  if (lVar1 != 0) {
    cVar2 = FUN_14145f240(lVar1);
    if (cVar2 == '\0') {
      FUN_14144c410(lVar1,param_2);
    }
  }
  return;
}


