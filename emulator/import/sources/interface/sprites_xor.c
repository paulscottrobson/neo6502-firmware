// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sprites_xor.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      4th January 2024
//      Reviewed :  No
//      Purpose :   XOR Sprite drawing code.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//								XOR in image L->R
//
// ***************************************************************************************

static void _SPXORDrawForwardLine(SPRITE_ACTION *sa) {
	uint8_t bytes = sa->xSize/2;
	uint8_t *image = sa->image;
	uint8_t *display = sa->display;
	uint8_t b;
	while (bytes--) {
		if ((b = *image++)) {
			if (b & 0xF0) {
				*display ^= (b & 0xF0);
			}
			display++;
			if (b & 0x0F) {
				*display ^= b << 4;
			}		
			display++;
		} else {
			display += 2;
		}
	}
}

// ***************************************************************************************
//
//								XOR in image R->L
//
// ***************************************************************************************

static void _SPXORDrawBackwardLine(SPRITE_ACTION *sa) {
	uint8_t bytes = sa->xSize/2;
	uint8_t *image = sa->image;
	uint8_t *display = sa->display + sa->xSize;
	uint8_t b;
	while (bytes--) {
		if ((b = *image++)) {
			--display;
			if (b & 0xF0) {
				*display ^= (b & 0xF0);
			}
			--display;
			if (b & 0x0F) {
				*display ^= b << 4;
			}					
		} else {
			display -= 2;
		}
	}
}

// ***************************************************************************************
//
//					  Erase sprite : Same as draw as we are XORing
//
// ***************************************************************************************

void SPRPHYErase(SPRITE_ACTION *s) {
	SPRPHYDraw(s); 
}

// ***************************************************************************************
//
//										Draw a sprite
//
// ***************************************************************************************

void SPRPHYDraw(SPRITE_ACTION *s) {

	int yAdjust = gMode.xGSize;  												// Handle vertical flipping.
	if (s->flip & 2) {
		s->display += (s->ySize-1) * gMode.xGSize;
	 	yAdjust = -yAdjust;
	}
	for (int yPos = 0;yPos < s->ySize;yPos++) {   								// Work top to bottom
		if (s->flip & 1) {  													// Draw according to x flip
			_SPXORDrawBackwardLine(s); 					
		} else {
			_SPXORDrawForwardLine(s); 					
		}
		s->image += s->xSize/2;
		s->display += yAdjust;
	}
	//printf("%d %d\n",s->isVisible,s->drawAddress);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
