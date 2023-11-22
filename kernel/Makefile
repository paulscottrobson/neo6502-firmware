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

all: always
	$(CDEL) build$(S)binary.h
	$(ASSEMBLER) $(KERNELSOURCES) -o build$(S)kernel.bin -L build$(S)kernel.lst $(ASMEND)
	$(PYTHON) scripts$(S)hconvert.py build$(S)kernel.bin >build$(S)binary.h
	$(CCOPY) build$(S)kernel.bin $(BINDIR)
	
always:

