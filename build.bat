@echo off
setlocal EnableDelayedExpansion

echo ==========================================
echo Building UC-Online DLLs (x86 and x64)
echo ==========================================
echo.

:: Check for Visual Studio installation
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo ERROR: Visual Studio not found. Please install Visual Studio with C++ support.
    exit /b 1
)

:: Detect latest Visual Studio version
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -property installationPath`) do (
    set "VS_PATH=%%i"
)

if not defined VS_PATH (
    echo ERROR: Could not find Visual Studio installation.
    exit /b 1
)

echo Found Visual Studio at: %VS_PATH%

:: Try to detect VS version for CMake generator
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -property displayName`) do (
    set "VS_NAME=%%i"
)
echo Visual Studio: %VS_NAME%

:: Determine CMake generator based on VS version
set "CMAKE_GEN="
echo %VS_NAME% | findstr "2022" >nul && set "CMAKE_GEN=Visual Studio 17 2022"
echo %VS_NAME% | findstr "2019" >nul && set "CMAKE_GEN=Visual Studio 16 2019"
echo %VS_NAME% | findstr "2017" >nul && set "CMAKE_GEN=Visual Studio 15 2017"

if not defined CMAKE_GEN (
    echo WARNING: Unknown VS version, trying Visual Studio 17 2022
    set "CMAKE_GEN=Visual Studio 17 2022"
)

echo Using CMake generator: %CMAKE_GEN%
echo.

:: Clean previous builds if they exist
if exist build-x86 (
    echo Cleaning previous x86 build...
    rmdir /s /q build-x86
)
if exist build-x64 (
    echo Cleaning previous x64 build...
    rmdir /s /q build-x64
)

:: Build x86 version
echo.
echo ==========================================
echo Building 32-bit (x86) version...
echo ==========================================
mkdir build-x86
cd build-x86

echo Running CMake for x86...
cmake -G "%CMAKE_GEN%" -A Win32 ..
if errorlevel 1 (
    echo ERROR: CMake configuration failed for x86
    cd ..
    exit /b 1
)

echo Building x86 Release...
cmake --build . --config Release
if errorlevel 1 (
    echo ERROR: Build failed for x86
    cd ..
    exit /b 1
)

cd ..
echo x86 build completed successfully!

:: Build x64 version
echo.
echo ==========================================
echo Building 64-bit (x64) version...
echo ==========================================
mkdir build-x64
cd build-x64

echo Running CMake for x64...
cmake -G "%CMAKE_GEN%" -A x64 ..
if errorlevel 1 (
    echo ERROR: CMake configuration failed for x64
    cd ..
    exit /b 1
)

echo Building x64 Release...
cmake --build . --config Release
if errorlevel 1 (
    echo ERROR: Build failed for x64
    cd ..
    exit /b 1
)

cd ..
echo x64 build completed successfully!

:: Create output directory and copy files
echo.
echo ==========================================
echo Copying built files to output directory...
echo ==========================================

if not exist output mkdir output
if not exist output\x86 mkdir output\x86
if not exist output\x64 mkdir output\x64
if not exist output\winmm-x86 mkdir output\winmm-x86
if not exist output\winmm-x64 mkdir output\winmm-x64

:: Copy x86 files
copy /y build-x86\Release\uc-online.dll output\x86\
copy /y build-x86\Release\uc-online.lib output\x86\
copy /y build-x86\Release\uc-online.exp output\x86\

:: Copy x64 files
copy /y build-x64\Release\uc-online64.dll output\x64\
copy /y build-x64\Release\uc-online64.lib output\x64\
copy /y build-x64\Release\uc-online64.exp output\x64\

:: Create winmm proxy versions
copy /y output\x86\uc-online.dll output\winmm-x86\winmm.dll
copy /y output\x64\uc-online64.dll output\winmm-x64\winmm.dll

echo.
echo ==========================================
echo Build Summary
echo ==========================================
echo.
echo x86 (32-bit) files in: output\x86\
echo   - uc-online.dll (original name)
echo.
echo x64 (64-bit) files in: output\x64\
echo   - uc-online64.dll (original name)
echo.
echo winmm proxy files:
echo   - output\winmm-x86\winmm.dll (for 32-bit games)
echo   - output\winmm-x64\winmm.dll (for 64-bit games)
echo.
echo ==========================================
echo Build completed successfully!
echo ==========================================

endlocal