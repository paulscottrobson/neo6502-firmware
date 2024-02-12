// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		hardware.c
//		Purpose:	Hardware Emulation
//		Created:	22nd November 2023
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#include "sys_processor.h"
#include "hardware.h"
#include "gfx.h"
#include <stdio.h>
#include "common.h"
#include "interface/kbdcodes.h"
#include "sys/stat.h"
#include "sys/types.h"
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <filesystem>

static FILE* fileHandles[FIO_NUM_FILES];
static int frameCount = 0;
static std::filesystem::path storagePath = "storage";
static std::filesystem::path currentPath = storagePath;

static std::string getAbspath(const std::string& path) {
	std::filesystem::path newPath;
	if (!path.empty() && (path[0] == '/'))
		newPath = storagePath / path.substr(1);
	else
		newPath = currentPath / path;
	return newPath.string();
}

// *******************************************************************************************************************************
//
//												Reset Hardware
//
// *******************************************************************************************************************************

void HWReset(void) {
	std::filesystem::create_directories(currentPath);
}

// *******************************************************************************************************************************
//
//											 Frame Sync any hardware
//
// *******************************************************************************************************************************

void HWSync(void) {
	TICKProcess();
	frameCount++;
}

int  RNDGetFrameCount(void) {
	return frameCount;
}

// *******************************************************************************************************************************
//
//												Read 100Hz timer
//
// *******************************************************************************************************************************

uint32_t TMRRead(void) {
	return GFXTimer() / 10+104867;
}

// *******************************************************************************************************************************
//
//											Keyboard Sync/Initialise dummies
//
// *******************************************************************************************************************************

void KBDSync(void) { }
void KBDInitialise(void) { }

// *******************************************************************************************************************************
//
//											   Sound system initialise
//
// *******************************************************************************************************************************

void SNDInitialise(void) {
}

// *******************************************************************************************************************************
//
//											   Sound system set pitch
//
// *******************************************************************************************************************************

void SNDSetFrequency(uint8_t channel,uint16_t frequency,bool isNoise) {
	//printf("%d %d %d\n",channel,frequency,isNoise);
	if (frequency != 0) {
		GFXSetFrequency(frequency,1);
	} else {
		GFXSilence();
	}
}

// *******************************************************************************************************************************
//
//										  Receive SDL Keyboard events
//
// *******************************************************************************************************************************

#include "hid2sdl.h"

void HWQueueKeyboardEvent(int sdlCode,int isDown) {
	int found = -1;
	for (int i = 0;i < SDL2HIDSIZE;i++) {
		if (SDL2HIDMapping[i] == sdlCode) found = i;
	}
	if (found >= 0) {
		int modifier = 0;
		if (GFXIsKeyPressed(GFXKEY_SHIFT)) modifier |= KEY_SHIFT;
		if (GFXIsKeyPressed(GFXKEY_CONTROL)) modifier |= KEY_CONTROL;
		if (GFXIsKeyPressed(GFXKEY_ALT)) modifier |= KEY_ALT;
		if (GFXIsKeyPressed(GFXKEY_ALTGR)) modifier |= KEY_ALTGR;
		KBDEvent(isDown,found,modifier);
	}
}

// *******************************************************************************************************************************
//
//												Dummy debug write
//
// *******************************************************************************************************************************

void FDBWrite(uint8_t c) {
}

// ***************************************************************************************
//
//									Read File
//
// ***************************************************************************************

uint8_t FISReadFile(const std::string& fileName,uint16_t loadAddress,uint16_t maxSize) {
	std::string abspath = getAbspath(fileName);
	printf("Reading %s to $%x\n",abspath.c_str(),loadAddress);
	FILE *f = fopen(abspath.c_str(), "rb");
	if (f != NULL) {
		size_t r;
		if (loadAddress == 0xFFFF) {
			r = fread(gfxMemory,1,maxSize,f);
		} else {
			r = fread(CPUAccessMemory()+loadAddress,1,maxSize,f);
		}
		fclose(f);

		if (r != maxSize)
			return 1;
	}
	return (f == NULL) ? 1 : 0;
}

// ***************************************************************************************
//
//									Write File
//
// ***************************************************************************************

uint8_t FISWriteFile(const std::string& fileName,uint16_t startAddress,uint16_t size) {
	std::string abspath = getAbspath(fileName);
	printf("Writing %s from $%x size $%x\n",abspath.c_str(),startAddress,size);
	FILE *f = fopen(abspath.c_str(), "wb");
	if (f != NULL) {
		fwrite(CPUAccessMemory()+startAddress,1,size,f);
		fclose(f);
	}
	return (f == NULL) ? 1 : 0;
}

// ***************************************************************************************
//
//									Rename file
//
// ***************************************************************************************

uint8_t FISRenameFile(const std::string& oldFilename, const std::string& newFilename) {
	std::string oldAbspath = getAbspath(oldFilename);
	std::string newAbspath = getAbspath(newFilename);
	printf("Renaming %s to %s: ", oldAbspath.c_str(), newAbspath.c_str());
	std::error_code ec;
	std::filesystem::rename(oldAbspath, newAbspath, ec);
	if (ec)
		printf("%s\n", ec.message().c_str());
	else
		printf("OK\n");

	return !ec ? 1 : 0;
}

// ***************************************************************************************
//
//									Delete file
//
// ***************************************************************************************

uint8_t FISDeleteFile(const std::string& filename) {
	std::string abspath = getAbspath(filename);
	printf("FISDeleteFile('%s') -> ", abspath.c_str());
	std::error_code ec;
	bool removed = std::filesystem::remove(abspath, ec);
	if (ec || !removed) {
		printf("%s\n", ec.message().c_str());
		return 1;
	} else {
		printf("OK\n");
		return 0;
	}
}

// ***************************************************************************************
//
//									Create directory
//
// ***************************************************************************************

uint8_t FISCreateDirectory(const std::string& filename) {
	std::string abspath = getAbspath(filename);
	printf("FISCreateDirectory('%s') -> ", abspath.c_str());
	std::error_code ec;
	std::filesystem::create_directory(abspath, ec);
	if (ec)
		printf("%s\n", ec.message().c_str());
	else
		printf("OK\n");

	return !ec ? 1 : 0;
}

// ***************************************************************************************
//
//									Change directory
//
// ***************************************************************************************

uint8_t FISChangeDirectory(const std::string& filename) {
	std::string abspath = getAbspath(filename);
	printf("FISChangeDirectory('%s') -> ", abspath.c_str());

	std::error_code ec;
	auto status = std::filesystem::status(abspath, ec);

	if (!ec && (status.type() == std::filesystem::file_type::directory)) {
		printf("OK\n");
		currentPath = abspath;
		return 0;
	} else {
		return 1;
	}
}

// ***************************************************************************************
//
//									   Stat file
//
// ***************************************************************************************

uint8_t FISStatFile(const std::string& filename, uint32_t* length, uint8_t* attribs) {
	std::string abspath = getAbspath(filename);
	printf("FISStatFile('%s') -> ", abspath.c_str());

	try {
		*length = std::filesystem::file_size(abspath);
		*attribs = std::filesystem::is_directory(abspath) ? FIOATTR_DIR : 0;
		printf("OK\n");
		return 0;
	} catch (const std::filesystem::filesystem_error& e) {
		printf("%s\n", e.what());
		return 1;
	}
}

// ***************************************************************************************
//
//								Directory enumeration
//
// ***************************************************************************************

static std::filesystem::directory_iterator readDirIterator;

uint8_t FISOpenDir(const std::string& filename) {
	std::string abspath = getAbspath(filename);
	printf("FISOpenDir('%s') -> ", abspath.c_str());
	errno = 0;
	try {
		readDirIterator = std::filesystem::directory_iterator(abspath);
		printf("OKs\n");
		return 0;
	} catch (const std::filesystem::filesystem_error& e) {
		printf("%s\n", e.what());
		return 1;
	}
}

uint8_t FISReadDir(std::string& filename, uint32_t* size, uint8_t* attribs) {
	printf("FISReadDir() -> ");

	if (readDirIterator != std::filesystem::directory_iterator()) {
		const auto& de = *readDirIterator++;

		filename = de.path().filename().string();
		*attribs = de.is_directory() ? FIOATTR_DIR : 0;
		*size = de.is_regular_file() ? de.file_size() : 0;
		printf("OK: '%s'\n", filename.c_str());
		return 0;
	} else {
		printf("Failed\n");
		return 1;
	}
}

uint8_t FISCloseDir() {
	readDirIterator = std::filesystem::directory_iterator();
	return 0;
}

// ***************************************************************************************
//
//								File-handle based functions
//
// ***************************************************************************************

uint8_t FISOpenFileHandle(uint8_t fileno, const std::string& filename, uint8_t mode) {
	std::string abspath = getAbspath(filename);

	if (fileno >= FIO_NUM_FILES)
		return 1;

	/* Check if already open. */
	if (fileHandles[fileno])
		return 1;

	static const char* const modes[] = {
		"rb",	// 0: FIOMODE_RDONLY
		"r+b", 	// 1: FIOMODE_WRONLY
		"r+b",	// 2: FIOMODE_RDWR
		"w+b",	// 3: FIOMODE_RDWR_CREATE
	};
	if (mode >= sizeof(modes)/sizeof(*modes))
		return 1;

	fprintf(stderr, "FISOpenFileHandle(%d, '%s', 0x%02x) -> ", fileno, abspath.c_str(), mode);
	errno = 0;
	fileHandles[fileno] = fopen(abspath.c_str(), modes[mode]);
	fprintf(stderr, "%s\n", strerror(errno));
	return fileHandles[fileno] ? 0 : 1;
}

static FILE* getF(uint8_t fileno) {
	if (fileno >= FIO_NUM_FILES)
		return NULL;
	return fileHandles[fileno];
}

uint8_t FISCloseFileHandle(uint8_t fileno) {
	fprintf(stderr, "FISCloseFileHandle(%d)\n", fileno);
	if (fileno == 0xff) {
		for (FILE*& f : fileHandles) {
			if (f) {
				fclose(f);
				f = NULL;
			}
		}
		return 0;
	} else {
		FILE* f = getF(fileno);
		if (!f)
			return 1;

		int result = fclose(f);
		fileHandles[fileno] = NULL;
		return !result ? 0 : 1;
	}
}

uint8_t FISSeekFileHandle(uint8_t fileno, uint32_t offset) {
	printf("FISSeekFileHandle(%d, %u)\n", fileno, offset);
	FILE* f = getF(fileno);
	if (!f)
		return 1;

	int result = fseek(f, offset, SEEK_SET);
	return (result >= 0) ? 0 : 1;
}

uint8_t FISTellFileHandle(uint8_t fileno, uint32_t* offset) {
	FILE* f = getF(fileno);
	if (!f)
		return 1;

	*offset = ftell(f);
	return 0;
}

uint8_t FISReadFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	FILE* f = getF(fileno);
	if (!f)
		return 1;

	errno = 0;
	printf("FISReadFileHandle(%d, @0x%x, %d) -> ", fileno, address, *size);
	size_t result = fread(cpuMemory+address, *size, 1, f);
	printf("%d: %s\n", (int)result, (result != *size) ? strerror(errno) : "OK");
	*size = result;

	return (result > 0) ? 0 : 1;
}

uint8_t FISWriteFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	FILE* f = getF(fileno);
	if (!f)
		return 1;

	printf("FISWriteFileHandle(%d, @0x%x, %d) -> ", fileno, address, *size);
	size_t result = fwrite(cpuMemory+address, *size, 1, f);
	printf("%d: %s\n", (int)result, (result != *size) ? strerror(errno) : "OK");
	*size = result;

	return (result > 0) ? 0 : 1;
}

uint8_t FISGetSizeFileHandle(uint8_t fileno, uint32_t* size) {
	FILE* f = getF(fileno);
	if (!f)
		return 1;

	printf("FISGetSizeFileHandle(%d) -> ", fileno);
	errno = 0;
	size_t oldpos = fseek(f, 0, SEEK_END);
	*size = ftell(f);
	fseek(f, SEEK_SET, oldpos);
	printf("%d: %s\n", *size, strerror(errno));

	return 0;
}

uint8_t FISSetSizeFileHandle(uint8_t fileno, uint32_t size) {
	FILE* f = getF(fileno);
	if (!f)
		return 1;

	printf("FISSetSizeFileHandle(%d, %d) -> ", fileno, size);
	errno = 0;
	fflush(f);
	int result = ftruncate(::fileno(f), size);
	printf("%s\n", strerror(errno));

	return !result ? 0 : 1;
}

// ***************************************************************************************
//
//								Dummy initialise & synchronise
//
// ***************************************************************************************

void STOInitialise(void) {
}

void STOSynchronise(void) {
	CONWriteString("Stored in 'storage' directory\r");
}

// ***************************************************************************************
//
//								Dummy serial functions
//
// ***************************************************************************************

bool SERInitialise(void) {
	return false;
}

bool SERIsByteAvailable(void) {
	return false;
}

uint8_t SERReadByte(void) {
	return 0;
}
