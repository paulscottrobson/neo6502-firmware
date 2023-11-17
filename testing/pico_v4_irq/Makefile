# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		Makefile
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		17th October 2023
#		Reviewed :	No
#		Purpose :	Makefile for this directory contents.
#
# ***************************************************************************************
# ***************************************************************************************

ifeq ($(OS),Windows_NT)
include ..\..\documents\common.make
else
include ../../documents/common.make
endif

all: build

prelim: 
	$(CDEL) src$(S)binary.h
	$(ASSEMBLER) asm$(S)test.asm -o asm$(S)test.bin -L asm$(S)test.lst
	$(PYTHON) $(BINDIR)hconvert.py asm$(S)test.bin >src$(S)binary.h
	touch src$(S)roms.cpp

pio:
	tools/pioasm src/memory_sm1_address.pio src/memory_sm1_address.pio.h
	touch src$(S)mos65c02.cpp

build: prelim pio
	pio run

run: prelim pio
	pio run -t upload