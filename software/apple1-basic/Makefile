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

check:always
	$(ASSEMBLER) -D NEO=0 a1basic.asm -o build$(S)a1basic.original.bin
	cmp build$(S)a1basic.original.bin a1basic.bin.rom

build:always
	$(ASSEMBLER) -D NEO=1 a1basic.asm -o build$(S)a1basic.bin -L build$(S)a1basic.lst
	$(CCOPY) build$(S)a1basic.bin $(BINDIR)
	
always:

run: build
	$(EMULATOR) $(BINDIR)kernel.bin@FC00 $(BINDIR)a1basic.bin@E000