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
//
//														CPU / Memory
//
// *******************************************************************************************************************************

LONG32 cycles;																		// Cycle Count.
static int argumentCount;
static char **argumentList;
static bool useDebuggerKeys = false;  												// Use the debugger keys.
static bool traceMode = false;														// Dump each CPU instruction to stdout.

WORD16 CPUGetPC(void) {
	return CPUGetPC65();
}

// *******************************************************************************************************************************
//
//											   	Read and Write Functions
//
// *******************************************************************************************************************************

BYTE8 *CPUAccessMemory(void) {
	return cpuMemory;
}

BYTE8 Read(WORD16 address) {
	return cpuMemory[address];
}

void _Write(WORD16 address,BYTE8 data) { 
	 cpuMemory[address] = data;			
	 if (address == CONTROLPORT) {
	 	DSPHandler(cpuMemory+address,cpuMemory);
	 }
}

// *******************************************************************************************************************************
//
//													Remember Arguments
//
// *******************************************************************************************************************************

void CPUSaveArguments(int argc,char *argv[]) {
	argumentCount = argc;
	argumentList = argv;
}

// *******************************************************************************************************************************
//
//														Reset the CPU
//
// *******************************************************************************************************************************

//#include "binary.h"

void CPUReset(void) {
	char command[128];

	for (int i = 1;i < argumentCount;i++) { 										// Look for loads.
		strcpy(command,argumentList[i]);  											// Copy command
		printf("[%s]\n",command);
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
				cpuMemory[0xFFFC] = address & 0xFF;  								// 6502 run.
				cpuMemory[0xFFFD] = address >> 8;
			} else {
				p = cpuMemory+address;				 								// Load here.	
				if (address == 0xFFFF) p = gfxObjectMemory;  						// Load to graphics memory
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
			if (strcmp(command,"keys") == 0) { 										// Keys work properly.
				useDebuggerKeys = true;
			}
			if (strncmp(command,"path:",5) == 0) {  								// Set storage path
				HWSetDefaultPath(command+5);
			}
			if (strcmp(command,"trace") == 0) { 									// Dump every CPU instruction to stdout.
				traceMode = true;
			}
		}
	}
	HWReset();																		// Reset Hardware
	CPUReset6502();
}

// *******************************************************************************************************************************
//
//							When non-zero disables the debugger keys, requiring control
//
// *******************************************************************************************************************************

int CPUUseDebugKeys(void) {
	return useDebuggerKeys ? 1 : 0;
}

// *******************************************************************************************************************************
//
//												Execute a single instruction
//
// *******************************************************************************************************************************

BYTE8 CPUExecuteInstruction(void) {
	BYTE8 forceSync = 0;

	if (CPUGetPC() == 0xFFFF) {
		printf("Hit CPU $FFFF - exiting emulator\n");
		CPUExit();
		return FRAME_RATE;
	}

    if (traceMode) {
		char mem[10]; // "XX XX XX \0"
		char dasm[32];
		DBGXDumpMem(CPUGetPC(), DBGXInstructionSize65(CPUGetPC()), mem);
		DBGXDasm65(CPUGetPC(), dasm);
		printf("%04x  %-10s %s\n", CPUGetPC(), mem, dasm);
	}

	forceSync = CPUExecute6502();

	int cycleMax = CYCLES_PER_FRAME; 	
	if (cycles < cycleMax && forceSync == 0) return 0;								// Not completed a frame.
	cycles = 0;																		// Reset cycle counter.
	HWSync();																		// Update any hardware
	return FRAME_RATE;																// Return frame rate.
}

// *******************************************************************************************************************************
//
//												Read/Write Memory
//
// *******************************************************************************************************************************

BYTE8 CPUReadMemory(WORD16 address) {
	return Read(address);
}

void CPUWriteMemory(WORD16 address,BYTE8 data) {
	Write(address,data);
}

#include "gfx.h"

// *******************************************************************************************************************************
//
//		Execute chunk of code, to either of two break points or frame-out, return non-zero frame rate on frame, breakpoint 0
//
// *******************************************************************************************************************************

BYTE8 CPUExecute(WORD16 breakPoint1,WORD16 breakPoint2) { 
	BYTE8 next;
	BYTE8 brk = 0x03;
	do {
		BYTE8 r = CPUExecuteInstruction();											// Execute an instruction
		if (r != 0) return r; 														// Frame out.
		next = CPUReadMemory(CPUGetPC());
	} while (CPUGetPC() != breakPoint1 && CPUGetPC() != breakPoint2 && next!=brk);	// Stop on breakpoint or UNOP, which is now break debugger
	return 0; 
}

// *******************************************************************************************************************************
//
//									Return address of breakpoint for step-over, or 0 if N/A
//
// *******************************************************************************************************************************

WORD16 CPUGetStepOverBreakpoint(void) {
	BYTE8 opcode = CPUReadMemory(CPUGetPC());										// Current opcode.
	int offset = CPUGetStep65(opcode); 									 			// Get offset
	if (offset != 0) offset = (CPUGetPC()+offset) & 0xFFFF;							// Step over Subroutines
	return offset;																	// Do a normal single step
}

// *******************************************************************************************************************************make //
//												Called at exit, dumps CPU memory
//
// *******************************************************************************************************************************

void CPUEndRun(void) {
	FILE *f = fopen("memory.dump","wb");
	fwrite(cpuMemory,1,MEMSIZE,f);
	fclose(f);	
}

void CPUExit(void) {	
	printf("Exiting.\n");
	GFXExit();
}



