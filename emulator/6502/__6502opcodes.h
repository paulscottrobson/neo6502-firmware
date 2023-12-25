case 0x00: /* $00 brk */
	Cycles(7);brkCode();break;
case 0x01: /* $01 ora (@1,x) */
	Cycles(7);temp8 = (Fetch()+x) & 0xFF;eac = ReadWord(temp8);sValue = zValue = a = a | Read(eac);break;
case 0x04: /* $04 tsb @1 */
	Cycles(3);eac = Fetch(); trsbCode(eac,1);break;
case 0x05: /* $05 ora @1 */
	Cycles(3);eac = Fetch();sValue = zValue = a = a | Read(eac);break;
case 0x06: /* $06 asl @1 */
	Cycles(5);eac = Fetch(); Write(eac,aslCode(Read(eac)));break;
case 0x07: /* $07 rmb0 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)&((1 << 0)^0xFF);Write(eac,temp8);break;
case 0x08: /* $08 php */
	Cycles(3);Push(constructFlagRegister());break;
case 0x09: /* $09 ora #@1 */
	Cycles(2);sValue = zValue = a = a | Fetch();break;
case 0x0a: /* $0a asl a */
	Cycles(2);a = aslCode(a);break;
case 0x0c: /* $0c tsb @2 */
	Cycles(4);FetchWord();eac = temp16; trsbCode(eac,1);break;
case 0x0d: /* $0d ora @2 */
	Cycles(4);FetchWord();eac = temp16;sValue = zValue = a = a | Read(eac);break;
case 0x0e: /* $0e asl @2 */
	Cycles(6);FetchWord();eac = temp16; Write(eac,aslCode(Read(eac)));break;
case 0x0f: /* $0f bbr0 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 0)) == 0);break;
case 0x10: /* $10 bpl @r */
	Cycles(2);Branch((sValue & 0x80) == 0);break;
case 0x11: /* $11 ora (@1),y */
	Cycles(6);temp8 = Fetch();eac = (ReadWord(temp8)+y) & 0xFFFF;sValue = zValue = a = a | Read(eac);break;
case 0x12: /* $12 ora (@1) */
	Cycles(6);temp8 = Fetch();eac = ReadWord(temp8);sValue = zValue = a = a | Read(eac);break;
case 0x14: /* $14 trb @1 */
	Cycles(3);eac = Fetch(); trsbCode(eac,0);break;
case 0x15: /* $15 ora @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF;sValue = zValue = a = a | Read(eac);break;
case 0x16: /* $16 asl @1,x */
	Cycles(6);eac = (Fetch()+x) & 0xFF; Write(eac,aslCode(Read(eac)));break;
case 0x17: /* $17 rmb1 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)&((1 << 1)^0xFF);Write(eac,temp8);break;
case 0x18: /* $18 clc */
	Cycles(2);carryFlag = 0;break;
case 0x19: /* $19 ora @2,y */
	Cycles(4);FetchWord();eac = (temp16+y) & 0xFFFF;sValue = zValue = a = a | Read(eac);break;
case 0x1a: /* $1a inc */
	Cycles(2);sValue = zValue = a = (a + 1) & 0xFF;break;
case 0x1c: /* $1c trb @2 */
	Cycles(4);FetchWord();eac = temp16; trsbCode(eac,0);break;
case 0x1d: /* $1d ora @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF;sValue = zValue = a = a | Read(eac);break;
case 0x1e: /* $1e asl @2,x */
	Cycles(6);FetchWord();eac = (temp16+x) & 0xFFFF; Write(eac,aslCode(Read(eac)));break;
case 0x1f: /* $1f bbr1 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 1)) == 0);break;
case 0x20: /* $20 jsr @2 */
	Cycles(6);FetchWord();eac = temp16;pc--;Push(pc >> 8);Push(pc & 0xFF);pc = eac;break;
case 0x21: /* $21 and (@1,x) */
	Cycles(7);temp8 = (Fetch()+x) & 0xFF;eac = ReadWord(temp8); a = a & Read(eac) ; sValue = zValue = a;break;
case 0x24: /* $24 bit @1 */
	Cycles(2);eac = Fetch(); bitCode(Read(eac));break;
case 0x25: /* $25 and @1 */
	Cycles(3);eac = Fetch(); a = a & Read(eac) ; sValue = zValue = a;break;
case 0x26: /* $26 rol @1 */
	Cycles(3);eac = Fetch(); Write(eac,rolCode(Read(eac)));break;
case 0x27: /* $27 rmb2 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)&((1 << 2)^0xFF);Write(eac,temp8);break;
case 0x28: /* $28 plp */
	Cycles(4);explodeFlagRegister(Pop());break;
case 0x29: /* $29 and #@1 */
	Cycles(2); a = a & Fetch() ; sValue = zValue = a;break;
case 0x2a: /* $2a rol a */
	Cycles(2);a = rolCode(a);break;
case 0x2c: /* $2c bit @2 */
	Cycles(3);FetchWord();eac = temp16; bitCode(Read(eac));break;
case 0x2d: /* $2d and @2 */
	Cycles(4);FetchWord();eac = temp16; a = a & Read(eac) ; sValue = zValue = a;break;
case 0x2e: /* $2e rol @2 */
	Cycles(4);FetchWord();eac = temp16; Write(eac,rolCode(Read(eac)));break;
case 0x2f: /* $2f bbr2 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 2)) == 0);break;
case 0x30: /* $30 bmi @r */
	Cycles(2);Branch((sValue & 0x80) != 0);break;
case 0x31: /* $31 and (@1),y */
	Cycles(6);temp8 = Fetch();eac = (ReadWord(temp8)+y) & 0xFFFF; a = a & Read(eac) ; sValue = zValue = a;break;
case 0x32: /* $32 and (@1) */
	Cycles(6);temp8 = Fetch();eac = ReadWord(temp8); a = a & Read(eac) ; sValue = zValue = a;break;
case 0x34: /* $34 bit @1,x */
	Cycles(3);eac = (Fetch()+x) & 0xFF; bitCode(Read(eac));break;
case 0x35: /* $35 and @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF; a = a & Read(eac) ; sValue = zValue = a;break;
case 0x36: /* $36 rol @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF; Write(eac,rolCode(Read(eac)));break;
case 0x37: /* $37 rmb3 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)&((1 << 3)^0xFF);Write(eac,temp8);break;
case 0x38: /* $38 sec */
	Cycles(2);carryFlag = 1;break;
case 0x39: /* $39 and @2,y */
	Cycles(4);FetchWord();eac = (temp16+y) & 0xFFFF; a = a & Read(eac) ; sValue = zValue = a;break;
case 0x3a: /* $3a dec */
	Cycles(2);sValue = zValue = a = (a - 1) & 0xFF;break;
case 0x3c: /* $3c bit @2,x */
	Cycles(3);FetchWord();eac = (temp16+x) & 0xFFFF; bitCode(Read(eac));break;
case 0x3d: /* $3d and @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF; a = a & Read(eac) ; sValue = zValue = a;break;
case 0x3e: /* $3e rol @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF; Write(eac,rolCode(Read(eac)));break;
case 0x3f: /* $3f bbr3 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 3)) == 0);break;
case 0x40: /* $40 rti */
	Cycles(6);explodeFlagRegister(Pop());pc = Pop();pc = pc | (((WORD16)Pop()) << 8);break;
case 0x41: /* $41 eor (@1,x) */
	Cycles(7);temp8 = (Fetch()+x) & 0xFF;eac = ReadWord(temp8);sValue = zValue = a = a ^ Read(eac);break;
case 0x45: /* $45 eor @1 */
	Cycles(3);eac = Fetch();sValue = zValue = a = a ^ Read(eac);break;
case 0x46: /* $46 lsr @1 */
	Cycles(3);eac = Fetch(); Write(eac,lsrCode(Read(eac)));break;
case 0x47: /* $47 rmb4 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)&((1 << 4)^0xFF);Write(eac,temp8);break;
case 0x48: /* $48 pha */
	Cycles(3);Push(a);break;
case 0x49: /* $49 eor #@1 */
	Cycles(2);sValue = zValue = a = a ^ Fetch();break;
case 0x4a: /* $4a lsr a */
	Cycles(2);a = lsrCode(a);break;
case 0x4c: /* $4c jmp @2 */
	Cycles(3);FetchWord();eac = temp16;pc = eac;break;
case 0x4d: /* $4d eor @2 */
	Cycles(4);FetchWord();eac = temp16;sValue = zValue = a = a ^ Read(eac);break;
case 0x4e: /* $4e lsr @2 */
	Cycles(4);FetchWord();eac = temp16; Write(eac,lsrCode(Read(eac)));break;
case 0x4f: /* $4f bbr4 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 4)) == 0);break;
case 0x50: /* $50 bvc @r */
	Cycles(2);Branch(overflowFlag == 0);break;
case 0x51: /* $51 eor (@1),y */
	Cycles(6);temp8 = Fetch();eac = (ReadWord(temp8)+y) & 0xFFFF;sValue = zValue = a = a ^ Read(eac);break;
case 0x52: /* $52 eor (@1) */
	Cycles(6);temp8 = Fetch();eac = ReadWord(temp8);sValue = zValue = a = a ^ Read(eac);break;
case 0x55: /* $55 eor @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF;sValue = zValue = a = a ^ Read(eac);break;
case 0x56: /* $56 lsr @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF; Write(eac,lsrCode(Read(eac)));break;
case 0x57: /* $57 rmb5 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)&((1 << 5)^0xFF);Write(eac,temp8);break;
case 0x58: /* $58 cli */
	Cycles(2);interruptDisableFlag = 0;break;
case 0x59: /* $59 eor @2,y */
	Cycles(4);FetchWord();eac = (temp16+y) & 0xFFFF;sValue = zValue = a = a ^ Read(eac);break;
case 0x5a: /* $5a phy */
	Cycles(3);Push(y);break;
case 0x5d: /* $5d eor @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF;sValue = zValue = a = a ^ Read(eac);break;
case 0x5e: /* $5e lsr @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF; Write(eac,lsrCode(Read(eac)));break;
case 0x5f: /* $5f bbr5 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 5)) == 0);break;
case 0x60: /* $60 rts */
	Cycles(6);pc = Pop();pc = pc | (((WORD16)Pop()) << 8);pc++;break;
case 0x61: /* $61 adc (@1,x) */
	Cycles(7);temp8 = (Fetch()+x) & 0xFF;eac = ReadWord(temp8);sValue = zValue = a = add8Bit(a,Read(eac),decimalFlag);break;
case 0x64: /* $64 stz @1 */
	Cycles(3);eac = Fetch();Write(eac,0);break;
case 0x65: /* $65 adc @1 */
	Cycles(3);eac = Fetch();sValue = zValue = a = add8Bit(a,Read(eac),decimalFlag);break;
case 0x66: /* $66 ror @1 */
	Cycles(3);eac = Fetch(); Write(eac,rorCode(Read(eac)));break;
case 0x67: /* $67 rmb6 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)&((1 << 6)^0xFF);Write(eac,temp8);break;
case 0x68: /* $68 pla */
	Cycles(4);a = sValue = zValue = Pop();break;
case 0x69: /* $69 adc #@1 */
	Cycles(2);sValue = zValue = a = add8Bit(a,Fetch(),decimalFlag);break;
case 0x6a: /* $6a ror a */
	Cycles(2);a = rorCode(a);break;
case 0x6c: /* $6c jmp (@2) */
	Cycles(5);FetchWord();eac = ReadWord(temp16);pc = eac;break;
case 0x6d: /* $6d adc @2 */
	Cycles(4);FetchWord();eac = temp16;sValue = zValue = a = add8Bit(a,Read(eac),decimalFlag);break;
case 0x6e: /* $6e ror @2 */
	Cycles(4);FetchWord();eac = temp16; Write(eac,rorCode(Read(eac)));break;
case 0x6f: /* $6f bbr6 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 6)) == 0);break;
case 0x70: /* $70 bvs @r */
	Cycles(2);Branch(overflowFlag != 0);break;
case 0x71: /* $71 adc (@1),y */
	Cycles(6);temp8 = Fetch();eac = (ReadWord(temp8)+y) & 0xFFFF;sValue = zValue = a = add8Bit(a,Read(eac),decimalFlag);break;
case 0x72: /* $72 adc (@1) */
	Cycles(6);temp8 = Fetch();eac = ReadWord(temp8);sValue = zValue = a = add8Bit(a,Read(eac),decimalFlag);break;
case 0x74: /* $74 stz @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF;Write(eac,0);break;
case 0x75: /* $75 adc @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF;sValue = zValue = a = add8Bit(a,Read(eac),decimalFlag);break;
case 0x76: /* $76 ror @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF; Write(eac,rorCode(Read(eac)));break;
case 0x77: /* $77 rmb7 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)&((1 << 7)^0xFF);Write(eac,temp8);break;
case 0x78: /* $78 sei */
	Cycles(2);interruptDisableFlag = 1;break;
case 0x79: /* $79 adc @2,y */
	Cycles(4);FetchWord();eac = (temp16+y) & 0xFFFF;sValue = zValue = a = add8Bit(a,Read(eac),decimalFlag);break;
case 0x7a: /* $7a ply */
	Cycles(4);y = sValue = zValue = Pop();break;
case 0x7c: /* $7c jmp (@2,x) */
	Cycles(5);FetchWord();temp16 = (temp16+x) & 0xFFFF;eac = ReadWord(temp16);pc = eac;break;
case 0x7d: /* $7d adc @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF;sValue = zValue = a = add8Bit(a,Read(eac),decimalFlag);break;
case 0x7e: /* $7e ror @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF; Write(eac,rorCode(Read(eac)));break;
case 0x7f: /* $7f bbr7 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 7)) == 0);break;
case 0x80: /* $80 bra @r */
	Cycles(2);Branch(1);break;
case 0x81: /* $81 sta (@1,x) */
	Cycles(7);temp8 = (Fetch()+x) & 0xFF;eac = ReadWord(temp8);Write(eac,a);break;
case 0x84: /* $84 sty @1 */
	Cycles(3);eac = Fetch();Write(eac,y);break;
case 0x85: /* $85 sta @1 */
	Cycles(3);eac = Fetch();Write(eac,a);break;
case 0x86: /* $86 stx @1 */
	Cycles(3);eac = Fetch();Write(eac,x);break;
case 0x87: /* $87 smb0 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)|(1 << 0);Write(eac,temp8);break;
case 0x88: /* $88 dey */
	Cycles(2);sValue = zValue = y = (y - 1) & 0xFF;break;
case 0x89: /* $89 bit #@1 */
	Cycles(3);bitCode(Fetch());break;
case 0x8a: /* $8a txa */
	Cycles(2);sValue = zValue = a = x;break;
case 0x8c: /* $8c sty @2 */
	Cycles(4);FetchWord();eac = temp16;Write(eac,y);break;
case 0x8d: /* $8d sta @2 */
	Cycles(4);FetchWord();eac = temp16;Write(eac,a);break;
case 0x8e: /* $8e stx @2 */
	Cycles(4);FetchWord();eac = temp16;Write(eac,x);break;
case 0x8f: /* $8f bbs0 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 0)) != 0);break;
case 0x90: /* $90 bcc @r */
	Cycles(2);Branch(carryFlag == 0);break;
case 0x91: /* $91 sta (@1),y */
	Cycles(6);temp8 = Fetch();eac = (ReadWord(temp8)+y) & 0xFFFF;Write(eac,a);break;
case 0x92: /* $92 sta (@1) */
	Cycles(6);temp8 = Fetch();eac = ReadWord(temp8);Write(eac,a);break;
case 0x94: /* $94 sty @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF;Write(eac,y);break;
case 0x95: /* $95 sta @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF;Write(eac,a);break;
case 0x96: /* $96 stx @1,y */
	Cycles(4);eac = (Fetch()+y) & 0xFF;Write(eac,x);break;
case 0x97: /* $97 smb1 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)|(1 << 1);Write(eac,temp8);break;
case 0x98: /* $98 tya */
	Cycles(2);sValue = zValue = a = y;break;
case 0x99: /* $99 sta @2,y */
	Cycles(4);FetchWord();eac = (temp16+y) & 0xFFFF;Write(eac,a);break;
case 0x9a: /* $9a txs */
	Cycles(2);s = x;break;
case 0x9c: /* $9c stz @2 */
	Cycles(4);FetchWord();eac = temp16;Write(eac,0);break;
case 0x9d: /* $9d sta @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF;Write(eac,a);break;
case 0x9e: /* $9e stz @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF;Write(eac,0);break;
case 0x9f: /* $9f bbs1 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 1)) != 0);break;
case 0xa0: /* $a0 ldy #@1 */
	Cycles(2);y = sValue = zValue = Fetch();break;
case 0xa1: /* $a1 lda (@1,x) */
	Cycles(7);temp8 = (Fetch()+x) & 0xFF;eac = ReadWord(temp8);a = sValue = zValue = Read(eac);break;
case 0xa2: /* $a2 ldx #@1 */
	Cycles(2);x = sValue = zValue = Fetch();break;
case 0xa4: /* $a4 ldy @1 */
	Cycles(3);eac = Fetch();y = sValue = zValue = Read(eac);break;
case 0xa5: /* $a5 lda @1 */
	Cycles(3);eac = Fetch();a = sValue = zValue = Read(eac);break;
case 0xa6: /* $a6 ldx @1 */
	Cycles(3);eac = Fetch();x = sValue = zValue = Read(eac);break;
case 0xa7: /* $a7 smb2 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)|(1 << 2);Write(eac,temp8);break;
case 0xa8: /* $a8 tay */
	Cycles(2);sValue = zValue = y = a;break;
case 0xa9: /* $a9 lda #@1 */
	Cycles(2);a = sValue = zValue = Fetch();break;
case 0xaa: /* $aa tax */
	Cycles(2);sValue = zValue = x = a;break;
case 0xac: /* $ac ldy @2 */
	Cycles(4);FetchWord();eac = temp16;y = sValue = zValue = Read(eac);break;
case 0xad: /* $ad lda @2 */
	Cycles(4);FetchWord();eac = temp16;a = sValue = zValue = Read(eac);break;
case 0xae: /* $ae ldx @2 */
	Cycles(4);FetchWord();eac = temp16;x = sValue = zValue = Read(eac);break;
case 0xaf: /* $af bbs2 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 2)) != 0);break;
case 0xb0: /* $b0 bcs @r */
	Cycles(2);Branch(carryFlag != 0);break;
case 0xb1: /* $b1 lda (@1),y */
	Cycles(6);temp8 = Fetch();eac = (ReadWord(temp8)+y) & 0xFFFF;a = sValue = zValue = Read(eac);break;
case 0xb2: /* $b2 lda (@1) */
	Cycles(6);temp8 = Fetch();eac = ReadWord(temp8);a = sValue = zValue = Read(eac);break;
case 0xb4: /* $b4 ldy @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF;y = sValue = zValue = Read(eac);break;
case 0xb5: /* $b5 lda @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF;a = sValue = zValue = Read(eac);break;
case 0xb6: /* $b6 ldx @1,y */
	Cycles(4);eac = (Fetch()+y) & 0xFF;x = sValue = zValue = Read(eac);break;
case 0xb7: /* $b7 smb3 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)|(1 << 3);Write(eac,temp8);break;
case 0xb8: /* $b8 clv */
	Cycles(2);overflowFlag = 0;break;
case 0xb9: /* $b9 lda @2,y */
	Cycles(4);FetchWord();eac = (temp16+y) & 0xFFFF;a = sValue = zValue = Read(eac);break;
case 0xba: /* $ba tsx */
	Cycles(2);sValue = zValue = x = s;break;
case 0xbc: /* $bc ldy @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF;y = sValue = zValue = Read(eac);break;
case 0xbd: /* $bd lda @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF;a = sValue = zValue = Read(eac);break;
case 0xbe: /* $be ldx @2,y */
	Cycles(4);FetchWord();eac = (temp16+y) & 0xFFFF;x = sValue = zValue = Read(eac);break;
case 0xbf: /* $bf bbs3 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 3)) != 0);break;
case 0xc0: /* $c0 cpy #@1 */
	Cycles(2);carryFlag = 1;sValue = zValue = sub8Bit(y,Fetch(),0);break;
case 0xc1: /* $c1 cmp (@1,x) */
	Cycles(7);temp8 = (Fetch()+x) & 0xFF;eac = ReadWord(temp8);carryFlag = 1;sValue = zValue = sub8Bit(a,Read(eac),0);break;
case 0xc4: /* $c4 cpy @1 */
	Cycles(3);eac = Fetch();carryFlag = 1;sValue = zValue = sub8Bit(y,Read(eac),0);break;
case 0xc5: /* $c5 cmp @1 */
	Cycles(3);eac = Fetch();carryFlag = 1;sValue = zValue = sub8Bit(a,Read(eac),0);break;
case 0xc6: /* $c6 dec @1 */
	Cycles(5);eac = Fetch();sValue = zValue = (Read(eac)-1) & 0xFF; Write(eac,sValue);break;
case 0xc7: /* $c7 smb4 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)|(1 << 4);Write(eac,temp8);break;
case 0xc8: /* $c8 iny */
	Cycles(2);sValue = zValue = y = (y + 1) & 0xFF;break;
case 0xc9: /* $c9 cmp #@1 */
	Cycles(2);carryFlag = 1;sValue = zValue = sub8Bit(a,Fetch(),0);break;
case 0xca: /* $ca dex */
	Cycles(2);sValue = zValue = x = (x - 1) & 0xFF;break;
case 0xcc: /* $cc cpy @2 */
	Cycles(4);FetchWord();eac = temp16;carryFlag = 1;sValue = zValue = sub8Bit(y,Read(eac),0);break;
case 0xcd: /* $cd cmp @2 */
	Cycles(4);FetchWord();eac = temp16;carryFlag = 1;sValue = zValue = sub8Bit(a,Read(eac),0);break;
case 0xce: /* $ce dec @2 */
	Cycles(6);FetchWord();eac = temp16;sValue = zValue = (Read(eac)-1) & 0xFF; Write(eac,sValue);break;
case 0xcf: /* $cf bbs4 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 4)) != 0);break;
case 0xd0: /* $d0 bne @r */
	Cycles(2);Branch(zValue != 0);break;
case 0xd1: /* $d1 cmp (@1),y */
	Cycles(6);temp8 = Fetch();eac = (ReadWord(temp8)+y) & 0xFFFF;carryFlag = 1;sValue = zValue = sub8Bit(a,Read(eac),0);break;
case 0xd2: /* $d2 cmp (@1) */
	Cycles(6);temp8 = Fetch();eac = ReadWord(temp8);carryFlag = 1;sValue = zValue = sub8Bit(a,Read(eac),0);break;
case 0xd5: /* $d5 cmp @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF;carryFlag = 1;sValue = zValue = sub8Bit(a,Read(eac),0);break;
case 0xd6: /* $d6 dec @1,x */
	Cycles(6);eac = (Fetch()+x) & 0xFF;sValue = zValue = (Read(eac)-1) & 0xFF; Write(eac,sValue);break;
case 0xd7: /* $d7 smb5 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)|(1 << 5);Write(eac,temp8);break;
case 0xd8: /* $d8 cld */
	Cycles(2);decimalFlag = 0;break;
case 0xd9: /* $d9 cmp @2,y */
	Cycles(4);FetchWord();eac = (temp16+y) & 0xFFFF;carryFlag = 1;sValue = zValue = sub8Bit(a,Read(eac),0);break;
case 0xda: /* $da phx */
	Cycles(3);Push(x);break;
case 0xdd: /* $dd cmp @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF;carryFlag = 1;sValue = zValue = sub8Bit(a,Read(eac),0);break;
case 0xde: /* $de dec @2,x */
	Cycles(6);FetchWord();eac = (temp16+x) & 0xFFFF;sValue = zValue = (Read(eac)-1) & 0xFF; Write(eac,sValue);break;
case 0xdf: /* $df bbs5 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 5)) != 0);break;
case 0xe0: /* $e0 cpx #@1 */
	Cycles(2);carryFlag = 1;sValue = zValue = sub8Bit(x,Fetch(),0);break;
case 0xe1: /* $e1 sbc (@1,x) */
	Cycles(7);temp8 = (Fetch()+x) & 0xFF;eac = ReadWord(temp8);sValue = zValue = a = sub8Bit(a,Read(eac),decimalFlag);break;
case 0xe4: /* $e4 cpx @1 */
	Cycles(3);eac = Fetch();carryFlag = 1;sValue = zValue = sub8Bit(x,Read(eac),0);break;
case 0xe5: /* $e5 sbc @1 */
	Cycles(3);eac = Fetch();sValue = zValue = a = sub8Bit(a,Read(eac),decimalFlag);break;
case 0xe6: /* $e6 inc @1 */
	Cycles(5);eac = Fetch();sValue = zValue = (Read(eac)+1) & 0xFF; Write(eac, sValue);break;
case 0xe7: /* $e7 smb6 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)|(1 << 6);Write(eac,temp8);break;
case 0xe8: /* $e8 inx */
	Cycles(2);sValue = zValue = x = (x + 1) & 0xFF;break;
case 0xe9: /* $e9 sbc #@1 */
	Cycles(2);sValue = zValue = a = sub8Bit(a,Fetch(),decimalFlag);break;
case 0xea: /* $ea nop */
	Cycles(2);{};break;
case 0xec: /* $ec cpx @2 */
	Cycles(4);FetchWord();eac = temp16;carryFlag = 1;sValue = zValue = sub8Bit(x,Read(eac),0);break;
case 0xed: /* $ed sbc @2 */
	Cycles(4);FetchWord();eac = temp16;sValue = zValue = a = sub8Bit(a,Read(eac),decimalFlag);break;
case 0xee: /* $ee inc @2 */
	Cycles(6);FetchWord();eac = temp16;sValue = zValue = (Read(eac)+1) & 0xFF; Write(eac, sValue);break;
case 0xef: /* $ef bbs6 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 6)) != 0);break;
case 0xf0: /* $f0 beq @r */
	Cycles(2);Branch(zValue == 0);break;
case 0xf1: /* $f1 sbc (@1),y */
	Cycles(6);temp8 = Fetch();eac = (ReadWord(temp8)+y) & 0xFFFF;sValue = zValue = a = sub8Bit(a,Read(eac),decimalFlag);break;
case 0xf2: /* $f2 sbc (@1) */
	Cycles(6);temp8 = Fetch();eac = ReadWord(temp8);sValue = zValue = a = sub8Bit(a,Read(eac),decimalFlag);break;
case 0xf5: /* $f5 sbc @1,x */
	Cycles(4);eac = (Fetch()+x) & 0xFF;sValue = zValue = a = sub8Bit(a,Read(eac),decimalFlag);break;
case 0xf6: /* $f6 inc @1,x */
	Cycles(6);eac = (Fetch()+x) & 0xFF;sValue = zValue = (Read(eac)+1) & 0xFF; Write(eac, sValue);break;
case 0xf7: /* $f7 smb7 @1 */
	Cycles(3);eac = Fetch();temp8 = Read(eac)|(1 << 7);Write(eac,temp8);break;
case 0xf8: /* $f8 sed */
	Cycles(2);decimalFlag = 1;break;
case 0xf9: /* $f9 sbc @2,y */
	Cycles(4);FetchWord();eac = (temp16+y) & 0xFFFF;sValue = zValue = a = sub8Bit(a,Read(eac),decimalFlag);break;
case 0xfa: /* $fa plx */
	Cycles(4);x = sValue = zValue = Pop();break;
case 0xfc: /* $fc dbg @2 */
	Cycles(6);FetchWord();eac = temp16;showDebug(eac);break;
case 0xfd: /* $fd sbc @2,x */
	Cycles(4);FetchWord();eac = (temp16+x) & 0xFFFF;sValue = zValue = a = sub8Bit(a,Read(eac),decimalFlag);break;
case 0xfe: /* $fe inc @2,x */
	Cycles(6);FetchWord();eac = (temp16+x) & 0xFFFF;sValue = zValue = (Read(eac)+1) & 0xFF; Write(eac, sValue);break;
case 0xff: /* $ff bbs7 @1,@r */
	Cycles(5);eac = Fetch();Branch((Read(eac) & (1 << 7)) != 0);break;
