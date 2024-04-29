// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		sys_processor.h
//		Purpose:	Processor Emulation (header)
//		Created:	22nd November 2023
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#define MEMSIZE 		(0x10000)													// 64kb of Memory.

typedef unsigned short WORD16;														// 8 and 16 bit types.
typedef unsigned char  BYTE8;
typedef unsigned int   LONG32;														// 32 bit type.

#define DEFAULT_BUS_VALUE (0xFF)													// What's on the bus if it's not memory.

#define AKEY_BACKSPACE	(0x5F)														// Apple Backspace

// *******************************************************************************************************************************
//
//														   Timing
//
// *******************************************************************************************************************************

#define CYCLE_RATE 		(62*1024*1024/10)											// Cycles per second (6.25Mhz)
#define FRAME_RATE		(60)														// Frames per second (60 same as DVI)
#define CYCLES_PER_FRAME (CYCLE_RATE / FRAME_RATE)									// Cycles per frame

extern LONG32 cycles;

// *******************************************************************************************************************************
//
//														Prototypes
//
// *******************************************************************************************************************************

void CPUReset(void);
BYTE8 CPUExecuteInstruction(void);
BYTE8 CPUWriteKeyboard(BYTE8 pattern);
BYTE8 CPUReadMemory(WORD16 address);
BYTE8 *CPUAccessMemory(void);
int CPUUseDebugKeys(void);

#define Read(a) 	_Read(a)														// Basic Read
#define Write(a,d)	_Write(a,d)														// Basic Write
#define ReadWord(a) (Read(a) | ((Read((a)+1) << 8)))								// Read 16 bit, Basic
#define Cycles(n) 	cycles += (n)													// Bump Cycles
#define Fetch() 	_Read(pc++)														// Fetch byte
#define FetchWord()	{ temp16 = Fetch();temp16 |= (Fetch() << 8); }					// Fetch word

BYTE8 _Read(WORD16 address);														// Need to be forward defined as 
void _Write(WORD16 address,BYTE8 data);												// used in support functions.
WORD16 CPUGetPC(void);

BYTE8 CPUExecute(WORD16 breakPoint1,WORD16 breakPoint2);
WORD16 CPUGetStepOverBreakpoint(void);
void CPUWriteMemory(WORD16 address,BYTE8 data);
void CPUEndRun(void);
void CPULoadBinary(char *fileName);
void CPUExit(void);
void CPUSaveArguments(int argc,char *argv[]);

// *******************************************************************************************************************************
//
//														6502 Prototypes
//
// *******************************************************************************************************************************

typedef struct __CPUSTATUS65 {
	int a,x,y,sp,pc;
	int carry,zero,sign,interruptDisable,decimal,brk,overflow,status;
	int cycles;		
} CPUSTATUS65;

BYTE8 CPUExecute6502(void);
void CPUReset6502(void);
CPUSTATUS65 *CPUGetStatus65(void);
WORD16 CPUGetPC65(void);
int CPUGetStep65(BYTE8 opcode);

// *******************************************************************************************************************************
//
//													  Sweet 16 Prototypes
//
// *******************************************************************************************************************************

BYTE8 CPUExecute16(void);
void CPUReset16(void);
WORD16 CPUGetPC16(void);
int CPUGetStep16(BYTE8 opcode);

#endif
