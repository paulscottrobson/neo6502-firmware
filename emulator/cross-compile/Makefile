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
FRASRC = $(ROOTDIR)emulator$(S)framework$(S)

SOURCES = 	$(FRASRC)main.cpp $(FRASRC)gfx.cpp $(FRASRC)debugger.cpp \
			$(EMUSRC)core$(S)sys_processor.cpp  $(EMUSRC)core$(S)sys_debugger.cpp $(EMUSRC)core$(S)hardware.cpp \
  			$(IMPSRC)memory.cpp $(IMPSRC)graphics.cpp $(IMPSRC)console.cpp $(IMPSRC)keyboard.cpp $(IMPSRC)dispatch.cpp \
  			$(IMPSRC)maths.cpp  $(IMPSRC)config.cpp $(IMPSRC)gfxcommands.cpp $(IMPSRC)efla.cpp $(IMPSRC)ellipse.cpp \
  			$(IMPSRC)fileinterface.cpp $(IMPSRC)sprites.cpp $(IMPSRC)logo.cpp $(IMPSRC)sprites_xor.cpp \
  			$(IMPSRC)sndmanager.cpp $(IMPSRC)sfxmanager.cpp $(IMPSRC)tick.cpp $(IMPSRC)tilemap.cpp \
  			$(IMPSRC)serialmanager.cpp $(IMPSRC)turtle.cpp $(IMPSRC)locale.cpp
  			
INCLUDES= -I ../include -I ../framework -I .. -I $(COMDIR)include

all: 
	x86_64-w64-mingw32-g++ -fpermissive -Wall -D IBMPC -D INCLUDE_DEBUGGING_SUPPORT -o neo.exe $(SOURCES) $(INCLUDES) `x86_64-w64-mingw32/bin/sdl2-config --cflags --static-libs` -static-libgcc -static-libstdc++ 

run: 
	wine neo.exe
