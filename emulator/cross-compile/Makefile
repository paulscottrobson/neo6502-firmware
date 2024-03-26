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

SOURCES = 	$(FRASRC)main.cpp $(FRASRC)gfx.cpp $(FRASRC)debugger.cpp $(FRASRC)beeper.cpp \
			$(EMUSRC)core$(S)sys_processor.cpp  $(EMUSRC)core$(S)sys_debugger.cpp $(EMUSRC)core$(S)hardware.cpp \
  			$(IMPSRC)memory.cpp $(IMPSRC)graphics.cpp $(IMPSRC)console.cpp $(IMPSRC)keyboard.cpp $(IMPSRC)dispatch.cpp \
  			$(IMPSRC)maths.cpp $(IMPSRC)gfxcommands.cpp $(IMPSRC)efla.cpp $(IMPSRC)ellipse.cpp $(IMPSRC)mos.cpp \
  			$(IMPSRC)fileinterface.cpp $(IMPSRC)sprites.cpp $(IMPSRC)logo.cpp $(IMPSRC)sprites_xor.cpp $(IMPSRC)cursor.cpp \
  			$(IMPSRC)sndmanager.cpp $(IMPSRC)sfxmanager.cpp $(IMPSRC)tick.cpp $(IMPSRC)tilemap.cpp $(IMPSRC)blitter.cpp \
  			$(IMPSRC)serialmanager.cpp $(IMPSRC)turtle.cpp $(IMPSRC)locale.cpp $(IMPSRC)pininterface.cpp
  			
INCLUDES= -I ../include -I ../framework -I .. -I $(COMDIR)include

all: 
	x86_64-w64-mingw32-g++ -std=c++17 -fpermissive -Wall -D IBMPC -D INCLUDE_DEBUGGING_SUPPORT -o neo.exe $(SOURCES) $(INCLUDES) `x86_64-w64-mingw32/bin/sdl2-config --cflags --static-libs` -static-libgcc -static-libstdc++ 
	$(CDEL) neowin.zip
	zip neowin.zip neo.exe

run: 
	wine neo.exe