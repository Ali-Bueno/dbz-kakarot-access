// ================= TARGET 0x141a82450  FUN_141a82450 @ 141a82450  size=0x34 =================
// callers: FUN_1419e0920@1419e0920 FUN_141b6d1f0@141b6d1f0 FUN_1419df290@1419df290 FUN_1419e00e0@1419e00e0 FUN_141b7b9b0@141b7b9b0 FUN_1419cf770@1419cf770 FUN_1419e1a40@1419e1a40 FUN_141b7c090@141b7c090 FUN_1419debf0@1419debf0 

void FUN_141a82450(undefined8 param_1)

{
  FUN_140c052a0(&DAT_14569c360,param_1);
  FUN_140c052a0(&DAT_14569c3b0,param_1);
  DAT_14569c149 = 1;
  return;
}



// =================   CALLER-OF 0x141a82450  FUN_1419e0920 @ 1419e0920  size=0x111c =================
// strings: [Walk, Move_Up, Move_Left, Move_Down, Move_Right, Boost, BoostUp, Jump, Fly_Rolling, Fly_Ascend, Fly_Descend, KiSearch, TargetCursor, KiBlust, MapChange, Ride, Ride_Off, RideChange_Left, RideChange_Right, MineralRadar, Car_Forward, Car_Reverse, Car_ChangeView, Car_Sound_Left, Car_Sound_Right, BaseBall_Swing, FastForward, TalkAutoFeed]
// callers: FUN_1416802b0@1416802b0 

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1419e0920(longlong param_1,char param_2)

{
  int iVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  undefined1 *puVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  undefined1 auStack_a8 [32];
  int local_88 [2];
  undefined1 local_80 [8];
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
  
  local_18 = DAT_145909780 ^ (ulonglong)auStack_a8;
  lVar5 = 0;
  local_68 = 0;
  local_60 = 0;
  local_48 = 0;
  local_40 = 0;
  local_38 = 0xffffffff;
  local_34 = 0;
  local_28 = (undefined1 *)0x0;
  local_20 = 0;
  if (param_2 == '\0') {
    FUN_1419e3970(param_1,*(undefined1 *)(param_1 + 0x178));
    FUN_1419d65b0(param_1,&local_68);
    FUN_141a82450(&local_68);
  }
  else {
    FUN_1419d65b0(param_1,&local_68);
    FUN_141eb3490(local_88,L"Walk",1);
    plVar2 = (longlong *)FUN_141eb3490(local_80,L"Walk",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0a1d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar6);
      }
    }
    lVar6 = lVar5;
LAB_1419e0a1d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_88,&DAT_145b9f6e8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Move_Up",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Move_Up",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0add;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar6);
      }
    }
    lVar6 = lVar5;
LAB_1419e0add:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f3b8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Move_Left",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Move_Left",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0b9d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e0b9d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f1a8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Move_Down",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Move_Down",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0c5d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e0c5d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f358,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Move_Right",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Move_Right",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0d1d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e0d1d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f1f0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Boost",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Boost",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0ddd;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e0ddd:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f1c0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"BoostUp",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"BoostUp",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0e9d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e0e9d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f250,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Jump",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Jump",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0f5d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e0f5d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9efb0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Fly_Rolling",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Fly_Rolling",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e101d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e101d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9eea8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Fly_Ascend",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Fly_Ascend",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e10dd;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar6);
      }
    }
    lVar6 = lVar5;
LAB_1419e10dd:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f208,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Fly_Descend",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Fly_Descend",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e119d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar6);
      }
    }
    lVar6 = lVar5;
LAB_1419e119d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f1d8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"KiSearch",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_78,L"KiSearch",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_70,local_80,&DAT_145b9f6d0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"TargetCursor",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"TargetCursor",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f220,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"KiBlust",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"KiBlust",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9eec0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"MapChange",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"MapChange",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f2c8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Ride",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Ride",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f040,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Ride_Off",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Ride_Off",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f070,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"RideChange_Left",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"RideChange_Left",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f028,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"RideChange_Right",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"RideChange_Right",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f058,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"MineralRadar",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"MineralRadar",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f328,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Car_Forward",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Car_Forward",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9eea8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Car_Reverse",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Car_Reverse",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9eec0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Car_ChangeView",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Car_ChangeView",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9ef38,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Car_Sound_Left",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Car_Sound_Left",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f028,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Car_Sound_Right",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Car_Sound_Right",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f058,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"BaseBall_Swing",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"BaseBall_Swing",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9eea8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"FastForward",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"FastForward",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f400,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"TalkAutoFeed",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"TalkAutoFeed",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    if (local_88[0] != -1) {
      lVar5 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f3d0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141a84ae0(&local_68);
  }
  local_20 = 0;
  if (local_28 != (undefined1 *)0x0) {
    FUN_141de9200();
  }
  FUN_1407fa390(&local_68);
  return;
}



// =================   CALLER-OF 0x141a82450  FUN_141b6d1f0 @ 141b6d1f0  size=0x307 =================
// strings: [Left, Down, Right, Turn_End, Tab_Left, Tab_Right, Deck_Switch, Detail, Card_Delete, Card_Generate, Test_Battle, KeyConfig_Title]
// callers: FUN_141aec650@141aec650 

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141b6d1f0(undefined8 param_1,char param_2)

{
  undefined1 auStack_178 [32];
  undefined8 *local_158;
  undefined4 local_150;
  longlong local_148 [2];
  undefined1 local_138 [16];
  undefined8 local_128;
  undefined8 uStack_120;
  undefined8 local_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 uStack_100;
  undefined8 local_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  undefined8 uStack_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined8 local_c8 [2];
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 local_98;
  undefined8 local_90;
  undefined4 local_88;
  undefined4 local_84;
  longlong local_78;
  undefined4 local_70;
  undefined8 local_68;
  undefined8 local_60;
  undefined8 local_48;
  undefined8 local_40;
  undefined4 local_38;
  undefined4 local_34;
  longlong local_28;
  undefined4 local_20;
  ulonglong local_18;
  
  local_18 = DAT_145909780 ^ (ulonglong)auStack_178;
  if (param_2 != '\0') {
    FUN_141b7c1a0();
  }
  local_38 = 0xffffffff;
  local_68 = 0;
  local_60 = 0;
  local_48 = 0;
  local_40 = 0;
  local_34 = 0;
  local_28 = 0;
  local_20 = 0;
  local_128 = 0;
  uStack_120 = 0;
  local_118 = 0;
  uStack_110 = 0;
  local_108 = 0;
  uStack_100 = 0;
  local_f8 = 0;
  uStack_f0 = 0;
  local_e8 = 0;
  uStack_e0 = 0;
  local_d8 = 0;
  uStack_d0 = 0;
  local_c8[0] = 0;
  FUN_141b7c1a0(&local_128);
  FUN_141b74800(&local_128,&local_68);
  FUN_141a82490(&local_68);
  local_b8 = 0;
  local_b0 = 0;
  local_98 = 0;
  local_90 = 0;
  local_88 = 0xffffffff;
  local_84 = 0;
  local_78 = 0;
  local_70 = 0;
  FUN_141b74800(param_1,&local_b8);
  FUN_141a82450(&local_b8);
  FUN_141eb3490(&local_128,&DAT_144579ce4,1);
  FUN_141eb3490(&uStack_120,L"Left",1);
  FUN_141eb3490(&local_118,L"Down",1);
  FUN_141eb3490(&uStack_110,L"Right",1);
  FUN_141eb3490(&local_108,L"Turn_End",1);
  FUN_141eb3490(&uStack_100,L"Tab_Left",1);
  FUN_141eb3490(&local_f8,L"Tab_Right",1);
  FUN_141eb3490(&uStack_f0,L"Deck_Switch",1);
  FUN_141eb3490(&local_e8,L"Detail",1);
  FUN_141eb3490(&uStack_e0,&DAT_14457a3c8,1);
  FUN_141eb3490(&local_d8,L"Card_Delete",1);
  FUN_141eb3490(&uStack_d0,L"Card_Generate",1);
  FUN_141eb3490(local_c8,L"Test_Battle",1);
  FUN_141eb3490(local_138,L"KeyConfig_Title",1);
  local_158 = (undefined8 *)0x0;
  local_150 = 0xd;
  FUN_140826f80(&local_158,0xd,0);
  *local_158 = local_128;
  local_158[1] = uStack_120;
  local_158[2] = local_118;
  local_158[3] = uStack_110;
  local_158[4] = local_108;
  local_158[5] = uStack_100;
  local_158[6] = local_f8;
  local_158[7] = uStack_f0;
  local_158[8] = local_e8;
  local_158[9] = uStack_e0;
  *(undefined4 *)(local_158 + 10) = (undefined4)local_d8;
  *(undefined4 *)((longlong)local_158 + 0x54) = local_d8._4_4_;
  *(undefined4 *)(local_158 + 0xb) = (undefined4)uStack_d0;
  *(undefined4 *)((longlong)local_158 + 0x5c) = uStack_d0._4_4_;
  local_158[0xc] = local_c8[0];
  FUN_141a824a0(local_138,&local_158);
  if (local_158 != (undefined8 *)0x0) {
    FUN_141de9200();
  }
  local_148[0] = 0;
  local_148[1] = 0;
  FUN_141b72b60(local_148);
  FUN_141a81fd0(local_148);
  FUN_141d05990();
  if (local_148[0] != 0) {
    FUN_141de9200();
  }
  local_70 = 0;
  if (local_78 != 0) {
    FUN_141de9200();
  }
  FUN_1407fa390(&local_b8);
  local_20 = 0;
  if (local_28 != 0) {
    FUN_141de9200();
  }
  FUN_1407fa390(&local_68);
  return;
}



// =================   CALLER-OF 0x141a82450  FUN_1419df290 @ 1419df290  size=0xcdb =================
// strings: [Battle_Boost, Battle_HighBoost, Battle_TargetChange_Left, Battle_TargetChange_Right, Battle_TargetCancel, Battle_MeleeAtk, Battle_KiAtk, Battle_KiRecover, Battle_MoveMode, Battle_Guard, Battle_Zcombo, Battle_FormChange_Palette, Battle_Support_Palette, Battle_ActiveSkill_Palette, Battle_Item_Palette, Battle_Palette_Up, Battle_Palette_Left, Battle_Palette_Right, Battle_Palette_Down]
// callers: FUN_1416802b0@1416802b0 

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1419df290(longlong param_1,char param_2)

{
  int iVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  undefined1 *puVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  undefined1 auStack_a8 [32];
  int local_88 [2];
  undefined1 local_80 [8];
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
  
  local_18 = DAT_145909780 ^ (ulonglong)auStack_a8;
  lVar7 = 0;
  local_68 = 0;
  local_60 = 0;
  local_48 = 0;
  local_40 = 0;
  local_38 = 0xffffffff;
  local_34 = 0;
  local_28 = (undefined1 *)0x0;
  local_20 = 0;
  if (param_2 == '\0') {
    FUN_1419e2230(param_1,*(undefined1 *)(param_1 + 0x178));
    FUN_1419d65b0(param_1,&local_68);
    FUN_141a82450(&local_68);
  }
  else {
    FUN_1419d65b0(param_1,&local_68);
    FUN_141eb3490(local_88,L"Battle_Boost",1);
    plVar2 = (longlong *)FUN_141eb3490(local_80,L"Battle_Boost",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar5 + local_68) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df38d;
          break;
        }
        iVar1 = *(int *)(lVar5 + 0x10 + local_68);
      }
    }
    lVar5 = lVar7;
LAB_1419df38d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_88,&DAT_145b9efb0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_HighBoost",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_HighBoost",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar5 + local_68) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df44d;
          break;
        }
        iVar1 = *(int *)(lVar5 + 0x10 + local_68);
      }
    }
    lVar5 = lVar7;
LAB_1419df44d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f1c0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_TargetChange_Left",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_TargetChange_Left",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar5 + local_68) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df50d;
          break;
        }
        iVar1 = *(int *)(lVar5 + 0x10 + local_68);
      }
    }
    lVar5 = lVar7;
LAB_1419df50d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9ee60,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_TargetChange_Right",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_TargetChange_Right",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar5 + local_68) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df5cd;
          break;
        }
        iVar1 = *(int *)(lVar5 + 0x10 + local_68);
      }
    }
    lVar5 = lVar7;
LAB_1419df5cd:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9ee78,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_TargetCancel",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_TargetCancel",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df68d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419df68d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9eed8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_MeleeAtk",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_MeleeAtk",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df74d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419df74d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9eea8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_KiAtk",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_KiAtk",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = local_68 + lVar5;
          if (iVar1 != -1) goto LAB_1419df80d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419df80d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9eec0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_KiRecover",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_KiRecover",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df8cd;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419df8cd:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f340,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_MoveMode",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_MoveMode",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df98d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419df98d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f6d0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_Guard",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_Guard",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419dfa4d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419dfa4d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f220,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_Zcombo",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_Zcombo",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419dfb0d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419dfb0d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f2e0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_FormChange_Palette",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_78,L"Battle_FormChange_Palette",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_70,local_80,&DAT_145b9ef38,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_Support_Palette",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_Support_Palette",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f3d0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_ActiveSkill_Palette",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_ActiveSkill_Palette",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f3a0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_Item_Palette",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_Item_Palette",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f1d8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_Palette_Up",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_Palette_Up",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f0d0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_Palette_Left",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_Palette_Left",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f0e8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_Palette_Right",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_Palette_Right",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f100,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_Palette_Down",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_Palette_Down",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f118,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141a84ae0(&local_68);
  }
  local_20 = 0;
  if (local_28 != (undefined1 *)0x0) {
    FUN_141de9200();
  }
  FUN_1407fa390(&local_68);
  return;
}



// ================= TARGET 0x141a84ae0  FUN_141a84ae0 @ 141a84ae0  size=0x1f =================
// callers: FUN_1419e0920@1419e0920 FUN_1419df290@1419df290 FUN_141d0c1f0@141d0c1f0 FUN_1419e00e0@1419e00e0 

void FUN_141a84ae0(undefined8 param_1)

{
  FUN_140c052a0(&DAT_14569c3b0,param_1);
  DAT_14569c149 = 0;
  return;
}



// =================   CALLER-OF 0x141a84ae0  FUN_1419e0920 @ 1419e0920  size=0x111c =================
// strings: [Walk, Move_Up, Move_Left, Move_Down, Move_Right, Boost, BoostUp, Jump, Fly_Rolling, Fly_Ascend, Fly_Descend, KiSearch, TargetCursor, KiBlust, MapChange, Ride, Ride_Off, RideChange_Left, RideChange_Right, MineralRadar, Car_Forward, Car_Reverse, Car_ChangeView, Car_Sound_Left, Car_Sound_Right, BaseBall_Swing, FastForward, TalkAutoFeed]
// callers: FUN_1416802b0@1416802b0 

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1419e0920(longlong param_1,char param_2)

{
  int iVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  undefined1 *puVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  undefined1 auStack_a8 [32];
  int local_88 [2];
  undefined1 local_80 [8];
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
  
  local_18 = DAT_145909780 ^ (ulonglong)auStack_a8;
  lVar5 = 0;
  local_68 = 0;
  local_60 = 0;
  local_48 = 0;
  local_40 = 0;
  local_38 = 0xffffffff;
  local_34 = 0;
  local_28 = (undefined1 *)0x0;
  local_20 = 0;
  if (param_2 == '\0') {
    FUN_1419e3970(param_1,*(undefined1 *)(param_1 + 0x178));
    FUN_1419d65b0(param_1,&local_68);
    FUN_141a82450(&local_68);
  }
  else {
    FUN_1419d65b0(param_1,&local_68);
    FUN_141eb3490(local_88,L"Walk",1);
    plVar2 = (longlong *)FUN_141eb3490(local_80,L"Walk",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0a1d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar6);
      }
    }
    lVar6 = lVar5;
LAB_1419e0a1d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_88,&DAT_145b9f6e8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Move_Up",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Move_Up",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0add;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar6);
      }
    }
    lVar6 = lVar5;
LAB_1419e0add:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f3b8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Move_Left",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Move_Left",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0b9d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e0b9d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f1a8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Move_Down",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Move_Down",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0c5d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e0c5d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f358,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Move_Right",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Move_Right",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0d1d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e0d1d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f1f0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Boost",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Boost",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0ddd;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e0ddd:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f1c0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"BoostUp",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"BoostUp",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0e9d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e0e9d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f250,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Jump",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Jump",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e0f5d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e0f5d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9efb0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Fly_Rolling",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Fly_Rolling",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar6 + local_68) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e101d;
          break;
        }
        iVar1 = *(int *)(lVar6 + 0x10 + local_68);
      }
    }
    lVar6 = lVar5;
LAB_1419e101d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9eea8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Fly_Ascend",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Fly_Ascend",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e10dd;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar6);
      }
    }
    lVar6 = lVar5;
LAB_1419e10dd:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f208,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Fly_Descend",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Fly_Descend",1);
    lVar7 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar7 >> 0x20) + (int)lVar7) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar6 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar7) {
          lVar6 = local_68 + lVar6;
          if (iVar1 != -1) goto LAB_1419e119d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar6);
      }
    }
    lVar6 = lVar5;
LAB_1419e119d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f1d8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"KiSearch",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_78,L"KiSearch",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_70,local_80,&DAT_145b9f6d0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"TargetCursor",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"TargetCursor",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f220,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"KiBlust",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"KiBlust",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9eec0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"MapChange",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"MapChange",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f2c8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Ride",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Ride",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f040,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Ride_Off",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Ride_Off",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f070,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"RideChange_Left",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"RideChange_Left",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f028,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"RideChange_Right",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"RideChange_Right",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f058,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"MineralRadar",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"MineralRadar",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f328,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Car_Forward",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Car_Forward",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9eea8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Car_Reverse",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Car_Reverse",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9eec0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Car_ChangeView",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Car_ChangeView",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9ef38,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Car_Sound_Left",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Car_Sound_Left",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f028,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Car_Sound_Right",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Car_Sound_Right",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f058,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"BaseBall_Swing",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"BaseBall_Swing",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9eea8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"FastForward",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"FastForward",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar7 = lVar5;
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f400,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"TalkAutoFeed",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"TalkAutoFeed",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    if (local_88[0] != -1) {
      lVar5 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f3d0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141a84ae0(&local_68);
  }
  local_20 = 0;
  if (local_28 != (undefined1 *)0x0) {
    FUN_141de9200();
  }
  FUN_1407fa390(&local_68);
  return;
}



// =================   CALLER-OF 0x141a84ae0  FUN_1419df290 @ 1419df290  size=0xcdb =================
// strings: [Battle_Boost, Battle_HighBoost, Battle_TargetChange_Left, Battle_TargetChange_Right, Battle_TargetCancel, Battle_MeleeAtk, Battle_KiAtk, Battle_KiRecover, Battle_MoveMode, Battle_Guard, Battle_Zcombo, Battle_FormChange_Palette, Battle_Support_Palette, Battle_ActiveSkill_Palette, Battle_Item_Palette, Battle_Palette_Up, Battle_Palette_Left, Battle_Palette_Right, Battle_Palette_Down]
// callers: FUN_1416802b0@1416802b0 

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1419df290(longlong param_1,char param_2)

{
  int iVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  undefined1 *puVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  undefined1 auStack_a8 [32];
  int local_88 [2];
  undefined1 local_80 [8];
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
  
  local_18 = DAT_145909780 ^ (ulonglong)auStack_a8;
  lVar7 = 0;
  local_68 = 0;
  local_60 = 0;
  local_48 = 0;
  local_40 = 0;
  local_38 = 0xffffffff;
  local_34 = 0;
  local_28 = (undefined1 *)0x0;
  local_20 = 0;
  if (param_2 == '\0') {
    FUN_1419e2230(param_1,*(undefined1 *)(param_1 + 0x178));
    FUN_1419d65b0(param_1,&local_68);
    FUN_141a82450(&local_68);
  }
  else {
    FUN_1419d65b0(param_1,&local_68);
    FUN_141eb3490(local_88,L"Battle_Boost",1);
    plVar2 = (longlong *)FUN_141eb3490(local_80,L"Battle_Boost",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar5 + local_68) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df38d;
          break;
        }
        iVar1 = *(int *)(lVar5 + 0x10 + local_68);
      }
    }
    lVar5 = lVar7;
LAB_1419df38d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_88,&DAT_145b9efb0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_HighBoost",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_HighBoost",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar5 + local_68) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df44d;
          break;
        }
        iVar1 = *(int *)(lVar5 + 0x10 + local_68);
      }
    }
    lVar5 = lVar7;
LAB_1419df44d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f1c0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_TargetChange_Left",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_TargetChange_Left",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar5 + local_68) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df50d;
          break;
        }
        iVar1 = *(int *)(lVar5 + 0x10 + local_68);
      }
    }
    lVar5 = lVar7;
LAB_1419df50d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9ee60,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_TargetChange_Right",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_TargetChange_Right",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(lVar5 + local_68) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df5cd;
          break;
        }
        iVar1 = *(int *)(lVar5 + 0x10 + local_68);
      }
    }
    lVar5 = lVar7;
LAB_1419df5cd:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9ee78,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_TargetCancel",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_TargetCancel",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df68d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419df68d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9eed8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_MeleeAtk",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_MeleeAtk",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df74d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419df74d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9eea8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_KiAtk",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_KiAtk",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = local_68 + lVar5;
          if (iVar1 != -1) goto LAB_1419df80d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419df80d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9eec0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_KiRecover",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_KiRecover",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df8cd;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419df8cd:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f340,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_MoveMode",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_MoveMode",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419df98d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419df98d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f6d0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_Guard",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_Guard",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419dfa4d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419dfa4d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f220,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_88,L"Battle_Zcombo",1);
    plVar2 = (longlong *)FUN_141eb3490(local_78,L"Battle_Zcombo",1);
    lVar6 = *plVar2;
    if ((int)local_60 != local_34) {
      puVar4 = local_30;
      if (local_28 != (undefined1 *)0x0) {
        puVar4 = local_28;
      }
      iVar1 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar6 >> 0x20) + (int)lVar6) &
                                (longlong)local_20 - 1U) * 4);
      while (iVar1 != -1) {
        lVar5 = (longlong)iVar1 * 0x18;
        if (*(longlong *)(local_68 + (longlong)iVar1 * 0x18) == lVar6) {
          lVar5 = lVar5 + local_68;
          if (iVar1 != -1) goto LAB_1419dfb0d;
          break;
        }
        iVar1 = *(int *)(local_68 + 0x10 + lVar5);
      }
    }
    lVar5 = lVar7;
LAB_1419dfb0d:
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_80,local_88,&DAT_145b9f2e0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar5 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_FormChange_Palette",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_78,L"Battle_FormChange_Palette",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_70,local_80,&DAT_145b9ef38,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_Support_Palette",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_Support_Palette",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f3d0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_ActiveSkill_Palette",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_ActiveSkill_Palette",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f3a0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_Item_Palette",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_Item_Palette",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f1d8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_Palette_Up",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_Palette_Up",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f0d0,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_Palette_Left",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_Palette_Left",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f0e8,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_Palette_Right",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_Palette_Right",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    lVar6 = lVar7;
    if (local_88[0] != -1) {
      lVar6 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f100,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar6 + 8) = *puVar3;
    FUN_141eb3490(local_80,L"Battle_Palette_Down",1);
    puVar3 = (undefined8 *)FUN_141eb3490(local_70,L"Battle_Palette_Down",1);
    FUN_140b349f0(&local_68,local_88,*puVar3);
    if (local_88[0] != -1) {
      lVar7 = local_68 + (longlong)local_88[0] * 0x18;
    }
    puVar3 = (undefined8 *)
             FUN_140d26f70(local_78,local_80,&DAT_145b9f118,*(undefined1 *)(param_1 + 0x178));
    *(undefined8 *)(lVar7 + 8) = *puVar3;
    FUN_141a84ae0(&local_68);
  }
  local_20 = 0;
  if (local_28 != (undefined1 *)0x0) {
    FUN_141de9200();
  }
  FUN_1407fa390(&local_68);
  return;
}



// =================   CALLER-OF 0x141a84ae0  FUN_141d0c1f0 @ 141d0c1f0  size=0x105a =================
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



// ================= TARGET 0x141a57b50  FUN_141a57b50 @ 141a57b50  size=0x23 =================
// callers: FUN_141d0c1f0@141d0c1f0 FUN_1416802b0@1416802b0 

void FUN_141a57b50(void)

{
  FUN_140c052a0(&DAT_14569c360,&DAT_14569c3b0);
  DAT_14569c149 = 1;
  return;
}



// =================   CALLER-OF 0x141a57b50  FUN_141d0c1f0 @ 141d0c1f0  size=0x105a =================
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



// =================   CALLER-OF 0x141a57b50  FUN_1416802b0 @ 1416802b0  size=0xfbc =================
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



// ================= TARGET 0x141a8e3b0  FUN_141a8e3b0 @ 141a8e3b0  size=0x23 =================
// callers: FUN_141d0c1f0@141d0c1f0 FUN_1416802b0@1416802b0 

void FUN_141a8e3b0(void)

{
  FUN_140c052a0(&DAT_14569c3b0,&DAT_14569c360);
  DAT_14569c149 = 1;
  return;
}



// =================   CALLER-OF 0x141a8e3b0  FUN_141d0c1f0 @ 141d0c1f0  size=0x105a =================
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



// =================   CALLER-OF 0x141a8e3b0  FUN_1416802b0 @ 1416802b0  size=0xfbc =================
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



// ================= TARGET 0x141a51e70  FUN_141a51e70 @ 141a51e70  size=0x46b =================
// callers: thunk_FUN_141a51e70@141a58a00 

/* WARNING: Restarted to delay deadcode elimination for space: ram */

void FUN_141a51e70(longlong param_1)

{
  longlong lVar1;
  uint uVar2;
  int iVar3;
  longlong lVar4;
  int iVar5;
  undefined *puVar6;
  longlong *plVar7;
  longlong *plVar8;
  int iVar9;
  longlong *plVar10;
  uint *puVar11;
  longlong *plVar12;
  longlong lVar13;
  int local_res8 [2];
  longlong *local_res10;
  uint local_a8;
  uint uStack_a4;
  int iStack_94;
  uint uStack_90;
  undefined4 uStack_8c;
  uint uStack_80;
  uint uStack_7c;
  undefined *local_78;
  uint uStack_70;
  int iStack_6c;
  undefined8 local_68;
  longlong *local_60;
  undefined8 uStack_58;
  undefined *puStack_50;
  uint uStack_48;
  int iStack_44;
  undefined8 uStack_40;
  
  lVar4 = *(longlong *)(param_1 + 8);
  plVar10 = (longlong *)0x0;
  if (DAT_14569c3b8 != DAT_14569c3e4) {
    puVar6 = DAT_14569c3f0;
    if (DAT_14569c3f0 == (undefined *)0x0) {
      puVar6 = &DAT_14569c3e8;
    }
    iVar9 = *(int *)(puVar6 + ((longlong)((int)((ulonglong)lVar4 >> 0x20) + (int)lVar4) &
                              (longlong)(int)DAT_14569c3f8 - 1U) * 4);
    while (iVar9 != -1) {
      lVar13 = (longlong)iVar9 * 0x18;
      if (*(longlong *)(lVar13 + DAT_14569c3b0) == lVar4) {
        if ((iVar9 != -1) && (lVar13 = lVar13 + DAT_14569c3b0, lVar13 != 0)) goto LAB_141a51f5f;
        break;
      }
      iVar9 = *(int *)(lVar13 + 0x10 + DAT_14569c3b0);
    }
  }
  local_res10 = (longlong *)(param_1 + 8);
  FUN_141a460d0(&DAT_14569c3b0,local_res8,&local_res10,0);
  lVar13 = DAT_14569c3b0 + (longlong)local_res8[0] * 0x18;
LAB_141a51f5f:
  if (*(longlong *)(param_1 + 0x10) != *(longlong *)(lVar13 + 8)) {
    if (*(char *)(param_1 + 100) != '\0') {
      lVar4 = *(longlong *)(param_1 + 8);
      if (DAT_14569c458 != DAT_14569c484) {
        puVar6 = DAT_14569c490;
        if (DAT_14569c490 == (undefined *)0x0) {
          puVar6 = &DAT_14569c488;
        }
        iVar9 = *(int *)(puVar6 + ((longlong)((int)((ulonglong)lVar4 >> 0x20) + (int)lVar4) &
                                  (longlong)DAT_14569c498 - 1U) * 4);
        while (iVar9 != -1) {
          lVar1 = (longlong)iVar9 * 0x18;
          if (*(longlong *)(lVar1 + DAT_14569c450) == lVar4) {
            plVar7 = (longlong *)(lVar1 + DAT_14569c450);
            if (iVar9 != -1) goto LAB_141a5200d;
            break;
          }
          iVar9 = *(int *)(lVar1 + 0x10 + DAT_14569c450);
        }
      }
      plVar7 = plVar10;
LAB_141a5200d:
      plVar12 = plVar7 + 1;
      if (plVar7 == (longlong *)0x0) {
        plVar12 = plVar10;
      }
      if (plVar12 != (longlong *)0x0) {
        local_a8 = 0;
        uStack_a4 = 1;
        iStack_94 = 0;
        uStack_90 = 0;
        iVar9 = (int)DAT_14569c478;
        if (iVar9 != 0) {
          puVar11 = DAT_14569c470;
          if (DAT_14569c470 == (uint *)0x0) {
            puVar11 = (uint *)&DAT_14569c460;
          }
          uVar2 = *puVar11;
          plVar7 = plVar10;
          plVar8 = plVar10;
          while (uVar2 == 0) {
            iVar5 = (int)plVar7;
            uStack_90 = (int)plVar8 + 0x20;
            plVar8 = (longlong *)(ulonglong)uStack_90;
            local_a8 = iVar5 + 1;
            plVar7 = (longlong *)(ulonglong)local_a8;
            iStack_94 = iVar9;
            if ((int)(iVar9 + -1 + (iVar9 + -1 >> 0x1f & 0x1fU)) >> 5 <= iVar5) goto LAB_141a520e5;
            uVar2 = puVar11[(longlong)iVar5 + 1];
          }
          uStack_a4 = -uVar2 & uVar2;
          iVar5 = 0x1f;
          if (uStack_a4 != 0) {
            for (; uStack_a4 >> iVar5 == 0; iVar5 = iVar5 + -1) {
            }
          }
          if (uStack_a4 == 0) {
            iVar5 = 0x20;
          }
          else {
            iVar5 = 0x1f - iVar5;
          }
          iStack_94 = (uStack_90 - iVar5) + 0x1f;
          if (iVar9 < iStack_94) {
            iStack_94 = iVar9;
          }
        }
LAB_141a520e5:
        uStack_70 = 0xffffffff;
        iStack_6c = iStack_94;
        local_68 = CONCAT44(uStack_8c,uStack_90);
        uStack_58 = CONCAT44(uStack_a4,local_a8);
        puStack_50 = &DAT_14569c460;
        local_60 = &DAT_14569c450;
        uStack_80 = local_a8;
        uStack_7c = uStack_a4;
        local_78 = &DAT_14569c460;
        uStack_40 = local_68;
        uStack_48 = uStack_70;
        iStack_44 = iStack_6c;
        while( true ) {
          if ((iStack_6c == iVar9) && (local_78 == &DAT_14569c460)) break;
          plVar7 = (longlong *)(DAT_14569c450 + (longlong)iStack_6c * 0x18);
          lVar4 = *plVar7;
          if ((*(longlong *)(param_1 + 8) != lVar4) && (*plVar12 == plVar7[1])) {
            iVar5 = (int)((ulonglong)lVar4 >> 0x20);
            if (DAT_14569c3b8 != DAT_14569c3e4) {
              puVar6 = DAT_14569c3f0;
              if (DAT_14569c3f0 == (undefined *)0x0) {
                puVar6 = &DAT_14569c3e8;
              }
              iVar3 = *(int *)(puVar6 + (longlong)
                                        (int)(iVar5 + (int)lVar4 & (int)DAT_14569c3f8 - 1U) * 4);
              while (iVar3 != -1) {
                lVar1 = (longlong)iVar3 * 0x18;
                if (*(longlong *)(lVar1 + DAT_14569c3b0) == lVar4) {
                  plVar7 = (longlong *)(lVar1 + DAT_14569c3b0);
                  if (iVar3 != -1) goto LAB_141a521ed;
                  break;
                }
                iVar3 = *(int *)(lVar1 + 0x10 + DAT_14569c3b0);
              }
            }
            plVar7 = plVar10;
LAB_141a521ed:
            plVar8 = plVar7 + 1;
            if (plVar7 == (longlong *)0x0) {
              plVar8 = plVar10;
            }
            if ((plVar8 != (longlong *)0x0) && (*(longlong *)(param_1 + 0x10) == *plVar8)) {
              if (DAT_14569c3b8 != DAT_14569c3e4) {
                puVar6 = DAT_14569c3f0;
                if (DAT_14569c3f0 == (undefined *)0x0) {
                  puVar6 = &DAT_14569c3e8;
                }
                iVar5 = *(int *)(puVar6 + (longlong)
                                          (int)(iVar5 + (int)lVar4 & (int)DAT_14569c3f8 - 1U) * 4);
                while (iVar5 != -1) {
                  lVar1 = (longlong)iVar5 * 0x18;
                  if (*(longlong *)(lVar1 + DAT_14569c3b0) == lVar4) {
                    plVar7 = (longlong *)(lVar1 + DAT_14569c3b0);
                    if (iVar5 != -1) goto LAB_141a5226d;
                    break;
                  }
                  iVar5 = *(int *)(lVar1 + 0x10 + DAT_14569c3b0);
                }
              }
              plVar7 = plVar10;
LAB_141a5226d:
              plVar7[1] = *(longlong *)(lVar13 + 8);
            }
          }
          uStack_70 = uStack_70 & ~uStack_7c;
          FUN_1407e9ca0(&uStack_80);
        }
      }
    }
    *(undefined8 *)(lVar13 + 8) = *(undefined8 *)(param_1 + 0x10);
    DAT_14569c149 = 0;
  }
  return;
}



// =================   CALLER-OF 0x141a51e70  thunk_FUN_141a51e70 @ 141a58a00  size=0x5 =================
// callers: FUN_141a820d0@141a820d0 

void thunk_FUN_141a51e70(void)

{
  FUN_141a51e70();
  return;
}



// ================= TARGET 0x141a66300  FUN_141a66300 @ 141a66300  size=0xcb =================
// callers: FUN_141a66a80@141a66a80 FUN_141615dd0@141615dd0 FUN_141597810@141597810 FUN_141ceee50@141ceee50 FUN_14171c460@14171c460 FUN_14171c640@14171c640 FUN_141a65cf0@141a65cf0 

undefined8 * FUN_141a66300(undefined8 *param_1,longlong *param_2)

{
  longlong lVar1;
  int iVar2;
  longlong lVar3;
  undefined *puVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  
  lVar3 = *param_2;
  if (DAT_14569c3b8 != DAT_14569c3e4) {
    puVar4 = DAT_14569c3f0;
    if (DAT_14569c3f0 == (undefined *)0x0) {
      puVar4 = &DAT_14569c3e8;
    }
    iVar2 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar3 >> 0x20) + (int)lVar3) &
                              (longlong)(int)DAT_14569c3f8 - 1U) * 4);
    while (iVar2 != -1) {
      lVar1 = (longlong)iVar2 * 0x18;
      if (*(longlong *)(lVar1 + DAT_14569c3b0) == lVar3) {
        puVar6 = (undefined8 *)(lVar1 + DAT_14569c3b0);
        if (iVar2 != -1) goto LAB_141a6638e;
        break;
      }
      iVar2 = *(int *)(lVar1 + 0x10 + DAT_14569c3b0);
    }
  }
  puVar6 = (undefined8 *)0x0;
LAB_141a6638e:
  puVar5 = puVar6 + 1;
  if (puVar6 == (undefined8 *)0x0) {
    puVar5 = (undefined8 *)0x0;
  }
  if (puVar5 != (undefined8 *)0x0) {
    *param_1 = *puVar5;
    return param_1;
  }
  FUN_141eb3490(param_1,&DAT_1444fc0c4,1);
  return param_1;
}



// =================   CALLER-OF 0x141a66300  FUN_141a66a80 @ 141a66a80  size=0x3aa =================
// callers: FUN_141a56b80@141a56b80 

undefined8 FUN_141a66a80(longlong param_1,undefined8 *param_2,longlong *param_3,undefined1 *param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  void *_Src;
  int iVar4;
  int *piVar5;
  longlong *plVar6;
  undefined *puVar7;
  undefined8 *puVar8;
  undefined2 *puVar9;
  int iVar10;
  undefined2 *puVar11;
  longlong lVar12;
  undefined8 *puVar13;
  undefined8 *puVar14;
  undefined8 local_res8;
  longlong local_res10;
  longlong local_38 [2];
  
  puVar11 = &DAT_14391f7d0;
  if (*(int *)(param_2 + 1) == 0) {
    puVar9 = &DAT_14391f7d0;
  }
  else {
    puVar9 = (undefined2 *)*param_2;
  }
  iVar4 = FUN_1407e51a0(puVar9);
  if (param_4 != (undefined1 *)0x0) {
    *param_4 = 0;
  }
  if (0 < *(int *)(param_1 + 0x50)) {
    lVar12 = *(longlong *)(param_1 + 0x48);
    puVar14 = (undefined8 *)0x0;
    piVar5 = (int *)(lVar12 + 0x28);
    puVar8 = puVar14;
    puVar13 = puVar14;
    do {
      iVar10 = (int)puVar8;
      if (piVar5[6] == iVar4) {
        iVar1 = *piVar5;
        iVar2 = piVar5[4];
        iVar3 = piVar5[-4];
        if ((1 < iVar1) && (iVar3 < 2)) {
          if (*(int *)(lVar12 + 0x28 + (longlong)iVar10 * 0x48) != 0) {
            puVar11 = *(undefined2 **)(lVar12 + 0x20 + (longlong)iVar10 * 0x48);
          }
          FUN_141eb3490(&local_res10,puVar11,1);
          if (DAT_14569c3b8 == DAT_14569c3e4) goto LAB_141a66c4a;
          puVar7 = DAT_14569c3f0;
          if (DAT_14569c3f0 == (undefined *)0x0) {
            puVar7 = &DAT_14569c3e8;
          }
          iVar4 = *(int *)(puVar7 + ((longlong)
                                     ((int)((ulonglong)local_res10 >> 0x20) + (int)local_res10) &
                                    (longlong)(int)DAT_14569c3f8 - 1U) * 4);
          goto joined_r0x000141a66c0d;
        }
        if ((1 < iVar2) && (iVar3 < 2)) {
          if (*(int *)(lVar12 + 0x38 + (longlong)iVar10 * 0x48) != 0) {
            puVar11 = *(undefined2 **)(lVar12 + 0x30 + (longlong)iVar10 * 0x48);
          }
          FUN_141eb3490(&local_res10,puVar11,1);
          if (DAT_14569c3b8 == DAT_14569c3e4) goto LAB_141a66c4a;
          puVar7 = DAT_14569c3f0;
          if (DAT_14569c3f0 == (undefined *)0x0) {
            puVar7 = &DAT_14569c3e8;
          }
          iVar4 = *(int *)(puVar7 + ((longlong)
                                     ((int)((ulonglong)local_res10 >> 0x20) + (int)local_res10) &
                                    (longlong)(int)DAT_14569c3f8 - 1U) * 4);
          goto joined_r0x000141a66d49;
        }
        if ((iVar1 < 2) && (iVar2 < 2)) {
          if (1 < iVar3) {
LAB_141a66b29:
            plVar6 = (longlong *)(lVar12 + ((longlong)iVar10 * 9 + 2) * 8);
            if (param_3 != plVar6) {
              iVar4 = (int)plVar6[1];
              _Src = (void *)*plVar6;
              *(int *)(param_3 + 1) = iVar4;
              if ((iVar4 == 0) && (*(int *)((longlong)param_3 + 0xc) == 0)) {
                *(undefined4 *)((longlong)param_3 + 0xc) = 0;
              }
              else {
                FUN_1407f1420(param_3,iVar4);
                memcpy((void *)*param_3,_Src,(longlong)iVar4 * 2);
              }
            }
            if (param_4 == (undefined1 *)0x0) {
              return 1;
            }
            *param_4 = 0;
            return 1;
          }
        }
        else if (1 < iVar3) {
          if ((iVar1 < 2) || (DAT_145a33620 != 0)) {
            if ((iVar2 < 2) || (1 < DAT_145a33620 - 1U)) goto LAB_141a66b29;
            if (*(int *)(lVar12 + 0x38 + (longlong)iVar10 * 0x48) != 0) {
              puVar11 = *(undefined2 **)(lVar12 + 0x30 + (longlong)iVar10 * 0x48);
            }
          }
          else if (*(int *)(lVar12 + 0x28 + (longlong)iVar10 * 0x48) != 0) {
            puVar11 = *(undefined2 **)(lVar12 + 0x20 + (longlong)iVar10 * 0x48);
          }
          FUN_141eb3490(&local_res8,puVar11,1);
          puVar8 = (undefined8 *)FUN_141a66300(&local_res10,&local_res8);
          goto LAB_141a66c80;
        }
      }
      puVar8 = (undefined8 *)(ulonglong)(iVar10 + 1);
      puVar13 = (undefined8 *)((longlong)puVar13 + 1);
      piVar5 = piVar5 + 0x12;
    } while ((longlong)puVar13 < (longlong)*(int *)(param_1 + 0x50));
  }
  return 0;
joined_r0x000141a66c0d:
  if (iVar4 == -1) goto LAB_141a66c4a;
  lVar12 = (longlong)iVar4 * 0x18;
  if (*(longlong *)(lVar12 + DAT_14569c3b0) == local_res10) goto LAB_141a66c41;
  iVar4 = *(int *)(lVar12 + 0x10 + DAT_14569c3b0);
  goto joined_r0x000141a66c0d;
joined_r0x000141a66d49:
  if (iVar4 == -1) goto LAB_141a66c4a;
  lVar12 = (longlong)iVar4 * 0x18;
  if (*(longlong *)(lVar12 + DAT_14569c3b0) == local_res10) goto LAB_141a66c41;
  iVar4 = *(int *)(lVar12 + 0x10 + DAT_14569c3b0);
  goto joined_r0x000141a66d49;
LAB_141a66c41:
  puVar8 = (undefined8 *)(lVar12 + DAT_14569c3b0);
  if (iVar4 == -1) {
LAB_141a66c4a:
    puVar8 = puVar14;
  }
  puVar13 = puVar8 + 1;
  if (puVar8 == (undefined8 *)0x0) {
    puVar13 = puVar14;
  }
  if (puVar13 == (undefined8 *)0x0) {
    FUN_141eb3490(&local_res8,&DAT_1444fc0c4,1);
  }
  else {
    local_res8 = *puVar13;
  }
  puVar8 = &local_res8;
LAB_141a66c80:
  plVar6 = (longlong *)FUN_141eca8b0(puVar8,local_38);
  if (param_3 != plVar6) {
    if (*param_3 != 0) {
      FUN_141de9200();
    }
    *param_3 = *plVar6;
    *plVar6 = 0;
    *(int *)(param_3 + 1) = (int)plVar6[1];
    *(undefined4 *)((longlong)param_3 + 0xc) = *(undefined4 *)((longlong)plVar6 + 0xc);
    plVar6[1] = 0;
  }
  if (local_38[0] != 0) {
    FUN_141de9200();
  }
  if (param_4 != (undefined1 *)0x0) {
    *param_4 = 1;
  }
  return 1;
}



// =================   CALLER-OF 0x141a66300  FUN_141615dd0 @ 141615dd0  size=0x4e4 =================
// strings: [MSG_Guard_Operation, MSG_CommandList_ZCombination, Battle_Support_Palette, Battle_Palette_Right, MSG_CharacterChange_Operation, MSG_Slander, MSG_Chase, SG_Slander]
// callers: FUN_140ef6cc0@140ef6cc0 FUN_140ef6e40@140ef6e40 FUN_1414def20@1414def20 FUN_140ef6d80@140ef6d80 FUN_140ef6f00@140ef6f00 FUN_1414aaa80@1414aaa80 FUN_1414b7bb0@1414b7bb0 

void FUN_141615dd0(longlong *param_1,int param_2,undefined4 param_3)

{
  undefined1 *puVar1;
  bool bVar2;
  undefined4 uVar3;
  undefined1 uVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  char *pcVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined1 local_res8 [8];
  undefined1 local_res20 [8];
  undefined4 in_stack_ffffffffffffff98;
  undefined2 uVar11;
  longlong local_58 [8];
  
  uVar11 = (undefined2)((uint)in_stack_ffffffffffffff98 >> 0x10);
  if ((char)param_1[0xaa] != '\0') {
    (**(code **)(*param_1 + 0x250))(param_1,3);
    if (*(char *)((longlong)param_1 + 0x551) == '\0') {
      lVar6 = FUN_1414a4460();
      if (((lVar6 != 0) && (*(longlong *)(lVar6 + 0x2c0) != 0)) &&
         (*(char *)(*(longlong *)(lVar6 + 0x2c0) + 0x408) != '\0')) {
        FUN_1414ae530();
      }
      if ((param_2 == 1) && (lVar6 = FUN_140d2a220(), lVar6 != 0)) {
        lVar7 = FUN_141758410();
        if ((*(int *)(*(longlong *)(lVar6 + 0x10) + 0x90) < *(int *)(lVar7 + 0x90)) ||
           (*(longlong *)
             (*(longlong *)(*(longlong *)(lVar6 + 0x10) + 0x88) +
             (longlong)*(int *)(lVar7 + 0x90) * 8) != lVar7 + 0x88)) {
          bVar2 = false;
        }
        else {
          bVar2 = true;
        }
        if (((bVar2) && (iVar5 = FUN_140f0bfb0(lVar6), 0x2b < iVar5)) &&
           (iVar5 = FUN_140f0bfb0(lVar6), iVar5 < 0x76)) {
          return;
        }
      }
      *(undefined1 *)((longlong)param_1 + 0x551) = 1;
      *(int *)(param_1 + 0xab) = param_2;
      *(undefined4 *)((longlong)param_1 + 0x554) = 0;
      (**(code **)(*(longlong *)param_1[0x9b] + 0x250))((longlong *)param_1[0x9b],3);
      uVar3 = DAT_1450d6c88;
      if (param_2 == 1) {
        FUN_14171adf0(param_1[0x9e],param_1 + 0x83,0);
        (**(code **)(*(longlong *)param_1[0xa1] + 0x250))((longlong *)param_1[0xa1],1);
        local_58[0] = 0;
        local_58[1] = 0x14;
        FUN_1407e2ee0(local_58,0);
        FUN_1407e4a20(local_58[0],0x14,"MSG_Guard_Operation",0x14,CONCAT22(uVar11,0x3f));
        FUN_1416fb690(param_1[0xa0],local_58,1);
        uVar10 = DAT_143aa069c;
        if (local_58[0] != 0) {
          FUN_141de9200();
          uVar10 = DAT_143aa069c;
        }
      }
      else {
        uVar10 = uVar3;
        if (param_2 == 2) {
          FUN_14171adf0(param_1[0x9e],param_1 + 0x93,0);
          (**(code **)(*(longlong *)param_1[0xa1] + 0x250))((longlong *)param_1[0xa1],1);
          (**(code **)(*(longlong *)param_1[0x9d] + 0x250))((longlong *)param_1[0x9d],1);
          (**(code **)(*(longlong *)param_1[0x9f] + 0x250))((longlong *)param_1[0x9f],1);
          local_58[0] = 0;
          local_58[1] = 0x1d;
          FUN_1407e2ee0(local_58,0);
          FUN_1407e4a20(local_58[0],0x1d,"MSG_CommandList_ZCombination",0x1d,CONCAT22(uVar11,0x3f));
          FUN_1416fb690(param_1[0xa0],local_58,1);
          if (local_58[0] != 0) {
            FUN_141de9200();
          }
        }
        else if (param_2 == 3) {
          FUN_141eb3410(local_res8,"Battle_Support_Palette",1);
          FUN_141a66300(local_res20,local_res8);
          FUN_141eb3410(local_res8,"Battle_Palette_Right",1);
          FUN_141a66300(local_58,local_res8);
          if (2 < (int)param_1[0x92]) {
            puVar1 = (undefined1 *)param_1[0x91];
            uVar4 = FUN_141710a40(param_1[0x9e],local_res20);
            *puVar1 = uVar4;
            lVar6 = param_1[0x91];
            uVar4 = FUN_141710a40(param_1[0x9e],local_58);
            *(undefined1 *)(lVar6 + 2) = uVar4;
          }
          FUN_14171adf0(param_1[0x9e],param_1 + 0x8b,0);
          (**(code **)(*(longlong *)param_1[0xa1] + 0x250))((longlong *)param_1[0xa1],1);
          (**(code **)(*(longlong *)param_1[0x9d] + 0x250))((longlong *)param_1[0x9d],1);
          (**(code **)(*(longlong *)param_1[0x9f] + 0x250))((longlong *)param_1[0x9f],1);
          local_58[2] = 0;
          local_58[3] = 0x1e;
          FUN_1407e2ee0(local_58 + 2,0);
          FUN_1407e4a20(local_58[2],0x1e,"MSG_CharacterChange_Operation",0x1e,CONCAT22(uVar11,0x3f))
          ;
          FUN_1416fb690(param_1[0xa0],local_58 + 2,1);
          if (local_58[2] != 0) {
            FUN_141de9200();
          }
        }
        else {
          FUN_14171adf0(param_1[0x9e],param_1 + 0x7b,0);
          (**(code **)(*(longlong *)param_1[0xa1] + 0x250))((longlong *)param_1[0xa1],3);
          (**(code **)(*(longlong *)param_1[0x9d] + 0x250))((longlong *)param_1[0x9d],3);
          (**(code **)(*(longlong *)param_1[0x9f] + 0x250))((longlong *)param_1[0x9f],3);
          local_58[2] = 0;
          if (param_2 == 4) {
            local_58[3] = 0xc;
            FUN_1407e2ee0(local_58 + 2,0);
            uVar9 = 0xc;
            pcVar8 = "MSG_Slander";
          }
          else {
            local_58[3] = 10;
            FUN_1407e2ee0(local_58 + 2,0);
            uVar9 = 10;
            pcVar8 = "MSG_Chase";
          }
          FUN_1407e4a20(local_58[2],uVar9,pcVar8,uVar9,CONCAT22(uVar11,0x3f));
          FUN_1416fb690(param_1[0xa0],local_58 + 2,1);
          if (local_58[2] != 0) {
            FUN_141de9200();
          }
          FUN_141617e40(param_1[0xa1],uVar3);
        }
      }
      FUN_141a83e30(param_1[0x9c],uVar10);
      *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | 8;
      *(undefined4 *)((longlong)param_1 + 0x55c) = param_3;
      *(undefined4 *)(param_1 + 0xac) = 0;
      (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0xa5],0,1,0,uVar3);
      FUN_141485f00(0x68,uVar3,0,0);
    }
    else {
      *(undefined4 *)((longlong)param_1 + 0x55c) = param_3;
      *(undefined4 *)(param_1 + 0xac) = 0;
    }
  }
  return;
}



// =================   CALLER-OF 0x141a66300  FUN_141597810 @ 141597810  size=0x2b9 =================
// strings: [Decision, Gamepad_FaceButton_Right, Gamepad_FaceButton_Bottom, amepad_FaceButton_Bottom, Cancel, amepad_FaceButton_Right]
// callers: FUN_1415a9000@1415a9000 

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_141597810(undefined8 param_1,longlong param_2)

{
  wchar_t _C;
  char cVar1;
  char cVar2;
  wint_t wVar3;
  wint_t wVar4;
  int iVar5;
  int iVar6;
  byte *pbVar7;
  ulonglong *puVar8;
  undefined2 *puVar9;
  uint uVar10;
  uint uVar11;
  undefined2 *puVar12;
  wint_t *pwVar13;
  wchar_t *pwVar14;
  longlong lVar15;
  undefined2 *puVar16;
  bool bVar17;
  undefined8 local_res10;
  undefined1 auStack_868 [32];
  undefined2 *local_848;
  ulonglong local_840;
  ulonglong local_838 [2];
  wchar_t local_828 [1024];
  ulonglong local_28;
  
  local_28 = DAT_145909780 ^ (ulonglong)auStack_868;
  puVar12 = (undefined2 *)0x0;
  local_838[0] = 0;
  if (param_2 == 0) goto LAB_141597aa8;
  local_res10 = param_2;
  FUN_141a66300(local_838,&local_res10);
  cVar1 = DAT_14569c148;
  pbVar7 = (byte *)FUN_141eb81a0(&local_res10);
  puVar16 = puVar12;
  if (pbVar7 != (byte *)0x0) {
    pwVar14 = L"Decision";
    iVar6 = 0;
    local_848 = (undefined2 *)((ulonglong)local_848 & 0xffffffff00000000);
    cVar2 = thunk_FUN_141eb2bc0(L"Decision",local_828,0x400,&local_848);
    iVar5 = iVar6;
    if (cVar2 != '\0') {
      pwVar14 = local_828;
      iVar5 = (int)local_848 + 1;
    }
    if (iVar5 == local_res10._4_4_) {
      pwVar13 = (wint_t *)(pbVar7 + 0x10);
      if ((*pbVar7 & 1) == 0) {
        do {
          _C = *pwVar14;
          if ((_C == L'\0') && ((byte)*pwVar13 == 0)) goto LAB_141597965;
          if ((uint)(ushort)_C != (int)(char)(byte)*pwVar13) {
            wVar3 = towlower(_C);
            iVar5 = tolower((int)(char)(byte)*pwVar13);
            if ((uint)wVar3 != (int)(char)iVar5) goto LAB_141597963;
          }
          pwVar14 = pwVar14 + 1;
          pwVar13 = (wint_t *)((longlong)pwVar13 + 1);
        } while( true );
      }
      lVar15 = (longlong)pwVar14 - (longlong)pwVar13;
      do {
        wVar3 = *(wint_t *)(lVar15 + (longlong)pwVar13);
        if ((wVar3 == 0) && (*pwVar13 == 0)) {
          bVar17 = true;
          goto LAB_141597967;
        }
        if (wVar3 != *pwVar13) {
          wVar3 = towlower(wVar3);
          wVar4 = towlower(*pwVar13);
          if (wVar3 != wVar4) goto LAB_141597917;
        }
        pwVar13 = pwVar13 + 1;
      } while( true );
    }
  }
  goto LAB_1415979ab;
LAB_141597917:
  bVar17 = wVar3 == wVar4;
  goto LAB_141597967;
LAB_141597963:
  iVar6 = (uint)wVar3 - (int)(char)iVar5;
LAB_141597965:
  bVar17 = iVar6 == 0;
LAB_141597967:
  if (bVar17) {
    local_848 = (undefined2 *)0x0;
    local_840 = 0;
    pwVar14 = L"Gamepad_FaceButton_Bottom";
    if (cVar1 == '\0') {
      pwVar14 = L"Gamepad_FaceButton_Right";
    }
    if (*pwVar14 == L'\0') {
LAB_141597a39:
      puVar9 = &DAT_14391f7d0;
    }
    else {
      lVar15 = -1;
      do {
        lVar15 = lVar15 + 1;
      } while (pwVar14[lVar15] != L'\0');
LAB_1415979fd:
      local_848 = (undefined2 *)0x0;
      uVar10 = (int)lVar15 + 1;
      local_840 = (ulonglong)uVar10;
      uVar11 = uVar10;
      if (0 < (int)uVar10) {
        FUN_1407e2ee0(&local_848,0);
        uVar11 = (uint)local_840;
      }
      puVar9 = local_848;
      FUN_141dd1c10(local_848,pwVar14,(longlong)(int)uVar10 * 2);
      puVar16 = puVar9;
      if (uVar11 == 0) goto LAB_141597a39;
    }
    puVar8 = (ulonglong *)FUN_141eb3490(&local_848,puVar9,1);
    local_838[0] = *puVar8;
    if (puVar16 != (undefined2 *)0x0) {
      FUN_141de9200(puVar16);
    }
  }
  else {
LAB_1415979ab:
    cVar2 = FUN_140c51ef0(&local_res10,L"Cancel");
    if (cVar2 != '\0') {
      local_848 = (undefined2 *)0x0;
      local_840 = 0;
      pwVar14 = L"Gamepad_FaceButton_Right";
      if (cVar1 == '\0') {
        pwVar14 = L"Gamepad_FaceButton_Bottom";
      }
      if (*pwVar14 != L'\0') {
        lVar15 = -1;
        do {
          lVar15 = lVar15 + 1;
        } while (pwVar14[lVar15] != L'\0');
        goto LAB_1415979fd;
      }
      goto LAB_141597a39;
    }
  }
  do {
    local_28 = (&DAT_14598b9b0)[(int)puVar12];
    if (local_838[0] == local_28) {
      return (ulonglong)puVar12 & 0xff;
    }
    uVar11 = (int)puVar12 + 1;
    puVar12 = (undefined2 *)(ulonglong)uVar11;
  } while ((int)uVar11 < 0x96);
LAB_141597aa8:
  return CONCAT71((int7)(local_28 >> 8),0x97);
}



