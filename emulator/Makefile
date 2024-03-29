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
COMMONSOURCES = $(wildcard $(IMPSRC)*.cpp)
COMMONOBJECTS = $(subst .cpp,.o,$(COMMONSOURCES))

SOURCES = 	src$(S)core$(S)sys_processor.o src$(S)core$(S)hardware.o src$(S)framework$(S)beeper.o \
			 src$(S)framework$(S)main.o src$(S)framework$(S)gfx.o src$(S)framework$(S)debugger.o \
			src$(S)core$(S)sys_debugger.o  \
			$(COMMONOBJECTS)

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
	$(MAKE) -C ..$(S)kernel

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
	$(CDEL) src$(S)framework$(S)*.o 
	
# ***************************************************************************************
#
#								 Build for Windows.
#
# ***************************************************************************************

windows: clearobj $(APPNAME)	
	$(CCOPY) $(APPNAME)	 ..$(S)bin
	$(MAKE) -B clearobj

# ***************************************************************************************
#
#							Build application / compile files
#
# ***************************************************************************************

$(APPNAME): $(SOURCES)
	$(CC) $(SOURCES) $(LDFLAGS) $(SDL_LDFLAGS) -o $@

%.o:%.cpp
	$(CC) $(CADDRESSES) $(CXXFLAGS) -fpermissive -D IBMPC -D INCLUDE_DEBUGGING_SUPPORT -I cpu -I src$(S)framework -I include -I $(COMDIR)include -c -o $@ $<

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
	$(CDEL) src$(S)framework$(S)*.o
	$(CDEL) 6502$(S)*.*
	$(CDEL) $(IMPSRC)*.o


# ***************************************************************************************
#
#						Prebuild stuff, generating files & tables		
#
# ***************************************************************************************

prebuild:
	$(CMAKEDIR) 6502
	$(MAKE) -C processor
	$(PYTHON) scripts$(S)mapper.py >include$(S)hid2sdl.h
	$(CCOPY) $(BINDIR)*_binary.h include

# ***************************************************************************************
#
#								Force firmware rebuild		
#
# ***************************************************************************************

fwbuild:
	$(MAKE) -B -C $(FIRMWARE)

# ***************************************************************************************
#
#									Release rebuild
#
# ***************************************************************************************

release:
		$(MAKE) -C $(ROOTDIR)firmware prelim
		$(MAKE) -B clean
		$(MAKE) -B emulator
		$(CDEL) neolinux.zip
		zip neolinux.zip neo
		$(MAKE) -B -C cross-compile

# ***************************************************************************************
#
#						Make emulator
#
# ***************************************************************************************

elinux:
		$(MAKE) -C $(ROOTDIR)firmware prelim
		$(MAKE) -B clean
		$(MAKE) -B emulator

emacos:
		$(MAKE) -C $(ROOTDIR)firmware prelim
		$(MAKE) -B clean
		$(MAKE) -B prebuild
		$(MAKE) -B emulator

ewindows: 
		$(MAKE) -C $(ROOTDIR)firmware prelim
		$(MAKE) -B clean
		$(MAKE) -B prebuild
		$(MAKE) -B -C cross-compile
