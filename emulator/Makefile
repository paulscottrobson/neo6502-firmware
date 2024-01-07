# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		Makefile
#		Purpose :	Build Emulator
#		Date :		22nd November 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

ifeq ($(OS),Windows_NT)
include ..\build_env\common.make
else
include ../build_env/common.make
endif

APPNAME = neo$(APPSTEM)
IMPSRC = import$(S)sources$(S)interface$(S)

SOURCES = 	src$(S)core$(S)sys_processor.o  framework$(S)main.o framework$(S)gfx.o framework$(S)debugger.o \
			src$(S)core$(S)sys_debugger.o src$(S)core$(S)hardware.o \
  			$(IMPSRC)memory.o $(IMPSRC)graphics.o $(IMPSRC)console.o $(IMPSRC)keyboard.o $(IMPSRC)dispatch.o \
  			$(IMPSRC)maths.o  $(IMPSRC)config.o $(IMPSRC)gfxcommands.o $(IMPSRC)efla.o $(IMPSRC)ellipse.o \
  			$(IMPSRC)fileinterface.o $(IMPSRC)sprites.o $(IMPSRC)logo.o

FIRMWARE = ..$(S)firmware$(S)

CC = g++

.PHONY: all clean emulator prebuild release run

all: emulator

always:

# ***************************************************************************************
#
#									Run emulator
#
# ***************************************************************************************
	
run: always emulator
	$(EMULATOR)

# ***************************************************************************************
#
#						Run emulator with directly built kernel
#
# ***************************************************************************************
	
krun: always kernel emulator
	$(EMULATOR) $(BINDIR)kernel.bin@FC00 

# ***************************************************************************************
#
#									Make the emulator
#
# ***************************************************************************************

emulator: prebuild $(APPNAME)
	$(CCOPY) $(APPNAME) ..$(S)bin

# ***************************************************************************************
#
#								Clear all object files								
#
# ***************************************************************************************

clearobj:
	$(CDEL) src$(S)core$(S)*.o
	$(CDEL) framework$(S)*.o 
	$(CDEL) import$(S)sources$(S)interface$(S)*.o

# ***************************************************************************************
#
#								 Build for Windows.
#
# ***************************************************************************************

windows: clearobj $(APPNAME)	
	$(CCOPY) $(APPNAME)	 ..$(S)bin
	$(CCOPY) SDL2.dll ..$(S)bin
	make -B clearobj

# ***************************************************************************************
#
#							Build application / compile files
#
# ***************************************************************************************

$(APPNAME): $(SOURCES)
	$(CC) $(SOURCES) $(LDFLAGS) $(SDL_LDFLAGS) -o $@

%.o:%.cpp
	$(CC) $(CADDRESSES) $(CXXFLAGS) -D IBMPC -D INCLUDE_DEBUGGING_SUPPORT -I cpu -I framework -I include -I import$(S)include -c -o $@ $<

%.o:%.c
	$(CC) $(CADDRESSES) $(CXXFLAGS) -D IBMPC -D INCLUDE_DEBUGGING_SUPPORT -I cpu -I framework -I include -I import$(S)include -c -o $@ $<

# ***************************************************************************************
#
#								Import firmware into emulator	
#
# ***************************************************************************************

import: always
	$(CCOPY) $(FIRMWARE)include$(S)common.h import$(S)include
	$(CCOPYREC) $(FIRMWARE)include$(S)interface import$(S)include
	$(CCOPYREC) $(FIRMWARE)include$(S)data import$(S)include
	$(CCOPYREC) $(FIRMWARE)sources$(S)interface import$(S)sources

# ***************************************************************************************
#
#								Rebuild the kernel
#
# ***************************************************************************************

kernel:	
	make -B -C ../kernel
	$(CCOPY) $(BINDIR)*_binary.h import$(S)include$(S)data
	$(TOUCH) import$(S)sources$(S)interface$(S)memory.c

always:

# ***************************************************************************************
#
#						Clean & Import interface from firmware
#
# ***************************************************************************************

clean: import
	$(CDEL) $(APPNAME) 
	$(CDEL) src$(S)*.o 
	$(CDEL) src$(S)core$(S)*.o 
	$(CDEL) framework$(S)*.o


# ***************************************************************************************
#
#						Prebuild stuff, generating files & tables		
#
# ***************************************************************************************

prebuild:
	make -C processor
	$(PYTHON) scripts$(S)mapper.py >include$(S)hid2sdl.h
	$(CCOPY) $(BINDIR)*_binary.h include

# ***************************************************************************************
#
#								Force firmware rebuild		
#
# ***************************************************************************************

fwbuild:
	make -B -C $(FIRMWARE)

# ***************************************************************************************
#
#									Release rebuild
#
# ***************************************************************************************

release:
		make -B clean
		make -B import 
		make -B emulator

