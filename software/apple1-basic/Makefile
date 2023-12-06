# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		Makefile
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		20th November 2023
#		Reviewed :	No
#		Purpose :	Makefile for Apple 1 BASIC
#
# ***************************************************************************************
# ***************************************************************************************

ifeq ($(OS),Windows_NT)
include ..\..\build_env\common.make
else
include ../../build_env/common.make
endif

all: build 

check:always
	$(ASSEMBLER) -D NEO=0 a1basic.asm -o build$(S)a1basic.original.bin
	cmp build$(S)a1basic.original.bin a1basic.bin.rom

build:always
	$(CCOPY) $(BINDIR)neo6502.inc build
	$(ASSEMBLER) -D NEO=1 build$(S)neo6502.inc a1basic.asm -o build$(S)a1basic.bin -L build$(S)a1basic.lst
	$(PYTHON) $(BINDIR)hconvert.py build$(S)a1basic.bin build$(S)a1basic_binary.h a1basic E000
	$(CCOPY) build$(S)a1basic.bin $(BINDIR)
	$(CCOPY) build$(S)a1basic_binary.h $(BINDIR)
	
always:

run: build
	$(EMULATOR) $(BINDIR)kernel.bin@FC00 $(BINDIR)a1basic.bin@E000
