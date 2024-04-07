// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      mouse.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Sascha Schneider (schneider.sascha@mein.gmx)
//      Date :      3rd April 2024
//      Reviewed :  No
//      Purpose :   Mouse code.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "data/cursors.h"

static uint16_t xCursor, yCursor;                                                // Current position
static bool isCursorVisible;                                                     // True if visible
static uint8_t buttonState;
static uint8_t scrollWheelState;

// ***************************************************************************************
//
//                              Initialise mouse
//
// ***************************************************************************************

void MSEInitialise(void) {
    xCursor = yCursor = 0;
    buttonState = 0;
    scrollWheelState = 0;
    isCursorVisible = false;
}

// ***************************************************************************************
//
//                              Set cursor position
//
// ***************************************************************************************

void MSESetPosition(uint16_t x, uint16_t y) {
    xCursor = x;
    yCursor = y;
}

void MSEOffsetPosition(int8_t dx, int8_t dy) {
    if(dx < 0 && xCursor < abs(dx)) {
        xCursor = 0;
    } else {
        xCursor += dx;
    }
    if(dy < 0 && yCursor < abs(dy)) {
        yCursor = 0;
    } else {
        yCursor += dy;
    }

    if(xCursor > gMode.xGSize) xCursor = gMode.xGSize;
    if(yCursor > gMode.yGSize) yCursor = gMode.yGSize;
}

// ***************************************************************************************
//
//                              Update cursor visibility
//
// ***************************************************************************************

void MSESetVisible(bool isVisible) {
    isCursorVisible = isVisible;
}

// ***************************************************************************************
//
//                              Get cursor draw information
//
// ***************************************************************************************

bool MSEGetCursorDrawInformation(const uint8_t **pData, uint16_t *pX, uint16_t *pY) {
    *pData = default_cursor_data;                                               // Just one cursor
    *pX = xCursor; *pY = yCursor;                                               // This is top left - so crosshairs will need adjusting for example.
    return isCursorVisible && xCursor < gMode.xGSize && yCursor < gMode.yGSize; // On and on screen.
}

// ***************************************************************************************
//
//                              Update mouse scroll wheel state
//
// ***************************************************************************************

void MSEUpdateScrollWheel(int8_t ds) {
    scrollWheelState += ds;
}

// ***************************************************************************************
//
//                              Update mouse button state
//
// ***************************************************************************************

void MSEUpdateButtonState(uint8_t bs) {
    buttonState = bs;
}

// ***************************************************************************************
//
//                              Get mouse position, button state and scroll wheel state
//
// ***************************************************************************************

void MSEGetState(uint16_t *pX, uint16_t *pY, uint8_t *pButtonState, uint8_t *pScrollWheelState) {
    *pX = xCursor;
    *pY = yCursor;
    *pButtonState = buttonState;
    *pScrollWheelState = scrollWheelState;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
