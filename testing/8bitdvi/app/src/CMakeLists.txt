
add_executable(hello_dvi main.c
)

target_compile_options(hello_dvi PRIVATE -Wall)

target_link_libraries(hello_dvi
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
	tinyusb_host
	tinyusb_board
)

target_include_directories(hello_dvi PUBLIC ${CMAKE_CURRENT_LIST_DIR})
pico_add_extra_outputs(hello_dvi)