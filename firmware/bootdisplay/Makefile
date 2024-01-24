
ifeq ($(OS),Windows_NT)
include ..\..\build_env\common.make
else
include ../../build_env/common.make
endif

all: 
	$(PYTHON) convert.py
	$(CCOPY) *.h $(COMDIR)include$(S)data