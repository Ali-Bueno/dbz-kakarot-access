// FUN_14145e980 @ 14145e980

undefined8 *
FUN_14145e980(longlong param_1,undefined8 *param_2,undefined8 param_3,undefined8 param_4,
             undefined1 param_5)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined1 local_res8 [32];
  
  lVar1 = *(longlong *)(param_1 + 0x138);
  *param_2 = 0;
  if (lVar1 != 0) {
    puVar2 = (undefined8 *)FUN_14145e7e0(lVar1,local_res8,param_3,param_4,param_5);
    *param_2 = *puVar2;
  }
  return param_2;
}


