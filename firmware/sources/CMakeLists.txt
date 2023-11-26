add_executable(firmware)

pico_generate_pio_header(firmware ${CMAKE_CURRENT_LIST_DIR}/system/sm0_memory_emulation_with_clock.pio)

target_sources(firmware PRIVATE main.c 
			hardware/dvi_320x240x256.c hardware/usbdriver.c hardware/timer.c hardware/sound.c
			# system/processor_pio.c
			system/processor_bitbang.c
			interface/graphics.c interface/console.c interface/keyboard.c interface/memory.c interface/dispatch.c
)

target_compile_options(firmware PRIVATE -Wall)

target_compile_definitions(firmware
    PRIVATE PICO
)

target_link_libraries(firmware
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
	tinyusb_host
	tinyusb_board
)

target_include_directories(firmware PUBLIC ${CMAKE_CURRENT_LIST_DIR})
pico_add_extra_outputs(firmware)