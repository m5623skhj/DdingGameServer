#pragma once
#include "Creature.h"
#include "GameServerDefine.h"
#include <memory>
#include "DefineType.h"
#include "RIOSession.h"

class PC : public Creature
{
public:
	PC() = delete;
	virtual ~PC() = default;
	explicit PC(RIOSession& inSession, PCDBID inPCId);

public:
	void SendPacket(IGameAndClientPacket& packet);
	void SendPacket(NetBuffer& packet);
	void SendPacketAndDisconnect(IGameAndClientPacket& packet);
	void SendPacketAndDisconnect(NetBuffer& packet);

	void Disconnect();

public:
	SessionId GetSessionId() const { return pcDBID; }
	PCDBID GetPCId() const { return session.GetSessionId(); }

public:
	virtual void OnEnterWorld();
	virtual void OnLeaveWorld();
	virtual void OnDamaged();
	virtual void OnDied();
	virtual void OnRevived();

private:
	PCDBID pcDBID;

private:
	RIOSession& session;
};