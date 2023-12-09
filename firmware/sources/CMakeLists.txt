add_executable(firmware)

pico_generate_pio_header(firmware ${CMAKE_CURRENT_LIST_DIR}/system/sm0_memory_emulation_with_clock.pio)

target_sources(firmware PRIVATE main.c 
			hardware/dvi_320x240x256.c hardware/usbdriver.c hardware/timer.c hardware/sound.c hardware/usb_storage.c
			#system/processor_pio.c
			system/processor_bitbang.c
			interface/graphics.c interface/console.c interface/keyboard.c interface/memory.c 
			interface/dispatch.c interface/maths.c
)

target_compile_options(firmware PRIVATE -Wall)

target_compile_definitions(firmware
    PRIVATE PICO
)

target_link_libraries(firmware
	hardware_pio
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
	tinyusb_host
	fatfs
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
