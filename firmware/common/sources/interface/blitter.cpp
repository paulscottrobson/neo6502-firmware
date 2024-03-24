// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      blitter.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      24th March 2024
//      Reviewed :  No
//      Purpose :   Blitter code
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//						Convert page/address to a physical address
//
// ***************************************************************************************

static uint8_t *_BLTGetRealAddress(uint8_t page,uint16_t address) {
	int pos;
	uint8_t *ptr = NULL;
	switch(page) {
		case 0x00: 																	// CPU Memory
			ptr = cpuMemory + address;break;
		case 0x80:  																// Video RAM
		case 0x81:
			pos = ((page - 0x80) << 16) | address;  								// Offset in video RAM
			if (pos < 320*240) ptr = graphicsMemory + pos; 							// Position in bitmap RAM
			break;
		case 0x90: 																	// Graphic storage RAM
			if (address < GFX_MEMORY_SIZE) ptr = gfxObjectMemory + address;
			break;
	}
	return ptr;
}

// ***************************************************************************************
//
//								Copy Blitter, very simple
//
// ***************************************************************************************

uint8_t BLTSimpleCopy(uint8_t pageFrom,uint16_t addressFrom, uint8_t pageTo, uint16_t addressTo, uint16_t transferSize) {	
	printf("Blit: %02x:%04x to %02x:%04x bytes %04x\n",pageFrom,pageTo,addressFrom,addressTo,transferSize);
	while (transferSize != 0) {  													// Still copying
		transferSize--;  															// One fewer byte
		uint8_t *src = _BLTGetRealAddress(pageFrom,addressFrom);  					// Copy from here
		uint8_t *dst = _BLTGetRealAddress(pageTo,addressTo);  						// To here.
		if (src == NULL || dst == NULL) return 1;  									// Both legitimate addresses
		*dst = *src;  																// Byte copy
		if (++addressFrom == 0) pageFrom++;  										// Bump addresses.
		if (++addressTo == 0) pageTo++;
	}
	return 0;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
