// FUN_141664c50 @ 141664c50

void FUN_141664c50(longlong param_1)

{
  float fVar1;
  float fVar2;
  undefined4 uVar3;
  char cVar4;
  
  if ((((*(char *)(param_1 + 0x1650) == '\0') && (*(char *)(param_1 + 0x1550) != '\0')) &&
      (*(char *)(param_1 + 0x1681) != '\0')) && (cVar4 = FUN_141655b50(), cVar4 != '\0')) {
    fVar1 = *(float *)(*(longlong *)(param_1 + 0x400) + 0x3f8);
    fVar2 = *(float *)(*(longlong *)(param_1 + 0x400) + 0x3fc);
    if (*(int *)(param_1 + 0x157c) == 7) {
      FUN_1429e1d30(*(undefined8 *)(param_1 + 0x408),CONCAT44(DAT_145a8af0c,DAT_145a8af08));
      *(undefined4 *)(param_1 + 0x1648) = DAT_145a8af08;
      *(undefined4 *)(param_1 + 0x164c) = DAT_145a8af0c;
      *(undefined4 *)(param_1 + 0x1630) = 0;
      *(undefined4 *)(param_1 + 0x157c) = 5;
      *(undefined1 *)(param_1 + 0x1680) = 0;
      *(float *)(param_1 + 0x1640) = *(float *)(param_1 + 0x1618) - fVar1;
      *(float *)(param_1 + 0x1644) = *(float *)(param_1 + 0x161c) - fVar2;
      FUN_14166e2e0(param_1);
      FUN_141485f00(1,DAT_1450d6c88,0,0);
      return;
    }
    if (*(int *)(param_1 + 0x157c) == 0) {
      *(undefined4 *)(param_1 + 0x157c) = 6;
      *(undefined1 *)(param_1 + 0x1680) = 1;
      FUN_14166e2e0(param_1);
      *(undefined8 *)(param_1 + 0x1648) = *(undefined8 *)(*(longlong *)(param_1 + 0x410) + 0x90);
      *(undefined4 *)(param_1 + 0x1640) = DAT_145a8af08;
      uVar3 = DAT_1450d6c88;
      *(undefined4 *)(param_1 + 0x1644) = DAT_145a8af0c;
      *(undefined4 *)(param_1 + 0x1630) = 0;
      FUN_141485f00(1,uVar3,0,0);
      *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 8;
    }
  }
  return;
}


