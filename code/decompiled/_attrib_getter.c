// ===== getter 0x140801680 (FUN_140801680) =====

undefined8 * FUN_140801680(longlong param_1,undefined8 *param_2,longlong *param_3)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined1 local_res8 [8];
  
  FUN_140800d00(param_1 + 0x80,local_res8);
  lVar1 = param_3[1];
  *param_2 = 0;
  if (((int)lVar1 != 0) && (param_3 = (longlong *)*param_3, param_3 != (longlong *)0x0)) {
    puVar2 = (undefined8 *)(**(code **)(*param_3 + 0x28))(param_3,local_res8);
    *param_2 = *puVar2;
  }
  return param_2;
}



// ===== getter 0x140b913b0 (FUN_140b913b0) =====

undefined8 FUN_140b913b0(longlong param_1)

{
  if (((*(longlong *)(param_1 + 0x170) != 0) &&
      ((*(byte *)(*(longlong *)(param_1 + 0x170) + 0x20) & 3) == 3)) &&
     (*(char *)(param_1 + 0x210) != '\0')) {
    return 1;
  }
  return 0;
}



// ===== getter 0x142c1aac0 (FUN_142c1aac0) =====

void FUN_142c1aac0(longlong param_1)

{
  *(byte *)(param_1 + 0xba) = *(byte *)(param_1 + 0xba) & 0x7f;
  return;
}



// ===== getter 0x142008b80 (FUN_142008b80) =====

void FUN_142008b80(longlong *param_1,longlong *param_2)

{
  void *_Src;
  int iVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  int iVar4;
  void *pvVar5;
  undefined4 uVar6;
  longlong local_68;
  longlong local_60;
  void *local_58;
  ulonglong local_50;
  undefined8 local_48;
  void *local_40;
  int local_38;
  undefined4 local_34;
  
  (**(code **)(*param_1 + 400))(param_1,&local_68);
  pvVar5 = (void *)0x0;
  if ((local_68 != 0) && (local_60 != 0)) {
    FUN_141eca8b0(&local_68,&local_58);
    _Src = local_58;
    local_48 = DAT_145b9cdb8;
    local_40 = (void *)0x0;
    iVar4 = (int)local_50;
    local_38 = iVar4;
    if (iVar4 == 0) {
      uVar6 = 0;
    }
    else {
      FUN_1407f1420(&local_40,local_50 & 0xffffffff,0);
      pvVar5 = local_40;
      memcpy(local_40,_Src,(longlong)iVar4 * 2);
      uVar6 = local_34;
    }
    iVar1 = local_38;
    uVar3 = local_48;
    iVar4 = (int)param_2[1];
    *(int *)(param_2 + 1) = iVar4 + 1;
    if (*(int *)((longlong)param_2 + 0xc) < iVar4 + 1) {
      FUN_1407f1620(param_2,iVar4);
    }
    puVar2 = (undefined8 *)((longlong)iVar4 * 0x20 + *param_2);
    *puVar2 = uVar3;
    puVar2[1] = pvVar5;
    *(undefined4 *)((longlong)puVar2 + 0x14) = uVar6;
    *(int *)(puVar2 + 2) = iVar1;
    puVar2[3] = 1;
    if (local_58 != (void *)0x0) {
      FUN_141de9200();
    }
    uVar3 = FUN_141eca8b0(&local_60,&local_58);
    local_48 = DAT_145b9cf28;
    FUN_1407e5680(&local_40,uVar3);
    iVar4 = (int)param_2[1];
    *(int *)(param_2 + 1) = iVar4 + 1;
    if (*(int *)((longlong)param_2 + 0xc) < iVar4 + 1) {
      FUN_1407f1620(param_2,iVar4);
    }
    puVar2 = (undefined8 *)((longlong)iVar4 * 0x20 + *param_2);
    *puVar2 = local_48;
    puVar2[1] = local_40;
    *(int *)(puVar2 + 2) = local_38;
    *(undefined4 *)((longlong)puVar2 + 0x14) = local_34;
    puVar2[3] = 1;
    if (local_58 != (void *)0x0) {
      FUN_141de9200();
    }
  }
  FUN_142008f20(param_1,param_2);
  return;
}



// ===== getter 0x1407e2f60 (_guard_check_icall) =====

void _guard_check_icall(void)

{
  return;
}



// ===== getter 0x1407e4520 (FUN_1407e4520) =====

undefined8 FUN_1407e4520(void)

{
  return 0;
}



// ===== getter 0x140cb7360 (FUN_140cb7360) =====

void FUN_140cb7360(longlong param_1)

{
  float fVar1;
  float fVar2;
  longlong lVar3;
  int iVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  undefined8 local_48;
  undefined4 local_40;
  
  lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x1a0) + 0x168);
  if (lVar3 == 0) {
    local_48 = DAT_145a8af10;
    local_40 = DAT_145a8af18;
  }
  else {
    local_48 = *(undefined8 *)(lVar3 + 0x1a0);
    local_40 = *(undefined4 *)(lVar3 + 0x1a8);
  }
  *(undefined8 *)(param_1 + 0x4bc) = local_48;
  *(undefined4 *)(param_1 + 0x4c4) = local_40;
  *(undefined8 *)(param_1 + 0x488) = 0;
  FUN_140cbe6b0(param_1);
  *(undefined8 *)(param_1 + 0x510) = 0;
  if (*(byte *)(param_1 + 0x1f8) < 2) {
    uVar5 = FUN_140d29590();
    puVar6 = (undefined8 *)FUN_140cdf3f0(uVar5,&local_48,*(undefined1 *)(param_1 + 0x1f9));
    uVar5 = puVar6[1];
    *(undefined8 *)(param_1 + 0x420) = *puVar6;
    *(undefined8 *)(param_1 + 0x428) = uVar5;
    *(undefined8 *)(param_1 + 0x430) = puVar6[2];
    fVar1 = *(float *)(param_1 + 0x424);
    fVar2 = *(float *)(param_1 + 0x428);
    iVar4 = rand();
    *(float *)(param_1 + 0x43c) =
         (float)iVar4 * DAT_14391fc5c * (fVar1 - fVar2) + fVar2 + *(float *)(param_1 + 0x420);
  }
  *(undefined4 *)(param_1 + 0x4fc) = 0;
  *(undefined4 *)(param_1 + 0x500) = 1;
  return;
}



// ===== getter 0x1428739f0 (FUN_1428739f0) =====

void FUN_1428739f0(longlong param_1,undefined4 *param_2)

{
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  longlong local_28;
  longlong local_20;
  longlong local_18;
  
  local_38 = *param_2;
  uStack_34 = param_2[1];
  uStack_30 = param_2[2];
  uStack_2c = param_2[3];
  local_28 = param_1 + 0x10;
  local_20 = param_1 + 0x38;
  local_18 = param_1 + 0x20;
  FUN_142873700(&local_28,&local_38);
  return;
}



// ===== getter 0x1407e4720 (FUN_1407e4720) =====

bool FUN_1407e4720(longlong *param_1)

{
  char cVar1;
  
  cVar1 = (**(code **)(*param_1 + 0x20))();
  return cVar1 == '\0';
}



// ===== getter 0x1407e4600 (FUN_1407e4600) =====

undefined8 * FUN_1407e4600(longlong param_1,undefined8 *param_2)

{
  *param_2 = *(undefined8 *)(param_1 + 0x20);
  return param_2;
}



// ===== getter 0x1407e7b60 (FUN_1407e7b60) =====

void FUN_1407e7b60(longlong param_1,longlong *param_2)

{
  undefined8 *puVar1;
  int iVar2;
  longlong lVar3;
  longlong *plVar4;
  
  iVar2 = (int)param_2[1];
  if (iVar2 == 0) {
    plVar4 = (longlong *)0x0;
  }
  else {
    plVar4 = (longlong *)*param_2;
  }
  if (plVar4 != (longlong *)0x0) {
    (**(code **)(*plVar4 + 0x30))(plVar4,0);
    iVar2 = (int)param_2[1];
  }
  if (iVar2 != 2) {
    lVar3 = FUN_141df5850(*param_2,0x20);
    *param_2 = lVar3;
    *(undefined4 *)(param_2 + 1) = 2;
  }
  puVar1 = (undefined8 *)*param_2;
  if (puVar1 != (undefined8 *)0x0) {
    *puVar1 = &PTR_FUN_14391fb80;
    puVar1[1] = *(undefined8 *)(param_1 + 8);
    puVar1[3] = *(undefined8 *)(param_1 + 0x18);
    *puVar1 = &PTR_FUN_14391fbd0;
  }
  return;
}



// ===== getter 0x14164cba0 (FUN_14164cba0) =====

undefined ** FUN_14164cba0(void)

{
  return &PTR_vftable_145911160;
}



// ===== getter 0x141a705e0 (FUN_141a705e0) =====

void FUN_141a705e0(longlong param_1)

{
  char cVar1;
  
  thunk_FUN_142a09170();
  if ((*(int *)(param_1 + 800) != 0) && (*(longlong **)(param_1 + 0x318) != (longlong *)0x0)) {
    cVar1 = (**(code **)(**(longlong **)(param_1 + 0x318) + 0x20))();
    if (cVar1 != '\0') {
      if (*(int *)(param_1 + 800) != 0) {
                    /* WARNING: Could not recover jumptable at 0x000141a70625. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (**(code **)(**(longlong **)(param_1 + 0x318) + 0x40))();
        return;
      }
                    /* WARNING: Could not recover jumptable at 0x000141a70633. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(lRam0000000000000000 + 0x40))();
      return;
    }
  }
  return;
}



// ===== getter 0x1418a0dd0 (FUN_1418a0dd0) =====

undefined8 FUN_1418a0dd0(undefined8 param_1,undefined1 *param_2,longlong param_3,int param_4)

{
  undefined8 *puVar1;
  void *_Src;
  longlong lVar2;
  longlong *plVar3;
  undefined1 *puVar4;
  int iVar5;
  longlong lVar6;
  longlong *plVar7;
  undefined8 *puVar8;
  
  if (param_4 != 0) {
    puVar8 = (undefined8 *)(param_3 + 8);
    do {
      puVar1 = (undefined8 *)(param_2 + 8);
      *param_2 = *(undefined1 *)(puVar8 + -1);
      if (puVar1 != puVar8) {
        iVar5 = *(int *)(puVar8 + 1);
        _Src = (void *)*puVar8;
        *(int *)(param_2 + 0x10) = iVar5;
        if ((iVar5 == 0) && (*(int *)(param_2 + 0x14) == 0)) {
          *(undefined4 *)(param_2 + 0x14) = 0;
        }
        else {
          FUN_1407f1420(puVar1,iVar5);
          memcpy((void *)*puVar1,_Src,(longlong)iVar5 * 2);
        }
      }
      plVar7 = (longlong *)(param_2 + 0x18);
      if (plVar7 != puVar8 + 2) {
        plVar3 = (longlong *)*plVar7;
        for (iVar5 = *(int *)(param_2 + 0x20); iVar5 != 0; iVar5 = iVar5 + -1) {
          if (*plVar3 != 0) {
            FUN_141de9200();
          }
          plVar3 = plVar3 + 4;
        }
        iVar5 = *(int *)(puVar8 + 3);
        lVar2 = puVar8[2];
        *(int *)(param_2 + 0x20) = iVar5;
        if ((iVar5 == 0) && (*(int *)(param_2 + 0x24) == 0)) {
          *(undefined4 *)(param_2 + 0x24) = 0;
        }
        else {
          FUN_140827100(plVar7,iVar5);
          lVar6 = *plVar7;
          if (iVar5 != 0) {
            puVar4 = (undefined1 *)(lVar2 + 0x14);
            do {
              FUN_1407e5680(lVar6,puVar4 + -0x14);
              *(undefined4 *)(lVar6 + 0x10) = *(undefined4 *)(puVar4 + -4);
              *(undefined1 *)(lVar6 + 0x14) = *puVar4;
              *(undefined1 *)(lVar6 + 0x15) = puVar4[1];
              *(undefined4 *)(lVar6 + 0x18) = *(undefined4 *)(puVar4 + 4);
              lVar6 = lVar6 + 0x20;
              iVar5 = iVar5 + -1;
              puVar4 = puVar4 + 0x20;
            } while (iVar5 != 0);
          }
        }
      }
      puVar8 = puVar8 + 5;
      param_4 = param_4 + -1;
      param_2 = param_2 + 0x28;
    } while (param_4 != 0);
  }
  return 1;
}



// ===== getter 0x1407e4710 (caseD_42) =====

undefined1 switchD_140c10c67::caseD_42(void)

{
  return 0;
}



// ===== getter 0x140f2bf00 (FUN_140f2bf00) =====

void FUN_140f2bf00(undefined8 param_1,undefined1 param_2)

{
  FUN_142b2a970();
  FUN_140f95bf0(param_1,param_2);
  return;
}



// ===== getter 0x1429b67a0 (FUN_1429b67a0) =====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_1429b67a0(undefined8 param_1,undefined8 param_2,undefined8 *param_3)

{
  undefined8 uVar1;
  undefined1 auStack_138 [32];
  undefined8 local_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 uStack_100;
  undefined4 local_f8;
  undefined4 uStack_f4;
  undefined4 uStack_f0;
  undefined4 uStack_ec;
  undefined8 local_e8;
  undefined1 local_d8 [192];
  ulonglong local_18;
  
  local_18 = DAT_145909780 ^ (ulonglong)auStack_138;
  local_118 = *param_3;
  uStack_110 = param_3[1];
  local_108 = param_3[2];
  uStack_100 = param_3[3];
  local_f8 = *(undefined4 *)(param_3 + 4);
  uStack_f4 = *(undefined4 *)((longlong)param_3 + 0x24);
  uStack_f0 = *(undefined4 *)(param_3 + 5);
  uStack_ec = *(undefined4 *)((longlong)param_3 + 0x2c);
  local_e8 = param_3[6];
  uVar1 = FUN_142a08ca0(local_f8,local_d8,&local_118);
  FUN_141537f50(param_2,uVar1);
  FUN_1407e5c80(local_d8);
  return param_2;
}



// ===== getter 0x141b5b5a0 (FUN_141b5b5a0) =====

void FUN_141b5b5a0(longlong param_1)

{
  longlong lVar1;
  undefined4 local_res8;
  undefined4 local_resc;
  
  FUN_141b5e790();
  FUN_1420575e0(&local_res8,0);
  *(undefined4 *)(param_1 + 0x104c) = local_res8;
  *(undefined4 *)(param_1 + 0x1050) = local_resc;
  lVar1 = FUN_141ae9860();
  if (lVar1 != 0) {
    FUN_141ae43f0(lVar1);
  }
  *(undefined8 *)(param_1 + 0x170) = 0;
  FUN_1420575e0(&local_res8,0);
  *(undefined4 *)(param_1 + 0x1028) = local_res8;
  *(undefined4 *)(param_1 + 0x102c) = local_resc;
  lVar1 = FUN_141ae9860();
  if (lVar1 != 0) {
    FUN_141ae4420(lVar1);
  }
  *(undefined1 *)(param_1 + 0x145) = 1;
  return;
}



// ===== getter 0x14210fd50 (FUN_14210fd50) =====

undefined8
FUN_14210fd50(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
             undefined4 param_5)

{
  longlong *plVar1;
  int *piVar2;
  longlong *plVar3;
  undefined8 local_18;
  longlong *plStack_10;
  
  local_18 = 0;
  plStack_10 = (longlong *)0x0;
  (**(code **)(*DAT_145ba0ec8 + 0x100))
            (DAT_145ba0ec8,param_2,param_3,&local_18,param_5,param_1 + 0x590);
  plVar3 = plStack_10;
  if (plStack_10 != (longlong *)0x0) {
    plVar1 = plStack_10 + 1;
    *(int *)plVar1 = (int)*plVar1 + -1;
    if ((int)*plVar1 == 0) {
      (**(code **)*plStack_10)(plStack_10);
      piVar2 = (int *)((longlong)plVar3 + 0xc);
      *piVar2 = *piVar2 + -1;
      if (*piVar2 == 0) {
        (**(code **)(*plVar3 + 8))(plVar3,1);
      }
    }
  }
  return param_2;
}



// ===== getter 0x1420134c0 (FUN_1420134c0) =====

undefined8 FUN_1420134c0(longlong *param_1,short *param_2,short *param_3,uint param_4)

{
  short sVar1;
  short sVar2;
  bool bVar3;
  uint uVar4;
  undefined8 *puVar5;
  longlong *plVar6;
  longlong lVar7;
  short *psVar8;
  short *psVar9;
  longlong lVar10;
  int iVar11;
  longlong lVar12;
  undefined2 *puVar13;
  undefined2 *puVar14;
  undefined2 *puVar15;
  short *psVar16;
  int iVar17;
  short *local_res18;
  uint local_res20;
  short *in_stack_ffffffffffffff38;
  ulonglong uVar18;
  undefined4 uVar19;
  undefined2 *in_stack_ffffffffffffff40;
  ulonglong uVar20;
  undefined4 uVar21;
  longlong *local_78;
  undefined2 *local_70;
  int local_68;
  undefined4 local_64;
  short *local_60;
  int local_58;
  longlong local_50 [3];
  
  psVar9 = &DAT_14391f7d0;
  local_res18 = param_3;
  local_res20 = param_4;
  if (param_3 != (short *)0x0) {
    lVar12 = *(longlong *)(param_1[2] + 0xc0);
    if ((*(int *)(*(longlong *)(param_3 + 8) + 0x90) < *(int *)(lVar12 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(param_3 + 8) + 0x88) + (longlong)*(int *)(lVar12 + 0x90) * 8)
        != lVar12 + 0x88)) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if ((!bVar3) && ((*(uint *)(param_1 + 1) >> 4 & 1) == 0)) {
      local_78 = *(longlong **)(lVar12 + 0x18);
      FUN_141eca8b0(&local_78,&local_70);
      puVar13 = &DAT_14391f7d0;
      if (local_68 != 0) {
        puVar13 = local_70;
      }
      puVar5 = (undefined8 *)FUN_142041d60(param_3,local_50,0);
      if (*(int *)(puVar5 + 1) == 0) {
        puVar15 = &DAT_14391f7d0;
      }
      else {
        puVar15 = (undefined2 *)*puVar5;
      }
      puVar5 = (undefined8 *)FUN_142041d60(param_1,&local_60,0);
      if (*(int *)(puVar5 + 1) == 0) {
        puVar14 = &DAT_14391f7d0;
      }
      else {
        puVar14 = (undefined2 *)*puVar5;
      }
      FUN_141e49950("Unknown",0xa9,L"Cannot rename %s into Outer %s as it is not of type %s",puVar14
                    ,puVar15,puVar13);
      if (local_60 != (short *)0x0) {
        FUN_141de9200();
      }
      if (local_50[0] != 0) {
        FUN_141de9200();
      }
      if (local_70 != (undefined2 *)0x0) {
        FUN_141de9200();
      }
      local_78 = *(longlong **)(*(longlong *)(param_1[2] + 0xc0) + 0x18);
      FUN_141eca8b0(&local_78,&local_70);
      puVar13 = &DAT_14391f7d0;
      if (local_68 != 0) {
        puVar13 = local_70;
      }
      puVar5 = (undefined8 *)FUN_142041d60(param_3,&local_60,0);
      if (*(int *)(puVar5 + 1) == 0) {
        in_stack_ffffffffffffff40 = &DAT_14391f7d0;
      }
      else {
        in_stack_ffffffffffffff40 = (undefined2 *)*puVar5;
      }
      puVar5 = (undefined8 *)FUN_142041d60(param_1,local_50,0);
      if (*(int *)(puVar5 + 1) == 0) {
        in_stack_ffffffffffffff38 = &DAT_14391f7d0;
      }
      else {
        in_stack_ffffffffffffff38 = (short *)*puVar5;
      }
      FUN_141e40cb0(&DAT_14476d652,"Unknown",0xa9,
                    L"Cannot rename %s into Outer %s as it is not of type %s",
                    in_stack_ffffffffffffff38,in_stack_ffffffffffffff40,puVar13);
      if (local_50[0] != 0) {
        FUN_141de9200();
      }
      if (local_60 != (short *)0x0) {
        FUN_141de9200();
      }
      if (local_70 != (undefined2 *)0x0) {
        FUN_141de9200();
      }
    }
  }
  lVar12 = -1;
  psVar16 = param_3;
  if ((param_4 & 0x40) != 0) {
    psVar16 = (short *)0xffffffffffffffff;
  }
  if (param_2 != (short *)0x0) {
    psVar8 = psVar16;
    if (psVar16 == (short *)0x0) {
      psVar8 = (short *)param_1[4];
    }
    plVar6 = (longlong *)FUN_14204d1d0(0,psVar8,param_2,1);
    local_78 = plVar6;
    if (plVar6 == param_1) {
      puVar13 = (undefined2 *)0x0;
      iVar17 = 0;
      local_70 = (undefined2 *)0x0;
      local_64 = 0;
      if (*param_2 != 0) {
        do {
          lVar12 = lVar12 + 1;
        } while (param_2[lVar12] != 0);
        iVar11 = (int)lVar12 + 1;
        local_68 = iVar11;
        if (0 < iVar11) {
          FUN_1407e2ee0(&local_70,0);
        }
        iVar17 = local_68;
        puVar13 = local_70;
        FUN_141dd1c10(local_70,param_2,(longlong)iVar11 * 2);
      }
      local_78 = (longlong *)local_78[3];
      FUN_141eca8b0(&local_78,&local_60);
      puVar15 = &DAT_14391f7d0;
      if (iVar17 != 0) {
        puVar15 = puVar13;
      }
      if (local_58 != 0) {
        psVar9 = local_60;
      }
      lVar12 = (longlong)puVar15 - (longlong)psVar9;
      do {
        sVar1 = *psVar9;
        sVar2 = *(short *)((longlong)psVar9 + lVar12);
        if (sVar1 != sVar2) break;
        psVar9 = psVar9 + 1;
      } while (sVar2 != 0);
      if (local_60 != (short *)0x0) {
        FUN_141de9200();
      }
      if (puVar13 != (undefined2 *)0x0) {
        FUN_141de9200(puVar13);
      }
      param_3 = local_res18;
      if (sVar1 == sVar2) {
        return 1;
      }
    }
    else if (plVar6 != (longlong *)0x0) {
      if ((local_res20 & 2) != 0) {
        return 0;
      }
      puVar5 = (undefined8 *)FUN_142041d60(plVar6,&local_60,0);
      if (*(int *)(puVar5 + 1) == 0) {
        puVar13 = &DAT_14391f7d0;
      }
      else {
        puVar13 = (undefined2 *)*puVar5;
      }
      puVar5 = (undefined8 *)FUN_142041d60(param_1,local_50,0);
      if (*(int *)(puVar5 + 1) == 0) {
        puVar15 = &DAT_14391f7d0;
      }
      else {
        puVar15 = (undefined2 *)*puVar5;
      }
      FUN_141e49950("Unknown",200,
                    L"Renaming an object (%s) on top of an existing object (%s) is not allowed",
                    puVar15,puVar13);
      if (local_50[0] != 0) {
        FUN_141de9200();
      }
      if (local_60 != (short *)0x0) {
        FUN_141de9200();
      }
      puVar5 = (undefined8 *)FUN_142041d60(plVar6,&local_60,0);
      if (*(int *)(puVar5 + 1) == 0) {
        in_stack_ffffffffffffff40 = &DAT_14391f7d0;
      }
      else {
        in_stack_ffffffffffffff40 = (undefined2 *)*puVar5;
      }
      puVar5 = (undefined8 *)FUN_142041d60(param_1,local_50,0);
      if (*(int *)(puVar5 + 1) != 0) {
        psVar9 = (short *)*puVar5;
      }
      FUN_141e40cb0(&DAT_14476d657,"Unknown",200,
                    L"Renaming an object (%s) on top of an existing object (%s) is not allowed",
                    psVar9,in_stack_ffffffffffffff40);
      in_stack_ffffffffffffff38 = psVar9;
      if (local_50[0] != 0) {
        FUN_141de9200();
        in_stack_ffffffffffffff38 = psVar9;
      }
      if (local_60 != (short *)0x0) {
        FUN_141de9200();
      }
    }
  }
  uVar4 = local_res20;
  if ((local_res20 & 2) != 0) {
    return 1;
  }
  if ((local_res20 & 1) == 0) {
    FUN_141ff65a0(param_1[4]);
  }
  uVar19 = (undefined4)((ulonglong)in_stack_ffffffffffffff38 >> 0x20);
  uVar21 = (undefined4)((ulonglong)in_stack_ffffffffffffff40 >> 0x20);
  lVar12 = param_1[3];
  if (param_2 == (short *)0x0) {
    if (param_3 != (short *)0x0) {
      uVar20 = (ulonglong)in_stack_ffffffffffffff40 & 0xffffffff00000000;
      uVar18 = (ulonglong)in_stack_ffffffffffffff38 & 0xffffffffffffff00;
      lVar7 = FUN_14205b720(0,param_3,lVar12,0,uVar18,uVar20,0);
      uVar19 = (undefined4)(uVar18 >> 0x20);
      uVar21 = (undefined4)(uVar20 >> 0x20);
      lVar10 = lVar12;
      if (lVar7 == 0) goto LAB_14201395f;
    }
    if (psVar16 == (short *)0x0) {
      psVar16 = (short *)param_1[4];
    }
    local_res18 = (short *)0x0;
    plVar6 = (longlong *)FUN_142045cb0(&local_res18,psVar16,param_1[2],0);
  }
  else {
    plVar6 = (longlong *)FUN_141eb3490(&local_res18,param_2,1);
  }
  lVar10 = *plVar6;
LAB_14201395f:
  if ((uVar4 & 0x20) == 0) {
    (**(code **)(*param_1 + 0x68))(param_1,(uVar4 & 4) == 0);
  }
  lVar7 = param_1[4];
  if (((*(byte *)(param_1 + 1) & 1) != 0) && (DAT_145b9d178 == 0)) {
    FUN_141f291f0(L"/Script/CoreUObject",L"Package",&DAT_145b9d178,_guard_check_icall,
                  CONCAT44(uVar19,0x90),CONCAT44(uVar21,0x10000000),0x400000000,L"Engine",
                  FUN_141ffd820,FUN_141ffe120,_guard_check_icall,&LAB_14083c250,&LAB_14083c250,0);
  }
  if ((param_3 != (short *)0x0) && ((local_res20 & 4) == 0)) {
    FUN_142046040(param_3);
  }
  FUN_142045c60(param_1,lVar10,param_3);
  (**(code **)(*param_1 + 0xc0))(param_1,lVar7,lVar12);
  return 1;
}



// ===== getter 0x14211e820 (FUN_14211e820) =====

void FUN_14211e820(longlong param_1,longlong param_2)

{
  FUN_142018180();
  FUN_141fd0370(param_1 + 0x28,param_2,param_1,0xffffffff);
  if ((*(byte *)(param_2 + 0x28) & 1) != 0) {
    FUN_1419e34c0(param_1 + 0x28,8);
  }
  return;
}



// ===== getter 0x1420cb1b0 (FUN_1420cb1b0) =====

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float * FUN_1420cb1b0(longlong param_1,float *param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  longlong lVar6;
  char cVar7;
  longlong *plVar8;
  float *pfVar9;
  longlong *plVar10;
  char local_res8 [8];
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  
  plVar10 = (longlong *)0x0;
  lVar6 = *(longlong *)(param_1 + 0x210);
  if (((*(int *)(lVar6 + 0xb0) != 0) && (*(longlong **)(lVar6 + 0xa8) != (longlong *)0x0)) &&
     (cVar7 = (**(code **)(**(longlong **)(lVar6 + 0xa8) + 0x20))(), cVar7 != '\0')) {
    plVar8 = plVar10;
    if (*(int *)(lVar6 + 0xb0) != 0) {
      plVar8 = *(longlong **)(lVar6 + 0xa8);
    }
    (**(code **)(*plVar8 + 0x40))(plVar8,local_res8);
    *(char *)(lVar6 + 0xa0) = local_res8[0];
  }
  if (*(char *)(lVar6 + 0xa0) == DAT_1456e8e2d) {
    *(undefined8 *)param_2 = _DAT_145a8af08;
    return param_2;
  }
  if (((*(int *)(param_1 + 0x248) != 0) && (*(longlong **)(param_1 + 0x240) != (longlong *)0x0)) &&
     (cVar7 = (**(code **)(**(longlong **)(param_1 + 0x240) + 0x20))(), cVar7 != '\0')) {
    if (*(int *)(param_1 + 0x248) != 0) {
      plVar10 = *(longlong **)(param_1 + 0x240);
    }
    (**(code **)(*plVar10 + 0x40))(plVar10,&local_38);
    *(undefined4 *)(param_1 + 0x228) = local_38;
    *(undefined4 *)(param_1 + 0x22c) = uStack_34;
    *(undefined4 *)(param_1 + 0x230) = uStack_30;
    *(undefined4 *)(param_1 + 0x234) = uStack_2c;
  }
  fVar1 = *(float *)(param_1 + 0x230);
  fVar2 = *(float *)(param_1 + 0x234);
  fVar3 = *(float *)(param_1 + 0x228);
  fVar4 = *(float *)(param_1 + 0x22c);
  plVar10 = *(longlong **)(param_1 + 0x210);
  if (DAT_145ba0eb4 == 0) {
    pfVar9 = (float *)&DAT_145a8af08;
    if (*(char *)((longlong)plVar10 + 0x24) != '\0') {
      pfVar9 = (float *)((longlong)plVar10 + 0x1c);
    }
  }
  else {
    if ((*(byte *)((longlong)plVar10 + 0x19) & 6) == 2) {
      *(byte *)((longlong)plVar10 + 0x19) = *(byte *)((longlong)plVar10 + 0x19) | 4;
      (**(code **)(*plVar10 + 0x160))(plVar10,(int)plVar10[5]);
      *(byte *)((longlong)plVar10 + 0x19) = *(byte *)((longlong)plVar10 + 0x19) & 0xfb;
    }
    pfVar9 = (float *)((longlong)plVar10 + 0x1c);
  }
  fVar5 = pfVar9[1];
  *param_2 = *pfVar9 + fVar1 + fVar3;
  param_2[1] = fVar2 + fVar4 + fVar5;
  return param_2;
}



// ===== getter 0x1408157b0 (FUN_1408157b0) =====

longlong FUN_1408157b0(longlong param_1)

{
  return param_1 + 0x18;
}



// ===== getter 0x140aca550 (FUN_140aca550) =====

void FUN_140aca550(longlong param_1)

{
  FUN_140abc3d0(param_1 + 0x28,0);
  return;
}



// ===== getter 0x1427013b0 (FUN_1427013b0) =====

undefined8 FUN_1427013b0(longlong param_1,undefined8 param_2)

{
  (**(code **)(**(longlong **)(param_1 + 0x548) + 0x30))();
  return param_2;
}



// ===== getter 0x1427831b0 (FUN_1427831b0) =====

undefined8 * FUN_1427831b0(longlong param_1,undefined8 *param_2,undefined8 *param_3)

{
  undefined4 uVar1;
  longlong lVar2;
  undefined4 uVar3;
  longlong lVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  undefined1 local_res8 [8];
  undefined8 local_res10;
  undefined8 *local_res18 [2];
  undefined8 *local_48;
  undefined8 **local_40;
  
  uVar1 = *(undefined4 *)(param_3 + 1);
  lVar2 = *(longlong *)(param_1 + 0x58);
  uVar3 = FUN_141e72540(*param_3,uVar1,0);
  local_res10 = CONCAT44(uVar3,uVar1);
  lVar4 = FUN_14277c4f0(lVar2 + 0x418,4);
  if ((lVar4 == 0) || (puVar6 = (undefined8 *)(lVar4 + -0x30), puVar6 == (undefined8 *)0x0)) {
    puVar5 = (undefined8 *)FUN_140b40c10(0x180);
    puVar6 = (undefined8 *)0x0;
    if (puVar5 != (undefined8 *)0x0) {
      *puVar5 = &PTR_FUN_14391f7a0;
      *(undefined4 *)(puVar5 + 1) = 0;
      *(undefined4 *)((longlong)puVar5 + 0xc) = 0;
      *(undefined2 *)(puVar5 + 2) = 0x100;
      puVar5[3] = 0;
      puVar5[4] = 0;
      *(undefined4 *)(puVar5 + 5) = 0;
      *puVar5 = &PTR_FUN_1447f09c0;
      FUN_14276c6a0(puVar5 + 6,lVar2,4,8);
      *puVar5 = &PTR__scalar_deleting_destructor__14490bbd0;
      puVar5[6] = &PTR_LAB_14490bbd8;
      puVar6 = puVar5;
    }
    FUN_14278a280(puVar6 + 6,param_3,local_res10);
    AcquireSRWLockExclusive((PSRWLOCK)(lVar2 + 0x418));
    local_48 = &local_res10;
    local_res18[0] = puVar6 + 6;
    if (puVar6 == (undefined8 *)0x0) {
      local_res18[0] = (undefined8 *)0x0;
    }
    local_40 = local_res18;
    FUN_1427672f0(lVar2 + 0x560,local_res8,&local_48,0);
    ReleaseSRWLockExclusive((PSRWLOCK)(lVar2 + 0x418));
  }
  *param_2 = puVar6;
  if (puVar6 != (undefined8 *)0x0) {
    LOCK();
    *(int *)(puVar6 + 1) = *(int *)(puVar6 + 1) + 1;
    UNLOCK();
  }
  return param_2;
}



// ===== getter 0x14277c480 (FUN_14277c480) =====

void FUN_14277c480(longlong param_1)

{
  FUN_14277b920(*(undefined8 *)(param_1 + 0x10),*(undefined8 *)(param_1 + 0x18),
                *(undefined8 *)(param_1 + 0x20),*(undefined4 *)(param_1 + 0x28),
                *(undefined4 *)(param_1 + 0x2c),*(undefined4 *)(param_1 + 0x30),
                *(undefined8 *)(param_1 + 0x38));
  return;
}



// ===== getter 0x14201b8c0 (FUN_14201b8c0) =====

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14201b8c0(undefined8 param_1,uint param_2)

{
  longlong *plVar1;
  uint uVar2;
  ulonglong uVar3;
  uint uVar4;
  int iVar5;
  undefined1 auStack_f8 [32];
  undefined1 local_d8;
  undefined1 local_d0;
  undefined1 local_c8;
  longlong local_b8;
  undefined8 local_b0;
  undefined1 local_a8 [32];
  undefined1 local_88 [64];
  longlong local_48;
  undefined4 local_40;
  ulonglong local_18;
  
  local_18 = DAT_145909780 ^ (ulonglong)auStack_f8;
  local_c8 = 1;
  local_d0 = 1;
  local_d8 = 1;
  local_b8 = 0;
  local_b0 = 0;
  FUN_141fd8700(local_a8,&local_b8,param_1,0);
  FUN_141fe60b0(local_a8,param_1,0,0);
  uVar3 = 0;
  while ((iVar5 = (int)uVar3, -1 < iVar5 && (iVar5 < (int)local_b0))) {
    plVar1 = *(longlong **)(local_b8 + (longlong)iVar5 * 8);
    if (plVar1 != (longlong *)0x0) {
      uVar2 = *(uint *)((longlong)plVar1 + 0xc);
      uVar3 = 0;
      if ((int)uVar2 < (int)DAT_145b9e804) {
        uVar4 = uVar2 & 0xffff;
        if ((int)uVar2 < 0) {
          uVar2 = uVar2 + 0xffff;
          uVar4 = uVar4 - 0x10000;
        }
        uVar3 = *(longlong *)(DAT_145b9e7f0 + (longlong)((int)uVar2 >> 0x10) * 8) +
                (longlong)(int)uVar4 * 0x18;
      }
      if ((*(uint *)(uVar3 + 8) >> 0x1e & 1) == 0) {
        *(uint *)(plVar1 + 1) = *(uint *)(plVar1 + 1) | param_2;
        (**(code **)(*plVar1 + 0x1a8))(plVar1,param_2);
      }
    }
    uVar3 = (ulonglong)(iVar5 + 1);
  }
  local_40 = 0;
  if (local_48 != 0) {
    FUN_141de9200();
  }
  FUN_1407fa190(local_88);
  FUN_1420362d0(local_a8);
  if (local_b8 != 0) {
    FUN_141de9200();
  }
  return;
}



// ===== getter 0x142aa6cb0 (FUN_142aa6cb0) =====

undefined8 * FUN_142aa6cb0(undefined8 *param_1,ulonglong param_2)

{
  int iVar1;
  longlong lVar2;
  
  if (param_1[0x1e] != 0) {
    FUN_141de9200();
  }
  if (param_1[0x1c] != 0) {
    FUN_141de9200();
  }
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x17));
  if (param_1[0x15] != 0) {
    FUN_141de9200();
  }
  iVar1 = *(int *)(param_1 + 0xf);
  lVar2 = param_1[0xe];
  if (iVar1 != 0) {
    lVar2 = lVar2 + 8;
    do {
      FUN_142aa2470(lVar2);
      lVar2 = lVar2 + 0x18;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    lVar2 = param_1[0xe];
  }
  if (lVar2 != 0) {
    FUN_141de9200();
  }
  *param_1 = &PTR_FUN_144b83728;
  FUN_1430769a0(param_1);
  if ((param_2 & 1) != 0) {
    thunk_FUN_141de9200(param_1,0x100);
  }
  return param_1;
}



// ===== getter 0x140b7b150 (FUN_140b7b150) =====

undefined1 * FUN_140b7b150(undefined8 param_1,undefined1 *param_2)

{
  *param_2 = 0;
  *(undefined8 *)(param_2 + 8) = 0;
  *(undefined8 *)(param_2 + 0x10) = 0;
  param_2[0x20] = 0;
  *(undefined8 *)(param_2 + 0x28) = 0;
  *(undefined8 *)(param_2 + 0x30) = 0;
  *(undefined8 *)(param_2 + 0x38) = 0;
  *(undefined8 *)(param_2 + 0x40) = 0;
  *(undefined8 *)(param_2 + 0x48) = 0;
  *(undefined8 *)(param_2 + 0x50) = 0;
  *(undefined8 *)(param_2 + 0x58) = 0;
  *(undefined8 *)(param_2 + 0x60) = 0;
  *(undefined8 *)(param_2 + 0x68) = 0;
  *(undefined8 *)(param_2 + 0x70) = 0;
  *(undefined8 *)(param_2 + 0x78) = 0;
  *(undefined8 *)(param_2 + 0x80) = 0;
  *(undefined8 *)(param_2 + 0x88) = 0;
  *(undefined8 *)(param_2 + 0x90) = 0;
  *(undefined8 *)(param_2 + 0x98) = 0;
  *(undefined8 *)(param_2 + 0xa0) = 0;
  *(undefined8 *)(param_2 + 0xa8) = 0;
  *(uint *)(param_2 + 0xb4) = *(uint *)(param_2 + 0xb4) & 0xffffff00;
  *(undefined4 *)(param_2 + 0xb0) = 0x20702;
  return param_2;
}



// ===== getter 0x142aaad80 (FUN_142aaad80) =====

void FUN_142aaad80(longlong param_1)

{
                    /* WARNING: Could not recover jumptable at 0x000142aaad8a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(longlong **)(param_1 + 0x748) + 0xe8))();
  return;
}



// ===== getter 0x141eb0bb0 (FUN_141eb0bb0) =====

void FUN_141eb0bb0(longlong param_1,char param_2)

{
  *(byte *)(param_1 + 0x28) = *(byte *)(param_1 + 0x28) & 0xfd;
  *(byte *)(param_1 + 0x28) = *(byte *)(param_1 + 0x28) | param_2 * '\x02';
  return;
}



// ===== getter 0x14102b600 (FUN_14102b600) =====

void FUN_14102b600(longlong param_1)

{
                    /* WARNING: Could not recover jumptable at 0x00014102b60a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(longlong **)(param_1 + 0x90) + 0x108))();
  return;
}



// ===== getter 0x140a50db0 (FUN_140a50db0) =====

undefined8 FUN_140a50db0(void)

{
  return 0;
}



// ===== getter 0x14203da90 (FUN_14203da90) =====

void FUN_14203da90(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined1 local_res8 [8];
  
  lVar1 = FUN_14203d510(0,param_3);
  *(uint *)(lVar1 + 0x50) = *(uint *)(lVar1 + 0x50) | 0x10;
  *(longlong *)(param_1 + 0x20) = lVar1;
  *(undefined8 *)(param_1 + 0x10) = param_2;
  puVar2 = (undefined8 *)FUN_141eb3490(local_res8,param_4,1);
  FUN_142037040(param_1,*puVar2,0);
  return;
}



// ===== getter 0x1407e4550 (FUN_1407e4550) =====

undefined1 FUN_1407e4550(void)

{
  return 1;
}



