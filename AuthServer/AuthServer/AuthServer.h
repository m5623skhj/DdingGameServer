#pragma once

#include "LanServer.h"
#include "NetServer.h"

class AuthLanServer : public CLanServer
{
	virtual void OnClientJoin(UINT64 OutClientID);
	virtual void OnClientLeave(UINT64 ClientID);
	virtual bool OnConnectionRequest();

	virtual void OnRecv(UINT64 ReceivedSessionID, CSerializationBuf* OutReadBuf);
	virtual void OnSend(UINT64 ClientID, int sendsize);

	virtual void OnWorkerThreadBegin();
	virtual void OnWorkerThreadEnd();
	virtual void OnError(st_Error* OutError);
};

class AuthNetServer : public CNetServer
{
	virtual void OnClientJoin(UINT64 OutClientID);
	virtual void OnClientLeave(UINT64 ClientID);
	virtual bool OnConnectionRequest(const WCHAR* IP);

	virtual void OnRecv(UINT64 ReceivedSessionID, CNetServerSerializationBuf* OutReadBuf);
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
};