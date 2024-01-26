// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      serialmanager.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      25th January 2024
//      Reviewed :  No
//      Purpose :   Serial interface communication.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

//static void SERCommand(uint8_t command,uint8_t *data,uint8_t size);

// ***************************************************************************************
//
//									Input and process buffer.
//
// ***************************************************************************************

//static uint8_t sBuffer[256];  													// Input buffer.

void SERCheckDataAvailable(void) {
	// if (SERIsByteAvailable()) {   												// Something waiting.
	// 	sBuffer[0] = SERReadByte();  											// Read length of data 
	// 	sBuffer[1] = SERReadByte();  											// Read checksum
	// 	uint8_t checksum = 0;
	// 	for (int16_t i = 0;i < sBuffer[0];i++) {  								// Read in data and calculate checksum
	// 		sBuffer[i+2] = SERReadByte();
	// 		checksum += sBuffer[i+2];			
	// 	}
	// 	if (sBuffer[1] == checksum) {  											// Checksum okay.
	// 		SERCommand(sBuffer[2],sBuffer+3,sBuffer[0]-1); 						// Do command.
	// 	} else {
	// 		CONWriteString("Serial checksum error.\r");  
	// 	}
	// }
}

// ***************************************************************************************
//
//									Handle serial command
//
// ***************************************************************************************

// static uint8_t *dataPtr = NULL;  												// Where we write stuff
// static uint16_t startAddress,currentAddress;  									// Start and current address accessing.

// static void SERCommand(uint8_t command,uint8_t *data,uint8_t size) {
// 	uint16_t a;

// 	switch (command) {
// 		case 0:  																// 0 detach
// 			break;
// 		case 1:  																// 1 data transmit
// 			if (dataPtr != NULL) {  											// Address assigned ?
// 				for (uint8_t i = 0;i < size;i++) { 								// Copy data if so
// 					*dataPtr++ = data[i];
// 					currentAddress++;
// 				}
// 			}
// 			break;
// 		case 2: 																// 2 set 6502 memory address
// 		case 3: 																// 3 set gfxmemory address
// 			currentAddress = startAddress = data[0] + (data[1] << 8);
// 			dataPtr = (command == 2) ? cpuMemory+currentAddress:gfxMemory+currentAddress;
// 			break;
// 		case 4:  																// 4 set 6502 memory address indirect
// 			a = data[0] + (data[1] << 8);  										// Get address from here.
// 			currentAddress = startAddress = cpuMemory[a] + (cpuMemory[a+1]<<8); // Get the actual address
// 			dataPtr = cpuMemory + currentAddress;   							// Initialise the pointer
// 			break;
// 		case 5:
// 			KBDInsertQueue(data[0]);  											// 5 Insert key in keyboard queue
// 			break;
// 	}
// }

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
