// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      console.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st November 2023
//      Reviewed :  No
//      Purpose :   Console headers
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "interface/graphics.h"

void CONInitialise(struct GraphicsMode *gMode);
void CONWrite(int c);

#endif

