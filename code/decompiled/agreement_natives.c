// UAT_UIXcmnAgreement native registration table (exec + impl + impl callees).
// Winner table @ 0x144698b40  anchorScore=16  members=74
// Reflected anchors: +0x398 RootPanel, +0x3A8 SubjectImage, +0x3B0 HeadImage,
//   +0x3C0 LeftArrow, +0x3C8 RightArrow, +0x508 ImageTextureMap(TMap<u32,UTexture2D*>).
//   Tail 0x3E0..0x508 = unreflected state (currentPage / currentDoc live here).

// ======== EXEC OnCloseFinished  @ 141d7d880 ========

void FUN_141d7d880(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd7810();
  return;
}



// ======== EXEC OnCloseNewDialogFinished  @ 141d7d960 ========

void FUN_141d7d960(longlong param_1,longlong param_2)

{
  int iVar1;
  int *piVar2;
  
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  *(undefined1 *)(param_1 + 0xb8) = 0;
  iVar1 = 0;
  *(undefined8 *)(param_1 + 0xe0) = 0;
  if (0 < *(int *)(param_1 + 200)) {
    piVar2 = *(int **)(param_1 + 0xd0);
    while (*piVar2 != 2) {
      iVar1 = iVar1 + 1;
      piVar2 = piVar2 + 8;
      if (*(int *)(param_1 + 200) <= iVar1) {
        return;
      }
    }
    *(int **)(param_1 + 0xe0) = *(int **)(param_1 + 0xd0) + (longlong)iVar1 * 8;
  }
  return;
}



// ======== EXEC OnDeckRecipeOpenFinished  @ 141d7d9e0 ========

void FUN_141d7d9e0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd80b0();
  return;
}



// ======== EXEC OnEnterFinished  @ 141d7da80 ========

void FUN_141d7da80(longlong param_1,longlong param_2)

{
  int iVar1;
  int *piVar2;
  
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  if ((-1 < *(int *)(param_1 + 0x180)) && (*(int *)(param_1 + 0x180) < *(int *)(param_1 + 0x178))) {
    iVar1 = 0;
    *(undefined8 *)(param_1 + 0xe0) = 0;
    if (0 < *(int *)(param_1 + 200)) {
      piVar2 = *(int **)(param_1 + 0xd0);
      while (*piVar2 != 3) {
        iVar1 = iVar1 + 1;
        piVar2 = piVar2 + 8;
        if (*(int *)(param_1 + 200) <= iVar1) {
          FUN_141b7df90(param_1 + 0xc0,param_1,0);
          return;
        }
      }
      *(int **)(param_1 + 0xe0) = *(int **)(param_1 + 0xd0) + (longlong)iVar1 * 8;
    }
    FUN_141b7df90(param_1 + 0xc0,param_1,0);
    return;
  }
  return;
}



// ======== EXEC OnOpenFinished  @ 141d7dc40 ========

void FUN_141d7dc40(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bdc3e0();
  return;
}



// ======== EXEC OnOpenNewDialogFinished  @ 141d7dce0 ========

void FUN_141d7dce0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bdc510();
  return;
}



// ======== EXEC CheckHovered  @ 141d7be20 ========

void FUN_141d7be20(undefined8 param_1,longlong param_2)

{
  undefined8 local_res10;
  
  local_res10 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bcce50(param_1,local_res10);
  return;
}



// ======== IMPL CheckHovered  @ 14204e7f0 ========

void FUN_14204e7f0(longlong param_1,undefined8 param_2)

{
  byte bVar1;
  
  bVar1 = **(byte **)(param_1 + 0x20);
  *(byte **)(param_1 + 0x20) = *(byte **)(param_1 + 0x20) + 1;
                    /* WARNING: Could not recover jumptable at 0x00014204e812. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(&DAT_145b9d480)[bVar1])(param_2,param_1);
  return;
}



// ======== EXEC MouseClickDecide  @ 141d7c6f0 ========

void FUN_141d7c6f0(undefined8 param_1,longlong param_2)

{
  undefined1 local_res10 [8];
  undefined8 local_res20;
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  local_res20 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res20);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res20);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd67f0(param_1,local_res10[0],local_res20);
  return;
}



// ======== EXEC MouseWheel  @ 141d7d4e0 ========

void FUN_141d7d4e0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [6];
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd6ed0(param_1,local_res10[0]);
  return;
}



// ======== EXEC OnBegined  @ 141d77ec0 ========

void FUN_141d77ec0(longlong param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  *(undefined1 *)(param_1 + 0xb8) = 1;
  return;
}



// ======== EXEC OnCloseDialogFinished  @ 141d7d7a0 ========

void FUN_141d7d7a0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd74a0();
  return;
}



// ======== EXEC OnCloseFinished  @ 141d7d8a0 ========

void FUN_141d7d8a0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd7b80();
  return;
}



// ======== EXEC OnEnterInputFinished  @ 141d7dae0 ========

void FUN_141d7dae0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd60a0();
  return;
}



// ======== EXEC OnOpenFinished  @ 141d7dc60 ========

void FUN_141d7dc60(longlong param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  *(undefined1 *)(param_1 + 0xb8) = 0;
  FUN_141be1a10(param_1,1);
  return;
}



// ======== EXEC PressButtonBack  @ 141d7dda0 ========

void FUN_141d7dda0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bdd840();
  return;
}



// ======== EXEC CheckHovered  @ 141d7bea0 ========

void FUN_141d7bea0(undefined8 param_1,longlong param_2)

{
  undefined8 local_res10;
  
  local_res10 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bcd190(param_1,local_res10);
  return;
}



// ======== EXEC MouseClickDecide  @ 141d7c7b0 ========

void FUN_141d7c7b0(undefined8 param_1,longlong param_2)

{
  undefined1 local_res10 [8];
  undefined8 local_res20;
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  local_res20 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res20);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res20);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd6980(param_1,local_res10[0],local_res20);
  return;
}



// ======== EXEC OnCloseFinished  @ 141d785e0 ========

void FUN_141d785e0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141b71eb0(param_1,0);
  return;
}



// ======== EXEC CheckHovered  @ 141d7bf20 ========

void FUN_141d7bf20(undefined8 param_1,longlong param_2)

{
  undefined8 local_res10;
  
  local_res10 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bcd260(param_1,local_res10);
  return;
}



// ======== EXEC MouseClickDecide  @ 141d7c870 ========

void FUN_141d7c870(undefined8 param_1,longlong param_2)

{
  undefined1 local_res10 [8];
  undefined8 local_res20;
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  local_res20 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res20);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res20);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd6a20(param_1,local_res10[0],local_res20);
  return;
}



// ======== EXEC MouseWheel  @ 141d7d560 ========

void FUN_141d7d560(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [6];
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd6fb0(param_1,local_res10[0]);
  return;
}



// ======== EXEC CheckHoveredBtn  @ 141d7c120 ========

void FUN_141d7c120(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bcd5a0();
  return;
}



// ======== EXEC CheckHoveredInput  @ 141d7c1c0 ========

void FUN_141d7c1c0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bcd720();
  return;
}



// ======== EXEC MouseClickDecide  @ 141d7c930 ========

void FUN_141d7c930(undefined8 param_1,longlong param_2)

{
  undefined1 local_res10 [8];
  undefined8 local_res20;
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  local_res20 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res20);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res20);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd6ab0(param_1,local_res10[0],local_res20);
  return;
}



// ======== EXEC OnCloseFinished  @ 141d7d8c0 ========

void FUN_141d7d8c0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd7c40();
  return;
}



// ======== EXEC OnCloseWindowFinished  @ 141d7d9a0 ========

void FUN_141d7d9a0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd7ff0();
  return;
}



// ======== EXEC OnEnterFinished  @ 141d7daa0 ========

void FUN_141d7daa0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd85d0();
  return;
}



// ======== EXEC OnEnterInputFinished  @ 141d7db00 ========

void FUN_141d7db00(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd6330();
  return;
}



// ======== EXEC OnOpenFinished  @ 141d78360 ========

void FUN_141d78360(longlong param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  *(undefined1 *)(param_1 + 0xb8) = 0;
  return;
}



// ======== EXEC OnOpenWindowFinished  @ 141d785c0 ========

void FUN_141d785c0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bbc370();
  return;
}



// ======== EXEC CheckHovered  @ 141d7bfa0 ========

void FUN_141d7bfa0(undefined8 param_1,longlong param_2)

{
  undefined8 local_res10;
  
  local_res10 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bcd360(param_1,local_res10);
  return;
}



// ======== EXEC MouseClickDecide  @ 141d7c9f0 ========

void FUN_141d7c9f0(undefined8 param_1,longlong param_2)

{
  undefined1 local_res10 [8];
  undefined8 local_res20;
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  local_res20 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res20);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res20);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd6b00(param_1,local_res10[0],local_res20);
  return;
}



// ======== EXEC MouseWheel  @ 141d77e40 ========

void FUN_141d77e40(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [6];
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bb7ec0(param_1,local_res10[0]);
  return;
}



// ======== EXEC CheckHovered  @ 141d7c020 ========

void FUN_141d7c020(undefined8 param_1,longlong param_2)

{
  undefined8 local_res10;
  
  local_res10 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bcd460(param_1,local_res10);
  return;
}



// ======== EXEC MouseClickDecide  @ 141d7cab0 ========

void FUN_141d7cab0(undefined8 param_1,longlong param_2)

{
  undefined1 local_res10 [8];
  undefined8 local_res20;
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  local_res20 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res20);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res20);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd6b90(param_1,local_res10[0],local_res20);
  return;
}



// ======== EXEC OnCloseFinished  @ 141d7d8e0 ========

void FUN_141d7d8e0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd7de0();
  return;
}



// ======== EXEC OnEnterFinished  @ 141d7dac0 ========

void FUN_141d7dac0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bd86b0();
  return;
}



// ======== EXEC OnOpenFinished  @ 141d7dc80 ========

void FUN_141d7dc80(longlong param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  *(undefined1 *)(param_1 + 0xb8) = 0;
  FUN_141be29c0();
  return;
}



// ======== EXEC OnWindowClosed  @ 141d7dd80 ========

void FUN_141d7dd80(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bdc780();
  return;
}



// ======== EXEC OnCloseDialogFinished  @ 141d7d7c0 ========

void FUN_141d7d7c0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf8320();
  return;
}



// ======== EXEC OnCloseFinished  @ 141d7d900 ========

void FUN_141d7d900(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf8550();
  return;
}



// ======== EXEC OnLoaded  @ 141d7db40 ========

void FUN_141d7db40(undefined8 param_1,longlong param_2)

{
  undefined8 local_res10;
  
  local_res10 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bfb4b0(param_1,local_res10);
  return;
}



// ======== EXEC OnOpenDialogFinished  @ 141d7dbc0 ========

void FUN_141d7dbc0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bfb820();
  return;
}



// ======== EXEC OnOpenFinished  @ 141d7dca0 ========

void FUN_141d7dca0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bfb870();
  return;
}



// ======== EXEC OnReceiveFinished  @ 141d7dd00 ========

void FUN_141d7dd00(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bfbb60();
  return;
}



// ======== EXEC CheckHovered  @ 141d7c0a0 ========

void FUN_141d7c0a0(undefined8 param_1,longlong param_2)

{
  undefined8 local_res10;
  
  local_res10 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141befc80(param_1,local_res10);
  return;
}



// ======== EXEC MouseClickDecide  @ 141d7cb70 ========

void FUN_141d7cb70(undefined8 param_1,longlong param_2)

{
  undefined1 local_res10 [8];
  undefined8 local_res20;
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  local_res20 = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_res20);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_res20);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf7240(param_1,local_res10[0],local_res20);
  return;
}



// ======== EXEC MouseWheel  @ 141d7d5e0 ========

void FUN_141d7d5e0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [6];
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf7670(param_1,local_res10[0]);
  return;
}



// ======== EXEC OnCloseDialogFinished  @ 141d7d7e0 ========

void FUN_141d7d7e0(longlong param_1,longlong param_2)

{
  int iVar1;
  int *piVar2;
  
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  *(undefined1 *)(param_1 + 0xb8) = 0;
  iVar1 = 0;
  *(undefined8 *)(param_1 + 0xe0) = 0;
  if (0 < *(int *)(param_1 + 200)) {
    piVar2 = *(int **)(param_1 + 0xd0);
    while (*piVar2 != 6) {
      iVar1 = iVar1 + 1;
      piVar2 = piVar2 + 8;
      if (*(int *)(param_1 + 200) <= iVar1) {
        return;
      }
    }
    *(int **)(param_1 + 0xe0) = *(int **)(param_1 + 0xd0) + (longlong)iVar1 * 8;
  }
  return;
}



// ======== EXEC OnCloseFinished  @ 141d7d920 ========

void FUN_141d7d920(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf8700();
  return;
}



// ======== EXEC OnInitializeFinished  @ 141d7db20 ========

void FUN_141d7db20(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf8a70();
  return;
}



// ======== EXEC OnOpenFinished  @ 141d7dcc0 ========

void FUN_141d7dcc0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bfb9d0();
  return;
}



// ======== EXEC OnReceiveFinished  @ 141d7dd20 ========

void FUN_141d7dd20(longlong param_1,longlong param_2)

{
  int iVar1;
  int *piVar2;
  
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  iVar1 = 0;
  *(undefined8 *)(param_1 + 0xe0) = 0;
  if (0 < *(int *)(param_1 + 200)) {
    piVar2 = *(int **)(param_1 + 0xd0);
    do {
      if (*piVar2 == 0) {
        *(int **)(param_1 + 0xe0) = *(int **)(param_1 + 0xd0) + (longlong)iVar1 * 8;
        break;
      }
      iVar1 = iVar1 + 1;
      piVar2 = piVar2 + 8;
    } while (iVar1 < *(int *)(param_1 + 200));
  }
  FUN_141b7df90(param_1 + 0xc0,param_1,0);
  return;
}



// ======== EXEC OnSlideFinished  @ 141d7dd40 ========

void FUN_141d7dd40(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bfbc30();
  return;
}



// ======== EXEC PressButtonEnter  @ 141d7ddc0 ========

void FUN_141d7ddc0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bfd3d0();
  return;
}



// ======== EXEC PressButtonL1  @ 141d7dde0 ========

void FUN_141d7dde0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bfe2e0();
  return;
}



// ======== EXEC PressButtonR1  @ 141d7de00 ========

void FUN_141d7de00(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bfe740();
  return;
}



// ======== EXEC DetailMouseWheel  @ 141d7c1e0 ========

void FUN_141d7c1e0(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [6];
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf0a60(param_1,local_res10[0]);
  return;
}



// ======== EXEC MouseClickDecide  @ 141d7cc30 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141d7cc30(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  int *piVar2;
  longlong *plVar3;
  undefined1 *puVar4;
  undefined ***pppuVar5;
  undefined1 auStack_e8 [32];
  undefined **local_c8;
  ushort local_c0;
  undefined1 local_be;
  undefined4 local_bc;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 *local_98;
  undefined8 local_90;
  undefined8 local_88;
  longlong *plStack_80;
  undefined8 local_78;
  undefined4 local_70;
  undefined2 local_6c;
  undefined8 local_68;
  undefined1 local_60;
  undefined1 local_58 [56];
  ulonglong local_20;
  
  local_20 = DAT_145909780 ^ (ulonglong)auStack_e8;
  FUN_1420b2130(local_58);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_58);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_58);
  }
  local_be = 0;
  local_bc = 0;
  puVar4 = local_58;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar4 = *(undefined1 **)(param_2 + 0x38);
  }
  local_88 = 0;
  plStack_80 = (longlong *)0x0;
  local_c0 = local_c0 & 0xfe00;
  local_70 = DAT_1450d6c88;
  local_c8 = &PTR_FUN_143fd02c0;
  local_b8 = 0;
  local_98 = &DAT_145ba1000;
  local_b0 = 0;
  local_a8 = 0;
  uStack_a0 = 0;
  local_90 = 0;
  local_78 = 0;
  local_6c = 0;
  local_68 = 0;
  local_60 = 0;
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_c8);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_c8);
  }
  pppuVar5 = &local_c8;
  if (*(undefined ****)(param_2 + 0x38) != (undefined ***)0x0) {
    pppuVar5 = *(undefined ****)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf72d0(param_1,puVar4,pppuVar5);
  plVar3 = plStack_80;
  if (plStack_80 != (longlong *)0x0) {
    plVar1 = plStack_80 + 1;
    *(int *)plVar1 = (int)*plVar1 + -1;
    if ((int)*plVar1 == 0) {
      (**(code **)*plStack_80)(plStack_80);
      piVar2 = (int *)((longlong)plVar3 + 0xc);
      *piVar2 = *piVar2 + -1;
      if (*piVar2 == 0) {
        (**(code **)(*plVar3 + 8))(plVar3,1);
      }
    }
  }
  return;
}



// ======== IMPL MouseClickDecide  @ 1420b2130 ========

undefined8 * FUN_1420b2130(undefined8 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0x3f800000;
  *(undefined4 *)(param_1 + 2) = 0;
  *(undefined4 *)((longlong)param_1 + 0x24) = 0;
  *(undefined8 *)((longlong)param_1 + 0x1c) = 0x3f800000;
  param_1[5] = 0x3f800000;
  *(undefined4 *)(param_1 + 6) = 0;
  *(byte *)((longlong)param_1 + 0x34) = *(byte *)((longlong)param_1 + 0x34) & 0xfe;
  return param_1;
}



// ======== EXEC MouseClickDownArrow  @ 141d7cf40 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141d7cf40(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  int *piVar2;
  longlong *plVar3;
  undefined1 *puVar4;
  undefined ***pppuVar5;
  undefined1 auStack_e8 [32];
  undefined **local_c8;
  ushort local_c0;
  undefined1 local_be;
  undefined4 local_bc;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 *local_98;
  undefined8 local_90;
  undefined8 local_88;
  longlong *plStack_80;
  undefined8 local_78;
  undefined4 local_70;
  undefined2 local_6c;
  undefined8 local_68;
  undefined1 local_60;
  undefined1 local_58 [56];
  ulonglong local_20;
  
  local_20 = DAT_145909780 ^ (ulonglong)auStack_e8;
  FUN_1420b2130(local_58);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_58);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_58);
  }
  local_be = 0;
  local_bc = 0;
  puVar4 = local_58;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar4 = *(undefined1 **)(param_2 + 0x38);
  }
  local_88 = 0;
  plStack_80 = (longlong *)0x0;
  local_c0 = local_c0 & 0xfe00;
  local_70 = DAT_1450d6c88;
  local_c8 = &PTR_FUN_143fd02c0;
  local_b8 = 0;
  local_98 = &DAT_145ba1000;
  local_b0 = 0;
  local_a8 = 0;
  uStack_a0 = 0;
  local_90 = 0;
  local_78 = 0;
  local_6c = 0;
  local_68 = 0;
  local_60 = 0;
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_c8);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_c8);
  }
  pppuVar5 = &local_c8;
  if (*(undefined ****)(param_2 + 0x38) != (undefined ***)0x0) {
    pppuVar5 = *(undefined ****)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf75b0(param_1,puVar4,pppuVar5);
  plVar3 = plStack_80;
  if (plStack_80 != (longlong *)0x0) {
    plVar1 = plStack_80 + 1;
    *(int *)plVar1 = (int)*plVar1 + -1;
    if ((int)*plVar1 == 0) {
      (**(code **)*plStack_80)(plStack_80);
      piVar2 = (int *)((longlong)plVar3 + 0xc);
      *piVar2 = *piVar2 + -1;
      if (*piVar2 == 0) {
        (**(code **)(*plVar3 + 8))(plVar3,1);
      }
    }
  }
  return;
}



// ======== EXEC MouseClickTabLeft  @ 141d7d190 ========

void FUN_141d7d190(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf75e0();
  return;
}



// ======== EXEC MouseClickTabRight  @ 141d7d1b0 ========

void FUN_141d7d1b0(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf7610();
  return;
}



// ======== EXEC MouseClickUpArrow  @ 141d7d1d0 ========

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_141d7d1d0(undefined8 param_1,longlong param_2)

{
  longlong *plVar1;
  int *piVar2;
  longlong *plVar3;
  undefined1 *puVar4;
  undefined ***pppuVar5;
  undefined1 auStack_e8 [32];
  undefined **local_c8;
  ushort local_c0;
  undefined1 local_be;
  undefined4 local_bc;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 *local_98;
  undefined8 local_90;
  undefined8 local_88;
  longlong *plStack_80;
  undefined8 local_78;
  undefined4 local_70;
  undefined2 local_6c;
  undefined8 local_68;
  undefined1 local_60;
  undefined1 local_58 [56];
  ulonglong local_20;
  
  local_20 = DAT_145909780 ^ (ulonglong)auStack_e8;
  FUN_1420b2130(local_58);
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_58);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_58);
  }
  local_be = 0;
  local_bc = 0;
  puVar4 = local_58;
  if (*(undefined1 **)(param_2 + 0x38) != (undefined1 *)0x0) {
    puVar4 = *(undefined1 **)(param_2 + 0x38);
  }
  local_88 = 0;
  plStack_80 = (longlong *)0x0;
  local_c0 = local_c0 & 0xfe00;
  local_70 = DAT_1450d6c88;
  local_c8 = &PTR_FUN_143fd02c0;
  local_b8 = 0;
  local_98 = &DAT_145ba1000;
  local_b0 = 0;
  local_a8 = 0;
  uStack_a0 = 0;
  local_90 = 0;
  local_78 = 0;
  local_6c = 0;
  local_68 = 0;
  local_60 = 0;
  *(undefined8 *)(param_2 + 0x38) = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,&local_c8);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),&local_c8);
  }
  pppuVar5 = &local_c8;
  if (*(undefined ****)(param_2 + 0x38) != (undefined ***)0x0) {
    pppuVar5 = *(undefined ****)(param_2 + 0x38);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf7640(param_1,puVar4,pppuVar5);
  plVar3 = plStack_80;
  if (plStack_80 != (longlong *)0x0) {
    plVar1 = plStack_80 + 1;
    *(int *)plVar1 = (int)*plVar1 + -1;
    if ((int)*plVar1 == 0) {
      (**(code **)*plStack_80)(plStack_80);
      piVar2 = (int *)((longlong)plVar3 + 0xc);
      *piVar2 = *piVar2 + -1;
      if (*piVar2 == 0) {
        (**(code **)(*plVar3 + 8))(plVar3,1);
      }
    }
  }
  return;
}



// ======== EXEC MouseWheel  @ 141d7d660 ========

void FUN_141d7d660(undefined8 param_1,longlong param_2)

{
  undefined4 local_res10 [6];
  
  local_res10[0] = 0;
  if (*(longlong *)(param_2 + 0x20) == 0) {
    *(undefined8 *)(param_2 + 0x80) = *(undefined8 *)(*(longlong *)(param_2 + 0x80) + 0x28);
    FUN_14204e820(param_2,local_res10);
  }
  else {
    FUN_14204e7f0(param_2,*(undefined8 *)(param_2 + 0x18),local_res10);
  }
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf76d0(param_1,local_res10[0]);
  return;
}



// ======== EXEC OnCloseDetailFinished  @ 141d7d780 ========

void FUN_141d7d780(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf82b0();
  return;
}



// ======== EXEC OnCloseFinished  @ 141d7d940 ========

void FUN_141d7d940(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141bf8760();
  return;
}



// ======== EXEC OnSlideOutFinished  @ 141d7dd60 ========

void FUN_141d7dd60(undefined8 param_1,longlong param_2)

{
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  FUN_141c005d0();
  return;
}



