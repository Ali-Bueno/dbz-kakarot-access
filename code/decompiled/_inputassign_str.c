// ================= Controller_Btn_B <- 1410520c9  FUN_141051f50 @ 141051f50  size=0x282 =================
// callers: <none>

void FUN_141051f50(longlong param_1)

{
  ulonglong uVar1;
  char cVar2;
  int iVar3;
  longlong lVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined2 *puVar7;
  longlong lVar8;
  undefined8 local_res8;
  undefined1 local_res10 [8];
  longlong local_98;
  undefined8 local_90;
  undefined1 local_88 [8];
  undefined1 local_80 [8];
  longlong local_78;
  undefined8 local_70;
  undefined1 *local_68;
  undefined8 local_60;
  undefined1 local_58 [64];
  
  lVar4 = FUN_140d24f50();
  if ((lVar4 != 0) && (lVar4 = *(longlong *)(lVar4 + 0x158), lVar4 != 0)) {
    if (*(int *)(param_1 + 0x38) == 0) {
      puVar7 = &DAT_14391f7d0;
    }
    else {
      puVar7 = *(undefined2 **)(param_1 + 0x30);
    }
    FUN_141eb3490(&local_res8,puVar7,1);
    puVar5 = (undefined8 *)FUN_141044e00(lVar4 + 0x88,local_res10,param_1,FUN_14105ca90);
    *(undefined8 *)(param_1 + 0x158) = *puVar5;
    puVar5 = (undefined8 *)FUN_141044e00(lVar4 + 0xa0,local_res10,param_1,FUN_14105c8d0);
    *(undefined8 *)(param_1 + 0x160) = *puVar5;
    uVar6 = FUN_142058d50(param_1 + 0x118);
    FUN_1420575e0(lVar4 + 0x1b8,uVar6);
    uVar6 = FUN_142058d50(param_1 + 0x120);
    FUN_1420575e0(lVar4 + 0x1c0,uVar6);
    FUN_140d66620(lVar4,local_res8);
  }
  lVar4 = FUN_1414a4460();
  if ((lVar4 != 0) && (lVar4 = *(longlong *)(lVar4 + 0x2a8), lVar4 != 0)) {
    local_98 = 0;
    local_90 = 0;
    FUN_141eb3490(local_88,&DAT_14391f7d0,1);
    FUN_141eb3490(local_80,&DAT_14391f7d0,1);
    local_78 = 0;
    local_70 = 0;
    local_68 = (undefined1 *)0x0;
    local_60 = 1;
    FUN_140827170(&local_68,0);
    uVar1 = local_90;
    *local_68 = 0x1a;
    lVar8 = (longlong)(int)local_90;
    iVar3 = (int)local_90 + 1;
    local_90 = CONCAT44(local_90._4_4_,iVar3);
    if (local_90._4_4_ < iVar3) {
      FUN_140827170(&local_98,uVar1 & 0xffffffff);
    }
    *(undefined1 *)(lVar8 + local_98) = 0;
    FUN_141eb3490(&local_res8,L"Controller_Btn_B",1);
    uVar1 = local_70;
    lVar8 = (longlong)(int)local_70;
    iVar3 = (int)local_70 + 1;
    local_70 = CONCAT44(local_70._4_4_,iVar3);
    if (local_70._4_4_ < iVar3) {
      FUN_1407f1510(&local_78,uVar1 & 0xffffffff);
    }
    *(undefined8 *)(local_78 + lVar8 * 8) = local_res8;
    uVar6 = FUN_141045e20(local_58,&local_98);
    FUN_141619330(lVar4,uVar6);
    if (local_68 != (undefined1 *)0x0) {
      FUN_141de9200();
    }
    if (local_78 != 0) {
      FUN_141de9200();
    }
    if (local_98 != 0) {
      FUN_141de9200();
    }
  }
  FUN_14105ecf0(param_1,0);
  lVar4 = FUN_142058d50(param_1 + 0x120);
  lVar4 = FUN_140f87f00(*(undefined8 *)(lVar4 + 0x8e0));
  if (lVar4 != 0) {
    uVar6 = FUN_142058d50(param_1 + 0x120);
    cVar2 = FUN_140f10680(uVar6);
    if (cVar2 != '\0') {
      *(float *)(lVar4 + 0x378) = *(float *)(lVar4 + 0x37c) + *(float *)(lVar4 + 0x378);
      lVar4 = FUN_140d282e0();
      if ((lVar4 != 0) && (*(longlong *)(lVar4 + 0x570) != 0)) {
        FUN_1414a2c60();
      }
    }
  }
  return;
}



// ================= Controller_Btn_B <- 1411dda37  FUN_1411dba70 @ 1411dba70  size=0x3d02 =================
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1411dba70(code *param_1)

{
  longlong lVar1;
  bool bVar2;
  char cVar3;
  longlong lVar4;
  undefined8 *puVar5;
  longlong *plVar6;
  longlong lVar7;
  byte *pbVar8;
  undefined8 *puVar9;
  undefined8 *puVar10;
  wchar_t *pwVar11;
  undefined1 auStack_7e8 [32];
  code **local_7c8;
  code *local_7b8;
  longlong local_7b0;
  undefined1 local_7a8 [8];
  undefined1 local_7a0 [8];
  code *local_798;
  undefined4 uStack_790;
  undefined4 uStack_78c;
  longlong local_788;
  undefined1 local_778 [16];
  undefined8 local_768;
  undefined8 uStack_760;
  undefined4 local_758;
  uint uStack_754;
  undefined1 local_748 [8];
  undefined1 local_740 [8];
  undefined1 local_738 [8];
  undefined1 local_730 [8];
  undefined1 local_728 [8];
  undefined1 local_720 [8];
  undefined1 local_718 [8];
  undefined1 local_710 [8];
  undefined1 local_708 [8];
  undefined1 local_700 [8];
  undefined1 local_6f8 [8];
  undefined1 local_6f0 [8];
  undefined1 local_6e8 [8];
  undefined1 local_6e0 [8];
  undefined1 local_6d8 [8];
  undefined1 local_6d0 [8];
  code *local_6c8 [2];
  longlong local_6b8 [4];
  longlong *local_698;
  byte local_688;
  byte local_687;
  undefined1 local_686;
  undefined8 local_680;
  longlong *local_678;
  int local_670;
  undefined8 local_668;
  undefined4 local_660;
  undefined1 local_658 [8];
  undefined8 local_650;
  undefined1 local_648;
  undefined1 local_638 [64];
  undefined1 local_5f8 [64];
  undefined1 local_5b8 [64];
  undefined1 local_578 [64];
  undefined1 local_538 [64];
  undefined1 local_4f8 [64];
  undefined1 local_4b8 [64];
  undefined1 local_478 [64];
  undefined1 local_438 [64];
  undefined1 local_3f8 [64];
  undefined1 local_3b8 [64];
  undefined1 local_378 [64];
  undefined1 local_338 [64];
  undefined1 local_2f8 [64];
  undefined1 local_2b8 [64];
  undefined1 local_278 [64];
  undefined1 local_238 [64];
  undefined1 local_1f8 [64];
  undefined1 local_1b8 [64];
  undefined1 local_178 [64];
  undefined1 local_138 [64];
  undefined1 local_f8 [64];
  undefined1 local_b8 [64];
  undefined1 local_78 [64];
  ulonglong local_38;
  
  local_38 = DAT_145909780 ^ (ulonglong)auStack_7e8;
  FUN_141201810();
  lVar1 = *(longlong *)(param_1 + 0xf0);
  if (lVar1 != 0) {
    lVar4 = FUN_141863300();
    if ((*(int *)(*(longlong *)(lVar1 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar1 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
        != lVar4 + 0x88)) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    if ((bVar2) && (lVar4 = *(longlong *)(param_1 + 0x238), lVar4 != 0)) {
      *(undefined8 *)(param_1 + 0x248) = *(undefined8 *)(lVar4 + 0x1188);
      puVar10 = (undefined8 *)0x0;
      *(undefined4 *)(param_1 + 0x270) = 0;
      if (*(int *)(param_1 + 0x274) != 0) {
        FUN_140827be0(param_1 + 0x268,0);
      }
      *(undefined4 *)(param_1 + 0x280) = 0;
      if (*(int *)(param_1 + 0x284) != 0) {
        FUN_140827be0(param_1 + 0x278,0);
      }
      local_688 = local_688 & 0xfc;
      local_687 = local_687 & 0xfe;
      local_768 = FUN_1412c2bf0;
      uStack_760._0_4_ = 0;
      local_686 = 0;
      puVar5 = (undefined8 *)FUN_141eb3410(&local_7b0,"PauseMenu",1);
      local_678 = (longlong *)0x0;
      local_670 = 0;
      local_668 = 0;
      local_680 = *puVar5;
      local_660 = 0;
      FUN_1420575e0(local_658,0);
      local_650 = 0;
      local_648 = 0;
      FUN_1411ef520(&local_678);
      local_648 = 1;
      plVar6 = (longlong *)FUN_141df5850(0,0x30,0);
      if (plVar6 == (longlong *)0x0) {
        if ((local_670 != 0) && (local_678 != (longlong *)0x0)) {
          (**(code **)(*local_678 + 0x30))(local_678,0);
          if (local_678 != (longlong *)0x0) {
            local_678 = (longlong *)FUN_141df5850(local_678,0,0);
          }
          local_670 = 0;
        }
      }
      else {
        *plVar6 = (longlong)&PTR_FUN_143968d20;
        FUN_1420575e0(plVar6 + 1,lVar1);
        *(undefined4 *)(plVar6 + 2) = (undefined4)local_768;
        *(undefined4 *)((longlong)plVar6 + 0x14) = local_768._4_4_;
        *(undefined4 *)(plVar6 + 3) = (undefined4)uStack_760;
        *(uint *)((longlong)plVar6 + 0x1c) = uStack_760._4_4_;
        lVar7 = FUN_141db8700();
        plVar6[5] = lVar7;
        *plVar6 = (longlong)&PTR_FUN_143968d70;
        (*(code *)PTR_FUN_143968da8)(plVar6,&local_678);
        (**(code **)(*plVar6 + 0x30))(plVar6,0);
        lVar7 = FUN_141df5850(plVar6,0,0);
        if (lVar7 != 0) {
          FUN_141de9200(lVar7);
        }
      }
      pbVar8 = (byte *)FUN_142c367c0(param_1,&local_688);
      FUN_1407e59d0(&local_668);
      FUN_1407e59d0(&local_678);
      *pbVar8 = *pbVar8 | 2;
      FUN_141eb3490(local_6f8,L"Controller_Btn_Start",1);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = lVar1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d15b0;
      }
      FUN_141eb3490(local_778,L"PauseMenu",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_6f8,local_778,0);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      uStack_760 = 0;
      local_768 = FUN_140f4b390;
      local_758 = 0;
      local_788 = (ulonglong)uStack_754 << 0x20;
      local_798 = FUN_140f4b390;
      uStack_790 = 0;
      uStack_78c = 0;
      puVar5 = (undefined8 *)FUN_141eb3410(&local_7b0,"InputRightStickXAxis",1);
      FUN_1411c8650(param_1,*puVar5,lVar4,&local_798);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1340;
      }
      FUN_141eb3490(local_778,L"Battle_TargetChange_Left",1);
      FUN_1412158c0(param_1,local_778,0,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d18f0;
      }
      FUN_141eb3490(local_778,L"Battle_TargetChange_Right",1);
      FUN_1412158c0(param_1,local_778,0,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      FUN_1411d25d0(param_1);
      FUN_141eb3490(local_740,L"Controller_Btn_R3",1);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1cc0;
      }
      FUN_141eb3490(local_778,L"LockOn",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_740,local_778,0);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1cc0;
      }
      FUN_141eb3490(local_778,L"Battle_TargetCancel",1);
      FUN_1412158c0(param_1,local_778,0,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      bVar2 = false;
      if ((*(longlong *)(param_1 + 0x238) != 0) && (cVar3 = FUN_140f0a040(), cVar3 == '\x02')) {
        bVar2 = true;
      }
      local_768 = (code *)&LAB_1411e4df0;
      uStack_760._0_4_ = 0;
      puVar5 = (undefined8 *)FUN_141eb3410(&local_798,"MoveRight",1);
      FUN_140b1fbe0(param_1,*puVar5,param_1,&local_768);
      local_768 = (code *)&LAB_1411e4e10;
      uStack_760 = (ulonglong)uStack_760._4_4_ << 0x20;
      puVar5 = (undefined8 *)FUN_141eb3410(&local_798,"MoveForward",1);
      FUN_140b1fbe0(param_1,*puVar5,param_1,&local_768);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d1280;
      }
      FUN_141eb3490(&local_7b8,L"Move_Up",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d1280;
      }
      FUN_141eb3490(&local_7b8,L"Move_Up",1);
      FUN_1412158c0(param_1,&local_7b8,2,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d1560;
      }
      FUN_141eb3490(&local_7b8,L"Move_Up",1);
      FUN_1412158c0(param_1,&local_7b8,1,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d17c0;
      }
      FUN_141eb3490(&local_7b8,L"Move_Left",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d17c0;
      }
      FUN_141eb3490(&local_7b8,L"Move_Left",1);
      FUN_1412158c0(param_1,&local_7b8,2,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d1040;
      }
      FUN_141eb3490(&local_7b8,L"Move_Left",1);
      FUN_1412158c0(param_1,&local_7b8,1,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d0f00;
      }
      FUN_141eb3490(&local_7b8,L"Move_Down",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d0f00;
      }
      FUN_141eb3490(&local_7b8,L"Move_Down",1);
      FUN_1412158c0(param_1,&local_7b8,2,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d0dd0;
      }
      FUN_141eb3490(&local_7b8,L"Move_Down",1);
      FUN_1412158c0(param_1,&local_7b8,1,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d1640;
      }
      FUN_141eb3490(&local_7b8,L"Move_Right",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d1640;
      }
      FUN_141eb3490(&local_7b8,L"Move_Right",1);
      FUN_1412158c0(param_1,&local_7b8,2,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d16e0;
      }
      FUN_141eb3490(&local_7b8,L"Move_Right",1);
      FUN_1412158c0(param_1,&local_7b8,1,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      FUN_141eb3490(local_748,L"Controller_Btn_RT",1);
      local_698 = (longlong *)0x0;
      puVar5 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      if (bVar2) {
        puVar9 = puVar10;
        if (puVar5 != (undefined8 *)0x0) {
          puVar5[1] = param_1;
          *puVar5 = &PTR_FUN_143969680;
          puVar9 = puVar5;
        }
        if (puVar9 != (undefined8 *)0xfffffffffffffff8) {
          local_6c8[0] = (code *)&LAB_1411d0b30;
        }
        FUN_141eb3490(&local_7b8,L"VerticalMove_Pressed",1);
        local_7c8 = local_6c8;
        FUN_141215910(param_1,local_748,&local_7b8,0);
        if (local_6c8[0] != (code *)0x0) {
          plVar6 = local_6b8;
          if (local_698 != (longlong *)0x0) {
            plVar6 = local_698;
          }
          (**(code **)(*plVar6 + 0x10))(plVar6,0);
        }
        if (local_698 != (longlong *)0x0) {
          FUN_141de9200();
        }
        local_698 = (longlong *)0x0;
        puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
        puVar5 = puVar10;
        if (puVar9 != (undefined8 *)0x0) {
          puVar9[1] = param_1;
          *puVar9 = &PTR_FUN_143969680;
          puVar5 = puVar9;
        }
        if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
          local_6c8[0] = (code *)&LAB_1411d0d30;
        }
        FUN_141eb3490(&local_7b8,L"VerticalMove_Peleased",1);
        local_7c8 = local_6c8;
        FUN_141215910(param_1,local_748,&local_7b8,1);
        if (local_6c8[0] != (code *)0x0) {
          plVar6 = local_6b8;
          if (local_698 != (longlong *)0x0) {
            plVar6 = local_698;
          }
          (**(code **)(*plVar6 + 0x10))(plVar6,0);
        }
        if (local_698 != (longlong *)0x0) {
          FUN_141de9200();
        }
        local_698 = (longlong *)0x0;
        puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
        puVar5 = puVar10;
        if (puVar9 != (undefined8 *)0x0) {
          puVar9[1] = param_1;
          *puVar9 = &PTR_FUN_143969680;
          puVar5 = puVar9;
        }
        if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
          local_6c8[0] = (code *)&LAB_1411d0b30;
        }
        FUN_141eb3490(&local_7b8,L"Battle_MoveMode",1);
        FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
        if (local_6c8[0] != (code *)0x0) {
          plVar6 = local_6b8;
          if (local_698 != (longlong *)0x0) {
            plVar6 = local_698;
          }
          (**(code **)(*plVar6 + 0x10))(plVar6,0);
        }
        if (local_698 != (longlong *)0x0) {
          FUN_141de9200();
        }
        local_698 = (longlong *)0x0;
        puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
        puVar5 = puVar10;
        if (puVar9 != (undefined8 *)0x0) {
          puVar9[1] = param_1;
          *puVar9 = &PTR_FUN_143969680;
          puVar5 = puVar9;
        }
        if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
          local_6c8[0] = (code *)&LAB_1411d0d30;
        }
        pwVar11 = L"Battle_MoveMode";
      }
      else {
        puVar9 = puVar10;
        if (puVar5 != (undefined8 *)0x0) {
          puVar5[1] = param_1;
          *puVar5 = &PTR_FUN_143969680;
          puVar9 = puVar5;
        }
        if (puVar9 != (undefined8 *)0xfffffffffffffff8) {
          local_6c8[0] = (code *)&LAB_1411d10a0;
        }
        FUN_141eb3490(&local_7b8,L"VerticalMove_Pressed",1);
        local_7c8 = local_6c8;
        FUN_141215910(param_1,local_748,&local_7b8,0);
        if (local_6c8[0] != (code *)0x0) {
          plVar6 = local_6b8;
          if (local_698 != (longlong *)0x0) {
            plVar6 = local_698;
          }
          (**(code **)(*plVar6 + 0x10))(plVar6,0);
        }
        if (local_698 != (longlong *)0x0) {
          FUN_141de9200();
        }
        local_698 = (longlong *)0x0;
        puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
        puVar5 = puVar10;
        if (puVar9 != (undefined8 *)0x0) {
          puVar9[1] = param_1;
          *puVar9 = &PTR_FUN_143969680;
          puVar5 = puVar9;
        }
        if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
          local_6c8[0] = (code *)&LAB_1411d0d30;
        }
        FUN_141eb3490(&local_7b8,L"VerticalMove_Peleased",1);
        local_7c8 = local_6c8;
        FUN_141215910(param_1,local_748,&local_7b8,1);
        if (local_6c8[0] != (code *)0x0) {
          plVar6 = local_6b8;
          if (local_698 != (longlong *)0x0) {
            plVar6 = local_698;
          }
          (**(code **)(*plVar6 + 0x10))(plVar6,0);
        }
        if (local_698 != (longlong *)0x0) {
          FUN_141de9200();
        }
        local_698 = (longlong *)0x0;
        puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
        puVar5 = puVar10;
        if (puVar9 != (undefined8 *)0x0) {
          puVar9[1] = param_1;
          *puVar9 = &PTR_FUN_143969680;
          puVar5 = puVar9;
        }
        if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
          local_6c8[0] = (code *)&LAB_1411d10a0;
        }
        FUN_141eb3490(&local_7b8,L"Battle_MoveMode",1);
        FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
        if (local_6c8[0] != (code *)0x0) {
          plVar6 = local_6b8;
          if (local_698 != (longlong *)0x0) {
            plVar6 = local_698;
          }
          (**(code **)(*plVar6 + 0x10))(plVar6,0);
        }
        if (local_698 != (longlong *)0x0) {
          FUN_141de9200();
        }
        local_698 = (longlong *)0x0;
        puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
        puVar5 = puVar10;
        if (puVar9 != (undefined8 *)0x0) {
          puVar9[1] = param_1;
          *puVar9 = &PTR_FUN_143969680;
          puVar5 = puVar9;
        }
        if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
          local_6c8[0] = (code *)&LAB_1411d0d30;
        }
        pwVar11 = L"Battle_MoveMode";
      }
      FUN_141eb3490(&local_7b8,pwVar11,1);
      FUN_1412158c0(param_1,&local_7b8,1,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      FUN_141eb3490(local_738,L"Controller_Btn_LB",1);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1500;
      }
      FUN_141eb3490(&local_7b8,L"CommandPaletteSkill_Pressed",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_738,&local_7b8,0);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d18a0;
      }
      FUN_141eb3490(&local_7b8,L"CommandPaletteSkill_Repeat",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_738,&local_7b8,2);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1b10;
      }
      FUN_141eb3490(&local_7b8,L"CommandPaletteSkill_Released",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_738,&local_7b8,1);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1500;
      }
      FUN_141eb3490(&local_7b8,L"Battle_ActiveSkill_Palette",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d18a0;
      }
      FUN_141eb3490(&local_7b8,L"Battle_ActiveSkill_Palette",1);
      FUN_1412158c0(param_1,&local_7b8,2,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1b10;
      }
      FUN_141eb3490(&local_7b8,L"Battle_ActiveSkill_Palette",1);
      FUN_1412158c0(param_1,&local_7b8,1,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      FUN_141eb3490(local_730,L"Controller_Btn_RB",1);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1210;
      }
      FUN_141eb3490(&local_7b8,L"CommandPaletteSupport_Pressed",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_730,&local_7b8,0);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1210;
      }
      FUN_141eb3490(&local_7b8,L"CommandPaletteSupport_Repeat",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_730,&local_7b8,2);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d0e00;
      }
      FUN_141eb3490(&local_7b8,L"CommandPaletteSupport_Released",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_730,&local_7b8,1);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1210;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Support_Palette",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1210;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Support_Palette",1);
      FUN_1412158c0(param_1,&local_7b8,2,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d0e00;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Support_Palette",1);
      FUN_1412158c0(param_1,&local_7b8,1,local_6c8);
      if (local_6c8[0] != (code *)0x0) {
        plVar6 = local_6b8;
        if (local_698 != (longlong *)0x0) {
          plVar6 = local_698;
        }
        (**(code **)(*plVar6 + 0x10))(plVar6,0);
      }
      if (local_698 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_768 = (code *)&LAB_1411e4800;
      uStack_760._0_4_ = 0;
      puVar5 = (undefined8 *)FUN_141eb3410(&local_798,"CommandPaletteItem",1);
      local_7c8 = (code **)&local_768;
      FUN_1411c84e0(param_1,*puVar5,0,param_1);
      local_768 = (code *)&LAB_1411e4800;
      uStack_760._0_4_ = 0;
      puVar5 = (undefined8 *)FUN_141eb3410(&local_798,"CommandPaletteItem",1);
      local_7c8 = (code **)&local_768;
      FUN_1411c84e0(param_1,*puVar5,2,param_1);
      local_768 = (code *)&LAB_1411e47e0;
      uStack_760 = (ulonglong)uStack_760._4_4_ << 0x20;
      puVar5 = (undefined8 *)FUN_141eb3410(&local_798,"CommandPaletteItem",1);
      local_7c8 = (code **)&local_768;
      FUN_1411c84e0(param_1,*puVar5,1,param_1);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1390;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Item_Palette",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1390;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Item_Palette",1);
      FUN_1412158c0(param_1,&local_7b8,2,local_6c8);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1960;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Item_Palette",1);
      FUN_1412158c0(param_1,&local_7b8,1,local_6c8);
      FUN_1408f46c0(local_6c8);
      FUN_141eb3490(local_728,L"Controller_Btn_LT",1);
      FUN_141eb3490(local_740,L"Controller_Btn_RT",1);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1070;
      }
      FUN_141eb3490(&local_7b8,L"Battle_FormChange_Palette1",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_728,&local_7b8,0);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d0ae0;
      }
      FUN_141eb3490(&local_7b8,L"Battle_FormChange_Palette1",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_728,&local_7b8,2);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d1bf0;
      }
      FUN_141eb3490(&local_7b8,L"Battle_FormChange_Palette1",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_728,&local_7b8,1);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d15d0;
      }
      FUN_141eb3490(&local_7b8,L"Battle_FormChange_Palette2",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_740,&local_7b8,0);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d1990;
      }
      FUN_141eb3490(&local_7b8,L"Battle_FormChange_Palette2",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_740,&local_7b8,1);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d11b0;
      }
      FUN_141eb3490(&local_7b8,L"Battle_FormChange_Palette",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d0ae0;
      }
      FUN_141eb3490(&local_7b8,L"Battle_FormChange_Palette",1);
      FUN_1412158c0(param_1,&local_7b8,2,local_6c8);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d1420;
      }
      FUN_141eb3490(&local_7b8,L"Battle_FormChange_Palette",1);
      FUN_1412158c0(param_1,&local_7b8,1,local_6c8);
      FUN_1408f46c0(local_6c8);
      FUN_141eb3490(local_6f0,L"Controller_Btn_Y",1);
      FUN_141eb3490(local_6e8,L"Controller_Btn_X",1);
      FUN_141eb3490(local_6e0,L"Controller_Btn_B",1);
      FUN_141eb3490(local_6d8,L"Controller_Btn_A",1);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d14a0;
      }
      FUN_141eb3490(&local_7b8,L"CommandPaletteUp",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_6f0,&local_7b8,0);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1540;
      }
      FUN_141eb3490(&local_7b8,L"CommandPaletteLeft",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_6e8,&local_7b8,0);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d0ee0;
      }
      FUN_141eb3490(&local_7b8,L"CommandPaletteRight",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_6e0,&local_7b8,0);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1710;
      }
      FUN_141eb3490(&local_7b8,L"CommandPaletteDown",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_6d8,&local_7b8,0);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d14a0;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Palette_Up",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1540;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Palette_Left",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d0ee0;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Palette_Right",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1710;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Palette_Down",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      FUN_1408f46c0(local_6c8);
      FUN_141eb3490(local_720,L"Controller_Btn_LT",1);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d0f80;
      }
      FUN_141eb3490(&local_7b8,L"Guard_Pressed",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_720,&local_7b8,0);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d0e30;
      }
      FUN_141eb3490(&local_7b8,L"Guard_Repeat",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_720,&local_7b8,2);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d0a10;
      }
      FUN_141eb3490(&local_7b8,L"Guard_Released",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_720,&local_7b8,1);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d0f80;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Guard",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d0e30;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Guard",1);
      FUN_1412158c0(param_1,&local_7b8,2,local_6c8);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d0a10;
      }
      FUN_141eb3490(&local_7b8,L"Battle_Guard",1);
      FUN_1412158c0(param_1,&local_7b8,1,local_6c8);
      FUN_1408f46c0(local_6c8);
      FUN_141eb3490(local_718,L"Controller_Btn_X",1);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d0a00;
      }
      FUN_141eb3490(&local_7b8,L"Battle_KiAtk_Pressed",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_718,&local_7b8,0);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d0cb0;
      }
      FUN_141eb3490(&local_7b8,L"Battle_KiAtk_Repeat",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_718,&local_7b8,2);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d0ac0;
      }
      FUN_141eb3490(&local_7b8,L"Battle_KiAtk_Released",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_718,&local_7b8,1);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d0a00;
      }
      FUN_141eb3490(&local_7b8,L"Battle_KiAtk",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_6c8);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = FUN_1411d0cb0;
      }
      FUN_141eb3490(&local_7b8,L"Battle_KiAtk",1);
      FUN_1412158c0(param_1,&local_7b8,2,local_6c8);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d0ac0;
      }
      FUN_141eb3490(&local_7b8,L"Battle_KiAtk",1);
      FUN_1412158c0(param_1,&local_7b8,1,local_6c8);
      FUN_1408f46c0(local_6c8);
      FUN_141eb3490(local_6d0,L"Controller_Btn_A",1);
      local_698 = (longlong *)0x0;
      puVar9 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      puVar5 = puVar10;
      if (puVar9 != (undefined8 *)0x0) {
        puVar9[1] = param_1;
        *puVar9 = &PTR_FUN_143969680;
        puVar5 = puVar9;
      }
      if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d1880;
      }
      FUN_141eb3490(&local_7b8,L"GuardAround",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_6d0,&local_7b8,1);
      FUN_1408f46c0(local_6c8);
      FUN_141eb3490(local_700,L"Controller_Btn_Y",1);
      local_698 = (longlong *)0x0;
      puVar5 = (undefined8 *)FUN_1407e5ee0(0x10,local_6b8);
      if (puVar5 != (undefined8 *)0x0) {
        puVar5[1] = param_1;
        *puVar5 = &PTR_FUN_143969680;
        puVar10 = puVar5;
      }
      if (puVar10 != (undefined8 *)0xfffffffffffffff8) {
        local_6c8[0] = (code *)&LAB_1411d0d50;
      }
      FUN_141eb3490(&local_7b8,L"Charge_Pressed",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_700,&local_7b8,0);
      FUN_1408f46c0(local_6c8);
      local_698 = (longlong *)0x0;
      local_7b8 = param_1;
      lVar4 = FUN_1411c8320(local_6b8,&local_7b8);
      if (lVar4 != 0) {
        local_6c8[0] = (code *)&LAB_1411d1380;
      }
      FUN_141eb3490(&local_798,L"Charge_Repeat",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_700,&local_798,2);
      FUN_1408f46c0(local_6c8);
      local_798 = param_1;
      FUN_1411c7c00(local_378,&local_798);
      FUN_141eb3490(&local_7b8,L"Charge_Released",1);
      local_7c8 = (code **)local_378;
      FUN_141215910(param_1,local_700,&local_7b8,1);
      FUN_1408f46c0(local_378);
      local_798 = param_1;
      FUN_1411c7f60(local_2b8,&local_798);
      FUN_141eb3490(&local_7b8,L"Battle_KiRecover",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_2b8);
      FUN_1408f46c0(local_2b8);
      local_798 = param_1;
      FUN_1411c7960(local_278,&local_798);
      FUN_141eb3490(&local_7b8,L"Battle_KiRecover",1);
      FUN_1412158c0(param_1,&local_7b8,2,local_278);
      FUN_1408f46c0(local_278);
      local_798 = param_1;
      FUN_1411c7c00(local_5b8,&local_798);
      FUN_141eb3490(&local_7b8,L"Battle_KiRecover",1);
      FUN_1412158c0(param_1,&local_7b8,1,local_5b8);
      FUN_1408f46c0(local_5b8);
      FUN_141eb3490(local_7a8,L"Controller_Btn_L3",1);
      local_798 = param_1;
      FUN_1411c76c0(local_578,&local_798);
      FUN_141eb3490(&local_7b8,L"HighBoost",1);
      local_7c8 = (code **)local_578;
      FUN_141215910(param_1,local_7a8,&local_7b8,0);
      FUN_1408f46c0(local_578);
      local_798 = param_1;
      FUN_1411c76c0(local_538,&local_798);
      FUN_141eb3490(&local_7b8,L"Battle_HighBoost",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_538);
      FUN_1408f46c0(local_538);
      local_798 = (code *)&LAB_1411e4e30;
      uStack_790 = 0;
      puVar10 = (undefined8 *)FUN_141eb3410(&local_768,"LongBlowoffChase",1);
      local_7c8 = &local_798;
      FUN_1411c84e0(param_1,*puVar10,0,param_1);
      FUN_141eb3490(local_7a0,L"Controller_Btn_B",1);
      local_798 = param_1;
      FUN_1411c7c60(local_4f8,&local_798);
      FUN_141eb3490(&local_7b8,L"LongBlowoffChase",1);
      local_7c8 = (code **)local_4f8;
      FUN_141215910(param_1,local_7a0,&local_7b8,0);
      FUN_1408f46c0(local_4f8);
      local_798 = param_1;
      FUN_1411c7c60(local_4b8,&local_798);
      FUN_141eb3490(&local_7b8,&DAT_143d153fc,1);
      FUN_1412158c0(param_1,&local_7b8,0,local_4b8);
      FUN_1408f46c0(local_4b8);
      FUN_141eb3490(local_710,L"Controller_Btn_LB",1);
      FUN_141eb3490(local_708,L"Controller_Btn_RB",1);
      local_798 = param_1;
      FUN_1411c7660(local_478,&local_798);
      FUN_141eb3490(&local_7b8,L"SkillCooperation1",1);
      local_7c8 = (code **)local_478;
      FUN_141215910(param_1,local_710,&local_7b8,0);
      FUN_1408f46c0(local_478);
      local_798 = param_1;
      FUN_1411c73c0(local_438,&local_798);
      FUN_141eb3490(&local_7b8,L"SkillCooperation1",1);
      local_7c8 = (code **)local_438;
      FUN_141215910(param_1,local_710,&local_7b8,2);
      FUN_1408f46c0(local_438);
      local_798 = param_1;
      FUN_1411c7e40(local_3f8,&local_798);
      FUN_141eb3490(&local_7b8,L"SkillCooperation1",1);
      local_7c8 = (code **)local_3f8;
      FUN_141215910(param_1,local_710,&local_7b8,1);
      FUN_1408f46c0(local_3f8);
      local_798 = param_1;
      FUN_1411c7840(local_3b8,&local_798);
      FUN_141eb3490(&local_7b8,L"SkillCooperation2",1);
      local_7c8 = (code **)local_3b8;
      FUN_141215910(param_1,local_708,&local_7b8,0);
      FUN_1408f46c0(local_3b8);
      local_798 = param_1;
      FUN_1411c7b40(local_238,&local_798);
      FUN_141eb3490(&local_7b8,L"SkillCooperation2",1);
      local_7c8 = (code **)local_238;
      FUN_141215910(param_1,local_708,&local_7b8,2);
      FUN_1408f46c0(local_238);
      local_798 = param_1;
      FUN_1411c7a80(local_338,&local_798);
      FUN_141eb3490(&local_7b8,L"SkillCooperation2",1);
      local_7c8 = (code **)local_338;
      FUN_141215910(param_1,local_708,&local_7b8,1);
      FUN_1408f46c0(local_338);
      local_798 = param_1;
      FUN_1411c7fc0(local_2f8,&local_798);
      FUN_141eb3490(&local_7b8,L"Battle_Zcombo",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_2f8);
      FUN_1408f46c0(local_2f8);
      local_798 = param_1;
      FUN_1411c7f00(local_638,&local_798);
      FUN_141eb3490(&local_7b8,L"Battle_Zcombo",1);
      FUN_1412158c0(param_1,&local_7b8,2,local_638);
      FUN_1408f46c0(local_638);
      local_798 = param_1;
      FUN_1411c72a0(local_5f8,&local_798);
      FUN_141eb3490(&local_7b8,L"Battle_Zcombo",1);
      FUN_1412158c0(param_1,&local_7b8,1,local_5f8);
      FUN_1408f46c0(local_5f8);
      FUN_141eb3490(local_778,L"Controller_Btn_B",1);
      if (bVar2) {
        local_798 = param_1;
        FUN_1411c7420();
        FUN_141eb3490(&local_7b8,L"Attack",1);
        local_7c8 = (code **)local_5f8;
        FUN_141215910(param_1,local_778,&local_7b8,0);
        FUN_1408f46c0(local_5f8);
        local_798 = param_1;
        FUN_1411c7420(local_638,&local_798);
        pwVar11 = L"Battle_MeleeAtk";
      }
      else {
        local_798 = param_1;
        FUN_1411c77e0(local_5f8,&local_798);
        FUN_141eb3490(&local_7b8,L"Attack",1);
        local_7c8 = (code **)local_5f8;
        FUN_141215910(param_1,local_778,&local_7b8,0);
        FUN_1408f46c0(local_5f8);
        local_798 = param_1;
        FUN_1411c77e0(local_638,&local_798);
        pwVar11 = L"Battle_MeleeAtk";
      }
      FUN_141eb3490(&local_7b8,pwVar11,1);
      FUN_1412158c0(param_1,&local_7b8,0,local_638);
      FUN_1408f46c0(local_638);
      FUN_141eb3490(&local_7b0,L"Controller_Btn_A",1);
      local_798 = param_1;
      FUN_1411c7ba0(local_5f8,&local_798);
      FUN_141eb3490(&local_7b8,L"Step",1);
      local_7c8 = (code **)local_5f8;
      FUN_141215910(param_1,&local_7b0,&local_7b8,0);
      FUN_1408f46c0(local_5f8);
      local_798 = param_1;
      FUN_1411c75a0(local_638,&local_798);
      FUN_141eb3490(&local_7b8,L"Battle_Boost",1);
      FUN_1412158c0(param_1,&local_7b8,0,local_638);
      FUN_1408f46c0(local_638);
      local_7b0 = lVar1;
      FUN_1411c7300(local_2f8,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_Y",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_Y",1);
      local_7c8 = (code **)local_2f8;
      FUN_141215910(param_1,local_7a8,local_7a0,0);
      FUN_1408f46c0(local_2f8);
      local_7b0 = lVar1;
      FUN_1411c80e0(local_338,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_Y",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_Y",1);
      local_7c8 = (code **)local_338;
      FUN_141215910(param_1,local_7a8,local_7a0,1);
      FUN_1408f46c0(local_338);
      local_7b0 = lVar1;
      FUN_1411c7360(local_238,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_A",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_A",1);
      local_7c8 = (code **)local_238;
      FUN_141215910(param_1,local_7a8,local_7a0,0);
      FUN_1408f46c0(local_238);
      local_7b0 = lVar1;
      FUN_1411c7600(local_3b8,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_A",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_A",1);
      local_7c8 = (code **)local_3b8;
      FUN_141215910(param_1,local_7a8,local_7a0,1);
      FUN_1408f46c0(local_3b8);
      local_7b0 = lVar1;
      FUN_1411c7780(local_3f8,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_B",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_B",1);
      local_7c8 = (code **)local_3f8;
      FUN_141215910(param_1,local_7a8,local_7a0,0);
      FUN_1408f46c0(local_3f8);
      local_7b0 = lVar1;
      FUN_1411c79c0(local_438,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_B",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_B",1);
      local_7c8 = (code **)local_438;
      FUN_141215910(param_1,local_7a8,local_7a0,1);
      FUN_1408f46c0(local_438);
      local_7b0 = lVar1;
      FUN_1411c78a0(local_478,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_X",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_X",1);
      local_7c8 = (code **)local_478;
      FUN_141215910(param_1,local_7a8,local_7a0,0);
      FUN_1408f46c0(local_478);
      local_7b0 = lVar1;
      FUN_1411c8080(local_4b8,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_X",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_X",1);
      local_7c8 = (code **)local_4b8;
      FUN_141215910(param_1,local_7a8,local_7a0,1);
      FUN_1408f46c0(local_4b8);
      local_7b0 = lVar1;
      FUN_1411c7de0(local_4f8,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_Start",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_Start",1);
      local_7c8 = (code **)local_4f8;
      FUN_141215910(param_1,local_7a8,local_7a0,0);
      FUN_1408f46c0(local_4f8);
      local_7b0 = lVar1;
      FUN_1411c7d20(local_538,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_Start",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_Start",1);
      local_7c8 = (code **)local_538;
      FUN_141215910(param_1,local_7a8,local_7a0,1);
      FUN_1408f46c0(local_538);
      local_7b0 = lVar1;
      FUN_1411c74e0(local_578,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_Select",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_Select",1);
      local_7c8 = (code **)local_578;
      FUN_141215910(param_1,local_7a8,local_7a0,0);
      FUN_1408f46c0(local_578);
      local_7b0 = lVar1;
      FUN_1411c7d80(local_5b8,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_Select",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_Select",1);
      local_7c8 = (code **)local_5b8;
      FUN_141215910(param_1,local_7a8,local_7a0,1);
      FUN_1408f46c0(local_5b8);
      local_7b0 = lVar1;
      FUN_1411c7a20(local_278,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_RB",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_RB",1);
      local_7c8 = (code **)local_278;
      FUN_141215910(param_1,local_7a8,local_7a0,0);
      FUN_1408f46c0(local_278);
      local_7b0 = lVar1;
      FUN_1411c7540(local_2b8,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_RB",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_RB",1);
      local_7c8 = (code **)local_2b8;
      FUN_141215910(param_1,local_7a8,local_7a0,1);
      FUN_1408f46c0(local_2b8);
      local_7b0 = lVar1;
      FUN_1411c8200(local_378,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_LB",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_LB",1);
      local_7c8 = (code **)local_378;
      FUN_141215910(param_1,local_7a8,local_7a0,0);
      FUN_1408f46c0(local_378);
      local_7b0 = lVar1;
      FUN_1411c81a0(local_6c8,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_LB",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_LB",1);
      local_7c8 = local_6c8;
      FUN_141215910(param_1,local_7a8,local_7a0,1);
      FUN_1408f46c0(local_6c8);
      local_7b0 = lVar1;
      FUN_1411c7ae0(local_1f8,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_RT",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_RT",1);
      local_7c8 = (code **)local_1f8;
      FUN_141215910(param_1,local_7a8,local_7a0,0);
      FUN_1408f46c0(local_1f8);
      local_7b0 = lVar1;
      FUN_1411c7ea0(local_78,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_RT",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_RT",1);
      local_7c8 = (code **)local_78;
      FUN_141215910(param_1,local_7a8,local_7a0,1);
      FUN_1408f46c0(local_78);
      local_7b0 = lVar1;
      FUN_1411c8020(local_1b8,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_LT",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_LT",1);
      local_7c8 = (code **)local_1b8;
      FUN_141215910(param_1,local_7a8,local_7a0,0);
      FUN_1408f46c0(local_1b8);
      local_7b0 = lVar1;
      FUN_1411c7720(local_178,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_LT",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_LT",1);
      local_7c8 = (code **)local_178;
      FUN_141215910(param_1,local_7a8,local_7a0,1);
      FUN_1408f46c0(local_178);
      local_7b0 = lVar1;
      FUN_1411c8140(local_138,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_SR",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_SR",1);
      local_7c8 = (code **)local_138;
      FUN_141215910(param_1,local_7a8,local_7a0,0);
      FUN_1408f46c0(local_138);
      local_7b0 = lVar1;
      FUN_1411c7900(local_f8,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_SR",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_SR",1);
      local_7c8 = (code **)local_f8;
      FUN_141215910(param_1,local_7a8,local_7a0,1);
      FUN_1408f46c0(local_f8);
      local_7b0 = lVar1;
      FUN_1411c7cc0(local_b8,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_SL",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_SL",1);
      local_7c8 = (code **)local_b8;
      FUN_141215910(param_1,local_7a8,local_7a0,0);
      FUN_1408f46c0(local_b8);
      local_7b0 = lVar1;
      FUN_1411c7480(&local_688,&local_7b0);
      FUN_141eb3490(local_7a0,L"Controller_Btn_SL",1);
      FUN_141eb3490(local_7a8,L"Controller_Btn_SL",1);
      local_7c8 = (code **)&local_688;
      FUN_141215910(param_1,local_7a8,local_7a0,1);
      FUN_1408f46c0(&local_688);
    }
  }
  return;
}



// (already emitted) Controller_Btn_B <- 1411de799 -> FUN_1411dba70
// (already emitted) Controller_Btn_B <- 1411deb6b -> FUN_1411dba70
// (already emitted) Controller_Btn_B <- 1411def16 -> FUN_1411dba70
// (already emitted) Controller_Btn_B <- 1411def2a -> FUN_1411dba70
// (already emitted) Controller_Btn_B <- 1411def81 -> FUN_1411dba70
// (already emitted) Controller_Btn_B <- 1411def95 -> FUN_1411dba70
// ================= Controller_Btn_B <- 1411e040e  FUN_1411e0180 @ 1411e0180  size=0x1c3e =================
// callers: FUN_141427500@141427500 FUN_14143a170@14143a170 

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1411e0180(longlong param_1)

{
  bool bVar1;
  longlong lVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong lVar5;
  longlong *plVar6;
  undefined8 *puVar7;
  longlong *plVar8;
  longlong *plVar9;
  longlong *plVar10;
  undefined1 auStack_138 [32];
  code **local_118;
  code *local_108;
  undefined4 uStack_100;
  uint uStack_fc;
  longlong local_f8;
  code *local_e8;
  longlong lStack_e0;
  undefined4 local_d8;
  uint uStack_d4;
  undefined1 *local_c8;
  undefined4 uStack_c0;
  uint uStack_bc;
  undefined1 local_b8 [8];
  undefined1 local_b0 [8];
  undefined1 local_a8 [8];
  undefined1 local_a0 [8];
  undefined1 local_98 [8];
  undefined1 local_90 [8];
  undefined1 local_88 [16];
  code *local_78 [2];
  longlong local_68 [4];
  longlong *local_48;
  ulonglong local_38;
  
  local_38 = DAT_145909780 ^ (ulonglong)auStack_138;
  plVar9 = *(longlong **)(param_1 + 0xf0);
  plVar10 = (longlong *)0x0;
  if (plVar9 == (longlong *)0x0) {
LAB_1411e01f1:
    plVar9 = plVar10;
  }
  else {
    lVar2 = FUN_141742050();
    if ((*(int *)(plVar9[2] + 0x90) < *(int *)(lVar2 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar9[2] + 0x88) + (longlong)*(int *)(lVar2 + 0x90) * 8) !=
        lVar2 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (!bVar1) goto LAB_1411e01f1;
  }
  plVar9 = (longlong *)plVar9[0x6f];
  if (plVar9 != (longlong *)0x0) {
    lVar2 = FUN_141742340();
    if ((*(int *)(plVar9[2] + 0x90) < *(int *)(lVar2 + 0x90)) ||
       (*(longlong *)(*(longlong *)(plVar9[2] + 0x88) + (longlong)*(int *)(lVar2 + 0x90) * 8) !=
        lVar2 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) goto LAB_1411e0239;
  }
  plVar9 = plVar10;
LAB_1411e0239:
  lVar2 = *(longlong *)(param_1 + 0xf0);
  if (lVar2 != 0) {
    lVar3 = FUN_141742050();
    if ((*(int *)(*(longlong *)(lVar2 + 0x10) + 0x90) < *(int *)(lVar3 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar2 + 0x10) + 0x88) + (longlong)*(int *)(lVar3 + 0x90) * 8)
        != lVar3 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if ((bVar1) && (plVar8 = *(longlong **)(lVar2 + 0x368), plVar8 != (longlong *)0x0)) {
      uVar4 = FUN_141741ce0();
      lVar3 = (**(code **)(*plVar8 + 0x600))(plVar8,uVar4);
      if (lVar3 != 0) {
        lVar5 = FUN_141742340();
        if ((*(int *)(plVar8[2] + 0x90) < *(int *)(lVar5 + 0x90)) ||
           (*(longlong *)(*(longlong *)(plVar8[2] + 0x88) + (longlong)*(int *)(lVar5 + 0x90) * 8) !=
            lVar5 + 0x88)) {
          bVar1 = false;
        }
        else {
          bVar1 = true;
        }
        plVar6 = plVar8;
        if (!bVar1) {
          plVar6 = plVar10;
        }
        *(longlong **)(param_1 + 0x248) = plVar6;
        uVar4 = FUN_141741af0();
        uVar4 = (**(code **)(*plVar8 + 0x600))(plVar8,uVar4);
        *(longlong *)(param_1 + 600) = lVar3;
        *(undefined8 *)(param_1 + 0x250) = uVar4;
        FUN_142f7dfc0(lVar2,param_1);
        *(undefined8 *)(param_1 + 0x260) = *(undefined8 *)(lVar2 + 0x3d8);
      }
    }
  }
  if (plVar9 != (longlong *)0x0) {
    local_c8 = &LAB_1411e4610;
    uStack_c0 = 0;
    local_118 = &local_108;
    local_108 = (code *)&LAB_1411e4610;
    uStack_100 = 0;
    uStack_fc = uStack_bc;
    FUN_1411c84e0(param_1,*(undefined8 *)(param_1 + 0x268),0,param_1);
    uStack_100 = 0;
    local_108 = _guard_check_icall;
    local_e8 = _guard_check_icall;
    lStack_e0 = (ulonglong)uStack_fc << 0x20;
    puVar7 = (undefined8 *)FUN_141eb3410(&local_108,"PushPad_X",1);
    local_118 = &local_e8;
    FUN_1411c84e0(param_1,*puVar7,0,param_1);
    FUN_141eb3490(local_90,L"Controller_Btn_X",1);
    FUN_141eb3490(local_88,L"Controller_Btn_Y",1);
    FUN_141eb3490(local_b8,L"Controller_Btn_B",1);
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d0d40;
    }
    FUN_141eb3490(&local_108,L"PushPad_X",1);
    local_118 = local_78;
    FUN_141a584f0(param_1,local_90,&local_108,0);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d0dc0;
    }
    FUN_141eb3490(&local_108,L"FirstPersonView",1);
    local_118 = local_78;
    FUN_141a584f0(param_1,local_88,&local_108,0);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = _guard_check_icall;
    }
    FUN_141eb3490(&local_108,L"PushPad_B",1);
    local_118 = local_78;
    FUN_141a584f0(param_1,local_b8,&local_108,0);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d1940;
    }
    FUN_141eb3490(&local_108,L"Ride_Off",1);
    FUN_141a58370(param_1,&local_108,0,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d0dc0;
    }
    FUN_141eb3490(&local_108,L"Car_ChangeView",1);
    FUN_141a58370(param_1,&local_108,0,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d0d40;
    }
    FUN_141eb3490(&local_108,L"MineralRadar",1);
    FUN_141a58370(param_1,&local_108,0,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    FUN_141eb3490(local_a8,L"Controller_Btn_RT",1);
    FUN_141eb3490(local_a0,L"Controller_Btn_LT",1);
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d0da0;
    }
    FUN_141eb3490(&local_108,L"CarAccele_Pressed",1);
    local_118 = local_78;
    FUN_141a584f0(param_1,local_a8,&local_108,0);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d14b0;
    }
    FUN_141eb3490(&local_108,L"CarAccele_Released",1);
    local_118 = local_78;
    FUN_141a584f0(param_1,local_a8,&local_108,1);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d0d10;
    }
    FUN_141eb3490(&local_108,L"CarBack_Pressed",1);
    local_118 = local_78;
    FUN_141a584f0(param_1,local_a0,&local_108,0);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d18d0;
    }
    FUN_141eb3490(&local_108,L"CarBack_Released",1);
    local_118 = local_78;
    FUN_141a584f0(param_1,local_a0,&local_108,1);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d0da0;
    }
    FUN_141eb3490(&local_108,L"Car_Forward",1);
    FUN_141a58370(param_1,&local_108,0,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d14b0;
    }
    FUN_141eb3490(&local_108,L"Car_Forward",1);
    FUN_141a58370(param_1,&local_108,1,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d0d10;
    }
    FUN_141eb3490(&local_108,L"Car_Reverse",1);
    FUN_141a58370(param_1,&local_108,0,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d18d0;
    }
    FUN_141eb3490(&local_108,L"Car_Reverse",1);
    FUN_141a58370(param_1,&local_108,1,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    uStack_100 = 0;
    local_108 = (code *)&LAB_1411e32c0;
    local_e8 = (code *)&LAB_1411e32c0;
    lStack_e0 = (ulonglong)uStack_fc << 0x20;
    puVar7 = (undefined8 *)FUN_141eb3410(&local_108,"MoveForward",1);
    FUN_140b1fbe0(param_1,*puVar7,param_1,&local_e8);
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d0b80;
    }
    FUN_141eb3490(&local_108,L"Move_Up",1);
    FUN_141a58370(param_1,&local_108,0,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d1910;
    }
    FUN_141eb3490(&local_108,L"Move_Up",1);
    FUN_141a58370(param_1,&local_108,1,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d0a40;
    }
    FUN_141eb3490(&local_108,L"Move_Down",1);
    FUN_141a58370(param_1,&local_108,0,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d1180;
    }
    FUN_141eb3490(&local_108,L"Move_Down",1);
    FUN_141a58370(param_1,&local_108,1,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    uStack_100 = 0;
    local_108 = (code *)&LAB_1411e32e0;
    local_e8 = (code *)&LAB_1411e32e0;
    lStack_e0 = (ulonglong)uStack_fc << 0x20;
    puVar7 = (undefined8 *)FUN_141eb3410(&local_108,"MoveRight",1);
    FUN_140b1fbe0(param_1,*puVar7,param_1,&local_e8);
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d1c30;
    }
    FUN_141eb3490(&local_108,L"Move_Left",1);
    FUN_141a58370(param_1,&local_108,0,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d14d0;
    }
    FUN_141eb3490(&local_108,L"Move_Left",1);
    FUN_141a58370(param_1,&local_108,1,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d19d0;
    }
    FUN_141eb3490(&local_108,L"Move_Right",1);
    FUN_141a58370(param_1,&local_108,0,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d1790;
    }
    FUN_141eb3490(&local_108,L"Move_Right",1);
    FUN_141a58370(param_1,&local_108,1,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    uStack_100 = 0;
    local_108 = (code *)&LAB_1411d9110;
    local_e8 = (code *)&LAB_1411d9110;
    lStack_e0 = (ulonglong)uStack_fc << 0x20;
    puVar7 = (undefined8 *)FUN_141eb3410(&local_108,"CameraForward",1);
    FUN_140b1fbe0(param_1,*puVar7,param_1,&local_e8);
    uStack_100 = 0;
    local_108 = (code *)&LAB_1411d90f0;
    local_e8 = (code *)&LAB_1411d90f0;
    lStack_e0 = (ulonglong)uStack_fc << 0x20;
    puVar7 = (undefined8 *)FUN_141eb3410(&local_108,"CameraRight",1);
    FUN_140b1fbe0(param_1,*puVar7,param_1,&local_e8);
    FUN_141eb3490(local_98,L"Controller_Btn_Back",1);
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d15c0;
    }
    FUN_141eb3490(&local_108,L"MapChange_Pressed",1);
    local_118 = local_78;
    FUN_141a584f0(param_1,local_98,&local_108,0);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d13b0;
    }
    FUN_141eb3490(&local_108,L"MapChange_Released",1);
    local_118 = local_78;
    FUN_141a584f0(param_1,local_98,&local_108,1);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d15c0;
    }
    FUN_141eb3490(&local_108,L"MapChange",1);
    FUN_141a58370(param_1,&local_108,0,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d13b0;
    }
    FUN_141eb3490(&local_108,L"MapChange",1);
    FUN_141a58370(param_1,&local_108,1,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    uStack_100 = 0;
    local_108 = FUN_1411e6c50;
    local_e8 = FUN_1411e6c50;
    lStack_e0 = (ulonglong)uStack_fc << 0x20;
    puVar7 = (undefined8 *)FUN_141eb3410(&local_108,"PauseMenu",1);
    local_118 = &local_e8;
    FUN_1411c84e0(param_1,*puVar7,0,param_1);
    FUN_141eb3490(local_b0,L"Controller_Btn_Start",1);
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d1840;
    }
    FUN_141eb3490(&local_108,L"PauseMenu",1);
    local_118 = local_78;
    FUN_141a584f0(param_1,local_b0,&local_108,0);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_108 = FUN_1411e6d30;
    local_118 = &local_e8;
    uStack_100 = 0;
    local_e8 = FUN_1411e6d30;
    lStack_e0 = (ulonglong)uStack_fc << 0x20;
    FUN_1411c84e0(param_1,*(undefined8 *)(param_1 + 0x270),0,param_1);
    FUN_141eb3490(&local_c8,L"Controller_Btn_R3",1);
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411d1be0;
    }
    FUN_141eb3490(&local_108,L"ResetCamera",1);
    local_118 = local_78;
    FUN_141a584f0(param_1,&local_c8,&local_108,0);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    uStack_100 = 0;
    local_108 = FUN_1411e6e00;
    local_e8 = FUN_1411e6e00;
    lStack_e0 = (ulonglong)uStack_fc << 0x20;
    puVar7 = (undefined8 *)FUN_141eb3490(&local_108,L"RideBGMForward",1);
    local_118 = &local_e8;
    FUN_1411c84e0(param_1,*puVar7,0,param_1);
    local_108 = FUN_1411e6e00;
    uStack_100 = 0;
    local_e8 = FUN_1411e6e00;
    lStack_e0 = (ulonglong)uStack_fc << 0x20;
    puVar7 = (undefined8 *)FUN_141eb3490(&local_108,L"RideBGMForward",1);
    local_118 = &local_e8;
    FUN_1411c84e0(param_1,*puVar7,2,param_1);
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d1120;
    }
    FUN_141eb3490(&local_108,L"Car_Sound_Left",1);
    FUN_141a58370(param_1,&local_108,0,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d1120;
    }
    FUN_141eb3490(&local_108,L"Car_Sound_Left",1);
    FUN_141a58370(param_1,&local_108,2,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    uStack_100 = 0;
    local_108 = FUN_1411e6dd0;
    local_e8 = FUN_1411e6dd0;
    lStack_e0 = (ulonglong)uStack_fc << 0x20;
    puVar7 = (undefined8 *)FUN_141eb3490(&local_108,L"RideBGMBack",1);
    local_118 = &local_e8;
    FUN_1411c84e0(param_1,*puVar7,0,param_1);
    local_108 = FUN_1411e6dd0;
    uStack_100 = 0;
    local_e8 = FUN_1411e6dd0;
    lStack_e0 = (ulonglong)uStack_fc << 0x20;
    puVar7 = (undefined8 *)FUN_141eb3490(&local_108,L"RideBGMBack",1);
    local_118 = &local_e8;
    FUN_1411c84e0(param_1,*puVar7,2,param_1);
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d1010;
    }
    FUN_141eb3490(&local_108,L"Car_Sound_Right",1);
    FUN_141a58370(param_1,&local_108,0,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong *)0x0;
    plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
    plVar9 = plVar10;
    if (plVar8 != (longlong *)0x0) {
      plVar8[1] = param_1;
      *plVar8 = (longlong)&PTR_FUN_143969680;
      plVar9 = plVar8;
    }
    if (plVar9 != (longlong *)0xfffffffffffffff8) {
      local_78[0] = FUN_1411d1010;
    }
    FUN_141eb3490(&local_108,L"Car_Sound_Right",1);
    FUN_141a58370(param_1,&local_108,2,local_78);
    if (local_78[0] != (code *)0x0) {
      plVar9 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar9 = local_48;
      }
      (**(code **)(*plVar9 + 0x10))(plVar9,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
  }
  lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x248) + 0x780);
  if (lVar2 != 0) {
    lVar3 = FUN_141758410();
    if ((*(int *)(*(longlong *)(lVar2 + 0x10) + 0x90) < *(int *)(lVar3 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar2 + 0x10) + 0x88) + (longlong)*(int *)(lVar3 + 0x90) * 8)
        != lVar3 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) {
      lStack_e0 = 0;
      local_e8 = FUN_140f15730;
      local_d8 = 0;
      local_f8 = (ulonglong)uStack_d4 << 0x20;
      local_108 = FUN_140f15730;
      uStack_100 = 0;
      uStack_fc = 0;
      puVar7 = (undefined8 *)FUN_141eb3490(&local_c8,L"FieldAction",1);
      local_118 = &local_108;
      FUN_1411c8360(param_1,*puVar7,0,lVar2);
      lStack_e0 = 0;
      local_e8 = FUN_140f158b0;
      local_d8 = 0;
      local_f8 = (ulonglong)uStack_d4 << 0x20;
      local_108 = FUN_140f158b0;
      uStack_100 = 0;
      uStack_fc = 0;
      puVar7 = (undefined8 *)FUN_141eb3490(&local_c8,L"FieldSubAction",1);
      local_118 = &local_108;
      FUN_1411c8360(param_1,*puVar7,0,lVar2);
      FUN_141eb3490(local_b0,L"Controller_Btn_B",1);
      FUN_141eb3490(local_b8,L"Controller_Btn_Y",1);
      local_48 = (longlong *)0x0;
      plVar8 = (longlong *)FUN_1407e5ee0(0x10,local_68);
      plVar9 = plVar10;
      if (plVar8 != (longlong *)0x0) {
        plVar8[1] = lVar2;
        *plVar8 = (longlong)&PTR_FUN_143969680;
        plVar9 = plVar8;
      }
      if (plVar9 != (longlong *)0xfffffffffffffff8) {
        local_78[0] = (code *)&LAB_1411d0e60;
      }
      FUN_141eb3490(&local_c8,L"FieldAction",1);
      local_118 = local_78;
      FUN_141215910(param_1,local_b0,&local_c8,0);
      if (local_78[0] != (code *)0x0) {
        plVar9 = local_68;
        if (local_48 != (longlong *)0x0) {
          plVar9 = local_48;
        }
        (**(code **)(*plVar9 + 0x10))(plVar9,0);
      }
      if (local_48 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_48 = (longlong *)0x0;
      plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_68);
      if (plVar9 != (longlong *)0x0) {
        plVar9[1] = lVar2;
        *plVar9 = (longlong)&PTR_FUN_143969680;
        plVar10 = plVar9;
      }
      if (plVar10 != (longlong *)0xfffffffffffffff8) {
        local_78[0] = (code *)&LAB_1411d1170;
      }
      FUN_141eb3490(&local_c8,L"FieldSubAction",1);
      local_118 = local_78;
      FUN_141215910(param_1,local_b8,&local_c8,0);
      if (local_78[0] != (code *)0x0) {
        plVar9 = local_68;
        if (local_48 != (longlong *)0x0) {
          plVar9 = local_48;
        }
        (**(code **)(*plVar9 + 0x10))(plVar9,0);
      }
      if (local_48 != (longlong *)0x0) {
        FUN_141de9200();
      }
    }
  }
  return;
}



// (already emitted) Controller_Btn_B <- 1411e1c39 -> FUN_1411e0180
// ================= Controller_Btn_B <- 141200a3f  FUN_141200920 @ 141200920  size=0xcba =================
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141200920(longlong param_1,undefined8 param_2)

{
  bool bVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  undefined8 *puVar7;
  longlong *plVar8;
  undefined8 *puVar9;
  longlong **pplVar10;
  undefined1 auStack_108 [32];
  code **local_e8;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined4 local_c8;
  uint uStack_c4;
  undefined1 local_c0 [8];
  undefined1 local_b8 [8];
  undefined1 local_b0 [8];
  undefined1 local_a8 [8];
  undefined1 local_a0 [8];
  undefined4 local_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  longlong local_88;
  code *local_78;
  undefined8 local_70;
  longlong *local_68;
  int local_60;
  undefined8 local_58;
  undefined4 local_50;
  longlong **local_48;
  undefined8 local_40;
  undefined1 local_38;
  ulonglong local_28;
  
  local_28 = DAT_145909780 ^ (ulonglong)auStack_108;
  *(undefined8 *)(param_1 + 0x238) = param_2;
  if ((DAT_14595f490 == '\0') && (lVar3 = FUN_140d2ae50(), lVar3 != 0)) {
    FUN_1419cf770(lVar3 + 0x720);
    DAT_14595f490 = '\x01';
  }
  lVar3 = *(longlong *)(param_1 + 0xf0);
  if (lVar3 != 0) {
    lVar4 = FUN_141863300();
    if ((*(int *)(*(longlong *)(lVar3 + 0x10) + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8)
        != lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if ((bVar1) && (plVar8 = *(longlong **)(lVar3 + 0x368), plVar8 != (longlong *)0x0)) {
      uVar5 = FUN_14187c7b0();
      uVar5 = (**(code **)(*plVar8 + 0x600))(plVar8,uVar5);
      *(undefined8 *)(param_1 + 0x248) = uVar5;
    }
  }
  puVar9 = (undefined8 *)0x0;
  local_d8 = FUN_140f15730;
  uStack_d0 = 0;
  local_c8 = 0;
  local_88 = (ulonglong)uStack_c4 << 0x20;
  local_98 = 0x40f15730;
  uStack_94 = 1;
  uStack_90 = 0;
  uStack_8c = 0;
  puVar6 = (undefined8 *)FUN_141eb3490(&local_d8,L"FieldAction",1);
  local_e8 = (code **)&local_98;
  FUN_1411c8360(param_1,*puVar6,0,param_2);
  FUN_141eb3490(local_b0,L"Controller_Btn_B",1);
  local_48 = (longlong **)0x0;
  puVar7 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  puVar6 = puVar9;
  if (puVar7 != (undefined8 *)0x0) {
    puVar7[1] = param_2;
    *puVar7 = &PTR_FUN_143969680;
    puVar6 = puVar7;
  }
  if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = (code *)&LAB_1411d0e60;
  }
  FUN_141eb3490(&local_d8,L"FieldAction",1);
  FUN_141a8e270(param_1,&local_d8,0);
  local_e8 = &local_78;
  FUN_141a584f0(param_1,local_b0,&local_d8,0);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  uStack_d0 = 0;
  local_d8 = FUN_140f158b0;
  local_c8 = 0;
  local_88 = (ulonglong)uStack_c4 << 0x20;
  local_98 = 0x40f158b0;
  uStack_94 = 1;
  uStack_90 = 0;
  uStack_8c = 0;
  puVar6 = (undefined8 *)FUN_141eb3490(&local_d8,L"FieldSubAction",1);
  local_e8 = (code **)&local_98;
  FUN_1411c8360(param_1,*puVar6,0,param_2);
  FUN_141eb3490(local_a8,L"Controller_Btn_Y",1);
  local_48 = (longlong **)0x0;
  puVar7 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  puVar6 = puVar9;
  if (puVar7 != (undefined8 *)0x0) {
    puVar7[1] = param_2;
    *puVar7 = &PTR_FUN_143969680;
    puVar6 = puVar7;
  }
  if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = (code *)&LAB_1411d1170;
  }
  FUN_141eb3490(&local_d8,L"FieldSubAction",1);
  FUN_141a8e270(param_1,&local_d8,0);
  local_e8 = &local_78;
  FUN_141a584f0(param_1,local_a8,&local_d8,0);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_78 = (code *)((ulonglong)local_78 & 0xfffffffffffffefc);
  local_d8 = _guard_check_icall;
  local_70 = *(undefined8 *)(param_1 + 0x250);
  uStack_d0 = uStack_d0 & 0xffffffff00000000;
  local_78._0_3_ = (uint3)(ushort)local_78;
  local_68 = (longlong *)0x0;
  local_60 = 0;
  local_58 = 0;
  local_50 = 0;
  FUN_1420575e0(&local_48,0);
  local_40 = 0;
  local_38 = 0;
  FUN_1411ef520(&local_68);
  local_38 = 1;
  plVar8 = (longlong *)FUN_141df5850(0,0x30);
  if (plVar8 == (longlong *)0x0) {
    if ((local_60 != 0) && (local_68 != (longlong *)0x0)) {
      (**(code **)(*local_68 + 0x30))(local_68,0);
      if (local_68 != (longlong *)0x0) {
        local_68 = (longlong *)FUN_141df5850(local_68,0,0);
      }
      local_60 = 0;
    }
  }
  else {
    *plVar8 = (longlong)&PTR_FUN_143968d20;
    FUN_1420575e0(plVar8 + 1,param_1);
    *(undefined4 *)(plVar8 + 2) = (undefined4)local_d8;
    *(undefined4 *)((longlong)plVar8 + 0x14) = local_d8._4_4_;
    *(undefined4 *)(plVar8 + 3) = (undefined4)uStack_d0;
    *(undefined4 *)((longlong)plVar8 + 0x1c) = uStack_d0._4_4_;
    lVar3 = FUN_141db8700();
    plVar8[5] = lVar3;
    *plVar8 = (longlong)&PTR_FUN_143968d70;
    (*(code *)PTR_FUN_143968da8)(plVar8,&local_68);
    (**(code **)(*plVar8 + 0x30))(plVar8,0);
    lVar3 = FUN_141df5850(plVar8,0,0);
    if (lVar3 != 0) {
      FUN_141de9200(lVar3);
    }
  }
  FUN_142c367c0(param_1,&local_78);
  FUN_1407e59d0(&local_58);
  FUN_1407e59d0(&local_68);
  FUN_141eb3490(local_a0,L"Controller_Btn_LT",1);
  local_48 = (longlong **)0x0;
  puVar7 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  puVar6 = puVar9;
  if (puVar7 != (undefined8 *)0x0) {
    puVar7[1] = param_1;
    *puVar7 = &PTR_FUN_143969680;
    puVar6 = puVar7;
  }
  if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = _guard_check_icall;
  }
  FUN_141eb3490(&local_d8,L"KiSearch",1);
  FUN_141a8e270(param_1,&local_d8,0);
  local_e8 = &local_78;
  FUN_141a584f0(param_1,local_a0,&local_d8,0);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_c0,L"Controller_Btn_LB",1);
  local_48 = (longlong **)0x0;
  puVar7 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  puVar6 = puVar9;
  if (puVar7 != (undefined8 *)0x0) {
    puVar7[1] = param_1;
    *puVar7 = &PTR_FUN_143969680;
    puVar6 = puVar7;
  }
  if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = FUN_1411f93d0;
  }
  FUN_141eb3490(&local_d8,L"v03_KiSearch_Pressed",1);
  FUN_141a8e270(param_1,&local_d8,0);
  local_e8 = &local_78;
  FUN_141a584f0(param_1,local_c0,&local_d8,0);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong **)0x0;
  puVar7 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  puVar6 = puVar9;
  if (puVar7 != (undefined8 *)0x0) {
    puVar7[1] = param_1;
    *puVar7 = &PTR_FUN_143969680;
    puVar6 = puVar7;
  }
  if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = FUN_1411f93d0;
  }
  FUN_141eb3490(&local_d8,L"v03_KiSearch_Repeat",1);
  FUN_141a8e270(param_1,&local_d8,2);
  local_e8 = &local_78;
  FUN_141a584f0(param_1,local_c0,&local_d8,2);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong **)0x0;
  puVar7 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  puVar6 = puVar9;
  if (puVar7 != (undefined8 *)0x0) {
    puVar7[1] = param_1;
    *puVar7 = &PTR_FUN_143969680;
    puVar6 = puVar7;
  }
  if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = (code *)&LAB_1411f8bf0;
  }
  FUN_141eb3490(&local_d8,L"v03_KiSearch_Released",1);
  FUN_141a8e270(param_1,&local_d8,1);
  local_e8 = &local_78;
  FUN_141a584f0(param_1,local_c0,&local_d8,1);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong **)0x0;
  puVar7 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  puVar6 = puVar9;
  if (puVar7 != (undefined8 *)0x0) {
    puVar7[1] = param_1;
    *puVar7 = &PTR_FUN_143969680;
    puVar6 = puVar7;
  }
  if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = FUN_1411f93d0;
  }
  FUN_141eb3490(&local_d8,L"KiSearch",1);
  FUN_141a8e170(param_1,&local_d8,0);
  FUN_141a58370(param_1,&local_d8,0,&local_78);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong **)0x0;
  puVar7 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  puVar6 = puVar9;
  if (puVar7 != (undefined8 *)0x0) {
    puVar7[1] = param_1;
    *puVar7 = &PTR_FUN_143969680;
    puVar6 = puVar7;
  }
  if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = FUN_1411f93d0;
  }
  FUN_141eb3490(&local_d8,L"KiSearch",1);
  FUN_141a8e170(param_1,&local_d8,2);
  FUN_141a58370(param_1,&local_d8,2,&local_78);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong **)0x0;
  puVar7 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  puVar6 = puVar9;
  if (puVar7 != (undefined8 *)0x0) {
    puVar7[1] = param_1;
    *puVar7 = &PTR_FUN_143969680;
    puVar6 = puVar7;
  }
  if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = (code *)&LAB_1411f8bf0;
  }
  FUN_141eb3490(&local_d8,L"KiSearch",1);
  FUN_141a8e170(param_1,&local_d8,1);
  FUN_141a58370(param_1,&local_d8,1,&local_78);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_b8,L"Controller_Btn_Back",1);
  local_48 = (longlong **)0x0;
  puVar7 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  puVar6 = puVar9;
  if (puVar7 != (undefined8 *)0x0) {
    puVar7[1] = param_1;
    *puVar7 = &PTR_FUN_143969680;
    puVar6 = puVar7;
  }
  if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = (code *)&LAB_1411f8e80;
  }
  FUN_141eb3490(&local_d8,L"MapChange_Pressed",1);
  FUN_141a8e270(param_1,&local_d8,0);
  local_e8 = &local_78;
  FUN_141a584f0(param_1,local_b8,&local_d8,0);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong **)0x0;
  puVar7 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  puVar6 = puVar9;
  if (puVar7 != (undefined8 *)0x0) {
    puVar7[1] = param_1;
    *puVar7 = &PTR_FUN_143969680;
    puVar6 = puVar7;
  }
  if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = FUN_1411f9270;
  }
  FUN_141eb3490(&local_d8,L"MapChange_Released",1);
  FUN_141a8e270(param_1,&local_d8,1);
  local_e8 = &local_78;
  FUN_141a584f0(param_1,local_b8,&local_d8,1);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong **)0x0;
  puVar7 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  puVar6 = puVar9;
  if (puVar7 != (undefined8 *)0x0) {
    puVar7[1] = param_1;
    *puVar7 = &PTR_FUN_143969680;
    puVar6 = puVar7;
  }
  if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = (code *)&LAB_1411f8e80;
  }
  FUN_141eb3490(&local_d8,L"MapChange",1);
  FUN_141a8e170(param_1,&local_d8,0);
  FUN_141a58370(param_1,&local_d8,0,&local_78);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong **)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar9 = puVar6;
  }
  if (puVar9 != (undefined8 *)0xfffffffffffffff8) {
    local_78 = FUN_1411f9270;
  }
  FUN_141eb3490(&local_d8,L"MapChange",1);
  FUN_141a8e170(param_1,&local_d8,1);
  FUN_141a58370(param_1,&local_d8,1,&local_78);
  if (local_78 != (code *)0x0) {
    pplVar10 = &local_68;
    if (local_48 != (longlong **)0x0) {
      pplVar10 = local_48;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_48 != (longlong **)0x0) {
    FUN_141de9200();
  }
  uStack_d0 = uStack_d0 & 0xffffffff00000000;
  local_d8 = (code *)&LAB_1412130d0;
  plVar8 = (longlong *)FUN_141df5850(0,0x30);
  if (plVar8 != (longlong *)0x0) {
    *plVar8 = (longlong)&PTR_FUN_143c39278;
    FUN_1420575e0(plVar8 + 1,param_1);
    *(undefined4 *)(plVar8 + 2) = (undefined4)local_d8;
    *(undefined4 *)((longlong)plVar8 + 0x14) = local_d8._4_4_;
    *(undefined4 *)(plVar8 + 3) = (undefined4)uStack_d0;
    *(undefined4 *)((longlong)plVar8 + 0x1c) = uStack_d0._4_4_;
    lVar3 = FUN_141db8700();
    plVar8[5] = lVar3;
    *plVar8 = (longlong)&PTR_FUN_143c392c8;
    FUN_1407e7830(&DAT_1456de4a0,1);
    (**(code **)(*plVar8 + 0x28))(plVar8,&local_d8);
    iVar2 = DAT_1456de4a8;
    lVar3 = (longlong)DAT_1456de4a8;
    DAT_1456de4a8 = DAT_1456de4a8 + 1;
    if (DAT_1456de4ac < DAT_1456de4a8) {
      FUN_1407f1790(&DAT_1456de4a0,iVar2);
    }
    puVar9 = (undefined8 *)(lVar3 * 0x10 + DAT_1456de4a0);
    *puVar9 = plVar8;
    *(undefined4 *)(puVar9 + 1) = 3;
  }
  return;
}



// ================= Controller_Btn_B <- 1412007f9  FUN_141200680 @ 141200680  size=0x2a0 =================
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141200680(longlong param_1,undefined8 param_2)

{
  bool bVar1;
  char cVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  longlong *plVar7;
  char *pcVar8;
  wchar_t *pwVar9;
  undefined1 auStack_c8 [32];
  code **local_a8;
  undefined1 *local_98;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined1 local_88 [16];
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  code *local_68 [2];
  longlong local_58 [4];
  longlong *local_38;
  ulonglong local_28;
  
  local_28 = DAT_145909780 ^ (ulonglong)auStack_c8;
  *(undefined8 *)(param_1 + 0x238) = param_2;
  if ((DAT_14595f490 == '\0') && (lVar3 = FUN_140d2ae50(), lVar3 != 0)) {
    FUN_1419cf770(lVar3 + 0x720);
    DAT_14595f490 = '\x01';
  }
  lVar3 = *(longlong *)(param_1 + 0xf0);
  if (lVar3 != 0) {
    lVar4 = FUN_141863300();
    lVar3 = *(longlong *)(lVar3 + 0x10);
    if ((*(int *)(lVar3 + 0x90) < *(int *)(lVar4 + 0x90)) ||
       (*(longlong *)(*(longlong *)(lVar3 + 0x88) + (longlong)*(int *)(lVar4 + 0x90) * 8) !=
        lVar4 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if ((bVar1) && (lVar3 = *(longlong *)(param_1 + 0x238), lVar3 != 0)) {
      cVar2 = FUN_140d2f9d0();
      *(undefined8 *)(param_1 + 0x248) = *(undefined8 *)(lVar3 + 0x1188);
      puVar6 = (undefined8 *)0x0;
      local_98 = &LAB_1411e4e30;
      uStack_90 = 0;
      local_78 = 0x411e4e30;
      uStack_74 = 1;
      uStack_70 = 0;
      uStack_6c = uStack_8c;
      if (cVar2 == '\0') {
        pcVar8 = "LongBlowoffChase";
      }
      else {
        puVar5 = (undefined8 *)FUN_141eb3410(&local_98,"LongBlowoffChase",1);
        local_a8 = (code **)&local_78;
        FUN_1411c84e0(param_1,*puVar5,0,param_1);
        local_98 = &LAB_141213250;
        pcVar8 = "LongBlowoffChase";
        local_78 = 0x41213250;
        uStack_6c = uStack_8c;
      }
      uStack_74 = 1;
      uStack_70 = 0;
      uStack_90 = 0;
      puVar5 = (undefined8 *)FUN_141eb3410(&local_98,pcVar8,1);
      local_a8 = (code **)&local_78;
      FUN_1411c84e0(param_1,*puVar5,0,param_1);
      FUN_141eb3490(local_88,L"Controller_Btn_B",1);
      local_38 = (longlong *)0x0;
      if (cVar2 == '\0') {
        puVar5 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
        if (puVar5 != (undefined8 *)0x0) {
          puVar5[1] = param_1;
          *puVar5 = &PTR_FUN_143969680;
          puVar6 = puVar5;
        }
        if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
          local_68[0] = (code *)&LAB_1411d1620;
        }
        pwVar9 = L"LongBlowoffChaseFinish";
      }
      else {
        puVar5 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
        if (puVar5 != (undefined8 *)0x0) {
          puVar5[1] = param_1;
          *puVar5 = &PTR_FUN_143969680;
          puVar6 = puVar5;
        }
        if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
          local_68[0] = FUN_1411f9760;
        }
        pwVar9 = L"LongBlowoffChaseFinish";
      }
      FUN_141eb3490(&local_98,pwVar9,1);
      FUN_141a8e270(param_1,&local_98,0);
      local_a8 = local_68;
      FUN_141a584f0(param_1,local_88,&local_98,0);
      if (local_68[0] != (code *)0x0) {
        plVar7 = local_58;
        if (local_38 != (longlong *)0x0) {
          plVar7 = local_38;
        }
        (**(code **)(*plVar7 + 0x10))(plVar7,0);
      }
      if (local_38 != (longlong *)0x0) {
        FUN_141de9200();
      }
    }
  }
  return;
}



// ================= Controller_Btn_B <- 14120590d  FUN_1412032b0 @ 1412032b0  size=0x2f04 =================
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1412032b0(longlong param_1,undefined8 param_2)

{
  bool bVar1;
  longlong lVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  longlong lVar6;
  code *pcVar7;
  undefined8 uVar8;
  longlong *plVar9;
  undefined1 auStack_138 [32];
  code **local_118;
  code *local_108;
  undefined4 uStack_100;
  code *local_f8;
  undefined4 uStack_f0;
  undefined1 local_e8 [8];
  undefined1 local_e0 [8];
  undefined1 local_d8 [8];
  undefined1 local_d0 [8];
  undefined1 local_c8 [8];
  undefined1 local_c0 [8];
  undefined1 local_b8 [8];
  undefined1 local_b0 [8];
  undefined1 local_a8 [8];
  undefined1 local_a0 [8];
  undefined1 local_98 [8];
  undefined1 local_90 [8];
  undefined1 local_88 [8];
  undefined1 local_80 [8];
  undefined1 local_78 [16];
  code *local_68 [2];
  longlong local_58 [4];
  longlong *local_38;
  ulonglong local_28;
  
  local_28 = DAT_145909780 ^ (ulonglong)auStack_138;
  *(undefined8 *)(param_1 + 0x238) = param_2;
  if ((DAT_14595f490 == '\0') && (lVar2 = FUN_140d2ae50(), lVar2 != 0)) {
    FUN_1419cf770(lVar2 + 0x720);
    DAT_14595f490 = '\x01';
  }
  puVar5 = (undefined8 *)0x0;
  local_f8 = (code *)&LAB_141212f50;
  uStack_f0 = 0;
  FUN_140b1fbe0(param_1,*(undefined8 *)(param_1 + 0x248),param_1,&local_f8);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f8960;
  }
  FUN_141eb3490(&local_108,L"Move_Up",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f8960;
  }
  FUN_141eb3490(&local_108,L"Move_Up",1);
  FUN_141a8e170(param_1,&local_108,2);
  FUN_141a58370(param_1,&local_108,2,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f8b70;
  }
  FUN_141eb3490(&local_108,L"Move_Up",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f9160;
  }
  FUN_141eb3490(&local_108,L"Move_Down",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f9160;
  }
  FUN_141eb3490(&local_108,L"Move_Down",1);
  FUN_141a8e170(param_1,&local_108,2);
  FUN_141a58370(param_1,&local_108,2,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f9430;
  }
  FUN_141eb3490(&local_108,L"Move_Down",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_f8 = (code *)&LAB_141212f80;
  uStack_f0 = 0;
  FUN_140b1fbe0(param_1,*(undefined8 *)(param_1 + 0x260),param_1,&local_f8);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f8f10;
  }
  FUN_141eb3490(&local_108,L"Move_Left",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f8f10;
  }
  FUN_141eb3490(&local_108,L"Move_Left",1);
  FUN_141a8e170(param_1,&local_108,2);
  FUN_141a58370(param_1,&local_108,2,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f9890;
  }
  FUN_141eb3490(&local_108,L"Move_Left",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f98f0;
  }
  FUN_141eb3490(&local_108,L"Move_Right",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f98f0;
  }
  FUN_141eb3490(&local_108,L"Move_Right",1);
  FUN_141a8e170(param_1,&local_108,2);
  FUN_141a58370(param_1,&local_108,2,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f8ee0;
  }
  FUN_141eb3490(&local_108,L"Move_Right",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_118 = &local_f8;
  local_f8 = FUN_141214030;
  uStack_f0 = 0;
  FUN_1411c84e0(param_1,*(undefined8 *)(param_1 + 0x278),0,param_1);
  local_118 = &local_f8;
  local_f8 = FUN_141215480;
  uStack_f0 = 0;
  FUN_1411c84e0(param_1,*(undefined8 *)(param_1 + 0x278),1,param_1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9b10;
  }
  FUN_141eb3490(&local_108,L"Walk",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f95d0;
  }
  FUN_141eb3490(&local_108,L"Walk",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_e8,L"Controller_Btn_A",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9420;
  }
  FUN_141eb3490(&local_108,L"Step_Repeat",1);
  FUN_141a8e270(param_1,&local_108,2);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_e8,&local_108,2);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8b00;
  }
  FUN_141eb3490(&local_108,L"Step_Released",1);
  FUN_141a8e270(param_1,&local_108,1);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_e8,&local_108,1);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9420;
  }
  FUN_141eb3490(&local_108,L"MoveStep",1);
  FUN_141a8e170(param_1,&local_108,2);
  FUN_141a58370(param_1,&local_108,2,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8b00;
  }
  FUN_141eb3490(&local_108,L"MoveStep",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9420;
  }
  FUN_141eb3490(&local_108,L"Battle_Boost",1);
  FUN_141a8e170(param_1,&local_108,2);
  FUN_141a58370(param_1,&local_108,2,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8b00;
  }
  FUN_141eb3490(&local_108,L"Battle_Boost",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_118 = &local_f8;
  local_f8 = FUN_1412146f0;
  uStack_f0 = 0;
  FUN_1411c84e0(param_1,*(undefined8 *)(param_1 + 0x298),0,param_1);
  FUN_141eb3490(local_98,L"Controller_Btn_X",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f8ab0;
  }
  FUN_141eb3490(&local_108,L"Push_X_Pressed",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_98,&local_108,0);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_e0,L"Controller_Btn_RT",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f97a0;
  }
  FUN_141eb3490(&local_108,L"MoveMode_Pressed",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_e0,&local_108,0);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9590;
  }
  FUN_141eb3490(&local_108,L"MoveMode_Released",1);
  FUN_141a8e270(param_1,&local_108,1);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_e0,&local_108,1);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f97a0;
  }
  FUN_141eb3490(&local_108,L"Battle_MoveMode",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9590;
  }
  FUN_141eb3490(&local_108,L"Battle_MoveMode",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_d8,L"Controller_Btn_RB",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8930;
  }
  FUN_141eb3490(&local_108,L"Fly_Ascend_Pressed",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_d8,&local_108,0);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f91f0;
  }
  FUN_141eb3490(&local_108,L"Fly_Ascend_Released",1);
  FUN_141a8e270(param_1,&local_108,1);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_d8,&local_108,1);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8930;
  }
  FUN_141eb3490(&local_108,L"Fly_Ascend",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f91f0;
  }
  FUN_141eb3490(&local_108,L"Fly_Ascend",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_d0,L"Controller_Btn_RT",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8d40;
  }
  FUN_141eb3490(&local_108,L"Fly_Descend_Pressed",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_d0,&local_108,0);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9150;
  }
  FUN_141eb3490(&local_108,L"Fly_Descend_Released",1);
  FUN_141a8e270(param_1,&local_108,1);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_d0,&local_108,1);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8d40;
  }
  FUN_141eb3490(&local_108,L"Fly_Descend",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9150;
  }
  FUN_141eb3490(&local_108,L"Fly_Descend",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_c8,L"Controller_Btn_A",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8ce0;
  }
  FUN_141eb3490(&local_108,L"Jump_Pressed",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_c8,&local_108,0);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = _guard_check_icall;
  }
  FUN_141eb3490(&local_108,L"Jump_Released",1);
  FUN_141a8e270(param_1,&local_108,1);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_c8,&local_108,1);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8ce0;
  }
  FUN_141eb3490(&local_108,L"Jump",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = _guard_check_icall;
  }
  FUN_141eb3490(&local_108,L"Jump",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_c0,L"Controller_Btn_L3",1);
  FUN_141eb3490(local_90,L"Controller_Btn_A",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9970;
  }
  FUN_141eb3490(&local_108,L"v03_BoostAndBraking_Pressed",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_c0,&local_108,0);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f91e0;
  }
  FUN_141eb3490(&local_108,L"v03_BoostAndBraking_Released",1);
  FUN_141a8e270(param_1,&local_108,1);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_c0,&local_108,1);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9740;
  }
  FUN_141eb3490(&local_108,L"Braking",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_90,&local_108,0);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8a20;
  }
  FUN_141eb3490(&local_108,L"Boost",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f91e0;
  }
  FUN_141eb3490(&local_108,L"Boost",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_b8,L"Controller_Btn_Y",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f8c00;
  }
  FUN_141eb3490(&local_108,L"BoostUp_Pressed",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_b8,&local_108,0);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9120;
  }
  FUN_141eb3490(&local_108,L"BoostUp_Released",1);
  FUN_141a8e270(param_1,&local_108,1);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_b8,&local_108,1);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f8c00;
  }
  FUN_141eb3490(&local_108,L"BoostUp",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9120;
  }
  FUN_141eb3490(&local_108,L"BoostUp",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_88,L"Controller_Btn_Y",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8ba0;
  }
  FUN_141eb3490(&local_108,L"Fly_Rolling",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_88,&local_108,0);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8ba0;
  }
  FUN_141eb3490(&local_108,L"Fly_Rolling",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_b0,L"Controller_Btn_LT",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f92e0;
  }
  FUN_141eb3490(&local_108,L"TargetCursor_Pressed",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_b0,&local_108,0);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9880;
  }
  FUN_141eb3490(&local_108,L"TargetCursor_Released",1);
  FUN_141a8e270(param_1,&local_108,1);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_b0,&local_108,1);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f92e0;
  }
  FUN_141eb3490(&local_108,L"TargetCursor",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9880;
  }
  FUN_141eb3490(&local_108,L"TargetCursor",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_a8,L"Controller_Btn_B",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = _guard_check_icall;
  }
  FUN_141eb3490(&local_108,L"v03_KiBlust_Pressed",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_a8,&local_108,0);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = _guard_check_icall;
  }
  FUN_141eb3490(&local_108,L"v03_KiBlust_Released",1);
  FUN_141a8e270(param_1,&local_108,1);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_a8,&local_108,1);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_a0,L"Controller_Btn_X",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8eb0;
  }
  FUN_141eb3490(&local_108,L"v04_KiBlust_Pressed",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_a0,&local_108,0);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8bc0;
  }
  FUN_141eb3490(&local_108,L"v04_KiBlust_Released",1);
  FUN_141a8e270(param_1,&local_108,1);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_a0,&local_108,1);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8eb0;
  }
  FUN_141eb3490(&local_108,L"KiBlust",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8bc0;
  }
  FUN_141eb3490(&local_108,L"KiBlust",1);
  FUN_141a8e170(param_1,&local_108,1);
  FUN_141a58370(param_1,&local_108,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar9 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar9 = local_38;
    }
    (**(code **)(*plVar9 + 0x10))(plVar9,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_118 = &local_f8;
  local_f8 = FUN_141214970;
  uStack_f0 = 0;
  FUN_1411c84e0(param_1,*(undefined8 *)(param_1 + 0x2f0),0,param_1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f9b80;
  }
  FUN_141eb3490(&local_108,L"Ride",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  FUN_1408f46c0(local_68);
  local_118 = &local_f8;
  local_f8 = FUN_141214890;
  uStack_f0 = 0;
  FUN_1411c84e0(param_1,*(undefined8 *)(param_1 + 0x2f8),0,param_1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f9300;
  }
  FUN_141eb3490(&local_108,L"Ride_Off",1);
  FUN_141a8e170(param_1,&local_108,0);
  FUN_141a58370(param_1,&local_108,0,local_68);
  FUN_1408f46c0(local_68);
  FUN_141eb3490(local_80,L"Controller_Btn_RT",1);
  local_38 = (longlong *)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  puVar4 = puVar5;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = param_1;
    *puVar3 = &PTR_FUN_143969680;
    puVar4 = puVar3;
  }
  if (puVar4 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f9690;
  }
  FUN_141eb3490(&local_108,L"NimbusMove",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_80,&local_108,0);
  FUN_1408f46c0(local_68);
  local_118 = &local_f8;
  local_f8 = (code *)&LAB_141214960;
  uStack_f0 = 0;
  FUN_1411c84e0(param_1,*(undefined8 *)(param_1 + 0x360),0,param_1);
  FUN_141eb3490(local_78,L"Controller_Btn_R3",1);
  local_38 = (longlong *)0x0;
  puVar4 = (undefined8 *)FUN_1407e5ee0(0x10,local_58);
  if (puVar4 != (undefined8 *)0x0) {
    puVar4[1] = param_1;
    *puVar4 = &PTR_FUN_143969680;
    puVar5 = puVar4;
  }
  if (puVar5 != (undefined8 *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8e50;
  }
  FUN_141eb3490(&local_108,L"ResetCamera",1);
  FUN_141a8e270(param_1,&local_108,0);
  local_118 = local_68;
  FUN_141a584f0(param_1,local_78,&local_108,0);
  FUN_1408f46c0(local_68);
  lVar2 = *(longlong *)(param_1 + 0xf0);
  if (lVar2 != 0) {
    lVar6 = FUN_141863300();
    if ((*(int *)(*(longlong *)(lVar2 + 0x10) + 0x90) < *(int *)(lVar6 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar2 + 0x10) + 0x88) + (longlong)*(int *)(lVar6 + 0x90) * 8)
        != lVar6 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) {
      pcVar7 = (code *)FUN_141df5850(0,0x20);
      uStack_f0 = 2;
      local_f8 = pcVar7;
      if (pcVar7 != (code *)0x0) {
        *(longlong *)(pcVar7 + 8) = param_1;
        *(undefined ***)pcVar7 = &PTR_FUN_143d3b2c0;
        uVar8 = FUN_141db8700();
        *(undefined8 *)(pcVar7 + 0x18) = uVar8;
        *(undefined ***)pcVar7 = &PTR_FUN_143d3b310;
        FUN_1408b2ad0(lVar2 + 0x6b8,&local_108,&local_f8);
      }
      FUN_1407e59d0(&local_f8);
    }
  }
  uStack_100 = 0;
  local_108 = FUN_1412130e0;
  local_f8 = (code *)FUN_141df5850(0,0x30);
  uStack_f0 = 3;
  if (local_f8 != (code *)0x0) {
    FUN_1411f0fa0(local_f8,param_1,&local_108);
  }
  FUN_1409e14e0(&DAT_1456de4a0,&local_108,&local_f8);
  FUN_1407e59d0(&local_f8);
  return;
}



// ================= Controller_Btn_B <- 1411ff1d7  FUN_1411ff170 @ 1411ff170  size=0x403 =================
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1411ff170(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  longlong *plVar4;
  longlong lVar5;
  undefined8 *puVar6;
  longlong **pplVar7;
  undefined1 auStack_d8 [32];
  undefined1 **local_b8;
  undefined1 local_a8 [8];
  undefined1 local_a0 [8];
  undefined1 local_98 [8];
  undefined8 local_90;
  undefined4 uStack_88;
  undefined4 uStack_84;
  undefined1 *local_78;
  undefined8 local_70;
  longlong *local_68;
  int local_60;
  undefined8 local_58;
  undefined4 local_50;
  longlong **local_48;
  undefined8 local_40;
  undefined1 local_38;
  ulonglong local_28;
  
  local_28 = DAT_145909780 ^ (ulonglong)auStack_d8;
  *(undefined8 *)(param_1 + 0x238) = param_2;
  if ((DAT_14595f490 == '\0') && (lVar1 = FUN_140d2ae50(), lVar1 != 0)) {
    FUN_1419cf770(lVar1 + 0x720);
    DAT_14595f490 = '\x01';
  }
  lVar1 = FUN_140d24ea0();
  if (lVar1 != 0) {
    FUN_141eb3490(local_a0,L"Controller_Btn_B",1);
    puVar6 = (undefined8 *)0x0;
    local_48 = (longlong **)0x0;
    puVar2 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
    puVar3 = puVar6;
    if (puVar2 != (undefined8 *)0x0) {
      puVar2[1] = lVar1;
      *puVar2 = &PTR_FUN_143969680;
      puVar3 = puVar2;
    }
    if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
      local_78 = &LAB_1411f90b0;
    }
    FUN_141eb3490(local_a8,L"PushPad_B",1);
    FUN_141a8e270(param_1,local_a8,0);
    local_b8 = &local_78;
    FUN_141a584f0(param_1,local_a0,local_a8,0);
    if (local_78 != (undefined1 *)0x0) {
      pplVar7 = &local_68;
      if (local_48 != (longlong **)0x0) {
        pplVar7 = local_48;
      }
      (*(code *)(*pplVar7)[2])(pplVar7,0);
    }
    if (local_48 != (longlong **)0x0) {
      FUN_141de9200();
    }
    local_48 = (longlong **)0x0;
    puVar2 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
    puVar3 = puVar6;
    if (puVar2 != (undefined8 *)0x0) {
      puVar2[1] = lVar1;
      *puVar2 = &PTR_FUN_143969680;
      puVar3 = puVar2;
    }
    if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
      local_78 = &LAB_1411f90b0;
    }
    FUN_141eb3490(local_a8,L"BaseBall_Swing",1);
    FUN_141a8e170(param_1,local_a8,0);
    FUN_141a58370(param_1,local_a8,0,&local_78);
    if (local_78 != (undefined1 *)0x0) {
      pplVar7 = &local_68;
      if (local_48 != (longlong **)0x0) {
        pplVar7 = local_48;
      }
      (*(code *)(*pplVar7)[2])(pplVar7,0);
    }
    if (local_48 != (longlong **)0x0) {
      FUN_141de9200();
    }
    local_78 = (undefined1 *)((ulonglong)local_78 & 0xfffffffffffffefc);
    local_90 = FUN_1411696a0;
    uStack_88 = 0;
    local_78._0_3_ = (uint3)(ushort)local_78;
    puVar3 = (undefined8 *)FUN_141eb3410(local_a8,"PauseMenu",1);
    local_68 = (longlong *)0x0;
    local_60 = 0;
    local_58 = 0;
    local_70 = *puVar3;
    local_50 = 0;
    FUN_1420575e0(&local_48,0);
    local_40 = 0;
    local_38 = 0;
    FUN_1411ef520(&local_68);
    local_38 = 1;
    plVar4 = (longlong *)FUN_141df5850(0,0x30);
    if (plVar4 == (longlong *)0x0) {
      if ((local_60 != 0) && (local_68 != (longlong *)0x0)) {
        (**(code **)(*local_68 + 0x30))(local_68,0);
        if (local_68 != (longlong *)0x0) {
          local_68 = (longlong *)FUN_141df5850(local_68,0,0);
        }
        local_60 = 0;
      }
    }
    else {
      *plVar4 = (longlong)&PTR_FUN_143968d20;
      FUN_1420575e0(plVar4 + 1,lVar1);
      *(undefined4 *)(plVar4 + 2) = (undefined4)local_90;
      *(undefined4 *)((longlong)plVar4 + 0x14) = local_90._4_4_;
      *(undefined4 *)(plVar4 + 3) = uStack_88;
      *(undefined4 *)((longlong)plVar4 + 0x1c) = uStack_84;
      lVar5 = FUN_141db8700();
      plVar4[5] = lVar5;
      *plVar4 = (longlong)&PTR_FUN_143968d70;
      (*(code *)PTR_FUN_143968da8)(plVar4,&local_68);
    }
    if (plVar4 != (longlong *)0x0) {
      (**(code **)(*plVar4 + 0x30))(plVar4,0);
      lVar5 = FUN_141df5850(plVar4,0,0);
      if (lVar5 != 0) {
        FUN_141de9200(lVar5);
      }
    }
    FUN_142c367c0(param_1,&local_78);
    FUN_1407e59d0(&local_58);
    FUN_1407e59d0(&local_68);
    FUN_141eb3490(local_98,L"Controller_Btn_Start",1);
    local_48 = (longlong **)0x0;
    puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,&local_68);
    if (puVar3 != (undefined8 *)0x0) {
      puVar3[1] = lVar1;
      *puVar3 = &PTR_FUN_143969680;
      puVar6 = puVar3;
    }
    if (puVar6 != (undefined8 *)0xfffffffffffffff8) {
      local_78 = &LAB_1411f99f0;
    }
    FUN_141eb3490(local_a8,L"PauseMenu",1);
    FUN_141a8e270(param_1,local_a8,0);
    local_b8 = &local_78;
    FUN_141a584f0(param_1,local_98,local_a8,0);
    if (local_78 != (undefined1 *)0x0) {
      pplVar7 = &local_68;
      if (local_48 != (longlong **)0x0) {
        pplVar7 = local_48;
      }
      (*(code *)(*pplVar7)[2])(pplVar7,0);
    }
    if (local_48 != (longlong **)0x0) {
      FUN_141de9200();
    }
  }
  return;
}



// ================= Controller_Btn_B <- 14120ed59  FUN_14120d920 @ 14120d920  size=0x1658 =================
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14120d920(longlong param_1)

{
  longlong lVar1;
  bool bVar2;
  undefined4 uVar3;
  ulonglong uVar4;
  ulonglong uVar5;
  longlong lVar6;
  undefined8 uVar7;
  longlong lVar8;
  longlong *plVar9;
  undefined8 *puVar10;
  longlong *plVar11;
  longlong *plVar12;
  undefined1 auStack_158 [32];
  code **local_138;
  undefined1 local_128 [16];
  code *local_118;
  undefined8 uStack_110;
  undefined4 local_108;
  uint uStack_104;
  undefined4 local_f8;
  undefined4 uStack_f4;
  undefined4 uStack_f0;
  uint uStack_ec;
  undefined1 local_e8 [8];
  undefined1 local_e0 [8];
  undefined1 local_d8 [16];
  code *local_c8;
  undefined8 uStack_c0;
  ulonglong local_b8;
  undefined1 local_a8 [8];
  undefined1 local_a0 [8];
  undefined1 local_98 [8];
  undefined1 local_90 [8];
  undefined4 local_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  ulonglong local_78;
  code *local_68 [2];
  longlong local_58 [4];
  longlong *local_38;
  ulonglong local_28;
  
  local_28 = DAT_145909780 ^ (ulonglong)auStack_158;
  lVar1 = *(longlong *)(param_1 + 0xf0);
  plVar12 = (longlong *)0x0;
  if (lVar1 != 0) {
    lVar6 = FUN_141911200();
    if ((*(int *)(*(longlong *)(lVar1 + 0x10) + 0x90) < *(int *)(lVar6 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar1 + 0x10) + 0x88) + (longlong)*(int *)(lVar6 + 0x90) * 8)
        != lVar6 + 0x88)) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    if ((bVar2) && (plVar11 = *(longlong **)(lVar1 + 0x368), plVar11 != (longlong *)0x0)) {
      uVar7 = FUN_141912880();
      lVar6 = (**(code **)(*plVar11 + 0x600))(plVar11,uVar7);
      if (lVar6 != 0) {
        lVar8 = FUN_141911560();
        if ((*(int *)(plVar11[2] + 0x90) < *(int *)(lVar8 + 0x90)) ||
           (*(longlong *)(*(longlong *)(plVar11[2] + 0x88) + (longlong)*(int *)(lVar8 + 0x90) * 8)
            != lVar8 + 0x88)) {
          bVar2 = false;
        }
        else {
          bVar2 = true;
        }
        plVar9 = plVar11;
        if (!bVar2) {
          plVar9 = plVar12;
        }
        *(longlong **)(param_1 + 0x280) = plVar9;
        uVar7 = FUN_141911fb0();
        uVar7 = (**(code **)(*plVar11 + 0x600))(plVar11,uVar7);
        *(longlong *)(param_1 + 0x298) = lVar6;
        *(undefined8 *)(param_1 + 0x288) = uVar7;
        *(longlong *)(param_1 + 0x290) = lVar1;
        FUN_142f7dfc0(lVar1,param_1);
        *(undefined8 *)(param_1 + 0x2a0) = *(undefined8 *)(lVar1 + 0x3d8);
      }
    }
  }
  local_118 = (code *)&LAB_141212f60;
  uStack_110 = (ulonglong)uStack_110._4_4_ << 0x20;
  FUN_140b1fbe0(param_1,*(undefined8 *)(param_1 + 0x260),param_1,&local_118);
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f9a90;
  }
  FUN_141eb3490(local_128,L"Move_Up",1);
  FUN_141a58370(param_1,local_128,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f9980;
  }
  FUN_141eb3490(local_128,L"Move_Up",1);
  FUN_141a58370(param_1,local_128,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f97d0;
  }
  FUN_141eb3490(local_128,L"Move_Down",1);
  FUN_141a58370(param_1,local_128,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f90c0;
  }
  FUN_141eb3490(local_128,L"Move_Down",1);
  FUN_141a58370(param_1,local_128,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_118 = (code *)&LAB_141212f90;
  uStack_110 = uStack_110 & 0xffffffff00000000;
  FUN_140b1fbe0(param_1,*(undefined8 *)(param_1 + 0x268),param_1,&local_118);
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f94a0;
  }
  FUN_141eb3490(local_128,L"Move_Left",1);
  FUN_141a58370(param_1,local_128,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f9460;
  }
  FUN_141eb3490(local_128,L"Move_Left",1);
  FUN_141a58370(param_1,local_128,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f9030;
  }
  FUN_141eb3490(local_128,L"Move_Right",1);
  FUN_141a58370(param_1,local_128,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f8cf0;
  }
  FUN_141eb3490(local_128,L"Move_Right",1);
  FUN_141a58370(param_1,local_128,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  uVar4 = uStack_110;
  uStack_110 = uStack_110 & 0xffffffff00000000;
  uVar5 = uStack_110;
  local_118 = _guard_check_icall;
  uStack_110._4_4_ = SUB84(uVar4,4);
  local_f8 = 0x407e2f60;
  uStack_f4 = 1;
  uStack_f0 = 0;
  uStack_ec = uStack_110._4_4_;
  uStack_110 = uVar5;
  puVar10 = (undefined8 *)FUN_141eb3410(&local_118,"PushPad_Y",1);
  local_138 = (code **)&local_f8;
  FUN_1411c84e0(param_1,*puVar10,0,param_1);
  uVar4 = uStack_110;
  uStack_110 = uStack_110 & 0xffffffff00000000;
  uVar5 = uStack_110;
  local_118 = _guard_check_icall;
  uStack_110._4_4_ = SUB84(uVar4,4);
  local_f8 = 0x407e2f60;
  uStack_f4 = 1;
  uStack_f0 = 0;
  uStack_ec = uStack_110._4_4_;
  uStack_110 = uVar5;
  puVar10 = (undefined8 *)FUN_141eb3410(&local_118,"PushPad_X",1);
  local_138 = (code **)&local_f8;
  FUN_1411c84e0(param_1,*puVar10,0,param_1);
  local_138 = &local_118;
  local_118 = (code *)&LAB_141213270;
  uStack_110._0_4_ = 0;
  FUN_1411c84e0(param_1,*(undefined8 *)(param_1 + 0x248),0,param_1);
  uStack_110._0_4_ = 0;
  local_118 = _guard_check_icall;
  local_f8 = 0x407e2f60;
  uStack_f4 = 1;
  uStack_f0 = 0;
  uStack_ec = uStack_110._4_4_;
  puVar10 = (undefined8 *)FUN_141eb3410(&local_118,"PushPad_RT",1);
  local_138 = (code **)&local_f8;
  FUN_1411c84e0(param_1,*puVar10,0,param_1);
  uStack_110._0_4_ = 0;
  local_118 = _guard_check_icall;
  local_f8 = 0x407e2f60;
  uStack_f4 = 1;
  uStack_f0 = 0;
  uStack_ec = uStack_110._4_4_;
  puVar10 = (undefined8 *)FUN_141eb3410(&local_118,"PushPad_RT",1);
  local_138 = (code **)&local_f8;
  FUN_1411c84e0(param_1,*puVar10,1,param_1);
  local_118 = FUN_141214660;
  uStack_110 = (ulonglong)uStack_110._4_4_ << 0x20;
  local_138 = &local_118;
  FUN_1411c84e0(param_1,*(undefined8 *)(param_1 + 0x250),0,param_1);
  FUN_141eb3490(local_e8,L"Controller_Btn_A",1);
  FUN_141eb3490(local_a8,L"Controller_Btn_Y",1);
  FUN_141eb3490(local_a0,L"Controller_Btn_X",1);
  FUN_141eb3490(local_e0,L"Controller_Btn_RT",1);
  FUN_141eb3490(local_98,L"Controller_Btn_R3",1);
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8ed0;
  }
  FUN_141eb3490(local_128,L"Jump_Pressed",1);
  local_138 = local_68;
  FUN_141a584f0(param_1,local_e8,local_128,0);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = _guard_check_icall;
  }
  FUN_141eb3490(local_128,L"Jump_Released",1);
  local_138 = local_68;
  FUN_141a584f0(param_1,local_e8,local_128,1);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = _guard_check_icall;
  }
  FUN_141eb3490(local_128,L"PushPad_Y",1);
  local_138 = local_68;
  FUN_141a584f0(param_1,local_a8,local_128,0);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8e70;
  }
  FUN_141eb3490(local_128,L"PushPad_X",1);
  local_138 = local_68;
  FUN_141a584f0(param_1,local_a0,local_128,0);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = _guard_check_icall;
  }
  FUN_141eb3490(local_128,L"PushPad_RT_Pressed",1);
  local_138 = local_68;
  FUN_141a584f0(param_1,local_e0,local_128,0);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = _guard_check_icall;
  }
  FUN_141eb3490(local_128,L"PushPad_RT_Released",1);
  local_138 = local_68;
  FUN_141a584f0(param_1,local_e0,local_128,1);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8da0;
  }
  FUN_141eb3490(local_128,L"ResetCamera",1);
  local_138 = local_68;
  FUN_141a584f0(param_1,local_98,local_128,0);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8ed0;
  }
  FUN_141eb3490(local_128,L"Jump",1);
  FUN_141a58370(param_1,local_128,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = _guard_check_icall;
  }
  FUN_141eb3490(local_128,L"Jump",1);
  FUN_141a58370(param_1,local_128,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f9a80;
  }
  FUN_141eb3490(local_128,L"Ride_Off",1);
  FUN_141a58370(param_1,local_128,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8e70;
  }
  FUN_141eb3490(local_128,L"MineralRadar",1);
  FUN_141a58370(param_1,local_128,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(local_d8,L"Controller_Btn_Back",1);
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8f90;
  }
  FUN_141eb3490(local_128,L"TwinFoot_MapChange_Pressed",1);
  local_138 = local_68;
  FUN_141a584f0(param_1,local_d8,local_128,0);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f8db0;
  }
  FUN_141eb3490(local_128,L"TwinFoot_MapChange_Released",1);
  local_138 = local_68;
  FUN_141a584f0(param_1,local_d8,local_128,1);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = (code *)&LAB_1411f8f90;
  }
  FUN_141eb3490(local_128,L"MapChange",1);
  FUN_141a58370(param_1,local_128,0,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411f8db0;
  }
  FUN_141eb3490(local_128,L"MapChange",1);
  FUN_141a58370(param_1,local_128,1,local_68);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  uVar4 = uStack_110;
  uStack_110 = uStack_110 & 0xffffffff00000000;
  uVar5 = uStack_110;
  local_118 = FUN_1411e6c50;
  uStack_110._4_4_ = SUB84(uVar4,4);
  local_f8 = 0x411e6c50;
  uStack_f4 = 1;
  uStack_f0 = 0;
  uStack_ec = uStack_110._4_4_;
  uStack_110 = uVar5;
  puVar10 = (undefined8 *)FUN_141eb3410(&local_118,"PauseMenu",1);
  local_138 = (code **)&local_f8;
  FUN_1411c84e0(param_1,*puVar10,0,param_1);
  FUN_141eb3490(local_90,L"Controller_Btn_Start",1);
  local_38 = (longlong *)0x0;
  plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
  plVar11 = plVar12;
  if (plVar9 != (longlong *)0x0) {
    plVar9[1] = param_1;
    *plVar9 = (longlong)&PTR_FUN_143969680;
    plVar11 = plVar9;
  }
  if (plVar11 != (longlong *)0xfffffffffffffff8) {
    local_68[0] = FUN_1411d1840;
  }
  FUN_141eb3490(local_128,L"PauseMenu",1);
  local_138 = local_68;
  FUN_141a584f0(param_1,local_90,local_128,0);
  if (local_68[0] != (code *)0x0) {
    plVar11 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar11 = local_38;
    }
    (**(code **)(*plVar11 + 0x10))(plVar11,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x280) + 0x780);
  if (lVar1 != 0) {
    lVar6 = FUN_141758410();
    if ((*(int *)(*(longlong *)(lVar1 + 0x10) + 0x90) < *(int *)(lVar6 + 0x90)) ||
       (*(longlong *)
         (*(longlong *)(*(longlong *)(lVar1 + 0x10) + 0x88) + (longlong)*(int *)(lVar6 + 0x90) * 8)
        != lVar6 + 0x88)) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    if (bVar2) {
      uStack_110 = 0;
      local_118 = FUN_140f15730;
      local_108 = 0;
      local_b8 = (ulonglong)uStack_104 << 0x20;
      local_c8 = FUN_140f15730;
      uStack_c0 = 0;
      puVar10 = (undefined8 *)FUN_141eb3490(local_128,L"FieldAction",1);
      local_138 = &local_c8;
      FUN_1411c8360(param_1,*puVar10,0,lVar1);
      uStack_c0 = 0;
      local_c8 = FUN_140f158b0;
      local_b8 = local_b8 & 0xffffffff00000000;
      local_88 = 0x40f158b0;
      uStack_84 = 1;
      uStack_80 = 0;
      uStack_7c = 0;
      local_78 = local_b8;
      puVar10 = (undefined8 *)FUN_141eb3490(local_128,L"FieldSubAction",1);
      local_138 = (code **)&local_88;
      FUN_1411c8360(param_1,*puVar10,0,lVar1);
      FUN_141eb3490(&local_f8,L"Controller_Btn_B",1);
      FUN_141eb3490(&local_118,L"Controller_Btn_Y",1);
      local_38 = (longlong *)0x0;
      plVar9 = (longlong *)FUN_1407e5ee0(0x10,local_58);
      plVar11 = plVar12;
      if (plVar9 != (longlong *)0x0) {
        plVar9[1] = lVar1;
        *plVar9 = (longlong)&PTR_FUN_143969680;
        plVar11 = plVar9;
      }
      if (plVar11 != (longlong *)0xfffffffffffffff8) {
        local_68[0] = (code *)&LAB_1411d0e60;
      }
      FUN_141eb3490(local_128,L"FieldAction",1);
      FUN_141a8e270(param_1,local_128,0);
      local_138 = local_68;
      FUN_141a584f0(param_1,&local_f8,local_128,0);
      if (local_68[0] != (code *)0x0) {
        plVar11 = local_58;
        if (local_38 != (longlong *)0x0) {
          plVar11 = local_38;
        }
        (**(code **)(*plVar11 + 0x10))(plVar11,0);
      }
      if (local_38 != (longlong *)0x0) {
        FUN_141de9200();
      }
      local_38 = (longlong *)0x0;
      plVar11 = (longlong *)FUN_1407e5ee0(0x10,local_58);
      if (plVar11 != (longlong *)0x0) {
        plVar11[1] = lVar1;
        *plVar11 = (longlong)&PTR_FUN_143969680;
        plVar12 = plVar11;
      }
      if (plVar12 != (longlong *)0xfffffffffffffff8) {
        local_68[0] = (code *)&LAB_1411d1170;
      }
      FUN_141eb3490(local_128,L"FieldSubAction",1);
      FUN_141a8e270(param_1,local_128,0);
      local_138 = local_68;
      FUN_141a584f0(param_1,&local_118,local_128,0);
      if (local_68[0] != (code *)0x0) {
        plVar12 = local_58;
        if (local_38 != (longlong *)0x0) {
          plVar12 = local_38;
        }
        (**(code **)(*plVar12 + 0x10))(plVar12,0);
      }
      if (local_38 != (longlong *)0x0) {
        FUN_141de9200();
      }
    }
  }
  *(undefined8 *)(param_1 + 0x2a8) = DAT_145a8af10;
  uVar3 = DAT_145a8af18;
  *(undefined4 *)(param_1 + 0x2b0) = DAT_145a8af18;
  *(undefined8 *)(param_1 + 0x2b4) = *(undefined8 *)(param_1 + 0x2a8);
  *(undefined4 *)(param_1 + 700) = uVar3;
  *(undefined4 *)(param_1 + 0x2d8) = 0;
  if (*(int *)(param_1 + 0x2dc) != 0) {
    FUN_140827be0(param_1 + 0x2d0,0);
  }
  *(undefined4 *)(param_1 + 0x2e8) = 0;
  if (*(int *)(param_1 + 0x2ec) != 0) {
    FUN_140827be0(param_1 + 0x2e0,0);
  }
  return;
}



// ================= Controller_Btn_B <- 14120935d  FUN_141207c60 @ 141207c60  size=0x17c1 =================
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141207c60(longlong param_1,undefined8 param_2)

{
  char cVar1;
  longlong lVar2;
  undefined8 *puVar3;
  undefined8 uVar4;
  byte *pbVar5;
  undefined8 *puVar6;
  undefined8 *puVar7;
  longlong *plVar8;
  undefined1 auStack_b8 [32];
  code **local_98;
  undefined8 local_90;
  undefined1 local_88 [8];
  undefined1 local_80 [8];
  code *local_78 [2];
  longlong local_68 [4];
  longlong *local_48;
  ulonglong local_38;
  
  local_38 = DAT_145909780 ^ (ulonglong)auStack_b8;
  *(undefined8 *)(param_1 + 0x238) = param_2;
  if ((DAT_14595f490 == '\0') && (lVar2 = FUN_140d2ae50(), lVar2 != 0)) {
    FUN_1419cf770(lVar2 + 0x720);
    DAT_14595f490 = '\x01';
  }
  cVar1 = FUN_140d2f9d0();
  if ((cVar1 != '\0') && (*(longlong *)(param_1 + 0x238) != 0)) {
    *(undefined8 *)(param_1 + 0x250) = *(undefined8 *)(*(longlong *)(param_1 + 0x238) + 0x1188);
  }
  puVar7 = (undefined8 *)0x0;
  if (*(longlong *)(param_1 + 0x248) == 0) {
    puVar3 = (undefined8 *)FUN_141eb3410(local_88,"AT_UICmnInput",1);
    local_90 = 0;
    local_98 = (code **)((ulonglong)local_98 & 0xffffffffffffff00);
    uVar4 = FUN_1411f0cf0(param_1,*puVar3,0,0);
    *(undefined8 *)(param_1 + 0x248) = uVar4;
  }
  lVar2 = *(longlong *)(param_1 + 0xf8);
  if (lVar2 == 0) {
    lVar2 = FUN_142c17e40(param_1);
  }
  FUN_1414d14d0(*(undefined8 *)(param_1 + 0x248),lVar2);
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_Up",1);
  local_98 = (code **)FUN_1414d8fe0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_Down",1);
  local_98 = (code **)FUN_1414d8ec0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_Right",1);
  local_98 = (code **)FUN_1414d8f80;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_Left",1);
  local_98 = (code **)FUN_1414d8f20;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_Up",1);
  local_98 = (code **)&LAB_1414d9d50;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_Down",1);
  local_98 = (code **)&LAB_1414d9c30;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_Right",1);
  local_98 = (code **)&LAB_1414d9cf0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_Left",1);
  local_98 = (code **)&LAB_1414d9c90;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_Up",1);
  local_98 = (code **)&LAB_1414d9540;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,1,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_Down",1);
  local_98 = (code **)&LAB_1414d9510;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,1,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_Left",1);
  local_98 = (code **)&LAB_1414d9520;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,1,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_Right",1);
  local_98 = (code **)&LAB_1414d9530;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,1,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SL_Up",1);
  local_98 = (code **)&LAB_1414d8e40;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SL_Down",1);
  local_98 = (code **)&LAB_1414d8cc0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SL_Right",1);
  local_98 = (code **)&LAB_1414d8dc0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SL_Left",1);
  local_98 = (code **)&LAB_1414d8d40;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SL_Up",1);
  local_98 = (code **)&LAB_1414d9bb0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SL_Down",1);
  local_98 = (code **)&LAB_1414d9a30;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SL_Right",1);
  local_98 = (code **)&LAB_1414d9b30;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SL_Left",1);
  local_98 = (code **)&LAB_1414d9ab0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SR_Up",1);
  local_98 = (code **)&LAB_1414d9070;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SR_Down",1);
  local_98 = (code **)&LAB_1414d9040;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SR_Right",1);
  local_98 = (code **)&LAB_1414d9060;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SR_Left",1);
  local_98 = (code **)&LAB_1414d9050;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SR_Up",1);
  local_98 = (code **)&LAB_1414d9070;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SR_Down",1);
  local_98 = (code **)&LAB_1414d9040;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SR_Right",1);
  local_98 = (code **)&LAB_1414d9060;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushPad_SR_Left",1);
  local_98 = (code **)&LAB_1414d9050;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_A",1);
  local_98 = (code **)FUN_1414d8a00;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_B",1);
  local_98 = (code **)FUN_1414d8a40;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushNotPad_Decide",1);
  local_98 = (code **)&LAB_1414d8ad0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"PushNotPad_Cancel",1);
  local_98 = (code **)&LAB_1414d8ac0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_Y",1);
  local_98 = (code **)&LAB_1414d8cb0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_X",1);
  local_98 = (code **)&LAB_1414d8ca0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_LB",1);
  local_98 = (code **)&LAB_1414d8ae0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_RB",1);
  local_98 = (code **)&LAB_1414d8bb0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_LT",1);
  local_98 = (code **)&LAB_1414d8b40;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_RT",1);
  local_98 = (code **)&LAB_1414d8c10;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_LB",1);
  local_98 = (code **)&LAB_1414d98b0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_RB",1);
  local_98 = (code **)&LAB_1414d9970;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_LT",1);
  local_98 = (code **)&LAB_1414d9910;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_RT",1);
  local_98 = (code **)&LAB_1414d99d0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,2,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_Start",1);
  local_98 = (code **)&LAB_1414d8c90;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Menu_PushPad_Select",1);
  local_98 = (code **)&LAB_1414d8c80;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"InputLeftStickXAxis",1);
  pbVar5 = (byte *)FUN_1411f05c0(param_1,*puVar3,uVar4,&LAB_1414d6300);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"InputLeftStickYAxis",1);
  pbVar5 = (byte *)FUN_1411f05c0(param_1,*puVar3,uVar4,&LAB_1414d6310);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"InputRightStickXAxis",1);
  pbVar5 = (byte *)FUN_1411f05c0(param_1,*puVar3,uVar4,&LAB_1414d6b90);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"InputRightStickYAxis",1);
  pbVar5 = (byte *)FUN_1411f05c0(param_1,*puVar3,uVar4,&LAB_1414d6c10);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Pad_Left",1);
  pbVar5 = (byte *)FUN_1411f05c0(param_1,*puVar3,uVar4,&LAB_1414d3840);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Pad_Right",1);
  pbVar5 = (byte *)FUN_1411f05c0(param_1,*puVar3,uVar4,&LAB_1414d3840);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Pad_Up",1);
  pbVar5 = (byte *)FUN_1411f05c0(param_1,*puVar3,uVar4,&LAB_1414d3870);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Pad_Down",1);
  pbVar5 = (byte *)FUN_1411f05c0(param_1,*puVar3,uVar4,&LAB_1414d3870);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Mouse_RightButton",1);
  local_98 = (code **)&LAB_1414d8c70;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Mouse_LeftButton",1);
  local_98 = (code **)&LAB_1414d8ba0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Input_BattleRush_Right",1);
  local_98 = (code **)&LAB_1414d8aa0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Input_BattleRush_Down",1);
  local_98 = (code **)&LAB_1414d8a80;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Input_BattleRush_Left",1);
  local_98 = (code **)&LAB_1414d8a90;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  *pbVar5 = *pbVar5 | 2;
  uVar4 = *(undefined8 *)(param_1 + 0x248);
  puVar3 = (undefined8 *)FUN_141eb3410(local_88,"Input_BattleRush_Up",1);
  local_98 = (code **)&LAB_1414d8ab0;
  pbVar5 = (byte *)FUN_1411f0460(param_1,*puVar3,0,uVar4);
  local_48 = (longlong *)0x0;
  *pbVar5 = *pbVar5 | 2;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
  puVar3 = puVar7;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_78[0] = FUN_1411f89e0;
  }
  FUN_141eb3490(local_80,L"Move_Up",1);
  FUN_141a8e170(param_1,local_80,0);
  FUN_141a58370(param_1,local_80,0,local_78);
  if (local_78[0] != (code *)0x0) {
    plVar8 = local_68;
    if (local_48 != (longlong *)0x0) {
      plVar8 = local_48;
    }
    (**(code **)(*plVar8 + 0x10))(plVar8,0);
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong *)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
  puVar3 = puVar7;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_78[0] = FUN_1411f8a80;
  }
  FUN_141eb3490(local_80,L"Move_Down",1);
  FUN_141a8e170(param_1,local_80,0);
  FUN_141a58370(param_1,local_80,0,local_78);
  if (local_78[0] != (code *)0x0) {
    plVar8 = local_68;
    if (local_48 != (longlong *)0x0) {
      plVar8 = local_48;
    }
    (**(code **)(*plVar8 + 0x10))(plVar8,0);
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong *)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
  puVar3 = puVar7;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_78[0] = FUN_1411f8b20;
  }
  FUN_141eb3490(local_80,L"Move_Left",1);
  FUN_141a8e170(param_1,local_80,0);
  FUN_141a58370(param_1,local_80,0,local_78);
  if (local_78[0] != (code *)0x0) {
    plVar8 = local_68;
    if (local_48 != (longlong *)0x0) {
      plVar8 = local_48;
    }
    (**(code **)(*plVar8 + 0x10))(plVar8,0);
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong *)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
  puVar3 = puVar7;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_78[0] = FUN_1411f9520;
  }
  FUN_141eb3490(local_80,L"Move_Right",1);
  FUN_141a8e170(param_1,local_80,0);
  FUN_141a58370(param_1,local_80,0,local_78);
  if (local_78[0] != (code *)0x0) {
    plVar8 = local_68;
    if (local_48 != (longlong *)0x0) {
      plVar8 = local_48;
    }
    (**(code **)(*plVar8 + 0x10))(plVar8,0);
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong *)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
  puVar3 = puVar7;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_78[0] = (code *)&DAT_1411f8e40;
  }
  FUN_141eb3490(local_80,L"Move_Up",1);
  FUN_141a8e170(param_1,local_80,2);
  FUN_141a58370(param_1,local_80,2,local_78);
  if (local_78[0] != (code *)0x0) {
    plVar8 = local_68;
    if (local_48 != (longlong *)0x0) {
      plVar8 = local_48;
    }
    (**(code **)(*plVar8 + 0x10))(plVar8,0);
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong *)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
  puVar3 = puVar7;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_78[0] = (code *)&DAT_1411f8ec0;
  }
  FUN_141eb3490(local_80,L"Move_Down",1);
  FUN_141a8e170(param_1,local_80,2);
  FUN_141a58370(param_1,local_80,2,local_78);
  if (local_78[0] != (code *)0x0) {
    plVar8 = local_68;
    if (local_48 != (longlong *)0x0) {
      plVar8 = local_48;
    }
    (**(code **)(*plVar8 + 0x10))(plVar8,0);
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong *)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
  puVar3 = puVar7;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_78[0] = (code *)&DAT_1411f98e0;
  }
  FUN_141eb3490(local_80,L"Move_Left",1);
  FUN_141a8e170(param_1,local_80,2);
  FUN_141a58370(param_1,local_80,2,local_78);
  if (local_78[0] != (code *)0x0) {
    plVar8 = local_68;
    if (local_48 != (longlong *)0x0) {
      plVar8 = local_48;
    }
    (**(code **)(*plVar8 + 0x10))(plVar8,0);
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong *)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
  puVar3 = puVar7;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_78[0] = (code *)&DAT_1411f8ff0;
  }
  FUN_141eb3490(local_80,L"Move_Right",1);
  FUN_141a8e170(param_1,local_80,2);
  FUN_141a58370(param_1,local_80,2,local_78);
  if (local_78[0] != (code *)0x0) {
    plVar8 = local_68;
    if (local_48 != (longlong *)0x0) {
      plVar8 = local_48;
    }
    (**(code **)(*plVar8 + 0x10))(plVar8,0);
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong *)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
  puVar3 = puVar7;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_78[0] = (code *)&LAB_1411f8af0;
  }
  FUN_141eb3490(local_80,L"Move_Up",1);
  FUN_141a8e170(param_1,local_80,1);
  FUN_141a58370(param_1,local_80,1,local_78);
  if (local_78[0] != (code *)0x0) {
    plVar8 = local_68;
    if (local_48 != (longlong *)0x0) {
      plVar8 = local_48;
    }
    (**(code **)(*plVar8 + 0x10))(plVar8,0);
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong *)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
  puVar3 = puVar7;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_78[0] = (code *)&LAB_1411f9490;
  }
  FUN_141eb3490(local_80,L"Move_Down",1);
  FUN_141a8e170(param_1,local_80,1);
  FUN_141a58370(param_1,local_80,1,local_78);
  if (local_78[0] != (code *)0x0) {
    plVar8 = local_68;
    if (local_48 != (longlong *)0x0) {
      plVar8 = local_48;
    }
    (**(code **)(*plVar8 + 0x10))(plVar8,0);
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong *)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
  puVar3 = puVar7;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_78[0] = (code *)&LAB_1411f8d20;
  }
  FUN_141eb3490(local_80,L"Move_Left",1);
  FUN_141a8e170(param_1,local_80,1);
  FUN_141a58370(param_1,local_80,1,local_78);
  if (local_78[0] != (code *)0x0) {
    plVar8 = local_68;
    if (local_48 != (longlong *)0x0) {
      plVar8 = local_48;
    }
    (**(code **)(*plVar8 + 0x10))(plVar8,0);
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200();
  }
  local_48 = (longlong *)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
  puVar3 = puVar7;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = param_1;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_78[0] = (code *)&LAB_1411f8ea0;
  }
  FUN_141eb3490(local_80,L"Move_Right",1);
  FUN_141a8e170(param_1,local_80,1);
  FUN_141a58370(param_1,local_80,1,local_78);
  if (local_78[0] != (code *)0x0) {
    plVar8 = local_68;
    if (local_48 != (longlong *)0x0) {
      plVar8 = local_48;
    }
    (**(code **)(*plVar8 + 0x10))(plVar8,0);
  }
  if (local_48 != (longlong *)0x0) {
    FUN_141de9200();
  }
  if ((*(longlong *)(param_1 + 0x250) != 0) && (cVar1 = FUN_140d2f9d0(), cVar1 != '\0')) {
    local_48 = (longlong *)0x0;
    puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,local_68);
    if (puVar3 != (undefined8 *)0x0) {
      puVar3[1] = param_1;
      *puVar3 = &PTR_FUN_143969680;
      puVar7 = puVar3;
    }
    if (puVar7 != (undefined8 *)0xfffffffffffffff8) {
      local_78[0] = (code *)&LAB_1411f8e10;
    }
    FUN_141eb3490(local_80,L"Controller_Btn_B",1);
    FUN_141eb3490(local_88,L"Controller_Btn_B",1);
    FUN_141a8e270(param_1,local_80,0);
    local_98 = local_78;
    FUN_141a584f0(param_1,local_88,local_80,0);
    if (local_78[0] != (code *)0x0) {
      plVar8 = local_68;
      if (local_48 != (longlong *)0x0) {
        plVar8 = local_48;
      }
      (**(code **)(*plVar8 + 0x10))(plVar8,0);
    }
    if (local_48 != (longlong *)0x0) {
      FUN_141de9200();
    }
  }
  FUN_142c27d80(param_1,1);
  FUN_142c26740(param_1,1);
  return;
}



// (already emitted) Controller_Btn_B <- 141209377 -> FUN_141207c60
// ================= Controller_Btn_B <- 1414adebb  FUN_1414addc0 @ 1414addc0  size=0x2ad =================
// callers: FUN_1414a7c90@1414a7c90 

void FUN_1414addc0(longlong *param_1)

{
  longlong *plVar1;
  undefined4 uVar2;
  void *pvVar3;
  ulonglong uVar4;
  undefined1 *_Src;
  int iVar5;
  longlong lVar6;
  size_t sVar7;
  undefined8 local_res8;
  void *local_98;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 local_80;
  void *local_78;
  undefined8 local_70;
  undefined1 *local_68;
  ulonglong local_60;
  void *local_58;
  int local_50;
  undefined4 local_4c;
  undefined8 local_48;
  undefined8 local_40;
  void *local_38;
  int local_30;
  undefined4 local_2c;
  void *local_28;
  int local_20;
  undefined4 local_1c;
  
  if ((char)param_1[0x81] != '\x01') {
    (**(code **)(*param_1 + 0x250))(param_1,3);
    plVar1 = (longlong *)param_1[0x77];
    *(undefined1 *)(param_1 + 0x81) = 1;
    if ((plVar1 != (longlong *)0x0) && ((char)plVar1[0x75] != '\0')) {
      (**(code **)(*plVar1 + 0x250))(plVar1,1);
      *(undefined1 *)(plVar1 + 0x75) = 0;
    }
    FUN_1414ac6f0(param_1,0);
    if (param_1[0x7a] != 0) {
      local_98 = (void *)0x0;
      local_90 = 0;
      FUN_141eb3490(&local_88,&DAT_14391f7d0,1);
      FUN_141eb3490(&local_80,&DAT_14391f7d0,1);
      local_78 = (void *)0x0;
      local_70 = 0;
      local_68 = (undefined1 *)0x0;
      local_60 = 1;
      FUN_140827170(&local_68,0);
      uVar4 = local_90;
      *local_68 = 0;
      lVar6 = (longlong)(int)local_90;
      iVar5 = (int)local_90 + 1;
      local_90 = CONCAT44(local_90._4_4_,iVar5);
      if (local_90._4_4_ < iVar5) {
        FUN_140827170(&local_98,uVar4 & 0xffffffff);
      }
      *(undefined1 *)(lVar6 + (longlong)local_98) = 0;
      FUN_141eb3490(&local_res8,L"Controller_Btn_B",1);
      uVar4 = local_70;
      lVar6 = (longlong)(int)local_70;
      iVar5 = (int)local_70 + 1;
      local_70 = CONCAT44(local_70._4_4_,iVar5);
      if (local_70._4_4_ < iVar5) {
        FUN_1407f1510(&local_78,uVar4 & 0xffffffff);
      }
      *(undefined8 *)((longlong)local_78 + lVar6 * 8) = local_res8;
      FUN_141eb3490(&local_res8,L"Battle_MeleeAtk",1);
      pvVar3 = local_98;
      sVar7 = (size_t)(int)local_90;
      local_80 = local_res8;
      local_58 = (void *)0x0;
      local_50 = (int)local_90;
      if ((int)local_90 == 0) {
        local_4c = 0;
      }
      else {
        FUN_140826f10(&local_58,local_90 & 0xffffffff,0);
        memcpy(local_58,pvVar3,sVar7);
      }
      pvVar3 = local_78;
      lVar6 = (longlong)(int)local_70;
      local_48 = local_88;
      local_38 = (void *)0x0;
      local_30 = (int)local_70;
      local_40 = local_80;
      if ((int)local_70 == 0) {
        local_2c = 0;
      }
      else {
        FUN_140826f80(&local_38,local_70 & 0xffffffff,0);
        memcpy(local_38,pvVar3,lVar6 << 3);
      }
      _Src = local_68;
      sVar7 = (size_t)(int)local_60;
      local_28 = (void *)0x0;
      local_20 = (int)local_60;
      if ((int)local_60 == 0) {
        local_1c = 0;
      }
      else {
        FUN_140826f10(&local_28,local_60 & 0xffffffff,0);
        memcpy(local_28,_Src,sVar7);
      }
      FUN_1416192d0(param_1[0x7a],&local_58);
      FUN_1416130e0(param_1[0x7a]);
      if (local_68 != (undefined1 *)0x0) {
        FUN_141de9200();
      }
      if (local_78 != (void *)0x0) {
        FUN_141de9200();
      }
      if (local_98 != (void *)0x0) {
        FUN_141de9200();
      }
    }
    uVar2 = DAT_1450d6c88;
    *(undefined1 *)((longlong)param_1 + 0x409) = 1;
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x7d],0,1,0,uVar2);
  }
  return;
}



// ================= Controller_Btn_B <- 140330d63  FUN_14032fad0 @ 14032fad0  size=0x1783 =================
// callers: <none>

void FUN_14032fad0(void)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined1 local_res8 [8];
  undefined1 local_res10 [8];
  undefined8 local_res18 [2];
  void *local_88;
  undefined8 local_80;
  void *local_78;
  undefined4 local_70;
  undefined8 *local_68;
  undefined8 local_60;
  undefined8 local_58;
  undefined8 local_50;
  undefined8 local_48;
  undefined8 local_40;
  undefined8 local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20;
  
  local_res8[0] = 0x1a;
  local_res10[0] = 0;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res8,1);
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_78,&local_88,&local_68);
  FUN_1415845c0(&DAT_14597fac0,0,uVar1);
  local_res8[0] = 0x1b;
  local_res10[0] = 1;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597fb08,1,uVar1);
  local_res8[0] = 0x1a;
  local_res10[0] = 0;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597fb50,2,uVar1);
  local_res8[0] = 0x1b;
  local_res10[0] = 1;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597fb98,3,uVar1);
  local_58 = 0;
  local_48 = 0;
  local_40 = 0;
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  local_50 = 0;
  FUN_141eb3490(&local_48,&DAT_14391f7d0,1);
  FUN_141eb3490(&local_40,&DAT_14391f7d0,1);
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  FUN_1415845c0(&DAT_14597fbe0,0x1c,&local_58);
  local_48 = 0;
  local_40 = 0;
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  local_58 = 0;
  local_50 = 0;
  FUN_141eb3490(&local_48,&DAT_14391f7d0,1);
  FUN_141eb3490(&local_40,&DAT_14391f7d0,1);
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  FUN_1415845c0(&DAT_14597fc28,0x1d,&local_58);
  local_58 = 0;
  local_48 = 0;
  local_40 = 0;
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  local_50 = 0;
  FUN_141eb3490(&local_48,&DAT_14391f7d0,1);
  FUN_141eb3490(&local_40,&DAT_14391f7d0,1);
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  FUN_1415845c0(&DAT_14597fc70,0x1e,&local_58);
  local_res8[0] = 0x56;
  local_res10[0] = 3;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597fcb8,5,uVar1);
  local_res8[0] = 0x54;
  local_res10[0] = 3;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597fd00,0x2a,uVar1);
  local_res8[0] = 0x54;
  local_res10[0] = 2;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597fd48,4,uVar1);
  local_res8[0] = 0x56;
  local_res10[0] = 2;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597fd90,0x2f,uVar1);
  local_res8[0] = 0x41;
  local_res10[0] = 4;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597fdd8,6,uVar1);
  local_58 = 0;
  local_48 = 0;
  local_40 = 0;
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  local_50 = 0;
  FUN_141eb3490(&local_48,&DAT_14391f7d0,1);
  FUN_141eb3490(&local_40,&DAT_14391f7d0,1);
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  FUN_1415845c0(&DAT_14597fe20,0x1f,&local_58);
  local_res8[0] = 0x41;
  local_res10[0] = 4;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597fe68,0x2c,uVar1);
  local_res8[0] = 0x4d;
  local_res10[0] = 7;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597feb0,9,uVar1);
  local_res8[0] = 0x4d;
  local_res10[0] = 7;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597fef8,0x2b,uVar1);
  local_res8[0] = 0x3f;
  local_res10[0] = 5;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597ff40,7,uVar1);
  local_res8[0] = 0x52;
  local_res10[0] = 5;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597ff88,0x2e,uVar1);
  local_res8[0] = 0x52;
  local_res10[0] = 8;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_14597ffd0,10,uVar1);
  local_res8[0] = 0x3f;
  local_res10[0] = 8;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980018,0x2d,uVar1);
  local_res8[0] = 0x15;
  local_res10[0] = 6;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980060,8,uVar1);
  local_res8[0] = 0x4a;
  local_res10[0] = 6;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_1459800a8,0x31,uVar1);
  local_res8[0] = 0x15;
  local_res10[0] = 9;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_1459800f0,0xb,uVar1);
  local_res8[0] = 0x3e;
  local_res10[0] = 9;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980138,0x30,uVar1);
  local_res8[0] = 0x30;
  local_res10[0] = 10;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980180,0xc,uVar1);
  local_res8[0] = 0x2a;
  local_res10[0] = 0xb;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_1459801c8,0xd,uVar1);
  local_res8[0] = 0x2b;
  local_res10[0] = 0xc;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980210,0xe,uVar1);
  local_res8[0] = 0x2c;
  local_res10[0] = 0xd;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980258,0xf,uVar1);
  local_res8[0] = 0x2d;
  local_res10[0] = 0xe;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_1459802a0,0x10,uVar1);
  local_res8[0] = 0x2f;
  local_res10[0] = 0xf;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_1459802e8,0x11,uVar1);
  local_res8[0] = 0x2e;
  local_res10[0] = 0x10;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980330,0x12,uVar1);
  local_res8[0] = 0x15;
  local_res10[0] = 0x11;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980378,0x13,uVar1);
  local_res8[0] = 0x30;
  local_res10[0] = 0x12;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_1459803c0,0x14,uVar1);
  local_res8[0] = 0x24;
  local_res10[0] = 0x13;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980408,0x15,uVar1);
  local_res8[0] = 0x49;
  local_res10[0] = 0x14;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980450,0x16,uVar1);
  local_res8[0] = 0x49;
  local_res10[0] = 0x14;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980498,0x32,uVar1);
  local_res8[0] = 2;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  puVar2 = (undefined8 *)FUN_141eb3490(local_res10,L"FastForward",1);
  uVar1 = FUN_1415849f0(&local_58,&local_78,*puVar2,&local_68);
  FUN_1415845c0(&DAT_1459804e0,0x19,uVar1);
  local_res8[0] = 3;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  puVar2 = (undefined8 *)FUN_141eb3490(local_res10,L"TalkAutoFeed",1);
  uVar1 = FUN_1415849f0(&local_58,&local_78,*puVar2,&local_68);
  FUN_1415845c0(&DAT_145980528,0x1a,uVar1);
  FUN_141eb3490(local_res18,L"Controller_Btn_B",1);
  local_res8[0] = 0x1a;
  local_res10[0] = 0;
  local_68 = (undefined8 *)0x0;
  local_60 = CONCAT44(local_60._4_4_,1);
  FUN_140826f80(&local_68,1,0);
  *local_68 = local_res18[0];
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80 = CONCAT44(local_80._4_4_,1);
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980570,0x1b,uVar1);
  local_58 = 0;
  local_50 = 0;
  local_48 = 0;
  local_40 = 0;
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  uVar1 = FUN_141584ac0(&local_58);
  FUN_1415845c0(&DAT_1459805b8,0x21,uVar1);
  local_58 = 0;
  local_50 = 0;
  local_48 = 0;
  local_40 = 0;
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  uVar1 = FUN_141584ac0(&local_58);
  FUN_1415845c0(&DAT_145980600,0x20,uVar1);
  local_res8[0] = 0x53;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80 = 0;
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980648,0x22,uVar1);
  local_res8[0] = 0x3d;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80 = 0;
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980690,0x23,uVar1);
  local_res8[0] = 0x4f;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80 = 0;
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_1459806d8,0x24,uVar1);
  local_res8[0] = 0x40;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80 = 0;
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980720,0x25,uVar1);
  local_res8[0] = 0x53;
  local_res10[0] = 0xc;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980768,0x26,uVar1);
  local_res8[0] = 0x3d;
  local_res10[0] = 0xb;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_1459807b0,0x27,uVar1);
  local_res8[0] = 0x4f;
  local_res10[0] = 0xe;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80._0_4_ = 1;
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_1459807f8,0x28,uVar1);
  local_res8[0] = 0x40;
  local_res10[0] = 0xd;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_78 = (void *)0x0;
  local_70 = 1;
  FUN_140826f10(&local_78,1,0);
  memcpy(local_78,local_res8,1);
  local_88 = (void *)0x0;
  local_80 = CONCAT44(local_80._4_4_,1);
  FUN_140826f10(&local_88,1,0);
  memcpy(local_88,local_res10,1);
  uVar1 = FUN_141584910(&local_58,&local_88,&local_78,&local_68);
  FUN_1415845c0(&DAT_145980840,0x29,uVar1);
  local_58 = 0;
  local_50 = 0;
  local_48 = 0;
  local_40 = 0;
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  uVar1 = FUN_141584ac0(&local_58);
  FUN_1415845c0(&DAT_145980888,0x33,uVar1);
  atexit(FUN_143869750);
  return;
}



// ================= Controller_Btn_B <- 14034f065  FUN_14034ef40 @ 14034ef40  size=0x244 =================
// callers: <none>

void FUN_14034ef40(void)

{
  undefined8 *puVar1;
  undefined1 local_res8 [8];
  undefined8 local_res10;
  undefined1 local_res18 [16];
  void *local_38;
  undefined4 local_30;
  undefined8 *local_28;
  undefined4 local_20;
  
  FUN_141eb3490(&local_res10,L"Controller_Btn_Y",1);
  local_res8[0] = 3;
  local_28 = (undefined8 *)0x0;
  local_20 = 1;
  FUN_140826f80(&local_28,1,0);
  *local_28 = local_res10;
  local_38 = (void *)0x0;
  local_30 = 1;
  FUN_140826f10(&local_38,1,0);
  memcpy(local_38,local_res8,1);
  puVar1 = (undefined8 *)FUN_141eb3410(local_res18,"Battle_Palette_Up",1);
  FUN_1415849f0(&DAT_145986e90,&local_38,*puVar1,&local_28);
  FUN_141eb3490(&local_res10,L"Controller_Btn_X",1);
  local_res8[0] = 2;
  local_28 = (undefined8 *)0x0;
  local_20 = 1;
  FUN_140826f80(&local_28,1,0);
  *local_28 = local_res10;
  local_38 = (void *)0x0;
  local_30 = 1;
  FUN_140826f10(&local_38,1,0);
  memcpy(local_38,local_res8,1);
  puVar1 = (undefined8 *)FUN_141eb3410(local_res18,"Battle_Palette_Left",1);
  FUN_1415849f0(&DAT_145986ed0,&local_38,*puVar1,&local_28);
  FUN_141eb3490(&local_res10,L"Controller_Btn_B",1);
  local_res8[0] = 0;
  local_28 = (undefined8 *)0x0;
  local_20 = 1;
  FUN_140826f80(&local_28,1,0);
  *local_28 = local_res10;
  local_38 = (void *)0x0;
  local_30 = 1;
  FUN_140826f10(&local_38,1,0);
  memcpy(local_38,local_res8,1);
  puVar1 = (undefined8 *)FUN_141eb3410(local_res18,"Battle_Palette_Right",1);
  FUN_1415849f0(&DAT_145986f10,&local_38,*puVar1,&local_28);
  FUN_141eb3490(&local_res10,L"Controller_Btn_A",1);
  local_res8[0] = 1;
  local_28 = (undefined8 *)0x0;
  local_20 = 1;
  FUN_140826f80(&local_28,1,0);
  *local_28 = local_res10;
  local_38 = (void *)0x0;
  local_30 = 1;
  FUN_140826f10(&local_38,1,0);
  memcpy(local_38,local_res8,1);
  puVar1 = (undefined8 *)FUN_141eb3410(local_res18,"Battle_Palette_Down",1);
  FUN_1415849f0(&DAT_145986f50,&local_38,*puVar1,&local_28);
  atexit(FUN_14386b430);
  return;
}



// ================= Controller_Btn_B <- 1419e61c7  FUN_1419e48a0 @ 1419e48a0  size=0x1e8d =================
// callers: FUN_1416802b0@1416802b0 

void FUN_1419e48a0(undefined8 *param_1,longlong *param_2)

{
  int iVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  int *piVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  int local_res10 [2];
  undefined1 local_res18 [8];
  
  plVar2 = (longlong *)FUN_141eb3490(local_res10,L"Walk",1);
  lVar7 = 0;
  lVar5 = *plVar2;
  if ((int)param_2[1] != *(int *)((longlong)param_2 + 0x34)) {
    plVar2 = (longlong *)param_2[8];
    if ((longlong *)param_2[8] == (longlong *)0x0) {
      plVar2 = param_2 + 7;
    }
    iVar1 = *(int *)((longlong)plVar2 +
                    ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                    (longlong)(int)param_2[9] - 1U) * 4);
    if (iVar1 != -1) {
      do {
        if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
          if (iVar1 != -1) {
            plVar2 = (longlong *)FUN_141eb3490(local_res10,L"Walk",1);
            lVar5 = *plVar2;
            if ((int)param_2[1] == *(int *)((longlong)param_2 + 0x34)) goto LAB_1419e49c0;
            plVar2 = (longlong *)param_2[8];
            if ((longlong *)param_2[8] == (longlong *)0x0) {
              plVar2 = param_2 + 7;
            }
            iVar1 = *(int *)((longlong)plVar2 +
                            ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                            (longlong)(int)param_2[9] - 1U) * 4);
            if (iVar1 == -1) goto LAB_1419e49c0;
            goto LAB_1419e49a0;
          }
          break;
        }
        iVar1 = *(int *)(*param_2 + 0x10 + (longlong)iVar1 * 0x18);
      } while (iVar1 != -1);
    }
  }
  goto LAB_1419e49ca;
  while (iVar1 = *(int *)(lVar6 + 0x10), iVar1 != -1) {
LAB_1419e4ab0:
    lVar6 = *param_2 + (longlong)iVar1 * 0x18;
    if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
      if (iVar1 != -1) goto LAB_1419e4ad3;
      break;
    }
  }
LAB_1419e4ad0:
  lVar6 = lVar7;
LAB_1419e4ad3:
  param_1[1] = *(undefined8 *)(lVar6 + 8);
  goto LAB_1419e4adb;
  while (iVar1 = *(int *)(lVar6 + 0x10), iVar1 != -1) {
LAB_1419e4bc0:
    lVar6 = *param_2 + (longlong)iVar1 * 0x18;
    if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
      if (iVar1 != -1) goto LAB_1419e4be3;
      break;
    }
  }
LAB_1419e4be0:
  lVar6 = lVar7;
LAB_1419e4be3:
  param_1[2] = *(undefined8 *)(lVar6 + 8);
  goto LAB_1419e4beb;
  while (iVar1 = *(int *)(lVar6 + 0x10), iVar1 != -1) {
LAB_1419e4cd0:
    lVar6 = *param_2 + (longlong)iVar1 * 0x18;
    if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
      if (iVar1 != -1) goto LAB_1419e4cf3;
      break;
    }
  }
LAB_1419e4cf0:
  lVar6 = lVar7;
LAB_1419e4cf3:
  param_1[3] = *(undefined8 *)(lVar6 + 8);
  goto LAB_1419e4cfb;
  while (iVar1 = *(int *)(lVar6 + 0x10), iVar1 != -1) {
LAB_1419e4de0:
    lVar6 = *param_2 + (longlong)iVar1 * 0x18;
    if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
      if (iVar1 != -1) goto LAB_1419e4e03;
      break;
    }
  }
LAB_1419e4e00:
  lVar6 = lVar7;
LAB_1419e4e03:
  param_1[4] = *(undefined8 *)(lVar6 + 8);
  goto LAB_1419e4e0b;
  while (iVar1 = *(int *)(lVar6 + 0x10), iVar1 != -1) {
LAB_1419e4ef0:
    lVar6 = *param_2 + (longlong)iVar1 * 0x18;
    if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
      if (iVar1 != -1) goto LAB_1419e4f13;
      break;
    }
  }
LAB_1419e4f10:
  lVar6 = lVar7;
LAB_1419e4f13:
  param_1[5] = *(undefined8 *)(lVar6 + 8);
  goto LAB_1419e4f1b;
  while (iVar1 = *(int *)(lVar6 + 0x10), iVar1 != -1) {
LAB_1419e49a0:
    lVar6 = *param_2 + (longlong)iVar1 * 0x18;
    if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
      if (iVar1 != -1) goto LAB_1419e49c3;
      break;
    }
  }
LAB_1419e49c0:
  lVar6 = lVar7;
LAB_1419e49c3:
  *param_1 = *(undefined8 *)(lVar6 + 8);
LAB_1419e49ca:
  plVar2 = (longlong *)FUN_141eb3490(local_res10,L"Move_Up",1);
  lVar5 = *plVar2;
  if ((int)param_2[1] != *(int *)((longlong)param_2 + 0x34)) {
    plVar2 = (longlong *)param_2[8];
    if ((longlong *)param_2[8] == (longlong *)0x0) {
      plVar2 = param_2 + 7;
    }
    iVar1 = *(int *)((longlong)plVar2 +
                    ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                    (longlong)(int)param_2[9] - 1U) * 4);
    if (iVar1 != -1) {
      do {
        if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
          if (iVar1 != -1) {
            plVar2 = (longlong *)FUN_141eb3490(local_res10,L"Move_Up",1);
            lVar5 = *plVar2;
            if ((int)param_2[1] == *(int *)((longlong)param_2 + 0x34)) goto LAB_1419e4ad0;
            plVar2 = (longlong *)param_2[8];
            if ((longlong *)param_2[8] == (longlong *)0x0) {
              plVar2 = param_2 + 7;
            }
            iVar1 = *(int *)((longlong)plVar2 +
                            ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                            (longlong)(int)param_2[9] - 1U) * 4);
            if (iVar1 == -1) goto LAB_1419e4ad0;
            goto LAB_1419e4ab0;
          }
          break;
        }
        iVar1 = *(int *)(*param_2 + 0x10 + (longlong)iVar1 * 0x18);
      } while (iVar1 != -1);
    }
  }
LAB_1419e4adb:
  plVar2 = (longlong *)FUN_141eb3490(local_res10,L"Move_Left",1);
  lVar5 = *plVar2;
  if ((int)param_2[1] != *(int *)((longlong)param_2 + 0x34)) {
    plVar2 = (longlong *)param_2[8];
    if ((longlong *)param_2[8] == (longlong *)0x0) {
      plVar2 = param_2 + 7;
    }
    iVar1 = *(int *)((longlong)plVar2 +
                    ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                    (longlong)(int)param_2[9] - 1U) * 4);
    if (iVar1 != -1) {
      do {
        if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
          if (iVar1 != -1) {
            plVar2 = (longlong *)FUN_141eb3490(local_res10,L"Move_Left",1);
            lVar5 = *plVar2;
            if ((int)param_2[1] == *(int *)((longlong)param_2 + 0x34)) goto LAB_1419e4be0;
            plVar2 = (longlong *)param_2[8];
            if ((longlong *)param_2[8] == (longlong *)0x0) {
              plVar2 = param_2 + 7;
            }
            iVar1 = *(int *)((longlong)plVar2 +
                            ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                            (longlong)(int)param_2[9] - 1U) * 4);
            if (iVar1 == -1) goto LAB_1419e4be0;
            goto LAB_1419e4bc0;
          }
          break;
        }
        iVar1 = *(int *)(*param_2 + 0x10 + (longlong)iVar1 * 0x18);
      } while (iVar1 != -1);
    }
  }
LAB_1419e4beb:
  plVar2 = (longlong *)FUN_141eb3490(local_res10,L"Move_Down",1);
  lVar5 = *plVar2;
  if ((int)param_2[1] != *(int *)((longlong)param_2 + 0x34)) {
    plVar2 = (longlong *)param_2[8];
    if ((longlong *)param_2[8] == (longlong *)0x0) {
      plVar2 = param_2 + 7;
    }
    iVar1 = *(int *)((longlong)plVar2 +
                    ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                    (longlong)(int)param_2[9] - 1U) * 4);
    if (iVar1 != -1) {
      do {
        if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
          if (iVar1 != -1) {
            plVar2 = (longlong *)FUN_141eb3490(local_res10,L"Move_Down",1);
            lVar5 = *plVar2;
            if ((int)param_2[1] == *(int *)((longlong)param_2 + 0x34)) goto LAB_1419e4cf0;
            plVar2 = (longlong *)param_2[8];
            if ((longlong *)param_2[8] == (longlong *)0x0) {
              plVar2 = param_2 + 7;
            }
            iVar1 = *(int *)((longlong)plVar2 +
                            ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                            (longlong)(int)param_2[9] - 1U) * 4);
            if (iVar1 == -1) goto LAB_1419e4cf0;
            goto LAB_1419e4cd0;
          }
          break;
        }
        iVar1 = *(int *)(*param_2 + 0x10 + (longlong)iVar1 * 0x18);
      } while (iVar1 != -1);
    }
  }
LAB_1419e4cfb:
  plVar2 = (longlong *)FUN_141eb3490(local_res10,L"Move_Right",1);
  lVar5 = *plVar2;
  if ((int)param_2[1] != *(int *)((longlong)param_2 + 0x34)) {
    plVar2 = (longlong *)param_2[8];
    if ((longlong *)param_2[8] == (longlong *)0x0) {
      plVar2 = param_2 + 7;
    }
    iVar1 = *(int *)((longlong)plVar2 +
                    ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                    (longlong)(int)param_2[9] - 1U) * 4);
    if (iVar1 != -1) {
      do {
        if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
          if (iVar1 != -1) {
            plVar2 = (longlong *)FUN_141eb3490(local_res10,L"Move_Right",1);
            lVar5 = *plVar2;
            if ((int)param_2[1] == *(int *)((longlong)param_2 + 0x34)) goto LAB_1419e4e00;
            plVar2 = (longlong *)param_2[8];
            if ((longlong *)param_2[8] == (longlong *)0x0) {
              plVar2 = param_2 + 7;
            }
            iVar1 = *(int *)((longlong)plVar2 +
                            ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                            (longlong)(int)param_2[9] - 1U) * 4);
            if (iVar1 == -1) goto LAB_1419e4e00;
            goto LAB_1419e4de0;
          }
          break;
        }
        iVar1 = *(int *)(*param_2 + 0x10 + (longlong)iVar1 * 0x18);
      } while (iVar1 != -1);
    }
  }
LAB_1419e4e0b:
  plVar2 = (longlong *)FUN_141eb3490(local_res10,L"Boost",1);
  lVar5 = *plVar2;
  if ((int)param_2[1] != *(int *)((longlong)param_2 + 0x34)) {
    plVar2 = (longlong *)param_2[8];
    if ((longlong *)param_2[8] == (longlong *)0x0) {
      plVar2 = param_2 + 7;
    }
    iVar1 = *(int *)((longlong)plVar2 +
                    ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                    (longlong)(int)param_2[9] - 1U) * 4);
    if (iVar1 != -1) {
      do {
        if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
          if (iVar1 != -1) {
            plVar2 = (longlong *)FUN_141eb3490(local_res10,L"Boost",1);
            lVar5 = *plVar2;
            if ((int)param_2[1] == *(int *)((longlong)param_2 + 0x34)) goto LAB_1419e4f10;
            plVar2 = (longlong *)param_2[8];
            if ((longlong *)param_2[8] == (longlong *)0x0) {
              plVar2 = param_2 + 7;
            }
            iVar1 = *(int *)((longlong)plVar2 +
                            ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                            (longlong)(int)param_2[9] - 1U) * 4);
            if (iVar1 == -1) goto LAB_1419e4f10;
            goto LAB_1419e4ef0;
          }
          break;
        }
        iVar1 = *(int *)(*param_2 + 0x10 + (longlong)iVar1 * 0x18);
      } while (iVar1 != -1);
    }
  }
LAB_1419e4f1b:
  plVar2 = (longlong *)FUN_141eb3490(local_res10,L"BoostUp",1);
  lVar5 = *plVar2;
  if ((int)param_2[1] != *(int *)((longlong)param_2 + 0x34)) {
    plVar2 = (longlong *)param_2[8];
    if ((longlong *)param_2[8] == (longlong *)0x0) {
      plVar2 = param_2 + 7;
    }
    iVar1 = *(int *)((longlong)plVar2 +
                    ((longlong)((int)((ulonglong)lVar5 >> 0x20) + (int)lVar5) &
                    (longlong)(int)param_2[9] - 1U) * 4);
    if (iVar1 != -1) {
      do {
        if (*(longlong *)(*param_2 + (longlong)iVar1 * 0x18) == lVar5) {
          if (iVar1 != -1) {
            puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"BoostUp",1);
            FUN_140b349f0(param_2,local_res10,*puVar3);
            lVar5 = lVar7;
            if (local_res10[0] != -1) {
              lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
            }
            param_1[6] = *(undefined8 *)(lVar5 + 8);
          }
          break;
        }
        iVar1 = *(int *)(*param_2 + 0x10 + (longlong)iVar1 * 0x18);
      } while (iVar1 != -1);
    }
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Jump",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Jump",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[7] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Fly_Rolling",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Fly_Rolling",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[8] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Fly_Ascend",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Fly_Ascend",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[9] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Fly_Descend",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Fly_Descend",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[10] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"KiSearch",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"KiSearch",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0xb] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"TargetCursor",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"TargetCursor",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0xc] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"KiBlust",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"KiBlust",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0xd] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"MapChange",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"MapChange",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0xe] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Ride",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Ride",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0xf] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Ride_Off",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Ride_Off",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x10] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"RideChange_Left",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"RideChange_Left",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x11] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"RideChange_Right",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"RideChange_Right",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x12] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"MineralRadar",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"MineralRadar",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x13] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Car_Forward",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Car_Forward",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x14] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Car_Reverse",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Car_Reverse",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x15] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Car_ChangeView",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Car_ChangeView",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x16] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Car_Sound_Left",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Car_Sound_Left",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x17] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Car_Sound_Right",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Car_Sound_Right",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x18] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"BaseBall_Swing",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"BaseBall_Swing",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x19] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"FastForward",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"FastForward",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x1a] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"TalkAutoFeed",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"TalkAutoFeed",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x1b] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Boost",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Boost",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x1c] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_HighBoost",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_HighBoost",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x1d] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_TargetChange_Left",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_TargetChange_Left",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x1e] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_TargetChange_Right",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_TargetChange_Right",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x1f] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_TargetCancel",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_TargetCancel",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x20] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_MeleeAtk",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_MeleeAtk",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x21] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_KiAtk",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_KiAtk",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x22] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_KiRecover",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_KiRecover",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x23] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_MoveMode",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_MoveMode",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x24] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Guard",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Guard",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x25] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Zcombo",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Zcombo",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x26] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_FormChange_Palette",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_FormChange_Palette",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x27] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Support_Palette",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Support_Palette",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x28] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_ActiveSkill_Palette",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_ActiveSkill_Palette",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x29] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Item_Palette",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Item_Palette",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x2a] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Palette_Up",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Palette_Up",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x2b] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Palette_Left",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Palette_Left",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x2c] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Palette_Down",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Palette_Down",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x2d] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Palette_Right",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Battle_Palette_Right",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x2e] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_B",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_B",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x30] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_X",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_X",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x31] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_A",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_A",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x32] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_Y",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_Y",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x33] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_LB",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_LB",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x34] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_LT",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_LT",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x35] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_L3",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_L3",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x36] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_RB",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_RB",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x37] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_RT",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_RT",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x38] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_R3",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_R3",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x39] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_Start",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_Start",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    lVar5 = lVar7;
    if (local_res10[0] != -1) {
      lVar5 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x3a] = *(undefined8 *)(lVar5 + 8);
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_Back",1);
  piVar4 = (int *)FUN_140b349f0(param_2,local_res10,*puVar3);
  if (*piVar4 != -1) {
    puVar3 = (undefined8 *)FUN_141eb3490(local_res18,L"Controller_Btn_Back",1);
    FUN_140b349f0(param_2,local_res10,*puVar3);
    if (local_res10[0] != -1) {
      lVar7 = *param_2 + (longlong)local_res10[0] * 0x18;
    }
    param_1[0x3b] = *(undefined8 *)(lVar7 + 8);
  }
  return;
}



// (already emitted) Controller_Btn_B <- 1419e61f1 -> FUN_1419e48a0
// ================= Controller_Btn_B <- 1419d6d25  FUN_1419d65b0 @ 1419d65b0  size=0x971 =================
// callers: FUN_1419cf770@1419cf770 FUN_1419e00e0@1419e00e0 FUN_1419d6280@1419d6280 FUN_1419e3660@1419e3660 FUN_1419debf0@1419debf0 FUN_1419e1a40@1419e1a40 FUN_1419e0920@1419e0920 FUN_1419df290@1419df290 

void FUN_1419d65b0(longlong param_1,undefined8 param_2)

{
  undefined1 local_res8 [8];
  wchar_t *local_18;
  longlong local_10;
  
  FUN_141477c10(param_2,0);
  local_18 = L"Walk";
  local_10 = param_1;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Move_Up";
  local_10 = param_1 + 8;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Move_Left";
  local_10 = param_1 + 0x10;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Move_Down";
  local_10 = param_1 + 0x18;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Move_Right";
  local_10 = param_1 + 0x20;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Boost";
  local_10 = param_1 + 0x28;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"BoostUp";
  local_10 = param_1 + 0x30;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Jump";
  local_10 = param_1 + 0x38;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Fly_Rolling";
  local_10 = param_1 + 0x40;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Fly_Ascend";
  local_10 = param_1 + 0x48;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Fly_Descend";
  local_10 = param_1 + 0x50;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"KiSearch";
  local_10 = param_1 + 0x58;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"TargetCursor";
  local_10 = param_1 + 0x60;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"KiBlust";
  local_10 = param_1 + 0x68;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"MapChange";
  local_10 = param_1 + 0x70;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Ride";
  local_10 = param_1 + 0x78;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Ride_Off";
  local_10 = param_1 + 0x80;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"RideChange_Left";
  local_10 = param_1 + 0x88;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"RideChange_Right";
  local_10 = param_1 + 0x90;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"MineralRadar";
  local_10 = param_1 + 0x98;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Car_Forward";
  local_10 = param_1 + 0xa0;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Car_Reverse";
  local_10 = param_1 + 0xa8;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Car_ChangeView";
  local_10 = param_1 + 0xb0;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Car_Sound_Left";
  local_10 = param_1 + 0xb8;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Car_Sound_Right";
  local_10 = param_1 + 0xc0;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"BaseBall_Swing";
  local_10 = param_1 + 200;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"FastForward";
  local_10 = param_1 + 0xd0;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"TalkAutoFeed";
  local_10 = param_1 + 0xd8;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_Boost";
  local_10 = param_1 + 0xe0;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_HighBoost";
  local_10 = param_1 + 0xe8;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_TargetChange_Left";
  local_10 = param_1 + 0xf0;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_TargetChange_Right";
  local_10 = param_1 + 0xf8;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_TargetCancel";
  local_10 = param_1 + 0x100;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_MeleeAtk";
  local_10 = param_1 + 0x108;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_KiAtk";
  local_10 = param_1 + 0x110;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_KiRecover";
  local_10 = param_1 + 0x118;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_MoveMode";
  local_10 = param_1 + 0x120;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_Guard";
  local_10 = param_1 + 0x128;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_Zcombo";
  local_10 = param_1 + 0x130;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_FormChange_Palette";
  local_10 = param_1 + 0x138;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_Support_Palette";
  local_10 = param_1 + 0x140;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_ActiveSkill_Palette";
  local_10 = param_1 + 0x148;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_Item_Palette";
  local_10 = param_1 + 0x150;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_Palette_Up";
  local_10 = param_1 + 0x158;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_Palette_Left";
  local_10 = param_1 + 0x160;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_Palette_Down";
  local_10 = param_1 + 0x168;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Battle_Palette_Right";
  local_10 = param_1 + 0x170;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Controller_Btn_B";
  local_10 = param_1 + 0x180;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Controller_Btn_X";
  local_10 = param_1 + 0x188;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Controller_Btn_A";
  local_10 = param_1 + 400;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Controller_Btn_Y";
  local_10 = param_1 + 0x198;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Controller_Btn_LB";
  local_10 = param_1 + 0x1a0;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Controller_Btn_LT";
  local_10 = param_1 + 0x1a8;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Controller_Btn_L3";
  local_10 = param_1 + 0x1b0;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Controller_Btn_RB";
  local_10 = param_1 + 0x1b8;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Controller_Btn_RT";
  local_10 = param_1 + 0x1c0;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Controller_Btn_R3";
  local_10 = param_1 + 0x1c8;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Controller_Btn_Start";
  local_10 = param_1 + 0x1d0;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  local_18 = L"Controller_Btn_Back";
  local_10 = param_1 + 0x1d8;
  FUN_1419c6de0(param_2,local_res8,&local_18,0);
  return;
}



// (already emitted) Controller_Btn_B <- 1419d6d2f -> FUN_1419d65b0
// ================= Controller_Btn_B <- 1419cf8d4  FUN_1419cf770 @ 1419cf770  size=0xacb =================
// callers: FUN_141206ac0@141206ac0 FUN_141209430@141209430 FUN_141201810@141201810 FUN_1411ff170@1411ff170 FUN_1411ff580@1411ff580 FUN_1411ffec0@1411ffec0 FUN_1411ffcd0@1411ffcd0 FUN_1411fefd0@1411fefd0 FUN_141200480@141200480 FUN_141207c20@141207c20 FUN_141203070@141203070 FUN_1412032b0@1412032b0 FUN_141207c60@141207c60 FUN_141200920@141200920 FUN_141201850@141201850 FUN_1411ffb50@1411ffb50 FUN_1412031e0@1412031e0 FUN_141207610@141207610 FUN_141206f40@141206f40 FUN_141200680@141200680 FUN_1412061c0@1412061c0 ...

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1419cf770(undefined8 param_1)

{
  undefined1 uVar1;
  undefined1 auStack_358 [32];
  undefined8 *local_338;
  undefined4 local_330;
  undefined8 *local_328;
  undefined8 local_320;
  undefined8 local_318;
  longlong local_310 [3];
  undefined1 local_2f8 [480];
  undefined8 local_118;
  undefined8 uStack_110;
  undefined4 local_108;
  undefined4 uStack_104;
  undefined4 uStack_100;
  undefined4 uStack_fc;
  undefined8 local_f8;
  undefined8 uStack_f0;
  undefined4 local_e8;
  undefined4 uStack_e4;
  undefined8 uStack_e0;
  undefined8 local_d8;
  undefined4 uStack_d0;
  undefined4 uStack_cc;
  undefined4 local_c8;
  undefined4 uStack_c4;
  undefined4 uStack_c0;
  undefined4 uStack_bc;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined4 local_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined8 local_78 [2];
  undefined8 local_68;
  undefined8 local_60;
  undefined8 local_48;
  undefined8 local_40;
  undefined4 local_38;
  undefined4 local_34;
  longlong local_28;
  undefined4 local_20;
  ulonglong local_18;
  
  local_18 = DAT_145909780 ^ (ulonglong)auStack_358;
  uVar1 = FUN_141a66f70();
  FUN_1419e3970(param_1,uVar1);
  FUN_1419e2230(param_1,uVar1);
  FUN_1419e3210(param_1);
  local_e8 = 0xffffffff;
  local_118 = 0;
  uStack_110 = 0;
  local_f8 = 0;
  uStack_f0 = 0;
  uStack_e4 = 0;
  local_d8 = 0;
  uStack_d0 = 0;
  FUN_1419c8e60(local_2f8);
  uVar1 = FUN_141a66f70();
  FUN_1419e3970(local_2f8,uVar1);
  FUN_1419e2230(local_2f8,uVar1);
  FUN_1419e3210(local_2f8);
  FUN_1419d65b0(local_2f8,&local_118);
  FUN_141a82490(&local_118);
  uStack_d0 = 0;
  if (local_d8 != 0) {
    FUN_141de9200();
  }
  FUN_1407fa390(&local_118);
  local_68 = 0;
  local_60 = 0;
  local_48 = 0;
  local_40 = 0;
  local_38 = 0xffffffff;
  local_34 = 0;
  local_28 = 0;
  local_20 = 0;
  FUN_1419d65b0(param_1,&local_68);
  FUN_141a82450(&local_68);
  FUN_141eb3490(&local_118,L"Controller_Btn_B",1);
  FUN_141eb3490(&uStack_110,L"Controller_Btn_X",1);
  FUN_141eb3490(&local_108,L"Controller_Btn_A",1);
  FUN_141eb3490(&uStack_100,L"Controller_Btn_Y",1);
  FUN_141eb3490(&local_f8,L"Controller_Btn_LB",1);
  FUN_141eb3490(&uStack_f0,L"Controller_Btn_LT",1);
  FUN_141eb3490(&local_e8,L"Controller_Btn_L3",1);
  FUN_141eb3490(&uStack_e0,L"Controller_Btn_RB",1);
  FUN_141eb3490(&local_d8,L"Controller_Btn_RT",1);
  FUN_141eb3490(&uStack_d0,L"Controller_Btn_R3",1);
  FUN_141eb3490(&local_c8,L"Controller_Btn_Start",1);
  FUN_141eb3490(&uStack_c0,L"Controller_Btn_Back",1);
  FUN_141eb3490(&local_318,L"Controller",1);
  local_338 = (undefined8 *)0x0;
  local_330 = 0xc;
  FUN_140826f80(&local_338,0xc);
  *local_338 = local_118;
  local_338[1] = uStack_110;
  local_338[2] = CONCAT44(uStack_104,local_108);
  local_338[3] = CONCAT44(uStack_fc,uStack_100);
  local_338[4] = local_f8;
  local_338[5] = uStack_f0;
  local_338[6] = CONCAT44(uStack_e4,local_e8);
  local_338[7] = uStack_e0;
  *(undefined4 *)(local_338 + 8) = (undefined4)local_d8;
  *(undefined4 *)((longlong)local_338 + 0x44) = local_d8._4_4_;
  *(undefined4 *)(local_338 + 9) = uStack_d0;
  *(undefined4 *)((longlong)local_338 + 0x4c) = uStack_cc;
  *(undefined4 *)(local_338 + 10) = local_c8;
  *(undefined4 *)((longlong)local_338 + 0x54) = uStack_c4;
  *(undefined4 *)(local_338 + 0xb) = uStack_c0;
  *(undefined4 *)((longlong)local_338 + 0x5c) = uStack_bc;
  FUN_141a824a0(&local_318,&local_338);
  if (local_338 != (undefined8 *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(&local_118,L"Walk",1);
  FUN_141eb3490(&uStack_110,L"Move_Up",1);
  FUN_141eb3490(&local_108,L"Move_Left",1);
  FUN_141eb3490(&uStack_100,L"Move_Down",1);
  FUN_141eb3490(&local_f8,L"Move_Right",1);
  FUN_141eb3490(&uStack_f0,L"Boost",1);
  FUN_141eb3490(&local_e8,L"BoostUp",1);
  FUN_141eb3490(&uStack_e0,L"Jump",1);
  FUN_141eb3490(&local_d8,L"Fly_Rolling",1);
  FUN_141eb3490(&uStack_d0,L"Fly_Ascend",1);
  FUN_141eb3490(&local_c8,L"Fly_Descend",1);
  FUN_141eb3490(&uStack_c0,L"KiSearch",1);
  FUN_141eb3490(&local_b8,L"TargetCursor",1);
  FUN_141eb3490(&uStack_b0,L"KiBlust",1);
  FUN_141eb3490(&local_a8,L"MapChange",1);
  FUN_141eb3490(&uStack_a0,L"Ride",1);
  FUN_141eb3490(&local_98,L"Ride_Off",1);
  FUN_141eb3490(&uStack_90,L"RideChange_Left",1);
  FUN_141eb3490(&local_88,L"RideChange_Right",1);
  FUN_141eb3490(&uStack_80,L"FastForward",1);
  FUN_141eb3490(local_78,L"TalkAutoFeed",1);
  FUN_141eb3490(&local_318,L"Field",1);
  local_338 = (undefined8 *)0x0;
  local_330 = 0x15;
  FUN_140826f80(&local_338,0x15);
  *local_338 = local_118;
  local_338[1] = uStack_110;
  local_338[2] = CONCAT44(uStack_104,local_108);
  local_338[3] = CONCAT44(uStack_fc,uStack_100);
  local_338[4] = local_f8;
  local_338[5] = uStack_f0;
  local_338[6] = CONCAT44(uStack_e4,local_e8);
  local_338[7] = uStack_e0;
  local_338[8] = local_d8;
  local_338[9] = CONCAT44(uStack_cc,uStack_d0);
  local_338[10] = CONCAT44(uStack_c4,local_c8);
  local_338[0xb] = CONCAT44(uStack_bc,uStack_c0);
  local_338[0xc] = local_b8;
  local_338[0xd] = uStack_b0;
  local_338[0xe] = local_a8;
  local_338[0xf] = uStack_a0;
  *(undefined4 *)(local_338 + 0x10) = local_98;
  *(undefined4 *)((longlong)local_338 + 0x84) = uStack_94;
  *(undefined4 *)(local_338 + 0x11) = uStack_90;
  *(undefined4 *)((longlong)local_338 + 0x8c) = uStack_8c;
  *(undefined4 *)(local_338 + 0x12) = local_88;
  *(undefined4 *)((longlong)local_338 + 0x94) = uStack_84;
  *(undefined4 *)(local_338 + 0x13) = uStack_80;
  *(undefined4 *)((longlong)local_338 + 0x9c) = uStack_7c;
  local_338[0x14] = local_78[0];
  FUN_141a824a0(&local_318,&local_338);
  if (local_338 != (undefined8 *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(&local_118,L"MineralRadar",1);
  FUN_141eb3490(&uStack_110,L"Car_Forward",1);
  FUN_141eb3490(&local_108,L"Car_Reverse",1);
  FUN_141eb3490(&uStack_100,L"Car_ChangeView",1);
  FUN_141eb3490(&local_f8,L"Car_Sound_Left",1);
  FUN_141eb3490(&uStack_f0,L"Car_Sound_Right",1);
  FUN_141eb3490(&local_318,&DAT_144455d58,1);
  local_338 = (undefined8 *)0x0;
  local_330 = 6;
  FUN_140826f80(&local_338,6);
  *local_338 = local_118;
  local_338[1] = uStack_110;
  *(undefined4 *)(local_338 + 2) = local_108;
  *(undefined4 *)((longlong)local_338 + 0x14) = uStack_104;
  *(undefined4 *)(local_338 + 3) = uStack_100;
  *(undefined4 *)((longlong)local_338 + 0x1c) = uStack_fc;
  *(undefined4 *)(local_338 + 4) = (undefined4)local_f8;
  *(undefined4 *)((longlong)local_338 + 0x24) = local_f8._4_4_;
  *(undefined4 *)(local_338 + 5) = (undefined4)uStack_f0;
  *(undefined4 *)((longlong)local_338 + 0x2c) = uStack_f0._4_4_;
  FUN_141a824a0(&local_318,&local_338);
  if (local_338 != (undefined8 *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(&local_318,L"BaseBall_Swing",1);
  FUN_141eb3490(&local_338,L"BaseBall",1);
  local_328 = (undefined8 *)0x0;
  local_320 = CONCAT44(local_320._4_4_,1);
  FUN_140826f80(&local_328,1);
  *local_328 = local_318;
  FUN_141a824a0(&local_338,&local_328);
  if (local_328 != (undefined8 *)0x0) {
    FUN_141de9200();
  }
  FUN_141eb3490(&local_118,L"Battle_Boost",1);
  FUN_141eb3490(&uStack_110,L"Battle_HighBoost",1);
  FUN_141eb3490(&local_108,L"Battle_TargetChange_Left",1);
  FUN_141eb3490(&uStack_100,L"Battle_TargetChange_Right",1);
  FUN_141eb3490(&local_f8,L"Battle_TargetCancel",1);
  FUN_141eb3490(&uStack_f0,L"Battle_MeleeAtk",1);
  FUN_141eb3490(&local_e8,L"Battle_KiAtk",1);
  FUN_141eb3490(&uStack_e0,L"Battle_KiRecover",1);
  FUN_141eb3490(&local_d8,L"Battle_MoveMode",1);
  FUN_141eb3490(&uStack_d0,L"Battle_Guard",1);
  FUN_141eb3490(&local_c8,L"Battle_Zcombo",1);
  FUN_141eb3490(&uStack_c0,L"Battle_FormChange_Palette",1);
  FUN_141eb3490(&local_b8,L"Battle_Support_Palette",1);
  FUN_141eb3490(&uStack_b0,L"Battle_ActiveSkill_Palette",1);
  FUN_141eb3490(&local_a8,L"Battle_Item_Palette",1);
  FUN_141eb3490(&uStack_a0,L"Battle_Palette_Up",1);
  FUN_141eb3490(&local_98,L"Battle_Palette_Left",1);
  FUN_141eb3490(&uStack_90,L"Battle_Palette_Down",1);
  FUN_141eb3490(&local_88,L"Battle_Palette_Right",1);
  FUN_141eb3490(&local_338,L"Battle",1);
  local_328 = (undefined8 *)0x0;
  local_320 = CONCAT44(local_320._4_4_,0x13);
  FUN_140826f80(&local_328,0x13);
  *local_328 = local_118;
  local_328[1] = uStack_110;
  local_328[2] = CONCAT44(uStack_104,local_108);
  local_328[3] = CONCAT44(uStack_fc,uStack_100);
  local_328[4] = local_f8;
  local_328[5] = uStack_f0;
  local_328[6] = CONCAT44(uStack_e4,local_e8);
  local_328[7] = uStack_e0;
  local_328[8] = local_d8;
  local_328[9] = CONCAT44(uStack_cc,uStack_d0);
  local_328[10] = CONCAT44(uStack_c4,local_c8);
  local_328[0xb] = CONCAT44(uStack_bc,uStack_c0);
  local_328[0xc] = local_b8;
  local_328[0xd] = uStack_b0;
  local_328[0xe] = local_a8;
  local_328[0xf] = uStack_a0;
  *(undefined4 *)(local_328 + 0x10) = local_98;
  *(undefined4 *)((longlong)local_328 + 0x84) = uStack_94;
  *(undefined4 *)(local_328 + 0x11) = uStack_90;
  *(undefined4 *)((longlong)local_328 + 0x8c) = uStack_8c;
  local_328[0x12] = CONCAT44(uStack_84,local_88);
  FUN_141a824a0(&local_338,&local_328);
  if (local_328 != (undefined8 *)0x0) {
    FUN_141de9200();
  }
  local_328 = (undefined8 *)0x0;
  local_320 = 0;
  FUN_1419d3f50(&local_328);
  FUN_141a81fd0(&local_328);
  if (local_328 != (undefined8 *)0x0) {
    FUN_141de9200();
  }
  local_310[0] = 0;
  local_310[1] = 0;
  FUN_1419d5890(local_310);
  FUN_141a82050(local_310);
  if (local_310[0] != 0) {
    FUN_141de9200();
  }
  local_20 = 0;
  if (local_28 != 0) {
    FUN_141de9200();
  }
  FUN_1407fa390(&local_68);
  return;
}



// ================= Controller_Btn_B <- 1419e0140  FUN_1419e00e0 @ 1419e00e0  size=0x834 =================
// callers: FUN_1416802b0@1416802b0 

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1419e00e0(undefined8 param_1,char param_2)

{
  int iVar1;
  longlong lVar2;
  longlong *plVar3;
  undefined8 *puVar4;
  undefined1 *puVar5;
  longlong lVar6;
  longlong lVar7;
  undefined1 auStack_98 [32];
  undefined1 local_78 [8];
  undefined1 local_70 [8];
  longlong local_68;
  undefined8 local_60;
  undefined8 local_48;
  undefined8 local_40;
  undefined4 local_38;
  int local_34;
  undefined1 local_30 [8];
  undefined1 *local_28;
  int local_20;
  ulonglong local_18;
  
  local_18 = DAT_145909780 ^ (ulonglong)auStack_98;
  lVar7 = 0;
  local_38 = 0xffffffff;
  local_68 = 0;
  local_60 = 0;
  local_48 = 0;
  local_40 = 0;
  local_34 = 0;
  local_28 = (undefined1 *)0x0;
  local_20 = 0;
  if (param_2 == '\0') {
    FUN_1419e3210();
    FUN_1419d65b0(param_1,&local_68);
    FUN_141a82450(&local_68);
  }
  else {
    FUN_1419d65b0(param_1,&local_68);
    plVar3 = (longlong *)FUN_141eb3490(local_78,L"Controller_Btn_B",1);
    lVar2 = *plVar3;
    if ((int)local_60 != local_34) {
      puVar5 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar5 = local_28;
      }
      iVar1 = *(int *)(puVar5 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar2) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e01bd;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar7;
LAB_1419e01bd:
    puVar4 = (undefined8 *)FUN_142060720(&DAT_145b9fb50,local_70);
    *(undefined8 *)(lVar6 + 8) = *puVar4;
    plVar3 = (longlong *)FUN_141eb3490(local_70,L"Controller_Btn_X",1);
    lVar2 = *plVar3;
    if ((int)local_60 != local_34) {
      puVar5 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar5 = local_28;
      }
      iVar1 = *(int *)(puVar5 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar2) {
          lVar6 = lVar6 + local_68;
          if (iVar1 != -1) goto LAB_1419e025d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar7;
LAB_1419e025d:
    puVar4 = (undefined8 *)FUN_142060720(&DAT_145b9fb68,local_78);
    *(undefined8 *)(lVar6 + 8) = *puVar4;
    plVar3 = (longlong *)FUN_141eb3490(local_70,L"Controller_Btn_A",1);
    lVar2 = *plVar3;
    if ((int)local_60 != local_34) {
      puVar5 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar5 = local_28;
      }
      iVar1 = *(int *)(puVar5 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar2) {
          lVar6 = lVar6 + local_68;
          if (iVar1 != -1) goto LAB_1419e02fd;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar7;
LAB_1419e02fd:
    puVar4 = (undefined8 *)FUN_142060720(&DAT_145b9fb38,local_78);
    *(undefined8 *)(lVar6 + 8) = *puVar4;
    plVar3 = (longlong *)FUN_141eb3490(local_70,L"Controller_Btn_Y",1);
    lVar2 = *plVar3;
    if ((int)local_60 != local_34) {
      puVar5 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar5 = local_28;
      }
      iVar1 = *(int *)(puVar5 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar2) {
          lVar6 = lVar6 + local_68;
          if (iVar1 != -1) goto LAB_1419e039d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar7;
LAB_1419e039d:
    puVar4 = (undefined8 *)FUN_142060720(&DAT_145b9fb80,local_78);
    *(undefined8 *)(lVar6 + 8) = *puVar4;
    plVar3 = (longlong *)FUN_141eb3490(local_70,L"Controller_Btn_LB",1);
    lVar2 = *plVar3;
    if ((int)local_60 != local_34) {
      puVar5 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar5 = local_28;
      }
      iVar1 = *(int *)(puVar5 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar2) {
          lVar6 = lVar6 + local_68;
          if (iVar1 != -1) goto LAB_1419e043d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar7;
LAB_1419e043d:
    puVar4 = (undefined8 *)FUN_142060720(&DAT_145b9fb98,local_78);
    *(undefined8 *)(lVar6 + 8) = *puVar4;
    plVar3 = (longlong *)FUN_141eb3490(local_70,L"Controller_Btn_LT",1);
    lVar2 = *plVar3;
    if ((int)local_60 != local_34) {
      puVar5 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar5 = local_28;
      }
      iVar1 = *(int *)(puVar5 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar2) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e04dd;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar7;
LAB_1419e04dd:
    puVar4 = (undefined8 *)FUN_142060720(&DAT_145b9fbc8,local_78);
    *(undefined8 *)(lVar6 + 8) = *puVar4;
    plVar3 = (longlong *)FUN_141eb3490(local_70,L"Controller_Btn_L3",1);
    lVar2 = *plVar3;
    if ((int)local_60 != local_34) {
      puVar5 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar5 = local_28;
      }
      iVar1 = *(int *)(puVar5 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar2) {
          lVar6 = lVar6 + local_68;
          if (iVar1 != -1) goto LAB_1419e057d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar7;
LAB_1419e057d:
    puVar4 = (undefined8 *)FUN_142060720(&DAT_145b9faa8,local_78);
    *(undefined8 *)(lVar6 + 8) = *puVar4;
    plVar3 = (longlong *)FUN_141eb3490(local_70,L"Controller_Btn_RB",1);
    lVar2 = *plVar3;
    if ((int)local_60 != local_34) {
      puVar5 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar5 = local_28;
      }
      iVar1 = *(int *)(puVar5 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar2) {
          lVar6 = lVar6 + local_68;
          if (iVar1 != -1) goto LAB_1419e061d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar7;
LAB_1419e061d:
    puVar4 = (undefined8 *)FUN_142060720(&DAT_145b9fbb0,local_78);
    *(undefined8 *)(lVar6 + 8) = *puVar4;
    plVar3 = (longlong *)FUN_141eb3490(local_70,L"Controller_Btn_RT",1);
    lVar2 = *plVar3;
    if ((int)local_60 != local_34) {
      puVar5 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar5 = local_28;
      }
      iVar1 = *(int *)(puVar5 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar2) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e06bd;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar7;
LAB_1419e06bd:
    puVar4 = (undefined8 *)FUN_142060720(&DAT_145b9fbe0,local_78);
    *(undefined8 *)(lVar6 + 8) = *puVar4;
    plVar3 = (longlong *)FUN_141eb3490(local_70,L"Controller_Btn_R3",1);
    lVar2 = *plVar3;
    if ((int)local_60 != local_34) {
      puVar5 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar5 = local_28;
      }
      iVar1 = *(int *)(puVar5 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar2) {
          lVar6 = lVar6 + local_68;
          if (iVar1 != -1) goto LAB_1419e075d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar7;
LAB_1419e075d:
    puVar4 = (undefined8 *)FUN_142060720(&DAT_145b9fac0,local_78);
    *(undefined8 *)(lVar6 + 8) = *puVar4;
    plVar3 = (longlong *)FUN_141eb3490(local_70,L"Controller_Btn_Start",1);
    lVar2 = *plVar3;
    if ((int)local_60 != local_34) {
      puVar5 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar5 = local_28;
      }
      iVar1 = *(int *)(puVar5 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar2) {
          lVar6 = lVar6 + local_68;
          if (iVar1 != -1) goto LAB_1419e07fd;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar7;
LAB_1419e07fd:
    puVar4 = (undefined8 *)FUN_142060720(&DAT_145b9fb20,local_78);
    *(undefined8 *)(lVar6 + 8) = *puVar4;
    plVar3 = (longlong *)FUN_141eb3490(local_70,L"Controller_Btn_Back",1);
    lVar2 = *plVar3;
    if ((int)local_60 != local_34) {
      puVar5 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar5 = local_28;
      }
      iVar1 = *(int *)(puVar5 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar2) {
          lVar6 = lVar6 + local_68;
          if (iVar1 != -1) goto LAB_1419e089d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar7;
LAB_1419e089d:
    puVar4 = (undefined8 *)FUN_142060720(&DAT_145b9fad8,local_78);
    *(undefined8 *)(lVar6 + 8) = *puVar4;
    FUN_141a84ae0(&local_68);
  }
  local_20 = 0;
  if (local_28 != (undefined1 *)0x0) {
    FUN_141de9200();
  }
  FUN_1407fa390(&local_68);
  return;
}



// (already emitted) Controller_Btn_Start <- 1411dbcab -> FUN_1411dba70
// (already emitted) Controller_Btn_Start <- 1411df0c2 -> FUN_1411dba70
// (already emitted) Controller_Btn_Start <- 1411df0d6 -> FUN_1411dba70
// (already emitted) Controller_Btn_Start <- 1411df12d -> FUN_1411dba70
// (already emitted) Controller_Btn_Start <- 1411df141 -> FUN_1411dba70
// (already emitted) Controller_Btn_Start <- 1411e15df -> FUN_1411e0180
// ================= Controller_Btn_Start <- 14120b22e  FUN_14120b080 @ 14120b080  size=0x14b7 =================
// callers: <none>

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14120b080(undefined8 param_1,longlong param_2)

{
  bool bVar1;
  longlong lVar2;
  undefined8 *puVar3;
  longlong *plVar4;
  byte *pbVar5;
  undefined8 *puVar6;
  undefined8 uVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  longlong **pplVar10;
  char *pcVar11;
  undefined1 auStack_e8 [32];
  undefined1 **local_c8;
  undefined8 local_b8;
  undefined4 uStack_b0;
  undefined4 uStack_ac;
  undefined1 local_a8 [8];
  undefined1 local_a0 [8];
  undefined1 *local_98;
  undefined8 local_90;
  longlong *local_88;
  int local_80;
  undefined8 local_78;
  undefined4 local_70;
  longlong **local_68;
  undefined8 local_60;
  undefined1 local_58;
  ulonglong local_48;
  
  local_48 = DAT_145909780 ^ (ulonglong)auStack_e8;
  puVar8 = *(undefined8 **)(param_2 + 0x370);
  puVar9 = (undefined8 *)0x0;
  if (puVar8 != (undefined8 *)0x0) {
    lVar2 = FUN_1418fb190();
    if ((*(int *)(puVar8[2] + 0x90) < *(int *)(lVar2 + 0x90)) ||
       (*(longlong *)(*(longlong *)(puVar8[2] + 0x88) + (longlong)*(int *)(lVar2 + 0x90) * 8) !=
        lVar2 + 0x88)) {
      bVar1 = false;
    }
    else {
      bVar1 = true;
    }
    if (bVar1) goto LAB_14120b0f5;
  }
  puVar8 = puVar9;
LAB_14120b0f5:
  local_98 = (undefined1 *)((ulonglong)local_98 & 0xfffffffffffffefc);
  local_b8 = FUN_141438bd0;
  uStack_b0 = 0;
  local_98._0_3_ = (uint3)(ushort)local_98;
  puVar3 = (undefined8 *)FUN_141eb3410(local_a8,"PauseMenu",1);
  local_88 = (longlong *)0x0;
  local_80 = 0;
  local_78 = 0;
  local_90 = *puVar3;
  local_70 = 0;
  FUN_1420575e0(&local_68,0);
  local_60 = 0;
  local_58 = 0;
  FUN_1411ef520(&local_88);
  local_58 = 1;
  plVar4 = (longlong *)FUN_141df5850(0,0x30);
  if (plVar4 == (longlong *)0x0) {
    if ((local_80 != 0) && (local_88 != (longlong *)0x0)) {
      (**(code **)(*local_88 + 0x30))(local_88,0);
      if (local_88 != (longlong *)0x0) {
        local_88 = (longlong *)FUN_141df5850(local_88,0,0);
      }
      local_80 = 0;
    }
  }
  else {
    *plVar4 = (longlong)&PTR_FUN_143968d20;
    FUN_1420575e0(plVar4 + 1,puVar8);
    *(undefined4 *)(plVar4 + 2) = (undefined4)local_b8;
    *(undefined4 *)((longlong)plVar4 + 0x14) = local_b8._4_4_;
    *(undefined4 *)(plVar4 + 3) = uStack_b0;
    *(undefined4 *)((longlong)plVar4 + 0x1c) = uStack_ac;
    lVar2 = FUN_141db8700();
    plVar4[5] = lVar2;
    *plVar4 = (longlong)&PTR_FUN_143968d70;
    (*(code *)PTR_FUN_143968da8)(plVar4,&local_88);
    (**(code **)(*plVar4 + 0x30))(plVar4,0);
    lVar2 = FUN_141df5850(plVar4,0,0);
    if (lVar2 != 0) {
      FUN_141de9200(lVar2);
    }
  }
  pbVar5 = (byte *)FUN_142c367c0(param_1,&local_98);
  FUN_1407e59d0(&local_78);
  FUN_1407e59d0(&local_88);
  *pbVar5 = *pbVar5 | 2;
  FUN_141eb3490(local_a0,L"Controller_Btn_Start",1);
  local_68 = (longlong **)0x0;
  puVar6 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  puVar3 = puVar9;
  if (puVar6 != (undefined8 *)0x0) {
    puVar6[1] = puVar8;
    *puVar6 = &PTR_FUN_143969680;
    puVar3 = puVar6;
  }
  if (puVar3 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f98d0;
  }
  FUN_141eb3490(local_a8,L"PauseMenu",1);
  local_c8 = &local_98;
  FUN_141a584f0(param_1,local_a0,local_a8,0);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  uVar7 = FUN_140d29250();
  local_b8 = (code *)&LAB_1412998d0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_Y",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_1412998a0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_X",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299570;
  uStack_b0 = 0;
  if (DAT_14569c148 == '\0') {
    puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_B",1);
    local_c8 = (undefined1 **)&local_b8;
    pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
    pcVar11 = "Menu_PushPad_A";
  }
  else {
    puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_A");
    local_c8 = (undefined1 **)&local_b8;
    pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
    pcVar11 = "Menu_PushPad_B";
  }
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299540;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,pcVar11,1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299570;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushNotPad_Decide",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299540;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushNotPad_Cancel",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = FUN_141299870;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushPad_Up",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = FUN_1412995a0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushPad_Down",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = FUN_141299630;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushPad_Left",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = FUN_141299780;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushPad_Right",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_1412995d0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_LB",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299720;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_RB",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299600;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_LT",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299750;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_RT",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,0,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_1412997e0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"InputRightStickXAxis",1);
  pbVar5 = (byte *)FUN_140b1fbe0(param_1,*puVar8,uVar7,&local_b8);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299690;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"InputLeftStickXAxis",1);
  pbVar5 = (byte *)FUN_140b1fbe0(param_1,*puVar8,uVar7,&local_b8);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299810;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"InputRightStickYAxis",1);
  pbVar5 = (byte *)FUN_140b1fbe0(param_1,*puVar8,uVar7,&local_b8);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_1412996c0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"InputLeftStickYAxis",1);
  pbVar5 = (byte *)FUN_140b1fbe0(param_1,*puVar8,uVar7,&local_b8);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299cf0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_Y",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299cc0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_X",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_1412999f0;
  uStack_b0 = 0;
  if (DAT_14569c148 == '\0') {
    puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_B",1);
    local_c8 = (undefined1 **)&local_b8;
    pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
    pcVar11 = "Menu_PushPad_A";
  }
  else {
    puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_A",1);
    local_c8 = (undefined1 **)&local_b8;
    pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
    pcVar11 = "Menu_PushPad_B";
  }
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_1412999c0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,pcVar11,1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_1412999f0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushNotPad_Decide",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_1412999c0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushNotPad_Cancel",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = FUN_141299c90;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushPad_Up",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = FUN_141299a20;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushPad_Down",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = FUN_141299ab0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushPad_Left",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = FUN_141299c30;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushPad_Right",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299a50;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_LB",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299bd0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_RB",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299a80;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_LT",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = (code *)&LAB_141299c00;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"Menu_PushPad_RT",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,1,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = FUN_141299870;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushPad_Up",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,2,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = FUN_1412995a0;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushPad_Down",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,2,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = FUN_141299630;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushPad_Left",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,2,uVar7);
  *pbVar5 = *pbVar5 | 2;
  local_b8 = FUN_141299780;
  uStack_b0 = 0;
  puVar8 = (undefined8 *)FUN_141eb3410(local_a8,"PushPad_Right",1);
  local_c8 = (undefined1 **)&local_b8;
  pbVar5 = (byte *)FUN_1411c84e0(param_1,*puVar8,2,uVar7);
  local_68 = (longlong **)0x0;
  *pbVar5 = *pbVar5 | 2;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  puVar8 = puVar9;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = uVar7;
    *puVar3 = &PTR_FUN_143969680;
    puVar8 = puVar3;
  }
  if (puVar8 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f8cd0;
  }
  FUN_141eb3490(local_a8,L"Move_Up",1);
  FUN_141a58370(param_1,local_a8,0,&local_98);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_68 = (longlong **)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  puVar8 = puVar9;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = uVar7;
    *puVar3 = &PTR_FUN_143969680;
    puVar8 = puVar3;
  }
  if (puVar8 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f9200;
  }
  FUN_141eb3490(local_a8,L"Move_Down",1);
  FUN_141a58370(param_1,local_a8,0,&local_98);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_68 = (longlong **)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  puVar8 = puVar9;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = uVar7;
    *puVar3 = &PTR_FUN_143969680;
    puVar8 = puVar3;
  }
  if (puVar8 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f9750;
  }
  FUN_141eb3490(local_a8,L"Move_Left",1);
  FUN_141a58370(param_1,local_a8,0,&local_98);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_68 = (longlong **)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  puVar8 = puVar9;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = uVar7;
    *puVar3 = &PTR_FUN_143969680;
    puVar8 = puVar3;
  }
  if (puVar8 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f8a10;
  }
  FUN_141eb3490(local_a8,L"Move_Right",1);
  FUN_141a58370(param_1,local_a8,0,&local_98);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_68 = (longlong **)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  puVar8 = puVar9;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = uVar7;
    *puVar3 = &PTR_FUN_143969680;
    puVar8 = puVar3;
  }
  if (puVar8 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f9c60;
  }
  FUN_141eb3490(local_a8,L"Move_Up",1);
  FUN_141a58370(param_1,local_a8,1,&local_98);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_68 = (longlong **)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  puVar8 = puVar9;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = uVar7;
    *puVar3 = &PTR_FUN_143969680;
    puVar8 = puVar3;
  }
  if (puVar8 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f98c0;
  }
  FUN_141eb3490(local_a8,L"Move_Down",1);
  FUN_141a58370(param_1,local_a8,1,&local_98);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_68 = (longlong **)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  puVar8 = puVar9;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = uVar7;
    *puVar3 = &PTR_FUN_143969680;
    puVar8 = puVar3;
  }
  if (puVar8 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f8b10;
  }
  FUN_141eb3490(local_a8,L"Move_Left",1);
  FUN_141a58370(param_1,local_a8,1,&local_98);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_68 = (longlong **)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  puVar8 = puVar9;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = uVar7;
    *puVar3 = &PTR_FUN_143969680;
    puVar8 = puVar3;
  }
  if (puVar8 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f90f0;
  }
  FUN_141eb3490(local_a8,L"Move_Right",1);
  FUN_141a58370(param_1,local_a8,1,&local_98);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_68 = (longlong **)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  puVar8 = puVar9;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = uVar7;
    *puVar3 = &PTR_FUN_143969680;
    puVar8 = puVar3;
  }
  if (puVar8 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f8cd0;
  }
  FUN_141eb3490(local_a8,L"Move_Up",1);
  FUN_141a58370(param_1,local_a8,2,&local_98);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_68 = (longlong **)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  puVar8 = puVar9;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = uVar7;
    *puVar3 = &PTR_FUN_143969680;
    puVar8 = puVar3;
  }
  if (puVar8 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f9200;
  }
  FUN_141eb3490(local_a8,L"Move_Down",1);
  FUN_141a58370(param_1,local_a8,2,&local_98);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_68 = (longlong **)0x0;
  puVar3 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  puVar8 = puVar9;
  if (puVar3 != (undefined8 *)0x0) {
    puVar3[1] = uVar7;
    *puVar3 = &PTR_FUN_143969680;
    puVar8 = puVar3;
  }
  if (puVar8 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f9750;
  }
  FUN_141eb3490(local_a8,L"Move_Left",1);
  FUN_141a58370(param_1,local_a8,2,&local_98);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  local_68 = (longlong **)0x0;
  puVar8 = (undefined8 *)FUN_1407e5ee0(0x10,&local_88);
  if (puVar8 != (undefined8 *)0x0) {
    puVar8[1] = uVar7;
    *puVar8 = &PTR_FUN_143969680;
    puVar9 = puVar8;
  }
  if (puVar9 != (undefined8 *)0xfffffffffffffff8) {
    local_98 = &LAB_1411f8a10;
  }
  FUN_141eb3490(local_a8,L"Move_Right",1);
  FUN_141a58370(param_1,local_a8,2,&local_98);
  if (local_98 != (undefined1 *)0x0) {
    pplVar10 = &local_88;
    if (local_68 != (longlong **)0x0) {
      pplVar10 = local_68;
    }
    (*(code *)(*pplVar10)[2])(pplVar10,0);
  }
  if (local_68 != (longlong **)0x0) {
    FUN_141de9200();
  }
  return;
}



// (already emitted) Controller_Btn_Start <- 1411ff48c -> FUN_1411ff170
// (already emitted) Controller_Btn_Start <- 14120eb9e -> FUN_14120d920
// (already emitted) Controller_Btn_Start <- 1419e0773 -> FUN_1419e00e0
// (already emitted) Controller_Btn_Start <- 1419e6645 -> FUN_1419e48a0
// (already emitted) Controller_Btn_Start <- 1419e666f -> FUN_1419e48a0
// (already emitted) Controller_Btn_Start <- 1419d6ebf -> FUN_1419d65b0
// (already emitted) Controller_Btn_Start <- 1419d6ec9 -> FUN_1419d65b0
// (already emitted) Controller_Btn_Start <- 1419cf9ce -> FUN_1419cf770
// (already emitted) Controller_Btn_R3 <- 1411dbee5 -> FUN_1411dba70
// (already emitted) Controller_Btn_R3 <- 1411e16d3 -> FUN_1411e0180
// (already emitted) Controller_Btn_R3 <- 141205fca -> FUN_1412032b0
// (already emitted) Controller_Btn_R3 <- 14120e1ba -> FUN_14120d920
// (already emitted) Controller_Btn_R3 <- 1419e06d3 -> FUN_1419e00e0
// (already emitted) Controller_Btn_R3 <- 1419e65d2 -> FUN_1419e48a0
// (already emitted) Controller_Btn_R3 <- 1419e65fc -> FUN_1419e48a0
// (already emitted) Controller_Btn_R3 <- 1419d6e96 -> FUN_1419d65b0
// (already emitted) Controller_Btn_R3 <- 1419d6ea0 -> FUN_1419d65b0
// (already emitted) Controller_Btn_R3 <- 1419cf9b5 -> FUN_1419cf770
