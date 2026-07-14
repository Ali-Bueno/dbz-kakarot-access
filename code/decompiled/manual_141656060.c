// FUN_141656060 @ 141656060

void FUN_141656060(longlong param_1,undefined4 param_2,undefined8 *param_3)

{
  int *piVar1;
  undefined4 uVar2;
  void *_Src;
  char cVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  longlong lVar8;
  undefined8 *puVar9;
  longlong *plVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined8 local_res20;
  longlong local_7b8;
  undefined8 local_7b0;
  longlong local_7a8;
  longlong local_7a0;
  undefined1 *local_798;
  undefined8 local_790;
  longlong local_788;
  longlong local_780;
  longlong local_778 [36];
  void *local_658;
  int local_650;
  undefined4 local_64c;
  undefined8 local_648;
  undefined4 local_640;
  undefined4 local_63c;
  undefined8 local_638;
  undefined4 local_630;
  undefined4 local_62c;
  undefined8 local_628;
  undefined4 local_620;
  undefined4 local_61c;
  undefined8 local_618;
  undefined4 local_610;
  undefined4 local_60c;
  undefined8 local_608;
  undefined4 local_600;
  undefined4 local_5fc;
  longlong local_5f8;
  undefined4 local_5f0;
  undefined4 local_5ec;
  undefined8 local_5e8;
  undefined4 local_5e0;
  undefined4 local_5dc;
  void *local_5d8;
  ulonglong local_5d0;
  char local_5c8 [8];
  longlong local_5c0;
  undefined8 local_5b8;
  undefined8 local_5b0;
  undefined8 local_5a8;
  longlong local_5a0;
  undefined8 local_598;
  longlong local_590;
  undefined8 local_588;
  undefined8 local_580;
  longlong local_578;
  undefined8 local_570;
  undefined1 local_568 [8];
  undefined8 local_560;
  undefined8 local_558;
  undefined8 local_550;
  undefined8 local_548;
  undefined8 local_540;
  undefined8 local_538;
  undefined8 local_530;
  undefined8 local_528;
  undefined8 local_520;
  undefined8 local_518;
  undefined8 local_510;
  longlong local_508 [2];
  longlong local_4f8 [2];
  longlong local_4e8 [2];
  longlong local_4d8 [2];
  longlong local_4c8 [2];
  undefined1 local_4b8 [8];
  longlong *local_4b0;
  undefined1 local_4a0 [8];
  longlong *local_498;
  undefined **local_488;
  longlong local_480;
  longlong local_470;
  longlong local_460;
  longlong local_450;
  undefined **local_440;
  longlong local_438;
  longlong local_428;
  longlong local_418;
  longlong local_408;
  undefined **local_3f8;
  longlong local_3f0;
  longlong local_3e0;
  longlong local_3d0;
  longlong local_3c0;
  undefined **local_3b0;
  longlong local_3a8;
  longlong local_398;
  longlong local_388;
  longlong local_378;
  undefined **local_368;
  longlong local_360;
  longlong local_350;
  longlong local_340;
  longlong local_330;
  undefined **local_320;
  longlong local_318;
  longlong local_308;
  longlong local_2f8;
  longlong local_2e8;
  undefined **local_2d8;
  longlong local_2d0;
  longlong local_2c0;
  longlong local_2b0;
  longlong local_2a0;
  undefined **local_290;
  longlong local_288;
  longlong local_278;
  longlong local_268;
  longlong local_258;
  undefined **local_248;
  longlong local_240;
  longlong local_230;
  longlong local_220;
  longlong local_210;
  undefined **local_200;
  longlong local_1f8;
  longlong local_1e8;
  longlong local_1d8;
  longlong local_1c8;
  undefined **local_1b8;
  longlong local_1b0;
  longlong local_1a0;
  longlong local_190;
  longlong local_180;
  undefined **local_170;
  longlong local_168;
  longlong local_158;
  longlong local_148;
  longlong local_138;
  undefined **local_128;
  longlong local_120;
  longlong local_110;
  longlong local_100;
  longlong local_f0;
  undefined1 local_e0 [16];
  undefined1 local_d0 [16];
  undefined1 local_c0 [16];
  undefined1 local_b0 [16];
  undefined1 local_a0 [16];
  undefined1 local_90 [16];
  undefined1 local_80 [72];
  
  lVar5 = FUN_140d2aef0();
  if (lVar5 == 0) {
    return;
  }
  lVar5 = *(longlong *)(lVar5 + 0x130);
  if (lVar5 == 0) {
    return;
  }
  lVar6 = FUN_1411fd780();
  if (lVar6 == 0) {
    return;
  }
  lVar7 = FUN_14145e360(lVar5,*param_3);
  local_788 = 0;
  local_780 = 0;
  local_res20 = 0;
  if ((*(longlong *)(lVar7 + 0x90) != 0) && (lVar8 = FUN_140d2aa10(0), lVar8 != 0)) {
    cVar3 = FUN_140bd2e60(lVar7 + 0x48,"DLC_02_01002");
    if (cVar3 == '\0') {
      cVar3 = FUN_140bd2e60(lVar7 + 0x48,"DLC_01_01001_Cpl001");
      if ((cVar3 != '\0') ||
         (cVar3 = FUN_140bd2e60(lVar7 + 0x48,"DLC_01_01001_Cpl003"), cVar3 != '\0'))
      goto LAB_141656360;
    }
    else {
      puVar9 = (undefined8 *)FUN_141eb3490(&local_res20,L"DLC_02_01007",1);
      cVar3 = FUN_1413dd4c0(lVar8,*puVar9);
      if (cVar3 == '\0') {
LAB_141656360:
        cVar3 = FUN_1413dd4c0(lVar8,*(undefined8 *)(lVar7 + 0x48));
        if (cVar3 == '\0') {
          FUN_14166bf30(param_1,lVar6);
          local_778[3] = 0x12;
          local_778[2] = 0;
          FUN_1407e2ee0(local_778 + 2,0);
          FUN_1407e4a20(local_778[2],0x12,"MSG_Break_Through",0x12,0x3f);
          local_778[0] = 0;
          local_778[1] = 0xf;
          FUN_1407e2ee0(local_778,0);
          FUN_1407e4a20(local_778[0],0xf,"MSG_Conditions",0xf,0x3f);
          lVar5 = FUN_1411fd400(&local_290,local_778 + 2);
          local_648 = *(undefined8 *)(lVar5 + 0x38);
          *(undefined8 *)(lVar5 + 0x38) = 0;
          local_640 = *(undefined4 *)(lVar5 + 0x40);
          local_63c = *(undefined4 *)(lVar5 + 0x44);
          *(undefined8 *)(lVar5 + 0x40) = 0;
          lVar5 = FUN_1411fd400(&local_488,local_778);
          local_638 = *(undefined8 *)(lVar5 + 0x38);
          *(undefined8 *)(lVar5 + 0x38) = 0;
          local_630 = *(undefined4 *)(lVar5 + 0x40);
          local_62c = *(undefined4 *)(lVar5 + 0x44);
          *(undefined8 *)(lVar5 + 0x40) = 0;
          FUN_1416fded0(lVar6,&local_638,&local_648,1);
          if (local_450 != 0) {
            FUN_141de9200();
          }
          if (local_460 != 0) {
            FUN_141de9200();
          }
          if (local_470 != 0) {
            FUN_141de9200();
          }
          if (local_480 != 0) {
            FUN_141de9200();
          }
          local_488 = &PTR_FUN_14397c0d8;
          if (local_258 != 0) {
            FUN_141de9200();
          }
          if (local_268 != 0) {
            FUN_141de9200();
          }
          if (local_278 != 0) {
            FUN_141de9200();
          }
          if (local_288 != 0) {
            FUN_141de9200();
          }
          local_290 = &PTR_FUN_14397c0d8;
          lVar5 = local_778[2];
          if (local_778[0] != 0) {
            FUN_141de9200();
            lVar5 = local_778[2];
          }
          goto LAB_1416574e2;
        }
      }
    }
  }
  local_5c8[0] = FUN_14144efc0(lVar5,param_2,param_3);
  if (local_5c8[0] == '\x05') {
    FUN_14166bf30(param_1,lVar6);
    local_778[6] = 0;
    local_778[7] = 0x1e;
    FUN_1407e2ee0(local_778 + 6,0);
    FUN_1407e4a20(local_778[6],0x1e,"PassiveSkill_Slot_Release_004",0x1e,0x3f);
    local_778[4] = 0;
    local_778[5] = 0xf;
    FUN_1407e2ee0(local_778 + 4,0);
    FUN_1407e4a20(local_778[4],0xf,"MSG_Conditions",0xf,0x3f);
    lVar5 = FUN_1411fd400(&local_3f8,local_778 + 6);
    local_628 = *(undefined8 *)(lVar5 + 0x38);
    *(undefined8 *)(lVar5 + 0x38) = 0;
    local_620 = *(undefined4 *)(lVar5 + 0x40);
    local_61c = *(undefined4 *)(lVar5 + 0x44);
    *(undefined8 *)(lVar5 + 0x40) = 0;
    lVar5 = FUN_1411fd400(&local_440,local_778 + 4);
    local_618 = *(undefined8 *)(lVar5 + 0x38);
    *(undefined8 *)(lVar5 + 0x38) = 0;
    local_610 = *(undefined4 *)(lVar5 + 0x40);
    local_60c = *(undefined4 *)(lVar5 + 0x44);
    *(undefined8 *)(lVar5 + 0x40) = 0;
    FUN_1416fded0(lVar6,&local_618,&local_628,1);
    if (local_408 != 0) {
      FUN_141de9200();
    }
    if (local_418 != 0) {
      FUN_141de9200();
    }
    if (local_428 != 0) {
      FUN_141de9200();
    }
    if (local_438 != 0) {
      FUN_141de9200();
    }
    local_440 = &PTR_FUN_14397c0d8;
    if (local_3c0 != 0) {
      FUN_141de9200();
    }
    if (local_3d0 != 0) {
      FUN_141de9200();
    }
    if (local_3e0 != 0) {
      FUN_141de9200();
    }
    if (local_3f0 != 0) {
      FUN_141de9200();
    }
    local_3f8 = &PTR_FUN_14397c0d8;
    lVar5 = local_778[6];
    if (local_778[4] != 0) {
      FUN_141de9200();
      lVar5 = local_778[6];
    }
LAB_1416574e2:
    if (lVar5 != 0) {
      FUN_141de9200();
    }
    FUN_141485f00(3,DAT_1450d6c88,0,0);
    return;
  }
  if (local_5c8[0] != '\x01') {
    if (local_5c8[0] == '\x02') {
      local_798 = (undefined1 *)0x0;
      local_790 = 0;
      local_5c0 = 0;
      local_5b8 = 0;
      local_5b0 = 0;
      local_5a8 = 0;
      local_5a0 = 0;
      local_598 = 0;
      local_590 = 0;
      local_588 = 0;
      local_580 = 0;
      local_578 = 0;
      local_570 = 0;
      FUN_140fba270(&local_798,local_5c8,1);
      if (local_578 != 0) {
        FUN_141de9200();
      }
      if (local_590 != 0) {
        FUN_141de9200();
      }
      if (local_5a0 != 0) {
        FUN_141de9200();
      }
      FUN_140ca06c0(&local_5b0);
      if (local_5c0 != 0) {
        FUN_141de9200();
      }
      local_778[0x12] = 0;
      local_778[0x13] = 0x12;
      *local_798 = 3;
      *(undefined4 *)(local_798 + 0x48) = *(undefined4 *)(lVar7 + 0x58);
      FUN_1407e2ee0(local_778 + 0x12,0);
      FUN_1407e4a20(local_778[0x12],0x12,"MSG_Conditions_Lv",0x12,0x3f);
      lVar5 = FUN_1411fd400(&local_248,local_778 + 0x12);
      FUN_141215960(&local_5d8,lVar5 + 0x38,&local_798,0);
      if (local_210 != 0) {
        FUN_141de9200();
      }
      if (local_220 != 0) {
        FUN_141de9200();
      }
      if (local_230 != 0) {
        FUN_141de9200();
      }
      if (local_240 != 0) {
        FUN_141de9200();
      }
      local_248 = &PTR_FUN_14397c0d8;
      if (local_778[0x12] != 0) {
        FUN_141de9200();
      }
      FUN_14166bf30(param_1,lVar6);
      local_778[0x15] = 0xf;
      local_778[0x14] = 0;
      FUN_1407e2ee0(local_778 + 0x14,0);
      FUN_1407e4a20(local_778[0x14],0xf,"MSG_Conditions",0xf,0x3f);
      _Src = local_5d8;
      local_658 = (void *)0x0;
      iVar4 = (int)local_5d0;
      local_650 = iVar4;
      if (iVar4 == 0) {
        local_64c = 0;
      }
      else {
        FUN_1407f1420(&local_658,local_5d0 & 0xffffffff,0);
        memcpy(local_658,_Src,(longlong)iVar4 * 2);
      }
      lVar5 = FUN_1411fd400(&local_200,local_778 + 0x14);
      local_608 = *(undefined8 *)(lVar5 + 0x38);
      *(undefined8 *)(lVar5 + 0x38) = 0;
      local_600 = *(undefined4 *)(lVar5 + 0x40);
      local_5fc = *(undefined4 *)(lVar5 + 0x44);
      *(undefined8 *)(lVar5 + 0x40) = 0;
      FUN_1416fded0(lVar6,&local_608,&local_658,1);
      if (local_1c8 != 0) {
        FUN_141de9200();
      }
      if (local_1d8 != 0) {
        FUN_141de9200();
      }
      if (local_1e8 != 0) {
        FUN_141de9200();
      }
      if (local_1f8 != 0) {
        FUN_141de9200();
      }
      local_200 = &PTR_FUN_14397c0d8;
      if (local_778[0x14] != 0) {
        FUN_141de9200();
      }
      FUN_141485f00(3,DAT_1450d6c88,0,0);
      if (local_5d8 != (void *)0x0) {
        FUN_141de9200();
      }
      FUN_140ca06c0(&local_798);
      return;
    }
    if (local_5c8[0] == '\a') {
      local_778[0x16] = 0;
      local_778[0x17] = 0x16;
      FUN_1407e2ee0(local_778 + 0x16,0);
      FUN_1407e4a20(local_778[0x16],0x16,"Tutorial_Trk_Title_03",0x16,0x3f);
      lVar5 = FUN_1411fd400(&local_1b8,local_778 + 0x16);
      local_5f8 = *(longlong *)(lVar5 + 0x38);
      *(undefined8 *)(lVar5 + 0x38) = 0;
      local_5f0 = *(undefined4 *)(lVar5 + 0x40);
      local_5ec = *(undefined4 *)(lVar5 + 0x44);
      *(undefined8 *)(lVar5 + 0x40) = 0;
      if (local_180 != 0) {
        FUN_141de9200();
      }
      if (local_190 != 0) {
        FUN_141de9200();
      }
      if (local_1a0 != 0) {
        FUN_141de9200();
      }
      if (local_1b0 != 0) {
        FUN_141de9200();
      }
      local_1b8 = &PTR_FUN_14397c0d8;
      if (local_778[0x16] != 0) {
        FUN_141de9200();
      }
      FUN_14165a170(param_1,param_3,&local_5f8);
      if (local_5f8 == 0) {
        return;
      }
      FUN_141de9200();
      return;
    }
    if (1 < (byte)(local_5c8[0] - 3U)) {
      lVar5 = FUN_140d2c2a0();
      if ((lVar5 != 0) &&
         ((((iVar4 = FUN_141731110(lVar5,1), iVar4 < *(int *)(param_1 + 0x490) ||
            (iVar4 = FUN_141731110(lVar5,0), iVar4 < *(int *)(param_1 + 0x494))) ||
           ((iVar4 = FUN_141731110(lVar5,2), iVar4 < *(int *)(param_1 + 0x498) ||
            ((iVar4 = FUN_141731110(lVar5,3), iVar4 < *(int *)(param_1 + 0x49c) ||
             (iVar4 = FUN_141731110(lVar5,4), iVar4 < *(int *)(param_1 + 0x4a0))))))) ||
          (iVar4 = FUN_141731110(lVar5,5), iVar4 < *(int *)(param_1 + 0x4a4))))) {
        local_568[0] = 2;
        local_778[0x1c] = 0;
        local_778[0x1d] = 0;
        local_560 = 0;
        local_558 = 0;
        local_550 = 0;
        local_548 = 0;
        local_540 = 0;
        local_538 = 0;
        local_530 = 0;
        local_528 = 0;
        local_520 = 0;
        local_518 = 0;
        local_510 = 0;
        FUN_140fba270(local_778 + 0x1c,local_568,1);
        FUN_141134830(local_568);
        local_778[0x18] = 0;
        local_778[0x19] = 10;
        FUN_1407e2ee0(local_778 + 0x18,0);
        FUN_141dd1c10(local_778[0x18],L"unit_Zorb",0x14);
        FUN_141a83c70(local_778[0x1c],local_778 + 0x18);
        if (local_778[0x18] != 0) {
          FUN_141de9200();
        }
        local_778[0x1a] = 0;
        local_778[0x1b] = 0x11;
        FUN_1407e2ee0(local_778 + 0x1a,0);
        FUN_1407e4a20(local_778[0x1a],0x11,"MSG_reject_msg_1",0x11,0x3f);
        lVar5 = FUN_1411fd400(local_80,local_778 + 0x1a);
        plVar10 = (longlong *)FUN_141215960(local_4c8,lVar5 + 0x38,local_778 + 0x1c,0);
        local_788 = *plVar10;
        *plVar10 = 0;
        local_780 = plVar10[1];
        plVar10[1] = 0;
        if (local_4c8[0] != 0) {
          FUN_141de9200();
        }
        FUN_14125d1e0(local_80);
        if (local_778[0x1a] != 0) {
          FUN_141de9200();
        }
        FUN_14166bf30(param_1,lVar6);
        uVar11 = FUN_1407e5680(local_e0,&local_788);
        FUN_1416fcc70(lVar6,uVar11,1);
        FUN_141485f00(3,DAT_1450d6c88,0,0);
        FUN_140ca06c0(local_778 + 0x1c);
        if (local_788 == 0) {
          return;
        }
        FUN_141de9200();
        return;
      }
      local_7a8 = 0;
      local_7a0 = 0;
      uVar11 = FUN_14165b100(param_1,local_4f8,lVar7 + 0x20,1);
      plVar10 = (longlong *)FUN_1412b5c20(local_508,uVar11);
      if (&local_7a8 != plVar10) {
        local_7a8 = *plVar10;
        *plVar10 = 0;
        local_7a0 = plVar10[1];
        plVar10[1] = 0;
      }
      if (local_508[0] != 0) {
        FUN_141de9200();
      }
      if (local_4f8[0] != 0) {
        FUN_141de9200();
      }
      local_778[0x1e] = 0;
      local_778[0x1f] = 8;
      FUN_1407e2ee0(local_778 + 0x1e,0);
      FUN_1407e4a20(local_778[0x1e],8,"<var 1>",8,0x3f);
      uVar11 = FUN_141a69ad0(*(undefined8 *)(param_1 + 0x480),local_4b8);
      uVar11 = FUN_141e3adf0(uVar11);
      uVar11 = FUN_1407e5680(local_d0,uVar11);
      uVar12 = FUN_1407e5680(local_c0,&local_7a8);
      plVar10 = (longlong *)FUN_1416f3e80(local_4e8,uVar12,uVar11,local_778 + 0x1e);
      if (&local_7a8 != plVar10) {
        if (local_7a8 != 0) {
          FUN_141de9200();
        }
        local_7a8 = *plVar10;
        *plVar10 = 0;
        local_7a0 = plVar10[1];
        plVar10[1] = 0;
      }
      if (local_4e8[0] != 0) {
        FUN_141de9200();
      }
      if (local_4b0 != (longlong *)0x0) {
        LOCK();
        plVar10 = local_4b0 + 1;
        lVar5 = *plVar10;
        *(int *)plVar10 = (int)*plVar10 + -1;
        UNLOCK();
        if ((int)lVar5 == 1) {
          (**(code **)*local_4b0)(local_4b0);
          LOCK();
          piVar1 = (int *)((longlong)local_4b0 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*local_4b0 + 8))(local_4b0,1);
          }
        }
      }
      if (local_778[0x1e] != 0) {
        FUN_141de9200();
      }
      local_778[0x20] = 0;
      local_778[0x21] = 8;
      FUN_1407e2ee0(local_778 + 0x20,0);
      FUN_141dd1c10(local_778[0x20],L"<var 2>",0x10);
      uVar11 = FUN_141a69ad0(*(undefined8 *)(param_1 + 0x468),local_4a0);
      uVar11 = FUN_141e3adf0(uVar11);
      uVar11 = FUN_1407e5680(local_b0,uVar11);
      uVar12 = FUN_1407e5680(local_a0,&local_7a8);
      plVar10 = (longlong *)FUN_1416f3e80(local_4d8,uVar12,uVar11,local_778 + 0x20);
      if (&local_7a8 != plVar10) {
        if (local_7a8 != 0) {
          FUN_141de9200();
        }
        local_7a8 = *plVar10;
        *plVar10 = 0;
        local_7a0 = plVar10[1];
        plVar10[1] = 0;
      }
      if (local_4d8[0] != 0) {
        FUN_141de9200();
      }
      if (local_498 != (longlong *)0x0) {
        LOCK();
        plVar10 = local_498 + 1;
        lVar5 = *plVar10;
        *(int *)plVar10 = (int)*plVar10 + -1;
        UNLOCK();
        if ((int)lVar5 == 1) {
          (**(code **)*local_498)(local_498);
          LOCK();
          piVar1 = (int *)((longlong)local_498 + 0xc);
          iVar4 = *piVar1;
          *piVar1 = *piVar1 + -1;
          UNLOCK();
          if (iVar4 == 1) {
            (**(code **)(*local_498 + 8))(local_498,1);
          }
        }
      }
      if (local_778[0x20] != 0) {
        FUN_141de9200();
      }
      uVar11 = FUN_1407e5680(local_90,&local_7a8);
      FUN_1416fe020(lVar6,uVar11);
      FUN_1411484c0(*(undefined8 *)(param_1 + 0x3e0));
      uVar2 = DAT_1450d6c88;
      *(undefined4 *)(param_1 + 0x4b0) = param_2;
      *(undefined8 *)(param_1 + 0x4b8) = *param_3;
      *(undefined1 *)(param_1 + 0x4ad) = 1;
      *(undefined1 *)(*(longlong *)(param_1 + 0x3e0) + 0x1650) = 1;
      *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 8;
      *(undefined4 *)(param_1 + 0x4d0) = 0;
      FUN_141485f00(1,uVar2,0,0);
      if (local_7a8 == 0) {
        return;
      }
      FUN_141de9200();
      return;
    }
    FUN_14166bf30(param_1,lVar6);
    local_778[0x22] = 0;
    local_778[0x23] = 0x11;
    FUN_1407e2ee0(local_778 + 0x22,0);
    FUN_1407e4a20(local_778[0x22],0x11,"MSG_reject_msg_2",0x11,0x3f);
    lVar5 = FUN_1411fd400(&local_170,local_778 + 0x22);
    local_5e8 = *(undefined8 *)(lVar5 + 0x38);
    *(undefined8 *)(lVar5 + 0x38) = 0;
    local_5e0 = *(undefined4 *)(lVar5 + 0x40);
    local_5dc = *(undefined4 *)(lVar5 + 0x44);
    *(undefined8 *)(lVar5 + 0x40) = 0;
    FUN_1416fcc70(lVar6,&local_5e8,1);
    if (local_138 != 0) {
      FUN_141de9200();
    }
    if (local_148 != 0) {
      FUN_141de9200();
    }
    if (local_158 != 0) {
      FUN_141de9200();
    }
    if (local_168 != 0) {
      FUN_141de9200();
    }
    local_170 = &PTR_FUN_14397c0d8;
    lVar5 = local_778[0x22];
    goto LAB_1416574e2;
  }
  local_7b8 = 0;
  local_7b0 = 0;
  if (((*(longlong *)(lVar7 + 0x90) == DAT_145986928) && (lVar5 = FUN_140d2aa10(), lVar5 != 0)) &&
     (cVar3 = FUN_1413dd900(lVar5,lVar7 + 0x48,&DAT_145986928), cVar3 != '\0')) {
    local_778[8] = 0;
    local_778[9] = 0x15;
    FUN_1407e2ee0(local_778 + 8,0);
    FUN_1407e4a20(local_778[8],0x15,"ZCW_STORY_CATEGORY21",0x15,0x3f);
    lVar5 = FUN_1411fd400(&local_3b0,local_778 + 8);
    plVar10 = (longlong *)(lVar5 + 0x38);
    if (&local_7b8 != plVar10) {
      local_7b8 = *plVar10;
      *plVar10 = 0;
      local_7b0 = *(undefined8 *)(lVar5 + 0x40);
      *(undefined8 *)(lVar5 + 0x40) = 0;
    }
    if (local_378 != 0) {
      FUN_141de9200();
    }
    if (local_388 != 0) {
      FUN_141de9200();
    }
    if (local_398 != 0) {
      FUN_141de9200();
    }
    if (local_3a8 != 0) {
      FUN_141de9200();
    }
    local_3b0 = &PTR_FUN_14397c0d8;
    lVar5 = local_778[8];
  }
  else {
    iVar4 = FUN_1412dcb10(lVar7 + 0x48);
    if (iVar4 == 1) {
      local_778[0x10] = 0;
      local_778[0x11] = 0x16;
      FUN_1407e2ee0(local_778 + 0x10,0);
      FUN_1407e4a20(local_778[0x10],0x16,"ZCW_DIAGRAM_CATEGORY1",0x16,0x3f);
      lVar5 = FUN_1411fd400(&local_128,local_778 + 0x10);
      plVar10 = (longlong *)(lVar5 + 0x38);
      if (&local_7b8 != plVar10) {
        local_7b8 = *plVar10;
        *plVar10 = 0;
        local_7b0 = *(undefined8 *)(lVar5 + 0x40);
        *(undefined8 *)(lVar5 + 0x40) = 0;
      }
      if (local_f0 != 0) {
        FUN_141de9200();
      }
      if (local_100 != 0) {
        FUN_141de9200();
      }
      if (local_110 != 0) {
        FUN_141de9200();
      }
      if (local_120 != 0) {
        FUN_141de9200();
      }
      local_128 = &PTR_FUN_14397c0d8;
      lVar5 = local_778[0x10];
    }
    else if (iVar4 == 2) {
      local_778[0xe] = 0;
      local_778[0xf] = 0x16;
      FUN_1407e2ee0(local_778 + 0xe,0);
      FUN_1407e4a20(local_778[0xe],0x16,"ZCW_DIAGRAM_CATEGORY2",0x16,0x3f);
      lVar5 = FUN_1411fd400(&local_2d8,local_778 + 0xe);
      plVar10 = (longlong *)(lVar5 + 0x38);
      if (&local_7b8 != plVar10) {
        local_7b8 = *plVar10;
        *plVar10 = 0;
        local_7b0 = *(undefined8 *)(lVar5 + 0x40);
        *(undefined8 *)(lVar5 + 0x40) = 0;
      }
      if (local_2a0 != 0) {
        FUN_141de9200();
      }
      if (local_2b0 != 0) {
        FUN_141de9200();
      }
      if (local_2c0 != 0) {
        FUN_141de9200();
      }
      if (local_2d0 != 0) {
        FUN_141de9200();
      }
      local_2d8 = &PTR_FUN_14397c0d8;
      lVar5 = local_778[0xe];
    }
    else if (iVar4 == 3) {
      local_778[0xc] = 0;
      local_778[0xd] = 0x16;
      FUN_1407e2ee0(local_778 + 0xc,0);
      FUN_1407e4a20(local_778[0xc],0x16,"ZCW_DIAGRAM_CATEGORY3",0x16,0x3f);
      lVar5 = FUN_1411fd400(&local_320,local_778 + 0xc);
      plVar10 = (longlong *)(lVar5 + 0x38);
      if (&local_7b8 != plVar10) {
        local_7b8 = *plVar10;
        *plVar10 = 0;
        local_7b0 = *(undefined8 *)(lVar5 + 0x40);
        *(undefined8 *)(lVar5 + 0x40) = 0;
      }
      if (local_2e8 != 0) {
        FUN_141de9200();
      }
      if (local_2f8 != 0) {
        FUN_141de9200();
      }
      if (local_308 != 0) {
        FUN_141de9200();
      }
      if (local_318 != 0) {
        FUN_141de9200();
      }
      local_320 = &PTR_FUN_14397c0d8;
      lVar5 = local_778[0xc];
    }
    else {
      if (iVar4 != 4) goto LAB_1416569ea;
      local_778[10] = 0;
      local_778[0xb] = 0x16;
      FUN_1407e2ee0(local_778 + 10,0);
      FUN_1407e4a20(local_778[10],0x16,"ZCW_DIAGRAM_CATEGORY4",0x16,0x3f);
      lVar5 = FUN_1411fd400(&local_368,local_778 + 10);
      plVar10 = (longlong *)(lVar5 + 0x38);
      if (&local_7b8 != plVar10) {
        local_7b8 = *plVar10;
        *plVar10 = 0;
        local_7b0 = *(undefined8 *)(lVar5 + 0x40);
        *(undefined8 *)(lVar5 + 0x40) = 0;
      }
      if (local_330 != 0) {
        FUN_141de9200();
      }
      if (local_340 != 0) {
        FUN_141de9200();
      }
      if (local_350 != 0) {
        FUN_141de9200();
      }
      if (local_360 != 0) {
        FUN_141de9200();
      }
      local_368 = &PTR_FUN_14397c0d8;
      lVar5 = local_778[10];
    }
  }
  if (lVar5 != 0) {
    FUN_141de9200();
  }
LAB_1416569ea:
  FUN_14165a170(param_1,param_3,&local_7b8);
  if (local_7b8 == 0) {
    return;
  }
  FUN_141de9200();
  return;
}


