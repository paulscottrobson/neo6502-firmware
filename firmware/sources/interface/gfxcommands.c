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
	pixelAnd = cmd[0];
	pixelXor = cmd[1];
	useSolidFill = cmd[2];
	drawSize = cmd[3];
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
	}
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
