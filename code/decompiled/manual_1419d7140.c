// FUN_1419d7140 @ 1419d7140

undefined4 FUN_1419d7140(longlong param_1,undefined4 param_2)

{
  char cVar1;
  int iVar2;
  
  cVar1 = FUN_140d30560(param_2);
  if (cVar1 != '\0') {
    iVar2 = FUN_140d2ac30(param_2);
    if (-1 < iVar2) {
      return *(undefined4 *)((longlong)iVar2 * 0x368 + 0x3f0 + param_1);
    }
  }
  return 1;
}


