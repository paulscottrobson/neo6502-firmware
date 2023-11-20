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
include ..\documents\common.make
else
include ../documents/common.make
endif

BINARY = firmware
KERNELSOURCES = kernel.asm
all: build

always:

kernel: always
	$(CDEL) sources$(S)binary.h
	cd kernel && $(ASSEMBLER) $(KERNELSOURCES) -o kernel.bin -L kernel.lst $(ASMEND)
	$(PYTHON) scripts$(S)hconvert.py kernel$(S)kernel.bin >include$(S)binary.h
	$(TOUCH) sources$(S)roms.c*

rebuild: clean build

build: kernel always
	make -C build

clean: always
	rm -Rf build
	cmake -B build -S .

run: build
	$(CCOPY) build$(S)sources$(S)$(BINARY).elf .
	$(UPLOADER) $(UPCONFIG) $(UPCOMMANDS)
