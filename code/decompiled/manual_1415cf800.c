// FUN_1415cf800 @ 1415cf800

void FUN_1415cf800(longlong *param_1,int param_2)

{
  int iVar1;
  longlong *plVar2;
  void *_Src;
  undefined4 uVar3;
  undefined8 uVar4;
  undefined1 *puVar5;
  longlong lVar6;
  longlong *plVar7;
  longlong lVar8;
  longlong lVar9;
  void *local_68;
  int local_60;
  undefined4 local_5c;
  undefined1 local_58 [64];
  
  uVar3 = DAT_1450d6c88;
  iVar1 = *(int *)((longlong)param_1 + 0x514);
  if (iVar1 != param_2) {
    lVar8 = 0;
    if (iVar1 != -1) {
      plVar2 = *(longlong **)((longlong)iVar1 * 0x20 + 8 + param_1[0x9a]);
      (**(code **)(*plVar2 + 0x2e8))(plVar2,*(undefined8 *)(plVar2[0x78] + 0x18));
      (**(code **)(*plVar2 + 0x2e0))(plVar2,*(undefined8 *)(plVar2[0x78] + 0x20),0,1,0,uVar3);
      *(undefined4 *)(plVar2 + 0x7e) = 4;
      FUN_1415cd470(param_1[0x8b],0xf);
    }
    if (param_2 == -1) {
      if (*(int *)((longlong)param_1 + 0x514) != -1) {
        do {
          FUN_14159f9e0(*(undefined8 *)(lVar8 + param_1[0x97]));
          lVar8 = lVar8 + 8;
        } while (lVar8 < 0x18);
        if (2 < (int)param_1[0xa1] - 0xbU) {
          FUN_1415cb3f0(param_1,1);
          FUN_1415c6dd0(param_1,3);
        }
      }
    }
    else {
      lVar9 = (longlong)param_2 * 0x20;
      plVar2 = *(longlong **)(param_1[0x9a] + 8 + lVar9);
      (**(code **)(*plVar2 + 0x2e0))(plVar2,*(undefined8 *)(plVar2[0x78] + 0x18),0,1,0,uVar3);
      *(undefined4 *)(plVar2 + 0x7e) = 3;
      lVar8 = *(longlong *)(param_1[0x9a] + 8 + lVar9);
      local_68 = (void *)0x0;
      local_60 = *(int *)(lVar8 + 0x400);
      lVar6 = (longlong)local_60;
      _Src = *(void **)(lVar8 + 0x3f8);
      if (local_60 == 0) {
        local_5c = 0;
      }
      else {
        FUN_1407f1420(&local_68,local_60,0);
        memcpy(local_68,_Src,lVar6 * 2);
      }
      lVar8 = param_1[0x9f];
      if (*(longlong *)(lVar8 + 8) != 0) {
        uVar4 = FUN_141e241b0(local_58,&local_68);
        FUN_141a87ad0(*(undefined8 *)(lVar8 + 8),uVar4);
        FUN_141a54c00(*(undefined8 *)(param_1[0x9f] + 8));
      }
      if (local_68 != (void *)0x0) {
        FUN_141de9200(local_68);
      }
      if (*(int *)((longlong)param_1 + 0x534) != 2) {
        plVar2 = (longlong *)param_1[0x93];
        if (plVar2 != (longlong *)0x0) {
          (**(code **)(*plVar2 + 0x250))(plVar2,1);
        }
        plVar2 = (longlong *)param_1[0x94];
        plVar7 = (longlong *)0x0;
        if (plVar2 != (longlong *)0x0) {
          (**(code **)(*plVar2 + 0x250))(plVar2,3);
          plVar7 = (longlong *)param_1[0x94];
        }
        plVar2 = (longlong *)plVar7[0x73];
        *(undefined1 *)((longlong)plVar7 + 0x3b4) = 1;
        if (plVar2 != (longlong *)0x0) {
          (**(code **)(*plVar2 + 0x250))(plVar2,3);
        }
        if ((int)plVar7[0x76] != 1) {
          (**(code **)(*plVar7 + 0x2e0))(plVar7,*(undefined8 *)(plVar7[0x74] + 8),0,1,0,uVar3);
          *(undefined4 *)(plVar7 + 0x76) = 1;
        }
        *(undefined4 *)((longlong)param_1 + 0x534) = 2;
      }
      (**(code **)(*param_1 + 0x2e8))(param_1,*(undefined8 *)(param_1[0x9d] + 0x18));
      (**(code **)(*param_1 + 0x2e0))(param_1,*(undefined8 *)(param_1[0x9d] + 0x10),0,1,0,uVar3);
      puVar5 = (undefined1 *)FUN_1415bd150(param_1,*(undefined4 *)(lVar9 + 0x1c + param_1[0x9a]));
      FUN_1415cd470(param_1[0x8b],*puVar5);
      FUN_141485f00(0,uVar3,0,0);
      FUN_1415cc4c0(param_1,param_2);
      FUN_1415ca300(param_1,param_2);
      FUN_1415ca5a0(param_1,param_2);
    }
    *(int *)((longlong)param_1 + 0x514) = param_2;
  }
  return;
}


