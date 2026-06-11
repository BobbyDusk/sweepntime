cmake_minimum_required(VERSION 3.25)
project(Sweepntime C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Look for the SDL3 package provided by your Nix environment
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED)

# Enable AddressSanitizer for debug builds when requested.
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")

# Your targets
add_executable(sweepntime ${SOURCES})

# 2. Link your executable against SDL3
# This automatically handles include paths and linking flags
target_link_libraries(sweepntime PRIVATE 
    SDL3::SDL3 
    SDL3_image::SDL3_image 
    SDL3_mixer::SDL3_mixer 
    SDL3_ttf::SDL3_ttf
)

# 3. Base strict warning options for your specific target
target_compile_options(sweepntime PRIVATE -Wall -Werror -Wextra -Wpedantic -fcolor-diagnostics)

# 4. Use lld linker for your specific target
target_link_options(sweepntime PRIVATE -fuse-ld=lld)

# 5. Enable target-specific ASan if requested
if(ENABLE_ASAN)
    target_compile_options(sweepntime PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(sweepntime PRIVATE -fsanitize=address)
endif()