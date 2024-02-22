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
	$(MAKE) -B -C kernel release
	$(MAKE) -B -C basic release
	$(MAKE) -B -C firmware release
	$(MAKE) -B -C emulator release
	$(MAKE) -B -C examples release
	$(MAKE) -B -C release

# ***************************************************************************************
#
#							Make windows & linux versions
#
# ***************************************************************************************

windows:
		$(CMAKEDIR) bin
		$(MAKE) -B -C kernel
		$(MAKE) -B -C basic release
		$(MAKE) -B -C emulator clean
		$(MAKE) -B -C emulator ewindows

linux:
		$(CMAKEDIR) bin
		$(MAKE) -B -C kernel
		$(MAKE) -B -C basic release
		$(MAKE) -B -C emulator clean
		$(MAKE) -B -C emulator elinux

macos:
		make -B -C emulator emacos


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

