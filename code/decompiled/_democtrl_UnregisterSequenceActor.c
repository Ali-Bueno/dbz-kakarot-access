// UFunction 'UnregisterSequenceActor' — exec thunk(s) + native impl (via UE native-registration table)
// ---- FUN_141872ad0 @ 141872ad0 ----

void FUN_141872ad0(undefined8 param_1,longlong param_2)

{
  undefined8 local_res10;
  
  local_res10 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1410668c0(param_1,local_res10);
  return;
}



// ---- FUN_14204e7f0 @ 14204e7f0 ----

void FUN_14204e7f0(longlong param_1,undefined8 param_2)

{
  byte bVar1;
  
  bVar1 = **(byte **)(param_1 + 0x20);
  *(byte **)(param_1 + 0x20) = *(byte **)(param_1 + 0x20) + 1;
                    /* WARNING: Could not recover jumptable at 0x00014204e812. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(&DAT_145b9d480)[bVar1])(param_2,param_1);
  return;
}



