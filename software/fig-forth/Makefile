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
include ..\..\documents\common.make
else
include ../../documents/common.make
endif

all: build 

build:always
	$(ASSEMBLER) fig-forth.asm -o build$(S)fig-forth.bin -L build$(S)fig-forth.lst $(ASMEND)
	$(CCOPY) $(BINDIR)neo6502.inc .
	$(PYTHON) $(BINDIR)hconvert.py build$(S)fig-forth.bin build$(S)figforth_binary.h figforth 400
	$(CCOPY) build$(S)fig-forth.bin $(BINDIR)figforth.bin
	$(CCOPY) build$(S)figforth_binary.h $(BINDIR)

always:

run: build
	$(EMULATOR) $(BINDIR)kernel.bin@FC00 $(BINDIR)figforth.bin@0400

