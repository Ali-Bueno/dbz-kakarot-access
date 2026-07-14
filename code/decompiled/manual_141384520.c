// FUN_141384520 @ 141384520

void FUN_141384520(undefined4 param_1,undefined8 param_2,char param_3)

{
  undefined8 uVar1;
  longlong *plVar2;
  uint uVar3;
  undefined8 local_res20;
  
  uVar1 = FUN_140d2aef0();
  FUN_14144bff0(uVar1,param_1,param_2);
  FUN_14144bb10(uVar1,param_1);
  if (param_3 != '\0') {
    uVar3 = 0;
    local_res20 = 0;
    do {
      plVar2 = (longlong *)FUN_14145e980(uVar1,&local_res20,param_1,uVar3 & 0xff,0);
      if (*plVar2 == 0) {
        FUN_14144eee0(uVar1,param_1,uVar3 & 0xff,param_2);
        return;
      }
      uVar3 = uVar3 + 1;
    } while ((int)uVar3 < 4);
  }
  return;
}


