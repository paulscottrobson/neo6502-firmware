// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      graphics.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Rien Matthijsse
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
	int xGSize,yGSize;															// Size of console (pixels)

	VOIDFUNC setPalette; 	 													// Set the palette
	VOIDFUNC startMode;  														// Start up the mode.
};

void GFXSetMode(int Mode);

#endif

