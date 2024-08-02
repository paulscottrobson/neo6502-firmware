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
#include <memory>
#include <functional>

static constexpr int COPY_BUFFER_SIZE = 1024;

static FIL fileHandles[FIO_NUM_FILES];

// ***************************************************************************************
//
//									Rename File
//
// ***************************************************************************************

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
//									Rename File
//
// ***************************************************************************************

uint8_t FISRenameFile(const std::string& oldFilename, const std::string& newFilename) {
	STOInitialise();
	// CONWriteString("FISRenameFile('%s', '%s') -> ", oldFilename.c_str(), newFilename.c_str());
	FRESULT result = f_rename(oldFilename.c_str(), newFilename.c_str());
	// CONWriteString("%d\r", result);
	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//									Copy File
//
// ***************************************************************************************

uint8_t FISCopyFile(const std::string& oldFilename, const std::string& newFilename) {
	STOInitialise();

	// Our stack is quite small and there's a lot of buffer here, so
	// allocate them from the heap. The unique_ptr<> causes them to be
	// cleaned up automatically when the function ends.
	std::unique_ptr<FIL> oldFile(new (std::nothrow) FIL);
	std::unique_ptr<FIL> newFile(new (std::nothrow) FIL);
	std::unique_ptr<uint8_t[]> buffer(new (std::nothrow) uint8_t[COPY_BUFFER_SIZE]);
	if (!oldFile || !newFile || !buffer) {
		// Out of memory.
		// CONWriteString("Out of memory\r");
		return 1;
	}

	FRESULT result = f_open(&*oldFile, oldFilename.c_str(), FA_READ);
	// CONWriteString("Opened '%s' for read -> %d\r", oldFilename.c_str(), result);
	if (result == FR_OK)
	{
		result = f_open(&*newFile, newFilename.c_str(), FA_WRITE|FA_CREATE_ALWAYS);
		// CONWriteString("Opened '%s' for write -> %d\r", newFilename.c_str(), result);
		if (result == FR_OK)
		{
			for (;;) {
				UINT bytesRead;
				result = f_read(&*oldFile, buffer.get(), COPY_BUFFER_SIZE, &bytesRead);
				if (result != FR_OK)
					break;
				if (bytesRead == 0)
					break;
				
				UINT bytesWritten;
				result = f_write(&*newFile, buffer.get(), bytesRead, &bytesWritten);
				if (result != FR_OK)
					break;
				if (bytesWritten != bytesRead) {
					// Disk full.
					result = FR_DISK_ERR;
					break;
				}
			}

			f_close(&*newFile);
		}

		f_close(&*oldFile);
	}

	return (result == FR_OK) ? 0 : 1;
}


// ***************************************************************************************
//
//									Delete File
//
// ***************************************************************************************

uint8_t FISDeleteFile(const std::string& filename) {
	STOInitialise();
	// CONWriteString("FISDeleteFile('%s') ->", filename.c_str());
	FRESULT result = f_unlink(filename.c_str());
	// CONWriteString("%d\r", result);
	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//									Create Directory
//
// ***************************************************************************************

uint8_t FISCreateDirectory(const std::string& filename) {
	STOInitialise();
	// CONWriteString("FISCreateDirectory('%s') ->", filename.c_str());
	FRESULT result = f_mkdir(filename.c_str());
	// CONWriteString("%d\r", result);
	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//									Change Directory
//
// ***************************************************************************************

uint8_t FISChangeDirectory(const std::string& filename) {
	STOInitialise();
	// CONWriteString("FISChangeDirectory('%s') ->", filename.c_str());
	FRESULT result = f_chdir(filename.c_str());
	// CONWriteString("%d\r", result);
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
//									Set file attributes
//
// ***************************************************************************************

uint8_t FISSetFileAttributes(const std::string& filename, uint8_t attribs) {
	#if FF_USE_CHMOD
		STOInitialise();

		uint8_t fatattribs = 0;
		if (attribs & FIOATTR_ARCHIVE)
			fatattribs |= AM_ARC;
		if (attribs & FIOATTR_HIDDEN)
			fatattribs |= AM_HID;
		if (attribs & FIOATTR_READONLY)
			fatattribs |= AM_RDO;
		if (attribs & FIOATTR_SYSTEM)
			fatattribs |= AM_SYS;
		
		FRESULT result = f_chmod(filename.c_str(), fatattribs, 0xff);

		return (result == FR_OK) ? 0 : 1;
	#else
		return 1;
	#endif
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

	uint16_t toread = *size;
	if (address != 0xFFFF) toread = std::min(toread, uint16_t(0x10000 - address));

	// CONWriteString("FISReadFileHandle(%d, @0x%04x, 0x%04x) -> ", fileno, address, *size);
	UINT read;
	FRESULT result;
	if (address != 0xFFFF) {
		result = f_read(f, cpuMemory+address, toread, &read);
	} else {
		result = f_read(f, gfxObjectMemory, toread, &read);	
	}
	*size = read;
	// CONWriteString("%d, 0x%04x\r", result, read);

	return ((result == FR_OK) && (read != 0)) ? 0 : 1;
}

uint8_t FISWriteFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	FIL* f = getF(fileno);
	if (!f)
		return 1;

	uint16_t towrite = std::min(*size, uint16_t(0x10000 - address));

	// CONWriteString("FISWriteFileHandle(%d, @0x%04x, 0x%04x) -> ", fileno, address, *size);
	UINT written;
	FRESULT result = f_write(f, cpuMemory+address, towrite, &written);
	*size = written;
	// CONWriteString("%d, 0x%04x\r", result, written);

	return ((result == FR_OK) && (written != 0)) ? 0 : 1;
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
	// CONWriteString("FISSetSizeFileHandle(%d, 0x%08x) -> ", fileno, size);
	FRESULT result = f_lseek(f, size);
	if (result == FR_OK)
		result = f_truncate(f);
	// CONWriteString("%d\r", result);

	return (result == FR_OK) ? 0 : 1;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
