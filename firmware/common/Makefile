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
#is this really needed?  WSL2 is the 2023.
include ..\..\build_env\common.make
else
include ../../build_env/common.make
endif

all: 
	$(MAKE) -B -C bootdisplay

	$(PYTHON) $(COMDIR)scripts$(S)sweet16.py $(COMDIR)scripts$(S)sweet16.gen >$(COMDIR)include$(S)data$(S)sweet_opcodes.h

	$(PYTHON) $(COMDIR)scripts$(S)makedispatch.py dispatch.config >$(COMDIR)include$(S)data$(S)dispatch_code.h
	touch sources$(S)interface$(S)dispatch.cpp
	$(CCOPY) $(BINDIR)basic_binary.h include$(S)data

	$(PYTHON) $(COMDIR)scripts$(S)who.py >$(COMDIR)include$(S)data$(S)neowho.h

	$(PYTHON) $(COMDIR)scripts$(S)prompt.py $(COMDIR)scripts$(S)build.count >$(COMDIR)include$(S)data$(S)prompt.h

	$(PYTHON) $(COMDIR)scripts$(S)sfx.py >$(COMDIR)include$(S)data$(S)sfxdata.h

	$(PYTHON) $(COMDIR)scripts$(S)triangle.py >$(COMDIR)include$(S)data$(S)triangle.h

	$(PYTHON) $(COMDIR)locale$(S)keymaps.py >$(COMDIR)include$(S)data$(S)neolocale.h

	$(PYTHON) $(COMDIR)scripts$(S)cursors.py >$(COMDIR)include$(S)data$(S)cursors.h
