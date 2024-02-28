# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		Makefile
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		17th February 2024
#		Reviewed :	No
#		Purpose :	Main firmware makefile, most of the work is done by CMake.
#
# ***************************************************************************************
# ***************************************************************************************

ifeq ($(OS),Windows_NT)
include ..\build_env\common.make
else
include ../build_env/common.make
endif

RELEASEFILE = neo6502.zip
TMPDIR = /tmp/builder$(S)
EXAMPLEDIR = $(ROOTDIR)examples$(S)
DOCDIR = $(ROOTDIR)documents$(S)release$(S)

#
#		Documentation files included
#
DOCUMENTS =  $(DOCDIR)*.pdf $(BINDIR)neo6502.inc
#
#		Binary files included
#
BINARIES = 	 $(BINDIR)*.uf2 $(BINDIR)*.elf \
			 $(ROOTDIR)emulator$(S)cross-compile$(S)neo.exe $(ROOTDIR)emulator$(S)cross-compile$(S)neo.exe \
			 $(BINDIR)basic.bin \
			 $(ROOTDIR)emulator$(S)neo
#
#		Python applications included.
#
PYTHONAPPS = $(BINDIR)makebasic.zip $(BINDIR)listbasic.zip $(BINDIR)exec.zip  \
			 $(BINDIR)createblanks.zip $(BINDIR)makeimg.zip $(BINDIR)cvimg.zip \
			 $(BINDIR)nxmit.zip

# ***************************************************************************************
#
#								Make the release zip
#
# ***************************************************************************************

all: 
	# Delete release files
	$(CDEL) $(RELEASEFILE)
	$(CDEL) $(TMPDIR)$(RELEASEFILE)

	# Create dirs for emu binaries and python apps
	$(CMAKEDIR) $(TMPDIR)
	$(CCOPY) $(BINARIES) $(TMPDIR)
	$(CCOPY) $(PYTHONAPPS) $(TMPDIR)

	# Create dirs for documents in own dir
	$(CMAKEDIR) $(TMPDIR)$(S)documents
	$(CCOPY) $(DOCUMENTS) $(TMPDIR)$(S)documents

	# Create dir for crossdev & copy it
	$(CMAKEDIR) $(TMPDIR)$(S)crossdev
	$(CCOPY) -r $(DOCDIR)crossdev$(S)* $(TMPDIR)crossdev

	# Create dir for examples and copy , note basic-examples => basic
	$(CMAKEDIR) $(TMPDIR)$(S)examples
	$(CCOPY) -r $(EXAMPLEDIR)assembly $(TMPDIR)examples
	$(CCOPY) -r $(EXAMPLEDIR)C $(TMPDIR)examples
	$(CMAKEDIR) $(TMPDIR)$(S)examples$(S)basic
	$(CCOPY) -r $(EXAMPLEDIR)basic-examples$(S)* $(TMPDIR)examples$(S)basic

	# Zip up and copy back
	cd $(TMPDIR) ; zip -r $(RELEASEFILE) *
	$(CCOPY) $(TMPDIR)$(RELEASEFILE) .
