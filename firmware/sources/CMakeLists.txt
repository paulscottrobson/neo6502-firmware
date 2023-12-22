add_executable(firmware)

pico_generate_pio_header(firmware ${CMAKE_CURRENT_LIST_DIR}/system/sm0_memory_emulation_with_clock.pio)

add_definitions(-w)

target_sources(firmware PRIVATE main.c 
    #
    #       Hardware sources
    #
    hardware/dvi_320x240x256.c hardware/usbdriver.c hardware/timer.c hardware/sound.c 
    hardware/fdebug.c hardware/fileio.c
    #
    #       Select Storage interface
    #
    hardware/storage/usb_storage.c
    #hardware/storage/null_storage.c
    #hardware/storage/sdcard_storage.c
    #
    #       Select CPU driver
    #
    system/processor_pio.c
    #system/processor_bitbang.c
    #
    #       Common interface routines
    #
    interface/graphics.c interface/console.c interface/keyboard.c interface/memory.c 
    interface/dispatch.c interface/maths.c
)

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
    libdvi
    tinyusb_host
    tinyusb_device
    tinyusb_board
    fatfs
    #FatFs_SPI
)

target_include_directories(firmware PUBLIC ${CMAKE_CURRENT_LIST_DIR})
pico_add_extra_outputs(firmware)

add_subdirectory(../libraries/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

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
