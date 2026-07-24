#!/usr/bin/env just --justfile

set shell := ["bash", "-u", "-c"]

# Default recipe
default:
    @just run-asan

# Build directories
BUILD_DEBUG := "build/debug"
BUILD_ASAN := "build/asan"
BUILD_RELEASE := "build/release"

# Build with Debug symbols (for valgrind, lldb)
build-debug:
    cmake -B {{BUILD_DEBUG}} -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cmake --build {{BUILD_DEBUG}}
    ln -sf {{BUILD_DEBUG}}/compile_commands.json .

# Build with AddressSanitizer enabled
build-asan:
    cmake -B {{BUILD_ASAN}} -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DENABLE_ASAN=ON
    cmake --build {{BUILD_ASAN}}
    ln -sf {{BUILD_ASAN}}/compile_commands.json .

# Build optimized Release (no debug symbols, no sanitizers)
build-release:
    cmake -B {{BUILD_RELEASE}} -G Ninja -DCMAKE_BUILD_TYPE=Release
    cmake --build {{BUILD_RELEASE}}

# Quick recompile shortcuts
build-quick-debug:
    cmake --build {{BUILD_DEBUG}}

build-quick-asan:
    cmake --build {{BUILD_ASAN}}

build-quick-release:
    cmake --build {{BUILD_RELEASE}}

# Clean all build directories
clean:
    rm -rf build/
    rm -f compile_commands.json

rebuild-debug: clean build-quick-debug

rebuild-asan: clean build-quick-asan

rebuild-release: clean build-quick-release

# Format all source files
fmt:
    clang-format -i src/**/*.{c,h} include/**/*.h 2>/dev/null || true

# Run clang-tidy linter
lint:
    clang-tidy src/*.c -- -I./include -I./build/debug

# Run linter with fixes
lint-fix:
    clang-tidy src/*.c --fix -- -I./include -I./build/debug

# Generate compile_commands.json symlink
commands:
    ln -sf build/debug/compile_commands.json .

# Run the compiled executable (default: ASan)
run: run-asan

# Run with AddressSanitizer enabled build
run-asan: build-asan
    LSAN_OPTIONS=suppressions=asan_suppressions.txt nixGL ./build/asan/sweepntime

# Run with Valgrind memory checking (slow, for leak detection)
run-valgrind: build-debug
    nixGL valgrind --leak-check=full --track-origins=yes --suppressions=valgrind_suppressions.txt ./build/debug/sweepntime

# Run debug build (no sanitizers)
run-debug: build-debug
    nixGL ./build/debug/sweepntime

# Run release build (for performance testing or distribution)
run-release: build-release
    nixGL ./build/release/sweepntime

# Debug with lldb
debug: build-debug
    nixGL lldb ./build/debug/sweepntime

