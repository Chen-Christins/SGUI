cmake_minimum_required(VERSION 3.14)
project(SGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Fetch raylib for rendering
# Add policy bypass for Raylib 5.0 CMake requirement error
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://gitee.com/mirrors/raylib.git
  GIT_TAG 5.0 # Use stable 5.0 release
  GIT_SHALLOW ON # Make cloning much faster
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_executable(main 
    src/main.cpp
)

target_include_directories(main PUBLIC include)
target_link_libraries(main PRIVATE raylib)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)