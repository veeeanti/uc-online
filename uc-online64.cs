using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace uc_online
{
    public class uc_online64 : IDisposable
    {
        private bool _steamInitialized = false;
        private uint _currentAppID = 480;
        private IniConfig _config;
        private Logger _logger;
        private string _gameExecutable = "";
        private string _gameArguments = "";
        private string _steamApiDllPath = "";

        public uc_online64(string iniFilePath = "config.ini")
        {
            _config = new IniConfig(iniFilePath);
            _currentAppID = _config.GetAppID();
            _gameExecutable = _config.GetGameExecutable();
            _gameArguments = _config.GetGameArguments();
            _steamApiDllPath = _config.GetSteamApiDllPath();

            string logFile = _config.GetValue("Logging", "LogFile", "uc_online.log");
            bool enableLogging = bool.TryParse(_config.GetValue("Logging", "EnableLogging", "true"), out bool loggingEnabled) && loggingEnabled;
            _logger = new Logger(logFile, enableLogging);

            _logger.Log($"uc-online64 initialized with AppID: {_currentAppID}");
            _logger.Log($"Game executable: {(string.IsNullOrEmpty(_gameExecutable) ? "not configured" : _gameExecutable)}");
            _logger.Log($"Steam API DLL path: {(string.IsNullOrEmpty(_steamApiDllPath) ? "default loading" : _steamApiDllPath)}");
        }

        [DllImport("steam_api64", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool SteamAPI_Init(out StringBuilder errorMessage);

        [DllImport("steam_api64", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SteamAPI_Shutdown();

        [DllImport("steam_api64", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SteamAPI_RunCallbacks();

        [DllImport("steam_api64", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool SteamAPI_RestartAppIfNecessary(uint appId);

        [DllImport("steam_api64", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr SteamClient();

        public bool Initializeuc_online()
        {
            try
            {
                _logger.Log("Initializing Steam with AppID: " + _currentAppID);

                CreateAppIdFile();

                LoadSteamApi64Dll();

                StringBuilder errorMsg = new StringBuilder(1024);
                bool result = SteamAPI_Init(out errorMsg);

                if (!result)
                {
                    Console.WriteLine($"SteamAPI_Init failed: {errorMsg}");

                    if (SteamAPI_RestartAppIfNecessary(_currentAppID))
                    {
                        return false;
                    }

                    return false;
                }

                _steamInitialized = true;
                _logger.Log("Steam initialized successfully");

                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Exception during Steam initialization: {ex.Message}");
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
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to create steam_appid.txt: {ex.Message}");
            }
        }

        private void LoadSteamApi64Dll()
        {
            try
            {
                if (string.IsNullOrEmpty(_steamApiDllPath))
                {
                    _logger.Log("No custom Steam API DLL path configured, using default loading");
                    return;
                }

                string dllPath = Path.Combine(_steamApiDllPath, "steam_api64.dll");
                if (File.Exists(dllPath))
                {
                    _logger.Log($"Found steam_api64.dll at: {dllPath}");

                    IntPtr handle = LoadLibrary(dllPath);
                    if (handle != IntPtr.Zero)
                    {
                        _logger.Log($"Successfully loaded steam_api64.dll from custom path");
                    }
                    else
                    {
                        _logger.LogWarning($"Failed to load steam_api64.dll from custom path, falling back to default loading");
                    }
                }
                else
                {
                    string win64Path = Path.Combine(_steamApiDllPath, "win64", "steam_api64.dll");
                    if (File.Exists(win64Path))
                    {
                        _logger.Log($"Found steam_api64.dll at: {win64Path}");

                        IntPtr handle = LoadLibrary(win64Path);
                        if (handle != IntPtr.Zero)
                        {
                            _logger.Log($"Successfully loaded steam_api64.dll from win64 subdirectory");
                            return;
                        }
                        else
                        {
                            _logger.LogWarning($"Failed to load steam_api64.dll from win64 subdirectory, falling back to default loading");
                        }
                    }

                    _logger.LogWarning($"steam_api64.dll not found at configured path, using default loading");
                }
            }
            catch (Exception ex)
            {
                _logger.LogException(ex, "Error loading Steam API DLL");
                Console.WriteLine($"Error loading Steam API DLL: {ex.Message}");
            }
        }

        [DllImport("kernel32", SetLastError = true, CharSet = CharSet.Unicode)]
        private static extern IntPtr LoadLibrary(string lpFileName);

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

        public void Dispose()
        {
            _logger.Log("uc-online64 shutting down");
            Shutdownuc_online();
        }
    }
}