// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      fileimplementation.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      18th December 2023
//      Reviewed :  No
//      Purpose :   File I/O implementation code
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include <inttypes.h>
#include "ff.h"

static FIL fileHandles[FIO_NUM_FILES];

// ***************************************************************************************
//
//									Directory Functions
//
// ***************************************************************************************

static DIR dir;
static FILINFO fi;

int FISDirectoryOpen(void) {
	STOInitialise();
	FRESULT rDir = f_opendir(&dir,"/");
	return (rDir == FR_OK) ? 0 : 1;
}

int FISDirectoryClose(void) {
	f_closedir(&dir);
	return 0;
}

int FISDirectoryNext(char *buffer,int *isDirectory,int *fileSize) {
	int isError = 1;
	if (f_readdir(&dir,&fi) == FR_OK && fi.fname[0] != '\0') {
		strcpy(buffer,fi.fname);
		*isDirectory = (fi.fattrib & AM_DIR) != 0;
		*fileSize = fi.fsize;
		isError = 0;

	}
	return isError;
}   

// ***************************************************************************************
//
//									Read File
//
// ***************************************************************************************

uint8_t FISReadFile(const char *fileName,uint16_t loadAddress,uint16_t maxSize) {
	FIL file;
	FRESULT result;
	UINT bytesRead;
	STOInitialise();
	result = f_open(&file, fileName, FA_READ);
	if (result == FR_OK) {
		if (loadAddress == 0xFFFF) {
			f_read(&file,gfxMemory,maxSize,&bytesRead);
		} else {
			f_read(&file,cpuMemory+loadAddress,0x10000-loadAddress,&bytesRead);
		}
		f_close(&file);
	}
	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//									Write File
//
// ***************************************************************************************

uint8_t FISWriteFile(const char *fileName,uint16_t startAddress,uint16_t size) {
	FIL file;
	FRESULT result;
	UINT bytesWritten;
	STOInitialise();
	result = f_open(&file, fileName, FA_WRITE|FA_CREATE_ALWAYS);
	if (result == FR_OK) {
		f_write(&file,cpuMemory+startAddress,size,&bytesWritten);
		f_close(&file);
	}
	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//									Rename File
//
// ***************************************************************************************

uint8_t FISRenameFile(const std::string& oldFilename, const std::string& newFilename) {
	STOInitialise();
	FRESULT result = f_rename(oldFilename.c_str(), newFilename.c_str());
	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//								File-handle based functions
//
// ***************************************************************************************

uint8_t FISOpenFileHandle(uint8_t fileno, const char* filename, uint8_t mode) {
	if (fileno >= FIO_NUM_FILES)
		return 1;
	FIL* f = &fileHandles[fileno];

	/* Check if already open. */
	if (f->obj.fs)
		return 1;

	static int const modes[] = {
		FA_READ,							// 0: FIOMODE_RDONLY
		FA_WRITE, 							// 1: FIOMODE_WRONLY
		FA_READ|FA_WRITE,                 	// 2: FIOMODE_RDWR
		FA_READ|FA_WRITE|FA_CREATE_ALWAYS,	// 3: FIOMODE_RDWR_CREATE
	};
	if (mode >= sizeof(modes)/sizeof(*modes))
		return 1;

	STOInitialise();
	FRESULT result = f_open(f, filename, modes[mode]);
	return (result == FR_OK) ? 0 : 1;
}

static FIL* getF(uint8_t fileno) {
	if (fileno >= FIO_NUM_FILES)
		return NULL;
	FIL* f = &fileHandles[fileno];
	if (!f->obj.fs)
		return NULL;
	return f;
}

uint8_t FISCloseFileHandle(uint8_t fileno) {
	if (fileno == 0xff) {
		for (FIL& f : fileHandles)
		{
			if (f.obj.fs)
				f_close(&f);
		}
		return 0;
	} else {
		FIL* f = getF(fileno);
		if (!f)
			return 1;

		FRESULT result = f_close(f);
		return (result == FR_OK) ? 0 : 1;
	}
}

uint8_t FISSeekFileHandle(uint8_t fileno, uint32_t offset) {
	FIL* f = getF(fileno);
	if (!f)
		return 1;

	FRESULT result = f_lseek(f, offset);
	return (result == FR_OK) ? 0 : 1;
}

uint8_t FISTellFileHandle(uint8_t fileno, uint32_t* offset) {
	FIL* f = getF(fileno);
	if (!f)
		return 1;

	*offset = f_tell(f);
	return 0;
}

uint8_t FISReadFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	FIL* f = getF(fileno);
	if (!f)
		return 1;

	UINT written;
	FRESULT result = f_read(f, cpuMemory+address, *size, &written);
	*size = written;

	return (result == FR_OK) ? 0 : 1;
}

uint8_t FISWriteFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	FIL* f = getF(fileno);
	if (!f)
		return 1;

	UINT written;
	FRESULT result = f_write(f, cpuMemory+address, *size, &written);
	*size = written;

	return (result == FR_OK) ? 0 : 1;
}

uint8_t FISGetSizeFileHandle(uint8_t fileno, uint32_t* size) {
	FIL* f = getF(fileno);
	if (!f)
		return 1;

	*size = f_size(f);
	return 0;
}

uint8_t FISSetSizeFileHandle(uint8_t fileno, uint32_t size) {
	FIL* f = getF(fileno);
	if (!f)
		return 1;

	uint32_t oldPos = f_tell(f);
	FRESULT result = f_lseek(f, size);
	if (result == FR_OK)
		result = f_truncate(f);

	f_lseek(f, oldPos);
	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
