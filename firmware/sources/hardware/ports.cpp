// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      ports.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      20th February 2024
//      Reviewed :  No
//      Purpose :   UEXT Handling code.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//					  UEXT -> GPIO mapping, from the Schematic
//
// ***************************************************************************************

static int GPIOMapping[11] = {
	 0, 0, 0,28,29, 																// Pin 0-4
	23,22,24,27,26, 																// Pin 5-9
	25 																				// Pin 10
};

static bool isInitialised = false;

static int GPIODirection[11];  														// Current set direction.

#define DIR_NONE 	(0)
#define DIR_INPUT 	(1)
#define DIR_OUTPUT 	(2)

// ***************************************************************************************
//
//							Initialise all ports on first usage
//
// ***************************************************************************************

static void _UEXTInitialise(void) {
	if (!isInitialised) {
		isInitialised = true;
		for (int i = 0;i <= 10;i++) {
			if (GPIOMapping[i] > 0) gpio_init(GPIOMapping[i]);
			GPIODirection[i] = DIR_NONE;
		}
	}
}

// ***************************************************************************************
//
//										Write to GPIO
//
// ***************************************************************************************

int UEXTSetGPIO(int pinID,bool isOn) {
	if (pinID < 3 || pinID > 10) return 1; 											// You can't set the power supply :)
	_UEXTInitialise();
	int gpioID = GPIOMapping[pinID]; 												// Make into a GPIO #
	if (GPIODirection[pinID] != DIR_OUTPUT) {
	    gpio_set_dir(gpioID, GPIO_OUT);  											// Set direction if not already output
	    GPIODirection[pinID] = DIR_OUTPUT;
	}
    gpio_put(gpioID,isOn ? 1 : 0);  												// Set pin
	return 0;
}

// ***************************************************************************************
//
//									   Read from GPIO
//
// ***************************************************************************************

int UEXTGetGPIO(int pinID,bool *pIsHigh) {
	if (pinID == 1 || pinID == 2) {  												// Power lines.
		*pIsHigh = (pinID == 1);													// 3.3v and 0v.
		return 0;
	}
	if (pinID < 3 || pinID > 10) return 1; 											// Legitimate pins.
	int gpioID = GPIOMapping[pinID];  												// Make into a GPIO #
	_UEXTInitialise();
	if (GPIODirection[pinID] != DIR_INPUT) {
	    gpio_set_dir(gpioID, GPIO_IN);  											// Set direction if not already input
	    GPIODirection[pinID] = DIR_INPUT;
	}
    *pIsHigh = gpio_get(gpioID);  													// Get pin status
	return 0;
}