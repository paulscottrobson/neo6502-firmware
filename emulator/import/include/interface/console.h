// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      console.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st November 2023
//      Reviewed :  No
//      Purpose :   Console headers
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "interface/graphics.h"

extern const unsigned char font_5x7[];

void CONInitialise(struct GraphicsMode *gMode);
void CONWrite(int c);
void CONWriteHex(uint16_t h);	
void CONWriteString(const char *s);
void CONGetScreenLine(uint16_t addr);

#define CONTROL(c) 	((c) & 0x1F)

#define CC_  		CONTROL('')

#define CC_LEFT 	CONTROL('A')
#define CC_RIGHT 	CONTROL('D')
#define CC_INSERT 	CONTROL('E')
#define CC_PAGEDOWN	CONTROL('F')
#define CC_END 		CONTROL('G')
#define CC_BACKSPACE CONTROL('H')
#define CC_TAB  	CONTROL('I')
#define CC_LF  		CONTROL('J')
#define CC_CLS  	CONTROL('L')
#define CC_ENTER 	CONTROL('M')
#define CC_PAGEUP	CONTROL('R')
#define CC_DOWN 	CONTROL('S')
#define CC_HOME 	CONTROL('T')
#define CC_UP  		CONTROL('W')
#define CC_REVERSE  CONTROL('X')
#define CC_DELETE 	CONTROL('Z')
#define CC_ESC 		CONTROL('[')


#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
