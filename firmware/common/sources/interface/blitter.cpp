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
	printf("Blit: %02x:%04x to %02x:%04x bytes %04x\n",pageFrom,addressFrom,pageTo,addressTo,transferSize);
	if (transferSize == 0) return 0;
	uint8_t *src = _BLTGetRealAddress(pageFrom,addressFrom);  						// Copy from here
	uint8_t *dst = _BLTGetRealAddress(pageTo,addressTo);  							// To here.
	if (src == NULL || dst == NULL) return 1;  										// Start both legitimate addresses
	if (_BLTGetRealAddress(pageFrom,addressFrom+transferSize-1) == NULL) return 1; 	// Check end both legitimate addresses
	if (_BLTGetRealAddress(pageTo,addressTo+transferSize-1) == NULL) return 1;
	memmove(dst,src,transferSize); 													// Copy it.

	return 0;
}

// ***************************************************************************************
//
//						Add constant to a physical address
//
// ***************************************************************************************

void _BLTAddAddress(struct BlitterArea *ba,uint32_t add) {
	uint32_t a = ba->address + add;  												// Work out the result
	ba->address = a & 0xFFFF; 														// Put address part back
	if (a & 0x10000) ba->page++;  													// If carry out, go to next page.
}

// ***************************************************************************************
//
//							 Load a blitter object structure
//
// ***************************************************************************************

void _BLTLoadBlitterAreaObject(uint16_t addr,struct BlitterArea *b) {
	b->address = cpuMemory[addr]+(cpuMemory[addr+1] << 8);
	b->page = cpuMemory[addr+2];
	b->stepSize = cpuMemory[addr+4]+(cpuMemory[addr+5] << 8)+(cpuMemory[addr+6] << 16);
	b->stepOffset = cpuMemory[addr+8]+(cpuMemory[addr+9] << 8)+(cpuMemory[addr+10] << 16);
	b->stepCount = cpuMemory[addr+12]+(cpuMemory[addr+13] << 8)+(cpuMemory[addr+14] << 16);
}

// ***************************************************************************************
//
//								More Complex Blitter Copy
//
// ***************************************************************************************

uint8_t BLTComplexCopy(uint8_t action,uint16_t aSource,uint16_t aTarget) {
	struct BlitterArea source,target;

	_BLTLoadBlitterAreaObject(aSource,&source);
	_BLTLoadBlitterAreaObject(aTarget,&target);

	uint32_t srcCount = source.stepSize;  											// Bytes to copy to/from till next step.
	uint32_t tgtCount = target.stepSize;

	bool complete = source.stepCount == 0;  										// Completed if already zero.

	while (!complete) {  															// Until done every source step, e.g. the whole thing has been copied.
		//printf("SRC:%02x:%04x %d %d %d [%d]\n",source.page,(int)source.address,(int)source.stepCount,(int)source.stepSize,(int)source.stepOffset,(int)srcCount);
		//printf("TGT:%02x:%04x %d %d %d [%d]\n",target.page,(int)target.address,(int)target.stepCount,(int)target.stepSize,(int)target.stepOffset,(int)tgtCount);

		uint32_t transferSize = (srcCount < tgtCount) ? srcCount:tgtCount; 			// The amount to transfer this next time.
		//printf("%d transfer.\n",(int)transferSize);
		if (transferSize != 0) {
			uint8_t *src = _BLTGetRealAddress(source.page,source.address); 			// Workout addresses
			uint8_t *tgt = _BLTGetRealAddress(target.page,target.address);
			if (src == NULL || tgt == NULL) return 1; 								// Bad addresses.
			memmove(tgt,src,transferSize);  										// Do the copy
			srcCount -= transferSize;  												// Adjust for the 'to next step'
			tgtCount -= transferSize;
			_BLTAddAddress(&source,transferSize);  									// Adjust addresses.
			_BLTAddAddress(&target,transferSize);
		}

		if (srcCount == 0) {      													// Reached step on source.
			source.stepCount--;  													// Done one fewer
			srcCount = source.stepSize;  						 					// Rest to copy count.
			_BLTAddAddress(&source,source.stepOffset-source.stepSize); 				// Adjust the address to the next line.
			if (source.stepCount == 0) complete = true; 							// Transferred all the data.
		}

		if (tgtCount == 0) {      													// Reached step on target
			target.stepCount--;  													// Done one fewer
			tgtCount = target.stepSize;  						 					// Rest to copy count.
			_BLTAddAddress(&target,target.stepOffset-target.stepSize); 				// Adjust the address to the next line.
			if (target.stepCount == 0) complete = true; 							// Nowhere else to go. If zero initially goes till completed.
		}
	}
	return 0;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
