// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      graphics.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Rien Matthijsse
//      Date :      21st November 2023
//      Reviewed :  No
//      Purpose :   Graphics mode manager
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

struct GraphicsMode gMode;														// Info about current mode.

static uint8_t screenMemory[320 * 240],consoleMemory[80*30]; 					// RAM used for graphics and console text.

// ***************************************************************************************
//
//							Initialise the graphics mode 0
//
// ***************************************************************************************

static void GFXInitialise320x240x256(void) {
	gMode.xCSize = 48;gMode.yCSize = 30;										// Console text size
	gMode.xGSize = 320;gMode.yGSize = 240;  									// Graphics pixel size.
	gMode.startMode = (VOIDFUNC)RNDStartMode0;  								// Set up support functions.
	gMode.setPalette = (VOIDFUNC)RNDSetPalette;
}

// ***************************************************************************************
//
//								Change display mode
//
// ***************************************************************************************

void GFXSetMode(int Mode) {
	GFXInitialise320x240x256(); 												// Initialise mode 0
	(*gMode.startMode)(consoleMemory,screenMemory);  							// Start it, telling it about memory.
	
	for (int i = 0;i < 256;i++) {  												// Default (wrong) palette.
		uint8_t c = i;
		uint8_t cOn = (c & 8) ? 127:255,cOff = 0;
		(*gMode.setPalette)(c,(i & 1) ? cOn:cOff,(i & 2) ? cOn:cOff,(i & 4) ? cOn:cOff); 
   }
}

