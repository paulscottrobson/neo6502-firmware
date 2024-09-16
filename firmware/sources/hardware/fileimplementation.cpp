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

static uint8_t convertError(FRESULT result) {
	switch (result) {
		case FR_OK:						return FIOERROR_OK;
		case FR_DISK_ERR: 				return FIOERROR_DISK_ERR;
		case FR_INT_ERR: 				return FIOERROR_INT_ERR;
		case FR_NOT_READY: 				return FIOERROR_NOT_READY;
		case FR_NO_FILE: 				return FIOERROR_NO_FILE;
		case FR_NO_PATH: 				return FIOERROR_NO_PATH;
		case FR_INVALID_NAME: 			return FIOERROR_INVALID_NAME;
		case FR_DENIED: 				return FIOERROR_DENIED;
		case FR_EXIST: 					return FIOERROR_EXIST;
		case FR_INVALID_OBJECT: 		return FIOERROR_INVALID_OBJECT;
		case FR_WRITE_PROTECTED:		return FIOERROR_WRITE_PROTECTED;
		case FR_INVALID_DRIVE: 			return FIOERROR_INVALID_DRIVE;
		case FR_NOT_ENABLED: 			return FIOERROR_NOT_ENABLED;
		case FR_NO_FILESYSTEM: 			return FIOERROR_NO_FILESYSTEM;
		case FR_MKFS_ABORTED: 			return FIOERROR_MKFS_ABORTED;
		case FR_TIMEOUT: 				return FIOERROR_TIMEOUT;
		case FR_LOCKED: 				return FIOERROR_LOCKED;
		case FR_NOT_ENOUGH_CORE: 		return FIOERROR_NOT_ENOUGH_CORE;
		case FR_TOO_MANY_OPEN_FILES: 	return FIOERROR_TOO_MANY_OPEN_FILES;
		case FR_INVALID_PARAMETER: 		return FIOERROR_INVALID_PARAMETER;
		default:						return FIOERROR_UNKNOWN;
	}
}

namespace {
	class FilObject {
	public:
		FilObject():
			_fil(new (std::nothrow) FIL)
		{}

		~FilObject()
		{
			if (_fil) {
				f_close(_fil);
			}
			delete _fil;
		}

		operator FIL* () const {
			return _fil;
		}

		operator bool () const {
			return _fil;
		}

	private:
		FIL* _fil;
	};
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
	return convertError(result);
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
	FilObject oldFile;
	FilObject newFile;
	std::unique_ptr<uint8_t[]> buffer(new (std::nothrow) uint8_t[COPY_BUFFER_SIZE]);
	if (!oldFile || !newFile || !buffer) {
		// Out of memory.
		// CONWriteString("Out of memory\r");
		return FIOERROR_UNKNOWN;
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
					return FIOERROR_DENIED;
				}
			}
		}
	}

	return convertError(result);
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
	return convertError(result);
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
	return convertError(result);
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
	return convertError(result);
}

// ***************************************************************************************
//
//							Read current directory as a string
//
// ***************************************************************************************

uint8_t FISGetCurrentDirectory(char *target,int maxSize) {
	return convertError(f_getcwd(target,maxSize));
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

	return convertError(result);
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

		return convertError(result);
	#else
		return FIOERROR_UNIMPLEMENTED;
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
	return convertError(result);
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
		return FIOERROR_OK;
	} else {
		// CONWriteString("finished\r");
		f_closedir(&readDir);
		return FIOERROR_EOF;
	}
}

uint8_t FISCloseDir() {
	// CONWriteString("FISCloseDir()\r");
	if (readDir.obj.fs)
		f_closedir(&readDir);

	return FIOERROR_OK;
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
		return FIOERROR_INVALID_PARAMETER;

	static int const modes[] = {
		FA_READ,							// 0: FIOMODE_RDONLY
		FA_WRITE, 							// 1: FIOMODE_WRONLY
		FA_READ|FA_WRITE,                 	// 2: FIOMODE_RDWR
		FA_READ|FA_WRITE|FA_CREATE_ALWAYS,	// 3: FIOMODE_RDWR_CREATE
	};
	if (mode >= sizeof(modes)/sizeof(*modes))
		return FIOERROR_INVALID_PARAMETER;

	STOInitialise();
	FRESULT result = f_open(f, filename.c_str(), modes[mode]);
	return convertError(result);
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
		return FIOERROR_OK;
	} else {
		FIL* f = getF(fileno);
		if (!f)
			return FIOERROR_INVALID_PARAMETER;

		FRESULT result = f_close(f);
		return convertError(result);
	}
}

uint8_t FISSeekFileHandle(uint8_t fileno, uint32_t offset) {
	FIL* f = getF(fileno);
	if (!f)
		return 1;

	FRESULT result = f_lseek(f, offset);
	return convertError(result);
}

uint8_t FISTellFileHandle(uint8_t fileno, uint32_t* offset) {
	FIL* f = getF(fileno);
	if (!f)
		return FIOERROR_INVALID_PARAMETER;

	*offset = f_tell(f);
	return FIOERROR_OK;
}

uint8_t FISReadFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	FIL* f = getF(fileno);
	if (!f)
		return FIOERROR_INVALID_PARAMETER;

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

	if ((result == FR_OK) && (read == 0))
		return FIOERROR_EOF;
	return convertError(result);
}

uint8_t FISWriteFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	FIL* f = getF(fileno);
	if (!f)
		return FIOERROR_INVALID_PARAMETER;

	uint16_t towrite = std::min(*size, uint16_t(0x10000 - address));

	// CONWriteString("FISWriteFileHandle(%d, @0x%04x, 0x%04x) -> ", fileno, address, *size);
	UINT written;
	FRESULT result = f_write(f, cpuMemory+address, towrite, &written);
	*size = written;
	// CONWriteString("%d, 0x%04x\r", result, written);

	if (written < towrite) {
		return FIOERROR_DENIED; /* out of disk space */
	}
	return convertError(result);
}

uint8_t FISGetSizeFileHandle(uint8_t fileno, uint32_t* size) {
	FIL* f = getF(fileno);
	if (!f)
		return FIOERROR_INVALID_PARAMETER;

	*size = f_size(f);
	// CONWriteString("FISGetSize(%d) -> 0x%08x\r", fileno, *size);
	return FIOERROR_OK;
}

uint8_t FISSetSizeFileHandle(uint8_t fileno, uint32_t size) {
	FIL* f = getF(fileno);
	if (!f)
		return FIOERROR_INVALID_PARAMETER;

	uint32_t oldPos = f_tell(f);
	// CONWriteString("FISSetSizeFileHandle(%d, 0x%08x) -> ", fileno, size);
	FRESULT result = f_lseek(f, size);
	if (result == FR_OK)
		result = f_truncate(f);
	// CONWriteString("%d\r", result);

	return convertError(result);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
