// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      mos.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      26th March 2024
//      Reviewed :  No
//      Purpose :   MOS OSCLI type interface code
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#define MAX_PARAMS 	(4)

static uint8_t paramCount,stringPos;
static uint8_t *params[MAX_PARAMS];
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
	while (paramCount < MAX_PARAMS) {
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

#define CPPPARAM(n) 	std::string((const char *)(params[n]+1))

uint8_t MOSExecute(uint8_t *command) {
	_MOSUnpackParameters(command);  											// Process parameters
	for (int i = 0;i < paramCount;i++) printf("%d %d [%s]\n",i,*params[i],params[i]+1);
	if (paramCount == 0) return 0;  											// Nothing to do.
	const char *cmd = (const char *)(params[0]+1); 								// Command
	uint8_t error = 255;
	//
	//		*. <mask> *cat <mask> list directory
	//
	if (strcmp(cmd,".") == 0 || strcmp(cmd,"cat") == 0) {  				
		FIODirectory(paramCount > 1 ? (const char *)(params[1]+1) : "");
		error = 0;
	}
	//
	//		*del <file> delete file
	//
	if (strcmp(cmd,"del") == 0) {
		if (paramCount == 2) {
			error = FIODeleteFile(CPPPARAM(1));
		}
	}
	//
	//		*file <file> file exists, error if can't open.
	//
	if (strcmp(cmd,"file") == 0) {
		if (paramCount == 2) {
			error = FIOOpenFileHandle(0,CPPPARAM(1),0);
			if (error == 0) FIOCloseFileHandle(0);
		}
	}
	//
	//		*copy <file1> <file2> copy file
	//
	if (strcmp(cmd,"copy") == 0) {
		if (paramCount == 3) {
			error = FIOCopyFile(CPPPARAM(1),CPPPARAM(2));
		}
	}
	//
	//		*ren <file1> <file2> rename file
	//
	if (strcmp(cmd,"ren") == 0) {
		if (paramCount == 3) {
			error = FIORenameFile(CPPPARAM(1),CPPPARAM(2));
		}
	}
	//
	//		*md <directory>
	//
	if (strcmp(cmd,"md") == 0) {
		if (paramCount == 2) {
			error = FIOCreateDirectory(CPPPARAM(1));
		}
	}
	//
	//		*cd <directory>
	//
	if (strcmp(cmd,"cd") == 0) {
		if (paramCount == 2) {
			error = FIOChangeDirectory(CPPPARAM(1));
		}
	}


	return error;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
