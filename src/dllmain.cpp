#include "uc_online.hpp"
#include <windows.h>
#include <string>
#include <unordered_map>
#include <mutex>

// Global storage for string returns (thread-safe)
static std::mutex g_stringMutex;
static std::unordered_map<void*, std::string> g_gameExeStrings;
static std::unordered_map<void*, std::string> g_gameArgStrings;

// Global UCOnline instance for auto-initialization when used as dinput8.dll proxy
static UCOnline* g_ucOnlineInstance = nullptr;
static bool g_autoInitialized = false;
static bool g_deferInitialization = true; // Defer Steam init until first DirectInput call

// Forward declarations for dinput8.dll proxy
extern "C" {
    typedef HRESULT (WINAPI *DirectInput8Create_t)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
    DirectInput8Create_t OriginalDirectInput8Create = nullptr;
}

// Forward declarations for auto-initialization functions
void AutoInitializeUCOnline();
void AutoShutdownUCOnline();

// Load the original dinput8.dll and get the DirectInput8Create function
bool InitializeDInput8Proxy() {
    char systemPath[MAX_PATH];
    if (GetSystemDirectoryA(systemPath, MAX_PATH) == 0) {
        return false;
    }
    
    std::string dinput8Path = std::string(systemPath) + "\\dinput8.dll";
    HMODULE hOriginal = LoadLibraryA(dinput8Path.c_str());
    if (!hOriginal) {
        return false;
    }
    
    OriginalDirectInput8Create = (DirectInput8Create_t)GetProcAddress(hOriginal, "DirectInput8Create");
    return OriginalDirectInput8Create != nullptr;
}

// Proxy function that forwards to the original dinput8.dll
extern "C" __declspec(dllexport) HRESULT WINAPI DirectInput8Create(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID* ppvOut,
    LPUNKNOWN punkOuter
) {
    // Initialize Steam lazily on first DirectInput call to avoid DllMain restrictions
    if (g_deferInitialization) {
        g_deferInitialization = false;
        AutoInitializeUCOnline();
    }

    if (!OriginalDirectInput8Create) {
        return E_FAIL;
    }
    return OriginalDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

// Auto-initialize UCOnline when loaded as a proxy DLL
void AutoInitializeUCOnline() {
    if (g_autoInitialized || g_ucOnlineInstance) {
        return;
    }
    
    // Create and initialize UCOnline instance
    g_ucOnlineInstance = new UCOnline("config.ini");
    if (g_ucOnlineInstance) {
        if (g_ucOnlineInstance->InitializeUCOnline()) {
            g_autoInitialized = true;
            // Note: We don't call LaunchGame() here because the game is already
            // being launched by the process that's loading this DLL as dinput8.dll
        } else {
            delete g_ucOnlineInstance;
            g_ucOnlineInstance = nullptr;
        }
    }
}

// Cleanup auto-initialized instance
void AutoShutdownUCOnline() {
    if (g_ucOnlineInstance) {
        g_ucOnlineInstance->ShutdownUCOnline();
        delete g_ucOnlineInstance;
        g_ucOnlineInstance = nullptr;
        g_autoInitialized = false;
    }
}

// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // Initialize the dinput8 proxy (just load the original DLL)
        InitializeDInput8Proxy();
        // Note: Steam initialization is deferred to avoid crashes in DllMain
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        // Cleanup when DLL is unloaded
        AutoShutdownUCOnline();
        std::lock_guard<std::mutex> lock(g_stringMutex);
        g_gameExeStrings.clear();
        g_gameArgStrings.clear();
        break;
    }
    return TRUE;
}

// C API Implementation
extern "C" {
    __declspec(dllexport) void* UCOnline_Create(const char* iniFilePath) {
        try {
            std::string path = iniFilePath ? iniFilePath : "config.ini";
            return new UCOnline(path);
        } catch (...) {
            return nullptr;
        }
    }

    __declspec(dllexport) void UCOnline_Destroy(void* instance) {
        if (instance) {
            UCOnline* uc = static_cast<UCOnline*>(instance);
            delete uc;
            
            // Clean up stored strings for this instance
            std::lock_guard<std::mutex> lock(g_stringMutex);
            g_gameExeStrings.erase(instance);
            g_gameArgStrings.erase(instance);
        }
    }

    __declspec(dllexport) bool UCOnline_Initialize(void* instance) {
        if (!instance) return false;
        UCOnline* uc = static_cast<UCOnline*>(instance);
        return uc->InitializeUCOnline();
    }

    __declspec(dllexport) void UCOnline_Shutdown(void* instance) {
        if (!instance) return;
        UCOnline* uc = static_cast<UCOnline*>(instance);
        uc->ShutdownUCOnline();
    }

    __declspec(dllexport) void UCOnline_SetAppID(void* instance, uint32_t appID) {
        if (!instance) return;
        UCOnline* uc = static_cast<UCOnline*>(instance);
        uc->SetCustomAppID(appID);
    }

    __declspec(dllexport) uint32_t UCOnline_GetAppID(void* instance) {
        if (!instance) return 0;
        UCOnline* uc = static_cast<UCOnline*>(instance);
        return uc->GetCurrentAppID();
    }

    __declspec(dllexport) bool UCOnline_IsSteamInitialized(void* instance) {
        if (!instance) return false;
        UCOnline* uc = static_cast<UCOnline*>(instance);
        return uc->IsSteamInitialized();
    }

    __declspec(dllexport) bool UCOnline_LaunchGame(void* instance) {
        if (!instance) return false;
        UCOnline* uc = static_cast<UCOnline*>(instance);
        return uc->LaunchGame();
    }

    __declspec(dllexport) void UCOnline_SetGameExecutable(void* instance, const char* exePath) {
        if (!instance || !exePath) return;
        UCOnline* uc = static_cast<UCOnline*>(instance);
        uc->SetGameExecutable(exePath);
    }

    __declspec(dllexport) void UCOnline_SetGameArguments(void* instance, const char* arguments) {
        if (!instance || !arguments) return;
        UCOnline* uc = static_cast<UCOnline*>(instance);
        uc->SetGameArguments(arguments);
    }

    __declspec(dllexport) const char* UCOnline_GetGameExecutable(void* instance) {
        if (!instance) return "";
        UCOnline* uc = static_cast<UCOnline*>(instance);
        
        std::lock_guard<std::mutex> lock(g_stringMutex);
        g_gameExeStrings[instance] = uc->GetGameExecutable();
        return g_gameExeStrings[instance].c_str();
    }

    __declspec(dllexport) const char* UCOnline_GetGameArguments(void* instance) {
        if (!instance) return "";
        UCOnline* uc = static_cast<UCOnline*>(instance);
        
        std::lock_guard<std::mutex> lock(g_stringMutex);
        g_gameArgStrings[instance] = uc->GetGameArguments();
        return g_gameArgStrings[instance].c_str();
    }

    __declspec(dllexport) void UCOnline_RunCallbacks(void* instance) {
        if (!instance) return;
        UCOnline* uc = static_cast<UCOnline*>(instance);
        uc->RunSteamCallbacks();
    }
}
