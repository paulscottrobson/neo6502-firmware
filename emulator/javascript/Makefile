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
IMPSRC = $(FIRMDIR)sources$(S)interface$(S)
EMUSRC = $(ROOTDIR)emulator$(S)src$(S)
FRASRC = $(ROOTDIR)emulator$(S)framework$(S)

SOURCES = 	$(FRASRC)main.cpp $(FRASRC)gfx.cpp $(FRASRC)debugger.cpp \
			$(EMUSRC)core$(S)sys_processor.cpp  $(EMUSRC)core$(S)sys_debugger.cpp $(EMUSRC)core$(S)hardware.cpp \
  			$(IMPSRC)memory.cpp $(IMPSRC)graphics.cpp $(IMPSRC)console.cpp $(IMPSRC)keyboard.cpp $(IMPSRC)dispatch.cpp \
  			$(IMPSRC)maths.cpp  $(IMPSRC)config.cpp $(IMPSRC)gfxcommands.cpp $(IMPSRC)efla.cpp $(IMPSRC)ellipse.cpp \
  			$(IMPSRC)fileinterface.cpp $(IMPSRC)sprites.cpp $(IMPSRC)logo.cpp $(IMPSRC)sprites_xor.cpp \
  			$(IMPSRC)sndmanager.cpp $(IMPSRC)sfxmanager.cpp $(IMPSRC)tick.cpp

FLAGS = -DINCLUDE_OS_SUPPORT -DIBMPC -I.. -I ../framework -I ../cpu -I ../bin -I ../include -I $(FIRMDIR)/include -I 6502 -O1 \
		-s USE_SDL=2 -s WASM=1 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file storage \
		 -D INCLUDE_DEBUGGING_SUPPORT -s LINKABLE=1 -s EXPORT_ALL=1 

all: app

app:
	$(CMAKEDIR) storage
	make -B -C $(ROOTDIR)basic demos
	$(CCOPY) $(ROOTDIR)basic$(S)storage$(S)* storage
	$(CCOPY) $(ROOTDIR)basic$(S)code$(S)*.bas storage
	emcc  -D EMSCRIPTEN -D LINUX -D INCLUDE_DEBUGGING_SUPPORT $(FLAGS) $(SOURCES) -o neo.html

server: 
	python -m http.server