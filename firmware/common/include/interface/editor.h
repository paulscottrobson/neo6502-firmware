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
#define ES_INITIALISE 		(0) 					// Initialise state. Triggers initialise call out, switch to repaint
#define ES_REPAINT 			(1) 					// State repaints lines with call out, can be re-entered for multiple lines or switch to edit
#define ES_EDIT 			(2) 					// Editing state ; left when needs updating, line change etc.
#define ES_UPDATE 			(3) 					// Update state, call out updates changed lines, 
#define ES_DISPATCH			(4) 					// Performs required code needed driven by reason for leaving edit state.
#define ES_EXIT 			(5) 					// Exit state

void EDITInitialise(uint16_t handler);
uint8_t EDITContinue(void);

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
