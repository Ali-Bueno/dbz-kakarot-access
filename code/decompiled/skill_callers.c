// ======== caller 14165efa0 of 0x14166c2a0 ========

void FUN_14165efa0(longlong param_1)

{
  longlong *plVar1;
  char cVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined8 uVar5;
  uint uVar6;
  ulonglong uVar7;
  longlong *plVar8;
  longlong lVar9;
  ulonglong uVar10;
  int iVar11;
  ulonglong uVar12;
  int iVar13;
  longlong *plVar14;
  int *piVar15;
  float fVar16;
  float fVar17;
  longlong local_res10 [2];
  
  uVar7 = 0;
  *(undefined8 *)(param_1 + 0x15e8) = 0x1e;
  *(undefined8 *)(param_1 + 0x15f0) = 0x23;
  piVar15 = (int *)(param_1 + 0x4cc);
  uVar12 = uVar7;
  uVar10 = uVar7;
  do {
    iVar13 = (int)uVar12;
    iVar11 = (int)uVar10;
    if (0 < *piVar15) {
      FUN_141666880(param_1,*piVar15,uVar10,uVar12);
      if (iVar11 < *(int *)(param_1 + 0x15f0)) {
        *(int *)(param_1 + 0x15f0) = iVar11;
      }
      if (iVar13 < *(int *)(param_1 + 0x15e8)) {
        *(int *)(param_1 + 0x15e8) = iVar13;
      }
      if (*(int *)(param_1 + 0x15f4) < iVar11) {
        *(int *)(param_1 + 0x15f4) = iVar11;
      }
      if (*(int *)(param_1 + 0x15ec) < iVar13) {
        *(int *)(param_1 + 0x15ec) = iVar13;
      }
    }
    piVar15 = piVar15 + 1;
    uVar12 = (ulonglong)(iVar13 + 1U);
  } while (((int)(iVar13 + 1U) < 0x1e) ||
          (uVar10 = (ulonglong)(iVar11 + 1U), uVar12 = uVar7, (int)(iVar11 + 1U) < 0x23));
  if (*(longlong *)(param_1 + 0x1658) != 0) {
    if (0 < *(int *)(param_1 + 0x1540)) {
      plVar8 = *(longlong **)(param_1 + 0x1538);
      do {
        iVar11 = (int)uVar12;
        if (*plVar8 == *(longlong *)(param_1 + 0x1658)) goto LAB_14165f087;
        uVar12 = (ulonglong)(iVar11 + 1U);
        plVar8 = plVar8 + 1;
      } while ((int)(iVar11 + 1U) < *(int *)(param_1 + 0x1540));
    }
    iVar11 = 0;
LAB_14165f087:
    plVar8 = *(longlong **)(*(longlong *)(param_1 + 0x438) + (longlong)iVar11 * 8);
    if (plVar8 != (longlong *)0x0) {
      (**(code **)(*plVar8 + 0x250))(plVar8,1);
      plVar14 = plVar8 + 0x88;
      uVar12 = uVar7;
      if (plVar14 <= plVar8 + 0x8c) {
        do {
          if (*plVar14 != 0) {
            plVar1 = *(longlong **)(*plVar14 + 0x30);
            (**(code **)(*plVar1 + 0x250))(plVar1,1);
          }
          plVar14 = plVar14 + 1;
          uVar12 = uVar12 + 1;
        } while (uVar12 != 4);
      }
      FUN_14165a130(param_1,plVar8[0x81]);
    }
  }
  fVar16 = (float)*(int *)(param_1 + 0x15e8) * *(float *)(param_1 + 0x458);
  fVar17 = (float)*(int *)(param_1 + 0x15f0) * *(float *)(param_1 + 0x458);
  uVar3 = FUN_1411fc870();
  plVar8 = *(longlong **)(param_1 + 0x438);
  uVar12 = (longlong)*(int *)(param_1 + 0x440) & 0x1fffffffffffffff;
  if (plVar8 + *(int *)(param_1 + 0x440) < plVar8) {
    uVar12 = uVar7;
  }
  uVar10 = uVar7;
  if (uVar12 != 0) {
    do {
      plVar14 = (longlong *)*plVar8;
      if (plVar14 != (longlong *)0x0) {
        *(float *)((longlong)plVar14 + 0x464) = *(float *)((longlong)plVar14 + 0x464) - fVar16;
        *(float *)(plVar14 + 0x8d) = *(float *)(plVar14 + 0x8d) - fVar17;
        FUN_1429e1d30(plVar14,*(undefined8 *)((longlong)plVar14 + 0x464));
        local_res10[0] = plVar14[0x8e];
        if ((local_res10[0] != 0) && (cVar2 = FUN_14107d720(uVar3,local_res10), cVar2 == '\0')) {
          lVar4 = FUN_140d2a6d0();
          if ((lVar4 != 0) &&
             (((local_res10[0] == DAT_145986940 && (*(int *)(param_1 + 0x1584) == 1)) &&
              (cVar2 = FUN_1412e0630(), cVar2 != '\0')))) {
            uVar5 = FUN_1412ddc30(lVar4);
            cVar2 = FUN_140c51ef0(uVar5,L"MainEp_Finish");
            if (cVar2 != '\0') goto LAB_14165f251;
          }
          FUN_14166c2a0(plVar14,1);
          (**(code **)(*plVar14 + 0x250))(plVar14,1);
          FUN_14165ec80(plVar14);
          FUN_14165a130(param_1,plVar14[0x81]);
        }
LAB_14165f251:
        cVar2 = FUN_141367d80();
        if (((cVar2 == '\0') && (local_res10[1] = 0, plVar14[0x8f] != 0)) &&
           ((lVar4 = FUN_140d2aa10(), lVar4 != 0 &&
            (cVar2 = FUN_1413dd4c0(lVar4,plVar14[0x8f]), cVar2 == '\0')))) {
          FUN_14166c2a0(plVar14,1);
          (**(code **)(*plVar14 + 0x250))(plVar14,1);
          FUN_14165ec80(plVar14);
          FUN_14165a130(param_1,plVar14[0x81]);
        }
        if (plVar14[0x8e] == DAT_145986928) {
          if (*(int *)(param_1 + 0x1584) - 0x2aU < 2) {
            cVar2 = FUN_1412e08a0();
            if (cVar2 != '\0') {
              cVar2 = FUN_1413a34d0();
LAB_14165f32b:
              if (cVar2 != '\0') goto LAB_14165f35e;
            }
LAB_14165f32f:
            FUN_14166c2a0(plVar14,1);
            (**(code **)(*plVar14 + 0x250))(plVar14,1);
            FUN_14165ec80(plVar14);
            FUN_14165a130(param_1,plVar14[0x81]);
          }
        }
        else if (((plVar14[0x8e] == DAT_145986940) && (lVar4 = FUN_140d2a6d0(), lVar4 != 0)) &&
                (*(int *)(param_1 + 0x1584) == 1)) {
          cVar2 = FUN_1412e0630();
          if (cVar2 != '\0') {
            uVar5 = FUN_1412ddc30(lVar4);
            cVar2 = FUN_140c51ef0(uVar5,L"MainEp_Finish");
            goto LAB_14165f32b;
          }
          goto LAB_14165f32f;
        }
      }
LAB_14165f35e:
      plVar8 = plVar8 + 1;
      uVar10 = uVar10 + 1;
    } while (uVar10 != uVar12);
  }
  plVar8 = *(longlong **)(param_1 + 0x448);
  uVar12 = (longlong)*(int *)(param_1 + 0x450) & 0x1fffffffffffffff;
  if (plVar8 + *(int *)(param_1 + 0x450) < plVar8) {
    uVar12 = uVar7;
  }
  uVar10 = uVar7;
  if (uVar12 != 0) {
    do {
      if (*plVar8 != 0) {
        lVar4 = *(longlong *)(*plVar8 + 0x30);
        FUN_1429e1d30(lVar4,CONCAT44(*(float *)(lVar4 + 0x94) - fVar17,
                                     *(float *)(lVar4 + 0x90) - fVar16));
      }
      plVar8 = plVar8 + 1;
      uVar10 = uVar10 + 1;
    } while (uVar10 != uVar12);
  }
  *(undefined1 *)(param_1 + 0x1681) = 1;
  fVar16 = *(float *)(param_1 + 0x15a4) /
           ((float)(*(int *)(param_1 + 0x15f4) - *(int *)(param_1 + 0x15f0)) *
           *(float *)(param_1 + 0x458));
  fVar17 = *(float *)(param_1 + 0x15a0) /
           ((float)(*(int *)(param_1 + 0x15ec) - *(int *)(param_1 + 0x15e8)) *
           *(float *)(param_1 + 0x458));
  if (fVar17 <= fVar16) {
    fVar16 = fVar17;
  }
  *(float *)(param_1 + 0x1634) = fVar16;
  cVar2 = FUN_140d2f940();
  if ((cVar2 != '\0') && (DAT_1450d6c88 < *(float *)(param_1 + 0x1634))) {
    *(undefined1 *)(param_1 + 0x1681) = 0;
  }
  fVar16 = *(float *)(*(longlong *)(param_1 + 0x410) + 0x9c);
  fVar17 = *(float *)(*(longlong *)(param_1 + 0x410) + 0x98);
  if (fVar17 <= fVar16) {
    fVar16 = fVar17;
  }
  *(float *)(param_1 + 0x1638) = fVar16;
  lVar9 = *(longlong *)(param_1 + 0x438);
  lVar4 = lVar9 + (longlong)*(int *)(param_1 + 0x440) * 8;
  do {
    if (lVar9 == lVar4) {
LAB_14165f4ad:
      lVar9 = *(longlong *)(param_1 + 0x448);
      lVar4 = lVar9 + (longlong)*(int *)(param_1 + 0x450) * 8;
      while ((lVar9 != lVar4 && (uVar6 = (int)uVar7 + 1, uVar7 = (ulonglong)uVar6, (int)uVar6 < 6)))
      {
        lVar9 = lVar9 + 8;
      }
      return;
    }
    uVar6 = (int)uVar7 + 1;
    uVar7 = (ulonglong)uVar6;
    if (5 < (int)uVar6) {
      uVar7 = 0;
      goto LAB_14165f4ad;
    }
    lVar9 = lVar9 + 8;
  } while( true );
}



// ======== caller 141668580 of 0x14166c2a0 ========

void FUN_141668580(longlong *param_1,undefined8 param_2)

{
  longlong lVar1;
  char cVar2;
  int iVar3;
  undefined8 uVar4;
  
  FUN_1407fc730(param_1 + 0x84);
  cVar2 = (char)param_1[0x8c];
  if (cVar2 == '\x03') {
    uVar4 = 2;
  }
  else if (cVar2 == '\x06') {
    uVar4 = 5;
  }
  else {
    if (2 < (byte)(cVar2 - 8U)) goto LAB_1416685c3;
    uVar4 = 8;
  }
  FUN_14166c2a0(param_1,uVar4);
LAB_1416685c3:
  lVar1 = *(longlong *)param_1[0x7b];
  if ((byte)((char)param_1[0x8c] - 8U) < 3) {
    (**(code **)(lVar1 + 0x250))();
    (**(code **)(*(longlong *)param_1[0x7c] + 0x250))((longlong *)param_1[0x7c],3);
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x74],0,1,0,DAT_1450d6c88);
    if (param_1[0x88] != 0) {
      lVar1 = param_1[0x82];
      iVar3 = FUN_14165b470(lVar1,param_1,0);
      if (*(longlong *)(*(longlong *)(lVar1 + 0x438) + -8 + (longlong)iVar3 * 8) != 0) {
        cVar2 = FUN_14166d180();
        if (cVar2 == '\0') {
          (**(code **)(**(longlong **)(param_1[0x88] + 0x30) + 0x250))
                    (*(longlong **)(param_1[0x88] + 0x30),3);
        }
      }
    }
    if (param_1[0x89] != 0) {
      lVar1 = param_1[0x82];
      iVar3 = FUN_14165b470(lVar1,param_1,1);
      if (*(longlong *)(*(longlong *)(lVar1 + 0x438) + -8 + (longlong)iVar3 * 8) != 0) {
        cVar2 = FUN_14166d180();
        if (cVar2 == '\0') {
          (**(code **)(**(longlong **)(param_1[0x89] + 0x30) + 0x250))
                    (*(longlong **)(param_1[0x89] + 0x30),3);
        }
      }
    }
    if (param_1[0x8a] != 0) {
      lVar1 = param_1[0x82];
      iVar3 = FUN_14165b470(lVar1,param_1,2);
      if (*(longlong *)(*(longlong *)(lVar1 + 0x438) + -8 + (longlong)iVar3 * 8) != 0) {
        cVar2 = FUN_14166d180();
        if (cVar2 == '\0') {
          (**(code **)(**(longlong **)(param_1[0x8a] + 0x30) + 0x250))
                    (*(longlong **)(param_1[0x8a] + 0x30),3);
        }
      }
    }
    if (param_1[0x8b] != 0) {
      lVar1 = param_1[0x82];
      iVar3 = FUN_14165b470(lVar1,param_1,3);
      if (*(longlong *)(*(longlong *)(lVar1 + 0x438) + -8 + (longlong)iVar3 * 8) != 0) {
        cVar2 = FUN_14166d180();
        if (cVar2 == '\0') {
          (**(code **)(**(longlong **)(param_1[0x8b] + 0x30) + 0x250))
                    (*(longlong **)(param_1[0x8b] + 0x30),3);
        }
      }
    }
  }
  else {
    (**(code **)(lVar1 + 0x250))((longlong *)param_1[0x7b],3);
    (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x73],0,1,0,DAT_1450d6c88);
  }
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | 8;
  FUN_1407e59d0(param_2);
  return;
}



// ======== caller 141675670 of 0x14166c2a0 ========

void FUN_141675670(longlong param_1,int param_2,undefined8 param_3)

{
  longlong lVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  int iVar5;
  longlong lVar6;
  undefined8 uVar7;
  longlong lVar8;
  undefined8 uVar9;
  undefined8 *puVar10;
  longlong *plVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  char cVar14;
  int iVar15;
  undefined2 *puVar16;
  ulonglong uVar17;
  undefined4 uVar18;
  undefined8 local_res8;
  undefined8 local_res18;
  longlong local_res20;
  undefined1 local_58 [16];
  longlong local_48 [2];
  
  lVar1 = (longlong)param_2 * 8;
  *(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x418) = param_3;
  local_res18 = param_3;
  lVar6 = FUN_140d2aef0();
  if (lVar6 == 0) {
    uVar9 = 0;
  }
  else {
    uVar9 = *(undefined8 *)(lVar6 + 0x130);
  }
  cVar3 = '\0';
  uVar18 = 0;
  uVar7 = FUN_140d2aef0();
  lVar6 = FUN_14145e360(uVar9,local_res18);
  cVar2 = FUN_141462a10(uVar9,local_res18);
  if (cVar2 == '\0') {
    lVar8 = FUN_14145d670(uVar7,*(undefined8 *)(lVar6 + 0x20));
    if (lVar8 != 0) {
      if (*(int *)(lVar6 + 0x28) == 1) {
        cVar3 = *(char *)(lVar8 + 100);
      }
      goto LAB_141675722;
    }
  }
  else {
    lVar8 = FUN_14145cc40();
    uVar18 = 0;
    if (lVar8 != 0) {
      if (*(int *)(lVar6 + 0x28) == 1) {
        cVar3 = *(char *)(lVar8 + 0x62);
      }
LAB_141675722:
      uVar18 = *(undefined4 *)(lVar6 + 0x88);
    }
  }
  plVar11 = *(longlong **)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x3c8);
  if (plVar11 != (longlong *)0x0) {
    if (cVar3 == '\0') {
      (**(code **)(*plVar11 + 0x260))();
    }
    else {
      (**(code **)(*plVar11 + 0x250))(plVar11,3);
    }
  }
  FUN_141a83cf0(*(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x3d0),
                uVar18);
  cVar3 = FUN_14145c4b0(uVar9,*(undefined4 *)(param_1 + 0x1584),&local_res18);
  if (cVar3 == '\0') {
    cVar3 = FUN_141462ac0(uVar9);
    FUN_14166c2a0(*(undefined8 *)(lVar1 + *(longlong *)(param_1 + 0x438)),cVar3 != '\0');
    lVar6 = FUN_1411fc870();
    if (lVar6 != 0) {
      local_res8 = 0;
      local_res20 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x470);
      if ((local_res20 != 0) && (cVar3 = FUN_14107d720(lVar6,&local_res20), cVar3 == '\0')) {
        FUN_14166c2a0(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),1);
      }
    }
    cVar3 = FUN_141367d80();
    if ((((cVar3 == '\0') &&
         (local_res8 = 0,
         *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x478) != 0)) &&
        (lVar6 = FUN_140d2aa10(), lVar6 != 0)) &&
       (cVar3 = FUN_1413dd4c0(lVar6,*(undefined8 *)
                                     (*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x478)
                             ), cVar3 == '\0')) {
      FUN_14166c2a0(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),1);
    }
    lVar6 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x470);
    if (lVar6 == DAT_145986928) {
      if (1 < *(int *)(param_1 + 0x1584) - 0x2aU) {
        return;
      }
      cVar3 = FUN_1412e08a0();
      if (cVar3 == '\0') goto LAB_1416758f1;
      cVar3 = FUN_1413a34d0();
    }
    else {
      if (lVar6 != DAT_145986940) {
        return;
      }
      lVar6 = FUN_140d2a6d0();
      if (lVar6 == 0) {
        return;
      }
      if (*(int *)(param_1 + 0x1584) != 1) {
        return;
      }
      cVar3 = FUN_1412e0630();
      if (cVar3 == '\0') goto LAB_1416758f1;
      uVar9 = FUN_1412ddc30(lVar6);
      cVar3 = FUN_140c51ef0(uVar9,L"MainEp_Finish");
    }
    if (cVar3 != '\0') {
      return;
    }
LAB_1416758f1:
    FUN_14166c2a0(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),1);
    return;
  }
  cVar2 = FUN_14165eda0(param_1,local_res18);
  cVar4 = FUN_141461e60(uVar9,local_res18);
  if (cVar4 == '\0') {
    cVar14 = '\0';
    cVar4 = FUN_141462ac0(uVar9,local_res18);
    if (cVar4 == '\0') {
      if (cVar3 == '\x01') {
        cVar14 = (cVar2 == '\0') + '\x06';
      }
      else if (cVar3 == '\x02') {
        cVar14 = '\x05';
        FUN_14166bc40(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),1);
      }
      uVar7 = 4;
      goto LAB_141675b1a;
    }
    local_res8 = local_res18;
    lVar6 = FUN_140d2aef0();
    if ((lVar6 == 0) || (lVar6 = *(longlong *)(lVar6 + 0x130), lVar6 == 0)) {
LAB_141675a53:
      cVar14 = '\0';
      uVar7 = 5;
    }
    else {
      iVar5 = FUN_14145dff0(lVar6,local_res8);
      if (iVar5 != 1) {
        iVar5 = FUN_14145dff0(lVar6,local_res8);
        if ((iVar5 < 2) ||
           ((cVar3 = FUN_141462040(lVar6,local_res8), cVar3 != '\0' &&
            (cVar3 = FUN_141461fc0(lVar6,*(undefined4 *)(param_1 + 0x1584),local_res8),
            cVar3 != '\0')))) goto LAB_141675a94;
        goto LAB_141675a53;
      }
      cVar3 = FUN_141461fc0(lVar6,*(undefined4 *)(param_1 + 0x1584),local_res8);
      if (cVar3 == '\0') goto LAB_141675a53;
LAB_141675a94:
      cVar3 = FUN_14145c4b0(lVar6,*(undefined4 *)(param_1 + 0x1584),&local_res8);
      if (cVar3 == '\x01') {
        cVar3 = FUN_14165eda0(param_1,local_res8);
        uVar7 = 5;
        cVar14 = (cVar3 == '\0') + '\t';
      }
      else {
        if (cVar3 != '\x02') goto LAB_141675a53;
        cVar14 = '\b';
        FUN_14166bc40(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),1);
        uVar7 = 5;
      }
    }
LAB_141675b1a:
    FUN_141a83cf0(*(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x3c0),
                  uVar7);
    cVar3 = FUN_14166d180(*(undefined8 *)(lVar1 + *(longlong *)(param_1 + 0x438)));
    if (cVar3 != '\0') {
      cVar14 = '\x01';
    }
    FUN_14166c2a0(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),cVar14);
    goto LAB_141675b5f;
  }
  if (cVar3 == '\x01') {
    cVar3 = (cVar2 == '\0') + '\x03';
    uVar7 = *(undefined8 *)(lVar1 + *(longlong *)(param_1 + 0x438));
LAB_14167596b:
    FUN_14166c2a0(uVar7,cVar3);
  }
  else if (cVar3 == '\x02') {
    FUN_14166bc40(*(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1),1);
    cVar3 = '\x02';
    uVar7 = *(undefined8 *)(*(longlong *)(param_1 + 0x438) + lVar1);
    goto LAB_14167596b;
  }
  lVar6 = FUN_141459960(uVar9,local_res18);
  if (lVar6 == 0) {
    puVar10 = (undefined8 *)FUN_141eca8b0(&local_res18,local_48);
    if (*(int *)(puVar10 + 1) == 0) {
      puVar16 = &DAT_14391f7d0;
    }
    else {
      puVar16 = (undefined2 *)*puVar10;
    }
    FUN_141dbbf60(local_58,L"FSkillDataTable : Not Found Skill - %s",puVar16);
    FUN_1416e6620(2,local_58);
    if (local_48[0] != 0) {
      FUN_141de9200();
    }
  }
  else {
    FUN_141a83cf0(*(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x3c0),
                  *(byte *)(lVar6 + 0x20) - 1);
  }
LAB_141675b5f:
  lVar6 = FUN_14145e360(uVar9,local_res18);
  uVar13 = 0;
  iVar5 = 0;
  if (*(longlong *)(lVar6 + 0x30) != 0) {
    iVar15 = iVar5;
    if (0 < *(int *)(param_1 + 0x1540)) {
      plVar11 = *(longlong **)(param_1 + 0x1538);
      uVar12 = uVar13;
      uVar17 = uVar13;
      do {
        iVar15 = (int)uVar17;
        if (*plVar11 == *(longlong *)(lVar6 + 0x30)) break;
        uVar17 = (ulonglong)((int)uVar17 + 1);
        uVar12 = uVar12 + 1;
        plVar11 = plVar11 + 1;
        iVar15 = iVar5;
      } while ((longlong)uVar12 < (longlong)*(int *)(param_1 + 0x1540));
    }
    *(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + (longlong)iVar15 * 8) + 0x408) =
         *(undefined8 *)(lVar1 + *(longlong *)(param_1 + 0x438));
  }
  if (*(longlong *)(lVar6 + 0x38) != 0) {
    iVar15 = iVar5;
    if (0 < *(int *)(param_1 + 0x1540)) {
      plVar11 = *(longlong **)(param_1 + 0x1538);
      uVar12 = uVar13;
      uVar17 = uVar13;
      do {
        iVar15 = (int)uVar17;
        if (*plVar11 == *(longlong *)(lVar6 + 0x38)) break;
        uVar17 = (ulonglong)((int)uVar17 + 1);
        uVar12 = uVar12 + 1;
        plVar11 = plVar11 + 1;
        iVar15 = iVar5;
      } while ((longlong)uVar12 < (longlong)*(int *)(param_1 + 0x1540));
    }
    *(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + (longlong)iVar15 * 8) + 0x408) =
         *(undefined8 *)(lVar1 + *(longlong *)(param_1 + 0x438));
  }
  if (*(longlong *)(lVar6 + 0x40) != 0) {
    iVar15 = iVar5;
    if (0 < *(int *)(param_1 + 0x1540)) {
      plVar11 = *(longlong **)(param_1 + 0x1538);
      uVar12 = uVar13;
      do {
        iVar15 = (int)uVar12;
        if (*plVar11 == *(longlong *)(lVar6 + 0x40)) break;
        uVar12 = (ulonglong)((int)uVar12 + 1);
        uVar13 = uVar13 + 1;
        plVar11 = plVar11 + 1;
        iVar15 = iVar5;
      } while ((longlong)uVar13 < (longlong)*(int *)(param_1 + 0x1540));
    }
    *(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + (longlong)iVar15 * 8) + 0x408) =
         *(undefined8 *)(lVar1 + *(longlong *)(param_1 + 0x438));
  }
  *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x438) + lVar1) + 0x410) = param_1;
  return;
}



// ======== caller 141667db0 of 0x14166c2a0 ========

void FUN_141667db0(longlong *param_1,char param_2,undefined8 param_3)

{
  undefined4 uVar1;
  char cVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  longlong lVar5;
  longlong *plVar6;
  undefined1 local_res8 [8];
  
  FUN_1407fc730(param_1 + 0x86,param_3);
  (**(code **)(*param_1 + 0x250))(param_1,3);
  lVar5 = 0;
  plVar6 = param_1 + 0x88;
  if (plVar6 <= param_1 + 0x8c) {
    do {
      if (*plVar6 != 0) {
        uVar3 = FUN_1429a9b10(*(undefined8 *)(*plVar6 + 0x30));
        puVar4 = (undefined8 *)FUN_141eb3490(local_res8,L"Gage_Percent",1);
        FUN_142e31a70(uVar3,*puVar4,0);
      }
      plVar6 = plVar6 + 1;
      lVar5 = lVar5 + 1;
    } while (lVar5 != 4);
  }
  uVar1 = DAT_1450d6c88;
  (**(code **)(*param_1 + 0x2e0))(param_1,param_1[0x75],0,1,0,DAT_1450d6c88);
  (**(code **)(*(longlong *)param_1[0x7d] + 0x250))((longlong *)param_1[0x7d],1);
  (**(code **)(*(longlong *)param_1[0x7c] + 0x250))((longlong *)param_1[0x7c],1);
  (**(code **)(*(longlong *)param_1[0x7f] + 0x250))((longlong *)param_1[0x7f],1);
  (**(code **)(*(longlong *)param_1[0x80] + 0x250))((longlong *)param_1[0x80],1);
  *(byte *)((longlong)param_1 + 0x1ac) = *(byte *)((longlong)param_1 + 0x1ac) | 8;
  if (param_2 != '\0') {
    FUN_141657c40(param_1);
    if (((param_1[0x81] != 0) && (*(char *)(param_1[0x81] + 0x460) == '\x01')) &&
       (cVar2 = FUN_14166d180(), cVar2 == '\0')) {
      FUN_14166c2a0(param_1[0x81],0);
    }
  }
  FUN_141485f00(0x67,uVar1,0,0);
  FUN_1407e59d0(param_3);
  return;
}



// ======== caller 1416641f0 of 0x14166c2a0 ========

void FUN_1416641f0(longlong param_1,longlong param_2)

{
  char cVar1;
  undefined8 uVar2;
  longlong *plVar3;
  
  if ((param_2 == *(longlong *)(param_1 + 0x398)) || (param_2 == *(longlong *)(param_1 + 0x3a0))) {
    if ((*(int *)(param_1 + 0x428) != 0) && (*(longlong **)(param_1 + 0x420) != (longlong *)0x0)) {
      cVar1 = (**(code **)(**(longlong **)(param_1 + 0x420) + 0x20))();
      if (cVar1 != '\0') {
        if (*(int *)(param_1 + 0x428) == 0) {
          plVar3 = (longlong *)0x0;
        }
        else {
          plVar3 = *(longlong **)(param_1 + 0x420);
        }
        (**(code **)(*plVar3 + 0x48))();
      }
    }
    if ((*(int *)(param_1 + 0x428) != 0) &&
       (plVar3 = *(longlong **)(param_1 + 0x420), plVar3 != (longlong *)0x0)) {
      (**(code **)(*plVar3 + 0x30))(plVar3,0);
      if (*(longlong *)(param_1 + 0x420) != 0) {
        uVar2 = FUN_141df5850(*(longlong *)(param_1 + 0x420),0,0);
        *(undefined8 *)(param_1 + 0x420) = uVar2;
      }
      *(undefined4 *)(param_1 + 0x428) = 0;
    }
  }
  else if (param_2 == *(longlong *)(param_1 + 0x3a8)) {
    FUN_14166c2a0(param_1,*(undefined1 *)(param_1 + 0x460));
    return;
  }
  return;
}



