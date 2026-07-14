// FUN_14165eda0 @ 14165eda0

undefined8 FUN_14165eda0(longlong param_1,undefined8 param_2)

{
  char cVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  lVar2 = FUN_140d2aef0();
  uVar3 = 0;
  if (lVar2 != 0) {
    uVar3 = *(undefined8 *)(lVar2 + 0x130);
  }
  lVar2 = FUN_14145e360(uVar3,param_2);
  if ((((*(longlong *)(lVar2 + 0x30) == 0) ||
       (cVar1 = FUN_14145c4b0(uVar3,*(undefined4 *)(param_1 + 0x1584),lVar2 + 0x30), cVar1 == '\x02'
       )) && ((*(longlong *)(lVar2 + 0x38) == 0 ||
              (cVar1 = FUN_14145c4b0(uVar3,*(undefined4 *)(param_1 + 0x1584)), cVar1 == '\x02'))))
     && ((*(longlong *)(lVar2 + 0x40) == 0 ||
         (cVar1 = FUN_14145c4b0(uVar3,*(undefined4 *)(param_1 + 0x1584)), cVar1 == '\x02')))) {
    uVar3 = 1;
  }
  else {
    uVar3 = 0;
  }
  return uVar3;
}


