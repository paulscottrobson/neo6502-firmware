// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      graphics.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st November 2023
//      Reviewed :  No
//      Purpose :   Graphics mode manager include
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _GRAPHICS_H
#define _GRAPHICS_H

void RNDSetPalette(uint8_t colour,uint8_t r,uint8_t g,uint8_t b); 				// Implementation specific.
void RNDStartMode0(uint8_t *memConsole,uint8_t *memGraphics);

struct GraphicsMode {
	int xCSize,yCSize;															// Size of console (chars)
	int xGSize,yGSize;															// Size of console (pixels) [0,0 = no graphics]
	int textFont;   															// Text font to use.

	uint8_t *screenMemory,*consoleMemory;  										// Graphics & text memory.

	VOIDFUNC setPalette; 	 													// Set the palette
	VOIDFUNC startMode;  														// Start up the mode.
};

#define MAXGRAPHICSMEMORY 	(320 * 240)  										// Max byte memory , graphics
#define MAXCONSOLEMEMORY 	(80*30) 											// Max byte memory, console text.

#define USE_FONT_5X7  		(57) 												// Use 5 x 7 font

void GFXSetMode(int Mode);

#endif

