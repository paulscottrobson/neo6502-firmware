# ***************************************************************************************
# ***************************************************************************************
#
# Name     : Makefile
# Author   : Paul Robson (paul@robsons.org.uk)
# Date     : 20th November 2023
# Reviewed : No
# Purpose  : Main firmware makefile, most of the work is done by CMake.
#
# ***************************************************************************************
# ***************************************************************************************

ifeq ($(OS),Windows_NT)
include build_env\common.make
else
include build_env/common.make
endif


# ***************************************************************************************
#
# Remake everything to release state
#
# ***************************************************************************************

all: firmware-deps emulator-deps-nix emulator-deps-win
	$(CMAKEDIR) bin
	@echo building firmware
	$(MAKE) -B -C kernel release
	$(MAKE) -B -C basic release
	$(MAKE) -B -C firmware release
	@echo building emulators
	$(MAKE) -B -C emulator release
	$(MAKE) -B -C examples release
	@echo building release package
	$(MAKE) -B -C release


# ***************************************************************************************
#
# Make firmware only
#
# ***************************************************************************************

firmware: firmware-deps
	@echo building firmware
	$(CMAKEDIR) bin
	$(MAKE) -B -C kernel release
	$(MAKE) -B -C basic release
	$(MAKE) -B -C firmware release


# ***************************************************************************************
#
# Make emulator only
#
# ***************************************************************************************

windows: emulator-deps-nix emulator-deps-win
	@echo building windows emulator
	$(CMAKEDIR) bin
	$(MAKE) -B -C kernel
	$(MAKE) -B -C basic release
	$(MAKE) -B -C emulator clean
	$(MAKE) -B -C emulator ewindows
	$(MAKE) -B -C examples release

linux: emulator-deps-nix
	@echo building nix emulator
	$(CMAKEDIR) bin
	$(MAKE) -B -C kernel
	$(MAKE) -B -C basic release
	$(MAKE) -B -C emulator clean
	$(MAKE) -B -C emulator elinux
	$(MAKE) -B -C examples release

macos: emulator-deps-nix
	@echo building macos emulator
	make -B -C emulator emacos
	make -B -C examples release




# ***************************************************************************************
#
# Verify that dependencies are installed
#
# ***************************************************************************************

firmware-deps:
	@echo checking for firmware dependencies:
	@cmake             --version
	@g++               --version
	@arm-none-eabi-g++ --version
	@# NOTE: this is not accounting for 'arm-none-eabi-newlib'

emulator-deps-win:
	@x86_64-w64-mingw32-g++ --version

emulator-deps-nix:
	@echo checking for emulator dependencies:
	@g++         --version
	@64tass      --version
	@sdl2-config --version
	@zip         --version
	@python3     --version
	@python3 -c 'from importlib.metadata import version ; pkg="gitpython" ; print("python-%s: %s" % (pkg , version(pkg)))'
	@python3 -c 'from importlib.metadata import version ; pkg="pillow" ; print("python-%s: %s" % (pkg , version(pkg)))'

# ***************************************************************************************
#
# Clean everything
#
# ***************************************************************************************

clean:
	$(MAKE) -B -C kernel clean
	$(MAKE) -B -C basic clean
	$(MAKE) -B -C emulator clean
	$(MAKE) -B -C firmware clean


# ***************************************************************************************
#
# Easy testing of API documentation build
#
# ***************************************************************************************

testdoc:
	make -C firmware prelim
#	make -C release documentation
