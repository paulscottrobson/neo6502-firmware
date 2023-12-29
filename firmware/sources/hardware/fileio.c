// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      fileio.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      18th December 2023
//      Reviewed :  No
//      Purpose :   File I/O code.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include <inttypes.h>
#include "ff.h"

// ***************************************************************************************
//
//									Display directory
//
// ***************************************************************************************

void FIODirectory(void) {
	DIR d;
	STOInitialise();
	FRESULT r = f_opendir(&d,"/");
	char szBuffer[320];
	if (r == FR_OK) {
		FILINFO fi;
		while (f_readdir(&d,&fi) == FR_OK && fi.fname[0] != '\0') {
			if (fi.fname[0] != '.') {
				sprintf(szBuffer,"%-32s ",fi.fname);
				if (fi.fattrib & AM_DIR) {
					strcat(szBuffer,"<DIR>");
				} else {
					sprintf(szBuffer+strlen(szBuffer),"%d bytes.",(int)fi.fsize);
				}
				CONWriteString(szBuffer);
				CONWriteString("\r");
			}
		}
		f_closedir(&d);
	}
}   

// ***************************************************************************************
//
//									Read File
//
// ***************************************************************************************

uint8_t FIOReadFile(char *fileName,uint16_t loadAddress) {
	FIL file;
	FRESULT result;
	UINT bytesRead;
	STOInitialise();
	result = f_open(&file, fileName, FA_READ);
	if (result == FR_OK) {
		f_read(&file,cpuMemory+loadAddress,0x10000-loadAddress,&bytesRead);
		f_close(&file);
	}
	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//									Write File
//
// ***************************************************************************************

uint8_t FIOWriteFile(char *fileName,uint16_t startAddress,uint16_t size) {
	FIL file;
	FRESULT result;
	UINT bytesWritten;
	if (size > 0x10000-startAddress) return 2;
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
