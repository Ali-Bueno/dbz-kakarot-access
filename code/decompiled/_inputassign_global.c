// ============ FUN_141a51e70 @ 141a51e70  size=0x46b ============
//   141a51e86  MOV EAX,dword ptr [0x14569c3b8]
//   141a51e95  CMP EAX,dword ptr [0x14569c3e4]
//   141a51ea0  MOV RSI,qword ptr [0x14569c3f0]
//   141a51ea7  LEA R8,[0x14569c3e8]
//   141a51eae  MOV R14,qword ptr [0x14569c3f8]
//   141a51ede  MOV R9,qword ptr [0x14569c3b0]
//   141a51f2e  LEA RCX,[0x14569c3b0]
//   141a51f42  MOV R9,qword ptr [0x14569c3b0]
//   141a51f49  MOV R14,qword ptr [0x14569c3f8]
//   141a51f50  MOV RSI,qword ptr [0x14569c3f0]
//   141a52177  MOV R10D,dword ptr [0x14569c3b8]
//   141a5217e  LEA R11,[0x14569c3e8]
//   141a52185  SUB R10D,dword ptr [0x14569c3e4]
//   141a52286  MOV R9,qword ptr [0x14569c3b0]
//   141a52291  MOV R14,qword ptr [0x14569c3f8]
//   141a5229f  MOV RSI,qword ptr [0x14569c3f0]

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


// ============ FUN_141a57b50 @ 141a57b50  size=0x23 ============
//   141a57b54  LEA RDX,[0x14569c3b0]

void FUN_141a57b50(void)

{
  FUN_140c052a0(&DAT_14569c360,&DAT_14569c3b0);
  DAT_14569c149 = 1;
  return;
}


// ============ FUN_141a66230 @ 141a66230  size=0xcb ============
//   141a6624d  MOV R9,qword ptr [0x14569c3a0]
//   141a66267  MOVSXD RAX,dword ptr [0x14569c3a8]

undefined8 * FUN_141a66230(undefined8 *param_1,longlong *param_2)

{
  longlong lVar1;
  int iVar2;
  longlong lVar3;
  undefined *puVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  
  lVar3 = *param_2;
  if (DAT_14569c368 != DAT_14569c394) {
    puVar4 = DAT_14569c3a0;
    if (DAT_14569c3a0 == (undefined *)0x0) {
      puVar4 = &DAT_14569c398;
    }
    iVar2 = *(int *)(puVar4 + ((longlong)((int)((ulonglong)lVar3 >> 0x20) + (int)lVar3) &
                              (longlong)DAT_14569c3a8 - 1U) * 4);
    while (iVar2 != -1) {
      lVar1 = (longlong)iVar2 * 0x18;
      if (*(longlong *)(lVar1 + DAT_14569c360) == lVar3) {
        puVar6 = (undefined8 *)(lVar1 + DAT_14569c360);
        if (iVar2 != -1) goto LAB_141a662be;
        break;
      }
      iVar2 = *(int *)(lVar1 + 0x10 + DAT_14569c360);
    }
  }
  puVar6 = (undefined8 *)0x0;
LAB_141a662be:
  puVar5 = puVar6 + 1;
  if (puVar6 == (undefined8 *)0x0) {
    puVar5 = (undefined8 *)0x0;
  }
  if (puVar5 != (undefined8 *)0x0) {
    *param_1 = *puVar5;
    return param_1;
  }
  FUN_141eb3490(param_1,&DAT_1444fbfdc,1);
  return param_1;
}


// ============ FUN_141a66300 @ 141a66300  size=0xcb ============
//   141a66306  MOV EAX,dword ptr [0x14569c3b8]
//   141a6630f  CMP EAX,dword ptr [0x14569c3e4]
//   141a6631d  MOV R9,qword ptr [0x14569c3f0]
//   141a66324  LEA RCX,[0x14569c3e8]
//   141a66337  MOVSXD RAX,dword ptr [0x14569c3f8]
//   141a6635a  MOV R9,qword ptr [0x14569c3b0]

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


// ============ FUN_141a663e0 @ 141a663e0  size=0x8 ============
//   141a663e0  LEA RAX,[0x14569c3b0]

undefined8 * FUN_141a663e0(void)

{
  return &DAT_14569c3b0;
}


// ============ FUN_141a66a80 @ 141a66a80  size=0x3aa ============
//   141a66bc1  MOV EAX,dword ptr [0x14569c3b8]
//   141a66bc7  CMP EAX,dword ptr [0x14569c3e4]
//   141a66bd3  LEA R9,[0x14569c3e8]
//   141a66bda  MOV R8,qword ptr [0x14569c3f0]
//   141a66bed  MOVSXD RAX,dword ptr [0x14569c3f8]
//   141a66c0f  MOV R9,qword ptr [0x14569c3b0]
//   141a66cf9  MOV EAX,dword ptr [0x14569c3b8]
//   141a66cff  CMP EAX,dword ptr [0x14569c3e4]
//   141a66d0f  LEA R9,[0x14569c3e8]
//   141a66d16  MOV R8,qword ptr [0x14569c3f0]
//   141a66d29  MOVSXD RAX,dword ptr [0x14569c3f8]
//   141a66d4f  MOV R9,qword ptr [0x14569c3b0]

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


// ============ FUN_141a820d0 @ 141a820d0  size=0x35a ============
//   141a822e6  LEA RCX,[0x14569c3b0]
//   141a82305  LEA RCX,[0x14569c3b0]
//   141a82311  LEA RDX,[0x14569c3b0]

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141a820d0(undefined4 *param_1)

{
  int iVar1;
  longlong lVar2;
  undefined *puVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  longlong *plVar7;
  undefined8 *puVar8;
  undefined1 auStack_148 [32];
  undefined4 local_128 [2];
  longlong *local_120;
  wchar_t *local_118;
  code *local_108;
  longlong local_f8 [4];
  longlong *local_d8;
  code *local_c8;
  longlong local_b8 [4];
  longlong *local_98;
  undefined1 local_88 [32];
  longlong local_68;
  longlong local_58 [4];
  longlong *local_38;
  ulonglong local_18;
  
  local_18 = DAT_145909780 ^ (ulonglong)auStack_148;
  lVar2 = *(longlong *)(param_1 + 2);
  puVar8 = (undefined8 *)0x0;
  if (DAT_14569c458 != DAT_14569c484) {
    puVar3 = DAT_14569c490;
    if (DAT_14569c490 == (undefined *)0x0) {
      puVar3 = &DAT_14569c488;
    }
    for (iVar1 = *(int *)(puVar3 + ((longlong)((int)((ulonglong)lVar2 >> 0x20) + (int)lVar2) &
                                   (longlong)DAT_14569c498 - 1U) * 4); iVar1 != -1;
        iVar1 = *(int *)(DAT_14569c450 + 0x10 + (longlong)iVar1 * 0x18)) {
      if (*(longlong *)(DAT_14569c450 + (longlong)iVar1 * 0x18) == lVar2) {
        if (iVar1 != -1) goto LAB_141a821a8;
        break;
      }
    }
  }
  local_118 = L"Unknown_SwapGroupId";
  local_120 = (longlong *)(param_1 + 2);
  FUN_141a48bc0(&DAT_14569c450,local_128,&local_120,0);
LAB_141a821a8:
  FUN_141a4c060(local_88,param_1);
  local_98 = (longlong *)0x0;
  puVar4 = (undefined8 *)FUN_1407e5ee0(0x80,local_b8);
  puVar5 = puVar8;
  if (puVar4 != (undefined8 *)0x0) {
    *puVar4 = &PTR_FUN_14451af68;
    FUN_141a4c060(puVar4 + 2,local_88);
    puVar5 = puVar4;
  }
  if (puVar5 != (undefined8 *)0xfffffffffffffff0) {
    local_c8 = thunk_FUN_141a51e70;
  }
  if (local_68 != 0) {
    plVar7 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar7 = local_38;
    }
    (**(code **)(*plVar7 + 0x10))(plVar7,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  FUN_141a4c060(local_88,param_1);
  local_d8 = (longlong *)0x0;
  puVar5 = (undefined8 *)FUN_1407e5ee0(0x80,local_f8);
  if (puVar5 != (undefined8 *)0x0) {
    *puVar5 = &PTR_FUN_14451af68;
    FUN_141a4c060(puVar5 + 2,local_88);
    puVar8 = puVar5;
  }
  if (puVar8 != (undefined8 *)0xfffffffffffffff0) {
    local_108 = FUN_141a589d0;
  }
  if (local_68 != 0) {
    plVar7 = local_58;
    if (local_38 != (longlong *)0x0) {
      plVar7 = local_38;
    }
    (**(code **)(*plVar7 + 0x10))(plVar7,0);
  }
  if (local_38 != (longlong *)0x0) {
    FUN_141de9200();
  }
  switch(*param_1) {
  case 1:
    FUN_140c052a0(&DAT_14569c3b0,&DAT_14569c400);
    DAT_14569c149 = 0;
    break;
  case 2:
    FUN_140c052a0(&DAT_14569c3b0,&DAT_14569c400);
    goto LAB_141a82311;
  case 3:
    plVar7 = local_b8;
    if (local_98 != (longlong *)0x0) {
      plVar7 = local_98;
    }
    uVar6 = (**(code **)(*plVar7 + 8))();
    (*local_c8)(uVar6);
    break;
  case 4:
    plVar7 = local_b8;
    if (local_98 != (longlong *)0x0) {
      plVar7 = local_98;
    }
    uVar6 = (**(code **)(*plVar7 + 8))();
    (*local_c8)(uVar6);
LAB_141a82311:
    FUN_140c052a0(&DAT_14569c360,&DAT_14569c3b0);
    DAT_14569c149 = 1;
    break;
  case 5:
    local_128[0] = 3;
    goto LAB_141a8237f;
  case 6:
    local_128[0] = 4;
LAB_141a8237f:
    plVar7 = local_f8;
    if (local_d8 != (longlong *)0x0) {
      plVar7 = local_d8;
    }
    uVar6 = (**(code **)(*plVar7 + 8))();
    (*local_108)(uVar6,local_128);
  }
  if (local_108 != (code *)0x0) {
    plVar7 = local_f8;
    if (local_d8 != (longlong *)0x0) {
      plVar7 = local_d8;
    }
    (**(code **)(*plVar7 + 0x10))(plVar7,0);
  }
  if (local_d8 != (longlong *)0x0) {
    FUN_141de9200();
  }
  if (local_c8 != (code *)0x0) {
    plVar7 = local_b8;
    if (local_98 != (longlong *)0x0) {
      plVar7 = local_98;
    }
    (**(code **)(*plVar7 + 0x10))(plVar7,0);
  }
  if (local_98 != (longlong *)0x0) {
    FUN_141de9200();
  }
  return;
}


// ============ FUN_141a82450 @ 141a82450  size=0x34 ============
//   141a8246b  LEA RCX,[0x14569c3b0]

void FUN_141a82450(undefined8 param_1)

{
  FUN_140c052a0(&DAT_14569c360,param_1);
  FUN_140c052a0(&DAT_14569c3b0,param_1);
  DAT_14569c149 = 1;
  return;
}


// ============ FUN_141a84ae0 @ 141a84ae0  size=0x1f ============
//   141a84ae7  LEA RCX,[0x14569c3b0]

void FUN_141a84ae0(undefined8 param_1)

{
  FUN_140c052a0(&DAT_14569c3b0,param_1);
  DAT_14569c149 = 0;
  return;
}


// ============ FUN_141a8e3b0 @ 141a8e3b0  size=0x23 ============
//   141a8e3bb  LEA RCX,[0x14569c3b0]

void FUN_141a8e3b0(void)

{
  FUN_140c052a0(&DAT_14569c3b0,&DAT_14569c360);
  DAT_14569c149 = 1;
  return;
}


// ============ FUN_14387b570 @ 14387b570  size=0x2f ============
//   14387b574  MOV RCX,qword ptr [0x14569c3a0]
//   14387b57b  MOV dword ptr [0x14569c3a8],0x0

void FUN_14387b570(void)

{
  DAT_14569c3a8 = 0;
  if (DAT_14569c3a0 != 0) {
    FUN_141de9200();
  }
  FUN_1407fa390(&DAT_14569c360);
  return;
}


// ============ FUN_14387b5d0 @ 14387b5d0  size=0x2f ============
//   14387b5d4  MOV RCX,qword ptr [0x14569c3f0]
//   14387b5db  MOV dword ptr [0x14569c3f8],0x0
//   14387b5ef  LEA RCX,[0x14569c3b0]

void FUN_14387b5d0(void)

{
  DAT_14569c3f8._0_4_ = 0;
  if (DAT_14569c3f0 != 0) {
    FUN_141de9200();
  }
  FUN_1407fa390(&DAT_14569c3b0);
  return;
}


