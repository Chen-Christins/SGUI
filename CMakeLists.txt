cmake_minimum_required(VERSION 3.14)
project(SGUI)

include(cmake/common.cmake)
include(cmake/dependencies.cmake)

# Add the SGUI library
add_library(sgui STATIC
    src/core/widget.cc
)

target_include_directories(sgui PUBLIC src)
target_link_libraries(sgui PUBLIC raylib)

add_executable(main
    src/main.cpp
)

target_link_libraries(main PRIVATE sgui)
