// FUN_1415cfff0 @ 1415cfff0

void FUN_1415cfff0(longlong param_1,int param_2)

{
  undefined8 uVar1;
  
  FUN_1415cf800(param_1,param_2);
  *(undefined8 *)(param_1 + 0x548) =
       *(undefined8 *)((longlong)param_2 * 0x20 + 0x14 + *(longlong *)(param_1 + 0x4d0));
  uVar1 = CONCAT44(*(float *)(*(longlong *)(param_1 + 0x470) + 0x94) + *(float *)(param_1 + 0x54c),
                   (int)*(undefined8 *)(param_1 + 0x548));
  *(undefined8 *)(param_1 + 0x548) = uVar1;
  *(undefined8 *)(param_1 + 0x538) = uVar1;
  if (*(longlong *)(param_1 + 0x498) != 0) {
    FUN_1429e1d30(*(longlong *)(param_1 + 0x498),*(undefined8 *)(param_1 + 0x548));
  }
  if (*(longlong *)(param_1 + 0x4a0) != 0) {
    FUN_1429e1d30(*(longlong *)(param_1 + 0x4a0),*(undefined8 *)(param_1 + 0x538));
  }
  return;
}


