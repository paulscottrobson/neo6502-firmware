add_executable(firmware)

set(STORAGE_TYPE "USB" CACHE STRING "Sets your storage type. Can be USB or SDCARD.")

if (DEFINED ENV{PICO_FATFS_PATH})
    set(no-os-fatfs-sd-spi-rpi-pico_SOURCE_DIR $ENV{PICO_FATFS_PATH})
    set(no-os-fatfs-sd-spi-rpi-pico_BINARY_DIR "_deps/no-os-fatfs-sd-spi-rpi-pico-build")
else()
    FetchContent_Declare(
        no-OS-FatFS-SD-SPI-RPi-Pico
        GIT_REPOSITORY https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico.git
        GIT_TAG        bbccc5d2deea09e128e2576ef35f72b473c5ba5f # just the latest on 30/01/2024
    )
    FetchContent_MakeAvailable(no-OS-FatFS-SD-SPI-RPi-Pico)
endif()

pico_generate_pio_header(firmware ${CMAKE_CURRENT_LIST_DIR}/system/sm0_memory_emulation_with_clock.pio)

add_definitions(-Wno-error=unused-variable)

set(COMMONSRC "../common/sources/interface")

file(GLOB PADSOURCES "hardware/gamepads/*.cpp")
file(GLOB COMMONSOURCES "${COMMONSRC}/*.cpp")

target_sources(firmware PRIVATE main.cpp
    #
    #       Common ${COMMONSRC} routines
    #
    ${COMMONSOURCES}
    #
    #       Hardware sources
    #
    hardware/dvi_320x240x256.cpp  hardware/usbdriver.cpp  hardware/timer.cpp  hardware/sound.cpp
    hardware/fileimplementation.cpp hardware/tick.cpp hardware/serial.cpp hardware/ports.cpp
    hardware/GamepadController.cpp ${PADSOURCES}

    #
    #       CPU type (pretty much permanently PIO now)
    #
    system/processor_pio.cpp
    #system/processor_bitbang.cpp
)

if(STORAGE_TYPE STREQUAL "USB")
    message("Building for USB Storage")
    add_definitions(-DUSBKEY_STORAGE=1)
    target_sources(firmware PRIVATE hardware/storage/usb_storage.cpp )
    target_link_libraries(firmware fatfs)
elseif(STORAGE_TYPE STREQUAL "SDCARD")
    message("Building for SDCARD Storage")
    target_sources(firmware PRIVATE hardware/storage/sdcard_storage.cpp )
    target_link_libraries(firmware FatFs_SPI)
    add_subdirectory(${no-os-fatfs-sd-spi-rpi-pico_SOURCE_DIR}/FatFs_SPI ${no-os-fatfs-sd-spi-rpi-pico_BINARY_DIR})
else()
    message(FATAL_ERROR "You need to set STORAGE_TYPE!")
endif()

target_compile_definitions(firmware
    PRIVATE PICO
    PRIVATE PICO_FLASH_SPI_CLKDIV=4
)

pico_define_boot_stage2(firmware_boot2 ${PICO_SDK_PATH}/src/rp2_common/boot_stage2/boot2_generic_03h.S)

target_compile_definitions(firmware_boot2 PRIVATE
	PICO_FLASH_SPI_CLKDIV=4
	PICO_FLASH_SIZE_BYTES=2097152
)

pico_set_boot_stage2(firmware firmware_boot2)

target_link_libraries(firmware
    hardware_pio
    pico_stdlib
    pico_multicore
    pico_util
    hardware_i2c
    hardware_adc
    hardware_spi
    libdvi
    tinyusb_host
    tinyusb_device
    tinyusb_board
)

target_include_directories(firmware PUBLIC ${CMAKE_CURRENT_LIST_DIR})
pico_add_extra_outputs(firmware)

add_custom_target(upload
    COMMAND openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program firmware.elf verify reset exit"
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Upload via picoprobe..."
)

add_custom_target(reset
    COMMAND openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "init" -c "reset run" -c "exit"
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Reset via picoprobe..."
)

add_dependencies(upload firmware)