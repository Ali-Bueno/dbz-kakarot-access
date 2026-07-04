// FUN_1414f38f0 @ 1414f38f0

void FUN_1414f38f0(longlong *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined8 uVar3;
  longlong local_res8 [2];
  
  if (*(char *)((longlong)param_1 + 0x79c) != '\0') {
    local_res8[0] = 0;
    if (param_1[0xf7] != 0) {
      FUN_1414f8ae0();
      return;
    }
    uVar3 = FUN_140d25b00();
    uVar3 = FUN_141991f50(uVar3,*(undefined1 *)((longlong)param_1 + 0x625));
    iVar2 = FUN_1414f2ab0(param_1,0);
    if (0 < iVar2) {
      if ((((iVar2 == 1) || ((int)param_1[0xa1] != 0)) || (*(int *)((longlong)param_1 + 0x50c) != 0)
          ) || ((int)param_1[0xa2] != 0)) {
        FUN_141485f00(3,DAT_1450d6c88,0,0);
      }
      else {
        *(undefined1 *)((longlong)param_1 + 0x77b) = 1;
        FUN_141994820(uVar3,local_res8,iVar2);
        uVar1 = DAT_1450d6c88;
        local_res8[1] = 0;
        if (local_res8[0] != 0) {
          FUN_141485f00(1,DAT_1450d6c88,0,0);
          *(undefined1 *)(param_1 + 0xf9) = *(undefined1 *)((longlong)param_1 + 0x625);
          *(int *)(param_1 + 0xf8) = iVar2;
          (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0xbe],0,1,0,uVar1);
          FUN_141505090(*(undefined8 *)(param_1[0xa9] + (longlong)(iVar2 + -1) * 8));
          FUN_141501b00(param_1,local_res8);
          param_1[0xf7] = local_res8[0];
          if (param_1[0xc2] != 0) {
            FUN_1414c8c70(param_1[0xc2],4);
            FUN_1414dee00(param_1[0xc2],local_res8);
          }
        }
      }
    }
  }
  return;
}


