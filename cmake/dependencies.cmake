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

# Suppress warnings from third-party raylib
if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(raylib PRIVATE -w)
endif()
