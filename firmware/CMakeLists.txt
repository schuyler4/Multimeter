# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)
#include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
# Set name of project (as PROJECT_NAME) and C/C++ Standards
project(Multimeter C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()
# point out the CMake, where to find the executable source file
add_executable(${PROJECT_NAME}
    main.c
    MCP3561.c
    calcs.c
    display_driver.c
    adjustment.c
)
# create map/bin/hex/uf2 files.
pico_add_extra_outputs(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
# Pull in our pico_stdlib which pulls in commonly used features (gpio, timer-delay etc)
target_link_libraries(${PROJECT_NAME} pico_stdlib)
target_link_libraries(${PROJECT_NAME} hardware_spi)

option(REV1 "Compile for revision 1 hardware" ON)
if(REV1)
    message("revision 1")
    add_compile_definitions(VOLTAGE_POSITIVE_CALIBRATION_OFFSET=true)
    add_compile_definitions(VOLTAGE_POSITIVE_CALIBRATION_OFFSET=false)
else()
    message("revision 2")
endif()
unset(REV1 CACHE)
