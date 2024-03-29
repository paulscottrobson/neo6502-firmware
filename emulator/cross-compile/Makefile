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
  			
INCLUDES= -I ../include -I ../framework -I .. -I $(COMDIR)include

all: 
	x86_64-w64-mingw32-g++ -std=c++17 -fpermissive -Wall -D IBMPC -D INCLUDE_DEBUGGING_SUPPORT -o neo.exe $(SOURCES) $(INCLUDES) `x86_64-w64-mingw32/bin/sdl2-config --cflags --static-libs` -static-libgcc -static-libstdc++ 
	$(CDEL) neowin.zip
	zip neowin.zip neo.exe

run: 
	wine neo.exe