#!/bin/bash

# Test script to validate cross-platform build
# This script tests compilation without requiring Steam libraries

set -e

echo "Testing cross-platform build configuration..."
echo "============================================="

# Test building the library
echo "Testing library build..."
dotnet build uc_online_crossplatform.csproj -c Release --no-restore

if [ $? -eq 0 ]; then
    echo "✅ Library build successful"
else
    echo "❌ Library build failed"
    exit 1
fi

# Test building the launcher
echo "Testing launcher build..."
dotnet build uc_onlineLauncher_crossplatform.csproj -c Release --no-restore

if [ $? -eq 0 ]; then
    echo "✅ Launcher build successful"
else
    echo "❌ Launcher build failed"
    exit 1
fi

# Test building for specific runtime
echo "Testing Linux-specific build..."
dotnet build uc_online_crossplatform.csproj -c Release --runtime linux-x64 --no-restore

if [ $? -eq 0 ]; then
    echo "✅ Linux build successful"
else
    echo "❌ Linux build failed"
    exit 1
fi

echo "============================================="
echo "🎉 All build tests passed!"
echo ""
echo "The cross-platform implementation is ready for deployment."
echo ""
echo "Next steps:"
echo "1. Transfer files to target platform"
echo "2. Run './build.sh' to build for specific platforms"
echo "3. Configure Steam API library paths in config.ini"
echo "4. Run the launcher to test Steam integration"