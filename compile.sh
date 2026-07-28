#!/usr/bin/env bash

# - - - Exit immediately if a command exits with a non-zero status
set -e

# - - - Define directories
BUILD_DIR="build"
ROOT_DIR=$(pwd)

# - - - Mode 2: Clean and Rebuild
if [ "$1" == "clean" ]; then
    echo "Cleaning previous build artifacts..."
    rm -rf "$BUILD_DIR"
fi

# Ensure the build directory exists
mkdir -p "$BUILD_DIR"

# Navigate to build directory
cd "$BUILD_DIR"

# Run CMake and compile
echo "Running CMake..."
cmake ..

echo "Compiling project..."
make -j$(nproc)

# Handle the compile commands for Neovim
if [ -f "compile_commands.json" ]; then
    echo "Updating compile_commands.json for Neovim..."
    # Copy instead of symlink so it persists safely
    cp compile_commands.json "$ROOT_DIR/" 
fi

# Return to root directory
cd "$ROOT_DIR"
echo "Build completed successfully!"
