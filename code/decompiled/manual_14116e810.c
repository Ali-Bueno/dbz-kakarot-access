// FUN_14116e810 @ 14116e810

void FUN_14116e810(longlong param_1,undefined1 *param_2)

{
  float *pfVar1;
  float *pfVar2;
  uint uVar3;
  float fVar4;
  float fVar5;
  undefined1 auVar6 [16];
  float local_68;
  float local_64;
  float local_60;
  float local_58;
  float local_54;
  float local_50;
  undefined1 local_48 [64];
  
  *(undefined1 *)(param_1 + 0x460) = *param_2;
  *(undefined1 *)(param_1 + 0x461) = param_2[1];
  fVar4 = *(float *)(param_2 + 0x1c) * DAT_143a86318;
  *(float *)(param_1 + 0x42c) = fVar4;
  *(float *)(param_1 + 0x430) = fVar4;
  *(undefined4 *)(param_1 + 0x464) = *(undefined4 *)(param_2 + 0x20);
  *(undefined4 *)(param_1 + 0x468) = *(undefined4 *)(param_2 + 0x24);
  *(undefined4 *)(param_1 + 0x46c) = *(undefined4 *)(param_2 + 0x28);
  *(undefined4 *)(param_1 + 0x448) = *(undefined4 *)(param_2 + 0x40);
  *(undefined4 *)(param_1 + 0x44c) = *(undefined4 *)(param_2 + 0x44);
  local_68 = *(float *)(param_2 + 0x10) - *(float *)(param_2 + 4);
  local_64 = *(float *)(param_2 + 0x14) - *(float *)(param_2 + 8);
  local_60 = *(float *)(param_2 + 0x18) - *(float *)(param_2 + 0xc);
  fVar4 = local_64 * local_64 + local_68 * local_68 + local_60 * local_60;
  if (DAT_143983648 < fVar4) {
    auVar6 = rsqrtss(ZEXT416((uint)fVar4),ZEXT416((uint)fVar4));
    fVar5 = auVar6._0_4_;
    fVar5 = fVar5 + fVar5 * (DAT_14391b46c - fVar4 * DAT_14391b46c * fVar5 * fVar5);
    fVar5 = fVar5 + fVar5 * (DAT_14391b46c - fVar4 * DAT_14391b46c * fVar5 * fVar5);
    local_68 = local_68 * fVar5;
    local_64 = local_64 * fVar5;
    local_60 = local_60 * fVar5;
  }
  FUN_141e4e750(&local_68,local_48);
  FUN_141e4e690(local_48,&local_58,&DAT_1456d9d48);
  uVar3 = *(uint *)(param_2 + 0x48);
  *(uint *)(param_1 + 0x450) = uVar3;
  if ((float)(uVar3 & DAT_14391b650) <= DAT_143983648) {
    fVar4 = *(float *)(param_2 + 0x50) * *(float *)(param_1 + 0x42c);
    *(float *)(param_1 + 0x450) = fVar4;
    *(float *)(param_1 + 0x450) = fVar4 * *(float *)(param_2 + 0x4c);
  }
  pfVar1 = (float *)(param_1 + 0x3cc);
  fVar4 = *(float *)(param_1 + 0x44c);
  pfVar2 = (float *)(param_1 + 0x3d8);
  *(undefined8 *)pfVar1 = *(undefined8 *)(param_2 + 0x10);
  *(undefined4 *)(param_1 + 0x3d4) = *(undefined4 *)(param_2 + 0x18);
  *(undefined8 *)pfVar2 = *(undefined8 *)(param_2 + 0x10);
  *(undefined4 *)(param_1 + 0x3e0) = *(undefined4 *)(param_2 + 0x18);
  *(float *)(param_1 + 0x3d4) = *(float *)(param_1 + 0x448) + *(float *)(param_1 + 0x3d4);
  *(float *)(param_1 + 0x3d0) = local_54 * fVar4 + *(float *)(param_1 + 0x3d0);
  *pfVar1 = local_58 * fVar4 + *pfVar1;
  *(float *)(param_1 + 0x3d4) = local_50 * fVar4 + *(float *)(param_1 + 0x3d4);
  *(undefined4 *)(param_1 + 0x444) = *(undefined4 *)(param_2 + 0x2c);
  FUN_1415f0670(*(undefined8 *)(param_1 + 0x488),pfVar1,pfVar2);
  fVar5 = *(float *)(param_1 + 0x3dc) - *(float *)(param_2 + 8);
  fVar4 = *(float *)(param_1 + 0x3e0) - *(float *)(param_2 + 0xc);
  fVar4 = SQRT(fVar5 * fVar5 +
               (*pfVar2 - *(float *)(param_2 + 4)) * (*pfVar2 - *(float *)(param_2 + 4)) +
               fVar4 * fVar4);
  FUN_1415f46a0(*(undefined8 *)(param_1 + 0x488),fVar4);
  FUN_1415f4660(*(undefined8 *)(param_1 + 0x488),*(undefined4 *)(param_2 + 0x30),
                *(undefined4 *)(param_2 + 0x34),*(undefined4 *)(param_2 + 0x38));
  FUN_1415f1040(*(undefined8 *)(param_1 + 0x488),*(undefined4 *)(param_2 + 0x3c));
  FUN_1415f87d0(*(undefined8 *)(param_1 + 0x488),*(float *)(param_1 + 0x450) / fVar4);
  *(undefined4 *)(param_1 + 0x474) = 1;
  *(undefined1 *)(param_1 + 0x478) = 0;
  return;
}


