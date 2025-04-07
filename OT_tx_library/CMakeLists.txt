add_library(OT_tx_library INTERFACE)

target_sources(OT_tx_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/OT_tx_library.c)

# invoke pio_asm to assemble the state machine programs
#
pico_generate_pio_header(OT_tx_library ${CMAKE_CURRENT_LIST_DIR}/OT_tx_library.pio)

target_link_libraries(OT_tx_library INTERFACE
        pico_stdlib
        hardware_pio
        )

# add the `binary` directory so that the generated headers are included in the project
#
target_include_directories(OT_tx_library INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)