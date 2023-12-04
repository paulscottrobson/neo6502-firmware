# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		Makefile
#		Purpose :	Build Basic
#		Date :		25th May 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

ifeq ($(OS),Windows_NT)
include ..\..\..\..\documents\common.make
else
include ../../../../documents/common.make
endif


create: 
	python asmtest.py
	$(CCOPY) assem.bas ..$(S)..$(S)build$(S)test.bas
	
check:
	python asmcheck.py	