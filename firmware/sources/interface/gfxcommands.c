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

void GFXRectangle(struct GraphicsMode *gMode,int x1,int y1,int x2,int y2,int solid) {
    int yRow;
	int yEnd;

	if (y2>y1) {
		yRow = y1;
		yEnd = y2+1;
	} else {
		yRow = y2;
		yEnd = y1+1;
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
}

// ***************************************************************************************
//
//									Draw scaled string
//
// ***************************************************************************************

void GFXScaledText(struct GraphicsMode *gMode,char *s,int x,int y) {
	while (*s != '\0') {
		char c = *s++;
		if (c >= ' ' && c < 0x80) {
			int y1 = y;
			for (int yc = 0;yc < 7;yc++) {				
				int bits = font_5x7[(c-' ')*8+yc];
				int x1 = x;
				while (bits != 0) {
					if (bits & 0x80) {
						GFXRectangle(gMode,x1,y1,x1+drawSize-1,y1+drawSize-1,-1);
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
			GFXRectangle(&gMode,x1,y1,x2,y2,useSolidFill);
			break;
		case 4:
			GFXEllipse(&gMode,x1,y1,x2,y2,useSolidFill);
			break;
		case 5:
			GFXPlotPixelChecked(&gMode,x1,y1);
			break;
		case 6:
			GFXScaledText(&gMode,DSPGetString(data,8),x1,y1);
			break;
	}
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
