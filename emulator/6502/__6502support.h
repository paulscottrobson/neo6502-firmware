static BYTE8 temp8;
static WORD16 eac,temp16;
static void bitCode(BYTE8 n) {
 zValue = (n & a);
 sValue = n & 0x80;
 overflowFlag = (n & 0x40) ? 1 : 0;
}
static void trsbCode(WORD16 eac,BYTE8 set) {
 BYTE8 n = Read(eac);
 zValue = (n & a);
 n = set ? (n | a) : (n & (a^0xFF));
 Write(eac,n);
}
static BYTE8 add8Bit(BYTE8 n1,BYTE8 n2,BYTE8 isDecimalMode) {
 WORD16 result;
  BYTE8 r,t;
  if (isDecimalMode) {
       r = (n1 & 0xf)+(n2 & 0xf)+carryFlag;
        if (r > 9) r += 6;
        t = (n1 >> 4)+(n2 >> 4)+ (r >= 0x10 ? 1 : 0);
        if (t > 9) t += 6;
        result = (r & 0xF) | (t << 4);
        carryFlag = (t >= 0x10 ? 1 : 0);
    } else {
        result = n1 + n2 + carryFlag;
        overflowFlag = ((n1 & 0x80) == (n2 & 0x80) && (n1 & 0x80) != (result & 0x80)) ? 1 : 0;
        carryFlag = (result >> 8) & 1;
    }
  return result & 0xFF;
}
static BYTE8 sub8Bit(BYTE8 n1,BYTE8 n2,BYTE8 isDecimalMode) {
  WORD16 result;
  BYTE8 r,t;
  if (isDecimalMode) {
   r = (n1 & 0xf) - (n2 & 0xf) - (carryFlag ^ 1);
        if (r & 0x10) r -= 6;
   t = (n1 >> 4) - (n2 >> 4) - ((r & 0x10)>>4);
        if (t & 0x10) t -= 6;
   result = (r & 0xF) | (t << 4);
   carryFlag = (t > 15) ? 0 : 1;
    } else {
        result = n1 + (n2 ^ 0xFF) + carryFlag;
   carryFlag = (result >> 8) & 1;
   overflowFlag = ((n1 & 0x80) != (n2 & 0x80) && (n1 & 0x80) != (result & 0x80)) ? 1 : 0;
  }
  return result & 0xFF;
}
static BYTE8 aslCode(BYTE8 n) {
 carryFlag = (n >> 7);
 n = sValue = zValue = (n << 1) & 0xFF;
 return n;
}
static BYTE8 lsrCode(BYTE8 n) {
 carryFlag = n & 1;
 n = sValue = zValue = (n >> 1) & 0x7F;
 return n;
}
static BYTE8 rolCode(BYTE8 n) {
 BYTE8 newCarry = (n >> 7) & 1;
 n = sValue = zValue = ((n << 1) & 0xFF) | carryFlag;
 carryFlag = newCarry;
 return n;
}
static BYTE8 rorCode(BYTE8 n) {
 BYTE8 newCarry = n & 1;
 n = sValue = zValue = (n >> 1) | (carryFlag << 7);
 carryFlag = newCarry;
 return n;
}
static void Branch(BYTE8 test) {
 temp8 = Fetch();
 if (test) {
  if (temp8 & 0x80) {
   pc = (pc+temp8-256) & 0xFFFF;
  } else {
   pc = (pc+temp8) & 0xFFFF;
  }
 }
}
static void Push(BYTE8 v) {
 Write(0x100+s,v);
 s = (s - 1) & 0xFF;
}
static BYTE8 Pop(void) {
 s = (s + 1) & 0xFF;
 return Read(0x100+s);
}
static void explodeFlagRegister(BYTE8 f) {
 carryFlag = f & 1;
 zValue = (f & 2) ? 0 : -1;
 interruptDisableFlag = (f >> 2) & 1;
 decimalFlag = (f >> 3) & 1;
 breakFlag = (f >> 4) & 1;
 overflowFlag = (f >> 6) & 1;
 sValue = f & 0x80;
}
static BYTE8 constructFlagRegister(void) {
 BYTE8 f = 0x20 | carryFlag | (interruptDisableFlag << 2) |
     (decimalFlag << 3) | (breakFlag << 4) | (overflowFlag << 6);
 if (zValue == 0) f |= 0x02;
 if (sValue & 0x80) f |= 0x80;
 return f;
}
static void showDebug(WORD16 a) {
 fprintf(stdout,"DEBUG:[PC %04x] ",pc);
 while (CPUReadMemory(a) != 0) {
  fprintf(stdout,"%c",CPUReadMemory(a));
  a++;
 }
 fprintf(stdout,"\n");
}
static void executeInterrupt(WORD16 vector,BYTE8 setBreakFlag) {
 BYTE8 oldBreakFlag = breakFlag;
 Push(pc >> 8);Push(pc & 0xFF);
 if (setBreakFlag) breakFlag = 1;
 Push(constructFlagRegister());
 breakFlag = oldBreakFlag;
 pc = ReadWord(vector);
 interruptDisableFlag = 1;
}
static void brkCode(void) {
 pc++;
 executeInterrupt(0xFFFE,0);
}
static void nmiCode(void) {
 executeInterrupt(0xFFFA,1);
}
static void irqCode(void) {
 if (interruptDisableFlag == 0) executeInterrupt(0xFFFE,1);
}
static void resetProcessor(void) {
 interruptDisableFlag = 1;
 carryFlag &= 1;overflowFlag &= 1;
 breakFlag &= 1;decimalFlag &= 1;
 pc = ReadWord(0xFFFC);
}