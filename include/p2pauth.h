//========= Copyright � 1996-2004, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#pragma once

#include <steam/steam_api.h>
#include <steam/isteamnetworking.h>
#include <steam/isteamuser.h>

const int k_cMaxSockets = 16;
const int MAX_PLAYERS_PER_SERVER = 64;

class CP2PAuthPlayer;

bool SendAuthTicketToConnection( CSteamID steamIDFrom, HSteamNetConnection hConnectionTo, uint32 cubTicket, uint8 *pubTicket );

// Forward declaration for game engine interface
class IGameEngine;

// Message structure for P2P ticket sending
struct MsgP2PSendingTicket_t
{
    CSteamID m_steamID;
    uint32 m_cubTicket;
    uint8 m_rgubTicket[1024];

    void SetSteamID(uint64 steamID) { m_steamID.SetFromUint64(steamID); }
    uint64 GetSteamID() const { return m_steamID.ConvertToUint64(); }
    void SetToken(const uint8* pubToken, uint32 cubToken) {
        m_cubTicket = std::min(cubToken, sizeof(m_rgubTicket));
        memcpy(m_rgubTicket, pubToken, m_cubTicket);
    }
    const uint8* GetTokenPtr() const { return m_rgubTicket; }
    uint32 GetTokenLen() const { return m_cubTicket; }
};

//-----------------------------------------------------------------------------
// Purpose: one player p2p auth process state machine
//-----------------------------------------------------------------------------
class CP2PAuthPlayer
{
public:
	CP2PAuthPlayer( IGameEngine *pGameEngine, CSteamID steamID, HSteamNetConnection hServerConn );
	~CP2PAuthPlayer();
	void EndGame();
	void StartAuthPlayer();
	bool BIsAuthOk();
	void HandleP2PSendingTicket( const MsgP2PSendingTicket_t *pMsg );

	CSteamID GetSteamID();

	STEAM_CALLBACK( CP2PAuthPlayer, OnBeginAuthResponse, ValidateAuthTicketResponse_t, m_CallbackBeginAuthResponse );

	const CSteamID m_steamID;
	const HSteamNetConnection m_hServerConnection;
private:
	uint64 GetGameTimeInSeconds()
	{
		return m_pGameEngine->GetGameTickCount()/1000;
	}
	bool m_bSentTicket;
	bool m_bSubmittedHisTicket;
	bool m_bHaveAnswer;
	uint64 m_ulConnectTime;
	uint64 m_ulTicketTime;
	uint64 m_ulAnswerTime;
	uint32 m_cubTicketIGaveThisUser;
	uint8 m_rgubTicketIGaveThisUser[1024];
	uint32 m_cubTicketHeGaveMe;
	uint8 m_rgubTicketHeGaveMe[1024];
	HAuthTicket m_hAuthTicketIGaveThisUser;
	EBeginAuthSessionResult m_eBeginAuthSessionResult;
	EAuthSessionResponse m_eAuthSessionResponse;

	IGameEngine *m_pGameEngine;
};

//-----------------------------------------------------------------------------
// Purpose: simple wrapper for multiple players
//-----------------------------------------------------------------------------
class CP2PAuthedGame
{
public:
	CP2PAuthedGame( IGameEngine *pGameEngine );
	void PlayerDisconnect( int iSlot );
	void EndGame();
	void StartAuthPlayer( int iSlot, CSteamID steamID );
	void RegisterPlayer( int iSlot, CSteamID steamID );
	void HandleP2PSendingTicket( const void *pMessage );
	CSteamID GetSteamID();
	void InternalInitPlayer( int iSlot, CSteamID steamID, bool bStartAuthProcess );

	CP2PAuthPlayer *m_rgpP2PAuthPlayer[MAX_PLAYERS_PER_SERVER];
	IGameEngine *m_pGameEngine;
	HSteamNetConnection m_hConnServer;
};
