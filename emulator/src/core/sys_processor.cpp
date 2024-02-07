// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		sys_processor.c
//		Purpose:	Processor Emulation.
//		Created:	22nd November 2023
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>
#include "sys_processor.h"
#include "sys_debug_system.h"
#include "hardware.h"
#include "common.h"

// *******************************************************************************************************************************
//														   Timing
// *******************************************************************************************************************************

#define CYCLE_RATE 		(62*1024*1024/10)											// Cycles per second (6.25Mhz)
#define FRAME_RATE		(50)														// Frames per second (50 arbitrary)
#define CYCLES_PER_FRAME (CYCLE_RATE / FRAME_RATE)									// Cycles per frame

// *******************************************************************************************************************************
//														CPU / Memory
// *******************************************************************************************************************************

static BYTE8 a,x,y,s;																// 6502 A,X,Y and Stack registers
static BYTE8 carryFlag,interruptDisableFlag,breakFlag,								// Values representing status reg
			 decimalFlag,overflowFlag,sValue,zValue;
static WORD16 pc;																	// Program Counter.
static int argumentCount;
static char **argumentList;
static LONG32 cycles;																// Cycle Count.
static int inFastMode = 0; 															// Fast mode flag (speeds up unit testing)

// *******************************************************************************************************************************
//											 Memory and I/O read and write macros.
// *******************************************************************************************************************************

#define Read(a) 	_Read(a)														// Basic Read
#define Write(a,d)	_Write(a,d)														// Basic Write
#define ReadWord(a) (Read(a) | ((Read((a)+1) << 8)))								// Read 16 bit, Basic
#define Cycles(n) 	cycles += (n)													// Bump Cycles
#define Fetch() 	_Read(pc++)														// Fetch byte
#define FetchWord()	{ temp16 = Fetch();temp16 |= (Fetch() << 8); }					// Fetch word

static inline BYTE8 _Read(WORD16 address);											// Need to be forward defined as 
static inline void _Write(WORD16 address,BYTE8 data);								// used in support functions.

#include "6502/__6502support.h"

// *******************************************************************************************************************************
//											   Read and Write Inline Functions
// *******************************************************************************************************************************

BYTE8 *CPUAccessMemory(void) {
	return cpuMemory;
}

static inline BYTE8 _Read(WORD16 address) {
	return cpuMemory[address];
}

static inline void _Write(WORD16 address,BYTE8 data) { 
	 cpuMemory[address] = data;			
	 if (address == CONTROLPORT) {
	 	DSPHandler(cpuMemory+address,cpuMemory);
	 }
}

// *******************************************************************************************************************************
//													Remember Arguments
// *******************************************************************************************************************************

void CPUSaveArguments(int argc,char *argv[]) {
	argumentCount = argc;
	argumentList = argv;
}

// *******************************************************************************************************************************
//														Reset the CPU
// *******************************************************************************************************************************

//#include "binary.h"

void CPUReset(void) {
	char command[128];
	HWReset();																		// Reset Hardware
	for (int i = 1;i < argumentCount;i++) { 										// Look for loads.
		strcpy(command,argumentList[i]);  											// Copy command
		char *pos = strchr(command,'@'); 											// Look for splitting @
		if (pos != NULL) {
			int ch,address;
			unsigned char *p;
			*pos++ = '\0'; 															// Split it
			if (strcmp(pos,"page") == 0) { 											// Load to page.
				address = cpuMemory[0x820] + (cpuMemory[0x821] << 8);
			} else {
				if (sscanf(pos,"%x",&address) != 1)  								// Hex -> Decimal
							exit(fprintf(stderr,"Bad format %s",pos));
			}
			if (strcmp(command,"run") == 0) {  										// Arbitrary run address run@x
				printf("Run machine code from $%x\n",address);
				cpuMemory[0xFFFC] = address & 0xFF;
				cpuMemory[0xFFFD] = address >> 8;
			} else {
				p = cpuMemory+address;				 								// Load here.	
				if (address == 0xFFFF) p = gfxMemory;  								// Load to graphics memory
				printf("Load %s to %x\n",command,address);
				FILE *f = fopen(command,"rb");  									// Read file in and copy to RAM.
				if (f == NULL) exit(fprintf(stderr,"Bad file %s",command));
				while (ch = fgetc(f),ch >= 0) {
					*p++ = ch;
					address = (address+1) & 0xFFFF;
				}
				fclose(f);
			}
		} else {			
			if (strcmp(command,"cold") == 0) { 										// Cold boots from $800
				printf("Cold boot $800\n");
				cpuMemory[0xFFFC] = 0;cpuMemory[0xFFFD] = 8;
			}
			if (strcmp(command,"warm") == 0) { 										// Warm boots from $803
				printf("Warm boot $803\n");
				cpuMemory[0xFFFC] = 3;cpuMemory[0xFFFD] = 8;
			}
			if (strcmp(command,"exec") == 0) { 										// Warm boots from $806
				printf("Warm boot $806\n");
				cpuMemory[0xFFFC] = 6;cpuMemory[0xFFFD] = 8;
			}
		}
	}
	resetProcessor();																// Reset CPU		
}

// *******************************************************************************************************************************
//													  Invoke IRQ
// *******************************************************************************************************************************

void CPUInterruptMaskable(void) {
	irqCode();
}

// *******************************************************************************************************************************
//												Execute a single instruction
// *******************************************************************************************************************************

BYTE8 CPUExecuteInstruction(void) {
	if (pc == 0xFFFF) {
		printf("Hit CPU $FFFF - exiting emulator\n");
		CPUExit();
		return FRAME_RATE;
	}
	BYTE8 opcode = Fetch();															// Fetch opcode.
	switch(opcode) {																// Execute it.
		#include "6502/__6502opcodes.h"
	}
	int cycleMax = inFastMode ? CYCLES_PER_FRAME*10:CYCLES_PER_FRAME; 		
	if (cycles < cycleMax) return 0;												// Not completed a frame.
	cycles = 0;																		// Reset cycle counter.
	HWSync();																		// Update any hardware
	return FRAME_RATE;																// Return frame rate.
}

// *******************************************************************************************************************************
//												Read/Write Memory
// *******************************************************************************************************************************

BYTE8 CPUReadMemory(WORD16 address) {
	return Read(address);
}

void CPUWriteMemory(WORD16 address,BYTE8 data) {
	Write(address,data);
}


#include "gfx.h"

// *******************************************************************************************************************************
//		Execute chunk of code, to either of two break points or frame-out, return non-zero frame rate on frame, breakpoint 0
// *******************************************************************************************************************************

BYTE8 CPUExecute(WORD16 breakPoint1,WORD16 breakPoint2) { 
	BYTE8 next;
	do {
		BYTE8 r = CPUExecuteInstruction();											// Execute an instruction
		if (r != 0) return r; 														// Frame out.
		next = CPUReadMemory(pc);
	} while (pc != breakPoint1 && pc != breakPoint2 && next != 0x03);				// Stop on breakpoint or UNOP, which is now break debugger
	return 0; 
}

// *******************************************************************************************************************************
//									Return address of breakpoint for step-over, or 0 if N/A
// *******************************************************************************************************************************

WORD16 CPUGetStepOverBreakpoint(void) {
	BYTE8 opcode = CPUReadMemory(pc);												// Current opcode.
	if (opcode == 0x20) return (pc+3) & 0xFFFF;										// Step over JSR.
	return 0;																		// Do a normal single step
}

void CPUEndRun(void) {
	FILE *f = fopen("memory.dump","wb");
	fwrite(cpuMemory,1,MEMSIZE,f);
	fclose(f);	
}

void CPUExit(void) {	
	printf("Exiting.\n");
	GFXExit();
}


// *******************************************************************************************************************************
//											Retrieve a snapshot of the processor
// *******************************************************************************************************************************

static CPUSTATUS st;																	// Status area

CPUSTATUS *CPUGetStatus(void) {
	st.a = a;st.x = x;st.y = y;st.sp = s;st.pc = pc;
	st.carry = carryFlag;st.interruptDisable = interruptDisableFlag;st.zero = (zValue == 0);
	st.decimal = decimalFlag;st.brk = breakFlag;st.overflow = overflowFlag;
	st.sign = (sValue & 0x80) != 0;st.status = constructFlagRegister();
	st.cycles = cycles;
	return &st;
}

