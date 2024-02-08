// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      console.cpp
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
//						User defined font memory (64 characters)
//
// ***************************************************************************************

uint8_t userDefinedFont[64*8];

uint8_t CONUpdateUserFont(uint8_t *data) {
	if (data[0] < 0xC0) return 1;
	for (int i = 0;i < 7;i++) {
		userDefinedFont[(data[0] & 0x3F)*8 + i] = data[i+1];
	}
	return 0;
}	

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
				if (ch >= 192) b = userDefinedFont[(ch & 0x3F) * 8 + y1]; 		// $C0-$FF UDG Memory.												

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
		if (SPRSpritesInUse()) {  												// Sprites present, only delete that layer
			for (int i = 0;i < gMode.xGSize*gMode.yGSize;i++) {
				graphMode->graphicsMemory[i] &= 0xF0;
			}
		} else {																// Erase graphics screen to black
			memset(graphMode->graphicsMemory,graphMode->backCol,MAXGRAPHICSMEMORY); 
		}
	}
	memset(graphMode->consoleMemory,' ',MAXCONSOLEMEMORY); 						// Erase character display to spaces.
	for (int y = 0;y < graphMode->yCSize;y++) {  								// No extended lines.
		graphMode->isExtLine[y] = 0;
	}
	GFXResetDefaults(); 														// Reset graphics defaults.
}

// ***************************************************************************************
//
//								   Reposition cursor
//
// ***************************************************************************************

uint8_t CONSetCursorPosition(uint8_t x,uint8_t y) {
	if (x >= graphMode->xCSize || y >= graphMode->yCSize) return 1;  			// Range error
	graphMode->xCursor = x;  													// Set new position.
	graphMode->yCursor = y;
	return 0;
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
//						Copy a character on display and console mirror
//	
// ***************************************************************************************

static void CONCopy(uint16_t xFrom,uint16_t yFrom,uint16_t xTo,uint16_t yTo) {
	graphMode->consoleMemory[xTo + yTo * MAXCONSOLEWIDTH] =  					// Copy console mirror
							graphMode->consoleMemory[xFrom + yFrom * MAXCONSOLEWIDTH];
	uint8_t *tgt = graphMode->graphicsMemory +  								// Goes here
					(yTo * graphMode->fontHeight * graphMode->xGSize) + (xTo * graphMode->fontWidth);							
	uint8_t *src = graphMode->graphicsMemory +  								// From here
					(yFrom * graphMode->fontHeight * graphMode->xGSize) + (xFrom * graphMode->fontWidth);							
	for (int y = 0;y < graphMode->fontHeight;y++) {  							// Copy character graphics
		memcpy(tgt,src,graphMode->fontWidth);   								// One row at a time.
		tgt += graphMode->xGSize;
		src += graphMode->xGSize;
	}
}

// ***************************************************************************************
//
//										Get end position
//	
// ***************************************************************************************

static uint16_t CONGetEndPosition(uint8_t y) {
	while (y < graphMode->yCSize-1) {  											// Until reached the last line.
		if (!graphMode->isExtLine[y]) return y; 								// This is a line end
		y++;  																	// Continuation, try next line.
	}
	return y;
}

// ***************************************************************************************
//
//								Delete character at cursor
//
// ***************************************************************************************

static void CONDeleteCharacter(void) {
	int yBase = CONGetEndPosition(graphMode->yCursor);  						// End of line
	int x = graphMode->xCursor,y = graphMode->yCursor;  						// Current position
	int xNext,yNext;
	while (x != graphMode->xCSize-1 || y != yBase) {  							// While not end of line
		xNext = x+1;yNext = y;if (xNext == graphMode->xCSize) { xNext = 0;yNext++; }
		CONCopy(xNext,yNext,x,y);
		x = xNext;y = yNext;
	}
	CONDrawCharacter(x,y,' ',7,0);	
}

// ***************************************************************************************
//
//								Insert Space at cursor
//
// ***************************************************************************************

static void CONInsertCharacter(void) {
	int y = CONGetEndPosition(graphMode->yCursor);  						// End of line
	int x = graphMode->xCSize-1;

	while (x != graphMode->xCursor || y != graphMode->yCursor) {  			// While not end of line
		int xPrev = x-1;int yPrev = y;   									// Work out prior character
		if (xPrev < 0) { yPrev--;xPrev = graphMode->xCSize-1; }
		CONCopy(xPrev,yPrev,x,y);  											// Copy previous forward.
		x = xPrev;y = yPrev;
	}
	CONDrawCharacter(x,y,' ',7,0);	
}

// ***************************************************************************************
//
//			Send command to console. Similar to BBC Micro, not all supported.
//
// ***************************************************************************************

void CONWrite(int c) {

	switch (c) {

		case CC_LEFT:															// A/1 left
			if (graphMode->xCursor-- == 0) 
				graphMode->xCursor = graphMode->xCSize-1;
			break;

		case CC_RIGHT:															// D/4 right
			if (++graphMode->xCursor == graphMode->xCSize) 
				graphMode->xCursor = 0;
			break;

		case CC_INSERT:  														// E/5 Insert
			CONInsertCharacter();break;

		case CC_BACKSPACE: 														// H/8 backspace
			if (graphMode->xCursor == 0) {  									// Start of line ?
				if (graphMode->yCursor == 0) return; 							// Top of screen.
				if (!graphMode->isExtLine[graphMode->yCursor-1]) return; 		// Start of a line ? 
				graphMode->xCursor = graphMode->xCSize-1;graphMode->yCursor--;	// Go to end of previous line.
			} else {
				graphMode->xCursor--;
			}
			CONDeleteCharacter();
			break;

		case CC_TAB:  															// I/9 Tab
			do {
				CONWrite(CC_RIGHT);
			} while ((graphMode->xCursor % 8) != 0);
			if (graphMode->xCursor == 0) CONWrite(CC_DOWN);
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

		case CC_DOWN: 															// S/19 down
			graphMode->yCursor = (graphMode->yCursor+1) % graphMode->yCSize;
			break;

		case CC_HOME: 															// T/20 home cursor.		
			graphMode->xCursor = graphMode->yCursor = 0;
			break;

		case CC_VTAB:  															// V/22 Vertical Tab
			do {
				CONWrite(CC_DOWN);
			} while ((graphMode->yCursor % 8) != 0);
			break;

		case CC_UP:																// W/23 up cursor
			graphMode->yCursor = (graphMode->yCursor+graphMode->yCSize - 1) % graphMode->yCSize;
			break;

		case CC_REVERSE:  														// X/24 reverse character at cursor
			CONReverseCursorBlock();break;

		case CC_DELETE:  														// Z/26 delete
			CONDeleteCharacter();break;

		default:
			if ((c >= ' ' && c < 127) || c >= 192) {  							// 32-126,192+ output a character.
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
//					Load screen line from current cursor position
//
// ***************************************************************************************

void CONGetScreenLine(uint16_t addr) {
	while (graphMode->isExtLine[graphMode->yCursor]) {  						// Find the bottom line.
		graphMode->yCursor++;
	}
	int start = graphMode->yCursor;  											// Figure out where to start.
	while (start > 0 && graphMode->isExtLine[start-1]) start--;

	int bufferSize = 0;
	for (int line = start;line <= graphMode->yCursor;line++) { 					// Input into buffer.
		for (int x = 0;x < graphMode->xCSize;x++) {
			if (bufferSize < 255) {
				int ch = graphMode->consoleMemory[x+line*MAXCONSOLEWIDTH];
				ch = (ch < ' ') ? ' ' : ch;
				cpuMemory[addr + bufferSize + 1] = ch;
				bufferSize++;
			}
		}
	}
	while (bufferSize > 0 && cpuMemory[addr+bufferSize] == ' ') bufferSize--;  	// Strip trailing spaces.
	cpuMemory[addr] = bufferSize;
	CONWrite(CC_ENTER); 														// Start of next line
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

void CONWriteString(const char *s) {
	while (*s != '\0') CONWrite(*s++);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//		11-01-24	Added user defined font option.
//		17-01-24 	TAB goes down at end of line.
//		30-01-24 	Fixed clear screen to clear text area only, not sprites too.
//		31-01-24 	Added functionality to set cursor position.
//		07-02-24	Added Insert and Delete functionality
//
// ***************************************************************************************
