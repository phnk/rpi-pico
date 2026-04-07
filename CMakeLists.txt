cmake_minimum_required(VERSION 3.13...3.27)
set(PICO_BOARD pico2)

include(external/pico-sdk/pico_sdk_init.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(my_project)

pico_sdk_init()

# rest of your project
add_executable(main main.cpp)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

target_link_libraries(main pico_stdlib)

pico_add_extra_outputs(main)
