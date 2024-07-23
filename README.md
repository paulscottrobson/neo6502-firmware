# neo6502-firmware
Firmware and emulators for the Neo6502 retro-computer

## The home page
- http://www.neo6502.com

## The Documentation wiki
- https://github.com/paulscottrobson/neo6502-firmware/wiki

## Releases

The release tarballs contain pre-built firmware, emulators for windows and *nixes,
documentation, and examples/demos of BASIC, C, and assembly programs.
- https://github.com/paulscottrobson/neo6502-firmware/releases

## Compilation

### Sources

Note that to build the firmware, additional third-party sources are required.
If not supplied, these will be downloaded automatically at build-time.
These are not needed to build the emulators however.
The automatic downloads, and the 'git' dependency may be avoided by collecting these sources manually.

* pico-sdk:   https://github.com/raspberrypi/pico-sdk/releases
* tinyusb:    https://github.com/hathach/tinyusb/releases
* PicoDVI:    https://github.com/Wren6991/PicoDVI/archive/master.tar.gz
* pico-fatfs: https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico/releases

Then explode the tarballs and set the following variables in the environment:

```
$ export PICO_SDK_FETCH_FROM_GIT=OFF
$ export PICO_SDK_PATH=/path/to/picosdk_sources
$ export PICO_TINYUSB_PATH=/path/to/tinyusb_sources
$ export PICO_DVI_PATH=/path/to/picodvi_sources
$ export PICO_FATFS_PATH=/path/to/picofatfs_sources
```

### Dependencies

- 'build-essential', 'base-devel', or equivalent (GCC for *nix binaries)
- 'x86_64-w64-mingw32-g++', or equivalent (GCC for windows binaries)
- 'gcc-arm-none-eabi' (GCC for ARM binaries)
- 'arm-none-eabi-newlib' (C stdlibs for bare-metal ARM target),
- '64tass' (6502 assembler)
- 'cmake'
- 'libsdl2-dev', 'sdl2', or equivalent (SDL2 development libraries)
- 'python3'
    - 'pillow'
    - 'gitpython'
    - 'pyserial' (not needed at build-time - optional for uploading files via serial port)

All of the sources and dependencies are freely available.
Most distros have the dependencies packaged.

* Arch-based distros:  
  _NOTE: '64tass' is in the AUR (https://aur.archlinux.org/packages/64tass)_.
  * Firmware only:
    ```
    $ sudo pacman -S arm-none-eabi-gcc arm-none-eabi-newlib cmake
    ```
  * *nix emulator only:
    ```
    $ sudo pacman -S 64tass python-gitpython python-pillow sdl2 zip
    ```
  * Windows emulator only:
    ```
    $ sudo pacman -S 64tass mingw-w64-gcc python-gitpython python-pillow sdl2 zip
    ```
  * Everything (firmware, all emulators, documentation, examples, demos, etc):
    ```
    $ sudo pacman -S 64tass arm-none-eabi-gcc arm-none-eabi-newlib cmake git mingw-w64-gcc python-gitpython python-pillow sdl2 zip
    ```

* Debian-based distros:
  * Firmware only:
    ```
    $ sudo apt install build-essential cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
    ```
  * *nix emulator only:
    ```
    $ sudo apt install 64tass build-essential cmake libsdl2-dev python3 python3-pip zip
    ```
  * Windows emulator only:
    ```
    $ sudo apt install 64tass build-essential cmake g++-mingw-w64-x86-64 libsdl2-dev mingw-w64 python3 python3-pip zip
    ```
  * Everything (firmware, all emulators, documentation, examples, demos, etc):
    ```
    $ sudo apt install 64tass build-essential cmake g++-mingw-w64-x86-64 gcc-arm-none-eabi git libnewlib-arm-none-eabi libsdl2-dev libstdc++-arm-none-eabi-newlib mingw-w64 python3 python3-pip zip
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
