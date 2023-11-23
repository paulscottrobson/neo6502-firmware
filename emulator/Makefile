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
include ..\documents\common.make
else
include ../documents/common.make
endif

APPNAME = neo$(APPSTEM)
IMPSRC = import$(S)sources$(S)interface$(S)

SOURCES = 	src$(S)core$(S)sys_processor.o  framework$(S)main.o framework$(S)gfx.o framework$(S)debugger.o \
			src$(S)core$(S)sys_debugger.o src$(S)core$(S)hardware.o \
  			$(IMPSRC)memory.o $(IMPSRC)graphics.o $(IMPSRC)console.o $(IMPSRC)keyboard.o $(IMPSRC)dispatch.o

FIRMWARE = ..$(S)firmware$(S)

CC = g++

.PHONY: all clean emulator prebuild release run

all: emulator

always:
	
run: always emulator
	$(EMULATOR) 
	
krun: always kernel emulator
	$(EMULATOR) $(BINDIR)kernel.bin@FC00 $(BINDIR)a1basic.bin@E000

emulator: prebuild $(APPNAME)
	$(CCOPY) $(APPNAME) ..$(S)bin
	
%.o:%.cpp
	$(CC) $(CADDRESSES) $(CXXFLAGS) -D IBMPC -D INCLUDE_DEBUGGING_SUPPORT -I cpu -I framework -I include -I import$(S)include -c -o $@ $<

%.o:%.c
	$(CC) $(CADDRESSES) $(CXXFLAGS) -D IBMPC -D INCLUDE_DEBUGGING_SUPPORT -I cpu -I framework -I include -I import$(S)include -c -o $@ $<

import: clean kernel always
	$(CCOPY) $(FIRMWARE)include$(S)common.h import$(S)include
	$(CCOPYREC) $(FIRMWARE)include$(S)interface import$(S)include
	$(CCOPYREC) $(FIRMWARE)include$(S)data import$(S)include
	$(CCOPYREC) $(FIRMWARE)sources$(S)interface import$(S)sources

kernel:	
	make -B -C ../kernel
	$(CCOPY) $(BINDIR)*_binary.h import$(S)include$(S)data
	$(TOUCH) import$(S)sources$(S)interface$(S)memory.c

always:

clean: import
	$(CDEL) $(APPNAME) 
	$(CDEL) src$(S)*.o 
	$(CDEL) src$(S)core$(S)*.o 
	$(CDEL) framework$(S)*.o

$(APPNAME): $(SOURCES)
	$(CC) $(SOURCES) $(LDFLAGS) $(SDL_LDFLAGS) -o $@

prebuild:
	make -C processor
	python scripts$(S)mapper.py >include$(S)hid2sdl.h
	$(CCOPY) $(BINDIR)*_binary.h include

fwbuild:
	make -C $(FIRMWARE)