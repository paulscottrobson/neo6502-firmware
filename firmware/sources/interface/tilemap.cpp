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
static int16_t xPos,yPos; 													// Position in map (pixels)
static uint16_t xWindow,yWindow,wWindow,hWindow; 							// Draw window
static uint8_t *gDraw; 														// Draw from here.
static uint8_t *tilePtr; 													// Tile data from here.
static uint8_t *tilePixels; 												// Tile pixel data.
static uint16_t yTile; 														// Tracking tile draw position.

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
	mapData = data;xPos = xOffset;yPos = yOffset;
	width = mapData[1];height = mapData[2];
}

// ***************************************************************************************
//
//								Draw a tilemap in the given window
//
// ***************************************************************************************

uint8_t TMPDrawTileMap(uint8_t *data) {
	if (mapData == NULL) return 1;  										// No map specified.
	if (xPos < 0 ||  yPos < 0 ||  											// Invalid draw position.
				xPos >= width * 16 || yPos >= height * 16) return 1;

	uint16_t x1 = data[4]+(data[5] << 8);  									// Parameters.
	uint16_t y1 = data[6]+(data[7] << 8);
	uint16_t x2 = data[8]+(data[9] << 8);
	uint16_t y2 = data[10]+(data[11] << 8);

	if (x1 >= gMode.xGSize || x2 >= gMode.xGSize ||  						// Validate parameters.
			y1 >= gMode.yGSize || y2 >= gMode.yGSize) return 1;

	xWindow = (x1 < x2) ? x1 : x2;yWindow = (y1 < y2) ? y1 : y2; 			// Work out drawing window
	wWindow = abs(x1-x2);hWindow = abs(y1-y2);
	if (wWindow < 16) return 1;  											// Reject this special case.

	int x = xWindow;  														// Start position.
	int y = yWindow;  														// Draw from top to bottom.
	int lineCount = hWindow;
	yTile = yPos; 															// Tracking tile line.

	int xLeader,xBlock16,xTrailer;

	int todo = wWindow;   													// Pixels horizontally
	if (xPos+wWindow >= width * 16) todo = width*16-xPos;  					// Trim to right side
	xLeader = (-xPos & 15);  												// Pixels to display to first whole tile.
	xBlock16 = (todo - xLeader) / 16;   									// Number of whole tiles to display
	xTrailer = todo - xLeader - xBlock16 * 16;  							// Pixels to display to edge.

	gDraw = gMode.graphicsMemory + x + yWindow * gMode.xGSize;				// Start drawing here.

//	printf("TD:%d,%d %d,%d\n",xWindow,yWindow,wWindow,hWindow);	
//	printf("%d %d %d\n",xLeader,xBlock16,xTrailer);	

	while (lineCount > 0 && yTile < height * 16) {  						// Until complete or off tile map
		tilePtr = mapData + 3 + (yTile >> 4) * width + (xPos >> 4);			// Tile data comes from here.
		uint8_t *gStart = gDraw;  											// Start of drawing.

		if (0) {  															// Solid block code will go here.

		} else {
			if (xLeader != 0) TMRenderTileLineStart(xLeader); 				// Do the first pixels.
			for (int i = 0;i < xBlock16;i++) TMRRenderTileLine(16); 		// Render complete 16 pixel tiles.
			if (xTrailer != 0) TMRRenderTileLine(xTrailer); 				// Do the last pixels.
			if (todo != wWindow) TMROutputBackground(wWindow-todo);			// Any following blanks

			y++;lineCount--;  												// Next line.
			gDraw = gStart + gMode.xGSize;  								// Down on screen
			yTile++; 														// Next tile position
		}
	}
	while (lineCount-- > 0) {
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
	tilePixels = TMPGetTileRowAddress(*tilePtr,yTile & 15);  			// Tile Pixel data comes from here.

	if (tilePixels == NULL) {   										// Transparent/Solid tile.
		if (*tilePtr > 0xF0) {  										// Handle solid tile
			uint8_t b = *tilePtr & 0x0F;
			while (count--) {
				*gDraw = (*gDraw & 0xF0) | b;gDraw++;
			}
		} else {
			gDraw += count;
		}
		tilePtr++;
		return;
	}

	uint8_t i = count >> 1;  											// Number of whole bytes to unpack
	while (i-- > 0) {
		uint8_t bData = *tilePixels++;
		*gDraw = ((*gDraw) & 0xF0) | (bData >> 4);
		gDraw++;
		*gDraw = ((*gDraw) & 0xF0) | (bData & 0x0F);
		gDraw++;
	}
	if (count & 1) {  													// Unpack one extra as there are two pixels/byte
		uint8_t bData = *tilePixels++;
		*gDraw = ((*gDraw) & 0xF0) | (bData >> 4);
		gDraw++;		
	}
	tilePtr++;
}

// ***************************************************************************************
//
//					 Render the left hand edge of the tile
//
// ***************************************************************************************

static void TMRenderTileLineStart(uint8_t count) {
	tilePixels = TMPGetTileRowAddress(*tilePtr,yTile & 15);  			// Tile Pixel data comes from here.
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
	uint8_t i = count >> 1;  											// Number of whole pixels
	tilePixels += (16-count) >> 1;  									// Position in tile line
	if (count & 1) {  													// Is there the odd half-byte ?
		uint8_t bData = *tilePixels++;
		*gDraw = ((*gDraw) & 0xF0) | (bData & 0x0F);
		gDraw++;
	}
	while (i-- > 0) {  													// Do all the remaining bytes
		uint8_t bData = *tilePixels++;
		*gDraw = ((*gDraw) & 0xF0) | (bData >> 4);
		gDraw++;
		*gDraw = ((*gDraw) & 0xF0) | (bData & 0x0F);
		gDraw++;
	}
	tilePtr++;  														// Next entry in tile map
}
// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
