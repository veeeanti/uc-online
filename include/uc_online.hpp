#pragma once

#include "ini_config.hpp"
#include "logger.hpp"
#include <string>
#include <memory>
#include <windows.h>

class UCOnline {
public:
    UCOnline(const std::string& iniFilePath = "config.ini");
    ~UCOnline();

    bool InitializeUCOnline();
    void ShutdownUCOnline();
    void RunSteamCallbacks();
    void SetCustomAppID(uint32_t appID);
    uint32_t GetCurrentAppID() const;
    bool IsSteamInitialized() const;

    void CreateAppIdFile();
    bool LaunchGame();
    void SetGameExecutable(const std::string& gameExePath);
    void SetGameArguments(const std::string& arguments);
    std::string GetGameExecutable() const;
    std::string GetGameArguments() const;
    void SaveConfig();
    void ReloadConfig();

    Logger* GetLogger();
    void SetLoggingEnabled(bool enabled);
    bool IsLoggingEnabled() const;
    void ClearLog();

    std::string GetSteamApiDllPath() const;
    void SetSteamApiDllPath(const std::string& dllPath);

private:
    bool _steamInitialized = false;
    uint32_t _currentAppID;
    std::unique_ptr<IniConfig> _config;
    std::unique_ptr<Logger> _logger;
    std::string _gameExecutable;
    std::string _gameArguments;
    std::string _steamApiDllPath;

};