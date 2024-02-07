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
// 								Draw a horizontal line.
//
// ***************************************************************************************

static void GFXHorizontalLine(struct GraphicsMode *gMode,int x1,int x2,int y) {
    int x;
	int xEnd;

	if (x2>x1){
		x = x1;
		xEnd = x2+1;
	} else {
		x = x2;
		xEnd = x1+1;
	}

	do {
		GFXPlotPixelChecked(gMode,x,y);
		x++;
	} while( x != xEnd);

}

// ***************************************************************************************
//
// 										Draw a rectangle
//
// ***************************************************************************************

void GFXRectangle(struct GraphicsMode *gMode,int x1,int y1,int x2,int y2,int solid,bool setZero) {
    int yRow;
	int yEnd;
	int orgPixelAnd = pixelAnd;
	int orgPixelXor = pixelXor;

	if (y2>y1) {
		yRow = y1;
		yEnd = y2+1;
	} else {
		yRow = y2;
		yEnd = y1+1;
	}

	if (setZero) {
		pixelAnd = 0;pixelXor = 0;
	}

    do {
		if (solid != 0 || yRow == y1 || yRow == y2) {
			GFXHorizontalLine(gMode,x1,x2,yRow);
		} else {
			GFXPlotPixelChecked(gMode,x1,yRow);
			GFXPlotPixelChecked(gMode,x2,yRow);
		}
        yRow++;

	} while( yRow != yEnd);

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
			for (int yc = 0;yc < 7;yc++) {				
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

	if (id >= gfxMemory[type+1]) return -1;  									// Not a valid graphic element

	switch(type) {
		case 0:  																// 16x16 tiles
			addr = 256 + id * (16*16/2);
			break;
		case 1:  																// 16x16 sprites
			addr = 256 + gfxMemory[1]*(16*16/2) + id*(16*16/2);
			break;
		case 2:  																// 32x32 sprites
			addr = 256 + gfxMemory[1]*(16*16/2) + gfxMemory[2]*(16*16/2) + id*(32*32/2);
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
	if (gfxMemory[0] == 0) return;  											// No graphics installed.
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
			int pixel = gfxMemory[address+xc/2+(yc * size / 2)];   				// Access the pixel pair.
			pixel = (xc & 1) ? pixel & 0x0F : pixel >> 4;						// Extract the half pixel to draw.
			if (pixel != 0 || type == 0 || solidFill) {  						// If non-zero, or tile, or solid then draw it.
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
