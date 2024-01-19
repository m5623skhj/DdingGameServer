#pragma once

#include "LanServer.h"
#include "NetServer.h"
#include <string>

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

private:
	AuthLanServer authLanServer;
	AuthNetServer authNetServer;

	// DB와 통신할 채널이 하나 필요할 듯 함
	// 인증서버인 만큼, 여러개가 필요하진 않아 보이고, 1개만 있어도 충분할 듯
};