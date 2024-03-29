// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      editor.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      29th March 2024
//      Reviewed :  No
//      Purpose :   System Editor Code, main entry points.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//									Editor state variables
//
// ***************************************************************************************

uint8_t 	edState; 																// Current state
uint16_t 	edLineCount; 															// Total number of lines in the edited object
uint8_t  	edWindowTop,edWindowBottom,edWindowLeft,edWindowRight; 					// Editor window
uint8_t 	edXPos,edYPos;  														// Cursor position on screen
uint16_t  	edTopLine;  															// Line# of top line on screen (starting at 1)
uint8_t  	edPendingAction;  														// Action waiting to be performed.

// ***************************************************************************************
//
//							Enter the editor for the first time
//
// ***************************************************************************************

uint8_t EDITInitialise(void) {
	printf("ED:Initialise\n");
	edState = ES_INITIALISE;   														// State will be initialised after call-out.
	return EX_INITIALISE;   														// Go away and get initialised information.
}

// ***************************************************************************************
//
//		   Re-enter the editor after a call-out to perform a specific function
//
// ***************************************************************************************

uint8_t EDITContinue(void) {
	printf("ED:Reenter state %c\n",edState); 
	return EX_EXIT;

}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
