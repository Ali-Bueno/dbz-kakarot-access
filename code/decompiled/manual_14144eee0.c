// FUN_14144eee0 @ 14144eee0

void FUN_14144eee0(longlong param_1,int param_2,byte param_3,undefined8 param_4)

{
  longlong lVar1;
  int iVar2;
  longlong lVar3;
  byte bVar4;
  longlong alStack_18 [2];
  
  lVar1 = *(longlong *)(param_1 + 0x138);
  if (lVar1 == 0) {
    return;
  }
  if (*(int *)(lVar1 + 0x30) != *(int *)(lVar1 + 0x5c)) {
    lVar3 = *(longlong *)(lVar1 + 0x68);
    if (*(longlong *)(lVar1 + 0x68) == 0) {
      lVar3 = lVar1 + 0x60;
    }
    iVar2 = *(int *)(lVar3 + ((longlong)*(int *)(lVar1 + 0x70) - 1U & (longlong)param_2) * 4);
    if (iVar2 != -1) {
      lVar1 = *(longlong *)(lVar1 + 0x28);
      do {
        lVar3 = (longlong)iVar2;
        if (*(int *)(lVar1 + lVar3 * 0x18) == param_2) {
          if (iVar2 == -1) {
            return;
          }
          lVar1 = lVar1 + lVar3 * 0x18;
          if (lVar1 == 0) {
            return;
          }
          lVar1 = *(longlong *)(lVar1 + 8);
          if (lVar1 == 0) {
            return;
          }
          alStack_18[0] = 0;
          alStack_18[1] = 0x2a;
          FUN_1407e2ee0(alStack_18,0);
          lVar3 = alStack_18[0];
          FUN_141dd1c10(alStack_18[0],L"USkillSave::SkillSaveInfo::SparkingSkills",0x54);
          bVar4 = param_3;
          if (3 < param_3) {
            bVar4 = 3;
          }
          if (lVar3 != 0) {
            FUN_141de9200(lVar3);
          }
          if (param_3 != bVar4) {
            return;
          }
          *(undefined8 *)(lVar1 + 0x150 + (ulonglong)param_3 * 8) = param_4;
          return;
        }
        iVar2 = *(int *)(lVar1 + 0x10 + lVar3 * 0x18);
      } while (iVar2 != -1);
    }
  }
  return;
}


