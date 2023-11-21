// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      console.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st November 2023
//      Reviewed :  No
//      Purpose :   Console system.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "data/font_5x7.h"

struct GraphicsMode *graphMode;                                         
uint16_t xCursor = 0,yCursor = 0;
uint16_t foreCol = 7,backCol = 0;

// ***************************************************************************************
//
//									Raw character output
//
// ***************************************************************************************

static void CONDrawCharacter(uint16_t x,uint16_t y,uint16_t ch,uint16_t fcol,uint16_t bcol) {
	if (x < graphMode->xCSize && y < graphMode->yCSize) {  						// Coords in range.
		graphMode->consoleMemory[x + y * MAXCONSOLEWIDTH] = ch;  				// Update console memory.
		uint16_t cWidth = graphMode->fontWidth,cHeight = graphMode->fontHeight; 
		if (graphMode->xGSize != 0) {  											// Only if graphics mode.
			for (uint16_t y1 = 0;y1 < cHeight;y1++) {  							// Each line of font data
				uint16_t b = font_5x7[(ch-32)*cHeight + y1]; 					// Bit pattern for that line.
				uint8_t *screen = graphMode->graphicsMemory+					// Where in memory it starts.
												x*cWidth+(y*cHeight+y1) * 320;	
				screen += ((320-graphMode->xCSize*cWidth)/2);  					// Horizontal centering.
				for (uint16_t x1 = 0;x1 < cWidth;x1++) { 						// Output colours MSB first.
					*screen++ = (b & 0x80) ? fcol : bcol;
					b = b << 1;
				}
			}
		}
	}
}

// ***************************************************************************************
//
//									Clear the screen
//
// ***************************************************************************************

static void CONClearScreen(void) {
	xCursor = yCursor = 0;  													// Home cursor
	if (graphMode->xGSize != 0) {  												// Graphics present ?
		memset(graphMode->graphicsMemory,backCol,MAXGRAPHICSMEMORY); 	 		// Erase graphics screen to black
	}
	memset(graphMode->consoleMemory,' ',MAXCONSOLEMEMORY); 						// Erase character display to spaces.
}

// ***************************************************************************************
//
//								Initialise the console system
//
// ***************************************************************************************

void CONInitialise(struct GraphicsMode *gMode) {
	graphMode = gMode;	
	foreCol = 7;backCol = 0; 	 												// Reset colours
	CONWrite(12);  																// Clear screen / home cursor.
}

// ***************************************************************************************
//
//									Scroll the screen up
//
// ***************************************************************************************

void CONScrollUp(void) {
	memmove(graphMode->consoleMemory,  											// Scroll console up.
		   graphMode->consoleMemory + MAXCONSOLEWIDTH,  							
		   (MAXCONSOLEHEIGHT-1) * MAXCONSOLEWIDTH);

	memset(graphMode->consoleMemory + (graphMode->yCSize-1) * MAXCONSOLEWIDTH,	// Blank bottom line.
																' ',MAXCONSOLEWIDTH);
	if (graphMode->xGSize != 0) {
		memmove(graphMode->graphicsMemory,  									// Scroll screen up (graphics)
			   graphMode->graphicsMemory+320*graphMode->fontHeight,
			   (graphMode->yCSize-1) * graphMode->fontHeight * 320);
																				// Clear bottom line.
		memset(graphMode->graphicsMemory + (graphMode->yCSize-1) * graphMode->fontHeight * 320,
				backCol, graphMode->fontHeight * 320);		
	}		
}

// ***************************************************************************************
//
//			Send command to console. Similar to BBC Micro, not all supported.
//
// ***************************************************************************************

void CONWrite(int c) {
	switch (c) {
		case CC_LF:
			yCursor++; 															// J/10 down with scrolling.
			if (yCursor == graphMode->yCSize) {
				yCursor--;
				CONScrollUp();
			}
			break;
		case CC_CLS: 	 														// L/12 clears the screen
			CONClearScreen();break;
		case CC_ENTER: 	 														// M/13 carriage return.
			xCursor = 0;CONWrite(CC_LF);break;
		case CC_HOME: 															// T/20 home cursor.
			xCursor = yCursor = 0;break;
		default:
			if (c >= ' ' && c < 127) {  										// 32-126 output a character.
				CONDrawCharacter(xCursor,yCursor,c,foreCol,backCol);
				xCursor++;
				if (xCursor == graphMode->xCSize) CONWrite(CC_ENTER);
			} else {

			}
	}
}

// ***************************************************************************************
//
//			Rubbish debugging tool - don't use it for printing hex numbers.
//
// ***************************************************************************************

void CONWriteHex(uint16_t h) {
	CONWrite(' ');
	for (uint16_t i = 0;i < 4;i++) {
		CONWrite("0123456789ABCDEF"[(h >> 12) & 0x0F]);
		h = h << 4;
	}
}

