// FUN_1414d59c0 @ 1414d59c0

void FUN_1414d59c0(longlong param_1)

{
  char cVar1;
  undefined8 local_res8 [4];
  
  if (*(int *)(param_1 + 0x500) == 7) {
    cVar1 = FUN_1414f93b0(*(undefined8 *)(param_1 + 0x4a0));
    if ((cVar1 == '\0') && (*(longlong *)(param_1 + 0x5f8) != 0)) {
      *(undefined1 *)(*(longlong *)(param_1 + 0x5f8) + 0x49) = *(undefined1 *)(param_1 + 0x50c);
      local_res8[0] = 0;
      FUN_1414fde80(*(undefined8 *)(param_1 + 0x5f8),2,local_res8);
      *(undefined4 *)(param_1 + 0x500) = 10;
    }
  }
  return;
}


