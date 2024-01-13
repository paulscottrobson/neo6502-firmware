// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      dispatch.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      22nd November 2023
//      Reviewed :  No
//      Purpose :   Message dispatcher
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _DISPATCH_H
#define _DISPATCH_H

void DSPHandler(uint8_t *cBlock,uint8_t *memory);
void DSPSync(void);
void DSPReset(void);
char *DSPGetString(uint8_t *command,uint8_t paramOffset);
uint16_t DSPGetInt16(uint8_t *command,uint8_t paramOffset);
#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
