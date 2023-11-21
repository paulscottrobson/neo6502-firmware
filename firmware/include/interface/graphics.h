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
	uint16_t xCSize,yCSize;														// Size of console (chars)
	uint16_t xGSize,yGSize;														// Size of console (pixels) [0,0 = no graphics]
	uint8_t  fontWidth,fontHeight;  											// Font size in pixels.

	uint8_t *graphicsMemory,*consoleMemory;  									// Graphics & text memory.

	VOIDFUNC setPalette; 	 													// Set the palette
	VOIDFUNC startMode;  														// Start up the mode.
};

#define MAXCONSOLEWIDTH  	(80) 	 											// Max console size 
#define MAXCONSOLEHEIGHT  	(31)
#define MAXGRAPHICSMEMORY 	(320 * 240)  										// Max byte memory , graphics
#define MAXCONSOLEMEMORY 	(MAXCONSOLEWIDTH * (MAXCONSOLEHEIGHT+1))			// Max byte memory, console text.
																				// (extra line for scrolling.)

void GFXSetMode(int Mode);

#endif

