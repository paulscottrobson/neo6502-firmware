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

static void drawCharacter(int x,int y,int ch,int col) {
   for (int y1 = 0;y1 < 7;y1++) {
      int b = font_5x7[ch*7 + y1];
      for (int x1 = 0;x1 < 5;x1++) {
    int p = (x+x1)+(y+y1)*320;
    graphMode->screenMemory[p] = (b & 0x80) ? col : 0;
    b = b << 1;
      }
   }
}

static void CONClearScreen(void) {
   for (int x = 0;x < 320;x++) {                   // Default clear screen
      for (int y = 0;y < 240;y++) {
    	graphMode->screenMemory[x+y*320] = (x >> 4) + (y >> 3) * 16;
    }
	}
}

void CONInitialise(struct GraphicsMode *gMode) {
	graphMode = gMode;	
	CONClearScreen();
}

int xc = 0,yc = 0;

void CONWrite(int c) {
   if (c != 13) {
      drawCharacter(xc*6+5,yc*8,c,7);
      xc += 1;
   } else {
      xc = 52;
   }
   if (xc >= 52) { xc = 0;yc = (yc + 1) % 30; }
}
