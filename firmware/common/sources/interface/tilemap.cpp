// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      tilemap.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st January 2024
//      Reviewed :  No
//      Purpose :   Tilemap Handler
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

static uint8_t *mapData = NULL; 											// Address of map data
static int16_t width,height; 												// Size of map in tiles.
static int16_t xTilePos,yTilePos; 													// Position in map (pixels)
static int16_t xWindow,yWindow,wWindow,hWindow; 							// Draw window
static uint8_t *gDraw; 														// Draw from here.
static uint8_t *tilePtr; 													// Tile data from here.
static uint8_t *tilePixels; 												// Tile pixel data.
static uint16_t yTile; 														// Tracking tile draw position.
static uint8_t tileSize;  													// Tile Size = 16 or 32
static uint8_t tileShift;  													// Tile shift to divide (4 or 5)

static void TMRRenderTileLine(uint8_t count);
static void TMRenderTileLineStart(uint8_t count);	
static uint8_t *TMPGetTileRowAddress(uint8_t tileID,uint8_t yOffset);
static void TMROutputBackground(uint16_t n);

// ***************************************************************************************
//
//								Set up a tilemap for use.
//
// ***************************************************************************************

void TMPSelectTileMap(uint8_t *data,uint16_t xOffset,uint16_t yOffset) {
//	printf("TM:$%x %d,%d\n",data-cpuMemory,xOffset,yOffset);
	mapData = data;xTilePos = xOffset;yTilePos = yOffset;
	width = mapData[1];height = mapData[2];
}

// ***************************************************************************************
//
//								Draw a tilemap in the given window
//
// ***************************************************************************************

uint8_t TMPDrawTileMap(uint8_t *data) {

	int xPos = xTilePos,yPos = yTilePos;

	tileSize = 16;tileShift = 4;  											// Work out size and shift
	if (GFXGetDrawSize() == 2) {
		tileSize = 32;tileShift = 5;
	}

	if (mapData == NULL) return 1;  										// No map specified.
	if (xPos < 0 ||  yPos < 0 ||  											// Invalid draw position.
				xPos >= width * tileSize || yPos >= height * tileSize) return 1;

	int16_t x1 = data[4]+(data[5] << 8);  									// Parameters.
	int16_t y1 = data[6]+(data[7] << 8);
	int16_t x2 = data[8]+(data[9] << 8);
	int16_t y2 = data[10]+(data[11] << 8);

	xWindow = (x1 < x2) ? x1 : x2;yWindow = (y1 < y2) ? y1 : y2; 			// Work out drawing window
	wWindow = abs(x1-x2);hWindow = abs(y1-y2);
	if (wWindow < tileSize) return 1;  										// Reject this special case.


	if (xWindow < 0) {  													// Adjust for off left
		xPos += abs(xWindow);
		wWindow -= abs(xWindow);
		xWindow = 0;
	}
	if (yWindow < 0) {  													// Adjust for off top
		yPos += abs(yWindow);
		hWindow -= abs(yWindow);
		yWindow = 0;
	}

	if (xWindow < 0 || yWindow < 0) return 0;
	if (xWindow >= gMode.xGSize || yWindow >= gMode.yGSize) return 0; 		// Entirely off 
	if (xWindow + wWindow >= gMode.xGSize) wWindow = gMode.xGSize-xWindow; 	// Trim to top and bottom.
	if (yWindow + hWindow >= gMode.yGSize) hWindow = gMode.yGSize-yWindow;

	int x = xWindow;  														// Start position.
	int y = yWindow;  														// Draw from top to bottom.
	int lineCount = hWindow;
	yTile = yPos; 															// Tracking tile line.

	int xLeader,xBlock16,xTrailer;

	int todo = wWindow;   													// Pixels horizontally
	if (xPos+wWindow >= width * tileSize) todo = width*tileSize-xPos;  		// Trim to right side
	xLeader = (-xPos & (tileSize-1));  										// Pixels to display to first whole tile.
	xBlock16 = (todo - xLeader) / tileSize; 								// Number of whole tiles to display
	xTrailer = todo - xLeader - xBlock16 * tileSize;  						// Pixels to display to edge.

	gDraw = gMode.graphicsMemory + x + yWindow * gMode.xGSize;				// Start drawing here.

//	printf("TD:%d,%d %d,%d\n",xWindow,yWindow,wWindow,hWindow);	
//	printf("%d %d %d\n",xLeader,xBlock16,xTrailer);	

	while (lineCount > 0 && yTile < height * tileSize) {  					// Until complete or off tile map
		tilePtr = mapData + 3 + (yTile>>tileShift)*width+(xPos>>tileShift);	// Tile data comes from here.
		uint8_t *gStart = gDraw;  											// Start of drawing.
		if (y >= 0 && y <= gMode.yGSize) {			
			if (xLeader != 0) TMRenderTileLineStart(xLeader); 				// Do the first pixels.
			for (int i = 0;i < xBlock16;i++) TMRRenderTileLine(tileSize); 	// Render complete 16 pixel tiles.
			if (xTrailer != 0) TMRRenderTileLine(xTrailer); 				// Do the last pixels.
			if (todo != wWindow) TMROutputBackground(wWindow-todo);			// Any following blanks
		}
		y++;lineCount--;  													// Next line.
		gDraw = gStart + gMode.xGSize;  									// Down on screen
		yTile++; 															// Next tile position
	}
	while (lineCount-- > 0) {  												// Blank the bottom unused area to sprites only.
		for (uint16_t i = 0;i < wWindow;i++) {
			gDraw[i] &= 0xF0;
		}
		gDraw += gMode.xGSize;
	}
	return 0;
}

static void TMROutputBackground(uint16_t n) {
	while (n-- > 0) *gDraw++ &= 0xF0;
}
// ***************************************************************************************
//
//							Get address of tile row
//
// ***************************************************************************************

static uint8_t *TMPGetTileRowAddress(uint8_t tileID,uint8_t yOffset) {
	if (tileID >= gfxMemory[1]) return NULL;
	return gfxMemory + 256 + tileID * 16*16/2 + yOffset * 8;
}

// ***************************************************************************************
//
//					Render count pixels of the currrent tile
//
// ***************************************************************************************

static void TMRRenderTileLine(uint8_t count) {
	uint8_t yOffset = (tileSize == 32) ? yTile >> 1 : yTile;  			// Offset in tile.
	tilePixels = TMPGetTileRowAddress(*tilePtr,yOffset & 15);  			// Tile Pixel data comes from here.

	if (tilePixels == NULL) {   										// Transparent/Solid tile.
		if (*tilePtr >= 0xF0) {  										// Handle solid tile			
			uint8_t b = *tilePtr & 0x0F;  								// Colour
			uint32_t b2 = b | (b << 8) | (b << 16) | (b << 24);  		// Colour in 32 bits
			//
			//		Do while tiles to render and not on a 32 bit boundary
			//
			while (count > 0 && ((uint32_t(gDraw)) & 3)) {  			
				*gDraw = (*gDraw & 0xF0) | b;gDraw++;count--;
			}
			//
			//		While on a 32 bit boundary do it 4 bytes at a time.
			//
			while (count > 3) {
				*((uint32_t *)gDraw) = (*((uint32_t *)gDraw) & 0xF0F0F0F0) | b2;
				gDraw += 4;count -= 4;
			}
			//
			//		Do anything remaining.
			//
			while (count > 0) {  			
				*gDraw = (*gDraw & 0xF0) | b;gDraw++;count--;
			}

		} else {
			gDraw += count;
		}
		tilePtr++;
		return;
	}
	uint8_t i = count >> 1;  											// Number of whole bytes to unpack
	if (tileSize == 16) {  												// 16x16 tiles. Done this long winded way to help optimisation
		while (i-- > 0) {
			uint8_t bData = *tilePixels++;
			*gDraw = ((*gDraw) & 0xF0) | (bData >> 4);
			gDraw++;
			*gDraw = ((*gDraw) & 0xF0) | (bData & 0x0F);
			gDraw++;		
		}
	} else {  															// 32x32 scaled tiles
		i = i >> 1;
		while (i-- > 0) {
			uint8_t bData = *tilePixels++;
			*gDraw = ((*gDraw) & 0xF0) | (bData >> 4);
			gDraw++;
			*gDraw = ((*gDraw) & 0xF0) | (bData >> 4);
			gDraw++;
			*gDraw = ((*gDraw) & 0xF0) | (bData & 0x0F);
			gDraw++;
			*gDraw = ((*gDraw) & 0xF0) | (bData & 0x0F);
			gDraw++;
		}
	}
	if (count & 1) {  													// Unpack one.twi extra as there are two pixels/byte
		uint8_t bData = *tilePixels++;
		*gDraw = ((*gDraw) & 0xF0) | (bData >> 4);
		gDraw++;		
		if (tileSize == 32) {
			*gDraw = ((*gDraw) & 0xF0) | (bData >> 4);
			gDraw++;					
		}
	}
	tilePtr++;
}

// ***************************************************************************************
//
//					 Render the left hand edge of the tile
//
// ***************************************************************************************

static void TMRenderTileLineStart(uint8_t count) {
	uint8_t yOffset = (tileSize == 32) ? yTile >> 1 : yTile;  			// Offset in tile.
	tilePixels = TMPGetTileRowAddress(*tilePtr,yOffset & 15);  			// Tile Pixel data comes from here.

	if (tilePixels == NULL) {  											// Transparent/solid tile.
		if (*tilePtr > 0xF0) {  										// F1..FF are solid tiles in that colour.
			while (count--) {
				*gDraw = (*gDraw & 0xF0) | (*tilePtr & 0x0F);
				gDraw++;
			}
		} else {
			gDraw += count;
		}
		tilePtr++;
		return;
	}
	if (tileSize == 32) count >>= 1;
	uint8_t i = count >> 1;  											// Number of whole pixels
	tilePixels += (16-count) >> 1;  									// Position in tile line
	if (count & 1) {  													// Is there the odd half-byte ?
		uint8_t bData = *tilePixels++;
		*gDraw = ((*gDraw) & 0xF0) | (bData & 0x0F);
		gDraw++;
		if (tileSize == 32) {  											// Handle for scaled tiles.
			*gDraw = ((*gDraw) & 0xF0) | (bData & 0x0F);
			gDraw++;			
		}
	}
	while (i-- > 0) {  													// Do all the remaining bytes
		uint8_t bData = *tilePixels++;
		*gDraw = ((*gDraw) & 0xF0) | (bData >> 4);
		gDraw++;
		if (tileSize == 32) {
			*gDraw = ((*gDraw) & 0xF0) | (bData >> 4);
			gDraw++;
		}
		*gDraw = ((*gDraw) & 0xF0) | (bData & 0x0F);
		gDraw++;
		if (tileSize == 32) {
			*gDraw = ((*gDraw) & 0xF0) | (bData & 0x0F);
			gDraw++;	
		}
	}
	tilePtr++;  														// Next entry in tile map
}
// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//		28-01-24 	Amended to allow tilemaps to overlap windows.
//		07-02-24 	Started conversion to allow 32x32 tiles (scaled)
//
// ***************************************************************************************
