add_library(myproject_library INTERFACE)

target_sources(myproject_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/myproject_library.c)

# invoke pio_asm to assemble the state machine programs
#
pico_generate_pio_header(myproject_library ${CMAKE_CURRENT_LIST_DIR}/myproject_library.pio)

target_link_libraries(myproject_library INTERFACE
        pico_stdlib
        hardware_pio
        )

# add the `binary` directory so that the generated headers are included in the project
#
target_include_directories(myproject_library INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)