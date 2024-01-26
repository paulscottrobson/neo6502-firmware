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

static uint8_t turtleSpriteID = 0xFF; 											// ID of sprite
static int turtleRotate; 														// Turtle rotation
static int xTurtle,yTurtle; 													// Position

#define TURTLE_COLOUR 	(3) 													// Yellow turtle

// ***************************************************************************************
//
//									Turtle initialise
//
// ***************************************************************************************

void TTLInitialise(uint8_t spriteID) {
	printf("TTL:Setup sprite %d\n",spriteID);
	turtleSpriteID = spriteID;
	turtleRotate = 0;
	xTurtle = gMode.xGSize / 2;yTurtle = gMode.yGSize/2;
	SPRSetTurtleSprite(turtleSpriteID,turtleRotate,TURTLE_COLOUR);
}

// ***************************************************************************************
//
//							Rotate turtle right by turn degrees
//
// ***************************************************************************************

void TTLRotate(int16_t turn) {
	printf("TTL:Right %d\n",turn);
	if (turtleSpriteID == 0xFF) return;
	int rotate = abs(turn) % 360;  												// Force into +/- 0..359 range.
	turtleRotate = (turtleRotate + (turn < 0 ? -rotate : rotate)) % 360;  		// Work out the new angle
	TTLMove(0); 																// Rotate but no move
}

// ***************************************************************************************
//
//									Move turtle by n.
//
// ***************************************************************************************

static SUINFO SUITurtle;

void TTLMove(int16_t distance) {
	printf("TTL:Move %d\n",distance);
	if (turtleSpriteID == 0xFF) return;
	int xStart = xTurtle,yStart = yTurtle;
	if (distance != 0) {
		double radians = - (turtleRotate - 90) / (2 * 3.14159);  				// Convert to radians.
		xTurtle += cos(radians) * distance; 									// New position
		yTurtle += sin(radians) * distance;
		GFXFastLine(&gMode,xStart,yStart,xTurtle,yTurtle); 						// Draw it.
	}
	SPRSetTurtleSprite(turtleSpriteID,turtleRotate,TURTLE_COLOUR); 				// Rotate it

	SUITurtle.spriteID = turtleSpriteID;  										// Prepare to update sprite
	SUITurtle.x = xTurtle;SUITurtle.y = yTurtle;
	SUITurtle.imageSize = 16;SUITurtle.flip = SUITurtle.anchor = 0;
	SPRUpdate((uint8_t *)&SUITurtle); 											// Update, draw etc.
}

// ***************************************************************************************
//
//									Hide turtle
//
// ***************************************************************************************

void TTLHide(void) {
	printf("TTL:Hide\n");
	if (turtleSpriteID != 0xFF) {
		SUITurtle.spriteID = turtleSpriteID;  									// Nothing else is used to hide
		SPRHide((uint8_t *)&SUITurtle);
	}
}




// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
