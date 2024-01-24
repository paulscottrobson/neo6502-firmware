// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		memory.h
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		20th November 2023
//		Reviewed :	No
//		Purpose :	Setup for Memory
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _MEMORY_h
#define _MEMORY_h

#define MEMORY_SIZE  0x10000 	// 64k
#define GFX_MEMORY_SIZE 0xC000  // 48k for sprites/tiles/etc.

#define DEFAULT_PORT 0xFF00

extern uint8_t  cpuMemory[];
extern uint8_t gfxMemory[];
extern uint8_t graphicsMemory[];
extern uint8_t consoleMemory[];

extern uint16_t controlPort;
//
//		Access the control port address via this Macro !
//
#define CONTROLPORT 	(controlPort)

void MEMInitialiseMemory(void);
void MEMLoadBasic(void);

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
