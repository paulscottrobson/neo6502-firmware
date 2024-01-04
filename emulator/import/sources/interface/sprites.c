// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sprites.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      4th January 2024
//      Reviewed :  No
//      Purpose :   Sprite drawing code.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

void SPRReset(void) {
	printf("Sprite reset\n");	
}

void SPRHide(uint8_t *paramData) {
	printf("Sprite #%d hiding\n",*paramData);
}

void SPRUpdate(uint8_t *paramData) {
	uint16_t x = paramData[1] + (paramData[2] << 8);
	uint16_t y = paramData[3] + (paramData[4] << 8);
	printf("Sprite #%d to (%d,%d) ImSize:%x Flip:%x\n",*paramData,x,y,paramData[5],paramData[6]);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
