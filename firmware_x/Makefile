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

all: compile

run: compile upload

upload:
	cp build$(S)sources$(S)firmware.elf .
	cp build$(S)sources$(S)firmware.uf2 .
	openocd -f interface$(S)cmsis-dap.cfg -f target$(S)rp2040.cfg -c "adapter speed 5000" -c "program firmware.elf verify reset exit"

compile:
	make -C build

rebuild: clean compile

clean:
	rm -Rf build
	cmake -B build -S .

