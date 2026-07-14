// FUN_141462a10 @ 141462a10

bool FUN_141462a10(undefined8 param_1,undefined8 param_2)

{
  short *psVar1;
  undefined8 *puVar2;
  int iVar3;
  short *psVar4;
  undefined8 local_res10 [3];
  longlong local_18 [2];
  
  local_res10[0] = param_2;
  puVar2 = (undefined8 *)FUN_141eca8b0(local_res10,local_18);
  psVar1 = (short *)*puVar2;
  psVar4 = psVar1;
  do {
    if (psVar4 == psVar1 + *(int *)(puVar2 + 1)) {
      iVar3 = -1;
LAB_141462a54:
      if (local_18[0] != 0) {
        FUN_141de9200();
      }
      return iVar3 == 0;
    }
    if (*psVar4 == 0x50) {
      iVar3 = (int)((longlong)psVar4 - (longlong)psVar1 >> 1);
      goto LAB_141462a54;
    }
    psVar4 = psVar4 + 1;
  } while( true );
}


