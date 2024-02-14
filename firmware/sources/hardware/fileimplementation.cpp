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

static uint8_t getAttributes(const FILINFO* fno) {
	uint8_t attrs = 0;
	if (fno->fattrib & AM_DIR)
		attrs |= FIOATTR_DIR;
	if (fno->fattrib & AM_ARC)
		attrs |= FIOATTR_ARCHIVE;
	if (fno->fattrib & AM_HID)
		attrs |= FIOATTR_HIDDEN;
	if (fno->fattrib & AM_RDO)
		attrs |= FIOATTR_READONLY;
	return attrs;
}

// ***************************************************************************************
//
//									Read File
//
// ***************************************************************************************

uint8_t FISReadFile(const std::string& filename,uint16_t loadAddress,uint16_t maxSize) {
	FIL file;
	FRESULT result;
	UINT bytesRead;
	STOInitialise();
	result = f_open(&file, filename.c_str(), FA_READ);
	if (result == FR_OK) {
		if (loadAddress == 0xFFFF) {
			result = f_read(&file,gfxMemory,maxSize,&bytesRead);
		} else {
			result = f_read(&file,cpuMemory+loadAddress,0x10000-loadAddress,&bytesRead);
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

uint8_t FISWriteFile(const std::string& filename,uint16_t startAddress,uint16_t size) {
	FIL file;
	FRESULT result;
	UINT bytesWritten;
	STOInitialise();
	result = f_open(&file, filename.c_str(), FA_WRITE|FA_CREATE_ALWAYS);
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
//									Delete File
//
// ***************************************************************************************

uint8_t FISDeleteFile(const std::string& filename) {
	STOInitialise();
	FRESULT result = f_unlink(filename.c_str());
	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//									Create Directory
//
// ***************************************************************************************

uint8_t FISCreateDirectory(const std::string& filename) {
	STOInitialise();
	FRESULT result = f_mkdir(filename.c_str());
	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//									Change Directory
//
// ***************************************************************************************

uint8_t FISChangeDirectory(const std::string& filename) {
	STOInitialise();
	FRESULT result = f_chdir(filename.c_str());
	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//									   Stat file
//
// ***************************************************************************************

uint8_t FISStatFile(const std::string& filename, uint32_t* length, uint8_t* attribs) {
	STOInitialise();

	FILINFO fno;
	FRESULT result = f_stat(filename.c_str(), &fno);

	*length = fno.fsize;
	*attribs = getAttributes(&fno);

	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//								Directory enumeration
//
// ***************************************************************************************

static DIR readDir;

uint8_t FISOpenDir(const std::string& filename) {
	STOInitialise();

	// CONWriteString("FISOpenDir('%s')\r", filename.c_str());
	if (readDir.obj.fs)
		f_closedir(&readDir);

	FRESULT result = f_opendir(&readDir, filename.c_str());
	return (result == FR_OK) ? 0 : 1;
}

uint8_t FISReadDir(std::string& filename, uint32_t* size, uint8_t* attribs) {
	if (!readDir.obj.fs)
		return 1;

	// CONWriteString("FISReadDir() ->");
	FILINFO fno;
	FRESULT result = f_readdir(&readDir, &fno);
	if ((result == FR_OK) && fno.fname[0]) {
		filename = fno.fname;
		*attribs = getAttributes(&fno);
		*size = fno.fsize;
		// CONWriteString("'%s'\r", fno.fname);
		return 0;
	} else {
		// CONWriteString("finished\r");
		f_closedir(&readDir);
		return 1;
	}
}

uint8_t FISCloseDir() {
	// CONWriteString("FISCloseDir()\r");
	if (readDir.obj.fs)
		f_closedir(&readDir);

	return 0;
}

// ***************************************************************************************
//
//								File-handle based functions
//
// ***************************************************************************************

uint8_t FISOpenFileHandle(uint8_t fileno, const std::string& filename, uint8_t mode) {
	// CONWriteString("FISOpen(%d, '%s')\r", fileno, filename.c_str());
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
	FRESULT result = f_open(f, filename.c_str(), modes[mode]);
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
	// CONWriteString("FISCloseFileHandle(%d)\r", fileno);
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

	uint16_t toread = std::min(*size, uint16_t(0x10000 - address));

	UINT read;
	FRESULT result = f_read(f, cpuMemory+address, toread, &read);
	*size = read;

	return ((result == FR_OK) && (toread == read)) ? 0 : 1;
}

uint8_t FISWriteFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	FIL* f = getF(fileno);
	if (!f)
		return 1;

	uint16_t towrite = std::min(*size, uint16_t(0x10000 - address));

	UINT written;
	FRESULT result = f_write(f, cpuMemory+address, towrite, &written);
	*size = written;

	return ((result == FR_OK) && (towrite == written)) ? 0 : 1;
}

uint8_t FISGetSizeFileHandle(uint8_t fileno, uint32_t* size) {
	FIL* f = getF(fileno);
	if (!f)
		return 1;

	*size = f_size(f);
	// CONWriteString("FISGetSize(%d) -> 0x%08x\r", fileno, *size);
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
