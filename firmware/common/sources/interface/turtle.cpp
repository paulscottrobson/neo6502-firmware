// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      turtle.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      26th January 2024
//      Reviewed :  No
//      Purpose :   Turtle graphics code
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

static bool isTurtleVisible;													// True if turtle is visible.
static uint8_t turtleSpriteID; 													// ID of sprite
static int turtleRotate; 														// Turtle rotation
static double xTurtle,yTurtle; 													// Position
static uint8_t SUITurtle[8];													// Turtle info.


#define TURTLE_COLOUR 	(3) 													// Yellow turtle

// ***************************************************************************************
//
//									Turtle initialise
//
// ***************************************************************************************

void TTLInitialise(uint8_t spriteID) {
	turtleSpriteID = spriteID;
	isTurtleVisible = false;
	TTLHome();
	TTLUpdate();
}

// ***************************************************************************************
//
//							Rotate turtle right by turn degrees
//
// ***************************************************************************************

void TTLRotate(int16_t turn) {
	int rotate = abs(turn) % 360;  												// Force into +/- 0..359 range.
	turtleRotate = (turtleRotate + (turn < 0 ? -rotate : rotate)) % 360;  		// Work out the new angle
	TTLUpdate();																// Update it.
}

// ***************************************************************************************
//
//								Turtle to Home Position
//
// ***************************************************************************************

void TTLHome(void) {
	turtleRotate = 0;
	xTurtle = gMode.xGSize / 2;yTurtle = gMode.yGSize/2;
	TTLUpdate();																// Update it.
}

// ***************************************************************************************
//
//									Move turtle by n.
//
// ***************************************************************************************

void TTLForward(int16_t distance,uint8_t colour,uint8_t drawLine) {
	int xStart = xTurtle,yStart = yTurtle;
	if (distance != 0) {
		double radians = (turtleRotate+270) * 3.1415926 / 180;  				// Convert to radians.
		xTurtle += cos(radians) * distance; 									// New position
		yTurtle += sin(radians) * distance;
		if (drawLine != 0) {
			GFXSetColour(colour & 0x0F);  										// We have sprites.
			GFXFastLine(&gMode,xStart,yStart,xTurtle,yTurtle); 					// Draw it.
		}
		TTLUpdate();
	}	
}

// ***************************************************************************************
//
//				Update sprite position/rotatation and visibility
//
// ***************************************************************************************

void TTLUpdate(void) {
	if (isTurtleVisible) { 														// Visible
		int x = (int)(xTurtle+0.5);
		int y = (int)(yTurtle+0.5);
		SPRSetTurtleSprite(turtleSpriteID,turtleRotate,TURTLE_COLOUR); 			// Rotate it
		SUITurtle[0] = turtleSpriteID;  										// Prepare to update sprite
		SUITurtle[1] = x & 0xFF;SUITurtle[2] = x >> 8;
		SUITurtle[3] = y & 0xFF;SUITurtle[4] = y >> 8;
		SUITurtle[5] = 16;SUITurtle[6] = SUITurtle[7] = 0;
		SPRUpdate(SUITurtle); 													// Update, draw etc.
	} else {
		SUITurtle[0] = turtleSpriteID;  										// Prepare to hide sprite
		SPRHide(SUITurtle);		
	}
}

// ***************************************************************************************
//
//								Show or Hide turtle
//
// ***************************************************************************************

void TTLSetVisible(bool isVisible) {
	isTurtleVisible = isVisible;
	TTLUpdate();
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//		17/06/24 	Largely rewritten.
//
// ***************************************************************************************
