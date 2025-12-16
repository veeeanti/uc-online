# uc-online Crossplatform

A cross-platform C# Steam API wrapper that works on Windows, Linux, and macOS.

## 🚀 Quick Start (Cross-Compile from Windows)

Build Linux binaries directly from Windows in seconds:

```cmd
REM Build Linux version from Windows
build_windows.bat linux-x64

REM The Linux binaries are ready in: bin\Release\linux-x64\
REM Transfer to Linux and run with: chmod +x uc_onlineLauncher_crossplatform
```

## Features

- **Cross-platform Steam API integration** - Works on Windows, Linux, and macOS
- **Cross-compilation from Windows** - Build Linux/macOS binaries directly from Windows
- **Automatic platform detection** - Detects and adapts to the current operating system
- **Platform-specific library loading** - Handles different Steam API library names and paths
- **Flexible configuration** - Supports platform-specific configuration options
- **Game launching** - Launch games with Steam integration across all platforms

## Platform Support

| Platform | Status | Steam API Library | Executable Extension |
|----------|--------|-------------------|---------------------|
| Windows 64-bit | ✅ Supported | `steam_api64.dll` | `.exe` |
| Windows 32-bit | ✅ Supported | `steam_api.dll` | `.exe` |
| Linux 64-bit | ✅ Supported | `libsteam_api.so` | (no extension) |
| macOS 64-bit | ✅ Supported | `libsteam_api.dylib` | (no extension) |

## Quick Start

### Building

#### Cross-Compilation from Windows
```cmd
# Build for Windows only (default)
build_windows.bat

# Build for Linux only (cross-compile)
build_windows.bat linux-x64

# Build for multiple platforms at once
build_windows.bat win-x64 linux-x64 osx-x64

# Build for all supported platforms
build_windows.bat win-x64 win-x86 linux-x64 osx-x64
```

#### Building on Linux/macOS
```bash
# Make the build script executable
chmod +x build.sh

# Build for current platform
./build.sh

# Build for specific platforms
./build.sh linux-x64
./build.sh win-x64
./build.sh osx-x64

# Build for multiple platforms
./build.sh linux-x64 win-x64
```

#### Windows Native Building
```cmd
# Using dotnet directly
dotnet build uc_online_crossplatform.csproj -c Release
dotnet build uc_onlineLauncher_crossplatform.csproj -c Release
```

### Running

#### Linux/macOS
```bash
# Run the launcher
./bin/Release/linux-x64/uc_onlineLauncher_crossplatform
```

#### Windows
```cmd
# Run the launcher
bin\Release\win-x64\uc_onlineLauncher_crossplatform.exe
```

## Configuration

The application uses `config.ini` for configuration. On first run, it creates a default configuration file with platform-specific examples.

### Platform-Specific Configuration

#### Linux Configuration Example
```ini
[uc-online]
AppID = 480
GameExecutable = /home/user/.local/share/Steam/steamapps/common/GameName/game
GameArguments = 
SteamApiDLLPath = ~/.local/share/Steam/steamapps/common/GameName/

[Platform]
CurrentPlatform = Linux
AutoDetectSteamPath = true
DefaultSteamPath = ~/.local/share/Steam

[GamePaths]
CustomGamePath1 = ~/.local/share/Steam/steamapps/common
CustomGamePath2 = ~/Games/steam
```

#### Windows Configuration Example
```ini
[uc-online]
AppID = 480
GameExecutable = C:\Program Files (x86)\Steam\steamapps\common\GameName\game.exe
GameArguments = 
SteamApiDLLPath = C:\Program Files (x86)\Steam

[Platform]
CurrentPlatform = Windows
AutoDetectSteamPath = true
DefaultSteamPath = C:\Program Files (x86)\Steam
```

## Steam API Library Locations

### Linux
- **Default Steam Path**: `~/.local/share/Steam`
- **SDK Path**: `~/.steam/sdk/`
- **Game-specific**: `~/.local/share/Steam/steamapps/common/GameName/`

### Windows
- **Default Steam Path**: `C:\Program Files (x86)\Steam`
- **Game-specific**: `C:\Program Files (x86)\Steam\steamapps\common\GameName\`

### macOS
- **Default Steam Path**: `~/Library/Application Support/Steam`
- **Game-specific**: `~/Library/Application Support/Steam/steamapps/common/GameName/`

## Development

### Project Structure
```
├── uc_online_crossplatform.cs          # Main cross-platform Steam API wrapper
├── IniConfig_crossplatform.cs          # Cross-platform configuration management
├── Program_crossplatform.cs            # Cross-platform launcher
├── uc_online_crossplatform.csproj      # Library project file
├── uc_onlineLauncher_crossplatform.csproj  # Launcher project file
├── build.sh                            # Cross-platform build script
├── build.bat                           # Windows build script
└── README_crossplatform.md            # This file
```

### Key Classes

#### `uc_online_crossplatform`
Main Steam API wrapper class with:
- Automatic platform detection
- Cross-platform library loading
- Steam API initialization and management
- Game launching functionality

#### `IniConfig_crossplatform`
Enhanced configuration management with:
- Platform-specific default values
- Custom game path management
- Environment variable expansion
- Path normalization

### Building for Multiple Platforms

```bash
# Build for all supported platforms
./build.sh linux-x64 win-x64 win-x86 osx-x64

# The output will be in bin/Release/{platform}/
bin/Release/linux-x64/
├── uc_online_crossplatform.dll
├── uc_onlineLauncher_crossplatform
└── config.ini (auto-generated)

bin/Release/win-x64/
├── uc_online_crossplatform.dll
├── uc_onlineLauncher_crossplatform.exe
└── config.ini (auto-generated)
```

## Troubleshooting

### Linux-specific Issues

1. **Steam API library not found**
   - Ensure Steam is installed
   - Check library path in configuration
   - Verify file permissions

2. **Game executable not launching**
   - Make sure the game executable has execute permissions: `chmod +x game`
   - Check that the path is correct in config.ini

3. **Permission denied errors**
   - Run with appropriate permissions
   - Check that Steam installation is accessible

### Common Issues

1. **Steam not running**
   - Make sure Steam is installed and running
   - The application requires Steam to be active

2. **AppID not found**
   - Verify the AppID in config.ini is correct
   - Ensure steam_appid.txt can be created in the application directory

3. **Library loading failures**
   - Check Steam API library paths in configuration
   - Verify the library exists and is accessible

## API Usage Example

```csharp
using uc_online;

// Create instance
using (var steamWrapper = new uc_online_crossplatform())
{
    // Initialize Steam
    if (steamWrapper.Initializeuc_online())
    {
        Console.WriteLine($"Platform: {steamWrapper.GetCurrentPlatform()}");
        Console.WriteLine($"Steam Path: {steamWrapper.GetDefaultSteamInstallationPath()}");
        
        // Set custom AppID
        steamWrapper.SetCustomAppID(480);
        
        // Launch game if configured
        if (!string.IsNullOrEmpty(steamWrapper.GetGameExecutable()))
        {
            steamWrapper.LaunchGame();
        }
        
        // Run Steam callbacks
        for (int i = 0; i < 10; i++)
        {
            steamWrapper.RunSteamCallbacks();
            Thread.Sleep(1000);
        }
    }
}
```

## License

This project maintains the same license as the original uc-online project. Please see the original project's license terms.

## Contributing

When contributing to this cross-platform version:

1. Test changes on multiple platforms when possible
2. Ensure platform-specific code is properly abstracted
3. Update configuration examples for new platforms
4. Maintain backward compatibility with the original API

## Platform Differences

### Library Loading
- **Windows**: Uses `LoadLibrary` from `kernel32.dll`
- **Linux**: Uses `dlopen` from `libdl.so.2`
- **macOS**: Uses dynamic loading (not fully implemented in current version)

### Path Handling
- **Windows**: Uses backslashes, environment variables with `%VAR%`
- **Linux/macOS**: Uses forward slashes, environment variables with `$VAR` or `${VAR}`
- **Home Directory**: Windows uses `%USERPROFILE%`, Unix-like systems use `~`

### Executable Extensions
- **Windows**: `.exe` files
- **Linux/macOS**: No extension required, file mode determines executability

This cross-platform implementation provides a unified API while handling platform-specific differences transparently.