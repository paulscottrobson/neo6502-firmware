
add_executable(firmware main.c 
						hardware/dvi_video.c hardware/usbkeyboard.c 
						system/memory.c system/processor.c
)

target_compile_options(firmware PRIVATE -Wall)

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