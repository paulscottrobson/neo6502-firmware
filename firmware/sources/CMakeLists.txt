
add_executable(firmware main.c 
			hardware/dvi_320x240x256.c hardware/usbdriver.c hardware/timer.c hardware/sound.c
			system/processor.c
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