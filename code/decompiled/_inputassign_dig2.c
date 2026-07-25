// ================= TARGET 0x1417b4e99  FUN_1417b4e80 @ 1417b4e80  size=0x34 =================
// callers: <none>

undefined8 FUN_1417b4e80(undefined8 param_1,ulonglong param_2)

{
  FUN_1417b4b40();
  if ((param_2 & 1) != 0) {
    thunk_FUN_141de9200(param_1,0x850);
  }
  return param_1;
}



// ================= TARGET 0x1417b50d7  FUN_1417b50b0 @ 1417b50b0  size=0x50 =================
// callers: <none>

undefined8 FUN_1417b50b0(undefined8 param_1,longlong param_2,void *param_3,int param_4)

{
  if (param_4 != 0) {
    param_2 = param_2 - (longlong)param_3;
    do {
      memcpy((void *)(param_2 + (longlong)param_3),param_3,0x670);
      param_3 = (void *)((longlong)param_3 + 0x670);
      param_4 = param_4 + -1;
    } while (param_4 != 0);
  }
  return 1;
}



// ================= TARGET 0x142a9bc61  FUN_142a9bad0 @ 142a9bad0  size=0x362 =================
// callers: src_simple@142a9de30 src_new@142a9db20 

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_142a9bad0(longlong param_1,int param_2)

{
  longlong lVar1;
  int iVar2;
  code *pcVar3;
  void *_Dst;
  undefined8 *puVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  undefined1 auStack_8a8 [32];
  undefined8 local_888;
  undefined8 uStack_880;
  undefined8 local_878;
  undefined8 uStack_870;
  undefined4 local_868;
  int local_864;
  int local_850;
  int local_84c;
  undefined *local_838;
  int local_824;
  ulonglong local_18;
  
  local_18 = DAT_145909780 ^ (ulonglong)auStack_8a8;
  if (*(longlong *)(param_1 + 0x20) != 0) {
    FUN_141de9200();
    *(undefined8 *)(param_1 + 0x20) = 0;
  }
  memset(&local_868,0,0x850);
  local_864 = *(int *)(param_1 + 0x14);
  iVar6 = 0x80;
  local_868 = 0x26a5050;
  if (local_864 < 0x81) {
    if (local_864 == 1) {
      pcVar3 = FUN_142a9c180;
    }
    else if (local_864 == 2) {
      pcVar3 = FUN_142a9cd10;
    }
    else if (local_864 == 4) {
      pcVar3 = FUN_142a9c960;
    }
    else {
      pcVar3 = FUN_142a9c610;
      if (local_864 == 6) {
        pcVar3 = FUN_142a9be40;
      }
    }
    *(code **)(param_1 + 0x30) = pcVar3;
    *(code **)(param_1 + 0x28) = pcVar3;
    *(code **)(param_1 + 0x38) = FUN_142a9cca0;
    if (param_2 == 0) {
      iVar6 = 0x94d;
      local_838 = &DAT_144a328b4;
      local_850 = 0x5310d;
    }
    else if (param_2 == 1) {
      iVar6 = 0x1eb;
      local_838 = &DAT_144a1ca14;
      local_850 = 0x57a4;
    }
    else {
      if (param_2 != 2) goto LAB_142a9bc9e;
      local_838 = &DAT_144a1a384;
      local_850 = 0x99e;
    }
    iVar2 = (int)((((double)local_850 * _DAT_144b7eeb0) / (double)iVar6) * DAT_1450d6cd8);
    local_824 = 0x1000;
    if (0x1000 < iVar2) {
      local_824 = iVar2;
    }
    local_824 = local_824 * local_864;
    local_84c = iVar6;
    _Dst = (void *)FUN_141df35c0((longlong)(local_824 + local_864) * 4 + 0x84c);
    if (_Dst != (void *)0x0) {
      memcpy(_Dst,&local_868,0x850);
      *(void **)(param_1 + 0x20) = _Dst;
      FUN_142a9cca0();
      uVar7 = *(uint *)((longlong)_Dst + 0x18);
      uVar5 = 0;
      if (1 < (int)uVar7) {
        do {
          uVar7 = uVar7 | 1 << (uVar5 & 0x1f);
          uVar5 = uVar5 + 1;
        } while (1 << ((byte)uVar5 & 0x1f) < (int)uVar7);
      }
      if ((int)(uVar5 + 0xb) < 0x20) {
        return 0;
      }
    }
  }
LAB_142a9bc9e:
  if (param_2 == 3) {
    if (*(longlong *)(param_1 + 0x20) != 0) {
      FUN_141de9200();
      *(undefined8 *)(param_1 + 0x20) = 0;
    }
    puVar4 = (undefined8 *)FUN_141df35c0((longlong)*(int *)(param_1 + 0x14) * 4 + 0x20);
    local_888 = 0;
    uStack_880 = 0;
    local_878 = 0;
    uStack_870 = 0;
    *puVar4 = 0;
    puVar4[1] = 0;
    puVar4[2] = 0;
    puVar4[3] = 0;
    memset(puVar4 + 4,0,(longlong)*(int *)(param_1 + 0x14) << 2);
    *(undefined8 **)(param_1 + 0x20) = puVar4;
    *(undefined4 *)puVar4 = 0x6f70a93;
    *(undefined4 *)((longlong)puVar4 + 4) = *(undefined4 *)(param_1 + 0x14);
    *(code **)(param_1 + 0x30) = FUN_142a9d230;
    *(code **)(param_1 + 0x28) = FUN_142a9d230;
  }
  else {
    if (param_2 != 4) {
      return 10;
    }
    if (*(longlong *)(param_1 + 0x20) != 0) {
      FUN_141de9200();
      *(undefined8 *)(param_1 + 0x20) = 0;
    }
    puVar4 = (undefined8 *)FUN_141df35c0((longlong)*(int *)(param_1 + 0x14) * 4 + 0x20);
    local_888 = 0;
    uStack_880 = 0;
    local_878 = 0;
    uStack_870 = 0;
    *puVar4 = 0;
    puVar4[1] = 0;
    puVar4[2] = 0;
    puVar4[3] = 0;
    memset(puVar4 + 4,0,(longlong)*(int *)(param_1 + 0x14) << 2);
    *(undefined8 **)(param_1 + 0x20) = puVar4;
    *(undefined4 *)puVar4 = 0x787c4fc;
    *(undefined4 *)((longlong)puVar4 + 4) = *(undefined4 *)(param_1 + 0x14);
    *(code **)(param_1 + 0x30) = FUN_142a9b4d0;
    *(code **)(param_1 + 0x28) = FUN_142a9b4d0;
  }
  lVar1 = *(longlong *)(param_1 + 0x20);
  *(undefined1 **)(param_1 + 0x38) = &LAB_142a9b4a0;
  if (lVar1 != 0) {
    iVar6 = *(int *)(param_1 + 0x14);
    *(int *)(lVar1 + 4) = iVar6;
    *(undefined4 *)(lVar1 + 8) = 1;
    memset((void *)(lVar1 + 0x1c),0,(longlong)iVar6 << 2);
  }
  return 0;
}



// =================   CALLER-OF 0x142a9bc61  src_simple @ 142a9de30  size=0xe4 =================
// callers: <none>

undefined4 src_simple(longlong param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  undefined8 *_Dst;
  undefined4 uVar2;
  
                    /* 0x2a9de30  125  src_simple */
  if (param_3 < 1) {
    uVar2 = 0xb;
  }
  else {
    _Dst = (undefined8 *)FUN_141df35c0(0x60);
    if (_Dst == (undefined8 *)0x0) {
      uVar2 = 1;
    }
    else {
      memset(_Dst,0,0x60);
      *(int *)((longlong)_Dst + 0x14) = param_3;
      *(undefined4 *)(_Dst + 3) = 0x22b;
      iVar1 = FUN_142a9bad0(_Dst,param_2);
      if (iVar1 == 0) {
        if ((code *)_Dst[7] != (code *)0x0) {
          (*(code *)_Dst[7])(_Dst);
        }
        _Dst[1] = 0;
        *_Dst = 0;
        _Dst[0xb] = 0;
        *(undefined4 *)(_Dst + 10) = 0;
        *(undefined4 *)(_Dst + 2) = 0;
        *(undefined4 *)(param_1 + 0x20) = 1;
        uVar2 = src_process(_Dst);
        if (_Dst[4] != 0) {
          FUN_141de9200();
        }
        memset(_Dst,0,0x60);
      }
      else {
        uVar2 = 10;
      }
      FUN_141de9200(_Dst);
    }
  }
  return uVar2;
}



// =================   CALLER-OF 0x142a9bc61  src_new @ 142a9db20  size=0xe2 =================
// callers: src_callback_new@142a9d630 

undefined8 * src_new(undefined4 param_1,int param_2,undefined4 *param_3)

{
  int iVar1;
  undefined8 *_Dst;
  
                    /* 0x2a9db20  120  src_new */
  if (param_3 != (undefined4 *)0x0) {
    *param_3 = 0;
  }
  if (param_2 < 1) {
    if (param_3 != (undefined4 *)0x0) {
      *param_3 = 0xb;
    }
    _Dst = (undefined8 *)0x0;
  }
  else {
    _Dst = (undefined8 *)FUN_141df35c0(0x60);
    if (_Dst == (undefined8 *)0x0) {
      if (param_3 != (undefined4 *)0x0) {
        *param_3 = 1;
      }
      _Dst = (undefined8 *)0x0;
    }
    else {
      memset(_Dst,0,0x60);
      *(int *)((longlong)_Dst + 0x14) = param_2;
      *(undefined4 *)(_Dst + 3) = 0x22b;
      iVar1 = FUN_142a9bad0(_Dst,param_1);
      if (iVar1 == 0) {
        if ((code *)_Dst[7] != (code *)0x0) {
          (*(code *)_Dst[7])(_Dst);
        }
        _Dst[1] = 0;
        *_Dst = 0;
        _Dst[0xb] = 0;
        *(undefined4 *)(_Dst + 10) = 0;
        *(undefined4 *)(_Dst + 2) = 0;
      }
      else {
        if (param_3 != (undefined4 *)0x0) {
          *param_3 = 10;
        }
        FUN_141de9200(_Dst);
        _Dst = (undefined8 *)0x0;
      }
    }
  }
  return _Dst;
}



// ================= TARGET 0x141a32797  FUN_141a32770 @ 141a32770  size=0x50 =================
// callers: <none>

undefined8 FUN_141a32770(undefined8 param_1,longlong param_2,void *param_3,int param_4)

{
  if (param_4 != 0) {
    param_2 = param_2 - (longlong)param_3;
    do {
      memcpy((void *)(param_2 + (longlong)param_3),param_3,0x66c);
      param_3 = (void *)((longlong)param_3 + 0x66c);
      param_4 = param_4 + -1;
    } while (param_4 != 0);
  }
  return 1;
}



// ================= TARGET 0x141680ae3  FUN_1416802b0 @ 1416802b0  size=0xfbc =================
// strings: [Option_Confirm_Save, Option_Complete_Save, AnimSong_FieldBGM_Config, AnimSong_BattleBGM_Config, Option_Progress_Save, KeyConfig_Help_2, KeyConfig_Controller, KeyConfig_Default_Controller_Confirm, KeyConfig_FieldTitle, KeyConfig_Default_Field_Confirm, KeyConfig_BattleTitle, KeyConfig_Default_Battle_Confirm, Option_Complete_Default]
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1416802b0(longlong *param_1)

{
  char cVar1;
  undefined1 uVar2;
  char cVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  longlong **pplVar7;
  longlong *plVar8;
  longlong lVar9;
  undefined8 *puVar10;
  undefined8 uVar11;
  longlong lVar12;
  longlong *plVar13;
  uint uVar14;
  ulonglong uVar15;
  wchar_t *pwVar16;
  int iVar17;
  ulonglong uVar18;
  ulonglong uVar19;
  longlong lVar20;
  longlong *plVar21;
  longlong *plVar22;
  undefined1 auStack_6e8 [32];
  undefined4 local_6c8;
  undefined4 local_6c0;
  longlong *local_6b8;
  ulonglong local_6b0;
  undefined1 local_6a8 [1644];
  undefined4 local_3c;
  ulonglong local_38;
  
  local_38 = DAT_145909780 ^ (ulonglong)auStack_6e8;
  switch((char)param_1[0x93]) {
  case '\x03':
    cVar3 = FUN_14168ff90(param_1);
    if (cVar3 == '\0') {
      return;
    }
    *(undefined1 *)(param_1 + 0x93) = 8;
    local_6b8 = (longlong *)0x0;
    local_6b0 = 0x14;
    FUN_1407e2ee0(&local_6b8,0);
    FUN_141dd1c10(local_6b8,L"Option_Confirm_Save",0x28);
    FUN_141692f90(param_1 + 0x276,&local_6b8);
    FUN_1416a1710(param_1,0);
    return;
  case '\x04':
    cVar3 = FUN_1416843b0(param_1 + 0x276);
    if (cVar3 != '\0') {
      return;
    }
    *(undefined1 *)(param_1 + 0x93) = 9;
    local_6b8 = (longlong *)0x0;
    local_6b0 = 0x15;
    FUN_1407e2ee0(&local_6b8,0);
    FUN_141dd1c10(local_6b8,L"Option_Complete_Save",0x2a);
    pplVar7 = &local_6b8;
    goto LAB_1416803cc;
  case '\x05':
    cVar3 = FUN_14168ff90(param_1);
    if (cVar3 == '\0') {
      return;
    }
    *(undefined1 *)(param_1 + 0x93) = 0xf;
    uVar2 = FUN_141479380(0);
    *(undefined1 *)(param_1 + 0x278) = uVar2;
    lVar12 = FUN_140d29250();
    if (lVar12 != 0) {
      uVar11 = FUN_140d29250();
      FUN_1412a04d0(uVar11);
    }
    lVar12 = (longlong)(int)param_1[0x91];
    uVar18 = 0;
    iVar4 = *(int *)((longlong)param_1 + 0x48c);
    uVar15 = uVar18;
    if ((-1 < iVar4) && (iVar4 < (int)param_1[lVar12 * 0xc + 0x242])) {
      uVar15 = (longlong)iVar4 * 0x60 + param_1[lVar12 * 0xc + 0x241];
    }
    if ((*(char *)(uVar15 + 0x5d) == '\0') && (*(char *)(uVar15 + 0x5e) == '\0')) {
      if ((-1 < iVar4) && (iVar4 < (int)param_1[lVar12 * 0xc + 0x242])) {
        uVar18 = (longlong)iVar4 * 0x60 + param_1[lVar12 * 0xc + 0x241];
      }
      FUN_141693090(param_1 + 0x277,uVar18);
      return;
    }
    plVar8 = (longlong *)FUN_141eb3490(&local_6b8,L"AnimSong_FieldBGM_Config",1);
    lVar12 = (longlong)(int)param_1[0x91];
    iVar4 = (int)param_1[lVar12 * 0xc + 0x242];
    if (0 < iVar4) {
      plVar13 = (longlong *)param_1[lVar12 * 0xc + 0x241];
      uVar15 = uVar18;
      uVar19 = uVar18;
      do {
        iVar6 = (int)uVar19;
        if (*plVar13 == *plVar8) goto LAB_1416804fe;
        uVar19 = (ulonglong)(iVar6 + 1);
        uVar15 = uVar15 + 1;
        plVar13 = plVar13 + 0xc;
      } while ((longlong)uVar15 < (longlong)iVar4);
    }
    iVar6 = -1;
LAB_1416804fe:
    iVar17 = *(int *)((longlong)param_1 + 0x48c);
    if (iVar6 == iVar17) {
      if ((-1 < iVar17) && (iVar17 < iVar4)) {
        uVar18 = (longlong)iVar17 * 0x60 + param_1[lVar12 * 0xc + 0x241];
      }
      FUN_141693650(param_1 + 0x277,uVar18,1);
      return;
    }
    plVar8 = (longlong *)FUN_141eb3490(&local_6b8,L"AnimSong_BattleBGM_Config",1);
    lVar12 = (longlong)(int)param_1[0x91];
    iVar4 = (int)param_1[lVar12 * 0xc + 0x242];
    if (0 < iVar4) {
      plVar13 = (longlong *)param_1[lVar12 * 0xc + 0x241];
      uVar15 = uVar18;
      uVar19 = uVar18;
      do {
        iVar6 = (int)uVar19;
        if (*plVar13 == *plVar8) goto LAB_14168059d;
        uVar19 = (ulonglong)(iVar6 + 1);
        uVar15 = uVar15 + 1;
        plVar13 = plVar13 + 0xc;
      } while ((longlong)uVar15 < (longlong)iVar4);
    }
    iVar6 = -1;
LAB_14168059d:
    iVar17 = *(int *)((longlong)param_1 + 0x48c);
    if (iVar6 != iVar17) {
      return;
    }
    if ((-1 < iVar17) && (iVar17 < iVar4)) {
      uVar18 = (longlong)iVar17 * 0x60 + param_1[lVar12 * 0xc + 0x241];
    }
    FUN_141693650(param_1 + 0x277,uVar18,2);
    return;
  case '\x06':
    cVar3 = FUN_14168ff90(param_1);
    if (cVar3 == '\0') {
      return;
    }
    if (param_1[0x8f] == 0) {
      *(undefined1 *)(param_1 + 0x93) = 1;
      return;
    }
    if (param_1[0x8d] != 0) {
      FUN_1416f34e0();
    }
    iVar4 = *(int *)((longlong)param_1 + 0x48c);
    iVar6 = 0;
    if ((-1 < iVar4) && (iVar4 < (int)param_1[(longlong)(int)param_1[0x91] * 0xc + 0x242])) {
      iVar6 = *(int *)((longlong)iVar4 * 0x60 + 0x38 +
                      param_1[(longlong)(int)param_1[0x91] * 0xc + 0x241]);
    }
    local_6b8 = (longlong *)0x0;
    local_6b0._0_4_ = 0;
    plVar8 = (longlong *)FUN_141df5850(0,0x20);
    local_6b0 = CONCAT44(local_6b0._4_4_,2);
    local_6b8 = plVar8;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = (longlong)param_1;
      *plVar8 = (longlong)&PTR_FUN_143efe2b8;
      lVar12 = FUN_141db8700();
      plVar8[3] = lVar12;
      *plVar8 = (longlong)&PTR_FUN_143efe308;
    }
    FUN_1416f2020(param_1[0x8f],&local_6b8,iVar6 != 0);
    if ((int)local_6b0 != 0) {
      if (local_6b8 == (longlong *)0x0) goto LAB_141680708;
      (**(code **)(*local_6b8 + 0x30))(local_6b8,0);
      if (local_6b8 != (longlong *)0x0) {
        local_6b8 = (longlong *)FUN_141df5850(local_6b8,0,0);
      }
      local_6b0 = local_6b0 & 0xffffffff00000000;
    }
    if (local_6b8 != (longlong *)0x0) {
      FUN_141de9200(local_6b8);
    }
LAB_141680708:
    *(undefined1 *)(param_1 + 0x93) = 0x10;
    return;
  case '\a':
    cVar3 = FUN_1416843b0(param_1 + 0x276);
    if (cVar3 != '\0') {
      return;
    }
    *(undefined1 *)(param_1 + 0x93) = 0x11;
    pplVar7 = (longlong **)FUN_1407e5680(&local_6b8,param_1 + 0x9c);
LAB_1416803cc:
    FUN_141693bc0(param_1 + 0x276,pplVar7,1);
    FUN_1416a1710(param_1,0);
    return;
  case '\b':
    cVar3 = FUN_1416843b0(param_1 + 0x276);
    if (cVar3 != '\0') {
      return;
    }
    if (*(int *)((longlong)param_1 + 0x13b4) == 0) {
      memcpy(local_6a8,param_1 + 0x16c,0x670);
      plVar8 = param_1 + 0x242;
      lVar12 = 5;
      plVar13 = param_1 + 0x23a;
      lVar9 = 5;
      plVar21 = plVar13;
      plVar22 = plVar8;
      do {
        if (0 < (int)*plVar22) {
          FUN_14168a0f0(plVar21,local_6a8);
        }
        plVar21 = plVar21 + 0xc;
        plVar22 = plVar22 + 0xc;
        lVar9 = lVar9 + -1;
      } while (lVar9 != 0);
      cVar3 = FUN_1419d0e20(local_6a8,param_1 + 0x16c,1);
      if ((cVar3 == '\0') || (cVar3 = FUN_141a6d860(), cVar3 == '\0')) {
        uVar2 = 1;
      }
      else {
        uVar2 = 0;
      }
      *(undefined1 *)((longlong)param_1 + 0x485) = uVar2;
      lVar9 = FUN_140d2ae50();
      if (lVar9 != 0) {
        FUN_1419de840(lVar9 + 0xb0,local_6a8);
        uVar11 = FUN_141a663e0();
        FUN_1419e48a0(lVar9 + 0x720,uVar11);
      }
      lVar9 = FUN_1411fc890();
      lVar9 = *(longlong *)(*(longlong *)(lVar9 + 0x1ea0) + 0x100);
      if (lVar9 != 0) {
        *(undefined4 *)(lVar9 + 0xde8d8) = local_3c;
      }
      lVar9 = FUN_140d24f50();
      if (lVar9 != 0) {
        FUN_140d6a150(*(undefined8 *)(lVar9 + 0x150));
      }
      do {
        if (0 < (int)*plVar8) {
          FUN_14168ce00(plVar13,local_6a8,1);
        }
        plVar13 = plVar13 + 0xc;
        plVar8 = plVar8 + 0xc;
        lVar12 = lVar12 + -1;
      } while (lVar12 != 0);
      FUN_1419de9c0(local_6a8);
      *(undefined4 *)((longlong)param_1 + 0x494) = 0xffffffff;
      cVar3 = FUN_141479590();
      if (cVar3 == '\0') {
        cVar3 = '\x01';
      }
      lVar12 = FUN_1411fc830();
      if (lVar12 != 0) {
        iVar4 = FUN_14145aa30(lVar12,1);
        if ((iVar4 == 0) && (cVar1 = FUN_14145b8a0(lVar12,1), cVar1 == -0x61)) {
          FUN_14148b6f0(cVar3,0);
        }
        else {
          iVar4 = FUN_14145aa30(lVar12,1);
          if ((*(int *)(lVar12 + 0x130) - 2U < 2) &&
             ((iVar4 - 2U < 2 && (*(int *)(lVar12 + 0x130) != iVar4)))) {
            *(undefined1 *)(lVar12 + 0xd2) = 0x9f;
          }
          FUN_141483be0(cVar3,(char)param_1[0x278],1);
          *(int *)(lVar12 + 0x130) = iVar4;
        }
      }
      FUN_1416a2520(param_1);
      FUN_1419e92a0(local_6a8);
      FUN_141a57b50();
      plVar8 = DAT_14569c340;
      do {
        if (plVar8 == DAT_14569c340 + (longlong)DAT_14569c348 * 2) {
LAB_141680983:
          uVar2 = FUN_1419dcef0(local_6a8);
          uVar11 = FUN_140d6de90();
          FUN_14124d850(uVar11,uVar2);
          memcpy(param_1 + 0x16c,local_6a8,0x670);
          *(undefined1 *)(param_1 + 0x93) = 0x13;
          local_6b8 = (longlong *)0x0;
          local_6b0 = 0x15;
          FUN_1407e2ee0(&local_6b8,0);
          FUN_141dd1c10(local_6b8,L"Option_Progress_Save",0x2a);
          FUN_141693bc0(param_1 + 0x276,&local_6b8,0);
          lVar12 = FUN_1411fd700();
          if (lVar12 == 0) {
            return;
          }
          FUN_141593540(lVar12,0);
          if (*(char *)((longlong)param_1 + 0x483) != '\0') {
            local_6b8 = (longlong *)0x0;
            local_6b0 = 0x11;
            FUN_1407e2ee0(&local_6b8);
            FUN_141dd1c10(local_6b8,L"KeyConfig_Help_2",0x22);
            FUN_1415a17a0(lVar12,&local_6b8);
            FUN_14159fd20(lVar12,0);
            return;
          }
          FUN_14159fd00(lVar12,0);
          return;
        }
        if (((int)plVar8[1] != 0) && (*plVar8 != 0)) {
          FUN_1407e7240(&DAT_14569c340);
          goto LAB_141680983;
        }
        plVar8 = plVar8 + 2;
      } while( true );
    }
    break;
  case '\t':
  case '\x0e':
  case '\x11':
    cVar3 = FUN_1416843b0(param_1 + 0x276);
    if (cVar3 != '\0') {
      return;
    }
    break;
  case '\n':
    cVar3 = FUN_1416843b0(param_1 + 0x276);
    if (cVar3 != '\0') {
      return;
    }
    if (*(int *)((longlong)param_1 + 0x13b4) == 0) {
      FUN_1419de9c0(param_1 + 0x16c);
      lVar12 = FUN_140d2ae50();
      if (lVar12 != 0) {
        memcpy((void *)(lVar12 + 0xb0),param_1 + 0x16c,0x66c);
      }
      FUN_141a8e3b0();
      FUN_1416965c0(param_1);
      return;
    }
    break;
  case '\v':
    cVar3 = FUN_1416843b0(param_1 + 0x276);
    if (cVar3 != '\0') {
      return;
    }
    if (*(int *)((longlong)param_1 + 0x13b4) == 0) {
      plVar8 = param_1 + 0x242;
      lVar12 = 5;
      plVar13 = param_1 + 0x23a;
      do {
        if (0 < (int)*plVar8) {
          FUN_141697490(plVar13);
          FUN_141696d40(plVar13);
        }
        plVar13 = plVar13 + 0xc;
        plVar8 = plVar8 + 0xc;
        lVar12 = lVar12 + -1;
      } while (lVar12 != 0);
      FUN_141a8e3b0();
      uVar5 = FUN_14168c000(param_1);
      *(undefined4 *)(param_1 + 0x91) = uVar5;
      *(undefined4 *)((longlong)param_1 + 0x48c) = 0;
      FUN_1416a2bc0(param_1,0);
      lVar12 = param_1[0x88];
LAB_141680b90:
      local_6c0 = DAT_1450d6c88;
      local_6c8 = 0;
      (**(code **)(*param_1 + 0x2e0))(param_1,lVar12,0,1);
      *(undefined1 *)((longlong)param_1 + 0x481) = 1;
      *(undefined1 *)(param_1 + 0x93) = 1;
      FUN_1416a1710(param_1,1);
      return;
    }
    break;
  case '\f':
    cVar3 = FUN_1416843b0(param_1 + 0x276);
    if (cVar3 != '\0') {
      return;
    }
    if (*(int *)((longlong)param_1 + 0x13b4) == 0) {
      plVar8 = param_1 + 0x242;
      lVar12 = 5;
      plVar13 = param_1 + 0x23a;
      do {
        if (0 < (int)*plVar8) {
          FUN_141697490(plVar13);
          FUN_141696d40(plVar13);
        }
        plVar13 = plVar13 + 0xc;
        plVar8 = plVar8 + 0xc;
        lVar12 = lVar12 + -1;
      } while (lVar12 != 0);
      FUN_141a8e3b0();
      uVar5 = FUN_14168b8a0(param_1);
      *(undefined4 *)(param_1 + 0x91) = uVar5;
      *(undefined4 *)((longlong)param_1 + 0x48c) = 0;
      FUN_1416a2bc0(param_1,0);
      lVar12 = param_1[0x86];
      goto LAB_141680b90;
    }
    break;
  case '\r':
    cVar3 = FUN_1416843b0(param_1 + 0x276);
    if (cVar3 != '\0') {
      return;
    }
    if (*(int *)((longlong)param_1 + 0x13b4) == 0) {
      lVar12 = (longlong)(int)param_1[0x91];
      plVar8 = (longlong *)FUN_141eb3490(&local_6b8,L"KeyConfig_Controller",1);
      if (param_1[lVar12 * 0xc + 0x23a] == *plVar8) {
        lVar9 = FUN_140d2ae50();
        if (lVar9 != 0) {
          FUN_1419e00e0(lVar9 + 0x720,1);
        }
        *(undefined1 *)(param_1 + lVar12 * 0xc + 0x245) = 1;
        local_6b8 = (longlong *)0x0;
        local_6b0 = 0x25;
        FUN_1407e2ee0(&local_6b8,0);
        uVar11 = 0x4a;
        pwVar16 = L"KeyConfig_Default_Controller_Confirm";
      }
      else {
        plVar8 = (longlong *)FUN_141eb3490(&local_6b8,L"KeyConfig_FieldTitle",1);
        if (param_1[lVar12 * 0xc + 0x23a] == *plVar8) {
          lVar9 = FUN_140d2ae50();
          if (lVar9 != 0) {
            FUN_1419e0920(lVar9 + 0x720,1);
          }
          *(undefined1 *)(param_1 + lVar12 * 0xc + 0x245) = 1;
          local_6b8 = (longlong *)0x0;
          local_6b0 = 0x20;
          FUN_1407e2ee0(&local_6b8,0);
          uVar11 = 0x40;
          pwVar16 = L"KeyConfig_Default_Field_Confirm";
        }
        else {
          plVar8 = (longlong *)FUN_141eb3490(&local_6b8,L"KeyConfig_BattleTitle",1);
          if (param_1[lVar12 * 0xc + 0x23a] != *plVar8) {
            FUN_1416a2bc0(param_1,1);
            lVar9 = param_1[0xa4];
            cVar3 = FUN_1416edb60();
            plVar8 = param_1 + 0x242;
            plVar13 = param_1 + 0x23a;
            uVar14 = 2;
            if (cVar3 != '\0') {
              uVar14 = (uint)((int)lVar9 != 0);
            }
            lVar9 = 5;
            *(uint *)(param_1 + 0xa4) = uVar14;
            do {
              if (0 < (int)*plVar8) {
                FUN_14168ce00(plVar13,param_1 + 0x9e,0);
                FUN_141696d40(plVar13);
              }
              plVar13 = plVar13 + 0xc;
              plVar8 = plVar8 + 0xc;
              lVar9 = lVar9 + -1;
            } while (lVar9 != 0);
            local_6b8 = (longlong *)0x0;
            local_6b0 = 0x18;
            FUN_1407e2ee0(&local_6b8,0);
            FUN_141dd1c10(local_6b8,L"Option_Complete_Default",0x30);
            FUN_141693bc0(param_1 + 0x276,&local_6b8,1);
            cVar3 = FUN_1419d0e20(param_1 + 0x9e,param_1 + 0x16c,1);
            if ((cVar3 == '\0') ||
               (*(int *)((longlong)param_1 + 0xb5c) != *(int *)((longlong)param_1 + 0x11cc))) {
              *(undefined1 *)(param_1 + lVar12 * 0xc + 0x245) = 1;
            }
            FUN_1416a2520(param_1);
            *(undefined1 *)(param_1 + 0x93) = 0xe;
            FUN_1416a1710(param_1,0);
            return;
          }
          lVar9 = FUN_140d2ae50();
          if (lVar9 != 0) {
            FUN_1419df290(lVar9 + 0x720,1);
          }
          *(undefined1 *)(param_1 + lVar12 * 0xc + 0x245) = 1;
          local_6b8 = (longlong *)0x0;
          local_6b0 = 0x21;
          FUN_1407e2ee0(&local_6b8,0);
          uVar11 = 0x42;
          pwVar16 = L"KeyConfig_Default_Battle_Confirm";
        }
      }
      FUN_141dd1c10(local_6b8,pwVar16,uVar11);
      FUN_141693bc0(param_1 + 0x276,&local_6b8,1);
      FUN_1416a2520(param_1);
      *(undefined1 *)(param_1 + 0x93) = 0xe;
      FUN_1416a1710(param_1,0);
      return;
    }
    break;
  case '\x0f':
    if (((char)param_1[0x277] != '\0') && (lVar12 = FUN_1411fd780(), lVar12 != 0)) {
      cVar3 = FUN_1416ed9d0(lVar12);
      if (cVar3 == '\0') {
        return;
      }
      cVar3 = FUN_1416ed940(lVar12);
      if (cVar3 == '\0') {
        uVar5 = FUN_1416eb690(lVar12);
      }
      else {
        uVar5 = 0xffffffff;
      }
      *(undefined4 *)((longlong)param_1 + 0x13bc) = uVar5;
      lVar12 = FUN_1411fc830();
      if (lVar12 != 0) {
        FUN_141464cb0(lVar12,0);
      }
      *(undefined1 *)(param_1 + 0x277) = 0;
      lVar12 = FUN_1411fd700();
      if (lVar12 != 0) {
        FUN_141593540(lVar12,0);
        FUN_14159fd00(lVar12,0);
      }
    }
    iVar4 = *(int *)((longlong)param_1 + 0x13bc);
    puVar10 = (undefined8 *)FUN_141eb3490(&local_6b8,L"AnimSong_FieldBGM_Config",1);
    plVar8 = param_1 + (longlong)(int)param_1[0x91] * 0xc + 0x23a;
    iVar6 = FUN_14168b770(plVar8,*puVar10);
    if (iVar6 == *(int *)((longlong)param_1 + 0x48c)) {
LAB_14168101f:
      if (*(char *)((longlong)param_1 + 0x482) == '\0') {
        uVar15 = (ulonglong)*(byte *)(param_1 + 0x278);
      }
      else {
        uVar15 = (ulonglong)plVar8 & 0xffffffffffffff00;
      }
      FUN_141483fd0(uVar15,0);
      lVar12 = FUN_140d29250();
      if (lVar12 != 0) {
        uVar11 = FUN_140d29250();
        FUN_1412a04d0(uVar11);
      }
      FUN_14168b300(param_1);
      lVar12 = FUN_1411fc830();
      if (lVar12 != 0) {
        cVar3 = FUN_141462a80(lVar12);
        iVar17 = (cVar3 != '\0') + 2;
        iVar6 = 1;
        if ((iVar4 <= iVar17) && (iVar6 = iVar4, iVar17 == iVar4)) {
          iVar6 = 4;
        }
        FUN_141466920(lVar12,*(undefined4 *)(lVar12 + 0xd8),iVar6);
        FUN_140984da0(lVar12,0);
      }
    }
    else {
      puVar10 = (undefined8 *)FUN_141eb3490(&local_6b8,L"AnimSong_BattleBGM_Config",1);
      plVar8 = param_1 + (longlong)(int)param_1[0x91] * 0xc + 0x23a;
      iVar6 = FUN_14168b770(plVar8,*puVar10);
      if (iVar6 == *(int *)((longlong)param_1 + 0x48c)) goto LAB_14168101f;
    }
    iVar6 = (int)param_1[0x91];
    iVar17 = *(int *)((longlong)param_1 + 0x48c);
    lVar12 = (longlong)iVar17;
    plVar8 = param_1 + (longlong)iVar6 * 0xc + 0x23a;
    if (iVar4 < 0) {
      if ((-1 < iVar17) && (iVar17 < (int)plVar8[8])) {
        lVar9 = plVar8[7];
        lVar20 = lVar12 * 0x60;
        *(undefined4 *)(lVar9 + 0x38 + lVar20) = *(undefined4 *)(lVar9 + 0x34 + lVar20);
        *(undefined1 *)(plVar8[7] + 0x52 + lVar20) = 0;
        FUN_141696aa0(plVar8,lVar12,CONCAT71((int7)((ulonglong)lVar9 >> 8),1));
        iVar6 = (int)param_1[0x91];
      }
      FUN_141696f60(param_1 + (longlong)iVar6 * 0xc + 0x23a,param_1[0x7f],0xffffffff);
      if (param_1[0x7f] != 0) {
        FUN_1415c5350();
      }
    }
    else {
      FUN_14169ec00(plVar8,lVar12,iVar4,0);
      iVar4 = (int)param_1[0x91];
      iVar6 = *(int *)((longlong)param_1 + 0x48c);
      if ((-1 < iVar6) && (iVar6 < (int)param_1[(longlong)iVar4 * 0xc + 0x242])) {
        *(undefined1 *)((longlong)iVar6 * 0x60 + 0x52 + param_1[(longlong)iVar4 * 0xc + 0x241]) = 0;
        iVar4 = (int)param_1[0x91];
      }
      FUN_141696f60(param_1 + (longlong)iVar4 * 0xc + 0x23a,param_1[0x7f],0xffffffff);
      if (param_1[0x7f] != 0) {
        FUN_1415c54b0();
        *(undefined1 *)(param_1 + 0x93) = 1;
        FUN_1416a1710(param_1,1);
        return;
      }
    }
    break;
  case '\x10':
    goto switchD_141680311_caseD_10;
  case '\x12':
    cVar3 = FUN_1416843b0(param_1 + 0x276);
    if (cVar3 != '\0') {
      return;
    }
    *(undefined1 *)(param_1 + 0x93) = 2;
    FUN_1416a1710(param_1,1);
    *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0xf7;
    *(undefined1 *)((longlong)param_1 + 0x483) = 0;
    FUN_141681690(param_1);
    return;
  case '\x13':
    if (((*(char *)((longlong)param_1 + 0x13b1) != '\0') && (lVar12 = FUN_1411fd780(), lVar12 != 0))
       && (cVar3 = FUN_1416ed8f0(lVar12), cVar3 != '\0')) {
      return;
    }
  default:
    *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0xf7;
    goto switchD_141680311_caseD_10;
  }
  *(undefined1 *)(param_1 + 0x93) = 1;
  FUN_1416a1710(param_1,1);
switchD_141680311_caseD_10:
  return;
}



// ================= TARGET 0x14169f072  FUN_14169efd0 @ 14169efd0  size=0x411 =================
// strings: [Option_Category_Control, Option_Category_Graphic, KeyConfig_Controller, KeyConfig_FieldTitle, KeyConfig_BattleTitle]
// callers: FUN_1416951a0@1416951a0 

void FUN_14169efd0(longlong param_1)

{
  undefined1 uVar1;
  longlong *plVar2;
  bool bVar3;
  uint uVar4;
  undefined8 *puVar5;
  longlong lVar6;
  longlong lVar7;
  uint uVar8;
  undefined1 local_res8 [8];
  
  puVar5 = (undefined8 *)(param_1 + 0x1200);
  *(undefined8 *)(param_1 + 0x488) = 0;
  lVar7 = param_1 + 0x1208;
  lVar6 = 5;
  do {
    FUN_141688ed0(lVar7,0);
    lVar7 = lVar7 + 0x60;
    *puVar5 = *(undefined8 *)(param_1 + 0x458);
    puVar5 = puVar5 + 0xc;
    lVar6 = lVar6 + -1;
  } while (lVar6 != 0);
  FUN_1419e34e0(param_1 + 0x4f0);
  *(undefined4 *)(param_1 + 0xb5c) = 1;
  memcpy((void *)(param_1 + 0xb60),(void *)(param_1 + 0x4f0),0x670);
  lVar7 = FUN_140d2ae50();
  if (lVar7 != 0) {
    memcpy((void *)(param_1 + 0xb60),(void *)(lVar7 + 0xb0),0x66c);
  }
  lVar7 = FUN_1411fc890();
  lVar7 = *(longlong *)(*(longlong *)(lVar7 + 0x1ea0) + 0x100);
  if (lVar7 != 0) {
    *(undefined4 *)(param_1 + 0x11cc) = *(undefined4 *)(lVar7 + 0xde8d8);
  }
  uVar1 = *(undefined1 *)(param_1 + 0x482);
  puVar5 = (undefined8 *)FUN_141eb3490(local_res8,L"Option_Category_Control",1);
  FUN_141685240(param_1 + 0x11d0,*(undefined8 *)(param_1 + 0x448),*puVar5,uVar1);
  uVar1 = *(undefined1 *)(param_1 + 0x482);
  puVar5 = (undefined8 *)FUN_141eb3490(local_res8,L"Option_Category_Graphic",1);
  FUN_141685240(param_1 + 0x1230,*(undefined8 *)(param_1 + 0x448),*puVar5,uVar1);
  uVar1 = *(undefined1 *)(param_1 + 0x482);
  puVar5 = (undefined8 *)FUN_141eb3490(local_res8,L"KeyConfig_Controller",1);
  FUN_141685240(param_1 + 0x1290,*(undefined8 *)(param_1 + 0x450),*puVar5,uVar1);
  uVar1 = *(undefined1 *)(param_1 + 0x482);
  puVar5 = (undefined8 *)FUN_141eb3490(local_res8,L"KeyConfig_FieldTitle",1);
  FUN_141685240(param_1 + 0x12f0,*(undefined8 *)(param_1 + 0x448),*puVar5,uVar1);
  uVar1 = *(undefined1 *)(param_1 + 0x482);
  puVar5 = (undefined8 *)FUN_141eb3490(local_res8,L"KeyConfig_BattleTitle",1);
  FUN_141685240(param_1 + 0x1350,*(undefined8 *)(param_1 + 0x448),*puVar5,uVar1);
  FUN_14169ed70(param_1 + 0x11d0,*(undefined8 *)(param_1 + 0x460));
  FUN_14169ed70(param_1 + 0x1230,*(undefined8 *)(param_1 + 0x460));
  FUN_14169ed70(param_1 + 0x1290,*(undefined8 *)(param_1 + 0x460));
  FUN_14169ed70(param_1 + 0x12f0,*(undefined8 *)(param_1 + 0x460));
  FUN_14169ed70(param_1 + 0x1350,*(undefined8 *)(param_1 + 0x460));
  FUN_14168ce00(param_1 + 0x11d0,param_1 + 0xb60,1);
  FUN_14168ce00(param_1 + 0x1230,param_1 + 0xb60,1);
  *(undefined4 *)(param_1 + 0x490) = 0;
  bVar3 = *(int *)(param_1 + 0x1210) < 1;
  if (!bVar3) {
    *(undefined4 *)(param_1 + 0x488) = 0;
    *(undefined4 *)(param_1 + 0x490) = 1;
  }
  uVar4 = (uint)!bVar3;
  uVar8 = (uint)!bVar3;
  if (0 < *(int *)(param_1 + 0x1270)) {
    if (bVar3) {
      *(undefined4 *)(param_1 + 0x488) = 1;
      uVar8 = *(uint *)(param_1 + 0x490);
    }
    bVar3 = false;
    uVar4 = uVar8 + 1;
    *(uint *)(param_1 + 0x490) = uVar4;
  }
  if (0 < *(int *)(param_1 + 0x12d0)) {
    if (bVar3) {
      *(undefined4 *)(param_1 + 0x488) = 2;
      bVar3 = false;
      uVar4 = *(uint *)(param_1 + 0x490);
    }
    uVar4 = uVar4 + 1;
    *(uint *)(param_1 + 0x490) = uVar4;
  }
  if (0 < *(int *)(param_1 + 0x1330)) {
    if (bVar3) {
      *(undefined4 *)(param_1 + 0x488) = 3;
      bVar3 = false;
      uVar4 = *(uint *)(param_1 + 0x490);
    }
    uVar4 = uVar4 + 1;
    *(uint *)(param_1 + 0x490) = uVar4;
  }
  if (0 < *(int *)(param_1 + 0x1390)) {
    if (bVar3) {
      *(undefined4 *)(param_1 + 0x488) = 4;
      uVar4 = *(uint *)(param_1 + 0x490);
    }
    uVar4 = uVar4 + 1;
    *(uint *)(param_1 + 0x490) = uVar4;
  }
  plVar2 = *(longlong **)(param_1 + 0x3c0);
  if ((int)uVar4 < 2) {
    if (plVar2 != (longlong *)0x0) {
      (**(code **)(*plVar2 + 0x250))(plVar2,3);
    }
    plVar2 = *(longlong **)(param_1 + 0x3d0);
    if (plVar2 != (longlong *)0x0) {
      (**(code **)(*plVar2 + 0x250))(plVar2,1);
    }
    plVar2 = *(longlong **)(param_1 + 0x3c8);
    if (plVar2 != (longlong *)0x0) {
      (**(code **)(*plVar2 + 0x250))(plVar2,1);
    }
  }
  else {
    if (plVar2 != (longlong *)0x0) {
      (**(code **)(*plVar2 + 0x250))(plVar2,1);
    }
    plVar2 = *(longlong **)(param_1 + 0x3d0);
    if (plVar2 != (longlong *)0x0) {
      (**(code **)(*plVar2 + 0x250))(plVar2,3);
    }
    plVar2 = *(longlong **)(param_1 + 0x3c8);
    if (plVar2 != (longlong *)0x0) {
      (**(code **)(*plVar2 + 0x250))(plVar2,4);
    }
    if (*(longlong *)(param_1 + 0x408) != 0) {
      FUN_1416f37c0();
    }
    if (*(longlong *)(param_1 + 0x410) != 0) {
      FUN_1416f37c0();
    }
  }
  return;
}



// =================   CALLER-OF 0x14169f072  FUN_1416951a0 @ 1416951a0  size=0x1b0 =================
// callers: FUN_1412a63f0@1412a63f0 

void FUN_1416951a0(longlong *param_1,undefined1 param_2)

{
  longlong lVar1;
  char cVar2;
  undefined1 uVar3;
  longlong lVar4;
  longlong lVar5;
  undefined8 uVar6;
  
  if (param_1[0x89] != 0) {
    *(undefined1 *)((longlong)param_1 + 0x482) = param_2;
    *(undefined4 *)((longlong)param_1 + 0x494) = 0xffffffff;
    if (((char)param_1[0x90] == '\0') &&
       (((param_1[0x7f] == 0 || (cVar2 = FUN_1415c1770(), cVar2 == '\0')) &&
        (*(char *)((longlong)param_1 + 0x481) == '\0')))) {
      *(undefined1 *)(param_1 + 0x93) = 1;
      if ((param_1[0x8d] == 0) && (lVar4 = FUN_140d2ba50(), lVar4 != 0)) {
        lVar4 = FUN_1415458f0(lVar4);
        param_1[0x8d] = lVar4;
      }
      if (param_1[0x8d] != 0) {
        FUN_1416f7bd0(param_1[0x8d],3,8,0);
      }
      if (param_1[0x7f] != 0) {
        FUN_1415bd310();
      }
      FUN_14169efd0(param_1);
      uVar3 = FUN_141479380(0);
      *(undefined1 *)(param_1 + 0x278) = uVar3;
      FUN_1416a2520(param_1);
      FUN_141a88520(param_1);
      (**(code **)(*param_1 + 0x468))(param_1,1);
      (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x83],0,1,0,DAT_1450d6c88);
      *(undefined2 *)(param_1 + 0x90) = 0x101;
      _guard_check_icall(8);
      if ((*(char *)((longlong)param_1 + 0x482) == '\x01') && (param_1[0x8f] == 0)) {
        if (param_1[0x8e] != 0) {
          lVar5 = FUN_1417bd0d0();
          lVar4 = param_1[0x8e];
          for (lVar1 = lVar4; lVar1 != 0; lVar1 = *(longlong *)(lVar1 + 0x30)) {
            if (lVar1 == lVar5) goto LAB_141695310;
          }
        }
        lVar4 = 0;
LAB_141695310:
        uVar6 = (**(code **)(*param_1 + 0x148))(param_1);
        lVar4 = FUN_140d18890(uVar6,lVar4);
        param_1[0x8f] = lVar4;
        if (lVar4 != 0) {
          FUN_141a58910(lVar4,0xa0);
          return;
        }
      }
    }
  }
  return;
}



// ================= TARGET 0x141d0c430  FUN_141d0c1f0 @ 141d0c1f0  size=0x105a =================
// strings: [Option_Confirm_Save, Option_Complete_Save, ErrorMsgId, Tab_Left, Tab_Right, V_TCG, Option_Progress_Save, KeyConfig_Help_2, KeyConfig_Title, Option_Complete_Default]
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_141d0c1f0(longlong *param_1)

{
  uint uVar1;
  bool bVar2;
  undefined1 uVar3;
  char cVar4;
  undefined4 uVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  undefined8 *puVar9;
  longlong lVar10;
  ulonglong uVar11;
  undefined8 uVar12;
  wchar_t *pwVar13;
  longlong lVar14;
  longlong *plVar15;
  ulonglong uVar16;
  ulonglong uVar17;
  uint uVar18;
  uint *puVar19;
  undefined1 *puVar20;
  uint uVar21;
  longlong *plVar22;
  undefined1 auStack_7e8 [32];
  uint local_7c8;
  undefined4 local_7c0;
  longlong *local_7b8;
  undefined8 local_7b0;
  char local_7a8;
  undefined1 local_7a4 [4];
  longlong *local_7a0;
  longlong *local_798;
  undefined8 uStack_790;
  undefined8 uStack_788;
  undefined8 uStack_780;
  longlong lStack_778;
  undefined8 local_770;
  longlong local_768;
  undefined8 local_760;
  longlong *local_758;
  undefined8 uStack_750;
  uint *local_748;
  undefined8 uStack_740;
  longlong local_738;
  undefined8 local_730;
  longlong local_728;
  undefined8 local_720;
  uint local_718;
  uint uStack_714;
  uint *puStack_710;
  undefined4 local_708;
  int iStack_704;
  uint uStack_700;
  undefined4 uStack_6fc;
  longlong local_6f8;
  undefined8 local_6f0;
  undefined8 local_6d8;
  undefined8 local_6d0;
  undefined4 local_6c8;
  int local_6c4;
  undefined1 local_6c0 [8];
  undefined1 *local_6b8;
  int local_6b0;
  undefined1 local_6a8 [24];
  int local_690;
  ulonglong local_38;
  
  local_38 = DAT_145909780 ^ (ulonglong)auStack_7e8;
  switch(*(undefined1 *)((longlong)param_1 + 0x494)) {
  case 3:
    cVar4 = FUN_141d13240(param_1);
    if (cVar4 != '\0') {
      *(undefined1 *)((longlong)param_1 + 0x494) = 7;
      local_7b8 = (longlong *)0x0;
      local_7b0 = 0x14;
      FUN_1407e2ee0(&local_7b8,0);
      FUN_141dd1c10(local_7b8,L"Option_Confirm_Save",0x28);
      FUN_141d1afd0(param_1 + 0x259,&local_7b8);
      FUN_141d28ad0(param_1,0);
      return;
    }
    return;
  case 4:
    cVar4 = FUN_141d100a0(param_1 + 0x259);
    if (cVar4 != '\0') {
      return;
    }
    *(undefined1 *)((longlong)param_1 + 0x494) = 8;
    local_7b8 = (longlong *)0x0;
    local_7b0 = 0x15;
    FUN_1407e2ee0(&local_7b8,0);
    uVar5 = 0x2a;
    pwVar13 = L"Option_Complete_Save";
    goto LAB_141d0c2fd;
  case 5:
    cVar4 = FUN_141d13240(param_1);
    if (cVar4 != '\0') {
      *(undefined1 *)((longlong)param_1 + 0x494) = 0xe;
      uVar12 = FUN_1411fc890();
      uVar3 = FUN_14120f0b0(uVar12);
      *(undefined1 *)((longlong)param_1 + 0x12e1) = uVar3;
      uVar12 = FUN_1411fc890();
      uVar3 = FUN_14120f070(uVar12);
      *(undefined1 *)((longlong)param_1 + 0x12e2) = uVar3;
      uVar12 = FUN_1411fc890();
      FUN_1412169a0(uVar12,1);
      uVar12 = FUN_1411fc890();
      FUN_141216990(uVar12,1);
      uVar12 = FUN_14168b880(param_1 + (longlong)*(int *)((longlong)param_1 + 0x484) * 0xc + 0x235,
                             (int)param_1[0x91]);
      FUN_141693090((longlong)param_1 + 0x12d4,uVar12);
      return;
    }
    return;
  case 6:
    cVar4 = FUN_141d100a0(param_1 + 0x259);
    if (cVar4 != '\0') {
      return;
    }
    *(undefined1 *)((longlong)param_1 + 0x494) = 0xf;
    local_7b8 = (longlong *)0x0;
    local_7b0 = 0xb;
    FUN_1407e2ee0(&local_7b8,0);
    uVar5 = 0x16;
    pwVar13 = L"ErrorMsgId";
LAB_141d0c2fd:
    FUN_141dd1c10(local_7b8,pwVar13,uVar5);
    FUN_141d1bfa0(param_1 + 0x259,&local_7b8,1);
    FUN_141d28ad0(param_1,0);
    return;
  case 7:
    cVar4 = FUN_141d100a0(param_1 + 0x259);
    if (cVar4 != '\0') {
      return;
    }
    cVar4 = FUN_14168c070(param_1 + 0x259);
    if (cVar4 != '\0') {
      memcpy(local_6a8,(void *)((longlong)param_1 + 0xb34),0x66c);
      FUN_141b671a0(local_7a4);
      FUN_141b6d1d0(local_7a4,(longlong)param_1 + 0x11a3);
      plVar15 = param_1 + 0x235;
      lVar10 = 3;
      lVar14 = 3;
      plVar22 = plVar15;
      do {
        cVar4 = FUN_1416900f0(plVar22);
        if (cVar4 != '\0') {
          FUN_141d11bc0(param_1,plVar22,local_6a8,local_7a4);
        }
        plVar22 = plVar22 + 0xc;
        lVar14 = lVar14 + -1;
      } while (lVar14 != 0);
      lVar14 = FUN_1411fc890();
      if (lVar14 != 0) {
        lVar14 = FUN_1411fc890();
        if ((*(longlong *)(lVar14 + 0x1ea0) != 0) &&
           (lVar14 = *(longlong *)(*(longlong *)(lVar14 + 0x1ea0) + 0x108), lVar14 != 0)) {
          memcpy((void *)(lVar14 + 0xb0),local_6a8,0x66c);
        }
      }
      lVar14 = FUN_141aea7a0();
      if (lVar14 != 0) {
        if (lVar14 + 1000 != 0) {
          FUN_141b6d1d0(lVar14 + 1000,local_7a4);
        }
        if (lVar14 + 0x3f0 != 0) {
          uVar12 = FUN_141a663e0();
          FUN_141b7cc30(lVar14 + 0x3f0,uVar12);
        }
      }
      do {
        cVar4 = FUN_1416900f0(plVar15);
        if (cVar4 != '\0') {
          local_7c8 = CONCAT31(local_7c8._1_3_,1);
          FUN_141d120c0(param_1,plVar15,local_6a8,local_7a4);
        }
        plVar15 = plVar15 + 0xc;
        lVar10 = lVar10 + -1;
      } while (lVar10 != 0);
      FUN_1419de9c0(local_6a8);
      *(undefined4 *)(param_1 + 0x92) = 0xffffffff;
      FUN_141d29690(param_1);
      uVar16 = 0;
      if ((param_1[0x85] != 0) && (param_1[0x86] != 0)) {
        local_798 = (longlong *)0x0;
        uStack_790 = 0;
        FUN_141eb3490(&uStack_788,&DAT_14391f7d0,1);
        FUN_141eb3490(&uStack_780,&DAT_14391f7d0,1);
        lStack_778 = 0;
        local_770 = 0;
        local_768 = 0;
        local_760 = 0;
        puVar9 = (undefined8 *)FUN_141eb3490(&local_7b8,L"Tab_Left",1);
        uVar3 = FUN_141ceee50(*puVar9);
        uVar11 = local_760;
        lVar10 = (longlong)(int)local_760;
        iVar8 = (int)local_760 + 1;
        local_760 = CONCAT44(local_760._4_4_,iVar8);
        if (local_760._4_4_ < iVar8) {
          FUN_140827170(&local_768,uVar11 & 0xffffffff);
        }
        uVar11 = uStack_790;
        *(undefined1 *)(lVar10 + local_768) = uVar3;
        lVar10 = (longlong)(int)uStack_790;
        iVar8 = (int)uStack_790 + 1;
        uStack_790 = CONCAT44(uStack_790._4_4_,iVar8);
        if (uStack_790._4_4_ < iVar8) {
          FUN_140827170(&local_798,uVar11 & 0xffffffff);
        }
        *(undefined1 *)(lVar10 + (longlong)local_798) = 7;
        FUN_1416fb0e0(param_1[0x85],&local_798);
        local_758 = (longlong *)0x0;
        uStack_750 = 0;
        FUN_141eb3490(&local_748,&DAT_14391f7d0,1);
        FUN_141eb3490(&uStack_740,&DAT_14391f7d0,1);
        local_738 = 0;
        local_730 = 0;
        local_728 = 0;
        local_720 = 0;
        puVar9 = (undefined8 *)FUN_141eb3490(&local_7b8,L"Tab_Right",1);
        uVar3 = FUN_141ceee50(*puVar9);
        uVar11 = local_720;
        lVar10 = (longlong)(int)local_720;
        iVar8 = (int)local_720 + 1;
        local_720 = CONCAT44(local_720._4_4_,iVar8);
        if (local_720._4_4_ < iVar8) {
          FUN_140827170(&local_728,uVar11 & 0xffffffff);
        }
        uVar11 = uStack_750;
        *(undefined1 *)(lVar10 + local_728) = uVar3;
        lVar10 = (longlong)(int)uStack_750;
        iVar8 = (int)uStack_750 + 1;
        uStack_750 = CONCAT44(uStack_750._4_4_,iVar8);
        if ((int)uStack_750._4_4_ < iVar8) {
          FUN_140827170(&local_758,uVar11 & 0xffffffff);
        }
        *(undefined1 *)(lVar10 + (longlong)local_758) = 4;
        FUN_1416fb0e0(param_1[0x86],&local_758);
        if (local_728 != 0) {
          FUN_141de9200();
        }
        if (local_738 != 0) {
          FUN_141de9200();
        }
        if (local_758 != (longlong *)0x0) {
          FUN_141de9200();
        }
        if (local_768 != 0) {
          FUN_141de9200();
        }
        if (lStack_778 != 0) {
          FUN_141de9200();
        }
        if (local_798 != (longlong *)0x0) {
          FUN_141de9200();
        }
      }
      if ((*(int *)((longlong)param_1 + 0xb4c) != local_690) &&
         (lVar10 = FUN_141ae8590(), lVar10 != 0)) {
        local_7b8 = (longlong *)0x0;
        local_7b0 = 6;
        FUN_1407e2ee0(&local_7b8,0);
        FUN_141dd1c10(local_7b8,L"V_TCG",0xc);
        FUN_141b08210(lVar10,&local_7b8);
        FUN_141aee5f0(lVar10,1);
      }
      FUN_1419e92a0(local_6a8);
      FUN_141a57b50();
      memcpy((void *)((longlong)param_1 + 0xb34),local_6a8,0x66c);
      FUN_141b6d1d0((longlong)param_1 + 0x11a3,local_7a4);
      plVar15 = DAT_14569c340;
      while( true ) {
        if (plVar15 == DAT_14569c340 + (longlong)DAT_14569c348 * 2) goto LAB_141d0c91b;
        if (((int)plVar15[1] != 0) && (*plVar15 != 0)) break;
        plVar15 = plVar15 + 2;
      }
      DAT_14569c354 = DAT_14569c354 + 1;
      lVar10 = (longlong)(DAT_14569c348 + -1);
      bVar2 = false;
      if (-1 < DAT_14569c348 + -1) {
        lVar14 = lVar10 << 4;
        bVar2 = false;
        do {
          if (((*(int *)(lVar14 + 8 + (longlong)DAT_14569c340) == 0) ||
              (*(longlong **)(lVar14 + (longlong)DAT_14569c340) == (longlong *)0x0)) ||
             (cVar4 = (**(code **)(**(longlong **)(lVar14 + (longlong)DAT_14569c340) + 0x48))(),
             cVar4 == '\0')) {
            bVar2 = true;
          }
          lVar14 = lVar14 + -0x10;
          lVar10 = lVar10 + -1;
        } while (-1 < lVar10);
      }
      iVar8 = DAT_14569c348;
      DAT_14569c354 = DAT_14569c354 + -1;
      if ((bVar2) && (DAT_14569c354 < 1)) {
        uVar11 = uVar16;
        if (0 < DAT_14569c348) {
          do {
            if (((*(int *)(uVar16 + 8 + (longlong)DAT_14569c340) == 0) ||
                (*(longlong **)(uVar16 + (longlong)DAT_14569c340) == (longlong *)0x0)) ||
               (cVar4 = (**(code **)(**(longlong **)(uVar16 + (longlong)DAT_14569c340) + 0x18))(),
               cVar4 != '\0')) {
              FUN_1407f1040(&DAT_14569c340,uVar11,1,1);
            }
            else {
              uVar11 = (ulonglong)((int)uVar11 + 1);
              uVar16 = uVar16 + 0x10;
            }
          } while ((int)uVar11 < DAT_14569c348);
        }
        _DAT_14569c350 = DAT_14569c348 * 2;
        if (_DAT_14569c350 < 3) {
          _DAT_14569c350 = 2;
        }
        if ((_DAT_14569c350 < iVar8) && (DAT_14569c34c != DAT_14569c348)) {
          FUN_1407f1a80(&DAT_14569c340);
        }
      }
LAB_141d0c91b:
      *(undefined1 *)((longlong)param_1 + 0x494) = 0x11;
      local_7b8 = (longlong *)0x0;
      local_7b0 = 0x15;
      FUN_1407e2ee0(&local_7b8,0);
      FUN_141dd1c10(local_7b8,L"Option_Progress_Save",0x2a);
      FUN_141d1bfa0(param_1 + 0x259,&local_7b8,0);
      FUN_141ce91e0();
      if (*(char *)((longlong)param_1 + 0x482) == '\0') {
        FUN_141ce9a00();
        return;
      }
      local_7b8 = (longlong *)0x0;
      local_7b0 = 0x11;
      FUN_1407e2ee0(&local_7b8,0);
      FUN_141dd1c10(local_7b8,L"KeyConfig_Help_2",0x22);
      FUN_141d00df0(&local_7b8);
      if (local_7b8 != (longlong *)0x0) {
        FUN_141de9200();
      }
      FUN_141cfcc90();
      return;
    }
    break;
  case 8:
  case 0xd:
  case 0xf:
    cVar4 = FUN_141d100a0(param_1 + 0x259);
    if (cVar4 != '\0') {
      return;
    }
    break;
  case 9:
    cVar4 = FUN_141d100a0(param_1 + 0x259);
    if (cVar4 != '\0') {
      return;
    }
    cVar4 = FUN_14168c070(param_1 + 0x259);
    if (cVar4 != '\0') {
      FUN_1419de9c0((longlong)param_1 + 0xb34);
      FUN_141a8e3b0();
      FUN_141d1c800(param_1);
      return;
    }
    break;
  case 10:
    cVar4 = FUN_141d100a0(param_1 + 0x259);
    if (cVar4 != '\0') {
      return;
    }
    cVar4 = FUN_14168c070(param_1 + 0x259);
    if (cVar4 != '\0') {
      plVar15 = param_1 + 0x235;
      lVar10 = 3;
      do {
        cVar4 = FUN_1416900f0(plVar15);
        if (cVar4 != '\0') {
          FUN_141697490(plVar15);
          FUN_141696d40(plVar15);
        }
        plVar15 = plVar15 + 0xc;
        lVar10 = lVar10 + -1;
      } while (lVar10 != 0);
      FUN_141a8e3b0();
      uVar5 = FUN_141d12010(param_1);
      *(undefined4 *)((longlong)param_1 + 0x484) = uVar5;
      *(undefined4 *)(param_1 + 0x91) = 0;
      FUN_141d29950(param_1,0);
      lVar10 = param_1[0x8c];
LAB_141d0cac2:
      local_7c0 = DAT_1450d6c88;
      local_7c8 = 0;
      (**(code **)(*param_1 + 0x2e0))(param_1,lVar10,0,1);
      *(undefined1 *)((longlong)param_1 + 0x481) = 1;
      *(undefined1 *)((longlong)param_1 + 0x494) = 1;
      FUN_141d28ad0(param_1,1);
      return;
    }
    break;
  case 0xb:
    cVar4 = FUN_141d100a0(param_1 + 0x259);
    if (cVar4 != '\0') {
      return;
    }
    cVar4 = FUN_14168c070(param_1 + 0x259);
    if (cVar4 != '\0') {
      plVar15 = param_1 + 0x235;
      lVar10 = 3;
      do {
        cVar4 = FUN_1416900f0(plVar15);
        if (cVar4 != '\0') {
          FUN_141697490(plVar15);
          FUN_141696d40(plVar15);
        }
        plVar15 = plVar15 + 0xc;
        lVar10 = lVar10 + -1;
      } while (lVar10 != 0);
      FUN_141a8e3b0();
      uVar5 = FUN_141d11f90(param_1);
      *(undefined4 *)((longlong)param_1 + 0x484) = uVar5;
      *(undefined4 *)(param_1 + 0x91) = 0;
      FUN_141d29950(param_1,0);
      lVar10 = param_1[0x8a];
      goto LAB_141d0cac2;
    }
    break;
  case 0xc:
    cVar4 = FUN_141d100a0(param_1 + 0x259);
    if (cVar4 != '\0') {
      return;
    }
    cVar4 = FUN_14168c070(param_1 + 0x259);
    if (cVar4 == '\0') break;
    plVar15 = param_1 + (longlong)*(int *)((longlong)param_1 + 0x484) * 0xc + 0x235;
    local_7b8 = plVar15;
    puVar9 = (undefined8 *)FUN_141eb3490(&local_7a0,L"KeyConfig_Title",1);
    cVar4 = FUN_14168f1e0(plVar15,*puVar9);
    uVar16 = 0;
    if (cVar4 == '\0') {
      FUN_141d29950(param_1,1);
      plVar22 = param_1 + 0x235;
      lVar10 = 3;
      do {
        cVar4 = FUN_1416900f0(plVar22);
        if (cVar4 != '\0') {
          local_7c8 = local_7c8 & 0xffffff00;
          FUN_141d120c0(param_1,plVar22,param_1 + 0x99,param_1 + 0x234);
          FUN_141696d40(plVar22);
        }
        plVar22 = plVar22 + 0xc;
        lVar10 = lVar10 + -1;
      } while (lVar10 != 0);
      cVar4 = FUN_1419d0e20(param_1 + 0x99,(longlong)param_1 + 0xb34,1);
      if (cVar4 == '\0') {
        *(undefined1 *)(plVar15 + 0xb) = 1;
      }
      goto LAB_141d0d022;
    }
    lVar10 = FUN_141aea7a0();
    if ((lVar10 != 0) && (lVar10 + 0x3f0 != 0)) {
      local_7a0 = (longlong *)FUN_141a663d0();
      local_6f8 = 0;
      local_6f0 = 0;
      local_6d8 = 0;
      local_6d0 = 0;
      local_6c8 = 0xffffffff;
      local_6c4 = 0;
      local_6b8 = (undefined1 *)0x0;
      local_6b0 = 0;
      FUN_141b74800(lVar10 + 0x3f0,&local_6f8);
      local_748 = (uint *)(local_7a0 + 2);
      iVar8 = (int)local_7a0[5];
      local_7a8 = (int)local_7a0[1] - *(int *)((longlong)local_7a0 + 0x34) !=
                  (int)local_6f0 - local_6c4;
      uStack_714 = 1;
      local_718 = 0;
      iStack_704 = 0;
      uStack_700 = 0;
      if (iVar8 != 0) {
        puVar19 = (uint *)local_7a0[4];
        if ((uint *)local_7a0[4] == (uint *)0x0) {
          puVar19 = local_748;
        }
        uVar7 = *puVar19;
        uVar11 = uVar16;
        uVar17 = uVar16;
        while (uVar7 == 0) {
          iVar6 = (int)uVar11;
          uStack_700 = (int)uVar17 + 0x20;
          uVar17 = (ulonglong)uStack_700;
          local_718 = iVar6 + 1;
          uVar11 = (ulonglong)local_718;
          iStack_704 = iVar8;
          if ((int)(iVar8 + -1 + (iVar8 + -1 >> 0x1f & 0x1fU)) >> 5 <= iVar6) goto LAB_141d0cd35;
          uVar7 = puVar19[(longlong)iVar6 + 1];
        }
        uStack_714 = -uVar7 & uVar7;
        iVar6 = 0x1f;
        if (uStack_714 != 0) {
          for (; uStack_714 >> iVar6 == 0; iVar6 = iVar6 + -1) {
          }
        }
        if (uStack_714 == 0) {
          iVar6 = 0x20;
        }
        else {
          iVar6 = 0x1f - iVar6;
        }
        iStack_704 = (uStack_700 - iVar6) + 0x1f;
        if (iVar8 < iStack_704) {
          iStack_704 = iVar8;
        }
      }
LAB_141d0cd35:
      local_708 = 0xffffffff;
      uVar7 = *(uint *)(local_7a0 + 5);
      uStack_790 = CONCAT44(uStack_714,local_718);
      uVar1 = *(uint *)(local_7a0 + 5);
      uStack_780 = CONCAT44(iStack_704,0xffffffff);
      lStack_778 = CONCAT44(uStack_6fc,uStack_700);
      local_798 = local_7a0;
      uVar21 = -1 << ((byte)uVar7 & 0x1f);
      uVar18 = uVar7 & 0xffffffe0;
      uStack_788 = CONCAT44(uVar7,uVar21);
      local_758 = local_7a0;
      uStack_750 = CONCAT44(uStack_714,local_718);
      uStack_740 = uStack_780;
      local_738 = lStack_778;
      puStack_710 = local_748;
      uVar12 = uStack_788;
      if (uVar7 != uVar1) {
        plVar15 = (longlong *)local_7a0[4];
        if ((longlong *)local_7a0[4] == (longlong *)0x0) {
          plVar15 = local_7a0 + 2;
        }
        uVar21 = *(uint *)((longlong)plVar15 + (longlong)((int)uVar7 >> 5) * 4) & uVar21;
        iVar8 = (int)uVar7 >> 5;
        while (uVar21 == 0) {
          uVar18 = uVar18 + 0x20;
          if ((int)((uVar1 - 1) + ((int)(uVar1 - 1) >> 0x1f & 0x1fU)) >> 5 <= iVar8) {
            uStack_788 = CONCAT44(uVar1,(undefined4)uStack_788);
            uVar12 = uStack_788;
            goto LAB_141d0ce30;
          }
          uVar21 = *(uint *)((longlong)plVar15 + (longlong)iVar8 * 4 + 4);
          uStack_788 = 0xffffffff;
          iVar8 = iVar8 + 1;
        }
        uVar21 = -uVar21 & uVar21;
        iVar8 = 0x1f;
        if (uVar21 != 0) {
          for (; uVar21 >> iVar8 == 0; iVar8 = iVar8 + -1) {
          }
        }
        if (uVar21 == 0) {
          iVar8 = 0x20;
        }
        else {
          iVar8 = 0x1f - iVar8;
        }
        uVar7 = (uVar18 - iVar8) + 0x1f;
        if ((int)uVar1 < (int)uVar7) {
          uVar7 = uVar1;
        }
        uStack_788 = CONCAT44(uVar7,(undefined4)uStack_788);
        uVar12 = uStack_788;
      }
LAB_141d0ce30:
      do {
        if (((uStack_740._4_4_ == (int)((ulonglong)uVar12 >> 0x20)) && (local_748 == puStack_710))
           && (cVar4 = local_7a8, local_758 == local_7a0)) goto LAB_141d0cf2b;
        lVar10 = *(longlong *)(*local_758 + (longlong)uStack_740._4_4_ * 0x18);
        if ((int)local_6f0 == local_6c4) goto LAB_141d0cea8;
        puVar20 = local_6c0;
        if (local_6b8 != (undefined1 *)0x0) {
          puVar20 = local_6b8;
        }
        iVar6 = (int)((ulonglong)lVar10 >> 0x20);
        iVar8 = *(int *)(puVar20 + (longlong)(int)(iVar6 + (int)lVar10 & local_6b0 - 1U) * 4);
        while( true ) {
          if (iVar8 == -1) goto LAB_141d0cea8;
          if (*(longlong *)(local_6f8 + (longlong)iVar8 * 0x18) == lVar10) break;
          iVar8 = *(int *)(local_6f8 + 0x10 + (longlong)iVar8 * 0x18);
        }
        if (iVar8 == -1) goto LAB_141d0cea8;
        iVar8 = *(int *)(puVar20 + (longlong)(int)(iVar6 + (int)lVar10 & local_6b0 - 1U) * 4);
        while (iVar8 != -1) {
          lVar14 = (longlong)iVar8;
          if (*(longlong *)(local_6f8 + lVar14 * 0x18) == lVar10) {
            uVar11 = local_6f8 + lVar14 * 0x18;
            if (iVar8 != -1) goto LAB_141d0cef5;
            break;
          }
          iVar8 = *(int *)(local_6f8 + 0x10 + lVar14 * 0x18);
        }
        uVar11 = uVar16;
LAB_141d0cef5:
        if (*(longlong *)(*local_758 + 8 + (longlong)uStack_740._4_4_ * 0x18) !=
            *(longlong *)(uVar11 + 8)) goto LAB_141d0cea8;
        uStack_740 = CONCAT44(uStack_740._4_4_,(uint)uStack_740 & ~uStack_750._4_4_);
        FUN_1407e9ca0(&uStack_750);
      } while( true );
    }
    goto LAB_141d0cf60;
  case 0xe:
    cVar4 = FUN_141684430((longlong)param_1 + 0x12d4);
    if (cVar4 != '\0') {
      return;
    }
    uVar12 = FUN_1411fc890();
    FUN_1412169a0(uVar12,*(undefined1 *)((longlong)param_1 + 0x12e1));
    uVar12 = FUN_1411fc890();
    FUN_141216990(uVar12,*(undefined1 *)((longlong)param_1 + 0x12e2));
    iVar8 = opus_repacketizer_get_nb_frames((longlong)param_1 + 0x12d4);
    if (iVar8 < 0) {
      FUN_141698bf0(param_1 + (longlong)*(int *)((longlong)param_1 + 0x484) * 0xc + 0x235,
                    (int)param_1[0x91],0,1);
      FUN_141696f60(param_1 + (longlong)*(int *)((longlong)param_1 + 0x484) * 0xc + 0x235,
                    param_1[0x83],0xffffffff);
      FUN_141d082d0(param_1);
      if (param_1[0x83] != 0) {
        FUN_1415c5350();
      }
    }
    else {
      FUN_14169ec00(param_1 + (longlong)*(int *)((longlong)param_1 + 0x484) * 0xc + 0x235,
                    (int)param_1[0x91],iVar8,0);
      FUN_141698bf0(param_1 + (longlong)*(int *)((longlong)param_1 + 0x484) * 0xc + 0x235,
                    (int)param_1[0x91],0,0);
      FUN_141696f60(param_1 + (longlong)*(int *)((longlong)param_1 + 0x484) * 0xc + 0x235,
                    param_1[0x83],0xffffffff);
      FUN_141d082d0(param_1);
      if (param_1[0x83] != 0) {
        FUN_1415c54b0();
        *(undefined1 *)((longlong)param_1 + 0x494) = 1;
        FUN_141d28ad0(param_1,1);
        return;
      }
    }
    break;
  case 0x10:
    cVar4 = FUN_141d100a0(param_1 + 0x259);
    if (cVar4 == '\0') {
      *(undefined1 *)((longlong)param_1 + 0x494) = 2;
      FUN_141d28ad0(param_1,1);
      *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0xf7;
      *(undefined1 *)((longlong)param_1 + 0x482) = 0;
      FUN_141d0de60(param_1);
      return;
    }
    return;
  case 0x11:
    cVar4 = FUN_14168f020(param_1 + 0x259);
    if (cVar4 != '\0') {
      return;
    }
  default:
    *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0xf7;
    return;
  }
  *(undefined1 *)((longlong)param_1 + 0x494) = 1;
  FUN_141d28ad0(param_1,1);
  return;
LAB_141d0cea8:
  cVar4 = '\x01';
LAB_141d0cf2b:
  FUN_141a84ae0(local_7a0);
  FUN_141d05990();
  local_6b0 = 0;
  if (local_6b8 != (undefined1 *)0x0) {
    FUN_141de9200();
  }
  FUN_1407fa390(&local_6f8);
  if (cVar4 != '\0') {
    *(undefined1 *)(local_7b8 + 0xb) = 1;
  }
LAB_141d0cf60:
  local_7b8 = (longlong *)0x0;
  local_7b0 = 0x18;
  FUN_1407e2ee0(&local_7b8,0);
  FUN_141dd1c10(local_7b8,L"Option_Complete_Default",0x30);
  FUN_141d1bfa0(param_1 + 0x259,&local_7b8,1);
LAB_141d0d022:
  FUN_141d29690(param_1);
  *(undefined1 *)((longlong)param_1 + 0x494) = 0xd;
  local_7b8 = (longlong *)0x0;
  local_7b0 = 0x18;
  FUN_1407e2ee0(&local_7b8,0);
  FUN_141dd1c10(local_7b8,L"Option_Complete_Default",0x30);
  FUN_141d1bfa0(param_1 + 0x259,&local_7b8,1);
  return;
}



// ================= TARGET 0x1405dbded  FUN_1405dbded @ 1405dbded  size=0x12 =================
// callers: <none>

void FUN_1405dbded(undefined8 param_1,undefined8 param_2)

{
  FUN_14204ef20(&PTR_PTR_145693a80,param_2,0x9af98);
  return;
}



// ================= TARGET 0x1403ead7d  FUN_1403ead7d @ 1403ead7d  size=0x12 =================
// callers: <none>

void FUN_1403ead7d(undefined8 param_1,undefined8 param_2)

{
  FUN_14204ef20(&PTR_PTR_1454e2788,param_2,0x850);
  return;
}



