# neo6502-firmware
Firmware for a Neo6502 Retrocomputer

## Getting Started:

The release files on the right contain eveything you need to get started:
- documentation
- samples and example code and explainers.
- firmware for the RP 2040 with WozMon and BASIC for SDCard or USB Host.
- tools for cross development
- Windows and Ubuntu emulators

## Building:
### required software:
- Pico SDK (Remember to set PICO_SDK_PATH to point where it's installed)
- 64tass
- gcc-arm-none-eabi
- cmake
- build-essential
- python3
- Python Imaging Library / Pillow

run:

``` 
build_all.sh
```	

## Building emulators only
### required software
- gcc-arm-none-eabi *OR* build-essential
- python3
- Python Imaging Library / Pillow

make linux or make windows

#Release Contents

see: release/neo6502.zip after running build_all.sh

### Neo6502 firmware
* firmware_usb.uf2 - Neo6502 firmware with USB mass storage support
* firmware_sd.uf2 -  Neo6502 firmware with SD Card support

### Documents
* api.pdf - Neo6502 API description
* basic.pdf - Basic Reference
* memorymap.pdf - The memory map
* wozmon.pdf - documentation about the monitor
* various text files

### PC Software
* neolinux.zip neowin.zip - emulators
* listbasic.zip - Python zipapp for converting tokenized basic to ASCII basic source code
* makebasic.zip - Python zipapp for converting ASCII text source code to tokenized basic.
* createblanks.zip - create skeletons for graphics
* makeimg.zip - convert graphics to the internal format

### Neo6502 software
* samples - assorted examples, games, demos, test programs
* crossdev - a working example of cross platform development