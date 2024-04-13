// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      cursor.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      13th April 2024
//      Reviewed :  No
//      Purpose :   Cursor Repository.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "data/cursors.h"
static uint8_t currentCursor;

// ***************************************************************************************
//
//							Initialise cursors
//
// ***************************************************************************************

void CURInitialise(void) {
	currentCursor = 0;
}

// ***************************************************************************************
//
//							Get current cursor
//
// ***************************************************************************************

const uint8_t *CURGetCurrent(uint8_t *xHit,uint8_t *yHit) {
	if (xHit != NULL) *xHit = cursor_hitPoint[currentCursor*2];
	if (yHit != NULL) *yHit = cursor_hitPoint[currentCursor*2+1];
	return cursor_data+currentCursor*256;
}

// ***************************************************************************************
//
//							Set current cursor
//
// ***************************************************************************************

bool CURSetCurrent(uint8_t cursorID) {
	bool isOk = cursorID < CURSOR_IMAGE_COUNT; 									// Number of cursors
	currentCursor = isOk ? cursorID : 0;  										// Bad value sets default
	return !isOk; 																// Return false on error
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
