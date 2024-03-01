// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      pininterface.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st February 2024
//      Reviewed :  No
//      Purpose :   Pin Interface code.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//					  UEXT -> IOPIN mapping, from the Schematic
//
// ***************************************************************************************

#define IOPIN_MIN	(3)  															// Pin Range
#define IOPIN_MAX	(10)

static int GPIOMapping[11] = {
	 0, 0, 0,28,29, 																// Pin 0-4
	23,22,24,27,26, 																// Pin 5-9
	25 																				// Pin 10
};

static bool IOPINOutputLatch[IOPIN_MAX+1]; 											// State of output latch
static int  IOPINPinDirection[IOPIN_MAX+1]; 										// What the pin direction is set to.
static bool IOPINDisabled[IOPIN_MAX+1];  											// Pin usage disabled
static bool IOI2CInitialised = false; 												// True if I2C initialised.

// ***************************************************************************************
//
//						Convert a Pin ID to a Physical IOPIN Pin
//
// ***************************************************************************************

static int IOMapPinToGPIO(int pinID) {
	if (pinID < IOPIN_MIN || pinID > IOPIN_MAX) return -1; 							// Out of range
	if (GPIOMapping[pinID] == 0) return -1;  										// Pin not present
	if (IOPINDisabled[pinID]) return -1;  											// Disabled (probably in use by SPI, Serial, I2C)
	return GPIOMapping[pinID];
}

// ***************************************************************************************
//
//				Initialise - all valid pins set to input, latch cleared
//
// ***************************************************************************************

void IOInitialise(void) {
	for (int i = IOPIN_MIN;i <= IOPIN_MAX;i++) {  									// Scan range
		if (GPIOMapping[i] != 0) { 													// Pin exists ?
			IOPINDisabled[i] = false;  												// Not disabled
			IOPINOutputLatch[i] = false; 											// Clear O/P Latch
			IOPINPinDirection[i] = UEXT_INPUT;   									// Remember current direction.
			UEXTSetGPIODirection(GPIOMapping[i],UEXT_INPUT); 						// Set port to input. 
		}
	}
	IOI2CInitialised = false;
}

// ***************************************************************************************
//
//								Set pin direction
//
// ***************************************************************************************

int IOSetDirection(int pinID,int pinType) {
	int gpio = IOMapPinToGPIO(pinID);  												// Map it
	if (pinType == UEXT_ANALOGUE && !UEXT_IS_GPIO_ANALOGUE(gpio)) return 1;  		// Only certain GPIO do ADC.
	if (gpio > 0) {  																// Mapping okay
		UEXTSetGPIODirection(gpio,pinType);  										// Set direction to whatever
		IOPINPinDirection[pinID] = pinType;  										// Save direction
		if (pinType == UEXT_OUTPUT) UEXTSetGPIO(gpio,IOPINOutputLatch[pinID]); 		// If set to Output put output latch on pin.
	}	
	return (gpio > 0) ? 0 : 1;  													// 0 if okay,1 if bad
}

// ***************************************************************************************
//
//
// ***************************************************************************************

int IOWrite(int pinID,bool isHigh) {
	int gpio = IOMapPinToGPIO(pinID);  												// Map it
	if (gpio > 0) {  																// Mapping okay
		IOPINOutputLatch[pinID] = isHigh; 											// Save our output latch.
		if (IOPINPinDirection[pinID] == UEXT_OUTPUT) {  							// If actually output
			UEXTSetGPIO(gpio,isHigh);  												// Write to the physical hardware
		}
	}
	return (gpio > 0) ? 0 : 1;  													// 0 if okay,1 if bad
}

// ***************************************************************************************
//
//											Read GPIO pin
//
// ***************************************************************************************

int IORead(int pinID,bool *pIsHigh) {
	int gpio = IOMapPinToGPIO(pinID);  												// Map it
	if (gpio > 0) {  																// Mapping okay
		if (IOPINPinDirection[pinID] == UEXT_INPUT) {  								// Is input
			UEXTGetGPIO(gpio,pIsHigh);  											// Return the actual pin value
		} else {  																	// Is output
			*pIsHigh = IOPINOutputLatch[pinID]; 									// Return the current latch value
		}
	}
	return (gpio > 0) ? 0 : 1;  													// 0 if okay,1 if bad
}

// ***************************************************************************************
//
//								Read GPIO pin Analogue
//
// ***************************************************************************************

int IOReadAnalogue(int pinID,uint16_t *pLevel) {
	int gpio = IOMapPinToGPIO(pinID);  												// Map it
	if (UEXT_IS_GPIO_ANALOGUE(gpio)) {  											// Mapping okay
		if (IOPINPinDirection[pinID] != UEXT_ANALOGUE) return 2;  					// Not set to analogue input.
		UEXTGetGPIOAnalogue(gpio,pLevel);  											// Read it.
	}	
	return (UEXT_IS_GPIO_ANALOGUE(gpio)) ? 0 : 1;  									// 0 if okay,1 if bad
}

// ***************************************************************************************
//
//				Initialise I2C system if not already initialised
//
// ***************************************************************************************

static void IOI2CInitialise(void) {
	if (!IOI2CInitialised) {  														// If not initialised
		IOI2CInitialised = true;  													// Mark initialised
		UEXTI2CInitialise();  														// Set it up
		IOPINDisabled[3] = IOPINDisabled[4] = true; 								// Disable SCL/SDA from GPIO usage.
	}
}

// ***************************************************************************************
//
//								Write to i2C Register
//
// ***************************************************************************************

int IOI2CWriteRegister(uint8_t device,uint8_t reg,uint8_t data) {
	uint8_t buffer[2];
	IOI2CInitialise();
	buffer[0] = reg;buffer[1] = data;
	return UEXTI2CWriteBlock(device,buffer,2);
}

// ***************************************************************************************
//
//								Read from i2c Register
//
// ***************************************************************************************

int IOI2CReadRegister(uint8_t device,uint8_t reg,uint8_t *pData) {
	IOI2CInitialise();
	int e =UEXTI2CWriteBlock(device,&reg,1);
	if (e == 0) e = UEXTI2CReadBlock(device,pData,1);
	return e;
}

// ***************************************************************************************
//
//								  Read/write mappers
//
// ***************************************************************************************

int IOI2CReadBlock(uint8_t device, uint8_t *data,size_t size) {
	IOI2CInitialise();
	return UEXTI2CReadBlock(device,data,size);
}

int IOI2CWriteBlock(uint8_t device, uint8_t *data,size_t size) {
	IOI2CInitialise();
	return UEXTI2CWriteBlock(device,data,size);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
