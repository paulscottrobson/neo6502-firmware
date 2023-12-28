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
	graphMode->xCursor = graphMode->yCursor = 0;  								// Home cursor
	if (graphMode->xGSize != 0) {  												// Graphics present ?
		memset(graphMode->graphicsMemory,graphMode->backCol,MAXGRAPHICSMEMORY); // Erase graphics screen to black
	}
	memset(graphMode->consoleMemory,' ',MAXCONSOLEMEMORY); 						// Erase character display to spaces.
	for (int y = 0;y < graphMode->yCSize;y++) {  								// No extended lines.
		graphMode->isExtLine[y] = 0;
	}
}

// ***************************************************************************************
//
//								Initialise the console system
//
// ***************************************************************************************

void CONInitialise(struct GraphicsMode *gMode) {
	graphMode = gMode;	
	graphMode->foreCol = 7;graphMode->backCol = 0; 	 							// Reset colours
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
			   graphMode->backCol, graphMode->fontHeight * 320);		
	}		
	for (int y = 0;y < graphMode->yCSize;y++) {  								// Scroll extended line.
		graphMode->isExtLine[y] = graphMode->isExtLine[y+1];
	}
	graphMode->isExtLine[graphMode->yCSize-1] = 0;
}

// ***************************************************************************************
//
//								Reverse colours at cursor
//
// ***************************************************************************************

static void CONReverseCursorBlock(void) {
	for (int y = 0;y < graphMode->fontHeight;y++) {
		uint8_t *p = graphMode->graphicsMemory + 
					 (y + graphMode->yCursor * graphMode->fontHeight) * graphMode->xGSize +
					graphMode->xCursor * graphMode->fontWidth;
		for (int x = 0;x < graphMode->fontWidth;x++) {
			*p ^= 7;
			p++;
		}
	}
}

// ***************************************************************************************
//
//			Send command to console. Similar to BBC Micro, not all supported.
//
// ***************************************************************************************

void CONWrite(int c) {

	FDBWrite(c);  																// Echo to debug stream
	
	switch (c) {

		case CC_LEFT:															// A/1 left
			if (graphMode->xCursor-- == 0) 
				graphMode->xCursor = graphMode->xCSize-1;
			break;

		case CC_RIGHT:															// D/4 right
			if (++graphMode->xCursor == graphMode->xCSize) 
				graphMode->xCursor = 0;
			break;

		case CC_BACKSPACE: 														// H/8 backspace
			if (graphMode->xCursor > 0) {
				graphMode->xCursor--;
				CONDrawCharacter(graphMode->xCursor,graphMode->yCursor,' ',graphMode->backCol,graphMode->backCol);
			}
			break;

		case CC_TAB:  															// I/9 Tab
			do {
				CONWrite(' ');
			} while ((graphMode->xCursor % 8) != 0);
			break;
			
		case CC_LF:
			graphMode->yCursor++; 												// J/10 down with scrolling.
			if (graphMode->yCursor == graphMode->yCSize) {
				graphMode->yCursor--;
				CONScrollUp();
			}
			break;

		case CC_CLS: 	 														// L/12 clears the screen
			CONClearScreen();break;

		case CC_ENTER: 	 														// M/13 carriage return.
			graphMode->isExtLine[graphMode->yCursor] = 0;
			graphMode->xCursor = 0;CONWrite(CC_LF);break;

		case CC_HOME: 															// T/20 home cursor.		
			graphMode->xCursor = graphMode->yCursor = 0;break;

		case CC_UP:																// W/22 up cursor
			if (graphMode->yCursor > 0) graphMode->yCursor--;
			break;

		case CC_REVERSE:  														// X/24 reverse character at cursor
			CONReverseCursorBlock();break;

		default:
			if (c >= ' ' && c < 127) {  										// 32-126 output a character.
				CONDrawCharacter(graphMode->xCursor,graphMode->yCursor,c,graphMode->foreCol,graphMode->backCol);
				graphMode->xCursor++;
				if (graphMode->xCursor == graphMode->xCSize) {  				// Char at EOL mark extended.
					graphMode->isExtLine[graphMode->yCursor] = 1;
					graphMode->xCursor = 0;CONWrite(CC_LF);
				}

			} else {
				if (c >= 0x80 && c < 0x90) graphMode->foreCol = c & 0x0F;
				if (c >= 0x90 && c < 0xA0) graphMode->backCol = c & 0x0F;
			}
	}
}

// ***************************************************************************************
//
//								Rubbish debugging tools.
//
// ***************************************************************************************

void CONWriteHex(uint16_t h) {
	CONWrite(' ');
	for (uint16_t i = 0;i < 4;i++) {
		CONWrite("0123456789ABCDEF"[(h >> 12) & 0x0F]);
		h = h << 4;
	}
}

void CONWriteString(char *s) {
	while (*s != '\0') CONWrite(*s++);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
