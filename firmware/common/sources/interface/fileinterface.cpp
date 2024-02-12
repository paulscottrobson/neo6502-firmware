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

void FIODirectory(const char *subString) {
	if (FISOpenDir(".") == 0) {
		std::string buffer;
		uint32_t fileSize;
		uint8_t attribs;

		buffer.reserve(64);
		while (FISReadDir(buffer, &fileSize, &attribs) == 0) {
			if (buffer[0] != '.') {
				if (fileSize >= 0) {
					while (buffer.size() < 32)
					 	buffer += ' ';
					if (attribs & FIOATTR_DIR)
						buffer += "<Dir>";
					else {
						buffer += std::to_string(fileSize);
						buffer += " bytes.";
					}
				}
				CONWriteString(buffer.c_str());
				CONWriteString("\r");				
			}
		}
		FISCloseDir();
	}
}   

// ***************************************************************************************
//
//									Read File
//
// ***************************************************************************************

uint8_t FIOReadFile(const std::string& filename, uint16_t loadAddress) {
	uint16_t maxRead = (loadAddress == 0xFFFF) ? GFX_MEMORY_SIZE : 0x10000-loadAddress;
	return FISReadFile(filename, loadAddress,maxRead);
}

// ***************************************************************************************
//
//									Write File
//
// ***************************************************************************************

uint8_t FIOWriteFile(const std::string& filename, uint16_t startAddress,uint16_t size) {
	if (startAddress == 0xFFFF) return 1;
	return FISWriteFile(filename, startAddress,size);
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
//									Delete File
//
// ***************************************************************************************

uint8_t FIODeleteFile(const std::string& filename) {
	return FISDeleteFile(filename);
}

// ***************************************************************************************
//
//									Create directory
//
// ***************************************************************************************

uint8_t FIOCreateDirectory(const std::string& filename) {
	return FISCreateDirectory(filename);
}

// ***************************************************************************************
//
//									Change directory
//
// ***************************************************************************************

uint8_t FIOChangeDirectory(const std::string& filename) {
	return FISChangeDirectory(filename);
}

// ***************************************************************************************
//
//									Stat file
//
// ***************************************************************************************

uint8_t FIOStatFile(const std::string& filename, uint32_t* length, uint8_t* mode) {
	return FISStatFile(filename, length, mode);
}

// ***************************************************************************************
//
//								Directory enumeration
//
// ***************************************************************************************

uint8_t FIOOpenDir(const std::string& filename) {
	return FISOpenDir(filename);
}

uint8_t FIOReadDir(std::string& result, uint32_t* size, uint8_t* attribs) {
	return FISReadDir(result, size, attribs);
}

uint8_t FIOCloseDir() {
	return FISCloseDir();
}

// ***************************************************************************************
//
//								File-handle based functions
//
// ***************************************************************************************

uint8_t FIOOpenFileHandle(uint8_t fileno, const std::string& filename, uint8_t mode) {
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
//		10-02-24 	Added support for cat wildcards.
//
// ***************************************************************************************
