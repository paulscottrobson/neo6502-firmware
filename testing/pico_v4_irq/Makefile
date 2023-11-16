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
	64tass -b -c asm$(S)test.asm -o asm$(S)test.bin -L asm$(S)test.lst
	$(PYTHON) $(BINDIR)hconvert.py asm$(S)test.bin >src$(S)binary.h
	touch src$(S)roms.cpp

build: prelim
	pio run

run: prelim
	pio run -t upload