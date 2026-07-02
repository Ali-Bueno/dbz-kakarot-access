// UFunction 'OnEnterAnimEnd' — exec thunk(s) + native impl (via UE native-registration table)
// ---- FUN_1417c2740 @ 1417c2740 ----

void FUN_1417c2740(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_1416f0180();
  return;
}



// ---- FUN_1454f25f0 @ 1454f25f0 ----

/* WARNING: Control flow encountered bad instruction data */

void FUN_1454f25f0(undefined8 param_1,undefined2 param_2,char *param_3)

{
  undefined1 uVar1;
  uint uVar2;
  char cVar3;
  undefined4 in_EAX;
  undefined3 uVar5;
  undefined4 in_register_00000004;
  undefined1 *unaff_RDI;
  char in_CF;
  undefined4 uVar4;
  
  uVar5 = (undefined3)((uint)in_EAX >> 8);
  cVar3 = (char)in_EAX + *(char *)CONCAT44(in_register_00000004,in_EAX) + in_CF;
  uVar4 = CONCAT31(uVar5,cVar3);
  *(char *)CONCAT44(in_register_00000004,uVar4) =
       *(char *)CONCAT44(in_register_00000004,uVar4) + cVar3;
  *(char *)CONCAT44(in_register_00000004,uVar4) =
       *(char *)CONCAT44(in_register_00000004,uVar4) + cVar3;
  *(char *)CONCAT44(in_register_00000004,uVar4) =
       *(char *)CONCAT44(in_register_00000004,uVar4) + cVar3;
  uVar1 = in(param_2);
  *unaff_RDI = uVar1;
  uVar2 = CONCAT31(uVar5,cVar3);
  *(int *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(int *)(CONCAT44(in_register_00000004,uVar2) | 8) + (int)param_3;
  cVar3 = (char)(uVar2 | 8);
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *param_3 = *param_3 + (char)param_3;
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *(uint *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(int *)(CONCAT44(in_register_00000004,uVar2) | 8) + (uVar2 | 8);
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
  *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) =
       *(char *)(CONCAT44(in_register_00000004,uVar2) | 8) + cVar3;
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



