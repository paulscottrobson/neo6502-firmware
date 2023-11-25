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

makebasic:
	$(CCOPY) scripts$(S)*.py build
	$(CCOPY) build$(S)makebasic.py build$(S)__main__.py
	$(CDEL) $(BINDIR)makebasic.zip
	zip -j $(BINDIR)makebasic.zip build$(S)__main__.py build$(S)tokens.py build$(S)tokeniser.py
	$(CDEL) build$(S)*.py

prelim:
	$(CDEL) sources$(S)generated$(S)*.*
	$(CCOPY) $(BINDIR)neo6502.inc sources$(S)generated
	$(PYTHON) scripts$(S)token_code.py