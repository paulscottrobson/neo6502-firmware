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

static void _SPXORDrawForwardLine(uint8_t bytes,uint8_t *display,uint8_t *image) {
	while (bytes--) {
		uint8_t b = *image++;
		if (b & 0xF0) {
			*display ^= (b & 0xF0);
		}
		display++;
		if (b & 0x0F) {
			*display ^= b << 4;
		}		
		display++;
	}
}

static void _SPXORDrawBackwardLine(uint8_t bytes,uint8_t *display,uint8_t *image) {
	while (bytes--) {
		uint8_t b = *image++;
		--display;
		if (b & 0xF0) {
			*display ^= (b & 0xF0);
		}
		--display;
		if (b & 0x0F) {
			*display ^= b << 4;
		}		
	}
}

// ***************************************************************************************
//
//					  Erase sprite : Same as draw as we are XORing
//
// ***************************************************************************************

void SPRPHYErase(SPRITE_INTERNAL *s) {
	SPRPHYDraw(s); 
}

// ***************************************************************************************
//
//										Draw a sprite
//
// ***************************************************************************************

void SPRPHYDraw(SPRITE_INTERNAL *s) {
	s->drawAddress = s->x + s->y * gMode.xGSize;  	 							// Work out the draw address top left of sprite.
	uint8_t *imgData = gfxMemory + s->imageAddress;  							// Where to get it from.
	uint8_t yXor = (s->flip & 2) ? (s->ySize-1) : 0;  							// Used to y-flip
	for (int yPos = 0;yPos < s->ySize;yPos++) {   								// Work top to bottom
		uint8_t *screenLine = gMode.graphicsMemory + s->drawAddress + (yPos ^ yXor) * gMode.xGSize;
		if (s->flip & 1) {
			_SPXORDrawBackwardLine(s->xSize/2,screenLine+s->xSize-1,imgData); 	// Draw a backward line.
		} else {
			_SPXORDrawForwardLine(s->xSize/2,screenLine,imgData); 				// Draw a forward line.
		}
		imgData += (s->xSize / 2);  											// Next row.
	}
	//printf("%d %d\n",s->isVisible,s->drawAddress);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
