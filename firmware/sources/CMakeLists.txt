add_executable(firmware)

pico_generate_pio_header(firmware ${CMAKE_CURRENT_LIST_DIR}/system/sm0_memory_emulation_with_clock.pio)

add_definitions(-Wno-error=unused-variable)

target_sources(firmware PRIVATE main.cpp  
    #
    #       Common interface routines
    #
    interface/graphics.cpp  interface/console.cpp  interface/keyboard.cpp  interface/memory.cpp  
    interface/dispatch.cpp  interface/maths.cpp  interface/config.cpp  interface/efla.cpp 
    interface/gfxcommands.cpp  interface/ellipse.cpp  interface/fileinterface.cpp  
    interface/sprites.cpp  interface/logo.cpp  interface/sprites_xor.cpp  interface/tilemap.cpp
    interface/sndmanager.cpp  interface/sfxmanager.cpp interface/tick.cpp
    #
    #       Hardware sources
    #
    hardware/dvi_320x240x256.cpp  hardware/usbdriver.cpp  hardware/timer.cpp  hardware/sound.cpp  
    hardware/fdebug.cpp  hardware/fileimplementation.cpp hardware/tick.cpp
    #
    #       CPU type (pretty much permanently PIO now)
    #
    system/processor_pio.cpp 
    #system/processor_bitbang.cpp 
)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

if(USBKEY_STORAGE)
    add_definitions(-DUSBKEY_STORAGE=1)
    target_sources(firmware PRIVATE hardware/storage/usb_storage.cpp )
endif()
if(SDCARD_STORAGE)
    add_definitions(-DSDCARD_STORAGE=1)
     target_sources(firmware PRIVATE hardware/storage/sdcard_storage.cpp )
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
    libdvi
    tinyusb_host
    tinyusb_device
    tinyusb_board
)

if(USBKEY_STORAGE)
    target_link_libraries(firmware fatfs)
endif()
if(SDCARD_STORAGE)
    target_link_libraries(firmware FatFs_SPI)
endif()

target_include_directories(firmware PUBLIC ${CMAKE_CURRENT_LIST_DIR})
pico_add_extra_outputs(firmware)

add_subdirectory(../libraries/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

add_custom_target(upload
    COMMAND openocd -f interface/cmsis-dap.cpp fg -f target/rp2040.cpp fg -c "adapter speed 5000" -c "program firmware.elf verify reset exit"
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Upload via picoprobe..."
)

add_custom_target(reset
    COMMAND openocd -f interface/cmsis-dap.cpp fg -f target/rp2040.cpp fg -c "adapter speed 5000" -c "init" -c "reset run" -c "exit"
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Reset via picoprobe..."
)

add_dependencies(upload firmware)
