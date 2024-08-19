# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		Makefile
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		12th February 2024
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


all: basicdemos
	$(CDEL) samples.zip
	zip -r samples.zip basic-examples c assembly

release:
	make -B all

# ***************************************************************************************
#
#										BASIC demos
#
# ***************************************************************************************

DEMOSRC = $(shell find basic -name '*.bsc')
DEMOBAS = $(subst bsc,bas,$(DEMOSRC))
DEMOGFX = $(shell find basic -name '*.gfx')
DEMOLIB = $(shell find basic -name '*.lib')

BASDIR = basic-examples

basicdemos: clearbuild $(DEMOBAS)
	$(CCOPY) -R $(DEMOGFX) $(DEMOLIB) $(BASDIR)
	$(CCOPY) $(ROOTDIR)basic$(S)images$(S)graphics.gfx $(BASDIR)
	$(CCOPY) $(ROOTDIR)basic$(S)images$(S)test$(S)test.gfx $(BASDIR)

clearbuild:
	$(CMAKEDIR) build
	$(CMAKEDIR) $(BASDIR)
	$(CDEL) $(BASDIR)$(S)*.*

basic/games/asteroids.bas : basic/games/asteroids.bsc basic/games/asteroid_asm.lib
	$(PYTHON) $(BINDIR)makebasic.zip $< -obasic/games/asteroids.bas
	$(CCOPY) basic/games/asteroid*.bsc $(BASDIR)

%.bas : %.bsc
	$(PYTHON) $(BINDIR)makebasic.zip $< -o$@
	$(CCOPY) $@ $< $(BASDIR)


# ***************************************************************************************
#
#				Does a common check of BASIC inline assembler vs 64 tass
#
# ***************************************************************************************

checkasm:
	$(ASSEMBLER) -Wno-error=implied-reg code$(S)checkasm.tass -L build/checkasm.lst -o build/asmtest.bin
	cmp -b $(BASDIR)/asmtest.inline build/asmtest.bin

# ***************************************************************************************
#
#								Run the machine clean.
#
# ***************************************************************************************

basemachine:
	$(EMULATOR)

# ***************************************************************************************
#
#								Start VLC showing HDMI in (me only)
#
# ***************************************************************************************

vlc:
	vlc v4l2:///dev/video0:fps=30:chroma=mjpg:width=640:height=480 &

# ***************************************************************************************
#
#											Clean
#
# ***************************************************************************************

clean:
	$(CDEL) build$(S)*.*
	$(CDEL) sources$(S)generate$(S)*.*
	$(CDEL) _include.inc
	$(CDEL) test.bas
	$(CDEL) memory.dump
