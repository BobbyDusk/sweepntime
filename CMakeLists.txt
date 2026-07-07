cmake_minimum_required(VERSION 3.25)
project(Sweepntime C)

include(FetchContent)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Force CMake to explicitly list all include paths in compile_commands.json
# instead of hiding them as "system implicit" paths.
set(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "")
set(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "")

# 1. Look for the SDL3 package provided by your Nix environment
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED)

FetchContent_Declare(
    flecs
    GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
    GIT_TAG        master # Or use a specific version tag like v4.1.5
)

# Fetch Box2D (Pure C version)
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG        v3.1.1 # Pinned to stable version v3
)
FetchContent_MakeAvailable(flecs box2d)

# Enable AddressSanitizer for debug builds when requested.
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS "lib/*.c")

if(LIB_SOURCES)
    add_library(thirdparty_libs STATIC ${LIB_SOURCES})
    # Mark 'lib' folder as SYSTEM so headers don't complain when included elsewhere
    target_include_directories(thirdparty_libs SYSTEM PUBLIC lib)
endif()

# Your targets
add_executable(sweepntime ${SOURCES})

target_include_directories(sweepntime SYSTEM PRIVATE src)

# 2. Link your executable against SDL3
# This automatically handles include paths and linking flags
target_link_libraries(sweepntime PRIVATE 
    SDL3::SDL3 
    SDL3_image::SDL3_image 
    SDL3_mixer::SDL3_mixer 
    SDL3_ttf::SDL3_ttf
    flecs::flecs_static
    box2d::box2d
)

if(TARGET thirdparty_libs)
    target_link_libraries(sweepntime PRIVATE thirdparty_libs)
endif()

# 3. Base strict warning options for your specific target
target_compile_options(sweepntime PRIVATE -Wall -Werror -Wextra -Wpedantic -fcolor-diagnostics)

# 4. Use lld linker for your specific target
target_link_options(sweepntime PRIVATE -fuse-ld=lld)

# 5. Enable target-specific ASan if requested
if(ENABLE_ASAN)
    target_compile_options(sweepntime PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(sweepntime PRIVATE -fsanitize=address)
endif()