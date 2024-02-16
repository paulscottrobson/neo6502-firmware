// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      filewrapper.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      16th February 2024
//      Reviewed :  No
//      Purpose :   Higher level File functions.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include <inttypes.h>

// ***************************************************************************************
//
//									Read File
//
// ***************************************************************************************

uint8_t FISReadFile(const std::string& filename,uint16_t loadAddress,uint16_t maxSize) {
	STOInitialise();
	uint8_t error = FISOpenFileHandle(FIO_SYSTEM_HANDLE,filename,FIOMODE_RDONLY);
	if (error == 0) {
		error = FISReadFileHandle(FIO_SYSTEM_HANDLE,loadAddress,&maxSize);
	}
	if (error == 0) {
		error = FISCloseFileHandle(FIO_SYSTEM_HANDLE);
	}
	return error;
}

// ***************************************************************************************
//
//									Write File
//
// ***************************************************************************************

uint8_t FISWriteFile(const std::string& filename,uint16_t startAddress,uint16_t size) {

	STOInitialise();
	printf("%s\n",filename.c_str());
	uint8_t error = FISOpenFileHandle(FIO_SYSTEM_HANDLE,filename,FIOMODE_RDWR_CREATE);
	printf("%d\n",error);
	if (error == 0) {
		error = FISWriteFileHandle(FIO_SYSTEM_HANDLE,startAddress,&size);
	}
	printf("%d\n",error);
	if (error == 0) {
		error = FISCloseFileHandle(FIO_SYSTEM_HANDLE);
	}
	printf("%d\n",error);
	return error;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
