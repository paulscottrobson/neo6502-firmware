# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		Makefile
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		20th November 2023
#		Reviewed :	No
#		Purpose :	Main firmware makefile, most of the work is done by CMake.
#
# ***************************************************************************************
# ***************************************************************************************

ifeq ($(OS),Windows_NT)
#is this really needed?  WSL2 is the 2023.
include ..\build_env\common.make
else
include ../build_env/common.make
endif

BINARY = firmware
#
#		Can be SDCARD or USB currently. If you change this use make switch to
#		rebuild as it cmake clean doesn't clean things up enough (probably me)
#
STORAGE = USB

all: build

always:

# ***************************************************************************************
#
#									Rebuild the kernel		
#
# ***************************************************************************************

kernel: always
	$(MAKE) -C ..$(S)kernel
	$(CCOPY) $(BINDIR)kernel_binary.h $(COMDIR)include$(S)data
	$(CCOPY) $(BINDIR)basic_binary.h $(COMDIR)include$(S)data
	$(TOUCH) sources$(S)memory.c*

# ***************************************************************************************
#
#									Rebuild the firmware
#
# ***************************************************************************************

switch: 
	rm -Rf build
	$(MAKE) rebuild 

rebuild: clean build

# ***************************************************************************************
#
#							  Remake the preliminary files
#
# ***************************************************************************************

prelim: kernel
	$(MAKE) -B -C $(COMDIR)
	
# ***************************************************************************************
#
#									Build the firmware
#
# ***************************************************************************************

build: prelim always
	cmake --fresh -B build -DSTORAGE_TYPE=$(STORAGE)
	$(MAKE) -C build
	$(CCOPY) build$(S)sources$(S)$(BINARY).elf .
	$(CCOPY) build$(S)sources$(S)$(BINARY).uf2 .

# ***************************************************************************************
#
#								Clean build directories
#
# ***************************************************************************************

clean: always
	cmake --fresh -B build -DSTORAGE_TYPE=$(STORAGE)
	cmake --build build --target clean

# ***************************************************************************************
#
#								Build & Upload firmware	
#
# ***************************************************************************************

run: build upload

upload: 
	$(UPLOADER) $(UPCONFIG) $(UPCOMMANDS)

# ***************************************************************************************
#
#				Send last BASIC program translated over the serial port.
#
# ***************************************************************************************

checkserial:
	$(PYTHON) $(BINDIR)nxmit.zip /dev/ttyUSB0 b:$(ROOTDIR)basic$(S)build$(S)tokenised.dat

# ***************************************************************************************
#
#									Reset RP2040
#
# ***************************************************************************************

reset:
	$(UPLOADER) $(UPCONFIG) -c "adapter speed 5000" -c "init" -c "reset init" -c "reset run" -c "exit"

# ***************************************************************************************
#
#								Build both firmware
#
# ***************************************************************************************

dual: prelim always pythonapps
	$(CDEL)   firmware.elf firmware.uf2

	cmake -B build -DSTORAGE_TYPE=SDCARD
	cmake --build build --target clean
	$(MAKE) -C build
	$(CCOPY) build$(S)sources$(S)$(BINARY).elf $(BINDIR)$(BINARY)_sd.elf
	$(CCOPY) build$(S)sources$(S)$(BINARY).uf2 $(BINDIR)$(BINARY)_sd.uf2

	cmake -B build -DSTORAGE_TYPE=USB
	cmake --build build --target clean
	$(MAKE) -C build
	$(CCOPY) build$(S)sources$(S)$(BINARY).elf $(BINDIR)$(BINARY)_usb.elf
	$(CCOPY) build$(S)sources$(S)$(BINARY).uf2 $(BINDIR)$(BINARY)_usb.uf2

pythonapps:
	$(CMAKEDIR) build
	$(CCOPY) scripts$(S)nxmit.py build$(S)__main__.py
	zip -j $(BINDIR)nxmit.zip build$(S)__main__.py
	$(CDEL) build$(S)*.p
		
# ***************************************************************************************
#
#									Release rebuild
#
# ***************************************************************************************

release:
	$(MAKE) -B dual
