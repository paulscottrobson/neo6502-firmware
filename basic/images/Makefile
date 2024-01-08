
ifeq ($(OS),Windows_NT)
include ..\..\build_env\common.make
else
include ../../build_env/common.make
endif

creator:
	$(CCOPY) ..$(S)scripts$(S)gconvert.py build
	$(CCOPY) create_blanks.py build$(S)__main__.py
	$(CDEL) $(BINDIR)createblanks.zip
	zip -j $(BINDIR)createblanks.zip build$(S)__main__.py build$(S)gconvert.py
	$(CDEL) build$(S)*.py

all: creator
	$(PYTHON) $(BINDIR)createblanks.zip
