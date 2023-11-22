// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      dispatch.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      22nd November 2023
//      Reviewed :  No
//      Purpose :   Message dispatcher
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"


// ***************************************************************************************
//
//							Handle commands sent by message
//
// ***************************************************************************************

void DSPHandler(uint8_t *cBlock,uint8_t *memory) {

       CONWrite(*cBlock);                                                   // Execute the message (temp)

       *cBlock = 0;																// Clear the message indicating completion.
}

// ***************************************************************************************
//
//     	  Polling code. Called at low repeat rate. About every 64k cycles is right.
//
// ***************************************************************************************

void DSPSync(void) {
    KBDSync();
}

// ***************************************************************************************
//
//     	  		Reset interfaces. Called at start, and also on command 0,0
//
// ***************************************************************************************

void DSPReset(void) {
    const char bootString[] = "NEO6502 Retrocomputer\r\r";
    MEMInitialiseMemory();
    GFXSetMode(0);
    KBDInitialise();
    KBDEvent(0,0xFF,0);                                                         // Reset the keyboard manager
    SNDInitialise();
    const char *c = bootString;
    while (*c != '\0') CONWrite(*c++);	
}
