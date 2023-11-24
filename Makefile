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
include documents\common.make
else
include documents/common.make
endif

BINARY = firmware
RELEASEFILE = neo6502.zip

DOCDIR = documents$(S)release$(S)

DOCUMENTS = $(DOCDIR)*.pdf $(DOCDIR)*.txt $(BINDIR)neo6502.inc
BINARIES = $(BINDIR)firmware.uf2 $(BINDIR)firmware.elf

all: 
	zip -r -j release$(S)$(RELEASEFILE) $(DOCUMENTS) $(BINARIES)

always:

