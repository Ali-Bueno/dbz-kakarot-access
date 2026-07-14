// FUN_14166c2a0 @ 14166c2a0

void FUN_14166c2a0(longlong *param_1,byte param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  longlong *plVar4;
  undefined *puVar5;
  undefined4 uVar6;
  undefined1 local_res8 [8];
  
  if (param_1[0x76] == 0) {
    return;
  }
  if (param_1[0x77] == 0) {
    return;
  }
  (**(code **)(*(longlong *)param_1[0x7b] + 0x250))((longlong *)param_1[0x7b],1);
  uVar1 = FUN_1429a9b10(param_1[0x76]);
  uVar2 = FUN_1429a9b10(param_1[0x77]);
  if (10 < param_2) goto LAB_14166c7b6;
  switch((ulonglong)param_2) {
  case 0:
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,L"Alpha_Eff",1);
    FUN_142e31a70(uVar1,*puVar3,DAT_1450d6c88);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee8748,1);
    FUN_142e31a70(uVar1,*puVar3,0);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee8798,1);
    FUN_142e31a70(uVar2,*puVar3,DAT_14391fc74);
    uVar1 = 1;
    goto LAB_14166c794;
  case 1:
    (**(code **)(*param_1 + 0x250))
              (param_1,CONCAT71((int7)((ulonglong)
                                       (IMAGE_DOS_HEADER_140000000.e_magic +
                                       (&switchD_14166c327::switchdataD_14166c7f4)[param_2]) >> 8),1
                               ));
    FUN_14165ec80(param_1);
    goto LAB_14166c7b6;
  case 2:
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,L"Alpha_Eff",1);
    FUN_142e31a70(uVar1,*puVar3,DAT_143aaf3cc);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee87b8,1);
    FUN_142e31a70(uVar1,*puVar3,DAT_14391fc74);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee87c0,1);
    uVar6 = DAT_14391b46c;
    goto LAB_14166c787;
  case 3:
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,L"Alpha_Eff",1);
    FUN_142e31a70(uVar1,*puVar3,DAT_1450d6c88);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee87e0,1);
    FUN_142e31a70(uVar1,*puVar3,DAT_143a08b04);
    puVar5 = &DAT_143ee87e8;
    break;
  case 4:
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,L"Alpha_Eff",1);
    FUN_142e31a70(uVar1,*puVar3,DAT_1450d6c88);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee8808,1);
    FUN_142e31a70(uVar1,*puVar3,DAT_14391b46c);
    puVar5 = &DAT_143ee8810;
    break;
  case 5:
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,L"Alpha_Eff",1);
    FUN_142e31a70(uVar1,*puVar3,DAT_143aaf3cc);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee8830,1);
    FUN_142e31a70(uVar1,*puVar3,DAT_143a862e8);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee8838,1);
    uVar6 = DAT_143a862f8;
    goto LAB_14166c787;
  case 6:
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,L"Alpha_Eff",1);
    FUN_142e31a70(uVar1,*puVar3,DAT_1450d6c88);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee8858,1);
    FUN_142e31a70(uVar1,*puVar3,DAT_143a08b14);
    puVar5 = &DAT_143ee8860;
    break;
  case 7:
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,L"Alpha_Eff",1);
    FUN_142e31a70(uVar1,*puVar3,DAT_1450d6c88);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee8900,1);
    FUN_142e31a70(uVar1,*puVar3,DAT_143aaf3cc);
    puVar5 = &DAT_143ee8908;
    break;
  case 8:
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,L"Alpha_Eff",1);
    FUN_142e31a70(uVar1,*puVar3,DAT_143aaf3cc);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee8938,1);
    FUN_142e31a70(uVar1,*puVar3,DAT_143a7e528);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee8940,1);
    FUN_142e31a70(uVar2,*puVar3,DAT_14391fc74);
    (**(code **)(*(longlong *)param_1[0x78] + 0x250))((longlong *)param_1[0x78],3);
    plVar4 = (longlong *)param_1[0x7e];
    goto LAB_14166c7ab;
  case 9:
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,L"Alpha_Eff",1);
    FUN_142e31a70(uVar1,*puVar3,DAT_1450d6c88);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee8960,1);
    FUN_142e31a70(uVar1,*puVar3,DAT_143a862f4);
    puVar5 = &DAT_143ee8968;
    break;
  case 10:
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,L"Alpha_Eff",1);
    FUN_142e31a70(uVar1,*puVar3,DAT_1450d6c88);
    puVar3 = (undefined8 *)FUN_141eb3490(local_res8,&DAT_143ee8990,1);
    FUN_142e31a70(uVar1,*puVar3,DAT_143a862f8);
    puVar5 = &DAT_143ee8998;
  }
  puVar3 = (undefined8 *)FUN_141eb3490(local_res8,puVar5,1);
  uVar6 = DAT_14391fc74;
LAB_14166c787:
  FUN_142e31a70(uVar2,*puVar3,uVar6);
  uVar1 = 3;
LAB_14166c794:
  (**(code **)(*(longlong *)param_1[0x78] + 0x250))((longlong *)param_1[0x78],uVar1);
  plVar4 = (longlong *)param_1[0x7d];
LAB_14166c7ab:
  (**(code **)(*plVar4 + 0x250))(plVar4,3);
LAB_14166c7b6:
  (**(code **)(*(longlong *)param_1[0x7f] + 0x250))((longlong *)param_1[0x7f],3);
  (**(code **)(*(longlong *)param_1[0x80] + 0x250))((longlong *)param_1[0x80],3);
  *(byte *)(param_1 + 0x8c) = param_2;
  return;
}


