# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		common.make
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		20th November 2023
#		Reviewed :	No
#		Purpose :	Common hopefully cross platform elements. God knows with Windows.
#
# ***************************************************************************************
# ***************************************************************************************
#
#	NB: Windows SDL2 is hard coded, C:\SDL2 should contain the four directories
# 	(bin include etc.) unpacked from the archived mingw version of SDL.
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
TOUCH =
PICO_SDK_PATH=<somewhere, god alone knows>
PYTHON = c:\Python312\python.exe
else
PYTHON = python3
CCOPY = cp
CCOPYREC = cp -r
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
TOUCH = touch -c
PICO_SDK_LOCATION=/usr/share/pico_sdk
endif
#
#		Directories
#
ROOTDIR =  $(dir $(realpath $(lastword $(MAKEFILE_LIST))))..$(S)
BINDIR = $(ROOTDIR)bin$(S)
SRCDIR = $(ROOTDIR)kernel$(S)
CSCRIPTS = $(ROOTDIR)basic$(S)scripts
#
#		Current applications.
# 
ASSEMBLER = 64tass -q -b --mr65c02 -Wall
ASMEND = 
UPLOADER = openocd 
UPCONFIG = -f interface/cmsis-dap.cfg -f target/rp2040.cfg 
UPCOMMANDS = -c "adapter speed 5000" -c "program $(BINARY).elf verify reset exit"
EMULATOR = $(BINDIR)neo$(APPSTEM)
#
#		Export path to the common scripts.
#
ifeq ($(OS),Windows_NT)
PYTHONPATH=%CSCRIPTS%
else
export PYTHONPATH=$(CSCRIPTS)
endif
#
#		Uncommenting .SILENT will shut the whole build up.
#
ifndef VERBOSE
.SILENT:
endif

