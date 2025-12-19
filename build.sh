#!/bin/bash

# Linux/macOS build script for uc-online crossplatform
# This script builds the project for multiple platforms

set -e  # Exit on any error

echo "Building uc-online crossplatform for multiple platforms..."
echo "=========================================================="

# Configuration
CONFIGURATION="Release"
FRAMEWORK="net8.0"

# Function to build for a specific platform
build_platform() {
    local runtime_id=$1
    local output_dir="bin/${CONFIGURATION}/${runtime_id}"
    
    echo "Building for ${runtime_id}..."
    echo "Output directory: ${output_dir}"
    
    # Clean previous build
    rm -rf "${output_dir}"
    
    # Build for specific runtime
    dotnet build uc_online_crossplatform.csproj \
        -c ${CONFIGURATION} \
        --runtime ${runtime_id} \
        --framework ${FRAMEWORK} \
        -o "${output_dir}"
    
    echo "✓ Built successfully for ${runtime_id}"
    echo ""
}

# Function to build launcher
build_launcher() {
    local runtime_id=$1
    local output_dir="bin/${CONFIGURATION}/${runtime_id}"
    local launcher_name="uc_onlineLauncher_crossplatform"
    
    echo "Building launcher for ${runtime_id}..."
    
    # Build launcher with dependency on crossplatform library
    dotnet build uc_onlineLauncher_crossplatform.csproj \
        -c ${CONFIGURATION} \
        --runtime ${runtime_id} \
        --framework ${FRAMEWORK} \
        -o "${output_dir}"
    
    echo "✓ Launcher built successfully for ${runtime_id}"
    echo ""
}

echo "Available platforms:"
echo "  linux-x64    - Linux 64-bit"
echo "  win-x64      - Windows 64-bit"
echo "  win-x86      - Windows 32-bit"
echo "  osx-x64      - macOS 64-bit"
echo ""

# Parse command line arguments
PLATFORMS=()

if [ $# -eq 0 ]; then
    # Default: build for current platform
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        PLATFORMS=("linux-x64")
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        PLATFORMS=("osx-x64")
    else
        echo "Unknown OS. Please specify platforms manually."
        echo "Usage: $0 [platform1] [platform2] ..."
        echo "Example: $0 linux-x64 win-x64"
        exit 1
    fi
else
    PLATFORMS=("$@")
fi

echo "Building for platforms: ${PLATFORMS[@]}"
echo ""

# Build the library for each platform
for platform in "${PLATFORMS[@]}"; do
    case $platform in
        "linux-x64"|"win-x64"|"win-x86"|"osx-x64")
            build_platform $platform
            ;;
        *)
            echo "⚠ Unknown platform: $platform"
            echo "Valid platforms: linux-x64, win-x64, win-x86, osx-x64"
            exit 1
            ;;
    esac
done

# Build launchers for each platform
for platform in "${PLATFORMS[@]}"; do
    case $platform in
        "linux-x64"|"win-x64"|"win-x86"|"osx-x64")
            build_launcher $platform
            ;;
    esac
done

echo "=========================================================="
echo "🎉 Build completed successfully!"
echo ""
echo "Output directories:"
for platform in "${PLATFORMS[@]}"; do
    echo "  ${platform}: bin/${CONFIGURATION}/${platform}/"
done

echo ""
echo "Library files:"
for platform in "${PLATFORMS[@]}"; do
    echo "  ${platform}: bin/${CONFIGURATION}/${platform}/uc_online_crossplatform.dll"
done

echo ""
echo "Executable files:"
for platform in "${PLATFORMS[@]}"; do
    case $platform in
        "linux-x64"|"osx-x64")
            echo "  ${platform}: bin/${CONFIGURATION}/${platform}/uc_onlineLauncher_crossplatform"
            ;;
        "win-x64"|"win-x86")
            echo "  ${platform}: bin/${CONFIGURATION}/${platform}/uc_onlineLauncher_crossplatform.exe"
            ;;
    esac
done

echo ""
echo "To run the application:"
echo "  Linux/macOS: ./bin/${CONFIGURATION}/linux-x64/uc_onlineLauncher_crossplatform"
echo "  Windows:     ./bin/${CONFIGURATION}/win-x64/uc_onlineLauncher_crossplatform.exe"