@echo off
rem
rem		Where executables are
rem
set BINDIR=../../../bin
rem
rem		Convert graphics files and put in storage (which is the SD Card/USB stick in the emulator.)
rem		This takes the 3 png files and creates graphics.gfx
rem 
python %BINDIR%\makeimg.zip
if not exist storage md storage
copy graphics.gfx storage
rem
rem		Convert source.bsc to a tokenised program
rem
python %BINDIR%\makebasic.zip source.bsc -oprogram.bas
rem
rem		Run it in the emulator. Loads program.bas in to 'page' (where program code goes)
rem		Then start the interpreter and runs the program (warm just starts it)
rem
%BINDIR%\neo.exe program.bas@page exec
