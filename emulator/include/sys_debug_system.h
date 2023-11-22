// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		sys_debug_system.h
//		Purpose:	Debugger Code (System Dependent) Header
//		Created:	22nd November 2023
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#include "sys_processor.h"

#ifndef _DEBUG_SYS_H
#define _DEBUG_SYS_H
#include "sys_processor.h"
#include "hardware.h"

#define WIN_TITLE 		"Neo6502 Emulator"											// Initial Window stuff
#define WIN_WIDTH		(42*8*4)
#define WIN_HEIGHT		(32*8*4)
#define WIN_BACKCOLOUR	0x004

// *******************************************************************************************************************************
//							These functions need to be implemented by the dependent debugger.
// *******************************************************************************************************************************

#define DEBUG_ARGUMENTS(ac,av) { CPUSaveArguments(ac,av); }

#define DEBUG_CPURENDER(x) 	DBGXRender(x,0)											// Render the debugging display
#define DEBUG_VDURENDER(x)	DBGXRender(x,1)											// Render the game display etc.

#define DEBUG_RESET() 		CPUReset()												// Reset the CPU / Hardware.
#define DEBUG_HOMEPC()		((CPUGetStatus()->pc) & 0xFFFF) 						// Get PC Home Address (e.g. current PCTR value)

#define DEBUG_SINGLESTEP()	CPUExecuteInstruction()									// Execute a single instruction, return 0 or Frame rate on frame end.
#define DEBUG_RUN(b1,b2) 	CPUExecute(b1,b2) 										// Run a frame or to breakpoint, returns -1 if breakpoint
#define DEBUG_GETOVERBREAK() CPUGetStepOverBreakpoint()								// Where would we break to step over here. (0 == single step)

#define DEBUG_RAMSTART 		(0x0)													// Initial RAM address for debugger.
#define DEBUG_SHIFT(d,v)	((((d) << 4) | v) & 0xFFFF)								// Shifting into displayed address.

#define DEBUG_KEYMAP(k,r)	k

void DBGXRender(int *address,int isRunMode);										// Render the debugger screen.
BYTE8 DRVGFXHandler(BYTE8 key,BYTE8 isRunMode);
BYTE8 *DBGXGetVideoRAM(void);

#endif
