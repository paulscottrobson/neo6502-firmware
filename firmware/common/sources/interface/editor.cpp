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

#define CPARAMS 	(cpuMemory+controlPort+4)
#define EPRINTF 	printf 

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
uint16_t  	edRepaintY,edRepaintYLast;  											// Repainting tracker.
uint8_t  	edCurrentIndent = 0; 													// Current line indent

// ***************************************************************************************
//
//							Enter the editor for the first time
//
// ***************************************************************************************

uint8_t EDITInitialise(void) {
	EPRINTF("ED:Initialise\n");
	edState = ES_INITIALISE;   														// State will be initialise after call-out.
	return EX_INITIALISE;   														// Go away and get initialised information.
}

// ***************************************************************************************
//
//								Check if repaint required
//
// ***************************************************************************************

static void _EDITScrollTopLine(uint16_t newTop) {
	bool repaint = true;
	if (repaint) {
		edTopLine = newTop;  														// New top line
		edRepaintY = edWindowTop; 													// Repaint from here.
		edRepaintYLast = edWindowBottom; 											// To here.
		EPRINTF("ED:repaint from %d-%d\n", edRepaintY,edRepaintYLast);
	}
}

// ***************************************************************************************
//
//									Set up editor state
//
// ***************************************************************************************

static uint8_t _EDITStateInitialise(void) {
	edLineCount = CPARAMS[0]+(CPARAMS[1] << 8);  									// Number of lines.
	EPRINTF("ED:Init:has %d lines\n",edLineCount);
	edWindowTop = 0;edWindowLeft = 0;edWindowRight = gMode.xCSize-1;  				// Work out edit window.
	edWindowBottom = gMode.yCSize-2;
	edXPos = 0;edYPos = 0;  														// Cursor position
	edTopLine = -1;   																// Illegal top line, forces repaint.
	_EDITScrollTopLine(1);  														// Scroll screen to fit.
	edState = ES_REPAINT; 															// And go to repaint state
	return EX_NOCALLBACK;
}

// ***************************************************************************************
//
//									Repaint state
//
// ***************************************************************************************

static uint8_t _EDITStateRepaint(void) {
	edCurrentIndent = 0;  															// Start of line.
	if (edRepaintY > edRepaintYLast) {  											// Repaint completed , switch to editing state
		edState = ES_EDIT;  													
		return EX_NOCALLBACK; 
	}

	int line = edRepaintY+edTopLine;  												// Index of line to repaint.
	if (line <= edLineCount) {  													// In program space
		EPRINTF("ED:Repaint:Repainting line %d to Y %d\n",line,edRepaintY);
		CPARAMS[0] = line & 0xFF;  													// Line to get to display on callback.
		CPARAMS[1] = line >> 8;
		edState = ES_PAINTER;  														// Switch to painter state
		return EX_GETLINE;  														// Retrieve on callback
	} else {
		CONSetCursorPosition(edWindowLeft,edRepaintY);  							// Erase that line.
		for (int i = 0;i <= edWindowRight-edWindowLeft;i++) CONWrite(' ');
		EPRINTF("ED:Repaint:Blank %d\n",edRepaintY);
		edRepaintY++;
		return EX_NOCALLBACK;  														// Keep going.
	}
}

// ***************************************************************************************
//
//			State : have tokenised line, display it and it is in the edit buffer
//
// ***************************************************************************************

static uint8_t _EDITStatePainter(void) {
	uint8_t x,y;
	CONSetCursorPosition(edWindowLeft,edRepaintY);  								// Set cursor position	
	CONWriteHex(edRepaintY);
	CONGetCursorPosition(&x,&y);while (x++ <= edWindowRight) CONWrite(' ');   		// Blank the rest of the line
	edRepaintY++;   																// Next line
	edState = ES_REPAINT;  															// Go back to repaint state
	return EX_NOCALLBACK;
}

// ***************************************************************************************
//
//		   Re-enter the editor after a call-out to perform a specific function
//
// ***************************************************************************************

uint8_t EDITContinue(void) {
	uint8_t func;
	do {
		func = EX_EXIT;
		EPRINTF("ED:Reenter state %c\n",edState); 
		switch(edState) {
			case ES_INITIALISE:
				func = _EDITStateInitialise();break;
			case ES_REPAINT:
				func = _EDITStateRepaint();break;
			case ES_PAINTER:
				func = _EDITStatePainter();break;
			case ES_EDIT:
				break;
		}
	} while (func == EX_NOCALLBACK);
	return func;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************

// TODO: Detokenising code to access buffer
// TODO: Display that code , coloured ?
// TODO: Editing
// TODO: Exit commands (e.g. up down CR initially)
// TODO: Write back
