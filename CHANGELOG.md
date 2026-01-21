# Changelog

## [Unreleased] - 2026-01-21

### Fixed
- **Working directory issue when launched from external launcher**: Files (config.ini, steam_appid.txt, log files) are now created in the game's directory instead of the external launcher's directory when the game is launched via an external launcher (e.g., Steam).

### Added
- **PathUtils utility class**: New utility class for resolving file paths relative to the executable's directory
  - `GetExecutableDirectory()`: Returns the directory where the executable is located
  - `ResolveRelativeToExecutable(path)`: Resolves a relative path to be relative to the executable directory
  - Static caching to avoid repeated system calls
  - Robust error handling with 32KB buffer to support long paths on modern Windows

### Changed
- **IniConfig**: Now resolves config.ini path relative to executable directory
- **Logger**: Now resolves log file paths relative to executable directory  
- **UCOnline/UCOnline64**: Now creates steam_appid.txt in executable directory

### Technical Details
- Uses Windows API `GetModuleFileNameA` to get the executable path
- All file operations (config, logs, steam_appid.txt) now work correctly regardless of the current working directory
- The fix ensures that when launched from any external launcher, all files are created alongside the executable

### Testing Instructions
1. Build the project using the existing CMake workflow
2. Place the executable in a test directory (e.g., `C:\Games\TestGame\`)
3. Create a launcher application in a different directory (e.g., `C:\Launcher\`)
4. Launch the uc-online executable from the external launcher
5. Verify that the following files are created in the executable's directory (`C:\Games\TestGame\`):
   - `config.ini`
   - `steam_appid.txt`
   - `uc_online.log` (if logging is enabled)
6. Verify that NO files are created in the launcher's directory (`C:\Launcher\`)
