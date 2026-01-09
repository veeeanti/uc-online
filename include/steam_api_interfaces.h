#pragma once

#include <cstdint>
#include <windows.h>

// Steam API interfaces and declarations
extern "C" {
    // Core Steam API functions
    bool SteamAPI_RestartAppIfNecessary(uint32_t appId);
    bool SteamAPI_Init();
    bool SteamAPI_InitEx(const char* pchVersion);
    void SteamAPI_Shutdown();
    void SteamAPI_RunCallbacks();
    bool SteamAPI_IsSteamRunning();
    
    // Steam Client
    void* SteamClient();
    bool SteamClient_Init(int32_t nAppId, bool bUsePipes);
    void SteamClient_Shutdown();
    
    // Steam User
    const char* SteamUser();
    uint32_t SteamUser_GetSteamID();
    const char* SteamUser_GetPersonaName();
    bool SteamUser_BLoggedOn();
    
    // Steam Apps
    bool SteamApps();
    bool SteamApps_BIsSubscribed();
    bool SteamApps_BIsSubscribedApp(uint32_t nAppID);
    bool SteamApps_BIsDlcInstalled(uint32_t nDLCID);
    uint32_t SteamApps_GetAppBuildId();
    bool SteamApps_GetAppInstallDir(uint32_t nAppID, char* pchFolder, int32_t cchFolder);
    bool SteamApps_GetAppOwnershipTicketData(uint32_t nAppID, void* pvBuffer, uint32_t cbBufferLength, uint32_t* picbTicket);
    
    // Steam Friends
    bool SteamFriends();
    int32_t SteamFriends_GetFriendCount(int32_t iFriendFlags);
    uint32_t SteamFriends_GetFriendByIndex(int32_t iFriend, int32_t iFriendFlags);
    const char* SteamFriends_GetFriendPersonaName(uint32_t steamIDFriend);
    bool SteamFriends_SetRichPresence(const char* pchKey, const char* pchValue);
    
    // Steam User Stats
    bool SteamUserStats();
    bool SteamUserStats_RequestCurrentStats();
    bool SteamUserStats_GetStat(const char* pchName, int32_t* pData);
    bool SteamUserStats_SetStat(const char* pchName, int32_t iData);
    bool SteamUserStats_StoreStats();
    
    // Steam Networking
    bool SteamNetworking();
    bool SteamNetworking_SendP2PPacket(uint32_t unRemoteSteamID, const void* pubData, uint32_t cubData, bool bReliable);
    bool SteamNetworking_IsP2PPacketAvailable(uint32_t* pcubMsgSize);
    bool SteamNetworking_ReadP2PPacket(void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize, uint32_t* psteamIDRemote);
    bool SteamNetworking_InitP2PConnection(uint32_t unRemoteSteamID);
    bool SteamNetworking_CloseP2PConnection(uint32_t unRemoteSteamID);
    
    // Steam Matchmaking
    bool SteamMatchmaking();
    void* SteamMatchmaking_CreateLobby(int32_t eLobbyType, int32_t cMaxMembers);
    bool SteamMatchmaking_JoinLobby(void* pLobby);
    bool SteamMatchmaking_LeaveLobby(void* pLobby);
    
    // Steam UGC
    bool SteamUGC();
    void* SteamUGC_CreateItem(void* pConsumerAppId, int32_t eFileType);
    bool SteamUGC_SubmitItemUpdate(void* hUpdate, const char* pchChangeNote);
    
    // Steam HTTP
    bool SteamHTTP();
    void* SteamHTTP_CreateHTTPRequest(int32_t eHTTPRequestMethod, const char* pchAbsoluteURL);
    bool SteamHTTP_SendHTTPRequest(void* hRequest);
    
    // Steam Remote Storage
    bool SteamRemoteStorage();
    bool SteamRemoteStorage_FileWrite(const char* pchFile, const void* pvData, int32_t cubData);
    int32_t SteamRemoteStorage_FileRead(const char* pchFile, void* pvData, int32_t cubDataToRead);
    
    // Steam Screenshots
    bool SteamScreenshots();
    void SteamScreenshots_HookScreenshots(bool bHook);
    
    // Steam Music
    bool SteamMusic();
    bool SteamMusic_BIsEnabled();
    
    // Steam Video
    bool SteamVideo();
    bool SteamVideo_BIsEnabled();
    
    // Steam Parenting
    bool SteamParenting();
    bool SteamParenting_BIsEnabled();
    
    // Steam Input
    bool SteamInput();
    bool SteamInput_Init();
    bool SteamInput_Shutdown();
    
    // Steam GameServer
    bool SteamGameServer();
    bool SteamGameServer_Init(uint32_t unIP, uint16_t usPort, uint16_t usGamePort, uint32_t unFlags, const char* pchVersionString);
    void SteamGameServer_Shutdown();
    
    // Steam GameServerStats
    bool SteamGameServerStats();
    bool SteamGameServerStats_RequestUserStats(uint32_t unSteamIDUser);
    
    // Steam HTML Surface
    bool SteamHTMLSurface();
    bool SteamHTMLSurface_Init();
    bool SteamHTMLSurface_Shutdown();
    
    // Steam Inventory
    bool SteamInventory();
    bool SteamInventory_GetResultStatus(void* hResult);
    
    // Steam Controller
    bool SteamController();
    bool SteamController_Init();
    bool SteamController_Shutdown();
    
    // Steam Remote Play
    bool SteamRemotePlay();
    bool SteamRemotePlay_BIsEnabled();
    
    // Steam Timeline
    bool SteamTimeline();
    bool SteamTimeline_BIsEnabled();
    
    // Steam PS3 Overlay Renderer
    bool SteamPS3OverlayRenderer();
    bool SteamPS3OverlayRenderer_BIsEnabled();
    
    // Steam DualSense
    bool SteamDualSense();
    bool SteamDualSense_BIsEnabled();
    
    // Steam Music Remote
    bool SteamMusicRemote();
    bool SteamMusicRemote_BIsEnabled();
    
    // Steam HTTP Request
    bool SteamHTTPRequest();
    bool SteamHTTPRequest_BIsEnabled();
    
    // Steam Networking Messages
    bool SteamNetworkingMessages();
    bool SteamNetworkingMessages_BIsEnabled();
    
    // Steam Networking Sockets
    bool SteamNetworkingSockets();
    bool SteamNetworkingSockets_BIsEnabled();
    
    // Steam Networking Utils
    bool SteamNetworkingUtils();
    bool SteamNetworkingUtils_BIsEnabled();
    
    // Steam Game Coordinator
    bool SteamGameCoordinator();
    bool SteamGameCoordinator_BIsEnabled();
    
    // Steam Game Server HTTP
    bool SteamGameServerHTTP();
    bool SteamGameServerHTTP_BIsEnabled();
    
    // Steam App List
    bool SteamAppList();
    bool SteamAppList_BIsEnabled();
}

// Function to check if a function is exported from a DLL
inline bool IsFunctionExported(HMODULE hModule, const char* functionName) {
    return GetProcAddress(hModule, functionName) != nullptr;
}

// Function to load Steam API DLL and check for required exports
inline HMODULE LoadSteamAPI(const char* dllPath) {
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