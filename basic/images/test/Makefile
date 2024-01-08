
ifeq ($(OS),Windows_NT)
include ..\..\..\build_env\common.make
else
include ../../../build_env/common.make
endif

all: 
	$(PYTHON) testgfx.py
	$(CCOPY) test.gfx $(ROOTDIR)basic$(S)storage