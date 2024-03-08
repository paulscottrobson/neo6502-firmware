# neo6502-firmware
Firmware for a Neo6502 Retrocomputer

## The home page
- http://www.neo6502.com

## The Documentation wiki
- https://github.com/paulscottrobson/neo6502-firmware/wiki

## Releases

The release tarballs contain pre-build firmware, emulators for windows and *nixes,
documentation, and examples/demos of BASIC, C, and assembly programs.
- https://github.com/paulscottrobson/neo6502-firmware/releases

## Compilation

### Dependencies

- Arch-based distros:

```
## firmware only
$ sudo pacman -S arm-none-eabi-gcc arm-none-eabi-newlib cmake

## *nix emulator only
$ sudo pacman -S 64tass python-gitpython python-pillow sdl2 zip

## windows emulator only - *nix emulator deps, plus:
$ sudo pacman -S mingw-w64-gcc

## documentation only
$ sudo pacman -S pandoc texlive-latex texlive-latexextra texlive-latexrecommended

## everything - firmware deps, *nix and windows emulator deps, documentation deps, plus:
$ sudo pacman -S git
```

- Debian-based distros:

```
## firmware only
$ sudo apt install build-essential cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib

## *nix emulator only
$ sudo apt install 64tass build-essential cmake libsdl2-dev gcc-arm-none-eabi git python3 python3-pip zip

## windows emulator only - *nix emulator deps, plus:
$ sudo apt install g++-mingw-w64-x86-64 mingw-w64

## documentation only
$ sudo apt install pandoc texlive-latex-base texlive-latex-extra texlive-latex-recommended

## everything - firmware deps, *nix and windows emulator deps, documentation deps, plus:
$ sudo apt install git
```

Note that to build the firmware, extra third-party sources are required.
These are not needed to build the emulators.
These will be downloaded automatically at build-time if not supplied.
That, and the 'git' dependency may be avoided by collecting those sources manually.
- pico-sdk:   https://github.com/raspberrypi/pico-sdk/releases
- tinyusb:    https://github.com/hathach/tinyusb/releases
- PicoDVI:    https://github.com/Wren6991/PicoDVI/archive/master.tar.gz
- pico_fatfs: https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico/releases
Then explode the tarballs and set the following variables in the environment:
```
  export PICO_SDK_FETCH_FROM_GIT=OFF
  export PICO_SDK_PATH=/path/to/picosdk_sources
  export PICO_TINYUSB_PATH=/path/to/tinyusb_sources
  export PICO_DVI_PATH=/path/to/picodvi_sources
  export PICO_FATFS_PATH=/path/to/picofatfs_sources
```

### Build

Build the firmware only:

```
$ make firmware
```

Build the windows emulator only:

```
$ make windows
```

Build the *nix emulator only:

```
$ make linux
```

Build the mac emulator only:

```
$ make macos
```

Build the documentation only:

```
$ make docs
```

Build everything (firmware, all emulators, documentation, examples, demos, etc):

```
$ make
```
