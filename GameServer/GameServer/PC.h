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

public:
	SessionId GetSessionId() const { return pcDBID; }
	PCDBID GetPCId() const { return session.GetSessionId(); }

private:
	PCDBID pcDBID;

private:
	RIOSession& session;
};