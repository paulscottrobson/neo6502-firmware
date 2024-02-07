// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      memory.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Rien Matthijsse
//      Date :      20th November 2023
//      Reviewed :  No
//      Purpose :   Memory initialisation code.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "data/kernel_binary.h"                                            		// Contains kernel image.
#include "data/basic_binary.h" 													// NeoBasic

#ifdef PICO
/* _Alignas(MEMORY_SIZE) */ uint8_t cpuMemory[MEMORY_SIZE] = {0};  				// Processor memory, aligned for Pico
uint8_t gfxMemory[GFX_MEMORY_SIZE] = {0}; 										// Graphics memory.
uint8_t graphicsMemory[MAXGRAPHICSMEMORY] = {0};								// RAM used for graphics and console text.
uint8_t consoleMemory[MAXCONSOLEMEMORY] = {0};  								// Console RAM

#else
uint8_t cpuMemory[MEMORY_SIZE] = {0};
uint8_t gfxMemory[GFX_MEMORY_SIZE] = {0};
uint8_t graphicsMemory[MAXGRAPHICSMEMORY] = {0};
uint8_t consoleMemory[MAXCONSOLEMEMORY] = {0};  					
#endif

uint16_t controlPort = DEFAULT_PORT;       										// Control point.

// ***************************************************************************************
//
//                          Load a binary image into ROM space
//
// ***************************************************************************************

static void loadROM(const uint8_t *vROM, uint16_t startAddress, uint16_t romSize) {
	memcpy(cpuMemory+startAddress,vROM,romSize);
}

// ***************************************************************************************
//
//                     Initialise CPU memory (call once on boot up)
//
// ***************************************************************************************

void MEMInitialiseMemory(void) {
	loadROM(kernel_bin,KERNEL_LOAD,KERNEL_SIZE);    							// Load in the kernel
	loadROM(basic_bin,BASIC_LOAD,BASIC_SIZE);  									// Load in BASIC to run by default
	cpuMemory[DEFAULT_PORT] = 0x00;               								// Clear the default command port
}

// ***************************************************************************************
//
//                     			Load BASIC from Flash
//
// ***************************************************************************************

void MEMLoadBasic(void) {
//	loadROM(basic_bin,BASIC_LOAD,BASIC_SIZE);  									// Copy ROM image into memory crashes
	cpuMemory[0x0] = BASIC_LOAD & 0xFF;  										// Start with jmp (0)
	cpuMemory[0x1] = BASIC_LOAD >> 8;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//		16-01-24 	Cleared memory on restart (not loading graphics library could crash)
//
// ***************************************************************************************
