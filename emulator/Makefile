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
FIRMDIR = $(ROOTDIR)firmware$(S)
IMPSRC = $(COMDIR)sources$(S)interface$(S)

SOURCES = 	src$(S)core$(S)sys_processor.o  framework$(S)main.o framework$(S)gfx.o framework$(S)debugger.o \
			src$(S)core$(S)sys_debugger.o src$(S)core$(S)hardware.o \
  			$(IMPSRC)memory.o $(IMPSRC)graphics.o $(IMPSRC)console.o $(IMPSRC)keyboard.o $(IMPSRC)dispatch.o \
  			$(IMPSRC)maths.o  $(IMPSRC)config.o $(IMPSRC)gfxcommands.o $(IMPSRC)efla.o $(IMPSRC)ellipse.o \
  			$(IMPSRC)fileinterface.o $(IMPSRC)sprites.o $(IMPSRC)logo.o $(IMPSRC)sprites_xor.o \
  			$(IMPSRC)sndmanager.o $(IMPSRC)sfxmanager.o $(IMPSRC)tick.o $(IMPSRC)tilemap.o \
  			$(IMPSRC)serialmanager.o $(IMPSRC)turtle.o $(IMPSRC)locale.o

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

kernel:
	make -C ..$(S)kernel

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
	
# ***************************************************************************************
#
#								 Build for Windows.
#
# ***************************************************************************************

windows: clearobj $(APPNAME)	
	$(CCOPY) $(APPNAME)	 ..$(S)bin
	make -B clearobj

# ***************************************************************************************
#
#							Build application / compile files
#
# ***************************************************************************************

$(APPNAME): $(SOURCES)
	$(CC) $(SOURCES) $(LDFLAGS) $(SDL_LDFLAGS) -o $@

%.o:%.cpp
	$(CC) $(CADDRESSES) $(CXXFLAGS) -fpermissive -D IBMPC -D INCLUDE_DEBUGGING_SUPPORT -I cpu -I framework -I include -I $(COMDIR)include -c -o $@ $<

always:

# ***************************************************************************************
#
#						Clean & Import interface from firmware
#
# ***************************************************************************************

clean:
	$(CDEL) $(APPNAME) 
	$(CDEL) src$(S)*.o 
	$(CDEL) src$(S)core$(S)*.o 
	$(CDEL) framework$(S)*.o
	$(CDEL) 6502$(S)*.*
	$(CDEL) $(IMPSRC)*.o


# ***************************************************************************************
#
#						Prebuild stuff, generating files & tables		
#
# ***************************************************************************************

prebuild:
	$(CMAKEDIR) 6502
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
		make -C $(ROOTDIR)firmware prelim
		make -B clean
		make -B emulator
		$(CDEL) neolinux.zip
		zip neolinux.zip neo
		make -B -C cross-compile

# ***************************************************************************************
#
#						Make windows/linux emulator only
#
# ***************************************************************************************

elinux:
		make -C $(ROOTDIR)firmware prelim
		make -B clean
		make -B emulator

ewindows: 
		make -C $(ROOTDIR)firmware prelim
		make -B clean
		make -B prebuild
		make -B -C cross-compile
