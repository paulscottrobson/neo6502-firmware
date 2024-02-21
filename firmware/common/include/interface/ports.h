// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      ports.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      20th February 2024
//      Reviewed :  No
//      Purpose :   UEXT interfacing code
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _UEXT_H
#define _UEXT_H

#define UEXT_INPUT 			(1)
#define UEXT_OUTPUT 		(2)

void IOInitialise(void);
int IOSetDirection(int pinID,int pinType);
int IOWrite(int pinID,bool isHigh);
int IORead(int pinID,bool *pIsHigh);

int UEXTSetGPIODirection(int gpio,int pinType);
int UEXTSetGPIO(int gpio,bool isOn);
int UEXTGetGPIO(int gpio,bool *pIsHigh);

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
