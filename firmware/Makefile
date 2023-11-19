# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		Makefile
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		19th November 2023
#		Reviewed :	No
#		Purpose :	Makefile for this directory contents.
#
# ***************************************************************************************
# ***************************************************************************************

ifeq ($(OS),Windows_NT)
include ..\documents\common.make
else
include ../documents/common.make
endif

all: build

run: build upload

upload:
	cp build$(S)sources$(S)hello_dvi.elf firmware.elf
	openocd -f interface$(S)cmsis-dap.cfg -f target$(S)rp2040.cfg -c "adapter speed 5000" -c "program firmware.elf verify reset exit"

build:
	make -C build

rebuild: clean build

clean:
	rm -Rf build
	cmake -B build -S .

