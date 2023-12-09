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
include ..\build_env\common.make
else
include ../build_env/common.make
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
	$(PYTHON) scripts$(S)scanner.py 20 BinaryVectorTable > sources$(S)generated$(S)vectors_binary.asm
	$(PYTHON) scripts$(S)scanner.py 80 StandardVectorTable > sources$(S)generated$(S)vectors_standard.asm
	$(PYTHON) scripts$(S)scanner.py 180 ExtendedVectorTable > sources$(S)generated$(S)vectors_extended.asm
	$(PYTHON) scripts$(S)scanner.py 280 AssemblerVectorTable > sources$(S)generated$(S)vectors_assembler.asm
	$(PYTHON) scripts$(S)token_code.py
	$(PYTHON) scripts$(S)errors.py
	$(PYTHON) scripts$(S)makebuild.py >_include.inc
	$(PYTHON) $(BINDIR)makebasic.zip build$(S)test.bas -obuild$(S)tokenised.dat

wedge:
	$(ASSEMBLER) wedge.asm -o build$(S)wedge.bin

build: prelim
	$(ASSEMBLER) basic.asm -L build/basic.lst -o build/basic.bin
	$(PYTHON) $(BINDIR)hconvert.py build$(S)basic.bin build$(S)basic_binary.h basic 1000
	$(CCOPY) build$(S)basic_binary.h $(BINDIR)

run:build	
	$(EMULATOR) $(BINDIR)kernel.bin@FC00 build$(S)basic.bin@1000 build$(S)wedge.bin@FFFA

import:
	make -C ..$(S)emulator import
	make -C ..$(S)emulator

firmware:
	make -C ..$(S)firmware
	
fi : firmware import
	
always:

test:
	make binary 
	make brackets
	make unary
	make string
	make assign

binary:
	$(PYTHON) tests$(S)binary.py >build$(S)test.bas
	make run
brackets:	
	$(PYTHON) tests$(S)brackets.py >build$(S)test.bas
	make run
unary:
	$(PYTHON) tests$(S)unary.py >build$(S)test.bas
	make run
string:
	$(PYTHON) tests$(S)string.py >build$(S)test.bas
	make run
assign:
	$(PYTHON) tests$(S)assign.py >build$(S)test.bas
	make run
