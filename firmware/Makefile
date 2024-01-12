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
#		Can be SDCARD or USBKEY currently.
#
STORAGE = SDCARD

all: build

# ***************************************************************************************
#
#							Update the dispatcher generated code
#
# ***************************************************************************************

always:
	$(PYTHON) scripts$(S)makedispatch.py dispatch.config >include$(S)data$(S)dispatch_code.h
	$(CCOPY) $(BINDIR)basic_binary.h include$(S)data

# ***************************************************************************************
#
#									Rebuild the kernel		
#
# ***************************************************************************************

kernel: always
	make -C ..$(S)kernel
	$(CCOPY) $(BINDIR)kernel_binary.h include$(S)data
	$(CCOPY) $(BINDIR)basic_binary.h include$(S)data
	$(TOUCH) sources$(S)memory.c*

# ***************************************************************************************
#
#									Rebuild the firmware
#
# ***************************************************************************************

rebuild: clean build

# ***************************************************************************************
#
#									Build the firmware
#
# ***************************************************************************************

build: kernel always
	make -B -C bootdisplay
	$(PYTHON) scripts$(S)who.py >sources$(S)interface$(S)_who.c
	$(PYTHON) scripts$(S)keymaps.py >sources$(S)interface$(S)_locale.c
	$(PYTHON) scripts$(S)prompt.py >include$(S)data$(S)prompt.h
	$(PYTHON) scripts$(S)sfx.py >include$(S)data$(S)_sfxdata.h
	make -C build
	$(CCOPY) build$(S)sources$(S)$(BINARY).elf .
	$(CCOPY) build$(S)sources$(S)$(BINARY).uf2 .

# ***************************************************************************************
#
#								Clean build directories
#
# ***************************************************************************************

clean: always
	rm -Rf build
	export PICO_SDK_PATH=$(PICO_SDK_LOCATION) ; cmake -B build -S . -D$(STORAGE)_STORAGE=1

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

dual: kernel always
	$(CDEL)   firmware.elf firmware.uf2

	make -B -C bootdisplay
	$(PYTHON) scripts$(S)who.py >sources$(S)interface$(S)_who.c
	$(PYTHON) scripts$(S)keymaps.py >sources$(S)interface$(S)_locale.c
	$(PYTHON) scripts$(S)prompt.py >include$(S)data$(S)prompt.h
	$(PYTHON) scripts$(S)sfx.py >include$(S)data$(S)_sfxdata.h

	rm -Rf build
	export PICO_SDK_PATH=$(PICO_SDK_LOCATION) ; cmake -B build -S . -DSDCARD_STORAGE=1
	make -C build
	$(CCOPY) build$(S)sources$(S)$(BINARY).elf $(BINDIR)$(BINARY)_sd.elf
	$(CCOPY) build$(S)sources$(S)$(BINARY).uf2 $(BINDIR)$(BINARY)_sd.uf2

	rm -Rf build
	export PICO_SDK_PATH=$(PICO_SDK_LOCATION) ; cmake -B build -S . -DUSBKEY_STORAGE=1
	make -C build
	$(CCOPY) build$(S)sources$(S)$(BINARY).elf $(BINDIR)$(BINARY)_usb.elf
	$(CCOPY) build$(S)sources$(S)$(BINARY).uf2 $(BINDIR)$(BINARY)_usb.uf2

	make -B clean
	
# ***************************************************************************************
#
#									Release rebuild
#
# ***************************************************************************************

release:
	make -B dual
