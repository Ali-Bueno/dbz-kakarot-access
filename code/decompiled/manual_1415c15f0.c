// FUN_1415c15f0 @ 1415c15f0

void FUN_1415c15f0(longlong param_1)

{
  undefined1 uVar1;
  undefined1 *puVar2;
  longlong lVar3;
  undefined4 uVar4;
  longlong local_18 [2];
  
  if (*(char *)(param_1 + 0x51d) == '\0') {
    return;
  }
  if ((*(int *)(param_1 + 0x508) != 6) && (*(int *)(param_1 + 0x508) != 0xc)) {
    return;
  }
  if (*(int *)(param_1 + 0x514) == -1) {
    FUN_141485f00(3,DAT_1450d6c88,0,0);
    return;
  }
  puVar2 = (undefined1 *)
           FUN_1415bd150(param_1,*(undefined4 *)
                                  ((longlong)*(int *)(param_1 + 0x514) * 0x20 + 0x1c +
                                  *(longlong *)(param_1 + 0x4d0)));
  uVar1 = *puVar2;
  lVar3 = FUN_1415bd150(param_1,*(undefined4 *)
                                 ((longlong)*(int *)(param_1 + 0x514) * 0x20 + 0x1c +
                                 *(longlong *)(param_1 + 0x4d0)));
  FUN_1415bcf40(local_18,uVar1,*(undefined1 *)(lVar3 + 1));
  FUN_1415c56f0(*(undefined8 *)(param_1 + 0x490),uVar1,local_18);
  uVar4 = DAT_1450d6c88;
  *(undefined1 *)(param_1 + 0x51d) = 0;
  FUN_141485f00(1,uVar4,0,0);
  if (*(int *)(param_1 + 0x508) == 6) {
    *(undefined8 *)(param_1 + 0x508) = 9;
    uVar4 = 9;
  }
  else {
    if (*(int *)(param_1 + 0x508) != 0xc) goto LAB_1415c16ff;
    *(undefined8 *)(param_1 + 0x508) = 10;
    uVar4 = 10;
  }
  FUN_1415cd100(param_1,uVar4);
LAB_1415c16ff:
  if (local_18[0] == 0) {
    return;
  }
  FUN_141de9200();
  return;
}


