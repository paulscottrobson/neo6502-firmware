# FatFs for USB storage

These unmodified FatFs R0.15 with patch1 files were copied from TinyUSB 0.16.0,
`lib/fatfs/source/`, including its existing `ffconf.h` configuration.
TinyUSB 0.21.0 no longer bundles this filesystem library.

Original project by ChaN: https://elm-chan.org/fsw/ff/

Copied from: https://github.com/hathach/tinyusb/tree/0.16.0/lib/fatfs/source

The copyright and redistribution conditions are retained in the source files.
`diskio.c` is intentionally omitted: the Neo6502 implements the disk interface
in `firmware/sources/hardware/storage/usb_storage.cpp`.
