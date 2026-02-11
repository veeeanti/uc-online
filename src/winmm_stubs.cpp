// winmm_stubs.cpp - Proxy stub functions for ALL winmm.dll exports
// Each function forwards to the original winmm.dll loaded at runtime.
// The .def file maps the real export names (e.g. "timeGetTime") to these
// proxy_ prefixed functions.

#include <windows.h>

// Original function pointers (declared in dllmain.cpp / dllmain64.cpp)
#define DECLARE_ORIG_EXTERN(name) extern FARPROC orig_##name

DECLARE_ORIG_EXTERN(CloseDriver);
DECLARE_ORIG_EXTERN(DefDriverProc);
DECLARE_ORIG_EXTERN(DriverCallback);
DECLARE_ORIG_EXTERN(DrvGetModuleHandle);
DECLARE_ORIG_EXTERN(GetDriverModuleHandle);
DECLARE_ORIG_EXTERN(OpenDriver);
DECLARE_ORIG_EXTERN(PlaySoundA);
DECLARE_ORIG_EXTERN(PlaySoundW);
DECLARE_ORIG_EXTERN(SendDriverMessage);
DECLARE_ORIG_EXTERN(auxGetDevCapsA);
DECLARE_ORIG_EXTERN(auxGetDevCapsW);
DECLARE_ORIG_EXTERN(auxGetNumDevs);
DECLARE_ORIG_EXTERN(auxGetVolume);
DECLARE_ORIG_EXTERN(auxOutMessage);
DECLARE_ORIG_EXTERN(auxSetVolume);
DECLARE_ORIG_EXTERN(joyConfigChanged);
DECLARE_ORIG_EXTERN(joyGetDevCapsA);
DECLARE_ORIG_EXTERN(joyGetDevCapsW);
DECLARE_ORIG_EXTERN(joyGetNumDevs);
DECLARE_ORIG_EXTERN(joyGetPos);
DECLARE_ORIG_EXTERN(joyGetPosEx);
DECLARE_ORIG_EXTERN(joyGetThreshold);
DECLARE_ORIG_EXTERN(joyReleaseCapture);
DECLARE_ORIG_EXTERN(joySetCapture);
DECLARE_ORIG_EXTERN(joySetThreshold);
DECLARE_ORIG_EXTERN(mciDriverNotify);
DECLARE_ORIG_EXTERN(mciDriverYield);
DECLARE_ORIG_EXTERN(mciExecute);
DECLARE_ORIG_EXTERN(mciFreeCommandResource);
DECLARE_ORIG_EXTERN(mciGetCreatorTask);
DECLARE_ORIG_EXTERN(mciGetDeviceIDA);
DECLARE_ORIG_EXTERN(mciGetDeviceIDW);
DECLARE_ORIG_EXTERN(mciGetDeviceIDFromElementIDA);
DECLARE_ORIG_EXTERN(mciGetDeviceIDFromElementIDW);
DECLARE_ORIG_EXTERN(mciGetDriverData);
DECLARE_ORIG_EXTERN(mciGetErrorStringA);
DECLARE_ORIG_EXTERN(mciGetErrorStringW);
DECLARE_ORIG_EXTERN(mciGetYieldProc);
DECLARE_ORIG_EXTERN(mciLoadCommandResource);
DECLARE_ORIG_EXTERN(mciSendCommandA);
DECLARE_ORIG_EXTERN(mciSendCommandW);
DECLARE_ORIG_EXTERN(mciSendStringA);
DECLARE_ORIG_EXTERN(mciSendStringW);
DECLARE_ORIG_EXTERN(mciSetDriverData);
DECLARE_ORIG_EXTERN(mciSetYieldProc);
DECLARE_ORIG_EXTERN(midiConnect);
DECLARE_ORIG_EXTERN(midiDisconnect);
DECLARE_ORIG_EXTERN(midiInAddBuffer);
DECLARE_ORIG_EXTERN(midiInClose);
DECLARE_ORIG_EXTERN(midiInGetDevCapsA);
DECLARE_ORIG_EXTERN(midiInGetDevCapsW);
DECLARE_ORIG_EXTERN(midiInGetErrorTextA);
DECLARE_ORIG_EXTERN(midiInGetErrorTextW);
DECLARE_ORIG_EXTERN(midiInGetID);
DECLARE_ORIG_EXTERN(midiInGetNumDevs);
DECLARE_ORIG_EXTERN(midiInMessage);
DECLARE_ORIG_EXTERN(midiInOpen);
DECLARE_ORIG_EXTERN(midiInPrepareHeader);
DECLARE_ORIG_EXTERN(midiInReset);
DECLARE_ORIG_EXTERN(midiInStart);
DECLARE_ORIG_EXTERN(midiInStop);
DECLARE_ORIG_EXTERN(midiInUnprepareHeader);
DECLARE_ORIG_EXTERN(midiOutCacheDrumPatches);
DECLARE_ORIG_EXTERN(midiOutCachePatches);
DECLARE_ORIG_EXTERN(midiOutClose);
DECLARE_ORIG_EXTERN(midiOutGetDevCapsA);
DECLARE_ORIG_EXTERN(midiOutGetDevCapsW);
DECLARE_ORIG_EXTERN(midiOutGetErrorTextA);
DECLARE_ORIG_EXTERN(midiOutGetErrorTextW);
DECLARE_ORIG_EXTERN(midiOutGetID);
DECLARE_ORIG_EXTERN(midiOutGetNumDevs);
DECLARE_ORIG_EXTERN(midiOutGetVolume);
DECLARE_ORIG_EXTERN(midiOutLongMsg);
DECLARE_ORIG_EXTERN(midiOutMessage);
DECLARE_ORIG_EXTERN(midiOutOpen);
DECLARE_ORIG_EXTERN(midiOutPrepareHeader);
DECLARE_ORIG_EXTERN(midiOutReset);
DECLARE_ORIG_EXTERN(midiOutSetVolume);
DECLARE_ORIG_EXTERN(midiOutShortMsg);
DECLARE_ORIG_EXTERN(midiOutUnprepareHeader);
DECLARE_ORIG_EXTERN(midiStreamClose);
DECLARE_ORIG_EXTERN(midiStreamOpen);
DECLARE_ORIG_EXTERN(midiStreamOut);
DECLARE_ORIG_EXTERN(midiStreamPause);
DECLARE_ORIG_EXTERN(midiStreamPosition);
DECLARE_ORIG_EXTERN(midiStreamProperty);
DECLARE_ORIG_EXTERN(midiStreamRestart);
DECLARE_ORIG_EXTERN(midiStreamStop);
DECLARE_ORIG_EXTERN(mixerClose);
DECLARE_ORIG_EXTERN(mixerGetControlDetailsA);
DECLARE_ORIG_EXTERN(mixerGetControlDetailsW);
DECLARE_ORIG_EXTERN(mixerGetDevCapsA);
DECLARE_ORIG_EXTERN(mixerGetDevCapsW);
DECLARE_ORIG_EXTERN(mixerGetID);
DECLARE_ORIG_EXTERN(mixerGetLineControlsA);
DECLARE_ORIG_EXTERN(mixerGetLineControlsW);
DECLARE_ORIG_EXTERN(mixerGetLineInfoA);
DECLARE_ORIG_EXTERN(mixerGetLineInfoW);
DECLARE_ORIG_EXTERN(mixerGetNumDevs);
DECLARE_ORIG_EXTERN(mixerMessage);
DECLARE_ORIG_EXTERN(mixerOpen);
DECLARE_ORIG_EXTERN(mixerSetControlDetails);
DECLARE_ORIG_EXTERN(mmDrvInstall);
DECLARE_ORIG_EXTERN(mmGetCurrentTask);
DECLARE_ORIG_EXTERN(mmTaskBlock);
DECLARE_ORIG_EXTERN(mmTaskCreate);
DECLARE_ORIG_EXTERN(mmTaskSignal);
DECLARE_ORIG_EXTERN(mmTaskYield);
DECLARE_ORIG_EXTERN(mmioAdvance);
DECLARE_ORIG_EXTERN(mmioAscend);
DECLARE_ORIG_EXTERN(mmioClose);
DECLARE_ORIG_EXTERN(mmioCreateChunk);
DECLARE_ORIG_EXTERN(mmioDescend);
DECLARE_ORIG_EXTERN(mmioFlush);
DECLARE_ORIG_EXTERN(mmioGetInfo);
DECLARE_ORIG_EXTERN(mmioInstallIOProcA);
DECLARE_ORIG_EXTERN(mmioInstallIOProcW);
DECLARE_ORIG_EXTERN(mmioOpenA);
DECLARE_ORIG_EXTERN(mmioOpenW);
DECLARE_ORIG_EXTERN(mmioRead);
DECLARE_ORIG_EXTERN(mmioRenameA);
DECLARE_ORIG_EXTERN(mmioRenameW);
DECLARE_ORIG_EXTERN(mmioSeek);
DECLARE_ORIG_EXTERN(mmioSendMessage);
DECLARE_ORIG_EXTERN(mmioSetBuffer);
DECLARE_ORIG_EXTERN(mmioSetInfo);
DECLARE_ORIG_EXTERN(mmioStringToFOURCCA);
DECLARE_ORIG_EXTERN(mmioStringToFOURCCW);
DECLARE_ORIG_EXTERN(mmioWrite);
DECLARE_ORIG_EXTERN(mmsystemGetVersion);
DECLARE_ORIG_EXTERN(sndPlaySoundA);
DECLARE_ORIG_EXTERN(sndPlaySoundW);
DECLARE_ORIG_EXTERN(timeBeginPeriod);
DECLARE_ORIG_EXTERN(timeEndPeriod);
DECLARE_ORIG_EXTERN(timeGetDevCaps);
DECLARE_ORIG_EXTERN(timeGetSystemTime);
DECLARE_ORIG_EXTERN(timeGetTime);
DECLARE_ORIG_EXTERN(timeKillEvent);
DECLARE_ORIG_EXTERN(timeSetEvent);
DECLARE_ORIG_EXTERN(waveInAddBuffer);
DECLARE_ORIG_EXTERN(waveInClose);
DECLARE_ORIG_EXTERN(waveInGetDevCapsA);
DECLARE_ORIG_EXTERN(waveInGetDevCapsW);
DECLARE_ORIG_EXTERN(waveInGetErrorTextA);
DECLARE_ORIG_EXTERN(waveInGetErrorTextW);
DECLARE_ORIG_EXTERN(waveInGetID);
DECLARE_ORIG_EXTERN(waveInGetNumDevs);
DECLARE_ORIG_EXTERN(waveInGetPosition);
DECLARE_ORIG_EXTERN(waveInMessage);
DECLARE_ORIG_EXTERN(waveInOpen);
DECLARE_ORIG_EXTERN(waveInPrepareHeader);
DECLARE_ORIG_EXTERN(waveInReset);
DECLARE_ORIG_EXTERN(waveInStart);
DECLARE_ORIG_EXTERN(waveInStop);
DECLARE_ORIG_EXTERN(waveInUnprepareHeader);
DECLARE_ORIG_EXTERN(waveOutBreakLoop);
DECLARE_ORIG_EXTERN(waveOutClose);
DECLARE_ORIG_EXTERN(waveOutGetDevCapsA);
DECLARE_ORIG_EXTERN(waveOutGetDevCapsW);
DECLARE_ORIG_EXTERN(waveOutGetErrorTextA);
DECLARE_ORIG_EXTERN(waveOutGetErrorTextW);
DECLARE_ORIG_EXTERN(waveOutGetID);
DECLARE_ORIG_EXTERN(waveOutGetNumDevs);
DECLARE_ORIG_EXTERN(waveOutGetPitch);
DECLARE_ORIG_EXTERN(waveOutGetPlaybackRate);
DECLARE_ORIG_EXTERN(waveOutGetPosition);
DECLARE_ORIG_EXTERN(waveOutGetVolume);
DECLARE_ORIG_EXTERN(waveOutMessage);
DECLARE_ORIG_EXTERN(waveOutOpen);
DECLARE_ORIG_EXTERN(waveOutPause);
DECLARE_ORIG_EXTERN(waveOutPrepareHeader);
DECLARE_ORIG_EXTERN(waveOutReset);
DECLARE_ORIG_EXTERN(waveOutRestart);
DECLARE_ORIG_EXTERN(waveOutSetPitch);
DECLARE_ORIG_EXTERN(waveOutSetPlaybackRate);
DECLARE_ORIG_EXTERN(waveOutSetVolume);
DECLARE_ORIG_EXTERN(waveOutUnprepareHeader);
DECLARE_ORIG_EXTERN(waveOutWrite);

// Deferred init trigger (declared in dllmain.cpp / dllmain64.cpp)
extern void AutoInitializeUCOnline();
extern bool g_deferInitialization;

static inline void TryDeferredInit() {
    if (g_deferInitialization) {
        g_deferInitialization = false;
        AutoInitializeUCOnline();
    }
}

// ============================================================================
// Proxy stub functions
// Each is exported via the .def file under the real winmm function name.
// They forward all arguments to the original function via the stored pointer.
//
// For functions whose exact signature we don't need to know, we use a
// variadic-args approach: since all winmm functions are __stdcall, the
// caller pushes args right-to-left and the callee cleans the stack.
// We can safely forward by casting to the right type.
//
// For the most commonly used functions we use proper signatures.
// For rarely-used ones we use a generic 6-arg forwarder which is safe
// because extra args on the stack are harmless for __stdcall forwarding
// when the callee cleans exactly what it expects.
// ============================================================================

// Generic forwarder type - handles up to 6 DWORD_PTR args
typedef DWORD_PTR (WINAPI *GenericFunc6)(DWORD_PTR, DWORD_PTR, DWORD_PTR, DWORD_PTR, DWORD_PTR, DWORD_PTR);

// Macro for generic proxy with up to 6 args
#define PROXY_FUNC(name) \
    extern "C" DWORD_PTR WINAPI proxy_##name(DWORD_PTR a1, DWORD_PTR a2, DWORD_PTR a3, DWORD_PTR a4, DWORD_PTR a5, DWORD_PTR a6) { \
        TryDeferredInit(); \
        if (!orig_##name) return 0; \
        return ((GenericFunc6)orig_##name)(a1, a2, a3, a4, a5, a6); \
    }

// ---- Time functions (proper signatures) ----
extern "C" DWORD WINAPI proxy_timeGetTime() {
    TryDeferredInit();
    typedef DWORD (WINAPI *fn_t)();
    return orig_timeGetTime ? ((fn_t)orig_timeGetTime)() : 0;
}

extern "C" UINT WINAPI proxy_timeBeginPeriod(UINT uPeriod) {
    TryDeferredInit();
    typedef UINT (WINAPI *fn_t)(UINT);
    return orig_timeBeginPeriod ? ((fn_t)orig_timeBeginPeriod)(uPeriod) : 0;
}

extern "C" UINT WINAPI proxy_timeEndPeriod(UINT uPeriod) {
    TryDeferredInit();
    typedef UINT (WINAPI *fn_t)(UINT);
    return orig_timeEndPeriod ? ((fn_t)orig_timeEndPeriod)(uPeriod) : 0;
}

extern "C" UINT WINAPI proxy_timeGetDevCaps(void* ptc, UINT cbtc) {
    TryDeferredInit();
    typedef UINT (WINAPI *fn_t)(void*, UINT);
    return orig_timeGetDevCaps ? ((fn_t)orig_timeGetDevCaps)(ptc, cbtc) : 0;
}

extern "C" UINT WINAPI proxy_timeGetSystemTime(void* pmmt, UINT cbmmt) {
    TryDeferredInit();
    typedef UINT (WINAPI *fn_t)(void*, UINT);
    return orig_timeGetSystemTime ? ((fn_t)orig_timeGetSystemTime)(pmmt, cbmmt) : 0;
}

extern "C" UINT WINAPI proxy_timeSetEvent(UINT uDelay, UINT uResolution, void* fptc, DWORD_PTR dwUser, UINT fuEvent) {
    TryDeferredInit();
    typedef UINT (WINAPI *fn_t)(UINT, UINT, void*, DWORD_PTR, UINT);
    return orig_timeSetEvent ? ((fn_t)orig_timeSetEvent)(uDelay, uResolution, fptc, dwUser, fuEvent) : 0;
}

extern "C" UINT WINAPI proxy_timeKillEvent(UINT uTimerID) {
    TryDeferredInit();
    typedef UINT (WINAPI *fn_t)(UINT);
    return orig_timeKillEvent ? ((fn_t)orig_timeKillEvent)(uTimerID) : 0;
}

// ---- PlaySound (proper signatures) ----
extern "C" BOOL WINAPI proxy_PlaySoundA(LPCSTR pszSound, HMODULE hmod, DWORD fdwSound) {
    TryDeferredInit();
    typedef BOOL (WINAPI *fn_t)(LPCSTR, HMODULE, DWORD);
    return orig_PlaySoundA ? ((fn_t)orig_PlaySoundA)(pszSound, hmod, fdwSound) : FALSE;
}

extern "C" BOOL WINAPI proxy_PlaySoundW(LPCWSTR pszSound, HMODULE hmod, DWORD fdwSound) {
    TryDeferredInit();
    typedef BOOL (WINAPI *fn_t)(LPCWSTR, HMODULE, DWORD);
    return orig_PlaySoundW ? ((fn_t)orig_PlaySoundW)(pszSound, hmod, fdwSound) : FALSE;
}

extern "C" BOOL WINAPI proxy_sndPlaySoundA(LPCSTR pszSound, UINT fuSound) {
    TryDeferredInit();
    typedef BOOL (WINAPI *fn_t)(LPCSTR, UINT);
    return orig_sndPlaySoundA ? ((fn_t)orig_sndPlaySoundA)(pszSound, fuSound) : FALSE;
}

extern "C" BOOL WINAPI proxy_sndPlaySoundW(LPCWSTR pszSound, UINT fuSound) {
    TryDeferredInit();
    typedef BOOL (WINAPI *fn_t)(LPCWSTR, UINT);
    return orig_sndPlaySoundW ? ((fn_t)orig_sndPlaySoundW)(pszSound, fuSound) : FALSE;
}

// ---- All remaining functions use generic 6-arg forwarder ----
PROXY_FUNC(CloseDriver)
PROXY_FUNC(DefDriverProc)
PROXY_FUNC(DriverCallback)
PROXY_FUNC(DrvGetModuleHandle)
PROXY_FUNC(GetDriverModuleHandle)
PROXY_FUNC(OpenDriver)
PROXY_FUNC(SendDriverMessage)
PROXY_FUNC(auxGetDevCapsA)
PROXY_FUNC(auxGetDevCapsW)
PROXY_FUNC(auxGetNumDevs)
PROXY_FUNC(auxGetVolume)
PROXY_FUNC(auxOutMessage)
PROXY_FUNC(auxSetVolume)
PROXY_FUNC(joyConfigChanged)
PROXY_FUNC(joyGetDevCapsA)
PROXY_FUNC(joyGetDevCapsW)
PROXY_FUNC(joyGetNumDevs)
PROXY_FUNC(joyGetPos)
PROXY_FUNC(joyGetPosEx)
PROXY_FUNC(joyGetThreshold)
PROXY_FUNC(joyReleaseCapture)
PROXY_FUNC(joySetCapture)
PROXY_FUNC(joySetThreshold)
PROXY_FUNC(mciDriverNotify)
PROXY_FUNC(mciDriverYield)
PROXY_FUNC(mciExecute)
PROXY_FUNC(mciFreeCommandResource)
PROXY_FUNC(mciGetCreatorTask)
PROXY_FUNC(mciGetDeviceIDA)
PROXY_FUNC(mciGetDeviceIDW)
PROXY_FUNC(mciGetDeviceIDFromElementIDA)
PROXY_FUNC(mciGetDeviceIDFromElementIDW)
PROXY_FUNC(mciGetDriverData)
PROXY_FUNC(mciGetErrorStringA)
PROXY_FUNC(mciGetErrorStringW)
PROXY_FUNC(mciGetYieldProc)
PROXY_FUNC(mciLoadCommandResource)
PROXY_FUNC(mciSendCommandA)
PROXY_FUNC(mciSendCommandW)
PROXY_FUNC(mciSendStringA)
PROXY_FUNC(mciSendStringW)
PROXY_FUNC(mciSetDriverData)
PROXY_FUNC(mciSetYieldProc)
PROXY_FUNC(midiConnect)
PROXY_FUNC(midiDisconnect)
PROXY_FUNC(midiInAddBuffer)
PROXY_FUNC(midiInClose)
PROXY_FUNC(midiInGetDevCapsA)
PROXY_FUNC(midiInGetDevCapsW)
PROXY_FUNC(midiInGetErrorTextA)
PROXY_FUNC(midiInGetErrorTextW)
PROXY_FUNC(midiInGetID)
PROXY_FUNC(midiInGetNumDevs)
PROXY_FUNC(midiInMessage)
PROXY_FUNC(midiInOpen)
PROXY_FUNC(midiInPrepareHeader)
PROXY_FUNC(midiInReset)
PROXY_FUNC(midiInStart)
PROXY_FUNC(midiInStop)
PROXY_FUNC(midiInUnprepareHeader)
PROXY_FUNC(midiOutCacheDrumPatches)
PROXY_FUNC(midiOutCachePatches)
PROXY_FUNC(midiOutClose)
PROXY_FUNC(midiOutGetDevCapsA)
PROXY_FUNC(midiOutGetDevCapsW)
PROXY_FUNC(midiOutGetErrorTextA)
PROXY_FUNC(midiOutGetErrorTextW)
PROXY_FUNC(midiOutGetID)
PROXY_FUNC(midiOutGetNumDevs)
PROXY_FUNC(midiOutGetVolume)
PROXY_FUNC(midiOutLongMsg)
PROXY_FUNC(midiOutMessage)
PROXY_FUNC(midiOutOpen)
PROXY_FUNC(midiOutPrepareHeader)
PROXY_FUNC(midiOutReset)
PROXY_FUNC(midiOutSetVolume)
PROXY_FUNC(midiOutShortMsg)
PROXY_FUNC(midiOutUnprepareHeader)
PROXY_FUNC(midiStreamClose)
PROXY_FUNC(midiStreamOpen)
PROXY_FUNC(midiStreamOut)
PROXY_FUNC(midiStreamPause)
PROXY_FUNC(midiStreamPosition)
PROXY_FUNC(midiStreamProperty)
PROXY_FUNC(midiStreamRestart)
PROXY_FUNC(midiStreamStop)
PROXY_FUNC(mixerClose)
PROXY_FUNC(mixerGetControlDetailsA)
PROXY_FUNC(mixerGetControlDetailsW)
PROXY_FUNC(mixerGetDevCapsA)
PROXY_FUNC(mixerGetDevCapsW)
PROXY_FUNC(mixerGetID)
PROXY_FUNC(mixerGetLineControlsA)
PROXY_FUNC(mixerGetLineControlsW)
PROXY_FUNC(mixerGetLineInfoA)
PROXY_FUNC(mixerGetLineInfoW)
PROXY_FUNC(mixerGetNumDevs)
PROXY_FUNC(mixerMessage)
PROXY_FUNC(mixerOpen)
PROXY_FUNC(mixerSetControlDetails)
PROXY_FUNC(mmDrvInstall)
PROXY_FUNC(mmGetCurrentTask)
PROXY_FUNC(mmTaskBlock)
PROXY_FUNC(mmTaskCreate)
PROXY_FUNC(mmTaskSignal)
PROXY_FUNC(mmTaskYield)
PROXY_FUNC(mmioAdvance)
PROXY_FUNC(mmioAscend)
PROXY_FUNC(mmioClose)
PROXY_FUNC(mmioCreateChunk)
PROXY_FUNC(mmioDescend)
PROXY_FUNC(mmioFlush)
PROXY_FUNC(mmioGetInfo)
PROXY_FUNC(mmioInstallIOProcA)
PROXY_FUNC(mmioInstallIOProcW)
PROXY_FUNC(mmioOpenA)
PROXY_FUNC(mmioOpenW)
PROXY_FUNC(mmioRead)
PROXY_FUNC(mmioRenameA)
PROXY_FUNC(mmioRenameW)
PROXY_FUNC(mmioSeek)
PROXY_FUNC(mmioSendMessage)
PROXY_FUNC(mmioSetBuffer)
PROXY_FUNC(mmioSetInfo)
PROXY_FUNC(mmioStringToFOURCCA)
PROXY_FUNC(mmioStringToFOURCCW)
PROXY_FUNC(mmioWrite)
PROXY_FUNC(mmsystemGetVersion)
PROXY_FUNC(waveInAddBuffer)
PROXY_FUNC(waveInClose)
PROXY_FUNC(waveInGetDevCapsA)
PROXY_FUNC(waveInGetDevCapsW)
PROXY_FUNC(waveInGetErrorTextA)
PROXY_FUNC(waveInGetErrorTextW)
PROXY_FUNC(waveInGetID)
PROXY_FUNC(waveInGetNumDevs)
PROXY_FUNC(waveInGetPosition)
PROXY_FUNC(waveInMessage)
PROXY_FUNC(waveInOpen)
PROXY_FUNC(waveInPrepareHeader)
PROXY_FUNC(waveInReset)
PROXY_FUNC(waveInStart)
PROXY_FUNC(waveInStop)
PROXY_FUNC(waveInUnprepareHeader)
PROXY_FUNC(waveOutBreakLoop)
PROXY_FUNC(waveOutClose)
PROXY_FUNC(waveOutGetDevCapsA)
PROXY_FUNC(waveOutGetDevCapsW)
PROXY_FUNC(waveOutGetErrorTextA)
PROXY_FUNC(waveOutGetErrorTextW)
PROXY_FUNC(waveOutGetID)
PROXY_FUNC(waveOutGetNumDevs)
PROXY_FUNC(waveOutGetPitch)
PROXY_FUNC(waveOutGetPlaybackRate)
PROXY_FUNC(waveOutGetPosition)
PROXY_FUNC(waveOutGetVolume)
PROXY_FUNC(waveOutMessage)
PROXY_FUNC(waveOutOpen)
PROXY_FUNC(waveOutPause)
PROXY_FUNC(waveOutPrepareHeader)
PROXY_FUNC(waveOutReset)
PROXY_FUNC(waveOutRestart)
PROXY_FUNC(waveOutSetPitch)
PROXY_FUNC(waveOutSetPlaybackRate)
PROXY_FUNC(waveOutSetVolume)
PROXY_FUNC(waveOutUnprepareHeader)
PROXY_FUNC(waveOutWrite)
