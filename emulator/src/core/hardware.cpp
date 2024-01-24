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
//									Read File
//
// ***************************************************************************************

uint8_t FISReadFile(const char *fileName,uint16_t loadAddress,uint16_t maxSize) {
	printf("Reading %s to $%x\n",fileName,loadAddress);
	char szFileName[64];
	sprintf(szFileName,"storage/%s",fileName);
	FILE *f = fopen(szFileName,"rb");
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
	char szFileName[64];
	sprintf(szFileName,"storage/%s",fileName);
	FILE *f = fopen(szFileName,"wb");
	if (f != NULL) {
		fwrite(CPUAccessMemory()+startAddress,1,size,f);
		fclose(f);
	}
	return (f == NULL) ? 1 : 0;
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

void SERInitialise(void) {
}

bool SERIsByteAvailable(void) {
	return false;
}

uint8_t SERReadByte(void) {
	return 0;
}

void SERCheckDataAvailable(void) {
}