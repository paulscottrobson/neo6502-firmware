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
#is this really needed? Windows user may use WSL2
include ..\build_env\common.make
else
include ../build_env/common.make
endif

BINARY = firmware
KERNELSOURCES = kernel.asm

all: always kernel

# ***************************************************************************************
#
#									Release kernel
#
# ***************************************************************************************

kernel:
	$(CDEL) $(BINDIR)binary.h
	$(CDEL) build$(S)binary.h
	$(CDEL) $(BINDIR)kernel.bin
	$(PYTHON) scripts$(S)vectors.py
	$(ASSEMBLER) $(KERNELSOURCES) -o build$(S)kernel.bin -L build$(S)kernel.lst $(ASMEND)
	$(PYTHON) $(ROOTDIR)basic$(S)scripts$(S)hconvert.py build$(S)kernel.bin build$(S)kernel_binary.h kernel
	$(CCOPY) build$(S)kernel.bin $(BINDIR)
	$(CCOPY) build$(S)kernel_binary.h $(BINDIR)
	$(CCOPY) neo6502.inc $(BINDIR)
	$(CCOPY) neo6502.inc $(ROOTDIR)documents$(S)release

always:

# ***************************************************************************************
#
#									Release rebuild
#
# ***************************************************************************************

release:
		make -B kernel

