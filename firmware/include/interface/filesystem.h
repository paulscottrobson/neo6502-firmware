// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      filesystem.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      18th December 2023
//      Reviewed :  No
//      Purpose :   File I/O implementation code.
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _FILESYS_H
#define _FILESYS_H

int FISDirectoryOpen(void);
int FISDirectoryClose(void);
int FISDirectoryNext(char *buffer,int *isDirectory,int *fileSize);

uint8_t FISReadFile(const char *fileName,uint16_t loadAddress,uint16_t maxSize);
uint8_t FISWriteFile(const char *fileName,uint16_t startAddress,uint16_t size);

void FIODirectory(void);
uint8_t FIOReadFile(const char *fileName,uint16_t loadAddress);
uint8_t FIOWriteFile(const char *fileName,uint16_t startAddress,uint16_t size);
#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
