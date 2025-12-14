using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace uc_online
{
    public class IniConfig
    {
        private string _iniFilePath;
        private Dictionary<string, Dictionary<string, string>> _configData;

        public IniConfig(string iniFilePath = "config.ini")
        {
            _iniFilePath = iniFilePath;
            _configData = new Dictionary<string, Dictionary<string, string>>(StringComparer.OrdinalIgnoreCase);
            LoadConfig();
        }

        public void LoadConfig()
        {
            _configData.Clear();

            if (!File.Exists(_iniFilePath))
            {
                CreateDefaultConfig();
                return;
            }

            try
            {
                string[] lines = File.ReadAllLines(_iniFilePath);
                string currentSection = "";

                foreach (string line in lines)
                {
                    string trimmedLine = line.Trim();

                    if (string.IsNullOrWhiteSpace(trimmedLine) || trimmedLine.StartsWith(";") || trimmedLine.StartsWith("#"))
                        continue;

                    if (trimmedLine.StartsWith("[") && trimmedLine.EndsWith("]"))
                    {
                        currentSection = trimmedLine.Substring(1, trimmedLine.Length - 2);
                        if (!_configData.ContainsKey(currentSection))
                        {
                            _configData[currentSection] = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
                        }
                        continue;
                    }

                    if (!string.IsNullOrEmpty(currentSection))
                    {
                        int equalsPos = trimmedLine.IndexOf('=');
                        if (equalsPos > 0)
                        {
                            string key = trimmedLine.Substring(0, equalsPos).Trim();
                            string value = trimmedLine.Substring(equalsPos + 1).Trim();
                            _configData[currentSection][key] = value;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error loading config: {ex.Message}");
                CreateDefaultConfig();
            }
        }

        private void CreateDefaultConfig()
        {
            var defaultConfig = new StringBuilder();
            defaultConfig.AppendLine("[uc-online]");
            defaultConfig.AppendLine("AppID = 480");
            defaultConfig.AppendLine("GameExecutable = ");
            defaultConfig.AppendLine("GameArguments = ");
            defaultConfig.AppendLine("SteamAppIdFile = steam_appid.txt");
            defaultConfig.AppendLine("; Path to steam_api.dll (leave empty to use default loading)");
            defaultConfig.AppendLine("; Example: SteamApiDllPath = C:\\Steam\\steam_api.dll");
            defaultConfig.AppendLine("SteamApiDLLPath = ");
            defaultConfig.AppendLine("");
            defaultConfig.AppendLine("[Logging]");
            defaultConfig.AppendLine("EnableLogging = true");
            defaultConfig.AppendLine("LogFile = uc-online.log");

            try
            {
                File.WriteAllText(_iniFilePath, defaultConfig.ToString());
                LoadConfig();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error creating default config: {ex.Message}");
            }
        }

        public void SaveConfig()
        {
            try
            {
                var configBuilder = new StringBuilder();

                foreach (var section in _configData)
                {
                    configBuilder.AppendLine($"[{section.Key}]");

                    foreach (var kvp in section.Value)
                    {
                        configBuilder.AppendLine($"{kvp.Key} = {kvp.Value}");
                    }

                    configBuilder.AppendLine();
                }

                File.WriteAllText(_iniFilePath, configBuilder.ToString());
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error saving config: {ex.Message}");
            }
        }

        public string GetValue(string section, string key, string defaultValue = "")
        {
            if (_configData.TryGetValue(section, out var sectionData) &&
                sectionData.TryGetValue(key, out var value))
            {
                return value;
            }
            return defaultValue;
        }

        public void SetValue(string section, string key, string value)
        {
            if (!_configData.ContainsKey(section))
            {
                _configData[section] = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
            }

            _configData[section][key] = value;
        }

        public uint GetAppID()
        {
            string appIdStr = GetValue("uc-online", "AppID", "480");
            if (uint.TryParse(appIdStr, out uint appId))
            {
                return appId;
            }
            return 480;
        }

        public void SetAppID(uint appId)
        {
            SetValue("uc-online", "AppID", appId.ToString());
        }

        public string GetGameExecutable()
        {
            return GetValue("uc-online", "GameExecutable", "");
        }

        public void SetGameExecutable(string gameExePath)
        {
            SetValue("uc-online", "GameExecutable", gameExePath);
        }

        public string GetGameArguments()
        {
            return GetValue("uc-online", "GameArguments", "");
        }

        public void SetGameArguments(string arguments)
        {
            SetValue("uc-online", "GameArguments", arguments);
        }

        public string GetSteamApiDllPath()
        {
            return GetValue("uc-online", "SteamApiDllPath", "");
        }

        public void SetSteamApiDllPath(string dllPath)
        {
            SetValue("uc-online", "SteamApiDllPath", dllPath);
            SaveConfig();
        }
    }
}