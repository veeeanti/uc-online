#include "steam_api_interfaces.h"
#include <windows.h>

// Function to check if a function is exported from a DLL
bool IsFunctionExported(HMODULE hModule, const char* functionName) {
    return GetProcAddress(hModule, functionName) != nullptr;
}

// Function to load Steam API DLL and check for required exports
HMODULE LoadSteamAPI(const char* dllPath) {
    HMODULE hModule = LoadLibraryA(dllPath);
    if (!hModule) {
        return nullptr;
    }
    
    // Check for required exports
    bool hasSteamAPI_Init = IsFunctionExported(hModule, "SteamAPI_Init");
    bool hasSteamAPI_Shutdown = IsFunctionExported(hModule, "SteamAPI_Shutdown");
    bool hasSteamAPI_RunCallbacks = IsFunctionExported(hModule, "SteamAPI_RunCallbacks");
    bool hasSteamAPI_RestartAppIfNecessary = IsFunctionExported(hModule, "SteamAPI_RestartAppIfNecessary");
    
    // If any of the required functions are missing, unload the DLL and return nullptr
    if (!hasSteamAPI_Init || !hasSteamAPI_Shutdown || !hasSteamAPI_RunCallbacks || !hasSteamAPI_RestartAppIfNecessary) {
        FreeLibrary(hModule);
        return nullptr;
    }
    
    return hModule;
}