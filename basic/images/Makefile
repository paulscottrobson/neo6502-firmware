
ifeq ($(OS),Windows_NT)
include ..\..\build_env\common.make
else
include ../../build_env/common.make
endif
 
all: always creator builder tofromtext
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

tofromtext:
	$(CMAKEDIR) build
	$(CCOPY) ..$(S)scripts$(S)gfxtext.py build
	$(CCOPY) cvimg.py build$(S)__main__.py 
	$(CDEL) $(BINDIR)cvimg.zip
	zip -j $(BINDIR)cvimg.zip build$(S)__main__.py build$(S)gfxtext.py
	$(CDEL) build$(S)*.py

checklib: tofromtext
	$(PYTHON) $(BINDIR)cvimg.zip graphics.gfx graphics.txt
	$(PYTHON) $(BINDIR)cvimg.zip graphics.txt _test.gfx
	cmp _test.gfx graphics.gfx
	$(CDEL) _test.gfx

checkc:
	gcc -Wall -D NEO6502RUNTEST ..$(S)scripts$(S)gfxtext.c -ogfxtext
	./gfxtext
	#hexdump -C _cdemo.gfx
	cmp -b _cdemo.gfx graphics.gfx
	rm gfxtext
	rm _cdemo.gfx

	
always:

