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

static uint8_t FIOReadByte(uint8_t *commandPtr);

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
					if (strstr(buffer.c_str(),subString) != NULL) {
						while (buffer.size() < 32)
						 	buffer += ' ';
						if (attribs & FIOATTR_DIR)
							buffer += "<Dir>";
						else {
							buffer += std::to_string(fileSize);
							buffer += " bytes.";
						}
						CONWriteString(buffer.c_str());
						CONWrite('\r');
					}
				}
			}
		}
		FISCloseDir();
	}
}   

// ***************************************************************************************
//
//									Read File (New)
//
// ***************************************************************************************

uint8_t FIOReadFile(const std::string& fileName,uint16_t loadAddress,uint8_t *commandPtr) {
	STOInitialise();
	printf("Trying to read header for %s\n",fileName.c_str());	
	uint8_t *header = commandPtr+4;  												// Header goes here.
	header[0] = header[1] = header[2] = header[3] = 0;  							// Zero the space to receive the header.
	uint8_t error = FISOpenFileHandle(0,fileName.c_str(),FIOMODE_RDONLY);  			// Try to open the file	
	if (error != 0) return error;													// Failed.
	uint16_t loadRequired = 8;  	 												// Read the first 4 bytes.
	FISReadFileHandle(0,header-cpuMemory,&loadRequired);
	bool isExec = header[0] == 0x03 && header[1] == 0x4E &&  						// Look for the magic number
								header[2] == 0x45 && header[3] == 0x4F;
	commandPtr[8] = 0x60;  															// Set the default code to $60, the 65C02 RTS.
	if (!isExec) {  																// Not an executable, use the normal loader.
		error = FISCloseFileHandle(0);  											// Close the file.
		return (error != 0) ? error : FIOReadFileBasic(fileName,loadAddress);	 	// if okay then use the old reader.
	}
	printf("Header found.\n");
	uint16_t execAddress = header[6] + (header[7] << 8); 							// Get the execute address
	printf("Execute from $%x\n",execAddress);
	bool processing = true; 
	while (processing && error == 0) {
		error = FIOReadBlock(FIOReadByte,commandPtr,&processing); 					// Read one block.
	}
	if (error == 0) error = FIOCloseFileHandle(0);  								// Close the source file.
	if (execAddress != 0xFFFF) {  													// Not the default $FFFF e.g. don't execute
		commandPtr[8] = 0x4C;  														// Set the paraemeter area to JMP <exec address>
		commandPtr[9] = execAddress & 0xFF;
		commandPtr[10] = execAddress >> 8;
	}
	return error;
}

// ***************************************************************************************
//
//								Read in the next block
//
// ***************************************************************************************

static char commentBlock[32];  														// Space for comment text.

uint8_t FIOReadBlock(FILEREADBYTE readfn,uint8_t *commandPtr,bool *pContinue) {
	uint8_t contByte = (*readfn)(commandPtr);  										// Read the continuation byte
	printf("Control byte %x\n",contByte);
	*pContinue = (contByte & 0x80) != 0;  											// Continue if control continue bit set
	uint16_t loadAddress,loadSize;  												// Read address and size.
	loadAddress = (*readfn)(commandPtr);
	loadAddress |= (*readfn)(commandPtr) << 8;
	loadSize = (*readfn)(commandPtr);
	loadSize |= (*readfn)(commandPtr) << 8;
	printf("Load to %x size %x\n",loadAddress,loadSize);
	if (loadAddress == 0xFFFD) {  													// Basic LOAD ?
		loadAddress = cpuMemory[0x820]+(cpuMemory[0x821] << 8);
		printf("Load to BASIC at $%x\n",loadAddress);
	}
	char *commChar = commentBlock;   												// Read in comment block.			
	uint8_t count = 0;
	while (*commChar = (*readfn)(commandPtr),*commChar != 0) {
		if (++count > sizeof(commentBlock)) commChar++;
	}
	printf("Comment %s\n",commentBlock);
	printf("Loading bytes.\n");
	for (int i = 0;i < loadSize;i++) {
		uint8_t c = (*readfn)(commandPtr);
		if (loadAddress == 0xFFFF) {
			gfxObjectMemory[i] = c;		
		} else {
			cpuMemory[loadAddress+i] = c;
		}
	}
	return 0;
}

// ***************************************************************************************
//
//								Read a single byte
//
// ***************************************************************************************

static uint8_t FIOReadByte(uint8_t *commandPtr) {
	uint16_t loadRequired = 1;   													// Read in one byte
	FISReadFileHandle(0,commandPtr+4-cpuMemory,&loadRequired);
	return commandPtr[4];  	
}

// ***************************************************************************************
//
//									Read File (Basic)
//
// ***************************************************************************************

uint8_t FIOReadFileBasic(const std::string& fileName,uint16_t loadAddress) {
	STOInitialise();
	printf("Reading %s to $%x\n",fileName.c_str(),loadAddress);
	uint16_t maxRead = (loadAddress == 0xFFFF) ? GFX_MEMORY_SIZE : 0x10000-loadAddress;
	uint8_t error = FISOpenFileHandle(0,fileName,FIOMODE_RDONLY);
	if (error == 0) {
		error = FISReadFileHandle(0,loadAddress,&maxRead);
	}
	if (error == 0) {
		error = FISCloseFileHandle(0);
	}
	printf("%d\n",error);
	return error;
}

// ***************************************************************************************
//
//									Write File
//
// ***************************************************************************************

uint8_t FIOWriteFile(const std::string& filename,uint16_t startAddress,uint16_t size) {
	STOInitialise();
	if (startAddress == 0xFFFF) return 1;
	uint8_t error = FISOpenFileHandle(0,filename,FIOMODE_RDWR_CREATE);
	if (error == 0) {
		error = FISWriteFileHandle(0,startAddress,&size);
	}
	if (error == 0) {
		error = FISCloseFileHandle(0);
	}
	return error;
}

// ***************************************************************************************
//
//									Check file exists
//
// ***************************************************************************************

uint8_t FIOExistsFile(const std::string& filename,uint8_t *pExistsFlag) {
	uint8_t error,mode;
	uint32_t size;
	error = FIOStatFile(filename,&size,&mode);
	*pExistsFlag = (error == 0) ? 1 : 0;
	return 0;
}

// ***************************************************************************************
//
//									Rename File
//
// ***************************************************************************************

uint8_t FIORenameFile(const std::string& oldFilename, const std::string& newFilename) {
	uint8_t error,exists = 0;
	error = FIOExistsFile(newFilename,&exists);
	if (error != 0 || exists != 0) return 1;
	return FISRenameFile(oldFilename, newFilename);
}

// ***************************************************************************************
//
//									 Copy file 
//
// ***************************************************************************************

uint8_t FIOCopyFile(const std::string& oldFilename, const std::string& newFilename) {
	return FISCopyFile(oldFilename, newFilename);
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
//								Check End of File
//
// ***************************************************************************************

uint8_t FIOIsEndOfFileHandle(uint8_t fileno,uint8_t *pIsEndOfFile) {
	uint32_t size,pos;
	uint8_t err;
	err = FIOGetSizeFileHandle(fileno,&size);
	if (err != 0) return err;	
	err = FIOTellFileHandle(fileno,&pos);	
	if (err != 0) return err;	
	*pIsEndOfFile = (size == pos) ? 1 : 0;
	return 0;
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
//								    Set file attributes
//
// ***************************************************************************************

uint8_t FIOSetFileAttributes(const std::string& filename, uint8_t attribs) {
	return FISSetFileAttributes(filename, attribs);
}

// ***************************************************************************************
//
//								File-handle based functions
//
// ***************************************************************************************

uint8_t FIOOpenFileHandle(uint8_t fileno, const std::string& filename, uint8_t mode) {
	if (mode == FIOMODE_RDWR_CREATE) {  											// If open truncate 
		uint8_t exists;
		if (FIOExistsFile(filename,&exists) == 0) {  								// If file exists
			uint8_t error = FIODeleteFile(filename);  								// try to delete it, as we truncate it anyway !
			if (error) return error;
		}
	}
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
//		16-02-24 	cat is broken.
//
// ***************************************************************************************
