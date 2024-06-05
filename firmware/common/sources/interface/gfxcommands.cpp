// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      gfxcommands.cpp
//      Authors :   Po-Han Lin (http://www.edepot.com)
//      Date :      30th December 2023
//      Reviewed :  No
//      Purpose :   Graphics Commands
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include <algorithm>	// For std::min/max
#include <utility>	  // For std::swap

static uint8_t pixelAnd,pixelXor,useSolidFill,drawSize,flipBits;

// ***************************************************************************************
//
// 						Reset defaults to standard, called on CLS
//
// ***************************************************************************************

void GFXResetDefaults(void) {
	pixelAnd = 0;pixelXor = 7;
	useSolidFill = 0;drawSize = 1;
	flipBits = 0;
}

// ***************************************************************************************
//
// 									Set Graphics Colour
//
// ***************************************************************************************

void GFXSetColour(uint8_t colour) {
	pixelAnd = 0;
	pixelXor = colour;
}

// ***************************************************************************************
//
// 						Set Defaults according to command buffer
//
// ***************************************************************************************

void GFXSetDefaults(uint8_t *cmd) {
	pixelAnd = cmd[4];
	pixelXor = cmd[5];
	useSolidFill = cmd[6];
	drawSize = cmd[7];
	flipBits = cmd[8];
}

// ***************************************************************************************
//
// 								  Set Defaults specifically
//
// ***************************************************************************************

void GFXSetDrawColour(uint8_t colour) {
	pixelAnd = 0;pixelXor = colour;
}

void GFXSetSolidFlag(uint8_t isSolid) {
	useSolidFill = (isSolid != 0) ? 0xFF : 0x00;
}

void GFXSetDrawSize(uint8_t size) {
	drawSize = size;
}

void GFXSetFlipBits(uint8_t flip) {
	flipBits = flip;
}

// ***************************************************************************************
//
//									Accessor for draw size
//
// ***************************************************************************************

uint8_t GFXGetDrawSize(void) {
	return drawSize;
}

// ***************************************************************************************
//
// 										Draw a pixel
//
// ***************************************************************************************

void GFXPlotPixel(struct GraphicsMode *gMode,int x,int y) {
	uint8_t sprAnd = (SPRSpritesInUse() ? pixelAnd | 0xF0 : pixelAnd);
	uint8_t *pixel = gMode->graphicsMemory+x+y*gMode->xGSize;
	*pixel = ((*pixel) & sprAnd) ^ pixelXor;
}

void GFXPlotPixelChecked(struct GraphicsMode *gMode,int x,int y) {
	uint8_t sprAnd = (SPRSpritesInUse() ? pixelAnd | 0xF0 : pixelAnd);
	if (x >= 0 && y >= 0 && x < gMode->xGSize && y < gMode->yGSize) {
		uint8_t *pixel = gMode->graphicsMemory+x+y*gMode->xGSize;
		*pixel = ((*pixel) & sprAnd) ^ pixelXor;
	}
}

// ***************************************************************************************
//
// 										Draw a rectangle
//
// ***************************************************************************************

// Helper to draw a horizontal line (without safety checks!)
static void hline_noclip(struct GraphicsMode *gMode, int xMin, int xMax, int y) {
	uint8_t sprAnd = (SPRSpritesInUse() ? pixelAnd | 0xF0 : pixelAnd);
	uint8_t *pixel = gMode->graphicsMemory + xMin + (y * gMode->xGSize);
	if (sprAnd == 0) {
		// Fast-track case: no reads or bitmasking needed.
		memset(pixel, pixelXor, (xMax+1)-xMin);
	} else {
		for (int x = xMin; x <= xMax; ++x) {
			*pixel = ((*pixel) & sprAnd) ^ pixelXor;
			++pixel;
		}
	}
}

// Helper to draw a vertical line (without safety checks!)
static void vline_noclip(struct GraphicsMode *gMode, int x, int yMin, int yMax) {
	uint8_t sprAnd = (SPRSpritesInUse() ? pixelAnd | 0xF0 : pixelAnd);
	uint8_t *pixel = gMode->graphicsMemory + x + (yMin * gMode->xGSize);
	for (int y = yMin; y <= yMax; ++y) {
		*pixel = ((*pixel) & sprAnd) ^ pixelXor;
		pixel += gMode->xGSize;
	}
}


void GFXRectangle(struct GraphicsMode *gMode,int x1,int y1,int x2,int y2,int solid,bool setZero) {
	int xMin, xMax, yMin, yMax;
	// Exchange coordinates if needed.
	if (x1 > x2) {
		std::swap(x1, x2);
	}
	if (y1 > y2) {
		std::swap(y1, y2);
	}

	// Clip against screen.
	xMin = std::max(0, x1);
	xMax = std::min((int)gMode->xGSize - 1, x2);
	yMin = std::max(0, y1);
	yMax = std::min((int)gMode->yGSize - 1, y2);

	int w = xMax - xMin;
	int h = yMax - yMin;
	if (w < 0 || h < 0) {
		return; // Totally offscreen.
	}

	// Save some drawing state.
	int orgPixelAnd = pixelAnd;
	int orgPixelXor = pixelXor;

	if (setZero) {
		pixelAnd = 0;
		pixelXor = 0;
	}

	if (solid == 0) {
		// We're drawing an rectangle outline.
		// We might be XORing pixels, so be careful to avoid overdraw!

		// Top edge onscreen?
		if (yMin == y1) {
			hline_noclip(gMode, xMin, xMax, yMin);
			++yMin; // Avoid overdraw for sides.
		}
		// Bottom edge onscreen (and not coincident with top edge)?
		if (yMax == y2 && yMax != y1) {
			hline_noclip(gMode, xMin, xMax, yMax);
			--yMax; // Avoid overdraw for sides.
		}

		// Draw sides (if there's a gap between top and bottom).
		if (yMax >= yMin) {
			if (xMin == x1) {
				vline_noclip(gMode, xMin, yMin, yMax);
			}
			if (xMax == x2) {
				vline_noclip(gMode, xMax, yMin, yMax);
			}
		}
	} else {
		// We're drawing a filled rectangle.
		for (int y = yMin; y <= yMax; ++y) {
			hline_noclip(gMode, xMin, xMax, y);
		}
	}

	// Restore drawing state.
	pixelAnd = orgPixelAnd;
	pixelXor = orgPixelXor;
}

// ***************************************************************************************
//
//									Draw scaled string
//
// ***************************************************************************************

void GFXScaledText(struct GraphicsMode *gMode,char *s,int x,int y,int useSolidFill) {
	while (*s != '\0') {
		uint8_t c = *s++;
		if ((c >= ' ' && c < 0x80) || (c >= 0xC0)) {
			int y1 = y;
			for (int yc = 0;yc < 8;yc++) {				
				int bits = font_5x7[(c-' ')*8+yc];
				if (c >= 0xC0) bits = userDefinedFont[(c & 0x3F) * 8 + yc];
				int x1 = x;
				for (int pixel = 0;pixel < 6;pixel++) {
					if (bits & 0x80) {
						GFXRectangle(gMode,x1,y1,x1+drawSize-1,y1+drawSize-1,-1,false);
					} else {
						if (useSolidFill) {
							GFXRectangle(gMode,x1,y1,x1+drawSize-1,y1+drawSize-1,-1,true);						
						}
					}
					x1 += drawSize;
					bits = (bits << 1) & 0xFF;
				}
			y1 += drawSize;
			}
		}
		x = x + 6 * drawSize;
	}
}

// ***************************************************************************************
//
//					Get address of image in the image graphics set.
//
// ***************************************************************************************

int GFXFindImage(int type,int id) {
	int addr = -1;

	if (id >= gfxObjectMemory[type+1]) return -1;  								// Not a valid graphic element

	switch(type) {
		case 0:  																// 16x16 tiles
			addr = 256 + id * (16*16/2);
			break;
		case 1:  																// 16x16 sprites
			addr = 256 + gfxObjectMemory[1]*(16*16/2) + id*(16*16/2);
			break;
		case 2:  																// 32x32 sprites
			addr = 256 + gfxObjectMemory[1]*(16*16/2) + gfxObjectMemory[2]*(16*16/2) + id*(32*32/2);
			break;
	}
	return addr;
}

// ***************************************************************************************
//
//		Image renderer. This is not required to be too speed optimised as it is a
//		drawing function rather than a gaming function.
//
// ***************************************************************************************

void GFXDrawImage(struct GraphicsMode *gMode,int x,int y,int id,int scale,int flip,int solidFill) {
	if (gfxObjectMemory[0] == 0) return;  										// No graphics installed.
	int size = 16;  															// Figure out the size in pixels, type (0-2) and id in that type.
	int type = 0;   															// Sprite records use a different mapping.
	if (id >= 0x80) {   														// Drawing a sprite 16x16 ($80-$BF) 32x32 ($C0-$FF)
		if (id >= 0xC0) size = 32;
		type = (id >= 0xC0) ? 2 : 1;
		id = id & 0x3F;
	}
	int xFlip = 0,yFlip = 0; 													// Work out horizontal/vertical flipping.
	if (flip & 1) xFlip = size-1;  												// This is done with exclusive ORing.
	if (flip & 2) yFlip = size-1;
	int address = GFXFindImage(type,id);  										// Address in the graphics memory

	for (int xc = 0;xc < size;xc++) {  											// For each pixel
		for (int yc = 0;yc < size;yc++) {
			int pixel = gfxObjectMemory[address+xc/2+(yc * size / 2)];   		// Access the pixel pair.
			pixel = (xc & 1) ? pixel & 0x0F : pixel >> 4;						// Extract the half pixel to draw.
			if (pixel != 0 || solidFill) {  									// If not transparent or solid then draw it.
				pixelXor = pixel;
				pixelAnd = 0;
				int x1 = x + (xc ^ xFlip) * scale;
				int y1 = y + (yc ^ yFlip) * scale;
				GFXRectangle(gMode,x1,y1,x1+scale-1,y1+scale-1,true,false);
			}
		}
	}
}

// ***************************************************************************************
//
//								Handle graphics commands
//
// ***************************************************************************************

void GFXGraphicsCommand(uint8_t cmd,uint8_t *data) {
	
	int16_t x1 = data[4]+(data[5] << 8);
	int16_t y1 = data[6]+(data[7] << 8);
	int16_t x2 = data[8]+(data[9] << 8);
	int16_t y2 = data[10]+(data[11] << 8);
	bool isOk;

	switch(cmd) {
		case 2:
			GFXFastLine(&gMode,x1,y1,x2,y2);
			break;
		case 3:
			GFXRectangle(&gMode,x1,y1,x2,y2,useSolidFill,false);
			break;
		case 4:
			GFXEllipse(&gMode,x1,y1,x2,y2,useSolidFill);
			break;
		case 5:
			GFXPlotPixelChecked(&gMode,x1,y1);
			break;
		case 6:
			GFXScaledText(&gMode,DSPGetString(data,8),x1,y1,useSolidFill);
			break;
		case 7:
			GFXDrawImage(&gMode,x1,y1,data[8],drawSize,flipBits,useSolidFill);
			break;
		case 33:
			isOk = (x1 >= 0 && y1 >= 0 && x1 < gMode.xGSize && y1 < gMode.yGSize);
			data[2] = isOk ? 0 : 1;
			if (isOk) {
				data[4] = gMode.graphicsMemory[x1 + y1 * gMode.xGSize];
				if (SPRSpritesInUse()) data[4] &= 0x0F;
			}
			break;
		case 36:
			isOk = (x1 >= 0 && y1 >= 0 && x1 < gMode.xGSize && y1 < gMode.yGSize);
			data[2] = isOk ? 0 : 1;
			data[4] = 0;
			if (isOk && SPRSpritesInUse()) {
				data[4] = gMode.graphicsMemory[x1 + y1 * gMode.xGSize] >> 4;
			}
			break;
	}
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//		11-01-24	Modified to support UDGs
//		17-01-24 	Modified so graphics work ok with sprites.
//		18-01-24 	Added function 33 (read pixel)
//		19/01/24 	Added SOLID option to TEXT and IMAGE
//		28/01/24 	Line parameters are now signed integers
//		30/01/24 	Added SPOINT to read sprite pixel (36)
//		31/01/24 	Fixed pixel leakage on IMAGE.
//
// ***************************************************************************************
