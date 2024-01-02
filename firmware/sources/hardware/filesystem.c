// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      filesystem.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      2nd January 2024
//      Reviewed :  No
//      Purpose :   File I/O interface code.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include <inttypes.h>
#include "ff.h"
#include "system/fileio.h"

// ***************************************************************************************
//
//									Display directory
//
// ***************************************************************************************

void FIODirectory(void) {
	if (FISDirectoryOpen() == 0) {
		char szBuffer[320];
		int isDirectory,fileSize;
		while (FISDirectoryNext(szBuffer,&isDirectory,&fileSize) == 0) {
			if (szBuffer[0] != '.') {
				while (strlen(szBuffer) < 32) strcat(szBuffer," ");
				if (isDirectory != 0) {
					strcat(szBuffer,"<Dir>");
				} else {
					sprintf(szBuffer+strlen(szBuffer),"%d bytes.",fileSize);
				}
				CONWriteString(szBuffer);CONWriteString("\r");
			}
		}
		FISDirectoryClose();
	}
}   

// ***************************************************************************************
//
//									Read File
//
// ***************************************************************************************

uint8_t FIOReadFile(char *fileName,uint16_t loadAddress) {
	uint16_t maxRead = (loadAddress == 0xFFFF) ? GFX_MEMORY_SIZE : 0x10000-loadAddress;
	return FISReadFile(fileName,loadAddress,maxRead);
}

// ***************************************************************************************
//
//									Write File
//
// ***************************************************************************************

uint8_t FIOWriteFile(char *fileName,uint16_t startAddress,uint16_t size) {
	if (startAddress == 0xFFFF) return 1;
	return FISWriteFile(fileName,startAddress,size);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
