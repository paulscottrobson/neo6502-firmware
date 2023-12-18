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

all: build

always:
	$(PYTHON) scripts$(S)makedispatch.py dispatch.config >include$(S)data$(S)dispatch_code.h

kernel: always
	make -C ..$(S)kernel
	$(CCOPY) $(BINDIR)kernel_binary.h include$(S)data
	$(CCOPY) $(BINDIR)a1basic_binary.h include$(S)data
	$(CCOPY) $(BINDIR)figforth_binary.h include$(S)data
	$(CCOPY) $(BINDIR)basic_binary.h include$(S)data
	$(TOUCH) sources$(S)memory.c*

rebuild: clean build

build: kernel always
	$(PYTHON) scripts$(S)prompt.py >include$(S)data$(S)prompt.h
	make -C build
	$(CCOPY) build$(S)sources$(S)$(BINARY).elf .
	$(CCOPY) build$(S)sources$(S)$(BINARY).uf2 .
	$(CCOPY) $(BINARY).uf2 $(BINDIR)
	$(CCOPY) $(BINARY).elf $(BINDIR)

clean: always
	rm -Rf build
	export PICO_SDK_PATH=$(PICO_SDK_LOCATION) ; cmake -B build -S .

run: build
	$(UPLOADER) $(UPCONFIG) $(UPCOMMANDS)

reset:
	$(UPLOADER) $(UPCONFIG) -c "adapter speed 5000" -c "init" -c "reset init" -c "reset run" -c "exit"