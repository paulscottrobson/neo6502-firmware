// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      console.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      3rd March 2024
//      Reviewed :  No
//      Purpose :   Console system. Version 2.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#include "interface/font_5x7.h"
#include <stdarg.h>

struct GraphicsMode *graphMode;                                         

// ***************************************************************************************
//
//						User defined font memory (64 characters)
//
// ***************************************************************************************

uint8_t userDefinedFont[64*8];

// ***************************************************************************************
//
//									Update a user character
//
// ***************************************************************************************

uint8_t CONUpdateUserFont(uint8_t *data) {
	if (data[0] < 0xC0) return 1;
	for (int i = 0;i < 7;i++) {
		userDefinedFont[(data[0] & 0x3F)*8 + i] = data[i+1];
	}
	return 0;
}	

// ***************************************************************************************
//
//						Repaint character at console position (x,y)
//
// ***************************************************************************************

static void CONPaintCharacter(uint16_t x,uint16_t y) {
 	if (x < graphMode->xCSize && y < graphMode->yCSize) {  						// Coords in range.
 		uint16_t ch = graphMode->consoleMemory[x + y * MAXCONSOLEWIDTH];		// Character data
 		uint8_t fcol = (ch >> 8) & 0x0F,bcol = (ch >> 12) & 0x0F; 				// Extract colours
 		uint16_t cWidth = graphMode->fontWidth,cHeight = graphMode->fontHeight; 
 		ch = ch & 0xFF;  														// Character #

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
//						Raw character output, tracking console.
//
// ***************************************************************************************

static void CONDrawCharacter(uint16_t x,uint16_t y,uint16_t ch,uint16_t fcol,uint16_t bcol) {
 	if (x < graphMode->xCSize && y < graphMode->yCSize) {  						// Coords in range.
 		graphMode->consoleMemory[x + y * MAXCONSOLEWIDTH] =  					// Character and colours are packed into the console Memory.
 												ch | (fcol << 8) | (bcol << 12);
		CONPaintCharacter(x,y);  												// And repaint it. 												
	}
}

// ***************************************************************************************
//
//									Clear the screen
//
// ***************************************************************************************

void CONClearScreen(void) {
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
	for (int c = 0;c < MAXCONSOLEMEMORY;c++) {  								// Erase the console memory.
		graphMode->consoleMemory[c] = ' ' + (7 << 8) + (0 << 12);
	}
	for (int y = 0;y < graphMode->yCSize;y++) {  								// No extended lines.
		graphMode->isExtLine[y] = 0;
	}
	CONSetCursorVisible(1);														// Cursor now visible again.
	GFXResetDefaults(); 														// Reset graphics defaults.
}

// ***************************************************************************************
//
//							Accessors for cursor position
//
// ***************************************************************************************

uint8_t CONSetCursorPosition(uint8_t x,uint8_t y) {
	if (x >= graphMode->xCSize || y >= graphMode->yCSize) return 1;  			// Range error
	graphMode->xCursor = x;  													// Set new position.
	graphMode->yCursor = y;
	return 0;
}

void CONGetCursorPosition(uint8_t* x, uint8_t* y) {
	*x = graphMode->xCursor;
	*y = graphMode->yCursor;
}

// ***************************************************************************************
//
//								  Fetch the screen size
//
// ***************************************************************************************

void CONGetScreenSizeChars(uint8_t* width, uint8_t* height) {
	*width = graphMode->xCSize;
	*height = graphMode->yCSize;
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
//						Copy a character from one position to another
//	
// ***************************************************************************************

static void CONCopy(uint16_t xFrom,uint16_t yFrom,uint16_t xTo,uint16_t yTo) {
	graphMode->consoleMemory[xTo + yTo * MAXCONSOLEWIDTH] =  					// Copy console mirror data
		 						graphMode->consoleMemory[xFrom + yFrom * MAXCONSOLEWIDTH];
	CONPaintCharacter(xTo,yTo);		 						
}

// ***************************************************************************************
//
//									Insert and delete lines
//
// ***************************************************************************************

void CONInsertLine(uint8_t y) {
	if (y >= graphMode->yCSize) return; 										// Bad line #
	uint8_t y1 = graphMode->yCSize-1;  											// Copy to line.
	while (y != y1) {  															// Copy the main block down.
		for (int x = 0;x < graphMode->xCSize;x++) {
			CONCopy(x,y-1,x,y);
			graphMode->isExtLine[y] = graphMode->isExtLine[y-1];
		}
		y1--;
	}
	for (int x = 0;x < graphMode->xCSize;x++) {  								// Blank bottom line and make it not extended.
		CONDrawCharacter(x,y,' ',graphMode->foreCol,graphMode->backCol);
	}
	graphMode->isExtLine[y] = false;  
}

// ***************************************************************************************
//
//										Delete lines
//
// ***************************************************************************************

void CONDeleteLine(uint8_t y) {
	if (y >= graphMode->yCSize) return; 										// Bad line #
	while (y != graphMode->yCSize-1) {  										// Copy the main block up.
		for (int x = 0;x < graphMode->xCSize;x++) {
			CONCopy(x,y+1,x,y);
			graphMode->isExtLine[y] = graphMode->isExtLine[y+1];
		}
		y++;
	}
	for (int x = 0;x < graphMode->xCSize;x++) {  								// Blank bottom line and make it not extended.
		CONDrawCharacter(x,y,' ',graphMode->foreCol,graphMode->backCol);
	}
	graphMode->isExtLine[y] = false;  
}

// ***************************************************************************************
//
//								Clears an area on the screen
//
// ***************************************************************************************

void CONClearArea(int x1, int y1, int x2, int y2) {
	x1 = std::min(graphMode->xCSize-1, x1);
	y1 = std::min(graphMode->yCSize-1, y1);
	x2 = std::min(graphMode->xCSize-1, x2);
	y2 = std::min(graphMode->yCSize-1, y2);
	x2 = std::max(x2, x1);
	y2 = std::max(y2, y1);

	for (int x=x1; x<=x2; x++)
		for (int y=y1; y<=y2; y++)
			CONDrawCharacter(x, y, ' ', graphMode->foreCol, graphMode->backCol);
}

// ***************************************************************************************
//
//								Set/Get the text colours
//
// ***************************************************************************************

void CONSetForeBackColour(int fg, int bg) {
	graphMode->foreCol = fg & 0x0f;
	graphMode->backCol = bg & 0x0f;
}

void CONGetForeBackColour(int *fg, int *bg) {
	*fg = graphMode->foreCol;
	*bg = graphMode->backCol;
}

// ***************************************************************************************
//
//								Set cursor visible flag
//
// ***************************************************************************************

void CONSetCursorVisible(uint8_t vFlag) {
	graphMode->isCursorVisible = vFlag;
}
// ***************************************************************************************
//
//								Reverse colours at cursor
//
// ***************************************************************************************

void CONReverseCursorBlock(void) {
	if (graphMode->isCursorVisible != 0) {
		for (int y = 0;y < graphMode->fontHeight;y++) {
			uint8_t *p = graphMode->graphicsMemory + 
						 (y + graphMode->yCursor * graphMode->fontHeight) * graphMode->xGSize +
						graphMode->xCursor * graphMode->fontWidth;
			for (int x = 0;x < graphMode->fontWidth;x++) {
				*p ^= graphMode->foreCol;
				p++;
			}
		}
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
	CONDrawCharacter(x,y,' ',graphMode->foreCol,graphMode->backCol);	
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
	CONDrawCharacter(x,y,' ',graphMode->foreCol,graphMode->backCol);	
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
				CONDeleteLine(0);
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
				int ch = graphMode->consoleMemory[x+line*MAXCONSOLEWIDTH] & 0xFF;
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

void CONWriteString(const char *s, ...) {
	va_list ap;

	va_start(ap, s);
	int len = vsnprintf(NULL, 0, s, ap);
	va_end(ap);

	char buffer[len+1];
	va_start(ap, s);
	vsnprintf(buffer, len+1, s, ap);
	va_end(ap);
	
	s = buffer;
	while (*s != '\0') CONWrite(*s++);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
