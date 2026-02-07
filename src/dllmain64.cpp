#include "uc_online64.hpp"
#include <windows.h>
#include <string>
#include <unordered_map>
#include <mutex>

// Global storage for string returns (thread-safe)
static std::mutex g_stringMutex;
static std::unordered_map<void*, std::string> g_gameExeStrings;
static std::unordered_map<void*, std::string> g_gameArgStrings;

// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // Initialize when DLL is loaded
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        // Cleanup when DLL is unloaded
        std::lock_guard<std::mutex> lock(g_stringMutex);
        g_gameExeStrings.clear();
        g_gameArgStrings.clear();
        break;
    }
    return TRUE;
}

// C API Implementation
extern "C" {
    __declspec(dllexport) void* UCOnline64_Create(const char* iniFilePath) {
        try {
            std::string path = iniFilePath ? iniFilePath : "config.ini";
            return new UCOnline64(path);
        } catch (...) {
            return nullptr;
        }
    }

    __declspec(dllexport) void UCOnline64_Destroy(void* instance) {
        if (instance) {
            UCOnline64* uc = static_cast<UCOnline64*>(instance);
            delete uc;
            
            // Clean up stored strings for this instance
            std::lock_guard<std::mutex> lock(g_stringMutex);
            g_gameExeStrings.erase(instance);
            g_gameArgStrings.erase(instance);
        }
    }

    __declspec(dllexport) bool UCOnline64_Initialize(void* instance) {
        if (!instance) return false;
        UCOnline64* uc = static_cast<UCOnline64*>(instance);
        return uc->InitializeUCOnline();
    }

    __declspec(dllexport) void UCOnline64_Shutdown(void* instance) {
        if (!instance) return;
        UCOnline64* uc = static_cast<UCOnline64*>(instance);
        uc->ShutdownUCOnline();
    }

    __declspec(dllexport) void UCOnline64_SetAppID(void* instance, uint32_t appID) {
        if (!instance) return;
        UCOnline64* uc = static_cast<UCOnline64*>(instance);
        uc->SetCustomAppID(appID);
    }

    __declspec(dllexport) uint32_t UCOnline64_GetAppID(void* instance) {
        if (!instance) return 0;
        UCOnline64* uc = static_cast<UCOnline64*>(instance);
        return uc->GetCurrentAppID();
    }

    __declspec(dllexport) bool UCOnline64_IsSteamInitialized(void* instance) {
        if (!instance) return false;
        UCOnline64* uc = static_cast<UCOnline64*>(instance);
        return uc->IsSteamInitialized();
    }

    __declspec(dllexport) bool UCOnline64_LaunchGame(void* instance) {
        if (!instance) return false;
        UCOnline64* uc = static_cast<UCOnline64*>(instance);
        return uc->LaunchGame();
    }

    __declspec(dllexport) void UCOnline64_SetGameExecutable(void* instance, const char* exePath) {
        if (!instance || !exePath) return;
        UCOnline64* uc = static_cast<UCOnline64*>(instance);
        uc->SetGameExecutable(exePath);
    }

   __declspec(dllexport) void UCOnline64_SetGameArguments(void* instance, const char* arguments) {
        if (!instance || !arguments) return;
        UCOnline64* uc = static_cast<UCOnline64*>(instance);
        uc->SetGameArguments(arguments);
    }

    __declspec(dllexport) const char* UCOnline64_GetGameExecutable(void* instance) {
        if (!instance) return "";
        UCOnline64* uc = static_cast<UCOnline64*>(instance);
        
        std::lock_guard<std::mutex> lock(g_stringMutex);
        g_gameExeStrings[instance] = uc->GetGameExecutable();
        return g_gameExeStrings[instance].c_str();
    }

    __declspec(dllexport) const char* UCOnline64_GetGameArguments(void* instance) {
        if (!instance) return "";
        UCOnline64* uc = static_cast<UCOnline64*>(instance);
        
        std::lock_guard<std::mutex> lock(g_stringMutex);
        g_gameArgStrings[instance] = uc->GetGameArguments();
        return g_gameArgStrings[instance].c_str();
    }

    __declspec(dllexport) void UCOnline64_RunCallbacks(void* instance) {
        if (!instance) return;
        UCOnline64* uc = static_cast<UCOnline64*>(instance);
        uc->RunSteamCallbacks();
    }
}
