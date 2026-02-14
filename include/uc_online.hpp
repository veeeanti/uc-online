#pragma once

#include "ini_config.hpp"
#include "logger.hpp"
#include <string>
#include <memory>

// Linux-specific includes
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <spawn.h>
#include <sys/wait.h>

class UCOnline {
public:
    UCOnline(const std::string& iniFilePath = "config.ini");
    ~UCOnline();

    bool InitializeUCOnline();
    void ShutdownUCOnline();
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

private:
    bool _steamInitialized = false;
    uint32_t _currentAppID;
    std::unique_ptr<IniConfig> _config;
    std::unique_ptr<Logger> _logger;
    std::string _gameExecutable;
    std::string _gameArguments;
};