// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      editor.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      29th March 2024
//      Reviewed :  No
//      Purpose :   Editor header
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _EDITOR_H
#define _EDITOR_H

//
//		States ; it's a sort of FSM, with dropouts.
//
#define ES_INITIALISE 		('I')  					// Called when have initialised state.
#define ES_REPAINT 			('R') 					// State repaints lines with call out, can be re-entered for multiple lines or switch to edit
#define ES_PAINTER  		('P')  					// Do the actual repaint.
#define ES_LOADLINE  		('L')  					// Start editing new line.
#define ES_EDIT 			('E') 					// Editing state ; left when needs updating, line change etc.
#define ES_DISPATCH			('D') 					// Performs required code needed driven by reason for leaving edit state.
#define ES_EXIT 			('X') 					// Exit state

//
//		Call out functions.
//
#define EX_EXIT 			(0)  					// End edit
#define EX_INITIALISE 	 	(1)  					// Set up and count lines in editable object.
#define EX_GETLINE			(2)  					// Get required line into memory in ASCII format detokenising
#define EX_GETKEY  			(3)   					// Get keystroke
#define EX_PUTLINE 			(4)  					// Write line into memory in tokenised form, provided in ASCII
#define EX_NOCALLBACK		(0xFF)

extern uint8_t 		edState; 	
extern uint16_t 	edLineCount;
extern uint8_t  	edWindowTop,edWindowBottom,edWindowLeft,edWindowRight; 				
extern uint8_t 		edXPos,edYPos;  				
extern uint16_t  	edTopLine;  					
extern uint8_t  	edPendingAction;

uint8_t EDITInitialise(void);
uint8_t EDITContinue(void);

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
