# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		Makefile
#		Purpose :	Build Emulator
#		Date :		13th Jan 2024
#		Author : 	Paul Robson (paul@robsons.crg.uk)
#
# *******************************************************************************************
# *******************************************************************************************

ifeq ($(OS),Windows_NT)
include ..\..\build_env\common.make
else
include ../../build_env/common.make
endif

APPNAME = neo$(APPSTEM)
FIRMDIR = $(ROOTDIR)firmware$(S)
IMPSRC = $(COMDIR)sources$(S)interface$(S)
EMUSRC = $(ROOTDIR)emulator$(S)src$(S)
FRASRC = $(ROOTDIR)emulator$(S)src$(S)framework$(S)

COMMONSOURCES = $(wildcard $(IMPSRC)*.cpp)

SOURCES = 	$(FRASRC)main.cpp $(FRASRC)gfx.cpp $(FRASRC)debugger.cpp $(FRASRC)beeper.cpp \
			$(EMUSRC)core$(S)sys_processor.cpp  $(EMUSRC)core$(S)sys_debugger.cpp $(EMUSRC)core$(S)hardware.cpp \
			$(COMMONSOURCES)

FLAGS = -DINCLUDE_OS_SUPPORT -DIBMPC -I.. -I ../framework -I ../cpu -I ../bin -I ../include -I $(COMDIR)/include -I 6502 -O1 \
		-s USE_SDL=2 -s WASM=1 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file storage \
		 -D INCLUDE_DEBUGGING_SUPPORT -s LINKABLE=1 -s EXPORT_ALL=1 -fpermissive

all: app

app:
	$(CMAKEDIR) storage
	$(MAKE) -B -C $(ROOTDIR)basic release
	$(CCOPY) -r $(ROOTDIR)basic$(S)storage$(S)* storage
	emcc  -D EMSCRIPTEN -D LINUX -D INCLUDE_DEBUGGING_SUPPORT $(FLAGS) $(SOURCES) -o neo.html

upload:app
	lftp -c "open ftp://neo6502@studio2.org.uk:justfuckingdoit99@ftp.studio2.org.uk; ; set ssl:check-hostname off ; put neo.html ; put neo.data ; put neo.js ; put neo.wasm ; ls"

server: 
	python -m http.server