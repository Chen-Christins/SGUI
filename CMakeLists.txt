cmake_minimum_required(VERSION 3.14)
project(SGUI)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build." FORCE)
endif()

include(cmake/common.cmake)
include(cmake/dependencies.cmake)

# Add the SGUI library
add_library(sgui STATIC
    src/core/text.cc
    src/core/window.cc
)

target_include_directories(sgui PUBLIC src)
target_link_libraries(sgui PUBLIC raylib)

# ---- Tests ----
add_executable(test_text tests/test_text.cc)
target_link_libraries(test_text PRIVATE sgui)