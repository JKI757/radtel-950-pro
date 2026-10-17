/* Ghidra 12.1.4 decompilation of V0.27 app image.
 * Image base 0x08003000. Pseudocode is analysis output, not original source. */

/* ===== requested 0x80032a0 ===== */
/* function FUN_08003280 entry 0x08003280 */

void FUN_08003280(void)

{
  undefined4 uVar1;
  undefined4 extraout_r2;
  undefined8 uVar2;
  
  uVar1 = FUN_08004f8c();
  FUN_08003258(uVar1,extraout_r2);
  FUN_08028d88();
  uVar2 = func_0x0800542c();
  FUN_0800327c();
  FUN_080277b6((int)uVar2,(int)((ulonglong)uVar2 >> 0x20));
  (*DAT_080032c8)();
                    /* WARNING: Could not recover jumptable at 0x080032a6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_080032cc)();
  return;
}



/* ===== requested 0x80155ae ===== */
/* function FUN_080155ae entry 0x080155ae */

void FUN_080155ae(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x14) = param_2;
  return;
}



/* ===== requested 0x80155b2 ===== */
/* function FUN_080155b2 entry 0x080155b2 */

void FUN_080155b2(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x10) = param_2;
  return;
}



/* ===== requested 0x801559a ===== */
/* function FUN_0801559a entry 0x0801559a */

bool FUN_0801559a(int param_1,uint param_2)

{
  return (*(uint *)(param_1 + 8) & param_2) != 0;
}



/* ===== requested 0x800a4f2 ===== */
/* no function */

/* ===== requested 0x8016b2a ===== */
/* function FUN_08016b20 entry 0x08016b20 */

void FUN_08016b20(void)

{
  undefined4 uVar1;
  undefined4 local_1c;
  undefined2 local_18;
  undefined2 local_16;
  undefined2 local_14;
  undefined2 local_12;
  undefined2 local_10;
  short local_c;
  undefined1 local_a;
  undefined1 local_9;
  
  FUN_080155b6(&local_c);
  uVar1 = DAT_08016ba4;
  local_c = 0x400;
  local_a = 2;
  local_9 = 0x18;
  FUN_080150a0(DAT_08016ba4,&local_c);
  local_c = (short)((int)uVar1 >> 0x13);
  local_9 = 0x48;
  FUN_080150a0(uVar1,&local_c);
  FUN_080255d4(&local_1c);
  uVar1 = DAT_08016ba8;
  local_1c = 0x2580;
  local_18 = 0;
  local_16 = 0;
  local_14 = 0;
  local_10 = 0;
  local_12 = 0xc;
  FUN_0802540c(DAT_08016ba8,&local_1c);
  FUN_080253d4(uVar1,0x525,1);
  FUN_08025308(uVar1,1);
  return;
}



/* ===== requested 0x8025738 ===== */
/* no function */

/* ===== requested 0x8017000 ===== */
/* no function */

/* ===== requested 0x801e260 ===== */
/* function FUN_0801e260 entry 0x0801e260 */

void FUN_0801e260(uint param_1)

{
  undefined4 uVar1;
  uint uVar2;
  
  uVar1 = DAT_0801e2ac;
  uVar2 = 0x80;
  do {
    FUN_080155ae(uVar1,0x400);
    if ((uVar2 & param_1) == 0) {
      FUN_080155ae(uVar1,0x800);
    }
    else {
      FUN_080155b2(uVar1,0x800);
    }
    FUN_0800d962(5);
    FUN_080155b2(uVar1,0x400);
    FUN_0800d962(5);
    uVar2 = uVar2 >> 1;
  } while (uVar2 != 0);
  return;
}



/* ===== requested 0x801f090 ===== */
/* function FUN_0801f090 entry 0x0801f090 */

void FUN_0801f090(undefined4 param_1,uint param_2)

{
  undefined4 uVar1;
  
  uVar1 = DAT_0801f0e0;
  FUN_080155ae(DAT_0801f0e0,0x100);
  FUN_0800d962(5);
  FUN_0801e260(param_1);
  FUN_0801e260(param_2 >> 8);
  FUN_0801e260(param_2 & 0xff);
  FUN_0800d962(5);
  FUN_080155b2(uVar1,0x100);
  FUN_0800d962(5);
  FUN_080155ae(uVar1,0x400);
  return;
}



/* ===== requested 0x801f0e4 ===== */
/* function FUN_0801f0e4 entry 0x0801f0e4 */

void FUN_0801f0e4(undefined4 param_1,uint param_2)

{
  undefined4 uVar1;
  
  uVar1 = DAT_0801f12c;
  FUN_080155ae(DAT_0801f12c,0x8000);
  FUN_0800d962(5);
  FUN_0801e260(param_1);
  FUN_0801e260(param_2 >> 8);
  FUN_0801e260(param_2 & 0xff);
  FUN_080155b2(uVar1,0x8000);
  FUN_0800d962(5);
  FUN_080155ae(uVar1,0x400);
  return;
}



/* ===== requested 0x801d268 ===== */
/* function FUN_0801d268 entry 0x0801d268 */

void FUN_0801d268(int param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar2 = DAT_0801d3ec;
  uVar1 = DAT_0801d3e8;
  if (param_2 == 1) {
    if ((*(char *)(DAT_0801d3e4 + 0x4a) == -0x5b) && (*(char *)(DAT_0801d3e4 + 1) != '\x02')) {
      FUN_0801dfcc(0);
    }
    if (param_1 != 0) {
      if ((param_1 == 1) || (param_1 == 3)) {
        FUN_080155ae(uVar1,0x2000);
        FUN_080155ae(uVar2,1);
        FUN_080155b2(uVar1,0x1000);
        return;
      }
      if (param_1 == 4) {
        FUN_080155ae(uVar1,0x1000);
        FUN_080155ae(uVar2,1);
        FUN_080155b2(uVar1,0x2000);
        return;
      }
    }
    FUN_080155ae(uVar1,0x2000);
    FUN_080155ae(uVar1,0x1000);
    FUN_080155b2(uVar2,1);
    return;
  }
  if (param_2 == 2) {
    if (*(char *)(DAT_0801d3e4 + 0x4a) == -0x5b) {
      FUN_0801dfcc(0);
    }
    switch(param_1) {
    case 0:
    case 2:
      FUN_080155ae(uVar1,0x80);
      FUN_080155b2(uVar2,2);
      FUN_080155ae(uVar1,0x4000);
      return;
    case 1:
    case 3:
      FUN_080155ae(uVar2,2);
      FUN_080155b2(uVar1,0x80);
      FUN_080155ae(uVar1,0x4000);
      return;
    case 4:
      FUN_080155ae(uVar1,0x80);
      FUN_080155ae(uVar2,2);
      FUN_080155b2(uVar1,0x4000);
      return;
    default:
      FUN_080155ae(uVar1,0x80);
      FUN_080155ae(uVar2,2);
      FUN_080155ae(uVar1,0x4000);
      return;
    }
  }
  if (param_2 == 3) {
    FUN_080155ae(DAT_0801d3e8,0x2000);
    FUN_080155ae(uVar1,0x1000);
    FUN_080155ae(uVar2,1);
    return;
  }
  FUN_080155ae(DAT_0801d3e8,0x2000);
  FUN_080155ae(uVar1,0x80);
  FUN_080155ae(uVar2,2);
  FUN_080155ae(uVar1,0x1000);
  FUN_080155ae(uVar2,1);
  FUN_080155ae(uVar1,0x4000);
  return;
}



/* ===== requested 0x8012ff8 ===== */
/* function FUN_0800b1fc entry 0x0800b1fc */

void FUN_0800b1fc(undefined2 param_1)

{
  undefined4 uVar1;
  uint uVar2;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  uint local_14;
  
  *(undefined2 *)(DAT_0800b210 + (uint)*(byte *)(DAT_0800b210 + 0xfa) * 2 + 0x102) = param_1;
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  uVar2 = FUN_08011f68(0xe000,0x20);
  uVar1 = local_20;
  local_28 = *(undefined4 *)(DAT_08013088 + 0x102);
  local_24 = CONCAT22(*(undefined2 *)(DAT_0801308c + 2),*(undefined2 *)(DAT_08013088 + 0x106));
  local_20._3_1_ = SUB41(uVar1,3);
  local_20._0_3_ = CONCAT12(DAT_08013090[1],CONCAT11(*DAT_08013090,DAT_08013090[0x19]));
  local_18 = FUN_0800d4b8(&local_28,0xe);
  local_1c = CONCAT22((undefined2)local_18,(undefined2)local_1c);
  if (uVar2 < 0xfd) {
    FUN_08024210((uVar2 + 1) * 0x10 + 0xe020,&local_28,0x10);
    local_14 = (uint)*(byte *)(DAT_08013094 + (uVar2 & 7));
    FUN_08024210((uVar2 >> 3) + 0xe000,&local_14,1);
  }
  else {
    FUN_08023fbc(0xe000);
    FUN_08024210(0xe020,&local_28,0x10);
  }
  return;
}



/* ===== requested 0x8010710 ===== */
/* no function */

/* ===== requested 0x8026954 ===== */
/* function FUN_080268f8 entry 0x080268f8 */

undefined4 FUN_080268f8(void)

{
  ulonglong uVar1;
  uint uVar2;
  char cVar3;
  undefined4 uVar4;
  undefined8 in_d0;
  
  cVar3 = (~(uint)((ulonglong)in_d0 >> 0x34) & 0x7ff) == 0;
  if (!(bool)cVar3) {
    uVar1 = (ulonglong)DAT_080269c0 >> 0x20;
    uVar4 = (undefined4)DAT_080269c0;
    FUN_0802a57c();
    if (cVar3 == '\0') {
      in_d0 = FUN_0802b96c();
      uVar2 = (uint)((ulonglong)in_d0 >> 0x20);
      cVar3 = (~(uVar2 >> 0x14) & 0x7ff) == 0;
      if ((bool)cVar3) {
        FUN_080041c4(2);
        uVar4 = FUN_08027908();
        return uVar4;
      }
      FUN_0802a57c((int)in_d0,uVar2,uVar4,(int)uVar1,in_d0);
      if (cVar3 != '\0') {
        FUN_080041c4(2);
        uVar4 = FUN_08027928();
        return uVar4;
      }
    }
  }
  return (int)in_d0;
}



/* ===== requested 0x80267b8 ===== */
/* function FUN_0802665c entry 0x0802665c */

float FUN_0802665c(float param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  bool bVar4;
  uint in_fpscr;
  uint uVar5;
  float fVar6;
  float fVar7;
  
  bVar4 = false;
  if (DAT_08026808 < (int)param_1 * 2 + 0x99000000U) {
    if ((~(uint)param_1 & 0x7f800000) == 0) {
      if (param_1 != -INFINITY) {
        return param_1 + param_1;
      }
      return DAT_08026840;
    }
    if ((uint)((int)param_1 * 2) < 0x67000000) {
      return 1.0;
    }
    if (DAT_0802683c < (uint)((int)param_1 << 1)) {
      FUN_080041c4(2);
      if (((uint)param_1 & 0x80000000) != 0) {
        fVar6 = (float)FUN_08027974();
        return fVar6;
      }
      goto LAB_08027964;
    }
    bVar4 = true;
  }
  if (((uint)param_1 & 0x80000000) == 0) {
    fVar6 = 0.5;
  }
  else {
    fVar6 = -0.5;
  }
  uVar5 = (uint)(fVar6 + param_1 * DAT_0802680c);
  fVar6 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
  fVar6 = (param_1 - fVar6 * DAT_08026810) - fVar6 * DAT_08026814;
  uVar2 = uVar5 & 3;
  iVar1 = (int)uVar5 >> 2;
  fVar6 = *(float *)(DAT_08026828 + 0x80266da + uVar2 * 4) +
          *(float *)(DAT_0802682c + 0x80266e6 + uVar2 * 4) *
          (DAT_08026824 + fVar6 * (DAT_08026820 + fVar6 * (DAT_0802681c + fVar6 * DAT_08026818))) *
          fVar6 + *(float *)(DAT_08026830 + 0x80266f6 + uVar2 * 4);
  if (!bVar4) {
    fVar7 = (float)VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
    fVar7 = (float)VectorFloatToUnsigned(DAT_08026838 + fVar7 * DAT_08026834,3);
    return fVar7 * fVar6;
  }
  iVar3 = iVar1 - ((int)uVar5 >> 0x1f) >> 1;
  fVar6 = (float)((iVar1 - iVar3) * 0x800000 + 0x3f800000) *
          (float)(iVar3 * 0x800000 + 0x3f800000) * fVar6;
  if (fVar6 == 0.0) {
    FUN_080041c4(2);
    fVar6 = (float)FUN_08027974();
    return fVar6;
  }
  if (fVar6 != INFINITY) {
    iVar1 = FUN_08026294(fVar6);
    if (iVar1 == 4) {
      FUN_08027974();
    }
    return fVar6;
  }
  FUN_080041c4(2);
LAB_08027964:
  return DAT_08027970 * DAT_08027970;
}



/* ===== requested 0x80266c4 ===== */
/* function FUN_0802665c entry 0x0802665c */

float FUN_0802665c(float param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  bool bVar4;
  uint in_fpscr;
  uint uVar5;
  float fVar6;
  float fVar7;
  
  bVar4 = false;
  if (DAT_08026808 < (int)param_1 * 2 + 0x99000000U) {
    if ((~(uint)param_1 & 0x7f800000) == 0) {
      if (param_1 != -INFINITY) {
        return param_1 + param_1;
      }
      return DAT_08026840;
    }
    if ((uint)((int)param_1 * 2) < 0x67000000) {
      return 1.0;
    }
    if (DAT_0802683c < (uint)((int)param_1 << 1)) {
      FUN_080041c4(2);
      if (((uint)param_1 & 0x80000000) != 0) {
        fVar6 = (float)FUN_08027974();
        return fVar6;
      }
      goto LAB_08027964;
    }
    bVar4 = true;
  }
  if (((uint)param_1 & 0x80000000) == 0) {
    fVar6 = 0.5;
  }
  else {
    fVar6 = -0.5;
  }
  uVar5 = (uint)(fVar6 + param_1 * DAT_0802680c);
  fVar6 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
  fVar6 = (param_1 - fVar6 * DAT_08026810) - fVar6 * DAT_08026814;
  uVar2 = uVar5 & 3;
  iVar1 = (int)uVar5 >> 2;
  fVar6 = *(float *)(DAT_08026828 + 0x80266da + uVar2 * 4) +
          *(float *)(DAT_0802682c + 0x80266e6 + uVar2 * 4) *
          (DAT_08026824 + fVar6 * (DAT_08026820 + fVar6 * (DAT_0802681c + fVar6 * DAT_08026818))) *
          fVar6 + *(float *)(DAT_08026830 + 0x80266f6 + uVar2 * 4);
  if (!bVar4) {
    fVar7 = (float)VectorSignedToFloat(iVar1,(byte)(in_fpscr >> 0x16) & 3);
    fVar7 = (float)VectorFloatToUnsigned(DAT_08026838 + fVar7 * DAT_08026834,3);
    return fVar7 * fVar6;
  }
  iVar3 = iVar1 - ((int)uVar5 >> 0x1f) >> 1;
  fVar6 = (float)((iVar1 - iVar3) * 0x800000 + 0x3f800000) *
          (float)(iVar3 * 0x800000 + 0x3f800000) * fVar6;
  if (fVar6 == 0.0) {
    FUN_080041c4(2);
    fVar6 = (float)FUN_08027974();
    return fVar6;
  }
  if (fVar6 != INFINITY) {
    iVar1 = FUN_08026294(fVar6);
    if (iVar1 == 4) {
      FUN_08027974();
    }
    return fVar6;
  }
  FUN_080041c4(2);
LAB_08027964:
  return DAT_08027970 * DAT_08027970;
}



/* ===== requested 0x8017c40 ===== */
/* function FUN_08017c40 entry 0x08017c40 */

void FUN_08017c40(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = DAT_08017c88;
  if (param_1 != 0) {
    FUN_080155b2(DAT_08017c84,0x40);
    FUN_080155b2(uVar1,8);
    FUN_080155b2(uVar1,8);
    return;
  }
  FUN_080155ae(DAT_08017c84,0x40);
  FUN_080155ae(uVar1,8);
  FUN_080155ae(uVar1,8);
  return;
}



/* ===== requested 0x8016820 ===== */
/* no function */

/* ===== requested 0x801685c ===== */
/* function FUN_0801685c entry 0x0801685c */

uint FUN_0801685c(void)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  
  uVar1 = DAT_08016894;
  FUN_0800623c(DAT_08016894,0,1,7);
  FUN_080062ba(uVar1,1);
  do {
    iVar2 = FUN_080061d8(uVar1,2);
  } while (iVar2 == 0);
  FUN_08006176(uVar1,2);
  uVar3 = FUN_080061d2(uVar1);
  return (uVar3 & 0xfff) >> 4;
}



/* ===== requested 0x801befe ===== */
/* no function */

/* ===== requested 0x8029440 ===== */
/* function FUN_080293e4 entry 0x080293e4 */

void FUN_080293e4(void)

{
  char cVar1;
  int iVar2;
  undefined1 *puVar3;
  undefined4 unaff_r4;
  undefined4 unaff_lr;
  
  FUN_0802962c();
  puVar3 = DAT_08029450;
  *DAT_08029450 = 1;
  puVar3[1] = 0x31;
  puVar3[2] = 5;
  FUN_08029584(3,puVar3);
  FUN_0800d946(0x226);
  FUN_080295d0();
  iVar2 = DAT_08029454;
  FUN_08029550(*(undefined1 *)(DAT_08029458 + (uint)*(byte *)(DAT_08029454 + 0x16)),1,0);
  FUN_08029604(0x3302,0);
  FUN_08029604(0x3103,0x7800);
  *(undefined1 *)(iVar2 + 0x17) = *(undefined1 *)(DAT_0802945c + 0x98);
  FUN_08011bc0();
  iVar2 = DAT_08011b50;
  if (*(byte *)(DAT_08011b4c + 0x43) != 1) {
    if (1 < *(byte *)(DAT_08011b4c + 0x43)) {
      FUN_08029522(0x96,30000,0,1,unaff_r4,unaff_lr);
      FUN_0802953e(*(undefined1 *)(DAT_08011b54 + (uint)*(byte *)(iVar2 + 0x16)));
      FUN_08029604(0x100,*(undefined2 *)(iVar2 + 0x18));
      return;
    }
    FUN_08029604(0x1400,0x1964,0);
    FUN_08029604(0x1401,0x2a30);
    return;
  }
  cVar1 = *(char *)(DAT_08011b50 + 1);
  if (cVar1 == '\0') {
    FUN_08029522(0x99,0x117,0,1,unaff_r4,unaff_lr);
    FUN_08029604(0x3402,9);
    return;
  }
  if (cVar1 != '\x01') {
    FUN_08029522(0x8fc,30000,cVar1,1,unaff_r4,unaff_lr);
    FUN_08029604(0x3402,5);
    return;
  }
  FUN_08029522(0x208,0x6ae,1,1,unaff_r4,unaff_lr);
  FUN_08029604(0x3402,9);
  return;
}


