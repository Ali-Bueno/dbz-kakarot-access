// FUN_140f8aba0 @ 140f8aba0

undefined8 FUN_140f8aba0(longlong param_1)

{
  longlong lVar1;
  bool bVar2;
  longlong lVar3;
  undefined8 uVar4;
  
  lVar1 = *(longlong *)(param_1 + 0xf0);
  if (lVar1 != 0) {
    lVar3 = FUN_141758410();
    if ((*(int *)(*(longlong *)(lVar1 + 0x10) + 0x90) < *(int *)(lVar3 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar1 + 0x10) + 0x88) + (longlong)*(int *)(lVar3 + 0x90) * 8)
        != lVar3 + 0x88)) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    if ((bVar2) && (*(longlong **)(lVar1 + 0x8e8) != (longlong *)0x0)) {
                    /* WARNING: Could not recover jumptable at 0x000140f8abfc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar4 = (**(code **)(**(longlong **)(lVar1 + 0x8e8) + 1000))();
      return uVar4;
    }
  }
  return 1;
}


