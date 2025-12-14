@echo off
echo Building uc-online for both x86 and x64 architectures...

echo Building x86 library...
dotnet build uc-online.csproj -c Release

echo Building x64 library...
dotnet build uc-online64.csproj -c Release

echo Building x86 example...
dotnet build uc-onlineLauncher.csproj -c Release

echo Building x64 example...
dotnet build uc-onlineLauncher64.csproj -c Release

echo.
echo Build complete!
echo.
echo Libraries:
echo x86 version: uc\bin\Release\x86\net8.0\uc-online.dll
echo x64 version: uc\bin\Release\x64\net8.0\uc-online64.dll
echo.
echo Examples:
echo x86 version: uc\bin\Release\x86\net8.0\uc-onlineLauncher.exe
echo x64 version: uc\bin\Release\x64\net8.0\uc-onlineLauncher64.exe

pause