// FUN_141672080 @ 141672080

void FUN_141672080(longlong param_1)

{
  undefined8 *puVar1;
  undefined4 uVar2;
  longlong lVar3;
  char cVar4;
  longlong lVar5;
  undefined8 uVar6;
  int iVar7;
  longlong *plVar8;
  undefined8 uVar9;
  char cVar10;
  longlong lVar11;
  
  lVar11 = (longlong)
           *(int *)(param_1 + 0x4cc +
                   ((longlong)*(int *)(param_1 + 0x15fc) * 0x1e +
                   (longlong)*(int *)(param_1 + 0x15f8)) * 4);
  lVar5 = *(longlong *)(param_1 + 0x400);
  lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x438) + -8 + lVar11 * 8);
  puVar1 = (undefined8 *)(lVar3 + 0x464);
  uVar2 = *(undefined4 *)(lVar3 + 0x468);
  uVar9 = *puVar1;
  *(undefined4 *)(lVar5 + 0x3f8) = *(undefined4 *)puVar1;
  *(undefined4 *)(lVar5 + 0x3fc) = uVar2;
  FUN_1429e1d30(lVar5,uVar9);
  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 8;
  uVar9 = 0;
  *(undefined4 *)(param_1 + 0x1628) = 0;
  *(undefined1 *)(param_1 + 0x162c) = 1;
  lVar5 = FUN_140d2aef0();
  if (lVar5 != 0) {
    uVar9 = *(undefined8 *)(lVar5 + 0x130);
  }
  uVar6 = FUN_140d2aef0();
  lVar5 = FUN_14145e360(uVar9,*(undefined8 *)(*(longlong *)(param_1 + 0x1538) + -8 + lVar11 * 8));
  uVar2 = DAT_1450d6c88;
  cVar10 = '\0';
  if (lVar5 == 0) {
    iVar7 = 1;
  }
  else {
    iVar7 = *(int *)(lVar5 + 0x28);
  }
  if (iVar7 == 1) {
    FUN_1429e1be0(*(undefined8 *)(param_1 + 0x400),CONCAT44(DAT_1450d6c88,DAT_1450d6c88));
    cVar4 = FUN_141462a10(uVar9,*(undefined8 *)(lVar5 + 0x20));
    if (cVar4 == '\0') {
      lVar5 = FUN_14145d670(uVar6,*(undefined8 *)(lVar5 + 0x20));
      if (lVar5 != 0) {
        cVar10 = *(char *)(lVar5 + 100);
      }
    }
    else {
      lVar5 = FUN_14145cc40();
      if (lVar5 != 0) {
        cVar10 = *(char *)(lVar5 + 0x62);
      }
    }
    plVar8 = *(longlong **)(*(longlong *)(param_1 + 0x400) + 0x3f0);
    lVar5 = *plVar8;
    if (cVar10 != '\0') {
      uVar9 = 3;
      goto LAB_1416721ea;
    }
  }
  else {
    FUN_1429e1be0(*(undefined8 *)(param_1 + 0x400),*(undefined8 *)(param_1 + 0x420));
    plVar8 = *(longlong **)(*(longlong *)(param_1 + 0x400) + 0x3f0);
    lVar5 = *plVar8;
  }
  uVar9 = 1;
LAB_1416721ea:
  (**(code **)(lVar5 + 0x250))(plVar8,uVar9);
  if (*(longlong *)(param_1 + 0x1548) != 0) {
    FUN_141674e40(*(longlong *)(param_1 + 0x1548),*(longlong *)(param_1 + 0x1538) + -8 + lVar11 * 8)
    ;
  }
  FUN_141485f00(0,uVar2,0,0);
  return;
}


