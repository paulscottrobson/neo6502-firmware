// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      fileimplementation.c
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

uint8_t FISReadFile(char *fileName,uint16_t loadAddress,uint16_t maxSize) {
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

uint8_t FISWriteFile(char *fileName,uint16_t startAddress,uint16_t size) {
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
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
