// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		serial.h
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		24th January 2024
//		Reviewed :	No
//		Purpose :	Serial functions
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _SERIALHARDWARE_H
#define _SERIALHARDWARE_H

#define SERIAL_PROTOCOL_8N1 	(0)

#define SERIAL_TRANSFER_BAUD_RATE 230400

bool SERInitialise(void);
void SERSetSerialFormat(uint32_t baudRate,uint32_t protocol);
bool SERIsByteAvailable(void);
uint8_t SERReadByte(void);
void SERWriteByte(uint8_t b);
void SERCheckDataAvailable(void);

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************

