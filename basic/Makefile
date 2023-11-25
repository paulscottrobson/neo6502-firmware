# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		Makefile
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		25th November 2023
#		Reviewed :	No
#		Purpose :	Basic Main Makefile
#
# ***************************************************************************************
# ***************************************************************************************

ifeq ($(OS),Windows_NT)
include ..\documents\common.make
else
include ../documents/common.make
endif

BASPROG = test.bas 

all: build

makebasic:
	$(CCOPY) scripts$(S)*.py build
	$(CCOPY) build$(S)makebasic.py build$(S)__main__.py
	$(CDEL) $(BINDIR)makebasic.zip
	zip -j $(BINDIR)makebasic.zip build$(S)__main__.py build$(S)tokens.py build$(S)tokeniser.py
	$(CDEL) build$(S)*.py

prelim: always
	$(CDEL) sources$(S)generated$(S)*.*
	$(CCOPY) $(BINDIR)neo6502.inc sources$(S)generated
	$(PYTHON) scripts$(S)token_code.py
	$(PYTHON) scripts$(S)makebuild.py >_include.inc
	$(PYTHON) $(BINDIR)makebasic.zip build$(S)test.bas -obuild$(S)tokenised.dat

wedge:
	$(ASSEMBLER) build$(S)wedge.asm -o build$(S)wedge.bin

build: prelim
	$(ASSEMBLER) basic.asm -L build/basic.lst -o build/basic.bin
	$(PYTHON) $(BINDIR)hconvert.py build$(S)basic.bin build$(S)basic_binary.h basic C000
	$(CCOPY) build$(S)basic_binary.h $(BINDIR)

run:build	
	$(EMULATOR) $(BINDIR)kernel.bin@FC00 build$(S)basic.bin@C000 build$(S)wedge.bin@FFFA

always:
