using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace uc_online
{
    public class uc_online_crossplatform : IDisposable
    {
        private bool _steamInitialized = false;
        private uint _currentAppID = 480;
        private IniConfig_crossplatform _config;
        private Logger _logger;
        private string _gameExecutable = "";
        private string _gameArguments = "";
        private string _steamApiDllPath = "";
        private PlatformType _currentPlatform;

        public enum PlatformType
        {
            Windows,
            Linux,
            MacOS
        }

        public uc_online_crossplatform(string iniFilePath = "config.ini")
        {
            _config = new IniConfig_crossplatform(iniFilePath);
            _currentAppID = _config.GetAppID();
            _gameExecutable = _config.GetGameExecutable();
            _gameArguments = _config.GetGameArguments();
            _steamApiDllPath = _config.GetSteamApiDllPath();

            // Detect current platform
            _currentPlatform = DetectPlatform();

            string logFile = _config.GetValue("Logging", "LogFile", "uc_online.log");
            bool enableLogging = bool.TryParse(_config.GetValue("Logging", "EnableLogging", "true"), out bool loggingEnabled) && loggingEnabled;
            _logger = new Logger(logFile, enableLogging);

            _logger.Log($"uc-online crossplatform initialized with AppID: {_currentAppID}");
            _logger.Log($"Platform: {_currentPlatform}");
            _logger.Log($"Game executable: {(string.IsNullOrEmpty(_gameExecutable) ? "not configured" : _gameExecutable)}");
            _logger.Log($"Steam API path: {(string.IsNullOrEmpty(_steamApiDllPath) ? "default loading" : _steamApiDllPath)}");
        }

        private PlatformType DetectPlatform()
        {
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
                return PlatformType.Windows;
            else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
                return PlatformType.Linux;
            else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
                return PlatformType.MacOS;
            else
                throw new NotSupportedException($"Platform {RuntimeInformation.OSDescription} is not supported");
        }

        private string GetSteamApiLibraryName()
        {
            switch (_currentPlatform)
            {
                case PlatformType.Windows:
                    return Environment.Is64BitProcess ? "steam_api64.dll" : "steam_api.dll";
                case PlatformType.Linux:
                    return "libsteam_api.so";
                case PlatformType.MacOS:
                    return "libsteam_api.dylib";
                default:
                    throw new NotSupportedException($"Platform {_currentPlatform} is not supported");
            }
        }

        private string GetSteamApiFunctionName(string functionName)
        {
            // On Linux/macOS, we might need to add platform-specific prefixes/suffixes
            // Steam API functions are typically the same across platforms
            return functionName;
        }

        // Platform-specific DllImport declarations
        [DllImport("kernel32", SetLastError = true, CharSet = CharSet.Unicode)]
        private static extern IntPtr LoadLibraryWindows(string lpFileName);

        [DllImport("libdl.so.2")]
        private static extern IntPtr LoadLibraryLinux(string lpFileName);

        [DllImport("steam_api", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool SteamAPI_Init(out StringBuilder errorMessage);

        [DllImport("steam_api", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool SteamAPI_InitFlat(out StringBuilder errorMessage);

        [DllImport("steam_api", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SteamAPI_Shutdown();

        [DllImport("steam_api", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SteamAPI_RunCallbacks();

        [DllImport("steam_api", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool SteamAPI_RestartAppIfNecessary(uint appId);

        [DllImport("steam_api", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr SteamClient();

        [DllImport("steam_api", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr SteamApps();

        public bool Initializeuc_online()
        {
            try
            {
                _logger.Log("Initializing Steam with AppID: " + _currentAppID);

                CreateAppIdFile();

                LoadSteamApiDll();

                bool result = TryMultipleInitializationMethods();

                if (!result)
                {
                    return false;
                }

                _steamInitialized = true;
                _logger.Log("Steam initialized successfully");

                if (SteamClient() != IntPtr.Zero && SteamApps() != IntPtr.Zero)
                {
                    _logger.Log("Steam interfaces accessible");
                }
                else
                {
                    _logger.LogWarning("Steam interfaces not accessible");
                }

                return true;
            }
            catch (Exception ex)
            {
                _logger.LogException(ex, "Exception during Steam initialization");
                Console.WriteLine($"Exception during Steam initialization: {ex.Message}");
                return false;
            }
        }

        private bool TryMultipleInitializationMethods()
        {
            StringBuilder errorMsg = new StringBuilder(1024);
            bool result = SteamAPI_Init(out errorMsg);

            if (result)
            {
                _logger.Log("SteamAPI_Init succeeded");
                return true;
            }
            else
            {
                _logger.Log($"SteamAPI_Init failed: {errorMsg}");

                // Try SteamAPI_InitFlat if available (mainly for Windows)
                try
                {
                    StringBuilder errorMsgFlat = new StringBuilder(1024);
                    bool resultFlat = SteamAPI_InitFlat(out errorMsgFlat);

                    if (resultFlat)
                    {
                        _logger.Log("SteamAPI_InitFlat succeeded");
                        return true;
                    }
                    else
                    {
                        _logger.Log($"SteamAPI_InitFlat failed: {errorMsgFlat}");
                    }
                }
                catch (EntryPointNotFoundException)
                {
                    _logger.Log("SteamAPI_InitFlat not available on this platform");
                }

                if (SteamAPI_RestartAppIfNecessary(_currentAppID))
                {
                    _logger.Log("Steam requested app restart");
                    return false;
                }

                return false;
            }
        }

        public void Shutdownuc_online()
        {
            if (_steamInitialized)
            {
                _logger.Log("Shutting down Steam");
                SteamAPI_Shutdown();
                _steamInitialized = false;
                _logger.Log("Steam shutdown complete");
            }
        }

        public void RunSteamCallbacks()
        {
            if (_steamInitialized)
            {
                SteamAPI_RunCallbacks();
            }
        }

        public void SetCustomAppID(uint appID)
        {
            _currentAppID = appID;

            if (_steamInitialized)
            {
                Shutdownuc_online();
                Initializeuc_online();
            }
        }

        public uint GetCurrentAppID()
        {
            return _currentAppID;
        }

        public bool IsSteamInitialized()
        {
            return _steamInitialized;
        }

        private void CreateAppIdFile()
        {
            try
            {
                using (StreamWriter writer = new StreamWriter("steam_appid.txt", false))
                {
                    writer.Write(_currentAppID.ToString());
                }
                _logger.Log("Created steam_appid.txt file");
            }
            catch (Exception ex)
            {
                _logger.LogException(ex, "Failed to create steam_appid.txt");
                Console.WriteLine($"Failed to create steam_appid.txt: {ex.Message}");
            }
        }

        private void LoadSteamApiDll()
        {
            try
            {
                string libraryName = GetSteamApiLibraryName();
                _logger.Log($"Looking for Steam API library: {libraryName}");

                if (string.IsNullOrEmpty(_steamApiDllPath))
                {
                    _logger.Log("No custom Steam API DLL path configured, using default loading");
                    return;
                }

                string dllPath = Path.Combine(_steamApiDllPath, libraryName);
                if (File.Exists(dllPath))
                {
                    _logger.Log($"Found {libraryName} at: {dllPath}");
                    IntPtr handle = LoadLibraryPlatformSpecific(dllPath);
                    if (handle != IntPtr.Zero)
                    {
                        _logger.Log($"Successfully loaded {libraryName} from custom path");
                        return;
                    }
                    else
                    {
                        _logger.LogWarning($"Failed to load {libraryName} from custom path, falling back to default loading");
                    }
                }
                else
                {
                    // Try platform-specific subdirectories
                    string platformSubdir = GetPlatformSubdirectory();
                    if (!string.IsNullOrEmpty(platformSubdir))
                    {
                        string platformPath = Path.Combine(_steamApiDllPath, platformSubdir, libraryName);
                        if (File.Exists(platformPath))
                        {
                            _logger.Log($"Found {libraryName} at: {platformPath}");
                            IntPtr handle = LoadLibraryPlatformSpecific(platformPath);
                            if (handle != IntPtr.Zero)
                            {
                                _logger.Log($"Successfully loaded {libraryName} from {platformSubdir} subdirectory");
                                return;
                            }
                        }
                    }

                    _logger.LogWarning($"{libraryName} not found at configured path, using default loading");
                }
            }
            catch (Exception ex)
            {
                _logger.LogException(ex, "Error loading Steam API DLL");
                Console.WriteLine($"Error loading Steam API DLL: {ex.Message}");
            }
        }

        private string GetPlatformSubdirectory()
        {
            switch (_currentPlatform)
            {
                case PlatformType.Windows:
                    return Environment.Is64BitProcess ? "win64" : "win32";
                case PlatformType.Linux:
                    return "linux64";
                case PlatformType.MacOS:
                    return "macos";
                default:
                    return null;
            }
        }

        private IntPtr LoadLibraryPlatformSpecific(string libraryPath)
        {
            try
            {
                switch (_currentPlatform)
                {
                    case PlatformType.Windows:
                        return LoadLibraryWindows(libraryPath);
                    case PlatformType.Linux:
                        return LoadLibraryLinux(libraryPath);
                    case PlatformType.MacOS:
                        // For macOS, you would implement dlopen here
                        throw new NotImplementedException("macOS library loading not implemented");
                    default:
                        throw new NotSupportedException($"Platform {_currentPlatform} not supported");
                }
            }
            catch (Exception ex)
            {
                _logger.LogException(ex, $"Failed to load library: {libraryPath}");
                return IntPtr.Zero;
            }
        }

        private string GetDefaultSteamPath()
        {
            switch (_currentPlatform)
            {
                case PlatformType.Windows:
                    return Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86), "Steam");
                case PlatformType.Linux:
                    // Common Linux Steam installation paths
                    string userHome = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);
                    return Path.Combine(userHome, ".local/share/Steam");
                case PlatformType.MacOS:
                    return Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.UserProfile), "Library", "Application Support", "Steam");
                default:
                    return null;
            }
        }

        public bool LaunchGame()
        {
            _logger.Log($"Attempting to launch game: {_gameExecutable}");

            if (string.IsNullOrWhiteSpace(_gameExecutable))
            {
                _logger.LogError("No game executable configured in INI file.");
                Console.WriteLine("No game executable configured in INI file.");
                return false;
            }

            if (!File.Exists(_gameExecutable))
            {
                _logger.LogError($"Game executable not found: {_gameExecutable}");
                Console.WriteLine($"Game executable not found: {_gameExecutable}");
                return false;
            }

            try
            {
                _logger.Log($"Launching game: {_gameExecutable} {_gameArguments}");
                Console.WriteLine($"Launching game: {_gameExecutable} {_gameArguments}");

                var processInfo = new ProcessStartInfo
                {
                    FileName = _gameExecutable,
                    Arguments = _gameArguments,
                    WorkingDirectory = Path.GetDirectoryName(_gameExecutable),
                    UseShellExecute = false
                };

                // On Linux, we might need to make the file executable
                if (_currentPlatform == PlatformType.Linux && !_gameExecutable.EndsWith(".sh"))
                {
                    try
                    {
                        var chmodProcess = new ProcessStartInfo
                        {
                            FileName = "chmod",
                            Arguments = $"+x \"{_gameExecutable}\"",
                            UseShellExecute = false
                        };
                        Process.Start(chmodProcess);
                    }
                    catch (Exception ex)
                    {
                        _logger.LogWarning($"Could not set executable permissions: {ex.Message}");
                    }
                }

                Process gameProcess = Process.Start(processInfo);
                if (gameProcess != null)
                {
                    _logger.Log($"Game launched successfully (PID: {gameProcess.Id})");
                    Console.WriteLine($"Game launched successfully (PID: {gameProcess.Id})");
                    return true;
                }
                else
                {
                    _logger.LogError("Failed to launch game process");
                    Console.WriteLine("Failed to launch game process");
                    return false;
                }
            }
            catch (Exception ex)
            {
                _logger.LogException(ex, "Game launch failed");
                Console.WriteLine($"Error launching game: {ex.Message}");
                return false;
            }
        }

        public void SetGameExecutable(string gameExePath)
        {
            _gameExecutable = gameExePath;
            _config.SetGameExecutable(gameExePath);
            _config.SaveConfig();
        }

        public void SetGameArguments(string arguments)
        {
            _gameArguments = arguments;
            _config.SetGameArguments(arguments);
            _config.SaveConfig();
        }

        public string GetGameExecutable()
        {
            return _gameExecutable;
        }

        public string GetGameArguments()
        {
            return _gameArguments;
        }

        public void SaveConfig()
        {
            _config.SetAppID(_currentAppID);
            _config.SetGameExecutable(_gameExecutable);
            _config.SetGameArguments(_gameArguments);
            _config.SaveConfig();
        }

        public void ReloadConfig()
        {
            _config.LoadConfig();
            _currentAppID = _config.GetAppID();
            _gameExecutable = _config.GetGameExecutable();
            _gameArguments = _config.GetGameArguments();
        }

        public Logger GetLogger()
        {
            return _logger;
        }

        public void SetLoggingEnabled(bool enabled)
        {
            _logger.SetLoggingEnabled(enabled);
            _logger.Log($"Logging {(enabled ? "enabled" : "disabled")}");
        }

        public bool IsLoggingEnabled()
        {
            return _logger.IsLoggingEnabled();
        }

        public void ClearLog()
        {
            _logger.ClearLog();
        }

        public string GetSteamApiDllPath()
        {
            return _steamApiDllPath;
        }

        public void SetSteamApiDllPath(string dllPath)
        {
            _steamApiDllPath = dllPath;
            _config.SetSteamApiDllPath(dllPath);
            _config.SaveConfig();
        }

        public PlatformType GetCurrentPlatform()
        {
            return _currentPlatform;
        }

        public string GetDefaultSteamInstallationPath()
        {
            return GetDefaultSteamPath();
        }

        public void Dispose()
        {
            _logger.Log("uc-online crossplatform shutting down");
            Shutdownuc_online();
        }
    }
}