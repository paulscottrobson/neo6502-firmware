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
IMPSRC = import$(S)sources$(S)interface$(S)

SOURCES = 	src$(S)core$(S)sys_processor.cpp  framework$(S)main.cpp framework$(S)gfx.cpp framework$(S)debugger.cpp \
			src$(S)core$(S)sys_debugger.cpp src$(S)core$(S)hardware.cpp \
  			$(IMPSRC)memory.c $(IMPSRC)graphics.c $(IMPSRC)console.c $(IMPSRC)keyboard.c $(IMPSRC)dispatch.c \
  			$(IMPSRC)maths.c  $(IMPSRC)config.c $(IMPSRC)gfxcommands.c $(IMPSRC)efla.c $(IMPSRC)ellipse.c \
  			$(IMPSRC)fileinterface.c $(IMPSRC)sprites.c $(IMPSRC)logo.c $(IMPSRC)sprites_xor.c \
  			$(IMPSRC)sndmanager.c $(IMPSRC)sfxmanager.c

FLAGS = -DINCLUDE_OS_SUPPORT -DIBMPC -I. -I framework -I./cpu -I./bin -I include -I import/include -I 6502 -O1 \
		-s USE_SDL=2 -s WASM=1 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file storage \
		 -D INCLUDE_DEBUGGING_SUPPORT -s LINKABLE=1 -s EXPORT_ALL=1 

all: app

app:
	rm -Rf build
	mkdir -p build
	cp -R ../src ../include ../framework ../6502 ../storage ../import build -R
	cd build ; emcc  -D EMSCRIPTEN -D LINUX -D INCLUDE_DEBUGGING_SUPPORT $(FLAGS) $(SOURCES) -o neo.html

server: app
	cd build ; python -m http.server