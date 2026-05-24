cmake_minimum_required(VERSION 3.14)
project(SGUI)

include(cmake/common.cmake)
include(cmake/dependencies.cmake)

# Add the SGUI library
add_library(sgui STATIC
    src/core/widget.cc
    src/core/modifier.cc
    src/core/layout.cc
    src/core/text_field.cc
    src/core/checkbox.cc
    src/core/scroll.cc
)

target_include_directories(sgui PUBLIC src)
target_link_libraries(sgui PUBLIC raylib)

add_executable(main
    src/main.cpp
)

target_link_libraries(main PRIVATE sgui)

# ---- Tests ----
add_executable(test_layout tests/test_layout.cpp)
target_link_libraries(test_layout PRIVATE sgui)

add_executable(test_button tests/test_button.cpp)
target_link_libraries(test_button PRIVATE sgui)

add_executable(test_textfield tests/test_textfield.cpp)
target_link_libraries(test_textfield PRIVATE sgui)

add_executable(test_checkbox tests/test_checkbox.cpp)
target_link_libraries(test_checkbox PRIVATE sgui)

add_executable(test_scroll tests/test_scroll.cpp)
target_link_libraries(test_scroll PRIVATE sgui)

add_executable(test_composition_local tests/test_composition_local.cpp)
target_link_libraries(test_composition_local PRIVATE sgui)
