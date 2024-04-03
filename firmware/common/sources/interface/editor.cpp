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

#define ISDISPLAYABLE(c) ((c) < 0x80 || (c) >= 0xC0)

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
uint8_t  	edCurrentIndent; 														// Current line indent
uint8_t  	edCurrentLine[256],edCurrentSize;  										// Current line text
bool  		edCursorShown;  														// Cursor visible
bool  		edLineChanged;  														// True when current line changed

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
		edRepaintY = 0; 															// Repaint from here.
		edRepaintYLast = edWindowBottom-edWindowTop; 								// To here.
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
	edWindowTop = 1;edWindowLeft = 0;edWindowRight = gMode.xCSize-1;  				// Work out edit window.
	edWindowBottom = gMode.yCSize-2;
	edXPos = 0;edYPos = 0;  														// Cursor position
	edTopLine = -1;   																// Illegal top line, forces repaint.
	edCurrentIndent = 0;
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
	int line;
	if (edRepaintY > edRepaintYLast) {  											// Repaint completed , switch to editing state
		edState = ES_LOADLINE;  													// First loading the displayed line
		line = edTopLine+edYPos;  													// Current line to edit.
		CPARAMS[1] = line & 0xFF;  													// Line to get to display on callback.
		CPARAMS[2] = line >> 8;
		return EX_GETLINE; 
	}
	line = edRepaintY+edTopLine;  													// Index of line to repaint.
	if (line <= edLineCount) {  													// In program space
		EPRINTF("ED:Repaint:Repainting line %d to Y %d\n",line,edRepaintY);
		CPARAMS[1] = line & 0xFF;  													// Line to get to display on callback.
		CPARAMS[2] = line >> 8;
		edState = ES_PAINTER;  														// Switch to painter state
		return EX_GETLINE;  														// Retrieve on callback
	} else {
		EPRINTF("ED:Repaint:Blank %d\n",edRepaintY);
		CONSetCursorPosition(edWindowLeft,edRepaintY+edWindowTop); 					// Erase that line.
		for (int i = 0;i <= edWindowRight-edWindowLeft;i++) CONWrite(' ');
		edRepaintY++;
		return EX_NOCALLBACK;  														// Keep going.
	}
}

// ***************************************************************************************
//
//						Repaint a coloured line without an indent
//
// ***************************************************************************************

static void _EDITRepaintLine(uint16_t y) {
	uint8_t x = edWindowRight-edWindowLeft+1;  										// Characters to output.
	uint8_t *s = cpuMemory+CPARAMS[0]+(CPARAMS[1] << 8);
 	CONSetCursorPosition(edWindowLeft,y+edWindowTop);  								// Set cursor position	
	for (uint8_t p = 0;p < *s && x > 0;p++) {
		uint8_t c = s[p+1];
		CONWrite(c);
		if (ISDISPLAYABLE(c)) x--;
	}
	while (x-- > 0) CONWrite(' '); 													// Blank rest of line.
}

// ***************************************************************************************
//
//						Repaint a plain line with an indent
//
// ***************************************************************************************

static void _EDITRepaintEditLine(uint16_t y) {
	uint8_t x = edWindowRight-edWindowLeft+1;  										// Characters to output.
	if (edCurrentSize < edWindowRight-edWindowLeft+1) edCurrentIndent = 0;  		// Current indent forced zero.
	uint8_t p = edCurrentIndent;  													// Start from here.
	uint8_t count = edCurrentSize-p;  												// Max to output
 	CONSetCursorPosition(edWindowLeft,y+edWindowTop);  								// Set cursor position	
 	CONWrite(0x85);  																// Line editing colour
	while (x > 0 && count > 0) {
		CONWrite(edCurrentLine[p++]);count--;x--;
	}
	while (x-- > 0) CONWrite(' '); 													// Blank rest of line.
	edCursorShown = false;  														// Cursor not displayed.
}	


// ***************************************************************************************
//
//			State : have tokenised line, display it and it is in the edit buffer
//
// ***************************************************************************************

static uint8_t _EDITStatePainter(void) {
	_EDITRepaintLine(edRepaintY);
	edRepaintY++;   																// Next line
	edState = ES_REPAINT;  															// Go back to repaint state
	return EX_NOCALLBACK;
}

// ***************************************************************************************
//
//								Load line into buffer
//
// ***************************************************************************************

static uint8_t _EDITStateLoadLine(void) {
	uint8_t *s = cpuMemory+CPARAMS[0]+(CPARAMS[1]<<8);  							// Line to read.
	edCurrentSize = 0;
	for (uint8_t i = 0;i < *s;i++) {
		uint8_t c = s[i+1];
		if (ISDISPLAYABLE(c)) edCurrentLine[edCurrentSize++] = c; 					// Read in printables only.
	}
	EPRINTF("ED:LoadLine %d size %d\n",edTopLine+edYPos,edCurrentSize);
	edLineChanged = false;  														// Line not changed.
	_EDITRepaintEditLine(edYPos);  													// Repaint current line.
	edState = ES_EDIT;																// Switch to edit state requesting keyboard.
	return EX_GETKEY;
}

// ***************************************************************************************
//
//								Handle basic keystrokes
//
// ***************************************************************************************

static bool _EDITBasicKeyHandler(uint8_t c) {
	bool isProcessed = true;
	switch(c) {  																	// Deal with left, right, tab, delete, backspace.
		default:
			isProcessed = false;break;
	}
	return isProcessed;
}

// ***************************************************************************************
//
//						Get key, process basic functionality
//
// ***************************************************************************************

static uint8_t _EDITStateEdit(void) {
	if (CPARAMS[0] == 0) {  														// No key yet ?
		if (!edCursorShown) { 
			CONSetCursorPosition(edWindowLeft+edXPos,edWindowTop+edYPos);
			CONWrite(CC_REVERSE);
			edCursorShown = true;
		}
		return EX_GETKEY;
	}
	uint8_t key = CPARAMS[0];  														// Key received
	if (edCursorShown) {    														// Hide cursor
		CONSetCursorPosition(edWindowLeft+edXPos,edWindowTop+edYPos);
		CONWrite(CC_REVERSE);
		edCursorShown = false;
	}
	if (_EDITBasicKeyHandler(key)) {  												// Basic actions in this line.
		_EDITRepaintEditLine(edYPos);  												// Repaint the edit line.
		return EX_GETKEY;  															// Get the next key
	}
	// TODO: Leaving the line.
	// TODO: Write back if changed, otherwise switch to execute command, saving pending command.
	return EX_EXIT;
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
			case ES_LOADLINE:
				func = _EDITStateLoadLine();break;
				break;
			case ES_EDIT:
				func = _EDITStateEdit();break;
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

// TODO: Editing left right insert etc.
// TODO: Exit commands (e.g. up down CR initially)
// TODO: Write back
