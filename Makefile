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
BINARIES = 	 $(BINDIR)*.uf2 $(BINDIR)*.elf $(ROOTDIR)emulator$(S)cross-compile$(S)neowin.zip $(BINDIR)basic.bin 
PYTHONAPPS = $(BINDIR)makebasic.zip $(BINDIR)listbasic.zip $(BINDIR)createblanks.zip $(BINDIR)makeimg.zip

all: 
	$(CMAKEDIR) bin
	$(CMAKEDIR) release
	make -B -C kernel release
	make -B -C basic release
	make -B -C firmware release
	make -B -C emulator release
	make -B zipfile 

zipfile: samples crossdev
	zip -r -j -q release$(S)$(RELEASEFILE) $(DOCUMENTS) $(BINARIES) $(PYTHONAPPS) release$(S)samples.zip release$(S)crossdev.zip
	$(CDEL) release$(S)samples.zip
	$(CDEL) release$(S)crossdev.zip

crossdev:
	zip -r -j -q release$(S)crossdev.zip documents$(S)release$(S)crossdev

samples:
	zip -r -j -q release$(S)samples.zip basic$(S)code basic$(S)images$(S)test$(S)test.gfx basic$(S)images$(S)graphics.gfx
	zip -d -q release$(S)samples.zip *.tass *.bsc

clean:
	make -B -C kernel clean
	make -B -C basic clean
	make -B -C emulator clean
	make -B -C firmware clean
