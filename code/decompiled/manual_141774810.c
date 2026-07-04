// FUN_141774810 @ 141774810

void FUN_141774810(longlong param_1,longlong param_2)

{
  longlong *plVar1;
  longlong lVar2;
  
  *(ulonglong *)(param_2 + 0x20) =
       (ulonglong)(*(longlong *)(param_2 + 0x20) != 0) + *(longlong *)(param_2 + 0x20);
  plVar1 = *(longlong **)(param_1 + 0x620);
  if (plVar1 == (longlong *)0x0) {
    return;
  }
  if ((char)plVar1[0x80] == '\0') {
    lVar2 = plVar1[0x7c];
  }
  else {
    lVar2 = plVar1[0x7f];
  }
  (**(code **)(*plVar1 + 0x2e0))(plVar1,lVar2,0,1,0,DAT_1450d6c88);
  *(byte *)((longlong)plVar1 + 0x1ac) = *(byte *)((longlong)plVar1 + 0x1ac) & 0xf7;
  *(undefined2 *)((longlong)plVar1 + 0x401) = 1;
  return;
}


