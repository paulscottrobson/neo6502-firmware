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
include build_env\common.make
else
include build_env/common.make
endif

BINARY = firmware
RELEASEFILE = neo6502.zip

DOCDIR = documents$(S)release$(S)

DOCUMENTS = $(DOCDIR)*.pdf $(DOCDIR)*.txt $(BINDIR)neo6502.inc
BINARIES = $(BINDIR)firmware.uf2 $(BINDIR)firmware.elf $(BINDIR)*.dll $(BINDIR)*.exe

all: samples
	zip -r -j release$(S)$(RELEASEFILE) $(DOCUMENTS) $(BINARIES) release$(S)samples.zip
	$(CDEL) release$(S)samples.zip

samples:
	zip -r -j release$(S)samples.zip basic$(S)code
	zip -d release$(S)samples.zip *.bsc *.tass

always:

