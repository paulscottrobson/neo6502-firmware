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

DOCDIR      = documents$(S)release$(S)
RELEASEFILE = release$(S)neo6502.zip

DOCUMENTS  = $(DOCDIR)*.pdf $(DOCDIR)*.txt $(BINDIR)neo6502.inc
BINARIES   = bin$(S)*.uf2 bin$(S)*.elf bin$(S)basic.bin bin$(S)neo bin$(S)neo.exe
PYTHONAPPS = $(BINDIR)makebasic.zip $(BINDIR)listbasic.zip $(BINDIR)createblanks.zip \
             $(BINDIR)makeimg.zip $(BINDIR)nxmit.zip

# ***************************************************************************************
#
# Remake everything to release state
#
# ***************************************************************************************

all:
	$(CMAKEDIR) bin
	$(CMAKEDIR) release
	make -B -C kernel release
	make -B -C basic release
	make -B -C firmware release
	make -B -C emulator release
	make -B zipfile

# ***************************************************************************************
#
# Make the release zip
#
# ***************************************************************************************

zipfile:
	zip                                     --quiet $(RELEASEFILE) $(BINARIES)
	zip --grow --recurse-paths              --quiet $(RELEASEFILE) examples
	zip --grow --recurse-paths --junk-paths --quiet $(RELEASEFILE) $(DOCUMENTS)
	zip --grow --recurse-paths --junk-paths --quiet $(RELEASEFILE) $(PYTHONAPPS)

# ***************************************************************************************
#
# Make windows & linux versions
#
# ***************************************************************************************

windows:
		make -B -C kernel clean
		make -B -C basic clean
		make -B -C emulator clean
		make -B -C emulator ewindows

linux:
		make -B -C kernel clean
		make -B -C basic clean
		make -B -C emulator clean
		make -B -C emulator elinux


# ***************************************************************************************
#
# Clean everything
#
# ***************************************************************************************

clean:
	make -B -C kernel clean
	make -B -C basic clean
	make -B -C emulator clean
	make -B -C firmware clean

