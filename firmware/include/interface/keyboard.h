// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      keyboard.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st November 2023
//      Reviewed :  No
//      Purpose :   Keyboard prototypes
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

void KBDInitialise(void);
void KBDSync(void);

void KBDEvent(uint8_t isDown,uint8_t keyCode,uint8_t modifiers);

#endif

