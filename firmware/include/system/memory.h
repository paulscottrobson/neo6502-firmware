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

#define MEMORY_SIZE  0x10000 // 64k
#define DEFAULT_PORT 0xFF00

extern uint8_t  cpuMemory[];

void MEMInitialiseMemory(void);

#endif

