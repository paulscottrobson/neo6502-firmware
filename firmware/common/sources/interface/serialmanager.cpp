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

static bool SERCommand(uint8_t command,uint8_t *data,uint8_t size);

// ***************************************************************************************
//
//									Input and process buffer.
//
// ***************************************************************************************

static uint8_t sBuffer[256];  														// Input buffer.
static bool isInitialised = false;

void SERCheckDataAvailable(void) {
	if (!isInitialised) {  															// Try to initialise it.
	    isInitialised = SERInitialise();                                         	// Initialise serial port.
	    if (!isInitialised) {
	    	CONWriteString("Serial not functioning.\r");  							// Failed to initialise.
	    	return; 												
	    }
	    while (SERIsByteAvailable()) SERReadByte();  								// Clear anything already incoming.
	}
	SERSetSerialFormat(SERIAL_TRANSFER_BAUD_RATE,SERIAL_PROTOCOL_8N1); 				// Set transceive format.
	
	bool completed = false;
	CONWriteString("Serial link enabled.\r");
	while (!completed) {
		KBDSync();  																// Update USB stuff 
		if (cpuMemory[controlPort+3] & 0x80) {  									// ESC pressed ?
			completed = true;
		}
		if (SERIsByteAvailable()) {   												// Something waiting.
			sBuffer[0] = SERReadByte();  											// Read length of data
			sBuffer[1] = SERReadByte();  											// Read checksum
			uint8_t checksum = 0;
			for (int16_t i = 0;i < sBuffer[0];i++) {  								// Read in data and calculate checksum
				sBuffer[i+2] = SERReadByte();
				checksum += sBuffer[i+2];			
			}
			if (sBuffer[1] == checksum) {  											// Checksum okay.
				completed = SERCommand(sBuffer[2],sBuffer+3,sBuffer[0]-1); 			// Do command.
			} else {
				CONWriteString("Serial checksum error.\r");  
				completed = true;
			}
		}
	}
	CONWriteString("Serial link disabled.\r");
}

// ***************************************************************************************
//
//									Handle serial command
//
// ***************************************************************************************

static uint8_t *dataPtr = NULL;  												// Where we write stuff
static uint16_t startAddress,currentAddress;  									// Start and current address accessing.
static uint16_t saveAddress,saveSize;  											// For writing.
static char fileName[32]; 														// File name

static bool SERCommand(uint8_t command,uint8_t *data,uint8_t size) {
	uint16_t a;
	switch (command) {
		case 0:  																// 0 detach
			break;
		case 1:  																// 1 data transmit
			if (dataPtr != NULL) {  											// Address assigned ?
				for (uint8_t i = 0;i < size;i++) { 								// Copy data if so
					*dataPtr++ = data[i];
					currentAddress++;
				}
			}
			break;
		case 2: 																// 2 set 6502 memory address
		case 3: 																// 3 set gfxmemory address
			currentAddress = startAddress = data[0] + (data[1] << 8);
			dataPtr = (command == 2) ? cpuMemory+currentAddress:gfxObjectMemory+currentAddress;
			break;
		case 4:  																// 4 set 6502 memory address indirect
			a = data[0] + (data[1] << 8);  										// Get address from here.
			currentAddress = startAddress = cpuMemory[a] + (cpuMemory[a+1]<<8); // Get the actual address
			dataPtr = cpuMemory + currentAddress;   							// Initialise the pointer
			break;
		case 5:
			KBDInsertQueue(data[0]);  											// 5 Insert key in keyboard queue
			break;
		case 6:  																// 6 Set save address
			saveAddress = data[0]+(data[1] << 8);
			saveSize = data[2]+(data[3] << 8);
			//CONWriteString("%d %d\r",saveAddress,saveSize);
			break;
		case 7:  																// 7 Save file.			
			for (int i = 0;i < data[0];i++) fileName[i] = data[i+1];  			// Make C String
			fileName[data[0]] = '\0';
			FIOWriteFile(fileName,saveAddress,saveSize);   						// Save it
			CONWriteString("Saved '%s' (%d bytes)\r",fileName,saveSize); 		// Announce it.
			break;
	}
	return (command == 0);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
