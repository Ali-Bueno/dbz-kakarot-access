// ================= TARGET 0x1434f2db0  FUN_1434f2db0 @ 1434f2db0  size=0x183 =================
// callers: FUN_1434f3340@1434f3340 

undefined8 FUN_1434f2db0(longlong *param_1,ushort param_2,undefined4 param_3)

{
  longlong *plVar1;
  longlong lVar2;
  char cVar3;
  short sVar4;
  undefined4 *puVar5;
  
  plVar1 = param_1 + (longlong)*(int *)((longlong)param_1 + 500) * 2 + 0x3f;
  if ((int)param_1[0x3e] <= (int)(uint)*(ushort *)((longlong)plVar1 + 6)) {
    lVar2 = *param_1;
    cVar3 = FUN_1434f7c80(lVar2 + 0x50,lVar2 + 0x2c,1,(uint)*(ushort *)((longlong)plVar1 + 6) * 0x30
                         );
    if (cVar3 == '\0') {
      return 0;
    }
    if ((longlong *)param_1[0x3d] == param_1 + 1) {
      memcpy(*(void **)(lVar2 + 0x50),param_1 + 1,0x1e0);
    }
    param_1[0x3d] = *(longlong *)(lVar2 + 0x50);
    *(undefined4 *)(param_1 + 0x3e) = *(undefined4 *)(lVar2 + 0x2c);
  }
  puVar5 = (undefined4 *)((ulonglong)*(ushort *)((longlong)plVar1 + 6) * 0x18 + param_1[0x3d]);
  *puVar5 = param_3;
  puVar5[1] = (uint)param_2;
  puVar5[4] = *(undefined4 *)((longlong)plVar1 + 0xc);
  puVar5[2] = (int)*plVar1;
  *(undefined2 *)(puVar5 + 3) = 0;
  sVar4 = *(short *)((longlong)plVar1 + 6) + 1;
  *(short *)((longlong)plVar1 + 6) = sVar4;
  return CONCAT71((uint7)(byte)((ushort)sVar4 >> 8),1);
}



// =================   CALLER-OF 0x1434f2db0  FUN_1434f3340 @ 1434f3340  size=0x6db =================
// callers: FUN_1434f5810@1434f5810 

undefined8 FUN_1434f3340(longlong *param_1,int param_2)

{
  longlong *plVar1;
  longlong lVar2;
  byte bVar3;
  char cVar4;
  int iVar5;
  ulonglong uVar6;
  byte local_58;
  ushort local_48;
  ushort local_44;
  uint local_40;
  uint local_3c;
  uint local_38;
  
  plVar1 = param_1 + (longlong)*(int *)((longlong)param_1 + 500) * 2 + 0x3f;
  lVar2 = *(longlong *)(*param_1 + 0x78);
  local_58 = *(byte *)(lVar2 + param_2);
  if (local_58 == 10) {
    local_48 = *(ushort *)(*(longlong *)(*param_1 + 0x10) + (longlong)param_2 * 2);
    local_40 = (uint)*(ushort *)((longlong)plVar1 + 6);
    do {
      local_40 = local_40 - 1;
      if ((int)local_40 < (int)(uint)*(ushort *)((longlong)plVar1 + 4)) goto LAB_1434f35f7;
    } while (*(uint *)(param_1[0x3d] + 4 + (longlong)(int)local_40 * 0x18) != (uint)local_48);
    bVar3 = FUN_1434f3a20(param_1,local_40,param_2);
    if (bVar3 != 10) {
      *(undefined1 *)((longlong)plVar1 + 10) = 10;
      *(uint *)((longlong)plVar1 + 0xc) = (uint)bVar3;
      *(int *)plVar1 = param_2;
      bVar3 = *(byte *)(*(longlong *)(*param_1 + 0x80) + (longlong)param_2);
      if ((bVar3 & 0x80) != 0) {
        bVar3 = bVar3 & 1;
        *(byte *)((longlong)plVar1 + 9) = bVar3;
        for (local_38 = (uint)*(ushort *)((longlong)plVar1 + 4); (int)local_38 < (int)local_40;
            local_38 = local_38 + 1) {
          *(ushort *)(param_1[0x3d] + 0xc + (longlong)(int)local_38 * 0x18) =
               *(ushort *)(param_1[0x3d] + 0xc + (longlong)(int)local_38 * 0x18) |
               (ushort)(1 << bVar3);
        }
        *(byte *)(*(longlong *)(*param_1 + 0x80) + (longlong)param_2) =
             *(byte *)(*(longlong *)(*param_1 + 0x80) + (longlong)param_2) & 0x7f;
      }
      *(byte *)(*(longlong *)(*param_1 + 0x80) +
               (longlong)*(int *)(param_1[0x3d] + (longlong)(int)local_40 * 0x18)) =
           *(byte *)(*(longlong *)(*param_1 + 0x80) +
                    (longlong)*(int *)(param_1[0x3d] + (longlong)(int)local_40 * 0x18)) & 0x7f;
      return 1;
    }
    local_48 = 0;
LAB_1434f35f7:
    if (local_48 == 0) {
      local_44 = 0;
    }
    else {
      local_44 = FUN_14351ebc0(local_48);
    }
    if ((local_44 != local_48) &&
       (iVar5 = FUN_14351f290(*(undefined8 *)(*param_1 + 8),local_48), iVar5 == 1)) {
      if (local_44 == 0x232a) {
        cVar4 = FUN_1434f2db0(param_1,0x3009,param_2);
        if (cVar4 == '\0') {
          return 0;
        }
      }
      else if ((local_44 == 0x3009) &&
              (cVar4 = FUN_1434f2db0(param_1,0x232a,param_2), cVar4 == '\0')) {
        return 0;
      }
      cVar4 = FUN_1434f2db0(param_1,local_44,param_2);
      if (cVar4 == '\0') {
        return 0;
      }
    }
  }
  bVar3 = *(byte *)(*(longlong *)(*param_1 + 0x80) + (longlong)param_2);
  if ((bVar3 & 0x80) == 0) {
    if ((local_58 < 2) || (local_58 == 0xd)) {
      *(byte *)((longlong)plVar1 + 10) = local_58;
      *(byte *)((longlong)plVar1 + 9) = local_58;
      *(uint *)((longlong)plVar1 + 0xc) = (uint)(local_58 != 0);
      *(int *)plVar1 = param_2;
      local_58 = local_58 != 0;
    }
    else if (local_58 == 2) {
      *(undefined1 *)((longlong)plVar1 + 10) = 2;
      if (*(char *)((longlong)plVar1 + 9) == '\0') {
        local_58 = 0;
        if ((char)param_1[0x13d] == '\0') {
          *(undefined1 *)(lVar2 + param_2) = 0x17;
        }
        *(undefined4 *)((longlong)plVar1 + 0xc) = 0;
        *(int *)plVar1 = param_2;
      }
      else {
        local_58 = 1;
        if (*(char *)((longlong)plVar1 + 9) == '\r') {
          *(undefined1 *)(lVar2 + param_2) = 5;
        }
        else {
          *(undefined1 *)(lVar2 + param_2) = 0x18;
        }
        *(undefined4 *)((longlong)plVar1 + 0xc) = 1;
        *(int *)plVar1 = param_2;
      }
    }
    else if (local_58 == 5) {
      local_58 = 1;
      *(undefined1 *)((longlong)plVar1 + 10) = 5;
      *(undefined4 *)((longlong)plVar1 + 0xc) = 1;
      *(int *)plVar1 = param_2;
    }
    else if (local_58 == 0x11) {
      local_58 = *(byte *)((longlong)plVar1 + 10);
      if (local_58 == 10) {
        *(undefined1 *)(lVar2 + param_2) = 10;
      }
    }
    else {
      *(byte *)((longlong)plVar1 + 10) = local_58;
    }
  }
  else {
    bVar3 = bVar3 & 1;
    if (((local_58 != 8) && (local_58 != 9)) && (local_58 != 10)) {
      *(byte *)(lVar2 + param_2) = bVar3;
    }
    *(byte *)((longlong)plVar1 + 10) = bVar3;
    *(byte *)((longlong)plVar1 + 9) = bVar3;
    *(uint *)((longlong)plVar1 + 0xc) = (uint)bVar3;
    *(int *)plVar1 = param_2;
    local_58 = bVar3;
  }
  if ((local_58 < 2) || (uVar6 = 0, local_58 == 0xd)) {
    for (local_3c = (uint)*(ushort *)((longlong)plVar1 + 4);
        uVar6 = (ulonglong)*(ushort *)((longlong)plVar1 + 6),
        (int)local_3c < (int)(uint)*(ushort *)((longlong)plVar1 + 6); local_3c = local_3c + 1) {
      if (*(int *)(param_1[0x3d] + (longlong)(int)local_3c * 0x18) < param_2) {
        *(ushort *)(param_1[0x3d] + 0xc + (longlong)(int)local_3c * 0x18) =
             *(ushort *)(param_1[0x3d] + 0xc + (longlong)(int)local_3c * 0x18) |
             (ushort)(1 << (local_58 != 0));
      }
    }
  }
  return CONCAT71((int7)(uVar6 >> 8),1);
}



// ================= TARGET 0x141a32150  FUN_141a32150 @ 141a32150  size=0x26 =================
// callers: <none>

void FUN_141a32150(undefined8 param_1,void *param_2)

{
  memset(param_2,0,0x1e0);
  FUN_1419c8e60(param_2);
  return;
}



// ================= TARGET 0x142f2cc60  FUN_142f2cc60 @ 142f2cc60  size=0x199 =================
// callers: FUN_1428d8480@1428d8480 

longlong * FUN_142f2cc60(longlong param_1,undefined8 param_2,longlong param_3,undefined8 param_4)

{
  longlong lVar1;
  int iVar2;
  undefined8 *puVar3;
  longlong *plVar4;
  int iVar5;
  longlong *plVar6;
  longlong lVar7;
  int iVar8;
  
  if (param_3 == 0) {
    return (longlong *)0x0;
  }
  plVar6 = (longlong *)0x0;
  plVar4 = (longlong *)(param_1 + 0x138);
  iVar2 = *(int *)(param_1 + 0x140);
  iVar8 = iVar2 + 1;
  *(int *)(param_1 + 0x140) = iVar8;
  if (*(int *)(param_1 + 0x144) < iVar8) {
    FUN_142f36620(plVar4,iVar2);
  }
  lVar7 = (longlong)iVar2 * 0x1e0;
  FUN_142f27f90(*plVar4 + lVar7,param_2);
  FUN_142f36210();
  lVar1 = *plVar4;
  if (((*(int *)(lVar1 + 0x48 + lVar7) == 0) && (*(int *)(lVar1 + 0x4c + lVar7) == 0)) &&
     (*(int *)(lVar1 + 0x50 + lVar7) == 0)) {
    iVar5 = *(int *)(param_1 + 0x140);
    iVar2 = (iVar5 - iVar2) + -1;
    if (iVar2 != 0) {
      memmove((void *)(lVar1 + lVar7),(void *)((longlong)iVar8 * 0x1e0 + lVar1),
              (longlong)iVar2 * 0x1e0);
      iVar5 = *(int *)(param_1 + 0x140);
    }
    *(int *)(param_1 + 0x140) = iVar5 + -1;
  }
  else {
    iVar8 = *(int *)(param_1 + 0x38);
    *(int *)(param_1 + 0x38) = iVar8 + 1;
    if (*(int *)(param_1 + 0x3c) < iVar8 + 1) {
      FUN_1407f1790(param_1 + 0x30,iVar8);
    }
    puVar3 = (undefined8 *)((longlong)iVar8 * 0x10 + *(longlong *)(param_1 + 0x30));
    *puVar3 = 0;
    puVar3[1] = 0;
    puVar3 = (undefined8 *)((longlong)iVar2 * 0x10 + *(longlong *)(param_1 + 0x30));
    *puVar3 = param_4;
    puVar3[1] = param_3;
    plVar4 = (longlong *)FUN_140b40c10(0x10);
    if (plVar4 != (longlong *)0x0) {
      iVar2 = *(int *)(param_1 + 0x38);
      *plVar4 = param_1;
      *(int *)(plVar4 + 1) = iVar2 + -1;
      plVar6 = plVar4;
    }
    iVar2 = *(int *)(param_1 + 0x18);
    *(int *)(param_1 + 0x18) = iVar2 + 1;
    if (*(int *)(param_1 + 0x1c) < iVar2 + 1) {
      FUN_1407f1510(param_1 + 0x10,iVar2);
    }
    *(longlong **)(*(longlong *)(param_1 + 0x10) + (longlong)iVar2 * 8) = plVar6;
    *(undefined1 *)(param_1 + 0x148) = 1;
  }
  return plVar6;
}



// =================   CALLER-OF 0x142f2cc60  FUN_1428d8480 @ 1428d8480  size=0xb8e =================
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1428d8480(longlong param_1,longlong param_2)

{
  undefined4 *puVar1;
  longlong lVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  void **ppvVar9;
  char cVar10;
  int iVar11;
  int iVar12;
  longlong lVar13;
  undefined8 uVar14;
  ulonglong uVar15;
  uint *puVar16;
  void *_Dst;
  undefined1 *puVar17;
  longlong *plVar18;
  longlong *plVar19;
  ulonglong uVar20;
  longlong *plVar21;
  longlong lVar22;
  int iVar23;
  undefined8 *puVar24;
  longlong *plVar25;
  ulonglong uVar26;
  longlong *plVar27;
  ushort *puVar28;
  ulonglong uVar29;
  uint uVar30;
  bool bVar31;
  uint uVar32;
  uint uVar33;
  uint uVar34;
  undefined1 auStack_248 [32];
  longlong *local_228;
  undefined8 local_220;
  undefined8 local_218;
  undefined4 local_210;
  undefined8 local_208;
  undefined1 local_1f8;
  longlong local_1f0;
  undefined8 local_1e8;
  undefined8 uStack_1e0;
  undefined4 local_1d8;
  int iStack_1d4;
  uint uStack_1d0;
  undefined4 uStack_1cc;
  longlong *local_1c8;
  undefined8 local_1c0;
  void *local_1b8;
  int local_1b0;
  int local_1ac;
  longlong local_1a8;
  longlong local_1a0;
  longlong local_198;
  uint local_190;
  uint local_18c;
  longlong local_188;
  undefined8 local_180;
  void *local_178;
  int local_170;
  undefined4 local_16c;
  longlong local_168;
  undefined8 local_160;
  longlong local_158;
  int local_150;
  undefined4 local_14c;
  longlong local_148 [2];
  undefined4 local_138;
  undefined4 local_134;
  undefined4 local_130;
  longlong *local_128;
  undefined8 uStack_120;
  uint *local_118;
  uint uStack_110;
  int iStack_10c;
  undefined8 local_100;
  undefined8 local_f8 [2];
  longlong local_e8;
  undefined8 local_e0;
  undefined8 local_c8;
  undefined8 local_c0;
  undefined4 local_b8;
  int local_b4;
  undefined1 local_b0 [8];
  undefined1 *local_a8;
  int local_a0;
  longlong *local_98;
  void **ppvStack_90;
  uint *puStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  longlong local_70;
  longlong *local_68;
  void **local_60;
  longlong *local_58;
  ulonglong local_48;
  
  local_48 = DAT_145909780 ^ (ulonglong)auStack_248;
  local_1a8 = *(longlong *)(param_2 + 0x20);
  lVar2 = *(longlong *)(local_1a8 + 0x590);
  local_100 = lVar2 + 0x118;
  local_1f0 = *(longlong *)(param_1 + 0xf0);
  local_1a0 = param_1;
  if (local_1f0 == 0) {
    local_1f0 = FUN_142c6e620();
  }
  lVar13 = *(longlong *)(param_2 + 0x28);
  iVar11 = FUN_142b90ea0(lVar13,lVar2);
  lVar2 = *(longlong *)(lVar13 + 0x2f0);
  uVar30 = *(uint *)(lVar13 + 0x80);
  uVar26 = (ulonglong)uVar30;
  plVar25 = (longlong *)(param_1 + 0x198);
  *(undefined4 *)(param_1 + 0x1a0) = 0;
  if (((int)*(uint *)(param_1 + 0x1a4) < (int)uVar30) && (*(uint *)(param_1 + 0x1a4) != uVar30)) {
    FUN_140827be0(plVar25,uVar26);
  }
  *(undefined4 *)(param_1 + 0x1a0) = 0;
  if (*(uint *)(param_1 + 0x1a4) != uVar30) {
    FUN_140827be0(plVar25,uVar26);
  }
  if (0 < (int)uVar30) {
    do {
      iVar23 = *(int *)(param_1 + 0x1a0);
      *(int *)(param_1 + 0x1a0) = iVar23 + 1;
      if (*(int *)(param_1 + 0x1a4) < iVar23 + 1) {
        FUN_1407f1490(plVar25,iVar23);
      }
      puVar1 = (undefined4 *)(*plVar25 + (longlong)iVar23 * 4);
      if (puVar1 != (undefined4 *)0x0) {
        *puVar1 = 0xffffffff;
      }
      uVar26 = uVar26 - 1;
    } while (uVar26 != 0);
  }
  lVar13 = *(longlong *)(param_2 + 0x20);
  uVar14 = *(undefined8 *)(lVar13 + 0x198);
  *(undefined8 *)(param_1 + 0x530) = *(undefined8 *)(lVar13 + 400);
  *(undefined8 *)(param_1 + 0x538) = uVar14;
  uVar3 = *(undefined4 *)(lVar13 + 0x1a4);
  uVar4 = *(undefined4 *)(lVar13 + 0x1a8);
  uVar5 = *(undefined4 *)(lVar13 + 0x1ac);
  *(undefined4 *)(param_1 + 0x540) = *(undefined4 *)(lVar13 + 0x1a0);
  *(undefined4 *)(param_1 + 0x544) = uVar3;
  *(undefined4 *)(param_1 + 0x548) = uVar4;
  *(undefined4 *)(param_1 + 0x54c) = uVar5;
  uVar3 = *(undefined4 *)(lVar13 + 0x1b4);
  uVar4 = *(undefined4 *)(lVar13 + 0x1b8);
  uVar5 = *(undefined4 *)(lVar13 + 0x1bc);
  *(undefined4 *)(param_1 + 0x550) = *(undefined4 *)(lVar13 + 0x1b0);
  *(undefined4 *)(param_1 + 0x554) = uVar3;
  *(undefined4 *)(param_1 + 0x558) = uVar4;
  *(undefined4 *)(param_1 + 0x55c) = uVar5;
  if (local_1f0 != 0) {
    lVar13 = *(longlong *)(param_1 + 0x170);
    if (lVar13 != 0) {
      FUN_142f29420(lVar13);
      thunk_FUN_141de9200(lVar13,0x2a0);
    }
    lVar13 = FUN_140b40c10(0x2a0);
    if (lVar13 == 0) {
      uVar14 = 0;
    }
    else {
      uVar14 = FUN_142f28a90(lVar13);
    }
    uVar26 = 0;
    *(undefined8 *)(param_1 + 0x170) = uVar14;
    iVar23 = *(int *)(local_1f0 + 0x48);
    *(undefined4 *)(param_1 + 400) = 0;
    if (*(int *)(param_1 + 0x194) != iVar23) {
      FUN_140827c50(param_1 + 0x188,iVar23);
    }
    *(undefined4 *)(param_1 + 0x1e0) = 0;
    if ((*(int *)(param_1 + 0x1e4) < 0) && (*(int *)(param_1 + 0x1e4) != 0)) {
      FUN_1407f1a80(param_1 + 0x1d8,0);
    }
    *(undefined4 *)(param_1 + 0x208) = 0xffffffff;
    *(undefined4 *)(param_1 + 0x20c) = 0;
    iVar12 = *(int *)(param_1 + 0x200) + 0x1f;
    _Dst = *(void **)(param_1 + 0x1f8);
    if (*(void **)(param_1 + 0x1f8) == (void *)0x0) {
      _Dst = (void *)(param_1 + 0x1e8);
    }
    memset(_Dst,0,(longlong)((int)(iVar12 + (iVar12 >> 0x1f & 0x1fU)) >> 5) << 2);
    *(undefined4 *)(param_1 + 0x200) = 0;
    if (0 < (int)*(uint *)(param_1 + 0x220)) {
      uVar29 = (ulonglong)*(uint *)(param_1 + 0x220);
      uVar20 = uVar26;
      do {
        lVar13 = *(longlong *)(param_1 + 0x218);
        if (*(longlong *)(param_1 + 0x218) == 0) {
          lVar13 = param_1 + 0x210;
        }
        uVar15 = (longlong)*(int *)(param_1 + 0x220) - 1U & uVar20;
        uVar20 = uVar20 + 1;
        *(undefined4 *)(lVar13 + uVar15 * 4) = 0xffffffff;
        uVar29 = uVar29 - 1;
      } while (uVar29 != 0);
    }
    plVar25 = (longlong *)(param_1 + 0x1a8);
    *(undefined4 *)(param_1 + 0x1b0) = 0;
    if ((*(int *)(param_1 + 0x1b4) < iVar23) && (*(int *)(param_1 + 0x1b4) != iVar23)) {
      FUN_140827dc0(plVar25,iVar23);
    }
    iVar12 = *(int *)(param_1 + 0x1b0);
    *(int *)(param_1 + 0x1b0) = iVar12 + iVar23;
    if (*(int *)(param_1 + 0x1b4) < iVar12 + iVar23) {
      FUN_1408273a0(plVar25,iVar12);
    }
    uVar8 = uRam0000000145c5e2fc;
    uVar7 = uRam0000000145c5e2f8;
    uVar6 = uRam0000000145c5e2f4;
    uVar30 = _DAT_145c5e2f0;
    puVar17 = (undefined1 *)((longlong)iVar12 * 0x40 + *plVar25);
    if (iVar23 != 0) {
      puVar16 = (uint *)(puVar17 + 0x30);
      do {
        *puVar17 = 0;
        *(undefined1 *)((longlong)puVar16 + -0x2f) = 0;
        uVar32 = uRam0000000145c5e2ec;
        puVar17 = puVar17 + 0x40;
        puVar16[-8] = 0;
        puVar16[-7] = 0;
        puVar16[-6] = 0;
        puVar16[-5] = uVar32;
        puVar16[-4] = 0;
        puVar16[-3] = 0;
        puVar16[-2] = 0;
        puVar16[-1] = 0;
        uVar32 = uRam0000000145c5e2e4 & uVar6;
        uVar33 = uRam0000000145c5e2e8 & uVar7;
        uVar34 = uRam0000000145c5e2ec & uVar8;
        *puVar16 = _DAT_145c5e2e0 & uVar30;
        puVar16[1] = uVar32;
        puVar16[2] = uVar33;
        puVar16[3] = uVar34;
        iVar23 = iVar23 + -1;
        puVar16 = puVar16 + 0x10;
      } while (iVar23 != 0);
    }
    *(undefined4 *)(param_1 + 0x238) = 0;
    local_1c8 = (longlong *)0x0;
    local_1c0 = 0;
    local_188 = 0;
    local_180 = 0;
    if (*(int *)(param_1 + 0x138) == 1) {
      puVar24 = (undefined8 *)&local_138;
      local_138 = *(undefined4 *)(local_1a8 + 0x1b0);
      local_134 = *(undefined4 *)(local_1a8 + 0x1b4);
      local_130 = *(undefined4 *)(local_1a8 + 0x1b8);
    }
    else {
      puVar24 = &local_1e8;
      local_1e8 = (longlong *)0x3f8000003f800000;
      uStack_1e0 = (uint *)CONCAT44(uStack_1e0._4_4_,0x3f800000);
    }
    local_208 = FUN_1408f36a0(local_f8);
    local_228 = &local_188;
    local_210 = 0xffffffff;
    local_218 = 0;
    local_220 = 0;
    FUN_142ff6590(local_1a8,local_1f0,puVar24,&local_1c8);
    local_e8 = 0;
    local_e0 = 0;
    local_c8 = 0;
    local_c0 = 0;
    local_b8 = 0xffffffff;
    local_b4 = 0;
    local_a8 = (undefined1 *)0x0;
    local_a0 = 0;
    local_148[0] = 0;
    local_148[1] = 0;
    local_168 = 0;
    local_160 = 0;
    FUN_1428b74a0(&local_168);
    iVar23 = (int)local_160;
    lVar13 = (longlong)(int)local_160;
    local_178 = (void *)0x0;
    local_16c = 0;
    local_170 = (int)local_160;
    if (0 < (int)local_160) {
      FUN_1407f1510(&local_178);
    }
    memset(local_178,0,lVar13 * 8);
    local_1ac = 0;
    local_1b8 = (void *)0x0;
    local_1b0 = iVar23;
    if (0 < iVar23) {
      FUN_1407f1510(&local_1b8);
    }
    memset(local_1b8,0,lVar13 * 8);
    uVar20 = (ulonglong)((longlong)(local_1c8 + (int)local_1c0) + (7 - (longlong)local_1c8)) >> 3;
    if (local_1c8 + (int)local_1c0 < local_1c8) {
      uVar20 = 0;
    }
    plVar25 = local_1c8;
    if (uVar20 != 0) {
      do {
        lVar22 = *plVar25;
        cVar10 = FUN_142f16f50(lVar22);
        if (cVar10 != '\0') {
          *(longlong *)((longlong)local_1b8 + (longlong)*(short *)(lVar22 + 4) * 8) = lVar22;
        }
        plVar25 = plVar25 + 1;
        uVar26 = uVar26 + 1;
      } while (uVar26 != uVar20);
    }
    local_98 = &local_168;
    ppvStack_90 = &local_1b8;
    puStack_88 = (uint *)&local_1f0;
    uStack_80 = &local_1a8;
    uStack_78 = local_1a0;
    local_68 = &local_e8;
    local_60 = &local_178;
    local_70 = lVar2 + 0x10 + (longlong)iVar11 * 0x20;
    local_58 = local_148;
    FUN_1428b17e0(&local_98,1);
    plVar25 = uStack_80;
    ppvVar9 = ppvStack_90;
    uVar20 = 0;
    puVar28 = (ushort *)*local_98;
    uVar26 = (longlong)(int)local_98[1] & 0x7fffffffffffffff;
    if (puVar28 + (int)local_98[1] < puVar28) {
      uVar26 = uVar20;
    }
    if (uVar26 != 0) {
      do {
        if (*(longlong *)((longlong)*ppvVar9 + (ulonglong)*puVar28 * 8) != 0) {
          FUN_142c6c5c0(*plVar25,&local_98,*puVar28);
          local_f8[0] = 0;
        }
        puVar28 = puVar28 + 1;
        uVar20 = uVar20 + 1;
      } while (uVar20 != uVar26);
    }
    plVar25 = (longlong *)0x0;
    local_150 = iVar23;
    local_158 = 0;
    local_14c = 0;
    if (0 < iVar23) {
      FUN_1407f1490(&local_158,0);
    }
    plVar19 = plVar25;
    plVar27 = plVar25;
    if (0 < lVar13) {
      do {
        plVar21 = (longlong *)((longlong)plVar19 + 1);
        *(int *)(local_158 + (ulonglong)*(ushort *)(local_168 + (longlong)plVar19 * 2) * 4) =
             (int)plVar27;
        plVar19 = plVar21;
        plVar27 = (longlong *)(ulonglong)((int)plVar27 + 1);
      } while ((longlong)plVar21 < lVar13);
    }
    local_1e8 = &local_158;
    uStack_1e0 = (uint *)local_100;
    FUN_1428ad2f0(local_188,(longlong)(int)local_180,local_1f8,&local_1e8);
    if (0 < (int)local_e0 - local_b4) {
      plVar19 = plVar25;
      plVar27 = plVar25;
      if (0 < (int)local_180) {
        do {
          lVar2 = *(longlong *)((longlong)plVar27 + local_188);
          lVar13 = *(longlong *)(lVar2 + 0x20);
          plVar21 = plVar25;
          if ((int)local_e0 != local_b4) {
            puVar17 = local_b0;
            if (local_a8 != (undefined1 *)0x0) {
              puVar17 = local_a8;
            }
            iVar11 = *(int *)(puVar17 +
                             (longlong)
                             (int)((int)((ulonglong)lVar13 >> 0x20) + (int)lVar13 & local_a0 - 1U) *
                             4);
            while (iVar11 != -1) {
              lVar22 = (longlong)iVar11;
              if (*(longlong *)(local_e8 + lVar22 * 0x18) == lVar13) {
                if ((iVar11 != -1) && (lVar13 = local_e8 + lVar22 * 0x18, lVar13 != 0)) {
                  plVar21 = *(longlong **)(lVar13 + 8);
                }
                break;
              }
              iVar11 = *(int *)(local_e8 + 0x10 + lVar22 * 0x18);
            }
          }
          lVar13 = *(longlong *)(lVar2 + 0x28);
          plVar18 = plVar25;
          if ((int)local_e0 != local_b4) {
            puVar17 = local_b0;
            if (local_a8 != (undefined1 *)0x0) {
              puVar17 = local_a8;
            }
            iVar11 = *(int *)(puVar17 +
                             (longlong)
                             (int)((int)((ulonglong)lVar13 >> 0x20) + (int)lVar13 & local_a0 - 1U) *
                             4);
            while (iVar11 != -1) {
              lVar22 = (longlong)iVar11;
              if (*(longlong *)(local_e8 + lVar22 * 0x18) == lVar13) {
                if ((iVar11 != -1) && (lVar13 = local_e8 + lVar22 * 0x18, lVar13 != 0)) {
                  plVar18 = *(longlong **)(lVar13 + 8);
                }
                break;
              }
              iVar11 = *(int *)(local_e8 + 0x10 + lVar22 * 0x18);
            }
          }
          if (((plVar21 != (longlong *)0x0) && (plVar18 != (longlong *)0x0)) &&
             ((*(uint *)(plVar21 + 7) < *(uint *)(plVar21[6] + 0x168) ||
              (*(uint *)(plVar18 + 7) < *(uint *)(plVar18[6] + 0x168))))) {
            FUN_142f2cc60(*(undefined8 *)(local_1a0 + 0x170),*(undefined8 *)(lVar2 + 8),plVar21,
                          plVar18);
            FUN_142f47b30(local_1f0,(int)plVar21[7],(int)plVar18[7]);
          }
          FUN_142f37af0(lVar2);
          if (*(int *)(lVar2 + 0x1b0) == 0) {
            plVar21 = *(longlong **)(lVar2 + 0x1a8);
            plVar18 = plVar25;
          }
          else {
            plVar21 = *(longlong **)(lVar2 + 0x1a8);
            plVar18 = plVar21;
          }
          if (plVar18 == (longlong *)0x0) {
            if ((*(int *)(lVar2 + 0x1b0) != 0) && (plVar21 != (longlong *)0x0)) {
              lVar13 = *plVar21;
              goto LAB_1428d8c68;
            }
          }
          else {
            lVar13 = *plVar18;
            plVar21 = plVar18;
LAB_1428d8c68:
            (**(code **)(lVar13 + 0x30))(plVar21,0);
            if (*(longlong *)(lVar2 + 0x1a8) != 0) {
              uVar14 = FUN_141df5850(*(longlong *)(lVar2 + 0x1a8),0,0);
              *(undefined8 *)(lVar2 + 0x1a8) = uVar14;
            }
            *(undefined4 *)(lVar2 + 0x1b0) = 0;
          }
          if (*(longlong *)(lVar2 + 0x1a8) != 0) {
            FUN_141de9200();
          }
          thunk_FUN_141de9200(lVar2,0x1b8);
          uVar30 = (int)plVar19 + 1;
          if ((int)local_180 <= (int)uVar30) break;
          plVar19 = (longlong *)(ulonglong)uVar30;
          plVar27 = plVar27 + 1;
        } while( true );
      }
      *(undefined1 *)(local_1a0 + 0x56c) = 2;
    }
    lVar2 = local_1a0;
    plVar19 = (longlong *)
              ((ulonglong)((longlong)(local_1c8 + (int)local_1c0) + (7 - (longlong)local_1c8)) >> 3)
    ;
    if (local_1c8 + (int)local_1c0 < local_1c8) {
      plVar19 = plVar25;
    }
    plVar27 = plVar25;
    plVar21 = local_1c8;
    if (plVar19 != (longlong *)0x0) {
      do {
        lVar13 = *plVar21;
        cVar10 = FUN_142f16f50(lVar13);
        if (cVar10 != '\0') {
          FUN_142f21f80(lVar13,1);
        }
        if (lVar13 != 0) {
          FUN_14088b860(lVar13);
          thunk_FUN_141de9200(lVar13,0x150);
        }
        plVar27 = (longlong *)((longlong)plVar27 + 1);
        plVar21 = plVar21 + 1;
      } while (plVar27 != plVar19);
    }
    bVar31 = local_1c0._4_4_ != 0;
    local_1c0 = (ulonglong)local_1c0._4_4_ << 0x20;
    if (bVar31) {
      FUN_140827c50(&local_1c8,0);
    }
    local_1b0 = 0;
    if (local_1ac != 0) {
      FUN_140827c50(&local_1b8,0);
    }
    local_128 = (longlong *)(local_1f0 + 0xb8);
    local_198 = 0;
    uStack_1e0 = (uint *)(local_1f0 + 200);
    local_190 = 0;
    local_18c = 0;
    iVar11 = *(int *)(local_1f0 + 0xe0);
    local_1e8 = (longlong *)0x100000000;
    iStack_1d4 = 0;
    uStack_1d0 = 0;
    if (iVar11 != 0) {
      puVar16 = *(uint **)(local_1f0 + 0xd8);
      if (*(uint **)(local_1f0 + 0xd8) == (uint *)0x0) {
        puVar16 = uStack_1e0;
      }
      uVar30 = *puVar16;
      plVar19 = plVar25;
      plVar27 = plVar25;
      while (uVar30 == 0) {
        iVar23 = (int)plVar19;
        uStack_1d0 = (int)plVar27 + 0x20;
        plVar27 = (longlong *)(ulonglong)uStack_1d0;
        plVar19 = (longlong *)(ulonglong)(iVar23 + 1U);
        local_1e8._4_4_ = (undefined4)((ulonglong)local_1e8 >> 0x20);
        local_1e8 = (longlong *)CONCAT44(local_1e8._4_4_,iVar23 + 1U);
        iStack_1d4 = iVar11;
        if ((int)(iVar11 + -1 + (iVar11 + -1 >> 0x1f & 0x1fU)) >> 5 <= iVar23) goto LAB_1428d8e5d;
        uVar30 = puVar16[(longlong)iVar23 + 1];
      }
      uVar30 = -uVar30 & uVar30;
      iVar23 = 0x1f;
      if (uVar30 != 0) {
        for (; uVar30 >> iVar23 == 0; iVar23 = iVar23 + -1) {
        }
      }
      local_1e8 = (longlong *)CONCAT44(uVar30,(undefined4)local_1e8);
      if (uVar30 == 0) {
        iVar23 = 0x20;
      }
      else {
        iVar23 = 0x1f - iVar23;
      }
      iStack_1d4 = (uStack_1d0 - iVar23) + 0x1f;
      if (iVar11 < iStack_1d4) {
        iStack_1d4 = iVar11;
      }
    }
LAB_1428d8e5d:
    local_1d8 = 0xffffffff;
    uStack_80._0_4_ = 0xffffffff;
    uVar3 = (undefined4)uStack_80;
    uStack_80._0_4_ = 0xffffffff;
    uStack_80._4_4_ = iStack_1d4;
    uStack_78 = CONCAT44(uStack_1cc,uStack_1d0);
    ppvStack_90 = (void **)local_1e8;
    uStack_120 = local_1e8;
    local_100 = CONCAT44(uStack_1cc,uStack_1d0);
    uStack_110 = 0xffffffff;
    iStack_10c = iStack_1d4;
    plVar19 = plVar25;
    local_118 = uStack_1e0;
    local_98 = local_128;
    puStack_88 = uStack_1e0;
    if (iStack_1d4 < iVar11) {
      while( true ) {
        local_1e8 = *(longlong **)
                     ((longlong)local_178 +
                     (longlong)*(int *)(*local_128 + (longlong)iStack_10c * 0x14) * 8);
        uStack_1e0 = *(uint **)((longlong)local_178 +
                               (longlong)*(int *)(*local_128 + 4 + (longlong)iStack_10c * 0x14) * 8)
        ;
        local_190 = (int)plVar25 + 1;
        if ((int)plVar19 < (int)local_190) {
          FUN_1407f1790(&local_198,plVar25);
        }
        puVar1 = (undefined4 *)(local_198 + (longlong)(int)plVar25 * 0x10);
        *puVar1 = (undefined4)local_1e8;
        puVar1[1] = local_1e8._4_4_;
        puVar1[2] = (undefined4)uStack_1e0;
        puVar1[3] = uStack_1e0._4_4_;
        uStack_110 = uStack_110 & ~uStack_120._4_4_;
        FUN_1407e9ca0(&uStack_120);
        uVar3 = (undefined4)uStack_80;
        if ((int)local_118[6] <= iStack_10c) break;
        plVar25 = (longlong *)(ulonglong)local_190;
        plVar19 = (longlong *)(ulonglong)local_18c;
      }
    }
    uStack_80._0_4_ = uVar3;
    FUN_142f36c40(*(undefined8 *)(lVar2 + 0x170),&local_198);
    FUN_142f36b10(*(undefined8 *)(lVar2 + 0x170),local_148);
    if (local_198 != 0) {
      FUN_141de9200();
    }
    if (local_158 != 0) {
      FUN_141de9200();
    }
    if (local_1b8 != (void *)0x0) {
      FUN_141de9200();
    }
    if (local_178 != (void *)0x0) {
      FUN_141de9200();
    }
    if (local_168 != 0) {
      FUN_141de9200();
    }
    if (local_148[0] != 0) {
      FUN_141de9200();
    }
    local_a0 = 0;
    if (local_a8 != (undefined1 *)0x0) {
      FUN_141de9200();
    }
    FUN_1407fa390(&local_e8);
    if (local_188 != 0) {
      FUN_141de9200();
    }
    if (local_1c8 != (longlong *)0x0) {
      FUN_141de9200();
    }
  }
  return;
}



// ================= TARGET 0x14338f660  FUN_14338f660 @ 14338f660  size=0x1e8c =================
// strings: [{0} {1}: {2}, FilterRequire, EnvQueryGenerator, require, NotWithSpace, not , {0} {1}{2}, DontScore, don't score, x{0}, {0} [{1}], ScoreConstant, constant score, {0}: {1}, ScoreFactor, score factor, {0} {1}{2} [x{3}], ScorePrefer, prefer, {0}, {1}]
// callers: FUN_1433be930@1433be930 FUN_1433bdf90@1433bdf90 FUN_1433be8c0@1433be8c0 

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Removing unreachable block (ram,0x00014338fdc1) */
/* WARNING: Removing unreachable block (ram,0x00014338fdd1) */
/* WARNING: Removing unreachable block (ram,0x00014338fddd) */
/* WARNING: Removing unreachable block (ram,0x00014338fdf1) */

undefined8 FUN_14338f660(longlong param_1,undefined8 param_2,undefined4 *param_3)

{
  int *piVar1;
  char *pcVar2;
  longlong *plVar3;
  int iVar4;
  longlong *plVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  bool bVar9;
  bool bVar10;
  bool bVar11;
  bool bVar12;
  longlong lVar13;
  char cVar14;
  undefined8 uVar15;
  longlong lVar16;
  ulonglong uVar17;
  longlong lVar18;
  longlong *plVar19;
  undefined1 *puVar20;
  undefined4 *puVar21;
  undefined8 *puVar22;
  char *pcVar23;
  bool bVar24;
  undefined1 auStack_b58 [32];
  undefined2 local_b38;
  longlong local_b28;
  ulonglong local_b20;
  undefined4 *local_b18;
  undefined4 *puStack_b10;
  undefined4 *local_b08;
  undefined4 *puStack_b00;
  undefined8 *local_af8;
  undefined4 *puStack_af0;
  longlong local_ae8 [14];
  undefined4 *local_a78;
  undefined4 *puStack_a70;
  undefined4 *local_a68;
  ulonglong *puStack_a60;
  undefined1 local_a58 [8];
  longlong *local_a50;
  undefined8 local_a40;
  undefined1 local_a38 [8];
  longlong *local_a30;
  undefined8 local_a20;
  longlong local_a18;
  undefined8 local_a10;
  longlong local_a08;
  undefined8 local_a00;
  longlong *plStack_9f8;
  undefined8 local_9f0;
  longlong local_9e8;
  undefined8 local_9e0;
  longlong local_9d8;
  undefined8 local_9d0;
  longlong *plStack_9c8;
  undefined8 local_9c0;
  longlong local_9b8;
  undefined8 local_9b0;
  longlong local_9a8;
  undefined1 local_9a0 [20];
  undefined4 local_98c;
  undefined8 local_988;
  undefined8 uStack_980;
  undefined1 local_978 [16];
  undefined8 local_968;
  undefined8 uStack_960;
  undefined1 local_958 [16];
  longlong local_948 [2];
  longlong local_938 [2];
  longlong local_928 [2];
  undefined8 local_918;
  undefined8 uStack_910;
  undefined1 local_908 [16];
  undefined8 local_8f8;
  undefined8 uStack_8f0;
  undefined1 local_8e8 [16];
  undefined8 local_8d8;
  undefined8 uStack_8d0;
  undefined1 local_8c8 [16];
  longlong local_8b8 [2];
  undefined8 local_8a8;
  undefined8 uStack_8a0;
  undefined1 local_898 [16];
  undefined8 local_888;
  undefined8 uStack_880;
  undefined1 local_878 [16];
  undefined1 local_868 [8];
  longlong *local_860;
  undefined1 local_850 [8];
  longlong *local_848;
  undefined1 local_838 [8];
  longlong *local_830;
  undefined1 local_820 [8];
  longlong *local_818;
  undefined1 local_808 [8];
  longlong *local_800;
  undefined1 local_7f0 [8];
  longlong *local_7e8;
  undefined1 local_7d8 [8];
  longlong *local_7d0;
  undefined1 local_7c0 [8];
  longlong *local_7b8;
  undefined1 local_7a8 [8];
  longlong *local_7a0;
  undefined1 local_790 [8];
  longlong *local_788;
  undefined1 local_778 [8];
  longlong *local_770;
  undefined1 local_760 [8];
  longlong *local_758;
  undefined1 local_748 [8];
  longlong *local_740;
  undefined1 local_730 [8];
  longlong *local_728;
  undefined1 local_718 [8];
  longlong *local_710;
  undefined1 local_700 [8];
  longlong *local_6f8;
  undefined1 local_6e8 [8];
  longlong *local_6e0;
  undefined1 local_6d0 [8];
  longlong *local_6c8;
  undefined1 local_6b8 [8];
  longlong *local_6b0;
  undefined1 local_6a0 [24];
  undefined1 local_688 [8];
  longlong *local_680;
  undefined1 local_670 [8];
  longlong *local_668;
  undefined1 local_658 [8];
  longlong *local_650;
  undefined1 local_640 [8];
  longlong *local_638;
  undefined1 local_628 [8];
  longlong *local_620;
  undefined1 local_610 [8];
  longlong *local_608;
  undefined1 local_5f8 [8];
  longlong *local_5f0;
  undefined1 local_5e0 [8];
  longlong *local_5d8;
  undefined1 local_5c8 [8];
  longlong *local_5c0;
  undefined1 local_5b0 [8];
  longlong *local_5a8;
  undefined1 local_598 [8];
  longlong *local_590;
  undefined1 local_580 [16];
  undefined1 local_570 [16];
  undefined1 local_560 [16];
  undefined1 local_550 [16];
  undefined1 local_540 [16];
  undefined1 local_530 [16];
  undefined1 local_520 [16];
  undefined1 local_510 [24];
  undefined1 local_4f8 [24];
  undefined1 local_4e0 [24];
  undefined1 local_4c8 [24];
  undefined1 local_4b0 [24];
  undefined1 local_498 [24];
  undefined1 local_480 [24];
  undefined1 local_468 [24];
  undefined1 local_450 [24];
  undefined1 local_438 [24];
  undefined1 local_420 [24];
  undefined1 local_408 [24];
  undefined1 local_3f0 [24];
  undefined1 local_3d8 [24];
  undefined1 local_3c0 [24];
  undefined1 local_3a8 [24];
  undefined1 local_390 [24];
  undefined4 local_378 [4];
  undefined1 local_368 [8];
  longlong *local_360;
  char local_350;
  undefined4 local_348 [4];
  undefined1 local_338 [24];
  undefined1 local_320;
  undefined4 local_318;
  undefined1 local_308 [24];
  undefined1 local_2f0;
  undefined4 local_2e8 [4];
  undefined1 local_2d8 [24];
  char local_2c0 [8];
  undefined4 local_2b8;
  undefined1 local_2a8 [24];
  undefined1 local_290;
  undefined4 local_288 [4];
  undefined1 local_278 [24];
  char local_260 [8];
  undefined4 local_258;
  undefined1 local_248 [24];
  undefined1 local_230;
  undefined4 local_228 [4];
  undefined1 local_218 [24];
  char local_200 [8];
  undefined4 local_1f8;
  undefined1 local_1e8 [24];
  undefined1 local_1d0;
  undefined4 local_1c8;
  undefined1 local_1b8 [24];
  undefined1 local_1a0;
  undefined4 local_198 [4];
  undefined1 local_188 [24];
  char local_170 [8];
  undefined4 local_168;
  undefined1 local_158 [24];
  undefined1 local_140;
  undefined4 local_138;
  undefined1 local_128 [24];
  undefined1 local_110;
  undefined4 local_108 [4];
  undefined1 local_f8 [24];
  char local_e0 [8];
  undefined4 local_d8;
  undefined1 local_c8 [24];
  undefined1 local_b0;
  undefined4 local_a8;
  undefined1 local_98 [24];
  undefined1 local_80;
  undefined4 local_78;
  undefined1 local_68 [24];
  undefined1 local_50;
  ulonglong local_48 [2];
  
  local_48[0] = DAT_145909780 ^ (ulonglong)auStack_b58;
  local_b20 = local_b20 & 0xffffffff00000000;
  local_b18 = param_3;
  local_a40 = param_2;
  FUN_141e09010(local_a38);
  local_988 = (undefined8 *)CONCAT44(local_988._4_4_,(undefined4)local_988);
  uStack_980 = (undefined4 *)CONCAT44(uStack_980._4_4_,(undefined4)uStack_980);
  local_968 = (undefined4 *)CONCAT44(local_968._4_4_,(undefined4)local_968);
  uStack_960 = (undefined4 *)CONCAT44(uStack_960._4_4_,(undefined4)uStack_960);
  if ((*(char *)(param_1 + 0x34) != '\x01') &&
     (local_988 = (undefined8 *)CONCAT44(local_988._4_4_,(undefined4)local_988),
     uStack_980 = (undefined4 *)CONCAT44(uStack_980._4_4_,(undefined4)uStack_980),
     local_968 = (undefined4 *)CONCAT44(local_968._4_4_,(undefined4)local_968),
     uStack_960 = (undefined4 *)CONCAT44(uStack_960._4_4_,(undefined4)uStack_960),
     *(char *)(param_1 + 0x4a) == '\x03')) {
    if (*(longlong *)(param_1 + 0x60) == 0) {
      bVar24 = *(char *)(param_1 + 0x78) == '\0';
      if (bVar24) {
        uVar15 = FUN_141e22aa0(local_688,L"not ",L"EnvQueryGenerator",L"NotWithSpace");
      }
      else {
        uVar15 = FUN_140869340();
        uVar15 = FUN_141e08fe0(local_670,uVar15);
      }
      bVar12 = false;
      bVar11 = false;
      bVar10 = false;
      bVar9 = !bVar24;
      bVar8 = true;
      bVar7 = false;
      bVar6 = true;
      local_ae8[0] = 0;
      local_ae8[1] = 0xb;
      FUN_1407e2ee0(local_ae8,0);
      local_b38 = 0x3f;
      FUN_1407e4a20(local_ae8[0],0xb,"{0} {1}{2}");
      puVar21 = (undefined4 *)FUN_141e241b0(local_4c8,param_3);
      local_b08 = puVar21;
      uVar17 = FUN_141e08fe0(local_4b0,uVar15);
      local_b20 = uVar17;
      lVar16 = FUN_141e22aa0(local_498,L"require",L"EnvQueryGenerator",L"FilterRequire");
      local_b28 = lVar16;
      uVar15 = FUN_141e24190(local_6a0,local_ae8);
      puVar22 = (undefined8 *)FUN_141e09170(local_520,uVar15);
      local_198[0] = 4;
      FUN_141e08fe0(local_188,lVar16);
      local_170[0] = '\x01';
      local_168 = 4;
      FUN_141e08fe0(local_158,uVar17);
      local_140 = 1;
      local_138 = 4;
      FUN_141e08fe0(local_128,puVar21);
      local_a78 = local_198;
      local_110 = 1;
      puStack_a70 = local_108;
      local_a20 = *puVar22;
      local_a18 = puVar22[1];
      if (local_a18 != 0) {
        LOCK();
        *(int *)(local_a18 + 8) = *(int *)(local_a18 + 8) + 1;
        UNLOCK();
      }
      local_968 = local_a78;
      uStack_960 = puStack_a70;
      uVar15 = FUN_1407e5240(local_958,&local_968);
      uVar15 = FUN_141e22b00(local_6b8,&local_a20,uVar15);
      FUN_141e08fe0(local_7c0,uVar15);
      if (local_6b0 != (longlong *)0x0) {
        LOCK();
        plVar19 = local_6b0 + 1;
        lVar16 = *plVar19;
        *(int *)plVar19 = (int)*plVar19 + -1;
        UNLOCK();
        if ((int)lVar16 == 1) {
          (**(code **)*local_6b0)(local_6b0);
          LOCK();
          piVar1 = (int *)((longlong)local_6b0 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*local_6b0 + 8))(local_6b0,1);
          }
        }
      }
      FUN_1407e58c0(local_958);
      lVar16 = 3;
      pcVar23 = local_e0;
      do {
        lVar16 = lVar16 + -1;
        pcVar2 = pcVar23 + -0x30;
        if (*pcVar2 != '\0') {
          *pcVar2 = '\0';
          plVar19 = *(longlong **)(pcVar23 + -0x40);
          if (plVar19 != (longlong *)0x0) {
            LOCK();
            plVar5 = plVar19 + 1;
            lVar18 = *plVar5;
            *(int *)plVar5 = (int)*plVar5 + -1;
            UNLOCK();
            if ((int)lVar18 == 1) {
              (**(code **)*plVar19)(plVar19);
              LOCK();
              piVar1 = (int *)((longlong)plVar19 + 0xc);
              iVar4 = *piVar1;
              *piVar1 = *piVar1 + -1;
              UNLOCK();
              if (iVar4 == 1) {
                (**(code **)(*plVar19 + 8))(plVar19,1);
              }
            }
          }
        }
        pcVar23 = pcVar2;
      } while (lVar16 != 0);
      plVar19 = (longlong *)puVar22[1];
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar16 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar16 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      plVar19 = *(longlong **)(local_b28 + 8);
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar16 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar16 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      plVar19 = *(longlong **)(local_b20 + 8);
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar16 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar16 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      plVar19 = *(longlong **)(local_b08 + 2);
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar16 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar16 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      puVar20 = local_7c0;
    }
    else {
      local_ae8[2] = 0;
      local_ae8[3] = 0xd;
      FUN_1407e2ee0(local_ae8 + 2,0);
      local_b38 = 0x3f;
      FUN_1407e4a20(local_ae8[2],0xd,"{0} {1}: {2}");
      uVar15 = FUN_1433adac0(param_1 + 0x50,local_948);
      lVar16 = FUN_141e24190(local_510,uVar15);
      local_b28 = lVar16;
      uVar17 = FUN_141e241b0(local_4f8,param_3);
      local_b20 = uVar17;
      lVar18 = FUN_141e22aa0(local_4e0,L"require",L"EnvQueryGenerator",L"FilterRequire");
      uVar15 = FUN_141e24190(local_778,local_ae8 + 2);
      plVar19 = (longlong *)FUN_141e09170(local_530,uVar15);
      local_228[0] = 4;
      FUN_141e08fe0(local_218,lVar18);
      local_200[0] = '\x01';
      local_1f8 = 4;
      FUN_141e08fe0(local_1e8,uVar17);
      local_1d0 = 1;
      local_1c8 = 4;
      FUN_141e08fe0(local_1b8,lVar16);
      local_af8 = (undefined8 *)local_228;
      local_1a0 = 1;
      puStack_af0 = local_198;
      local_a78 = (undefined4 *)*plVar19;
      puStack_a70 = (undefined4 *)plVar19[1];
      if (puStack_a70 != (undefined4 *)0x0) {
        LOCK();
        puStack_a70[2] = puStack_a70[2] + 1;
        UNLOCK();
      }
      local_988 = local_af8;
      uStack_980 = puStack_af0;
      uVar15 = FUN_1407e5240(local_978,&local_988);
      uVar15 = FUN_141e22b00(local_6d0,&local_a78,uVar15);
      FUN_141e08fe0(local_7a8,uVar15);
      if (local_6c8 != (longlong *)0x0) {
        LOCK();
        plVar5 = local_6c8 + 1;
        lVar16 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar16 == 1) {
          (**(code **)*local_6c8)(local_6c8);
          LOCK();
          piVar1 = (int *)((longlong)local_6c8 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*local_6c8 + 8))(local_6c8,1);
          }
        }
      }
      FUN_1407e58c0(local_978);
      lVar16 = 3;
      pcVar23 = local_170;
      do {
        lVar16 = lVar16 + -1;
        pcVar2 = pcVar23 + -0x30;
        if (*pcVar2 != '\0') {
          *pcVar2 = '\0';
          plVar5 = *(longlong **)(pcVar23 + -0x40);
          if (plVar5 != (longlong *)0x0) {
            LOCK();
            plVar3 = plVar5 + 1;
            lVar13 = *plVar3;
            *(int *)plVar3 = (int)*plVar3 + -1;
            UNLOCK();
            if ((int)lVar13 == 1) {
              (**(code **)*plVar5)(plVar5);
              LOCK();
              piVar1 = (int *)((longlong)plVar5 + 0xc);
              iVar4 = *piVar1;
              *piVar1 = *piVar1 + -1;
              UNLOCK();
              if (iVar4 == 1) {
                (**(code **)(*plVar5 + 8))(plVar5,1);
              }
            }
          }
        }
        pcVar23 = pcVar2;
      } while (lVar16 != 0);
      plVar19 = (longlong *)plVar19[1];
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar16 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar16 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      plVar19 = *(longlong **)(lVar18 + 8);
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar16 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar16 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      plVar19 = *(longlong **)(local_b20 + 8);
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar16 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar16 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      plVar19 = *(longlong **)(local_b28 + 8);
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar16 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar16 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      puVar20 = local_7a8;
      bVar12 = true;
      bVar11 = true;
      bVar10 = true;
      bVar9 = false;
      bVar24 = false;
      bVar8 = false;
      bVar7 = true;
      bVar6 = false;
    }
    FUN_141e0d0b0(local_a38,puVar20);
    if ((bVar6) && (local_7b8 != (longlong *)0x0)) {
      LOCK();
      plVar19 = local_7b8 + 1;
      lVar16 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar16 == 1) {
        (**(code **)*local_7b8)(local_7b8);
        LOCK();
        piVar1 = (int *)((longlong)local_7b8 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_7b8 + 8))(local_7b8,1);
        }
      }
    }
    if ((bVar8) && (local_ae8[0] != 0)) {
      FUN_141de9200();
    }
    if ((bVar24) && (local_680 != (longlong *)0x0)) {
      LOCK();
      plVar19 = local_680 + 1;
      lVar16 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar16 == 1) {
        (**(code **)*local_680)(local_680);
        LOCK();
        piVar1 = (int *)((longlong)local_680 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_680 + 8))(local_680,1);
        }
      }
    }
    if ((bVar9) && (local_668 != (longlong *)0x0)) {
      LOCK();
      plVar19 = local_668 + 1;
      lVar16 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar16 == 1) {
        (**(code **)*local_668)(local_668);
        LOCK();
        piVar1 = (int *)((longlong)local_668 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_668 + 8))(local_668,1);
        }
      }
    }
    if ((bVar10) && (local_7a0 != (longlong *)0x0)) {
      LOCK();
      plVar19 = local_7a0 + 1;
      lVar16 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar16 == 1) {
        (**(code **)*local_7a0)(local_7a0);
        LOCK();
        piVar1 = (int *)((longlong)local_7a0 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_7a0 + 8))(local_7a0,1);
        }
      }
    }
    if ((bVar11) && (local_770 != (longlong *)0x0)) {
      LOCK();
      plVar19 = local_770 + 1;
      lVar16 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar16 == 1) {
        (**(code **)*local_770)(local_770);
        LOCK();
        piVar1 = (int *)((longlong)local_770 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_770 + 8))(local_770,1);
        }
      }
    }
    if ((bVar12) && (local_948[0] != 0)) {
      FUN_141de9200();
    }
    param_3 = local_b18;
    if ((bVar7) && (local_ae8[2] != 0)) {
      FUN_141de9200();
      param_3 = local_b18;
    }
  }
  bVar6 = false;
  bVar24 = false;
  FUN_141e08540(local_9a0);
  lVar16 = 2;
  local_98c = 2;
  FUN_141e09010(local_a58);
  if (*(char *)(param_1 + 0x34) == '\0') {
    uVar15 = FUN_141e22aa0(local_640,L"don\'t score",L"EnvQueryGenerator",L"DontScore");
    FUN_141e0d0b0(local_a58,uVar15);
    local_7e8 = local_638;
  }
  else if (*(char *)(param_1 + 0xe1) == '\x04') {
    if (*(longlong *)(param_1 + 0x158) == 0) {
      local_ae8[4] = 0;
      local_ae8[5] = 5;
      local_a00 = 0;
      plStack_9f8 = (longlong *)0x0;
      FUN_1407e2ee0(local_ae8 + 4,0);
      local_b38 = 0x3f;
      FUN_1407e4a20(local_ae8[4],5,&DAT_144eea294);
      bVar10 = true;
      bVar9 = true;
      bVar8 = true;
      bVar7 = true;
      lVar18 = FUN_141e150a0(local_480,*(uint *)(param_1 + 0x170) & DAT_14391b650,local_9a0,
                             &local_a00);
      uVar15 = FUN_141e24190(local_610,local_ae8 + 4);
      puVar22 = (undefined8 *)FUN_141e09170(local_580,uVar15);
      local_378[0] = 4;
      FUN_141e08fe0(local_368,lVar18);
      local_b08 = local_378;
      local_350 = '\x01';
      puStack_b00 = local_348;
      local_a10 = *puVar22;
      local_a08 = puVar22[1];
      if (local_a08 != 0) {
        LOCK();
        *(int *)(local_a08 + 8) = *(int *)(local_a08 + 8) + 1;
        UNLOCK();
      }
      local_918 = local_b08;
      uStack_910 = puStack_b00;
      uVar15 = FUN_1407e5240(local_908,&local_918);
      uVar15 = FUN_141e22b00(local_628,&local_a10,uVar15);
      FUN_141e08fe0(local_790,uVar15);
      if (local_620 != (longlong *)0x0) {
        LOCK();
        plVar19 = local_620 + 1;
        lVar13 = *plVar19;
        *(int *)plVar19 = (int)*plVar19 + -1;
        UNLOCK();
        if ((int)lVar13 == 1) {
          (**(code **)*local_620)(local_620);
          LOCK();
          piVar1 = (int *)((longlong)local_620 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*local_620 + 8))(local_620,1);
          }
        }
      }
      FUN_1407e58c0(local_908);
      if ((local_350 != '\0') && (local_350 = '\0', local_360 != (longlong *)0x0)) {
        LOCK();
        plVar19 = local_360 + 1;
        lVar13 = *plVar19;
        *(int *)plVar19 = (int)*plVar19 + -1;
        UNLOCK();
        if ((int)lVar13 == 1) {
          (**(code **)*local_360)(local_360);
          LOCK();
          piVar1 = (int *)((longlong)local_360 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*local_360 + 8))(local_360,1);
          }
        }
      }
      plVar19 = (longlong *)puVar22[1];
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar13 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar13 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      plVar19 = *(longlong **)(lVar18 + 8);
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar18 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar18 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      puVar20 = local_790;
    }
    else {
      plVar19 = *(longlong **)(param_1 + 0x158);
      bVar24 = plVar19 == (longlong *)0x0;
      if (bVar24) {
        plVar19 = (longlong *)
                  (*(code *)**(undefined8 **)(param_1 + 0x148))
                            ((undefined8 *)(param_1 + 0x148),local_938);
      }
      else {
        plVar19 = (longlong *)
                  (**(code **)(*plVar19 + 0x248))
                            (plVar19,local_928,*(undefined8 *)(param_1 + 0x160));
      }
      local_af8 = (undefined8 *)*plVar19;
      *plVar19 = 0;
      puStack_af0 = (undefined4 *)plVar19[1];
      plVar19[1] = 0;
      if ((bVar24) && (local_938[0] != 0)) {
        FUN_141de9200();
      }
      if ((!bVar24) && (local_928[0] != 0)) {
        FUN_141de9200();
      }
      bVar24 = true;
      bVar10 = false;
      bVar9 = false;
      bVar8 = false;
      bVar7 = false;
      bVar6 = true;
      puVar20 = (undefined1 *)FUN_141e24190(local_5f8,&local_af8);
    }
    FUN_141e08fe0(local_850,puVar20);
    if ((bVar7) && (local_788 != (longlong *)0x0)) {
      LOCK();
      plVar19 = local_788 + 1;
      lVar18 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*local_788)(local_788);
        LOCK();
        piVar1 = (int *)((longlong)local_788 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_788 + 8))(local_788,1);
        }
      }
    }
    if ((bVar8) && (local_608 != (longlong *)0x0)) {
      LOCK();
      plVar19 = local_608 + 1;
      lVar18 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*local_608)(local_608);
        LOCK();
        piVar1 = (int *)((longlong)local_608 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_608 + 8))(local_608,1);
        }
      }
    }
    if ((bVar9) && (local_ae8[4] != 0)) {
      FUN_141de9200();
    }
    plVar19 = plStack_9f8;
    if ((bVar10) && (plStack_9f8 != (longlong *)0x0)) {
      LOCK();
      plVar5 = plStack_9f8 + 1;
      lVar18 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*plStack_9f8)(plStack_9f8);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    if ((bVar24) && (local_5f0 != (longlong *)0x0)) {
      LOCK();
      plVar19 = local_5f0 + 1;
      lVar18 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*local_5f0)(local_5f0);
        LOCK();
        piVar1 = (int *)((longlong)local_5f0 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_5f0 + 8))(local_5f0,1);
        }
      }
    }
    if ((bVar6) && (local_af8 != (undefined8 *)0x0)) {
      FUN_141de9200();
    }
    local_ae8[6] = 0;
    local_ae8[7] = 10;
    FUN_1407e2ee0(local_ae8 + 6,0);
    local_b38 = 0x3f;
    FUN_1407e4a20(local_ae8[6],10,"{0} [{1}]");
    lVar18 = FUN_141e08fe0(local_468,local_850);
    local_b28 = lVar18;
    puVar21 = (undefined4 *)
              FUN_141e22aa0(local_450,L"constant score",L"EnvQueryGenerator",L"ScoreConstant");
    local_b08 = puVar21;
    uVar15 = FUN_141e24190(local_5c8,local_ae8 + 6);
    puVar22 = (undefined8 *)FUN_141e09170(local_570,uVar15);
    local_348[0] = 4;
    FUN_141e08fe0(local_338,puVar21);
    local_320 = 1;
    local_318 = 4;
    FUN_141e08fe0(local_308,lVar18);
    local_b18 = local_348;
    local_2f0 = 1;
    puStack_b10 = local_2e8;
    local_9f0 = *puVar22;
    local_9e8 = puVar22[1];
    if (local_9e8 != 0) {
      LOCK();
      *(int *)(local_9e8 + 8) = *(int *)(local_9e8 + 8) + 1;
      UNLOCK();
    }
    local_8f8 = local_b18;
    uStack_8f0 = puStack_b10;
    uVar15 = FUN_1407e5240(local_8e8,&local_8f8);
    uVar15 = FUN_141e22b00(local_5e0,&local_9f0,uVar15);
    FUN_141e08fe0(local_868,uVar15);
    if (local_5d8 != (longlong *)0x0) {
      LOCK();
      plVar19 = local_5d8 + 1;
      lVar18 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*local_5d8)(local_5d8);
        LOCK();
        piVar1 = (int *)((longlong)local_5d8 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_5d8 + 8))(local_5d8,1);
        }
      }
    }
    FUN_1407e58c0(local_8e8);
    lVar18 = 2;
    pcVar23 = local_2c0;
    do {
      lVar18 = lVar18 + -1;
      pcVar2 = pcVar23 + -0x30;
      if (*pcVar2 != '\0') {
        *pcVar2 = '\0';
        plVar19 = *(longlong **)(pcVar23 + -0x40);
        if (plVar19 != (longlong *)0x0) {
          LOCK();
          plVar5 = plVar19 + 1;
          lVar13 = *plVar5;
          *(int *)plVar5 = (int)*plVar5 + -1;
          UNLOCK();
          if ((int)lVar13 == 1) {
            (**(code **)*plVar19)(plVar19);
            LOCK();
            piVar1 = (int *)((longlong)plVar19 + 0xc);
            iVar4 = *piVar1;
            *piVar1 = *piVar1 + -1;
            UNLOCK();
            if (iVar4 == 1) {
              (**(code **)(*plVar19 + 8))(plVar19,1);
            }
          }
        }
      }
      pcVar23 = pcVar2;
    } while (lVar18 != 0);
    plVar19 = (longlong *)puVar22[1];
    if (plVar19 != (longlong *)0x0) {
      LOCK();
      plVar5 = plVar19 + 1;
      lVar18 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*plVar19)(plVar19);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    plVar19 = *(longlong **)(local_b08 + 2);
    if (plVar19 != (longlong *)0x0) {
      LOCK();
      plVar5 = plVar19 + 1;
      lVar18 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*plVar19)(plVar19);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    plVar19 = *(longlong **)(local_b28 + 8);
    if (plVar19 != (longlong *)0x0) {
      LOCK();
      plVar5 = plVar19 + 1;
      lVar18 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*plVar19)(plVar19);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    FUN_141e0d0b0(local_a58,local_868);
    if (local_860 != (longlong *)0x0) {
      LOCK();
      plVar19 = local_860 + 1;
      lVar18 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*local_860)(local_860);
        LOCK();
        piVar1 = (int *)((longlong)local_860 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_860 + 8))(local_860,1);
        }
      }
    }
    if (local_5c0 != (longlong *)0x0) {
      LOCK();
      plVar19 = local_5c0 + 1;
      lVar18 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*local_5c0)(local_5c0);
        LOCK();
        piVar1 = (int *)((longlong)local_5c0 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_5c0 + 8))(local_5c0,1);
        }
      }
    }
    if (local_ae8[6] != 0) {
      FUN_141de9200();
    }
    local_7e8 = local_848;
  }
  else {
    if (*(longlong *)(param_1 + 0x158) != 0) {
      local_ae8[8] = 0;
      local_ae8[9] = 9;
      FUN_1407e2ee0(local_ae8 + 8,0);
      local_b38 = 0x3f;
      FUN_1407e4a20(local_ae8[8],9,"{0}: {1}");
      uVar15 = FUN_1433adac0(param_1 + 0x148,local_8b8);
      lVar18 = FUN_141e24190(local_438,uVar15);
      local_b28 = lVar18;
      puVar21 = (undefined4 *)
                FUN_141e22aa0(local_420,L"score factor",L"EnvQueryGenerator",L"ScoreFactor");
      local_b08 = puVar21;
      uVar15 = FUN_141e24190(local_760,local_ae8 + 8);
      puVar22 = (undefined8 *)FUN_141e09170(local_560,uVar15);
      local_2e8[0] = 4;
      FUN_141e08fe0(local_2d8,puVar21);
      local_2c0[0] = '\x01';
      local_2b8 = 4;
      FUN_141e08fe0(local_2a8,lVar18);
      local_b18 = local_2e8;
      local_290 = 1;
      puStack_b10 = local_288;
      local_9e0 = *puVar22;
      local_9d8 = puVar22[1];
      if (local_9d8 != 0) {
        LOCK();
        *(int *)(local_9d8 + 8) = *(int *)(local_9d8 + 8) + 1;
        UNLOCK();
      }
      local_8d8 = local_b18;
      uStack_8d0 = puStack_b10;
      uVar15 = FUN_1407e5240(local_8c8,&local_8d8);
      uVar15 = FUN_141e22b00(local_5b0,&local_9e0,uVar15);
      FUN_141e08fe0(local_838,uVar15);
      if (local_5a8 != (longlong *)0x0) {
        LOCK();
        plVar19 = local_5a8 + 1;
        lVar18 = *plVar19;
        *(int *)plVar19 = (int)*plVar19 + -1;
        UNLOCK();
        if ((int)lVar18 == 1) {
          (**(code **)*local_5a8)(local_5a8);
          LOCK();
          piVar1 = (int *)((longlong)local_5a8 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*local_5a8 + 8))(local_5a8,1);
          }
        }
      }
      FUN_1407e58c0(local_8c8);
      lVar18 = 2;
      pcVar23 = local_260;
      do {
        lVar18 = lVar18 + -1;
        pcVar2 = pcVar23 + -0x30;
        if (*pcVar2 != '\0') {
          *pcVar2 = '\0';
          plVar19 = *(longlong **)(pcVar23 + -0x40);
          if (plVar19 != (longlong *)0x0) {
            LOCK();
            plVar5 = plVar19 + 1;
            lVar13 = *plVar5;
            *(int *)plVar5 = (int)*plVar5 + -1;
            UNLOCK();
            if ((int)lVar13 == 1) {
              (**(code **)*plVar19)(plVar19);
              LOCK();
              piVar1 = (int *)((longlong)plVar19 + 0xc);
              iVar4 = *piVar1;
              *piVar1 = *piVar1 + -1;
              UNLOCK();
              if (iVar4 == 1) {
                (**(code **)(*plVar19 + 8))(plVar19,1);
              }
            }
          }
        }
        pcVar23 = pcVar2;
      } while (lVar18 != 0);
      plVar19 = (longlong *)puVar22[1];
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar18 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar18 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      plVar19 = *(longlong **)(local_b08 + 2);
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar18 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar18 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      plVar19 = *(longlong **)(local_b28 + 8);
      if (plVar19 != (longlong *)0x0) {
        LOCK();
        plVar5 = plVar19 + 1;
        lVar18 = *plVar5;
        *(int *)plVar5 = (int)*plVar5 + -1;
        UNLOCK();
        if ((int)lVar18 == 1) {
          (**(code **)*plVar19)(plVar19);
          LOCK();
          piVar1 = (int *)((longlong)plVar19 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar19 + 8))(plVar19,1);
          }
        }
      }
      FUN_141e0d0b0(local_a58,local_838);
      if (local_830 != (longlong *)0x0) {
        LOCK();
        plVar19 = local_830 + 1;
        lVar18 = *plVar19;
        *(int *)plVar19 = (int)*plVar19 + -1;
        UNLOCK();
        if ((int)lVar18 == 1) {
          (**(code **)*local_830)(local_830);
          LOCK();
          piVar1 = (int *)((longlong)local_830 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*local_830 + 8))(local_830,1);
          }
        }
      }
      if (local_758 != (longlong *)0x0) {
        LOCK();
        plVar19 = local_758 + 1;
        lVar18 = *plVar19;
        *(int *)plVar19 = (int)*plVar19 + -1;
        UNLOCK();
        if ((int)lVar18 == 1) {
          (**(code **)*local_758)(local_758);
          LOCK();
          piVar1 = (int *)((longlong)local_758 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*local_758 + 8))(local_758,1);
          }
        }
      }
      if (local_8b8[0] != 0) {
        FUN_141de9200();
      }
      if (local_ae8[8] != 0) {
        FUN_141de9200();
      }
      goto LAB_1433910a9;
    }
    bVar24 = *(float *)(param_1 + 0x170) <= 0.0;
    if (bVar24) {
      uVar15 = FUN_141e22aa0(local_748,L"not ",L"EnvQueryGenerator",L"NotWithSpace");
    }
    else {
      uVar15 = FUN_140869340();
      uVar15 = FUN_141e08fe0(local_730,uVar15);
    }
    FUN_141e08fe0(local_7f0,uVar15);
    if ((bVar24) && (local_740 != (longlong *)0x0)) {
      LOCK();
      plVar19 = local_740 + 1;
      lVar18 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*local_740)(local_740);
        LOCK();
        piVar1 = (int *)((longlong)local_740 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_740 + 8))(local_740,1);
        }
      }
    }
    if ((!bVar24) && (local_728 != (longlong *)0x0)) {
      LOCK();
      plVar19 = local_728 + 1;
      lVar18 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*local_728)(local_728);
        LOCK();
        piVar1 = (int *)((longlong)local_728 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_728 + 8))(local_728,1);
        }
      }
    }
    local_9d0 = 0;
    plStack_9c8 = (longlong *)0x0;
    FUN_141e150a0(local_808,*(uint *)(param_1 + 0x170) & DAT_14391b650,local_9a0,&local_9d0);
    plVar19 = plStack_9c8;
    if (plStack_9c8 != (longlong *)0x0) {
      LOCK();
      plVar5 = plStack_9c8 + 1;
      lVar18 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*plStack_9c8)(plStack_9c8);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    local_ae8[10] = 0;
    local_ae8[0xb] = 0x12;
    FUN_1407e2ee0(local_ae8 + 10,0);
    local_b38 = 0x3f;
    FUN_1407e4a20(local_ae8[10],0x12,"{0} {1}{2} [x{3}]");
    local_b18 = (undefined4 *)FUN_141e08fe0(local_408,local_808);
    uVar17 = FUN_141e241b0(local_3f0,param_3);
    local_b20 = uVar17;
    lVar18 = FUN_141e08fe0(local_3d8,local_7f0);
    local_b28 = lVar18;
    puVar21 = (undefined4 *)FUN_141e22aa0(local_3c0,L"prefer",L"EnvQueryGenerator",L"ScorePrefer");
    local_b08 = puVar21;
    uVar15 = FUN_141e24190(local_700,local_ae8 + 10);
    puVar22 = (undefined8 *)FUN_141e09170(local_550,uVar15);
    local_108[0] = 4;
    FUN_141e08fe0(local_f8,puVar21);
    local_e0[0] = '\x01';
    local_d8 = 4;
    FUN_141e08fe0(local_c8,lVar18);
    local_b0 = 1;
    local_a8 = 4;
    FUN_141e08fe0(local_98,uVar17);
    local_80 = 1;
    local_78 = 4;
    FUN_141e08fe0(local_68,local_b18);
    local_a68 = local_108;
    local_50 = 1;
    puStack_a60 = local_48;
    local_9c0 = *puVar22;
    local_9b8 = puVar22[1];
    if (local_9b8 != 0) {
      LOCK();
      *(int *)(local_9b8 + 8) = *(int *)(local_9b8 + 8) + 1;
      UNLOCK();
    }
    local_8a8 = local_a68;
    uStack_8a0 = puStack_a60;
    uVar15 = FUN_1407e5240(local_898,&local_8a8);
    uVar15 = FUN_141e22b00(local_718,&local_9c0,uVar15);
    FUN_141e08fe0(local_820,uVar15);
    if (local_710 != (longlong *)0x0) {
      LOCK();
      plVar19 = local_710 + 1;
      lVar18 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*local_710)(local_710);
        LOCK();
        piVar1 = (int *)((longlong)local_710 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_710 + 8))(local_710,1);
        }
      }
    }
    FUN_1407e58c0(local_898);
    lVar18 = 4;
    pcVar23 = &stack0xffffffffffffffe0;
    do {
      lVar18 = lVar18 + -1;
      pcVar2 = pcVar23 + -0x30;
      if (*pcVar2 != '\0') {
        *pcVar2 = '\0';
        plVar19 = *(longlong **)(pcVar23 + -0x40);
        if (plVar19 != (longlong *)0x0) {
          LOCK();
          plVar5 = plVar19 + 1;
          lVar13 = *plVar5;
          *(int *)plVar5 = (int)*plVar5 + -1;
          UNLOCK();
          if ((int)lVar13 == 1) {
            (**(code **)*plVar19)(plVar19);
            LOCK();
            piVar1 = (int *)((longlong)plVar19 + 0xc);
            iVar4 = *piVar1;
            *piVar1 = *piVar1 + -1;
            UNLOCK();
            if (iVar4 == 1) {
              (**(code **)(*plVar19 + 8))(plVar19,1);
            }
          }
        }
      }
      pcVar23 = pcVar2;
    } while (lVar18 != 0);
    plVar19 = (longlong *)puVar22[1];
    if (plVar19 != (longlong *)0x0) {
      LOCK();
      plVar5 = plVar19 + 1;
      lVar18 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*plVar19)(plVar19);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    plVar19 = *(longlong **)(local_b08 + 2);
    if (plVar19 != (longlong *)0x0) {
      LOCK();
      plVar5 = plVar19 + 1;
      lVar18 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*plVar19)(plVar19);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    plVar19 = *(longlong **)(local_b28 + 8);
    if (plVar19 != (longlong *)0x0) {
      LOCK();
      plVar5 = plVar19 + 1;
      lVar18 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*plVar19)(plVar19);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    plVar19 = *(longlong **)(local_b20 + 8);
    if (plVar19 != (longlong *)0x0) {
      LOCK();
      plVar5 = plVar19 + 1;
      lVar18 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*plVar19)(plVar19);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    plVar19 = *(longlong **)(local_b18 + 2);
    if (plVar19 != (longlong *)0x0) {
      LOCK();
      plVar5 = plVar19 + 1;
      lVar18 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*plVar19)(plVar19);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    FUN_141e0d0b0(local_a58,local_820);
    if (local_818 != (longlong *)0x0) {
      LOCK();
      plVar19 = local_818 + 1;
      lVar18 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*local_818)(local_818);
        LOCK();
        piVar1 = (int *)((longlong)local_818 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_818 + 8))(local_818,1);
        }
      }
    }
    if (local_6f8 != (longlong *)0x0) {
      LOCK();
      plVar19 = local_6f8 + 1;
      lVar18 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*local_6f8)(local_6f8);
        LOCK();
        piVar1 = (int *)((longlong)local_6f8 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_6f8 + 8))(local_6f8,1);
        }
      }
    }
    if (local_ae8[10] != 0) {
      FUN_141de9200();
    }
    if (local_800 != (longlong *)0x0) {
      LOCK();
      plVar19 = local_800 + 1;
      lVar18 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar18 == 1) {
        (**(code **)*local_800)(local_800);
        LOCK();
        piVar1 = (int *)((longlong)local_800 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_800 + 8))(local_800,1);
        }
      }
    }
  }
  local_8d8 = (undefined4 *)CONCAT44(local_8d8._4_4_,(undefined4)local_8d8);
  uStack_8d0 = (undefined4 *)CONCAT44(uStack_8d0._4_4_,(undefined4)uStack_8d0);
  if (local_7e8 != (longlong *)0x0) {
    LOCK();
    plVar19 = local_7e8 + 1;
    lVar18 = *plVar19;
    *(int *)plVar19 = (int)*plVar19 + -1;
    UNLOCK();
    local_8d8 = (undefined4 *)CONCAT44(local_8d8._4_4_,(undefined4)local_8d8);
    uStack_8d0 = (undefined4 *)CONCAT44(uStack_8d0._4_4_,(undefined4)uStack_8d0);
    if ((int)lVar18 == 1) {
      (**(code **)*local_7e8)(local_7e8);
      LOCK();
      piVar1 = (int *)((longlong)local_7e8 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*local_7e8 + 8))(local_7e8,1);
      }
    }
  }
LAB_1433910a9:
  cVar14 = FUN_141e2a480(local_a38);
  if (cVar14 == '\0') {
    local_ae8[0xd] = 9;
    local_ae8[0xc] = 0;
    FUN_1407e2ee0(local_ae8 + 0xc,0);
    local_b38 = 0x3f;
    FUN_1407e4a20(local_ae8[0xc],9,"{0}, {1}");
    puVar21 = (undefined4 *)FUN_141e08fe0(local_3a8,local_a58);
    local_b08 = puVar21;
    lVar18 = FUN_141e08fe0(local_390,local_a38);
    uVar15 = FUN_141e24190(local_6e8,local_ae8 + 0xc);
    puVar22 = (undefined8 *)FUN_141e09170(local_540,uVar15);
    local_288[0] = 4;
    FUN_141e08fe0(local_278,lVar18);
    local_260[0] = '\x01';
    local_258 = 4;
    FUN_141e08fe0(local_248,puVar21);
    local_a68 = local_288;
    local_230 = 1;
    puStack_a60 = (ulonglong *)local_228;
    local_9b0 = *puVar22;
    local_9a8 = puVar22[1];
    if (local_9a8 != 0) {
      LOCK();
      *(int *)(local_9a8 + 8) = *(int *)(local_9a8 + 8) + 1;
      UNLOCK();
    }
    local_888 = local_a68;
    uStack_880 = puStack_a60;
    uVar15 = FUN_1407e5240(local_878,&local_888);
    uVar15 = FUN_141e22b00(local_658,&local_9b0,uVar15);
    FUN_141e08fe0(local_7d8,uVar15);
    if (local_650 != (longlong *)0x0) {
      LOCK();
      plVar19 = local_650 + 1;
      lVar13 = *plVar19;
      *(int *)plVar19 = (int)*plVar19 + -1;
      UNLOCK();
      if ((int)lVar13 == 1) {
        (**(code **)*local_650)(local_650);
        LOCK();
        piVar1 = (int *)((longlong)local_650 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_650 + 8))(local_650,1);
        }
      }
    }
    FUN_1407e58c0(local_878);
    pcVar23 = local_200;
    do {
      lVar16 = lVar16 + -1;
      pcVar2 = pcVar23 + -0x30;
      if (*pcVar2 != '\0') {
        *pcVar2 = '\0';
        plVar19 = *(longlong **)(pcVar23 + -0x40);
        if (plVar19 != (longlong *)0x0) {
          LOCK();
          plVar5 = plVar19 + 1;
          lVar13 = *plVar5;
          *(int *)plVar5 = (int)*plVar5 + -1;
          UNLOCK();
          if ((int)lVar13 == 1) {
            (**(code **)*plVar19)(plVar19);
            LOCK();
            piVar1 = (int *)((longlong)plVar19 + 0xc);
            iVar4 = *piVar1;
            *piVar1 = *piVar1 + -1;
            UNLOCK();
            if (iVar4 == 1) {
              (**(code **)(*plVar19 + 8))(plVar19,1);
            }
          }
        }
      }
      pcVar23 = pcVar2;
    } while (lVar16 != 0);
    plVar19 = (longlong *)puVar22[1];
    if (plVar19 != (longlong *)0x0) {
      LOCK();
      plVar5 = plVar19 + 1;
      lVar16 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar16 == 1) {
        (**(code **)*plVar19)(plVar19);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    plVar19 = *(longlong **)(lVar18 + 8);
    if (plVar19 != (longlong *)0x0) {
      LOCK();
      plVar5 = plVar19 + 1;
      lVar16 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar16 == 1) {
        (**(code **)*plVar19)(plVar19);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    plVar19 = *(longlong **)(local_b08 + 2);
    if (plVar19 != (longlong *)0x0) {
      LOCK();
      plVar5 = plVar19 + 1;
      lVar16 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar16 == 1) {
        (**(code **)*plVar19)(plVar19);
        LOCK();
        piVar1 = (int *)((longlong)plVar19 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar19 + 8))(plVar19,1);
        }
      }
    }
    puVar20 = local_7d8;
    bVar8 = false;
    bVar7 = true;
    bVar6 = true;
    bVar24 = true;
  }
  else {
    puVar20 = (undefined1 *)FUN_141e08fe0(local_598,local_a58);
    bVar8 = true;
    bVar7 = false;
    bVar6 = false;
    bVar24 = false;
  }
  uVar15 = local_a40;
  FUN_141e08fe0(local_a40,puVar20);
  if ((bVar24) && (local_7d0 != (longlong *)0x0)) {
    LOCK();
    plVar19 = local_7d0 + 1;
    lVar16 = *plVar19;
    *(int *)plVar19 = (int)*plVar19 + -1;
    UNLOCK();
    if ((int)lVar16 == 1) {
      (**(code **)*local_7d0)(local_7d0);
      LOCK();
      piVar1 = (int *)((longlong)local_7d0 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*local_7d0 + 8))(local_7d0,1);
      }
    }
  }
  if ((bVar6) && (local_6e0 != (longlong *)0x0)) {
    LOCK();
    plVar19 = local_6e0 + 1;
    lVar16 = *plVar19;
    *(int *)plVar19 = (int)*plVar19 + -1;
    UNLOCK();
    if ((int)lVar16 == 1) {
      (**(code **)*local_6e0)(local_6e0);
      LOCK();
      piVar1 = (int *)((longlong)local_6e0 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*local_6e0 + 8))(local_6e0,1);
      }
    }
  }
  if ((bVar7) && (local_ae8[0xc] != 0)) {
    FUN_141de9200();
  }
  if ((bVar8) && (local_590 != (longlong *)0x0)) {
    LOCK();
    plVar19 = local_590 + 1;
    lVar16 = *plVar19;
    *(int *)plVar19 = (int)*plVar19 + -1;
    UNLOCK();
    if ((int)lVar16 == 1) {
      (**(code **)*local_590)(local_590);
      LOCK();
      piVar1 = (int *)((longlong)local_590 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*local_590 + 8))(local_590,1);
      }
    }
  }
  if (local_a50 != (longlong *)0x0) {
    LOCK();
    plVar19 = local_a50 + 1;
    lVar16 = *plVar19;
    *(int *)plVar19 = (int)*plVar19 + -1;
    UNLOCK();
    if ((int)lVar16 == 1) {
      (**(code **)*local_a50)(local_a50);
      LOCK();
      piVar1 = (int *)((longlong)local_a50 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*local_a50 + 8))(local_a50,1);
      }
    }
  }
  if (local_a30 != (longlong *)0x0) {
    LOCK();
    plVar19 = local_a30 + 1;
    lVar16 = *plVar19;
    *(int *)plVar19 = (int)*plVar19 + -1;
    UNLOCK();
    if ((int)lVar16 == 1) {
      (**(code **)*local_a30)(local_a30);
      LOCK();
      piVar1 = (int *)((longlong)local_a30 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*local_a30 + 8))(local_a30,1);
      }
    }
  }
  return uVar15;
}



// =================   CALLER-OF 0x14338f660  FUN_1433be930 @ 1433be930  size=0x34f =================
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_1433be930(longlong param_1,undefined8 param_2)

{
  int *piVar1;
  char *pcVar2;
  longlong *plVar3;
  int iVar4;
  longlong *plVar5;
  longlong lVar6;
  longlong lVar7;
  longlong lVar8;
  undefined8 uVar9;
  undefined8 *puVar10;
  char *pcVar11;
  undefined1 auStack_1a8 [32];
  undefined2 local_188;
  longlong local_178 [4];
  undefined4 *local_158;
  ulonglong *puStack_150;
  longlong local_148;
  undefined8 local_140;
  longlong local_138;
  undefined1 local_130 [16];
  undefined1 local_120 [8];
  longlong *local_118;
  undefined1 local_108 [8];
  longlong *local_100;
  undefined1 local_f0 [16];
  undefined1 local_e0 [24];
  undefined1 local_c8 [32];
  undefined4 local_a8 [4];
  undefined1 local_98 [24];
  undefined1 local_80;
  undefined4 local_78;
  undefined1 local_68 [24];
  undefined1 local_50;
  ulonglong local_48 [2];
  
  local_48[0] = DAT_145909780 ^ (ulonglong)auStack_1a8;
  local_178[3] = 4;
  local_178[2] = 0;
  FUN_1407e2ee0(local_178 + 2,0);
  local_188 = 0x3f;
  FUN_1407e4a20(local_178[2],4,&DAT_144efc48c);
  local_178[0] = 0;
  local_178[1] = 8;
  FUN_1407e2ee0(local_178,0);
  local_188 = 0x3f;
  FUN_1407e4a20(local_178[0],8,"{0}\n{1}");
  lVar7 = FUN_14338f660(param_1,local_e0,local_178 + 2);
  local_148 = lVar7;
  lVar8 = FUN_1433ae3d0(param_1 + 0x1c0,local_c8,1);
  uVar9 = FUN_141e24190(local_108,local_178);
  puVar10 = (undefined8 *)FUN_141e09170(local_f0,uVar9);
  local_a8[0] = 4;
  FUN_141e08fe0(local_98,lVar8);
  local_80 = 1;
  local_78 = 4;
  FUN_141e08fe0(local_68,lVar7);
  local_50 = 1;
  puStack_150 = local_48;
  local_140 = *puVar10;
  local_138 = puVar10[1];
  if (local_138 != 0) {
    LOCK();
    *(int *)(local_138 + 8) = *(int *)(local_138 + 8) + 1;
    UNLOCK();
  }
  local_158 = local_a8;
  uVar9 = FUN_1407e5240(local_130,&local_158);
  uVar9 = FUN_141e22b00(local_120,&local_140,uVar9);
  FUN_141e08fe0(param_2,uVar9);
  if (local_118 != (longlong *)0x0) {
    LOCK();
    plVar5 = local_118 + 1;
    lVar7 = *plVar5;
    *(int *)plVar5 = (int)*plVar5 + -1;
    UNLOCK();
    if ((int)lVar7 == 1) {
      (**(code **)*local_118)(local_118);
      LOCK();
      piVar1 = (int *)((longlong)local_118 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*local_118 + 8))(local_118,1);
      }
    }
  }
  FUN_1407e58c0(local_130);
  lVar7 = 2;
  pcVar11 = &stack0xffffffffffffffe0;
  do {
    lVar7 = lVar7 + -1;
    pcVar2 = pcVar11 + -0x30;
    if (*pcVar2 != '\0') {
      *pcVar2 = '\0';
      plVar5 = *(longlong **)(pcVar11 + -0x40);
      if (plVar5 != (longlong *)0x0) {
        LOCK();
        plVar3 = plVar5 + 1;
        lVar6 = *plVar3;
        *(int *)plVar3 = (int)*plVar3 + -1;
        UNLOCK();
        if ((int)lVar6 == 1) {
          (**(code **)*plVar5)(plVar5);
          LOCK();
          piVar1 = (int *)((longlong)plVar5 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*plVar5 + 8))(plVar5,1);
          }
        }
      }
    }
    pcVar11 = pcVar2;
  } while (lVar7 != 0);
  plVar5 = (longlong *)puVar10[1];
  if (plVar5 != (longlong *)0x0) {
    LOCK();
    plVar3 = plVar5 + 1;
    lVar7 = *plVar3;
    *(int *)plVar3 = (int)*plVar3 + -1;
    UNLOCK();
    if ((int)lVar7 == 1) {
      (**(code **)*plVar5)(plVar5);
      LOCK();
      piVar1 = (int *)((longlong)plVar5 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*plVar5 + 8))(plVar5,1);
      }
    }
  }
  plVar5 = *(longlong **)(lVar8 + 8);
  if (plVar5 != (longlong *)0x0) {
    LOCK();
    plVar3 = plVar5 + 1;
    lVar7 = *plVar3;
    *(int *)plVar3 = (int)*plVar3 + -1;
    UNLOCK();
    if ((int)lVar7 == 1) {
      (**(code **)*plVar5)(plVar5);
      LOCK();
      piVar1 = (int *)((longlong)plVar5 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*plVar5 + 8))(plVar5,1);
      }
    }
  }
  plVar5 = *(longlong **)(local_148 + 8);
  if (plVar5 != (longlong *)0x0) {
    LOCK();
    plVar3 = plVar5 + 1;
    lVar7 = *plVar3;
    *(int *)plVar3 = (int)*plVar3 + -1;
    UNLOCK();
    if ((int)lVar7 == 1) {
      (**(code **)*plVar5)(plVar5);
      LOCK();
      piVar1 = (int *)((longlong)plVar5 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*plVar5 + 8))(plVar5,1);
      }
    }
  }
  if (local_100 != (longlong *)0x0) {
    LOCK();
    plVar5 = local_100 + 1;
    lVar7 = *plVar5;
    *(int *)plVar5 = (int)*plVar5 + -1;
    UNLOCK();
    if ((int)lVar7 == 1) {
      (**(code **)*local_100)(local_100);
      LOCK();
      piVar1 = (int *)((longlong)local_100 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*local_100 + 8))(local_100,1);
      }
    }
  }
  if (local_178[0] != 0) {
    FUN_141de9200();
  }
  if (local_178[2] != 0) {
    FUN_141de9200();
  }
  return param_2;
}



// =================   CALLER-OF 0x14338f660  FUN_1433bdf90 @ 1433bdf90  size=0x92e =================
// strings: [discard unreachable, DiscardUnreachable, EnvQueryGenerator, {0}: {1}, existing path]
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_1433bdf90(longlong param_1,undefined8 param_2)

{
  int *piVar1;
  char *pcVar2;
  longlong *plVar3;
  int iVar4;
  longlong *plVar5;
  longlong lVar6;
  char cVar7;
  undefined8 uVar8;
  longlong lVar9;
  ulonglong uVar10;
  undefined8 *puVar11;
  longlong lVar12;
  char *pcVar13;
  longlong lVar14;
  bool bVar15;
  undefined1 auStack_388 [32];
  undefined2 local_368;
  undefined8 local_358;
  undefined4 *local_348;
  undefined4 *puStack_340;
  longlong local_338 [6];
  undefined4 *local_308;
  ulonglong *puStack_300;
  undefined1 local_2f8 [8];
  longlong *local_2f0;
  undefined1 local_2e0 [8];
  longlong *local_2d8;
  undefined1 local_2c8 [8];
  longlong *local_2c0;
  undefined8 local_2b0;
  longlong local_2a8;
  undefined8 local_2a0;
  longlong local_298;
  undefined8 local_288;
  undefined8 uStack_280;
  undefined1 local_278 [16];
  longlong local_268 [2];
  undefined4 *local_258;
  ulonglong *puStack_250;
  undefined1 local_248 [16];
  undefined1 local_238 [8];
  longlong *local_230;
  undefined1 local_220 [8];
  longlong *local_218;
  undefined1 local_208 [8];
  longlong *local_200;
  undefined1 local_1f0 [8];
  longlong *local_1e8;
  undefined1 local_1d8 [8];
  longlong *local_1d0;
  undefined1 local_1c0 [8];
  longlong *local_1b8;
  undefined1 local_1a8 [8];
  longlong *local_1a0;
  undefined1 local_190 [16];
  undefined1 local_180 [16];
  undefined1 local_170 [24];
  undefined1 local_158 [24];
  undefined1 local_140 [24];
  undefined1 local_128 [32];
  undefined4 local_108 [4];
  undefined1 local_f8 [24];
  undefined1 local_e0;
  undefined4 local_d8;
  undefined1 local_c8 [24];
  undefined1 local_b0;
  undefined4 local_a8 [4];
  undefined1 local_98 [24];
  char local_80 [8];
  undefined4 local_78;
  undefined1 local_68 [24];
  undefined1 local_50;
  ulonglong local_48 [2];
  
  local_48[0] = DAT_145909780 ^ (ulonglong)auStack_388;
  local_338[4] = local_338[4] & 0xffffffff00000000;
  local_358 = param_2;
  FUN_141e22aa0(local_2c8,L"discard unreachable",L"EnvQueryGenerator",L"DiscardUnreachable");
  FUN_141e09010(local_2f8);
  lVar14 = 2;
  if (*(longlong *)(param_1 + 0x210) == 0) {
    if (*(char *)(param_1 + 0x228) != '\0') {
      FUN_141e0d0e0(local_2f8,local_2c8);
    }
  }
  else {
    local_338[0] = 0;
    local_338[1] = 9;
    FUN_1407e2ee0(local_338,0);
    local_368 = 0x3f;
    FUN_1407e4a20(local_338[0],9,"{0}: {1}");
    uVar8 = FUN_1433adac0(param_1 + 0x200,local_268);
    lVar9 = FUN_141e24190(local_170,uVar8);
    local_308 = (undefined4 *)lVar9;
    uVar10 = FUN_141e08fe0(local_158,local_2c8);
    local_338[4] = uVar10;
    uVar8 = FUN_141e24190(local_208,local_338);
    puVar11 = (undefined8 *)FUN_141e09170(local_190,uVar8);
    local_108[0] = 4;
    FUN_141e08fe0(local_f8,uVar10);
    local_e0 = 1;
    local_d8 = 4;
    FUN_141e08fe0(local_c8,lVar9);
    local_348 = local_108;
    local_b0 = 1;
    puStack_340 = local_a8;
    local_2b0 = *puVar11;
    local_2a8 = puVar11[1];
    if (local_2a8 != 0) {
      LOCK();
      *(int *)(local_2a8 + 8) = *(int *)(local_2a8 + 8) + 1;
      UNLOCK();
    }
    local_288 = local_348;
    uStack_280 = puStack_340;
    uVar8 = FUN_1407e5240(local_278,&local_288);
    uVar8 = FUN_141e22b00(local_220,&local_2b0,uVar8);
    FUN_141e08fe0(local_238,uVar8);
    if (local_218 != (longlong *)0x0) {
      LOCK();
      plVar5 = local_218 + 1;
      lVar9 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar9 == 1) {
        (**(code **)*local_218)(local_218);
        LOCK();
        piVar1 = (int *)((longlong)local_218 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_218 + 8))(local_218,1);
        }
      }
    }
    FUN_1407e58c0(local_278);
    pcVar13 = local_80;
    lVar9 = lVar14;
    do {
      lVar9 = lVar9 + -1;
      pcVar2 = pcVar13 + -0x30;
      if (*pcVar2 != '\0') {
        *pcVar2 = '\0';
        plVar5 = *(longlong **)(pcVar13 + -0x40);
        if (plVar5 != (longlong *)0x0) {
          LOCK();
          plVar3 = plVar5 + 1;
          lVar12 = *plVar3;
          *(int *)plVar3 = (int)*plVar3 + -1;
          UNLOCK();
          if ((int)lVar12 == 1) {
            (**(code **)*plVar5)(plVar5);
            LOCK();
            piVar1 = (int *)((longlong)plVar5 + 0xc);
            iVar4 = *piVar1;
            *piVar1 = *piVar1 + -1;
            UNLOCK();
            if (iVar4 == 1) {
              (**(code **)(*plVar5 + 8))(plVar5,1);
            }
          }
        }
      }
      pcVar13 = pcVar2;
    } while (lVar9 != 0);
    plVar5 = (longlong *)puVar11[1];
    if (plVar5 != (longlong *)0x0) {
      LOCK();
      plVar3 = plVar5 + 1;
      lVar9 = *plVar3;
      *(int *)plVar3 = (int)*plVar3 + -1;
      UNLOCK();
      if ((int)lVar9 == 1) {
        (**(code **)*plVar5)(plVar5);
        LOCK();
        piVar1 = (int *)((longlong)plVar5 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar5 + 8))(plVar5,1);
        }
      }
    }
    plVar5 = *(longlong **)(local_338[4] + 8);
    if (plVar5 != (longlong *)0x0) {
      LOCK();
      plVar3 = plVar5 + 1;
      lVar9 = *plVar3;
      *(int *)plVar3 = (int)*plVar3 + -1;
      UNLOCK();
      if ((int)lVar9 == 1) {
        (**(code **)*plVar5)(plVar5);
        LOCK();
        piVar1 = (int *)((longlong)plVar5 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar5 + 8))(plVar5,1);
        }
      }
    }
    plVar5 = *(longlong **)((longlong)local_308 + 8);
    if (plVar5 != (longlong *)0x0) {
      LOCK();
      plVar3 = plVar5 + 1;
      lVar9 = *plVar3;
      *(int *)plVar3 = (int)*plVar3 + -1;
      UNLOCK();
      if ((int)lVar9 == 1) {
        (**(code **)*plVar5)(plVar5);
        LOCK();
        piVar1 = (int *)((longlong)plVar5 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar5 + 8))(plVar5,1);
        }
      }
    }
    FUN_141e0d0b0(local_2f8,local_238);
    if (local_230 != (longlong *)0x0) {
      LOCK();
      plVar5 = local_230 + 1;
      lVar9 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar9 == 1) {
        (**(code **)*local_230)(local_230);
        LOCK();
        piVar1 = (int *)((longlong)local_230 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_230 + 8))(local_230,1);
        }
      }
    }
    if (local_200 != (longlong *)0x0) {
      LOCK();
      plVar5 = local_200 + 1;
      lVar9 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar9 == 1) {
        (**(code **)*local_200)(local_200);
        LOCK();
        piVar1 = (int *)((longlong)local_200 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_200 + 8))(local_200,1);
        }
      }
    }
    if (local_268[0] != 0) {
      FUN_141de9200();
    }
    param_2 = local_358;
    if (local_338[0] != 0) {
      FUN_141de9200();
      param_2 = local_358;
    }
  }
  bVar15 = (*(byte *)(param_1 + 0x1b8) & 1) == 0;
  if (bVar15) {
    local_338[2] = 0;
    local_338[3] = 0xe;
    FUN_1407e2ee0(local_338 + 2,0);
    local_368 = 0x3f;
    FUN_1407e4a20(local_338[2],0xe,"existing path");
    uVar8 = FUN_14338f660(param_1,local_1f0,local_338 + 2);
  }
  else {
    uVar8 = FUN_143391540(param_1,local_1d8);
  }
  FUN_141e08fe0(local_2e0,uVar8);
  if ((bVar15) && (local_1e8 != (longlong *)0x0)) {
    LOCK();
    plVar5 = local_1e8 + 1;
    lVar9 = *plVar5;
    *(int *)plVar5 = (int)*plVar5 + -1;
    UNLOCK();
    if ((int)lVar9 == 1) {
      (**(code **)*local_1e8)(local_1e8);
      LOCK();
      piVar1 = (int *)((longlong)local_1e8 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*local_1e8 + 8))(local_1e8,1);
      }
    }
  }
  if ((bVar15) && (local_338[2] != 0)) {
    FUN_141de9200();
  }
  if ((!bVar15) && (local_1d0 != (longlong *)0x0)) {
    LOCK();
    plVar5 = local_1d0 + 1;
    lVar9 = *plVar5;
    *(int *)plVar5 = (int)*plVar5 + -1;
    UNLOCK();
    if ((int)lVar9 == 1) {
      (**(code **)*local_1d0)(local_1d0);
      LOCK();
      piVar1 = (int *)((longlong)local_1d0 + 0xc);
      iVar4 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar4 == 1) {
        (**(code **)(*local_1d0 + 8))(local_1d0,1);
      }
    }
  }
  cVar7 = FUN_141e2a480(local_2f8);
  if (cVar7 == '\0') {
    local_348 = (undefined4 *)0x0;
    puStack_340 = (undefined4 *)0x8;
    FUN_1407e2ee0(&local_348,0);
    local_368 = 0x3f;
    FUN_1407e4a20(local_348,8,"{0}\n{1}");
    lVar9 = FUN_141e08fe0(local_140,local_2e0);
    lVar12 = FUN_141e08fe0(local_128,local_2f8);
    uVar8 = FUN_141e24190(local_1a8,&local_348);
    puVar11 = (undefined8 *)FUN_141e09170(local_180,uVar8);
    local_a8[0] = 4;
    FUN_141e08fe0(local_98,lVar12);
    local_80[0] = '\x01';
    local_78 = 4;
    FUN_141e08fe0(local_68,lVar9);
    local_308 = local_a8;
    local_50 = 1;
    puStack_300 = local_48;
    local_2a0 = *puVar11;
    local_298 = puVar11[1];
    if (local_298 != 0) {
      LOCK();
      *(int *)(local_298 + 8) = *(int *)(local_298 + 8) + 1;
      UNLOCK();
    }
    local_258 = local_308;
    puStack_250 = puStack_300;
    uVar8 = FUN_1407e5240(local_248,&local_258);
    uVar8 = FUN_141e22b00(local_1c0,&local_2a0,uVar8);
    FUN_141e08fe0(local_358,uVar8);
    if (local_1b8 != (longlong *)0x0) {
      LOCK();
      plVar5 = local_1b8 + 1;
      lVar6 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar6 == 1) {
        (**(code **)*local_1b8)(local_1b8);
        LOCK();
        piVar1 = (int *)((longlong)local_1b8 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_1b8 + 8))(local_1b8,1);
        }
      }
    }
    FUN_1407e58c0(local_248);
    pcVar13 = &stack0xffffffffffffffe0;
    do {
      lVar14 = lVar14 + -1;
      pcVar2 = pcVar13 + -0x30;
      if (*pcVar2 != '\0') {
        *pcVar2 = '\0';
        plVar5 = *(longlong **)(pcVar13 + -0x40);
        if (plVar5 != (longlong *)0x0) {
          LOCK();
          plVar3 = plVar5 + 1;
          lVar6 = *plVar3;
          *(int *)plVar3 = (int)*plVar3 + -1;
          UNLOCK();
          if ((int)lVar6 == 1) {
            (**(code **)*plVar5)(plVar5);
            LOCK();
            piVar1 = (int *)((longlong)plVar5 + 0xc);
            iVar4 = *piVar1;
            *piVar1 = *piVar1 + -1;
            UNLOCK();
            if (iVar4 == 1) {
              (**(code **)(*plVar5 + 8))(plVar5,1);
            }
          }
        }
      }
      pcVar13 = pcVar2;
    } while (lVar14 != 0);
    plVar5 = (longlong *)puVar11[1];
    if (plVar5 != (longlong *)0x0) {
      LOCK();
      plVar3 = plVar5 + 1;
      lVar14 = *plVar3;
      *(int *)plVar3 = (int)*plVar3 + -1;
      UNLOCK();
      if ((int)lVar14 == 1) {
        (**(code **)*plVar5)(plVar5);
        LOCK();
        piVar1 = (int *)((longlong)plVar5 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar5 + 8))(plVar5,1);
        }
      }
    }
    plVar5 = *(longlong **)(lVar12 + 8);
    if (plVar5 != (longlong *)0x0) {
      LOCK();
      plVar3 = plVar5 + 1;
      lVar14 = *plVar3;
      *(int *)plVar3 = (int)*plVar3 + -1;
      UNLOCK();
      if ((int)lVar14 == 1) {
        (**(code **)*plVar5)(plVar5);
        LOCK();
        piVar1 = (int *)((longlong)plVar5 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar5 + 8))(plVar5,1);
        }
      }
    }
    plVar5 = *(longlong **)(lVar9 + 8);
    if (plVar5 != (longlong *)0x0) {
      LOCK();
      plVar3 = plVar5 + 1;
      lVar14 = *plVar3;
      *(int *)plVar3 = (int)*plVar3 + -1;
      UNLOCK();
      if ((int)lVar14 == 1) {
        (**(code **)*plVar5)(plVar5);
        LOCK();
        piVar1 = (int *)((longlong)plVar5 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*plVar5 + 8))(plVar5,1);
        }
      }
    }
    if (local_1a0 != (longlong *)0x0) {
      LOCK();
      plVar5 = local_1a0 + 1;
      lVar14 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar14 == 1) {
        (**(code **)*local_1a0)(local_1a0);
        LOCK();
        piVar1 = (int *)((longlong)local_1a0 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_1a0 + 8))(local_1a0,1);
        }
      }
    }
    if (local_348 != (undefined4 *)0x0) {
      FUN_141de9200();
    }
    if (local_2d8 != (longlong *)0x0) {
      LOCK();
      plVar5 = local_2d8 + 1;
      lVar14 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar14 == 1) {
        (**(code **)*local_2d8)(local_2d8);
        LOCK();
        piVar1 = (int *)((longlong)local_2d8 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_2d8 + 8))(local_2d8,1);
        }
      }
    }
    if (local_2f0 != (longlong *)0x0) {
      LOCK();
      plVar5 = local_2f0 + 1;
      lVar14 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar14 == 1) {
        (**(code **)*local_2f0)(local_2f0);
        LOCK();
        piVar1 = (int *)((longlong)local_2f0 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_2f0 + 8))(local_2f0,1);
        }
      }
    }
    param_2 = local_358;
    if (local_2c0 != (longlong *)0x0) {
      LOCK();
      plVar5 = local_2c0 + 1;
      lVar14 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar14 == 1) {
        (**(code **)*local_2c0)(local_2c0);
        LOCK();
        piVar1 = (int *)((longlong)local_2c0 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        param_2 = local_358;
        if (iVar4 == 1) {
          (**(code **)(*local_2c0 + 8))(local_2c0,1);
          param_2 = local_358;
        }
      }
    }
  }
  else {
    FUN_141e08fe0(param_2,local_2e0);
    if (local_2d8 != (longlong *)0x0) {
      LOCK();
      plVar5 = local_2d8 + 1;
      lVar14 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar14 == 1) {
        (**(code **)*local_2d8)(local_2d8);
        LOCK();
        piVar1 = (int *)((longlong)local_2d8 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_2d8 + 8))(local_2d8,1);
        }
      }
    }
    if (local_2f0 != (longlong *)0x0) {
      LOCK();
      plVar5 = local_2f0 + 1;
      lVar14 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar14 == 1) {
        (**(code **)*local_2f0)(local_2f0);
        LOCK();
        piVar1 = (int *)((longlong)local_2f0 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_2f0 + 8))(local_2f0,1);
        }
      }
    }
    if (local_2c0 != (longlong *)0x0) {
      LOCK();
      plVar5 = local_2c0 + 1;
      lVar14 = *plVar5;
      *(int *)plVar5 = (int)*plVar5 + -1;
      UNLOCK();
      if ((int)lVar14 == 1) {
        (**(code **)*local_2c0)(local_2c0);
        LOCK();
        piVar1 = (int *)((longlong)local_2c0 + 0xc);
        iVar4 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar4 == 1) {
          (**(code **)(*local_2c0 + 8))(local_2c0,1);
        }
      }
    }
  }
  return param_2;
}



// =================   CALLER-OF 0x14338f660  FUN_1433be8c0 @ 1433be8c0  size=0x70 =================
// strings: [projected]
// callers: <none>

undefined8 FUN_1433be8c0(undefined8 param_1,undefined8 param_2)

{
  longlong local_18 [2];
  
  local_18[1] = 10;
  local_18[0] = 0;
  FUN_1407e2ee0(local_18,0);
  FUN_141dd1c10(local_18[0],L"projected",0x14);
  FUN_14338f660(param_1,param_2,local_18);
  if (local_18[0] != 0) {
    FUN_141de9200();
  }
  return param_2;
}



// ================= TARGET 0x142d47c10  FUN_142d47c10 @ 142d47c10  size=0x92d =================
// callers: FUN_142d52470@142d52470 FUN_142d43b90@142d43b90 FUN_142d48540@142d48540 

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_142d47c10(longlong param_1)

{
  int *piVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  int iVar7;
  uint uVar8;
  undefined4 uVar9;
  void *pvVar10;
  double dVar11;
  undefined4 uVar12;
  undefined8 uVar13;
  longlong lVar14;
  longlong lVar15;
  char cVar16;
  int iVar17;
  undefined4 uVar18;
  longlong *plVar19;
  float *pfVar20;
  longlong *plVar21;
  longlong *plVar22;
  longlong *plVar23;
  longlong lVar24;
  longlong lVar25;
  longlong **pplVar26;
  undefined1 auStack_208 [32];
  double local_1e8;
  void *local_1d8;
  float local_1d0;
  void *local_1c8;
  ulonglong local_1c0;
  longlong *local_1b8 [2];
  longlong *local_1a8;
  longlong local_1a0;
  float local_198;
  void *local_190;
  int local_188;
  undefined4 local_184;
  longlong *local_180;
  longlong *local_178;
  longlong *local_170;
  longlong *local_168;
  undefined8 local_160;
  float fStack_158;
  float fStack_154;
  float fStack_150;
  float fStack_14c;
  undefined4 local_148;
  LARGE_INTEGER local_140;
  longlong *local_138 [2];
  undefined8 local_128;
  undefined4 uStack_120;
  undefined4 uStack_11c;
  longlong *local_118;
  longlong *local_110;
  undefined4 local_108;
  undefined4 uStack_104;
  undefined4 uStack_100;
  undefined4 uStack_fc;
  undefined4 local_e0;
  undefined1 local_d8 [24];
  undefined1 local_c0 [32];
  longlong local_a0 [4];
  longlong *local_80;
  undefined8 local_78;
  ulonglong local_70;
  
  local_70 = DAT_145909780 ^ (ulonglong)auStack_208;
  if (((*(int *)(param_1 + 0x600) < 1) || (*(longlong *)(param_1 + 0x5a0) == 0)) ||
     (cVar16 = FUN_1430824a0(*(longlong *)(param_1 + 0x5a0),1,0xffffffff), cVar16 == '\0')) {
    plVar23 = (longlong *)FUN_140b40c10(0x10);
    plVar22 = (longlong *)0x0;
    plVar21 = plVar22;
    if (plVar23 != (longlong *)0x0) {
      *plVar23 = 0;
      plVar23[1] = 0;
      plVar21 = plVar23;
    }
    plVar23 = (longlong *)FUN_140b40c10(0x18);
    if (plVar23 != (longlong *)0x0) {
      *(undefined4 *)(plVar23 + 1) = 1;
      *(undefined4 *)((longlong)plVar23 + 0xc) = 1;
      *plVar23 = (longlong)&PTR_FUN_143d86c10;
      plVar23[2] = (longlong)plVar21;
      plVar22 = plVar23;
    }
    local_170 = plVar21;
    local_168 = plVar22;
    FUN_1407e9150(&local_170,plVar21,plVar21);
    if ((longlong **)(param_1 + 0x688) != &local_170) {
      *(longlong **)(param_1 + 0x688) = plVar21;
      local_170 = (longlong *)0x0;
      FUN_1407fc5e0(param_1 + 0x690,&local_168);
      plVar22 = local_168;
    }
    if (plVar22 != (longlong *)0x0) {
      LOCK();
      plVar21 = plVar22 + 1;
      lVar25 = *plVar21;
      *(int *)plVar21 = (int)*plVar21 + -1;
      UNLOCK();
      if ((int)lVar25 == 1) {
        (**(code **)*plVar22)(plVar22);
        LOCK();
        piVar1 = (int *)((longlong)plVar22 + 0xc);
        iVar17 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar17 == 1) {
          (**(code **)(*plVar22 + 8))(plVar22,1);
        }
      }
    }
    *(undefined8 *)(param_1 + 0x6a8) = 0;
    *(undefined4 *)(param_1 + 0x728) = 0;
    *(undefined4 *)(param_1 + 0x620) = 0;
    if (*(int *)(param_1 + 0x624) != 0) {
      FUN_140827be0(param_1 + 0x618,0);
    }
    *(undefined4 *)(param_1 + 0x6a0) = 0;
    if (*(int *)(param_1 + 0x6a4) != 0) {
      FUN_140827be0(param_1 + 0x698,0);
    }
    local_e0 = 0;
    *(undefined8 *)(param_1 + 0x704) = 0;
    *(undefined8 *)(param_1 + 0x70c) = 0;
    *(undefined8 *)(param_1 + 0x714) = 0;
    *(undefined4 *)(param_1 + 0x71c) = 0;
    *(undefined4 *)(param_1 + 0x6f0) = 0;
    if (*(int *)(param_1 + 0x6f4) != 0) {
      FUN_140b0b9c0(param_1 + 0x6e8,0);
    }
    *(undefined8 *)(param_1 + 0x6bc) = DAT_145a8af10;
    uVar6 = DAT_145a8af18;
    *(undefined4 *)(param_1 + 0x6c4) = DAT_145a8af18;
    *(undefined8 *)(param_1 + 0x6b0) = *(undefined8 *)(param_1 + 0x6bc);
    *(undefined4 *)(param_1 + 0x6b8) = uVar6;
    *(undefined1 *)(param_1 + 0x6c8) = 0;
    return;
  }
  QueryPerformanceCounter(&local_140);
  iVar17 = *(int *)(param_1 + 0x608);
  dVar11 = (double)local_140.QuadPart * DAT_145a65c58 + DAT_14391fc98;
  while (iVar17 == 0) {
    iVar17 = rand();
    *(int *)(param_1 + 0x608) = iVar17;
  }
  uVar8 = *(uint *)(param_1 + 0x600);
  lVar25 = (longlong)(int)uVar8;
  plVar22 = (longlong *)0x0;
  local_1c8 = (void *)0x0;
  local_1c0 = 0;
  if ((int)uVar8 < 1) {
    plVar21 = plVar22;
    if ((-1 < (int)uVar8) || (plVar21 = (longlong *)0x0, uVar8 == 0)) goto LAB_142d47d15;
    local_1c0 = (ulonglong)uVar8;
    FUN_140879500(&local_1c8);
  }
  else {
    local_1c0 = (ulonglong)uVar8;
    FUN_1408273a0(&local_1c8,0);
  }
  plVar21 = (longlong *)(local_1c0 & 0xffffffff);
LAB_142d47d15:
  iVar17 = (int)plVar21;
  plVar23 = plVar22;
  if (0 < (int)uVar8) {
    do {
      lVar24 = *(longlong *)(param_1 + 0x5f8);
      plVar23 = plVar23 + 8;
      puVar2 = (undefined8 *)(lVar24 + -0x40 + (longlong)plVar23);
      uVar13 = puVar2[1];
      puVar3 = (undefined8 *)((longlong)local_1c8 + -0x40 + (longlong)plVar23);
      *puVar3 = *puVar2;
      puVar3[1] = uVar13;
      puVar2 = (undefined8 *)(lVar24 + -0x30 + (longlong)plVar23);
      uVar13 = puVar2[1];
      puVar3 = (undefined8 *)((longlong)local_1c8 + -0x30 + (longlong)plVar23);
      *puVar3 = *puVar2;
      puVar3[1] = uVar13;
      puVar4 = (undefined4 *)(lVar24 + -0x20 + (longlong)plVar23);
      uVar6 = puVar4[1];
      uVar9 = puVar4[2];
      uVar12 = puVar4[3];
      puVar5 = (undefined4 *)((longlong)local_1c8 + -0x20 + (longlong)plVar23);
      *puVar5 = *puVar4;
      puVar5[1] = uVar6;
      puVar5[2] = uVar9;
      puVar5[3] = uVar12;
      puVar2 = (undefined8 *)(lVar24 + -0x10 + (longlong)plVar23);
      uVar13 = puVar2[1];
      puVar3 = (undefined8 *)((longlong)local_1c8 + -0x10 + (longlong)plVar23);
      *puVar3 = *puVar2;
      puVar3[1] = uVar13;
      lVar25 = lVar25 + -1;
    } while (lVar25 != 0);
  }
  plVar19 = (longlong *)FUN_140b40c10(0xa0);
  plVar23 = plVar22;
  if (plVar19 != (longlong *)0x0) {
    pfVar20 = (float *)FUN_14307e7a0(*(undefined8 *)(param_1 + 0x5a0),local_c0);
    local_148 = CONCAT31(local_148._1_3_,1);
    local_190 = (void *)0x0;
    fStack_154 = pfVar20[3] + *pfVar20;
    local_198 = pfVar20[5] + pfVar20[2];
    fStack_150 = pfVar20[4] + pfVar20[1];
    local_1d0 = pfVar20[2] - pfVar20[5];
    local_160 = CONCAT44(pfVar20[1] - pfVar20[4],*pfVar20 - pfVar20[3]);
    local_188 = iVar17;
    fStack_158 = local_1d0;
    fStack_14c = local_198;
    if (iVar17 == 0) {
      local_184 = 0;
      local_1d8 = (void *)0x0;
    }
    else {
      FUN_1409674b0(&local_190,plVar21,0);
      local_1d8 = local_190;
      memcpy(local_190,local_1c8,(longlong)iVar17 << 6);
    }
    uVar12 = local_184;
    iVar17 = local_188;
    uVar9 = *(undefined4 *)(param_1 + 0x608);
    uVar6 = *(undefined4 *)(param_1 + 0x6fc);
    uVar18 = FUN_142d4b930(param_1);
    *(undefined4 *)(plVar19 + 1) = (undefined4)local_160;
    *(undefined4 *)((longlong)plVar19 + 0xc) = local_160._4_4_;
    *(float *)(plVar19 + 2) = fStack_158;
    *(float *)((longlong)plVar19 + 0x14) = fStack_154;
    *(undefined4 *)(plVar19 + 6) = uVar18;
    plVar19[3] = CONCAT44(fStack_14c,fStack_150);
    *(undefined4 *)((longlong)plVar19 + 0x3c) = uVar6;
    *(undefined4 *)(plVar19 + 4) = local_148;
    *(int *)plVar19 = iVar17;
    *(undefined4 *)(plVar19 + 7) = uVar9;
    plVar19[8] = 0;
    plVar19[9] = 0;
    plVar19[10] = 0;
    plVar19[0xb] = 0;
    plVar19[0xc] = (longlong)local_1d8;
    *(int *)(plVar19 + 0xd) = iVar17;
    *(undefined4 *)((longlong)plVar19 + 0x6c) = uVar12;
    plVar19[0xe] = 0;
    plVar19[0xf] = 0;
    plVar19[0x10] = 0;
    plVar19[0x11] = 0;
    plVar19[0x12] = 0;
    plVar19[0x13] = 0;
    plVar23 = plVar19;
  }
  local_180 = plVar23;
  plVar21 = (longlong *)FUN_140b40c10(0x18);
  if (plVar21 != (longlong *)0x0) {
    *(undefined4 *)(plVar21 + 1) = 1;
    *plVar21 = (longlong)&PTR_FUN_144c4c778;
    *(undefined4 *)((longlong)plVar21 + 0xc) = 1;
    plVar21[2] = (longlong)plVar23;
    plVar22 = plVar21;
  }
  local_178 = plVar22;
  FUN_1407e9150(&local_180,plVar23,plVar23);
  plVar22 = local_180;
  *(undefined1 *)(param_1 + 0x720) = 1;
  plVar21 = (longlong *)FUN_141df5850(0,0x30);
  local_138[0] = plVar21;
  if (plVar21 != (longlong *)0x0) {
    plVar21[1] = (longlong)plVar22;
    *plVar21 = (longlong)&PTR_FUN_14391f6e0;
    plVar21[2] = (longlong)FUN_142d47a30;
    lVar25 = FUN_141db8700();
    plVar21[4] = lVar25;
    *plVar21 = (longlong)&PTR_FUN_14391f750;
  }
  plVar22 = (longlong *)FUN_142815960(local_d8,0,2);
  lVar25 = *plVar22;
  pplVar26 = (longlong **)(lVar25 + 0x10);
  *pplVar26 = (longlong *)0x0;
  *(undefined4 *)(lVar25 + 0x18) = 0;
  if ((local_138 != pplVar26) && (plVar21 != (longlong *)0x0)) {
    (**(code **)(*plVar21 + 0x38))(plVar21,pplVar26);
  }
  *(undefined4 *)(lVar25 + 0x20) = 0x8ff;
  lVar25 = *plVar22;
  lVar14 = plVar22[2];
  lVar24 = plVar22[1];
  local_1a0 = *(longlong *)(lVar25 + 0x30);
  if (local_1a0 != 0) {
    LOCK();
    *(int *)(local_1a0 + 0x48) = *(int *)(local_1a0 + 0x48) + 1;
    UNLOCK();
    plVar21 = local_138[0];
  }
  FUN_14087af90(lVar25,lVar24,(int)lVar14,1);
  if (plVar21 != (longlong *)0x0) {
    (**(code **)(*plVar21 + 0x30))(plVar21,0);
    local_138[0] = (longlong *)FUN_141df5850(plVar21,0,0);
    if (local_138[0] != (longlong *)0x0) {
      FUN_141de9200(local_138[0]);
    }
  }
  lVar25 = param_1 + 0x748;
  iVar17 = *(int *)(param_1 + 0x770);
  *(int *)(param_1 + 0x770) = iVar17 + 1;
  if (*(int *)(param_1 + 0x774) < iVar17 + 1) {
    FUN_1407f16a0(lVar25,iVar17);
  }
  plVar22 = local_178;
  lVar24 = *(longlong *)(param_1 + 0x768);
  if (*(longlong *)(param_1 + 0x768) == 0) {
    lVar24 = lVar25;
  }
  *(longlong *)(lVar24 + (longlong)iVar17 * 8) = local_1a0;
  if (local_1a0 != 0) {
    LOCK();
    *(int *)(local_1a0 + 0x48) = *(int *)(local_1a0 + 0x48) + 1;
    UNLOCK();
  }
  local_128 = FUN_142d43b90;
  local_1a8 = local_180;
  uStack_120 = 0;
  if (local_178 != (longlong *)0x0) {
    LOCK();
    *(int *)(local_178 + 1) = (int)local_178[1] + 1;
    UNLOCK();
    if (local_178 != (longlong *)0x0) {
      LOCK();
      *(int *)(local_178 + 1) = (int)local_178[1] + 1;
      UNLOCK();
    }
  }
  plVar21 = (longlong *)FUN_141df5850(0,0x40);
  local_1b8[0] = plVar21;
  if (plVar21 != (longlong *)0x0) {
    local_108 = (undefined4)local_128;
    uStack_104 = local_128._4_4_;
    uStack_100 = uStack_120;
    uStack_fc = uStack_11c;
    local_118 = local_1a8;
    local_110 = plVar22;
    if (plVar22 != (longlong *)0x0) {
      LOCK();
      *(int *)(plVar22 + 1) = (int)plVar22[1] + 1;
      UNLOCK();
    }
    local_1e8 = dVar11;
    FUN_142d3cc60(plVar21,param_1,&local_108,&local_118);
  }
  if (plVar22 != (longlong *)0x0) {
    LOCK();
    plVar21 = plVar22 + 1;
    lVar24 = *plVar21;
    *(int *)plVar21 = (int)*plVar21 + -1;
    UNLOCK();
    if ((int)lVar24 == 1) {
      (**(code **)*plVar22)(plVar22);
      LOCK();
      piVar1 = (int *)((longlong)plVar22 + 0xc);
      iVar17 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar17 == 1) {
        (**(code **)(*plVar22 + 8))(plVar22,1);
      }
    }
    plVar21 = local_1b8[0];
    if (plVar22 != (longlong *)0x0) {
      LOCK();
      plVar23 = plVar22 + 1;
      lVar24 = *plVar23;
      *(int *)plVar23 = (int)*plVar23 + -1;
      UNLOCK();
      if ((int)lVar24 == 1) {
        (**(code **)*plVar22)(plVar22);
        LOCK();
        piVar1 = (int *)((longlong)plVar22 + 0xc);
        iVar17 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        plVar21 = local_1b8[0];
        if (iVar17 == 1) {
          (**(code **)(*plVar22 + 8))(plVar22,1);
          plVar21 = local_1b8[0];
        }
      }
    }
  }
  local_80 = (longlong *)0x0;
  local_78 = 1;
  FUN_1407f16a0(local_a0,0);
  plVar22 = local_a0;
  if (local_80 != (longlong *)0x0) {
    plVar22 = local_80;
  }
  *plVar22 = local_1a0;
  if (local_1a0 != 0) {
    LOCK();
    *(int *)(local_1a0 + 0x48) = *(int *)(local_1a0 + 0x48) + 1;
    UNLOCK();
    plVar21 = local_1b8[0];
  }
  plVar22 = (longlong *)FUN_142815960(&local_160,local_a0,2);
  lVar24 = *plVar22;
  pplVar26 = (longlong **)(lVar24 + 0x10);
  *pplVar26 = (longlong *)0x0;
  *(undefined4 *)(lVar24 + 0x18) = 0;
  if ((local_1b8 != pplVar26) && (plVar21 != (longlong *)0x0)) {
    (**(code **)(*plVar21 + 0x38))(plVar21,pplVar26);
  }
  *(undefined4 *)(lVar24 + 0x20) = 2;
  lVar24 = *plVar22;
  lVar15 = plVar22[2];
  lVar14 = plVar22[1];
  pvVar10 = *(void **)(lVar24 + 0x30);
  if (pvVar10 != (void *)0x0) {
    LOCK();
    *(int *)((longlong)pvVar10 + 0x48) = *(int *)((longlong)pvVar10 + 0x48) + 1;
    UNLOCK();
    plVar21 = local_1b8[0];
  }
  local_1d8 = pvVar10;
  FUN_14087af90(lVar24,lVar14,(int)lVar15,1);
  plVar22 = local_a0;
  iVar17 = (int)local_78;
  if (local_80 != (longlong *)0x0) {
    plVar22 = local_80;
  }
  for (; iVar17 != 0; iVar17 = iVar17 + -1) {
    if (*plVar22 != 0) {
      LOCK();
      piVar1 = (int *)(*plVar22 + 0x48);
      iVar7 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar7 == 1) {
        FUN_141dbd6f0();
      }
    }
    pvVar10 = local_1d8;
    plVar22 = plVar22 + 1;
    plVar21 = local_1b8[0];
  }
  if (local_80 != (longlong *)0x0) {
    FUN_141de9200();
  }
  if (plVar21 != (longlong *)0x0) {
    (**(code **)(*plVar21 + 0x30))(plVar21,0);
    local_1b8[0] = (longlong *)FUN_141df5850(plVar21,0,0);
    if (local_1b8[0] != (longlong *)0x0) {
      FUN_141de9200(local_1b8[0]);
    }
  }
  iVar17 = *(int *)(param_1 + 0x770);
  *(int *)(param_1 + 0x770) = iVar17 + 1;
  if (*(int *)(param_1 + 0x774) < iVar17 + 1) {
    FUN_1407f16a0(lVar25,iVar17);
  }
  lVar24 = *(longlong *)(param_1 + 0x768);
  if (*(longlong *)(param_1 + 0x768) == 0) {
    lVar24 = lVar25;
  }
  *(void **)(lVar24 + (longlong)iVar17 * 8) = pvVar10;
  if (pvVar10 != (void *)0x0) {
    LOCK();
    *(int *)((longlong)pvVar10 + 0x48) = *(int *)((longlong)pvVar10 + 0x48) + 1;
    UNLOCK();
    LOCK();
    piVar1 = (int *)((longlong)pvVar10 + 0x48);
    iVar17 = *piVar1;
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (iVar17 == 1) {
      FUN_141dbd6f0(local_1d8);
    }
  }
  if (local_1a0 != 0) {
    LOCK();
    piVar1 = (int *)(local_1a0 + 0x48);
    iVar17 = *piVar1;
    *piVar1 = *piVar1 + -1;
    UNLOCK();
    if (iVar17 == 1) {
      FUN_141dbd6f0(local_1a0);
    }
  }
  plVar22 = local_178;
  if (local_178 != (longlong *)0x0) {
    LOCK();
    plVar21 = local_178 + 1;
    lVar25 = *plVar21;
    *(int *)plVar21 = (int)*plVar21 + -1;
    UNLOCK();
    if ((int)lVar25 == 1) {
      (**(code **)*local_178)(local_178);
      LOCK();
      piVar1 = (int *)((longlong)plVar22 + 0xc);
      iVar17 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar17 == 1) {
        (**(code **)(*plVar22 + 8))(plVar22,1);
      }
    }
  }
  if (local_1c8 != (void *)0x0) {
    FUN_141de9200();
  }
  return;
}



// =================   CALLER-OF 0x142d47c10  FUN_142d52470 @ 142d52470  size=0x19e =================
// callers: <none>

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_142d52470(void)

{
  float fVar1;
  longlong lVar2;
  void *_Src;
  uint uVar3;
  uint uVar4;
  void *pvVar5;
  uint uVar6;
  float fVar7;
  void *local_48;
  ulonglong local_40;
  ulonglong local_38;
  void *local_30;
  uint local_28;
  
  fVar1 = *DAT_145c6fdd0;
  if (fVar1 != _DAT_1457567d0) {
    if (0.0 <= fVar1) {
      fVar7 = fVar1;
      if (DAT_1450d6c88 <= fVar1) {
        fVar7 = DAT_1450d6c88;
      }
    }
    else {
      fVar7 = 0.0;
    }
    _DAT_1457567d0 = fVar1;
    FUN_142d3cd60(&local_48,0x30,1,0x20000000);
    _Src = local_48;
    pvVar5 = (void *)0x0;
    uVar6 = (uint)local_40;
    local_30 = (void *)0x0;
    local_28 = uVar6;
    if (uVar6 != 0) {
      local_40._0_4_ = uVar6;
      FUN_140826f80(&local_30,local_40 & 0xffffffff,0);
      pvVar5 = local_30;
      memcpy(local_30,_Src,(longlong)(int)uVar6 << 3);
      uVar6 = (uint)local_40;
    }
    uVar3 = local_28;
    uVar4 = (uint)local_38;
joined_r0x000142d52537:
    if (uVar4 != uVar6) {
      uVar4 = (uint)local_38;
      lVar2 = *(longlong *)((longlong)pvVar5 + (longlong)(int)uVar4 * 8);
      if (((*(byte *)(lVar2 + 0x6f8) & 1) != 0) && (fVar7 != *(float *)(lVar2 + 0x6fc))) {
        *(float *)(lVar2 + 0x6fc) = fVar7;
        if ((*(longlong *)(lVar2 + 0x5a0) != 0) &&
           ((*(uint *)(*(longlong *)(lVar2 + 0x5a0) + 8) >> 10 & 1) == 0)) {
          FUN_142003fc0();
          if (*(char *)(lVar2 + 0x720) == '\0') {
            FUN_142d47c10(lVar2);
          }
          else {
            *(undefined2 *)(lVar2 + 0x721) = 0x100;
          }
        }
      }
      do {
        uVar4 = uVar4 + 1;
        local_38 = (ulonglong)uVar4;
        if ((int)uVar3 <= (int)uVar4) break;
      } while (*(longlong *)((longlong)pvVar5 + (longlong)(int)uVar4 * 8) == 0);
      goto joined_r0x000142d52537;
    }
    if (pvVar5 != (void *)0x0) {
      FUN_141de9200(pvVar5);
    }
    if (local_48 != (void *)0x0) {
      FUN_141de9200();
    }
  }
  return;
}



// =================   CALLER-OF 0x142d47c10  FUN_142d43b90 @ 142d43b90  size=0x4e1 =================
// callers: <none>

void FUN_142d43b90(longlong param_1,undefined8 param_2,undefined8 param_3,longlong *param_4)

{
  int *piVar1;
  longlong **pplVar2;
  int iVar3;
  undefined8 uVar4;
  undefined4 uVar5;
  undefined8 *puVar6;
  undefined8 uVar7;
  longlong lVar8;
  longlong *plVar9;
  undefined8 *puVar10;
  longlong *plVar11;
  int iVar12;
  longlong *plVar13;
  longlong *local_a8;
  undefined8 local_a0;
  longlong *local_98;
  longlong *local_90;
  undefined8 local_88;
  undefined4 uStack_80;
  undefined8 uStack_7c;
  undefined4 uStack_74;
  undefined1 local_70;
  undefined8 local_6c;
  undefined4 local_64;
  undefined8 local_60;
  undefined4 local_58;
  undefined1 local_54;
  undefined1 local_50 [40];
  
  plVar11 = (longlong *)0x0;
  *(undefined1 *)(param_1 + 0x720) = 0;
  iVar12 = *(int *)(param_1 + 0x770);
  plVar13 = *(longlong **)(param_1 + 0x768);
  if (*(longlong **)(param_1 + 0x768) == (longlong *)0x0) {
    plVar13 = (longlong *)(param_1 + 0x748);
  }
  for (; iVar12 != 0; iVar12 = iVar12 + -1) {
    if (*plVar13 != 0) {
      LOCK();
      piVar1 = (int *)(*plVar13 + 0x48);
      iVar3 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar3 == 1) {
        FUN_141dbd6f0();
      }
    }
    plVar13 = plVar13 + 1;
  }
  *(undefined4 *)(param_1 + 0x770) = 0;
  if (*(int *)(param_1 + 0x774) != 0) {
    FUN_141dbecc0((longlong *)(param_1 + 0x748),0);
  }
  if (*(char *)(param_1 + 0x721) == '\0') {
    if (*(char *)(param_1 + 0x722) == '\0') {
      *(undefined4 *)(param_1 + 0x6a8) = *(undefined4 *)(*(longlong *)(*param_4 + 0x90) + 0x28);
      *(undefined4 *)(param_1 + 0x6ac) = *(undefined4 *)(*(longlong *)(*param_4 + 0x90) + 0x18);
      plVar9 = (longlong *)FUN_140b40c10(0x10);
      plVar13 = plVar11;
      if (plVar9 != (longlong *)0x0) {
        plVar13 = *(longlong **)(*param_4 + 0x90);
        *plVar9 = 0;
        *plVar9 = *plVar13;
        *plVar13 = 0;
        *(int *)(plVar9 + 1) = (int)plVar13[1];
        *(undefined4 *)((longlong)plVar9 + 0xc) = *(undefined4 *)((longlong)plVar13 + 0xc);
        plVar13[1] = 0;
        plVar13 = plVar9;
      }
      plVar9 = (longlong *)FUN_140b40c10(0x18);
      if (plVar9 != (longlong *)0x0) {
        *(undefined4 *)(plVar9 + 1) = 1;
        *(undefined4 *)((longlong)plVar9 + 0xc) = 1;
        *plVar9 = (longlong)&PTR_FUN_143d86c10;
        plVar9[2] = (longlong)plVar13;
        plVar11 = plVar9;
      }
      pplVar2 = (longlong **)(param_1 + 0x688);
      local_98 = plVar13;
      local_90 = plVar11;
      FUN_1407e9150(&local_98,plVar13,plVar13);
      if (pplVar2 != &local_98) {
        *pplVar2 = plVar13;
        local_98 = (longlong *)0x0;
        FUN_1407fc5e0(param_1 + 0x690,&local_90);
        plVar11 = local_90;
      }
      if (plVar11 != (longlong *)0x0) {
        LOCK();
        plVar13 = plVar11 + 1;
        lVar8 = *plVar13;
        *(int *)plVar13 = (int)*plVar13 + -1;
        UNLOCK();
        if ((int)lVar8 == 1) {
          (**(code **)*plVar11)(plVar11);
          LOCK();
          piVar1 = (int *)((longlong)plVar11 + 0xc);
          iVar12 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar12 == 1) {
            (**(code **)(*plVar11 + 8))(plVar11,1);
          }
        }
      }
      plVar13 = (longlong *)(param_1 + 0x618);
      plVar11 = *pplVar2;
      lVar8 = *(longlong *)(*param_4 + 0x90);
      plVar9 = (longlong *)(lVar8 + 0x20);
      if (plVar13 != plVar9) {
        if (*plVar13 != 0) {
          FUN_141de9200();
        }
        *plVar13 = *plVar9;
        *plVar9 = 0;
        *(undefined4 *)(param_1 + 0x620) = *(undefined4 *)(lVar8 + 0x28);
        *(undefined4 *)(param_1 + 0x624) = *(undefined4 *)(lVar8 + 0x2c);
        *(undefined8 *)(lVar8 + 0x28) = 0;
      }
      plVar13 = (longlong *)(param_1 + 0x698);
      lVar8 = *(longlong *)(*param_4 + 0x90);
      plVar9 = (longlong *)(lVar8 + 0x10);
      if (plVar13 != plVar9) {
        if (*plVar13 != 0) {
          FUN_141de9200();
        }
        *plVar13 = *plVar9;
        *plVar9 = 0;
        *(undefined4 *)(param_1 + 0x6a0) = *(undefined4 *)(lVar8 + 0x18);
        *(undefined4 *)(param_1 + 0x6a4) = *(undefined4 *)(lVar8 + 0x1c);
        *(undefined8 *)(lVar8 + 0x18) = 0;
      }
      puVar10 = (undefined8 *)FUN_14307e7a0(*(undefined8 *)(param_1 + 0x5a0),local_50);
      uVar4 = puVar10[1];
      *(undefined8 *)(param_1 + 0x704) = *puVar10;
      *(undefined8 *)(param_1 + 0x70c) = uVar4;
      *(undefined8 *)(param_1 + 0x714) = puVar10[2];
      *(undefined4 *)(param_1 + 0x71c) = *(undefined4 *)(puVar10 + 3);
      plVar13 = *(longlong **)(*param_4 + 0x98);
      *(undefined8 *)(*param_4 + 0x98) = 0;
      *(undefined4 *)(param_1 + 0x700) = *(undefined4 *)(*(longlong *)(*param_4 + 0x90) + 0x30);
      if ((int)plVar11[1] < 1) {
        local_58 = DAT_145a8af18;
        local_64 = DAT_145a8af18;
        puVar10 = &local_6c;
        local_60 = DAT_145a8af10;
        local_6c = DAT_145a8af10;
        local_54 = 0;
      }
      else {
        puVar6 = (undefined8 *)*plVar11;
        local_70 = 1;
        uStack_80 = *(undefined4 *)(puVar6 + 1);
        local_88 = *puVar6;
        uStack_74 = *(undefined4 *)(puVar6 + 3);
        puVar10 = &local_88;
        uStack_7c = puVar6[2];
      }
      uVar7 = puVar10[1];
      uVar4 = puVar10[2];
      uVar5 = *(undefined4 *)(puVar10 + 3);
      *(undefined8 *)(param_1 + 0x6b0) = *puVar10;
      *(undefined8 *)(param_1 + 0x6b8) = uVar7;
      *(undefined8 *)(param_1 + 0x6c0) = uVar4;
      *(undefined4 *)(param_1 + 0x6c8) = uVar5;
      *(undefined8 *)(param_1 + 0x6d8) = DAT_145a8af10;
      uVar5 = DAT_145a8af18;
      *(undefined4 *)(param_1 + 0x6e0) = DAT_145a8af18;
      *(undefined8 *)(param_1 + 0x6cc) = *(undefined8 *)(param_1 + 0x6d8);
      *(undefined4 *)(param_1 + 0x6d4) = uVar5;
      *(undefined1 *)(param_1 + 0x6e4) = 0;
      *(undefined4 *)(param_1 + 0x6f0) = 0;
      if (*(int *)(param_1 + 0x6f4) != 0) {
        FUN_140b0b9c0(param_1 + 0x6e8,0);
      }
      FUN_142d523b0(param_1);
      *(undefined4 *)(param_1 + 0x728) = *(undefined4 *)(param_1 + 0x6a8);
      FUN_142d988f0(param_1 + 0x640);
      local_a8 = (longlong *)0x0;
      local_a0 = 0;
      FUN_142d7f2b0(param_1,&local_a8);
      FUN_142d6aee0(param_1,plVar13,&local_a8);
      if (*(longlong *)(param_1 + 0x630) == 0) {
        FUN_142d91100(param_1,0,plVar13,0);
      }
      else {
        FUN_142da0700(*(longlong *)(param_1 + 0x630),plVar13);
      }
      FUN_142d40330(*(undefined8 *)(param_1 + 0x630),&local_a8);
      FUN_142c196d0(param_1);
      plVar11 = local_a8;
      for (iVar12 = (int)local_a0; iVar12 != 0; iVar12 = iVar12 + -1) {
        puVar10 = (undefined8 *)*plVar11;
        if (puVar10 != (undefined8 *)0x0) {
          piVar1 = (int *)(puVar10 + 1);
          *piVar1 = *piVar1 + -1;
          if (*piVar1 == 0) {
            (**(code **)*puVar10)(puVar10,1);
          }
        }
        plVar11 = plVar11 + 1;
      }
      if (local_a8 != (longlong *)0x0) {
        FUN_141de9200(local_a8);
      }
      if (plVar13 != (longlong *)0x0) {
        puVar10 = (undefined8 *)*plVar13;
        if (puVar10 != (undefined8 *)0x0) {
          (**(code **)*puVar10)(puVar10,1);
        }
        puVar10 = (undefined8 *)plVar13[4];
        if (puVar10 != (undefined8 *)0x0) {
          (**(code **)*puVar10)(puVar10,1);
        }
        puVar10 = (undefined8 *)plVar13[2];
        if (puVar10 != (undefined8 *)0x0) {
          (**(code **)*puVar10)(puVar10,1);
        }
        thunk_FUN_141de9200(plVar13,0x38);
      }
    }
    else {
      *(undefined1 *)(param_1 + 0x722) = 0;
      FUN_142d47c10(param_1);
    }
  }
  else {
    *(undefined1 *)(param_1 + 0x721) = 0;
  }
  plVar13 = (longlong *)param_4[1];
  if (plVar13 != (longlong *)0x0) {
    LOCK();
    plVar11 = plVar13 + 1;
    lVar8 = *plVar11;
    *(int *)plVar11 = (int)*plVar11 + -1;
    UNLOCK();
    if ((int)lVar8 == 1) {
      (**(code **)*plVar13)(plVar13);
      LOCK();
      piVar1 = (int *)((longlong)plVar13 + 0xc);
      iVar12 = *piVar1;
      *piVar1 = *piVar1 + -1;
      UNLOCK();
      if (iVar12 == 1) {
        (**(code **)(*plVar13 + 8))(plVar13,1);
      }
    }
  }
  return;
}



// =================   CALLER-OF 0x142d47c10  FUN_142d48540 @ 142d48540  size=0x16b =================
// callers: FUN_142d70240@142d70240 FUN_142d65d40@142d65d40 FUN_142d65da0@142d65da0 FUN_142d42140@142d42140 FUN_142d61840@142d61840 FUN_142d62640@142d62640 FUN_142d44080@142d44080 

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_142d48540(longlong param_1,char param_2,char param_3)

{
  int iVar1;
  float *pfVar2;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  
  if ((((((param_3 == '\0') && (*(int *)(param_1 + 0x654) == 0)) &&
        (*(int *)(param_1 + 0x620) == *(int *)(param_1 + 0x600))) &&
       (*(int *)(param_1 + 0x6a8) == *(int *)(param_1 + 0x600))) &&
      ((*(longlong *)(param_1 + 0x5a0) == 0 ||
       (((((pfVar2 = (float *)FUN_14307e7a0(*(longlong *)(param_1 + 0x5a0),&local_28),
           *pfVar2 == *(float *)(param_1 + 0x704) && (pfVar2[1] == *(float *)(param_1 + 0x708))) &&
          ((pfVar2[2] == *(float *)(param_1 + 0x70c) &&
           ((pfVar2[3] == *(float *)(param_1 + 0x710) && (pfVar2[4] == *(float *)(param_1 + 0x714)))
           )))) && (pfVar2[5] == *(float *)(param_1 + 0x718))) &&
        (pfVar2[6] == *(float *)(param_1 + 0x71c))))))) &&
     ((*(int *)(param_1 + 0x6f0) < 1 && (iVar1 = FUN_14200b8a0(param_1), 0x1ba < iVar1)))) {
    local_28 = _DAT_1456debc0;
    uStack_24 = uRam00000001456debc4;
    uStack_20 = uRam00000001456debc8;
    uStack_1c = uRam00000001456debcc;
    iVar1 = FUN_14200b7e0(param_1,&local_28);
    if (0xd < iVar1) {
      return 0;
    }
  }
  if ((*(longlong *)(param_1 + 0x5a0) == 0) ||
     ((*(uint *)(*(longlong *)(param_1 + 0x5a0) + 8) >> 10 & 1) != 0)) {
    return 0;
  }
  FUN_142003fc0();
  if (param_2 == '\0') {
    FUN_142d47170(param_1);
    return 1;
  }
  if (*(char *)(param_1 + 0x720) == '\0') {
    FUN_142d47c10(param_1);
    return 1;
  }
  *(undefined2 *)(param_1 + 0x721) = 0x100;
  return 1;
}



// ================= TARGET 0x1434f2e78  FUN_1434f2db0 @ 1434f2db0  size=0x183 =================
// callers: FUN_1434f3340@1434f3340 

undefined8 FUN_1434f2db0(longlong *param_1,ushort param_2,undefined4 param_3)

{
  longlong *plVar1;
  longlong lVar2;
  char cVar3;
  short sVar4;
  undefined4 *puVar5;
  
  plVar1 = param_1 + (longlong)*(int *)((longlong)param_1 + 500) * 2 + 0x3f;
  if ((int)param_1[0x3e] <= (int)(uint)*(ushort *)((longlong)plVar1 + 6)) {
    lVar2 = *param_1;
    cVar3 = FUN_1434f7c80(lVar2 + 0x50,lVar2 + 0x2c,1,(uint)*(ushort *)((longlong)plVar1 + 6) * 0x30
                         );
    if (cVar3 == '\0') {
      return 0;
    }
    if ((longlong *)param_1[0x3d] == param_1 + 1) {
      memcpy(*(void **)(lVar2 + 0x50),param_1 + 1,0x1e0);
    }
    param_1[0x3d] = *(longlong *)(lVar2 + 0x50);
    *(undefined4 *)(param_1 + 0x3e) = *(undefined4 *)(lVar2 + 0x2c);
  }
  puVar5 = (undefined4 *)((ulonglong)*(ushort *)((longlong)plVar1 + 6) * 0x18 + param_1[0x3d]);
  *puVar5 = param_3;
  puVar5[1] = (uint)param_2;
  puVar5[4] = *(undefined4 *)((longlong)plVar1 + 0xc);
  puVar5[2] = (int)*plVar1;
  *(undefined2 *)(puVar5 + 3) = 0;
  sVar4 = *(short *)((longlong)plVar1 + 6) + 1;
  *(short *)((longlong)plVar1 + 6) = sVar4;
  return CONCAT71((uint7)(byte)((ushort)sVar4 >> 8),1);
}



// =================   CALLER-OF 0x1434f2e78  FUN_1434f3340 @ 1434f3340  size=0x6db =================
// callers: FUN_1434f5810@1434f5810 

undefined8 FUN_1434f3340(longlong *param_1,int param_2)

{
  longlong *plVar1;
  longlong lVar2;
  byte bVar3;
  char cVar4;
  int iVar5;
  ulonglong uVar6;
  byte local_58;
  ushort local_48;
  ushort local_44;
  uint local_40;
  uint local_3c;
  uint local_38;
  
  plVar1 = param_1 + (longlong)*(int *)((longlong)param_1 + 500) * 2 + 0x3f;
  lVar2 = *(longlong *)(*param_1 + 0x78);
  local_58 = *(byte *)(lVar2 + param_2);
  if (local_58 == 10) {
    local_48 = *(ushort *)(*(longlong *)(*param_1 + 0x10) + (longlong)param_2 * 2);
    local_40 = (uint)*(ushort *)((longlong)plVar1 + 6);
    do {
      local_40 = local_40 - 1;
      if ((int)local_40 < (int)(uint)*(ushort *)((longlong)plVar1 + 4)) goto LAB_1434f35f7;
    } while (*(uint *)(param_1[0x3d] + 4 + (longlong)(int)local_40 * 0x18) != (uint)local_48);
    bVar3 = FUN_1434f3a20(param_1,local_40,param_2);
    if (bVar3 != 10) {
      *(undefined1 *)((longlong)plVar1 + 10) = 10;
      *(uint *)((longlong)plVar1 + 0xc) = (uint)bVar3;
      *(int *)plVar1 = param_2;
      bVar3 = *(byte *)(*(longlong *)(*param_1 + 0x80) + (longlong)param_2);
      if ((bVar3 & 0x80) != 0) {
        bVar3 = bVar3 & 1;
        *(byte *)((longlong)plVar1 + 9) = bVar3;
        for (local_38 = (uint)*(ushort *)((longlong)plVar1 + 4); (int)local_38 < (int)local_40;
            local_38 = local_38 + 1) {
          *(ushort *)(param_1[0x3d] + 0xc + (longlong)(int)local_38 * 0x18) =
               *(ushort *)(param_1[0x3d] + 0xc + (longlong)(int)local_38 * 0x18) |
               (ushort)(1 << bVar3);
        }
        *(byte *)(*(longlong *)(*param_1 + 0x80) + (longlong)param_2) =
             *(byte *)(*(longlong *)(*param_1 + 0x80) + (longlong)param_2) & 0x7f;
      }
      *(byte *)(*(longlong *)(*param_1 + 0x80) +
               (longlong)*(int *)(param_1[0x3d] + (longlong)(int)local_40 * 0x18)) =
           *(byte *)(*(longlong *)(*param_1 + 0x80) +
                    (longlong)*(int *)(param_1[0x3d] + (longlong)(int)local_40 * 0x18)) & 0x7f;
      return 1;
    }
    local_48 = 0;
LAB_1434f35f7:
    if (local_48 == 0) {
      local_44 = 0;
    }
    else {
      local_44 = FUN_14351ebc0(local_48);
    }
    if ((local_44 != local_48) &&
       (iVar5 = FUN_14351f290(*(undefined8 *)(*param_1 + 8),local_48), iVar5 == 1)) {
      if (local_44 == 0x232a) {
        cVar4 = FUN_1434f2db0(param_1,0x3009,param_2);
        if (cVar4 == '\0') {
          return 0;
        }
      }
      else if ((local_44 == 0x3009) &&
              (cVar4 = FUN_1434f2db0(param_1,0x232a,param_2), cVar4 == '\0')) {
        return 0;
      }
      cVar4 = FUN_1434f2db0(param_1,local_44,param_2);
      if (cVar4 == '\0') {
        return 0;
      }
    }
  }
  bVar3 = *(byte *)(*(longlong *)(*param_1 + 0x80) + (longlong)param_2);
  if ((bVar3 & 0x80) == 0) {
    if ((local_58 < 2) || (local_58 == 0xd)) {
      *(byte *)((longlong)plVar1 + 10) = local_58;
      *(byte *)((longlong)plVar1 + 9) = local_58;
      *(uint *)((longlong)plVar1 + 0xc) = (uint)(local_58 != 0);
      *(int *)plVar1 = param_2;
      local_58 = local_58 != 0;
    }
    else if (local_58 == 2) {
      *(undefined1 *)((longlong)plVar1 + 10) = 2;
      if (*(char *)((longlong)plVar1 + 9) == '\0') {
        local_58 = 0;
        if ((char)param_1[0x13d] == '\0') {
          *(undefined1 *)(lVar2 + param_2) = 0x17;
        }
        *(undefined4 *)((longlong)plVar1 + 0xc) = 0;
        *(int *)plVar1 = param_2;
      }
      else {
        local_58 = 1;
        if (*(char *)((longlong)plVar1 + 9) == '\r') {
          *(undefined1 *)(lVar2 + param_2) = 5;
        }
        else {
          *(undefined1 *)(lVar2 + param_2) = 0x18;
        }
        *(undefined4 *)((longlong)plVar1 + 0xc) = 1;
        *(int *)plVar1 = param_2;
      }
    }
    else if (local_58 == 5) {
      local_58 = 1;
      *(undefined1 *)((longlong)plVar1 + 10) = 5;
      *(undefined4 *)((longlong)plVar1 + 0xc) = 1;
      *(int *)plVar1 = param_2;
    }
    else if (local_58 == 0x11) {
      local_58 = *(byte *)((longlong)plVar1 + 10);
      if (local_58 == 10) {
        *(undefined1 *)(lVar2 + param_2) = 10;
      }
    }
    else {
      *(byte *)((longlong)plVar1 + 10) = local_58;
    }
  }
  else {
    bVar3 = bVar3 & 1;
    if (((local_58 != 8) && (local_58 != 9)) && (local_58 != 10)) {
      *(byte *)(lVar2 + param_2) = bVar3;
    }
    *(byte *)((longlong)plVar1 + 10) = bVar3;
    *(byte *)((longlong)plVar1 + 9) = bVar3;
    *(uint *)((longlong)plVar1 + 0xc) = (uint)bVar3;
    *(int *)plVar1 = param_2;
    local_58 = bVar3;
  }
  if ((local_58 < 2) || (uVar6 = 0, local_58 == 0xd)) {
    for (local_3c = (uint)*(ushort *)((longlong)plVar1 + 4);
        uVar6 = (ulonglong)*(ushort *)((longlong)plVar1 + 6),
        (int)local_3c < (int)(uint)*(ushort *)((longlong)plVar1 + 6); local_3c = local_3c + 1) {
      if (*(int *)(param_1[0x3d] + (longlong)(int)local_3c * 0x18) < param_2) {
        *(ushort *)(param_1[0x3d] + 0xc + (longlong)(int)local_3c * 0x18) =
             *(ushort *)(param_1[0x3d] + 0xc + (longlong)(int)local_3c * 0x18) |
             (ushort)(1 << (local_58 != 0));
      }
    }
  }
  return CONCAT71((int7)(uVar6 >> 8),1);
}



