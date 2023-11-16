# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		Makefile
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		17th October 2023
#		Reviewed :	No
#		Purpose :	Makefile for this directory contents.
#
# ***************************************************************************************
# ***************************************************************************************

ifeq ($(OS),Windows_NT)
include ..\..\documents\common.make
else
include ../../documents/common.make
endif

all: build

prebuild:
	tools$(S)pioasm src$(S)memory_sm0_clock.pio src$(S)memory_sm0_clock.pio.h
	tools$(S)pioasm src$(S)memory_sm1_address.pio src$(S)memory_sm1_address.pio.h

build: prebuild

	pio run

run: prebuild
	pio run -t upload