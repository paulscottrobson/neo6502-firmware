# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# compile ASM with /usr/bin/arm-none-eabi-gcc
ASM_DEFINES = -DPICO_BOARD=\"pico\" -DPICO_BUILD=1 -DPICO_NO_HARDWARE=0 -DPICO_ON_DEVICE=1

ASM_INCLUDES = -I/aux/pico/pico-sdk/src/rp2_common/boot_stage2/asminclude -I/aux/pico/pico-sdk/src/rp2040/hardware_regs/include -I/aux/pico/pico-sdk/src/rp2_common/hardware_base/include -I/aux/pico/pico-sdk/src/common/pico_base/include -I/home/paulr/Projects/neo6502-firmware/testing/picodvi_cmake/app/build/generated/pico_base -I/aux/pico/pico-sdk/src/boards/include -I/aux/pico/pico-sdk/src/rp2_common/pico_platform/include -I/aux/pico/pico-sdk/src/rp2_common/boot_stage2/include

ASM_FLAGS = -mcpu=cortex-m0plus -mthumb -O3 -DNDEBUG

