// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      memory.c
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
_Alignas(MEMORY_SIZE) uint8_t cpuMemory[MEMORY_SIZE];  							// Processor memory, aligned for Pico
#else
uint8_t cpuMemory[MEMORY_SIZE];
#endif

uint16_t controlPort = DEFAULT_PORT;       										// Control point.

// ***************************************************************************************
//
//                          Load a binary image into ROM space
//
// ***************************************************************************************

static void loadROM(const uint8_t *vROM, uint16_t startAddress, uint16_t romSize) {
	for (uint16_t i = 0; i < romSize; i++) {
		cpuMemory[i + startAddress] = vROM[i];
	}
}

// ***************************************************************************************
//
//                     Initialise CPU memory (call once on boot up)
//
// ***************************************************************************************

void MEMInitialiseMemory(void) {
	loadROM(kernel_bin,KERNEL_LOAD,KERNEL_SIZE);    							// Load in the kernel
	loadROM(basic_bin,BASIC_LOAD,BASIC_SIZE);  									// **** Temp fix ****
	cpuMemory[DEFAULT_PORT] = 0x00;               								// Clear the default command port
}

// ***************************************************************************************
//
//                     			Load BASIC from Flash
//
// ***************************************************************************************

void MEMLoadBasic(void) {
	//loadROM(basic_bin,BASIC_LOAD,BASIC_SIZE);  								// Copy ROM image into memory crashes
	cpuMemory[0x0] = BASIC_LOAD & 0xFF;  										// Start with jmp (0)
	cpuMemory[0x1] = BASIC_LOAD >> 8;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
