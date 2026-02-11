#include "uc_online.hpp"
#include <windows.h>
#include <string>
#include <unordered_map>
#include <mutex>

// Global storage for string returns (thread-safe)
static std::mutex g_stringMutex;
static std::unordered_map<void*, std::string> g_gameExeStrings;
static std::unordered_map<void*, std::string> g_gameArgStrings;

// Global UCOnline instance for auto-initialization when used as winmm.dll proxy
static UCOnline* g_ucOnlineInstance = nullptr;
static bool g_autoInitialized = false;
bool g_deferInitialization = true;

// Handle to the original winmm.dll
static HMODULE g_hOriginalWinmm = nullptr;

// ============================================================================
// winmm.dll proxy - original function pointers
// These are resolved at DLL load time and used by winmm_stubs.cpp
// ============================================================================

#define DECLARE_ORIG(name) FARPROC orig_##name = nullptr

DECLARE_ORIG(CloseDriver);
DECLARE_ORIG(DefDriverProc);
DECLARE_ORIG(DriverCallback);
DECLARE_ORIG(DrvGetModuleHandle);
DECLARE_ORIG(GetDriverModuleHandle);
DECLARE_ORIG(OpenDriver);
DECLARE_ORIG(PlaySoundA);
DECLARE_ORIG(PlaySoundW);
DECLARE_ORIG(SendDriverMessage);
DECLARE_ORIG(auxGetDevCapsA);
DECLARE_ORIG(auxGetDevCapsW);
DECLARE_ORIG(auxGetNumDevs);
DECLARE_ORIG(auxGetVolume);
DECLARE_ORIG(auxOutMessage);
DECLARE_ORIG(auxSetVolume);
DECLARE_ORIG(joyConfigChanged);
DECLARE_ORIG(joyGetDevCapsA);
DECLARE_ORIG(joyGetDevCapsW);
DECLARE_ORIG(joyGetNumDevs);
DECLARE_ORIG(joyGetPos);
DECLARE_ORIG(joyGetPosEx);
DECLARE_ORIG(joyGetThreshold);
DECLARE_ORIG(joyReleaseCapture);
DECLARE_ORIG(joySetCapture);
DECLARE_ORIG(joySetThreshold);
DECLARE_ORIG(mciDriverNotify);
DECLARE_ORIG(mciDriverYield);
DECLARE_ORIG(mciExecute);
DECLARE_ORIG(mciFreeCommandResource);
DECLARE_ORIG(mciGetCreatorTask);
DECLARE_ORIG(mciGetDeviceIDA);
DECLARE_ORIG(mciGetDeviceIDW);
DECLARE_ORIG(mciGetDeviceIDFromElementIDA);
DECLARE_ORIG(mciGetDeviceIDFromElementIDW);
DECLARE_ORIG(mciGetDriverData);
DECLARE_ORIG(mciGetErrorStringA);
DECLARE_ORIG(mciGetErrorStringW);
DECLARE_ORIG(mciGetYieldProc);
DECLARE_ORIG(mciLoadCommandResource);
DECLARE_ORIG(mciSendCommandA);
DECLARE_ORIG(mciSendCommandW);
DECLARE_ORIG(mciSendStringA);
DECLARE_ORIG(mciSendStringW);
DECLARE_ORIG(mciSetDriverData);
DECLARE_ORIG(mciSetYieldProc);
DECLARE_ORIG(midiConnect);
DECLARE_ORIG(midiDisconnect);
DECLARE_ORIG(midiInAddBuffer);
DECLARE_ORIG(midiInClose);
DECLARE_ORIG(midiInGetDevCapsA);
DECLARE_ORIG(midiInGetDevCapsW);
DECLARE_ORIG(midiInGetErrorTextA);
DECLARE_ORIG(midiInGetErrorTextW);
DECLARE_ORIG(midiInGetID);
DECLARE_ORIG(midiInGetNumDevs);
DECLARE_ORIG(midiInMessage);
DECLARE_ORIG(midiInOpen);
DECLARE_ORIG(midiInPrepareHeader);
DECLARE_ORIG(midiInReset);
DECLARE_ORIG(midiInStart);
DECLARE_ORIG(midiInStop);
DECLARE_ORIG(midiInUnprepareHeader);
DECLARE_ORIG(midiOutCacheDrumPatches);
DECLARE_ORIG(midiOutCachePatches);
DECLARE_ORIG(midiOutClose);
DECLARE_ORIG(midiOutGetDevCapsA);
DECLARE_ORIG(midiOutGetDevCapsW);
DECLARE_ORIG(midiOutGetErrorTextA);
DECLARE_ORIG(midiOutGetErrorTextW);
DECLARE_ORIG(midiOutGetID);
DECLARE_ORIG(midiOutGetNumDevs);
DECLARE_ORIG(midiOutGetVolume);
DECLARE_ORIG(midiOutLongMsg);
DECLARE_ORIG(midiOutMessage);
DECLARE_ORIG(midiOutOpen);
DECLARE_ORIG(midiOutPrepareHeader);
DECLARE_ORIG(midiOutReset);
DECLARE_ORIG(midiOutSetVolume);
DECLARE_ORIG(midiOutShortMsg);
DECLARE_ORIG(midiOutUnprepareHeader);
DECLARE_ORIG(midiStreamClose);
DECLARE_ORIG(midiStreamOpen);
DECLARE_ORIG(midiStreamOut);
DECLARE_ORIG(midiStreamPause);
DECLARE_ORIG(midiStreamPosition);
DECLARE_ORIG(midiStreamProperty);
DECLARE_ORIG(midiStreamRestart);
DECLARE_ORIG(midiStreamStop);
DECLARE_ORIG(mixerClose);
DECLARE_ORIG(mixerGetControlDetailsA);
DECLARE_ORIG(mixerGetControlDetailsW);
DECLARE_ORIG(mixerGetDevCapsA);
DECLARE_ORIG(mixerGetDevCapsW);
DECLARE_ORIG(mixerGetID);
DECLARE_ORIG(mixerGetLineControlsA);
DECLARE_ORIG(mixerGetLineControlsW);
DECLARE_ORIG(mixerGetLineInfoA);
DECLARE_ORIG(mixerGetLineInfoW);
DECLARE_ORIG(mixerGetNumDevs);
DECLARE_ORIG(mixerMessage);
DECLARE_ORIG(mixerOpen);
DECLARE_ORIG(mixerSetControlDetails);
DECLARE_ORIG(mmDrvInstall);
DECLARE_ORIG(mmGetCurrentTask);
DECLARE_ORIG(mmTaskBlock);
DECLARE_ORIG(mmTaskCreate);
DECLARE_ORIG(mmTaskSignal);
DECLARE_ORIG(mmTaskYield);
DECLARE_ORIG(mmioAdvance);
DECLARE_ORIG(mmioAscend);
DECLARE_ORIG(mmioClose);
DECLARE_ORIG(mmioCreateChunk);
DECLARE_ORIG(mmioDescend);
DECLARE_ORIG(mmioFlush);
DECLARE_ORIG(mmioGetInfo);
DECLARE_ORIG(mmioInstallIOProcA);
DECLARE_ORIG(mmioInstallIOProcW);
DECLARE_ORIG(mmioOpenA);
DECLARE_ORIG(mmioOpenW);
DECLARE_ORIG(mmioRead);
DECLARE_ORIG(mmioRenameA);
DECLARE_ORIG(mmioRenameW);
DECLARE_ORIG(mmioSeek);
DECLARE_ORIG(mmioSendMessage);
DECLARE_ORIG(mmioSetBuffer);
DECLARE_ORIG(mmioSetInfo);
DECLARE_ORIG(mmioStringToFOURCCA);
DECLARE_ORIG(mmioStringToFOURCCW);
DECLARE_ORIG(mmioWrite);
DECLARE_ORIG(mmsystemGetVersion);
DECLARE_ORIG(sndPlaySoundA);
DECLARE_ORIG(sndPlaySoundW);
DECLARE_ORIG(timeBeginPeriod);
DECLARE_ORIG(timeEndPeriod);
DECLARE_ORIG(timeGetDevCaps);
DECLARE_ORIG(timeGetSystemTime);
DECLARE_ORIG(timeGetTime);
DECLARE_ORIG(timeKillEvent);
DECLARE_ORIG(timeSetEvent);
DECLARE_ORIG(waveInAddBuffer);
DECLARE_ORIG(waveInClose);
DECLARE_ORIG(waveInGetDevCapsA);
DECLARE_ORIG(waveInGetDevCapsW);
DECLARE_ORIG(waveInGetErrorTextA);
DECLARE_ORIG(waveInGetErrorTextW);
DECLARE_ORIG(waveInGetID);
DECLARE_ORIG(waveInGetNumDevs);
DECLARE_ORIG(waveInGetPosition);
DECLARE_ORIG(waveInMessage);
DECLARE_ORIG(waveInOpen);
DECLARE_ORIG(waveInPrepareHeader);
DECLARE_ORIG(waveInReset);
DECLARE_ORIG(waveInStart);
DECLARE_ORIG(waveInStop);
DECLARE_ORIG(waveInUnprepareHeader);
DECLARE_ORIG(waveOutBreakLoop);
DECLARE_ORIG(waveOutClose);
DECLARE_ORIG(waveOutGetDevCapsA);
DECLARE_ORIG(waveOutGetDevCapsW);
DECLARE_ORIG(waveOutGetErrorTextA);
DECLARE_ORIG(waveOutGetErrorTextW);
DECLARE_ORIG(waveOutGetID);
DECLARE_ORIG(waveOutGetNumDevs);
DECLARE_ORIG(waveOutGetPitch);
DECLARE_ORIG(waveOutGetPlaybackRate);
DECLARE_ORIG(waveOutGetPosition);
DECLARE_ORIG(waveOutGetVolume);
DECLARE_ORIG(waveOutMessage);
DECLARE_ORIG(waveOutOpen);
DECLARE_ORIG(waveOutPause);
DECLARE_ORIG(waveOutPrepareHeader);
DECLARE_ORIG(waveOutReset);
DECLARE_ORIG(waveOutRestart);
DECLARE_ORIG(waveOutSetPitch);
DECLARE_ORIG(waveOutSetPlaybackRate);
DECLARE_ORIG(waveOutSetVolume);
DECLARE_ORIG(waveOutUnprepareHeader);
DECLARE_ORIG(waveOutWrite);

// ============================================================================
// Load the original winmm.dll and resolve ALL function pointers
// ============================================================================

bool InitializeWinmmProxy() {
    char systemPath[MAX_PATH];
    if (GetSystemDirectoryA(systemPath, MAX_PATH) == 0) {
        return false;
    }

    std::string winmmPath = std::string(systemPath) + "\\winmm.dll";
    g_hOriginalWinmm = LoadLibraryA(winmmPath.c_str());
    if (!g_hOriginalWinmm) {
        return false;
    }

    #define RESOLVE(name) orig_##name = GetProcAddress(g_hOriginalWinmm, #name)

    RESOLVE(CloseDriver);
    RESOLVE(DefDriverProc);
    RESOLVE(DriverCallback);
    RESOLVE(DrvGetModuleHandle);
    RESOLVE(GetDriverModuleHandle);
    RESOLVE(OpenDriver);
    RESOLVE(PlaySoundA);
    RESOLVE(PlaySoundW);
    RESOLVE(SendDriverMessage);
    RESOLVE(auxGetDevCapsA);
    RESOLVE(auxGetDevCapsW);
    RESOLVE(auxGetNumDevs);
    RESOLVE(auxGetVolume);
    RESOLVE(auxOutMessage);
    RESOLVE(auxSetVolume);
    RESOLVE(joyConfigChanged);
    RESOLVE(joyGetDevCapsA);
    RESOLVE(joyGetDevCapsW);
    RESOLVE(joyGetNumDevs);
    RESOLVE(joyGetPos);
    RESOLVE(joyGetPosEx);
    RESOLVE(joyGetThreshold);
    RESOLVE(joyReleaseCapture);
    RESOLVE(joySetCapture);
    RESOLVE(joySetThreshold);
    RESOLVE(mciDriverNotify);
    RESOLVE(mciDriverYield);
    RESOLVE(mciExecute);
    RESOLVE(mciFreeCommandResource);
    RESOLVE(mciGetCreatorTask);
    RESOLVE(mciGetDeviceIDA);
    RESOLVE(mciGetDeviceIDW);
    RESOLVE(mciGetDeviceIDFromElementIDA);
    RESOLVE(mciGetDeviceIDFromElementIDW);
    RESOLVE(mciGetDriverData);
    RESOLVE(mciGetErrorStringA);
    RESOLVE(mciGetErrorStringW);
    RESOLVE(mciGetYieldProc);
    RESOLVE(mciLoadCommandResource);
    RESOLVE(mciSendCommandA);
    RESOLVE(mciSendCommandW);
    RESOLVE(mciSendStringA);
    RESOLVE(mciSendStringW);
    RESOLVE(mciSetDriverData);
    RESOLVE(mciSetYieldProc);
    RESOLVE(midiConnect);
    RESOLVE(midiDisconnect);
    RESOLVE(midiInAddBuffer);
    RESOLVE(midiInClose);
    RESOLVE(midiInGetDevCapsA);
    RESOLVE(midiInGetDevCapsW);
    RESOLVE(midiInGetErrorTextA);
    RESOLVE(midiInGetErrorTextW);
    RESOLVE(midiInGetID);
    RESOLVE(midiInGetNumDevs);
    RESOLVE(midiInMessage);
    RESOLVE(midiInOpen);
    RESOLVE(midiInPrepareHeader);
    RESOLVE(midiInReset);
    RESOLVE(midiInStart);
    RESOLVE(midiInStop);
    RESOLVE(midiInUnprepareHeader);
    RESOLVE(midiOutCacheDrumPatches);
    RESOLVE(midiOutCachePatches);
    RESOLVE(midiOutClose);
    RESOLVE(midiOutGetDevCapsA);
    RESOLVE(midiOutGetDevCapsW);
    RESOLVE(midiOutGetErrorTextA);
    RESOLVE(midiOutGetErrorTextW);
    RESOLVE(midiOutGetID);
    RESOLVE(midiOutGetNumDevs);
    RESOLVE(midiOutGetVolume);
    RESOLVE(midiOutLongMsg);
    RESOLVE(midiOutMessage);
    RESOLVE(midiOutOpen);
    RESOLVE(midiOutPrepareHeader);
    RESOLVE(midiOutReset);
    RESOLVE(midiOutSetVolume);
    RESOLVE(midiOutShortMsg);
    RESOLVE(midiOutUnprepareHeader);
    RESOLVE(midiStreamClose);
    RESOLVE(midiStreamOpen);
    RESOLVE(midiStreamOut);
    RESOLVE(midiStreamPause);
    RESOLVE(midiStreamPosition);
    RESOLVE(midiStreamProperty);
    RESOLVE(midiStreamRestart);
    RESOLVE(midiStreamStop);
    RESOLVE(mixerClose);
    RESOLVE(mixerGetControlDetailsA);
    RESOLVE(mixerGetControlDetailsW);
    RESOLVE(mixerGetDevCapsA);
    RESOLVE(mixerGetDevCapsW);
    RESOLVE(mixerGetID);
    RESOLVE(mixerGetLineControlsA);
    RESOLVE(mixerGetLineControlsW);
    RESOLVE(mixerGetLineInfoA);
    RESOLVE(mixerGetLineInfoW);
    RESOLVE(mixerGetNumDevs);
    RESOLVE(mixerMessage);
    RESOLVE(mixerOpen);
    RESOLVE(mixerSetControlDetails);
    RESOLVE(mmDrvInstall);
    RESOLVE(mmGetCurrentTask);
    RESOLVE(mmTaskBlock);
    RESOLVE(mmTaskCreate);
    RESOLVE(mmTaskSignal);
    RESOLVE(mmTaskYield);
    RESOLVE(mmioAdvance);
    RESOLVE(mmioAscend);
    RESOLVE(mmioClose);
    RESOLVE(mmioCreateChunk);
    RESOLVE(mmioDescend);
    RESOLVE(mmioFlush);
    RESOLVE(mmioGetInfo);
    RESOLVE(mmioInstallIOProcA);
    RESOLVE(mmioInstallIOProcW);
    RESOLVE(mmioOpenA);
    RESOLVE(mmioOpenW);
    RESOLVE(mmioRead);
    RESOLVE(mmioRenameA);
    RESOLVE(mmioRenameW);
    RESOLVE(mmioSeek);
    RESOLVE(mmioSendMessage);
    RESOLVE(mmioSetBuffer);
    RESOLVE(mmioSetInfo);
    RESOLVE(mmioStringToFOURCCA);
    RESOLVE(mmioStringToFOURCCW);
    RESOLVE(mmioWrite);
    RESOLVE(mmsystemGetVersion);
    RESOLVE(sndPlaySoundA);
    RESOLVE(sndPlaySoundW);
    RESOLVE(timeBeginPeriod);
    RESOLVE(timeEndPeriod);
    RESOLVE(timeGetDevCaps);
    RESOLVE(timeGetSystemTime);
    RESOLVE(timeGetTime);
    RESOLVE(timeKillEvent);
    RESOLVE(timeSetEvent);
    RESOLVE(waveInAddBuffer);
    RESOLVE(waveInClose);
    RESOLVE(waveInGetDevCapsA);
    RESOLVE(waveInGetDevCapsW);
    RESOLVE(waveInGetErrorTextA);
    RESOLVE(waveInGetErrorTextW);
    RESOLVE(waveInGetID);
    RESOLVE(waveInGetNumDevs);
    RESOLVE(waveInGetPosition);
    RESOLVE(waveInMessage);
    RESOLVE(waveInOpen);
    RESOLVE(waveInPrepareHeader);
    RESOLVE(waveInReset);
    RESOLVE(waveInStart);
    RESOLVE(waveInStop);
    RESOLVE(waveInUnprepareHeader);
    RESOLVE(waveOutBreakLoop);
    RESOLVE(waveOutClose);
    RESOLVE(waveOutGetDevCapsA);
    RESOLVE(waveOutGetDevCapsW);
    RESOLVE(waveOutGetErrorTextA);
    RESOLVE(waveOutGetErrorTextW);
    RESOLVE(waveOutGetID);
    RESOLVE(waveOutGetNumDevs);
    RESOLVE(waveOutGetPitch);
    RESOLVE(waveOutGetPlaybackRate);
    RESOLVE(waveOutGetPosition);
    RESOLVE(waveOutGetVolume);
    RESOLVE(waveOutMessage);
    RESOLVE(waveOutOpen);
    RESOLVE(waveOutPause);
    RESOLVE(waveOutPrepareHeader);
    RESOLVE(waveOutReset);
    RESOLVE(waveOutRestart);
    RESOLVE(waveOutSetPitch);
    RESOLVE(waveOutSetPlaybackRate);
    RESOLVE(waveOutSetVolume);
    RESOLVE(waveOutUnprepareHeader);
    RESOLVE(waveOutWrite);

    #undef RESOLVE
    return true;
}

// ============================================================================
// Auto-initialization
// ============================================================================

void AutoInitializeUCOnline() {
    if (g_autoInitialized || g_ucOnlineInstance) {
        return;
    }

    g_ucOnlineInstance = new UCOnline("config.ini");
    if (g_ucOnlineInstance) {
        if (g_ucOnlineInstance->InitializeUCOnline()) {
            g_autoInitialized = true;
        } else {
            delete g_ucOnlineInstance;
            g_ucOnlineInstance = nullptr;
        }
    }
}

void AutoShutdownUCOnline() {
    if (g_ucOnlineInstance) {
        g_ucOnlineInstance->ShutdownUCOnline();
        delete g_ucOnlineInstance;
        g_ucOnlineInstance = nullptr;
        g_autoInitialized = false;
    }
}

// ============================================================================
// DLL Entry Point
// ============================================================================

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        InitializeWinmmProxy();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        AutoShutdownUCOnline();
        {
            std::lock_guard<std::mutex> lock(g_stringMutex);
            g_gameExeStrings.clear();
            g_gameArgStrings.clear();
        }
        break;
    }
    return TRUE;
}

// ============================================================================
// C API Implementation (UCOnline functions)
// ============================================================================

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
            std::lock_guard<std::mutex> lock(g_stringMutex);
            g_gameExeStrings.erase(instance);
            g_gameArgStrings.erase(instance);
        }
    }

    __declspec(dllexport) bool UCOnline_Initialize(void* instance) {
        if (!instance) return false;
        return static_cast<UCOnline*>(instance)->InitializeUCOnline();
    }

    __declspec(dllexport) void UCOnline_Shutdown(void* instance) {
        if (!instance) return;
        static_cast<UCOnline*>(instance)->ShutdownUCOnline();
    }

    __declspec(dllexport) void UCOnline_SetAppID(void* instance, uint32_t appID) {
        if (!instance) return;
        static_cast<UCOnline*>(instance)->SetCustomAppID(appID);
    }

    __declspec(dllexport) uint32_t UCOnline_GetAppID(void* instance) {
        if (!instance) return 0;
        return static_cast<UCOnline*>(instance)->GetCurrentAppID();
    }

    __declspec(dllexport) bool UCOnline_IsSteamInitialized(void* instance) {
        if (!instance) return false;
        return static_cast<UCOnline*>(instance)->IsSteamInitialized();
    }

    __declspec(dllexport) bool UCOnline_LaunchGame(void* instance) {
        if (!instance) return false;
        return static_cast<UCOnline*>(instance)->LaunchGame();
    }

    __declspec(dllexport) void UCOnline_SetGameExecutable(void* instance, const char* exePath) {
        if (!instance || !exePath) return;
        static_cast<UCOnline*>(instance)->SetGameExecutable(exePath);
    }

    __declspec(dllexport) void UCOnline_SetGameArguments(void* instance, const char* arguments) {
        if (!instance || !arguments) return;
        static_cast<UCOnline*>(instance)->SetGameArguments(arguments);
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
        static_cast<UCOnline*>(instance)->RunSteamCallbacks();
    }
}
