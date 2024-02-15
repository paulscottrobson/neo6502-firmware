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

#define FIOATTR_DIR      (1<<0)
#define FIOATTR_SYSTEM   (1<<1)
#define FIOATTR_ARCHIVE  (1<<2)
#define FIOATTR_READONLY (1<<3)
#define FIOATTR_HIDDEN   (1<<4)

uint8_t FISReadFile(const std::string& filename,uint16_t loadAddress,uint16_t maxSize);
uint8_t FISWriteFile(const std::string& filename,uint16_t startAddress,uint16_t size);
uint8_t FISRenameFile(const std::string& oldFilename, const std::string& newFilename);
uint8_t FISDeleteFile(const std::string& filename);
uint8_t FISCreateDirectory(const std::string& filename);
uint8_t FISChangeDirectory(const std::string& filename);
uint8_t FISStatFile(const std::string& filename, uint32_t* length, uint8_t* attribs);
uint8_t FISOpenDir(const std::string& dirname);
uint8_t FISReadDir(std::string& filename, uint32_t* size, uint8_t* attribs);
uint8_t FISCloseDir();

uint8_t FISOpenFileHandle(uint8_t fileno, const std::string& filename, uint8_t mode);
uint8_t FISCloseFileHandle(uint8_t fileno);
uint8_t FISSeekFileHandle(uint8_t fileno, uint32_t offset);
uint8_t FISTellFileHandle(uint8_t fileno, uint32_t* offset);
uint8_t FISReadFileHandle(uint8_t fileno, uint16_t address, uint16_t* size);
uint8_t FISWriteFileHandle(uint8_t fileno, uint16_t address, uint16_t* size);
uint8_t FISGetSizeFileHandle(uint8_t fileno, uint32_t* size);
uint8_t FISSetSizeFileHandle(uint8_t fileno, uint32_t size);

#define FIO_NUM_FILES  8
#define FIOMODE_RDONLY   0
#define FIOMODE_WRONLY  1
#define FIOMODE_RDWR   2
#define FIOMODE_RDWR_CREATE 3 // truncates

void FIODirectory(const char *subString);
uint8_t FIOReadFile(const std::string& filename, uint16_t loadAddress);
uint8_t FIOWriteFile(const std::string& filename, uint16_t startAddress,uint16_t size);
uint8_t FIORenameFile(const std::string& oldFilename, const std::string& newFilename);
uint8_t FIODeleteFile(const std::string& filename);
uint8_t FIOCreateDirectory(const std::string& filename);
uint8_t FIOChangeDirectory(const std::string& filename);
uint8_t FIOStatFile(const std::string& filename, uint32_t* length, uint8_t* attribs);
uint8_t FIOOpenDir(const std::string& dirname);
uint8_t FIOReadDir(std::string& filename, uint32_t* size, uint8_t* attribs);
uint8_t FIOCloseDir();
uint8_t FIORenameFile(const std::string& oldFilename, const std::string& newFilename);

uint8_t FIOOpenFileHandle(uint8_t fileno, const std::string& filename, uint8_t mode);
uint8_t FIOCloseFileHandle(uint8_t fileno);
uint8_t FIOSeekFileHandle(uint8_t fileno, uint32_t offset);
uint8_t FIOTellFileHandle(uint8_t fileno, uint32_t* offset);
uint8_t FIOReadFileHandle(uint8_t fileno, uint16_t address, uint16_t* size);
uint8_t FIOWriteFileHandle(uint8_t fileno, uint16_t address, uint16_t* size);
uint8_t FIOGetSizeFileHandle(uint8_t fileno, uint32_t* size);
uint8_t FIOSetSizeFileHandle(uint8_t fileno, uint32_t size);

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
