// FUN_1416f7bd0 @ 1416f7bd0

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1416f7bd0(longlong *param_1,byte param_2,char param_3,char param_4)

{
  longlong *plVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  ulonglong uVar5;
  wchar_t wVar6;
  wint_t wVar7;
  undefined8 uVar8;
  wchar_t **ppwVar9;
  int iVar10;
  wint_t *pwVar11;
  wchar_t *_Src;
  uint uVar12;
  int iVar13;
  wchar_t *pwVar14;
  longlong *plVar15;
  int iVar16;
  longlong lVar17;
  wchar_t *pwVar18;
  wchar_t *pwVar19;
  int iVar20;
  bool bVar21;
  undefined1 auStack_488 [32];
  int local_468;
  wchar_t *local_458;
  undefined8 local_450;
  char local_448;
  wchar_t *local_440;
  int local_438;
  int local_434;
  wchar_t *local_430;
  undefined8 local_428;
  wchar_t *local_420;
  int local_418;
  int local_414;
  longlong local_410 [3];
  wchar_t *local_3f8;
  wint_t *local_3f0 [70];
  undefined8 local_1c0;
  undefined8 local_1b8;
  undefined8 local_1b0;
  undefined8 local_1a8;
  undefined8 local_1a0;
  undefined8 local_198;
  undefined8 local_190;
  undefined8 local_188;
  undefined8 local_180;
  undefined8 local_178;
  undefined8 local_170;
  undefined8 local_168;
  undefined8 local_160;
  undefined8 local_158;
  undefined8 local_150;
  undefined8 local_148;
  undefined8 local_140;
  undefined8 local_138;
  undefined8 local_130;
  undefined8 local_128;
  undefined8 local_120;
  undefined8 local_118;
  undefined8 local_110;
  undefined8 local_108;
  undefined8 local_100;
  undefined8 local_f8;
  undefined8 local_f0;
  undefined8 local_e8;
  undefined8 local_e0;
  undefined8 local_d8;
  undefined8 local_d0;
  undefined8 local_c8;
  undefined8 local_c0;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined8 local_70;
  undefined8 local_68;
  undefined8 local_60;
  longlong local_58 [4];
  
  local_58[2] = DAT_145909780 ^ (ulonglong)auStack_488;
  plVar15 = (longlong *)param_1[0x74];
  _Src = (wchar_t *)0x0;
  local_420 = (wchar_t *)CONCAT71(local_420._1_7_,param_3);
  local_458 = (wchar_t *)0x0;
  local_450 = 0;
  local_430 = (wchar_t *)0x0;
  local_428 = (wchar_t *)0x0;
  pwVar14 = _Src;
  local_448 = param_4;
  if (param_3 == '\b') {
    if (plVar15 != (longlong *)0x0) {
      (**(code **)(*plVar15 + 0x250))(plVar15,3);
    }
    plVar15 = (longlong *)param_1[0x75];
    if (plVar15 != (longlong *)0x0) {
      (**(code **)(*plVar15 + 0x250))(plVar15,1);
    }
    *(undefined1 *)(param_1 + 0x80) = 0;
  }
  else {
    if (plVar15 != (longlong *)0x0) {
      (**(code **)(*plVar15 + 0x250))(plVar15,1);
    }
    plVar15 = (longlong *)param_1[0x75];
    if (plVar15 != (longlong *)0x0) {
      (**(code **)(*plVar15 + 0x250))(plVar15,3);
    }
    param_2 = 0x19;
    *(undefined1 *)(param_1 + 0x80) = 1;
    switch(param_3) {
    case '\0':
      local_450 = (ulonglong)local_450._4_4_ << 0x20;
      FUN_1407f1af0(&local_458,0xd);
      uVar12 = (uint)local_450 + 0xd;
      local_450 = CONCAT44(local_450._4_4_,uVar12);
      if ((int)local_450._4_4_ < (int)uVar12) {
        FUN_1407e2ee0(&local_458);
        uVar12 = (uint)local_450;
      }
      uVar4 = u_Lang_Commu00_143f0f3c0._12_4_;
      uVar3 = u_Lang_Commu00_143f0f3c0._8_4_;
      uVar2 = u_Lang_Commu00_143f0f3c0._4_4_;
      pwVar14 = (wchar_t *)(ulonglong)uVar12;
      *(undefined4 *)local_458 = u_Lang_Commu00_143f0f3c0._0_4_;
      *(undefined4 *)(local_458 + 2) = uVar2;
      *(undefined4 *)(local_458 + 4) = uVar3;
      *(undefined4 *)(local_458 + 6) = uVar4;
      *(undefined8 *)(local_458 + 8) = u_Lang_Commu00_143f0f3c0._16_8_;
      wVar6 = u_Lang_Commu00_143f0f3c0[0xc];
      break;
    case '\x01':
      local_450 = (ulonglong)local_450._4_4_ << 0x20;
      FUN_1407f1af0(&local_458,0xd);
      uVar12 = (uint)local_450 + 0xd;
      local_450 = CONCAT44(local_450._4_4_,uVar12);
      if ((int)local_450._4_4_ < (int)uVar12) {
        FUN_1407e2ee0(&local_458);
        uVar12 = (uint)local_450;
      }
      uVar4 = u_Lang_Commu01_143f0f3e0._12_4_;
      uVar3 = u_Lang_Commu01_143f0f3e0._8_4_;
      uVar2 = u_Lang_Commu01_143f0f3e0._4_4_;
      pwVar14 = (wchar_t *)(ulonglong)uVar12;
      *(undefined4 *)local_458 = u_Lang_Commu01_143f0f3e0._0_4_;
      *(undefined4 *)(local_458 + 2) = uVar2;
      *(undefined4 *)(local_458 + 4) = uVar3;
      *(undefined4 *)(local_458 + 6) = uVar4;
      *(undefined8 *)(local_458 + 8) = u_Lang_Commu01_143f0f3e0._16_8_;
      wVar6 = u_Lang_Commu01_143f0f3e0[0xc];
      break;
    case '\x02':
      local_450 = (ulonglong)local_450._4_4_ << 0x20;
      FUN_1407f1af0(&local_458,0xd);
      uVar12 = (uint)local_450 + 0xd;
      local_450 = CONCAT44(local_450._4_4_,uVar12);
      if ((int)local_450._4_4_ < (int)uVar12) {
        FUN_1407e2ee0(&local_458);
        uVar12 = (uint)local_450;
      }
      uVar4 = u_Lang_Commu02_143f0f450._12_4_;
      uVar3 = u_Lang_Commu02_143f0f450._8_4_;
      uVar2 = u_Lang_Commu02_143f0f450._4_4_;
      pwVar14 = (wchar_t *)(ulonglong)uVar12;
      *(undefined4 *)local_458 = u_Lang_Commu02_143f0f450._0_4_;
      *(undefined4 *)(local_458 + 2) = uVar2;
      *(undefined4 *)(local_458 + 4) = uVar3;
      *(undefined4 *)(local_458 + 6) = uVar4;
      *(undefined8 *)(local_458 + 8) = u_Lang_Commu02_143f0f450._16_8_;
      wVar6 = u_Lang_Commu02_143f0f450[0xc];
      break;
    case '\x03':
      local_450 = (ulonglong)local_450._4_4_ << 0x20;
      FUN_1407f1af0(&local_458,0xd);
      uVar12 = (uint)local_450 + 0xd;
      local_450 = CONCAT44(local_450._4_4_,uVar12);
      if ((int)local_450._4_4_ < (int)uVar12) {
        FUN_1407e2ee0(&local_458);
        uVar12 = (uint)local_450;
      }
      uVar4 = u_Lang_Commu03_143f0f4d0._12_4_;
      uVar3 = u_Lang_Commu03_143f0f4d0._8_4_;
      uVar2 = u_Lang_Commu03_143f0f4d0._4_4_;
      pwVar14 = (wchar_t *)(ulonglong)uVar12;
      *(undefined4 *)local_458 = u_Lang_Commu03_143f0f4d0._0_4_;
      *(undefined4 *)(local_458 + 2) = uVar2;
      *(undefined4 *)(local_458 + 4) = uVar3;
      *(undefined4 *)(local_458 + 6) = uVar4;
      *(undefined8 *)(local_458 + 8) = u_Lang_Commu03_143f0f4d0._16_8_;
      wVar6 = u_Lang_Commu03_143f0f4d0[0xc];
      break;
    case '\x04':
      local_450 = (ulonglong)local_450._4_4_ << 0x20;
      FUN_1407f1af0(&local_458,0xd);
      uVar12 = (uint)local_450 + 0xd;
      local_450 = CONCAT44(local_450._4_4_,uVar12);
      if ((int)local_450._4_4_ < (int)uVar12) {
        FUN_1407e2ee0(&local_458);
        uVar12 = (uint)local_450;
      }
      uVar4 = u_Lang_Commu04_143f0f4f0._12_4_;
      uVar3 = u_Lang_Commu04_143f0f4f0._8_4_;
      uVar2 = u_Lang_Commu04_143f0f4f0._4_4_;
      pwVar14 = (wchar_t *)(ulonglong)uVar12;
      *(undefined4 *)local_458 = u_Lang_Commu04_143f0f4f0._0_4_;
      *(undefined4 *)(local_458 + 2) = uVar2;
      *(undefined4 *)(local_458 + 4) = uVar3;
      *(undefined4 *)(local_458 + 6) = uVar4;
      *(undefined8 *)(local_458 + 8) = u_Lang_Commu04_143f0f4f0._16_8_;
      wVar6 = u_Lang_Commu04_143f0f4f0[0xc];
      break;
    case '\x05':
      local_450 = (ulonglong)local_450._4_4_ << 0x20;
      FUN_1407f1af0(&local_458,0xd);
      uVar12 = (uint)local_450 + 0xd;
      local_450 = CONCAT44(local_450._4_4_,uVar12);
      if ((int)local_450._4_4_ < (int)uVar12) {
        FUN_1407e2ee0(&local_458);
        uVar12 = (uint)local_450;
      }
      uVar4 = u_Lang_Commu05_143f0f510._12_4_;
      uVar3 = u_Lang_Commu05_143f0f510._8_4_;
      uVar2 = u_Lang_Commu05_143f0f510._4_4_;
      pwVar14 = (wchar_t *)(ulonglong)uVar12;
      *(undefined4 *)local_458 = u_Lang_Commu05_143f0f510._0_4_;
      *(undefined4 *)(local_458 + 2) = uVar2;
      *(undefined4 *)(local_458 + 4) = uVar3;
      *(undefined4 *)(local_458 + 6) = uVar4;
      *(undefined8 *)(local_458 + 8) = u_Lang_Commu05_143f0f510._16_8_;
      wVar6 = u_Lang_Commu05_143f0f510[0xc];
      break;
    case '\x06':
      local_450 = (ulonglong)local_450._4_4_ << 0x20;
      FUN_1407f1af0(&local_458,0xd);
      uVar12 = (uint)local_450 + 0xd;
      local_450 = CONCAT44(local_450._4_4_,uVar12);
      if ((int)local_450._4_4_ < (int)uVar12) {
        FUN_1407e2ee0(&local_458);
        uVar12 = (uint)local_450;
      }
      uVar4 = u_Lang_Commu06_143f0f530._12_4_;
      uVar3 = u_Lang_Commu06_143f0f530._8_4_;
      uVar2 = u_Lang_Commu06_143f0f530._4_4_;
      pwVar14 = (wchar_t *)(ulonglong)uVar12;
      *(undefined4 *)local_458 = u_Lang_Commu06_143f0f530._0_4_;
      *(undefined4 *)(local_458 + 2) = uVar2;
      *(undefined4 *)(local_458 + 4) = uVar3;
      *(undefined4 *)(local_458 + 6) = uVar4;
      *(undefined8 *)(local_458 + 8) = u_Lang_Commu06_143f0f530._16_8_;
      wVar6 = u_Lang_Commu06_143f0f530[0xc];
      break;
    default:
      local_450 = (ulonglong)local_450._4_4_ << 0x20;
      goto LAB_1416f8090;
    }
    _Src = local_458;
    local_458[0xc] = wVar6;
    iVar13 = (int)pwVar14;
    if (1 < iVar13) {
      iVar20 = iVar13 + -1;
      if (iVar13 == 0) {
        iVar20 = 0;
      }
      local_440 = (wchar_t *)0x0;
      local_434 = 0;
      local_438 = iVar20 + 0x23;
      if (0 < local_438) {
        FUN_1407e2ee0(&local_440,0);
      }
      iVar13 = local_438;
      pwVar19 = local_440;
      uVar8 = u__Game_Art_UI_L10N_LANG_Icon_Comm_143f0f550._8_8_;
      *(undefined8 *)local_440 = u__Game_Art_UI_L10N_LANG_Icon_Comm_143f0f550._0_8_;
      *(undefined8 *)(local_440 + 4) = uVar8;
      uVar8 = u__Game_Art_UI_L10N_LANG_Icon_Comm_143f0f550._24_8_;
      *(undefined8 *)(local_440 + 8) = u__Game_Art_UI_L10N_LANG_Icon_Comm_143f0f550._16_8_;
      *(undefined8 *)(local_440 + 0xc) = uVar8;
      uVar8 = u__Game_Art_UI_L10N_LANG_Icon_Comm_143f0f550._40_8_;
      *(undefined8 *)(local_440 + 0x10) = u__Game_Art_UI_L10N_LANG_Icon_Comm_143f0f550._32_8_;
      *(undefined8 *)(local_440 + 0x14) = uVar8;
      uVar8 = u__Game_Art_UI_L10N_LANG_Icon_Comm_143f0f550._56_8_;
      *(undefined8 *)(local_440 + 0x18) = u__Game_Art_UI_L10N_LANG_Icon_Comm_143f0f550._48_8_;
      *(undefined8 *)(local_440 + 0x1c) = uVar8;
      *(undefined4 *)(local_440 + 0x20) = u__Game_Art_UI_L10N_LANG_Icon_Comm_143f0f550._64_4_;
      memcpy(local_440 + 0x22,_Src,(longlong)iVar20 * 2);
      pwVar19[(longlong)iVar20 + 0x22] = L'\0';
      if (local_430 != (wchar_t *)0x0) {
        FUN_141de9200();
      }
      local_428 = (wchar_t *)CONCAT44(local_434,iVar13);
      local_440 = (wchar_t *)0x0;
      local_438 = iVar13;
      local_430 = pwVar19;
      if (iVar13 == 0) {
        local_434 = 0;
      }
      else {
        FUN_1407f1420(&local_440,iVar13,0);
        memcpy(local_440,pwVar19,(longlong)iVar13 * 2);
      }
      ppwVar9 = (wchar_t **)FUN_141a690a0(local_410,&local_440,0);
      if (&local_430 != ppwVar9) {
        if (local_430 != (wchar_t *)0x0) {
          FUN_141de9200();
        }
        local_430 = *ppwVar9;
        *ppwVar9 = (wchar_t *)0x0;
        local_428 = ppwVar9[1];
        ppwVar9[1] = (wchar_t *)0x0;
      }
      if (local_410[0] != 0) {
        FUN_141de9200();
      }
      FUN_14159f740(param_1 + 0x97,&local_430);
      param_3 = (char)local_420;
    }
LAB_1416f8090:
    FUN_141a83cf0(param_1[0x78],param_3);
  }
  local_3f8 = (wchar_t *)0x0;
  local_3f0[0] = (wint_t *)0xb;
  FUN_1407e2ee0(&local_3f8,0);
  FUN_141dd1c10(local_3f8,L"Lang_Commu",0x16);
  local_3f0[1] = (wint_t *)0x0;
  local_3f0[2] = (wint_t *)0x10;
  FUN_1407e2ee0(local_3f0 + 1,0);
  FUN_141dd1c10(local_3f0[1],L"Lang_Commuboard",0x20);
  local_3f0[3] = (wint_t *)0x0;
  local_3f0[4] = (wint_t *)0xa;
  FUN_1407e2ee0(local_3f0 + 3,0);
  FUN_141dd1c10(local_3f0[3],L"Lang_Item",0x14);
  local_3f0[5] = (wint_t *)0x0;
  local_3f0[6] = (wint_t *)0x9;
  FUN_1407e2ee0(local_3f0 + 5,0);
  FUN_141dd1c10(local_3f0[5],L"Lang_Opt",0x12);
  local_3f0[7] = (wint_t *)0x0;
  local_3f0[8] = (wint_t *)0xa;
  FUN_1407e2ee0(local_3f0 + 7,0);
  FUN_141dd1c10(local_3f0[7],L"Lang_Save",0x14);
  local_3f0[9] = (wint_t *)0x0;
  local_3f0[10] = (wint_t *)0xf;
  FUN_1407e2ee0(local_3f0 + 9,0);
  FUN_141dd1c10(local_3f0[9],L"Lang_Soulbadge",0x1e);
  local_3f0[0xb] = (wint_t *)0x0;
  local_3f0[0xc] = (wint_t *)0x11;
  FUN_1407e2ee0(local_3f0 + 0xb,0);
  FUN_141dd1c10(local_3f0[0xb],L"Lang_Storyresult",0x22);
  local_3f0[0xd] = (wint_t *)0x0;
  local_3f0[0xe] = (wint_t *)0xe;
  FUN_1407e2ee0(local_3f0 + 0xd,0);
  FUN_141dd1c10(local_3f0[0xd],L"Lang_MainMenu",0x1c);
  local_3f0[0xf] = (wint_t *)0x0;
  local_3f0[0x10] = (wint_t *)0xb;
  FUN_1407e2ee0(local_3f0 + 0xf,0);
  FUN_141dd1c10(local_3f0[0xf],L"Lang_World",0x16);
  local_3f0[0x11] = (wint_t *)0x0;
  local_3f0[0x12] = (wint_t *)0xa;
  FUN_1407e2ee0(local_3f0 + 0x11,0);
  FUN_141dd1c10(local_3f0[0x11],L"Lang_Char",0x14);
  local_3f0[0x13] = (wint_t *)0x0;
  local_3f0[0x14] = (wint_t *)0xf;
  FUN_1407e2ee0(local_3f0 + 0x13,0);
  FUN_141dd1c10(local_3f0[0x13],L"Lang_Customize",0x1e);
  local_3f0[0x15] = (wint_t *)0x0;
  local_3f0[0x16] = (wint_t *)0xd;
  FUN_1407e2ee0(local_3f0 + 0x15,0);
  FUN_141dd1c10(local_3f0[0x15],L"Lang_Develop",0x1a);
  local_3f0[0x17] = (wint_t *)0x0;
  local_3f0[0x18] = (wint_t *)0xd;
  FUN_1407e2ee0(local_3f0 + 0x17,0);
  FUN_141dd1c10(local_3f0[0x17],L"Lang_Powerup",0x1a);
  local_3f0[0x19] = (wint_t *)0x0;
  local_3f0[0x1a] = (wint_t *)0xd;
  FUN_1407e2ee0(local_3f0 + 0x19,0);
  FUN_141dd1c10(local_3f0[0x19],L"Lang_Cooking",0x1a);
  local_3f0[0x1b] = (wint_t *)0x0;
  local_3f0[0x1c] = (wint_t *)0xf;
  FUN_1407e2ee0(local_3f0 + 0x1b,0);
  FUN_141dd1c10(local_3f0[0x1b],L"Lang_Skilltree",0x1e);
  local_3f0[0x1d] = (wint_t *)0x0;
  local_3f0[0x1e] = (wint_t *)0xe;
  FUN_1407e2ee0(local_3f0 + 0x1d,0);
  FUN_141dd1c10(local_3f0[0x1d],L"Lang_Skillset",0x1c);
  local_3f0[0x1f] = (wint_t *)0x0;
  local_3f0[0x20] = (wint_t *)0xb;
  FUN_1407e2ee0(local_3f0 + 0x1f,0);
  FUN_141dd1c10(local_3f0[0x1f],L"Lang_Quest",0x16);
  local_3f0[0x21] = (wint_t *)0x0;
  local_3f0[0x22] = (wint_t *)0xa;
  FUN_1407e2ee0(local_3f0 + 0x21,0);
  FUN_141dd1c10(local_3f0[0x21],L"Lang_Tips",0x14);
  local_3f0[0x23] = (wint_t *)0x0;
  local_3f0[0x24] = (wint_t *)0xa;
  FUN_1407e2ee0(local_3f0 + 0x23,0);
  FUN_141dd1c10(local_3f0[0x23],L"Lang_Load",0x14);
  local_3f0[0x25] = (wint_t *)0x0;
  local_3f0[0x26] = (wint_t *)0x8;
  FUN_1407e2ee0(local_3f0 + 0x25,0);
  FUN_141dd1c10(local_3f0[0x25],L"Lang_Db",0x10);
  local_3f0[0x27] = (wint_t *)0x0;
  local_3f0[0x28] = (wint_t *)0xa;
  FUN_1407e2ee0(local_3f0 + 0x27,0);
  FUN_141dd1c10(local_3f0[0x27],L"Lang_Shop",0x14);
  local_3f0[0x29] = (wint_t *)0x0;
  local_3f0[0x2a] = (wint_t *)0xd;
  FUN_1407e2ee0(local_3f0 + 0x29,0);
  FUN_141dd1c10(local_3f0[0x29],L"Lang_Areamap",0x1a);
  local_3f0[0x2b] = (wint_t *)0x0;
  local_3f0[0x2c] = (wint_t *)0x13;
  FUN_1407e2ee0(local_3f0 + 0x2b,0);
  FUN_141dd1c10(local_3f0[0x2b],L"Lang_Minigame_Race",0x26);
  local_3f0[0x2d] = (wint_t *)0x0;
  local_3f0[0x2e] = (wint_t *)0x17;
  FUN_1407e2ee0(local_3f0 + 0x2d,0);
  FUN_141dd1c10(local_3f0[0x2d],L"Lang_Minigame_BaseBall",0x2e);
  local_3f0[0x2f] = (wint_t *)0x0;
  local_3f0[0x30] = (wint_t *)0xb;
  FUN_1407e2ee0(local_3f0 + 0x2f,0);
  FUN_141dd1c10(local_3f0[0x2f],L"Lang_Party",0x16);
  local_3f0[0x31] = (wint_t *)0x0;
  local_3f0[0x32] = (wint_t *)0x10;
  FUN_1407e2ee0(local_3f0 + 0x31,0);
  FUN_141dd1c10(local_3f0[0x31],L"Lang_Commuquest",0x20);
  local_3f0[0x33] = (wint_t *)0x0;
  local_3f0[0x34] = (wint_t *)0xc;
  FUN_1407e2ee0(local_3f0 + 0x33,0);
  FUN_141dd1c10(local_3f0[0x33],L"Lang_System",0x18);
  local_3f0[0x35] = (wint_t *)0x0;
  local_3f0[0x36] = (wint_t *)0xf;
  FUN_1407e2ee0(local_3f0 + 0x35,0);
  FUN_141dd1c10(local_3f0[0x35],L"Lang_Shop_Info",0x1e);
  local_3f0[0x37] = (wint_t *)0x0;
  local_3f0[0x38] = (wint_t *)0x11;
  FUN_1407e2ee0(local_3f0 + 0x37,0);
  FUN_141dd1c10(local_3f0[0x37],L"Lang_Past_Quests",0x22);
  local_3f0[0x39] = (wint_t *)0x0;
  local_3f0[0x3a] = (wint_t *)0xe;
  FUN_1407e2ee0(local_3f0 + 0x39,0);
  FUN_141dd1c10(local_3f0[0x39],L"Lang_Mealtime",0x1c);
  local_3f0[0x3b] = (wint_t *)0x0;
  local_3f0[0x3c] = (wint_t *)0xf;
  FUN_1407e2ee0(local_3f0 + 0x3b,0);
  FUN_141dd1c10(local_3f0[0x3b],L"Lang_Shop_Food",0x1e);
  local_3f0[0x3d] = (wint_t *)0x0;
  local_3f0[0x3e] = (wint_t *)0x13;
  FUN_1407e2ee0(local_3f0 + 0x3d,0);
  FUN_141dd1c10(local_3f0[0x3d],L"Lang_Shop_Material",0x26);
  local_3f0[0x3f] = (wint_t *)0x0;
  local_3f0[0x40] = (wint_t *)0x10;
  FUN_1407e2ee0(local_3f0 + 0x3f,0);
  FUN_141dd1c10(local_3f0[0x3f],L"Lang_Shop_Trade",0x20);
  local_3f0[0x41] = (wint_t *)0x0;
  local_3f0[0x42] = (wint_t *)0xe;
  FUN_1407e2ee0(local_3f0 + 0x41,0);
  FUN_141dd1c10(local_3f0[0x41],L"Lang_Learning",0x1c);
  local_3f0[0x43] = (wint_t *)0x0;
  local_3f0[0x44] = (wint_t *)0xa;
  FUN_1407e2ee0(local_3f0 + 0x43,0);
  FUN_141dd1c10(local_3f0[0x43],L"Lang_Trai",0x14);
  local_3f0[0x45] = (wint_t *)0x0;
  local_1c0 = 9;
  FUN_1407e2ee0(local_3f0 + 0x45,0);
  FUN_141dd1c10(local_3f0[0x45],L"Lang_DLC",0x12);
  local_1b8 = 0;
  local_1b0 = 0x13;
  FUN_1407e2ee0(&local_1b8,0);
  FUN_141dd1c10(local_1b8,L"Lang_Past_Quests01",0x26);
  local_1a8 = 0;
  local_1a0 = 0xd;
  FUN_1407e2ee0(&local_1a8,0);
  FUN_141dd1c10(local_1a8,L"Lang_Skyseed",0x1a);
  local_198 = 0;
  local_190 = 0x16;
  FUN_1407e2ee0(&local_198,0);
  FUN_141dd1c10(local_198,L"Lang_VolcanrockTrader",0x2c);
  local_188 = 0;
  local_180 = 0xe;
  FUN_1407e2ee0(&local_188,0);
  FUN_141dd1c10(local_188,L"Lang_Shop_Bug",0x1c);
  local_178 = 0;
  local_170 = 0x11;
  FUN_1407e2ee0(&local_178,0);
  FUN_141dd1c10(local_178,L"Lang_Fast_Travel",0x22);
  local_168 = 0;
  local_160 = 10;
  FUN_1407e2ee0(&local_168,0);
  FUN_141dd1c10(local_168,L"Lang_Home",0x14);
  local_158 = 0;
  local_150 = 0xd;
  FUN_1407e2ee0(&local_158,0);
  FUN_141dd1c10(local_158,L"Lang_Profile",0x1a);
  local_148 = 0;
  local_140 = 0x11;
  FUN_1407e2ee0(&local_148,0);
  FUN_141dd1c10(local_148,L"Lang_Deck_Select",0x22);
  local_138 = 0;
  local_130 = 0xf;
  FUN_1407e2ee0(&local_138,0);
  FUN_141dd1c10(local_138,L"Lang_Deck_Edit",0x1e);
  local_128 = 0;
  local_120 = 0xc;
  FUN_1407e2ee0(&local_128,0);
  FUN_141dd1c10(local_128,L"Lang_Bdokai",0x18);
  local_118 = 0;
  local_110 = 0x12;
  FUN_1407e2ee0(&local_118,0);
  FUN_141dd1c10(local_118,L"Lang_Mission_List",0x24);
  local_108 = 0;
  local_100 = 0xe;
  FUN_1407e2ee0(&local_108,0);
  FUN_141dd1c10(local_108,L"Lang_Gift_Box",0x1c);
  local_f8 = 0;
  local_f0 = 10;
  FUN_1407e2ee0(&local_f8,0);
  FUN_141dd1c10(local_f8,L"Lang_Rank",0x14);
  local_e8 = 0;
  local_e0 = 10;
  FUN_1407e2ee0(&local_e8,0);
  FUN_141dd1c10(local_e8,L"Lang_Help",0x14);
  local_d8 = 0;
  local_d0 = 0xe;
  FUN_1407e2ee0(&local_d8,0);
  FUN_141dd1c10(local_d8,L"Lang_Training",0x1c);
  local_c8 = 0;
  local_c0 = 0xc;
  FUN_1407e2ee0(&local_c8,0);
  FUN_141dd1c10(local_c8,L"Lang_Option",0x18);
  local_b8 = 0;
  local_b0 = 0x11;
  FUN_1407e2ee0(&local_b8,0);
  FUN_141dd1c10(local_b8,L"Lang_Shop_Select",0x22);
  local_a8 = 0;
  local_a0 = 0xc;
  FUN_1407e2ee0(&local_a8,0);
  FUN_141dd1c10(local_a8,L"Lang_System",0x18);
  local_98 = 0;
  local_90 = 0x13;
  FUN_1407e2ee0(&local_98,0);
  FUN_141dd1c10(local_98,L"Lang_Battle_Select",0x26);
  local_88 = 0;
  local_80 = 0xd;
  FUN_1407e2ee0(&local_88,0);
  FUN_141dd1c10(local_88,L"Lang_Gallery",0x1a);
  local_78 = 0;
  local_70 = 0x12;
  FUN_1407e2ee0(&local_78,0);
  FUN_141dd1c10(local_78,L"Lang_Battle_Lobby",0x24);
  local_68 = 0;
  local_60 = 0x12;
  FUN_1407e2ee0(&local_68,0);
  FUN_141dd1c10(local_68,L"Lang_Stage_Select",0x24);
  local_58[0] = 0;
  local_58[1] = 0x13;
  FUN_1407e2ee0(local_58,0);
  FUN_141dd1c10(local_58[0],L"Lang_Random_Battle",0x26);
  if (param_2 < 0x3b) {
    if (&local_458 == &local_3f8 + (ulonglong)param_2 * 2) goto LAB_1416f8dc8;
    local_468 = 0;
    FUN_1407e4b70(&local_458,(&local_3f8)[(ulonglong)param_2 * 2],
                  *(undefined4 *)(local_3f0 + (ulonglong)param_2 * 2),local_450._4_4_);
  }
  else {
    if (_Src == L"") goto LAB_1416f8dc8;
    bVar21 = local_450._4_4_ != 0;
    pwVar14 = (wchar_t *)0x0;
    local_450 = (ulonglong)local_450._4_4_ << 0x20;
    if (bVar21) {
      FUN_1407f1af0(&local_458,0);
      pwVar14 = (wchar_t *)(local_450 & 0xffffffff);
      _Src = local_458;
    }
    if ((int)pwVar14 <= (int)local_450._4_4_) goto LAB_1416f8dc8;
    FUN_1407e2ee0(&local_458,pwVar14);
  }
  pwVar14 = (wchar_t *)(local_450 & 0xffffffff);
  _Src = local_458;
LAB_1416f8dc8:
  iVar13 = (int)pwVar14;
  if (1 < iVar13) {
    local_458 = (wchar_t *)0x0;
    local_450 = local_450 & 0xffffffff;
    if (param_2 < 0x29) {
      if (param_2 < 0x25) {
        local_450 = 0;
        FUN_1407f1af0(&local_458,0x24);
        iVar20 = (uint)local_450 + 0x24;
        local_450 = CONCAT44(local_450._4_4_,iVar20);
        if ((int)local_450._4_4_ < iVar20) {
          FUN_1407e2ee0(&local_458);
          iVar20 = (uint)local_450;
        }
        uVar8 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f101e0._8_8_;
        *(undefined8 *)local_458 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f101e0._0_8_;
        *(undefined8 *)(local_458 + 4) = uVar8;
        uVar8 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f101e0._24_8_;
        *(undefined8 *)(local_458 + 8) = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f101e0._16_8_;
        *(undefined8 *)(local_458 + 0xc) = uVar8;
        uVar8 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f101e0._40_8_;
        *(undefined8 *)(local_458 + 0x10) = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f101e0._32_8_;
        *(undefined8 *)(local_458 + 0x14) = uVar8;
        uVar8 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f101e0._56_8_;
        *(undefined8 *)(local_458 + 0x18) = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f101e0._48_8_;
        *(undefined8 *)(local_458 + 0x1c) = uVar8;
        *(undefined8 *)(local_458 + 0x20) = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f101e0._64_8_;
        pwVar19 = local_458;
      }
      else {
        local_420 = (wchar_t *)FUN_140d25f00(local_410,0,0);
        local_440 = (wchar_t *)0x0;
        local_434 = 0;
        iVar20 = *(int *)(local_420 + 4) + -1;
        if (*(int *)(local_420 + 4) == 0) {
          iVar20 = 0;
        }
        local_438 = iVar20 + 0xb;
        if (0 < local_438) {
          FUN_1407e2ee0(&local_440,0);
        }
        iVar10 = local_434;
        iVar16 = local_438;
        pwVar19 = local_440;
        uVar8 = _UNK_143f10130;
        *(undefined8 *)local_440 = _DAT_143f10128;
        *(undefined8 *)(local_440 + 4) = uVar8;
        *(undefined4 *)(local_440 + 8) = DAT_143f10138;
        memcpy(local_440 + 10,*(void **)local_420,(longlong)iVar20 * 2);
        pwVar19[(longlong)iVar20 + 10] = L'\0';
        local_440 = pwVar19;
        local_438 = iVar16;
        local_434 = iVar10;
        FUN_1407e4910(&local_420,&local_440);
        iVar16 = local_418;
        lVar17 = (longlong)local_418;
        local_418 = (0x1f - (uint)(local_418 != 0)) + local_418;
        if (local_414 < local_418) {
          FUN_1407e2ee0(&local_420,iVar16);
        }
        iVar20 = local_418;
        pwVar19 = local_420;
        uVar8 = u__Art_UI_L10N_LANG_Xcmn_Header__143f101a0._8_8_;
        local_458 = local_420;
        local_450 = CONCAT44(local_414,(uint)local_450);
        lVar17 = lVar17 - (ulonglong)(iVar16 != 0);
        *(undefined8 *)(local_420 + lVar17) = u__Art_UI_L10N_LANG_Xcmn_Header__143f101a0._0_8_;
        *(undefined8 *)(local_420 + lVar17 + 4) = uVar8;
        uVar8 = u__Art_UI_L10N_LANG_Xcmn_Header__143f101a0._24_8_;
        *(undefined8 *)(local_420 + lVar17 + 8) = u__Art_UI_L10N_LANG_Xcmn_Header__143f101a0._16_8_;
        *(undefined8 *)(local_420 + lVar17 + 0xc) = uVar8;
        uVar4 = u__Art_UI_L10N_LANG_Xcmn_Header__143f101a0._44_4_;
        uVar3 = u__Art_UI_L10N_LANG_Xcmn_Header__143f101a0._40_4_;
        uVar2 = u__Art_UI_L10N_LANG_Xcmn_Header__143f101a0._36_4_;
        *(undefined4 *)(local_420 + lVar17 + 0x10) =
             u__Art_UI_L10N_LANG_Xcmn_Header__143f101a0._32_4_;
        *(undefined4 *)(local_420 + lVar17 + 0x12) = uVar2;
        *(undefined4 *)(local_420 + lVar17 + 0x14) = uVar3;
        *(undefined4 *)(local_420 + lVar17 + 0x16) = uVar4;
        *(undefined8 *)(local_420 + lVar17 + 0x18) =
             u__Art_UI_L10N_LANG_Xcmn_Header__143f101a0._48_8_;
        *(undefined4 *)(local_420 + lVar17 + 0x1c) =
             u__Art_UI_L10N_LANG_Xcmn_Header__143f101a0._56_4_;
        local_420[lVar17 + 0x1e] = L'\0';
        if (local_440 != (wchar_t *)0x0) {
          FUN_141de9200();
        }
        if (local_410[0] != 0) {
          FUN_141de9200();
        }
      }
    }
    else {
      local_450 = 0;
      FUN_1407f1af0(&local_458,0x2e);
      iVar20 = (uint)local_450 + 0x2e;
      local_450 = CONCAT44(local_450._4_4_,iVar20);
      if ((int)local_450._4_4_ < iVar20) {
        FUN_1407e2ee0(&local_458);
        iVar20 = (uint)local_450;
      }
      uVar8 = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._8_8_;
      *(undefined8 *)local_458 = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._0_8_;
      *(undefined8 *)(local_458 + 4) = uVar8;
      uVar8 = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._24_8_;
      *(undefined8 *)(local_458 + 8) = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._16_8_;
      *(undefined8 *)(local_458 + 0xc) = uVar8;
      uVar8 = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._40_8_;
      *(undefined8 *)(local_458 + 0x10) = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._32_8_;
      *(undefined8 *)(local_458 + 0x14) = uVar8;
      uVar8 = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._56_8_;
      *(undefined8 *)(local_458 + 0x18) = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._48_8_;
      *(undefined8 *)(local_458 + 0x1c) = uVar8;
      uVar4 = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._76_4_;
      uVar3 = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._72_4_;
      uVar2 = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._68_4_;
      *(undefined4 *)(local_458 + 0x20) = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._64_4_;
      *(undefined4 *)(local_458 + 0x22) = uVar2;
      *(undefined4 *)(local_458 + 0x24) = uVar3;
      *(undefined4 *)(local_458 + 0x26) = uVar4;
      *(undefined8 *)(local_458 + 0x28) = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._80_8_;
      *(undefined4 *)(local_458 + 0x2c) = u__Game_DLC_TCG_art_UI_L10N_LANG_C_143f10140._88_4_;
      pwVar19 = local_458;
    }
    pwVar11 = &DAT_14391f7d0;
    if ((int)local_1c0 != 0) {
      pwVar11 = local_3f0[0x45];
    }
    pwVar18 = L"";
    if (iVar13 != 0) {
      pwVar18 = _Src;
    }
    lVar17 = (longlong)pwVar18 - (longlong)pwVar11;
    for (; (wVar7 = *(wint_t *)((longlong)pwVar11 + lVar17), wVar7 != 0 || (*pwVar11 != 0));
        pwVar11 = pwVar11 + 1) {
      if (wVar7 != *pwVar11) {
        wVar7 = towlower(wVar7);
        local_420 = (wchar_t *)CONCAT62(local_420._2_6_,wVar7);
        wVar7 = towlower(*pwVar11);
        if ((wint_t)local_420 != wVar7) goto LAB_1416f917e;
      }
    }
    iVar16 = 0;
    if (pwVar19 != L"/Game/Art/UI_L10N/LANG/Xcmn_Header/PLAT_W/") {
      bVar21 = local_450._4_4_ != 0x2b;
      local_450 = (ulonglong)local_450._4_4_ << 0x20;
      if (bVar21) {
        FUN_1407f1af0(&local_458,0x2b);
        iVar16 = (int)local_450;
        pwVar19 = local_458;
      }
      iVar20 = iVar16 + 0x2b;
      local_450 = CONCAT44(local_450._4_4_,iVar20);
      if ((int)local_450._4_4_ < iVar20) {
        FUN_1407e2ee0(&local_458);
        pwVar19 = local_458;
        iVar20 = (uint)local_450;
      }
      uVar8 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._8_8_;
      *(undefined8 *)pwVar19 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._0_8_;
      *(undefined8 *)(pwVar19 + 4) = uVar8;
      uVar8 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._24_8_;
      *(undefined8 *)(pwVar19 + 8) = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._16_8_;
      *(undefined8 *)(pwVar19 + 0xc) = uVar8;
      uVar8 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._40_8_;
      *(undefined8 *)(pwVar19 + 0x10) = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._32_8_;
      *(undefined8 *)(pwVar19 + 0x14) = uVar8;
      uVar8 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._56_8_;
      *(undefined8 *)(pwVar19 + 0x18) = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._48_8_;
      *(undefined8 *)(pwVar19 + 0x1c) = uVar8;
      uVar4 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._76_4_;
      uVar3 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._72_4_;
      uVar2 = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._68_4_;
      *(undefined4 *)(pwVar19 + 0x20) = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._64_4_;
      *(undefined4 *)(pwVar19 + 0x22) = uVar2;
      *(undefined4 *)(pwVar19 + 0x24) = uVar3;
      *(undefined4 *)(pwVar19 + 0x26) = uVar4;
      *(undefined4 *)(pwVar19 + 0x28) = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250._80_4_;
      pwVar19[0x2a] = u__Game_Art_UI_L10N_LANG_Xcmn_Head_143f10250[0x2a];
    }
LAB_1416f917e:
    pwVar18 = (wchar_t *)0x0;
    iVar16 = 0;
    if (iVar20 < 2) {
      local_440 = (wchar_t *)0x0;
      local_438 = iVar13;
      if (iVar13 != 0) {
        FUN_1407f1420(&local_440,pwVar14,0);
        pwVar18 = local_440;
        memcpy(local_440,_Src,(longlong)iVar13 * 2);
        iVar13 = local_438;
        iVar16 = local_434;
      }
    }
    else {
      iVar10 = iVar13 + -1;
      if (iVar13 == 0) {
        iVar10 = iVar16;
      }
      if ((iVar20 != 0) || (local_468 = 1, iVar10 == 0)) {
        local_468 = iVar16;
      }
      local_468 = local_468 + iVar10;
      local_458 = (wchar_t *)0x0;
      FUN_1407e4b70(&local_458,pwVar19,iVar20,0);
      uVar5 = local_450;
      pwVar18 = local_458;
      if (iVar10 != 0) {
        iVar13 = (uint)local_450;
        lVar17 = (longlong)(int)(uint)local_450;
        iVar20 = ((uint)local_450 == 0) + (uint)local_450 + iVar10;
        local_450 = CONCAT44(local_450._4_4_,iVar20);
        if ((int)local_450._4_4_ < iVar20) {
          FUN_1407e2ee0(&local_458,uVar5 & 0xffffffff);
        }
        pwVar18 = local_458;
        pwVar14 = local_458 + (lVar17 - (ulonglong)(iVar13 != 0));
        memcpy(pwVar14,_Src,(longlong)iVar10 * 2);
        pwVar14[iVar10] = L'\0';
      }
      iVar13 = (uint)local_450;
      iVar16 = local_450._4_4_;
    }
    if (local_430 != (wchar_t *)0x0) {
      FUN_141de9200();
    }
    local_428 = (wchar_t *)CONCAT44(iVar16,iVar13);
    local_430 = pwVar18;
    uVar8 = FUN_1407e5680(&local_440,&local_430);
    ppwVar9 = (wchar_t **)FUN_141a690a0(local_410,uVar8,0);
    if (&local_430 != ppwVar9) {
      if (local_430 != (wchar_t *)0x0) {
        FUN_141de9200();
      }
      local_430 = *ppwVar9;
      *ppwVar9 = (wchar_t *)0x0;
      local_428 = ppwVar9[1];
      ppwVar9[1] = (wchar_t *)0x0;
    }
    if (local_410[0] != 0) {
      FUN_141de9200();
    }
    lVar17 = 0x460;
    if ((char)param_1[0x80] == '\0') {
      lVar17 = 0x408;
    }
    FUN_14159f740(lVar17 + (longlong)param_1,&local_430);
    if (pwVar19 != (wchar_t *)0x0) {
      FUN_141de9200(pwVar19);
    }
  }
  (**(code **)(*param_1 + 0x468))(param_1,0);
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) & 0xf7;
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | local_448 << 3;
  if (local_448 != '\0') {
    *(undefined2 *)((longlong)param_1 + 0x401) = 0x100;
    (**(code **)(*param_1 + 0x250))(param_1,3);
  }
  plVar15 = local_58 + 2;
  lVar17 = 0x3b;
  do {
    plVar1 = plVar15 + -2;
    plVar15 = plVar15 + -2;
    lVar17 = lVar17 + -1;
    if (*plVar1 != 0) {
      FUN_141de9200();
    }
  } while (lVar17 != 0);
  if (local_430 != (wchar_t *)0x0) {
    FUN_141de9200();
  }
  if (_Src != (wchar_t *)0x0) {
    FUN_141de9200(_Src);
  }
  return;
}


