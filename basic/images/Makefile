
ifeq ($(OS),Windows_NT)
include ..\..\build_env\common.make
else
include ../../build_env/common.make
endif

all: always creator builder 
	$(PYTHON) $(BINDIR)makeimg.zip
	$(CCOPY) graphics.gfx ..$(S)storage
	
creator: always
	$(CMAKEDIR) build
	$(CCOPY) ..$(S)scripts$(S)gconvert.py build
	$(CCOPY) create_blanks.py build$(S)__main__.py
	$(CDEL) $(BINDIR)createblanks.zip
	zip -j $(BINDIR)createblanks.zip build$(S)__main__.py build$(S)gconvert.py
	$(CDEL) build$(S)*.py

builder: always
	$(CMAKEDIR) build
	$(CCOPY) ..$(S)scripts$(S)gconvert.py build
	$(CCOPY) makeimg.py build$(S)__main__.py
	$(CDEL) $(BINDIR)makeimg.zip
	zip -j $(BINDIR)makeimg.zip build$(S)__main__.py build$(S)gconvert.py
	$(CDEL) build$(S)*.py

checklib:
	$(PYTHON) ..$(S)scripts$(S)gfxtext.py
	cmp _test.gfx graphics.gfx
	$(CDEL) _test.gfx
	
always:

