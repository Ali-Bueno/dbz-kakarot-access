// FUN_14144efc0 @ 14144efc0

undefined8 FUN_14144efc0(undefined8 param_1,undefined4 param_2,longlong *param_3)

{
  longlong *plVar1;
  short *psVar2;
  bool bVar3;
  char cVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  undefined8 uVar8;
  longlong lVar9;
  undefined8 *puVar10;
  undefined8 uVar11;
  longlong lVar12;
  longlong *plVar13;
  short *psVar14;
  undefined8 local_res20;
  longlong local_38 [2];
  
  lVar6 = FUN_14145e140();
  lVar7 = FUN_14145e360(param_1,*param_3);
  if ((lVar7 == 0) || (lVar6 == 0)) {
LAB_14144f499:
    uVar8 = 0;
  }
  else {
    uVar8 = FUN_140d2aef0();
    iVar5 = 0;
    local_res20 = 0;
    if ((*(longlong *)(lVar7 + 0x48) != 0) && (lVar9 = FUN_140d2aa10(), lVar9 != 0)) {
      if (*(longlong *)(lVar7 + 0x90) == DAT_145973860) {
        puVar10 = (undefined8 *)FUN_141eb3490(&local_res20,L"DLC_02_01007",1);
        cVar4 = FUN_1413dd4c0(lVar9,*puVar10);
        if (cVar4 != '\0') goto LAB_14144f075;
      }
      cVar4 = FUN_1413dd4c0(lVar9,*(undefined8 *)(lVar7 + 0x48));
      if (cVar4 == '\0') {
        return 1;
      }
    }
LAB_14144f075:
    if (*(longlong *)(lVar7 + 0x90) == DAT_145973868) {
      if ((*(int *)(lVar7 + 0x18) - 0x2aU < 2) &&
         ((cVar4 = FUN_1412e08a0(), cVar4 == '\0' || (cVar4 = FUN_1413a34d0(), cVar4 == '\0')))) {
        return 1;
      }
    }
    else if (((*(longlong *)(lVar7 + 0x90) == DAT_145973880) &&
             (lVar9 = FUN_140d2a6d0(), lVar9 != 0)) && (*(int *)(lVar7 + 0x18) == 1)) {
      cVar4 = FUN_1412e0630();
      if (cVar4 == '\0') {
        return 1;
      }
      uVar11 = FUN_1412ddc30(lVar9);
      cVar4 = FUN_140c51ef0(uVar11,L"MainEp_Finish");
      if (cVar4 == '\0') {
        return 1;
      }
    }
    lVar9 = FUN_1411fc890();
    if (((lVar9 != 0) && (*(longlong *)(lVar9 + 0x1ea0) != 0)) &&
       (lVar9 = *(longlong *)(*(longlong *)(lVar9 + 0x1ea0) + 0x100), lVar9 != 0)) {
      iVar5 = FUN_1419d7140(lVar9 + 0x38,param_2);
    }
    if (iVar5 < *(int *)(lVar7 + 0x58)) {
      return 2;
    }
    cVar4 = FUN_14122f430(param_3);
    if (cVar4 != '\0') {
      lVar9 = *param_3;
      lVar12 = FUN_14145e140(param_1,*(undefined4 *)(lVar7 + 0x18));
      plVar1 = *(longlong **)(lVar12 + 0x100);
      plVar13 = plVar1;
      if (plVar1 == plVar1 + *(int *)(lVar12 + 0x108)) {
        return 7;
      }
      while (*plVar13 != lVar9) {
        plVar13 = plVar13 + 1;
        if (plVar13 == plVar1 + *(int *)(lVar12 + 0x108)) {
          return 7;
        }
      }
      if ((int)((longlong)plVar13 - (longlong)plVar1 >> 3) == -1) {
        return 7;
      }
    }
    bVar3 = false;
    if ((*(int *)(lVar7 + 0x30) == 0) && (*(int *)(lVar7 + 0x34) == 0)) {
LAB_14144f214:
      if ((*(int *)(lVar7 + 0x38) == 0) && (*(int *)(lVar7 + 0x3c) == 0)) {
LAB_14144f29e:
        if ((*(int *)(lVar7 + 0x40) == 0) && (*(int *)(lVar7 + 0x44) == 0)) {
LAB_14144f319:
          if (bVar3) {
            return 4;
          }
          local_res20 = *param_3;
          puVar10 = (undefined8 *)FUN_141eca8b0(&local_res20,local_38);
          psVar2 = (short *)*puVar10;
          for (psVar14 = psVar2; psVar14 != psVar2 + *(int *)(puVar10 + 1); psVar14 = psVar14 + 1) {
            if (*psVar14 == 0x50) {
              iVar5 = (int)((longlong)psVar14 - (longlong)psVar2 >> 1);
              goto LAB_14144f368;
            }
          }
          iVar5 = -1;
LAB_14144f368:
          if (local_38[0] != 0) {
            FUN_141de9200();
          }
          if (iVar5 != 0) {
            iVar5 = FUN_140fff180(lVar7);
            lVar6 = *param_3;
            FUN_141eb3490(&local_res20,L"A_C028A_S_SKL002_2_LV01",1);
            if (((((int)lVar6 != (int)local_res20) ||
                 ((int)((ulonglong)lVar6 >> 0x20) != local_res20._4_4_)) || (iVar5 == 0)) &&
               ((lVar6 = FUN_14145d670(uVar8,*(undefined8 *)(lVar7 + 0x20)), lVar6 != 0 &&
                (*(char *)(lVar6 + 0x20) == '\x04')))) {
              FUN_14144c340(uVar8,param_2,*(undefined8 *)(lVar7 + 0x20));
            }
          }
          iVar5 = FUN_140fff180(lVar7);
          if (iVar5 == 0) {
            local_res20 = *param_3;
            puVar10 = (undefined8 *)FUN_141eca8b0(&local_res20,local_38);
            psVar2 = (short *)*puVar10;
            for (psVar14 = psVar2; psVar14 != psVar2 + *(int *)(puVar10 + 1); psVar14 = psVar14 + 1)
            {
              if (*psVar14 == 0x50) {
                iVar5 = (int)((longlong)psVar14 - (longlong)psVar2 >> 1);
                goto LAB_14144f427;
              }
            }
            iVar5 = -1;
LAB_14144f427:
            if (local_38[0] != 0) {
              FUN_141de9200();
            }
            if (iVar5 == 0) {
              lVar6 = FUN_14145cc40();
              if (lVar6 != 0) {
                if (*(char *)(lVar6 + 0x60) != '\0') {
                  return 6;
                }
                if (*(char *)(lVar6 + 0x62) != '\0') {
                  return 5;
                }
              }
            }
            else {
              lVar6 = FUN_14145d670(uVar8,*(undefined8 *)(lVar7 + 0x20));
              if ((lVar6 != 0) && (*(char *)(lVar6 + 100) != '\0')) {
                return 5;
              }
            }
          }
          goto LAB_14144f499;
        }
        plVar1 = *(longlong **)(lVar6 + 0x100);
        for (plVar13 = plVar1; plVar13 != plVar1 + *(int *)(lVar6 + 0x108); plVar13 = plVar13 + 1) {
          if (*plVar13 == *(longlong *)(lVar7 + 0x40)) {
            if ((int)((longlong)plVar13 - (longlong)plVar1 >> 3) != -1) goto LAB_14144f319;
            break;
          }
        }
        plVar1 = *(longlong **)(lVar6 + 0xf0);
        plVar13 = plVar1;
        if (plVar1 != plVar1 + *(int *)(lVar6 + 0xf8)) {
          while (*plVar13 != *(longlong *)(lVar7 + 0x40)) {
            plVar13 = plVar13 + 1;
            if (plVar13 == plVar1 + *(int *)(lVar6 + 0xf8)) {
              return 3;
            }
          }
          if ((int)((longlong)plVar13 - (longlong)plVar1 >> 3) != -1) {
            return 4;
          }
        }
      }
      else {
        plVar1 = *(longlong **)(lVar6 + 0x100);
        for (plVar13 = plVar1; plVar13 != plVar1 + *(int *)(lVar6 + 0x108); plVar13 = plVar13 + 1) {
          if (*plVar13 == *(longlong *)(lVar7 + 0x38)) {
            if ((int)((longlong)plVar13 - (longlong)plVar1 >> 3) != -1) goto LAB_14144f29e;
            break;
          }
        }
        plVar1 = *(longlong **)(lVar6 + 0xf0);
        plVar13 = plVar1;
        if (plVar1 != plVar1 + *(int *)(lVar6 + 0xf8)) {
          while (*plVar13 != *(longlong *)(lVar7 + 0x38)) {
            plVar13 = plVar13 + 1;
            if (plVar13 == plVar1 + *(int *)(lVar6 + 0xf8)) {
              return 3;
            }
          }
          if ((int)((longlong)plVar13 - (longlong)plVar1 >> 3) != -1) {
            bVar3 = true;
            goto LAB_14144f29e;
          }
        }
      }
    }
    else {
      plVar1 = *(longlong **)(lVar6 + 0x100);
      for (plVar13 = plVar1; plVar13 != plVar1 + *(int *)(lVar6 + 0x108); plVar13 = plVar13 + 1) {
        if (*plVar13 == *(longlong *)(lVar7 + 0x30)) {
          if ((int)((longlong)plVar13 - (longlong)plVar1 >> 3) != -1) goto LAB_14144f214;
          break;
        }
      }
      plVar1 = *(longlong **)(lVar6 + 0xf0);
      for (plVar13 = plVar1; plVar13 != plVar1 + *(int *)(lVar6 + 0xf8); plVar13 = plVar13 + 1) {
        if (*plVar13 == *(longlong *)(lVar7 + 0x30)) {
          if ((int)((longlong)plVar13 - (longlong)plVar1 >> 3) != -1) {
            bVar3 = true;
            goto LAB_14144f214;
          }
          break;
        }
      }
    }
    uVar8 = 3;
  }
  return uVar8;
}


