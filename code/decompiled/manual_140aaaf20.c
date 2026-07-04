// FUN_140aaaf20 @ 140aaaf20

void FUN_140aaaf20(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  _guard_check_icall();
  return;
}


