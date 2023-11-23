# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		Makefile
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		20th November 2023
#		Reviewed :	No
#		Purpose :	Makefile for Apple 1 BASIC
#
# ***************************************************************************************
# ***************************************************************************************

ifeq ($(OS),Windows_NT)
include ..\..\..\documents\common.make
else
include ../../../documents/common.make
endif

all: build 

build:always
	$(PYTHON) translate.py >forth.asm
	$(ASSEMBLER) forth.asm -o forth.bin -L forth.lst $(ASMEND)
	cmp forth.bin forth.bin.correct

always:


