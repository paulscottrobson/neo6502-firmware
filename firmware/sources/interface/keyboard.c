// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      keyboard.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st November 2023
//      Reviewed :  No
//      Purpose :   Converts keyboard events to a queue and key state array.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "interface/kbdcodes.h"

// ***************************************************************************************
//
//					Handle a key event. Note keyCode = 0xFF is reset
//
// ***************************************************************************************

void KBDEvent(uint8_t isDown,uint8_t keyCode,uint8_t modifiers) {
	CONWriteHex(isDown);
	CONWriteHex(keyCode);
	CONWriteHex(modifiers);
	CONWrite(13);
}
