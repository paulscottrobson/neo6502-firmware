@echo off
rem
rem		Where executables are
rem
set BINDIR=z:\bin
rem
rem		Convert source.bsc to a tokenised program
rem
python %BINDIR%\makebasic.zip source.bsc -oprogram.bas
rem
rem		Run it in the emulator. Loads program.bas in to 'page' (where program code goes)
rem		Then warm starts the interpreter. You can use exec to auto run it.
rem
%BINDIR%\neo.exe program.bas@page warm
