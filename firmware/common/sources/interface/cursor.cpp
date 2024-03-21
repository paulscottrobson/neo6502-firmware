// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      cursor.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st March 2024
//      Reviewed :  No
//      Purpose :   Mouse cursor code.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "data/cursors.h"

static uint16_t xCursor,yCursor; 												// Current position
static bool isCursorVisible; 													// True if visible

// ***************************************************************************************
//
//								Initialise mouse cursor
//
// ***************************************************************************************

void CURInitialise(void) {
	xCursor = yCursor = -1;isCursorVisible = false;
	xCursor = 32;yCursor = 48;isCursorVisible = true; 
}

// ***************************************************************************************
//
//								Update cursor position
//
// ***************************************************************************************

void CURSetPosition(uint16_t x,uint16_t y) {
	xCursor = x;yCursor = y;
}

// ***************************************************************************************
//
//								Update cursor visibility
//
// ***************************************************************************************

void CURSetVisible(bool isVisible) {
	isCursorVisible = isVisible;
}

// ***************************************************************************************
//
//								Get cursor draw information
//
// ***************************************************************************************

bool CURGetCursorDrawInformation(const uint8_t **pData,uint16_t *pX,uint16_t *pY) {
	*pData = default_cursor_data; 												// Just one cursor
	*pX = xCursor;*pY = yCursor; 												// This is top left - so crosshairs will need adjusting for example.
	return isCursorVisible && xCursor < gMode.xGSize && yCursor < gMode.yGSize; // On and on screen.
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
