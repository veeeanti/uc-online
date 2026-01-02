#include "uc_online64.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>

UCOnline64::UCOnline64(const std::string& iniFilePath) {
    _config = std::make_unique<IniConfig>(iniFilePath);
    _currentAppID = _config->GetAppID();
    _gameExecutable = _config->GetGameExecutable();
    _gameArguments = _config->GetGameArguments();
    _steamApiDllPath = _config->GetSteamApiDllPath();

    std::string logFile = _config->GetValue("Logging", "LogFile", "uc_online.log");
    bool enableLogging = _config->GetValue("Logging", "EnableLogging", "true") == "true";
    _logger = std::make_unique<Logger>(logFile, enableLogging);

    _logger->Log("uc-online64 initialized with appid: " + std::to_string(_currentAppID));
    _logger->Log("Game executable: " + (_gameExecutable.empty() ? "not configured" : _gameExecutable));
    _logger->Log("steam_api64.dll path: " + (_steamApiDllPath.empty() ? "default loading" : _steamApiDllPath));
}

UCOnline64::~UCOnline64() {
    _logger->Log("uc-online64 shutting down");
    ShutdownUCOnline();
}

bool UCOnline64::InitializeUCOnline() {
    try {
        if (_currentAppID == 0) {
            _logger->LogWarning("No appid set in the config.ini. This likely will not work.");
            _logger->LogWarning("Please set appid in config.ini, if there is not one - there will be one after running this.");
            _logger->LogWarning("Continuing without set appid.");
        } else {
            _logger->Log("Initializing Steam with appid: " + std::to_string(_currentAppID));
            CreateAppIdFile();
        }

        if (SteamAPI_RestartAppIfNecessary(_currentAppID)) {
            _logger->Log("Steam requested app restart");
            return false;
        }

        if (!InitializeSteamInterfaces()) {
            std::cout << "Failed to initialize Steam interfaces" << std::endl;
            return false;
        }

        char errorMsg[1024] = {0};
        if (!SteamAPI_Init(errorMsg)) {
            std::cout << "SteamAPI_Init failed: " << errorMsg << std::endl;
            return false;
        }

        _steamInitialized = true;
        _logger->Log("Steam initialized successfully");

        return true;
    } catch (const std::exception& ex) {
        std::cout << "Exception during Steam initialization: " << ex.what() << std::endl;
        return false;
    }
}

void UCOnline64::ShutdownUCOnline() {
    if (_steamInitialized) {
        _logger->Log("Shutting down...");
        SteamAPI_Shutdown();
        _steamInitialized = false;
        _logger->Log("Shutdown complete");
    }
}

void UCOnline64::RunSteamCallbacks() {
    if (_steamInitialized) {
        SteamAPI_RunCallbacks();
    }
}

void UCOnline64::SetCustomAppID(uint32_t appID) {
    _currentAppID = appID;
    _config->SetAppID(appID);
    _config->SaveConfig();
    _logger->Log("Appid changed to: " + std::to_string(appID));

    if (_steamInitialized) {
        _logger->Log("Reinitializing Steam with new appid");
        ShutdownUCOnline();
        InitializeUCOnline();
    }
}

uint32_t UCOnline64::GetCurrentAppID() const {
    return _currentAppID;
}

bool UCOnline64::IsSteamInitialized() const {
    return _steamInitialized;
}

void UCOnline64::CreateAppIdFile() {
    if (_currentAppID == 0) {
        _logger->Log("Skipping steam_appid.txt creation - no appid configured.");
        _logger->Log("If there is one already, it will be ignored.");
        return;
    }

    try {
        std::ofstream file("steam_appid.txt");
        if (file.is_open()) {
            file << _currentAppID;
            _logger->Log("Created steam_appid.txt with set appid: " + std::to_string(_currentAppID));
        } else {
            std::cerr << "Failed to create steam_appid.txt, make sure you're running this in a directory like your Documents or Downloads folder." << std::endl;
            std::cerr << "(e.g., \"C:\\Users\\user\\Downloads\\game folder\")" << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << "Failed to create steam_appid.txt: " << ex.what() << std::endl;
    }
}



bool UCOnline64::InitializeSteamInterfaces() {
    try {
        if (!SteamUser()) {
            _logger->LogError("SteamUser interface not available");
            return false;
        }

        if (!SteamApps()) {
            _logger->LogWarning("SteamApps interface not available");
        } else {
            _logger->Log("Successfully obtained SteamApps interface");
        }

        return true;
    } catch (const std::exception& ex) {
        _logger->LogException(ex, "Error initializing Steam interfaces");
        return false;
    }
}

bool UCOnline64::LaunchGame() {
    _logger->Log("Attempting to launch game: " + _gameExecutable);

    if (_gameExecutable.empty()) {
        _logger->LogError("No game executable configured in config.ini file. You'll need to do that to get anywhere here.");
        std::cout << "No game executable configured in config.ini file. (I suggest you set it lol)" << std::endl;
        return false;
    }

    if (!std::filesystem::exists(_gameExecutable)) {
        _logger->LogError("Game executable not found (Did you write it correctly? Path and all too, if applicable.): " + _gameExecutable);
        std::cout << "Game executable not found (Did you write it correctly? Path and all too, if applicable.): " << _gameExecutable << std::endl;
        return false;
    }

    try {
        _logger->Log("Launching game: " + _gameExecutable + " " + _gameArguments);
        std::cout << "Launching game: " << _gameExecutable << " " << _gameArguments << std::endl;

        std::filesystem::path exePath(_gameExecutable);
        std::string workingDir = exePath.parent_path().string();

        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi;

        std::string commandLine = "\"" + _gameExecutable + "\" " + _gameArguments;

        if (CreateProcessA(NULL, const_cast<char*>(commandLine.c_str()), NULL, NULL, FALSE, 0, NULL, workingDir.c_str(), &si, &pi)) {
            _logger->Log("Game launched successfully! (PID: " + std::to_string(pi.dwProcessId) + ")");
            std::cout << "Game launched successfully! The game's window should appear shortly. This window can be closed and / or may close on its own." << std::endl;
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return true;
        } else {
            _logger->LogError("Failed to launch game process");
            std::cout << "Failed to launch game process" << std::endl;
            return false;
        }
    } catch (const std::exception& ex) {
        _logger->LogException(ex, "Game launch failed");
        _logger->Log("This can happen for many reasons, if you're certain it should work then just try whatever you can. Throw whatever you got at the wall and see what sticks.");
        std::cout << "Error launching game: " << ex.what() << std::endl;
        return false;
    }
}

void UCOnline64::SetGameExecutable(const std::string& gameExePath) {
    _gameExecutable = gameExePath;
    _config->SetGameExecutable(gameExePath);
    _config->SaveConfig();
}

void UCOnline64::SetGameArguments(const std::string& arguments) {
    _gameArguments = arguments;
    _config->SetGameArguments(arguments);
    _config->SaveConfig();
}

std::string UCOnline64::GetGameExecutable() const {
    return _gameExecutable;
}

std::string UCOnline64::GetGameArguments() const {
    return _gameArguments;
}

void UCOnline64::SaveConfig() {
    _config->SetAppID(_currentAppID);
    _config->SetGameExecutable(_gameExecutable);
    _config->SetGameArguments(_gameArguments);
    _config->SaveConfig();
}

void UCOnline64::ReloadConfig() {
    _config->LoadConfig();
    _currentAppID = _config->GetAppID();
    _gameExecutable = _config->GetGameExecutable();
    _gameArguments = _config->GetGameArguments();
}

Logger* UCOnline64::GetLogger() {
    return _logger.get();
}

void UCOnline64::SetLoggingEnabled(bool enabled) {
    _logger->SetLoggingEnabled(enabled);
    _logger->Log("Logging " + std::string(enabled ? "enabled" : "disabled"));
}

bool UCOnline64::IsLoggingEnabled() const {
    return _logger->IsLoggingEnabledA();
}

void UCOnline64::ClearLog() {
    _logger->ClearLog();
}

std::string UCOnline64::GetSteamApiDllPath() const {
    return _steamApiDllPath;
}

void UCOnline64::SetSteamApiDllPath(const std::string& dllPath) {
    _steamApiDllPath = dllPath;
    _config->SetSteamApiDllPath(dllPath);
    _config->SaveConfig();
}