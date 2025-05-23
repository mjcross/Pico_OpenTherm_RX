cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

#-------------------------------------------------
# Define the name of the project (subsequently reference it as ${PROJECT_NAME})
project(OT_rx C CXX ASM)
#-------------------------------------------------

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is <project name>.c, version 0.1

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.c )

# if the program name is different to the project name then set it here!
#pico_set_program_name(${PROJECT_NAME} "your_program_name")
pico_set_program_version(${PROJECT_NAME} "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME} 0)

# Add the standard library to the build
target_link_libraries(${PROJECT_NAME}
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

#------------------------------------
# generate our user-defined libraries
#------------------------------------
add_subdirectory(OT_rx_library)
add_subdirectory(OT_tx_library)

# Add additional libraries to the project
target_link_libraries(${PROJECT_NAME} 
        hardware_pio
        pico_rand
		OT_rx_library
        OT_tx_library
        )

pico_add_extra_outputs(${PROJECT_NAME})
