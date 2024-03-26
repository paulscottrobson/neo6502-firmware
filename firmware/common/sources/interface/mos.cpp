// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      mos.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      26th March 2024
//      Reviewed :  No
//      Purpose :   MOS code
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

static uint8_t paramCount,stringPos;
static uint8_t *params[4];
static uint8_t paramData[256];

// ***************************************************************************************
//
//								  Unpack parameters
//
// ***************************************************************************************

static uint8_t _MOSGet(uint8_t *cmd) {
	if (stringPos >= *cmd) return '\0'; 										// Nothing left.
	return cmd[++stringPos];
}

static void _MOSUnpackParameters(uint8_t *cmd) {
	uint8_t c,*dataPos = paramData;
	stringPos = 0;paramCount = 0;
	while (cmd[stringPos+1] == '*') stringPos++; 								// Skip *
	while (paramCount < 4) {
		while (c = _MOSGet(cmd),c == ' ') {} 									// Skip spaces
		if (c == '\0') return; 													// Reached the end.
		uint8_t endMarker = ' ';  												// Ends with ' '
		if (c == '\'' || c == '"') {  											// Quote surrounded, either.
			endMarker = c;c = _MOSGet(cmd);  									// Ends with that
		} 
		params[paramCount] = dataPos; 											// Store param position, both length prefixed
		*dataPos++ = 0;  														// and NULL terminated
		do {
			*dataPos++ = c;
			*(params[paramCount]) = *(params[paramCount])+1;
			c = _MOSGet(cmd);  													// Get next command
		} while (c != '\0' && c != endMarker);  								// Until the end.
		*dataPos++ = '\0';
		paramCount++;  															// Bump paarameter count
	}
}

// ***************************************************************************************
//
//								Do provided command
//
// ***************************************************************************************

uint8_t MOSExecute(uint8_t *command) {
	_MOSUnpackParameters(command);
	for (int i = 0;i < paramCount;i++) printf("%d %d [%s]\n",i,*params[i],params[i]+1);
	return 0;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
