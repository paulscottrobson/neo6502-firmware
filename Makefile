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

RELEASEFILE = neo6502.zip

DOCDIR = documents$(S)release$(S)

DOCUMENTS =  $(DOCDIR)*.pdf $(DOCDIR)*.txt $(BINDIR)neo6502.inc
BINARIES = 	 $(BINDIR)*.uf2 $(BINDIR)*.elf $(BINDIR)*.dll $(BINDIR)*.exe $(BINDIR)basic.bin 
PYTHONAPPS = $(BINDIR)makebasic.zip $(BINDIR)listbasic.zip

all: 
	make -B -C kernel release
	make -B -C basic release
	make -B -C firmware release
	make -B -C emulator release
	make -B zipfile 

zipfile: samples
	zip -r -j -q release$(S)$(RELEASEFILE) $(DOCUMENTS) $(BINARIES) $(PYTHONAPPS) release$(S)samples.zip
	$(CDEL) release$(S)samples.zip

samples:
	zip -r -j -q release$(S)samples.zip basic$(S)code documents$(S)images$(S)test.gfx
	zip -d -q release$(S)samples.zip *.tass *.bsc


