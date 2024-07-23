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
#define UEXT_ANALOGUE 		(3)

#define UEXT_IS_GPIO_ANALOGUE(n) ((n) >= 26 && (n) <= 29)

void IOInitialise(void);
int IOSetDirection(int pinID,int pinType);
int IOWrite(int pinID,bool isHigh);
int IORead(int pinID,bool *pIsHigh);
int IOReadAnalogue(int pinID,uint16_t *pLevel);

int IOI2CWriteRegister(uint8_t device,uint8_t reg,uint8_t data);
int IOI2CReadRegister(uint8_t device,uint8_t reg,uint8_t *pData);
int IOI2CReadBlock(uint8_t device, uint8_t *data,size_t size);
int IOI2CWriteBlock(uint8_t device, uint8_t *data,size_t size);

int IOSPIReadBlock(uint8_t *data,size_t size);
int IOSPIWriteBlock(uint8_t *data,size_t size);

void IOUARTInitialise(uint32_t baudRate,uint32_t protocol);
int IOUARTReadBlock(uint8_t *data,size_t size);
int IOUARTWriteBlock(uint8_t *data,size_t size);

int UEXTSetGPIODirection(int gpio,int pinType);
int UEXTSetGPIO(int gpio,bool isOn);
int UEXTGetGPIO(int gpio,bool *pIsHigh);
int UEXTGetGPIOAnalogue(int gpio,uint16_t *pLevel);

int UEXTI2CInitialise(void);
int UEXTI2CReadBlock(uint8_t device, uint8_t *data,size_t size);
int UEXTI2CWriteBlock(uint8_t device, uint8_t *data,size_t size);

int UEXTSPIInitialise(void);
int UEXTSPIReadBlock(uint8_t *data,size_t size);
int UEXTSPIWriteBlock(uint8_t *data,size_t size);


#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
