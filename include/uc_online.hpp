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

// Steam includes
#include <steam/steam_api.h>
#include <steam/isteamgameserver.h>
#include <steam/isteamugc.h>
#include <steam/isteamhttp.h>
#include <steam/isteamnetworking.h>
#include <steam/isteamclient.h>

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

    // Steam interface accessors
    ISteamApps* GetSteamApps() const { return _steamApps; }
    ISteamUser* GetSteamUser() const { return _steamUser; }
    ISteamGameServer* GetSteamGameServer() const { return _steamGameServer; }
    ISteamUGC* GetSteamUGC() const { return _steamUGC; }
    ISteamHTTP* GetSteamHTTP() const { return _steamHTTP; }
    ISteamNetworking* GetSteamNetworking() const { return _steamNetworking; }
    ISteamClient* GetSteamClient() const { return _steamClient; }

private:
    bool _steamInitialized = false;
    uint32_t _currentAppID;
    std::unique_ptr<IniConfig> _config;
    std::unique_ptr<Logger> _logger;
    std::string _gameExecutable;
    std::string _gameArguments;

    bool InitializeSteamAPI();
    bool InitializeSteamInterfaces();
    bool InitializeSteamGameServer();
    bool InitializeSteamUGC();
    bool InitializeSteamHTTP();
    bool InitializeSteamNetworking();
    bool InitializeSteamClient();

    // Steam interfaces
    ISteamApps* _steamApps = nullptr;
    ISteamUser* _steamUser = nullptr;
    ISteamGameServer* _steamGameServer = nullptr;
    ISteamUGC* _steamUGC = nullptr;
    ISteamHTTP* _steamHTTP = nullptr;
    ISteamNetworking* _steamNetworking = nullptr;
    ISteamClient* _steamClient = nullptr;
};