// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      blitter.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      24th March 2024
//      Reviewed :  No
//      Purpose :   Blitter header
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _BLITTER_H
#define _BLITTER_H

struct BlitterArea {
	uint16_t	address;
	uint8_t		page;
    uint8_t     padding;     // unused. Should be zero.
	int16_t		stride;		 // Number of bytes between start of each line in memory (in bytes).
	uint8_t		format;      // one of BLTFMT_*
    // Everything below here is ignored for target.
	uint8_t		transparent; // transparent value in src, for BLTACT_MASK and BLTACT_SOLID.
	uint8_t		solid;	 	 // constant value for BLTACT_SOLID.
	uint8_t  	height;		 // Number of lines.
	uint16_t  	width;		 // Size of line, in src units.
};

// Format types for BlitterArea.format field
#define BLTFMT_BYTE 0		// whole byte
#define BLTFMT_PAIR 1		// 2 4-bit values, aka nibbles (src only)
#define BLTFMT_BITS 2		// 8 1-bit values (src only)
#define BLTFMT_HIGH 3		// High nibble (target only)
#define BLTFMT_LOW  4		// Low nibble (target only)

uint8_t BLTSimpleCopy(uint8_t pageFrom,uint16_t addressFrom, uint8_t pageTo, uint16_t addressTo, uint16_t transferSize);

// Values for BLTComplexCopy and BLTImage action params
#define BLTACT_COPY 0		// Straight rectangle copy
#define BLTACT_MASK 1		// Copy, but only where src != srcarea.transparent
#define BLTACT_SOLID 2		// Fill with constant srcarea.solid value, but only where src != srcarea.transparent

uint8_t BLTComplexCopy(uint8_t action,uint16_t aSource,uint16_t aTarget);

uint8_t BLTImage(uint8_t action, uint16_t sourceArea, int16_t x, int16_t y, uint8_t destFmt);

#endif
// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
