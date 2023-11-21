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

all: build

always:

kernel: always
	make -C ..$(S)kernel
	$(CCOPY) ..$(S)kernel$(S)build$(S)binary.h include$(S)data
	$(TOUCH) sources$(S)memory.c*

rebuild: clean build

build: kernel always
	make -C build

clean: always
	rm -Rf build
	export PICO_SDK_PATH=$(PICO_SDK_LOCATION) ; cmake -B build -S .

run: build
	$(CCOPY) build$(S)sources$(S)$(BINARY).elf .
	$(UPLOADER) $(UPCONFIG) $(UPCOMMANDS)
