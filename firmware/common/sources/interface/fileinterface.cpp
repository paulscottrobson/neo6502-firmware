// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      fileinterface.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      2nd January 2024
//      Reviewed :  No
//      Purpose :   File I/O interface code.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include <inttypes.h>

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
				if (fileSize >= 0) {
					while (strlen(szBuffer) < 32) strcat(szBuffer," ");
					if (isDirectory != 0) {
						strcat(szBuffer,"<Dir>");
					} else {
						sprintf(szBuffer+strlen(szBuffer),"%d bytes.",fileSize);
					}
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

uint8_t FIOReadFile(const char *fileName,uint16_t loadAddress) {
	uint16_t maxRead = (loadAddress == 0xFFFF) ? GFX_MEMORY_SIZE : 0x10000-loadAddress;
	return FISReadFile(fileName,loadAddress,maxRead);
}

// ***************************************************************************************
//
//									Write File
//
// ***************************************************************************************

uint8_t FIOWriteFile(const char *fileName,uint16_t startAddress,uint16_t size) {
	if (startAddress == 0xFFFF) return 1;
	return FISWriteFile(fileName,startAddress,size);
}

// ***************************************************************************************
//
//									Rename File
//
// ***************************************************************************************

uint8_t FIORenameFile(const std::string& oldFilename, const std::string& newFilename) {
	return FISRenameFile(oldFilename, newFilename);
}

// ***************************************************************************************
//
//								File-handle based functions
//
// ***************************************************************************************

uint8_t FIOOpenFileHandle(uint8_t fileno, const char* filename, uint8_t mode) {
	return FISOpenFileHandle(fileno, filename, mode);
}

uint8_t FIOCloseFileHandle(uint8_t fileno) {
	return FISCloseFileHandle(fileno);
}

uint8_t FIOSeekFileHandle(uint8_t fileno, uint32_t pos) {
	return FISSeekFileHandle(fileno, pos);
}

uint8_t FIOTellFileHandle(uint8_t fileno, uint32_t* pos) {
	return FISTellFileHandle(fileno, pos);
}

uint8_t FIOReadFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	if (address == 0xFFFF) return 1;
	return FISReadFileHandle(fileno, address, size);
}

uint8_t FIOWriteFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	if (address == 0xFFFF) return 1;
	return FISWriteFileHandle(fileno, address, size);
}

uint8_t FIOGetSizeFileHandle(uint8_t fileno, uint32_t* size) {
	return FISGetSizeFileHandle(fileno, size);
}

uint8_t FIOSetSizeFileHandle(uint8_t fileno, uint32_t size) {
	return FISSetSizeFileHandle(fileno, size);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//		16-01-24 	Modified so fileSize < 0 doesn't display information.
//
// ***************************************************************************************
