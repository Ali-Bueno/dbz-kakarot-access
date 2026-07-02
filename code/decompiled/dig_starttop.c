// ---- FUN_141001dc0 @ 141001dc0 ----

ulonglong FUN_141001dc0(longlong param_1)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  bool bVar4;
  uint uVar5;
  longlong lVar6;
  longlong lVar7;
  int iVar8;
  uint uVar9;
  uint *puVar10;
  longlong *plVar11;
  int local_88;
  uint uStack_84;
  uint uStack_74;
  int iStack_70;
  undefined4 uStack_6c;
  int iStack_60;
  uint uStack_5c;
  uint *local_58;
  uint uStack_50;
  uint uStack_4c;
  undefined8 local_48;
  longlong *local_40;
  undefined8 uStack_38;
  uint *puStack_30;
  uint uStack_28;
  uint uStack_24;
  undefined8 uStack_20;
  
  uStack_84 = 1;
  lVar7 = param_1;
  if (*(longlong *)(param_1 + 0x590) != 0) {
    lVar7 = *(longlong *)(param_1 + 0x590);
  }
  plVar11 = (longlong *)(lVar7 + 0x538);
  local_88 = 0;
  uVar2 = *(uint *)(lVar7 + 0x560);
  puVar1 = (uint *)(lVar7 + 0x548);
  uStack_74 = 0;
  iStack_70 = 0;
  if (uVar2 != 0) {
    puVar10 = *(uint **)(lVar7 + 0x558);
    if (*(uint **)(lVar7 + 0x558) == (uint *)0x0) {
      puVar10 = puVar1;
    }
    uVar3 = *puVar10;
    iStack_70 = 0;
    local_88 = 0;
    while (uVar3 == 0) {
      lVar6 = (longlong)local_88;
      iStack_70 = iStack_70 + 0x20;
      iVar8 = local_88 + 1;
      bVar4 = (int)((uVar2 - 1) + ((int)(uVar2 - 1) >> 0x1f & 0x1fU)) >> 5 <= local_88;
      local_88 = iVar8;
      uStack_74 = uVar2;
      if (bVar4) goto LAB_141001eb2;
      uVar3 = puVar10[lVar6 + 1];
    }
    uStack_84 = -uVar3 & uVar3;
    iVar8 = 0x1f;
    if (uStack_84 != 0) {
      for (; uStack_84 >> iVar8 == 0; iVar8 = iVar8 + -1) {
      }
    }
    if (uStack_84 == 0) {
      iVar8 = 0x20;
    }
    else {
      iVar8 = 0x1f - iVar8;
    }
    uStack_74 = (iStack_70 - iVar8) + 0x1f;
    if ((int)uVar2 < (int)uStack_74) {
      uStack_74 = uVar2;
    }
  }
LAB_141001eb2:
  uVar3 = *(uint *)(lVar7 + 0x560);
  uStack_50 = 0xffffffff;
  uStack_4c = uStack_74;
  local_48 = CONCAT44(uStack_6c,iStack_70);
  uStack_38 = CONCAT44(uStack_84,local_88);
  uVar9 = uVar3 & 0xffffffe0;
  iStack_60 = local_88;
  uStack_5c = uStack_84;
  local_58 = puVar1;
  local_40 = plVar11;
  puStack_30 = puVar1;
  uStack_20 = local_48;
  uStack_28 = uStack_50;
  uStack_24 = uStack_4c;
  uStack_74 = uVar3;
  if (uVar3 != uVar2) {
    puVar10 = *(uint **)(lVar7 + 0x558);
    if (*(uint **)(lVar7 + 0x558) == (uint *)0x0) {
      puVar10 = puVar1;
    }
    uVar5 = puVar10[(int)uVar3 >> 5] & -1 << ((byte)uVar3 & 0x1f);
    iVar8 = (int)uVar3 >> 5;
    while (uVar5 == 0) {
      uVar9 = uVar9 + 0x20;
      uStack_74 = uVar2;
      if ((int)((uVar2 - 1) + ((int)(uVar2 - 1) >> 0x1f & 0x1fU)) >> 5 <= iVar8) goto LAB_141001f81;
      uVar5 = puVar10[(longlong)iVar8 + 1];
      iVar8 = iVar8 + 1;
    }
    uVar5 = -uVar5 & uVar5;
    iVar8 = 0x1f;
    if (uVar5 != 0) {
      for (; uVar5 >> iVar8 == 0; iVar8 = iVar8 + -1) {
      }
    }
    if (uVar5 == 0) {
      iVar8 = 0x20;
    }
    else {
      iVar8 = 0x1f - iVar8;
    }
    uStack_74 = (uVar9 - iVar8) + 0x1f;
    if ((int)uVar2 < (int)uStack_74) {
      uStack_74 = uVar2;
    }
  }
LAB_141001f81:
  while( true ) {
    if ((uStack_4c == uStack_74) && (local_58 == puVar1)) {
      return (ulonglong)plVar11 & 0xffffffffffffff00;
    }
    lVar7 = (longlong)(int)uStack_4c * 0x20 + *plVar11;
    iVar8 = *(int *)(param_1 + 0x588) + *(int *)(lVar7 + 8);
    if ((*(int *)(param_1 + 0x4e4) <= iVar8) && (*(char *)(lVar7 + 0x11) != '\0')) break;
    uStack_50 = uStack_50 & ~uStack_5c;
    FUN_1407e9ca0(&iStack_60);
  }
  return CONCAT71((uint7)(uint3)((uint)iVar8 >> 8),1);
}



// ---- FUN_141002000 @ 141002000 ----

ulonglong FUN_141002000(longlong param_1)

{
  uint uVar1;
  ulonglong uVar2;
  int *piVar3;
  int *piVar4;
  int local_res8 [2];
  
  FUN_14088d620(param_1 + 0x538,local_res8);
  uVar2 = (ulonglong)local_res8[0];
  piVar4 = (int *)0x0;
  if (local_res8[0] != -1) {
    piVar4 = (int *)(uVar2 * 0x20 + *(longlong *)(param_1 + 0x538));
  }
  piVar3 = piVar4 + 2;
  if (piVar4 == (int *)0x0) {
    piVar3 = (int *)0x0;
  }
  if (piVar3 != (int *)0x0) {
    uVar1 = *(int *)(param_1 + 0x588) + *piVar3;
    uVar2 = (ulonglong)uVar1;
    if ((*(int *)(param_1 + 0x4e4) <= (int)uVar1) && (*(char *)((longlong)piVar3 + 9) != '\0')) {
      return CONCAT71((uint7)(uint3)(uVar1 >> 8),1);
    }
  }
  return uVar2 & 0xffffffffffffff00;
}



// ---- FUN_141012a80 @ 141012a80 ----

undefined8 * FUN_141012a80(undefined8 *param_1)

{
  undefined8 uVar1;
  
  uVar1 = FUN_142040a20();
  FUN_140feb340(param_1,uVar1);
  *param_1 = &PTR_FUN_143c5c908;
  param_1[9] = 0;
  param_1[10] = 0;
  *(undefined4 *)(param_1 + 6) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x34) = 0;
  param_1[8] = 0;
  *(undefined4 *)(param_1 + 10) = 0;
  if ((*(int *)((longlong)param_1 + 0x54) < 0) && (*(int *)((longlong)param_1 + 0x54) != 0)) {
    FUN_1407f1af0(param_1 + 9,0);
  }
  if ((undefined2 *)param_1[9] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[9] = 0;
  }
  *(undefined4 *)(param_1 + 7) = 0;
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  *(undefined4 *)(param_1 + 0xb) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x5c) = 0;
  param_1[0xd] = 0;
  *(undefined4 *)(param_1 + 0xf) = 0;
  if ((*(int *)((longlong)param_1 + 0x7c) < 0) && (*(int *)((longlong)param_1 + 0x7c) != 0)) {
    FUN_1407f1af0(param_1 + 0xe,0);
  }
  if ((undefined2 *)param_1[0xe] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0xe] = 0;
  }
  *(undefined4 *)(param_1 + 0xc) = 0;
  param_1[0x13] = 0;
  param_1[0x14] = 0;
  *(undefined4 *)(param_1 + 0x10) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x84) = 0;
  param_1[0x12] = 0;
  *(undefined4 *)(param_1 + 0x14) = 0;
  if ((*(int *)((longlong)param_1 + 0xa4) < 0) && (*(int *)((longlong)param_1 + 0xa4) != 0)) {
    FUN_1407f1af0(param_1 + 0x13,0);
  }
  if ((undefined2 *)param_1[0x13] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x13] = 0;
  }
  *(undefined4 *)(param_1 + 0x11) = 0;
  param_1[0x18] = 0;
  param_1[0x19] = 0;
  *(undefined4 *)(param_1 + 0x15) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xac) = 0;
  param_1[0x17] = 0;
  *(undefined4 *)(param_1 + 0x19) = 0;
  if ((*(int *)((longlong)param_1 + 0xcc) < 0) && (*(int *)((longlong)param_1 + 0xcc) != 0)) {
    FUN_1407f1af0(param_1 + 0x18,0);
  }
  if ((undefined2 *)param_1[0x18] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x18] = 0;
  }
  *(undefined4 *)(param_1 + 0x16) = 0;
  param_1[0x1d] = 0;
  param_1[0x1e] = 0;
  *(undefined4 *)(param_1 + 0x1a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xd4) = 0;
  param_1[0x1c] = 0;
  *(undefined4 *)(param_1 + 0x1e) = 0;
  if ((*(int *)((longlong)param_1 + 0xf4) < 0) && (*(int *)((longlong)param_1 + 0xf4) != 0)) {
    FUN_1407f1af0(param_1 + 0x1d,0);
  }
  if ((undefined2 *)param_1[0x1d] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x1d] = 0;
  }
  *(undefined4 *)(param_1 + 0x1b) = 0;
  param_1[0x22] = 0;
  param_1[0x23] = 0;
  *(undefined4 *)(param_1 + 0x1f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xfc) = 0;
  param_1[0x21] = 0;
  *(undefined4 *)(param_1 + 0x23) = 0;
  if ((*(int *)((longlong)param_1 + 0x11c) < 0) && (*(int *)((longlong)param_1 + 0x11c) != 0)) {
    FUN_1407f1af0(param_1 + 0x22,0);
  }
  if ((undefined2 *)param_1[0x22] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x22] = 0;
  }
  *(undefined4 *)(param_1 + 0x20) = 0;
  param_1[0x27] = 0;
  param_1[0x28] = 0;
  *(undefined4 *)(param_1 + 0x24) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x124) = 0;
  param_1[0x26] = 0;
  *(undefined4 *)(param_1 + 0x28) = 0;
  if ((*(int *)((longlong)param_1 + 0x144) < 0) && (*(int *)((longlong)param_1 + 0x144) != 0)) {
    FUN_1407f1af0(param_1 + 0x27,0);
  }
  if ((undefined2 *)param_1[0x27] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x27] = 0;
  }
  *(undefined4 *)(param_1 + 0x25) = 0;
  param_1[0x2c] = 0;
  param_1[0x2d] = 0;
  *(undefined4 *)(param_1 + 0x29) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x14c) = 0;
  param_1[0x2b] = 0;
  *(undefined4 *)(param_1 + 0x2d) = 0;
  if ((*(int *)((longlong)param_1 + 0x16c) < 0) && (*(int *)((longlong)param_1 + 0x16c) != 0)) {
    FUN_1407f1af0(param_1 + 0x2c,0);
  }
  if ((undefined2 *)param_1[0x2c] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x2c] = 0;
  }
  *(undefined4 *)(param_1 + 0x2a) = 0;
  param_1[0x31] = 0;
  param_1[0x32] = 0;
  *(undefined4 *)(param_1 + 0x2e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x174) = 0;
  param_1[0x30] = 0;
  *(undefined4 *)(param_1 + 0x32) = 0;
  if ((*(int *)((longlong)param_1 + 0x194) < 0) && (*(int *)((longlong)param_1 + 0x194) != 0)) {
    FUN_1407f1af0(param_1 + 0x31,0);
  }
  if ((undefined2 *)param_1[0x31] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x31] = 0;
  }
  *(undefined4 *)(param_1 + 0x2f) = 0;
  param_1[0x36] = 0;
  param_1[0x37] = 0;
  *(undefined4 *)(param_1 + 0x33) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x19c) = 0;
  param_1[0x35] = 0;
  *(undefined4 *)(param_1 + 0x37) = 0;
  if ((*(int *)((longlong)param_1 + 0x1bc) < 0) && (*(int *)((longlong)param_1 + 0x1bc) != 0)) {
    FUN_1407f1af0(param_1 + 0x36,0);
  }
  if ((undefined2 *)param_1[0x36] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x36] = 0;
  }
  *(undefined4 *)(param_1 + 0x34) = 0;
  param_1[0x3b] = 0;
  param_1[0x3c] = 0;
  *(undefined4 *)(param_1 + 0x38) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1c4) = 0;
  param_1[0x3a] = 0;
  *(undefined4 *)(param_1 + 0x3c) = 0;
  if ((*(int *)((longlong)param_1 + 0x1e4) < 0) && (*(int *)((longlong)param_1 + 0x1e4) != 0)) {
    FUN_1407f1af0(param_1 + 0x3b,0);
  }
  if ((undefined2 *)param_1[0x3b] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x3b] = 0;
  }
  *(undefined4 *)(param_1 + 0x39) = 0;
  param_1[0x40] = 0;
  param_1[0x41] = 0;
  *(undefined4 *)(param_1 + 0x3d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1ec) = 0;
  param_1[0x3f] = 0;
  *(undefined4 *)(param_1 + 0x41) = 0;
  if ((*(int *)((longlong)param_1 + 0x20c) < 0) && (*(int *)((longlong)param_1 + 0x20c) != 0)) {
    FUN_1407f1af0(param_1 + 0x40,0);
  }
  if ((undefined2 *)param_1[0x40] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x40] = 0;
  }
  *(undefined4 *)(param_1 + 0x3e) = 0;
  param_1[0x45] = 0;
  param_1[0x46] = 0;
  *(undefined4 *)(param_1 + 0x42) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x214) = 0;
  param_1[0x44] = 0;
  *(undefined4 *)(param_1 + 0x46) = 0;
  if ((*(int *)((longlong)param_1 + 0x234) < 0) && (*(int *)((longlong)param_1 + 0x234) != 0)) {
    FUN_1407f1af0(param_1 + 0x45,0);
  }
  if ((undefined2 *)param_1[0x45] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x45] = 0;
  }
  *(undefined4 *)(param_1 + 0x43) = 0;
  param_1[0x4a] = 0;
  param_1[0x4b] = 0;
  *(undefined4 *)(param_1 + 0x47) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x23c) = 0;
  param_1[0x49] = 0;
  *(undefined4 *)(param_1 + 0x4b) = 0;
  if ((*(int *)((longlong)param_1 + 0x25c) < 0) && (*(int *)((longlong)param_1 + 0x25c) != 0)) {
    FUN_1407f1af0(param_1 + 0x4a,0);
  }
  if ((undefined2 *)param_1[0x4a] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x4a] = 0;
  }
  *(undefined4 *)(param_1 + 0x48) = 0;
  param_1[0x4f] = 0;
  param_1[0x50] = 0;
  *(undefined4 *)(param_1 + 0x4c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x264) = 0;
  param_1[0x4e] = 0;
  *(undefined4 *)(param_1 + 0x50) = 0;
  if ((*(int *)((longlong)param_1 + 0x284) < 0) && (*(int *)((longlong)param_1 + 0x284) != 0)) {
    FUN_1407f1af0(param_1 + 0x4f,0);
  }
  if ((undefined2 *)param_1[0x4f] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0x4f] = 0;
  }
  *(undefined4 *)(param_1 + 0x4d) = 0;
  param_1[0x53] = 0;
  param_1[0x54] = 0;
  param_1[0x55] = 0;
  *(undefined4 *)(param_1 + 0x51) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x28c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x52) = 0;
  param_1[0x58] = 0;
  param_1[0x59] = 0;
  param_1[0x5a] = 0;
  *(undefined4 *)(param_1 + 0x56) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2b4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x57) = 0;
  param_1[0x5d] = 0;
  param_1[0x5e] = 0;
  param_1[0x5f] = 0;
  *(undefined4 *)(param_1 + 0x5b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2dc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5c) = 0;
  param_1[0x62] = 0;
  param_1[99] = 0;
  param_1[100] = 0;
  *(undefined4 *)(param_1 + 0x60) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x304) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x61) = 0;
  param_1[0x67] = 0;
  param_1[0x68] = 0;
  param_1[0x69] = 0;
  *(undefined4 *)(param_1 + 0x65) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x32c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x66) = 0;
  param_1[0x6c] = 0;
  param_1[0x6d] = 0;
  param_1[0x6e] = 0;
  *(undefined4 *)(param_1 + 0x6a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x354) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6b) = 0;
  param_1[0x71] = 0;
  param_1[0x72] = 0;
  param_1[0x73] = 0;
  *(undefined4 *)(param_1 + 0x6f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x37c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x70) = 0;
  param_1[0x76] = 0;
  param_1[0x77] = 0;
  param_1[0x78] = 0;
  *(undefined4 *)(param_1 + 0x74) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3a4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x75) = 0;
  param_1[0x7b] = 0;
  param_1[0x7c] = 0;
  param_1[0x7d] = 0;
  *(undefined4 *)(param_1 + 0x79) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3cc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7a) = 0;
  param_1[0x80] = 0;
  param_1[0x81] = 0;
  param_1[0x82] = 0;
  *(undefined4 *)(param_1 + 0x7e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3f4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7f) = 0;
  param_1[0x85] = 0;
  param_1[0x86] = 0;
  param_1[0x87] = 0;
  *(undefined4 *)(param_1 + 0x83) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x41c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x84) = 0;
  param_1[0x8a] = 0;
  param_1[0x8b] = 0;
  param_1[0x8c] = 0;
  *(undefined4 *)(param_1 + 0x88) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x444) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x89) = 0;
  param_1[0x8f] = 0;
  param_1[0x90] = 0;
  param_1[0x91] = 0;
  *(undefined4 *)(param_1 + 0x8d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x46c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x8e) = 0;
  param_1[0x94] = 0;
  param_1[0x95] = 0;
  param_1[0x96] = 0;
  *(undefined4 *)(param_1 + 0x92) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x494) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x93) = 0;
  param_1[0x99] = 0;
  param_1[0x9a] = 0;
  param_1[0x9b] = 0;
  *(undefined4 *)(param_1 + 0x97) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x4bc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x98) = 0;
  param_1[0x9e] = 0;
  param_1[0x9f] = 0;
  param_1[0xa0] = 0;
  *(undefined4 *)(param_1 + 0x9c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x4e4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x9d) = 0;
  param_1[0xa3] = 0;
  param_1[0xa4] = 0;
  param_1[0xa5] = 0;
  *(undefined4 *)(param_1 + 0xa1) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x50c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xa2) = 0;
  param_1[0xa8] = 0;
  param_1[0xa9] = 0;
  param_1[0xaa] = 0;
  *(undefined4 *)(param_1 + 0xa6) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x534) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xa7) = 0;
  param_1[0xad] = 0;
  param_1[0xae] = 0;
  param_1[0xaf] = 0;
  *(undefined4 *)(param_1 + 0xab) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x55c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xac) = 0;
  param_1[0xb2] = 0;
  param_1[0xb3] = 0;
  param_1[0xb4] = 0;
  *(undefined4 *)(param_1 + 0xb0) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x584) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xb1) = 0;
  param_1[0xb7] = 0;
  param_1[0xb8] = 0;
  param_1[0xb9] = 0;
  *(undefined4 *)(param_1 + 0xb5) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x5ac) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xb6) = 0;
  param_1[0xbc] = 0;
  param_1[0xbd] = 0;
  param_1[0xbe] = 0;
  *(undefined4 *)(param_1 + 0xba) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x5d4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xbb) = 0;
  param_1[0xc1] = 0;
  param_1[0xc2] = 0;
  param_1[0xc3] = 0;
  *(undefined4 *)(param_1 + 0xbf) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x5fc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xc0) = 0;
  param_1[0xc6] = 0;
  param_1[199] = 0;
  param_1[200] = 0;
  *(undefined4 *)(param_1 + 0xc4) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x624) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xc5) = 0;
  param_1[0xcb] = 0;
  param_1[0xcc] = 0;
  param_1[0xcd] = 0;
  *(undefined4 *)(param_1 + 0xc9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x64c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xca) = 0;
  param_1[0xd0] = 0;
  param_1[0xd1] = 0;
  param_1[0xd2] = 0;
  *(undefined4 *)(param_1 + 0xce) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x674) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xcf) = 0;
  param_1[0xd5] = 0;
  param_1[0xd6] = 0;
  param_1[0xd7] = 0;
  *(undefined4 *)(param_1 + 0xd3) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x69c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xd4) = 0;
  param_1[0xda] = 0;
  param_1[0xdb] = 0;
  param_1[0xdc] = 0;
  *(undefined4 *)(param_1 + 0xd8) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x6c4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xd9) = 0;
  param_1[0xdf] = 0;
  param_1[0xe0] = 0;
  param_1[0xe1] = 0;
  *(undefined4 *)(param_1 + 0xdd) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x6ec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xde) = 0;
  param_1[0xe4] = 0;
  param_1[0xe5] = 0;
  param_1[0xe6] = 0;
  *(undefined4 *)(param_1 + 0xe2) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x714) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xe3) = 0;
  param_1[0xe9] = 0;
  param_1[0xea] = 0;
  param_1[0xeb] = 0;
  *(undefined4 *)(param_1 + 0xe7) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x73c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xe8) = 0;
  param_1[0xee] = 0;
  param_1[0xef] = 0;
  param_1[0xf0] = 0;
  *(undefined4 *)(param_1 + 0xec) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x764) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xed) = 0;
  param_1[0xf3] = 0;
  param_1[0xf4] = 0;
  param_1[0xf5] = 0;
  *(undefined4 *)(param_1 + 0xf1) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x78c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xf2) = 0;
  param_1[0xf8] = 0;
  param_1[0xf9] = 0;
  param_1[0xfa] = 0;
  *(undefined4 *)(param_1 + 0xf6) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x7b4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xf7) = 0;
  param_1[0xfd] = 0;
  param_1[0xfe] = 0;
  param_1[0xff] = 0;
  *(undefined4 *)(param_1 + 0xfb) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x7dc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0xfc) = 0;
  param_1[0x102] = 0;
  param_1[0x103] = 0;
  param_1[0x104] = 0;
  *(undefined4 *)(param_1 + 0x100) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x804) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x101) = 0;
  param_1[0x107] = 0;
  param_1[0x108] = 0;
  param_1[0x109] = 0;
  *(undefined4 *)(param_1 + 0x105) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x82c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x106) = 0;
  param_1[0x10c] = 0;
  param_1[0x10d] = 0;
  param_1[0x10e] = 0;
  *(undefined4 *)(param_1 + 0x10a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x854) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x10b) = 0;
  param_1[0x111] = 0;
  param_1[0x112] = 0;
  param_1[0x113] = 0;
  *(undefined4 *)(param_1 + 0x10f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x87c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x110) = 0;
  param_1[0x116] = 0;
  param_1[0x117] = 0;
  param_1[0x118] = 0;
  *(undefined4 *)(param_1 + 0x114) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x8a4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x115) = 0;
  param_1[0x11b] = 0;
  param_1[0x11c] = 0;
  param_1[0x11d] = 0;
  *(undefined4 *)(param_1 + 0x119) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x8cc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x11a) = 0;
  param_1[0x120] = 0;
  param_1[0x121] = 0;
  param_1[0x122] = 0;
  *(undefined4 *)(param_1 + 0x11e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x8f4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x11f) = 0;
  param_1[0x125] = 0;
  param_1[0x126] = 0;
  param_1[0x127] = 0;
  *(undefined4 *)(param_1 + 0x123) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x91c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x124) = 0;
  param_1[0x12a] = 0;
  param_1[299] = 0;
  param_1[300] = 0;
  *(undefined4 *)(param_1 + 0x128) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x944) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x129) = 0;
  param_1[0x12f] = 0;
  param_1[0x130] = 0;
  param_1[0x131] = 0;
  *(undefined4 *)(param_1 + 0x12d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x96c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x12e) = 0;
  param_1[0x134] = 0;
  param_1[0x135] = 0;
  param_1[0x136] = 0;
  *(undefined4 *)(param_1 + 0x132) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x994) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x133) = 0;
  param_1[0x139] = 0;
  param_1[0x13a] = 0;
  param_1[0x13b] = 0;
  *(undefined4 *)(param_1 + 0x137) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x9bc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x138) = 0;
  param_1[0x13e] = 0;
  param_1[0x13f] = 0;
  param_1[0x140] = 0;
  *(undefined4 *)(param_1 + 0x13c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x9e4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x13d) = 0;
  param_1[0x141] = 0;
  param_1[0x142] = 0;
  FUN_1407e5330(param_1 + 0x143,0,0);
  *(undefined4 *)(param_1 + 0x147) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xa3c) = 0;
  param_1[0x149] = 0;
  *(undefined4 *)(param_1 + 0x14a) = 0;
  param_1[0x14b] = 0;
  param_1[0x14c] = 0;
  FUN_1407e5330(param_1 + 0x14d,0,0);
  *(undefined4 *)(param_1 + 0x151) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xa8c) = 0;
  param_1[0x153] = 0;
  *(undefined4 *)(param_1 + 0x154) = 0;
  param_1[0x157] = 0;
  param_1[0x158] = 0;
  param_1[0x159] = 0;
  *(undefined4 *)(param_1 + 0x155) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xaac) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x156) = 0;
  param_1[0x15c] = 0;
  param_1[0x15d] = 0;
  param_1[0x15e] = 0;
  *(undefined4 *)(param_1 + 0x15a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xad4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x15b) = 0;
  param_1[0x161] = 0;
  param_1[0x162] = 0;
  param_1[0x163] = 0;
  *(undefined4 *)(param_1 + 0x15f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xafc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x160) = 0;
  param_1[0x166] = 0;
  param_1[0x167] = 0;
  param_1[0x168] = 0;
  *(undefined4 *)(param_1 + 0x164) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xb24) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x165) = 0;
  param_1[0x16b] = 0;
  param_1[0x16c] = 0;
  param_1[0x16d] = 0;
  *(undefined4 *)(param_1 + 0x169) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xb4c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x16a) = 0;
  param_1[0x170] = 0;
  param_1[0x171] = 0;
  param_1[0x172] = 0;
  *(undefined4 *)(param_1 + 0x16e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xb74) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x16f) = 0;
  param_1[0x175] = 0;
  param_1[0x176] = 0;
  param_1[0x177] = 0;
  *(undefined4 *)(param_1 + 0x173) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xb9c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x174) = 0;
  param_1[0x17a] = 0;
  param_1[0x17b] = 0;
  param_1[0x17c] = 0;
  *(undefined4 *)(param_1 + 0x178) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xbc4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x179) = 0;
  param_1[0x17f] = 0;
  param_1[0x180] = 0;
  param_1[0x181] = 0;
  *(undefined4 *)(param_1 + 0x17d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xbec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x17e) = 0;
  param_1[0x184] = 0;
  param_1[0x185] = 0;
  param_1[0x186] = 0;
  *(undefined4 *)(param_1 + 0x182) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xc14) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x183) = 0;
  param_1[0x189] = 0;
  param_1[0x18a] = 0;
  param_1[0x18b] = 0;
  *(undefined4 *)(param_1 + 0x187) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xc3c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x188) = 0;
  param_1[0x18e] = 0;
  param_1[399] = 0;
  param_1[400] = 0;
  *(undefined4 *)(param_1 + 0x18c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xc64) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x18d) = 0;
  param_1[0x193] = 0;
  param_1[0x194] = 0;
  param_1[0x195] = 0;
  *(undefined4 *)(param_1 + 0x191) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xc8c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x192) = 0;
  param_1[0x198] = 0;
  param_1[0x199] = 0;
  param_1[0x19a] = 0;
  *(undefined4 *)(param_1 + 0x196) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xcb4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x197) = 0;
  param_1[0x19d] = 0;
  param_1[0x19e] = 0;
  param_1[0x19f] = 0;
  *(undefined4 *)(param_1 + 0x19b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xcdc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x19c) = 0;
  param_1[0x1a2] = 0;
  param_1[0x1a3] = 0;
  param_1[0x1a4] = 0;
  *(undefined4 *)(param_1 + 0x1a0) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xd04) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1a1) = 0;
  param_1[0x1a7] = 0;
  param_1[0x1a8] = 0;
  param_1[0x1a9] = 0;
  *(undefined4 *)(param_1 + 0x1a5) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xd2c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1a6) = 0;
  param_1[0x1ac] = 0;
  param_1[0x1ad] = 0;
  param_1[0x1ae] = 0;
  *(undefined4 *)(param_1 + 0x1aa) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xd54) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1ab) = 0;
  param_1[0x1b1] = 0;
  param_1[0x1b2] = 0;
  param_1[0x1b3] = 0;
  *(undefined4 *)(param_1 + 0x1af) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xd7c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1b0) = 0;
  param_1[0x1b6] = 0;
  param_1[0x1b7] = 0;
  param_1[0x1b8] = 0;
  *(undefined4 *)(param_1 + 0x1b4) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xda4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1b5) = 0;
  param_1[0x1bb] = 0;
  param_1[0x1bc] = 0;
  param_1[0x1bd] = 0;
  *(undefined4 *)(param_1 + 0x1b9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xdcc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1ba) = 0;
  param_1[0x1c0] = 0;
  param_1[0x1c1] = 0;
  param_1[0x1c2] = 0;
  *(undefined4 *)(param_1 + 0x1be) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xdf4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1bf) = 0;
  param_1[0x1c5] = 0;
  param_1[0x1c6] = 0;
  param_1[0x1c7] = 0;
  *(undefined4 *)(param_1 + 0x1c3) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xe1c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1c4) = 0;
  param_1[0x1ca] = 0;
  param_1[0x1cb] = 0;
  param_1[0x1cc] = 0;
  *(undefined4 *)(param_1 + 0x1c8) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xe44) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1c9) = 0;
  param_1[0x1cf] = 0;
  param_1[0x1d0] = 0;
  param_1[0x1d1] = 0;
  *(undefined4 *)(param_1 + 0x1cd) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xe6c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1ce) = 0;
  param_1[0x1d4] = 0;
  param_1[0x1d5] = 0;
  param_1[0x1d6] = 0;
  *(undefined4 *)(param_1 + 0x1d2) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xe94) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1d3) = 0;
  param_1[0x1d9] = 0;
  param_1[0x1da] = 0;
  param_1[0x1db] = 0;
  *(undefined4 *)(param_1 + 0x1d7) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xebc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1d8) = 0;
  param_1[0x1de] = 0;
  param_1[0x1df] = 0;
  param_1[0x1e0] = 0;
  *(undefined4 *)(param_1 + 0x1dc) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xee4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1dd) = 0;
  param_1[0x1e3] = 0;
  param_1[0x1e4] = 0;
  param_1[0x1e5] = 0;
  *(undefined4 *)(param_1 + 0x1e1) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xf0c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1e2) = 0;
  param_1[0x1e8] = 0;
  param_1[0x1e9] = 0;
  param_1[0x1ea] = 0;
  *(undefined4 *)(param_1 + 0x1e6) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xf34) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1e7) = 0;
  param_1[0x1ed] = 0;
  param_1[0x1ee] = 0;
  param_1[0x1ef] = 0;
  *(undefined4 *)(param_1 + 0x1eb) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xf5c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1ec) = 0;
  param_1[0x1f2] = 0;
  param_1[499] = 0;
  param_1[500] = 0;
  *(undefined4 *)(param_1 + 0x1f0) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xf84) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1f1) = 0;
  param_1[0x1f7] = 0;
  param_1[0x1f8] = 0;
  param_1[0x1f9] = 0;
  *(undefined4 *)(param_1 + 0x1f5) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xfac) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1f6) = 0;
  param_1[0x1fc] = 0;
  param_1[0x1fd] = 0;
  param_1[0x1fe] = 0;
  *(undefined4 *)(param_1 + 0x1fa) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xfd4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x1fb) = 0;
  param_1[0x201] = 0;
  param_1[0x202] = 0;
  param_1[0x203] = 0;
  *(undefined4 *)(param_1 + 0x1ff) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xffc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x200) = 0;
  param_1[0x206] = 0;
  param_1[0x207] = 0;
  param_1[0x208] = 0;
  *(undefined4 *)(param_1 + 0x204) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1024) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x205) = 0;
  param_1[0x20b] = 0;
  param_1[0x20c] = 0;
  param_1[0x20d] = 0;
  *(undefined4 *)(param_1 + 0x209) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x104c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x20a) = 0;
  param_1[0x210] = 0;
  param_1[0x211] = 0;
  param_1[0x212] = 0;
  *(undefined4 *)(param_1 + 0x20e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1074) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x20f) = 0;
  param_1[0x215] = 0;
  param_1[0x216] = 0;
  param_1[0x217] = 0;
  *(undefined4 *)(param_1 + 0x213) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x109c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x214) = 0;
  param_1[0x21a] = 0;
  param_1[0x21b] = 0;
  param_1[0x21c] = 0;
  *(undefined4 *)(param_1 + 0x218) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x10c4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x219) = 0;
  param_1[0x21f] = 0;
  param_1[0x220] = 0;
  param_1[0x221] = 0;
  *(undefined4 *)(param_1 + 0x21d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x10ec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x21e) = 0;
  param_1[0x224] = 0;
  param_1[0x225] = 0;
  param_1[0x226] = 0;
  *(undefined4 *)(param_1 + 0x222) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1114) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x223) = 0;
  param_1[0x229] = 0;
  param_1[0x22a] = 0;
  param_1[0x22b] = 0;
  *(undefined4 *)(param_1 + 0x227) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x113c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x228) = 0;
  param_1[0x22e] = 0;
  param_1[0x22f] = 0;
  param_1[0x230] = 0;
  *(undefined4 *)(param_1 + 0x22c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1164) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x22d) = 0;
  param_1[0x233] = 0;
  param_1[0x234] = 0;
  param_1[0x235] = 0;
  *(undefined4 *)(param_1 + 0x231) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x118c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x232) = 0;
  param_1[0x238] = 0;
  param_1[0x239] = 0;
  param_1[0x23a] = 0;
  *(undefined4 *)(param_1 + 0x236) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x11b4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x237) = 0;
  param_1[0x23d] = 0;
  param_1[0x23e] = 0;
  param_1[0x23f] = 0;
  *(undefined4 *)(param_1 + 0x23b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x11dc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x23c) = 0;
  param_1[0x242] = 0;
  param_1[0x243] = 0;
  param_1[0x244] = 0;
  *(undefined4 *)(param_1 + 0x240) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1204) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x241) = 0;
  param_1[0x247] = 0;
  param_1[0x248] = 0;
  param_1[0x249] = 0;
  *(undefined4 *)(param_1 + 0x245) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x122c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x246) = 0;
  param_1[0x24c] = 0;
  param_1[0x24d] = 0;
  param_1[0x24e] = 0;
  *(undefined4 *)(param_1 + 0x24a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1254) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x24b) = 0;
  param_1[0x251] = 0;
  param_1[0x252] = 0;
  param_1[0x253] = 0;
  *(undefined4 *)(param_1 + 0x24f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x127c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x250) = 0;
  param_1[0x256] = 0;
  param_1[599] = 0;
  param_1[600] = 0;
  *(undefined4 *)(param_1 + 0x254) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x12a4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x255) = 0;
  param_1[0x25b] = 0;
  param_1[0x25c] = 0;
  param_1[0x25d] = 0;
  *(undefined4 *)(param_1 + 0x259) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x12cc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x25a) = 0;
  param_1[0x260] = 0;
  param_1[0x261] = 0;
  param_1[0x262] = 0;
  *(undefined4 *)(param_1 + 0x25e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x12f4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x25f) = 0;
  param_1[0x265] = 0;
  param_1[0x266] = 0;
  param_1[0x267] = 0;
  *(undefined4 *)(param_1 + 0x263) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x131c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x264) = 0;
  param_1[0x26a] = 0;
  param_1[0x26b] = 0;
  param_1[0x26c] = 0;
  *(undefined4 *)(param_1 + 0x268) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1344) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x269) = 0;
  param_1[0x26f] = 0;
  param_1[0x270] = 0;
  param_1[0x271] = 0;
  *(undefined4 *)(param_1 + 0x26d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x136c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x26e) = 0;
  param_1[0x274] = 0;
  param_1[0x275] = 0;
  param_1[0x276] = 0;
  *(undefined4 *)(param_1 + 0x272) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1394) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x273) = 0;
  param_1[0x279] = 0;
  param_1[0x27a] = 0;
  param_1[0x27b] = 0;
  *(undefined4 *)(param_1 + 0x277) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x13bc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x278) = 0;
  param_1[0x27e] = 0;
  param_1[0x27f] = 0;
  param_1[0x280] = 0;
  *(undefined4 *)(param_1 + 0x27c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x13e4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x27d) = 0;
  param_1[0x283] = 0;
  param_1[0x284] = 0;
  param_1[0x285] = 0;
  *(undefined4 *)(param_1 + 0x281) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x140c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x282) = 0;
  param_1[0x288] = 0;
  param_1[0x289] = 0;
  param_1[0x28a] = 0;
  *(undefined4 *)(param_1 + 0x286) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1434) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x287) = 0;
  param_1[0x28d] = 0;
  param_1[0x28e] = 0;
  param_1[0x28f] = 0;
  *(undefined4 *)(param_1 + 0x28b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x145c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x28c) = 0;
  param_1[0x292] = 0;
  param_1[0x293] = 0;
  param_1[0x294] = 0;
  *(undefined4 *)(param_1 + 0x290) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1484) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x291) = 0;
  param_1[0x297] = 0;
  param_1[0x298] = 0;
  param_1[0x299] = 0;
  *(undefined4 *)(param_1 + 0x295) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x14ac) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x296) = 0;
  param_1[0x29c] = 0;
  param_1[0x29d] = 0;
  param_1[0x29e] = 0;
  *(undefined4 *)(param_1 + 0x29a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x14d4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x29b) = 0;
  param_1[0x2a1] = 0;
  param_1[0x2a2] = 0;
  param_1[0x2a3] = 0;
  *(undefined4 *)(param_1 + 0x29f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x14fc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2a0) = 0;
  param_1[0x2a6] = 0;
  param_1[0x2a7] = 0;
  param_1[0x2a8] = 0;
  *(undefined4 *)(param_1 + 0x2a4) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1524) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2a5) = 0;
  param_1[0x2ab] = 0;
  param_1[0x2ac] = 0;
  param_1[0x2ad] = 0;
  *(undefined4 *)(param_1 + 0x2a9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x154c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2aa) = 0;
  param_1[0x2b0] = 0;
  param_1[0x2b1] = 0;
  param_1[0x2b2] = 0;
  *(undefined4 *)(param_1 + 0x2ae) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1574) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2af) = 0;
  param_1[0x2b5] = 0;
  param_1[0x2b6] = 0;
  param_1[0x2b7] = 0;
  *(undefined4 *)(param_1 + 0x2b3) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x159c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2b4) = 0;
  param_1[0x2ba] = 0;
  param_1[699] = 0;
  param_1[700] = 0;
  *(undefined4 *)(param_1 + 0x2b8) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x15c4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2b9) = 0;
  param_1[0x2bf] = 0;
  param_1[0x2c0] = 0;
  param_1[0x2c1] = 0;
  *(undefined4 *)(param_1 + 0x2bd) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x15ec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2be) = 0;
  param_1[0x2c4] = 0;
  param_1[0x2c5] = 0;
  param_1[0x2c6] = 0;
  *(undefined4 *)(param_1 + 0x2c2) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1614) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2c3) = 0;
  param_1[0x2c9] = 0;
  param_1[0x2ca] = 0;
  param_1[0x2cb] = 0;
  *(undefined4 *)(param_1 + 0x2c7) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x163c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2c8) = 0;
  param_1[0x2ce] = 0;
  param_1[0x2cf] = 0;
  param_1[0x2d0] = 0;
  *(undefined4 *)(param_1 + 0x2cc) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1664) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2cd) = 0;
  param_1[0x2d3] = 0;
  param_1[0x2d4] = 0;
  param_1[0x2d5] = 0;
  *(undefined4 *)(param_1 + 0x2d1) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x168c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2d2) = 0;
  param_1[0x2d8] = 0;
  param_1[0x2d9] = 0;
  param_1[0x2da] = 0;
  *(undefined4 *)(param_1 + 0x2d6) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x16b4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2d7) = 0;
  param_1[0x2dd] = 0;
  param_1[0x2de] = 0;
  param_1[0x2df] = 0;
  *(undefined4 *)(param_1 + 0x2db) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x16dc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2dc) = 0;
  param_1[0x2e2] = 0;
  param_1[0x2e3] = 0;
  param_1[0x2e4] = 0;
  *(undefined4 *)(param_1 + 0x2e0) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1704) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2e1) = 0;
  param_1[0x2e7] = 0;
  param_1[0x2e8] = 0;
  param_1[0x2e9] = 0;
  *(undefined4 *)(param_1 + 0x2e5) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x172c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2e6) = 0;
  param_1[0x2ec] = 0;
  param_1[0x2ed] = 0;
  param_1[0x2ee] = 0;
  *(undefined4 *)(param_1 + 0x2ea) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1754) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2eb) = 0;
  param_1[0x2f1] = 0;
  param_1[0x2f2] = 0;
  param_1[0x2f3] = 0;
  *(undefined4 *)(param_1 + 0x2ef) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x177c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2f0) = 0;
  param_1[0x2f6] = 0;
  param_1[0x2f7] = 0;
  param_1[0x2f8] = 0;
  *(undefined4 *)(param_1 + 0x2f4) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x17a4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2f5) = 0;
  param_1[0x2fb] = 0;
  param_1[0x2fc] = 0;
  param_1[0x2fd] = 0;
  *(undefined4 *)(param_1 + 0x2f9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x17cc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2fa) = 0;
  param_1[0x300] = 0;
  param_1[0x301] = 0;
  param_1[0x302] = 0;
  *(undefined4 *)(param_1 + 0x2fe) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x17f4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x2ff) = 0;
  param_1[0x305] = 0;
  param_1[0x306] = 0;
  param_1[0x307] = 0;
  *(undefined4 *)(param_1 + 0x303) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x181c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x304) = 0;
  param_1[0x30a] = 0;
  param_1[0x30b] = 0;
  param_1[0x30c] = 0;
  *(undefined4 *)(param_1 + 0x308) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1844) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x309) = 0;
  param_1[0x30f] = 0;
  param_1[0x310] = 0;
  param_1[0x311] = 0;
  *(undefined4 *)(param_1 + 0x30d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x186c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x30e) = 0;
  param_1[0x314] = 0;
  param_1[0x315] = 0;
  param_1[0x316] = 0;
  *(undefined4 *)(param_1 + 0x312) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1894) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x313) = 0;
  param_1[0x319] = 0;
  param_1[0x31a] = 0;
  param_1[0x31b] = 0;
  *(undefined4 *)(param_1 + 0x317) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x18bc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x318) = 0;
  param_1[0x31e] = 0;
  param_1[799] = 0;
  param_1[800] = 0;
  *(undefined4 *)(param_1 + 0x31c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x18e4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x31d) = 0;
  param_1[0x323] = 0;
  param_1[0x324] = 0;
  param_1[0x325] = 0;
  *(undefined4 *)(param_1 + 0x321) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x190c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x322) = 0;
  param_1[0x328] = 0;
  param_1[0x329] = 0;
  param_1[0x32a] = 0;
  *(undefined4 *)(param_1 + 0x326) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1934) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x327) = 0;
  param_1[0x32d] = 0;
  param_1[0x32e] = 0;
  param_1[0x32f] = 0;
  *(undefined4 *)(param_1 + 0x32b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x195c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x32c) = 0;
  param_1[0x332] = 0;
  param_1[0x333] = 0;
  param_1[0x334] = 0;
  *(undefined4 *)(param_1 + 0x330) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1984) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x331) = 0;
  param_1[0x337] = 0;
  param_1[0x338] = 0;
  param_1[0x339] = 0;
  *(undefined4 *)(param_1 + 0x335) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x19ac) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x336) = 0;
  param_1[0x33c] = 0;
  param_1[0x33d] = 0;
  param_1[0x33e] = 0;
  *(undefined4 *)(param_1 + 0x33a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x19d4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x33b) = 0;
  param_1[0x341] = 0;
  param_1[0x342] = 0;
  param_1[0x343] = 0;
  *(undefined4 *)(param_1 + 0x33f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x19fc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x340) = 0;
  param_1[0x346] = 0;
  param_1[0x347] = 0;
  param_1[0x348] = 0;
  *(undefined4 *)(param_1 + 0x344) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1a24) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x345) = 0;
  param_1[0x34b] = 0;
  param_1[0x34c] = 0;
  param_1[0x34d] = 0;
  *(undefined4 *)(param_1 + 0x349) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1a4c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x34a) = 0;
  param_1[0x350] = 0;
  param_1[0x351] = 0;
  param_1[0x352] = 0;
  *(undefined4 *)(param_1 + 0x34e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1a74) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x34f) = 0;
  param_1[0x355] = 0;
  param_1[0x356] = 0;
  param_1[0x357] = 0;
  *(undefined4 *)(param_1 + 0x353) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1a9c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x354) = 0;
  param_1[0x35c] = 0;
  param_1[0x35d] = 0;
  param_1[0x35e] = 0;
  *(undefined4 *)(param_1 + 0x35a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1ad4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x35b) = 0;
  param_1[0x361] = 0;
  param_1[0x362] = 0;
  param_1[0x363] = 0;
  *(undefined4 *)(param_1 + 0x35f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1afc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x360) = 0;
  param_1[0x366] = 0;
  param_1[0x367] = 0;
  param_1[0x368] = 0;
  *(undefined4 *)(param_1 + 0x364) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1b24) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x365) = 0;
  param_1[0x36b] = 0;
  param_1[0x36c] = 0;
  param_1[0x36d] = 0;
  *(undefined4 *)(param_1 + 0x369) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1b4c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x36a) = 0;
  param_1[0x370] = 0;
  param_1[0x371] = 0;
  param_1[0x372] = 0;
  *(undefined4 *)(param_1 + 0x36e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1b74) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x36f) = 0;
  param_1[0x375] = 0;
  param_1[0x376] = 0;
  param_1[0x377] = 0;
  *(undefined4 *)(param_1 + 0x373) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1b9c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x374) = 0;
  param_1[0x37a] = 0;
  param_1[0x37b] = 0;
  param_1[0x37c] = 0;
  *(undefined4 *)(param_1 + 0x378) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1bc4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x379) = 0;
  param_1[0x37f] = 0;
  param_1[0x380] = 0;
  param_1[0x381] = 0;
  *(undefined4 *)(param_1 + 0x37d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1bec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x37e) = 0;
  param_1[900] = 0;
  param_1[0x385] = 0;
  param_1[0x386] = 0;
  *(undefined4 *)(param_1 + 0x382) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1c14) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 899) = 0;
  param_1[0x389] = 0;
  param_1[0x38a] = 0;
  param_1[0x38b] = 0;
  *(undefined4 *)(param_1 + 0x387) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1c3c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x388) = 0;
  param_1[0x38e] = 0;
  param_1[0x38f] = 0;
  param_1[0x390] = 0;
  *(undefined4 *)(param_1 + 0x38c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1c64) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x38d) = 0;
  param_1[0x393] = 0;
  param_1[0x394] = 0;
  param_1[0x395] = 0;
  *(undefined4 *)(param_1 + 0x391) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1c8c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x392) = 0;
  param_1[0x398] = 0;
  param_1[0x399] = 0;
  param_1[0x39a] = 0;
  *(undefined4 *)(param_1 + 0x396) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1cb4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x397) = 0;
  param_1[0x39d] = 0;
  param_1[0x39e] = 0;
  param_1[0x39f] = 0;
  *(undefined4 *)(param_1 + 0x39b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1cdc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x39c) = 0;
  param_1[0x3a2] = 0;
  param_1[0x3a3] = 0;
  param_1[0x3a4] = 0;
  *(undefined4 *)(param_1 + 0x3a0) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1d04) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3a1) = 0;
  param_1[0x3a7] = 0;
  param_1[0x3a8] = 0;
  param_1[0x3a9] = 0;
  *(undefined4 *)(param_1 + 0x3a5) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1d2c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3a6) = 0;
  param_1[0x3ac] = 0;
  param_1[0x3ad] = 0;
  param_1[0x3ae] = 0;
  *(undefined4 *)(param_1 + 0x3aa) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1d54) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3ab) = 0;
  param_1[0x3b1] = 0;
  param_1[0x3b2] = 0;
  param_1[0x3b3] = 0;
  *(undefined4 *)(param_1 + 0x3af) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1d7c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3b0) = 0;
  param_1[0x3b6] = 0;
  param_1[0x3b7] = 0;
  param_1[0x3b8] = 0;
  *(undefined4 *)(param_1 + 0x3b4) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1da4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3b5) = 0;
  param_1[0x3bb] = 0;
  param_1[0x3bc] = 0;
  param_1[0x3bd] = 0;
  *(undefined4 *)(param_1 + 0x3b9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1dcc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3ba) = 0;
  param_1[0x3c0] = 0;
  param_1[0x3c1] = 0;
  param_1[0x3c2] = 0;
  *(undefined4 *)(param_1 + 0x3be) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1df4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3bf) = 0;
  param_1[0x3c5] = 0;
  param_1[0x3c6] = 0;
  param_1[0x3c7] = 0;
  *(undefined4 *)(param_1 + 0x3c3) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1e1c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3c4) = 0;
  param_1[0x3ca] = 0;
  param_1[0x3cb] = 0;
  param_1[0x3cc] = 0;
  *(undefined4 *)(param_1 + 0x3c8) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1e44) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3c9) = 0;
  param_1[0x3cf] = 0;
  param_1[0x3d0] = 0;
  param_1[0x3d1] = 0;
  *(undefined4 *)(param_1 + 0x3cd) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1e6c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3ce) = 0;
  param_1[0x3d4] = 0;
  param_1[0x3d5] = 0;
  param_1[0x3d6] = 0;
  *(undefined4 *)(param_1 + 0x3d2) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1e94) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3d3) = 0;
  param_1[0x3d9] = 0;
  param_1[0x3da] = 0;
  param_1[0x3db] = 0;
  *(undefined4 *)(param_1 + 0x3d7) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1ebc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3d8) = 0;
  param_1[0x3de] = 0;
  param_1[0x3df] = 0;
  param_1[0x3e0] = 0;
  *(undefined4 *)(param_1 + 0x3dc) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1ee4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3dd) = 0;
  param_1[0x3e3] = 0;
  param_1[0x3e4] = 0;
  param_1[0x3e5] = 0;
  *(undefined4 *)(param_1 + 0x3e1) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1f0c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3e2) = 0;
  param_1[1000] = 0;
  param_1[0x3e9] = 0;
  param_1[0x3ea] = 0;
  *(undefined4 *)(param_1 + 0x3e6) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1f34) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 999) = 0;
  param_1[0x3ed] = 0;
  param_1[0x3ee] = 0;
  param_1[0x3ef] = 0;
  *(undefined4 *)(param_1 + 0x3eb) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1f5c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3ec) = 0;
  param_1[0x3f2] = 0;
  param_1[0x3f3] = 0;
  param_1[0x3f4] = 0;
  *(undefined4 *)(param_1 + 0x3f0) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1f84) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3f1) = 0;
  param_1[0x3f7] = 0;
  param_1[0x3f8] = 0;
  param_1[0x3f9] = 0;
  *(undefined4 *)(param_1 + 0x3f5) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1fac) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3f6) = 0;
  param_1[0x3fc] = 0;
  param_1[0x3fd] = 0;
  param_1[0x3fe] = 0;
  *(undefined4 *)(param_1 + 0x3fa) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1fd4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x3fb) = 0;
  param_1[0x401] = 0;
  param_1[0x402] = 0;
  param_1[0x403] = 0;
  *(undefined4 *)(param_1 + 0x3ff) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x1ffc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x400) = 0;
  param_1[0x406] = 0;
  param_1[0x407] = 0;
  param_1[0x408] = 0;
  *(undefined4 *)(param_1 + 0x404) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2024) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x405) = 0;
  param_1[0x40b] = 0;
  param_1[0x40c] = 0;
  param_1[0x40d] = 0;
  *(undefined4 *)(param_1 + 0x409) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x204c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x40a) = 0;
  param_1[0x410] = 0;
  param_1[0x411] = 0;
  param_1[0x412] = 0;
  *(undefined4 *)(param_1 + 0x40e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2074) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x40f) = 0;
  param_1[0x415] = 0;
  param_1[0x416] = 0;
  param_1[0x417] = 0;
  *(undefined4 *)(param_1 + 0x413) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x209c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x414) = 0;
  param_1[0x41a] = 0;
  param_1[0x41b] = 0;
  param_1[0x41c] = 0;
  *(undefined4 *)(param_1 + 0x418) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x20c4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x419) = 0;
  param_1[0x41f] = 0;
  param_1[0x420] = 0;
  param_1[0x421] = 0;
  *(undefined4 *)(param_1 + 0x41d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x20ec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x41e) = 0;
  param_1[0x424] = 0;
  param_1[0x425] = 0;
  param_1[0x426] = 0;
  *(undefined4 *)(param_1 + 0x422) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2114) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x423) = 0;
  param_1[0x429] = 0;
  param_1[0x42a] = 0;
  param_1[0x42b] = 0;
  *(undefined4 *)(param_1 + 0x427) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x213c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x428) = 0;
  param_1[0x42e] = 0;
  param_1[0x42f] = 0;
  param_1[0x430] = 0;
  *(undefined4 *)(param_1 + 0x42c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2164) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x42d) = 0;
  param_1[0x433] = 0;
  param_1[0x434] = 0;
  param_1[0x435] = 0;
  *(undefined4 *)(param_1 + 0x431) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x218c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x432) = 0;
  param_1[0x438] = 0;
  param_1[0x439] = 0;
  param_1[0x43a] = 0;
  *(undefined4 *)(param_1 + 0x436) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x21b4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x437) = 0;
  param_1[0x43d] = 0;
  param_1[0x43e] = 0;
  param_1[0x43f] = 0;
  *(undefined4 *)(param_1 + 0x43b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x21dc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x43c) = 0;
  param_1[0x442] = 0;
  param_1[0x443] = 0;
  param_1[0x444] = 0;
  *(undefined4 *)(param_1 + 0x440) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2204) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x441) = 0;
  param_1[0x447] = 0;
  param_1[0x448] = 0;
  param_1[0x449] = 0;
  *(undefined4 *)(param_1 + 0x445) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x222c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x446) = 0;
  param_1[0x44c] = 0;
  param_1[0x44d] = 0;
  param_1[0x44e] = 0;
  *(undefined4 *)(param_1 + 0x44a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2254) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 1099) = 0;
  param_1[0x451] = 0;
  param_1[0x452] = 0;
  param_1[0x453] = 0;
  *(undefined4 *)(param_1 + 0x44f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x227c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x450) = 0;
  param_1[0x456] = 0;
  param_1[0x457] = 0;
  param_1[0x458] = 0;
  *(undefined4 *)(param_1 + 0x454) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x22a4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x455) = 0;
  param_1[0x45b] = 0;
  param_1[0x45c] = 0;
  param_1[0x45d] = 0;
  *(undefined4 *)(param_1 + 0x459) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x22cc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x45a) = 0;
  param_1[0x460] = 0;
  param_1[0x461] = 0;
  param_1[0x462] = 0;
  *(undefined4 *)(param_1 + 0x45e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x22f4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x45f) = 0;
  param_1[0x465] = 0;
  param_1[0x466] = 0;
  param_1[0x467] = 0;
  *(undefined4 *)(param_1 + 0x463) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x231c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x464) = 0;
  param_1[0x46a] = 0;
  param_1[0x46b] = 0;
  param_1[0x46c] = 0;
  *(undefined4 *)(param_1 + 0x468) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2344) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x469) = 0;
  param_1[0x46f] = 0;
  param_1[0x470] = 0;
  param_1[0x471] = 0;
  *(undefined4 *)(param_1 + 0x46d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x236c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x46e) = 0;
  param_1[0x474] = 0;
  param_1[0x475] = 0;
  param_1[0x476] = 0;
  *(undefined4 *)(param_1 + 0x472) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2394) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x473) = 0;
  param_1[0x479] = 0;
  param_1[0x47a] = 0;
  param_1[0x47b] = 0;
  *(undefined4 *)(param_1 + 0x477) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x23bc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x478) = 0;
  param_1[0x47e] = 0;
  param_1[0x47f] = 0;
  param_1[0x480] = 0;
  *(undefined4 *)(param_1 + 0x47c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x23e4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x47d) = 0;
  param_1[0x483] = 0;
  param_1[0x484] = 0;
  param_1[0x485] = 0;
  *(undefined4 *)(param_1 + 0x481) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x240c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x482) = 0;
  param_1[0x488] = 0;
  param_1[0x489] = 0;
  param_1[0x48a] = 0;
  *(undefined4 *)(param_1 + 0x486) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2434) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x487) = 0;
  param_1[0x48d] = 0;
  param_1[0x48e] = 0;
  param_1[0x48f] = 0;
  *(undefined4 *)(param_1 + 0x48b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x245c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x48c) = 0;
  param_1[0x492] = 0;
  param_1[0x493] = 0;
  param_1[0x494] = 0;
  *(undefined4 *)(param_1 + 0x490) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2484) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x491) = 0;
  param_1[0x497] = 0;
  param_1[0x498] = 0;
  param_1[0x499] = 0;
  *(undefined4 *)(param_1 + 0x495) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x24ac) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x496) = 0;
  param_1[0x49c] = 0;
  param_1[0x49d] = 0;
  param_1[0x49e] = 0;
  *(undefined4 *)(param_1 + 0x49a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x24d4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x49b) = 0;
  param_1[0x4a1] = 0;
  param_1[0x4a2] = 0;
  param_1[0x4a3] = 0;
  *(undefined4 *)(param_1 + 0x49f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x24fc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4a0) = 0;
  param_1[0x4a6] = 0;
  param_1[0x4a7] = 0;
  param_1[0x4a8] = 0;
  *(undefined4 *)(param_1 + 0x4a4) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2524) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4a5) = 0;
  param_1[0x4ab] = 0;
  param_1[0x4ac] = 0;
  param_1[0x4ad] = 0;
  *(undefined4 *)(param_1 + 0x4a9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x254c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4aa) = 0;
  param_1[0x4b0] = 0;
  param_1[0x4b1] = 0;
  param_1[0x4b2] = 0;
  *(undefined4 *)(param_1 + 0x4ae) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2574) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4af) = 0;
  param_1[0x4b5] = 0;
  param_1[0x4b6] = 0;
  param_1[0x4b7] = 0;
  *(undefined4 *)(param_1 + 0x4b3) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x259c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4b4) = 0;
  param_1[0x4ba] = 0;
  param_1[0x4bb] = 0;
  param_1[0x4bc] = 0;
  *(undefined4 *)(param_1 + 0x4b8) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x25c4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4b9) = 0;
  param_1[0x4bf] = 0;
  param_1[0x4c0] = 0;
  param_1[0x4c1] = 0;
  *(undefined4 *)(param_1 + 0x4bd) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x25ec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4be) = 0;
  param_1[0x4c4] = 0;
  param_1[0x4c5] = 0;
  param_1[0x4c6] = 0;
  *(undefined4 *)(param_1 + 0x4c2) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2614) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4c3) = 0;
  param_1[0x4c9] = 0;
  param_1[0x4ca] = 0;
  param_1[0x4cb] = 0;
  *(undefined4 *)(param_1 + 0x4c7) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x263c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4c8) = 0;
  param_1[0x4ce] = 0;
  param_1[0x4cf] = 0;
  param_1[0x4d0] = 0;
  *(undefined4 *)(param_1 + 0x4cc) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2664) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4cd) = 0;
  param_1[0x4d3] = 0;
  param_1[0x4d4] = 0;
  param_1[0x4d5] = 0;
  *(undefined4 *)(param_1 + 0x4d1) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x268c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4d2) = 0;
  param_1[0x4d8] = 0;
  param_1[0x4d9] = 0;
  param_1[0x4da] = 0;
  *(undefined4 *)(param_1 + 0x4d6) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x26b4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4d7) = 0;
  param_1[0x4dd] = 0;
  param_1[0x4de] = 0;
  param_1[0x4df] = 0;
  *(undefined4 *)(param_1 + 0x4db) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x26dc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4dc) = 0;
  param_1[0x4e2] = 0;
  param_1[0x4e3] = 0;
  param_1[0x4e4] = 0;
  *(undefined4 *)(param_1 + 0x4e0) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2704) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4e1) = 0;
  param_1[0x4e7] = 0;
  param_1[0x4e8] = 0;
  param_1[0x4e9] = 0;
  *(undefined4 *)(param_1 + 0x4e5) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x272c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4e6) = 0;
  param_1[0x4ec] = 0;
  param_1[0x4ed] = 0;
  param_1[0x4ee] = 0;
  *(undefined4 *)(param_1 + 0x4ea) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2754) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4eb) = 0;
  param_1[0x4f1] = 0;
  param_1[0x4f2] = 0;
  param_1[0x4f3] = 0;
  *(undefined4 *)(param_1 + 0x4ef) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x277c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4f0) = 0;
  param_1[0x4f6] = 0;
  param_1[0x4f7] = 0;
  param_1[0x4f8] = 0;
  *(undefined4 *)(param_1 + 0x4f4) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x27a4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4f5) = 0;
  param_1[0x4fb] = 0;
  param_1[0x4fc] = 0;
  param_1[0x4fd] = 0;
  *(undefined4 *)(param_1 + 0x4f9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x27cc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4fa) = 0;
  param_1[0x500] = 0;
  param_1[0x501] = 0;
  param_1[0x502] = 0;
  *(undefined4 *)(param_1 + 0x4fe) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x27f4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x4ff) = 0;
  param_1[0x505] = 0;
  param_1[0x506] = 0;
  param_1[0x507] = 0;
  *(undefined4 *)(param_1 + 0x503) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x281c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x504) = 0;
  param_1[0x50a] = 0;
  param_1[0x50b] = 0;
  param_1[0x50c] = 0;
  *(undefined4 *)(param_1 + 0x508) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2844) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x509) = 0;
  param_1[0x50f] = 0;
  param_1[0x510] = 0;
  param_1[0x511] = 0;
  *(undefined4 *)(param_1 + 0x50d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x286c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x50e) = 0;
  param_1[0x514] = 0;
  param_1[0x515] = 0;
  param_1[0x516] = 0;
  *(undefined4 *)(param_1 + 0x512) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2894) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x513) = 0;
  param_1[0x519] = 0;
  param_1[0x51a] = 0;
  param_1[0x51b] = 0;
  *(undefined4 *)(param_1 + 0x517) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x28bc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x518) = 0;
  param_1[0x51e] = 0;
  param_1[0x51f] = 0;
  param_1[0x520] = 0;
  *(undefined4 *)(param_1 + 0x51c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x28e4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x51d) = 0;
  param_1[0x523] = 0;
  param_1[0x524] = 0;
  param_1[0x525] = 0;
  *(undefined4 *)(param_1 + 0x521) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x290c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x522) = 0;
  param_1[0x528] = 0;
  param_1[0x529] = 0;
  param_1[0x52a] = 0;
  *(undefined4 *)(param_1 + 0x526) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2934) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x527) = 0;
  param_1[0x52d] = 0;
  param_1[0x52e] = 0;
  param_1[0x52f] = 0;
  *(undefined4 *)(param_1 + 0x52b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x295c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x52c) = 0;
  param_1[0x532] = 0;
  param_1[0x533] = 0;
  param_1[0x534] = 0;
  *(undefined4 *)(param_1 + 0x530) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2984) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x531) = 0;
  param_1[0x537] = 0;
  param_1[0x538] = 0;
  param_1[0x539] = 0;
  *(undefined4 *)(param_1 + 0x535) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x29ac) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x536) = 0;
  param_1[0x53c] = 0;
  param_1[0x53d] = 0;
  param_1[0x53e] = 0;
  *(undefined4 *)(param_1 + 0x53a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x29d4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x53b) = 0;
  param_1[0x541] = 0;
  param_1[0x542] = 0;
  param_1[0x543] = 0;
  *(undefined4 *)(param_1 + 0x53f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x29fc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x540) = 0;
  param_1[0x546] = 0;
  param_1[0x547] = 0;
  param_1[0x548] = 0;
  *(undefined4 *)(param_1 + 0x544) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2a24) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x545) = 0;
  param_1[0x54b] = 0;
  param_1[0x54c] = 0;
  param_1[0x54d] = 0;
  *(undefined4 *)(param_1 + 0x549) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2a4c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x54a) = 0;
  param_1[0x550] = 0;
  param_1[0x551] = 0;
  param_1[0x552] = 0;
  *(undefined4 *)(param_1 + 0x54e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2a74) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x54f) = 0;
  param_1[0x555] = 0;
  param_1[0x556] = 0;
  param_1[0x557] = 0;
  *(undefined4 *)(param_1 + 0x553) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2a9c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x554) = 0;
  param_1[0x55a] = 0;
  param_1[0x55b] = 0;
  param_1[0x55c] = 0;
  *(undefined4 *)(param_1 + 0x558) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2ac4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x559) = 0;
  param_1[0x55f] = 0;
  param_1[0x560] = 0;
  param_1[0x561] = 0;
  *(undefined4 *)(param_1 + 0x55d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2aec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x55e) = 0;
  param_1[0x564] = 0;
  param_1[0x565] = 0;
  param_1[0x566] = 0;
  *(undefined4 *)(param_1 + 0x562) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2b14) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x563) = 0;
  param_1[0x569] = 0;
  param_1[0x56a] = 0;
  param_1[0x56b] = 0;
  *(undefined4 *)(param_1 + 0x567) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2b3c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x568) = 0;
  param_1[0x56e] = 0;
  param_1[0x56f] = 0;
  param_1[0x570] = 0;
  *(undefined4 *)(param_1 + 0x56c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2b64) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x56d) = 0;
  param_1[0x573] = 0;
  param_1[0x574] = 0;
  param_1[0x575] = 0;
  *(undefined4 *)(param_1 + 0x571) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2b8c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x572) = 0;
  param_1[0x578] = 0;
  param_1[0x579] = 0;
  param_1[0x57a] = 0;
  *(undefined4 *)(param_1 + 0x576) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2bb4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x577) = 0;
  param_1[0x57d] = 0;
  param_1[0x57e] = 0;
  param_1[0x57f] = 0;
  *(undefined4 *)(param_1 + 0x57b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2bdc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x57c) = 0;
  param_1[0x582] = 0;
  param_1[0x583] = 0;
  param_1[0x584] = 0;
  *(undefined4 *)(param_1 + 0x580) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2c04) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x581) = 0;
  param_1[0x587] = 0;
  param_1[0x588] = 0;
  param_1[0x589] = 0;
  *(undefined4 *)(param_1 + 0x585) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2c2c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x586) = 0;
  param_1[0x58c] = 0;
  param_1[0x58d] = 0;
  param_1[0x58e] = 0;
  *(undefined4 *)(param_1 + 0x58a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2c54) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x58b) = 0;
  param_1[0x591] = 0;
  param_1[0x592] = 0;
  param_1[0x593] = 0;
  *(undefined4 *)(param_1 + 0x58f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2c7c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x590) = 0;
  param_1[0x596] = 0;
  param_1[0x597] = 0;
  param_1[0x598] = 0;
  *(undefined4 *)(param_1 + 0x594) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2ca4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x595) = 0;
  param_1[0x59b] = 0;
  param_1[0x59c] = 0;
  param_1[0x59d] = 0;
  *(undefined4 *)(param_1 + 0x599) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2ccc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x59a) = 0;
  param_1[0x5a0] = 0;
  param_1[0x5a1] = 0;
  param_1[0x5a2] = 0;
  *(undefined4 *)(param_1 + 0x59e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2cf4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x59f) = 0;
  param_1[0x5a5] = 0;
  param_1[0x5a6] = 0;
  param_1[0x5a7] = 0;
  *(undefined4 *)(param_1 + 0x5a3) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2d1c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5a4) = 0;
  param_1[0x5aa] = 0;
  param_1[0x5ab] = 0;
  param_1[0x5ac] = 0;
  *(undefined4 *)(param_1 + 0x5a8) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2d44) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5a9) = 0;
  param_1[0x5af] = 0;
  param_1[0x5b0] = 0;
  param_1[0x5b1] = 0;
  *(undefined4 *)(param_1 + 0x5ad) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2d6c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5ae) = 0;
  param_1[0x5b4] = 0;
  param_1[0x5b5] = 0;
  param_1[0x5b6] = 0;
  *(undefined4 *)(param_1 + 0x5b2) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2d94) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5b3) = 0;
  param_1[0x5b9] = 0;
  param_1[0x5ba] = 0;
  param_1[0x5bb] = 0;
  *(undefined4 *)(param_1 + 0x5b7) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2dbc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5b8) = 0;
  param_1[0x5be] = 0;
  param_1[0x5bf] = 0;
  param_1[0x5c0] = 0;
  *(undefined4 *)(param_1 + 0x5bc) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2de4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5bd) = 0;
  param_1[0x5c3] = 0;
  param_1[0x5c4] = 0;
  param_1[0x5c5] = 0;
  *(undefined4 *)(param_1 + 0x5c1) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2e0c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5c2) = 0;
  param_1[0x5c8] = 0;
  param_1[0x5c9] = 0;
  param_1[0x5ca] = 0;
  *(undefined4 *)(param_1 + 0x5c6) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2e34) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5c7) = 0;
  param_1[0x5cd] = 0;
  param_1[0x5ce] = 0;
  param_1[0x5cf] = 0;
  *(undefined4 *)(param_1 + 0x5cb) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2e5c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5cc) = 0;
  param_1[0x5d2] = 0;
  param_1[0x5d3] = 0;
  param_1[0x5d4] = 0;
  *(undefined4 *)(param_1 + 0x5d0) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2e84) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5d1) = 0;
  param_1[0x5d7] = 0;
  param_1[0x5d8] = 0;
  param_1[0x5d9] = 0;
  *(undefined4 *)(param_1 + 0x5d5) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2eac) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5d6) = 0;
  param_1[0x5dc] = 0;
  param_1[0x5dd] = 0;
  param_1[0x5de] = 0;
  *(undefined4 *)(param_1 + 0x5da) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2ed4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5db) = 0;
  param_1[0x5e1] = 0;
  param_1[0x5e2] = 0;
  param_1[0x5e3] = 0;
  *(undefined4 *)(param_1 + 0x5df) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2efc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5e0) = 0;
  param_1[0x5e6] = 0;
  param_1[0x5e7] = 0;
  param_1[0x5e8] = 0;
  *(undefined4 *)(param_1 + 0x5e4) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2f24) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5e5) = 0;
  param_1[0x5eb] = 0;
  param_1[0x5ec] = 0;
  param_1[0x5ed] = 0;
  *(undefined4 *)(param_1 + 0x5e9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2f4c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5ea) = 0;
  param_1[0x5f0] = 0;
  param_1[0x5f1] = 0;
  param_1[0x5f2] = 0;
  *(undefined4 *)(param_1 + 0x5ee) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2f74) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5ef) = 0;
  param_1[0x5f5] = 0;
  param_1[0x5f6] = 0;
  param_1[0x5f7] = 0;
  *(undefined4 *)(param_1 + 0x5f3) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2f9c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5f4) = 0;
  param_1[0x5fa] = 0;
  param_1[0x5fb] = 0;
  param_1[0x5fc] = 0;
  *(undefined4 *)(param_1 + 0x5f8) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2fc4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5f9) = 0;
  param_1[0x5ff] = 0;
  param_1[0x600] = 0;
  param_1[0x601] = 0;
  *(undefined4 *)(param_1 + 0x5fd) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2fec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x5fe) = 0;
  param_1[0x602] = 0;
  param_1[0x603] = 0;
  FUN_1407e5330(param_1 + 0x604,0,0);
  *(undefined4 *)(param_1 + 0x608) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3044) = 0;
  param_1[0x60a] = 0;
  *(undefined4 *)(param_1 + 0x60b) = 0;
  param_1[0x60e] = 0;
  param_1[0x60f] = 0;
  param_1[0x610] = 0;
  *(undefined4 *)(param_1 + 0x60c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3064) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x60d) = 0;
  param_1[0x613] = 0;
  param_1[0x614] = 0;
  param_1[0x615] = 0;
  *(undefined4 *)(param_1 + 0x611) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x308c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x612) = 0;
  param_1[0x618] = 0;
  param_1[0x619] = 0;
  param_1[0x61a] = 0;
  *(undefined4 *)(param_1 + 0x616) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x30b4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x617) = 0;
  param_1[0x61d] = 0;
  param_1[0x61e] = 0;
  param_1[0x61f] = 0;
  *(undefined4 *)(param_1 + 0x61b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x30dc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x61c) = 0;
  param_1[0x622] = 0;
  param_1[0x623] = 0;
  param_1[0x624] = 0;
  *(undefined4 *)(param_1 + 0x620) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3104) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x621) = 0;
  param_1[0x627] = 0;
  param_1[0x628] = 0;
  param_1[0x629] = 0;
  *(undefined4 *)(param_1 + 0x625) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x312c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x626) = 0;
  param_1[0x62c] = 0;
  param_1[0x62d] = 0;
  param_1[0x62e] = 0;
  *(undefined4 *)(param_1 + 0x62a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3154) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x62b) = 0;
  param_1[0x631] = 0;
  param_1[0x632] = 0;
  param_1[0x633] = 0;
  *(undefined4 *)(param_1 + 0x62f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x317c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x630) = 0;
  param_1[0x636] = 0;
  param_1[0x637] = 0;
  param_1[0x638] = 0;
  *(undefined4 *)(param_1 + 0x634) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x31a4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x635) = 0;
  param_1[0x63b] = 0;
  param_1[0x63c] = 0;
  param_1[0x63d] = 0;
  *(undefined4 *)(param_1 + 0x639) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x31cc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x63a) = 0;
  param_1[0x640] = 0;
  param_1[0x641] = 0;
  param_1[0x642] = 0;
  *(undefined4 *)(param_1 + 0x63e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x31f4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x63f) = 0;
  param_1[0x645] = 0;
  param_1[0x646] = 0;
  param_1[0x647] = 0;
  *(undefined4 *)(param_1 + 0x643) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x321c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x644) = 0;
  param_1[0x64a] = 0;
  param_1[0x64b] = 0;
  param_1[0x64c] = 0;
  *(undefined4 *)(param_1 + 0x648) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3244) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x649) = 0;
  param_1[0x64f] = 0;
  param_1[0x650] = 0;
  param_1[0x651] = 0;
  *(undefined4 *)(param_1 + 0x64d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x326c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x64e) = 0;
  param_1[0x654] = 0;
  param_1[0x655] = 0;
  param_1[0x656] = 0;
  *(undefined4 *)(param_1 + 0x652) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3294) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x653) = 0;
  param_1[0x659] = 0;
  param_1[0x65a] = 0;
  param_1[0x65b] = 0;
  *(undefined4 *)(param_1 + 0x657) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x32bc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x658) = 0;
  param_1[0x65e] = 0;
  param_1[0x65f] = 0;
  param_1[0x660] = 0;
  *(undefined4 *)(param_1 + 0x65c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x32e4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x65d) = 0;
  *(undefined1 *)(param_1 + 0x661) = 0;
  *(undefined8 *)((longlong)param_1 + 0x330c) = 0;
  *(undefined4 *)((longlong)param_1 + 0x3314) = 0x3e4ccccd;
  param_1[0x665] = 0;
  param_1[0x666] = 0;
  param_1[0x667] = 0;
  *(undefined4 *)(param_1 + 0x663) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x331c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x664) = 0;
  param_1[0x66a] = 0;
  param_1[0x66b] = 0;
  param_1[0x66c] = 0;
  *(undefined4 *)(param_1 + 0x668) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3344) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x669) = 0;
  param_1[0x66f] = 0;
  param_1[0x670] = 0;
  param_1[0x671] = 0;
  *(undefined4 *)(param_1 + 0x66d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x336c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x66e) = 0;
  param_1[0x674] = 0;
  param_1[0x675] = 0;
  param_1[0x676] = 0;
  *(undefined4 *)(param_1 + 0x672) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3394) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x673) = 0;
  param_1[0x679] = 0;
  param_1[0x67a] = 0;
  param_1[0x67b] = 0;
  *(undefined4 *)(param_1 + 0x677) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x33bc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x678) = 0;
  param_1[0x67e] = 0;
  param_1[0x67f] = 0;
  param_1[0x680] = 0;
  *(undefined4 *)(param_1 + 0x67c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x33e4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x67d) = 0;
  param_1[0x683] = 0;
  param_1[0x684] = 0;
  param_1[0x685] = 0;
  *(undefined4 *)(param_1 + 0x681) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x340c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x682) = 0;
  param_1[0x688] = 0;
  param_1[0x689] = 0;
  param_1[0x68a] = 0;
  *(undefined4 *)(param_1 + 0x686) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3434) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x687) = 0;
  param_1[0x68d] = 0;
  param_1[0x68e] = 0;
  param_1[0x68f] = 0;
  *(undefined4 *)(param_1 + 0x68b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x345c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x68c) = 0;
  param_1[0x692] = 0;
  param_1[0x693] = 0;
  param_1[0x694] = 0;
  *(undefined4 *)(param_1 + 0x690) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3484) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x691) = 0;
  param_1[0x697] = 0;
  param_1[0x698] = 0;
  param_1[0x699] = 0;
  *(undefined4 *)(param_1 + 0x695) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x34ac) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x696) = 0;
  param_1[0x69c] = 0;
  param_1[0x69d] = 0;
  param_1[0x69e] = 0;
  *(undefined4 *)(param_1 + 0x69a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x34d4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x69b) = 0;
  param_1[0x6a1] = 0;
  param_1[0x6a2] = 0;
  param_1[0x6a3] = 0;
  *(undefined4 *)(param_1 + 0x69f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x34fc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6a0) = 0;
  param_1[0x6a6] = 0;
  param_1[0x6a7] = 0;
  param_1[0x6a8] = 0;
  *(undefined4 *)(param_1 + 0x6a4) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3524) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6a5) = 0;
  param_1[0x6ab] = 0;
  param_1[0x6ac] = 0;
  param_1[0x6ad] = 0;
  *(undefined4 *)(param_1 + 0x6a9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x354c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6aa) = 0;
  param_1[0x6b0] = 0;
  param_1[0x6b1] = 0;
  param_1[0x6b2] = 0;
  *(undefined4 *)(param_1 + 0x6ae) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3574) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6af) = 0;
  param_1[0x6b5] = 0;
  param_1[0x6b6] = 0;
  param_1[0x6b7] = 0;
  *(undefined4 *)(param_1 + 0x6b3) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x359c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6b4) = 0;
  param_1[0x6ba] = 0;
  param_1[0x6bb] = 0;
  param_1[0x6bc] = 0;
  *(undefined4 *)(param_1 + 0x6b8) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x35c4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6b9) = 0;
  param_1[0x6bf] = 0;
  param_1[0x6c0] = 0;
  param_1[0x6c1] = 0;
  *(undefined4 *)(param_1 + 0x6bd) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x35ec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6be) = 0;
  param_1[0x6c4] = 0;
  param_1[0x6c5] = 0;
  param_1[0x6c6] = 0;
  *(undefined4 *)(param_1 + 0x6c2) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3614) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6c3) = 0;
  param_1[0x6c9] = 0;
  param_1[0x6ca] = 0;
  param_1[0x6cb] = 0;
  *(undefined4 *)(param_1 + 0x6c7) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x363c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6c8) = 0;
  param_1[0x6ce] = 0;
  param_1[0x6cf] = 0;
  param_1[0x6d0] = 0;
  *(undefined4 *)(param_1 + 0x6cc) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3664) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6cd) = 0;
  param_1[0x6d3] = 0;
  param_1[0x6d4] = 0;
  param_1[0x6d5] = 0;
  *(undefined4 *)(param_1 + 0x6d1) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x368c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6d2) = 0;
  param_1[0x6d8] = 0;
  param_1[0x6d9] = 0;
  param_1[0x6da] = 0;
  *(undefined4 *)(param_1 + 0x6d6) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x36b4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6d7) = 0;
  param_1[0x6dd] = 0;
  param_1[0x6de] = 0;
  param_1[0x6df] = 0;
  *(undefined4 *)(param_1 + 0x6db) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x36dc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6dc) = 0;
  param_1[0x6e2] = 0;
  param_1[0x6e3] = 0;
  param_1[0x6e4] = 0;
  *(undefined4 *)(param_1 + 0x6e0) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3704) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6e1) = 0;
  param_1[0x6e7] = 0;
  param_1[0x6e8] = 0;
  param_1[0x6e9] = 0;
  *(undefined4 *)(param_1 + 0x6e5) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x372c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6e6) = 0;
  param_1[0x6ec] = 0;
  param_1[0x6ed] = 0;
  param_1[0x6ee] = 0;
  *(undefined4 *)(param_1 + 0x6ea) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3754) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6eb) = 0;
  param_1[0x6f1] = 0;
  param_1[0x6f2] = 0;
  param_1[0x6f3] = 0;
  *(undefined4 *)(param_1 + 0x6ef) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x377c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6f0) = 0;
  param_1[0x6f6] = 0;
  param_1[0x6f7] = 0;
  param_1[0x6f8] = 0;
  *(undefined4 *)(param_1 + 0x6f4) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x37a4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6f5) = 0;
  param_1[0x6fb] = 0;
  param_1[0x6fc] = 0;
  param_1[0x6fd] = 0;
  *(undefined4 *)(param_1 + 0x6f9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x37cc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6fa) = 0;
  param_1[0x700] = 0;
  param_1[0x701] = 0;
  param_1[0x702] = 0;
  *(undefined4 *)(param_1 + 0x6fe) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x37f4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x6ff) = 0;
  param_1[0x705] = 0;
  param_1[0x706] = 0;
  param_1[0x707] = 0;
  *(undefined4 *)(param_1 + 0x703) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x381c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x704) = 0;
  param_1[0x70a] = 0;
  param_1[0x70b] = 0;
  param_1[0x70c] = 0;
  *(undefined4 *)(param_1 + 0x708) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3844) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x709) = 0;
  param_1[0x70f] = 0;
  param_1[0x710] = 0;
  param_1[0x711] = 0;
  *(undefined4 *)(param_1 + 0x70d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x386c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x70e) = 0;
  param_1[0x714] = 0;
  param_1[0x715] = 0;
  param_1[0x716] = 0;
  *(undefined4 *)(param_1 + 0x712) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3894) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x713) = 0;
  param_1[0x719] = 0;
  param_1[0x71a] = 0;
  param_1[0x71b] = 0;
  *(undefined4 *)(param_1 + 0x717) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x38bc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x718) = 0;
  param_1[0x71f] = 0;
  param_1[0x720] = 0;
  param_1[0x721] = 0;
  *(undefined4 *)(param_1 + 0x71d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x38ec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x71e) = 0;
  param_1[0x724] = 0;
  param_1[0x725] = 0;
  param_1[0x726] = 0;
  *(undefined4 *)(param_1 + 0x722) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3914) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x723) = 0;
  param_1[0x729] = 0;
  param_1[0x72a] = 0;
  param_1[0x72b] = 0;
  *(undefined4 *)(param_1 + 0x727) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x393c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x728) = 0;
  param_1[0x72e] = 0;
  param_1[0x72f] = 0;
  param_1[0x730] = 0;
  *(undefined4 *)(param_1 + 0x72c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3964) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x72d) = 0;
  param_1[0x733] = 0;
  param_1[0x734] = 0;
  param_1[0x735] = 0;
  *(undefined4 *)(param_1 + 0x731) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x398c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x732) = 0;
  param_1[0x738] = 0;
  param_1[0x739] = 0;
  param_1[0x73a] = 0;
  *(undefined4 *)(param_1 + 0x736) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x39b4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x737) = 0;
  param_1[0x73d] = 0;
  param_1[0x73e] = 0;
  param_1[0x73f] = 0;
  *(undefined4 *)(param_1 + 0x73b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x39dc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x73c) = 0;
  param_1[0x742] = 0;
  param_1[0x743] = 0;
  param_1[0x744] = 0;
  *(undefined4 *)(param_1 + 0x740) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3a04) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x741) = 0;
  param_1[0x747] = 0;
  param_1[0x748] = 0;
  param_1[0x749] = 0;
  *(undefined4 *)(param_1 + 0x745) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3a2c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x746) = 0;
  param_1[0x74c] = 0;
  param_1[0x74d] = 0;
  param_1[0x74e] = 0;
  *(undefined4 *)(param_1 + 0x74a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3a54) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x74b) = 0;
  param_1[0x751] = 0;
  param_1[0x752] = 0;
  param_1[0x753] = 0;
  *(undefined4 *)(param_1 + 0x74f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3a7c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x750) = 0;
  param_1[0x756] = 0;
  param_1[0x757] = 0;
  param_1[0x758] = 0;
  *(undefined4 *)(param_1 + 0x754) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3aa4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x755) = 0;
  param_1[0x75b] = 0;
  param_1[0x75c] = 0;
  param_1[0x75d] = 0;
  *(undefined4 *)(param_1 + 0x759) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3acc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x75a) = 0;
  param_1[0x760] = 0;
  param_1[0x761] = 0;
  param_1[0x762] = 0;
  *(undefined4 *)(param_1 + 0x75e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3af4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x75f) = 0;
  param_1[0x765] = 0;
  param_1[0x766] = 0;
  param_1[0x767] = 0;
  *(undefined4 *)(param_1 + 0x763) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3b1c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x764) = 0;
  param_1[0x76a] = 0;
  param_1[0x76b] = 0;
  param_1[0x76c] = 0;
  *(undefined4 *)(param_1 + 0x768) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3b44) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x769) = 0;
  param_1[0x76f] = 0;
  param_1[0x770] = 0;
  param_1[0x771] = 0;
  *(undefined4 *)(param_1 + 0x76d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3b6c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x76e) = 0;
  param_1[0x774] = 0;
  param_1[0x775] = 0;
  param_1[0x776] = 0;
  *(undefined4 *)(param_1 + 0x772) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3b94) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x773) = 0;
  param_1[0x779] = 0;
  param_1[0x77a] = 0;
  param_1[0x77b] = 0;
  *(undefined4 *)(param_1 + 0x777) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3bbc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x778) = 0;
  param_1[0x77e] = 0;
  param_1[0x77f] = 0;
  param_1[0x780] = 0;
  *(undefined4 *)(param_1 + 0x77c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3be4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x77d) = 0;
  param_1[0x783] = 0;
  param_1[0x784] = 0;
  param_1[0x785] = 0;
  *(undefined4 *)(param_1 + 0x781) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3c0c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x782) = 0;
  param_1[0x788] = 0;
  param_1[0x789] = 0;
  param_1[0x78a] = 0;
  *(undefined4 *)(param_1 + 0x786) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3c34) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x787) = 0;
  param_1[0x78d] = 0;
  param_1[0x78e] = 0;
  param_1[0x78f] = 0;
  *(undefined4 *)(param_1 + 0x78b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3c5c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x78c) = 0;
  param_1[0x792] = 0;
  param_1[0x793] = 0;
  param_1[0x794] = 0;
  *(undefined4 *)(param_1 + 0x790) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3c84) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x791) = 0;
  param_1[0x797] = 0;
  param_1[0x798] = 0;
  param_1[0x799] = 0;
  *(undefined4 *)(param_1 + 0x795) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3cac) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x796) = 0;
  param_1[0x79c] = 0;
  param_1[0x79d] = 0;
  param_1[0x79e] = 0;
  *(undefined4 *)(param_1 + 0x79a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3cd4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x79b) = 0;
  param_1[0x7a1] = 0;
  param_1[0x7a2] = 0;
  param_1[0x7a3] = 0;
  *(undefined4 *)(param_1 + 0x79f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3cfc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7a0) = 0;
  param_1[0x7a6] = 0;
  param_1[0x7a7] = 0;
  param_1[0x7a8] = 0;
  *(undefined4 *)(param_1 + 0x7a4) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3d24) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7a5) = 0;
  param_1[0x7ab] = 0;
  param_1[0x7ac] = 0;
  param_1[0x7ad] = 0;
  *(undefined4 *)(param_1 + 0x7a9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3d4c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7aa) = 0;
  param_1[0x7b0] = 0;
  param_1[0x7b1] = 0;
  param_1[0x7b2] = 0;
  *(undefined4 *)(param_1 + 0x7ae) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3d74) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7af) = 0;
  param_1[0x7b5] = 0;
  param_1[0x7b6] = 0;
  param_1[0x7b7] = 0;
  *(undefined4 *)(param_1 + 0x7b3) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3d9c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7b4) = 0;
  param_1[0x7ba] = 0;
  param_1[0x7bb] = 0;
  param_1[0x7bc] = 0;
  *(undefined4 *)(param_1 + 0x7b8) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3dc4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7b9) = 0;
  param_1[0x7bf] = 0;
  param_1[0x7c0] = 0;
  param_1[0x7c1] = 0;
  *(undefined4 *)(param_1 + 0x7bd) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3dec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7be) = 0;
  param_1[0x7c4] = 0;
  param_1[0x7c5] = 0;
  param_1[0x7c6] = 0;
  *(undefined4 *)(param_1 + 0x7c2) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3e14) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7c3) = 0;
  param_1[0x7c9] = 0;
  param_1[0x7ca] = 0;
  param_1[0x7cb] = 0;
  *(undefined4 *)(param_1 + 0x7c7) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3e3c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7c8) = 0;
  param_1[0x7ce] = 0;
  param_1[1999] = 0;
  param_1[2000] = 0;
  *(undefined4 *)(param_1 + 0x7cc) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3e64) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7cd) = 0;
  param_1[0x7d3] = 0;
  param_1[0x7d4] = 0;
  param_1[0x7d5] = 0;
  *(undefined4 *)(param_1 + 0x7d1) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3e8c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7d2) = 0;
  param_1[0x7d8] = 0;
  param_1[0x7d9] = 0;
  param_1[0x7da] = 0;
  *(undefined4 *)(param_1 + 0x7d6) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3eb4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7d7) = 0;
  param_1[0x7dd] = 0;
  param_1[0x7de] = 0;
  param_1[0x7df] = 0;
  *(undefined4 *)(param_1 + 0x7db) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3edc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7dc) = 0;
  param_1[0x7e2] = 0;
  param_1[0x7e3] = 0;
  param_1[0x7e4] = 0;
  *(undefined4 *)(param_1 + 0x7e0) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3f04) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7e1) = 0;
  param_1[0x7e7] = 0;
  param_1[0x7e8] = 0;
  param_1[0x7e9] = 0;
  *(undefined4 *)(param_1 + 0x7e5) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3f2c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7e6) = 0;
  param_1[0x7ec] = 0;
  param_1[0x7ed] = 0;
  param_1[0x7ee] = 0;
  *(undefined4 *)(param_1 + 0x7ea) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3f54) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7eb) = 0;
  param_1[0x7ef] = 0;
  param_1[0x7f0] = 0;
  param_1[0x7f1] = 0;
  param_1[0x7f2] = 0;
  param_1[0x7f5] = 0;
  param_1[0x7f6] = 0;
  param_1[0x7f7] = 0;
  *(undefined4 *)(param_1 + 0x7f3) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3f9c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7f4) = 0;
  param_1[0x7fa] = 0;
  param_1[0x7fb] = 0;
  param_1[0x7fc] = 0;
  *(undefined4 *)(param_1 + 0x7f8) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3fc4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7f9) = 0;
  param_1[0x7ff] = 0;
  param_1[0x800] = 0;
  param_1[0x801] = 0;
  *(undefined4 *)(param_1 + 0x7fd) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x3fec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x7fe) = 0;
  param_1[0x804] = 0;
  param_1[0x805] = 0;
  param_1[0x806] = 0;
  *(undefined4 *)(param_1 + 0x802) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x4014) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x803) = 0;
  param_1[0x807] = 0;
  param_1[0x808] = 0;
  param_1[0x80b] = 0;
  param_1[0x80c] = 0;
  param_1[0x80d] = 0;
  *(undefined4 *)(param_1 + 0x809) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x404c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x80a) = 0;
  param_1[0x810] = 0;
  param_1[0x811] = 0;
  param_1[0x812] = 0;
  *(undefined4 *)(param_1 + 0x80e) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x4074) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x80f) = 0;
  param_1[0x815] = 0;
  param_1[0x816] = 0;
  param_1[0x817] = 0;
  *(undefined4 *)(param_1 + 0x813) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x409c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x814) = 0;
  param_1[0x81a] = 0;
  param_1[0x81b] = 0;
  param_1[0x81c] = 0;
  *(undefined4 *)(param_1 + 0x818) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x40c4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x819) = 0;
  param_1[0x81f] = 0;
  param_1[0x820] = 0;
  param_1[0x821] = 0;
  *(undefined4 *)(param_1 + 0x81d) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x40ec) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x81e) = 0;
  param_1[0x824] = 0;
  param_1[0x825] = 0;
  param_1[0x826] = 0;
  *(undefined4 *)(param_1 + 0x822) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x4114) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x823) = 0;
  param_1[0x829] = 0;
  param_1[0x82a] = 0;
  param_1[0x82b] = 0;
  *(undefined4 *)(param_1 + 0x827) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x413c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x828) = 0;
  param_1[0x82e] = 0;
  param_1[0x82f] = 0;
  param_1[0x830] = 0;
  *(undefined4 *)(param_1 + 0x82c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x4164) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x82d) = 0;
  param_1[0x833] = 0;
  param_1[0x834] = 0;
  param_1[0x835] = 0;
  *(undefined4 *)(param_1 + 0x831) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x418c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x832) = 0;
  param_1[0x838] = 0;
  param_1[0x839] = 0;
  param_1[0x83a] = 0;
  *(undefined4 *)(param_1 + 0x836) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x41b4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x837) = 0;
  param_1[0x83d] = 0;
  param_1[0x83e] = 0;
  param_1[0x83f] = 0;
  *(undefined4 *)(param_1 + 0x83b) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x41dc) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x83c) = 0;
  param_1[0x842] = 0;
  param_1[0x843] = 0;
  param_1[0x844] = 0;
  *(undefined4 *)(param_1 + 0x840) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x4204) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x841) = 0;
  param_1[0x847] = 0;
  param_1[0x848] = 0;
  param_1[0x849] = 0;
  *(undefined4 *)(param_1 + 0x845) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x422c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x846) = 0;
  param_1[0x84c] = 0;
  param_1[0x84d] = 0;
  param_1[0x84e] = 0;
  *(undefined4 *)(param_1 + 0x84a) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x4254) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x84b) = 0;
  param_1[0x851] = 0;
  param_1[0x852] = 0;
  param_1[0x853] = 0;
  *(undefined4 *)(param_1 + 0x84f) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x427c) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x850) = 0;
  param_1[0x856] = 0;
  param_1[0x857] = 0;
  param_1[0x858] = 0;
  *(undefined4 *)(param_1 + 0x854) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x42a4) = 0;
  FUN_1410095c0();
  *(undefined4 *)(param_1 + 0x855) = 0;
  param_1[0x859] = 0;
  param_1[0x85a] = 0;
  FUN_143093380(param_1 + 0x85b);
  *(undefined1 *)(param_1 + 0x876) = 0;
  param_1[0x877] = 0;
  *(undefined4 *)(param_1 + 0x878) = 0;
  return param_1;
}



// ---- FUN_141028ed0 @ 141028ed0 ----

void FUN_141028ed0(longlong param_1,longlong param_2)

{
  undefined8 local_28;
  undefined1 local_20 [24];
  
  *(undefined4 *)(param_1 + 0x378) = *(undefined4 *)(param_2 + 8);
  *(undefined4 *)(param_1 + 0x37c) = *(undefined4 *)(param_2 + 0xc);
  *(undefined4 *)(param_1 + 0x380) = *(undefined4 *)(param_2 + 0x10);
  local_28 = *(undefined8 *)(param_2 + 0x18);
  FUN_1407e5680(local_20,param_2 + 0x20);
  FUN_1420366d0(param_1 + 0x388,&local_28);
  *(undefined4 *)(param_1 + 0x3a0) = *(undefined4 *)(param_2 + 0x30);
  *(undefined4 *)(param_1 + 0x3a4) = *(undefined4 *)(param_2 + 0x34);
  *(undefined4 *)(param_1 + 0x3a8) = *(undefined4 *)(param_2 + 0x38);
  local_28 = *(undefined8 *)(param_2 + 0x40);
  FUN_1407e5680(local_20,param_2 + 0x48);
  FUN_1420366d0(param_1 + 0x3b0,&local_28);
  *(undefined4 *)(param_1 + 0x3c8) = *(undefined4 *)(param_2 + 0x58);
  *(undefined4 *)(param_1 + 0x3cc) = *(undefined4 *)(param_2 + 0x5c);
  *(undefined4 *)(param_1 + 0x3d0) = *(undefined4 *)(param_2 + 0x60);
  local_28 = *(undefined8 *)(param_2 + 0x68);
  FUN_1407e5680(local_20,param_2 + 0x70);
  FUN_1420366d0(param_1 + 0x3d8,&local_28);
  *(undefined4 *)(param_1 + 0x3f0) = *(undefined4 *)(param_2 + 0x80);
  *(undefined4 *)(param_1 + 0x3f4) = *(undefined4 *)(param_2 + 0x84);
  *(undefined4 *)(param_1 + 0x3f8) = *(undefined4 *)(param_2 + 0x88);
  local_28 = *(undefined8 *)(param_2 + 0x90);
  FUN_1407e5680(local_20,param_2 + 0x98);
  FUN_1420366d0(param_1 + 0x400,&local_28);
  *(undefined4 *)(param_1 + 0x418) = *(undefined4 *)(param_2 + 0xa8);
  *(undefined4 *)(param_1 + 0x41c) = *(undefined4 *)(param_2 + 0xac);
  *(undefined4 *)(param_1 + 0x420) = *(undefined4 *)(param_2 + 0xb0);
  local_28 = *(undefined8 *)(param_2 + 0xb8);
  FUN_1407e5680(local_20,param_2 + 0xc0);
  FUN_1420366d0(param_1 + 0x428,&local_28);
  *(undefined4 *)(param_1 + 0x440) = *(undefined4 *)(param_2 + 0xd0);
  *(undefined4 *)(param_1 + 0x444) = *(undefined4 *)(param_2 + 0xd4);
  *(undefined4 *)(param_1 + 0x448) = *(undefined4 *)(param_2 + 0xd8);
  local_28 = *(undefined8 *)(param_2 + 0xe0);
  FUN_1407e5680(local_20,param_2 + 0xe8);
  FUN_1420366d0(param_1 + 0x450,&local_28);
  *(undefined4 *)(param_1 + 0x468) = *(undefined4 *)(param_2 + 0xf8);
  *(undefined4 *)(param_1 + 0x46c) = *(undefined4 *)(param_2 + 0xfc);
  *(undefined4 *)(param_1 + 0x470) = *(undefined4 *)(param_2 + 0x100);
  local_28 = *(undefined8 *)(param_2 + 0x108);
  FUN_1407e5680(local_20,param_2 + 0x110);
  FUN_1420366d0(param_1 + 0x478,&local_28);
  *(undefined4 *)(param_1 + 0x490) = *(undefined4 *)(param_2 + 0x120);
  *(undefined4 *)(param_1 + 0x494) = *(undefined4 *)(param_2 + 0x124);
  *(undefined4 *)(param_1 + 0x498) = *(undefined4 *)(param_2 + 0x128);
  local_28 = *(undefined8 *)(param_2 + 0x130);
  FUN_1407e5680(local_20,param_2 + 0x138);
  FUN_1420366d0(param_1 + 0x4a0,&local_28);
  *(undefined4 *)(param_1 + 0x4b8) = *(undefined4 *)(param_2 + 0x148);
  *(undefined4 *)(param_1 + 0x4bc) = *(undefined4 *)(param_2 + 0x14c);
  *(undefined4 *)(param_1 + 0x4c0) = *(undefined4 *)(param_2 + 0x150);
  local_28 = *(undefined8 *)(param_2 + 0x158);
  FUN_1407e5680(local_20,param_2 + 0x160);
  FUN_1420366d0(param_1 + 0x4c8,&local_28);
  *(undefined4 *)(param_1 + 0x4e0) = *(undefined4 *)(param_2 + 0x170);
  *(undefined4 *)(param_1 + 0x4e4) = *(undefined4 *)(param_2 + 0x174);
  *(undefined4 *)(param_1 + 0x4e8) = *(undefined4 *)(param_2 + 0x178);
  local_28 = *(undefined8 *)(param_2 + 0x180);
  FUN_1407e5680(local_20,param_2 + 0x188);
  FUN_1420366d0(param_1 + 0x4f0,&local_28);
  *(undefined4 *)(param_1 + 0x508) = *(undefined4 *)(param_2 + 0x198);
  *(undefined4 *)(param_1 + 0x50c) = *(undefined4 *)(param_2 + 0x19c);
  *(undefined4 *)(param_1 + 0x510) = *(undefined4 *)(param_2 + 0x1a0);
  local_28 = *(undefined8 *)(param_2 + 0x1a8);
  FUN_1407e5680(local_20,param_2 + 0x1b0);
  FUN_1420366d0(param_1 + 0x518,&local_28);
  *(undefined4 *)(param_1 + 0x530) = *(undefined4 *)(param_2 + 0x1c0);
  *(undefined4 *)(param_1 + 0x534) = *(undefined4 *)(param_2 + 0x1c4);
  *(undefined4 *)(param_1 + 0x538) = *(undefined4 *)(param_2 + 0x1c8);
  local_28 = *(undefined8 *)(param_2 + 0x1d0);
  FUN_1407e5680(local_20,param_2 + 0x1d8);
  FUN_1420366d0(param_1 + 0x540,&local_28);
  *(undefined4 *)(param_1 + 0x558) = *(undefined4 *)(param_2 + 0x1e8);
  *(undefined4 *)(param_1 + 0x55c) = *(undefined4 *)(param_2 + 0x1ec);
  *(undefined4 *)(param_1 + 0x560) = *(undefined4 *)(param_2 + 0x1f0);
  local_28 = *(undefined8 *)(param_2 + 0x1f8);
  FUN_1407e5680(local_20,param_2 + 0x200);
  FUN_1420366d0(param_1 + 0x568,&local_28);
  *(undefined4 *)(param_1 + 0x580) = *(undefined4 *)(param_2 + 0x210);
  *(undefined4 *)(param_1 + 0x584) = *(undefined4 *)(param_2 + 0x214);
  *(undefined4 *)(param_1 + 0x588) = *(undefined4 *)(param_2 + 0x218);
  local_28 = *(undefined8 *)(param_2 + 0x220);
  FUN_1407e5680(local_20,param_2 + 0x228);
  FUN_1420366d0(param_1 + 0x590,&local_28);
  *(undefined4 *)(param_1 + 0x5a8) = *(undefined4 *)(param_2 + 0x238);
  *(undefined4 *)(param_1 + 0x5ac) = *(undefined4 *)(param_2 + 0x23c);
  *(undefined4 *)(param_1 + 0x5b0) = *(undefined4 *)(param_2 + 0x240);
  local_28 = *(undefined8 *)(param_2 + 0x248);
  FUN_1407e5680(local_20,param_2 + 0x250);
  FUN_1420366d0(param_1 + 0x5b8,&local_28);
  *(undefined4 *)(param_1 + 0x5d0) = *(undefined4 *)(param_2 + 0x260);
  *(undefined4 *)(param_1 + 0x5d4) = *(undefined4 *)(param_2 + 0x264);
  *(undefined4 *)(param_1 + 0x5d8) = *(undefined4 *)(param_2 + 0x268);
  local_28 = *(undefined8 *)(param_2 + 0x270);
  FUN_1407e5680(local_20,param_2 + 0x278);
  FUN_1420366d0(param_1 + 0x5e0,&local_28);
  *(undefined4 *)(param_1 + 0x5f8) = *(undefined4 *)(param_2 + 0x288);
  *(undefined4 *)(param_1 + 0x5fc) = *(undefined4 *)(param_2 + 0x28c);
  *(undefined4 *)(param_1 + 0x600) = *(undefined4 *)(param_2 + 0x290);
  local_28 = *(undefined8 *)(param_2 + 0x298);
  FUN_1407e5680(local_20,param_2 + 0x2a0);
  FUN_1420366d0(param_1 + 0x608,&local_28);
  *(undefined4 *)(param_1 + 0x620) = *(undefined4 *)(param_2 + 0x2b0);
  *(undefined4 *)(param_1 + 0x624) = *(undefined4 *)(param_2 + 0x2b4);
  *(undefined4 *)(param_1 + 0x628) = *(undefined4 *)(param_2 + 0x2b8);
  local_28 = *(undefined8 *)(param_2 + 0x2c0);
  FUN_1407e5680(local_20,param_2 + 0x2c8);
  FUN_1420366d0(param_1 + 0x630,&local_28);
  *(undefined4 *)(param_1 + 0x648) = *(undefined4 *)(param_2 + 0x2d8);
  *(undefined4 *)(param_1 + 0x64c) = *(undefined4 *)(param_2 + 0x2dc);
  *(undefined4 *)(param_1 + 0x650) = *(undefined4 *)(param_2 + 0x2e0);
  local_28 = *(undefined8 *)(param_2 + 0x2e8);
  FUN_1407e5680(local_20,param_2 + 0x2f0);
  FUN_1420366d0(param_1 + 0x658,&local_28);
  *(undefined4 *)(param_1 + 0x670) = *(undefined4 *)(param_2 + 0x300);
  *(undefined4 *)(param_1 + 0x674) = *(undefined4 *)(param_2 + 0x304);
  *(undefined4 *)(param_1 + 0x678) = *(undefined4 *)(param_2 + 0x308);
  local_28 = *(undefined8 *)(param_2 + 0x310);
  FUN_1407e5680(local_20,param_2 + 0x318);
  FUN_1420366d0(param_1 + 0x680,&local_28);
  return;
}



// ---- FUN_1410b9320 @ 1410b9320 ----

void FUN_1410b9320(longlong param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  undefined1 auVar4 [16];
  float fVar5;
  float fVar6;
  float fVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  undefined8 *puVar12;
  longlong lVar13;
  longlong *plVar14;
  undefined8 uVar15;
  float fVar16;
  undefined8 local_128;
  float local_120;
  undefined8 local_118;
  float fStack_110;
  float fStack_10c;
  undefined4 local_108;
  undefined4 uStack_104;
  undefined4 local_100;
  undefined4 local_fc;
  undefined4 local_f8;
  undefined4 local_f4;
  undefined8 local_f0;
  float local_e8;
  undefined8 local_e0;
  undefined4 local_d8;
  undefined8 local_d4;
  undefined4 local_cc;
  undefined1 local_c8 [16];
  undefined1 local_b8 [40];
  undefined1 local_90;
  
  iVar8 = FUN_141147080(*(undefined8 *)(param_1 + 400),1);
  iVar9 = FUN_141147080(*(undefined8 *)(param_1 + 400),0);
  fVar7 = DAT_143c9c104;
  fVar6 = DAT_14391fc5c;
  fVar5 = DAT_14391b46c;
  for (; 0 < iVar8; iVar8 = iVar8 + -1) {
    lVar13 = *(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x168);
    if (lVar13 == 0) {
      local_fc = 0x3f800000;
      puVar12 = (undefined8 *)&local_fc;
      local_f8 = 0x3f800000;
      local_f4 = 0x3f800000;
    }
    else {
      puVar12 = (undefined8 *)&local_108;
      local_108 = *(undefined4 *)(lVar13 + 0x1b0);
      local_100 = *(undefined4 *)(lVar13 + 0x1b8);
      uStack_104 = *(undefined4 *)(lVar13 + 0x1b4);
    }
    local_f0 = *puVar12;
    local_e8 = *(float *)(puVar12 + 1);
    local_128 = *(undefined8 *)(param_1 + 0x160);
    local_120 = *(float *)(param_1 + 0x168) +
                local_e8 * *(float *)(param_1 + 0x224) + *(float *)(param_1 + 0x1bc);
    lVar13 = *(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x168);
    if (lVar13 == 0) {
      local_cc = DAT_145a8af28;
      puVar12 = &local_d4;
      local_d4 = DAT_145a8af20;
    }
    else {
      fVar1 = *(float *)(lVar13 + 400);
      local_118 = *(undefined8 *)(lVar13 + 400);
      fStack_110 = *(float *)(lVar13 + 0x198);
      fStack_10c = *(float *)(lVar13 + 0x19c);
      auVar4._4_4_ = -(uint)(*(float *)(lVar13 + 0x1d4) != *(float *)(lVar13 + 0x194));
      auVar4._0_4_ = -(uint)(*(float *)(lVar13 + 0x1d0) != fVar1);
      auVar4._8_4_ = -(uint)(*(float *)(lVar13 + 0x1d8) != fStack_110);
      auVar4._12_4_ = -(uint)(*(float *)(lVar13 + 0x1dc) != fStack_10c);
      iVar10 = movmskps(*(float *)(param_1 + 0x168),auVar4);
      if (iVar10 != 0) {
        *(float *)(lVar13 + 0x1d0) = fVar1;
        *(float *)(lVar13 + 0x1d4) = *(float *)(lVar13 + 0x194);
        *(float *)(lVar13 + 0x1d8) = fStack_110;
        *(float *)(lVar13 + 0x1dc) = fStack_10c;
        puVar12 = (undefined8 *)FUN_141e4e960(&local_118,local_c8);
        *(undefined8 *)(lVar13 + 0x1e0) = *puVar12;
        *(undefined4 *)(lVar13 + 0x1e8) = *(undefined4 *)(puVar12 + 1);
      }
      local_d8 = *(undefined4 *)(lVar13 + 0x1e8);
      puVar12 = &local_e0;
      local_e0 = *(undefined8 *)(lVar13 + 0x1e0);
    }
    uVar15 = *puVar12;
    fStack_110 = *(float *)(puVar12 + 1);
    fVar1 = *(float *)(param_1 + 0x230);
    iVar10 = rand();
    fVar2 = *(float *)(param_1 + 0x230);
    fVar16 = *(float *)(param_1 + 0x228);
    fVar3 = *(float *)(param_1 + 0x22c);
    local_118 = uVar15;
    iVar11 = rand();
    fVar16 = ((float)iVar11 * fVar6 * (fVar3 - fVar16) + fVar16) * (float)local_f0;
    uVar15 = FUN_143820fb0((((float)iVar10 * fVar6 * fVar1 + (float)((ulonglong)uVar15 >> 0x20)) -
                           fVar2 * fVar5) * fVar7);
    local_128 = CONCAT44(local_128._4_4_ + (float)uVar15 * fVar16,
                         (float)local_128 + (float)((ulonglong)uVar15 >> 0x20) * fVar16);
    FUN_143126c80(local_b8);
    local_90 = 0;
    uVar15 = FUN_1418d84b0();
    lVar13 = *(longlong *)(param_1 + 0xf8);
    if (lVar13 == 0) {
      lVar13 = FUN_142c17e40(param_1);
    }
    plVar14 = (longlong *)FUN_142de92d0(lVar13,uVar15,&local_128,&DAT_145a8af20,local_b8);
    if (plVar14 != (longlong *)0x0) {
      lVar13 = *(longlong *)(param_1 + 400);
      local_118 = *(undefined8 *)(lVar13 + 0x108);
      fStack_110 = *(float *)(lVar13 + 0x110);
      fStack_10c = *(float *)(lVar13 + 0x114);
      FUN_141151540(plVar14[0x98],*(undefined8 *)(lVar13 + 0x100),&local_118);
      if (*(longlong *)(param_1 + 0x1d8) != 0) {
        plVar14[0x94] = *(longlong *)(param_1 + 0x1d8);
      }
      if (*(longlong *)(param_1 + 0x1f0) != 0) {
        plVar14[0x95] = *(longlong *)(param_1 + 0x1f0);
      }
      if (*(longlong *)(param_1 + 0x208) != 0) {
        plVar14[0x96] = *(longlong *)(param_1 + 0x208);
      }
      *(undefined4 *)(plVar14 + 0x97) = *(undefined4 *)(param_1 + 0x21c);
      *(undefined4 *)(plVar14 + 0x99) = *(undefined4 *)(param_1 + 0x220);
      plVar14[0x9b] = *(longlong *)(param_1 + 0x1e0);
      *(undefined4 *)(plVar14 + 0x9c) = *(undefined4 *)(param_1 + 0x1e8);
      *(undefined8 *)((longlong)plVar14 + 0x4e4) = *(undefined8 *)(param_1 + 0x1f8);
      *(undefined4 *)((longlong)plVar14 + 0x4ec) = *(undefined4 *)(param_1 + 0x200);
      plVar14[0x9e] = *(longlong *)(param_1 + 0x210);
      *(undefined4 *)(plVar14 + 0x9f) = *(undefined4 *)(param_1 + 0x218);
      (**(code **)(*plVar14 + 0x6f0))(plVar14);
      FUN_141193790(plVar14,1);
    }
  }
  for (; 0 < iVar9; iVar9 = iVar9 + -1) {
    lVar13 = FUN_1410b97b0(param_1);
    if (lVar13 != 0) {
      FUN_141193790(lVar13,2);
    }
  }
  return;
}



// ---- FUN_1410b97b0 @ 1410b97b0 ----

longlong * FUN_1410b97b0(longlong param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  undefined1 auVar4 [16];
  int iVar5;
  undefined8 *puVar6;
  longlong lVar7;
  longlong *plVar8;
  undefined4 uVar9;
  float fVar11;
  undefined8 uVar10;
  float fVar12;
  float fVar13;
  undefined8 local_c8;
  float local_c0;
  undefined8 local_b8;
  float local_b0;
  undefined8 local_a8;
  undefined4 local_a0;
  float local_98;
  float fStack_94;
  float fStack_90;
  float fStack_8c;
  undefined1 local_88 [40];
  undefined1 local_60;
  
  lVar7 = *(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x168);
  if (lVar7 == 0) {
    local_c8 = 0x3f8000003f800000;
    local_c0 = 1.0;
  }
  else {
    local_c8 = *(undefined8 *)(lVar7 + 0x1b0);
    local_c0 = *(float *)(lVar7 + 0x1b8);
  }
  local_b8 = local_c8;
  local_c8 = *(undefined8 *)(param_1 + 0x160);
  local_b0 = local_c0;
  local_c0 = *(float *)(param_1 + 0x168) +
             local_c0 * *(float *)(param_1 + 0x224) + *(float *)(param_1 + 0x1bc);
  lVar7 = *(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x168);
  if (lVar7 == 0) {
    uVar9 = (undefined4)DAT_145a8af20;
    fVar11 = (float)((ulonglong)DAT_145a8af20 >> 0x20);
    local_a0 = DAT_145a8af28;
  }
  else {
    local_98 = *(float *)(lVar7 + 400);
    fStack_94 = *(float *)(lVar7 + 0x194);
    fStack_90 = *(float *)(lVar7 + 0x198);
    fStack_8c = *(float *)(lVar7 + 0x19c);
    auVar4._4_4_ = -(uint)(*(float *)(lVar7 + 0x1d4) != fStack_94);
    auVar4._0_4_ = -(uint)(*(float *)(lVar7 + 0x1d0) != local_98);
    auVar4._8_4_ = -(uint)(*(float *)(lVar7 + 0x1d8) != fStack_90);
    auVar4._12_4_ = -(uint)(*(float *)(lVar7 + 0x1dc) != fStack_8c);
    iVar5 = movmskps(*(float *)(param_1 + 0x168),auVar4);
    if (iVar5 != 0) {
      *(float *)(lVar7 + 0x1d0) = local_98;
      *(float *)(lVar7 + 0x1d4) = fStack_94;
      *(float *)(lVar7 + 0x1d8) = fStack_90;
      *(float *)(lVar7 + 0x1dc) = fStack_8c;
      puVar6 = (undefined8 *)FUN_141e4e960(&local_98,&local_a8);
      *(undefined8 *)(lVar7 + 0x1e0) = *puVar6;
      *(undefined4 *)(lVar7 + 0x1e8) = *(undefined4 *)(puVar6 + 1);
    }
    uVar9 = (undefined4)*(undefined8 *)(lVar7 + 0x1e0);
    fVar11 = (float)((ulonglong)*(undefined8 *)(lVar7 + 0x1e0) >> 0x20);
    local_a0 = *(undefined4 *)(lVar7 + 0x1e8);
  }
  fVar1 = *(float *)(param_1 + 0x230);
  local_a8 = CONCAT44(fVar11,uVar9);
  iVar5 = rand();
  fVar2 = *(float *)(param_1 + 0x230);
  fVar12 = *(float *)(param_1 + 0x228);
  fVar13 = (float)iVar5 * DAT_14391fc5c;
  fVar3 = *(float *)(param_1 + 0x22c);
  iVar5 = rand();
  fVar12 = ((float)iVar5 * DAT_14391fc5c * (fVar3 - fVar12) + fVar12) * (float)local_b8;
  uVar10 = FUN_143820fb0(((fVar13 * fVar1 + fVar11) - fVar2 * DAT_14391b46c) * DAT_143c9c104);
  local_c8 = CONCAT44(local_c8._4_4_ + (float)uVar10 * fVar12,
                      (float)local_c8 + (float)((ulonglong)uVar10 >> 0x20) * fVar12);
  FUN_143126c80(local_88);
  local_60 = 0;
  uVar10 = FUN_1418d84b0();
  lVar7 = *(longlong *)(param_1 + 0xf8);
  if (lVar7 == 0) {
    lVar7 = FUN_142c17e40(param_1);
  }
  plVar8 = (longlong *)FUN_142de92d0(lVar7,uVar10,&local_c8,&DAT_145a8af20,local_88);
  if (plVar8 != (longlong *)0x0) {
    lVar7 = *(longlong *)(param_1 + 400);
    local_98 = *(float *)(lVar7 + 0x108);
    fStack_94 = *(float *)(lVar7 + 0x10c);
    fStack_90 = *(float *)(lVar7 + 0x110);
    fStack_8c = *(float *)(lVar7 + 0x114);
    FUN_141151540(plVar8[0x98],*(undefined8 *)(lVar7 + 0x100),&local_98);
    if (*(longlong *)(param_1 + 0x1d8) != 0) {
      plVar8[0x94] = *(longlong *)(param_1 + 0x1d8);
    }
    if (*(longlong *)(param_1 + 0x1f0) != 0) {
      plVar8[0x95] = *(longlong *)(param_1 + 0x1f0);
    }
    if (*(longlong *)(param_1 + 0x208) != 0) {
      plVar8[0x96] = *(longlong *)(param_1 + 0x208);
    }
    *(undefined4 *)(plVar8 + 0x97) = *(undefined4 *)(param_1 + 0x21c);
    *(undefined4 *)(plVar8 + 0x99) = *(undefined4 *)(param_1 + 0x220);
    plVar8[0x9b] = *(longlong *)(param_1 + 0x1e0);
    *(undefined4 *)(plVar8 + 0x9c) = *(undefined4 *)(param_1 + 0x1e8);
    *(undefined8 *)((longlong)plVar8 + 0x4e4) = *(undefined8 *)(param_1 + 0x1f8);
    *(undefined4 *)((longlong)plVar8 + 0x4ec) = *(undefined4 *)(param_1 + 0x200);
    plVar8[0x9e] = *(longlong *)(param_1 + 0x210);
    *(undefined4 *)(plVar8 + 0x9f) = *(undefined4 *)(param_1 + 0x218);
    (**(code **)(*plVar8 + 0x6f0))(plVar8);
  }
  return plVar8;
}



// ---- FUN_14112db50 @ 14112db50 ----

void FUN_14112db50(longlong param_1,longlong *param_2)

{
  longlong lVar1;
  undefined4 local_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  lVar1 = *(longlong *)(param_1 + 0x658);
  local_18 = *(undefined4 *)(lVar1 + 0x108);
  uStack_14 = *(undefined4 *)(lVar1 + 0x10c);
  uStack_10 = *(undefined4 *)(lVar1 + 0x110);
  uStack_c = *(undefined4 *)(lVar1 + 0x114);
  FUN_141151540(param_2[0x98],*(undefined8 *)(lVar1 + 0x100),&local_18);
  *(undefined1 *)(param_1 + 0x770) = 1;
  if (*(longlong *)(param_1 + 0x698) != 0) {
    param_2[0x94] = *(longlong *)(param_1 + 0x698);
  }
  if (*(longlong *)(param_1 + 0x6b0) != 0) {
    param_2[0x95] = *(longlong *)(param_1 + 0x6b0);
  }
  if (*(longlong *)(param_1 + 0x6c8) != 0) {
    param_2[0x96] = *(longlong *)(param_1 + 0x6c8);
  }
  if (*(longlong *)(param_1 + 0x598) != 0) {
    param_2[0x6a] = *(longlong *)(param_1 + 0x598);
  }
  if (*(longlong *)(param_1 + 0x5a0) != 0) {
    param_2[0x6b] = *(longlong *)(param_1 + 0x5a0);
  }
  if (*(longlong *)(param_1 + 0x708) != 0) {
    param_2[0x6f] = *(longlong *)(param_1 + 0x708);
  }
  if (*(longlong *)(param_1 + 0x710) != 0) {
    param_2[0x70] = *(longlong *)(param_1 + 0x710);
  }
  if (*(longlong *)(param_1 + 0x718) != 0) {
    param_2[0x71] = *(longlong *)(param_1 + 0x718);
  }
  if (*(longlong *)(param_1 + 0x720) != 0) {
    param_2[0x72] = *(longlong *)(param_1 + 0x720);
  }
  *(undefined4 *)(param_2 + 0x97) = *(undefined4 *)(param_1 + 0x6dc);
  *(undefined4 *)(param_2 + 0x99) = *(undefined4 *)(param_1 + 0x6e0);
  param_2[0x9b] = *(longlong *)(param_1 + 0x6a0);
  *(undefined4 *)(param_2 + 0x9c) = *(undefined4 *)(param_1 + 0x6a8);
  *(undefined8 *)((longlong)param_2 + 0x4e4) = *(undefined8 *)(param_1 + 0x6b8);
  *(undefined4 *)((longlong)param_2 + 0x4ec) = *(undefined4 *)(param_1 + 0x6c0);
  param_2[0x9e] = *(longlong *)(param_1 + 0x6d0);
  *(undefined4 *)(param_2 + 0x9f) = *(undefined4 *)(param_1 + 0x6d8);
                    /* WARNING: Could not recover jumptable at 0x00014112dcb9. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_2 + 0x6f0))(param_2);
  return;
}



// ---- FUN_141175b90 @ 141175b90 ----

undefined8 * FUN_141175b90(undefined8 *param_1)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined1 local_res8 [8];
  
  uVar1 = FUN_142040a20();
  FUN_1410c68a0(param_1,uVar1);
  FUN_140d16120(param_1 + 0x80);
  *(undefined4 *)((longlong)param_1 + 0x4cc) = 0x42c80000;
  *param_1 = &PTR_FUN_143cf90f0;
  *(undefined4 *)(param_1 + 0x9a) = 0x41200000;
  param_1[0x78] = &PTR_LAB_143cf97f0;
  *(undefined4 *)((longlong)param_1 + 0x4d4) = 0x40a00000;
  param_1[0x80] = &PTR_LAB_143cf9880;
  param_1[0x94] = 0;
  param_1[0x95] = 0;
  param_1[0x96] = 0;
  *(undefined4 *)(param_1 + 0x97) = 0;
  param_1[0x98] = 0;
  *(undefined4 *)(param_1 + 0x99) = 0;
  *(undefined4 *)(param_1 + 0x9b) = 0x3f800000;
  *(undefined4 *)((longlong)param_1 + 0x4dc) = 0x3f800000;
  *(undefined4 *)(param_1 + 0x9c) = 0x3f800000;
  *(undefined4 *)((longlong)param_1 + 0x4e4) = 0x3f800000;
  *(undefined4 *)(param_1 + 0x9d) = 0x3f800000;
  *(undefined4 *)((longlong)param_1 + 0x4ec) = 0x3f800000;
  *(undefined4 *)(param_1 + 0x9e) = 0x3f800000;
  *(undefined4 *)((longlong)param_1 + 0x4f4) = 0x3f800000;
  *(undefined4 *)(param_1 + 0x9f) = 0x3f800000;
  *(byte *)((longlong)param_1 + 0x44) = *(byte *)((longlong)param_1 + 0x44) | 2;
  *(undefined1 *)((longlong)param_1 + 0x4fc) = 0;
  param_1[0xa1] = 0;
  param_1[0xa2] = 0;
  param_1[0xa3] = 0;
  param_1[0xa4] = 0;
  *(undefined4 *)(param_1 + 0xa5) = 0;
  *(undefined4 *)((longlong)param_1 + 0x52c) = 0x41200000;
  *(undefined2 *)((longlong)param_1 + 0x534) = 1;
  *(undefined4 *)(param_1 + 0xa7) = 0;
  *(undefined1 *)((longlong)param_1 + 0x53c) = 0xff;
  *(undefined4 *)(param_1 + 0xa8) = 0x3f800000;
  puVar2 = (undefined8 *)FUN_141eb3490(local_res8,L"FieldItemTableComponent",1);
  uVar1 = *puVar2;
  uVar3 = FUN_1418b5b00();
  uVar1 = FUN_142006400(param_1,uVar1,uVar3,uVar3,1,0,0);
  param_1[0x98] = uVar1;
  return param_1;
}



// ---- FUN_1411cb350 @ 1411cb350 ----

undefined8 * FUN_1411cb350(undefined8 *param_1)

{
  FUN_141a4b5a0();
  *param_1 = &PTR_FUN_143d1e768;
  *(undefined4 *)(param_1 + 0x9c) = 0;
  *(undefined4 *)((longlong)param_1 + 0x4e4) = DAT_145a8af08;
  *(undefined4 *)(param_1 + 0x9d) = DAT_145a8af0c;
  param_1[0x9e] = 0;
  param_1[0x9f] = 0;
  param_1[0xa0] = 2;
  return param_1;
}



// ---- FUN_1411cb3c0 @ 1411cb3c0 ----

undefined8 * FUN_1411cb3c0(undefined8 *param_1)

{
  FUN_141a4b5a0();
  *param_1 = &PTR_FUN_143d1e768;
  *(undefined4 *)(param_1 + 0x9c) = 0;
  *(undefined4 *)((longlong)param_1 + 0x4e4) = DAT_145a8af08;
  *(undefined4 *)(param_1 + 0x9d) = DAT_145a8af0c;
  param_1[0x9e] = 0;
  param_1[0x9f] = 0;
  param_1[0xa0] = 2;
  *param_1 = &PTR_FUN_143d22e78;
  param_1[0xa1] = 0;
  param_1[0xa2] = 0;
  param_1[0xa3] = 0;
  param_1[0xa4] = 0;
  param_1[0xa7] = 0;
  param_1[0xa8] = 0;
  *(undefined4 *)(param_1 + 0xa9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x54c) = 0;
  param_1[0xab] = 0;
  *(undefined4 *)(param_1 + 0xac) = 0;
  param_1[0xad] = 0;
  param_1[0xae] = 0;
  param_1[0xaf] = 0;
  param_1[0xb0] = 0;
  param_1[0xb2] = 0;
  param_1[0xb3] = 0;
  param_1[0xb4] = 0;
  param_1[0xb5] = 0;
  param_1[0xb6] = 0;
  param_1[0xb7] = 0;
  *(undefined4 *)(param_1 + 0xb8) = 0;
  *(undefined1 *)((longlong)param_1 + 0x5c4) = 0;
  param_1[0xba] = 0;
  param_1[0xbb] = 0;
  param_1[0xbc] = 0;
  param_1[0xbe] = 0;
  param_1[0xbf] = 0;
  param_1[0xc0] = 0;
  param_1[0xc1] = 0;
  param_1[0xb9] = 0;
  *(undefined4 *)(param_1 + 0xbd) = 0;
  return param_1;
}



// ---- FUN_1411cb9d0 @ 1411cb9d0 ----

undefined8 * FUN_1411cb9d0(undefined8 *param_1)

{
  undefined4 uVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined1 local_res8 [8];
  
  FUN_141132780();
  FUN_1411f3dc0(param_1 + 0x6d);
  puVar5 = param_1 + 0x72;
  FUN_140d16120(puVar5);
  *(undefined2 *)(param_1 + 0x86) = 0;
  *param_1 = &PTR_FUN_143d1ee70;
  param_1[0x6d] = &PTR_LAB_143d1f4a8;
  *puVar5 = &PTR_LAB_143d1f538;
  param_1[0x87] = 0;
  *(undefined1 *)((longlong)param_1 + 0x471) = 1;
  param_1[0x8f] = &PTR_FUN_143d0d5f8;
  param_1[0x91] = 0;
  param_1[0x92] = 0;
  param_1[0x94] = 0;
  param_1[0x95] = 0;
  param_1[0x96] = 0;
  param_1[0x97] = 0;
  param_1[0x9a] = 0;
  param_1[0x9b] = 0;
  *(undefined4 *)(param_1 + 0x9c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x4e4) = 0;
  param_1[0x9e] = 0;
  *(undefined4 *)(param_1 + 0x9f) = 0;
  param_1[0xa0] = &PTR_FUN_143d0d628;
  param_1[0xa2] = 0;
  param_1[0xa3] = 0;
  param_1[0xa4] = 0;
  param_1[0xa5] = 0;
  param_1[0xa7] = 0;
  param_1[0xa8] = 0;
  param_1[0xa9] = 0;
  param_1[0xaa] = 0;
  param_1[0xac] = 0;
  param_1[0xad] = 0;
  *(undefined1 *)(param_1 + 0xae) = 0;
  param_1[0xaf] = 0;
  param_1[0xb0] = 0;
  *(undefined1 *)(param_1 + 0xb1) = 0;
  param_1[0xb2] = 0;
  param_1[0xb3] = 0;
  param_1[0xb6] = 0;
  param_1[0xb7] = 0;
  *(undefined4 *)(param_1 + 0xb8) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x5c4) = 0;
  param_1[0xba] = 0;
  *(undefined4 *)(param_1 + 0xbb) = 0;
  FUN_1411765c0(param_1 + 0xbc);
  uVar1 = DAT_143aaf408;
  param_1[0xdc] = 0;
  param_1[0xdd] = 0;
  param_1[0xde] = 0;
  param_1[0xdf] = 0;
  *(undefined4 *)(param_1 + 0xe0) = 0;
  param_1[0xe1] = 0;
  *(undefined4 *)(param_1 + 0xe2) = 0;
  param_1[0xe3] = 0;
  *(undefined4 *)(param_1 + 0xe4) = 0;
  *(undefined2 *)(param_1 + 0xe5) = 0;
  param_1[0xe7] = 0;
  param_1[0xe8] = 0;
  param_1[0xeb] = 0;
  param_1[0xec] = 0;
  *(undefined2 *)(param_1 + 0xee) = 0;
  param_1[0xef] = 0;
  param_1[0xf0] = 0;
  param_1[0xf7] = 0;
  param_1[0xf8] = 0;
  *(byte *)((longlong)param_1 + 0x44) = *(byte *)((longlong)param_1 + 0x44) | 2;
  param_1[0xfa] = 1;
  param_1[0x6a] = 0;
  *(undefined4 *)(param_1 + 0x6b) = uVar1;
  puVar2 = (undefined8 *)FUN_141eb3490(local_res8,L"KiSearchComponent",1);
  uVar6 = *puVar2;
  uVar3 = FUN_1418c46b0();
  lVar4 = FUN_142006400(param_1,uVar6,uVar3,uVar3,1,0,0);
  param_1[0xf2] = lVar4;
  if (lVar4 != 0) {
    *(undefined8 **)(lVar4 + 0x188) = puVar5;
    *(undefined1 *)(param_1[0xf2] + 0x170) = 8;
    *(undefined1 *)(param_1[0xf2] + 0x171) = 1;
  }
  puVar5 = (undefined8 *)FUN_141eb3490(local_res8,L"QuestActorFindListComponent",1);
  uVar6 = *puVar5;
  uVar3 = FUN_1418eddc0();
  uVar6 = FUN_142006400(param_1,uVar6,uVar3,uVar3,1,0,0);
  param_1[0xf5] = uVar6;
  return param_1;
}



// ---- FUN_1411d4850 @ 1411d4850 ----

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1411d4850(longlong param_1)

{
  _DAT_1459647a8 = (float)*(int *)(*(longlong *)(param_1 + 0x388) + 0x40);
  _DAT_1459647ac = (float)*(int *)(*(longlong *)(param_1 + 0x388) + 0x44);
  *(float *)(param_1 + 0x4e4) = _DAT_1459647a8;
  *(float *)(param_1 + 0x4e8) = _DAT_1459647ac;
  FUN_142d80700();
  FUN_1407e7240(param_1 + 0x4f0);
  return;
}



// ---- FUN_1411d48b0 @ 1411d48b0 ----

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1411d48b0(longlong *param_1)

{
  bool bVar1;
  longlong lVar2;
  longlong lVar3;
  undefined8 uVar4;
  
  _DAT_1459647ac = (float)*(int *)(param_1[0x71] + 0x44);
  _DAT_1459647a8 = (float)*(int *)(param_1[0x71] + 0x40);
  *(float *)(param_1 + 0x9d) = _DAT_1459647ac;
  *(float *)((longlong)param_1 + 0x4e4) = _DAT_1459647a8;
  FUN_142d80700();
  FUN_1407e7240(param_1 + 0x9e);
  if (*(char *)((longlong)param_1 + 0x5c2) == '\0') {
    return;
  }
  lVar2 = (**(code **)(*param_1 + 0x148))(param_1);
  lVar2 = *(longlong *)(lVar2 + 0x140);
  if (lVar2 != 0) {
    lVar3 = FUN_14184ec60();
    if ((*(int *)(*(longlong *)(lVar2 + 0x10) + 0x90) < *(int *)(lVar3 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar2 + 0x10) + 0x88) + (longlong)*(int *)(lVar3 + 0x90) * 8)
        != lVar3 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) goto LAB_1411d4963;
  }
  lVar2 = 0;
LAB_1411d4963:
  lVar2 = *(longlong *)(lVar2 + 0x448);
  if (lVar2 != 0) {
    FUN_14157c470(lVar2,param_1);
    if (param_1[0xae] != 0) {
      FUN_1414ba240(param_1[0xae],lVar2);
    }
    if (param_1[0xaf] != 0) {
      FUN_141557c90(param_1[0xaf],lVar2);
    }
    uVar4 = FUN_140d29250();
    FUN_14128b160(uVar4,param_1);
  }
  return;
}



// ---- FUN_1411e6f80 @ 1411e6f80 ----

void FUN_1411e6f80(longlong param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  undefined8 uVar7;
  longlong *plVar8;
  longlong *plVar9;
  longlong lVar10;
  longlong lVar11;
  longlong lVar12;
  ulonglong uVar13;
  char cVar14;
  longlong lVar15;
  ulonglong uVar16;
  undefined4 *puVar17;
  undefined8 local_res8;
  longlong local_res10 [3];
  longlong *local_e8;
  undefined8 local_e0;
  undefined1 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  longlong local_c8;
  longlong local_c0 [2];
  undefined1 local_b0;
  undefined4 local_ac;
  undefined1 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  longlong local_98;
  longlong local_90 [3];
  undefined1 local_78 [8];
  undefined1 local_70 [8];
  undefined1 local_68 [8];
  undefined1 local_60 [8];
  undefined1 local_58 [8];
  undefined1 local_50 [16];
  
  uVar7 = FUN_14165b2e0();
  FUN_14165b050(uVar7);
  cVar14 = *(char *)(param_1 + 0x7b0);
  lVar11 = 0;
  local_res10[0] = 0;
  if (cVar14 == '\x01') {
    local_res10[0] = *(longlong *)(param_1 + 0x490);
    lVar12 = *(longlong *)(param_1 + 0x488);
    if (*(int *)(param_1 + 0x4b8) != *(int *)(param_1 + 0x4e4)) {
      iVar5 = (int)((ulonglong)lVar12 >> 0x20);
      uVar13 = (longlong)*(int *)(param_1 + 0x4f8) - 1;
      uVar16 = (ulonglong)(*(longlong *)(param_1 + 0x4f0) != 0) - 1 & param_1 + 0x4e8U |
               *(ulonglong *)(param_1 + 0x4f0);
      iVar6 = *(int *)(uVar16 + ((longlong)(iVar5 + (int)lVar12) & uVar13) * 4);
      if (iVar6 != -1) {
        lVar10 = *(longlong *)(param_1 + 0x4b0);
        do {
          if (*(longlong *)(lVar10 + (longlong)iVar6 * 0x18) == lVar12) {
            if (iVar6 != -1) {
              if ((*(int *)(param_1 + 0x4b8) == *(int *)(param_1 + 0x4e4)) ||
                 (iVar6 = *(int *)(uVar16 + ((longlong)(iVar5 + (int)lVar12) & uVar13) * 4),
                 iVar6 == -1)) goto LAB_1411e7091;
              goto LAB_1411e7071;
            }
            break;
          }
          iVar6 = *(int *)(lVar10 + 0x10 + (longlong)iVar6 * 0x18);
        } while (iVar6 != -1);
      }
    }
  }
  else if (cVar14 == '\x02') {
    plVar8 = (longlong *)FUN_1407e9f70(param_1 + 0x500,local_78);
    local_res10[0] = *plVar8;
    lVar12 = *(longlong *)(param_1 + 0x510);
    if (*(int *)(param_1 + 0x598) != *(int *)(param_1 + 0x5c4)) {
      iVar5 = (int)((ulonglong)lVar12 >> 0x20);
      uVar13 = (longlong)*(int *)(param_1 + 0x5d8) - 1;
      uVar16 = (ulonglong)(*(longlong *)(param_1 + 0x5d0) != 0) - 1 & param_1 + 0x5c8U |
               *(ulonglong *)(param_1 + 0x5d0);
      iVar6 = *(int *)(uVar16 + ((longlong)(iVar5 + (int)lVar12) & uVar13) * 4);
      if (iVar6 != -1) {
        lVar10 = *(longlong *)(param_1 + 0x590);
        do {
          if (*(longlong *)(lVar10 + (longlong)iVar6 * 0x18) == lVar12) {
            if (iVar6 != -1) {
              if ((*(int *)(param_1 + 0x598) == *(int *)(param_1 + 0x5c4)) ||
                 (iVar6 = *(int *)(uVar16 + ((longlong)(iVar5 + (int)lVar12) & uVar13) * 4),
                 iVar6 == -1)) goto LAB_1411e7230;
              goto LAB_1411e7210;
            }
            break;
          }
          iVar6 = *(int *)(lVar10 + 0x10 + (longlong)iVar6 * 0x18);
        } while (iVar6 != -1);
      }
    }
  }
  else if (cVar14 == '\x03') {
    plVar8 = (longlong *)FUN_1407e9f70(param_1 + 0x500,local_70);
    local_res10[0] = *plVar8;
    lVar12 = *(longlong *)(param_1 + 0x510);
    if (*(int *)(param_1 + 0x598) != *(int *)(param_1 + 0x5c4)) {
      iVar5 = (int)((ulonglong)lVar12 >> 0x20);
      uVar13 = (longlong)*(int *)(param_1 + 0x5d8) - 1;
      uVar16 = (ulonglong)(*(longlong *)(param_1 + 0x5d0) != 0) - 1 & param_1 + 0x5c8U |
               *(ulonglong *)(param_1 + 0x5d0);
      iVar6 = *(int *)(uVar16 + ((longlong)(iVar5 + (int)lVar12) & uVar13) * 4);
      if (iVar6 != -1) {
        lVar10 = *(longlong *)(param_1 + 0x590);
        do {
          if (*(longlong *)(lVar10 + (longlong)iVar6 * 0x18) == lVar12) {
            if (iVar6 != -1) {
              if ((*(int *)(param_1 + 0x598) == *(int *)(param_1 + 0x5c4)) ||
                 (iVar6 = *(int *)(uVar16 + ((longlong)(iVar5 + (int)lVar12) & uVar13) * 4),
                 iVar6 == -1)) goto LAB_1411e7391;
              goto LAB_1411e7371;
            }
            break;
          }
          iVar6 = *(int *)(lVar10 + 0x10 + (longlong)iVar6 * 0x18);
        } while (iVar6 != -1);
      }
    }
  }
  else if (cVar14 == '\x04') {
    plVar8 = (longlong *)FUN_1407e9f70(param_1 + 0x500,local_68);
    local_res10[0] = *plVar8;
    lVar12 = *(longlong *)(param_1 + 0x510);
    if (*(int *)(param_1 + 0x598) != *(int *)(param_1 + 0x5c4)) {
      iVar5 = (int)((ulonglong)lVar12 >> 0x20);
      uVar13 = (longlong)*(int *)(param_1 + 0x5d8) - 1;
      uVar16 = (ulonglong)(*(longlong *)(param_1 + 0x5d0) != 0) - 1 & param_1 + 0x5c8U |
               *(ulonglong *)(param_1 + 0x5d0);
      iVar6 = *(int *)(uVar16 + ((longlong)(iVar5 + (int)lVar12) & uVar13) * 4);
      if (iVar6 != -1) {
        lVar10 = *(longlong *)(param_1 + 0x590);
        do {
          if (*(longlong *)(lVar10 + (longlong)iVar6 * 0x18) == lVar12) {
            if (iVar6 != -1) {
              if ((*(int *)(param_1 + 0x598) == *(int *)(param_1 + 0x5c4)) ||
                 (iVar6 = *(int *)(uVar16 + ((longlong)(iVar5 + (int)lVar12) & uVar13) * 4),
                 iVar6 == -1)) goto LAB_1411e74f1;
              goto LAB_1411e74d1;
            }
            break;
          }
          iVar6 = *(int *)(lVar10 + 0x10 + (longlong)iVar6 * 0x18);
        } while (iVar6 != -1);
      }
    }
  }
  else if (cVar14 == '\x05') {
    plVar8 = (longlong *)FUN_1407e9f70(param_1 + 0x500,local_60);
    local_res10[0] = *plVar8;
    lVar12 = *(longlong *)(param_1 + 0x510);
    if (*(int *)(param_1 + 0x598) != *(int *)(param_1 + 0x5c4)) {
      iVar5 = (int)((ulonglong)lVar12 >> 0x20);
      uVar13 = (longlong)*(int *)(param_1 + 0x5d8) - 1;
      uVar16 = (ulonglong)(*(longlong *)(param_1 + 0x5d0) != 0) - 1 & param_1 + 0x5c8U |
               *(ulonglong *)(param_1 + 0x5d0);
      iVar6 = *(int *)(uVar16 + ((longlong)(iVar5 + (int)lVar12) & uVar13) * 4);
      if (iVar6 != -1) {
        lVar10 = *(longlong *)(param_1 + 0x590);
        do {
          if (*(longlong *)(lVar10 + (longlong)iVar6 * 0x18) == lVar12) {
            if (iVar6 != -1) {
              if ((*(int *)(param_1 + 0x598) == *(int *)(param_1 + 0x5c4)) ||
                 (iVar6 = *(int *)(uVar16 + ((longlong)(iVar5 + (int)lVar12) & uVar13) * 4),
                 iVar6 == -1)) goto LAB_1411e7681;
              goto LAB_1411e7661;
            }
            break;
          }
          iVar6 = *(int *)(lVar10 + 0x10 + (longlong)iVar6 * 0x18);
        } while (iVar6 != -1);
      }
    }
  }
  else if (cVar14 == '\x06') {
    plVar8 = (longlong *)FUN_1407e9f70(param_1 + 0x500,local_58);
    local_res10[0] = *plVar8;
    lVar12 = *(longlong *)(param_1 + 0x510);
    if (*(int *)(param_1 + 0x598) != *(int *)(param_1 + 0x5c4)) {
      iVar5 = (int)((ulonglong)lVar12 >> 0x20);
      uVar13 = (longlong)*(int *)(param_1 + 0x5d8) - 1;
      uVar16 = (ulonglong)(*(longlong *)(param_1 + 0x5d0) != 0) - 1 & param_1 + 0x5c8U |
               *(ulonglong *)(param_1 + 0x5d0);
      iVar6 = *(int *)(uVar16 + ((longlong)(iVar5 + (int)lVar12) & uVar13) * 4);
      if (iVar6 != -1) {
        lVar10 = *(longlong *)(param_1 + 0x590);
        do {
          if (*(longlong *)(lVar10 + (longlong)iVar6 * 0x18) == lVar12) {
            if (iVar6 != -1) {
              if ((*(int *)(param_1 + 0x598) == *(int *)(param_1 + 0x5c4)) ||
                 (iVar6 = *(int *)(uVar16 + ((longlong)(iVar5 + (int)lVar12) & uVar13) * 4),
                 iVar6 == -1)) goto LAB_1411e7811;
              goto LAB_1411e77f1;
            }
            break;
          }
          iVar6 = *(int *)(lVar10 + 0x10 + (longlong)iVar6 * 0x18);
        } while (iVar6 != -1);
      }
    }
  }
  else if (cVar14 == '\a') {
    plVar9 = (longlong *)FUN_1407e9f70(param_1 + 0x500,local_50);
    lVar12 = *(longlong *)(param_1 + 0x510);
    plVar8 = (longlong *)(param_1 + 0x590);
    local_res10[0] = *plVar9;
    if (*(int *)(param_1 + 0x598) != *(int *)(param_1 + 0x5c4)) {
      lVar10 = *(longlong *)(param_1 + 0x5d0);
      if (*(longlong *)(param_1 + 0x5d0) == 0) {
        lVar10 = param_1 + 0x5c8;
      }
      iVar6 = *(int *)(lVar10 + ((longlong)((int)((ulonglong)lVar12 >> 0x20) + (int)lVar12) &
                                (longlong)*(int *)(param_1 + 0x5d8) - 1U) * 4);
      if (iVar6 != -1) {
        do {
          if (*(longlong *)(*plVar8 + (longlong)iVar6 * 0x18) == lVar12) {
            if (iVar6 != -1) {
              FUN_1408bd5d0(plVar8,&local_res8);
              if ((int)local_res8 != -1) {
                lVar11 = *plVar8 + (longlong)(int)local_res8 * 0x18;
              }
              lVar11 = *(longlong *)(lVar11 + 8);
              local_res10[1] = 0;
              if ((*(longlong *)(lVar11 + 8) != 0) &&
                 ((*(char *)(param_1 + 0x7d1) == '\0' || (*(char *)(lVar11 + 0x2c) == '\0')))) {
                FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 8),*(undefined4 *)(lVar11 + 0x20));
              }
              local_res10[1] = 0;
              if ((*(longlong *)(lVar11 + 0x10) != 0) &&
                 ((*(char *)(param_1 + 0x7d1) == '\0' || (*(char *)(lVar11 + 0x2d) == '\0')))) {
                FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x10),*(undefined4 *)(lVar11 + 0x24));
              }
              local_res10[1] = 0;
              if ((*(longlong *)(lVar11 + 0x18) != 0) &&
                 ((*(char *)(param_1 + 0x7d1) == '\0' || (*(char *)(lVar11 + 0x2e) == '\0')))) {
                FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x18),*(undefined4 *)(lVar11 + 0x28));
              }
              lVar11 = FUN_140d25a50();
              if (lVar11 != 0) {
                FUN_141257c60(lVar11,local_res10);
              }
            }
            break;
          }
          iVar6 = *(int *)(*plVar8 + 0x10 + (longlong)iVar6 * 0x18);
        } while (iVar6 != -1);
      }
    }
  }
  goto LAB_1411e7a15;
  while (iVar6 = *(int *)(lVar15 + 0x10), iVar6 != -1) {
LAB_1411e77f1:
    lVar15 = lVar10 + (longlong)iVar6 * 0x18;
    if (*(longlong *)(lVar10 + (longlong)iVar6 * 0x18) == lVar12) {
      if (iVar6 != -1) goto LAB_1411e7814;
      break;
    }
  }
LAB_1411e7811:
  lVar15 = lVar11;
LAB_1411e7814:
  lVar11 = *(longlong *)(lVar15 + 8);
  local_res8 = 0;
  if ((*(longlong *)(lVar11 + 8) != 0) &&
     ((*(char *)(param_1 + 0x7d1) == '\0' || (*(char *)(lVar11 + 0x2c) == '\0')))) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 8),*(undefined4 *)(lVar11 + 0x20));
  }
  local_res8 = 0;
  if ((*(longlong *)(lVar11 + 0x10) != 0) &&
     ((*(char *)(param_1 + 0x7d1) == '\0' || (*(char *)(lVar11 + 0x2d) == '\0')))) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x10),*(undefined4 *)(lVar11 + 0x24));
  }
  local_res8 = 0;
  if ((*(longlong *)(lVar11 + 0x18) != 0) &&
     ((*(char *)(param_1 + 0x7d1) == '\0' || (*(char *)(lVar11 + 0x2e) == '\0')))) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x18),*(undefined4 *)(lVar11 + 0x28));
  }
  lVar11 = FUN_140d25a50();
  if (lVar11 != 0) {
    FUN_141257c50(lVar11,local_res10);
  }
  goto LAB_1411e7a15;
  while (iVar6 = *(int *)(lVar15 + 0x10), iVar6 != -1) {
LAB_1411e7661:
    lVar15 = lVar10 + (longlong)iVar6 * 0x18;
    if (*(longlong *)(lVar10 + (longlong)iVar6 * 0x18) == lVar12) {
      if (iVar6 != -1) goto LAB_1411e7684;
      break;
    }
  }
LAB_1411e7681:
  lVar15 = lVar11;
LAB_1411e7684:
  lVar11 = *(longlong *)(lVar15 + 8);
  local_res8 = 0;
  if ((*(longlong *)(lVar11 + 8) != 0) &&
     ((*(char *)(param_1 + 0x7d1) == '\0' || (*(char *)(lVar11 + 0x2c) == '\0')))) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 8),*(undefined4 *)(lVar11 + 0x20));
  }
  local_res8 = 0;
  if ((*(longlong *)(lVar11 + 0x10) != 0) &&
     ((*(char *)(param_1 + 0x7d1) == '\0' || (*(char *)(lVar11 + 0x2d) == '\0')))) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x10),*(undefined4 *)(lVar11 + 0x24));
  }
  local_res8 = 0;
  if ((*(longlong *)(lVar11 + 0x18) != 0) &&
     ((*(char *)(param_1 + 0x7d1) == '\0' || (*(char *)(lVar11 + 0x2e) == '\0')))) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x18),*(undefined4 *)(lVar11 + 0x28));
  }
  lVar11 = FUN_140d25a50();
  if (lVar11 != 0) {
    FUN_141258700(lVar11,local_res10);
  }
  goto LAB_1411e7a15;
  while (iVar6 = *(int *)(lVar15 + 0x10), iVar6 != -1) {
LAB_1411e74d1:
    lVar15 = lVar10 + (longlong)iVar6 * 0x18;
    if (*(longlong *)(lVar10 + (longlong)iVar6 * 0x18) == lVar12) {
      if (iVar6 != -1) goto LAB_1411e74f4;
      break;
    }
  }
LAB_1411e74f1:
  lVar15 = lVar11;
LAB_1411e74f4:
  lVar11 = *(longlong *)(lVar15 + 8);
  local_res8 = 0;
  if ((*(longlong *)(lVar11 + 8) != 0) &&
     ((*(char *)(param_1 + 0x7d1) == '\0' || (*(char *)(lVar11 + 0x2c) == '\0')))) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 8),*(undefined4 *)(lVar11 + 0x20));
  }
  local_res8 = 0;
  if ((*(longlong *)(lVar11 + 0x10) != 0) &&
     ((*(char *)(param_1 + 0x7d1) == '\0' || (*(char *)(lVar11 + 0x2d) == '\0')))) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x10),*(undefined4 *)(lVar11 + 0x24));
  }
  local_res8 = 0;
  if ((*(longlong *)(lVar11 + 0x18) != 0) &&
     ((*(char *)(param_1 + 0x7d1) == '\0' || (*(char *)(lVar11 + 0x2e) == '\0')))) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x18),*(undefined4 *)(lVar11 + 0x28));
  }
  lVar11 = FUN_140d25a50();
  if (lVar11 != 0) {
    FUN_141257c40(lVar11,local_res10);
  }
  goto LAB_1411e7a15;
  while (iVar6 = *(int *)(lVar15 + 0x10), iVar6 != -1) {
LAB_1411e7371:
    lVar15 = lVar10 + (longlong)iVar6 * 0x18;
    if (*(longlong *)(lVar10 + (longlong)iVar6 * 0x18) == lVar12) {
      if (iVar6 != -1) goto LAB_1411e7394;
      break;
    }
  }
LAB_1411e7391:
  lVar15 = lVar11;
LAB_1411e7394:
  lVar11 = *(longlong *)(lVar15 + 8);
  local_res8 = 0;
  if (*(longlong *)(lVar11 + 8) != 0) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 8),*(undefined4 *)(lVar11 + 0x20));
  }
  local_res8 = 0;
  if (*(longlong *)(lVar11 + 0x10) != 0) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x10),*(undefined4 *)(lVar11 + 0x24));
  }
  local_res8 = 0;
  if (*(longlong *)(lVar11 + 0x18) != 0) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x18),*(undefined4 *)(lVar11 + 0x28));
  }
  lVar11 = FUN_140d25a50();
  if (lVar11 != 0) {
    FUN_141257c30(lVar11,local_res10);
  }
  goto LAB_1411e7a15;
  while (iVar6 = *(int *)(lVar15 + 0x10), iVar6 != -1) {
LAB_1411e7210:
    lVar15 = lVar10 + (longlong)iVar6 * 0x18;
    if (*(longlong *)(lVar10 + (longlong)iVar6 * 0x18) == lVar12) {
      if (iVar6 != -1) goto LAB_1411e7233;
      break;
    }
  }
LAB_1411e7230:
  lVar15 = lVar11;
LAB_1411e7233:
  lVar11 = *(longlong *)(lVar15 + 8);
  local_res8 = 0;
  if (*(longlong *)(lVar11 + 8) != 0) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 8),*(undefined4 *)(lVar11 + 0x20));
  }
  local_res8 = 0;
  if (*(longlong *)(lVar11 + 0x10) != 0) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x10),*(undefined4 *)(lVar11 + 0x24));
  }
  local_res8 = 0;
  if (*(longlong *)(lVar11 + 0x18) != 0) {
    FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x18),*(undefined4 *)(lVar11 + 0x28));
  }
  lVar11 = FUN_140d25a50();
  if (lVar11 != 0) {
    FUN_141257c20(lVar11,local_res10);
  }
  goto LAB_1411e7a15;
  while (iVar6 = *(int *)(lVar15 + 0x10), iVar6 != -1) {
LAB_1411e7071:
    lVar15 = lVar10 + (longlong)iVar6 * 0x18;
    if (*(longlong *)(lVar10 + (longlong)iVar6 * 0x18) == lVar12) {
      if (iVar6 != -1) goto LAB_1411e7094;
      break;
    }
  }
LAB_1411e7091:
  lVar15 = lVar11;
LAB_1411e7094:
  lVar11 = *(longlong *)(lVar15 + 8);
  local_res8 = 0;
  if (*(longlong *)(lVar11 + 8) != 0) {
    uVar2 = *(undefined4 *)(lVar11 + 0x20);
    lVar12 = FUN_140d28a90();
    if (lVar12 != 0) {
      FUN_141224670(lVar12,lVar11 + 8,uVar2,0);
    }
  }
  local_res8 = 0;
  if (*(longlong *)(lVar11 + 0x10) != 0) {
    uVar2 = *(undefined4 *)(lVar11 + 0x24);
    lVar12 = FUN_140d28a90();
    if (lVar12 != 0) {
      FUN_141224670(lVar12,lVar11 + 0x10,uVar2,0);
    }
  }
  local_res8 = 0;
  if (*(longlong *)(lVar11 + 0x18) != 0) {
    uVar2 = *(undefined4 *)(lVar11 + 0x28);
    lVar12 = FUN_140d28a90();
    if (lVar12 != 0) {
      FUN_141224670(lVar12,(longlong *)(lVar11 + 0x18),uVar2,0);
    }
  }
LAB_1411e7a15:
  lVar11 = FUN_1411fc890();
  if (((*(longlong *)(lVar11 + 0x1ed0) != 0) && (local_res10[1] = 0, local_res10[0] != 0)) &&
     (lVar11 = FUN_141991840(*(longlong *)(lVar11 + 0x1ed0),local_res10), lVar11 != 0)) {
    uVar13 = 0;
    puVar3 = *(undefined4 **)(lVar11 + 0x128);
    iVar6 = *(int *)(lVar11 + 0x130);
    if (puVar3 != puVar3 + (longlong)iVar6 * 0xc) {
      puVar17 = puVar3 + 2;
      do {
        lVar10 = 0;
        local_d8 = *(undefined1 *)(puVar17 + -2);
        local_d4 = puVar17[-1];
        local_d0 = *puVar17;
        local_c8 = *(longlong *)(puVar17 + 2);
        FUN_1407e5680(local_c0,puVar17 + 4);
        local_b0 = *(undefined1 *)(puVar17 + 8);
        local_ac = puVar17[9];
        lVar12 = FUN_140d24f50();
        lVar11 = lVar10;
        if ((lVar12 != 0) && (*(longlong *)(lVar12 + 0x170) != 0)) {
          lVar11 = FUN_1410d40f0(*(longlong *)(lVar12 + 0x170),local_d8,local_d4);
        }
        cVar14 = '\0';
        if ((((byte)(*(char *)(param_1 + 0x7b0) - 2U) < 6) &&
            (local_res10[1] = 0, *(longlong *)(param_1 + 0x548) != 0)) &&
           ((local_res10[2] = 0, *(longlong *)(param_1 + 0x550) != 0 &&
            (lVar12 = FUN_140d2ae30(), lVar12 != 0)))) {
          lVar12 = FUN_1414a44d0(lVar12 + 0xdca50);
          cVar14 = *(char *)(uVar13 + *(longlong *)(lVar12 + 0x78));
        }
        uVar13 = (ulonglong)(byte)((char)uVar13 + 1);
        if (((lVar11 != 0) && (*(char *)(lVar11 + 0x28) != '\0')) || (cVar14 != '\0')) {
          lVar11 = FUN_1411fc890();
          lVar11 = *(longlong *)(lVar11 + 0x1ef0);
          lVar15 = lVar11 + 0x23e0;
          lVar12 = FUN_142058d50(lVar15);
          if (lVar12 == 0) {
            LOCK();
            if (DAT_1456e073c == 0) {
              DAT_1456e073c = 0;
            }
            UNLOCK();
            if ((DAT_1456e073c != *(int *)(lVar11 + 0x23e8)) &&
               (*(longlong *)(lVar11 + 0x23f0) != 0)) {
              lVar12 = FUN_14204ac70();
              FUN_1420575e0(lVar15,lVar12);
              if ((lVar12 != 0) || (DAT_145b9d472 == '\0')) {
                LOCK();
                if (DAT_1456e073c == 0) {
                  DAT_1456e073c = 0;
                }
                UNLOCK();
                *(int *)(lVar11 + 0x23e8) = DAT_1456e073c;
              }
              lVar12 = FUN_142058d50(lVar15);
              if (lVar12 != 0) goto LAB_1411e7bfb;
            }
LAB_1411e7c31:
            lVar12 = lVar10;
          }
          else {
LAB_1411e7bfb:
            lVar11 = FUN_143188280();
            if ((*(int *)(*(longlong *)(lVar12 + 0x10) + 0x90) < *(int *)(lVar11 + 0x90)) ||
               (*(longlong *)
                 (*(longlong *)(*(longlong *)(lVar12 + 0x10) + 0x88) +
                 (longlong)*(int *)(lVar11 + 0x90) * 8) != lVar11 + 0x88)) {
              bVar4 = false;
            }
            else {
              bVar4 = true;
            }
            if (!bVar4) goto LAB_1411e7c31;
          }
          local_e8 = (longlong *)0x0;
          local_e0 = 0;
          iVar5 = 0;
          if (lVar12 != 0) {
            FUN_140d459d0(lVar12,&DAT_143d15414,&local_e8);
            iVar5 = (int)local_e0;
          }
          plVar9 = local_e8;
          local_a8 = local_d8;
          local_a4 = local_d4;
          local_a0 = local_d0;
          local_98 = local_c8;
          FUN_1407e5680(local_90,local_c0);
          for (plVar8 = plVar9; plVar8 != plVar9 + iVar5; plVar8 = plVar8 + 1) {
            if (*(longlong *)(*plVar8 + 8) == local_98) {
              if (local_90[0] != 0) {
                FUN_141de9200();
              }
              goto LAB_1411e7cd8;
            }
          }
          if (local_90[0] != 0) {
            FUN_141de9200();
          }
          plVar8 = (longlong *)0x0;
LAB_1411e7cd8:
          lVar11 = *plVar8;
          if (lVar11 != 0) {
            if (*(longlong *)(lVar11 + 0x10) != 0) {
              FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x10),*(undefined4 *)(lVar11 + 0x18));
              lVar11 = *plVar8;
            }
            if (*(longlong *)(lVar11 + 0x28) != 0) {
              FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x28),*(undefined4 *)(lVar11 + 0x30));
              lVar11 = *plVar8;
            }
            if (*(longlong *)(lVar11 + 0x40) != 0) {
              FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x40),*(undefined4 *)(lVar11 + 0x48));
              lVar11 = *plVar8;
            }
            if (*(longlong *)(lVar11 + 0x58) != 0) {
              FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x58),*(undefined4 *)(lVar11 + 0x60));
              lVar11 = *plVar8;
            }
            if (*(longlong *)(lVar11 + 0x70) != 0) {
              FUN_1411b7ed0(param_1,(longlong *)(lVar11 + 0x70),*(undefined4 *)(lVar11 + 0x78));
            }
          }
          if (plVar9 != (longlong *)0x0) {
            FUN_141de9200(plVar9);
          }
        }
        if (local_c0[0] != 0) {
          FUN_141de9200();
        }
        puVar1 = puVar17 + 10;
        puVar17 = puVar17 + 0xc;
      } while (puVar1 != puVar3 + (longlong)iVar6 * 0xc);
    }
  }
  return;
}



// ---- FUN_1411f46d0 @ 1411f46d0 ----

undefined8 * FUN_1411f46d0(undefined8 *param_1)

{
  undefined8 uVar1;
  
  uVar1 = FUN_142040a20();
  FUN_141a4d0e0(param_1,uVar1);
  *(undefined1 *)(param_1 + 0x48) = 0;
  *param_1 = &PTR_FUN_143d34b70;
  param_1[5] = &PTR_LAB_143d34f50;
  FUN_141eb3490(param_1 + 0x49,L"MoveForward",1);
  FUN_141eb3490(param_1 + 0x4a,L"MoveForward_Up",1);
  FUN_141eb3490(param_1 + 0x4b,L"MoveForward_Down",1);
  FUN_141eb3490(param_1 + 0x4c,L"MoveRight",1);
  FUN_141eb3490(param_1 + 0x4d,L"MoveRight_Left",1);
  FUN_141eb3490(param_1 + 0x4e,L"MoveRight_Right",1);
  FUN_141eb3490(param_1 + 0x4f,L"MoveShift",1);
  FUN_141eb3490(param_1 + 0x50,L"Step",1);
  FUN_141eb3490(param_1 + 0x51,L"PushPad_B",1);
  FUN_141eb3490(param_1 + 0x52,L"PushPad_Y",1);
  FUN_141eb3490(param_1 + 0x53,L"PushPad_X",1);
  FUN_141eb3490(param_1 + 0x54,L"VerticalMove",1);
  FUN_141eb3490(param_1 + 0x55,L"NewFloatRoll",1);
  FUN_141eb3490(param_1 + 0x56,L"NewFieldCursor",1);
  FUN_141eb3490(param_1 + 0x57,L"NewFieldShot",1);
  FUN_141eb3490(param_1 + 0x58,L"NewDash2",1);
  FUN_141eb3490(param_1 + 0x59,L"NewStandBoost2",1);
  FUN_141eb3490(param_1 + 0x5a,L"NewNimbusRoll",1);
  FUN_141eb3490(param_1 + 0x5b,L"NewNimbusDash",1);
  FUN_141eb3490(param_1 + 0x5c,L"v03_Float",1);
  FUN_141eb3490(param_1 + 0x5d,L"v03_Fall",1);
  FUN_141eb3490(param_1 + 0x5e,L"v03_Ride",1);
  FUN_141eb3490(param_1 + 0x5f,L"v03_GetOff",1);
  FUN_141eb3490(param_1 + 0x60,L"v03_RideMenuPrev",1);
  FUN_141eb3490(param_1 + 0x61,L"v03_RideMenuNext",1);
  FUN_141eb3490(param_1 + 0x62,L"v03_Jump",1);
  FUN_141eb3490(param_1 + 99,L"v03_Search",1);
  FUN_141eb3490(param_1 + 100,L"v03_VerticalUp",1);
  FUN_141eb3490(param_1 + 0x65,L"v03_VerticalDown",1);
  FUN_141eb3490(param_1 + 0x66,L"v03_FloatBoost",1);
  FUN_141eb3490(param_1 + 0x67,L"v03_Braking",1);
  FUN_141eb3490(param_1 + 0x68,L"v03_StandBoost",1);
  FUN_141eb3490(param_1 + 0x69,L"v03_StandDash",1);
  FUN_141eb3490(param_1 + 0x6a,L"v03_FieldCursor",1);
  FUN_141eb3490(param_1 + 0x6b,L"v03_FieldShot",1);
  FUN_141eb3490(param_1 + 0x6c,L"v03_ResetCamera",1);
  FUN_141eb3490(param_1 + 0x6d,L"v03_NimbusMove",1);
  FUN_141eb3490(param_1 + 0x6e,L"v03_NimbusBoost",1);
  FUN_141eb3490(param_1 + 0x6f,L"v04_VerticalUp",1);
  FUN_141eb3490(param_1 + 0x70,L"v04_FieldCursor",1);
  FUN_141eb3490(param_1 + 0x71,L"v04_FieldShot",1);
  FUN_141eb3490(param_1 + 0x72,L"v04_Braking",1);
  param_1[0x73] = 0;
  param_1[0x74] = 0;
  param_1[0x75] = 0;
  param_1[0x76] = 0;
  param_1[0x77] = 0;
  param_1[0x78] = 0;
  param_1[0x79] = 0;
  param_1[0x7b] = DAT_145a8af10;
  *(undefined4 *)(param_1 + 0x7c) = DAT_145a8af18;
  *(undefined8 *)((longlong)param_1 + 0x3e4) = DAT_145a8af10;
  *(undefined4 *)((longlong)param_1 + 0x3ec) = DAT_145a8af18;
  param_1[0x7e] = DAT_145a8af10;
  *(undefined4 *)(param_1 + 0x7f) = DAT_145a8af18;
  param_1[0x80] = 0;
  param_1[0x81] = 0;
  param_1[0x82] = 0;
  param_1[0x83] = 0;
  param_1[0x84] = 0;
  param_1[0x8a] = 0;
  param_1[0x8c] = 0;
  param_1[0x92] = 0;
  param_1[0x94] = 0;
  *(undefined1 *)((longlong)param_1 + 0x4a9) = 0;
  *(undefined1 *)(param_1 + 0x9a) = 0;
  *(undefined4 *)((longlong)param_1 + 0x4d4) = 0;
  *(undefined4 *)(param_1 + 0x9c) = 0;
  *(undefined1 *)((longlong)param_1 + 0x4e4) = 0;
  param_1[0x9d] = 0;
  param_1[0x9e] = 0;
  param_1[0x9f] = 0;
  param_1[0xa0] = 2;
  *(undefined1 *)(param_1 + 0x48) = 2;
  *(byte *)((longlong)param_1 + 0x4c) = *(byte *)((longlong)param_1 + 0x4c) & 0xfb | 2;
  return param_1;
}



// ---- FUN_141213b00 @ 141213b00 ----

void FUN_141213b00(longlong *param_1)

{
  longlong lVar1;
  
  if (*(char *)((longlong)param_1 + 0x241) != '\0') {
    *(undefined1 *)((longlong)param_1 + 0x241) = 0;
    lVar1 = param_1[0x74];
    if ((((lVar1 != 0) && (*(char *)(lVar1 + 0x704) == '\0')) && (*(char *)(lVar1 + 0x484) == '\0'))
       && ((*(char *)(lVar1 + 0x4c0) == '\0' && (*(int *)(lVar1 + 0x25c) != 1)))) {
      if (param_1[0x76] != 0) {
        FUN_140fe43e0(param_1[0x76]);
        lVar1 = param_1[0x74];
      }
      FUN_140f61a00(lVar1);
    }
    if (param_1[0x73] != 0) {
      FUN_140f38fa0();
    }
    if ((char)param_1[0x9a] != '\0') {
      FUN_141215410(param_1);
    }
    if ((param_1[0x74] != 0) && (*(char *)(param_1[0x74] + 0x704) == '\0')) {
      param_1[0x7b] = DAT_145a8af10;
      *(undefined4 *)(param_1 + 0x7c) = DAT_145a8af18;
      *(longlong *)((longlong)param_1 + 0x3e4) = DAT_145a8af10;
      *(undefined4 *)((longlong)param_1 + 0x3ec) = DAT_145a8af18;
    }
    *(undefined1 *)(param_1 + 0x9a) = 0;
    lVar1 = FUN_140d2ba50();
    if (lVar1 != 0) {
      lVar1 = FUN_1415453e0(lVar1);
      if (lVar1 != 0) {
        FUN_141543f90(lVar1);
      }
    }
    if (param_1[0x74] != 0) {
      *(undefined1 *)(param_1[0x74] + 0x628) = 0;
    }
    param_1[0x73] = 0;
    param_1[0x74] = 0;
    param_1[0x75] = 0;
    param_1[0x76] = 0;
    param_1[0x77] = 0;
    param_1[0x78] = 0;
    param_1[0x79] = 0;
    *(undefined4 *)(param_1 + 0x81) = 0;
    if (*(int *)((longlong)param_1 + 0x40c) != 0) {
      FUN_140827be0(param_1 + 0x80,0);
    }
    *(undefined4 *)(param_1 + 0x83) = 0;
    if (*(int *)((longlong)param_1 + 0x41c) != 0) {
      FUN_140827be0(param_1 + 0x82,0);
    }
    *(undefined2 *)((longlong)param_1 + 0x4a4) = 0;
    *(undefined1 *)((longlong)param_1 + 0x4a7) = 0;
    *(undefined1 *)((longlong)param_1 + 0x4a9) = 0;
    *(undefined1 *)(param_1 + 0x9a) = 0;
    *(undefined1 *)((longlong)param_1 + 0x4ac) = 0;
    *(undefined4 *)((longlong)param_1 + 0x4d4) = 0;
    *(undefined1 *)((longlong)param_1 + 0x4e4) = 1;
    *(undefined4 *)(param_1 + 0x9c) = 0;
    (**(code **)(*param_1 + 0x328))(param_1,0);
  }
  return;
}



// ---- FUN_141216560 @ 141216560 ----

void FUN_141216560(longlong param_1)

{
  undefined4 uVar1;
  bool bVar2;
  char cVar3;
  int *piVar4;
  int *piVar5;
  ulonglong uVar6;
  ulonglong uVar7;
  float fVar8;
  float fVar9;
  
  piVar4 = *(int **)(param_1 + 0x400);
  piVar5 = piVar4;
  do {
    if (piVar5 == piVar4 + *(int *)(param_1 + 0x408)) {
      bVar2 = false;
LAB_1412165a5:
      uVar6 = 0;
      fVar9 = 0.0;
      uVar7 = (longlong)*(int *)(param_1 + 0x408) & 0x3fffffffffffffff;
      if (piVar4 + *(int *)(param_1 + 0x408) < piVar4) {
        uVar7 = uVar6;
      }
      if (uVar7 != 0) {
        do {
          if (*piVar4 == 1) {
            if (bVar2) {
              fVar9 = fVar9 + DAT_14391fc78;
            }
            else {
              fVar9 = fVar9 + DAT_1450d6c88;
            }
          }
          else if (*piVar4 == 2) {
            fVar8 = DAT_1450d6c88;
            if (bVar2) {
              fVar8 = DAT_14391fc78;
            }
            fVar9 = fVar9 - fVar8;
          }
          piVar4 = piVar4 + 1;
          uVar6 = uVar6 + 1;
        } while (uVar6 != uVar7);
      }
      cVar3 = FUN_14120f780(param_1);
      *(float *)(param_1 + 0x3f0) = fVar9;
      if (cVar3 != '\0') {
        if (*(char *)(param_1 + 0x4e4) == '\0') {
          uVar1 = *(undefined4 *)(param_1 + 0x3d8);
          *(float *)(param_1 + 0x3d8) = fVar9;
          *(undefined4 *)(param_1 + 0x3e4) = uVar1;
          return;
        }
        *(undefined1 *)(param_1 + 0x4e4) = 0;
        return;
      }
      if (*(longlong *)(param_1 + 0x3a0) != 0) {
        *(undefined4 *)(param_1 + 0x3e4) = 0;
        *(undefined4 *)(param_1 + 0x3d8) = 0;
      }
      return;
    }
    if (*piVar5 == 5) {
      bVar2 = true;
      goto LAB_1412165a5;
    }
    piVar5 = piVar5 + 1;
  } while( true );
}



// ---- FUN_141216720 @ 141216720 ----

void FUN_141216720(longlong param_1)

{
  undefined4 uVar1;
  bool bVar2;
  char cVar3;
  int *piVar4;
  int *piVar5;
  ulonglong uVar6;
  ulonglong uVar7;
  float fVar8;
  float fVar9;
  
  piVar4 = *(int **)(param_1 + 0x410);
  piVar5 = piVar4;
  do {
    if (piVar5 == piVar4 + *(int *)(param_1 + 0x418)) {
      bVar2 = false;
LAB_141216765:
      uVar6 = 0;
      fVar9 = 0.0;
      uVar7 = (longlong)*(int *)(param_1 + 0x418) & 0x3fffffffffffffff;
      if (piVar4 + *(int *)(param_1 + 0x418) < piVar4) {
        uVar7 = uVar6;
      }
      if (uVar7 != 0) {
        do {
          if (*piVar4 == 4) {
            if (bVar2) {
              fVar9 = fVar9 + DAT_14391fc78;
            }
            else {
              fVar9 = fVar9 + DAT_1450d6c88;
            }
          }
          else if (*piVar4 == 3) {
            fVar8 = DAT_1450d6c88;
            if (bVar2) {
              fVar8 = DAT_14391fc78;
            }
            fVar9 = fVar9 - fVar8;
          }
          piVar4 = piVar4 + 1;
          uVar6 = uVar6 + 1;
        } while (uVar6 != uVar7);
      }
      cVar3 = FUN_14120f780(param_1);
      *(float *)(param_1 + 0x3f4) = fVar9;
      if (cVar3 != '\0') {
        if (*(char *)(param_1 + 0x4e4) == '\0') {
          uVar1 = *(undefined4 *)(param_1 + 0x3dc);
          *(float *)(param_1 + 0x3dc) = fVar9;
          *(undefined4 *)(param_1 + 1000) = uVar1;
          return;
        }
        *(undefined1 *)(param_1 + 0x4e4) = 0;
        return;
      }
      if (*(longlong *)(param_1 + 0x3a0) != 0) {
        *(undefined4 *)(param_1 + 1000) = 0;
        *(undefined4 *)(param_1 + 0x3dc) = 0;
      }
      return;
    }
    if (*piVar5 == 5) {
      bVar2 = true;
      goto LAB_141216765;
    }
    piVar5 = piVar5 + 1;
  } while( true );
}



// ---- FUN_1412169b0 @ 1412169b0 ----

void FUN_1412169b0(longlong param_1,undefined4 param_2)

{
  undefined4 uVar1;
  char cVar2;
  
  cVar2 = FUN_14120f780();
  *(undefined4 *)(param_1 + 0x3f0) = param_2;
  if (cVar2 == '\0') {
    if (*(longlong *)(param_1 + 0x3a0) != 0) {
      *(undefined4 *)(param_1 + 0x3d8) = 0;
      *(undefined4 *)(param_1 + 0x3e4) = 0;
    }
    return;
  }
  if (*(char *)(param_1 + 0x4e4) == '\0') {
    uVar1 = *(undefined4 *)(param_1 + 0x3d8);
    *(undefined4 *)(param_1 + 0x3d8) = param_2;
    *(undefined4 *)(param_1 + 0x3e4) = uVar1;
    return;
  }
  *(undefined1 *)(param_1 + 0x4e4) = 0;
  return;
}



// ---- FUN_141216a30 @ 141216a30 ----

void FUN_141216a30(longlong param_1,undefined4 param_2)

{
  undefined4 uVar1;
  char cVar2;
  
  cVar2 = FUN_14120f780();
  *(undefined4 *)(param_1 + 0x3f4) = param_2;
  if (cVar2 == '\0') {
    if (*(longlong *)(param_1 + 0x3a0) != 0) {
      *(undefined4 *)(param_1 + 0x3dc) = 0;
      *(undefined4 *)(param_1 + 1000) = 0;
    }
    return;
  }
  if (*(char *)(param_1 + 0x4e4) == '\0') {
    uVar1 = *(undefined4 *)(param_1 + 0x3dc);
    *(undefined4 *)(param_1 + 0x3dc) = param_2;
    *(undefined4 *)(param_1 + 1000) = uVar1;
    return;
  }
  *(undefined1 *)(param_1 + 0x4e4) = 0;
  return;
}



// ---- FUN_1412f96a0 @ 1412f96a0 ----

void FUN_1412f96a0(longlong *param_1)

{
  longlong *plVar1;
  float fVar2;
  undefined1 auVar3 [16];
  bool bVar4;
  longlong lVar5;
  undefined8 uVar6;
  ulonglong uVar7;
  longlong *plVar8;
  ulonglong *puVar9;
  longlong lVar10;
  undefined8 *puVar11;
  undefined8 uVar12;
  int *piVar13;
  int iVar14;
  ulonglong uVar15;
  uint uVar16;
  undefined4 uVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float local_res8 [2];
  ulonglong in_stack_fffffffffffffef8;
  undefined8 local_e8;
  float local_e0;
  undefined8 local_d8;
  float local_d0;
  undefined8 local_c8;
  float fStack_c0;
  float fStack_bc;
  undefined8 local_b8;
  float local_b0;
  
  FUN_1412f8290();
  uVar7 = param_1[0x23];
  uVar15 = 0;
  if (uVar7 == 0) {
LAB_1412f9734:
    uVar7 = uVar15;
  }
  else {
    lVar5 = FUN_141758410();
    if ((*(int *)(*(longlong *)(uVar7 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar7 + 0x10) + 0x88) + (longlong)*(int *)(lVar5 + 0x90) * 8)
        != lVar5 + 0x88)) {
      bVar4 = false;
    }
    else {
      bVar4 = true;
    }
    if (!bVar4) goto LAB_1412f9734;
  }
  FUN_1420575e0(param_1 + 0x99,uVar7);
  lVar5 = FUN_140d22e90(param_1,0);
  param_1[0x9a] = lVar5;
  uVar6 = FUN_142058d50(param_1 + 0x99);
  uVar7 = FUN_140f0bf90(uVar6);
  if (uVar7 != 0) {
    lVar5 = FUN_1417cd340();
    if ((*(int *)(*(longlong *)(uVar7 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(uVar7 + 0x10) + 0x88) + (longlong)*(int *)(lVar5 + 0x90) * 8)
        != lVar5 + 0x88)) {
      bVar4 = false;
    }
    else {
      bVar4 = true;
    }
    if (bVar4) goto LAB_1412f97a8;
  }
  uVar7 = uVar15;
LAB_1412f97a8:
  param_1[0x9b] = uVar7;
  uVar17 = FUN_140dce320(uVar7);
  lVar5 = param_1[0x9b];
  plVar1 = param_1 + 0x95;
  *(undefined4 *)((longlong)param_1 + 0x4f4) = uVar17;
  *(undefined4 *)(param_1 + 0x9f) = *(undefined4 *)(lVar5 + 0x1b0);
  plVar8 = (longlong *)FUN_141df5850(0,0x30);
  uVar7 = uVar15;
  if (plVar8 != (longlong *)0x0) {
    *plVar8 = (longlong)&PTR_FUN_143963528;
    FUN_1420575e0(plVar8 + 1,lVar5);
    plVar8[2] = (longlong)&LAB_140e97170;
    lVar5 = FUN_141db8700();
    plVar8[4] = lVar5;
    *plVar8 = (longlong)&PTR_FUN_143963578;
    FUN_1407e7830(plVar1,1);
    puVar9 = (ulonglong *)(**(code **)(*plVar8 + 0x28))(plVar8,local_res8);
    iVar14 = (int)param_1[0x96];
    uVar7 = *puVar9;
    *(int *)(param_1 + 0x96) = iVar14 + 1;
    if (*(int *)((longlong)param_1 + 0x4b4) < iVar14 + 1) {
      FUN_1407f1790(plVar1,iVar14);
    }
    lVar5 = *plVar1;
    *(longlong **)(lVar5 + (longlong)iVar14 * 0x10) = plVar8;
    *(undefined4 *)(lVar5 + 8 + (longlong)iVar14 * 0x10) = 3;
  }
  lVar5 = param_1[0x2d];
  param_1[0x98] = uVar7;
  if (lVar5 == 0) {
    local_d8 = DAT_145a8af10;
    local_d0 = DAT_145a8af18;
  }
  else {
    local_d8 = *(undefined8 *)(lVar5 + 0x1a0);
    local_d0 = *(float *)(lVar5 + 0x1a8);
  }
  fVar18 = (float)local_d8;
  fVar23 = (float)((ulonglong)local_d8 >> 0x20);
  local_c8 = local_d8;
  local_res8[0] = local_d0;
  uVar6 = FUN_142058d50(param_1 + 0x99);
  lVar5 = FUN_140f3e8b0(uVar6);
  if (lVar5 != 0) {
    lVar10 = FUN_141758410();
    if ((*(int *)(*(longlong *)(lVar5 + 0x10) + 0x90) < *(int *)(lVar10 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar5 + 0x10) + 0x88) + (longlong)*(int *)(lVar10 + 0x90) * 8)
        != lVar10 + 0x88)) {
      bVar4 = false;
    }
    else {
      bVar4 = true;
    }
    if (bVar4) {
      lVar5 = *(longlong *)(lVar5 + 0x168);
      if (lVar5 == 0) {
        local_d8 = DAT_145a8af10;
        local_d0 = DAT_145a8af18;
      }
      else {
        local_d8 = *(undefined8 *)(lVar5 + 0x1a0);
        local_d0 = *(float *)(lVar5 + 0x1a8);
      }
      local_e0 = local_d0;
      local_e8 = local_d8;
      fVar18 = *(float *)(*(longlong *)(param_1[0x9b] + 0x170) + -4 +
                         (longlong)(int)param_1[0x9f] * 4);
      lVar5 = FUN_142058d50(param_1 + 0x99);
      if (*(longlong *)(lVar5 + 0x168) == 0) {
        local_d0 = (float)DAT_1456d9d40;
        puVar11 = &local_d8;
        local_d8 = DAT_1456d9d38;
      }
      else {
        puVar11 = (undefined8 *)FUN_142c42ce0(*(longlong *)(lVar5 + 0x168),&local_b8);
      }
      local_d8 = *puVar11;
      local_d0 = *(float *)(puVar11 + 1);
      local_e8._4_4_ = local_e8._4_4_ + (float)((ulonglong)local_d8 >> 0x20) * fVar18;
      local_e0 = local_e0 + local_d0 * fVar18;
      local_e8._0_4_ = (float)local_e8 + (float)local_d8 * fVar18;
      FUN_142b247d0(param_1,&local_e8,0,0,in_stack_fffffffffffffef8 & 0xffffffffffffff00);
      fVar18 = (float)local_e8;
      fVar23 = local_e8._4_4_;
    }
  }
  lVar5 = param_1[0x2d];
  iVar14 = (int)(*(float *)(param_1[0x87] + -4 + (longlong)(int)param_1[0x9f] * 4) /
                *(float *)(param_1 + 0x86));
  *(int *)((longlong)param_1 + 0x4e4) = iVar14;
  if (lVar5 == 0) {
    local_c8 = DAT_145a8af10;
    fStack_c0 = DAT_145a8af18;
  }
  else {
    local_c8 = *(undefined8 *)(lVar5 + 0x1a0);
    fStack_c0 = *(float *)(lVar5 + 0x1a8);
  }
  local_e0 = fStack_c0;
  local_e8 = local_c8;
  fVar19 = (float)(iVar14 + -1) * *(float *)(param_1 + 0x86) * DAT_14391b46c;
  if (lVar5 == 0) {
    puVar11 = &local_c8;
    local_c8 = DAT_1456d9d48;
    fStack_c0 = (float)DAT_1456d9d50;
    lVar5 = 0;
  }
  else {
    puVar11 = (undefined8 *)FUN_142c6ec90(lVar5,&local_b8);
    lVar5 = param_1[0x2d];
  }
  local_c8 = *puVar11;
  fStack_c0 = *(float *)(puVar11 + 1);
  fVar21 = (float)local_e8 - (float)local_c8 * fVar19;
  fVar20 = local_e0 - fStack_c0 * fVar19;
  fVar22 = local_e8._4_4_ - (float)((ulonglong)local_c8 >> 0x20) * fVar19;
  local_e0 = fVar20;
  if (lVar5 == 0) {
    local_c8 = DAT_1456d9d48;
    puVar11 = &local_c8;
    fStack_c0 = (float)DAT_1456d9d50;
  }
  else {
    puVar11 = (undefined8 *)FUN_142c6ec90(fStack_c0 * fVar19,&local_b8);
  }
  uVar6 = *puVar11;
  fVar19 = *(float *)(param_1 + 0x86);
  fVar2 = *(float *)(puVar11 + 1);
  uVar7 = uVar15;
  if (0 < *(int *)((longlong)param_1 + 0x4e4)) {
    do {
      local_d0 = DAT_145a8af28;
      local_d8 = DAT_145a8af20;
      local_b0 = local_e0;
      local_c8 = 0x3f8000003f800000;
      fStack_c0 = 1.0;
      local_b8 = CONCAT44(fVar22,fVar21);
      uVar12 = (**(code **)(*param_1 + 0x148))(param_1);
      lVar5 = FUN_142d2f9c0(uVar12,param_1[0x84],&local_b8,&local_d8,&local_c8,1,0);
      if (lVar5 != 0) {
        iVar14 = (int)param_1[0xa1];
        *(int *)(param_1 + 0xa1) = iVar14 + 1;
        if (*(int *)((longlong)param_1 + 0x50c) < iVar14 + 1) {
          FUN_1407f1510(param_1 + 0xa0,iVar14);
        }
        *(longlong *)(param_1[0xa0] + (longlong)iVar14 * 8) = lVar5;
      }
      fVar20 = fVar20 + fVar2 * fVar19;
      uVar16 = (int)uVar7 + 1;
      fVar21 = fVar21 + (float)uVar6 * fVar19;
      fVar22 = fVar22 + (float)((ulonglong)uVar6 >> 0x20) * fVar19;
      uVar7 = (ulonglong)uVar16;
      local_e0 = fVar20;
    } while ((int)uVar16 < *(int *)((longlong)param_1 + 0x4e4));
  }
  fStack_c0 = local_res8[0];
  local_c8 = CONCAT44(fVar23,fVar18);
  lVar5 = FUN_140d24c90();
  if (lVar5 != 0) {
    uVar17 = FUN_14108a0e0(lVar5,param_1 + 0x7a,&local_c8,&local_c8);
    *(undefined4 *)(param_1 + 0x83) = uVar17;
    FUN_141088ac0(*(undefined8 *)(lVar5 + 0x100),param_1 + 0x83,0);
    local_e8 = 0x3f8000003f800000;
    local_e0 = 1.0;
    if (1 < (int)param_1[0x9f]) {
      local_e0 = ((float *)param_1[0x87])[(longlong)(int)param_1[0x9f] + -1] /
                 *(float *)param_1[0x87];
    }
    local_b8 = 0x3f8000003f800000;
    local_b0 = local_e0;
    uVar17 = FUN_1410889a0(*(undefined8 *)(lVar5 + 0x100),param_1 + 0x83,&local_b8);
    lVar10 = param_1[0x2d];
    if (lVar10 == 0) {
      uVar17 = (undefined4)DAT_145a8af20;
      fVar18 = (float)((ulonglong)DAT_145a8af20 >> 0x20);
      local_b0 = DAT_145a8af28;
    }
    else {
      fVar18 = *(float *)(lVar10 + 400);
      local_c8 = *(undefined8 *)(lVar10 + 400);
      fStack_c0 = *(float *)(lVar10 + 0x198);
      fStack_bc = *(float *)(lVar10 + 0x19c);
      auVar3._4_4_ = -(uint)(*(float *)(lVar10 + 0x1d4) != *(float *)(lVar10 + 0x194));
      auVar3._0_4_ = -(uint)(*(float *)(lVar10 + 0x1d0) != fVar18);
      auVar3._8_4_ = -(uint)(*(float *)(lVar10 + 0x1d8) != fStack_c0);
      auVar3._12_4_ = -(uint)(*(float *)(lVar10 + 0x1dc) != fStack_bc);
      iVar14 = movmskps(uVar17,auVar3);
      if (iVar14 != 0) {
        *(float *)(lVar10 + 0x1d0) = fVar18;
        *(float *)(lVar10 + 0x1d4) = *(float *)(lVar10 + 0x194);
        *(float *)(lVar10 + 0x1d8) = fStack_c0;
        *(float *)(lVar10 + 0x1dc) = fStack_bc;
        puVar11 = (undefined8 *)FUN_141e4e960(&local_c8,&local_b8);
        *(undefined8 *)(lVar10 + 0x1e0) = *puVar11;
        *(undefined4 *)(lVar10 + 0x1e8) = *(undefined4 *)(puVar11 + 1);
      }
      uVar17 = (undefined4)*(undefined8 *)(lVar10 + 0x1e0);
      fVar18 = (float)((ulonglong)*(undefined8 *)(lVar10 + 0x1e0) >> 0x20);
      local_b0 = *(float *)(lVar10 + 0x1e8);
    }
    local_b8 = CONCAT44(fVar18 + DAT_14391b47c,uVar17);
    FUN_141088910(*(undefined8 *)(lVar5 + 0x100),param_1 + 0x83,&local_b8);
  }
  param_1[0x8e] = 0;
  if (0 < (int)param_1[0x8b]) {
    piVar13 = (int *)param_1[0x8c];
    do {
      if (*piVar13 == 1) {
        param_1[0x8e] = (longlong)(int)uVar15 * 0x20 + param_1[0x8c];
        break;
      }
      uVar16 = (int)uVar15 + 1;
      uVar15 = (ulonglong)uVar16;
      piVar13 = piVar13 + 8;
    } while ((int)uVar16 < (int)param_1[0x8b]);
  }
  FUN_140c58980(param_1 + 0x8a,param_1,0,param_1 + 0x8b,param_1 + 0x93);
  return;
}



// ---- FUN_14130fca0 @ 14130fca0 ----

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14130fca0(longlong param_1,float param_2)

{
  longlong lVar1;
  uint uVar2;
  int iVar3;
  undefined8 uVar4;
  undefined8 *puVar5;
  int *piVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  undefined1 auStack_98 [32];
  longlong local_78;
  undefined8 local_68;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined1 local_48 [16];
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  ulonglong local_28;
  
  local_28 = DAT_145909780 ^ (ulonglong)auStack_98;
  param_2 = *(float *)(param_1 + 0x4e0) - param_2;
  *(float *)(param_1 + 0x4e0) = param_2;
  if (param_2 <= 0.0) {
    lVar1 = *(longlong *)(param_1 + 0x168);
    uStack_54 = 0;
    local_48._12_4_ = 0;
    local_48._0_12_ = *(undefined1 (*) [12])(param_1 + 0x4e8);
    if (lVar1 == 0) {
      local_58 = 0;
      uStack_60 = 0;
      uStack_5c = DAT_1450d6c88;
    }
    else {
      local_58 = *(undefined4 *)(lVar1 + 400);
      uStack_54 = *(undefined4 *)(lVar1 + 0x194);
      uStack_60 = *(undefined4 *)(lVar1 + 0x198);
      uStack_5c = *(undefined4 *)(lVar1 + 0x19c);
    }
    local_68 = CONCAT44(uStack_54,local_58);
    local_38 = _DAT_143973ef0;
    uStack_34 = _UNK_143973ef4;
    uStack_30 = _UNK_143973ef8;
    uStack_2c = _UNK_143973efc;
    uStack_50 = uStack_60;
    uStack_4c = uStack_5c;
    uVar4 = FUN_142058d50(param_1 + 0x4c8);
    FUN_140d3a360(*(undefined8 *)(param_1 + 0x448),uVar4,param_1,&local_58);
    *(int *)(param_1 + 0x4e4) = *(int *)(param_1 + 0x4e4) + -1;
    if (*(int *)(param_1 + 0x4e4) < 1) {
      iVar3 = 0;
      *(undefined8 *)(param_1 + 0x470) = 0;
      if (0 < *(int *)(param_1 + 0x458)) {
        piVar6 = *(int **)(param_1 + 0x460);
        do {
          if (*piVar6 == 3) {
            *(int **)(param_1 + 0x470) = *(int **)(param_1 + 0x460) + (longlong)iVar3 * 8;
            break;
          }
          iVar3 = iVar3 + 1;
          piVar6 = piVar6 + 8;
        } while (iVar3 < *(int *)(param_1 + 0x458));
      }
      local_78 = param_1 + 0x498;
      FUN_140c58980(param_1 + 0x450,param_1,0,param_1 + 0x458);
    }
    else {
      if (*(longlong *)(param_1 + 0x168) == 0) {
        uStack_60 = DAT_1456d9d50;
        puVar5 = &local_68;
        local_68 = DAT_1456d9d48;
      }
      else {
        puVar5 = (undefined8 *)FUN_142c6ec90(*(longlong *)(param_1 + 0x168),&local_68);
      }
      fVar9 = *(float *)(param_1 + 0x430);
      fVar7 = (float)((ulonglong)*puVar5 >> 0x20) * fVar9;
      fVar8 = (float)*puVar5 * fVar9;
      fVar9 = *(float *)(puVar5 + 1) * fVar9;
      uVar2 = *(uint *)(param_1 + 0x4f8) & 0x80000001;
      if ((int)uVar2 < 0) {
        uVar2 = (uVar2 - 1 | 0xfffffffe) + 1;
      }
      if (uVar2 == 1) {
        fVar8 = (float)((uint)fVar8 ^ DAT_14391b660);
        fVar7 = (float)((uint)fVar7 ^ DAT_14391b660);
        fVar9 = (float)((uint)fVar9 ^ DAT_14391b660);
      }
      *(float *)(param_1 + 0x4e8) = fVar8 + *(float *)(param_1 + 0x4e8);
      *(float *)(param_1 + 0x4ec) = fVar7 + *(float *)(param_1 + 0x4ec);
      *(float *)(param_1 + 0x4f0) = fVar9 + *(float *)(param_1 + 0x4f0);
      *(undefined4 *)(param_1 + 0x4e0) = *(undefined4 *)(param_1 + 0x42c);
    }
  }
  return;
}



// ---- FUN_1413105b0 @ 1413105b0 ----

void FUN_1413105b0(longlong param_1)

{
  int iVar1;
  bool bVar2;
  int iVar3;
  undefined8 uVar4;
  longlong lVar5;
  longlong lVar6;
  undefined8 *puVar7;
  int *piVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  undefined8 local_38;
  undefined4 local_30;
  undefined8 local_28;
  float local_20;
  
  if ((*(longlong *)(param_1 + 0x4d8) == 0) ||
     (fVar9 = (float)FUN_140dce320(),
     *(float *)(param_1 + 0x4f4) + *(float *)(param_1 + 0x428) <= fVar9)) {
    lVar5 = *(longlong *)(param_1 + 0x168);
    if (lVar5 == 0) {
      local_38 = DAT_145a8af10;
      local_30 = DAT_145a8af18;
    }
    else {
      local_38 = *(undefined8 *)(lVar5 + 0x1a0);
      local_30 = *(undefined4 *)(lVar5 + 0x1a8);
    }
    *(undefined8 *)(param_1 + 0x4e8) = local_38;
    *(undefined4 *)(param_1 + 0x4f0) = local_30;
    uVar4 = FUN_142058d50(param_1 + 0x4c8);
    lVar5 = FUN_140f3e8b0(uVar4);
    if (lVar5 != 0) {
      lVar6 = FUN_141758410();
      if ((*(int *)(*(longlong *)(lVar5 + 0x10) + 0x90) < *(int *)(lVar6 + 0x90)) ||
         (*(longlong *)
           (*(longlong *)(*(longlong *)(lVar5 + 0x10) + 0x88) + (longlong)*(int *)(lVar6 + 0x90) * 8
           ) != lVar6 + 0x88)) {
        bVar2 = false;
      }
      else {
        bVar2 = true;
      }
      if (bVar2) {
        lVar5 = *(longlong *)(lVar5 + 0x168);
        if (lVar5 == 0) {
          local_38 = DAT_145a8af10;
          local_30 = DAT_145a8af18;
        }
        else {
          local_38 = *(undefined8 *)(lVar5 + 0x1a0);
          local_30 = *(undefined4 *)(lVar5 + 0x1a8);
        }
        local_28 = local_38;
        local_20 = (float)local_30;
        *(undefined4 *)(param_1 + 0x4f0) = local_30;
      }
    }
    fVar9 = (float)(*(int *)(param_1 + 0x4e4) + -1) * *(float *)(param_1 + 0x430) * DAT_14391b46c;
    if (*(longlong *)(param_1 + 0x168) == 0) {
      local_20 = (float)DAT_1456d9d50;
      puVar7 = &local_28;
      local_28 = DAT_1456d9d48;
    }
    else {
      puVar7 = (undefined8 *)FUN_142c6ec90(*(longlong *)(param_1 + 0x168),&local_28);
    }
    local_20 = *(float *)(puVar7 + 1);
    fVar10 = (float)((ulonglong)*puVar7 >> 0x20) * fVar9;
    fVar11 = (float)*puVar7 * fVar9;
    fVar9 = local_20 * fVar9;
    if ((*(byte *)(param_1 + 0x4f8) & 1) == 0) {
      fVar11 = (float)((uint)fVar11 ^ DAT_14391b660);
      fVar10 = (float)((uint)fVar10 ^ DAT_14391b660);
      fVar9 = (float)((uint)fVar9 ^ DAT_14391b660);
    }
    iVar3 = 0;
    *(float *)(param_1 + 0x4e8) = fVar11 + *(float *)(param_1 + 0x4e8);
    *(float *)(param_1 + 0x4ec) = fVar10 + *(float *)(param_1 + 0x4ec);
    *(float *)(param_1 + 0x4f0) = fVar9 + *(float *)(param_1 + 0x4f0);
    *(undefined4 *)(param_1 + 0x4e0) = 0;
    *(undefined1 *)(param_1 + 0x4fc) = 0;
    iVar1 = *(int *)(param_1 + 0x458);
    *(undefined8 *)(param_1 + 0x470) = 0;
    if (0 < iVar1) {
      piVar8 = *(int **)(param_1 + 0x460);
      do {
        if (*piVar8 == 2) {
          *(longlong *)(param_1 + 0x470) = (longlong)iVar3 * 0x20 + *(longlong *)(param_1 + 0x460);
          break;
        }
        iVar3 = iVar3 + 1;
        piVar8 = piVar8 + 8;
      } while (iVar3 < iVar1);
    }
    FUN_140c58980(param_1 + 0x450,param_1,0,(int *)(param_1 + 0x458),param_1 + 0x498);
  }
  return;
}



// ---- FUN_1413120d0 @ 1413120d0 ----

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1413120d0(longlong *param_1,float param_2)

{
  longlong lVar1;
  float fVar2;
  undefined8 uVar3;
  undefined4 extraout_EAX;
  int iVar4;
  undefined8 *puVar5;
  float *pfVar6;
  float fVar7;
  float fVar8;
  undefined1 auVar9 [16];
  undefined1 auVar10 [16];
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  undefined8 local_78;
  float local_70;
  undefined1 local_68 [8];
  float local_60;
  float local_58;
  float fStack_54;
  float fStack_50;
  float fStack_4c;
  
  fVar7 = *(float *)((longlong)param_1 + 0x41c) - param_2;
  *(float *)((longlong)param_1 + 0x41c) = fVar7;
  if (fVar7 <= 0.0) {
    (**(code **)(*param_1 + 0x628))();
  }
  fVar8 = powf(DAT_14391fc7c,param_2 * DAT_14391fca4);
  lVar1 = param_1[0x2d];
  fVar8 = DAT_1450d6c88 - fVar8;
  fVar7 = DAT_145a8af28;
  uVar3 = DAT_145a8af20;
  if (lVar1 != 0) {
    local_58 = *(float *)(lVar1 + 400);
    fStack_54 = *(float *)(lVar1 + 0x194);
    fStack_50 = *(float *)(lVar1 + 0x198);
    fStack_4c = *(float *)(lVar1 + 0x19c);
    auVar10._4_4_ = -(uint)(*(float *)(lVar1 + 0x1d4) != fStack_54);
    auVar10._0_4_ = -(uint)(*(float *)(lVar1 + 0x1d0) != local_58);
    auVar10._8_4_ = -(uint)(*(float *)(lVar1 + 0x1d8) != fStack_50);
    auVar10._12_4_ = -(uint)(*(float *)(lVar1 + 0x1dc) != fStack_4c);
    iVar4 = movmskps(extraout_EAX,auVar10);
    if (iVar4 != 0) {
      *(float *)(lVar1 + 0x1d0) = local_58;
      *(float *)(lVar1 + 0x1d4) = fStack_54;
      *(float *)(lVar1 + 0x1d8) = fStack_50;
      *(float *)(lVar1 + 0x1dc) = fStack_4c;
      puVar5 = (undefined8 *)FUN_141e4e960(&local_58,local_68);
      *(undefined8 *)(lVar1 + 0x1e0) = *puVar5;
      *(undefined4 *)(lVar1 + 0x1e8) = *(undefined4 *)(puVar5 + 1);
    }
    fVar7 = *(float *)(lVar1 + 0x1e8);
    uVar3 = *(undefined8 *)(lVar1 + 0x1e0);
  }
  fVar12 = *(float *)(param_1 + 0x9c) - (float)uVar3;
  local_78._4_4_ = (float)((ulonglong)uVar3 >> 0x20);
  fVar11 = *(float *)((longlong)param_1 + 0x4e4) - local_78._4_4_;
  fVar14 = (float)DAT_145967e00;
  fVar2 = DAT_145967e00._4_4_;
  fVar15 = DAT_145967e00._8_4_;
  auVar9._4_4_ = fVar11;
  auVar9._0_4_ = fVar12;
  auVar9._8_4_ = *(float *)(param_1 + 0x9d) - fVar7;
  auVar9._12_4_ = 0;
  auVar10 = divps(auVar9,_DAT_145967e00);
  fVar12 = fVar12 - (float)(int)auVar10._0_4_ * fVar14;
  fVar11 = fVar11 - (float)(int)auVar10._4_4_ * fVar2;
  fVar13 = (*(float *)(param_1 + 0x9d) - fVar7) - (float)(int)auVar10._8_4_ * fVar15;
  fVar12 = (float)(((uint)(fVar12 + fVar14) ^ (uint)fVar12) & -(uint)(0.0 <= fVar12) ^
                  (uint)(fVar12 + fVar14));
  fVar11 = (float)(((uint)(fVar11 + fVar2) ^ (uint)fVar11) & -(uint)(0.0 <= fVar11) ^
                  (uint)(fVar11 + fVar2));
  fVar13 = (float)(((uint)(fVar13 + fVar15) ^ (uint)fVar13) & -(uint)(0.0 <= fVar13) ^
                  (uint)(fVar13 + fVar15));
  fVar7 = (float)(((uint)(fVar13 - fVar15) ^ (uint)fVar13) & -(uint)(fRam0000000145967df8 < fVar13)
                 ^ (uint)fVar13) * fVar8 + fVar7;
  fVar15 = (float)uVar3 +
           (float)(((uint)(fVar12 - fVar14) ^ (uint)fVar12) & -(uint)(_DAT_145967df0 < fVar12) ^
                  (uint)fVar12) * fVar8;
  fVar14 = local_78._4_4_ +
           (float)(((uint)(fVar11 - fVar2) ^ (uint)fVar11) & -(uint)(fRam0000000145967df4 < fVar11)
                  ^ (uint)fVar11) * fVar8;
  fVar8 = *(float *)(param_1[0x69] + 0x144);
  local_78 = CONCAT44(fVar14,fVar15);
  local_70 = fVar7;
  local_60 = fVar7;
  pfVar6 = (float *)FUN_142dbcb20(&local_58,&local_78);
  lVar1 = param_1[0x69];
  local_78 = CONCAT44(fVar14,fVar15);
  local_60 = fVar8 * pfVar6[2];
  *(ulonglong *)(lVar1 + 0x114) = CONCAT44(fVar8 * pfVar6[1],fVar8 * *pfVar6);
  *(float *)(lVar1 + 0x11c) = local_60;
  local_70 = fVar7;
  FUN_142b251b0(param_1,&local_78,0);
  local_70 = *(float *)((longlong)param_1 + 0x41c);
  local_78 = CONCAT44(local_70,local_70);
  local_60 = local_70;
  FUN_142c7e810(param_1[0x82],&local_78);
  return;
}



// ---- FUN_1413197f0 @ 1413197f0 ----

void FUN_1413197f0(longlong *param_1,float param_2)

{
  code *pcVar1;
  bool bVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong lVar5;
  longlong local_28;
  undefined4 local_20;
  
  lVar3 = FUN_142058d50(param_1 + 0x8a);
  if (lVar3 == 0) {
    (**(code **)(*param_1 + 0x628))(param_1);
    FUN_142b28560(param_1,param_2);
    *(float *)(param_1 + 0x6c) = param_2 + *(float *)(param_1 + 0x6c);
    return;
  }
  uVar4 = FUN_142058d50(param_1 + 0x8a);
  lVar3 = FUN_140f3e8b0(uVar4);
  if (lVar3 != 0) {
    lVar5 = FUN_141758410();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar5 + 0x90) * 8)
        != lVar5 + 0x88)) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    if (bVar2) {
      *(longlong *)((longlong)param_1 + 0x484) = param_1[0x8f];
      *(int *)((longlong)param_1 + 0x48c) = (int)param_1[0x90];
      param_1[0x92] = param_1[0x8f];
      *(int *)(param_1 + 0x93) = (int)param_1[0x90];
      *(longlong *)((longlong)param_1 + 0x49c) = param_1[0x8f];
      *(int *)((longlong)param_1 + 0x4a4) = (int)param_1[0x90];
      param_1[0x95] = param_1[0x8f];
      *(int *)(param_1 + 0x96) = (int)param_1[0x90];
      *(longlong *)((longlong)param_1 + 0x4b4) = param_1[0x8f];
      *(int *)((longlong)param_1 + 0x4bc) = (int)param_1[0x90];
      param_1[0x98] = param_1[0x8f];
      *(int *)(param_1 + 0x99) = (int)param_1[0x90];
      *(longlong *)((longlong)param_1 + 0x4cc) = param_1[0x8f];
      *(int *)((longlong)param_1 + 0x4d4) = (int)param_1[0x90];
      param_1[0x9b] = param_1[0x8f];
      *(int *)(param_1 + 0x9c) = (int)param_1[0x90];
      *(longlong *)((longlong)param_1 + 0x4e4) = param_1[0x8f];
      *(int *)((longlong)param_1 + 0x4ec) = (int)param_1[0x90];
      lVar3 = *(longlong *)(lVar3 + 0x168);
      if (lVar3 == 0) {
        local_28 = DAT_145a8af10;
        local_20 = DAT_145a8af18;
      }
      else {
        local_28 = *(longlong *)(lVar3 + 0x1a0);
        local_20 = *(undefined4 *)(lVar3 + 0x1a8);
      }
      param_1[0x8f] = local_28;
      *(undefined4 *)(param_1 + 0x90) = local_20;
    }
  }
  FUN_142b28560(param_1,param_2);
  *(float *)(param_1 + 0x6c) = param_2 + *(float *)(param_1 + 0x6c);
  FUN_140c58980(param_1 + 0x7f,param_1,param_2,param_1 + 0x80,param_1 + 0x88);
  if ((param_1[0x82] != 0) && (pcVar1 = *(code **)(param_1[0x82] + 0x10), pcVar1 != (code *)0x0)) {
    (*pcVar1)(param_1,param_2);
  }
  *(float *)((longlong)param_1 + 0x424) = param_2 + *(float *)((longlong)param_1 + 0x424);
  return;
}



// ---- FUN_141339f90 @ 141339f90 ----

void FUN_141339f90(longlong param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined4 uVar2;
  
  *(undefined1 *)(param_1 + 0x5c8) = *(undefined1 *)((longlong)param_2 + 0x24);
  *(undefined4 *)(param_1 + 0x5cc) = *(undefined4 *)(param_2 + 4);
  *(undefined4 *)(param_1 + 0x5d0) = *(undefined4 *)((longlong)param_2 + 0x1c);
  *(undefined1 *)(param_1 + 0x5d4) = *(undefined1 *)((longlong)param_2 + 0x25);
  *(undefined4 *)(param_1 + 0x4f8) = *(undefined4 *)(param_2 + 2);
  *(undefined1 *)(param_1 + 0x520) = *(undefined1 *)((longlong)param_2 + 0x26);
  *(undefined1 *)(param_1 + 0x521) = *(undefined1 *)((longlong)param_2 + 0x2c);
  *(undefined4 *)(param_1 + 0x524) = *(undefined4 *)(param_2 + 6);
  *(undefined1 *)(param_1 + 0x528) = *(undefined1 *)((longlong)param_2 + 0x34);
  *(undefined4 *)(param_1 + 0x5a0) = *(undefined4 *)(param_2 + 7);
  *(undefined4 *)(param_1 + 0x5a4) = *(undefined4 *)((longlong)param_2 + 0x3c);
  *(undefined4 *)(param_1 + 0x4e4) = *(undefined4 *)(param_2 + 8);
  *(undefined1 *)(param_1 + 0x4ea) = *(undefined1 *)((longlong)param_2 + 0x44);
  *(undefined4 *)(param_1 + 0x4e0) = *(undefined4 *)(param_1 + 0x364);
  *(undefined4 *)(param_1 + 0x4e4) = *(undefined4 *)(param_2 + 8);
  *(undefined1 *)(param_1 + 0x4e8) = *(undefined1 *)(param_1 + 0x350);
  *(undefined8 *)(param_1 + 0x4f0) = *(undefined8 *)(param_1 + 0x358);
  uVar2 = FUN_141303bd0();
  *(undefined4 *)(param_1 + 0x5a8) = uVar2;
  FUN_1420575e0(param_1 + 0x438,param_1);
  uVar1 = FUN_140d26ef0(param_1);
  FUN_1420575e0(param_1 + 0x440,uVar1);
  *(undefined8 *)(param_1 + 0x4d0) = *param_2;
  *(undefined8 *)(param_1 + 0x4d8) = param_2[1];
  return;
}



// ---- FUN_141359980 @ 141359980 ----

void FUN_141359980(longlong param_1)

{
  longlong *plVar1;
  bool bVar2;
  char cVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined8 local_58;
  undefined4 local_50;
  undefined8 local_48;
  undefined4 local_40;
  undefined8 local_38;
  undefined4 local_30;
  undefined8 local_28;
  undefined4 local_20;
  undefined4 local_10;
  
  lVar4 = *(longlong *)(param_1 + 0x168);
  if (lVar4 == 0) {
    local_48 = DAT_145a8af10;
    local_40 = DAT_145a8af18;
  }
  else {
    local_48 = *(undefined8 *)(lVar4 + 0x1a0);
    local_40 = *(undefined4 *)(lVar4 + 0x1a8);
  }
  local_58 = local_48;
  local_50 = local_40;
  if (*(longlong *)(param_1 + 0x3d0) != 0) {
    local_40 = *(undefined4 *)(param_1 + 0x468);
    local_48 = CONCAT44(local_40,local_40);
    local_30 = DAT_145a8af28;
    local_38 = DAT_145a8af20;
    local_20 = *(undefined4 *)(param_1 + 0x4e4);
    local_28 = CONCAT44(*(undefined4 *)(param_1 + 0x4e0),*(uint *)(param_1 + 0x4e8) ^ DAT_14391b660)
    ;
    local_10 = local_20;
    lVar4 = FUN_142d2fc60(*(longlong *)(param_1 + 0x3d0),lVar4,0,&local_28,&local_38,&local_48,0,1,0
                         );
    *(longlong *)(param_1 + 0x4c8) = lVar4;
    if ((*(char *)(param_1 + 0x3d8) == -1) || (lVar4 == 0)) {
      cVar3 = FUN_140d2f950();
      if ((cVar3 == '\0') ||
         ((*(char *)(param_1 + 0x3d9) == '6' || (lVar4 = *(longlong *)(param_1 + 0x4c8), lVar4 == 0)
          ))) goto LAB_141359af6;
    }
    local_58 = *(undefined8 *)(lVar4 + 0x1a0);
    local_50 = (undefined4)*(undefined8 *)(lVar4 + 0x1a8);
    local_10 = local_50;
  }
LAB_141359af6:
  if (*(int *)(param_1 + 1000) != 0) {
    FUN_1412eb6a0(param_1,*(undefined4 *)(param_1 + 0x3e0));
  }
  uVar5 = FUN_1412eb400(param_1,param_1 + 0x370);
  *(undefined8 *)(param_1 + 0x4d0) = uVar5;
  if (*(char *)(param_1 + 0x3d8) == -1) {
    cVar3 = FUN_140d2f950();
    if (cVar3 != '\0') {
      FUN_141485c60(*(undefined1 *)(param_1 + 0x3d9),&local_58,0,0);
    }
  }
  else {
    FUN_141485d20(*(char *)(param_1 + 0x3d8),&local_58,0,0);
  }
  plVar1 = *(longlong **)(param_1 + 0x118);
  if (plVar1 != (longlong *)0x0) {
    lVar4 = FUN_1418dd1f0();
    if ((*(int *)(plVar1[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar1[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    if (bVar2) {
      (**(code **)(*plVar1 + 0x628))(plVar1);
    }
  }
  return;
}



// ---- FUN_1414bc680 @ 1414bc680 ----

undefined8 * FUN_1414bc680(undefined8 *param_1)

{
  FUN_141a4f560();
  *(undefined4 *)(param_1 + 0x73) = 0x41000000;
  param_1[0x74] = 0;
  *param_1 = &PTR_FUN_143e64718;
  param_1[0x20] = &PTR_LAB_143e64bf0;
  param_1[0x75] = 0;
  param_1[0x76] = 0;
  param_1[0x77] = 0;
  param_1[0x78] = 0;
  param_1[0x79] = 0;
  param_1[0x7a] = 0;
  param_1[0x7b] = 0;
  param_1[0x7c] = 0;
  param_1[0x7d] = 0;
  param_1[0x7e] = 0;
  param_1[0x7f] = 0;
  param_1[0x80] = 0;
  param_1[0x81] = 0;
  param_1[0x82] = 0;
  param_1[0x84] = 0;
  param_1[0x85] = 0;
  param_1[0x86] = 0;
  param_1[0x87] = 0;
  param_1[0x8c] = 0;
  param_1[0x8d] = 0;
  param_1[0x8e] = 0;
  param_1[0x8f] = 0;
  *(undefined4 *)(param_1 + 0x90) = 0;
  *(undefined4 *)((longlong)param_1 + 0x484) = 0x10001;
  param_1[0x91] = 0;
  param_1[0x92] = 0;
  param_1[0x9a] = 0;
  *(undefined2 *)(param_1 + 0x9b) = 0x101;
  *(undefined4 *)((longlong)param_1 + 0x4dc) = 0;
  *(undefined1 *)(param_1 + 0x9c) = 0;
  *(undefined4 *)((longlong)param_1 + 0x4e4) = 0;
  *(undefined1 *)(param_1 + 0x9d) = 0;
  *(undefined8 *)((longlong)param_1 + 0x4ec) = 0;
  *(undefined8 *)((longlong)param_1 + 0x4f4) = 0;
  param_1[0xa0] = 0;
  param_1[0xa1] = 0;
  *(undefined4 *)(param_1 + 0xa2) = 0xffffffff;
  *(undefined1 *)((longlong)param_1 + 0x514) = 0;
  param_1[0xa3] = 0;
  *(undefined1 *)(param_1 + 0xa4) = 0;
  *(undefined4 *)((longlong)param_1 + 0x524) = 0;
  *(undefined1 *)(param_1 + 0xa5) = 0;
  param_1[0xa6] = 0;
  param_1[0xa7] = 0;
  *(undefined1 *)(param_1 + 0xa8) = 0;
  *(undefined4 *)(param_1 + 0xa9) = 0xffffffff;
  param_1[0xac] = 0;
  param_1[0xad] = 0;
  *(undefined4 *)(param_1 + 0xa9) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x54c) = 0;
  param_1[0xab] = 0;
  *(undefined4 *)(param_1 + 0xad) = 0;
  if (0x7fffffff < *(uint *)((longlong)param_1 + 0x56c)) {
    FUN_1407f1af0(param_1 + 0xac,0);
  }
  if ((undefined2 *)param_1[0xac] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0xac] = 0;
  }
  *(undefined8 *)((longlong)param_1 + 0x54c) = 0;
  *(undefined4 *)(param_1 + 0xa9) = 0xffffffff;
  param_1[0xab] = 0;
  *(undefined4 *)(param_1 + 0xad) = 0;
  if ((*(int *)((longlong)param_1 + 0x56c) < 0) && (*(int *)((longlong)param_1 + 0x56c) != 0)) {
    FUN_1407f1af0(param_1 + 0xac,0);
  }
  if ((undefined2 *)param_1[0xac] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0xac] = 0;
  }
  *(undefined4 *)(param_1 + 0xaa) = 0;
  *(undefined4 *)(param_1 + 0xae) = 0xffffffff;
  param_1[0xb1] = 0;
  param_1[0xb2] = 0;
  *(undefined4 *)(param_1 + 0xae) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x574) = 0;
  param_1[0xb0] = 0;
  *(undefined4 *)(param_1 + 0xb2) = 0;
  if ((*(int *)((longlong)param_1 + 0x594) < 0) && (*(int *)((longlong)param_1 + 0x594) != 0)) {
    FUN_1407f1af0(param_1 + 0xb1,0);
  }
  if ((undefined2 *)param_1[0xb1] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0xb1] = 0;
  }
  *(undefined8 *)((longlong)param_1 + 0x574) = 0;
  *(undefined4 *)(param_1 + 0xae) = 0xffffffff;
  param_1[0xb0] = 0;
  *(undefined4 *)(param_1 + 0xb2) = 0;
  if ((*(int *)((longlong)param_1 + 0x594) < 0) && (*(int *)((longlong)param_1 + 0x594) != 0)) {
    FUN_1407f1af0(param_1 + 0xb1,0);
  }
  if ((undefined2 *)param_1[0xb1] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0xb1] = 0;
  }
  *(undefined4 *)(param_1 + 0xaf) = 0;
  *(undefined4 *)(param_1 + 0xb3) = 0xffffffff;
  param_1[0xb6] = 0;
  param_1[0xb7] = 0;
  *(undefined4 *)(param_1 + 0xb3) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x59c) = 0;
  param_1[0xb5] = 0;
  *(undefined4 *)(param_1 + 0xb7) = 0;
  if ((*(int *)((longlong)param_1 + 0x5bc) < 0) && (*(int *)((longlong)param_1 + 0x5bc) != 0)) {
    FUN_1407f1af0(param_1 + 0xb6,0);
  }
  if ((undefined2 *)param_1[0xb6] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0xb6] = 0;
  }
  *(undefined8 *)((longlong)param_1 + 0x59c) = 0;
  *(undefined4 *)(param_1 + 0xb3) = 0xffffffff;
  param_1[0xb5] = 0;
  *(undefined4 *)(param_1 + 0xb7) = 0;
  if ((*(int *)((longlong)param_1 + 0x5bc) < 0) && (*(int *)((longlong)param_1 + 0x5bc) != 0)) {
    FUN_1407f1af0(param_1 + 0xb6,0);
  }
  if ((undefined2 *)param_1[0xb6] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0xb6] = 0;
  }
  *(undefined4 *)(param_1 + 0xb4) = 0;
  *(undefined4 *)(param_1 + 0xb8) = 0xffffffff;
  param_1[0xbb] = 0;
  param_1[0xbc] = 0;
  *(undefined4 *)(param_1 + 0xb8) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x5c4) = 0;
  param_1[0xba] = 0;
  *(undefined4 *)(param_1 + 0xbc) = 0;
  if ((*(int *)((longlong)param_1 + 0x5e4) < 0) && (*(int *)((longlong)param_1 + 0x5e4) != 0)) {
    FUN_1407f1af0(param_1 + 0xbb,0);
  }
  if ((undefined2 *)param_1[0xbb] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0xbb] = 0;
  }
  *(undefined8 *)((longlong)param_1 + 0x5c4) = 0;
  *(undefined4 *)(param_1 + 0xb8) = 0xffffffff;
  param_1[0xba] = 0;
  *(undefined4 *)(param_1 + 0xbc) = 0;
  if ((*(int *)((longlong)param_1 + 0x5e4) < 0) && (*(int *)((longlong)param_1 + 0x5e4) != 0)) {
    FUN_1407f1af0(param_1 + 0xbb,0);
  }
  if ((undefined2 *)param_1[0xbb] != (undefined2 *)0x0) {
    *(undefined2 *)param_1[0xbb] = 0;
  }
  *(undefined4 *)(param_1 + 0xb9) = 0;
  return param_1;
}



// ---- FUN_1414c4db0 @ 1414c4db0 ----

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1414c4db0(longlong *param_1)

{
  bool bVar1;
  undefined4 uVar2;
  longlong lVar3;
  longlong lVar4;
  undefined2 *puVar5;
  longlong lVar6;
  undefined1 local_res8 [8];
  undefined2 *local_28;
  undefined8 local_20;
  undefined2 *local_18;
  undefined8 local_10;
  
  if (*(char *)((longlong)param_1 + 0x485) == '\x01') {
    return;
  }
  FUN_141eb3410(local_res8,"All_Loading",1);
  lVar3 = FUN_1429af1b0(param_1,local_res8);
  lVar6 = 0;
  if (lVar3 == 0) {
LAB_1414c4e39:
    lVar3 = lVar6;
  }
  else {
    lVar4 = FUN_1429f2a50();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
        != lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1414c4e39;
  }
  param_1[0x74] = lVar3;
  FUN_141eb3410(local_res8,"Txt_Title",1);
  lVar3 = FUN_1429af1b0(param_1,local_res8);
  if (lVar3 == 0) {
LAB_1414c4e9f:
    lVar3 = lVar6;
  }
  else {
    lVar4 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
        != lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1414c4e9f;
  }
  param_1[0x76] = lVar3;
  FUN_141eb3410(local_res8,"Txt_Detail",1);
  lVar3 = FUN_1429af1b0(param_1,local_res8);
  if (lVar3 == 0) {
LAB_1414c4f05:
    lVar3 = lVar6;
  }
  else {
    lVar4 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
        != lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1414c4f05;
  }
  param_1[0x77] = lVar3;
  FUN_141eb3410(local_res8,"Loading_Bar",1);
  lVar3 = FUN_1429af1b0(param_1,local_res8);
  if (lVar3 == 0) {
LAB_1414c4f6b:
    lVar3 = lVar6;
  }
  else {
    lVar4 = FUN_14176faf0();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
        != lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1414c4f6b;
  }
  param_1[0x78] = lVar3;
  FUN_141eb3410(local_res8,"Dmy_Img",1);
  lVar3 = FUN_1429af1b0(param_1,local_res8);
  if (lVar3 == 0) {
LAB_1414c4fd1:
    lVar3 = lVar6;
  }
  else {
    lVar4 = FUN_1429f6b30();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
        != lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1414c4fd1;
  }
  param_1[0x79] = lVar3;
  FUN_141eb3410(local_res8,"Pnl_Txt_Tips",1);
  lVar3 = FUN_1429af1b0(param_1,local_res8);
  if (lVar3 == 0) {
LAB_1414c5037:
    lVar3 = lVar6;
  }
  else {
    lVar4 = FUN_1429f30c0();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
        != lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1414c5037;
  }
  param_1[0x7a] = lVar3;
  FUN_141eb3410(local_res8,"Txt_Tips_Title",1);
  lVar3 = FUN_1429af1b0(param_1,local_res8);
  if (lVar3 == 0) {
LAB_1414c509d:
    lVar3 = lVar6;
  }
  else {
    lVar4 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
        != lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1414c509d;
  }
  param_1[0x7b] = lVar3;
  FUN_141eb3410(local_res8,"Txt_Tips_Detail",1);
  lVar3 = FUN_1429af1b0(param_1,local_res8);
  if (lVar3 != 0) {
    lVar4 = FUN_141aa08b0();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
        != lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) goto LAB_1414c5106;
  }
  lVar3 = lVar6;
LAB_1414c5106:
  param_1[0x7c] = lVar3;
  FUN_141eb3410(local_res8,"Change_Tips",1);
  lVar3 = FUN_141a65110(param_1,local_res8);
  param_1[0x7d] = lVar3;
  local_18 = (undefined2 *)0x0;
  local_10 = 0x1f;
  FUN_1407e2ee0(&local_18,0);
  FUN_141dd1c10(local_18,L"/Game/Parameter/ChapterUIParam",0x3e);
  puVar5 = &DAT_14391f7d0;
  if ((int)local_10 != 0) {
    puVar5 = local_18;
  }
  uVar2 = FUN_1407e51a0(puVar5);
  *(undefined4 *)((longlong)param_1 + 0x4dc) = uVar2;
  FUN_141a6eae0(param_1,&local_18,2);
  *(undefined1 *)(param_1 + 0x9c) = 0;
  local_28 = (undefined2 *)0x0;
  local_20 = 0x21;
  FUN_1407e2ee0(&local_28,0);
  FUN_141dd1c10(local_28,L"/Game/Parameter/LoadingTipsTable",0x42);
  puVar5 = &DAT_14391f7d0;
  if ((int)local_20 != 0) {
    puVar5 = local_28;
  }
  uVar2 = FUN_1407e51a0(puVar5);
  *(undefined4 *)((longlong)param_1 + 0x4e4) = uVar2;
  FUN_141a6eae0(param_1,&local_28,2);
  *(undefined1 *)(param_1 + 0x9d) = 0;
  FUN_1414cafb0(param_1);
  FUN_141eb3410(local_res8,&DAT_143e58a4c,1);
  lVar3 = FUN_141a65110(param_1,local_res8);
  param_1[0x8a] = lVar3;
  FUN_141eb3410(local_res8,"Bg_In",1);
  lVar3 = FUN_141a65110(param_1,local_res8);
  param_1[0x88] = lVar3;
  FUN_141eb3410(local_res8,"Bg_Out",1);
  lVar3 = FUN_141a65110(param_1,local_res8);
  param_1[0x89] = lVar3;
  uVar2 = DAT_143a86300;
  if ((_DAT_145a33618 < 0xf) && ((0x5881U >> (_DAT_145a33618 & 0x1f) & 1) != 0)) {
    uVar2 = DAT_143c6efe4;
  }
  FUN_141a830a0(param_1[0x77],uVar2);
  (**(code **)(*param_1 + 0x250))(param_1,1);
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0xf7;
  *(undefined1 *)((longlong)param_1 + 0x485) = 1;
  if (local_28 != (undefined2 *)0x0) {
    FUN_141de9200();
  }
  if (local_18 != (undefined2 *)0x0) {
    FUN_141de9200();
  }
  return;
}



// ---- FUN_1414c6640 @ 1414c6640 ----

void FUN_1414c6640(longlong param_1,longlong *param_2)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  undefined8 uVar4;
  bool bVar5;
  longlong lVar6;
  int iVar7;
  longlong lVar8;
  undefined2 *puVar9;
  longlong lVar10;
  undefined8 *puVar11;
  
  puVar2 = (undefined8 *)*param_2;
  lVar6 = param_2[1];
  if (puVar2 != puVar2 + (longlong)(int)lVar6 * 0xc) {
    puVar11 = puVar2 + 1;
    do {
      if (*(int *)(puVar11 + 5) == 0) {
        puVar9 = &DAT_14391f7d0;
      }
      else {
        puVar9 = (undefined2 *)puVar11[4];
      }
      iVar7 = FUN_1407e51a0(puVar9);
      if (iVar7 == *(int *)(param_1 + 0x480)) {
        lVar10 = puVar11[-1];
        if (lVar10 == 0) {
LAB_1414c66d5:
          lVar10 = 0;
        }
        else {
          lVar8 = FUN_14327bf50();
          if ((*(int *)(*(longlong *)(lVar10 + 0x10) + 0x90) < *(int *)(lVar8 + 0x90)) ||
             (*(longlong *)
               (*(longlong *)(*(longlong *)(lVar10 + 0x10) + 0x88) +
               (longlong)*(int *)(lVar8 + 0x90) * 8) != lVar8 + 0x88)) {
            bVar5 = false;
          }
          else {
            bVar5 = true;
          }
          if (!bVar5) goto LAB_1414c66d5;
        }
        plVar3 = *(longlong **)(param_1 + 0x3c8);
        *(longlong *)(param_1 + 0x458) = lVar10;
        if ((plVar3 != (longlong *)0x0) && (lVar10 != 0)) {
          (**(code **)(*plVar3 + 0x2b0))(plVar3,lVar10,0);
          *(undefined4 *)(param_1 + 0x490) = 2;
        }
      }
      else if (iVar7 == *(int *)(param_1 + 0x4dc)) {
        *(undefined8 *)(param_1 + 0x3f0) = *puVar11;
        *(undefined1 *)(param_1 + 0x4e0) = 1;
      }
      else if (iVar7 == *(int *)(param_1 + 0x4ec)) {
        uVar4 = *puVar11;
        *(uint *)(param_1 + 0x494) = *(uint *)(param_1 + 0x494) | 1;
        *(undefined8 *)(param_1 + 0x3f8) = uVar4;
      }
      else if (iVar7 == *(int *)(param_1 + 0x4f0)) {
        uVar4 = *puVar11;
        *(uint *)(param_1 + 0x494) = *(uint *)(param_1 + 0x494) | 2;
        *(undefined8 *)(param_1 + 0x400) = uVar4;
      }
      else if (iVar7 == *(int *)(param_1 + 0x4f4)) {
        uVar4 = *puVar11;
        *(uint *)(param_1 + 0x494) = *(uint *)(param_1 + 0x494) | 4;
        *(undefined8 *)(param_1 + 0x408) = uVar4;
      }
      else if (iVar7 == *(int *)(param_1 + 0x4f8)) {
        uVar4 = *puVar11;
        *(uint *)(param_1 + 0x494) = *(uint *)(param_1 + 0x494) | 8;
        *(undefined8 *)(param_1 + 0x410) = uVar4;
      }
      else if (iVar7 == *(int *)(param_1 + 0x4fc)) {
        uVar4 = *puVar11;
        *(uint *)(param_1 + 0x494) = *(uint *)(param_1 + 0x494) | 0x10;
        *(undefined8 *)(param_1 + 0x418) = uVar4;
      }
      else if (iVar7 == *(int *)(param_1 + 0x4e4)) {
        *(undefined8 *)(param_1 + 0x420) = *puVar11;
        *(undefined1 *)(param_1 + 0x4e8) = 1;
      }
      else if (iVar7 == *(int *)(param_1 + 0x51c)) {
        *(undefined8 *)(param_1 + 0x428) = *puVar11;
        *(undefined1 *)(param_1 + 0x520) = 1;
      }
      else if (iVar7 == *(int *)(param_1 + 0x524)) {
        *(undefined8 *)(param_1 + 0x430) = *puVar11;
        *(undefined1 *)(param_1 + 0x528) = 1;
      }
      else {
        FUN_141a8e600(param_1,puVar11 + 4);
      }
      puVar1 = puVar11 + 0xb;
      puVar11 = puVar11 + 0xc;
    } while (puVar1 != puVar2 + (longlong)(int)lVar6 * 0xc);
  }
  return;
}



// ---- FUN_1414f21d0 @ 1414f21d0 ----

void FUN_1414f21d0(longlong param_1,byte param_2)

{
  float fVar1;
  float fVar2;
  bool bVar3;
  int iVar4;
  byte bVar5;
  int iVar6;
  undefined4 uVar7;
  undefined8 uVar8;
  undefined8 *puVar9;
  float *pfVar10;
  longlong lVar11;
  longlong lVar12;
  longlong lVar13;
  int iVar14;
  longlong lVar15;
  longlong lVar16;
  undefined8 uVar17;
  undefined4 uVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  undefined8 local_res8;
  byte local_res10;
  int local_res18;
  longlong local_res20;
  undefined1 local_e8 [8];
  undefined1 local_e0 [8];
  undefined1 local_d8 [8];
  undefined1 local_d0 [144];
  
  local_res10 = param_2;
  iVar6 = FUN_140ad2d70(*(undefined8 *)(param_1 + 0x600));
  lVar15 = (longlong)iVar6;
  uVar8 = FUN_140d25b00();
  uVar8 = FUN_141991f50(uVar8,param_2);
  FUN_1414f7e80(param_1,&local_res8,param_2);
  iVar6 = FUN_140b686a0(uVar8);
  fVar26 = (float)iVar6 * (float)local_res8;
  iVar6 = FUN_141992af0(uVar8);
  fVar25 = (float)iVar6 * local_res8._4_4_;
  iVar6 = FUN_140819140(uVar8);
  fVar20 = (float)iVar6 * (float)local_res8;
  iVar6 = FUN_141992b00(uVar8);
  fVar26 = fVar26 - fVar20;
  fVar25 = fVar25 - (float)iVar6 * local_res8._4_4_;
  uVar7 = FUN_140819140(*(undefined8 *)(param_1 + 0x600));
  local_res8 = CONCAT44(local_res8._4_4_,uVar7);
  local_res18 = FUN_141992b00(*(undefined8 *)(param_1 + 0x600));
  bVar5 = local_res10;
  fVar20 = DAT_14391b46c;
  lVar16 = 0;
  *(undefined4 *)(param_1 + 0x628) = 0;
  fVar21 = DAT_143e70c20;
  iVar6 = 1;
  if (0 < lVar15) {
    iVar4 = (int)(float)local_res8;
    local_res20 = lVar15;
    do {
      FUN_14298e390(*(undefined8 *)(param_1 + 0x528),
                    *(undefined8 *)(*(longlong *)(param_1 + 0x548) + lVar16));
      uVar8 = *(undefined8 *)
               (*(longlong *)(param_1 + 0x548) + (longlong)*(int *)(param_1 + 0x628) * 8);
      if (iVar6 == 1) {
        FUN_141502e80(uVar8,1,param_1 + 0x418);
        FUN_141a83cf0(*(undefined8 *)
                       (*(longlong *)
                         (*(longlong *)(param_1 + 0x548) + (longlong)*(int *)(param_1 + 0x628) * 8)
                       + 0x500),bVar5 - 1);
      }
      else {
        FUN_141502e80(uVar8,0,param_1 + 0x410);
      }
      puVar9 = (undefined8 *)FUN_1419936d0(*(undefined8 *)(param_1 + 0x600),&local_res8,iVar6);
      uVar8 = *puVar9;
      pfVar10 = (float *)FUN_1414f7e80(param_1,local_e8,bVar5);
      fVar19 = *pfVar10;
      lVar11 = FUN_1414f7e80(param_1,local_e0,bVar5);
      iVar14 = (int)uVar8 - local_res18;
      fVar22 = *(float *)(lVar11 + 4);
      pfVar10 = (float *)FUN_1419938b0(*(undefined8 *)(param_1 + 0x600),local_d8,iVar6);
      fVar24 = *pfVar10;
      fVar1 = pfVar10[1];
      lVar11 = *(longlong *)
                (*(longlong *)(param_1 + 0x548) + (longlong)*(int *)(param_1 + 0x628) * 8);
      lVar13 = *(longlong *)(lVar11 + 0x28);
      if (lVar13 != 0) {
        lVar12 = FUN_1429f33e0();
        if ((*(int *)(*(longlong *)(lVar13 + 0x10) + 0x90) < *(int *)(lVar12 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(lVar13 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar12 + 0x90) * 8) != lVar12 + 0x88)) {
          bVar3 = false;
        }
        else {
          bVar3 = true;
        }
        if (bVar3) {
          FUN_1429e4040(lVar13,100);
          pfVar10 = (float *)FUN_1429ad310(lVar13,local_d0);
          fVar2 = *pfVar10;
          *(float *)(lVar11 + 0x554) =
               (fVar1 * fVar21 + (float)iVar14 * fVar22) - pfVar10[1] * fVar20;
          *(float *)(lVar11 + 0x550) =
               (fVar24 * fVar21 + (float)((int)((ulonglong)uVar8 >> 0x20) - iVar4) * fVar19) -
               fVar2 * fVar20;
          FUN_1429e1d30(lVar11,*(undefined8 *)(lVar11 + 0x550));
        }
      }
      iVar14 = *(int *)(param_1 + 0x628);
      lVar11 = *(longlong *)(*(longlong *)(param_1 + 0x548) + (longlong)iVar14 * 8);
      lVar13 = *(longlong *)(lVar11 + 0x408);
      if (lVar13 != 0) {
        FUN_1416fa8c0(lVar13,CONCAT71((int7)(int3)((uint)iVar14 >> 8),1));
        uVar8 = FUN_1416eb8d0(*(undefined8 *)(lVar11 + 0x408));
        FUN_14157aae0(uVar8,0);
        iVar14 = *(int *)(param_1 + 0x628);
      }
      iVar6 = iVar6 + 1;
      lVar16 = lVar16 + 8;
      *(int *)(param_1 + 0x628) = iVar14 + 1;
      local_res20 = local_res20 + -1;
    } while (local_res20 != 0);
    local_res20 = 0;
  }
  iVar6 = 1;
  lVar11 = 0;
  fVar21 = *(float *)(param_1 + 0x638);
  fVar19 = *(float *)(param_1 + 0x63c);
  lVar16 = *(longlong *)(*(longlong *)(param_1 + 0x528) + 0x28);
  if (lVar16 == 0) {
LAB_1414f259c:
    lVar16 = lVar11;
  }
  else {
    lVar13 = FUN_1429f33e0();
    if ((*(int *)(*(longlong *)(lVar16 + 0x10) + 0x90) < *(int *)(lVar13 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar16 + 0x10) + 0x88) + (longlong)*(int *)(lVar13 + 0x90) * 8
         ) != lVar13 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (!bVar3) goto LAB_1414f259c;
  }
  bVar5 = local_res10;
  fVar24 = fVar21 * fVar26;
  fVar22 = fVar25 * fVar19;
  if (((fVar24 <= *(float *)(param_1 + 0x7dc)) &&
      (fVar22 < *(float *)(param_1 + 0x7e0) || fVar22 == *(float *)(param_1 + 0x7e0))) ||
     (local_res10 != 3)) {
    *(undefined8 *)(param_1 + 0x640) = *(undefined8 *)(param_1 + 0x638);
    *(undefined1 *)(param_1 + 0x7ed) = 0;
  }
  else {
    *(undefined1 *)(param_1 + 0x7ed) = 1;
    fVar21 = *(float *)(param_1 + 0x7e0) / fVar25;
    fVar19 = *(float *)(param_1 + 0x7dc) / fVar26;
    if (fVar21 < fVar19) {
      fVar19 = fVar21;
    }
    *(float *)(param_1 + 0x640) = fVar19;
    fVar22 = fVar25 * fVar19;
    fVar24 = fVar26 * fVar19;
    *(float *)(param_1 + 0x644) = fVar19;
    fVar21 = fVar19;
  }
  if (0 < lVar15) {
    do {
      lVar13 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x548) + lVar11) + 0x28);
      if (lVar13 != 0) {
        lVar12 = FUN_1429f33e0();
        if ((*(int *)(*(longlong *)(lVar13 + 0x10) + 0x90) < *(int *)(lVar12 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(lVar13 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar12 + 0x90) * 8) != lVar12 + 0x88)) {
          bVar3 = false;
        }
        else {
          bVar3 = true;
        }
        if (bVar3) {
          lVar12 = FUN_1419937b0(*(undefined8 *)(param_1 + 0x600),iVar6);
          FUN_1429e4040(lVar13,*(int *)(lVar12 + 0x44) + 100);
        }
      }
      iVar6 = iVar6 + 1;
      lVar11 = lVar11 + 8;
      lVar15 = lVar15 + -1;
    } while (lVar15 != 0);
  }
  fVar1 = *(float *)(param_1 + 0x7dc);
  fVar2 = *(float *)(param_1 + 0x7e0);
  FUN_1429e2640(lVar16,CONCAT44(fVar25,fVar26));
  fVar23 = DAT_143d68184 - *(float *)(param_1 + 0x7dc);
  local_res8 = CONCAT44(DAT_145a8af0c,DAT_145a8af08);
  iVar6 = bVar5 - 1;
  switch(iVar6) {
  case 0:
    uVar7 = *(undefined4 *)(param_1 + 0x500);
    uVar18 = *(undefined4 *)(param_1 + 0x504);
    break;
  case 1:
    uVar7 = *(undefined4 *)(param_1 + 0x4d0);
    uVar18 = *(undefined4 *)(param_1 + 0x4d4);
    break;
  case 2:
    uVar7 = *(undefined4 *)(param_1 + 0x4d8);
    uVar18 = *(undefined4 *)(param_1 + 0x4dc);
    break;
  case 3:
    uVar7 = *(undefined4 *)(param_1 + 0x4e0);
    uVar18 = *(undefined4 *)(param_1 + 0x4e4);
    break;
  case 4:
    uVar7 = *(undefined4 *)(param_1 + 0x4e8);
    uVar18 = *(undefined4 *)(param_1 + 0x4ec);
    break;
  case 5:
    uVar7 = *(undefined4 *)(param_1 + 0x4f0);
    uVar18 = *(undefined4 *)(param_1 + 0x4f4);
    break;
  case 6:
    uVar7 = *(undefined4 *)(param_1 + 0x4f8);
    uVar18 = *(undefined4 *)(param_1 + 0x4fc);
    break;
  default:
    goto switchD_1414f274c_default;
  }
  local_res8 = CONCAT44(uVar18,uVar7);
switchD_1414f274c_default:
  FUN_1429e1d30(*(undefined8 *)(param_1 + 0x528),local_res8);
  FUN_1429e1be0(*(undefined8 *)(param_1 + 0x528),CONCAT44(fVar19,fVar21));
  switch(iVar6) {
  case 0:
    uVar8 = *(undefined8 *)(param_1 + 0x440);
    uVar17 = *(undefined8 *)(param_1 + 0x448);
    break;
  case 1:
    uVar8 = *(undefined8 *)(param_1 + 0x450);
    uVar17 = *(undefined8 *)(param_1 + 0x458);
    break;
  case 2:
    uVar8 = *(undefined8 *)(param_1 + 0x460);
    uVar17 = *(undefined8 *)(param_1 + 0x468);
    break;
  case 3:
    uVar8 = *(undefined8 *)(param_1 + 0x470);
    uVar17 = *(undefined8 *)(param_1 + 0x478);
    break;
  case 4:
    uVar8 = *(undefined8 *)(param_1 + 0x480);
    uVar17 = *(undefined8 *)(param_1 + 0x488);
    break;
  case 5:
    uVar8 = *(undefined8 *)(param_1 + 0x490);
    uVar17 = *(undefined8 *)(param_1 + 0x498);
    break;
  case 6:
    uVar8 = *(undefined8 *)(param_1 + 0x4a0);
    uVar17 = *(undefined8 *)(param_1 + 0x4a8);
    break;
  default:
    goto switchD_1414f280e_default;
  }
  *(undefined8 *)(param_1 + 0x938) = uVar8;
  *(undefined8 *)(param_1 + 0x940) = uVar17;
switchD_1414f280e_default:
  lVar15 = *(longlong *)(param_1 + 0x568);
  *(float *)(param_1 + 0x7a8) = fVar23 + (fVar1 - fVar24) * fVar20;
  *(float *)(param_1 + 0x7ac) = (fVar2 - fVar22) * fVar20;
  *(float *)(param_1 + 0x7a0) = fVar26;
  *(float *)(param_1 + 0x7a4) = fVar25;
  *(float *)(param_1 + 0x7b0) = fVar21;
  *(float *)(param_1 + 0x7b4) = fVar19;
  fVar20 = DAT_145a8af08;
  fVar21 = DAT_145a8af0c;
  switch(iVar6) {
  case 0:
    fVar20 = *(float *)(param_1 + 0x500);
    fVar21 = *(float *)(param_1 + 0x504);
    break;
  case 1:
    fVar20 = *(float *)(param_1 + 0x4d0);
    fVar21 = *(float *)(param_1 + 0x4d4);
    break;
  case 2:
    fVar20 = *(float *)(param_1 + 0x4d8);
    fVar21 = *(float *)(param_1 + 0x4dc);
    break;
  case 3:
    fVar20 = *(float *)(param_1 + 0x4e0);
    fVar21 = *(float *)(param_1 + 0x4e4);
    break;
  case 4:
    fVar20 = *(float *)(param_1 + 0x4e8);
    fVar21 = *(float *)(param_1 + 0x4ec);
    break;
  case 5:
    fVar20 = *(float *)(param_1 + 0x4f0);
    fVar21 = *(float *)(param_1 + 0x4f4);
    break;
  case 6:
    fVar20 = *(float *)(param_1 + 0x4f8);
    fVar21 = *(float *)(param_1 + 0x4fc);
  }
  fVar26 = DAT_145a8af0c;
  fVar25 = DAT_145a8af08;
  switch(*(undefined1 *)(param_1 + 0x626)) {
  case 1:
    fVar25 = *(float *)(param_1 + 0x500);
    fVar26 = *(float *)(param_1 + 0x504);
    break;
  case 2:
    fVar25 = *(float *)(param_1 + 0x4d0);
    fVar26 = *(float *)(param_1 + 0x4d4);
    break;
  case 3:
    fVar25 = *(float *)(param_1 + 0x4d8);
    fVar26 = *(float *)(param_1 + 0x4dc);
    break;
  case 4:
    fVar25 = *(float *)(param_1 + 0x4e0);
    fVar26 = *(float *)(param_1 + 0x4e4);
    break;
  case 5:
    fVar25 = *(float *)(param_1 + 0x4e8);
    fVar26 = *(float *)(param_1 + 0x4ec);
    break;
  case 6:
    fVar25 = *(float *)(param_1 + 0x4f0);
    fVar26 = *(float *)(param_1 + 0x4f4);
    break;
  case 7:
    fVar25 = *(float *)(param_1 + 0x4f8);
    fVar26 = *(float *)(param_1 + 0x4fc);
  }
  FUN_1429e1d30(lVar15,CONCAT44((fVar26 - fVar21) + *(float *)(lVar15 + 0x94),
                                (fVar25 - fVar20) + *(float *)(lVar15 + 0x90)));
  return;
}



// ---- FUN_1415390e0 @ 1415390e0 ----

undefined8 * FUN_1415390e0(undefined8 *param_1)

{
  FUN_141702500();
  *param_1 = &PTR_FUN_143e91a58;
  param_1[0x20] = &PTR_LAB_143e91f40;
  param_1[0x7b] = 0;
  *(undefined4 *)(param_1 + 0x7c) = 0;
  param_1[0x7d] = 0;
  param_1[0x7e] = 0;
  param_1[0x7f] = 0;
  param_1[0x80] = 0;
  *(undefined4 *)(param_1 + 0x81) = 0x420c0000;
  *(undefined4 *)((longlong)param_1 + 0x40c) = 0x42c80000;
  *(undefined4 *)(param_1 + 0x82) = 0x42c80000;
  *(undefined4 *)((longlong)param_1 + 0x414) = 0x42c80000;
  *(undefined4 *)(param_1 + 0x83) = 0x42c80000;
  *(undefined4 *)((longlong)param_1 + 0x41c) = 0x3f4ccccd;
  *(undefined4 *)(param_1 + 0x84) = 0x3f4ccccd;
  *(undefined4 *)((longlong)param_1 + 0x424) = 0x3ecccccd;
  *(undefined4 *)(param_1 + 0x85) = 0x3ecccccd;
  *(undefined4 *)((longlong)param_1 + 0x42c) = 0x3f800000;
  *(undefined4 *)(param_1 + 0x86) = 0x3f800000;
  *(undefined4 *)((longlong)param_1 + 0x434) = 0x40000000;
  *(undefined4 *)(param_1 + 0x87) = 0x40000000;
  param_1[0x8c] = 0;
  *(undefined4 *)((longlong)param_1 + 0x43c) = 0xc2480000;
  *(undefined4 *)(param_1 + 0x88) = 0xc1d00000;
  *(undefined4 *)((longlong)param_1 + 0x444) = 0xc2480000;
  *(undefined4 *)(param_1 + 0x89) = 0xc1d00000;
  param_1[0x8d] = 0;
  param_1[0x90] = 0;
  param_1[0x91] = 0;
  *(undefined4 *)(param_1 + 0x92) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x494) = 0;
  param_1[0x94] = 0;
  *(undefined4 *)(param_1 + 0x95) = 0;
  param_1[0x96] = 0;
  param_1[0x97] = 0;
  param_1[0x9a] = 0;
  param_1[0x9b] = 0;
  *(undefined4 *)(param_1 + 0x9c) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x4e4) = 0;
  param_1[0x9e] = 0;
  *(undefined4 *)(param_1 + 0x9f) = 0;
  param_1[0xa0] = 0;
  param_1[0xa1] = 0;
  param_1[0xa2] = 0;
  param_1[0xa3] = 0;
  param_1[0xa4] = 0;
  param_1[0xa5] = 0;
  param_1[0xa6] = 0;
  param_1[0xa7] = 0;
  param_1[0xa8] = 0;
  param_1[0xa9] = 0;
  param_1[0xaa] = 0;
  param_1[0xab] = 0;
  param_1[0xac] = 0;
  param_1[0xad] = 0;
  param_1[0xae] = 0;
  param_1[0xaf] = 0;
  param_1[0xb0] = 0;
  param_1[0xb1] = 0;
  param_1[0xb2] = 0;
  param_1[0xb3] = 0;
  param_1[0xb4] = 0;
  param_1[0xb5] = 0;
  *(undefined4 *)(param_1 + 0xb6) = 0;
  *(undefined2 *)((longlong)param_1 + 0x5b4) = 0;
  *(undefined1 *)((longlong)param_1 + 0x5b6) = 0;
  param_1[0xb7] = 0xffffffffffffffff;
  param_1[0xb8] = 0;
  param_1[0xb9] = 0;
  param_1[0xba] = 0;
  param_1[0xbb] = 0;
  param_1[0xbd] = 0;
  param_1[0xbc] = 0;
  param_1[0xbe] = 0;
  param_1[0xbf] = 0;
  param_1[0xc0] = 0;
  param_1[0xc1] = 0;
  param_1[0xc2] = 0;
  param_1[0xc3] = 0;
  param_1[0xc4] = 0;
  param_1[0xc5] = 0;
  param_1[0xc6] = 0;
  param_1[199] = 0;
  param_1[200] = 0;
  param_1[0xc9] = 0;
  param_1[0xca] = 0;
  param_1[0xcb] = 0;
  param_1[0xcc] = 0;
  param_1[0xcd] = 0;
  param_1[0xce] = 0;
  param_1[0xcf] = 0;
  *(undefined4 *)(param_1 + 0xd0) = 0;
  *(undefined4 *)((longlong)param_1 + 0x694) = 0xffffffff;
  return param_1;
}



// ---- FUN_1415419d0 @ 1415419d0 ----

void FUN_1415419d0(longlong param_1)

{
  bool bVar1;
  int iVar2;
  undefined8 *puVar3;
  longlong lVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  int iVar8;
  undefined2 *puVar9;
  longlong *plVar10;
  longlong local_res20;
  undefined4 local_118;
  undefined8 local_114;
  longlong local_108;
  longlong local_100;
  undefined8 local_f8;
  undefined4 local_f0;
  undefined8 local_ec;
  longlong local_e0;
  longlong local_d8;
  undefined8 local_d0;
  undefined1 local_c8 [8];
  int local_c0;
  longlong local_b8;
  longlong local_b0;
  undefined1 local_a0 [8];
  int local_98;
  longlong local_90;
  longlong local_88;
  undefined4 local_78;
  undefined4 local_74;
  longlong local_70;
  longlong local_68 [2];
  undefined8 local_58;
  undefined1 local_50 [24];
  
  puVar3 = (undefined8 *)FUN_140d26530(local_68);
  if (*(int *)(puVar3 + 1) == 0) {
    puVar9 = &DAT_14391f7d0;
  }
  else {
    puVar9 = (undefined2 *)*puVar3;
  }
  FUN_141eb3490(&local_res20,puVar9);
  if (local_68[0] != 0) {
    FUN_141de9200();
  }
  if (*(int *)(param_1 + 0x4b8) != *(int *)(param_1 + 0x4e4)) {
    lVar4 = *(longlong *)(param_1 + 0x4f0);
    if (*(longlong *)(param_1 + 0x4f0) == 0) {
      lVar4 = param_1 + 0x4e8;
    }
    iVar2 = *(int *)(lVar4 + ((longlong)((int)((ulonglong)local_res20 >> 0x20) + (int)local_res20) &
                             (longlong)*(int *)(param_1 + 0x4f8) - 1U) * 4);
    if (iVar2 != -1) {
      do {
        plVar10 = (longlong *)((longlong)iVar2 * 0x38 + *(longlong *)(param_1 + 0x4b0));
        if (*plVar10 == local_res20) {
          if ((iVar2 != -1) && (plVar10 != (longlong *)0x0)) {
            FUN_140ccaa30(&local_f0,plVar10 + 1);
            goto LAB_141541abe;
          }
          break;
        }
        iVar2 = (int)plVar10[6];
      } while (iVar2 != -1);
    }
  }
  local_d8 = 0;
  local_e0 = 0;
  local_d0 = 0;
  local_f0 = 0xffffffff;
  local_ec = 0;
LAB_141541abe:
  lVar4 = FUN_142058d50(&local_f0);
  if (lVar4 == 0) {
    LOCK();
    if (DAT_1456e073c == 0) {
      DAT_1456e073c = 0;
    }
    UNLOCK();
    if ((DAT_1456e073c != local_ec._4_4_) && (local_e0 != 0)) {
      lVar4 = FUN_14204ac70(&local_e0);
      FUN_1420575e0(&local_f0,lVar4);
      if ((lVar4 != 0) || (DAT_145b9d472 == '\0')) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        local_ec = CONCAT44(DAT_1456e073c,(undefined4)local_ec);
      }
      lVar4 = FUN_142058d50(&local_f0);
      if (lVar4 != 0) goto LAB_141541b42;
    }
  }
  else {
LAB_141541b42:
    lVar5 = FUN_14327bf50();
    if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) + (longlong)*(int *)(lVar5 + 0x90) * 8)
        != lVar5 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) {
      lVar4 = FUN_142058d50(&local_f0);
      if (lVar4 == 0) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        if ((DAT_1456e073c != local_ec._4_4_) && (local_e0 != 0)) {
          lVar4 = FUN_14204ac70(&local_e0);
          FUN_1420575e0(&local_f0,lVar4);
          if ((lVar4 != 0) || (DAT_145b9d472 == '\0')) {
            LOCK();
            if (DAT_1456e073c == 0) {
              DAT_1456e073c = 0;
            }
            UNLOCK();
            local_ec = CONCAT44(DAT_1456e073c,(undefined4)local_ec);
          }
          lVar4 = FUN_142058d50(&local_f0);
          if (lVar4 != 0) goto LAB_141541c1d;
        }
LAB_141541c53:
        lVar4 = 0;
      }
      else {
LAB_141541c1d:
        lVar5 = FUN_14327bf50();
        if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar5 + 0x90) * 8) != lVar5 + 0x88)) {
          bVar1 = false;
        }
        else {
          bVar1 = true;
        }
        if (!bVar1) goto LAB_141541c53;
      }
      plVar10 = *(longlong **)(param_1 + 0x538);
      if ((plVar10 != (longlong *)0x0) && (lVar4 != 0)) {
        *(longlong *)(param_1 + 0x580) = lVar4;
        (**(code **)(*plVar10 + 0x2b0))(plVar10,lVar4);
      }
    }
  }
  if (local_d8 != 0) {
    FUN_141de9200();
  }
  lVar4 = *(longlong *)(param_1 + 0x608);
  lVar5 = (longlong)*(int *)(param_1 + 0x610) * 0x40 + lVar4;
  for (; lVar4 != lVar5; lVar4 = lVar4 + 0x40) {
    lVar6 = *(longlong *)(lVar4 + 8);
    local_100 = 0;
    local_f8 = 0;
    local_108 = 0;
    local_118 = *(undefined4 *)(lVar6 + 8);
    local_114 = *(undefined8 *)(lVar6 + 0xc);
    local_58 = *(undefined8 *)(lVar6 + 0x18);
    FUN_1407e5680(local_50,lVar6 + 0x20);
    FUN_1420366d0(&local_108,&local_58);
    lVar6 = FUN_142058d50(&local_118);
    if (lVar6 == 0) {
      LOCK();
      if (DAT_1456e073c == 0) {
        DAT_1456e073c = 0;
      }
      UNLOCK();
      if ((DAT_1456e073c != local_114._4_4_) && (local_108 != 0)) {
        lVar6 = FUN_14204ac70(&local_108);
        FUN_1420575e0(&local_118,lVar6);
        if ((lVar6 != 0) || (DAT_145b9d472 == '\0')) {
          LOCK();
          if (DAT_1456e073c == 0) {
            DAT_1456e073c = 0;
          }
          UNLOCK();
          local_114 = CONCAT44(DAT_1456e073c,(undefined4)local_114);
        }
        lVar6 = FUN_142058d50(&local_118);
        if (lVar6 != 0) goto LAB_141541d9c;
      }
    }
    else {
LAB_141541d9c:
      lVar7 = FUN_14327bf50();
      if ((*(int *)(*(longlong *)(lVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
         (*(longlong *)
           (*(longlong *)(*(longlong *)(lVar6 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8
           ) != lVar7 + 0x88)) {
        bVar1 = false;
      }
      else {
        bVar1 = true;
      }
      if (bVar1) {
        lVar6 = FUN_142058d50(&local_118);
        if (lVar6 == 0) {
          LOCK();
          if (DAT_1456e073c == 0) {
            DAT_1456e073c = 0;
          }
          UNLOCK();
          if ((DAT_1456e073c != local_114._4_4_) && (local_108 != 0)) {
            lVar6 = FUN_14204ac70(&local_108);
            FUN_1420575e0(&local_118,lVar6);
            if ((lVar6 != 0) || (DAT_145b9d472 == '\0')) {
              LOCK();
              if (DAT_1456e073c == 0) {
                DAT_1456e073c = 0;
              }
              UNLOCK();
              local_114 = CONCAT44(DAT_1456e073c,(undefined4)local_114);
            }
            lVar6 = FUN_142058d50(&local_118);
            if (lVar6 != 0) goto LAB_141541e5a;
          }
        }
        else {
LAB_141541e5a:
          lVar7 = FUN_14327bf50();
          if ((*(int *)(*(longlong *)(lVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
             (*(longlong *)
               (*(longlong *)(*(longlong *)(lVar6 + 0x10) + 0x88) +
               (longlong)*(int *)(lVar7 + 0x90) * 8) != lVar7 + 0x88)) {
            bVar1 = false;
          }
          else {
            bVar1 = true;
          }
          if ((bVar1) && (*(longlong *)(param_1 + 0x598) != 0)) {
            local_78 = 0xffffffff;
            local_74 = 0;
            local_70 = 0;
            lVar6 = FUN_142058d50(&local_118);
            if (lVar6 == 0) {
              LOCK();
              if (DAT_1456e073c == 0) {
                DAT_1456e073c = 0;
              }
              UNLOCK();
              if ((DAT_1456e073c != local_114._4_4_) && (local_108 != 0)) {
                lVar6 = FUN_14204ac70(&local_108);
                FUN_1420575e0(&local_118,lVar6);
                if ((lVar6 != 0) || (DAT_145b9d472 == '\0')) {
                  LOCK();
                  if (DAT_1456e073c == 0) {
                    DAT_1456e073c = 0;
                  }
                  UNLOCK();
                  local_114 = CONCAT44(DAT_1456e073c,(undefined4)local_114);
                }
                lVar6 = FUN_142058d50(&local_118);
                if (lVar6 != 0) goto LAB_141541f30;
              }
LAB_141541f6a:
              local_70 = 0;
            }
            else {
LAB_141541f30:
              lVar7 = FUN_14327bf50();
              if ((*(int *)(*(longlong *)(lVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
                 (*(longlong *)
                   (*(longlong *)(*(longlong *)(lVar6 + 0x10) + 0x88) +
                   (longlong)*(int *)(lVar7 + 0x90) * 8) != lVar7 + 0x88)) {
                bVar1 = false;
              }
              else {
                bVar1 = true;
              }
              local_70 = lVar6;
              if (!bVar1) goto LAB_141541f6a;
            }
            FUN_1415522f0(*(undefined8 *)(param_1 + 0x598),&local_78);
          }
        }
      }
    }
    if (local_100 != 0) {
      FUN_141de9200();
    }
  }
  if (*(int *)(param_1 + 0x468) == *(int *)(param_1 + 0x494)) {
    return;
  }
  iVar2 = *(int *)(((ulonglong)(*(longlong *)(param_1 + 0x4a0) != 0) - 1 & param_1 + 0x498U |
                   *(ulonglong *)(param_1 + 0x4a0)) +
                  ((longlong)((int)((ulonglong)local_res20 >> 0x20) + (int)local_res20) &
                  (longlong)*(int *)(param_1 + 0x4a8) - 1U) * 4);
  if (iVar2 == -1) {
    return;
  }
  lVar4 = *(longlong *)(param_1 + 0x460);
  iVar8 = iVar2;
  while (*(longlong *)((longlong)iVar8 * 0x60 + lVar4) != local_res20) {
    iVar8 = *(int *)((longlong)iVar8 * 0x60 + 0x58 + lVar4);
    if (iVar8 == -1) {
      return;
    }
  }
  if (iVar8 == -1) {
    return;
  }
  if (*(int *)(param_1 + 0x468) == *(int *)(param_1 + 0x494)) {
    return;
  }
  if (iVar2 == -1) {
    return;
  }
  while (plVar10 = (longlong *)((longlong)iVar2 * 0x60 + lVar4), *plVar10 != local_res20) {
    iVar2 = (int)plVar10[0xb];
    if (iVar2 == -1) {
      return;
    }
  }
  if (iVar2 == -1) {
    return;
  }
  if (plVar10 == (longlong *)0x0) {
    return;
  }
  if (plVar10 + 1 == (longlong *)0x0) {
    return;
  }
  FUN_140ccaa30(local_a0,plVar10 + 1);
  FUN_140ccaa30(local_c8,plVar10 + 6);
  lVar4 = FUN_142058d50(local_a0);
  if (lVar4 == 0) {
    LOCK();
    if (DAT_1456e073c == 0) {
      DAT_1456e073c = 0;
    }
    UNLOCK();
    if ((DAT_1456e073c != local_98) && (local_90 != 0)) {
      lVar4 = FUN_14204ac70(&local_90);
      FUN_1420575e0(local_a0,lVar4);
      if ((lVar4 != 0) || (DAT_145b9d472 == '\0')) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        local_98 = DAT_1456e073c;
      }
      lVar4 = FUN_142058d50(local_a0);
      if (lVar4 != 0) goto LAB_14154214f;
    }
  }
  else {
LAB_14154214f:
    lVar5 = FUN_14327bf50();
    if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) + (longlong)*(int *)(lVar5 + 0x90) * 8)
        != lVar5 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) {
      lVar4 = FUN_142058d50(local_a0);
      if (lVar4 == 0) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        if ((DAT_1456e073c != local_98) && (local_90 != 0)) {
          lVar4 = FUN_14204ac70(&local_90);
          FUN_1420575e0(local_a0,lVar4);
          if ((lVar4 != 0) || (DAT_145b9d472 == '\0')) {
            LOCK();
            if (DAT_1456e073c == 0) {
              DAT_1456e073c = 0;
            }
            UNLOCK();
            local_98 = DAT_1456e073c;
          }
          lVar4 = FUN_142058d50(local_a0);
          if (lVar4 != 0) goto LAB_141542206;
        }
      }
      else {
LAB_141542206:
        lVar5 = FUN_14327bf50();
        if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar5 + 0x90) * 8) != lVar5 + 0x88)) {
          bVar1 = false;
        }
        else {
          bVar1 = true;
        }
        if (bVar1) {
          lVar4 = FUN_142058d50(local_a0);
          if (lVar4 == 0) {
            LOCK();
            if (DAT_1456e073c == 0) {
              DAT_1456e073c = 0;
            }
            UNLOCK();
            if ((DAT_1456e073c != local_98) && (local_90 != 0)) {
              lVar4 = FUN_14204ac70(&local_90);
              FUN_1420575e0(local_a0,lVar4);
              if ((lVar4 != 0) || (DAT_145b9d472 == '\0')) {
                LOCK();
                if (DAT_1456e073c == 0) {
                  DAT_1456e073c = 0;
                }
                UNLOCK();
                local_98 = DAT_1456e073c;
              }
              lVar4 = FUN_142058d50(local_a0);
              if (lVar4 != 0) goto LAB_1415422b9;
            }
          }
          else {
LAB_1415422b9:
            lVar5 = FUN_14327bf50();
            if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
               (*(longlong *)
                 (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) +
                 (longlong)*(int *)(lVar5 + 0x90) * 8) != lVar5 + 0x88)) {
              bVar1 = false;
            }
            else {
              bVar1 = true;
            }
            if (bVar1) {
              *(longlong *)(param_1 + 0x588) = lVar4;
            }
          }
        }
      }
    }
  }
  lVar4 = FUN_142058d50(local_c8);
  if (lVar4 == 0) {
    LOCK();
    if (DAT_1456e073c == 0) {
      DAT_1456e073c = 0;
    }
    UNLOCK();
    if ((DAT_1456e073c == local_c0) || (local_b8 == 0)) goto LAB_141542529;
    lVar4 = FUN_14204ac70(&local_b8);
    FUN_1420575e0(local_c8,lVar4);
    if ((lVar4 != 0) || (DAT_145b9d472 == '\0')) {
      LOCK();
      if (DAT_1456e073c == 0) {
        DAT_1456e073c = 0;
      }
      UNLOCK();
      local_c0 = DAT_1456e073c;
    }
    lVar4 = FUN_142058d50(local_c8);
    if (lVar4 == 0) goto LAB_141542529;
  }
  lVar5 = FUN_14327bf50();
  if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
     (*(longlong *)
       (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) + (longlong)*(int *)(lVar5 + 0x90) * 8) !=
      lVar5 + 0x88)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  if (!bVar1) goto LAB_141542529;
  lVar4 = FUN_142058d50(local_c8);
  if (lVar4 == 0) {
    LOCK();
    if (DAT_1456e073c == 0) {
      DAT_1456e073c = 0;
    }
    UNLOCK();
    if ((DAT_1456e073c == local_c0) || (local_b8 == 0)) goto LAB_141542529;
    lVar4 = FUN_14204ac70(&local_b8);
    FUN_1420575e0(local_c8,lVar4);
    if ((lVar4 != 0) || (DAT_145b9d472 == '\0')) {
      LOCK();
      if (DAT_1456e073c == 0) {
        DAT_1456e073c = 0;
      }
      UNLOCK();
      local_c0 = DAT_1456e073c;
    }
    lVar4 = FUN_142058d50(local_c8);
    if (lVar4 == 0) goto LAB_141542529;
  }
  lVar5 = FUN_14327bf50();
  if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
     (*(longlong *)
       (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) + (longlong)*(int *)(lVar5 + 0x90) * 8) !=
      lVar5 + 0x88)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  if (!bVar1) goto LAB_141542529;
  lVar4 = FUN_142058d50(local_c8);
  if (lVar4 == 0) {
    LOCK();
    if (DAT_1456e073c == 0) {
      DAT_1456e073c = 0;
    }
    UNLOCK();
    if ((DAT_1456e073c == local_c0) || (local_b8 == 0)) goto LAB_141542529;
    lVar4 = FUN_14204ac70(&local_b8);
    FUN_1420575e0(local_c8,lVar4);
    if ((lVar4 != 0) || (DAT_145b9d472 == '\0')) {
      LOCK();
      if (DAT_1456e073c == 0) {
        DAT_1456e073c = 0;
      }
      UNLOCK();
      local_c0 = DAT_1456e073c;
    }
    lVar4 = FUN_142058d50(local_c8);
    if (lVar4 == 0) goto LAB_141542529;
  }
  lVar5 = FUN_14327bf50();
  if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(lVar5 + 0x90)) ||
     (*(longlong *)
       (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) + (longlong)*(int *)(lVar5 + 0x90) * 8) !=
      lVar5 + 0x88)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  if (bVar1) {
    *(longlong *)(param_1 + 0x590) = lVar4;
  }
LAB_141542529:
  if (local_b0 != 0) {
    FUN_141de9200();
  }
  if (local_88 != 0) {
    FUN_141de9200();
  }
  return;
}



// ---- FUN_141553d90 @ 141553d90 ----

void FUN_141553d90(longlong param_1)

{
  int *piVar1;
  bool bVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined8 *puVar6;
  longlong lVar7;
  longlong lVar8;
  ulonglong uVar9;
  undefined8 uVar10;
  int iVar11;
  undefined2 *puVar12;
  longlong *plVar13;
  longlong *plVar14;
  longlong *plVar15;
  longlong *plVar16;
  longlong lVar17;
  longlong lVar18;
  longlong local_res8;
  undefined8 local_res10;
  undefined8 local_res18;
  undefined8 local_res20;
  longlong local_138;
  undefined8 local_130;
  undefined4 local_128;
  undefined4 local_124;
  int local_120;
  longlong local_118;
  undefined2 *local_110;
  undefined8 local_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined1 local_f0 [8];
  int local_e8;
  longlong local_e0;
  longlong local_d8 [2];
  undefined1 local_c8 [8];
  int local_c0;
  longlong local_b8;
  longlong local_b0 [2];
  longlong local_a0 [2];
  undefined4 local_90;
  undefined8 local_8c;
  longlong local_80;
  longlong local_78;
  undefined8 local_70;
  undefined8 *local_68;
  undefined4 local_60;
  longlong local_58 [2];
  undefined1 local_48 [8];
  longlong *local_40;
  
  plVar14 = (longlong *)0x0;
  local_138 = 0;
  local_130 = 0;
  puVar6 = (undefined8 *)FUN_140d26530(local_58);
  if (*(int *)(puVar6 + 1) == 0) {
    puVar12 = &DAT_14391f7d0;
  }
  else {
    puVar12 = (undefined2 *)*puVar6;
  }
  FUN_141eb3490(&local_res8,puVar12);
  if (local_58[0] != 0) {
    FUN_141de9200();
  }
  if ((*(int *)(param_1 + 0x4b8) != *(int *)(param_1 + 0x4e4)) &&
     (iVar5 = *(int *)(((ulonglong)(*(longlong *)(param_1 + 0x4f0) != 0) - 1 & param_1 + 0x4e8U |
                       *(ulonglong *)(param_1 + 0x4f0)) +
                      ((longlong)((int)((ulonglong)local_res8 >> 0x20) + (int)local_res8) &
                      (longlong)*(int *)(param_1 + 0x4f8) - 1U) * 4), iVar5 != -1)) {
    lVar17 = *(longlong *)(param_1 + 0x4b0);
    iVar11 = iVar5;
    do {
      if (*(longlong *)((longlong)iVar11 * 0x38 + lVar17) == local_res8) {
        if (iVar11 != -1) {
          plVar16 = plVar14;
          if ((*(int *)(param_1 + 0x4b8) == *(int *)(param_1 + 0x4e4)) || (iVar5 == -1))
          goto LAB_141553eae;
          goto LAB_141553e90;
        }
        break;
      }
      iVar11 = *(int *)((longlong)iVar11 * 0x38 + 0x30 + lVar17);
    } while (iVar11 != -1);
  }
  goto LAB_141553ef4;
  while (iVar5 = (int)plVar15[6], iVar5 != -1) {
LAB_141553e90:
    plVar15 = (longlong *)((longlong)iVar5 * 0x38 + lVar17);
    if (*plVar15 == local_res8) {
      if (((iVar5 == -1) || (plVar15 == (longlong *)0x0)) ||
         (plVar16 = plVar15 + 1, plVar16 == (longlong *)0x0)) break;
      lVar17 = FUN_142058d50(plVar16);
      if (lVar17 == 0) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        if ((DAT_1456e073c == (int)plVar15[2]) || (local_res10 = 0, plVar15[3] == 0)) break;
        lVar17 = FUN_14204ac70();
        FUN_1420575e0(plVar16,lVar17);
        if ((lVar17 != 0) || (DAT_145b9d472 == '\0')) {
          LOCK();
          if (DAT_1456e073c == 0) {
            DAT_1456e073c = 0;
          }
          UNLOCK();
          *(int *)(plVar15 + 2) = DAT_1456e073c;
        }
        lVar17 = FUN_142058d50(plVar16);
        if (lVar17 == 0) break;
      }
      lVar18 = FUN_14327bf50();
      if ((*(int *)(*(longlong *)(lVar17 + 0x10) + 0x90) < *(int *)(lVar18 + 0x90)) ||
         (*(longlong *)
           (*(longlong *)(*(longlong *)(lVar17 + 0x10) + 0x88) +
           (longlong)*(int *)(lVar18 + 0x90) * 8) != lVar18 + 0x88)) {
        bVar2 = false;
      }
      else {
        bVar2 = true;
      }
      if (bVar2) {
        if (*(int *)(param_1 + 0x4b8) == *(int *)(param_1 + 0x4e4)) goto LAB_1415540cb;
        lVar17 = *(longlong *)(param_1 + 0x4f0);
        if (*(longlong *)(param_1 + 0x4f0) == 0) {
          lVar17 = param_1 + 0x4e8;
        }
        iVar5 = *(int *)(lVar17 + ((longlong)
                                   ((int)((ulonglong)local_res8 >> 0x20) + (int)local_res8) &
                                  (longlong)*(int *)(param_1 + 0x4f8) - 1U) * 4);
        if (iVar5 == -1) goto LAB_1415540cb;
        goto LAB_1415540b0;
      }
      break;
    }
  }
LAB_141553eae:
  uVar9 = local_130;
  lVar17 = (longlong)(int)local_130;
  iVar5 = (int)local_130 + 1;
  local_130 = CONCAT44(local_130._4_4_,iVar5);
  if (local_130._4_4_ < iVar5) {
    FUN_1407f1590(&local_138,uVar9 & 0xffffffff);
  }
  plVar14 = (longlong *)(local_138 + lVar17 * 0x18);
  *plVar14 = plVar16[2];
  FUN_1407e5680(plVar14 + 1,plVar16 + 3);
  goto LAB_141553ef4;
  while (iVar5 = (int)plVar13[6], iVar5 != -1) {
LAB_1415540b0:
    plVar13 = (longlong *)((longlong)iVar5 * 0x38 + *(longlong *)(param_1 + 0x4b0));
    if (*plVar13 == local_res8) {
      if ((iVar5 != -1) && (plVar13 != (longlong *)0x0)) {
        FUN_140ccaa30(&local_90,plVar13 + 1);
        goto LAB_1415540ec;
      }
      break;
    }
  }
LAB_1415540cb:
  local_78 = 0;
  local_80 = 0;
  local_70 = 0;
  local_90 = 0xffffffff;
  local_8c = 0;
  local_res10 = 0;
LAB_1415540ec:
  lVar17 = FUN_142058d50(&local_90);
  if (lVar17 == 0) {
    LOCK();
    if (DAT_1456e073c == 0) {
      DAT_1456e073c = 0;
    }
    UNLOCK();
    if ((DAT_1456e073c != local_8c._4_4_) && (local_res10 = 0, local_80 != 0)) {
      lVar17 = FUN_14204ac70(&local_80);
      FUN_1420575e0(&local_90,lVar17);
      if ((lVar17 != 0) || (DAT_145b9d472 == '\0')) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        local_8c = CONCAT44(DAT_1456e073c,(undefined4)local_8c);
      }
      lVar17 = FUN_142058d50(&local_90);
      if (lVar17 != 0) goto LAB_14155416c;
    }
  }
  else {
LAB_14155416c:
    lVar18 = FUN_14327bf50();
    if ((*(int *)(*(longlong *)(lVar17 + 0x10) + 0x90) < *(int *)(lVar18 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar17 + 0x10) + 0x88) + (longlong)*(int *)(lVar18 + 0x90) * 8
         ) != lVar18 + 0x88)) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    if (bVar2) {
      plVar13 = (longlong *)FUN_142058d50(plVar16);
      if (plVar13 == (longlong *)0x0) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        if ((DAT_1456e073c != (int)plVar15[2]) && (local_res10 = 0, plVar15[3] != 0)) {
          lVar17 = FUN_14204ac70();
          FUN_1420575e0(plVar16,lVar17);
          if ((lVar17 != 0) || (DAT_145b9d472 == '\0')) {
            LOCK();
            if (DAT_1456e073c == 0) {
              DAT_1456e073c = 0;
            }
            UNLOCK();
            *(int *)(plVar15 + 2) = DAT_1456e073c;
          }
          plVar13 = (longlong *)FUN_142058d50(plVar16);
          if (plVar13 != (longlong *)0x0) goto LAB_14155423e;
        }
LAB_141554274:
        plVar13 = plVar14;
      }
      else {
LAB_14155423e:
        lVar17 = FUN_14327bf50();
        if ((*(int *)(plVar13[2] + 0x90) < *(int *)(lVar17 + 0x90)) ||
           (*(longlong *)(*(longlong *)(plVar13[2] + 0x88) + (longlong)*(int *)(lVar17 + 0x90) * 8)
            != lVar17 + 0x88)) {
          bVar2 = false;
        }
        else {
          bVar2 = true;
        }
        if (!bVar2) goto LAB_141554274;
      }
      plVar14 = *(longlong **)(param_1 + 0x538);
      if ((plVar14 != (longlong *)0x0) && (plVar13 != (longlong *)0x0)) {
        *(longlong **)(param_1 + 0x580) = plVar13;
        (**(code **)(*plVar14 + 0x2b0))(plVar14,plVar13);
      }
    }
  }
  if (local_78 != 0) {
    FUN_141de9200();
  }
LAB_141553ef4:
  if ((*(int *)(param_1 + 0x468) != *(int *)(param_1 + 0x494)) &&
     (iVar5 = *(int *)(((ulonglong)(*(longlong *)(param_1 + 0x4a0) != 0) - 1 & param_1 + 0x498U |
                       *(ulonglong *)(param_1 + 0x4a0)) +
                      ((longlong)((int)((ulonglong)local_res8 >> 0x20) + (int)local_res8) &
                      (longlong)*(int *)(param_1 + 0x4a8) - 1U) * 4), iVar5 != -1)) {
    lVar17 = *(longlong *)(param_1 + 0x460);
    iVar11 = iVar5;
    do {
      if (*(longlong *)((longlong)iVar11 * 0x60 + lVar17) == local_res8) {
        if ((iVar11 != -1) && (*(int *)(param_1 + 0x468) != *(int *)(param_1 + 0x494)))
        goto joined_r0x0001415542ca;
        break;
      }
      iVar11 = *(int *)((longlong)iVar11 * 0x60 + 0x58 + lVar17);
    } while (iVar11 != -1);
  }
LAB_141554828:
  lVar17 = *(longlong *)(param_1 + 0x608);
  lVar18 = (longlong)*(int *)(param_1 + 0x610) * 0x40 + lVar17;
  do {
    if (lVar17 == lVar18) {
      lVar17 = FUN_1411fc890();
      local_a0[0] = 0;
      local_a0[1] = 0x1f;
      FUN_1407e2ee0(local_a0,0);
      FUN_141dd1c10(local_a0[0],L"RequestAsyncLoad ArrayDelegate",0x3e);
      uStack_f8 = uStack_f8 & 0xffffffff00000000;
      local_68 = (undefined8 *)0x0;
      local_100 = FUN_1415419d0;
      local_60 = 0;
      puVar6 = (undefined8 *)FUN_141df5850(0,0x30);
      local_60 = 3;
      local_68 = puVar6;
      if (puVar6 != (undefined8 *)0x0) {
        *puVar6 = &PTR_FUN_143968d20;
        FUN_1420575e0(puVar6 + 1,param_1);
        *(undefined4 *)(puVar6 + 2) = (undefined4)local_100;
        *(undefined4 *)((longlong)puVar6 + 0x14) = local_100._4_4_;
        *(undefined4 *)(puVar6 + 3) = (undefined4)uStack_f8;
        *(undefined4 *)((longlong)puVar6 + 0x1c) = uStack_f8._4_4_;
        uVar10 = FUN_141db8700();
        puVar6[5] = uVar10;
        *puVar6 = &PTR_FUN_143968d70;
      }
      FUN_1430a5950(lVar17 + 0xf0,local_48,&local_138,&local_68,0,0,0,local_a0);
      if (local_40 != (longlong *)0x0) {
        plVar14 = local_40 + 1;
        *(int *)plVar14 = (int)*plVar14 + -1;
        if ((int)*plVar14 == 0) {
          (**(code **)*local_40)(local_40);
          piVar1 = (int *)((longlong)local_40 + 0xc);
          *piVar1 = *piVar1 + -1;
          if (*piVar1 == 0) {
            (**(code **)(*local_40 + 8))(local_40,1);
          }
        }
      }
      if (local_a0[0] != 0) {
        FUN_141de9200();
      }
      FUN_1408439e0(&local_138);
      return;
    }
    lVar7 = *(longlong *)(lVar17 + 8);
    local_118 = 0;
    local_110 = (undefined2 *)0x0;
    local_108 = 0;
    local_128 = 0xffffffff;
    local_124 = 0;
    FUN_1410095c0(&local_118);
    local_120 = 0;
    FUN_14101af50(&local_128,lVar7 + 8);
    lVar7 = FUN_142058d50(&local_128);
    if (lVar7 == 0) {
      LOCK();
      if (DAT_1456e073c == 0) {
        DAT_1456e073c = 0;
      }
      UNLOCK();
      if ((DAT_1456e073c != local_120) && (local_res10 = 0, local_118 != 0)) {
        lVar7 = FUN_14204ac70(&local_118);
        FUN_1420575e0(&local_128,lVar7);
        if ((lVar7 != 0) || (DAT_145b9d472 == '\0')) {
          LOCK();
          if (DAT_1456e073c == 0) {
            DAT_1456e073c = 0;
          }
          UNLOCK();
          local_120 = DAT_1456e073c;
        }
        lVar7 = FUN_142058d50(&local_128);
        if (lVar7 != 0) goto LAB_141554914;
      }
LAB_141554b3e:
      uVar9 = local_130;
      lVar7 = (longlong)(int)local_130;
      iVar5 = (int)local_130 + 1;
      local_130 = CONCAT44(local_130._4_4_,iVar5);
      if (local_130._4_4_ < iVar5) {
        FUN_1407f1590(&local_138,uVar9 & 0xffffffff);
      }
      plVar14 = (longlong *)(local_138 + lVar7 * 0x18);
      *plVar14 = local_118;
      FUN_1407e5680(plVar14 + 1,&local_110);
    }
    else {
LAB_141554914:
      lVar8 = FUN_14327bf50();
      if ((*(int *)(*(longlong *)(lVar7 + 0x10) + 0x90) < *(int *)(lVar8 + 0x90)) ||
         (*(longlong *)
           (*(longlong *)(*(longlong *)(lVar7 + 0x10) + 0x88) + (longlong)*(int *)(lVar8 + 0x90) * 8
           ) != lVar8 + 0x88)) {
        bVar2 = false;
      }
      else {
        bVar2 = true;
      }
      if (!bVar2) goto LAB_141554b3e;
      lVar7 = FUN_142058d50(&local_128);
      if (lVar7 == 0) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        if ((DAT_1456e073c != local_120) && (local_res18 = 0, local_118 != 0)) {
          lVar7 = FUN_14204ac70(&local_118);
          FUN_1420575e0(&local_128,lVar7);
          if ((lVar7 != 0) || (DAT_145b9d472 == '\0')) {
            LOCK();
            if (DAT_1456e073c == 0) {
              DAT_1456e073c = 0;
            }
            UNLOCK();
            local_120 = DAT_1456e073c;
          }
          lVar7 = FUN_142058d50(&local_128);
          if (lVar7 != 0) goto LAB_1415549d5;
        }
        goto LAB_141554b3e;
      }
LAB_1415549d5:
      lVar8 = FUN_14327bf50();
      if ((*(int *)(*(longlong *)(lVar7 + 0x10) + 0x90) < *(int *)(lVar8 + 0x90)) ||
         (*(longlong *)
           (*(longlong *)(*(longlong *)(lVar7 + 0x10) + 0x88) + (longlong)*(int *)(lVar8 + 0x90) * 8
           ) != lVar8 + 0x88)) {
        bVar2 = false;
      }
      else {
        bVar2 = true;
      }
      if ((!bVar2) || (*(longlong *)(param_1 + 0x598) == 0)) goto LAB_141554b3e;
      local_100 = (code *)0xffffffff;
      uStack_f8 = 0;
      uVar9 = FUN_142058d50(&local_128);
      if (uVar9 == 0) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        if ((DAT_1456e073c != local_120) && (local_res20 = 0, local_118 != 0)) {
          lVar7 = FUN_14204ac70(&local_118);
          FUN_1420575e0(&local_128,lVar7);
          if ((lVar7 != 0) || (DAT_145b9d472 == '\0')) {
            LOCK();
            if (DAT_1456e073c == 0) {
              DAT_1456e073c = 0;
            }
            UNLOCK();
            local_120 = DAT_1456e073c;
          }
          uVar9 = FUN_142058d50(&local_128);
          if (uVar9 != 0) goto LAB_141554ab0;
        }
LAB_141554aea:
        uStack_f8 = 0;
      }
      else {
LAB_141554ab0:
        lVar7 = FUN_14327bf50();
        if ((*(int *)(*(longlong *)(uVar9 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(uVar9 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar7 + 0x90) * 8) != lVar7 + 0x88)) {
          bVar2 = false;
        }
        else {
          bVar2 = true;
        }
        uStack_f8 = uVar9;
        if (!bVar2) goto LAB_141554aea;
      }
      uVar3 = FUN_141545920(0,(int)((ulonglong)local_118 >> 0x20) + (int)local_118);
      puVar12 = &DAT_14391f7d0;
      if ((int)local_108 != 0) {
        puVar12 = local_110;
      }
      uVar4 = FUN_1407e51a0(puVar12);
      uVar3 = FUN_141545920(uVar3,uVar4);
      local_100 = (code *)CONCAT44(uVar3,(undefined4)local_100);
      FUN_1415522f0(*(undefined8 *)(param_1 + 0x598),&local_100,lVar17);
    }
    if (local_110 != (undefined2 *)0x0) {
      FUN_141de9200();
    }
    lVar17 = lVar17 + 0x40;
  } while( true );
joined_r0x0001415542ca:
  if (iVar5 == -1) goto LAB_141554828;
  plVar14 = (longlong *)((longlong)iVar5 * 0x60 + lVar17);
  if (*plVar14 == local_res8) {
    if (((iVar5 != -1) && (plVar14 != (longlong *)0x0)) && (plVar14 + 1 != (longlong *)0x0)) {
      FUN_140ccaa30(local_c8,plVar14 + 1);
      FUN_140ccaa30(local_f0,plVar14 + 6);
      lVar17 = FUN_142058d50(local_c8);
      if (lVar17 == 0) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        if ((DAT_1456e073c != local_c0) && (local_res10 = 0, local_b8 != 0)) {
          lVar17 = FUN_14204ac70(&local_b8);
          FUN_1420575e0(local_c8,lVar17);
          if ((lVar17 != 0) || (DAT_145b9d472 == '\0')) {
            LOCK();
            if (DAT_1456e073c == 0) {
              DAT_1456e073c = 0;
            }
            UNLOCK();
            local_c0 = DAT_1456e073c;
          }
          lVar17 = FUN_142058d50(local_c8);
          if (lVar17 != 0) goto LAB_1415543a8;
        }
LAB_14155455b:
        uVar9 = local_130;
        lVar17 = (longlong)(int)local_130;
        iVar5 = (int)local_130 + 1;
        local_130 = CONCAT44(local_130._4_4_,iVar5);
        if (local_130._4_4_ < iVar5) {
          FUN_1407f1590(&local_138,uVar9 & 0xffffffff);
        }
        plVar14 = (longlong *)(local_138 + lVar17 * 0x18);
        *plVar14 = local_b8;
        FUN_1407e5680(plVar14 + 1,local_b0);
      }
      else {
LAB_1415543a8:
        lVar18 = FUN_14327bf50();
        if ((*(int *)(*(longlong *)(lVar17 + 0x10) + 0x90) < *(int *)(lVar18 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(lVar17 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar18 + 0x90) * 8) != lVar18 + 0x88)) {
          bVar2 = false;
        }
        else {
          bVar2 = true;
        }
        if (!bVar2) goto LAB_14155455b;
        lVar17 = FUN_142058d50(local_c8);
        if (lVar17 == 0) {
          LOCK();
          if (DAT_1456e073c == 0) {
            DAT_1456e073c = 0;
          }
          UNLOCK();
          if ((DAT_1456e073c != local_c0) && (local_res10 = 0, local_b8 != 0)) {
            lVar17 = FUN_14204ac70(&local_b8);
            FUN_1420575e0(local_c8,lVar17);
            if ((lVar17 != 0) || (DAT_145b9d472 == '\0')) {
              LOCK();
              if (DAT_1456e073c == 0) {
                DAT_1456e073c = 0;
              }
              UNLOCK();
              local_c0 = DAT_1456e073c;
            }
            lVar17 = FUN_142058d50(local_c8);
            if (lVar17 != 0) goto LAB_141554462;
          }
          goto LAB_14155455b;
        }
LAB_141554462:
        lVar18 = FUN_14327bf50();
        if ((*(int *)(*(longlong *)(lVar17 + 0x10) + 0x90) < *(int *)(lVar18 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(lVar17 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar18 + 0x90) * 8) != lVar18 + 0x88)) {
          bVar2 = false;
        }
        else {
          bVar2 = true;
        }
        if (!bVar2) goto LAB_14155455b;
        lVar17 = FUN_142058d50(local_c8);
        if (lVar17 == 0) {
          LOCK();
          if (DAT_1456e073c == 0) {
            DAT_1456e073c = 0;
          }
          UNLOCK();
          if ((DAT_1456e073c != local_c0) && (local_res10 = 0, local_b8 != 0)) {
            lVar17 = FUN_14204ac70(&local_b8);
            FUN_1420575e0(local_c8,lVar17);
            if ((lVar17 != 0) || (DAT_145b9d472 == '\0')) {
              LOCK();
              if (DAT_1456e073c == 0) {
                DAT_1456e073c = 0;
              }
              UNLOCK();
              local_c0 = DAT_1456e073c;
            }
            lVar17 = FUN_142058d50(local_c8);
            if (lVar17 != 0) goto LAB_14155451c;
          }
        }
        else {
LAB_14155451c:
          lVar18 = FUN_14327bf50();
          if ((*(int *)(*(longlong *)(lVar17 + 0x10) + 0x90) < *(int *)(lVar18 + 0x90)) ||
             (*(longlong *)
               (*(longlong *)(*(longlong *)(lVar17 + 0x10) + 0x88) +
               (longlong)*(int *)(lVar18 + 0x90) * 8) != lVar18 + 0x88)) {
            bVar2 = false;
          }
          else {
            bVar2 = true;
          }
          if (bVar2) {
            *(longlong *)(param_1 + 0x588) = lVar17;
          }
        }
      }
      lVar17 = FUN_142058d50(local_f0);
      if (lVar17 == 0) {
        LOCK();
        if (DAT_1456e073c == 0) {
          DAT_1456e073c = 0;
        }
        UNLOCK();
        if ((DAT_1456e073c != local_e8) && (local_res10 = 0, local_e0 != 0)) {
          lVar17 = FUN_14204ac70(&local_e0);
          FUN_1420575e0(local_f0,lVar17);
          if ((lVar17 != 0) || (DAT_145b9d472 == '\0')) {
            LOCK();
            if (DAT_1456e073c == 0) {
              DAT_1456e073c = 0;
            }
            UNLOCK();
            local_e8 = DAT_1456e073c;
          }
          lVar17 = FUN_142058d50(local_f0);
          if (lVar17 != 0) goto LAB_14155461a;
        }
LAB_1415547cd:
        uVar9 = local_130;
        lVar17 = (longlong)(int)local_130;
        iVar5 = (int)local_130 + 1;
        local_130 = CONCAT44(local_130._4_4_,iVar5);
        if (local_130._4_4_ < iVar5) {
          FUN_1407f1590(&local_138,uVar9 & 0xffffffff);
        }
        plVar14 = (longlong *)(local_138 + lVar17 * 0x18);
        *plVar14 = local_e0;
        FUN_1407e5680(plVar14 + 1,local_d8);
      }
      else {
LAB_14155461a:
        lVar18 = FUN_14327bf50();
        if ((*(int *)(*(longlong *)(lVar17 + 0x10) + 0x90) < *(int *)(lVar18 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(lVar17 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar18 + 0x90) * 8) != lVar18 + 0x88)) {
          bVar2 = false;
        }
        else {
          bVar2 = true;
        }
        if (!bVar2) goto LAB_1415547cd;
        lVar17 = FUN_142058d50(local_f0);
        if (lVar17 == 0) {
          LOCK();
          if (DAT_1456e073c == 0) {
            DAT_1456e073c = 0;
          }
          UNLOCK();
          if ((DAT_1456e073c != local_e8) && (local_res10 = 0, local_e0 != 0)) {
            lVar17 = FUN_14204ac70(&local_e0);
            FUN_1420575e0(local_f0,lVar17);
            if ((lVar17 != 0) || (DAT_145b9d472 == '\0')) {
              LOCK();
              if (DAT_1456e073c == 0) {
                DAT_1456e073c = 0;
              }
              UNLOCK();
              local_e8 = DAT_1456e073c;
            }
            lVar17 = FUN_142058d50(local_f0);
            if (lVar17 != 0) goto LAB_1415546d4;
          }
          goto LAB_1415547cd;
        }
LAB_1415546d4:
        lVar18 = FUN_14327bf50();
        if ((*(int *)(*(longlong *)(lVar17 + 0x10) + 0x90) < *(int *)(lVar18 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(lVar17 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar18 + 0x90) * 8) != lVar18 + 0x88)) {
          bVar2 = false;
        }
        else {
          bVar2 = true;
        }
        if (!bVar2) goto LAB_1415547cd;
        lVar17 = FUN_142058d50(local_f0);
        if (lVar17 == 0) {
          LOCK();
          if (DAT_1456e073c == 0) {
            DAT_1456e073c = 0;
          }
          UNLOCK();
          if ((DAT_1456e073c != local_e8) && (local_res10 = 0, local_e0 != 0)) {
            lVar17 = FUN_14204ac70(&local_e0);
            FUN_1420575e0(local_f0,lVar17);
            if ((lVar17 != 0) || (DAT_145b9d472 == '\0')) {
              LOCK();
              if (DAT_1456e073c == 0) {
                DAT_1456e073c = 0;
              }
              UNLOCK();
              local_e8 = DAT_1456e073c;
            }
            lVar17 = FUN_142058d50(local_f0);
            if (lVar17 != 0) goto LAB_14155478e;
          }
        }
        else {
LAB_14155478e:
          lVar18 = FUN_14327bf50();
          if ((*(int *)(*(longlong *)(lVar17 + 0x10) + 0x90) < *(int *)(lVar18 + 0x90)) ||
             (*(longlong *)
               (*(longlong *)(*(longlong *)(lVar17 + 0x10) + 0x88) +
               (longlong)*(int *)(lVar18 + 0x90) * 8) != lVar18 + 0x88)) {
            bVar2 = false;
          }
          else {
            bVar2 = true;
          }
          if (bVar2) {
            *(longlong *)(param_1 + 0x590) = lVar17;
          }
        }
      }
      if (local_d8[0] != 0) {
        FUN_141de9200();
      }
      if (local_b0[0] != 0) {
        FUN_141de9200();
      }
    }
    goto LAB_141554828;
  }
  iVar5 = (int)plVar14[0xb];
  goto joined_r0x0001415542ca;
}



// ---- FUN_14169ba90 @ 14169ba90 ----

void FUN_14169ba90(longlong *param_1,undefined8 param_2,char param_3,char param_4)

{
  code *pcVar1;
  longlong *plVar2;
  void *pvVar3;
  char cVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  longlong *plVar7;
  undefined4 uVar8;
  undefined7 uVar9;
  int iVar10;
  int iVar11;
  longlong lVar12;
  undefined1 local_res8 [8];
  undefined4 in_stack_ffffffffffffff68;
  undefined2 uVar13;
  void *local_88;
  ulonglong local_80;
  longlong local_78 [2];
  void *local_68;
  int local_60;
  undefined4 uStack_5c;
  void *local_58;
  ulonglong local_50;
  void *local_48;
  ulonglong local_40;
  undefined1 local_38 [16];
  
  uVar13 = (undefined2)((uint)in_stack_ffffffffffffff68 >> 0x10);
  FUN_141678320(param_1 + 0x89);
  if (param_3 != '\0') {
    pcVar1 = *(code **)(*param_1 + 0x2e0);
    if (param_4 == '\0') {
      (*pcVar1)(param_1,param_1[0x7f],0,1,0,DAT_1450d6c88);
      return;
    }
    (*pcVar1)(param_1,param_1[0x80],pcVar1,1,0,DAT_1450d6c88);
    return;
  }
  lVar12 = param_1[0x8a];
  puVar5 = (undefined8 *)FUN_141eb3410(local_res8,"Texture",1);
  uVar6 = FUN_1429a9b10(param_1[0xd7]);
  FUN_142e31bc0(uVar6,*puVar5,lVar12);
  local_80 = 10;
  local_88 = (void *)0x0;
  FUN_1407e2ee0(&local_88,0);
  uVar8 = CONCAT22(uVar13,0x3f);
  FUN_1407e4a20(local_88,10,"MSG_Level",10,uVar8);
  uVar13 = (undefined2)((uint)uVar8 >> 0x10);
  FUN_1412b5c20(&local_48,&local_88);
  if (local_88 != (void *)0x0) {
    FUN_141de9200();
  }
  pvVar3 = local_48;
  local_88 = (void *)0x0;
  iVar10 = (int)local_40;
  local_80 = CONCAT44(local_80._4_4_,iVar10);
  if (iVar10 == 0) {
    local_80 = local_40 & 0xffffffff;
  }
  else {
    FUN_1407f1420(&local_88,local_40 & 0xffffffff,0);
    memcpy(local_88,pvVar3,(longlong)iVar10 * 2);
  }
  uVar6 = FUN_1416eaed0(&local_58,(int)param_1[0x8b],&local_88,0);
  FUN_1416fb690(param_1[0xd8],uVar6,0);
  if (local_58 != (void *)0x0) {
    FUN_141de9200();
  }
  FUN_1416fb690(param_1[0xd9],param_1 + 0x8c,0);
  local_78[0] = 0;
  local_78[1] = 0;
  cVar4 = FUN_141545080(param_1[0x8e],local_78,local_38);
  plVar7 = (longlong *)param_1[0xdc];
  pcVar1 = *(code **)(*plVar7 + 0x250);
  uVar9 = (undefined7)((ulonglong)*plVar7 >> 8);
  if (cVar4 == '\0') {
    (*pcVar1)(plVar7,CONCAT71(uVar9,1));
  }
  else {
    (*pcVar1)(plVar7,CONCAT71(uVar9,3));
    FUN_141a881a0(param_1[0xdc],local_38);
    FUN_1416fb690(param_1[0xdc],local_78,0);
    uVar13 = 0;
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x82],0,0,0,DAT_1450d6c88);
  }
  FUN_141a881a0(param_1[0xdb],local_38);
  FUN_1416fb690(param_1[0xdb],local_78,0);
  FUN_1416fb640(param_1[0xe9],(char)param_1[0x99]);
  local_88 = (void *)0x0;
  local_80 = 0xc;
  if ((char)param_1[0xf1] == '\0') {
    FUN_1407e2ee0(&local_88,0);
    FUN_1407e4a20(local_88,0xc,"MSG_Support",0xc,CONCAT22(uVar13,0x3f));
    FUN_1412b5c20(&local_58,&local_88);
    if (local_88 != (void *)0x0) {
      FUN_141de9200();
    }
    pvVar3 = local_58;
    iVar10 = *(int *)((longlong)param_1 + 0x4fc);
    if (iVar10 == 0) {
      (**(code **)(*(longlong *)param_1[0xe1] + 0x250))((longlong *)param_1[0xe1],3);
      (**(code **)(*(longlong *)param_1[0xe3] + 0x250))((longlong *)param_1[0xe3],1);
      uVar6 = 3;
LAB_14169c0f8:
      plVar7 = (longlong *)param_1[0xbc];
LAB_14169c0ff:
      (**(code **)(*plVar7 + 0x250))(plVar7,uVar6);
    }
    else {
      iVar11 = (int)local_50;
      if (iVar10 == 1) {
        local_68 = (void *)0x0;
        if (iVar11 == 0) {
          uStack_5c = 0;
          local_60 = iVar11;
        }
        else {
          local_60 = iVar11;
          FUN_1407f1420(&local_68,local_50 & 0xffffffff,0);
          memcpy(local_68,pvVar3,(longlong)iVar11 * 2);
        }
        uVar6 = FUN_1416eaed0(&local_88,1,&local_68);
        FUN_1416fb690(param_1[0xe4],uVar6,0);
        if (local_88 != (void *)0x0) {
          FUN_141de9200();
        }
        (**(code **)(*(longlong *)param_1[0xe1] + 0x250))((longlong *)param_1[0xe1],1);
        (**(code **)(*(longlong *)param_1[0xe3] + 0x250))((longlong *)param_1[0xe3],3);
        uVar6 = 1;
        goto LAB_14169c0f8;
      }
      if (iVar10 == 2) {
        local_68 = (void *)0x0;
        if (iVar11 == 0) {
          uStack_5c = 0;
          local_60 = iVar11;
        }
        else {
          local_60 = iVar11;
          FUN_1407f1420(&local_68,local_50 & 0xffffffff,0);
          memcpy(local_68,pvVar3,(longlong)iVar11 * 2);
        }
        uVar6 = FUN_1416eaed0(&local_88,2,&local_68);
        FUN_1416fb690(param_1[0xe4],uVar6,0);
        if (local_88 != (void *)0x0) {
          FUN_141de9200();
        }
        (**(code **)(*(longlong *)param_1[0xe1] + 0x250))((longlong *)param_1[0xe1],1);
        plVar7 = (longlong *)param_1[0xe3];
        uVar6 = 3;
        goto LAB_14169c0ff;
      }
      if (iVar10 == 3) {
        (**(code **)(*(longlong *)param_1[0xe1] + 0x250))((longlong *)param_1[0xe1],1);
        (**(code **)(*(longlong *)param_1[0xe3] + 0x250))((longlong *)param_1[0xe3],1);
        uVar6 = 1;
        goto LAB_14169c0f8;
      }
    }
    if ((int)param_1[0x89] - 3U < 2) {
      (**(code **)(*(longlong *)param_1[0xa3] + 0x250))((longlong *)param_1[0xa3],1);
      (**(code **)(*(longlong *)param_1[0xa9] + 0x250))((longlong *)param_1[0xa9],3);
      cVar4 = FUN_140d2f3c0(*(undefined4 *)((longlong)param_1 + 0x44c));
      if ((cVar4 == '\0') &&
         (cVar4 = FUN_140d2fe90(*(undefined4 *)((longlong)param_1 + 0x44c)), cVar4 != '\0')) {
        (**(code **)(*(longlong *)param_1[0xed] + 0x250))((longlong *)param_1[0xed],3);
        (**(code **)(*(longlong *)param_1[0xbc] + 0x250))((longlong *)param_1[0xbc],1);
        if (param_1[0x85] != 0) {
          FUN_141698ad0(param_1[0x85],0);
        }
        if (param_1[0x86] != 0) {
          FUN_141698ad0(param_1[0x86],0);
        }
        if (4 < (int)param_1[0x88]) {
          plVar7 = (longlong *)param_1[0x87];
          lVar12 = *plVar7;
          if (lVar12 != 0) {
            plVar7 = *(longlong **)(lVar12 + 0x28);
            if (plVar7 != (longlong *)0x0) {
              (**(code **)(*plVar7 + 0x250))(plVar7,3);
              lVar12 = *(longlong *)param_1[0x87];
            }
            if (*(longlong *)(lVar12 + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(lVar12 + 0x30),0,0);
              lVar12 = *(longlong *)param_1[0x87];
            }
            FUN_14169a8e0(lVar12,(int)param_1[0x91],*(undefined4 *)((longlong)param_1 + 0x4b4),
                          *(undefined1 *)((longlong)param_1 + 0x505),
                          *(undefined1 *)((longlong)param_1 + 0x511));
            plVar7 = (longlong *)param_1[0x87];
          }
          if (plVar7[1] != 0) {
            plVar2 = *(longlong **)(plVar7[1] + 0x28);
            if (plVar2 != (longlong *)0x0) {
              (**(code **)(*plVar2 + 0x250))(plVar2,3);
              plVar7 = (longlong *)param_1[0x87];
            }
            if (*(longlong *)(plVar7[1] + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(plVar7[1] + 0x30),1);
              plVar7 = (longlong *)param_1[0x87];
            }
            FUN_14169a8e0(plVar7[1],(int)param_1[0x92],*(undefined4 *)((longlong)param_1 + 0x4bc),
                          *(undefined1 *)((longlong)param_1 + 0x507),
                          *(undefined1 *)((longlong)param_1 + 0x513));
            plVar7 = (longlong *)param_1[0x87];
          }
          if (plVar7[2] != 0) {
            plVar2 = *(longlong **)(plVar7[2] + 0x28);
            if (plVar2 != (longlong *)0x0) {
              (**(code **)(*plVar2 + 0x250))(plVar2,3);
              plVar7 = (longlong *)param_1[0x87];
            }
            if (*(longlong *)(plVar7[2] + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(plVar7[2] + 0x30),4);
              plVar7 = (longlong *)param_1[0x87];
            }
            FUN_14169aab0(plVar7[2]);
            plVar7 = (longlong *)param_1[0x87];
          }
          if ((plVar7[3] != 0) &&
             (plVar2 = *(longlong **)(plVar7[3] + 0x28), plVar2 != (longlong *)0x0)) {
            (**(code **)(*plVar2 + 0x250))(plVar2,1);
            plVar7 = (longlong *)param_1[0x87];
          }
          if ((plVar7[4] != 0) &&
             (plVar7 = *(longlong **)(plVar7[4] + 0x28), plVar7 != (longlong *)0x0)) {
            (**(code **)(*plVar7 + 0x250))(plVar7,1);
          }
        }
      }
      else {
        (**(code **)(*(longlong *)param_1[0xed] + 0x250))((longlong *)param_1[0xed],1);
        (**(code **)(*(longlong *)param_1[0xbc] + 0x250))((longlong *)param_1[0xbc],3);
        if (param_1[0x85] != 0) {
          FUN_141698ad0(param_1[0x85],1);
          iVar10 = *(int *)((longlong)param_1 + 0x47c);
          lVar12 = param_1[0x85];
          *(undefined1 *)(lVar12 + 0x4c) = 0;
          *(float *)(lVar12 + 0x2c) = (float)iVar10;
          if (*(int *)(lVar12 + 0x40) != 0) {
            *(float *)(lVar12 + 0x38) = (float)iVar10 / (float)*(int *)(lVar12 + 0x40);
          }
          FUN_1416985b0(param_1[0x85],(int)param_1[0x8f],*(undefined4 *)((longlong)param_1 + 0x4ac))
          ;
        }
        if (param_1[0x86] != 0) {
          FUN_141698ad0(param_1[0x86],1);
          iVar10 = *(int *)((longlong)param_1 + 0x484);
          lVar12 = param_1[0x86];
          *(undefined1 *)(lVar12 + 0x4c) = 0;
          *(float *)(lVar12 + 0x2c) = (float)iVar10;
          if (*(int *)(lVar12 + 0x40) != 0) {
            *(float *)(lVar12 + 0x38) = (float)iVar10 / (float)*(int *)(lVar12 + 0x40);
          }
          FUN_1416985b0(param_1[0x86],(int)param_1[0x90],(int)param_1[0x96]);
        }
        if (4 < (int)param_1[0x88]) {
          plVar7 = (longlong *)param_1[0x87];
          lVar12 = *plVar7;
          if (lVar12 != 0) {
            plVar7 = *(longlong **)(lVar12 + 0x28);
            if (plVar7 != (longlong *)0x0) {
              (**(code **)(*plVar7 + 0x250))(plVar7,3);
              lVar12 = *(longlong *)param_1[0x87];
            }
            if (*(longlong *)(lVar12 + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(lVar12 + 0x30),0,0);
              lVar12 = *(longlong *)param_1[0x87];
            }
            FUN_14169a8e0(lVar12,(int)param_1[0x91],*(undefined4 *)((longlong)param_1 + 0x4b4),
                          *(undefined1 *)((longlong)param_1 + 0x505),
                          *(undefined1 *)((longlong)param_1 + 0x511));
            plVar7 = (longlong *)param_1[0x87];
          }
          if (plVar7[1] != 0) {
            plVar2 = *(longlong **)(plVar7[1] + 0x28);
            if (plVar2 != (longlong *)0x0) {
              (**(code **)(*plVar2 + 0x250))(plVar2,3);
              plVar7 = (longlong *)param_1[0x87];
            }
            if (*(longlong *)(plVar7[1] + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(plVar7[1] + 0x30),2);
              plVar7 = (longlong *)param_1[0x87];
            }
            FUN_14169a8e0(plVar7[1],*(undefined4 *)((longlong)param_1 + 0x48c),(int)param_1[0x97],
                          *(undefined1 *)((longlong)param_1 + 0x506),
                          *(undefined1 *)((longlong)param_1 + 0x512));
            plVar7 = (longlong *)param_1[0x87];
          }
          if (plVar7[2] != 0) {
            plVar2 = *(longlong **)(plVar7[2] + 0x28);
            if (plVar2 != (longlong *)0x0) {
              (**(code **)(*plVar2 + 0x250))(plVar2,3);
              plVar7 = (longlong *)param_1[0x87];
            }
            if (*(longlong *)(plVar7[2] + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(plVar7[2] + 0x30),1);
              plVar7 = (longlong *)param_1[0x87];
            }
            FUN_14169a8e0(plVar7[2],(int)param_1[0x92],*(undefined4 *)((longlong)param_1 + 0x4bc),
                          *(undefined1 *)((longlong)param_1 + 0x507),
                          *(undefined1 *)((longlong)param_1 + 0x513));
            plVar7 = (longlong *)param_1[0x87];
          }
          if (plVar7[3] != 0) {
            plVar2 = *(longlong **)(plVar7[3] + 0x28);
            if (plVar2 != (longlong *)0x0) {
              (**(code **)(*plVar2 + 0x250))(plVar2,3);
              plVar7 = (longlong *)param_1[0x87];
            }
            if (*(longlong *)(plVar7[3] + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(plVar7[3] + 0x30),3);
              plVar7 = (longlong *)param_1[0x87];
            }
            FUN_14169a8e0(plVar7[3],*(undefined4 *)((longlong)param_1 + 0x494),(int)param_1[0x98],
                          (char)param_1[0xa1],*(undefined1 *)((longlong)param_1 + 0x514));
            plVar7 = (longlong *)param_1[0x87];
          }
          if (plVar7[4] != 0) {
            plVar2 = *(longlong **)(plVar7[4] + 0x28);
            if (plVar2 != (longlong *)0x0) {
              (**(code **)(*plVar2 + 0x250))(plVar2,3);
              plVar7 = (longlong *)param_1[0x87];
            }
            if (*(longlong *)(plVar7[4] + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(plVar7[4] + 0x30),4);
              plVar7 = (longlong *)param_1[0x87];
            }
            FUN_14169aab0(plVar7[4]);
          }
        }
      }
    }
    else {
      (**(code **)(*(longlong *)param_1[0xed] + 0x250))();
      (**(code **)(*(longlong *)param_1[0xa3] + 0x250))((longlong *)param_1[0xa3],3);
      (**(code **)(*(longlong *)param_1[0xa9] + 0x250))((longlong *)param_1[0xa9],1);
      lVar12 = param_1[0x83];
      if (lVar12 != 0) {
        iVar10 = *(int *)((longlong)param_1 + 0x47c);
        *(undefined1 *)(lVar12 + 0x4c) = 1;
        *(float *)(lVar12 + 0x2c) = (float)iVar10;
        if (*(int *)(lVar12 + 0x40) != 0) {
          *(float *)(lVar12 + 0x38) = (float)iVar10 / (float)*(int *)(lVar12 + 0x40);
        }
        FUN_1416985b0(param_1[0x83],(int)param_1[0x8f],*(undefined4 *)((longlong)param_1 + 0x4ac));
      }
      lVar12 = param_1[0x84];
      if (lVar12 != 0) {
        iVar10 = *(int *)((longlong)param_1 + 0x484);
        *(undefined1 *)(lVar12 + 0x4c) = 1;
        *(float *)(lVar12 + 0x2c) = (float)iVar10;
        if (*(int *)(lVar12 + 0x40) != 0) {
          *(float *)(lVar12 + 0x38) = (float)iVar10 / (float)*(int *)(lVar12 + 0x40);
        }
        FUN_1416985b0(param_1[0x84],(int)param_1[0x90],(int)param_1[0x96]);
      }
    }
  }
  else {
    FUN_1407e2ee0(&local_88,0);
    FUN_1407e4a20(local_88,0xc,"MSG_Support",0xc,CONCAT22(uVar13,0x3f));
    FUN_1412b5c20(&local_68,&local_88);
    if (local_88 != (void *)0x0) {
      FUN_141de9200();
    }
    iVar11 = local_60;
    pvVar3 = local_68;
    iVar10 = *(int *)((longlong)param_1 + 0x4fc);
    if (iVar10 == 0) {
LAB_14169bd7d:
      uVar6 = 1;
LAB_14169bd7f:
      (**(code **)(*(longlong *)param_1[0xe3] + 0x250))((longlong *)param_1[0xe3],uVar6);
    }
    else {
      if (iVar10 == 1) {
        local_88 = (void *)0x0;
        local_80 = CONCAT44(local_80._4_4_,local_60);
        if (local_60 == 0) {
          local_80 = 0;
          uVar8 = 1;
        }
        else {
          FUN_1407f1420(&local_88,local_60,0);
          memcpy(local_88,pvVar3,(longlong)iVar11 * 2);
          uVar8 = 1;
        }
LAB_14169be1d:
        uVar6 = FUN_1416eaed0(&local_58,uVar8,&local_88,0);
        FUN_1416fb690(param_1[0xe4],uVar6,0);
        if (local_58 != (void *)0x0) {
          FUN_141de9200();
        }
        uVar6 = 3;
        goto LAB_14169bd7f;
      }
      if (iVar10 == 2) {
        local_88 = (void *)0x0;
        local_80 = CONCAT44(local_80._4_4_,local_60);
        if (local_60 == 0) {
          local_80 = 0;
        }
        else {
          FUN_1407f1420(&local_88,local_60,0);
          memcpy(local_88,pvVar3,(longlong)iVar11 * 2);
        }
        uVar8 = 2;
        goto LAB_14169be1d;
      }
      if (iVar10 == 3) goto LAB_14169bd7d;
    }
    plVar7 = (longlong *)param_1[0xe5];
    if (*(char *)((longlong)param_1 + 0x4c9) == '\0') {
      (**(code **)(*plVar7 + 0x250))(plVar7,1);
      uVar6 = 1;
    }
    else {
      FUN_1416f5900(plVar7,*(char *)((longlong)param_1 + 0x4c9),(int)param_1[0x9c]);
      FUN_1416fb690(param_1[0xe7],param_1 + 0x9a,1);
      (**(code **)(*(longlong *)param_1[0xe5] + 0x250))((longlong *)param_1[0xe5],3);
      uVar6 = 3;
    }
    (**(code **)(*(longlong *)param_1[0xe7] + 0x250))((longlong *)param_1[0xe7],uVar6);
    plVar7 = (longlong *)param_1[0xe6];
    if (*(char *)((longlong)param_1 + 0x4e4) == '\0') {
      (**(code **)(*plVar7 + 0x250))(plVar7,1);
      uVar6 = 1;
    }
    else {
      FUN_1416f5900(plVar7,*(char *)((longlong)param_1 + 0x4e4),(int)param_1[0x9f]);
      FUN_1416fb690(param_1[0xe8],param_1 + 0x9d,1);
      (**(code **)(*(longlong *)param_1[0xe6] + 0x250))((longlong *)param_1[0xe6],3);
      uVar6 = 3;
    }
    (**(code **)(*(longlong *)param_1[0xe8] + 0x250))((longlong *)param_1[0xe8],uVar6);
    local_58 = local_68;
  }
  if (local_58 != (void *)0x0) {
    FUN_141de9200();
  }
  iVar10 = (int)param_1[0x89];
  if (iVar10 == 0) {
LAB_14169c7dd:
    (**(code **)(*(longlong *)param_1[0xdf] + 0x250))((longlong *)param_1[0xdf],3);
    uVar6 = 4;
  }
  else {
    if (iVar10 != 1) {
      if (((iVar10 != 2) && (iVar10 != 3)) && (iVar10 != 4)) goto LAB_14169c801;
      goto LAB_14169c7dd;
    }
    (**(code **)(*(longlong *)param_1[0xdf] + 0x250))((longlong *)param_1[0xdf],1);
    uVar6 = 1;
  }
  (**(code **)(*(longlong *)param_1[0xe0] + 0x250))((longlong *)param_1[0xe0],uVar6);
LAB_14169c801:
  FUN_14169e920(param_1,(int)param_1[0x89]);
  if (local_78[0] != 0) {
    FUN_141de9200();
  }
  if (local_48 != (void *)0x0) {
    FUN_141de9200();
  }
  return;
}



// ---- FUN_14169c850 @ 14169c850 ----

void FUN_14169c850(longlong *param_1,undefined8 param_2,char param_3,char param_4)

{
  longlong *plVar1;
  code *pcVar2;
  longlong *plVar3;
  void *pvVar4;
  char cVar5;
  int iVar6;
  undefined8 uVar7;
  undefined8 *puVar8;
  longlong *plVar9;
  undefined4 uVar10;
  char *pcVar11;
  int iVar12;
  longlong lVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  undefined1 local_res8 [8];
  undefined4 in_stack_ffffffffffffff58;
  undefined2 uVar17;
  void *local_98;
  ulonglong local_90;
  void *local_88;
  ulonglong local_80;
  void *local_78;
  ulonglong local_70;
  undefined1 local_68 [16];
  longlong local_58 [6];
  
  uVar17 = (undefined2)((uint)in_stack_ffffffffffffff58 >> 0x10);
  plVar1 = param_1 + 0x89;
  FUN_141678320(plVar1);
  if (param_3 != '\0') {
    pcVar2 = *(code **)(*param_1 + 0x2e0);
    if (param_4 != '\0') {
      (*pcVar2)(param_1,param_1[0x80],pcVar2,1,0,DAT_1450d6c88);
      return;
    }
    (*pcVar2)(param_1,param_1[0x7f],0,1,0,DAT_1450d6c88);
    return;
  }
  plVar9 = (longlong *)param_1[0xd8];
  if ((int)*plVar1 - 5U < 2) {
    (**(code **)(*plVar9 + 0x250))(plVar9,1);
    (**(code **)(*(longlong *)param_1[0xd9] + 0x250))((longlong *)param_1[0xd9],1);
    (**(code **)(*(longlong *)param_1[0xdb] + 0x250))((longlong *)param_1[0xdb],1);
    (**(code **)(*(longlong *)param_1[0xda] + 0x250))((longlong *)param_1[0xda],1);
    (**(code **)(*(longlong *)param_1[0xf0] + 0x250))((longlong *)param_1[0xf0],1);
    (**(code **)(*(longlong *)param_1[0xf9] + 0x250))((longlong *)param_1[0xf9],3);
    FUN_1416fb690(param_1[0xfb],param_1 + 0x8c,0);
    local_80 = 10;
    local_88 = (void *)0x0;
    FUN_1407e2ee0(&local_88,0);
    uVar10 = CONCAT22(uVar17,0x3f);
    FUN_1407e4a20(local_88,10,"MSG_Level",10,uVar10);
    uVar17 = (undefined2)((uint)uVar10 >> 0x10);
    FUN_1412b5c20(&local_78,&local_88);
    if (local_88 != (void *)0x0) {
      FUN_141de9200();
    }
    pvVar4 = local_78;
    local_88 = (void *)0x0;
    iVar12 = (int)local_70;
    local_80 = CONCAT44(local_80._4_4_,iVar12);
    if (iVar12 == 0) {
      local_80 = local_70 & 0xffffffff;
    }
    else {
      FUN_1407f1420(&local_88,local_70 & 0xffffffff,0);
      memcpy(local_88,pvVar4,(longlong)iVar12 * 2);
    }
    uVar7 = FUN_1416eaed0(local_58,(int)param_1[0x8b],&local_88,0);
    FUN_1416fb690(param_1[0xfe],uVar7,0);
    if (local_58[0] != 0) {
      FUN_141de9200();
    }
    plVar9 = (longlong *)param_1[0x103];
    if ((int)param_1[0x8b] < *(int *)((longlong)param_1 + 0x49c)) {
      if ((int)*plVar1 == 6) {
        *(undefined1 *)(param_1 + 0x107) = 0;
        (**(code **)(*plVar9 + 0x250))(plVar9,1);
        FUN_141a881a0(param_1[0xff],&DAT_1456d9c40);
        uVar10 = (undefined4)param_1[0x8b];
        goto LAB_14169cda3;
      }
      *(undefined1 *)(param_1 + 0x107) = 1;
      (**(code **)(*plVar9 + 0x250))(plVar9,3);
      FUN_141a881a0(param_1[0xff],param_1 + 0x105);
      local_88 = (void *)0x0;
      local_80 = 0;
      FUN_141db3e50(&local_88,*(undefined4 *)((longlong)param_1 + 0x49c));
      FUN_1416fb690(param_1[0xff],&local_88,0);
      if (local_88 != (void *)0x0) {
        FUN_141de9200();
      }
      iVar12 = 0;
    }
    else {
      *(undefined1 *)(param_1 + 0x107) = 0;
      (**(code **)(*plVar9 + 0x250))(plVar9,1);
      FUN_141a881a0(param_1[0xff],&DAT_1456d9c40);
      uVar10 = *(undefined4 *)((longlong)param_1 + 0x49c);
LAB_14169cda3:
      local_88 = (void *)0x0;
      local_80 = 0;
      FUN_141db3e50(&local_88,uVar10);
      FUN_1416fb690(param_1[0xff],&local_88,0);
      if (local_88 != (void *)0x0) {
        FUN_141de9200();
      }
      iVar6 = (int)param_1[0x94] - (int)param_1[0x95];
      iVar12 = 0;
      if (-1 < iVar6) {
        iVar12 = iVar6;
      }
    }
    local_88 = (void *)0x0;
    local_80 = 0;
    FUN_141db3e50(&local_88,iVar12);
    FUN_1416fb690(param_1[0xfd],&local_88,0);
    if (local_88 != (void *)0x0) {
      FUN_141de9200();
    }
    fVar16 = DAT_1450d6c88;
    fVar15 = 0.0;
    if (*(int *)((longlong)param_1 + 0x4a4) < 1) {
      if (*(char *)((longlong)param_1 + 0x839) == '\0') {
        puVar8 = (undefined8 *)FUN_141eb3410(local_res8,"Gauge_Skin1",1);
        FUN_142e31a70(param_1[0x102],*puVar8,fVar16);
        puVar8 = (undefined8 *)FUN_141eb3410(local_res8,"Gauge_Skin3",1);
        FUN_142e31a70(param_1[0x102],*puVar8,0);
        pcVar11 = "Gauge_Skin2";
      }
      else {
        *(undefined4 *)(param_1 + 0x109) = 0;
        pcVar11 = "Gauge_Skin2";
      }
    }
    else {
      fVar15 = (float)*(int *)((longlong)param_1 + 0x4a4);
      fVar14 = fVar15 - (float)(int)param_1[0x94];
      fVar15 = DAT_1450d6c88 / fVar15;
      fVar16 = fVar14 * fVar15;
      fVar15 = ((float)(int)param_1[0x95] + fVar14) * fVar15;
      if (DAT_1450d6c88 <= fVar16) {
        fVar16 = DAT_1450d6c88;
      }
      if (DAT_1450d6c88 <= fVar15) {
        fVar15 = DAT_1450d6c88;
      }
      if (*(char *)((longlong)param_1 + 0x839) == '\0') {
        puVar8 = (undefined8 *)FUN_141eb3410(local_res8,"Gauge_Skin1",1);
        FUN_142e31a70(param_1[0x102],*puVar8,fVar16);
        puVar8 = (undefined8 *)FUN_141eb3410(local_res8,"Gauge_Skin3",1);
        FUN_142e31a70(param_1[0x102],*puVar8,fVar15);
      }
      else {
        *(float *)(param_1 + 0x109) = fVar15;
      }
      pcVar11 = "Gauge_Skin2";
    }
    puVar8 = (undefined8 *)FUN_141eb3410(local_res8,pcVar11,1);
    FUN_142e31a70(param_1[0x102],*puVar8,0);
    if (*(char *)((longlong)param_1 + 0x839) == '\0') {
      *(float *)((longlong)param_1 + 0x83c) = fVar16;
      *(float *)(param_1 + 0x108) = fVar15;
    }
    else {
      if (fVar16 < *(float *)((longlong)param_1 + 0x83c)) {
        *(undefined4 *)((longlong)param_1 + 0x83c) = 0;
      }
      *(float *)(param_1 + 0x108) = fVar16;
      *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | 8;
      *(undefined4 *)((longlong)param_1 + 0x844) = *(undefined4 *)((longlong)param_1 + 0x83c);
    }
  }
  else {
    (**(code **)(*plVar9 + 0x250))(plVar9,3);
    (**(code **)(*(longlong *)param_1[0xd9] + 0x250))((longlong *)param_1[0xd9],3);
    (**(code **)(*(longlong *)param_1[0xdb] + 0x250))((longlong *)param_1[0xdb],3);
    (**(code **)(*(longlong *)param_1[0xda] + 0x250))((longlong *)param_1[0xda],3);
    (**(code **)(*(longlong *)param_1[0xf0] + 0x250))((longlong *)param_1[0xf0],3);
    (**(code **)(*(longlong *)param_1[0xf9] + 0x250))((longlong *)param_1[0xf9],1);
    local_90 = 0xd;
    local_98 = (void *)0x0;
    FUN_1407e2ee0(&local_98,0);
    uVar10 = CONCAT22(uVar17,0x3f);
    FUN_1407e4a20(local_98,0xd,"MSG_PowLevel",0xd,uVar10);
    uVar17 = (undefined2)((uint)uVar10 >> 0x10);
    uVar7 = FUN_1412b5c20(local_58,&local_98);
    FUN_1416fb690(param_1[0xda],uVar7,0);
    if (local_58[0] != 0) {
      FUN_141de9200();
    }
    if (local_98 != (void *)0x0) {
      FUN_141de9200();
    }
    local_98 = (void *)0x0;
    local_90 = 10;
    FUN_1407e2ee0(&local_98,0);
    uVar10 = CONCAT22(uVar17,0x3f);
    FUN_1407e4a20(local_98,10,"MSG_Level",10,uVar10);
    uVar17 = (undefined2)((uint)uVar10 >> 0x10);
    FUN_1412b5c20(&local_78,&local_98);
    if (local_98 != (void *)0x0) {
      FUN_141de9200();
    }
    pvVar4 = local_78;
    local_98 = (void *)0x0;
    iVar12 = (int)local_70;
    local_90 = CONCAT44(local_90._4_4_,iVar12);
    if (iVar12 == 0) {
      local_90 = local_70 & 0xffffffff;
    }
    else {
      FUN_1407f1420(&local_98,local_70 & 0xffffffff,0);
      memcpy(local_98,pvVar4,(longlong)iVar12 * 2);
    }
    uVar7 = FUN_1416eaed0(local_58,(int)param_1[0x8b],&local_98,0);
    FUN_1416fb690(param_1[0xd8],uVar7,0);
    if (local_58[0] != 0) {
      FUN_141de9200();
    }
    FUN_1416fb690(param_1[0xd9],param_1 + 0x8c,0);
    FUN_141a825c0(param_1[0xdb],0);
    FUN_141a54c00(param_1[0xdb]);
    local_88 = (void *)0x0;
    local_80 = 0;
    cVar5 = FUN_141545080(param_1[0x8e],&local_88,local_68);
    plVar9 = (longlong *)param_1[0xdc];
    if (cVar5 == '\0') {
      (**(code **)(*plVar9 + 0x250))(plVar9,1);
    }
    else {
      (**(code **)(*plVar9 + 0x250))(plVar9,3);
      FUN_141a881a0(param_1[0xdc],local_68);
      FUN_1416fb690(param_1[0xdc],&local_88,0);
      uVar17 = 0;
      (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x82],0,0,0,DAT_1450d6c88);
    }
    FUN_141a881a0(param_1[0xdb],local_68);
    FUN_1416fb690(param_1[0xdb],&local_88,0);
    if (local_88 != (void *)0x0) {
      FUN_141de9200();
    }
  }
  if (local_78 != (void *)0x0) {
    FUN_141de9200();
  }
  lVar13 = param_1[0x8a];
  puVar8 = (undefined8 *)FUN_141eb3410(local_res8,"Texture",1);
  uVar7 = FUN_1429a9b10(param_1[0xd7]);
  FUN_142e31bc0(uVar7,*puVar8,lVar13);
  FUN_1416fb640(param_1[0xe9]);
  local_88 = (void *)0x0;
  local_80 = 0xc;
  if ((char)param_1[0xf1] == '\0') {
    FUN_1407e2ee0(&local_88);
    FUN_1407e4a20(local_88,0xc,"MSG_Support",0xc,CONCAT22(uVar17,0x3f));
    FUN_1412b5c20(&local_98,&local_88);
    if (local_88 != (void *)0x0) {
      FUN_141de9200();
    }
    pvVar4 = local_98;
    iVar12 = *(int *)((longlong)param_1 + 0x4fc);
    if (iVar12 == 0) {
      (**(code **)(*(longlong *)param_1[0xe1] + 0x250))((longlong *)param_1[0xe1],3);
      (**(code **)(*(longlong *)param_1[0xe3] + 0x250))((longlong *)param_1[0xe3],1);
      uVar7 = 3;
LAB_14169d42b:
      plVar9 = (longlong *)param_1[0xbc];
LAB_14169d432:
      (**(code **)(*plVar9 + 0x250))(plVar9,uVar7);
    }
    else {
      iVar6 = (int)local_90;
      if (iVar12 == 1) {
        local_88 = (void *)0x0;
        local_80 = CONCAT44(local_80._4_4_,iVar6);
        if (iVar6 == 0) {
          local_80 = local_90 & 0xffffffff;
        }
        else {
          FUN_1407f1420(&local_88,local_90 & 0xffffffff,0);
          memcpy(local_88,pvVar4,(longlong)iVar6 * 2);
        }
        uVar7 = FUN_1416eaed0(local_58,1,&local_88);
        FUN_1416fb690(param_1[0xe4],uVar7,0);
        if (local_58[0] != 0) {
          FUN_141de9200();
        }
        (**(code **)(*(longlong *)param_1[0xe1] + 0x250))((longlong *)param_1[0xe1],1);
        (**(code **)(*(longlong *)param_1[0xe3] + 0x250))((longlong *)param_1[0xe3],3);
        uVar7 = 1;
        goto LAB_14169d42b;
      }
      if (iVar12 == 2) {
        local_88 = (void *)0x0;
        local_80 = CONCAT44(local_80._4_4_,iVar6);
        if (iVar6 == 0) {
          local_80 = local_90 & 0xffffffff;
        }
        else {
          FUN_1407f1420(&local_88,local_90 & 0xffffffff,0);
          memcpy(local_88,pvVar4,(longlong)iVar6 * 2);
        }
        uVar7 = FUN_1416eaed0(local_58,2,&local_88);
        FUN_1416fb690(param_1[0xe4],uVar7,0);
        if (local_58[0] != 0) {
          FUN_141de9200();
        }
        (**(code **)(*(longlong *)param_1[0xe1] + 0x250))((longlong *)param_1[0xe1],1);
        plVar9 = (longlong *)param_1[0xe3];
        uVar7 = 3;
        goto LAB_14169d432;
      }
      if (iVar12 == 3) {
        (**(code **)(*(longlong *)param_1[0xe1] + 0x250))((longlong *)param_1[0xe1],1);
        (**(code **)(*(longlong *)param_1[0xe3] + 0x250))((longlong *)param_1[0xe3],1);
        uVar7 = 1;
        goto LAB_14169d42b;
      }
    }
    if ((int)*plVar1 - 3U < 2) {
      (**(code **)(*(longlong *)param_1[0xa3] + 0x250))((longlong *)param_1[0xa3],1);
      (**(code **)(*(longlong *)param_1[0xa9] + 0x250))((longlong *)param_1[0xa9],3);
      cVar5 = FUN_140d2f3c0(*(undefined4 *)((longlong)param_1 + 0x44c));
      if ((cVar5 == '\0') &&
         (cVar5 = FUN_140d2fe90(*(undefined4 *)((longlong)param_1 + 0x44c)), cVar5 != '\0')) {
        (**(code **)(*(longlong *)param_1[0xed] + 0x250))((longlong *)param_1[0xed],3);
        (**(code **)(*(longlong *)param_1[0xbc] + 0x250))((longlong *)param_1[0xbc],1);
        if (param_1[0x85] != 0) {
          FUN_141698ad0(param_1[0x85],0);
        }
        if (param_1[0x86] != 0) {
          FUN_141698ad0(param_1[0x86],0);
        }
        if (4 < (int)param_1[0x88]) {
          plVar9 = (longlong *)param_1[0x87];
          lVar13 = *plVar9;
          if (lVar13 != 0) {
            plVar9 = *(longlong **)(lVar13 + 0x28);
            if (plVar9 != (longlong *)0x0) {
              (**(code **)(*plVar9 + 0x250))(plVar9,3);
              lVar13 = *(longlong *)param_1[0x87];
            }
            if (*(longlong *)(lVar13 + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(lVar13 + 0x30),0,0);
              lVar13 = *(longlong *)param_1[0x87];
            }
            FUN_14169a8e0(lVar13,(int)param_1[0x91],*(undefined4 *)((longlong)param_1 + 0x4b4),
                          *(undefined1 *)((longlong)param_1 + 0x505),
                          *(undefined1 *)((longlong)param_1 + 0x511));
            plVar9 = (longlong *)param_1[0x87];
          }
          if (plVar9[1] != 0) {
            plVar3 = *(longlong **)(plVar9[1] + 0x28);
            if (plVar3 != (longlong *)0x0) {
              (**(code **)(*plVar3 + 0x250))(plVar3,3);
              plVar9 = (longlong *)param_1[0x87];
            }
            if (*(longlong *)(plVar9[1] + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(plVar9[1] + 0x30),1);
              plVar9 = (longlong *)param_1[0x87];
            }
            FUN_14169a8e0(plVar9[1],(int)param_1[0x92],*(undefined4 *)((longlong)param_1 + 0x4bc),
                          *(undefined1 *)((longlong)param_1 + 0x507),
                          *(undefined1 *)((longlong)param_1 + 0x513));
            plVar9 = (longlong *)param_1[0x87];
          }
          if (plVar9[2] != 0) {
            plVar3 = *(longlong **)(plVar9[2] + 0x28);
            if (plVar3 != (longlong *)0x0) {
              (**(code **)(*plVar3 + 0x250))(plVar3,3);
              plVar9 = (longlong *)param_1[0x87];
            }
            if (*(longlong *)(plVar9[2] + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(plVar9[2] + 0x30),4);
              plVar9 = (longlong *)param_1[0x87];
            }
            FUN_14169aab0(plVar9[2]);
            plVar9 = (longlong *)param_1[0x87];
          }
          if ((plVar9[3] != 0) &&
             (plVar3 = *(longlong **)(plVar9[3] + 0x28), plVar3 != (longlong *)0x0)) {
            (**(code **)(*plVar3 + 0x250))(plVar3,1);
            plVar9 = (longlong *)param_1[0x87];
          }
          if ((plVar9[4] != 0) &&
             (plVar9 = *(longlong **)(plVar9[4] + 0x28), plVar9 != (longlong *)0x0)) {
            (**(code **)(*plVar9 + 0x250))(plVar9,1);
          }
        }
      }
      else {
        (**(code **)(*(longlong *)param_1[0xed] + 0x250))((longlong *)param_1[0xed],1);
        (**(code **)(*(longlong *)param_1[0xbc] + 0x250))((longlong *)param_1[0xbc],3);
        if (param_1[0x85] != 0) {
          FUN_141698ad0(param_1[0x85],1);
          iVar12 = *(int *)((longlong)param_1 + 0x47c);
          lVar13 = param_1[0x85];
          *(undefined1 *)(lVar13 + 0x4c) = 0;
          *(float *)(lVar13 + 0x2c) = (float)iVar12;
          if (*(int *)(lVar13 + 0x40) != 0) {
            *(float *)(lVar13 + 0x38) = (float)iVar12 / (float)*(int *)(lVar13 + 0x40);
          }
          FUN_1416985b0(param_1[0x85],(int)param_1[0x8f],*(undefined4 *)((longlong)param_1 + 0x4ac))
          ;
        }
        if (param_1[0x86] != 0) {
          FUN_141698ad0(param_1[0x86],1);
          iVar12 = *(int *)((longlong)param_1 + 0x484);
          lVar13 = param_1[0x86];
          *(undefined1 *)(lVar13 + 0x4c) = 0;
          *(float *)(lVar13 + 0x2c) = (float)iVar12;
          if (*(int *)(lVar13 + 0x40) != 0) {
            *(float *)(lVar13 + 0x38) = (float)iVar12 / (float)*(int *)(lVar13 + 0x40);
          }
          FUN_1416985b0(param_1[0x86],(int)param_1[0x90],(int)param_1[0x96]);
        }
        if (4 < (int)param_1[0x88]) {
          plVar9 = (longlong *)param_1[0x87];
          lVar13 = *plVar9;
          if (lVar13 != 0) {
            plVar9 = *(longlong **)(lVar13 + 0x28);
            if (plVar9 != (longlong *)0x0) {
              (**(code **)(*plVar9 + 0x250))(plVar9,3);
              lVar13 = *(longlong *)param_1[0x87];
            }
            if (*(longlong *)(lVar13 + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(lVar13 + 0x30),0,0);
              lVar13 = *(longlong *)param_1[0x87];
            }
            FUN_14169a8e0(lVar13,(int)param_1[0x91],*(undefined4 *)((longlong)param_1 + 0x4b4),
                          *(undefined1 *)((longlong)param_1 + 0x505),
                          *(undefined1 *)((longlong)param_1 + 0x511));
            plVar9 = (longlong *)param_1[0x87];
          }
          if (plVar9[1] != 0) {
            plVar3 = *(longlong **)(plVar9[1] + 0x28);
            if (plVar3 != (longlong *)0x0) {
              (**(code **)(*plVar3 + 0x250))(plVar3,3);
              plVar9 = (longlong *)param_1[0x87];
            }
            if (*(longlong *)(plVar9[1] + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(plVar9[1] + 0x30),2);
              plVar9 = (longlong *)param_1[0x87];
            }
            FUN_14169a8e0(plVar9[1],*(undefined4 *)((longlong)param_1 + 0x48c),(int)param_1[0x97],
                          *(undefined1 *)((longlong)param_1 + 0x506),
                          *(undefined1 *)((longlong)param_1 + 0x512));
            plVar9 = (longlong *)param_1[0x87];
          }
          if (plVar9[2] != 0) {
            plVar3 = *(longlong **)(plVar9[2] + 0x28);
            if (plVar3 != (longlong *)0x0) {
              (**(code **)(*plVar3 + 0x250))(plVar3,3);
              plVar9 = (longlong *)param_1[0x87];
            }
            if (*(longlong *)(plVar9[2] + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(plVar9[2] + 0x30),1);
              plVar9 = (longlong *)param_1[0x87];
            }
            FUN_14169a8e0(plVar9[2],(int)param_1[0x92],*(undefined4 *)((longlong)param_1 + 0x4bc),
                          *(undefined1 *)((longlong)param_1 + 0x507),
                          *(undefined1 *)((longlong)param_1 + 0x513));
            plVar9 = (longlong *)param_1[0x87];
          }
          if (plVar9[3] != 0) {
            plVar3 = *(longlong **)(plVar9[3] + 0x28);
            if (plVar3 != (longlong *)0x0) {
              (**(code **)(*plVar3 + 0x250))(plVar3,3);
              plVar9 = (longlong *)param_1[0x87];
            }
            if (*(longlong *)(plVar9[3] + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(plVar9[3] + 0x30),3);
              plVar9 = (longlong *)param_1[0x87];
            }
            FUN_14169a8e0(plVar9[3],*(undefined4 *)((longlong)param_1 + 0x494),(int)param_1[0x98],
                          (char)param_1[0xa1],*(undefined1 *)((longlong)param_1 + 0x514));
            plVar9 = (longlong *)param_1[0x87];
          }
          if (plVar9[4] != 0) {
            plVar3 = *(longlong **)(plVar9[4] + 0x28);
            if (plVar3 != (longlong *)0x0) {
              (**(code **)(*plVar3 + 0x250))(plVar3,3);
              plVar9 = (longlong *)param_1[0x87];
            }
            if (*(longlong *)(plVar9[4] + 0x30) != 0) {
              FUN_1416f9420(*(longlong *)(plVar9[4] + 0x30),4);
              plVar9 = (longlong *)param_1[0x87];
            }
            FUN_14169aab0(plVar9[4]);
          }
        }
      }
    }
    else {
      (**(code **)(*(longlong *)param_1[0xed] + 0x250))();
      (**(code **)(*(longlong *)param_1[0xa3] + 0x250))((longlong *)param_1[0xa3],3);
      (**(code **)(*(longlong *)param_1[0xa9] + 0x250))((longlong *)param_1[0xa9],1);
      lVar13 = param_1[0x83];
      if (lVar13 != 0) {
        iVar12 = *(int *)((longlong)param_1 + 0x47c);
        *(undefined1 *)(lVar13 + 0x4c) = 1;
        *(float *)(lVar13 + 0x2c) = (float)iVar12;
        if (*(int *)(lVar13 + 0x40) != 0) {
          *(float *)(lVar13 + 0x38) = (float)iVar12 / (float)*(int *)(lVar13 + 0x40);
        }
        FUN_1416985b0(param_1[0x83],(int)param_1[0x8f],*(undefined4 *)((longlong)param_1 + 0x4ac));
      }
      lVar13 = param_1[0x84];
      if (lVar13 != 0) {
        iVar12 = *(int *)((longlong)param_1 + 0x484);
        *(undefined1 *)(lVar13 + 0x4c) = 1;
        *(float *)(lVar13 + 0x2c) = (float)iVar12;
        if (*(int *)(lVar13 + 0x40) != 0) {
          *(float *)(lVar13 + 0x38) = (float)iVar12 / (float)*(int *)(lVar13 + 0x40);
        }
        FUN_1416985b0(param_1[0x84],(int)param_1[0x90],(int)param_1[0x96]);
      }
    }
  }
  else {
    FUN_1407e2ee0(&local_88);
    FUN_1407e4a20(local_88,0xc,"MSG_Support",0xc,CONCAT22(uVar17,0x3f));
    FUN_1412b5c20(&local_98,&local_88);
    if (local_88 != (void *)0x0) {
      FUN_141de9200();
    }
    pvVar4 = local_98;
    iVar12 = *(int *)((longlong)param_1 + 0x4fc);
    if (iVar12 == 0) {
LAB_14169d0b0:
      uVar7 = 1;
LAB_14169d0b2:
      (**(code **)(*(longlong *)param_1[0xe3] + 0x250))((longlong *)param_1[0xe3],uVar7);
    }
    else {
      iVar6 = (int)local_90;
      if (iVar12 == 1) {
        local_88 = (void *)0x0;
        local_80 = CONCAT44(local_80._4_4_,iVar6);
        if (iVar6 == 0) {
          local_80 = local_90 & 0xffffffff;
          uVar10 = 1;
        }
        else {
          FUN_1407f1420(&local_88,local_90 & 0xffffffff,0);
          memcpy(local_88,pvVar4,(longlong)iVar6 * 2);
          uVar10 = 1;
        }
LAB_14169d151:
        uVar7 = FUN_1416eaed0(local_58,uVar10,&local_88,0);
        FUN_1416fb690(param_1[0xe4],uVar7,0);
        if (local_58[0] != 0) {
          FUN_141de9200();
        }
        uVar7 = 3;
        goto LAB_14169d0b2;
      }
      if (iVar12 == 2) {
        local_88 = (void *)0x0;
        local_80 = CONCAT44(local_80._4_4_,iVar6);
        if (iVar6 == 0) {
          local_80 = local_90 & 0xffffffff;
        }
        else {
          FUN_1407f1420(&local_88,local_90 & 0xffffffff,0);
          memcpy(local_88,pvVar4,(longlong)iVar6 * 2);
        }
        uVar10 = 2;
        goto LAB_14169d151;
      }
      if (iVar12 == 3) goto LAB_14169d0b0;
    }
    plVar9 = (longlong *)param_1[0xe5];
    if (*(char *)((longlong)param_1 + 0x4c9) == '\0') {
      (**(code **)(*plVar9 + 0x250))(plVar9,1);
      uVar7 = 1;
    }
    else {
      FUN_1416f5900(plVar9,*(char *)((longlong)param_1 + 0x4c9),(int)param_1[0x9c]);
      FUN_1416fb690(param_1[0xe7],param_1 + 0x9a,1);
      (**(code **)(*(longlong *)param_1[0xe5] + 0x250))((longlong *)param_1[0xe5],3);
      uVar7 = 3;
    }
    (**(code **)(*(longlong *)param_1[0xe7] + 0x250))((longlong *)param_1[0xe7],uVar7);
    plVar9 = (longlong *)param_1[0xe6];
    if (*(char *)((longlong)param_1 + 0x4e4) == '\0') {
      (**(code **)(*plVar9 + 0x250))(plVar9,1);
      uVar7 = 1;
    }
    else {
      FUN_1416f5900(plVar9,*(char *)((longlong)param_1 + 0x4e4),(int)param_1[0x9f]);
      FUN_1416fb690(param_1[0xe8],param_1 + 0x9d,1);
      (**(code **)(*(longlong *)param_1[0xe6] + 0x250))((longlong *)param_1[0xe6],3);
      uVar7 = 3;
    }
    (**(code **)(*(longlong *)param_1[0xe8] + 0x250))((longlong *)param_1[0xe8],uVar7);
  }
  if (local_98 != (void *)0x0) {
    FUN_141de9200();
  }
  (**(code **)(*(longlong *)param_1[0x100] + 0x250))((longlong *)param_1[0x100],3);
  switch((int)*plVar1) {
  case 0:
  case 2:
  case 3:
  case 4:
    (**(code **)(*(longlong *)param_1[0xdf] + 0x250))((longlong *)param_1[0xdf],0x140000003);
    uVar7 = 4;
    break;
  case 1:
    (**(code **)(*(longlong *)param_1[0xdf] + 0x250))((longlong *)param_1[0xdf],0x140000001);
    uVar7 = 1;
    break;
  case 5:
  case 6:
    (**(code **)(*(longlong *)param_1[0xdf] + 0x250))((longlong *)param_1[0xdf],0x140000003);
    (**(code **)(*(longlong *)param_1[0xe0] + 0x250))((longlong *)param_1[0xe0],4);
    (**(code **)(*(longlong *)param_1[0xa3] + 0x250))((longlong *)param_1[0xa3],1);
    (**(code **)(*(longlong *)param_1[0xa9] + 0x250))((longlong *)param_1[0xa9],1);
    plVar9 = (longlong *)param_1[0x100];
    uVar7 = 1;
    goto LAB_14169db9e;
  default:
    goto switchD_14169db18_default;
  }
  plVar9 = (longlong *)param_1[0xe0];
LAB_14169db9e:
  (**(code **)(*plVar9 + 0x250))(plVar9,uVar7);
switchD_14169db18_default:
  FUN_14169e920(param_1,(int)*plVar1);
  return;
}



// ---- FUN_1416b0ec0 @ 1416b0ec0 ----

void FUN_1416b0ec0(longlong param_1)

{
  bool bVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  longlong lVar7;
  longlong *plVar8;
  undefined1 local_res8 [8];
  float local_res10;
  float local_res14;
  undefined8 local_res18;
  undefined8 *local_38;
  undefined4 local_30;
  wchar_t *local_28;
  undefined8 **local_20;
  
  *(undefined4 *)(param_1 + 0x480) = 1;
  *(undefined4 *)(param_1 + 0x484) = 3;
  plVar8 = (longlong *)0x0;
  *(undefined4 *)(param_1 + 0x47c) = 0;
  *(undefined4 *)(param_1 + 0x488) = 4;
  *(undefined4 *)(param_1 + 0x48c) = 6;
  *(undefined4 *)(param_1 + 0x490) = 7;
  *(undefined4 *)(param_1 + 0x494) = 8;
  *(undefined4 *)(param_1 + 0x498) = 9;
  *(undefined4 *)(param_1 + 0x49c) = 10;
  *(undefined4 *)(param_1 + 0x4a0) = 2;
  *(undefined4 *)(param_1 + 0x4a4) = 5;
  *(undefined4 *)(param_1 + 0x4a8) = 0xf;
  *(undefined4 *)(param_1 + 0x4ac) = 0x10;
  *(undefined4 *)(param_1 + 0x4d8) = 0xb;
  *(undefined4 *)(param_1 + 0x4dc) = 0xc;
  *(undefined4 *)(param_1 + 0x4e0) = 0xd;
  *(undefined4 *)(param_1 + 0x4e4) = 0xe;
  *(undefined4 *)(param_1 + 0x4b0) = 0x11;
  *(undefined4 *)(param_1 + 0x4b4) = 0x12;
  *(undefined4 *)(param_1 + 0x4b8) = 0x14;
  *(undefined4 *)(param_1 + 0x4c0) = 0x14;
  *(undefined4 *)(param_1 + 0x4bc) = 0x15;
  *(undefined4 *)(param_1 + 0x4c4) = 0x16;
  *(undefined4 *)(param_1 + 0x4c8) = 0x17;
  *(undefined4 *)(param_1 + 0x4cc) = 0x18;
  *(undefined4 *)(param_1 + 0x4d4) = 0x18;
  *(undefined4 *)(param_1 + 0x4d0) = 0x19;
  *(undefined4 *)(param_1 + 0x4e8) = 0xb;
  *(undefined4 *)(param_1 + 0x4ec) = 0xb;
  FUN_141eb3410(local_res8,"All_Nut",1);
  plVar3 = (longlong *)FUN_1429af1b0(param_1,local_res8);
  if (plVar3 == (longlong *)0x0) {
LAB_1416b104c:
    plVar3 = plVar8;
  }
  else {
    lVar4 = FUN_1429f30c0();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1416b104c;
  }
  *(longlong **)(param_1 + 0x410) = plVar3;
  FUN_141eb3410(local_res8,"All_Icon_State",1);
  plVar3 = (longlong *)FUN_1429af1b0(param_1,local_res8);
  if (plVar3 == (longlong *)0x0) {
LAB_1416b10b2:
    plVar3 = plVar8;
  }
  else {
    lVar4 = FUN_1429f2a50();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1416b10b2;
  }
  *(longlong **)(param_1 + 0x418) = plVar3;
  FUN_141eb3410(local_res8,"Ins_Base",1);
  plVar3 = (longlong *)FUN_1429af1b0(param_1,local_res8);
  if (plVar3 == (longlong *)0x0) {
LAB_1416b1118:
    plVar3 = plVar8;
  }
  else {
    lVar4 = FUN_1429f6b30();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1416b1118;
  }
  *(longlong **)(param_1 + 0x428) = plVar3;
  FUN_141eb3410(local_res8,"Ins_Icon_St",1);
  plVar3 = (longlong *)FUN_1429af1b0(param_1,local_res8);
  if (plVar3 == (longlong *)0x0) {
LAB_1416b117e:
    plVar3 = plVar8;
  }
  else {
    lVar4 = FUN_1429f6b30();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1416b117e;
  }
  *(longlong **)(param_1 + 0x430) = plVar3;
  FUN_141eb3410(local_res8,"Ins_Base_Light",1);
  plVar3 = (longlong *)FUN_1429af1b0(param_1,local_res8);
  if (plVar3 == (longlong *)0x0) {
LAB_1416b11e4:
    plVar3 = plVar8;
  }
  else {
    lVar4 = FUN_1429f6b30();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1416b11e4;
  }
  *(longlong **)(param_1 + 0x440) = plVar3;
  FUN_141eb3410(local_res8,"Ins_Base_Eff00",1);
  plVar3 = (longlong *)FUN_1429af1b0(param_1,local_res8);
  if (plVar3 == (longlong *)0x0) {
LAB_1416b124a:
    plVar3 = plVar8;
  }
  else {
    lVar4 = FUN_1429f6b30();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1416b124a;
  }
  *(longlong **)(param_1 + 0x438) = plVar3;
  FUN_141eb3410(local_res8,"Ins_Eff_Light",1);
  plVar3 = (longlong *)FUN_1429af1b0(param_1,local_res8);
  if (plVar3 == (longlong *)0x0) {
LAB_1416b12b0:
    plVar3 = plVar8;
  }
  else {
    lVar4 = FUN_1429f6b30();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1416b12b0;
  }
  *(longlong **)(param_1 + 0x448) = plVar3;
  FUN_141eb3410(local_res8,"Ins_Icon_Curs",1);
  plVar3 = (longlong *)FUN_1429af1b0(param_1,local_res8);
  if (plVar3 == (longlong *)0x0) {
LAB_1416b1316:
    plVar3 = plVar8;
  }
  else {
    lVar4 = FUN_1429f6b30();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1416b1316;
  }
  *(longlong **)(param_1 + 0x450) = plVar3;
  plVar3 = *(longlong **)(*(longlong *)(param_1 + 0x418) + 0x28);
  if (plVar3 == (longlong *)0x0) {
LAB_1416b1366:
    plVar3 = plVar8;
  }
  else {
    lVar4 = FUN_1429f33e0();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1416b1366;
  }
  *(longlong **)(param_1 + 0x420) = plVar3;
  FUN_141eb3410(local_res8,"Op_Num",1);
  plVar3 = (longlong *)FUN_1429af1b0(param_1,local_res8);
  if (plVar3 == (longlong *)0x0) {
LAB_1416b13cc:
    plVar3 = plVar8;
  }
  else {
    lVar4 = FUN_1429f6b30();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1416b13cc;
  }
  *(longlong **)(param_1 + 0x458) = plVar3;
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 0x250))(plVar3,1);
  }
  *(undefined8 *)(param_1 + 0x3b0) = 0;
  FUN_141eb3410(local_res8,&DAT_143eff284,1);
  uVar5 = FUN_141a65110(param_1,local_res8);
  *(undefined8 *)(param_1 + 0x3b8) = uVar5;
  FUN_141eb3410(local_res8,&DAT_143eff2b4,1);
  uVar5 = FUN_141a65110(param_1,local_res8);
  *(undefined8 *)(param_1 + 0x3c0) = uVar5;
  FUN_141eb3410(local_res8,&DAT_143f00368,1);
  uVar5 = FUN_141a65110(param_1,local_res8);
  *(undefined8 *)(param_1 + 0x3c8) = uVar5;
  FUN_141eb3410(local_res8,"In_Fade",1);
  uVar5 = FUN_141a65110(param_1,local_res8);
  *(undefined8 *)(param_1 + 0x3d0) = uVar5;
  FUN_141eb3410(local_res8,"Out_Fade",1);
  uVar5 = FUN_141a65110(param_1,local_res8);
  *(undefined8 *)(param_1 + 0x3d8) = uVar5;
  FUN_141eb3410(local_res8,"Slide_L",1);
  uVar5 = FUN_141a65110(param_1,local_res8);
  *(undefined8 *)(param_1 + 0x3e0) = uVar5;
  FUN_141eb3410(local_res8,"Slide_R",1);
  uVar5 = FUN_141a65110(param_1,local_res8);
  *(undefined8 *)(param_1 + 1000) = uVar5;
  FUN_141eb3410(local_res8,"Slide_L",1);
  uVar5 = FUN_141a65110(param_1,local_res8);
  *(undefined8 *)(param_1 + 0x3f0) = uVar5;
  FUN_141eb3410(local_res8,"Slide_L",1);
  uVar5 = FUN_141a65110(param_1,local_res8);
  *(undefined8 *)(param_1 + 0x3f8) = uVar5;
  FUN_141eb3410(local_res8,"Slide_R",1);
  uVar5 = FUN_141a65110(param_1,local_res8);
  *(undefined8 *)(param_1 + 0x400) = uVar5;
  FUN_141eb3410(local_res8,"Slide_R",1);
  uVar5 = FUN_141a65110(param_1,local_res8);
  *(undefined8 *)(param_1 + 0x408) = uVar5;
  plVar3 = *(longlong **)(*(longlong *)(param_1 + 0x430) + 0x28);
  if (plVar3 == (longlong *)0x0) {
LAB_1416b15f9:
    plVar3 = plVar8;
  }
  else {
    lVar4 = FUN_1429f33e0();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1416b15f9;
  }
  FUN_1429ad310(plVar3,&local_res10);
  plVar3 = *(longlong **)(*(longlong *)(param_1 + 0x440) + 0x28);
  if (plVar3 == (longlong *)0x0) {
LAB_1416b164e:
    plVar3 = plVar8;
  }
  else {
    lVar4 = FUN_1429f33e0();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1416b164e;
  }
  puVar6 = (undefined8 *)FUN_1429ad310(plVar3,local_res8);
  uVar5 = *puVar6;
  *(float *)(param_1 + 0x4f8) = local_res14 / (float)((ulonglong)uVar5 >> 0x20);
  *(float *)(param_1 + 0x4f4) = local_res10 / (float)uVar5;
  *(undefined8 *)(param_1 + 0x4fc) = uVar5;
  plVar3 = *(longlong **)(*(longlong *)(param_1 + 0x448) + 0x28);
  if (plVar3 != (longlong *)0x0) {
    lVar4 = FUN_1429f33e0();
    if ((*(int *)(plVar3[2] + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar3[2] + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) goto LAB_1416b16db;
  }
  plVar3 = plVar8;
LAB_1416b16db:
  lVar4 = FUN_1429ac570(plVar3,local_res8);
  *(undefined4 *)(param_1 + 0x50c) = *(undefined4 *)(lVar4 + 4);
  (**(code **)(**(longlong **)(param_1 + 0x448) + 0x250))(*(longlong **)(param_1 + 0x448),3);
  (**(code **)(**(longlong **)(param_1 + 0x440) + 0x250))(*(longlong **)(param_1 + 0x440),3);
  if (*(char *)(param_1 + 0x3a0) == '\0') {
    lVar4 = param_1 + 0x288;
    FUN_1410770a0(lVar4,0);
    FUN_141eb3490(&local_res18,L"Pnl_Anm_On_01",1);
    local_38 = (undefined8 *)0x0;
    local_30 = 1;
    FUN_140826f80(&local_38,1);
    *local_38 = local_res18;
    local_28 = L"In";
    local_20 = &local_38;
    FUN_141492080(lVar4,local_res8,&local_28,0);
    if (local_38 != (undefined8 *)0x0) {
      FUN_141de9200();
    }
    FUN_141eb3490(&local_res18,L"Pnl_Anm_On_01",1);
    local_38 = (undefined8 *)0x0;
    local_30 = 1;
    FUN_140826f80(&local_38,1);
    *local_38 = local_res18;
    local_28 = L"Sign";
    local_20 = &local_38;
    FUN_141492080(lVar4,local_res8,&local_28,0);
    if (local_38 != (undefined8 *)0x0) {
      FUN_141de9200();
    }
    FUN_141eb3490(&local_res18,L"Pnl_Anm_On_01",1);
    local_38 = (undefined8 *)0x0;
    local_30 = 1;
    FUN_140826f80(&local_38,1);
    *local_38 = local_res18;
    local_28 = L"Out";
    local_20 = &local_38;
    FUN_141492080(lVar4,local_res8,&local_28,0);
    if (local_38 != (undefined8 *)0x0) {
      FUN_141de9200();
    }
    FUN_141eb3490(&local_res18,L"Pnl_Anm_On_01",1);
    local_38 = (undefined8 *)0x0;
    local_30 = 1;
    FUN_140826f80(&local_38,1);
    *local_38 = local_res18;
    local_28 = L"Slide_R";
    local_20 = &local_38;
    FUN_141492080(lVar4,local_res8,&local_28,0);
    if (local_38 != (undefined8 *)0x0) {
      FUN_141de9200();
    }
    FUN_141eb3490(&local_res18,L"Pnl_Anm_On_01",1);
    local_38 = (undefined8 *)0x0;
    local_30 = 1;
    FUN_140826f80(&local_38,1);
    *local_38 = local_res18;
    local_28 = L"In_Fade";
    local_20 = &local_38;
    FUN_141492080(lVar4,local_res8,&local_28,0);
    if (local_38 != (undefined8 *)0x0) {
      FUN_141de9200();
    }
    FUN_141eb3490(&local_res18,L"Pnl_Anm_On_01",1);
    local_38 = (undefined8 *)0x0;
    local_30 = 1;
    FUN_140826f80(&local_38,1);
    *local_38 = local_res18;
    local_28 = L"Out_Fade";
    local_20 = &local_38;
    FUN_141492080(lVar4,local_res8,&local_28,0);
    if (local_38 != (undefined8 *)0x0) {
      FUN_141de9200();
    }
    FUN_141a55c30(param_1);
  }
  else {
    FUN_141eb3410(local_res8,"All_Nut_InvalidationBox",1);
    lVar4 = FUN_1429af1b0(param_1,local_res8);
    if (lVar4 != 0) {
      lVar7 = FUN_1429fbb50();
      if ((*(int *)(*(longlong *)(lVar4 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
         (*(longlong *)
           (*(longlong *)(*(longlong *)(lVar4 + 0x10) + 0x88) + (longlong)*(int *)(lVar7 + 0x90) * 8
           ) != lVar7 + 0x88)) {
        bVar1 = false;
      }
      else {
        bVar1 = true;
      }
      if (bVar1) {
        FUN_1429dd980(lVar4,0);
      }
    }
  }
  uVar2 = FUN_140d2f940();
  *(undefined1 *)(param_1 + 0x518) = uVar2;
  return;
}



// ---- FUN_1416b6b00 @ 1416b6b00 ----

void FUN_1416b6b00(longlong param_1)

{
  int iVar1;
  
  if (*(longlong *)(param_1 + 0x4d0) != 0) {
    FUN_1416f7bd0(*(longlong *)(param_1 + 0x4d0),7,8,1);
  }
  iVar1 = *(int *)(param_1 + 0x4e4);
  if (((-1 < iVar1) && (iVar1 < *(int *)(param_1 + 0x430))) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x428) + (longlong)iVar1 * 8) != 0)) {
    FUN_1416c71b0();
    return;
  }
  return;
}



// ---- FUN_1416b6bf0 @ 1416b6bf0 ----

void FUN_1416b6bf0(longlong param_1)

{
  int iVar1;
  
  if (*(longlong *)(param_1 + 0x4d0) != 0) {
    FUN_1416f34e0();
  }
  iVar1 = *(int *)(param_1 + 0x4e4);
  if (((-1 < iVar1) && (iVar1 < *(int *)(param_1 + 0x430))) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x428) + (longlong)iVar1 * 8) != 0)) {
    FUN_1416c7070();
    return;
  }
  return;
}



// ---- FUN_1416bca00 @ 1416bca00 ----

ulonglong FUN_1416bca00(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  
  iVar1 = *(int *)(param_1 + 0x4e4);
  uVar3 = (ulonglong)iVar1;
  uVar4 = uVar3;
  if ((-1 < iVar1) && (iVar1 < *(int *)(param_1 + 0x430))) {
    uVar4 = *(ulonglong *)(param_1 + 0x428);
    lVar2 = *(longlong *)(uVar4 + uVar3 * 8);
    if (lVar2 != 0) {
      return (ulonglong)*(byte *)(lVar2 + 0x404);
    }
  }
  return uVar4 & 0xffffffffffffff00;
}



