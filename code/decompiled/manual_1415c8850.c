// FUN_1415c8850 @ 1415c8850

void FUN_1415c8850(longlong *param_1,byte param_2)

{
  code *pcVar1;
  longlong lVar2;
  longlong *plVar3;
  undefined8 uVar4;
  byte bVar5;
  ulonglong uVar6;
  uint uVar7;
  ulonglong uVar8;
  undefined4 uVar9;
  
  if (*(char *)((longlong)param_1 + 0x51e) != '\0') {
    uVar6 = 0;
    *(undefined4 *)((longlong)param_1 + 0x50c) = 0;
    pcVar1 = *(code **)(*param_1 + 0x2e8);
    if (param_2 == 0) {
      *(undefined4 *)(param_1 + 0xa1) = 0xd;
      (*pcVar1)(param_1,*(undefined8 *)(param_1[0x9d] + 0x10));
      uVar9 = DAT_1450d6c88;
      (**(code **)(*param_1 + 0x2e0))
                (param_1,*(undefined8 *)(param_1[0x9d] + 0x28),0,1,0,DAT_1450d6c88);
      uVar4 = 2;
    }
    else {
      *(undefined4 *)(param_1 + 0xa1) = 0xb;
      (*pcVar1)(param_1,*(undefined8 *)(param_1[0x9d] + 0x10));
      uVar9 = DAT_1450d6c88;
      (**(code **)(*param_1 + 0x2e0))
                (param_1,*(undefined8 *)(param_1[0x9d] + 0x20),0,1,0,DAT_1450d6c88);
      uVar4 = 1;
    }
    FUN_141485f00(uVar4,uVar9,0,0);
    uVar8 = uVar6;
    if (0 < (int)param_1[0x9b]) {
      do {
        lVar2 = param_1[0x9a];
        bVar5 = param_2;
        if (*(char *)(uVar6 + 0x11 + lVar2) == '\0') {
          if (*(char *)(uVar6 + 0x10 + lVar2) != '\0') {
            bVar5 = param_2 ^ 1;
            goto LAB_1415c8972;
          }
        }
        else {
LAB_1415c8972:
          FUN_1415c7b10(*(undefined8 *)(uVar6 + 8 + lVar2),bVar5);
        }
        uVar7 = (int)uVar8 + 1;
        uVar6 = uVar6 + 0x20;
        uVar8 = (ulonglong)uVar7;
      } while ((int)uVar7 < (int)param_1[0x9b]);
    }
    FUN_1415cf800(param_1,0xffffffff);
    if (*(int *)((longlong)param_1 + 0x534) != 3) {
      plVar3 = (longlong *)param_1[0x93];
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0x250))(plVar3,1);
      }
      plVar3 = (longlong *)param_1[0x94];
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0x250))(plVar3,1);
      }
      *(undefined4 *)((longlong)param_1 + 0x534) = 3;
    }
    *(undefined1 *)((longlong)param_1 + 0x51d) = 0;
  }
  return;
}


