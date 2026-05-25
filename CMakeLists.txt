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
    src/core/slider.cc
    src/core/toggle.cc
    src/core/image.cc
    src/core/progress_bar.cc
    src/core/separator.cc
    src/core/radio_button.cc
    src/core/dropdown.cc
    src/core/tabs.cc
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

add_executable(test_slider tests/test_slider.cpp)
target_link_libraries(test_slider PRIVATE sgui)

add_executable(test_toggle tests/test_toggle.cpp)
target_link_libraries(test_toggle PRIVATE sgui)

add_executable(test_image tests/test_image.cpp)
target_link_libraries(test_image PRIVATE sgui)

add_executable(test_progress_bar tests/test_progress_bar.cpp)
target_link_libraries(test_progress_bar PRIVATE sgui)

add_executable(test_separator tests/test_separator.cpp)
target_link_libraries(test_separator PRIVATE sgui)

add_executable(test_radio_button tests/test_radio_button.cpp)
target_link_libraries(test_radio_button PRIVATE sgui)

add_executable(test_dropdown tests/test_dropdown.cpp)
target_link_libraries(test_dropdown PRIVATE sgui)

add_executable(test_tabs tests/test_tabs.cpp)
target_link_libraries(test_tabs PRIVATE sgui)
