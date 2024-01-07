// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      logo.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      7th January 2024
//      Reviewed :  No
//      Purpose :   Logo drawer
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//								 Logo monochrome data
//
// ***************************************************************************************

static const uint8_t graphic_logo[] = 
#include "data/logo_logo_png.h"

static const uint8_t graphic_text[] = 
#include "data/logo_neotext_png.h"

static const uint8_t graphic_olimex[] = 
#include "data/logo_olimex_png.h"

// ***************************************************************************************
//
//									Draw logo etc.
//
// ***************************************************************************************

static void _LOGDraw(const uint8_t *data,uint8_t colour,int yPos) {
	int xByteSize = *data++;
	int ySize = *data++;
	printf("%d\n",xByteSize);
	for (int y = 0;y < ySize;y++) {
		int x = 160 - (xByteSize * 4);
		for (int x1 = 0;x1 < xByteSize;x1++) {
			int b = *data++;
			for (int x2 = 0;x2 < 8;x2++) {
				if (b & 0x80) gMode.graphicsMemory[x+(yPos+y)*320] = colour;
				b <<= 1;
				x++;
			}
		}
	}
}

void LOGDrawLogo(void) {
	_LOGDraw(graphic_logo,3,32);
	_LOGDraw(graphic_text,6,100);
	_LOGDraw(graphic_olimex,1,140);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
