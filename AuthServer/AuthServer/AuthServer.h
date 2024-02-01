#pragma once

#include "LanServer.h"
#include "NetServer.h"
#include <string>
#include <unordered_map>
#include <set>
#include <shared_mutex>

class AuthLanServer : public CLanServer
{
public:
	bool Start(const std::wstring& optionFile);

private:
	virtual void OnClientJoin(UINT64 OutClientID);
	virtual void OnClientLeave(UINT64 ClientID);
	virtual bool OnConnectionRequest();

	virtual void OnRecv(UINT64 sessionId, CSerializationBuf* receivedBuffer);
	virtual void OnSend(UINT64 ClientID, int sendsize);

	virtual void OnWorkerThreadBegin();
	virtual void OnWorkerThreadEnd();
	virtual void OnError(st_Error* OutError);
};

class AuthNetServer : public CNetServer
{
public:
	bool Start(const std::wstring& optionFile);

private:
	virtual void OnClientJoin(UINT64 OutClientID);
	virtual void OnClientLeave(UINT64 ClientID);
	virtual bool OnConnectionRequest(const WCHAR* IP);

	virtual void OnRecv(UINT64 sessionId, CNetServerSerializationBuf* receivedBuffer);
	virtual void OnSend(UINT64 ClientID, int sendsize);

	virtual void OnWorkerThreadBegin();
	virtual void OnWorkerThreadEnd();
	virtual void OnError(st_Error* OutError);

	virtual void GQCSFailed(int LastError, UINT64 ClientID);
};

using UserId = UINT64;
using GameServerId = WORD;

class AuthServer
{
private:
	AuthServer() = default;
	~AuthServer() = default;

public:
	static AuthServer& GetInst()
	{
		static AuthServer instance;
		return instance;
	}

	bool StartAuthServer(const std::wstring& lanServerOptionFile, const std::wstring& netServerOptionFile);

public:
	void HandleC2AuthPacket(UINT64 sessionId, WORD packetType, CNetServerSerializationBuf& recvBuffer);
	void HandleGame2AuthPacket(UINT64 sessionId, WORD packetType, CSerializationBuf& recvBuffer);

	void OnGameServerConnected(UINT64 sessionId);
	void OnGameServerDisconnected(UINT64 sessionId);

private:
	bool SendPacketToLanClient(UINT64 sessionId, CSerializationBuf* packet);
	bool SendPacketToNetClient(UINT64 sessionId, CNetServerSerializationBuf* packet);

private:
	AuthLanServer authLanServer;
	AuthNetServer authNetServer;

private:
	struct UserInfo
	{
		UserId userId = 0;
		GameServerId nowInServerId = 0;
		// PCList?
	};

	std::unordered_map<UserId, UserInfo> userInfoMap;
	std::shared_mutex userInfoMapLock;

	std::unordered_map<UINT64, GameServerId> gameServerMap;
	std::shared_mutex gameServerMapLock;

#pragma region C2AuthProtocolHandler
private:
	void ClientLogin(UINT64 sessionId, std::string_view id, std::string_view password);
#pragma endregion C2AuthProtocolHandler

#pragma region Auth2GameProtocolHandler
private:
	void GameServerRegisterRequest(UINT64 sessionId, GameServerId gameServerId);
#pragma endregion Auth2GameProtocolHandler
};