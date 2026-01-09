#include "steam_api_interfaces.h"
#include <windows.h>

// Function to check if a function is exported from a DLL
bool IsFunctionExported(HMODULE hModule, const char* functionName) {
    return GetProcAddress(hModule, functionName) != nullptr;
}

// Function to load Steam API DLL and try different initialization functions
HMODULE LoadSteamAPI(const char* dllPath) {
    HMODULE hModule = LoadLibraryA(dllPath);
    if (!hModule) {
        return nullptr;
    }
    
    // Try different Steam API initialization functions
    if (IsFunctionExported(hModule, "SteamAPI_Init")) {
        return hModule;
    }
    
    if (IsFunctionExported(hModule, "SteamAPI_InitEx")) {
        return hModule;
    }
    
    if (IsFunctionExported(hModule, "SteamAPI_InitFlat")) {
        return hModule;
    }
    
    if (IsFunctionExported(hModule, "SteamAPI_InitSafe")) {
        return hModule;
    }
    
    // If none of the initialization functions are available, unload the DLL and return nullptr
    FreeLibrary(hModule);
    return nullptr;
}