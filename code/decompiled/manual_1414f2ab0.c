// FUN_1414f2ab0 @ 1414f2ab0

ulonglong FUN_1414f2ab0(longlong param_1,char param_2)

{
  ulonglong uVar1;
  float fVar2;
  float fVar3;
  longlong lVar4;
  longlong *plVar5;
  ulonglong uVar6;
  ulonglong uVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  undefined1 local_res18 [16];
  
  uVar7 = (ulonglong)*(int *)(param_1 + 0x628);
  fVar2 = *(float *)(*(longlong *)(param_1 + 0x568) + 0x90);
  fVar3 = *(float *)(*(longlong *)(param_1 + 0x568) + 0x94);
  if (0 < *(int *)(param_1 + 0x628)) {
    uVar6 = uVar7;
    do {
      lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x548) + -8 + uVar6 * 8);
      uVar1 = uVar6 - 1;
      if (uVar6 == 1) {
        fVar9 = *(float *)(param_1 + 0x430);
        fVar8 = *(float *)(param_1 + 0x434);
        fVar10 = *(float *)(param_1 + 0x424);
      }
      else {
        fVar9 = *(float *)(param_1 + 0x428);
        fVar8 = *(float *)(param_1 + 0x42c);
        fVar10 = *(float *)(param_1 + 0x420);
      }
      fVar9 = *(float *)(lVar4 + 0x3f0) + *(float *)(lVar4 + 0x550) + fVar9;
      fVar8 = *(float *)(lVar4 + 0x3f4) + *(float *)(lVar4 + 0x554) + fVar8;
      if ((((fVar9 <= fVar10 + fVar2) && (fVar2 - fVar10 <= fVar9)) && (fVar8 <= fVar10 + fVar3)) &&
         (fVar3 - fVar10 <= fVar8)) {
        if (param_2 != '\0') {
          plVar5 = (longlong *)
                   FUN_141994820(*(undefined8 *)(param_1 + 0x600),local_res18,uVar7 & 0xffffffff);
          uVar7 = uVar7 & 0xffffffff;
          if (*plVar5 == 0) {
            uVar7 = 0xffffffff;
          }
        }
        return uVar7 & 0xffffffff;
      }
      uVar7 = (ulonglong)((int)uVar7 - 1);
      uVar6 = uVar1;
    } while (0 < (longlong)uVar1);
  }
  return 0xffffffff;
}


