// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      gfxcommands.c
//      Authors :   Po-Han Lin (http://www.edepot.com)
//      Date :      30th December 2023
//      Reviewed :  No
//      Purpose :   Graphics Commands
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

static uint8_t pixelAnd,pixelXor,useSolidFill,drawSize;

// ***************************************************************************************
//
// 						Reset defaults to standard, called on CLS
//
// ***************************************************************************************

void GFXResetDefaults(void) {
	pixelAnd = 0;pixelXor = 7;
	useSolidFill = 0;drawSize = 1;
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
}

// ***************************************************************************************
//
// 										Draw a pixel
//
// ***************************************************************************************

void GFXPlotPixel(struct GraphicsMode *gMode,int x,int y) {
	uint8_t *pixel = gMode->graphicsMemory+x+y*gMode->xGSize;
	*pixel = ((*pixel) & pixelAnd) ^ pixelXor;
}

void GFXPlotPixelChecked(struct GraphicsMode *gMode,int x,int y) {
	if (x >= 0 && y >= 0 && x < gMode->xGSize && y < gMode->yGSize) {
		uint8_t *pixel = gMode->graphicsMemory+x+y*gMode->xGSize;
		*pixel = ((*pixel) & pixelAnd) ^ pixelXor;
	}
}

// ***************************************************************************************
//
// 										Draw a rectangle
//
// ***************************************************************************************

void GFXRectangle(struct GraphicsMode *gMode,int x1,int y1,int x2,int y2) {
	int ySize = abs(y1-y2);
	int yDir = (y1 < y2) ? 1 : -1;
	for (int l = 0;l < ySize;l++) {
		if (useSolidFill != 0 || l == 0 || l == ySize-1) {
			GFXFastLine(gMode,x1,y1,x2,y1);
		} else {
			GFXPlotPixelChecked(gMode,x1,y1);
			GFXPlotPixelChecked(gMode,x2,y1);
		}
		y1 += yDir;
	}
}

// ***************************************************************************************
//
//								Handle graphics commands
//
// ***************************************************************************************

void GFXGraphicsCommand(uint8_t cmd,uint8_t *data) {
	
	uint16_t x1 = data[4]+(data[5] << 8);
	uint16_t y1 = data[6]+(data[7] << 8);
	uint16_t x2 = data[8]+(data[9] << 8);
	uint16_t y2 = data[10]+(data[11] << 8);

	switch(cmd) {
		case 2:
			GFXFastLine(&gMode,x1,y1,x2,y2);
			break;
		case 3:
			GFXRectangle(&gMode,x1,y1,x2,y2);
			break;
		case 4:
			GFXEllipse(&gMode,x1,y1,x2,y2,useSolidFill);
			break;
		case 5:
			GFXPlotPixelChecked(&gMode,x1,y1);
			break;
	}
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
