Release Notes
-------------

This is a more advanced version, save/load works, the NeoBasic is implemented (no sound/graphics yet) and Apple 1 BASIC
and Fig FORTH have been removed.

BASIC is booted via 'N' or 800R. There is a short documentation as a pdf/odt file available and some sample code in the
samples.

All suggestions are welcome.

Build Requirements
------------------
Gnu C for the platform(s)
Pico SDK
Python 3 & PIL/Pillow installed
SDL2

Running the emulator / real hardware - keyboard development
-----------------------------------------------------------

This has been tested pretty visciously and has held up so far. However, there may well be bugs. 

So if you want to type code into the emulator, or the real machine, it's advisable that you save regularly and save to
different file names (e.g.  myprog.001 myprog.002 etc.) just in case something goes awry. If it does, let me know.

The SDCard seems more robust than the USB Key but my experience is if it works it always works. 

If using the emulator be careful of the ESC key (which exits it) and the Function keys, which do debuggery things.

Running the emulator - Cross Development
----------------------------------------

The emulator has several options on the command line.

	"cold" boots BASIC in standard mode, avoiding the monitor
	"warm" boots BASIC in warmstart mode, anything in memory will be retained.
	"exec" boots BASIC and runs the program in memory.

Program can be loaded into memory using the syntax <file>@<address in hex>. That address can be "page" which means
the address BASIC expects to find the code in.
	
So to load and run a program use the following :

	../bin/neo build/basic.bin@800  build/tokenised.dat@page exec

Or in windows

	..\bin\neo.exe build\basic.bin@800  build\tokenised.dat@page exec

This
	(1) starts the emulator
	(2) loads the BASIC interpreter to $800 (this is normally done by the monitor which is being skipped)
	(3) loads the tokenised BASIC program to "page" (currently at $3200 but changes over time)
	(4) sets it to run 'exec'

One can create an example BASIC program from a text equivalent (see the basic/code directory for many examples), this is done
with the makebasic.zip program which requires Python 3

Something like 

	python3 makebasic.zip build/source.txt -obuild/tokenised.dat

this converts the file "source.txt" which is a basic source file in text form, line numbers optional to a tokenised program 
which can be run as above.

These examples work. Obviously you may want to change file names and directories.

My Development Environment
--------------------------
Ubuntu 23.10. Sublime Text is the editor,  ARM GCC v13.2, Python 3.11.6 with PIL/Pillow installed (does the graphics conversion stuff). 
64tass does the 6502 assembly. CMake 3.28.1 Gnu Make 4.4.1 (though it doesn't do anything clever !). 
Documentation is mostly libreoffice. Hardware is a i5-4690 with 16Gb of RAM, most of the uploading uses a Pico based debugger from 
"The Pi Hut" which I think is a standard design. A Raspi Keyboard with USB hub, a SanDisk USB3.1 key and 
Olimex's SDCard device are permanently plugged in. You can't use both at once but they don't seem to upset each other.

*****************
*** IMPORTANT ***
*****************

If you are using the SD Card storage board made by Olimex, it requires the switches 2-4 on the B boards to be OFF. The 
board is shipped with all switches set to 'on' (Switch 1 is the on board beeper). 

Without this change the SD Card will not work (see the board manual page 12).

Paul Robson.
paul@robsons.org.uk
14th January 2024
