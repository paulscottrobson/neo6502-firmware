REM Hello World assembly example for the Neo6502 (build script)
REM SPDX-License-Identifier: CC0-1.0
REM
REM requires 'mads' assembler
REM adjust PATH according to your build host


@setlocal
@SET PATH=%PATH%;D:\mads;D:\neo6502


@if %ERRORLEVEL% == 0 mads hello.asm -x -o:hello.neo
@if %ERRORLEVEL% == 0 neo.exe hello.neo@800 exec

