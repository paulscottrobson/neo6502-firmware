# ***********************************************************************************
#
#										Common Build 
#
# ***********************************************************************************
#
#	NB: Windows SDL2 is hard coded.
#
ifeq ($(OS),Windows_NT)
CCOPY = copy
CMAKE = make
CDEL = del /Q
CDELQ = >NUL
APPSTEM = .exe
S = \\
SDLDIR = C:\\sdl2
CXXFLAGS = -I$(SDLDIR)$(S)include$(S)SDL2 -I . -fno-stack-protector -w -Wl,-subsystem,windows -DSDL_MAIN_HANDLED
LDFLAGS = -lmingw32
SDL_LDFLAGS = -L$(SDLDIR)$(S)lib -lSDL2 -lSDL2main -static-libstdc++ -static-libgcc
OSNAME = windows
EXTRAFILES = libwinpthread-1.dll  SDL2.dll
PYTHON = python
else
CCOPY = cp
CDEL = rm -f
CDELQ = 
CMAKE = make
APPSTEM =
S = /
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LDFLAGS = $(shell sdl2-config --libs)
CXXFLAGS = $(SDL_CFLAGS) -O2 -DLINUX  -fmax-errors=5 -I.  
LDFLAGS = 
OSNAME = linux
EXTRAFILES = 
PYTHON = python3
endif
#
#		Directories
#
ROOTDIR =  $(dir $(realpath $(lastword $(MAKEFILE_LIST))))..$(S)
BINDIR = $(ROOTDIR)bin$(S)
SRCDIR = $(ROOTDIR)kernel$(S)
AEMUDIR = $(ROOTDIR)$(S)..$(S)fab-agon-emulator$(S)
#
#		Current applications.
# 
MAKEOPTS = --no-print-directory
PYTHON = python3
ASSEMBLER = 64tass -b --mr65c02 -Wall
ASMEND = 
#
#		Export path to the common scripts.
#
ifeq ($(OS),Windows_NT)
SET PYTHONPATH=$(CSCRIPTS)
else
export PYTHONPATH=$(CSCRIPTS)
endif
#
#		Uncommenting .SILENT will shut the whole build up.
#
ifndef VERBOSE
#.SILENT:
endif

