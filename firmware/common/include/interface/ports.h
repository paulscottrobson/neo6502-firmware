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

int IOI2CWrite(uint8_t device,uint8_t reg,uint8_t data);
int IOI2CRead(uint8_t device,uint8_t reg,uint8_t *pData);

int UEXTSetGPIODirection(int gpio,int pinType);
int UEXTSetGPIO(int gpio,bool isOn);
int UEXTGetGPIO(int gpio,bool *pIsHigh);

int UEXTI2CInitialise(void);
int UEXTI2CWrite(uint8_t device,uint8_t reg,uint8_t data);
int UEXTI2CRead(uint8_t device,uint8_t reg,uint8_t *pData);

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
