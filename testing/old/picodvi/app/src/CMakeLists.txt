
add_executable(hello_dvi main.c
)

target_compile_options(hello_dvi PRIVATE -Wall)

target_link_libraries(hello_dvi
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
)

pico_add_extra_outputs(hello_dvi)