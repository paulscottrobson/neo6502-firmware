// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sprites.cpp
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
	p->xc = p->yc = p->x = p->y = -1;
	p->imageSize = 0xFF;p->flip = p->xSize = p->ySize = 0;
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

static SPRITE_ACTION saHide;

void SPRHide(uint8_t *paramData) {
	int spriteID = *paramData;  												// Sprite ID
	if (spriteID < MAX_SPRITES) {  												// Legit ?
		SPRITE_INTERNAL *s = &sprites[spriteID];
		if (s->isDrawn) {  														// If drawn, erase it and mark not drawn.
			saHide.display = gMode.graphicsMemory + s->x + s->y * gMode.xGSize; // Work out the draw address top left of sprite.
			saHide.image = s->imageAddress; 									// Where from.
			saHide.xSize = s->xSize;saHide.ySize = s->ySize;  					// Size and flip.
			saHide.flip = s->flip;
			SPRPHYErase(&saHide);
			sprites[spriteID].isDrawn = false;
		}
		sprites[spriteID].isVisible = false;  									// Mark not visible.
	}
}

// ***************************************************************************************
//
//								Get sprite position.
//
// ***************************************************************************************

uint8_t SPRGetSpriteData(uint8_t *param) {
	if (*param >= MAX_SPRITES) return 1; 										// Bad sprite #
	SPRITE_INTERNAL *p = &sprites[*param];										// Sprite structure
	param[1] = p->xc & 0xFF;param[2] = p->xc >> 8;
	param[3] = p->yc & 0xFF;param[4] = p->yc >> 8;
	return 0;
}

// ***************************************************************************************
//
//								Update a sprite
//
// ***************************************************************************************

static SPRITE_ACTION saRemove,saDraw;

int SPRUpdate(uint8_t *paramData) {

	uint8_t spriteID = paramData[0];  											// Sprite ID
	if (spriteID >= MAX_SPRITES) return 1;  									// Invalid

	uint16_t x = paramData[1] + (paramData[2] << 8);  							// Extract new data.
	uint16_t y = paramData[3] + (paramData[4] << 8);
	uint8_t  imageSize = paramData[5];
	uint8_t flip = paramData[6];

	
	SPRITE_INTERNAL *p = &sprites[spriteID];  									// Pointer to sprite structures

	bool xyChanged = ((x & 0xFF00) != 0x8000) && (p->x != x || p->y != y); 		// Check to see if elements changed.
	bool isChanged = (imageSize != 0x80) && (imageSize != p->imageSize);
	bool flipChanged = (flip != 0x80) && (flip != p->flip);

	//printf("Sprite #%d to (%d,%d) ImSize:%x Flip:%x %d %d %d @ %d,%d %d:%d\n",*paramData,x,y,paramData[5],paramData[6],xyChanged,isChanged,flipChanged,p->x,p->y,p->isVisible,p->isDrawn);

	if (xyChanged | isChanged | flipChanged) {   								// Some change made.
		if (p->isDrawn) {  														// Erase if currently drawn
			saRemove.display = gMode.graphicsMemory + p->x + p->y*gMode.xGSize; // Work out the draw address top left of sprite.
			saRemove.image = p->imageAddress; 									// Where from.
			saRemove.xSize = p->xSize;saRemove.ySize = p->ySize;  				// Size and flip.
			saRemove.flip = p->flip;
			SPRPHYErase(&saRemove);
			p->isDrawn = false;
		}
		//p->isVisible = true;  												// Mark as visible

		if (flipChanged) { 														// Flip has changed
			p->flip = flip; 
		}									

		if (isChanged) {  														// Image or size changed
			p->imageSize = imageSize;  											// Update image size.
			p->xSize = p->ySize = (imageSize & 0x40) ? 32:16;   				// Size of image.
			int img = GFXFindImage((p->xSize == 16) ? 1 : 2,imageSize & 0x3F);	// Address of image (offset in gfx memory)													
			if (img < 0) return 2;   											// Bad image number
			p->imageAddress = gfxMemory + img;  								// Physical address
		}

		if (xyChanged) {  														// Positions changed.
			p->x = x - p->xSize / 2;   											// Initially anchor point is centre.
			p->y = y - p->ySize / 2;
			p->xc = x;p->yc = y;  												// Remember centre position
			p->isVisible = true;
			//printf("changed %d %d %d\n",p->x,p->y,p->isVisible);
		}

		if (p->isVisible) {  													// Redraw if possible.
			saDraw.display = gMode.graphicsMemory + p->x + p->y * gMode.xGSize; // Work out the draw address top left of sprite.
			saDraw.image = p->imageAddress; 									// Where from.
			saDraw.x = p->x;saDraw.y = p->y;  									// Position on screen
			saDraw.xSize = p->xSize;saDraw.ySize = p->ySize;  					// Size and flip.
			saDraw.flip = p->flip;
			SPRPHYDraw(&saDraw);
			p->isDrawn = true;  												// And mark as drawn.
		}
	}

	return 0; 
}

// ***************************************************************************************
//
//								Collision check
//
// ***************************************************************************************

uint8_t SPRCollisionCheck(uint8_t *error,uint8_t s1,uint8_t s2,uint8_t distance) {
	if (s1 >= MAX_SPRITES || s2 >= MAX_SPRITES) { 								// Validate parameters
		*error = 1;
		return 0;
	}
	SPRITE_INTERNAL *p1 = &sprites[s1],*p2 = &sprites[s2]; 						// Structure pointers.
	int sdist = (p1->xc-p2->xc)*(p1->xc-p2->xc)+(p1->yc-p2->yc)*(p1->yc-p2->yc);// Square of distance
	return sdist <= distance*distance;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//		12-01-24	Changed initial image size to $FF as not picked up if sprite initialise to $80
//
// ***************************************************************************************
