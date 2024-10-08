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

#include "gfx.h"
#include "sys_processor.h"
#include "sys_debug_system.h"
#include "hardware.h"
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
#include <vector>
#include "serial_emu.h"

#define DEFAULT_STORAGE "storage"

static const SerialInterface* serialInterface = NULL;
static FILE* fileHandles[FIO_NUM_FILES];
static int frameCount = 0;
static std::filesystem::path storagePath = DEFAULT_STORAGE;
static std::filesystem::path currentPath = storagePath;

// *******************************************************************************************************************************
//
//												Storage path management
//
// *******************************************************************************************************************************

void HWSetDefaultPath(const char *defaultPath) {
	storagePath = defaultPath;
	currentPath = defaultPath;
}

static std::string getAbspath(const std::string& path) {
	std::filesystem::path newPath;
	if (!path.empty() && (path[0] == '/'))
		newPath = storagePath / path.substr(1);
	else
		newPath = currentPath / path;
	return newPath.string();
}

static uint8_t getAttributes(const std::string& filename) {
	using std::filesystem::perms;
	std::filesystem::file_status status = std::filesystem::status(filename);

	return ((status.type() == std::filesystem::file_type::directory) ? FIOATTR_DIR : 0) |
		((status.permissions() & perms::owner_write) == perms::none ? FIOATTR_READONLY : 0);
}

static uint8_t convertError(const std::error_code& errcode) {
	static const std::vector<std::pair<std::errc, FIOErrno>> errorsList = {
		{ std::errc::no_such_device, FIOERROR_INVALID_NAME },
		{ std::errc::file_exists, FIOERROR_EXIST },
		{ std::errc::permission_denied, FIOERROR_DENIED },
		{ std::errc::is_a_directory, FIOERROR_DENIED },
		{ std::errc::not_a_directory, FIOERROR_NO_PATH },
		{ std::errc::no_space_on_device, FIOERROR_DENIED },
		{ std::errc::filename_too_long, FIOERROR_INVALID_PARAMETER },
		{ std::errc::file_too_large, FIOERROR_DENIED },
		{ std::errc::directory_not_empty, FIOERROR_DENIED },
	};

	if (!errcode) {
		return FIOERROR_OK;
	}

	for (const auto& it : errorsList) {
		if (errcode == it.first) {
			return it.second;
		}
	}
	return FIOERROR_UNKNOWN;
}

static uint8_t convertError(int e) {
	const std::error_code ec = { e, std::system_category() };
	return convertError(ec);
}


// *******************************************************************************************************************************
//
//												Reset Hardware
//
// *******************************************************************************************************************************

void HWReset(void) {
	std::filesystem::create_directories(currentPath);
	MSEEnableMouse();
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
	return GFXTimer() / 10;
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

// // *******************************************************************************************************************************
// //
// //											   Sound system set pitch
// //
// // *******************************************************************************************************************************

// void SNDUpdateSoundChannel(uint8_t channel,SOUND_CHANNEL *c) {
// 	if (c->isPlayingNote != 0) {
// 		GFXSetFrequency(c->currentFrequency,1);
// 	} else {
// 		GFXSilence();
// 	}
// }

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
	fputc(c,stderr);
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

	return convertError(ec);
}

// ***************************************************************************************
//
//									 Copy file 
//
// ***************************************************************************************

uint8_t FISCopyFile(const std::string& oldFilename, const std::string& newFilename) {
	std::string oldAbspath = getAbspath(oldFilename);
	std::string newAbspath = getAbspath(newFilename);
	printf("FISCopyFile('%s', '%s') -> ", oldAbspath.c_str(), newAbspath.c_str());
	std::error_code ec;
	std::filesystem::copy(oldAbspath, newAbspath,
		std::filesystem::copy_options::overwrite_existing, ec);
	if (ec)
		printf("%s\n", ec.message().c_str());
	else
		printf("OK\n");

	return convertError(ec);
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
		return convertError(ec);
	} else {
		printf("OK\n");
		return FIOERROR_OK;
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

	return convertError(ec);
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
		return FIOERROR_OK;
	} else {
		return convertError(ec);
	}
}

// ***************************************************************************************
//
//								Read current directory
//
// ***************************************************************************************

uint8_t FISGetCurrentDirectory(char *target,int maxSize) {
	strcpy(target,(const char *)currentPath.c_str()+strlen(DEFAULT_STORAGE)-1);
	*target = '/';
	printf("FISGetCurrentDirectory() ->\n");
	return FIOERROR_OK;
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
		if (!std::filesystem::is_directory(abspath)) {
			*length = std::filesystem::file_size(abspath);
		}
		*attribs = getAttributes(abspath);
		printf("OK; length=0x%04x; permissions=0x%02x\n", *length, *attribs);
		return FIOERROR_OK;
	} catch (const std::filesystem::filesystem_error& e) {
		printf("%s\n", e.what());
		return convertError(e.code());
	}
}

// ***************************************************************************************
//
//									Set file attributes
//
// ***************************************************************************************

uint8_t FISSetFileAttributes(const std::string& filename, uint8_t attribs) {
	std::string abspath = getAbspath(filename);
	printf("FISSetFileAttributes('%s') -> ", abspath.c_str());

	try {
		using std::filesystem::perms;
		std::filesystem::permissions(abspath,
			(std::filesystem::perms)((attribs & FIOATTR_READONLY) ? 0444 : 0666),
			std::filesystem::perm_options::replace);
		printf("OK\n");
		return FIOERROR_OK;
	} catch (const std::filesystem::filesystem_error& e) {
		printf("%s\n", e.what());
		return convertError(e.code());
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
		printf("OK\n");
		return FIOERROR_OK;
	} catch (const std::filesystem::filesystem_error& e) {
		printf("%s\n", e.what());
		return convertError(e.code());
	}
}

uint8_t FISReadDir(std::string& filename, uint32_t* size, uint8_t* attribs) {
	printf("FISReadDir() -> ");

	if (readDirIterator != std::filesystem::directory_iterator()) {
		const auto& de = *readDirIterator++;

		filename = de.path().filename().string();
		*size = de.is_regular_file() ? de.file_size() : 0;
		*attribs = getAttributes(de.path().string());
		printf("OK: '%s', length=0x%04x, attribus=0x%02x\n", filename.c_str(), *size, *attribs);
		return FIOERROR_OK;
	} else {
		printf("Failed\n");
		return FIOERROR_EOF;
	}
}

uint8_t FISCloseDir() {
	readDirIterator = std::filesystem::directory_iterator();
	return FIOERROR_OK;
}

// ***************************************************************************************
//
//								File-handle based functions
//
// ***************************************************************************************

uint8_t FISOpenFileHandle(uint8_t fileno, const std::string& filename, uint8_t mode) {
	std::string abspath = getAbspath(filename);

	if (fileno >= FIO_NUM_FILES)
		return FIOERROR_INVALID_PARAMETER;

	/* Check if already open. */
	if (fileHandles[fileno])
		return FIOERROR_INVALID_PARAMETER;

	static const char* const modes[] = {
		"rb",	// 0: FIOMODE_RDONLY
		"r+b", 	// 1: FIOMODE_WRONLY
		"r+b",	// 2: FIOMODE_RDWR
		"w+b",	// 3: FIOMODE_RDWR_CREATE
	};
	if (mode >= sizeof(modes)/sizeof(*modes))
		return FIOERROR_INVALID_PARAMETER;

	fprintf(stderr, "FISOpenFileHandle(%d, '%s', 0x%02x) -> ", fileno, abspath.c_str(), mode);
	errno = 0;
	fileHandles[fileno] = fopen(abspath.c_str(), modes[mode]);
	fprintf(stderr, "%s\n", strerror(errno));
	return fileHandles[fileno] ? FIOERROR_OK : convertError(errno);
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
		return FIOERROR_OK;
	} else {
		FILE* f = getF(fileno);
		if (!f)
			return FIOERROR_INVALID_PARAMETER;

		int result = fclose(f);
		fileHandles[fileno] = NULL;
		return !result ? FIOERROR_OK : convertError(errno);
	}
}

uint8_t FISSeekFileHandle(uint8_t fileno, uint32_t offset) {
	printf("FISSeekFileHandle(%d, %u)\n", fileno, offset);
	FILE* f = getF(fileno);
	if (!f)
		return FIOERROR_INVALID_PARAMETER;

	int result = fseek(f, offset, SEEK_SET);
	return (result >= 0) ? FIOERROR_OK : convertError(errno);
}

uint8_t FISTellFileHandle(uint8_t fileno, uint32_t* offset) {

	FILE* f = getF(fileno);
	if (!f)
		return FIOERROR_INVALID_PARAMETER;

	*offset = ftell(f);
	printf("FISTellFileHandle(%d, %d) -> ", fileno, *offset);
	return FIOERROR_OK;
}

uint8_t FISReadFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	FILE* f = getF(fileno);
	if (!f)
		return FIOERROR_INVALID_PARAMETER;

	errno = 0;
	printf("FISReadFileHandle(%d, @0x%x, %d) -> ", fileno, address, *size);
	size_t result;
	if (address != 0xFFFF) {
		result = fread(cpuMemory+address, 1,*size, f);
	} else {
		result = fread(gfxObjectMemory,1,*size,f);
	}
	printf("%d: %s\n", (int)result, (result != *size) ? strerror(errno) : "OK");
	*size = result;

	if ((errno == 0) && (result == 0)) {
		return FIOERROR_EOF;
	}
	return convertError(errno);
}

uint8_t FISWriteFileHandle(uint8_t fileno, uint16_t address, uint16_t* size) {
	FILE* f = getF(fileno);
	if (!f)
		return FIOERROR_INVALID_PARAMETER;

	printf("FISWriteFileHandle(%d, @0x%x, %d) -> ", fileno, address, *size);
	size_t result = fwrite(cpuMemory+address, *size, 1, f);
	printf("%d: %s\n", (int)result, (result != 1) ? strerror(errno) : "OK");
	//*size = result;

	if (result == 1) {
		return FIOERROR_OK;
	}
	return convertError(errno);
}

uint8_t FISGetSizeFileHandle(uint8_t fileno, uint32_t* size) {
	FILE* f = getF(fileno);
	if (!f)
		return FIOERROR_INVALID_PARAMETER;

	printf("FISGetSizeFileHandle(%d) -> ", fileno);
	errno = 0;
	size_t oldpos = ftell(f);
	fseek(f, 0, SEEK_END);
	*size = ftell(f);
	fseek(f, oldpos, SEEK_SET);
	printf("%d: %s\n", *size, strerror(errno));

	return FIOERROR_OK;
}

uint8_t FISSetSizeFileHandle(uint8_t fileno, uint32_t size) {
	FILE* f = getF(fileno);
	if (!f)
		return FIOERROR_INVALID_PARAMETER;

	printf("FISSetSizeFileHandle(%d, %d) -> ", fileno, size);
	errno = 0;
	fflush(f);
	int result = ftruncate(::fileno(f), size);
	printf("%s\n", strerror(errno));

	return !result ? FIOERROR_OK : convertError(errno);
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
//								Emulated serial functions
//
// ***************************************************************************************

bool SERInitialise(void) {
	return false;
}

bool SERIsByteAvailable(void) {
    return serialInterface ? serialInterface->isByteAvailable() : false;
}

uint8_t SERReadByte(void) {
    return serialInterface ? serialInterface->readByte() : 0;
}

void SERWriteByte(uint8_t b) {
    if (serialInterface) {
        serialInterface->writeByte(b);
    }
}

void SERSetSerialFormat(uint32_t baudRate, uint32_t protocol) {
    // Initialize the interface if it hasn't been set yet
    if (!serialInterface) {
        serialInterface = SerialInterfaceOpen(SER_TCP);
    }
    
    if (serialInterface) {
        serialInterface->setSerialFormat(baudRate, protocol);
    }
}


// ***************************************************************************************
//
//								Dummy GPIO functions
//
// ***************************************************************************************

int UEXTSetGPIODirection(int gpio,int pinType) {
	//printf("Pin %d set direction to %d\n",gpio,pinType);
	return 0;
}

int UEXTSetGPIO(int gpio,bool isOn) {
	printf("Set Pin %d to %d\n",gpio,isOn ? 1 : 0);
	return 0;
}

int UEXTGetGPIO(int gpio,bool *pIsHigh) {
	printf("Read pin %d, value is (not) %d\n",gpio,gpio & 1);
	*pIsHigh = (gpio & 1) != 0;
	return 0;
}

int UEXTGetGPIOAnalogue(int gpio,uint16_t *pLevel) {
	*pLevel = gpio+1000;
	printf("Read Analogue pin %d, value is (not) %d\n",gpio,*pLevel);
	return 0;
}

// ***************************************************************************************
//
//                                     Dummy I2C functions
//
// ***************************************************************************************

int UEXTI2CInitialise(void) {
	printf("I2C Initialise\n");
    return 0;
}

// ***************************************************************************************
//
//                          	Write bytes to I2C device
//
// ***************************************************************************************

int UEXTI2CWriteBlock(uint8_t device,uint8_t *data,size_t size) {
	printf("I2C Write to $%02x %d bytes\n",device,(int)size);
	for (int i = 0;i < size;i++) {
		printf(" $%02x",data[i]);
	}
	printf("\n");
	return 0;
}

// ***************************************************************************************
//
//                          	Read bytes from I2C device 
//
// ***************************************************************************************

int UEXTI2CReadBlock(uint8_t device,uint8_t *data,size_t size) {
	if (device == 0x7F) return 1;
	printf("I2C Read from $%x %d bytes\n",device,(int)size);
	for (int i = 0;i < size;i++) {
		data[i] = device + 0x12 + i * 3;
		printf(" $%02x",data[i]);
	}
	printf("\n");
    return 0;
}

// ***************************************************************************************
//
//                                     Dummy SPI functions
//
// ***************************************************************************************

int UEXTSPIInitialise(void) {
	printf("SPI Initialise\n");
    return 0;
}

// ***************************************************************************************
//
//                          Write bytes to SPI device
//
// ***************************************************************************************

int UEXTSPIWriteBlock(uint8_t *data,size_t size) {
	printf("SPI Write to %d bytes\n",(int)size);
	for (int i = 0;i < size;i++) {
		printf(" $%02x",data[i]);
	}
	printf("\n");
	return 0;
}

// ***************************************************************************************
//
//                          Read bytes from I2C device
//
// ***************************************************************************************

int UEXTSPIReadBlock(uint8_t *data,size_t size) {
	printf("SPI Read from %d bytes\n",(int)size);
	for (int i = 0;i < size;i++) {
		data[i] = 0x12 + i * 3;
		printf(" $%02x",data[i]);
	}
	printf("\n");
    return 0;
}
// ***************************************************************************************
//
//                          			Hardware reset
//
// ***************************************************************************************

void ResetSystem(void) {
	printf("Hardware reset.\n");
}

// ***************************************************************************************
//
//                          			Dummy Gamepad
//
// ***************************************************************************************

uint8_t GMPGetControllerCount(void) {
	return GFXControllerCount();
}

// ***************************************************************************************
//
//                          		Read controller status
//
// ***************************************************************************************

uint32_t GMPReadDigitalController(uint8_t index) {
	return GFXReadController(index);
}

// ***************************************************************************************
//
//                          		 Handle dispatch warning.
//
// ***************************************************************************************

void DSPWarnHandler(uint8_t group,uint8_t func) {
	fprintf(stderr,"** WARN ** Execute %d.%d not defined.\n",group,func);
}

// ***************************************************************************************
//
//                          Update mouse state - move or buttons
//
// ***************************************************************************************

void HWUpdateMouse(void) {
	int x,y;
	SDL_Rect r;
	int xScale,yScale,xWidth,yWidth;

	Uint32 sbut = SDL_GetMouseState(&x,&y);
	DGBXGetActiveDisplayInfo(&r,&xScale,&yScale,&xWidth,&yWidth);

	if (x >= r.x && y >= r.y && x < r.x+r.w && y < r.y+r.h) {
		x = (x - r.x) / xScale;y = (y - r.y) / yScale;
		int buttons = 0;
		if (sbut & SDL_BUTTON(1)) buttons |= 0x1;
		if (sbut & SDL_BUTTON(3)) buttons |= 0x2;
		if (sbut & SDL_BUTTON(2)) buttons |= 0x4;
		// printf("%x %x %x\n",buttons,x,y);
		MSESetPosition(x & 0xFFFF,y & 0xFFFF);
		MSEUpdateButtonState(buttons & 0xFF);
	}
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
