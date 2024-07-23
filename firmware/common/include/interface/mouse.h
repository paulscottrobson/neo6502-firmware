// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      cursor.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st March 2024
//      Reviewed :  No
//      Purpose :   Mouse Cursor functionality
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _MOUSE_H
#define _MOUSE_H

void MSEInitialise(void);
void MSESetPosition(uint16_t x, uint16_t y);
void MSEOffsetPosition(int8_t dx, int8_t dy);
void MSEUpdateScrollWheel(int8_t ds);
void MSEUpdateButtonState(uint8_t buttonState);
void MSEGetState(uint16_t *pX, uint16_t *pY, uint8_t *pButtonState, uint8_t *pScrollWheelState);
bool MSEGetCursorDrawInformation(uint16_t *pX, uint16_t *pY);
void MSESetVisible(bool isVisible);
void MSEEnableMouse(void);
bool MSEMousePresent(void);

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
