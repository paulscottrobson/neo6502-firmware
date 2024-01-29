// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      locale.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      29th January 2024
//      Reviewed :  No
//      Purpose :   Locale Handling
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "interface/kbdcodes.h"

// ***************************************************************************************
//
//								Locale Data
//
// ***************************************************************************************

static const uint8_t _KBDLocaleData[] = {
	#include "data/neolocale.h"
};

static const uint8_t *_KBDLocaleCurrent = NULL;

// ***************************************************************************************
//
//									Set Locale
//
// ***************************************************************************************

void LOCSetLocale(char c1,char c2) {
	const uint8_t *search = _KBDLocaleData;
	_KBDLocaleCurrent = NULL;
	while (search[0] != 0) {  													// Look through the locale table
		if (c1 == search[1] && c2 == search[2]) _KBDLocaleCurrent = search+3; 	// Found the locale, use it
		search += search[0];													// Follow the list.
	}
}

// ***************************************************************************************
//
//								Process locale fixes
//
// ***************************************************************************************

uint8_t LOCLocaleMapping(uint8_t asciiCode,uint8_t keyCode,uint8_t modifiers) {
	//CONWriteHex(asciiCode);CONWriteHex(keyCode);CONWriteHex(modifiers);CONWrite('\r');

	if (_KBDLocaleCurrent != NULL) {  											// Is there a locale conversion ?	

	}
	return asciiCode;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
