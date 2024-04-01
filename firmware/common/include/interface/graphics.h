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

#define MAXCONSOLEWIDTH  	(80) 	 											// Max console size 
#define MAXCONSOLEHEIGHT  	(30)
#define MAXGRAPHICSMEMORY 	(320 * 240)  										// Max byte memory , graphics
#define MAXCONSOLEMEMORY 	(MAXCONSOLEWIDTH * (MAXCONSOLEHEIGHT+1))			// Max byte memory, console text.
																				// (extra line for scrolling.)
struct GraphicsMode {
	uint16_t xCSize,yCSize;														// Max size of console (chars)
	uint16_t xGSize,yGSize;														// Size of console (pixels) [0,0 = no graphics]
	uint8_t  fontWidth,fontHeight;  											// Font size in pixels.
	uint8_t  xCursor,yCursor;  													// Cursor position
	uint8_t  foreCol,backCol;  													// Current colours
	uint8_t *graphicsMemory;  													// graphics memory
	uint16_t *consoleMemory;  									  				// console memory.
	uint8_t  isExtLine[MAXCONSOLEHEIGHT]; 										// True if console is extended line.
};

extern struct GraphicsMode gMode;

void RNDSetPalette(uint8_t colour,uint8_t r,uint8_t g,uint8_t b); 				// Implementation specific.
int  RNDGetFrameCount(void);
void RNDStartMode0(struct GraphicsMode *gMode);

void GFXSetMode(int Mode);  													// General.g
void GFXDefaultPalette(void);
void GFXResetDefaults(void);
void GFXSetDefaults(uint8_t *cmd);
void GFXSetColour(uint8_t colour);
void GFXSetPalette(uint8_t colour,uint8_t r,uint8_t g,uint8_t b); 
void GFXGetPalette(uint8_t colour,uint8_t *r,uint8_t *g,uint8_t *b);
void GFXGraphicsCommand(uint8_t cmd,uint8_t *data);
void GFXFastLine(struct GraphicsMode *gMode,int x, int y, int x2, int y2);
void GFXPlotPixel(struct GraphicsMode *gMode,int x,int y);
void GFXPlotPixelChecked(struct GraphicsMode *gMode,int x,int y);
void GFXEllipse(struct GraphicsMode *gMode,int x1,int y1,int x2,int y2,int useSolidFill);
int GFXFindImage(int type,int id);
uint8_t GFXGetDrawSize(void);
void GFXSetDrawColour(uint8_t colour);
void GFXSetSolidFlag(uint8_t isSolid);
void GFXSetDrawSize(uint8_t size);
void GFXSetFlipBits(uint8_t flip);

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
