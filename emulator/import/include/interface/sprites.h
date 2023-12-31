// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sprites.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      4th January 2024
//      Reviewed :  No
//      Purpose :   Sprite drawing code.
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _SPRITES_H
#define _SPRITES_H

#define MAX_SPRITES (128) 														// Max supported. Doesn't mean it's a good idea :)

typedef struct _sprite_internal {
	bool isDrawn;  																// TRUE if currently drawn
	bool isVisible;  															// TRUE if currently visible
	uint16_t x,y;  																// Current drawn position (e.g. passed in)
	uint16_t xSize,ySize;  														// Sprite horizontal/vertical size.
	uint8_t imageSize;  														// image (0:5) size (6) value
	uint8_t *imageAddress; 	 													// Physical graphic address in gfxMemory
	uint8_t flip;  																// flip 0:x 1:y
} SPRITE_INTERNAL;

typedef struct _sprite_action {
	uint8_t *display;
	uint8_t *image;
	uint8_t flip;
	uint8_t xSize,ySize;
} SPRITE_ACTION;

void SPRReset(void);  															// Sprite reset.
void SPRHide(uint8_t *paramData);
int SPRUpdate(uint8_t *paramData);

void SPRPHYErase(SPRITE_ACTION *s); 											// Sprite draw/erase routines
void SPRPHYDraw(SPRITE_ACTION *s);

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
