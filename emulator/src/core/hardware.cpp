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

static FILE* fileHandles[FIO_NUM_FILES];
static int frameCount = 0;

// *******************************************************************************************************************************
//
//												Reset Hardware
//
// *******************************************************************************************************************************

void HWReset(void) {
	#ifdef LINUX
	mkdir("storage",0755);
	#else
	mkdir("storage");
	#endif
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

// *******************************************************************************************************************************
//
//											 Dummy directory listing.
//
// *******************************************************************************************************************************

static struct dirent * dirEntry;
static DIR *directory;

int FISDirectoryOpen(void) {
    directory = opendir("storage");
	return (directory == NULL) ? 1 : 0;
}

int FISDirectoryClose(void) {
    if (directory != NULL) closedir(directory);
	return 0;
}

int FISDirectoryNext(char *buffer,int *isDirectory,int *fileSize) {
	dirEntry = readdir(directory);
	if (dirEntry != NULL) {
		strcpy(buffer,dirEntry->d_name);
		*isDirectory = 0;
		*fileSize = -1;
	}
	return (dirEntry == NULL) ? 1 : 0;
}   

// ***************************************************************************************
//
//									File utils
//
// ***************************************************************************************

static std::string getAbspath(std::string filename) {
	return "storage/" + filename;
}

// ***************************************************************************************
//
//									Read File
//
// ***************************************************************************************

uint8_t FISReadFile(const char *fileName,uint16_t loadAddress,uint16_t maxSize) {
	printf("Reading %s to $%x\n",fileName,loadAddress);
	FILE *f = fopen(getAbspath(fileName).c_str(), "rb");
	if (f != NULL) {
		int b;
		if (loadAddress == 0xFFFF) {
			b = fread(gfxMemory,1,maxSize,f);
		} else {
			b = fread(CPUAccessMemory()+loadAddress,1,maxSize,f);
		}
		fclose(f);
	}
	return (f == NULL) ? 1 : 0;
}

// ***************************************************************************************
//
//									Write File
//
// ***************************************************************************************

uint8_t FISWriteFile(const char *fileName,uint16_t startAddress,uint16_t size) {
	printf("Writing %s from $%x size $%x\n",fileName,startAddress,size);
	FILE *f = fopen(getAbspath(fileName).c_str(), "wb");
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
	printf("Renaming %s to %s: ", oldFilename.c_str(), newFilename.c_str());
	errno = 0;
	rename(getAbspath(oldFilename).c_str(), getAbspath(newFilename).c_str());
	printf("%s\n", strerror(errno));

	return (errno == 0) ? 1 : 0;
}

// ***************************************************************************************
//
//								File-handle based functions
//
// ***************************************************************************************

uint8_t FISOpenFileHandle(uint8_t fileno, const char* filename, uint8_t mode) {
	fprintf(stderr, "OpenFileHandle(%d, '%s', 0x%02x)\n", fileno, filename, mode);

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

	errno = 0;
	fileHandles[fileno] = fopen(getAbspath(filename).c_str(), modes[mode]);
	fprintf(stderr, "Result: %s\n", strerror(errno));
	return fileHandles[fileno] ? 0 : 1;
}

static FILE* getF(uint8_t fileno) {
	if (fileno >= FIO_NUM_FILES)
		return NULL;
	return fileHandles[fileno];
}

uint8_t FISCloseFileHandle(uint8_t fileno) {
	fprintf(stderr, "CloseFileHandle(%d)\n", fileno);
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
		return !result ? 0 : 1;
	}
}

uint8_t FISSeekFileHandle(uint8_t fileno, uint32_t offset) {
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

	size_t result = fread(cpuMemory+address, *size, 1, f);
	*size = result;

	return (result >= 0) ? 0 : 1;
}

uint8_t FISWriteFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	FILE* f = getF(fileno);
	if (!f)
		return 1;

	size_t result = fwrite(cpuMemory+address, *size, 1, f);
	*size = result;

	return (result >= 0) ? 0 : 1;
}

uint8_t FISGetSizeFileHandle(uint8_t fileno, uint32_t* size) {
	FILE* f = getF(fileno);
	if (!f)
		return 1;

	size_t oldpos = fseek(f, SEEK_END, 0);
	*size = ftell(f);
	fseek(f, SEEK_SET, oldpos);

	return 0;
}

uint8_t FISSetSizeFileHandle(uint8_t fileno, uint32_t size) {
	FILE* f = getF(fileno);
	if (!f)
		return 1;

	fflush(f);
	int result = ftruncate(::fileno(f), size);

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
