# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		Makefile
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		20th November 2023
#		Reviewed :	No
#		Purpose :	Main firmware makefile, most of the work is done by CMake.
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
#						Remake everything to release state
#
# ***************************************************************************************

all:
	$(CMAKEDIR) bin
	$(CMAKEDIR) release
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

firmware:
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

windows:
		@echo building windows emulator
		$(CMAKEDIR) bin
		$(MAKE) -B -C kernel
		$(MAKE) -B -C basic release
		$(MAKE) -B -C emulator clean
		$(MAKE) -B -C emulator ewindows
		$(MAKE) -B -C examples release

linux:
		@echo building nix emulator
		$(CMAKEDIR) bin
		$(MAKE) -B -C kernel
		$(MAKE) -B -C basic release
		$(MAKE) -B -C emulator clean
		$(MAKE) -B -C emulator elinux
		$(MAKE) -B -C examples release

macos:
		@echo building macos emulator
		make -B -C emulator emacos
		make -B -C examples release


# ***************************************************************************************
#
# Make documentation only
#
# ***************************************************************************************

docs:
	@echo building documentation
	$(MAKE) -B -C release documentation


# ***************************************************************************************
#
#								Clean everything
#
# ***************************************************************************************

clean:
	$(MAKE) -B -C kernel clean
	$(MAKE) -B -C basic clean
	$(MAKE) -B -C emulator clean
	$(MAKE) -B -C firmware clean

# ***************************************************************************************
#
#					  Easy testing of API documentation build
#
# ***************************************************************************************

testdoc:
	make -C firmware prelim
	make -C release documentation
