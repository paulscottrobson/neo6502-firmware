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

void TTLInitialise(uint8_t spriteID) {
	printf("TTL:Setup sprite %d\n",spriteID);
}

void TTLRotate(uint16_t turn) {
	printf("TTL:Right %d\n",turn);
}

void TTLMove(uint16_t distance,uint8_t colour) {
	printf("TTL:Move %d %d\n",distance,colour);
}


void TTLHide(void) {
	printf("TTL:Hide\n");
}




// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
