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

enum FIOErrno {
    FIOERROR_OK = 0,                /* Succeeded */
    FIOERROR_UNKNOWN = 1,           /* An error what we don't know what it is */
    FIOERROR_EOF = 2,               /* End of file or directory */
    FIOERROR_UNIMPLEMENTED = 3,     /* This operation is not supported */

    /* These are straight copies of the fatfs error codes and have the same
     * semantics (but with 100 added to each number). */

    FIOERROR_DISK_ERR = 101,        /* A hard error occurred in the low level disk I/O layer */
    FIOERROR_INT_ERR,               /* Assertion failed */
    FIOERROR_NOT_READY,             /* The physical drive cannot work */
    FIOERROR_NO_FILE,               /* Could not find the file */
    FIOERROR_NO_PATH,               /* Could not find the path */
    FIOERROR_INVALID_NAME,          /* The path name format is invalid */
    FIOERROR_DENIED,                /* Access denied due to prohibited access or directory full */
    FIOERROR_EXIST,                 /* Access denied due to prohibited access */
    FIOERROR_INVALID_OBJECT,        /* The file/directory object is invalid */
    FIOERROR_WRITE_PROTECTED,       /* The physical drive is write protected */
    FIOERROR_INVALID_DRIVE,         /* The logical drive number is invalid */
    FIOERROR_NOT_ENABLED,           /* The volume has no work area */
    FIOERROR_NO_FILESYSTEM,         /* There is no valid FAT volume */
    FIOERROR_MKFS_ABORTED,          /* The f_mkfs() aborted due to any problem */
    FIOERROR_TIMEOUT,               /* Could not get a grant to access the volume within defined period */
    FIOERROR_LOCKED,                /* The operation is rejected according to the file sharing policy */
    FIOERROR_NOT_ENOUGH_CORE,       /* LFN working buffer could not be allocated */
    FIOERROR_TOO_MANY_OPEN_FILES,   /* Number of open files > FF_FS_LOCK */
    FIOERROR_INVALID_PARAMETER      /* Given parameter is invalid */
};

uint8_t FISRenameFile(const std::string& oldFilename, const std::string& newFilename);
uint8_t FISCopyFile(const std::string& oldFilename, const std::string& newFilename);
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
uint8_t FISSetFileAttributes(const std::string& filename, uint8_t attribs);

typedef uint8_t (* FILEREADBYTE)(uint8_t *);

#define FIO_NUM_FILES  8
#define FIOMODE_RDONLY   0
#define FIOMODE_WRONLY  1
#define FIOMODE_RDWR   2
#define FIOMODE_RDWR_CREATE 3 // truncates

void FIODirectory(const char *subString);
uint8_t FIOReadFile(const std::string& fileName,uint16_t loadAddress,uint8_t *commandPtr);
uint8_t FIOReadFileBasic(const std::string& fileName,uint16_t loadAddress);
uint8_t FIOWriteFile(const std::string& filename, uint16_t startAddress,uint16_t size);
uint8_t FIORenameFile(const std::string& oldFilename, const std::string& newFilename);
uint8_t FIOCopyFile(const std::string& oldFilename, const std::string& newFilename);
uint8_t FIODeleteFile(const std::string& filename);
uint8_t FIOCreateDirectory(const std::string& filename);
uint8_t FIOChangeDirectory(const std::string& filename);
uint8_t FIOStatFile(const std::string& filename, uint32_t* length, uint8_t* attribs);
uint8_t FIOExistsFile(const std::string& filename,uint8_t *pExistsFlag);
uint8_t FIOOpenDir(const std::string& dirname);
uint8_t FIOReadDir(std::string& filename, uint32_t* size, uint8_t* attribs);
uint8_t FIOCloseDir();
uint8_t FIORenameFile(const std::string& oldFilename, const std::string& newFilename);
uint8_t FIOSetFileAttributes(const std::string& filename, uint8_t attribs);
uint8_t FIOReadBlock(FILEREADBYTE readfn,uint8_t *commandPtr,bool *pContinue);

uint8_t FIOOpenFileHandle(uint8_t fileno, const std::string& filename, uint8_t mode);
uint8_t FIOCloseFileHandle(uint8_t fileno);
uint8_t FIOSeekFileHandle(uint8_t fileno, uint32_t offset);
uint8_t FIOTellFileHandle(uint8_t fileno, uint32_t* offset);
uint8_t FIOReadFileHandle(uint8_t fileno, uint16_t address, uint16_t* size);
uint8_t FIOWriteFileHandle(uint8_t fileno, uint16_t address, uint16_t* size);
uint8_t FIOGetSizeFileHandle(uint8_t fileno, uint32_t* size);
uint8_t FIOSetSizeFileHandle(uint8_t fileno, uint32_t size);
uint8_t FIOIsEndOfFileHandle(uint8_t fileno,uint8_t *pIsEndOfFile);
#endif

// ***************************************************************************************
//
//      Date        Revision
//      ====        ========
//
// ***************************************************************************************
