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
//					  UEXT -> GPIO mapping, from the Schematic
//
// ***************************************************************************************

#define GPIO_MIN	(3)  															// Pin Range
#define GPIO_MAX	(10)

static int GPIOMapping[11] = {
	 0, 0, 0,28,29, 																// Pin 0-4
	23,22,24,27,26, 																// Pin 5-9
	25 																				// Pin 10
};

static bool GPIOOutputLatch[GPIO_MAX+1]; 											// State of output latch
static int  GPIOPinDirection[GPIO_MAX+1]; 											// What the pin direction is set to.
static bool GPIODisabled[GPIO_MAX+1];  												// Pin usage disabled

// ***************************************************************************************
//
//						Convert a Pin ID to a Physical GPIO Pin
//
// ***************************************************************************************

static int _IOMapPinToGPIO(int pinID) {
	if (pinID < GPIO_MIN || pinID > GPIO_MAX) return -1; 							// Out of range
	if (GPIOMapping[pinID] == 0) return -1;  										// Pin not present
	if (GPIODisabled[pinID]) return -1;  											// Disabled (probably in use by SPI, Serial, I2C)
	return GPIOMapping[pinID];
}

// ***************************************************************************************
//
//				Initialise - all valid pins set to input, latch cleared
//
// ***************************************************************************************

void IOInitialise(void) {
	for (int i = GPIO_MIN;i <= GPIO_MAX;i++) {  									// Scan range
		if (GPIOMapping[i] != 0) { 													// Pin exists ?
			GPIODisabled[i] = false;  												// Not disabled
			GPIOOutputLatch[i] = false; 											// Clear O/P Latch
			GPIOPinDirection[i] = UEXT_INPUT;   									// Remember current direction.
			UEXTSetGPIODirection(GPIOMapping[i],UEXT_INPUT); 						// Set port to input. 
		}
	}
}

// ***************************************************************************************
//
//								Set pin direction
//
// ***************************************************************************************

int IOSetDirection(int pinID,int pinType) {
	int gpio = _IOMapPinToGPIO(pinID);  											// Map it
	if (gpio > 0) {  																// Mapping okay
		UEXTSetGPIODirection(gpio,pinType);  										// Set direction to whatever
		GPIOPinDirection[gpio] = pinType;  											// Save direction
		if (pinType == UEXT_OUTPUT) UEXTSetGPIO(gpio,GPIOOutputLatch[gpio]);  		// If set to Output put output latch on pin.
	}	
	return (gpio > 0) ? 0 : 1;  													// 0 if okay,1 if bad
}

// ***************************************************************************************
//
//
// ***************************************************************************************

int IOWrite(int pinID,bool isHigh) {
	int gpio = _IOMapPinToGPIO(pinID);  											// Map it
	if (gpio > 0) {  																// Mapping okay
		GPIOOutputLatch[gpio] = isHigh; 											// Save our output latch.
		if (GPIOPinDirection[gpio] == UEXT_OUTPUT) {  								// If actually output
			UEXTSetGPIO(gpio,isHigh);  												// Write to the physical hardware
		}
	}
	return (gpio > 0) ? 0 : 1;  													// 0 if okay,1 if bad
}

// ***************************************************************************************
//
//
// ***************************************************************************************

int IORead(int pinID,bool *pIsHigh) {
	int gpio = _IOMapPinToGPIO(pinID);  											// Map it
	if (gpio > 0) {  																// Mapping okay
		if (GPIOPinDirection[gpio] == UEXT_INPUT) {  								// Is input
			UEXTGetGPIO(gpio,pIsHigh);  											// Return the actual pin value
		} else {  																	// Is output
			*pIsHigh = GPIOOutputLatch[gpio];  										// Return the current latch value
		}
	}
	return (gpio > 0) ? 0 : 1;  													// 0 if okay,1 if bad
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
