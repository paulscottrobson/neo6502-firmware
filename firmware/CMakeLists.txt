cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)


if(NOT DEFINED ENV{PICO_SDK_FETCH_FROM_GIT})
    set(PICO_SDK_FETCH_FROM_GIT on)
    message("Defaulting PICO_SDK_FETCH_FROM_GIT")
endif()
include(pico_sdk_import.cmake)

project(firmware C CXX ASM)

include(FetchContent)

# if((NOT DEFINED ENV{PICO_TINYUSB_FETCH_FROM_GIT}) OR ENV{PICO_TINYUSB_FETCH_FROM_GIT})
if(DEFINED ENV{PICO_TINYUSB_PATH})
    set(tinyusb_SOURCE_DIR $ENV{PICO_TINYUSB_PATH})
else()
    FetchContent_Declare(
        tinyusb
        GIT_REPOSITORY https://github.com/hathach/tinyusb.git
        GIT_TAG        0.16.0
    )
    FetchContent_MakeAvailable(tinyusb)
endif()
set(PICO_TINYUSB_PATH ${tinyusb_SOURCE_DIR})

pico_sdk_init()

add_compile_options(-Wall -Werror -O2)

# To change the default serial config for all apps, pass e.g.
#   cmake -DDVI_DEFAULT_SERIAL_CONFIG=pimoroni_demo_hdmi_cfg ..
# then rebuild. This is applied via target_compile_definitions on each app
# individually.
set(DVI_DEFAULT_SERIAL_CONFIG "pico_sock_cfg" CACHE STRING
    "Select a default pin configuration from common_dvi_pin_configs.h")

include_directories(
    include
    common/include
)

if(DEFINED ENV{PICO_DVI_PATH})
    set(picodvi_SOURCE_DIR $ENV{PICO_DVI_PATH})
    set(picodvi_BINARY_DIR "_deps/picodvi-build")
else()
    FetchContent_Declare(
        PicoDVI
        GIT_REPOSITORY https://github.com/paulscottrobson/PicoDVI
        #GIT_TAG        a248d72b8cc95a08b43ef4307cd954eb801272b2 # just the latest on 11/12/2023
    )
    FetchContent_MakeAvailable(PicoDVI)
endif()
add_subdirectory(${picodvi_SOURCE_DIR}/software/libdvi ${picodvi_BINARY_DIR})

add_library(fatfs INTERFACE)
target_sources(fatfs INTERFACE
    ${tinyusb_SOURCE_DIR}/lib/fatfs/source/ff.c
    ${tinyusb_SOURCE_DIR}/lib/fatfs/source/ffsystem.c
    ${tinyusb_SOURCE_DIR}/lib/fatfs/source/ffunicode.c
)
target_include_directories(fatfs INTERFACE ${tinyusb_SOURCE_DIR}/lib/fatfs/source/)

add_subdirectory(sources)

pico_enable_stdio_usb(firmware 0)
pico_enable_stdio_uart(firmware 0)
