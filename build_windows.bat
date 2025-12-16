@echo off
REM Windows batch file for building uc-online crossplatform
REM This script can build for Windows, Linux, and macOS from Windows

setlocal enabledelayedexpansion

echo Building uc-online crossplatform for multiple platforms from Windows...
echo ===================================================================

REM Configuration
set CONFIGURATION=Release
set FRAMEWORK=net8.0

echo Available platforms:
echo   win-x64       - Windows 64-bit
echo   win-x86       - Windows 32-bit  
echo   linux-x64     - Linux 64-bit (cross-compile)
echo   osx-x64       - macOS 64-bit (cross-compile)
echo.

REM Parse command line arguments
set PLATFORMS=
if "%~1"=="" (
    REM Default: build for Windows x64
    set PLATFORMS=win-x64
    echo Building for default platform: win-x64
) else (
    REM Use provided platforms
    set PLATFORMS=%*
    echo Building for platforms: !PLATFORMS!
)

echo.

REM Function to build for a specific platform
setlocal enabledelayedexpansion
for %%p in (!PLATFORMS!) do (
    call :build_platform %%p
)
endlocal

echo.
echo ===================================================================
echo 🎉 Build completed successfully!
echo.
echo Output directories:
for %%p in (!PLATFORMS!) do (
    echo   %%p: bin\!CONFIGURATION!\%%p\
)

echo.
echo Library files:
for %%p in (!PLATFORMS!) do (
    echo   %%p: bin\!CONFIGURATION!\%%p\uc_online_crossplatform.dll
)

echo.
echo Executable files:
for %%p in (!PLATFORMS!) do (
    if "%%p"=="win-x64" (
        echo   %%p: bin\!CONFIGURATION!\%%p\uc_onlineLauncher_crossplatform.exe
    ) else if "%%p"=="win-x86" (
        echo   %%p: bin\!CONFIGURATION!\%%p\uc_onlineLauncher_crossplatform.exe
    ) else (
        echo   %%p: bin\!CONFIGURATION!\%%p\uc_onlineLauncher_crossplatform
    )
)

echo.
echo To deploy to Linux:
echo   1. Copy bin\!CONFIGURATION!\linux-x64\ folder to your Linux system
echo   2. On Linux: chmod +x bin\!CONFIGURATION!\linux-x64\uc_onlineLauncher_crossplatform
echo   3. Run: ./bin\!CONFIGURATION!\linux-x64/uc_onlineLauncher_crossplatform

goto :eof

:build_platform
setlocal
set RUNTIME_ID=%1

echo Building for %RUNTIME_ID%...
set OUTPUT_DIR=bin\%CONFIGURATION%\%RUNTIME_ID%

REM Clean previous build
if exist "%OUTPUT_DIR%" (
    echo Cleaning previous build for %RUNTIME_ID%...
    rmdir /s /q "%OUTPUT_DIR%" 2>nul || del /q "%OUTPUT_DIR%\*" 2>nul
)

REM Validate runtime ID
echo Checking platform: %RUNTIME_ID%
set VALID_PLATFORMS=win-x64 win-x86 linux-x64 osx-x64
echo !VALID_PLATFORMS! | findstr /i "%RUNTIME_ID%" >nul
if errorlevel 1 (
    echo ⚠ Unknown platform: %RUNTIME_ID%
    echo Valid platforms: win-x64, win-x86, linux-x64, osx-x64
    goto :eof
)

REM Build the library
echo Building library for %RUNTIME_ID%...
dotnet build uc_online_crossplatform.csproj -c %CONFIGURATION% --runtime %RUNTIME_ID% --framework %FRAMEWORK% -o "%OUTPUT_DIR%"
if errorlevel 1 (
    echo ❌ Library build failed for %RUNTIME_ID%
    goto :eof
)

REM Build the launcher
echo Building launcher for %RUNTIME_ID%...
dotnet build uc_onlineLauncher_crossplatform.csproj -c %CONFIGURATION% --runtime %RUNTIME_ID% --framework %FRAMEWORK% -o "%OUTPUT_DIR%"
if errorlevel 1 (
    echo ❌ Launcher build failed for %RUNTIME_ID%
    goto :eof
)

echo ✅ Built successfully for %RUNTIME_ID%
echo.
endlocal
goto :eof

:eof