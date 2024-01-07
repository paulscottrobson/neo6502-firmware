// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sprites.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      4th January 2024
//      Reviewed :  No
//      Purpose :   Sprite drawing code.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

static SPRITE_INTERNAL sprites[MAX_SPRITES];

// ***************************************************************************************
//
//										Reset a sprites
//
// ***************************************************************************************

static void _SPRResetSprite(int n) {
	SPRITE_INTERNAL *p = &sprites[n];  											// Set everything to default values.
	p->isDrawn = p->isVisible = false;
	p->x = p->y = -1;
	p->imageSize = p-> flip = p->xSize = p->ySize = 0;
	p->drawAddress = p->imageAddress = 0;
}

// ***************************************************************************************
//
//									Reset all sprites
//
// ***************************************************************************************

void SPRReset(void) {
	for (int i = 0;i < MAX_SPRITES;i++) {  										// Reset all sprites.
		_SPRResetSprite(i);
	}
	for (int i = 0;i < gMode.xGSize * gMode.yGSize;i++) {  						// Clear the sprite layer
		gMode.graphicsMemory[i] &= 0x0F;  										// top 4 bits og graphics memory.
	}
}

// ***************************************************************************************
//
//									Reset all sprites
//
// ***************************************************************************************

void SPRHide(uint8_t *paramData) {
	int spriteID = *paramData;  												// Sprite ID
	if (spriteID < MAX_SPRITES) {  												// Legit ?
		if (sprites[spriteID].isDrawn) {  										// If drawn, erase it and mark not drawn.
			SPRPHYErase(&sprites[spriteID]);
			sprites[spriteID].isDrawn = false;
		}
		sprites[spriteID].isVisible = false;  									// Mark not visible.
	}
}

// ***************************************************************************************
//
//								Update a sprite
//
// ***************************************************************************************

int SPRUpdate(uint8_t *paramData) {

	uint8_t spriteID = paramData[0];  											// Sprite ID
	if (spriteID >= MAX_SPRITES) return 1;  									// Invalid

	uint16_t x = paramData[1] + (paramData[2] << 8);  							// Extract new data.
	uint16_t y = paramData[3] + (paramData[4] << 8);
	uint8_t  imageSize = paramData[5];
	uint8_t flip = paramData[6];

	printf("Sprite #%d to (%d,%d) ImSize:%x Flip:%x\n",*paramData,x,y,paramData[5],paramData[6]);

	bool xyChanged = (x & 0xFF00) != 0x8000;  									// Check to see if elements changed.
	bool isChanged = (imageSize != 0x80);
	bool flipChanged = (flip != 0x80);

	if (xyChanged | isChanged | flipChanged) {   								// Some change made.
		SPRITE_INTERNAL *p = &sprites[spriteID];  								// Pointer to sprite structures
		if (p->isDrawn) {  														// Erase if currently drawn
			SPRPHYErase(p);
			p->isDrawn = false;
		}
		p->isVisible = true;  													// Mark as visible

		if (flipChanged) p->flip = flip;  										// Flip has changed.

		if (isChanged) {  														// Image or size changed
			p->imageSize = imageSize;  											// Update image size.
			p->xSize = p->ySize = (imageSize & 0x40) ? 32:16;   				// Size of image.
			p->imageAddress = GFXFindImage((p->xSize == 16) ? 1 : 2,  			// Address of image
															imageSize & 0x3F);
			if (p->imageAddress < 0) return 2;   								// Bad image number
			printf("%d %d %d %d\n",p->imageSize,p->xSize,p->ySize,p->flip);
		}

		if (xyChanged) {  														// Positions changed.
			p->x = x - p->xSize / 2;   											// Initially anchor point is centre.
			p->y = y - p->ySize / 2;
			printf("%d %d\n",p->x,p->y);
			p->isVisible = (p->x >= 0 && p->y >= 0 &&  							// Initially clip very simply all must be on screen.
									p->x < gMode.xGSize-p->xSize && p->y < gMode.yGSize-p->ySize);
		}

		if (p->isVisible) {  													// Redraw if possible.
			SPRPHYDraw(p); 
			p->isDrawn = true;  												// And mark as drawn.
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
