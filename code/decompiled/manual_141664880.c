// FUN_141664880 @ 141664880

void FUN_141664880(longlong param_1)

{
  longlong *plVar1;
  undefined4 uVar2;
  char cVar3;
  undefined8 uVar4;
  
  if ((*(char *)(param_1 + 0x1650) == '\0') && (*(char *)(param_1 + 0x1550) != '\0')) {
    cVar3 = FUN_141655b50();
    uVar2 = DAT_1450d6c88;
    if ((cVar3 != '\0') && ((*(int *)(param_1 + 0x157c) == 0 || (*(int *)(param_1 + 0x157c) == 7))))
    {
      FUN_141485f00(2,DAT_1450d6c88,0,0);
      plVar1 = *(longlong **)(param_1 + 0x400);
      (**(code **)(*plVar1 + 0x2e8))(plVar1,plVar1[0x7c]);
      (**(code **)(*plVar1 + 0x2e0))(plVar1,plVar1[0x7d],0,1,0,uVar2);
      uVar4 = FUN_140d283a0();
      uVar4 = FUN_140d2ba00(uVar4);
      FUN_1414cb3d0(uVar4,param_1);
      *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 8;
      *(undefined8 *)(param_1 + 0x157c) = 2;
    }
  }
  return;
}


