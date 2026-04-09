cmake_minimum_required(VERSION 3.13...3.27)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico2_w)

include(external/pico-sdk/pico_sdk_init.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(my_project)

pico_sdk_init()

# rest of your project
add_executable(main main.cpp)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(main
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
)

pico_add_extra_outputs(main)
