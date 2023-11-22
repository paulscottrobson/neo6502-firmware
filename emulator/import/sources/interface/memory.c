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
#include "data/binary.h"                                                  		// Contains kernel image.

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
	cpuMemory[DEFAULT_PORT] = 0x00;               								// Clear the default command port
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
