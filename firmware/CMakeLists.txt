cmake_minimum_required(VERSION 3.14)
set(PICO_TINYUSB_PATH $ENV{PICO_SDK_PATH}/lib/tinyusb)
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
project(firmware C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

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
)

include(FetchContent)

FetchContent_Declare(
    PicoDVI
    GIT_REPOSITORY https://github.com/Wren6991/PicoDVI
    GIT_TAG        a248d72b8cc95a08b43ef4307cd954eb801272b2 # just the latest on 11/12/2023
)
FetchContent_MakeAvailable(PicoDVI)

add_subdirectory(${picodvi_SOURCE_DIR}/software/libdvi ${picodvi_BINARY_DIR})
add_subdirectory(libraries/fatfs/source)
add_subdirectory(sources)