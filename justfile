#!/usr/bin/env just --justfile

set shell := ["bash", "-u", "-c"]

# Default recipe
default:
    @just --list

# Build with Debug symbols
build-debug:
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cmake --build build
    ln -sf build/compile_commands.json .

# Build with Debug symbols and AddressSanitizer enabled
build-asan:
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DENABLE_ASAN=ON
    cmake --build build
    ln -sf build/compile_commands.json .

# Build optimized Release
build-release:
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
    cmake --build build

# Quick recompile (assumes cmake already configured)
build:
    cmake --build build

# Clean build directory
clean:
    rm -rf build/
    rm -f compile_commands.json

# Full clean rebuild
rebuild: clean build-debug

# Format all source files
fmt:
    clang-format -i src/**/*.{c,h} include/**/*.h 2>/dev/null || true

# Run clang-tidy linter
lint:
    clang-tidy src/*.c -- -I./include -I./build

# Run linter with fixes
lint-fix:
    clang-tidy src/*.c --fix -- -I./include -I./build

# Generate compile_commands.json symlink
commands:
    ln -sf build/compile_commands.json .

# Run the compiled executable
run: build
    nixGL ./build/sweepntime

# Run with AddressSanitizer enabled build
run-asan: build-asan
    nixGL ./build/sweepntime

# Run with arguments
run-args *ARGS: build
    nixGL ./build/sweepntime {{ARGS}}

# Debug with lldb
debug: build-debug
    nixGL lldb ./build/sweepntime

# Set up dev environment (first time setup)
setup:
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    ln -sf build/compile_commands.json .
    echo "✓ Development environment ready!"
    echo "  Run 'just build' to compile"
    echo "  Run 'just run' to execute"
    echo "  Run 'just debug' to debug"