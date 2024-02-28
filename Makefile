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

DOCUMENTS = $(DOCDIR)*.odt $(DOCDIR)*.pdf $(DOCDIR)*.txt $(BINDIR)neo6502.inc
BINARIES = 	 $(BINDIR)*.uf2 $(BINDIR)*.elf $(ROOTDIR)emulator$(S)cross-compile$(S)neowin.zip $(BINDIR)basic.bin \
			 $(ROOTDIR)emulator$(S)neolinux.zip
PYTHONAPPS = $(BINDIR)makebasic.zip $(BINDIR)listbasic.zip $(BINDIR)createblanks.zip $(BINDIR)makeimg.zip \
			 $(BINDIR)nxmit.zip

# ***************************************************************************************
#
#						Remake everything to release state
#
# ***************************************************************************************

all:
	$(CMAKEDIR) bin
	$(CMAKEDIR) release
	$(MAKE) -B -C kernel release
	$(MAKE) -B -C basic release
	$(MAKE) -B -C firmware release
	$(MAKE) -B -C emulator release
	$(MAKE) -B -C examples release
	$(MAKE) -B -C release

documentation:
	# generate ODT documentation (requires 'pandoc')
	pandoc -f latex -t odt -o $(DOCDIR)api.odt documents$(S)api.texi
	# generate PDF documentation (requires 'texlive-latex')
	pdflatex -output-directory=$(DOCDIR) documents$(S)api.texi


# ***************************************************************************************
#
#							Make windows & linux versions
#
# ***************************************************************************************

windows:
		$(CMAKEDIR) bin
		$(MAKE) -B -C kernel
		$(MAKE) -B -C basic release
		$(MAKE) -B -C emulator clean
		$(MAKE) -B -C emulator ewindows

linux:
		$(CMAKEDIR) bin
		$(MAKE) -B -C kernel
		$(MAKE) -B -C basic release
		$(MAKE) -B -C emulator clean
		$(MAKE) -B -C emulator elinux

macos:
		make -B -C emulator emacos


# ***************************************************************************************
#
#								Clean everything
#
# ***************************************************************************************

clean:
	$(MAKE) -B -C kernel clean
	$(MAKE) -B -C basic clean
	$(MAKE) -B -C emulator clean
	$(MAKE) -B -C firmware clean

